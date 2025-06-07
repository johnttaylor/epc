#------------------------------------------------------------------------------
# TOOLCHAIN
#
#   Host:       Windows
#   Compiler:   Mingw_w64
#   Target:     Windows
#   Output:     Static Library
#------------------------------------------------------------------------------

import sys
from nqbplib import utils
from . import console_exe

class ToolChain( console_exe.ToolChain ): #base.ToolChain ):
    def __init__( self, exename, prjdir, build_variants, default_variant='release' ):
        console_exe.ToolChain.__init__( self, exename, prjdir, build_variants, default_variant )
 
    #--------------------------------------------------------------------------
    def link( self, arguments, builtlibs, local_external_setting, variant ):

        # Build list of libraries
        libs = []
        for item in builtlibs:
            libs.append( item[0] )

        # Generate ninja build statement
        opts = self._ar_options + 'T'
        self._ninja_writer.build( 
            outputs = self._final_output_name + ".a",
            rule = 'arlibs',
            inputs = libs,
            variables = {"aropts":opts, "arout":self._ar_out} )
        self._ninja_writer.newline()
        return None
 
    def finalize( self, arguments, builtlibs, objfiles, local_external_setting, linkout=None ):
        self._ninja_writer.default( self._final_output_name + ".a" )

    #--------------------------------------------------------------------------
    def _build_arlibs_rule( self ):
        self._win32_build_arlibs_rule()
