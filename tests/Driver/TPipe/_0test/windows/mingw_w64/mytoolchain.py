#---------------------------------------------------------------------------
# This python module is used to customize a supported toolchain for your 
# project specific settings.
#
# Notes:
#    - ONLY edit/add statements in the sections marked by BEGIN/END EDITS
#      markers.
#    - Maintain indentation level and use spaces (it's a python thing) 
#    - rvalues must be enclosed in quotes (single ' ' or double " ")
#    - The structure/class 'BuildValues' contains (at a minimum the
#      following data members.  Any member not specifically set defaults
#      to null/empty string
#            .inc 
#            .asminc
#            .cflags
#            .cppflags
#            .asmflags
#            .linkflags
#            .linklibs
#           
#---------------------------------------------------------------------------

# get definition of the Options structure
from nqbplib.base import BuildValues
from nqbplib.my_globals import NQBP_PKG_ROOT
from nqbplib.my_globals import NQBP_PRE_PROCESS_SCRIPT
from nqbplib.my_globals import NQBP_PRE_PROCESS_SCRIPT_ARGS
import os, copy


#===================================================
# BEGIN EDITS/CUSTOMIZATIONS
#---------------------------------------------------

# Set the name for the final output item
FINAL_OUTPUT_NAME = 'a.exe'

# Link unittest directory by object module so that Catch's self-registration mechanism 'works'
unit_test_objects = '_BUILT_DIR_.src/Driver/TPipe/_0test'

# Use Catch2 as a static library
catch2_inc  = f'-I{os.path.join( NQBP_PKG_ROOT(), "xsrc", "catch2", "src" )}'
catch2_lib  = f'{os.path.join( NQBP_PKG_ROOT(), "projects", "xsrc", "catch2", "lib", "windows", "mingw_w64", "_BUILD_VARIANT_DIR_", "catch2.a" )}'

NQBP_PRE_PROCESS_SCRIPT( 'preprocess.py' )
NQBP_PRE_PROCESS_SCRIPT_ARGS( r'windows\mingw_w64' )


#
# For build config/variant: "win32"
#

# Set project specific 'base' (i.e always used) options
base_win32           = BuildValues()        # Do NOT comment out this line
base_win32.cflags    = '-m32 -std=c++17 -Wall -Werror -x c++  -fprofile-arcs -ftest-coverage'
base_win32.inc       = catch2_inc
base_win32.linkflags = '-m32 -fprofile-arcs'
base_win32.linklibs  = f'-lgcov {catch2_lib}'
base_win32.firstobjs = unit_test_objects

# Set project specific 'optimized' options
optimized_win32           = BuildValues()    # Do NOT comment out this line
optimized_win32.cflags    = '-O3'

# Set project specific 'debug' options
debug_win32           = BuildValues()       # Do NOT comment out this line



#
# For build config/variant: "win64"
#

# Construct option structs
base_win64      = BuildValues()
optimized_win64 = BuildValues()
debug_win64     = BuildValues()

# Set 'base' options
base_win64.cflags     = '-m64 -std=c++17 -Wall -Werror -x c++ -fprofile-arcs -ftest-coverage'
base_win64.inc        = catch2_inc
base_win64.linkflags  = '-m64'
base_win64.firstobjs  = unit_test_objects
base_win64.linklibs   = f'-lgcov {catch2_lib}'

# Set 'Optimized' options
optimized_win64.cflags    = '-O3'

# Set 'debug' options


#
# For build config/variant: "cpp11"
#

# same options as win64 (but uses different libdirs entries)
base_cpp11      = copy.deepcopy(base_win64)
optimized_cpp11 = copy.deepcopy(optimized_win64)
debug_cpp11     = copy.deepcopy(debug_win64)

#-------------------------------------------------
# ONLY edit this section if you are ADDING options
# for build configurations/variants OTHER than the
# 'release' build
#-------------------------------------------------

win32_opts = { 'user_base':base_win32, 
               'user_optimized':optimized_win32, 
               'user_debug':debug_win32
             }
               
# Add new dictionary of for new build configuration options
win64_opts = { 'user_base':base_win64,
               'user_optimized':optimized_win64,
               'user_debug':debug_win64
             }
               
# Add new dictionary of for new build configuration options
cpp11_opts = { 'user_base':base_win64,
               'user_optimized':optimized_win64,
               'user_debug':debug_win64
             }
        
# Add new variant option dictionary to # dictionary of 
# build variants
build_variants = { 'win32':win32_opts,
                   'win64':win64_opts,
                   'cpp11':cpp11_opts,
                 }    


#---------------------------------------------------
# END EDITS/CUSTOMIZATIONS
#===================================================



# Capture project/build directory
import os
prjdir = os.path.dirname(os.path.abspath(__file__))


# Select Module that contains the desired toolchain
from nqbplib.toolchains.windows.mingw_w64.console_exe import ToolChain


# Function that instantiates an instance of the toolchain
def create():
    tc = ToolChain( FINAL_OUTPUT_NAME, prjdir, build_variants, "win32" )
    return tc 
