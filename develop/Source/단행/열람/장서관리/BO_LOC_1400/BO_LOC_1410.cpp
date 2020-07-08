// BO_LOC_1410.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1410.h"

#include "BO_LOC_1411.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1410 dialog


CBO_LOC_1410::CBO_LOC_1410(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1410)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_1410::~CBO_LOC_1410()
{
	
}

VOID CBO_LOC_1410::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1410)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1410, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1410)
	ON_BN_CLICKED(IDC_VIEW_NUM, OnViewNum)
	ON_BN_CLICKED(IDC_btnSEARHC, OnbtnSEARHC)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_CBN_SELCHANGE(IDC_cmbFIND2, OnSelchangecmbFIND2)
	ON_EN_SETFOCUS(IDC_edtFIND10, OnSetfocusedtFIND10)
	ON_EN_SETFOCUS(IDC_edtFIND11, OnSetfocusedtFIND11)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1410 message handlers

BOOL CBO_LOC_1410::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_1410::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_1410")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	ChangeSearchCondEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1410::OnViewNum() 
{
	// TODO: Add your control notification handler code here
	CBO_LOC_1411 DLG(this);
	DLG.DoModal();
	
}

VOID CBO_LOC_1410::OnbtnSEARHC() 
{
	CBL_BO_LOC_1400 SpfWork(this);
	SpfWork.SPFExcutePath( SEARCH );
}

VOID CBO_LOC_1410::OnbtnCLOSE() 
{
	OnCancel();	
}

VOID CBO_LOC_1410::OnSelchangecmbFIND2() 
{
	ChangeSearchCondEnableDisable();
}

INT CBO_LOC_1410::ChangeSearchCondEnableDisable()
{
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbFIND2);

	CString sStatus;
	pCombo->GetLBText( pCombo->GetCurSel() , sStatus );

	const INT nEnableCnt = 9;
	INT nEnableIDC[nEnableCnt] = 
	{
		IDC_edtFIND1 , IDC_edtFIND2 , IDC_edtFIND3 , IDC_datFIND1 , IDC_datFIND2 , 
		IDC_datFIND3 , IDC_datFIND4 , IDC_datFIND5 , IDC_datFIND6
	};

	for( INT i = 0 ; i < nEnableCnt ; i++ )
		GetDlgItem( nEnableIDC[i] )->EnableWindow(TRUE);


	if( sStatus.Compare( _T("제본등록대상자료") ) == 0 )
	{
		const INT nDisableCnt = 9;

		INT nDisableIDC[nDisableCnt] = 
		{
			IDC_edtFIND1 , IDC_edtFIND2 , IDC_edtFIND3 , IDC_datFIND1 , IDC_datFIND2 , 
			IDC_datFIND3 , IDC_datFIND4 , IDC_datFIND5 , IDC_datFIND6
		};

		for( INT i = 0 ; i < nDisableCnt ; i++ )
			GetDlgItem( nDisableIDC[i] )->EnableWindow(FALSE);
	}
	if( sStatus.Compare( _T("제본처리중자료") ) == 0 )
	{
		const INT nDisableCnt = 2;

		INT nDisableIDC[nDisableCnt] = 
		{
			IDC_datFIND5 , IDC_datFIND6
		};

		for( INT i = 0 ; i < nDisableCnt ; i++ )
			GetDlgItem( nDisableIDC[i] )->EnableWindow(FALSE);
	}
	if( sStatus.Compare( _T("제본등록자료") ) == 0 )
	{
		const INT nDisableCnt = 7;

		INT nDisableIDC[nDisableCnt] = 
		{
			IDC_edtFIND1 , IDC_edtFIND2 , IDC_edtFIND3 ,  
			IDC_datFIND3 , IDC_datFIND4 , IDC_datFIND5 , IDC_datFIND6
		};

		for( INT i = 0 ; i < nDisableCnt ; i++ )
			GetDlgItem( nDisableIDC[i] )->EnableWindow(FALSE);
	}
	if( sStatus.Compare( _T("제본의뢰자료") ) == 0 )
	{
		const INT nDisableCnt = 2;

		INT nDisableIDC[nDisableCnt] = 
		{
			IDC_datFIND5 , IDC_datFIND6
		};

		for( INT i = 0 ; i < nDisableCnt ; i++ )
			GetDlgItem( nDisableIDC[i] )->EnableWindow(FALSE);
	}

	return 0;
}

BOOL CBO_LOC_1410::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) 
		{
			OnbtnSEARHC();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_LOC_1410::SetIME(INT nIDC, INT nMode)
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

VOID CBO_LOC_1410::OnSetfocusedtFIND10() 
{
	SetIME(IDC_edtFIND10, 1);
}

VOID CBO_LOC_1410::OnSetfocusedtFIND11() 
{
	SetIME(IDC_edtFIND10, 1);
}

HBRUSH CBO_LOC_1410::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
