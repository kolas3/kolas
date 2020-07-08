#include "stdafx.h"
#include "FAMILY_ID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFAMILY_ID::CFAMILY_ID(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{	
	m_pParent = pParent;
	m_strUserAccessAuth = _T("");
}


CFAMILY_ID::~CFAMILY_ID()
{
	
}

VOID CFAMILY_ID::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN
	//{{AFX_DATA_MAP(CFAMILY_ID) 
	//}}AFX_DATA_MAP
	CDialog::DoDataExchange(pDX);
	EFS_END
}


BEGIN_MESSAGE_MAP(CFAMILY_ID, CDialog)
	//{{AFX_MSG_MAP(CFAMILY_ID)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_FAMILY_LOAN_HIS, OnBtnFamilyLoanHis)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFAMILY_ID::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CFAMILY_ID::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	
	if(InitESL_Mgr(_T("SM_FAMILY_ID_INFO")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_nCloseMode = 0;
	
	CRect rect;

	GetWindowRect(&rect);

	MoveWindow(m_point.x-15, m_point.y-15, rect.Width(), rect.Height());	
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_FAMILY_ID_INFO"), _T("FamilyGrid"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("가족 상세정보 엑셀출력");
	}

	if( m_strUserAccessAuth.IsEmpty() || m_strUserAccessAuth == _T("1") )
	{
		pGrid->SetColumnEncryption(_T("집전화"),TRUE);
		pGrid->SetColumnEncryption(_T("핸드폰"),TRUE);
		pGrid->SetColumnEncryption(_T("아이디"),TRUE);
		pGrid->SetColumnEncryption(_T("이름"),TRUE);
	}

	INT ids;

	ids = AllControlDisplay( _T("CM_FAMILY_ID_INFO") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CFAMILY_ID::OnInitDialog()") );

	SetSelect();
	
	return TRUE;  
	              

	EFS_END
	return FALSE;

}

VOID CFAMILY_ID::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	CRect rect;

	GetClientRect(&rect);

	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}



INT CFAMILY_ID::GetMemberRecKey( CString &sRecKey )
{
	EFS_BEGIN

	INT ids;

	if( m_nCloseMode == 0 )
		return 0;
	
	ids = GetDataMgrData( _T("DM_FAMILY_ID_INFO") , _T("대출자KEY") , sRecKey , m_nCurrentPos );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFAMILY_ID::GetMemberRecKey( CString &sRecKey )") );

	return 0;

	EFS_END
	return -1;

}


BEGIN_EVENTSINK_MAP(CFAMILY_ID, CDialog)
    //{{AFX_EVENTSINK_MAP(CFAMILY_ID)
	ON_EVENT(CFAMILY_ID, FamilyGrid, -601 /* DblClick */, OnDblClickgridFAMILY, VTS_NONE)
	ON_EVENT(CFAMILY_ID, FamilyGrid, -600 /* Click */, OnClickgridFAMILY, VTS_NONE)
	ON_EVENT(CFAMILY_ID, FamilyGrid, -604 /* KeyUp */, OnKeyUpgridFAMILY, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CFAMILY_ID::OnDblClickgridFAMILY() 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelect(2);
	if( ids < 0 ) return ;
	m_nCloseMode = 1;

	OnCancel();

	EFS_END
}

VOID CFAMILY_ID::OnClickgridFAMILY() 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelect(2);
	if( ids < 0 ) return ;

	EFS_END
}

VOID CFAMILY_ID::OnKeyUpgridFAMILY(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	if( *KeyCode == 13 )
	{
		OnDblClickgridFAMILY();
	}
	if( *KeyCode == 27 )
	{
		OnCancel();
	}

	EFS_END
}

INT CFAMILY_ID::SetSelect( INT n )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_FAMILY_ID_INFO");
	CString sGridAlias = _T("FamilyGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFAMILY_ID::SetSelect( INT n )") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFAMILY_ID::SetSelect( INT n )") );
	
	if( n == 0 ) 	ids = pGrid->SetReverse(0);

	m_nCurrentPos = pGrid->m_nReverse;

	return 0;

	EFS_END
	return -1;

}

BOOL CFAMILY_ID::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if(pMsg->message == WM_KEYUP && ( (pMsg->wParam == VK_UP) || (pMsg->wParam == VK_DOWN) )  )
	{		
		SetSelect(2);
		return TRUE;	
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

VOID CFAMILY_ID::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN


	CDialog::OnTimer(nIDEvent);

	EFS_END
}

VOID CFAMILY_ID::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelect(2);
	if( ids < 0 ) return ;
	m_nCloseMode = 1;

	OnCancel();
	EFS_END
}

VOID CFAMILY_ID::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelect(2);
	if( ids < 0 ) return ;

	EFS_END
}

VOID CFAMILY_ID::OnBtnFamilyLoanHis() 
{
	INT		ids;
	CString strFamilyID;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( !pDM ) 	return;

	ids = pDM->GetCellData( _T("가족KEY") , 0, strFamilyID );
	if( 0>ids ) return;
	
	CFAMILY_LOAN_HIS Dlg(m_pParent);
	Dlg.m_strFamilyID = strFamilyID;
	Dlg.DoModal();
}

HBRUSH CFAMILY_ID::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
