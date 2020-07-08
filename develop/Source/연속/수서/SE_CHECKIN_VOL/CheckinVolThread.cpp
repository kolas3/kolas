// CheckinVolThread.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolThread.h"

#include "CheckInVolDoc.h"
#include "CheckinVolViewGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolThread

IMPLEMENT_DYNCREATE(CCheckinVolThread, CWinThread)

CCheckinVolThread::CCheckinVolThread()
{
	m_pCheckinVolMgr = NULL;
	m_pFrame = NULL;
	m_pParentWnd = NULL;
}

CCheckinVolThread::~CCheckinVolThread()
{

}

BOOL CCheckinVolThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CCheckinVolFrame);
	m_pFrame = static_cast<CCheckinVolFrame*>(pRuntimeClass->CreateObject());
	if (m_pFrame == NULL) return FALSE;

	// Set Mgr
	m_pFrame->SetCheckinVolMgr(m_pCheckinVolMgr);
	
	// Document
	pRuntimeClass = RUNTIME_CLASS(CCheckInVolDoc);
	CCheckInVolDoc *pDoc = static_cast<CCheckInVolDoc*>(pRuntimeClass->CreateObject());

	// CreateContext
	CCreateContext createContext;
	createContext.m_pCurrentFrame = m_pFrame;
	createContext.m_pNewViewClass = RUNTIME_CLASS(CCheckinVolViewGrid);
	createContext.m_pCurrentDoc = pDoc;

	// Create Frame
	m_pFrame->Create(NULL, _T("권호별 체크인"), WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_VISIBLE, CRect(0,0,700,700), NULL, NULL, 0, &createContext);
	
	// Init Frame
	m_pFrame->InitialUpdateFrame(pDoc, TRUE);

	// test : create resource;
	m_pCheckinVolMgr->CreateResource(m_pFrame);

	m_pMainWnd = m_pFrame;

	return TRUE;
}

INT CCheckinVolThread::ExitInstance()
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

BEGIN_MESSAGE_MAP(CCheckinVolThread, CWinThread)
	//{{AFX_MSG_MAP(CCheckinVolThread)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolThread message handlers

INT CCheckinVolThread::StartThread(CCheckinVolMgr *pParentCheckinVolMgr, CWnd *pParentWnd)
{
	if (!pParentCheckinVolMgr) return -1;

	m_pCheckinVolMgr = pParentCheckinVolMgr;
	m_pParentWnd = pParentWnd;
	
	if (m_hThread == NULL)
		if (CreateThread())
			return 0;
		else return -1;

	return -1;
}

INT CCheckinVolThread::EndThread()
{
	PostThreadMessage(WM_QUIT, 0, 0);
	::WaitForSingleObject(m_hThread,INFINITE);

	return 0;
}

CWnd* CCheckinVolThread::GetMainWnd() 
{
	// TODO: Add your specialized code here and/or call the base class
	//return m_pFrame;
	
	return CWinThread::GetMainWnd();
}
