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
- Vehicle configuration is now stored in "vehicleConfiguration.h", so the main code always stays the same.
- Crude frequency hopping, if bad receiving quality is detected (needs to be improved).

## Usage

See pictures in this repo

(c) 2016 TheDIYGuy999