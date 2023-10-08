""" Common/Helper routines for test cases
"""
from rattlib import *
import config

def advance_and_validate( passcode, numTicks, heatMode, fanMode, setpt, tempErr, heatPWM, fanPWM, sensorAlert, safetyAlert ):
    if ( passcode == config.g_passed ):
        t = uut.cli( f'tick +{numTicks}').strip() 
        r = uut.cli( 'state' )
        if ( r == None ):
            output.writeline( f"ERROR {t}: UUT timed out")
            passcode = config.g_failed
        
        # Validate expected state
        else:
            passcode = expected( r, t, heatMode, fanMode, setpt, tempErr, heatPWM, fanPWM, sensorAlert, safetyAlert )

    return passcode

def expected( r, t, heatMode, fanMode, setpt, tempErr, heatPWM, fanPWM, sensorAlert, safetyAlert ):
    r = r.replace("\n"," ")
    val = (float) (parse_field( r, "err="))
    if val != tempErr:
        output.writeline( f"ERROR {t}: Temp Error: actual: {val}, expected: {tempErr}" )
        return config.g_failed

    val = (int) (parse_field( r, "Setpoint:"))
    if val != setpt:
        output.writeline( f"ERROR {t}: Setpoint: actual: {val}, expected: {setpt}" )
        return config.g_failed

    val = (int) (parse_field( r, "heatPWM:", "%"))
    if val != heatPWM:
        output.writeline( f"ERROR {t}: HeatPWM: actual: {val}, expected: {heatPWM}" )
        return config.g_failed

    val = (int) (parse_field( r, "fanPWM:", "%"))
    if val != fanPWM:
        output.writeline( f"ERROR {t}: FanPWM: actual: {val}, expected: {fanPWM}" )
        return config.g_failed

    val = parse_field( r, "Heating:")
    if val != heatMode:
        output.writeline( f"ERROR {t}: Heating Mode: actual: {val}, expected: {heatMode}" )
        return config.g_failed

    val = parse_field( r, "Fan:")
    if val != fanMode:
        output.writeline( f"ERROR {t}: Fan Mode: actual: {val}, expected: {fanMode}" )
        return config.g_failed

    val = parse_field( r, "sensor:")
    if val != sensorAlert:
        output.writeline( f"ERROR {t}: Sensor Alert: actual: {val}, expected: {sensorAlert}" )
        return config.g_failed

    val = parse_field( r, "safety:")
    if val != safetyAlert:
        output.writeline( f"ERROR {t}: Safety Alert: actual: {val}, actual: {safetyAlert}" )
        return config.g_failed

    return config.g_passed

def parse_field( r, fieldmarker, delim=" " ):
    idx    = r.find(fieldmarker)
    tokens = r[idx+len(fieldmarker):].strip().split(delim)
    return tokens[0]
