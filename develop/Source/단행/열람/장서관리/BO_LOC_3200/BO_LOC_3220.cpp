// BO_LOC_3220.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3220.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3220 dialog


CBO_LOC_3220::CBO_LOC_3220(CESL_Mgr* pParent , CLonaManageValue *pLona, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3220)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pSpfWork = NULL;
	m_pSpfWork = new CBL_LOC_LOAN_MNG_PROC(this,pLona,m_pLoanShareManager);

	m_sParentCMAlias = _T("CM_BO_LOC_3200");
	m_sParentDMAlias = _T("DM_BO_LOC_3200");
	m_sGridAlias = _T("MainGrid");
	m_sSMAlias = _T("BO_LOC_3220");
	m_sClientCMAlias = _T("CM_BO_LOC_3220");

	m_hLoanTime = NULL;
	m_hReturnPlanTime = NULL;
	m_hReturnTime = NULL;
	m_hReservationTime = NULL;
	m_hReservationExpireTime = NULL;

	m_nViewMode = 0;
	m_nCurrentIndex = 0;
	m_nLastIndex = 0;
	m_nFirstIndex = 0;
	bFirstView = FALSE;
}

CBO_LOC_3220::~CBO_LOC_3220()
{
	if( m_pSpfWork )
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}

	if( m_hLoanTime )
	{
		delete m_hLoanTime;
		m_hLoanTime = NULL;
	}

	if( m_hReturnPlanTime )
	{
		delete m_hReturnPlanTime;
		m_hReturnPlanTime = NULL;
	}

	if( m_hReturnTime )
	{
		delete m_hReturnTime;
		m_hReturnTime = NULL;
	}

	if( m_hReservationTime )
	{
		delete m_hReservationTime;
		m_hReservationTime = NULL;
	}

	if( m_hReservationExpireTime )
	{
		delete m_hReservationExpireTime;
		m_hReservationExpireTime = NULL;
	}
}

VOID CBO_LOC_3220::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3220)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3220, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3220)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnMODIFY, OnbtnMODIFY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3220 message handlers

BOOL CBO_LOC_3220::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3220::OnClose() 
{
	EFS_BEGIN

	CDialog::OnClose();

	EFS_END
}

BOOL CBO_LOC_3220::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	if(InitESL_Mgr(m_sSMAlias) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_hLoanTime = new CHourEditControl;
	m_hLoanTime->SubclassWindow( GetDlgItem(IDC_hLoanHour)->GetSafeHwnd() );
	m_hLoanTime->InitEdit();

	m_hReturnPlanTime = new CHourEditControl;
	m_hReturnPlanTime->SubclassWindow( GetDlgItem(IDC_hReturnPlanHour)->GetSafeHwnd() );
	m_hReturnPlanTime->InitEdit();

	m_hReturnTime = new CHourEditControl;
	m_hReturnTime->SubclassWindow( GetDlgItem(IDC_hReturnHour)->GetSafeHwnd() );
	m_hReturnTime->InitEdit();

	m_hReservationTime = new CHourEditControl;
	m_hReservationTime->SubclassWindow( GetDlgItem(IDC_hReservationHour)->GetSafeHwnd() );
	m_hReservationTime->InitEdit();

	m_hReservationExpireTime = new CHourEditControl;
	m_hReservationExpireTime->SubclassWindow( GetDlgItem(IDC_hReservationExpireHour)->GetSafeHwnd() );
	m_hReservationExpireTime->InitEdit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3220::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	bFirstView = TRUE;

	InitSetStartPos();

	INT ids = ViewCurrentWindow();
	if( ids < 0 ) return;

	bFirstView = FALSE;
	

	EFS_END
}

INT CBO_LOC_3220::InitSetStartPos()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->SelectGetIdx();

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3220::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex( 0 );	

	EFS_END
}

VOID CBO_LOC_3220::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex( 1 );

	EFS_END
}


CESL_Grid* CBO_LOC_3220::GetMainGridPtr()
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

INT CBO_LOC_3220::MoveIndex( INT nMode )
{	
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3220::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	ViewCurrentWindow();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3220::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;
	CString sBookRecKey;
	
	CString sCMAlias = m_sClientCMAlias;
	ids = AllControlDisplay( sCMAlias , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3220::ViewCurrentWindow()") );

	ids = ApplyEditableControl();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3220::ViewCurrentWindow()") );

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3220::ViewCurrentWindow()") );

	GetDlgItem(IDC_btnPREV)->EnableWindow( TRUE );
	GetDlgItem(IDC_btnNEXT)->EnableWindow( TRUE );

	if( m_nCurrentIndex == m_nFirstIndex )
		GetDlgItem(IDC_btnPREV)->EnableWindow( FALSE );	
	if( m_nCurrentIndex == m_nLastIndex )
		GetDlgItem(IDC_btnNEXT)->EnableWindow( FALSE );
	
	pGrid->SetReverse( m_nCurrentIndex );

	CESL_DataMgr *pDM = FindDM( m_sParentDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5, _T("CBO_LOC_3220::ViewCurrentWindow()") );

	CString sWorkingStatus;
	ids = pDM->GetCellData( _T("배가상태") , m_nCurrentIndex , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("CBO_LOC_3220::ViewCurrentWindow()") );
	
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

	ids = SetControlData( m_sClientCMAlias , _T("배가상태") , sWorkingStatus  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("CBO_LOC_3220::ViewCurrentWindow()") );

	if(_T("Y") == m_pSpfWork->m_pLoan->m_sIsUnityLoanService)
	{
		CString strGetData;
		ids = pDM->GetCellData(_T("통합회원여부"), m_nCurrentIndex, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("CBO_LOC_3220::ViewCurrentWindow()"));
		ids = SetControlData(m_sClientCMAlias, _T("통합회원여부"), strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("CBO_LOC_3220::ViewCurrentWindow()"));
	}

	CString strDate;
	INT     nPos;
	const INT nCnt = 5;
	CString strAlias[nCnt] = { _T("대출일"), _T("반납예정일"), _T("반납일"), _T("예약일"), _T("예약만기일") };
	CHourEditControl* pControl[5]={m_hLoanTime,m_hReturnPlanTime,m_hReturnTime,
		                           m_hReservationTime,m_hReservationExpireTime};

	for( INT i=0;i<nCnt;i++ )
	{
		ids = pDM->GetCellData(strAlias[i], m_nCurrentIndex, strDate);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8, _T("CBO_LOC_3220::ViewCurrentWindow()") );
		strDate.TrimLeft();strDate.TrimRight();

		nPos=strDate.Find(_T(" "));
		if( nPos != -1 )
		{
			strDate=strDate.Mid(nPos+1);
			pControl[i]->SetData(strDate);
		}
		else
		{
			if( strDate.GetLength() )
			{
				pControl[i]->SetData(_T("00:00:00"));
			}
		}
	}



	return 0;


	EFS_END
	return -1;

}

VOID CBO_LOC_3220::OnbtnClose() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}


INT CBO_LOC_3220::CheckParentDMCount()
{
	EFS_BEGIN

	CESL_DataMgr *pParentDM = FindDM( m_sParentDMAlias );

	return pParentDM->GetRowCount();

	EFS_END
	return -1;

}

VOID CBO_LOC_3220::OnbtnMODIFY() 
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3220"));
	if( !pDM ) 
	{
		AfxMessageBox(_T("CBO_LOC_3220::OnbtnMODIFY() ERROR -1"));
		return ;
	}

	pDM->FreeData();
	pDM->InsertRow(-1);

	CString strTime;
	m_hLoanTime->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		pDM->SetCellData(_T("대출시간"),strTime,0);
	}
	m_hReturnPlanTime->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
	}
	m_hReturnTime->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		pDM->SetCellData(_T("반납시간"),strTime,0);
	}
	m_hReservationTime->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
		pDM->SetCellData(_T("예약시간"),strTime,0);
	}
	m_hReservationExpireTime->GetData( strTime );
	if( !strTime.IsEmpty() )
	{
	}

	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_수정") );
	if( ids < 0 ) return ;

	AfxMessageBox(_T("수정이 되었습니다."));

	EFS_END
}


INT CBO_LOC_3220::ApplyEditableControl()
{
	EFS_BEGIN

	INT ids;

	CString sLoanStatus;
	ids = GetDataMgrData( m_sParentDMAlias , _T("대출상태") , sLoanStatus , m_nCurrentIndex );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::ApplyEditableControl()") );
	
	const INT nCnt = 7;

	INT nChangeIDC[nCnt] = 
	{
		IDC_datMODIFY1 , IDC_datMODIFY2 , IDC_datMODIFY3 , IDC_datMODIFY4 , IDC_datMODIFY5 , 
		IDC_edtMODIFY13 , IDC_edtMODIFY14
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nChangeIDC[i])->EnableWindow(FALSE);
	}

	m_hLoanTime->EnableWindow(FALSE);
	m_hReturnPlanTime->EnableWindow(FALSE);
	m_hReturnTime->EnableWindow(FALSE);
	m_hReservationTime->EnableWindow(FALSE);
	m_hReservationExpireTime->EnableWindow(FALSE);

	if( (sLoanStatus.Compare(_T("대출"))==0) || (sLoanStatus.Compare(_T("반납연기"))==0) 
		|| (sLoanStatus.Compare(_T("책두레대출"))==0) || (sLoanStatus.Compare(_T("책두레반납연기"))==0)
		
		)
	{
		const INT nEnableCnt = 2;
		INT nTmpEnableIDC[nEnableCnt] = 
		{
			0 , 1
		};
		for( i = 0 ; i < nEnableCnt ; i++ )
			GetDlgItem(nChangeIDC[nTmpEnableIDC[i]])->EnableWindow(TRUE);

		m_hLoanTime->EnableWindow(TRUE);
		m_hReturnPlanTime->EnableWindow(TRUE);	
	}
	if( ( sLoanStatus.Compare( _T("반납") )==0 ) || 
		( sLoanStatus.Find( _T("분관") ) != -1 ) || ( sLoanStatus.Find( _T("타관") ) != -1 ) 
		|| (sLoanStatus.Compare(_T("책두레반납"))==0)
	
	) 
	{
		const INT nEnableCnt = 5;
		INT nTmpEnableIDC[nEnableCnt] = 
		{
			0 , 1 , 2 , 5 , 6
		};
		for( i = 0 ; i < nEnableCnt ; i++ )
			GetDlgItem(nChangeIDC[nTmpEnableIDC[i]])->EnableWindow(TRUE);

		m_hLoanTime->EnableWindow(TRUE);
		m_hReturnPlanTime->EnableWindow(TRUE);
		m_hReturnTime->EnableWindow(TRUE);
	}
	if( sLoanStatus.Find(_T("예약")) != -1 )
	{
		const INT nEnableCnt = 2;
		INT nTmpEnableIDC[nEnableCnt] = 
		{
			3 , 4
		};
		for( i = 0 ; i < nEnableCnt ; i++ )
			GetDlgItem(nChangeIDC[nTmpEnableIDC[i]])->EnableWindow(TRUE);

		m_hReservationTime->EnableWindow(TRUE);
		m_hReservationExpireTime->EnableWindow(TRUE);
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3220::GetPublishForm( CString &sPublishForm )
{
	EFS_BEGIN

	INT ids;

	ids = GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetPublishForm") );

	return 0;

	EFS_END
	return -1;

}

HBRUSH CBO_LOC_3220::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
