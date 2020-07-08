// SMSTypeGetter.cpp : implementation file
//

#include "stdafx.h"
#include "SMSTypeGetter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSTypeGetter dialog


CSMSTypeGetter::CSMSTypeGetter(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSTypeGetter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSTypeGetter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSMSTypeGetter::~CSMSTypeGetter()
{	
}


VOID CSMSTypeGetter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSTypeGetter)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSTypeGetter, CDialog)
	//{{AFX_MSG_MAP(CSMSTypeGetter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSTypeGetter message handlers

VOID CSMSTypeGetter::OnOK() 
{
	// TODO: Add extra validation here
	
	CButton *pBtn_7 = ( CButton * )GetDlgItem( IDC_radSMS03_1 );
	CButton *pBtn_8 = ( CButton * )GetDlgItem( IDC_radSMS03_2 );

	if( pBtn_7->GetCheck() )
		m_SMSType = _T("SMS07");
	else if( pBtn_8->GetCheck() )
		m_SMSType = _T("SMS08");
	else
	{
		AfxMessageBox( _T("문자종류를 선택하여 주십시요") );
		return;
	}
	
	CDialog::OnOK();
}


HBRUSH CSMSTypeGetter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}