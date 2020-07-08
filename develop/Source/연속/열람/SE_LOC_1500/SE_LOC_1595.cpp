// SE_LOC_1595.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1595.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1595 dialog


CSE_LOC_1595::CSE_LOC_1595(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1595)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1595::~CSE_LOC_1595()
{
}

VOID CSE_LOC_1595::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1595)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1595, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1595)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_radCODE2, OnradCODE2)
	ON_BN_CLICKED(IDC_radCODE3, OnradCODE3)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_CBN_SELCHANGE(IDC_cmbCLASS2, OnSelchangecmbCLASS2)
	ON_CBN_SELCHANGE(IDC_cmbCLASS3, OnSelchangecmbCLASS3)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1595 message handlers

BOOL CSE_LOC_1595::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1595::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();

	EFS_END
}

BOOL CSE_LOC_1595::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1595")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	((CButton*)GetDlgItem( IDC_radCODE2 ))->SetCheck(1);
	ChangeCodeMode( 0 );

	INT ids = SettingBookCnt();
	if( ids < 0 ) AfxMessageBox(_T("바보"));

	CString sBookCnt;
	ids = GetControlData( _T("CM_SE_LOC_1595") ,  _T("책수") , sBookCnt );
	if( ids < 0 ) return -1;
	if( _ttoi(sBookCnt)==0 )
	{
		AfxMessageBox(_T("자료를 선택해주십시오"));
		OnCancel();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1595::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}


VOID CSE_LOC_1595::OnradCODE2() 
{
	EFS_BEGIN

	ChangeCodeMode( 0 );	

	EFS_END
}

VOID CSE_LOC_1595::OnradCODE3() 
{
	EFS_BEGIN

	ChangeCodeMode( 1 );	

	EFS_END
}

VOID CSE_LOC_1595::ChangeCodeMode( INT nMode )
{
	EFS_BEGIN

	INT nComboIDC[2] = { IDC_cmbCLASS2 , IDC_cmbCLASS3 };
	for( INT i = 0 ; i < 2 ; i++ )
		GetDlgItem( nComboIDC[i] )->ShowWindow( SW_HIDE );

	GetDlgItem( nComboIDC[nMode] )->ShowWindow( SW_SHOW );
	CString sState;
	sState.Format( _T("%d") , nMode );
	GetDlgItem( IDC_CURRENT_STATE )->SetWindowText( sState );

	if( nMode == 0 )
	{
		GetDlgItem( IDC_chkSPECIES_CHANGE )->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem( IDC_chkSPECIES_CHANGE )->ShowWindow(SW_HIDE);
	}

		
	SetChangeButtonEnable();

	EFS_END
}


VOID CSE_LOC_1595::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	

	

	EFS_END
}

INT CSE_LOC_1595::SettingBookCnt()
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("메인그리드") ));
	if( pGrid == NULL ) return -1;
	
	CString sDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM ==NULL ) return -1;

	for( INT i = 0 ; i <  pDM->GetRowCount() ; i++ )
	{
		ids = SetDataMgrData( _T("DM_SE_LOC_1500") , _T("선정") , _T("") , i );
		if( ids < 0 ) return -1;
	}
	
	ids = pGrid->SelectMakeList();
	if( ids < 0 ) return -1;

	pGrid->SelectGetHeadPosition();
	for(  i = 0 ; i < pGrid->SelectGetCount() ;i++ )
	{
		INT nIndex = pGrid->SelectGetIdx();
		ids = SetDataMgrData( _T("DM_SE_LOC_1500") , _T("선정") , _T("V") , nIndex );
		if( ids < 0 ) return -1;
		pGrid->SelectGetNext();
	}


	CString sMainDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("종정보KEY") , pGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DisplayDefaultValue") ); 

	pGrid->SelectMakeList();

	sBookCnt.Format(_T("%d") , pGrid->SelectGetCount() );

	CString sClientCMAlias = _T("CM_SE_LOC_1595");
	ids = SetControlData( sClientCMAlias , _T("종수") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 

	ids = SetControlData( sClientCMAlias , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DisplayDefaultValue") ); 

	return 0;


	EFS_END
	return -1;

}

VOID CSE_LOC_1595::OnbtnSAVE() 
{
	EFS_BEGIN

	INT ids;
	
	CString sBookCnt;
	ids = GetControlData( _T("CM_SE_LOC_1595") , _T("책수") , sBookCnt );
	if( ids < 0 ) return;

	if( sBookCnt.Compare( _T("0") ) == 0 )
	{
		AfxMessageBox( _T("그리드에서 책을 먼저 선정해주십시오") );
		return ;
	}



	CBL_SE_LOC_15 SpfWork( this );

	CButton* pCheckButton = (CButton*)GetDlgItem(IDC_chkSPECIES_CHANGE);
	CButton* pUseLimitButton = (CButton*)GetDlgItem(IDC_radCODE2);

	if( pUseLimitButton->GetCheck() == 1 && ( pCheckButton->GetCheck() == 1 ) )
		ids = SpfWork.SPFExcutePath( ALL_SPECIES_CODE_CHANGE );
	else
		ids = SpfWork.SPFExcutePath( ALL_BOOK_CODE_CHANGE );
	
	EFS_END
}


VOID CSE_LOC_1595::OnSelchangecmbCLASS2() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END
}

VOID CSE_LOC_1595::OnSelchangecmbCLASS3() 
{
	EFS_BEGIN

	SetChangeButtonEnable();

	EFS_END
}

VOID CSE_LOC_1595::SetChangeButtonEnable()
{
	EFS_BEGIN

	INT nComboIDC[2] = { IDC_radCODE2 , IDC_radCODE3 };
	CString sControlAlias[2] = { _T("이용제한구분코드") , _T("관리구분코드") };

	CString sData;
	INT ids;

	for( INT i = 0 ; i < 2 ; i++ )
	{
		if( ((CButton*)(GetDlgItem( nComboIDC[i] )))->GetCheck() == 1 )
		{
			ids = GetControlData( _T("CM_SE_LOC_1595") , sControlAlias[i] , sData , -1 , 0 );
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

HBRUSH CSE_LOC_1595::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
