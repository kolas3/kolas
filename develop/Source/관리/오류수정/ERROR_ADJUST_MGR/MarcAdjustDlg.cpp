// MarcAdjustDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjustDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjustDlg dialog


CMarcAdjustDlg::CMarcAdjustDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMarcAdjustDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcAdjustDlg)
	m_bCurrencyCode = FALSE;
	m_bPubPlace = FALSE;
	m_bCopyCode = FALSE;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pCond = NULL;
}
CMarcAdjustDlg::~CMarcAdjustDlg()
{	
	
}


VOID CMarcAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAdjustDlg)
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	DDX_Check(pDX, IDC_chkPUBLISH_PLACE, m_bPubPlace);
	DDX_Check(pDX, IDC_chkRESTRUCT_COPY_CODE, m_bCopyCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcAdjustDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcAdjustDlg)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjustDlg message handlers

BOOL CMarcAdjustDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_마크수정화면")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_pCM = FindCM(_T("CM_마크수정화면"));

	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcAdjustDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CString strData, strTag;
	if(m_pCond == NULL) return;
	if(m_bCopyCode)
	{
		strTag = _T("049");		strData.Empty();
		m_pCond->AddMarcInfo(strTag, strData, REPLACE);
	}
	if(m_bCurrencyCode)
	{
		strTag = _T("950$b");
		m_pCM->GetControlMgrData(_T("화폐구분"), strData);
		m_pCond->AddMarcInfo(strTag, strData, REPLACE);
	}
	if(m_bPubPlace)
	{
		strTag = _T("260$a");
		m_pCM->GetControlMgrData(_T("발행지"), strData);
		m_pCond->AddMarcInfo(strTag, strData, REPLACE);
	}
	
	CDialog::OnOK();
}

VOID CMarcAdjustDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



HBRUSH CMarcAdjustDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
