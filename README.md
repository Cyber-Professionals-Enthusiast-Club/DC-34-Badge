# About
DC-34 Badge contains the KiCad 6 design files for the Cyber Professionals 
Enthusiast Club (CPEC) DEFCON 34 Badge. 

## Board Features
- 2.2" 320x240 display
- Wifi and Bluetooth LE equiped
- 3 Shitty Add On (SAO) 1.69bis ports equiped with I2C
- Gameboy equivalent buttons
- Rumble pack

## Project Structure
The structure of this repo is organized as such:

| Directory                | About                                                                    |
|:-------------------------|:-------------------------------------------------------------------------|
| art                      | SVG files of onboard art                                                 |
| DC-34-Badge              | KiCad 6 Design files                                                     |
| DC-34-Reference-Firmware | Reference firmware, which demonstrates driving hardware devices on board |
| digikey-kicad-libraries  | Symbol libraries and footprints downloaded from digikey                  |
| kicad-libraries          | git subtree containing expressif design files                            |

## Running
### Board Design
To view schematics, please run KiCad 6.0.11 or later. Change directory into 
DC-34-Badge and run
```
$ kicad DC-34-Badge.kicad_pro &
```

Please note: please run kicad from the same directory as the .kicad_pro file 
is stored because the fp-lib-table and sym-lib-table use relative paths 
so that this project is portable between computers.

### Reference Firmware 
To develop the reference firmware, which demonstrates interfacing with hardware
devices on the board, change directory into DC-34-Reference-Firmware and run:
```
$ arduino-1.8.5 DC-34-Reference-Firmware &
```
Follow the instructions in the comments to set up your development enviroment
for the board. 

Prototype "Rev 1" board should be set up with the configuration in the image below: 

![Rev 1 CPEC DC 34 Board showing backwards placed screen and USB dataline inverter](https://i.imgur.com/GchWWEE.jpeg)

# Associated Repos
Board firmware is developed in this repo: 
[https://github.com/Cyber-Professionals-Enthusiast-Club/DC34-Mech-Badge-Game](https://github.com/Cyber-Professionals-Enthusiast-Club/DC34-Mech-Badge-Game)

# Programming Updates for Rev 2
This board is built on a tight schedule, meaning there is zero margin for 
integrating embeded software with Rev 2. The firmware will need to be integrated 
and tested in hours, preceeding the con before distribution. The following 
tables serves as a guide for new or changed board features for firmware 
development to be conducted before Rev 2 is delivered. 

## What is the same with Rev 1
Most board features on Rev 2 are identical to the Rev 1 prototype board. This
includes:
- Buttons are identical, no buttons have been added or removed. All buttons 
remain on the same GPIO pins
- Display is the same. The display has not changed its pin outs
- SAO ports I2C pins and addresses are the same 

## Board Identifiers
Rev 2, and all mech boards going forward, feature 
*board identification resistors* that board firmware can key in on to determine
what board it is. 

| Identification Bit | GPIO   |
|:-------------------|:-------|
| 0                  | GPIO13 |
| 1                  | GPIO15 |
| 2                  | GPIO18 |
| 3                  | GPIO39 |

These pins are pulled to ground, through a 5.1k ohm resistor, to indicate 0, and 
pulled to 3.3v, through a 5.1k ohm resistor, to indicate 1. 

The board identification bits can be used by firmware, at initial boot up, to
determine what board it is running on. 

This can be configured with the following code:
```
// Declare a global variable to hold the Board ID for use by functions
static volatile uint8_t boardId = 0;

// Set up Identification Bit Pins
pinMode(13, INPUT);
pinMode(15, INPUT);
pinMode(18, INPUT);
pinMode(39, INPUT);

// Get the Board Identification bit values
boardId = 0;
boardId |= (digitalRead(13) & 0x1) << 0;
boardId |= (digitalRead(15) & 0x1) << 1;
boardId |= (digitalRead(18) & 0x1) << 2;
boardId |= (digitalRead(39) & 0x1) << 3;
```

Expect `boardId` to have a value between 0 and 15. The ID for Rev 2 of the 
board, is `0`. Future revs and different versions of the board, will each get 
unique bit values. 

## Rumble Pack
Rev 2 introduces an *enable bit* for the H-Bridge, which drives the motor for
the rumble pack. This was added as a power saving measure. When the board is 
not active, the MOTOR_EN (motor enable) pin should be set to low to prevent the 
motor circuitry from drawing power. This means that there are two GPIO pins 
that drive the rumble pack. 

| Identification Bit | GPIO   |
|:-------------------|:-------|
| MOTOR_EN           | GPIO16 |
| RUMBLE             | GPIO17 |

`MOTOR_EN` should be set up as an `OUTPUT` pin with the `pinMode()` function. 
RUMBLE can either be set up as a digital output pin, or as a 
*Pulse Width Modulation* pin to control the motor with different levels. On
the ESP-32, this will let the firmware precisely control the motor to turn it 
at a slower rate than fully on. This can be done with the `ledcAttach()` 
and `ledcWrite()` functions.

`MOTOR_EN` should be set to `LOW` when the user is not actively using the board.

## Output LEDs
The board has six programable LEDs that can be turned on or off. Their qualities
are enumerated in the table below.

| LED Position  | Color  | GPIO   |
|:--------------|:-------|:-------|
| Right Knee    | RED    | GPIO12 |
| Left Knee     | RED    | GPIO45 |
| Right Ear     | RED    | GPIO40 |
| Left Ear      | RED    | GPIO41 |
| Right Sholder | RED    | GPIO6  |
| Cockpit Light | YELLOW | GPIO42 |

These should be set up as `OUTPUT` pins with the `pinMode()` function. They 
can be turned on or off with the `digitalWrite()` function. Setting the pin
to `HIGH` turns the LED on, setting the pin to `LOW` turns the LED off. 

## Photo Resistor
The Rev 2 board is equiped with a photo resistor to sense light levels. This
can be used to set the screen intensity higher when there is more light. This
resistor is put on `GPIO3`, which can be configured by setting the pin to 
analog read. 

The photo resistor has a maximum resistance of 33kOhms, and is in series with
a 10kohm resistor. At low light levels, it is expected to read a value close 
to 100%, and at high light levels, the pin is expected to read 23%. 

This pin can be used with the `analogRead()` function. 

## Buzzer
The Rev 2 board is equipped with a buzzer to provide sound output. Although 
the Rev 1 board also had a buzzer, its output was too quiet to hear. This 
buzzer is a self contained circuit, and is simpler. The buzzer produces a 
single 3,000 Hz tone, and can be used to indicate something is happening on
the board and that users should direct their attention to it. 

The new buzzer is driven by `GPIO33`. This can be configured as an `OUTPUT` pin 
with the `pinMode()` function. It can be turned on or off with the 
`digitalWrite()` function. Setting the pin to `HIGH` turns the buzzer on, 
setting the pin to `LOW` turns the buzzer off. 

# Bill of Materials (BOM) 
The file, `MechFighters_Badge_BOM_PCBWay_Rev2p0.xlsx` in this git repo contains
the SMD parts list that the board assembler should put on the board. 

The board has throughhole componets that are assembled by the user. The list of 
throughhole componets are listed below, and should be procured for final 
assembly. 

|Part Number | Symbol | Notes | Quantity per Board | Price | Link |
|:-----------|:-------|:------|:-------------------|:------|:-----|
| CFAF240320A0-024SR | DISPLAY | 240x320 Display | 1 | $15.23 | [CrystalFonz](https://www.crystalfontz.com/product/cfaf240320a0024sr-240x320-resistive-touchscreen-tft-display) |
| CPT-1207-5LTH-T    | BZ1 | Buzzer | 1 | $0.53 | [Digikey](https://www.digikey.com/en/products/detail/same-sky-formerly-cui-devices/CPT-1207-5LTH-T/29292285) |
| 2011 | BATTERY | Battery | 1 | $12.50 | [Digikey](https://www.digikey.com/en/products/detail/adafruit-industries-llc/2011/6612469)
| VZ4TL2B0370055L | J6 | Haptic Feedback | 1 | $2.63 | [Digikey](https://www.digikey.com/en/products/detail/vybronics-inc/VZ4TL2B0370055L/15220806?s=N4IgTCBcDaIIwDYDsAGAtANQFoBYAqAMmAEIoDMqKArFQWgHIAiIAugL5A) |
| JS202011CQN | SW1 | On/Off Switch | 1 | $0.67 | [Digikey](https://www.digikey.com/en/products/detail/c-k/JS202011CQN/1640097) |
| SFH11-NBPC-D03-ST-BK | J3 - J5 | SAO Connector | 3 | $0.54 | [Digikey](https://www.digikey.com/en/products/detail/sullins-connector-solutions/SFH11-NBPC-D03-ST-BK/4558818) | 
| 1825910-6 | SW2 - SW11 | Button | 10 | $0.09 | [Digikey](https://www.digikey.com/en/products/detail/te-connectivity-alcoswitch-switches/1825910-6/1632536) | 
| S2B-PH-K-S | J7 | Battery Connector | 1 | $0.97 | [Digikey](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S2B-PH-K-S/926626) | 
| PDV-P8103 | R31 | Photo Resistor | 1 | $0.61 | [Digikey](https://www.digikey.com/en/products/detail/advanced-photonix/PDV-P8103/480610) |

# Media
## Board Schematics 
![Rev 2 Schematic with ESP-32-S3](https://i.imgur.com/lkTOgR3.png)

## 3D Renders

### Board Front View
![Rev 2 Board Front View](https://i.imgur.com/9MR0nGA.png)

### Board Back View
![Rev 2 Board Back View](https://i.imgur.com/noXTeLr.png)

### Panelized Board Front View
![Rev 2 Panelized Board Front View](https://i.imgur.com/FbZeWPK.png)

### Panelized Board Back View
![Rev 2 Panelized Board Back View](https://i.imgur.com/y9HTTpk.png)
