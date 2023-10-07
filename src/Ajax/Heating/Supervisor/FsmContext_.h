#ifndef Ajax_Heating_Supervisor_FsmContext_x_h_
#define Ajax_Heating_Supervisor_FsmContext_x_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */



/* This file is auto-generated DO NOT MANUALLY EDIT this file! */


///
namespace Ajax { namespace Heating { namespace Supervisor { 


/// Context (aka actions/guards) for my Finite State Machine
class FsmContext_
{
public:
    /// Action
    virtual void allOff() noexcept = 0;

    /// Action
    virtual void checkForSensor() noexcept = 0;

    /// Action
    virtual void fanOn() noexcept = 0;

    /// Action
    virtual void heatOff() noexcept = 0;

    /// Action
    virtual void runHeatingAlgo() noexcept = 0;


public:
    /// Guard
    virtual bool isSensorAvailable() noexcept = 0;


public:
    /// Virtual Destructor
    virtual ~FsmContext_(){}

};

};};};  // end namespace(s)
#endif /// end header latch
