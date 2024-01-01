#ifndef Eros_Test_Thermistor_Api_h_
#define Eros_Test_Thermistor_Api_h_
/*-----------------------------------------------------------------------------
* COPYRIGHT_HEADER_TO_BE_FILLED_LATER
*----------------------------------------------------------------------------*/
/** @file */


#include "colony_config.h"
#include "Cpl/MApp/MApp_.h"
#include "Cpl/Dm/Mp/Array.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/Dm/SubscriberComposer.h"


/// Default interval time (in milliseconds) for displaying temperature
#ifndef OPTION_EROS_TEST_THERMISTOR_DISPLAY_INTERVAL_MS
#define OPTION_EROS_TEST_THERMISTOR_DISPLAY_INTERVAL_MS      (10*1000) // 10 second
#endif


///
namespace Eros {
///
namespace Test {
///
namespace Thermistor {


/** This concrete class implements MApp the monitors the output of the
    specialized Eros Thermistor driver and periodically writes the temperature
    and ADC-bits values to the trace output. The MApp also collects some metrics
 */
class Api : public Cpl::MApp::MApp_
{
public:
    /// The default MApp Name
    static constexpr const char* DEFAULT_NAME = "thermistor";

    /** Usage string (recommended that lines do not exceed 80 chars)
                                                         1         2         3         4         5         6         7         8
                                                12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* USAGE       = "args: [<displayms>]\n"
                                               "  <displayms>  milliseconds between outputs. Default is 10000ms";

    /** Description string (recommended that lines do not exceed 80 chars)
                                                         1         2         3         4         5         6         7         8
                                                12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* DESCRIPTION = "Periodically Samples temperature and displays sample/metric values.";


public:
    /// Constructor. 
    Api( Cpl::Container::Map<MAppApi>&    mappList,
         Cpl::Dm::MailboxServer&          myMbox,
         Cpl::Dm::Mp::ArrayInt32<2>&      mpDriverSamples,
         const char*                      name = DEFAULT_NAME );

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

    /// Change notification for new ADC Sample available
    void changedADCSample( Cpl::Dm::Mp::ArrayInt32<2>& mpThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;


protected:
    /// Model point that contains the ADC samples
    Cpl::Dm::Mp::ArrayInt32<2>&   m_mpSample;
    
    /// Observer for change notifications
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::ArrayInt32<2>> m_obSample;

    /// Cumulative sum Temp
    int64_t             m_sumTemp;

    /// Cumulative sum ADC-Bits
    uint64_t            m_sumADCBits;

    /// Maximum Temp. sampled
    int32_t             m_maxTemp;

    /// Minimum Temp. sampled
    int32_t             m_minTemp;

    /// Maximum ADC-Bits. sampled
    uint32_t            m_maxADCBits;

    /// Minimum ADC-Bits. sampled
    uint32_t            m_minADCBits;

    /// Number of samples
    uint32_t            m_numSamples;

    /// Display interval time
    uint32_t            m_displayMs;

    /// Time marker of last display action
    uint32_t            m_timeMarkerMs;

    /// Track my invalid data state
    bool                m_invalidData;
};

}       // end namespaces
} 
} 
#endif  // end header latch
