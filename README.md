# LPD6803 driver for the ESP8266 on FreeRTOS

Driver for an LPD6803 based LED strip for use in FreeRTOS on the ESP8266. I built this using the excellent [esp-open-rtos](https://github.com/SuperHouse/esp-open-rtos) project, but it should work with the Espressif RTOS SDK with little modification. This improves on other implementations I've found by making use of the SPI hardware in the ESP8266 rather than bit-banging it with GPIO pins. I've tried to provide a simple interface for making complex LED strip effects that can be displayed time-accurately with a user-definable period.

I've been able to drive a strip of 50 addressable segments at up to around 25MHz as the datasheet specifies before I start getting errors on the far end of the strip. I've been using 20MHz because it's easy to set up the SPI clock for that. The library is currently set to refresh the entire strip once every 10ms (100Hz) for very smooth animations. This can be changed in `lpd6803.h`.

## Getting started
Just throw this repository in the extras folder of your esp-open-rtos SDK and you're good to go. You can optionally copy the example folder wherever you'd like and modify it for a quick start. Just make sure to update the relative path to common.mk in its Makefile.

## Hardware connection
This library uses the SPI hardware in the ESP8266 and thus is limited to using pins that can be muxed to the SPI hardware. By default SPI clock and data (MOSI) are shared with GPIO pins 14 and 13 respectively on boot. You'll need to figure out where those are connected on your breakout.

## Notes
I've provided a few effect implementations in the effects.c file which you can optionally include and use in your project. They use a few of the helpful macros defined in `lpd6803.h` which may also be of use to you, but can be a bit tricky to understand at first. The premade effects should give you a good overview of their use.

## References
* [LPD6803 Datasheet](https://www.adafruit.com/datasheets/LPD6803.pdf) from Adafruit
* [SPI Driver](https://github.com/MetalPhreak/ESP8266_SPI_Driver) with some modifications to make it work with esp-open-rtos
