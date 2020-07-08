#include "stdafx.h"
#include "BO_LOC_3110.h"
#include "..\\BL_LOC_LOAN\\BL_LOC_LOAN_PROC.h"
#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_LOC_3110::CBO_LOC_3110(CESL_Mgr* pParent/*=NULL*/, CLonaManageValue *pLona/*=NULL*/, INT nMode/*=0*/, CLoanShareManager* pLoanShareManager/*=NULL*/, CPreRegCodeMgr* pPreRegNoMgr/*=NULL*/, CString strUserNo/*=_T("")*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_pLona = pLona;
	m_nMode = nMode;
	m_pLoanShareManager = pLoanShareManager;
	m_bShow = FALSE;
	m_pPreRegNoMgr = pPreRegNoMgr;

	m_strUserNo = strUserNo;
}

CBO_LOC_3110::~CBO_LOC_3110()
{
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;
	m_pSpfWork = NULL;
}

VOID CBO_LOC_3110::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBO_LOC_3110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3110, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3110)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSERFIND1, OnbtnUSERFIND1)
	ON_BN_CLICKED(IDC_btnUSERCLOSE, OnbtnUSERCLOSE)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_BN_CLICKED(IDC_btnSEARCH_DETAIL, OnbtnSEARCHDETAIL)
	ON_EN_SETFOCUS(IDC_edtUSERFIND1, OnSetfocusedtUSERFIND1)
	ON_WM_SHOWWINDOW()
	ON_EN_SETFOCUS(IDC_edtSREG_NO, OnSetfocusedtSREGNO)
	ON_EN_CHANGE(IDC_edtUSERFIND9, OnChangeedtUSERFIND9)
	ON_EN_CHANGE(IDC_edtUSERFIND10, OnChangeedtUSERFIND10)
	ON_EN_SETFOCUS(IDC_edtUSERFIND2, OnSetfocusedtUSERFIND2)
	ON_EN_SETFOCUS(IDC_edtUSERFIND3, OnSetfocusedtUSERFIND3)
	ON_EN_SETFOCUS(IDC_edtUSERFIND4, OnSetfocusedtUSERFIND4)
	ON_EN_SETFOCUS(IDC_edt3100_USERFIND5, OnSetfocusedtUSERFIND5)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MENU, OnSelchangeTabMenu)
	ON_WM_CTLCOLOR()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SHOWED, OnShowed)	
END_MESSAGE_MAP()




BOOL CBO_LOC_3110::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3110::OnClose() 
{
	EFS_BEGIN

		CDialog::OnClose();

	EFS_END
}

BOOL CBO_LOC_3110::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if(InitESL_Mgr(_T("BO_LOC_3110")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	((CButton*)GetDlgItem(IDC_chkUSE_LIMIT))->SetCheck(1);


	SetWindowSize( CString(_T("간략검색")) );

	m_pSpfWork = new CBL_LOC_LOAN_PROC(this,m_pLoanShareManager,m_pLona,m_pPreRegNoMgr);
	m_pSpfWork->m_strUserNo = m_strUserNo;

	((CComboBox*)GetDlgItem(IDC_cmbUSERFIND13))->SetCurSel(0);

	InitTabControl();

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3110"), _T("등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);	

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	return TRUE;  

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3110::OnbtnUSERFIND1() 
{
	EFS_BEGIN

	
	if( m_nMode == 0 )	
	{
		m_pSpfWork->SPFExcutePath( _T("이용자자료찾기") );
	}
	else if(1 == m_nMode)
	{
		INT ids;

		CString sRegNo;
		CString sPublishForm;
		ids = m_pSpfWork->SearchUserBook(1,sRegNo,sPublishForm);
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnUSERFIND1") );

		if( sRegNo.IsEmpty() ) return ;

		m_sRegNo = sRegNo;
		m_sPublishForm = sPublishForm;

		OnCancel();
	}
	else if(2 == m_nMode)
	{
		m_pSpfWork->SPFExcutePath( _T("이용자자료찾기") );
	}

	
	GetDlgItem(IDC_edtUSERFIND1)->SetFocus();

	EFS_END
}

VOID CBO_LOC_3110::OnbtnUSERCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_3110::OnbtnCLEAR() 
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_3110");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCLEAR") );

	ids = pCM->AllControlClear();
	if( ids < 0 ) return ;

	
	GetDlgItem(IDC_edtUSERFIND1)->SetFocus();

	EFS_END
}

VOID CBO_LOC_3110::SetWindowSize( CString &sCaption )
{
	EFS_BEGIN

	CRect ChangeWndPos;		
	CRect WndPos;				  
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	
	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staSEARCH_VIEWDETAIL;
	}
	
	else
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staSEARCH_VIEWSIMPLE;
	}

	
	GetWindowRect( &WndPos );
	
	
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	
	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	EFS_END
}

BOOL CBO_LOC_3110::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam)
		{
			ShowWindow(SW_HIDE);
			return TRUE;
		}
		else if(VK_RETURN == pMsg->wParam)
		{
			OnbtnUSERFIND1();
			return TRUE;
		}
		
	}
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;
}

VOID CBO_LOC_3110::OnbtnSEARCHDETAIL() 
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnSEARCH_DETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );		

	EFS_END
}

VOID CBO_LOC_3110::OnSetfocusedtUSERFIND1() 
{
	
    HIMC hIME;
    DWORD dwConversion, dwSentence;
	
    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND1)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND1)->m_hWnd,hIME); 
    } 
	
	
}

VOID CBO_LOC_3110::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_bShow = bShow;
	PostMessage( WM_SHOWED );
}


LRESULT CBO_LOC_3110::OnShowed(WPARAM wParam, LPARAM lParam)
{
	if( IsWindowVisible() )
	{
		OnbtnCLEAR();
		GetDlgItem(IDC_edtUSERFIND1)->SetFocus();
	}

	return 0;
}
VOID CBO_LOC_3110::OnSetfocusedtSREGNO() 
{
	
	
    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtNUMBER)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtNUMBER)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3110::OnChangeedtUSERFIND9() 
{	
	CheckClassNo( IDC_edtUSERFIND9 );
}

VOID CBO_LOC_3110::OnChangeedtUSERFIND10() 
{
	CheckClassNo( IDC_edtUSERFIND10 );
}

VOID CBO_LOC_3110::CheckClassNo( UINT ID )
{
	return;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( ID );
	CString sClassNo, sNewClassNo;
	pEdt->GetWindowText( sClassNo );

	sClassNo.TrimLeft();
	sClassNo.TrimRight();
	sClassNo.Replace( _T(" "), _T("") );
	INT len = sClassNo.GetLength();
	if( !len )
		return;

	TCHAR cClassNo;
	for( INT i = 0; i < len; i++ )
	{
		cClassNo = sClassNo.GetAt( i );

		if( ( cClassNo >= '0' && cClassNo <= '9' ) || cClassNo == '.' )
			sNewClassNo += cClassNo;
	}
		
	pEdt->SetWindowText( sNewClassNo );
	len = sNewClassNo.GetLength();
	pEdt->SetSel( len, len );
}

VOID CBO_LOC_3110::OnSetfocusedtUSERFIND2() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND2)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND2)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3110::OnSetfocusedtUSERFIND3() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND3)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND3)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3110::OnSetfocusedtUSERFIND4() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND4)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND4)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3110::OnSetfocusedtUSERFIND5() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edt3100_USERFIND5)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edt3100_USERFIND5)->m_hWnd,hIME); 
    }
}

void CBO_LOC_3110::OnSelchangeTabMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_MENU);
	if( pTab )
	{
		
		INT nFlag = pTab->GetCurSel();

		CButton* pBtn = (CButton*)GetDlgItem(IDC_chkUSE_UserSearch);
		if( pBtn )
		{
			pBtn->SetCheck(nFlag);
		}
	}

	*pResult = 0;
}

void CBO_LOC_3110::InitTabControl()
{
EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_MENU);
	if( NULL == pTab )	return;

	TC_ITEM item;
	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;
	
	
	item.pszText = _T("업무용 자료찾기");
	pTab->InsertItem(0, &item);
	
	
	item.pszText = _T("이용자용 자료찾기");
	pTab->InsertItem(1, &item);

	pTab->SetCurSel(0);

EFS_END
}

HBRUSH CBO_LOC_3110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
		
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBO_LOC_3110::OnNcActivate(BOOL bActive) 
{	
	return CDialog::OnNcActivate(bActive);
}
