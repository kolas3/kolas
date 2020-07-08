// MyListCtrl.cpp: implementation of the CMyListCtrlEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyListCtrlEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyListCtrlEx::CMyListCtrlEx(INT iIDC, CWnd* pParent)
{
	m_pWnd = NULL;
	m_pWnd = pParent;
	m_iIDC = iIDC;
}

CMyListCtrlEx::~CMyListCtrlEx()
{

}

BOOLEAN CMyListCtrlEx::IsSelectedItem()
{
	INT iIndex = GetSelectedItemIdx();
	if(iIndex == -1) {
		return FALSE;
	} else {
		return TRUE;
	}	
}

VOID CMyListCtrlEx::SetItem(INT iIdx, INT iSubItem, CString sValue)
{

}

VOID CMyListCtrlEx::ResetContent()
{
	((CListCtrl*)(m_pWnd -> GetDlgItem(m_iIDC))) -> DeleteAllItems();
}

CString CMyListCtrlEx::GetSelectedItem(INT iSubItem)
{
    CListCtrl* pList = (CListCtrl*) (m_pWnd -> GetDlgItem(m_iIDC));
    INT iIdx = GetSelectedItemIdx();
    if(iIdx >= 0)
        return pList -> GetItemText(iIdx,iSubItem);
    else return _T("ERROR");
}

INT CMyListCtrlEx::GetSelectedItemIdx()
{
 	CListCtrl* pList = (CListCtrl*) (m_pWnd -> GetDlgItem(m_iIDC));   
    INT iMaxCount = pList -> GetItemCount();
	if(iMaxCount <= 0) {
		return 0;
	}

    INT iResult = -1;

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		if( pList -> GetItemState(i,LVIF_TEXT) == LVIS_FOCUSED ) {
            iResult = i;
            break;
        }
    }

    return iResult;
}

VOID CMyListCtrlEx::AddItem(INT iItem, INT iSubItem, CString sValue)
{
	CListCtrl* pList = (CListCtrl*) (m_pWnd -> GetDlgItem(m_iIDC));
    
    INT iMaxCount = pList -> GetItemCount();
	LV_ITEM lvitem;

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iItem;
	lvitem.iSubItem = iSubItem;
	lvitem.pszText = sValue.GetBuffer(0);
	
    switch(iSubItem) {
    case iHANJA  : pList -> InsertItem(&lvitem);	break;
    case iHANGUL : pList -> SetItem(&lvitem);		break;
    }    
}

VOID CMyListCtrlEx::SetParentPtr(CWnd * pParent)
{
    m_pWnd = pParent;
}

INT CMyListCtrlEx::GetItemCount()
{
    CListCtrl* pList = (CListCtrl*) (m_pWnd -> GetDlgItem(m_iIDC));
    INT iCount = pList -> GetItemCount();
    return iCount;
}

VOID CMyListCtrlEx::AddIconItem(INT iItem, CString sValue, INT iImageIdx)
{
	CListCtrl* pList = (CListCtrl*) (m_pWnd -> GetDlgItem(m_iIDC));

	pList -> InsertItem(iItem,sValue.GetBuffer(0),iImageIdx);
}

VOID CMyListCtrlEx::SetSelectedItem(INT iIdx)
{
	CListCtrl* pList = (CListCtrl*) (m_pWnd -> GetDlgItem(m_iIDC));

	LV_ITEM* pItem = NULL;

	pList -> SetItemState(iIdx,LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}
