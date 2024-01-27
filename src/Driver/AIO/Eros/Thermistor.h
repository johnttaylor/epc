#ifndef Driver_AIO_Eros_Thermisttor_h_
#define Driver_AIO_Eros_Thermisttor_h_
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
/** @file 
*/

#include "Driver/AIO/Ajax/Thermistor.h"


/// namespace
namespace Driver
{
/// namespace
namespace AIO
{
/// namespace
namespace Eros
{

/** This class is an Eros specific extension of the Driver::AIO::Ajax::Thermistor
    driver.  The only difference is that the Eros driver 'published' the ADC
    Bit value via model points.
 */
class Thermistor : public Driver::AIO::Ajax::Thermistor
{
public:
    /// Index of the ADC bit value
    static constexpr unsigned IDX_ADC_BITS = 0;

    /// Index of the Temperature value
    static constexpr unsigned IDX_TEMPERATURE = 1;

public:
    /// Constructor
    Thermistor( Cpl::Dm::MailboxServer&    myMbox,
                DriverAIOHalSingleInput_T  adcDriver,
                Cpl::Dm::Mp::Int32&        mpTemperatureOutput );


protected:
    /// Set Driver::AIO::Ajax::Thermistor
    void hookAdcSample( uint32_t adcBits, int32_t temperatureF = 0 );

};

}           // End namespace(s)
}
}
#endif      // end header-latch
