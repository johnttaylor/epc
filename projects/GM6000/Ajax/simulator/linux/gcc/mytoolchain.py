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
from nqbplib.my_globals import NQBP_WORK_ROOT

#===================================================
# BEGIN EDITS/CUSTOMIZATIONS
#---------------------------------------------------

# Set the name for the final output item
FINAL_OUTPUT_NAME = 'ajax-sim'

#
# For build config/variant: "Release" (aka posix build variant)
#

# 
# For build config/variant: "posix64" (same as release, except 64bit target)
# (note: uses same internal toolchain options as the 'Release' variant, 
#        only the 'User' options will/are different)
#

# Construct option structs
base_posix64     = BuildValues()
optimzed_posix64 = BuildValues()
debug_posix64    = BuildValues()

# Set project specific 'base' (i.e always used) options
base_posix64.cflags    = '-m64 -std=c++11 -Wall -Werror -x c++'
base_posix64.linkflags = '-m64'
base_posix64.linklibs  = '-lpthread -lm'

# Set project specific 'optimized' options
optimzed_posix64.cflags    = '-O3'
optimzed_posix64.linklibs  = '-lstdc++'

# Set project specific 'debug' options
debug_posix64.linklibs  = '-lstdc++'


#-------------------------------------------------
# ONLY edit this section if you are ADDING options
# for build configurations/variants OTHER than the
# 'release' build
#-------------------------------------------------

posix64_opts = { 'user_base':base_posix64, 
                 'user_optimized':optimzed_posix64, 
                 'user_debug':debug_posix64
               }
  
        
# Add new variant option dictionary to # dictionary of 
# build variants
build_variants = { 'posix64':posix64_opts
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
