// SocketTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketTest.h"
#include "SocketTestDlg.h"

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
// CSocketTestDlg dialog

CSocketTestDlg::CSocketTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsBusy = FALSE;
}

CSocketTestDlg::~CSocketTestDlg()
{
	if(m_pSocket)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}
}
void CSocketTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocketTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketTestDlg message handlers

BOOL CSocketTestDlg::OnInitDialog()
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
	m_pSocket = new CSocket;
	BOOL bResult = m_pSocket->Create();

	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	SetDlgItemText(IDC_EDIT1, _T("E00401000A998DD0"));
// 	m_pSocket->Connect(_T("127.0.0.1"), 10062);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketTestDlg::OnPaint() 
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
HCURSOR CSocketTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSocketTestDlg::OnOK() 
{
	// TODO: Add extra validation here

	
// 	char pChar[101];
// 	strcpy(pChar, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
// 
// 	pSocket->Send(pChar, 27);
// 
// 	CHAR lpBuf[MAX_PATH];
// 	pSocket->Receive(lpBuf, MAX_PATH);

// 	CDialog::OnOK();
}

void CSocketTestDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
}

void CSocketTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_MEMBER_CARD_OPEN"));

	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);
}

void CSocketTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CString strUID, strUserNo;

	GetDlgItemText(IDC_EDIT1, strUID);
	GetDlgItemText(IDC_EDIT3, strUserNo);

	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_MEMBER_STATUS"));	

	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), strUserNo);
	Writer.addElement(_T("UID"), strUID);

	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);
}

void CSocketTestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_LIBRARY"));

	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);
}

void CSocketTestDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_MEMBER_NO"));	

	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);

	CFrameReader Reader;
	Reader.parseFrame(strResult);
	CString strCommand = Reader.getCommand();
	if(_T("KL_MEMBER_NO") == strCommand)
	{
		CString strUserNo = Reader.getElement(0, 0);
		SetDlgItemText(IDC_EDIT3, strUserNo);
	}
}

void CSocketTestDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	CString strUID, strUserNo;

	GetDlgItemText(IDC_EDIT1, strUID);
	GetDlgItemText(IDC_EDIT3, strUserNo);
	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_MEMBER_WRTIE_RFID_UID"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), strUserNo);
	Writer.addElement(_T("UID"), strUID);

	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);
}

void CSocketTestDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_MEMBER_CARD_CLOSE"));	

	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);
}

INT CSocketTestDlg::Send(CFrameWriter& Writer, CString& strResult)
{	
	if(m_bIsBusy)
	{
		return 1000;
	}
	m_bIsBusy = TRUE;

	CString strFrame = Writer.getFrame();
	SetDlgItemText(IDC_EDIT4, strFrame);
	CHAR* szQuery = Writer.WideCharToUTF8(strFrame.GetBuffer(0));
	strFrame.ReleaseBuffer();

	CHAR szResult[10000];
	memset(szResult, 0x00, sizeof(szResult));
 	m_pSocket->Send(szQuery, strlen(szQuery));
	m_pSocket->Receive(szResult, 10000);

	delete []szQuery;
	szQuery = NULL;
	
	TCHAR* sztResult = Writer.UTF8ToWideChar(szResult);
	strResult.Format(_T("%s"), sztResult);
	delete []sztResult;
	sztResult = NULL;

	m_bIsBusy = FALSE;

	return 0;
}

void CSocketTestDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	m_pSocket->Close();
	m_pSocket->Create();
	BOOL bResult = m_pSocket->Connect(_T("127.0.0.1"), 10062);
	if(bResult)
	{
		SetDlgItemText(IDC_EDIT5, _T("접속성공!"));
	}
	else
	{
		SetDlgItemText(IDC_EDIT5, _T("접속실패!"));
	}
}

void CSocketTestDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	CString strTagType;
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(bCheck)
	{
		GetDlgItemText(IDC_RADIO1, strTagType);
	}
	
	bCheck = ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck();
	if(bCheck)
	{
		GetDlgItemText(IDC_RADIO2, strTagType);
	}


	CFrameWriter Writer(NULL);	
	Writer.setCommand(_T("KL_TAG_TYPE_SET"));	

	Writer.addRecord();
	Writer.addElement(_T("TYPE"), strTagType);
	
	CString strResult;
	Send(Writer, strResult);
	SetDlgItemText(IDC_EDIT2, strResult);
}
