// CatApi_BO.cpp: implementation of the CCatApi_BO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CatApi_BO.h"

#include "..\BO_SPECIES_API\BO_SPECIES.h"

CString g_strMarcVersion;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatApi_BO::CCatApi_BO(CESL_Mgr *pParentMgr) 
	: CCatApi(pParentMgr)
{
	LoadRegCode();
	Init();

	g_strMarcVersion = _T("DEFAULT");

}

CCatApi_BO::~CCatApi_BO()
{

}

INT CCatApi_BO::InitMemberData()
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
	m_mapMarcAlias.SetAt(_T("총서편차"), _T("490$a"));
	m_mapMarcAlias.SetAt(_T("가격"), _T("950$b"));
	m_mapMarcAlias.SetAt(_T("가격잡정보"), _T("950$c"));
	m_mapMarcAlias.SetAt(_T("면장수"), _T("300$a"));
	m_mapMarcAlias.SetAt(_T("물리적특성"), _T("300$b"));
	m_mapMarcAlias.SetAt(_T("크기"), _T("300$c"));
	m_mapMarcAlias.SetAt(_T("딸림자료"), _T("300$e"));

	m_mapCatDMtoMarc.SetAt(_T("IBS_판사항"), _T("250$a"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_발행지"), _T("260$a"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_발행자"), _T("260$b"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_발행년"), _T("260$c"));


	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용저작자"), _T("저작자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_범위검색용발행년도"), _T("범위검색용발행년도"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용범위검색발행자"), _T("범위검색용발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용정렬용본표제"), _T("범위검색용본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용키워드"), _T("키워드파일"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용발행자"), _T("발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용본표제"), _T("본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_표제사항"), _T("표제사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_저작자사항"), _T("저작자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_발행자사항"), _T("발행자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_발행년도사항"), _T("발행년도사항"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_주제명파일"), _T("주제명파일"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_판종색인"), _T("판종색인"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_판종"), _T("판종"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_도서기호"), _T("기타부호_청구기호_도서기호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_권책기호"), _T("기타부호_청구기호_권책기호"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_청구_권책DISP"), _T("기타부호_청구기호_권책기호_DISP"));

	m_mapWorkingStatusCodeDesc.SetAt(_T("BOT111O"), _T("기초자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOT112O"), _T("반입자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOA111N"), _T("구입대상자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOA112N"), _T("주문자료(검수대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOA113O"), _T("미납품자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOA114O"), _T("(구입)인계대상자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOA211O"), _T("기증접수자료(인계대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOA212O"), _T("기증부당자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOR111N"), _T("등록인계자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOR112N"), _T("등록자료(정리인계대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOR113O"), _T("가원부번호부여자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOC111O"), _T("복본조사대상자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOC112O"), _T("분류대상자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOC113N"), _T("정리대상자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOC114O"), _T("열람인계대상자료(정리완료자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL111O"), _T("열람인계자료(배가대상자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL112N"), _T("배가자료(소장자료)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL113O"), _T("수리제본자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL114O"), _T("파손자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL115O"), _T("제적자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL116O"), _T("재정리자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL252N"), _T("분실자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL211O"), _T("관외대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL212O"), _T("관내대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL213O"), _T("특별대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL214O"), _T("긴급대출자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL215O"), _T("배달자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL217O"), _T("장기대출자료"));

	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL218O"), _T("상호대차대출자료"));

	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL219O"), _T("책나래대출자료"));

	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL411O"), _T("단행책두레업무진행자료"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL511O"), _T("통합도서타관반납"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("BOL611O"), _T("통합도서타관대출"));

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

INT CCatApi_BO::AddBook(CESL_DataMgr *pDM_Book, 
						CString strRegCode, 
						CString strVol, 
						INT nCount /*= 1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT ids = -1;

	INT nStandardRowIdx = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"), strVol)) continue;

		nStandardRowIdx = idx;
		break;
	}

	CStringArray arrIgnoreAlias;
	arrIgnoreAlias.Add(_T("BB_대표책여부"));
	arrIgnoreAlias.Add(_T("BB_등록번호"));
	arrIgnoreAlias.Add(_T("BB_복본기호"));
	arrIgnoreAlias.Add(_T("BB_배가위치부호"));
	arrIgnoreAlias.Add(_T("BB_DB_FLAG"));
	arrIgnoreAlias.Add(_T("BB_별치기호_코드"));
	
	CString strQueryFlag;
	CString strDupCode;
	INT nNewRow = -1;
	for (idx = 0; idx < nCount; idx++)
	{
		nNewRow = CopyDM(pDM_Book, nStandardRowIdx, pDM_Book, -1, _T(""), &arrIgnoreAlias);
		if (nNewRow < 0) continue;

		pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, nNewRow);

		pDM_Book->SetCellData(_T("BB_등록번호"), _T(""), nNewRow);

		pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), nNewRow);

		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), nNewRow);

		INT nDupNumber = GetLastDupCodeNumber(pDM_Book, strVol);
		if (nDupNumber < 0) continue;
		strDupCode.Format(_T("%d"), ++nDupNumber);
		pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, nNewRow);		
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ApplyAppendixRegNo(CESL_DataMgr *pDM_Appendix,
								   INT nAppMode)
{
	EFS_BEGIN

	if (pDM_Appendix == NULL) return -1;

	CString strQuery =	_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='기타' AND GROUP_3='공통' ")
						_T("AND CLASS_ALIAS='부록등록번호자동부여여부' AND MANAGE_CODE=UDF_MANAGE_CODE");

	CString strValue;
	pDM_Appendix->GetOneValueQuery(strQuery, strValue);
	if (strValue != _T("Y")) return 0;

	INT nRowCount = pDM_Appendix->GetRowCount();
	if (nRowCount <= 0) return 0;

	INT ids = -1;
	CString strRecKey;
	CString strResult;
	CString strRegCode;
	CString strRegNo;
	CString strQueryFlag;
	INT nNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM_Appendix->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids >= 0 && !strRegNo.IsEmpty()) continue;

		ids = pDM_Appendix->GetCellData( _T("BB_등록구분_코드"), idx, strRegCode);
		if (ids < 0 || strRegCode.IsEmpty()) continue;

		strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
						_T("WHERE KIND_CODE = 'REG_NO' AND ")
						_T("DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strRegCode);
		ids = pDM_Appendix->GetOneValueQuery(strQuery, strResult);

		if (ids < 0 || strResult.IsEmpty())
		{
			strResult = _T("0");

			INT ids = pDM_Appendix->StartFrame();
			if ( ids < 0 ) 
			{
				AfxMessageBox(_T("StartFrame 를 확인하세요"));
				return -1;
			}

			ids = pDM_Appendix->MakeRecKey(strRecKey);
			if (ids < 0) return ids;

			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
							_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, MANAGE_CODE) ")
							_T("VALUES (%s, '%s', '%s', '%s', '%s', %s, UDF_MANAGE_CODE);")
							, strRecKey, _T("CO"), _T("REG_NO"), strRegCode, CTime::GetCurrentTime().Format(_T("%Y")), strResult);

			ids = pDM_Appendix->AddFrame(strQuery);
			if (ids < 0) return ids;
			ids = pDM_Appendix->SendFrame();
			if (ids < 0) return ids;
			ids = pDM_Appendix->EndFrame();
			if (ids < 0) return ids;
		}
		nNumber = _ttoi(strResult) + 1;
		
		CString strTemp;
		strTemp.Format(_T("%d"),nNumber);
		strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegCode, strTemp);
	
		ids = pDM_Appendix->SetCellData( _T("BB_등록번호"), strRegNo, idx);

		INT ids = pDM_Appendix->StartFrame();

		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}

		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET ")
						_T("LAST_NUMBER=%d WHERE KIND_CODE='REG_NO' AND DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
						, nNumber, strRegCode );

		ids = pDM_Appendix->AddFrame(strQuery);
		if (ids < 0) return ids;
		ids = pDM_Appendix->SendFrame();
		if (ids < 0) return ids;
		ids = pDM_Appendix->EndFrame();
		if (ids < 0) return ids;

		ids = pDM_Appendix->GetCellData( _T("BB_DB_FLAG"), idx, strQueryFlag);
		strQueryFlag= GetQueryFlag(MNG_MODIFY, nAppMode, strQueryFlag);

		if ( strQueryFlag != _T("") ) {
			ids = pDM_Appendix->SetCellData( _T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ApplyDefaultValue_Species(CESL_DataMgr *pDM_Species, 
										  CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Species == NULL) return -1;
	
	if (pDM_Species->GetCellData( _T("BS_수입년도"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("BS_수입년도"), CTime::GetCurrentTime().Format(_T("%Y")), 0);
	}

	pDM_Species->SetCellData( _T("BS_작업자"), m_pParnetMgr->m_pInfo->USER_ID, 0);

	if (pDM_Species->GetCellData( _T("BS_이용제한구분_코드"), 0).IsEmpty())
	{
		CString strUseLitmitCode;
		GetSpeciesUseLimitCode(pDM_Book, strUseLitmitCode, _T("BB"));
		pDM_Species->SetCellData( _T("BS_이용제한구분_코드"), strUseLitmitCode, 0);
	}

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
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
	pDM_Species->SetCellData( _T("BS_KDC분류기호"), strClass, 0);
	pDM_Species->SetCellData( _T("BS_KDC버전"), strVersion, 0);

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
	pDM_Species->SetCellData( _T("BS_DDC분류기호"), strClass, 0);
	pDM_Species->SetCellData( _T("BS_DDC버전"), strVersion, 0);

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
	pDM_Species->SetCellData( _T("BS_LC분류기호"), strClass, 0);
	
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

INT CCatApi_BO::ApplyDefaultValue_Index(CESL_DataMgr *pDM_Species, 
										CESL_DataMgr *pDM_Index, 
										CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Index == NULL || pDM_Book == NULL) return -1;

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
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
	
	ids = pDM_Index->SetCellData( _T("IBS_분류기호"), strClassCode, 0);

	ids = pDM_Index->SetCellData( _T("IBS_도서기호"), strBookCode, 0);

	CString strClass;
	pDM_Species->GetCellData( _T("BS_KDC분류기호"), 0, strClass);
	pDM_Index->SetCellData( _T("IBS_분류기호KDC"), strClass, 0);

	pDM_Species->GetCellData( _T("BS_DDC분류기호"), 0, strClass);
	pDM_Index->SetCellData( _T("IBS_분류기호DDC"), strClass, 0);

	pDM_Species->GetCellData( _T("BS_LC분류기호"), 0, strClass);
	pDM_Index->SetCellData( _T("IBS_분류기호LC"), strClass, 0);

	INT nRepresentationBookIdx = -1;
	ids = GetRepresentationBook(pDM_Book, _T("BB"), nRepresentationBookIdx);
	if (ids < 0)
		nRepresentationBookIdx = SetRepresentationBook(pDM_Book, _T("BB"));

	ids = pDM_Index->SetCellData(_T("IBS_별치기호_코드"), pDM_Book->GetCellData(_T("BB_별치기호_코드"), nRepresentationBookIdx), 0);
	
	CString strVolCode090 = _T("");
	m_pMarcMgr->GetItem(&marc, _T("090$c"), strVolCode090);
	if ( strVolCode090 != _T("") )
		ids = pDM_Index->SetCellData(_T("IBS_권책기호"), strVolCode090, 0);
	else 
		ids = pDM_Index->SetCellData(_T("IBS_권책기호"), pDM_Book->GetCellData(_T("BB_권책기호"), nRepresentationBookIdx), 0);

	CString strICS, strSTANDARD;
	CIndex *pIndex = m_pIndexTool->GetIndex();

	m_pMarcMgr->GetField(&marc, _T("091"), strICS);
	if ( strICS != _T("") )
	{
		strICS = strICS.Mid( 4 );
		CString strSubfieldCode;
		strSubfieldCode.Format(_T("%ca"), SUBFIELD_CODE);
		strICS.Replace( strSubfieldCode , _T(" "));

		pIndex->MakeIndex(strICS);
		pIndex->ConvertHanjaToHangul ( strICS );

		pDM_Index->SetCellData(_T("IBS_ICS"), strICS, 0 );
	}	
	
	
	m_pMarcMgr->GetField(&marc, _T("085"), strSTANDARD);
	if ( strSTANDARD != _T("") )
	{	
		strSTANDARD = strSTANDARD.Mid( 4 );
		CString strSubfieldCode;
		strSubfieldCode.Format(_T("%ca"), SUBFIELD_CODE);
		strSTANDARD.Replace( strSubfieldCode , _T(" "));

		INT nPosSpace = strSTANDARD.Find(_T(" "));
		if ( nPosSpace < 0 ) 
		{
			pIndex->MakeIndex(strSTANDARD);
			pIndex->ConvertHanjaToHangul ( strSTANDARD );
			pDM_Index->SetCellData(_T("IBS_STANDARDNO"), strSTANDARD, 0 );

		}
		else if ( nPosSpace > 0 )
		{
			CString strSTANDARDNO_TOTAL;
			CString strSTANDARDNO_LASTITEM;
			strSTANDARDNO_TOTAL.Empty();

			CString strFieldTerminator;
			strFieldTerminator.Format(_T("%c"), FIELD_TERMINATOR);
			strSTANDARD.Replace( strFieldTerminator , _T(""));
			
			CString strTmp;
			strTmp = strSTANDARD;
			pIndex->MakeIndex(strTmp);
			pIndex->ConvertHanjaToHangul ( strTmp );
			strSTANDARDNO_TOTAL = strSTANDARDNO_TOTAL + strTmp ;

			strTmp = strSTANDARD;
			INT nPosSpace = strSTANDARD.Find(_T(" "));
			while ( nPosSpace > 0 )
			{
				strTmp = strTmp.Mid( nPosSpace+1 );
				nPosSpace = strTmp.Find(_T(" "));
			}
			strSTANDARDNO_LASTITEM = strTmp;
			pIndex->MakeIndex(strSTANDARDNO_LASTITEM);
			pIndex->ConvertHanjaToHangul ( strSTANDARDNO_LASTITEM );

			strSTANDARDNO_TOTAL = strSTANDARDNO_TOTAL + _T(" ") + strSTANDARDNO_LASTITEM ;

			CString strItemData;
			CString strLeftData;
			strTmp = strSTANDARD;
			nPosSpace = strSTANDARD.Find(_T(" "));
			while ( nPosSpace > 0 )
			{
				strLeftData = strTmp.Left( nPosSpace );
				if ( strLeftData == strSTANDARDNO_LASTITEM ) break;

				strItemData = strLeftData + strSTANDARDNO_LASTITEM ;

				pIndex->MakeIndex(strItemData);
				pIndex->ConvertHanjaToHangul ( strItemData );
				strSTANDARDNO_TOTAL = strSTANDARDNO_TOTAL + _T(" ") + strItemData ;

				if ( strTmp != _T("") ) 
					strTmp = strTmp.Mid( nPosSpace+1 );

				nPosSpace = strTmp.Find(_T(" "));

			}
			pDM_Index->SetCellData(_T("IBS_STANDARDNO"), strSTANDARDNO_TOTAL, 0 );

		}
	}

	CString strSTANDARD_INFO;
	m_pMarcMgr->GetField(&marc, _T("085"), strSTANDARD_INFO);
	if ( strSTANDARD_INFO != _T("") )
	{
		strSTANDARD_INFO = strSTANDARD_INFO.Mid(4);
		strSTANDARD_INFO.Replace(SUBFIELD_CODE, '$');
		strSTANDARD_INFO.Replace(_T("$a"), _T(" "));
		strSTANDARD_INFO.Replace( FIELD_TERMINATOR , ' ');

		pDM_Index->SetCellData(_T("STANDARDNO_INFO"), strSTANDARD_INFO, 0 );
	}

	CString sPriceChar;
	ids = m_pMarcMgr->GetItem(&marc, _T("950$a"), sPriceChar);
	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		pDM_Book->SetCellData( _T("BB_가격성격"), sPriceChar, i );
	}

	if (g_strMarcVersion.CompareNoCase(_T("DEFALUT")) == 0)
	{
		CESL_DataMgr DM;
		DM.SetCONNECTION_INFO(pDM_Index->CONNECTION_INFO);
		
		CString strQuery;
		CString strValue;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='통합마크개정' AND VALUE_NAME='그룹명' AND MANAGE_CODE='%s'"), m_pParnetMgr->m_pInfo->MANAGE_CODE);
		DM.GetOneValueQuery(strQuery, strValue);
		
		g_strMarcVersion = strValue;
	}
	
	CString strVersion;
	if (g_strMarcVersion.CompareNoCase(_T("통합마크_개정1")) == 0)
	{
		strVersion = _T("2014");
	}
	pDM_Index->SetCellData(_T("IBS_마크버전"), strVersion, 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ApplyDefaultValue_Book(CESL_DataMgr *pDM_Book, 
									   CESL_DataMgr *pDM_Index)
{
	EFS_BEGIN

	if (pDM_Book == NULL || pDM_Index == NULL) return -1;

	INT ids = -1;
	CIndex *pIndex = m_pIndexTool->GetIndex();
	CString strItem;
	CString strAcqCode = pDM_Index->GetCellData( _T("IBS_수입구분_코드"), 0);
	CString strInpuDate = GetToday();

	CString strClassCode = pDM_Index->GetCellData( _T("IBS_분류기호"), 0);
	CString strBookCode = pDM_Index->GetCellData( _T("IBS_도서기호"), 0);

	CString strClass;
	ids = pDM_Index->GetCellData( _T("IBS_분류표구분"), 0, strClass);
	ids = pDM_Book->SetCellData( _T("BB_분류기호구분_코드"), strClass, 0);

	INT nRowCount = pDM_Book->GetRowCount();

	CString sAccessionRecKey;
	CString sClassNo;
	CString sBookCode;
	CString sClassNoType;
	for ( INT i = 0 ; i < nRowCount ; i++ ) {
		CString sRepresentBookYn = pDM_Book->GetCellData( _T("BB_대표책여부"), i );
		if ( sRepresentBookYn == _T("Y") ) {
			sAccessionRecKey = pDM_Book->GetCellData( _T("BB_원부KEY"), i );
			sClassNo = pDM_Book->GetCellData( _T("BB_분류기호"), i );
			sClassNoType = pDM_Book->GetCellData( _T("BB_분류기호구분_코드"), i );
			sBookCode = pDM_Book->GetCellData( _T("BB_도서기호"), i );
			break;
		}
	}

	CESL_DataMgr *pDM_Appendix;	

	pDM_Appendix = m_pParnetMgr->FindDM(_T("DM_단행_정리DM_부록정보"));
	if ( pDM_Appendix == NULL ) return -1;	
		
	for ( i = 0 ; i < pDM_Appendix->GetRowCount(); i++ ) {

		CString sAccKey = pDM_Appendix->GetCellData( _T("BB_원부KEY"), i );
		if ( sAccKey == _T("") ) {
			pDM_Appendix->SetCellData( _T("BB_원부KEY"), sAccessionRecKey, i );
		}
		pDM_Appendix->SetCellData( _T("BB_분류기호"), strClassCode, i );
		pDM_Appendix->SetCellData( _T("BB_도서기호"), strBookCode, i );
		pDM_Appendix->SetCellData( _T("BB_분류기호구분_코드"), sClassNoType, i );	
	}

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		if (pDM_Book->GetCellData( _T("BB_수입구분_코드"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("BB_수입구분_코드"), strAcqCode, idx);
		
		if (pDM_Book->GetCellData( _T("BB_입력일"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("BB_입력일"), strInpuDate, idx);
	
		if (pDM_Book->GetCellData( _T("BB_등록일"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("BB_등록일"), strInpuDate, idx);

		ids = pDM_Book->GetCellData( _T("BB_대표책여부"), idx, strItem);
		if (ids < 0 || strItem.CompareNoCase(_T("Y")) != 0)
			pDM_Book->SetCellData( _T("BB_대표책여부"), _T("N"), idx);

		if (pDM_Book->GetCellData( _T("BB_가격"), idx).IsEmpty()) 
			pDM_Book->SetCellData( _T("BB_가격"), _T("0"), idx);

		ids = pDM_Book->GetCellData( _T("BB_낱권ISBN"), idx, strItem);
		pIndex->MakeIndex(strItem);
		pDM_Book->SetCellData( _T("BB_낱권ISBN"), strItem, idx);

		ids = pDM_Book->SetCellData( _T("BB_분류기호"), strClassCode, idx);
		
		ids = pDM_Book->SetCellData( _T("BB_도서기호"), strBookCode, idx);

		CString strItemData = pDM_Index->GetCellData( _T("IBS_분류표구분"), 0);
		ids = pDM_Book->SetCellData( _T("BB_분류기호구분_코드"), strItemData, idx);

		CString sTmpFirstWork;
		pDM_Book->GetCellData(_T("FIRST_WORK"), idx, sTmpFirstWork);
		if ( sTmpFirstWork == _T("") )
		{
			pDM_Book->SetCellData(_T("FIRST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);
		}
		pDM_Book->SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG), idx);

		ids = pDM_Book->GetCellData(_T("BB_편제명부록명"), idx, strItem);
		if (strItem.GetLength() != 0)
		{
			m_pIndexTool->GetIndex()->MakeIndex(strItem);
		}
		pDM_Book->SetCellData(_T("BB_편제명부록명색인"), strItem, idx);

		ids = pDM_Book->GetCellData(_T("BB_편권차"), idx, strItem);
		if (strItem.GetLength() != 0)
		{
			m_pIndexTool->GetIndex()->MakeIndex(strItem);
		}
		pDM_Book->SetCellData(_T("BB_편권차색인"), strItem, idx);
	}	

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ApplyExtraQuery(CESL_DataMgr *pDM_Species, 
								CESL_DataMgr *pDM_Index, 
								CESL_DataMgr *pDM_Book, 
								CString &strSpeciesKey, 
								CStringArray *pArrStrAddFrame /*= NULL*/)
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
	mapFuncName3Fields.SetAt(_T("IDX_VOL_TITLE"), _T("VOL_TITLE_INDEX"));
	mapFuncName3Fields.SetAt(_T("IDX_SE_SHELF_CODE"), _T("SEPARATE_SHELF_CODE"));
	mapFuncName3Fields.SetAt(_T("IDX_VOL"), _T("VOL_INDEX"));

	CString strQuery;
	CString strSetQuery;

	CString strTemp;

	strTemp.Format(_T("IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	strSetQuery += strTemp;

	strTemp.Format(_T(", MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	strSetQuery +=  strTemp;

	CString strField;
	CString strParam;
	POSITION pos = mapFuncName3Fields.GetStartPosition();
	while (pos)
	{
		mapFuncName3Fields.GetNextAssoc(pos, strField, strParam);
		
		strTemp.Format(_T(", %s = GET_BO_BOOK_INFO(%s, '%s')"), strField, strSpeciesKey, strParam);
		strSetQuery += strTemp;
	}

	strQuery.Format(_T("UPDATE %s SET %s WHERE REC_KEY = %s;"), pDM_Index->TBL_NAME, strSetQuery, strSpeciesKey);
	INT ids = pDM_Species->AddFrame(strQuery);
	if (ids < 0) return ids;

	CString strRecKey;
	ids = pDM_Species->MakeRecKey(strRecKey);
	if (ids < 0) return ids;

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

INT CCatApi_BO::ApplyIndex(CESL_DataMgr *pDM_Species, 
						   CESL_DataMgr *pDM_Index, 
						   CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book) return -1;

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc;
	strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
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

		INT nCheck;
		nCheck = strIndexToolKitAlias.Find(_T("도서기호"));
		if (ids < 0 || nCheck >= 0 )
		{
			continue;
		}
		pDM_Index->SetCellData(strIndexDMFieldAlias, strIndexItem, 0);
	}

	CString strItemData;

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_저작자"), 0, strItemData);
	if (strItemData.IsEmpty())
	{
	
		m_pMarcMgr->GetItem(&marc, _T("245$d"), strItemData);
		DeleteRoleWord(strItemData);
		pDM_Index->SetCellData( _T("IBS_저작자"), strItemData, 0);

		if (strItemData.IsEmpty()) 
		{
			CString strFormCode;
			pDM_Index->GetCellData( _T("IBS_형식구분_코드"), 0, strFormCode);
			if (strFormCode == _T("BK")) 
			{
				m_pMarcMgr->GetItem(&marc, _T("100$a"), strItemData);
				pDM_Index->SetCellData( _T("IBS_저작자"), strItemData, 0);
			} 
			else 
			{
				m_pMarcMgr->GetItem(&marc, _T("700$a"), strItemData);
				pDM_Index->SetCellData( _T("IBS_저작자"), strItemData, 0);
			}
		}
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_본표제"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		pDM_Index->GetCellData( _T("IBS_검색용본표제"), 0, strItemData);
		pDM_Index->SetCellData( _T("IBS_본표제"), strItemData, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_검색용범위검색발행자"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("발행자"), strItemData);
		pDM_Index->SetCellData( _T("IBS_검색용범위검색발행자"), strItemData, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_발행자"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("발행자"), strItemData);
		pDM_Index->SetCellData( _T("IBS_발행자"), strItemData, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_발행년"), 0, strItemData);	
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("범위검색용발행년도"), strItemData);
		pDM_Index->SetCellData( _T("IBS_발행년"), strItemData, 0);
	}

	strItemData.Empty();
	m_pMarcMgr->GetItem( &marc , _T("008@15-17"), strItemData );
	pDM_Index->SetCellData( _T("IBS_발행국코드"), strItemData, 0);


	CString strIdxVolTitle;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM_Book->GetCellData( _T("BB_편권차색인"), idx, strItemData);
		if (ids < 0 || strItemData.IsEmpty()) continue;
		
		strIdxVolTitle += _T(" ") + strItemData;
	}
	pDM_Index->SetCellData( _T("IBS_권서명"), strIdxVolTitle, 0);
	
	CString strIdxTitle;
	CString strIdxAuthor;
	CString strIdxPublisher;
	CString strIdxKeyword;
	CString strIdxSubject;

	ids = pDM_Index->GetCellData( _T("IBS_검색용본표제"), 0, strIdxTitle);
	ids = pDM_Index->GetCellData( _T("IBS_검색용저작자"), 0, strIdxAuthor);
	ids = pDM_Index->GetCellData( _T("IBS_검색용발행자"), 0, strIdxPublisher);
	ids = pDM_Index->GetCellData( _T("IBS_검색용키워드"), 0, strIdxKeyword);	
	
	ids = pDM_Index->GetCellData( _T("IBS_주제명파일"), 0, strIdxSubject);	
	
	CString strAuthor = pDM_Index->GetCellData( _T("IBS_저작자"), 0 );
	strAuthor.MakeLower();
	CString strQuery;
	strQuery.Format(_T("SELECT LOWER(FROM_DATA), UPPER(REPLACE(FROM_DATA, ' ', '')) FROM CO_CHARSET_ROLEWORD_TBL ORDER BY LENGTH(FROM_DATA) DESC"));
	CESL_DataMgr dmRole;
	dmRole.SetCONNECTION_INFO(pDM_Species->CONNECTION_INFO);
	dmRole.RestructDataManager(strQuery);
	INT nResultRoleCount = 0;
	nResultRoleCount = dmRole.GetRowCount();
	CString strFromData;
	CString strFromDataIdx;
	for(INT step = 0; step<nResultRoleCount; step++)
	{
		strFromData.Format(_T("%s "), dmRole.GetCellData(step, 0));
		
		if(0 == strAuthor.Find(strFromData, 0))
		{
			strFromDataIdx = dmRole.GetCellData(step, 1);
			if(0 == strIdxAuthor.Find(strFromDataIdx, 0))
			{
				CString strTemp;
				INT nLength;
				nLength = strFromDataIdx.GetLength();
				strTemp = strIdxAuthor.Mid(nLength);
				strIdxAuthor = strTemp;
				pDM_Index->SetCellData(_T("IBS_검색용저작자"), strIdxAuthor, 0);
			}

			strFromDataIdx.Format(_T(" %s"), dmRole.GetCellData(step, 1));
			ids = strIdxAuthor.Find(strFromDataIdx, 0);
			if(0 <= ids)
			{
				strIdxAuthor.Replace(strFromDataIdx, _T(" "));
				pDM_Index->SetCellData(_T("IBS_검색용저작자"), strIdxAuthor, 0);
			}
		}
	}
	CString strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword + _T(" ") +strIdxSubject;
	CStringArray arrStrTarget;

	if ( strIdxAllItem.GetLength() > 1333 )
	{
		strIdxAllItem = strIdxAllItem.Left(1333);
	}
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
	pDM_Index->SetCellData( _T("IBS_전체검색용색인어"), strIdxAllItem, 0);

	CString strWhere;
	CString strYN;
	CString strRecKey;

	pDM_Index->GetCellData( _T("IBS_REC_KEY"), 0, strRecKey);
	
	strWhere.Format(_T("select DECODE(MAX(CONTENTS_KEY), NULL, 'N', 'Y')||DECODE(MAX(ABSTRACTS_KEY), NULL, 'N', 'Y') FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY=%s"), strRecKey);
	pDM_Species->GetOneValueQuery(strWhere, strYN);
	
	pDM_Index->SetCellData( _T("IBS_목차유무"), strYN.Left(1), 0);
	pDM_Index->SetCellData( _T("IBS_초록유무"), strYN.Right(1), 0);

	return 0;

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ApplyIndex_ST_CODE(CESL_DataMgr *pDM_Index, 
								   CESL_DataMgr *pDM_Book, 
								   INT nMode)
{
	EFS_BEGIN

	CString strSTCODE, strSETISBN, strDelimeter;

	CStringArray arrStrISBN;
		
	if (nMode > 0)
	{
		strDelimeter = _T(" ");
		pDM_Index->GetCellData( _T("IBS_ISBN"), 0, strSTCODE);
		
		MakeIndexArray(&arrStrISBN, strSTCODE);
	}

	pDM_Index->GetCellData( _T("IBS_세트_ISBN1"), 0, strSETISBN);
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
		pDM_Book->GetCellData( _T("BB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_Book->GetCellData( _T("BB_낱권ISBN"), i, strEAISBN);
		m_pIndexTool->GetIndex()->MakeIndex(strEAISBN);
		if(!IsStringInArray(&arrStrISBN, strEAISBN)) arrStrISBN.Add(strEAISBN);
		pDM_Book->GetCellData( _T("BB_추가낱권ISBN"), i, strEXTRA_EAISBN);
		strEXTRA_EAISBN.Replace(_T("|"),_T(" "));
		MakeIndexArray(&arrStrISBN, strEXTRA_EAISBN);
	}

	strSTCODE.Empty();
	for(i=0 ; i<arrStrISBN.GetSize() ; i++)
	{
		if(strSTCODE.IsEmpty()) strSTCODE = arrStrISBN.GetAt(i);
		else strSTCODE += _T(" ") + arrStrISBN.GetAt(i);
	}

	pDM_Index->SetCellData( _T("IBS_ISBN"), strSTCODE, 0);
	return 0;


	EFS_END
	return -1;
}

INT CCatApi_BO::ApplyShelfCodeData(CESL_DataMgr *pDM_Book, 
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
	dm.SetCONNECTION_INFO(m_pParnetMgr->m_pInfo->CONNECTION_INFO);
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

		strStatusCode = pDM_Book->GetCellData ( _T("BB_자료상태"), idx );
		sDbFlag = pDM_Book->GetCellData( _T("BB_DB_FLAG"), idx );

		if ( ((strStatusCode > _T("BOL112") && sDbFlag != _T("I") ) || sDbFlag == _T("D")) &&
			!pDM_Book->GetCellData( _T("BB_배가위치부호"), idx).IsEmpty() )	
			continue ;
	
		ids = pDM_Book->GetCellData( _T("BB_관리구분_코드"), idx, strManageCode);
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
					CString strField;
					strOption = strLC;				
					m_pMarcMgr->GetField(pMarc, _T("085"), strField ); 
					strClassType = strField.Mid(0,1);					
				}

				if ( strOption.IsEmpty() )
				{					
					AfxMessageBox(_T("분류기호가 없습니다.\n배가실을 결정할 수 없습니다."));
					return -1;
				}
				if ( strClassType==_T("") )
				{
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_CLS_NUM_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'MO' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("START_CLASS_NO <= '%s' AND END_CLASS_NO >= '%s'"))
								   , strManageCode, strOption, strOption);
				}
				else
				{
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_CLS_NUM_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'MO' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("START_CLASS_NO <= '%s' AND END_CLASS_NO >= '%s' AND CLASS_NO_TYPE='%s'"))
								   , strManageCode, strOption, strOption, strClassType);
				}
			}
			else if (dm.GetCellData(nPriority, 0) == _T("등록구분"))
			{
				ids = pDM_Book->GetCellData( _T("BB_등록구분_코드"), idx, strOption);
				if (ids < 0)
				{
					AfxMessageBox(_T("등록구분이 없습니다.\n배가실을 결정할 수 없습니다."));
					return -1;
				}
				strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_REG_CODE_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'MO' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("REG_CODE = '%s'")), strManageCode, strOption);
			}
			else if (dm.GetCellData(nPriority, 0) == _T("별치기호"))
			{
				ids = pDM_Book->GetCellData( _T("BB_별치기호_코드"), idx, strOption);
				if (ids < 0)
				{
					AfxMessageBox(_T("별치기호가 없습니다.\n배가실을 결정할 수 없습니다."));
					return -1;
				}
				if(strOption.IsEmpty()){
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_SEP_CODE_SHELF_RULE_MNG_TBL "
						_T("WHERE PUBLISH_FORM_CODE = 'MO' AND ")
						_T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
						_T("SEPARATE_SHELF_CODE IS NULL ")), strManageCode);
				}else {
				strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_SEP_CODE_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'MO' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("SEPARATE_SHELF_CODE = '%s'")), strManageCode, strOption);
				}
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
				if ( pDM_Book->GetCellData( _T("BB_배가위치부호"), idx).IsEmpty())
				{
					pDM_Book->SetCellData( _T("BB_배가위치부호"), strShelfLocCode, idx);

					CString strQueryFlag = GetQueryFlag(MNG_MODIFY, nAppMode, pDM_Book->GetCellData( _T("BB_DB_FLAG"), idx));
					pDM_Book->SetCellData( _T("BB_DB_FLAG"), strQueryFlag, idx);
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

INT CCatApi_BO::ApplyVolumeUpdate(CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Book, 
								  INT nVolumeIdx, 
								  CString strAlias, 
								  CString strItemData)
{
	EFS_BEGIN

	if (!pDM_Volume || !pDM_Book) return -1;

	CString strVol;
	INT ids = pDM_Volume->GetCellData( _T("BB_편권차"), nVolumeIdx, strVol);
	if (ids < 0) return ids;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"), strVol)) continue;

		pDM_Book->SetCellData(strAlias, strItemData, idx);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::CompleteCataloging( CESL_DataMgr *pDM_Out, 
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
	
	ids = pDM_Species->GetCellData( _T("BS_종KEY"), nIdx, strSpeciesKey);
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
		
		ids = pDM_Species->GetCellData( _T("BS_MARC"), nIdx, strStreamMarc);
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
			pDM_Species->SetCellData( _T("BS_제어번호"), strItem, nIdx);
			return 0;
		}
	}

	ids = pDM_Species->GetCellData( _T("BS_제어자료구분_코드"), nIdx, strControlCode);
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
			strItem = _T("");
			ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				m_strLastErrMsg = _T("등록번호가 부여되지 않은 책이 있습니다.");
				return -1;
			}			

			strItem = _T("");
			ids = pDM_Book->GetCellData( _T("BB_배가위치부호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				m_strLastErrMsg = _T("배가실 코드가 부여되지 않은 책이 있습니다.");
				return -1;
			}

			strItem = _T("");
			ids = pDM_Book->GetCellData( _T("BB_정리완료일"), idx, strItem);
			if ( strItem == _T("") )
			{
				CString strCatCoompleteDate;
				CTime t = CTime::GetCurrentTime();
				strCatCoompleteDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
				pDM_Book->SetCellData( _T("BB_정리완료일"), strCatCoompleteDate, idx );
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

	ids = pDM_Species->GetOneValueQuery(strQuery, strItem);
	if (ids < 0)
	{
		strControlNo = _T("1");

		CString sControlNo;
		sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );

		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL ")
						_T("WHERE CONTROL_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), sControlNo);

		ids = pDM_Species->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return -1;
		}

		ids = pDM_Out->MakeRecKey(strReckey);
		if (ids < 0) return ids;

		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
						_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, ")
						_T("LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
						_T("VALUES ('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);")
						, strReckey, strControlCode, strYear, strControlNo
						, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG)
						, m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG) );			

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

		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL ")
						_T("WHERE CONTROL_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), sControlNo);

		ids = pDM_Species->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return -1;
		}
		
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%s, LAST_WORK='%s' ")
						_T("WHERE DIVISION_VALUE='%s' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;"), 
			strControlNo, m_pParnetMgr->GetWorkLogMsg(_T("Marc수정"),WORK_LOG), strControlCode, strYear);

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


	ids = pDM_Out->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("STRING"), strSpeciesKey );
	if ( ids < 0 ) {
		AfxMessageBox( _T("CatApi MakeUpdateFrame 1 확인") );
		return -1;
	}	

	strQuery.Format(_T("UPDATE IDX_BO_TBL SET CONTROL_NO = '%s' WHERE REC_KEY = '%s';")
		, strControlNo, strSpeciesKey);
	
	ids = pDM_Out->AddFrame(strQuery);
	if (ids < 0) return ids;

	ids = pDM_Species->SetCellData( _T("BS_제어번호"), strControlNo, nIdx);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, nIdx);

	if (pDM_Index)
		ids = pDM_Index->SetCellData( _T("IBS_제어번호"), strControlNo, nIdx);

	if (pDM_Book)
	{
		nRowCount = pDM_Book->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			ids = pDM_Book->GetCellData( _T("BB_책KEY"), idx, strBookKey);
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

INT CCatApi_BO::Convert049ToBook(CMarc *pMarc, 
								 CESL_DataMgr *pDM_Book,
								 BOOL bNewVolSortNo)
{
	EFS_BEGIN

	if (pMarc == NULL || pDM_Book == NULL) return -1;

	INT ids = -1;
	CMarc marc;

	CString strFirstInd;
	CString strDefaultShelfLocCode;

	ids = MakeTemp049Marc(pMarc, &marc, strFirstInd, strDefaultShelfLocCode);
	if (ids < 0) return 0;

	INT nIdx;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strRegNo;
	CString strSubfieldCode;
	CString strItemData;
	POSITION pos = marc.m_listTag.GetTailPosition();
	POSITION posOld = NULL;
	while (pos)
	{
		posOld = pos;
		pTag = reinterpret_cast<CTag*>(marc.m_listTag.GetPrev(pos));
		if (pTag == NULL) continue;

		strSubfieldCode.Format(_T("%cl"), SUBFIELD_CODE);

		pSubfield = pTag->FindSubfield(strSubfieldCode);
		if (pSubfield == NULL) continue;

		strRegNo = pSubfield->GetItemData();
		GetBookIdxByRegNo(pDM_Book, strRegNo, nIdx, _T("BB") );
		
		if (nIdx < 0)
		{
			continue;
		}
		
		strSubfieldCode.Format(_T("%cc"), SUBFIELD_CODE);
		pSubfield = pTag->FindSubfield(strSubfieldCode);
		if (pSubfield)
		{
			ids = pDM_Book->SetCellData( _T("BB_복본기호"), pSubfield->GetItemData(), nIdx);
		}
		else
		{
			ids = pDM_Book->SetCellData( _T("BB_복본기호"), _T(""), nIdx);
		}

		strSubfieldCode.Format(_T("%cv"), SUBFIELD_CODE);
		pSubfield = pTag->FindSubfield(strSubfieldCode);
		if (pSubfield)
		{
			ids = pDM_Book->SetCellData( _T("BB_권책기호"), pSubfield->GetItemData(), nIdx);
			ids = pDM_Book->SetCellData( _T("BB_편권차"), pSubfield->GetItemData(), nIdx);
		}
		else
		{
			ids = pDM_Book->SetCellData( _T("BB_권책기호"), _T(""), nIdx);
			ids = pDM_Book->SetCellData( _T("BB_편권차"), _T(""), nIdx);
		}
		
		if (strFirstInd == _T("1"))
		{
			strSubfieldCode.Format(_T("%cf"), SUBFIELD_CODE);
			pSubfield = pTag->FindSubfield(strSubfieldCode);
			if (pSubfield)
			{
				ids = pDM_Book->SetCellData( _T("BB_별치기호_코드"), pSubfield->GetItemData(), nIdx);
			}
			else 
				ids = pDM_Book->SetCellData( _T("BB_별치기호_코드"), _T(""), nIdx);
		}
		else if (strFirstInd == _T("0"))
		{
			ids = pDM_Book->SetCellData( _T("BB_별치기호_코드"), strDefaultShelfLocCode, nIdx);
		}
	}

	m_pMarcMgr->DeleteField(pMarc, _T("049"));

	CString str049Field;
	pos = marc.m_listTag.GetHeadPosition();
	while (pos)
	{
		pTag = reinterpret_cast<CTag*>(marc.m_listTag.GetNext(pos));
		if (pTag == NULL) continue;

		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (pSubfield == NULL) continue;

			str049Field += pSubfield->GetPunctuationMark() + pSubfield->GetSubfieldCode() + pSubfield->GetItemData();
		}
	}

	if (!str049Field.IsEmpty())
	{
		CString strTagCode;
		strTagCode.Format(_T("049%s "), strFirstInd);
		m_pMarcMgr->InsertField(pMarc, strTagCode + str049Field);
	}

	INT nVolCount = GetVolumeCount(pDM_Book, _T("BB"));

	if ( bNewVolSortNo )
	{
		if (nVolCount > 1) {
			INT iMaxVolSortNo = 0 ;
			for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
				CString sVolSortNo = pDM_Book->GetCellData( _T("BB_권일련번호"), i );
				INT iVolSortNo = _ttoi( sVolSortNo );
				if ( iVolSortNo > iMaxVolSortNo ) {
					iMaxVolSortNo = iVolSortNo;
				}
			}
			
			INT iFind = 0;
			for ( i = 0 ; i < pDM_Book->GetRowCount()-1 ; i++ ) {
				CString sVolNoMain = pDM_Book->GetCellData( _T("BB_편권차"), i );
				CString sVolSortNoMain = pDM_Book->GetCellData( _T("BB_권일련번호"), i );
				for ( INT j = i+1 ; j < pDM_Book->GetRowCount() ; j++ ) {
					CString sVolCodeObj = pDM_Book->GetCellData( _T("BB_편권차"), j );
					CString sVolSortNoObj = pDM_Book->GetCellData( _T("BB_권일련번호"), j );
					if ( sVolNoMain == sVolCodeObj ) {
						if ( sVolSortNoMain != sVolSortNoObj ) {
							pDM_Book->SetCellData( _T("BB_권일련번호"), sVolSortNoMain, j );
						}
					}
					else {
						if ( sVolSortNoMain == sVolSortNoObj ) {
							iMaxVolSortNo += 10;
							CString sMaxVolSortNo;
							sMaxVolSortNo.Format( _T("%d"), iMaxVolSortNo );
							pDM_Book->SetCellData( _T("BB_권일련번호"), sMaxVolSortNo, j );
						}
					}
				}
			}
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ConvertCatDMToMarc( CMarc *pMarc, 
									CESL_DataMgr *pDM_Species, 
									CESL_DataMgr *pDM_Index, 
									CESL_DataMgr *pDM_Book, 
									CESL_DataMgr *pDM_Volume, 
									CESL_DataMgr *pDM_Purchase /*= NULL*/)
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

	INT nRepresentBookIdx;
	GetRepresentationBook(pDM_Book, _T("BB"), nRepresentBookIdx);
	if (nRepresentBookIdx < 0) return -1;

	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		if (strDMFieldAlias.Find(_T("IBS")) >= 0)
			pDM_Index->GetCellData(strDMFieldAlias, 0, strItemData);
		else if (strDMFieldAlias.Find(_T("BB")) >= 0)
			pDM_Book->GetCellData(strDMFieldAlias, 0, strItemData);

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
			strItemData = _T("s");
			m_pMarcMgr->SetItem(pMarc, _T("008@6"), strItemData);
		}
		
		if (strItemData == _T("s"))
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
		
		CString strTempUseCode ;
        pDM_Index->GetCellData(_T("IBS_이용대상구분_코드"), 0, strTempUseCode);
		if (strTempUseCode.IsEmpty())
		strTempUseCode = pDM_Book->GetCellData(_T("BB_이용대상구분_코드"), nRepresentBookIdx);
    }

	CString strVol;
	ConvertBookToVolume(pDM_Book, pDM_Volume);
	INT nRowCount = pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strItemData = pDM_Volume->GetCellData(_T("BB_편권차"), idx);
		if (strItemData.IsEmpty()) continue;

		if (strVol.IsEmpty())
			strVol = strItemData;
		else
			strVol += _T(",") + strItemData;
	}

	INT nVolCount = GetVolumeCount(pDM_Book, _T("BB"));

	m_mapMarcAlias.Lookup(_T("편제"), strMarcAlias);
	m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);
	if (nVolCount == 1)
	{
		pDM_Book->GetCellData(_T("BB_편제명부록명"), nRepresentBookIdx, strItemData);
		if (!strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
	}

	m_mapMarcAlias.Lookup(_T("크기"), strMarcAlias);
	
	if (nVolCount == 1)
	{
		pDM_Book->GetCellData(_T("BB_크기"), nRepresentBookIdx, strItemData);
		if (!strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
		else 
			m_pMarcMgr->DeleteItem(pMarc, strMarcAlias, strItemData);
	}

	m_mapMarcAlias.Lookup(_T("면장수"), strMarcAlias);
	
	if (nVolCount == 1)
	{
		pDM_Book->GetCellData(_T("BB_면장수"), nRepresentBookIdx, strItemData);
		if (!strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
		else
			m_pMarcMgr->DeleteItem(pMarc, strMarcAlias, strItemData);
	}

	m_mapMarcAlias.Lookup(_T("물리적특성"), strMarcAlias);
	
	if (nVolCount == 1)
	{
		pDM_Book->GetCellData(_T("BB_물리적특성"), nRepresentBookIdx, strItemData);
		if (!strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
		else
			m_pMarcMgr->DeleteItem(pMarc, strMarcAlias, strItemData);
	}

	m_mapMarcAlias.Lookup(_T("딸림자료"), strMarcAlias);
	
	if (nVolCount == 1)
	{
		pDM_Book->GetCellData(_T("BB_딸림자료"), nRepresentBookIdx, strItemData);
		if (!strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
		else
			m_pMarcMgr->DeleteItem(pMarc, strMarcAlias, strItemData);
	}

	if (nVolCount == 1)
	{
		pDM_Book->GetCellData(_T("BB_가격잡정보"), nRepresentBookIdx, strItemData);
		if (!strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, _T("950$c"), strItemData);
		else
			m_pMarcMgr->DeleteItem(pMarc, _T("950$c"), strItemData);
	}

	CString strFieldData;
	m_pMarcMgr->GetField( pMarc, _T("300"), strFieldData);
	strFieldData.TrimLeft(); strFieldData.TrimRight();
	if( 2 > strFieldData.GetLength() )
	{
		m_pMarcMgr->DeleteField( pMarc, _T("300") );
	}

	if (nVolCount == 1)
	{
		INT nIdx;
		GetRepresentationBook(pDM_Book, _T("BB"), nIdx);
		if (nIdx >= 0)
		{
			m_mapMarcAlias.Lookup(_T("가격"), strMarcAlias);

			CString strPriceCode;
			m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
			if (strItemData.IsEmpty())
				strPriceCode = _T("\\");
			else
			{
				strPriceCode = GetPriceCode(strItemData, strItemData);
			}

			strItemData = pDM_Book->GetCellData(_T("BB_가격"), nIdx);

			if ( strItemData != _T("") ) {
				m_pMarcMgr->SetItem(pMarc, strMarcAlias, strPriceCode + strItemData);
			}
		}
	}
	else if (nVolCount >= 1)
	{
		m_mapMarcAlias.Lookup(_T("가격"), strMarcAlias);
		
		CString strPriceCode;
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		if (strItemData.IsEmpty())
			strPriceCode = _T("\\");
		else
		{
			strPriceCode = GetPriceCode(strItemData, strItemData);
		}

		INT nSum = 0;
		ConvertBookToVolume(pDM_Book, pDM_Volume);
		GetBookPriceSum(pDM_Volume, nSum);
		strItemData.Format(_T("%d"), nSum);
		m_pMarcMgr->SetItem(pMarc, strMarcAlias, strPriceCode + strItemData);
	}

	if (nVolCount == 1)
	{
		strItemData = pDM_Index->GetCellData(_T("IBS_저작자"), 0);
		if (strItemData.IsEmpty())
			strItemData = pDM_Book->GetCellData(_T("BB_저작자"), nRepresentBookIdx);
		if (!strItemData.IsEmpty())
		{
			m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
			m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);
			m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
			m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);

			INT n = 0;

			CString strData;
			CString strTempItemData;
			INT nLocation;
			while(1)
			{
				if (n == 0)
				{
					m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
				}
				else
				{
					m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
				}
				nLocation = strItemData.FindOneOf(_T(",;"));				
				if( nLocation > 0 )
				{
					strData = strItemData.Mid( 0, nLocation);
					m_pMarcMgr->SetItem(pMarc, strMarcAlias, strData);
					strTempItemData = strItemData.Mid( nLocation+1, strItemData.GetLength()-nLocation-1);
					strItemData.Format(strTempItemData);				
				}
				else
				{
					m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
					break;
				}
				
				n++;
			}
		}
	}

	strItemData = pDM_Index->GetCellData(_T("IBS_발행자"), 0);
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
	
	ConvertBookToVolume(pDM_Book, pDM_Volume);
	CString strItemData1;
	CString strItemData2;
	CString strItemData3;
	CString strItemData4;
	m_pMarcMgr->DeleteField(pMarc, _T("020"), ' ', ' ');
	nRowCount = pDM_Volume->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strItemData.Empty();

		strItemData1 = pDM_Volume->GetCellData(_T("BB_낱권ISBN"), idx);
		if (!strItemData1.IsEmpty())
		{
			strItemData1.Replace(_T("-"), _T(""));
			strItemData = pDM_Volume->GetCellData(_T("BB_편권차"), idx);
			if (!strItemData.IsEmpty())
			{
				strItemData1 += _T("(") + strItemData + _T(")");
			}
		}

		strItemData2 = pDM_Volume->GetCellData(_T("BB_낱권ISBN부가기호"), idx);
		strItemData3 = pDM_Volume->GetCellData(_T("BB_낱권오류ISBN"), idx);
		strItemData4 = pDM_Volume->GetCellData(_T("BB_가격"), idx);
        
		m_mapMarcAlias.Lookup(_T("가격"), strMarcAlias);

		CString strPriceCode;
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		if (!strItemData.IsEmpty()) 
		{
			strPriceCode = GetPriceCode(strItemData, strItemData);
		}
		strItemData4 = strPriceCode + strItemData4;

		if (strItemData1.IsEmpty() && strItemData2.IsEmpty() && strItemData3.IsEmpty() && strItemData4.IsEmpty()) continue;
		
		strItemData = _T("020  ");
		CString strTmp = _T("");
		if (!strItemData1.IsEmpty())
		{
			strTmp.Format(_T("%ca%s"), SUBFIELD_CODE, strItemData1);
			strItemData += strTmp;
		}
		if (!strItemData2.IsEmpty())
		{
			strTmp.Format(_T("%cg%s"), SUBFIELD_CODE, strItemData2);
			strItemData += strTmp;
		}
		if (!strItemData3.IsEmpty())
		{
			strTmp.Format(_T("%cz%s"), SUBFIELD_CODE, strItemData3);
			strItemData += strTmp;
		}
		if (!strItemData4.IsEmpty() && (!strItemData1.IsEmpty() || !strItemData2.IsEmpty() || !strItemData3.IsEmpty()))
		{
			strTmp.Format(_T("%cc%s"), SUBFIELD_CODE, strItemData4);
			strItemData += strTmp;
		}

		if (strItemData.GetLength() > 5)
			m_pMarcMgr->InsertField(pMarc, strItemData);
	}

	ConvertBookTo049(pMarc, pDM_Book);

	m_pMarcMgr->CheckSimpleSyntax(pMarc);
  
	CString strDM245;
	CString strDefault245;
	CString strChange245;
	CString strTemp245;
	CString strCode;
	CString strFindcode;
	INT nFindLocation;
	INT nCode;
	
	nCode = 31;
	strItemData = pDM_Index->GetCellData( _T("IBS_저작자"), 0 );

	if( !strItemData.IsEmpty() )
	{
		strCode.Format(_T(";%ce"), (TCHAR)nCode);
		strItemData.Replace( _T(";"), strCode );
		pMarc->GetField( _T("245"), strDefault245 );
		strFindcode.Format(_T("%cd"),(TCHAR)nCode);
		nFindLocation = strDefault245.Find( strFindcode, 0 );
		strTemp245 = strDefault245.Mid( 0, nFindLocation+2 );
		strChange245.Format( _T("245%s%s"), strTemp245, strItemData );
		strTemp245.Format( _T("245%s"), strDefault245 );
		m_pMarcMgr->UpdateField(pMarc, strTemp245, strChange245 );	
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ConvertMarcToCatDM(CMarc *pMarc, 
								   CESL_DataMgr *pDM_Species, 
								   CESL_DataMgr *pDM_Index, 
								   CESL_DataMgr *pDM_Book, 
								   CESL_DataMgr *pDM_Volume, 
								   CESL_DataMgr *pDM_Purchase /*= NULL*/)
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

	INT nRepresentBookIdx;
	GetRepresentationBook(pDM_Book, _T("BB"), nRepresentBookIdx);
	if (nRepresentBookIdx < 0) return -1;

	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		if (ids < 0) continue;

		if (strDMFieldAlias.Find(_T("IBS")) >= 0)
			pDM_Index->SetCellData(strDMFieldAlias, strItemData, 0);		
		else if (strDMFieldAlias.Find(_T("BB")) >= 0)
			pDM_Book->SetCellData(strDMFieldAlias, strItemData, 0);				
	}

    CString strValue;
	pDM_Book->GetCellData (_T("BB_이용대상구분_코드"),0,strItemData) ;
	if(!strItemData.IsEmpty ())
    { 
        pDM_Index->SetCellData( _T("IBS_이용대상구분_코드"), strItemData, 0);
	}
	else
	{
		ids = m_pMarcMgr->GetItem(pMarc, _T("008@22"), strItemData);
		if (strItemData.IsEmpty()) strItemData = _T(" ");
	
		m_map008ToUseObjCode.Lookup(strItemData, strValue);
		pDM_Index->SetCellData( _T("IBS_이용대상구분_코드"), strValue, 0);
     
    }	

	if (m_pMarcMgr->GetMarcGroup() == _T("단행") && m_pMarcMgr->GetItem(pMarc, _T("300$b"), strItemData) < 0)
	{
		CString strKey;
		CString strData;
		ids = m_pMarcMgr->GetItem(pMarc, _T("008@18-21"), strItemData);
		INT nCount = strItemData.GetLength();
		for (INT idx = 0; idx < nCount; idx++)
		{
			strKey.Format(_T("%c"), strItemData.GetAt(idx));

			strValue = _T("");
			FindSabdoToDesc (strKey, strValue);
			if ( !strData.IsEmpty() )
				strData += _T(", ") + strValue;
			
		}
		if (!strData.IsEmpty() && strData.GetLength() > 2)
		{
			strData = strData.Mid(2);
			m_pMarcMgr->SetItem(pMarc, _T("300$b"), strData);
		}
	}	

	nVolCount = GetVolumeCount(pDM_Book, _T("BB"));
	if (nVolCount == 1)
	{
		CString sDmVol;
		sDmVol = pDM_Book->GetCellData( _T("BB_편권차"), 0 );
		if ( sDmVol == _T("") ) {
			m_mapMarcAlias.Lookup(_T("편권차"), strMarcAlias);
			ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
			if (ids >= 0) UpdateRepresentVolume(pDM_Book, _T("BB_편권차"), strItemData, nRepresentBookIdx);
		}

		m_mapMarcAlias.Lookup(_T("편제"), strMarcAlias);
		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		if (ids >= 0) UpdateRepresentVolume(pDM_Book, _T("BB_편제명부록명"), strItemData, nRepresentBookIdx);
	}

	if (nVolCount == 1)
	{
		m_mapMarcAlias.Lookup(_T("가격"), strMarcAlias);
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		CString strPrice;
		GetPriceCode(strItemData, strPrice);
		UpdateRepresentVolume(pDM_Book, _T("BB_가격"), strPrice);

		m_mapMarcAlias.Lookup(_T("면장수"), strMarcAlias);
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		UpdateRepresentVolume(pDM_Book, _T("BB_면장수"), strItemData);

		m_mapMarcAlias.Lookup(_T("물리적특성"), strMarcAlias);
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		UpdateRepresentVolume(pDM_Book, _T("BB_물리적특성"), strItemData);

		m_mapMarcAlias.Lookup(_T("크기"), strMarcAlias);
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		UpdateRepresentVolume(pDM_Book, _T("BB_크기"), strItemData);

		m_mapMarcAlias.Lookup(_T("딸림자료"), strMarcAlias);
		m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		UpdateRepresentVolume(pDM_Book, _T("BB_딸림자료"), strItemData);

		m_pMarcMgr->GetItem( pMarc, _T("260$c"), strItemData );
		UpdateRepresentVolume(pDM_Book, _T("BB_발행년"), strItemData );
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

	pDM_Index->SetCellData(_T("IBS_본표제"), strTitle, 0);
	
	m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
	m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);

	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData, &arrItemData);

		CString strGetField;
		CString strCutField;
		CString strFind;
		CString strGetCode;
		INT nGetLocation;
		INT nCode;
		nCode = 31;
		strFind.Format(_T("%ce"),nCode); 
		
		pMarc->GetField(_T("245"),strGetField);
		if (ids >= 0)
		{
			INT nOffset = 0;
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;

				nGetLocation = strGetField.Find(strFind,nOffset);

				if ( nGetLocation < 0 ) break;
				strGetCode = strGetField.Mid(nGetLocation-1,1);
				if ( !(strGetCode == _T(";") || strGetCode == _T(",")) )
				{
					strGetCode = _T(";");
				}
				strAuthor += strGetCode + strItemData;
				
				nOffset = nGetLocation + 1;

			}
		}

		if (nVolCount == 1) {
			UpdateRepresentVolume(pDM_Book, _T("BB_저작자"), strAuthor, nRepresentBookIdx);
		}

		pDM_Index->SetCellData(_T("IBS_저작자"), strAuthor, 0);
	}
	CString strVol;

	m_pMarcMgr->GetItem(pMarc, _T("245$n"), strVol);
	if (!strVol.IsEmpty())
	{
		pDM_Index->SetCellData(_T("IBS_편권차"), strVol, 0);
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

		pDM_Index->SetCellData(_T("IBS_발행자"), strPublisher, 0);
	}

	if (GetVolumeCount(pDM_Book, _T("BB")) == 1)
	{
		CTag *pTag = NULL;
		CSubfield *pSubfield = NULL;
		CString strSubfieldCode;
		POSITION posTag = NULL;
		POSITION posSubfield = NULL;
		posTag = pMarc->m_listTag.GetHeadPosition();
		while (posTag)
		{
			pTag = reinterpret_cast<CTag*>(pMarc->m_listTag.GetNext(posTag));
			if (pTag == NULL) continue;

			if (pTag->GetTagCode() != _T("020")) continue;
			if (pTag->GetFirstIndicator() != _T(" ") || pTag->GetSecondIndicator() != _T(" ")) continue;
			
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
				if (pSubfield == NULL) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, _T('$'));

				strItemData = pSubfield->GetItemData();

				if (strSubfieldCode == _T("$a"))
				{
					pDM_Index->SetCellData(_T("IBS_낱권ISBN"), strItemData, 0);
					m_pIndexTool->GetIndex()->RemoveAllParenthesis(strItemData);
					UpdateRepresentVolume(pDM_Book, _T("BB_낱권ISBN"), strItemData, nRepresentBookIdx);
				}
				else if (strSubfieldCode == _T("$g"))
				{
					pDM_Index->SetCellData(_T("IBS_낱권ISBN부가기호"), strItemData, 0);
					UpdateRepresentVolume(pDM_Book, _T("BB_낱권ISBN부가기호"), strItemData, nRepresentBookIdx);
				}
				else if (strSubfieldCode == _T("%z"))
				{
					pDM_Index->SetCellData(_T("IBS_낱권오류ISBN"), strItemData, 0);
					UpdateRepresentVolume(pDM_Book, _T("BB_낱권오류ISBN"), strItemData, nRepresentBookIdx);
				}
			}
		}
	}	

	posTag = pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = reinterpret_cast<CTag*>(pMarc->m_listTag.GetNext(posTag));
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() != _T("020")) continue;
		if (pTag->GetFirstIndicator() != _T("1") || pTag->GetSecondIndicator() != _T(" ")) continue;
		
		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (pSubfield == NULL) continue;

			strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, _T('$'));

			strItemData = pSubfield->GetItemData();

			if (strSubfieldCode == _T("$a"))
			{
				pDM_Index->SetCellData(_T("IBS_세트_ISBN1"), strItemData, 0);
			}
			else if (strSubfieldCode == _T("$g"))
			{
				if(strItemData.GetLength() > 5) strItemData = strItemData.Left(5);
				pDM_Index->SetCellData(_T("IBS_세트_ISBN2"), strItemData, 0);
			}
			else if (strSubfieldCode == _T("%z"))
			{
				pDM_Index->SetCellData(_T("IBS_세트_ISBN3"), strItemData, 0);
			}
		}
	}

	Convert049ToBook(pMarc, pDM_Book);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::UpdateSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix, 
							  INT nAppMode,
							  CStringArray *pArrStrAddFrame /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL 
		) return -1;

	INT ids = -1;

	CString strSpeciesKey;
	
	CString strPurchaseinfo_key = _T("-1");

	CString strBookKey;

	CString strItem;

	CArray <CString, CString> RemoveAliasList;

	ids = pDM_Species->GetCellData( _T("BS_종KEY"), 0, strSpeciesKey);
	
	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("BS"));

	ids = ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	pDM_Species->GetCellData( _T("BS_이용제한구분_코드"), 0, strItem);
	GetSpeciesUseLimitCode(pDM_Book, strItem, _T("BB"), 1, strItem);
	pDM_Species->SetCellData( _T("BS_이용제한구분_코드"), strItem, 0);

	RemoveAliasList.Add( _T("BS_종KEY") );
	
	pDM_Species->InitDBFieldData();
	pDM_Species->MakeQueryByDM(RemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	ids = ApplyIndex(pDM_Species, pDM_Index, pDM_Book);
	
	ApplyIndex_ST_CODE(pDM_Index, pDM_Book);

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add( _T("IBS_REC_KEY"));

	pDM_Species->InitDBFieldData();
	pDM_Species->MakeQueryByDM(RemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("IBS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("IBS_관리구분_코드"));
	Index_classfycodes.Add( _T("IBS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("IBS_등록구분_코드"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("BB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("BB_관리구분_코드"));
	Book_classfycodes.Add( _T("BB_배가위치부호"));
	Book_classfycodes.Add( _T("BB_등록구분_코드"));	
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("BB"));
	ids = pDM_Species->AddFrame(Index_Update_sql);
	if ( 0 > ids ) return -1;

	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		CString sDbFlag = pDM_Book->GetCellData( _T("BB_DB_FLAG"), i );
		if ( sDbFlag == _T("") ) {
			pDM_Book->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
		}
	}
	ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, strPurchaseinfo_key, pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("책정보 쿼리만들기 실패");
		return ids;
	}
    
    CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->SetCONNECTION_INFO(m_pParnetMgr->CONNECTION_INFO);
	pDM_EBook->InitDMPKFromFile(_T("DMUP_단행_종_EBOOK"));
	pDM_EBook->SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);
      

	CBO_SPECIES *pBO_Species = new CBO_SPECIES(m_pParnetMgr );
    ids = pBO_Species->MakeEBookInfoAndQueryFrame (pDM_Book,pDM_EBook,pDM_Species) ;
    if (ids < 0) 
	{
		m_strLastErrMsg = _T("ebook정보 쿼리만들기 실패");
		return ids;
	}

	if (pDM_Appendix)
	{
		ids = ApplyAppendixRegNo(pDM_Appendix, nAppMode );
		if (ids < 0)
		{
			m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");
			return ids;
		}

		ids = MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("-1"), pDM_Species);
		if (ids < 0)
		{
			m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");
			AfxMessageBox(_T("부록등록번호 자동부여 중복입니다.\n마지막번호를 확인하세요"));
			return ids;
		}
	}

	ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
    
	if(m_LastSpeciesKey.GetLength() > 1)
	{
		CString strQuery ;
		CString sDup = pDM_Index->GetCellData( _T("IBS_작업상태INFO"), 0 );
		if ( sDup.Left(3) != _T("BOL") ) {
			strQuery.Format (_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG = 'Y' WHERE REC_KEY = %s;"),m_LastSpeciesKey) ;
			ids = pDM_Species ->AddFrame (strQuery) ;
			if (ids < 0) return ids;
		}

		CString sDupSpeciesKeyList;
		for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
			CString sDupSpeciesKey = pDM_Book->GetCellData( _T("BB_복본종KEY"), i );
			if ( sDupSpeciesKey != _T("") ) {
				sDupSpeciesKeyList = sDupSpeciesKeyList + _T(",") + sDupSpeciesKey;
			}
		}
		if ( sDupSpeciesKeyList.GetLength() > 1 ) {
			sDupSpeciesKeyList = sDupSpeciesKeyList.Mid(1);
			strQuery.Format (_T("UPDATE IDX_BO_TBL SET WORKING_STATUS = 'BOT212O' WHERE REC_KEY IN (%s);"),sDupSpeciesKeyList) ;
			ids = pDM_Species ->AddFrame (strQuery) ;
			if (ids < 0) return ids;

			strQuery.Format (_T("UPDATE BO_PURCHASEINFO_TBL SET SPECIES_KEY = %s WHERE SPECIES_KEY IN (%s);"), m_LastSpeciesKey, sDupSpeciesKeyList) ;
			ids = pDM_Species ->AddFrame (strQuery) ;
			if (ids < 0) return ids;
			strQuery.Format (_T("UPDATE BO_DONINFO_TBL SET SPECIES_KEY = %s WHERE SPECIES_KEY IN (%s);"), m_LastSpeciesKey, sDupSpeciesKeyList) ;
			ids = pDM_Species ->AddFrame (strQuery) ;
			if (ids < 0) return ids;
		}
	}
	
	CString strSetCallNoQuery;
	CString strBookCode		= _T("") ;
	CString strClassCode	= _T("") ;

	pDM_Index->GetCellData( _T("IBS_도서기호") , 0 , strBookCode  ) ;
	pDM_Index->GetCellData( _T("IBS_분류기호") , 0 , strClassCode ) ;
	
	strSetCallNoQuery.Format(_T("UPDATE BO_BOOK_TBL SET BOOK_CODE='%s', CLASS_NO='%s' WHERE SPECIES_KEY = %s;"), strBookCode, strClassCode, strSpeciesKey );
	pDM_Species->AddFrame(strSetCallNoQuery);

	CString sEcoSearch;
	if ( m_pParnetMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
		sEcoSearch.Format( _T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey );
		ids = pDM_Species->AddFrame(sEcoSearch);
		if ( 0 > ids ) return -1;
	}
	else if ( m_pParnetMgr->m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
		AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
		return -1;
	}	

	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_마지막번호 확인") );
		return -1;
	}
	for ( i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("BB_등록구분_코드"), i );
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

	CString sTmpIDXDelKey = _T("") ;
	pDM_Species->GetCellData( _T("BS_삭제색인KEY") , 0 , sTmpIDXDelKey ) ;
	if ( sTmpIDXDelKey != _T("") )
	{
		CString sTmpQuery = _T("");
		sTmpQuery.Format(_T("DELETE IDX_BO_TBL WHERE REC_KEY = %s;"), sTmpIDXDelKey );
		pDM_Species->AddFrame(sTmpQuery);
	}



	ids = pDM_Species->SendFrame();
	if ( 0 < ids ) return -1;
	ids = pDM_Species->EndFrame();
	if (ids < 0 )
	{
		m_strLastErrMsg = _T("DB에 마크를 입력하는데 에러가 발생했습니다.");
		return ids;
	}
	
	FILE *fpCode;
	CString sDate = pDM_Index->GetDate();
	sDate = sDate.Left(10);
	sDate.Replace( _T("/"), NULL );
	CString sTitle = _T("..\\tunning\\색인종책값") + sDate + _T(".txt");
	fpCode = _tfopen( sTitle, _T("a+b") );
	fseek(fpCode, 0, SEEK_END);		
	if( 0 == ftell(fpCode) )
	{
		/*UNCHANGE*/fputc(0xFF, fpCode);
		/*UNCHANGE*/fputc(0xFE, fpCode);
	}
	
	_ftprintf( fpCode, _T("\n\n수정값\r\n") );
	for ( i = 0 ; i < pDM_Index->GetRowCount() ; i++ ) {
		for ( INT j = 0 ; j < pDM_Index->GetColCount() ; j++ ) {
			
			CString sItem = pDM_Index->GetCellData( i, j );
			sItem = sItem + _T(" ");
			_ftprintf( fpCode, sItem );
		}
	}
	_ftprintf( fpCode, _T("\r\n") );
	for ( i = 0 ; i < pDM_Species->GetRowCount() ; i++ ) {
		for ( INT j = 0 ; j < pDM_Species->GetColCount() ; j++ ) {
			CString sItem = pDM_Species->GetCellData( i, j );			
			sItem = sItem + _T(" ");
			_ftprintf( fpCode, sItem );
		}
	}
	_ftprintf( fpCode, _T("\r\n") );
	for ( i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		for ( INT j = 0 ; j < pDM_Book->GetColCount() ; j++ ) {
			CString sItem = pDM_Book->GetCellData( i, j );			
			sItem = sItem + _T(" ");
			_ftprintf( fpCode, sItem );
		}
		_ftprintf( fpCode, _T("\r\n") );
	}	
	fclose( fpCode );

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ConvertBookTo049(CMarc *pMarc, 
								 CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pMarc == NULL || pDM_Book == NULL) return -1;

	INT ids = -1;
	CMarc marc;

	CSubfield *pSubfield = NULL;
	CTag *pNewTag = NULL;

	CString strFirstInd = _T(" ");
	CString strSubfieldCode;
	CString strDefaultShelfLocCode;

	ids = MakeTemp049Marc(pMarc, &marc, strFirstInd, strDefaultShelfLocCode);

	CString temp;
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapShelfLocCode;
	for(INT k =0 ; k < pDM_Book->GetRowCount ();k++)
	{
		temp  = pDM_Book->GetCellData(_T("BB_별치기호_코드"), k);
		mapShelfLocCode.SetAt(temp,temp);
    }
    if(mapShelfLocCode.GetCount()>1)
	{
		m_pMarcMgr->SetTagIndicator (&marc,_T("049"),_T("1"),_T(""));
        strFirstInd = _T("1");
	}

	ids = m_pMarcMgr->DeleteItem(&marc, _T("049$f"));
	
	ids = m_pMarcMgr->DeleteItem(&marc, _T("049$c"));
	
    ids = m_pMarcMgr->DeleteItem(&marc, _T("049$v"));

    ids = m_pMarcMgr->DeleteItem(&marc, _T("049$l"));

	CArray<CString, CString&> arrStrItem;
	CString strField;
	CString strItemData;
	CString strRegNo;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		arrStrItem.RemoveAll();
		ids = m_pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrStrItem);
		
		if (IsRegNoInArray(&arrStrItem, strRegNo)) continue;

		strField.Format(_T("049  %cl%s"), SUBFIELD_CODE, strRegNo);
		m_pMarcMgr->InsertField(&marc, strField);
		
	}

	INT nIdx;
	CTag *pTag = NULL;
	POSITION pos = marc.m_listTag.GetTailPosition();
	POSITION posOld = NULL;
	while (pos)
	{
		posOld = pos;
		pTag = reinterpret_cast<CTag*>(marc.m_listTag.GetPrev(pos));
		if (pTag == NULL) continue;

		strSubfieldCode.Format(_T("%cl"), SUBFIELD_CODE);

		pSubfield = pTag->FindSubfield(strSubfieldCode);
		if (pSubfield == NULL) continue;

		strRegNo = pSubfield->GetItemData();
		GetBookIdxByRegNo(pDM_Book, strRegNo, nIdx, _T("BB") );

		if (nIdx < 0)
		{
			continue;
		}

		strItemData.Empty();
		ids = pDM_Book->GetCellData(_T("BB_편권차"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			strSubfieldCode.Format(_T("%cv"), SUBFIELD_CODE);
			pSubfield = pTag->FindSubfield(strSubfieldCode);
			if (pSubfield == NULL)
			{
				pSubfield = new CSubfield;
				pTag->AddSubfield(pSubfield);
				pSubfield->SetSubfieldCode(strSubfieldCode);
			}
			if (pSubfield->GetItemData().IsEmpty())
			{
				pSubfield->SetItemData(strItemData);			
			}
		    else
			{
				pSubfield->SetItemData(strItemData);
			}
		}		

		strItemData.Empty();
		ids = pDM_Book->GetCellData( _T("BB_복본기호"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			strSubfieldCode.Format(_T("%cc"), SUBFIELD_CODE);

			pSubfield = new CSubfield;
			pTag->AddSubfield(pSubfield);
			pSubfield->SetSubfieldCode(strSubfieldCode);
			pSubfield->SetItemData(strItemData);
		}

		if (strFirstInd == _T("1"))
		{
			strItemData.Empty();
			ids = pDM_Book->GetCellData( _T("BB_별치기호_코드"), nIdx, strItemData);
			if (!strItemData.IsEmpty())
			{
				strSubfieldCode.Format(_T("%cf"), SUBFIELD_CODE);

				pSubfield = new CSubfield;
				pTag->AddSubfield(pSubfield);
				pSubfield->SetSubfieldCode(strSubfieldCode);
				pSubfield->SetItemData(strItemData);
			}
		}
	}

	GetRepresentationBook(pDM_Book, _T("BB"), nIdx);

	if (nIdx >= 0) 
	{
		ids = pDM_Book->GetCellData( _T("BB_별치기호_코드"), nIdx, strItemData);
		if (!strItemData.IsEmpty() && strFirstInd == _T("0"))
		{
			pos = marc.m_listTag.GetTailPosition();
			if (pos)
			{
				pTag = reinterpret_cast<CTag*>(marc.m_listTag.GetAt(pos));
				if (pTag)
				{
					strSubfieldCode.Format(_T("%cf"), SUBFIELD_CODE);

					pSubfield = new CSubfield;
					pTag->AddSubfield(pSubfield);
					pSubfield->SetSubfieldCode(strSubfieldCode);
					pSubfield->SetItemData(strItemData);
				}
			}
		}
	}
    CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_Book->CONNECTION_INFO);
    dm.InitDataMgrRef (1) ;
	dm.SetDataMgrRef (0,_T("등록번호정렬"),_T(""),_T("str"),_T(""),_T("udf")) ;

	m_pMarcMgr->DeleteField(pMarc, _T("049"));

	CString str049Field;
	pos = marc.m_listTag.GetHeadPosition();
	while (pos)
	{
		pTag = reinterpret_cast<CTag*>(marc.m_listTag.GetNext(pos));
		if (pTag == NULL) continue;

		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (pSubfield == NULL) continue;

			str049Field += pSubfield->GetPunctuationMark() + pSubfield->GetSubfieldCode() + pSubfield->GetItemData();
		}
	    dm.SetCellData (_T("등록번호정렬"),str049Field,-1) ;
		str049Field.Empty () ;
	}

    dm.SORT (_T("등록번호정렬"),FALSE) ;
	CString strSortTemp ;
	str049Field.Empty () ;
	strSortTemp.Empty () ;
    for(INT i = 0 ; i< dm.GetRowCount ();i++)
	{
		 dm.GetCellData (_T("등록번호정렬"),i,strSortTemp) ;
		 str049Field += strSortTemp ;
	}

	if (!str049Field.IsEmpty())
	{
		CString strTagCode;
		strTagCode.Format(_T("049%s "), strFirstInd);
		m_pMarcMgr->DeleteField(pMarc, _T("049"));
		m_pMarcMgr->InsertField(pMarc, strTagCode + str049Field);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::ConvertBookToVolume(CESL_DataMgr *pDM_Book, 
									CESL_DataMgr *pDM_Volume)
{
	EFS_BEGIN

	if (!pDM_Book || !pDM_Volume) return -35001;

	pDM_Volume->FreeData();

	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();

	INT nRowCount = pDM_Book->GetRowCount();
	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG;
	BOOL IsNewVolume;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = pDM_Book->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("D")) continue;
		if (pDM_Book->GetCellData(_T("BB_책부록플래그"), i) == _T("A")) continue;
		IsNewVolume = TRUE;
		ids = pDM_Book->GetCellData(_T("BB_편권차"), i, curVOL_SORT);
		for (j = 0; j < i; j++) {
			if (pDM_Book->GetCellData(_T("BB_책부록플래그"), j) == _T("A")) continue;
			ids = pDM_Book->GetCellData(_T("BB_편권차"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == TRUE) CopyRowIdxList.Add(i);
	}

	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT RowIdx = 0;
	CString strItemData;
	for (i = 0; i < nCopyRowIdxList; i++) {
		pDM_Volume->InsertRow(-1);
		CopyDM(pDM_Book, CopyRowIdxList.GetAt(i), pDM_Volume, RowIdx);
		ids = pDM_Volume->GetCellData(_T("BB_편권차"), RowIdx, curVOL_SORT);
		strItemData.Format(_T("%d"), GetBookCount(pDM_Book, _T("BB"), _T(""), curVOL_SORT));
		ids = pDM_Volume->SetCellData(_T("BB_책수"), strItemData, RowIdx);

		RowIdx++;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::DeleteBookDMRow(CESL_DataMgr *pDM_Book, 
								INT idx, 
								CString strQueryFlag /*=_T("")*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strQueryFlag.IsEmpty())
	{
		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
	}

	CString strKey = GetBookKey(pDM_Book, idx, _T("BB"));
	
	if (strQueryFlag == _T("D")) 
	{
		
	}
	else if (strQueryFlag == _T("I"))
	{
		pDM_Book->DeleteRow(idx);
	}
	else if (strQueryFlag == _T("U"))
	{
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}
	else
	{
		pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), idx);
	}

	if (strKey.IsEmpty()) return 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT i = nRowCount - 1; i >= 0; i--)
	{
		if (!IsAppendix(pDM_Book, i, _T("BB"))) continue;

		if (pDM_Book->GetCellData(_T("BB_부모책KEY"), i) != strKey) continue;

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), i);

		if (strQueryFlag == _T("D"))
		{
		}
		else if (strQueryFlag == _T("I"))
		{
			pDM_Book->DeleteRow(i);
		}
		else if (strQueryFlag == _T("U"))
		{
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), i);
		}
		else
		{
			pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("D"), i);
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::DeleteSpecies(CStringArray *pArrStrSpeciesKey)
{
	if (!pArrStrSpeciesKey) return -1;

	INT ids = -1;
	INT nSpeciesCount = pArrStrSpeciesKey->GetSize();
	if (nSpeciesCount < 1) return -1;

	CESL_DataMgr DM_Species;
	CESL_DataMgr DM_Book;
	CESL_DataMgr DM_Volume;
	CESL_DataMgr * DM_Toc = new CESL_DataMgr(); 

	DM_Toc->SetCONNECTION_INFO(m_pParnetMgr->m_pInfo->CONNECTION_INFO);
	
	DM_Species.SetCONNECTION_INFO(m_pParnetMgr->m_pInfo->CONNECTION_INFO);
	DM_Species.InitDMPKFromFile(_T("DM_단행_정리DM_종정보"));
	DM_Species.SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);
	
	DM_Book.SetCONNECTION_INFO(m_pParnetMgr->m_pInfo->CONNECTION_INFO);
	DM_Book.InitDMPKFromFile(_T("DM_단행_정리DM_책정보"));
	DM_Book.SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);

	DM_Volume.SetCONNECTION_INFO(m_pParnetMgr->m_pInfo->CONNECTION_INFO);
	DM_Volume.InitDMPKFromFile(_T("DM_단행_정리DM_권정보"));
	DM_Volume.SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);

	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = DM_Volume.RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)DM_Volume.RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == ( _T("BB_삭제날자")))
		{
			pRef->DATA_TYPE = _T("");
			break;
		}
	}

	CString strCurDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	
	LPCTSTR lpszDeleteSpeciesTblName = _T("BO_DELETE_SPECIES_TBL");
	LPCTSTR lpszDeleteBookTblName = _T("BO_DELETE_BOOK_TBL");

	CArray <CString, CString> RemoveAliasList;

	ids = DM_Species.StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	INT nBookCount = -1;
	CString strTemp;
	CString strRegNo;
	CString strOption;
	CString strQuery;
	CString strRecKey;
	CString strSpeciesKey;
	CString strTocKey;
	
	CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->SetCONNECTION_INFO(m_pParnetMgr->CONNECTION_INFO);
	pDM_EBook->InitDMPKFromFile(_T("DMUP_단행_종_EBOOK"));
	pDM_EBook->SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);

	for (INT idxSpecies = 0; idxSpecies < nSpeciesCount; idxSpecies++)
	{
		strSpeciesKey = pArrStrSpeciesKey->GetAt(idxSpecies);
		if (strSpeciesKey.IsEmpty()) continue;

		strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
		ids = DM_Species.RefreshDataManager(strOption);
		if (ids < 0) continue;

		if (DM_Species.GetRowCount() != 1) continue;
		
		RemoveAliasList.RemoveAll();
		ids = DM_Species.SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("목록완성삭제"),WORK_LOG), 0);

		ids = DM_Species.SetCellData(_T("BS_제어번호"), _T(""), 0);
		
		DM_Species.MakeQueryByDM(RemoveAliasList, &DM_Species, lpszDeleteSpeciesTblName, 0, &DM_Species);

		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), DM_Species.TBL_NAME, strSpeciesKey);
		ids = DM_Species.AddFrame(strQuery);
		if (ids < 0) return ids;

		strQuery.Format(_T("UPDATE IDX_BO_TBL SET SPECIES_CLASS = '3', DELETE_DATE = '%s' WHERE REC_KEY = %s;"), strCurDate, strSpeciesKey);
		ids = DM_Species.AddFrame(strQuery);
		if (ids < 0) return ids;

		ids = DM_Species.MakeRecKey(strRecKey);
		if (ids < 0) return ids;

		strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"),strSpeciesKey);
		ids = DM_Toc->RestructDataManager(strQuery);
		if( ids < 0 ) continue;

		INT nCnt = DM_Toc->GetRowCount();
		if( nCnt > 0 )
		{
			for( INT i = 0; i<nCnt ; i++ )
			{
				strTocKey = DM_Toc->GetCellData(i,0);
				strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = '%s';"),strTocKey);
				ids = DM_Species.AddFrame(strQuery);
				if (ids < 0) return ids;

				strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE REC_KEY = '%s';"),strTocKey);
				ids = DM_Species.AddFrame(strQuery);
				if (ids < 0) return ids;

				strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = '%s';"),strTocKey);
				ids = DM_Species.AddFrame(strQuery);
				if (ids < 0) return ids;

				strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = '%s';"),strTocKey);
				ids = DM_Species.AddFrame(strQuery);
				if (ids < 0) return ids;
			}
		}

		strOption.Format(_T("b.SPECIES_KEY = %s"), strSpeciesKey);
		ids = DM_Book.RefreshDataManager(strOption);
		if (ids < 0) continue;

		CString sBookKeyList;
		nBookCount = DM_Book.GetRowCount();
		for (INT idxBook = 0; idxBook < nBookCount; idxBook++)
		{
			strRegNo = DM_Book.GetCellData( _T("BB_등록번호"), idxBook);
			if(!strRegNo.IsEmpty()) 
				InsertToDropAccessionNoTbl(strRegNo, &DM_Species, FALSE);

			strRecKey = DM_Book.GetCellData( _T("BB_책KEY"), idxBook);
			if (strRecKey.IsEmpty()) continue;

			sBookKeyList = sBookKeyList + _T(", ") + strRecKey;
		}

		strQuery.Format(_T("DELETE FROM %s WHERE b.SPECIES_KEY = %s;"), DM_Book.TBL_NAME, strSpeciesKey);
		ids = DM_Species.AddFrame(strQuery);
		if (ids < 0) return ids;

		CString strEbookQeury ;
		INT nRowCnt ;
		strEbookQeury.Format (_T("BOOK_KEY IN ( SELECT REC_KEY FROM BO_BOOK_TBL b WHERE %s)"),strOption ) ;
        ids = pDM_EBook->RefreshDataManager (strEbookQeury);
		nRowCnt = pDM_EBook->GetRowCount ();
		for(INT i = 0; i < nRowCnt ;i++)
		{
			pDM_EBook->SetCellData (_T("BB_DB_FLAG"),_T("D"),i) ;
		}
		CBO_SPECIES  pBO_SPECIES(m_pParnetMgr) ;
		CString strTmpData ;
		
		RemoveAliasList.RemoveAll();
		RemoveAliasList.Add( _T("BB_등록번호"));
		RemoveAliasList.Add( _T("BB_복본기호"));
		
		ids = ConvertBookToVolume(&DM_Book, &DM_Volume);
		if (ids < 0) continue;

		nBookCount = DM_Volume.GetRowCount();
		for (idxBook = 0; idxBook < nBookCount; idxBook++)
		{
			ids = DM_Species.MakeRecKey(strRecKey);
			if (ids < 0) return ids;
			DM_Volume.SetCellData( _T("BB_책KEY"), strRecKey, idxBook);
			DM_Volume.SetCellData( _T("BB_삭제날자"), strCurDate, idxBook);
			DM_Volume.SetCellData(_T("FIRST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG) , idxBook);
			DM_Volume.SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc입력"),WORK_LOG) , idxBook);

			DM_Species.MakeQueryByDM(RemoveAliasList, &DM_Volume, lpszDeleteBookTblName, idxBook, &DM_Species);
		
			strTmpData = DM_Book.GetCellData (_T("BB_책KEY"),idxBook);
			pBO_SPECIES.SetEBookDMDBFlag (pDM_EBook,strTmpData,_T(""));
		}
	
        pBO_SPECIES.MakeEBookQueryFrame (pDM_EBook,&DM_Species) ;
    }

	CString sEcoSearch;
	if ( m_pParnetMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
		sEcoSearch.Format( _T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey );
		ids = DM_Species.AddFrame(sEcoSearch);
		if ( 0 > ids ) return -1;
	}
	else if ( m_pParnetMgr->m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
		AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
		return -1;
	}	

	ids = DM_Species.SendFrame();
	if (ids < 0) return ids;

	ids = DM_Species.EndFrame();
	if (ids < 0) return ids;

	return 0;
}

INT CCatApi_BO::DeleteVolume(CESL_DataMgr *pDM_Book, 
							 CString strVol)
{
	EFS_BEGIN

	if (pDM_Book == NULL || strVol.IsEmpty()) return -1;
	
	INT nRowCount = pDM_Book->GetRowCount();

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (pDM_Book->GetCellData(_T("BB_편권차"), idx) == strVol)
		{
			DeleteBookDMRow(pDM_Book, idx);
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::GetBookPriceSum(CESL_DataMgr *pDM_Book, 
								INT &nSum)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	
	nSum = 0;
	INT ids = -1;
	INT nResult = 0;
	CString strTemp;


	INT nRowCount = pDM_Book->GetRowCount();
	if (nRowCount <= 0) return -1;

	for (INT idx = 0 ; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData(_T("BB_가격"), idx, strTemp);
		if (ids < 0 || strTemp.IsEmpty()) 
		{
			nResult ++;
			continue;
		}

		nSum += _ttoi(strTemp);		
	}

	return nResult;

	EFS_END
	return -1;
}

INT CCatApi_BO::GetLastDupCodeNumber(CESL_DataMgr *pDM_Book, 
									 CString strVol)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	INT nDupNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		CString strTemp;
		INT nNumber = 0;
		TCHAR ch = ' ';

		if (!IsBook(pDM_Book, idx, _T("BB"), strVol)) continue;

		strTemp = pDM_Book->GetCellData(_T("BB_복본기호"), idx);
		if (strTemp.IsEmpty()) continue;

		_stscanf(strTemp, _T("%c%d"), &ch, &nNumber);
		
		nNumber = _ttoi(strTemp);

		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;

	EFS_END
	return -1;
}

INT CCatApi_BO::ImportSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix /*= NULL*/, 
							  CStringArray *pArrStrAddFrame /*= NULL*/, 
							  LPCTSTR lpszShelfDate /*= NULL*/,
							  INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;

	CESL_DataMgr *DM_LastNumber = NULL;

	if ( nAppMode == MNG_IMPORT && nAppMode == MNG_IMPORT_WORKING )
	{
		DM_LastNumber = m_pParnetMgr->FindDM( _T("DM_마크반입_마지막번호") );
		
		CString sLastNumberQuery = _T("") ;

		sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
								_T("WHERE KIND_CODE='REG_NO' AND MANAGE_CODE=UDF_MANAGE_CODE"));

		DM_LastNumber->RestructDataManager(sLastNumberQuery);

		nRowCount = pDM_Book->GetRowCount();
	}

	
	nRowCount = pDM_Book->GetRowCount();

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;


		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
			}
		}

		pDM_Book->SetCellData( _T("BB_등록번호"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, pDM_Book->TBL_NAME, strRegNo );

		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			m_strLastErrMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
			AfxMessageBox ( m_strLastErrMsg );
			return -1;
		}

	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	ids = ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;

	ApplyLastModifyData(&marc);

	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("BS_정리된자료"), _T("Y"), 0 );
	
	ids = pDM_Species->SetCellData( _T("BS_작업상태구분"), _T("1"), 0 );	
	
	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, nAppMode, pArrStrAddFrame, FALSE);

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

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("BS_종KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("BB_책KEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("BB_책KEY"), idx));
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

INT CCatApi_BO::WebImportSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix /*= NULL*/, 
							  CStringArray *pArrStrAddFrame /*= NULL*/, 
							  LPCTSTR lpszShelfDate /*= NULL*/,
							  INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;


	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;

	CESL_DataMgr *DM_LastNumber = NULL;

	if ( nAppMode == MNG_IMPORT && nAppMode == MNG_IMPORT_WORKING )
	{
		DM_LastNumber = m_pParnetMgr->FindDM( _T("DM_마크반입_마지막번호") );
		
		CString sLastNumberQuery = _T("") ;
	
		sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
								_T("WHERE KIND_CODE='REG_NO' AND MANAGE_CODE=UDF_MANAGE_CODE"));

		DM_LastNumber->RestructDataManager(sLastNumberQuery);

		nRowCount = pDM_Book->GetRowCount();
	}

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;
		
		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
			}
		}

		pDM_Book->SetCellData( _T("BB_등록번호"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, pDM_Book->TBL_NAME, strRegNo );

		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			m_strLastErrMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
			AfxMessageBox ( m_strLastErrMsg );
			return -1;
		}

	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	CString strShelfLocCode;
	pDM_Book->GetCellData(_T("BB_배가위치부호"), 0, strShelfLocCode );
	if ( strShelfLocCode==_T("") )
	{
		ids = ApplyShelfCodeData(pDM_Book, &marc, FALSE);
		if (ids < 0) return -1;
	}	

	ApplyLastModifyData(&marc);

	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("BS_정리된자료"), _T("Y"), 0 );
	
	ids = pDM_Species->SetCellData( _T("BS_작업상태구분"), _T("1"), 0 );

	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, nAppMode, pArrStrAddFrame, FALSE);

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

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("BS_종KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("BB_책KEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("BB_책KEY"), idx));
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

INT CCatApi_BO::DMDPImportSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix /*= NULL*/, 
							  CStringArray *pArrStrAddFrame /*= NULL*/, 
							  LPCTSTR lpszShelfDate /*= NULL*/,
							  INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;


	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;

	CESL_DataMgr *DM_LastNumber = NULL;

	if ( nAppMode == MNG_IMPORT && nAppMode == MNG_IMPORT_WORKING )
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

	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;
				
		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
			}
		}

		pDM_Book->SetCellData( _T("BB_등록번호"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s'"), pDM_Book->TBL_NAME, strRegNo);
		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			m_strLastErrMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
			AfxMessageBox ( m_strLastErrMsg );
			return -1;
		}

	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	CString strShelfLocCode;
	pDM_Book->GetCellData(_T("BB_배가위치부호"), 0, strShelfLocCode );
	if ( strShelfLocCode==_T("") )
	{
		ids = ApplyShelfCodeData(pDM_Book, &marc, FALSE);
		if (ids < 0) return -1;
	}	

	ApplyLastModifyData(&marc);

	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("BS_정리된자료"), _T("Y"), 0 );	
	
	ids = pDM_Species->SetCellData( _T("BS_작업상태구분"), _T("1"), 0 );

	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, nAppMode, pArrStrAddFrame, FALSE);
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

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("BS_종KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("BB_책KEY"), idx));
	}	

	ids = DMDPRecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, FALSE);
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

INT CCatApi_BO::InsertSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix, 
							  INT nAppMode,
							  CStringArray *pArrStrAddFrame /*= NULL*/, 
							  BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL
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

	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("BS"));

	ids = ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_Species->GetCellData( _T("BS_MARC"), 0, strStreamMarc);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strCurrencyCode;
	CString strItemData;
	CString strTemp;
	ids = m_pMarcMgr->GetItem(&marc, _T("950$b"), strItemData);
	if (ids >= 0)
	{
		if (!strItemData.IsEmpty()) 
		{
			strCurrencyCode = GetPriceCode(strItemData, strTemp);
		}
	}
	
	ids = m_pMarcMgr->DeleteField( &marc, _T("001") );
	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0 );

	CString strItemRecKey;
	pDM_Species->GetCellData(_T("BS_종KEY"), 0, strItemRecKey);
	if ( strItemRecKey.IsEmpty() )
	{
		ids = pDM_Species->MakeRecKey(strSpeciesKey);
		if (ids < 0) return ids;
		pDM_Species->SetCellData( _T("BS_종KEY"), strSpeciesKey, 0);
	}
	else 
	{
		strSpeciesKey = strItemRecKey;
	}

	pDM_Species->InitDBFieldData();


	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	LogToFile(pDM_Species->m_lastmsg);

	pDM_Index->SetCellData( _T("IBS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("IBS_종구분값"), _T("0"), 0);

	CString strConYN, strAbsYN;
	
	pDM_Index->GetCellData(_T("IBS_목차유무"), 0, strConYN);
	pDM_Index->GetCellData(_T("IBS_목차유무"), 0, strAbsYN);

	arrRemoveAliasList.RemoveAll();	
	ids = ApplyIndex(pDM_Species, pDM_Index, pDM_Book);
	pDM_Index->SetCellData(_T("IBS_목차유무"), strConYN, 0);
	pDM_Index->SetCellData(_T("IBS_목차유무"), strAbsYN, 0);
	
	ApplyIndex_ST_CODE(pDM_Index, pDM_Book);
	
	pDM_Species->InitDBFieldData();
	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species);
	LogToFile(pDM_Species->m_lastmsg);

	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("IBS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("IBS_관리구분_코드"));
	Index_classfycodes.Add( _T("IBS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("IBS_등록구분_코드"));

	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("BB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("BB_관리구분_코드"));
	Book_classfycodes.Add( _T("BB_배가위치부호"));
	Book_classfycodes.Add( _T("BB_등록구분_코드"));

	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("BB"));


	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	CString sAcqCode = pDM_Book->GetCellData( _T("BB_수입구분_코드"), 0 );
	
	CString sAcqKey;
	ids = pDM_Species->MakeRecKey(sAcqKey);
	if ( 0 > ids ) return -1;

	CString sProvider = pDM_Book->GetCellData( _T("BB_구입처"), 0 );

	CTime t = CTime::GetCurrentTime();
	CString strInputDate;
	CString strAcqYear;
	strInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	strAcqYear = pDM_Book->GetCellData( _T("BB_수입년도"), 0 );

	CString strPrice; 
	strPrice = pDM_Book->GetCellData( _T("BB_가격"), 0 );


	if ( sAcqCode == _T("1") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("PROVIDER_CODE"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
		pDM_Species->AddDBFieldData( _T("ORDER_YEAR"), _T("STRING"), strAcqYear);
		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
		pDM_Species->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
		pDM_Species->MakeInsertFrame(_T("BO_PURCHASEINFO_TBL"));
	}
	else if ( sAcqCode == _T("2") ) {
		CString strDonatorKey;
		CESL_DataMgr *pDmDonatorMng = m_pParnetMgr->FindDM( _T("DM_단행_정리DM_색인정보") );
		if ( pDmDonatorMng == NULL )
		{
			AfxMessageBox( _T("[DM_단행_정리DM_색인정보] 확인!!") );
			return -1;
		}
		pDmDonatorMng->GetCellData( _T("IBS_기증자")   , 0, sProvider);
		pDmDonatorMng->GetCellData( _T("IBS_기증자KEY"), 0, strDonatorKey);

		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("DONATOR_NAME"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		pDM_Species->AddDBFieldData( _T("DONATOR_KEY"), _T("STRING"), strDonatorKey);
		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
		pDM_Species->AddDBFieldData( _T("ACQ_YEAR"), _T("STRING"), strAcqYear);
		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
		pDM_Species->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));

		pDM_Species->MakeInsertFrame(_T("BO_DONINFO_TBL"));
	}

	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		pDM_Book->SetCellData( _T("BB_수정정보KEY"), sAcqKey, i );
	}

	if ( sAcqKey != _T("") ) {
		ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, sAcqKey, pDM_Species);
	}
	else {
		ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("-1"), pDM_Species);
	}

	if (ids < 0)
	{
		if ( m_strLastErrMsg == _T("") ) {
			m_strLastErrMsg = _T("책정보 쿼리만들기 실패");
		}
		return ids;
	}

    CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->SetCONNECTION_INFO(m_pParnetMgr->CONNECTION_INFO);
	pDM_EBook->InitDMPKFromFile(_T("DMUP_단행_종_EBOOK"));
	pDM_EBook->SetMarcMgr(m_pParnetMgr->m_pInfo->pMarcMgr);

	CBO_SPECIES *pBO_Species = new CBO_SPECIES ( m_pParnetMgr );

	ids = pBO_Species->MakeEBookInfoAndQueryFrame (pDM_Book,pDM_EBook,pDM_Species) ;
    if (ids < 0) 
	{
		m_strLastErrMsg = _T("ebook정보 쿼리만들기 실패");
		return ids;
	}

	if ( pDM_Appendix != NULL && 0 < pDM_Appendix->GetRowCount())
	{
		ids = ApplyAppendixRegNo(pDM_Appendix, nAppMode);
		if (ids < 0)
		{
			m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");
			return ids;
		}

		CString strUseLimitCode;
		CString strPage;
		CString strPhysicalProperty;
		CString strBookSize;
		CString strAccompMat;
		INT nRowCnt;
		nRowCnt = pDM_Appendix->GetRowCount();

	    pDM_Index->GetCellData( _T("IBS_이용제한구분_코드") , 0, strUseLimitCode);

		CMarc marc;
		CString strTempField;
		CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
		m_pMarcMgr->Decoding(strStreamMarc, &marc);
		m_pMarcMgr->GetItem(&marc, _T("300$a"), strPage);
		m_pMarcMgr->GetItem(&marc, _T("300$b"), strPhysicalProperty);
		m_pMarcMgr->GetItem(&marc, _T("300$c"), strBookSize);
		m_pMarcMgr->GetItem(&marc, _T("300$e"), strAccompMat);

		for( INT i = 0 ; i < nRowCnt ; i++)
		{
			pDM_Appendix->SetCellData(_T("BB_이용제한구분_코드"), strUseLimitCode, i);
			pDM_Appendix->SetCellData(_T("BB_면장수"), strPage, i);
			pDM_Appendix->SetCellData(_T("BB_물리적특성"), strPhysicalProperty, i);
			pDM_Appendix->SetCellData(_T("BB_크기"), strBookSize, i);
			pDM_Appendix->SetCellData(_T("BB_딸림자료"), strAccompMat, i);
		}
		ids = MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("-1"), pDM_Species);
		if (ids < 0)
		{
			m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");			
			return ids;
		}
	}

	ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	
	if(m_pParnetMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = pDM_Species->AddFrame(strJJSQuery);
		if (ids < 0) return ids;
	}

	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_마지막번호 확인") );
		return -1;
	}

	INT iBookCount = pDM_Book->GetRowCount();
	pDM_Book->SORT(_T("BB_등록번호"));
	CString sRegno = pDM_Book->GetCellData( _T("BB_등록번호"), iBookCount-1 );
	INT iRegno = 0;

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	if ( sRegno != _T("") ) {
		iRegno = _ttoi(sRegno.Mid(nRegCodeLength));
	}

	for ( i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("BB_등록구분_코드"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("마지막번호"), i );
		INT iLastNum = _ttoi ( sLastNum );
		if ( iRegno != 0 ) {
			if(iLastNum < iRegno)
			{
				iLastNum = iRegno;
			}
		}
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

	if (bCommit)
	{
		ids = pDM_Species->SendFrame();
		if ( 0 > ids ) return -1;		
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

INT CCatApi_BO::MakeBookFromRegCode(CArray<CCatRegCode, 
									CCatRegCode> *pArrRegCode, 
									CESL_DataMgr *pDM_Book, 
									CString strVol)
{
	EFS_BEGIN

	if (pArrRegCode == NULL || pDM_Book == NULL) return -1;
	
	INT ids = -1;
	INT nDMRowCount = pDM_Book->GetRowCount();
	if (nDMRowCount == 0) return -1;

	INT nRegCodeCount = pArrRegCode->GetSize();
	if (nRegCodeCount == 0) return -1;

	CString strRegCode;
	CString strQueryFlag;

	CCatRegCode regCode;
	INT nBookCount;
	for (INT idx = 0; idx < nRegCodeCount; idx++)
	{
		regCode = pArrRegCode->GetAt(idx);
		nBookCount = GetBookCount(pDM_Book, _T("BB"), regCode.m_strRegCode, strVol);

		if (regCode.m_nCount < nBookCount)
		{
			RemoveBook(pDM_Book, regCode.m_strRegCode, strVol, nBookCount - regCode.m_nCount);
		}
		else if (regCode.m_nCount > nBookCount)
		{
			AddBook(pDM_Book, regCode.m_strRegCode, strVol, regCode.m_nCount - nBookCount);
		}
	}

	nDMRowCount = pDM_Book->GetRowCount();
	for (idx = nDMRowCount - 1; idx >= 0; idx--)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"), strVol)) continue;
		if (pDM_Book->GetCellData(_T("BB_복본기호"), idx) == _T("1"))
			pDM_Book->SetCellData(_T("BB_복본기호"), _T(""), idx);

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx);
		if (!IsInRegCode(pArrRegCode, strRegCode))
			DeleteBookDMRow(pDM_Book, idx, strQueryFlag);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::MakeBookQueryFrame(CESL_DataMgr *pDM_Book, 
								   CString strSpeciesKey, 
								   CString strPurchaseinfo_key, 
								   CESL_DataMgr *pDM_OUT, 
								   INT nSendCount/*=-1*/)
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

	INT nRowCount = pDM_Book->GetRowCount();
	
	CString strIsRepresentBook;
	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	for (idx = 0; idx < nRowCount; idx++) 
	{
		ids = pDM_Book->GetCellData( _T("BB_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		if (strQueryFlag == _T("I"))
		{
			ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
			
			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
				if ( strRegNo.GetLength() > nRegCodeLength ) {
					strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
				}
			}

            if( strRegNo.IsEmpty ())
			{
				AfxMessageBox(_T("등록번호가 부여되지 않은 책이 있습니다.\r\n 등록번호를 확인하여 주십시요"));
				return -1;
			}

			if (ids >= 0 && !strRegNo.IsEmpty())
			{
				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
								, pDM_Book->TBL_NAME, strRegNo);

				ids = pDM_OUT->GetOneValueQuery(strQuery, strResult);
				if (ids >= 0 && !strResult.IsEmpty())
				{
					m_strLastErrMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
					return -1000;
				}
			}

			ids = pDM_OUT->MakeRecKey(strBookKey);
			if (ids < 0) return ids;
			pDM_Book->SetCellData( _T("BB_책KEY"), strBookKey, idx);

			CTime t = CTime::GetCurrentTime();
			CString str;
			str.Format(_T("%04d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
			
			pDM_Book->SetCellData( _T("BB_입력일"), str, idx);
            pDM_Book->SetCellData( _T("BB_일련번호"),_T("-1"), idx);
		}
	}
	
	for (idx = 0; idx < nRowCount; idx++) {
		RemoveAliasList.RemoveAll();
		ids = pDM_Book->GetCellData( _T("BB_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		CString sPrice;
		CString sRealPrice;
		ids = pDM_Book->GetCellData( _T("BB_가격"), idx, sPrice);
		if ( sPrice != _T("") ) {
			for( INT i = 0 ; i < sPrice.GetLength() ; i++ ) {
				if ( (sPrice.Mid( i, 1 ) >= _T("0") && 
					 sPrice.Mid( i, 1 ) <= _T("9")) || 
					 sPrice.Mid( i, 1 ) == _T(".") 
				   ) 
				{
					sRealPrice = sRealPrice + sPrice.Mid( i, 1 );
				}
			}
			ids = pDM_Book->SetCellData( _T("BB_가격"), sRealPrice, idx);
		}

		if (strQueryFlag == _T("I")) {
			pDM_Book->SetCellData( _T("BB_종KEY"), strSpeciesKey, idx);
			pDM_Book->SetCellData( _T("BB_수서정보KEY"), strPurchaseinfo_key, idx);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, pDM_Book->TBL_NAME, idx, pDM_OUT);
			LogToFile(pDM_OUT->m_lastmsg);
		}
		else if (strQueryFlag == _T("U")) 
		{
			RemoveAliasList.Add( _T("BB_책KEY") );

			ids = pDM_Book->GetCellData( _T("BB_책KEY"), idx, strBookKey);
			
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, pDM_Book->TBL_NAME, idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
			LogToFile(pDM_OUT->m_lastmsg);
		}
		else if (strQueryFlag == _T("D")) 
		{
			ids = pDM_Book->GetCellData( _T("BB_책KEY"), idx, strBookKey);
			
			strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM_Book->TBL_NAME, strBookKey);
			ids = pDM_OUT->AddFrame(strQuery);
			if (ids < 0) return ids;

			CString strRegNo;
			ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
			if ( strRegNo != _T("")){

				INT iRegNumber;
				iRegNumber = _ttoi(strRegNo.Mid(3, strRegNo.GetLength()));
			
				CString strRegCode;
				pDM_Book->GetCellData( _T("BB_등록구분_코드"), idx, strRegCode);

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

			if (IsBook(pDM_Book, idx, _T("BB")))
			{
				CString strRecNo = pDM_Book->GetCellData( _T("BB_등록번호"), idx);
				if(!strRecNo.IsEmpty()) 
					InsertToDropAccessionNoTbl(strRecNo, pDM_OUT, TRUE);
			}
			LogToFile(pDM_OUT->m_lastmsg);
		}
	}

	return 0;

EFS_END
	return -1;
}

INT CCatApi_BO::MakeMarcAdjustDM(CESL_DataMgr *pDM_From_Species, 
								 INT nFromSpeciesRow, 
								 CESL_DataMgr *pDM_From_Index, 
								 INT nFromIndexRow, 
								 CESL_DataMgr *pDM_To_Species, 
								 CESL_DataMgr *pDM_To_Index, 
								 CESL_DataMgr *pDM_To_Book, 
								 CESL_DataMgr *pDM_To_Volume)
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
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);
	

	CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);

	CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);

	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	for ( INT i = 0; i < nRowCount ; i++)
	{
		CString sTmp049;
		sTmp049 = arrItem.GetAt(i);		
		
		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
			}
		}
		arrItem.SetAt(i, sTmp049);
	}

	if (nRowCount > 0)
		pDM_To_Book->FreeData();

	else if ( nRowCount == 0 ) 
	{
		pDM_To_Book->FreeData();

		pDM_To_Book->InsertRow(-1);

		pDM_To_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), 0);

		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), 0);

		CopyDM(pDM_To_Species, 0, pDM_To_Book, 0, _T("BB"));

		CopyDM(pDM_To_Index, 0, pDM_To_Book, 0, _T("BB"));
	}

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		strRegNo = arrItem.GetAt(idx);

		pDM_To_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

		CString sRegCode;
		if ( strRegNo.GetLength() >= nRegCodeLength ) {
			sRegCode = strRegNo.Left(nRegCodeLength);
			pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
		}

		pDM_To_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), idx);

		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));

		CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
	}

	INT nRepBookIdx = SetRepresentationBook(pDM_To_Book, _T("BB"));

	ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::RemoveBook(CESL_DataMgr *pDM_Book, 
						   CString strRegCode, 
						   CString strVol, 
						   INT nCount /*= 1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	CString strQueryFlag;
	INT nDeletedCount = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"), strVol)) continue;

		if (pDM_Book->GetCellData(_T("BB_등록구분_코드"), idx) != strRegCode) continue;

		strQueryFlag = pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		
		if (DeleteBookDMRow(pDM_Book, idx, strQueryFlag) < 0) continue;

		if (nCount == ++nDeletedCount) break;
	}

	return nDeletedCount;

	EFS_END
	return -1;
}

INT CCatApi_BO::RecordLocDate(CESL_DataMgr *pDM_Out, 
							  CStringArray *pArrSpeciesKey, 
							  CStringArray *pArrBookKey, 
							  CStringArray *pArrAppendixKey, 
							  CString &strShelfDate, 
							  BOOL bCommit /*= TRUE*/)
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

		strQuery.Format(_T("UPDATE IDX_BO_TBL SET HOLD_DATE = '%s', SPECIES_CLASS = '1', WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;")
							, strShelfDate, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET LOC_FLAG = '0' WHERE REC_KEY = %s;"), strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
		
	}

	nRowCount = pArrBookKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrBookKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'BOL112N', SHELF_WORKER = '%s' WHERE REC_KEY = %s;")
			, strShelfDate, m_pParnetMgr->m_pInfo->USER_ID, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		CString strWhere;
		CString strAcqCode;
		CString strAcqKey;
		strQuery.Format(_T("SELECT ACQ_CODE FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strRecKey);
		pDM_Out->GetOneValueQuery(strQuery, strAcqCode);
		if (!strAcqCode.IsEmpty())
			strWhere.Format(_T("ACQ_CODE = '%s'"), strAcqCode);
		
		strQuery.Format(_T("SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strRecKey);
		pDM_Out->GetOneValueQuery(strQuery, strAcqKey);
		if (!strAcqKey.IsEmpty()) {
			if ( strWhere != _T("") ) {
				strWhere += _T(" AND ACQ_KEY = ") + strAcqKey;
			}
			else {
				strWhere += _T("ACQ_KEY = ") + strAcqKey;
			}
		}

		if ( strWhere != _T("") ) {
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS = '2' WHERE %s;"), strWhere);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}

	if (pArrAppendixKey)
	{
		nRowCount = pArrAppendixKey->GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strRecKey = pArrAppendixKey->GetAt(idx);
			if (strRecKey.IsEmpty()) continue;

			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'BOL112N', SHELF_WORKER = '%s' WHERE REC_KEY = %s;")
				,strShelfDate, m_pParnetMgr->m_pInfo->USER_ID, strRecKey);
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

INT CCatApi_BO::DMDPRecordLocDate(CESL_DataMgr *pDM_Out, 
							  CStringArray *pArrSpeciesKey, 
							  CStringArray *pArrBookKey, 
							  CStringArray *pArrAppendixKey, 							  
							  BOOL bCommit /*= TRUE*/)
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

		strQuery.Format(_T("UPDATE IDX_BO_TBL SET SPECIES_CLASS = '0', WORKING_STATUS = 'BOC113N' WHERE REC_KEY = %s;")
							, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
		
		strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET LOC_FLAG = '1' WHERE REC_KEY = %s;"), strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='2', WORKING_STATUS='BOC113N', VOL_SORT_NO=10 WHERE SPECIES_KEY = %s;"), strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
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

INT CCatApi_BO::UpdateRepresentVolume(CESL_DataMgr *pDM_Book, 
									  CString strAlias, 
									  CString strItemData, 
									  INT nIdx/*=-1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (nIdx == -1)
	{
		GetRepresentationBook(pDM_Book, _T(""), nIdx);
		if (nIdx < 0) return -1;
	}

	CString strVol;
	INT ids = pDM_Book->GetCellData(_T("BB_편권차"), nIdx, strVol);

	if (ids < 0) return ids;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"), strVol)) continue;

		pDM_Book->SetCellData(strAlias, strItemData, idx);
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_BO::InitLastNumDM()
{
	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("[DM_마지막번호]의 초기화에 실패하였습니다") );
		return -1;
	}
	pDmLastNum->FreeData();

	return 0;
}

VOID CCatApi_BO::FindSabdoToDesc(CString strKey, CString &strValue)
{
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapSabdoToDesc;

	mapSabdoToDesc.SetAt(_T("a"), _T("삽도"));
	mapSabdoToDesc.SetAt(_T("b"), _T("지도"));
	mapSabdoToDesc.SetAt(_T("c"), _T("초상화"));
	mapSabdoToDesc.SetAt(_T("d"), _T("챠트"));
	mapSabdoToDesc.SetAt(_T("e"), _T("설계도"));
	mapSabdoToDesc.SetAt(_T("f"), _T("도판"));
	mapSabdoToDesc.SetAt(_T("g"), _T("악보"));
	mapSabdoToDesc.SetAt(_T("h"), _T("팩시밀리"));
	mapSabdoToDesc.SetAt(_T("i"), _T("문장"));
	mapSabdoToDesc.SetAt(_T("j"), _T("연대표"));
	mapSabdoToDesc.SetAt(_T("k"), _T("서식"));
	mapSabdoToDesc.SetAt(_T("l"), _T("견본"));
	mapSabdoToDesc.SetAt(_T("m"), _T("음반"));
	mapSabdoToDesc.SetAt(_T("n"), _T("수표"));
	mapSabdoToDesc.SetAt(_T("o"), _T("사진"));

	mapSabdoToDesc.Lookup(strKey, strValue);

}
