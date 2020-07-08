// GroupMgr.cpp: implementation of the CGroupMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupMgr.h"
#include "DivisionNo_Select_Dlg.h"
#include "Group_Select_Dlg.h"
#include "RequisitionNo_Select_Dlg.h"
#include "Group_Select_Dlg.h"
#include "GroupCollection.h"
#include "GroupPair.h"
#include "CheckHoldDataDlg.h"
#include "../BO_SPECIES_API/BO_SPECIES.h"
#include "BO_BudgetMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("그룹관리"), _T(__FILE__), __LINE__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupMgr::CGroupMgr(CESL_Mgr * pParentMgr):CSPF_Work(pParentMgr)
{
	m_pDataProcessingDlg = NULL;
}

CGroupMgr::~CGroupMgr()
{
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CGroupMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(m_pParentMgr);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();

	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CGroupMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CGroupMgr::SPFExecutePath(CString strPath)
{
	if( strPath.IsEmpty() == TRUE )		return -1;

	if		( strPath == _T("차수생성")				)	return CreateDivisionNoInfo();
	else if	( strPath == _T("차수수정")				)	return UpdateDivisionNoInfo();
	else if	( strPath == _T("차수삭제")				)	return DeleteDivisionNoInfo();
	else if	( strPath == _T("DM구성")				)	return MakeDivisionNoDM();
	else if	( strPath == _T("선정자료추출")			)	return ExtractSelectInfo();
	else if	( strPath == _T("그룹추가")				)	return InsertGroup();
	else if	( strPath == _T("그룹수정")				)	return UpdateGroup();
	else if	( strPath == _T("그룹검색")				)	return SearchGroup();
	else if	( strPath == _T("그룹삭제")				)	return DeleteGroup();
	else if	( strPath == _T("그룹이름추출")			)	return ExtractGroupName();
	else if	( strPath == _T("선정자료그룹이름추출")	)	return SelectedDataExtractGroupName();
	else if	( strPath == _T("그룹통합")				)	return MergeGroup();
	else if	( strPath == _T("총계정보추출")			)	return GetTotalPriceAndBookCount();		
	else if	( strPath == _T("자료확인DM구성")		)	return MakeDivNoDataInfoDM();	
	else if	( strPath == _T("마지막번호추출")		)	return GetLastNumber();	
	else if	( strPath == _T("접수번호생성")			)	return InsertRequisitionNo();
	else if	( strPath == _T("접수번호수정")			)	return UpdateRequisitionNo();
	else if	( strPath == _T("접수번호검색")			)	return SearchRequisitionNo();
	else if	( strPath == _T("접수번호삭제")			)	return DeleteReceiptNoInfo();
	else if	( strPath == _T("마지막작업번호추출")	)	return GetLastWorkNo();
	else if	( strPath == _T("그룹번호정보추출")		)	return GetGroupNo();
	else if	( strPath == _T("작업번호정보추출")		)	return GetWorkNo();
	else if	( strPath == _T("마지막일련번호추출")	)	return GetLastSerialNumber();
	else if	( strPath == _T("작업번호생성")			)	return MakeWorkNoInfo();
	else if	( strPath == _T("일련번호갱신")			)	return UpdateLastSerialNo();
	else if	( strPath == _T("그룹번호유효성검사")	)	return CheckGroupNoValidate();
	else if	( strPath == _T("그룹정보추출")			)	return MakeGroupInfo();
	else if	( strPath == _T("그룹중복검사")			)	return CheckDuplicateGroupName();
	else if	( strPath == _T("그룹자료수구하기")		)	return GetGroupDataCount();
	else if	( strPath == _T("차수상태변경")			)	return ChangeDivNoStatus();
	else if	( strPath == _T("그룹책수설정")			)	return SetGroupInfoBookCnt();
	else if	( strPath == _T("접수번호책수설정")		)	return SetReceiptNoBookCnt();
	else if	( strPath == _T("차수번호책수설정")		)	return SetDivisionNoBookCnt();
	else if	( strPath == _T("수서그룹번호책수설정")	)	return SetOtherGroupNoBookCnt();
	else if	( strPath == _T("수서그룹번호생성")		)	return InsertOtherGroupNo();
	else if	( strPath == _T("수서그룹번호수정")		)	return UpdateOtherGroupNo();
	else if	( strPath == _T("수서그룹삭제")			)	return DeleteOtherGroupNo();

	return -1;
}

INT CGroupMgr::GetLastSerialNumber(INT nAcqType, CString strAcqYear, CString strGroupInfo, CString strWorkInfo, CString &strLastSerialNum, CString strWorkCode/*=_T("")*/)
{
	INT ids = -1;
	ACQ_TYPE nATYPE = (ACQ_TYPE)nAcqType;

	switch( nATYPE )
	{	
	case ACQ_TYPE_INVALID:
	{
		ids = GetGroupLastSerialNum(strAcqYear, strGroupInfo, strLastSerialNum);
	}
		break;
	case PURCHASE:
	{
			ids = GetDivNoLastSerialNum(strAcqYear, strGroupInfo, strWorkInfo, strWorkCode, strLastSerialNum);
	}
		break;
	case DONATE:
	{
		ids = GetReceiptNoLastSerialNum(strAcqYear, strGroupInfo, strLastSerialNum);
	}
		break;
	case OTHER_ACQ_TYPE:
	{
		CString strAcqCode;
		GetTempData(_T("수입구분"), strAcqCode);
		ids = GetOtherGroupNoLastSerialNum(strAcqCode, strAcqYear, strGroupInfo, strLastSerialNum);
	}
		break;
	default:	ids = -1;
		break;
	}

	return ids;
}

INT CGroupMgr::GetLastSerialNumber()
{
	ACQ_TYPE nAcqCode;
	CString strAcqYear, strGroupInfo, strWorkInfo, strLastSerialNum, strWorkCode;
	INT tmpData;
	GetTempData(_T("ACQ_TYPE"), tmpData);
	nAcqCode = (ACQ_TYPE)tmpData;
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	GetTempData(_T("Group_Info"), strGroupInfo);
	GetTempData(_T("Work_Info"), strWorkInfo);

	INT ids = -1;
	if(nAcqCode == ACQ_TYPE_INVALID)
	{
		ids = GetGroupLastSerialNum(strAcqYear, strGroupInfo, strLastSerialNum);
	}
	else if(nAcqCode == PURCHASE)
	{
		GetTempData(_T("Work_Code"), strWorkCode);
		ids = GetDivNoLastSerialNum(strAcqYear, strGroupInfo, strWorkInfo, strWorkCode, strLastSerialNum);
	}
	else if(nAcqCode == DONATE)
	{
		return 0;
	}

	SetTempData(_T("Last_Serial_No"), strLastSerialNum.GetBuffer(0));
	return ids;
}

INT CGroupMgr::GetDivNoLastSerialNum(CString strAcqYear, CString strGroupInfo, CString strWorkInfo, CString strWorkCode, CString &strLastSerialNum)
{
	INT ids, nLastNum;
	CString strQuery, strTmpData;
	CESL_DataMgr tmpDM;

	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	if ( strWorkInfo.IsEmpty() ) strWorkInfo = _T("0");
	
	if( strWorkCode.IsEmpty() == TRUE )
	{	
		strQuery.Format(_T("SELECT LAST_SERIAL_NO ")
						_T("FROM BO_PURCHASE_WORK_NO_TBL ")
						_T("WHERE WORK_NO = %s ")
						_T("AND PURCHASE_SEQ_NO_KEY = ")
						_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL)"),
		strWorkInfo, strAcqYear, strGroupInfo);
	}
	else
	{
		strQuery.Format(_T("SELECT LAST_SERIAL_NO ")
						_T("FROM BO_PURCHASE_WORK_NO_TBL ")
						_T("WHERE WORK_NO = %s ")
						_T("AND PURCHASE_SEQ_NO_KEY = ")
						_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = %s)"),
						strWorkInfo, strAcqYear, strGroupInfo, strWorkCode);
	}

	ids = tmpDM.RestructDataManager(strQuery);
	if( ids < 0 )	return -1;

	strTmpData	= tmpDM.GetCellData(0,0);
	nLastNum	= _ttoi(strTmpData.GetBuffer(0));

	strLastSerialNum.Format(_T("%d"), nLastNum);

	return 0;
}

INT CGroupMgr::GetGroupLastSerialNum(CString strAcqYear, CString strGroupInfo, CString &strLastSerialNum)
{
	CESL_DataMgr tmpDM;
	if(strAcqYear.IsEmpty() || strGroupInfo == _T("적용안함")) 
	{
		strLastSerialNum = _T("0");
		return 0;
	}
	
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO ")
					_T("FROM BO_ACQ_GROUP_TBL ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' AND GROUP_NAME = '%s'"), strAcqYear, strGroupInfo);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	strLastSerialNum = tmpDM.GetCellData(0,0);
	return 0;
}

INT CGroupMgr::GetWorkNo()
{
	CString strAcqYear, strGroupNo, strWorkCode, strWorkNoUseYN;
	CStringArray * parrWorkNo = NULL;
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	GetTempData(_T("Group_No"), strGroupNo);
	GetTempData(_T("WORK_CODE"), strWorkCode);
	parrWorkNo = (CStringArray*)GetTempPtr(_T("Work_No"));	
	
	CESL_DataMgr tmpDM;
	if(m_pParentMgr->CONNECTION_INFO.IsEmpty()) return -1;
	
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	CString strQuery;
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL) ORDER BY WORK_NO"),
			strAcqYear,
			strGroupNo);
	else
		strQuery.Format(
			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE='%s') ORDER BY WORK_NO"),
			strAcqYear,
			strGroupNo, 
			strWorkCode);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	parrWorkNo->RemoveAll();
	INT cnt = tmpDM.GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = tmpDM.GetCellData(i, 0);
		parrWorkNo->Add(strTmpData);
	}

	if(strWorkCode.IsEmpty())
		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL"), strAcqYear, strGroupNo);
	else 
		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s'"), strAcqYear, strGroupNo, strWorkCode);

	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	strWorkNoUseYN = tmpDM.GetCellData(0, 0);

	if(strWorkNoUseYN == _T("Y")) return 1000;
	else if(strWorkNoUseYN.IsEmpty() || strWorkNoUseYN == _T("N")) return 0;

	return 0;
}

INT CGroupMgr::GetGroupNo()
{
	ACQ_TYPE nAcqCode;
	SPECIES_TYPE nSpeciesType;
	CString strAcqYear;
	CStringArray * parrGroupNo = NULL;
	INT tmpData;
	GetTempData(_T("ACQ_TYPE"), tmpData);
	nAcqCode = (ACQ_TYPE)tmpData;
	GetTempData(_T("SPECIES_TYPE"), tmpData);
	nSpeciesType = (SPECIES_TYPE)tmpData;
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	parrGroupNo = (CStringArray*)GetTempPtr(_T("GroupNo"));

	parrGroupNo->RemoveAll();
	INT ids = -1;
	if(nAcqCode == ACQ_TYPE_INVALID)
	{
		CStringArray * parrGroupInfo = NULL;
		parrGroupInfo = (CStringArray*)GetTempPtr(_T("GroupInfo"));
		parrGroupInfo->RemoveAll();
		ids = GetGroupNameList(strAcqYear, parrGroupNo, parrGroupInfo);
	}
	else if(nAcqCode == PURCHASE)
	{
		ids = GetDivNoList(nSpeciesType, strAcqYear, parrGroupNo);
	}
	else if(nAcqCode == DONATE)
	{
		ids = GetReqNoList(strAcqYear, parrGroupNo);
	}

	return ids;
}

INT CGroupMgr::GetGroupNameList(CString strAcqYear, CStringArray * parrGroupName, CStringArray * parrGroupInfo)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT GROUP_NAME, GROUP_SEQ FROM BO_ACQ_GROUP_TBL WHERE CREATE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY GROUP_SEQ"), strAcqYear);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT cnt = tmpDM.GetRowCount();
	CString strTempData;
	parrGroupName->Add(_T("적용안함"));
	parrGroupInfo->Add(_T("0"));
	for(INT i=0 ; i<cnt ; i++)
	{
		strTempData = tmpDM.GetCellData(i, 0);
		parrGroupName->Add(strTempData);
		strTempData = tmpDM.GetCellData(i, 1);
		parrGroupInfo->Add(strTempData);
	}
	return 0;
}

INT CGroupMgr::GetDivNoList(SPECIES_TYPE speciesType, CString strAcqYear, CStringArray * parrDivNo)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT SEQ_NO FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s'"), strAcqYear);

	if(speciesType == BO_PURCHASE)
		strQuery += _T(" AND WORKING_STATUS = '0'");
	else if(speciesType == BO_ORDER)
		strQuery += _T(" AND WORKING_STATUS = '1'");
	else if(speciesType == BO_MISSING || speciesType == BO_REGOBJ || speciesType == BO_REG_PURCHASE || speciesType == BO_REG_DONATE || speciesType == BO_REG_INVALID)
		strQuery += _T(" AND WORKING_STATUS = '2'");
	strQuery += _T(" ORDER BY SEQ_NO");
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT cnt = tmpDM.GetRowCount();
	CString strTempData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTempData = tmpDM.GetCellData(i, 0);
		parrDivNo->Add(strTempData);
	}
	return 0;
}

INT CGroupMgr::GetReqNoList(CString strAcqYear, CStringArray * parrReqNo)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT RECEIPT_NO FROM BO_DON_RECEIPT_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ORDER BY RECEIPT_NO"), strAcqYear);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT cnt = tmpDM.GetRowCount();
	CString strTempData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTempData = tmpDM.GetCellData(i, 0);
		parrReqNo->Add(strTempData);
	}
	return 0;
}

INT CGroupMgr::GetLastWorkNo()
{
	INT ids;
	CString strAcqYear, strDivNo, strWorkCode, strLastWorkNo, strQuery;
	CESL_DataMgr tmpDM;

	strWorkCode = _T("");

	GetTempData(_T("AcqYear"),	strAcqYear	);
	GetTempData(_T("DivNo"),	strDivNo	);
	GetTempData(_T("WorkCode"),	strWorkCode	);
	if( strAcqYear.IsEmpty() || strDivNo.IsEmpty() )	return -1000;
	
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	
	if( strWorkCode.IsEmpty() == TRUE )
	{
		strQuery.Format(_T("SELECT LAST_WORK_NO ")
						_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
						_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
						_T("AND SEQ_NO = %s ")
						_T("AND WORK_CODE IS NULL"), strAcqYear, strDivNo);
	}
	else
	{
		strQuery.Format(_T("SELECT LAST_WORK_NO ")
						_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
						_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
						_T("AND SEQ_NO = %s ")
						_T("AND WORK_CODE = '%s'"), strAcqYear, strDivNo, strWorkCode);
	}

	ids = tmpDM.GetOneValueQuery(strQuery, strLastWorkNo);
	if( ids < 0 )	return ids;
	
	return _ttoi(strLastWorkNo.GetBuffer(0));
}

INT CGroupMgr::InsertRequisitionNo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	BOOL bDropNoApply = FALSE;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("DROP_NO_APPLY"), bDropNoApply);

	if(!pCM || !pDM) return -1000;

	CString strGroupNo, strGroupYear;
	pCM->GetControlMgrData(_T("접수번호"), strGroupNo);
	pCM->GetControlMgrData(_T("수입년도"), strGroupYear);
	INT ids = CheckDupGroupNo(pDM, _T("BO_DON_RECEIPT_TBL"), _T("RECEIPT_NO"), strGroupNo, _T("ACQ_YEAR"), strGroupYear);
	if(ids != 0) return ids;

	pDM->InsertRow(-1);
	INT nCurRow = pDM->GetRowCount() - 1;
	SetDMRowbyCMData(pCM, pDM, nCurRow);
	CString strToday, strReckey;
	strToday = GetTodayDate();
	pDM->MakeRecKey(strReckey);
	pDM->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("접수번호KEY"), strReckey, nCurRow);
	
	pDM->StartFrame();
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("책수"));
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("BO_DON_RECEIPT_TBL"), nCurRow, pDM);

	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	if(!bDropNoApply)
		SetLastNumber(_T("DON_NO"), _T("MO"), pDM->GetCellData(_T("수입년도"), nCurRow), _T(""), pDM->GetCellData(_T("접수번호"), nCurRow));
	
	return 0;
}

INT CGroupMgr::UpdateRequisitionNo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	INT idx;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("Index"), idx);
	
	if(!pCM || !pDM) return -1000;

	pDM->StartFrame();
		
	SetDMRowbyCMData(pCM, pDM, idx);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), idx);

	CString strReckey = pDM->GetCellData(_T("접수번호KEY"), idx);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("책수"));
	RemoveAliasList.Add(_T("접수번호KEY"));
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("BO_DON_RECEIPT_TBL"), idx, pDM, _T("REC_KEY"), _T("NUMERIC"), strReckey, idx);
	
	INT ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	return 0;
}

INT CGroupMgr::GetLastNumber()
{
	CString strType, strAcqYear, strDesc, strLastNo, strPubFormCode;
	GetTempData(_T("Type"), strType);
	GetTempData(_T("AcqYear"), strAcqYear);
	GetTempData(_T("Desc"), strDesc);
	GetTempData(_T("Pub_Form_Code"), strPubFormCode);

	if(strAcqYear.IsEmpty()) return -1;
	CLastNumberManager lastNumMgr(m_pParentMgr);
	lastNumMgr.GetLastNumber(strType, strPubFormCode, strAcqYear, strDesc, strLastNo);
	
	INT lastNo = _ttoi(strLastNo.GetBuffer(0));
	strLastNo.Format(_T("%d"), lastNo + 1);
	SetTempData(_T("LastNumber"), strLastNo.GetBuffer(0));
	return 0;
}

INT CGroupMgr::SetLastNumber(CString strKindCode, CString strPubFormCode, CString strCreateYear, CString strDivVal, CString strLastNum)
{
	CLastNumberManager lastNumber(m_pParentMgr);
	INT ids = lastNumber.SetLastNumber(strKindCode, strPubFormCode, strCreateYear, strDivVal, strLastNum);
	if(ids < 0) return ids;
	return 0;
}

INT CGroupMgr::MergeGroup()
{
	return 0;
}

INT CGroupMgr::SelectedDataExtractGroupName()
{
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CStringArray *arrNames = NULL;
	CStringArray *arrKeys = NULL;

	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	arrNames = (CStringArray*)GetTempPtr(_T("그룹이름리스트"));
	arrKeys = (CStringArray*)GetTempPtr(_T("그룹KEY리스트"));

	INT i = pGrid->SelectMakeList();
	i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -5000;

	while(i >= 0)
	{
		arrNames->Add(pDM->GetCellData(_T("그룹이름"), i));
		arrKeys->Add(pDM->GetCellData(_T("그룹KEY"), i));
		i = pGrid->SelectGetNext();
	}
	return 0;
}

INT CGroupMgr::ExtractGroupName()
{
	CString strCreateYear;
	CStringArray *arrNames = NULL;
	CStringArray *arrKeys = NULL;

	GetTempData(_T("그룹생성년도"), strCreateYear);
	arrNames = (CStringArray*)GetTempPtr(_T("그룹이름리스트"));
	arrKeys = (CStringArray*)GetTempPtr(_T("그룹KEY리스트"));
	
	if(strCreateYear.IsEmpty() ) return -1000;

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	CString query;
	query.Format(_T("SELECT REC_KEY, GROUP_NAME FROM BO_ACQ_GROUP_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s'"));
	tmpDM.RestructDataManager(query);

	INT cnt = tmpDM.GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = tmpDM.GetCellData(i, 0);
		arrKeys->Add(strTmpData);
		strTmpData = tmpDM.GetCellData(i, 1);
		arrNames->Add(strTmpData);
	}

	return 0;
}

INT CGroupMgr::DeleteGroup()
{
	INT nMode;
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_DataMgr * pDM_DELETE = NULL;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pDM_DELETE = (CESL_DataMgr*)GetTempPtr(_T("DELETE_DM"));
	GetTempData(_T("그룹모드"), nMode);

	if(!pCM || !pDM || !pGrid || !pDM_DELETE) return -1000;
	
	INT i = pGrid->SelectMakeList();
	i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -5000;
	CString strLastNo, strCreateYear, strReckey, strGroupNo, strTmpData, strAcqYear;
	INT delType = 0;
	CArray <INT, INT> idxArray;		idxArray.RemoveAll();

	CGroupInfo TargetGroup;
	CGroupCollection SourceCollection(m_pParentMgr);
	SourceCollection.SetAcqType(ACQ_TYPE_INVALID);
	
	INT ids = -1;

	pDM->StartFrame();

	CString strGroupBookCnt;
	INT iGroupBookCnt = 0;
	INT iGroupBooksSumCnt = 0;

	while(i >= 0)
	{
		CGroupInfo pTmpGroup;
		strAcqYear = pDM->GetCellData(_T("그룹생성년도"), i);
		pTmpGroup.SetAcqYear(strAcqYear);
		pTmpGroup.SetGroupInfo(pDM->GetCellData(_T("그룹이름"), i));
		pTmpGroup.SetGroupNo(pDM->GetCellData(_T("그룹번호"), i));
		SourceCollection.AddGroupInfo(&pTmpGroup);
		strTmpData = pDM->GetCellData(_T("그룹KEY"), i);

		strGroupBookCnt = pDM->GetCellData(_T("그룹책수"), i);
		iGroupBookCnt = _ttoi(strGroupBookCnt);

		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
		idxArray.Add(i);
		iGroupBooksSumCnt += iGroupBookCnt;
		i = pGrid->SelectGetNext();
	}

	CGroup_Delete_Dlg dlg(m_pParentMgr);
	dlg.SetOpType((INT)ACQ_TYPE_INVALID);

	dlg.m_iGroupBookSumCnt = iGroupBooksSumCnt;

	if(dlg.DoModal() == IDCANCEL) return 1000;
	delType = dlg.GetDelType();
	if(delType == 1)
	{
		CGroup_Select_Dlg selDlg(m_pParentMgr);
		if(nMode == 1) selDlg.SetSpeciesType(BO_RECYCLE);
		selDlg.SetAcqYear(strAcqYear);

		dlg.m_iGroupBookSumCnt = iGroupBooksSumCnt;
		
		if(selDlg.DoModal() == IDCANCEL)
		{
			return 0;
		}
		else
		{
			TargetGroup.SetAcqYear(selDlg.GetSelGroup()->GetAcqYear());
			TargetGroup.SetGroupInfo(selDlg.GetSelGroup()->GetGroupInfo());
			TargetGroup.SetGroupNo(selDlg.GetSelGroup()->GetGroupNo());
		}
		if(SourceCollection.IsHaveGroupInfo(&TargetGroup))
		{
			m_pParentMgr->ESLAfxMessageBox(_T("삭제될 그룹을 선정하였습니다."));
			return 1000;
		}
		CGroupPair mgr(m_pParentMgr);
		mgr.SetGroupType(GROUP_NO);
		mgr.SetSourceGroupCollection(&SourceCollection);
		mgr.SetTargetGroup(&TargetGroup);
		mgr.SetMainDM(pDM);
		if(nMode == 1) mgr.SetDeleteDataYN(TRUE);
		ids = mgr.MergeGroup();
	}
	else if(delType == 0)
	{
		if(nMode == 1)
			ids = DeleteRecycleDataByGroup(pDM_DELETE, &SourceCollection);
		else
			ids = DeleteDataByGroup(pDM_DELETE, &SourceCollection, ACQ_TYPE_INVALID);
	
		ids = pDM->SendFrame();
		if(ids < 0) return -2000;
		ids = pDM->EndFrame();
		if(ids < 0) return -2000;
	}
	if(ids < 0) return -1;
	
	if(delType == 0) DeleteSelectRow(pDM, &idxArray);

	return 0;
}

INT CGroupMgr::DeleteSelectRow(CESL_DataMgr * pDM, CArray<INT, INT> * pidxlist)
{
	INT cnt = pidxlist->GetSize();
	if(cnt == 0 || !pDM) return -1000;
	for(INT i=cnt-1 ; i >= 0 ; i--)
	{
		pDM->DeleteRow(pidxlist->GetAt(i));
	}
	return 0;
}

INT CGroupMgr::DeleteGroup(CString strCreateYear, CString strReckey, CString strGroupNo, CESL_DataMgr * pDM, INT delType)
{
	CString query;
	if(delType == 1)
	{	
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strReckey);
		query.Format(_T("UPDATE BO_BOOK_TBL SET SEQ_NO = 0, LAST_WORK = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s' AND (WORKING_STATUS = 'BOT111O' OR WORKING_STATUS = 'BOT112O');"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			strGroupNo,
			strCreateYear);
		pDM->AddFrame(query);
	}
	else if(delType == 0)
	{
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strReckey);
		query.Format(_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s' AND (WORKING_STATUS = 'BOT111O' OR WORKING_STATUS = 'BOT112O'));"),
			strGroupNo, 
			strCreateYear);
		pDM->AddFrame(query);
		query.Format(_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s' AND (WORKING_STATUS = 'BOT111O' OR WORKING_STATUS = 'BOT112O'));"),

			strGroupNo, 
			strCreateYear);
		pDM->AddFrame(query);
		query.Format(_T("DELETE FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s' AND (WORKING_STATUS = 'BOT111O' OR WORKING_STATUS = 'BOT112O');"),
			strGroupNo, 
			strCreateYear);
		pDM->AddFrame(query);
	}
	return 0;
}

INT CGroupMgr::SearchGroup()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CString strCreator;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("Creator"), strCreator);
	
	if(!pCM || !pDM) return -1000;

	CString strQuery, strAcqYear, strGroupName, strTmpData;
	pCM->GetControlMgrData(_T("그룹생성년도"), strAcqYear);
	
	if(!strAcqYear.IsEmpty()) strQuery.Format(_T("CREATE_YEAR = '%s'"), strAcqYear);
	if(!strCreator.IsEmpty())
	{
		if(strQuery.IsEmpty()) strQuery.Format(_T("GROUP_CREATOR = '%s'"), strCreator);
		else 
		{
			strTmpData.Format(_T(" AND GROUP_CREATOR = '%s'"), strCreator);
			strQuery += strTmpData;
		}
	}	
	pCM->GetControlMgrData(_T("그룹이름"), strGroupName);
	if(!strGroupName.IsEmpty())
	{
		if(strQuery.IsEmpty()) strQuery.Format(_T("GROUP_NAME LIKE '%s%%'"), strGroupName);
		else
		{
			strTmpData.Format(_T(" AND GROUP_NAME LIKE '%s%%'"), strGroupName);
			strQuery += strTmpData;
		}
	}

	INT ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;

	SetTempData(_T("조건"), strQuery.GetBuffer(0));

	return 0;
}

INT CGroupMgr::SearchRequisitionNo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CString strCreator;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("Creator"), strCreator);

	if(!pCM || !pDM) return -1000;

	CString strQuery, strAcqYear, strTmpData;
	pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	if(!strAcqYear.IsEmpty()) strQuery.Format(_T("ACQ_YEAR = '%s'"), strAcqYear);
	if(!strCreator.IsEmpty())
	{
		if(strQuery.IsEmpty()) strQuery.Format(_T("RECEIPT_NO_CREATE_WORKER = '%s'"), strCreator);
		else
		{
			strTmpData.Format(_T(" AND RECEIPT_NO_CREATE_WORKER = '%s'"), strCreator);
			strQuery += strTmpData;
		}
	}

	pDM->RefreshDataManager(strQuery);
	
	return 0;
}

INT CGroupMgr::ExtractSelectInfo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CGroupCollection * pGroupCollection = NULL;
	CString strInfoAlias, strKeyAlias, strYearAlias;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("GroupCollection"));
	GetTempData(_T("InfoAlias"), strInfoAlias);
	GetTempData(_T("KeyAlias"), strKeyAlias);
	GetTempData(_T("YearAlias"), strYearAlias);
	
	if(!pCM || !pDM || !pGrid) return -1000;

	CStringArray arrInfo, arrKey;
	arrKey.RemoveAll();		arrInfo.RemoveAll();

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1;
	CString strInfo, strInfoData, strKey, strKeyData, strYearData;
	strInfo.Empty();
	pGroupCollection->RemoveAll();
	while (i >= 0)
	{
		pDM->GetCellData(strInfoAlias, i, strInfoData);
		if(strInfo.IsEmpty()) strInfo= strInfoData;
		else strInfo += _T(", ") + strInfoData;

		if(!strKeyAlias.IsEmpty())
		{
			pDM->GetCellData(strKeyAlias, i, strKeyData);
			if(strKey.IsEmpty()) strKey = strKeyData;
			else strKey += _T(", ") + strKeyData;
		}

		pDM->GetCellData(strYearAlias, i, strYearData);

		pGroupCollection->AddGroupInfo(strYearData, strInfoData, strKeyData);
		
		i = pGrid->SelectGetNext();
	}
	SetTempData(_T("SelInfo"), strInfo.GetBuffer(0));
	return 0;
}
INT CGroupMgr::MakeDivisionNoDM()
{
	CESL_DataMgr * pDM = NULL;
	BOOL bDivNoMgr = TRUE;
	CString strStatus, strAcqYear, strCreator, strWorkCode;
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("Status"), strStatus);
	GetTempData(_T("AcquisitYear"), strAcqYear);
	GetTempData(_T("Creator"), strCreator);
	GetTempData(_T("WorkCode"), strWorkCode);
	GetTempData(_T("DIVNO_MGR"), bDivNoMgr);
	CString whereStmt, strOption;
	BOOL isFirst = TRUE;
	
	whereStmt.Empty();
	
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T("ACQ_YEAR = '%s'"), strAcqYear); 
		if(isFirst)
		{
			whereStmt += strOption;
			isFirst = FALSE;
		}	
		else whereStmt += _T(" AND ") + strOption; 
	}
	if(!strCreator.IsEmpty())
	{
		strOption.Format(_T("SEQ_NO_CREATE_WORKER = '%s'"), strCreator);
		if(isFirst)
		{
			whereStmt += strOption;
			isFirst = FALSE;
		}	
		else whereStmt += _T(" AND ") + strOption; 
	}
	if(!strWorkCode.IsEmpty())
	{
		strOption.Format(_T("WORK_CODE = '%s'"), strWorkCode);
		if(isFirst)
		{
			whereStmt += strOption;
			isFirst = FALSE;
		}	
		else whereStmt += _T(" AND ") + strOption; 
	}
	else
	{
		strOption.Format(_T("WORK_CODE IS NULL"));
		if(isFirst)
		{
			whereStmt += strOption;
			isFirst = FALSE;
		}	
		else whereStmt += _T(" AND ") + strOption; 
	}
	pDM->RefreshDataManager(whereStmt);
	SetTempData(_T("조건"), whereStmt.GetBuffer(0));
	return 0;
}

INT CGroupMgr::CreateDivisionNoInfo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pWorkNoDM = NULL;
	CESL_DataMgr * pDM_DELETE = NULL;
	BOOL bWorkNoUseYN = FALSE;
	BOOL bDropNoApply = FALSE;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));		
	pWorkNoDM = (CESL_DataMgr*)GetTempPtr(_T("WorkNoDM"));
	GetTempData(_T("WORK_NO_USE_YN"), bWorkNoUseYN);
	GetTempData(_T("DROP_NO_APPLY"), bDropNoApply);
	if(!pDM || !pCM || !pWorkNoDM) return -1000;

	CString strGroupNo, strWorkCode, strGroupYear;
	pCM->GetControlMgrData(_T("수입년도"), strGroupYear);
	pCM->GetControlMgrData(_T("차수번호"), strGroupNo);
	pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	if(strWorkCode.IsEmpty()) strWorkCode = _T("NULL");
	INT ids = CheckDupGroupNo(pDM, _T("BO_PURCHASE_SEQ_NO_TBL"), _T("SEQ_NO"), strGroupNo, _T("ACQ_YEAR"), strGroupYear, strWorkCode);
	if(ids != 0) return ids;

	pDM->StartFrame();

	pDM->InsertRow(-1);
	INT nCurRow = pDM->GetRowCount() - 1;

	SetDMRowbyCMData(pCM, pDM, nCurRow);
	CString strReckey;
	
	pDM->MakeRecKey(strReckey);
	pDM->SetCellData(_T("구입차수KEY"), strReckey, nCurRow);
	pDM->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	if(bWorkNoUseYN) pDM->SetCellData(_T("작업번호사용여부"), _T("N"), nCurRow);
	else pDM->SetCellData(_T("작업번호사용여부"), _T("Y"), nCurRow);

	CString strWorkStatus;
	pDM->GetCellData(_T("작업상태"), nCurRow, strWorkStatus);
	if(strWorkStatus == _T("구입대상")) strWorkStatus = _T("0");
	else if(strWorkStatus == _T("주문")) strWorkStatus = _T("1");
	else if(strWorkStatus == _T("검수완료")) strWorkStatus = _T("2");
	pDM->SetCellData(_T("작업상태"), strWorkStatus, nCurRow);
	if(strWorkStatus == _T("0"))
	{
		pDM->SetCellData(_T("중복조사여부"), _T("N"), nCurRow);
		pDM->SetCellData(_T("복본조사여부"), _T("N"), nCurRow);
		pDM->SetCellData(_T("심의완료여부"), _T("N"), nCurRow);
	}
	else if(strWorkStatus == _T("1"))
	{
		pDM->SetCellData(_T("중복조사여부"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("복본조사여부"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("심의완료여부"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("주문일자"), GetTodayDate(), nCurRow);
	}
	else if(strWorkStatus == _T("2"))
	{
		pDM->SetCellData(_T("중복조사여부"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("복본조사여부"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("심의완료여부"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("주문일자"), GetTodayDate(), nCurRow);
		pDM->SetCellData(_T("검수완료일자"), GetTodayDate(), nCurRow);
	}

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("책수"));

	RemoveAliasList.Add(_T("예산집행정보KEY"));
	RemoveAliasList.Add(_T("예산집행상태"));
	RemoveAliasList.Add(_T("UDF_예산집행상태"));

	pDM->InitDBFieldData();

	pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S");

	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, nCurRow, pDM);

	pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S, CO_BUDGET_EXECUTION_TBL B");

	MakeWorkNoInfo(pDM, pWorkNoDM, _T("0"), nCurRow);
	
	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	if(strWorkCode == _T("NULL")) strWorkCode.Empty();
	if(!bDropNoApply)
		SetLastNumber(_T("PUR_NO"), _T("MO"), pDM->GetCellData(_T("수입년도"), nCurRow), strWorkCode, pDM->GetCellData(_T("차수번호"), nCurRow));
	
	return 0;
}

INT CGroupMgr::UpdateLastSerialNo()
{
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	CString strAcqYear, strDivNo, strWorkNo, strLastSerialNo, strDMPK, strTmpData;
	CESL_DataMgr * pDM_OUT = NULL;
	INT nTmpData, nAddNo;
	GetTempData(_T("ACQ_TYPE"), nTmpData);
	nAcqType = (ACQ_TYPE)nTmpData;
	GetTempData(_T("증가값"), nAddNo);
	GetTempData(_T("수입년도"), strAcqYear);
	GetTempData(_T("그룹번호"), strDivNo);
	GetTempData(_T("작업번호"), strWorkNo);
	GetTempData(_T("발주일련번호"), strLastSerialNo);
	nTmpData = _ttoi(strLastSerialNo.GetBuffer(0));
	nTmpData += nAddNo;
	strLastSerialNo.Format(_T("%d"), nTmpData);
	pDM_OUT = (CESL_DataMgr*)GetTempPtr(_T("OUT_DM"));
	
	CString strQuery;
	if(nAcqType == PURCHASE)
	{
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_WORK = '%s', LAST_SERIAL_NO = %s WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s);"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			strLastSerialNo,
			strWorkNo,
			strAcqYear,
			strDivNo
		);
	}
	else if(nAcqType == ACQ_TYPE_INVALID)
	{
		strQuery.Format(
			_T("UPDATE BO_ACQ_GROUP_TBL SET LAST_WORK = '%s', GROUP_LAST_SERIAL_NO = %s WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_SEQ = %s AND CREATE_YEAR = '%s';"),
			m_pParentMgr->GetWorkLogMsg(WORK_LOG),
			strLastSerialNo,
			strDivNo,
			strAcqYear
		);		
	}

	pDM_OUT->AddFrame(strQuery);
	return 0;
}

INT CGroupMgr::MakeWorkNoInfo()
{
	CString strAcqYear, strDivNo, strDMPK, strTmpData, strRecKey, strWorkCode;
	INT nLastWorkNo = -1;
	CESL_DataMgr * pDM_OUT = NULL;
	GetTempData(_T("수입년도"), strAcqYear);
	GetTempData(_T("차수번호"), strDivNo);
	GetTempData(_T("업무구분"), strWorkCode);
	pDM_OUT = (CESL_DataMgr*)GetTempPtr(_T("OUT_DM"));

	CESL_DataMgr * pDM = NULL;
	pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	pDM->InitDMPKFromFile(_T("DM_구입차수관리"));
	pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE= '%s'"), strAcqYear, strDivNo, strWorkCode);
	pDM->RefreshDataManager(strQuery);

	pDM_OUT->MakeRecKey(strRecKey);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	pDM_OUT->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
	pDM_OUT->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
	pDM_OUT->AddDBFieldData(_T("WORK_NO_CREATE_DATE"), _T("STRING"), GetTodayDate());
	pDM_OUT->AddDBFieldData(_T("WORK_NO_CREATE_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_NAME);
	pDM_OUT->AddDBFieldData(_T("PURCHASE_SEQ_NO_KEY"), _T("NUMERIC"), pDM->GetCellData(_T("구입차수KEY"), 0));
	strTmpData = pDM->GetCellData(_T("마지막작업번호"), 0);
	nLastWorkNo = _ttoi(strTmpData.GetBuffer(0));
	strTmpData.Format(_T("%d"), nLastWorkNo + 1);
	pDM_OUT->AddDBFieldData(_T("WORK_NO"), _T("NUMERIC"), strTmpData);
	pDM_OUT->AddDBFieldData(_T("LAST_SERIAL_NO"), _T("NUMERIC"), _T("1"));
	
	pDM_OUT->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));

	pDM_OUT->MakeInsertFrame(_T("BO_PURCHASE_WORK_NO_TBL"));

	strQuery.Format(_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET LAST_WORK = '%s', LAST_WORK_NO = %s WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE='%s';"),
		m_pParentMgr->GetWorkLogMsg(WORK_LOG),
		strTmpData,
		strAcqYear,
		strDivNo,
		strWorkCode
	);
	
	pDM_OUT->AddFrame(strQuery);
	
	return nLastWorkNo+1;
}

INT CGroupMgr::MakeWorkNoInfo(CESL_DataMgr * pDM, CESL_DataMgr * pWorkNoDM, CString strLastSerialNo, INT nCurRow)
{
	pWorkNoDM->InsertRow(-1);
	INT nRow = pWorkNoDM->GetRowCount()-1;
	INT nLastWorkNo;
	CString strRecKey, strTmpData;
	pWorkNoDM->MakeRecKey(strRecKey);
	pWorkNoDM->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRow);
	pWorkNoDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRow);
	pWorkNoDM->SetCellData(_T("작업번호생성일"), GetTodayDate(), nRow);
	pWorkNoDM->SetCellData(_T("작업번호생성자"), m_pParentMgr->m_pInfo->USER_NAME, nRow);
	strTmpData = pDM->GetCellData(_T("마지막작업번호"), nCurRow);
	nLastWorkNo = _ttoi(strTmpData.GetBuffer(0));
	strTmpData.Format(_T("%d"), nLastWorkNo);
	pWorkNoDM->SetCellData(_T("작업번호"), strTmpData, nRow);
	pWorkNoDM->SetCellData(_T("작업번호KEY"), strRecKey, nRow);
	pWorkNoDM->SetCellData(_T("구입차수KEY"), pDM->GetCellData(_T("구입차수KEY"), nCurRow), nRow);
	pWorkNoDM->SetCellData(_T("마지막일련번호"), _T("0"), nRow);
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pWorkNoDM, _T("BO_PURCHASE_WORK_NO_TBL"), nRow, pDM);

	return nLastWorkNo;
}

INT CGroupMgr::UpdateDivisionNoInfo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	INT nCurRow ;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));		
	GetTempData(_T("Index"), nCurRow);
	
	if(!pDM || !pCM) return -1000;

	SetDMRowbyCMData(pCM, pDM, nCurRow);
	CString strReckey;
	pDM->GetCellData(_T("구입차수KEY"), nCurRow, strReckey);
	
	pDM->StartFrame();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("책수"));
	RemoveAliasList.Add(_T("구입차수KEY"));

	RemoveAliasList.Add(_T("예산집행정보KEY"));
	RemoveAliasList.Add(_T("예산집행상태"));	
	pDM->InitDBFieldData();

	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("BO_PURCHASE_SEQ_NO_TBL S"), nCurRow, pDM, _T("REC_KEY"), _T("NUMERIC"), strReckey, nCurRow);
	
	INT ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	return 0;
}

INT CGroupMgr::SetDMRowbyCMData(CESL_ControlMgr * pCM, CESL_DataMgr * pDM, INT nCurRow)
{
	INT ids;

	if(!pCM || !pDM)
		return -1000;

	INT controlCnt = pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = pCM->Control_List.GetHeadPosition();
	CString strTmp, alias;

	for(INT i=0 ; i<controlCnt && pos != NULL; i++)
	{
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (!pCE) continue;		
		alias = pCE->GetAlias();
		ids = pCM->GetControlMgrData(alias, strTmp);
		if (ids < 0) continue;
		strTmp.TrimLeft();	strTmp.TrimRight();
		pDM->SetCellData(alias, strTmp, nCurRow);		
	}
	
	return 0;
}

CString CGroupMgr::FindDMPK(CString strDMAlias)
{
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"));
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// 정보파일을 찾을 수 없음
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR szBuf[16000];
	CString strLine;
	INT pos;
	

	INT nStart = 0;
	CString PK, ALIAS;
	CString result = _T("");
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();
		
		nStart = 0;

		pos = strLine.Find('|', nStart);
		PK = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		PK.TrimLeft();		PK.TrimRight();
		ALIAS.TrimLeft();	ALIAS.TrimRight();

		if (ALIAS == strDMAlias) {
			result = PK;
			break;
		}
	}
	fd.Close();
	return result;
}

INT CGroupMgr::InsertGroup()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CString strLastNo;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("그룹번호"), strLastNo);
	
	if(!pCM || !pDM) return -1000;

	CString strGroupYear;
	pCM->GetControlMgrData(_T("그룹생성년도"), strGroupYear);
	INT ids = CheckDupGroupNo(pDM, _T("BO_ACQ_GROUP_TBL"), _T("GROUP_SEQ"), strLastNo, _T("CREATE_YEAR"), strGroupYear);
	if(ids != 0) return ids;

	pDM->InsertRow(-1);
	INT nCurRow = pDM->GetRowCount() - 1;
	SetDMRowbyCMData(pCM, pDM, nCurRow);
	CString strToday, strReckey, strGroupNo, strQuery;
	strToday = GetTodayDate();
	pDM->MakeRecKey(strReckey);
	pDM->SetCellData(_T("그룹생성일자"), strToday, nCurRow);
	pDM->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("그룹번호"), strLastNo, nCurRow);
	pDM->SetCellData(_T("그룹KEY"), strReckey, nCurRow);
	pDM->SetCellData(_T("그룹책수"), _T("0"), nCurRow);

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("그룹책수"));
	
	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, nCurRow, pDM);

	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	SetLastNumber(_T("GRP_NO"), _T("MO"), pDM->GetCellData(_T("그룹생성년도"), nCurRow), _T(""), pDM->GetCellData(_T("그룹번호"), nCurRow));
	return 0;
}

INT CGroupMgr::UpdateGroup()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	INT idx;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("Index"), idx);

	if(!pCM || !pDM) return -1000;

	SetDMRowbyCMData(pCM, pDM, idx);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), idx);

	CString strReckey = pDM->GetCellData(_T("그룹KEY"), idx);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("그룹책수"));
	RemoveAliasList.Add(_T("그룹KEY"));
	
	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, idx, pDM, _T("REC_KEY"), _T("NUMERIC"), strReckey, idx);
	
	INT ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;
	return 0;
}

CString CGroupMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CGroupMgr::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CGroupMgr::GetTotalPriceAndBookCount()
{
	INT acqCode = -1;
	CString strAcqYear, strDivNo, strTotalPrice, strBookCnt, strWorkCode, str64;
	GetTempData(_T("AcqCode"), acqCode);
	GetTempData(_T("AcqYear"), strAcqYear);
	GetTempData(_T("DivNo"), strDivNo);

	GetTempData(_T("str64"), str64);
	if(acqCode == 1)
		GetTempData(_T("WorkCode"), strWorkCode);
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	
	CString headQuery, strAccuTbl;
	if(acqCode == 1) strAccuTbl = _T("bo_purchaseinfo_tbl p");
	else if(acqCode == 2) strAccuTbl = _T("bo_doninfo_tbl p");
	if(acqCode == 1)
	{
		if(strWorkCode.IsEmpty())
		{
			headQuery.Format(
				_T("SELECT COUNT (b.rec_key), ROUND(SUM(GetPriceWon(b.acq_key, b.price, 'Y', UDF_MANAGE_CODE)),%s) FROM idx_bo_tbl s, bo_book_tbl b, %s WHERE s.rec_key = p.species_key and s.MANAGE_CODE = UDF_MANAGE_CODE AND b.acq_key = p.rec_key and b.book_appendix_flag='B' and b.acq_code = '%d' and b.acq_year = '%s' and b.seq_no = '%s' and s.work_code is null"),
				str64,strAccuTbl, acqCode, strAcqYear, strDivNo);	
		}
		else
		{
			headQuery.Format(
				_T("SELECT COUNT (b.rec_key), ROUND(SUM(GetPriceWon(b.acq_key, b.price, 'Y', UDF_MANAGE_CODE)),%s) FROM idx_bo_tbl s, bo_book_tbl b, %s WHERE s.rec_key = p.species_key and s.MANAGE_CODE = UDF_MANAGE_CODE AND b.acq_key = p.rec_key and b.book_appendix_flag='B' and b.acq_code = '%d' and b.acq_year = '%s' and b.seq_no = '%s' and s.work_code = '%s'"),
				str64, strAccuTbl, acqCode, strAcqYear, strDivNo, strWorkCode);	
		}
	}
	else if(acqCode == 2)
	{
		headQuery.Format(
			_T("SELECT COUNT (b.rec_key), SUM (b.price) FROM bo_book_tbl b, %s WHERE b.acq_key = p.rec_key and b.MANAGE_CODE = UDF_MANAGE_CODE AND b.book_appendix_flag='B' and b.acq_code = '%d' and b.acq_year = '%s' and b.seq_no = '%s'"),
			strAccuTbl, acqCode, strAcqYear, strDivNo);
	}
	else 
	{
		headQuery.Format(
			_T("SELECT COUNT (b.rec_key), SUM (b.price) FROM bo_book_tbl b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.book_appendix_flag='B' and b.acq_code = '%d' and b.acq_year = '%s' and b.seq_no = '%s'"),
			acqCode, strAcqYear, strDivNo);
	}
	INT ids = tmpDM.RestructDataManager(headQuery);
	if(ids < 0) return ids;
	
	strTotalPrice = tmpDM.GetCellData(0, 1);
	strBookCnt = tmpDM.GetCellData(0, 0);
	
	SetTempData(_T("TotalPrice"), strTotalPrice.GetBuffer(0));
	SetTempData(_T("BookCount"), strBookCnt.GetBuffer(0));
	return 0;
}

INT CGroupMgr::MakeDivNoDataInfoDM()
{
	INT acqCode = -1;
	CString strAcqYear, strDivNo, strTotalPrice, strWorkCode,str64;
	INT totalBookCnt = -1;
	DOUBLE totalPrice = 0.00;

	CESL_DataMgr * pDM = NULL;
	GetTempData(_T("AcqCode"), acqCode);
	GetTempData(_T("AcqYear"), strAcqYear);
	GetTempData(_T("DivNo"), strDivNo);
	GetTempData(_T("BookCount"), totalBookCnt);
	GetTempData(_T("TotalPrice"), strTotalPrice);
	
	if( acqCode == 1 )		GetTempData(_T("WorkCode"), strWorkCode);
	strTotalPrice.Replace(_T(","), _T(""));

	TCHAR* pEndPtr = NULL;
	totalPrice = _tcstod(strTotalPrice.GetBuffer(0), &pEndPtr);

	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));

	pDM->FreeData();

	CString headQuery, strAccuTbl;
	if(acqCode == 1) strAccuTbl = _T("bo_purchaseinfo_tbl p");
	else if(acqCode == 2) strAccuTbl = _T("bo_doninfo_tbl p");
	else strAccuTbl.Empty();

	if(acqCode == 1)
	{
		if(strWorkCode.IsEmpty())
		{
			headQuery.Format(_T("SELECT b.working_status, COUNT (b.rec_key), ")
							_T("GET_ORDERPRICE(SUM(GetPriceWon(b.acq_key, b.price, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
							_T("FROM idx_bo_tbl s, bo_book_tbl b, %s ")
							_T("WHERE s.rec_key = b.species_key ")
							_T("and b.acq_key = p.rec_key(+) ")							
							_T("and s.MANAGE_CODE = UDF_MANAGE_CODE AND b.acq_code = '%d' ")
							_T("and b.acq_year = '%s' ")
							_T("and b.seq_no = '%s' ")
							_T("and s.work_code is null"),
							strAccuTbl, acqCode, strAcqYear, strDivNo);
		}
		else 
		{
			headQuery.Format(_T("SELECT b.working_status, COUNT (b.rec_key), ")
							_T("GET_ORDERPRICE(SUM(GetPriceWon(b.acq_key, b.price, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
							_T("FROM idx_bo_tbl s, bo_book_tbl b, %s ")
							_T("WHERE s.rec_key = b.species_key ")
							_T("and b.acq_key = p.rec_key(+) ")							
							_T("and s.MANAGE_CODE = UDF_MANAGE_CODE AND b.acq_code = '%d' ")
							_T("and b.acq_year = '%s' ")
							_T("and b.seq_no = '%s' ")
							_T("and s.work_code = '%s'"),
							strAccuTbl, acqCode, strAcqYear, strDivNo, strWorkCode);
		}
	}
	else if(acqCode == 2)
	{
		headQuery.Format(_T("SELECT b.working_status, COUNT (b.rec_key), SUM(b.price) ")
						_T("FROM bo_book_tbl b, %s ")
						_T("WHERE b.acq_key = p.rec_key(+) AND b.MANAGE_CODE = UDF_MANAGE_CODE ")
						_T("and b.book_appendix_flag='B' ")
						_T("and b.acq_code = '%d' ")
						_T("and b.acq_year = '%s' ")
						_T("and b.seq_no = '%s'"),
			strAccuTbl, acqCode, strAcqYear, strDivNo);
	}
	else
	{
		headQuery.Format(_T("SELECT b.working_status, COUNT(b.rec_key), SUM(b.price) ")
						_T("FROM bo_book_tbl b ")
						_T("WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.book_appendix_flag='B' ")
						_T("and b.acq_code = '%d' ")
						_T("and b.acq_year = '%s' ")
						_T("and b.seq_no = '%s'"),
			acqCode, strAcqYear, strDivNo);
	}
	
			
	headQuery += _T(" GROUP BY b.working_status");

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	INT ids = tmpDM.RestructDataManager(headQuery);
	if(ids < 0) return -1002;

	INT bookCnt = -1;
	DOUBLE price = 0.0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	CString strTmpData;
	INT cnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<cnt ; i++)
	{
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("자료상태"), tmpDM.GetCellData(i, 0), nRow);
		bookCnt = _ttoi((tmpDM.GetCellData(i, 1)).GetBuffer(0));
		bookrate = (float)bookCnt/totalBookCnt * 100;
		strTmpData.Format(_T("%d"), bookCnt);
		pDM->SetCellData(_T("책수"), strTmpData, nRow);
		strTmpData.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책수비율"), strTmpData, nRow);

		TCHAR* pEndPtr = NULL;
		price = _tcstod(tmpDM.GetCellData(i, 2).GetBuffer(0), &pEndPtr);

		strTmpData.Format(_T("%.0f"), price);
		pDM->SetCellData(_T("금액"), strTmpData, nRow);
		if( totalPrice == 0.0 )		pricerate = 0;
		else						pricerate = (float)(price/totalPrice * 100);
		strTmpData.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), strTmpData, nRow);
		bookCnt = 0;
		price = 0.0;
		bookrate = 0.0;
		pricerate = 0.0;
	}

	return 0;	
}

INT CGroupMgr::CheckGroupNoValidate()
{
	INT nAcqCode;
	CString strAcqYear, strWorkStatus, strGroupNo, strWorkCode, strQuery, strResult;
	CESL_DataMgr tmpDM;

	GetTempData(_T("수입구분"), nAcqCode		);
	GetTempData(_T("수입년도"), strAcqYear		);
	GetTempData(_T("작업상태"), strWorkStatus	);
	GetTempData(_T("그룹번호"), strGroupNo		);
	GetTempData(_T("업무구분"), strWorkCode		);

	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	switch( nAcqCode )
	{
	case 1:
	{
			if( strWorkStatus.IsEmpty() == TRUE )
			{
				if( strWorkCode.IsEmpty() == TRUE )
		{
					strQuery.Format(_T("SELECT REC_KEY ")
									_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
									_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
									_T("AND SEQ_NO = %s ")
									_T("AND WORK_CODE IS NULL "), strAcqYear, strGroupNo);
				}
			else
				{
					strQuery.Format(_T("SELECT REC_KEY ")
									_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
									_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
									_T("AND SEQ_NO = %s ")
									_T("AND WORK_CODE='%s'"), strAcqYear, strGroupNo, strWorkCode);
				}
		}
		else
		{
			if		( strWorkStatus == _T("구입대상")	)	strWorkStatus = _T("0");
			else if	( strWorkStatus == _T("주문")		)	strWorkStatus = _T("1");
			else if	( strWorkStatus == _T("검수완료")	)	strWorkStatus = _T("2");			

			if( strWorkCode.IsEmpty() == TRUE )
			{
				strQuery.Format(_T("SELECT REC_KEY ")
								_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
								_T("AND SEQ_NO = %s ")
								_T("AND WORK_CODE IS NULL"), strAcqYear, strGroupNo);
			}
		else
			{
				strQuery.Format(_T("SELECT REC_KEY ")
								_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
								_T("AND SEQ_NO = %s ")
								_T("AND WORK_CODE='%s'"),strAcqYear, strGroupNo, strWorkCode);
			}
	
		}

		tmpDM.GetOneValueQuery(strQuery, strResult);
			if( strResult.IsEmpty() == TRUE )	return -1;
	}
		break;

	case 2:
	{
			strQuery.Format(_T("SELECT REC_KEY ")
							_T("FROM BO_DON_RECEIPT_TBL ")
							_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' ")
							_T("AND RECEIPT_NO = %s"), strAcqYear, strGroupNo);

		tmpDM.GetOneValueQuery(strQuery, strResult);
			if( strResult.IsEmpty() == TRUE )	return -1;
	}
		break;

	case 0:
	{
			strQuery.Format(_T("SELECT REC_KEY ")
							_T("FROM BO_ACQ_GROUP_TBL ")
							_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' ")
							_T("AND GROUP_NAME = '%s'"), strAcqYear, strGroupNo);

		tmpDM.GetOneValueQuery(strQuery, strResult);
			if( strResult.IsEmpty() == TRUE )	return -1;
	}
		break;

	default:
	{
			strQuery.Format(_T("SELECT REC_KEY ")
							_T("FROM BO_ACQ_SEQ_NO_TBL ")
							_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='%d' ")
							_T("AND CREATE_YEAR = '%s' ")
							_T("AND GROUP_SEQ = '%s'"),	nAcqCode, strAcqYear, strGroupNo);

		tmpDM.GetOneValueQuery(strQuery, strResult);
			if( strResult.IsEmpty() == TRUE )	return -1;
		}
		break;
	}

	return 0;
}

INT CGroupMgr::DeleteDivisionNoInfo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_DataMgr * pDM_DELETE = NULL;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pDM_DELETE = (CESL_DataMgr*)GetTempPtr(_T("DELETE_DM"));

	if(!pCM || !pDM || !pGrid || !pDM_DELETE) return -1000;
	
	INT i = pGrid->SelectMakeList();
	i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -5000;
	CString strLastNo, strCreateYear, strReckey, strGroupNo, strTmpData, strAcqYear, strWorkStatus;
	INT delType = 0;
	CArray <INT, INT> idxArray;		idxArray.RemoveAll();
	INT ids = -1;

	CGroupInfo TargetGroup;
	CGroupCollection SourceCollection(m_pParentMgr);
	SourceCollection.SetAcqType(PURCHASE);
	
	pDM->StartFrame();
	
	while(i >= 0)
	{
		CGroupInfo pTmpGroup;
		strAcqYear = pDM->GetCellData(_T("수입년도"), i);
		strWorkStatus = pDM->GetCellData(_T("작업상태"), i);
		pTmpGroup.SetAcqYear(strAcqYear);
		pTmpGroup.SetGroupInfo(pDM->GetCellData(_T("차수번호"), i));
		pTmpGroup.SetLastWorkNo(pDM->GetCellData(_T("마지막작업번호"), i));
		pTmpGroup.SetWorkCode(pDM->GetCellData(_T("업무구분"), i));
		SourceCollection.AddGroupInfo(&pTmpGroup);
		i = pGrid->SelectGetNext();		
	}
	
	CCheckHoldDataDlg dlg(m_pParentMgr);
	dlg.SetAcqType(PURCHASE);
	dlg.SetGroupCollection(&SourceCollection);
	if(IDOK != dlg.DoModal()) return 0;

	if(dlg.GetOccurError()) return -1;
	
	ids = DeleteDataByGroup(pDM_DELETE, &SourceCollection, PURCHASE);
	if(ids < 0) return -1;
	if(ids == 1000) return ids;

	i = pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		if(SourceCollection.IsIncludeGroupInfo(pDM->GetCellData(_T("수입년도"), i), pDM->GetCellData(_T("차수번호"), i), pDM->GetCellData(_T("업무구분"), i)))
		{
			strTmpData = pDM->GetCellData(_T("구입차수KEY"), i);
			pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S");
			pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
			pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S, CO_BUDGET_EXECUTION_TBL B");
			pDM->MakeDeleteFrame(_T("BO_PURCHASE_WORK_NO_TBL"), _T("PURCHASE_SEQ_NO_KEY"), _T("NUMERIC"), strTmpData);
			idxArray.Add(i);
		}
		i = pGrid->SelectGetNext();
	}
	
	CBO_BudgetMgr BudgetMgr(m_pParentMgr);
	BudgetMgr.SetDMOut(pDM);
	BudgetMgr.SetGroupCollection(&SourceCollection);
	BUDGET_ERROR_TYPE res = BudgetMgr.DeleteBudgetExecutionInfo();
	if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
		return -1;

	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	if(delType == 0) DeleteSelectRow(pDM, &idxArray);

	return 0;
}

INT CGroupMgr::DeleteDivisionNoInfo(CString strCreateYear, CString strReckey, CString strGroupNo, CESL_DataMgr *pDM, INT delType)
{
	CString query;
	if(delType == 0)
	{	
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strReckey);
		pDM->MakeDeleteFrame(_T("BO_PURCHASE_WORK_NO_TBL"), _T("PURCHASE_SEQ_NO_KEY"), _T("NUMERIC"), strReckey);
		query.Format(
			_T("DELETE FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s';"),
			strGroupNo,
			strCreateYear);
		pDM->AddFrame(query);
		query.Format(
			_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s';"),
			strGroupNo,
			strCreateYear);
		pDM->AddFrame(query);
		query.Format(
			_T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY IN (SELECT SPECIES_KEY FROM BO_PURCHASEINFO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s');"),
			strGroupNo,
			strCreateYear);
		pDM->AddFrame(query);
		query.Format(
			_T("DELETE FROM IDX_BO_TBL WHERE REC_KEY IN (SELECT SPECIES_KEY FROM BO_PURCHASEINFO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO = %s AND ACQ_YEAR = '%s');"),
			strGroupNo,
			strCreateYear);

		pDM->AddFrame(query);
	}
	else if(delType == 1)
	{
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strReckey);
		pDM->MakeDeleteFrame(_T("BO_PURCHASE_WORK_NO_TBL"), _T("PURCHASE_SEQ_NO_KEY"), _T("NUMERIC"), strReckey);
	}
	return 0;
}

INT CGroupMgr::MakeGroupInfo()
{
	CString strWorkStatus, strAcqYear, strGroupNo;
	ACQ_TYPE nAcqType;
	INT tmpData = -1;
	CArray <CGroupInfo *, CGroupInfo *> * pGroupInfo = NULL;
	GetTempData(_T("ACQ_TYPE"), tmpData);
	nAcqType = (ACQ_TYPE)tmpData;
	if(nAcqType == PURCHASE)
		GetTempData(_T("작업상태"), strWorkStatus);
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	GetTempData(_T("GroupNo"), strGroupNo);
	pGroupInfo = (CArray <CGroupInfo *, CGroupInfo *>*)GetTempPtr(_T("GroupInfo"));

	CString strDMPK, strDMAlias;
	if(nAcqType == PURCHASE)
		strDMAlias = _T("DM_구입차수관리");
	else if(nAcqType == DONATE)
		strDMAlias = _T("DM_접수번호관리");
	else if(nAcqType == ACQ_TYPE_INVALID )
		strDMAlias = _T("DM_그룹관리");
	CESL_DataMgr * pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	pDM->InitDMPKFromFile(strDMAlias);
	pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CString * strGroupInfoAlias = NULL;
	INT nAliasCnt = -1;
	CString strQuery, strOption;
	if(nAcqType == PURCHASE)
	{
		strGroupInfoAlias = new CString[3];
		strGroupInfoAlias[0] = _T("차수번호");
		strGroupInfoAlias[1] = _T("수입년도");
		strGroupInfoAlias[2] = _T("마지막작업번호");
		nAliasCnt = 3;
		if(strWorkStatus == _T("구입대상")) strWorkStatus = _T("0");
		else if(strWorkStatus == _T("주문")) strWorkStatus = _T("1");
		else if(strWorkStatus == _T("검수완료")) strWorkStatus = _T("2");		
		strQuery.Format(_T("ACQ_YEAR='%s'"), strAcqYear);
		if(!strGroupNo.IsEmpty())
		{
			strOption.Format(_T(" AND SEQ_NO IN (%s)"), strGroupNo);
			strQuery += strOption;
		}
	}
	else if(nAcqType == DONATE)
	{
		strGroupInfoAlias = new CString[2];
		strGroupInfoAlias[0] = _T("접수번호");
		strGroupInfoAlias[1] = _T("수입년도");
		nAliasCnt = 2;
		strQuery.Format(_T("ACQ_YEAR='%s'"), strAcqYear);
		if(!strGroupNo.IsEmpty())
		{
			strOption.Format(_T(" AND RECEIPT_NO IN (%s)"), strGroupNo);
			strQuery += strOption;
		}
	}
	else if(nAcqType == ACQ_TYPE_INVALID)
	{
		strGroupInfoAlias = new CString[4];
		strGroupInfoAlias[0] = _T("그룹이름");
		strGroupInfoAlias[1] = _T("그룹생성년도");
		strGroupInfoAlias[2] = _T("그룹번호");
		strGroupInfoAlias[3] = _T("마지막일련번호");
		nAliasCnt = 4;
		strQuery.Format(_T("CREATE_YEAR='%s'"), strAcqYear);
		if(!strGroupNo.IsEmpty())
		{
			strOption.Format(_T(" AND GROUP_NAME IN (%s)"), strGroupNo);
			strQuery += strOption;
		}
	}

	INT ids = MakeGroupInfo(pDM, strQuery, strGroupInfoAlias, nAliasCnt, pGroupInfo);

	if(pDM)
	{
		delete pDM;
		pDM = NULL;
	}
	if(strGroupInfoAlias)
	{
		delete[] strGroupInfoAlias;
		strGroupInfoAlias = NULL;
	}
	return ids;
}	

INT CGroupMgr::MakeGroupInfo(CESL_DataMgr * pDM, CString strQuery, CString * strGroupInfoAlias, 
							 INT nAliasCnt, CArray<CGroupInfo *, CGroupInfo *> * pGroupInfo)
{
	INT ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	CString * strGroupInfoData = new CString[nAliasCnt];
	INT nRowCnt = pDM->GetRowCount();

	INT j = -1;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pDM->GetCellData(strGroupInfoAlias, nAliasCnt, strGroupInfoData, i);
		
		pGroup = new CGroupInfo();
		pGroup->SetGroupInfo(strGroupInfoData[0]);
		pGroup->SetAcqYear(strGroupInfoData[1]);
		if(nAliasCnt == 3)
		{
			pGroup->SetLastWorkNo(strGroupInfoData[2]);
		}
		else if(nAliasCnt == 4)
		{
			pGroup->SetGroupNo(strGroupInfoData[2]);
			pGroup->SetLastSerialNo(_ttoi(strGroupInfoData[3].GetBuffer(0)));
		}
		pGroupInfo->Add(pGroup);
	}

	if(strGroupInfoData)
	{
		delete[] strGroupInfoData;
		strGroupInfoData = NULL;
	}
	return 0;
}

INT CGroupMgr::CheckDuplicateGroupName()
{
	CString strAcqYear, strGroupName;
	GetTempData(_T("CREATE_YEAR"), strAcqYear);
	GetTempData(_T("GROUP_NAME"), strGroupName);
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
		_T("SELECT REC_KEY FROM BO_ACQ_GROUP_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' AND GROUP_NAME = '%s'"),
		strAcqYear,
		strGroupName
	);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() > 0)
		return -1;
	return 0;
}

INT CGroupMgr::GetGroupDataCount()
{
	CString strAcqYear, strGroupNo;
	BOOL bSpecies = TRUE;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	INT nTmpData = 0;
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	GetTempData(_T("GROUP_NO"), strGroupNo);
	GetTempData(_T("SPECIES_YN"), bSpecies);
	GetTempData(_T("ACQ_TYPE"), nTmpData);
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	nAcqType = (ACQ_TYPE)nTmpData;

	CString strQuery;
	if(bSpecies && nAcqType == PURCHASE)
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
			strAcqYear, strGroupNo);
	}
	else if(bSpecies && nAcqType == DONATE)
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_DONINFO_TBL WHERE ACQ_YEAR='%s' AND RECEIPT_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
			strAcqYear, strGroupNo);
	}
	else if(bSpecies && nAcqType == ACQ_TYPE_INVALID)
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_SPECIES_TBL WHERE REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE (ACQ_CODE='0' OR ACQ_CODE IS NULL) AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE)"),
			strAcqYear, strGroupNo);
	}
	else if(!bSpecies && nAcqType == PURCHASE)
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
			strAcqYear, strGroupNo);
	}
	else if(!bSpecies && nAcqType == DONATE)
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE ACQ_CODE='2' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
			strAcqYear, strGroupNo);
	}
	else
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE (ACQ_CODE='0' OR ACQ_CODE IS NULL) AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
			strAcqYear, strGroupNo);
	}
	if(!pDM) return -1;
	CString strResult;
	pDM->GetOneValueQuery(strQuery, strResult);
	if(strResult.IsEmpty()) return 0;
	return _ttoi(strResult.GetBuffer(0));
}	

INT CGroupMgr::DeleteRecycleDataByGroup(CESL_DataMgr *pDM_DELETE, CGroupCollection *pGroupCollection)
{
	if(pDM_DELETE == NULL || pGroupCollection == NULL) return -1;

	INT ids = -1;
	CString strQuery, strMsg, strGroupNoList;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<pGroupCollection->GetGroupCount() ; i++)
	{
		pGroup = pGroupCollection->GetGroupInfo(i);
		if(strGroupNoList.IsEmpty()) strGroupNoList = pGroup->GetGroupNo();
		else strGroupNoList += _T(", ") + pGroup->GetGroupNo();
	}

	if(strGroupNoList.IsEmpty()) return 0;

	strQuery.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_DELETE_BOOK_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO IN (%s))"),
		pGroup->GetAcqYear(), strGroupNoList);

	ids = pDM_DELETE->RefreshDataManager(strQuery);
	if(ids < 0) return ids;
	
	CString strSpeciesKey, strValue;
	
	INT nRowCnt = pDM_DELETE->GetRowCount();
	
	INT nPos = 1;
	ShowDataProcessingDlg();
	strMsg.Format(_T("%d건의 자료를 삭제중입니다."), nRowCnt);
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	
	for(i=0 ; i<nRowCnt ; i++)
	{
		pDM_DELETE->GetCellData(_T("IBS_종KEY"), i, strSpeciesKey);
		if(strSpeciesKey.IsEmpty()) continue;
		
		pDM_DELETE->StartFrame();
		
		pDM_DELETE->MakeDeleteFrame(_T("BO_DELETE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
		pDM_DELETE->MakeDeleteFrame(_T("BO_DELETE_BOOK_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), strSpeciesKey);
		pDM_DELETE->MakeDeleteFrame(_T("BO_PURCHASEINFO_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), strSpeciesKey);
		pDM_DELETE->MakeDeleteFrame(_T("BO_DONINFO_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), strSpeciesKey);
		pDM_DELETE->MakeDeleteFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
		
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey);
			pDM_DELETE->AddFrame(strQuery);
		}

		pDM_DELETE->MakeDeleteFrame(_T("BO_EBOOK_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), strSpeciesKey);
		
		strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"), strSpeciesKey);
		pDM_DELETE->GetOneValueQuery(strQuery, strValue);
		if(!strValue.IsEmpty())
		{
			strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
			pDM_DELETE->AddFrame(strQuery);
			strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s;"), strSpeciesKey);
			pDM_DELETE->AddFrame(strQuery);
			strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
			pDM_DELETE->AddFrame(strQuery);
			strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
			pDM_DELETE->AddFrame(strQuery);
		}
		ids = pDM_DELETE->SendFrame();
		if(ids<0) return -1;
		ids = pDM_DELETE->EndFrame();
		if(ids<0) return -1;
	}
	
	DeleteDataProcessingDlg();
	
	return 0;	
}

INT CGroupMgr::DeleteDataByGroup(CESL_DataMgr * pDM, CGroupCollection *pGroupCollection, ACQ_TYPE nAcqType)
{
	INT ids = -1;
	CString strQuery, strMsg;
	MakeQueryByGroup(pGroupCollection, nAcqType, strQuery);
	if(strQuery.IsEmpty())	return 1000;
	
	ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;
	
	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-7000);
	api.InitDataMgr();
	CString strSpeciesKey, strAcqKey;
	INT nSpeciesType = -1;
	if(nAcqType == ACQ_TYPE_INVALID)
	{
		nSpeciesType = (INT)BO_BASIC;
		strAcqKey = _T("-1");
	}
	else if(nAcqType == PURCHASE) nSpeciesType = (INT)BO_PURCHASE;
	else if(nAcqType == DONATE) nSpeciesType = (INT)BO_DONATE;
	else if(nAcqType == OTHER_ACQ_TYPE) nSpeciesType = (INT)BO_REG_INVALID;

	INT nRowCnt = pDM->GetRowCount();

	CGroupInfo pTargetGroup;
	pTargetGroup.SetAcqYear(GetTodayYear());
	pTargetGroup.SetGroupInfo(_T(""));
	pTargetGroup.SetGroupNo(_T("0"));
	
	if(nAcqType != ACQ_TYPE_INVALID && nRowCnt > 0)
	{
		CGroup_Select_Dlg dlg(m_pParentMgr);
		dlg.SetSpeciesType(BO_RECYCLE);
		dlg.SetAcqYear(GetTodayYear());
		
		if(IDOK == dlg.DoModal())
		{
			pTargetGroup.SetAcqYear(dlg.GetSelGroup()->GetAcqYear());
			pTargetGroup.SetGroupInfo(dlg.GetSelGroup()->GetGroupInfo());
			pTargetGroup.SetGroupNo(dlg.GetSelGroup()->GetGroupNo());
		}
	}

	INT nPos = 1;
	ShowDataProcessingDlg();
	strMsg.Format(_T("%d건의 자료를 삭제중입니다."), nRowCnt);
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);
		if(nAcqType != ACQ_TYPE_INVALID && nAcqType != OTHER_ACQ_TYPE)
			strAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), i);
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(nAcqType == ACQ_TYPE_INVALID)
			ids = api.BO_SPECIES_DELETE(strSpeciesKey, strAcqKey, (INT)nAcqType);
		else
			ids = api.BO_SPECIES_RECYCLE(strSpeciesKey, strAcqKey, (INT)nAcqType, &pTargetGroup);
		if(ids < 0) return -1;
	}

	DeleteDataProcessingDlg();
	
	return 0;
}

INT CGroupMgr::MakeQueryByGroup(CGroupCollection *pGroupCollection, ACQ_TYPE nAcqType, CString &strQuery)
{
	INT i=0;
	INT nGroupCount = pGroupCollection->GetGroupCount();
	if(nGroupCount == 0) return 0;

	CString strGroupNoList, strWorkCode, strGroupNo, strGroupYear, strTmpData;
	CGroupInfo * pGroup = NULL;
	if(nAcqType == PURCHASE)
	{
		for(i=0 ; i<nGroupCount ; i++)
		{
			pGroup = pGroupCollection->GetGroupInfo(i);
			if(strGroupNoList.IsEmpty()) 
			{
				strGroupNo = pGroup->GetGroupInfo();
				strWorkCode = pGroup->GetWorkCode();
				strGroupYear = pGroup->GetAcqYear();
				if(strWorkCode.IsEmpty())
					strTmpData.Format(_T("(P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.WORK_CODE IS NULL)"), strGroupYear, strGroupNo, strWorkCode);
				else 
					strTmpData.Format(_T("(P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.WORK_CODE='%s')"), strGroupYear, strGroupNo, strWorkCode);
				strGroupNoList = strTmpData;
			}
			else
			{
				strGroupNo = pGroup->GetGroupInfo();
				strWorkCode = pGroup->GetWorkCode();
				strGroupYear = pGroup->GetAcqYear();
				if(strWorkCode.IsEmpty())
					strTmpData.Format(_T("(P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.WORK_CODE IS NULL)"), strGroupYear, strGroupNo, strWorkCode);
				else 
					strTmpData.Format(_T("(P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.WORK_CODE='%s')"), strGroupYear, strGroupNo, strWorkCode);
				strGroupNoList += _T(" OR ") + strTmpData;
			}
		}
		strQuery.Format(_T("I.REC_KEY=P.SPECIES_KEY AND (%s)"),
			strGroupNoList);
	}
	else if(nAcqType == DONATE)
	{
		for(i=0 ; i<nGroupCount ; i++)
		{
			pGroup = pGroupCollection->GetGroupInfo(i);
			if(strGroupNoList.IsEmpty()) strGroupNoList = pGroup->GetGroupInfo();
			else strGroupNoList += _T(", ") + pGroup->GetGroupInfo();
		}
		strQuery.Format(_T("I.REC_KEY=P.SPECIES_KEY AND P.ACQ_YEAR='%s' AND P.RECEIPT_NO IN (%s)"),
			pGroup->GetAcqYear(), strGroupNoList);
	}
	else if(nAcqType == ACQ_TYPE_INVALID)
	{
		for(i=0 ; i<nGroupCount ; i++)
		{
			pGroup = pGroupCollection->GetGroupInfo(i);
			if(strGroupNoList.IsEmpty()) strGroupNoList = pGroup->GetGroupNo();
			else strGroupNoList += _T(", ") + pGroup->GetGroupNo();
		}
		strQuery.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE (ACQ_CODE='0' OR ACQ_CODE IS NULL) AND ACQ_YEAR='%s' AND SEQ_NO IN (%s))"),
			pGroup->GetAcqYear(), strGroupNoList);
	}
	else if(nAcqType == OTHER_ACQ_TYPE)
	{
		for(i=0 ; i<nGroupCount ; i++)
		{
			pGroup = pGroupCollection->GetGroupInfo(i);
			if(strGroupNoList.IsEmpty()) strGroupNoList = pGroup->GetGroupNo();
			else strGroupNoList += _T(", ") + pGroup->GetGroupNo();
		}
		strQuery.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO IN (%s))"),
			pGroupCollection->GetAcqCode(),	pGroup->GetAcqYear(), strGroupNoList);
	}
	return 0;
}

INT CGroupMgr::DeleteReceiptNoInfo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_DataMgr * pDM_DELETE = NULL;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pDM_DELETE = (CESL_DataMgr*)GetTempPtr(_T("DELETE_DM"));
	
	if(!pCM || !pDM || !pGrid || !pDM_DELETE) return -1000;
	
	INT i = pGrid->SelectMakeList();
	i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -5000;
	CString strLastNo, strCreateYear, strReckey, strGroupNo, strTmpData, strAcqYear;
	INT delType = 0;
	CArray <INT, INT> idxArray;		idxArray.RemoveAll();

	CGroupInfo TargetGroup;
	CGroupCollection SourceCollection(m_pParentMgr);
	SourceCollection.SetAcqType(DONATE);
	
	INT ids;
	CString strBookCnt;
	INT nBookCnt;
	INT nTotalBookCnt;
	
	pDM->StartFrame();
	nTotalBookCnt = 0;	
	while(i >= 0)
	{
		CGroupInfo pTmpGroup;
		strAcqYear = pDM->GetCellData(_T("수입년도"), i);
		pTmpGroup.SetAcqYear(strAcqYear);
		pTmpGroup.SetGroupInfo(pDM->GetCellData(_T("접수번호"), i));
		SourceCollection.AddGroupInfo(&pTmpGroup);
		ids = pDM->GetCellData(_T("책수"), i, strBookCnt);
		nBookCnt = _ttoi(strBookCnt);
		nTotalBookCnt += nBookCnt;
		i = pGrid->SelectGetNext();
		
	}


	CCheckHoldDataDlg dlg(m_pParentMgr);
	dlg.SetAcqType(DONATE);
	dlg.SetGroupCollection(&SourceCollection);
	if(IDOK != dlg.DoModal()) return 0;

	if(dlg.GetOccurError()) return -1;
	
	ids = DeleteDataByGroup(pDM_DELETE, &SourceCollection, DONATE);
	if(ids < 0) return -1;
	if(ids == 1000) return ids;

	
	i = pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		if(SourceCollection.IsIncludeGroupInfo(pDM->GetCellData(_T("수입년도"), i), pDM->GetCellData(_T("접수번호"), i)))
		{
			strTmpData = pDM->GetCellData(_T("접수번호KEY"), i);
			pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
			idxArray.Add(i);
		}
		i = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	if(delType == 0) DeleteSelectRow(pDM, &idxArray);

	return 0;
}

INT CGroupMgr::CheckDupGroupNo(CESL_DataMgr * pDM, 
							   CString strTblName, 
							   CString strFieldName, 
							   CString strGroupNo, 
							   CString strYearFieldName, 
							   CString strGroupYear, 
							   CString strWorkCode/*=_T("")*/)
{
	CString strQuery, strResult, strTmpData;
	if(strWorkCode.IsEmpty()) 
		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM %s WHERE %s=%s AND %s='%s'"), strTblName, strFieldName, strGroupNo, strYearFieldName, strGroupYear);
	else if(strWorkCode == _T("NULL"))
		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM %s WHERE %s=%s AND %s='%s' AND WORK_CODE IS NULL"), strTblName, strFieldName, strGroupNo, strYearFieldName, strGroupYear);
	else if(strWorkCode.Find(_T("수입구분:")) >= 0)
	{
		strTmpData = strWorkCode;
		strTmpData.Replace(_T("수입구분:"), _T(""));
		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM %s WHERE %s=%s AND %s='%s' AND ACQ_CODE='%s'"), strTblName, strFieldName, strGroupNo, strYearFieldName, strGroupYear, strTmpData);
	}
	else
		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM %s WHERE %s=%s AND %s='%s' AND WORK_CODE='%s'"), strTblName, strFieldName, strGroupNo, strYearFieldName, strGroupYear, strWorkCode);
	
	strTblName.TrimLeft();
	strTblName.TrimRight();
	if(_T("BO_PURCHASE_SEQ_NO_TBL") == strTblName ||
	   _T("BO_ACQ_SEQ_NO_TBL")	== strTblName ||
	   _T("BO_ACQ_GROUP_TBL")	== strTblName ||
	   _T("BO_DON_RECEIPT_TBL")	== strTblName 
	   )	   
	{
		strQuery = strQuery + _T(" AND MANAGE_CODE = UDF_MANAGE_CODE");
	}

	INT ids = pDM->GetOneValueQuery(strQuery, strResult);
	if(ids < 0) return -1;

	if(!strResult.IsEmpty()) 
	{
		if(strWorkCode.IsEmpty())
			strQuery.Format(_T("SELECT MAX(%s) FROM %s WHERE %s = '%s'"), strFieldName, strTblName, strYearFieldName, strGroupYear);
		else if(strWorkCode == _T("NULL"))
			strQuery.Format(_T("SELECT MAX(%s) FROM %s WHERE %s = '%s' AND WORK_CODE IS NULL"), strFieldName, strTblName, strYearFieldName, strGroupYear);
		else if(strWorkCode.Find(_T("수입구분:")) >= 0)
		{
			strTmpData = strWorkCode;
			strTmpData.Replace(_T("수입구분:"), _T(""));
			strQuery.Format(_T("SELECT MAX(REC_KEY) FROM %s WHERE %s=%s AND %s='%s' AND ACQ_CODE='%s'"), strTblName, strFieldName, strGroupNo, strYearFieldName, strGroupYear, strTmpData);
		}
		else
			strQuery.Format(_T("SELECT MAX(%s) FROM %s WHERE %s = '%s' AND WORK_CODE='%s'"), strFieldName, strTblName, strYearFieldName, strGroupYear, strWorkCode);

		strTblName.TrimLeft();
	strTblName.TrimRight();

		INT ids = pDM->GetOneValueQuery(strQuery, strResult);
		if(ids < 0) return -1;
		return _ttoi(strResult.GetBuffer(0));
	}
	return 0;
}

INT CGroupMgr::GetReceiptNoLastSerialNum(CString strAcqYear, CString strGroupInfo, CString strLastSerialNum)
{
	CESL_DataMgr tmpDM;
	if(strAcqYear.IsEmpty() || strGroupInfo.IsEmpty() || strGroupInfo == _T("적용안함")) 
	{
		return -1;
	}
	
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT LAST_SERIAL_NO ")
					_T("FROM BO_DON_RECEIPT_TBL ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"), strAcqYear, strGroupInfo);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	strLastSerialNum = tmpDM.GetCellData(0,0);
	return 0;
}

INT CGroupMgr::ChangeDivNoStatus()
{
	CString strAcqYear, strWorkCode, strDivNo, strQuery, strWorkStatus;
	GetTempData(_T("수입년도"), strAcqYear);
	GetTempData(_T("업무구분"), strWorkCode);
	GetTempData(_T("선정차수"), strDivNo);
	GetTempData(_T("변경상태"), strWorkStatus);
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	
	if(!strWorkCode.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL B, IDX_BO_TBL I WHERE I.REC_KEY=B.SPECIES_KEY ")
			_T("AND I.WORK_CODE='%s' AND B.ACQ_YEAR='%s' AND B.SEQ_NO IN (%s) AND B.ACQ_CODE='1' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
			strWorkCode, strAcqYear, strDivNo);
	}
	else
	{
		strQuery.Format(
			_T("SELECT COUNT(*) FROM BO_BOOK_TBL B, IDX_BO_TBL I WHERE I.REC_KEY=B.SPECIES_KEY ")
			_T("AND I.WORK_CODE IS NULL AND B.ACQ_YEAR='%s' AND B.SEQ_NO IN (%s) AND B.ACQ_CODE='1' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
			strWorkCode, strAcqYear, strDivNo);
	}
	CString strValue;
	pDM->GetOneValueQuery(strQuery, strValue);
	if(_ttoi(strValue.GetBuffer(0)) > 0) return 1000;

	pDM->StartFrame();

	CArray<INT, INT> * pArrSelIdx = (CArray<INT, INT>*)GetTempPtr(_T("SELECT_INDEX"));
	if(pDM == NULL) return -1;
	if(strAcqYear.IsEmpty() || strDivNo.IsEmpty() || strWorkStatus.IsEmpty()) return -1;

	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='%s', LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO IN (%s) AND WORK_CODE IS NULL;"),
			strWorkStatus, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strDivNo
			);
	else
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET WORKING_STATUS='%s', LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO IN (%s) AND WORK_CODE='%s';"),
			strWorkStatus, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strAcqYear, strDivNo, strWorkCode
			);
	pDM->StartFrame();
	pDM->AddFrame(strQuery);
	INT ids = pDM->SendFrame();
	if(ids < 0) return ids;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;

	if(pArrSelIdx != NULL)
		DeleteSelectRow(pDM, pArrSelIdx);
	
	return 0;
}

INT CGroupMgr::SetGroupInfoBookCnt()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CGroupCollection * pGroupCollection = (CGroupCollection*)GetTempPtr(_T("GroupCollection"));
	CString strWhere, strQuery, strTmpData, strAcqYear;
	GetTempData(_T("조건"), strWhere);
	INT nSpeciesType;
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	
	if(pDM == NULL || pGroupCollection == NULL) return -1;
	
	INT ids = GroupQueryChange(pGroupCollection, strWhere);
	if(ids < 0) return ids;
	if(strWhere.IsEmpty()) return 0;

	if(strAcqYear.IsEmpty() || ((SPECIES_TYPE)nSpeciesType == BO_DATA_IMPORT))

		strTmpData.Format(_T(" AND B.SEQ_NO = G.GROUP_SEQ"));
	else
		strTmpData.Format(_T(" AND B.SEQ_NO = G.GROUP_SEQ AND B.ACQ_YEAR = '%s'"), strAcqYear);
	strWhere += strTmpData;

	if(0 > strWhere.Find(_T("MANAGE_CODE = UDF_MANAGE_CODE"))) strWhere = strWhere + _T(" AND B.MANAGE_CODE = UDF_MANAGE_CODE");

	if((SPECIES_TYPE)nSpeciesType == SPECIES_TYPE_INVALID)
	{
		strWhere += _T(" AND (B.ACQ_CODE = '0' OR B.ACQ_CODE IS NULL)");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_ACQ_GROUP_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_BASIC)
	{
		strWhere += _T(" AND B.WORKING_STATUS = 'BOT111O' AND (B.ACQ_CODE = '0' OR B.ACQ_CODE IS NULL)");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_ACQ_GROUP_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_IMPORT)
	{	
		strWhere += _T(" AND B.WORKING_STATUS = 'BOT112O' AND (B.ACQ_CODE = '0' OR B.ACQ_CODE IS NULL)");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_ACQ_GROUP_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_RECYCLE)
	{
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_DELETE_BOOK_TBL B, BO_ACQ_GROUP_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_DATA_IMPORT)
	{
		strWhere += _T(" AND B.WORKING_STATUS > 'BOT' AND (B.ACQ_CODE = '0' OR B.ACQ_CODE IS NULL)");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_ACQ_GROUP_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT nDMCnt = pDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nDMCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("그룹KEY"), i);
		for(INT j=0 ; j<nRowCnt ; j++)
		{
			if(strTmpData == tmpDM.GetCellData(j,0))
			{
				strTmpData = tmpDM.GetCellData(j, 1);
				if(strTmpData.IsEmpty()) strTmpData = _T("0");
				pDM->SetCellData(_T("그룹책수"), strTmpData, i);
				break;
			}
		}
		if(j == nRowCnt) pDM->SetCellData(_T("그룹책수"), _T("0"), i);
	}
	return 0;
}

INT CGroupMgr::GroupQueryChange(CGroupCollection *pGroupCollection, CString &strWhere)
{
	if(pGroupCollection == NULL) return -1;
	if(strWhere.IsEmpty()) return 0;
	CString strTmpData, strFieldName, strValue;
	CStringArray CondElement;
	CStringArray ArrCond;
	CStringArray ArrQeury;
	INT ids = pGroupCollection->ParsingStringApplyDuplicate(_T(" AND "), strWhere, &CondElement);
	if(ids < 0) return -1;
	strWhere.Empty();
	INT nCnt = CondElement.GetSize();
	INT nCondCnt;
	INT nPos;

	for(INT i=0 ; i<nCnt ; i++)
	{
		ArrCond.RemoveAll();
		strTmpData = CondElement.GetAt(i);
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		ids = pGroupCollection->ParsingStringApplyDuplicate(_T("="), strTmpData, &ArrCond);
		if(ids < 0) return -1;
		nCondCnt = ArrCond.GetSize();
		if(nCondCnt == 2)
		{
			strFieldName = ArrCond.GetAt(0);
			strValue = ArrCond.GetAt(1);
			strFieldName.TrimLeft();	strFieldName.TrimRight();
			strValue.TrimLeft();		strValue.TrimRight();
	
			nPos = strFieldName.Find(_T("."));
			if(nPos >= 0)
			{
				strFieldName = strFieldName.Mid(nPos+1);
			}
			
			strTmpData = strValue;
			strTmpData.MakeUpper();
			nPos = strTmpData.Find(_T("B."));
			if(nPos >= 0) continue;
			
			strTmpData = _T("G.") + strFieldName + _T(" = ") + strValue;
			ArrQeury.Add(strTmpData);
		}
	}
	
	nCnt = ArrQeury.GetSize();
	for(i=0 ; i<nCnt ; i++)
	{
		if(strWhere.IsEmpty()) strWhere = ArrQeury.GetAt(i);
		else strWhere += _T(" AND ") + ArrQeury.GetAt(i);
	}
	return 0;
}

INT CGroupMgr::SetReceiptNoBookCnt()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CGroupCollection * pGroupCollection = (CGroupCollection*)GetTempPtr(_T("GroupCollection"));
	CString strWhere, strQuery, strTmpData, strAcqYear;
	GetTempData(_T("조건"), strWhere);
	INT nSpeciesType;
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	
	if(pDM == NULL || pGroupCollection == NULL) return -1;
	
	INT ids = GroupQueryChange(pGroupCollection, strWhere);
	if(ids < 0) return ids;
	if(strWhere.IsEmpty()) return 0;
	
	if(strAcqYear.IsEmpty())
		strTmpData.Format(_T(" AND B.ACQ_CODE = '2' AND  B.SEQ_NO = G.RECEIPT_NO"));
	else	
		strTmpData.Format(_T(" AND B.ACQ_CODE = '2' AND  B.SEQ_NO = G.RECEIPT_NO AND B.ACQ_YEAR = '%s'"), strAcqYear);
	strWhere += strTmpData;
	
	if(0 > strWhere.Find(_T("MANAGE_CODE = UDF_MANAGE_CODE"))) strWhere = strWhere + _T(" AND B.MANAGE_CODE = UDF_MANAGE_CODE");

	if((SPECIES_TYPE)nSpeciesType == SPECIES_TYPE_INVALID)
	{
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_DON_RECEIPT_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_DONATE)
	{
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA211O'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_DON_RECEIPT_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_NONDONATE)
	{	
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA212O'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_DON_RECEIPT_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_REGOBJ)
	{
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA211O'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_DON_RECEIPT_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_REG_DONATE)
	{
		strWhere += _T(" AND B.WORKING_STATUS > 'BOR' AND B.WORKING_STATUS < 'BOT'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_DON_RECEIPT_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT nDMCnt = pDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nDMCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("접수번호KEY"), i);
		for(INT j=0 ; j<nRowCnt ; j++)
		{
			if(strTmpData == tmpDM.GetCellData(j, 0))
			{
				strTmpData = tmpDM.GetCellData(j, 1);
				if(strTmpData.IsEmpty()) strTmpData = _T("0");
				pDM->SetCellData(_T("책수"), strTmpData, i);
				break;
			}
		}
		if(j == nRowCnt) pDM->SetCellData(_T("책수"), _T("0"), i);
	}
	return 0;
}

INT CGroupMgr::SetDivisionNoBookCnt()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CGroupCollection * pGroupCollection = (CGroupCollection*)GetTempPtr(_T("GroupCollection"));
	CString strWhere, strWorkCode, strQuery, strTmpData, strAcqYear;
	GetTempData(_T("조건"), strWhere);
	GetTempData(_T("WORK_CODE"), strWorkCode);
	INT nSpeciesType;
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	
	if(pDM == NULL || pGroupCollection == NULL) return -1;
	
	INT ids = GroupQueryChange(pGroupCollection, strWhere);
	if(ids < 0) return ids;
	if(strWhere.IsEmpty()) return 0;
	
	if(strWorkCode.IsEmpty())
	{
		strTmpData.Format(
			_T(" AND B.ACQ_CODE = '1' AND I.REC_KEY = B.SPECIES_KEY")
			_T(" AND B.SEQ_NO = G.SEQ_NO AND I.WORK_CODE IS NULL"));
		strWhere += strTmpData;
	}
	else
	{
		strTmpData.Format(
			_T(" AND B.ACQ_CODE = '1' AND I.REC_KEY = B.SPECIES_KEY")
			_T(" AND B.SEQ_NO = G.SEQ_NO AND I.WORK_CODE = '%s'"), strWorkCode);
		strWhere += strTmpData;
	}
	if(!strAcqYear.IsEmpty())
	{
		strTmpData.Format(_T(" AND B.ACQ_YEAR='%s'"), strAcqYear);
		strWhere += strTmpData;
	}

	if(0 > strWhere.Find(_T("MANAGE_CODE = UDF_MANAGE_CODE"))) strWhere = strWhere + _T(" AND I.MANAGE_CODE = UDF_MANAGE_CODE");

	if((SPECIES_TYPE)nSpeciesType == SPECIES_TYPE_INVALID)
	{
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_PURCHASE)
	{
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA111N'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_ORDER)
	{	
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA112N'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_MISSING)
	{
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA113O'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_REGOBJ)
	{
		strWhere += _T(" AND B.WORKING_STATUS = 'BOA114O'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	else if((SPECIES_TYPE)nSpeciesType == BO_REG_PURCHASE)
	{
		strWhere += _T(" AND B.WORKING_STATUS > 'BOR' AND B.WORKING_STATUS < 'BOT'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT nDMCnt = pDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nDMCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("구입차수KEY"), i);
		for(INT j=0 ; j<nRowCnt ; j++)
		{
			if(strTmpData == tmpDM.GetCellData(j, 0))
			{
				strTmpData = tmpDM.GetCellData(j, 1);
				if(strTmpData.IsEmpty()) strTmpData = _T("0");
				pDM->SetCellData(_T("책수"), strTmpData, i);
				break;
			}
		}
		if(j == nRowCnt) pDM->SetCellData(_T("책수"), _T("0"), i);
	}
	return 0;
}

INT CGroupMgr::SetOtherGroupNoBookCnt()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CGroupCollection * pGroupCollection = (CGroupCollection*)GetTempPtr(_T("GroupCollection"));
	CString strWhere, strQuery, strTmpData, strAcqYear, strAcqCode;
	GetTempData(_T("조건"), strWhere);
	INT nSpeciesType;
	GetTempData(_T("SPECIES_TYPE"), nSpeciesType);
	GetTempData(_T("ACQ_YEAR"), strAcqYear);
	GetTempData(_T("ACQ_CODE"), strAcqCode);
	
	if(pDM == NULL || pGroupCollection == NULL || strAcqCode.IsEmpty()) return -1;
	
	INT ids = GroupQueryChange(pGroupCollection, strWhere);
	if(ids < 0) return ids;
	if(strWhere.IsEmpty()) return 0;
	
	if(strAcqYear.IsEmpty())
		strTmpData.Format(_T(" AND B.ACQ_CODE = '%s' AND  B.SEQ_NO = G.GROUP_SEQ"), strAcqCode);
	else
		strTmpData.Format(_T(" AND B.ACQ_CODE = '%s' AND  B.SEQ_NO = G.GROUP_SEQ AND B.ACQ_YEAR = '%s'"), strAcqCode, strAcqYear);
	strWhere += strTmpData;
	
	if(0 > strWhere.Find(_T("MANAGE_CODE = UDF_MANAGE_CODE"))) strWhere = strWhere + _T(" AND G.MANAGE_CODE = UDF_MANAGE_CODE");

	if((SPECIES_TYPE)nSpeciesType == BO_REG_INVALID)
	{
		strWhere += _T(" AND B.WORKING_STATUS > 'BOR' AND B.WORKING_STATUS < 'BOT'");
		strQuery.Format(
			_T("SELECT G.REC_KEY, COUNT(B.REC_KEY) FROM BO_BOOK_TBL B, BO_ACQ_SEQ_NO_TBL G ")
			_T("WHERE %s GROUP BY G.REC_KEY"), strWhere);		
	}
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	INT nDMCnt = pDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nDMCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("그룹번호KEY"), i);
		for(INT j=0 ; j<nRowCnt ; j++)
		{
			if(strTmpData == tmpDM.GetCellData(j, 0))
			{
				strTmpData = tmpDM.GetCellData(j, 1);
				if(strTmpData.IsEmpty()) strTmpData = _T("0");
				pDM->SetCellData(_T("책수"), strTmpData, i);
				break;
			}
		}
		if(j == nRowCnt) pDM->SetCellData(_T("책수"), _T("0"), i);
	}
	return 0;
}

INT CGroupMgr::GetOtherGroupNoLastSerialNum(CString strAcqCode, CString strAcqYear, CString strGroupInfo, CString &strLastSerialNum)
{
	CESL_DataMgr tmpDM;
	if(strAcqCode.IsEmpty() || strAcqYear.IsEmpty() || strGroupInfo.IsEmpty() || strGroupInfo == _T("적용안함")) 
	{
		return -1;
	}
	
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO ")
					_T("FROM BO_ACQ_SEQ_NO_TBL ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='%s' AND CREATE_YEAR = '%s' AND GROUP_SEQ = %s"),
		strAcqCode, strAcqYear, strGroupInfo);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	strLastSerialNum = tmpDM.GetCellData(0,0);
	return 0;
}

INT CGroupMgr::InsertOtherGroupNo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	BOOL bDropNoApply = FALSE;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("DROP_NO_APPLY"), bDropNoApply);
	
	if(!pCM || !pDM) return -1000;

	CString strGroupNo, strGroupYear, strAcqCode;
	pCM->GetControlMgrData(_T("그룹번호"), strGroupNo);
	pCM->GetControlMgrData(_T("수입년도"), strGroupYear);
	pCM->GetControlMgrData(_T("수입구분"), strAcqCode);
	strAcqCode = _T("수입구분:") + strAcqCode;
	INT ids = CheckDupGroupNo(pDM, _T("BO_ACQ_SEQ_NO_TBL"), _T("GROUP_SEQ"), strGroupNo, _T("CREATE_YEAR"), strGroupYear, strAcqCode);
	if(ids != 0) return ids;
	
	pDM->InsertRow(-1);
	INT nCurRow = pDM->GetRowCount() - 1;
	SetDMRowbyCMData(pCM, pDM, nCurRow);
	CString strToday, strReckey;
	strToday = GetTodayDate();
	pDM->MakeRecKey(strReckey);
	pDM->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	pDM->SetCellData(_T("그룹번호KEY"), strReckey, nCurRow);
	
	pDM->StartFrame();
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("책수"));
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("BO_ACQ_SEQ_NO_TBL"), nCurRow, pDM);
	
	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;
	
	if(!bDropNoApply)
		SetLastNumber(_T("GRP_NO"), _T("MO"), pDM->GetCellData(_T("수입년도"), nCurRow), pDM->GetCellData(_T("수입구분"), nCurRow), pDM->GetCellData(_T("그룹번호"), nCurRow));
	
	return 0;
}

INT CGroupMgr::UpdateOtherGroupNo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	INT idx;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	GetTempData(_T("Index"), idx);

	if(!pCM || !pDM) return -1000;
	
	pDM->StartFrame();
	
	SetDMRowbyCMData(pCM, pDM, idx);
	pDM->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), idx);
	
	CString strReckey = pDM->GetCellData(_T("그룹번호KEY"), idx);
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("책수"));
	RemoveAliasList.Add(_T("그룹번호KEY"));
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("BO_ACQ_SEQ_NO_TBL"), idx, pDM, _T("REC_KEY"), _T("NUMERIC"), strReckey, idx);
	
	INT ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;
	
	return 0;
}

INT CGroupMgr::DeleteOtherGroupNo()
{
	CESL_ControlMgr * pCM = NULL;
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	CESL_DataMgr * pDM_DELETE = NULL;
	pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pDM_DELETE = (CESL_DataMgr*)GetTempPtr(_T("DELETE_DM"));

	if(!pCM || !pDM || !pGrid || !pDM_DELETE) return -1000;
	
	INT i = pGrid->SelectMakeList();
	i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -5000;
	CString strLastNo, strCreateYear, strReckey, strGroupNo, strTmpData, strAcqYear;
	INT delType = 0;
	CArray <INT, INT> idxArray;		idxArray.RemoveAll();
	
	CGroupInfo TargetGroup;
	CGroupCollection SourceCollection(m_pParentMgr);
	SourceCollection.SetAcqType(OTHER_ACQ_TYPE);
	SourceCollection.SetAcqCode(pDM->GetCellData(_T("수입구분"), 0));

	INT ids;
	
	pDM->StartFrame();
	
	while(i >= 0)
	{
		CGroupInfo pTmpGroup;
		strAcqYear = pDM->GetCellData(_T("수입년도"), i);
		pTmpGroup.SetAcqYear(strAcqYear);
		pTmpGroup.SetGroupInfo(pDM->GetCellData(_T("그룹번호"), i));
		pTmpGroup.SetGroupNo(pDM->GetCellData(_T("수입구분"), i));
		SourceCollection.AddGroupInfo(&pTmpGroup);
		i = pGrid->SelectGetNext();
	}
	
	CCheckHoldDataDlg dlg(m_pParentMgr);
	dlg.SetAcqType(OTHER_ACQ_TYPE);
	dlg.SetGroupCollection(&SourceCollection);
	if(IDOK != dlg.DoModal()) return 0;

	if(dlg.GetOccurError()) return -1;
	
	ids = DeleteDataByGroup(pDM_DELETE, &SourceCollection, OTHER_ACQ_TYPE);
	if(ids < 0) return -1;
	if(ids == 1000) return ids;
	
	
	i = pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		if(SourceCollection.IsIncludeGroupInfo(pDM->GetCellData(_T("수입년도"), i), pDM->GetCellData(_T("그룹번호"), i)))
		{
			strTmpData = pDM->GetCellData(_T("그룹번호KEY"), i);
			pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
			idxArray.Add(i);
		}
		i = pGrid->SelectGetNext();
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -2000;
	ids = pDM->EndFrame();
	if(ids < 0) return -2000;
	
	if(delType == 0) DeleteSelectRow(pDM, &idxArray);
	
	return 0;
}

