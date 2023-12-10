# target specific stuffs
src/Ajax/Main/_plat_alpha1_atmel
src/Ajax/Main/_plat_alpha1_atmel/_app_platform

# CPL - Board specific
#src/Cpl/System/FreeRTOS
src/Cpl/System/FreeRTOS/_fatalerror
src/Cpl/System/_trace/_arduino
src/Cpl/Io/Serial/Arduino

# Ardunio Core library for the Adafruit's Grand Central M4 board (Atmel SAMD51 mcu)
arduino/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino/USB
arduino/hardware/samd/$ARDUINO_BSP_VER$/libraries/Wire
arduino/hardware/samd/$ARDUINO_BSP_VER$/libraries/SPI
arduino/hardware/samd/$ARDUINO_BSP_VER$/libraries/Adafruit_ZeroDMA
arduino/hardware/samd/$ARDUINO_BSP_VER$/variants/grand_central_m4
arduino/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino > main.cpp cortex_handlers.c 

src/Bsp/Initech/alpha1-atmel > sdfat.cpp
src/Bsp/Initech/alpha1-atmel/FreeRTOS/source
src/Bsp/Initech/alpha1-atmel/FreeRTOS/source/portable/GCC/ARM_CM4F  
src/Bsp/Initech/alpha1-atmel/FreeRTOS/source/portable/MemMang  


# Common stuffs
../../../ajax_common_libdirs.b
../../../ajax_target_common_libdirs.b
../../../../common_libdirs.b
../../../../target_common_libdirs.b
../../../../arduino_target_common_libdirs.b
