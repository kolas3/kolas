// BO_CAT_ACQUISIT_NO.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_ACQUISIT_NO.h"
#include "BO_CAT_ACQUISIT_NO_0100.h"
#include "BO_CAT_ACQUISIT_NO_0200.h"
#include "BO_CAT_NUMBER_0200.h"
#include "BO_CAT_NUMBER_0300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ACQUISIT_NO dialog


CBO_CAT_ACQUISIT_NO::CBO_CAT_ACQUISIT_NO(INT nType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_ACQUISIT_NO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// CHILD
	if (nType == CHILD)
	{
		m_lpszTemplateName = MAKEINTRESOURCE(IDD);
		m_nIDHelp = IDD;
	}
	// POPUP
	else if (nType == POPUP)
	{
		m_lpszTemplateName = MAKEINTRESOURCE(IDD_POPUP);
		m_nIDHelp = IDD_POPUP;
	}

	//m_sWork = _T("");
}

CBO_CAT_ACQUISIT_NO::~CBO_CAT_ACQUISIT_NO()
{
}

BOOL CBO_CAT_ACQUISIT_NO::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_ACQUISIT_NO::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_ACQUISIT_NO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_ACQUISIT_NO, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_ACQUISIT_NO)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_CBN_SELCHANGE(IDC_cWORK_TYPE, OnSelchangecWORKTYPE)
	ON_BN_CLICKED(IDC_bOmitNumber, OnbOmitNumber)
	ON_BN_CLICKED(IDC_bDELETE_BCN, OnbDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ACQUISIT_NO message handlers

BOOL CBO_CAT_ACQUISIT_NO::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_ACQUISIT_NO")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	if ( m_iWorkCode == 2 ) {
		//SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("작업구분"), _T("누락수입순번호관리") );
		//GetDlgItem(IDC_cWORK_TYPE)->SetWindowText( _T("누락수입순번호관리") );
		CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cWORK_TYPE );
		pCombo->SetCurSel( 1 );
		
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호구분"), m_sClassType );
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호부터"), m_sClassNo );		
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("별치기호"), m_sShelfCode );

		GetDlgItem(IDC_cWORK_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_bOmitNumber)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_GRD_ACQUISIT_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRD_ACQUISIT_NO2)->ShowWindow(SW_SHOW);

		OnbSEARCH();
	}

	// 처음 화면 뜰때 수입순번호 검색되서 보이도록. 수정 ( 2004.09.20)
	/************************************************************************
	 ************************************************************************/
	//20071227 UPDATE PJW : OnbSEARCH()를 한번만 호출하기 위해서 m_iWorkCode!=2 추가
	if( m_sClassNo != _T("")  && m_iWorkCode != 2)  
	{
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호부터"), m_sClassNo );	
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("별치기호"), m_sShelfCode );

		OnbSEARCH();
	}
	/************************************************************************
	 ************************************************************************/

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_ACQUISIT_NO::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	/*
	if ( m_iWorkCode == 2 ) {
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("작업구분"), _T("누락수입순번호관리") );
		//GetDlgItem(IDC_cWORK_TYPE)->SetWindowText( _T("누락수입순번호관리") );
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호구분"), m_sClassType );
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호부터"), m_sClassNo );
		SetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("별치기호"), m_sShelfCode );

		GetDlgItem(IDC_cWORK_TYPE)->EnableWindow(false);
		GetDlgItem(IDC_bSELECT)->ShowWindow(SW_SHOW);
	}
	*/

	CString sWorkTbl, sClassCodeType, sShelfCode, sClassCodeStart, sClassCodeEnd;
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("별치기호"), sShelfCode );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호구분"), sClassCodeType );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호부터"), sClassCodeStart );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호까지"), sClassCodeEnd );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("작업구분"), sWorkTbl );

	CString sWhere;

	sClassCodeType.Replace( _T(" "), NULL );
	if ( sClassCodeType != _T("") ) {
		sWhere = _T("CLASS_NO_TYPE = '") + sClassCodeType + _T("'");
	}
	if ( sShelfCode != _T("") ) {
		if ( sWhere != _T("") ) {
			sWhere = sWhere + _T(" AND ");
		}
		sWhere += _T("SEPARATE_SHELF_CODE = '") + sShelfCode + _T("'");
	}
	if ( sClassCodeStart != _T("") ) {
		if ( sClassCodeEnd != _T("") ) {
			if ( sWhere != _T("") ) {
				sWhere = sWhere + _T(" AND ");
			}
			sWhere += _T("CLASS_NO >= '") + sClassCodeStart + _T("' AND CLASS_NO <= '") + sClassCodeEnd + _T("'");
		}
		else {
			if ( sWhere != _T("") ) {
				sWhere = sWhere + _T(" AND ");
			}
			sWhere += _T("CLASS_NO = '") + sClassCodeStart + _T("'");
		}
	}
	if ( sClassCodeEnd != _T("") && sClassCodeStart == _T("") ) {
		AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
		return;
		//sWhere += _T(" AND CLASS_NO <= '") + sClassCodeEnd + _T("'");
	}
	//sWhere.Format( _T("CLASS_NO_TYPE = '%s' AND CLASS_NO >= '%s' AND CLASS_NO <= '%s' AND SEPARATE_SHELF_CODE = '%s'"), sClassCodeType, sClassCodeStart, sClassCodeEnd, sShelfCode );
	
	CESL_DataMgr *pDM;
	if ( sWorkTbl == _T("마지막수입순번호관리") ) {
		pDM = FindDM( _T("DM_BO_CAT_ACQUISIT_NO") );
	}
	else if ( sWorkTbl == _T("누락수입순번호관리") ) {
		pDM = FindDM( _T("DM_BO_CAT_OMIT_NO") );
	}
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_WORD error") );
		return;
	}

	if (pDM->RefreshDataManager(sWhere) < 0)	return;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_ACQUISIT_NO"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_ACQUISIT_NO error") );
		return;
	}
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO::OnbINPUT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//CBO_CAT_ACQUISIT_NO_0100 dlg(this);
	CBO_CAT_NUMBER_0200 dlg(this);
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO::OnbUPDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWorkType;
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("작업구분"), sWorkType );
	//if ( sWorkType == _T("마지막수입순번호관리") ) {
		//CBO_CAT_ACQUISIT_NO_0200 dlg(this);
		CBO_CAT_NUMBER_0300 dlg(this);
		dlg.SetsWork(sWorkType);
		dlg.DoModal();
	//}

EFS_END
}

//VOID CBO_CAT_ACQUISIT_NO::OnbDELETE(CString m_sWork) 
VOID CBO_CAT_ACQUISIT_NO::OnbDELETE() 
{
	// TODO: Add your control notification handler code here
	CString sGridAlias;
	CString sWorkTbl;
	INT ids;
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO") , _T("작업구분"), sWorkTbl );
	if ( sWorkTbl == _T("마지막수입순번호관리") || m_sWork == _T("마지막수입순번호") ) {
		sGridAlias = _T("GRD_ACQUISIT_NO");
	}
	else {	
		sGridAlias = _T("GRD_ACQUISIT_NO2");
	}

	SelectMakeList(_T("CM_BO_CAT_ACQUISIT_NO"), sGridAlias );
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_ACQUISIT_NO"), sGridAlias );
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM;
		if ( sWorkTbl == _T("마지막수입순번호관리") || m_sWork == _T("마지막수입순번호") ) {
			pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));
		}
		else {
			pDM = FindDM(_T("DM_BO_CAT_OMIT_NO"));
		}

		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_ACQUISIT_NO") );
		}
		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return;
		}


		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

			if ( sWorkTbl == _T("마지막수입순번호관리") || m_sWork == _T("마지막수입순번호") ) {
				pDM->MakeDeleteFrame(_T("CO_LAST_BOOK_ACQUISITNO_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			}
			else {
				pDM->MakeDeleteFrame(_T("CO_OMIT_BOOK_ACQUISITNO_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			}
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_ACQUISIT_NO"), sGridAlias );			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		/*
		CString sQuery;
		//if ( sWorkTbl )
		sQuery.Format( _T("DELETE FROM CO_LAST_BOOK_ACQUISITNO_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
		*/

		ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return;
		}
		ids = pDM->EndFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("EndFrame 를 확인하세요"));
			return;
		}

	}	

	AllControlDisplay(_T("CM_BO_CAT_ACQUISIT_NO"));
/*
	if ( sWorkTbl == _T("마지막수입순번호관리") ) {
		return;
	}
	*/
/*
	SelectMakeList(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM;
		//if ( sWorkTbl )
		pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));

		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_ACQUISIT_NO") );
		}
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

			//if ( sWorkTbl )
			pDM->MakeDeleteFrame(_T("CO_LAST_BOOK_ACQUISITNO_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO"));			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		//if ( sWorkTbl )
		sQuery.Format( _T("DELETE FROM CO_LAST_BOOK_ACQUISITNO_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);

		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return;
		}
		pDM->EndFrame();
	}	

	AllControlDisplay(_T("CM_BO_CAT_ACQUISIT_NO"));
	*/
}

VOID CBO_CAT_ACQUISIT_NO::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	EndDialog(IDOK);
	//DestroyWindow();
	//CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO::OnSelchangecWORKTYPE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWorkType;
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("작업구분"), sWorkType );
	if ( sWorkType == _T("마지막수입순번호관리") ) {
		GetDlgItem(IDC_GRD_ACQUISIT_NO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRD_ACQUISIT_NO)->ShowWindow(SW_SHOW);
	}
	else {
		GetDlgItem(IDC_GRD_ACQUISIT_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRD_ACQUISIT_NO2)->ShowWindow(SW_SHOW);
	}

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO::SetData( INT iWorkCode, CString sShelfCode, CString sClassType, CString sClassNo, CString sAcqNo )
{
	m_iWorkCode = iWorkCode;
	m_sShelfCode = sShelfCode;
	m_sClassType = sClassType;
	m_sClassNo = sClassNo;
	m_sAcqNo = sAcqNo;
}

VOID CBO_CAT_ACQUISIT_NO::OnbOmitNumber() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	INT idx;
	SelectMakeList(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO2"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO2"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		//EndDialog(IDCANCEL);
		//m_bInitDlg = FALSE;
		return;		
	}

	CESL_DataMgr *pDM;	
	pDM = FindDM( _T("DM_BO_CAT_OMIT_NO") );	
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_WORD error") );
		return;
	}

	m_sOmitAcqNo = pDM->GetCellData( _T("수입순번호"), idx );
	CDialog::OnOK();

	EFS_END
}

HBRUSH CBO_CAT_ACQUISIT_NO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
