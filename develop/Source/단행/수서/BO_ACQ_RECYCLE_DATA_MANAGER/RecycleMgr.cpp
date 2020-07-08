// RecycleMgr.cpp: implementation of the CRecycleMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RecycleMgr.h"
#include "..\..\..\공통\BO_SPECIES_API\BO_SPECIES.h"
#include "..\..\..\공통\BO_ACQ_API\BO_ACQ_Business_Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("재활용자료관리"), _T(__FILE__), __LINE__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecycleMgr::CRecycleMgr(CESL_Mgr *pWnd):CSPF_Work(pWnd)
{
	pMakeSearchData = new CMakeSearchData(pWnd);
}

CRecycleMgr::~CRecycleMgr()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}
}

INT CRecycleMgr::Search()
{
	CString CMAlias;
	this->GetTempData(_T("CMAlias"), CMAlias);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);

	CESL_DataMgr *pDM = NULL;

	if(!pCM)
		return -1;

	CString workCode, classCodeType, manageCode, title, delDate1, delDate2;
	pCM->GetControlMgrData(_T("업무구분"), workCode);
	pCM->GetControlMgrData(_T("이용대상구분"), manageCode);
	pCM->GetControlMgrData(_T("본표제"), title);
	pCM->GetControlMgrData(_T("삭제일1"), delDate1);
	pCM->GetControlMgrData(_T("삭제일2"), delDate2);

	workCode.TrimLeft();			workCode.TrimRight();
	manageCode.TrimLeft();			manageCode.TrimRight();
	delDate1.TrimLeft();			delDate1.TrimRight();
	delDate2.TrimLeft();			delDate2.TrimRight();
	title.TrimLeft();				title.TrimRight();

	pMakeSearchData->GetKeyWord(title);	pMakeSearchData->ChangeHanjaToHangul(title);

	if(delDate1.IsEmpty() && !delDate2.IsEmpty())
	{
		m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return 1000;
	}
	CString strWhere, strOption;

	INT DataType = 0;

	strWhere = _T("(I.SPECIES_CLASS = '2' OR I.SPECIES_CLASS = '3') AND I.REC_KEY = P.SPECIES_KEY(+)");
	pDM = m_pParentMgr->FindDM(_T("DM_재활용_종"));
	strWhere += MakeDateQuery(delDate1, delDate2, strWhere.IsEmpty(), true);
	
	if(!workCode.IsEmpty() && workCode != _T("0"))
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), workCode);		
		strWhere += strOption;
	}

	if(!manageCode.IsEmpty() && manageCode != _T("0"))
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), manageCode);		
		strWhere += strOption;
	}

	if(!title.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
		strWhere += strOption;
	}

	pDM->RefreshDataManager(strWhere);
	
	return DataType;
}

CString CRecycleMgr::MakeDateQuery(CString Date1, CString Date2, BOOL isEmpty, BOOL isSpecies)
{
	CString query;

	if(isEmpty)
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(Date1.IsEmpty() && !(Date2.IsEmpty()))
		{
			/*m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
			return _T("");
			
			if(isSpecies)
				query.Format(_T("I.DELETE_DATE <= '%s'"), Date2);
			else 
				query.Format(_T("B.DELETE_DATE <= '%s'"), Date2);
			return query;
			*/
		}
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			if(isSpecies)
				query.Format(_T("I.DELETE_DATE = '%s'"), Date1);
			else 
				query.Format(_T("B.DELETE_DATE = '%s'"), Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			if(isSpecies)
			{
				if(Date1 == Date2)
					query.Format(_T("I.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T("I.DELETE_DATE >= '%s' AND I.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			else
			{
				if(Date1 == Date2)
					query.Format(_T("B.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T("B.DELETE_DATE >= '%s' AND B.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			return query;
		}
	}
	else
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(Date1.IsEmpty() && !(Date2.IsEmpty()))
		{
/*			m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
			return _T("");
			if(isSpecies)
				query.Format(_T(" AND I.DELETE_DATE <= '%s'"), Date2);
			else 
				query.Format(_T(" AND B.DELETE_DATE <= '%s'"), Date2);
			return query;
*/		}
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			if(isSpecies)
				query.Format(_T(" AND I.DELETE_DATE = '%s'"), Date1);
			else
				query.Format(_T(" AND B.DELETE_DATE = '%s'"), Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			if(isSpecies)
			{
				if(Date1 == Date2)
					query.Format(_T(" AND I.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T(" AND I.DELETE_DATE >= '%s' AND I.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			else
			{
				if(Date1 == Date2)
					query.Format(_T(" AND B.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T(" AND B.DELETE_DATE >= '%s' AND B.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			return query;
		}
	}
	return _T("");
}

INT CRecycleMgr::DetailSearch()
{
	BOOL isMyData;
	CString CMAlias;
	this->GetTempData(_T("IsMyData"), isMyData);
	this->GetTempData(_T("CMAlias"), CMAlias);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_DataMgr *pDM = NULL;

	if(!pCM)
		return -1;

	CString workCode, classCodeType, inputType, UseObjCode, FormCode, pub_year, errstr, result;
	CString title, delDate1, delDate2, author, publisher, isbn1, isbn2, workingst, dataClass ;
	pCM->GetControlMgrData(_T("업무구분"), workCode);
	pCM->GetControlMgrData(_T("분류표구분"), classCodeType);
	pCM->GetControlMgrData(_T("본표제"), title);
	pCM->GetControlMgrData(_T("날짜1"), delDate1);
	pCM->GetControlMgrData(_T("날짜2"), delDate2);
	delDate1.TrimLeft();		delDate1.TrimRight();
	delDate2.TrimLeft();		delDate2.TrimRight();
	if(delDate1.IsEmpty() && !delDate2.IsEmpty())
	{
		m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return 1000;
	}
	pCM->GetControlMgrData(_T("ISBN1"), isbn1);
	pCM->GetControlMgrData(_T("발행년"),pub_year);
	pub_year.TrimLeft();		pub_year.TrimRight();
	INT ids = pMakeSearchData->IsValidYearData(pub_year);
	if( ids < 0)
	{
		errstr.Format(_T("입력하신 입수년도 값(%s)이 올바르지 않습니다."), pub_year);
		m_pParentMgr->ESLAfxMessageBox(errstr.GetBuffer(0));
		CTime t = CTime::GetCurrentTime();
		result.Format(_T("%04d"), t.GetYear());
		pCM->SetControlMgrData(_T("수입년도"), result);		
		return 1000;
	}
	pCM->GetControlMgrData(_T("저작자"), author);
	pCM->GetControlMgrData(_T("발행자"), publisher);
	pCM->GetControlMgrData(_T("최종작업상태"), workingst);
	pCM->GetControlMgrData(_T("입력구분"), inputType);
	pCM->GetControlMgrData(_T("이용대상구분"), UseObjCode);
	pCM->GetControlMgrData(_T("형식구분"), FormCode);

	workCode.TrimLeft();		workCode.TrimRight();
	classCodeType.TrimLeft();	classCodeType.TrimRight();
	isbn1.TrimLeft();			isbn1.TrimRight();
	workingst.TrimLeft();		workingst.TrimRight();
	inputType.TrimLeft();		inputType.TrimRight();
	UseObjCode.TrimLeft();		UseObjCode.TrimRight();
	FormCode.TrimLeft();		FormCode.TrimRight();
	title.TrimLeft();			title.TrimRight();
	author.TrimLeft();			author.TrimRight();
	publisher.TrimLeft();		publisher.TrimRight();

	//본표제, 저작자, 발행자 색인어 생성형식으로 변환
	pMakeSearchData->GetKeyWord(title);		pMakeSearchData->ChangeHanjaToHangul(title);
	pMakeSearchData->GetKeyWord(author);	pMakeSearchData->ChangeHanjaToHangul(author);
	pMakeSearchData->GetKeyWord(publisher);	pMakeSearchData->ChangeHanjaToHangul(publisher);

	INT dataType = 0;
	CString strWhere, strOption;
	ConvertWorkingStatusDescToCode(workingst);

	strWhere = _T("(I.SPECIES_CLASS = '2' OR I.SPECIES_CLASS = '3') AND I.REC_KEY = P.SPECIES_KEY(+) AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	dataType = 1;
	pDM = m_pParentMgr->FindDM(_T("DM_재활용_종"));
	strWhere += MakeDateQuery(delDate1, delDate2, strWhere.IsEmpty(), true);
	if(!workingst.IsEmpty())
	{
		strOption.Format(_T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_DELETE_BOOK_TBL WHERE WORKING_STATUS = '%s' AND BOOK_APPENDIX_FLAG='B')"), workingst);
		strWhere += strOption;
		strOption.Format(_T(" AND P.REC_KEY IN (SELECT ACCU_KEY FROM BO_DELETE_BOOK_TBL WHERE WORKING_STATUS = '%s' AND BOOK_APPENDIX_FLAG='B')"), workingst);
		strWhere += strOption;
	}
	
	if(!classCodeType.IsEmpty() && classCodeType != _T("적용안함"))
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), classCodeType);
		strWhere += strOption;
	}
		
	if(!title.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
		strWhere += strOption;
	}

	if(!author.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), author);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), author);
		strWhere += strOption;
	}

	if(!publisher.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), publisher);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), publisher);
		strWhere += strOption;
	}

	if(!workCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), workCode);
		strWhere += strOption;
	}
	if(isMyData)
	{
		strOption.Format(_T(" AND I.WORKER = '%s'"), m_pParentMgr->m_pInfo->USER_ID);
		strWhere += strOption;
	}
	if(!inputType.IsEmpty())
	{
		strOption.Format(_T(" AND P.INPUT_TYPE = '%s'"), inputType);
		strWhere += strOption;
	}

	if(!UseObjCode.IsEmpty() && UseObjCode != _T("0"))
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), UseObjCode);
		strWhere += strOption;
	}
	if(!FormCode.IsEmpty() && FormCode != _T("0"))
	{
		strOption.Format(_T(" AND I.FORM_CODE = '%s'"), FormCode);
		strWhere += strOption;
	}
	if(!pub_year.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_IPUB_YEAR, '%s*', NULL)>0"), pub_year);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_IPUB_YEAR, '%s*', NULL)>0"), pub_year);
		strWhere += strOption;
	}
	if(!isbn1.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.ST_CODE, '%s*', NULL)>0"), isbn1);
		//strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s*', NULL)>0"), isbn1);
		strWhere += strOption;
	}
	pDM->RefreshDataManager(strWhere);
	return dataType;
}

VOID CRecycleMgr::ReplaceWorkingStatusCodeToDesc(CESL_DataMgr * pDM)
{
	INT cnt = pDM->GetRowCount();
	CString tmpstr;
	for(INT i=0 ; i<cnt ; i++)
	{
		pDM->GetCellData(_T("IBS_자료상태"), i, tmpstr);
		tmpstr = GetSpeciesWorkingStatusDesc(tmpstr);
		pDM->SetCellData(_T("UDF_자료상태설명"), tmpstr, i);
	}
}

CString CRecycleMgr::GetSpeciesWorkingStatusDesc(CString workingstatus)
{
	if(workingstatus.IsEmpty()) return _T("수서자료");
	else
	{
		CString tmpstr = workingstatus.Left(3);
		tmpstr.MakeUpper();
		if(tmpstr == _T("BOA")) return _T("수서자료");
		else if( tmpstr == _T("BOC")) return _T("정리자료");
		else if( tmpstr == _T("BOL")) return _T("소장자료");
	}
	return _T("");
}

CString CRecycleMgr::MakeISBNQuery(CString isbn1, CString isbn2)
{
	CString query;

	if(isbn1.IsEmpty() && isbn2.IsEmpty())
		return _T("");
	else if(isbn1.IsEmpty() && !(isbn2.IsEmpty()))
	{
		query.Format(_T(" AND &ECOSEARCH(I.ST_CODE, '%s', NULL)>0"), isbn1);
		//query.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s', NULL)>0"), isbn1);
		return query;
	}
	else if(!(isbn1.IsEmpty()) && isbn2.IsEmpty())
	{
		query.Format(_T(" AND &ECOSEARCH(I.ST_CODE, '%s', NULL)>0"), isbn1);
		//query.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s', NULL)>0"), isbn1);
		return query;
	}
	else if(!(isbn1.IsEmpty()) && !(isbn2.IsEmpty()))
	{
		if(isbn1 == isbn2)
			query.Format(_T(" AND &ECOSEARCH(I.ST_CODE, '")_T("%s")_T("', NULL)>0"), isbn1);
			//query.Format(_T(" AND CATSEARCH(I.ST_CODE, '")_T("%s")_T("', NULL)>0"), isbn1);
		else
			query.Format(_T(" AND &ECOSEARCH(I.ST_CODE, '%s', NULL)>0 AND &ECOSEARCH(I.ST_CODE, '%s', NULL)>0"), isbn1, isbn2);
			//query.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s', NULL)>0 AND CATSEARCH(I.ST_CODE, '%s', NULL)>0"), isbn1, isbn2);
		return query;
	}
	return _T("");	
}

//return 값 검사
// -1000 : 삭제관련 자료의 제거를 위한 DM을 찾을 수 없습니다. 
// -1001 : 선택리스트를 구성할 수 없습니다.
//  1002 : 삭제작업을 취소하였습니다.
// -1003 : 삭제 쿼리를 수행할 수 없습니다.
// -1004 : DM에서 삭제 로우를 제거할 수 없습니다.
INT CRecycleMgr::Delete()
{
	CString CMAlias, DMAlias, BookDMAlias, GridAlias;
	CDataProcessingDlg * pPrgCtrl = NULL;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("BookDMAlias"), BookDMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	pPrgCtrl = (CDataProcessingDlg*)GetTempPtr(_T("ProgressCtrl"));
	
	CESL_DataMgr * pDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	pDM = m_pParentMgr->FindDM(DMAlias);
	pBookDM = m_pParentMgr->FindDM(BookDMAlias);
	
	if(!pDM) return -1000;
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids<0) return -1001;
	INT nSelectCount = m_pParentMgr->SelectGetCount(CMAlias, GridAlias);
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i<0) return -1001;

	if(IDCANCEL == m_pParentMgr->ESLAfxMessageBox(_T("선정한 자료를 삭제하시겠습니까?"), MB_OKCANCEL))
		return 1002;

	if(pPrgCtrl)
	{
		pPrgCtrl->SetMessage(_T("선정된 재활용 자료를 삭제중 입니다."));
		pPrgCtrl->GetProgressCtrl()->SetPos(0);
		pPrgCtrl->GetProgressCtrl()->SetRange(0, nSelectCount);
	}

	CString key, tmpstr, strQuery;
	CStringList idxList;
	CStringArray ArrSpeciesKey;
	
	INT nCnt = 0;
	INT count = 1;
	while(i >= 0)
	{
		if(pPrgCtrl)
		{
			pPrgCtrl->GetProgressCtrl()->SetPos(++nCnt);
		}
		pDM->GetCellData(_T("IBS_종KEY"), i, key);
		if(key.IsEmpty())
		{
			i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
			continue;
		}
		pDM->StartFrame();
		pDM->MakeDeleteFrame(_T("BO_DELETE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), key);
		pDM->MakeDeleteFrame(_T("BO_DELETE_BOOK_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), key);
		pDM->MakeDeleteFrame(_T("BO_PURCHASEINFO_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), key);
		pDM->MakeDeleteFrame(_T("BO_DONINFO_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), key);
		pDM->MakeDeleteFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), key);

		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), key);
			pDM->AddFrame(strQuery);
		}
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (rec_key, species_key, idx_work_flag, input_date) ")
//  			_T(" values (ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"), key);
// 		pDM->AddFrame(strQuery);
		//=====================================================
 		
		pDM->MakeDeleteFrame(_T("BO_EBOOK_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), key);
		BO_TOC_ABSTRACT_DELETE(pDM, key);
		
		tmpstr.Format(_T("%d"), i);
		idxList.AddTail(tmpstr);
		ArrSpeciesKey.Add(key);
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
		count++;

		ids = pDM->SendFrame();
		if(ids<0) return -1003;
		ids = pDM->EndFrame();
		if(ids<0) return -1003;	
	}
	
	ids = SelectRowDelete(pDM, idxList);
	if(ids < 0) return -1004;
	
	CBO_ACQ_Business_Api api(m_pParentMgr);
	api.InitTempData();	api.InitTempPtr();
	api.SetTempPtr(_T("종DM"), (DWORD*)NULL);
	api.SetTempPtr(_T("권DM"), (DWORD*)NULL);
	api.SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	api.SetTempPtr(_T("삭제데이터"), (DWORD*)&ArrSpeciesKey);
	api.SetTempData(_T("비교Alias"), _T("IBS_종KEY"));
	ids = api.DeleteDMData();
	if(ids < 0) return -4000;

	return 0;
}


INT CRecycleMgr::BO_TOC_ABSTRACT_DELETE(CESL_DataMgr *pDM_OUT, CString strSpeciesKey)
{
	CString strQuery, strValue;
	strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"), strSpeciesKey);
	pDM_OUT->GetOneValueQuery(strQuery, strValue);
	if(!strValue.IsEmpty())
	{
		strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s;"), strSpeciesKey);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
	}

	return 0;
}

// return 값 검사
// -1000 : 삭제를 위한 DM을 찾을 수 없습니다.
// -1001 : 삭제를 위한 DM을 재구성할 수 없습니다.
// -1002 : 삭제 쿼리를 수행할 수 없습니다.
INT CRecycleMgr::AllDelete()
{
	CDataProcessingDlg * pPrgCtrl = NULL;
	CString DMAlias, strQuery;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("SOURCE_DM"));
	pPrgCtrl = (CDataProcessingDlg*)GetTempPtr(_T("ProgressCtrl"));
	if(!pDM) return -1000;

	INT rowCnt = pDM->GetRowCount();
	if(rowCnt < 1) return 0;

	CString rec_key = _T("");

	if(IDCANCEL == m_pParentMgr->ESLAfxMessageBox(_T("검색된 모든 자료를 삭제하시겠습니까?"), MB_OKCANCEL))
		return 1003;

	if(pPrgCtrl)
	{
		pPrgCtrl->SetMessage(_T("검색되어진 재활용 자료를 삭제중 입니다."));
		pPrgCtrl->GetProgressCtrl()->SetPos(0);
		pPrgCtrl->GetProgressCtrl()->SetRange(0, rowCnt);
	}
	
	INT ids;
	
	for(INT i=0 ; i<rowCnt ; i++)
	{
		if(pPrgCtrl)
		{
			pPrgCtrl->GetProgressCtrl()->SetPos(i+1);
		}
		pDM->GetCellData(_T("IBS_종KEY"), i, rec_key);
		if(rec_key.IsEmpty()) continue;

		pDM->StartFrame();
		pDM->MakeDeleteFrame(_T("BO_DELETE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), rec_key);
		pDM->MakeDeleteFrame(_T("BO_DELETE_BOOK_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), rec_key);
		pDM->MakeDeleteFrame(_T("BO_PURCHASEINFO_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), rec_key);
		pDM->MakeDeleteFrame(_T("BO_DONINFO_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), rec_key);
		pDM->MakeDeleteFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), rec_key);
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), rec_key);
			pDM->AddFrame(strQuery);
		}
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (rec_key, species_key, idx_work_flag, input_date) ")
//  			_T(" values (ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"), rec_key);
// 		pDM->AddFrame(strQuery);
		//=====================================================


		pDM->MakeDeleteFrame(_T("BO_EBOOK_TBL"), _T("SPECIES_KEY"), _T("NUMERIC"), rec_key);
		BO_TOC_ABSTRACT_DELETE(pDM, rec_key);
	
		ids = pDM->SendFrame();
		if(ids<0) return -1002;
		ids = pDM->EndFrame();
		if(ids<0) return -1002;
	}

	pDM->FreeData();
	return 0;
}

// return 값 검사
// -1000 : 구입처리를 위한 DM을 찾을 수 없습니다.
// -1001 : 선택리스트를 구성할 수 없습니다.
// -1002 : 선정자료를 구입처리할 수 없습니다.
// -1003 : 구입처리되어진 자료를 제거할 수 없습니다.
// -1004 : 구입처리된 자료의 DM로우를 제거할 수 없습니다.
INT CRecycleMgr::Purchase()
{
	CString CMAlias, DMAlias, GridAlias;
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids<0) return -1001;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i<0) return -1001;
	
	CString SpeciesKey, tmpstr;
	CStringList idxList, skeyList;

	pDM->StartFrame();
	
	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-6000);
	api.InitDataMgr();

	INT count = 1;
	while(i >= 0)
	{
		pDM->GetCellData(_T("IBS_종KEY"), i, SpeciesKey);
		skeyList.AddTail(SpeciesKey);
		//공통측의 재활용의 구입처리 메소드를 호출한다.
		INT ids= api.BO_SPECIES_RESTORE(_T("PURCHASE"), SpeciesKey);
		if(ids < 0) return -1002;
		tmpstr.Format(_T("%d"), i);
		idxList.AddTail(tmpstr);
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
		count++;
	}

	CString inquery = MakeReckeyInQuery(skeyList);
	CString tmpquery;
	pDM->StartFrame();
	tmpquery.Format(
		_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='2' WHERE SPEICES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_STATUS < 'BOC' AND MANAGE_CODE = UDF_MANAGE_CODE AND REC_KEY IN %s);"),
		inquery
	);
	pDM->AddFrame(tmpquery);
	ids = pDM->SendFrame();
	if(ids < 0) return ids;
	ids = pDM->EndFrame();
	if(ids < 0) return ids;
	//JOB.2019.0073 : 충돌 License 제거
	//ShowRequestMailDlg(skeyList, TRUE);

	ids = this->SelectRowDelete(pDM, idxList);
	if(ids<0) return -1004;
	
	return 0;
}

// -1000 : 복원처리를 위한 DM을 찾을 수 없습니다.
// -1001 : 선택리스트를 구성할 수 없습니다.
// -1002 : 선정자료를 복원처리할 수 없습니다.
// -1003 : 
// -1004 : 복원처리된 자료를 제거할 수 없습니다.
// -1005 : 복원처리된 자료의 DM로우를 제거할 수 없습니다.
//INT CRecycleMgr::Restore(INT tabCount)
INT CRecycleMgr::Restore()
{
	CESL_DataMgr * pDM;
	CString GridAlias, CMAlias, DMAlias;
//	INT tabCount;
//	this->GetTempData(_T("TabCount"), tabCount);
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	
	pDM = m_pParentMgr->FindDM(_T("DM_재활용_종"));
	if(!pDM) return -1000;

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids<0) return -1001;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i<0) return -1001;

	CStringList BookReckey, idxList, skeyList;

	CString Key, tmpstr;

	pDM->StartFrame();

	CBO_SPECIES api(m_pParentMgr);
	api.SetOpenMode(-6000);
	api.InitDataMgr();

	INT count = 1;
	while(i >= 0)
	{
//		if(tabCount == 0)
//		{
			pDM->GetCellData(_T("IBS_종KEY"), i, Key);
			skeyList.AddTail(Key);
			//공통측의 재활용의 자료복원 메소드를 호출한다.
			INT ids= api.BO_SPECIES_RESTORE(_T("RESTORE"), Key);
			if(ids < 0) return -1002;
/*		}
		else
		{
			pDM->GetCellData(_T("BB_책KEY"), i, Key);
			BookReckey.AddTail(Key);
		}
*/		tmpstr.Format(_T("%d"), i);
		idxList.AddTail(tmpstr);
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
		count++;
	}
	CString tmpquery, inquery;
/*	if(tabCount != 0)
	{
		ids = this->RestoreDelBook(BookReckey);
		if(ids < 0) return -1003;
		inquery = MakeReckeyInQuery(BookReckey);
		pDM->StartFrame();
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='2' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE ACQUISIT_CODE='1' AND WORKING_STATUS LIKE 'BOA%%' AND REC_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='3' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE ACQUISIT_CODE='2' AND WORKING_STATUS LIKE 'BOA%%' AND REC_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='4' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE WORKING_STATUS LIKE 'BOC%%' AND REC_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='5' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE WORKING_STATUS LIKE 'BOL%%' AND REC_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		ids = pDM->SendFrame();
		pDM->EndFrame();

		ShowRequestMailDlg(BookReckey, FALSE);
	}
	else
	{
*/		inquery = MakeReckeyInQuery(skeyList);
		pDM->StartFrame();
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='2' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE ACQUISIT_CODE='1' AND WORKING_STATUS LIKE 'BOA%%' AND MANAGE_CODE = UDF_MANAGE_CODE AND SPECIES_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='3' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE ACQUISIT_CODE='2' AND WORKING_STATUS LIKE 'BOA%%' AND MANAGE_CODE = UDF_MANAGE_CODE AND SPECIES_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='4' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE WORKING_STATUS LIKE 'BOC%%' AND MANAGE_CODE = UDF_MANAGE_CODE AND SPECIES_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		tmpquery.Format(
			_T("UPDATE BO_FURNISH_TBL SET STATUS='5' WHERE REC_KEY IN (SELECT FURNISH_DATA_KEY FROM BO_BOOK_TBL WHERE WORKING_STATUS LIKE 'BOL%%' AND MANAGE_CODE = UDF_MANAGE_CODE AND SPECIES_KEY IN %s);"),
			inquery
		);
		pDM->AddFrame(tmpquery);
		ids = pDM->SendFrame();
		if(ids < 0) return ids;
		ids = pDM->EndFrame();
		if(ids < 0) return ids;

		//JOB.2019.0073 : 충돌 License 제거
		//ShowRequestMailDlg(skeyList, TRUE);
//	}

	ids = this->SelectRowDelete(pDM, idxList);
	if(ids < 0) return -1005;
	
	return 0;
}

INT CRecycleMgr::RestoreDelBook(CStringList &key)
{
	if (key.IsEmpty()) return -1;

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_재활용관리_삭제책정보"));
	CString query = MakeReckeyInQuery(key);
	CString inquery;
	inquery.Format(_T("REC_KEY IN %s OR (BOOK_APPENDIX_FLAG='A' AND MOTHER_INFO_KEY IN %s) AND MANAGE_CODE = UDF_MANAGE_CODE"), query, query);
	pDM->RefreshDataManager(inquery);

	pDM->TBL_NAME = _T("BO_BOOK_TBL");

	pDM->StartFrame();

	INT ids = MakeInsertQuery(pDM);
	if(ids < 0)	return ids;
	
	ids = pDM->SendFrame();
	if(ids < 0)
		return ids;
	pDM->EndFrame();

	return 0;
}

INT CRecycleMgr::MakeInsertQuery(CESL_DataMgr *pDM)
{
	if (!pDM) return -1;
	
	INT nColumn = pDM->RefList.GetCount();
	CString field_alias, field_name, field_type, data_type, strData, keyData;
	pDM->InitDBFieldData();
	INT row, col;
	INT nRow = pDM->GetRowCount();
	for (row = 0; row < nRow; row++) {
		pDM->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), row);
		for (col = 0; col < nColumn; col++) {
			strData = _T("");
			pDM->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, row, strData);
			data_type.MakeLower();
			field_name.MakeLower();
			if(field_name == _T("rec_key")) keyData = strData;
			if (data_type == _T("udf")) continue;
			
			field_type.MakeLower();

			if (field_type == _T("str")) pDM->AddDBFieldData(field_name, _T("STRING"), strData, 1);
			if (field_type == _T("int")) pDM->AddDBFieldData(field_name, _T("NUMERIC"), strData);
		}
		pDM->MakeInsertFrame(pDM->TBL_NAME);
		pDM->MakeDeleteFrame(_T("BO_DELETE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), keyData);
	}
	return 0;
}



CString CRecycleMgr::MakeReckeyInQuery(CStringList &keys)
{
	if(keys.GetCount() == 0)
		return _T("");

	POSITION pos = keys.GetHeadPosition();
	CString query = keys.GetNext(pos);
	CString subquery;

	while(pos != NULL)
	{
		subquery.Format(_T(",%s"), keys.GetNext(pos));
		query += subquery;
	}

	CString returnquery ;
	returnquery.Format(_T("(%s)"), query);
	return returnquery;
}

INT CRecycleMgr::SelectRowDelete(CESL_DataMgr *pDM, CStringList &idxList)
{
	if(idxList.IsEmpty() || !pDM) return -1000;
	POSITION pos = idxList.GetTailPosition();
	CString tmpstr;
	INT rowidx, ids;
	while(pos != NULL)
	{
		tmpstr = idxList.GetPrev(pos);
		rowidx = _ttoi(tmpstr.GetBuffer(0));
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

INT CRecycleMgr::SPFExecutePath(CString strPath)
{
	if(strPath.GetLength() == 0) return -1;

	if(strPath == _T("재활용삭제"))
		return Delete();
	else if(strPath == _T("재활용일괄삭제"))
		return AllDelete();
	else if(strPath == _T("재활용수정"))
		return Modify();
	else if(strPath == _T("그리드재활용수정"))
		return ShowSpeciesModifyDialogByGrid();
	else if(strPath == _T("재활용구입처리"))
		return Purchase();
	else if(strPath == _T("재활용자료복원"))
		return Restore();
	else if(strPath == _T("상세검색"))
		return DetailSearch();
	else if(strPath == _T("간략검색"))
		return Search();
	return -1;
}

INT CRecycleMgr::Modify()
{
	CString CMAlias;
	this->GetTempData(_T("CMAlias"), CMAlias);

	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	if(!pCM) return -1000;

	CString GridAlias = _T("MainGrid"); 
	CString DMAlias = _T("DM_재활용자료관리");
	CESL_Grid * pGrid = (CESL_Grid *)pCM->FindControl(GridAlias);
	if(!pGrid) return -1001;

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1003;
	if(pDM->GetRowCount() == 0) return -1006;

	CStringList RecKeyList;
	CArray<INT, INT> GridRowArray;
	
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1006;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(ids < 0) return -1006;

	CString skey;
	while(i >= 0)
	{
		pDM->GetCellData(_T("IBS_종KEY"), i, skey);
		RecKeyList.AddTail(skey);
		GridRowArray.Add(i);
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
	}
	
	if(RecKeyList.GetCount() == 0) return -1006;
	return 0;
}

INT CRecycleMgr::CreateSpeciesUpdateDialog(CESL_Grid *pGrid, CESL_DataMgr *pDM, CString skeyAlias, CString pkeyAlias)
{
/*	if(!pGrid || !pDM || skeyAlias.GetLength() == 0 || pkeyAlias.GetLength() == 0)
		return -1004;
*/

	return 0;
}

CString CRecycleMgr::GetGridCtrlAlias(INT tabCount, BOOL isSpecies)
{
	if(tabCount == 0)
		return _T("MainGrid_Species");
	return _T("MainGrid_Book");
}

CString CRecycleMgr::GetMainDMAlias(INT tabCount, BOOL isSpecies)
{
	if(tabCount == 0)
		return _T("DM_재활용_종");
	return _T("DM_재활용_책");
}


INT CRecycleMgr::ShowSpeciesModifyDialogByGrid()
{
	CString CMAlias;
//	BOOL isSpecies;
//	INT tabCount;
	this->GetTempData(_T("CMAlias"), CMAlias);
//	this->GetTempData(_T("IsSpecies"), isSpecies);
//	this->GetTempData(_T("TabCount"), tabCount);

	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	if(!pCM) return -1000;
	CString GridAlias = _T("MainGrid"); //GetGridCtrlAlias(tabCount, isSpecies);
	CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(GridAlias);
	if(!pGrid) return -1000;

	INT count = pGrid->GetCount();
	if(count <= 0) return 0;
	for(INT i=1 ; i<count ; i++)
	{
		pGrid->SetAt(i, 0, _T(""));
	}
	INT nRow = pGrid->GetRow();
	if(nRow < 0) return -1001;
	pGrid->SetAt(nRow-1, 0, _T("V"));
	pGrid->SetReverse(nRow-1);
	return this->Modify();
}

VOID CRecycleMgr::ShowRequestMailDlg(CStringList &skeyList, BOOL isSpecies)
{
/*	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_재활용_종"));
//	CREQUEST_MAIL_MGR mailMgr(m_pParentMgr);
//	INT result = mailMgr.SetFunishKeyList(skeyList, isSpecies, pDM->CONNECTION_INFO);
	CString tmpstr;
	if(result > 0)
	{	
		tmpstr.Format(_T("%d건의 비치희망신청자료가 있습니다.\n신청자에게 알림 메일을 보내시겠습니까?"), result);
		if(IDOK == m_pParentMgr->ESLAfxMessageBox(tmpstr.GetBuffer(0), MB_OKCANCEL))
		{
//			mailMgr.SetOpenCode(1000);
//			mailMgr.DoModal();
//			mailMgr.SetMailDM();
		}
	}*/
}

VOID CRecycleMgr::ConvertWorkingStatusDescToCode(CString &workingst)
{
	if(workingst == _T("적용안함"))
		workingst = _T("");
	else if(workingst == _T("구입대상자료"))
		workingst = _T("BOA111N");
	else if(workingst == _T("검수대상자료"))
		workingst = _T("BOA113N");
	else if(workingst == _T("미납자료"))
		workingst = _T("BOA114O");
	else if(workingst == _T("검수완료자료"))
		workingst = _T("BOA141N");
	else if(workingst == _T("편목인계자료"))
		workingst = _T("BOC111N");
	else if(workingst == _T("편목대상자료"))
		workingst = _T("BOC113N");
}

INT CRecycleMgr::SendFrame(CESL_DataMgr *pDM_OUT, INT nSendCount)
{
	if (!pDM_OUT) return -1;
	if (nSendCount <= 0) return -2;
	
	INT ids;
	while(TRUE) {
		if (pDM_OUT->frame.IsEmpty() == TRUE) break;
		ids = pDM_OUT->ORACLE_SendFrame2(nSendCount);
		if (ids < 0) return ids;
	}
	return 0;
}
