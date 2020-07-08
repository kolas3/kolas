// US7ASCII2KSC5601Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "US7ASCII2KSC5601.h"
#include "US7ASCII2KSC5601Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <direct.h>

/////////////////////////////////////////////////////////////////////////////
// CUS7ASCII2KSC5601Dlg dialog

CUS7ASCII2KSC5601Dlg::CUS7ASCII2KSC5601Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUS7ASCII2KSC5601Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUS7ASCII2KSC5601Dlg)
	m_strID = _T("kolas2");
	m_strPWD = _T("kolas2#");
	m_strSID = _T("kolas2");
	m_strPath = _T("");
	m_nQueryCount = 25;
	m_strTBLNAME = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nReverse = -1;

	m_pGrid = new CESL_Grid(NULL);
}

VOID CUS7ASCII2KSC5601Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUS7ASCII2KSC5601Dlg)
	DDX_Text(pDX, IDC_eID, m_strID);
	DDX_Text(pDX, IDC_ePWD, m_strPWD);
	DDX_Text(pDX, IDC_eSID, m_strSID);
	DDX_Text(pDX, IDC_ePATH, m_strPath);
	DDX_Text(pDX, IDC_eQUERYCOUNT, m_nQueryCount);
	DDV_MinMaxUInt(pDX, m_nQueryCount, 1, 500);
	DDX_Text(pDX, IDC_eTBLNAME, m_strTBLNAME);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUS7ASCII2KSC5601Dlg, CDialog)
	//{{AFX_MSG_MAP(CUS7ASCII2KSC5601Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bREFRESH, OnbREFRESH)
	ON_BN_CLICKED(IDC_bEXPORT, OnbEXPORT)
	ON_BN_CLICKED(IDC_bBROWSE, OnbBROWSE)
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUS7ASCII2KSC5601Dlg message handlers

BOOL CUS7ASCII2KSC5601Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID1, this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CUS7ASCII2KSC5601Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUS7ASCII2KSC5601Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CUS7ASCII2KSC5601Dlg::OnbREFRESH() 
{
	UpdateData(TRUE);
	INT ids;
	CString strQuery = _T("SELECT TNAME FROM TAB");
	CESL_DataMgr dm;
	dm.m_dbid = m_strID;		dm.m_dbpwd = m_strPWD;		dm.m_dbservice = m_strSID;

	ids = dm.RestructDataManager(strQuery);
	m_pGrid->Clear();
	INT nRows = dm.GetRowCount();
	if (nRows == 0) {
		m_pGrid->SetRows(2);
	} else {
		m_pGrid->SetRows(nRows+1);
	}

	SetColumnInfo(m_pGrid, 0, _T(""), 40, 4);
	SetColumnInfo(m_pGrid, 1, _T(""), 60, 4);
	SetColumnInfo(m_pGrid, 2, _T("TNAME"), 350, 1);

	for (INT r = 0; r < nRows; r++) {
		m_pGrid->SetTextMatrix(r+1, 2, dm.GetCellData(r, 0));
	}
}

VOID CUS7ASCII2KSC5601Dlg::OnbEXPORT() 
{
	INT ids;
	CString strTblName;
	INT nRow = m_pGrid->GetRow();
	strTblName = m_pGrid->GetTextMatrix(nRow, 2);
	strTblName.TrimLeft();			strTblName.TrimRight();
	RefreshColumnInfo(strTblName);

	CString strTmp, strField = _T("");
	for (INT i = 0; i < m_nCols; i++) {
		if (m_strColumnType[i] == _T("BLOB")) continue;
		if (strField.GetLength() == 0) strTmp = m_strColumnName[i];
			else strTmp.Format(_T(",%s"), m_strColumnName[i]);
		strField += strTmp;
	}
	CString strQuery;
	strQuery.Format(_T("SELECT %s FROM %s"), strField, strTblName);
	CESL_DataMgr dm;
	dm.m_dbid = m_strID;		dm.m_dbpwd = m_strPWD;		dm.m_dbservice = m_strSID;
	ids = dm.RestructDataManager(strQuery);
	INT nRows = dm.GetRowCount();
	if (nRows == 0) {
		if (AfxMessageBox(_T("데이타가 없습니다. 계속 하시겠습니까?"), MB_YESNO) != IDYES) return;
	}
	_tmkdir(strTblName.GetBuffer(0));
	CString strFilename;
	strFilename.Format(_T("%s\\%s"), strTblName, strTblName);
	FILE *fd = _tfopen(strFilename, _T("wb"));
	if (fd) fclose(fd);

	BOOL IsBLOB = FALSE;
	INT j;
	CString strData, strType;
	CString strMsg;
	INT nColumn;
	for (i = 0; i < nRows; i++) {
		dm.InitDBFieldData();
		nColumn = 0;
		for (j = 0; j < m_nCols; j++) {
			if (m_strColumnType[j] == _T("BLOB")) {
				IsBLOB = TRUE;
				continue;
			}
			strData = _T("");
			ids = dm.GetCellData(i, nColumn, strData);
			strType = GetType(m_strColumnType[j]);
			strData.Replace(_T("\r\n"), _T("<BR>"));
			dm.AddDBFieldData(m_strColumnName[j], strType, strData, 1);
			nColumn++;
		}
		ids = dm.MakeInsertFrame(strTblName);
		INT nTmp = dm.frame.GetCount();
		if (i % 100 == 0) 	FlushQuery(strFilename, &dm);
		strMsg.Format(_T("%d / %d"), i+1, nRows);
		GetDlgItem(IDC_STATIC_MSG1)->SetWindowText(strMsg);
	}
	FlushQuery(strFilename, &dm);

	// BLOB 처리
	if (IsBLOB) {
		ids = ReadBlobCfgFile(strTblName);
		if (ids < 0) {
			AfxMessageBox(_T("BLOB Cfg Read Error!"));
		}
		if (ids == 0) {
			BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
			CString strPKData;
			CString strSize;
			for (i = 0; i < nRows; i++) {
				INT nBlobSize = MAX_BLOB_BUFFER;
				strPKData = _T("");
				strPKData = GetData(&dm, i, m_PKFieldName);
				strSize = _T("");
				strSize = GetData(&dm, i, m_BLOBSizeFieldName);

				ids = dm.LOBGet(strTblName, m_PKFieldName, _T("NUMERIC"), strPKData, m_BLOBFieldName, pBuffer, &nBlobSize);
				SaveBLOBFile(strTblName, m_BLOBFieldName, strSize, m_PKFieldName, strPKData, i, pBuffer);
				strMsg.Format(_T("BLOB : %d / %d"), i+1, nRows);
				GetDlgItem(IDC_STATIC_MSG1)->SetWindowText(strMsg);
			}
			if (pBuffer) {
				free(pBuffer);
				pBuffer = NULL;
			}
		}
	}
	AfxMessageBox(_T("Finish!"));
}

INT CUS7ASCII2KSC5601Dlg::SaveBLOBFile(CString strTblName, CString strFieldName, CString strSize, CString PKFieldName, CString PKData, INT nRowIdx, BYTE *pBuffer)
{
	CString strFileName;
	strFileName.Format(_T("%s\\%s_%05d_%s.dat"), strTblName, strTblName, nRowIdx, strFieldName);

	CFile fd;
	if (fd.Open(strFileName, CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("File Open Error !"));
		return -1;
	}
	
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
		
	INT nSize = _ttoi(strSize);
	fd.WriteHuge(pBuffer, nSize);
	fd.Close();

	strFileName.Format(_T("%s\\%s_%05d_%s.cfg"), strTblName, strTblName, nRowIdx, strFieldName);

	CFile fd2;
	if (fd2.Open(strFileName, CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("File Open Error !"));
		return -1;
	}
	
	if( 0 == fd2.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd2.Write( &cUni, sizeof(TCHAR));
	}
		
	CString str;

	str.Format(_T("SIZE = %s\r\n"), strSize);
	fd2.Write(str, str.GetLength());

	str.Format(_T("PKFIELDNAME = %s\r\n"), PKFieldName);
	fd2.Write(str, str.GetLength());

	str.Format(_T("PKDATA = %s\r\n"), PKData);
	fd2.Write(str, str.GetLength());

	str.Format(_T("BLOBFIELDNAME = %s\r\n"), strFieldName);
	fd2.Write(str, str.GetLength());

	fd2.Close();

	return 0;
}

INT CUS7ASCII2KSC5601Dlg::ReadBlobCfgFile(CString strTblName)
{
	CString strFilename;
	strFilename.Format(_T("_TABLE_INFO\\%s.cfg"), strTblName);
	CStdioFile fd;
	if (fd.Open(strFilename, CFile::modeRead | CFile::typeBinary) == NULL) {
		return -1;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString str;
	m_PKFieldName = _T("");	m_PKData = _T("");	m_BLOBFieldName = _T("");	m_BLOBSizeFieldName = _T("");

	CString strTag, strData;
	INT pos;
	while(TRUE) {
		if (fd.ReadString(str) != TRUE) break;
		pos = str.Find('=', 0);
		if (pos == -1) continue;
		strTag = _T("");		strData = _T("");
		strTag = str.Mid(0, pos);
		strTag.TrimLeft();		strTag.TrimRight();
		strTag.MakeUpper();
		strData = str.Mid(pos+1);
		strData.TrimLeft();		strData.TrimRight();

		if (strTag == _T("PKFIELDNAME")) m_PKFieldName = strData;
		if (strTag == _T("BLOBFIELDNAME")) m_BLOBFieldName = strData;
		if (strTag == _T("BLOBSIZEFIELDNAME")) m_BLOBSizeFieldName = strData;
	}
	fd.Close();
	return 0;
}

CString CUS7ASCII2KSC5601Dlg::GetData(CESL_DataMgr *pDM, INT nRowIdx, CString strFieldName)
{
	INT j;
	INT nColumn = 0;
	for (j = 0; j < m_nCols; j++) {
		if (m_strColumnType[j] == _T("BLOB")) continue;
		if (m_strColumnName[j] == strFieldName) {
			break;
		}
		nColumn++;
	}
	CString result = _T("");
	pDM->GetCellData(nRowIdx, nColumn, result);
	return result;
}

CString CUS7ASCII2KSC5601Dlg::GetType(CString strType)
{
	if (strType == _T("VARCHAR")) return _T("STRING");
	if (strType == _T("VARCHAR2")) return _T("STRING");
	if (strType == _T("INTEGER")) return _T("NUMERIC");
	if (strType == _T("NUMERIC")) return _T("NUMERIC");
	if (strType == _T("NUMBER")) return _T("NUMERIC");
	if (strType == _T("TCHAR")) return _T("STRING");
	if (strType == _T("LONG")) return _T("STRING");
	return _T("STRING");
}

INT CUS7ASCII2KSC5601Dlg::FlushQuery(CString strFilename, CESL_DataMgr *pDM)
{
	FILE *fd = _tfopen(strFilename, _T("a+b"));
	if (fd == NULL) {
		return -1;
	}
		
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fd, 0, SEEK_END);		
		if( 0 == ftell(fd) )
		{
			/*UNCHANGE*/fputc(0xFF, fd);
			/*UNCHANGE*/fputc(0xFE, fd);
		}
		// ------------------------------------------------------------------------------
		
	INT nCount = pDM->frame.GetCount();
	POSITION pos = pDM->frame.GetHeadPosition();
	CString *pQuery;
	for (INT i = 0; i < nCount; i++) {
		pQuery = NULL;
		pQuery = (CString*)pDM->frame.GetNext(pos);
		if (pQuery) {
			_ftprintf(fd, _T("%s\r\n"), pQuery->GetBuffer(0));
		}
	}
	fclose(fd);
	pDM->EndFrame();
	return 0;
}

INT CUS7ASCII2KSC5601Dlg::SetColumnInfo(CMSHFlexGrid *pGrid, INT col, CString str, INT width, INT alignment)
{
	if (pGrid == NULL) return -1;
	pGrid->SetColAlignmentFixed(col, 4);
	pGrid->SetTextMatrix(0, col, str);
	pGrid->SetColWidth(col, 0, width*15);
	pGrid->SetColAlignment(col, alignment);
	return 0;
}

INT CUS7ASCII2KSC5601Dlg::SetReverse(CMSHFlexGrid *pGrid, INT row)
{
	if (pGrid == NULL) return -1;
	INT i;

	COLORREF blue, white, black;
	//blue = RGB(43,79,119);			
	blue = RGB(228,243,251);			//E4F3FB
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT col;
    col = pGrid->GetCol();
    
	INT cols;
	cols = pGrid->GetCols(0);

	if ( m_nReverse >= 0 && m_nReverse+1 < pGrid->GetRows()) {
		pGrid->SetRow(m_nReverse+1);
		for ( i = 1 ; i < cols ; i++ ) {
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(white);
			pGrid->SetCellForeColor(black);
		}
	}

	if ( row >= 0 ) {

		pGrid->SetRow(row+1);
		for ( i = 1 ; i < cols ; i++ ) {
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(blue);
			//SetCellForeColor(white);
			pGrid->SetCellForeColor(black);
		}
		m_nReverse = row;
	}
	
	pGrid->SetCol(col);

	return 0;
}

BEGIN_EVENTSINK_MAP(CUS7ASCII2KSC5601Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUS7ASCII2KSC5601Dlg)
	ON_EVENT(CUS7ASCII2KSC5601Dlg, IDC_MSHFLEXGRID1, -600 /* Click */, OnClickMshflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CUS7ASCII2KSC5601Dlg::OnClickMshflexgrid1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	INT nRow = m_pGrid->GetRow();	

	SetReverse(m_pGrid, nRow-1);
}

BOOL CUS7ASCII2KSC5601Dlg::DestroyWindow() 
{
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	return CDialog::DestroyWindow();
}

INT CUS7ASCII2KSC5601Dlg::RefreshColumnInfo(CString strTblName)
{
	UpdateData(TRUE);
	INT ids;
	CESL_DataMgr dm;
	dm.m_dbid = m_strID;		dm.m_dbpwd = m_strPWD;		dm.m_dbservice = m_strSID;
	CString strQuery;
	strQuery.Format(_T("SELECT CNAME, COLTYPE, WIDTH FROM COL WHERE TNAME='%s' ORDER BY CNAME"), strTblName);
	ids = dm.RestructDataManager(strQuery);
	m_nCols = dm.GetRowCount();
	if (m_nCols == 0) return -1;

	INT nRows = dm.GetRowCount();
	INT nColumn = 0;
	for (INT r = 0; r < nRows; r++) {
		m_strColumnName[r] = dm.GetCellData(r, 0);
		m_strColumnType[r] = dm.GetCellData(r, 1);
		m_strColumnWidth[r] = dm.GetCellData(r, 2);
	}
	return 0;
}

VOID CUS7ASCII2KSC5601Dlg::OnbBROWSE() 
{
	UpdateData();

	TCHAR szBuffer[1025];
	GetCurrentDirectory(1024, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	wmemset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;


	OFN.lpstrFilter = _T("모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("파일 열기");


	if (GetOpenFileName(&OFN) != 0)
	{
		m_strPath = OFN.lpstrFile;
		INT pos = 0;
		pos = m_strPath.ReverseFind('\\');
		if (pos == -1) pos = 0;
		m_strTBLNAME = m_strPath.Mid(pos+1);
		UpdateData(FALSE);
	}

	SetCurrentDirectory(szBuffer);
	return;	
}

VOID CUS7ASCII2KSC5601Dlg::OnbIMPORT() 
{
	CStdioFile fd;
	if (fd.Open(m_strPath, CFile::modeRead | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("File Open Error !"));
		return;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	UpdateData(TRUE);
	INT ids;
	CESL_DataMgr dm;
	dm.m_dbid = m_strID;		dm.m_dbpwd = m_strPWD;		dm.m_dbservice = m_strSID;

	CString str;
	INT i = 0;
	ids = dm.StartFrame();
	CString msg;
	INT nRowCount = 0;
	while(TRUE) {
		i++;
		if (fd.ReadString(str) != TRUE) break;
		ids = dm.AddFrame(str);
		nRowCount++;
		if (i % m_nQueryCount == 0) {
			ids = dm.SendFrame();
			dm.EndFrame();
			dm.StartFrame();
			if (ids != 0) {
				msg.Format(_T("SendFrame Error! [%d]"), ids);
				AfxMessageBox(msg);
				fd.Close();
				return;
			}
		}
		msg.Format(_T("%d"), i+1);
		GetDlgItem(IDC_STATIC_MSG2)->SetWindowText(msg);
	}
	ids = dm.SendFrame();
	dm.EndFrame();

	fd.Close();

	ids = ReadBlobCfgFile(m_strTBLNAME);
	CString strBlobFieldName = m_BLOBFieldName;
	if (ids == 0) {		// BLOB 가 있는 경우
		for (i = 0; i < nRowCount; i++) {
			UpdateBLOBFile(&dm, m_strTBLNAME, strBlobFieldName, i);
			msg.Format(_T("BLOB : %d"), i+1);
			GetDlgItem(IDC_STATIC_MSG2)->SetWindowText(msg);
		}
	}
	AfxMessageBox(_T("FINISH!"));
}

INT CUS7ASCII2KSC5601Dlg::UpdateBLOBFile(CESL_DataMgr *pDM, CString strTblName, CString strFieldName, INT nRowIdx)
{
	INT ids;
	CString strFileName, strFileNameCfg;
	strFileName.Format(_T("%s\\%s_%05d_%s.dat"), strTblName, strTblName, nRowIdx, strFieldName);
	strFileNameCfg.Format(_T("%s\\%s_%05d_%s.cfg"), strTblName, strTblName, nRowIdx, strFieldName);

	CFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("File Open Error !"));
		return -1;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
	DWORD nBufSize = MAX_BLOB_BUFFER;
	DWORD nReadedBufSize = fd.ReadHuge(pBuffer, nBufSize);
	fd.Close();

	CString SIZE = _T("");
	CString PKFIELDNAME = _T("");
	CString PKDATA = _T("");
	CString BLOBFIELDNAME = _T("");

	ReadCfgFile(strFileNameCfg, SIZE, PKFIELDNAME, PKDATA, BLOBFIELDNAME);
	ids = pDM->LOBUpdate(strTblName, PKFIELDNAME, _T("NUMERIC"), PKDATA, BLOBFIELDNAME, pBuffer, nReadedBufSize);

	if (pBuffer) {
		// KOL.UDF.022
		//delete pBuffer;
		free(pBuffer);
		pBuffer = NULL;
	}
	return 0;
}

INT CUS7ASCII2KSC5601Dlg::ReadCfgFile(CString strFileName, CString &SIZE, CString &PKFIELDNAME, CString &PKDATA, CString &BLOBFIELDNAME)
{
	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::typeBinary) == NULL) {
		return -1;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString str;
	m_PKFieldName = _T("");	m_PKData = _T("");	m_BLOBFieldName = _T("");	m_BLOBSizeFieldName = _T("");

	CString strTag, strData;
	INT pos;
	while(TRUE) {
		if (fd.ReadString(str) != TRUE) break;
		pos = str.Find('=', 0);
		if (pos == -1) continue;
		strTag = _T("");		strData = _T("");
		strTag = str.Mid(0, pos);
		strTag.TrimLeft();		strTag.TrimRight();
		strTag.MakeUpper();
		strData = str.Mid(pos+1);
		strData.TrimLeft();		strData.TrimRight();

		if (strTag == _T("SIZE")) SIZE = strData;
		if (strTag == _T("PKFIELDNAME")) PKFIELDNAME = strData;
		if (strTag == _T("PKDATA")) PKDATA = strData;
		if (strTag == _T("BLOBFIELDNAME")) BLOBFIELDNAME = strData;
	}
	fd.Close();
	return 0;
}