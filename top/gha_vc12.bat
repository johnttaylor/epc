@echo on
:: This script is used by the CI\Build machine to build using the Visual Studio 
:: compiler for the Windows projects
::
:: usage: gha_vc12.bat <buildNumber> [branch]
::

set _TOPDIR=%~dp0
set _ROOT=%_TOPDIR%..
echo:%_ROOT%
echo:%_TOPDIR%

:: Set the CI build flag
set NQBP_CI_BUILD=1

:: Make sure the _artifacts directory exists and is empty
cd %_ROOT%
rmdir /s /q _artifacts\projects
mkdir _artifacts\projects

:: Set the NQBP_BIN path (and other magic variables - but no compiler selected)
call ./env.bat

:: Set Build info (and force build number to zero for "non-official" builds)
set BUILD_NUMBER=%1
set BUILD_BRANCH=none
IF NOT "/%2"=="/" set BUILD_BRANCH=%2
IF "%BUILD_BRANCH%"=="none" set BUILD_NUMBER=0
echo:
echo:BUILD: BUILD_NUMBER=%BUILD_NUMBER%, BRANCH=%BUILD_BRANCH% 
echo:

::
:: Build Visual Studio projects
::

:: Build NON-unit-test projects (debug builds)
cd %_ROOT%\projects      
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --exclude catch2 --script-prefix python vc12 -cg --bldtime -b win32 --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Build the Catch2 static library
cd %_ROOT%\projects\xsrc\catch2
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python vc12 -c --bld-all


:: Build the unit tests
cd %_ROOT%\tests
echo:Building unit tests...
python %_ROOT%\xsrc\nqbp2\other\bob.py -v4 --script-prefix python vc12 -c --bldtime --bld-all --bldnum %BUILD_NUMBER%
IF ERRORLEVEL 1 EXIT /b 1

:: Run unit tests
echo:Running unit tests...
cd %_ROOT%\tests
python %_ROOT%\xsrc\nqbp2\other\chuck.py -vt --match a.exe --dir vc12
IF ERRORLEVEL 1 EXIT /b 1
python %_ROOT%\xsrc\nqbp2\other\chuck.py -v --match aa.exe --dir vc12
IF ERRORLEVEL 1 EXIT /b 1
python %_ROOT%\xsrc\nqbp2\other\chuck.py -vt --script-prefix python --match a.py --dir vc12
IF ERRORLEVEL 1 EXIT /b 1
python %_ROOT%\xsrc\nqbp2\other\chuck.py -v --script-prefix python --match aa.py --dir vc12
IF ERRORLEVEL 1 EXIT /b 1

::
:: Skip additional project builds when NOT building develop or main
::
IF "%BUILD_BRANCH%"=="none" GOTO :builds_done

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

::
:: Everything worked!
::
:builds_done
exit /b 0
