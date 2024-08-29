# Helmet
Esp32 project to recreate the functionality of an Iron Man helmet.

## Current Progress
![current_progress](https://github.com/user-attachments/assets/1faaa313-dcce-40db-9d44-5c49e7fb2d33)

The Helmet 3D prints are mostly ready. I'm currently working on developing a reliable power source and preparing everything to create my own soldered circuit board for all the hardware. 

## Parts
* ESP32-Wroom-D32 DevKit
* 2 MG90s Servos
* 3,7 Lipo Battery
* 2 Step-Up-Converter (3,3V and 5V)
* USB-C Charge Module
* DIY LED Lights (light blue)
* Button(s)

## Status LED
| Errorcode | Status Led       | Meaning |
|:----------|:----------------:|:--------------------------------------------------|
| 1         | two short blinks | Leds cannot be switched on when the visor is open |
| 2         | one short blink  | Spamm protection for the visor button is on       |
| 3         | one long blink   | Spamm protection for the eye button is on         |

## Future
Build a voice recognition for the helmet and add commands for certain functionalities, e.g. opening the visor, or controlling the eye LEDs.
