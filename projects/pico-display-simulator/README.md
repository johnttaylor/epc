# GUI for the Pico-Display Simulator

This Visual Studio C# Solution is used to build the Windows executable that 
provides the simulated Display, RGB LED, and buttons.

The executable communicates with a console application via TCP sockets.
The executable is a _listener_ on port `5010`.  The GUI application must be
launch prior to running the GM6000 simulator console application.

#### Keys
The GUI has 4 button boxes/widgets - one for each physical key.  The application
uses key-combination (e.g. A+X to move right) to get 8 logical keys.  In the GUI
you can __not__ generate key-combination by selecting the button widgets with the 
mouse - you must use the keyboard for the key-combination.