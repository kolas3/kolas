// MListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportEditor.h"
#include "MListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMListCtrl
IMPLEMENT_DYNCREATE(CMListCtrl, CListCtrl)

CMListCtrl::CMListCtrl()
{
	m_nSelectIndex = -1;
	// KOL.UDF.022 시큐어코딩 보완
	m_nOleFormat = 0;
}

CMListCtrl::~CMListCtrl()
{

}


BEGIN_MESSAGE_MAP(CMListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMListCtrl message handlers

VOID CMListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListCtrl::PreSubclassWindow();
	
	m_nOleFormat = RegisterClipboardFormat(_T("SIREPORT_EDITOR"));
	
	if (!m_oleDropTarget.Register(this))
       AfxMessageBox(_T("Ole Register Drop Target Failed"));     

	EnableToolTips();
}

VOID CMListCtrl::InsertToolTip(INT nItem, CString &strToolTip)
{
	m_mapToolTip.SetAt(nItem, strToolTip);
}

INT CMListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
		if(GetItemCount())
		{
			INT nTopIndex = GetTopIndex();
			INT nBottomIndex = nTopIndex + GetCountPerPage();
			if(nBottomIndex > GetItemCount()) nBottomIndex = GetItemCount();
			for(INT nIndex = nTopIndex; nIndex <= nBottomIndex; nIndex++)
			{
				GetItemRect(nIndex, rect, LVIR_BOUNDS);
				if(rect.PtInRect(point))
				{
					pTI->hwnd = m_hWnd;
					pTI->uId = (UINT)(nIndex+1);
					pTI->lpszText = LPSTR_TEXTCALLBACK;
					pTI->rect = rect;
					return pTI->uId;
				}
			}
		}
	}
	return -1;
}

BOOL CMListCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if( nID == 0 )	  	// Notification in NT from automatically
		return FALSE;   	// created tooltip

	INT row = ((nID-1) >> 10) & 0x3fffff ;
	INT col = (nID-1) & 0x3ff;

	// Use Item's name as the tool tip. Change this for something different.
	// Like use its file size, etc.
	INT nItem = nID - 1;
	//strTipText = GetItemText(nItem, 0);
	m_mapToolTip.Lookup(nItem, strTipText);

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, 80);
	else
		_mbstowcsz(pTTTW->szText, strTipText, 80);
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, 80);
	else
		lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
	*pResult = 0;

	return TRUE;    // message was handled
}

VOID CMListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Get Selected Item index
	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	m_nSelectIndex = SubItemHitTest(&lvhti);

	
	CListCtrl::OnLButtonDown(nFlags, point);
}

VOID CMListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	// OLE (Clipboard)
	COleDataSource oleDataSrc;
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(DRAG_PARAM));
	if(hGlobal == NULL)
	{
		AfxMessageBox(_T("Can not allocate enough memory\t"),MB_OK|MB_ICONSTOP);
		return;
	}

	DRAG_PARAM* pData = (DRAG_PARAM*)::GlobalLock(hGlobal);
	
	CString& strItemName = GetItemText(m_nSelectIndex, 0);
	_tcscpy(pData->szItemName, strItemName.GetBuffer(0));

	::GlobalUnlock(hGlobal);

	oleDataSrc.CacheGlobalData(m_nOleFormat, hGlobal);
	oleDataSrc.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);

	::GlobalFree(hGlobal);
		
	*pResult = 0;
}


