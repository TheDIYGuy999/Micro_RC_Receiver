// 4 Channel "Micro RC" Receiver with 4 standard RC Servo Outputs
// ATMEL Mega 328P TQFP 32 soldered directly to the board, 8MHz external resonator,
// 2.4GHz NRF24L01 SMD radio module
// "Light" version without: self balancing, stability control, lights, TB6612FNG motor controller
// Intended for use in combination with ESP32 sound and light controller in serial communication mode:
// https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32

// See: https://www.youtube.com/playlist?list=PLGO5EJJClJBCjIvu8frS7LrEU3H2Yz_so

// * * * * N O T E ! The vehicle specific configurations are stored in "vehicleConfig.h" * * * *

const float codeVersion = 3.32; // Software revision (see https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/README.md)

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
//#include <Wire.h> // I2C library (for the MPU-6050 gyro /accelerometer)
#include <RF24.h> // Installed via Tools > Board > Boards Manager > Type RF24
#include <printf.h> // Required for NRF24 debug
#include <Servo.h>

// Tabs (header files in sketch directory)
#include "readVCC.h"
#include "vehicleConfig.h"
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

// Serial commands to light and sound controller
boolean serialCommands;

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
// MAIN ARDUINO SETUP (1x during startup)
// =======================================================================================================
//

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
  printf_begin();
  serialCommands = false;
  delay(3000);
#endif

#ifndef DEBUG
  // If TXO pin or RXI pin is used for other things, disable Serial
  if (TXO_momentary1 || TXO_toggle1) {
    Serial.end(); // make sure, serial is off!
    UCSR0B = 0b00000000;
    serialCommands = false;
  }
  else { // Otherwise use it for serial commands to the light and sound controller
    Serial.begin(115200);
    serialCommands = true;
  }
#endif

  // Radio setup
  setupRadio();

  // Servo pins
  servo1.attach(A0);
  servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3);

  // All axes to neutral position
  data.axis1 = 50;
  data.axis2 = 50;
  data.axis3 = 50;
  data.axis4 = 50;
  data.pot1 = 50; // Added in v3.32

  // Special functions
  if (TXO_momentary1 || TXO_toggle1) pinMode(DIGITAL_OUT_1, OUTPUT);

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
  servo1.write(map(data.axis1, 100, 0, lim1L, lim1R) ); // 45 - 135°

  // Elevator or shifting gearbox actuator
#ifdef TWO_SPEED_GEARBOX // Shifting gearbox mode, controlled by "Mode 1" button
  if (!tailLights) {
    if (data.mode1)servo2.write(lim2L);
    else servo2.write(lim2R);
  }

#else
#ifdef THREE_SPEED_GEARBOX // Shifting gearbox mode, controlled by 3 position switch
  if (data.axis2 < 10)servo2.write(lim2R);
  else if (data.axis2 > 90)servo2.write(lim2L);
  else servo2.write(lim2C);

#else // Servo controlled by joystick CH2
  if (!tailLights) servo2.write(map(data.axis2, 100, 0, lim2L, lim2R) ); // 45 - 135°
#endif
#endif

  // Throttle (for ESC control)
  if (data.mode1) { // limited speed!
    servo3.write(map(data.axis3, 100, 0, lim3Llow, lim3Rlow ) ); // less than +/- 45°
  }
  else { // full speed!
    servo3.write(map(data.axis3, 100, 0, lim3L, lim3R) ); // 45 - 135°
  }

  // Rudder or trailer unlock actuator
#ifdef TRACTOR_TRAILER_UNLOCK // Tractor trailer unlocking, controlled by "Momentary 1" ("Back / Pulse") button
  if (data.momentary1)servo4.write(lim4L);
  else servo4.write(lim4R);

#else // Servo controlled by joystick CH4 
  if (!potentiometer1) { // Servo 4 controlled by CH4
    servo4.write(map(data.axis4, 100, 0, lim4L, lim4R) ); // 45 - 135°
  }
  else { // Servo 4 controlled by transmitter potentiometer knob
    servo4.write(map(data.pot1, 0, 100, 45, 135) ); // 45 - 135°
  }
#endif

}

//
// =======================================================================================================
// WRITE DIGITAL OUTPUTS (SPECIAL FUNCTIONS)
// =======================================================================================================
//

void digitalOutputs() {

  static boolean wasPressed;

  if (TXO_momentary1) { // only, if momentary function is enabled in vehicle configuration
    if (data.momentary1) {
      digitalWrite(DIGITAL_OUT_1, HIGH);
    }
    else digitalWrite(DIGITAL_OUT_1, LOW);
  }

  if (TXO_toggle1) { // only, if toggle function is enabled in vehicle configuration

    if (data.momentary1  && !wasPressed) {
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

boolean battSense;

void checkBattery() {

  if (boardVersion < 1.2) battSense = false;
  else battSense = true;

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
  raw[0] = analogRead(BATTERY_DETECT_PIN); // else take the real voltage (compensates voltage drop while driving)
  float average = (raw[0] + raw[1] + raw[2] + raw[3] + raw[4] + raw[5]) / 619.999; // 1023steps / 9.9V * 6 = 619.999
  return average;
}

//
// =======================================================================================================
// SERIAL COMMANDS TO LIGHT- & SOUND CONTROLLER (if not DEBUG, not TXO_momentary1, not TXO_toggle1)
// =======================================================================================================
//

void sendSerialCommands() {

  static unsigned long lastSerialTime;

  // '\n' is used as delimiter (separator of variables) during parsing on the sound controller
  // it is generated by the "println" (print line) command!
  // See: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32

  if (serialCommands) { // only, if we are in serial command mode
    if (millis() - lastSerialTime > 20) { // Send the data every 20ms
      lastSerialTime = millis();
      Serial.print('<'); // Start marker
      Serial.println(data.axis1);
      Serial.println(data.axis2);
      Serial.println(data.axis3);
      Serial.println(data.axis4);
      Serial.println(data.pot1);
      Serial.println(data.mode1);
      Serial.println(data.mode2);
      Serial.println(data.momentary1);
      Serial.println(hazard);
      Serial.println(left);
      Serial.println(right);
      Serial.print('>'); // End marker
    }
  }
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

  // Battery check
  checkBattery();

  // Digital Outputs (special functions)
  digitalOutputs();

  // Send serial commands
  sendSerialCommands();
}
