cmake_minimum_required(VERSION 3.6)

# toolchain https://www.obdev.at/products/crosspack/index.html
# https://tomlankhorst.nl/avr-develop-debug-on-mac-os-x-with-clion/
##################################################################################
# tools to be used for programming the AVR
##################################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER arduino)
set(AVR_UPLOADTOOL_PORT /dev/tty.wchusbserial1420)
set(AVR_UPLOADTOOL_BAUDRATE 57600)
# AVR and fuses needs to be set
set(AVR_MCU atmega328p)
#set(AVR_H_FUSE 0x00)
#set(AVR_L_FUSE 0x00)
#set(ASF /Users/raczattila/Downloads/xdk-asf-3.45.0)
project(my_avr_project)
set(SOURCE_FILES
        main.c
        mcp2515.c
        my_utils.h
        uart.c
        )

#set_property(SOURCE asm_test.S PROPERTY LANGUAGE ASM)

add_compile_options(
        -O1
)

add_avr_executable(
        my_avr_project
        ${SOURCE_FILES}
)
# CMAKE COMMANDS
# https://the-missing-bit.com/post/2018-07-11-avr-cmake/

# manual build
# https://blog.podkalicki.com/how-to-compile-and-burn-the-code-to-avr-chip-on-linuxmacosxwindows/
# avr-gcc -Wall -g -Os -mmcu=atmega328P -o main.bin avr_blink.c
# avr-objcopy -j .text -j .data -O ihex main.bin main.hex
# avrdude -c arduino -p atmega328p -P /dev/tty.wchusbserial1420 -b 57600 -U flash:w:main.hex:i

# EMERGENCY
# BOOTLOADER REFLASH WITH RPI
# https://learn.adafruit.com/program-an-avr-or-arduino-using-raspberry-pi-gpio-pins/overview