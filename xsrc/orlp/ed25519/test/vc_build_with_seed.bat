:: Quick and dirty build script for the Visual studio compiler
:: Builds will a native Win32 seed() implementation
del *.obj
cl test.c ../*.c ../_win32/*.c Advapi32.lib
