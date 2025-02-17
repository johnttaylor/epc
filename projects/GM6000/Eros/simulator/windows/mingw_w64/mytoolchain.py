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
import os

#===================================================
# BEGIN EDITS/CUSTOMIZATIONS
#---------------------------------------------------

# Set the name for the final output item
FINAL_OUTPUT_NAME = 'eros-sim.exe'

# Additional Header paths for PIMORONI supplied code
pimoroni_src_path = os.path.join( NQBP_PKG_ROOT(), 'xsrc', 'pimoroni' )
pimoroni_inc      = f' -I{pimoroni_src_path}' + \
                    f' -I{os.path.join(pimoroni_src_path,"common")}' +\
                    f' -I{os.path.join(pimoroni_src_path,"libraries","pico_display")}' +\
                    f' -I{os.path.join(pimoroni_src_path,"libraries","pico_graphics")}' +\
                    f' -I{os.path.join(pimoroni_src_path,"libraries","pico_bitmap_fonts")}' +\
                    f' -I{os.path.join(pimoroni_src_path,"libraries","pico_hershey_fonts")}' 

#
# For build config/variant: "Release"
#

# Set project specific 'base' (i.e always used) options
base_release           = BuildValues()        # Do NOT comment out this line
base_release.cflags    = f'-Wno-attributes -m32 -D__unused= -D__always_inline=__attribute__((always_inline)) -Wall -Werror -x c++'
base_release.cppflags  = f'-std=gnu++17'
base_release.inc       = f'{pimoroni_inc}'
base_release.linkflags = '-m32 -fprofile-arcs'
base_release.linklibs  = '-lws2_32 -lstdc++ -lAdvapi32'

# Set project specific 'optimized' options
optimzed_release           = BuildValues()    # Do NOT comment out this line
optimzed_release.cflags    = '-O3'

# Set project specific 'debug' options
debug_release           = BuildValues()       # Do NOT comment out this line


#-------------------------------------------------
# ONLY edit this section if you are ADDING options
# for build configurations/variants OTHER than the
# 'release' build
#-------------------------------------------------

release_opts = { 'user_base':base_release, 
                 'user_optimized':optimzed_release, 
                 'user_debug':debug_release
               }
               
        
# Add new variant option dictionary to # dictionary of 
# build variants
build_variants = { 'win32':release_opts
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
