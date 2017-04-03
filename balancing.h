#ifndef balancing_h
#define balancing_h

#include "Arduino.h"

/* This code is based on Joop Brokkings excellent work:
http://www.brokking.net/imu.html
https://www.youtube.com/watch?v=4BoIE8YQwM8
https://www.youtube.com/watch?v=j-kE0AMEWy4

I (TheDIYGuy999) have modified it to fit my needs

This header file is required for the "balancing" (vehicleType = 4) option in the vehicleConfig.h
Connect an MPU-6050 sensor to GND, VDD (3.3 and 5V compatible), SCL and SDA.
Mount it as close as possible to the pivot point of your vehicle

-->> Note, that the receiver will not work, if vehicleType is set to 4 and no MPU-6050 sensor is wired up!!
*/

//
// =======================================================================================================
// GLOBAL VARIABLES
// =======================================================================================================
//

// 6050 variables
int gyro_x, gyro_y, gyro_z;
long acc_x_raw, acc_y_raw, acc_z_raw;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;
float angle_pitch, angle_roll;
boolean set_gyro_angles;
float angle_roll_acc, angle_pitch_acc;
float yaw_rate;

//Define PID Variables
double speedTarget, speedMeasured, speedOutput;
double angleTarget, angleMeasured, angleOutput;

double speedAverage;

// PID controllers (you may have to change their parameters in balancing() )
//Kp: proportional (instantly), Ki: integral (slow, precise), Kd: deriative (speed of difference)
PID speedPid(&speedMeasured, &speedOutput, &speedTarget, 0.0, 0.0, 0.0, DIRECT); // Speed: outer, slow control loop
PID anglePid(&angleMeasured, &angleOutput, &angleTarget, 0.0, 0.0, 0.0, DIRECT); // Angle: inner, fast control loop

// configuration variables (you may have to change them)
const int calibrationPasses = 500;
const float pitchCal = 2.5; // offset to zero in ° 1.25 (adjust, until jour robot does not move in throttle neutral pos.)
const float rollCal = 0.3; // offset to zero in ° 0.3

//
// =======================================================================================================
// PRINT DEBUG DATA
// =======================================================================================================
//

void writeDebug() {
#ifdef DEBUG
  static unsigned long lastPrint;
  if (millis() - lastPrint >= 250) {
    lastPrint = millis();

    Serial.print("P: ");
    Serial.print(angle_pitch);    //Print pitch
    Serial.print("   R: ");
    Serial.print(angle_roll);    //Print roll
    Serial.print("   Motor: ");
    Serial.println(angleOutput);    //Print Motor output
  }
#endif
}

//
// =======================================================================================================
// PROCESS MPU 6050 DATA SUBFUNCTION
// =======================================================================================================
//

void processMpu6050Data() {
  gyro_x -= gyro_x_cal;                                                // Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                // Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                // Subtract the offset calibration value from the raw gyro_z value

  //Gyro angle calculations
  //0.0000611 * 4 = 1 / (250Hz / 65.5)
  angle_pitch += gyro_x * 0.0002444;                                   // Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y * 0.0002444;                                    // Calculate the traveled roll angle and add this to the angle_roll variable
  yaw_rate = gyro_z * 0.0002444;                                       // Yaw rate in degrees per second

  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               // If the IMU has yawed transfer the roll angle to the pitch angle
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               // If the IMU has yawed transfer the pitch angle to the roll angle

  //Accelerometer reading averaging to improve vibration resistance (added by TheDIYGuy999)
  acc_x = (acc_x * 4 + acc_x_raw) / 5;
  acc_y = (acc_y * 4 + acc_y_raw) / 5;
  acc_z = (acc_z * 4 + acc_z_raw) / 5;
  
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x * acc_x) + (acc_y * acc_y) + (acc_z * acc_z)); // Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  if(abs(acc_y) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;          //Calculate the pitch angle.
  }
  if(abs(acc_x) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;          //Calculate the roll angle.
  }

  //Place the MPU-6050 spirit level and note the values in the following two lines for calibration
  angle_pitch_acc += pitchCal;                                         // Accelerometer calibration value for pitch
  angle_roll_acc += rollCal;                                           // Accelerometer calibration value for roll

  if (set_gyro_angles) {                                               // If the IMU is already started
    angle_pitch = angle_pitch * 0.995 + angle_pitch_acc * 0.005;       // Correct the drift of the gyro pitch angle with the accelerometer pitch angle // 0.995 0.005
    angle_roll = angle_roll * 0.995 + angle_roll_acc * 0.005;          // Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else {                                                               // At first start
    angle_pitch = angle_pitch_acc;                                     // Set the gyro pitch angle equal to the accelerometer pitch angle
    angle_roll = angle_roll_acc;                                       // Set the gyro roll angle equal to the accelerometer roll angle
    set_gyro_angles = true;                                            // Set the IMU started flag
  }

  angleMeasured = angle_pitch;
}

//
// =======================================================================================================
// READ MPU 6050 RAW DATA FUNCTION
// =======================================================================================================
//

void readMpu6050Data() {
  static unsigned long lastReading;
  if (micros() - lastReading >= 4000) {                                  // Read the data every 4000us (equals 250Hz)
    lastReading = micros();
    
    Wire.beginTransmission(0x68);                                        // Start communicating with the MPU-6050
    Wire.write(0x3B);                                                    // Send the requested starting register
    Wire.endTransmission();                                              // End the transmission
    Wire.requestFrom(0x68, 14);                                          // Request 14 bytes from the MPU-6050
    while (Wire.available() < 14);                                       // Wait until all the bytes are received
    acc_x_raw = Wire.read() << 8 | Wire.read();                              // Add the low and high byte to the acc_x variable
    acc_y_raw = Wire.read() << 8 | Wire.read();                              // Add the low and high byte to the acc_y variable
    acc_z_raw = Wire.read() << 8 | Wire.read();                              // Add the low and high byte to the acc_z variable
    temperature = Wire.read() << 8 | Wire.read();                        // Add the low and high byte to the temperature variable
    gyro_x = Wire.read() << 8 | Wire.read();                             // Add the low and high byte to the gyro_x variable
    gyro_y = Wire.read() << 8 | Wire.read();                             // Add the low and high byte to the gyro_y variable
    gyro_z = Wire.read() << 8 | Wire.read();                             // Add the low and high byte to the gyro_z variable

    processMpu6050Data();                                                // Process the MPU 6050 data
  }
}

//
// =======================================================================================================
// MPU 6050 SETUP
// =======================================================================================================
//

void setupMpu6050() {

  Wire.begin();                                                        // Start I2C as master

  // Activate the MPU-6050
  Wire.beginTransmission(0x68);                                        // Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    // Send the requested starting register
  Wire.write(0x00);                                                    // Set the requested starting register
  Wire.endTransmission();                                              // End the transmission
  // Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        // Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    // Send the requested starting register
  Wire.write(0x10);                                                    // Set the requested starting register
  Wire.endTransmission();                                              // End the transmission
  // Configure the gyro (2000° per second full scale)
  Wire.beginTransmission(0x68);                                        // Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    // Send the requested starting register
  Wire.write(0x18);                                                    // Set the requested starting register
  Wire.endTransmission();                                              // End the transmission

  // Calibrate the gyro (the vehicle must stay steady during this time!)
  delay(1500);                                                         // Delay 1.5 seconds to display the text
#ifdef DEBUG
  Serial.println("Calibrating gyro");                                  // Print text to console
#endif

  int cal_int = 0;
  while (cal_int < calibrationPasses) {                                // Run the calibrating code X times
    static unsigned long lastGyroCal;
    if (micros() - lastGyroCal >= 4000) {                              // Read the data every 4000us (equals 250Hz)
#ifdef DEBUG
      if (cal_int % (calibrationPasses / 32) == 0)Serial.print(".");   // Print a dot every X readings
#endif
      readMpu6050Data();                                               // Read the raw acc and gyro data from the MPU-6050
      gyro_x_cal += gyro_x;                                            // Add the gyro x-axis offset to the gyro_x_cal variable
      gyro_y_cal += gyro_y;                                            // Add the gyro y-axis offset to the gyro_y_cal variable
      gyro_z_cal += gyro_z;                                            // Add the gyro z-axis offset to the gyro_z_cal variable
      lastGyroCal = micros();
      cal_int ++;
    }
  }
  gyro_x_cal /= calibrationPasses;                                      // Divide the gyro_x_cal variable by X to get the avarage offset
  gyro_y_cal /= calibrationPasses;                                      // Divide the gyro_y_cal variable by X to get the avarage offset
  gyro_z_cal /= calibrationPasses;                                      // Divide the gyro_z_cal variable by X to get the avarage offset

#ifdef DEBUG
  Serial.println("done!");
#endif
}

//
// =======================================================================================================
// PID SETUP
// =======================================================================================================
//

void setupPid() {

  // Speed control loop
  speedPid.SetSampleTime(4); // calcualte every 4ms = 250Hz
  speedPid.SetOutputLimits(-28, 28); // output range from -28 to 28 (same as in balancing() )
  speedPid.SetMode(AUTOMATIC);

  // Angle control loop
  anglePid.SetSampleTime(4); // calcualte every 4ms = 250Hz
  anglePid.SetOutputLimits(-43, 43); // output range from -43 to 43 for motor
  anglePid.SetMode(AUTOMATIC);
}

#endif
