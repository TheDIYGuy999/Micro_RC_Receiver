
#ifndef vehicleConfig_h
#define vehicleConfig_h

#include "Arduino.h"

#define CONFIG_BRUSHLESS_D90 // <- Select the correct vehicle configuration here before uploading! CONFIG_WPL_B_36_MODE1  CONFIG_CATERPILLAR_TEST  CONFIG_KING_HAULER

// NOTE: SBUS not usable if "TXO_momentary1" or "TXO_toggle1" or "headLights" or DEBUG!
#define SBUS_SERIAL // serial connection uses SBUS protocol instead of normal protocol, if not commented out

#define ESC_MICROSECONDS // ESC controlled in microseconds instead of degrees (experimental)

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

  // MRSC
  #define MRSC_FIXED // Only use this definition, if you want to use an MRSC vehicle without a gain adjustment pot on your transmitter
  byte mrscGain = 25; // 25%

  // Lights (see: https://www.youtube.com/watch?v=qbhPqHdBz3o , https://www.youtube.com/watch?v=wBTfsIk4vkU&t=84s)
  boolean tailLights; // Caution: the taillights are wired to the servo pin 2! -> Servo 2 not usable, if "true"
  boolean headLights; // Caution: the headlights are wired to the RXI pin! -> Serial not usable, if "true"
  boolean indicators; // Caution: the indicators are wired to the SDA / SCL pins! -> not usable, if vehicleType is 4 or 5 (locked)
  boolean beacons; // Caution: the beacons are wired to the servo pin 4! -> Servo 4 not usable, if "true"

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

  // Variables for self balancing (vehicleType = 4) only!
  float tiltCalibration = -0.2; // -0.2° (+ = leans more backwards!) Vary a bit, if you have slow oscillation with big amplitude

  // Steering configuration (100% torque is 255)
  byte steeringTorque;

  // Motor 2 PWM frequency: 32 = 984Hz (default), 8 = 3936Hz, 1 = 31488Hz (only with board version. 1.3 or newer)
  byte pwmPrescaler2; // Motor 2 = steering motor (or driving motor in "HP" High Power board version)

  // Additional Channels
  boolean TXO_momentary1; // The TXO output is linked to the momentary1 channel! -> Serial not usable, if "true"
  boolean TXO_toggle1; // The TXO output is linked to the toggle1 channel! -> Serial not usable, if "true"
  boolean potentiometer1; // The potentiometer knob on the transmitter is linked to the servo output CH4

  // Engine sound (see: https://www.youtube.com/watch?v=pPlrx9yVI6E)
  boolean engineSound; // true = a "TheDIYGuy999" engine simulator is wired to servo channel 3 (allows to switch off RC signal, deprecated)

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;
// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Generic configuration, board v1.3 HP----------------------------------------------------------------------------
#ifdef CONFIG_GENERIC_V13_HP
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.6;

// Board type
float boardVersion = 1.3;
boolean HP = true; // High Power Board!

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean escBrakeLights = true;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Tamiya NEO Fighter Buggy -------------------------------------------------------------------
#ifdef CONFIG_TAMIYA_FIGHTER
// Battery type
boolean liPo = false; // ESC provides protection
float cutoffVoltage = 4.9; // Regulated 6.0V supply from the ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 41, lim1R = 131; // R 41, L131
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 125, lim3Rlow = 60; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// WlToys K 1:28 Rally Fiesta----------------------------------------------------------------------------
#ifdef CONFIG_FIESTA
// Battery type
boolean liPo = true;
float cutoffVoltage = 4.9; // Regulated 5.0V supply from the ESC

// Board type
float boardVersion = 1.3;
boolean HP = true; // High Power Board!

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 5; // MRSC vehicle!

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 55, lim1R = 150; // R55, L150
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 140;
byte lim3Llow = 75, lim3Rlow = 110; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 61, lim1R = 104;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Tamiya VW GOLF Mk. 1 Racing Group 2 -------------------------------------------------------------------
#ifdef CONFIG_TAMIYA_GOLF
// Battery type
boolean liPo = false; // ESC provides protection
float cutoffVoltage = 4.9; // Regulated 6.0V supply from the ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 2;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 110, lim1R = 60; // R 115, L62
byte lim2L = 45, lim2R = 135;
byte lim3L = 35, lim3R = 150; // ESC output signal not reversed
byte lim3Llow = 60, lim3Rlow = 125; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// WLtoys 18429 Desert Buggy---------------------------------------------------------------------------
#ifdef CONFIG_18429
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.45; // Regulated 5.0V supply from the ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 2;

// Vehicle type
byte vehicleType = 5;

// Lights
boolean escBrakeLights = true;
boolean tailLights = true;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 130, lim1R = 75; // R125, L70 Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = true;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// HG P407 Tamiya Bruiser Clone-------------------------------------------------------------------
#ifdef CONFIG_HG_P407
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.6;

// Board type
float boardVersion = 1.5;
boolean HP = false;

// Vehicle address
int vehicleNumber = 3;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 62, lim1C = 100, lim1R = 134; // R60 C96 L132
byte lim2L = 67, lim2C = 99, lim2R = 146; // 3 speed gearbox shifting servo 69 = 3. gear, 99 2. gear, 146 = 1. gear.
byte lim3L = 135, lim3R = 45;
byte lim3Llow = 105, lim3Rlow = 75; // limited top speed angles!
byte lim4L = 45, lim4R = 135;
#define THREE_SPEED_GEARBOX // Vehicle has a mechanical 3 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// JJRC Q46 Buggy -------------------------------------------------------------------
#ifdef CONFIG_JJRC_Q46
// Battery type
boolean liPo = false; // ESC provides protection
float cutoffVoltage = 4.9; // Regulated 6.0V supply from the ESC

// Board type
float boardVersion = 1.3;
boolean HP = false;

// Vehicle address
int vehicleNumber = 3;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 125, lim1R = 55; // R125, L70 Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 125, lim3Rlow = 60; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO 6952 "Tuning Radio Control"--------------------------------------------------------
#ifdef CONFIG_MECCANO_6953
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.3;

// Board type
float boardVersion = 1.3;
boolean HP = false;

// Vehicle address
int vehicleNumber = 3;

// Vehicle type
byte vehicleType = 5; // MRSC stability control

// MRSC
#define MRSC_FIXED
byte mrscGain = 35; // 35%

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 60, lim1R = 129;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 1; // This a show car and we don't want PWM switching noise! So, 31.5KHz frequency.

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = true;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 134, lim1R = 69; // 120 55
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255; // was 245
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// WPL C34KM Toyota FJ40 Land Cruiser-------------------------------------------------------------------
#ifdef CONFIG_C34
// Battery type
boolean liPo = true;
float cutoffVoltage = 4.5; // 5V receiver supply voltage surveillance from BEC only!

// Board type
float boardVersion = 1.5;
boolean HP = false;

// Vehicle address
int vehicleNumber = 4;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 60, lim1R = 145; // R60, L145
byte lim2L = 120, lim2R = 65; // Gearbox shifter limits (1. and 2. gear)
byte lim3L = 65, lim3R = 125; // +/-25° is still full throttle with the JMT-10A ESC! (Forward, Reverse)
byte lim3Llow = 65, lim3Rlow = 125; // same setting (full throttle), because of shifting gearbox!
byte lim4L = 45, lim4R = 135;
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
int vehicleNumber = 1; // one car number 1 and one number 5!

// Vehicle type
byte vehicleType = 5; // MRSC vehicle!

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 62, lim1R = 101; // Car # 5: R 65, L 101
//byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Tamiya King Hauler Truck with ESP32 Sound Controller in SBUS mode (ESC controlled by ESP32) -----------------
#ifdef CONFIG_KING_HAULER
// Battery type
boolean liPo = false; // LiPo is protected by ESC
float cutoffVoltage = 4.0; // 5V supply

// Board type
float boardVersion = 1.5;
boolean HP = false;

// Vehicle address
int vehicleNumber = 5;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 142, lim1R = 58; // Steering R 142, L 57
byte lim2L = 143, lim2C = 90, lim2R = 37; // 3 speed gearbox shifting servo (3., 2., 1. gear)
byte lim3L = 135, lim3R = 45; // ESC
byte lim3Llow = 135, lim3Rlow = 45; // limited top speed ESC angles! (full speed in this case)
byte lim4L = 45, lim4R = 135; // Controlled by pot, for sound triggering!
#define THREE_SPEED_GEARBOX // Vehicle has a mechanical 3 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
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
byte vehicleType = 5;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 160;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 1; // We don't want PWM switching noise from the steering! So, 31.5KHz frequency.

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// WPL B24 GAZ-66-------------------------------------------------
#ifdef CONFIG_GAZ_66
// Battery type
boolean liPo = true;
float cutoffVoltage = 4.5; // 5V receiver supply voltage surveillance from BEC only!

// Board type
float boardVersion = 1.3;
boolean HP = true;

// Vehicle address
int vehicleNumber = 5;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = true;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits ----
// Steering
byte lim1L = 57, lim1R = 123; // R57  L123

// Throttle
byte lim3L = 65, lim3R = 125; // +/-25° is still full throttle with the JMT-10A ESC! (Forward, Reverse)
byte lim3Llow = 65, lim3Rlow = 125; // same setting (full throttle), because of shifting gearbox!

// Horn impulse (wired to Das Mikro TBS-Micro input "Prop 2")
byte lim4L = 135, lim4R = 90; // Generating 90° (neutral) servo position, if switch released and 135°, if pressed
#define TRACTOR_TRAILER_UNLOCK // TRACTOR_TRAILER_UNLOCK used for 3 position toggle switch.
//Not usable in combination with the "beacons" and "potentiometer" option

// Gearbox shifting
byte lim2L = 43, lim2R = 120; // Gearbox shifter limits (1. and 2. gear)
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration ----
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Hercules Hobby Actros Truck with ESP32 Sound Controller in SBUS mode (ESC controlled by ESP32) -----------------
#ifdef CONFIG_ACTROS
// Battery type
boolean liPo = false; // LiPo is protected by ESC
float cutoffVoltage = 4.0; // 5V supply

// Board type
float boardVersion = 1.5;
boolean HP = false;

// Vehicle address
int vehicleNumber = 6;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 130, lim1C = 90, lim1R = 50; // Steering L 130, C 90, R 50
byte lim2L = 143, lim2C = 90, lim2R = 37; // 3 speed gearbox shifting servo (3., 2., 1. gear)
byte lim3L = 135, lim3R = 45; // ESC
byte lim3Llow = 135, lim3Rlow = 45; // limited top speed ESC angles! (full speed in this case)
byte lim4L = 45, lim4R = 135; // Controlled by pot, for sound triggering!
#define THREE_SPEED_GEARBOX // Vehicle has a mechanical 3 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Feiyue FY03 Eagle Buggy---------------------------------------------------------------------------
#ifdef CONFIG_FY03
// Battery type
boolean liPo = false;
float cutoffVoltage = 4.9; // Regulated 5.0V supply from the ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 6;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 117, lim1R = 62; // R125, L70 Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 110, lim3Rlow = 75; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// HBX 12891 DUNE THUNDER Buggy---------------------------------------------------------------------------
#ifdef CONFIG_HBX_12891
// Battery type
boolean liPo = false; // protected by the ESC
float cutoffVoltage = 3.5; // Regulated 5.0V supply from the ESC, but weak 18650 batteries!

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 7;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 125, lim1R = 70; // R125, L70 Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 110, lim3Rlow = 75; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = true;
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
float cutoffVoltage = 4.9; // 5V receiver supply voltage surveillance from BEC only!

// Board type
float boardVersion = 1.3;
boolean HP = true; // High Power Board!

// Vehicle address
int vehicleNumber = 7;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 130, lim1R = 50; // Direction inversed!
byte lim2L = 45, lim2R = 135;
byte lim3L = 65, lim3R = 120; // +/-25° is still full throttle with the JMT-10A ESC! (Forward, Reverse)
byte lim3Llow = 75, lim3Rlow = 110; // limited top speed angles! A slight offset towards reverse is required with this ESC
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;
// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:12 MN Model Landrover Defender D90 Brushless with 2 speed transmission---------------------------------
#ifdef CONFIG_BRUSHLESS_D90
// Battery type
boolean liPo = true;
float cutoffVoltage = 4.5; // 5V receiver supply voltage surveillance from BEC only!

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 7;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = true;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 55, lim1C = 95, lim1R = 130; // R55 C95 L130
byte lim2L = 10, lim2R = 120; // Gearbox shifter limits (1. and 2. gear)
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 150, lim3Rlow = 35; // same setting (full throttle), because of shifting gearbox!
byte lim4L = 45, lim4R = 135;
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Maisto Chevy Camaro SS---------------------------------------------------------------------------
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
byte vehicleType = 5; // MPU6050 module required!

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 127, lim1R = 52; // R120, L45 Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 1; // We don't want PWM switching noise from the steering! So, 31.5KHz frequency.

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// JLB Racing Cheetah----------------------------------------------------------------------------
#ifdef CONFIG_CHEETAH
// Battery type
boolean liPo = false;
float cutoffVoltage = 3.6; // Brushless ESC has its own battery protection

// Board type
float boardVersion = 1.3;
boolean HP = true; // High Power Board!

// Vehicle address
int vehicleNumber = 8;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 140, lim1R = 45; // Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 125, lim3Rlow = 60; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = true; // true = MRSC knob linked to servo CH4!

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Remo Hobby S Max---------------------------------------------------------------------------
#ifdef CONFIG_S_MAX
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.45; // Regulated 5.0V supply from the ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 9;

// Vehicle type
byte vehicleType = 5;

// Lights
boolean escBrakeLights = true;
boolean tailLights = true;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 125, lim1R = 80; // R125, L70 Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = true;
boolean potentiometer1 = true;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:16 WPL B-36 Russian URAL-4320 Military Command Truck (Gearbox switched with mode 1)------------------------------
#ifdef CONFIG_WPL_B_36_MODE1
// Battery type
boolean liPo = false;
float cutoffVoltage = 0.0; // 6V receiver supply voltage, but Sound module causes a lot of noise!

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 9;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 62, lim1C = 107, lim1R = 140; // R62 C106 L140
byte lim2L = 15, lim2R = 121; // Gearbox shifter limits (1. and 2. gear, 42, 123)
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 150, lim3Rlow = 35; // 2 speed transmission, so same values!
byte lim4L = 45, lim4R = 135;
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:16 JJRC Q60 (3D printed ZIL-131 body) Military Truck-------------------------------------------------
#ifdef CONFIG_JJRC_Q60
// Battery type
boolean liPo = true;
float cutoffVoltage = 4.5; // Original 6V NiCd battery pack or 2S LiPo, Receiver 5V supply from ESC

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = true;
boolean tailLights = true;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 74, lim1R = 117; // R74  L117
byte lim2L = 45, lim2R = 135;
byte lim3L = 65, lim3R = 125; // +/-25° is still full throttle with the JMT-10A ESC! (Forward, Reverse)
byte lim3Llow = 80, lim3Rlow = 110; 
byte lim4L = 55, lim4R = 75; // Tractor trailer coupler unlocking limits (L = "Back / Pulse" button pressed)
#define TRACTOR_TRAILER_UNLOCK // Vehicle has a trailer unlocking servo, switched by servo CH4.
//Not usable in combination with the "beacons" and "potentiometer" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Wltoys A959---------------------------------------------------------------------------
#ifdef CONFIG_A959
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.45; // Regulated 5.0V supply from the ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 5; // MRSC Vehicle!

// Lights
boolean escBrakeLights = true;
boolean tailLights = true;
boolean headLights = true;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 62, lim1R = 133; // R80, L125
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = true;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = true;

// Servo limits
byte lim1L = 145, lim1R = 35;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration (lift in this case)
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;
// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = true;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
float boardVersion = 1.3;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 2; // 2 = caterpillar mode

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
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
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// PIPER J3 CUB Plane-----------------------------------------------------------------------
#ifdef PIPER_J3
// Battery type
boolean liPo = true;
float cutoffVoltage = 3.1;

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 10;

// Vehicle type
byte vehicleType = 6; // 6 = differential thrust controlled plane

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 45, lim3Rlow = 135; // no limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 3;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:12 MN Model Landrover Defender D90 with 2 speed transmission & ESP32 sound controller (everything is controlled via SBUS)------------------------------
#ifdef CONFIG_MN_D90
// Battery type
boolean liPo = false;
float cutoffVoltage = 0.0; // 5V receiver supply voltage, but sound controller is handling cutoff

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 18;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits (not used, servos controlled via SBUS)
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 45, lim1C = 99, lim1R = 135; // R56 C91 L120
byte lim2L = 15, lim2R = 104; // Gearbox shifter limits (1. and 2. gear, 41, 123)
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 150, lim3Rlow = 35; // 2 speed transmission, so same values!
byte lim4L = 45, lim4R = 135;
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:10 RGT EX86100 Jeep Wrangler with ESP32 sound controller  (everything is controlled via SBUS)------------------------------
#ifdef CONFIG_RGT_EX86100
// Battery type
boolean liPo = false;
float cutoffVoltage = 0.0; // 6V receiver supply voltage, but ESC is handling cutoff

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 19;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits (not used, servos controlled via SBUS)
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 56, lim1C = 92, lim1R = 120; // R56 C91 L120
byte lim2L = 15, lim2R = 104; // Gearbox shifter limits (1. and 2. gear, 41, 123)
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 150, lim3Rlow = 35; // 2 speed transmission, so same values!
byte lim4L = 45, lim4R = 135;
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// 1:14 WPL C44KM Toyota (Gearbox switched with mode 1)------------------------------
#ifdef CONFIG_WPL_C44
// Battery type
boolean liPo = false;
float cutoffVoltage = 0.0; // 6V receiver supply voltage, but Sound module causes a lot of noise!

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 20;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
#define STEERING_3_POINT_CAL // steering center point is separately adjustable
byte lim1L = 56, lim1C = 92, lim1R = 120; // R56 C91 L120
byte lim2L = 15, lim2R = 104; // Gearbox shifter limits (1. and 2. gear, 41, 123)
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 150, lim3Rlow = 35; // 2 speed transmission, so same values!
byte lim4L = 45, lim4R = 135;
#define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCEISO'S MECCANO VEHICLES ***********************************************************************************
// https://www.youtube.com/channel/UCuzV4gX5T-5FzYAQXltWYnA ******************************************************

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
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCAPILLAR-----------------------------------------------------------------
#ifdef CONFIG_MECCAPILLAR
// Battery type
boolean liPo = false;
boolean cutoffVoltage = 3.5;

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 1;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 45, lim3Rlow = 135; // no limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO CRANE CHASSIS-----------------------------------------------------------------
#ifdef CONFIG_CRANE_CHASSIS
// Battery type
boolean liPo = false; // ESC provides protection
boolean cutoffVoltage = 3.5;

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 3;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 135, lim1R = 45; // Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO CRANE TOWER-----------------------------------------------------------------
#ifdef CONFIG_CRANE_TOWER
// Battery type
boolean liPo = false;
boolean cutoffVoltage = 3.5;

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 4;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 45, lim3Rlow = 135; // no limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO CAR 5 Willys Jeep -----------------------------------------------------------
#ifdef CONFIG_MECCANO_CAR_5
// Battery type
boolean liPo = false; // ESC provides protection
boolean cutoffVoltage = 3.5;

// Board type
float boardVersion = 1.2;
boolean HP = false;

// Vehicle address
int vehicleNumber = 5;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 135, lim1R = 45; // Steering reversed
byte lim2L = 45, lim2R = 135;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO CAR 6 with Dumbo RC 10A ESC-------------------------------------------------------
#ifdef CONFIG_MECCANO_CAR_6
// Battery type
boolean liPo = false; // Protected ESC!
float cutoffVoltage = 3.5; // 2S LiPo, Receiver 5V supply from ESC

// Board type
float boardVersion = 1.4;
boolean HP = false;

// Vehicle address
int vehicleNumber = 6;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135; // Steering reversed
byte lim2L = 45, lim2R = 135;
//byte lim3L = 65, lim3R = 125; // +/-25° is still full throttle with the JMT-10A ESC! (Forward, Reverse)
//byte lim3Llow = 80, lim3Rlow = 110;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// MECCANO CAR 7 with Dumbo RC 10A ESC-------------------------------------------------------
#ifdef CONFIG_MECCANO_CAR_7
// Battery type
boolean liPo = false; // Protected ESC!
float cutoffVoltage = 3.5; // 2S LiPo, Receiver 5V supply from ESC

// Board type
float boardVersion = 1.5;
boolean HP = false;

// Vehicle address
int vehicleNumber = 7;

// Vehicle type
byte vehicleType = 0;

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = true;
boolean indicators = true;
boolean beacons = false;

// Servo limits
byte lim1L = 45, lim1R = 135; // Steering reversed
byte lim2L = 45, lim2R = 135;
//byte lim3L = 65, lim3R = 125; // +/-25° is still full throttle with the JMT-10A ESC! (Forward, Reverse)
//byte lim3Llow = 80, lim3Rlow = 110;
byte lim3L = 150, lim3R = 35; // ESC output signal reversed
byte lim3Llow = 115, lim3Rlow = 70; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
int maxAccelerationFull = 3;
int maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255;

// Motor 2 PWM frequency
byte pwmPrescaler2 = 32;

// Additional Channels
boolean TXO_momentary1 = true;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

// Meccano dumper with ESP32 Sound Controller in SBUS mode (ESC controlled by ESP32) -----------------
#ifdef CONFIG_MECCANO_DUMPER
// Battery type
boolean liPo = false; // LiPo is protected by ESC
float cutoffVoltage = 4.0; // 5V supply

// Board type
float boardVersion = 1.5;
boolean HP = false;

// Vehicle address
int vehicleNumber = 8;

// Vehicle type
byte vehicleType = 3; // Forklift mode also used for dumper!
#define VEHICLE_TYPE_3_WITH_ESC // Vehicle with ESC, motor driver 1 is used for other stuff

// Lights
boolean escBrakeLights = false;
boolean tailLights = false;
boolean headLights = false;
boolean indicators = false;
boolean beacons = false;

// Servo limits
byte lim1L = 135, lim1R = 45;
byte lim2L = 45, lim2R = 135;
byte lim3L = 45, lim3R = 135;
byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles!
byte lim4L = 45, lim4R = 135;

// Motor configuration
int maxPWMfull = 255;
int maxPWMlimited = 170;
int minPWM = 0;
byte maxAccelerationFull = 7;
byte maxAccelerationLimited = 12;

// Variables for self balancing (vehicleType = 4) only!
float tiltCalibration = 0.0;

// Steering configuration
byte steeringTorque = 255; // Full power for motor driver

// Motor 2 PWM frequency
byte pwmPrescaler2 = 8; // 3936Hz

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = false;

// Engine sound
boolean engineSound = false;

// Tone sound
boolean toneOut = false;
#endif

#endif
