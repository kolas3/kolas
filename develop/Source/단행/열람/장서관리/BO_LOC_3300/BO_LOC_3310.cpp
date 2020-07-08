// BO_LOC_3310.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3310.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3310 dialog


CBO_LOC_3310::CBO_LOC_3310(CESL_Mgr* pParent ,CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3310)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_SpfWork = NULL;
	m_SpfWork = new BL_LOC_USER_PROC( this , m_pLoanShareManager );
}

CBO_LOC_3310::~CBO_LOC_3310()
{
	if( m_SpfWork != NULL)
		delete m_SpfWork;
	m_SpfWork = NULL;	
}

VOID CBO_LOC_3310::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3310)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3310, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3310)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_btnSEARCH_CLEAR, OnbtnSEARCHCLEAR)
	ON_EN_SETFOCUS(IDC_edtFIND8, OnFocusedtFIND8)
	ON_EN_SETFOCUS(IDC_edtFIND4, OnFocusedtFIND4)
	ON_EN_SETFOCUS(IDC_edtFIND14, OnFocusedtFIND14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3310 message handlers

BOOL CBO_LOC_3310::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);


	EFS_END
	return FALSE;

}

VOID CBO_LOC_3310::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	OnCancel();

	EFS_END
}

BOOL CBO_LOC_3310::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("BO_LOC_3310")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowSize( CString(_T("간략검색")) );

	CString sIsFamily;
	GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("가족회원기능사용여부"), sIsFamily );
	sIsFamily.TrimLeft();	sIsFamily.TrimRight();
	if( sIsFamily != _T("Y") )
	{
		GetDlgItem(IDC_STC_FAMILYID )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtFIND10 )->ShowWindow(SW_HIDE);	
	}

	CString strIsUnityLoanService;
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), strIsUnityLoanService);
	if(_T("Y")!=strIsUnityLoanService)
	{
		strIsUnityLoanService = _T("N");
	}
	m_SpfWork->m_sIsUnityLoanService = strIsUnityLoanService;
	if(_T("Y")==strIsUnityLoanService)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtFIND1);
		pEdit->SetLimitText(6);
		pEdit->ModifyStyle(NULL,ES_NUMBER);

		CRect rStaCivilNo;
		CStatic* pStaCivilNo = (CStatic*)GetDlgItem(IDC_stc3300CivilNo);
		pStaCivilNo->GetWindowRect(&rStaCivilNo);
		ScreenToClient(&rStaCivilNo);
		rStaCivilNo.left -= 15;
	}

	CString sDay;
	int ids = GetManageValue( _T("열람") , _T("공통") , _T("개인정보보호") , _T("재동의대상기간") , sDay );
	if(0 != ids) 
	{
		sDay = _T("90");
	}

	CString strDescription;
	strDescription.Format(_T("※ 만료일 %s일전 이용자"), sDay);

	GetDlgItem(IDC_staAgree)->SetWindowText(strDescription);

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3310::OnbtnFIND() 
{
	EFS_BEGIN

	INT ids;

	ids = SearchLoanUser();
	if( ids < 0 )
	{
		AfxMessageBox( _T("기능을 정상적으로 수행하지 못하였습니다." ) );
	}

	EFS_END
}

INT CBO_LOC_3310::SearchLoanUser()
{
	EFS_BEGIN

	INT ids;

	ids = m_SpfWork->SPFExcutePath( _T("이용자검색") );
	if( ids < 0 ) return -1;
	
	ids = ViewGrid();
	if( ids < 0 ) return -1;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300_LIST"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchLoanUser") );
	
	if( pDM->GetRowCount() > 0 )
		AfxMessageBox( _T("이용자가 검색되었습니다.") );

	return 0;

	EFS_END
	return -1;

}


VOID CBO_LOC_3310::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}


INT CBO_LOC_3310::ViewGrid()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");

	ids = m_SpfWork->UserCodeToDesc( sDMAlias );
	if( ids < 0 ) return -1;

	
	ids = AllControlDisplay( sCMAlias );
	if( ids < 0 ) return -1;

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3310::SetWindowSize( CString &sCaption )
{
	EFS_BEGIN

	CRect ChangeWndPos;
	CRect WndPos;
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staVIEWDETAIL;
	}
	else
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staVIEWSIMPLE;
	}

	GetWindowRect( &WndPos );
	
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	if(FALSE == m_pInfo->m_bCivilNoSearch)
	{
		if(_T("상세검색") == sCaption) ChangeWndPos.bottom = ChangeWndPos.bottom - 68;
	}

	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	EFS_END
}

VOID CBO_LOC_3310::OnbtnDETAIL() 
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnDETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );		

	EFS_END
}

BOOL CBO_LOC_3310::PreTranslateMessage(MSG* pMsg) 
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

VOID CBO_LOC_3310::OnbtnSEARCHCLEAR() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3310");
	
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCLEAR") );

	pCM->AllControlClear();

	((CComboBox*)GetDlgItem(IDC_cmbFIND3))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_cmbMemberClass))->SetCurSel(0);

	((CButton*)GetDlgItem(IDC_chk3310CivilNoCheck1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_chk3310CivilNoCheck2))->SetCheck(FALSE);

	EFS_END
}

VOID CBO_LOC_3310::OnFocusedtFIND8()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND8)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND8)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3310::OnFocusedtFIND4()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND4)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND4)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3310::OnFocusedtFIND14()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND14)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND14)->m_hWnd,hIME); 
    }
}

HBRUSH CBO_LOC_3310::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
