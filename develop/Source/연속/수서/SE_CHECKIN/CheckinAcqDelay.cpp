// CheckinAcqDelay.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinAcqDelay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinAcqDelay dialog


CCheckinAcqDelay::CCheckinAcqDelay(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinAcqDelay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinAcqDelay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strTITLE = _T("");
	m_bIsMultiProc = TRUE;
	
}
CCheckinAcqDelay::~CCheckinAcqDelay()
{
}

VOID CCheckinAcqDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinAcqDelay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinAcqDelay, CDialog)
	//{{AFX_MSG_MAP(CCheckinAcqDelay)
	ON_BN_CLICKED(IDC_btnNEXT_ACQ_DELAY, OnbtnNEXTACQDELAY)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinAcqDelay message handlers

BOOL CCheckinAcqDelay::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_체크인_입수지연") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인_입수지연") );
		return false;
	}
	
	CESL_ControlMgr* pCM = FindCM(_T("CM_체크인_입수지연"));
	if(!pCM) 	{
		AfxMessageBox(_T("CM Error : CM_체크인_입수지연"));
		return FALSE;
	}		
	
	pCM->SetControlMgrData(_T("본서명"),m_strTITLE);
	
	if(!m_bIsMultiProc)
	{
		GetDlgItem(IDC_btnNEXT_ACQ_DELAY)->ShowWindow(FALSE);
	}

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
	
	if(pGrid) pGrid->Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CCheckinAcqDelay::OnbtnNEXTACQDELAY() 
{
	// TODO: Add your control notification handler code here
	EndDialog(11);
}

VOID CCheckinAcqDelay::OnButton1() 
{
	CESL_ControlMgr* pCM = FindCM(_T("CM_체크인_입수지연"));
	if(!pCM) 	{
		AfxMessageBox(_T("CM Error : CM_체크인_입수지연"));
		return;
	}	
	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));

	if( pGrid->GetCount() > 0 )
		pGrid->OnExcelExport();
	
}

HBRUSH CCheckinAcqDelay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
