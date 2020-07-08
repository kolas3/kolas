// DataExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataExportDlg dialog


CDataExportDlg::CDataExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDataExportDlg::IDD, pParent)
{
	m_pCM = NULL;
	m_strAcqYear.Empty();
	m_strWorkCode.Empty();
	m_strSeqNo.Empty();	
	//{{AFX_DATA_INIT(CDataExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CDataExportDlg::~CDataExportDlg()
{
}

VOID CDataExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataExportDlg, CDialog)
	//{{AFX_MSG_MAP(CDataExportDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataExportDlg message handlers

VOID CDataExportDlg::OnOK() 
{
	// TODO: Add extra validation here
	// TODO: Add extra validation here
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);

	m_pCM->GetControlMgrData(_T("업무구분"), m_strWorkCode);
	
	m_pCM->GetControlMgrData(_T("차수번호"), m_strSeqNo);
	
	CDialog::OnOK();
}

BOOL CDataExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_검수_자료반출")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_단행_검수_자료반출"));
	if(m_pCM == NULL)
		return FALSE;

	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("업무구분"));
	pCmb->SetCurSel(1);
	
	m_pCM->AllControlDisplay( );
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDataExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}