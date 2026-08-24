# WizServoTester

<img width="730" height="615" alt="image" src="https://github.com/user-attachments/assets/6621f835-f1e0-4c27-976e-4a27b6e32805" />

A servo controller PCB that can control up to 4 servos using onboard rotary encoders. Powered by an ESP32-C6 with two power input modes, USB and FTC 12V Battery. 

## Design and Inspo:
I wanted to solve the issue that my team and many other FTC teams I know have. When testing Gobiulda Servos (such as during Robot In 3 Days) we need a fast way to move multiple servos without using code. 
Currently, the only solution available is from Gobiulda, and the full kit for 4 servos cost about $200. So, I made WizServotester!

When designing this I made sure to keep it useful to FTC teams by allowing teams to use their existing battery to power the device. Teams already have many 12v 10 cell batteries, so I have included the xt30 connector that all these batteries have and a buck converter to step the voltage down to 5v for use in the circuit. The entire thing can be reprogrammed using the USBc and inbuilt USB programmer on the ESP32-C6. 

### Power:
This thing can be powered in two ways - USB & Battery. When under battery power all 4 servos can be used at full load and in both continuous and normal modes. However, when in USB power, only 1 servo can be used at one time. This is due to the limited power capacity of a USB C cable. Power can be selected using a switch on the pcb.  

## PCB:
I made a 2 layered PCB on Kicad for the servo tester. The pcb is around 100mm by 60mm. I have included some instructions on how to use the power switch and battery connector as well as some PCB Art (my first time trying that out <3). 

### Schematic
<img width="1079" height="747" alt="Screenshot 2026-08-22 122518" src="https://github.com/user-attachments/assets/3d1df586-f78c-408b-9368-56f4f398a0ce" />

### PCB Layout
<img width="918" height="563" alt="Screenshot 2026-08-23 231227" src="https://github.com/user-attachments/assets/4b2c7a3c-abba-4239-9137-de7861eede82" />

## Case:
This is optional, if you are recreating this project, you can choose to use my case or just not use a case. Made with two parts, a base, and a top cover type thing. Uses m3 screws to fasten the PCB into the case and the bottom of the base to the top. Designed on Onshape. 

<img width="812" height="697" alt="Screenshot 2026-08-23 231656" src="https://github.com/user-attachments/assets/1ca00d3a-fcf2-46f2-8d6a-39705e4e8ef3" />

## Firmware:
The ESP32 will run C++ with Arduino. The code is still under development. The idea is that the code can detect the voltage of the USDB to see where it's receiving power from, then it can control the servos based on the position of the rotary encoder. Lots of the code is similar to my rocketry airbrake project so V1 code development was quick. The online app that is hosted on the ESP will be added based on how the PCB performs and feedback I receive from my team about the tester. 

## BOM:
