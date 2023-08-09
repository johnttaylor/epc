#ifndef Ajax_Ui_Events_h_
#define Ajax_Ui_Events_h_
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

    NOTE: This file can only contain valid C code since it will be included
          via the colony_map.h LHeader pattern
*/


 
/// Enum for the set of UI events (okay, there are only button events)
enum AjaxUiEvent_T
{
    AJAX_UI_EVENT_BUTTON_A,       //!< Top left button
    AJAX_UI_EVENT_BUTTON_B,       //!< Bottom left button
    AJAX_UI_EVENT_BUTTON_X,       //!< Top right button
    AJAX_UI_EVENT_BUTTON_Y,       //!< Bottom right button
    AJAX_UI_EVENT_BUTTON_ESC,     //!< The two bottom buttons pressed together
    AJAX_UI_EVENT_NO_EVENT        //!< No/null/nop event
};

 

#endif  // end header latch

