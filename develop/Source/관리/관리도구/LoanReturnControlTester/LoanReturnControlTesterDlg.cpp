// LoanReturnControlTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanReturnControlTester.h"
#include "LoanReturnControlTesterDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
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
// CLoanReturnControlTesterDlg dialog

CLoanReturnControlTesterDlg::CLoanReturnControlTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoanReturnControlTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanReturnControlTesterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsBusy = FALSE;
	m_nPort = 0;
}

CLoanReturnControlTesterDlg::~CLoanReturnControlTesterDlg()
{
}
void CLoanReturnControlTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanReturnControlTesterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoanReturnControlTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanReturnControlTesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_btnUserSearch, OnbtnUserSearch)
	ON_BN_CLICKED(IDC_btnLoan, OnbtnLoan)
	ON_BN_CLICKED(IDC_btnReturn, OnbtnReturn)
	ON_BN_CLICKED(IDC_btnClear, OnbtnClear)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanReturnControlTesterDlg message handlers

BOOL CLoanReturnControlTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.


	// IDM_ABOUTBOX must be in the system command range.
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
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_edtPORT, _T("31001"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoanReturnControlTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoanReturnControlTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoanReturnControlTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoanReturnControlTesterDlg::OnOK() 
{
	// TODO: Add extra validation here

 	CDialog::OnOK();
}

void CLoanReturnControlTesterDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
}

INT CLoanReturnControlTesterDlg::Send(CFrameWriter& Writer, CString& strResult)
{	
	if(m_bIsBusy) {
		return 1000;
	}
	m_bIsBusy = TRUE;

	CString strFrame = Writer.getFrame();
	SetDlgItemText(IDC_edtRequest, strFrame);
	CHAR* szQuery = Writer.WideCharToUTF8(strFrame.GetBuffer(0));
	strFrame.ReleaseBuffer();

	CHAR szResult[10000];
	memset(szResult, 0x00, sizeof(szResult));

	CSocket* pSocket = new CSocket;
	pSocket->Create();
	pSocket->Connect(_T("127.0.0.1"), m_nPort);
	pSocket->Send(szQuery, strlen(szQuery));
	pSocket->Receive(szResult, 10000);
	pSocket->Close();
	delete pSocket;
	pSocket = NULL;

	delete []szQuery;
	szQuery = NULL;
	
	TCHAR* sztResult = Writer.UTF8ToWideChar(szResult);
	strResult.Format(_T("%s"), sztResult);
	if(TRUE == strResult.IsEmpty()) strResult = _T("null");

	delete []sztResult;
	sztResult = NULL;

	m_bIsBusy = FALSE;

	return 0;
}

void CLoanReturnControlTesterDlg::OnbtnUserSearch() 
{
	if (0 > CheckPort()) return;

	CString strUserNo;
	GetDlgItemText(IDC_edtUSER_NO, strUserNo);
	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("LRC_001_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("대출자번호"), strUserNo);
	
	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_edtResponse, strResult);
}

void CLoanReturnControlTesterDlg::OnbtnLoan() 
{
	if (0 > CheckPort()) return;
	
	SetDlgItemText(IDC_edtResponse, _T(""));

	CString strMsg, strRegNo, strUserNo;
	int len = 0;
	CEdit* pEdtRegNo = (CEdit*)GetDlgItem(IDC_edtREG_NO);
	for (INT i = 0; i < pEdtRegNo->GetLineCount(); i++) {
		len = pEdtRegNo->LineLength(pEdtRegNo->LineIndex(i));
		pEdtRegNo->GetLine(i, strRegNo.GetBuffer(len), len);
		strRegNo.ReleaseBuffer(len);
		if(strRegNo.IsEmpty()) {
			strMsg.Format(_T("---------%d. 등록번호 : null -------"), i+1);
			AddResult(strMsg);
			continue;
		}
		strMsg.Format(_T("---------%d. 등록번호 : %s -------"), i+1, strRegNo);
		AddResult(strMsg);

		GetDlgItemText(IDC_edtUSER_NO, strUserNo);
		CFrameWriter Writer(NULL);	
		Writer.setCommand(_T("LRC_002_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("대출자번호"), strUserNo);
		Writer.addElement(_T("등록번호"), strRegNo);
		
		CString strResult;
		Send(Writer, strResult);
		AddResult(strResult);
	}
}

void CLoanReturnControlTesterDlg::OnbtnReturn() 
{
	if (0 > CheckPort()) return;
	
	SetDlgItemText(IDC_edtResponse, _T(""));

	CString strMsg, strRegNo, strUserNo;
	int len = 0;
	CEdit* pEdtRegNo = (CEdit*)GetDlgItem(IDC_edtREG_NO);
	for (INT i = 0; i < pEdtRegNo->GetLineCount(); i++) {
		len = pEdtRegNo->LineLength(pEdtRegNo->LineIndex(i));
		pEdtRegNo->GetLine(i, strRegNo.GetBuffer(len), len);
		strRegNo.ReleaseBuffer(len);
		if(strRegNo.IsEmpty()) {
			strMsg.Format(_T("---------%d. 등록번호 : null -------"), i+1);
			AddResult(strMsg);
			continue;
		}
		strMsg.Format(_T("---------%d. 등록번호 : %s -------"), i+1, strRegNo);
		AddResult(strMsg);
		
		GetDlgItemText(IDC_edtUSER_NO, strUserNo);
		CFrameWriter Writer(NULL);	
		Writer.setCommand(_T("LRC_003_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("대출자번호"), strUserNo);
		Writer.addElement(_T("등록번호"), strRegNo);
		
		CString strResult;
		Send(Writer, strResult);
		AddResult(strResult);
	}
}


void CLoanReturnControlTesterDlg::OnbtnClear() 
{
	if (0 > CheckPort()) return;

	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("LRC_004_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("기능"), _T("반납목록초기화"));

	CString strResult;
	Send(Writer, strResult);
	AddResult(strResult);
}

INT CLoanReturnControlTesterDlg::CheckPort()
{
	CString strPort;
	GetDlgItemText(IDC_edtPORT, strPort);
	if(strPort.IsEmpty()) {
		MessageBox(_T("PORT를 입력하세요."));
		return -1;
	}
	m_nPort = _ttoi(strPort);
	if(1 > m_nPort) {
		MessageBox(_T("PORT를 정확히 입력하세요."));
		return -2;
	}
	return 0;
}

void CLoanReturnControlTesterDlg::AddResult(CString msg)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtResponse);
	int len = pEdit->GetWindowTextLength();
	pEdit->SetSel(len, len);
	pEdit->ReplaceSel(msg);
	
	len = pEdit->GetWindowTextLength();
	pEdit->SetSel(len, len);
	pEdit->ReplaceSel(_T("\r\n"));
}