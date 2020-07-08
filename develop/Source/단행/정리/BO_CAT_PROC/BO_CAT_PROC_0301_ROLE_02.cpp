// BO_CAT_PROC_0301_ROLE_02.cpp : implementation file
//

#include "stdafx.h"

#include "BO_CAT_PROC_0301_ROLE_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301_ROLE_02 dialog


CBO_CAT_PROC_0301_ROLE_02::CBO_CAT_PROC_0301_ROLE_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0301_ROLE_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0301_ROLE_02::~CBO_CAT_PROC_0301_ROLE_02()
{
}

VOID CBO_CAT_PROC_0301_ROLE_02::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0301_ROLE_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0301_ROLE_02, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0301_ROLE_02)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCatRolePREV, OnbCatRolePREV)
	ON_BN_CLICKED(IDC_bCatRoleNEXT, OnbCatRoleNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301_ROLE_02 message handlers

BOOL CBO_CAT_PROC_0301_ROLE_02::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0301_ROLE_02::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_ROLE_0300")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	Display(0);		

	pDM = FindDM(_T("DM_BO_CAT_MNG_ROLE"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_MNG_ROLE ERROR") );
		m_bInitDlg = FALSE;
		return FALSE;
	}
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("역할어"), m_idx );
	SetControlData( _T("CM_BO_CAT_MNG_ROLE_0300"), _T("역할어"), sWord );

	if ( sRecKey.IsEmpty() ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0301_ROLE_02::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_MNG_ROLE_0300"), m_idx);
	SetReverse(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"), m_idx);

EFS_END
}


VOID CBO_CAT_PROC_0301_ROLE_02::OnbCatRolePREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);		
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

EFS_END
}

VOID CBO_CAT_PROC_0301_ROLE_02::OnbCatRoleNEXT() 
{
EFS_BEGIN

	Display(1);
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

EFS_END
}

VOID CBO_CAT_PROC_0301_ROLE_02::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_ROLE"));	
	if (!pDM) return;
	
	CString strID;

	strID = pDM->GetCellData ( _T("REC_KEY") ,m_idx );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_MNG_ROLE_0300"));
	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, m_idx);

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
  
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_MNG_ROLE_POPUP"));
	AllControlDisplay(_T("DM_BO_CAT_MNG_ROLE"));	
	pCM1->AllControlDisplay(m_idx);

	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_PROC_0301_ROLE_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
