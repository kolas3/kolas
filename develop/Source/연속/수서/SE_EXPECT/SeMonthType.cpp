// SeMonthType.cpp : implementation file
//

#include "stdafx.h"
#include "SeMonthType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeMonthType dialog


CSeMonthType::CSeMonthType(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeMonthType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeMonthType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

CSeMonthType::~CSeMonthType()
{
}

VOID CSeMonthType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeMonthType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeMonthType, CDialog)
	//{{AFX_MSG_MAP(CSeMonthType)
	ON_EN_CHANGE(IDC_edtMONTH1, OnChangeedtMONTH1)
	ON_EN_CHANGE(IDC_edtMONTH2, OnChangeedtMONTH2)
	ON_EN_CHANGE(IDC_edtMONTH3, OnChangeedtMONTH3)
	ON_EN_CHANGE(IDC_edtMONTH4, OnChangeedtMONTH4)
	ON_EN_CHANGE(IDC_edtMONTH5, OnChangeedtMONTH5)
	ON_EN_CHANGE(IDC_edtMONTH6, OnChangeedtMONTH6)
	ON_EN_CHANGE(IDC_edtMONTH7, OnChangeedtMONTH7)
	ON_EN_CHANGE(IDC_edtMONTH8, OnChangeedtMONTH8)
	ON_EN_CHANGE(IDC_edtMONTH9, OnChangeedtMONTH9)
	ON_EN_CHANGE(IDC_edtMONTH10, OnChangeedtMONTH10)
	ON_EN_CHANGE(IDC_edtMONTH11, OnChangeedtMONTH11)
	ON_EN_CHANGE(IDC_edtMONTH12, OnChangeedtMONTH12)
	ON_EN_KILLFOCUS(IDC_edtMONTH1, OnKillfocusedtMONTH1)
	ON_EN_KILLFOCUS(IDC_edtMONTH2, OnKillfocusedtMONTH2)
	ON_EN_KILLFOCUS(IDC_edtMONTH3, OnKillfocusedtMONTH3)
	ON_EN_KILLFOCUS(IDC_edtMONTH4, OnKillfocusedtMONTH4)
	ON_EN_KILLFOCUS(IDC_edtMONTH5, OnKillfocusedtMONTH5)
	ON_EN_KILLFOCUS(IDC_edtMONTH6, OnKillfocusedtMONTH6)
	ON_EN_KILLFOCUS(IDC_edtMONTH7, OnKillfocusedtMONTH7)
	ON_EN_KILLFOCUS(IDC_edtMONTH8, OnKillfocusedtMONTH8)
	ON_EN_KILLFOCUS(IDC_edtMONTH9, OnKillfocusedtMONTH9)
	ON_EN_KILLFOCUS(IDC_edtMONTH10, OnKillfocusedtMONTH10)
	ON_EN_KILLFOCUS(IDC_edtMONTH11, OnKillfocusedtMONTH11)
	ON_EN_KILLFOCUS(IDC_edtMONTH12, OnKillfocusedtMONTH12)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeMonthType message handlers
BOOL CSeMonthType::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CSeMonthType::GetEdtID(UINT id[])
{
	id[ 0 ] = IDC_edtMONTH1;
	id[ 1 ] = IDC_edtMONTH2;
	id[ 2 ] = IDC_edtMONTH3;
	id[ 3 ] = IDC_edtMONTH4;
	id[ 4 ] = IDC_edtMONTH5;
	id[ 5 ] = IDC_edtMONTH6;
	id[ 6 ] = IDC_edtMONTH7;
	id[ 7 ] = IDC_edtMONTH8;
	id[ 8 ] = IDC_edtMONTH9;
	id[ 9 ] = IDC_edtMONTH10;
	id[ 10 ] = IDC_edtMONTH11;
	id[ 11 ] = IDC_edtMONTH12;
	id[ 12 ] = IDC_edtMONTH13;
	id[ 13 ] = IDC_edtMONTH14;
	id[ 14 ] = IDC_edtMONTH15;
	id[ 15 ] = IDC_edtMONTH16;
	id[ 16 ] = IDC_edtMONTH17;
	id[ 17 ] = IDC_edtMONTH18;
	id[ 18 ] = IDC_edtMONTH19;
	id[ 19 ] = IDC_edtMONTH20;
	id[ 20 ] = IDC_edtMONTH21;
	id[ 21 ] = IDC_edtMONTH22;
	id[ 22 ] = IDC_edtMONTH23;
	id[ 23 ] = IDC_edtMONTH24;

}

VOID CSeMonthType::GetMonEdtID(UINT id[])
{

	id[ 0 ] = IDC_edtMONTH1;
	id[ 1 ] = IDC_edtMONTH2;
	id[ 2 ] = IDC_edtMONTH3;
	id[ 3 ] = IDC_edtMONTH4;
	id[ 4 ] = IDC_edtMONTH5;
	id[ 5 ] = IDC_edtMONTH6;
	id[ 6 ] = IDC_edtMONTH7;
	id[ 7 ] = IDC_edtMONTH8;
	id[ 8 ] = IDC_edtMONTH9;
	id[ 9 ] = IDC_edtMONTH10;
	id[ 10 ] = IDC_edtMONTH11;
	id[ 11 ] = IDC_edtMONTH12;
}

VOID CSeMonthType::GetCntEdtID(UINT id[])
{

	id[ 0  ] = IDC_edtMONTH13;
	id[ 1  ] = IDC_edtMONTH14;
	id[ 2  ] = IDC_edtMONTH15;
	id[ 3  ] = IDC_edtMONTH16;
	id[ 4  ] = IDC_edtMONTH17;
	id[ 5  ] = IDC_edtMONTH18;
	id[ 6  ] = IDC_edtMONTH19;
	id[ 7  ] = IDC_edtMONTH20;
	id[ 8  ] = IDC_edtMONTH21;
	id[ 9  ] = IDC_edtMONTH22;
	id[ 10 ] = IDC_edtMONTH23;
	id[ 11 ] = IDC_edtMONTH24;

}

VOID CSeMonthType::GetAllData(CString data[])
{
	const INT cnt = 24;
	UINT id[ cnt ];
	GetEdtID( id );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}

}

VOID CSeMonthType::SetAllData(CString data[])
{
	const INT cnt = 24;
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

VOID CSeMonthType::ClearAllData()
{
	const INT cnt = 24;
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

VOID CSeMonthType::DisableCntControls()
{
	
	const INT cnt = 12;
	UINT ID[ cnt ];
	GetCntEdtID( ID );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( ID[ i ] );
		pEdt->EnableWindow( false );
	}

}

BOOL CSeMonthType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	EnableThemeDialogTexture(GetSafeHwnd()); 
	//부수항목을 비활성화
	DisableCntControls();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeMonthType::CopyToAllEdt()
{
	const INT cnt = 12;

	UINT monID[ cnt ];
	UINT cntID[ cnt ];
	GetMonEdtID( monID );
	GetCntEdtID( cntID );
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( monID[ 0 ] );

	CString value, tmp;
	pEdt->GetWindowText( value );

	for( INT i = 0; i < cnt; i++ )
	{
		if( i )
		{
			pEdt = ( CEdit * )GetDlgItem( monID[ i ] );
			pEdt->GetWindowText( tmp );
			pEdt->SetWindowText( value );
			pEdt->EnableWindow( true );
		}

		pEdt = ( CEdit * )GetDlgItem( cntID[ i ] );
		pEdt->GetWindowText( tmp );
		if( !tmp.GetLength() )
			pEdt->SetWindowText( _T("1") );
		pEdt->EnableWindow( true );
	}
	
}

VOID CSeMonthType::ViewCntEdt(INT nMonth)
{
	const INT cnt = 12;
	UINT cntID[ cnt ];
	GetCntEdtID( cntID );
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( cntID[ nMonth-1 ] );
	pEdt->EnableWindow( true );
	pEdt->SetWindowText( _T("1") );
}


VOID CSeMonthType::DisableCntEdt(INT nMonth)
{
	const INT cnt = 12;
	
	UINT monID[ cnt ];
	GetMonEdtID( monID );
	
	UINT cntID[ cnt ];
	GetCntEdtID( cntID );
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( monID[ nMonth-1 ] );

	CString tmp;
	pEdt->GetWindowText(tmp);
	if(!tmp.IsEmpty()) return;
	

	pEdt = ( CEdit * )GetDlgItem( cntID[ nMonth-1 ] );
	pEdt->SetWindowText( _T("") );
	pEdt->EnableWindow( false );
}


VOID CSeMonthType::OnChangeedtMONTH1() 
{
	
	ViewCntEdt(1);

	//1월의 경우는 간행빈도를 파악해서 월간일 경우 다른 에디트에 모두 복사
	if('m' == m_charPUB_FREQ) CopyToAllEdt();
	
}

VOID CSeMonthType::OnChangeedtMONTH2() 
{
	ViewCntEdt(2);
}

VOID CSeMonthType::OnChangeedtMONTH3() 
{
	ViewCntEdt(3);
}

VOID CSeMonthType::OnChangeedtMONTH4() 
{
	ViewCntEdt(4);
}

VOID CSeMonthType::OnChangeedtMONTH5() 
{
	ViewCntEdt(5);
}

VOID CSeMonthType::OnChangeedtMONTH6() 
{
	ViewCntEdt(6);
}

VOID CSeMonthType::OnChangeedtMONTH7() 
{
	ViewCntEdt(7);
}

VOID CSeMonthType::OnChangeedtMONTH8() 
{
	ViewCntEdt(8);
}

VOID CSeMonthType::OnChangeedtMONTH9() 
{
	ViewCntEdt(9);
}

VOID CSeMonthType::OnChangeedtMONTH10() 
{
	ViewCntEdt(10);
}

VOID CSeMonthType::OnChangeedtMONTH11() 
{
	ViewCntEdt(11);
}

VOID CSeMonthType::OnChangeedtMONTH12() 
{
	ViewCntEdt(12);
}

VOID CSeMonthType::OnKillfocusedtMONTH1() 
{
	DisableCntEdt(1);
	
}

VOID CSeMonthType::OnKillfocusedtMONTH2() 
{
	DisableCntEdt(2);
}

VOID CSeMonthType::OnKillfocusedtMONTH3() 
{
	DisableCntEdt(3);
}

VOID CSeMonthType::OnKillfocusedtMONTH4() 
{
	DisableCntEdt(4);
}

VOID CSeMonthType::OnKillfocusedtMONTH5() 
{
	DisableCntEdt(5);
}

VOID CSeMonthType::OnKillfocusedtMONTH6() 
{
	DisableCntEdt(6);
}

VOID CSeMonthType::OnKillfocusedtMONTH7() 
{
	DisableCntEdt(7);
}

VOID CSeMonthType::OnKillfocusedtMONTH8() 
{
	DisableCntEdt(8);
}

VOID CSeMonthType::OnKillfocusedtMONTH9() 
{
	DisableCntEdt(9);
}

VOID CSeMonthType::OnKillfocusedtMONTH10() 
{
	DisableCntEdt(10);
}

VOID CSeMonthType::OnKillfocusedtMONTH11() 
{
	DisableCntEdt(11);
}

VOID CSeMonthType::OnKillfocusedtMONTH12() 
{
	DisableCntEdt(12);
}

BOOL CSeMonthType::PreTranslateMessage(MSG* pMsg) 
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

HBRUSH CSeMonthType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
