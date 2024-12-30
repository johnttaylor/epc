#!/usr/bin/python3

import os
import sys

# MAIN
if __name__ == '__main__':
    # Run the "real" script
    script = os.path.join(sys.argv[4], "xsrc", "nqbp2", "other", "compile_catch2_base.py" )
    cmd = script + " " + " ".join(sys.argv[1:])
    rc = os.system( cmd )
    sys.exit( rc )
