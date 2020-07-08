// SeExtType.cpp : implementation file
//

#include "stdafx.h"
#include "SeExtType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExtType dialog


CSeExtType::CSeExtType(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExtType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExtType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSeExtType::~CSeExtType()
{
}


VOID CSeExtType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExtType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExtType, CDialog)
	//{{AFX_MSG_MAP(CSeExtType)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExtType message handlers
BOOL CSeExtType::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CSeExtType::GetAllData(CString data[])
{
	const INT cnt = 4;
	UINT id[ cnt ];
	GetEdtID( id );

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}

}


VOID CSeExtType::GetEdtID(UINT ID[])
{
	ID[  0 ] = IDC_edtEXT1;
	ID[  1 ] = IDC_edtEXT2;
	ID[  2 ] = IDC_edtEXT3;
	ID[  3 ] = IDC_edtEXT4;
}

VOID CSeExtType::SetAllData(CString data[])
{
	const INT cnt = 4;
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

VOID CSeExtType::ClearAllData()
{
	const INT cnt = 4;
	UINT id[ cnt ];
	GetEdtID( id );
	CEdit *pEdt;
	
	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->SetWindowText(_T(""));
	}

}



BOOL CSeExtType::PreTranslateMessage(MSG* pMsg) 
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

HBRUSH CSeExtType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSeExtType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
