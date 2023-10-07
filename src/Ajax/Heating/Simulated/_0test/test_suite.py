""" Test Suite: 

    Test basic Heating operation

    Pre-Conditions:  The Mocked UUT must be running.

    Post-Conditions: The Mocked UUT will be exited
"""

from rattlib import *
import config
import time

prompt_string = '$ '

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

    # Provision the unit
    if ( passcode == config.g_passed ):
        r = uut.cli( 'prov T-101 SN800 PasswordNotUsed  1000 1000  5 20  -20 -10 0 10 20  39322 52429 65536  1200000' )
        if "ERROR: [prov]" in r:
            output.writeline("ERROR: Failed provision the UUT")
            passcode = config.g_failed

    uut.cli( "dm read fanLowPercentage" )
    # Test Basic Heating
    if ( passcode == config.g_passed ):
         tc = std.load("tc_basic_heating")
         passcode = config.g_failed if tc == None else tc.run()

    # Test Heating Alerts
    if ( passcode == config.g_passed ):
         tc = std.load("tc_heating_alerts")
         passcode = config.g_failed if tc == None else tc.run()

    # Cleanly Exit the UUT
    uut.cli("bye app")

    output.write_exit( __name__ )
    return passcode 