// BO_LOC_1310.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1310.h"

#include "..\\BL_LOC\\BL_BOSHELF.h"
#include "ShelfChangeReasonList.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1310 dialog


CBO_LOC_1310::CBO_LOC_1310(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1310)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_1310::~CBO_LOC_1310()
{
	
}

VOID CBO_LOC_1310::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1310)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_1310, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1310)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_radCondition2, OnradCondition2)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_CBN_SELCHANGE(IDC_cmbCODECHANGE, OnSelchangecmbCODECHANGE)
	ON_EN_SETFOCUS(IDC_edtAUTHOR, OnSetfocusedtAUTHOR)
	ON_EN_SETFOCUS(IDC_edtPUBLISHER, OnSetfocusedtPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtTITLE, OnSetfocusedtTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1310 message handlers

VOID CBO_LOC_1310::OnClose() 
{
	EFS_BEGIN
		
	//DestroyWindow();
	CDialog::OnCancel();

	EFS_END
}

BOOL CBO_LOC_1310::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 		
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_1310")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetWindowSize( CString(_T("간략검색")) );
	((CButton*)GetDlgItem( IDC_radCondition1 ))->SetCheck(1); 
	
	SetShelfChangeReasonControl();

	SetShelfWorkControl();	

	( (CEdit*) GetDlgItem (IDC_datCHANGEDATE1) )->EnableWindow(FALSE);
	( (CEdit*) GetDlgItem (IDC_datCHANGEDATE2) )->EnableWindow(FALSE);
	
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_BO_LOC_1310"), _T("별치기호1"));
	CString strData[2] = {_T("EMP"), _T("없음")};
	pCmb->SetData(strData, 2, pCmb->GetCount());
	
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_1310"), _T("등록번호1"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_1310"), _T("등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_1310"), _T("등록번호2"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_1310"), _T("등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_1310::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_1310::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_1310::OnbtnDETAIL() 
{
	EFS_BEGIN

	CString sCaption;
	GetDlgItem( IDC_btnDETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );	

	EFS_END
}


VOID CBO_LOC_1310::SetWindowSize( CString &sCaption )
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

	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );
	
	if( sCaption.Compare( szCaption[0] ) == 0 )
		CenterWindow();

	EFS_END
}

VOID CBO_LOC_1310::OnbtnFIND() 
{
	EFS_BEGIN

	INT ids;
	
	CBL_BOSHELF SpfWork( this );
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

VOID CBO_LOC_1310::SetShelfChangeReasonControl()
{
	EFS_BEGIN

	EFS_END
}


INT CBO_LOC_1310::GetShelfChangeReasonList( CString sGetData[] , INT &nRowCount )
{
	EFS_BEGIN

	CShelfChangeReasonList List(this);
	List.GetList( sGetData , nRowCount );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1310::GetShelfChangeReasonCount()
{
	EFS_BEGIN

	CShelfChangeReasonList List(this);
	return List.GetListCount();

	EFS_END
	return -1;

}
VOID CBO_LOC_1310::OnradCondition2() 
{
	EFS_BEGIN

	CBL_BOSHELF SpfWork( this );
	INT ids = SpfWork.SPFExcutePath( _T("배가자료관리_파일로검색") );	

	EFS_END
}

BOOL CBO_LOC_1310::PreTranslateMessage(MSG* pMsg) 
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

VOID CBO_LOC_1310::OnbtnINIT() 
{
	EFS_BEGIN

	CString sCMAlias;

	sCMAlias = _T("CM_BO_LOC_1310");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnINIT") );

	pCM->AllControlClear();
	((CComboBox*)GetDlgItem(IDC_cmbSHELFSTAT))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_cmbCODECHANGE))->SetCurSel(0);

	EFS_END
}

INT CBO_LOC_1310::SetShelfWorkControl()
{
	INT ids;

	CString sDMAlias = _T("DM_MNG_ACCESS_CONTROL_1000_사용자");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if ( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1310::SetShelfWorkControl") );
	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1310::SetShelfWorkControl") );

	INT nRowCnt = pDM->GetRowCount();
	if ( nRowCnt < 1 ) return 0;

	CString sCMAlias = _T("CM_BO_LOC_1310");
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
VOID CBO_LOC_1310::OnSetfocusedtAUTHOR() 
{
	SetIME(IDC_edtAUTHOR, 1);
}

VOID CBO_LOC_1310::OnSetfocusedtPUBLISHER() 
{
	SetIME(IDC_edtPUBLISHER, 1);
}

VOID CBO_LOC_1310::OnSetfocusedtTITLE() 
{
	SetIME(IDC_edtTITLE, 1);
}

VOID CBO_LOC_1310::SetIME(INT nIDC, INT nMode)
{
	EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
			

	EFS_END
}

VOID CBO_LOC_1310::OnSelchangecmbCODECHANGE() 
{
	INT nIndex = ((CComboBox*)GetDlgItem(IDC_cmbCODECHANGE))->GetCurSel();
	if ( nIndex == 0 )
	{
		( (CEdit*) GetDlgItem (IDC_datCHANGEDATE1) )->EnableWindow(FALSE);
		( (CEdit*) GetDlgItem (IDC_datCHANGEDATE2) )->EnableWindow(FALSE);
	}
	else
	{
		( (CEdit*) GetDlgItem (IDC_datCHANGEDATE1) )->EnableWindow(TRUE);
		( (CEdit*) GetDlgItem (IDC_datCHANGEDATE2) )->EnableWindow(TRUE);
	}

}

HBRUSH CBO_LOC_1310::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
