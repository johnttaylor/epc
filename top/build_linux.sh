#!/bin/bash
set -e

# This script is used by the CI/Build machine to build the Linux projects
#
# The script ASSUMES that the working directory is the package root
#
# usage: build_linux.sh <bldnum>
#

# setup the environment
source ./env.sh default

# Build all test linux projects (only 64bit versions)
pushd tests
$NQBP_BIN/other/bob.py -v4 linux  -b posix64 --bldtime --bldnum $1

# Run unit tests 
$NQBP_BIN/other/chuck.py -v --match a.out --dir _posix64
$NQBP_BIN/other/chuck.py -v --match aa.out --dir _posix64
$NQBP_BIN/other/chuck.py -v --match a.py --dir _posix64
$NQBP_BIN/other/chuck.py -v --match aa.py --dir _posix64

# Build all "projects/" linux projects (only 64bit versions)
# TODO: See above issue with GCC 12 -->FIXME!
popd
pushd projects
$NQBP_BIN/other/bob.py -v4 linux  -b posix64 --bldtime --bldnum $1

