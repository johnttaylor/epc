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

#include "Driver/DIO/Pwm.h"

using namespace Driver::DIO;

    
Pwm::Pwm( const DriverDioPwmSTM32Config_T& pwmConfig )
    : m_pwm( pwmConfig )
    , m_started( false )

{
}
bool Pwm::start( size_t initialDutyCycle )
{
    if ( !m_started )
    {
        m_started = true;
        
        // TODO: Support configuring the Timer/PWM peripheral directly
        if ( HAL_TIM_PWM_Start( m_pwm.timerBlock, m_pwm.channelNum ) != HAL_OK )
        {
            return false;
        }
        setDutyCycle( initialDutyCycle );
        return true;
    }

    // TODO: FIXME: Stop is broken -->so don't fail on restart
    //return false;
    return true;
}

void Pwm::stop()
{
    if ( m_started )
    {
        // TODO: FIXME: Stop does not work in that a subsequent start() call fails!
        //m_started = false;
        //HAL_TIM_PWM_Stop( m_pwm.timerBlock, m_pwm.channelNum );
        //HAL_TIM_PWM_MspDeInit( m_pwm.timerBlock );
        //HAL_TIM_PWM_DeInit( m_pwm.timerBlock ); 
    }
}

size_t Pwm::getMaxDutyCycle() const
{
    return m_pwm.timerBlock->Instance->ARR;
}

void Pwm::setDutyCycle( size_t dutyCycle )
{
    if ( m_started )
    {
        switch ( m_pwm.channelNum )
        {
        case TIM_CHANNEL_1:
            m_pwm.timerBlock->Instance->CCR1 = dutyCycle;
            break;
        case TIM_CHANNEL_2:
            m_pwm.timerBlock->Instance->CCR2 = dutyCycle;
            break;
        case TIM_CHANNEL_3:
            m_pwm.timerBlock->Instance->CCR3 = dutyCycle;
            break;
        case TIM_CHANNEL_4:
            m_pwm.timerBlock->Instance->CCR4 = dutyCycle;
            break;
        default:
            break;
        }
    }
}


