// BO_CAT_SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SearchDlg.h"

#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\SanFileAPI.h"
#include "MainDefaultDlg.h"

#include "CommonDefine.h"

#include "..\..\..\공통\AC_SEARCH\AC_SEARCH_DLG.h"

#include "sbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SearchDlg dialog


CBO_CAT_SearchDlg::CBO_CAT_SearchDlg(CESL_Mgr* pParent /*=NULL*/, CString strMenuAlias)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_SearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Result = NULL;
	m_pCM_Parent = NULL;

	m_bDetail = TRUE;

	m_strMenuAlias = strMenuAlias;
	m_MakeSearchData = new CMakeSearchData(this);
    m_Bo_ProgressDlg = NULL ;
	m_nAcSearchOpt = -1;

}
CBO_CAT_SearchDlg::~CBO_CAT_SearchDlg()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}

BOOL CBO_CAT_SearchDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CBO_CAT_SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_SearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SearchDlg)
	ON_BN_CLICKED(IDC_bSEARCH_CAT, OnbSEARCHCAT)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDetail)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bFileSearch, OnbFileSearch)
	ON_BN_CLICKED(IDC_bAC_TITLE, OnbACTITLE)
	ON_BN_CLICKED(IDC_bAC_AUTHOR, OnbACAUTHOR)
	ON_BN_CLICKED(IDC_btnSEARCH_TYPE, OnbtnSEARCHTYPE)
	ON_EN_SETFOCUS(IDC_eACCESSION_NO_START, OnSetfocuseACCESSIONNOSTART)
	ON_EN_SETFOCUS(IDC_eTITLE_ARTICLE, OnSetfocuseTITLEARTICLE)
	ON_EN_SETFOCUS(IDC_eKEYWORD, OnSetfocuseKEYWORD)
	ON_EN_SETFOCUS(IDC_eAUTHOR_BAP, OnSetfocuseAUTHORBAP)
	ON_EN_SETFOCUS(IDC_ePUBLISHER_BCP, OnSetfocusePUBLISHERBCP)
	ON_EN_SETFOCUS(IDC_eControlNoStart, OnSetfocuseControlNoStart)
	ON_EN_SETFOCUS(IDC_eControlNoEnd, OnSetfocuseControlNoEnd)
	ON_EN_SETFOCUS(IDC_eSUBJECT, OnSetfocuseSUBJECT)
	ON_EN_SETFOCUS(IDC_ePUBLISH_COUNTRY_CODE, OnSetfocusePUBLISHCOUNTRYCODE)
	ON_EN_SETFOCUS(IDC_eACCESSION_NO_END, OnSetfocuseACCESSIONNOEND)
	ON_BN_CLICKED(IDC_bSearchTest, OnbSearchTest)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cCatAcqCode, OnSelchangecCatAcqCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString CBO_CAT_SearchDlg::GetWorkingStatusSQL()
{
	EFS_BEGIN

	CString strResult;


	CString strHoldWorkingStatus = _T("'BOT212O', 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL252N', 'BOL211O', 'BOL212O', 'BOL213O', ")
								   _T("'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O', 'BOL611O'");

	if (m_strMenuAlias == _T("") )
	{
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);

		if (strWorkingStatus == _T("") )
			strResult = _T("b.WORKING_STATUS = 'BOC111O'");
		else
			strResult.Format(_T("b.WORKING_STATUS IN ('BOC112O', 'BOC113N', 'BOC114O', 'BOL111O', %s)"), strHoldWorkingStatus);
	}
	else if (m_strMenuAlias == _T("") )
	{
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);

		if (strWorkingStatus == _T("") )
			strResult = _T("b.WORKING_STATUS = 'BOC112O'");
		else
			strResult.Format(_T("b.WORKING_STATUS IN ('BOC113N', 'BOC114O', 'BOL111O', %s)"), strHoldWorkingStatus);		
	}
	else if (m_strMenuAlias == _T("1510") || m_strMenuAlias == _T("1520") )
	{
		CString strWorkingStatus;
		m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);
					
		if (strWorkingStatus == _T("소장자료"))
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s)"), strHoldWorkingStatus);
			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
		}
		else if (strWorkingStatus == _T("정리중자료"))
		{
			strResult = _T("b.WORKING_STATUS IN ('BOC113N', 'BOC114O', 'BOL111O')");	
		}
		else
		{
			strResult.Format(_T("b.WORKING_STATUS IN (%s, 'BOC113N', 'BOC114O', 'BOL111O')"), strHoldWorkingStatus);
			static_cast<CMainDefaultDlg*>(pParentMgr)->m_bCatHold = TRUE;
		}
	}
	else if (m_strMenuAlias == _T("") )
	{
		strResult = _T("b.WORKING_STATUS = 'BOC114O'");
	}
	else if (m_strMenuAlias == _T("") )
	{
		strResult = _T("b.WORKING_STATUS = 'BOL111O'");
	}
	else
		strResult = _T("b.WORKING_STATUS > 'BOA142N'");

	static_cast<CMainDefaultDlg*>(pParentMgr)->m_strWorkingStatusCodeSql = strResult;

	return strResult;

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SearchDlg message handlers

BOOL CBO_CAT_SearchDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CESL_DataMgr dm;
	CString strQuery, strReturnValue;
	CString strValue;
	CFile fDebug;
	

	m_pDM_Result = FindDM(_T("DM_단행_정리_검색결과"));

	dm.SetCONNECTION_INFO(m_pDM_Result->CONNECTION_INFO);
	
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'SEARCHBYSUBJ'");
	dm.GetOneValueQuery(strQuery, strReturnValue);

	CComboBox *pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_cobWorkingStatus));
	if (pComboBox == NULL) goto ERR;
	
	if (InitESL_Mgr(_T("SM_단행_정리_검색")) < 0)
		goto ERR;

	
	m_pCM_Parent = FindCM(_T("CM_단행_정리"));
	m_pCM = FindCM(_T("CM_단행_정리_검색"));

	if (m_pDM_Result == NULL ||
		m_pCM_Parent == NULL ||
		m_pCM == NULL) goto ERR;

	 
	if (m_strMenuAlias == _T("1510") )
	{
		
	}
	else if (m_strMenuAlias == _T("1520") )
	{

	}
	else if (m_strMenuAlias == _T("단행_정리_복본조사"))
	{
		pComboBox->ResetContent();
		pComboBox->AddString(_T("복본조사대상자료"));
		pComboBox->AddString(_T("복본조사이후자료"));
		pComboBox->SetCurSel(0);
	}
	else if (m_strMenuAlias == _T("단행_정리_분류"))
	{
		pComboBox->ResetContent();
		pComboBox->AddString(_T("분류대상자료"));
		pComboBox->AddString(_T("분류이후자료"));
		pComboBox->SetCurSel(0);
	}
	else
	{		
		CString strMenuAlias = m_strMenuAlias;
		if (m_strMenuAlias == _T("배가일기록_단행"))
			strMenuAlias = _T("단행_열람인계");
		
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
	if ( strReturnValue == _T("Y") )
	{
		GetDlgItem(IDC_eSUBJECT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_sSUBJECT)->ShowWindow(SW_SHOW);
	}
	GetManageValue( _T("기타") , _T("공통") , _T("기표원") , _T("기표원") , strValue );
	
	if ( strValue == _T("Y") ) 
	{
		GetDlgItem(IDC_sSUBJECTNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_eSUBJECTNO)->ShowWindow(SW_SHOW);
	}

	OnbDetail();

	pComboBox->SelectString(0,_T("전체"));
	
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_단행_정리_검색"), _T("BB_등록번호_시작"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_단행_정리_검색"), _T("BB_등록번호_까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

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

VOID CBO_CAT_SearchDlg::OnbSEARCHCAT() 
{
	EFS_BEGIN


	CString sDateSearch, sAccNoSearch, sRegNoSearch, sTitleSearch, sAuthorSearch, sSeqNo;
	CString sClassNoSearch, sKeyWordSearch, sPublisherSearch, sControlNoSearch, sSubjectSearch, sPubConCode, strSTANDARDNO;
	GetControlData( _T("CM_단행_정리_검색"), _T("날짜부터"), sDateSearch );
	GetDlgItem(IDC_eACCESSION_REC_NO)->GetWindowText(sAccNoSearch);
	GetDlgItem(IDC_eACCESSION_NO_START)->GetWindowText(sRegNoSearch);
	GetDlgItem(IDC_eTITLE_ARTICLE)->GetWindowText(sTitleSearch);
	GetDlgItem(IDC_eAUTHOR_BAP)->GetWindowText(sAuthorSearch);
	GetDlgItem(IDC_edtCssValue)->GetWindowText(sClassNoSearch);
	GetDlgItem(IDC_eKEYWORD)->GetWindowText(sKeyWordSearch);
	GetDlgItem(IDC_ePUBLISHER_BCP)->GetWindowText(sPublisherSearch);
	GetDlgItem(IDC_eControlNoStart)->GetWindowText(sControlNoSearch);
	GetDlgItem(IDC_eSEQ_NO_START)->GetWindowText(sSeqNo);
	GetDlgItem(IDC_ePUBLISH_COUNTRY_CODE)->GetWindowText(sPubConCode);
	GetDlgItem(IDC_eSUBJECTNO)->GetWindowText(strSTANDARDNO);

	GetDlgItem(IDC_eSUBJECT)->GetWindowText(sSubjectSearch);
	if ( sDateSearch == _T("") && sAccNoSearch == _T("") && sRegNoSearch == _T("") && sTitleSearch == _T("") ) {
		if ( sAuthorSearch == _T("") && sClassNoSearch == _T("") && sKeyWordSearch == _T("") && sPubConCode == _T("") ) {
			if ( sPublisherSearch == _T("") && sControlNoSearch == _T("") && sSeqNo ==_T("") && sSubjectSearch == _T("") && strSTANDARDNO == _T("") ) {
				AfxMessageBox( _T("검색 조건이 부족합니다.") );
				return;
			}
		}
	}

	
	CString sACQUISIT_CODE;
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sWORKING_STATUS;
	CString sPUBLISH_COUNTRY_CODE;
	
	CString sControlNo, sMAT_CODE , sMEDIA_CODE;
	CString sTitle, sKeyWord, sAuthor, sPublisher, sSubject;
	
	m_pCM->GetControlMgrData(_T("BB_수입구분_코드"), sACQUISIT_CODE);
	m_pCM->GetControlMgrData(_T("BB_등록번호_시작"), sRegNoStart);
	m_pCM->GetControlMgrData(_T("BB_등록번호_까지"), sRegNoEnd);

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), sWORKING_STATUS);
	m_pCM->GetControlMgrData(_T("BS_자료구분_코드"), sMAT_CODE);
	m_pCM->GetControlMgrData(_T("BS_매체구분_코드"), sMEDIA_CODE);
	m_pCM->GetControlMgrData(_T("IBS_본표제"), sTitle);
	m_pCM->GetControlMgrData(_T("IBS_키워드"), sKeyWord);
	m_pCM->GetControlMgrData(_T("IBS_저작자"), sAuthor);
	m_pCM->GetControlMgrData(_T("IBS_발행자"), sPublisher);

	INT nIds = 0;
	nIds = m_MakeSearchData->ChangeHanjaToHangul( sTitle );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->ChangeHanjaToHangul( sTitle ) error") );
		return;
	}
	nIds = m_MakeSearchData->UnicodeSpecialConvert(sTitle);
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sTitle ) error") );
		return;
	}
	nIds = m_MakeSearchData->GetKeyWord( sTitle );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sTitle ) error") );
		return;
	}
	nIds = m_MakeSearchData->ChangeHanjaToHangul( sAuthor );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->ChangeHanjaToHangul( sAuthor ) error") );
		return;
	}
	nIds = m_MakeSearchData->UnicodeSpecialConvert(sAuthor);
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sAuthor ) error") );
		return;
	}
	nIds = m_MakeSearchData->GetKeyWord( sAuthor );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sAuthor ) error") );
		return;
	}
	nIds = m_MakeSearchData->ChangeHanjaToHangul( sPublisher );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->ChangeHanjaToHangul( sPublisher ) error") );
		return;
	}
	nIds = m_MakeSearchData->UnicodeSpecialConvert(sPublisher);
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sPublisher ) error") );
		return;
	}
	nIds = m_MakeSearchData->GetKeyWord( sPublisher );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sPublisher ) error") );
		return;
	}
	nIds = m_MakeSearchData->ChangeHanjaToHangul( sKeyWord );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->ChangeHanjaToHangul( sKeyWord ) error") );
		return;
	}
	nIds = m_MakeSearchData->UnicodeSpecialConvert(sKeyWord);
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sKeyWord ) error") );
		return;
	}
	nIds = m_MakeSearchData->GetKeyWord( sKeyWord );
	if ( 0 != nIds ) {			
		AfxMessageBox ( _T("m_MakeSearchData->GetKeyWord( sKeyWord ) error") );
		return;
	}
	m_pCM->GetControlMgrData(_T("IBS_주제명"), sSubject );
	CString sWhere = _T("I.REC_KEY = S.REC_KEY");
	
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
	
	if (m_bDetail) {
		sMAT_CODE.Replace ( _T(" ") , NULL );
		if ( sMAT_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sMEDIA_CODE.Replace ( _T(" ") , NULL );
		if ( sMEDIA_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}
	}	

	CString strAccessionQuery;
	CString strAccestionNoMakeYear;
	CString strAccessionNoStart;
	CString strAccessionNoEnd;   

	m_pCM->GetControlMgrData(_T("원부작성년도"), strAccestionNoMakeYear);
	m_pCM->GetControlMgrData(_T("원부번호부터"), strAccessionNoStart);
	m_pCM->GetControlMgrData(_T("원부번호까지"), strAccessionNoEnd);

	if ( strAccessionNoStart != _T("") && strAccestionNoMakeYear == _T("") )
	{
		AfxMessageBox(_T("원부작성년도를 입력 하세요"));
		return;
	}

	if (strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("시작 원부번호를 입력 하세요"));
		return;
	}
	else if (!strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("B.ACCESSION_REC_KEY = A.REC_KEY AND A.ACCESSION_REC_NO = %s"), strAccessionNoStart);
	}
	else if (!strAccessionNoStart.IsEmpty() && !strAccessionNoEnd.IsEmpty())
	{
		strAccessionQuery.Format(_T("B.ACCESSION_REC_KEY = A.REC_KEY AND A.ACCESSION_REC_NO >= %s AND A.ACCESSION_REC_NO <= %s"), strAccessionNoStart, strAccessionNoEnd);
	}

	if (!strAccestionNoMakeYear.IsEmpty() && !( strAccessionNoStart.IsEmpty() && strAccessionNoEnd.IsEmpty() ) )
	{
		if (!strAccessionQuery.IsEmpty())
		{
			strAccessionQuery += _T(" and ");
		}
		else
		{
			strAccessionQuery = _T("B.ACCESSION_REC_KEY = A.REC_KEY AND ");
		}

		strAccessionQuery += _T("A.ACCESSION_REC_MAKE_YEAR = '") + strAccestionNoMakeYear + _T("'");
	}
	
	CString strWorkCode;
	CString strSeqNoQuery;
	CString strSeqNoMakeYear;
	CString strSeqNoStart;
	CString strSeqNoEnd;
	m_pCM->GetControlMgrData(_T("차수작성년도"), strSeqNoMakeYear);
	m_pCM->GetControlMgrData(_T("차수번호부터"), strSeqNoStart);
	m_pCM->GetControlMgrData(_T("차수번호까지"), strSeqNoEnd);
	m_pCM->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);

	if ( strSeqNoStart != _T("") && strSeqNoMakeYear==_T("") )
	{
		AfxMessageBox(_T("차수작성년도를 입력 하세요"));
		return;
	}

	if (strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("시작 차수번호를 입력하세요"));
		return;
	}
	else if (!strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("B.SEQ_NO = %s"), strSeqNoStart);
	}
	else if (!strSeqNoStart.IsEmpty() && !strSeqNoEnd.IsEmpty())
	{
		strSeqNoQuery.Format(_T("B.SEQ_NO >= %s AND B.SEQ_NO <= %s"), strSeqNoStart, strSeqNoEnd);
	}
    
	if (!strSeqNoMakeYear.IsEmpty() && !(strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty() ) )    
    {   
		if(strSeqNoMakeYear.IsEmpty()) 
		{
			AfxMessageBox(_T("차수년도를 입력하세요"));
			return;
		}
	}


	if (!strSeqNoMakeYear.IsEmpty() && !(strSeqNoStart.IsEmpty() && strSeqNoEnd.IsEmpty() ) )    
	{
		if (!strSeqNoQuery.IsEmpty())
		{
			strSeqNoQuery += _T(" AND ");
		}

		strSeqNoQuery += _T("B.ACQ_YEAR = P.ACQ_YEAR AND B.SEQ_NO = P.SEQ_NO AND TO_CHAR(P.SEQ_NO_CREATE_DATE, 'YYYY') = '") + strSeqNoMakeYear + _T("'");
	}

	if (!strSeqNoQuery.IsEmpty() && !strWorkCode.IsEmpty())
	{
		strSeqNoQuery += _T(" AND I.WORK_CODE = '") + strWorkCode + _T("'");
		strSeqNoQuery += _T(" AND P.WORK_CODE = '") + strWorkCode + _T("'");
	}
	else if (!strSeqNoQuery.IsEmpty() && strWorkCode.IsEmpty())
	{
		strSeqNoQuery += _T(" AND I.WORK_CODE IS NULL ");
		strSeqNoQuery += _T(" AND P.WORK_CODE IS NULL ");
	}


	CString strWorkingStatusQuery = GetWorkingStatusSQL();
	CString sBookWhere ;
	if(m_strMenuAlias == _T("단행_정리_열람인계처리"))
	{
		sBookWhere = strWorkingStatusQuery ;
	}
	else
	{
		sBookWhere = strWorkingStatusQuery;	
	}
	
	sACQUISIT_CODE.Replace ( _T(" ") , NULL );
	if ( sACQUISIT_CODE != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND b.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	m_pCM->GetControlMgrData(_T("발행국"), sPUBLISH_COUNTRY_CODE );
	if ( sPUBLISH_COUNTRY_CODE != _T("") ){
		sBookWhere = sBookWhere + _T(" AND I.PUBLISH_COUNTRY_CODE = '") + sPUBLISH_COUNTRY_CODE + _T("'");
	}

	CString strShelfCode;
	m_pCM->GetControlMgrData(_T("자료실"), strShelfCode);
	if ( !strShelfCode.IsEmpty() )
		sBookWhere = sBookWhere + _T(" AND TRIM(b.SHELF_LOC_CODE) = '") + strShelfCode + _T("'");

	CString strWhere = sWhere;	
	CString strText;
	
	( (CButton*) GetDlgItem (IDC_btnSEARCH_TYPE) )->GetWindowText(strText);

	if ( strText == _T("구분") ) 
	{
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
			if ( sRegNoStart.GetLength() > nRegCodeLength ) 
			{
				sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
				sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
			}
		}
		if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) 
		{
			if ( strAccessionNoStart == _T("") ) {
				if ( sBookWhere != _T("") ) {
					sBookWhere = sBookWhere + _T(" AND b.REG_NO = '") + sRegNoStart + _T("'");
				}
			}
			else {
				if ( sBookWhere != _T("") ) {
					sBookWhere = sBookWhere + _T(" AND b.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= 'ZZ9999999999'");
				}
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
	}
	else
	{
		if ( !sRegNoStart.IsEmpty() )
		{
			CString strRegNoWhere;
			CString strRegNo;	
			CString strTempRegNo;
			INT	nCommaPos;
			INT	offset;

			offset = 0;
			nCommaPos = 0;
			strRegNoWhere =_T("AND REG_NO IN (");

			while ( nCommaPos > -1 )
			{
				nCommaPos = sRegNoStart.Find(_T(","), offset );

				if ( nCommaPos == -1 && offset == 0)
				{
					sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );

					strRegNoWhere.Format(_T(" AND REG_NO='%s'"), sRegNoStart);
					break;
				}
				if ( offset != 0 ) strRegNoWhere += _T(",");
				if ( nCommaPos >= 0 )
				{
					strRegNo = sRegNoStart.Mid(offset,nCommaPos-offset);
					strRegNo.TrimLeft();
					strRegNo.TrimRight();

					strRegNo = m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );
					strRegNo = _T("'")+strRegNo+_T("'");
				}
				else
				{
					strRegNo = sRegNoStart.Right(sRegNoStart.GetLength()-offset);
					strRegNo.TrimLeft();
					strRegNo.TrimRight();

					strRegNo = m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );

					strRegNo = _T("'")+strRegNo+_T("')");
				}
				strRegNoWhere += strRegNo;
				offset = nCommaPos+1;
			}

			sBookWhere += strRegNoWhere;
		}
	}

	INT iIntersect = 0;
	if (m_bDetail) {
		if ( sKeyWord != _T("") ) {
			iIntersect++;
		}
		if ( sSubject != _T("") )
		{
			iIntersect++;
		}
		if ( sTitle != _T("") ) {
			iIntersect++;
		}
		if ( sAuthor != _T("") ) {
			iIntersect++;
		}
		if ( sPublisher != _T("") ) {
			iIntersect++;
		}

		if ( iIntersect >= 2 && m_pInfo->GetSearchIndexType() == ECO_SEARCH) {
			CString sSubQuery;
			if ( sKeyWord != _T("") ) {				
				CString sWhereTitle;			
				sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_KEYWORD, '%s*') "), sKeyWord );
				sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
			}
			if ( sSubject != _T("") ) {				
				CString sWhereTitle;
				sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_SUBJECT, '%s*') "), sSubject );
				sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
			}
			if ( sTitle != _T("") ) {
				CString sWhereTitle;	
				sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_TITLE, '%s*') "), sTitle );
				sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
			}
			if ( sAuthor != _T("") ) {
				CString sWhereTitle;				
				sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_AUTHOR, '%s*') "), sAuthor );
				sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
			}
			if ( sPublisher != _T("") ) {
				CString sWhereTitle;				
				sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_PUBLISHER, '%s*') "), sPublisher );
				sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
			}
			strWhere = strWhere + _T(" AND I.REC_KEY IN ( ") + sSubQuery.Mid(10) + _T(" )");
		}
		else {

			if ( sKeyWord != _T("") ){
				
				CString sWhereTitle;

				if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
					sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
				}
				else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
					sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_KEYWORD, '%s*')"), sKeyWord );
				}
				else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
					AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
					return;
				}
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
				
			}
			if ( sSubject != _T("") ){
				
				CString sWhereTitle;			
				if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
					sWhereTitle.Format( _T("CATSEARCH(I.IDX_SUBJECT,'%s*',NULL)>0") , sSubject );
				}
				else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
					sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_SUBJECT, '%s*')"), sSubject );
				}
				else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
					AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
					return;
				}
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
				
			}
			if ( sTitle != _T("") ){
				if ( m_nAcSearchOpt == 0 )
				{

					CString sWhereTitle;			
					if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
						sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
						sWhereTitle.Replace(_T(" "), _T("*', NULL )>0 OR CATSEARCH(I.IDX_TITLE,'"));
					}
					else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
						sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
						sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
					}
					else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
						AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
						return;
					}
					

					strWhere = strWhere + _T(" AND ") + sWhereTitle;	
				}
				else
				{
					
					CString sWhereTitle;			
					if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
						sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
					}
					else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
						sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
					}
					else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
						AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
						return;
					}
					strWhere = strWhere + _T(" AND ") + sWhereTitle;	
				}			
			}

			if ( sAuthor != _T("") )
			{
				if ( m_nAcSearchOpt == 1 )
				{
					CString sWhereTitle;										

					if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
						sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
						sWhereTitle.Replace(_T(" "), _T("*', NULL )>0 OR CATSEARCH(I.IDX_AUTHOR,'"));
					}
					else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
						sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
					}
					else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
						AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
						return;
					}
					strWhere = strWhere + _T(" AND ") + sWhereTitle;
				}
				else
				{
					CString sWhereTitle;			

					if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
						sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
					}
					else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
						sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
					}
					else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
						AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
						return;
					}
					strWhere = strWhere + _T(" AND ") + sWhereTitle;
				}
			}

			if ( sPublisher != _T("") ){				
				CString sWhereTitle;			
				if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
					sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );
				}
				else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
					sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_PUBLISHER, '%s*')"), sPublisher );
				}
				else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
					AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
					return;
				}
				strWhere = strWhere + _T(" AND ") + sWhereTitle;
			}
		}

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

		CString sCssDiv , sCssValueStart, sCssValueEnd, sCtxClassAlias, sEcoClassAlias;
		CString strClassSearch;
		m_pCM->GetControlMgrData( _T("분류표구분"), sCssDiv);
		m_pCM->GetControlMgrData( _T("분류기호부터"), sCssValueStart);
		m_pCM->GetControlMgrData( _T("분류기호까지"), sCssValueEnd);

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
		else if ( sCssDiv == _T("4") ) {
			if ( sCssValueStart == _T("") && sCssValueEnd != _T("") ) {
				AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
				return;
			}
			if ( sCssValueStart != _T("") && sCssValueEnd == _T("") ) {
				strWhere = strWhere + _T(" AND I.IDX_ICS = '") + sCssValueStart  + _T("'");
			}
			else if ( sCssValueStart != _T("") && sCssValueEnd != _T("") ) {
				strWhere = strWhere + _T(" AND I.IDX_ICS >= '") + sCssValueStart  + _T("' AND I.IDX_ICS <= '") + sCssValueEnd + _T("'");
			}	
		}
			
		CString sSeprateShelfCode;
		m_pCM->GetControlMgrData( _T("별치기호"), sSeprateShelfCode);	
		if ( sSeprateShelfCode != _T("") )
		{
			sBookWhere += _T(" AND B.SEPARATE_SHELF_CODE ='") + sSeprateShelfCode + _T("'");
		}

		CString sBookCodeStart, sBookCodeEnd;
		m_pCM->GetControlMgrData( _T("도서기호부터"), sBookCodeStart);
		m_pCM->GetControlMgrData( _T("도서기호까지"), sBookCodeEnd);

		if ( !sBookCodeStart.IsEmpty() && !sBookCodeEnd.IsEmpty() )
		{
			sBookWhere += _T(" AND B.BOOK_CODE BETWEEN '") + sBookCodeStart + _T("' AND '") + sBookCodeEnd + _T("'");
		}	
		else if ( !sBookCodeStart.IsEmpty() )
		{
			sBookWhere += _T(" AND B.BOOK_CODE='") + sBookCodeStart + _T("'");
		}
		else if ( !sBookCodeEnd.IsEmpty() )
		{
			sBookWhere += _T(" AND B.BOOK_CODE='") + sBookCodeEnd + _T("'");
		}

		CString strStandardNo;
		CString strWhereStandardNo;
		m_pCM->GetControlMgrData( _T("IBS_규격번호"), strStandardNo);

		INT iCh = m_MakeSearchData->ChangeHanjaToHangul( strStandardNo );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strStandardNo ) error") );
			return;
		}
		iCh = m_MakeSearchData->GetKeyWord( strStandardNo );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strStandardNo ) error") );
			return;
		}
		

		if ( strStandardNo != _T("") ){
				
				CString sWhereTitle;			
				if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
					strWhereStandardNo.Format( _T("CATSEARCH(I.IDX_STANDARDNO,'%s*',NULL)>0") , strStandardNo );
				}
				else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
					strWhereStandardNo.Format( _T("&ECOSEARCH(I.REC_KEY, IDX_BO_IDX_STANDARD, '%s*')"), strStandardNo );
				}
				else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
					AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
					return;
				}
				strWhere = strWhere + _T(" AND ") + strWhereStandardNo;
				
			}
	}

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
		sBookWhere = strAccessionQuery + _T(" AND ") + sBookWhere + _T(" AND A.REC_KEY = B.ACCESSION_REC_KEY");
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
		strFrom = _T(" BO_BOOK_TBL B ");

		if (!strAccessionQuery.IsEmpty())
		{
			strFrom += _T(", CO_ACCESSION_REC_TBL A ");
		}

		if (!strSeqNoQuery.IsEmpty())
		{
			strFrom += _T(", BO_PURCHASE_SEQ_NO_TBL P ");
		}
		if (!strSeqNoQuery.IsEmpty())
		{   
			sBookWhere += _T("AND B.SPECIES_KEY = I.REC_KEY ") ;
			strFrom += _T(", IDX_BO_TBL I ");
		}

		CString sEcoBookWhere;
		if ( sBookWhere != _T("") && m_pInfo->GetSearchIndexType() == ECO_SEARCH && m_bDetail ) 
		{
			if ( iIntersect >= 2 ) {
				CString sSubQuery;
				if ( sKeyWord != _T("") ) {				
					CString sWhereTitle;
					sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_KEYWORD, '%s*') "), sKeyWord );
					sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
				}
				if ( sSubject != _T("") ) {				
					CString sWhereTitle;
					sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_SUBJECT, '%s*') "), sSubject );
					sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
				}
				if ( sTitle != _T("") ) {
					CString sWhereTitle;				
					sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_TITLE, '%s*') "), sTitle );
					sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
				}
				if ( sAuthor != _T("") ) {
					CString sWhereTitle;				
					sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_AUTHOR, '%s*') "), sAuthor );
					sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
				}
				if ( sPublisher != _T("") ) {
					CString sWhereTitle;				
					sWhereTitle.Format( _T("&ECOSEARCH(, BO_IDX_PUBLISHER, '%s*') "), sPublisher );
					sSubQuery = sSubQuery + _T(" intersect ") + sWhereTitle;
				}
				sEcoBookWhere = sEcoBookWhere + _T(" B.SPECIES_KEY IN ( ") + sSubQuery.Mid(10) + _T(" )");
				sEcoBookWhere = sBookWhere + _T(" AND ") + sEcoBookWhere;
			}
			else {	
				CString sWhereTitle;
				if ( sTitle != _T("") ) 
				{				
					sWhereTitle.Format( _T("&ECOSEARCH(B.SPECIES_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
					sEcoBookWhere = sBookWhere + _T(" and ") + sWhereTitle;				
				}
				if ( sKeyWord != _T("") ) 
				{
					sWhereTitle.Format( _T("&ECOSEARCH(B.SPECIES_KEY, BO_IDX_KEYWORD, '%s*')"), sKeyWord );
					if ( sEcoBookWhere != _T("") ) {
						sEcoBookWhere = sEcoBookWhere + _T(" and ") + sWhereTitle;
					}
					else {
						sEcoBookWhere = sBookWhere + _T(" and ") + sWhereTitle;
					}
				}
				if ( sSubject != _T("") ) 
				{
					sWhereTitle.Format( _T("&ECOSEARCH(B.SPECIES_KEY, BO_IDX_SUBJECT, '%s*')"), sSubject );
					if ( sEcoBookWhere != _T("") ) {
						sEcoBookWhere = sEcoBookWhere + _T(" and ") + sWhereTitle;
					}
					else {
						sEcoBookWhere = sBookWhere + _T(" and ") + sWhereTitle;
					}
				}
				if ( sAuthor != _T("") ) 
				{
					sWhereTitle.Format( _T("&ECOSEARCH(B.SPECIES_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
					if ( sEcoBookWhere != _T("") ) {
						sEcoBookWhere = sEcoBookWhere + _T(" and ") + sWhereTitle;
					}
					else {
						sEcoBookWhere = sBookWhere + _T(" and ") + sWhereTitle;
					}
				}
				if ( sPublisher != _T("") ) 
				{
					sWhereTitle.Format( _T("&ECOSEARCH(B.SPECIES_KEY, BO_IDX_PUBLISHER, '%s*')"), sPublisher );
					if ( sEcoBookWhere != _T("") ) {
						sEcoBookWhere = sEcoBookWhere + _T(" and ") + sWhereTitle;
					}
					else {
						sEcoBookWhere = sBookWhere + _T(" and ") + sWhereTitle;
					}
				}
			}
		}
		if ( sEcoBookWhere != _T("") ) {
			strWhere = strWhere + _T(" AND s.REC_KEY IN (select distinct b.species_key from ") + strFrom + _T(" where ") + sEcoBookWhere + _T(")");
		}
		else {
			strWhere = strWhere + _T(" AND s.REC_KEY IN (select distinct b.species_key from ") + strFrom + _T(" where ") + sBookWhere + _T(")");
		}
	}
     
	CString strSpecialQuery_BookCount;

	if( 0 <= strFrom.Find(_T("BO_PURCHASE_SEQ_NO_TBL")) )
	{
		strSpecialQuery_BookCount.Format(_T("(SELECT COUNT(b.REC_KEY) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY AND P.MANAGE_CODE = UDF_MANAGE_CODE )||' ('||(SELECT COUNT(1) FROM BO_BOOK_TBL B WHERE b.SPECIES_KEY = S.REC_KEY)||')'"), strFrom, sBookWhere);
	}
	else
	{
		strSpecialQuery_BookCount.Format(_T("(SELECT COUNT(b.REC_KEY) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY )||' ('||(SELECT COUNT(1) FROM BO_BOOK_TBL B WHERE b.SPECIES_KEY = S.REC_KEY)||')'"), strFrom, sBookWhere);
	}

	CString strSpecialQuery_MinRegNo;
	
	strSpecialQuery_MinRegNo.Format(
		_T("NVL((SELECT MIN(b.REG_NO) FROM %s WHERE %s AND b.SPECIES_KEY = S.REC_KEY AND BOOK_APPENDIX_FLAG = 'B'),(SELECT MIN(b.REG_NO) FROM BO_BOOK_TBL B WHERE B.SPECIES_KEY = S.REC_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.REG_NO IS NOT NULL ))")
		, strFrom, sBookWhere);

	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM_Result->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM_Result->RefList.GetNext(pos);
		if (!pRef) continue;

		if (pRef->FIELD_ALIAS == _T("책수"))
		{
			pRef->FIELD_NAME = strSpecialQuery_BookCount;
		}
		else if (pRef->FIELD_ALIAS == _T("시작등록번호"))
		{
			pRef->FIELD_NAME = strSpecialQuery_MinRegNo;
		}
	}

   	m_Bo_ProgressDlg = NULL ;
	m_Bo_ProgressDlg = new CBO_CAT_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow  () ;	
	m_Bo_ProgressDlg->SetTitle (_T("자료를 검색하고 있습니다."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;

	BEGIN_SBL();
	if (m_pDM_Result->RefreshDataManager(strWhere,m_Bo_ProgressDlg->m_ProgressCtrl)<0) return ;
	INT nRowCount =  m_pDM_Result->GetRowCount();
	END_SBL(nRowCount);
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	
    
     delete m_Bo_ProgressDlg ;
	 m_Bo_ProgressDlg = NULL ;

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

		pDM_Where->SetCellData(_T("조건절"), strWhere, 0);

		pDM_Where->SetCellData(_T("책조건절"), sBookWhere, 0);
	}

	m_pDM_Result->SORT(_T("시작등록번호"));

	for ( INT i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ ) {
		CString sCatalogStatus = m_pDM_Result->GetCellData( _T("BS_정리상태"), i );
		if ( sCatalogStatus == _T("0") ) {
			m_pDM_Result->SetCellData( _T("BS_정리상태"), _T("정리안됨"), i );
		}
		else if ( sCatalogStatus == _T("1") ) {
			m_pDM_Result->SetCellData( _T("BS_정리상태"), _T("정리됨"), i );
		}
	}

	m_pCM_Parent->AllControlDisplay();	

	if( nRowCount > 0 )
		ShowWindow(SW_HIDE);
	
	EFS_END
}

VOID CBO_CAT_SearchDlg::OnbFileSearch() 
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_검색_등록번호"));
	if (pDM == NULL) return ;
	
	INT ids = SanFileAPI::RegNoFileLoad(pDM, this);
	if (ids < 0) return ;
	
	
	INT nRowCount = 0;
	nRowCount= pDM->GetRowCount();
	
	if (nRowCount <= 0) return ;
	
	CString strRegNo;
	CString strBookWhere = _T("b.REG_NO IN ( ");
	
	for (INT idx = 0; idx < nRowCount ; idx++)
	{
		strRegNo = pDM->GetCellData(_T("등록번호"), idx);
		if (strRegNo.IsEmpty()) continue;
		
		strBookWhere += _T("'") + strRegNo + _T("'");
		
		if (idx != nRowCount - 1 && idx % 1000 != 0 )
			strBookWhere += _T(" , ");
		
		if ( idx % 1000 == 0 && nRowCount != 1 )
		{
			strBookWhere += _T(") OR b.REG_NO IN (");
		}
		
	}
	strBookWhere = _T("( ") + strBookWhere + _T(" )");
	
	strBookWhere += _T(") AND ") + GetWorkingStatusSQL();
	
	CString strWhere;
	strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL b WHERE %s)"), strBookWhere);	

	CString strSpecialQuery_BookCount;
	strSpecialQuery_BookCount.Format(_T("(SELECT COUNT(b.REC_KEY) FROM BO_BOOK_TBL B WHERE %s AND b.SPECIES_KEY = S.REC_KEY)||' ('||( SELECT COUNT(1) FROM BO_BOOK_TBL B WHERE b.SPECIES_KEY = S.REC_KEY)||')'"), strBookWhere);
		
	CString strSpecialQuery_MinRegNo;	

	strSpecialQuery_MinRegNo.Format(_T("(SELECT MIN(b.REG_NO) FROM BO_BOOK_TBL B WHERE %s AND b.SPECIES_KEY = S.REC_KEY)"), strBookWhere);

	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM_Result->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM_Result->RefList.GetNext(pos);
		if (!pRef) continue;

		if (pRef->FIELD_ALIAS == _T("책수"))
		{
			pRef->FIELD_NAME = strSpecialQuery_BookCount;
		}
		else if (pRef->FIELD_ALIAS == _T("시작등록번호"))
		{
			pRef->FIELD_NAME = strSpecialQuery_MinRegNo;
		}
	}
	
	if (m_pDM_Result->RefreshDataManager(strWhere) < 0)	return ;

	INT nSourceCount;
	INT nTargetCount;
	INT nSortIdx;
	CString strSourceData;
	CString strTargetData;

	nSortIdx = 0;
	nSourceCount = pDM->GetRowCount(); 
	nTargetCount = m_pDM_Result->GetRowCount(); 
	for ( INT i=0 ; i<nSourceCount ; i++ )
	{
		strSourceData = pDM->GetCellData(_T("등록번호"), i);

		for ( INT j=nSortIdx ; j<nTargetCount ; j++ )
		{
			strTargetData = m_pDM_Result->GetCellData(_T("시작등록번호"), j);
			if ( strTargetData == strSourceData )
			{
				m_pDM_Result->SORT_SwapRow(j, nSortIdx);
				if ( nSortIdx < nTargetCount )
				{
					nSortIdx++;
				}
				break;
			}
		}		
	}
	pDM->FreeData();

	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	
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
		
		pDM_Where->SetCellData(_T("조건절"), strWhere, 0);
		
		pDM_Where->SetCellData(_T("책조건절"), strBookWhere, 0);
	}

	for ( i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ ) {
		CString sCatalogStatus = m_pDM_Result->GetCellData( _T("BS_정리상태"), i );
		if ( sCatalogStatus == _T("0") ) {
			m_pDM_Result->SetCellData( _T("BS_정리상태"), _T("정리안됨"), i );
		}
		else if ( sCatalogStatus == _T("1") ) {
			m_pDM_Result->SetCellData( _T("BS_정리상태"), _T("정리됨"), i );
		}
	}
	
	m_pCM_Parent->AllControlDisplay();	

	if( m_pDM_Result->GetRowCount() > 0 )
	{
		CDialog::OnCancel();
	}
		

EFS_END
return;
}

VOID CBO_CAT_SearchDlg::OnbDetail() 
{
	EFS_BEGIN

	m_bDetail = !m_bDetail;
	
	CRect rcClient;
	GetWindowRect(rcClient);

	CString strWindowText;
	if (m_bDetail)
	{
		rcClient.bottom += 209;
		strWindowText = _T("간략검색");
	}
	else
	{
		rcClient.bottom -= 209;
		strWindowText = _T("상세검색");
	}

	GetDlgItem(IDC_bDETAIL)->SetWindowText(strWindowText);;
	MoveWindow(rcClient);

	EFS_END
}

BOOL CBO_CAT_SearchDlg::PreTranslateMessage(MSG* pMsg) 
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

VOID CBO_CAT_SearchDlg::OnbCLEARSCREEN() 
{
	EFS_BEGIN

	m_pCM->AllControlClear();
	CESL_ControlMultiComboBox * pManageClassCode = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cMANAGE_CLASS_BC);
	GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("차수번호"));
	pManageClassCode->SelectString(_T("1"));
	pManageClassCode->ShowWindow(SW_SHOW);
	pManageClassCode->EnableWindow(TRUE);
	GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	CESL_ControlComboBox * pWorkingStatus = (CESL_ControlComboBox *)GetDlgItem(IDC_cobWorkingStatus);
	pWorkingStatus->SelectString(0, _T("전체"));

	EFS_END
}

VOID CBO_CAT_SearchDlg::OnbACTITLE() 
{
	CString sTitle;
	CAC_SEARCH_DLG dlg( this ) ;
	m_pCM->GetControlMgrData(_T("IBS_본표제"), sTitle);
	dlg.SetSearchItem( sTitle );
	dlg.SetSearchGroup(_T("서명"));
	if ( dlg.DoModal() == IDOK ) 
	{
		GetDlgItem(IDC_eTITLE_ARTICLE)->SetWindowText(dlg.GetSearchKeyword() );
		m_nAcSearchOpt = dlg.GetSearchOpt();
	}
}

VOID CBO_CAT_SearchDlg::OnbACAUTHOR() 
{
	CString sAuthor;
	CAC_SEARCH_DLG dlg( this );
	m_pCM->GetControlMgrData(_T("IBS_저작자"), sAuthor);
	dlg.SetSearchItem( sAuthor );
	dlg.SetSearchGroup(_T("저자"));
	if ( dlg.DoModal() == IDOK ) 
	{
		GetDlgItem(IDC_eAUTHOR_BAP)->SetWindowText(dlg.GetSearchKeyword() );
		m_nAcSearchOpt = dlg.GetSearchOpt();
	}
}

VOID CBO_CAT_SearchDlg::OnbtnSEARCHTYPE() 
{	
	CEdit	*pBtn;
	CRect	rectBtn;	
	CString strText;
	INT		nWidht;
	INT		nCommaPos;
	

	( (CButton*) GetDlgItem (IDC_btnSEARCH_TYPE) )->GetWindowText(strText);
	if ( strText == _T("범위") )
	{
		( (CButton*) GetDlgItem (IDC_btnSEARCH_TYPE   ) )->SetWindowText(_T("구분"));
		( (CEdit*  ) GetDlgItem (IDC_eACCESSION_NO_END) )->ShowWindow(TRUE);	
		( (CStatic*) GetDlgItem (IDC_STATIC_03        ) )->ShowWindow(TRUE);
		( (CEdit*  ) GetDlgItem (IDC_eACCESSION_NO_START) )->SetLimitText(12);
		( (CEdit*  ) GetDlgItem (IDC_eACCESSION_NO_START) )->GetWindowText(strText);
		nCommaPos = strText.Find(_T(","), 0 );
		if ( nCommaPos > 12 || nCommaPos == -1 ) 
		{
			nCommaPos = 12;			
		}
		
		( (CEdit*  ) GetDlgItem (IDC_eACCESSION_NO_START) )->SetWindowText(strText.Left(nCommaPos));


		pBtn = ((CEdit*) GetDlgItem (IDC_eACCESSION_NO_START)) ;
		pBtn->GetWindowRect(&rectBtn);
		ScreenToClient(rectBtn);	
		nWidht =  rectBtn.Width()-105;
		pBtn->MoveWindow(rectBtn.left, rectBtn.top, nWidht, rectBtn.Height() );
	}
	else
	{
		( (CButton*) GetDlgItem (IDC_btnSEARCH_TYPE   ) )->SetWindowText(_T("범위"));
		( (CStatic*) GetDlgItem (IDC_STATIC_03        ) )->ShowWindow(FALSE);
		( (CEdit*  ) GetDlgItem (IDC_eACCESSION_NO_END) )->ShowWindow(FALSE);
		( (CEdit*  ) GetDlgItem (IDC_eACCESSION_NO_START) )->SetLimitText(128);
		pBtn = ((CEdit*) GetDlgItem (IDC_eACCESSION_NO_START)) ;
		pBtn->GetWindowRect(&rectBtn);
		ScreenToClient(rectBtn);	
		nWidht =  rectBtn.Width()+105;
		pBtn->MoveWindow(rectBtn.left, rectBtn.top, nWidht, rectBtn.Height() );
		
	}
}

VOID CBO_CAT_SearchDlg::OnSetfocuseACCESSIONNOSTART() 
{
	SetIME(IDC_eACCESSION_NO_START, 0);	
}

VOID CBO_CAT_SearchDlg::OnSetfocuseACCESSIONNOEND() 
{
	SetIME(IDC_eACCESSION_NO_END, 0);		
}


VOID CBO_CAT_SearchDlg::OnSetfocuseTITLEARTICLE() 
{
	SetIME(IDC_eTITLE_ARTICLE, 1);		
}

VOID CBO_CAT_SearchDlg::OnSetfocuseKEYWORD() 
{
	SetIME(IDC_eKEYWORD, 1);			
}

VOID CBO_CAT_SearchDlg::OnSetfocuseAUTHORBAP() 
{
	SetIME(IDC_eAUTHOR_BAP, 1);
	
}

VOID CBO_CAT_SearchDlg::OnSetfocusePUBLISHERBCP() 
{
	SetIME(IDC_ePUBLISHER_BCP, 1);
	
}

VOID CBO_CAT_SearchDlg::OnSetfocuseControlNoStart() 
{
	SetIME(IDC_eControlNoStart, 0);
}

VOID CBO_CAT_SearchDlg::OnSetfocuseControlNoEnd() 
{
	SetIME(IDC_eControlNoEnd, 0);	
}

VOID CBO_CAT_SearchDlg::OnSetfocuseSUBJECT() 
{
	SetIME(IDC_eSUBJECT, 1);	
	
}

VOID CBO_CAT_SearchDlg::OnSetfocusePUBLISHCOUNTRYCODE() 
{
	SetIME(IDC_ePUBLISH_COUNTRY_CODE, 0);	
	
}

VOID CBO_CAT_SearchDlg::SetIME(INT nIDC, INT nMode)
{
	EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
			

	EFS_END
}


VOID CBO_CAT_SearchDlg::OnbSearchTest() 
{
	CStdioFile file ;
	CString strLine ;
	CString sTemp ;
	CString strItem;
	CString SearchWord;
	INT		idx;

	sTemp = _T("..\\tunning\\검색Test.txt") ;	
	if(!file.Open (sTemp,CFile::modeRead | CFile::typeBinary) ) return;	
	
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

	FILE *fpTime;
	fpTime = _tfopen( _T("..\\tunning\\정리검색수행시간.txt"), _T("a+b") );	

	DWORD start_total, end_total;
	start_total = GetTickCount();
	
	DWORD start_app, end_app;
	CString strMsg;		

		fseek(fpTime, 0, SEEK_END);		
		if( 0 == ftell(fpTime) )
		{
			/*UNCHANGE*/fputc(0xFF, fpTime);
			/*UNCHANGE*/fputc(0xFE, fpTime);
		}
		// ------------------------------------------------------------------------------
		
	SearchWord = _T("");
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;		
		if (strLine.Left(1) == _T("[") || strLine.IsEmpty()) continue;	
		
		if ( strLine.Left(1) == _T("/") )
		{
			start_app = GetTickCount();
			OnbSEARCHCAT();
			end_app = GetTickCount();			
			strMsg.Format(_T("%s\r\n \t %d.%03d\r\n===================================================\r\n"), SearchWord, (end_app-start_app)/1000, (end_app-start_app)%1000);
			_ftprintf( fpTime, strMsg );
			SearchWord = _T("");
			( (CEdit*) GetDlgItem( IDC_eSEQ_MAKE_YEAR ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eSEQ_NO_START ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eSEQ_NO_END ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eACCESSION_MAKE_YEAR ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eACCESSION_REC_NO ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eACCESSION_REC_NO2 ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eACCESSION_NO_START ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eACCESSION_NO_END ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eTITLE_ARTICLE ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eKEYWORD ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eAUTHOR_BAP ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_ePUBLISHER_BCP ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eControlNoStart ) )->SetWindowText(_T(""));
			( (CEdit*) GetDlgItem( IDC_eControlNoEnd ) )->SetWindowText(_T(""));
		}

		idx = strLine.Find(_T(":"));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			
			if (sTemp == _T("차수작성년도")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eSEQ_MAKE_YEAR ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("차수번호부터")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eSEQ_NO_START ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("차수번호까지")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eSEQ_NO_END ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("원부작성년도")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eACCESSION_MAKE_YEAR ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("원부번호부터")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eACCESSION_REC_NO ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("원부번호까지")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eACCESSION_REC_NO2 ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("등록번호부터")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eACCESSION_NO_START ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("등록번호까지")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eACCESSION_NO_END ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("서명")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eTITLE_ARTICLE ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("키워드")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eKEYWORD ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("저작자")) 
			{					
				strItem = strLine.Mid(idx+1);
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eAUTHOR_BAP ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("발행자")) 
			{					
				strItem = strLine.Mid(idx+1);				
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_ePUBLISHER_BCP ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("제어번호부터")) 
			{					
				strItem = strLine.Mid(idx+1);				
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eControlNoStart ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 
			else if (sTemp == _T("제어번호까지")) 
			{					
				strItem = strLine.Mid(idx+1);				
				strItem.TrimLeft();
				strItem.TrimRight();
				( (CEdit*) GetDlgItem( IDC_eControlNoEnd ) )->SetWindowText(strItem);
				SearchWord += sTemp + _T(" : ") + strItem + _T("\r\n");
			} 			
		}
	}
	file.Close () ;
	end_total = GetTickCount();
	strMsg.Format(_T("전체검색시간 \t %d.%03d\r\n========================================="), (end_total-start_total)/1000, (end_total-start_total)%1000);	
	_ftprintf( fpTime, strMsg );		
	fclose(fpTime);	
}

void CBO_CAT_SearchDlg::OnSelchangecCatAcqCode() 
{
	CESL_ControlMultiComboBox * pAcqCode = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cCatAcqCode);
	CESL_ControlMultiComboBox * pManageClassCode = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cMANAGE_CLASS_BC);
	INT nCurSel;
	CString strCode;
	nCurSel = pAcqCode->GetCurSel();
	pAcqCode->GetLBText(nCurSel,strCode);
	if(strCode == _T("    ") )
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("차수번호"));
		pManageClassCode->SelectString(_T("    "));
		pManageClassCode->ShowWindow(SW_SHOW);
		pManageClassCode->EnableWindow(FALSE);
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_eSEQ_NO_START)->SetWindowText(_T(""));
		GetDlgItem(IDC_eSEQ_NO_END)->SetWindowText(_T(""));
	}
	else if( strCode == _T("1") )
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("차수번호"));
		pManageClassCode->SelectString(_T("1"));
		pManageClassCode->ShowWindow(SW_SHOW);
		pManageClassCode->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	}
	else if( strCode == _T("2") )
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("접수번호"));
		pManageClassCode->SelectString(_T("    "));
		pManageClassCode->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("그룹번호"));
		pManageClassCode->SelectString(_T("    "));
		pManageClassCode->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	}
	
}
HBRUSH CBO_CAT_SearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

