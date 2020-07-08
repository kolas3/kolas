// SE_LOC_1110.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1110.h"

#include "..\..\..\..\단행\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"
// SanFile을 오픈하기 위해
#include "..\..\..\..\단행\열람\장서관리\OpenSanFile\\InputSanFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1110 dialog


CSE_LOC_1110::CSE_LOC_1110(CESL_Mgr* pParent /*=NULL*/, CString strMenuAlias)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Result = NULL;
	m_pCM_Parent = NULL;

	m_bDetail = TRUE;

	m_strMenuAlias = strMenuAlias;

	m_pMakeSearchData = new CMakeSearchData(this);
}

CSE_LOC_1110::~CSE_LOC_1110()
{
	delete m_pMakeSearchData;
}

BOOL CSE_LOC_1110::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CSE_LOC_1110::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1110, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1110)
	ON_BN_CLICKED(IDC_bSEARCH_CAT, OnbSEARCHCAT)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDetail)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bFileSearch, OnbFileSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString CSE_LOC_1110::GetWorkingStatusSQL()
{
	EFS_BEGIN

	CString strResult;

	CString strWorkingStatus;
	m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);

	if (strWorkingStatus == _T("열람인계자료"))
	{
		strResult = _T("b.WORKING_STATUS = 'SEL211O'");
	}
	else if( strWorkingStatus == _T("소장자료"))
	{																	
		// 2005-12-27 UPD BY PDW(Jr) ===============================================================================
		/* 수정전
		strResult = _T("b.WORKING_STATUS IN ("HOLD_SE_SEARCH_CONDITION_T(") AND b.PREV_WORKING_STATUS NOT IN ('SEL112N')"));
		*/
		strResult = _T("b.WORKING_STATUS IN (")_T(HOLD_SE_SEARCH_CONDITION)_T(",'SEL312O')");
		// =========================================================================================================
	}

	return strResult;

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1110 message handlers

BOOL CSE_LOC_1110::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SE_LOC_1110")) < 0)
	{
		AfxMessageBox( _T("CSE_LOC_1110::OnInitDialog() ERROR -1") );
		return FALSE;
	}

	m_pDM_Result = FindDM(_T("DM_SE_LOC_1100"));
	m_pCM_Parent = FindCM(_T("CM_SE_LOC_1100"));
	m_pCM = FindCM(_T("CM_SE_LOC_1110"));


	if (m_pDM_Result == NULL ||
		m_pCM_Parent == NULL ||
		m_pCM == NULL) 
	{
		AfxMessageBox( _T("CSE_LOC_1110::OnInitDialog() ERROR -1") );
		return FALSE;
	}

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cmbREGCODE);
	if ( !pCB )
	{
		AfxMessageBox( _T("CSE_LOC_1110::OnInitDialog() ERROR -1") );
		return FALSE;
	}
	pCB->SetCurSel( -1 );

	OnbDetail();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
	return FALSE;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-05 ~ 2005-12-05
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   함수설명   ] 
      [연속-배가일자기록] 검색하기 
   
   [   미니스펙   ]
      1. 기본 조건절 생성
	  2. 추가 조건절 생성 
--------------------------------------------------------------------------------------------*/
VOID CSE_LOC_1110::OnbSEARCHCAT() 
{
EFS_BEGIN

	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );
	
	CStringList whereList;
	CString sBuf;
		
	// ----------------------------------------------------------------------------
	// 1. 기본 조건절 생성     
	// ----------------------------------------------------------------------------
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sWORKING_STATUS;
	CString sControlNo, sMAT_CODE , sMEDIA_CODE;
	CString sTitle, sKeyWord, sAuthor, sPublisher;
	
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	m_pCM->GetControlMgrData(_T("BB_수입구분_코드"), sACQUISIT_CODE);
	m_pCM->GetControlMgrData(_T("BS_수입년도"), sACQUISIT_YEAR);
	// 2005-12-15 UPD BY PDW(Jr) ==================================================
	// 등록번호 입력 형식 수정	
	/* 수정전
	CString sRegNoStart, sRegNoEnd;
	m_pCM->GetControlMgrData(_T("BB_등록번호_시작"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_등록번호_까지"), sRegNoEnd);

	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > 2 ) {
			CString sRight;
			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
			sRegNoStart = sRegNoStart.Left(2) + sRight;
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > 2 ) {
			CString sRight;
			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
		}
	}
	*/
	CString sRegCode    = _T("");
	CString sRegNoStart = _T("");
	CString sRegNoEnd   = _T("");
	m_pCM->GetControlMgrData(_T("BB_등록구분"),      sRegCode);
	m_pCM->GetControlMgrData(_T("BB_등록번호_시작"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_등록번호_까지"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( !sRegCode.IsEmpty() )
// 	{
// 		if ( !sRegNoStart.IsEmpty() && sRegNoStart.GetLength() < 10 )
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart );
// 			sRegNoStart = sRegCode + sRight;
// 		}
// 
// 		if ( !sRegNoEnd.IsEmpty() && sRegNoEnd.GetLength() < 10 )
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd );
// 			sRegNoEnd = sRegCode + sRight;
// 		}
// 	}	
// 	else
// 	{
// 		if ( !sRegNoStart.IsEmpty() || !sRegNoEnd.IsEmpty() )
// 		{
// 			AfxMessageBox( _T("등록구분을 입력 하세요."));
// 			return;
// 		}
// 	}
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	if ( !sRegCode.IsEmpty() )
	{
		if ( !sRegNoStart.IsEmpty() && sRegNoStart.GetLength() < 12-nRegCodeLength )
		{
			sRegNoStart = m_pInfo->MakeRegNo(sRegCode,sRegNoStart);
		}

		if ( !sRegNoEnd.IsEmpty() && sRegNoEnd.GetLength() < 12-nRegCodeLength )
		{
			sRegNoEnd = m_pInfo->MakeRegNo(sRegCode,sRegNoEnd);
		}
	}	
	else
	{
		if ( !sRegNoStart.IsEmpty() || !sRegNoEnd.IsEmpty() )
		{
			AfxMessageBox( _T("등록구분을 입력 하세요."));
			return;
		}
	}
	//=====================================================
	
	// ============================================================================

	m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), sWORKING_STATUS);
	m_pCM->GetControlMgrData(_T("BS_자료구분_코드"), sMAT_CODE);
	m_pCM->GetControlMgrData(_T("BS_매체구분_코드"), sMEDIA_CODE);
	m_pCM->GetControlMgrData(_T("IBS_본표제"), sTitle);
	m_pCM->GetControlMgrData(_T("IBS_키워드"), sKeyWord);
	m_pCM->GetControlMgrData(_T("IBS_저작자"), sAuthor);
	m_pCM->GetControlMgrData(_T("IBS_발행자"), sPublisher);
	
	//--------------------------------------------------------------------------
	// 2005-11-23 WORK ADD BY PDW(Jr)
	// 관리구분
	//--------------------------------------------------------------------------
	CString sManageCode;
	m_pCM->GetControlMgrData(_T("관리구분"), sManageCode);

	CString sWhere = _T("I.REC_KEY = S.REC_KEY");
	
	//자세히 버튼 체크
	
	//제어번호
	if (m_bDetail) {
		CString sControlNoStart, sControlNoEnd;
		m_pCM->GetControlMgrData(_T("제어번호부터"), sControlNoStart );
		m_pCM->GetControlMgrData(_T("제어번호까지"), sControlNoEnd );
		if ( sControlNoStart == _T("") && sControlNoEnd != _T("") ) {
			AfxMessageBox( _T("시작 제어번호를 입력하세요.") );
			return;
		}
		else if ( sControlNoStart != _T("") && sControlNoEnd == _T("") ) {
			sWhere = sWhere + _T(" AND I.CONTROL_NO = '") + sControlNoStart + _T("'");
		}
		else if ( sControlNoStart != _T("") && sControlNoEnd != _T("") ) {
			sWhere = sWhere + _T(" AND I.CONTROL_NO >= '") + sControlNoStart + _T("' AND I.CONTROL_NO <= '") + sControlNoEnd + _T("'");
		}
	}

	//코드값	
	if (m_bDetail) {
		sMAT_CODE.Replace ( _T(" ") , NULL );
		if ( sMAT_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sMEDIA_CODE.Replace ( _T(" ") , NULL );
		if ( sMEDIA_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}

		// 간행빈도
		CString strPubFreq;
		m_pCM->GetControlMgrData(_T("IBS_간행빈도"), strPubFreq);
		strPubFreq.Replace(_T(" "), _T(""));
		if (!strPubFreq.IsEmpty())
		{
			sWhere = sWhere + _T(" AND I.PUB_FREQ = '") + strPubFreq + _T("'");
		}
	}	

	//원부번호 검색
	CString strAccessionQuery;
	CString strAccestionNoMakeYear;
	CString strAccessionNoStart;
	CString strAccessionNoEnd;

	m_pCM->GetControlMgrData(_T("원부작성년도"), strAccestionNoMakeYear);
	m_pCM->GetControlMgrData(_T("원부번호부터"), strAccessionNoStart);
	m_pCM->GetControlMgrData(_T("원부번호까지"), strAccessionNoEnd);

	if (strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("시작 원부번호를 입력 하세요"));
		return;
	}
	else if (!strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("(SELECT MAX(a.accession_rec_no) FROM CO_ACCESSION_REC_TBL a WHERE BB.accession_rec_key = a.rec_key ) = %s  "), strAccessionNoStart);		
	}
	else if (!strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("(SELECT MAX(a.accession_rec_no) FROM CO_ACCESSION_REC_TBL a WHERE BB.accession_rec_key = a.rec_key ) BETWEEN %s and  %s"), strAccessionNoStart, strAccessionNoEnd);		
	}

	if (!strAccestionNoMakeYear.IsEmpty())
	{
		CString strAccessionQuery2;
		strAccessionQuery2.Format( _T("(SELECT MAX(a.ACCESSION_REC_MAKE_YEAR ) FROM CO_ACCESSION_REC_TBL a WHERE BB.accession_rec_key = a.rec_key ) = '%s' ") , strAccestionNoMakeYear );		
		if( !strAccessionQuery.IsEmpty() )
			strAccessionQuery += _T(" AND ");
		strAccessionQuery += strAccessionQuery2;
	}	
	
	// 차수 검색
	CString strSeqNoQuery;
	CString strSeqNoStart;
	CString strSeqNoEnd;
	m_pCM->GetControlMgrData(_T("차수번호부터"), strSeqNoStart);
	m_pCM->GetControlMgrData(_T("차수번호까지"), strSeqNoEnd);

	if (strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("시작 접수번호 입력하세요"));
		return;
	}
	else if (!strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("BB.RECEIPT_NO = %s"), strSeqNoStart);
	}
	else if (!strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("BB.RECEIPT_NO >= %s and BB.RECEIPT_NO <= %s"), strSeqNoStart, strSeqNoEnd);
	}

	//자료상태
	CString strWorkingStatusQuery = GetWorkingStatusSQL();
	CString sBookWhere = strWorkingStatusQuery;


	//수입구분, 수입년도
	sACQUISIT_CODE.Replace ( _T(" ") , NULL );
	if ( sACQUISIT_CODE != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND b.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}
	if ( sACQUISIT_YEAR != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//등록번호검색
	CString strWhere = sWhere;

	if (m_bDetail) {
		
		// 2005-11-23 ADD BY PDW(Jr) ==================================================
		// 관리구분 추가 
		if ( sManageCode != _T("") ) 
		{			
			sBookWhere = sBookWhere + _T(" AND b.MANAGE_CODE = '") + sManageCode + _T("'");
		}
		// ============================================================================		

		// 2005-12-15 UPD BY PDW(Jr) ==================================================
		// 등록구분 추가 
		if ( !sRegCode.IsEmpty() )
		{
			sBookWhere = sBookWhere + _T(" AND b.REG_CODE = '") + sRegCode + _T("'");
		}
		// ============================================================================		

		if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {			
			if ( sBookWhere != _T("") ) {
				if( !strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty() )
					sBookWhere = sBookWhere + _T(" AND b.REG_NO >= '") + sRegNoStart + _T("'");
				else
					sBookWhere = sBookWhere + _T(" AND b.REG_NO = '") + sRegNoStart + _T("'");
			}
		}		
		if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
			AfxMessageBox( _T("시작 등록번호를 입력 하세요."));
			return;
		}
		if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {	
			if ( sBookWhere == _T("") ) {
				sBookWhere = _T("b.REG_NO >= '") + sRegNoStart + _T("' AND b.REG_NO <= '") + sRegNoEnd + _T("'");
			}
			else {
				sBookWhere = sBookWhere + _T(" AND b.REG_NO >= '") + sRegNoStart + _T("' AND b.REG_NO <= '") + sRegNoEnd + _T("'");
			}
		}		

		bool bIsEqualSearch;

		//키워드 검색
		if ( sKeyWord != _T("") ){
			/// 2003년 9월 1일 CATSEARCH -> ECOSEARCH로 변환
			bIsEqualSearch = FALSE;
			// 평상시는 우절단 검색 생성
			if( sKeyWord.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			m_pMakeSearchData->GetKeyWord(sKeyWord);
			if( !bIsEqualSearch ) 
				sKeyWord += _T("*");
			
			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_KEYWORD, '") + sKeyWord + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_KEYWORD, '%s' ) "), sKeyWord );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );

/*
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
#endif
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_KEYWORD, '") + sKeyWord + _T("') ");
#endif
			*/
			
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//서명검색	
		//sTitle
		if ( sTitle != _T("") ){
			/// 2003년 9월 1일 CATSEARCH -> ECOSEARCH로 변환
			bIsEqualSearch = FALSE;
			// 평상시는 우절단 검색 생성
			if( sTitle.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			INT iCh = m_pMakeSearchData->ChangeHanjaToHangul( sTitle );	//한자 한글 변환
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			iCh = m_pMakeSearchData->GetKeyWord( sTitle );				//공백, 특수문자 제거
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			
			if( !bIsEqualSearch ) 
				sTitle += _T("*");
			
			
			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_TITLE, '") + sTitle + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_TITLE, '%s' ) "), sTitle );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );

		/*
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
#endif
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_TITLE, '") + sTitle + _T("') ");
#endif
			*/
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//저작자
		if ( sAuthor != _T("") ){
			/// 2003년 9월 1일 CATSEARCH -> ECOSEARCH로 변환
			bIsEqualSearch = FALSE;
			// 평상시는 우절단 검색 생성
			if( sAuthor.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			m_pMakeSearchData->GetKeyWord(sAuthor);
			if( !bIsEqualSearch ) 
				sAuthor += _T("*");
			
			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_AUTHOR, '") + sAuthor + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '%s' ) "), sAuthor );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );

/*
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
#endif
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_AUTHOR, '") + sAuthor + _T("') ");
#endif
			*/
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//발행자
		if ( sPublisher != _T("") ){
			/// 2003년 9월 1일 CATSEARCH -> ECOSEARCH로 변환
			bIsEqualSearch = FALSE;
			// 평상시는 우절단 검색 생성
			if( sPublisher.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			m_pMakeSearchData->GetKeyWord(sPublisher);
			if( !bIsEqualSearch ) 
				sPublisher += _T("*");

			CString sWhereTitle;

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fp )
					sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_PUBLISHER, '") + sPublisher + _T("') ");
				else
				{
					sBuf.Format( _T(" &ECOSEARCH( , SE_IDX_PUBLISHER, '%s' ) "), sPublisher );
					whereList.AddTail( sBuf );
				}
			}
			else
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );


/*
#ifdef _ECOSEARCH_
			sWhereTitle = _T(" &ECOSEARCH( I.REC_KEY , SE_IDX_PUBLISHER, '") + sPublisher + _T("') ");
#endif
#ifndef _ECOSEARCH_
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );
#endif
			*/
			if( !sWhereTitle.IsEmpty() )
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//발행년도
		CString sPUB_YEAR_START, sPUB_YEAR_END;
		m_pCM->GetControlMgrData( _T("IBS_발행년도2_시작"), sPUB_YEAR_START);
		m_pCM->GetControlMgrData( _T("IBS_발행년도2_까지"), sPUB_YEAR_END);
		if ( sPUB_YEAR_START == _T("") && sPUB_YEAR_END != _T("") ) {
			AfxMessageBox ( _T("시작 발행년도를 입력하세요."));
			return;
		}
		else if ( sPUB_YEAR_START != _T("") && sPUB_YEAR_END == _T("") ) {
			strWhere = _T("I.PUB_YEAR = '") + sPUB_YEAR_START  + _T("' AND ") + strWhere;
		}
		else if ( sPUB_YEAR_START != _T("") && sPUB_YEAR_END != _T("") ) {
			strWhere = _T("I.PUB_YEAR >= '") + sPUB_YEAR_START + _T("' AND I.PUB_YEAR <= '") + sPUB_YEAR_END + _T("' AND ") + strWhere;
		}

		//분류기호
		CString sCssDiv , sCssValueStart, sCssValueEnd;
		m_pCM->GetControlMgrData( _T("분류표구분"), sCssDiv);
		m_pCM->GetControlMgrData( _T("분류기호부터"), sCssValueStart);
		m_pCM->GetControlMgrData( _T("분류기호까지"), sCssValueEnd);
		
		if ( sCssDiv == _T("1") ) {
			if ( sCssValueStart == _T("") && sCssValueEnd != _T("") ) {
				AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
				return;
			}
			if ( sCssValueStart != _T("") && sCssValueEnd == _T("") ) {
				strWhere = strWhere + _T(" AND I.KDC_CLASS = '") + sCssValueStart  + _T("'");
			}
			else if ( sCssValueStart != _T("") && sCssValueEnd != _T("") ) {
				strWhere = strWhere + _T(" AND I.KDC_CLASS >= '") + sCssValueStart  + _T("' AND I.KDC_CLASS <= '") + sCssValueEnd + _T("'");
			}			
		}
		else if ( sCssDiv == _T("2") ) {
			if ( sCssValueStart == _T("") && sCssValueEnd != _T("") ) {
				AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
				return;
			}
			if ( sCssValueStart != _T("") && sCssValueEnd == _T("") ) {
				strWhere = strWhere + _T(" AND I.DDC_CLASS = '") + sCssValueStart  + _T("'");
			}
			else if ( sCssValueStart != _T("") && sCssValueEnd != _T("") ) {
				strWhere = strWhere + _T(" AND I.DDC_CLASS >= '") + sCssValueStart  + _T("' AND I.DDC_CLASS <= '") + sCssValueEnd + _T("'");
			}	
		}

		INT cnt = whereList.GetCount();
		if( !fp && cnt )
		{
			if( 1 == cnt )
				sBuf.Format( _T(" I.REC_KEY IN %s  "), whereList.GetAt( whereList.FindIndex( 0 ) ) );
			else
			{
				sBuf = _T(" I.REC_KEY IN ( ") +  whereList.GetAt( whereList.FindIndex( 0 ) );
				for( INT i = 1; i < cnt; i++ )
					sBuf = sBuf + _T("  INTERSECT ") +  whereList.GetAt( whereList.FindIndex( i ) );						
				sBuf += _T(" ) ");
			}

			CString strTempAnd = strWhere.Right( 4 );
			if( strTempAnd.Replace(_T("AND"), _T("")) < 1 )
				strWhere += _T(" AND ");

			strWhere += sBuf;
		}

	}//자세히 검색 끝
	// 2008.06.06 UPDATE BY PDJ : NULL pointer Close error 처리
	if ( fp != NULL ) fclose(fp);
	
	CString sDateSelect;
	CString sDateStart, sDateEnd;
	m_pCM->GetControlMgrData(_T("날짜선택"), sDateSelect );
	m_pCM->GetControlMgrData(_T("날짜부터"), sDateStart );
	m_pCM->GetControlMgrData(_T("날짜까지"), sDateEnd );
	if ( sDateSelect == _T("입력일자") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.INPUT_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.INPUT_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("입력일자 시작을 입력하세요.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.INPUT_DATE >= '") + sDateStart + _T("' AND B.INPUT_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.INPUT_DATE >= '") + sDateStart + _T("' AND B.INPUT_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}
	else if ( sDateSelect == _T("등록일자") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.REG_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.REG_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("등록일자 시작을 입력하세요.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.REG_DATE >= '") + sDateStart + _T("' AND B.REG_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.REG_DATE >= '") + sDateStart + _T("' AND B.REG_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}
	else if ( sDateSelect == _T("배가일자") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.SHELF_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.SHELF_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("배가일자 시작을 입력하세요.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.SHELF_DATE >= '") + sDateStart + _T("' AND B.SHELF_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.SHELF_DATE >= '") + sDateStart + _T("' AND B.SHELF_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}
	else if ( sDateSelect == _T("인계일자") ) {
		if ( sDateStart != _T("") && sDateEnd == _T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.LOC_TRANSFER_DATE = '") + sDateStart + _T("'");
			}
			else {
				sBookWhere = _T("B.LOC_TRANSFER_DATE = '") + sDateStart + _T("'");
			}
		}
		else if ( sDateStart == _T("") && sDateEnd != _T("") ) {
			AfxMessageBox( _T("인계일자 시작을 입력하세요.") );
			return;
		}
		else if ( sDateStart != _T("") && sDateEnd !=_T("") ) {
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T(" AND B.LOC_TRANSFER_DATE >= '") + sDateStart + _T("' AND B.LOC_TRANSFER_DATE <= '") + sDateEnd + _T("'");
			}
			else {
				sBookWhere = _T("B.LOC_TRANSFER_DATE >= '") + sDateStart + _T("' AND B.LOC_TRANSFER_DATE <= '") + sDateEnd + _T("'");
			}
		}
	}

	// 2006-02-16 UPDATE BY PDW(Jr) ===============================================================
	// 종검색에서 책검색으로
	// 종키  -> 원부키로 변경
	if ( strAccessionQuery != _T("") ) {
		if( !sBookWhere.IsEmpty() ) sBookWhere += _T(" AND ");
//		sBookWhere += _T(" B.SPECIES_KEY IN ( SELECT BB.SPECIES_KEY FROM SE_BOOK_TBL BB WHERE ") + strAccessionQuery + _T(" ) AND ") + strWorkingStatusQuery;
		sBookWhere += _T(" B.ACCESSION_REC_KEY IN ( SELECT BB.ACCESSION_REC_KEY FROM SE_BOOK_TBL BB WHERE ") + strAccessionQuery + _T(" ) AND ") + strWorkingStatusQuery;
	}
	// ============================================================================================

	if (!strSeqNoQuery.IsEmpty())
	{
		if (!sBookWhere.IsEmpty())
			sBookWhere += _T(" AND ");
		sBookWhere += _T(" B.SPECIES_KEY IN ( SELECT BB.SPECIES_KEY FROM SE_BOOK_TBL BB WHERE ") + strSeqNoQuery + _T(" ) ");
	}

	if ( sBookWhere != _T("") ) 
	{
		if( !strWhere.IsEmpty() )
			strWhere += _T(" AND ");
		strWhere += sBookWhere;
	}
	
	
	DWORD START, FINISH;
	START = GetTickCount();
	
	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - 연속, 배가일자기록 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	m_pDM_Result->QueryLog( log.GetBuffer( 0 ) );
	
	

	INT nRowCount = m_pDM_Result->GetRowCount();
	if ( nRowCount < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	// 2006-02-16 MOVE BY PDW(Jr) =======================================================
	// m_pDM_Result을 변경시키는 함수가 아래 존재한다. 아래쪽으로 이동 시킴
	/*
	CString strMsg;

	strMsg.Format(_T("책수 : %5d"), nRowCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));
	*/
	// ==================================================================================
	INT ids;

	//시작 등록번호 소트
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("자료실구분") , _T("자료실") );
	CString sCode;
	for( INT i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ )
	{
		// 자료상태
		ids = m_pDM_Result->GetCellData( _T("자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbSEARCHCAT") );
		
		// 이전 자료상태
		ids = m_pDM_Result->GetCellData( _T("이전자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("이전자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("OnbSEARCHCAT") );
		
	}
	
	CLocCommonAPI::MakeShelfCode( this , _T("DM_SE_LOC_1100"));
	
	// (A_2004-1014_HAN...)
	// 검색 조건이 들어오지 않았다고 보고, 검사하지 않는다.
	if ( !strAccessionNoStart.IsEmpty( ) )
		DeleteWrongAccessionNoRow( m_pDM_Result, _ttoi( strAccessionNoStart ), _ttoi( strAccessionNoEnd ) );

	// 2006-02-16 MOVE BY PDW(Jr) =======================================================
	CString strMsg;

	strMsg.Format(_T("책수 : %5d"), m_pDM_Result->GetRowCount());

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));
	// ==================================================================================

	m_pCM_Parent->AllControlDisplay();	
		
	if( m_pDM_Result->GetRowCount() > 0 )
		ShowWindow(SW_HIDE);
	

EFS_END
}


VOID CSE_LOC_1110::OnbFileSearch() 
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_검색_등록번호"));
	if (pDM == NULL) return;

	// San File Load
	INT ids = SanFileAPI::RegNoFileLoad(pDM, this);
	if (ids < 0) return;
	
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount <= 0) return;
/*
	CString strRegNo;
	CString strBookWhere = _T("b.REG_NO IN ( ");

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strRegNo = pDM->GetCellData(_T("등록번호"), idx);
		if (strRegNo.IsEmpty()) continue;

		strBookWhere += _T("'") + strRegNo + _T("'");

		if (idx != nRowCount - 1)
			strBookWhere += _T(" , ");
	}

	strBookWhere += _T(")");


	CString strWhere;
	strWhere.Format(_T("%s"), strBookWhere);

	DWORD START, FINISH;
	START = GetTickCount();
	
	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;
	

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - 연속, 배가일자기록 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	m_pDM_Result->QueryLog( log.GetBuffer( 0 ) );	
*/
	
	/// 50개씩 잘라내서 검색한다.
	CESL_DataMgr TmpDM;
	
	CLocCommonAPI::MakeCopyDM( this , m_pDM_Result , &TmpDM );
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRegNoList;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		// SELECT할 등록번호 참조
		CString sRegNo;
		ids = pDM->GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );
		
		// RECKEY리스트 생성
		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRegNoList += _T(",");
		sRegNoList += _T("'") +sRegNo + _T("'");
		
		// 레코드가 100개가 되었거나 마지막일 경우 SQL문을 날려줌
		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nRowCount-1)) ) 
		{
			CString sWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
			//=====================================================
			//2008.11.13 ADD BY PJW : 파일검색할경우도 상태 콤보박스에 따라서 상태값이 들어가야한다.
			sWhere = GetWorkingStatusSQL( );
			//=====================================================
			ids = CLocCommonAPI::RefreshDataManager( this , &TmpDM , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -5 , _T("CBL_BOSHELF::OnbFileSearch()") );
			
			ids = CLocCommonAPI::AddDM( &TmpDM , m_pDM_Result , this );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -6 , _T("CBL_BOSHELF::OnbFileSearch()") );
			
			sRegNoList.Empty();
		}
	}
	
	
	
	nRowCount =  m_pDM_Result->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	//시작 등록번호 소트
	// 코드 -> 설명
	// 자료실
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("자료실구분") , _T("자료실") );
	
	CString sCode;
	for( i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ )
	{
		// 자료상태
		ids = m_pDM_Result->GetCellData( _T("자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbSEARCHCAT") );
		
		// 이전 자료상태
		ids = m_pDM_Result->GetCellData( _T("이전자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("이전자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("OnbSEARCHCAT") );
		
	}
	
	// 청구기호 생성
	CLocCommonAPI::MakeShelfCode( this , _T("DM_SE_LOC_1100") );
	
	m_pCM_Parent->AllControlDisplay();	

	EFS_END

}

VOID CSE_LOC_1110::OnbDetail() 
{
	EFS_BEGIN

	m_bDetail = !m_bDetail;
	
	CRect rcClient;
	GetWindowRect(rcClient);

	CString strWindowText;
	if (m_bDetail)
	{
		rcClient.bottom += 200;
		strWindowText = _T("간략히 <<");
	}
	else
	{
		rcClient.bottom -= 200;
		strWindowText = _T("자세히 <<");
	}

	GetDlgItem(IDC_bDETAIL)->SetWindowText(strWindowText);;
	MoveWindow(rcClient);

	EFS_END
}

BOOL CSE_LOC_1110::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) 
		{
			OnbSEARCHCAT();
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CSE_LOC_1110::OnbCLEARSCREEN() 
{
	EFS_BEGIN

	m_pCM->AllControlClear();	

	EFS_END
}


VOID CSE_LOC_1110::DeleteWrongAccessionNoRow( CESL_DataMgr *pDM, INT from, INT to )
{
	EFS_BEGIN

	INT accessionNo;
	INT cnt = pDM->GetRowCount();

	if( (to < from) && (to == 0) )
		to = from;

	CString sAppendix;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		/// 부록정보는 항상 보여주기
		pDM->GetCellData( _T("부록정보") , i , sAppendix );

		// 2006-01-18 UPDATE BY PDW(Jr) ===========================================
		// Decode로 설명으로 변경함 
		/*
		if( sAppendix.Compare(_T("A")) == 0 ) continue;
		*/
		if( sAppendix.Compare(_T("부록")) == 0 ) continue;
		// ========================================================================

		accessionNo = _ttoi( pDM->GetCellData( _T("원부번호"), i ) );

		if( accessionNo > to || accessionNo < from )
			pDM->DeleteRow( i );
	}
	EFS_END
}

HBRUSH CSE_LOC_1110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}