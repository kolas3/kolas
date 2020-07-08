// QueryLogMiningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QueryLogMining.h"
#include "QueryLogMiningDlg.h"

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
// CQueryLogMiningDlg dialog

CQueryLogMiningDlg::CQueryLogMiningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryLogMiningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQueryLogMiningDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQueryLogMiningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueryLogMiningDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQueryLogMiningDlg, CDialog)
	//{{AFX_MSG_MAP(CQueryLogMiningDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bStart, OnbStart)
	ON_BN_CLICKED(IDC_bOpenDlg, OnbOpenDlg)
	ON_BN_CLICKED(IDC_bOutputDlg, OnbOutputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueryLogMiningDlg message handlers

BOOL CQueryLogMiningDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQueryLogMiningDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQueryLogMiningDlg::OnPaint() 
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
HCURSOR CQueryLogMiningDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQueryLogMiningDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CQueryLogMiningDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CQueryLogMiningDlg::OnbStart() 
{
	CString strFileName;
	CString strStartWord;
	CString strEndWord;
	
	CStdioFile st;	

	( (CEdit*) GetDlgItem (IDC_eStartWord) )->GetWindowText(strStartWord);
	( (CEdit*) GetDlgItem (IDC_eEndWord) )->GetWindowText(strEndWord);
	( (CEdit*) GetDlgItem (IDC_eFileName) )->GetWindowText(strFileName);
	LoadDelWord();	
	LoadRplWord();
	
	BOOL bOpen = st.Open(strFileName, CStdioFile::modeRead | CFile::typeBinary);
	BOOL bWrite;
	
	bWrite = FALSE;
	if ( bOpen == TRUE) 
	{
		CString strLine, strTag, strData;
		TCHAR cUni;
		
		st.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			st.SeekToBegin();
		}
		while(TRUE) 
		{			
			if (st.ReadString(strLine) == FALSE) break;
			strLine.TrimLeft();				strLine.TrimRight();
			if (strLine.GetLength() == 0) continue;

			// Start구문 Find
			if ( strLine.Find(strStartWord,0) >= 0 )
			{
				bWrite = TRUE;
			}
			
			if ( bWrite ) 
			{
				QueryLog(strLine);
			}

			if ( strLine.Find(strEndWord,0) >= 0 )
			{
				bWrite = FALSE;
			}
		}

		st.Close();
	}

	AfxMessageBox(_T("완료"));

	return;
}


VOID CQueryLogMiningDlg::QueryLog(CString strLog)
{
	CString strFileName;
	CString strDelWord;
	CString strRplWord;

	( (CEdit*) GetDlgItem (IDC_eFileName2) )->GetWindowText(strFileName);

	if ( m_strDelWordArray.GetSize() > 0 )
	{
		for ( int i=0 ; i<m_strDelWordArray.GetSize() ; i++ )
		{
			strDelWord = m_strDelWordArray.GetAt(i);
			if ( strLog.Find(strDelWord,0) >= 0 )
			{
				return;
			}
		}
	}

	if ( m_strRplWordArray.GetSize() > 0 )
	{
		for ( int i=0 ; i<m_strRplWordArray.GetSize() ; i++ )
		{
			strRplWord = m_strRplWordArray.GetAt(i);
			if ( strLog.Find(strRplWord,0) >= 0 )
			{
				strLog.Replace(	strRplWord,_T(""));			
			}
		}
	}
	

	
	// 이전소스	
	FILE* fp;
	
	fp = _tfopen (strFileName, _T("a+b"));
	if (fp == NULL)
	{
		return ;
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	// KOLAS 2 UP UNICODE 수정작업
	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	// ------------------------------------------------------------------------------
	
	_ftprintf (fp, _T("%s\r\n"), strLog);
	
	fclose(fp);
}


VOID CQueryLogMiningDlg::LoadDelWord()
{
	CString strDelWord;
	CString strWord;
	( (CEdit*) GetDlgItem (IDC_eDelWord) )->GetWindowText(strDelWord);	

	m_strDelWordArray.RemoveAll();

	INT nOffset;
	INT nLength;

	nOffset=0;
	nLength=0;
	while (1)
	{
		nLength = strDelWord.Find(_T(","), nOffset);
		if ( nLength < 0 )
		{
			strWord = strDelWord.Mid(nOffset);
			strWord.TrimLeft();
			strWord.TrimRight();
			m_strDelWordArray.Add(strWord);	
			break;
		}
		else
		{
			strWord = strDelWord.Mid(nOffset, nLength-nOffset);
			strWord.TrimLeft();
			strWord.TrimRight();
			m_strDelWordArray.Add(strWord);	
			nOffset = nLength+1;
		}
	}
}

VOID CQueryLogMiningDlg::LoadRplWord()
{
	CString strRplWord;
	CString strWord;
	( (CEdit*) GetDlgItem (IDC_eRplWord) )->GetWindowText(strRplWord);	

	m_strRplWordArray.RemoveAll();

	INT nOffset;
	INT nLength;

	nOffset=0;
	nLength=0;
	while (1)
	{
		nLength = strRplWord.Find(_T(","), nOffset);
		if ( nLength < 0 )
		{
			strWord = strRplWord.Mid(nOffset);
			strWord.TrimLeft();
			strWord.TrimRight();
			m_strRplWordArray.Add(strWord);	
			break;
		}
		else
		{
			strWord = strRplWord.Mid(nOffset, nLength-nOffset);
			strWord.TrimLeft();
			strWord.TrimRight();
			m_strRplWordArray.Add(strWord);	
			nOffset = nLength+1;
		}
	}
}


void CQueryLogMiningDlg::OnbOpenDlg() 
{
	CString strPathName;
	CFileDialog dlg( TRUE, _T(""), _T("*.*"),
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("모든파일(*.*)|*.*|") );
	dlg.m_ofn.lpstrTitle = _T("첨부 파일 삽입");
	dlg.m_ofn.lStructSize;

	if(IDOK == dlg.DoModal () )
	{	
		strPathName = dlg.GetPathName();
		( (CEdit*) GetDlgItem (IDC_eFileName) )->SetWindowText(strPathName);

	}
}

void CQueryLogMiningDlg::OnbOutputDlg() 
{
	CFileDialog dlg(FALSE, _T("*.*"), _T(""),
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T("파일형식(*.txt)|All Files(*.*)|*.*||") );
	
	CString strPathName;
	if( dlg.DoModal() == IDOK) 
	{
		strPathName = dlg.GetPathName();
		( (CEdit*) GetDlgItem (IDC_eFileName2) )->SetWindowText(strPathName);
	}		
}
