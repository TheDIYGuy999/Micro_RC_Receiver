// 4 Channel "Micro RC" Receiver with 4 standard RC Servo Outputs
// ATMEL Mega 328P TQFP 32 soldered directly to the board, 8MHz external resonator,
// 2.4GHz NRF24L01 SMD radio module, TB6612FNG dual dc motor driver
// An MPU-6050 gyro / accelerometer can be used for MRSC stability control or self balancing robots
// SBUS output on pin TXO

// See: https://www.youtube.com/playlist?list=PLGO5EJJClJBCjIvu8frS7LrEU3H2Yz_so

// * * * * N O T E ! The vehicle specific configurations are stored in "vehicleConfig.h" * * * *

const float codeVersion = 3.9;  // Software revision (see https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/README.md)

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
#include <RF24.h>  // Installed via Sketch > Include Library > Manage Libraries > Type "RF24" (use V1.3.3!)
#include <printf.h>
#include <Servo.h>
#include <TB6612FNG.h>     // https://github.com/TheDIYGuy999/TB6612FNG ***NOTE*** V1.2 required!! <<<-----
#include <PWMFrequency.h>  // https://github.com/TheDIYGuy999/PWMFrequency

// Tabs (header files in sketch directory)
#include "readVCC.h"
#include "vehicleConfig.h"
#include "steeringCurves.h"
#include "helper.h"
#include "pgmRead64.h"  // Read 64 bit blocks from PROGMEM

//
// =======================================================================================================
// PIN ASSIGNMENTS & GLOBAL VARIABLES
// =======================================================================================================
//

// Radio channels (126 channels are supported)
byte chPointer = 0;  // Channel 1 (the first entry of the array) is active by default
const byte NRFchannel[]{
  1, 2
};

// the ID number of the used "radio pipe" must match with the selected ID on the transmitter!
// 20 ID's are available @ the moment
const uint64_t pipeIn[] PROGMEM = {
  0xE9E8F0F0B1LL, 0xE9E8F0F0B2LL, 0xE9E8F0F0B3LL, 0xE9E8F0F0B4LL, 0xE9E8F0F0B5LL,
  0xE9E8F0F0B6LL, 0xE9E8F0F0B7LL, 0xE9E8F0F0B8LL, 0xE9E8F0F0B9LL, 0xE9E8F0F0B0LL,
  0xE9E8F0F0C1LL, 0xE9E8F0F0C2LL, 0xE9E8F0F0C3LL, 0xE9E8F0F0C4LL, 0xE9E8F0F0C5LL,
  0xE9E8F0F0C6LL, 0xE9E8F0F0C7LL, 0xE9E8F0F0C8LL, 0xE9E8F0F0C9LL, 0xE9E8F0F0C0LL
};
const int maxVehicleNumber = (sizeof(pipeIn) / (sizeof(uint64_t)));

// Hardware configuration: Set up nRF24L01 radio on hardware SPI bus & pins 8 (CE) & 7 (CSN)
RF24 radio(8, 7);

// The size of this struct should not exceed 32 bytes
struct RcData {
  byte axis1;                  // Tilt
  byte axis2;                  // Lift
  byte axis3;                  // Throttle
  byte axis4;                  // Steering
  boolean mode1 = false;       // Mode1 (toggle speed limitation)
  boolean mode2 = false;       // Mode2 (toggle acc. / dec. limitation)
  boolean momentary1 = false;  // Momentary push button
  byte pot1;                   // Potentiometer
};
RcData data;

// This struct defines data, which are embedded inside the ACK payload
struct ackPayload {
  float vcc;                 // vehicle vcc voltage
  float batteryVoltage;      // vehicle battery voltage
  boolean batteryOk = true;  // the vehicle battery voltage is OK!
  byte channel = 1;          // the channel number
};
ackPayload payload;

// Battery voltage detection pin
#define BATTERY_DETECT_PIN A7  // The 20k (to battery) & 10k (to GND) battery detection voltage divider is connected to pin A7

// Create Servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Special functions
#define DIGITAL_OUT_1 1  // 1 = TXO Pin

// Headlight off delay
unsigned long millisLightOff = 0;

// Indicators
boolean left;
boolean right;
boolean hazard;

// Motors
boolean isDriving;  // is the vehicle driving?

// Motor objects
TB6612FNG Motor1;
TB6612FNG Motor2;

// Serial commands to light and sound controller
boolean serialCommands;

// ESC variables for tracked and half tracked mode
int lEsc;
int rEsc;

//
// =======================================================================================================
// RADIO SETUP
// =======================================================================================================
//

void setupRadio() {
  radio.begin();
  radio.setChannel(NRFchannel[chPointer]);

  // Set Power Amplifier (PA) level to one of four levels: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setPALevel(RF24_PA_HIGH);  // HIGH

  radio.setDataRate(RF24_250KBPS);
  //radio.setAutoAck(pipeIn[vehicleNumber - 1], true); // Ensure autoACK is enabled
  radio.setAutoAck(pgm_read_64(&pipeIn, vehicleNumber - 1), true);  // Ensure autoACK is enabled
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(5, 5);  // 5x250us delay (blocking!!), max. 5 retries
  //radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance

#ifdef DEBUG
  radio.printDetails();
  delay(3000);
#endif

  //radio.openReadingPipe(1, pipeIn[vehicleNumber - 1]);
  radio.openReadingPipe(1, pgm_read_64(&pipeIn, vehicleNumber - 1));
  radio.startListening();
}

//
// =======================================================================================================
// MOTOR DRIVER SETUP
// =======================================================================================================
//

void setupMotors() {

  // TB6612FNG H-Bridge pins
  // ---- IMPORTANT ---- The pin assignment depends on your board revision and is switched here to match!
  const byte motor1_in1 = 4;
  const byte motor1_in2 = 9;
  const byte motor1_pwm = 6;

  byte motor2_in1;
  const byte motor2_in2 = 2;
  byte motor2_pwm;

  // Switchable pins:
  if (boardVersion >= 1.3) {  // Board version >= 1.3:
    motor2_in1 = 5;           // 5
    motor2_pwm = 3;           // 3
  } else {                    // Board Version < 1.3:
    motor2_in1 = 3;           // 3
    motor2_pwm = 5;           // 5
  }

  // SYNTAX: IN1, IN2, PWM, min. input value, max. input value, neutral position width
  // invert rotation direction true or false
  Motor1.begin(motor1_in1, motor1_in2, motor1_pwm, 0, 100, 4, false);  // Drive motor
  Motor2.begin(motor2_in1, motor2_in2, motor2_pwm, 0, 100, 4, false);  // Steering motor (Drive in "HP" version)

  // Motor PWM frequency prescalers (Requires the PWMFrequency.h library)
  // Differential steering vehicles: locked to 984Hz, to make sure, that both motors use 984Hz.
  if (vehicleType == 1 || vehicleType == 2 || vehicleType == 6) pwmPrescaler2 = 32;

  // ----------- IMPORTANT!! --------------
  // Motor 1 always runs @ 984Hz PWM frequency and can't be changed, because timers 0 an 1 are in use for other things!
  // Motor 2 (pin 3) can be changed to the following PWM frequencies: 32 = 984Hz, 8 = 3936Hz, 1 = 31488Hz
  setPWMPrescaler(3, pwmPrescaler2);  // pin 3 is hardcoded, because we can't change all others anyway
}

//
// =======================================================================================================
// MAIN ARDUINO SETUP (1x during startup)
// =======================================================================================================
//

void setup() {

  // All axes to neutral position
  data.axis1 = 50;
  data.axis2 = 50;
  data.axis3 = 50;
  data.axis4 = 50;
  data.pot1 = 50;
  data.mode1 = true;
  data.mode2 = true;
  data.momentary1 = false;

#ifdef DEBUG
  Serial.begin(115200);
  printf_begin();
  serialCommands = false;
  delay(3000);
#endif

  // Radio setup
  setupRadio();

  // Servo pins
  servo1.attach(A0);
  servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3);

  // Special functions
  if (TXO_momentary1 || TXO_toggle1) pinMode(DIGITAL_OUT_1, OUTPUT);

  // Motor driver setup
  setupMotors();
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
    radio.writeAckPayload(pipeNo, &payload, sizeof(struct ackPayload));  // prepare the ACK payload
    radio.read(&data, sizeof(struct RcData));                            // read the radia data and send out the ACK payload
    hazard = false;
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
    chPointer++;
    if (chPointer >= sizeof((*NRFchannel) / sizeof(byte))) chPointer = 0;
    radio.setChannel(NRFchannel[chPointer]);
    payload.channel = NRFchannel[chPointer];
  }

  if (millis() - lastRecvTime > 1000) {  // set all analog values to their middle position, if no RC signal is received during 1s!
    data.axis1 = 50;                     // Aileron (Steering for car)
    data.axis2 = 50;                     // Elevator
    data.axis3 = 50;                     // Throttle
    data.axis4 = 50;                     // Rudder
    hazard = true;                       // Enable hazard lights
    payload.batteryOk = true;            // Clear low battery alert (allows to re-enable the vehicle, if you switch off the transmitter)
#ifdef DEBUG
    Serial.println("No Radio Available - Check Transmitter!");
#endif
  }

  if (millis() - lastRecvTime > 2000) {
    setupRadio();  // re-initialize radio
    lastRecvTime = millis();
  }
}

//
// =======================================================================================================
// WRITE SERVO POSITIONS
// =======================================================================================================
//

void writeServos() {

  // lift ----------------------------
  servo2.write(map(data.axis2, 100, 0, lim2L, lim2R));

  // steering servo ----------------------------
  servo4.write(map(data.axis4, 100, 0, lim4L, lim4R));
}

//
// =======================================================================================================
// "STEERING" MOTOR DRIVING FUNCTION (throttle & steering overlay for caterpillar vehicles)
// =======================================================================================================
//

void driveMotorsSteering() {

  int pwm[2];

  // The steering overlay
  int steeringFactorLeft;
  int steeringFactorRight;
  int steeringFactorLeft2;
  int steeringFactorRight2;

  // The input signal range
  const int servoMin = 5;
  const int servoMax = 95;
  const int servoNeutralMin = 48;
  const int servoNeutralMax = 52;

  // Compute steering overlay:
  // The steering signal is channel 4 = data.axis4
  // 100% = wheel spins with 100% of the requested speed forward
  // -100% = wheel spins with 100% of the requested speed backward
  if (data.axis4 <= servoNeutralMin) {
    steeringFactorLeft = map(data.axis4, servoMin, servoNeutralMin, 0, 100);
    steeringFactorLeft = constrain(steeringFactorLeft, 0, 100);
  } else {
    steeringFactorLeft = 100;
  }

  if (data.axis4 >= servoNeutralMax) {
    steeringFactorRight = map(data.axis4, servoMax, servoNeutralMax, 0, 100);
    steeringFactorRight = constrain(steeringFactorRight, 0, 100);
  } else {
    steeringFactorRight = 100;
  }

  // Nonlinear steering overlay correction
  steeringFactorLeft2 = reMap(curveForklift, steeringFactorLeft);  // Caterpillar mode
  steeringFactorRight2 = reMap(curveForklift, steeringFactorRight);


  // Drive caterpillar motors
  // The throttle signal (for both caterpillars) is channel 3 = data.axis3
  // -100 to 100%
  pwm[0] = map(data.axis3, servoMin, servoMax, 100, -100) * steeringFactorRight2 / 100;
  pwm[1] = map(data.axis3, servoMin, servoMax, 100, -100) * steeringFactorLeft2 / 100;

  pwm[0] = map(pwm[0], 100, -100, 100, 0);  // convert -100 to 100% to 0-100 for motor control
  pwm[1] = map(pwm[1], 100, -100, 100, 0);

  lEsc = pwm[1];  // Output for dual ESC
  rEsc = pwm[0];

  // right motor ----------------------------
  servo1.write(map(rEsc, 0, 100, lim1L, lim1R));

  // left motor ----------------------------
  servo3.write(map(lEsc, 0, 100, lim3L, lim3R));
}

//
// =======================================================================================================
// DRIVE MOTORS FORKLIFT (lifting)
// =======================================================================================================
//

void driveMotors() {

  // SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
  // false = brake in neutral position inactive

  Motor2.drive(data.axis1, 0, steeringTorque, 0, false);  // The tower tilting motor
}

//
// =======================================================================================================
// WRITE DIGITAL OUTPUTS (SPECIAL FUNCTIONS)
// =======================================================================================================
//

void digitalOutputs() {

  static boolean wasPressed;

  if (TXO_momentary1) {  // only, if momentary function is enabled in vehicle configuration
    if (data.momentary1) {
      digitalWrite(DIGITAL_OUT_1, HIGH);
    } else digitalWrite(DIGITAL_OUT_1, LOW);
  }

  if (TXO_toggle1) {  // only, if toggle function is enabled in vehicle configuration

    if (data.momentary1 && !wasPressed) {
      digitalWrite(DIGITAL_OUT_1, !digitalRead(DIGITAL_OUT_1));
      wasPressed = true;
    }
    if (!data.momentary1) wasPressed = false;
  }
}

//
// =======================================================================================================
// CHECK RX BATTERY & VCC VOLTAGES
// =======================================================================================================
//

bool battSense;

void checkBattery() {

  if (boardVersion < 1.2) battSense = false;
  else battSense = true;

  // switch between load and no load contition
  if (millis() - millisLightOff >= 1000) {  // one s after the vehicle did stop
    isDriving = false;                      // no load
  } else {
    isDriving = true;  // under load
  }

  // Every 1000 ms, take measurements
  static unsigned long lastTrigger;
  if (millis() - lastTrigger >= 1000) {
    lastTrigger = millis();

    // Read both averaged voltages
    payload.batteryVoltage = batteryAverage();
    payload.vcc = vccAverage();

    if (battSense) {  // Observe battery voltage
      if (payload.batteryVoltage <= cutoffVoltage) payload.batteryOk = false;
    } else {  // Observe vcc voltage
      if (payload.vcc <= cutoffVoltage) payload.batteryOk = false;
    }
  }
}

// Voltage read & averaging subfunctions -----------------------------------------
// vcc ----
float vccAverage() {
  static int raw[6];

  if (raw[0] == 0) {
    for (int i = 0; i <= 5; i++) {
      raw[i] = readVcc();  // Init array
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
  static int raw[6];

  if (!battSense) return 0;

  if (raw[0] == 0) {
    for (int i = 0; i <= 5; i++) {
      raw[i] = analogRead(BATTERY_DETECT_PIN);  // Init array
    }
  }

  raw[5] = raw[4];
  raw[4] = raw[3];
  raw[3] = raw[2];
  raw[2] = raw[1];
  raw[1] = raw[0];
  if (isDriving && HP) raw[0] = (analogRead(BATTERY_DETECT_PIN) + 31);              // add 0.3V while driving (HP version only): 1023 steps * 0.3V / 9.9V = 31
  else raw[0] = analogRead(BATTERY_DETECT_PIN);                                     // else take the real voltage (compensates voltage drop while driving)
  float average = (raw[0] + raw[1] + raw[2] + raw[3] + raw[4] + raw[5]) / 619.999;  // 1023steps / 9.9V * 6 = 619.999
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

  // ESC speed signals with steering overlay
  driveMotorsSteering();

  // Internal TB6612-FNG motor driver
  driveMotors();

  // Battery check
  checkBattery();

  // Digital Outputs (special functions)
  digitalOutputs();
}
