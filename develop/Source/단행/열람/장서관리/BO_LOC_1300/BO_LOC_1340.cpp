// BO_LOC_1340.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1340.h"

#include "..\\BL_LOC\\BL_BOSHELF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CODE_CHANGE_AGAIN	WM_USER + 827

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1340 dialog


CBO_LOC_1340::CBO_LOC_1340(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1340)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_1340::~CBO_LOC_1340()
{

}
	
VOID CBO_LOC_1340::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1340)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_1340, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1340)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radCODE1, OnradCODE1)
	ON_BN_CLICKED(IDC_radCODE2, OnradCODE2)
	ON_BN_CLICKED(IDC_radCODE3, OnradCODE3)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_CBN_SELCHANGE(IDC_cmbCLASS2, OnSelchangecmbCLASS2)
	ON_CBN_SELCHANGE(IDC_cmbCLASS3, OnSelchangecmbCLASS3)
	ON_BN_CLICKED(IDC_radCODE4, OnradCODE4)
	ON_CBN_SELCHANGE(IDC_cmbCLASS4, OnSelchangecmbCLASS4)
	ON_BN_CLICKED(IDC_radCODE5, OnradCODE5)
	ON_MESSAGE( CODE_CHANGE_AGAIN, ChandeCodeAgain )	
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1340 message handlers

BOOL CBO_LOC_1340::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_1340::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();

	EFS_END
}

BOOL CBO_LOC_1340::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_1340")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	((CButton*)GetDlgItem( IDC_radCODE1 ))->SetCheck(1);
	ChangeCodeMode( 0 );

	INT ids = SettingBookCnt();
	if( ids < 0 ) AfxMessageBox(_T("변경 책 수 설정 오류"));

	CString sBookCnt;
	ids = GetControlData( _T("CM_BO_LOC_1340") ,  _T("책수") , sBookCnt );
	if( ids < 0 ) return -1;
	if( _ttoi(sBookCnt)==0 )
	{
		AfxMessageBox(_T("자료를 선택해주십시오"));
		OnCancel();
	}

	((CButton*)GetDlgItem(IDC_chkDELETEBOOK2))->SetCheck(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_1340::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_1340::OnradCODE1() 
{
	EFS_BEGIN

	ChangeCodeMode( 0 );

	EFS_END
}

VOID CBO_LOC_1340::OnradCODE2() 
{
	EFS_BEGIN

	ChangeCodeMode( 1 );	

	EFS_END
}

VOID CBO_LOC_1340::OnradCODE3() 
{
	EFS_BEGIN

	ChangeCodeMode( 2 );	

	EFS_END
}

VOID CBO_LOC_1340::ChangeCodeMode( INT nMode )
{
	EFS_BEGIN

	INT nComboIDC[5] = { IDC_cmbCLASS1 , IDC_cmbCLASS2 , IDC_cmbCLASS3 , IDC_cmbCLASS4, IDC_cmbCLASS5};
	for( INT i = 0 ; i < 5 ; i++ )
		GetDlgItem( nComboIDC[i] )->ShowWindow( SW_HIDE );

	GetDlgItem( nComboIDC[nMode] )->ShowWindow( SW_SHOW );
	CString sState;
	sState.Format( _T("%d") , nMode );
	GetDlgItem( IDC_CURRENT_STATE )->SetWindowText( sState );

	if( nMode == 1 )
		GetDlgItem( IDC_chkDELETEBOOK )->ShowWindow( SW_SHOW );
	else
		GetDlgItem( IDC_chkDELETEBOOK )->ShowWindow( SW_HIDE );

	if( nMode == 1 )
		GetDlgItem( IDC_chkDELETEBOOK )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_chkDELETEBOOK )->EnableWindow( FALSE );

	SetChangeButtonEnable();

	EFS_END
}


VOID CBO_LOC_1340::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	EFS_END
}

INT CBO_LOC_1340::SettingBookCnt()
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_1300");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) return -1;
	
	CString sDMAlias = _T("DM_BO_LOC_1300");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM ==NULL ) return -1;

	for( INT i = 0 ; i <  pDM->GetRowCount() ; i++ )
	{
		ids = SetDataMgrData( _T("DM_BO_LOC_1300") , _T("선정") , _T("") , i );
		if( ids < 0 ) return -1;
	}
	
	ids = pGrid->SelectMakeList();
	if( ids < 0 ) return -1;

	pGrid->SelectGetHeadPosition();
	for(  i = 0 ; i < pGrid->SelectGetCount() ;i++ )
	{
		INT nIndex = pGrid->SelectGetIdx();
		ids = SetDataMgrData( _T("DM_BO_LOC_1300") , _T("선정") , _T("V") , nIndex );
		if( ids < 0 ) return -1;
		pGrid->SelectGetNext();
	}



	CString sBookCnt;
	sBookCnt.Format( _T("%d") , pGrid->SelectGetCount() );
	ids = SetControlData( _T("CM_BO_LOC_1340") ,  _T("책수") , sBookCnt );
	if( ids < 0 ) return -1;

	return 0;


	EFS_END
	return -1;

}

VOID CBO_LOC_1340::OnbtnSAVE() 
{
	EFS_BEGIN

	INT ids;
	
	CString sBookCnt;
	ids = GetControlData( _T("CM_BO_LOC_1340") , _T("책수") , sBookCnt );
	if( ids < 0 ) return;

	if( sBookCnt.Compare( _T("0") ) == 0 )
	{
		AfxMessageBox( _T("그리드에서 책을 먼저 선정해주십시오") );
		return ;
	}

	CBL_BOSHELF SpfWork( this );

	if( ((CButton*)GetDlgItem(IDC_chkDELETEBOOK))->GetCheck() == 1 )
	{
		ids = SpfWork.SetTempData( _T("제적자료변경허용") , CComVariant(CString(_T("Y"))) );
	}
	else
	{
		ids = SpfWork.SetTempData( _T("제적자료변경허용") , CComVariant(CString(_T("N"))) );
	}

	if( ((CButton*)GetDlgItem(IDC_chkDELETEBOOK2))->GetCheck() == 1 )
	{
		ids = SpfWork.SetTempData( _T("센터변경") , CComVariant(CString(_T("Y"))) );
	}
	else
	{
		ids = SpfWork.SetTempData( _T("센터변경") , CComVariant(CString(_T("N"))) );
	}

	if( ((CButton*)GetDlgItem(IDC_chkAppendix))->GetCheck() == 1 )
	{
		ids = SpfWork.SetTempData( _T("부록변경") , CComVariant(CString(_T("Y"))) );
	}
	else
	{
		ids = SpfWork.SetTempData( _T("부록변경") , CComVariant(CString(_T("N"))) );
	}

	ids = SpfWork.SPFExcutePath( _T("배가자료관리_코드일괄변경") );
	
	EFS_END
}


VOID CBO_LOC_1340::OnSelchangecmbCLASS2() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END
}

VOID CBO_LOC_1340::OnSelchangecmbCLASS3() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END
}

VOID CBO_LOC_1340::SetChangeButtonEnable()
{
	EFS_BEGIN

	INT nComboIDC[5] = { IDC_radCODE1 , IDC_radCODE2 , IDC_radCODE3 , IDC_radCODE4, IDC_radCODE5 };
	CString sControlAlias[5] = { _T("배가상태코드") , _T("이용제한구분코드") , _T("관리구분코드") , _T("수입구분코드"), _T("이용대상구분코드")};

	CString sData;
	INT ids;

	for( INT i = 0 ; i < 5 ; i++ )
	{
		if( ((CButton*)(GetDlgItem( nComboIDC[i] )))->GetCheck() == 1 )
		{
			ids = GetControlData( _T("CM_BO_LOC_1340") , sControlAlias[i] , sData , -1 , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SetChangeButtonEnable") );
			sData.TrimLeft(); sData.TrimRight();

			if( sData.IsEmpty() )
			{
				GetDlgItem(IDC_btnSAVE)->EnableWindow(FALSE);	
			}
			else
			{
				GetDlgItem(IDC_btnSAVE)->EnableWindow(TRUE);	
			}
		}
	}

	return ;

	EFS_END
}

INT CBO_LOC_1340::CheckChangeCode()
{
	EFS_BEGIN

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_radCODE1 );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbCLASS1 );

	CString code;
	pCombo->GetWindowText( code );
	code.Replace( _T(" "), _T("") );
	if( pBtn->GetCheck() && _T("제적자료") == code )
		return 1;
	
	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_1340::ChandeCodeAgain()
{
	EFS_BEGIN

	CButton *pBtn;
	pBtn = ( CButton * )GetDlgItem( IDC_radCODE1 );
	pBtn->SetCheck( false );
	pBtn = ( CButton * )GetDlgItem( IDC_radCODE2 );
	pBtn->SetCheck( true );
	ChangeCodeMode( 1 );

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_1340") );
	pCM->SetControlMgrData( _T("이용제한구분코드"), _T("CL") );
	
	OnbtnSAVE();

	EFS_END
}

VOID CBO_LOC_1340::OnradCODE4() 
{
	EFS_BEGIN
		
	ChangeCodeMode( 3 );	
	
	EFS_END
}

VOID CBO_LOC_1340::OnSelchangecmbCLASS4() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END	
}

VOID CBO_LOC_1340::OnradCODE5() 
{
	EFS_BEGIN

	ChangeCodeMode( 4 );	

	EFS_END	
}

HBRUSH CBO_LOC_1340::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
