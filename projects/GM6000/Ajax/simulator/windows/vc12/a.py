#!/usr/bin/python3
"""Launch's the RATT test script(s) for the smoke/sanity test of Ajax"""

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
    
    # Remove persistant storage
    try:
        os.remove( "eeprom.bin" )
    except:
        pass

    # Additional ratt options
    options = ' '.join(sys.argv[1:])

    # Get paths to important stuff...
    pkg_root   = os.environ.get('NQBP_PKG_ROOT')
    testdir    = os.path.join( pkg_root,  "src", "Ajax", "Main", "_0test" )
    ratt       = os.path.join( pkg_root, "xsrc", "ratt", "bin",  "ratt.py" )
    utils      = os.path.join( pkg_root, "scripts", "colony.core", "ratt" )

    # Run the test suite(s)
    cmd = f"{ratt} {options} --log --input provision_suite.py --path1 {testdir} --path2 {utils} --win _win32\\ajax-sim.exe"
    print( cmd )
    result, testoutput = run_shell( cmd  )
    print( testoutput )
    cmd = f"{ratt} {options} --log --input test_suite.py --path1 {testdir} --path2 {utils} --win _win32\\ajax-sim.exe"
    print( cmd )
    result, testoutput = run_shell( cmd  )
    print( testoutput )


    sys.exit( result )
