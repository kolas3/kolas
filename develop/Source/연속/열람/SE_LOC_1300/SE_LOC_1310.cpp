// SE_LOC_1310.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1310.h"

#include "ShelfChangeReasonList.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1310 dialog


CSE_LOC_1310::CSE_LOC_1310(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1310)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1310::~CSE_LOC_1310()
{
}

VOID CSE_LOC_1310::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1310)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1310, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1310)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_radCondition2, OnradCondition2)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1310 message handlers

VOID CSE_LOC_1310::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	//DestroyWindow();
	CDialog::OnCancel();

	EFS_END
}

BOOL CSE_LOC_1310::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1310")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowSize( CString(_T("간략검색")) );
	((CButton*)GetDlgItem( IDC_radCondition1 ))->SetCheck(1); 
	
	SetShelfChangeReasonControl();

	SetShelfWorkControl();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BOOL CSE_LOC_1310::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1310::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CSE_LOC_1310::OnbtnDETAIL() 
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnDETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );	

	EFS_END
}

///--------------------------------------------------------------------------------------
/// Function Name : SetWindowSize( CString &sCaption )
/// Purpose : 윈도우의 사이즈를 조정하기 위하여 
///                 실제 크기는 IDC_staVIEWDETAIL , IDC_staVIEWSIMPLE 의 크기를 참조하여 설정한다.
/// 
/// Parameters : _T("상세검색") , _T("간략검색") 만 입력 가능
///
VOID CSE_LOC_1310::SetWindowSize( CString &sCaption )
{
	EFS_BEGIN

	CRect ChangeWndPos;		/// 변경될 콘트롤 사이즈를 얻어오기 위한 인스턴스
	CRect WndPos;				  /// 현재 윈도우의 위치를 얻어오기 위한 인스턴스	
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	// 명령이 _T("상세검색")라면
	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staVIEWDETAIL;
	}
	// 명령이 _T("간략검색")라면
	else
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staVIEWSIMPLE;
	}

	// 현재 윈도우의 위치를 얻어옴
	GetWindowRect( &WndPos );
	
	// 변경될 위치를 얻어옴
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	// 변경될 위치로 변경함
	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	EFS_END
}

VOID CSE_LOC_1310::OnbtnFIND() 
{
	EFS_BEGIN

	INT ids;
	
	CBL_SE_LOC_13 SpfWork( this );
	if( ((CButton*)GetDlgItem( IDC_radCondition1 ))->GetCheck() == 1 )
	{
		ids = SpfWork.SPFExcutePath( _T("배가자료관리_검색") );
	}
	else
	{
		ids = SpfWork.SPFExcutePath( _T("배가자료관리_파일로검색") );
	}
	

	EFS_END
}

// 배가 변경 사유 콘트롤 보여주는 함수
VOID CSE_LOC_1310::SetShelfChangeReasonControl()
{
	EFS_BEGIN


	/*
	CString sCMAlias = _T("CM_SE_LOC_1310");
	CString sControlAlias = _T("배가변경사유");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CSE_LOC_1310::SetShelfChangeResonControl()") );

	CComboBox *pComboBox = (CComboBox*)(pCM->FindControl(sControlAlias));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CSE_LOC_1310::SetShelfChangeResonControl()") );

	CString *sGetData;
	sGetData = NULL;

	INT nRowCount;

	INT nReasonCnt = GetShelfChangeReasonCount();

	sGetData = new CString[nReasonCnt];

	ids = GetShelfChangeReasonList( sGetData , nRowCount );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("CSE_LOC_1310::SetShelfChangeResonControl()") );

	for( INT i = 0 ; i < nRowCount ; i++ )
	{	
		pComboBox->AddString( sGetData[i] );
	}
	
	pComboBox->SetCurSel(0);

	if( sGetData != NULL ) delete []sGetData;

	*/

	EFS_END
}


INT CSE_LOC_1310::GetShelfChangeReasonList( CString sGetData[] , INT &nRowCount )
{
	EFS_BEGIN

	CShelfChangeReasonList List(this);
	List.GetList( sGetData , nRowCount );

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1310::GetShelfChangeReasonCount()
{
	EFS_BEGIN

	CShelfChangeReasonList List(this);
	return List.GetListCount();

	EFS_END
	return -1;

}
VOID CSE_LOC_1310::OnradCondition2() 
{
	EFS_BEGIN

	CBL_SE_LOC_13 SpfWork( this );
	INT ids = SpfWork.SPFExcutePath( _T("배가자료관리_파일로검색") );	

	EFS_END
}

BOOL CSE_LOC_1310::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbtnFIND();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1310::OnbtnINIT() 
{
	EFS_BEGIN

	CString sCMAlias;

	sCMAlias = _T("CM_SE_LOC_1310");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnINIT") );

	pCM->AllControlClear();
	((CComboBox*)GetDlgItem(IDC_cmbSHELFSTAT))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_cmbCODECHANGE))->SetCurSel(0);

	EFS_END
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-16
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 배가변경자콤보 박스의 목록을 구성한다.
// [  PARAMETERS  ]
//    NULL     
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 사용자 DM을 찾는다.
// 2. DM의 정보를 콤보박스에 삽입한다.
//--------------------------------------------------------------------------------------------
INT CSE_LOC_1310::SetShelfWorkControl()
{
	INT ids;

	//-----------------------------------------------------------------------------
	// 1. 사용자 DM을 찾는다.  
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_MNG_ACCESS_CONTROL_1000_사용자");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if ( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1310::SetShelfWorkControl") );
	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	INT nRowCnt = pDM->GetRowCount();
	if ( nRowCnt < 1 ) return 0;

	//-----------------------------------------------------------------------------
	// 2. DM의 정보를 콤보박스에 삽입한다.  
	//-----------------------------------------------------------------------------
	CString sCMAlias = _T("CM_SE_LOC_1310");
	CString sControlAlias = _T("배가변경자");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	CComboBox *pComboBox = (CComboBox*)(pCM->FindControl(sControlAlias));
	if( pComboBox == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	CString sData = _T("적용안함");
	pComboBox->AddString( sData );

	for ( INT i = 0;i < nRowCnt;i++ )
	{
		ids = pDM->GetCellData( _T("USER_ID") , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_1310::SetShelfWorkControl") );
		sData.TrimLeft();sData.TrimRight();
		if ( sData.IsEmpty() ) continue;

		pComboBox->AddString( sData );
	}

	pComboBox->SetCurSel( 0 );
	return 0;
}
HBRUSH CSE_LOC_1310::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
