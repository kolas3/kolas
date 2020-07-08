// TransferProgress.cpp : implementation file
//

#include "stdafx.h"
#include "TransferProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferProgress dialog


CTransferProgress::CTransferProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CTransferProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTransferProgress::~CTransferProgress()
{
}

VOID CTransferProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferProgress)
	DDX_Control(pDX, IDC_prgTransfer, m_ProgressBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferProgress, CDialog)
	//{{AFX_MSG_MAP(CTransferProgress)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferProgress message handlers
BOOL CTransferProgress::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	return FALSE;

}

VOID CTransferProgress::InitRange( INT nRange )
{
	m_ProgressBar.SetRange( 0 , nRange );
	m_ProgressBar.SetPos( 0 );
	SetWindowText( _T("진행상태 - 0%") );
}

VOID CTransferProgress::ChangeProgressBar( INT nRange )
{
	INT nUpper, nLower;
	m_ProgressBar.GetRange( nLower , nUpper );

	if( nRange <= 0 ) nRange = 1;
	INT nPos = ((nRange*100)/nUpper);
	m_ProgressBar.SetPos( nRange );
	
	CString sPos;
	sPos.Format( _T("진행상태 - %d%%") , nPos  );
	SetWindowText( sPos );

}

HBRUSH CTransferProgress::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CTransferProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
