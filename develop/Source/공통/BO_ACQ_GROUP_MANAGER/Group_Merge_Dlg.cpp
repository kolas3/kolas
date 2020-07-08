// Group_Merge_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Group_Merge_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroup_Merge_Dlg dialog


CGroup_Merge_Dlg::CGroup_Merge_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CGroup_Merge_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroup_Merge_Dlg)
	m_nMergeType = -1;
	//}}AFX_DATA_INIT
	m_parrName = NULL;
	m_parrKey = NULL;
}

CGroup_Merge_Dlg::~CGroup_Merge_Dlg()
{
}

VOID CGroup_Merge_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroup_Merge_Dlg)
	DDX_Radio(pDX, IDC_radGROUP_NEW, m_nMergeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroup_Merge_Dlg, CDialog)
	//{{AFX_MSG_MAP(CGroup_Merge_Dlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup_Merge_Dlg message handlers

BOOL CGroup_Merge_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nMergeType = 0;
	InitCombo();
	ShowCtrlByMergeType();
	UpdateData(FALSE);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CGroup_Merge_Dlg::ShowCtrlByMergeType()
{
	if(m_nMergeType == 0)
		GetDlgItem(IDC_cmbGROUP_NAME_LIST)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_cmbGROUP_NAME_LIST)->EnableWindow(TRUE);
}

VOID CGroup_Merge_Dlg::InitCombo()
{
	if(!m_parrKey || !m_parrName) return;
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_cmbGROUP_NAME_LIST);
	if(!pCmb) return;

	INT cnt = m_parrName->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		pCmb->AddString(m_parrName->GetAt(i));
	}
	pCmb->SetCurSel(0);
}

VOID CGroup_Merge_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CESL_DataMgr * pDM = FindDM(_T("DM_그룹관리"));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)pDM);
	mgr.SetTempPtr(_T("그룹이름리스트"), (DWORD*)m_parrName);
	mgr.SetTempPtr(_T("그룹KEY리스트"), (DWORD*)m_parrKey);
	mgr.SetTempData(_T("통합종류"), m_nMergeType);
	CComboBox *pCmb = (CComboBox*)GetDlgItem(IDC_cmbGROUP_NAME_LIST);
	INT idx = pCmb->GetCurSel();
	mgr.SetTempData(_T("선정인덱스"), idx);
	INT ids = mgr.SPFExecutePath(_T("그룹통합"));
	if(ids < 0)
	{
		return;
	}
	CDialog::OnOK();
}

HBRUSH CGroup_Merge_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
