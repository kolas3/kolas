// IndexDataMgr.cpp: implementation of the CIndexDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndexDataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("단행종API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexDataMgr::CIndexDataMgr(CESL_Mgr* pParent /*=NULL*/): CESL_Mgr(IDD, pParent)
{
	m_pParentMgr = pParent;
	m_pIndexDM = NULL;
	m_bDMCreate = FALSE;


	nIndexSpeciesRelation = 0;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_형식구분_코드");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_업무구분_코드");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_보조등록구분_코드");	nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_제어자료구분_코드");	nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_분류표구분");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_자료구분_코드");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_INDEX[nIndexSpeciesRelation] = _T("IBS_비고");					nIndexSpeciesRelation++;
	
}

CIndexDataMgr::~CIndexDataMgr()
{
	if (m_pIndexDM && m_bDMCreate) 
	{
		delete m_pIndexDM;
		m_pIndexDM = NULL;
	}
}

INT CIndexDataMgr::ExtractIndexInMarc(CString strMarc, CStringArray &strAliasArray)
{
	INT nRowIdx, ids, i, nIndexToolKit;
	CString strData, strIdxTitle, strIdxKeyword, strIdxAuthor, strIdxPublisher, strIdxAllItem =_T("");
	CString IndexToolKit[16], IndexToolKitCommand[16];
	CStringArray TargetArray;
	CMarc Marc;

	strAliasArray.RemoveAll();
	m_MarcDataArray.RemoveAll();
	pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &Marc);		
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, &Marc);
	
	nIndexToolKit = 0;	
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용본표제");			IndexToolKitCommand[nIndexToolKit] = _T("본표제");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용키워드");			IndexToolKitCommand[nIndexToolKit] = _T("키워드파일");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용저작자");			IndexToolKitCommand[nIndexToolKit] = _T("저작자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용발행자");			IndexToolKitCommand[nIndexToolKit] = _T("발행자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_표제사항");				IndexToolKitCommand[nIndexToolKit] = _T("표제사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_저작자사항");				IndexToolKitCommand[nIndexToolKit] = _T("저작자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_발행자사항");				IndexToolKitCommand[nIndexToolKit] = _T("발행자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_발행년도사항");			IndexToolKitCommand[nIndexToolKit] = _T("발행년도사항");		nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용정렬용본표제");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용본표제");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용범위검색발행자");	IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행자");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_범위검색용발행년도");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행년도");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_판종색인");				IndexToolKitCommand[nIndexToolKit] = _T("판종색인");			nIndexToolKit++;	
	
	m_pIndexDM->FreeData();

	ids = InsertRow(-1);
	if( ids < 0 )	return ids;
	
	nRowIdx = m_pIndexDM->GetRowCount()-1;
	for( i=0; i<nIndexToolKit; i++ ) 
	{
		strData.Empty();
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(IndexToolKitCommand[i], strData);
		m_pIndexDM->SetCellData(IndexToolKit[i], strData, nRowIdx);
		strAliasArray.Add(IndexToolKit[i]);
		m_MarcDataArray.Add(strData);
	}	


	m_pIndexDM->GetCellData(_T("IBS_검색용본표제"), nRowIdx, strIdxTitle);
	m_pIndexDM->GetCellData(_T("IBS_검색용저작자"), nRowIdx, strIdxAuthor);
	m_pIndexDM->GetCellData(_T("IBS_검색용발행자"), nRowIdx, strIdxPublisher);
	m_pIndexDM->GetCellData(_T("IBS_검색용키워드"), nRowIdx, strIdxKeyword);

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_저작자"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE ) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&Marc, _T("245$d"), strData);
		DeleteUnlessWord(strData);
		m_pIndexDM->SetCellData(_T("IBS_저작자"), strData, nRowIdx);		
		strAliasArray.Add(_T("IBS_저작자"));
		m_MarcDataArray.Add(strData);
	}

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_본표제"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE )
	{
		//===================================================
		//2010.06.23 UPDATE BY PJW : IDX본표제가 아닌 표제사항을 추가한다.
//		m_pIndexDM->SetCellData(_T("IBS_본표제"), strIdxTitle, nRowIdx);
//		m_pIndexDM->GetCellData(_T("IBS_표제사항"), nRowIdx, strData);
		CString str245$x = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&Marc, _T("245$a"), strData);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&Marc, _T("245$x"), str245$x);
		if( !str245$x.IsEmpty() )
		{
			strData = strData + _T("=") + str245$x;
		}
		
		m_pIndexDM->SetCellData(_T("IBS_본표제"), strData, nRowIdx);
		//===================================================		
		strAliasArray.Add(_T("IBS_본표제"));
		m_MarcDataArray.Add(strIdxTitle);
	}

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_발행자"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE )
	{
		m_pIndexDM->SetCellData(_T("IBS_발행자"), strIdxPublisher, nRowIdx);
		strAliasArray.Add(_T("IBS_발행자"));
		m_MarcDataArray.Add(strIdxPublisher);
	}

	strData.Empty();
	m_pIndexDM->GetCellData(_T("IBS_발행년"), nRowIdx, strData);
	if( strData.IsEmpty() == TRUE )
	{
		m_pIndexDM->GetCellData(_T("IBS_발행년도사항"), nRowIdx, strData);
		m_pIndexDM->SetCellData(_T("IBS_발행년"), strData, nRowIdx);
		strAliasArray.Add(_T("IBS_발행년"));
		m_MarcDataArray.Add(strData);
	}

	
	// IDX_ALL_ITEM필드 설정 - 검색용본표제, 검색용저자, 검색용발행자, 검색용키워드
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	strIdxAllItem.Empty();
	for( i=0 ; i<TargetArray.GetSize(); i++ )
	{
		if( strIdxAllItem.IsEmpty() )	strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	m_pIndexDM->SetCellData(_T("IBS_전체검색용색인어"), strIdxAllItem, nRowIdx);
	strAliasArray.Add(_T("IBS_전체검색용색인어"));
	m_MarcDataArray.Add(strIdxAllItem);
	
	//DB 쿼리 문 생성 함수 추가.
	//RestructIndex(&m_AliasArray);
	return 0;
}

INT CIndexDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pIndexDM = new CESL_DataMgr;
		if(m_pIndexDM == NULL) return -1;
		m_pIndexDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pIndexDM->InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
		m_pIndexDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pIndexDM = pDM;
	}
	return 0;
}

INT CIndexDataMgr::SetRecKey(CString strRecKey, INT nRowIdx)
{
	INT ids = -1;
	ids = m_pIndexDM->SetCellData(_T("IBS_REC_KEY"), strRecKey, nRowIdx);
	if(ids < 0) return -1;

	return 0;
}
INT CIndexDataMgr::INDEX_to_SPECIES(CStringArray &strAlias, INT nRowIdx)
{
	INT ids;
	CString strData;

	for (INT i = 0; i < nIndexSpeciesRelation; i++) {
		strData = IndexSpeciesRelation_INDEX[i];
		ids = m_pIndexDM->GetCellData(strData, nRowIdx, strAlias[i]);
		if(ids < 0) return -1;
	}
	return 0;
}

INT CIndexDataMgr::InsertRow(INT nIdx)
{
	if(m_pIndexDM == NULL) return -1;
	return m_pIndexDM->InsertRow(nIdx);
}

INT CIndexDataMgr::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	INT pos;
	CString strTmpData;

	if( strSource.GetLength() == 0 )	return 0;

	strSource.TrimLeft();	strSource.TrimRight();
	pos = strSource.Find(DelimeterStr, 0);	
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		strSource.TrimLeft();	strSource.TrimRight();

		if( bIndexing )		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);

		if( !bIndexing || !CheckIsHave(pTargetArray, strTmpData) )		pTargetArray->Add(strTmpData);

		pos = strSource.Find(DelimeterStr, 0);
	}

	if( bIndexing )		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if( !bIndexing || !CheckIsHave(pTargetArray, strSource) )		pTargetArray->Add(strSource);

	return 0;
}

BOOL CIndexDataMgr::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

INT CIndexDataMgr::RestructIndexInMarc(CStringArray *pMarcAliasArray)
{
	INT nIds, i, nColIndex;	
	CString strColName, strAlias, strData;
	CString strFieldAlias, strFieldName, strType, strDataType;

/*	nIds = m_pIndexDM->StartFrame();
	if(nIds < 0) return -1;

	m_pIndexDM->InitDBFieldData();*/

	for( i=0; i<pMarcAliasArray->GetSize(); i++ )
	{
		strAlias = pMarcAliasArray->GetAt(i);
		nColIndex = m_pIndexDM->FindColumn(strAlias);
		if( nColIndex == -1 )	return -1;

		nIds = m_pIndexDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
		if( nIds < 0 )			continue;
		//============================================
		//20080611 UPDATE BY PJW : 소문자대문자들어갈수 있으므로 소문자로 바꾸도록 통일한다.
		//strType.MakeUpper();	strDataType.MakeUpper();
		strType.MakeLower(); strDataType.MakeLower();
		//============================================
		
		if( strDataType == _T("udf") )		continue;

		if		( strType == _T("str") )	
		{
			m_pIndexDM->AddDBFieldData(strFieldName, _T("STRING"), strData);
		}
		else if	( strType == _T("int") ) 
		{
			if( strData.IsEmpty() == TRUE )		m_pIndexDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T("NULL"));
			else								m_pIndexDM->AddDBFieldData(strFieldName, _T("NUMERIC"), strData);
		}
		
	}
	return 0;
}

INT CIndexDataMgr::RestructIndexInSpecies(CString strRecKey, BOOL bDelete/*=FALSE*/)
{
	INT ids, i;
	CString strTbl, strQuery = _T("");
	CString strCodes[7] = {	_T("WORK_CODE"),
							_T("MAT_CODE"),
							_T("FORM_CODE"),
							_T("MEDIA_CODE"),
							_T("USE_OBJ_CODE"),
							_T("SUB_REG_CODE"),
							_T("CLASS_NO_TYPE")	};
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pIndexDM->CONNECTION_INFO);

	if( strRecKey.IsEmpty() == TRUE )	return -1;

	if( bDelete == TRUE )	
	{
		strTbl = _T("BO_DELETE_SPECIES_TBL");
		
		// 삭제일을 추가한다.
		CString strAddQuery = _T("");
		CString strResult	= _T("");
		strAddQuery.Format(_T("SELECT S.DELETE_DATE ")
						   _T("FROM BO_DELETE_SPECIES_TBL S ")
						   _T("WHERE S.REC_KEY = %s"), strRecKey);		
		m_pIndexDM->GetOneValueQuery(strAddQuery, strResult);
		m_pIndexDM->AddDBFieldData(_T("DELETE_DATE"), _T("STRING"), strResult);
		
	}
	else
	{
		strTbl = _T("BO_SPECIES_TBL");	
	}
	
	
	strQuery.Format(_T("SELECT WORK_CODE, MAT_CODE, FORM_CODE, MEDIA_CODE, USE_OBJ_CODE, SUB_REG_CODE, CLASS_NO_TYPE ")
					_T("FROM %s ")
					_T("WHERE REC_KEY = %s"), strTbl, strRecKey);
	ids = tmpDM.RestructDataManager(strQuery);
	if( ids < 0 )	return ids;

	for( i=0; i<7 ; i++ )
	{
		CString strData = _T("");
		tmpDM.GetCellData(0,i,strData);
		m_pIndexDM->AddDBFieldData(strCodes[i], _T("STRING"), strData);
	}

	return 0;	
}


INT CIndexDataMgr::RestructIndexInBook(CString strSpeciesKey)
{
	CString strData;

	//RestructIndexInBook
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'REG_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("REG_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'MANAGE_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MANAGE_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL_TITLE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("PLACE_INFO"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_SE_SHELF_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_BOOK_INFO(%s,'EA_ISBN')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("ST_CODE"), _T("NUMERIC"), strData);
	//IDX_HOLD_PLACE, MAIN_PLACE_INFO 추가
	strData.Format(_T("GET_DISP_BO_BOOK_INFO(%s, 'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_HOLD_PLACE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MAIN_PLACE_INFO"), _T("NUMERIC"), strData);

	return 0;
}

INT CIndexDataMgr::RestructIndexInDeleteBook(CString strSpeciesKey)
{
	CString strData;

	//RestructIndexInBook
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'REG_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("REG_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'USE_LIMIT_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("USE_LIMIT_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'MANAGE_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MANAGE_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'VOL_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'VOL_TITLE_INDEX')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_VOL_TITLE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("PLACE_INFO"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_SE_SHELF_CODE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_BOOK_INFO(%s,'EA_ISBN')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("ST_CODE"), _T("NUMERIC"), strData);
	//IDX_HOLD_PLACE, MAIN_PLACE_INFO 추가
	strData.Format(_T("GET_DISP_BO_DELETE_BOOK_INFO(%s, 'SHELF_LOC_CODE')"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("IDX_HOLD_PLACE"), _T("NUMERIC"), strData);
	strData.Format(_T("GET_BO_DELETE_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	m_pIndexDM->AddDBFieldData(_T("MAIN_PLACE_INFO"), _T("NUMERIC"), strData);

	return 0;
}

INT CIndexDataMgr::MakeInsert(CString strSpeciesKey)
{
	m_pIndexDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	INT nIds = m_pIndexDM->MakeInsertFrame(m_pIndexDM->TBL_NAME);
	if(nIds < 0) return nIds;

	return 0;
}


INT CIndexDataMgr::MakeUpdate(CString strSpeciesKey)
{
	INT nIds = m_pIndexDM->MakeUpdateFrame(m_pIndexDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey, 0);
	if(nIds < 0) return nIds;

	return 0;
}

INT CIndexDataMgr::SyncIndexWithSearchEngine(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	CString strQuery;
// 	strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL ")
// 		_T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
// 		_T("FROM IDX_BO_TBL ")
// 					_T("WHERE REC_KEY=%s);"), strSpeciesKey);
// 	
// 	INT ids = pDM_OUT->AddFrame(strQuery);
// 	if(ids < 0)
// 	{
// 		ESLAfxMessageBox(_T("검색엔진과의 동기화를 위한 쿼리 실행 실패"));
// 		return -1;
// 	}
	//=====================================================

	return 0;
}

INT CIndexDataMgr::SyncIndexWithECO_SEARCH(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	CString strQuery;
	strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
	
	INT ids = pDM_OUT->AddFrame(strQuery);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("Oracle8i용 ECO_SEARCH와의 동기화를 위한 쿼리 실행 실패"));
		return -1;
	}
	
	return 0;
}

INT CIndexDataMgr::UpdateParentDM(CStringArray &strAliasArray, CESL_DataMgr * pParentDM, INT nParentDMIndex, CString strParentParam)
{
	CString strAlias;
	CString strData, strParentData;
	CString strFieldAlias, strFieldName, strType, strDataType;
	CString strParentAlias, strParentName, strParentType, strParentDataType;
	INT nColIndex = -1;
	INT nColCount = strAliasArray.GetSize();
	INT nFindParentFeild = -1;
	
	for(INT i=0; i < nColCount; i++)
	{
		strAlias = strAliasArray.GetAt(i);
		nColIndex = m_pIndexDM->FindColumn(strAlias);
		if( nColIndex < 0 )
			return -1;
		INT nIds = m_pIndexDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
		if(nIds < 0) continue;
		
		INT nParentColCount = pParentDM->GetColCount();
		for(INT j=0; j < nParentColCount; j++)
		{
			INT nParentIds = pParentDM->FindColumnFieldName(j, strParentAlias, strParentName, strParentType, strParentDataType, 0, strParentData);
			if(nParentIds < 0) continue;
			INT IsIndexParam = strParentName.Find(strParentParam+_T("."));
			if(IsIndexParam < 0) continue;
			CString strIndexName = strParentName.Replace(strParentParam+_T("."), _T(""));
			if(strFieldName == strParentName)
			{
				pParentDM->SetCellData(strParentAlias, strData, nParentDMIndex);
				break;
			}
			//같은 이름의 field_Name을 찾지 못한 경우는 HOW??
		}
		
	}
	
	return 0;

}

INT CIndexDataMgr::SetDefaultValue(INT nRowIdx, INT nOpenMode)
{
	if(!m_pIndexDM) return -1;

	CString worker = m_pParentMgr->m_pInfo->USER_ID;
	m_pIndexDM->SetCellData(_T("IBS_작업자"), worker, nRowIdx);
	if(nOpenMode > 0)
	{
		m_pIndexDM->SetCellData(_T("IBS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_청구_도서기호"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_청구_분류기호"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_제어번호"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_관리번호INFO"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_자료있는곳INFO_코드"), _T(""), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_종구분값"), _T("0"), nRowIdx);
		m_pIndexDM->SetCellData(_T("IBS_청구_권책기호"), _T(""), nRowIdx);
	}
	m_pIndexDM->SetCellData(_T("IBS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	return 0;
}

INT CIndexDataMgr::MakeInsertQuery(INT nRowIdx, CESL_DataMgr * pDM_OUT)
{
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	m_pIndexDM->InitDBFieldData();
	m_pIndexDM->MakeQueryByDM(RemoveAliasList, m_pIndexDM, m_pIndexDM->TBL_NAME, nRowIdx, pDM_OUT);
	return 0;
}

INT CIndexDataMgr::DeleteUnlessWord(CString &strData)
{
	CESL_CharSetMgr CharSetMgr_UnlessWord;
	CharSetMgr_UnlessWord.InitData(_T("역할어"), _T("역할어"));
	INT nFind = strData.ReverseFind(' ');
	if( nFind > 0 )
	{
		CString str1 = strData.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if( CharSetMgr_UnlessWord.Find(str1, str2) == 0 )
		{
			strData.Delete(nFind, strData.GetLength() - nFind);
		}
	}

	return 0;
}