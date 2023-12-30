#ifndef Driver_AIO_Ajax_Thermisttor_h_
#define Driver_AIO_Ajax_Thermisttor_h_
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

#include "colony_config.h"
#include "Driver/AIO/HalSingleInput.h"
#include "Cpl/Itc/CloseSync.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/System/Timer.h"
#include "Cpl/Dm/Mp/Int32.h"

/*** Sampling time, in milliseconds.
 */
#ifndef OPTION_DRIVER_AIO_AJAX_SAMPLE_TIME_MS
#define OPTION_DRIVER_AIO_AJAX_SAMPLE_TIME_MS           1000 // 1 HZ
#endif

/** Number of consecutive sample to take when averaging raw readings
 */
#ifndef OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG
#define OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG       3
#endif

/** Number of ADC Bits/resolution required
 */
#ifndef OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION
#define OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION       12
#endif

/** Threshold for max valid ADC reading (i.e. treated as a 'short')
 */
#ifndef OPTION_DRIVER_AIO_AJAX_ADC_MAX_RAIL_BITS
#define OPTION_DRIVER_AIO_AJAX_ADC_MAX_RAIL_BITS        (4096-100)
#endif

 /** Threshold for min valid ADC reading (i.e. treated as an 'open')
  */
#ifndef OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS
#define OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS        100
#endif


/// namespace
namespace Driver
{
/// namespace
namespace AIO
{
/// namespace
namespace Ajax
{

/** This class is responsible for sampling the on-board temperature sensor
    every N milliseconds and populating a model point.  The raw input is
    averaged and converted to hundreds of degree's Fahrenheit.
 */
class Thermistor : public Cpl::Itc::CloseSync, public Cpl::System::Timer
{
public:
    /// Constructor
    Thermistor( Cpl::Dm::MailboxServer&    myMbox,
                DriverAIOHalSingleInput_T  adcDriver,
                Cpl::Dm::Mp::Int32&        mpTemperatureOutput );

public:
    /// This method starts the driver (See Cpl::Itc::CloseSync)
    void request( OpenMsg& msg );

    /// This method stops the driver (See Cpl::Itc::CloseSync)
    void request( CloseMsg& msg );

protected:
    /// SW Timer expired
    void expired( void ) noexcept;

protected:
    /// Output MP
    Cpl::Dm::Mp::Int32&         m_mpTempOut;

    /// Handle to the underlying ADC driver/HW
    DriverAIOHalSingleInput_T   m_adcDriver;

    /// open/close state
    bool                        m_opened;
};

}           // End namespace(s)
}
}
#endif      // end header-latch
