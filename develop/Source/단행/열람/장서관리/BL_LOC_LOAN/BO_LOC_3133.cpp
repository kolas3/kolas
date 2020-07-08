#include "stdafx.h"
#include "BO_LOC_3133.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3133::CBO_LOC_3133( CString date, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_LOC_3133::IDD, pParent)
{
	m_defaultDate = date;	
	m_tTime = 0;
	
}

CBO_LOC_3133::~CBO_LOC_3133()
{

}
VOID CBO_LOC_3133::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3133)
	DDX_MonthCalCtrl(pDX, IDC_LOAN_MONTHCALENDAR1, m_tTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3133, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3133)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




VOID CBO_LOC_3133::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
		ShowDefaultLoanStopDate();
}

VOID CBO_LOC_3133::ShowDefaultLoanStopDate()
{
	INT y, m, d;
	_stscanf( m_defaultDate.GetBuffer( 0 ), _T("%d/%d/%d"), &y, &m, &d );
	m_tTime = CTime( y, m, d, 0, 0, 0 );
	
	UpdateData( false );
}

VOID CBO_LOC_3133::OnOK() 
{
	SYSTEMTIME sysTime;
	CMonthCalCtrl *pCal = ( CMonthCalCtrl * )GetDlgItem( IDC_LOAN_MONTHCALENDAR1 );
	pCal->GetCurSel( &sysTime );
	
	m_sLoanStopDate.Format( _T("%04d/%02d/%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay );

	CDialog::OnOK();
}

BOOL CBO_LOC_3133::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
	              
}

HBRUSH CBO_LOC_3133::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
