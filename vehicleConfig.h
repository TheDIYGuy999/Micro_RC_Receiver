#ifndef vehicleConfig_h
#define vehicleConfig_h

#include "Arduino.h"

const byte configID = 2; // <- Select the correct vehicle configuration ID here before uploading!

//
// =======================================================================================================
// DECLARATION OF VEHICLE SPECIFIC CONFIGURATION VARIABLES
// =======================================================================================================
//

// Battery type
boolean liPo; // If "true", the vehicle can't be reactivated once the cutoff voltage is reached
float cutoffVoltage; // Min. battery discharge voltage

// Board type
boolean battSense; // "true", if voltage divider resistors for battery voltage sensing are wired to input "A7"

// Vehicle address
int vehicleNumber; // This number must be unique for each vehicle!

// Vehicle type
byte vehicleType; // 0 = car, 1 = semi caterpillar, 2 = caterpillar

// Lights
boolean tailLights; // Caution: the taillights are wired to the servo pin 2! -> Servo 2 not usable, if "true"
boolean headLights; // Caution: the headlights are wired to the RXI pin! -> Serial not usable, if "true"

// Servo limits (45 - 135 means - 45° to 45° from the servo middle position)
byte lim1L, lim1R; // Servo 1
byte lim2L, lim2R;
byte lim3L, lim3R;
byte lim4L, lim4R; // Servo 4

// Motor configuration
int maxPWMfull; // (100% PWM is 255)
int maxPWMlimited;
byte maxAccelerationFull;// (ms per 1 step input signal change)
byte maxAccelerationLimited;

// Steering configuration (100% torque is 255)
byte steeringTorque;

// Additional Channels
boolean TXO_momentary1; // The TXO output is linked to the momentary1 channel! -> Serial not usable, if "true"
boolean potentiometer1;

//
// =======================================================================================================
// VEHICLE SPECIFIC CONFIGURATIONS
// =======================================================================================================
//

void setupVehicle() {
  switch (configID) {
    // Vehicle ID 0 (generic configuration without battery sensing)--------------------------------------
    case 0:
      // Battery type
      liPo = false;
      cutoffVoltage = 2.9;

      // Board type
      battSense = false;

      // Vehicle address
      vehicleNumber = 1;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = false;

      // Servo limits
      lim1L = 45; lim1R = 135;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 0;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 1 (battery sense test)------------------------------------------------------------------
    case 1:
      // Battery type
      liPo = true;
      cutoffVoltage = 2.9;

      // Board type
      battSense = true;

      // Vehicle address
      vehicleNumber = 1;
      
      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;

      // Servo limits
      lim1L = 45; lim1R = 135;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 3;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 2 (95 "DINOCO")-------------------------------------------------------------------------
    case 2:
      // Battery type
      liPo = false;
      cutoffVoltage = 2.9;

      // Board type
      battSense = false; // No sensing resistors, board revision 1.0!

      // Vehicle address
      vehicleNumber = 2;
      
      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;

      // Servo limits
      lim1L = 45; lim1R = 135;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 7;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 3 (Maisto Mustang GT Fastback)---------------------------------------------------------------
    case 3:
      // Battery type
      liPo = true;
      cutoffVoltage = 2.9;

      // Board type
      battSense = true;

      // Vehicle address
      vehicleNumber = 3;
      
      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;

      // Servo limits
      lim1L = 60; lim1R = 129;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 7;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 4 (Maisto Dodge Challenger)-------------------------------------------------------------------
    case 4:
      // Battery type
      liPo = true;
      cutoffVoltage = 2.9;

      // Board type
      battSense = true;

      // Vehicle address
      vehicleNumber = 4;
      
      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = true;
      headLights = true;

      // Servo limits
      lim1L = 120; lim1R = 55;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 7;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 5 (GearGmax / KIDZTECH TOYS Porsche GT3 RS 4.0)-----------------------------------------------
    case 5:
      // Battery type
      liPo = false;
      cutoffVoltage = 2.9;

      // Board type
      battSense = true;

      // Vehicle address
      vehicleNumber = 5;
      
      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;

      // Servo limits
      lim1L = 45; lim1R = 135;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 3;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

      // Vehicle ID 10 (Caterpillar vecicle test)--------------------------------------------------------------
    case 10:
      // Battery type
      liPo = false;
      cutoffVoltage = 2.9;

      // Board type
      battSense = false;

      // Vehicle address
      vehicleNumber = 10;
      
      // Vehicle type
      vehicleType = 2; // 2 = caterpillar mode

      // Lights
      tailLights = false;
      headLights = true;

      // Servo limits
      lim1L = 45; lim1R = 135;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 255;
      maxPWMlimited = 170;
      maxAccelerationFull = 3;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

  }
}

#endif
