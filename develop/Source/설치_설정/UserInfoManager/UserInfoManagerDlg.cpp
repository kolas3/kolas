// UserInfoManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserInfoManager.h"
#include "UserInfoManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CUserInfoManagerDlg dialog

CUserInfoManagerDlg::CUserInfoManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInfoManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInfoManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CUserInfoManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInfoManagerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUserInfoManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CUserInfoManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInfoManagerDlg message handlers

BOOL CUserInfoManagerDlg::OnInitDialog()
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
	
	InitControl();

	CFG.ReadCfgFile(_T(""));
	m_DM.SetCONNECTION_INFO(CFG.m_sConnection);		

	CString strID, strPASSWD;
	GetUserInfo(strID, strPASSWD);

	CString strQuery, strValue;

	strQuery.Format(_T("SELECT REC_KEY FROM MN_USER_TBL WHERE ID='%s' AND PASSWD='%s'"), strID, strPASSWD);
	m_DM.GetOneValueQuery(strQuery, strValue);

	if ( strValue==_T("") ) 
	{
		AfxMessageBox(_T("ID나 암호가 잘못되었습니다. 관리자에게 문의하십시오."));
		CDialog::OnCancel();
	}
	
	ShowUserInfo(strID, strPASSWD);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CUserInfoManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CUserInfoManagerDlg::OnPaint() 
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
HCURSOR CUserInfoManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


INT CUserInfoManagerDlg::GetUserInfo(CString &strID, CString &strPasswd )
{
	INT	idx;
	idx = m_strLoginInfo.Find(_T(" "), 0 );	

	if ( idx > 0 ) 
	{
		strID = m_strLoginInfo.Left(idx);
		strPasswd = m_strLoginInfo.Mid(idx+1);
	}
	else
	{
		strID = _T("");
		strPasswd = _T("");
	}

	return 0;
}


INT CUserInfoManagerDlg::ShowUserInfo(CString strID, CString strPasswd)
{
	CString strQuery;

	strQuery.Format(_T("SELECT DECODE(PERMITION, 'A', '관리자', '일반사용자'),ID,PASSWD,NAME,CIVIL_NO,( select DESCRIPTION FROM CD_CODE_TBL WHERE CODE=CLASS_NAME and CLASS=41),( select DESCRIPTION FROM CD_CODE_TBL WHERE CODE=DIVISION_NAME and CLASS=42),( select DESCRIPTION FROM CD_CODE_TBL WHERE CODE=MANAGE_CODE and CLASS=1),BUSINESS_TEL,EXCHANGE_TEL,E_MAIL,HOME_ZIP_CODE,HOME_TEL,HOME_ADDRESS, REC_KEY FROM MN_USER_TBL WHERE STATUS='Y' AND ID = '%s' AND PASSWD='%s'"), strID, strPasswd );
	m_DM.RestructDataManager(strQuery);
	
	( (CEdit*) GetDlgItem (IDC_ePERMITION) )->SetWindowText(m_DM.GetCellData(0,0));
	( (CEdit*) GetDlgItem (IDC_EDIT1) )->SetWindowText(m_DM.GetCellData(0,1));
	//( (CEdit*) GetDlgItem (IDC_EDIT2) )->SetWindowText(m_DM.GetCellData(0,2));
	( (CEdit*) GetDlgItem (IDC_EDIT4) )->SetWindowText(m_DM.GetCellData(0,3));
	( (CEdit*) GetDlgItem (IDC_EDIT5) )->SetWindowText(m_DM.GetCellData(0,4));
	( (CEdit*) GetDlgItem (IDC_EDIT12) )->SetWindowText(m_DM.GetCellData(0,5));
	( (CEdit*) GetDlgItem (IDC_EDIT15) )->SetWindowText(m_DM.GetCellData(0,6));
	( (CEdit*) GetDlgItem (IDC_EDIT16) )->SetWindowText(m_DM.GetCellData(0,7));
	( (CEdit*) GetDlgItem (IDC_EDIT11) )->SetWindowText(m_DM.GetCellData(0,8));
	( (CEdit*) GetDlgItem (IDC_EDIT13) )->SetWindowText(m_DM.GetCellData(0,9));
	( (CEdit*) GetDlgItem (IDC_EDIT14) )->SetWindowText(m_DM.GetCellData(0,10));
	( (CEdit*) GetDlgItem (IDC_EDIT7) )->SetWindowText(m_DM.GetCellData(0,11));
	( (CEdit*) GetDlgItem (IDC_EDIT8) )->SetWindowText(m_DM.GetCellData(0,12));
	( (CEdit*) GetDlgItem (IDC_EDIT9) )->SetWindowText(m_DM.GetCellData(0,13));	

	return 0;
}

VOID CUserInfoManagerDlg::OnOK() 
{
	CString strPasswd, strPasswd2;
	CString b_tel, b_ex_tel, e_mail, h_zip, h_tel, h_add;

	( (CEdit*) GetDlgItem(IDC_EDIT2) )->GetWindowText(strPasswd);
	( (CEdit*) GetDlgItem(IDC_EDIT3) )->GetWindowText(strPasswd2);

	
	if ( strPasswd.IsEmpty() || strPasswd2.IsEmpty() )
	{
		AfxMessageBox(_T("암호를 입력하세요"));
		return;
	}

	if ( strPasswd != strPasswd2 ) 
	{
		AfxMessageBox(_T("입력된 암호가 다릅니다. 다시 입력하여 주세요"));
		return;
	}

	if ( strPasswd.GetLength() <= 3 )
	{
		AfxMessageBox(_T("암호는 4자리 이상 입력하여 주세요"));
		return;
	}

	( (CEdit*) GetDlgItem(IDC_EDIT11) )->GetWindowText(b_tel);
	( (CEdit*) GetDlgItem(IDC_EDIT13) )->GetWindowText(b_ex_tel);
	( (CEdit*) GetDlgItem(IDC_EDIT14) )->GetWindowText(e_mail);
	( (CEdit*) GetDlgItem(IDC_EDIT7) )->GetWindowText(h_zip);
	( (CEdit*) GetDlgItem(IDC_EDIT8) )->GetWindowText(h_tel);
	( (CEdit*) GetDlgItem(IDC_EDIT9) )->GetWindowText(h_add);

	CString strQuery;

	m_DM.StartFrame () ;
	m_DM.InitDBFieldData () ;
	strQuery.Format(_T("UPDATE MN_USER_TBL SET PASSWD='%s', BUSINESS_TEL='%s', EXCHANGE_TEL='%s', E_MAIL='%s', HOME_ZIP_CODE='%s',HOME_TEL='%s',HOME_ADDRESS='%s' WHERE REC_KEY = %s;")
		, strPasswd, b_tel, b_ex_tel, e_mail, h_zip, h_tel, h_add, m_DM.GetCellData(0,14) );	
	m_DM.AddFrame(strQuery);	
	m_DM.SendFrame () ;
	m_DM.EndFrame () ;

	AfxMessageBox(_T("수정되었습니다."));
	
	CDialog::OnOK();
}

VOID CUserInfoManagerDlg::InitControl()
{
	( (CEdit*) GetDlgItem (IDC_EDIT2)	)->SetLimitText(8);
	( (CEdit*) GetDlgItem (IDC_EDIT3)		)->SetLimitText(8);
	( (CEdit*) GetDlgItem (IDC_EDIT11)		)->SetLimitText(25);
	( (CEdit*) GetDlgItem (IDC_EDIT13)		)->SetLimitText(25);
	( (CEdit*) GetDlgItem (IDC_EDIT14)		)->SetLimitText(70);
	( (CEdit*) GetDlgItem (IDC_EDIT7)		)->SetLimitText(7);
	( (CEdit*) GetDlgItem (IDC_EDIT8)		)->SetLimitText(200);
	( (CEdit*) GetDlgItem (IDC_EDIT9)		)->SetLimitText(25);
}

BOOL CUserInfoManagerDlg::PreTranslateMessage(MSG* pMsg) 
{	
	return CDialog::PreTranslateMessage(pMsg);
}
