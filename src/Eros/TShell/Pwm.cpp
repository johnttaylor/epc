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

#include "Driver/PicoDisplay/Api.h" // NOTE: Must be first because of naming-collision in the Pimoroni code
#include "Pwm.h"
#include "mp/ModelPoints.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Text/atob.h"
#include <string.h>

#define SECT_   "Eros::TShell"

///
using namespace Eros::TShell;


///////////////////////////
Pwm::Pwm( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
          Driver::DIO::Pwm&                          heaterPWMDriver,
          Driver::DIO::Pwm&                          fanPWMDriver,
          Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_heaterDriver( heaterPWMDriver )
    , m_fanDriver( fanPWMDriver )
{
}


///////////////////////////
Cpl::TShell::Command::Result_T Pwm::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();
    bool                            io       = true;

    // Set PWM
    if ( tokens.numParameters() == 3 )
    {
        // LCD Back-light
        if ( tokens.getParameter( 1 )[0] == 'b' )
        {
            unsigned brightness;
            if ( !Cpl::Text::a2ui( brightness, tokens.getParameter( 2 ) ) || brightness > 255 )
            {
                outtext.format( "Brightness argument invalid or out of range (%s)", tokens.getParameter( 2 ) );
                context.writeFrame( outtext );
                return Command::eERROR_INVALID_ARGS;
            }

            outtext.format( "Set LCD Brighntess to = %s", tokens.getParameter( 2 ));
            Driver::PicoDisplay::Api::setLCDBrightness( brightness );
            context.writeFrame( outtext );
            return io ? Command::eSUCCESS : Command::eERROR_IO;
        }

        // PWM Output
        else
        {
            // Select PWM signal
            Driver::DIO::Pwm* driver = &m_fanDriver;
            if ( tokens.getParameter( 1 )[0] == 'h' )
            {
                driver = &m_heaterDriver;
            }
            else if ( tokens.getParameter( 1 )[0] == 'f' )
            {
                driver = &m_fanDriver;
            }
            else
            {
                outtext.format( "Invalid PWM signal (%s)", tokens.getParameter( 1 ) );
                context.writeFrame( outtext );
                return Command::eERROR_INVALID_ARGS;
            }

            // Get duty cycle
            unsigned dutyCycle;
            if ( !Cpl::Text::a2ui( dutyCycle, tokens.getParameter( 2 ) ) || dutyCycle > 100 )
            {
                outtext.format( "Duty cycle invalid or out of range (%s)", tokens.getParameter( 2 ) );
                context.writeFrame( outtext );
                return Command::eERROR_INVALID_ARGS;
            }

            size_t dc = (dutyCycle * OPTION_DRIVER_DIO_PWM_MAX_DUTY_CYCLE_VALUE) / 100;
            outtext.format( "Set Duty cycle = %s%% (%lu)", tokens.getParameter( 2 ), dc );
            driver->setDutyCycle( dc );
            return io ? Command::eSUCCESS : Command::eERROR_IO;
        }
    }

    // If I get here, the command syntax was not valid
    return Command::eERROR_INVALID_ARGS;
}

