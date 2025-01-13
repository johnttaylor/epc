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
FINAL_OUTPUT_NAME = 'a.out'

# Link unittest directory by object module so that Catch's self-registration mechanism 'works'
unit_test_objects = '_BUILT_DIR_.src/Cpl/System/_0test _BUILT_DIR_.src/Cpl/System/_0test/_baremetal'

# Use Catch2 as a static library
catch2_inc  = f'-I{os.path.join( NQBP_PKG_ROOT(), "xsrc", "catch2", "src" )}'
catch2_lib  = f'{os.path.join( NQBP_PKG_ROOT(), "projects", "xsrc", "catch2", "lib", "linux", "gcc", "_BUILD_VARIANT_DIR_", "catch2.a" )}'

NQBP_PRE_PROCESS_SCRIPT( 'preprocess.py' )
NQBP_PRE_PROCESS_SCRIPT_ARGS( 'linux/gcc' )



#
# For build config/variant: "Release" (aka posix build variant)
#

# Set project specific 'base' (i.e always used) options
base_posix32           = BuildValues()        # Do NOT comment out this line
base_posix32.cflags    = '-m32 -std=c++17 -Wall -Werror -x c++ -fprofile-arcs -ftest-coverage'
base_posix32.inc       = catch2_inc
base_posix32.linkflags = '-m32 -fprofile-arcs'
base_posix32.linklibs  = f'-lgcov {catch2_lib}'
base_posix32.firstobjs = unit_test_objects


# Set project specific 'optimized' options
optimized_posix32           = BuildValues()    # Do NOT comment out this line
optimized_posix32.cflags    = '-O3'

# Set project specific 'debug' options
debug_posix32           = BuildValues()       # Do NOT comment out this line



# 
# For build config/variant: "posix64"
#

# Construct option structs
base_posix64      = BuildValues()
optimized_posix64 = BuildValues()
debug_posix64     = BuildValues()

# Set project specific 'base' (i.e always used) options
base_posix64.cflags    = '-m64 -std=c++17 -Wall -Werror -x c++ -fprofile-arcs -ftest-coverage'
base_posix64.inc       = catch2_inc
base_posix64.linkflags = '-fprofile-arcs'
base_posix64.linklibs  = f'-lgcov {catch2_lib}'
base_posix64.firstobjs = unit_test_objects

# Set project specific 'optimized' options
optimized_posix64.cflags    = '-O3'

# Set project specific 'debug' options

#
# For build config/variant: "cpp11"
#

# same options as posix64 (but uses different libdirs entries)
base_cpp11      = copy.deepcopy(base_posix64)
optimized_cpp11 = copy.deepcopy(optimized_posix64)
debug_cpp11     = copy.deepcopy(debug_posix64)

#-------------------------------------------------
# ONLY edit this section if you are ADDING options
# for build configurations/variants OTHER than the
# 'release' build
#-------------------------------------------------

release_opts = { 'user_base':base_posix32, 
                 'user_optimized':optimized_posix32, 
                 'user_debug':debug_posix32
               }
               
               
# Add new dictionary of for new build configuration options
cpp11_opts = { 'user_base':base_cpp11, 
               'user_optimized':optimized_cpp11, 
               'user_debug':debug_cpp11
             }
  
posix64_opts = { 'user_base':base_posix64, 
                 'user_optimized':optimized_posix64, 
                 'user_debug':debug_posix64
               }
  
        
# Add new variant option dictionary to # dictionary of 
# build variants
build_variants = { 'posix':release_opts,
                   'posix64':posix64_opts,
                   'cpp11':cpp11_opts,
                 }    

#---------------------------------------------------
# END EDITS/CUSTOMIZATIONS
#===================================================



# Capture project/build directory
import os
prjdir = os.path.dirname(os.path.abspath(__file__))


# Select Module that contains the desired toolchain
from nqbplib.toolchains.linux.gcc.console_exe import ToolChain


# Function that instantiates an instance of the toolchain
def create():
    tc = ToolChain( FINAL_OUTPUT_NAME, prjdir, build_variants, "posix64" )
    return tc 
