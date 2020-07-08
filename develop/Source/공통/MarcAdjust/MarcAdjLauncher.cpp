// MarcAdjLauncher.cpp: implementation of the CMarcAdjLauncher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcAdjLauncher.h"
#include "MarcAdjMgr_BO.h"
#include "MarcAdjMgr_SE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMarcAdjLauncher::CMarcAdjLauncher()
{
	m_pWinThread = NULL;
	m_pMarcAdjMgr = NULL;

	m_pDM_Index_Parent = NULL;
	m_pDM_Species_Parent = NULL;

	m_bCatHold = FALSE;

 	CString strFileName;
 	DWORD dwPID = ::GetCurrentProcessId();
 	strFileName.Format(_T("..\\cfg\\TouchPad\\TOOLH(%u).dat"), dwPID); 
 	::DeleteFile(strFileName);
}

CMarcAdjLauncher::~CMarcAdjLauncher()
{
	if (m_pWinThread)
	{
		m_pWinThread->EndThread();
		m_pWinThread = NULL;
	}

	if (m_pMarcAdjMgr)
	{
		delete m_pMarcAdjMgr;
		m_pMarcAdjMgr = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// CMarcAdjLauncher Operations
INT CMarcAdjLauncher::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CString strBookWhere, INT nSelectedGridRow /*= -1*/, BOOL bCatHold /*= FALSE*/)
{
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;
	m_strBookWhere = strBookWhere;
	m_nParentSelectedRow = nSelectedGridRow;
	m_bCatHold = bCatHold;

	return 0;
}

INT CMarcAdjLauncher::SetParentInfo(CESL_DataMgr *pDM_Species_Parent, CESL_DataMgr *pDM_Index_Parent, CESL_Grid *pGrid_Parent, INT nSelectedGridRow /*= -1*/, BOOL bCatHold /*= FALSE*/)
{
	m_pDM_Species_Parent = pDM_Species_Parent;
	m_pDM_Index_Parent = pDM_Index_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_nParentSelectedRow = nSelectedGridRow;
	m_bCatHold = bCatHold;

	return 0;
}

INT CMarcAdjLauncher::LaunchMarcAdjust(CWnd *pParentWnd, CESL_Mgr *pParentMgr, LPCTSTR lpszWorkingStatus /* = NULL */, INT nManageMode /*= MNG_INSERT*/, INT nModeMONO_SERIAL /*= MODE_MONO*/)
{
	if (pParentMgr == NULL) return -1;

	CString strFileName;
 	DWORD dwPID = ::GetCurrentProcessId();		
 	strFileName.Format(_T("..\\cfg\\TouchPad\\TOOLH(%u).dat"), dwPID); 
	CFileFind FF;
	if(TRUE == FF.FindFile(strFileName))
	{
		return 1000;
	}

	if (m_pWinThread)
		m_pWinThread->EndThread();

	if (m_pMarcAdjMgr)
		delete m_pMarcAdjMgr;

	if (nModeMONO_SERIAL == MODE_MONO)
		m_pMarcAdjMgr = new CMarcAdjMgr_BO;
	else if (nModeMONO_SERIAL == MODE_SERIAL)
		m_pMarcAdjMgr = new CMarcAdjMgr_SE;

	if (nManageMode == MNG_INSERT)
		m_pMarcAdjMgr->SetParentInfo(m_pDM_Parent, m_pGrid_Parent, m_strSpeciesKeyAlias, m_strBookWhere, m_nParentSelectedRow, m_bCatHold);
	else if (nManageMode == MNG_MODIFY)
		m_pMarcAdjMgr->SetParentInfo(m_pDM_Parent, m_pGrid_Parent, m_strSpeciesKeyAlias, m_strBookWhere, m_nParentSelectedRow, m_bCatHold);
	else if (nManageMode == MNG_IMPORT)
		m_pMarcAdjMgr->SetParentInfo(m_pDM_Species_Parent, m_pDM_Index_Parent, m_pGrid_Parent, m_nParentSelectedRow, m_bCatHold);
	else if (nManageMode == MNG_IMPORT_WORKING)
		m_pMarcAdjMgr->SetParentInfo(m_pDM_Species_Parent, m_pDM_Index_Parent, m_pGrid_Parent, m_nParentSelectedRow, m_bCatHold);

	if (m_pMarcAdjMgr->InitManager(nManageMode, pParentMgr, lpszWorkingStatus, NULL) < 0)
	{
		return -1;
	}

	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CMarcAdjThread);
	m_pWinThread = static_cast<CMarcAdjThread*>(pRuntimeClass->CreateObject());
	if (!m_pWinThread) return -1;

	if (m_pWinThread->StartThread(m_pMarcAdjMgr, pParentWnd) < 0) return -1;
	pParentWnd->BeginModalState();

	return 0;
}
