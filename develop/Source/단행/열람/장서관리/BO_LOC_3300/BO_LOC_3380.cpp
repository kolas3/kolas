// BO_LOC_3380.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3380.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3380 dialog


CBO_LOC_3380::CBO_LOC_3380(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3380)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMode = 0;
	m_nCloseMode = 0;
}

CBO_LOC_3380::~CBO_LOC_3380()
{

}

VOID CBO_LOC_3380::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3380)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3380, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3380)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnFILEMANAGE, OnbtnFILEMANAGE)
	ON_BN_CLICKED(IDC_radINPUT, OnradINPUT)
	ON_BN_CLICKED(IDC_radOUTPUT, OnradOUTPUT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3380 message handlers

BOOL CBO_LOC_3380::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd());
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAIN);

	TC_ITEM item;

	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;

	item.pszText = _T("반입/반출 선택  ");
	pTab->InsertItem(0, &item);

	m_nMode = 0;
	m_nCloseMode = 0;
	((CButton*)(GetDlgItem( IDC_radINPUT )))->SetCheck(1);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3380::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_3380::OnbtnFILEMANAGE() 
{
	EFS_BEGIN

	m_nCloseMode = 1;
	OnCancel();

	EFS_END
}


INT CBO_LOC_3380::GetRunMode()
{
	EFS_BEGIN

	return m_nMode;

	EFS_END
	return -1;

}

INT CBO_LOC_3380::GetCloseMode()
{
	EFS_BEGIN

	return m_nCloseMode;

	EFS_END
	return -1;

}

VOID CBO_LOC_3380::OnradINPUT() 
{
	EFS_BEGIN

	m_nMode = 0;

	EFS_END
}

VOID CBO_LOC_3380::OnradOUTPUT() 
{
	EFS_BEGIN

	m_nMode = 1;	

	EFS_END
}

HBRUSH CBO_LOC_3380::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
