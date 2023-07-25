#ifndef Driver_DIO_Pwm_h_
#define Driver_DIO_Pwm_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2023  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "colony_map.h"

/// Defer the definition of the PWM configuration to the application's 'platform'
#define DriverDioPwmConfig_T          DriverDioPwmConfig_T_MAP


///
namespace Driver {
///
namespace DIO {

/** This class defines a generic interface for controlling a simple PWM 
    output signal.  
 */
class Pwm
{
public:
    /** Constructor Note: the 'pinConfig' struct MUST stay in scope as long
        as the driver is in scope.
     */
    Pwm( const DriverDioPwmConfig_T& pinConfig );

public:
    /** Starts the driver.
        Returns false if an error was encountered 
     */
    bool start( size_t initialDutyCycle );

    /** Stops the driver, places the output into a 'safe' state.  The safe state
        is defined by the platform
     */
    void stop();

public:
    /** Returns the value that generates 100% duty cycle.  This method can be
        called at any time (e.g. before the driver is started)
     */
    size_t getMaxDutyCycle() const;

    /** Sets/updates the duty cycle.  A value of 0 is 0% duty cycle. A
        value of getMaxDutyCycle() is 100% duty cycle 
     */
    void setDutyCycle( size_t dutyCycle );

protected:
    /// PWM info
    const DriverDioPwmConfig_T& m_pwm;

    /// Started flag
    bool                            m_started;
};

} // End namespace(s)
}

/*--------------------------------------------------------------------------*/
#endif  // end header latch
