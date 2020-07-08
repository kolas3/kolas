// SearchMgr.cpp: implementation of the CSearchMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SearchMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearchMgr::CSearchMgr(CESL_Mgr * pParentMgr, SEARCH_TYPE openMode, ACQ_TYPE acqCode, SPECIES_TYPE nSpeciesType, CString strConnectionInfo):CSPF_Work(pParentMgr)
{
	m_nOpenMode = openMode;
	m_pSearchDlg = NULL;
	m_nAcqCode = acqCode;
	m_nSpeciesType = nSpeciesType;
	m_pDM = NULL;
	m_pGrid = NULL;
	CreateSearchDlg(strConnectionInfo);
}

CSearchMgr::CSearchMgr(CESL_Mgr * pParentMgr, SEARCH_TYPE openMode, CESL_DataMgr * pDM, CESL_Grid * pGrid):CSPF_Work(pParentMgr)
{
	m_nOpenMode = openMode;
	m_pSearchDlg = NULL;
	m_nAcqCode = ACQ_TYPE_INVALID;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_pDM = pDM;
	m_pGrid = pGrid;
	CreateSearchDlg(pDM->CONNECTION_INFO);
}

CSearchMgr::~CSearchMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
}

VOID CSearchMgr::ShowSearchDlg()
{
	if(!(m_pSearchDlg->GetCreate()))
		m_pSearchDlg->CreateSearchDlg(m_pParentMgr);
	m_pSearchDlg->ShowGroupInfo();
	m_pSearchDlg->CenterWindow();
	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->UpdateWindow();
	
}

VOID CSearchMgr::GetWhereStmt(CString &whereStmt)
{
	whereStmt = m_pSearchDlg->GetWhereStmt();
}

VOID CSearchMgr::SetWhereStmt(CString whereStmt)
{
	m_pSearchDlg->SetWhereStmt(whereStmt);
}

CGroupCollection * CSearchMgr::GetGroupCollection()
{
	return m_pSearchDlg->GetGroupCollection();
}

VOID CSearchMgr::CreateSearchDlg(CString strConnectionInfo)
{
	if(m_nOpenMode == BO_ACQ_BASIC)				m_pSearchDlg = new CBO_ACQ_BasicSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_IMPORT)		m_pSearchDlg = new CBO_ACQ_BasicSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_PURCHASE)		m_pSearchDlg = new CBO_ACQ_SearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_ORDER)		m_pSearchDlg = new CBO_ACQ_DivisionNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_MISSING)		m_pSearchDlg = new CBO_ACQ_DivisionNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_REGOBJ && m_nAcqCode == PURCHASE)		m_pSearchDlg = new CBO_ACQ_DivisionNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_REGOBJ && m_nAcqCode == DONATE)		m_pSearchDlg = new CBO_ACQ_ReceiptNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_WORK_SEARCH)	m_pSearchDlg = new CBO_ACQ_DivisionNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_HOLD_SEARCH)	m_pSearchDlg = new CBO_ACQ_DivisionNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_DONATE)		m_pSearchDlg = new CBO_ACQ_DonateDataSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_NONDONATE)	m_pSearchDlg = new CBO_ACQ_DonateDataSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_RESEARCH)		m_pSearchDlg = new CBO_ACQ_ResultSearchDlg(m_pParentMgr, m_pDM, m_pGrid);
	else if(m_nOpenMode == BO_ACQ_GROUP)		m_pSearchDlg = new CBO_ACQ_GroupSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_REQUEST)		m_pSearchDlg = new CBO_ACQ_RequestSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_ACQ_RECYCLE)		m_pSearchDlg = new CBO_ACQ_RecycleSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_DONATE_PRINT)		m_pSearchDlg = new CBO_ACQ_ReceiptNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_PURCHASE_PRINT)	m_pSearchDlg = new CBO_ACQ_DivisionNoSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == BO_DATA_SEARCH)		m_pSearchDlg = new CBO_DataSearchDlg(m_pParentMgr);
	else if(m_nOpenMode == SE_DATA_SEARCH)		m_pSearchDlg = new CSE_DataSearchDlg(m_pParentMgr);
	m_pSearchDlg->SetOpenMode(m_nOpenMode);
	m_pSearchDlg->SetAcqCode(m_nAcqCode);
	m_pSearchDlg->SetCONNECTION_INFO(strConnectionInfo);
	m_pSearchDlg->SetSpeciesType(m_nSpeciesType);
}

BOOL CSearchMgr::GetDataSortYN()
{
	return m_pSearchDlg->GetDataSortYN();
}

VOID CSearchMgr::SetDataSortItem(CSortItemSet *pItem)
{
	m_pSearchDlg->SetDataSortItem(pItem);
}

VOID CSearchMgr::HideSearchDlg()
{
	if( !(m_pSearchDlg->GetCreate()) )		return;	
	m_pSearchDlg->ShowWindow(SW_HIDE);
}
