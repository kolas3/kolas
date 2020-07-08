// BO_LOC_3170.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3170.h"
#include "..\\BL_LOC_LOAN\\BL_LOC_RESERVATOIN_PROC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3170 dialog


CBO_LOC_3170::CBO_LOC_3170(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3170)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_LOC_3170::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3170)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3170, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3170)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnRESERVATIONCLOSE, OnbtnRESERVATIONCLOSE)
	ON_BN_CLICKED(IDC_btnDUEDATE, OnbtnDUEDATE)
	ON_BN_CLICKED(IDC_btnSENDMAIL, OnbtnSENDMAIL)
	ON_BN_CLICKED(IDC_btnCANCEL, OnbtnCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3170 message handlers

BOOL CBO_LOC_3170::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3170::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CBO_LOC_3170::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3170")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	
	INT ids;
	ids = SearchRerserveInfo();
	if( ids > 0 ) OnCancel();
	if( ids < 0 ) return -1;

	ids = ViewScreen();
	if( ids < 0 )return -1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_LOC_3170::ViewScreen()
{
	INT ids;

	// 배가 상태 CODE->DESC
	ids = ViewDescWorkingStatus();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBL_LOC_3170::ViewScreen()") );

	const INT nCMCnt = 3;
	CString sCMAlias[nCMCnt] = 
	{
		_T("CM_BO_LOC_3170_BOOK_INFO") , _T("CM_BO_LOC_3170_LOAN_USER_INFO") , _T("CM_BO_LOC_3170_RESERVE_INFO")
	};
	INT nViewIdx[nCMCnt] = {0,0,-1};

	for( INT i = 0 ; i < nCMCnt; i++ )
	{
		ids = AllControlDisplay( sCMAlias[i] , nViewIdx[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBL_LOC_3170::ViewScreen()") );
	}


	InitControl();
	
	SetSelect(0);
	return 0;
}

VOID CBO_LOC_3170::OnbtnRESERVATIONCLOSE() 
{
	OnCancel();
}


INT CBO_LOC_3170::SearchRerserveInfo()
{
	INT ids;
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo( this );
	ids = ReservationInfo.SPFExcutePath( _T("예약정보검색") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::SearchRerserveInfo()") );
	if( ids > 0 ) return ids;

	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3170, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3170)
	ON_EVENT(CBO_LOC_3170, IDC_gridRESERVATION, -600 /* Click */, OnClickgridRESERVATION, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3170::OnClickgridRESERVATION() 
{
	SetSelect( 2 );
}


INT CBO_LOC_3170::SetSelect( INT n )
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
	CString sGridAlias = _T("ReserveGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::SetSelect( INT n )") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3170::SetSelect( INT n )") );
	
	if( n == 0 ) 	ids = pGrid->SetReverse(0);
	else ids = pGrid->SetReverse(pGrid->GetIdx());

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3170::SetSelect( INT n )") );
	return 0;
}

// 예약 만기일 부여
VOID CBO_LOC_3170::OnbtnDUEDATE() 
{
	INT ids;
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo( this );
	ids = ReservationInfo.SPFExcutePath( _T("예약만기일부여") );
	if( ids < 0 ) return ;

	ViewExpireDate();

}

// EMAIL 보내기
VOID CBO_LOC_3170::OnbtnSENDMAIL() 
{
	INT ids;
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo( this );
	ids = ReservationInfo.SPFExcutePath( _T("EMAIL보내기") );

}

// 예약 취소
VOID CBO_LOC_3170::OnbtnCANCEL() 
{
	INT ids;
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo( this );
	ids = ReservationInfo.SPFExcutePath( _T("예약취소") );
	if( ids < 0 ) return ;

	SuccessCancelReservation();
}

INT CBO_LOC_3170::InitControl()
{
	CString sReserveDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	CESL_DataMgr *pDM = FindDM( sReserveDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::InitControl()") );
	if( pDM->GetRowCount() < 1 )
	{
		EnableDisableButton(false);
	}
	else
	{
		EnableDisableButton(true);
	}
	return 0;
}

VOID CBO_LOC_3170::EnableDisableButton( bool bFlag )
{
	GetDlgItem(IDC_btnDUEDATE)->EnableWindow(bFlag);
	GetDlgItem(IDC_btnSENDMAIL)->EnableWindow(bFlag);
	GetDlgItem(IDC_btnCANCEL)->EnableWindow(bFlag);
}

//예약취소가 성공했을 경우 REFLESH
INT CBO_LOC_3170::SuccessCancelReservation()
{
	INT ids;

	CString sReserveDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	CESL_DataMgr *pDM = FindDM( sReserveDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::SuccessCancelReservation()") );
	CString sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
	CString sGridAlias = _T("ReserveGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::SetSelect( INT n )") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3170::SetSelect( INT n )") );

	ids = pDM->DeleteRow( pGrid->GetIdx() );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3170::SuccessCancelReservation()") ); 

	ViewScreen();
	

	return 0;
}

// 배가상태의 코드를 설명으로 보여주는 함수
INT CBO_LOC_3170::ViewDescWorkingStatus()
{
	INT ids;
	CString sCodeValue;
	ids = GetDataMgrData( _T("DM_BO_LOC_3170_BOOK_INFO") , _T("배가상태") , sCodeValue , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::ViewDescWorkingStatus()") ); 

	CBL_LOC_RESERVATOIN_PROC  ReservationInfo( this );
	ids = ReservationInfo.m_pLoanProc->GetWorkingStatusDescToCode( sCodeValue , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3170::ViewDescWorkingStatus()") ); 

	ids = SetDataMgrData( _T("DM_BO_LOC_3170_BOOK_INFO") , _T("배가상태") , sCodeValue , 0 );

	return 0;
}

// 예약 만기일 부여 이후 화면에 보여줄 내용 셋팅
INT CBO_LOC_3170::ViewExpireDate()
{
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo( this );
	INT nExpireDay;
	ReservationInfo.GetReservationExpireDay( nExpireDay );
	CString sExpireDate;
	ReservationInfo.MakeReservationExpireDate( nExpireDay , sExpireDate );

	CString sDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
	CString sGridAlias = _T("ReserveGrid");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::ViewExpireDate()") ); 
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3170::ViewExpireDate()") ); 

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3170::ViewExpireDate()") ); 

	INT nIndex = pGrid->GetIdx();
	pDM->SetCellData( _T("예약만기일") , sExpireDate , nIndex );

	ViewScreen();

	return 0;
}