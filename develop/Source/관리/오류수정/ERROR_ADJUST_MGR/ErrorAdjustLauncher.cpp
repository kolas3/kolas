// ErrorAdjustLauncher.cpp: implementation of the CErrorAdjustLauncher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustLauncher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustLauncher::CErrorAdjustLauncher()
{
	m_pErrorAdjustMgr = NULL;
	m_pWinThread = NULL;
}

CErrorAdjustLauncher::~CErrorAdjustLauncher()
{
	if(m_pErrorAdjustMgr)
	{
		delete m_pErrorAdjustMgr;
		m_pErrorAdjustMgr = NULL;
	}
	if(m_pWinThread)
	{
		m_pWinThread->EndThread();
		m_pWinThread = NULL;
	}
}

INT CErrorAdjustLauncher::LaunchErrorAdjustFrame(CWnd *pParentWnd,CESL_Mgr *pParentMgr)
{
	if (pParentMgr == NULL) return -1;
	
	if (m_pWinThread)
		m_pWinThread->EndThread();
	
	
	if (m_pErrorAdjustMgr)
		delete m_pErrorAdjustMgr;
	
	
	m_pErrorAdjustMgr = new CErrorAdjustMgr;
	
	//init
	if (m_pErrorAdjustMgr->InitManager(pParentMgr) < 0)
	{
		return -1;
	}
	
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CErrorAdjustThread);
	m_pWinThread = static_cast<CErrorAdjustThread*>(pRuntimeClass->CreateObject());
	if (!m_pWinThread) return -1;
	
	if(m_pWinThread->StartThread(m_pErrorAdjustMgr ,pParentWnd)<0) return -1;
	
	pParentWnd->BeginModalState();
	
	return 0;
}