// 4 Channel Micro RC Receiver with 4 standard RC Servo Outputs
// ATMEL Mega 328P TQFP 32 soldered directly to the board, 8MHz external resonator,
// 2.4GHz NRF24L01 SMD radio module, TB6612FNG dual dc motor driver

// Used in:
// - Maisto Mustang Fastback (vehicle number 3)

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
#include <SimpleTimer.h> // https://github.com/jfturcot/SimpleTimer
#include <statusLED.h> // https://github.com/TheDIYGuy999/statusLED

#include <TB6612FNG.h> // https://github.com/TheDIYGuy999/TB6612FNG
// Optional for motor PWM frequency adjustment:
#include <PWMFrequency.h> // https://github.com/kiwisincebirth/Arduino/tree/master/libraries/PWMFrequency

#include "readVCC.h"

//
// =======================================================================================================
// PIN ASSIGNMENTS & GLOBAL VARIABLES
// =======================================================================================================
//

// Battery type
static boolean liPo = true;
static byte cutoffVoltage = 3.0;

// Vehicle address
int vehicleNumber = 3; // This number must be unique for each vehicle!
const int maxVehicleNumber = 5;

// the ID number of the used "radio pipe" must match with the selected ID on the transmitter!
const uint64_t pipeIn[maxVehicleNumber] = { 0xE9E8F0F0B1LL, 0xE9E8F0F0B2LL, 0xE9E8F0F0B3LL, 0xE9E8F0F0B4LL, 0xE9E8F0F0B5LL };

// Hardware configuration: Set up nRF24L01 radio on hardware SPI bus & pins 8 (CE) & 7 (CSN)
RF24 radio(8, 7);

// The size of this struct should not exceed 32 bytes
struct RcData {
  byte axis1; // Aileron (Steering for car)
  byte axis2; // Elevator
  byte axis3; // Throttle
  byte axis4; // Rudder
  boolean mode1 = false; // Speed limitation
  boolean mode2 = false;
};
RcData data;

// This struct defines data, which are embedded inside the ACK payload
struct ackPayload {
  float vcc; // vehicle vcc voltage
  float batteryVoltage; // vehicle battery voltage
  boolean batteryOk = true; // the vehicle battery voltage is OK!
};
ackPayload payload;

// Create Servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Initialize TB6612FNG H-Bridge
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
statusLED battLED(false);
statusLED headLight(false);

// Timer
SimpleTimer timer;

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

  // LED setup
  battLED.begin(1); // 1 = TXO Pin
  headLight.begin(0); // 0 = RXI Pin

  // Radio setup
  radio.begin();
  radio.setChannel(1);
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
  servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3);

  // All axis to neutral position
  data.axis1 = 50;
  data.axis2 = 50;
  data.axis3 = 50;
  data.axis4 = 50;

  // Motor PWM frequency (Requires the PWMFrequency.h library)
  // ----------- DOES NOT WORK WITH Mega 328P, BECAUSE millis() IS AFFECTED!! --------------
  //setPWMPrescaler(motor1_pwm, 1); // 123Hz = 256,  492Hz = 64, 3936Hz = 8, 31488Hz = 1
  //setPWMPrescaler(motor2_pwm, 1);

  timer.setInterval(100, checkBattery); // Check battery voltage every 100ms
}

//
// =======================================================================================================
// LED
// =======================================================================================================
//

void led() {

  // Red LED (ON = battery empty, blinking = OK
  if (payload.batteryOk) {
    battLED.flash(140, 150, 500, vehicleNumber); // ON, OFF, PAUSE, PULSES
  } else {
    battLED.off(); // Always ON = battery low voltage
  }

  // Headlights switches off 10s after the vehicle did stop
  if (millis() - millisLightOff >= 10000) {
    headLight.off();
  }
  else {
    headLight.on();
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

  if (millis() - lastRecvTime > 1000) { // bring all servos to their middle position, if no RC signal is received during 1s!
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
  servo1.write(map(data.axis1, 100, 0, 45, 135) ); // 45 - 135° in theory (for Maisto Mustang R 82 - L 108)
  servo2.write(map(data.axis2, 100, 0, 45, 135) ); // 45 - 135°
  servo3.write(map(data.axis3, 100, 0, 45, 135) ); // 45 - 135°
  servo4.write(map(data.axis4, 100, 0, 45, 135) ); // 45 - 135°
}

//
// =======================================================================================================
// DRIVE MOTORS
// =======================================================================================================
//

void driveMotors() {
  int maxPWM;
  int maxAcceleration;

  // Speed limitation (max. is 255)
  if (data.mode1) {
    maxPWM = 170; // Limited
  } else {
    maxPWM = 255; // Full
  }

  if (!payload.batteryOk && liPo) maxPWM = 0; // Stop the vehicle, if the battery is empty! 

  // Acceleration & deceleration limitation (ms per 1 step PWM change)
  if (data.mode2) {
    maxAcceleration = 12; // Limited
  } else {
    maxAcceleration = 7; // Full
  }

  // ***************** Note! The ramptime is intended to protect the gearbox! *******************
  // SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
  // false = brake in neutral position inactive

  if (Motor1.drive(data.axis3, maxPWM, maxAcceleration, true) ) { // The drive motor (function returns true, if not in neutral)
    millisLightOff = millis(); // Reset the headlight delay timer, if the vehicle is driving!
  }

  Motor2.drive(data.axis1, 190, 0, false); // The steering motor (if the original steering motor is reused instead of a servo)
}

//
// =======================================================================================================
// CHECK RX VCC VOLTAGE (Battery is NOT monitored)
// =======================================================================================================
//

void checkBattery() {
  payload.vcc = readVcc() / 1000.0 ;

  if (payload.vcc >= cutoffVoltage) {
    //payload.batteryOk = true;
#ifdef DEBUG
    Serial.print(payload.vcc);
    Serial.println(" Vcc OK");
#endif
  } else {
    payload.batteryOk = false;
#ifdef DEBUG
    Serial.print(payload.vcc);
    Serial.println(" Vcc Low!");
#endif
  }
}

//
// =======================================================================================================
// MAIN LOOP
// =======================================================================================================
//

void loop() {

  // Timer
  timer.run();

  // Read radio data from transmitter
  readRadio();

  // Write the servo positions
  writeServos();

  // Drive the motors
  driveMotors();

  // LED
  led();
}

