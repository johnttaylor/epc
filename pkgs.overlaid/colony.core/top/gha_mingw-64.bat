:: This script is used by the CI\Build machine to build using the MinGW  
:: compiler for the Windows 64BIT projects
::
:: usage: gha_mingw-64.bat <buildNumber> [branch]
::
set _TOPDIR=%~dp0
set _ROOT=%_TOPDIR%..
echo:%_ROOT%
echo:%_TOPDIR%

:: Set the CI build flag
set NQBP_CI_BUILD=1

:: Set the NQBP_BIN path (and other magic variables - but no compiler selected)
call ./env.bat

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
:: Build Mingw projects
::

::echo on
::call %_ROOT%\env.bat 3

:: Build the Catch2 static library
cd %_ROOT%\projects
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python mingw_w64 -c --try win64
::python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python mingw_w64 --try  cpp11


:: Build the unit tests
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python mingw_w64 -c --bldtime --try win64 --bldnum %BUILD_NUMBER%
::python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python mingw_w64 --bldtime --try cpp11 --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Run unit tests
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\chuck.py -vt --match a.exe --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
python %_ROOT%\xsrc\nqbp2\other\chuck.py -v --match aa.exe --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
python %_ROOT%\xsrc\nqbp2\other\chuck.py -vt --script-prefix python --match a.py --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
python %_ROOT%\xsrc\nqbp2\other\chuck.py -v --script-prefix python --match aa.py --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1

::
:: Everything worked!
::
exit /b 0
