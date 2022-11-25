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

M5Stick C Plus

- 1x USB A to USB C cable
- 1x Grove wire
- 3x Jumper wires

# WebApp

A simple web application created in NodeJS and EJS. For the purpose of this module, the .env is exposed in the GitHub repo as it does not host any sensitive data.

NODEJS Guide

Assuming that you are already in the communication folder:

1. cd webapp/client

- Check that there is no node_module folder, if there is delete it

2. npm install
3. cd ../
4. cd webapp/server

- Check that there is no node_module folder, if there is delete it

5. npm install
6. npm run dev
   The URL will be exposed in the terminal, or alternatively, you may visit: http://localhost:5000/

Note: If you need to view items in your current folder directory, please run the following command:

- ls
