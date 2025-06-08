:: This script is used by the CI/Build machine to build using the GCC  
:: ARM Cortex M/R compiler on a Windows host
::
:: usage: gha_gcc-arm.bat <buildNumber> [branch]
::
set _TOPDIR=%~dp0
set _ROOT=%_TOPDIR%..
echo:%_ROOT%
echo:%_TOPDIR%
set _TARGET=alpha1
set _TARGET2=alpha1-atmel

:: Set Build info (and force build number to zero for "non-official" builds")
set BUILD_TYPE=%2
set BUILD_NUMBER=%1
IF "%BUILD_TYPE%"=="pr" set BUILD_NUMBER=0
IF "%BUILD_TYPE%"=="PR" set BUILD_NUMBER=0
IF "%BUILD_TYPE%"=="unknown" set BUILD_NUMBER=0
IF "/%BUILD_TYPE%" =="/" set BUILD_TYPE=private
echo:
echo:BUILD TYPE=%BUILD_TYPE%, BUILD_NUMBER=%BUILD_NUMBER%
echo:

::
:: Build STM projects
::

:: Set up the compiler (which is included in the repo)
call ./env.bat 5
echo:%PATH%

:: Build the Projects (non-debug)
cd %_ROOT%\projects
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc-arm -c --bldtime --bld-all --bldnum %BUILD_NUMBER%

:: Build the unit tests
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc-arm -c --bldtime --bld-all --bldnum %BUILD_NUMBER%

IF ERRORLEVEL 1 EXIT /b 1


::
:: Build Arduino projects
::

:: Set up the compiler (which is included in the repo)
call ./env.bat 4
echo:%PATH%

:: Build the Projects (non-debug)
cd %_ROOT%\projects
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc -c --bldtime --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Build the unit tests
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc -c --bldtime --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1


::
:: Skip additional project builds when NOT building develop or main
::
IF "%BUILD_BRANCH%"=="none" GOTO :builds_done

:: STM32: Zip up (NON-debug) 'release' builds
cd %_ROOT%\projects\GM6000\Ajax\%_TARGET%\windows\gcc-arm\_stm32
7z a ajax-%_TARGET%-%BUILD_NUMBER%.zip ajax.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

cd %_ROOT%\projects\GM6000\Eros\%_TARGET%\windows\gcc-arm\_stm32
7z a eros-%_TARGET%-%BUILD_NUMBER%.zip eros.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

:: Arduino: Zip up (NON-debug) 'release' builds
cd %_ROOT%\projects\GM6000\Ajax\%_TARGET2%\windows\gcc\_arduino
7z a ajax-%_TARGET2%-%BUILD_NUMBER%.zip ajax.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

cd %_ROOT%\projects\GM6000\Eros\%_TARGET2%\windows\gcc\_arduino
7z a eros-%_TARGET2%-%BUILD_NUMBER%.zip eros.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

::
:: DEBUG builds
::

:: STM32: Build DEBUG version of the Target builds
cd %_ROOT%\projects
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc-arm -cg --bldtime --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Zip up DEBUG target builds
cd %_ROOT%\projects\GM6000\Ajax\%_TARGET%\windows\gcc-arm\_stm32
7z a ajax-%_TARGET%-DEBUG-%BUILD_NUMBER%.zip ajax.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

cd %_ROOT%\projects\GM6000\Eros\%_TARGET%\windows\gcc-arm\_stm32
7z a eros-%_TARGET%-DEBUG-%BUILD_NUMBER%.zip eros.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

:: Arduino: Build DEBUG version of the Target builds
cd %_ROOT%\projects
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc -cg --bldtime --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Zip up DEBUG target builds
cd %_ROOT%\projects\GM6000\Ajax\%_TARGET2%\windows\gcc\_arduino
7z a ajax-%_TARGET2%-DEBUG-%BUILD_NUMBER%.zip ajax.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

cd %_ROOT%\projects\GM6000\Eros\%_TARGET2%\windows\gcc\_arduino
7z a eros-%_TARGET2%-DEBUG-%BUILD_NUMBER%.zip eros.*
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

::
:: Everything worked!
::
:builds_done
exit /b 0
