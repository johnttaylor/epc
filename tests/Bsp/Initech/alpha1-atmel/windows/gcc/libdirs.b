# Core CPL
src/Cpl/Container
src/Cpl/System
src/Cpl/System/_trace
src/Cpl/Text
src/Cpl/Io/Stdio
src/Cpl/Io

# CPL - Board specific
src/Cpl/System/FreeRTOS
src/Cpl/System/FreeRTOS/_fatalerror
src/Cpl/System/_trace/_arduino
src/Cpl/Io/Serial/Arduino

# Ardunio Core library for the Adafruit's Grand Central M4 board (Atmel SAMD51 mcu)
arduino/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino/USB
arduino/hardware/samd/$ARDUINO_BSP_VER$/libraries/Wire
arduino/hardware/samd/$ARDUINO_BSP_VER$/variants/grand_central_m4
arduino/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino > main.cpp cortex_handlers.c wiring.c

src/Bsp/Adafruit/grand_central_m4/gcc > sdfat.cpp
src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source
src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source/portable/GCC/ARM_CM4F  
src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source/portable/MemMang  

