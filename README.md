# Communication module

Github repo for the communication module

# Pre-requisites

Please ensure that the following are fulfilled before you can proceed to run the communication module of the Robokaar Project.

# IDEs

- MSP432 -> Code Composer
- M5Stick C Plus -> Installed in the Arduino IDE with guided tutorial via https://docs.m5stack.com/en/quick_start/m5stickc_plus/arduino

# Libraries used

- MSPDRIVERLIB - https://www.ti.com/tool/MSPDRIVERLIB
- Arduino M5Stick C Plus Lib Drivers - https://docs.m5stack.com/en/quick_start/m5stickc_plus/arduino
- ESP8266 Lib - https://github.com/amartinezacosta/MSP432-ESP8266

# Hardware

ESP8266

- 1x MicroUSB to USB A cable
- 1x MSP432
- 6x Jumper wires

UART Configuration for ESP8266
Baud Rate of 115200
 * VCC = 3.3V
 * GND = GND
 * TX = P3.2
 * RX = P3.3
 * RST = P6.1
 * CH_PD = 3.3V

M5Stick C Plus

- 1x USB A to USB C cable
- 1x Grove wire
- 3x Jumper wires

UART Configuration for M5Stick C Plus

- Baud Rate of 115200
- g26 UART RX pin -> p3.3 RX pin EUSCI_A2_BASE msp432 
- g0 UART TX pin -> p3.2 TX pin EUSCI_A2_BASE msp432

# WebApp

A simple web application created in NodeJS and EJS. For the purpose of this module, the port number is exposed in the GitHub repo.

NODEJS Guide

Assuming that you are already in the communication folder:

1. cd web/server

- Check that there is no node_module folder, if there is delete it

2. npm install

3. npm run dev
   To access TCP server endpoint, you may use http://localhost:5000
   The express webserver with UI is run on port 3001 you may visit: http://localhost:3001/ to access the dashboard.


Note: If you need to view items in your current folder directory, please run the following command:

- ls
