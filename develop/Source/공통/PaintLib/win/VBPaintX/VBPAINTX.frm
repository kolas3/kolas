VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form PaintXDemo 
   Caption         =   "PaintX Demo"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnLoadPicture 
      Caption         =   "LoadPicture"
      Height          =   495
      Left            =   0
      TabIndex        =   2
      Top             =   0
      Width           =   1335
   End
   Begin VB.CommandButton btnLoadResPicture 
      Caption         =   "LoadResPicture"
      Height          =   495
      Left            =   1440
      TabIndex        =   1
      Top             =   0
      Width           =   1335
   End
   Begin MSComDlg.CommonDialog dlgFileOpen 
      Left            =   4200
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      MaxFileSize     =   26000
   End
   Begin MSFlexGridLib.MSFlexGrid MyGrid 
      CausesValidation=   0   'False
      Height          =   2775
      Left            =   0
      TabIndex        =   0
      Top             =   480
      Width           =   4695
      _ExtentX        =   8281
      _ExtentY        =   4895
      _Version        =   393216
      Cols            =   1
      FixedCols       =   0
      WordWrap        =   -1  'True
      AllowUserResizing=   3
   End
End
Attribute VB_Name = "PaintXDemo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim MyDecoder As New PAINTXLib.PictureDecoder

Private OffsetWidth As Integer
Private OffsetHeight As Integer

Private Sub btnLoadPicture_Click()
  Dim filename
  Dim saFilenames
  Dim i

  ResetGrid
  
  dlgFileOpen.filename = ""
  dlgFileOpen.Flags = cdlOFNAllowMultiselect Or cdlOFNExplorer
  dlgFileOpen.ShowOpen
  
  ' Multiple selection?
  filename = dlgFileOpen.filename
    
  If InStr(1, filename, Chr(0), vbTextCompare) > 0 Then
    saFilenames = Split(filename, Chr(0), -1, vbTextCompare)
    ' zero-th element is the directory, the rest the filenames
    For i = 1 To UBound(saFilenames)
      Call LoadPictureTest(i - 1, saFilenames(0) & "\" & saFilenames(i))
    Next i
  Else
    Call LoadPictureTest(0, filename)
  End If

End Sub

Private Sub btnLoadResPicture_Click()
  ResetGrid
  Call LoadResPictureTest(0, 102)
End Sub

Private Sub Form_Load()
  OffsetWidth = Width - MyGrid.Width
  OffsetHeight = Height - MyGrid.Height
End Sub

Private Sub ResetGrid()
  MyGrid.Clear
  MyGrid.Rows = 2
  MyGrid.Cols = 1
  MyGrid.RowHeight(1) = 1440 ' 1 inch
End Sub

Private Function LoadResPictureTest(ByVal PicNum As Integer, ByVal ResId As Integer)
  Dim MyPicture As IPictureDisp ' decoded picture object
  Dim PictureHeight As Long     ' picture height in twips
  Dim PictureWidth As Long      ' picture width in twips
  
  ' Expand Grid if necessary
  If MyGrid.Cols < PicNum + 1 Then
    MyGrid.Cols = PicNum + 1
  End If
  ' Set Col Title to Filename
  MyGrid.Row = 0
  MyGrid.Col = PicNum
  MyGrid = "Resource #" & Str(102)
  
  ' Try to load Picture
  MyGrid.Row = 1
  On Error GoTo LoadError
  Set MyPicture = MyDecoder.LoadResPicture(ResId)
  
  ' Get picture size in twips
  PictureHeight = ScaleY(MyPicture.Height, vbHimetric, vbTwips)
  PictureWidth = ScaleX(MyPicture.Width, vbHimetric, vbTwips)
  
  ' Set cell size
  Call SetMinCellSize(1, PicNum, PictureWidth, PictureHeight)
  
  ' Show Picture
  Set MyGrid.CellPicture = MyPicture
  
  Exit Function
  
LoadError:
  MyGrid = Hex(Err.Number) & ":" & Err.Description & vbNewLine & _
           "Note: this will always fail if run from the environment!"
  Call SetMinCellSize(1, PicNum, 2000, 2000)
  Err.Clear
End Function

Private Sub LoadPictureTest(ByVal PicNum As Integer, ByVal filename As String)
  Dim MyPicture As IPictureDisp ' decoded picture object
  Dim PictureHeight As Long     ' picture height in twips
  Dim PictureWidth As Long      ' picture width in twips
  
  ' Expand Grid if necessary
  If MyGrid.Cols < PicNum + 1 Then
    MyGrid.Cols = PicNum + 1
  End If
  ' Set Col Title to Filename
  MyGrid.Row = 0
  MyGrid.Col = PicNum
  MyGrid = filename
  
  ' Try to load Picture
  MyGrid.Row = 1
  On Error GoTo LoadError
  Set MyPicture = MyDecoder.LoadPicture(filename)
  
  ' Get picture size in twips
  PictureHeight = ScaleY(MyPicture.Height, vbHimetric, vbTwips)
  PictureWidth = ScaleX(MyPicture.Width, vbHimetric, vbTwips)
  
  ' Set cell size
  Call SetMinCellSize(1, PicNum, PictureWidth, PictureHeight)
  ' Show Picture
  Set MyGrid.CellPicture = MyPicture
  
  Exit Sub
  
LoadError:
  MyGrid = Hex(Err.Number) & ":" & Err.Description
  Call SetMinCellSize(1, PicNum, 2000, 2000)
  Err.Clear
  
End Sub

Private Sub SetMinCellSize(Row As Integer, Col As Integer, Width As Long, Height As Long)
  MyGrid.ColWidth(Col) = Width
  If MyGrid.RowHeight(Row) < Height Then
    MyGrid.RowHeight(Row) = Height
  End If

End Sub
Private Sub Form_Resize()
  If Width > OffsetWidth Then
    MyGrid.Width = Width - OffsetWidth
  End If
  If Height > OffsetHeight Then
    MyGrid.Height = Height - OffsetHeight
  End If
End Sub

