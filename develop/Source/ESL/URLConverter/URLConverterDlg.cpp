// URLConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "URLConverter.h"
#include "URLConverterDlg.h"

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
// CURLConverterDlg dialog

CURLConverterDlg::CURLConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CURLConverterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CURLConverterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// KOL.UDF.022 시큐어코딩 보완
	m_filePath = _T("");
	m_fileContents.RemoveAll();
	m_urlInfo = NULL;
	
}

VOID CURLConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CURLConverterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CURLConverterDlg, CDialog)
	//{{AFX_MSG_MAP(CURLConverterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLConverterDlg message handlers

BOOL CURLConverterDlg::OnInitDialog()
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
	
	SetDefaultInfo();	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CURLConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CURLConverterDlg::OnPaint() 
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
HCURSOR CURLConverterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CURLConverterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		if( !ShowURLList() )
			PostMessage( WM_CLOSE );
}

bool CURLConverterDlg::ShowURLList()
{
	if( !GetURLList() )
	{
		AfxMessageBox( _T("ERROR :objectList.cfg 파일을 찾을 수 없습니다.") );
		return false;
	}
	
	const INT cnt = 8;
	UINT edtID[ cnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->SetWindowText( m_urlInfo[ i ].url );
	}

	return true;
}

VOID CURLConverterDlg::SetDefaultInfo()
{
	m_filePath.Format( _T("..\\Cfg\\UserMain\\objectList.cfg") );
	const INT cnt = 8;

	m_urlInfo = NULL;
	m_urlInfo = new URLInfo[ cnt ];

	CString title[ 8 ] = 
		{ _T("홈"), _T("공지사항"), _T("도서관안내"), _T("도서검색"), _T("신착자료보기"), _T("비치희망신청"), _T("국가자료공동목록검색"), _T("도움말") };

	for( INT i = 0; i < cnt; i++ )
		m_urlInfo[ i ].title = title[ i ];
}

VOID CURLConverterDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	if( m_urlInfo )
		delete [] m_urlInfo;
	
	CDialog::OnClose();
}

bool CURLConverterDlg::GetURLList()
{
	CStdioFile fp;
	fp.Open( m_filePath,  CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( !fp )
		return false;
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}
	
	m_fileContents.RemoveAll();
	
	CString row, tmp;
	INT idx = 0, i = 0, index = 0;
	const INT cnt = 8;
	bool flag = false;
	
	while( fp.ReadString( row ) )
	{
		idx++;
		m_fileContents.AddTail( row );

		row.Replace( _T(" "), _T("") );
		if( !row.GetLength() || _T(".") == row.Left( 1 ) )
			continue;
		
		if( !flag )
			for( i = 0; i < cnt; i++ )
			{
				if( row.Find( m_urlInfo[ i ].title ) > -1 )
				{
					index = i;
					flag = true;
				}
			}
		else if( flag )
			if( row.Find( _T("URL=") ) > -1 )
			{
				row.Replace( _T("URL="), _T("") );

				m_urlInfo[ index ].row = idx;
				m_urlInfo[ index ].url.Format( _T("%s"), row );
				flag = false;
			}
	}
	
	return true;
}

VOID CURLConverterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	SetModifiedURLInfo();
	SaveURLInfoToFile();
	
	CDialog::OnOK();
}

VOID CURLConverterDlg::SetModifiedURLInfo()
{
	const INT cnt = 8;
	UINT edtID[ cnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( m_urlInfo[ i ].url );
	}	
}

VOID CURLConverterDlg::SaveURLInfoToFile()
{
	FILE *fp;
	fp = _tfopen( m_filePath, _T("w+b") );
	
	POSITION pos = m_fileContents.GetHeadPosition();
	CString row;

	INT idx = 0;
	const INT cnt = 8;
	while( pos )
	{
		row = m_fileContents.GetNext( pos );
		idx++;

		for( INT i = 0; i < cnt; i++ )
			if( idx == m_urlInfo[ i ].row )
				row.Format( _T("URL=%s"), m_urlInfo[ i ].url );
		
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
		_ftprintf( fp, _T("%s\r\n"), row );
	}

	fclose( fp );
}
