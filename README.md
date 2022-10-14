# ZXPicoMD
Raspberry Pico ZX Spectrum Microdrive Hardware Emulator

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

I have designed a prototype PCB to house everyting

![image](https://github.com/TomDDG/ZXPicoMD/blob/main/Images/pico_md.png "PCB Prototype")
