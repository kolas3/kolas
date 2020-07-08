// SE_LOC_1510.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1510.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1510 dialog


CSE_LOC_1510::CSE_LOC_1510(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1510)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1510::~CSE_LOC_1510()
{
	//=====================================================
	//2009.02.09 ADD BY PJW : delete는 소멸자에서 한다.
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;
	//=====================================================
	
}

VOID CSE_LOC_1510::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1510)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1510, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1510)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_bRECEIPT_VIEW, OnbRECEIPTVIEW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1510 message handlers

BOOL CSE_LOC_1510::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_LOC_1510::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//=====================================================
	//2009.02.09 UPDATE BY PJW : delete는 소멸자에서 한다.
	// 	if( m_pSpfWork != NULL ) delete m_pSpfWork;
// 	m_pSpfWork = NULL;
// 	DestroyWindow();
	OnbCLOSE() ;
	//=====================================================
	

}

BOOL CSE_LOC_1510::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1510")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	SetWindowSize( CString(_T("간략검색")) );

	m_pSpfWork = new CBL_SE_LOC_15(this);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


///--------------------------------------------------------------------------------------
/// Function Name : SetWindowSize( CString &sCaption )
/// Purpose : 윈도우의 사이즈를 조정하기 위하여 
///                 실제 크기는 IDC_staVIEWDETAIL , IDC_staVIEWSIMPLE 의 크기를 참조하여 설정한다.
/// 
/// Parameters : _T("상세검색") , _T("간략검색") 만 입력 가능
///
VOID CSE_LOC_1510::SetWindowSize( CString &sCaption )
{
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
	
	if( sCaption.Compare( szCaption[0] ) == 0 )
		CenterWindow();
}

VOID CSE_LOC_1510::OnbtnDETAIL() 
{
	CString sCaption;
	GetDlgItem( IDC_btnDETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );		
}

VOID CSE_LOC_1510::OnbCLOSE() 
{
	OnCancel();	
}

VOID CSE_LOC_1510::OnbtnSEARCH() 
{
	m_pSpfWork->SPFExcutePath( SEARCH );	
}

VOID CSE_LOC_1510::OnbtnINIT() 
{
	CString sCMAlias = _T("CM_SE_LOC_1510");
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if(pCM == NULL)ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnINIT") );

	pCM->AllControlClear();

	// 자료 상태 초기화
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSTATUS);
	pCombo->SetCurSel(0);
	
}

BOOL CSE_LOC_1510::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) 
		{
			OnbtnSEARCH();
			return TRUE;
		}
	}
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSE_LOC_1510::OnbRECEIPTVIEW() 
{
	// TODO: Add your control notification handler code here
	
	CReceiptNumViwerDlg dlg( this );
	if( IDOK == dlg.DoModal() )
		GetDlgItem( IDC_datHAM3 )->SetWindowText( dlg.m_receiptNum );
}

HBRUSH CSE_LOC_1510::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
