# Helmet (WIP)
Esp32 project to recreate the functionality of an Iron Man helmet.

## Current Progress
![20241029_122954](https://github.com/user-attachments/assets/0a24c8c9-64a1-4181-9043-9c010b2f0c8d)

The Helmet 3D prints are ready and the Servos and LED-Lights are build in. I’m currently waiting for a friend who is designing a custom circuit board using my schematic.

![20241029_123108](https://github.com/user-attachments/assets/9185d658-3215-4aa7-9696-3782136e8f1a)

## Parts
* ESP32-Wroom-D32 DevKit
* 2 MG90s Servos
* 3,7 Lipo Battery
* 2 Step-Up-Converter (3,3V and 5V)
* USB-C Charge Module
* DIY LED Lights (light blue)
* Photoresistor
* Button(s)

## Status LED
| Errorcode | Status Led       | Meaning |
|:----------|:----------------:|:--------------------------------------------------|
| 1         | two short blinks | Leds cannot be switched on when the visor is open |
| 2 | wip | The visor is already closed |
| 3 | wip | The eyes are already on |
| 4 | wip | The eyes are already off |

## Future
I am currently building a voice recognition for the helmet functionalities, e.g. opening the visor, or controlling the eye LEDs. 
Look in the voice-recognition [Branch](https://github.com/BonusPizza/Helmet/tree/voice-recognition) for further infos.
