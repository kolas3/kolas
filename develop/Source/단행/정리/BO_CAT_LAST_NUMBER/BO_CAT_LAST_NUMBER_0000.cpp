// BO_CAT_LAST_NUMBER_0000.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_LAST_NUMBER_0000.h"
#include "BO_CAT_LAST_NUMBER_0100.h"
#include "BO_CAT_LAST_NUMBER_0200.h"

#include "BO_CAT_REG_NUMBER_0100.h"
#include "BO_CAT_REG_NUMBER_0200.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BO_CAT_LAST_NUMBER_0000 dialog


BO_CAT_LAST_NUMBER_0000::BO_CAT_LAST_NUMBER_0000(INT nType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(BO_CAT_LAST_NUMBER_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// CHILD
	if (nType == CHILD)
	{
		//m_lpszTemplateName = MAKEINTRESOURCE(IDD_POPUP);
		//m_nIDHelp = IDD_POPUP;

		//m_lpszTemplateName = MAKEINTRESOURCE(IDD);
		//m_nIDHelp = IDD;
		
		//child
		m_lpszTemplateName = MAKEINTRESOURCE(IDD_BO_CAT_LAST_NUMBER_0000_CHILD);
		m_nIDHelp = IDD_BO_CAT_LAST_NUMBER_0000_CHILD;	
	}
	// POPUP
	else if (nType == POPUP)
	{
		m_lpszTemplateName = MAKEINTRESOURCE(IDD);
		m_nIDHelp = IDD;		
		
		
	}
}

BO_CAT_LAST_NUMBER_0000::~BO_CAT_LAST_NUMBER_0000()
{
}

BOOL BO_CAT_LAST_NUMBER_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID BO_CAT_LAST_NUMBER_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BO_CAT_LAST_NUMBER_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(BO_CAT_LAST_NUMBER_0000, CDialog)
	//{{AFX_MSG_MAP(BO_CAT_LAST_NUMBER_0000)
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cTYPE, OnSelchangecTYPE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BO_CAT_LAST_NUMBER_0000 message handlers

BOOL BO_CAT_LAST_NUMBER_0000::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL BO_CAT_LAST_NUMBER_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_LAST_NUMBER")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID BO_CAT_LAST_NUMBER_0000::OnbFIND() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("번호종류"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_LAST_NUMBER") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_LAST_NUMBER error") );
		return;
	}

	CESL_DataMgr *pDM_REG;
	pDM_REG = FindDM( _T("DM_BO_CAT_REG_NUMBER") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_REG_NUMBER error") );
		return;
	}
	
	CString strWhere;
	if ( sWord == _T("(공통)등록원부번호") ) {
		//strWhere = _T("UNLESS_WORD LIKE '") + sWord + _T("%'");
		strWhere.Format( _T("KIND_CODE = '%s'"), _T("ACC_NO") );
		//GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_HIDE);
		if (pDM->RefreshDataManager(strWhere) < 0)	return;
		if ( pDM->GetRowCount() < 1 ) {
			AfxMessageBox (_T("검색된 결과가 없습니다."));
		}
		pDM->SORT( _T("년도"), FALSE );
	}
	else if ( sWord == _T("(공통)등록번호") ) {
		strWhere.Format( _T("KIND_CODE = '%s'"), _T("REG_NO") );
		//GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_SHOW);
		if (pDM_REG->RefreshDataManager(strWhere) < 0)	return;
		if ( pDM_REG->GetRowCount() < 1 ) {
			AfxMessageBox (_T("검색된 결과가 없습니다."));
		}
		pDM_REG->SORT( _T("구분값"), FALSE );
	}
	

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_LAST_NUMBER"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_LAST_NUMBER error") );
		return;
	}
	pCM->AllControlDisplay();	

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnbINPUT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("번호종류"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}

	if ( sWord == _T("(공통)등록원부번호") ) {
		CBO_CAT_LAST_NUMBER_0100 dlg(this);
		dlg.DoModal();
	}
	else {
		CBO_CAT_REG_NUMBER_0100 dlg(this);
		dlg.DoModal();
	}
	OnbFIND();

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnbUPDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("번호종류"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}

	if ( sWord == _T("(공통)등록원부번호") ) {
		CBO_CAT_LAST_NUMBER_0200 dlg(this);
		dlg.DoModal();
	}
	else {
		CBO_CAT_REG_NUMBER_0200 dlg(this);
		dlg.DoModal();
	}
	OnbFIND();

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnbDelete() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM;
	INT ids;
	pCM = FindCM( _T("CM_BO_CAT_LAST_NUMBER") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_LAST_NUMBER ERROR") );
		return;
	}

	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("번호종류"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}
	if ( sWord == _T("(공통)등록원부번호") ) {	
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		INT idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			return;
		}

		if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
		{
			CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER"));
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

				sAccRecKeyTmp += _T(",") + strCondition;		//키 조합

				pDM->MakeDeleteFrame(_T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
				pDM->DeleteRow(idx);

				idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));			
			}		
			while (idx > 0);
/*
			sAccRecKey = sAccRecKeyTmp.Mid(1);
			CString sQuery;
			sQuery.Format( _T("DELETE FROM CO_LAST_NUMBER_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
			INT iCh = pDM->AddFrame(sQuery);
*/
			ids = pDM->SendFrame();
			if (ids < 0) {
				AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
				return;
			}
			ids = pDM->EndFrame();
			if (ids < 0) {
				AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
				return;
			}
		}
	}
	else {
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		INT idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			return;
		}

		if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
		{
			CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_NUMBER"));
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

				sAccRecKeyTmp += _T(",") + strCondition;		//키 조합

				pDM->MakeDeleteFrame(_T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
				pDM->DeleteRow(idx);

				idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));			
			}		
			while (idx > 0);

			sAccRecKey = sAccRecKeyTmp.Mid(1);
			CString sQuery;
			sQuery.Format( _T("DELETE FROM CO_LAST_NUMBER_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
			INT iCh = pDM->AddFrame(sQuery);

			ids = pDM->SendFrame();
			if (ids < 0) {
				AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
				return;
			}
			ids = pDM->EndFrame();
			if (ids < 0) {
				AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
				return;
			}

		}
	}

	AllControlDisplay(_T("CM_BO_CAT_LAST_NUMBER"));

	return;

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnSelchangecTYPE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("번호종류"), sWord );

	if ( sWord == _T("(공통)등록원부번호") ) {
		GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_HIDE);
	}
	else if ( sWord == _T("(공통)등록번호") ) {
		GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_SHOW);
	}
	//다시 검색
	OnbFIND();

EFS_END
}

HBRUSH BO_CAT_LAST_NUMBER_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
