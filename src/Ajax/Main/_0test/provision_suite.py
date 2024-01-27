""" Test Suite: 

    Provisions the Unit

    Pre-Conditions:  The EEPROM.bin file has been deleted and Ajax-sim must be running.

    Post-Conditions: Ajax-sim will be exited and a new EEPROM.bin file has been created
"""

from rattlib import *
import config
import time

prompt_string = '$> '

def main():
    """ Entry point for the Test Suite
    """

    output.write_entry( __name__ )
    cpl = std.load( "cplutils" )
    passcode = config.g_passed
    uut.setprompt( prompt_string )

    # Wait for the UUT to start
    r = uut.waitfor( 10, prompt_string )
    if ( r == None ):
        output.writeline("ERROR: The UUT is not responding")
        passcode = config.g_failed

    # Allow time to get past the Splash screen
    time.sleep( 2 )

    # Verify POST failure
    if ( passcode == config.g_passed ):
        alerts = cpl.get_value( "alertSummary" )
        try:
            if alerts == None or alerts["count"] != 1:
                output.writeline(f"ERROR (alertSummary): Not the expected Alert summary {alerts}")
                passcode = config.g_failed
        except Exception as e:
                output.writeline(f"ERROR (alertSummary): Not the expected Alert summary {alerts}. Except={e}")
                passcode = config.g_failed

    # Provision the unit
    if ( passcode == config.g_passed ):
        r = uut.cli( 'prov T-101 SN800 PasswordNotUsed  1000 1000  5 20  -20 -10 0 10 20  39322 52429 65536  1200000' )
        if "ERROR: [prov]" in r:
            output.writeline("ERROR: Failed provision the UUT")
            passcode = config.g_failed

    # Cleanly Exit the UUT
    uut.cli("bye app")
    output.write_exit( __name__ )
    return passcode 