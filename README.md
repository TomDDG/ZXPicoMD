# ZXPicoMD

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/fullset-up.png "with Real Hardware")

Raspberry Pico ZX Spectrum Microdrive Hardware Emulator (https://en.wikipedia.org/wiki/ZX_Microdrive)

Hardware emulation of 8 microdrives for the ZX Spectrum only (no plans to adapt this for the QL). Main features are:
- Supports all 8 Microdrives from one device
- Supports downstream real h/w Microdrives (if you have a v1.1 PCB you need a small hardware mod, see below)
- OLED screen with buttons to navigate the menu - includes load, save & format cartridges as well as other options (see menu section below). No additional toolkits, software or connections needed as all managed from the menu
- FAT32 & exFAT supported Micro SD cards up to a theoretical 256TB (I've tested 2GB, 4GB and a lowly 64GB which can easily fit every Spectrum game and program ever made multiple times)
- Reset button just in case (also makes it easier to flash the Pico)
- Buzzer in case you like a buzzing sound to "simulate" the drive motor
- Includes a version of my Z80onMDR tool (https://github.com/TomDDG/Z80onMDR_lite) to make it easy to load Z80 & SNA snapshots on the fly. Very useful as not many actual Microdrives cartridges released commercially.
- Fully powered from the Spectrum & boots instantly
- Unfortunately you do still need an IF1 (https://en.wikipedia.org/wiki/ZX_Interface_1)

Load up the SD card with all your favourite games and programs and enjoy hours of nostalgia with the blistering 15kB/sec transfer rate. Yes you can now load a game in a few seconds!

## Installation
- Download the correct UF2 from the UF2 folder (_w for Pico W)
- Flash the Pico with the UF2 (see 3.2 https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- Build the circuit (see below) - best to use the PCB (https://www.pcbway.com/project/shareproject/Raspberry_Pi_Pico_ZX_Spectrum_Microdrive_Hardware_Emulator_37750e81.html)
- Connect to the IF1 
- Turn on the Spectrum and hopefully all good

## Version History
- v1.0beta2c (latest release) - fixed real hardware not working with the ZXPicoMD. *Note v1.1 PCB needs a small hardware mod to work, see mod section below
- v1.0beta1 - added support for real hardware Microdrives downstream of the ZXPicoMD *Note doesn't work with real h/w Microdrives, tested working with vDrive emulated drives only.
- v0.94 - added a simple filename generator to the cartridge save option
- v0.93 - fixed MF128 format
- v0.92 - added sound on/off menu option to turn off the buzzer, saves config to the SD Card
- v0.91 - first public release, all basic functions in place

## Usage
The device is very simple to use. Upon booting the Spectrum you will see a Splash Screen which will disappear after a couple of seconds. The screen will then stay blank (OLED is off to prevent burn-in) and the blue LED (or whatever colour you picked) will light up on the side to indicate the device is ready for input, either from the IF1 or by the user pressing a button. You can now use all the normal commands on the Spectrum to control the drive such as `RUN`, `LOAD *"m";1;"aaa"` or `FORMAT "m";5;"blank"`. 

When the blue LED is on (input ready) you can also enter the Menu to select new cartrdiges etc... for full list of menu items see the chart below. To enter the menu press the enter key which is 3rd from left. You should now see the Cartridge 1 icon. Pressing enter again takes you to the cartridge sub-menu, showing a folder icon. Pressing again and you will enter the file explorer where you can navigtate the listings to pick a new cartridge or snapshot to convert.

The buttons are (from the left): Previous; Next; Enter; Back; Reset

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/buttons.png "Button Layout")

Previous & Next are used to go left/right or up/down in the menu or file explorer. Enter is used to select an option or file (also to enter the menu in the first place). Back goes up one level in the menu or when using the file explorer moves up one directory. Reset reboots the Pico if all else fails (also useful when flashing the Pico). If you hold down the buttons it also behaves differently in certain scenarios, for example holding down Next in the file explorer goes down one page of file entries rather than just one file entry.

The LEDs show certain activity:
1. The top LED (furthest from the buttons) is Drive and Write for when the Microdrive is being accessed. Drive will come on when the simulated motor is on (if you installed a buzzer you will also hear it). Write will come on (replacing Drive) when the IF1 is receiving a WRITE signal.
2. The other LED is for SD Card access and Input ready. SD Card is simply to show when the SD card is being accessed (don't turn off). Input ready is to show when the drive is idling and therefore ready for a command or input.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/LEDs.png "LEDs")

## Development

Built from the ground up using the the Pico C SDK (https://raspberrypi.github.io/pico-sdk-doxygen/). Uses Pico multicore to separate the time critical emulation components (core 1) to the interrupt driven IO (core 0).

Big thanks to Alex from OqtaDrive fame (https://codeberg.org/xelalexv/oqtadrive) for help with understanding the Microdrive hardware & troubleshooting. 

Some additional references:
- Spectrum Microdrive Book - https://spectrumcomputing.co.uk/entry/2000365/Book/Spectrum_Microdrive_Book
- Microdrive & Interface 1 Manual - https://spectrumcomputing.co.uk/entry/2000442/Book/Microdrive_and_Interface_1_Manual
- Make the Most of your ZX Microdrive - https://spectrumcomputing.co.uk/entry/2000231/Book/Make_the_Most_of_Your_ZX_Microdrive
- Sinclair IF1, IF2 & Microdrive Service Manual - https://spectrumforeveryone.com/wp-content/uploads/2017/08/ZX-Interface-1-2-Microdrive-Service-Manual.pdf

Code makes extensive use of the excellent Pico SD Card Library by carlk3 (https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico). This gives full access to the also excellent small embedded systems FatFS (http://elm-chan.org/fsw/ff/00index_e.html).

## Connected to a ZX Spectrum

Following image shows everything connected. The ZXPicoMD is fully compatible with the Multiface 1 and 128 (shown in the photo).

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/FullSetup.png "Connected")

## The GUI

### Main Menu
- Power Off - not really required just don't switch off in the middle of a write as it could corrupt the files on the SD card
- Factory Reset - this creates 8 blank cartridges on the SD card root
- Eject SD Card - as it says, this safely unmounts the SD card so you can take it away and load up with more games
- Restart Emulation - Just resets the 2nd core, back button does the same at this level of the menu
- Cart 1 to 8 - Enter sub-menu for that specific Microdrive

### Microdrive Sub-Menu
- File Selector - enter the file explorer to navigate the folders on the SD card to select a file to load
- Switch to Real H/W - switches the drive number to a real H/W Microdrive, so the ZXPicoMD will emualte the shift register to activate the drive when needed
- Cartridge Info - shows a basic CAT of the cartridge inserted
- Write Portect - toggles write protect on/off for the cartridge inserted
- Save Cartridge - creates a copy of the cartridge on the SD card so all your work doesn't get lost if you load a new cartridge into the drive
- Insert Formatted - as it says load a blank formatted (to 127kB) cartridge into the drive

The file explorer will only show compatible files, those with extension MDR, Z80 & SNA and directories.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/menulayout_v3.png "Menu Layout v3")

### Working with Real H/W Microdrives

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/ZXPicoMD128k.png "128k with Real Hardware")

As of release v1 beta you can now put real h/w Microdrives downstream of the ZXPicoMD. Please note these only work downstream (ZXPicoMD closest to the Spectrum) as the ZXPicoMD needs to control the COMMs shift register in order to mix virtual/emulated drives and real h/w drives. Any drive combination can be used with the only restriction being a lower numbered drive will be activated first, example, if you pick drive 2 & 4 as real h/w drives the lower number, 2, will be the one next to the ZXPicoMD and 4 the one after.

The following diagram shows how to select drive 1 as real h/w.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/drive1hw.png "Drive 1 H/W")

and how to add drive 3 as real h/w.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/drive3hw.png "Drive 3 H/W")

With drive 1 & 3 set to real h/w, CAT 1 will access the first real drive connected to the ZXPicoMD. CAT 3 will access the second real drive, the one connected to the first real drive. All other drive selections will access a virtual/emulated drive.

To flip back to virtual/emulated drive just select the drive in the menu.

## v1.2 Circuit (the one without BOB)

THe following circuit does not include the level shifter which is no longer required due to the IF1 accepting the lower voltage output of the Pico ~3V. For the old circuit please click here https://github.com/TomDDG/ZXPicoMD/blob/main/Images/PicoDriveZX_Circuit_v3a.png

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/PicoDriveZX_Circuit_v4.png "Circuit v4")

## v1.1 PCB Hardware Mod for Real H/W Support

In order to get the v1.1 PCB working with real hardware you need to replace the BOB-12009 level shifter. Unfortunately the BOB interferes with the data output from the real Microdrive and lacks the OE pin which would put it into high impedence mode. During testing it worked fine with a vDrive in place of real h/w as the signal strength is much stronger and therefore wasn't affected by the BOB.

To fix this you can simply replace the BOB with 3 diodes as shown in the image below, note the cathode line at the top HV side. I've tested this with standard 1N4148 and BAT43 Schottky diodes. The BAT diodes have a better voltage drop, measuring I show 3.0V for the 1N4148 and 3.2V with the BAT. Recommend to use BATs although 1N4148 seem to work fine.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/diodes.png "Hardware Mod")

## BoM

- 1 Pico or Pico W with headers soldered
  - +2 20pin header sockets
- 1 Traco Power TSR 1-2450 (9v to 5v, 1 Amp) - https://www.tracopower.com/int/model/tsr-1-2450 (you can use alternatives these are just the best and very efficient, if you do use alternatives remember you may need additional circuitry)
- 3x BAT42/43 diodes, connected LV1-HV1, LV2-HV2 & LV3-HV3, line at HV side. BAT42/43 are a better option to 1N4148 due to the reduced voltage drop, however 1N4148 should also be ok. (v1.1 of the PCB had 1x SparkFun Logic Level Converter - Bi-Directional (BOB-12009) - https://www.sparkfun.com/products/12009, +2 6pin header sockets if using the BOB on the PCB) 
- 1 SSD1306 OLED 0.96" (you can get larger ones just make sure they are SSD1306). Be very careful of the GND & VCC placement as they are sometimes reversed
  - +1 4pin header socket if using the PCB. If also using the 3D printed case get header sockets with extra long legs so the OLED can be mounted higher
- 1 Adafruit Micro SD SPI or SDIO Card Breakout Board - https://www.adafruit.com/product/4682
  - +1 9pin header socket if using the PCB
- 5 6×6mm Right Angle Micro Push Buttons for mounting on the PCB (you can use any push button these are just the ones that I use)
- 2 Bi-Colour LEDs, common cathode (centre negative). Pick whatever colours you want and change the resistors to match, my rule of thumb is forward voltage/2 in kOhms so if forward voltage is 2V use a 1kOhm resistor. The LEDs are (D)rive/(W)rite and (S)D Card Access/(I)nput Ready.
- 5 1n4148 diodes [WR.PR(D), ERASE(D), COMMS(D), RW(D), CLK(D)]. These could also be replaced with BAT42/43 diodes but only WR.PR is output.
- 1 1n4001 diode for the 5v in, this is to allow USB to be connected and the Spectrum at the same time (any of the 400x series should work, I use 4004 as it was the one I had in stock) [5V(D)]
- 1 10kOhm Resistor for write protect circuit [WPC(R)]
- 1 6.8kOhm Resistor for write protect circuit [WPB(R)]
- 1 2N2222 Transistor for write protect circuit
- 1 1407 Type Passive Piezo Buzzer (if you use the PCB). 
- 1 3D Printed Case - STLs for the 3D Printed Case can be download from Printables (https://www.printables.com/model/297015-case-for-zx-picomd-sinclair-zx-spectrum-microdrive) or Thingiverse (https://www.thingiverse.com/thing:5569842)

## PCB

I have designed a PCB to house everything and you can purchase v1.1 on PCBWay (https://www.pcbway.com/project/shareproject/Raspberry_Pi_Pico_ZX_Spectrum_Microdrive_Hardware_Emulator_37750e81.html). I've also entered it into the 5th PCB Design Contest (https://www.pcbway.com/activity/5th-pcb-design-contest.html)

- v1.0 - original design for testing, lacks COMMS OUT & WRITE PROTECT circuit
- v1.1 - added simple transistor circuit to control Write Protect and also passes the COMMs down the chain for additional Microdrives. 
- v1.2 - removed bob-12009 level shifter and replaced with diodes, some refactoring. *Note v1.1 also works fine with real h/w by simply replacing the bob with diodes as shown in the hardware mod section

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/ZXPicoMDv1.2.png "PCB v1.2")

## Case

The PCB was originally designed to fit in a real Microdrive case as it is based on my Oqtadrive PCB. As such the screw holes etc... line up perfectly, however, the addition of the buttons and OLED mean, although it will fit, you won't be able to use it without cutting the case. As such I designed a custom 3D case (https://www.thingiverse.com/thing:5569842).

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/IMG_0151.jpg "In Case")

## Building with Case

This guide is based on v1.2 of the PCB, however it also applies to v1.1 with the diodes placed where the BOB was. 

First solder in the diodes, resistors, buttons, buzzer, transistor & TSR 1-2450 as per the silk screen, be careful of the orientation of the diodes. I recommend you use BAT42/43 Schottky diodes for Data 1 [D1(D)], Data 2 [D1(D)], Write Protect [WR.PR(D)] & Comms Out [CO(D)] as these can be outputs which may benefit from a lower voltage drop.

Follow this with the header sockets for the Pico (20-pin) and Micro SD Card reader (9-pin), ensure these are straight before soldering, best way is to solder one leg, check it is straight then solder the rest. For the Pico header I'd recommend pluging in the Pico first and then solder in place to ensure it all aligns. 

Now solder in the bi-colour LEDs. Mount these so the bulb is slightly above the top of header socket, this gives enough height to bend into place later.

After this you should have the following (blue diodes are BATs, red are 1N4148s):

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/DidodePlacement.png "Diode Placement")

The OLED socket needs to be raised to fit the case and why it needs longer legs, these are often referred to as stackable pin header sockets. The best way to get this at the correct height is to first mount the OLED into the case, plug the header socket in, and then put the PCB on top. Ensure the PCB is straight and then solder in place. It should now look like this:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/OLEDMount.png "OLED Mount")

You can now fit the Pico & Micro SD-Card socket and mount into the bottom part of the case by sliding in the front buttons first and securing with a smalls screw. Be careful when bending the LED legs to fit the side holes and ensure the Micro SD card slots is flush to the side of the case.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/AllBuilt.png "All Built")

## FAQ

- Why is my SD Card slow to list the directory? I've tested this with ~1000 files in a single directory and it should take no longer than a second to show a directory listing. If you are finding access is slow, please try the following:
  - Don't put thousands of files in a single directory, try to limit it to <1000 per directory otherwise it will take a long time to find what you need as the OLED only shows 4 files per screen. Put the files in a-z directories as an example.
  - Copy everything off your SD Card onto a PC, reformat the SD Card and copy it all back on. This ensures the files are on the SD Card sequentially which speeds up access significantly. This also helps as I use a basic sort algorithm, if the files are already sorted this has less to do.
  - Try to use a modern Micro SD Card of at least C10 rating. As of writing the cheapest cards seem to be the 32GB ones which are most likely UHS Speed Class which will work fine and way more space than you need. I've configured the SD Card access speed to be 12.5Mbits/s which is ~1.5MBytes/s (more like half that in reality) so anything above that in theory should be fine. Image below shows the results from a quick speed test on a couple of cards, top one is a class 10 and the other is an older 2GB rated for 30MB/s. Both of these cards worked fine:
  
![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/SDCardSpeed.png "SD Card Speed")
