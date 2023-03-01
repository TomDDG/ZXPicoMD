# ZXPicoMD

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/fullset-up.png "with Real Hardware")

Raspberry Pico ZX Spectrum Microdrive Hardware Emulator (https://en.wikipedia.org/wiki/ZX_Microdrive)

Hardware emulation of 8 microdrives for the ZX Spectrum only (no plans to adapt for the QL, if you want a Microdrive emulator for the QL see http://oqtadrive.org/). Main features are:
- Supports all 8 Microdrives from one device
- Supports downstream real h/w Microdrives (if you have a v1.1 PCB you need a [small hardware mod](#v11-pcb-hardware-mod-for-real-hw-support))
- OLED screen with buttons to navigate the menu - includes load, save & format cartridges as well as other options (see [menu section](#the-gui)). No additional toolkits, software or connections needed as all managed from the menu
- FAT32 & exFAT supported Micro SD cards up to a theoretical 256TB (I've tested 2GB, 4GB and a lowly 64GB which can easily fit every Spectrum game and program ever made multiple times)
- Reset button just in case (also makes it easier to flash the Pico)
- Buzzer in case you like a buzzing sound to "simulate" the drive motor
- Includes a version of my Z80onMDR tool (https://github.com/TomDDG/Z80onMDR_lite) to make it easy to load Z80 & SNA snapshots on the fly. Very useful as not many actual Microdrives cartridges released commercially.
- Can copy the contents of a TAP to a blank Virtual Microdrive Cartridge (see [TAP copy section](#tap-copy) for details)
- Fully powered from the Spectrum & boots instantly
- Unfortunately you do still need an IF1 (https://en.wikipedia.org/wiki/ZX_Interface_1)

Load up the SD card with all your favourite games and programs and enjoy hours of nostalgia with the blistering 15kB/sec transfer rate. Yes you can now load a game in a few seconds!

## How to Get One
- I designed ZX PicoMD to be very easy to build so anybody should be able to get the parts and put one together. I picked parts which are easy to get even with the current chip shortage, basing the whole build around the Raspberry Pico. It does require some soldering but this is all through hole not surface mount so a lot easier to do. I've included full details on how to [build one](#building-with-case) including [bill of materials](#bom) with links to the parts.
- I do occassional build some complete units to sell on eBay https://www.ebay.co.uk/usr/tom_dd

## Installation
- Download the correct UF2 from the UF2 folder (_w for Pico W)
- Flash the Pico with the UF2 (see 3.2 https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- Build the circuit (see below) - best to use the PCB (https://www.pcbway.com/project/shareproject/Raspberry_Pi_Pico_ZX_Spectrum_Microdrive_Hardware_Emulator_37750e81.html)
- Connect to the IF1 
- Turn on the Spectrum and hopefully all good

## Version History
- v1.3 (latest release) - Added [Quick Swap](#quick-swap) functionality to Drive 1 & added wrap around to the file explorer
- v1.2 - improved write code (timings, input delays, SD Card buffers) to remove chance of bad sectors plus some bug fixes
- v1.1 - added [TAP copy](#tap-copy), version number on splash screen & bug fixes
- v1.0beta2c  - fixed real hardware not working with the ZXPicoMD. *Note v1.1 PCB needs a small hardware mod to work, see mod section below
- v1.0beta1 - added support for real hardware Microdrives downstream of the ZXPicoMD *Note doesn't work with real h/w Microdrives, tested working with vDrive emulated drives only.
- v0.94 - added a simple filename generator to the cartridge save option
- v0.93 - fixed MF128 format
- v0.92 - added sound on/off menu option to turn off the buzzer, saves config to the SD Card
- v0.91 - first public release, all basic functions in place

## First Usage
First you need to format your micro SD card using a PC/MAC, I recommend exFAT but FAT32 is also fine. Using the same PC/MAC you can also create a folder for your files and copy games or programs into these directories. You don't need any files for the device to work so this is just if you want to use it to load games etc... Insert the micro SD Card into the device while it is powered off and then turn on the Spectrum with the device connected. If all ok you will see a Splash Screen followed by a screen saying it is setting up the SD Card. Just wait for this to complete and once the screen goes blank and the blue LED comes on you are good to go.

## Usage
The device is very simple to use, upon booting the Spectrum you will see a Splash Screen which will disappear after a couple of seconds. The screen will then stay blank (OLED is off to prevent burn-in) and the blue LED (or whatever colour you picked) will light up on the side to indicate the device is ready for input, either from the IF1 or by the user pressing a button. You can now use all the normal commands on the Spectrum to control the drive such as `RUN`, `LOAD *"m";1;"aaa"` or `FORMAT "m";5;"blank"`. 

When the blue LED is on so the unit is input ready you can enter the Menu to select new cartrdiges etc... for full list of menu items see the chart below. To enter the menu press the enter key which is 3rd from left. You should now see the Cartridge 1 icon. Pressing enter again takes you to the cartridge sub-menu, showing a folder icon. Pressing again and you will enter the file explorer where you can navigtate the listings to pick a new cartridge or snapshot to convert.

The buttons are (from the left): Previous; Next; Enter; Back; Reset

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/buttons.png "Button Layout")

Previous & Next are used to go left/right or up/down in the menu or file explorer. Enter is used to select an option or file (also to enter the menu in the first place). Back goes up one level in the menu or when using the file explorer moves up one directory. Reset reboots the Pico if all else fails (also useful when flashing the Pico). If you hold down the buttons it also behaves differently in certain scenarios, for example holding down Next in the file explorer goes down one page of file entries rather than just one file entry.

The LEDs show certain activity:
1. The top LED (furthest from the buttons) is Drive and Write for when the Microdrive is being accessed. Drive will come on when the simulated motor is on (if you installed a buzzer you will also hear it). Write will come on (replacing Drive) when the ZXPicoMD is receiving a WRITE signal.
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
- Quick Swap (Drive 1 only) - turns on/off [Quick Swap](#quick-swap) functionality
- Switch to Real H/W - switches the drive number to a real H/W Microdrive. With this enabled the ZXPicoMD will emulate the shift register to activate the drive when called
- Cartridge Info - shows a basic CAT of the cartridge inserted
- Write Portect - toggles write protect on/off for the cartridge inserted
- Save Cartridge - creates a copy of the cartridge on the SD card so all your work doesn't get lost if you load a new cartridge into the drive
- Insert Formatted - as it says load a blank formatted (to 127kB) cartridge into the drive

The file explorer will only show compatible files, those with extension MDR, TAP, Z80 & SNA and directories.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/menulayout_v3.png "Menu Layout v3")

### Quick Swap

Firmware v1.3 added quick swap functionality to drive 1 allowing the user to quickly swap between 4 pre-loaded cartridges. This was only added to drive 1 as this drive number is often hardcoded into programs so being able to quickly flip between cartridges is an advantage.

To use Quick Swap on drive 1 you have to first turn it on. To do this go into the menu (enter button), select drive 1 and scroll one right of the folder icon. You should see an icon with a swap symbol. Click on this and you will get a message that it is setting up Quick Swap for Drive 1. This takes a few seconds as it is creating 4 new cartridges, copying the contents of the current drive 1 cartridge to A, and creating 3 blank cartridges B,C & D. Once done it goes back to idle (blue light) and is ready to use.

To quickly swap the cartridges just press the PREV or NEXT buttons and a selector will pop up showing cartridges 1A to 1D. Just pick the one you want, press enter and it will complete the swap. Simple as that.

If you want to put a new image into a certain quick swap cartridge just first select the one you want, say 1D, and then just use the normal menu options to load a Z80 or format it etc... If you turn off quick swap (using the same icon), the cartridge currently selected remains in drive 1 and the others are lost, so recommend you save them first if needed.

### TAP copy

Selecting a TAP file from the file explorer will copy the contents to a blank cartridge. It works with headerless files as well as normal and also removes the autorun from basic files so they can be edited. As headerless files have no file type or start address these are simply set as a code block starting at memory end minus length.

Note this is a simple file copy so if you are wanting to run a loader, usually for a game, off the cartridge you will need to adjust said loader to use Microdrive commands. Unfortunately this isn't always possible as a lot of games use machine code loaders but some are and the following details how to do this for the classic Manic Miner.

Once the copy has complete `CAT` the drive and you will see

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm1.png "MM CAT")

Now Load the `(Info)` file:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm2.png "MM (Info) Load")

This will show details of the files copied from the TAP in `REM` statements. Write down the filenames `mm1` (the loading screen) & `MM2` (the main code) for later.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm4.png "MM (Info) Load 2")

Now load the main basic loader `ManicMiner`:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm5.png "MM Basic 1")

and `LIST`

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm7.png "MM Basic 2")

Now edit the loader with the correct Microdrive syntax `*"m";1;` and filenames you wrote down earlier:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm8.png "MM Basic 3")

Save this as `RUN` with autorun `LINE 1` added so you can use the shortcut on reboot:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mm9.png "MM RUN")

The game will now load using `RUN`:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/TAPtoMDR/mmload.png "MM LOAD")

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

The following circuit does not include the level shifter which is no longer required due to the IF1 accepting the lower voltage output of the Pico ~3V. For the old circuit please click here https://github.com/TomDDG/ZXPicoMD/blob/main/Images/PicoDriveZX_Circuit_v3a.png

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/PicoDriveZX_Circuit_v4a.png "Circuit v4a")

## v1.1 PCB Hardware Mod for Real H/W Support

Unfortunately during testing it became apparant that the BOB level shifter interferes with the data output of the real Microdrive and lacks the OE pin which would put it into high impedence mode. During initial testing everything worked fine with a vDrive, in place of real h/w, as its signal strength is much stronger and therefore wasn't affected by the BOB.

Luckily this is extremly simple to fix and doesn't need any complex re-routing or code changes, in fact all you need to do to get the v1.1 PCB working with real hardware is to replace the BOB-12009 level shifter with 3 diodes as shown in the image below (note the cathode line at the top HV side). I've tested this with standard 1N4148 and BAT43 Schottky diodes. The BAT diodes have a better voltage drop, measuring I show 3.0V for the 1N4148 and 3.2V with the BAT. Recommend to use BATs although 1N4148 seem to work fine.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/diodes.png "Hardware Mod")

## BoM

- 1x Pico or Pico W with headers soldered
  - +2x 20pin header sockets
- 1x Traco Power TSR 1-2450 (9v to 5v, 1 Amp) - https://www.tracopower.com/int/model/tsr-1-2450 (you can use alternatives these are just the best and very efficient, if you do use alternatives remember you may need additional circuitry)
- 3x BAT42/43 diodes for D1(D), D2(D) & CO(D). If using a v1.1 PCB connect these LV1-HV1, LV2-HV2 & LV3-HV3, line at HV side. BAT42/43 are a better option to 1N4148 due to the reduced voltage drop, however 1N4148 should also be ok. The v1.1 PCB can also use a SparkFun Logic Level Converter ([BOB-12009](https://www.sparkfun.com/products/12009)) if not paring with real h/w Microdrives, however recommend you use diodes.
- 1x SSD1306 OLED 0.96" (you can get larger ones just make sure they are SSD1306). Be very careful of the GND & VCC placement as they are sometimes reversed
  - +1x 4pin header socket if using the PCB. If also using the 3D printed case get header sockets with extra long legs so the OLED can be mounted higher
- 1x Adafruit Micro SD SPI or SDIO Card Breakout Board - https://www.adafruit.com/product/4682
  - +1x 9pin header socket if using the PCB
- 5x 6×6mm Right Angle Micro Push Buttons for mounting on the PCB (you can use any push button these are just the ones that I use)
- 2x Bi-Colour LEDs, common cathode (centre negative). Pick whatever colours you want and change the resistors to match, my rule of thumb is forward voltage/2 in kOhms so if forward voltage is 2V use a 1kOhm resistor. The LEDs are (D)rive/(W)rite and (S)D Card Access/(I)nput Ready.
- 5x 1n4148 diodes [WR.PR(D), ERASE(D), COMMS(D), RW(D), CLK(D)]. These could also be replaced with BAT42/43 diodes but only WR.PR is output.
- 1x 1n4001 diode for the 5v in, this is to allow USB to be connected and the Spectrum at the same time (any of the 400x series should work, I use 4004 as it was the one I had in stock) [5V(D)]
- 1x 10kOhm Resistor for write protect circuit [WPC(R)]
- 1x 6.8kOhm Resistor for write protect circuit [WPB(R)]
- 1x 2N2222 Transistor for write protect circuit
- 1x 1407 Type Passive Piezo Buzzer (if you use the PCB). 
- 1x 3D Printed Case - STLs for the 3D Printed Case can be download from Printables (https://www.printables.com/model/297015-case-for-zx-picomd-sinclair-zx-spectrum-microdrive) or Thingiverse (https://www.thingiverse.com/thing:5569842)
  - 1x #4 (3mm) 1/4" (6.5mm) screw to secure the PCB to the case and 2x #4 (3mm) 1/2" (13mm) screws to secure the top of the case to the bottom

## PCB

I have designed a PCB to house everything and you can purchase v1.1 on PCBWay (https://www.pcbway.com/project/shareproject/Raspberry_Pi_Pico_ZX_Spectrum_Microdrive_Hardware_Emulator_37750e81.html). I've also entered it into the 5th PCB Design Contest (https://www.pcbway.com/activity/5th-pcb-design-contest.html)

- v1.0 - original design for testing, lacks COMMS OUT & WRITE PROTECT circuit
- v1.1 - added simple transistor circuit to control Write Protect and also passes the COMMs down the chain for additional Microdrives. 
- v1.2 - removed bob-12009 level shifter and replaced with diodes, some refactoring. *Note this is simply a cosmetic change and v1.1 works fine with real h/w by simply replacing the bob with diodes as shown in the hardware mod section

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/ZXPicoMDv1.2.png "PCB v1.2")

## Case

The PCB was originally designed to fit in a real Microdrive case as it is based on my Oqtadrive PCB. As such the screw holes etc... line up perfectly, however, the addition of the buttons and OLED mean, although it will fit, you won't be able to use it without cutting the case. As such I designed a custom 3D case (https://www.thingiverse.com/thing:5569842) to match the Spectrum + models such as the 128k Toastrack.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/IMG_0151.jpg "In Case")

## Building with Case

This guide is based on v1.2 of the PCB, however it also applies to v1.1 with the diodes placed where the BOB was. 

First solder in the diodes, resistors, buttons, buzzer, transistor & TSR 1-2450 as per the silk screen, being mindful of the orientation of the diodes. I recommend you use BAT42/43 Schottky diodes for Data 1 [D1(D)], Data 2 [D1(D)] & Comms Out [CO(D)] as these are small signal outputs which may benefit from the lower voltage drop.

Follow this with the header sockets for the Pico (20-pin) and Micro SD Card reader (9-pin). Ensure these are straight before fully soldering with the best way being to solder one leg check it is straight and then soldering the rest. For the Pico header I'd recommend pluging in the Pico first, use a bit of blue tack to hold in place, and then solder ensuring it all aligns. 

Now solder in the bi-colour LEDs. Mount these so the bulb is slightly above the top of header socket, this gives enough height to bend into place later.

After this you should have the following set-up (blue diodes are BATs, red are 1N4148s):

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/DidodePlacement.png "Diode Placement")

The OLED socket needs to be raised off the PCB in order for the screen to fit in the top of the case, these longer legs sockets are often referred to as stackable pin header sockets. The best way to get this at the correct height is to first mount the OLED into the top case, plug the header socket in, and then put the PCB on top (bottom up). Ensure the PCB is straight and then solder in place. It should now look like this:

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/OLEDMount.png "OLED Mount")

You can now fit the Pico & Micro SD-Card socket and mount into the bottom part of the case by sliding in the front buttons first and then securing with a small screw. Be careful when bending the LED legs to fit the side holes and ensure the Micro SD card slot is flush to the side of the case as shown in the photo.

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/AllBuilt.png "All Built")

Pop the top of case on but remember the OLED screen pins need to fit in the raised socket. This can take a couple of goes. A final touch is to add a rainbow sticker to match the original case. I've created a PDF sticker sheet if you want to print your own (https://github.com/TomDDG/ZXPicoMD/blob/main/Images/stripes.pdf)

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/withstripes.jpg "with Stripes")

## Testing the ZX PicoMD

Once built I run the device through a series of tests. The main one is a test cartridge by Martin Smith, you can download this from the TestCarts folder (https://github.com/TomDDG/ZXPicoMD/blob/main/TestCarts/Test%20MDR%20Emulation%20(1994)(Martin%20Smith).mdr). This utility was initially designed to test emulators so works great for making sure everything is ok with the ZX PicoMD. Another test I run is making sure the write protect circuit is ok by toggling it on and trying to format a cartridge, this should fail with a write protect message, toggling it back off and it should work again.

In addition I've also uploaded a IF1 ROM test cartridge (based on the tape version by Blaze) which determines the ROM version of the IF1 which was useful for degbugging as I've only been able to test ZX PicoMD with a v2 ROM (https://github.com/TomDDG/ZXPicoMD/blob/main/TestCarts/IF1ROM.mdr).

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/if1rom.png "IF1 ROM version")

## FAQ

- I'm getting a `Microdrive not present, 0:1` message
  - Microdrives & Interface 1s are getting old now so make sure the edge connectors are clean. Use some Isopropyl (Rubbing) alchohol to clean the PCB connector on the IF1 (left hand side). Also check the Microdrive connectors are fully plugged in.
  - There is a known issue with some Spectrums that on first boot the CLK signal is set low when it should be high. This confuses the ZXPicoMD as it keeps thinking there is an incoming CLK pulse, a simple retype of the command will fix it and it will then be ok till next cold boot.

- Why is my SD Card slow to list the directory? I've tested this with ~1000 files in a single directory and it should take no longer than a second to show a directory listing. If you are finding access is slow, please try the following:
  - Don't put thousands of files in a single directory, try to limit it to <1000 per directory otherwise it will take a long time to find what you need as the OLED only shows 4 files per screen. Put the files in a-z directories as an example.
  - Copy everything off your SD Card onto a PC, reformat the SD Card and copy it all back on. This ensures the files are on the SD Card sequentially which speeds up access significantly. This also helps as I use a basic sort algorithm, if the files are already sorted this has less to do.
  - Try to use a modern Micro SD Card of at least C10 rating. As of writing the cheapest cards seem to be the 32GB ones which are most likely UHS Speed Class which will work fine and way more space than you need. I've configured the SD Card access speed to be 12.5Mbits/s which is ~1.5MBytes/s (more like half that in reality) so anything above that in theory should be fine. Image below shows the results from a quick speed test on a couple of cards, top one is a class 10 and the other is an older 2GB rated for 30MB/s. Both of these cards worked fine:
  
![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/SDCardSpeed.png "SD Card Speed")

- Can I use ZIP files?
  - The ZX PicoMD does not currently work with ZIP files. The main reason for this is the need to compress files to save space is kind of redundant with the size of the cheapest modern SD Cards. A 16GB or 32GB Micro SD card can easily store the entire uncompressed Spectrum library multiple times, including having both Z80s, SNAs, MDRs & TAPs. Z80 files are also already compressed, although not as well as ZIPs would be. Some quick maths shows that if we used uncompressed MDRs, which are 137923Bytes in size, a 16GB SD Card can store over 100,000 microdrive cartridges! Z80 & TAPs are usually smaller than MDRs. As of writing this a 32GB SanDisk branded micro SD card is £6 on Amazon.
