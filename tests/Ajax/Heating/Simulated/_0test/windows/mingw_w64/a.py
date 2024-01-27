#!/usr/bin/python3
"""Launch's the RATT test script(s) for the unittest project"""

import os
import sys
import subprocess

###
def run_shell( cmd ):
    p = subprocess.Popen( cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE )
    r = p.communicate()
    r0 = '' if r[0] == None else r[0].decode()
    r1 = '' if r[1] == None else r[1].decode()
    return (p.returncode, "{} {}".format(r0,r1))

# MAIN
if __name__ == '__main__':
    
    # Additional ratt options
    options = ' '.join(sys.argv[1:])

    # Get paths to important stuff...
    pkg_root   = os.environ.get('NQBP_PKG_ROOT')
    testdir    = os.path.join( pkg_root,  "src", "Ajax", "Heating", "Simulated", "_0test" )
    ratt       = os.path.join( pkg_root, "xsrc", "ratt", "bin",  "ratt.py" )

    # Run the test suite(s)
    cmd = f"{ratt} {options} --log --input test_suite.py --path1 {testdir} --win _win32\\b.exe"
    print( cmd )
    result, testoutput = run_shell( cmd  )
    print( testoutput )
    sys.exit( result )
