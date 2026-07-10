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
[https://github.com/wildcat86/DC34-Mech-Badge](https://github.com/wildcat86/DC34-Mech-Badge)

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
