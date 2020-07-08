// K2UP_TnsManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "K2UP_TnsManager.h"
#include "K2UP_TnsManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "K2UP_TngManagerConfigDlg.h"
#include "RegistryEx.h"

/////////////////////////////////////////////////////////////////////////////
// CK2UP_TnsManagerDlg dialog

const COLORREF CLOUDBLUE = RGB(128, 184, 223);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF BLACK = RGB(1, 1, 1);
const COLORREF DKGRAY = RGB(128, 128, 128);
const COLORREF LTGRAY = RGB(192, 192, 192);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF DKYELLOW = RGB(128, 128, 0);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF DKRED = RGB(128, 0, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF DKBLUE = RGB(0, 0, 128);
const COLORREF CYAN = RGB(0, 255, 255);
const COLORREF DKCYAN = RGB(0, 128, 128);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF DKGREEN = RGB(0, 128, 0);
const COLORREF MAGENTA = RGB(255, 0, 255);
const COLORREF DKMAGENTA = RGB(128, 0, 128);

CK2UP_TnsManagerDlg::CK2UP_TnsManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CK2UP_TnsManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CK2UP_TnsManagerDlg)
	m_strHOST = _T("");
	m_strPORT = _T("");
	m_strSERVICENAME = _T("");
	m_strSID = _T("K2UP");
	m_strTNSFILE = _T("");
	m_strSearch = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nReverse = -1;
	m_bInTray = FALSE;
	m_bInitSearch = TRUE;
}

VOID CK2UP_TnsManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CK2UP_TnsManagerDlg)
	DDX_Control(pDX, IDC_COMBO1, m_cSearchOption);
	DDX_Text(pDX, IDC_eHOST, m_strHOST);
	DDX_Text(pDX, IDC_ePORT, m_strPORT);
	DDX_Text(pDX, IDC_eSERVICENAME, m_strSERVICENAME);
	DDX_Text(pDX, IDC_eSID, m_strSID);
	DDX_Text(pDX, IDC_eTNSFILE, m_strTNSFILE);
	DDX_Control(pDX, IDC_COM_MSHFLEXGRID1, m_gList);
	DDX_Text(pDX, IDC_eSEARCH, m_strSearch);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CK2UP_TnsManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CK2UP_TnsManagerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bCONFIG, OnbCONFIG)
	ON_BN_CLICKED(IDC_bCHANGETNS, OnbCHANGETNS)
	ON_BN_CLICKED(IDC_bREFRESH, OnbREFRESH)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_bSET230, OnbSET230)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bHIDE, OnbHIDE)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_EN_CHANGE(IDC_eSEARCH, OnChangeeSEARCH)

	//==================================================
    //2004-03-25 [이학중]
	ON_WM_SYSCOMMAND()
	//ON_COMMMNAD(5100,OnClose);
	//==================================================
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSGID_ICON_NOTIFY,OnTrayIconNotify)
	ON_COMMAND_RANGE(4000,4999,user_select_tray_menu)
    
    



END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CK2UP_TnsManagerDlg message handlers

BOOL CK2UP_TnsManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	VERIFY(m_bChangeTns.Attach(IDC_bCHANGETNS, this, YELLOW, RED, YELLOW));
	
	TnsManagerConfig.LoadConfig();
	m_strTNSFILE = GetTnsnamesFilePath(TnsManagerConfig.nHOME);
	InitGrid();
	UpdateData(FALSE);

	OnbREFRESH();
	UpdateData(FALSE);

	m_cSearchOption.ResetContent();
	m_cSearchOption.AddString(_T("이름"));
	m_cSearchOption.AddString(_T("IP"));
	m_cSearchOption.SetCurSel(0);

	if (CheckNewVersion() == TRUE) {
		if (AfxMessageBox(_T("새버젼의 TnsManager가 나왔습니다. 업데이트 하시겠습니까?"), MB_YESNO) == IDYES) {
			// 업데이트 실행 플로우
			SetTimer(1, 300, NULL);
		} else {
			// 정상 실행 플로우
			SetTimer(0, 300, NULL);
		}
	} else {
		// 정상 실행 플로우
		SetTimer(0, 300, NULL);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CK2UP_TnsManagerDlg::OnPaint() 
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
HCURSOR CK2UP_TnsManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CK2UP_TnsManagerDlg::OnbCONFIG() 
{
	CK2UP_TngManagerConfigDlg dlg(this);
	dlg.m_strFileName = GetTnsnamesFilePath(TnsManagerConfig.nHOME);
	dlg.DoModal();

	TnsManagerConfig.LoadConfig();
	m_strTNSFILE = GetTnsnamesFilePath(TnsManagerConfig.nHOME);
	InitGrid();
	UpdateData(FALSE);

	OnbREFRESH();
	UpdateData(FALSE);
}

VOID CK2UP_TnsManagerDlg::OnbCHANGETNS() 
{
	SetTnsInformation();
}

CString CK2UP_TnsManagerDlg::GetTnsnamesFilePath(INT nHOME)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth;
	depth.Format(_T("Software\\ORACLE\\HOME%d"), nHOME);

	CString strHOME, strResult;
	CRegistryEx env(hKey);
	if ( env.VerifyKey( depth ) ) {
		env.Open( hKey, depth );
		env.Read( _T("ORACLE_HOME"), strHOME);
		env.Close();
	}
	strResult.Format(_T("%s\\network\\admin\\tnsnames.ora"), strHOME);
	return strResult;
}

INT CK2UP_TnsManagerDlg::RefreshList(VOID)
{
	MYSQL MySql;
	MYSQL_RES *pMyRes;
	MYSQL_ROW MyRow;

	CString strQuery = _T("SELECT PK, LIBRARY_NAME, DB_SERVER_IP FROM patch_library_tbl ORDER BY LIBRARY_NAME");

	mysql_init(&MySql);						
	mysql_real_connect(&MySql, TnsManagerConfig.DBMS_HOST.GetBuffer(0), TnsManagerConfig.DBMS_ID.GetBuffer(0), TnsManagerConfig.DBMS_PWD.GetBuffer(0), NULL, MYSQL_PORT, NULL, 0); 

	if (mysql_ping(&MySql) != 0) return -4004;		// DB 가 응답하지 않는 경우 -4004
	
	mysql_select_db(&MySql, TnsManagerConfig.DBMS_NAME.GetBuffer(0)); 
	INT nResult2 = mysql_query(&MySql, strQuery.GetBuffer(0));   
	if(mysql_errno(&MySql)) 
	{	
		mysql_close(&MySql);	
		return -4005;	// 쿼리에 에러가 있음
	}
	pMyRes =  mysql_store_result(&MySql);		 
	UINT num_fields, i;
	unsigned __int64 num_row;

	num_row    = mysql_num_rows(pMyRes);		 
	num_fields = mysql_num_fields(pMyRes);		  //Column의 갯수를 뽑아낸다.

	if (num_row == 0) m_gList.SetRows(2);
		else m_gList.SetRows((INT)(num_row+1));

	INT nRow = 0;
	while ( MyRow = mysql_fetch_row(pMyRes) )	  //Row를 Fetch한다.
	{
		nRow++;
		DWORD *lengths;
		lengths = mysql_fetch_lengths(pMyRes);	  //Column들의 Length를 Fetch한다.
		for(i = 0; i < num_fields; i++)
		{
			if (MyRow[i] == NULL) continue;
			if (_tcsclen(MyRow[i]) > 0) {
				m_gList.SetTextMatrix(nRow, i, MyRow[i]);
			} else {
				m_gList.SetTextMatrix(nRow, i, _T(""));
			}
		}
	}
	mysql_free_result(pMyRes);				
	mysql_close(&MySql);		
	UpdateData(FALSE);

	return 0;
}

INT CK2UP_TnsManagerDlg::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
	m_gList.SetColAlignmentFixed(col, 4);
	m_gList.SetTextMatrix(0, col, str);
	m_gList.SetColWidth(col, 0, width*15);
	m_gList.SetColAlignment(col, alignment);
	
	return 0;
}

VOID CK2UP_TnsManagerDlg::InitGrid(VOID)
{
	m_gList.Clear();
	m_gList.SetRows(2);
	m_gList.SetCols(0, 3);

	SetColumnInfo(0, _T("PK"), 50, 1);
	SetColumnInfo(1, _T("도서관이름"), 250, 1);
	SetColumnInfo(2, _T("DB_SERVER_IP"), 130, 1);

	return;
}

VOID CK2UP_TnsManagerDlg::OnbREFRESH() 
{
	TnsList.LoadTnsnames(GetTnsnamesFilePath(TnsManagerConfig.nHOME));
	ShowTnsInformation(_T("K2UP"));
	RefreshList();	
}

BOOL CK2UP_TnsManagerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	INT nRowsScrolled = 1 * zDelta / 120;

    if (nRowsScrolled > 0)
        for (INT i = 0; i < nRowsScrolled; i++)
           m_gList.PostMessage(WM_VSCROLL, SB_LINEUP, 0);
    else
        for (INT i = 0; i > nRowsScrolled; i--)
            m_gList.PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


INT CK2UP_TnsManagerDlg::SetReverse(INT row)
{
	INT i;

	COLORREF blue, white, black;
	//blue = RGB(43,79,119);			
	blue = RGB(228,243,251);			//E4F3FB
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT col;
    col = m_gList.GetCol();
    
	INT cols;
	cols = m_gList.GetCols(0);

	if ( m_nReverse >= 0 && m_nReverse+1 < m_gList.GetRows()) {
		m_gList.SetRow(m_nReverse+1);
		for ( i = 1 ; i < cols ; i++ ) {
			m_gList.SetCol(i);
			m_gList.SetCellBackColor(white);
			m_gList.SetCellForeColor(black);
		}
	}

	if ( row >= 0 ) {
		m_gList.SetRow(row+1);
		for ( i = 1 ; i < cols ; i++ ) {
			m_gList.SetCol(i);
			m_gList.SetCellBackColor(blue);
			m_gList.SetCellForeColor(black);
		}
		m_nReverse = row;
	}
	
	m_gList.SetCol(col);

	return 0;
}

BEGIN_EVENTSINK_MAP(CK2UP_TnsManagerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CK2UP_TnsManagerDlg)
	ON_EVENT(CK2UP_TnsManagerDlg, IDC_COM_MSHFLEXGRID1, -600 /* Click */, OnClickComMshflexgrid1, VTS_NONE)
	ON_EVENT(CK2UP_TnsManagerDlg, IDC_COM_MSHFLEXGRID1, -601 /* DblClick */, OnDblClickComMshflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CK2UP_TnsManagerDlg::OnClickComMshflexgrid1() 
{
	UpdateData(TRUE);
	SetReverse(m_gList.GetRow()-1);
	UpdateData(FALSE);
}

VOID CK2UP_TnsManagerDlg::ShowTnsInformation(CString strSID)
{
	strSID.TrimLeft();			strSID.TrimRight();
	strSID.MakeUpper();

	m_strSID = _T("");
	m_strHOST = _T("");
	m_strPORT = _T("");
	m_strSERVICENAME = _T("");
	UpdateData(FALSE);

	CTns *pTns = TnsList.FindTnsItem(strSID);
	if (pTns == NULL) return;
	m_strSID = pTns->m_strSID;
	m_strHOST = pTns->m_strHOST;
	m_strPORT = pTns->m_strPORT;
	m_strSERVICENAME = pTns->m_strSERVICE_NAME;

	UpdateData(FALSE);
}

VOID CK2UP_TnsManagerDlg::SetTnsInformation(INT nRow)
{
	UpdateData(TRUE);
	
	BOOL bUSER_SELECT_TRAY_MENU = FALSE;
	if (nRow >= USER_SELECT_TRAY_MENU_ID) {
		bUSER_SELECT_TRAY_MENU = TRUE;
		nRow = nRow - USER_SELECT_TRAY_MENU_ID;
	}

	CTns *pTns = TnsList.FindTnsItem(m_strSID);
	if (pTns == NULL) {
		pTns = TnsList.AddTnsItem();
	}

	if (nRow == -1) nRow = m_gList.GetRow();
	CString strNewHost = m_gList.GetTextMatrix(nRow, 2);

	pTns->m_strSID = m_strSID;
	pTns->m_strHOST = strNewHost;
	pTns->m_strPORT = m_strPORT;
	pTns->m_strSERVICE_NAME = m_strSERVICENAME;

	TnsList.SaveTnsnames(GetTnsnamesFilePath(TnsManagerConfig.nHOME));

	ShowTnsInformation(m_strSID);

	if (bUSER_SELECT_TRAY_MENU == FALSE) {
		if (TnsManagerConfig.EXECUTE_K2UP_AT_SCREEN_MODE == TRUE) EXECUTE_K2UP();
	} else {
		if (TnsManagerConfig.EXECUTE_K2UP == TRUE) EXECUTE_K2UP();
	}
	
	return;
}

INT CK2UP_TnsManagerDlg::EXECUTE_K2UP(VOID)
{
	TCHAR szOrgDir[513];
	GetCurrentDirectory(512, szOrgDir);

	CString strNewDir;
	INT nPos = TnsManagerConfig.K2UP_PATH.ReverseFind('\\');
	if (nPos != -1) {
		strNewDir = TnsManagerConfig.K2UP_PATH.Left(nPos);
	}
	SetCurrentDirectory(strNewDir.GetBuffer(0));
	::WinExec(TnsManagerConfig.K2UP_PATH, TRUE);

	SetCurrentDirectory(szOrgDir);
	return 0;
}

VOID CK2UP_TnsManagerDlg::OnbSET230() 
{
	UpdateData(TRUE);

	CTns *pTns = TnsList.FindTnsItem(m_strSID);
	if (pTns == NULL) {
		pTns = TnsList.AddTnsItem();
	}

	CString strNewHost = m_gList.GetTextMatrix(m_gList.GetRow(), 2);

	pTns->m_strSID = m_strSID;
	pTns->m_strHOST = _T("192.168.1.230");
	pTns->m_strPORT = m_strPORT;
	pTns->m_strSERVICE_NAME = m_strSERVICENAME;

	TnsList.SaveTnsnames(GetTnsnamesFilePath(TnsManagerConfig.nHOME));

	ShowTnsInformation(m_strSID);

	if (TnsManagerConfig.EXECUTE_K2UP_AT_SCREEN_MODE == TRUE) EXECUTE_K2UP();
}

HBRUSH CK2UP_TnsManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CK2UP_TnsManagerDlg::TRAY_IN(VOID)
{
	if (m_bInTray == TRUE) return TRUE;

	NOTIFYICONDATA data;
	
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd = m_hWnd;
	data.uID = IDD_K2UP_TNSMANAGER_DIALOG;
	data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	data.uCallbackMessage = MSGID_ICON_NOTIFY;
	data.hIcon = ((CK2UP_TnsManagerApp*)AfxGetApp())->LoadIcon(IDR_MAINFRAME);
	_tcscpy(data.szTip,_T("K2UP Tns Manager"));
	Shell_NotifyIcon(NIM_ADD,&data);
	m_bInTray = TRUE;
	ShowWindow(false);
	return TRUE;
}

BOOL CK2UP_TnsManagerDlg::TRAY_OUT()
{
	m_bInTray = FALSE;
	NOTIFYICONDATA data;
	
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd = m_hWnd;
	data.uID = IDD_K2UP_TNSMANAGER_DIALOG;
	
	Shell_NotifyIcon(NIM_DELETE, &data);
	
	return true;
}

VOID CK2UP_TnsManagerDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);

		TRAY_IN();
	}
	if (nIDEvent == 1) {
		// Smart Update
		KillTimer(1);
		TnsManagerConfig.SaveConfig();
		::WinExec(_T("K2UP_UTIL_SMART_UPDATE.exe"), TRUE);
		PostMessage(WM_QUIT);
	}
	CDialog::OnTimer(nIDEvent);
}

LONG CK2UP_TnsManagerDlg::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	CMenu m_ContextMenu;
	
	switch(lParam)
	{
	case WM_LBUTTONDOWN:
		PostMessage(WM_COMMAND, 4000);
		break;
	case WM_RBUTTONDOWN:
		m_ContextMenu.CreatePopupMenu();

		INT nID = 4000;
		m_ContextMenu.AppendMenu( MF_STRING , nID , _T("화면 보이기") );
		//================================================================
        //2004-03-25[이학중]
        m_ContextMenu.AppendMenu( MF_STRING , 5100 , _T("프로그램 종료") );
		//================================================================
		m_ContextMenu.AppendMenu(MF_SEPARATOR);
      
	

		INT nRows = m_gList.GetRows();
		CString strMsg;
		for (INT i = 1; i < nRows; i++) {
			strMsg.Format(_T("%s [%s]"), m_gList.GetTextMatrix(i, 1), m_gList.GetTextMatrix(i, 2));
	        m_ContextMenu.AppendMenu( MF_STRING , nID + i , strMsg );
		}

		CPoint pos;
		GetCursorPos(&pos);
		SetForegroundWindow();
		m_ContextMenu.TrackPopupMenu(TPM_RIGHTALIGN,pos.x,pos.y,this);
	}
	
	return TRUE;
}

VOID CK2UP_TnsManagerDlg::user_select_tray_menu( UINT  nIDC )
{
	if (nIDC == 4000) {
		ShowWindow(TRUE);
		return;
	}

	INT nRow = nIDC - 4000;
	SetTnsInformation(USER_SELECT_TRAY_MENU_ID + nRow);

	return;
}

VOID CK2UP_TnsManagerDlg::OnClose() 
{
	TRAY_OUT();
	//TRAY_IN();
	//AfxMessageBox(_T("test"));
	CDialog::OnClose();
}

VOID CK2UP_TnsManagerDlg::OnbHIDE() 
{
	if (m_bInTray == FALSE) {
		TRAY_IN();	
	} else {
		ShowWindow(FALSE);
	}
	return;
}

VOID CK2UP_TnsManagerDlg::OnbSEARCH() 
{
	UpdateData(TRUE);
	INT nCol = 1;
	CString strSearchOption;
	m_cSearchOption.GetWindowText(strSearchOption);
	strSearchOption.TrimLeft();			strSearchOption.TrimRight();
	strSearchOption.MakeUpper();

	if (strSearchOption == _T("이름")) nCol = 1;
	if (strSearchOption == _T("IP")) nCol = 2;

	INT nStartRow = 1;
	if (m_bInitSearch == FALSE) nStartRow = m_gList.GetRow() + 1;
		else nStartRow = 1;
	m_strSearch.MakeUpper();
	INT nRows = m_gList.GetRows();
	CString str;
	for (INT i = nStartRow; i < nRows; i++) {
		str = m_gList.GetTextMatrix(i, nCol);
		str.MakeUpper();
		if (str.Find(m_strSearch) != -1) {
			m_gList.SetTopRow(i);
			SetReverse(i-1);
			UpdateData(FALSE);
			m_bInitSearch = FALSE;
			return;
		}
	}
	m_bInitSearch = TRUE;
	return;
}

VOID CK2UP_TnsManagerDlg::OnDblClickComMshflexgrid1() 
{
	SetTnsInformation();	
}

VOID CK2UP_TnsManagerDlg::OnOK() 
{
	OnbSEARCH();
}

VOID CK2UP_TnsManagerDlg::OnChangeeSEARCH() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bInitSearch = TRUE;
}

BOOL CK2UP_TnsManagerDlg::CheckNewVersion(VOID)
{
	CString strGetUrl = TnsManagerConfig.CHECK_VERSION_URL;
	CString strLocalPath = _T("VERSION.TMP");

	INT nVERSION = 0;
	if (URLDownloadToFile(NULL, strGetUrl, strLocalPath, 0, NULL) != S_OK) 
	{	
		AfxMessageBox(_T(" 패치파일 다운로드에 실패하였습니다. "));
		return FALSE;
	} else {
		CStdioFile fd;
		if (fd.Open(strLocalPath, CStdioFile::modeRead | CFile::typeBinary) == FALSE) return FALSE;

		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}
		
		CString str, strHeader, strData;
		INT pos;
		while(TRUE) {
			if (fd.ReadString(str) == FALSE) break;

			str.TrimLeft();			str.TrimRight();
			if (str.GetLength() == 0) continue;
			if (str.GetAt(0) == '#') continue;
			if (str.GetAt(0) == '.') continue;

			strHeader = _T("");			strData = _T("");
			pos = str.Find('=', 0);
			if (pos == -1) continue;

			strHeader = str.Left(pos);
			strData = str.Right(str.GetLength() - pos-1);

			strHeader.TrimLeft();		strHeader.TrimRight();			strHeader.MakeUpper();
			strData.TrimLeft();		strData.TrimRight();			

			if (strHeader == _T("VERSION")) {
				nVERSION = _ttoi(strData);
			}
		}
		fd.Close();
	}

	if (nVERSION > TnsManagerConfig.nVERSION) {
		TnsManagerConfig.nVERSION = nVERSION;
		return TRUE;
	}

	return FALSE;
}




VOID CK2UP_TnsManagerDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
	if(nID == SC_CLOSE )
	{
		OnbHIDE();		
	}

}



BOOL CK2UP_TnsManagerDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if((UINT)wParam == 5100)
	{
		OnClose () ;
	}


	return CDialog::OnCommand(wParam, lParam);
}
