// AccessionNoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessionNoSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessionNoSearchDlg dialog


CAccessionNoSearchDlg::CAccessionNoSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CAccessionNoSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessionNoSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
}
CAccessionNoSearchDlg::~CAccessionNoSearchDlg()
{
	  
}


VOID CAccessionNoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessionNoSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessionNoSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessionNoSearchDlg)
	ON_BN_CLICKED(IDC_btnACCNO_SEARCH, OnbtnACCNOSEARCH)
	ON_WM_CTLCOLOR()    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessionNoSearchDlg message handlers

BOOL CAccessionNoSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_재활용자료_원부검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_재활용자료_원부검색"));
	m_pDM = FindDM(_T("DM_재활용자료_원부검색"));	
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessionNoSearchDlg::OnbtnACCNOSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strYear, strMaker;
	m_pCM->GetControlMgrData(_T("원부생성년도"), strYear);
	m_pCM->GetControlMgrData(_T("원부생성자"), strMaker);

	if(strYear.IsEmpty())
	{
		ESLAfxMessageBox(_T("원부생성년도는 입력하여야 합니다."));
		return;
	}

	CString strQuery, strOption;
	strQuery.Format(_T("A.ACCESSION_REC_MAKE_YEAR = '%s'"), strYear);
	if(!strMaker.IsEmpty())
	{
		strOption.Format(_T(" AND A.ACCESSION_REC_MAKER = '%s'"), strMaker);
		strQuery += strOption;
	}

	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("원부정보를 검색할 수 없습니다."));
		return;
	}
	m_pGrid->Display();

}

VOID CAccessionNoSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정정보를 추출할 수 없습니다."));
		return;
	}
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	m_strAccessionRecKey = m_pDM->GetCellData(_T("REC_KEY"), ids);
	m_strAccessionNo = m_pDM->GetCellData(_T("원부번호"), ids);
	
	CDialog::OnOK();
}


HBRUSH CAccessionNoSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

