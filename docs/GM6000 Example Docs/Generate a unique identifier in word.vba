'Part of the work-around for the MS Bug that prevents the copy-to-the-clipboard from working
'Handle 64-bit and 32-bit Office
#If VBA7 Then
  Private Declare PtrSafe Function GlobalUnlock Lib "kernel32" (ByVal hMem As LongPtr) As LongPtr
  Private Declare PtrSafe Function GlobalLock Lib "kernel32" (ByVal hMem As LongPtr) As LongPtr
  Private Declare PtrSafe Function GlobalAlloc Lib "kernel32" (ByVal wFlags As LongPtr, _
    ByVal dwBytes As LongPtr) As LongPtr
  Private Declare PtrSafe Function CloseClipboard Lib "user32" () As LongPtr
  Private Declare PtrSafe Function OpenClipboard Lib "user32" (ByVal hwnd As LongPtr) As LongPtr
  Private Declare PtrSafe Function EmptyClipboard Lib "user32" () As LongPtr
  Private Declare PtrSafe Function lstrcpy Lib "kernel32" (ByVal lpString1 As Any, _
    ByVal lpString2 As Any) As LongPtr
  Private Declare PtrSafe Function SetClipboardData Lib "user32" (ByVal wFormat As LongPtr, _
    ByVal hMem As LongPtr) As LongPtr
#Else
  Private Declare Function GlobalUnlock Lib "kernel32" (ByVal hMem As Long) As Long
  Private Declare Function GlobalLock Lib "kernel32" (ByVal hMem As Long) As Long
  Private Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Long, _
    ByVal dwBytes As Long) As Long
  Private Declare Function CloseClipboard Lib "user32" () As Long
  Private Declare Function OpenClipboard Lib "user32" (ByVal hwnd As Long) As Long
  Private Declare Function EmptyClipboard Lib "user32" () As Long
  Private Declare Function lstrcpy Lib "kernel32" (ByVal lpString1 As Any, _
    ByVal lpString2 As Any) As Long
Private Declare Function SetClipboardData Lib "user32" (ByVal wFormat _
    As Long, ByVal hMem As Long) As Long
#End If

Const GHND = &H42
Const CF_TEXT = 1
Const MAXSIZE = 4096

'Generates a unique identifier and inserts into the document at current cursor position
Sub TraceID()
    On Error GoTo MissingVariables ' no variable yet
    Selection.Text = "[" & ActiveDocument.CustomDocumentProperties("TracePrefix").Value & ActiveDocument.CustomDocumentProperties("TraceLastId").Value + 1 & "] "
    Selection.Collapse wdCollapseEnd
    ActiveDocument.CustomDocumentProperties("TraceLastId").Value = ActiveDocument.CustomDocumentProperties("TraceLastId").Value + 1
    Exit Sub
MissingVariables: ' No properties set
    Selection.Text = "[You must first define 'TracePrefix' and 'TraceLastId' Custom Document properties] "
End Sub

' Generates a list of trace identifiers and copies the list to the clipboard
Sub GetTraceIds()
    Dim tempTable As Paragraph
    Dim prefix As String
    Dim results As String
    Dim paraText As String
    Dim count As Integer
    
    count = 0
    results = ""
    prefix = "[" & ActiveDocument.CustomDocumentProperties("TracePrefix").Value
    For Each tempTable In ActiveDocument.Paragraphs
        If InStr(tempTable.Range.Text, prefix) = 1 Then
            paraTest = tempTable.Range.Text
            results = results & paraTest
            count = count + 1
            'Debug.Print Left$(tempTable.Range.Text, Len(tempTable.Range.Text) - 1)
        End If
    Next
    '
    'Note: DataObject requires the Microsoft Forms 2.0 Object Library
    '      You can reference this library by either going to your Visual Basic Editor's
    '      tools menu and enabling the Microsoft Forms 2.0 Object Library reference or
    '      by simply adding a userform to your project (the userform doesn't have to do
    '      anything, it's very existence tells your project that the Forms library is
    '      needed).
    
    'Put the list into the clipboard
    Dim obj As New DataObject
    ClipBoard_SetData results
    MsgBox "List of Trace IDs (count=" & count & ") have been copied to the Clipboard", vbInformation
    Debug.Print results
End Sub


Function ClipBoard_SetData(MyString As String)
'PURPOSE: API function to copy text to clipboard
'SOURCE: www.msdn.microsoft.com/en-us/library/office/ff192913.aspx

#If VBA7 Then
  Dim hGlobalMemory As LongPtr, lpGlobalMemory As LongPtr
  Dim hClipMemory As LongPtr, x As LongPtr
#Else
  Dim hGlobalMemory As Long, lpGlobalMemory As Long
  Dim hClipMemory As Long, x As Long
#End If

'Allocate moveable global memory
  hGlobalMemory = GlobalAlloc(GHND, Len(MyString) + 1)

'Lock the block to get a far pointer to this memory.
  lpGlobalMemory = GlobalLock(hGlobalMemory)

'Copy the string to this global memory.
  lpGlobalMemory = lstrcpy(lpGlobalMemory, MyString)

'Unlock the memory.
  If GlobalUnlock(hGlobalMemory) <> 0 Then
    MsgBox "Could not unlock memory location. Copy aborted."
    GoTo OutOfHere2
  End If

'Open the Clipboard to copy data to.
  If OpenClipboard(0&) = 0 Then
    MsgBox "Could not open the Clipboard. Copy aborted."
    Exit Function
  End If

'Clear the Clipboard.
  x = EmptyClipboard()

'Copy the data to the Clipboard.
  hClipMemory = SetClipboardData(CF_TEXT, hGlobalMemory)

OutOfHere2:
  If CloseClipboard() = 0 Then
    MsgBox "Could not close Clipboard."
  End If

End Function




