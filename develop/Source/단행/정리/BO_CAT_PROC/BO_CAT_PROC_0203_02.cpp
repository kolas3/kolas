// BO_CAT_PROC_0203_02.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0203_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203_02 dialog


CBO_CAT_PROC_0203_02::CBO_CAT_PROC_0203_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0203_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0203_02::~CBO_CAT_PROC_0203_02()
{
}

VOID CBO_CAT_PROC_0203_02::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0203_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0203_02, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0203_02)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203_02 message handlers

BOOL CBO_CAT_PROC_0203_02::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton * pPrint01;
	pPrint01=(CButton *)GetDlgItem(IDC_rLINE_PRINT);
	pPrint01->SetCheck(1);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0203_02::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	INT ids = SelectMakeList(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList error") );
	}
	INT idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0203"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0203 error") );
		return;
	}
	CString sLabelType = pDM->GetCellData( _T("BB_레이블출력구분"), idx );

	CButton * pPrint01;
	pPrint01=(CButton *)GetDlgItem(IDC_rLINE_PRINT);

	if ( pPrint01->GetCheck() == 1 ) {
		pDM->SetCellData( _T("BB_레이블출력구분"), _T("줄바꿔찍기"), idx );
	}
	else {
		pDM->SetCellData( _T("BB_레이블출력구분"), _T("이어찍기"), idx );
	}

	idx = SelectGetNext(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	while ( idx != -1 ) {
		if ( pPrint01->GetCheck() == 1 ) {
			pDM->SetCellData( _T("BB_레이블출력구분"), _T("줄바꿔찍기"), idx );
		}
		else {
			pDM->SetCellData( _T("BB_레이블출력구분"), _T("이어찍기"), idx );
		}
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0203"));
	pCM->AllControlDisplay();

	CDialog::OnOK();

EFS_END
}
HBRUSH CBO_CAT_PROC_0203_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
