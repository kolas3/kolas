// Group_Delete_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Group_Delete_Dlg.h"
#include "../INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroup_Delete_Dlg dialog


CGroup_Delete_Dlg::CGroup_Delete_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CGroup_Delete_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroup_Delete_Dlg)
	//}}AFX_DATA_INIT
	m_nOpType = -1 ;
	m_nDelType = -1;

	m_iGroupBookSumCnt = -1;
}

CGroup_Delete_Dlg::~CGroup_Delete_Dlg()
{
}

VOID CGroup_Delete_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroup_Delete_Dlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroup_Delete_Dlg, CDialog)
	//{{AFX_MSG_MAP(CGroup_Delete_Dlg)
	ON_BN_CLICKED(IDC_radGROUP_INDATA, OnradGROUPINDATA)
	ON_BN_CLICKED(IDC_radGROUP_ONLY, OnradGROUPONLY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup_Delete_Dlg message handlers

BOOL CGroup_Delete_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	INT nOptype = (ACQ_TYPE)m_nOpType;
	if(nOptype == ACQ_TYPE_INVALID)
	{
		GetDlgItem(IDC_radGROUP_INDATA)->SetWindowText(_T("그룹내 자료도 삭제"));
		if( m_iGroupBookSumCnt == 0 )
		{
			((CEdit*)GetDlgItem(IDC_radGROUP_ONLY))->EnableWindow(FALSE);
		}
		else
		{
			((CEdit*)GetDlgItem(IDC_radGROUP_ONLY))->EnableWindow(TRUE);
			GetDlgItem(IDC_radGROUP_ONLY)->SetWindowText(_T("그룹정보만 삭제"));
		}
	}
	else if(nOptype == PURCHASE)
	{
		GetDlgItem(IDC_radGROUP_INDATA)->SetWindowText(_T("차수내 자료도 삭제"));
		GetDlgItem(IDC_radGROUP_ONLY)->SetWindowText(_T("다른차수로 편입"));
	}
	else if(nOptype == DONATE)
	{
		GetDlgItem(IDC_radGROUP_INDATA)->SetWindowText(_T("접수번호내 자료도 삭제"));
		GetDlgItem(IDC_radGROUP_ONLY)->SetWindowText(_T("다른접수번호로 편입"));
	}
	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CGroup_Delete_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

VOID CGroup_Delete_Dlg::OnradGROUPINDATA() 
{
	// TODO: Add your control notification handler code here
	m_nDelType = 0;
}

VOID CGroup_Delete_Dlg::OnradGROUPONLY() 
{
	// TODO: Add your control notification handler code here
	m_nDelType = 1;
}

HBRUSH CGroup_Delete_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
