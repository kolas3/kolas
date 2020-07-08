// DataChangeManager.cpp: implementation of the CDataChangeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataChangeManager.h"
#include "../BO_ACQ_GROUP_MANAGER/GroupPair.h"
#include "../COMMON_CONVERSION_MGR/MarcInfoConversionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("수서/코드정보변경"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CDataChangeManager::CDataChangeManager(CESL_Mgr * pMgr):CSPF_Work(pMgr)
{
	pFormApi = new CBOFormApi(pMgr);
	pSpeciesApi = new CBO_SPECIES(pMgr);
	m_pDataProcessingDlg = NULL;
	m_MapSpeciesKey.RemoveAll();
	m_bSpeciesSubjectCode = FALSE;
	m_bVolYN = FALSE;
	m_bRefreshCopyCode = FALSE;
	m_bCatCode = FALSE;
	bChangeSubjectCode = FALSE;
}

CDataChangeManager::SetExchangeRate( CString strExchangeRate )
{
	m_ExchangeRate  =  strExchangeRate;
}	

// 소멸자
CDataChangeManager::~CDataChangeManager()
{
	if( pFormApi != NULL )
	{
		delete pFormApi;
		pFormApi = NULL;
	}
	
	if( pSpeciesApi != NULL )
	{
		delete pSpeciesApi;
		pSpeciesApi = NULL;
	}

	if( m_pDataProcessingDlg != NULL )
	{
		DeleteDataProcessingDlg();
	}

	DeleteDMArray();
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로 들어온 값에 해당하는 함수를 호출한다.
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::SPFExecutePath(CString strPath)
{
	if( strPath.GetLength() == 0 )	return -1;

	if		( strPath == _T("코드변경")		)		return CodeChange();
	else if	( strPath == _T("기증정보변경") )		return DonateInfoChange();
	else if	( strPath == _T("수서정보변경") )		return AcqInfoChange();
	else if	( strPath == _T("구입정보변경") )		return PurchaseInfoChange();
	else if	( strPath == _T("책코드변경")	)		return BookCodeChange();
	else if	( strPath == _T("최근환율정보추출") )	return GetLatestExchangeRate();

	return -1;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 코드 변경을 수행한다.
//
// [ 미니스펙 ]
// 1. 저장된 임시데이터 및 포인터에서 값을 가져온다.
// 2. 입수코드가 기초자료가 아니고 종이 아니라면 멤버변수 m_bVolYN을 TRUE로 설정한다.
// 3. 위에서 구한 값으로 책정보라면 권일련번호를 가져온다.
// 4. 종키값을 가져온다.
// 5. 선정된 자료들의 코드값을 변경한다.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::CodeChange()
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids, i, nPos, nBookIdx, nAcqCode = -1;
	BOOL bSpeciesSubjectCode = FALSE;
	BOOL bDeleteData = FALSE;
	BOOL bSpeciesYN, bApplyYN, bWorkCodeIgnore, bTmpData;
	CString strSpeciesKeyAlias, strVolSortNoAlias, strSpeciesKey, strVolSortNo, strMsg, strTmpData;
	CStringArray arrIgnoreSpecies;
	CCodeChangeCondition* pCont = NULL;

	
	//================================================================
	// 1. 저장된 임시데이터 및 포인터에서 값을 가져온다.
	//================================================================
	bSpeciesYN = bApplyYN = bWorkCodeIgnore = bTmpData = FALSE;

	CESL_DataMgr* pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_DataMgr* pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_ControlMgr* pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid* pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pCont = (CCodeChangeCondition*)GetTempPtr(_T("Condition"));
	//CM/DM검증
	if( pDM == NULL || pCM == NULL || pCont == NULL || pGrid == NULL)	return -1000;	
	
	GetTempData(_T("AcqCode"), nAcqCode);
	GetTempData(_T("SPECIES_SUBJECT_CODE"), bSpeciesSubjectCode);
	GetTempData(_T("IS_DELETE_DATA"), bDeleteData);
	GetTempData(_T("SPECIES_YN"), bSpeciesYN);
	GetTempData(_T("IS_CAT_CODE"), m_bCatCode);



	//================================================================
	// 2. 입수코드가 기초자료가 아니고 종이 아니라면 멤버변수 m_bVolYN을 TRUE로 설정한다.
	//================================================================
	if( nAcqCode != 0 && bSpeciesYN == FALSE )		m_bVolYN = TRUE;
	else m_bVolYN = FALSE;
	
	//================================================================
	// 3. 위에서 구한 값으로 책정보라면 권일련번호를 가져온다.
	//================================================================
	if( m_bVolYN == TRUE )	strVolSortNoAlias = _T("BB_권일련번호");
	else					strVolSortNoAlias.Empty();
	

	//================================================================
	// 4. 종키값을 가져온다.
	//================================================================
	strSpeciesKeyAlias = _T("IBS_종KEY");


	//================================================================
	// 5. 선정된 자료들의 코드값을 변경한다.
	//================================================================
	ids = pGrid->SelectMakeList();
	if( ids < 0 )	return -2000;
	i = pGrid->SelectGetHeadPosition();
	if( i < 0 )		return -2000;

	// Progress Ctrl 설정
	ShowDataProcessingDlg();
	strMsg.Format(_T("선정하신 %d건의 자료를 코드변경하고 있습니다."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	
	nPos = 0;
	nBookIdx = -1;
	arrIgnoreSpecies.RemoveAll();
	// Start
	pDM->StartFrame();
	while( i >= 0 )
	{
		// Progress Ctrl 위치값을 하나씩 늘린다.
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);

		// 종이거나 대표책이라면 bApplyYN을 TRUE로 설정한다.
		if( bSpeciesYN == TRUE )	bApplyYN = TRUE;
		else
		{
			strTmpData = pDM->GetCellData(_T("BB_대표책여부"), i);
			if( strTmpData == _T("Y") )	bApplyYN = TRUE;
		}
		
		// 기초대상자료 입수이고 종이 아니라면 구입일경우에 업무구분코드값을 변경하지 못하도록 한다.
		if( nAcqCode == 0 && bSpeciesYN == FALSE )
		{
			strTmpData = pDM->GetCellData(_T("BB_수입구분"), i);
			if( strTmpData == _T("1") )
			{
				bWorkCodeIgnore = TRUE;
				bTmpData = pCont->GetWorkCode();
				pCont->SetWorkCode(FALSE);
			}
		}
		// 종정보에서 코드변경쿼리를 작성한다.
		MakeSpeciesCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, bSpeciesSubjectCode, bDeleteData, bApplyYN);
		// 구입정보에서 코드변경쿼리를 작성한다.
		MakeAccuCodeChangeQuery(pDM, pCM, pCont, nAcqCode, i, &arrIgnoreSpecies, bSpeciesSubjectCode);
		// 책정보에서 코드변경쿼리를 작성한다.
		MakeBookCodeChangeQuery(pDM, pCM, pCont, i, bSpeciesYN, &arrIgnoreSpecies, bDeleteData);
		// 색인정보에서 코드변경쿼리를 작성한다.
		MakeIndexCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, bApplyYN);
		
		
		if( bWorkCodeIgnore == TRUE )
		{
			pCont->SetWorkCode(bTmpData);
			bWorkCodeIgnore = FALSE;
		}

		// 종DM을 CM의 값으로 적용한다.
		pFormApi->MakeDMFromCM(pCM, pDM, i, _T(""), &arrIgnoreSpecies);

		// 책DM이 있다면 책DM을 CM의 값으로 적용한다.
		if( pDM_BOOK )
		{
			CMapStringToString tmpMap;	tmpMap.RemoveAll();
			tmpMap.SetAt(strSpeciesKeyAlias, pDM->GetCellData(strSpeciesKeyAlias, i));
			
			if( strVolSortNoAlias.IsEmpty() == FALSE )
				tmpMap.SetAt(strVolSortNoAlias, pDM->GetCellData(strVolSortNoAlias, i));

			nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0);
			while( nBookIdx >= 0 )
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}

		//=====================================================
		//2009.02.03 UPDATE BY PJW : 주제구분코드가 ""일경우는 적용안함으로 들어간다.
// 		if( bSpeciesSubjectCode == FALSE && m_strSubjectCode.IsEmpty() == FALSE ) 
		
		//2009.03.27 UPDATE BY LKS : 주제구분을 변경하지 않을 시 무조건 "" 가 들어가 적용안함으로 변경되는 오류 수정
		if( bSpeciesSubjectCode == FALSE && bChangeSubjectCode ) 
		{
			pDM->SetCellData(_T("BS_주제구분"), m_strSubjectCode, i);
			//20071101 ADD PJW : 코드변경시 BP_주제 구분 포함
			pDM->SetCellData(_T("BP_주제구분"), m_strSubjectCode, i);
			m_strSubjectCode.Empty();
		}
		//=====================================================
		
		
		// 다음 선정자료로 이동한다.
		i = pGrid->SelectGetNext();
	}

	strMsg.Format(_T("구분코드값을 변경중 입니다."));
	m_pDataProcessingDlg->SetMessage(strMsg);

	// Send
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if( ids < 0 )	return -3000;
	// End
	ids = pDM->EndFrame();
	if( ids < 0 )	return -3000;

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종 테이블에 구분코드를 변경하는 쿼리를 작성하여 추가한다.
//
// [ 미니스펙 ]
// 1. 삭제 여부에 따라서 Head쿼리를 작성한다.(삭제시-삭제종테이블, 그외-종테이블)
// 2. CatCode에 따라서 삭제할 자료의 자료상태 범위를 결정하는 Tail쿼리를 작성한다.
// 3. 분류표구분값 변경 쿼리 추가
// 4. 주제구분값 변경 쿼리 추가
// 5. 제어자료구분값 변경 쿼리 추가
// 6. 관리구분값 변경 쿼리 추가(관리구분값 변경시 bApply가 TRUE일 때만 가능)
// 7. 자료구분값 변경 쿼리 추가
// 8. 매체구분값 변경 쿼리 추가 (매체구분값 변경시 bApplyYN이 TRUE일때만 가능)
// 9. 보조등록구분값 변경 쿼리 추가
// 10. 이용제한구분값 변경 쿼리 추가(변경시 bApplyYN이 TRUE일때만 가능)
// 11. 이용대상구분값 변경 쿼리 추가(변경시 bApplyYN이 TRUE일때만 가능)
// 12. 업무구분값 변경 쿼리 추가(변경시 수입구분이 구입일경우만 가능)
// 13. 도서분류코드값 변경 쿼리 추가
// 14. 위에서 추가된 Mid쿼리들을 SQL형식에 맞게 추가한다.
// 15. 완성된쿼리를 AddFrame 한다.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeSpeciesCodeChangeQuery(CESL_DataMgr *pDM, 
												   CESL_ControlMgr *pCM, 
												   CCodeChangeCondition *pCont, 
												   INT idx, 
												   CStringArray * pArrIgnoreSpecies, 
												   BOOL bSpeciesSubjectCode, 
												   BOOL bDeleteData,
												   BOOL bApplyYN)
{
	//================================================================
	// 변수정의
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strQuery;
	CStringArray arrMidQuery;

	//종 구분코드변경

	
	//================================================================
	// 1. 삭제 여부에 따라서 Head쿼리를 작성한다.(삭제시-삭제종테이블, 그외-종테이블)
	//================================================================
	if( bDeleteData == TRUE )	strHeadQuery = _T("UPDATE BO_DELETE_SPECIES_TBL SET ");
	else						strHeadQuery = _T("UPDATE BO_SPECIES_TBL SET ");

	//================================================================
	// 2. CatCode에 따라서 삭제할 자료의 자료상태 범위를 결정하는 Tail쿼리를 작성한다.
	//================================================================
	if( m_bCatCode == FALSE )
	{
		strTailQuery.Format(_T(" WHERE REC_KEY IN ")
							_T("(SELECT REC_KEY FROM IDX_BO_TBL ")							
 							_T("WHERE REC_KEY=%s AND (WORKING_STATUS<'BOC' OR WORKING_STATUS>'BOR'));"),
		pDM->GetCellData(_T("IBS_종KEY"), idx));
	}
	else
	{
		strTailQuery.Format(_T(" WHERE REC_KEY = %s;"), pDM->GetCellData(_T("IBS_종KEY"), idx));
	}


	//================================================================
	// 3. 분류표구분값 변경 쿼리 추가
	//================================================================
	if( pCont->GetClassNoType() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_분류표구분"), strTmpData);
		strTmpQuery.Format(_T("CLASS_NO_TYPE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);

		pDM->SetCellData(_T("IBS_분류표구분"), strTmpData, idx);
	}
	else pArrIgnoreSpecies->Add(_T("BS_분류표구분"));

	//================================================================
	// 4. 주제구분값 변경 쿼리 추가
	//================================================================
	if( pCont->GetSubjectCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BP_주제구분"), strTmpData);
		strTmpQuery.Format(_T("SUBJECT_CODE = '%s'"), strTmpData);
		if(!bSpeciesSubjectCode) m_strSubjectCode = strTmpData;
		arrMidQuery.Add(strTmpQuery);
		bChangeSubjectCode = TRUE;
	}
	else pArrIgnoreSpecies->Add(_T("BP_주제구분"));


	//================================================================
	// 5. 제어자료구분값 변경 쿼리 추가
	//================================================================
	if( pCont->GetControlCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_제어자료구분"), strTmpData);
		strTmpQuery.Format(_T("CONTROL_MAT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_제어자료구분"));

	//================================================================
	// 6. 관리구분값 변경 쿼리 추가(관리구분값 변경시 bApply가 TRUE일 때만 가능)
	//================================================================
	if( pCont->GetManageCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_관리구분"), strTmpData);
		strTmpQuery.Format(_T("MANAGE_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_관리구분"));

	//================================================================
	// 7. 자료구분값 변경 쿼리 추가
	//================================================================
	if( pCont->GetMatCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_자료구분"), strTmpData);
		strTmpQuery.Format(_T("MAT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_자료구분"));

	//================================================================
	// 8. 매체구분값 변경 쿼리 추가 (매체구분값 변경시 bApplyYN이 TRUE일때만 가능)
	//================================================================
	if( pCont->GetMediaCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_매체구분"), strTmpData);
		strTmpQuery.Format(_T("MEDIA_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		//=====================================================
		//2009.01.07 ADD BY PJW : 그리드 DISPLAY를 위해 DM의 데이터도 변경해준다.
		pDM->SetCellData(_T("IBS_매체구분_코드"), strTmpData, idx);
		pCM->GetControlMgrData(_T("BB_매체구분"), strTmpData,-1,1);
		pDM->SetCellData(_T("IBS_매체구분_설명"), strTmpData, idx);

		//=====================================================
		

	}
	else pArrIgnoreSpecies->Add(_T("BS_매체구분"));

	//================================================================
	// 9. 보조등록구분값 변경 쿼리 추가
	//================================================================
	if( pCont->GetSubRegCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_보조등록구분"), strTmpData);
		strTmpQuery.Format(_T("SUB_REG_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		//=====================================================
		//2009.01.07 ADD BY PJW : 그리드 DISPLAY를 위해 DM의 데이터도 변경해준다.
		pDM->SetCellData(_T("IBS_보조등록구분_코드"), strTmpData, idx);
		//=====================================================
	}
	else pArrIgnoreSpecies->Add(_T("BS_보조등록구분"));

	//================================================================
	// 10. 이용제한구분값 변경 쿼리 추가(변경시 bApplyYN이 TRUE일때만 가능)
	//================================================================
	if( pCont->GetUseLimitCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_이용제한구분"), strTmpData);
		strTmpQuery.Format(_T("USE_LIMIT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_이용제한구분"));


	//================================================================
	// 11. 이용대상구분값 변경 쿼리 추가(변경시 bApplyYN이 TRUE일때만 가능)
	//================================================================
	if( pCont->GetUseObjCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_이용대상구분"), strTmpData);
		strTmpQuery.Format(_T("USE_OBJ_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		//=====================================================
		//2009.01.07 ADD BY PJW : 그리드 DISPLAY를 위해 DM의 데이터도 변경해준다.
		pDM->SetCellData(_T("IBS_이용대상구분_코드"), strTmpData, idx);
		//=====================================================
	}
	else pArrIgnoreSpecies->Add(_T("BS_이용대상구분"));

	//================================================================
	// 12. 업무구분값 변경 쿼리 추가(변경시 수입구분이 구입일경우만 가능)
	//================================================================
	if( pCont->GetWorkCode() == TRUE )
	{
		strTmpData = pDM->GetCellData(_T("BP_수입구분"), idx);
		if( strTmpData != _T("1") )
		{
			pCM->GetControlMgrData(_T("BS_업무구분"), strTmpData);
			strTmpQuery.Format(_T("WORK_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
			//2006-02-06 hjy 추가: DM의 업무구분을 수정해준다.
			pDM->SetCellData(_T("IBS_업무구분"),strTmpData,idx);
		}
	}
	else pArrIgnoreSpecies->Add(_T("BS_업무구분"));

	//================================================================
	// 13. 도서분류코드값 변경 쿼리 추가
	//================================================================
	if( pCont->GetUserDefineCode1() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_사용자정의코드1"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE1 = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_사용자정의코드1"));
	if(pCont->GetUserDefineCode2())
	{
		pCM->GetControlMgrData(_T("BS_사용자정의코드2"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE2 = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_사용자정의코드2"));


	//================================================================
	// 14. 위에서 추가된 Mid쿼리들을 SQL형식에 맞게 추가한다.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	
	//================================================================
	// 15. 완성된쿼리를 AddFrame 한다.
	//================================================================
	if( cnt > 0 )
	{
		strTmpData.Format(_T("LAST_WORK='%s'"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		strMidQuery += _T(", ") + strTmpData;
		strQuery = strHeadQuery + strMidQuery + strTailQuery;
		pDM->AddFrame(strQuery);
	}


	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 구입/기증정보테이블에 주제구분코드를 변경하는 쿼리를 작성하여 추가한다.
//
// [ 미니스펙 ]
// 1. 입수구분의 값에 따라 테이블을 설정하는 Head쿼리를 작성한다.
// 2. 수서정보Key값을 가져와 Tail쿼리를 작성한다.
// 3. 주제구분값을 변경하는 쿼리를 추가한다.
// 4. 완성된 쿼리를 AddFrame한다.(수입구분값이 없다면 구입정보/기증테이블 모두 AddFrame한다.)
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeAccuCodeChangeQuery(CESL_DataMgr *pDM,
												CESL_ControlMgr *pCM,
												CCodeChangeCondition *pCont,
												INT nAcqCode, INT idx,
												CStringArray * pArrIgnoreSpecies,
												BOOL bSpeciesSubjectCode
												)
{
	
	//================================================================
	// 변수정의
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strAcqKey, strQuery;
	CStringArray arrMidQuery;

	//수서정보 구분코드변경
	//================================================================
	// 1. 입수구분의 값에 따라 테이블을 설정하는 Head쿼리를 작성한다.
	//================================================================
	if		( nAcqCode == 1 )	strHeadQuery = _T("UPDATE BO_PURCHASEINFO_TBL SET ");
	else if	( nAcqCode == 2 )	strHeadQuery = _T("UPDATE BO_DONINFO_TBL SET ");
	
	//================================================================
	// 2. 수서정보Key값을 가져와 Tail쿼리를 작성한다.
	//================================================================
	strAcqKey = pDM->GetCellData(_T("BP_수서정보KEY"), idx);
	if( strAcqKey.IsEmpty() == TRUE )	return 0;	
	strTailQuery.Format(_T(" WHERE REC_KEY = %s;"), strAcqKey);
	
	//================================================================
	// 3. 주제구분값을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetSubjectCode() == TRUE )
	{
		if( bSpeciesSubjectCode == TRUE )
		{
			pCM->GetControlMgrData(_T("BP_주제구분"), strTmpData);
			strTmpQuery.Format(_T("SUBJECT_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else pArrIgnoreSpecies->Add(_T("BP_주제구분"));

	//================================================================
	// 4. 완성된 쿼리를 AddFrame한다.(수입구분값이 없다면 구입정보/기증테이블 모두 AddFrame한다.)
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	
	if( cnt > 0 )
	{
		if( strHeadQuery.IsEmpty() == FALSE )
		{
			strQuery = strHeadQuery + strMidQuery + strTailQuery;
			pDM->AddFrame(strQuery);
		}
		else
		{
			if( strAcqKey.IsEmpty() == FALSE ) 
			{
				strQuery = _T("UPDATE BO_PURCHASEINFO_TBL SET ") + strMidQuery + strTailQuery;
				pDM->AddFrame(strQuery);
				strQuery = _T("UPDATE BO_DONINFO_TBL SET ") + strMidQuery + strTailQuery; 
				pDM->AddFrame(strQuery);
			}
		}
	}

	return 0;	
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 책테이블에 구분코드를 변경하는 쿼리를 작성하여 추가한다.
//
// [ 미니스펙 ]
// 1. 삭제여부에 따라 책/삭제책테이블을 결정하는 Head쿼리를 작성한다.
// 2. 종과 수서정보와 연결되어 있는지 확인하여 WHERE절을 구성하는 Tail쿼리를 작성한다.
// 3. m_bCatCode에 따라서 Tail쿼리에 작업상태의 범위를 추가한다.
// 4. 등록구분코드를 변경하는 쿼리를 추가한다. (등록번호가 부여되지 않은 자료일때만 가능)
// 5. 별치기호를 변경하는 쿼리를 추가한다.
// 6. 관리구분을 변경하는 쿼리를 추가한다.
// 7. 매체구분을 변경하는 쿼리를 추가한다.
// 8. 이용제한구분을 변경하는 쿼리를 추가한다.
// 9. 이용대상구분을 변경하는 쿼리를 추가한다.
// 10. 분류표구분을 변경하는 쿼리를 추가한다.
// 11. 완성된 쿼리문을 AddFrame한다.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeBookCodeChangeQuery(CESL_DataMgr *pDM, CESL_ControlMgr *pCM, CCodeChangeCondition *pCont, INT idx, BOOL bSpecies, CStringArray * pArrIgnoreSpecies, BOOL bDeleteData/*=FALSE*/)
{
	//================================================================
	// 변수정의
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strVolSortNo, strQuery;
	CStringArray arrMidQuery;

	//책 구분코드변경
	//================================================================
	// 1. 삭제여부에 따라 책/삭제책테이블을 결정하는 Head쿼리를 작성한다.
	//================================================================
	if( bDeleteData == TRUE )	strHeadQuery = _T("UPDATE BO_DELETE_BOOK_TBL SET ");
	else						strHeadQuery = _T("UPDATE BO_BOOK_TBL SET ");

	//================================================================
	// 2. 종과 수서정보와 연결되어 있는지 확인하여 WHERE절을 구성하는 Tail쿼리를 작성한다.
	//================================================================
	if( bSpecies == TRUE )
	{
		strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), idx);
		if( strTmpData.IsEmpty() == TRUE )
		{
			strTailQuery.Format(_T(" WHERE SPECIES_KEY = %s"), pDM->GetCellData(_T("IBS_종KEY"), idx));
		}
		else
		{
			strTailQuery.Format(_T(" WHERE SPECIES_KEY = %s AND ACQ_KEY = %s"), 
								pDM->GetCellData(_T("IBS_종KEY"), idx), strTmpData);
		}
	}
	else
	{
		if( m_bVolYN == TRUE )
		{
			strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), idx);
			if( strTmpData.IsEmpty() == TRUE )	return 0;

			strVolSortNo = pDM->GetCellData(_T("BB_권일련번호"), idx);
			if( strVolSortNo.IsEmpty() == TRUE )	
			{
				strTailQuery.Format(_T(" WHERE ACQ_KEY=%s"), strTmpData);
			}
			else
			{
				strTailQuery.Format(_T(" WHERE ACQ_KEY=%s AND VOL_SORT_NO=%s"), strTmpData, strVolSortNo);
		}
		}
		else
		{
			strTailQuery.Format(_T(" WHERE REC_KEY = %s"), pDM->GetCellData(_T("BB_책KEY"), idx));
	}
	}
	
	//================================================================
	// 3. m_bCatCode에 따라서 Tail쿼리에 작업상태의 범위를 추가한다.
	//================================================================
	if( m_bCatCode == TRUE )	strTailQuery += _T(";");
	else						strTailQuery += _T(" AND (WORKING_STATUS<'BOC' OR WORKING_STATUS>'BOR');");
	

	//================================================================
	// 4. 등록구분코드를 변경하는 쿼리를 추가한다. (등록번호가 부여되지 않은 자료일때만 가능)
	//================================================================
	if( pCont->GetRegCode() == TRUE && pDM->GetCellData(_T("BB_등록번호"), idx).IsEmpty() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_등록구분"), strTmpData);
		strTmpQuery.Format(_T("REG_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_등록구분_코드"), strTmpData, idx);
		//====================================================================
		//2009.01.07 ADD BY LKS : 그리드의 UDF_종등록구분값 갱신되도록 수정
		//(그리드 이벤트에서 IBS_색인등록구분값을 UDF_종등록구분에 SET!!)
		pDM->SetCellData(_T("IBS_색인등록구분"), strTmpData, idx);
		//====================================================================
		arrMidQuery.Add(strTmpQuery);		
		//복본기호부여기준에 등록구분이 설정되어 있다면 복본기호를 재설정해야 한다.
		if( m_cCopyCodeMgr.GetRegCodeCheckYN() == TRUE )	m_bRefreshCopyCode = TRUE;
	}
	else pArrIgnoreSpecies->Add(_T("BB_등록구분"));


	//================================================================
	// 5. 별치기호를 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetShelfCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_별치기호"), strTmpData);
		strTmpQuery.Format(_T("SEPARATE_SHELF_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_별치기호_코드"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
		//복본기호부여기준에 별치기호이 설정되어 있다면 복본기호를 재설정해야 한다.
		if(m_cCopyCodeMgr.GetSeShelfCodeCheckYN()) m_bRefreshCopyCode = TRUE;
	}
	else pArrIgnoreSpecies->Add(_T("BB_별치기호"));


	//================================================================
	// 6. 관리구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetManageCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_관리구분"), strTmpData);
		strTmpQuery.Format(_T("MANAGE_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_관리구분_코드"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_관리구분"));

	//================================================================
	// 7. 매체구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetMediaCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_매체구분"), strTmpData);
		strTmpQuery.Format(_T("MEDIA_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_매체구분_코드"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_매체구분"));


	//================================================================
	// 8. 이용제한구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetUseLimitCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_이용제한구분"), strTmpData);
		strTmpQuery.Format(_T("USE_LIMIT_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_이용제한구분_코드"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_이용제한구분"));


	//================================================================
	// 9. 이용대상구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetUseObjCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_이용대상구분"), strTmpData);
		strTmpQuery.Format(_T("USE_OBJECT_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_이용대상구분_코드"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_이용대상구분"));


	//================================================================
	// 10. 분류표구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetClassNoType() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_분류표구분"), strTmpData);
		strTmpQuery.Format(_T("CLASS_NO_TYPE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_분류표구분"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_분류표구분"));

	//================================================================
	// 11. 완성된 쿼리문을 AddFrame한다.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}

	strQuery = strHeadQuery + strMidQuery + strTailQuery;
	if(cnt > 0)		pDM->AddFrame(strQuery);

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인덱스테이블에 구분코드를 변경하는 쿼리를 작성하여 추가한다.
//
// [ 미니스펙 ]
// 1. 테이블을 정의하는 Head쿼리를 작성한다.
// 2. m_bCatCode에 따라서 WHERE절을 만드는 Tail쿼리를 작성한다.
// 3. 분류표구분을 변경하는 쿼리를 추가한다.
// 4. 업무구분을 변경하는 쿼리를 작성한다. (구입이 아닐때만 가능)
// 5. 자료구분을 변경하는 쿼리를 추가한다.
// 6. 보조등록구분구분을 변경하는 쿼리를 추가한다.
// 7. 등록구분을 변경하는 쿼리를 추가한다.
// 8. 이용제한구분을 변경하는 쿼리를 추가한다.
// 9. 관리구분을 변경하는 쿼리를 추가한다.
// 10. 별치기호 변경하는 쿼리를 추가한다.
// 11. 사용자정의코드를 변경하는 쿼리를 추가한다.
// 12. 이용대상구분을 변경하는 쿼리를 추가한다.(bApplyYN이 TRUE일경우만 가능)
// 13. 매체구분을 변경하는 쿼리를 추가한다.(bApplyYN이 TRUE일경우만 가능)
// 14. 완성된 쿼리문을 작성하여 AddFrame 한다.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeIndexCodeChangeQuery(CESL_DataMgr *pDM, 
												 CESL_ControlMgr *pCM, 
												 CCodeChangeCondition *pCont, 
												 INT idx, 
												 CStringArray * pArrIgnoreSpecies,
												 BOOL bApplyYN)
{
	
	//================================================================
	// 변수정의
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strQuery;
	CStringArray arrMidQuery;

	//색인 구분코드변경
	//================================================================
	// 1. 테이블을 정의하는 Head쿼리를 작성한다.
	//================================================================
	strHeadQuery = _T("UPDATE IDX_BO_TBL SET ");
	
	//================================================================
	// 2. m_bCatCode에 따라서 WHERE절을 만드는 Tail쿼리를 작성한다.
	//================================================================
	if( m_bCatCode == TRUE )
	{
		strTailQuery.Format(_T(" WHERE REC_KEY = %s;"), pDM->GetCellData(_T("IBS_종KEY"), idx));
	}
	else
	{
		strTailQuery.Format(_T(" WHERE REC_KEY = %s AND (WORKING_STATUS<'BOC' OR WORKING_STATUS>'BOR');"), 
							pDM->GetCellData(_T("IBS_종KEY"), idx));
	}

	//================================================================
	// 3. 분류표구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetClassNoType() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_분류표구분"), strTmpData);
		strTmpQuery.Format(_T("CLASS_NO_TYPE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T(""));


	//================================================================
	// 4. 업무구분을 변경하는 쿼리를 작성한다. (구입이 아닐때만 가능)
	//================================================================
	if( pCont->GetWorkCode() == TRUE )
	{
		strTmpData = pDM->GetCellData(_T("BP_수입구분"), idx);
		if(strTmpData != _T("1"))
		{
			pCM->GetControlMgrData(_T("BS_업무구분"), strTmpData);
			strTmpQuery.Format(_T("WORK_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else pArrIgnoreSpecies->Add(_T("IBS_업무구분"));


	//================================================================
	// 5. 자료구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetMatCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_자료구분"), strTmpData);
		strTmpQuery.Format(_T("MAT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_자료구분"));


	//================================================================
	// 6. 보조등록구분구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetSubRegCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_보조등록구분"), strTmpData);
		strTmpQuery.Format(_T("SUB_REG_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_보조등록구분"));


	//================================================================
	// 7. 등록구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetRegCode() == TRUE )
	{
		strTmpQuery.Format(_T("REG_CODE = GET_BO_BOOK_INFO(%s,'REG_CODE')"), pDM->GetCellData(_T("IBS_종KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_등록구분"));


	//================================================================
	// 8. 이용제한구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetUseLimitCode() == TRUE )
	{
		strTmpQuery.Format(_T("USE_LIMIT_CODE = GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE')"), pDM->GetCellData(_T("IBS_종KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_이용제한구분"));


	//================================================================
	// 9. 관리구분을 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetManageCode() == TRUE )
	{
		strTmpQuery.Format(_T("MANAGE_CODE = GET_BO_BOOK_INFO(%s,'MANAGE_CODE')"), pDM->GetCellData(_T("IBS_종KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_관리구분"));


	//================================================================
	// 10. 별치기호 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetShelfCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_별치기호"), strTmpData);
		strTmpQuery.Format(_T("IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), pDM->GetCellData(_T("IBS_종KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
		if( bApplyYN == TRUE )
		{
			strTmpQuery.Format(_T("SE_SHELF_CODE='%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else pArrIgnoreSpecies->Add(_T("IBS_별치기호"));

	//================================================================
	// 11. 사용자정의코드를 변경하는 쿼리를 추가한다.
	//================================================================
	if( pCont->GetUserDefineCode1() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_사용자정의코드1"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE1='%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_사용자정의코드1"));
	if( pCont->GetUserDefineCode2() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_사용자정의코드2"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE2='%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_사용자정의코드2"));


	//================================================================
	// 12. 이용대상구분을 변경하는 쿼리를 추가한다.(bApplyYN이 TRUE일경우만 가능)
	//================================================================
	if( pCont->GetUseObjCode() == TRUE  && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_이용대상구분"), strTmpData);
		pDM->SetCellData(_T("IBS_이용대상구분"), strTmpData, idx);
		strTmpQuery.Format(_T("USE_OBJ_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_이용대상구분"));

	//================================================================
	// 13. 매체구분을 변경하는 쿼리를 추가한다.(bApplyYN이 TRUE일경우만 가능)
	//================================================================
	if( pCont->GetMediaCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_매체구분"), strTmpData);
		pDM->SetCellData(_T("IBS_매체구분"), strTmpData, idx);
		strTmpQuery.Format(_T("MEDIA_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_매체구분"));


	//================================================================
	// 14. 완성된 쿼리문을 작성하여 AddFrame 한다.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}

	if( cnt > 0 )
	{
		strTmpData.Format(_T("LAST_WORK = '%s'"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		strMidQuery += _T(", ") + strTmpData;
		strQuery = strHeadQuery + strMidQuery + strTailQuery;
		pDM->AddFrame(strQuery);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM, _T("IDX_TMP_BO_TBL"), pDM->GetCellData(_T("IBS_종KEY"), idx), _T("I"));
		//=====================================================
		//ECOSEARCH를 위해서 색인잡기
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH )
		{
			strTmpQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), pDM->GetCellData(_T("IBS_종KEY"), idx));
			pDM->AddFrame(strTmpQuery);
		}	
	}
		
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::DonateInfoChange()
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids, i, cnt, nPos;
	CString strDonatorKey, strMsg;
	CArray<INT, INT> idxArray;	idxArray.RemoveAll();
	CESL_DataMgr* pDM		= NULL ;
	CESL_DataMgr* pDM_BOOK	= NULL ;
	CESL_ControlMgr* pCM	= NULL ;
	CESL_Grid* pGrid		= NULL ;
	CDonateInfoChangeCondition* pCont = NULL;
	
	
	
	//================================================================
	// 1. 저장된 포인터를 가져온다.
	//================================================================
	pDM			= (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pDM_BOOK	= (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	pCM			= (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pGrid		= (CESL_Grid*)GetTempPtr(_T("Grid"));
	pCont = (CDonateInfoChangeCondition*)GetTempPtr(_T("Condition"));
	if( pDM == NULL || pCM == NULL || pGrid == NULL || pCont == NULL )	return -1000;	

	//================================================================
	// 2. 저장된 기증자키값을 가져온다.
	//================================================================
	GetTempData(_T("DonatorKey"), strDonatorKey);


	//================================================================
	// 3. 그리드에 선정된 자료만을 기증정보 변경한다.
	//================================================================
	ids = pGrid->SelectMakeList();
	if( ids < 0 )	return -2000;
	i = pGrid->SelectGetHeadPosition();
	if( i < 0 )		return -2000;

	// Progress Ctrl
	ShowDataProcessingDlg();
	strMsg.Format(_T("선정하신 %d건의 기증정보를 변경하고 있습니다."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());

	nPos = 0;
	// Start
	pDM->StartFrame();
	while( i >= 0 )
	{
		// Progress Ctrl ++
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);

		MakeDonateInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, strDonatorKey, i);
		if( pCont->GetRegYn() ) 
		{
			if		( pDM->DB_MGR_ALIAS == _T("DM_기증접수자료관리_종") && pCont->GetNRegYn() == 1 )
			{
				idxArray.Add(i);
			}
			else if	( pDM->DB_MGR_ALIAS == _T("DM_기증부당자료관리_종") && pCont->GetNRegYn() == 0 )
		{
				idxArray.Add(i);
			}
		}
		i = pGrid->SelectGetNext();
	}
	
	CGroupPair api(m_pParentMgr);
	strMsg.Format(_T("기증정보 변경중 입니다."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;

	cnt = idxArray.GetSize();
	for( i=cnt-1 ; i>=0 ; i-- )
	{
		pDM->DeleteRow(idxArray.GetAt(i));
	}
	
	return 0;
}

INT CDataChangeManager::MakeDonateInfoChangeQuery(CESL_DataMgr *pDM, CESL_DataMgr *pDM_BOOK, CESL_ControlMgr *pCM, CDonateInfoChangeCondition *pCont, CString strDonatorKey, INT idx)
{
	//기증정보 변경
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strSpeciesQuery, strMarcQuery;
	CStringArray arrMidQuery, arrIgnoreSpecies;
	CString strDonInputDate;

		
	strSpeciesQuery.Empty();	
	strMarcQuery.Empty();
	arrIgnoreSpecies.RemoveAll();

	strHeadQuery = _T("UPDATE BO_DONINFO_TBL SET ");

	strTailQuery.Format(_T(" WHERE REC_KEY = %s"), pDM->GetCellData(_T("BP_수서정보KEY"), idx));

	if( pCont->GetDonator() )
	{
		pCM->GetControlMgrData(_T("BP_기증자"), strTmpData);
		pDM->SetCellData(_T("BP_기증자명"), strTmpData, idx);
		pDM->SetCellData(_T("BP_기증자정보KEY"), strDonatorKey, idx);
		/**********************************************************
		 **********************************************************/
		//20080417 ADD BY PJW : 기증테이블의 INPUT_DATE가 NULL인 경우 새로 입력해준다
		pDM->GetCellData(_T("BP_입력일"), idx, strDonInputDate);
		if( strDonInputDate.IsEmpty() )
		{
			CTime t = CTime::GetCurrentTime();
			strDonInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
			pDM->SetCellData(_T("BP_입력일"), strDonInputDate, idx);
			pDM_BOOK->SetCellData(_T("BP_입력일"), strDonInputDate, idx);
		}
		/**********************************************************
		 **********************************************************/
		strTmpData.Replace(_T("'"), _T("''"));
		strTmpQuery.Format(_T("DONATOR_NAME = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");
		strTmpQuery.Format(_T("DONATOR_KEY = %s"), strDonatorKey);
		arrMidQuery.Add(strTmpQuery);
		/**********************************************************
		 **********************************************************/
		//20080424 ADD BY PJW : 기증테이블의 기증일자도 함께 UPDATE한다.
		strTmpQuery.Format(_T("INPUT_DATE = '%s'"), strDonInputDate);
		arrMidQuery.Add(strTmpQuery);
		/**********************************************************
		 **********************************************************/
		//===================================================
		//2009.05.20 ADD BY LKS : 기증자의 소속 갱신되도록 수정
		CString sTempQuery,sPositionResult;
		sTempQuery.Format(_T("SELECT POSITION FROM CO_DONATOR_TBL WHERE REC_KEY = %s"),strDonatorKey);
		pDM->GetOneValueQuery(sTempQuery,sPositionResult);
		pDM->SetCellData(_T("DO_소속"), sPositionResult, idx);
		//===================================================
		//===================================================
		//2009.05.28 ADD BY LKS : 기증자의 주소 갱신되도록 수정
		CString sAddressResult;
		sTempQuery.Format(_T("SELECT ADDRESS FROM CO_DONATOR_TBL WHERE REC_KEY = %s"),strDonatorKey);
		pDM->GetOneValueQuery(sTempQuery,sAddressResult);
		pDM->SetCellData(_T("DO_주소"), sAddressResult, idx);
		//===================================================
	}
	else arrIgnoreSpecies.Add(_T("BP_기증자"));

	if(pCont->GetRegYn())
	{
		if(pCont->GetNRegYn() == 0)
		{
			strTmpQuery.Format(_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s' WHERE SPECIES_KEY = %s AND ACQ_KEY = %s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), _T("BOA211O"), pDM->GetCellData(_T("IBS_종KEY"), idx), pDM->GetCellData(_T("BP_수서정보KEY"), idx));
			pDM->AddFrame(strTmpQuery);
			arrIgnoreSpecies.Add(_T("BP_기증부당사유"));
		}
		else if(pCont->GetNRegYn() == 1)
		{
			strTmpQuery.Format(_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s' WHERE SPECIES_KEY = %s AND ACQ_KEY = %s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), _T("BOA212O"), pDM->GetCellData(_T("IBS_종KEY"), idx), pDM->GetCellData(_T("BP_수서정보KEY"), idx));
			pDM->AddFrame(strTmpQuery);
			pCM->GetControlMgrData(_T("BP_기증부당사유"), strTmpData);
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("NO_REG_REASON = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else arrIgnoreSpecies.Add(_T("BP_기증부당사유"));
	if(pCont->GetRemark())
	{
		pCM->GetControlMgrData(_T("BS_비고"), strTmpData);
		if(!strTmpData.IsEmpty())
		{	
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
			strSpeciesQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET REMARK='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));
			pDM->SetCellData(_T("BP_비고"), strTmpData, idx);
			pDM->SetCellData(_T("BP_비고2"), strTmpData, idx);  // 2005-02-04
		}
	}		
	else arrIgnoreSpecies.Add(_T("BS_비고"));
	if(pCont->GetCurrencyCode())
	{
		pCM->GetControlMgrData(_T("BP_화폐구분"), strTmpData);
		strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
		{
			strTmpData.Replace(_T("'"), _T("''"));
			strMarcQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));
			pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
		}
	}
	else arrIgnoreSpecies.Add(_T("BP_화폐구분"));

	strMidQuery.Empty();
	INT cnt = arrMidQuery.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strMidQuery.IsEmpty()) strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	CString strQuery = strHeadQuery + strMidQuery + strTailQuery + _T(" ;");
	if(cnt > 0)
	{
		pDM->AddFrame(strQuery);
		if(!strSpeciesQuery.IsEmpty()) pDM->AddFrame(strSpeciesQuery);
		if(!strMarcQuery.IsEmpty()) pDM->AddFrame(strMarcQuery);
		pFormApi->MakeDMFromCM(pCM, pDM, idx, _T(""), &arrIgnoreSpecies);
		if(pDM_BOOK)
		{
			CMapStringToString tmpMap;	tmpMap.RemoveAll();
			tmpMap.SetAt(_T("IBS_종KEY"), pDM->GetCellData(_T("IBS_종KEY"), i));

			INT nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0);
			while(nBookIdx >= 0)
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}
	}
	return 0;
}

INT CDataChangeManager::PurchaseInfoChange()
{
	CPurchaseInfoChangeCondition * pCont = NULL;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_DataMgr * pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pCont = (CPurchaseInfoChangeCondition*)GetTempPtr(_T("Condition"));
	CString strMsg;

	if(!pDM || !pCM || !pGrid || !pCont) return -1000;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -2000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -2000;

	ShowDataProcessingDlg();
	strMsg.Format(_T("선정하신 %d건의 구입정보를 변경하고 있습니다."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	INT nPos = 0;

	pDM->StartFrame();
	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);
		MakePurchaseInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, i);
		i = pGrid->SelectGetNext();
	}
	
	CGroupPair api(m_pParentMgr);
	strMsg.Format(_T("구입정보 변경중 입니다."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;

	//=====================================================================
	//2009.05.28 ADD BY LKS : 책수가 변경된 경우 할인율 변경작업을 
	//					통해 책수 변경에 따른 가격 업무를 수행한다.
	
	if(pCont->GetBookCount())
	{
		pCont->SetCurrencyCode(0);
		pCont->SetDiscountRate(1);
		pCont->SetExchangeRate(0);
		pCont->SetProvider(0);
		pCont->SetRemark(0);
		pCont->SetBookCount(0);

		INT ids = pGrid->SelectMakeList();
		INT i = pGrid->SelectGetHeadPosition();
		pDM->StartFrame();
		while(i >= 0)
		{
			MakePurchaseInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, i);
			i = pGrid->SelectGetNext();
		}
		ids = pDM->SendFrame();
		if(ids < 0) return -3000;
		ids = pDM->EndFrame();
		if(ids < 0) return -3000;
	}
	//=====================================================================
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 구입정보변경에 해당하는 쿼리를 만들고 DM을 재구성한다.
//
// [ 미니스펙 ]
// 1. HeadQuery와 TailQuery를 설정한다.
// 2. 구입처 체크
// 3.화폐구분 및 환율체크
// 4. 할인율 체크
// 5. 비고체크
// 6. 2~5번까지 체크한 값을 MidQuery로 만든다.
// 7. Head/Mid/Tail Query를 모두 합하여 완성된 쿼리문을 만든다.
// 8. MidQuery가 있다면 완성된 쿼리문을 추가한다.
// 9. 비고체크에 해당하는 쿼리가 있다면 추가한다.
// 10. 화폐구분체크에 해당하는 쿼리가 있다면 추가한다.
// 11. CM으로부터 DM을 재구성한다.
// 12.
// 13. 책수체크
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakePurchaseInfoChangeQuery(CESL_DataMgr *pDM, CESL_DataMgr *pDM_BOOK, CESL_ControlMgr *pCM, CPurchaseInfoChangeCondition *pCont, INT idx)
{
	//================================================================
	// 변수정의
	//================================================================
	INT cnt, i, nBookIdx ;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strSpeciesQuery, strMarcQuery, strQuery, strVolCnt, strBookCnt;
	CString strItemData		= _T("");	// 화폐구분
	CString strExchangeRate = _T("");	// 환율
	CString strDiscountRate = _T("");	// 할인율	
	CStringArray arrIgnoreSpecies, arrMidQuery;	
	CMapStringToString tmpMap;
	arrIgnoreSpecies.RemoveAll();
	strSpeciesQuery.Empty();	
	strMarcQuery.Empty();
	

	//================================================================
	// 1. HeadQuery와 TailQuery를 설정한다.
	//================================================================
	strHeadQuery = _T("UPDATE BO_PURCHASEINFO_TBL SET ");
	strTailQuery.Format(_T(" WHERE REC_KEY = %s ;"), pDM->GetCellData(_T("BP_수서정보KEY"), idx));
	
	//================================================================
	// 2. 구입처 체크
	//================================================================
	if(pCont->GetProvider())
	{
		pCM->GetControlMgrData(_T("BP_구입처부호"), strTmpData);
		strTmpQuery.Format(_T("PROVIDER_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else arrIgnoreSpecies.Add(_T("BP_구입처부호"));


	//================================================================
	// 3.화폐구분 및 환율체크
	//================================================================
		pCM->GetControlMgrData(_T("BP_화폐구분이전"), strItemData );
	if( pCont->GetCurrencyCode() && pDM->GetCellData(_T("BP_화폐구분"), idx) == strItemData )
		{
			pCM->GetControlMgrData(_T("BP_화폐구분"), strTmpData);
			strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);

			strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
			{
				strTmpData.Replace(_T("'"), _T("''"));
				strMarcQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
					strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));
				pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
			}

			// 화폐구분과 환율을 같이 처리한다.
			// 2005-10-12 강봉수
			//
			if( pCont->GetExchangeRate() )
			{
				strTmpQuery.Format(_T("EXCHANGE_RATE = %s"), m_ExchangeRate);
		arrMidQuery.Add(strTmpQuery);
			// 변경된 환율로 설정한다.
			strExchangeRate = m_ExchangeRate;
			if( strExchangeRate.IsEmpty() == TRUE )	strExchangeRate = _T("1");
			pDM->SetCellData(_T("BP_환율"), strExchangeRate, idx);
		}
	}		
	else 
	{
		arrIgnoreSpecies.Add(_T("BP_화폐구분"));
		arrIgnoreSpecies.Add(_T("BP_환율"));
		// 기존환율로 설정한다.
		strExchangeRate = pDM->GetCellData(_T("BP_환율"), idx);		
		if( strExchangeRate.IsEmpty() == TRUE )	strExchangeRate = _T("1");
	}		

	//================================================================
	// 4. 할인율 체크 및 환율과 할인율에 대한 발주가격, 납품가격 적용
	//================================================================
	// 2005-02-02
	if( pCont->GetDiscountRate() || pCont->GetExchangeRate() )
	{
		// 기존 할인율을 가지고 있는다.
		strDiscountRate = pDM->GetCellData(_T("BP_할인율"), idx);
		if( strDiscountRate.IsEmpty() == TRUE )	strDiscountRate = _T("0");
		
		// 변경된 경우에는 CM에서 변경된 값으로 대체한다.
		if( pCont->GetDiscountRate() )
		{
			//이부분 수정해야함..
			pCM->GetControlMgrData(_T("BP_할인율"), strDiscountRate);
			if( strDiscountRate.IsEmpty() == TRUE )	strDiscountRate = _T("0");
			strTmpQuery.Format(_T("PRICE_DISCOUNT_RATE = %s"), strDiscountRate);
			arrMidQuery.Add(strTmpQuery);
		}
		else	arrIgnoreSpecies.Add(_T("BP_할인율"));

		// 발주가격과 납품가격을 Update한다.
		strTmpQuery.Format(_T("ORDER_PRICE = GetSpeciesPrice(%s, %s, NULL, %s)"), pDM->GetCellData(_T("BP_수서정보KEY"), idx), strDiscountRate, strExchangeRate);
		arrMidQuery.Add(strTmpQuery);
		strTmpQuery.Format(_T("DELIVERY_PRICE = GetSpeciesPrice(%s, %s, 'Y', %s)"), pDM->GetCellData(_T("BP_수서정보KEY"), idx), strDiscountRate, strExchangeRate);
		arrMidQuery.Add(strTmpQuery);
	
	}
	else	arrIgnoreSpecies.Add(_T("BP_할인율"));
	
	//================================================================
	// 5. 비고체크
	//================================================================
	if(pCont->GetRemark())
	{
		pCM->GetControlMgrData(_T("BS_비고"), strTmpData);
		strTmpData.Replace(_T("'"), _T("''"));
		strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		strSpeciesQuery.Format( _T("UPDATE BO_SPECIES_TBL SET REMARK = '%s', LAST_WORK='%s' WHERE REC_KEY = %s;"),
			strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));
		//=====================================================
		//2008.11.19 UPDATE BY PJW : BP_비고,BP_비고2가 DM에서 존재하지 않음 -> BS_비고
// 		pDM->SetCellData(_T("BP_비고"), strTmpData, idx);
// 		pDM->SetCellData(_T("BP_비고2"), strTmpData, idx);  // 2005-02-04
		pDM->SetCellData(_T("BS_비고"), strTmpData, idx);
		//=====================================================
		INT nDmRowCnt;
		CString strBookSpeciesKey;
		CString strSpeciesKey;
		nDmRowCnt = pDM_BOOK->GetRowCount();
 		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"),idx);
 		for( INT i = 0 ; i < nDmRowCnt ; i++ )
 		{
			strBookSpeciesKey = pDM_BOOK->GetCellData(_T("IBS_종KEY"),i);
			if( strSpeciesKey == strBookSpeciesKey )
			{
				pDM_BOOK->SetCellData(_T("BS_비고"), strTmpData, i);
			}
			
 		}
	}
	else arrIgnoreSpecies.Add(_T("BS_비고"));

	//================================================================
	// 6. 학처 체크 20070605 ADD PJW : 학처 저장쿼리 
	//================================================================
	if(pCont->GetApplicant())
	{
		pCM->GetControlMgrData(_T("BP_학처명"), strTmpData);
		strTmpQuery.Format(_T("DEPT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else arrIgnoreSpecies.Add(_T("BP_학처명"));
	
	//================================================================
	// 7. 2~6번까지 체크한 값을 MidQuery로 만든다.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE ) strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}


	//================================================================
	// 8. Head/Mid/Tail Query를 모두 합하여 완성된 쿼리문을 만든다.
	//================================================================
	strQuery = strHeadQuery + strMidQuery + strTailQuery;


	//================================================================
	// 9. MidQuery가 있다면 완성된 쿼리문을 추가한다.
	// 10. 비고체크에 해당하는 쿼리가 있다면 추가한다.
	// 11. 화폐구분체크에 해당하는 쿼리가 있다면 추가한다.
	// 12. CM으로부터 DM을 재구성한다.
	// 13.
	//================================================================
	if(cnt > 0)
	{
		//9.
		pDM->AddFrame(strQuery);

		//10.
		if( strSpeciesQuery.IsEmpty() == FALSE )	pDM->AddFrame(strSpeciesQuery);

		//11.
		if( strMarcQuery.IsEmpty() == FALSE )		pDM->AddFrame(strMarcQuery);

		//12.
		pFormApi->MakeDMFromCM(pCM, pDM, idx, _T(""), &arrIgnoreSpecies);
		
		//13.
		if( pDM_BOOK )
		{
			tmpMap.RemoveAll();
			//=====================================================
			//2008.11.19 UPDATE BY PJW : 종DM의 INDEX는 idx이다.
// 			tmpMap.SetAt(_T("IBS_종KEY"), pDM->GetCellData(_T("IBS_종KEY"), i));
			tmpMap.SetAt(_T("IBS_종KEY"), pDM->GetCellData(_T("IBS_종KEY"), idx));
			//=====================================================
			
			nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0) ;
			while( nBookIdx >= 0 )
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}
	}

	//================================================================
	// 14. 책수체크
	// 2005-01-27 송승민
	// 단권본일 경우에만 책수를 변경한다.
	//================================================================
	if( pCont->GetBookCount() )
	{
		strQuery.Format(_T("SELECT COUNT(DISTINCT(VOL_SORT_NO)) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"), pDM->GetCellData(_T("IBS_종KEY"), idx));
		pDM->GetOneValueQuery(strQuery, strVolCnt);
		if(_ttoi(strVolCnt) == 1)
		{
			pCM->GetControlMgrData(_T("BB_책수"), strBookCnt);
			//종Key, 입수구분,차수년도,업무구분,차수,책상태,원하는책수
			strQuery.Format(_T("ECO$P_SET_BOBOOK(%s, '1', '%s', '%s', %s, '%s', %s);"),
							pDM->GetCellData(_T("IBS_종KEY"), idx),
							pDM->GetCellData(_T("BP_수입년도"), idx),
							pDM->GetCellData(_T("IBS_업무구분"), idx),
							pDM->GetCellData(_T("BP_그룹번호"), idx),
							pDM->GetCellData(_T("UDF_자료상태설명"), idx),
							strBookCnt);
			pDM->SetCellData(_T("UDF_책수"), strBookCnt, idx);  // 2005-01-29
//DEL			pDM->SetCellData(_T("BP_발주책수"), strBookCnt, idx);  // 2005-01-29
							
			pDM->AddFrame(strQuery);
		}
	}

	return 0;
}

INT CDataChangeManager::BookCodeChange()
{		
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	CCodeChangeCondition * pCont = (CCodeChangeCondition*)GetTempPtr(_T("Condition"));
	CString strMsg, strTmpData;
	if(!pDM || !pCM || !pCont || !pGrid) return -1000;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -2000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -2000;

	ShowDataProcessingDlg();
	strMsg.Format(_T("선정하신 %d건의 자료를 코드변경하고 있습니다."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	INT nPos = 0;
	
	CStringArray arrIgnoreSpecies;
	arrIgnoreSpecies.RemoveAll();
	
	pDM->StartFrame();
	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);
		strTmpData = pDM->GetCellData(_T("BB_대표책여부"), i);
		if(strTmpData == _T("Y"))
			MakeSpeciesCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, FALSE, FALSE, false);
		MakeBookCodeChangeQuery(pDM, pCM, pCont, i, FALSE, &arrIgnoreSpecies);
		MakeIndexCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, false);
		pFormApi->MakeDMFromCM(pCM, pDM, i, _T(""), &arrIgnoreSpecies);
		
		i = pGrid->SelectGetNext();
	}
	CGroupPair api(m_pParentMgr);
	strMsg.Format(_T("구분코드값을 변경중 입니다."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;

	return 0;	
}

INT CDataChangeManager::GetLatestExchangeRate()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	if(!pDM || !pCM) return -1000;
	
	CString tmpstr, strExchangeRate;
	pCM->GetControlMgrData(_T("BP_화폐구분"), tmpstr);
	if(tmpstr.IsEmpty()) strExchangeRate = _T("");
	else
	{
		/********************************************************************
		 ********************************************************************/
		//20080131 UPDATE BY PJW : 화폐구분이 \이면 APPLY_YN이 N 이더라도 가져 올수 있다.
		CString query;
		CString query1;
		query1.Format(
			_T("SELECT exchange_rate FROM co_exchange_rate_tbl ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%') ")
			// 최근정보 대신에 적용여부를 가지고 값을 가져온다.
			// 2005-09-28 강봉수
			//\
			//"AND input_date = "
			//"(SELECT MAX (input_date) FROM co_exchange_rate_tbl "
			//"WHERE user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%')) "
			_T("AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) ")
			_T("AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
			,
			tmpstr,
			tmpstr
			);
	
		if( tmpstr == _T("\\") )
		{
			query.Format( _T("%s") , query1);
		}
		else
		{
			query.Format( _T("%s AND APPLY_YN = 'Y'") , query1);
		}

		/*
		query.Format(
			_T("SELECT exchange_rate FROM co_exchange_rate_tbl ")
			_T("WHERE user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%') ")
			// 최근정보 대신에 적용여부를 가지고 값을 가져온다.
			// 2005-09-28 강봉수
			//
			//_T("AND input_date = ")
			//_T("(SELECT MAX (input_date) FROM co_exchange_rate_tbl ")
			//_T("WHERE user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%')) ")
			_T("AND APPLY_YN = 'Y' ")
			_T("AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) ")
			_T("AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
			,
			tmpstr,
			tmpstr
			);
		*/
		/********************************************************************
		 ********************************************************************/
		
		pDM->GetOneValueQuery(query, strExchangeRate);
		if(strExchangeRate.IsEmpty())		return -2000;
	}
	SetTempData(_T("ExchangeRate"), strExchangeRate.GetBuffer(0));
	return 0;
}

VOID CDataChangeManager::ShowDataProcessingDlg()
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
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CDataChangeManager::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CDataChangeManager::MakeDM(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;

	INT ids, nCnt, nDMRefreshCnt;
	nCnt = m_MapSpeciesKey.GetCount();
	if(nCnt == 0) return 0;
		
	nDMRefreshCnt = nCnt/800;
	if(nCnt%800 > 0) nDMRefreshCnt++;
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nDMRefreshCnt);

	nCnt = 0;
	CString strKeyList, strKey, strValue, strQuery;
	CESL_DataMgr * pTmpDM = NULL;
	POSITION pos = m_MapSpeciesKey.GetStartPosition();
	while( pos != NULL )
	{
		if(nCnt == 800)
		{
			m_pDataProcessingDlg->GetProgressCtrl()->StepIt();
			pTmpDM = MakeDMClone(pDM);
			if(pTmpDM == NULL) return -1;
			strQuery.Format(_T("SPECIES_KEY IN (%s)"), strKeyList);
			ids = MakeDM(pTmpDM, strQuery);
			if(ids < 0) return -1;
			m_ArrDM.Add(pTmpDM);
			nCnt = 0;
			strKeyList.Empty();
		}
		m_MapSpeciesKey.GetNextAssoc(pos, strKey, strValue);
		if(strKeyList.IsEmpty()) strKeyList = strKey;
		else strKeyList += _T(", ") + strKey;
		nCnt++;
	}

	m_pDataProcessingDlg->GetProgressCtrl()->StepIt();
	pTmpDM = MakeDMClone(pDM);
	if(pTmpDM == NULL) return -1;
	strQuery.Format(_T("SPECIES_KEY IN (%s)"), strKeyList);
	ids = MakeDM(pTmpDM, strQuery);
	if(ids < 0) return -1;
	m_ArrDM.Add(pTmpDM);
	
	m_pDataProcessingDlg->SetMessage(_T("구축된 책정보를 통합 중입니다."));
	
	ids = MergeDMArray(pDM);
	if(ids < 0) return -1;

	return 0;
}

CESL_DataMgr * CDataChangeManager::MakeDMClone(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return NULL;
	CESL_DataMgr * pTmpDM = new CESL_DataMgr;
	if(pTmpDM == NULL) return NULL;
	pTmpDM->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pTmpDM->InitDMPKFromFile(pDM->DB_MGR_ALIAS);
	pTmpDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	return pTmpDM;
}

INT CDataChangeManager::MakeDM(CESL_DataMgr *pDM, CString strQuery)
{
	if(pDM == NULL) return -1;
	if(strQuery.IsEmpty()) return -1;
	INT ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	return 0;
}

INT CDataChangeManager::MergeDMArray(CESL_DataMgr *pMergeDM)
{
	if(pMergeDM == NULL) return -1;
	INT ids;
	INT nCnt = m_ArrDM.GetSize();
	if(nCnt == 0) return 0;
	CESL_DataMgr * pTmpDM = NULL;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pTmpDM = m_ArrDM.GetAt(i);
		if(pTmpDM == NULL) continue;
		ids = AddToDM(pMergeDM, pTmpDM);
	}
	return 0;
}

INT CDataChangeManager::AddToDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM)
{
	if(pTargetDM == NULL || pSourceDM == NULL) return -1;
	INT ids, nCnt, nColCnt, nCurIdx;
	nCnt = pSourceDM->GetRowCount();
	if(nCnt == 0) return 0;
	
	nColCnt = pSourceDM->GetRefColumnCount();
	if(nColCnt == 0) return -1;
	CString field_alias, field_name, field_type, data_type, strData;

	for(INT i=0 ; i<nCnt ; i++)
	{
		ids = pTargetDM->InsertRow(-1);
		if(ids < 0) continue;
		nCurIdx = pTargetDM->GetRowCount()-1;
		for(INT j=0 ; j<nColCnt ; j++)
		{
			ids = pSourceDM->FindColumnFieldName(j, field_alias, field_name, field_type, data_type, i, strData);
			if(ids < 0) continue;
			pTargetDM->SetCellData(field_alias, strData, nCurIdx);
		}
	}
	return 0;
}


INT CDataChangeManager::DeleteDMArray()
{
	INT nCnt = m_ArrDM.GetSize();
	if(nCnt == 0) return 0;
	CESL_DataMgr * pTmpDM = NULL;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pTmpDM = m_ArrDM.GetAt(i);
		if(pTmpDM)
		{
			delete pTmpDM;
			pTmpDM = NULL;
		}
	}
	m_ArrDM.RemoveAll();
	return 0;
}

INT CDataChangeManager::RefreshBookCopyCode(CESL_DataMgr *pDM_OUT, CESL_ControlMgr * pCM, CCodeChangeCondition *pCont)
{
	if(pDM_OUT == NULL || pCM == NULL || pCont == NULL) return -1;
	CESL_DataMgr * pBookDM = m_pParentMgr->FindDM(_T("DM_복본기호갱신용DM"));
	if(pBookDM == NULL) return -1;

	m_pDataProcessingDlg->SetMessage(_T("복본기호를 갱신하기 위해 책정보를 구축중입니다."));
	INT ids = MakeDM(pBookDM);
	if(ids < 0) return -1;

	CString strTmpData;
	INT nCnt = pBookDM->GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(pCont->GetRegCode()) 
		{
			pCM->GetControlMgrData(_T("BB_등록구분"), strTmpData);
			pBookDM->SetCellData(_T("BB_등록구분_코드"), strTmpData, i);
		}
		if(pCont->GetShelfCode())
		{
			pCM->GetControlMgrData(_T("BB_별치기호"), strTmpData);
			pBookDM->SetCellData(_T("BB_별치기호_코드"), strTmpData, i);
		}
	}
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	
	strAlias[nCondColumns] = _T("BB_종KEY");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	pBookDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	

	CString strSpeciesKey;
	INT nStart, nSCnt, nNext;
	nStart = nSCnt = nNext = 0;
	
	m_pDataProcessingDlg->SetMessage(_T("복본기호를 갱신중입니다."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nCnt);

	while(nCnt > nStart)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nStart);
		pBookDM->GetCellData(_T("BB_종KEY"), nStart, strSpeciesKey);
		nSCnt = FindSameSpeciesKeyCnt(pBookDM, &nNext, nCnt, _T("BB_종KEY"), strSpeciesKey);
		ids = RefreshCopyCode(pBookDM, nStart, nStart+nSCnt);
		nStart = nNext;
	}
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.Add(_T("BB_책KEY"));
	for(i=0 ; i<nCnt ; i++)
	{
		strTmpData = pBookDM->GetCellData(_T("UDF_변경YN"), i);
		if(strTmpData != _T("Y")) continue;
		strTmpData = pBookDM->GetCellData(_T("BB_자료상태"), i);
		if(strTmpData.Left(2) == _T("BOC") || strTmpData.Left(2) == _T("BOL")) continue;
		strTmpData = pBookDM->GetCellData(_T("BB_책부록플래그"), i);
		if(strTmpData != _T("B")) continue;
		
		strTmpData = pBookDM->GetCellData(_T("BB_책KEY"), i);
		if(!strTmpData.IsEmpty())
		{
			ids = pBookDM->MakeQueryByDM(RemoveAliasList, pBookDM, pBookDM->TBL_NAME, i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
			if(ids < 0) continue;
		}
	}

	return 0;
}

INT CDataChangeManager::FindSameSpeciesKeyCnt(CESL_DataMgr * pBook_DM, INT *nStart, INT nRowCnt, CString strDivAlias, CString strSpeciesKey)
{
	INT i = *nStart;
	i++;
	INT cnt = 0;
	CString strTmpData;
	while(i < nRowCnt)
	{
		pBook_DM->GetCellData(strDivAlias, i, strTmpData);
		if(strTmpData == strSpeciesKey)
		{
			cnt++;
			i++;
		}
		else break;
	}
	*nStart = i;
	return cnt;
}

INT CDataChangeManager::RefreshCopyCode(CESL_DataMgr *pBookDM, INT nStartIdx, INT nEndIdx)
{
	if(pBookDM == NULL) return -1;
	if(nEndIdx == nStartIdx) return 0;
	
	CString strRegCode, strSeShelfCode, strVol, strTmpData;
	INT nLastCopyCode;
	for(INT i=nStartIdx ; i<=nEndIdx ; i++)
	{
		strRegCode = pBookDM->GetCellData(_T("BB_등록구분_코드"), i);
		strSeShelfCode = pBookDM->GetCellData(_T("BB_별치기호_코드"), i);
		strVol = pBookDM->GetCellData(_T("BB_편권차"), i);
		m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
		nLastCopyCode = pSpeciesApi->GetLastDupCodeNumber(pBookDM, &m_cCopyCodeMgr, i, nStartIdx, nEndIdx, _T("-1"));
		if(nLastCopyCode == 0) strTmpData = _T("");
		else strTmpData.Format(_T("%d"), ++nLastCopyCode);
		pBookDM->SetCellData(_T("BB_복본기호"), strTmpData, i);
		pBookDM->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		pBookDM->SetCellData(_T("UDF_변경YN"), _T("Y"), i);
	}
	return 0;
}

CString CDataChangeManager::SetCurrencyCodeToMarc(CString strMarc, CString strCurrencyCode)
{
	INT ids;
	CString strPriceInfo;
	CMarc marc;
	CMarcInfoConversionMgr mgr;

	if( strMarc.IsEmpty() == TRUE )		return _T("");
	
	mgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if( ids < 0 )	return _T("");

	ids = mgr.GetPriceInfo(&marc, strPriceInfo);
	if( ids < 0 )	return _T("");

	strPriceInfo = strCurrencyCode+strPriceInfo;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("950$b"));
	// 2005.11.07 강봉수 변경
	// 950태그에 값이 없더라도 저장되도록 수정
	//if( ids < 0 )	return _T("");
	ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("950$b"), strPriceInfo);
	if( ids < 0 )	return _T("");
	ids = m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if( ids < 0 )	return _T("");

	return strMarc;
}

// 수서정보변경
INT CDataChangeManager::AcqInfoChange()
{
	INT ids, i, nPos;
	CString strMsg;	

	CESL_DataMgr* pDM		= (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_DataMgr* pDM_BOOK	= (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_ControlMgr* pCM	= (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid* pGrid		= (CESL_Grid*)GetTempPtr(_T("Grid"));
	
	CDonateInfoChangeCondition* pCont = NULL;
	pCont = (CDonateInfoChangeCondition*)GetTempPtr(_T("Condition"));
	
	if( pDM == NULL || pCM == NULL || pGrid == NULL || pCont == NULL )	return -1000;
	
	ids = pGrid->SelectMakeList();
	if(ids < 0) return -2000;
	//i = pGrid->SelectGetHeadPosition();
	i = pGrid->SelectGetTailPosition();
	if(i < 0) return -2000;
	
	ShowDataProcessingDlg();
	if( m_pDataProcessingDlg != NULL )
	{
	strMsg.Format(_T("선정하신 %d건의 수서정보를 변경하고 있습니다."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	}
	
	nPos = 0;	
	pDM->StartFrame();
	while( i >= 0 )
	{
		if( m_pDataProcessingDlg != NULL )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);		
		MakeAcqInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, i);
		
		i = pGrid->SelectGetPrev();
	}
	
	//CGroupPair api(m_pParentMgr);
	if( m_pDataProcessingDlg != NULL )
	{
	strMsg.Format(_T("수서정보 변경중 입니다."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	}
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;
		
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로온 DM과 매핑된 변수 pMap의 알리아스와 데이터가 같을경우 DM의 Row번호를 리턴한다.
//
// [ 미니스펙 ]
// 1. for문은 DM의 Row만큼 탐색한다. -> 세로로 탐색한다.
// 2. while문은 pMap의 알리아스와 데이터를 가로로 탐색한다.
// 3. 값이 틀리다면 다음로우를 검색한다.
// 4. DM에서 pMap 변수의 내용을 모두 찾았다면 DM의 Row번호를 리턴한다.
// 5. DM에서 결국 찾지못했다면 -1을 리턴한다.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::FindIndex(CESL_DataMgr *pDM, CMapStringToString * pMap, INT nIdx/*=0*/)
{
	if( pMap->GetCount() == 0 )			return -1;
	if( nIdx > pDM->GetRowCount()-1 )	return -1;

	//================================================================
	// 변수정의
	//================================================================
	INT nRowCnt , i ;
	CString strTmpData, strAlias, strData;
	BOOL bSameFlag ;

	//================================================================
	// 1. for문은 DM의 Row만큼 탐색한다. -> 세로로 탐색한다.
	// 2. while문은 pMap의 알리아스와 데이터를 가로로 탐색한다.
	// 3. 값이 틀리다면 다음로우를 검색한다.
	// 4. DM에서 pMap 변수의 내용을 모두 찾았다면 DM의 Row번호를 리턴한다.
	// 5. DM에서 결국 찾지못했다면 -1을 리턴한다.
	//================================================================	
	nRowCnt = pDM->GetRowCount();	
	// 1.
	for( i=nIdx ; i<nRowCnt ; i++ )
	{
		bSameFlag = TRUE;
		POSITION pos = pMap->GetStartPosition();
		// 2.
		while( pos != NULL )
		{
			pMap->GetNextAssoc(pos, strAlias, strData);
			strTmpData = pDM->GetCellData(strAlias, i);
			// 3.
			if( strTmpData != strData )	
			{
				bSameFlag = FALSE;
				break;
			}
		}
		
		// 4.
		// 마지막 pos에서 값이 틀릴경우에도 맞다고 체크하고 나가는 것을 방지 하도록 수정
		// 2005.10.26 강봉수 bSameFlag 추가 
		if( pos == NULL && bSameFlag == TRUE )	return i;
	}

	// 5.
	return -1;
}
//20070611 UPDATE PJW : CString strDonatorKey
INT CDataChangeManager::MakeAcqInfoChangeQuery(CESL_DataMgr *pDM, CESL_DataMgr *pDM_BOOK, CESL_ControlMgr *pCM, CDonateInfoChangeCondition *pCont, INT idx)
{
	//수서정보 변경
	CESL_Grid* pGrid		= (CESL_Grid*)GetTempPtr(_T("Grid"));

	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strSpeciesQuery, strMarcQuery, strQuery;	
	CStringArray arrMidQuery, arrIgnoreSpecies;
	CString strDonatorKey;
	
	strSpeciesQuery.Empty();	
	strMarcQuery.Empty();
	arrIgnoreSpecies.RemoveAll();

	// 변경에 사용될 변수	
	CString strAcqKey;
	CString strOldAcqKey;
	CString strSpecieskey;
	CString strAcqYear;
	CString strWorkNo;
	CString strWorkCode;
	CString strSerialNo;
	CString strSubjectCode;
	CString strBookKey;
	CString strProvider;
	CString strDonName;
	CString strAcqCode;

	CString strOneValueQuery;
	CString strInputDate;
	
	// 1. 수입구분 변경일 경우 수입구분을 체크하여 변경하지 않는다.
	GetTempData(_T("BP_수입구분"), strAcqCode);
	if ( strAcqCode == _T("") )
	{
		// 수입구분 체크		
		pDM->GetCellData(_T("BP_수서정보KEY"), idx, strOldAcqKey);
		GetTempData(_T("BP_구입처"), strProvider);	
		
		strTmpData = pDM->GetCellData(_T("BB_수입구분"), idx);
		if(strTmpData == _T("1") && strProvider != _T("") )
		{
			strQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET PROVIDER_CODE='%s' WHERE REC_KEY=%s;"),
				strProvider, strOldAcqKey );
			pDM->AddFrame(strQuery);
		}
		//20070611 ADD PJW : 기증자 검색추가
		// 기증자검색
		// 저장된 기증자키값을 가져온다.	
		else if(strTmpData == _T("2"))
		{
			GetTempData(_T("DonatorKey"), strDonatorKey);
			if( pCont->GetDonator() )
			{
				pCM->GetControlMgrData(_T("BP_기증자"), strDonName);
				strDonName.Replace(_T("'"), _T("''"));	
				strTmpQuery.Format(_T("DONATOR_NAME = '%s'"), strTmpData);
				arrMidQuery.Add(strTmpQuery);
				if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");	
				strTmpQuery.Format(_T("DONATOR_KEY = %s"), strDonatorKey);
				arrMidQuery.Add(strTmpQuery);
				
				/**********************************************************
				 **********************************************************/
				//20080417 UPDATE BY PJW : 기증테이블의 INPUT_DATE가 NULL인 경우 새로 입력해준다.
				//strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET DONATOR_KEY='%s', DONATOR_NAME='%s' WHERE REC_KEY=%s;"),
				//	strDonatorKey, strDonName, strOldAcqKey );
				strOneValueQuery.Format(_T("SELECT INPUT_DATE FROM BO_DONINFO_TBL %s"), strTailQuery);
				pDM->GetOneValueQuery(strOneValueQuery,strInputDate);
				if( strInputDate.IsEmpty() )
				{
					CTime t = CTime::GetCurrentTime();
					strInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
					strTmpQuery.Format(_T("INPUT_DATE = '%s'"), strInputDate);
					arrMidQuery.Add(strTmpQuery);
					
				}
				else
				{
					strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET DONATOR_KEY='%s', DONATOR_NAME='%s' WHERE REC_KEY=%s;"),
						strDonatorKey, strDonName, strOldAcqKey );
				}
				/**********************************************************
				 **********************************************************/
				
				
				pDM->AddFrame(strQuery);
			}
		}
		else 
		{
			strHeadQuery.Empty();
			strTailQuery.Empty();
		}	
	}
	else
	{	
		pDM->MakeRecKey(strAcqKey);
		pDM->GetCellData(_T("BP_수서정보KEY"), idx, strOldAcqKey);
		pDM->GetCellData(_T("IBS_종KEY"), idx, strSpecieskey);
		pDM->GetCellData(_T("BS_주제구분"), idx, strSubjectCode);
		pDM->GetCellData(_T("BB_책KEY"), idx, strBookKey);		
		GetTempData(_T("BP_수입년도"), strAcqYear);
		
		// 구입정보 생성
		if ( strAcqCode == _T("1") )
		{
			GetTempData(_T("BP_구입처"), strProvider);	
			GetTempData(_T("BS_업무구분"), strWorkCode);
			GetTempData(_T("BP_그룹번호"), strSerialNo);
			GetTempData(_T("BP_작업번호"), strWorkNo);
			// 비치희망과 연결된 정보가 있으면 업데이트
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET ACQ_KEY=%s WHERE REC_KEY=%s;"), strAcqKey, strOldAcqKey );
			pDM->AddFrame(strQuery);
			// 과거 수서정보 삭제
			strQuery.Format(_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE REC_KEY=%s;"), strOldAcqKey );
			pDM->AddFrame(strQuery);			
			// 수서정보 생성
			strQuery.Format(_T("INSERT INTO BO_PURCHASEINFO_TBL ( REC_KEY, SPECIES_KEY, ORDER_YEAR, ORDER_SEQ_NO, ORDER_WORK_NO, SUBJECT_CODE, PROVIDER_CODE, INPUT_DATE, INPUT_TYPE, MANAGE_CODE ) VALUES ( %s, %s, '%s', '%s', '%s', '%s', '%s', SYSDATE, '0', UDF_MANAGE_CODE );"),
//DEL 			strQuery.Format(_T("INSERT INTO BO_PURCHASEINFO_TBL ( REC_KEY, SPECIES_KEY, ORDER_YEAR, ORDER_SEQ_NO, ORDER_WORK_NO, SUBJECT_CODE, PROVIDER_CODE, INPUT_DATE, INPUT_TYPE ) VALUES ( %s, %s, '%s', '%s', '%s', '%s', '%s', SYSDATE, '0' );"),
				strAcqKey, strSpecieskey, strAcqYear, strSerialNo, strWorkNo, strSubjectCode, strProvider );
			pDM->AddFrame(strQuery);
			// 책정보 수입구분, 수서정보 업데이트
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='1', ACQ_KEY=%s, SEQ_NO=%s WHERE REC_KEY=%s;"), strAcqKey, strSerialNo, strBookKey );
			pDM->AddFrame(strQuery);
			//======================================================
			//20080624 ADD BY PJW : 구입처가 바뀌면 업무구분도 바꿔준다.
			strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET WORK_CODE='%s' WHERE REC_KEY=%s;"), strWorkCode, strSpecieskey );
			pDM->AddFrame(strQuery);
			strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORK_CODE='%s' WHERE REC_KEY=%s;"), strWorkCode, strSpecieskey );
			pDM->AddFrame(strQuery);
			//======================================================
			pDM->DeleteRow(idx);
			//=================================================
			//20080612 UPDATE BY PJW : DM이 하나도 없을경우 그리드 ROW에러를 방지하기 위해서 그리드를 DISPLY한다.
			if( 1 < pDM->GetRowCount() )
			{
				pGrid->RemoveAt(idx+1);
			}
			else
			{
				pGrid->Display();
			}
			//=================================================			
		}
		else if ( strAcqCode == _T("2") )
		{				
			GetTempData(_T("DonatorKey"), strDonatorKey);
			GetTempData(_T("BP_그룹번호"), strSerialNo);

			// 비치희망과 연결된 정보가 있으면 업데이트
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET ACQ_KEY=%s WHERE REC_KEY=%s;"), strAcqKey, strOldAcqKey );
			pDM->AddFrame(strQuery);
			
			//========================기증 to 기증========================================================
			CString strTemp,strOldDonatorKey,strOldDonatorName;
			strTemp.Format(_T("SELECT DONATOR_KEY FROM BO_DONINFO_TBL WHERE REC_KEY=%s"),strOldAcqKey);
			pDM->GetOneValueQuery(strTemp, strOldDonatorKey);
			if(!strOldDonatorKey.IsEmpty())
			{
				strTemp.Format(_T("SELECT DONATOR_NAME FROM BO_DONINFO_TBL WHERE REC_KEY=%s"),strOldAcqKey);
				pDM->GetOneValueQuery(strTemp, strOldDonatorName);
			}
			///////////////////////////////////////////////////////////////////////////////////////////////

			// 과거 수서정보 삭제
			strQuery.Format(_T("DELETE FROM BO_DONINFO_TBL WHERE REC_KEY=%s;"), strOldAcqKey );
			pDM->AddFrame(strQuery);			

			strQuery = _T("INSERT INTO BO_DONINFO_TBL ( REC_KEY, SPECIES_KEY, ACQ_YEAR, RECEIPT_NO, SUBJECT_CODE, INPUT_DATE, INPUT_TYPE");
			// 기증정보 생성
			if( pCont->GetDonator() || (!strOldDonatorKey.IsEmpty()))
			//if( pCont->GetDonator())
			{
				if(!strOldDonatorKey.IsEmpty())
				{
					strDonName = strOldDonatorName;
					strDonName.Replace(_T("'"), _T("''"));		
					strDonatorKey = strOldDonatorKey;
				}
				else
				{
					pCM->GetControlMgrData(_T("BP_기증자"), strDonName);
					strDonName.Replace(_T("'"), _T("''"));				
					
					if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");		
				}
					/*pCM->GetControlMgrData(_T("BP_기증자"), strDonName);
					strDonName.Replace(_T("'"), _T("''"));				
					
					if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");		*/

				strTmpQuery.Format(_T(", DONATOR_KEY, DONATOR_NAME , MANAGE_CODE) VALUES ( %s, %s, '%s', '%s', '%s', SYSDATE, '0', %s, '%s', UDF_MANAGE_CODE );"),
					strAcqKey, strSpecieskey, strAcqYear, strSerialNo, strSubjectCode, strDonatorKey, strDonName );
				strQuery += strTmpQuery;
				pDM->AddFrame(strQuery);

			}			
			else
			{
				strTmpQuery.Format(_T(",MANAGE_CODE) VALUES ( %s, %s, '%s', '%s', '%s', SYSDATE, '0' ,UDF_MANAGE_CODE);"),
					strAcqKey, strSpecieskey, strAcqYear, strSerialNo, strSubjectCode );
				strQuery += strTmpQuery;
				pDM->AddFrame(strQuery);
			}		
			// 책정보 업데이트
			// 17/01/18 김혜영 : 단행>등록>등록자료관리의 수서정보에서 수입구분 및 기증자 변경시 오류 수정
/*//BEFORE-----------------------------------------------------------------------------
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='2', ACQ_KEY=%s, SEQ_NO=%s WHERE REC_KEY=%s;"), strAcqKey, strSerialNo, strBookKey );
*///AFTER------------------------------------------------------------------------------
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='2', ACQ_KEY=%s, SEQ_NO='%s' WHERE REC_KEY=%s;"), strAcqKey, strSerialNo, strBookKey );
/*//END-------------------------------------------------------------------------------*/			
			pDM->AddFrame(strQuery);			
			pDM->DeleteRow(idx);
			
			//20090327 UPDATE BY LKS : DM이 하나도 없을경우 그리드 ROW에러를 방지하기 위해서 그리드를 DISPLY한다.
			if( 1 < pDM->GetRowCount() )
			{
				pGrid->RemoveAt(idx+1);
			}
			else
			{
				pGrid->Display();
			}
		}
		//===================================================
		//2009.10.20 ADD BY PJW : 구입,기증이외의 수입구분일경우의 처리 추가함
		else
		{				
			GetTempData(_T("BP_그룹번호"), strSerialNo);

			// 비치희망과 연결된 정보가 있으면 업데이트
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET ACQ_KEY=%s WHERE ACQ_KEY=%s;"), strAcqKey, strOldAcqKey );
			pDM->AddFrame(strQuery);

			// 과거 수서정보 삭제
//			strQuery.Format(_T("DELETE FROM BO_DONINFO_TBL WHERE REC_KEY=%s;"), strOldAcqKey );
//			pDM->AddFrame(strQuery);				
			// 책정보 업데이트
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='%s', ACQ_KEY=%s, SEQ_NO=%s WHERE REC_KEY=%s;"), strAcqCode, strAcqKey, strSerialNo, strBookKey );
			pDM->AddFrame(strQuery);			
			pDM->DeleteRow(idx);
			
			//20090327 UPDATE BY LKS : DM이 하나도 없을경우 그리드 ROW에러를 방지하기 위해서 그리드를 DISPLY한다.
			if( 1 < pDM->GetRowCount() )
			{
				pGrid->RemoveAt(idx+1);
			}
			else
			{
				pGrid->Display();
			}
		}
		//===================================================
	}

	// 비고 체크
	if( pCont->GetRemark() )
	{
		pCM->GetControlMgrData(_T("BS_비고"), strTmpData);
		if( strTmpData.IsEmpty() == FALSE )
		{	
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
			strSpeciesQuery.Format(_T("UPDATE BO_SPECIES_TBL SET REMARK='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));
			//=============================================
			//2008.07.25 ADD BY PJW : 비고를 종DM에 추가한다.
			pDM->SetCellData(_T("BS_비고"), strTmpData, idx);
			//=============================================
			pDM->SetCellData(_T("BP_비고"), strTmpData, idx);
			pDM->SetCellData(_T("BP_비고2"), strTmpData, idx);  // 2005-02-04
			pDM->AddFrame(strSpeciesQuery);
		}
	}			

	// 화폐구분 체크
	if( pCont->GetCurrencyCode() )
	{
		pCM->GetControlMgrData(_T("BP_화폐구분"), strTmpData);
		
		//===================================================
		//2009.03.24 ADD BY LKS : 수정창에 화폐구분이 반영안되어 추가
		CString strTmpQuery,strAcqKey;
		pDM->GetCellData(_T("BP_수서정보KEY"), idx, strAcqKey);
		strTmpQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET CURRENCY_CODE='%s' WHERE REC_KEY=%s;"),strTmpData,strAcqKey);
		//반입자료관리에서는 BP_수서정보KEY 가 없다.
		if(!strAcqKey.IsEmpty())
			pDM->AddFrame(strTmpQuery);
		//===================================================

		strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);		
		strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
		{
			strTmpData.Replace(_T("'"), _T("''"));
			strMarcQuery.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));
			
			pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
			pDM->AddFrame(strMarcQuery);
		}
	}


	//
	// =======================================================================
	// 

	//20070611 ADD PJW : 기증자 검색추가
	// 기증자검색
	// 저장된 기증자키값을 가져온다.
		/*
	if(strTmpData == _T("2"))
	{
		GetTempData(_T("DonatorKey"), strDonatorKey);
		if( pCont->GetDonator() )
		{
			pCM->GetControlMgrData(_T("BP_기증자"), strTmpData);
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("DONATOR_NAME = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
			if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");
			strTmpQuery.Format(_T("DONATOR_KEY = %s"), strDonatorKey);
			arrMidQuery.Add(strTmpQuery);
		}
		else arrIgnoreSpecies.Add(_T("BP_기증자"));
	}
	
	// 비고 체크
	if( pCont->GetRemark() )
	{
		pCM->GetControlMgrData(_T("BS_비고"), strTmpData);
		if( strTmpData.IsEmpty() == FALSE )
		{	
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);

			strSpeciesQuery.Format(_T("UPDATE BO_SPECIES_TBL SET REMARK='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));

			pDM->SetCellData(_T("BP_비고"), strTmpData, idx);
			pDM->SetCellData(_T("BP_비고2"), strTmpData, idx);  // 2005-02-04
		}
	}		
	else arrIgnoreSpecies.Add(_T("BS_비고"));

	// 화폐구분 체크
	if( pCont->GetCurrencyCode() )
	{
		pCM->GetControlMgrData(_T("BP_화폐구분"), strTmpData);
		strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
		{
		strTmpData.Replace(_T("'"), _T("''"));
			strMarcQuery.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
			strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_종KEY"), idx));

		pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
	}
	}
	else arrIgnoreSpecies.Add(_T("BP_화폐구분"));


	
	// Mid쿼리문  추가
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	

	// 완성된 쿼리문 작성
	if( strHeadQuery.IsEmpty() == FALSE )	strQuery = strHeadQuery + strMidQuery + strTailQuery + _T(" ;");
	else strQuery.Empty();
	
	if( cnt > 0 )
	{
		if( strQuery.IsEmpty() == FALSE )			pDM->AddFrame(strQuery);
		if( strSpeciesQuery.IsEmpty() == FALSE )	pDM->AddFrame(strSpeciesQuery);
		if( strMarcQuery.IsEmpty() == FALSE )		pDM->AddFrame(strMarcQuery);

		pFormApi->MakeDMFromCM(pCM, pDM, idx, _T(""), &arrIgnoreSpecies);
		if( pDM_BOOK )
		{
			CMapStringToString tmpMap;	tmpMap.RemoveAll();
			tmpMap.SetAt(_T("IBS_종KEY"), pDM->GetCellData(_T("IBS_종KEY"), i));
			
			INT nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0);
			while(nBookIdx >= 0)
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}
	}
	*/

	return 0;
}
