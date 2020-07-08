// BO_LOC_3260.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3260.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3260 dialog


CBO_LOC_3260::CBO_LOC_3260(CESL_Mgr* pParent,CLonaManageValue *pLona, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{

	//{{AFX_DATA_INIT(CBO_LOC_3260)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_sParentCMAlias = _T("CM_BO_LOC_3200");
	m_sParentDMAlias = _T("DM_BO_LOC_3200");
	m_sGridAlias = _T("MainGrid");
	

	m_pParentDM = FindDM( m_sParentDMAlias );
	if( m_pParentDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3260()") );

	m_pSpfWork = NULL;
	m_pSpfWork = new CBL_LOC_LOAN_MNG_PROC(this,pLona,m_pLoanShareManager);

	m_hLoan = NULL;
	m_hReturnPlan = NULL;
	m_hReturn = NULL;
	m_hReservation = NULL;
	m_hReservationEcpire = NULL;

	m_bEnableExpireDay	=	TRUE;
}

CBO_LOC_3260::~CBO_LOC_3260()
{
	if( m_pSpfWork )
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}

	if( m_hLoan )
	{
		delete m_hLoan;
		m_hLoan = NULL;
	}

	if( m_hReturnPlan )
	{
		delete m_hReturnPlan;
		m_hReturnPlan = NULL;
	}

	if( m_hReturn )
	{
		delete m_hReturn;
		m_hReturn = NULL;
	}

	if( m_hReservation )
	{
		delete m_hReservation;
		m_hReservation = NULL;
	}

	if( m_hReservationEcpire )
	{
		delete m_hReservationEcpire;
		m_hReservationEcpire = NULL;
	}
}

VOID CBO_LOC_3260::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3260)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3260, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3260)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnMODIFY, OnbtnMODIFY)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3260 message handlers

BOOL CBO_LOC_3260::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3260")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_hLoan = new CHourEditControl;
	m_hLoan->SubclassWindow( GetDlgItem(IDC_edtLoanHour)->GetSafeHwnd() );
	m_hLoan->InitEdit();

	m_hReturnPlan = new CHourEditControl;
	m_hReturnPlan->SubclassWindow( GetDlgItem(IDC_edtReturnPlanHour)->GetSafeHwnd() );
	m_hReturnPlan->InitEdit();

	m_hReturn = new CHourEditControl;
	m_hReturn->SubclassWindow( GetDlgItem(IDC_edtReturnHour)->GetSafeHwnd() );
	m_hReturn->InitEdit();

	m_hReservation = new CHourEditControl;
	m_hReservation->SubclassWindow( GetDlgItem(IDC_edtReservationHour)->GetSafeHwnd() );
	m_hReservation->InitEdit();

	m_hReservationEcpire = new CHourEditControl;
	m_hReservationEcpire->SubclassWindow( GetDlgItem(IDC_edtReservationExpireHour)->GetSafeHwnd() );
	m_hReservationEcpire->InitEdit();

	INT ids;
	ids = InitScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnShowWindow") );

	if( ids == 2 )
	{
		AfxMessageBox( _T("수정할 자료를 먼저 선정해 주십시오") );
		OnCancel();
		return TRUE;
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3260::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	

	EFS_END
}

BOOL CBO_LOC_3260::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CBO_LOC_3260::CheckParentDMCount()
{
	EFS_BEGIN

	CESL_DataMgr *pParentDM = FindDM( m_sParentDMAlias );

	return pParentDM->GetRowCount();

	EFS_END
	return -1;

}

INT CBO_LOC_3260::InitScreen()
{	
	EFS_BEGIN

	INT ids;
	
	ids = MakeDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	if( ids > 0 ) return ids;

	ids = MakeControlEnable();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );
	
	ids = DisplayScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitScreen") );
	
	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3260::MakeDM()
{
	EFS_BEGIN

	INT ids;

	CString sBookCntDMAlias = _T("DM_BO_LOC_3260");
	CESL_DataMgr *pDM = FindDM( sBookCntDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeDM") );
	}

	pDM->FreeData();

	if( pDM->GetRowCount() < 1 ) 
	{
		pDM->InsertRow(-1);
	}
	
	CString sBookCntGridAlias = _T("");

	CESL_Grid *pMainGrid = GetMainGridPtr();
	if( NULL == pMainGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeDM") );
	}

	pMainGrid->SelectMakeList();

	INT nRowCount = pMainGrid->SelectGetCount();

	if( 0 == nRowCount )
	{
		return 2;
	}

	pMainGrid->SelectGetHeadPosition();

	const INT nColumnCnt = 5;
	TCHAR *sColumnAlias[nColumnCnt][2] =
	{
		{ _T("대출"),				_T("0") },	{ _T("반납"),				_T("1") },	
		{ _T("반납연기"),			_T("2") },	{ _T("예약"),				_T("3") }, 	
		{ _T("예약취소"),			_T("4") }
	};

	INT nIndex , i , j;
	INT nColumnIndex = -1;
	CString sResult;
	CString sCnt;

	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pMainGrid->SelectGetIdx();

		ids = m_pParentDM->GetCellData( _T("대출상태") , nIndex , sResult );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeDM") );
		}

		for( j=0 ; j<nColumnCnt ; j++ )
		{
			if ( '3' == sColumnAlias[j][1][0] && 0 == sResult.Compare( _T("예약") ) )
			{
				nColumnIndex = j;
			}
			else if( '4' == sColumnAlias[j][1][0] && 0 <= sResult.Find( _T("예약취소") , 0 ) )
			{
				nColumnIndex = j;
			}
			else if( '3' == sColumnAlias[j][1][0] && 0 <= sResult.Find( _T("무인대출") , 0 ) )
			{
				nColumnIndex = j;
			}
			else if( '1' == sColumnAlias[j][1][0] && 0 <= sResult.Find( _T("무인반납") , 0 ) )
			{
				nColumnIndex = j;
			}
			else if( '1' == sColumnAlias[j][1][0] && 0 <= sResult.Find( _T("분관반납") , 0 ) )
			{
				nColumnIndex = j;
			}
			else if( '1' == sColumnAlias[j][1][0] && 0 <= sResult.Find( _T("타관반납") , 0 ) )
			{
				nColumnIndex = j;
			}
			else if( '0' == sColumnAlias[j][1][0] && _T("책두레대출") == sResult)
			{
				nColumnIndex = j;
			}
			else if( '0' == sColumnAlias[j][1][0] && _T("책두레반납연기") == sResult)
			{
				nColumnIndex = j;
			}
			else if( '1' == sColumnAlias[j][1][0] && _T("책두레반납") == sResult)
			{
				nColumnIndex = j;
			}
			else if( ( 0 == sResult.Compare(sColumnAlias[j][0]) )  || ( 0 == sResult.Compare(sColumnAlias[j][1]) ) )
			{
				nColumnIndex = j;
			}
			else if( '3' == sColumnAlias[j][1][0] && 0 <= sResult.Find( _T("예약(") , 0 ) )
			{
				if( 0 == sResult.Compare( _T("예약(미지정)") ) )
				{
					m_bEnableExpireDay	=	FALSE;
				}

				nColumnIndex = j;
			}
		}
		
		if(0 <= nColumnIndex)
		{
			ids = pDM->GetCellData( sColumnAlias[nColumnIndex][0] , 0 , sCnt );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeDM") );
			}

			sCnt.Format( _T("%d") , _ttoi(sCnt) + 1 );

			ids = pDM->SetCellData( sColumnAlias[nColumnIndex][0] , sCnt , 0 );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeDM") );
			}
		}
		pMainGrid->SelectGetNext();
	}

	return 0;

	EFS_END
	return -1;

}

CESL_Grid* CBO_LOC_3260::GetMainGridPtr()
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( m_sParentCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( m_sGridAlias ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

INT CBO_LOC_3260::DisplayScreen()
{
	EFS_BEGIN

	INT ids;

	ids = AllControlDisplay( _T("CM_BO_LOC_3260_GRID") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3260::MakeControlEnable()
{
	EFS_BEGIN

	INT ids;

	const INT nColumnCnt = 5;
	TCHAR *sColumnAlias[nColumnCnt][2] =
	{
		{ _T("대출") , _T("0") } , { _T("반납") , _T("1") } , { _T("반납연기") , _T("2") } , { _T("예약") , _T("3") } , { _T("예약취소") , _T("4") }
	};

	CString sBookCntDMAlias = _T("DM_BO_LOC_3260");
	CESL_DataMgr *pDM = FindDM( sBookCntDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeControlEnable") );

	SetEnableControl();

	CString sValue;
	for( INT i=0;i<nColumnCnt;i++ )
	{
		ids = pDM->GetCellData( sColumnAlias[i][0] , 0 , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeControlEnable") );

		if( _ttoi(sValue) > 0 )
		{
			ids = SetControlDisable(i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeControlEnable") );
		}
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3260::OnbtnClose() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

INT CBO_LOC_3260::SetControlDisable( INT nMode )
{
	EFS_BEGIN

	const INT nCnt = 7;

	INT nChangeIDC[nCnt] = 
	{
		IDC_datMODIFY1 , IDC_datMODIFY2 , IDC_datMODIFY3 , IDC_datMODIFY4 , IDC_datMODIFY5 , 
		IDC_edtMODIFY13 , IDC_edtMODIFY14
	};

	INT i;
	if( (nMode==0) || (nMode==2) )
	{
		const INT nDisableCnt = 5;
		INT nTmpDisableIDC[nDisableCnt] = 
		{
			2 , 3 , 4 , 5 , 6
		};
		for( i = 0 ; i < nDisableCnt ; i++ )
			GetDlgItem(nChangeIDC[nTmpDisableIDC[i]])->EnableWindow(FALSE);
		
		m_hReturn->EnableWindow(FALSE);
		m_hReservation->EnableWindow(FALSE);
		m_hReservationEcpire->EnableWindow(FALSE);	
	}
	if( (nMode==1) ) 
	{
		const INT nDisableCnt = 2;
		INT nTmpDisableIDC[nDisableCnt] = 
		{
			3 , 4
		};
		for( i = 0 ; i < nDisableCnt ; i++ )
			GetDlgItem(nChangeIDC[nTmpDisableIDC[i]])->EnableWindow(FALSE);
		
		m_hReservation->EnableWindow(FALSE);
		m_hReservationEcpire->EnableWindow(FALSE);
	}
	if( (nMode==3) || (nMode==4) )
	{
		const INT nDisableCnt = 5;
		INT nTmpDisableIDC[nDisableCnt] = 
		{
			0 , 1 , 2 , 5 , 6
		};
		for( i = 0 ; i < nDisableCnt ; i++ )
		{
			GetDlgItem(nChangeIDC[nTmpDisableIDC[i]])->EnableWindow(FALSE);
		}

		if( FALSE == m_bEnableExpireDay )
		{
			GetDlgItem( IDC_datMODIFY5 )->EnableWindow(FALSE);
			GetDlgItem( IDC_edtReservationExpireHour )->EnableWindow(FALSE);
		}

		m_hLoan->EnableWindow(FALSE);
		m_hReturnPlan->EnableWindow(FALSE);	
		m_hReturn->EnableWindow(FALSE);		
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3260::SetEnableControl()
{
	EFS_BEGIN

	const INT nCnt = 7;

	INT nChangeIDC[nCnt] = 
	{
		IDC_datMODIFY1 , IDC_datMODIFY2 , IDC_datMODIFY3 , IDC_datMODIFY4 , IDC_datMODIFY5 , 
		IDC_edtMODIFY13 , IDC_edtMODIFY14
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nChangeIDC[i])->EnableWindow(TRUE);
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3260::OnbtnMODIFY() 
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3260"));
	if( !pDM ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbtnMODIFY") );
	CString strTime;

	m_hLoan->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		ids = pDM->SetCellData(_T("대출시간"), strTime, 0);
		if( 0>ids ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbtnMODIFY") );
	}
	m_hReturnPlan->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		ids = pDM->SetCellData(_T("반납예정시간"), strTime, 0);
		if( 0>ids ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbtnMODIFY") );
	}
	m_hReturn->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		ids = pDM->SetCellData(_T("반납시간"), strTime, 0);
		if( 0>ids ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbtnMODIFY") );
	}
	m_hReservation->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		ids = pDM->SetCellData(_T("예약시간"), strTime, 0);
		if( 0>ids ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbtnMODIFY") );
	}
	m_hReservationEcpire->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		ids = pDM->SetCellData(_T("예약만기시간"), strTime, 0);
		if( 0>ids ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbtnMODIFY") );
	}

	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_일괄수정") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnMODIFY") );

	EFS_END
}


INT CBO_LOC_3260::GetPublishForm( CString &sPublishForm )
{
	EFS_BEGIN

	INT ids;

	ids = GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetPublishForm") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3260::OnClose() 
{
	EFS_BEGIN

	CDialog::OnClose();

	EFS_END
}

HBRUSH CBO_LOC_3260::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
