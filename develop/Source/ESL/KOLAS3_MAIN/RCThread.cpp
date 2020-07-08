// RCThread.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "RCThread.h"

#include "KOLAS3_MAINDoc.h"
#include "RCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCThread

IMPLEMENT_DYNCREATE(CRCThread, CWinThread)

CRCThread::CRCThread()
{
	m_pRCFrame = NULL;
	
	m_pListToolBarGroup = NULL;
	m_pListToolBarItem  = NULL;
	
	m_pInfo = NULL;
	m_pRCDlg = NULL;

	// 자동으로 지워지지 않게 한다.
	//m_bAutoDelete = FALSE;
}

CRCThread::~CRCThread()
{
	
}

INT CRCThread::StartThread(CString strFrameTitle, CONST RECT &rcFrame, CString strMenuAlias, CString strMenuCaption, CString strMenuTarget, CObList *pListToolBarGroup, CObList *pListToolBarItem, CESL_Information *pInfo, CWnd * pRCDlg)
{
	if (strFrameTitle.IsEmpty() || strMenuAlias.IsEmpty() ||
		strMenuAlias.IsEmpty() || strMenuCaption.IsEmpty() || 
		strMenuTarget.IsEmpty() || pListToolBarGroup == NULL || 
		pListToolBarItem == NULL ||	pInfo == NULL )
		return -1;

	m_strFrameTitle = strFrameTitle;
	m_strMenuAlias = strMenuAlias;
	m_strMenuCaption = strMenuCaption;
	m_strMenuTarget = strMenuTarget;

	m_rcFrame = rcFrame;

	m_pListToolBarGroup = pListToolBarGroup;
	m_pListToolBarItem = pListToolBarItem;

	m_pInfo = pInfo;
	
	m_pRCDlg = pRCDlg;

	if(m_hThread == NULL)
		CreateThread();

	return 0;
}
INT CRCThread::EndThread()
{
	if (m_hThread == NULL) return 0;
	PostThreadMessage(WM_QUIT, 0, 0);
	//if (m_hThread) ::WaitForSingleObject(m_hThread,INFINITE);
	//delete this;
	
	return 0;
}

BOOL CRCThread::InitInstance()
{
	// 프레임을 생성한다.

	// FRAME
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CRCFrame);
	m_pRCFrame = (CRCFrame*)pRuntimeClass->CreateObject();

	// DOCUMENT
	pRuntimeClass = RUNTIME_CLASS(CKOLAS3_MAINDoc);
	CKOLAS3_MAINDoc *pDoc = (CKOLAS3_MAINDoc*)pRuntimeClass->CreateObject();

	CCreateContext createContext;
	createContext.m_pCurrentFrame = m_pRCFrame;
	createContext.m_pNewViewClass = RUNTIME_CLASS(CRCView);
	createContext.m_pCurrentDoc = pDoc;

	m_pRCFrame->Create(NULL, m_strFrameTitle, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_VISIBLE, m_rcFrame, NULL, NULL, 0, &createContext);
	m_pRCFrame->InitialUpdateFrame(pDoc, TRUE);
	m_pRCFrame->SetWindowText(m_strFrameTitle);
	UINT nID;
	nID = IDR_MAINFRAME;
	m_pRCFrame->SetIcon(::AfxGetApp()->LoadIcon(nID), FALSE);
	
	m_pMainWnd = m_pRCFrame;

	CRCView *pView = (CRCView*)m_pRCFrame->GetActiveView();
	pView->SetParent(m_pRCFrame);
	pView->SetParentInfo(m_strMenuAlias, m_strMenuTarget, m_strMenuCaption, m_pListToolBarGroup, m_pListToolBarItem);
	pView->m_pInfo = m_pInfo;
	pView->m_pRCDlg = m_pRCDlg;
	pView->Init();

	return TRUE;
}

INT CRCThread::ExitInstance()
{
	if (m_pRCFrame != NULL && m_pRCFrame->GetSafeHwnd())
	{
		if (::IsWindow(m_pRCFrame->GetSafeHwnd()))
		{
			if ( m_pRCFrame->GetSafeHwnd() != NULL ) m_pRCFrame->SendMessage(WM_CLOSE, 0 , 0);
		}
		m_pRCFrame = NULL;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRCThread, CWinThread)
	//{{AFX_MSG_MAP(CRCThread)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCThread message handlers

CWnd* CRCThread::GetMainWnd() 
{
	//return m_pRCFrame;
	return CWinThread::GetMainWnd();
}
