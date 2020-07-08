// ErrorAdjustThread.cpp : implementation file
//

#include "stdafx.h"
#include "ErrorAdjustThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustThread

IMPLEMENT_DYNCREATE(CErrorAdjustThread, CWinThread)

CErrorAdjustThread::CErrorAdjustThread()
{
	m_pErrorAdjustMgr = NULL;	
	// Frame
	m_pFrame = NULL;
	// Parent Frame
	m_pParentWnd = NULL;
}

CErrorAdjustThread::~CErrorAdjustThread()
{
}

BOOL CErrorAdjustThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CErrorAdjustFrame);
	m_pFrame = static_cast<CErrorAdjustFrame*>(pRuntimeClass->CreateObject());
	if (m_pFrame == NULL) return FALSE;
	
	// Set Mgr
	m_pFrame->SetErrorAdjustMgr(m_pErrorAdjustMgr);
	
	// Document
	pRuntimeClass = RUNTIME_CLASS(CErrorAdjustDoc);
	CErrorAdjustDoc *pDoc = static_cast<CErrorAdjustDoc*>(pRuntimeClass->CreateObject());
	
	// CreateContext
	CCreateContext createContext;
	createContext.m_pCurrentFrame = m_pFrame;
	createContext.m_pNewViewClass = RUNTIME_CLASS(CErrorAdjustFormView);
	createContext.m_pCurrentDoc = pDoc;
	
	// Create Frame
	m_pFrame->Create(NULL, _T("KOLAS II UP/LITE  오류수정"), WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_VISIBLE, CRect(0,0,700,700), NULL, NULL, 0, &createContext);
	
	// Init Frame
	m_pFrame->InitialUpdateFrame(pDoc, TRUE);
	
	m_pMainWnd = m_pFrame;
	
	return TRUE;
}

INT CErrorAdjustThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if (m_pParentWnd && m_pParentWnd->GetSafeHwnd())
	{
		m_pParentWnd->EndModalState();
		m_pParentWnd->SetForegroundWindow();
	}
	
	if (m_pFrame && m_pFrame->GetSafeHwnd())
	{
		if (::IsWindow(m_pFrame->GetSafeHwnd()))
		{
			m_pFrame->SendMessage(WM_CLOSE, 0, 0);
		}
	}
	
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CErrorAdjustThread, CWinThread)
	//{{AFX_MSG_MAP(CErrorAdjustThread)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustThread message handlers

INT CErrorAdjustThread::EndThread()
{
	PostThreadMessage(WM_QUIT, 0, 0);
	::WaitForSingleObject(m_hThread,INFINITE);
	
	return 0;
}

CWnd* CErrorAdjustThread::GetMainWnd() 
{
	// TODO: Add your specialized code here and/or call the base class
	//return m_pFrame;
	
	return CWinThread::GetMainWnd();
}

INT CErrorAdjustThread::StartThread(CErrorAdjustMgr *pParentErrorAdjustMgr, CWnd *pParentWnd)
{
	if (!pParentErrorAdjustMgr) return -1;
	
	m_pErrorAdjustMgr = pParentErrorAdjustMgr;
	m_pParentWnd = pParentWnd;
	
	if (m_hThread == NULL)
	{
		if (CreateThread())
			return 0;
		else return -1;
	}	
	return -1;
}