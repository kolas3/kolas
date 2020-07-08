// CheckinVolLauncher.cpp: implementation of the CCheckinVolLauncher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckinVolLauncher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckinVolLauncher::CCheckinVolLauncher()
{
	m_pCheckinVolMgr = NULL;
	m_pWinThread = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_nACQ_TYPE = 1; //default
	m_strACQ_YEAR = _T("");
	m_strRECEIPT_NO = _T("");
	m_bIsLightVersion = FALSE;
}

CCheckinVolLauncher::~CCheckinVolLauncher()
{
	if (m_pWinThread)
	{
		m_pWinThread->EndThread();
		m_pWinThread = NULL;
	}

	if (m_pCheckinVolMgr)
	{
		delete m_pCheckinVolMgr;
		m_pCheckinVolMgr = NULL;
	}
}

INT CCheckinVolLauncher::SetParentInfo(CESL_DataMgr *pParentDM, 
									   CESL_Grid *pParentGrid,
									   INT nACQ_TYPE,
									   CString strACQ_YEAR,
									   CString strRECEIPT_NO,
									   BOOL bIsLightVersion)
{
	m_pParentDM = pParentDM;
	m_pParentGrid = pParentGrid;
	m_nACQ_TYPE = nACQ_TYPE;
	m_strACQ_YEAR = strACQ_YEAR;
	m_strRECEIPT_NO = strRECEIPT_NO; 
	m_bIsLightVersion =	bIsLightVersion;

	return 0;
}

INT CCheckinVolLauncher::LaunchCheckinVol(CWnd *pParentWnd,CESL_Mgr *pParentMgr)
{
	if (pParentMgr == NULL) return -1;

	if (m_pWinThread)
		m_pWinThread->EndThread();
	

	if (m_pCheckinVolMgr)
		delete m_pCheckinVolMgr;


	m_pCheckinVolMgr = new CCheckinVolMgr;

	//set parent info
	m_pCheckinVolMgr->SetParentInfo(m_pParentDM,
									m_pParentGrid,
									m_nACQ_TYPE,
									m_strACQ_YEAR,
									m_strRECEIPT_NO,
									m_bIsLightVersion);

	//init
	if (m_pCheckinVolMgr->InitManager(pParentMgr) < 0)
	{
		//MessageBox(pParentMgr->m_hWnd, _T("CheckinVolMgr Init Failed!!") , _T("권호별 체크인") , MB_OK);
		return -1;
	}
	
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CCheckinVolThread);
	m_pWinThread = static_cast<CCheckinVolThread*>(pRuntimeClass->CreateObject());
	if (!m_pWinThread) return -1;

	if(m_pWinThread->StartThread(m_pCheckinVolMgr,pParentWnd)<0) return -1;
	
	pParentWnd->BeginModalState();


	return 0;
}