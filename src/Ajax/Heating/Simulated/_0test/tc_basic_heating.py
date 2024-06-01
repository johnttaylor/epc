""" Test Case: 

    Exercises the heating algorithm and validates the FLC is reacting as expected

    Pre-Conditions:  The Mocked UUT must be running and provisioned

    Post-Conditions: Running, Heating off, Fan mode High, House simulated disabled
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
    uut.cli( 'dm write {name:"heatSetpoint",val:7100}' )  # 71'F
    uut.cli( 'dm write {name:"fanMode",val:"eHIGH"}' )
    uut.cli( 'tick @60000') # Advance to a known time

    # enable the house simulator
    uut.cli( 'house enable 45')
       
    # Advance 10 seconds       
    passcode = helper.advance_and_validate( passcode, 10000, "ON", 'eHIGH', 71, 0.52, 12, 100, "---", "---" )
    
    # Advance 4 minutes
    passcode = helper.advance_and_validate( passcode, 240000, "ON", 'eHIGH', 71, 0.37, 39, 100, "---", "---" )

    # Advance 2 minutes
    passcode = helper.advance_and_validate( passcode, 120000, "ON", 'eHIGH', 71, 0.0, 43, 100, "---", "---")

    # Advance 4 minutes
    passcode = helper.advance_and_validate( passcode, 240000, "ON", 'eHIGH', 71, -0.56, 26, 100, "---", "---")

    # Advance 8 minutes
    passcode = helper.advance_and_validate( passcode, 480000, "ON", 'eHIGH', 71, 0.47, 12, 100, "---", "---")

    # end test case
    uut.cli( 'dm write {name:"heatingMode",val:false}' )
    uut.cli( 'house disable')
    output.write_exit(__name__)
    return passcode

