# ZXPicoMD
Raspberry Pico ZX Spectrum Microdrive Hardware Emulator

Hardware emulation of 8 microdrives for the ZX Spectrum only (no plans to adapt this for the QL). Main features are:
- 8 Microdrives
- OLED and buttons to navigate the menu - load, save & format cartridges as well as other options (see menu section below)
- FAT32 & exFAT supported SD cards up to a theoretical 256TB (I've only tested a lowly 64GB)
- Reset button just in case (also makes it easier to flash the Pico)
- Buzzer in case you like a buzzing sound to "simulate" the drive motor
- Includes a version of my Z80onMDR tool (https://github.com/TomDDG/Z80onMDR_lite) to make it easy to load Z80 & SNA snapshots

Built from the ground up using the the Pico C SDK (https://raspberrypi.github.io/pico-sdk-doxygen/). Big thanks to Alex from OqtaDrive fame (https://codeberg.org/xelalexv/oqtadrive) for help with understanding the Microdrive hardware & toubleshooting. 

Some additional references:
- Spectrum Microdrive Book - https://spectrumcomputing.co.uk/entry/2000365/Book/Spectrum_Microdrive_Book
- Microdrive & Interface 1 Manual - https://spectrumcomputing.co.uk/entry/2000442/Book/Microdrive_and_Interface_1_Manual

Code makes extensive use of the excellent Pico SD Card Library by carlk3 (https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico). This gives full access to the also excellent small embedded systems FatFS (http://elm-chan.org/fsw/ff/00index_e.html).

## Connected to my ZX Spectrum

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/connected.png "Connected")

## The Menu

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/menulayout.png "Menu Layout")

## The Circuit

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/PicoDriveZX_Circuit_v2.png "Circuit Prototype")

## BoM

- A Pico or Pico W
- Traco Power TSR 1-2450 (9v to 5v, 1 Amp) - https://www.tracopower.com/int/model/tsr-1-2450 (you can use alternatives these are just the best and very efficient, if you do use alternatives remember you may need additional circuitary)
- SparkFun Logic Level Converter - Bi-Directional (BOB-12009) - https://www.sparkfun.com/products/12009
- SSD1306 OLED 0.96" (you can get larger ones just make sure they are SSD1306). Be very careful of the GND & VCC placement as they are sometimes reversed
- Adafruit Micro SD SPI or SDIO Card Breakout Board - https://www.adafruit.com/product/4682
- 5 6×6mm Right Angle Micro Push Buttons for mounting on the PCB (you can use any push button these are just the ones that I use)
- 2 Bi-Colour LEDs, common cathode (centre negative). Pick whatever colours you want and change the resistors to match.
- 6 1n4148 diodes
- 1 1n4001 diode for the 5v in, this is to allow USB to be connected and the Spectrum at the same time (any of the 400x series should work, I use 4004 as it was the one I had in stock)
- Piezo Buzzer (if you want annoying sounds)
- 2 470Ohm resistors for the data lines

## PCB

I have designed a prototype PCB to house everything. The prototype is missing the circuit to control Write Protect so it is always off (cartridges are unprotected so you can write to them), and there is no provision for passing COMMs down the chain for additional Microdrives.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/pico_md.png "PCB Prototype")
