// AcqGroupThread.cpp : implementation file
//

#include "stdafx.h"
#include "AcqGroupThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupThread

IMPLEMENT_DYNCREATE(CAcqGroupThread, CWinThread)

CAcqGroupThread::CAcqGroupThread()
{
	m_pAcqGroupMgr = NULL;	
	m_pFrame = NULL;
	m_pParentWnd = NULL;
}

CAcqGroupThread::~CAcqGroupThread()
{
}

BOOL CAcqGroupThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CAcqGroupFrame);
	m_pFrame = static_cast<CAcqGroupFrame*>(pRuntimeClass->CreateObject());
	if (m_pFrame == NULL) return FALSE;
	
	// Set Mgr
	m_pFrame->SetAcqGroupMgr(m_pAcqGroupMgr);
	
	// Document
	pRuntimeClass = RUNTIME_CLASS(CAcqGroupDoc);
	CAcqGroupDoc *pDoc = static_cast<CAcqGroupDoc*>(pRuntimeClass->CreateObject());
	
	// CreateContext
	CCreateContext createContext;
	createContext.m_pCurrentFrame = m_pFrame;
	createContext.m_pNewViewClass = RUNTIME_CLASS(CAcqGroupFormView);
	createContext.m_pCurrentDoc = pDoc;
	
	// Create Frame
	m_pFrame->Create(NULL, _T("수서그룹관리"), WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_VISIBLE, CRect(0,0,700,700), NULL, NULL, 0, &createContext);
	
	// Init Frame
	m_pFrame->InitialUpdateFrame(pDoc, TRUE);
	
	m_pMainWnd = m_pFrame;
	
	return TRUE;
}

INT CAcqGroupThread::ExitInstance()
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

BEGIN_MESSAGE_MAP(CAcqGroupThread, CWinThread)
	//{{AFX_MSG_MAP(CAcqGroupThread)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupThread message handlers

INT CAcqGroupThread::EndThread()
{
	PostThreadMessage(WM_QUIT, 0, 0);
	::WaitForSingleObject(m_hThread,INFINITE);
	
	return 0;
}

CWnd* CAcqGroupThread::GetMainWnd() 
{
	// TODO: Add your specialized code here and/or call the base class
	//return m_pFrame;
	
	return CWinThread::GetMainWnd();
}

INT CAcqGroupThread::StartThread(CAcqGroupMgr *pParentAcqGroupMgr, CWnd *pParentWnd)
{
	if (!pParentAcqGroupMgr) return -1;
	
	m_pAcqGroupMgr = pParentAcqGroupMgr;
	m_pParentWnd = pParentWnd;
	
	if (m_hThread == NULL)
	{
		if (CreateThread())
			return 0;
		else return -1;
	}	
	return -1;
}