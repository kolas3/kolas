// BO_LOC_1110.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1110.h"

#include "..\LOC_CommonAPI\SanFileAPI.h"

#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1110 dialog


CBO_LOC_1110::CBO_LOC_1110(CESL_Mgr* pParent /*=NULL*/, CString strMenuAlias)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Result = NULL;
	m_pCM_Parent = NULL;

	m_bDetail = TRUE;

	m_strMenuAlias = strMenuAlias;

	m_pMakeSearchData = new CMakeSearchData(this);

	m_pCM = NULL;
}

CBO_LOC_1110::~CBO_LOC_1110()
{
	delete m_pMakeSearchData;
}

BOOL CBO_LOC_1110::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CBO_LOC_1110::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_1110, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1110)
	ON_BN_CLICKED(IDC_bSEARCH_CAT, OnbSEARCHCAT)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDetail)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bFileSearch, OnbFileSearch)
	ON_EN_SETFOCUS(IDC_eControlNoEnd, OnSetfocuseControlNoEnd)
	ON_EN_SETFOCUS(IDC_eControlNoStart, OnSetfocuseControlNoStart)
	ON_EN_SETFOCUS(IDC_eAUTHOR_BAP, OnSetfocuseAUTHORBAP)
	ON_EN_SETFOCUS(IDC_eKEYWORD, OnSetfocuseKEYWORD)
	ON_EN_SETFOCUS(IDC_ePUBLISHER_BCP, OnSetfocusePUBLISHERBCP)
	ON_EN_SETFOCUS(IDC_eTITLE_ARTICLE, OnSetfocuseTITLEARTICLE)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cCatAcqCode, OnSelchangecCatAcqCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString CBO_LOC_1110::GetWorkingStatusSQL()
{
	EFS_BEGIN

	CString strResult;

	CString strWorkingStatus;
	m_pCM->GetControlMgrData(_T("BS_자료상태_코드"), strWorkingStatus);

	if (strWorkingStatus == _T("열람인계자료"))
	{
		strResult = _T("b.WORKING_STATUS = 'BOL111O'");
	}
	else if( strWorkingStatus == _T("소장자료"))
	{
		strResult = _T("b.WORKING_STATUS IN (")_T(HOLD_SEARCH_CONDITION)_T(" , 'BOL217O' )");		
	}	
	return strResult;

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1110 message handlers

BOOL CBO_LOC_1110::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if (InitESL_Mgr(_T("BO_LOC_1110")) < 0)
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pDM_Result = FindDM(_T("DM_BO_LOC_1100"));
	m_pCM_Parent = FindCM(_T("CM_BO_LOC_1100"));
	m_pCM = FindCM(_T("CM_BO_LOC_1110"));


	if (m_pDM_Result == NULL ||
		m_pCM_Parent == NULL ||
		m_pCM == NULL) 
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cmbREGCODE);
	if ( !pCB ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );
	pCB->SetCurSel( -1 );

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_1110"), _T("BB_등록번호_시작"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_1110"), _T("BB_등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_1110"), _T("BB_등록번호_까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_1110"), _T("BB_등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	OnbDetail();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
	return FALSE;
}

VOID CBO_LOC_1110::OnbSEARCHCAT() 
{
	EFS_BEGIN

	INT ids = 0;
	INT nCondCnt = 0;
	BOOL bAccessionNoStatus = FALSE;

	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );
	CString sFinalWhere = _T("");
	CString sBasicWhere = _T("");
	CString sDetailWhere = _T("");
	CString sLog = _T("");
	DWORD START, FINISH;
 
	ids = MakeBasicCond( sBasicWhere, nCondCnt, bAccessionNoStatus );
	if ( 0!=ids ) return;

	if ( m_bDetail )
	{
		ids = MakeDetailCond( fp, sDetailWhere, nCondCnt, bAccessionNoStatus );
		if ( 0!=ids ) return;
	}

	sFinalWhere.Format( _T(" %s%s"), sBasicWhere, sDetailWhere );

	if( sFinalWhere.IsEmpty() || nCondCnt == 0 )
	{
		ids = MessageBox(	_T("검색 조건이 부족하여 검색 시간이 오래 걸릴 수 있습니다.\r\n")
							_T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) "),
							_T("검색 경고") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO ) return ;		
	}

START = GetTickCount();
	if (m_pDM_Result->RefreshDataManager(sFinalWhere) < 0)	return;
FINISH = GetTickCount();
sLog.Format( _T("[ END ] - 단행, 배가일자기록 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
m_pDM_Result->QueryLog( sLog.GetBuffer( 0 ) );

	INT ii = m_pDM_Result->GetRowCount();

	ids = ConvertResultDM( );
	if ( 0!=ids ) return;
	
	EFS_END
}

INT CBO_LOC_1110::MakeBasicCond(CString &sBasicWhere, INT &nCondCnt, BOOL &bAccessionNoStatus)
{
	EFS_BEGIN

	INT ids = 0;
	CString sMsg = _T("");
	CString sTmpWhere = _T("");
	
	CString sAcquisitCode = _T("");
	CString sAcquisitYear = _T("");
	
	CString sSeqMakeYear = _T("");
	CString sSeqNoStart = _T("");
	CString sSeqNoEnd = _T("");
	
	CString sAccessionMakeYear = _T("");
	CString sAccessionNoStart = _T("");
	CString sAccessionNoEnd = _T("");
	
	CString sDateAlias = _T("");
	CString sDateStart = _T("");
	CString sDateEnd = _T("");
	
	ids = m_pCM->GetControlMgrData(_T("BB_수입구분_코드"), sAcquisitCode);
	ids = m_pCM->GetControlMgrData(_T("BS_수입년도"), sAcquisitYear);
	
	ids = m_pCM->GetControlMgrData(_T("차수작성년도"), sSeqMakeYear);
	ids = m_pCM->GetControlMgrData(_T("차수번호부터"), sSeqNoStart);
	ids = m_pCM->GetControlMgrData(_T("차수번호까지"), sSeqNoEnd);
	
	ids = m_pCM->GetControlMgrData(_T("원부작성년도"), sAccessionMakeYear);
	ids = m_pCM->GetControlMgrData(_T("원부번호부터"), sAccessionNoStart);
	ids = m_pCM->GetControlMgrData(_T("원부번호까지"), sAccessionNoEnd);
	
	ids = m_pCM->GetControlMgrData(_T("날짜선택"), sDateAlias);
	ids = m_pCM->GetControlMgrData(_T("날짜부터"), sDateStart);
	ids = m_pCM->GetControlMgrData(_T("날짜까지"), sDateEnd);
	
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeBasicCond") );

	sBasicWhere = GetWorkingStatusSQL( );
	
	if ( !sAcquisitCode.IsEmpty( ) )
		sBasicWhere +=  _T(" AND B.ACQ_CODE = '") + sAcquisitCode + _T("'");
		
	if ( !sTmpWhere.IsEmpty( ) )	sTmpWhere += _T(" AND");
	
	if (!sSeqMakeYear.IsEmpty( ) && !sSeqNoStart.IsEmpty() )
	{
		sTmpWhere.Format(	_T(" (SELECT MAX(TO_CHAR(P.SEQ_NO_CREATE_DATE, 'YYYY'))")
							_T(" FROM BO_PURCHASE_SEQ_NO_TBL P")
							_T(" WHERE BB.SEQ_NO = P.SEQ_NO) = '%s'"), sSeqMakeYear );
	}
	
	if ( (sSeqNoStart.IsEmpty()) && (!sSeqNoEnd.IsEmpty()) ) 
	{
		AfxMessageBox( _T("시작 차수번호를 입력하세요") );
		return 1001;
	}
	else if ( !sSeqNoStart.IsEmpty() && sSeqMakeYear.IsEmpty() ) 
	{
		AfxMessageBox( _T("차수 작성년도를 입력하세요") );
		return 1001;
	}
	else if ( (!sSeqNoStart.IsEmpty()) && (sSeqNoEnd.IsEmpty()) )
		sTmpWhere += _T(" AND BB.SEQ_NO = ") + sSeqNoStart;
	else if ( (!sSeqNoStart.IsEmpty()) && (!sSeqNoEnd.IsEmpty()) )
		sTmpWhere += _T(" AND BB.SEQ_NO BETWEEN ") + sSeqNoStart + _T(" AND ") + sSeqNoEnd;

	if ( !sTmpWhere.IsEmpty( ) )
	{
		sBasicWhere +=	_T(" AND B.SPECIES_KEY IN ( SELECT BB.SPECIES_KEY FROM BO_BOOK_TBL BB WHERE ")
						+ sTmpWhere + _T(")");
		sTmpWhere.Empty( );
		nCondCnt++;
	}
	
	sAccessionNoStart.Replace( _T(" "), _T("") );
	sAccessionNoEnd.Replace( _T(" "), _T("") );
	
	if (sAccessionNoStart.IsEmpty() && !sAccessionNoEnd.IsEmpty()) 
	{
		AfxMessageBox(_T("시작 원부번호를 입력 하세요"));
		return 1002;
	}
	else if ( !sAccessionNoStart.IsEmpty() && sAccessionMakeYear.IsEmpty() )
	{
		AfxMessageBox(_T("원부 작성 년도를 입력하세요"));
		return 1002;
	}
	else if (!sAccessionNoStart.IsEmpty() && sAccessionNoEnd.IsEmpty())
	{
		bAccessionNoStatus = TRUE;
		sTmpWhere.Format(_T(" ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionMakeYear );
		sTmpWhere += _T(" AND ACCESSION_REC_NO = ")+ sAccessionNoStart;
	}
	else if (!sAccessionNoStart.IsEmpty() && !sAccessionNoEnd.IsEmpty())
	{	
		sTmpWhere.Format(_T(" ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionMakeYear );
		sTmpWhere += _T(" AND ACCESSION_REC_NO BETWEEN ") + sAccessionNoStart + _T(" AND ") + sAccessionNoEnd;
	}
					
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sBasicWhere +=	_T(" AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ")
						+ sTmpWhere + _T(")");
		sTmpWhere.Empty( );
		nCondCnt++;
	}
	
	sDateAlias.Replace( _T(" "), _T("") );	
	if ( _T("입력일자")==sDateAlias )		sDateAlias = _T("INPUT_DATE");
	else if ( _T("등록일자")==sDateAlias )	sDateAlias = _T("REG_DATE");
	else if ( _T("배가일자")==sDateAlias )	sDateAlias = _T("SHELF_DATE");
	else if ( _T("인계일자")==sDateAlias )	sDateAlias = _T("LOC_TRANSFER_DATE");

	if (sDateStart.IsEmpty() && !sDateEnd.IsEmpty()) 
	{
		sMsg.Format( _T("%s 시작을 입력하세요."), sDateAlias );
		AfxMessageBox( sMsg );
		return 1003;
	}
	else if (!sDateStart.IsEmpty() && sDateEnd.IsEmpty())
		sTmpWhere.Format(	_T(" B.%s = TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"),
							sDateAlias, sDateStart );
	else if (!sDateStart.IsEmpty() && !sDateEnd.IsEmpty())
		sTmpWhere.Format(	_T(" B.%s BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
							_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"),
							sDateAlias, sDateStart, sDateEnd );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sBasicWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
		nCondCnt++;
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_1110::MakeDetailCond(FILE *fp, CString &sDetailWhere, INT &nCondCnt, BOOL bAccessionNoStatus)
{
EFS_BEGIN

	INT ids = 0;
	INT nIndexType = -1;
	INT nECOSearchCnt = 0;
	CString sTmpWhere = _T("");
	BOOL bIsEqualSearch = FALSE;
	CStringList whereList;

	CString sRegNoStart = _T("");
	CString sRegNoEnd = _T("");
	CString sRegCode = _T("");
	CString sMatCode = _T("");
	CString sMediaCode = _T("");
	CString sManageCode = _T("");
	CString sControlNoStart = _T("");
	CString sControlNoEnd = _T("");

	CString sClassDivAlias = _T("");
	CString sClassCodeStart = _T("");
	CString sClassCodeEnd = _T("");

	CString sPubYearStart = _T("");
	CString sPubYearEnd = _T("");

	CString sTitle = _T("");
	CString sKeyword = _T("");
	CString sAuthor = _T("");
	CString sPublisher = _T("");

	CString sUseLimitCode = _T("");

	ids = m_pCM->GetControlMgrData(_T("BB_등록구분"),      sRegCode);
	ids = m_pCM->GetControlMgrData(_T("BB_등록번호_시작"), sRegNoStart);
	ids = m_pCM->GetControlMgrData(_T("BB_등록번호_까지"), sRegNoEnd);
	
	ids = m_pCM->GetControlMgrData(_T("BS_자료구분_코드"), sMatCode);
	ids = m_pCM->GetControlMgrData(_T("BS_매체구분_코드"), sMediaCode);
	ids = m_pCM->GetControlMgrData(_T("BS_관리구분_코드"), sManageCode);

	ids = m_pCM->GetControlMgrData(_T("제어번호부터"), sControlNoStart);
	ids = m_pCM->GetControlMgrData(_T("제어번호까지"), sControlNoEnd);

	ids = m_pCM->GetControlMgrData(_T("분류표구분"), sClassDivAlias);
	ids = m_pCM->GetControlMgrData(_T("분류기호부터"), sClassCodeStart);
	ids = m_pCM->GetControlMgrData(_T("분류기호까지"), sClassCodeEnd);
	
	ids = m_pCM->GetControlMgrData(_T("IBS_발행년도2_시작"), sPubYearStart);
	ids = m_pCM->GetControlMgrData(_T("IBS_발행년도2_까지"), sPubYearEnd);

	ids = m_pCM->GetControlMgrData(_T("IBS_본표제"), sTitle);
	ids = m_pCM->GetControlMgrData(_T("IBS_키워드"), sKeyword);
	ids = m_pCM->GetControlMgrData(_T("IBS_저작자"), sAuthor);
	ids = m_pCM->GetControlMgrData(_T("IBS_발행자"), sPublisher);

	ids = m_pCM->GetControlMgrData(_T("이용제한구분"), sUseLimitCode, -1, 0);

	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeDetailCond") );

	if ( !sRegCode.IsEmpty() )
	{
		if ( !sRegNoStart.IsEmpty() )
		{
			sRegNoStart = sRegCode + sRegNoStart;
			ids = CLocCommonAPI::MakeRegNoSpaceToZero( sRegNoStart );
			if ( 1==ids && !sRegNoStart.IsEmpty( ) ) 
			{
				AfxMessageBox( _T("시작 등록번호의 형식이 다릅니다.") );
				return 1001;
			}
		}

		if ( !sRegNoEnd.IsEmpty() )
		{
			sRegNoEnd = sRegCode + sRegNoEnd;
			ids = CLocCommonAPI::MakeRegNoSpaceToZero( sRegNoEnd );
			if ( 1==ids && !sRegNoEnd.IsEmpty( ) ) 
			{
				AfxMessageBox( _T("끝 등록번호의 형식이 다릅니다.") );
				return 1001;
			}
		}

		if ( sRegNoStart.IsEmpty( ) && !sRegNoEnd.IsEmpty( ) )
		{
				AfxMessageBox( _T("시작 등록번호를 입력 하세요."));
				return 1002;
		}
		else if ( !sRegNoStart.IsEmpty( ) && sRegNoEnd.IsEmpty( ) )
		{
			sTmpWhere.Format( _T(" B.REG_NO = '%s'"), sRegNoStart);
		}
		else if ( !sRegNoStart.IsEmpty( ) && !sRegNoEnd.IsEmpty( ) )
			sTmpWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s'"), sRegNoStart, sRegNoEnd );
		if ( !sTmpWhere.IsEmpty( ) )
		{
			sDetailWhere += _T(" AND") + sTmpWhere;
			sTmpWhere.Empty( );		
		}
		sTmpWhere.Format( _T(" B.REG_CODE = '%s'"), sRegCode );
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty();
		nCondCnt++;
	}
	else
	{
		if ( !sRegNoStart.IsEmpty() || !sRegNoEnd.IsEmpty() )
		{
			AfxMessageBox( _T("등록구분을 선택 하세요.") );
			return 1002;
		}
	}

	sMatCode.Replace ( _T(" ") , NULL );
	if ( !sMatCode.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.MAT_CODE = '%s'"), sMatCode );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
	}
	sMediaCode.Replace ( _T(" ") , NULL );
	if ( !sMediaCode.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.MEDIA_CODE = '%s'"), sMediaCode );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
	}
	sManageCode.Replace ( _T(" ") , NULL );
	if ( !sManageCode.IsEmpty( ) )
		sTmpWhere.Format( _T(" B.MANAGE_CODE = '%s'"), sManageCode );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
	}

	if ( sControlNoStart.IsEmpty( ) && !sControlNoEnd.IsEmpty( ) )
	{
		AfxMessageBox( _T("시작 제어번호를 입력하세요.") );
		return 1003;
	}
	else if ( !sControlNoStart.IsEmpty( ) && sControlNoEnd.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.CONTROL_NO = '%s'"), sControlNoStart );
	else if ( !sControlNoStart.IsEmpty( ) && !sControlNoEnd.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.CONTROL_NO BETWEEN '%s' AND '%s'"), sControlNoStart, sControlNoEnd );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
		nCondCnt++;
	}

	if		( _T("1")==sClassDivAlias )	sClassDivAlias = _T("KDC_CLASS");
	else if ( _T("2")==sClassDivAlias )	sClassDivAlias = _T("DDC_CLASS");

	if ( sClassCodeStart.IsEmpty( ) && !sClassCodeEnd.IsEmpty( ) )
	{
		AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
		return 1004;
	}
	if ( !sClassCodeStart.IsEmpty( ) && sClassCodeEnd.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.%s = '%s'"), sClassDivAlias, sClassCodeStart );
	else if ( !sClassCodeStart.IsEmpty( ) && !sClassCodeEnd.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.%s BETWEEN '%s' AND '%s'"), sClassDivAlias, sClassCodeStart, sClassCodeEnd );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
		nCondCnt++;
	}

	if ( sPubYearStart.IsEmpty( ) && !sPubYearEnd.IsEmpty( ) )
	{
		AfxMessageBox ( _T("시작 발행년도를 입력하세요."));
		return 1005;
	}
	else if ( !sPubYearStart.IsEmpty( ) && sPubYearEnd.IsEmpty( ) )
		sTmpWhere.Format( _T(" I.PUB_YEAR = '%s'"), sPubYearStart );
	else if ( !sPubYearStart.IsEmpty( ) && !sPubYearEnd.IsEmpty( ) )
		sTmpWhere.Format(	_T(" I.PUB_YEAR BETWEEN '%s' AND '%s'"),
							sPubYearStart, sPubYearEnd );
	if ( !sTmpWhere.IsEmpty( ) )
	{
		sDetailWhere += _T(" AND") + sTmpWhere;
		sTmpWhere.Empty( );
		nCondCnt++;
	}

	nIndexType = m_pInfo->GetSearchIndexType( );

	if ( !sTitle.IsEmpty( ) )
	{
		bIsEqualSearch = FALSE;
		if( sTitle.GetAt(0) == '\"' )	bIsEqualSearch = TRUE;

		ids = m_pMakeSearchData->ChangeHanjaToHangul( sTitle );
		if ( 0 != ids )
		{			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return ids;
		}
		ids = m_pMakeSearchData->GetKeyWord( sTitle );
		if ( 0 != ids )
		{			
			AfxMessageBox ( _T("pMakeData->GetKeyWord( sTitle ) error") );
			return ids;
		}
		
		if( !bIsEqualSearch )	sTitle += _T("*");

		if( 1==nIndexType )
		{
			if( fp )
				sTmpWhere.Format( _T(" &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s')"), sTitle );
			else
			{
				sTmpWhere.Format( _T(" &ECOSEARCH(, BO_IDX_TITLE, '%s')"), sTitle );
				whereList.AddTail( sTmpWhere );
				sTmpWhere.Empty( );
			}
		}
		else
			sTmpWhere.Format( _T(" CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0"), sTitle );

		if ( !sTmpWhere.IsEmpty( ) )
		{
			sDetailWhere += _T(" AND") + sTmpWhere;
			sTmpWhere.Empty( );
		}
		nCondCnt++;
	}

	if ( !sKeyword.IsEmpty( ) ){
		bIsEqualSearch = FALSE;

		if( sKeyword.GetAt(0) == '\"' ) bIsEqualSearch = TRUE;
		
		m_pMakeSearchData->GetKeyWord(sKeyword);
		if( !bIsEqualSearch ) sKeyword += _T("*");

		if( 1==nIndexType )
		{
			if( fp )
				sTmpWhere.Format( _T(" &ECOSEARCH(I.REC_KEY, BO_IDX_KEYWORD, '%s')"), sKeyword );
			else
			{
				sTmpWhere.Format( _T(" &ECOSEARCH(, BO_IDX_KEYWORD, '%s')"), sKeyword );
				whereList.AddTail( sTmpWhere );
				sTmpWhere.Empty( );
			}
		}
		else
			sTmpWhere.Format( _T(" CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0"), sKeyword );

		if ( !sTmpWhere.IsEmpty( ) )
		{
			sDetailWhere += _T(" AND") + sTmpWhere;
			sTmpWhere.Empty( );
		}
		nCondCnt++;
	}

	if ( !sAuthor.IsEmpty( ) ){
		bIsEqualSearch = FALSE;

		if( sAuthor.GetAt(0) == '\"' ) bIsEqualSearch = TRUE;
		
		m_pMakeSearchData->GetKeyWord(sAuthor);
		if( !bIsEqualSearch ) sAuthor += _T("*");

		if( 1==nIndexType )
		{
			if( fp )
				sTmpWhere.Format( _T(" &ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s')"), sAuthor );
			else
			{
				sTmpWhere.Format( _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '%s')"), sAuthor );
				whereList.AddTail( sTmpWhere );
				sTmpWhere.Empty( );
			}
		}
		else
			sTmpWhere.Format( _T(" CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0"), sAuthor );

		if ( !sTmpWhere.IsEmpty( ) )
		{
			sDetailWhere += _T(" AND") + sTmpWhere;
			sTmpWhere.Empty( );
		}
		nCondCnt++;
	}

	if ( !sPublisher.IsEmpty( ) ){
		bIsEqualSearch = FALSE;
		if( sPublisher.GetAt(0) == '\"' ) bIsEqualSearch = TRUE;
		
		m_pMakeSearchData->GetKeyWord(sPublisher);
		if( !bIsEqualSearch ) sPublisher += _T("*");

		if( 1==nIndexType )
		{
			if( fp )
				sTmpWhere.Format( _T(" &ECOSEARCH(I.REC_KEY, BO_IDX_PUBLISHER, '%s')"), sPublisher );
			else
			{
				sTmpWhere.Format( _T(" &ECOSEARCH(, BO_IDX_PUBLISHER, '%s')"), sPublisher );
				whereList.AddTail( sTmpWhere );
				sTmpWhere.Empty( );
			}
		}
		else
			sTmpWhere.Format( _T(" CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0"), sPublisher );

		if ( !sTmpWhere.IsEmpty( ) )
		{
			sDetailWhere += _T(" AND") + sTmpWhere;
			sTmpWhere.Empty( );
		}
		nCondCnt++;
	}

	nECOSearchCnt = whereList.GetCount( );
	if( !fp && nECOSearchCnt )
	{
		if( 1==nECOSearchCnt )
			sTmpWhere.Format( _T(" I.REC_KEY IN %s"), whereList.GetAt( whereList.FindIndex( 0 ) ) );
		else
		{
			sTmpWhere = _T(" I.REC_KEY IN(") +  whereList.GetAt( whereList.FindIndex( 0 ) );
			for( INT i = 1 ; i < nECOSearchCnt ; i++ )
				sTmpWhere = sTmpWhere + _T(" INTERSECT ") +  whereList.GetAt( whereList.FindIndex( i ) );						
			sTmpWhere += _T(")");
		}

		if ( !sTmpWhere.IsEmpty( ) )
		{
			sDetailWhere += _T(" AND") + sTmpWhere;
			sTmpWhere.Empty( );
		}
	}

	if( !sUseLimitCode.IsEmpty() && sUseLimitCode.Compare(_T("0")) != 0 && sUseLimitCode.Compare(_T("적용안함") ) != 0 )
	{
		if( sDetailWhere.GetLength() > 1 ) sDetailWhere += _T(" AND ");
		sDetailWhere += _T(" B.USE_LIMIT_CODE = '") + sUseLimitCode + _T("'");
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_1110::ConvertResultDM()
{
	EFS_BEGIN

	const INT FACE_CNT = 4;

	INT ids = 0;
	INT nRowCount = 0;
	CString sCode = _T("");
	CString sFace = _T("");
	CString sMsg = _T("");
	CString sAccessionMakeYear = _T("");
	CString sAccessionNoStart = _T("");
	CString sAccessionNoEnd = _T("");
	CString data[ FACE_CNT ];

	ids = m_pCM->GetControlMgrData(_T("원부작성년도"), sAccessionMakeYear);
	ids = m_pCM->GetControlMgrData(_T("차수번호부터"), sAccessionNoStart);
	ids = m_pCM->GetControlMgrData(_T("차수번호까지"), sAccessionNoEnd);
	
	ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("자료실구분") , _T("자료실") );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11, _T("ConvertResultDM") );

	ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("이용제한구분") , _T("BB_이용제한구분")  );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11, _T("ConvertResultDM") );

	for( INT i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ )
	{
		ids = m_pDM_Result->GetCellData( _T("자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertResultDM") );

		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );

		ids = m_pDM_Result->SetCellData( _T("자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertResultDM") );
		
		ids = m_pDM_Result->GetCellData( _T("이전자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertResultDM") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("이전자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ConvertResultDM") );

		data[ 0 ] = m_pDM_Result->GetCellData( _T("PAGE"), i );
		data[ 1 ] = m_pDM_Result->GetCellData( _T("PHYSICAL_PROPERTY"), i );
		data[ 2 ] = m_pDM_Result->GetCellData( _T("BOOK_SIZE"), i );
		data[ 3 ] = m_pDM_Result->GetCellData( _T("ACCOMP_MAT"), i );

		sFace.Empty();

		if( !data[ 0 ].IsEmpty() )	sFace += data[ 0 ];
		if( !data[ 1 ].IsEmpty() )	sFace = sFace + _T(":") + data[ 1 ];
		if( !data[ 2 ].IsEmpty() )	sFace = sFace + _T(";") + data[ 2 ];
		if( !data[ 3 ].IsEmpty() )	sFace = sFace + _T("+") + data[ 3 ];
		m_pDM_Result->SetCellData( _T("형태사항"), sFace, i );		
	}

	CLocCommonAPI::MakeShelfCode( this , _T("DM_BO_LOC_1100") );
	
	nRowCount =  m_pDM_Result->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return 1000;
	}
	
	sMsg.Format(_T("책수 : %5d"), nRowCount);	
	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)sMsg.GetBuffer(0));

	m_pCM_Parent->AllControlDisplay();	
	CESL_Grid *pGrid = (CESL_Grid *)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	pGrid->SetRowHeight(0, 400);

	return 0;

	EFS_END
	return -1;
}

VOID CBO_LOC_1110::OnbFileSearch() 
{
	EFS_BEGIN	

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_검색_등록번호"));
	if (pDM == NULL) return;

	INT ids = SanFileAPI::RegNoFileLoad(pDM, this);
	if (ids < 0) return;
	
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount <= 0) return;

	m_pDM_Result->FreeData();

	CString strRegNo;

	CESL_DataMgr TmpDM;
	
	CLocCommonAPI::MakeCopyDM( this , m_pDM_Result , &TmpDM );
	
	INT nMaxUpdateRecordCnt = 50;
	CString sRegNoList;
	CStringArray ArrayRegNoList;
	ArrayRegNoList.RemoveAll();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		CString sRegNo;
		ids = pDM->GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("CBL_BOSHELF::SearchFileBO_LOC_1310()") );
		
		ArrayRegNoList.Add(sRegNo);
		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRegNoList += _T(",");
		sRegNoList += _T("'") +sRegNo + _T("'");
		
		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nRowCount-1)) ) 
		{
			CString sWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
			
			sWhere = GetWorkingStatusSQL( ) + _T(" AND ") + sWhere;

			ids = CLocCommonAPI::RefreshDataManager( this , &TmpDM , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -5 , _T("CBL_BOSHELF::OnbFileSearch()") );
			
			ids = AppendDM( &TmpDM , m_pDM_Result, &ArrayRegNoList );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -6 , _T("CBL_BOSHELF::OnbFileSearch()") );
			
			sRegNoList.Empty();
		}
	}
	

	nRowCount =  m_pDM_Result->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CString sRegNo;
	
	CStdioFile fFile;
	
	TCHAR sPath[500];
	_tgetcwd(sPath,500);
	CString ssPath = sPath;
	ssPath = ssPath.Mid(0,ssPath.GetLength()-4 );
	CString sFileName = ssPath + CString(_T("\\검색에실패한등록번호\\검색에실패한등록번호.txt"));
	
	_tmkdir(_T("..\\검색에실패한등록번호"));
	CFileException ex;
	ids = fFile.Open( _T("..\\검색에실패한등록번호\\검색에실패한등록번호.txt"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary  , &ex );
	if( ids == 0 )
	{
		TCHAR sFEMsg[500];
		ex.GetErrorMessage(sFEMsg ,500);
		CString sEMsg;
		sEMsg.Format(_T("파일을 생성하는데 실패했습니다.[%s]") , sFEMsg  );
		AfxMessageBox(sEMsg);
		return ;
	}

	if( 0 == fFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fFile.Write( &cUni, sizeof(TCHAR));
	}
	
	bool bIsNotEqual = FALSE;
	
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData(  i , 0 , sRegNo );
		if( ids < 0 ) return ;
		
		ids = CLocCommonAPI::GetCellData( this , m_pDM_Result , _T("등록번호") , sRegNo , _T("등록번호") , sRegNo );
		if( ids == 1 )
		{
			fFile.WriteString(sRegNo);
			fFile.WriteString(_T("\r\n"));
		}
		
	}
	
	fFile.Close();
	
	if( bIsNotEqual )
	{
		CString sMsg;
		sMsg.Format(_T("불러오지 못한 등록번호는 '%s'파일에 기록하였습니다.") , sFileName );
		AfxMessageBox( sMsg );
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM_Result , _T("자료실구분") , _T("자료실") );
	
	CString sCode;
	for( i = 0 ; i < m_pDM_Result->GetRowCount() ; i++ )
	{
		ids = m_pDM_Result->GetCellData( _T("자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbSEARCHCAT") );
		
		ids = m_pDM_Result->GetCellData( _T("이전자료상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbSEARCHCAT") );
		
		CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
		
		ids = m_pDM_Result->SetCellData( _T("이전자료상태") ,  sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -4 , _T("OnbSEARCHCAT") );
		
	}
	
	CLocCommonAPI::MakeShelfCode( this , _T("DM_BO_LOC_1100") );

	
	
	m_pCM_Parent->AllControlDisplay();	

	EFS_END

}

VOID CBO_LOC_1110::OnbDetail() 
{
	EFS_BEGIN

	m_bDetail = !m_bDetail;
	
	CRect rcClient;
	GetWindowRect(rcClient);

	CString strWindowText;
	if (m_bDetail)
	{
		rcClient.bottom += 211;
		strWindowText = _T("간략검색");
	}
	else
	{
		rcClient.bottom -= 211;
		strWindowText = _T("상세검색");
	}

	GetDlgItem(IDC_bDETAIL)->SetWindowText(strWindowText);;
	MoveWindow(rcClient);

	EFS_END
}

BOOL CBO_LOC_1110::PreTranslateMessage(MSG* pMsg) 
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

VOID CBO_LOC_1110::OnbCLEARSCREEN() 
{
	EFS_BEGIN

	m_pCM->AllControlClear();
	GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("차수번호"));
	GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);

	EFS_END
}


VOID CBO_LOC_1110::DeleteWrongAccessionRegNoRow( CESL_DataMgr *pDM, CString startNo, CString endNo , CString sCRecMakeYear)
{
	EFS_BEGIN

	INT from = _ttoi( startNo );
	INT to   = _ttoi( endNo   );
	if( endNo.IsEmpty() ) 
		to = from;
	INT cnt  = pDM->GetRowCount();
	CString sGetData = _T("");
	BOOL bDeleteRow;

	for( INT i = cnt - 1; i > -1; i-- )
	{
		bDeleteRow = FALSE;

		if( !startNo.IsEmpty( ) )
		{
			pDM->GetCellData( _T("부록정보") , i , sGetData );

			if( sGetData.Compare(_T("책")) != 0 ) continue;

			pDM->GetCellData( _T("원부번호"), i, sGetData );

			if( _ttoi(sGetData) < from || _ttoi(sGetData) > to )
			{
				pDM->DeleteRow( i );
				bDeleteRow = TRUE;
			}
		}
		
		if( !bDeleteRow ) 
		{
			if( !sCRecMakeYear.IsEmpty() )
			{
				pDM->GetCellData( _T("원부작성년도") , i , sGetData );

				if( sGetData.Compare(sCRecMakeYear) != 0 )
					pDM->DeleteRow( i );
			}
		}
	}

	EFS_END
}

VOID CBO_LOC_1110::SetIME(INT nIDC, INT nMode)
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

VOID CBO_LOC_1110::OnSetfocuseControlNoEnd() 
{
	SetIME(IDC_eControlNoEnd, 0);
}

VOID CBO_LOC_1110::OnSetfocuseControlNoStart() 
{
	SetIME(IDC_eControlNoStart, 0);
}

VOID CBO_LOC_1110::OnSetfocuseAUTHORBAP() 
{
	SetIME(IDC_eAUTHOR_BAP, 1);
}

VOID CBO_LOC_1110::OnSetfocuseKEYWORD() 
{
	SetIME(IDC_eKEYWORD, 1);
}

VOID CBO_LOC_1110::OnSetfocusePUBLISHERBCP() 
{
	SetIME(IDC_ePUBLISHER_BCP, 1);
}

VOID CBO_LOC_1110::OnSetfocuseTITLEARTICLE() 
{
	SetIME(IDC_eTITLE_ARTICLE, 1);
}

VOID CBO_LOC_1110::OnCancel() 
{		
	CDialog::OnCancel();
}

VOID CBO_LOC_1110::OnClose() 
{		
	CDialog::OnCancel();
}

VOID CBO_LOC_1110::OnDestroy() 
{
	CDialog::OnCancel();
}

INT CBO_LOC_1110::AppendDM( CString sSrcDMAlias , CString sDstDMAlias, CStringArray *RegNoList )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( sSrcDMAlias );
	if( pSrcDM == NULL ) return -100;

	CESL_DataMgr *pDstDM = FindDM( sDstDMAlias );
	if( pDstDM == NULL ) return -110;

	ids = AppendDM( pSrcDM , pDstDM, RegNoList  );
	if( ids < 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1110::AppendDM( CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM, CStringArray *RegNoList )
{
	EFS_BEGIN

	INT ids;

	CString srcValue;
	CString destValue;

	for ( INT i=0 ; i<RegNoList->GetSize() ; i++ )
	{
		srcValue = RegNoList->GetAt(i);
		for( INT j = 0 ; j < pSrcDM->GetRowCount() ; j++ )
		{
			destValue = pSrcDM->GetCellData(_T("등록번호"), j);
			if ( srcValue == destValue )
			{
				ids = AppendDM( pSrcDM , pDstDM , j , -1 );
				if( ids < 0 )  return ids;
			}
		}
	}


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1110::AppendDM(	CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM , int nSrcIndex , int nDstIndex)
{
	EFS_BEGIN
	
	INT ids = 0;

	INT nAliasCnt = 0;

	if( &pDstDM == NULL ) return -1;

	nAliasCnt = pDstDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );
	
	if( ids < 0) return ids;

	if( nDstIndex == -1 )
	{
		pDstDM->InsertRow(-1);
		nDstIndex = pDstDM->GetRowCount()-1;
	}

	CString sResult;
	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sColAlias[i] , nSrcIndex , sResult );
		if( ids < 0 && (ids != -4012) )
		{
			CString sEMsg;
			sEMsg.Format( _T("sColAlias[i] : %s ") , sColAlias[i] );
			AfxMessageBox( sColAlias[i]);
		}
		if( ids < 0 && (ids != -4012) ) return ids;
		if( ids == -4012 ) sResult.Empty();
		
		ids = pDstDM->SetCellData( sColAlias[i] , sResult , nDstIndex );
		if( ids < 0 ) return ids;
	}

	delete []sColAlias;

	return 0;

	EFS_END
	return -1;
}

HBRUSH CBO_LOC_1110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CBO_LOC_1110::OnSelchangecCatAcqCode() 
{
	CESL_ControlMultiComboBox * pAcqCode = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cCatAcqCode);
	INT nCurSel;
	CString strCode;
	nCurSel = pAcqCode->GetCurSel();
	pAcqCode->GetLBText(nCurSel,strCode);
	if(strCode == _T("    ") )
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("차수번호"));
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_eSEQ_NO_START)->SetWindowText(_T(""));
		GetDlgItem(IDC_eSEQ_NO_END)->SetWindowText(_T(""));
	}
	else if( strCode == _T("1") )
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("차수번호"));
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	}
	else if( strCode == _T("2") )
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("접수번호"));
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_sMANAGE_CLASS_BC)->SetWindowText(_T("그룹번호"));
		GetDlgItem(IDC_eSEQ_NO_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSEQ_NO_END)->EnableWindow(TRUE);
	}
	
}