// DataProcessingThread.cpp : implementation file
//

#include "stdafx.h"
#include "DataProcessingThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataProcessingThread

IMPLEMENT_DYNCREATE(CDataProcessingThread, CWinThread)

CDataProcessingThread::CDataProcessingThread()
{
	// kill event starts out in the signaled state
    m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CDataProcessingThread::~CDataProcessingThread()
{
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CDataProcessingThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (WaitForSingleObject(m_hEventKill, 0) != WAIT_TIMEOUT)
	{
		// event is signaled
		return FALSE;
	}
	
	// create dummy window as parent for the progress dialog
	if (!m_wndDummy.CreateEx(0, AfxRegisterWndClass(0), _T("CWnd Dummy Window"),
			WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
		return FALSE;

	// Create the progress dialog box.
//	if (!m_ProcessDlg.Create(&m_wndDummy, m_hEventKill))
//		return FALSE;
	
	// wait until dialog is created
	WaitForProcessingDlg();

	// avoid entering standard message loop by returning FALSE
    return FALSE;
}

INT CDataProcessingThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
    m_ProcessDlg.DestroyWindow();
	m_wndDummy.DestroyWindow();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDataProcessingThread, CWinThread)
	//{{AFX_MSG_MAP(CDataProcessingThread)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(TM_SETPROCESSMSG, SetProcessMsg)
	ON_MESSAGE(TM_SETPROGRESSINIT, SetProgressRange)
	ON_MESSAGE(TM_SETPROGRESSPOS, SetProgressPos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataProcessingThread message handlers
/********************************************************************/

VOID CDataProcessingThread::WaitForProcessingDlg()
{
	if(m_ProcessDlg.m_hWnd == NULL)
	{
		while(m_ProcessDlg.m_hWnd == NULL)
		{
			DoEvents();
		}
	}
	if(!::IsWindow(m_ProcessDlg.m_hWnd))
	{
		while(!::IsWindow(m_ProcessDlg.m_hWnd))
		{
			DoEvents();
		}
	}
}

VOID CDataProcessingThread::DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

VOID CDataProcessingThread::KillThread()
{
    // reset the m_hEventKill which signals the thread to shutdown
    VERIFY(SetEvent(m_hEventKill));

	DWORD dwResult;
	// make sure it is running
	while (dwResult = ResumeThread() > 1)
	{
		if (dwResult == 0xFFFFFFFF)
			break;
	}

    // allow thread to run at higher priority during kill process
    SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
    WaitForSingleObject(m_hEventDead, INFINITE);
    WaitForSingleObject(m_hThread, INFINITE);

    // now delete CWinThread object since it's no longer necessary
    delete this;
}

VOID CDataProcessingThread::Delete()
{
    // calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}
VOID CDataProcessingThread::SetProcessMsg(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszData = (LPCTSTR)lParam;
	m_ProcessDlg.SetMessage(lpszData);
}

VOID CDataProcessingThread::SetProgressRange(WPARAM wParam, LPARAM lParam)
{
	m_ProcessDlg.GetProgressCtrl()->SetRange(wParam, lParam);
}

VOID CDataProcessingThread::SetProgressPos(WPARAM wParam, LPARAM lParam)
{
	m_ProcessDlg.GetProgressCtrl()->SetPos(wParam);
}