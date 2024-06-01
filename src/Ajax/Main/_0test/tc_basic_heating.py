""" Test Case: 

    Executes a simplified heating algorithm and sensor failure tests

    Pre-Conditions:  The Mocked UUT must be running and provisioned and user settings have been defaulted

    Post-Conditions: Heating off and Sensor failure
"""

from rattlib import *
import config
import time

def run():
    """ Entry point for the Test Case
    """

    output.write_entry(__name__)
    cpl = std.load( "cplutils" )
    passcode = config.g_passed

    # Allow time to get past the Splash screen
    time.sleep( 3 )

    # Verify No start-up errors
    if ( passcode == config.g_passed ):
        alerts = cpl.get_value( "alertSummary" )
        try:
            if alerts == None or alerts["count"] != 0:
                output.writeline(f"ERROR (alertSummary): At least one alert {alerts}")
                passcode = config.g_failed
        except Exception as e:
                output.writeline(f"ERROR (alertSummary): Not the expected Alert summary {alerts}. Except={e}")
                passcode = config.g_failed

    # Set up for heating
    if ( passcode == config.g_passed ):
        uut.cli( "ui btn-y" ) # enter edit setpoint
        time.sleep( 0.3 )
        for x in range(0,8):
            uut.cli( "ui btn-x" ) # change setpoint to 78
            time.sleep( 0.3 )
        uut.cli( "ui btn-b" ) # exit edit setpoint
        time.sleep( 0.3 )
        uut.cli( "ui btn-x" ) # set fan to HIGH
        time.sleep( 0.3 )
        uut.cli( "ui btn-a" ) # enable heating
        time.sleep( 0.3 )

    
    # Verify setting
    if ( passcode == config.g_passed ):
        if not cpl.read_and_compare( "heatingMode", True, silent=False ):
            passcode = config.g_failed
        if not cpl.read_and_check_state( "fanMode", "eHIGH", silent=False ):
            passcode = config.g_failed
        if not cpl.read_and_compare( "heatSetpoint", 7800 , silent=False):
            passcode = config.g_failed
        if not cpl.read_and_compare( "onBoardIdt", 7699, silent=False ):
            passcode = config.g_failed

    # Allow time for the heating algorithm to spin-up
    if ( passcode == config.g_passed ):
        time.sleep( 4 )
        if not cpl.read_and_compare( "cmdHeaterPWM", 1, compare='>', silent=False ):
            passcode = config.g_failed
        if not cpl.read_and_compare( "cmdFanPWM", 65536, silent=False ):
            passcode = config.g_failed


    # Trigger a IDT failure
    if ( passcode == config.g_passed ):
        cpl.write_numeric( "mockedADCBits", 0 );
        time.sleep( 3 )
        alerts = cpl.get_value( "alertSummary" )
        try:
            if alerts == None or alerts["count"] != 1:
                output.writeline(f"ERROR (alertSummary): Expecting Sensor failure {alerts}")
                passcode = config.g_failed
        except Exception as e:
                output.writeline(f"ERROR (alertSummary): Expecting Sensor failure {alerts}. Except={e}")
                passcode = config.g_failed
    
    # Turn heating off
    if ( passcode == config.g_passed ):
        uut.cli( "ui btn-a" ) # disable heating
        time.sleep( 3 )
        if not cpl.read_and_compare( "heatingMode", False, silent=False ):
            passcode = config.g_failed
        if not cpl.read_and_compare( "cmdHeaterPWM", 0, silent=False ):
            passcode = config.g_failed
        if not cpl.read_and_compare( "cmdFanPWM", 0, silent=False ):
            passcode = config.g_failed

    # end test case
    output.write_exit(__name__)
    return passcode

