// DonatorNameGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetterDlg dialog


CDonatorNameGetterDlg::CDonatorNameGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorNameGetterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
}


VOID CDonatorNameGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetterDlg message handlers

BOOL CDonatorNameGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("K2UPGRADE_기증자검색결과") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
		return false;
	}
	m_pCM = FindCM(_T("CM_기증자검색결과"));
	m_pDM = FindDM(_T("DM_기증자조회"));	
	
	m_pCM->ControlDisplay(_T("MainGrid"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonatorNameGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	if(ids < 0 || i < 0)  
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	i = pGrid->SelectGetCount();
	if(i > 1)
		ESLAfxMessageBox(_T("처음 선정된 자료만 반영됩니다."));
	
	m_pDM->GetCellData(_T("REC_KEY"), i, m_strDonatorKey);
	m_pDM->GetCellData(_T("DONATOR"), i, m_strDonatorName);

	CDialog::OnOK();
}
