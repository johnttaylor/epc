#!/bin/bash
set -e

# This script is used by the CI/Build machine to build the Linux test
# project on a Windows machine by running the WSL (Window Subsystem for Linux)
#
# The script ASSUMES that the working directory is the package root
#
# usage: wsl_build.sh <bldnum>
#
# Example: wsl_build.sh 32

# setup the environment
source ./env.sh default

# Build all test linux projects (only 64bit versions)
# TODO: Currently building debug versions -->For non-debug builds - warnings are generated when using gcc 12 -->FIXME!
pushd tests
$NQBP_BIN/other/bob.py -v4 linux  -gb posix64 --bldtime --bldnum $1

# Run unit tests (but only 64bit builds since WSL does not support 32bit executables)
$NQBP_BIN/other/chuck.py -vt --match a.out --dir _posix64
$NQBP_BIN/other/chuck.py -v --match aa.out --dir _posix64
$NQBP_BIN/other/chuck.py -vt --match a.py --dir _posix64
$NQBP_BIN/other/chuck.py -v --match aa.py --dir _posix64

# Build all "projects/" linux projects (only 64bit versions)
# TODO: See above issue with GCC 12 -->FIXME!
popd
pushd projects
$NQBP_BIN/other/bob.py -v4 linux  -gb posix64 --bldtime --bldnum $1

