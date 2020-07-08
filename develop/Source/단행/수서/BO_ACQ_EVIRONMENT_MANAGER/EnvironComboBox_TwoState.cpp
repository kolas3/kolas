// EnvironComboBox_TwoState.cpp : implementation file
//

#include "stdafx.h"
#include "EnvironComboBox_TwoState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvironComboBox_TwoState

CEnvironComboBox_TwoState::CEnvironComboBox_TwoState()
{
}

CEnvironComboBox_TwoState::~CEnvironComboBox_TwoState()
{
}


BEGIN_MESSAGE_MAP(CEnvironComboBox_TwoState, CComboBox)
	//{{AFX_MSG_MAP(CEnvironComboBox_TwoState)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvironComboBox_TwoState message handlers

VOID CEnvironComboBox_TwoState::Init(CStringArray *pData/*=NULL*/)
{
	if(pData == NULL) return;
	this->ResetContent();
	INT cnt = pData->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		AddString(pData->GetAt(i));
	}
}

VOID CEnvironComboBox_TwoState::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	if(m_pDM == NULL && m_nIdx < 0) return ;
	INT nCurSel = this->GetCurSel();
	CString strData;
	this->GetWindowText(strData);
	if(nCurSel == 0) 
	{
		m_pDM->SetCellData(_T("관리값"), _T("Y"), m_nIdx);
		((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, 4, strData);//_T("사용함"));
	}
	else
	{
		m_pDM->SetCellData(_T("관리값"), _T("N"), m_nIdx);
		((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, 4, strData);//_T("사용안함"));
	}
}

VOID CEnvironComboBox_TwoState::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	this->ShowWindow(SW_HIDE);
}

BOOL CEnvironComboBox_TwoState::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CComboBox::PreCreateWindow(cs);
}
