#!/usr/bin/env python3
"""
This script runs 'doxygen' (to generate HTML documentation of the package's
header files) on the package and checks for warnings.  Colony's coding 
standards require that doxygen execute without warning.

The script assumes that doxygen is in the command path.


Usage: doxygen [build-number] [build-branch] [nochm]

"""

import subprocess
import re
import shutil
import os
import sys

#------------------------------------------------------------------------------
def filter_warnings( output ):
    at_least_one = False
    lines = output.splitlines()
    for line in lines:
        # Filter blank lines
        line = line.strip()
        if ( len(line) == 0 ):
            continue

        # Filter
        if ( re.search( r"^.*error.*inline_dotgraph_.*\.dot", line ) ):
            continue

        # Filter
        if ( re.search( r"^.*Warning: node .*size too small for label$", line ) ):
            continue

        # Filter
        if ( re.search( r"^.*src/Cpl/Type/enum.h:.*warning:.*", line ) ):
            continue

        # Filter
        if ( re.search( r"^.*src/.*warning: argument .* of command @param is not found in the argument list of .*BETTER_ENUM.*", line ) ):
            continue

        # Filter
        if ( re.search( r"^.*warning: ignoring unsupported tag.*", line ) ):
            continue

        # Filter
        #if ( re.search( r"src/Cpl/Text/Frame/LineDecoder.h:.*warning: Found unknown command.*\\r", line ) ):
        #    continue
 
        # Filter
        #if ( re.search( 'src/Cpl/TShell/Dac/Cmd/Command.h:.*warning: Unsupported xml/html tag <esc> found', line ) ):
        #    continue
            
        # Passed ALL filters
        print( line )
        at_least_one = True

    # Display the results of the filtering
    if ( at_least_one == False ):
        print( "    All warnings are known warnings -->so you are good!")
        print()
    else:
        print()
        exit(1)
        
#------------------------------------------------------------------------------
print( "Running doxygen..." )    

# File name MUST match the base file name that is set for CHM_FILE in the 'Doxyfile' file
filename = "sdx-1330-gm6000-software-doxygen-output"

# Insert the build number into the Doxygen config
build_num    = '0'
build_branch = 'none'
chmfile      = "YES"
if ( len(sys.argv) > 3 and sys.argv[3] == 'nochm' ):
    chmfile = "NO"
if len(sys.argv) > 1:
    build_num = sys.argv[1]
if len(sys.argv) > 2:
    build_branch = sys.argv[2]
with open( "Doxyfile.src", 'r' ) as inf:
    d = inf.read()
    d = d.replace('$$$BUILD_NUMBER$$$', build_num )
    d = d.replace('$$$BUILD_BRANCH$$$', build_branch )
    d = d.replace('$$$GENERATE_HELP_FILE$$$',chmfile)
    with open( "Doxyfile", 'w' ) as outf:
        outf.write( d )

# run doxygen
cmd = "doxygen"
p   = subprocess.Popen( cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE )
r   = p.communicate()
if ( p.returncode != 0 ):
    exit( "ERROR: Doxygen failed to run.  Check if doxygen.exe is in your command path" )

# delete the HTML files - only keep the Windows Help (.chm) file
if ( chmfile == "YES" ):
    path = os.path.join( '..', 'docs', 'html' )
    shutil.rmtree( path, ignore_errors=True  )

# check for errors
if ( "warning: " in r[1].decode() or "error: " in r[1].decode() ):
    print()
    print( "*** Doxygen had one or more warnings! ***" )
    filter_warnings( r[1].decode() )
else:
    print( "Completed without warnings or errors." )

# Rename help file output to include the build info
if ( chmfile == "YES" ):
    outfile = os.path.join( '..', 'docs', f'{filename}.chm' )
    newfile = os.path.join( '..', 'docs', f'{filename}_{build_num}-{build_branch}.chm' )
    try:
        os.remove( newfile )
    except:
        pass
    try:
        shutil.move( outfile, newfile )
    except:
        pass


