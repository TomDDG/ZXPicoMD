# ZXPicoMD
Raspberry Pico ZX Spectrum Microdrive Hardware Emulator (https://en.wikipedia.org/wiki/ZX_Microdrive)

Hardware emulation of 8 microdrives for the ZX Spectrum only (no plans to adapt this for the QL). Main features are:
- Supports all 8 Microdrives from one device
- OLED screen with buttons to navigate the menu - includes load, save & format cartridges as well as other options (see menu section below). No additional toolkits, software or connections needed as all managed from the menu
- FAT32 & exFAT supported SD cards up to a theoretical 256TB (I've only tested a lowly 64GB which can easily fit every Spectrum game and program ever made multiple times)
- Reset button just in case (also makes it easier to flash the Pico)
- Buzzer in case you like a buzzing sound to "simulate" the drive motor
- Includes a version of my Z80onMDR tool (https://github.com/TomDDG/Z80onMDR_lite) to make it easy to load Z80 & SNA snapshots on the fly. Very useful as not many actual Microdrives cartridges released commercially.
- Fully powered from the Spectrum & boots instantly
- Unfortunately you do still need an IF1 (https://en.wikipedia.org/wiki/ZX_Interface_1)

Load up the SD card with all your favourite games and programs and enjoy hours of nostalgia with the blistering 15kB/sec transfer rate. Yes you can now load a game in a few seconds!

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/IMG_0151.jpg "In Case")

## Installation
- Download the correct UF2 from the UF2 folder (_w for Pico W)
- Flash the Pico with the UF2 (see 3.2 https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- Build the circuit (see below) - best to use the PCB once I've finished it
- Connect to the IF1 
- Turn on the Spectrum and hopefully all good

## Development

Built from the ground up using the the Pico C SDK (https://raspberrypi.github.io/pico-sdk-doxygen/). Uses Pico multicore to separate the time critical emulation components running on the 2nd core to interrupt driven IO on the 1st.

Big thanks to Alex from OqtaDrive fame (https://codeberg.org/xelalexv/oqtadrive) for help with understanding the Microdrive hardware & troubleshooting. 

Some additional references:
- Spectrum Microdrive Book - https://spectrumcomputing.co.uk/entry/2000365/Book/Spectrum_Microdrive_Book
- Microdrive & Interface 1 Manual - https://spectrumcomputing.co.uk/entry/2000442/Book/Microdrive_and_Interface_1_Manual
- Make the Most of your ZX Microdrive - https://spectrumcomputing.co.uk/entry/2000231/Book/Make_the_Most_of_Your_ZX_Microdrive
- Sinclair IF1, IF2 & Microdrive Service Manual - https://spectrumforeveryone.com/wp-content/uploads/2017/08/ZX-Interface-1-2-Microdrive-Service-Manual.pdf

Code makes extensive use of the excellent Pico SD Card Library by carlk3 (https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico). This gives full access to the also excellent small embedded systems FatFS (http://elm-chan.org/fsw/ff/00index_e.html).

## Connected to a ZX Spectrum

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/connected.png "Connected")

## The GUI

### Main Menu
- Power Off - not really required just don't switch off in the middle of a write as it could corrupt the files on the SD card
- Factory Reset - this creates 8 blank cartridges on the SD card root
- Eject SD Card - as it says, this safely unmounts the SD card so you can take it away and load up with more games
- Restart Emulation - Just resets the 2nd core, back button does the same at this level of the menu
- Cart 1 to 8 - Enter sub-menu for that specific Microdrive

### Microdrive Sub-Menu
- File Selector - enter the file explorer to navigate the folders on the SD card to select a file to load
- Cartridge Info - shows a basic CAT of the cartridge inserted
- Write Portect - toggles write protect on/off for the cartridge inserted
- Save Cartridge - creates a copy of the cartridge on the SD card so all your work doesn't get lost if you load a new cartridge into the drive
- Insert Formatted - as it says load a blank formatted (to 127kB) cartridge into the drive

The file explorer will only show compatible files, those with extension MDR, Z80 & SNA and directories.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/menulayout.png "Menu Layout")

## The Circuit

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/PicoDriveZX_Circuit_v3.png "Circuit Prototype v3")

## BoM

- 1 Pico or Pico W with headers soldered
  - +2 20pin header sockets
- 1 Traco Power TSR 1-2450 (9v to 5v, 1 Amp) - https://www.tracopower.com/int/model/tsr-1-2450 (you can use alternatives these are just the best and very efficient, if you do use alternatives remember you may need additional circuitry)
- 1 SparkFun Logic Level Converter - Bi-Directional (BOB-12009) - https://www.sparkfun.com/products/12009
  - +2 6pin header sockets if using the PCB
- 1 SSD1306 OLED 0.96" (you can get larger ones just make sure they are SSD1306). Be very careful of the GND & VCC placement as they are sometimes reversed
  - +1 4pin header socket if using the PCB. If also using the 3D printed case get header sockets with extra long legs so the OLED can be mounted higher
- 1 Adafruit Micro SD SPI or SDIO Card Breakout Board - https://www.adafruit.com/product/4682
  - +1 9pin header socket if using the PCB
- 5 6×6mm Right Angle Micro Push Buttons for mounting on the PCB (you can use any push button these are just the ones that I use)
- 2 Bi-Colour LEDs, common cathode (centre negative). Pick whatever colours you want and change the resistors to match. The LEDs are:
  1. (D)rive and (W)rite for when the Microdrive is being accessed
  2. (S)D Card Access and (I)nput Ready to show when the SD card is being read or written (don't turn off) or the unit is ready to receive an input either from the IF1 or the enter button to enter the menu.
- 5 1n4148 diodes [WR.PR(D), ERASE(D), COMMS(D), RW(D), CLK(D)]
- 1 1n4001 diode for the 5v in, this is to allow USB to be connected and the Spectrum at the same time (any of the 400x series should work, I use 4004 as it was the one I had in stock) [5V(D)]
- 1 10kOhm Resistor for write protect circuit [WPC(R)]
- 1 6.8kOhm Resistor for write protect circuit [WPB(R)]
- 1 2N2222 Transistor for write protect circuit
- 1 Piezo Buzzer (if you want annoying sounds)
- 1 3D Printed Case - STLs for the 3D Printed Case can be download from Printables (https://www.printables.com/model/297015-case-for-zx-picomd-sinclair-zx-spectrum-microdrive) or Thingiverse (https://www.thingiverse.com/thing:5569842)

## PCB

I have designed a PCB to house everything. v1.1 shown differs from the original prototype as it now has a simple transistor circuit to control Write Protect and also passes the COMMs down the chain for additional Microdrives.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/ZXPicoMDv1.1.png "PCB Prototype v1.1")

## FAQ

- SD Card access is very slow - I've tested this with ~1000 files in a single directory and it should take no longer than a second to show a directory listing. If you are finding access is slow, please try the following:
  - Don't put thousands of files in a single directory, try to limit it to <1000 per directory otherwise it will take a long time to find what you need as the OLED only shows 4 files per screen. Put the files in a-z directories as an example.
  - Copy everything off your SD Card onto a PC, reformat the SD Card and copy it all back on. This ensures the files are on the SD Card sequentially which speeds up access significantly. This also helps as I use a basic sort algorithm, if the files are already sorted this has less to do.
  - Try to use a modern Micro SD Card of at least C10 rating. As of writing this the cheapest cards seem to be the 32GB ones which are most likely UHS Speed Class which will work fine and way more space than you need. I've configured the SD Card access speed to be 12.5Mbits/s which is ~1.5MBytes/s (more like half that in reality) so anything above that in theory should be fine.
