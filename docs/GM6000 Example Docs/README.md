This directory contains the GM6000 example documents in native Microsoft Word 
and Excel formats.

# Requirements
When there is no requirements management tool available, the recommendation is
to use spreadsheet (instead of a Word document) for capturing and tracking 
requirements.

## Auto Numbering Macro
The requirement spreadsheets contain a macro - `Ctrl+Shift+A` - that auto 
generates unique requirement identifiers.  The macro is VBA call that uses the
following document property variables when generating the requirement 
identifiers.

| Variable | Description |
|----------|-------------|
| `TraceLastId` | Is a numeric counter that is incremented each time the macro is invoked |
| `TracePrefix` | Is a text field that is used to prefix the numeric counter value to form a complete requirement identifier |

# Software Architecture and Detailed Design
The SW architecture and detaild design documents contain unique identifiers 
that are applied to individual sections to label traceable architectural items.  
The identifiers once assigned shall not be re-used or reassigned to a 
different item.  

## Auto Numbering Macro
The documents contain a macro - `Ctrl+Shift+A` - that auto generates unique 
tracing identifiers.  The macro is VBA call that uses the following document 
property variables when generating the requirement identifiers.

| Variable | Description |
|----------|-------------|
| `TraceLastId` | Is a numeric counter that is incremented each time the macro is invoked |
| `TracePrefix` | Is text field that is used to prefix the numeric counter value to form a complete trace identifier |

## Get Trace Items
The documents contain a macro - `Ctrl+Shift+I` - generates a list the current 
trace identifiers and copies the list to Windows clipboard.



