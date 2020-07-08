// KL_CryptoDescPage.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoDescPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoDescPage dialog
IMPLEMENT_DYNAMIC(CKL_CryptoDescPage, CPropertyPage)

CKL_CryptoDescPage::CKL_CryptoDescPage()
	: CPropertyPage(CKL_CryptoDescPage::IDD)
{
	//{{AFX_DATA_INIT(CKL_CryptoDescPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKL_CryptoDescPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoDescPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_CryptoDescPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKL_CryptoDescPage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoDescPage message handlers

/*=============================================================
	@ Function    :	OnCtlColor
	@ Description :
=============================================================*/
HBRUSH CKL_CryptoDescPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_STRESS:
		pDC->SetTextColor(RGB(0x00, 0x00, 0xFF));
		break;
	case IDC_STATIC_WARN:
		pDC->SetTextColor(RGB(0xFF, 0x00, 0x00));
		break;
	case IDC_STATIC_INFO:
		pDC->SetTextColor(RGB(0x50, 0x50, 0x50));
		break;
	}

	return hbr;
}
