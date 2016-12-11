#This is an ATMEL Mega328P / NRF24L01+ based 2.4GHz radio receiver
## Features:
- 4 RC servo connectors
- integrated TB6612FNG dual dc motor driver
- serial header
- I2C header
- ICSP header for programming with Arduino IDE (use "TheDIYGuy999 328P" board definition and set brownout voltage to 1.8V)
- NRF24L01+ SMD module integrated
- very small size
- Eagle files are also provided, if you want to build your own receiver!

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

## Usage

See pictures in this repo

(c) 2016 TheDIYGuy999
