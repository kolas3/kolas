// BO_IndexData.cpp: implementation of the CBO_IndexData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_IndexData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_IndexData::CBO_IndexData(CESL_Mgr * pParentMgr) : CBO_BusinessData(pParentMgr)
{
	m_strDMAlias = _T("DMUP_단행_종_색인정보");
}

CBO_IndexData::~CBO_IndexData()
{

}

INT CBO_IndexData::Apply_IndexItem(CMarc * pMarc)
{
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->Init(m_pParentMgr->m_pInfo->pMarcMgr, pMarc);
	
	INT ids;
	INT nIndexToolKit = 0;
	CString IndexToolKit[20];
	CString IndexToolKitCommand[20];
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용저작자");			IndexToolKitCommand[nIndexToolKit] = _T("저작자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_범위검색용발행년도");		IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행년도");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용범위검색발행자");	IndexToolKitCommand[nIndexToolKit] = _T("범위검색용발행자");	nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용정렬용본표제");		IndexToolKitCommand[nIndexToolKit] = _T("본표제");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용키워드");			IndexToolKitCommand[nIndexToolKit] = _T("키워드");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용발행자");			IndexToolKitCommand[nIndexToolKit] = _T("발행자");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_검색용본표제");			IndexToolKitCommand[nIndexToolKit] = _T("본표제");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_저작자사항");				IndexToolKitCommand[nIndexToolKit] = _T("저작자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_발행자사항");				IndexToolKitCommand[nIndexToolKit] = _T("발행자사항");			nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_표제사항");				IndexToolKitCommand[nIndexToolKit] = _T("표제사항");				nIndexToolKit++;
	IndexToolKit[nIndexToolKit] = _T("IBS_발행년도사항");			IndexToolKitCommand[nIndexToolKit] = _T("발행년도사항");			nIndexToolKit++;

	INT i;
	CString strTag, strData, strIdxTitle, strIdxAuthor, strIdxPublihser, strIdxKeyword;
	for (i = 0; i < nIndexToolKit; i++) {
		strData.Empty();
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex(IndexToolKitCommand[i], strData);
		ids = SetCellData(IndexToolKitCommand[i], strData);
		if(ids < 0) return -1;
	}

	// IDX_ALL_ITEM필드 설정 - 검색용본표제, 검색용저자, 검색용 발행자, 검색용색인어
	//IDX_ALL_ITEM필드를 채우기 위해서 검색용본표제, 저작자, 발행자, 키워드 를 추출한다.
	ids = GetCellData(_T("IBS_검색용본표제"), strIdxTitle);
	if(ids < 0) return -1;
	ids = GetCellData(_T("IBS_검색용저작자"), strIdxAuthor);
	if(ids < 0) return -1;
	ids = GetCellData(_T("IBS_검색용발행자"), strIdxPublihser);
	if(ids < 0) return -1;
	ids = GetCellData(_T("IBS_검색용키워드"), strIdxKeyword);
	if(ids < 0) return -1;
	CString strIdxAllItem ;
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublihser + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray, TRUE);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(i=0 ; i<cnt ; i++)
	{
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	ids = SetCellData(_T("IBS_전체검색용색인어"), strIdxAllItem);	
	if(ids < 0) return -1;
	return 0;
}

INT CBO_IndexData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
{
	if(m_pDM == NULL || pDM == NULL) return -1;
	if(nIdx >= pDM->GetRowCount()) return -1;

	if(nIdx < 0)
	{
		pDM->InsertRow(-1);
		nIdx = pDM->GetRowCount() - 1;
	}
	
	INT ids ;

	INT nColCnt = pDM->GetRefColumnCount();
	INT nPos;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		strTmpData = FIELD_ALIAS.Left(2);
		nPos = FIELD_NAME.Find(_T("."), 0);
		if(nPos >= 0) FIELD_NAME = FIELD_NAME.Mid(nPos+1);
		if(strTmpData == _T("IB")) 
		{
			ids = ApplyDMOneCell(pDM, nIdx, i, FIELD_NAME);
			if(ids < 0) return -1;
		}
	}
	return 0;
}

INT CBO_IndexData::SynchronizedIndexAndBookData(CString strSpeciesKey, CESL_DataMgr * pDM_OUT)
{
	if(pDM_OUT == NULL) return -1;

	CString strQuery;
	strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE') where rec_key=%s;"), 
		strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey, strSpeciesKey);
	pDM_OUT->AddFrame(strQuery);	
	return 0;
}

INT CBO_IndexData::ApplyMarcToData(CMarc * pMarc)
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	if(m_ArrDataAlias.GetSize() == 0) return 0;
	if(pMarc == NULL) return -1;

	INT ids;
	CString strDM, strData1, strData2, strData3;

	// 일반적인 데이타 처리
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1);
		m_pDM->SetCellData(m_ArrDataAlias.GetAt(i), strData1, 0);
	}

	//260$c에 데이터가 없으나 008 7-10에 데이터가 있으면 해당 데이터를 색인 발행년에 적용시킨다.
	strData1 = m_pDM->GetCellData(_T("IBS_발행년"), 0);
	if(strData1.IsEmpty())
	{
		ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strData1);
		if(ids < 0) return -1;
		m_pDM->SetCellData(_T("IBS_발행년"), strData1, 0);
	}
	
	// 총서명, 총서편차
	CString strSeriesTitle, strSeriesVol;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("490$a"), strSeriesTitle);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_총서명"), strSeriesTitle, 0);
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$a"), strSeriesTitle);
	if(ids < 0) return -1;
	if(!strSeriesTitle.IsEmpty()) m_pDM->SetCellData(_T("IBS_총서명"), strSeriesTitle, 0);
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$v"), strSeriesVol);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_총서편차"), strSeriesVol, 0);

	// 본표제
	ids = Get_TITLE(pMarc, strData1, strData2);
	if(ids < 0) return -1;
	if (strData1.GetLength() > 0 && strData2.GetLength() > 0) 
	{
		strData3.Format(_T("%s=%s"), strData1, strData2);
		m_pDM->SetCellData(_T("IBS_본표제"), strData3, 0);
	} 
	else 
	{
		m_pDM->SetCellData(_T("IBS_본표제"), strData1, 0);
	}

	// 저작자
	ids = Get_AUTHOR(pMarc, strData1);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_저작자"), strData1, 0);

	//발행자
	ids = Get_PUBLISHER(pMarc, strData1);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("IBS_발행자"), strData1, 0);

	// 특수한 데이타 처리 - 020(낱권)		// 1개일 경우는 IDX와 대표BOOK로 1개 이상일경우는 무시
	CString strMarcData1;
	INT nPos;
	ids = FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
	if(ids < 0) return -1;
	if(ids == 1)
	{
		//isbn에 893726387382(편권차)이렇게 들어간다. 그래서 '()'안의 문자는 빠져야한다.
		nPos = strMarcData1.Find(_T("("), 0);
		if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
		m_pDM->SetCellData(_T("IBS_낱권ISBN"), strMarcData1, 0);	
		
		FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
		//파일반입시 부가기호 에러수정 - 부가기호는 TCHAR(5)이다 따라서 이 이상일 경우 Error메시지를 출력해야한다.
		if(strMarcData1.GetLength() > 5) 
		{
			strData1.Format(_T("낱권ISBN 부가기호[%s]가 오류가 있습니다. 해당 정보는 저장되지 않습니다."), strMarcData1);
			m_pParentMgr->ESLAfxMessageBox(strData1);
			strMarcData1.Empty();
		}
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strMarcData1);
		m_pDM->SetCellData(_T("IBS_낱권ISBN부가기호"), strMarcData1, 0);	
		
		FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
		m_pDM->SetCellData(_T("IBS_낱권오류ISBN"), strMarcData1, 0);	
	}

	// 특수한 데이타 처리 - 020(세트)
	FindItemByIndicator(pMarc, _T("020$a"), _T("1 "), strMarcData1);
	//세트isbn에 893726387382(세트)이렇게 들어간다. 그래서 '()'안의 문자는 빠져야한다.
	nPos = strMarcData1.Find(_T("("), 0);
	if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
	m_pDM->SetCellData(_T("IBS_세트_ISBN1"), strMarcData1, 0);	
	FindItemByIndicator(pMarc, _T("020$g"), _T("1 "), strMarcData1);
	m_pDM->SetCellData(_T("IBS_세트_ISBN2"), strMarcData1, 0);	
	FindItemByIndicator(pMarc, _T("020$z"), _T("1 "), strMarcData1);
	m_pDM->SetCellData(_T("IBS_세트_ISBN3"), strMarcData1, 0);	

	return 0;
}

INT CBO_IndexData::ApplyDataToMarc(CMarc * pMarc)
{
	if (pMarc == NULL || m_pParentMgr == NULL) return -1;

	CString strDM, strData1, strData2, strData3;
	INT ids;
	// 일반적인 데이타 처리
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) 
	{
		strData1.Empty();
		m_pDM->GetCellData(m_ArrDataAlias.GetAt(i), 0, strData1);
		if (strData1.GetLength() > 0) 
		{
			ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1, _T(""), MODIFY_ITEM);
			if(ids < 0) return -1;
		}
		else
		{
			ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, m_ArrMarcAlias.GetAt(i));
			if(ids < 0) return -1;
		}
	}
	
	//008발행년 필드 적용
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("008"));
	if(ids < 0) return -1;
	strData1 = m_pDM->GetCellData(_T("IBS_발행년"), 0);
	ids = GetDigitData(strData1, strData2);
	if(strData2.GetLength() == 4)
	{	
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@6"), _T("s") );
		if(ids < 0) return -1;
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@7-10"), strData2);		
		if(ids < 0) return -1;
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@38-39"), _T("  ") );
		if(ids < 0) return -1;
	}
	
	//총서명/총서편차
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("440"));
	if(ids < 0) return -1;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("490"));
	if(ids < 0) return -1;
	CString arrAlias[2] = {_T("IBS_총서명"), _T("IBS_총서편차")};
	CString arrData[2];
	m_pDM->GetCellData(arrAlias, 2, arrData, 0);
	if(!arrData[0].IsEmpty() && arrData[1].IsEmpty())
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("490$a"), arrData[0]);
	else if(arrData[0].IsEmpty() && !arrData[1].IsEmpty())
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$v"), arrData[1]);
	else if(!arrData[0].IsEmpty() && !arrData[1].IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$a"), arrData[0]);
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("440$v"), arrData[1]);
	}
	
	// 본표제
	strData1 = _T("");
	m_pDM->GetCellData(_T("IBS_본표제"), 0, strData1);
	ids = Set_TITLE(pMarc, strData1);
	if(ids < 0) return -1;
	// 저작자
	strData1 = _T("");
	m_pDM->GetCellData(_T("IBS_저작자"), 0, strData1);
	ids = Set_AUTHOR(pMarc, strData1);
	if(ids < 0) return -1;
	// 편권차
	strData1 = m_pDM->GetCellData(_T("IBS_편권차"), 0);
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$n"));
	if(!strData1.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$n"), strData1);
	}
	//발행자
	strData1 = _T("");
	m_pDM->GetCellData(_T("IBS_발행자"), 0, strData1);
	ids = Set_PUBLIHSER(pMarc, strData1);
	if(ids < 0) return -1;

	ids = Apply_MARC_700Author(pMarc);
	if(ids < 0) return -1;

	// 특수한 데이타 처리 - 020(세트)
	ids = SetISBN_to_Marc(pMarc, _T("020"), _T("1 "));		// IBS_세트_ISBN is 020 1 blank
	if(ids < 0) return -1;

	return 0;
}

INT CBO_IndexData::InitBusinessData(CString strConnectionInfo, CString strDMAlias/*=_T("")*/)
{
	if(strConnectionInfo.IsEmpty()) strConnectionInfo = m_pParentMgr->CONNECTION_INFO;
	if(!strDMAlias.IsEmpty()) m_strDMAlias = strDMAlias;

	if(strConnectionInfo.IsEmpty() || m_strDMAlias.IsEmpty()) return -1;

	INT ids = InitDataManager(strConnectionInfo, m_strDMAlias);
	if(ids < 0) return -1;

	m_ArrDataAlias.RemoveAll();	m_ArrMarcAlias.RemoveAll();
	m_ArrDataAlias.Add(_T("IBS_발행년"));		m_ArrDataAlias.Add(_T("IBS_딸림자료"));
	m_ArrDataAlias.Add(_T("IBS_면장수"));		m_ArrDataAlias.Add(_T("IBS_판사항"));
	m_ArrDataAlias.Add(_T("IBS_물리적특성"));	m_ArrDataAlias.Add(_T("IBS_발행지"));
	m_ArrDataAlias.Add(_T("IBS_크기"));			m_ArrDataAlias.Add(_T("IBS_발행자"));
	m_ArrDataAlias.Add(_T("IBS_편권차"));
	m_ArrMarcAlias.Add(_T("260$c"));			m_ArrMarcAlias.Add(_T("300$e"));
	m_ArrMarcAlias.Add(_T("300$a"));			m_ArrMarcAlias.Add(_T("250$a"));
	m_ArrMarcAlias.Add(_T("300$b"));			m_ArrMarcAlias.Add(_T("260$a"));
	m_ArrMarcAlias.Add(_T("300$c"));			m_ArrMarcAlias.Add(_T("260$b"));
	m_ArrMarcAlias.Add(_T("245$n"));

	return 0;
}

INT CBO_IndexData::Apply_MARC_700Author(CMarc *pMarc)
{
	if (pMarc == NULL) return -1;
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("700"));

	CString strData, strTmpData;
	CArray<CString, CString&> StrArrayFrom;
	CArray<CString, CString&> StrArrayOrigin1;
	CArray<CString, CString&> StrArrayOrigin2;

	INT ids = -1;
	INT nCountFrom = -1;
	INT nCountOrigin1 = -1;
	INT nCountOrigin2 = -1;

	/**
		245$e -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$e"), strData, &StrArrayFrom);	
	if (ids >= 0)
	{
		nCountFrom = StrArrayFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = _T("");
			strData = StrArrayFrom.GetAt(idx);

			// 존재하는지 체크한다.
			BOOL bIsInArr = FALSE;

			StrArrayOrigin1.RemoveAll();
			strTmpData = _T("");
			DeleteUnlessWord(strData);
			m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
			nCountOrigin1 = StrArrayOrigin1.GetSize();

			for (INT j = 0; j < nCountOrigin1; j++)
			{
				if (strData == StrArrayOrigin1.GetAt(j))
				{
					bIsInArr = TRUE;
					break;
				}
			}

			if (bIsInArr) continue;			

			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	/**
		245$d -> 700$a
	*/
	StrArrayFrom.RemoveAll();
	StrArrayOrigin1.RemoveAll();
	StrArrayOrigin2.RemoveAll();
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), strData, &StrArrayFrom);	
	if (ids >= 0)
	{
		nCountFrom = StrArrayFrom.GetSize();
		for (INT idx = 0; idx < nCountFrom; idx++)
		{
			strData = _T("");
			strData = StrArrayFrom.GetAt(idx);

			// 존재하는지 체크한다.
			BOOL bIsInArr = FALSE;

			StrArrayOrigin1.RemoveAll();
			strTmpData = _T("");
			DeleteUnlessWord(strData);
			m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("700$a"), strTmpData, &StrArrayOrigin1);
			nCountOrigin1 = StrArrayOrigin1.GetSize();

			for (INT j = 0; j < nCountOrigin1; j++)
			{
				if (strData == StrArrayOrigin1.GetAt(j))
				{
					bIsInArr = TRUE;
					break;
				}
			}

			if (bIsInArr) continue;			

			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("700$a"), strData);
		}
	}

	return 0;
}

INT CBO_IndexData::SetISBN_to_Marc(CMarc *pMarc, CString strTag, CString strInd)
{
	CString strData1=_T(""), strData2=_T(""), strData3=_T("");
	CString strMarcData1=_T(""), strMarcData2=_T(""), strMarcData3=_T("");
	CString strIndexData=_T(""), strNewIndexData=_T("");
	CString strPrice, strVol;
	
	strData1 = _T("");		strData2 = _T("");		strData3 = _T("");
	strMarcData1 = _T("");		strMarcData2 = _T("");		strMarcData3 = _T("");
	m_pDM->GetCellData(_T("IBS_세트_ISBN1"), 0, strData1);
	strData1.Replace(_T("-"), _T(""));
	if(!strData1.IsEmpty()) strData1 += _T("(세트)");
	m_pDM->GetCellData(_T("IBS_세트_ISBN2"), 0, strData2);
	m_pDM->GetCellData(_T("IBS_세트_ISBN3"), 0, strData3);
	
	if (strData1.GetLength() > 0) strMarcData1.Format(_T("%ca%s"), SUBFIELD_CODE, strData1);
	if (strData2.GetLength() > 0) strMarcData2.Format(_T("%cg%s"), SUBFIELD_CODE, strData2);
	if (strData3.GetLength() > 0) strMarcData3.Format(_T("%cz%s"), SUBFIELD_CODE, strData3);
	
	if (strData1.GetLength() == 0 && strData2.GetLength() == 0 && strData3.GetLength() == 0) 
	{
		;
	} 
	else 
	{
		strNewIndexData.Format(_T("0201  %s%s%s"), strMarcData1, strMarcData2, strMarcData3);
	}
	
	CArray <CString, CString&> Array;
	Array.RemoveAll();
	CString strField;
	
	CString strTmpInd;
	strTag.Replace('$', SUBFIELD_CODE);
	m_pParentMgr->m_pInfo->pMarcMgr->GetField(pMarc, strTag, strField, &Array);
	INT nSize = Array.GetSize();
	if (nSize == 0) {		// Marc has not any isbn field data
		m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	} else {						// Marc has some isbn field data
		BOOL bIsUpdate = FALSE;
		for (INT i = 0; i < nSize; i++) {
			if (Array.GetAt(i).GetLength() < 2) continue;
			strTmpInd = Array.GetAt(i).Left(2);
			
			if (strTmpInd == strInd) {
				strIndexData.Format(_T("%s%s"),  strTag, Array.GetAt(i));
				m_pParentMgr->m_pInfo->pMarcMgr->UpdateField(pMarc, strIndexData, strNewIndexData);
				bIsUpdate = TRUE;
				break;
			}
		}
		if (bIsUpdate == FALSE) m_pParentMgr->m_pInfo->pMarcMgr->InsertField(pMarc, strNewIndexData);
	}
	return 0;
}
