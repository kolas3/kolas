// BO_CAT_NUMBER_0100.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_NUMBER_0100.h"
#include "BO_CAT_NUMBER_0000.h"
#include "BO_CAT_ACQUISIT_NO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0100 dialog


CBO_CAT_NUMBER_0100::CBO_CAT_NUMBER_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_NUMBER_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_NUMBER_0100::~CBO_CAT_NUMBER_0100()
{
}

VOID CBO_CAT_NUMBER_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_NUMBER_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_NUMBER_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_NUMBER_0100)
	ON_BN_CLICKED(IDC_bNumber0100SEARCH, OnbNumber0100SEARCH)
	ON_CBN_SELCHANGE(IDC_cWORK_TYPE, OnSelchangecWORKTYPE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0100 message handlers

BOOL CBO_CAT_NUMBER_0100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}


BOOL CBO_CAT_NUMBER_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_NUMBER_0100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork = _T("마지막수입순번호");

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_NUMBER_0100::OnbNumber0100SEARCH()
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWorkTbl, sClassCodeType, sShelfCode, sClassCodeStart, sClassCodeEnd;
	GetControlData( _T("CM_BO_CAT_NUMBER_0100"), _T("별치기호"), sShelfCode );
	GetControlData( _T("CM_BO_CAT_NUMBER_0100"), _T("분류기호구분"), sClassCodeType );
	GetControlData( _T("CM_BO_CAT_NUMBER_0100"), _T("분류기호부터"), sClassCodeStart );
	GetControlData( _T("CM_BO_CAT_NUMBER_0100"), _T("분류기호까지"), sClassCodeEnd );
	GetControlData( _T("CM_BO_CAT_NUMBER_0100"), _T("작업구분"), sWorkTbl );

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
		if ( sWhere != _T("") ) {
			sWhere = sWhere + _T(" AND ");
		}
		sWhere += _T("CLASS_NO >= '") + sClassCodeStart + _T("'");
	}
	if ( sClassCodeEnd != _T("") ) {
		if ( sWhere != _T("") ) {
			sWhere = sWhere + _T(" AND ");
		}
		sWhere += _T("CLASS_NO <= '") + sClassCodeEnd + _T("'");
	}

	//======================================                    
	//2008.10.14 ADD BY PJW : 검색시 관리구분 조건추가        
	CString strManageCode ;                                   
	strManageCode = m_pInfo->MANAGE_CODE;                     
	if( !sWhere.IsEmpty() )                                   
	{                                                         
		sWhere += _T(" AND ");                                  
	}                                                         
	                                                          
	sWhere += _T(" MANAGE_CODE = '") + strManageCode +_T("' ");
	//======================================

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

	sWhere += _T("ORDER BY CLASS_NO");
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

VOID CBO_CAT_NUMBER_0100::OnSelchangecWORKTYPE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//CBO_CAT_NUMBER_0000 pDlg;	
	
	CString sWorkType;
	GetControlData( _T("CM_BO_CAT_NUMBER_0100"), _T("작업구분"), sWorkType );
	if ( sWorkType == _T("마지막수입순번호관리") ) {
		pParentMgr->GetDlgItem(IDC_GRD_ACQUISIT_NO2)->ShowWindow(SW_HIDE);
		pParentMgr->GetDlgItem(IDC_GRD_ACQUISIT_NO)->ShowWindow(SW_SHOW);
		((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork = _T("마지막수입순번호");
//		((CBO_CAT_ACQUISIT_NO*)pParentMgr)->m_sWork = _T("마지막수입순번호");
	}
	else {
		pParentMgr->GetDlgItem(IDC_GRD_ACQUISIT_NO)->ShowWindow(SW_HIDE);
		pParentMgr->GetDlgItem(IDC_GRD_ACQUISIT_NO2)->ShowWindow(SW_SHOW);
		((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork = _T("누락수입순번호");
//		((CBO_CAT_ACQUISIT_NO*)pParentMgr)->m_sWork = _T("누락수입순번호");
	}
	OnbNumber0100SEARCH();

EFS_END
}


HBRUSH CBO_CAT_NUMBER_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
