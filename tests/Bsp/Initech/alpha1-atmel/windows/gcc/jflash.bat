@echo off
:: Script to program the .BIN using Segger's JLINK 
JFlash.exe -openprjatsamd51P20-no-bootloader.jflash -open_arduino\blink.bin -auto -exit


