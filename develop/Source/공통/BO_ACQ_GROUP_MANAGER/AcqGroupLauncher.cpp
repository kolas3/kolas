// AcqGroupLauncher.cpp: implementation of the CAcqGroupLauncher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcqGroupLauncher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcqGroupLauncher::CAcqGroupLauncher()
{
	m_nGroupOpenType = GROUP_OPEN_TYPE_INVALID;
	m_pAcqGroupMgr = NULL;
	m_pWinThread = NULL;
}

CAcqGroupLauncher::~CAcqGroupLauncher()
{
	if(m_pAcqGroupMgr)
	{
		delete m_pAcqGroupMgr;
		m_pAcqGroupMgr = NULL;
	}
	if(m_pWinThread)
	{
		m_pWinThread->EndThread();
		m_pWinThread = NULL;
	}
}

INT CAcqGroupLauncher::LaunchAcqGroupFrame(CWnd *pParentWnd, CESL_Mgr *pParentMgr)
{
	if (pParentMgr == NULL) return -1;
	
	if (m_pWinThread)
		m_pWinThread->EndThread();
	
	
	if (m_pAcqGroupMgr)
		delete m_pAcqGroupMgr;
	
	
	m_pAcqGroupMgr = new CAcqGroupMgr;
	
	//init
	if (m_pAcqGroupMgr->InitManager(pParentMgr, m_nGroupOpenType) < 0)
	{
		return -1;
	}
	
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CAcqGroupThread);
	m_pWinThread = static_cast<CAcqGroupThread*>(pRuntimeClass->CreateObject());
	if (!m_pWinThread) return -1;
	
	if(m_pWinThread->StartThread(m_pAcqGroupMgr ,pParentWnd)<0) return -1;
	
	pParentWnd->BeginModalState();
	
	return 0;
}
