#ifndef Ajax_Main_plat_alpha1_app_platform_h
#define Ajax_Main_plat_alpha1_app_platform_h
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

    This file contains the interface(s) that are platform AND application 
    VARIANT specific
 */

 ///
namespace Ajax {
///
namespace Main {

/** This method is used to initialize the Platform+Application variant.  It is called
    at the END of platform_initialize0().
 */
void appvariant_platform_initialize0();

/** This method is used to initialize the Platform+Application  Variant model points to
    known state. It is called at the END of platform_initializeModelPoints0().
 */
void appvariant_platform_initializeModelPoints0();

/** This method is used to 'open' any Platform+Application Variant specific 
    objects.  It is called at the END of platform_open0().
 */
void appvariant_platform_open0();

/** This method is used to 'close' any Platform+Application variant specific 
    objects. It is called at the START of platform_close0().
 */
void appvariant_platform_close0();


}       // end namespaces
}
#endif  // end header latch
