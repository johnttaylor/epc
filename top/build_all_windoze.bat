@echo on
:: This script is used by the CI\Build machine to build the Windows test
:: projects
::
:: usage: build_all_windoze.bat <buildNumber> [branch]
::

set _TOPDIR=%~dp0
set _TOOLS=%_TOPDIR%..\xsrc\nqbp2\other

:: Set Build info (and force build number to zero for "non-official" builds)
set BUILD_NUMBER=%1
set BUILD_BRANCH=none
IF NOT "/%2"=="/" set BUILD_BRANCH=%2
echo:
echo:BUILD: BUILD_NUMBER=%BUILD_NUMBER%, BRANCH=%BUILD_BRANCH% 
echo:

:: Run Doxygen first 
::cd %_TOPDIR%
::run_doxygen.py %BUILD_NUMBER% %BUILD_BRANCH% 
::IF ERRORLEVEL 1 EXIT /b 1

::
:: Build Mingw projects (just the Win32 builds)
::
::call %_TOPDIR%..\env.bat 3
::@echo on
::
:::: Build NON-unit-test projects
::cd %_TOPDIR%..\projects
::%_TOOLS%\bob.py -v4 mingw_w64 -c --bldtime -b win32 --bldnum %BUILD_NUMBER%
::IF ERRORLEVEL 1 EXIT /b 1
::
:::: Build unit test projects
::cd %_TOPDIR%..\tests
::%_TOOLS%\bob.py -v4 mingw_w64 -c --bldtime -b win32  --bldnum %BUILD_NUMBER%
::IF ERRORLEVEL 1 EXIT /b 1

:: Run unit tests
cd %_TOPDIR%..\tests
%_TOOLS%\chuck.py -v --match a.exe --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match aa.exe --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match a.py --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match aa.py --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1

:: Generate code coverage metrics
%_TOOLS%\chuck.py -vt --dir mingw_w64 --match tca.py rpt --xml coverage.xml 
IF ERRORLEVEL 1 EXIT /b 1

::
:: Build Visual Studio projects (just the win32 builds)
::
::call %_TOPDIR%..\env.bat 1
::@echo on
::
:::: Build NON-unit-test projects 
::cd %_TOPDIR%..\projects
::%_TOOLS%\bob.py -v4 vc12 -c --bldtime -b win32 --bldnum %BUILD_NUMBER%
::IF ERRORLEVEL 1 EXIT /b 1
::
:::: Build unit test projects
::cd %_TOPDIR%..\tests
::%_TOOLS%\bob.py -v4 vc12 --bldtime --clean-all -b win32 --bldnum %BUILD_NUMBER%
::IF ERRORLEVEL 1 EXIT /b 1

:: Run unit tests
cd %_TOPDIR%\..\tests
%_TOOLS%\chuck.py -v --match a.exe --dir vc12
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match aa.exe --dir vc12
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match a.py --dir vc12
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match aa.py --dir vc12
IF ERRORLEVEL 1 EXIT /b 1

::
:: Build STM32 Target projects
::

call %_TOPDIR%..\env.bat 5
@echo on

:: Build NON-unit-test projects

cd %_TOPDIR%..\projects
%_TOOLS%\bob.py -v4 --p2 windows gcc-arm --bldtime -c --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Build unit test projects

cd %_TOPDIR%..\tests
%_TOOLS%\bob.py -v4 --p2 windows gcc-arm --bldtime -c --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1


:: TODO: Add Linux builds....
:: Build linux projects (under WSL)
:: Note: Because of Windows/Linux/Git newline issues - we brute force the shell scripts to have the correct newline characters
::
::FOR /F "tokens=*" %%g IN ('%_TOPDIR%win2wsl %_TOPDIR%') do (SET WSL_TOPDIR=%%g)
::wsl cd %WSL_TOPDIR%; dos2unix wsl_build.sh; cd ..; dos2unix env.sh; top/wsl_build.sh %BUILD_NUMBER%
::IF ERRORLEVEL 1 EXIT /b 1


::
:: Skip additional project builds when NOT build a develop or main
::
IF "%BUILD_BRANCH%"=="none" GOTO :builds_done

:: TODO: Zip up (NON-debug) 'release' builds
echo:zipping non-debug release builds...

:: TODO: Build DEBUG 'release' builds
echo:Building debug release builds...

:: TODO: Zip up (debug) 'release' builds
echo:zipping debug release builds...

:: TODO: Coverage Report
:: TODO: Archive build artifacts

::
:: Everything worked!
::
:builds_done
echo:EVERTHING WORKED
exit /b 0
