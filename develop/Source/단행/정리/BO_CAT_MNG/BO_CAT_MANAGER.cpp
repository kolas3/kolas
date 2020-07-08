// BO_CAT_MANAGER.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_MANAGER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_MANAGER dialog


CBO_CAT_MANAGER::CBO_CAT_MANAGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_CAT_MANAGER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_MANAGER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_MANAGER::~CBO_CAT_MANAGER()
{
}

BOOL CBO_CAT_MANAGER::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_MANAGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_MANAGER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_MANAGER, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_MANAGER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_MANAGER message handlers

BOOL CBO_CAT_MANAGER::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_MANAGER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sCheckYn;
	this->GetManageValue( _T("정리"), _T("단행"), _T("자동복본조사설정"), _T("유무"), sCheckYn );

	CButton *pCheckYes;
	pCheckYes = (CButton *)GetDlgItem(IDC_rSEPARATE);

	CButton *pCheckNo;
	pCheckNo = (CButton *)GetDlgItem(IDC_rINTERGRATE);

	if ( sCheckYn == _T("Y") ) {
		pCheckYes->SetCheck(1);
	}
	else if ( sCheckYn == _T("N") ) {
		pCheckNo->SetCheck(1);
	}

	//복본기호부여방식
	CButton *pBook;
	pBook = (CButton *)GetDlgItem(IDC_chCOPY_VOLCODE_YN);
	
	CString sDupSetting;
	this->GetManageValue( _T("정리"), _T("단행"), _T("복본기호부여방식"), _T("부여방식값"), sDupSetting );

	if ( sDupSetting == _T("권차별") ) {
		pBook->SetCheck(1);
	}
	else if ( sDupSetting == _T("등록구분별") ) {
	}

	//청구기호 중복조사 키설정
	CString sCallSetting;
	this->GetManageValue( _T("정리"), _T("단행"), _T("청구기호키설정"), _T("권책기호"), sCallSetting );
	CButton *pVolCode;
	pVolCode = (CButton *)GetDlgItem(IDC_chVOL_CODE);
	if ( sCallSetting == _T("Y") ) {
		pVolCode->SetCheck(1);
	}

	sCallSetting = _T("");
	this->GetManageValue( _T("정리"), _T("단행"), _T("청구기호키설정"), _T("별치기호"), sCallSetting );
	CButton *pShelfCode;
	pShelfCode = (CButton *)GetDlgItem(IDC_chSE_SHELF_CODE);
	if ( sCallSetting == _T("Y") ) {
		pShelfCode->SetCheck(1);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_MANAGER::OnOK() 
{
	// TODO: Add extra validation here
	CButton *pCheckYes;
	pCheckYes = (CButton *)GetDlgItem(IDC_rSEPARATE);

	CString sCheckYn;
	if ( pCheckYes->GetCheck() == 1 ) {
		sCheckYn = _T("Y");		
	}
	else {
		sCheckYn = _T("N");
	}
	this->SetManageValue( _T("정리"), _T("단행"), _T("자동복본조사설정"), _T("유무"), sCheckYn );

	//청구기호 중복조사 키설정
	CButton *pVolCode;
	pVolCode = (CButton *)GetDlgItem(IDC_chVOL_CODE);

	CString sCallSetting;
	if ( pVolCode->GetCheck() == 1 ) {
		sCallSetting = _T("Y");
	}
	else {
		sCallSetting = _T("N");
	}
	this->SetManageValue( _T("정리"), _T("단행"), _T("청구기호키설정"), _T("권책기호"), sCallSetting );
	

	CButton *pShelfCode;
	pShelfCode = (CButton *)GetDlgItem(IDC_chSE_SHELF_CODE);

	sCallSetting = _T("");
	if ( pShelfCode->GetCheck() == 1 ) {
		sCallSetting = _T("Y");
	}
	else {
		sCallSetting = _T("N");
	}
	this->SetManageValue( _T("정리"), _T("단행"), _T("청구기호키설정"), _T("별치기호"), sCallSetting );

	AfxMessageBox( _T("저장완료") );
/*
	sCallSetting = _T("");
	this->GetManageValue( _T("정리"), _T("단행"), _T("청구기호키설정"), _T("별치기호"), sCallSetting );
	CButton *pShelfCode;
	pShelfCode = (CButton *)GetDlgItem(IDC_chSE_SHELF_CODE);
	if ( sCallSetting == _T("Y") ) {
		pShelfCode->SetCheck(1);
	}
*/
	//CDialog::OnOK();
}

HBRUSH CBO_CAT_MANAGER::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
