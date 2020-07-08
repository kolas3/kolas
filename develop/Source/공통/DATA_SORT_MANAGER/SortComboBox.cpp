// SortComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "SortComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortComboBox

CSortComboBox::CSortComboBox()
{
	m_nMsg = NULL;
	m_pDM = NULL;
	m_nIdx = 0;
	m_pParentWnd = NULL;
	m_strAlias.Empty();
	m_nColIdx = 0;
	m_pMainMgr = NULL;
}

CSortComboBox::~CSortComboBox()
{
}


BEGIN_MESSAGE_MAP(CSortComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSortComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortComboBox message handlers

VOID CSortComboBox::SetCurSelData(CString strData)
{
	INT nIdx = this->FindString(0, strData);
	if(nIdx >= 0)
		this->SetCurSel(nIdx);
	else
		this->SetCurSel(0);
}

VOID CSortComboBox::SetDataMgr(CESL_DataMgr *pDM)
{
	m_pDM = pDM;
}

VOID CSortComboBox::SetIndex(INT nIdx)
{
	if(nIdx < 0) return ;
	m_nIdx = nIdx;
}

VOID CSortComboBox::Init(CStringArray *pData/*=NULL*/)
{
	if(pData == NULL) return;
	this->ResetContent();
	INT cnt = pData->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		AddString(pData->GetAt(i));
	}
}

VOID CSortComboBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	if(m_pDM == NULL && m_nIdx < 0) return ;
	if(m_strAlias.IsEmpty() || m_nColIdx < 0) return ;
	if(m_nMsg == 0) return ;
	CString strData;
	this->GetWindowText(strData);
	if(m_nMsg == SORT_DIRECTION_CHECK)
	{
		if(strData == _T("오름차순")) strData.Format(_T("%d"), ESL_DATAMGR_SORT_ASC);
		else if(strData == _T("내림차순")) strData.Format(_T("%d"), ESL_DATAMGR_SORT_DESC);		
	}
	m_pDM->SetCellData(m_strAlias, strData, m_nIdx);
	((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, m_nColIdx-1, strData);	
	m_pMainMgr->PostMessage(m_nMsg, m_nIdx, 1);
}

VOID CSortComboBox::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
}

BOOL CSortComboBox::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CComboBox::PreCreateWindow(cs);
}
