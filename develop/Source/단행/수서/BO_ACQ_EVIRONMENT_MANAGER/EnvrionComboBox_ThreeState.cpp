// EnvrionComboBox_ThreeState.cpp : implementation file
//

#include "stdafx.h"
#include "EnvrionComboBox_ThreeState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvrionComboBox_ThreeState

CEnvrionComboBox_ThreeState::CEnvrionComboBox_ThreeState()
{
}

CEnvrionComboBox_ThreeState::~CEnvrionComboBox_ThreeState()
{
}


BEGIN_MESSAGE_MAP(CEnvrionComboBox_ThreeState, CComboBox)
	//{{AFX_MSG_MAP(CEnvrionComboBox_ThreeState)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvrionComboBox_ThreeState message handlers

VOID CEnvrionComboBox_ThreeState::Init(CStringArray *pData/*=NULL*/)
{
	if(pData == NULL) return;
	this->ResetContent();
	INT cnt = pData->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		AddString(pData->GetAt(i));
	}
}

VOID CEnvrionComboBox_ThreeState::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	if(m_pDM == NULL && m_nIdx < 0) return ;
	INT nCurSel = this->GetCurSel();
	if(nCurSel == 0) 
	{
		m_pDM->SetCellData(_T("관리값"), _T("Y"), m_nIdx);
		((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, 4, _T("사용함"));
	}
	else if(nCurSel == 1)
	{
		m_pDM->SetCellData(_T("관리값"), _T("O"), m_nIdx);
		((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, 4, _T("선택사항"));
	}
	else if(nCurSel == 2)
	{
		m_pDM->SetCellData(_T("관리값"), _T("N"), m_nIdx);
		((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, 4, _T("사용안함"));
	}
}

VOID CEnvrionComboBox_ThreeState::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	this->ShowWindow(SW_HIDE);
}

BOOL CEnvrionComboBox_ThreeState::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CComboBox::PreCreateWindow(cs);
}
