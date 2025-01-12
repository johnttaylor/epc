/** @namespace Cpl::Io::Serial::PhonyStdio

The PhonyStdio namespace provides a crude implementation of the Cpl::Io::InputOutput 
interface using the C Library's fread/fwrite functions.  This allows the TShell
and the Trace interfaces to be decoupled-from/not-rely-on the Cpl::Io::Stdio
implementation. 

The use case for this implementation is:
    1. Your application (e.g. functional simulator) is running on a Host system 
       such as Windoze or Linux.
    2. Your application does NOT use the Host's native filesystem for the 
       implementing the CPL IO Stream/File abstractions (e.g. using littlefs)
    3. Your application does 'not care' about the poor/crude performance of
       PhonyStdio::InputOutput class.

NOTE: On some host platforms when using PhonyStdio and there is blocked call
      one of the read() methods, the C Library exit() function hangs instead of
      exiting the application.  On a Linux host - you will need to use the
      following call to terminate the application
      
        # include <signal.h>
        raise( SIGTERM ); 
*/  
