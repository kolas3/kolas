// SelectComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "SelectComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_ACCESS_CONTROL_1000.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectComboBox

CSelectComboBox::CSelectComboBox()
{
	m_nMSG = 0;
	m_nSelectedRow = -1;
}

CSelectComboBox::~CSelectComboBox()
{
}


BEGIN_MESSAGE_MAP(CSelectComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSelectComboBox)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectComboBox message handlers

INT CSelectComboBox::AddMSG(CString strMsg)
{
	m_strMSG[m_nMSG] = strMsg;
	m_nMSG++;
	return 0;
}

INT CSelectComboBox::Init(CString strMsg)
{
	ResetContent();
	for (INT i = 0; i < m_nMSG; i++) {
		AddString(m_strMSG[i]);
	}
	SelectString(0, strMsg);

	return 0;
}

VOID CSelectComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	if (m_pParentDlg) {
		CMNG_ACCESS_CONTROL_1000 *pDlg = (CMNG_ACCESS_CONTROL_1000 *)m_pParentDlg;
		if (PARENT == _T("SUBMENU")) pDlg->ApplyComboBoxInSubMenuGridSell();
		if (PARENT == _T("TOOLBAR")) pDlg->ApplyComboBoxInToolbarGridSell();

		ShowWindow(FALSE);
	}	
}

VOID CSelectComboBox::OnSelchange() 
{
	if (m_pParentDlg) {
		CMNG_ACCESS_CONTROL_1000 *pDlg = (CMNG_ACCESS_CONTROL_1000 *)m_pParentDlg;
		if (PARENT == _T("SUBMENU")) pDlg->ApplyComboBoxInSubMenuGridSell();
		if (PARENT == _T("TOOLBAR")) pDlg->ApplyComboBoxInToolbarGridSell();

		ShowWindow(FALSE);
	}
}
