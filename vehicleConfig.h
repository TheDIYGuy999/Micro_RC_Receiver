#ifndef vehicleConfig_h
#define vehicleConfig_h

#include "Arduino.h"

const byte configID = 0; // <- Select the correct vehicle configuration ID here before uploading!

//
// =======================================================================================================
// DECLARATION OF VEHICLE SPECIFIC CONFIGURATION VARIABLES
// =======================================================================================================
//

// Battery type
boolean liPo; // If "true", the vehicle can't be reactivated once the cutoff voltage is reached
float cutoffVoltage; // Min. battery discharge voltage, or min. VCC, if board rev. < 1.2

// Board type
float boardVersion; // Board revision (MUST MATCH WITH YOUR BOARD REVISION!!)
boolean HP; // HP = "High Power" version (both TB6612FNG channels wired in parallel) -> No motor 1, motor 2 is the driving motor

// Vehicle address
int vehicleNumber; // This number must be unique for each vehicle!

// Vehicle type
byte vehicleType; // 0 = car, 1 = semi caterpillar, 2 = caterpillar (0 only on HP version)

// Lights
boolean tailLights; // Caution: the taillights are wired to the servo pin 2! -> Servo 2 not usable, if "true"
boolean headLights; // Caution: the headlights are wired to the RXI pin! -> Serial not usable, if "true"
boolean indicators; // Caution: the indicators are wired to the SDA / SCL pins! -> I2C not usable, if "true"

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

// Motor 2 PWM frequency: 32 = 984Hz (default), 8 = 3936Hz, 1 = 31488Hz (only with board version. 1.3 or newer)
byte pwmPrescaler2; // Motor 2 = steering motor (or driving motor in "HP" High Power board version)

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
    // Vehicle ID 0 (generic configuration without battery sensing, board version <1.2)-------------------
    case 0:
      // Battery type
      liPo = false;
      cutoffVoltage = 3.1;

      // Board type
      boardVersion = 1.0;
      HP = false;

      // Vehicle address
      vehicleNumber = 1;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;
      indicators = true;

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

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 1 (MECCANO standard configuration)-----------------------------------------------------
    case 1:
      // Battery type
      liPo = false;
      cutoffVoltage = 3.3;

      // Board type
      boardVersion = 1.2;
      HP = false;

      // Vehicle address
      vehicleNumber = 1;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;
      indicators = true;

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

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 2 (95 "DINOCO")-------------------------------------------------------------------------
    case 2:
      // Battery type
      liPo = false;
      cutoffVoltage = 3.1;

      // Board type
      boardVersion = 1.0;
      HP = false;

      // Vehicle address
      vehicleNumber = 2;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;
      indicators = false;

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

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 3 (Maisto Mustang GT Fastback)---------------------------------------------------------------
    case 3:
      // Battery type
      liPo = true;
      cutoffVoltage = 3.3;

      // Board type
      boardVersion = 1.2;
      HP = false;

      // Vehicle address
      vehicleNumber = 3;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;
      indicators = false;

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

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 4 (Maisto Dodge Challenger)-------------------------------------------------------------------
    case 4:
      // Battery type
      liPo = true;
      cutoffVoltage = 3.5;

      // Board type
      boardVersion = 1.3;
      HP = true; // High Power Board!

      // Vehicle address
      vehicleNumber = 4;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = true;
      headLights = true;
      indicators = true;

      // Servo limits
      lim1L = 120; lim1R = 55;
      lim2L = 45; lim2R = 135;
      lim3L = 45; lim3R = 135;
      lim4L = 45; lim4R = 135;

      // Motor configuration
      maxPWMfull = 240;
      maxPWMlimited = 170;
      maxAccelerationFull = 7;
      maxAccelerationLimited = 12;

      // Steering configuration
      steeringTorque = 255;

      // Motor 2 PWM frequency
      pwmPrescaler2 = 1;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 5 (GearGmax / KIDZTECH TOYS Porsche GT3 RS 4.0)-----------------------------------------------
    case 5:
      // Battery type
      liPo = false;
      cutoffVoltage = 3.3;

      // Board type
      boardVersion = 1.2;
      HP = false;

      // Vehicle address
      vehicleNumber = 5;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;
      indicators = false;

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

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 6 (Coke Can Car)-----------------------------------------------
    case 6:
      // Battery type
      liPo = true;
      cutoffVoltage = 3.3;

      // Board type
      boardVersion = 1.2;
      HP = false;

      // Vehicle address
      vehicleNumber = 6;

      // Vehicle type
      vehicleType = 0;

      // Lights
      tailLights = false;
      headLights = true;
      indicators = false;

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
      steeringTorque = 160;

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

    // Vehicle ID 10 (Caterpillar vecicle test)--------------------------------------------------------------
    case 10:
      // Battery type
      liPo = false;
      cutoffVoltage = 3.1;

      // Board type
      boardVersion = 1.0;
      HP = false;

      // Vehicle address
      vehicleNumber = 10;

      // Vehicle type
      vehicleType = 2; // 2 = caterpillar mode

      // Lights
      tailLights = false;
      headLights = true;
      indicators = true;

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

      // Motor 2 PWM frequency
      pwmPrescaler2 = 32;

      // Additional Channels
      TXO_momentary1 = true;
      potentiometer1 = true;
      break;

  }
}

#endif
