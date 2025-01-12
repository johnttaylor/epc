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
import os, copy


#===================================================
# BEGIN EDITS/CUSTOMIZATIONS
#---------------------------------------------------

# Set the name for the final output item
FINAL_OUTPUT_NAME = 'catch2'

catch2_inc  = f'-I{os.path.join( NQBP_PKG_ROOT(), "xsrc", "catch2", "src" )}'


#
# For build config/variant: "win32" 
#

# Set project specific 'base' (i.e always used) options
base_win32           = BuildValues()        # Do NOT comment out this line
base_win32.cflags    = '/W3 /WX /EHsc '
base_win32.inc       = catch2_inc


# Set project specific 'optimized' options
optimized_win32          = BuildValues()    # Do NOT comment out this line
optimized_win32.cflags   = '/O2'

# Set project specific 'debug' options
debug_win32          = BuildValues()       # Do NOT comment out this line
debug_win32.cflags   = '/D "_MY_APP_DEBUG_SWITCH_"'


#
# For build config/variant: "cpp11"
#

# same options as win32 (but uses different libdirs entries)
base_cpp11      = copy.deepcopy(base_win32)
optimized_cpp11 = copy.deepcopy(optimized_win32)
debug_cpp11     = copy.deepcopy(debug_win32)

#-------------------------------------------------
# ONLY edit this section if you are ADDING options
# for build configurations/variants OTHER than the
# 'release' build
#-------------------------------------------------

win32_opts = { 'user_base':base_win32, 
               'user_optimized':optimized_win32, 
               'user_debug':debug_win32
             }
               
               
cpp11_opts = { 'user_base':base_cpp11,
               'user_optimized':optimized_cpp11,
               'user_debug':debug_cpp11
             }
        
# build variants
build_variants = { 'win32':win32_opts,
                   'cpp11':cpp11_opts
                 }    

#---------------------------------------------------
# END EDITS/CUSTOMIZATIONS
#===================================================



# Capture project/build directory
import os
prjdir = os.path.dirname(os.path.abspath(__file__))


# Select Module that contains the desired toolchain
from nqbplib.toolchains.windows.vc12.static_lib import ToolChain


# Function that instantiates an instance of the toolchain
def create():
    tc = ToolChain( FINAL_OUTPUT_NAME, prjdir, build_variants, 'win32' )
    return tc 
