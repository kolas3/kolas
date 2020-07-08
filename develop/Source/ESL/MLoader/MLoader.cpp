// MLoader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MLoader.h"
#include "MLoaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMLoaderApp

BEGIN_MESSAGE_MAP(CMLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CMLoaderApp)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMLoaderApp construction

CMLoaderApp::CMLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMLoaderApp object

CMLoaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMLoaderApp initialization

BOOL CMLoaderApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should _tremove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	ProcLoad(m_lpCmdLine);

	return FALSE;

	CMLoaderDlg dlg;
	m_pMainWnd = &dlg;
	INT nResponse = IDOK;//dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

INT CMLoaderApp::ProcLoad(const TCHAR *lpCmdLine)
{
	if (lpCmdLine == NULL) return -1;

	CStdioFile file;
	if( !file.Open(_T("..\\cfg\\path.cfg"), CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite) )
	{
		AfxMessageBox(_T("PATH 파일이 없습니다."));
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine;
	while( file.ReadString(strLine) )
	{
		if( strLine.Left(1) == _T(".") || strLine.IsEmpty() )		continue;

// 		TCHAR tch[1026];
// 		
// 		_stprintf(tch, _T("%s;%s"), strLine.GetBuffer(0), _tgetenv(_T("PATH")));
// 		
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
		
#ifdef _UNICODE
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, lpCmdLine, -1, NULL, 0, NULL,NULL);
		char* ctmp = new char[len]; 
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, lpCmdLine, -1, ctmp, len, NULL, NULL);
		INT ids = ::WinExec(ctmp, SW_SHOW);
		delete []ctmp;
#else		
		INT ids = ::WinExec(lpCmdLine, SW_SHOW);
#endif
		
		if(ids <= 31)
		{
			CString sTemp;
			sTemp.Format(_T("프로그램을 실행하던중 예상치 못한 에러가 발생하였습니다. [%d]"), ids);
			AfxMessageBox(sTemp);
		}
		
		break;
	}

	return 0;
}

INT CMLoaderApp::ApplyOtherEnvVariables(VOID)
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
