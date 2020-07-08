// BlLocProgressBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlLocProgressBarDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlLocProgressBarDlg dialog


CBlLocProgressBarDlg::CBlLocProgressBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlLocProgressBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlLocProgressBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBlLocProgressBarDlg::~CBlLocProgressBarDlg()
{

}
VOID CBlLocProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlLocProgressBarDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBlLocProgressBarDlg, CDialog)
	//{{AFX_MSG_MAP(CBlLocProgressBarDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlLocProgressBarDlg message handlers

BOOL CBlLocProgressBarDlg::Create( CWnd* pParentWnd ) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBlLocProgressBarDlg::InitRange( INT nRange )
{
	EFS_BEGIN

	m_ProgressBar.SetRange( 0 , nRange );
	m_ProgressBar.SetPos( 0 );
	SetWindowText( _T("진행상태 - 0%") );


	EFS_END
}

VOID CBlLocProgressBarDlg::ChangeProgressBar( INT nRange )
{
	EFS_BEGIN

	INT nUpper, nLower;
	m_ProgressBar.GetRange( nLower , nUpper );

	if( nRange <= 0 ) nRange = 1;
	INT nPos = ((nRange*100)/nUpper);
	m_ProgressBar.SetPos( nRange );
	
	CString sPos;
	sPos.Format( _T("진행상태 - %d%%") , nPos  );
	SetWindowText( sPos );

	EFS_END
}

BOOL CBlLocProgressBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBlLocProgressBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
