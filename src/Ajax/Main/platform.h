#ifndef Ajax_Main_platform_h
#define Ajax_Main_platform_h
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

    This file contains the interface(s) that are platform VARIANT specific
 */

///
namespace Ajax {
///
namespace Main {

/** This method is used to initialize 'stuff' immediately before an application
    initialization
 */
void platform_initialize0();

/** This method is used to initialize the model points to known state
 */
void platform_initializeModelPoints0();

/** This method is used to 'open' any platform specific objects.  It is called
    prior to the application 'open' calls
 */
void platform_open0();

/** This method is used to 'close' any platform specific objects.  It is called
    after to the application 'close' calls
 */
void platform_close0();

/** This method is used to 'exit' the application'
 */
int platform_exit( int exitCode );


}       // end namespaces
}
#endif  // end header latch
