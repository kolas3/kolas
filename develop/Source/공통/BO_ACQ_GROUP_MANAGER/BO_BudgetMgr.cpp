// BO_BudgetMgr.cpp: implementation of the CBO_BudgetMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_BudgetMgr.h"
#include "../COMMON_BUDGET_MGR/BudgetSelectDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("예산집행정보관리"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_BudgetMgr::CBO_BudgetMgr(CESL_Mgr * pMgr)
{
	m_pParentMgr = pMgr;
	m_pDM_OUT = NULL;
	m_pDM_EXECUTION = NULL;
	m_pGroup = NULL;
	m_pGroupCollection = NULL;
	m_strExecuteType.Empty();
	m_strIs64 = _T("N");
}

CBO_BudgetMgr::~CBO_BudgetMgr()
{
	if(m_pDM_EXECUTION)
	{
		delete m_pDM_EXECUTION;
		m_pDM_EXECUTION = NULL;
	}
}

BOOL CBO_BudgetMgr::Init()
{
	if(m_pParentMgr == NULL) return FALSE;
	if(m_pDM_OUT == NULL) return FALSE;
	if(m_pGroupCollection == NULL)
	{
		if(m_pGroup == NULL) return FALSE;
		else m_nOperationType = 0;
	}
	else m_nOperationType = 1;

	m_pDM_OUT->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '육사'"), m_strIs64);

	m_pDM_EXECUTION = new CESL_DataMgr;
	if(m_pDM_EXECUTION == NULL) return FALSE;
	m_pDM_EXECUTION->SetCONNECTION_INFO(m_pDM_OUT->CONNECTION_INFO);
	m_pDM_EXECUTION->InitDMPKFromFile(_T("DM_예산집행정보"));
	m_pDM_EXECUTION->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	return TRUE;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakePreBudgetExecutionInfo()
{
	if(!Init()) return BUDGET_ERROR;
	if(m_strBudgetKey.IsEmpty()) return BUDGET_ERROR;
	
	BUDGET_ERROR_TYPE ids;
	ids = CheckBudgetRemainder(_T("0"));
	if(ids != BUDGET_SUCCESS) return ids;

	if(m_nOperationType == 0)
		ids = MakePreBudgetExecutionInfoByGroup(m_pGroup);
	else if(m_nOperationType == 1)
		ids = MakePreBudgetExecutionInfoByGroupCollection();
	else return BUDGET_ERROR;
	
	return ids;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakePreBudgetExecutionInfoByGroup(CGroupInfo * pGroup)
{
	if(pGroup == NULL) return BUDGET_ERROR;

	BUDGET_ERROR_TYPE ids = InsertBudgetExecutionInfoByGroup(pGroup, m_strBudgetExeKey);
	if(ids < 0) return ids;

	ids = MakeBudgetSubExecutionInfoByMatCode(pGroup, m_strBudgetExeKey);
	if(ids < 0) return ids;

	CString strResult;
	m_pDM_OUT->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNGLIMIT'"), strResult);
	if(strResult == _T("N"))
	{
		ids = MakeBudgetSubExecutionInfoByCourseCode(pGroup, m_strBudgetExeKey);
		if(ids < 0) return ids;

		ids = MakeBudgetSubExecutionInfoByDeptCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByFormCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByManageCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByMediaCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoBySubregCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByUseobjCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;

	}

	return BUDGET_IGNORE;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakePreBudgetExecutionInfoByGroupCollection()
{
	if(m_pGroupCollection == NULL) return BUDGET_ERROR;
	if(m_pGroupCollection->GetGroupCount() == 0) return BUDGET_IGNORE;

	BUDGET_ERROR_TYPE ids;
	for(INT i=0 ; i<m_pGroupCollection->GetGroupCount() ; i++)
	{
		m_pGroup = m_pGroupCollection->GetGroupInfo(i);
		ids = MakePreBudgetExecutionInfoByGroup(m_pGroup);
		if(ids < 0) return ids;
	}

	return BUDGET_IGNORE;
}


BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetExecutionInfo()
{
	if(!Init()) return BUDGET_ERROR;
	
	BUDGET_ERROR_TYPE res = BUDGET_ERROR_TYPE_INVALID;
	
	if(m_nOperationType == 0)
		res = MakeBudgetExecutionInfoByGroup(m_pGroup);
	else if(m_nOperationType == 1)
		res = MakeBudgetExecutionInfoByGroupCollection();
	else return BUDGET_ERROR;
	
	return res;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetExecutionInfoByGroup(CGroupInfo *pGroup)
{
	BUDGET_ERROR_TYPE ids = InsertBudgetExecutionInfoByGroup(pGroup, m_strBudgetExeKey);

	if(ids != BUDGET_SUCCESS)
		return ids;

	ids = MakeBudgetSubExecutionInfoByMatCode(pGroup, m_strBudgetExeKey);
	if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;

	CString strResult;
	m_pDM_OUT->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNGLIMIT'"), strResult);
	if(strResult == _T("N"))
	{
		ids = MakeBudgetSubExecutionInfoByCourseCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;

		ids = MakeBudgetSubExecutionInfoByDeptCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByFormCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByManageCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByMediaCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoBySubregCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;
		ids = MakeBudgetSubExecutionInfoByUseobjCode(pGroup, m_strBudgetExeKey);
		if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;


	}

	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetExecutionInfoByGroupCollection()
{
	if(m_pGroupCollection == NULL) return BUDGET_ERROR;
	if(m_pGroupCollection->GetGroupCount() == 0) return BUDGET_SUCCESS;
	
	BUDGET_ERROR_TYPE ids;
	for(INT i=0 ; i<m_pGroupCollection->GetGroupCount() ; i++)
	{
		m_pGroup = m_pGroupCollection->GetGroupInfo(i);
		m_pGroup->SetRecKey(_T("1"), m_pDM_OUT, m_strMode  );
		ids = MakeBudgetExecutionInfoByGroup(m_pGroup);
		if(ids != BUDGET_SUCCESS) return ids;
	}
	return BUDGET_SUCCESS;
}

CString CBO_BudgetMgr::GetGroupKey(CGroupInfo *pGroup)
{
	CString strGroupKey = pGroup->GetRecKey();
	if(strGroupKey.IsEmpty()) 
	{
		INT ids = pGroup->SetRecKey(_T("1"), m_pDM_OUT, m_strMode );
		if(ids < 0) return _T("");
		strGroupKey = pGroup->GetRecKey();
	}
	return strGroupKey;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::DeleteBudgetExecutionInfo()
{
	if(!Init()) return BUDGET_ERROR;
	
	BUDGET_ERROR_TYPE ids;
	if(m_nOperationType == 0)
		ids = DeleteBudgetExecutionInfoByGroup(m_pGroup);
	else if(m_nOperationType == 1)
		ids = DeleteBudgetExecutionInfoByGroupCollection();
	else return BUDGET_ERROR;
	
	return ids;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::DeleteBudgetExecutionInfoByGroup(CGroupInfo *pGroup)
{
	CString strQuery, strGroupKey;
	
	strGroupKey = GetGroupKey(pGroup);

	if(!strGroupKey.IsEmpty())
	{
		strQuery.Format(
			_T("DELETE FROM CO_BUDGET_EXECUTION_SUB_TBL ")
			_T("WHERE BUDGET_EXECUTION_KEY IN ")
			_T("(SELECT REC_KEY FROM CO_BUDGET_EXECUTION_TBL WHERE EXECUTE_GROUP_KEY=%s);"),
			strGroupKey);
		INT ids = m_pDM_OUT->AddFrame(strQuery);
		if(ids < 0) return BUDGET_ERROR;

		strQuery.Format(
			_T("DELETE FROM CO_BUDGET_EXECUTION_TBL WHERE EXECUTE_GROUP_KEY=%s;"),
			strGroupKey);
		ids = m_pDM_OUT->AddFrame(strQuery);
		if(ids < 0) return BUDGET_ERROR;
	}
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::DeleteBudgetExecutionInfoByGroupCollection()
{
	if(m_pGroupCollection == NULL) return BUDGET_ERROR;
	if(m_pGroupCollection->GetGroupCount() == 0) return BUDGET_SUCCESS;
	
	BUDGET_ERROR_TYPE ids;
	for(INT i=0 ; i<m_pGroupCollection->GetGroupCount() ; i++)
	{
		m_pGroup = m_pGroupCollection->GetGroupInfo(i);
		ids = DeleteBudgetExecutionInfoByGroup(m_pGroup);
		if(ids < 0) return ids;
	}
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::InsertBudgetExecutionInfoByGroup(CGroupInfo *pGroup, 
													CString &strExecuteKey)
{
	INT ids;
	
	CString strGroupKey, strExecutionDate, strQuery, strExecuteAccount, strTmpData;
	CString strGroupInfo;

	m_pDM_OUT->MakeRecKey(strExecuteKey);
	CTime t = m_pDM_OUT->GetDBTime();
	strExecutionDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	
	strGroupKey = GetGroupKey(pGroup);
	if(strGroupKey.IsEmpty()) return BUDGET_ERROR;

	if(m_strMode == _T("SE"))
		strGroupInfo.Format(_T("연속:%s-%s"),pGroup->GetAcqYear(),pGroup->GetGroupInfo());
	else
		strGroupInfo.Format(_T("단행:%s-%s-%s"),pGroup->GetAcqYear(),pGroup->GetWorkCode(),pGroup->GetGroupInfo());
	
	BUDGET_ERROR_TYPE res = CheckDuplicateGroupKey(pGroup);
	if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE) return BUDGET_ERROR;
	
	if(m_strBudgetKey.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT max(rec_key) ")
			_T("FROM co_budget_compilation_tbl ")
			_T("WHERE rec_key IN (SELECT budget_compilation_key ")
			_T("FROM co_budget_execution_tbl ")
            _T("WHERE execute_group_key = %s)"),
			strGroupKey);
		ids = m_pDM_OUT->GetOneValueQuery(strQuery, m_strBudgetKey);
		if(ids < 0) return BUDGET_ERROR;
		if(m_strBudgetKey.IsEmpty()) 
		{
			SetBudgetInfo(m_strBudgetKey);
			if(m_strBudgetKey.IsEmpty()) return BUDGET_IGNORE;
			BUDGET_ERROR_TYPE res;
			res = CheckBudgetRemainder(m_strExecuteType);
			if(res != BUDGET_SUCCESS) return res;			
		}
	}

	if(m_strExecuteType == _T("0"))
		strTmpData.Empty();
	else if(m_strExecuteType == _T("1"))
		strTmpData.Format(_T("AND DELIVERY_YN='Y'"));
	CString sQuery;
	if( m_strIs64 == _T("Y") )
	{
		sQuery.Format(_T("SELECT ROUND(SUM (getpricewon (acq_key, price, 'Y', UDF_MANAGE_CODE)),-1) "));
	}
	else
	{
		sQuery.Format(_T("SELECT ROUND(SUM (getpricewon (acq_key, price, 'Y', UDF_MANAGE_CODE)),0) "));
	}

	if(m_strMode == _T("MO")){
		strQuery.Format(
			_T("%s")
			_T("FROM bo_book_tbl ")
			_T("WHERE rec_key IN ( ")
			_T("SELECT b.rec_key ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.rec_key = b.species_key AND i.MANAGE_CODE = UDF_MANAGE_CODE ")
			_T("AND I.work_code = '%s' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND book_appendix_flag = 'B' ")
			_T("AND acq_year = '%s' ")
			_T("AND acq_code = '1' ")
			_T("AND seq_no = %s %s)"),
			sQuery, pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else
	{
		strQuery.Format(
			_T("SELECT ROUND(SUM(price),0) ")
			_T("FROM se_purchase_species_tbl ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ORDER_YEAR = '%s' ")
			_T("AND ORDER_SEQ_NO = %s")
			,pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	
	ids = m_pDM_OUT->GetOneValueQuery(strQuery, strExecuteAccount);
	if(ids < 0) return BUDGET_ERROR;
	
	m_pDM_EXECUTION->FreeData();
	m_pDM_EXECUTION->InsertRow(-1);
	m_pDM_EXECUTION->SetCellData(_T("예산집행정보KEY"), strExecuteKey, 0);
	m_pDM_EXECUTION->SetCellData(_T("예산편성정보KEY"), m_strBudgetKey, 0);
	m_pDM_EXECUTION->SetCellData(_T("예산집행그룹KEY"), strGroupKey, 0);
	m_pDM_EXECUTION->SetCellData(_T("예산집행타입"), _T("0"), 0);
	m_pDM_EXECUTION->SetCellData(_T("집행예산"), strExecuteAccount, 0);
	m_pDM_EXECUTION->SetCellData(_T("집행일"), strExecutionDate, 0);
	m_pDM_EXECUTION->SetCellData(_T("집행상태"), m_strExecuteType, 0);
	m_pDM_EXECUTION->SetCellData(_T("예산집행작업자"), m_pParentMgr->GetUserID(), 0);
	m_pDM_EXECUTION->SetCellData(_T("비고"), strGroupInfo, 0);
	m_pDM_EXECUTION->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
	m_pDM_EXECUTION->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), 0);
	
	CArray <CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	ids = m_pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM_EXECUTION, m_pDM_EXECUTION->TBL_NAME, 0, m_pDM_OUT);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::CheckBudgetRemainder(CString strExecutionType)
{
	if(m_strBudgetKey.IsEmpty()) return BUDGET_ERROR;

	BUDGET_ERROR_TYPE ids;
	CString strQuery, strOrderAccount, strBudgetAccount, strBudgetRemainder, strMsg;
	if(m_nOperationType == 0) 
		ids = GetGroupPrice(m_pGroup, strOrderAccount, strExecutionType);
	else if(m_nOperationType == 1) 
		ids = GetGroupCollectionPrice(strOrderAccount, strExecutionType);
	else return BUDGET_ERROR;

	if(ids != BUDGET_SUCCESS && ids != BUDGET_IGNORE) return ids;

	strQuery.Format(
		_T("SELECT budget_account, budget_account - (SELECT nvl(SUM (execution_budget_account), 0) ")
		_T("FROM co_budget_execution_tbl ")
		_T("WHERE budget_compilation_key = %s) ")
		_T("FROM co_budget_compilation_tbl ")
		_T("WHERE rec_key = %s"),
		m_strBudgetKey, m_strBudgetKey);

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM_OUT->CONNECTION_INFO);
	INT res = tmpDM.RestructDataManager(strQuery);
	if(res < 0) return BUDGET_ERROR;

	strBudgetAccount = tmpDM.GetCellData(0, 0);
	strBudgetRemainder = tmpDM.GetCellData(0, 1);

	if(_ttol(strBudgetRemainder.GetBuffer(0)) < _ttol(strOrderAccount.GetBuffer(0)))
	{
		strMsg.Format(
			_T("선정하신 예산편성정보에 잔액이 부족합니다.\n")
			_T("예산잔액 : [%s], 주문액수 : [%s]")
			_T("그래도 집행하시겠습니까?.\n"),
			strBudgetRemainder, strOrderAccount);
		INT ids = m_pParentMgr->ESLAfxMessageBox(strMsg,MB_YESNO);
		if( ids == IDYES )
		{return BUDGET_SUCCESS;}	
		else
		{return BUDGET_OVERFLOW;}
		
		return BUDGET_OVERFLOW;
	}
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::GetGroupPrice(CGroupInfo *pGroup, CString &strPrice, CString strExecutionType)
{
	if(pGroup == NULL) return BUDGET_ERROR;


	CString strQuery, strTmpData;
	if(strExecutionType == _T("0"))
		strTmpData.Empty();
	else if(strExecutionType == _T("1"))
		strTmpData.Format(_T("AND DELIVERY_YN='Y'"));

	if(m_strMode == _T("MO"))
	{
		strQuery.Format(
			_T("SELECT ROUND(SUM (getpricewon (acq_key, price, 'Y', UDF_MANAGE_CODE)),0) ")
			_T("FROM bo_book_tbl ")
			_T("WHERE rec_key IN ( ")
			_T("SELECT b.rec_key ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.rec_key = b.species_key i.MANAGE_CODE = UDF_MANAGE_CODE ")
			_T("AND I.work_code = '%s' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND book_appendix_flag = 'B' ")
			_T("AND acq_year = '%s' ")
			_T("AND acq_code = '1' ")
			_T("AND seq_no = %s %s)"),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else
	{
		strQuery.Format(
			_T("SELECT ROUND(SUM(price),0) ")
			_T("FROM se_purchase_species_tbl ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ORDER_YEAR = '%s' ")
			_T("AND ORDER_SEQ_NO = %s")
			,pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	INT ids = m_pDM_OUT->GetOneValueQuery(strQuery, strPrice);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::GetGroupCollectionPrice(CString &strPrice, CString strExecutionType)
{
	if(m_pGroupCollection == NULL) return BUDGET_ERROR;
	if(m_pGroupCollection->GetGroupCount() == 0) return BUDGET_SUCCESS;
	if(m_pGroupCollection->GetAcqYear().IsEmpty()) return BUDGET_ERROR;

	CString strQuery, strTmpData, strGroupInfo;
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	if(strGroupInfo.IsEmpty()) return BUDGET_SUCCESS;

	if(strExecutionType == _T("0"))
		strTmpData.Empty();
	else if(strExecutionType == _T("1"))
		strTmpData.Format(_T("AND DELIVERY_YN='Y'"));

	CString str64, strGrade;
	m_pDM_OUT->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '육사'"), str64);
	if( str64 == _T("Y") )
		strGrade = _T("2");
	else
		strGrade = _T("0");

	if(m_strMode == _T("MO")){

		strQuery.Format(
			_T("SELECT ROUND(SUM (getpricewon (acq_key, price, 'Y', UDF_MANAGE_CODE)),%s) ")
			_T("FROM bo_book_tbl ")
			_T("WHERE rec_key IN ( ")
			_T("SELECT b.rec_key ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.rec_key = b.species_key AND i.MANAGE_CODE = UDF_MANAGE_CODE ")
			_T("AND I.work_code = '%s' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND book_appendix_flag = 'B' ")
			_T("AND acq_year = '%s' ")
			_T("AND acq_code = '1' ")
			_T("AND seq_no in (%s) %s)"),
			strGrade,m_pGroupCollection->GetWorkCode(), m_pGroupCollection->GetAcqYear(), strGroupInfo, strTmpData);
	}
	else
	{
		strQuery.Format(
			_T("SELECT ROUND(SUM(price),0) ")
			_T("FROM se_purchase_species_tbl ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ORDER_YEAR = '%s' ")
			_T("AND ORDER_SEQ_NO = %s")
			,m_pGroupCollection->GetAcqYear(), strGroupInfo);
	}
	INT ids = m_pDM_OUT->GetOneValueQuery(strQuery, strPrice);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::ModifyBudgetExecutionInfoStatus()
{
	if(!Init()) return BUDGET_ERROR;

	BUDGET_ERROR_TYPE ids;
	if(m_nOperationType == 0)
		ids = ModifyBudgetExecutionInfoStatusByGroup(m_pGroup);
	else if(m_nOperationType == 1)
		ids = ModifyBudgetExecutionInfoStatusByGroupCollection();
	else return BUDGET_ERROR;
	
	return ids;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::ModifyBudgetExecutionInfoStatusByGroup(CGroupInfo *pGroup)
{
	CString strQuery, strGroupKey;
	
	strGroupKey = GetGroupKey(pGroup);
	if(strGroupKey.IsEmpty()) return BUDGET_ERROR;
	
	strQuery.Format(
		_T("UPDATE co_budget_execution_tbl ")
		_T("SET budget_execution_status = '0', ")
		_T("last_work = '%s' ")
		_T("WHERE execute_group_key = %s;"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG), strGroupKey);
	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::ModifyBudgetExecutionInfoStatusByGroupCollection()
{
	if(m_pGroupCollection == NULL) return BUDGET_ERROR;
	if(m_pGroupCollection->GetGroupCount() == 0) return BUDGET_SUCCESS;
	
	INT ids;
	for(INT i=0 ; i<m_pGroupCollection->GetGroupCount() ; i++)
	{
		m_pGroup = m_pGroupCollection->GetGroupInfo(i);
		ids = ModifyBudgetExecutionInfoStatusByGroup(m_pGroup);
		if(ids < 0) return BUDGET_ERROR;
	}
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::SetBudgetInfo(CString &strBudgetKey)
{
	CBudgetSelectDlg dlg(m_pParentMgr) ;

	if(dlg.DoModal ()== IDOK)
	{
		strBudgetKey = dlg.m_strBUDGET_COMPILATION_KEY;
	}
	else strBudgetKey.IsEmpty();
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::CheckDuplicateGroupKey(CGroupInfo *pGroup)
{
	CString strQuery, strValue, strGroupKey;
	strGroupKey = GetGroupKey(pGroup);
	if(strGroupKey.IsEmpty()) return BUDGET_ERROR;
	
	strQuery.Format(
		_T("SELECT MAX(budget_compilation_key) ")
		_T("FROM co_budget_execution_tbl ")
        _T("WHERE execute_group_key = %s"), strGroupKey);
	INT ids = m_pDM_OUT->GetOneValueQuery(strQuery, strValue);
	if(ids < 0) return BUDGET_ERROR;
	if(!strValue.IsEmpty())
	{
		if(IDNO == m_pParentMgr->ESLAfxMessageBox(
					_T("중복된 예산 집행정보가 존재합니다.\n")
					_T("기존 예산집행정보를 삭제하고 작업을 진행 하시겠습니까?"),
					MB_YESNO))
			return BUDGET_ERROR;
		else
			return DeleteBudgetExecutionInfoByGroup(pGroup);
			
	}
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByMatCode(CGroupInfo * pGroup, 
													   CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));

		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'M', mat_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT i.mat_code mat_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND i.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY i.mat_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else 
	{
		strTmpData.Empty();
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'M', mat_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   s.mat_code mat_code, ROUND(SUM (s.PRICE),0) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.mat_code = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.mat_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo() );

	}

	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByUsageCode(CGroupInfo * pGroup, 
														 CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{

		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));
		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'U', usage_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   i.usage_code usage_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND i.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY i.usage_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else 
	{
		strTmpData.Empty();
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'U', usage_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   i.usage_code usage_code, ROUND(SUM (getpricewon (s.SPECIES_KEY, s.PRICE, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE")
			_T("FROM idx_se_tbl i, SE_PURCHASE_SPECIES_TBL s , se_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = s.SPECIES_KEY ")
			_T("AND i.rec_key = b.SPECIES_KEY ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY i.usage_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByCourseCode(CGroupInfo * pGroup, 
														  CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));

		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'C', course_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT p.course_code course_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b, BO_PURCHASEINFO_TBL p ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND p.species_key = i.rec_key ")
			_T("AND I.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY p.course_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else 
	{
		strTmpData.Empty();
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'C', COURCE, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   s.COURCE COURCE, ROUND(SUM (s.PRICE),0) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.COURCE));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}

	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}


BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByDeptCode(CGroupInfo *pGroup, CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO")) 
	{

		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));
		if( m_strIs64 == _T("Y") )
		{
			strQuery.Format(
				_T("INSERT INTO co_budget_execution_sub_tbl ")
				_T("(select esl_seq.nextval, %s, 'D', dept_code, account, '%s', '%s', MANAGE_CODE ")
				_T("from ")
				_T("(SELECT   p.dept_code dept_code, sum(p.order_price) account, i.MANAGE_CODE ")
				_T("FROM idx_bo_tbl i, BO_PURCHASEINFO_TBL p ")
				_T("WHERE p.MANAGE_CODE = UDF_MANAGE_CODE AND p.species_key = i.rec_key ")		
				_T("AND i.work_code = '%s' ")
				_T("AND (i.species_class = '0' OR i.species_class = '1') ")
				_T("AND p.order_year = '%s' ")
				_T("AND p.order_seq_no = %s ")
				_T("GROUP BY p.dept_code));"),
				strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
				pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
		}
		else
		{
			strQuery.Format(
				_T("INSERT INTO co_budget_execution_sub_tbl ")
				_T("(select esl_seq.nextval, %s, 'D', dept_code, account, '%s', '%s', MANAGE_CODE ")
				_T("from ")
				_T("(SELECT   p.dept_code dept_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
				_T("FROM idx_bo_tbl i, bo_book_tbl b, BO_PURCHASEINFO_TBL p ")
				_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND ")
				_T("AND p.species_key = i.rec_key ")
				_T("AND i.work_code = '%s' ")
				_T("AND b.book_appendix_flag = 'B' ")
				_T("AND (i.species_class = '0' OR i.species_class = '1') ")
				_T("AND b.acq_year = '%s' ")
				_T("AND b.acq_code = '1' ")
				_T("AND b.seq_no = %s %s ")
				_T("GROUP BY p.dept_code));"),
				strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
				pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
		}
	}
	else
	{
		
		strTmpData.Empty();
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'D', DEPT, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   s.DEPT DEPT, ROUND(SUM (s.PRICE),0) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.DEPT));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo() );
	}

	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByFormCode(CGroupInfo *pGroup, CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'F', form_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT i.form_code form_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND i.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY i.form_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else
	{
		strTmpData.Empty();
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'F', form_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT  s.form_code form_code, ROUND(SUM ( s.PRICE ),0) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.form_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}

	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByManageCode(CGroupInfo *pGroup, CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'A', manage_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   i.manage_code manage_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND i.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY i.manage_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else 
	{
		strTmpData.Empty();		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'A', manage_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   s.manage_code manage_code, ROUND(SUM (s.PRICE),0) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.manage_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());	
	}
	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByMediaCode(CGroupInfo *pGroup, CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'E', media_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   i.media_code media_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND i.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY i.media_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else
	{
		strTmpData.Empty();		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'E', media_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
						_T("(SELECT   s.media_code media_code, ROUND(SUM (s.PRICE),0) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.media_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoBySubregCode(CGroupInfo *pGroup, CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));		
		if( m_strIs64 == _T("Y") )
		{
			strQuery.Format(
				_T("INSERT INTO co_budget_execution_sub_tbl ")
				_T("(select esl_seq.nextval, %s, 'S', dept_code, account, '%s', '%s', MANAGE_CODE ")
				_T("from ")
				_T("(SELECT   i.sub_reg_code dept_code, sum(p.order_price) account, i.MANAGE_CODE ")
				_T("FROM idx_bo_tbl i, BO_PURCHASEINFO_TBL p ")
				_T("WHERE p.MANAGE_CODE = UDF_MANAGE_CODE AND p.species_key = i.rec_key ")
				_T("AND i.work_code = '%s' ")
				_T("AND (i.species_class = '0' OR i.species_class = '1') ")
				_T("AND p.order_year = '%s' ")
				_T("AND p.order_seq_no = %s ")
				_T("GROUP BY i.sub_reg_code));"),
				strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
				pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
		}
		else
		{
			strQuery.Format(
				_T("INSERT INTO co_budget_execution_sub_tbl ")
				_T("(select esl_seq.nextval, %s, 'S', sub_reg_code, account, '%s', '%s', MANAGE_CODE ")
				_T("from ")
				_T("(SELECT   i.sub_reg_code sub_reg_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
				_T("FROM idx_bo_tbl i, bo_book_tbl b ")
				_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
				_T("AND i.work_code = '%s' ")
				_T("AND b.book_appendix_flag = 'B' ")
				_T("AND (i.species_class = '0' OR i.species_class = '1') ")
				_T("AND b.acq_year = '%s' ")
				_T("AND b.acq_code = '1' ")
				_T("AND b.seq_no = %s %s ")
				_T("GROUP BY i.sub_reg_code));"),
				strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
				pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
		}
	}
	else
	{
		strTmpData.Empty();		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'S', sub_reg_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   s.sub_reg_code sub_reg_code, ROUND(SUM (s.PRICE )) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.sub_reg_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}
	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}

BUDGET_ERROR_TYPE CBO_BudgetMgr::MakeBudgetSubExecutionInfoByUseobjCode(CGroupInfo *pGroup, CString strExecuteKey)
{
	CString strQuery, strTmpData;
	if ( m_strMode == _T("MO") ) 
	{
		if(m_strExecuteType == _T("0")) strTmpData.Empty();
		else if(m_strExecuteType == _T("1")) strTmpData.Format(_T("AND b.delivery_yn='Y'"));		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'O', use_obj_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   i.use_obj_code use_obj_code, ROUND(SUM (getpricewon (b.acq_key, b.price, 'Y', i.MANAGE_CODE)),0) account, i.MANAGE_CODE ")
			_T("FROM idx_bo_tbl i, bo_book_tbl b ")
			_T("WHERE i.MANAGE_CODE = UDF_MANAGE_CODE AND i.rec_key = b.species_key ")
			_T("AND i.work_code = '%s' ")
			_T("AND b.book_appendix_flag = 'B' ")
			_T("AND (i.species_class = '0' OR i.species_class = '1') ")
			_T("AND b.acq_year = '%s' ")
			_T("AND b.acq_code = '1' ")
			_T("AND b.seq_no = %s %s ")
			_T("GROUP BY i.use_obj_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetWorkCode(), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strTmpData);
	}
	else 
	{
		strTmpData.Empty();		
		strQuery.Format(
			_T("INSERT INTO co_budget_execution_sub_tbl ")
			_T("(select esl_seq.nextval, %s, 'O', use_obj_code, account, '%s', '%s', MANAGE_CODE ")
			_T("from ")
			_T("(SELECT   s.use_obj_code use_obj_code, ROUND(SUM ( s.PRICE )) account, s.MANAGE_CODE ")
			_T("FROM SE_PURCHASE_SPECIES_TBL s ")
			_T("WHERE s.MANAGE_CODE = UDF_MANAGE_CODE AND s.ORDER_YEAR = '%s' ")
			_T("AND s.ORDER_SEQ_NO = %s ")
			_T("GROUP BY s.use_obj_code));"),
			strExecuteKey, m_pParentMgr->GetWorkLogMsg(WORK_LOG), m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			pGroup->GetAcqYear(), pGroup->GetGroupInfo());
	}

	INT ids = m_pDM_OUT->AddFrame(strQuery);
	if(ids < 0) return BUDGET_ERROR;
	return BUDGET_SUCCESS;
}
