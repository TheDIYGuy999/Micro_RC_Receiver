
#ifndef vehicleConfig_h
#define vehicleConfig_h

#include "Arduino.h"

#define CONFIG_KING_HAULER // <- Select the correct vehicle configuration here before uploading!

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

  // Vehicle address
  int vehicleNumber; // This number must be unique for each vehicle!

  // Servo limits (45 - 135 means - 45° to 45° from the servo middle position)
  byte lim1L, lim1R; // Servo 1
  byte lim2L, lim2C, lim2R; // Lim2C for THREE_SPEED_GEARBOX option (center position)
  byte lim3L, lim3R;
  byte lim3Llow = 75, lim3Rlow = 105; // limited top speed angles for external ESC
  byte lim4L, lim4R; // Servo 4, also used for tractor trailer unlocking servo (TRACTOR_TRAILER_UNLOCK option)
  #define TWO_SPEED_GEARBOX // Vehicle has a mechanical 2 speed shifting gearbox, switched by servo CH2. Not usable in combination with the "tailLights" option
  #define THREE_SPEED_GEARBOX // Vehicle has a mechanical 3 speed shifting gearbox, switched by servo CH2. Not usable in combination with the "tailLights" option

  // Additional Channels (Serial command mode for ESP32 sound and light controller is active, if TXO booleans are "false"
  boolean TXO_momentary1; // The TXO output is linked to the momentary1 channel! -> Serial not usable, if "true"
  boolean TXO_toggle1; // The TXO output is linked to the toggle1 channel! -> Serial not usable, if "true"
  boolean potentiometer1; // The potentiometer knob on the transmitter is linked to the servo output CH4 instead of CH4 joystick
*/

// Tamiya King Hauler Truck with ESP32 Sound Controller in Serial mode (ESC controlled by ESP32) -----------------
#ifdef CONFIG_KING_HAULER
// Battery type
boolean liPo = false; // LiPo is protected by ESC
float cutoffVoltage = 4.0; // 5V supply

// Board type
float boardVersion = 1.5;

// Vehicle address
int vehicleNumber = 5;

// Servo limits
byte lim1L = 135, lim1R = 58; // Steering R 135, L 58
byte lim2L = 143, lim2C = 90, lim2R = 37; // 3 speed gearbox shifting servo (3., 2., 1. gear)
byte lim3L = 135, lim3R = 45; // ESC
byte lim3Llow = 105, lim3Rlow = 75; // limited top speed ESC angles!
byte lim4L = 45, lim4R = 135; // Controlled by pot, for sound triggering!
#define THREE_SPEED_GEARBOX // Vehicle has a mechanical 3 speed shifting gearbox, switched by servo CH2.
// Not usable in combination with the "tailLights" option

// Additional Channels
boolean TXO_momentary1 = false;
boolean TXO_toggle1 = false;
boolean potentiometer1 = true;

#endif

#endif
