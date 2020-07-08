// GetComputerIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetComputerID.h"
#include "GetComputerIDDlg.h"

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
// CGetComputerIDDlg dialog

CGetComputerIDDlg::CGetComputerIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetComputerIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetComputerIDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_pMyInfo = NULL ;
}
CGetComputerIDDlg::~CGetComputerIDDlg()
{
	if(m_pMyInfo)
	{
		delete m_pMyInfo ;
		m_pMyInfo = NULL ;
	}
}

VOID CGetComputerIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetComputerIDDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetComputerIDDlg, CDialog)
	//{{AFX_MSG_MAP(CGetComputerIDDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bMAKEINFO, OnbMAKEINFO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetComputerIDDlg message handlers

BOOL CGetComputerIDDlg::OnInitDialog()
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
	m_strNetID  = _T("");
	m_strLibCode = _T("");
	m_strComputerName = _T("") ;


	m_pMyInfo = new CMyInfo ;


	GetMyComputerInfo() ;

	SetControlData();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CGetComputerIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CGetComputerIDDlg::OnPaint() 
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
HCURSOR CGetComputerIDDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CGetComputerIDDlg::GetMyComputerInfo()
{

	m_strNetID = m_pMyInfo->GetMyComputerNetID() ;	

	m_arrMANAGE_CODE.RemoveAll () ; 
    m_pMyInfo->GetManageCode (m_arrMANAGE_CODE ) ;

	
	m_strLibCode = m_pMyInfo->GetLibCode () ;


	return ;
}

VOID CGetComputerIDDlg::SetControlData()
{


	CWnd* pLibCode = (CWnd*)GetDlgItem (IDC_EDIT_LIBCODE) ;
	if(pLibCode)
	{
		pLibCode->SetWindowText (m_strLibCode);
	}

	CWnd* pNetID = (CWnd*)GetDlgItem (IDC_EDIT_ID) ;
	if(pNetID)
	{
		pNetID->SetWindowText (m_strNetID);
	}


	CComboBox * pManage = (CComboBox*)GetDlgItem (IDC_COMBO_MANAGE) ;
	if(pManage)
	{
		//pManage->SetWindowText (m_strLibCode);
		for(INT i = 0 ; i < m_arrMANAGE_CODE.GetSize () ;i++)
		{
			pManage->AddString (m_arrMANAGE_CODE.GetAt (i));

		}
		
		pManage->SelectString (0,_T("적용안함"));
	}

}

VOID CGetComputerIDDlg::OnbMAKEINFO() 
{
	// TODO: Add your control notification handler code here
	
	CString strMsg; 
	CString strManageCode ;

	CComboBox * pManage = (CComboBox*)GetDlgItem (IDC_COMBO_MANAGE) ;
	pManage->GetWindowText (strManageCode) ;


	strMsg.Format (_T("## 인증을 위한 사용자 정보 ######\r\n\r\n")
				   _T("도서관코드  = %s\r\n")
				   _T("COMPUTER_ID = %s\r\n")
				   _T("관리구분    = %s\r\n\r\n")
				   _T("#################################")
				   ,m_strLibCode,m_strNetID,strManageCode);


	CButton  *pWnd = (CButton*)GetDlgItem (IDC_EDIT_MSG) ;
	if(pWnd)
	{
		pWnd->SetWindowText (strMsg) ;

	}


}
