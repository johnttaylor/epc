'Generates a unique identifier and inserts into the current cell

Sub TraceID()
    On Error GoTo MissingVariables ' no variable yet
    Dim num As String
    num = ActiveWorkbook.CustomDocumentProperties("TraceLastId").Value + 1
    num = Format(num, "000")
    ActiveCell.Value = ActiveWorkbook.CustomDocumentProperties("TracePrefix").Value & num
    ActiveWorkbook.CustomDocumentProperties("TraceLastId").Value = ActiveWorkbook.CustomDocumentProperties("TraceLastId").Value + 1
    Exit Sub
MissingVariables: ' No properties set
    Selection.Text = "[You must first define 'TracePrefix' and 'TraceLastId' Custom Document properties] "
End Sub