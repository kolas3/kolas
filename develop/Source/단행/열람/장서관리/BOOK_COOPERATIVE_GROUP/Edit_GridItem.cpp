#include "stdafx.h"

#include "Edit_GridItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CEdit_GridItem::CEdit_GridItem()
{
}

CEdit_GridItem::~CEdit_GridItem()
{
}


BEGIN_MESSAGE_MAP(CEdit_GridItem, CEdit)
	//{{AFX_MSG_MAP(CEdit_GridItem)
	//}}AFX_MSG_MAP		
END_MESSAGE_MAP()


BOOL CEdit_GridItem::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			int nCol = (int)m_pGrid->GetCol();
			int nRow = (int)m_pGrid->GetRow();

			CString strOld = m_pGrid->GetTextMatrix(nRow, nCol);

			CString strOldNum;
			strOldNum.Format(_T("%d"), _ttoi(strOld));
			
			strOld.TrimLeft(strOldNum);

			CString strNew;
			GetWindowText(strNew);
			
			strNew.Format(_T("%d%s"), _ttoi(strNew), strOld);
			
			m_pGrid->SetTextMatrix(nRow, nCol, strNew);
			m_pDM->SetCellData(_T("변경여부"),_T("Y"), nRow-1);
			
			m_pGrid->GetParent()->SendMessage(UPDATE_OPTION_DM, 0, 0);
			ShowWindow(SW_HIDE);

			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
