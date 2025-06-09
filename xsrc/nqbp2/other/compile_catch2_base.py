#!/usr/bin/python3
#-------------------------------------------------------------------------------
# Helper script for building the Catch2 framework library

# Usage:
# preprocessing_base <a1> <a2> <a3> <a4> <a5> <a6> <a7> <a8> <prj1> [<ci>]
#
# where:
# <a1>: build|clean
# <a2>: verbose|terse
# <a3>: <workspace-dir>
# <a4>: <package-dir>
# <a5>: <project-dir>
# <a6>: <current-dir>
# <a7>: <build-variant>
# <a8>: <debug-type> // debug|release|clean
# <prj1>: <build-type> // e.g. windows\vc12, windows\mingw_w64, linux/gcc, etc.
#-----------------------------------------------------------------------------

#
import os, sys, shutil

# Skip when doing a CI build (i.e. relying on the 'build-all' script to build the Catch2 library)
NQBP_CI_BUILD = os.environ.get('NQBP_CI_BUILD')
if ( NQBP_CI_BUILD != None ):
    sys.exit(0)

print( "= Catch2 Library..." )
VERBOSE   = sys.argv[2]
CLEAN     = sys.argv[1]
PKG_ROOT  = sys.argv[4]
VARIANT   = sys.argv[7]
BUILDTYPE = sys.argv[9]
DBG       = '-g' if sys.argv[8] == "debug" else ""
if VERBOSE == 'verbose':
    print("Arguments:", sys.argv)

try:
    os.remove( "_delete_me_catch2.log" )
except:
    pass

# Build directory
blddir = os.path.join( PKG_ROOT, "projects", "xsrc", "catch2", "lib", BUILDTYPE )
if not os.path.exists( blddir ):
    sys.exit( "Unsupported/invalid Catch2 Library build project: {blddir}" )

# Clean command
ncmd = ""
if CLEAN == 'clean':
    ncmd = f'nqbp.py -z > _delete_me_catch2.log 2>&1'

# Build command
else:
    ncmd = f'nqbp.py -b {VARIANT} {DBG} > _delete_me_catch2.log 2>&1'

# Build the Catch2 library
original_cwd = os.getcwd() 
os.chdir( blddir)
catch2_root = os.getcwd()
if VERBOSE == 'verbose':
    print( f"Building Catch2 library: {ncmd}" )
rc = os.system( ncmd )
os.chdir( original_cwd )
sys.exit( rc )
