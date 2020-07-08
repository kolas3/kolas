// BO_CAT_SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_SearchDlg.h"

#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\SanFileAPI.h"
#include "MainDefaultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SearchDlg dialog


CSE_CAT_SearchDlg::CSE_CAT_SearchDlg(CESL_Mgr* pParent /*=NULL*/, CString strMenuAlias)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_SearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Result = NULL;
	m_pCM_Parent = NULL;

	m_bDetail = TRUE;

	m_strMenuAlias = strMenuAlias;
	m_MakeSearchData = new CMakeSearchData(this);
}

CSE_CAT_SearchDlg::~CSE_CAT_SearchDlg()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}

BOOL CSE_CAT_SearchDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CSE_CAT_SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_SearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_CAT_SearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_SearchDlg)
	ON_BN_CLICKED(IDC_bSEARCH_CAT, OnbSEARCHCAT)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDetail)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bFileSearch, OnbFileSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString CSE_CAT_SearchDlg::GetWorkingStatusSQL()
{
	EFS_BEGIN

	CString strResult;

	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	CString strHoldWorkingStatus = _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL217O', ")
// 								   _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL317O'");
	CString strHoldWorkingStatus = _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL217O', ")
									// 18.09.27 JMJ KOL.RED.2018.008
								   _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL317O', 'SEL411O', 'SEL511O', 'SEL611O' ");
	//=====================================================
	//===================================================
	//2009.05.18 UPDATE BY PJW : 전체로 검색시 자료상태 조건 추가
	CString strWholeWorkingStatus = _T("'SEC111N', 'SEC112O', 'SEC211O', 'SEL211O', ")
								    _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL217O', ")
								    _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', ")
									// 18.09.27 JMJ KOL.RED.2018.008
									_T("'SEL317O', 'SEL411O', 'SEL511O', 'SEL611O' ");
	//===================================================

	//===================================================
	//2009.05.25 UPDATE BY PJW : 메뉴 아이디와 alias모두 체크한다.
//	if (m_strMenuAlias == _T("2510") ) // _T("연속_정리_목록완성"))
	if (m_strMenuAlias == _T("2510") || m_strMenuAlias == _T("연속_정리_목록완성"))
	//===================================================	
	{
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);
		
		//===================================================
		//2009.05.25 UPDATE BY PJW : 전체조건 추가
//		if (strWorkingStatus == _T("소장자료"))
//		{
//			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
//			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
//		}
//		else if(strWorkingStatus == _T("정리중자료"))
//		{
//			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");	//050119_소정수정 열람인계자료 상태 추가
//		}
		if (strWorkingStatus == _T("소장자료"))
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
		}
		else if(strWorkingStatus == _T("정리중자료"))
		{
			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");	//050119_소정수정 열람인계자료 상태 추가
		}
		else
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strWholeWorkingStatus);
		}
		//===================================================
		
	}
	else if (m_strMenuAlias == _T("2520") )   //_T("연속_정리_목록편성"))
	{
		//===================================================
		//2009.05.25 UPDATE BY PJW : 전체조건 추가
//		CString strWorkingStatus;
//		m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);
//					
//		if (strWorkingStatus == _T("소장자료"))
//			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
//		else
//		{
//			// 2005.10.11 AD BY PDJ 
//			// 열람인계자료 상태추가
//			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O','SEC211O', 'SEL211O')"); 
//		}
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);
		if (strWorkingStatus == _T("소장자료"))
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
		}
		else if(strWorkingStatus == _T("정리중자료"))
		{
			strResult = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");	//050119_소정수정 열람인계자료 상태 추가
		}
		else
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strWholeWorkingStatus);
		}
		//===================================================			
	}
	else if (m_strMenuAlias == _T("연속_정리_열람인계처리"))
	{
		strResult = _T("b.WORKING_STATUS = 'SEC112O'");
	}
	else if (m_strMenuAlias == _T("2620") ) //_T("배가일기록_연속"))
	{
		strResult = _T("b.WORKING_STATUS = 'SEL211O'");
	}
	else
		strResult = _T("b.WORKING_STATUS > 'SEC111N'");

	if (strResult.GetLength() > 0) strResult += _T(" AND ");
	strResult += _T("(b.BINDING_BOOK_KEY IS NULL)");

	return strResult;

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SearchDlg message handlers

BOOL CSE_CAT_SearchDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CComboBox *pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_cobWorkingStatus));
	if (!pComboBox) goto ERR;
	
	if (InitESL_Mgr(_T("SM_연속_정리_검색")) < 0)
		goto ERR;

	m_pDM_Result = FindDM(_T("DM_연속_정리_검색결과"));
	m_pCM_Parent = FindCM(_T("CM_연속_정리"));
	m_pCM = FindCM(_T("CM_연속_정리_검색"));

	if (m_pDM_Result == NULL ||
		m_pCM_Parent == NULL ||
		m_pCM == NULL) goto ERR;

	if (m_strMenuAlias == _T("2510"))
	{
	}
	else if (m_strMenuAlias == _T("2520"))
	{
	}
	else
	{
		CString strMenuAlias = m_strMenuAlias;
		if (m_strMenuAlias == _T("배가일기록_연속"))
			strMenuAlias = _T("연속_열람인계");
		
		INT nFind = strMenuAlias.ReverseFind('_');
		if (nFind >= 0)
		{
			strMenuAlias = strMenuAlias.Mid(nFind + 1);
			strMenuAlias += _T("자료");
		}
	
		pComboBox->ResetContent();
		pComboBox->InsertString(0, strMenuAlias);
		pComboBox->SetCurSel(0);
		pComboBox->EnableWindow(FALSE);
	}

	OnbDetail();

	//======================================
	//2009.05.25 ADD BY PJW : 디폴트를 전체로둔다.
	pComboBox->SelectString(0,_T("전체"));
	//======================================
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	MessageBox(_T("다이얼로그 초기화 실패"));
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CSE_CAT_SearchDlg::OnbSEARCHCAT() 
{
	EFS_BEGIN

	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sWORKING_STATUS;
	
	CString sControlNo, sMAT_CODE , sMEDIA_CODE;
	CString sTitle, sKeyWord, sAuthor, sPublisher;
	
	m_pCM->GetControlMgrData(_T("BB_수입구분_코드"), sACQUISIT_CODE);
	m_pCM->GetControlMgrData(_T("BS_수입년도"), sACQUISIT_YEAR);
	m_pCM->GetControlMgrData(_T("BB_등록번호_시작"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_등록번호_까지"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
// 		if ( sRegNoStart.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 			sRegNoStart = sRegNoStart.Left(2) + sRight;
// 		}
// 	}
// 	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
// 		if ( sRegNoEnd.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 		}
// 	}

	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
		}
	}
	//=====================================================
	

	m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), sWORKING_STATUS);
	m_pCM->GetControlMgrData(_T("BS_자료구분_코드"), sMAT_CODE);
	m_pCM->GetControlMgrData(_T("BS_매체구분_코드"), sMEDIA_CODE);
	m_pCM->GetControlMgrData(_T("IBS_본표제"), sTitle);
	m_pCM->GetControlMgrData(_T("IBS_키워드"), sKeyWord);
	m_pCM->GetControlMgrData(_T("IBS_저작자"), sAuthor);
	m_pCM->GetControlMgrData(_T("IBS_발행자"), sPublisher);
	
	CString sWhere = _T("I.REC_KEY = S.REC_KEY");
	
	//상세검색 버튼 체크
	
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
		strAccessionQuery.Format(_T("b.accession_rec_key = a.rec_key and a.accession_rec_no = %s"), strAccessionNoStart);
	}
	else if (!strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("b.accession_rec_key = a.rec_key and a.accession_rec_no >= %s and a.accession_rec_no <= %s"), strAccessionNoStart, strAccessionNoEnd);
	}

	// 2005.11.29 ADD BY PDJ
	// 원부번호가 없을 경우 원부작성년도를 무시.
	//if (!strAccestionNoMakeYear.IsEmpty())
	if (!strAccestionNoMakeYear.IsEmpty() && !( strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty() ) )
	{
		if (!strAccessionQuery.IsEmpty())
		{
			strAccessionQuery += _T(" and ");
		}
		else
		{
			strAccessionQuery = _T("b.accession_rec_key = a.rec_key and ");
		}

		strAccessionQuery += _T("a.ACCESSION_REC_MAKE_YEAR = '") + strAccestionNoMakeYear + _T("'");
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
		strSeqNoQuery.Format(_T("b.RECEIPT_NO = %s"), strSeqNoStart);
	}
	else if (!strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("b.RECEIPT_NO >= %s and b.RECEIPT_NO <= %s"), strSeqNoStart, strSeqNoEnd);
	}

	//자료상태
	CString strWorkingStatusQuery = GetWorkingStatusSQL();
	CString sBookWhere = strWorkingStatusQuery;


	//수입구분, 수입년도
	sACQUISIT_CODE.Replace ( _T(" ") , NULL );
	if ( sACQUISIT_CODE != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND b.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}
	// 2005.11.28 ADD BY PDJ
	// 수입년도는 접수번호가 조건으로 들어와야만 인정
	if ( sACQUISIT_YEAR != _T("") && !(strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty() )) {
		sBookWhere = sBookWhere + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//등록번호검색
	CString strWhere = sWhere;

	if (m_bDetail) {
		if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {			
			if ( sBookWhere != _T("") ) {
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
		//키워드 검색
		if ( sKeyWord != _T("") ){
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_KEYWORD, '%s*')"), sKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
						
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//서명검색	
		//sTitle
		if ( sTitle != _T("") ){
			INT iCh = m_MakeSearchData->ChangeHanjaToHangul( sTitle );	//한자 한글 변환
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			iCh = m_MakeSearchData->GetKeyWord( sTitle );				//공백, 특수문자 제거
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
			
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//저작자
		if ( sAuthor != _T("") ){
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_AUTHOR, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
			
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		//발행자
		if ( sPublisher != _T("") ){
			CString sWhereTitle;
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_PUBLISHER, '%s*')"), sPublisher );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
			
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
	}//상세검색 검색 끝

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

	if ( strAccessionQuery != _T("") ) {
		//sBookWhere = strAccessionQuery + _T(" AND ") + sBookWhere + _T(" AND A.REC_KEY = B.ACCESSION_REC_KEY");
		sBookWhere = strAccessionQuery + _T(" AND ") + strWorkingStatusQuery + _T(" AND A.REC_KEY = B.ACCESSION_REC_KEY");
	}

	if (!strSeqNoQuery.IsEmpty())
	{
		if (!sBookWhere.IsEmpty())
			sBookWhere += _T(" AND ");
		sBookWhere += _T(" ") + strSeqNoQuery;
	}

	CString strFrom;
	if ( sBookWhere != _T("") ) 
	{
		strFrom = _T(" se_book_tbl b ");

		if (!strAccessionQuery.IsEmpty())
		{
			strFrom += _T(", co_accession_rec_tbl a ");
		}

		strWhere = strWhere + _T(" AND S.REC_KEY IN (select distinct b.species_key from ") + strFrom + _T(" where ") + sBookWhere + _T(")");
	}

	// 책수, 시작등록번호를 위한 쿼리 만들자
	CString strSpecialQuery_BookCount;
	strSpecialQuery_BookCount.Format(_T("(SELECT COUNT(b.REC_KEY) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY)"), strFrom, sBookWhere);

	CString strSpecialQuery_MinRegNo;

	//2018-11-13 KYJ
	//JOB.2018.0017 부록검색시에도 모체자료의 시작등록번호 조회하도록 쿼리 수정
	//strSpecialQuery_MinRegNo.Format(_T("(SELECT MIN(b.REG_NO) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY)"), strFrom, sBookWhere);
	//2019.07.23 JOB.2019.0023 부록이 아닌자료는 검색시 입력한 등록번호로 조회되고 비어있을경우 시작등록번호를 가져오도록 수정
	//strSpecialQuery_MinRegNo.Format(_T("(SELECT MIN(b.REG_NO) FROM SE_BOOK_TBL B WHERE B.SPECIES_KEY = S.REC_KEY AND B.BOOK_TYPE != 'A' AND B.REG_NO IS NOT NULL) ") );
	strSpecialQuery_MinRegNo.Format(
		_T("NVL((SELECT MIN(b.REG_NO) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY),(SELECT MIN(b.REG_NO) FROM SE_BOOK_TBL B WHERE B.SPECIES_KEY = S.REC_KEY AND B.BOOK_TYPE != 'A' AND B.REG_NO IS NOT NULL) ) ")
		, strFrom, sBookWhere
		);


	// DM의 레퍼런스를 바꾸어주자
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM_Result->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM_Result->RefList.GetNext(pos);
		if (!pRef) continue;

		// 책수
		if (pRef->FIELD_ALIAS == _T("책수"))
		{
			pRef->FIELD_NAME = strSpecialQuery_BookCount;
		}
		else if (pRef->FIELD_ALIAS == _T("시작등록번호"))
		{
			pRef->FIELD_NAME = strSpecialQuery_MinRegNo;
		}
	}

	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;

	INT nRowCount = m_pDM_Result->GetRowCount();
	if ( nRowCount < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}


	CString strMsg;

	INT nBookCount = 0;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM_Result->GetCellData(_T("책수"), idx));
	}

	strMsg.Format(_T("종수 : %5d\t책수 : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	{
		if (pDM_Where->GetRowCount() <= 0) pDM_Where->InsertRow(-1);

		// 전체조건절 저장
		pDM_Where->SetCellData(_T("조건절"), strWhere, 0);

		// 책조건절 저장
		pDM_Where->SetCellData(_T("책조건절"), sBookWhere, 0);
	}

	//시작 등록번호 소트
	m_pDM_Result->SORT(_T("시작등록번호"));

	m_pCM_Parent->AllControlDisplay();	

	if( nRowCount > 0 )
		ShowWindow(SW_HIDE);
	EFS_END
}

VOID CSE_CAT_SearchDlg::OnbDetail() 
{
	EFS_BEGIN

	m_bDetail = !m_bDetail;
	
	CRect rcClient;
	GetWindowRect(rcClient);

	CString strWindowText;
	if (m_bDetail)
	{
		rcClient.bottom += 170;
		strWindowText = _T("간략검색");
	}
	else
	{
		rcClient.bottom -= 170;
		strWindowText = _T("상세검색");
	}

	GetDlgItem(IDC_bDETAIL)->SetWindowText(strWindowText);;
	MoveWindow(rcClient);

	EFS_END
}

BOOL CSE_CAT_SearchDlg::PreTranslateMessage(MSG* pMsg) 
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

VOID CSE_CAT_SearchDlg::OnbCLEARSCREEN() 
{
	EFS_BEGIN

	m_pCM->AllControlClear();		

	EFS_END
}

VOID CSE_CAT_SearchDlg::OnbFileSearch() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_검색_등록번호"));
	if (pDM == NULL) return;

	// San File Load
	INT ids = SanFileAPI::RegNoFileLoad(pDM, this);
	if (ids < 0) return;
	
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount <= 0) return;

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
	strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN (SELECT SPECIES_KEY FROM SE_BOOK_TBL b WHERE %s)"), strBookWhere);

	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return;
	nRowCount =  m_pDM_Result->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	// 메시지 표시
	CString strMsg;

	INT nBookCount = 0;
	for (idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM_Result->GetCellData(_T("책수"), idx));
	}
	
	strMsg.Format(_T("종수 : %5d\t책수 : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	{
		if (pDM_Where->GetRowCount() <= 0) pDM_Where->InsertRow(-1);

		// 전체조건절 저장
		pDM_Where->SetCellData(_T("조건절"), strWhere, 0);

		// 책조건절 저장
		pDM_Where->SetCellData(_T("책조건절"), strBookWhere, 0);
	}

	//시작 등록번호 소트
	m_pDM_Result->SORT( _T("시작등록번호"), TRUE);

	m_pCM_Parent->AllControlDisplay();	
	
	if( nRowCount > 0 )
		DestroyWindow();
}

HBRUSH CSE_CAT_SearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
