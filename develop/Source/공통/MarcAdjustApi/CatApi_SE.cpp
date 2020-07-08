// CatApi_SE.cpp: implementation of the CCatApi_SE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CatApi_SE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "..\..\연속\수서\SE_API\SeApi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatApi_SE::CCatApi_SE(CESL_Mgr *pParentMgr)
	: CCatApi(pParentMgr)
{
	LoadRegCode();
	Init();
}

CCatApi_SE::~CCatApi_SE()
{

}

INT CCatApi_SE::InitMemberData()
{
	EFS_BEGIN

	m_mapMarcAlias.RemoveAll();
	m_mapCatDMtoMarc.RemoveAll();
	m_mapIndexDMtoIndexToolKit.RemoveAll();
	m_mapWorkingStatusCodeDesc.RemoveAll();

	m_mapMarcAlias.SetAt(_T("편권차"), _T("245$n"));
	m_mapMarcAlias.SetAt(_T("편제"), _T("245$p"));
	m_mapMarcAlias.SetAt(_T("서명"), _T("245$a"));
	m_mapMarcAlias.SetAt(_T("부서명"), _T("245$b"));
	m_mapMarcAlias.SetAt(_T("대등서명"), _T("245$x"));
	m_mapMarcAlias.SetAt(_T("저자"), _T("245$d"));
	m_mapMarcAlias.SetAt(_T("공저자"), _T("245$e"));
	m_mapMarcAlias.SetAt(_T("발행자"), _T("260$b"));
	m_mapMarcAlias.SetAt(_T("총서표제"), _T("440$a"));
	m_mapMarcAlias.SetAt(_T("총서편차"), _T("440$n"));
	m_mapMarcAlias.SetAt(_T("가격"), _T("350$a"));
	m_mapMarcAlias.SetAt(_T("면장수"), _T("300$a"));
	m_mapMarcAlias.SetAt(_T("물리적특성"), _T("300$b"));
	m_mapMarcAlias.SetAt(_T("크기"), _T("300$c"));
	m_mapMarcAlias.SetAt(_T("딸림자료"), _T("300$e"));

	m_mapCatDMtoMarc.SetAt(_T("ISS_판사항"), _T("250$a"));
	m_mapCatDMtoMarc.SetAt(_T("ISS_발행지"), _T("260$a"));
	m_mapCatDMtoMarc.SetAt(_T("ISS_발행자"), _T("260$b"));
	m_mapCatDMtoMarc.SetAt(_T("ISS_발행년"), _T("260$c"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_검색용저작자"), _T("저작자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_범위검색용발행년도"), _T("범위검색용발행년도"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_범위검색용발행자"), _T("범위검색용발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_검색용정렬용본표제"), _T("범위검색용본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_키워드"), _T("키워드파일"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_검색용발행자"), _T("발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_검색용본표제"), _T("본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_표제사항"), _T("표제사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_저작자사항"), _T("저작자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_발행자사항"), _T("발행자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_발행년도사항"), _T("발행년도사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_판종색인"), _T("판종색인"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_판종"), _T("판종"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_도서기호"), _T("기타부호_청구기호_도서기호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_권책기호"), _T("기타부호_청구기호_권책기호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_청구_권책DISP"), _T("기타부호_청구기호_권책기호_DISP"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_범위검색용본표제"), _T("범위검색용본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_발행년도2"), _T("발행년"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_ISBN"), _T("표준부호_ISBN"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_STRN"), _T("표준부호_STRN"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_RNSTRN"), _T("표준부호_RNSTRN"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_국가서지부호"), _T("표준부호_국가서지부호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_저작권등록번호"), _T("표준부호_저작권등록번호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호KDCP"), _T("KDCP"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호KDC"), _T("KDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호DDC"), _T("DDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호CEC"), _T("CEC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호CWC"), _T("CWC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호COC"), _T("COC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호UDC"), _T("UDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호NDC"), _T("NDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_분류기호LC"), _T("LC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_한국대학부호"), _T("한국대학부호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_한국정부기관부호"), _T("정부기관부호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_본문언어"), _T("본문언어"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_요약문언어"), _T("요약문언어"));

	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA111N"),_T("구입입수자료(인계대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA112N"),_T("구입결호자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA211N"),_T("기증입수자료(인계대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA212N"),_T("기증결호자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB111N"),_T("제본인계자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB112N"),_T("제본결정자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB113N"),_T("제본발주자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB114N"),_T("제본검수자료(등록인계대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL111N"),_T("실시간인계자료(실시간배가대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL112N"),_T("실시간배가자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL113O"),_T("실시간폐기자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SER111N"),_T("등록인계자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SER112N"),_T("등록자료(정리인계대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SER113O"),_T("가원부번호부여자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEC111N"),_T("정리대상자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEC112O"),_T("열람인계대상자료(정리완료자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL211O"),_T("열람인계자료(배가대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL212N"),_T("배가자료(소장자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL213O"),_T("수리제본자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL214O"),_T("파손자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL215O"),_T("제적자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL216O"),_T("분실자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL217O"),_T("재정리자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL311O"),_T("관외대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL312O"),_T("관내대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL411O"),_T("연속책두레업무진행자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL511O"),_T("통합도서타관반납"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL313O"),_T("특별대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL314O"),_T("긴급대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL315O"),_T("배달자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL317O"),_T("장기대출자료"));

	m_map008ToUseObjCode.SetAt(_T(" "), _T("PU"));
	m_map008ToUseObjCode.SetAt(_T("j"), _T("JU"));
	m_map008ToUseObjCode.SetAt(_T("a"), _T("JU"));
	m_map008ToUseObjCode.SetAt(_T("b"), _T("ES"));
	m_map008ToUseObjCode.SetAt(_T("c"), _T("MS"));
	m_map008ToUseObjCode.SetAt(_T("d"), _T("HS"));
	m_map008ToUseObjCode.SetAt(_T("e"), _T("AD"));
	m_map008ToUseObjCode.SetAt(_T("f"), _T("SP"));

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyDefaultValue_Species(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Species == NULL) return -1;
	
	if ( pDM_Species->GetCellData( _T("SS_작업상태구분"), 0).IsEmpty() )
	{
		pDM_Species->SetCellData( _T("SS_작업상태구분"), _T("1"), 0 );
	}

	if (pDM_Species->GetCellData( _T("SS_수입년도"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_수입년도"), CTime::GetCurrentTime().Format(_T("%Y")), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_입력구분"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_입력구분"), _T("0"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_입력일"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_입력일"), CTime::GetCurrentTime().Format(_T("%Y%m%d")), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_최초입수여부"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_최초입수여부"), _T("N"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_제본여부"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_제본여부"), _T("N"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_최종권호일련번호"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_최종권호일련번호"), _T("0"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_구입입수여부"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_구입입수여부"), _T("N"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_기증입수여부"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_기증입수여부"), _T("N"), 0);
	}

	pDM_Species->SetCellData( _T("SS_작업자"), m_pParnetMgr->m_pInfo->USER_ID, 0);

	if (pDM_Species->GetCellData( _T("SS_이용제한구분_코드"), 0).IsEmpty())
	{
		CString strUseLitmitCode;
		GetSpeciesUseLimitCode(pDM_Book, strUseLitmitCode, _T("SE"));
		pDM_Species->SetCellData( _T("SS_이용제한구분_코드"), strUseLitmitCode, 0);
	}

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	CString strClass;
	CString strVersion;

	CArray <CString ,CString&> arrClass ;    
	m_pMarcMgr->GetItem(&marc,_T("056$a"),strClass,&arrClass);
    if(arrClass.GetSize () >1) 
	{
		strClass.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strClass.IsEmpty())
			strClass = arrClass.GetAt (i) ;
			else
			strClass += _T(" ") + arrClass.GetAt (i) ;
		}
	}

	m_pMarcMgr->GetItem(&marc, _T("056$2"), strVersion);
	pDM_Species->SetCellData( _T("SS_KDC분류기호"), strClass, 0);
	pDM_Species->SetCellData( _T("SS_KDC버전"), strVersion, 0);

	arrClass.RemoveAll () ;
	m_pMarcMgr->GetItem(&marc,_T("082$a"),strClass,&arrClass);
    if(arrClass.GetSize () >1) 
	{
		strClass.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strClass.IsEmpty())
				strClass = arrClass.GetAt (i) ;
			else
				strClass += _T(" ") + arrClass.GetAt (i) ;
		}
	}
	m_pMarcMgr->GetItem(&marc, _T("082$2"), strVersion);
	pDM_Species->SetCellData( _T("SS_DDC분류기호"), strClass, 0);
	pDM_Species->SetCellData( _T("SS_DDC버전"), strVersion, 0);

	arrClass.RemoveAll () ;
	m_pMarcMgr->GetItem(&marc,_T("085$a"),strClass,&arrClass);
    if(arrClass.GetSize () >1) 
	{
		strClass.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strClass.IsEmpty())
				strClass = arrClass.GetAt (i) ;
			else
				strClass += _T(" ") + arrClass.GetAt (i) ;
		}
	}	
	pDM_Species->SetCellData( _T("SS_LC분류기호"), strClass, 0);

	CString sTmpFirstWork;
	pDM_Species->GetCellData(_T("FIRST_WORK"), 0, sTmpFirstWork);
		
	if ( sTmpFirstWork == _T("") )
	{
		pDM_Species->SetCellData(_T("FIRST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), 0);
	}
	pDM_Species->SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyDefaultValue_Index(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Index == NULL || pDM_Book == NULL) return -1;

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	
	CString strClassCode;
	ids = m_pMarcMgr->GetItem(&marc, _T("090$a"), strClassCode);
	if (ids < 0) 
	{
		return 0;
	}

	CString strBookCode;
	ids = m_pMarcMgr->GetItem(&marc, _T("090$b"), strBookCode);
	
	ids = pDM_Index->SetCellData( _T("ISS_분류기호"), strClassCode, 0);
	ids = pDM_Index->SetCellData( _T("ISS_도서기호"), strBookCode, 0);

	CString strClass;
	pDM_Species->GetCellData( _T("SS_KDC분류기호"), 0, strClass);
	pDM_Index->SetCellData( _T("ISS_분류기호KDC"), strClass, 0);

	pDM_Species->GetCellData( _T("SS_DDC분류기호"), 0, strClass);
	pDM_Index->SetCellData( _T("ISS_분류기호DDC"), strClass, 0);

	pDM_Species->GetCellData( _T("SS_LC분류기호"), 0, strClass);
	pDM_Index->SetCellData( _T("ISS_분류기호LC"), strClass, 0);

	CString sPriceChar;
	ids = m_pMarcMgr->GetItem(&marc, _T("950$a"), sPriceChar);
	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		pDM_Book->SetCellData( _T("SB_가격성격"), sPriceChar, i );
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyDefaultValue_Book(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Index)
{
	EFS_BEGIN

	if (pDM_Book == NULL || pDM_Index == NULL) return -1;

	INT ids = -1;
	CIndex *pIndex = m_pIndexTool->GetIndex();
	CString strItem;
	CString strAcqCode = pDM_Index->GetCellData( _T("ISS_수입구분_코드"), 0);
	CString strInpuDate = GetToday();

	CString strClassCode = pDM_Index->GetCellData( _T("ISS_분류기호"), 0);
	CString strBookCode = pDM_Index->GetCellData( _T("ISS_도서기호"), 0);

	CString strClass;
	ids = pDM_Index->GetCellData( _T("ISS_분류표구분"), 0, strClass);
	ids = pDM_Book->SetCellData( _T("SB_분류기호구분_코드"), strClass, 0);

	INT nRowCount = pDM_Book->GetRowCount();

	CString sAccessionRecKey;
	CString sClassNo;
	CString sBookCode;
	CString sClassNoType;
	for ( INT i = 0 ; i < nRowCount ; i++ ) {
		CString sRepresentBookYn = pDM_Book->GetCellData( _T("SB_대표책여부"), i );
		if ( sRepresentBookYn == _T("Y") ) {
			sAccessionRecKey = pDM_Book->GetCellData( _T("SB_등록원부KEY"), i );
			sClassNo = pDM_Book->GetCellData( _T("SB_분류기호"), i );
			sClassNoType = pDM_Book->GetCellData( _T("SB_분류기호구분_코드"), i );
			sBookCode = pDM_Book->GetCellData( _T("SB_도서기호"), i );
			break;
		}
	}

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;

		if (pDM_Book->GetCellData( _T("SB_수입구분_코드"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("SB_수입구분_코드"), strAcqCode, idx);
		if (pDM_Book->GetCellData( _T("SB_입력일"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("SB_입력일"), strInpuDate, idx);

		ids = pDM_Book->GetCellData( _T("SB_대표책여부"), idx, strItem);
		if (ids < 0 || strItem.CompareNoCase(_T("Y")) != 0)
			pDM_Book->SetCellData( _T("SB_대표책여부"), _T("N"), idx);

		if (pDM_Book->GetCellData( _T("SB_가격"), idx).IsEmpty()) 
			pDM_Book->SetCellData( _T("SB_가격"), _T("0"), idx);

		ids = pDM_Book->GetCellData( _T("SB_낱권ISBN"), idx, strItem);
		pIndex->MakeIndex(strItem);
		pDM_Book->SetCellData( _T("SB_낱권ISBN"), strItem, idx);

		ids = pDM_Book->SetCellData( _T("SB_분류기호"), strClassCode, idx);
		
		ids = pDM_Book->SetCellData( _T("SB_도서기호"), strBookCode, idx);

		CString strItemData = pDM_Index->GetCellData( _T("ISS_분류표구분"), 0);
		ids = pDM_Book->SetCellData( _T("SB_분류기호구분_코드"), strItemData, idx);

		CString sTmpFirstWork;
		pDM_Book->GetCellData(_T("FIRST_WORK"), idx, sTmpFirstWork);
		if ( sTmpFirstWork == _T("") )
		{
			pDM_Book->SetCellData(_T("FIRST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
		}
		pDM_Book->SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);

	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyExtraQuery(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CString &strSpeciesKey, CStringArray *pArrStrAddFrame /*= NULL*/)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book) return -1;

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapFuncName3Fields;
	
	mapFuncName3Fields.SetAt(_T("PLACE_INFO"), _T("SHELF_LOC_CODE"));
	mapFuncName3Fields.SetAt(_T("USE_LIMIT_CODE"), _T("USE_LIMIT_CODE"));
	mapFuncName3Fields.SetAt(_T("MANAGE_CODE"), _T("MANAGE_CODE"));
	mapFuncName3Fields.SetAt(_T("REG_CODE"), _T("REG_CODE"));

	CString strQuery;
	CString strSetQuery;

	CString strTemp;

	strTemp.Format(_T("IDX_HOLD_PLACE = GET_SE_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	strSetQuery += strTemp;

	strTemp.Format(_T(", MAIN_PLACE_INFO = GET_SE_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	strSetQuery +=  strTemp;

	CString strField;
	CString strParam;
	POSITION pos = mapFuncName3Fields.GetStartPosition();
	while (pos)
	{
		mapFuncName3Fields.GetNextAssoc(pos, strField, strParam);
		
		strTemp.Format(_T(", %s = GET_SE_BOOK_INFO(%s, '%s')"), strField, strSpeciesKey, strParam);
		strSetQuery += strTemp;
	}

	strQuery.Format(_T("UPDATE %s SET %s WHERE REC_KEY = %s;"), pDM_Index->TBL_NAME, strSetQuery, strSpeciesKey);
	INT ids = pDM_Species->AddFrame(strQuery);
	if (ids < 0) return ids;

	CString strRecKey;
	ids = pDM_Species->MakeRecKey(strRecKey);
	if (ids < 0) return ids;

	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
					_T("VALUES (%s, %s, 'I', '%s');")
					, strRecKey, strSpeciesKey, GetToday());

	ids = pDM_Species->AddFrame(strQuery);
	if (ids < 0) return ids;

	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_연속_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_연속_마지막번호 확인") );
		return -1;
	}
	for ( INT i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("SB_등록구분_코드"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("마지막번호"), i );
		INT iLastNum = _ttoi ( sLastNum );
		CString sQuery;

		sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER='%d', LAST_WORK='%s' ")
						_T("WHERE DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;") 
						, iLastNum, _T("자동등록번호부여"), sRegCode );

		ids = pDM_Species->AddFrame( sQuery );
		if ( 0 > ids ) {
			AfxMessageBox( _T("마지막번호 AddFrame 확인") );
			return -1;
		}
	}

	if (pArrStrAddFrame)
	{
		INT nRowCount = pArrStrAddFrame->GetSize();
		for (INT idx = 0; idx < nRowCount; idx++) {
			ids = pDM_Species->AddFrame(pArrStrAddFrame->GetAt(idx));
			if (ids < 0) return ids;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyIndex(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book) return -1;

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc;
	strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return ids;

	if (m_pIndexTool == NULL)
		m_pIndexTool = new CIndexTool;
	m_pIndexTool->Init(m_pMarcMgr, &marc);

	CString strIndexItem;
	CString strIndexDMFieldAlias;
	CString strIndexToolKitAlias;
	POSITION pos = m_mapIndexDMtoIndexToolKit.GetStartPosition();
	while (pos)
	{
		strIndexItem.Empty();
		m_mapIndexDMtoIndexToolKit.GetNextAssoc(pos, strIndexDMFieldAlias, strIndexToolKitAlias);
		
		ids = m_pIndexTool->GetIndex(strIndexToolKitAlias, strIndexItem);
		if (ids < 0) continue;

		pDM_Index->SetCellData(strIndexDMFieldAlias, strIndexItem, 0);
	}

	CString strItemData;

	ids = pDM_Index->GetCellData( _T("ISS_저작자"), 0, strItemData);
	if (strItemData.IsEmpty())
	{
		m_pMarcMgr->GetItem(&marc, _T("245$d"), strItemData);
		DeleteRoleWord(strItemData);
		pDM_Index->SetCellData( _T("ISS_저작자"), strItemData, 0);

		if (strItemData.IsEmpty()) 
		{
			CString strFormCode;
			pDM_Index->GetCellData( _T("ISS_형식구분_코드"), 0, strFormCode);
			if (strFormCode == _T("BK")) 
			{
				m_pMarcMgr->GetItem(&marc, _T("100$a"), strItemData);
				pDM_Index->SetCellData( _T("ISS_저작자"), strItemData, 0);
			} 
			else 
			{
				m_pMarcMgr->GetItem(&marc, _T("700$a"), strItemData);
				pDM_Index->SetCellData( _T("ISS_저작자"), strItemData, 0);
			}
		}
	}

	m_pMarcMgr->GetItem(&marc, _T("008@7-10"), strItemData);
	pDM_Index->SetCellData( _T("ISS_창간년"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("008@11-14"), strItemData);
	pDM_Index->SetCellData( _T("ISS_폐간년"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("022$a"), strItemData);
	pDM_Index->SetCellData( _T("ISS_ISSN"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("300$a"), strItemData);
	pDM_Index->SetCellData( _T("ISS_CODEN"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("008@18"), strItemData);
	if (strItemData.GetLength()==0) strItemData = _T("_");
	pDM_Index->SetCellData( _T("ISS_간행빈도"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("008@6"), strItemData);
	pDM_Index->SetCellData( _T("ISS_간행상태"), strItemData, 0);

	ids = pDM_Index->GetCellData( _T("ISS_본표제"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		pDM_Index->GetCellData( _T("ISS_검색용본표제"), 0, strItemData);
		pDM_Index->SetCellData( _T("ISS_본표제"), strItemData, 0);
	}

	ids = pDM_Index->GetCellData( _T("ISS_범위검색용발행자"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("발행자"), strItemData);
		pDM_Index->SetCellData( _T("ISS_범위검색용발행자"), strItemData, 0);
	}

	ids = pDM_Index->GetCellData( _T("ISS_발행자"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("발행자"), strItemData);
		pDM_Index->SetCellData( _T("ISS_발행자"), strItemData, 0);
	}

	ids = pDM_Index->GetCellData( _T("ISS_발행년"), 0, strItemData);	
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("범위검색용발행년도"), strItemData);
		pDM_Index->SetCellData( _T("ISS_발행년"), strItemData, 0);
	}

	CString strIdxVolTitle, strIdxPlaceCode, strIdxSeShelfCode;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM_Book->GetCellData( _T("SB_편권차색인"), idx, strItemData);
		if (ids >= 0 && !strItemData.IsEmpty()) strIdxVolTitle += _T(" ") + strItemData;
	
		ids = pDM_Book->GetCellData( _T("SB_배가위치부호"), idx, strItemData);
		if (ids >= 0 && !strItemData.IsEmpty()) strIdxPlaceCode += _T(" ") + strItemData;

		ids = pDM_Book->GetCellData( _T("SB_별치기호_코드"), idx, strItemData);
		if (ids >= 0 && !strItemData.IsEmpty()) strIdxSeShelfCode += _T(" ") + strItemData;
		
	}
	if (strIdxVolTitle.Mid(0,1)==_T(" ")) strIdxVolTitle = strIdxVolTitle.Mid(1);
	if (strIdxPlaceCode.Mid(0,1)==_T(" ")) strIdxPlaceCode = strIdxPlaceCode.Mid(1);
	if (strIdxSeShelfCode.Mid(0,1)==_T(" ")) strIdxSeShelfCode = strIdxSeShelfCode.Mid(1);
	pDM_Index->SetCellData( _T("ISS_권서명"), strIdxVolTitle, 0);
	pDM_Index->SetCellData( _T("ISS_자료실코드"), strIdxPlaceCode, 0);
	pDM_Index->SetCellData( _T("ISS_별치기호코드"), strIdxSeShelfCode, 0);
	
	CString strIdxTitle;
	CString strIdxAuthor;
	CString strIdxPublisher;
	CString strIdxKeyword;

	ids = pDM_Index->GetCellData( _T("ISS_검색용본표제"), 0, strIdxTitle);
	ids = pDM_Index->GetCellData( _T("ISS_검색용저작자"), 0, strIdxAuthor);
	ids = pDM_Index->GetCellData( _T("ISS_검색용발행자"), 0, strIdxPublisher);
	ids = pDM_Index->GetCellData( _T("ISS_키워드"), 0, strIdxKeyword);	
	
	CString strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	CStringArray arrStrTarget;
	MakeIndexArray(&arrStrTarget, strIdxAllItem);
	
	INT nCount = arrStrTarget.GetSize();

	strIdxAllItem.Empty();
	for (idx = 0 ; idx < nCount ; idx++)
	{
		strIndexItem = arrStrTarget.GetAt(idx);

		if(strIdxAllItem.IsEmpty()) 
			strIdxAllItem = strIndexItem;
		else 
			strIdxAllItem += _T(" ") + strIndexItem;
	}
	pDM_Index->SetCellData(_T("ISS_전체검색용색인어"), strIdxAllItem, 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyIndex_ST_CODE(CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, INT nMode)
{
	EFS_BEGIN

	CString strSTCODE, strSETISBN, strDelimeter;

	CStringArray arrStrISBN;
		
	if (nMode > 0)
	{
		strDelimeter = _T(" ");
		pDM_Index->GetCellData( _T("ISS_ISBN"), 0, strSTCODE);
		
		MakeIndexArray(&arrStrISBN, strSTCODE);
	}

	pDM_Index->GetCellData( _T("ISS_세트_ISBN1"), 0, strSETISBN);
	if(strSETISBN.GetLength() > 0)
	{
		m_pIndexTool->GetIndex()->MakeIndex(strSETISBN);
		if(!IsStringInArray(&arrStrISBN, strSETISBN)) 
			arrStrISBN.Add(strSETISBN);
	}
	
	CString strEAISBN, strEXTRA_EAISBN, strDBFlag;
	strDelimeter.Format(_T("%c"), 3);
	INT rowCnt = pDM_Book->GetRowCount();
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM_Book->GetCellData( _T("SB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_Book->GetCellData( _T("SB_낱권ISBN"), i, strEAISBN);
		m_pIndexTool->GetIndex()->MakeIndex(strEAISBN);
		if(!IsStringInArray(&arrStrISBN, strEAISBN)) arrStrISBN.Add(strEAISBN);
		pDM_Book->GetCellData( _T("SB_추가낱권ISBN"), i, strEXTRA_EAISBN);
		MakeIndexArray(&arrStrISBN, strEXTRA_EAISBN);
	}

	strSTCODE.Empty();
	for(i=0 ; i<arrStrISBN.GetSize() ; i++)
	{
		if(strSTCODE.IsEmpty()) strSTCODE = arrStrISBN.GetAt(i);
		else strSTCODE += _T(" ") + arrStrISBN.GetAt(i);
	}

	pDM_Index->SetCellData( _T("ISS_ISBN"), strSTCODE, 0);
	return 0;


	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyShelfCodeData(CESL_DataMgr *pDM_Book, 
								   CMarc *pMarc, 
								   INT nAppMode,
								   BOOL bCatHold /*=FALSE*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL || !pMarc) return -1;

	CString strQuery =	_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
						_T("WHERE GROUP_1='L' AND GROUP_2='정리' AND CLASS_ALIAS='배가실우선순위' ")
						_T("AND MANAGE_CODE=UDF_MANAGE_CODE ORDER BY VALUE_1");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_Book->CONNECTION_INFO);
	INT ids = dm.RestructDataManager(strQuery);
	if (ids < 0)
	{
		AfxMessageBox(_T("배가실 우선순위 설정을 찾을 수 없습니다."));
		return ids;
	}

	INT nPriCnt = dm.GetRowCount();

	CString strShelfLocCode;
	CString strManageCode;
	CString strOption;
	CString strKDC;
	CString strDDC;
	CString strLC;
	CString strClassType;

	BOOL bSuccess = FALSE;
	INT nPriority = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nPriority = 0;
		bSuccess = FALSE;
        
		CString strStatusCode, sDbFlag;
		strStatusCode = pDM_Book->GetCellData ( _T("SB_자료상태"), idx );
		sDbFlag = pDM_Book->GetCellData(  _T("SB_DB_FLAG"), idx );

		if ( (strStatusCode > _T("SEL212") && sDbFlag != _T("I") ) || sDbFlag == _T("D") )
			continue ;
		
		ids = pDM_Book->GetCellData( _T("SB_관리구분_코드"), idx, strManageCode);
		if (ids < 0) 
			strManageCode = _T("");
		
		while (nPriority < nPriCnt)
		{
			CString sTmp = dm.GetCellData(nPriority, 0);
			if (dm.GetCellData(nPriority, 0) == _T("분류기호"))
			{
				strOption=_T("");
				strClassType=_T("");
				strKDC = _T("");
				strDDC = _T("");
				strLC = _T("");

				m_pMarcMgr->GetItem(pMarc, _T("056$a"), strKDC);
				m_pMarcMgr->GetItem(pMarc, _T("082$a"), strDDC);
				m_pMarcMgr->GetItem(pMarc, _T("085$a"), strLC);
				
				if ( !strKDC.IsEmpty() )
				{	
					strOption = strKDC;
					strClassType = _T("1");
				}
				else if ( !strDDC.IsEmpty() )
				{
					strOption = strDDC;
					strClassType = _T("2");
				}
				else if ( !strLC.IsEmpty() )
				{
					strOption = strLC;					
					strClassType = _T("7");
				}

				if ( strOption.IsEmpty() )
				{					
					AfxMessageBox(_T("분류기호가 없습니다.\n배가실을 결정할 수 없습니다."));
					return -1;
				}
				if ( strClassType==_T("") )
				{
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_CLS_NUM_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("START_CLASS_NO <= '%s' AND END_CLASS_NO >= '%s'"))
								   , strManageCode, strOption, strOption);
				}
				else
				{
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_CLS_NUM_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("START_CLASS_NO <= '%s' AND END_CLASS_NO >= '%s' AND CLASS_NO_TYPE='%s'"))
								   , strManageCode, strOption, strOption, strClassType);
				}
			}
			else if (dm.GetCellData(nPriority, 0) == _T("등록구분"))
			{
				ids = pDM_Book->GetCellData( _T("SB_등록구분_코드"), idx, strOption);
				if (ids < 0)
				{
					AfxMessageBox(_T("등록구분이 없습니다.\n배가실을 결정할 수 없습니다."));
					return -1;
				}
				strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_REG_CODE_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("REG_CODE = '%s'")), strManageCode, strOption);
			}
			else if (dm.GetCellData(nPriority, 0) == _T("별치기호"))
			{
				ids = pDM_Book->GetCellData( _T("SB_별치기호_코드"), idx, strOption);
				if (ids < 0)
				{
					AfxMessageBox(_T("별치기호가 없습니다.\n배가실을 결정할 수 없습니다."));
					return -1;
				}

				strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_SEP_CODE_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("SEPARATE_SHELF_CODE = '%s'")), strManageCode, strOption);
			}
			else
			{
				nPriority++;
				continue;
			}

			ids = pDM_Book->GetOneValueQuery(strQuery, strShelfLocCode);
			if (ids < 0 || strShelfLocCode.IsEmpty())
			{
				nPriority++;
				continue;
			}
			else
			{
				if (!bCatHold || pDM_Book->GetCellData( _T("SB_배가위치부호"), idx).IsEmpty())
				{
					pDM_Book->SetCellData( _T("SB_배가위치부호"), strShelfLocCode, idx);

					CString strQueryFlag = GetQueryFlag(MNG_MODIFY, nAppMode, pDM_Book->GetCellData( _T("SB_DB_FLAG"), idx));
					pDM_Book->SetCellData( _T("SB_DB_FLAG"), strQueryFlag, idx);
				}
				bSuccess = TRUE;
				break;
			} 
		}

		if (!bSuccess)
		{
			AfxMessageBox(_T("배가위치를 지정할수 없습니다."));
			return -1;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::CompleteCataloging(CESL_DataMgr *pDM_Out, 
								CESL_DataMgr *pDM_Species, 
								INT nIdx /*= 0*/, 
								CESL_DataMgr *pDM_Book /*= NULL*/, 
								BOOL bCommit /*= TRUE*/, 
								CESL_DataMgr *pDM_Index /*= NULL*/, 
								CMarc *pMarc /*= NULL*/)
{
	EFS_BEGIN

	if (!pDM_Out ||
		!pDM_Species) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strItem;
	CString strSpeciesKey;
	CString strBookKey;
	CString strControlCode;
	CString strQuery;

	CString strYear = CTime::GetCurrentTime().Format(_T("%Y"));
	
	ids = pDM_Species->GetCellData( _T("SS_종KEY"), nIdx, strSpeciesKey);
	if (ids < 0 || strSpeciesKey.IsEmpty()) 
	{
		m_strLastErrMsg = _T("종키가 없는 자료입니다.");
		return -1;
	}

	CMarc marc;
	CString strStreamMarc;

	if (pMarc == NULL)
	{
		pMarc = &marc;
		
		ids = pDM_Species->GetCellData( _T("SS_MARC"), nIdx, strStreamMarc);
		ids = m_pMarcMgr->Decoding(strStreamMarc, pMarc);
	}

	ids = m_pMarcMgr->GetItem(pMarc, _T("001*"), strItem);
	strItem.TrimLeft();
	if (ids >= 0 && !strItem.IsEmpty())
	{
		if (strItem.GetLength() == 7)
		{
			TCHAR szTemp[4];
			_stscanf(strItem, _T("%3s%4s"), szTemp, strYear.GetBuffer(5));
			strYear.ReleaseBuffer();
		}
		else
		{
			pDM_Species->SetCellData( _T("SS_제어번호"), strItem, nIdx);
			return 0;
		}
	}

	ids = pDM_Species->GetCellData( _T("SS_제어자료구분_코드"), nIdx, strControlCode);
	if (ids < 0 || strControlCode.IsEmpty()) 
	{
		m_strLastErrMsg = _T("제어자료구분_코드가 없는 자료입니다.");
		return -1;
	}

	if (pDM_Book)
	{
		nRowCount = pDM_Book->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			ids = pDM_Book->GetCellData( _T("SB_등록번호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				m_strLastErrMsg = _T("등록번호가 부여되지 않은 책이 있습니다.");
				return -1;
			}			

			ids = pDM_Book->GetCellData( _T("SB_배가위치부호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				m_strLastErrMsg = _T("배가실 코드가 부여되지 않은 책이 있습니다.");
				return -1;
			}
		}
	}

	if (bCommit) {
		ids = pDM_Out->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}

	CString strReckey;
	CString strControlNo;
	INT iControlNoCount = 0;

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format( _T("%04d"), time.GetYear() );

	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE MAKE_YEAR='%s' AND DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
					, strYear, strControlCode );

	ids = pDM_Out->GetOneValueQuery(strQuery, strItem);
	if (ids < 0)
	{
		strControlNo = _T("1");

		CString sControlNo;
		sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );
	
		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, sControlNo);
	
		ids = pDM_Out->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return -1;
		}

		ids = pDM_Out->MakeRecKey(strReckey);
		if (ids < 0) return ids;

		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
						_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, ")
						_T("LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
						_T("VALUES('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);")
						, strReckey, strControlCode, strYear, strControlNo
						, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG)
						, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG)	);
	}
	else
	{
		if ( iControlNoCount < 0 ) {
			iControlNoCount = 0;
		}
		CESL_DataMgr *pDmControlNo = m_pParnetMgr->FindDM( _T("DM_정리_출력_장비") );
		INT iControlCount = pDmControlNo->GetRowCount();
		if ( iControlCount == 0 ) {
			pDmControlNo->InsertRow(-1);
			pDmControlNo->SetCellData( _T("작업상태"), strItem, 0 );
		}
		else {
			CString sControl = pDmControlNo->GetCellData( _T("작업상태"), 0 );
			if ( sControl != strItem ) {
				iControlNoCount = 0;
			}
		}

		strControlNo.Format(_T("%d"), _ttoi(strItem) + 1 + iControlNoCount);		

		CString sControlNo;
		sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );

		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, sControlNo );
		
		ids = pDM_Out->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return -1;
		}
		
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%s, LAST_WORK='%s' ")
						_T("WHERE DIVISION_VALUE='%s' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
						, strControlNo, m_pParnetMgr->GetWorkLogMsg(_T("Marc수정"),WORK_LOG)
						, strControlCode, strYear );

		iControlNoCount++;
	}

	ids = pDM_Out->AddFrame(strQuery);
	if ( 0 > ids ) {
		AfxMessageBox( _T("마지막번호 업데이트 AddFrame() 확인!") );
		return -1;
	}

	strItem = strControlNo;

	m_pMarcMgr->DeleteField(pMarc, _T("001"));
	
	strControlNo.Format(_T("%s%s%05s"), strControlCode, strYear, strItem);
	ids = m_pMarcMgr->InsertField(pMarc, _T("001") + strControlNo);
	m_pMarcMgr->Encoding(pMarc, strStreamMarc);


	pDM_Out->InitDBFieldData();
	pDM_Out->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, 1);
	pDM_Out->AddDBFieldData(_T("CONTROL_NO"), _T("STRING"), strControlNo, 1);
	pDM_Out->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParnetMgr->GetWorkLogMsg(_T("정리완료"),WORK_LOG), 1);


	ids = pDM_Out->MakeUpdateFrame( _T("SE_SPECIES_TBL"), _T("REC_KEY"), _T("STRING"), strSpeciesKey );
	if ( ids < 0 ) {
		AfxMessageBox( _T("CatApi MakeUpdateFrame 1 확인") );
		return -1;
	}	

	strQuery.Format(_T("UPDATE IDX_SE_TBL SET CONTROL_NO = '%s' WHERE REC_KEY = '%s';")
		, strControlNo, strSpeciesKey);
	
	ids = pDM_Out->AddFrame(strQuery);
	if (ids < 0) return ids;

	ids = pDM_Species->SetCellData( _T("SS_제어번호"), strControlNo, nIdx);
	ids = pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, nIdx);

	if (pDM_Index)
		ids = pDM_Index->SetCellData( _T("ISS_제어번호"), strControlNo, nIdx);

	if (pDM_Book)
	{
		nRowCount = pDM_Book->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			ids = pDM_Book->GetCellData( _T("SB_책KEY"), idx, strBookKey);
			if (ids < 0 || strBookKey.IsEmpty()) continue;

			strQuery.Format(_T("UPDATE %s SET WORKING_STATUS = 'BOC114O', LAST_WORK = '%s' WHERE REC_KEY = %s;"), pDM_Book->TBL_NAME, m_pParnetMgr->GetWorkLogMsg(_T("Marc수정"),WORK_LOG), strBookKey);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}


	if (bCommit)
	{
		ids = pDM_Out->SendFrame();
		if (ids < 0) return ids;

		ids = pDM_Out->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::UpdateSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix, 
							  CESL_DataMgr *pDM_HoldBook, 
							  CESL_DataMgr *pDM_HoldAppendix, 
							  CStringArray *pArrStrAddFrame /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL ||
		pDM_Appendix == NULL ||
		pDM_HoldBook == NULL ||
		pDM_HoldAppendix == NULL
		) return -1;

	INT ids = -1;
	CString strItem;

	CArray <CString, CString> RemoveAliasList;

	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	CString strSpeciesKey;
	ids = pDM_Species->GetCellData(_T("SS_종KEY"), 0, strSpeciesKey);
	
	CopyDM(pDM_Index, 0, pDM_Species, 0, _T("SS"));

	ids = ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	pDM_Species->GetCellData( _T("SS_이용제한구분_코드"), 0, strItem);
	GetSpeciesUseLimitCode(pDM_Book, strItem, _T("SE"), 1, strItem);
	pDM_Species->SetCellData( _T("SS_이용제한구분_코드"), strItem, 0);

	pDM_Species->SetCellData(_T("SS_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), 0);

	RemoveAliasList.Add( _T("SS_종KEY") );

	ids = ConvertBookTo049(pDM_Species, pDM_Book, MNG_MODIFY);
	if (ids >= 0)
	{
		CString strHoldData = pDM_Species->GetCellData( _T("SS_소장정보"), 0);
		if ( !strHoldData.IsEmpty() )
		{
			CString strQuery;
			CString strHoldInfoKey = pDM_Species->GetCellData( _T("SS_소장정보KEY"), 0);
			if (!strHoldInfoKey.IsEmpty()) 
			{
				strQuery.Format(_T("UPDATE SE_HOLDING_TBL SET HOLDING_DATA='%s' WHERE REC_KEY=%s"), strHoldData, strHoldInfoKey);

				TCHAR  *pFieldName[] = {_T("HOLDING_DATA")};
				TCHAR  *pFieldData[1];
				INT	  InDataType[] = {STRING};

			    pFieldData[0] = 	strHoldData.GetBuffer(0);
				ids = pDM_Species->ESL_UpdateProc(_T("SE_HOLDING_TBL"),
											1				,						     
											pFieldName     ,
											pFieldData     ,
											InDataType    ,
											_T("REC_KEY"),
											strHoldInfoKey.GetBuffer(0));			
			}
			else 
			{
				ids = pDM_Species->MakeRecKey(strHoldInfoKey);
				if (ids < 0) return ids;

				pDM_Species->InitDBFieldData();
				pDM_Species->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strHoldInfoKey, TRUE);
				pDM_Species->AddDBFieldData(_T("HOLDING_DATA"), _T("STRING"), strHoldData, TRUE);
				ids = pDM_Species->MakeInsertFrame(_T("SE_HOLDING_TBL"));
				if (ids < 0) return ids;
			}
		}
	}

	pDM_Species->InitDBFieldData();
	pDM_Species->MakeQueryByDM(RemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	ids = ApplyIndex(pDM_Species, pDM_Index, pDM_Book);

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add( _T("ISS_REC_KEY") );

	pDM_Species->InitDBFieldData();
	pDM_Species->MakeQueryByDM(RemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("ISS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("ISS_관리구분_코드"));
	Index_classfycodes.Add( _T("ISS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("ISS_등록구분_코드"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("SB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("SB_관리구분_코드"));
	Book_classfycodes.Add( _T("SB_배가위치부호"));
	Book_classfycodes.Add( _T("SB_등록구분_코드"));
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("SB"));
	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	ids = MakeVolumeQueryFrame(pDM_Volume, pDM_Book, pDM_HoldBook, pDM_Appendix, pDM_HoldAppendix, strSpeciesKey, pDM_Species);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("권호정보 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("책정보 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldBook, strSpeciesKey, _T("HOLD_BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("소장책 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldAppendix, strSpeciesKey, _T("HOLD_APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("소장부록정보 쿼리만들기 실패");
		return ids;
	}

	ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);

	ids = pDM_Species->SendFrame();
	if ( 0 > ids ) return -1;
	ids = pDM_Species->EndFrame();	
	if (ids < 0)
	{
		m_strLastErrMsg = _T("DB에 마크를 입력하는데 에러가 발생했습니다.");
		return ids;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ConvertBookTo049(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Book, INT nAppMode )
{
	EFS_BEGIN

	if (!pDM_Species || !pDM_Book) return -1;

	CESL_DataMgr dm;
	INT ids;
	dm.SetCONNECTION_INFO(pDM_Book->CONNECTION_INFO);
	dm.SetMarcMgr(pDM_Book->m_pMarcMgr);
	dm.InitDMPKFromFile(_T("DM_연속_정리DM_책정보"));

	if (nAppMode == MNG_MODIFY)
	{
		CString strSpeciesKey = pDM_Species->GetCellData(_T("SS_종KEY"), 0);
		if (strSpeciesKey.IsEmpty()) return -1;

		CString strOption;
		strOption.Format(_T(" ((ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57))"
			_T(" AND (SUBSTR(WORKING_STATUS,0,3) NOT IN ('SER'))")
			_T(" AND SPECIES_KEY = %s ")), strSpeciesKey);
		ids = dm.RefreshDataManager(strOption);
		if (ids < 0) return ids;
	}

	ids = MergeDM(pDM_Book, &dm, _T("SB_책KEY"), 1);
	if (ids < 0) return ids;

	CString str049;
	CStringArray arrStr049;

	CString strRegNo;
	CString strDupCode;
	CString strVolCode;
	CString strSepCode;

	INT nRowCount = dm.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		str049.Empty();
		
		strRegNo = dm.GetCellData( _T("SB_등록번호"), idx);
		strDupCode = dm.GetCellData( _T("SB_복본기호"), idx);
		strVolCode = dm.GetCellData( _T("SB_권책기호"), idx);
		strSepCode = dm.GetCellData( _T("SB_별치기호_코드"), idx);

		if (strRegNo.IsEmpty()) continue;
				
		str049.Format(_T("%cl%s"), SUBFIELD_CODE, strRegNo);
		if (!strVolCode.IsEmpty())
		{
			CString strTmp = str049;
			str049.Format(_T("%s%cv%s"), strTmp, SUBFIELD_CODE, strVolCode);
		}
		if (!strDupCode.IsEmpty())
		{
			CString strTmp = str049;
			str049.Format(_T("%s%cc%s"), strTmp, SUBFIELD_CODE, strDupCode);
		}
		if (!strSepCode.IsEmpty())
		{
			CString strTmp = str049;
			str049.Format(_T("%s%cf%s"), strTmp, SUBFIELD_CODE, strSepCode);
		}


		arrStr049.Add(str049);		
	}

	str049 = _T("0491 ");
	nRowCount = arrStr049.GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		str049 += arrStr049.GetAt(idx);
	}
	
	CMarc marc;
	ids = m_pMarcMgr->InsertField(&marc, str049);
	if (ids < 0) return ids;

	CString strStreamMarc;
	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	if (ids < 0) return ids;

	pDM_Species->SetCellData( _T("SS_소장정보"), strStreamMarc, 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ImportSpecies(CESL_DataMgr *pDM_Species, 
								CESL_DataMgr *pDM_Index, 
								CESL_DataMgr *pDM_Book, 
								CESL_DataMgr *pDM_Volume, 
								CESL_DataMgr *pDM_Appendix, 
								CESL_DataMgr *pDM_HoldBook, 
								CESL_DataMgr *pDM_HoldAppendix, 
								CStringArray *pArrStrAddFrame /*= NULL*/, 
								LPCTSTR lpszShelfDate /*= NULL*/,
								INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ||
		!pDM_Volume ||
		!pDM_Appendix ||
		!pDM_HoldBook ||
		!pDM_HoldAppendix ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;

	CESL_DataMgr *DM_LastNumber = NULL;

	if ( nAppMode == MNG_IMPORT || nAppMode == MNG_IMPORT_WORKING )
	{
		DM_LastNumber = m_pParnetMgr->FindDM( _T("DM_마크반입_마지막번호") );
		
		CString sLastNumberQuery = _T("") ;

		sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
								_T("WHERE KIND_CODE='REG_NO' AND MANAGE_CODE=UDF_MANAGE_CODE") );

		DM_LastNumber->RestructDataManager(sLastNumberQuery);

		nRowCount = pDM_Book->GetRowCount();
	}

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("SB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
			}
		}

		pDM_Book->SetCellData( _T("SB_등록번호"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, pDM_Book->TBL_NAME, strRegNo );

		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			m_strLastErrMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
			return -1;
		}
	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	ids = ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;

	CString strLibCode;
	GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	if ( strLibCode.IsEmpty() )
	{
		m_strLastErrMsg.Format(_T("도서관 부호가 없습니다. 수정해주세요"));
		return -1;
	}
	ApplyLastModifyData(&marc);


	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("SS_정리된자료"), _T("Y"), 0 );
	
	ids = pDM_Species->SetCellData( _T("SS_작업상태구분"), _T("1"), 0 );

	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, pDM_HoldBook, pDM_HoldAppendix, pArrStrAddFrame, FALSE);
	if (ids < 0) return -1;

	ids = CompleteCataloging(pDM_Species, pDM_Species, 0, pDM_Book, FALSE);
	if (ids < 0) return -1;

	CString strShelfDate;
	if (lpszShelfDate == NULL)
	{
		strShelfDate = _T("1200/01/01");
	}
	else
		strShelfDate = lpszShelfDate;		

	CStringArray arrSpeciesKey;
	CStringArray arrBookKey;
	CStringArray arrAppendixKey;

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("SS_종KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("SB_책KEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("SB_책KEY"), idx));
		}
	}

	ids = RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
	if (ids < 0) return -1;

	ids = pDM_Species->SendFrame();
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("DB에 데이터 저장 실패");
		return ids;
	}

	ids = pDM_Species->EndFrame();
	
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::InsertSpecies(CESL_DataMgr *pDM_Species, 
								CESL_DataMgr *pDM_Index, 
								CESL_DataMgr *pDM_Book, 
								CESL_DataMgr *pDM_Volume, 
								CESL_DataMgr *pDM_Appendix, 
								CESL_DataMgr *pDM_HoldBook, 
								CESL_DataMgr *pDM_HoldAppendix, 
								CStringArray *pArrStrAddFrame /*= NULL*/, 
								BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL ||
		pDM_Appendix == NULL ||
		pDM_HoldBook == NULL ||
		pDM_HoldAppendix == NULL
		) return -1;

	INT ids = -1;

	if (bCommit)
	{
		INT ids = pDM_Species->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}

	CString strSpeciesKey;
	CArray<CString, CString> arrRemoveAliasList;

	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("SS") );

	ids = ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	arrRemoveAliasList.Add( _T("SS_MARC"));

	ids = pDM_Species->MakeRecKey(strSpeciesKey);
	if (ids < 0) return ids;
	pDM_Species->SetCellData( _T("SS_종KEY"), strSpeciesKey, 0);

	ids = ConvertBookTo049(pDM_Species, pDM_Book, MNG_INSERT);
	if (ids >= 0)
	{
		CString strHoldData = pDM_Species->GetCellData( _T("SS_소장정보"), 0);
		CString strQuery;
		CString strHoldInfoKey;
		ids = pDM_Species->MakeRecKey(strHoldInfoKey);
		if (ids < 0) return ids;
		
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strHoldInfoKey, TRUE);
		pDM_Species->AddDBFieldData(_T("HOLDING_DATA"), _T("STRING"), strHoldData, TRUE);
		ids = pDM_Species->MakeInsertFrame(_T("SE_HOLDING_TBL"));

		if (ids < 0) return ids;
	}

	pDM_Species->SetCellData(_T("SS_최초작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), 0);
	pDM_Species->SetCellData(_T("SS_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), 0);

	pDM_Species->InitDBFieldData();
	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	LogToFile(pDM_Species->m_lastmsg);

	pDM_Index->SetCellData( _T("ISS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("ISS_종구분값"), _T("1"), 0);

	arrRemoveAliasList.RemoveAll();
	ids = ApplyIndex(pDM_Species, pDM_Index, pDM_Book);
	
	pDM_Species->InitDBFieldData();
	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species);
	LogToFile(pDM_Species->m_lastmsg);

	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("ISS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("ISS_관리구분_코드"));
	Index_classfycodes.Add( _T("ISS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("ISS_등록구분_코드"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("SB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("SB_관리구분_코드"));
	Book_classfycodes.Add( _T("SB_배가위치부호"));
	Book_classfycodes.Add( _T("SB_등록구분_코드"));
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("SB"));
	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	ids = MakeVolumeQueryFrame(pDM_Volume, pDM_Book, pDM_HoldBook, pDM_Appendix, pDM_HoldAppendix, strSpeciesKey, pDM_Species);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("권호정보 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("책정보 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldBook, strSpeciesKey, _T("HOLD_BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("소장책 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldAppendix, strSpeciesKey, _T("HOLD_APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("소장부록정보 쿼리만들기 실패");
		return ids;
	}

	ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	
	CString strStreamMarc;
	pDM_Species->GetCellData( _T("SS_MARC"), 0, strStreamMarc);
	pDM_Species->InitDBFieldData();
	pDM_Species->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, TRUE);
	pDM_Species->MakeUpdateFrame(_T("SE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	if (bCommit)
	{
		ids = pDM_Species->SendFrame();
		pDM_Species->EndFrame();
		if (ids < 0) 
		{
			m_strLastErrMsg = _T("DB에 종을 입력하는데 에러가 발생했습니다.");
			return ids;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::MakeRemoveListByHD( CESL_DataMgr *pDM_Book, CString strRemoveFieldHD, CArray <CString, CString> &RemoveAliasList )
{
	INT i;
	CString strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType;

	for (i=0; i<pDM_Book->GetRefColumnCount(); i++)
	{
		strFieldAlias = _T("");
		pDM_Book->GetColumnRefInfo(i, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);
		if (strFieldAlias.Find( strRemoveFieldHD ) >= 0)
			RemoveAliasList.Add( strFieldAlias );
	}
	return 0;
}

INT CCatApi_SE::MakeBookQueryFrame(CESL_DataMgr *pDM_Book, CString strSpeciesKey, CString strType,
								   CESL_DataMgr *pDM_OUT, INT nSendCount/*=-1*/)
{
	EFS_BEGIN

	INT ids = -1;
	INT idx = -1;
	
	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strBookKey;
	CString strQueryFlag;

	CIndex *pIndex = m_pIndexTool->GetIndex();

	CArray <CString, CString> RemoveAliasList;

	CSeApi api(m_pParnetMgr);
	CString strBOOK_KEY, strACQ_TYPE_VOL_KEY, strVOL_KEY;
	CString strSPECIES_KEY, strBOOK_TYPE, strBooktitle, msg;

	INT nRowCount = pDM_Book->GetRowCount();

	CString strIsRepresentBook;
	CString strRegNoList = _T("");
	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	for (idx = 0; idx < nRowCount; idx++) 
	{
		ids = pDM_Book->GetCellData( _T("SB_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		if (strQueryFlag == _T("I"))
		{
			ids = pDM_Book->GetCellData( _T("SB_등록번호"), idx, strRegNo);
            if( ids < 0 || strRegNo.IsEmpty ())
			{
				AfxMessageBox(_T("등록번호가 부여되지 않은 책이 있습니다.\r\n등록번호를 확인하여 주십시요"));
				return -1;
			}

			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
				if ( strRegNo.GetLength() > nRegCodeLength ) {
					strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
				}
			}
			
			if ( strRegNoList.GetLength() > 0 ) strRegNoList += _T(", '") + strRegNo + _T("'");
			else strRegNoList.Format(_T("'%s'"), strRegNo);
		}
	}
	
	if (!strRegNoList.IsEmpty())
	{
		strQuery.Format(_T("SELECT REC_KEY FROM SE_BOOK_TBL WHERE REG_NO IN (%s) AND MANAGE_CODE=UDF_MANAGE_CODE")
						, strRegNoList);
		ids = pDM_OUT->GetOneValueQuery(strQuery, strResult);
		if (ids >= 0 && !strResult.IsEmpty())
		{
			AfxMessageBox(_T("등록번호가 중복됩니다. [") + strRegNo + _T("]") );
			return -1;
		}
	}

	for (idx = 0; idx < nRowCount; idx++) {
		RemoveAliasList.RemoveAll();
		ids = pDM_Book->GetCellData(_T("SB_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		CString sPrice;
		CString sRealPrice;
		ids = pDM_Book->GetCellData(_T("SV_가격"), idx, sPrice);
		if ( sPrice != _T("") ) {
			for( INT i = 0 ; i < sPrice.GetLength() ; i++ ) {
				if ( sPrice.Mid( i, 1 ) >= _T("0") && sPrice.Mid( i, 1 ) <= _T("9") ) {
					sRealPrice = sRealPrice + sPrice.Mid( i, 1 );
				}
			}
			ids = pDM_Book->SetCellData(_T("SV_가격"), sRealPrice, idx);
		}

		if (strQueryFlag == _T("I")) 
		{
			pDM_Book->SetCellData(_T("SB_종KEY"), strSpeciesKey, idx);
			pDM_Book->SetCellData(_T("SB_최초작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
			pDM_Book->SetCellData(_T("SB_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
			if (strType == _T("BOOK"))
			{
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			else if (strType == _T("HOLD_BOOK"))
			{
				MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			else if (strType == _T("APPENDIX"))
			{
				MakeRemoveListByHD( pDM_Book, _T("SA"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);

				pDM_Book->SetCellData(_T("SA_최초작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
				pDM_Book->SetCellData(_T("SA_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
				RemoveAliasList.RemoveAll();
				MakeRemoveListByHD( pDM_Book, _T("SB"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_APPENDIX_BOOK_TBL AP"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			else if (strType == _T("HOLD_APPENDIX"))
			{
				MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );
				MakeRemoveListByHD( pDM_Book, _T("SA"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);

				pDM_Book->SetCellData(_T("SA_최초작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
				pDM_Book->SetCellData(_T("SA_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
				RemoveAliasList.RemoveAll();
				MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );
				MakeRemoveListByHD( pDM_Book, _T("SB"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_APPENDIX_BOOK_TBL AP"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			
		}
		else if (strQueryFlag == _T("U")) 
		{
			RemoveAliasList.Add(_T("SB_책KEY"));
			ids = pDM_Book->GetCellData(_T("SB_책KEY"), idx, strBookKey);
			pDM_Book->SetCellData(_T("SB_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
			
			if (strType == _T("BOOK") || strType == _T("HOLD_BOOK"))
			{
				if( strType == _T("HOLD_BOOK") )
					MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );

				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
				LogToFile(pDM_OUT->m_lastmsg);

				if( pDM_Book->GetCellData(_T("SB_책종류"), idx) == _T("B") )
				{
					RemoveAliasList.Add(_T("SB_등록원부KEY"));
					RemoveAliasList.Add(_T("SB_수입구분_코드"));
					RemoveAliasList.Add(_T("SB_입수일"));
					RemoveAliasList.Add(_T("SB_수입구분별권KEY"));
					RemoveAliasList.Add(_T("SB_수입년도"));
					RemoveAliasList.Add(_T("SB_제본책여부"));
					RemoveAliasList.Add(_T("SB_제본책KEY"));
					RemoveAliasList.Add(_T("SB_제본인계일"));
					RemoveAliasList.Add(_T("SB_장서점검"));
					RemoveAliasList.Add(_T("SB_도서기호"));
					RemoveAliasList.Add(_T("SB_책종류"));
					RemoveAliasList.Add(_T("SB_분류기호"));
					RemoveAliasList.Add(_T("SB_분류기호구분_코드"));
					RemoveAliasList.Add(_T("SB_복본기호"));
					RemoveAliasList.Add(_T("SB_실시간인계일"));
					RemoveAliasList.Add(_T("SB_입력일"));
					RemoveAliasList.Add(_T("SB_레이블출력구분"));
					RemoveAliasList.Add(_T("SB_접수번호"));
					RemoveAliasList.Add(_T("SB_접수일련번호"));
					RemoveAliasList.Add(_T("SB_등록구분_코드"));
					RemoveAliasList.Add(_T("SB_등록번호"));
					RemoveAliasList.Add(_T("SB_등록인계일"));
					RemoveAliasList.Add(_T("SB_비고"));
					RemoveAliasList.Add(_T("SB_별치기호_코드"));
					RemoveAliasList.Add(_T("SB_종KEY"));
					RemoveAliasList.Add(_T("SB_권책기호"));
					RemoveAliasList.Add(_T("SB_권KEY"));
					RemoveAliasList.Add(_T("SB_인계여부"));

					pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT, _T("BINDING_BOOK_KEY"), _T("NUMERIC"), strBookKey);
					LogToFile(pDM_OUT->m_lastmsg);
				}
			}
			else if (strType == _T("APPENDIX") || strType == _T("HOLD_APPENDIX"))
			{
				if( strType == _T("HOLD_APPENDIX") )
					MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );

				MakeRemoveListByHD( pDM_Book, _T("SA"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
				LogToFile(pDM_OUT->m_lastmsg);

				pDM_Book->SetCellData(_T("SA_마지막작업"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
				RemoveAliasList.RemoveAll();
				ids = pDM_Book->GetCellData(_T("SA_부록KEY"), idx, strBookKey);
				MakeRemoveListByHD( pDM_Book, _T("SB"), RemoveAliasList );

				if( strType == _T("HOLD_APPENDIX") )
					MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );

				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_APPENDIX_BOOK_TBL AP"), idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
				LogToFile(pDM_OUT->m_lastmsg);
			}
		}
		else if (strQueryFlag == _T("D")) 
		{
			strBOOK_KEY = pDM_Book->GetCellData(_T("SB_책KEY"),idx);
			strACQ_TYPE_VOL_KEY = pDM_Book->GetCellData(_T("SB_수입구분별권KEY"),idx);
			strVOL_KEY = pDM_Book->GetCellData(_T("SB_권KEY"),idx);
			strSPECIES_KEY = pDM_Book->GetCellData(_T("SB_종KEY"),idx);
			strBOOK_TYPE = pDM_Book->GetCellData(_T("SB_책종류"),idx);

			if (strType == _T("BOOK"))
				strBooktitle = pDM_Book->GetCellData(_T("SV_권호명"),idx);
			else if (strType == _T("APPENDIX"))
				strBooktitle = pDM_Book->GetCellData(_T("SA_부록명"),idx);

			ids = api.BOOK_CAT_DELETE(pDM_OUT,
							strBOOK_KEY,strACQ_TYPE_VOL_KEY,strVOL_KEY,
							strSPECIES_KEY,strBOOK_TYPE,strBooktitle,TRUE, FALSE);
			if(ids<0)
			{
				if(-1002 == ids) 
				{
					msg.Format(_T("[ %s ] 자료를 삭제할 수 없습니다!"), strBooktitle);
					AfxMessageBox(msg);
					continue;
				}
				else
				{
					msg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"), strBooktitle);
					AfxMessageBox(msg);
					return -200;
				}
			}

			CString strRegNo;
			ids = pDM_Book->GetCellData(_T("SB_등록번호"), idx, strRegNo);
			if ( strRegNo != _T("")){
				INT iRegNumber;
				iRegNumber = _ttoi(strRegNo.Mid(3, strRegNo.GetLength()));
			
				CString strRegCode;
				pDM_Book->GetCellData(_T("SB_등록구분_코드"), idx, strRegCode);

				CString strRecKey;
				ids = pDM_OUT->MakeRecKey(strRecKey);
				if ( 0 > ids ) return -1;

				strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
								_T("(REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
								_T("VALUES (%s, '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE);")
								, strRecKey, _T("MO"), strRegCode, iRegNumber
								, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG)
								, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG)	);

				ids = pDM_OUT->AddFrame(strQuery);
				if ( 0 > ids ) return -1;
			}
			LogToFile(pDM_OUT->m_lastmsg);
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::MakeMarcAdjustDM(CESL_DataMgr *pDM_From_Species, INT nFromSpeciesRow, CESL_DataMgr *pDM_From_Index, INT nFromIndexRow, CESL_DataMgr *pDM_To_Species, CESL_DataMgr *pDM_To_Index, CESL_DataMgr *pDM_To_Book, CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);

	INT ids = -1;
	CMarc marcSource;
	CMarc marcOutput;
	CString strItemData;
	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strFirstInd;
	CString strDefaultShelfLocCode;

	CArray<CString, CString&> arrItemData;

	CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);

	CopyDM(pDM_From_Index, nFromSpeciesRow, pDM_To_Index, 0);

	CString strStreamMarc;
	strStreamMarc = pDM_To_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marcSource);

	if (ids < 0) 
	{
		m_strLastErrMsg = _T("Marc Decoding 실패");
		return ids;
	}

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapCatDMtoMarc;
	mapCatDMtoMarc.RemoveAll();
	mapCatDMtoMarc.SetAt(_T("ISS_판사항"), _T("250$a"));
	mapCatDMtoMarc.SetAt(_T("ISS_발행지"), _T("260$a"));
	mapCatDMtoMarc.SetAt(_T("ISS_발행자"), _T("260$b"));
	mapCatDMtoMarc.SetAt(_T("ISS_발행년"), _T("260$c"));

	POSITION pos = mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		ids = m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData);
		if (ids < 0) continue;

		pDM_To_Index->SetCellData(strDMFieldAlias, strItemData, 0);	
	}

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;

	CString strTitle;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strSubfieldCode;
	posTag = marcSource.m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)marcSource.m_listTag.GetNext(posTag);
		if (pTag && pTag->GetTagCode() == _T("245"))
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$b"))
				{
					strTitle += pSubfield->GetPunctuationMark() + pSubfield->GetItemData();
				}
			}
		}		
	}
	
	pDM_To_Index->SetCellData(_T("ISS_본표제"), strTitle, 0);
	
	m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
	m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData);
	if (!strItemData.IsEmpty())
	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
		ids = m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData, &arrItemData);
		if (ids >= 0)
		{
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;

				strAuthor += _T(";") + strItemData;
			}
		}

		pDM_To_Index->SetCellData(_T("ISS_저작자"), strAuthor, 0);
	}

	CString strPublisher;
	arrItemData.RemoveAll();

	m_mapMarcAlias.Lookup(_T("발행자"), strMarcAlias);
	m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData);
	if (!strItemData.IsEmpty())
	{
		strPublisher = strItemData;
		INT nCount = arrItemData.GetSize();
		for (INT idx = 1; idx < nCount; idx++)
		{
			strItemData = arrItemData.GetAt(idx);
			if (strItemData.IsEmpty()) continue;

			strPublisher += _T(":") + strItemData;
		}

		pDM_To_Index->SetCellData(_T("ISS_발행자"), strPublisher, 0);
	}

	ids = MakeTemp049Marc(&marcSource, &marcOutput, strFirstInd, strDefaultShelfLocCode);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("049 Tag 만들기 실패");
		return 0;
	}
	
	if (marcOutput.m_listTag.GetCount() < 0) return 0;

	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapVolume;

	CString strUniqueKey;

	ids = pDM_To_Index->MakeRecKey(strUniqueKey);
	if (ids < 0) return ids;
	mapVolume.SetAt(_T(""), strUniqueKey);

	posTag = marcOutput.m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = reinterpret_cast<CTag*>(marcOutput.m_listTag.GetNext(posTag));
		if (!pTag) continue;

		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (!pSubfield) continue;

			strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');

			if (strSubfieldCode == _T("%v"))
			{
				mapVolume.SetAt(pSubfield->GetItemData(), strUniqueKey);
				Sleep(10);
			}
		}
	}

	INT nDMIdx = 0;
	CString strVolTitle;
	pos = mapVolume.GetStartPosition();
	while (pos)
	{
		mapVolume.GetNextAssoc(pos, strVolTitle, strUniqueKey);
		
		pDM_To_Volume->InsertRow(-1);

		ids = pDM_To_Volume->SetCellData( _T("SV_권KEY"), strUniqueKey, nDMIdx);

		ids = pDM_To_Volume->SetCellData( _T("SV_권호유형"), _T("V"), nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("245$a"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_본서명"), strItemData, nDMIdx);

		ids = pDM_To_Species->GetCellData( _T("SS_형식구분_코드"), nDMIdx, strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_형식구분_코드"), strItemData, nDMIdx);

		ids = pDM_To_Species->GetCellData( _T("SS_매체구분_코드"), nDMIdx, strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_매체구분_코드"), strItemData, nDMIdx);

		strItemData.Format(_T("%d"), (nDMIdx + 1) * 10);
		ids = pDM_To_Volume->SetCellData( _T("SV_권일련번호"), strItemData, nDMIdx);

		ids = pDM_To_Volume->SetCellData( _T("SV_권호명"), strVolTitle, nDMIdx);

		GetIndexTool()->GetIndex()->MakeIndex(strVolTitle);
		ids = pDM_To_Volume->SetCellData( _T("SV_색인권호명"), strVolTitle, nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$a"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_면장수"), strItemData, nDMIdx);
		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$b"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_물리적특성"), strItemData, nDMIdx);
		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$c"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_책크기"), strItemData, nDMIdx);
		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$e"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_딸림자료"), strItemData, nDMIdx);
		ids = pDM_To_Volume->SetCellData( _T("SV_권호구분"), _T("1"), nDMIdx);
		ids = pDM_To_Volume->SetCellData( _T("SV_목차유무"), _T("N"), nDMIdx);
		ids = pDM_To_Volume->SetCellData( _T("SV_DB_FLAG"), _T("I"), nDMIdx);

		nDMIdx++;
	}

	nDMIdx = 0;
	posTag = marcOutput.m_listTag.GetHeadPosition();
	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	while (posTag)
	{
		pTag = reinterpret_cast<CTag*>(marcOutput.m_listTag.GetNext(posTag));
		if (!pTag) continue;

		pDM_To_Book->InsertRow(-1);

		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (!pSubfield) continue;

			strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');

			if (strSubfieldCode == _T("$v"))
			{
				strItemData = pSubfield->GetItemData();
				if (mapVolume.Lookup(strItemData, strUniqueKey))
				{
					pDM_To_Book->SetCellData( _T("SB_권KEY"), strUniqueKey, nDMIdx);
				}

				pDM_To_Book->SetCellData( _T("SB_권책기호"), strItemData, nDMIdx);

			}
			else if (strSubfieldCode == _T("$l"))
			{
				strItemData = pSubfield->GetItemData();

				pDM_To_Book->SetCellData( _T("SB_등록번호"), strItemData, nDMIdx);
				pDM_To_Book->SetCellData( _T("SB_등록구분_코드"), strItemData.Left(nRegCodeLength), nDMIdx);
			}
			else if (strSubfieldCode == _T("$c"))
			{
				pDM_To_Book->SetCellData( _T("SB_복본기호"), strItemData, nDMIdx);
			}
			else if (strSubfieldCode == _T("$f"))
			{
				pDM_To_Book->SetCellData( _T("SB_배가위치부호"), strItemData, nDMIdx);
			}
		}

		if (pDM_To_Book->GetCellData( _T("SB_권KEY"), nDMIdx).IsEmpty())
		{
			if (mapVolume.Lookup(_T(""), strUniqueKey))
			{
				pDM_To_Book->SetCellData( _T("SB_권KEY"), strUniqueKey, nDMIdx);
			}
		}

		strItemData = pDM_To_Index->GetCellData( _T("ISS_수입구분_코드"), 0);
		ids = pDM_To_Book->SetCellData( _T("SB_수입구분_코드"), strItemData, nDMIdx);

		strItemData = pDM_To_Index->GetCellData( _T("ISS_수입년도"), 0);
		ids = pDM_To_Book->SetCellData( _T("SB_수입년도"), strItemData, nDMIdx);

		ids = pDM_To_Book->SetCellData( _T("SB_책종류"), _T("C"), nDMIdx);

		strItemData = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
		ids = pDM_To_Book->SetCellData( _T("SB_입력일"), strItemData, nDMIdx);

		ids = pDM_To_Book->SetCellData( _T("SB_입수일"), strItemData, nDMIdx);

		strItemData = pDM_To_Index->GetCellData( _T("ISS_분류표구분"), 0);
		ids = pDM_To_Book->SetCellData( _T("SB_분류기호구분_코드"), strItemData, nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("090$a"), strItemData);
		ids = pDM_To_Book->SetCellData( _T("SB_분류기호"), strItemData, nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("090$b"), strItemData);
		strItemData.Replace(_T("'"),_T("''"));
		ids = pDM_To_Book->SetCellData( _T("SB_도서기호"), strItemData, nDMIdx);
		
		ids = pDM_To_Book->SetCellData( _T("SB_작업자"), m_pParnetMgr->m_pInfo->USER_ID, nDMIdx);

		ids = pDM_To_Book->SetCellData( _T("SB_DB_FLAG"), _T("I"), nDMIdx);

		nDMIdx++;
	}	
		
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::UpdateSpeciesMarcbyStream(CESL_DataMgr *pDM_OUT_SPECIES, CString strTBL, CString strSpeciesKey, INT RowIdx, BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (!pDM_OUT_SPECIES) return-1;

	INT ids = -1;
	if (bCommit)
	{
		ids = pDM_OUT_SPECIES->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}

	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_OUT_SPECIES->GetCellData( _T("SS_MARC"), RowIdx, strStreamMarc);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	
	if ( bCommit ) {
		ids = m_pMarcMgr->DeleteField( &marc, _T("001") );
		ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	}

	if(strTBL == _T("BO_DELETE_SPECIES_TBL"))
	{
		ids = m_pMarcMgr->DeleteField(&marc, _T("049"));
		ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	}

	CString str049Field;
	ids = m_pMarcMgr->GetField(&marc, _T("049"), str049Field);
	if (ids >= 0)
	{
		ids = m_pMarcMgr->DeleteField(&marc, _T("049"));
		ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);

		ids = m_pMarcMgr->GetField(&marc, _T("049"), str049Field);

		CString strRecKey;
		ids = pDM_OUT_SPECIES->MakeRecKey(strRecKey);
		if (ids < 0) return ids;

		CString strQuery;
		strQuery.Format(_T("INSERT INTO SE_HOLDING_TBL (REC_KEY, HOLDING_DATA) VALUES (%s, '%s');"),
						strRecKey, _T("049") + str049Field);
		ids = pDM_OUT_SPECIES->AddFrame(strQuery);
		if (ids < 0) return ids;
		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET HOLDING_KEY = %s WHERE REC_KEY = %s;"), strRecKey, strSpeciesKey);
		ids = pDM_OUT_SPECIES->AddFrame(strQuery);
		if (ids < 0) return ids;

		pDM_OUT_SPECIES->SetCellData( _T("SS_MARC"), strStreamMarc, 0);
	}

	pDM_OUT_SPECIES->InitDBFieldData();
	pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, TRUE);
	ids = pDM_OUT_SPECIES->MakeUpdateFrame(_T("SE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	if (ids < 0) return ids;


	if (bCommit)
	{
		ids = pDM_OUT_SPECIES->SendFrame();
		pDM_OUT_SPECIES->EndFrame();
		return ids;
	}

	return 0;
	

	EFS_END
	return -1;
}

INT CCatApi_SE::MergeDM(CESL_DataMgr *pDM_Source, CESL_DataMgr *pDM_Output, LPCTSTR lpszKeyAlias, INT nMode /*= 0*/)
{
	EFS_BEGIN

	if (!pDM_Source || !pDM_Output) return -1;

	INT nSourceRowCount = pDM_Source->GetRowCount();
	INT nOutputRowCount = -1;

	CString strKey;

	BOOL bCopy = TRUE;
	INT nOutputIdx = -1;
	INT nSourceIdx = -1;

	for (nSourceIdx = 0; nSourceIdx < nSourceRowCount; nSourceIdx++)
	{
		nOutputRowCount = pDM_Output->GetRowCount();
		bCopy  = TRUE;

		if (nMode == 2)
			bCopy = TRUE;
		else
		{
			strKey = pDM_Source->GetCellData(lpszKeyAlias, nSourceIdx);
			for (nOutputIdx = 0; nOutputIdx < nOutputRowCount; nOutputIdx++)
			{
				if (strKey == pDM_Output->GetCellData(lpszKeyAlias, nOutputIdx))
				{
					if (nMode == 0)
						bCopy = FALSE;
					else if (nMode == 1)
					{
						pDM_Output->DeleteRow(nOutputIdx);
						bCopy = TRUE;
					}

					break;
				}
			}
		}

		if (bCopy)
			CopyDM(pDM_Source, nSourceIdx, pDM_Output, -1);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::SyncVolumeBookAppendixDM(CESL_DataMgr *pDM_Volume, 
										 CESL_DataMgr *pDM_Book, 
										 CESL_DataMgr *pDM_HoldBook, 
										 CESL_DataMgr *pDM_Appendix, 
										 CESL_DataMgr *pDM_HoldAppendix,
										 INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Book || !pDM_Volume || !pDM_HoldBook || !pDM_Appendix || !pDM_HoldAppendix) return -1;

	INT ids = -1;
	INT nVolIdx, nTmpIdx;
	INT nBookCountInVol, nHoldBookCountInVol, nAppendixCountInVol, nHoldAppendixCountInVol;
	INT nVolCount = pDM_Volume->GetRowCount();
	INT nBookCount = pDM_Book->GetRowCount();
	INT nHoldBookCount = pDM_HoldBook->GetRowCount();
	INT nAppendixCount = pDM_Appendix->GetRowCount();
	INT nHoldAppendixCount = pDM_HoldAppendix->GetRowCount();
	INT nTotDataCnt;

	CString strTmpCnt;

	CString strVolKey, strBookVolKey, strBookKey, strTotDataCnt;
	CString strVolDBFlag, strTmpDBFlag;
	CStringArray arrBookKeyList;
	BOOL bKeyInList;
	
	for (nVolIdx = 0; nVolIdx < nVolCount; nVolIdx++)
	{
		nBookCountInVol = 0;
		nHoldBookCountInVol = 0;
		nAppendixCountInVol = 0;
		nHoldAppendixCountInVol = 0;
		nTotDataCnt = 0;
		arrBookKeyList.RemoveAll();

		ids = pDM_Volume->GetCellData(_T("SV_권KEY"), nVolIdx, strVolKey);
		if (ids < 0 || strVolKey.IsEmpty()) continue;

		ids = pDM_Volume->GetCellData(_T("SV_총자료수"), nVolIdx, strTotDataCnt);
		if (ids < 0 || strTotDataCnt.IsEmpty()) continue;
		nTotDataCnt = _ttoi(strTotDataCnt);

		ids = pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nVolIdx, strVolDBFlag);
		if (ids < 0 ) continue;

		for (nTmpIdx = 0; nTmpIdx < nBookCount; nTmpIdx++)
		{
			strBookVolKey = pDM_Book->GetCellData(_T("SB_권KEY"), nTmpIdx);
			strTmpDBFlag = pDM_Book->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_Book->GetCellData(_T("SB_책KEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				arrBookKeyList.Add(strBookKey);
				if (strVolDBFlag==_T("D")) 
				{
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					nTotDataCnt--;
					continue;
				}

				CopyDM(pDM_Volume, nVolIdx, pDM_Book, nTmpIdx);
				if ( strTmpDBFlag == _T("") ) pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T("U"), nTmpIdx);

				if (pDM_Book->GetCellData(_T("SB_권책기호"), nTmpIdx).IsEmpty())
				{
					pDM_Book->SetCellData(_T("SB_권책기호"),  pDM_Book->GetCellData(_T("SV_권호명"), nTmpIdx), nTmpIdx);
				}
				nBookCountInVol++;
				nTotDataCnt--;
			}
		}

		for (nTmpIdx = 0; nTmpIdx < nHoldBookCount; nTmpIdx++)
		{
			strBookVolKey = pDM_HoldBook->GetCellData(_T("SB_권KEY"), nTmpIdx);
			strTmpDBFlag = pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_HoldBook->GetCellData(_T("SB_책KEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				bKeyInList = FALSE;
				for (INT i=0; i< arrBookKeyList.GetSize(); i++)
					if ( arrBookKeyList.GetAt(i) == strBookKey) bKeyInList = TRUE;
				if (!bKeyInList) arrBookKeyList.Add(strBookKey);

				if (strVolDBFlag==_T("D")) 
				{
					pDM_HoldBook->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					if (!bKeyInList) nTotDataCnt--;
					continue;
				}

				if (strTmpDBFlag ==_T("I"))
					CopyDM(pDM_Volume, nVolIdx, pDM_HoldBook, nTmpIdx);

				nHoldBookCountInVol++;
				if (!bKeyInList) nTotDataCnt--;
			}
		}

		for (nTmpIdx = 0; nTmpIdx < nAppendixCount; nTmpIdx++)
		{
			strBookVolKey = pDM_Appendix->GetCellData(_T("SB_권KEY"), nTmpIdx);
			strTmpDBFlag = pDM_Appendix->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_Appendix->GetCellData(_T("SB_책KEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				bKeyInList = FALSE;
				for (INT i=0; i< arrBookKeyList.GetSize(); i++)
					if ( arrBookKeyList.GetAt(i) == strBookKey) bKeyInList = TRUE;
				if (!bKeyInList) arrBookKeyList.Add(strBookKey);

				if (strVolDBFlag==_T("D")) 
				{
					pDM_Appendix->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					if (!bKeyInList) nTotDataCnt--;
					continue;
				}

				CopyDM(pDM_Volume, nVolIdx, pDM_Appendix, nTmpIdx);
				if ( strTmpDBFlag == _T("") ) pDM_Appendix->SetCellData(_T("SB_DB_FLAG"), _T("U"), nTmpIdx);

				if (pDM_Appendix->GetCellData(_T("SB_권책기호"), nTmpIdx).IsEmpty())
				{
					pDM_Appendix->SetCellData(_T("SB_권책기호"),  pDM_Appendix->GetCellData(_T("SV_권호명"), nTmpIdx), nTmpIdx);
				}
				nAppendixCountInVol++;
				if (!bKeyInList) nTotDataCnt--;
			}
		}

		for (nTmpIdx = 0; nTmpIdx < nHoldAppendixCount; nTmpIdx++)
		{
			strBookVolKey = pDM_HoldAppendix->GetCellData(_T("SB_권KEY"), nTmpIdx);
			strTmpDBFlag = pDM_HoldAppendix->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_HoldAppendix->GetCellData(_T("SB_책KEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				bKeyInList = FALSE;
				for (INT i=0; i< arrBookKeyList.GetSize(); i++)
					if ( arrBookKeyList.GetAt(i) == strBookKey) bKeyInList = TRUE;
				if (!bKeyInList) arrBookKeyList.Add(strBookKey);

				if (strVolDBFlag==_T("D")) 
				{
					pDM_HoldAppendix->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					if (!bKeyInList) nTotDataCnt--;
					continue;
				}

				if (strTmpDBFlag ==_T("I"))
					CopyDM(pDM_Volume, nVolIdx, pDM_HoldAppendix, nTmpIdx);

				nHoldAppendixCountInVol++;
				if (!bKeyInList) nTotDataCnt--;
			}
		}

		strTmpCnt.Format(_T("%d"), nBookCountInVol);
		pDM_Volume->SetCellData(_T("SV_책수"), strTmpCnt, nVolIdx);
		strTmpCnt.Format(_T("%d"), nHoldBookCountInVol);
		pDM_Volume->SetCellData(_T("SV_소장책수"), strTmpCnt, nVolIdx);
		strTmpCnt.Format(_T("%d"), nAppendixCountInVol);
		pDM_Volume->SetCellData(_T("SV_부록수"), strTmpCnt, nVolIdx);
		strTmpCnt.Format(_T("%d"), nHoldAppendixCountInVol);
		pDM_Volume->SetCellData(_T("SV_소장부록수"), strTmpCnt, nVolIdx);
		strTotDataCnt.Format(_T("%d"), nTotDataCnt);
		pDM_Volume->SetCellData(_T("SV_기타자료수"), strTotDataCnt, nVolIdx);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::RecordLocDate(CESL_DataMgr *pDM_Out, CStringArray *pArrSpeciesKey, CStringArray *pArrBookKey, CStringArray *pArrAppendixKey, CString &strShelfDate, BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (!pDM_Out || !pArrSpeciesKey || !pArrBookKey) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strRecKey;
	CString strQuery;

	if (bCommit)
	{
		ids = pDM_Out->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}

	nRowCount = pArrSpeciesKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrSpeciesKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		strQuery.Format(_T("UPDATE IDX_SE_TBL SET SPECIES_CLASS = '1', WORKING_STATUS = 'SEL212N' WHERE REC_KEY = %s;")
							, strRecKey);

		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET LOC_FLAG = '0' WHERE REC_KEY = %s;"), strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
		
	}

	nRowCount = pArrBookKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrBookKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		strQuery.Format(_T("UPDATE SE_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'SEL212N', SHELF_WORKER = '%s' WHERE REC_KEY = %s or BINDING_BOOK_KEY=%s;")
					, strShelfDate, m_pParnetMgr->m_pInfo->USER_ID, strRecKey, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
	}

	if (pArrAppendixKey)
	{
		nRowCount = pArrAppendixKey->GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strRecKey = pArrAppendixKey->GetAt(idx);
			if (strRecKey.IsEmpty()) continue;

			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'SEL212N', SHELF_WORKER = '%s' WHERE REC_KEY = %s or BINDING_BOOK_KEY=%s;")
					, strShelfDate, m_pParnetMgr->m_pInfo->USER_ID, strRecKey, strRecKey);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}

	if (bCommit)
	{
		ids = pDM_Out->SendFrame();
		if (ids < 0) 
		{
			m_strLastErrMsg = _T("DB에 데이터 저장 실패");
			return ids;
		}

		ids = pDM_Out->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::MakeVolumeQueryFrame(CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_HoldBook, 
		CESL_DataMgr *pDM_Appendix, CESL_DataMgr *pDM_HoldAppendix, CString strSpeciesKey, CESL_DataMgr *pDM_OUT)
{
	EFS_BEGIN

	if (!pDM_Volume || !pDM_Book || 
		!pDM_HoldBook || !pDM_Appendix || 
		!pDM_HoldAppendix || !pDM_OUT || 
		strSpeciesKey.IsEmpty()) return -1;

	INT ids = -1;
	INT idx = -1;
	CString strQuery;
	CString strVolumeKey;
	CString strQueryFlag;

	CIndex *pIndex = m_pIndexTool->GetIndex();

	CArray <CString, CString> RemoveAliasList;

	CSeApi api(m_pParnetMgr);
	CString strSPECIES_KEY, strVOL_KEY, strVOL_TYPE, strVolTitle, msg;
	CStringArray arrayChildBookKeys;

	INT nRowCount = pDM_Volume->GetRowCount();

	for (idx = 0; idx < nRowCount; idx++) {
		RemoveAliasList.RemoveAll();
		ids = pDM_Volume->GetCellData(_T("SV_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		ids = pDM_Volume->GetCellData(_T("SV_권KEY"), idx, strVolumeKey);
		if (strQueryFlag == _T("I")) 
		{
			RemoveAliasList.Add(_T("SV_총자료수"));
			pDM_Volume->SetCellData(_T("SV_종KEY"), strSpeciesKey, idx);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Volume, pDM_Volume->TBL_NAME, idx, pDM_OUT);
			LogToFile(pDM_OUT->m_lastmsg);
		}
		if (strQueryFlag == _T("U")) 
		{
			RemoveAliasList.Add(_T("SV_권KEY"));
			RemoveAliasList.Add(_T("SV_총자료수"));

			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Volume, pDM_Volume->TBL_NAME, idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strVolumeKey);
			LogToFile(pDM_OUT->m_lastmsg);
		}
		else if (strQueryFlag == _T("D")) 
		{
			strSPECIES_KEY = pDM_Volume->GetCellData(_T("SV_종KEY"),idx);
			strVOL_KEY = pDM_Volume->GetCellData(_T("SV_권KEY"),idx);
			strVOL_TYPE = pDM_Volume->GetCellData(_T("SV_권호유형"),idx);
			strVolTitle = pDM_Volume->GetCellData(_T("SV_권호명"),idx);

			INT i, nCount;
			nCount = pDM_Book->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_Book->GetCellData(_T("SB_권KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_Book->GetCellData(_T("SB_책KEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_소정추가
				}
			}
			nCount = pDM_HoldBook->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_HoldBook->GetCellData(_T("SB_권KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_HoldBook->GetCellData(_T("SB_책KEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_소정추가
				}
			}
			nCount = pDM_Appendix->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_Appendix->GetCellData(_T("SB_권KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_Appendix->GetCellData(_T("SB_책KEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_소정추가
				}
			}
			nCount = pDM_HoldAppendix->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_HoldAppendix->GetCellData(_T("SB_권KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_HoldAppendix->GetCellData(_T("SB_책KEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_소정추가
				}
			}

			ids = api.VOL_CAT_DELETE(pDM_OUT,
					   strSPECIES_KEY,
					   strVOL_KEY,
					   strVOL_TYPE,
					   strVolTitle,
					   arrayChildBookKeys,
					   TRUE, FALSE);
			if(ids<0)
			{
				if(-1002 == ids)
				{
					msg.Format(_T("[ %s ] 자료를 삭제할 수 없습니다!"), strVolTitle);
					AfxMessageBox(msg);
					continue;
				}
				else
				{
					msg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"), strVolTitle);
					AfxMessageBox(msg);
					return -200;
				}
			}

			LogToFile(pDM_OUT->m_lastmsg);
		}
	}	

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::GetBookCount(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Appendix, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL || pDM_Appendix == NULL) return -1;

	INT nReturnCnt = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"), lpszVol)) continue;

		if (pDM_Book->GetCellData( _T("SB_DB_FLAG"), idx) == _T("D")) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( _T("SB_등록구분_코드"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	nRowCount = pDM_Appendix->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Appendix, idx, _T("SB"), lpszVol)) continue;

		if (pDM_Appendix->GetCellData( _T("SB_DB_FLAG"), idx) == _T("D")) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Appendix->GetCellData( _T("SB_등록구분_코드"), idx) != strRegCode) continue;

		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

INT CCatApi_SE::InitLastNumDM()
{
	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_연속_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("[DM_마지막번호]의 초기화에 실패하였습니다") );
		return -1;
	}
	pDmLastNum->FreeData();

	return 0;
}

INT CCatApi_SE::InsertAppendixToDM(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Appendix
							   , INT nVolIndex, CString strWorkingStatus, INT nDupNumber
							   , CString strRegCode, CString strAcqDate, CString strShelfLocCode, CString strRemark
							   , CString strAppendixName, CString strAppFormCode, CString strMediaCode)
{
	INT nAppDMIdx = InsertBookToDM(pDM_Species, pDM_Volume, pDM_Appendix, nVolIndex
			, strWorkingStatus, nDupNumber, strRegCode, strAcqDate, strShelfLocCode, strRemark );
	if ( nAppDMIdx<0 ) 
		return nAppDMIdx;
	else 
	{
		CString strItemData;
		INT ids;

		ids = pDM_Appendix->MakeRecKey(strItemData);
		if (ids < 0) return ids;
		pDM_Appendix->SetCellData( _T("SA_부록KEY"), strItemData, nAppDMIdx);
		pDM_Appendix->SetCellData( _T("SA_책KEY"), pDM_Appendix->GetCellData(_T("SB_책KEY"), nAppDMIdx), nAppDMIdx);
		pDM_Appendix->SetCellData( _T("SA_부록명"), strAppendixName, nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_형식구분"), strAppFormCode, nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_매체구분"), strMediaCode, nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_단행본등록여부"), _T("N"), nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_폐기여부"), _T("N"), nAppDMIdx );

		pDM_Appendix->SetCellData( _T("SB_책종류"), _T("A"), nAppDMIdx );

	}

	return nAppDMIdx;
}

INT CCatApi_SE::InsertBookToDM(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Book
							   , INT nVolIndex, CString strWorkingStatus, INT nDupNumber
							   , CString strRegCode, CString strAcqDate, CString strShelfLocCode, CString strRemark)
{
	if (pDM_Species == NULL ||
		pDM_Volume == NULL ||
		pDM_Book == NULL ||
		nVolIndex < 0 ||
		strWorkingStatus.GetLength() == 0)
		return -1;

	INT ids;
	CString strItemData;

	ids = pDM_Book->InsertRow(-1);
	INT nBookIndex = pDM_Book->GetRowCount() -1;

	ids = pDM_Book->MakeRecKey(strItemData);
	if (ids < 0) return ids;
	pDM_Book->SetCellData( _T("SB_책KEY"), strItemData, nBookIndex);

	pDM_Book->SetCellData( _T("SB_종KEY"), pDM_Species->GetCellData( _T("SS_종KEY"), 0), nBookIndex );
	pDM_Book->SetCellData( _T("SB_권KEY"), pDM_Volume->GetCellData( _T("SV_권KEY"), nVolIndex), nBookIndex );

	strItemData = _T("");
	strItemData = CTime::GetCurrentTime().Format(_T("%Y"));
	pDM_Book->SetCellData( _T("SB_수입년도"), strItemData, nBookIndex );

	pDM_Book->SetCellData( _T("SB_책종류"), _T("C"), nBookIndex );
	pDM_Book->SetCellData( _T("SB_제본책여부"), _T("N"), nBookIndex );

	if (strRegCode.GetLength() > 0)
		pDM_Book->SetCellData( _T("SB_등록구분_코드"), strRegCode, nBookIndex );
	else pDM_Book->SetCellData( _T("SB_등록구분_코드"), pDM_Species->GetCellData( _T("SS_등록구분_코드"), 0), nBookIndex );

	pDM_Book->SetCellData( _T("SB_이용제한구분_코드"), pDM_Species->GetCellData( _T("SS_이용제한구분_코드"), 0), nBookIndex );
	pDM_Book->SetCellData( _T("SB_관리구분_코드"), pDM_Species->GetCellData( _T("SS_관리구분_코드"), 0), nBookIndex );

	strItemData = _T("");
	strItemData = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	pDM_Book->SetCellData( _T("SB_입력일"), strItemData, nBookIndex );

	if (strAcqDate.GetLength() > 0)
		pDM_Book->SetCellData( _T("SB_입수일"), strAcqDate, nBookIndex );
	else pDM_Book->SetCellData( _T("SB_입수일"), strItemData, nBookIndex );

	pDM_Book->SetCellData( _T("SB_인계여부"), _T("Y"), nBookIndex );

	if (strShelfLocCode.GetLength() > 0)
		pDM_Book->SetCellData( _T("SB_배가위치부호"), strShelfLocCode, nBookIndex );
	else pDM_Book->SetCellData( _T("SB_배가위치부호"), pDM_Species->GetCellData( _T("SS_배가위치_코드"), 0), nBookIndex );

	pDM_Book->SetCellData( _T("SB_분류기호구분_코드"), pDM_Species->GetCellData( _T("SS_분류표구분"), 0), nBookIndex );

	if (nDupNumber == 0) strItemData = _T("");
	else strItemData.Format(_T("%d"), nDupNumber);
	pDM_Book->SetCellData( _T("SB_복본기호"), strItemData, nBookIndex );

	pDM_Book->SetCellData( _T("SB_레이블출력구분"), _T("1"), nBookIndex );
	pDM_Book->SetCellData( _T("SB_자료상태"), strWorkingStatus, nBookIndex );
	pDM_Book->SetCellData( _T("SB_비고"), strRemark, nBookIndex );
	pDM_Book->SetCellData( _T("SB_작업자"), m_pParnetMgr->m_pInfo->USER_ID, nBookIndex );
	pDM_Book->SetCellData( _T("SB_장서점검"), _T("N"), nBookIndex );
	pDM_Book->SetCellData( _T("SB_DB_FLAG"), _T("I"), nBookIndex );


	return nBookIndex;
}

INT CCatApi_SE::ConvertCatDMToMarc(CMarc *pMarc, CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Purchase /*= NULL*/)
{
	EFS_BEGIN

	if (pMarc == NULL ||
		pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL ) return -1;

	INT ids = -1;
	
	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strItemData;
	CArray<CString, CString&> arrItemData;

	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		pDM_Index->GetCellData(strDMFieldAlias, 0, strItemData);

		if (strItemData.IsEmpty()) continue;

		m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData, _T(""), MODIFY_ITEM);
	}

	CString strCreateDate ;
    m_pMarcMgr->GetItem(pMarc,_T("008*"),strCreateDate) ;
    if(!strCreateDate.IsEmpty ())
    {   
		ids = -1;
		
		ids = m_pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
		strItemData.TrimLeft();
		strItemData.TrimRight();
		if (ids < 0 || strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, _T("008@0-5"), CTime::GetCurrentTime().Format(_T("%y%m%d")));

		ids = m_pMarcMgr->GetItem(pMarc, _T("008@6"), strItemData);
		if (ids < 0 || strItemData.IsEmpty())
		{
			strItemData = _T("c");
			m_pMarcMgr->SetItem(pMarc, _T("008@6"), strItemData);
		}
		
		if (strItemData == _T("c"))
		{
			ids = m_pMarcMgr->GetItem(pMarc, _T("260$c"), strItemData);
			
			CString strYear;
			INT nLength = strItemData.GetLength();
			TCHAR tch = 0;
			for (INT idx = 0; idx < nLength; idx++)
			{
				tch = strItemData.GetAt(idx);
				if (_istdigit(tch))
					strYear += tch;
			}
			
			if (ids < 0 || strYear.IsEmpty())
				strYear = CTime::GetCurrentTime().Format(_T("%Y"));
			
			m_pMarcMgr->SetItem(pMarc, _T("008@7-10"), strYear);
		}
    }

	strItemData = pDM_Index->GetCellData(_T("ISS_저작자"), 0);
	if (!strItemData.IsEmpty())
	{
		m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
		m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);
		m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
		m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);

		INT n = 0;

		LPTSTR lpszTok = _tcstok(strItemData.GetBuffer(0), _T(";"));
		while (lpszTok)
		{
			if (n == 0)
				m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
			else
				m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
			
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, lpszTok);
			n++;
			lpszTok = _tcstok(NULL, _T(";"));
		}
	}
	
	strItemData = pDM_Index->GetCellData(_T("ISS_발행자"), 0);
	if (!strItemData.IsEmpty())
	{
		m_mapMarcAlias.Lookup(_T("발행자"), strMarcAlias);
		m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);

		LPTSTR lpszTok = _tcstok(strItemData.GetBuffer(0), _T(":"));
		while (lpszTok)
		{
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, lpszTok);

			lpszTok = _tcstok(NULL, _T(":"));
		}
	}

	m_pMarcMgr->CheckSimpleSyntax(pMarc);
	
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ConvertMarcToCatDM(CMarc *pMarc, CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Purchase /*= NULL*/)
{
	EFS_BEGIN

	if (pMarc == NULL ||
		pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL
		) return -1;

	INT ids = -1;
	INT nVolCount = -1;

	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strItemData;
	CArray<CString, CString&> arrItemData;

	CString sStreamMarc;
	ids = m_pMarcMgr->Encoding(pMarc, sStreamMarc);
	if (ids<0 || sStreamMarc.GetLength() <=0 ) return -1;
	ids = pDM_Species->SetCellData(_T("SS_MARC"), sStreamMarc, 0);
	if (ids<0) return -1;

	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		if (ids < 0) continue;

		pDM_Index->SetCellData(strDMFieldAlias, strItemData, 0);		
	}

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;

	CString strTitle;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strSubfieldCode;
	posTag = pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)pMarc->m_listTag.GetNext(posTag);
		if (pTag && pTag->GetTagCode() == _T("245"))
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$b"))
				{
					strTitle += pSubfield->GetPunctuationMark() + pSubfield->GetItemData();
				}
			}
		}
	}

	pDM_Index->SetCellData(_T("ISS_본표제"), strTitle, 0);
		
	m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
	m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);

	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData, &arrItemData);
		if (ids >= 0)
		{
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;

				strAuthor += _T(";") + strItemData;
			}
		}

		pDM_Index->SetCellData(_T("ISS_저작자"), strAuthor, 0);
	}

	CString strPublisher;
	arrItemData.RemoveAll();

	m_mapMarcAlias.Lookup(_T("발행자"), strMarcAlias);
	m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData, &arrItemData);

	{
		strPublisher = strItemData;
		INT nCount = arrItemData.GetSize();
		for (INT idx = 1; idx < nCount; idx++)
		{
			strItemData = arrItemData.GetAt(idx);
			if (strItemData.IsEmpty()) continue;

			strPublisher += _T(":") + strItemData;
		}

		pDM_Index->SetCellData(_T("ISS_발행자"), strPublisher, 0);
	}

	return 0;

	EFS_END
	return -1;
}






