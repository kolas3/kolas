// SeWeekType.cpp : implementation file
//

#include "stdafx.h"
#include "SeWeekType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeWeekType dialog


CSeWeekType::CSeWeekType(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeWeekType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeWeekType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSeWeekType::~CSeWeekType()
{
}


VOID CSeWeekType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeWeekType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeWeekType, CDialog)
	//{{AFX_MSG_MAP(CSeWeekType)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeWeekType message handlers
BOOL CSeWeekType::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CSeWeekType::GetEdtID(UINT id[])
{
	id[ 0 ] = IDC_edtWEEK1;
	id[ 1 ] = IDC_edtWEEK2;
	id[ 2 ] = IDC_edtWEEK3;
	id[ 3 ] = IDC_edtWEEK4;
	id[ 4 ] = IDC_edtWEEK5;
	id[ 5 ] = IDC_edtWEEK6;
	id[ 6 ] = IDC_edtWEEK7;

}

VOID CSeWeekType::GetAllData(CString data[])
{
	const INT cnt = 7;
	UINT id[ cnt ];
	GetEdtID( id );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}

}

VOID CSeWeekType::SetAllData(CString data[])
{
	const INT cnt = 7;
	UINT id[ cnt ];
	GetEdtID( id );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->SetWindowText( data[ i ] );
	}
}

VOID CSeWeekType::ClearAllData()
{
	const INT cnt = 7;
	UINT id[ cnt ];
	GetEdtID( id );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->SetWindowText(_T(""));
	}
}

BOOL CSeWeekType::PreTranslateMessage(MSG* pMsg) 
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
HBRUSH CSeWeekType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSeWeekType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
