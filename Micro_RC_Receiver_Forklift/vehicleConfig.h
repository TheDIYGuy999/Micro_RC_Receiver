
#ifndef vehicleConfig_h
#define vehicleConfig_h

#include "Arduino.h"

#define CONFIG_FORKLIFT_MECCEISO // <- Select the correct vehicle configuration here before uploading! CONFIG_WPL_B_36_MODE1  CONFIG_CATERPILLAR_TEST  CONFIG_KING_HAULER

//
// =======================================================================================================
// VEHICLE SPECIFIC CONFIGURATIONS
// =======================================================================================================
//

/*
  // Battery type
  boolean liPo; // If "true", the vehicle can't be reactivated once the cutoff voltage is reached
  float cutoffVoltage; // Min. battery discharge voltage, or min. VCC, if board rev. < 1.2 (3.6V for LiPo, 1.1 per NiMh cell)

  // Board type (see: https://www.youtube.com/watch?v=-vbmHhCvspg&t=18s)
  float boardVersion; // Board revision (MUST MATCH WITH YOUR BOARD REVISION!!)
  boolean HP; // HP = "High Power" version (both TB6612FNG channels wired in parallel) -> No motor 1, motor 2 is the driving motor

  // Vehicle address
  int vehicleNumber; // This number must be unique for each vehicle!

  // Vehicle type
  byte vehicleType;
  0 = car (see: https://www.youtube.com/watch?v=A0SoK7KJxyc). Use this mode, if you want to use indicators. No MPU-6050 MRSC support.
  1 = semi caterpillar, 2 = caterpillar (see: https://www.youtube.com/watch?v=Tjikm6hJ8hQ)
  3 = forklift (see: https://www.youtube.com/watch?v=3iXL9WvE4ro)
  #define VEHICLE_TYPE_3_WITH_ESC = forklift or dumper with separate ESC, motor driver 1 is controlled by CH 4
  4 = balancing (see: https://www.youtube.com/watch?v=zse9-l2Yo3Y)
  5 = car with MRSC (Micro RC Stability Control). Similar with ABS, ESP, Traxxas Stability Management TSM. Potentiometer on A6 input of
  your transmitter required! (see: https://www.youtube.com/watch?v=IPve7QpdLBc&t=5s) Indicators can't be used in this mode (locked)!
  6 = simple dual motor plane with differential thrust steering. No rudders.


  // Servo limits (45 - 135 means - 45° to 45° from the servo middle position)
  #define STEERING_3_POINT_CAL // steering center point is separately adjustable
  byte lim1L, lim1C, lim1R; // Servo 1, Lim1C for STEERING_3_POINT_CAL option (center position)
  byte lim2L, lim2C, lim2R; // Lim2C for THREE_SPEED_GEARBOX option (center position)
  byte lim3L, lim3R;
  byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles for external ESC
  byte lim4L, lim4R; // Servo 4, also used for tractor trailer unlocking servo (TRACTOR_TRAILER_UNLOCK option)
  #define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2. Not usable in combination with the "tailLights" option
  #define THREE_SPEED_GEARBOX // Vehicle has a mechanical 3 speed shifting gearbox, switched by servo CH2. Not usable in combination with the "tailLights" option

  // Motor configuration
  int maxPWMfull; // (100% PWM is 255)
  int maxPWMlimited;
  int minPWM; // backlash compensation for self balancing applications
  byte maxAccelerationFull;// (ms per 1 step input signal change)
  byte maxAccelerationLimited;

  // Steering configuration (100% torque is 255)
  byte steeringTorque;

  // Motor 2 PWM frequency: 32 = 984Hz (default), 8 = 3936Hz, 1 = 31488Hz (only with board version. 1.3 or newer)
  byte pwmPrescaler2; // Motor 2 = steering motor (or driving motor in "HP" High Power board version)

  // Additional Channels
  boolean TXO_momentary1; // The TXO output is linked to the momentary1 channel! -> Serial not usable, if "true"
  boolean TXO_toggle1; // The TXO output is linked to the toggle1 channel! -> Serial not usable, if "true"
  boolean potentiometer1; // The potentiometer knob on the transmitter is linked to the servo output CH4
*/

// MECCEISO'S MECCANO VEHICLES ***********************************************************************************
// https://www.youtube.com/channel/UCuzV4gX5T-5FzYAQXltWYnA ******************************************************


// Meccano Forklift-------------------------------------------------------------------------
#ifdef CONFIG_FORKLIFT_MECCEISO
// Battery type
boolean liPo = false;
float cutoffVoltage = 4.4; // 4 Eneloop cells

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 2;

// Vehicle type
byte vehicleType = 3; // Forklift mode

// Servo limits in degrees
byte lim1L = 120, lim1R = 60; // ESC right
byte lim2L = 135, lim2R = 45; // ESC lifting
byte lim3L = 120, lim3R = 60; // ESC left
byte lim4L = 140, lim4R = 40; // Steering

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration (lift in this case)
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;
#endif



#endif
