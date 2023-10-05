""" Test Case: 

    TBD

    Pre-Conditions:  The Mocked UUT must be running and provisioned

    Post-Conditions: Running, Heating off, Fan mode High, House simulated disabled
"""

from rattlib import *
import config

def run():
    """ Entry point for the Test Case
    """

    output.write_entry(__name__)

    passcode = config.g_passed

    # Set up for heating
    uut.cli( 'dm write {name:"heatingMode",val:true}' )
    uut.cli( 'dm write {name:"heatSetpoint",val:7100}' )  # 71'F
    uut.cli( 'dm write {name:"fanMode",val:"eHIGH"}' )
    uut.cli( 'house enable 45')
       
    # Advance 20 seconds       
    if ( passcode == config.g_passed ):
        t = uut.cli( 'tick +20000').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline(f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, "Heating: ON", "Fan: eHIGH", "heatPWM:  32%  fanPWM: 100%", "Setpoint:    71             err=0.53" )

    # Advance 2 minutes
    if ( passcode == config.g_passed ):
        t = uut.cli( 'tick +120000').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, "Heating: ON", "Fan: eHIGH", "heatPWM:  48%  fanPWM: 100%", "Setpoint:    71             err=-0.48" )

    # Advance 2 minutes
    if ( passcode == config.g_passed ):
        t = uut.cli( 'tick +120000').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, "Heating: ON", "Fan: eHIGH", "heatPWM:   3%  fanPWM: 100%", "Setpoint:    71             err=0.08" )

    uut.cli( 'dm write {name:"heatingMode",val:false}' )
    uut.cli( 'house disable')
    output.write_exit(__name__)
    return passcode


def expected( r, t, expectedHeatMode, expectedFanMode, expectedPWM, expectedSetpt ):
    if not expectedHeatMode in r:
        output.writeline( f"ERROR {t}: Not in heating mode" )
        return config.g_failed
    if not expectedFanMode in r:
        output.writeline( f"ERROR {t}: Fan mode is NOT the expected eHIGH" )
        return config.g_failed
    if not expectedPWM in r:
        output.writeline( f"ERROR {t}: Heater/Fan PWM is NOT the expected values" )
        return config.g_failed
    if not expectedSetpt in r:
        output.writeline( f"ERROR {t}: IDT/delta-err is NOT the expected values" )
        return config.g_failed

    return config.g_passed