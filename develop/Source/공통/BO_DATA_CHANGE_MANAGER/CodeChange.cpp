// CodeChange.cpp : implementation file
//

#include "stdafx.h"
#include "CodeChange.h"
#include "DataChangeManager.h"
#include "CodeChangeCondition.h"
#include "../K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeChange dialog
// 생성자
CCodeChange::CCodeChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCodeChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeChange)
	m_bClassNoType = TRUE;
	m_bControlCode = TRUE;
	m_bManageCode = TRUE;
	m_bMatCode = TRUE;
	m_bMediaCode = TRUE;
	m_bRegCode = TRUE;
	m_bShelfCode = TRUE;
	m_bSubRegCode = TRUE;
	m_bSubjectCode = TRUE;
	m_bUseLimitCode = TRUE;
	m_bUseObjCode = TRUE;
	m_bWorkCode = TRUE;
	m_bUserDefineCode1 = TRUE;
	m_bUserDefineCode2 = TRUE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_nAcqType = 0;
	m_bSubjectCodeEnable = TRUE;
	m_bDeleteData = FALSE;
	m_bSpeciesYN = TRUE;
}

CCodeChange::~CCodeChange()
{
}

VOID CCodeChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeChange)
	DDX_Check(pDX, IDC_chkCLASS_NO_TYPE, m_bClassNoType);
	DDX_Check(pDX, IDC_chkCONTROL_CODE, m_bControlCode);
	DDX_Check(pDX, IDC_chkMANAGER_CODE, m_bManageCode);
	DDX_Check(pDX, IDC_chkMAT_CODE, m_bMatCode);
	DDX_Check(pDX, IDC_chkMEDIA_CODE, m_bMediaCode);
	DDX_Check(pDX, IDC_chkREG_CODE, m_bRegCode);
	DDX_Check(pDX, IDC_chkSE_SHELF_CODE, m_bShelfCode);
	DDX_Check(pDX, IDC_chkSUB_REG_CODE, m_bSubRegCode);
	DDX_Check(pDX, IDC_chkSUBJECT_CODE, m_bSubjectCode);
	DDX_Check(pDX, IDC_chkUSE_LIMIT_CODE, m_bUseLimitCode);
	DDX_Check(pDX, IDC_chkUSE_OBJ_CODE, m_bUseObjCode);
	DDX_Check(pDX, IDC_chkWORK_CODE, m_bWorkCode);
	DDX_Check(pDX, IDC_chkUSERDEFINE_CODE1, m_bUserDefineCode1);
	DDX_Check(pDX, IDC_chkUSERDEFINE_CODE2, m_bUserDefineCode2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCodeChange, CDialog)
	//{{AFX_MSG_MAP(CCodeChange)
	ON_BN_CLICKED(IDC_chkCLASS_NO_TYPE, OnchkCLASSNOTYPE)
	ON_BN_CLICKED(IDC_chkCONTROL_CODE, OnchkCONTROLCODE)
	ON_BN_CLICKED(IDC_chkMANAGER_CODE, OnchkMANAGERCODE)
	ON_BN_CLICKED(IDC_chkMAT_CODE, OnchkMATCODE)
	ON_BN_CLICKED(IDC_chkMEDIA_CODE, OnchkMEDIACODE)
	ON_BN_CLICKED(IDC_chkREG_CODE, OnchkREGCODE)
	ON_BN_CLICKED(IDC_chkSE_SHELF_CODE, OnchkSESHELFCODE)
	ON_BN_CLICKED(IDC_chkSUB_REG_CODE, OnchkSUBREGCODE)
	ON_BN_CLICKED(IDC_chkSUBJECT_CODE, OnchkSUBJECTCODE)
	ON_BN_CLICKED(IDC_chkUSE_LIMIT_CODE, OnchkUSELIMITCODE)
	ON_BN_CLICKED(IDC_chkUSE_OBJ_CODE, OnchkUSEOBJCODE)
	ON_BN_CLICKED(IDC_chkWORK_CODE, OnchkWORKCODE)
	ON_CBN_SELCHANGE(IDC_cmbCC_CLASS_TBL, OnSelchangecmbCCCLASSTBL)
	ON_BN_CLICKED(IDC_chkUSERDEFINE_CODE1, OnchkUSERDEFINECODE1)
	ON_BN_CLICKED(IDC_chkUSERDEFINE_CODE2, OnchkUSERDEFINECODE2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeChange message handlers
// 초기화
BOOL CCodeChange::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	// SM 설정
	if (InitESL_Mgr(_T("K2UPGRADE_코드변경")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_코드변경"));	
	
	OnchkMEDIACODE();
	OnchkCLASSNOTYPE();
	OnchkCONTROLCODE();
	OnchkMANAGERCODE();
	OnchkMATCODE();
	OnchkREGCODE();
	OnchkSUBJECTCODE();
	OnchkSESHELFCODE();
	OnchkSUBREGCODE();
	OnchkUSELIMITCODE();
	OnchkUSEOBJCODE();
	OnchkWORKCODE();
	OnchkUSERDEFINECODE1();  // 2005-01-27
	OnchkUSERDEFINECODE2();  // 2005-01-27
	SetSubjectCodeData();
	ShowCtrlByAcqCode();
	ShowCtrlByUserDefine();  // 2005-01-27

/*	if(m_bSubjectCodeEnable)
	{
		GetDlgItem(IDC_chkSUBJECT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbCC_SUBJECT)->ShowWindow(SW_SHOW);
	}
	else 
	{
		GetDlgItem(IDC_chkSUBJECT_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCC_SUBJECT)->ShowWindow(SW_HIDE);
	}
*/
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - CM_코드변경 : 분류표구분
/////////////////////////////////////////////////////////////////////////////
INT CCodeChange::SetSubjectCodeData()
{
	INT ids;
 	CMulitComboBoxManager mgr(this);

	ids = mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("BS_분류표구분"), _T("BP_주제구분"), _T(""));
	if(ids < 0) return -1;

	return 0;
}

VOID CCodeChange::OnchkCLASSNOTYPE() 
{
	// TODO: Add your control notification handler code here
	m_bClassNoType = !m_bClassNoType;
	if(m_bClassNoType) GetDlgItem(IDC_cmbCC_CLASS_TBL)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_CLASS_TBL)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkCONTROLCODE() 
{
	// TODO: Add your control notification handler code here
	m_bControlCode = !m_bControlCode;
	if(m_bControlCode) GetDlgItem(IDC_cmbCC_CONTROL_MAT_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_CONTROL_MAT_CLASS)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}
VOID CCodeChange::OnchkMANAGERCODE() 
{
	// TODO: Add your control notification handler code here
	m_bManageCode = !m_bManageCode;
	if(m_bManageCode) GetDlgItem(IDC_cmbCC_MANAGE_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_MANAGE_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkMATCODE() 
{
	m_bMatCode = !m_bMatCode;
	if(m_bMatCode) GetDlgItem(IDC_cmbCC_MAT_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_MAT_CLASS)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkMEDIACODE() 
{
	m_bMediaCode = !m_bMediaCode;
	if( m_bMediaCode == TRUE )		GetDlgItem(IDC_cmbCC_MEDIA_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_MEDIA_CODE)->EnableWindow(FALSE);	

	UpdateData(FALSE);
}

VOID CCodeChange::OnchkREGCODE() 
{
	// TODO: Add your control notification handler code here
	m_bRegCode = !m_bRegCode;
	if(m_bRegCode) GetDlgItem(IDC_cmbCC_REG_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_REG_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkSESHELFCODE() 
{
	// TODO: Add your control notification handler code here
	m_bShelfCode = !m_bShelfCode;
	if(m_bShelfCode) GetDlgItem(IDC_cmbCC_DIFFERENCE_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_DIFFERENCE_CLASS)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkSUBREGCODE() 
{
	// TODO: Add your control notification handler code here
	m_bSubRegCode = !m_bSubRegCode;
	if(m_bSubRegCode) GetDlgItem(IDC_cmbCC_SUPPORT_REG_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_SUPPORT_REG_CLASS)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkSUBJECTCODE() 
{
	// TODO: Add your control notification handler code here
	m_bSubjectCode = !m_bSubjectCode;
	if(m_bSubjectCode) GetDlgItem(IDC_cmbCC_SUBJECT)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_SUBJECT)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkUSELIMITCODE() 
{
	// TODO: Add your control notification handler code here
	m_bUseLimitCode = !m_bUseLimitCode;
	if(m_bUseLimitCode) GetDlgItem(IDC_cmbCC_USE_LIMIT_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_USE_LIMIT_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkUSEOBJCODE() 
{
	// TODO: Add your control notification handler code here
	m_bUseObjCode = !m_bUseObjCode;
	if(m_bUseObjCode) GetDlgItem(IDC_cmbCC_USE_OBJ_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_USE_OBJ_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkWORKCODE() 
{
	m_bWorkCode = !m_bWorkCode;
	if(m_bWorkCode) GetDlgItem(IDC_cmbCC_WORK_CODE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbCC_WORK_CODE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CCodeChange::OnchkUSERDEFINECODE1() 
{
	// TODO: Add your control notification handler code here
	m_bUserDefineCode1 = !m_bUserDefineCode1;
	if(m_bUserDefineCode1) GetDlgItem(IDC_cmbUSERDEFINE_CODE1)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbUSERDEFINE_CODE1)->EnableWindow(FALSE);	
	UpdateData(FALSE);	
}

VOID CCodeChange::OnchkUSERDEFINECODE2() 
{
	// TODO: Add your control notification handler code here
	m_bUserDefineCode2 = !m_bUserDefineCode2;
	if(m_bUserDefineCode2) GetDlgItem(IDC_cmbUSERDEFINE_CODE2)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbUSERDEFINE_CODE2)->EnableWindow(FALSE);	
	UpdateData(FALSE);	
}

VOID CCodeChange::OnOK() 
{
	UpdateData(FALSE);

	// 2005-02-17 등록번호가 없는 경우에만 등록구분이 변경된다는 메시지를 보인다.
	if( m_nAcqType == 0 && m_bSpeciesYN == FALSE && m_bRegCode == TRUE )
	{
		ESLAfxMessageBox(_T("등록번호가 이미 부여된 자료에 대해서는 등록구분이 변경되지 않습니다."));
	}
	
	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	CCodeChangeCondition cont;
	cont.SetClassNoType(m_bClassNoType);
	cont.SetControlCode(m_bControlCode);
	cont.SetManageCode(m_bManageCode);
	cont.SetMatCode(m_bMatCode);
	cont.SetMediaCode(m_bMediaCode);
	cont.SetRegCode(m_bRegCode);
	cont.SetShelfCode(m_bShelfCode);
	cont.SetSubjectCode(m_bSubjectCode);
	cont.SetSubRegCode(m_bSubRegCode);
	cont.SetUseLimitCode(m_bUseLimitCode);
	cont.SetUseObjCode(m_bUseObjCode);
	cont.SetWorkCode(m_bWorkCode);
	cont.SetUserDefineCode1(m_bUserDefineCode1);  // 2005-01-27
	cont.SetUserDefineCode2(m_bUserDefineCode2);  // 2005-01-27
	
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	mgr.SetTempData(_T("AcqCode"), m_nAcqType);
	mgr.SetTempData(_T("SPECIES_SUBJECT_CODE"), m_bSubjectCodeEnable);
	mgr.SetTempData(_T("IS_DELETE_DATA"), m_bDeleteData);
	mgr.SetTempData(_T("SPECIES_YN"), m_bSpeciesYN);
	mgr.SetTempData(_T("IS_CAT_CODE"), FALSE);
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

VOID CCodeChange::OnSelchangecmbCCCLASSTBL() 
{
	SetSubjectCodeData();
}

VOID CCodeChange::ShowCtrlByAcqCode()
{
	if(m_nAcqType == 1)
	{
		GetDlgItem(IDC_chkWORK_CODE)->EnableWindow(FALSE);
	}
}

// 2005-01-27
// 5163의 경우 사용자정의코드 보여준다.
VOID CCodeChange::ShowCtrlByUserDefine()
{
	BOOL bShow1 = FALSE;  // 사용자정의코드1을 보여줄지 말지 결정하는 플래그
	BOOL bShow2 = FALSE;  // 사용자정의코드2을 보여줄지 말지 결정하는 플래그
//	CString strIs5163;

//	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T(""), strIs5163);

//	if( _T("Y") == strIs5163 || _T("Y") )
//	{
		CESL_DataMgr * pTmpDM;
		pTmpDM = FindDM(_T("DM_복본기호갱신용DM"));  // 임의로 DM을 쓰기 위해 SM에 있는 DM을 가져다 썼다.
		
		CString strUseCodeYN;
		pTmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'"), strUseCodeYN);
		INT nUseCodeCnt = 0;
		if(strUseCodeYN == _T("Y"))
		{
			CString strYN, strName1, strName2;
			pTmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
			if( strYN == _T("Y")) 
			{
				m_pDM->GetOneValueQuery(_T("select class_alias from MN_USER_DEFINE_TBL where class_code = 91"), strName1);
				GetDlgItem(IDC_chkUSERDEFINE_CODE1)->SetWindowText(	strName1 );
				bShow1 = TRUE;
			}
			pTmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
			if( strYN == _T("Y")) 
			{
				m_pDM->GetOneValueQuery(_T("select class_alias from MN_USER_DEFINE_TBL where class_code = 92"), strName2);
				GetDlgItem(IDC_chkUSERDEFINE_CODE2)->SetWindowText(	strName2 );
				bShow2 = TRUE;
			}
		}
//	}

	if( bShow1 == TRUE )
	{
		GetDlgItem(IDC_chkUSERDEFINE_CODE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbUSERDEFINE_CODE1)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_chkUSERDEFINE_CODE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbUSERDEFINE_CODE1)->ShowWindow(SW_HIDE);
	}

	if(bShow2 == TRUE )
	{
		GetDlgItem(IDC_chkUSERDEFINE_CODE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbUSERDEFINE_CODE2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_chkUSERDEFINE_CODE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbUSERDEFINE_CODE2)->ShowWindow(SW_HIDE);
	}
}


HBRUSH CCodeChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
