// LoanBookListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanBookListDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanBookListDlg dialog


CLoanBookListDlg::CLoanBookListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanBookListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCloseStatus = 0;
}

CLoanBookListDlg::~CLoanBookListDlg()
{
}


VOID CLoanBookListDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanBookListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLoanBookListDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanBookListDlg)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	ON_BN_CLICKED(IDC_btnCLOSE_LOAN_BOOK_LIST, OnbtnCLOSELOANBOOKLIST)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanBookListDlg message handlers

BOOL CLoanBookListDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("BL_LOAN_BOOK_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	INT ids = AllControlDisplay( _T("CM_BL_LOAN_BOOK_LIST") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("OnInitDialog") );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CLoanBookListDlg::GetCloseStatus()
{
	EFS_BEGIN

	return m_nCloseStatus;

	EFS_END
	return -1;

}

INT CLoanBookListDlg::IsSelectClose()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("IsSelectClose") );

	INT nRowCount = pDM->GetRowCount();
	if( nRowCount <= 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("IsSelectClose") );

	CString sCMAlias = _T("CM_BL_LOAN_BOOK_LIST" );
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("IsSelectClose") );

	CString sGridAlias = _T("BookList" );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("IsSelectClose") );

	m_nCurrentIndex = pGrid->GetIdx();
	if( m_nCurrentIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("IsSelectClose") );
	
	return 0;

	EFS_END
	return -1;

}

INT CLoanBookListDlg::SetSelectDM()
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("SetSelectDM") );

	INT nRowCount = pDM->GetRowCount();
	if( nRowCount <= 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("SetSelectDM") );

	for( INT i = nRowCount-1 ; i > -1 ; i-- )
	{
		if( i != m_nCurrentIndex ) 
		{
			ids = pDM->DeleteRow( i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SetSelectDM") );
		}
	}

	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CLoanBookListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLoanBookListDlg)
	ON_EVENT(CLoanBookListDlg, IDC_MAINGRID2, -601 /* DblClick */, OnDblClickMaingrid2, VTS_NONE)
	ON_EVENT(CLoanBookListDlg, IDC_MAINGRID2, -600 /* Click */, OnClickMaingrid2, VTS_NONE)
	ON_EVENT(CLoanBookListDlg, IDC_MAINGRID2, -604 /* KeyUp */, OnKeyUpMaingrid2, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLoanBookListDlg::OnDblClickMaingrid2() 
{
	EFS_BEGIN

	INT ids;
	ids = IsSelectClose();
	if( ids < 0 ) return ;

	SetSelectDM();
	
	m_nCloseStatus = 1;
	OnCancel();

	EFS_END
}

VOID CLoanBookListDlg::OnClickMaingrid2() 
{
	EFS_BEGIN

	SetSelect();

	EFS_END
}

BOOL CLoanBookListDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	HWND MsgH = pMsg->hwnd;

	if(pMsg->message == WM_KEYUP && ( (pMsg->wParam == VK_UP) || (pMsg->wParam == VK_DOWN) )  )
	{
		if(	GetDlgItem(IDC_MAINGRID2)->m_hWnd==MsgH )
		{
			INT ids = SetSelect();
			if( ids < 0  )
				return FALSE;
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

INT CLoanBookListDlg::SetSelect( INT n )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BL_LOAN_BOOK_LIST");
	CString sGridAlias = _T("BookList");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("SetSelect( INT n )") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("SetSelect( INT n )") );
	
	if( n == 0 ) 	ids = pGrid->SetReverse(pGrid->GetIdx());
	else ids = pGrid->SetReverse(pGrid->GetIdx());

	if( ids < 0 )ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SetSelect( INT n )") );

	return 0;

	EFS_END
	return -1;

}

VOID CLoanBookListDlg::OnKeyUpMaingrid2(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	if( *KeyCode == 13 )
	{
		INT ids;
		ids = IsSelectClose();
		if( ids < 0 ) return ;

		SetSelectDM();
		
		m_nCloseStatus = 1;
		OnCancel();
	}

	EFS_END
}

VOID CLoanBookListDlg::OnbtnSELECT() 
{
	EFS_BEGIN

	INT ids;
	ids = IsSelectClose();
	if( ids < 0 ) return ;

	SetSelectDM();
	
	m_nCloseStatus = 1;
	OnCancel();

	EFS_END
}


VOID CLoanBookListDlg::OnbtnCLOSELOANBOOKLIST() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CLoanBookListDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

}

HBRUSH CLoanBookListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
