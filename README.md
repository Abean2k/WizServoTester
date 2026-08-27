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
<img width="948" height="661" alt="Screenshot 2026-08-25 163040" src="https://github.com/user-attachments/assets/c001fe1e-b01f-40da-9778-c7efedd93965" />

### PCB Layout
<img width="875" height="556" alt="image" src="https://github.com/user-attachments/assets/63c91cf0-f6d1-48eb-8390-3ab90329cf47" />

## Case:
This is optional, if you are recreating this project, you can choose to use my case or just not use a case. Made with two parts, a base, and a top cover type thing. Uses m3 screws to fasten the PCB into the case and the bottom of the base to the top. Designed on Onshape. 

<img width="812" height="697" alt="Screenshot 2026-08-23 231656" src="https://github.com/user-attachments/assets/1ca00d3a-fcf2-46f2-8d6a-39705e4e8ef3" />

## Firmware:
The ESP32 will run C++ with Arduino. The code is still under development. The idea is that the code can detect the voltage of the USDB to see where it's receiving power from, then it can control the servos based on the position of the rotary encoder. Lots of the code is similar to my rocketry airbrake project so V1 code development was quick. The online app that is hosted on the ESP will be added based on how the PCB performs and feedback I receive from my team about the tester. 

## Bill of Materials (BOM)

| Part | Quantity | Retailer | Total Price |
|---|---|---|---|
| ESP32-C6-WROOM-1 | 1x5 | LCSC | $21.16 |
| SMD Caps (10u, 100n, 1u, 22u) | 9 x 5 | Already have | $0.00 |
| 470u 16v Capacitor | 1x5 | LCSC | $0.61 |
| ESD9B3.3 Diode | 2x5 | LCSC | $0.53 |
| SS34 Schottky Diode | 1x5 | LCSC | $0.71 |
| SS54 Schottky Diode | 1x5 | LCSC | $0.45 |
| USB-C Receptacle | 1x5 | LCSC | $0.62 |
| XT30 Battery Connector | 1x5 | AliExpress | $2.97 |
| (01x03) Male 90 Degree Header | 4x5 | AliExpress | $5.58 |
| Resistors (10k, 5.1k, 4.7k) | 30 | Already have | $0.00 |
| EC11 Rotary Encoder | 4x5 | AliExpress | $3.24 |
| Push Button Switch | 2x5 | LCSC | $0.53 |
| SPDT Power Select Switch ($1.84) | 1x5 | LCSC | $1.84 |
| USBLC6-2SC6 (USB Protection) | 1x5 | LCSC | $0.48 |
| AMS1117-3.3 (LDO) | 1x5 | LCSC | $0.49 |
| Mini560 Buck Converter | 1x5 | AliExpress | $4.40 |
| PCB (5x boards) | 1x5 | LCSC | $11.32 |

**Taxes & Shipping: AliExpress ($0.96) + LCSC ($10.80)** 

**Total Cost For 5 PCBS: $66.69**
