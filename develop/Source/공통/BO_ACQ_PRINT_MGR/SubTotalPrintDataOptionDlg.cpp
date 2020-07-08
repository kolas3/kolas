// SubTotalPrintDataOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SubTotalPrintDataOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubTotalPrintDataOptionDlg dialog


CSubTotalPrintDataOptionDlg::CSubTotalPrintDataOptionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSubTotalPrintDataOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubTotalPrintDataOptionDlg)
	m_nPrintDataType = -1;
	//}}AFX_DATA_INIT
	m_bSubTotalPrint = FALSE;
}

CSubTotalPrintDataOptionDlg::~CSubTotalPrintDataOptionDlg()
{
}

VOID CSubTotalPrintDataOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubTotalPrintDataOptionDlg)
	DDX_Radio(pDX, IDC_radRESEARCH, m_nPrintDataType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubTotalPrintDataOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CSubTotalPrintDataOptionDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubTotalPrintDataOptionDlg message handlers

BOOL CSubTotalPrintDataOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nPrintDataType = 1;

	UpdateData(FALSE);

	if( m_bSubTotalPrint == FALSE )		GetDlgItem(IDC_radRESEARCH)->EnableWindow(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
}

VOID CSubTotalPrintDataOptionDlg::OnOK() 
{
	UpdateData(TRUE);

	if(m_nPrintDataType < 0) 
	{
		MessageBox(_T("출력 데이터 형식을 선정해 주십시요."));
		return ;
	}

	CDialog::OnOK();
}

HBRUSH CSubTotalPrintDataOptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
