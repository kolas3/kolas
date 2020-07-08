// BO_LOC_3430.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3430.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3430 dialog


CBO_LOC_3430::CBO_LOC_3430(CESL_Mgr* pParent, CLonaManageValue* pLoanManageValue)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3430)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoanManageValue = pLoanManageValue;
	m_pSpfWork = NULL;
}

CBO_LOC_3430::~CBO_LOC_3430()
{

}

VOID CBO_LOC_3430::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3430)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3430, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3430)
	ON_BN_CLICKED(IDC_btnDRECEIPT, OnbtnDRECEIPT)
	ON_BN_CLICKED(IDC_btn3400_DCLOSE, OnbtnDCLOSE)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3430 message handlers

BOOL CBO_LOC_3430::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3430::OnClose() 
{
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;

	m_pSpfWork = NULL;

	CDialog::OnClose();
}

BOOL CBO_LOC_3430::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3430")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_BO_LOC_34( this, m_pLoanManageValue );

	DisplaySelectCnt();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CBO_LOC_3430::DisplaySelectCnt()
{
	INT nSelectCnt = m_pSpfWork->GetSelectMainGridCnt();

	CString sSelectCnt;
	sSelectCnt.Format( _T("%d") , nSelectCnt );

	GetDlgItem(IDC_edtREPAIR_SPECIES)->SetWindowText(sSelectCnt);

	return 0;
}

VOID CBO_LOC_3430::OnbtnDRECEIPT() 
{
	m_pSpfWork->SPFExcutePath( RECEIPT );	
}

VOID CBO_LOC_3430::OnbtnDCLOSE() 
{
	OnCancel();	
}



HBRUSH CBO_LOC_3430::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
