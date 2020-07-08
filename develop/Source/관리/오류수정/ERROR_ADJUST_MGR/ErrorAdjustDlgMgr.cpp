// ErrorAdjustDlgMgr.cpp: implementation of the CErrorAdjustDlgMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustDlgMgr.h"
#include "IndexErrorAdjust.h"
#include "PurchaseInfoErrorAdjust.h"
#include "VolIndexErrorAdjust.h"
#include "WasteDataErrorAdjust1.h"
#include "RecycleDataRestoreDlg.h"
#include "RequestErrorAdjust.h"
#include "SpeciesInfoErrorAdjust.h"
#include "ManageNoErrorAdjust.h"
#include "MarcDataErrorAdjust.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustDlgMgr::CErrorAdjustDlgMgr(CESL_Mgr * pParentMgr)
{
	m_pMain = NULL;
	m_pErrorAdjustDlg = NULL;
	m_pParentMgr = pParentMgr;
	m_nErrorAdjustType = PURCHASEINFO_ERROR_ADJUST;
}

CErrorAdjustDlgMgr::~CErrorAdjustDlgMgr()
{
	if(m_pErrorAdjustDlg)
	{
		delete m_pErrorAdjustDlg;
		m_pErrorAdjustDlg = NULL;
	}
}

VOID CErrorAdjustDlgMgr::CreateErrorAdjustDlg(ERROR_ADJUST_TYPE nErrorAdjustType/*=ERROR_ADJUST_TYPE_INVALID*/)
{
	if(m_pErrorAdjustDlg)
	{
		delete m_pErrorAdjustDlg;
		m_pErrorAdjustDlg = NULL;
	}

	if(nErrorAdjustType != ERROR_ADJUST_TYPE_INVALID)
		m_nErrorAdjustType = nErrorAdjustType;

	switch(m_nErrorAdjustType) 
	{
	case PURCHASEINFO_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CPurchaseInfoErrorAdjust(m_pParentMgr);
		break;
	case INDEX_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CIndexErrorAdjust(m_pParentMgr);
		break;
	case VOLINDEX_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CVolIndexErrorAdjust(m_pParentMgr);
		break;
	case WASTEDATA_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CWasteDataErrorAdjust(m_pParentMgr);
		break;
	case RECYCLE_DATA_RESTORE:
		m_pErrorAdjustDlg = new CRecycleDataRestoreDlg(m_pParentMgr);
		break;
	case REQUEST_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CRequestErrorAdjust(m_pParentMgr);
		break;
	case SPECIESINFO_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CSpeciesInfoErrorAdjust(m_pParentMgr);
		break;
	case MANAGENO_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CManageNoErrorAdjust(m_pParentMgr);
		break;
	case MARCDATA_ERROR_ADJUST:
		m_pErrorAdjustDlg = new CMarcDataErrorAdjust(m_pParentMgr);
		break;
	default:
		m_pParentMgr->ESLAfxMessageBox(_T("오류수정 다이얼로그 타입을 지정해 주십시요."));
		return ;
	}
}

BOOL CErrorAdjustDlgMgr::ShowErrorAdjustDlg(CFormView * pParentWnd)
{
	if(m_pErrorAdjustDlg == NULL) return FALSE;
	if(!m_pErrorAdjustDlg->Create(pParentWnd))
		return FALSE;

	m_pErrorAdjustDlg->ShowWindow(SW_SHOW);
	return TRUE;
}
