// SeDayType.cpp : implementation file
//

#include "stdafx.h"
#include "SeDayType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDayType dialog


CSeDayType::CSeDayType(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeDayType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDayType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSeDayType::~CSeDayType()
{
}


VOID CSeDayType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDayType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDayType, CDialog)
	//{{AFX_MSG_MAP(CSeDayType)
	ON_EN_CHANGE(IDC_edtDAY1, OnChangeedtDAY1)
	ON_EN_CHANGE(IDC_edtDAY2, OnChangeedtDAY2)
	ON_EN_CHANGE(IDC_edtDAY3, OnChangeedtDAY3)
	ON_EN_CHANGE(IDC_edtDAY4, OnChangeedtDAY4)
	ON_EN_CHANGE(IDC_edtDAY5, OnChangeedtDAY5)
	ON_EN_CHANGE(IDC_edtDAY6, OnChangeedtDAY6)
	ON_EN_KILLFOCUS(IDC_edtDAY1, OnKillfocusedtDAY1)
	ON_EN_KILLFOCUS(IDC_edtDAY2, OnKillfocusedtDAY2)
	ON_EN_KILLFOCUS(IDC_edtDAY3, OnKillfocusedtDAY3)
	ON_EN_KILLFOCUS(IDC_edtDAY4, OnKillfocusedtDAY4)
	ON_EN_KILLFOCUS(IDC_edtDAY5, OnKillfocusedtDAY5)
	ON_EN_KILLFOCUS(IDC_edtDAY6, OnKillfocusedtDAY6)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDayType message handlers
BOOL CSeDayType::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CSeDayType::GetAllData(CString data[])
{
	const INT cnt = 12;
	UINT id[ cnt ];
	GetEdtID( id );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}

}


VOID CSeDayType::GetEdtID(UINT ID[])
{
	ID[  0 ] = IDC_edtDAY1;
	ID[  1 ] = IDC_edtDAY2;
	ID[  2 ] = IDC_edtDAY3;
	ID[  3 ] = IDC_edtDAY4;
	ID[  4 ] = IDC_edtDAY5;
	ID[  5 ] = IDC_edtDAY6;
	ID[  6 ] = IDC_edtDAY7;
	ID[  7 ] = IDC_edtDAY8;
	ID[  8 ] = IDC_edtDAY9;
	ID[  9 ] = IDC_edtDAY10;
	ID[ 10 ] = IDC_edtDAY11;
	ID[ 11 ] = IDC_edtDAY12;
}

VOID CSeDayType::SetAllData(CString data[])
{
	const INT cnt = 12;
	UINT id[ cnt ];
	GetEdtID( id );
	CEdit *pEdt;
	
	for( INT i = 0; i < cnt; i++ )
	{
		if(data[i].IsEmpty()) continue;
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->SetWindowText( data[ i ] );
	}

}

VOID CSeDayType::ClearAllData()
{
	const INT cnt = 12;
	UINT id[ cnt ];
	GetEdtID( id );
	CEdit *pEdt;
	
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->SetWindowText(_T(""));
	}

	DisableCntControls();
}



VOID CSeDayType::DisableCntControls()
{
	
	const INT cnt = 12;
	UINT id[ cnt ];
	GetEdtID( id );
	CEdit *pEdt;

	for( INT i = 6; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->EnableWindow( false );
	}
}

VOID CSeDayType::DisableCntEdt(INT nDay)
{
	const INT cnt = 12;
	UINT id[ cnt ];
	GetEdtID( id );
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( id[ nDay-1 ] );

	CString tmp;
	pEdt->GetWindowText(tmp);
	if(!tmp.IsEmpty()) return;
	

	pEdt = ( CEdit * )GetDlgItem( id[ nDay+5 ] );
	pEdt->SetWindowText( _T("") );
	pEdt->EnableWindow( false );
}

BOOL CSeDayType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	DisableCntControls();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeDayType::ViewCntEdt(INT nDay)
{
	const INT cnt = 12;
	UINT id[ cnt ];
	GetEdtID( id );
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( id[ nDay+5 ] );
	pEdt->EnableWindow( true );
	pEdt->SetWindowText( _T("1") );
}

VOID CSeDayType::OnChangeedtDAY1() 
{
	ViewCntEdt(1);
}

VOID CSeDayType::OnChangeedtDAY2() 
{
	ViewCntEdt(2);	
}

VOID CSeDayType::OnChangeedtDAY3() 
{
	ViewCntEdt(3);
}

VOID CSeDayType::OnChangeedtDAY4() 
{
	ViewCntEdt(4);
}

VOID CSeDayType::OnChangeedtDAY5() 
{
	ViewCntEdt(5);
}

VOID CSeDayType::OnChangeedtDAY6() 
{
	ViewCntEdt(6);
}

VOID CSeDayType::OnKillfocusedtDAY1() 
{
	DisableCntEdt(1);
}

VOID CSeDayType::OnKillfocusedtDAY2() 
{
	DisableCntEdt(2);
}

VOID CSeDayType::OnKillfocusedtDAY3() 
{
	DisableCntEdt(3);
}

VOID CSeDayType::OnKillfocusedtDAY4() 
{
	DisableCntEdt(4);
}

VOID CSeDayType::OnKillfocusedtDAY5() 
{
	DisableCntEdt(5);
}

VOID CSeDayType::OnKillfocusedtDAY6() 
{
	DisableCntEdt(6);
}

BOOL CSeDayType::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
		//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
		//KOL.RED.019-010 JY 연속간행물 기능 안정화
		if( pMsg->wParam == VK_RETURN )
		{
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSeDayType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
