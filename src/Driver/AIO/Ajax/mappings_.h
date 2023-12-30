#ifdef Driver_Button_Hal_h_

#ifndef Driver_Button_Arduino_mappings_x_h_
#define Driver_Button_Arduino_mappings_x_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file

    This file provides the mappings for Platform specific types, etc. for the 
    Driver::Button HAL

 */

#include "Bsp/Api.h"


/** Type for a button handle
 */
struct DriverButtonPinHalArduino_T
{
    int      pin;           //!< Arduino Pin number
    uint32_t mode;          //!< Pin Mode: Valid values are: INPUT, INPUT_PULLUP, INPUT_PULLDOWN
    bool     activeLow;     //!< Set to true when the pressed state is ACTIVE_LOW; else set to false for ACTIVE_HIGH

    /// Constructor
    DriverButtonPinHalArduino_T( int        pinNum,
                                 uint32_t   pinMode,
                                 bool       isActiveLow )
        : pin( pinNum )
        , mode( pinMode )
        , activeLow( isActiveLow )
    {
    }

    /// Constructor
    DriverButtonPinHalArduino_T( const DriverButtonPinHalArduino_T& other )
        : pin( other.pin )
        , mode( other.mode )
        , activeLow( other.activeLow )
    {
    }
};


/// Arduino Mapping
#define Driver_Button_Pin_Hal_T_MAP                 DriverButtonPinHalArduino_T  


/// Arduino Mapping
#define Driver_Button_Hal_getRawPressedState_MAP    driverButtonHalArduino_getRawPressState


/// Arduino specific implementation for getting the raw button state
bool driverButtonHalArduino_getRawPressState( DriverButtonPinHalArduino_T pinHandle );


#endif  // end header latch
#endif  // end Interface latch