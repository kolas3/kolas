// BO_CAT_PROC_0100.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0100.h"
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\SanFileAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0100 dialog


CBO_CAT_PROC_0100::CBO_CAT_PROC_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0100::~CBO_CAT_PROC_0100()
{
}

VOID CBO_CAT_PROC_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0100)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bFileCall, OnbFileCall)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bCat0100Close, OnbCat0100Close)
	ON_BN_CLICKED(IDC_bSEARCH2, OnbSEARCH2)
	ON_BN_CLICKED(IDC_bFileCall2, OnbFileCall2)
	ON_BN_CLICKED(IDC_bCat0100DETAIL, OnbCat0100DETAIL)
	ON_BN_CLICKED(IDC_bPROC0100CLEAR, OnbPROC0100CLEAR)
	ON_CBN_SELCHANGE(IDC_cobWorkingStatus, OnSelchangecobWorkingStatus)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0100 message handlers

BOOL CBO_CAT_PROC_0100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	OnbDetailNo();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0100::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )	{
		OnbSEARCH();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0100::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sWORKING_STATUS;
	
	CString sControlNo, sMAT_CODE , sMEDIA_CODE;
	CString sTitle, sKeyWord, sAuthor, sPublisher;
	
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BB_수입구분_코드"), sACQUISIT_CODE);
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BS_수입년도"), sACQUISIT_YEAR);
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("원부번호부터"), sAccessionNoStart);
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("원부번호까지"), sAccessionNoEnd);
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BB_등록번호_시작"), sRegNoStart);
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BB_등록번호_까지"), sRegNoEnd);

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > 2 ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
		}
	}

	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BS_자료상태_코드"), sWORKING_STATUS);

	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("제어번호"), sControlNo);
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BS_자료구분_코드"), sMAT_CODE);	
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("BS_매체구분_코드"), sMEDIA_CODE);	
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("IBS_본표제"), sTitle);	
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("IBS_키워드"), sKeyWord);	
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("IBS_저작자"), sAuthor);	
	GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("IBS_발행자"), sPublisher);	

	CString sWhere = _T("I.REC_KEY = S.REC_KEY");
	
	CString sDetail;
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetail);	

	if ( sDetail == _T("간략검색") )
	{
		CString sControlNoStart, sControlNoEnd;
		GetControlData( _T("CM_BO_CAT_PROC_0100"), _T("제어번호부터"), sControlNoStart );
		GetControlData( _T("CM_BO_CAT_PROC_0100"), _T("제어번호까지"), sControlNoEnd );
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

	if ( sDetail == _T("간략검색") )
	{
		sMAT_CODE.Replace ( _T(" ") , NULL );
		if ( sMAT_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sMEDIA_CODE.Replace ( _T(" ") , NULL );
		if ( sMEDIA_CODE != _T("") ) {
			sWhere = sWhere + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}
	}	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	CString sQuery;
	if ( (sAccessionNoStart != _T("")) && (sAccessionNoEnd == _T("")) ) {
		sQuery = _T("b.accession_rec_key = a.rec_key and a.accession_rec_no = ") + sAccessionNoStart;
	}
	if ( (sAccessionNoEnd != _T("")) && (sAccessionNoStart == _T("")) ) {
		AfxMessageBox( _T("시작 원부번호를 입력 하세요."));
		return;
	}
	if ( sAccessionNoStart != _T("") && sAccessionNoEnd != _T("") ) {
		sQuery = _T("b.accession_rec_key = a.rec_key and a.accession_rec_no >= ") + sAccessionNoStart + _T(" and a.accession_rec_no <= ") + sAccessionNoEnd;
	}

	CString sBookWhere;
	if ( sWORKING_STATUS == _T("전체자료")) {
		sBookWhere = _T("b.WORKING_STATUS > 'BOA142N'");
	}
	else if ( sWORKING_STATUS == _T("정리중자료") ) {
		sBookWhere = _T("b.WORKING_STATUS >= 'BOC111N' AND b.WORKING_STATUS <= 'BOC115N'");
	}
	else if ( sWORKING_STATUS == _T("소장자료") ) {
		sBookWhere = _T("b.WORKING_STATUS >= 'BOL112N'");
	}

	sACQUISIT_CODE.Replace ( _T(" ") , NULL );
	if ( sACQUISIT_CODE != _T("") ) {
		sBookWhere = sBookWhere + _T(" AND b.ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("'");
	}
	if ( sACQUISIT_YEAR != _T("") ) {
		sWhere = sWhere + _T(" AND S.ACQUISIT_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	CString strWhere = sWhere;

	if ( sDetail == _T("간략검색") )
	{
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

		if ( sKeyWord != _T("") ){

			CString sWhereTitle;
			
			strWhere = strWhere + _T(" AND ") + sWhereTitle;

		}

		if ( sTitle != _T("") ){
			CMakeSearchData *pMakeData = NULL;
			INT iCh = pMakeData->ChangeHanjaToHangul( sTitle );
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			iCh = pMakeData->GetKeyWord( sTitle );
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return;
			}
			
			CString sWhereTitle;

			strWhere = strWhere + _T(" AND ") + sWhereTitle;			
		}

		if ( sAuthor != _T("") ){

			CString sWhereTitle;

			strWhere = strWhere + _T(" AND ") + sWhereTitle;			
		}

		if ( sPublisher != _T("") ){
			CString sWhereTitle;

			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}

		CString sPUB_YEAR_START, sPUB_YEAR_END;
		GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("IBS_발행년도2_시작"), sPUB_YEAR_START);
		GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("IBS_발행년도2_까지"), sPUB_YEAR_END);
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

		CString sCssDiv , sCssValueStart, sCssValueEnd;
		GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("분류표구분"), sCssDiv);
		GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("분류기호부터"), sCssValueStart);
		GetControlData(_T("CM_BO_CAT_PROC_0100"), _T("분류기호까지"), sCssValueEnd);
		
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
	}
	
	CString sDateSelect;
	CString sDateStart, sDateEnd;
	GetControlData( _T("CM_BO_CAT_PROC_0100"), _T("날짜선택"), sDateSelect );
	GetControlData( _T("CM_BO_CAT_PROC_0100"), _T("날짜부터"), sDateStart );
	GetControlData( _T("CM_BO_CAT_PROC_0100"), _T("날짜까지"), sDateEnd );
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

	if ( sQuery != _T("") ) {
		sBookWhere = sQuery + _T(" AND ") + sBookWhere + _T(" AND A.REC_KEY = B.ACCESSION_REC_KEY");
	}

	if ( sBookWhere != _T("") ) {
		if ( sQuery != _T("") ) {
			strWhere = strWhere + _T(" AND S.REC_KEY IN (select distinct b.species_key from bo_book_tbl b , co_accession_rec_tbl a where ") + sBookWhere + _T(")");
		}
		else {
			strWhere = strWhere + _T(" AND S.REC_KEY IN (select distinct b.species_key from bo_book_tbl b where ") + sBookWhere + _T(")");
		}
	}
	m_sWhere = strWhere;

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	
	pDM->SORT( _T("시작등록번호"), TRUE );

	CESL_DataMgr *pWhere;
	pWhere = FindDM( _T("DM_BO_CAT_PROC_WHERE") );
	if ( pWhere == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_WHERE ERROR") );
		return;
	}

	if(0 == pWhere->GetRowCount())
		pWhere->InsertRow(-1);
	pWhere->SetCellData( _T("조건절"), m_sWhere, 0 );

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	pCM->AllControlDisplay();
	

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbFileCall() 
{
EFS_BEGIN

	CESL_DataMgr *pDM_0100;
	pDM_0100 = FindDM(_T("DM_BO_CAT_PROC_0100"));
	if ( pDM_0100 == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0100 error") );
		return;
	}

	SanFileAPI::SanFileLoad( pDM_0100, this );

	INT iRowCount = pDM_0100->GetRowCount();
	CString sOption;
	sOption = _T("REG_NO = '") + pDM_0100->GetCellData( _T("등록번호"), 0 ) + _T("'");
	for ( INT i = 1 ; i < iRowCount ; i++ ) {
		CString sRegNo = pDM_0100->GetCellData( _T("등록번호"), i );
		sOption += _T(" OR REG_NO = '") + sRegNo + _T("'");
	}

	CESL_DataMgr *pDM_0000;
	pDM_0000 = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( pDM_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ") + sOption + _T(")");

	strWhere += _T(" ORDER BY I.IDX_TITLE");
	if (pDM_0000->RefreshDataManager(strWhere) < 0)	return;
	if ( pDM_0000->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbDetailNo() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CString sDetailSee;
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetailSee);

	if ( sDetailSee == _T("간략검색") )
	 {
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom -= 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("상세검색"));
		GetDlgItem(IDC_STATIC_01)->ShowWindow( false );		
	}
	else
	{
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom += 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("간략검색"));
		GetDlgItem(IDC_STATIC_01)->ShowWindow( true );
	}

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbCat0100Close() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbSEARCH2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	OnbSEARCH();

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbFileCall2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	OnbFileCall();

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbCat0100DETAIL() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CRect rect;
	this->GetWindowRect( rect );
	
	rect.bottom += 180;

	this->MoveWindow( rect );		

	
	CWnd *pWnd;
	UINT wndID[ 9 ] = {
		IDC_STATIC, IDC_eACCESSION_NO_START, IDC_eACCESSION_NO_END, IDC_cDATA_FORM, IDC_STATIC_01, IDC_STATIC_02, IDC_STATIC_03, IDC_STATIC_04, IDC_STATIC_05 };
	for( INT i = 0; i < 9; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );		
		pWnd->ShowWindow( TRUE );
	}
	
	CButton *pBtn;
	UINT btnID[ 5 ] = {
		IDC_bSEARCH2, IDC_bFileCall2, IDC_bCat0100DETAIL, IDC_bCat0100Close, IDC_STATIC_06 };
	
	for( i = 0; i < 5; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( btnID[ i ] );		
		pBtn->ShowWindow( FALSE );
	}

EFS_END
}

VOID CBO_CAT_PROC_0100::OnbPROC0100CLEAR() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0100"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0100 error") );
		return;
	}
	pCM->AllControlClear();

EFS_END
}

VOID CBO_CAT_PROC_0100::OnSelchangecobWorkingStatus() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here	
	CComboBox *pCobDate = NULL;
	pCobDate = (CComboBox*)GetDlgItem(IDC_COB_DateSelect);

	CString sWorkStatus;
	GetControlData( _T("CM_BO_CAT_PROC_0100"), _T("BS_자료상태_코드"), sWorkStatus );
	if ( sWorkStatus == _T("정리중자료") ) {
		pCobDate->DeleteString( 2 );
		pCobDate->SetCurSel( 0 );
	}
	else if ( sWorkStatus == _T("소장자료") ) {
		pCobDate->InsertString( 2, _T("배가일자") );
		pCobDate->SetCurSel( 0 );
	}

EFS_END
}


VOID CBO_CAT_PROC_0100::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
HBRUSH CBO_CAT_PROC_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
