# Chess Clock

This repository contains a simple Chess Clock application implemented in Arduino IDE language (C? / C++?)

## Features

- Poker mode including hand counter, timer and individual player timer.
- Up counting mode. 1-9 players (for other timing purposes). 
- Down counting mode. 1-9 players (for other timing purposes). Includes time back, and individual time custimization
- Buzzer 
- Loop mode 1-9 players. Loops timer. Good for turn timer. Resets to default on button press
-Battery opperated

## Blender file for 3d printing

- This file is what was used for the 3d printing

## KiCAN file for PCB

- Main board and Display board for each display
- Used jlbPCB for printing. Was pretty useful. 

## Partlist 

## Partlist 

| Component              | Part Number/Description                                                                                                 |
|------------------------|--------------------------------------------------------------------------------------------------------------------------|
| ATMEGA                 | ATMEGA32                                                                                                                 |
| Control SW             | SW-PB1-1DZ-A-P1-A                                                                                                       |
| Multiplexer            | CD4028BM96                                                                                                               |
| Buzzer                 | WST-1210T                                                                                                                |
| Battery Holder         | 36-234-ND                                                                                                                |
| 5v Switching Reg       | AP63205QWU-7                                                                                                             |
| Inductor               | B78108E1472K000                                                                                                          |
| Caps 100nF 0603       |                                                                                                                          |
| Caps 22uF 0603        |                                                                                                                          |
| Main Button (Amazon)   | DaierTek 6pcs 12V Momentary Push Button Switch 12mm Waterproof Small Round Mini Micro Black Metal Power Pushbutton SPST for Car RV Marine Boat Automotive   |
| Power Button (Amazon)  | QTEATAK 5Pcs On/Off Boat Rocker Switch 5Pcs 2 Pin 2 Position Snap 12V 110V 250V                                        |
| Player Button          | 12Pcs Arcade Joystick Game Console Buttons 30mm Replacement Button for Most Hori and for MadCatz, Snap in Installation |


## ETC

I may be missing some parts from the list. I got most on digikey and the rest on amazon.
This design should be simplified But was sucessful for my purposes. 