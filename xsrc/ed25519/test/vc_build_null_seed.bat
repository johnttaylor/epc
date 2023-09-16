:: Quick and dirty build script for the Visual studio compiler
:: Builds will a 'null' seed() implementation
del *.obj
cl test.c ../*.c ../_null/*.c 
