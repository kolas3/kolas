// Kalendar_LoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar_Loader.h"
#include "Kalendar_LoaderDlg.h"
#include <process.h>
#include <errno.h>

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKalendar_LoaderDlg dialog

CKalendar_LoaderDlg::CKalendar_LoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKalendar_LoaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKalendar_LoaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CKalendar_LoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKalendar_LoaderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKalendar_LoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CKalendar_LoaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKalendar_LoaderDlg message handlers

BOOL CKalendar_LoaderDlg::OnInitDialog()
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
	//return TRUE;

	CStdioFile file;
	if (!file.Open(_T("..\\cfg\\path.cfg"), CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite))
	{
//		AfxMessageBox(_T("PATH 파일이 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CheckNewPatchFile(_T("..\\DLL\\COMMON\\OCIMgr.DLL"));

	CString strLine;
	while (file.ReadString(strLine))
	{
		if (strLine.Left(1) == _T(".") || strLine.IsEmpty())
			continue;

// 		TCHAR tch[1026];
// 		
// 		_stprintf(tch, _T("%s;%s"), strLine.GetBuffer(0), _tgetenv(_T("PATH")));
// 		_tputenv(tch);
		
		TCHAR* szPath = _tgetenv(_T("PATH"));
		INT nSize = _tcslen(szPath) + strLine.GetLength() + 2;
		TCHAR* szNewPath = new TCHAR[nSize];
		memset(szNewPath, 0x00, nSize);
		_stprintf(szNewPath, _T("%s;%s"), strLine.GetBuffer(0), szPath);
		strLine.ReleaseBuffer();
		_tputenv(szNewPath);
		delete []szNewPath;
		szNewPath = NULL;

		ApplyOtherEnvVariables();
		
		CString strCmd;
		INT ids;
		
		if ( m_strParam.IsEmpty() )
		{
			ids = ::WinExec("Kalendar.EXE", SW_SHOW);	
		}
		else
		{
			strCmd.Format(_T("Kalendar.EXE %s"), m_strParam); 
			
			int len; 
			len = WideCharToMultiByte(CP_ACP, 0, strCmd, -1, NULL, 0, NULL,NULL);
			char* ctmp = new char[len]; 
			// 실제 변환
			WideCharToMultiByte(CP_ACP, 0, strCmd, -1, ctmp, len, NULL, NULL);
			//strCmd.Format();
			ids = ::WinExec(ctmp, SW_SHOW);
			delete []ctmp;		
		}
		
		/*
		if ( m_strParam.IsEmpty() )
		{
			ids = ::WinExec("Kalendar.exe", SW_SHOW);	
		}
		else
		{
			strCmd.Format(_T("Kalendar.exe %s"), m_strParam); 
			
			int len; 
			len = WideCharToMultiByte(CP_ACP, 0, strCmd, -1, NULL, 0, NULL,NULL);
			char* ctmp = new char[len]; 
			// 실제 변환
			WideCharToMultiByte(CP_ACP, 0, strCmd, -1, ctmp, len, NULL, NULL);
			//strCmd.Format();
			ids = ::WinExec(ctmp, SW_SHOW);
			delete []ctmp;		
		}
		*/
		
		// If the WinExec function succeeds, the return value is greater than 31.
		if (ids <= 31)
		{
			CString sTemp;
			sTemp.Format(_T("프로그램을 실행하던중 예상치 못한 에러가 발생하였습니다. [%d]"), ids);
			AfxMessageBox(sTemp);
		}
		break;
	}

	EndDialog(IDCANCEL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CKalendar_LoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CKalendar_LoaderDlg::OnPaint() 
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
HCURSOR CKalendar_LoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CKalendar_LoaderDlg::OnOK() 
{
	return;
}

INT CKalendar_LoaderDlg::CheckNewPatchFile(CString strFileName)
{
	CString strPatchFile, strBackupFile;
	CTime t = CTime::GetCurrentTime();
	strPatchFile.Format(_T("%s_patch"), strFileName);
	strBackupFile.Format(_T("%s_%04d%02d%02d"), strFileName, t.GetYear(), t.GetMonth(), t.GetDay());

	CFileFind fd;
	if (fd.FindFile(strPatchFile) == FALSE) {
		return -1;
	}
	fd.Close();

	INT ids;
	CString strMsg;
	// 현재의 파일을 백업한다.
	ids = MoveFile(strFileName, strBackupFile);
	if (ids == 0) {	// Error 상황
		strMsg.Format(_T("기존 파일[%s]를 백업 하던중 에러가 발생하였습니다.[%d][%d]"), strFileName, ids, GetLastError());
		return -1;
	}

	// 패치 파일의 바꿔친다.
	ids = MoveFile(strPatchFile, strFileName);
	if (ids == 0) {	// Error 상황
		strMsg.Format(_T("새 파일[%s]를 복원 하던중 에러가 발생하였습니다.[%d][%d]"), strFileName, ids, GetLastError());
		return -1;
	}

	return 0;
}

INT CKalendar_LoaderDlg::ApplyOtherEnvVariables(VOID)
{
	// 2004. 1. 16 최원종 추가 기능
	// _putenv() Remarks ============================================================================================
	// The _putenv function adds new environment variables or modifies the values of existing 
	// environment variables. Environment variables define the environment in which a process 
	// executes (for example, the default search path for libraries to be linked with a program). 
	// _wputenv is a wide-character version of _putenv; the envstring argument to _wputenv is a wide-character string.
	// ==============================================================================================================

	CStdioFile fd;
	if (fd.Open(_T("..\\CFG\\ENVVAR.CFG"), CStdioFile::modeRead | CFile::typeBinary) == FALSE) return -1;

	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString strLine;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;

		strLine.TrimLeft();		strLine.TrimRight();
		if (strLine.GetLength() == 0) continue;
		if (strLine.GetAt(0) == '#') continue;
		if (strLine.GetAt(0) == '.') continue;

		_tputenv(strLine.GetBuffer(0));
	}

	fd.Close();

	return 0;
}	