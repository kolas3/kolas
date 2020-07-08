// BookCodeChange.cpp : implementation file
//

#include "stdafx.h"
#include "BookCodeChange.h"
#include "CodeChangeCondition.h"
#include "DataChangeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBookCodeChange dialog


CBookCodeChange::CBookCodeChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBookCodeChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBookCodeChange)
	m_bManageCode = TRUE;
	m_bMediaCode = TRUE;
	m_bRegCode = TRUE;
	m_bShelfCode = TRUE;
	m_bUseLimitCode = TRUE;
	m_bUseObjCode = TRUE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
}

CBookCodeChange::~CBookCodeChange()
{
}

VOID CBookCodeChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBookCodeChange)
	DDX_Check(pDX, IDC_chkMANAGE_CODE, m_bManageCode);
	DDX_Check(pDX, IDC_chkMEDIA_CODE, m_bMediaCode);
	DDX_Check(pDX, IDC_chkREG_CODE, m_bRegCode);
	DDX_Check(pDX, IDC_chkSE_SHELF_CODE, m_bShelfCode);
	DDX_Check(pDX, IDC_chkUSE_LIMIT_CODE, m_bUseLimitCode);
	DDX_Check(pDX, IDC_chkUSE_OBJ_CODE, m_bUseObjCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBookCodeChange, CDialog)
	//{{AFX_MSG_MAP(CBookCodeChange)
	ON_BN_CLICKED(IDC_chkMANAGE_CODE, OnchkMANAGECODE)
	ON_BN_CLICKED(IDC_chkMEDIA_CODE, OnchkMEDIACODE)
	ON_BN_CLICKED(IDC_chkREG_CODE, OnchkREGCODE)
	ON_BN_CLICKED(IDC_chkSE_SHELF_CODE, OnchkSESHELFCODE)
	ON_BN_CLICKED(IDC_chkUSE_LIMIT_CODE, OnchkUSELIMITCODE)
	ON_BN_CLICKED(IDC_chkUSE_OBJ_CODE, OnchkUSEOBJCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBookCodeChange message handlers

BOOL CBookCodeChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_책코드변경")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_책코드변경"));	
		
	OnchkMANAGECODE();
	OnchkMEDIACODE();
	OnchkREGCODE();
	OnchkSESHELFCODE();
	OnchkUSELIMITCODE();
	OnchkUSEOBJCODE();

	if(m_pDM)
	{
		INT nResult, nType;
		m_pDM->FindColumn(_T("시작등록번호"), nResult, nType);
		if(nResult < 0)
			m_nOpenPos = ACQ;
		else
			m_nOpenPos = REG;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBookCodeChange::OnchkMANAGECODE() 
{
	// TODO: Add your control notification handler code here
	m_bManageCode = !m_bManageCode;
	if(m_bManageCode)	GetDlgItem(IDC_cmbCC_MANAGE_CLASS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_cmbCC_MANAGE_CLASS)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CBookCodeChange::OnchkMEDIACODE() 
{
	// TODO: Add your control notification handler code here
	m_bMediaCode = !m_bMediaCode;
	if(m_bMediaCode)	GetDlgItem(IDC_cmbCC_MED_CLASS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_cmbCC_MED_CLASS)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CBookCodeChange::OnchkREGCODE() 
{
	// TODO: Add your control notification handler code here
	m_bRegCode = !m_bRegCode;
	if(m_bRegCode)	GetDlgItem(IDC_cmbCC_REG_CODE)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_cmbCC_REG_CODE)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CBookCodeChange::OnchkSESHELFCODE() 
{
	// TODO: Add your control notification handler code here
	m_bShelfCode = !m_bShelfCode;
	if(m_bShelfCode)	GetDlgItem(IDC_cmbCC_DIFFERENCE_CLASS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_cmbCC_DIFFERENCE_CLASS)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CBookCodeChange::OnchkUSELIMITCODE() 
{
	// TODO: Add your control notification handler code here
	m_bUseLimitCode = !m_bUseLimitCode;
	if(m_bUseLimitCode)	GetDlgItem(IDC_cmbCC_USE_LIMIT_CLASS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_cmbCC_USE_LIMIT_CLASS)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CBookCodeChange::OnchkUSEOBJCODE() 
{
	// TODO: Add your control notification handler code here
	m_bUseObjCode = !m_bUseObjCode;
	if(m_bUseObjCode)	GetDlgItem(IDC_cmbCC_USE_OBJ_CODE)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_cmbCC_USE_OBJ_CODE)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CBookCodeChange::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(FALSE);

	if(m_nOpenPos == REG && m_bRegCode)
	{
		ESLAfxMessageBox(_T("등록번호가 부여된 자료는 등록구분을 변경할 수 없습니다."));
	}
	
	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	CCodeChangeCondition cont;
	cont.SetManageCode(m_bManageCode);
	cont.SetMediaCode(m_bMediaCode);
	cont.SetRegCode(m_bRegCode);
	cont.SetShelfCode(m_bShelfCode);
	cont.SetUseLimitCode(m_bUseLimitCode);
	cont.SetUseObjCode(m_bUseObjCode);
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	INT ids = mgr.SPFExecutePath(_T("책코드변경"));
	if(ids < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정하신 자료의 코드를 변경할 수 없습니다."));
		CDialog::OnCancel();
	}	
	ESLAfxMessageBox(_T("선정하신 자료의 코드값을 변경하였습니다."));
	CDialog::OnOK();
}






















HBRUSH CBookCodeChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
