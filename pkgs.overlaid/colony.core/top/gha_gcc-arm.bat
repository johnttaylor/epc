:: This script is used by the CI/Build machine to build using the GCC  
:: ARM Cortex M/R compiler on a Windows host
::
:: usage: gha_gcc-arm.bat <buildNumber> [branch]
::
set _TOPDIR=%~dp0
set _ROOT=%_TOPDIR%..
echo:%_ROOT%
echo:%_TOPDIR%

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
:: Build the unit tests
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc-arm -c --bldtime --bld-all --bldnum %BUILD_NUMBER%

IF ERRORLEVEL 1 EXIT /b 1

::
:: Build ATSAMD51 Arduino projects (if any)
::

:: Set up the compiler (which is included in the repo)
call ./env.bat 4

:: Build the unit tests
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python --p2 windows gcc -c --bldtime --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

::
:: Everything worked!
::
exit /b 0
