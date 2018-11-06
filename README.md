# This is an ATMEL Mega328P / NRF24L01+ based 2.4GHz radio receiver
## Features:
- Programmable with Arduino IDE
- 4 RC servo connectors
- integrated TB6612FNG dual dc motor driver
- serial header
- I2C header (for example for the supported MPU-6050)
- ICSP header for programming with Arduino IDE (use "TheDIYGuy999 328P" board definition and set brownout voltage to 1.8V)
- NRF24L01+ SMD module integrated
- very small size
- Eagle files are also provided, if you want to build your own receiver!

See: https://www.youtube.com/playlist?list=PLGO5EJJClJBCjIvu8frS7LrEU3H2Yz_so

New in V 1.3:
- Vehicle configuration is now stored in "vehicleConfig.h", so the main code always stays the same.
- Crude frequency hopping, if bad receiving quality is detected (needs to be improved).

New in V 1.4:
- 10 vehicle addresses instead of 5
- More vehicle configuration options
- The "BACK" button on the transmitter can now be used (if the menu screen is not displayed on the OLED) as a momentary button, for example for a horn on the vehicle. It is sent as "data.momentary1"
- SimpleTimer library replaced with my own code, which uses significantly less memory

New in V 1.5:
- Added steering mixer for semi caterpillar and caterpillar vehicles such as tanks, diggers etc.
- Channel 3 is throttle, channel 1 is steering
- More vehicle configuration options in vehicleVonfig.h
- The left caterpillar is motor 2, the right is motor 1
- Steering servo 1 can be used in parallel (for semi caterpillar vehicles or vehicles with electronic differential)

New in V 1.6:
- Added indicator lights - wired to A4 (SDA) / A5 (SCL) ports
- enabled and disabled left / right by channel 4 left / right
- disabled by turning back the steering wheel (channel 1) - just like a real car
- hazard lights (all indicators flashing), if no connection to the transmitter

New in V 1.61:
- Battery cutoff bug fixed

New in V 1.7 (Support for board revision 1.3):
- **** TB6612FNG library update to V1.1 is required! ****
- motor driver pinout has changed in board V1.3. So we are now able to adjust the PWM frequency of motor2. This allows smooth motor operation without the typical PWM whining.
- vehicleConfig.h changed: "boardVersion" and "HP" variables added. They allow to stay compatible with older board versions. Just select the correct configuration.
- The new "HP" (High Power) board version has only one motor driver channel, because both TB6612FNG channels are wired in parallel. It's able to supply 2.4A (average) / 6.4A (peak) so we can use bigger motors. This is useful for models, which use a steering servo and don't need a second motor channel.'
- automatic radio re-initialisation after signal timeout

New in V 1.71
- Battery monitoring and cutoff triggering improved. Under load, 0.3V are now added to the battery voltage to compensate the voltage drop. This allows to set the cutoff voltage to 3.6V, even with bigger loads.
- Vehicle #7 added.

New in V 1.8
- New "engineSound" variable in "vehicleconfig.h". True means, that the servo 3 pulse signal can be switched on and off, using the  channel 2 joystick. This switches the engine sound on my "Rc Engine Sound" unit on and off.

New in V 1.9
- New vehicle specific configuration section in vehicleConfig.h. The configuration is now done entirely during compilation time.

New in V 2.0
- vehicle Config.h: New vehicleType 3 (forklift) added. Motor 2 is used as lifting motor in this case. A servo is required as steering motor.
- Beacon lights can be wired to servo channel 4. Select "beacons = true" in this case. The beacons are flashing during vehicle movement and are switching off, after the vehicle did not move for 10s.

New in V 2.1
- Sounds for a STAR WARS R2-D2 robot can now be generated on servo channel 3
- "toneOut" variable added to vehicleConfig.h. True means, that servo 3 channel is generating sounds instead of a servo signal.
- Do not enable "toneOut" and "engineSound" at the same time.
- Note: the Arduino tone() function is blocking. So don't play sounds while driving ;-)

New in V 2.2
- **** TB6612FNG library update to V1.2 is required! **** https://github.com/TheDIYGuy999/TB6612FNG
- **** The PID library is required! **** https://github.com/br3ttb/Arduino-PID-Library/
- Support for self balancing (inverted pendulum, segway) vehicles. See: https://www.youtube.com/watch?v=zse9-l2Yo3Y)
- A MPU-6050 accelerometer & gyro module is used for the balancing data communication via SDA & SCL
- Note, that the receiver will not work, if your vehicleType is 4 (balancing) and no MPU-6050 is connected!
- Do not enable "balancing" and "indicators" at the same time.

New in V 2.21
- Gyro drift calibration bug fixed
- Changed the balancing controller refresh rate to 125Hz to reduce jitter on a 8MHz MCU. Reason: one program loop takes 4 - 7ms, so we can't use a time base of 4ms (=250Hz).

New in V 2.3
- MRSC (Micro RC Stability Control) added. This functionality is similar to the Traxxas TSM or ESC. See: https://www.youtube.com/watch?v=IPve7QpdLBc&t=5s
- The steering servo angle is compensated in accordance with the speed, the requested steering angle and the measured yaw rate, which is coming from the MPU-6050
- Basically the mrsc() function was added. The required yaw rate is already calculated in the balancing.h, which is already existing for self balancing robots
- If you want to use this functionality, you have to set the vehicleType to 5 in the vehicleConfig.h. Please note, that the receiver will not work, if no MPU-6050 is connected and the vehicleType is set to 4 or 5!

New in V 2.31
 - MRSC gain factor now depending on the potentiometer 1 value on the transmitter
 - Allows to fine adjust the MRSC during driving, depending on the requirements of the street surface
 - Note, that you need to connect a potentiometer to the analog input A6 of your transmitter!
 
 New in V 2.32
 - Flickering headlights bug in vehicleType 5 fixed
 - The accelerometer vectors are not processed anymore in vehicleType 5. This is only required for the self balancing robot (vehicleType 4)
 
 New in V 2.4
 - The I2C bus is scanned for an MPU-6050 sensor during the setup() sequence. The vehicleType is automatically changed to 0 (car without MRSC), if no sensor is detected.
 - This means, that a car with vehicleType 5 (MRSC Stability Control) can now also be used without an MPU-6050 plugged in.
 - Note, that the self balancing mode (vehicleType 4) always requires an MPU-6050. Otherwise the receiver will not start.
 
 New in V 2.41
 - The "indicators" variable is now locked and can't be enabled, if the SDA and SCL pins are in use for the MPU-6050 sensor. This is the case in vehicleMode 4 and 5.
 
 New in V 2.5
 - The MRSC stability control is now also working for cars without a steering servo
 - Code was added to control a DC steering motor (as found in cheap RC cars) in accordance with the MPU-6050 yaw rate
 - This is for example very useful for Coke Can Cars. See: https://www.youtube.com/watch?v=jr5yYBal3vk
 
 New in V 2.6
 - The MRSC stability control can now be used with transmitters without gain adjustment potentiometer
 - The following optional parameters in the vehicleConfig.h allow to use a fixed gain: MRSC_FIXED, mrscGain
 - Allows to add an MPU-6050 gyro to every vehicle. So we always have a closed loop steering control, even without a proper servo with potentiometer feedback
 - On the "Non-High-Power" receiver model, 0.3V battery voltage are not added anymore during driving. This is not useful, if there is not much current draw.
 
 New in V 2.7
 - The MRSC stability control gain adjustment knob can now be linked to servo CH4, if "potentiometer1" is set to "true" in vehicleConfig.h
 - This is useful, if an external MRSC stability control unit for 3 pin servos is used. See: https://github.com/TheDIYGuy999/MRSC_Adapter_3Pin_Servo
 - Servo CH4 is wired to pin 5 in this case. This allows to adjust the MRSC gain on the transmitter, just as with the internal MRSC stability control
 
 New in V 2.8
 - Added support for differential thrust controlled airplanes
 - Tested with a Piper J3 CUB from Banggood
 
 New in V 2.9
 - Added TWO_SPEED_GEARBOX option for mechanical two speed gearboxes, shifted by a servo, connected to CH2, controlled by "Mode 1" button
 - Example see vehicle CONFIG_WPL_B-36
 - See video: https://www.youtube.com/watch?v=EaOJE_GU5pk&t=1s

## Usage

See pictures
![](https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/1.jpg)
![](https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/2.jpg)
![](https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/3.jpg)

Receiver

![](https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/Micro_RC_Receiver.png)

MPU-6050 shield

![](https://github.com/TheDIYGuy999/Micro_RC_Receiver/blob/master/MPU-6050_Shield.png)

(c) 2016 - 2018 TheDIYGuy999
