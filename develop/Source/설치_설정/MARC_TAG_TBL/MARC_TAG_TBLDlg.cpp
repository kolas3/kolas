// MARC_TAG_TBLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MARC_TAG_TBL.h"
#include "MARC_TAG_TBLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_DataMgr.h"

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
// CMARC_TAG_TBLDlg dialog

CMARC_TAG_TBLDlg::CMARC_TAG_TBLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMARC_TAG_TBLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_TAG_TBLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CMARC_TAG_TBLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_TAG_TBLDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMARC_TAG_TBLDlg, CDialog)
	//{{AFX_MSG_MAP(CMARC_TAG_TBLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bCONVERT, OnbCONVERT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_TAG_TBLDlg message handlers

BOOL CMARC_TAG_TBLDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CMARC_TAG_TBLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CMARC_TAG_TBLDlg::OnPaint() 
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
HCURSOR CMARC_TAG_TBLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CMARC_TAG_TBLDlg::OnbCONVERT() 
{
	INT ids;
	CString strQuery = _T("SELECT PK, DESCRIP FROM MARC_TAG_TBL");
	CESL_DataMgr dm;
	dm.m_dbid = _T("kolas2");		dm.m_dbpwd = _T("kolas2#");		dm.m_dbservice = _T("KSC5601");

	ids = dm.RestructDataManager(strQuery);
	INT nRows = dm.GetRowCount();
	CString strPK, strData;

	dm.StartFrame();
	CString str;
	for (INT i = 0; i < nRows; i++) {
		strPK = _T("");
		strData = _T("");
		strQuery = _T("");

		strPK = dm.GetCellData(i, 0);
		strData = dm.GetCellData(i, 1);
		strData.Replace(_T("<BR>"), _T("\r\n"));

		strQuery.Format(_T("UPDATE MARC_TAG_TBL SET DESCRIP='%s' WHERE PK = %s;"), strData, strPK);
		dm.AddFrame(strQuery);
		if (i % 10 == 0) {
			ids = dm.SendFrame();
			if (ids != 0) {
				AfxMessageBox(_T("Send Frame Error!"));
				return;
			}
			dm.EndFrame();
			dm.StartFrame();
		}
		str.Format(_T("%d/%d"), i+1, nRows);
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
	}
	ids = dm.SendFrame();
	if (ids != 0) {
		AfxMessageBox(_T("Send Frame Error!"));
		return;
	}
	dm.EndFrame();
	dm.StartFrame();
}
