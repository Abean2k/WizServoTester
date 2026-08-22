---
Title: WizServotester
Github: (https://github.com/Abean2k/WizServoTester)
Description: 4-channel servo tester for FTC
Created_at: "2026-08-19"
---

# August 21 & August 20: <Schematic Finished>

This project is one that me, my team, and many other FTC teams I know could use and would love to have. 

***Here is the issue:*** When testing Gobiulda servos, their servo testing kit is either very expensive ($200+) and/or not compatible with the FTC batteries and all four servos. 

To solve this before Robot In 3 Days and make something that I think other robotics people might want I started working on this 4-channel servo tester. 
I wanted this to be very frictionless while we (and others) are using it. So, I decided to use the Esp32 family for my MCU, and eventually chose the C6 due to the price, GPIO pins, and ease of use. 
For control I will be having 4 encoders (one for each servo) with buttons for switching servo modes. 
For power, the tester can power one servo when connected to USB power and all 4 at the same time when connected to the battery.
To increase the usefulness for more FTC teams I am also including multiple BAT connection ports (Xt60, Powerpole, and Pads). 

Today I spent my time reading the ESP32-C6-Wroom-1 Data Sheet to see how I should set up the boot pins, where I can route each servo and encoder, etc. 
After that I started making my schematic and was able to finish it just after 12AM lol...


**Here is my Schematic:**
<img width="1154" height="790" alt="Screenshot 2026-08-21 003544" src="https://github.com/user-attachments/assets/3b5783fe-3120-44a1-9bdd-b0cfc5e6cc46" />


**Total time spent: 6h**

# August 22: <PCB Routing Finished>

Today I worked on assigning all the footprints to their respective schematics. 

While doing so I also had to make a footprint for the AliExpress buck converter thing, here was the picture of that:
<img width="557" height="368" alt="image" src="https://github.com/user-attachments/assets/62af4da2-c1cc-4a3e-96ac-1fa2028c8669" />

After arraigning everything inside of a 100mm x 60mm pcb I routed everything. Making sure to use thick traces for the power traces. I also added the team logo at the front in the little free space.

**Here is my PCB:**
<img width="955" height="604" alt="image" src="https://github.com/user-attachments/assets/ceb6977a-0cc7-4ab7-8cfa-e066113be61a" />


