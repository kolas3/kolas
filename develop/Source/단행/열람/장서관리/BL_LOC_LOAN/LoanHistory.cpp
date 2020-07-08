// LoanHistory.cpp : implementation file
//

#include "stdafx.h"
#include "LoanHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanHistory dialog


CLoanHistory::CLoanHistory(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCloseMode = 1;
	m_bIsStart = TRUE;

	m_sUserKey = _T("");
	m_sBookKey = _T("");
	m_sFamilyKey = _T("");
	m_nMode = 0;
	m_nEntryPoint = 0;
	m_sIsUnityLoanService = _T("N");
}

CLoanHistory::~CLoanHistory()
{
}

VOID CLoanHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanHistory)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanHistory, CDialog)
	//{{AFX_MSG_MAP(CLoanHistory)
	ON_BN_CLICKED(IDC_btnLOAN_COTINUE, OnbtnLOANCOTINUE)
	ON_BN_CLICKED(IDC_btnLOAN_CLOSE, OnbtnLOANCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanHistory message handlers

BOOL CLoanHistory::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	

	INT ids = InitESL_Mgr( _T("SM_LOAN_HISTORY") );
	if( ids < 0 )
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	m_ViewerControl.InitViewer( IDC_LOAN_HISTORY_GRID2 , this );
	m_ViewerControl.SetViewerWidth( 20 , 75 );

	ViewCurrent();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END

	return FALSE;
}

INT CLoanHistory::SetInitData( INT nMode , INT nEntryPoint , CString sUserKey , CString sFamilyKey , CString sBookKey , CString sIsUnityLoanService )
{
	EFS_BEGIN

	m_nMode = nMode;
	m_nEntryPoint = nEntryPoint;
	m_sUserKey = sUserKey;
	m_sFamilyKey = sFamilyKey;
	m_sBookKey = sBookKey;
	m_sIsUnityLoanService = sIsUnityLoanService;

	return 0;

	EFS_END
	return -1;
}

INT CLoanHistory::ViewCurrent()
{
	EFS_BEGIN

	INT ids;

	ids = InitControl();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewCurrent") );

	ids = GetDataFromDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewCurrent") );

	ids = ViewGrid();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewCurrent") );
	

	return 0;

	EFS_END

	return -1;
}

INT CLoanHistory::InitControl()
{
	EFS_BEGIN

	if( m_nMode == 1 )
	{
		if( m_nEntryPoint == 0 )
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 이용자께서 대출한 자료입니다. 대출하시겠습니까?"));
		else
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 이용자께서 대출한 자료입니다. 예약하시겠습니까?"));			
	}
	else if( m_nMode == 2 )
	{
		if( m_nEntryPoint == 0 )
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 가족이 대출한 자료입니다. 대출하시겠습니까?"));
		else
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 가족이 대출한 자료입니다. 예약하시겠습니까?"));
			
	}
	else if( m_nMode == 11 )
	{
		GetDlgItem(IDC_btnLOAN_COTINUE)->ShowWindow(SW_HIDE);
		if( m_nEntryPoint == 0 )
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 이용자께서 대출한 자료입니다. 대출할 수 없습니다."));
		else
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 이용자께서 대출한 자료입니다. 예약할 수 없습니다."));
			

	}
	else if( m_nMode == 22 )
	{
		GetDlgItem(IDC_btnLOAN_COTINUE)->ShowWindow(SW_HIDE);
		if( m_nEntryPoint == 0 )
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 가족이 대출한 자료입니다. 대출할 수 없습니다."));
		else
			GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("예전에 가족이 대출한 자료입니다. 예약할 수 없습니다."));
	}
	else if( m_nMode == 33 )
	{
		GetDlgItem(IDC_btnLOAN_COTINUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_staTITLE2)->SetWindowText(_T("가족이 예약한 자료입니다. 예약할 수 없습니다."));
	}
	
	if( m_nEntryPoint == 0 )
	{

	}
	else if( m_nEntryPoint == 1 )
	{
		GetDlgItem(IDC_btnLOAN_COTINUE)->SetWindowText(_T("예약"));
	}

	return 0;

	EFS_END
	
	return -1;
}

INT CLoanHistory::GetDataFromDB()
{
EFS_BEGIN

	INT ids;

	CString sWhere;

	switch(m_nMode)
	{
	case 1:
	case 11:
		sWhere.Format( _T("CL.BOOK_KEY = %s AND CL.USER_KEY = %s ") , m_sBookKey , m_sUserKey  );
		break;
	case 2:
	case 22:
		sWhere.Format( _T("CL.BOOK_KEY = %s AND ( CLU.FID_KEY = %s ) ") , m_sBookKey , m_sFamilyKey );
		break;
	case 33:
		sWhere.Format( _T("CL.BOOK_KEY = %s AND ( CLU.FID_KEY = %s ) ") , m_sBookKey , m_sFamilyKey );
		break;
	default:
		return -1;
	}		

	CString sDMAlias = _T("DM_LOAN_HISTORY");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetDataFromDB") );

	ids = pDM->RefreshDataManager( sWhere );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("GetDataFromDB") );

	CString strValue1;
	ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("GetDataFromDB") );
	
	strValue1.TrimLeft();		strValue1.TrimRight();
	strValue1.MakeUpper();
	if( _T("Y") != strValue1 )
	{
		INT i , nRowCnt = pDM->GetRowCount();
		CString strUserNo;
		for( i=0 ; i<nRowCnt ; i++ )
		{
			ids = pDM->GetCellData( _T("이용자번호") , i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("GetDataFromDB") );

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
			ids = pDM->SetCellData( _T("이용자번호"), strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("GetDataFromDB") );
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CLoanHistory::ViewGrid()
{
	EFS_BEGIN


	INT ids;

	ids = DisplayBookStatus();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewGrid") );
	
	ids = AllControlDisplay( _T("CM_LOAN_HISTORY") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewGrid") );


	return 0;


	EFS_END

	return -1;

}

INT CLoanHistory::GetCloseMode()
{
	EFS_BEGIN

	return m_nCloseMode;

	EFS_END

	return -1;
}



INT CLoanHistory::DisplayBookStatus()
{
	EFS_BEGIN

	INT ids;
	
	CString sDMAlias = _T("DM_LOAN_HISTORY");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayBookStatus") );
	
	const INT nFieldCnt = 5;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("서명") , _T("저작자") , _T("발행자") , _T("등록번호") , _T("청구기호")
	};
	
	TCHAR* sTitleName[nFieldCnt] =
	{
		_T("서  명") , _T("저작자") , _T("발행자") , _T("등록번호") , _T("청구기호")
	};
		
	CString sData;
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = pDM->GetCellData(sDMFieldName[i] , 0 , sData );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData ) \r\n sDMFieldName[i] : %s \r\n m_nCurrentIndex : %d ") , 
				sDMFieldName[i] , 0 );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayBookStatus") );
		
		m_ViewerControl.SetControlData( sTitleName[i] , sData );
		
	}
	
	m_ViewerControl.Display();
	
	return 0;

	EFS_END

	return -1;
}

VOID CLoanHistory::OnbtnLOANCOTINUE() 
{
	EFS_BEGIN

	m_nCloseMode = 0;
	OnCancel();

	EFS_END
}

VOID CLoanHistory::OnbtnLOANCLOSE() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

BOOL CLoanHistory::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		if( GetDlgItem(IDC_btnLOAN_COTINUE)->IsWindowVisible() )
			OnbtnLOANCOTINUE();
		else
			OnbtnLOANCLOSE();

		return TRUE;
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CLoanHistory, CDialog)
    //{{AFX_EVENTSINK_MAP(CLoanHistory)
	ON_EVENT(CLoanHistory, IDC_LOAN_HISTORY_GRID, -604 /* KeyUp */, OnKeyUpLoanHistoryGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CLoanHistory, IDC_LOAN_HISTORY_GRID2, -604 /* KeyUp */, OnKeyUpLoanHistoryGrid2, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLoanHistory::OnKeyUpLoanHistoryGrid(short FAR* KeyCode, short Shift) 
{
	if( *KeyCode ==  13 )
	{
		if( m_bIsStart )
		{
			m_bIsStart = FALSE;
			return ;
		}

		if( !m_bIsStart )
			PostMessage( WM_KEYDOWN , VK_RETURN );
	}
	else if( *KeyCode == 27 )
	{
		OnCancel();
	}
}

VOID CLoanHistory::OnKeyUpLoanHistoryGrid2(short FAR* KeyCode, short Shift) 
{
	if( *KeyCode ==  13 )
	{
		if( m_bIsStart )
		{
			m_bIsStart = FALSE;
			return ;
		}

		if( !m_bIsStart )
			PostMessage( WM_KEYDOWN , VK_RETURN );
	}
	else if( *KeyCode == 27 )
	{
		OnCancel();
	}
}

HBRUSH CLoanHistory::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
