#ifndef Ajax_Heating_Flc_Api_h_
#define Ajax_Heating_Flc_Api_h_
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

#include "colony_config.h"
#include "Ajax/Dm/MpFlcConfig.h"

/// 
namespace Ajax {
/// 
namespace Heating {
/// 
namespace Flc {

/** This class implements the Fuzzy logic controller as defined by the 
    SWA-1330 GM6000 Fuzzy Logic Temperature Control document.  
    
    The interface should be called on periodic basis.  

    The interface receives its 'configuration' via model points

    Inputs are (all values must have the same degree units):
        Current Temperature
        Current Setpoint

    Outputs:
        Value to change/adjust the current capacity 'output signal'

 */
class Api
{
public:
    /// Constructor
    Api( Ajax::Dm::MpFlcConfig& mpCfg );

public:
    /** This method is used to initialize/reset the Controller.  It should
        be called once, before calls to calcChange(). 
    
        Return true if successful; else false is returned
    */
    bool start() noexcept;

    /** This method should be called on a fixed periodic basis to calculate
        the fuzzy output.  The output is change (to increase/decrease) the
        current request output capacity
     */
    int32_t calcChange( int32_t currentTemp, int32_t setpoint ) noexcept;

    /** This method is used to stop the controller.  Once stopped, the 
        controller can be restarted by calling start().
     */
    void stop() noexcept;

protected:
    /// Helper method to fuzzify an input value
    void fuzzify( int32_t inValue, int32_t fuzzyOut[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept;

    /// Helper that executes the inference rules
    void runInference( const int32_t m1Vector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS],
                       const int32_t m2Vector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS], 
                       int32_t       outVector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept;

    /// Helper method the defuzzifys the output vector
    int32_t defuzz( const int32_t outVector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept;

protected:
    /// Previous delta error
    int32_t                 m_prevDeltaError;

    /// Config Model Point
    Ajax::Dm::MpFlcConfig&  m_mpCfg;

    /// Runtime config
    Config_T                m_cfg;

    /// Flag for first-cycle
    bool                    m_firstCycle;
};


};      // end namespaces
};
};
#endif  // end header latch

