@echo on
:: This script is used by the CI\Build machine to build the Windows Host
:: projects
::
:: usage: build_all_windoze.bat <buildNumber> [branch]
::

set _TOPDIR=%~dp0
set _TOOLS=%_TOPDIR%..\xsrc\nqbp2\other
set _ROOT=%_TOPDIR%..
set _TARGET=alpha1

:: Set Build info (and force build number to zero for "non-official" builds)
set BUILD_NUMBER=%1
set BUILD_BRANCH=none
IF NOT "/%2"=="/" set BUILD_BRANCH=%2
IF "%BUILD_BRANCH%"=="none" set BUILD_NUMBER=0
echo:
echo:BUILD: BUILD_NUMBER=%BUILD_NUMBER%, BRANCH=%BUILD_BRANCH% 
echo:

:: Make sure the _artifacts directory exists and is empty
cd %_ROOT%
rmdir /s /q _artifacts
mkdir _artifacts


:: Run Doxygen first (and copy the output to artifacts dir)
cd %_TOPDIR%
run_doxygen.py %BUILD_NUMBER% %BUILD_BRANCH% 
IF ERRORLEVEL 1 EXIT /b 1
copy %_ROOT%\docs\sdx-1330-gm6000-software-doxygen-output.chm %_ROOT%\_artifacts\sdx-1330-gm6000-software-doxygen-output__%BUILD_BRANCH%-%BUILD_NUMBER%.chm
IF ERRORLEVEL 1 EXIT /b 1

::
:: Build Mingw projects (just the Win32 builds) 
::
call %_ROOT%\env.bat 3
@echo on

:: Build NON-unit-test projects
cd %_ROOT%\projects
%_TOOLS%\bob.py -v4 mingw_w64 -c --bldtime -b win32 --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Build unit test projects (debug builds for more accurate code coverage)
cd %_ROOT%\tests
%_TOOLS%\bob.py -v4 mingw_w64 -cg --bldtime -b win32  --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Run unit tests
cd %_ROOT%\tests
%_TOOLS%\chuck.py -v --match a.exe --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match aa.exe --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match a.py --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1
%_TOOLS%\chuck.py -v --match aa.py --dir mingw_w64
IF ERRORLEVEL 1 EXIT /b 1

:: Generate code coverage metrics
%_TOOLS%\chuck.py -v --dir mingw_w64 --match tca.py --ci rpt --xml jenkins-gcovr.xml 
IF ERRORLEVEL 1 EXIT /b 1

::
:: Build Visual Studio projects (just the win32 builds)
::
call %_ROOT%\env.bat 1
@echo on

:: Build NON-unit-test projects (debug builds)
cd %_ROOT%\projects      
%_TOOLS%\bob.py -v4 vc12 -cg --bldtime -b win32 --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Build unit test projects
cd %_ROOT%\tests
%_TOOLS%\bob.py -v4 vc12 --bldtime -c -b win32 --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Run unit tests
cd %_ROOT%\tests
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

call %_ROOT%\env.bat 5
@echo on

:: Build NON-unit-test projects
cd %_ROOT%\projects
%_TOOLS%\bob.py -v4 --p2 windows gcc-arm --bldtime -c --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Build unit test projects

cd %_ROOT%\tests
%_TOOLS%\bob.py -v4 --p2 windows gcc-arm --bldtime -c --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1


::
:: Skip additional project builds when NOT building develop or main
::
IF "%BUILD_BRANCH%"=="none" GOTO :builds_done

:: Zip up (NON-debug) 'release' builds
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

:: Zip up simulator builds
cd %_ROOT%\projects\GM6000\Ajax\simulator\windows\vc12\_win32
7z a ajax-simulator-%BUILD_NUMBER%.zip ajax-sim.exe ajax-sim.pdb
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

cd %_ROOT%\projects\GM6000\Eros\simulator\windows\vc12\_win32
7z a eros-simulator-%BUILD_NUMBER%.zip eros-sim.exe eros-sim.pdb
IF ERRORLEVEL 1 EXIT /b 1
copy *.zip %_ROOT%\_artifacts
IF ERRORLEVEL 1 EXIT /b 1

:: Build DEBUG version of the Target builds
cd %_ROOT%\projects
%_TOOLS%\bob.py -v4 --p2 windows gcc-arm --bldtime -cg --bld-all --bldnum %BUILD_NUMBER%
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

::
:: Everything worked!
::
:builds_done
echo:EVERTHING WORKED
exit /b 0
