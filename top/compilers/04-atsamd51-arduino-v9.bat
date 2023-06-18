echo off
IF "/%1"=="/name" ECHO:GCC-ARM compiler for Grand Central BSP 1.6.0 & exit /b 0

:: Core Arduino tools
set ARDUINO_TOOLS=C:\Users\johnt\AppData\Local\Arduino15\packages\adafruit
set ARDUINO_COMPILER_VER=9-2019q4
set ARDUINO_BSP_VER=1.6.0

:: Segger support
set PATH=%PATH%;c:\Program Files (x86)\SEGGER\JLink
set PATH=%PATH%;c:\Program Files\SEGGER\Ozone

::
ECHO:Environment set for GCC-ARM compiler for Grand Central BSP 1.6.0