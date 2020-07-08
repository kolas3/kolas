// RequestManager.cpp: implementation of the CRequestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RequestManager.h"
#include "../../../공통/BO_ACQ_COMMON/RequestMailMgr.h"
#include "../../../공통/BO_SPECIES_API/BO_SPECIES.h"
#include "../../../공통/Duplicate_Data_Search/DuplicateSearch_Interface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define _RRN_LEN    14 
#define WORK_LOG _T("비치희망정보처리"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRequestManager::CRequestManager(CESL_Mgr* pWnd):CSPF_Work(pWnd)
{
	pMakeSearchData = new CMakeSearchData(pWnd);
	m_bRemarkCode	 = FALSE;
	m_bFurnishStatus = FALSE;
}
CRequestManager::~CRequestManager()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL;
	}
}
INT CRequestManager::SPFExecutePath(CString PathAlias)
{
	if( PathAlias.GetLength() == 0 )	return -1;
	
	if		( PathAlias == _T("비치희망입력")	 )		return InsertREQData();
	else if	( PathAlias == _T("비치희망수정")	 )		return ModifyREQData();
	else if	( PathAlias == _T("비치희망삭제")	 )		return DeleteREQData();
	else if	( PathAlias == _T("비치희망구입처리"))		return GoPurchase();
	else if	( PathAlias == _T("비치희망기증처리"))		return GoPurchase();
	else if	( PathAlias == _T("비치상태변경")	 )		return ChangeFurnishStatus();
	else if	( PathAlias == _T("상세검색")		 )		return DetailSearch();

	return -1;
}

INT CRequestManager::InsertREQData( )
{	
	CString CMAlias, DMAlias;
	BOOL bSMSUseYN = FALSE;
	BOOL bReservationYN = FALSE;

	INT nCheck = 0;

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	if (!pDM || !pCM) return -1000;

	INT ids = CopyCMToDM(pCM, pDM, -1);
	if(ids < 0) return -1002;
	INT nCurRow = pDM->GetRowCount() - 1;

	this->GetTempData(_T("SMSUSEYN"), bSMSUseYN);
	this->GetTempData(_T("RESERVATIONYN"), bReservationYN);

	CString strTmpData, strHp, strEmail;
	pCM->GetControlMgrData(_T("핸드폰1"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += strTmpData;
	pCM->GetControlMgrData(_T("핸드폰2"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
	pCM->GetControlMgrData(_T("핸드폰3"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
	pCM->GetControlMgrData(_T("EMAIL_ID"), strTmpData);		if(!strTmpData.IsEmpty()) strEmail += strTmpData;
	CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(_T("EMAIL_SITE"));
	pCMB->GetWindowText(strTmpData);						if(!strTmpData.IsEmpty() && !strEmail.IsEmpty() && strTmpData != _T("적용안함")) strEmail += _T("@") + strTmpData;

	pDM->SetCellData(_T("핸드폰"), strHp, nCurRow);
	pDM->SetCellData(_T("EMAIL"), strEmail, nCurRow);

	if(bSMSUseYN) pDM->SetCellData(_T("SMS수신여부"), _T("Y"), nCurRow);
	else pDM->SetCellData(_T("SMS수신여부"), _T("N"), nCurRow);
	
	if(bReservationYN) pDM->SetCellData(_T("우선대출예약여부"), _T("Y"), nCurRow);
	else pDM->SetCellData(_T("우선대출예약여부"), _T("N"), nCurRow);
	
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("PRIORITYYN"), nCheck);

	CString rec_key, lastwork, firstwork;
	firstwork = m_pParentMgr->GetWorkLogMsg(WORK_LOG);
	lastwork = m_pParentMgr->GetWorkLogMsg(WORK_LOG);
	
	pDM->MakeRecKey(rec_key);
	pDM->SetCellData(_T("비치희망KEY"), rec_key, nCurRow);
	pDM->SetCellData(_T("비치상태"), _T("1"), nCurRow);
	pDM->SetCellData(_T("비치상태설명"), _T("신청중"), nCurRow);
	pDM->SetCellData(_T("최초작업"), firstwork, nCurRow);
	pDM->SetCellData(_T("마지막작업"), lastwork, nCurRow);
	strTmpData = pDM->GetCellData(_T("본표제"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("본표제인덱스"), strTmpData, nCurRow);
	strTmpData = pDM->GetCellData(_T("저작자"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("저작자인덱스"), strTmpData, nCurRow);
	strTmpData = pDM->GetCellData(_T("발행자"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("발행자인덱스"), strTmpData, nCurRow);
	strTmpData = pDM->GetCellData(_T("신청자"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("신청자인덱스"), strTmpData, nCurRow);

	if(nCheck == 0) 
	{
		pDM->SetCellData(_T("우선정리자료"), _T("N"), nCurRow);
		pDM->SetCellData(_T("UDF_우선정리자료"), _T(""), nCurRow);
	}
	else 
	{
		pDM->SetCellData(_T("우선정리자료"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("UDF_우선정리자료"), _T("★"), nCurRow);
	}

	strTmpData = pDM->GetCellData(_T("취소사유"), nCurRow);
	strTmpData.TrimLeft(); strTmpData.TrimRight();
	strTmpData.Replace(_T("\r"), _T(" "));  strTmpData.Replace(_T("\n"), _T(" "));
	pDM->SetCellData(_T("UDF_취소사유"), strTmpData, nCurRow);

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	RemoveAliasList.Add(_T("UDF_우선정리자료"));
	pDM->StartFrame();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, nCurRow, pDM);
	ids = pDM->SendFrame();
	if (ids < 0) return -1001;
	pDM->EndFrame();

	return 0;
}

INT CRequestManager::ModifyREQData( )
{
	CString CMAlias, DMAlias,MainCMAlias;
	INT idx;
	BOOL bSMSUseYN = FALSE;
	BOOL bReservationYN = FALSE;
	INT nCheck = 0;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("MainCMAlias"), MainCMAlias);
	this->GetTempData(_T("SMSUSEYN"), bSMSUseYN);
	this->GetTempData(_T("RESERVATIONYN"), bReservationYN);
	this->GetTempData(_T("IDX"), idx);
	this->GetTempData(_T("PRIORITYYN"), nCheck);

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);

	if (!pDM || !pCM) return -1000;

	INT ids = CopyCMToDM(pCM, pDM, idx);
	if(ids < 0) return -1002;
	INT nCurRow = idx;

	CString rec_key, lastwork;
	lastwork = m_pParentMgr->GetWorkLogMsg(WORK_LOG);
	
	CString strTmpData, strHp, strEmail;
	pCM->GetControlMgrData(_T("핸드폰1"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += strTmpData;
	pCM->GetControlMgrData(_T("핸드폰2"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
	pCM->GetControlMgrData(_T("핸드폰3"), strTmpData);		if(!strTmpData.IsEmpty()) strHp += _T("-") + strTmpData;
	pCM->GetControlMgrData(_T("EMAIL_ID"), strTmpData);		if(!strTmpData.IsEmpty()) strEmail += strTmpData;
	
	CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(_T("EMAIL_SITE"));
	pCMB->GetWindowText(strTmpData);						if(!strTmpData.IsEmpty() && !strEmail.IsEmpty() && strTmpData != _T("적용안함")) strEmail += _T("@") + strTmpData;
	
	pDM->SetCellData(_T("핸드폰"), strHp, nCurRow);
	pDM->SetCellData(_T("EMAIL"), strEmail, nCurRow);
	rec_key = pDM->GetCellData(_T("비치희망KEY"), nCurRow);

	if(pDM->GetCellData(_T("비치상태"), nCurRow) == _T("신청중")) pDM->SetCellData(_T("비치상태"), _T("1"), nCurRow);
	else if(pDM->GetCellData(_T("비치상태"), nCurRow) == _T("처리중")) pDM->SetCellData(_T("비치상태"), _T("2"), nCurRow);
	else if(pDM->GetCellData(_T("비치상태"), nCurRow) == _T("소장중")) pDM->SetCellData(_T("비치상태"), _T("3"), nCurRow);
	else if(pDM->GetCellData(_T("비치상태"), nCurRow) == _T("취소됨")) pDM->SetCellData(_T("비치상태"), _T("4"), nCurRow);

	pDM->SetCellData(_T("마지막작업"), lastwork, nCurRow);
	strTmpData = pDM->GetCellData(_T("본표제"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("본표제인덱스"), strTmpData, nCurRow);
	strTmpData = pDM->GetCellData(_T("저작자"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("저작자인덱스"), strTmpData, nCurRow);
	strTmpData = pDM->GetCellData(_T("발행자"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("발행자인덱스"), strTmpData, nCurRow);
	strTmpData = pDM->GetCellData(_T("신청자"), nCurRow);
	pMakeSearchData->GetKeyWord(strTmpData);
	pDM->SetCellData(_T("신청자인덱스"), strTmpData, nCurRow);
	if(bSMSUseYN) pDM->SetCellData(_T("SMS수신여부"), _T("Y"), nCurRow);
	else pDM->SetCellData(_T("SMS수신여부"), _T("N"), nCurRow);
	if(bReservationYN) pDM->SetCellData(_T("우선대출예약여부"), _T("Y"), nCurRow);
	else pDM->SetCellData(_T("우선대출예약여부"), _T("N"), nCurRow);

	if(nCheck == 0) 
	{
		pDM->SetCellData(_T("우선정리자료"), _T("N"), nCurRow);
		pDM->SetCellData(_T("UDF_우선정리자료"), _T(""), nCurRow);
	}
	else 
	{
		pDM->SetCellData(_T("우선정리자료"), _T("Y"), nCurRow);
		pDM->SetCellData(_T("UDF_우선정리자료"), _T("★"), nCurRow);
	}

	strTmpData = pDM->GetCellData(_T("취소사유"), nCurRow);
	strTmpData.TrimLeft(); strTmpData.TrimRight();
	strTmpData.Replace(_T("\r"), _T(" "));  strTmpData.Replace(_T("\n"), _T(" "));
	pDM->SetCellData(_T("UDF_취소사유"), strTmpData, nCurRow);

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_우선정리자료"));
	pDM->StartFrame();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, idx, pDM, _T("REC_KEY"), _T("NUMERIC"), rec_key);
	ids = pDM->SendFrame();
	if (ids < 0) return -1001;
	pDM->EndFrame();
	 
	return 0;

}

INT CRequestManager::DeleteREQData()
{
	CString DMAlias, CMAlias;
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("CMAlias"), CMAlias);
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, _T("MainGrid"));
	if(ids < 0) return -1001;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, _T("MainGrid"));
	if(i < 0) return -1001;

	CString rec_key, acq_key, species_query, tmpstr, strTableName, strAcqCode;
	CStringList idxList;

	pDM->StartFrame();
	INT count = 0;

	while(i >= 0)
	{
		m_pParentMgr->GetDataMgrData(DMAlias, _T("수입구분"), strAcqCode, i);
		m_pParentMgr->GetDataMgrData(DMAlias, _T("비치희망KEY"), rec_key, i);
		m_pParentMgr->GetDataMgrData(DMAlias, _T("수서정보KEY"), acq_key, i);
		strTableName.Empty();
		if(!acq_key.IsEmpty())
		{
			if(strAcqCode == _T("1")) strTableName = _T("BO_PURCHASEINFO_TBL");
			else if(strAcqCode == _T("2")) strTableName = _T("BO_DONINFO_TBL");

			if(!strTableName.IsEmpty())
			{
				species_query.Format(_T("UPDATE %s SET INPUT_TYPE = '%s' WHERE REC_KEY = %s;"), 
					strTableName, _T("0"), acq_key);
				pDM->AddFrame(species_query);			
			}
		}
		pDM->MakeDeleteFrame(_T("BO_FURNISH_TBL"), _T("REC_KEY"), _T("NUMERIC"), rec_key);
		tmpstr.Format(_T("%d"), i);
		idxList.AddTail(tmpstr);
		i = m_pParentMgr->SelectGetNext(CMAlias, _T("MainGrid"));
		count++;
	}
	ids = pDM->SendFrame();
	if(ids < 0) return -1002;
	ids = pDM->EndFrame();
	if(ids < 0) return -1002;

	ids = this->DeleteSelectRow(pDM, idxList);
	
	return 0;
}

INT CRequestManager::DetailViewREQData()
{
	return 0;
}

INT CRequestManager::Search(CESL_ControlMgr * pCM)
{
	if(!pCM) return -1000;	
	CString strWhere;		strWhere.Empty();
	CString strOption;		strOption.Empty();

	CString Status = _T("");
	pCM->GetControlMgrData(_T("비치상태"), Status);
	CString Date1 = _T("");
	pCM->GetControlMgrData(_T("신청일자1"), Date1);
	CString Date2 = _T("");
	pCM->GetControlMgrData(_T("신청일자2"), Date2);
	CString Title = _T("");
	pCM->GetControlMgrData(_T("도서명"), Title);
	
	Status.TrimLeft();	Status.TrimRight();
	Date1.TrimLeft();	Date1.TrimRight();
	Date2.TrimLeft();	Date2.TrimRight();
	Title.TrimLeft();	Title.TrimRight();

	if(!Title.IsEmpty())
	{
		strOption.Format(_T(" TITLE LIKE '%s%%'"), Title);
		strWhere += strOption;
	}
	
	if(!Status.IsEmpty())
	{
		if(strWhere.IsEmpty())
			strOption.Format(_T(" FURNISH_STATUS = '%s'"), Status);
		else
			strOption.Format(_T(" AND FURNISH_STATUS = '%s'"), Status);
		strWhere += strOption;
	}
	
	strWhere += MakeDateQuery(Date1, Date2, strWhere);

	SetQuery(strWhere);
	return 0;
}

INT CRequestManager::DetailSearch()
{
	CString CMAlias, DMAlias;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	if(!pCM) return -1000;

	CString status  = _T("");
	CString sosok = _T("");
	CString requestor = _T("");
	CString req_date1 = _T("");
	CString req_date2 = _T("");
	CString title = _T("");
	CString author = _T("");
	CString publisher = _T("");
	CString pub_year = _T("");

	INT ids = -1;
	ids = pCM->GetControlMgrData(_T("비치상태"), status);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("신청자"), requestor);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("신청일자1"), req_date1);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("신청일자2"), req_date2);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("본표제"), title);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("저작자"), author);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("발행자"), publisher);
	if(ids < 0) return -1001;
	ids = pCM->GetControlMgrData(_T("발행년"), pub_year);
	if(ids < 0) return -1001;

	status.TrimLeft();		status.TrimRight();
	req_date1.TrimLeft();	req_date1.TrimRight();	
	req_date2.TrimLeft();	req_date2.TrimRight();	
	pub_year.TrimLeft();	pub_year.TrimRight();
	title.TrimLeft();		title.TrimRight();
	author.TrimLeft();		author.TrimRight();		
	publisher.TrimLeft();	publisher.TrimRight();
	requestor.TrimLeft();	requestor.TrimRight();

	CString strWhere = _T("");
	CString strOption = _T("");

	ids = pMakeSearchData->IsValidYearData(pub_year);
	if(ids < 0) return -1002;

	if(!status.IsEmpty())
	{
		strOption.Format(_T(" FURNISH_STATUS = '%s'"), status);
		strWhere += strOption;
	}
	
	strWhere += MakeDateQuery(req_date1, req_date2, strWhere);

	if(!(requestor.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND APPLICANT_NAME LIKE '%s%%'"), requestor);
		}
		else
		{
			strOption.Format(_T("APPLICANT_NAME LIKE '%s%%'"), requestor);
		}
		strWhere += strOption;
	}
	
	if(!(title.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND TITLE LIKE '%s%%'"), title);
		}
		else
		{
			strOption.Format(_T("TITLE LIKE '%s%%'"), title);
		}
		strWhere += strOption;
	}

	if(!(author.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND AUTHOR LIKE '%s%%'"), author);
		}
		else
		{
			strOption.Format(_T("AUTHOR LIKE '%s%%'"), author);
		}
		strWhere += strOption;
	}
	
	if(!(publisher.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND PUBLISHER LIKE '%s%%'"), publisher);
		}
		else
		{
			strOption.Format(_T("PUBLISHER LIKE '%s%%'"), publisher);
		}
		strWhere += strOption;
	}

	if(!(pub_year.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND PUBLISH_YEAR = '%s'"), pub_year);
		}
		else
		{
			strOption.Format(_T("PUBLISH_YEAR = '%s'"), pub_year);
		}
		strWhere += strOption;
	}

	SetQuery(strWhere);

	ids = RefreshDM(DMAlias);
	if(ids < 0) return -1002;

	return 0;
}

INT CRequestManager::GoPurchase()
{
	return 0;	
}

CString CRequestManager::MakeStatusQuery(CString Status, CString WhereStmt)
{
	if(!WhereStmt.IsEmpty())
	{
		if(Status.IsEmpty() && Status == _T("적용안함"))
			return _T("");
		else if(Status == _T("신청중"))
			return _T(" AND FURNISH_STATUS = '1'");
		else if(Status == _T("구입중"))
			return _T(" AND FURNISH_STATUS = '2'");
		else if(Status == _T("기증접수중"))
			return _T(" AND FURNISH_STATUS = '3'");
		else if(Status == _T("소장중"))
			return _T(" AND FURNISH_STATUS = '4'");
		else if(Status == _T("취소됨"))
			return _T(" AND FURNISH_STATUS = '5'");
	}
	else
	{
		if(Status.IsEmpty() && Status == _T("적용안함"))
			return _T("");
		else if(Status == _T("신청중"))
			return _T("FURNISH_STATUS = '1'");
		else if(Status == _T("구입중"))
			return _T("FURNISH_STATUS = '2'");
		else if(Status == _T("기증접수중"))
			return _T("FURNISH_STATUS = '3'");
		else if(Status == _T("소장중"))
			return _T("FURNISH_STATUS = '4'");
		else if(Status == _T("취소됨"))
			return _T("FURNISH_STATUS = '5'");
	}
	return _T("");
}

CString CRequestManager::MakeDateQuery(CString Date1, CString Date2, CString WhereStmt)
{
	CString query = _T("");

	if(!WhereStmt.IsEmpty())
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(Date1.IsEmpty() && !(Date2.IsEmpty()))
		{
			m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
			return _T("");
		}
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			query.Format(_T(" AND APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			query.Format(_T(" AND APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date2);
			return query;
		}
	}
	else
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(Date1.IsEmpty() && !(Date2.IsEmpty()))
		{
			m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
			return _T("");
		}
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			query.Format(_T("APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			query.Format(_T("APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date2);
			return query;
		}
	}
	return _T("");
}

VOID CRequestManager::SetQuery(CString query)
{
	m_pQuery = query;
}

INT CRequestManager::RefreshDM(CString DMAlias)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;
	INT ids;
	ids = pDM->RefreshDataManager(m_pQuery);
	if(ids < 0) return -1002;

	return 0;
}

INT CRequestManager::UpdateFurnishDataStatus(CESL_DataMgr *pDM, CString status)
{
	INT rowCnt = pDM->GetRowCount();
	pDM->StartFrame();

	CString query, furnish_key;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(_T("비치희망KEY"), i, furnish_key);
		query.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS = '%s' WHERE REC_KEY = %s"), status, furnish_key);
		pDM->AddFrame(query);
	}

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	return ids;
}

INT CRequestManager::SendMail(CESL_DataMgr *pDM, INT status)
{
	return 0;
}

INT CRequestManager::ChangeFurnishStatus()
{
	INT ids, i;
	CString CMAlias, DMAlias, MainCMAlias, GridAlias, Status, CancelReason, strFurnishDate, reckey, skey ;
	CStringArray ArrayFurnishKey;
	CESL_DataMgr* pDM = NULL;

	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	this->GetTempData(_T("MainCMAlias"), MainCMAlias);
	this->GetTempData(_T("Status"), Status);
	this->GetTempData(_T("Cancel_Reason"), CancelReason);
	this->GetTempData(_T("Furnish_Date"), strFurnishDate);
	
	pDM = m_pParentMgr->FindDM(DMAlias);
	if( pDM == NULL )	return -1000;

	ids = m_pParentMgr->SelectMakeList(MainCMAlias, GridAlias);
	if( ids < 0 )	return -1001;
	i = m_pParentMgr->SelectGetHeadPosition(MainCMAlias, GridAlias);
	if( i < 0 )		return -1001;	
	
	ArrayFurnishKey.RemoveAll();

	pDM->StartFrame();
	while( i >= 0 )
	{
		pDM->GetCellData(_T("비치희망KEY"), i, reckey);
		ArrayFurnishKey.Add(reckey);
		
		pDM->GetCellData(_T("종KEY"), i, skey);

		pDM->InitDBFieldData();

		if( m_bRemarkCode == TRUE )
			{
				pDM->SetCellData(_T("취소사유"), CancelReason, i);
				pDM->AddDBFieldData(_T("CANCEL_REASON"), _T("STRING"), CancelReason, 1);
			}

		if( m_bFurnishStatus == TRUE )
		{
			if( Status == _T("3") )
			{
				if( strFurnishDate.IsEmpty() == TRUE )	strFurnishDate = GetTodayDate();
				pDM->SetCellData(_T("비치일자"),  strFurnishDate, i);
				pDM->AddDBFieldData(_T("FURNISH_DATE"), _T("STRING"), strFurnishDate);
			}
			else 
			{
				pDM->SetCellData(_T("비치일자"), _T(""), i);
				pDM->AddDBFieldData(_T("FURNISH_DATE"), _T("STRING"), _T(""));
			}
			pDM->SetCellData(_T("비치상태"), Status, i);
			pDM->AddDBFieldData(_T("FURNISH_STATUS"), _T("STRING"), Status);
			}
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));

		pDM->MakeUpdateFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), reckey, i);	
		
		i = m_pParentMgr->SelectGetNext(MainCMAlias, GridAlias);		
	}

	ids = pDM->SendFrame();
	pDM->EndFrame();
	if( ids < 0 )	return -1002;

	if(m_bFurnishStatus == TRUE)
	{

	}	
	
	return 0;
}

INT CRequestManager::ChangeFurnishStatusToCancel()
{
	CString CMAlias, DMAlias, MainCMAlias, GridAlias, Status, CancelReason;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	this->GetTempData(_T("MainCMAlias"), MainCMAlias);
	this->GetTempData(_T("Cancel_Reason"), CancelReason);
	
	return 0;
}

INT CRequestManager::ChangeFurnishStatusByHavingData(CString status)
{
	CString CMAlias, DMAlias, MainCMAlias, GridAlias, Status, CancelReason;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	this->GetTempData(_T("MainCMAlias"), MainCMAlias);
	this->GetTempData(_T("Status"), Status);
	this->GetTempData(_T("Cancel_Reason"), CancelReason);
	
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;
	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0) return -1001;
	INT i = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(i < 0) return -1001;
	CString reckey, skey ;
	
	pDM->StartFrame();

	while(i >= 0)
	{	
		pDM->GetCellData(_T("비치희망KEY"), i, reckey);
		pDM->GetCellData(_T("종KEY"), i, skey);
		if(Status != _T("1") && skey.GetLength() == 0)
		{
			i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
			continue;
		}

		pDM->InitDBFieldData();

		if(Status == _T("6"))
		{
			pDM->SetCellData(_T("비치상태"), Status, i);
			pDM->AddDBFieldData(_T("FURNISH_STATUS"), _T("STRING"), Status);
			if(m_bRemarkCode)  pDM->AddDBFieldData(_T("CANCEL_REASON"), _T("STRING"), CancelReason);
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		}
		else 
		{
			pDM->SetCellData(_T("비치상태"), Status, i);
			pDM->AddDBFieldData(_T("FURNISH_STATUS"), _T("STRING"), Status);
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		}
		pDM->MakeUpdateFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), reckey, i);	
		
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
	}

	ids = pDM->SendFrame();
	if(ids < 0) return -1002;

	pDM->EndFrame();

	if(Status == _T("소장중")) ids = SendMail(pDM, 4);
	else ids = SendMail(pDM, 5);
	if(ids < 0) return -1003;
	return 0;

	return 0;
}

INT CRequestManager::ChangeFurnishStatusToRequesting()
{
	CString CMAlias, DMAlias, MainCMAlias, GridAlias, Status, CancelReason;
	this->GetTempData(_T("CMAlias"), CMAlias);
	this->GetTempData(_T("DMAlias"), DMAlias);
	this->GetTempData(_T("GridAlias"), GridAlias);
	this->GetTempData(_T("MainCMAlias"), MainCMAlias);
	return 0;
}


BOOL CRequestManager::IsRRN(CString sRRN)
{

    if( sRRN.IsEmpty() == TRUE )	return TRUE;

    sRRN.Replace(_T(" "), _T(""));

    if( sRRN.GetLength() != _RRN_LEN )	return FALSE;

    if( sRRN[6] != '-' )	return FALSE;

    INT Gender;

    Gender = sRRN[7]-'0';
    if( Gender < 1 || Gender > 4 )	return FALSE;

    INT Val = 0;
    INT nWeight[] = {

        2, 3, 4, 5, 6, 7, 

        0,

        8, 9, 2, 3, 4, 5

    };

    for (INT i=0; i<_RRN_LEN-1; i++) 

        Val += (sRRN[i] - '0') * nWeight[i];

    Val = 11 - (Val % 11);

    Val = Val % 10;

    if (Val == (sRRN[13]-'0')) return TRUE;

    return FALSE;
}

BOOL CRequestManager::CheckDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}

INT CRequestManager::ChangeFurnishStatusCodeToDesc(CESL_DataMgr *pDM)
{
	if(!pDM) return -1000;
	INT rowCnt = pDM->GetRowCount();
	if(rowCnt <= 0) return 0;
	INT ids = -1;
	CString tmpstr;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		ids = pDM->GetCellData(_T("비치상태"), i, tmpstr);
		if(ids < 0) return -1001;
		if(tmpstr.GetLength() == 0)
			ids = pDM->SetCellData(_T("비치상태설명"), _T("적용안함"), i);
		else if(tmpstr == _T("1"))
			ids = pDM->SetCellData(_T("비치상태설명"), _T("신청중"), i);
		else if(tmpstr == _T("2"))
			ids = pDM->SetCellData(_T("비치상태설명"), _T("구입중"), i);
		else if(tmpstr == _T("3"))
			ids = pDM->SetCellData(_T("비치상태설명"), _T("기증접수중"), i);
		else if(tmpstr == _T("4"))
			ids = pDM->SetCellData(_T("비치상태설명"), _T("정리중"), i);
		else if(tmpstr == _T("5"))
			ids = pDM->SetCellData(_T("비치상태설명"), _T("소장중"), i);
		else if(tmpstr == _T("6"))
			ids = pDM->SetCellData(_T("비치상태설명"), _T("취소됨"), i);

		if(ids < 0) return -1002;
	}
	return 0;
}

INT CRequestManager::DeleteSelectRow(CESL_DataMgr * pDM, CStringList &idxlist)
{
	if(idxlist.IsEmpty() || !pDM) return -1000;
	POSITION pos = idxlist.GetTailPosition();
	CString tmpstr;
	INT rowidx, ids;
	while(pos != NULL)
	{
		tmpstr = idxlist.GetPrev(pos);
		rowidx = _ttoi(tmpstr.GetBuffer(0));
		ids = pDM->DeleteRow(rowidx);
		if(ids < 0)
			return -1001;
	}
	return 0;
}

INT CRequestManager::CopyCMToDM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, INT idx)
{
	INT ids, nCurRow;
	if(idx == -1) 
	{
		ids = pDM->InsertRow(-1);
		if (ids != 0) return ids;
		nCurRow =  pDM->GetRowCount() - 1;
	}
	else 
		nCurRow = idx;

	CString strTmp;
	INT cecount = pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (!pCE) continue;		
		ids = pCM->GetControlMgrData(pCE->GetAlias(), strTmp);
		if (ids < 0) continue;
		pDM->SetCellData(pCE->GetAlias(), strTmp, nCurRow);
	}
	return nCurRow;
}

INT CRequestManager::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL m_bIndex/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		if(m_bIndex) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
		if(pos <= 0)
		{
			pTargetArray->Add(strSource);
		}

	}
	if(m_bIndex) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CRequestManager::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}


CString CRequestManager::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CRequestManager::SetUDFCheck(CESL_DataMgr *pDM, CString strAlias)
{
	INT col, nCols;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	if( pDM == NULL || strAlias.IsEmpty() == TRUE )		return -1;
	
	nCols = pDM->m_nCols;
	if( nCols < 0 )		 nCols = pDM->GetRefColumnCount();
	for( col = 0; col < nCols; col++ ) 
		{
		if( pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE) != 0 )
			continue;
		if( FIELD_ALIAS == strAlias )
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}

	return 0;
}	
