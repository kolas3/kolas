// LocProgressBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LocProgressBarDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocProgressBarDlg dialog


CLocProgressBarDlg::CLocProgressBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLocProgressBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocProgressBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLocProgressBarDlg::~CLocProgressBarDlg()
{
}

VOID CLocProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocProgressBarDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLocProgressBarDlg, CDialog)
	//{{AFX_MSG_MAP(CLocProgressBarDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocProgressBarDlg message handlers

BOOL CLocProgressBarDlg::Create( CWnd* pParentWnd ) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CLocProgressBarDlg::InitRange( INT nRange )
{
	EFS_BEGIN

	m_ProgressBar.SetRange( 0 , nRange );
	m_ProgressBar.SetPos( 0 );
	SetWindowText( _T("진행상태 - 0%") );


	EFS_END
}

VOID CLocProgressBarDlg::ChangeProgressBar( INT nRange )
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

BOOL CLocProgressBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CLocProgressBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
