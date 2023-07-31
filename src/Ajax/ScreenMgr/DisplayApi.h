#ifndef Ajax_ScreenMgr_h_
#define Ajax_ScreenMgr_h_
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



/// 
namespace Ajax {
/// 
namespace ScreenMgr {


 
/** This class defines the Screen Manager's interface to Graphic library.  Only 
    functionality required by the Screen Manager is defined.  The expectation
    is that individual Screen instances directly access/use the Graphic library.
 */
class DisplayApi
{
public:
    /** This method is used to initialize/start the Graphics library.  Returns 
        false if an UNRECOVERABLE error occurred; else true is returned.
     */
    virtual bool start() noexcept = 0;

    /** This method is used to shutdown/stop the Graphics library
     */
    virtual void stop() noexcept = 0;

    /** This method requests that the Graphic library physically update the
        display.  Returns false if an UNRECOVERABLE error occurred; else
        true is returned.
     */
    virtual bool update() noexcept = 0;

    /** This method requests that the display is 'turned off'.  What 'off'
        means is application/library/platform specific. Returns false if an 
        UNRECOVERABLE error occurred; else true is returned.
     */
    virtual bool turnOff() noexcept = 0;

    /** This method restore the 'on' state after a previous call to turnOff().
        Returns false if an UNRECOVERABLE error occurred; else true is 
        returned.
     */
    virtual bool turnOn() noexcept = 0;


public:
    /// Virtual destructor
    virtual ~DisplayApi(){}
};

}      // end namespaces
}
#endif  // end header latch

