// SE_LOC_1420.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1420.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1420 dialog


CSE_LOC_1420::CSE_LOC_1420(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1420)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1420::~CSE_LOC_1420()
{
}

VOID CSE_LOC_1420::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1420)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1420, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1420)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1420 message handlers

BOOL CSE_LOC_1420::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_LOC_1420::OnClose() 
{
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;

	m_pSpfWork = NULL;

	CDialog::OnClose();
}

BOOL CSE_LOC_1420::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1420")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_SE_LOC_1400( this );

	DisplaySelectCnt();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1420::OnbtnSAVE() 
{
	m_pSpfWork->SPFExcutePath( REG_REPAIR );	
}

VOID CSE_LOC_1420::OnbtnCLOSE() 
{
	OnCancel();	
}


INT CSE_LOC_1420::DisplaySelectCnt()
{
	INT nSelectCnt = m_pSpfWork->GetSelectMainGridCnt();

	CString sSelectCnt;
	sSelectCnt.Format( _T("%d") , nSelectCnt );

	GetDlgItem(IDC_datSELECT_BOOK_CNT)->SetWindowText(sSelectCnt);

	return 0;
}
HBRUSH CSE_LOC_1420::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
