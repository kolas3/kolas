// ErrorAdjustSearchMgr.cpp: implementation of the CErrorAdjustSearchMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustSearchMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustSearchMgr::CErrorAdjustSearchMgr(UINT IDD, CESL_Mgr * pMgr) : CESL_Mgr(IDD, pMgr)
{
	m_nDlgID = IDD;
	m_bCreate = NULL;
	m_nErrorAdjustType = ERROR_ADJUST_TYPE_INVALID;
	m_strQuery.Empty();
}

CErrorAdjustSearchMgr::~CErrorAdjustSearchMgr()
{

}

VOID CErrorAdjustSearchMgr::SendQueryCreateMsg()
{
	::SendMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

CString CErrorAdjustSearchMgr::GetQuery()
{
	if(m_nErrorAdjustType == PURCHASEINFO_ERROR_ADJUST)
	{
		m_strQuery.Format(
			_T("I.REC_KEY=S.REC_KEY ")
			_T("AND S.REC_KEY=B.SPECIES_KEY ")
			_T("AND B.ACQ_CODE='1' ")
			_T("AND B.ACQ_KEY IS NULL ")
			_T("AND B.WORKING_STATUS < 'BOC' ")
			_T("AND ROWNUM < 5001"));
		return m_strQuery;
	}
	else if(m_nErrorAdjustType == VOLINDEX_ERROR_ADJUST)
	{
		m_strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY ")
			_T("AND S.REC_KEY = B.SPECIES_KEY ")
			_T("AND I.IDX_VOL IS NULL AND B.VOL IS NOT NULL ")
			_T("AND ROWNUM < 5001"));
		return m_strQuery;
	}
	else if(m_nErrorAdjustType == WASTEDATA_ERROR_ADJUST)
	{
		m_strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY(+) ")
			_T("AND S.REC_KEY = B.SPECIES_KEY(+) ")
			_T("AND I.SPECIES_CLASS IN ('0','1') ")
			_T("AND B.REC_KEY IS NULL ")
			_T("AND I.REC_KEY NOT IN (SELECT DUP_SPECIES_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY IS NOT NULL) ") 
			_T("AND ROWNUM < 5001"));
		return m_strQuery;
	}

	return m_strQuery;
}

BOOL CErrorAdjustSearchMgr::CreateSearchDlg(CWnd * pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(m_nDlgID, pParentWnd);
}

CString CErrorAdjustSearchMgr::MakeDateCondition_Stmt(CString fieldName, CString m_Date1, CString m_Date2)
{
	CString strOption, strTmpData;
	if(!m_Date2.IsEmpty() && m_Date1.Compare(m_Date2) > 0)
	{
		strTmpData = m_Date1;
		m_Date1 = m_Date2;
		m_Date2 = strTmpData;
	}
	
	if(m_Date1.IsEmpty() && !(m_Date2.IsEmpty()))
	{
		ESLAfxMessageBox(_T("검색 시작일을 입력하십시요."));
		return _T("-1");
	}
	else if(!(m_Date1.IsEmpty()) && m_Date2.IsEmpty())
		strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
	else if(!(m_Date1.IsEmpty()) && !(m_Date2.IsEmpty()))
	{
		if(m_Date1 == m_Date2)
			strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
		else
			strOption.Format(_T(" AND %s >= '%s' AND %s <= '%s'"), fieldName, m_Date1, fieldName, m_Date2);
	}
	return strOption;
}
