// EBookViewMgr.cpp: implementation of the EBookViewMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EBookViewMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEBookViewMgr::CEBookViewMgr()
{

}

CEBookViewMgr::~CEBookViewMgr()
{

}

 // 
// szFileName : 실행시킬 파일의 full path 
// szParam : 프로그램 실행시 argument가 필요하다면 이곳에.... 
// 
INT CEBookViewMgr::ExecuteApp(CString strExeFile, CString strParam/*=NULL*/) 
{ 
	DWORD dwExitCode=0; 
//	STARTUPINFO si; 
//	PROCESS_INFORMATION pi; 
	
	//if(strExeFile.IsEmpty()) return -1;
	strExeFile = _T("\"") + strExeFile + _T("\"");
	LPTSTR szFileName = strExeFile.GetBuffer(0);
	LPTSTR szParam = NULL;
	if(!strParam.IsEmpty())
	{
		strParam = _T("\"") + strParam + _T("\"");
		szParam = strParam.GetBuffer(0);
	}
	ShellExecute(NULL, _T("open"), szParam, NULL, NULL, SW_SHOWNORMAL);
/*
	SHELLEXECUTEINFO sei;
	sei.cbSize = sizeof( SHELLEXECUTEINFO );
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.lpVerb = NULL;
	sei.lpFile = szFileName;
	//sei.lpFile = szParam;
	sei.lpParameters = szParam;
	//sei.lpParameters = NULL;
	sei.lpDirectory = NULL;
	sei.nShow = SW_SHOWNORMAL;
//	ShellExecute(NULL, _T("open"), szParam, NULL, NULL, SW_SHOWNORMAL);
	if(!ShellExecuteEx( &sei ))
	{
		return -1;
	}

/*	ZeroMemory( &si, sizeof(si) ); 
	si.cb = sizeof(si); 
	ZeroMemory( &pi, sizeof(pi) ); 

	// Start the child process. 
	if( !CreateProcess (    szFileName, // module name 
							szParam, //NULL, // Command line. 
							NULL,    // Process handle not inheritable. 
							NULL,    // Thread handle not inheritable. 
							FALSE,   // Set handle inheritance to FALSE. 
							0,       // No creation flags. 
							NULL,    // Use parent's environment block. 
							NULL,    // Use parent's starting directory. 
							&si,     // Pointer to STARTUPINFO structure. 
							&pi )    // Pointer to PROCESS_INFORMATION structure. 
	) 
	{ 
		// execution FAIL... 
		dwExitCode = 0; 
	} 
	else 
	{ 
		DWORD dwCode = WaitForSingleObject(pi.hProcess, INFINITE ); 

		// Wait until child process exits. 
		switch(dwCode) 
		{ 
		case WAIT_OBJECT_0: 
			TRACE(_T(" OK... 정상 종료 되었음.")); 
			// Close process and thread handles. 
			if (GetExitCodeProcess(pi.hProcess, &dwExitCode)) 
			{ 
			TRACE(_T(" dwExitCode 는 %d입니다. "),dwExitCode); 
			} 
			else 
			dwExitCode = 0; 

			break; 
		case WAIT_TIMEOUT: 
			TRACE(_T(" 타임 아웃 걸림. ")); 
			dwExitCode = 0; 
			break; 
		default: 
			dwExitCode = 0; 
			break; 
		} 

		CloseHandle( pi.hProcess ); 
		CloseHandle( pi.hThread ); 
	} 
*/
	return 0; 
} 
 
INT CEBookViewMgr::EBookView(CString strBookKey, CString strExeFile, CString strServerFile)
{
	CEBookUpNDownloadMgr mgr;
	CString strLocalFile;
	CFile LocalFile; 
	CFileException fileException;	
	INT ids ;
	if ( !LocalFile.Open( strServerFile.GetBuffer(0), CFile::modeRead | CFile::typeBinary, &fileException ))
	{
		ids = mgr.DownloadEBook(strBookKey, strServerFile, strLocalFile);
		if(ids < 0) return ids;
	}
	else
	{
		strLocalFile = strServerFile;
		LocalFile.Close();
	}
	ids = ExecuteApp(strExeFile, strLocalFile);	
	if(ids < 0) return ids;	
		
	return 0;
}

INT CEBookViewMgr::EBookViewByBookKey(CString strBookKey, CString strConnectionInfo)
{
	if(strBookKey.IsEmpty() || strConnectionInfo.IsEmpty()) return -1;

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(strConnectionInfo);
	CString strQuery;
	strQuery.Format(_T("SELECT EXECUTE_NAME, FILENAME FROM BO_EBOOK_TBL WHERE BOOK_KEY=%s"), strBookKey);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return ids;
	
	return EBookView(strBookKey, tmpDM.GetCellData(0, 0), tmpDM.GetCellData(0, 1));
}

INT CEBookViewMgr::DeleteEBookTempFolder()
{
	CEBookUpNDownloadMgr mgr;
	return mgr.DeleteEBookTempFolder();
}





















