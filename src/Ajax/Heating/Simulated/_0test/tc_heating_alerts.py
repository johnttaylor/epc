""" Test Case: 

    TBD

    Pre-Conditions:  The Mocked UUT must be running and provisioned

    Post-Conditions: Running in Heating Mode, Fan mode: High, House simulated disabled
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
    uut.cli( 'dm write {name:"heatSetpoint",val:7100}' )
    uut.cli( 'dm write {name:"fanMode",val:"eMEDIUM"}' )
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
            passcode = expected( r, t, "Heating: ON", "Fan: eMEDIUM", "heatPWM:  10%  fanPWM:  80%", "Setpoint:    71             err=0.14" )

    # Advance 2 minutes
    if ( passcode == config.g_passed ):
        t = uut.cli( 'tick +120000').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, "Heating: ON", "Fan: eMEDIUM", "heatPWM:  55%  fanPWM:  80%", "Setpoint:    71             err=-0.02" )

    # Trigger Safety limit
    if ( passcode == config.g_passed ):
        uut.cli( 'dm write {name:"hwSafetyLimit",val:true}' )
        t = uut.cli( 'tick +1000').strip() # Advance 1 second
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expectedAlerts( r, t, "safety: RAISED  ack=no",  "sensor: ---" )

    # Remove the safety limit trip
    if ( passcode == config.g_passed ):
        uut.cli( 'dm write {name:"hwSafetyLimit",val:false}' )
        t = uut.cli( 'tick +1000').strip() # Advance 1 second
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expectedAlerts( r, t, "safety: ---",  "sensor: ---" )

    # Advance 20 seconds       
    if ( passcode == config.g_passed ):
        t = uut.cli( 'tick +20000').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline(f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, "Heating: ON", "Fan: eMEDIUM", "heatPWM:   2%  fanPWM:  80%", "Setpoint:    71             err=0.08" )

    # No sensor
    if ( passcode == config.g_passed ):
        uut.cli( 'dm write {name:"onBoardIdt",valid:false,locked:true}' )
        uut.cli( 'dm write {name:"remoteIdt",valid:false,locked:true}' )
        t = uut.cli( 'tick +4000').strip() # Advance 4 seconds
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expectedAlerts( r, t, "safety: ---",  "sensor: RAISED  ack=no" )

    # Restore the sensor
    if ( passcode == config.g_passed ):
        uut.cli( 'dm write {name:"onBoardIdt",locked:false}' )
        t = uut.cli( 'tick +4000').strip() # Advance 4 seconds
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expectedAlerts( r, t, "safety: ---",  "sensor: ---" )

    # Advance 20 seconds       
    if ( passcode == config.g_passed ):
        t = uut.cli( 'tick +20000').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline(f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, "Heating: ON", "Fan: eMEDIUM", "heatPWM:  10%  fanPWM:  80%", "Setpoint:    71             err=0.19" )

    uut.cli( 'dm write {name:"heatingMode",val:false}' )
    uut.cli( 'house disable')
    output.write_exit(__name__)
    return passcode


def expected( r, t, expectedHeatMode, expectedFanMode, expectedPWM, expectedSetpt ):
    if not expectedHeatMode in r:
        output.writeline( f"ERROR {t}: Heating mode is NOT the expected value" )
        return config.g_failed
    if not expectedFanMode in r:
        output.writeline( f"ERROR {t}: Fan mode is NOT the expected value" )
        return config.g_failed
    if not expectedPWM in r:
        output.writeline( f"ERROR {t}: Heater/Fan PWM are NOT the expected values" )
        return config.g_failed
    if not expectedSetpt in r:
        output.writeline( f"ERROR {t}: IDT/delta-err are NOT the expected values" )
        return config.g_failed

    return config.g_passed

def expectedAlerts( r, t, expectedSafety, expectedSensor ):
    if not expectedSafety in r:
        output.writeline( f"ERROR {t}: Not the expected Safety Alert value" )
        return config.g_failed
    if not expectedSensor in r:
        output.writeline( f"ERROR {t}: Not the expected Sensor Alert value" )
        return config.g_failed

    return config.g_passed

