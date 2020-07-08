// EFS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "efs.h"
#include "EFSReportDlg.h"

CEFSReportDlg m_EFSReportDlg = NULL;

INT HandleException(LPTSTR lpszFileName, INT nLineNumber, DWORD dwError)
{
	try
	{
		if (dwError > 0)
		{
			
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			
			m_EFSReportDlg.m_strErrMsg.Format(_T("[%u] : %s"), dwError, lpMsgBuf);

			LocalFree( lpMsgBuf );
		}

		m_EFSReportDlg.DumpScreen();

		m_EFSReportDlg.DumpException(lpszFileName, nLineNumber);

		m_EFSReportDlg.DoModal();
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

INT HandleException(CException *e, LPTSTR lpszFileName, INT nLineNumber)
{
	try
	{
		TCHAR szErrMsg[256];		
		e->GetErrorMessage(szErrMsg, 256);		
		if (_tcslen(szErrMsg) > 0)
			m_EFSReportDlg.m_strErrMsg.Format(_T("%s"), szErrMsg);
		
		e->Delete();
	}
	catch (...)
	{
		return -1;
	}
	
	return HandleException(lpszFileName, nLineNumber, 0);
}

INT HandleException(CEFSDMException *e, LPTSTR lpszFileName, INT nLineNumber)
{
	try
	{
		m_EFSReportDlg.m_strErrMsg = _T("데이터베이스와 작업을 하는중에 에러가 발생하였습니다.");

		m_EFSReportDlg.DumpDMException(e);
	}
	catch (...)
	{
		return -1;
	}

	return HandleException(static_cast<CException*>(e), lpszFileName, nLineNumber);
}

INT SetProgramPath(LPCTSTR lpszItemName, LPCTSTR lpszItemData)
{
	return m_EFSReportDlg.SetProgramPath(lpszItemName, lpszItemData);
}