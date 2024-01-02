#ifndef Eros_Test_Cycle_Api_h_
#define Eros_Test_Cycle_Api_h_
/*-----------------------------------------------------------------------------
* COPYRIGHT_HEADER_TO_BE_FILLED_LATER
*----------------------------------------------------------------------------*/
/** @file */


#include "colony_config.h"
#include "Cpl/MApp/MApp_.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/System/Timer.h"
#include "Driver/DIO/Pwm.h"

/// Default interval time (in milliseconds) for polling the duty cycle
#ifndef OPTION_EROS_TEST_CYCLE_INTERVAL_MS
#define OPTION_EROS_TEST_CYCLE_INTERVAL_MS               1000
#endif

///
namespace Eros {
///
namespace Test {
///
namespace Cycle {


/** This concrete class implements MApp the duty cycles the heating equipment
    based on command arguments
 */
class Api : public Cpl::MApp::MApp_, public Cpl::System::Timer
{
    /// The MApp Name
    static constexpr const char* MAPP_NAME = "cycle";
    
    /** Usage string (recommended that lines do not exceed 80 chars)
                                                         1         2         3         4         5         6         7         8
                                                12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* USAGE       = "args: [<heatpwm> <fanpwm> [<ontimems> <offtimems> [<repeat>]]]\n"
                                               "  <heatpwm>    duty cycle % for the heater PWM. Default: 100\n"
                                               "  <fanpwm>     duty cycle % for the fan PWM. Default: 100\n"
                                               "  <ontimems>   duration (in msec) for the equipment on.  Default: 60000\n"
                                               "  <offtimems>  duration (in msec) for the equipment off. Default: 60000\n"
                                               "  <repeat>     Number of times to repeat the duty cycle. Default: 5";

    /** Description string (recommended that lines do not exceed 80 chars)
                                                         1         2         3         4         5         6         7         8
                                                12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* DESCRIPTION = "Duty Cycles the Heating equipment.";


public:
    /// Constructor. 
    Api( Cpl::Container::Map<MAppApi>& mappList,
         Cpl::Dm::MailboxServer&       myMbox,
         Driver::DIO::Pwm&             heaterPWMDriver,
         Driver::DIO::Pwm&             fanPWMDriver );

protected:
    /// See Cpl::MApp::Api
    void intialize_() noexcept;

    /// See Cpl::MApp::Api
    bool start_( char* args ) noexcept;

    /// See Cpl::MApp::Api
    void stop_() noexcept;

    /// See Cpl::MApp::Api
    void shutdown_() noexcept;

protected:
    /// Helper method to parse the 'command line' options
    bool parse( char* args ) noexcept;

    /// Timer expired callback
    void expired( void ) noexcept;


protected:
    /// Driver
    Driver::DIO::Pwm&   m_heaterDriver;

    /// Driver
    Driver::DIO::Pwm&   m_fanDriver;

    /// Heater PWM
    unsigned            m_heaterPWM;

    /// Fan PWM
    unsigned            m_fanPWM;

    /// On time (in milliseconds)
    uint32_t            m_onTimeMs;

    /// Off time (in milliseconds)
    uint32_t            m_offTimeMs;

    /// Max Repeat count
    unsigned            m_maxCycles;

    /// Repeat count
    unsigned            m_cycleCount;

    /// Time marker for the duty cycle
    uint32_t            m_timeMarkerMs;

    /// Duty Cycle state
    bool                m_inOnCycle;

    /// Flag for initial cycle
    bool                m_firstCycle;
};

};      // end namespaces
};
};
#endif  // end header latch
