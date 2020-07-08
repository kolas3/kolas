// BO_CAT_WORD_0100.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_WORD_0100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_WORD_0100 dialog


CBO_CAT_WORD_0100::CBO_CAT_WORD_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_WORD_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_WORD_0100::~CBO_CAT_WORD_0100()
{
}


VOID CBO_CAT_WORD_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_WORD_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_WORD_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_WORD_0100)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_WORD_0100 message handlers

BOOL CBO_CAT_WORD_0100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_WORD_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_WORD_0100")) < 0)
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

VOID CBO_CAT_WORD_0100::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_MNG_WORD_0100"), _T("단어"), sWord );
	if ( sWord.IsEmpty() ){
		AfxMessageBox( _T("단어를 입력하세요.") );
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_MNG_WORD") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_WORD error") );
		return;
	}

	CString strWhere;
	strWhere = _T("AUX_WORD LIKE '") + sWord + _T("%'");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_MNG_WORD"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_MNG_WORD error") );
		return;
	}
	pCM->AllControlDisplay();

EFS_END
}


HBRUSH CBO_CAT_WORD_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}