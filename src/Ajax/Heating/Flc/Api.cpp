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

#include "Api.h"
 
using namespace Ajax::Heating::Flc;


#define SET_OM          0   // overshoot medium
#define SET_OS          1   // overshoot small
#define SET_ZE          2   // zero equal
#define SET_US          3   // undershoot small
#define SET_UM          4   // undershoot medium

static const unsigned char inferenceTable_[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS][AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] =
{
  { SET_UM, SET_UM, SET_UM, SET_US, SET_ZE },
  { SET_UM, SET_UM, SET_US, SET_ZE, SET_OS },
  { SET_UM, SET_US, SET_ZE, SET_OS, SET_OM },
  { SET_US, SET_ZE, SET_OS, SET_OM, SET_OM },
  { SET_ZE, SET_OS, SET_OM, SET_OM, SET_OM }
};

/////////////////////////////////////////////////////////////////////////////
Api::Api( Config_T cfg )
    : m_cfg( cfg )
    , m_firstCycle( true )
{
}

int32_t Api::calcChange( int32_t currentTemp, int32_t setpoint ) noexcept
{
    // Calculate the crisp inputs
    int32_t err      = setpoint - currentTemp;
    int32_t dErr     = m_firstCycle ? 0 : err - m_prevDeltaError;
    m_firstCycle     = false;
    m_prevDeltaError = err;

    // Fuzzify the inputs
    int32_t m1[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS];
    int32_t m2[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS];
    fuzzify( err * m_cfg.errScaler, m1 );
    fuzzify( dErr * m_cfg.dErrScaler, m2 );

    // Run the inference rules
    int32_t out[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS];
    runInference( m1, m2, out );

    // Defuzzify the output
    return defuzz( out );
}


void Api::fuzzify( int32_t inValue, int32_t fuzzyOut[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept
{
    // Initialize membership vector
    memset( fuzzyOut, 0, sizeof( int32_t ) * AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS );
    int32_t xbase = m_cfg.maxY;

    // Less than the OM center point
    if ( inValue < (xbase * (-2)) )
    {
        fuzzyOut[SET_OM] = m_cfg.maxY;
    }
    // Between: OM..OS 
    else if ( inValue < (xbase * (-1)) )
    {
        fuzzyOut[SET_OM] = m_cfg.maxY - (inValue + 2 * xbase);
        fuzzyOut[SET_OS] = inValue + 2 * xbase;
    }
    // Between: OS..ZE
    else if ( inValue < 0 )
    {
        fuzzyOut[SET_OS] = m_cfg.maxY - (inValue + xbase);
        fuzzyOut[SET_ZE] = inValue + xbase;
    }
    // Between: ZE..US
    else if ( inValue < xbase )
    {
        fuzzyOut[SET_ZE] = m_cfg.maxY - inValue;
        fuzzyOut[SET_US] = inValue;
    }
    // Between: US..UM
    else if ( inValue < (2 * xbase) )
    {
        fuzzyOut[SET_US] = m_cfg.maxY - (inValue - xbase);
        fuzzyOut[SET_UM] = inValue - xbase;
    }
    // Greater than the UM center point
    else
    {
        fuzzyOut[SET_UM] = m_cfg.maxY;
    }
}

void Api::runInference( const int32_t m1Vector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS],
                        const int32_t m2Vector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS],
                        int32_t       outVector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept
{
    // Initialize output vector
    memset( outVector, 0, sizeof( int32_t ) * AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS );

    // Loop through m1 membership set
    for ( unsigned i = 0; i < AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS; i++ )
    {
        int32_t minVector[5] ={ 0, };

        // Loop through m2 membership set
        for ( unsigned j = 0; j < AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS; j++ )
        {
            // Find the min value
            if ( m1Vector[i] < m2Vector[j] )
            {
                minVector[j] = m1Vector[i];
            }
            else
            {
                minVector[j] = m2Vector[j];
            }
        }

        // Find the maximum of the minimums
        int32_t  maxValue = minVector[0];
        unsigned maxIdx   = 0;
        for ( unsigned j=1; j < AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS; j++ )
        {
            // Capture the maximum and its index
            if ( maxValue < minVector[j] )
            {
                maxValue = minVector[j];
                maxIdx = j;
            }
        }

        // 'Run' the inference rule
        unsigned outIdx    = inferenceTable_[i][maxIdx];
        outVector[outIdx] += maxValue;
        if ( outVector[outIdx] > m_cfg.maxY )
        {
            // Clamp the output (cannot exceed Ymax)
            outVector[outIdx] = m_cfg.maxY;
        }
    }
}

int32_t Api::defuzz( const int32_t outVector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept
{
    int32_t numerator  = 0;
    int32_t denominator = 0;
    for ( unsigned i = 0; i < AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS; i++ )
    {
        denominator += outVector[i];
        numerator  += outVector[i] * m_cfg.outK[i];
    }

    // Do nothing if the denominator is zero (it should never be zero, but...)
    if ( denominator == 0 )
    {
        // TODO: Log the failure, throw an alert, something...
        return 0;
    }

    // Invert the final output
    return ((numerator * m_cfg.outputScaler) / denominator) * -1;
}


void Api::reset() noexcept
{
    m_firstCycle = true;
}


