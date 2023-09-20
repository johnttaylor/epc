:: Quick and dirty build script for the Visual studio compiler
:: Builds will a 'null' seed() implementation
del *.obj
cl /EHsc /FS /Zi /Od /D "_DEBUG" /RTCs /D DEBUG_BUILD /c signer.cpp docopt.cpp ../*.c ../_null/*.c 
link /nologo /subsystem:console /pdb:eros-sim.pdb /machine:X86 /debug /NODEFAULTLIB:LIBCMT *.obj libcmtd.lib /out:signer.exe
