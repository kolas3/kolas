// MarcAdjThread.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjThread.h"

#include "MarcAdjMgr.h"

#include "MarcAdjDoc.h"
#include "MarcAdjMarcEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjThread

IMPLEMENT_DYNCREATE(CMarcAdjThread, CWinThread)

CMarcAdjThread::CMarcAdjThread()
{
	m_pFrame = NULL;
	m_pMarcAdjMgr = NULL;
	m_pParentWnd = NULL;
	m_pMainWnd = NULL;
}

CMarcAdjThread::~CMarcAdjThread()
{
}

BOOL CMarcAdjThread::InitInstance()
{
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CMarcAdjFrame);
	m_pFrame = static_cast<CMarcAdjFrame*>(pRuntimeClass->CreateObject());
	if (m_pFrame == NULL) return FALSE;

	m_pFrame->SetMarcAdjMgr(m_pMarcAdjMgr);

	pRuntimeClass = RUNTIME_CLASS(CMarcAdjDoc);
	CMarcAdjDoc *pDoc = static_cast<CMarcAdjDoc*>(pRuntimeClass->CreateObject());

	CCreateContext createContext;
	createContext.m_pCurrentFrame = m_pFrame;
	createContext.m_pNewViewClass = RUNTIME_CLASS(CMarcAdjMarcEditView);
	createContext.m_pCurrentDoc = pDoc;

	m_pFrame->Create(NULL, NULL, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_VISIBLE, CRect(0,0,700,700), m_pParentWnd, NULL, 0, &createContext);

	m_pFrame->InitialUpdateFrame(pDoc, TRUE);
	m_pMainWnd = m_pFrame;

	CString strFileName;
 	DWORD dwPID = ::GetCurrentProcessId();		
 	strFileName.Format(_T("..\\cfg\\TouchPad\\TOOLH(%u).dat"), dwPID);
 
 	CStdioFile sf;
 	CString strHandle;
 	strHandle.Format(_T("%d"), m_pFrame->GetSafeHwnd());
 
 	if(TRUE == sf.Open(strFileName, CFile::modeCreate|CFile::modeWrite))
 	{	
 		sf.WriteString(strHandle);	
 		sf.Close();	
 	}

	return TRUE;
}

INT CMarcAdjThread::ExitInstance()
{
 	CString strFileName;
 	DWORD dwPID = ::GetCurrentProcessId();		
 	strFileName.Format(_T("..\\cfg\\TouchPad\\TOOLH(%u).dat"), dwPID); 
 	::DeleteFile(strFileName);

	if (m_pParentWnd && m_pParentWnd->GetSafeHwnd())
	{
		m_pParentWnd->EndModalState();
		m_pParentWnd->SetForegroundWindow();
		m_pParentWnd->PostMessage(WM_END_MARC_ADJUST);
	}

	if (m_pFrame && m_pFrame->GetSafeHwnd())
	{
		if (::IsWindow(m_pFrame->GetSafeHwnd()))
		{
			m_pFrame->SendMessage(WM_CLOSE, 0, 0);
		}
		m_pFrame = NULL;
		m_pMainWnd = NULL;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMarcAdjThread, CWinThread)
	//{{AFX_MSG_MAP(CMarcAdjThread)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjThread Operations

INT CMarcAdjThread::StartThread(CMarcAdjMgr *pParentMarcAdjMgr, CWnd *pParetWnd)
{
	if (!pParentMarcAdjMgr) return -1;

	m_pMarcAdjMgr = pParentMarcAdjMgr;
	m_pParentWnd = pParetWnd;

	if (m_hThread == NULL)
	{
		if (CreateThread())
			return 0;
		else return -1;
	}
	return -1;
}

INT CMarcAdjThread::EndThread()
{
	if (!m_hThread) return 0;
	PostThreadMessage(WM_QUIT, 0, 0);
	if (m_hThread)
		::WaitForSingleObject(m_hThread,INFINITE);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjThread message handlers

CWnd* CMarcAdjThread::GetMainWnd() 
{
	//return m_pFrame;
	return CWinThread::GetMainWnd();
}