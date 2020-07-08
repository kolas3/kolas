// SE_LOC_1410.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1410.h"

#include "SE_LOC_1411.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1410 dialog


CSE_LOC_1410::CSE_LOC_1410(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1410)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1410::~CSE_LOC_1410()
{
}

VOID CSE_LOC_1410::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1410)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1410, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1410)
	ON_BN_CLICKED(IDC_VIEW_NUM, OnViewNum)
	ON_BN_CLICKED(IDC_btnSEARHC, OnbtnSEARHC)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_CBN_SELCHANGE(IDC_cmbFIND2, OnSelchangecmbFIND2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1410 message handlers

BOOL CSE_LOC_1410::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_LOC_1410::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1410")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	// 검색 가능한 조건 생성
	ChangeSearchCondEnableDisable();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1410::OnViewNum() 
{
	// TODO: Add your control notification handler code here
	CSE_LOC_1411 DLG(this);
	DLG.DoModal();
	
}

VOID CSE_LOC_1410::OnbtnSEARHC() 
{
	CBL_SE_LOC_1400 SpfWork(this);
	SpfWork.SPFExcutePath( SEARCH );
}

VOID CSE_LOC_1410::OnbtnCLOSE() 
{
	OnCancel();	
}

// 자료 상태에 따라 검색 가능한 조건이 달라진다.
VOID CSE_LOC_1410::OnSelchangecmbFIND2() 
{
	ChangeSearchCondEnableDisable();
}

INT CSE_LOC_1410::ChangeSearchCondEnableDisable()
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

BOOL CSE_LOC_1410::PreTranslateMessage(MSG* pMsg) 
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

HBRUSH CSE_LOC_1410::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
