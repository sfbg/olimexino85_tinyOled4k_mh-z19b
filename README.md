# Olimexino-85 CO2 monitor with OLED display (MH-Z19B sensor)

### Hardware list:
- [Olimexino-85BC](https://github.com/OLIMEX/OLIMEXINO-85) (ATTINY85-based Digispark clone) with 5V jumper setting
- 0.96 inch 128x64 I2C SSD1306 OLED display
- MH-Z19B infrared CO2 sensor (0-5000PPM)

### Olimexino-85 board data conections:
- 0 - SDA OLED
- 1 - RX MH-Z19B
- 2 - SCL OLED
- 4 - TX MH-Z19B

### Details
This bases very heavily on **https://github.com/andvolodko/digispark_oled_128x32_mh-z19b** (higly recommended to check it out first and big thanks to the author) but is using the standard [TinyWireM](https://github.com/adafruit/TinyWireM) and [Tiny4kOLED](https://github.com/datacute/Tiny4kOLED) libraries for display instead of the custom implementation. 

I had to reimplement the display part as I had noise and other artefacts on my 128x64 OLED with the above repo. Part of the solution was to change the display buffer in the TinyWireM.h library file to 30 (found the hint [here](https://github.com/OLIMEX/OLIMEXINO-85/blob/master/SOFTWARE/O85-examples/O85_MOD_LCD_1x9/O85_MOD_LCD_1x9.ino)):

``#define USI_BUF_SIZE 30 //!< bytes in message buffer``

As the libraries require more space, **the display now shows just the CO2 PPM level in a centered 16x24px font and nothing else**. The sketch requires 6010 out of 6012 bytes available on the Digispark clones.

