#ifndef Ajax_Main_application_h
#define Ajax_Main_application_h
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

    This file contains the interface(s) that are application VARIANT specific
 */

///
namespace Ajax {
///
namespace Main {

/** This method is used to initialize the Application variant.  It is called
    BEFORE the common application initialization is performed.
 */
void appvariant_initialize0();

/** This method is used to initialize the Application Variant model points to 
    known state
 */
void appvariant_initializeModelPoints0();

/** This method is used to 'open' any Application Variant specific objects.  It 
    is called BEFORE the common application 'open' calls are made
 */
void appvariant_open0();

/** This method is use to launch the Application UI's home screen
 */
void appvariant_launchHomeScreen();

/** This method is used to 'close' any Application variant specific objects.  
    It is called AFTER to the application 'close' calls
 */
void appvariant_close0();


}       // end namespaces
}
#endif  // end header latch
