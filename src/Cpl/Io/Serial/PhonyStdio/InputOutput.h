#ifndef Cpl_Io_Serial_PhonyStdio_InputOutput_h_
#define Cpl_Io_Serial_PhonyStdio_InputOutput_h_
/*-----------------------------------------------------------------------------
 * This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2025  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/
/** @file */

#include "Cpl/Io/InputOutput.h"

///
namespace Cpl {
///
namespace Io {
///
namespace Serial {
///
namespace PhonyStdio {


/** This concrete class implements the InputOutput stream interface
    built on top the C Library's fread/fwrite functions with the 'stdin' and
    'stdout' file handles.
 */
class InputOutput : public Cpl::Io::InputOutput
{
public:
    /// Constructor
    InputOutput();

    /// Destructor
    ~InputOutput();


public:
    /// Pull in overloaded methods from base class
    using Cpl::Io::InputOutput::read;

    /** See Cpl::Io::Input.  Note: This method only attempts to read at most
        one byte from the 'stdin' file handle.  This is the 'workaround' for
        the fread() function blocking until the requested number of bytes are
        read (instead of returning after reading the number of bytes available).
     */
    bool read( void* buffer, int numBytes, int& bytesRead );

    /// See Cpl::Io::Input. Note: This method always returns true (i.e not supported)
    bool available();

public:
    /// Pull in overloaded methods from base class
    using Cpl::Io::InputOutput::write;

    /// See Cpl::Io::Output
    bool write( const void* buffer, int maxBytes, int& bytesWritten );

    /// See Cpl::Io::Output
    void flush();

    /// See Cpl::Io::IsEos.
    bool isEos();

    /// See Cpl::Io::Output
    void close();

protected:
    /// End-of-Stream state
    bool m_eos;
};

}  // end namespaces
}
}
}
#endif  // end header latch
