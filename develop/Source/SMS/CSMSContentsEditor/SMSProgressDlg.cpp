// SMSProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMSProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSProgressDlg dialog


CSMSProgressDlg::CSMSProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSMSProgressDlg::~CSMSProgressDlg()
{
}


VOID CSMSProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSProgressDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSProgressDlg message handlers

VOID CSMSProgressDlg::FinishDlg()
{
	CDialog::OnOK();
}


VOID CSMSProgressDlg::SetProgress( INT cnt, INT total )
{
	CString titleMsg;
	titleMsg.Format( _T(" %d 건 중 %d 번째 문자 발송중..."), total, cnt );
	this->SetWindowText( titleMsg );

	m_progress.SetPos( ( cnt * 100 ) / total );
}

BOOL CSMSProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_progress.SetRange( 0, 100 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSMSProgressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

