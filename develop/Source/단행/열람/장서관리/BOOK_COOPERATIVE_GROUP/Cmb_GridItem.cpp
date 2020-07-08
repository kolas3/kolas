#include "stdafx.h"

#include "Cmb_GridItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCmb_GridItem::CCmb_GridItem()
{
	m_pGrid = NULL;
}

CCmb_GridItem::~CCmb_GridItem()
{
	m_pGrid = NULL;
}


BEGIN_MESSAGE_MAP(CCmb_GridItem, CComboBox)
	//{{AFX_MSG_MAP(CCmb_GridItem)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCmb_GridItem::OnSelchange() 
{
	
	CString rString;
	int nIdx = GetCurSel();
	
	GetLBText(nIdx, rString.GetBuffer(0));

	int nCol = (int)m_pGrid->GetCol();
	int nRow = (int)m_pGrid->GetRow();
	ShowWindow(SW_HIDE);

	m_pGrid->SetTextMatrix(nRow, nCol, rString);
 	m_pDM->SetCellData(_T("변경여부"),_T("Y"), nRow-1);

	m_pGrid->GetParent()->SendMessage(UPDATE_OPTION_DM, 0, 0);
}
