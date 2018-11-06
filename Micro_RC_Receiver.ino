// 4 Channel "Micro RC" Receiver with 4 standard RC Servo Outputs
// ATMEL Mega 328P TQFP 32 soldered directly to the board, 8MHz external resonator,
// 2.4GHz NRF24L01 SMD radio module, TB6612FNG dual dc motor driver
// An MPU-6050 gyro / accelerometer can be used for MRSC stability control or self balancing robots

// See: https://www.youtube.com/playlist?list=PLGO5EJJClJBCjIvu8frS7LrEU3H2Yz_so

// * * * * N O T E ! The vehicle specific configurations are stored in "vehicleConfig.h" * * * *

const float codeVersion = 2.9; // Software revision (see https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/README.md)

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
#include <Wire.h> // I2C library (for the MPU-6050 gyro /accelerometer)
#include <RF24.h> // Installed via Tools > Board > Boards Manager > Type RF24
#include <printf.h>
#include <Servo.h>
#include <statusLED.h> // https://github.com/TheDIYGuy999/statusLED
#include <TB6612FNG.h> // https://github.com/TheDIYGuy999/TB6612FNG ***NOTE*** V1.2 required!! <<<-----
#include <PWMFrequency.h> // https://github.com/TheDIYGuy999/PWMFrequency
#include <PID_v1.h> // https://github.com/br3ttb/Arduino-PID-Library/

// Tabs (header files in sketch directory)
#include "readVCC.h"
#include "vehicleConfig.h"
#include "steeringCurves.h"
#include "tone.h"
#include "balancing.h"
#include "helper.h"

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

// Headlight off delay
unsigned long millisLightOff = 0;

// Indicators
boolean left;
boolean right;
boolean hazard;

// Motors
boolean isDriving; // is the vehicle driving?

// Motor objects
TB6612FNG Motor1;
TB6612FNG Motor2;

// Status LED objects
statusLED tailLight(false); // "false" = output not inverted
statusLED headLight(false);
statusLED indicatorL(false);
statusLED indicatorR(false);
statusLED beaconLights(false);

// Engine sound
boolean engineOn = false;

//
// =======================================================================================================
// RADIO SETUP
// =======================================================================================================
//

void setupRadio() {
  radio.begin();
  radio.setChannel(NRFchannel[chPointer]);

  // Set Power Amplifier (PA) level to one of four levels: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setPALevel(RF24_PA_HIGH); // HIGH

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
  if (boardVersion >= 1.3) { // Board version >= 1.3:
    motor2_in1 = 5;  // 5
    motor2_pwm = 3;  // 3
  }
  else { // Board Version < 1.3:
    motor2_in1 = 3;  // 3
    motor2_pwm = 5;  // 5
  }

  // SYNTAX: IN1, IN2, PWM, min. input value, max. input value, neutral position width
  // invert rotation direction true or false
  Motor1.begin(motor1_in1, motor1_in2, motor1_pwm, 0, 100, 4, false); // Drive motor
  Motor2.begin(motor2_in1, motor2_in2, motor2_pwm, 0, 100, 4, false); // Steering motor (Drive in "HP" version)

  // Motor PWM frequency prescalers (Requires the PWMFrequency.h library)
  // Differential steering vehicles: locked to 984Hz, to make sure, that both motors use 984Hz.
  if (vehicleType == 1 || vehicleType == 2 || vehicleType == 6) pwmPrescaler2 = 32;

  // ----------- IMPORTANT!! --------------
  // Motor 1 always runs @ 984Hz PWM frequency and can't be changed, because timers 0 an 1 are in use for other things!
  // Motor 2 (pin 3) can be changed to the following PWM frequencies: 32 = 984Hz, 8 = 3936Hz, 1 = 31488Hz
  setPWMPrescaler(3, pwmPrescaler2); // pin 3 is hardcoded, because we can't change all others anyway
}

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

#ifndef DEBUG
  Serial.end(); // make sure, serial is off!
  UCSR0B = 0b00000000;
#endif

  R2D2_tell();

  // LED setup
  if (vehicleType == 4 || vehicleType == 5 ) indicators = false; // Indicators use the same pins as the MPU-6050, so they can't be used in vehicleType 4 or 5!

  if (tailLights) tailLight.begin(A1); // A1 = Servo 2 Pin
  if (headLights) headLight.begin(0); // 0 = RXI Pin
  if (indicators) {
    indicatorL.begin(A4); // A4 = SDA Pin
    indicatorR.begin(A5); // A5 = SCL Pin
  }
  if (beacons) beaconLights.begin(A3); // A3 = Servo 4 Pin

  // Radio setup
  setupRadio();

  // Servo pins
  servo1.attach(A0);
  if (!tailLights) servo2.attach(A1);
  if (!engineSound && !toneOut) servo3.attach(A2);
  if (!beacons) servo4.attach(A3);

  // All axes to neutral position
  data.axis1 = 50;
  data.axis2 = 50;
  data.axis3 = 50;
  data.axis4 = 50;

  // Special functions
  if (TXO_momentary1) pinMode(DIGITAL_OUT_1, OUTPUT);

  // Motor driver setup
  setupMotors();

  if (vehicleType == 4 || vehicleType == 5) { // Only for self balancing vehicles and cars with MRSC
    // MPU 6050 accelerometer / gyro setup
    setupMpu6050();

    // PID controller setup
    setupPid();
  }
}

//
// =======================================================================================================
// LED
// =======================================================================================================
//

// Brake light subfunction for ESC vehicles
boolean escBrakeActive() {
  static byte driveState;
  boolean brake;

  switch (driveState) { // 0 = neutral, 1 = forward, 2 = reverse, 3 = brake

    case 0: // neutral
      if (data.axis3 > 55) driveState = 1; // forward
      if (data.axis3 < 45) driveState = 2; // reverse
      brake = false;
      break;

    case 1: // forward
      if (data.axis3 < 45) driveState = 3; // brake
      brake = false;
      break;

    case 2: // reverse
      if (data.axis3 > 55) driveState = 1; // forward
      brake = false;
      break;

    case 3: // brake
      if (data.axis3 > 45) driveState = 2; // go to reverse, if above neutral
      brake = true;
      break;

  }
  return brake;
}

void led() {

  // Lights are switching off 10s after the vehicle did stop
  if (millis() - millisLightOff >= 10000) {
    headLight.off(); // Headlight off
    tailLight.off(); // Taillight off
    beaconLights.off(); // Beacons off
  }
  else {
    if (!escBrakeLights && (!HP && Motor1.brakeActive() || HP && Motor2.brakeActive() )) { // if braking detected from TB6612FNG motor driver
      tailLight.on(); // Brake light (full brightness)
    }

    else if (escBrakeLights && escBrakeActive() ) { // or braking detected from ESC
      tailLight.on(); // Brake light (full brightness)
    }

    else {
      tailLight.flash(10, 14, 0, 0); // Taillight: 10 on  / 14 off = about 40% brightness (soft PWM)
    }
    beaconLights.flash(50, 650, 0, 0); // Simulate rotating beacon lights with short flashes
    headLight.on(); // Headlight on
  }

  // Indicator lights ----
  if (indicators) {
    // Set and reset by lever
    if (data.axis4 < 5) left = true;
    if (data.axis4 > 55) left = false;

    if (data.axis4 > 95) right = true;
    if (data.axis4 < 45) right = false;

    // Reset by steering
    static int steeringOld;

    if (data.axis1 > steeringOld + 10) {
      left = false;
      steeringOld = data.axis1;
    }

    if (data.axis1 < steeringOld - 10) {
      right = false;
      steeringOld = data.axis1;
    }

    // Lights
    if (left) { // Left indicator
      right = false;
      indicatorL.flash(375, 375, 0, 0);
      indicatorR.off();
    }

    if (right) { // Right indicator
      left = false;
      indicatorR.flash(375, 375, 0, 0);
      indicatorL.off();
    }

    if (hazard) { // Hazard lights
      if (left) {
        left = false;
        indicatorL.off();
      }
      if (right) {
        right = false;
        indicatorR.off();
      }
      indicatorL.flash(375, 375, 0, 0);
      indicatorR.flash(375, 375, 0, 0);
    }

    if (!hazard && !left && !right) {
      indicatorL.off();
      indicatorR.off();
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
    hazard = true; // Enable hazard lights
    payload.batteryOk = true; // Clear low battery alert (allows to re-enable the vehicle, if you switch off the transmitter)
#ifdef DEBUG
    Serial.println("No Radio Available - Check Transmitter!");
#endif
  }

  if (millis() - lastRecvTime > 2000) {
    setupRadio(); // re-initialize radio
    lastRecvTime = millis();
  }
}

//
// =======================================================================================================
// WRITE SERVO POSITIONS
// =======================================================================================================
//

void writeServos() {
  // Aileron or Steering
  if (vehicleType != 5) { // If not car with MSRC stabilty control
    servo1.write(map(data.axis1, 100, 0, lim1L, lim1R) ); // 45 - 135°
  }

  // Elevator or shifting gearbox actuator
#ifdef TWO_SPEED_GEARBOX // Shifting gearbox mode, controlled by "Mode 1" button
  if (!tailLights) {
    if (data.mode1)servo2.write(lim2L);
    else servo2.write(lim2R);
  }
  
#else // Servo controlled by joystick CH2
  if (!tailLights) servo2.write(map(data.axis2, 100, 0, lim2L, lim2R) ); // 45 - 135°
#endif

  // Throttle (for ESC control, if you don't use the internal TB6612FNG motor driver)
  if (data.mode1) { // limited speed!
    servo3.write(map(data.axis3, 100, 0, lim3Llow, lim3Rlow ) ); // less than +/- 45°
  }
  else { // full speed!
    servo3.write(map(data.axis3, 100, 0, lim3L, lim3R) ); // 45 - 135°
  }

  // Rudder
  if (!potentiometer1) { // Servo 4 controlled by CH4
    if (!beacons) servo4.write(map(data.axis4, 100, 0, lim4L, lim4R) ); // 45 - 135°
  }
  else { // Servo 4 controlled by transmitter potentiometer knob
    if (!beacons) servo4.write(map(data.pot1, 0, 100, 45, 135) ); // 45 - 135°
  }

  // Axis 2 on the joystick switches engine sound on servo channel 3 on and off!
  if (engineSound) {
    if (data.axis2 > 80) {
      servo3.attach(A2); // Enable servo 3 pulse
    }
    if (data. axis2 < 20) {
      servo3.detach(); // Disable servo 3 pulse = engine off signal for "TheDIYGuy999" engine simulator!
    }
  }
}

//
// =======================================================================================================
// DRIVE MOTORS CAR (for cars, one motor for driving, one for steering)
// =======================================================================================================
//

void driveMotorsCar() {

  int maxPWM;
  byte maxAcceleration;

  // Speed limitation (max. is 255)
  if (data.mode1) {
    maxPWM = maxPWMlimited; // Limited
  } else {
    maxPWM = maxPWMfull; // Full
  }

  if (!payload.batteryOk && liPo) data.axis3 = 50; // Stop the vehicle, if the battery is empty!

  // Acceleration & deceleration limitation (ms per 1 step input signal change)
  if (data.mode2) {
    maxAcceleration = maxAccelerationLimited; // Limited
  } else {
    maxAcceleration = maxAccelerationFull; // Full
  }

  // ***************** Note! The ramptime is intended to protect the gearbox! *******************
  // SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
  // false = brake in neutral position inactive

  if (!HP) { // Two channel version: ----
    if (Motor1.drive(data.axis3, minPWM, maxPWM, maxAcceleration, true) ) { // The drive motor (function returns true, if not in neutral)
      millisLightOff = millis(); // Reset the headlight delay timer, if the vehicle is driving!
    }
    if (vehicleType != 5) { // If not car with MSRC stabilty control
      Motor2.drive(data.axis1, 0, steeringTorque, 0, false); // The steering motor (if the original steering motor is reused instead of a servo)
    }
  }
  else { // High Power "HP" version. Motor 2 is the dirving motor, no motor 1: ----
    if (Motor2.drive(data.axis3, minPWM, maxPWM, maxAcceleration, true) ) { // The drive motor (function returns true, if not in neutral)
      millisLightOff = millis(); // Reset the headlight delay timer, if the vehicle is driving!
    }
  }
}

//
// =======================================================================================================
// DRIVE MOTORS FORKLIFT (for forklifts, one motor for driving, one for lifting)
// =======================================================================================================
//

void driveMotorsForklift() {

  int maxPWM;
  byte maxAcceleration;

  // Speed limitation (max. is 255)
  if (data.mode1) {
    maxPWM = maxPWMlimited; // Limited
  } else {
    maxPWM = maxPWMfull; // Full
  }

  if (!payload.batteryOk && liPo) data.axis3 = 50; // Stop the vehicle, if the battery is empty!

  // Acceleration & deceleration limitation (ms per 1 step input signal change)
  if (data.mode2) {
    maxAcceleration = maxAccelerationLimited; // Limited
  } else {
    maxAcceleration = maxAccelerationFull; // Full
  }

  // ***************** Note! The ramptime is intended to protect the gearbox! *******************
  // SYNTAX: Input value, max PWM, ramptime in ms per 1 PWM increment
  // false = brake in neutral position inactive


  if (Motor1.drive(data.axis3, minPWM, maxPWM, maxAcceleration, true) ) { // The drive motor (function returns true, if not in neutral)
    millisLightOff = millis(); // Reset the headlight delay timer, if the vehicle is driving!
  }
  Motor2.drive(data.axis2, 0, steeringTorque, 0, false); // The fork lifting motor (the steering is driven by servo 1)
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
  const int servoMin = 0;
  const int servoMax = 100;
  const int servoNeutralMin = 48;
  const int servoNeutralMax = 52;

  // Compute steering overlay:
  // The steering signal is channel 1 = data.axis1
  // 100% = wheel spins with 100% of the requested speed forward
  // -100% = wheel spins with 100% of the requested speed backward
  if (data.axis1 <= servoNeutralMin) {
    steeringFactorLeft = map(data.axis1, servoMin, servoNeutralMin, 0, 100);
    steeringFactorLeft = constrain(steeringFactorLeft, 0, 100);
  }
  else {
    steeringFactorLeft = 100;
  }

  if (data.axis1 >= servoNeutralMax) {
    steeringFactorRight = map(data.axis1, servoMax, servoNeutralMax, 0, 100);
    steeringFactorRight = constrain(steeringFactorRight, 0, 100);
  }
  else {
    steeringFactorRight = 100;
  }

  // Nonlinear steering overlay correction
  if (vehicleType == 6) {
    steeringFactorLeft2 = reMap(curveThrust, steeringFactorLeft); // Differential thrust mode
    steeringFactorRight2 = reMap(curveThrust, steeringFactorRight);
    data.axis3 = constrain(data.axis3, 50, 100); // reverse locked!
  }
  if (vehicleType == 2) {
    steeringFactorLeft2 = reMap(curveFull, steeringFactorLeft); // Caterpillar mode
    steeringFactorRight2 = reMap(curveFull, steeringFactorRight);
  }
  if (vehicleType == 1) {
    steeringFactorLeft2 = reMap(curveSemi, steeringFactorLeft); // Semi caterpillar mode
    steeringFactorRight2 = reMap(curveSemi, steeringFactorRight);
  }

  // Drive caterpillar motors
  // The throttle signal (for both caterpillars) is channel 3 = data.axis3
  // -100 to 100%
  pwm[0] = map(data.axis3, servoMin, servoMax, 100, -100) * steeringFactorRight2 / 100;
  pwm[1] = map(data.axis3, servoMin, servoMax, 100, -100) * steeringFactorLeft2 / 100;

  pwm[0] = map(pwm[0], 100, -100, 100, 0); // convert -100 to 100% to 0-100 for motor control
  pwm[1] = map(pwm[1], 100, -100, 100, 0);

  if (!payload.batteryOk && liPo) { // Both motors off, if LiPo battery is empty!
    pwm[0] = 0;
    pwm[1] = 0;
  }

  Motor1.drive(pwm[0], 0, 255, 0, false); // left caterpillar, 0ms ramp!
  Motor2.drive(pwm[1], 0, 255, 0, false); // right caterpillar

  if (pwm[0] < 40 || pwm[0] > 60 || pwm[1] < 40 || pwm[1] > 60) {
    millisLightOff = millis(); // Reset the headlight delay timer, if the vehicle is driving!
  }
}

//
// =======================================================================================================
// "BALANCING" MOTOR DRIVING FUNCTION (for self balancing robot)
// =======================================================================================================
//

void driveMotorsBalancing() {

  // The steering overlay is in degrees per second, controlled by the MPU 6050 yaw rate and yoystick axis 1
  int steering = ((data.axis1 - 50) / 7) - yaw_rate; // -50 to 50 / 8 = 7°/s - yaw_rate
  steering = constrain(steering, -7, 7);
  int speed = angleOutput + 50;

  // Calculate averaged motor power (speed) for speed controller feedback
  static unsigned long lastSpeed;
  if (millis() - lastSpeed >= 8) {  // 8ms
    speedAveraged = (speedAveraged * 3.0 + angleOutput) / 4.0; // 1:4 (1:8)
  }

  speed = constrain(speed, 7, 93); // same range as in setupPID() + 50 offset from above!

  if (angleMeasured > -20.0 && angleMeasured < 20.0) { // Only drive motors, if robot stands upright
    Motor1.drive(speed - steering, minPWM, maxPWMfull, 0, false); // left caterpillar, 0ms ramp! 50 = neutral!
    Motor2.drive(speed + steering, minPWM, maxPWMfull, 0, false); // right caterpillar
  }
  else { // keep motors off
    Motor1.drive(50, minPWM, maxPWMfull, 0, false); // left caterpillar, 0ms ramp!
    Motor2.drive(50, minPWM, maxPWMfull, 0, false); // right caterpillar
  }
}

//
// =======================================================================================================
// BALANCING CALCULATIONS
// =======================================================================================================
//

void balancing() {

  // Read sensor data
  readMpu6050Data();

  angleMeasured = angle_pitch - tiltCalibration;

  // Read speed pot with 0.2s fader
  static unsigned long lastPot;
  if (millis() - lastPot >= 40) { // 40ms
    lastPot = millis();
    speedPot = (speedPot * 4 + data.axis3) / 5; // 1:5
  }

  // PID Parameters (Test)
  double speedKp = 0.9, speedKi = 0.03, speedKd = 0.0;
  double angleKp = data.pot1 / 8.0, angleKi = 25.0, angleKd = 0.12; // /You need to connect a potentiometer to the transmitter analog input A6

  // PID Parameters (Working)
  //double speedKp = 0.9, speedKi = 0.03, speedKd = 0.0;
  //double angleKp = data.pot1 / 8.0, angleKi = 25.0, angleKd = 0.12; // You need to connect a potentiometer to the transmitter analog input A6

  // Speed PID controller (important to protect the robot from falling over at full motor rpm!)
  speedTarget = ((float)speedPot - 50.0) / 1.51; // (100 - 50) / 1.51 = Range of about +/- 33 (same as in setupPid() !)
  speedMeasured = speedAveraged * 1.3; //angleOutput; // 43 / 33 = 1.3
  speedPid.SetTunings(speedKp, speedKi, speedKd);
  speedPid.Compute();

  // Angle PID controller
  angleTarget = speedOutput / -8.25; // 33.0 (from above) / 8.25 = Range of about +/- 4.0° tilt angle
  //  angleTarget = (speedPot - 50) / -12.5; // 50 / 12.5 = Range of about +/- 4.0° tilt angle
  anglePid.SetTunings(angleKp, angleKi, angleKd);
  anglePid.Compute();

  // Send the calculated values to the motors
  driveMotorsBalancing();

  // Display PID variables on the transmitter OLED (for debugging only, comment out the corresponding variables in checkBattery() in this case)
  //loopDuration(); // compute the loop time
  //payload.vcc = loopTime;
  //payload.vcc = angleMeasured;
  //payload.vcc = speedTarget;
  //payload.batteryVoltage = speedOutput;
  //payload.batteryVoltage = speedMeasured;
}

//
// =======================================================================================================
// MRSC (MICRO RC STABILITY CONTROL) CALCULATIONS
// =======================================================================================================
// For cars with stability control (steering overlay depending on gyro yaw rate)

void mrsc() {

  // Read sensor data
  readMpu6050Data();

  // If the MRSC gain is a fixed value, read it!
#ifdef MRSC_FIXED
  data.pot1 = mrscGain;
#endif

  // Compute steering compensation overlay
  int turnRateSetPoint = data.axis1 - 50;  // turnRateSetPoint = steering angle (0 to 100) - 50 = -50 to 50
  int turnRateMeasured = yaw_rate * abs(data.axis3 - 50); // degrees/s * speed
  int steeringAngle = turnRateSetPoint + (turnRateMeasured * data.pot1 / 100);  // Compensation depending on the pot value

  steeringAngle = constrain (steeringAngle, -50, 50); // range = -50 to 50

  // Control steering servo (MRSC mode only)
  servo1.write(map(steeringAngle, 50, -50, lim1L, lim1R) ); // 45 - 135°

  // Control motor 2 (steering, not on "High Power" board type)
  if (!HP) {
    Motor2.drive((steeringAngle + 50), 0, steeringTorque, 0, false); // The steering motor (if the original steering motor is reused instead of a servo)
  }

  // Control motors
  driveMotorsCar();

}

//
// =======================================================================================================
// WRITE DIGITAL OUTPUTS (SPECIAL FUNCTIONS)
// =======================================================================================================
//

void digitalOutputs() {
  if (TXO_momentary1) { // only, if function is enabled in vehicle configuration
    if (data.momentary1) {
      digitalWrite(DIGITAL_OUT_1, HIGH);
      R2D2_tell();
    }
    else digitalWrite(DIGITAL_OUT_1, LOW);
  }
}

//
// =======================================================================================================
// CHECK RX BATTERY & VCC VOLTAGES
// =======================================================================================================
//

boolean battSense;

void checkBattery() {

  if (boardVersion < 1.2) battSense = false;
  else battSense = true;

  // switch between load and no load contition
  if (millis() - millisLightOff >= 1000) { // one s after the vehicle did stop
    isDriving = false; // no load
  }
  else {
    isDriving = true; // under load
  }

  // Every 1000 ms, take measurements
  static unsigned long lastTrigger;
  if (millis() - lastTrigger >= 1000) {
    lastTrigger = millis();

    // Read both averaged voltages
    payload.batteryVoltage = batteryAverage();
    payload.vcc = vccAverage();

    if (battSense) { // Observe battery voltage
      if (payload.batteryVoltage <= cutoffVoltage) payload.batteryOk = false;
    }
    else { // Observe vcc voltage
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
  static int raw[6];

  if (!battSense) return 0;

  if (raw[0] == 0) {
    for (int i = 0; i <= 5; i++) {
      raw[i] = analogRead(BATTERY_DETECT_PIN); // Init array
    }
  }

  raw[5] = raw[4];
  raw[4] = raw[3];
  raw[3] = raw[2];
  raw[2] = raw[1];
  raw[1] = raw[0];
  if (isDriving && HP) raw[0] = (analogRead(BATTERY_DETECT_PIN) + 31); // add 0.3V while driving (HP version only): 1023 steps * 0.3V / 9.9V = 31
  else raw[0] = analogRead(BATTERY_DETECT_PIN); // else take the real voltage (compensates voltage drop while driving)
  float average = (raw[0] + raw[1] + raw[2] + raw[3] + raw[4] + raw[5]) / 619.999; // 1023steps / 9.9V * 6 = 619.999
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
  if (vehicleType == 0) driveMotorsCar(); // Car
  else if (vehicleType == 5) mrsc(); // Car with MSRC stabilty control
  else if (vehicleType == 3) driveMotorsForklift(); // Forklift
  else if (vehicleType == 4) balancing(); // Self balancing robot
  else driveMotorsSteering(); // Caterpillar and half caterpillar vecicles

  // Battery check
  checkBattery();

  // Digital Outputs (special functions)
  digitalOutputs();

  // LED
  led();
}

