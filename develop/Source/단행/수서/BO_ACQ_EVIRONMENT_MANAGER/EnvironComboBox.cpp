// EnvironComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "EnvironComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvironComboBox

CEnvironComboBox::CEnvironComboBox()
{
	m_pDM = NULL;
	m_nIdx = -1;
}

CEnvironComboBox::~CEnvironComboBox()
{
}


BEGIN_MESSAGE_MAP(CEnvironComboBox, CComboBox)
	//{{AFX_MSG_MAP(CEnvironComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvironComboBox message handlers

VOID CEnvironComboBox::SetCurSelData(CString strData)
{
	if(strData == _T("Y")) SetCurSel(0);
	else SetCurSel(1);
}

VOID CEnvironComboBox::SetDataMgr(CESL_DataMgr *pDM)
{
	m_pDM = pDM;
}

VOID CEnvironComboBox::SetIndex(INT nIdx)
{
	if(nIdx < 0) return ;
	m_nIdx = nIdx;
}
