// CatCodeChange.cpp : implementation file
//

#include "stdafx.h"
#include "CatCodeChange.h"
#include "DataChangeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatCodeChange dialog


CCatCodeChange::CCatCodeChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCatCodeChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatCodeChange)
	m_bMediaCode = FALSE;
	m_bSubRegCode = FALSE;
	m_bUseObjCode = FALSE;
	m_bManageCode = FALSE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_bSubjectCodeEnable = TRUE;
	m_bDeleteData = FALSE;
}
CCatCodeChange::~CCatCodeChange()
{
}

VOID CCatCodeChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatCodeChange)
	DDX_Check(pDX, IDC_chkMEDIA_CODE, m_bMediaCode);
	DDX_Check(pDX, IDC_chkSUB_REG_CODE, m_bSubRegCode);
	DDX_Check(pDX, IDC_chkUSE_OBJ_CODE, m_bUseObjCode);
	DDX_Check(pDX, IDC_chkMANAGER_CODE, m_bManageCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatCodeChange, CDialog)
	//{{AFX_MSG_MAP(CCatCodeChange)
	ON_BN_CLICKED(IDC_chkSUB_REG_CODE, OnchkSUBREGCODE)
	ON_BN_CLICKED(IDC_chkUSE_OBJ_CODE, OnchkUSEOBJCODE)
	ON_BN_CLICKED(IDC_chkMEDIA_CODE, OnchkMEDIACODE)
	ON_BN_CLICKED(IDC_chkMANAGER_CODE, OnchkMANAGERCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatCodeChange message handlers

BOOL CCatCodeChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_정리코드변경")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_정리코드변경"));	
	
	OnchkMEDIACODE();
	OnchkMANAGERCODE();
	OnchkSUBREGCODE();
	OnchkUSEOBJCODE();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCatCodeChange::OnchkSUBREGCODE() 
{
	// TODO: Add your control notification handler code here
	m_bSubRegCode = !m_bSubRegCode;
	if(m_bSubRegCode) GetDlgItem(IDC_cmbCC_SUPPORT_REG_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_SUPPORT_REG_CLASS)->EnableWindow(FALSE);	
	UpdateData(FALSE);	
}

VOID CCatCodeChange::OnchkUSEOBJCODE() 
{
	// TODO: Add your control notification handler code here
	m_bUseObjCode = !m_bUseObjCode;
	if(m_bUseObjCode) GetDlgItem(IDC_cmbCC_USE_OBJ_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_USE_OBJ_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);	
}

VOID CCatCodeChange::OnchkMEDIACODE() 
{
	// TODO: Add your control notification handler code here
	m_bMediaCode = !m_bMediaCode;
	if(m_bMediaCode) GetDlgItem(IDC_cmbCC_MEDIA_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_MEDIA_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);	
}

VOID CCatCodeChange::OnchkMANAGERCODE() 
{
	// TODO: Add your control notification handler code here
	m_bManageCode = !m_bManageCode;
	if(m_bManageCode) GetDlgItem(IDC_cmbCC_MANAGE_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_MANAGE_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);	
}

VOID CCatCodeChange::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(FALSE);
	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	CCodeChangeCondition cont;
	cont.SetManageCode(m_bManageCode);
	cont.SetMediaCode(m_bMediaCode);
	cont.SetSubRegCode(m_bSubRegCode);
	cont.SetUseObjCode(m_bUseObjCode);
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	mgr.SetTempData(_T("AcqCode"), 0);
	mgr.SetTempData(_T("SPECIES_SUBJECT_CODE"), m_bSubjectCodeEnable);
	mgr.SetTempData(_T("IS_DELETE_DATA"), m_bDeleteData);
	mgr.SetTempData(_T("IS_CAT_CODE"), TRUE);
	// 2005.11.1 ADD BY PDJ
	// default value가 없어서 오류가 났었음.
	mgr.SetTempData(_T("SPECIES_YN"), TRUE);
	INT ids = mgr.SPFExecutePath(_T("코드변경"));
	if(ids < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정하신 자료의 코드를 변경할 수 없습니다."));
		CDialog::OnCancel();
	}
	ESLAfxMessageBox(_T("선정하신 자료의 코드값을 변경하였습니다."));
	CDialog::OnOK();
}

HBRUSH CCatCodeChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
