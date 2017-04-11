#ifndef vehicleConfig_h
#define vehicleConfig_h

#include "Arduino.h"

#define CONFIG_SELF_BALANCING // <- Select the correct vehicle configuration here before uploading!

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
  0 = car (see: https://www.youtube.com/watch?v=A0SoK7KJxyc)
  1 = semi caterpillar, 2 = caterpillar (see: https://www.youtube.com/watch?v=Tjikm6hJ8hQ)
  3 = forklift (see: https://www.youtube.com/watch?v=3iXL9WvE4ro)
  4 = balancing (see: https://www.youtube.com/watch?v=zse9-l2Yo3Y)

  // Lights (see: https://www.youtube.com/watch?v=qbhPqHdBz3o)
  boolean tailLights; // Caution: the taillights are wired to the servo pin 2! -> Servo 2 not usable, if "true"
  boolean headLights; // Caution: the headlights are wired to the RXI pin! -> Serial not usable, if "true"
  boolean indicators; // Caution: the indicators are wired to the SDA / SCL pins! -> I2C (self balancing) not usable, if "true"
  boolean beacons; // Caution: the beacons are wired to the servo pin 4! -> Servo 4 not usable, if "true"

  // Servo limits (45 - 135 means - 45° to 45° from the servo middle position)
  byte lim1L, lim1R; // Servo 1
  byte lim2L, lim2R;
  byte lim3L, lim3R;
  byte lim4L, lim4R; // Servo 4

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
  boolean potentiometer1;

  // Engine sound (see: https://www.youtube.com/watch?v=pPlrx9yVI6E)
  boolean engineSound; // true = a "TheDIYGuy999" engine simulator is wired to servo channel 3

  // Tone sound (see: https://www.youtube.com/watch?v=fe5_1mMtcLQ&t=3s)
  boolean toneOut; // true = a BC337 amplifier for tone() is connected instead of servo 3
*/

// Generic configuration, board v1.0-------------------------------------------------------------------------
#ifdef CONFIG_GENERIC_V10
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.1; // trigger, as soon as VCC drops! (no battery sensing)

// Board type
float boardVersion = 1.0;
boolean HP = false;

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO V1.2 standard configuration-----------------------------------------------------------------
#ifdef CONFIG_MECCANO_V12
// Battery type
boolean liPo = false;
boolean cutoffVoltage = 3.3;

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Generic configuration, board v1.3-------------------------------------------------------------------
#ifdef CONFIG_GENERIC_V13
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.6;

// Board type
float boardVersion = 1.3;
boolean HP = false;

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Disney Lightning McQueen 95----------------------------------------------------------------------
#ifdef CONFIG_MC_QUEEN
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.1; // trigger, as soon as VCC drops! (no battery sensing)

// Board type
float boardVersion = 1.0;
boolean HP = false;

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 61, lim1R = 104;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Disney 95 "DINOCO"--------------------------------------------------------------------------------
#ifdef CONFIG_DINOCO
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.1; // trigger, as soon as VCC drops! (no battery sensing)

// Board type
float boardVersion = 1.0;
boolean HP = false;

// Vehicle address
int vehicleNumber = 2;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Maisto Mustang GT Fastback---------------------------------------------------------------------------
#ifdef CONFIG_MUSTANG
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.45;

// Board type
float boardVersion = 1.3;
boolean HP = true;

// Vehicle address
int vehicleNumber = 3;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 60, lim1R = 129;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 1; // This a show car and we don't want PWM switching noise! So, 31.5KHz frequency.

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = true;

// Tone sound
boolean toneOut = false;
#endif

// Maisto Dodge Challenger----------------------------------------------------------------------------
#ifdef CONFIG_CHALLENGER
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.6;

// Board type
float boardVersion = 1.3;
boolean HP = true; // High Power Board!

// Vehicle address
int vehicleNumber = 4;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = true;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 120, lim1R = 55;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 240; // still a bit limited, but fast as hell!
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// GearGmax / KIDZTECH TOYS Porsche GT3 RS 4.0--------------------------------------------------------
#ifdef CONFIG_PORSCHE
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.3;

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 5; // one car number 2 and one number 5!

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Coke Can Car--------------------------------------------------------------------------------------
#ifdef CONFIG_CCC
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.6;

// Board type
float boardVersion = 1.3;
boolean HP = false;

// Vehicle address
int vehicleNumber = 6;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 160;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 1; // We don't want PWM switching noise from the steering! So, 31.5KHz frequency.

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// KD-Summit S600 RC Truggy-------------------------------------------------------------------------
#ifdef CONFIG_KD_SUMMIT
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.6;

// Board type
float boardVersion = 1.3;
boolean HP = true; // High Power Board!

// Vehicle address
int vehicleNumber = 7;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 130, lim1R = 50; // Direction inverted!
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Maisto Chevy Camaro---------------------------------------------------------------------------
#ifdef CONFIG_CAMARO
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.45;

// Board type
float boardVersion = 1.3;
boolean HP = false;

// Vehicle address
int vehicleNumber = 8;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 1; // We don't want PWM switching noise from the steering! So, 31.5KHz frequency.

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:18 LaFerrari-------------------------------------------------------------------------
#ifdef CONFIG_LAFERRARI
// Battery type
boolean liPo = false;
float cutoffVoltage = 4.4; // 4 NiMh cells

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 9;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Rui Chuang Forklift-------------------------------------------------------------------------
#ifdef CONFIG_FORKLIFT
// Battery type
boolean liPo = false;
float cutoffVoltage = 4.4; // 4 Eneloop cells

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 3; // Forklift mode

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = true;

// Servo limits
byte lim1L = 145, lim1R = 35;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

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
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// R2-D2 STAR WARS robot-----------------------------------------------------------------------
#ifdef CONFIG_R2D2
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.45;

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 2; // 2 = caterpillar mode

// Lights
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = true;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = true;
#endif

// Caterpillar test vehicle-----------------------------------------------------------------------
#ifdef CONFIG_CATERPILLAR_TEST
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.1;

// Board type
float boardVersion = 1.0;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 2; // 2 = caterpillar mode

// Lights
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Self balancing robot-----------------------------------------------------------------------
#ifdef CONFIG_SELF_BALANCING
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.1; // 4 Eneloop cells

// Board type
float boardVersion = 1.0;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 4; // 4 = balancing mode

// Lights
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 15; // 15 Backlash compensation, important for self balancing!
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = -0.2; // -0.2° (+ = leans more backwards!) Vary a bit, if you have slow oscillation with big amplitude

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = false;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

#endif
