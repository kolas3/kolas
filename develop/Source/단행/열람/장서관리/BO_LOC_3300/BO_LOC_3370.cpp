// BO_LOC_3370.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3370.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3370 dialog


CBO_LOC_3370::CBO_LOC_3370(CESL_Mgr* pParent ,CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3370)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pParent = pParent;
}

CBO_LOC_3370::~CBO_LOC_3370()
{
	
}

VOID CBO_LOC_3370::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3370)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3370, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3370)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_chKCHANGE1, OnchKCHANGE1)
	ON_BN_CLICKED(IDC_chKCHANGE2, OnchKCHANGE2)
	ON_BN_CLICKED(IDC_chKCHANGE3, OnchKCHANGE3)
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_BN_CLICKED(IDC_chKCHANGE4, OnchKCHANGE4)
	ON_BN_CLICKED(IDC_chKCHANGE5, OnchKCHANGE5)
	ON_BN_CLICKED(IDC_chkAutoGrantSerialNo, OnchkAutoGrantSerialNo)	
	ON_BN_CLICKED(IDC_Check_UserStatus, OnCheckUserStatus)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3370 message handlers

VOID CBO_LOC_3370::OnClose() 
{
EFS_BEGIN

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3370"));
	pCM->AllControlClear();

	OnCancel();

EFS_END
}

BOOL CBO_LOC_3370::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3370")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_3370::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3370::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnClose();

	EFS_END
}

VOID CBO_LOC_3370::OnchKCHANGE1() 
{
	EFS_BEGIN

	ChangeControlEnable();	

	EFS_END
}

VOID CBO_LOC_3370::ChangeControlEnable()
{
	EFS_BEGIN

	const INT nCount = 7;
	INT nIDC[nCount][2] =
	{
		{ IDC_chKCHANGE1 , IDC_cmbCHANGE1 } , { IDC_chKCHANGE2 , IDC_cmbCHANGE2 } , 
		{ IDC_chKCHANGE3 , IDC_edtCHANGE } , {IDC_chKCHANGE4 , IDC_edtCHANGE2 },
		{ IDC_chKCHANGE5, IDC_edtCHANGE3 } , {IDC_chkAutoGrantSerialNo , IDC_edtAutoGrantSerialNo},
		{ IDC_Check_UserStatus, IDC_cmbUSER7 }
	};
	
	CString sCMAlias = _T("CM_BO_LOC_3370");
	TCHAR *sControlAlias[nCount] = { _T("소속체크") , _T("직급체크") , _T("연체수체크") , _T("대출정지일체크"), _T("누적연체일수체크"), _T("대출자번호부여체크"), _T("회원상태체크") };
	INT nCheckState;
	bool bFlag = FALSE;

	for( INT i = 0 ; i < nCount ; i++ )
	{
		if ( i < nCount )
		{
			nCheckState = ((CButton*)GetDlgItem( nIDC[i][0] ))->GetCheck();
			if( nCheckState == 0 )
			{
				(GetDlgItem( nIDC[i][1] ))->EnableWindow(FALSE);
				SetControlData(sCMAlias , sControlAlias[i] , _T("") );
			}
			else
			{
				(GetDlgItem( nIDC[i][1] ))->EnableWindow(TRUE);
				SetControlData(sCMAlias , sControlAlias[i] , _T("V") );
				bFlag = TRUE;
			}
		}		
	}

	GetDlgItem(IDC_btnCHANGE)->EnableWindow( bFlag );


	EFS_END
}

VOID CBO_LOC_3370::OnchKCHANGE2() 
{
	EFS_BEGIN

	ChangeControlEnable();	

	EFS_END
}

VOID CBO_LOC_3370::OnchKCHANGE3() 
{
	EFS_BEGIN

	ChangeControlEnable();	

	EFS_END
}

VOID CBO_LOC_3370::OnbtnCHANGE() 
{
	EFS_BEGIN

	BL_LOC_USER_PROC SpfWork(this , m_pLoanShareManager);
	SpfWork.SPFExcutePath( _T("이용자_일괄변경") );
	m_pParent->SendMessage(WM_USER + 4007);

	EFS_END
}

VOID CBO_LOC_3370::OnchKCHANGE4() 
{
	EFS_BEGIN

	ChangeControlEnable();	

	EFS_END
}

VOID CBO_LOC_3370::OnchKCHANGE5() 
{
	EFS_BEGIN

	ChangeControlEnable();	

	EFS_END
}

VOID CBO_LOC_3370::OnchkAutoGrantSerialNo() 
{
	EFS_BEGIN	

	ChangeControlEnable();	

	EFS_END	
}

HBRUSH CBO_LOC_3370::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

VOID CBO_LOC_3370::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		const int nCnt = 7;
		UINT nID[nCnt] = {
			IDC_chKCHANGE1 , IDC_chKCHANGE2 , 
			IDC_chKCHANGE3 , IDC_chKCHANGE4 ,
			IDC_chKCHANGE5 , IDC_chkAutoGrantSerialNo,
			IDC_Check_UserStatus
		};
		CButton* pBtn = NULL;
		for( int i=0; i<nCnt; i++ )
		{
			pBtn = (CButton*)GetDlgItem(nID[i]);
			if( pBtn->GetSafeHwnd() )
			{
				pBtn->SetCheck(0);
			}
		}
		ChangeControlEnable();
		

		ChangeControlEnable();
	}
}

void CBO_LOC_3370::OnCheckUserStatus() 
{
	EFS_BEGIN
		
		ChangeControlEnable();	
	
	EFS_END
}
