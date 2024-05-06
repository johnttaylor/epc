""" Test Suite: 

    Smoke/Sanity test for Ajax

    Pre-Conditions:  Ajax-sim must be running.

    Post-Conditions: Ajax-sim will be exited
"""

from rattlib import *
import config
import time

prompt_string = '$> '

def main():
    """ Entry point for the Test Suite
    """

    output.write_entry( __name__ )
    passcode = config.g_passed
    uut.setprompt( prompt_string )

    # Wait for the UUT to start
    r = uut.waitfor( 10, prompt_string )
    if ( r == None ):
        output.writeline("ERROR: The UUT is not responding")
        passcode = config.g_failed


    # Tests...
    passcode = std.run("tc_basic_heating", passcode )
    #passcode = std.run("tc_heating_alerts", passcode )  # TODO: Implement this test case

    # Cleanly Exit the UUT
    uut.cli("bye app")
    output.write_exit( __name__ )
    return passcode 