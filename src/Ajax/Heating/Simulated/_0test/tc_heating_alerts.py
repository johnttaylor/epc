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
    helper = std.load( "tc_common" )

    passcode = config.g_passed

    # Set up for heating
    uut.cli( 'dm write {name:"heatingMode",val:true}' )
    uut.cli( 'dm write {name:"heatSetpoint",val:7100}' )
    uut.cli( 'dm write {name:"fanMode",val:"eMEDIUM"}' )
    uut.cli( 'tick @60000') # Advance to a known time

    # enable the house simulator
    uut.cli( 'house enable 45')
       
    # Advance 10 seconds       
    passcode = helper.advance_and_validate( passcode, 10000, "ON", 'eMEDIUM', 71, 0.49, 13, 80, "---", "---" )
    
    # Advance 2 minutes
    passcode = helper.advance_and_validate( passcode, 240000, "ON", 'eMEDIUM', 71, 0.5, 41, 80, "---", "---" )

    # Trigger Safety limit
    #uut.cli( 'trace section on *Ajax::Heating' )
    uut.cli( 'trace' )
    uut.cli( 'dm write {name:"hwSafetyLimit",val:true}' )
    passcode = helper.advance_and_validate( passcode, 1000, "ON", 'eMEDIUM', 71, 0.48, 0, 80, "---", "RAISED" )

    # Advance 1 minute
    passcode = helper.advance_and_validate( passcode, 60000, "ON", 'eMEDIUM', 71, -0.07, 0, 80, "---", "RAISED" )

    # Remove the safety limit trip
    uut.cli( 'dm write {name:"hwSafetyLimit",val:false}' )
    passcode = helper.advance_and_validate( passcode, 1000, "ON", 'eMEDIUM', 71, -0.07, 0, 80, "---", "---" )
    #uut.cli( 'trace section off *Ajax::Heating' )

    # Advance 20 seconds       
    passcode = helper.advance_and_validate( passcode, 20000, "ON", 'eMEDIUM', 71, -0.09, 41, 80, "---", "---" )

    # No sensor
    uut.cli( 'dm write {name:"onBoardIdt",valid:false,locked:true}' )
    uut.cli( 'dm write {name:"remoteIdt",valid:false,locked:true}' )
    passcode = helper.advance_and_validate( passcode, 1000, "ON", 'eMEDIUM', 71, 71, 0, 0, "RAISED", "---" )

    # Advance 20 seconds       
    passcode = helper.advance_and_validate( passcode, 20000, "ON", 'eMEDIUM', 71, 71, 0, 0, "RAISED", "---" )

    # Restore the sensor
    #uut.cli( 'trace section on *Ajax::Heating' )
    uut.cli( 'dm write {name:"onBoardIdt",locked:false}' )
    passcode = helper.advance_and_validate( passcode, 8000, "ON", 'eMEDIUM', 71, 0.1, 0, 80, "---", "---" )
    #uut.cli( 'dm read cmdHeaterPWM' )
    #uut.cli( 'dm read cmdFanPWM' )

    # Advance 2 minute
    passcode = helper.advance_and_validate( passcode, 120000, "ON", 'eMEDIUM', 71, 0.48, 7, 80, "---", "---" )


