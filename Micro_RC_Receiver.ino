// 4 Channel "Micro RC" Receiver with 4 standard RC Servo Outputs
// ATMEL Mega 328P TQFP 32 soldered directly to the board, 8MHz external resonator,
// 2.4GHz NRF24L01 SMD radio module, TB6612FNG dual dc motor driver

// * * * * N O T E ! The vehicle specific configurations are stored in "vehicleConfig.h" * * * *

const float codeVersion = 1.4; // Software revision

//
// =======================================================================================================
// BUILD OPTIONS (comment out unneeded options)
// =======================================================================================================
//

//#define DEBUG // if not commented out, Serial.print() is active! For debugging only!!

//
// =======================================================================================================
// INCLUDE LIRBARIES
// =======================================================================================================
//

// Libraries
#include <SPI.h>
#include <RF24.h> // Installed via Tools > Board > Boards Manager > Type RF24
#include <printf.h>
#include <Servo.h>
#include <statusLED.h> // https://github.com/TheDIYGuy999/statusLED
#include <TB6612FNG.h> // https://github.com/TheDIYGuy999/TB6612FNG
// Optional for motor PWM frequency adjustment:
#include <PWMFrequency.h> // https://github.com/kiwisincebirth/Arduino/tree/master/libraries/PWMFrequency

// Tabs (header files in sketch directory)
#include "readVCC.h"
#include "vehicleConfig.h"

//
// =======================================================================================================
// PIN ASSIGNMENTS & GLOBAL VARIABLES
// =======================================================================================================
//

// Radio channels (126 channels are supported)
byte chPointer = 0; // Channel 1 (the first entry of the array) is active by default
const byte NRFchannel[] {
  1, 2
};

// the ID number of the used "radio pipe" must match with the selected ID on the transmitter!
// 10 ID's are available @ the moment
const uint64_t pipeIn[] = {
  0xE9E8F0F0B1LL, 0xE9E8F0F0B2LL, 0xE9E8F0F0B3LL, 0xE9E8F0F0B4LL, 0xE9E8F0F0B5LL,
  0xE9E8F0F0B6LL, 0xE9E8F0F0B7LL, 0xE9E8F0F0B8LL, 0xE9E8F0F0B9LL, 0xE9E8F0F0B0LL
};
const int maxVehicleNumber = (sizeof(pipeIn) / (sizeof(uint64_t)));

// Hardware configuration: Set up nRF24L01 radio on hardware SPI bus & pins 8 (CE) & 7 (CSN)
RF24 radio(8, 7);

// The size of this struct should not exceed 32 bytes
struct RcData {
  byte axis1; // Aileron (Steering for car)
  byte axis2; // Elevator
  byte axis3; // Throttle
  byte axis4; // Rudder
  boolean mode1 = false; // Mode1 (toggle speed limitation)
  boolean mode2 = false; // Mode2 (toggle acc. / dec. limitation)
  boolean momentary1 = false; // Momentary push button
  byte pot1; // Potentiometer
};
RcData data;

// This struct defines data, which are embedded inside the ACK payload
struct ackPayload {
  float vcc; // vehicle vcc voltage
  float batteryVoltage; // vehicle battery voltage
  boolean batteryOk = true; // the vehicle battery voltage is OK!
  byte channel = 1; // the channel number
};
ackPayload payload;

// Battery voltage detection pin
#define BATTERY_DETECT_PIN A7 // The 20k (to battery) & 10k (to GND) battery detection voltage divider is connected to pin A7

// Create Servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Special functions
#define DIGITAL_OUT_1 1 // 1 = TXO Pin

// TB6612FNG H-Bridge
#define motor1_in1 4 // define motor pin numbers
#define motor1_in2 9
#define motor1_pwm 6

#define motor2_in1 3
#define motor2_in2 2
#define motor2_pwm 5

// Headlight off delay
unsigned long millisLightOff = 0;

// SYNTAX: IN1, IN2, PWM, min. input value, max. input value, neutral position width
// invert rotation direction true or false
TB6612FNG Motor1(motor1_in1, motor1_in2, motor1_pwm, 0, 100, 6, false); // Drive motor
TB6612FNG Motor2(motor2_in1, motor2_in2, motor2_pwm, 0, 100, 2, false); // Steering motor

// Status LED objects
statusLED tailLight(false); // "false" = output not inverted
statusLED headLight(false);

//
// =======================================================================================================
// MAIN ARDUINO SETUP (1x during startup)
// =======================================================================================================
//

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
  printf_begin();
  delay(3000);
#endif

  // Vehicle setup
  setupVehicle();

  // LED setup
  if (tailLights) tailLight.begin(A1); // A1 = Servo 2 Pin
  if (headLights) headLight.begin(0); // 0 = RXI Pin

  // Radio setup
  radio.begin();
  radio.setChannel(NRFchannel[chPointer]);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(pipeIn[vehicleNumber - 1], true); // Ensure autoACK is enabled
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(5, 5);                  // 5x250us delay (blocking!!), max. 5 retries
  //radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance

#ifdef DEBUG
  radio.printDetails();
  delay(3000);
#endif

  radio.openReadingPipe(1, pipeIn[vehicleNumber - 1]);
  radio.startListening();

  // Servo pins
  servo1.attach(A0);
  if (!tailLights) servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3);

  // All axes to neutral position
  data.axis1 = 50;
  data.axis2 = 50;
  data.axis3 = 50;
  data.axis4 = 50;

  // Special functions
  if (TXO_momentary1) pinMode(DIGITAL_OUT_1, OUTPUT);

  // Motor PWM frequency (Requires the PWMFrequency.h library)
  // ----------- DOES NOT WORK WITH Mega 328P, BECAUSE millis() IS AFFECTED!! --------------
  //setPWMPrescaler(motor1_pwm, 1); // 123Hz = 256,  492Hz = 64, 3936Hz = 8, 31488Hz = 1
  //setPWMPrescaler(motor2_pwm, 1);
}

//
// =======================================================================================================
// LED
// =======================================================================================================
//

void led() {

  // Lights are switching off 10s after the vehicle did stop
  if (millis() - millisLightOff >= 10000) {
    headLight.off(); // Headlight off
    tailLight.off(); // Taillight off
  }
  else {
    headLight.on();
    if (Motor1.brakeActive()) { // if braking detected from TB6612FNG motor driver
      tailLight.on(); // Brake light (full brightness)
    }
    else {
      tailLight.flash(10, 14, 0, 0); // Taillight: 10 on  / 14 off = about 40% brightness (soft PWM)
    }
  }
}

//
// =======================================================================================================
// READ RADIO DATA
// =======================================================================================================
//

void readRadio() {

  static unsigned long lastRecvTime = 0;
  byte pipeNo;

  if (radio.available(&pipeNo)) {
    radio.writeAckPayload(pipeNo, &payload, sizeof(struct ackPayload) );  // prepare the ACK payload
    radio.read(&data, sizeof(struct RcData)); // read the radia data and send out the ACK payload
    lastRecvTime = millis();
#ifdef DEBUG
    Serial.print(data.axis1);
    Serial.print("\t");
    Serial.print(data.axis2);
    Serial.print("\t");
    Serial.print(data.axis3);
    Serial.print("\t");
    Serial.print(data.axis4);
    Serial.println("\t");
#endif
  }

  // Switch channel
  if (millis() - lastRecvTime > 500) {
    chPointer ++;
    if (chPointer >= sizeof((*NRFchannel) / sizeof(byte))) chPointer = 0;
    radio.setChannel(NRFchannel[chPointer]);
    payload.channel = NRFchannel[chPointer];
  }

  if (millis() - lastRecvTime > 1000) { // set all analog values to their middle position, if no RC signal is received during 1s!
    data.axis1 = 50; // Aileron (Steering for car)
    data.axis2 = 50; // Elevator
    data.axis3 = 50; // Throttle
    data.axis4 = 50; // Rudder
#ifdef DEBUG
    Serial.println("No Radio Available - Check Transmitter!");
#endif
  }
}

//
// =======================================================================================================
// WRITE SERVO POSITIONS
// =======================================================================================================
//

void writeServos() {
  servo1.write(map(data.axis1, 100, 0, lim1L, lim1R) ); // 45 - 135°
  if (!tailLights) servo2.write(map(data.axis2, 100, 0, lim2L, lim2R) ); // 45 - 135°
  servo3.write(map(data.axis3, 100, 0, lim3L, lim3R) ); // 45 - 135°
  servo4.write(map(data.axis4, 100, 0, lim4L, lim4R) ); // 45 - 135°
}

//
// =======================================================================================================
// DRIVE MOTORS
// =======================================================================================================
//

void driveMotors() {

  int maxPWM;
  byte maxAcceleration;

  // Speed limitation (max. is 255)
  if (data.mode1) {
    maxPWM = maxPWMlimited; // Limited
  } else {
    maxPWM = maxPWMfull; // Full
  }

  if (!payload.batteryOk && liPo) maxPWM = 0; // Stop the vehicle, if the battery is empty!

  // Acceleration & deceleration limitation (ms per 1 step input signal change)
  if (data.mode2) {
    maxAcceleration = maxAccelerationLimited; // Limited
  } else {
    maxAcceleration = maxAccelerationFull; // Full
  }

  // ***************** Note! The ramptime is intended to protect the gearbox! *******************
  // SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
  // false = brake in neutral position inactive

  if (Motor1.drive(data.axis3, maxPWM, maxAcceleration, true) ) { // The drive motor (function returns true, if not in neutral)
    millisLightOff = millis(); // Reset the headlight delay timer, if the vehicle is driving!
  }

  Motor2.drive(data.axis1, steeringTorque, 0, false); // The steering motor (if the original steering motor is reused instead of a servo)
}

//
// =======================================================================================================
// WRITE DIGITAL OUTPUTS (SPECIAL FUNCTIONS)
// =======================================================================================================
//

void digitalOutputs() {
  if (TXO_momentary1) { // only, if function is enabled in vehicle configuration
    if (data.momentary1) digitalWrite(DIGITAL_OUT_1, HIGH);
    else digitalWrite(DIGITAL_OUT_1, LOW);
  }
}

//
// =======================================================================================================
// CHECK RX BATTERY % VCC VOLTAGES
// =======================================================================================================
//

void checkBattery() {

  // Every 2000 ms
  static unsigned long lastTrigger;
  if (millis() - lastTrigger >= 2000) {
    lastTrigger = millis();

    // Read both averaged voltages
    payload.batteryVoltage = batteryAverage();
    payload.vcc = vccAverage();

    if (battSense) { // Observe battery voltage
      if (!payload.batteryVoltage >= cutoffVoltage) payload.batteryOk = false;
    }
    else { // Observe vcc voltage
      if (!payload.vcc >= cutoffVoltage) payload.batteryOk = false;
    }
  }
}

// Voltage read & averaging subfunctions -----------------------------------------
// vcc ----
float vccAverage() {
  static int raw[6];

  if (raw[0] == 0) {
    for (int i = 0; i <= 5; i++) {
      raw[i] = readVcc(); // Init array
    }
  }

  raw[5] = raw[4];
  raw[4] = raw[3];
  raw[3] = raw[2];
  raw[2] = raw[1];
  raw[1] = raw[0];
  raw[0] = readVcc();
  float average = (raw[0] + raw[1] + raw[2] + raw[3] + raw[4] + raw[5]) / 6000.0;
  return average;
}

// battery ----
float batteryAverage() {
  static int raw[8];

  if (!battSense) return 0;

  if (raw[0] == 0) {
    for (int i = 0; i <= 7; i++) {
      raw[i] = analogRead(BATTERY_DETECT_PIN); // Init array
    }
  }

  raw[7] = raw[6];
  raw[6] = raw[5];
  raw[5] = raw[4];
  raw[4] = raw[3];
  raw[3] = raw[2];
  raw[2] = raw[1];
  raw[1] = raw[0];
  raw[0] = analogRead(BATTERY_DETECT_PIN);
  float average = (raw[0] + raw[1] + raw[2] + raw[3] + raw[4] + raw[5] + raw[6] + raw[7]) / 826.666; // 1023steps / 9.9V * 8 = 826.666
  return average;
}

//
// =======================================================================================================
// MAIN LOOP
// =======================================================================================================
//

void loop() {

  // Read radio data from transmitter
  readRadio();

  // Write the servo positions
  writeServos();

  // Drive the motors
  driveMotors();

  // Digital Outputs (special functions)
  digitalOutputs();
  
  // LED
  led();

  // Battery check
  checkBattery();
}

