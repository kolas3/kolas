// AcqGroupDlgMgr.cpp: implementation of the CAcqGroupDlgMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcqGroupDlgMgr.h"
#include "AcqDonateGroupDlg.h"
#include "AcqPurchaseGroupDlg.h"
#include "AcqObjGroupDlg.h"
#include "AcqOtherGroupDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcqGroupDlgMgr::CAcqGroupDlgMgr(CESL_Mgr * pParentMgr)
{
	m_pMain = NULL;
	m_pAcqGroupDlg = NULL;
	m_pParentMgr = pParentMgr;
	m_nAcqGroupType = ACQ_OBJ_GROUP;
}

CAcqGroupDlgMgr::~CAcqGroupDlgMgr()
{
	if(m_pAcqGroupDlg)
	{
		delete m_pAcqGroupDlg;
		m_pAcqGroupDlg = NULL;
	}
}

VOID CAcqGroupDlgMgr::CreateAcqGroupDlg(ACQ_GROUP_TYPE nAcqGroupType/*=ACQ_GROUP_INVALID*/)
{
	if(m_pAcqGroupDlg)
	{
		delete m_pAcqGroupDlg;
		m_pAcqGroupDlg = NULL;
	}
	
	if(nAcqGroupType != ACQ_GROUP_INVALID)
		m_nAcqGroupType = nAcqGroupType;
	
	switch(m_nAcqGroupType) 
	{
	case ACQ_OBJ_GROUP:
		m_pAcqGroupDlg = new CAcqObjGroupDlg(m_pParentMgr);
		break;
	case ACQ_PURCHASE_GROUP:
		m_pAcqGroupDlg = new CAcqPurchaseGroupDlg(m_pParentMgr);
		break;
	case ACQ_DONATE_GROUP:
		m_pAcqGroupDlg = new CAcqDonateGroupDlg(m_pParentMgr);
		break;
	case ACQ_OTHER_GROUP:
		m_pAcqGroupDlg = new CAcqOtherGroupDlg(m_pParentMgr);
		break;
	default:
		m_pParentMgr->ESLAfxMessageBox(_T("그룹관리 다이얼로그 타입을 지정해 주십시요."));
		return ;
	}
}

BOOL CAcqGroupDlgMgr::ShowAcqGroupDlg(CFormView * pParentWnd)
{
	if(m_pAcqGroupDlg == NULL) return FALSE;
	if(!m_pAcqGroupDlg->Create(pParentWnd))
		return FALSE;
	
	m_pAcqGroupDlg->ShowWindow(SW_SHOW);
	return TRUE;
}

