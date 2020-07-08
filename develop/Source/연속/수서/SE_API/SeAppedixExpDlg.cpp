// SeAppedixExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeAppedixExpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeAppedixExpDlg dialog


CSeAppedixExpDlg::CSeAppedixExpDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeAppedixExpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeAppedixExpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_strVOL_TITLE = _T("");
}
CSeAppedixExpDlg::~CSeAppedixExpDlg()
{
}

VOID CSeAppedixExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeAppedixExpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeAppedixExpDlg, CDialog)
	//{{AFX_MSG_MAP(CSeAppedixExpDlg)
	ON_BN_CLICKED(IDC_btnAPPENDIX_EXP, OnbtnAPPENDIXEXP)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeAppedixExpDlg message handlers

BOOL CSeAppedixExpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		if(InitESL_Mgr(_T("연속_수서_체크인_부록예측"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인_부록예측") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_체크인_부록예측"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_체크인_부록"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_체크인_부록예측") );
		return false;
	}	
	
	m_pCM->SetControlMgrData(_T("권호명"),m_strVOL_TITLE);
	INT nRowCnt = m_pDM->GetRowCount();
	CString msg;
	msg.Format(_T("총 [ %d ] 건의 부록 예측결과가 있습니다."),nRowCnt);
	GetDlgItem(IDC_stcAPPENDIX_EXP_RESULT)->SetWindowText(msg);
	m_pGrid->Display();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeAppedixExpDlg::OnbtnAPPENDIXEXP() 
{
	// TODO: Add your control notification handler code here
	// 일부 선택 
	m_pGrid->SelectMakeList();
	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0) {
		AfxMessageBox(_T("선택된 정보가 없습니다!"));
		return;
	}

	while(idx>=0)
	{
		m_pDM->SetCellData(_T("UDF_상태"),_T("I"),idx);
		idx = m_pGrid->SelectGetNext();
	}


	EndDialog(11);
}

VOID CSeAppedixExpDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 모두 선택 
	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		m_pDM->SetCellData(_T("UDF_상태"),_T("I"),i);
	}

	CDialog::OnOK();
}

VOID CSeAppedixExpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	// 선택 없음 
	
	CDialog::OnCancel();
}


HBRUSH CSeAppedixExpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
