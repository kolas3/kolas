// BO_LOC_1420.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1420.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1420 dialog


CBO_LOC_1420::CBO_LOC_1420(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1420)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSpfWork = NULL;
}

CBO_LOC_1420::~CBO_LOC_1420()
{	
	delete m_pSpfWork;
}

VOID CBO_LOC_1420::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1420)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1420, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1420)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1420 message handlers

BOOL CBO_LOC_1420::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_1420::OnClose() 
{
	CDialog::OnClose();
}

BOOL CBO_LOC_1420::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_1420")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_BO_LOC_1400( this );

	DisplaySelectCnt();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1420::OnbtnSAVE() 
{
	m_pSpfWork->SPFExcutePath( REG_REPAIR );	
}

VOID CBO_LOC_1420::OnbtnCLOSE() 
{
	OnCancel();	
}


INT CBO_LOC_1420::DisplaySelectCnt()
{
	INT nSelectCnt = m_pSpfWork->GetSelectMainGridCnt();

	CString sSelectCnt;
	sSelectCnt.Format( _T("%d") , nSelectCnt );

	GetDlgItem(IDC_datSELECT_BOOK_CNT)->SetWindowText(sSelectCnt);

	return 0;
}

HBRUSH CBO_LOC_1420::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
