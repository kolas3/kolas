// SetDMConnectionInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetDMConnectionInfo.h"
#include "SetDMConnectionInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DMFileInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDMConnectionInfoDlg dialog

CSetDMConnectionInfoDlg::CSetDMConnectionInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDMConnectionInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDMConnectionInfoDlg)
	m_strCONNECTION_INFO = _T("kolas2@k2up/kolas2#[127.0.0.1]");
	m_strDIR = _T("..\\CFG\\REFERENCE\\DM");
	m_bCheck1 = FALSE;
	m_bCheck2 = FALSE;
	m_bCheck3 = FALSE;
	m_bDM = FALSE;
	m_strMsg1 = _T("Ready");
	m_strCONNECTION_INFO_BEFORE = _T("");
	m_strHelp1 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CSetDMConnectionInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDMConnectionInfoDlg)
	DDX_Control(pDX, IDC_COMBO3, m_cCombo3);
	DDX_Control(pDX, IDC_COMBO2, m_cCombo2);
	DDX_Control(pDX, IDC_COMBO1, m_cCombo1);
	DDX_Text(pDX, IDC_eCONNECTION_INFO, m_strCONNECTION_INFO);
	DDX_Text(pDX, IDC_eDIR, m_strDIR);
	DDX_Check(pDX, IDC_CHECK1, m_bCheck1);
	DDX_Check(pDX, IDC_CHECK2, m_bCheck2);
	DDX_Check(pDX, IDC_CHECK3, m_bCheck3);
	DDX_Check(pDX, IDC_chDM, m_bDM);
	DDX_Text(pDX, IDC_STATIC_MSG1, m_strMsg1);
	DDX_Text(pDX, IDC_eCONNECTION_INFO_BEFORE, m_strCONNECTION_INFO_BEFORE);
	DDX_Text(pDX, IDC_STATIC_HELP1, m_strHelp1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetDMConnectionInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDMConnectionInfoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bCONVERT, OnbCONVERT)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bMOBILE, OnbMOBILE)
	ON_BN_CLICKED(IDC_bDBCONNINFO, OnbDBCONNINFO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDMConnectionInfoDlg message handlers

BOOL CSetDMConnectionInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_strHelp1 = _T("Before가 Empty인 경우는 무조건 After로 변환을 수행합니다. 만약 Before에 값이 있을 경우는 Before와 일치하는 것만 After로 변환 합니다., 대소문자 구분하지 않고 무조건 소문자로 캐스팅하여 비교합니다.");

	InitComboBoxControl();

	UpdateData(FALSE);

	OnbDBCONNINFO();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CSetDMConnectionInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CSetDMConnectionInfoDlg::OnPaint() 
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
HCURSOR CSetDMConnectionInfoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CSetDMConnectionInfoDlg::OnbCONVERT() 
{
	DWORD START, FINISH;
	START = GetTickCount();

	UpdateData(TRUE);
	
	SetCfgFile();

	if (m_bDM == FALSE) return;

	CString strTarget;
	strTarget.Format(_T("%s\\*.DM"), m_strDIR);
	CFileFind fd;
	if (fd.FindFile(strTarget) == FALSE) {
		AfxMessageBox(_T("검색된 파일이 없습니다."));
		return;
	}
	m_strCONNECTION_INFO_BEFORE.TrimLeft();			m_strCONNECTION_INFO_BEFORE.TrimRight();
	m_strCONNECTION_INFO_BEFORE.MakeLower();

	CString strPath, strMsg;
	CDMFileInfo DMFileInfo;
	INT ids;
	INT nTotal = 0, nConvert = 0, nSkip = 0;
	BOOL bConvert;
	while(TRUE) {
		if (fd.FindNextFile() == FALSE) break;

		strPath = fd.GetFilePath();
		m_strMsg1.Format(_T("%s"), strPath);									UpdateData(FALSE);

		ids = DMFileInfo.LoadFile(strPath);
		if (ids < 0) {
			strMsg.Format(_T("Load Error : %s"), strPath);
			AfxMessageBox(strMsg);
			continue;
		}
		nTotal++;
		bConvert = FALSE;
		
		if (m_strCONNECTION_INFO_BEFORE.GetLength() == 0) 
		{
			DMFileInfo.SetCONNECTION_INFO(m_strCONNECTION_INFO);
			bConvert = TRUE;
		}
		else 
		{
			CString strTmp;
			DMFileInfo.GetCONNECTION_INFO(&strTmp);
			strTmp.TrimLeft();			strTmp.TrimRight();
			strTmp.MakeLower();
			if (m_strCONNECTION_INFO_BEFORE == strTmp) 
			{
				DMFileInfo.SetCONNECTION_INFO(m_strCONNECTION_INFO);
				bConvert = TRUE;
			}
		}
		if (bConvert == TRUE) 
		{
			ids = DMFileInfo.SaveFile(strPath);
			if (ids < 0) {
				strMsg.Format(_T("Save Error : %s"), strPath);
				AfxMessageBox(strMsg);
				continue;
			}
			nConvert++;
		} else {
			nSkip++;
		}
	}

	FINISH = GetTickCount();
	m_strMsg1.Format(_T("%s in [%d.%03d sec][Total : %d, Convert : %d, Skip : %d]"), _T("Complete!"), (FINISH-START)/1000, (FINISH-START)%1000, nTotal, nConvert, nSkip);
	UpdateData(FALSE);
}

VOID CSetDMConnectionInfoDlg::OnbCLOSE() 
{
	CDialog::OnOK();
}

VOID CSetDMConnectionInfoDlg::SetCfgFile(VOID)
{
	UpdateData(TRUE);

	CString strDBKind;
	if (m_bCheck1 == TRUE) {	// MobileDB.CFG 변경
		m_cCombo1.GetWindowText(strDBKind);
		SetCfgFile(_T("..\\CFG\\MOBILE\\MobileDB.CFG"), strDBKind);
	}
	if (m_bCheck2 == TRUE) {	// MobileDB.CFG 변경
		m_cCombo2.GetWindowText(strDBKind);
		SetCfgFile(_T("..\\CFG\\MOBILE\\MainDB.cfg"), strDBKind);
	}
	if (m_bCheck3 == TRUE) {	// MobileDB.CFG 변경
		m_cCombo3.GetWindowText(strDBKind);
		SetCfgFile(_T("..\\CFG\\RefEditor\\refeditor_mbcs.cfg"), strDBKind);
	}
}

INT CSetDMConnectionInfoDlg::SetCfgFile(CString strFileName, CString strDBKind)
{
	m_strMsg1.Format(_T("%s [%s] 변경중"), strFileName, strDBKind);
	UpdateData(FALSE);

	CStdioFile file;
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		CString strMsg;
		strMsg.Format(_T("[%s]파일을 열수 없습니다."), strFileName);
		return -1;
	}

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CString strLine;

	// HOST
	strLine.Format(_T("HOST %s\r\n"), m_strCONNECTION_INFO);
	file.WriteString(strLine);

	// ID
	strLine.Format(_T("ID %s\r\n"), _T("이씨오"));
	file.WriteString(strLine);

	// SEQ
	strLine.Format(_T("SEQ %s\r\n"), _T("ESL_SEQ"));
	file.WriteString(strLine);

	strDBKind.MakeUpper();
	strDBKind.TrimLeft();		strDBKind.TrimRight();
	// DB
	strLine.Format(_T("DB %s\r\n"), strDBKind);
	file.WriteString(strLine);

	file.Close();

	m_strMsg1.Format(_T("%s [%s] 변경완료"), strFileName, strDBKind);
	UpdateData(FALSE);

	return 0;
}

VOID CSetDMConnectionInfoDlg::InitComboBoxControl(VOID)
{
	m_cCombo1.ResetContent();
	m_cCombo1.AddString(_T("Oracle"));
	m_cCombo1.AddString(_T("Mysql"));
	m_cCombo1.SelectString(0, _T("Mysql"));

	m_cCombo2.ResetContent();
	m_cCombo2.AddString(_T("Oracle"));
	m_cCombo2.AddString(_T("Mysql"));
	m_cCombo2.SelectString(0, _T("Oracle"));

	m_cCombo3.ResetContent();
	m_cCombo3.AddString(_T("Oracle"));
	m_cCombo3.AddString(_T("Mysql"));
	m_cCombo3.SelectString(0, _T("Oracle"));

	return;
}

HBRUSH CSetDMConnectionInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_MSG1) {
		pDC->SetTextColor(RGB(237, 108, 31));			// 주황색
		//pDC->SetTextColor(RGB(53, 155, 36));			// 연한 녹색
		return hbr;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_HELP1) {
		//pDC->SetTextColor(RGB(237, 108, 31));			// 주황색
		//pDC->SetTextColor(RGB(53, 155, 36));			// 연한 녹색
		pDC->SetTextColor(RGB(255, 0, 0));			// 빨강
		return hbr;
	}	

	return hbr;
}

VOID CSetDMConnectionInfoDlg::OnbMOBILE() 
{
	UpdateData(TRUE);

	m_strCONNECTION_INFO = _T("kolas2@k2up/kolas2#[127.0.0.1]");
	m_strCONNECTION_INFO_BEFORE = _T("");
	m_bDM = TRUE;
	m_bCheck1 = TRUE;			m_cCombo1.SelectString(0, _T("MYSQL"));
	m_bCheck2 = FALSE;			m_cCombo2.SelectString(0, _T("ORACLE"));
	m_bCheck3 = TRUE;			m_cCombo3.SelectString(0, _T("MYSQL"));

	UpdateData(FALSE);
}

VOID CSetDMConnectionInfoDlg::OnbDBCONNINFO() 
{
	UpdateData(TRUE);

	m_strCONNECTION_INFO = _T("kolas1@k2up/kolas1#[]");
	m_strCONNECTION_INFO_BEFORE = _T("kolas2@k2up/kolas2#[]");
	m_bDM = TRUE;
	m_bCheck1 = FALSE;			m_cCombo1.SelectString(0, _T("MYSQL"));
	m_bCheck2 = TRUE;			m_cCombo2.SelectString(0, _T("ORACLE"));
	m_bCheck3 = TRUE;			m_cCombo3.SelectString(0, _T("ORACLE"));

	UpdateData(FALSE);
}

