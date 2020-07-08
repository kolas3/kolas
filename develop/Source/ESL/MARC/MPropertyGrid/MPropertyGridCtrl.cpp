// MPropertyGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MPropertyGridCtrl.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPropertyGridCtrl

CMPropertyGridCtrl::CMPropertyGridCtrl()
{
	m_bAutoReadOnly = TRUE;
	m_nFixedColCount = 1;
	m_nDataColWidth = 100;
	m_nFixedColWidth = 100;

	m_nSelectedCol = -1;
	m_nSelectedRow = -1;

	m_pMultiComboBox = NULL;
	m_pEditCtrl = NULL;
	m_pDateTimeCtrl = NULL;

	m_nCurDMRow = -1;

	m_pCE = new CESL_Control_Element(this);

	m_bFirstSetting = FALSE;
}

CMPropertyGridCtrl::~CMPropertyGridCtrl()
{
	delete m_pCE;
	m_pCE = NULL;
	
	delete m_pMultiComboBox;
	m_pMultiComboBox = NULL;
	
	delete m_pEditCtrl;
	m_pEditCtrl = NULL;

	delete m_pDateTimeCtrl;
	m_pDateTimeCtrl = NULL;

	FreeData();
}

INT CMPropertyGridCtrl::FreeData()
{
	// Menu
	VOID *pClass = NULL;
	FreePtrList(&m_listPropertyItem, static_cast<CMPropertyItem*>(pClass));

	return 0;
}

template <class T> VOID CMPropertyGridCtrl::FreePtrList(CPtrList *pPtrList, T *pClass)
{
	if (pPtrList == NULL) return;

	pClass = NULL;
	POSITION pos = pPtrList->GetHeadPosition();
	while (pos)
	{
		pClass = static_cast<T*>(pPtrList->GetNext(pos));
		if (pClass)
		{
			delete pClass;
			pClass = NULL;
		}
	}

	pPtrList->RemoveAll();
}

BEGIN_MESSAGE_MAP(CMPropertyGridCtrl, CWnd)
	//{{AFX_MSG_MAP(CMPropertyGridCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDIT_MODIFY_MG, OnEditCtrlUpdate)
	ON_MESSAGE(WM_COMBO_MODIFY_MG, OnComboBoxUpdate)
	ON_MESSAGE(WM_DATE_TIME_MODIFY_MG, OnDateTimeCtrlUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPropertyGridCtrl Item Operations

INT CMPropertyGridCtrl::AddItem(LPCTSTR lpszItemName, LPCTSTR lpszGroup, UINT nItemType, UINT nItemStyle, LPCTSTR lpszLinkData /*= _T("")*/, LPCTSTR lpszListData /*= _T("")*/, LPCTSTR lpszItemData /*= _T("")*/)
{
	CMPropertyItem *pItem = new CMPropertyItem;
	m_listPropertyItem.AddTail(pItem);

	// Item 설정을 한다.
	pItem->m_strItemName = lpszItemName;
	pItem->m_nItemType = nItemType;
	pItem->m_nItemStyle = nItemStyle;
	pItem->m_strItemData = lpszItemData;
	pItem->m_strListData = lpszListData;
	pItem->m_strLinkData = lpszLinkData;
	pItem->m_strGroup = lpszGroup;

	m_bFirstSetting = TRUE;

	return 0;
}

INT CMPropertyGridCtrl::RemoveItem(LPCTSTR lpszItemName)
{
	CMPropertyItem *pItem = GetPropertyItem(lpszItemName);
	return 0;
}

CMPropertyItem *CMPropertyGridCtrl::GetPropertyItem(LPCTSTR lpszItemName)
{
	CMPropertyItem *pItem = NULL;
	POSITION pos = m_listPropertyItem.GetHeadPosition();
	while (pos)
	{
		pItem = static_cast<CMPropertyItem*>(m_listPropertyItem.GetNext(pos));
		if (!pItem) continue;

		if (pItem->m_strItemName == lpszItemName) return pItem;
	}

	return NULL;
}

INT CMPropertyGridCtrl::GetCellData(LPCTSTR lpszItemName, CString &strCellData)
{
	strCellData.Empty();

	CMPropertyItem *pItem = GetPropertyItem(lpszItemName);
	if (!pItem) return -1;
	
	strCellData = pItem->m_strItemData;

	return 0;
}

INT CMPropertyGridCtrl::SetCellData(LPCTSTR lpszItemName, CString &strCellData)
{
	CMPropertyItem *pItem = GetPropertyItem(lpszItemName);
	if (!pItem) return -1;
	
	pItem->m_strItemData = strCellData;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyGridCtrl Operations

INT CMPropertyGridCtrl::Init()
{
	FreeData();

	SetFixedRows(0);
	SetFixedCols(1);

	SetColAlignmentFixed(0, 4);

	m_mapColor.RemoveAll();

	m_bFirstSetting = TRUE;

	//SetColWidth(0, 0, 0);
	//SetRowHeight(0, 0);

	return 0;
}

INT CMPropertyGridCtrl::SetGroupColor(LPCTSTR lpszGroupName, COLORREF cr)
{
	m_mapColor.SetAt(lpszGroupName, cr);

	return 0;
}

INT CMPropertyGridCtrl::SetGroupDMPtr(LPCTSTR lpszGroupName, CESL_DataMgr *pMgr)
{
	if (pMgr == NULL) return -1;

	m_mapGroupDMptr.SetAt(lpszGroupName, pMgr);

	return 0;
}

INT CMPropertyGridCtrl::Display(INT nDMRow, CString strGroupAlias)
{
	m_nCurDMRow = nDMRow;
	m_strCurGroupAlias = strGroupAlias;

	if (m_bFirstSetting)
	{
		m_bFirstSetting = !m_bFirstSetting;

		Clear();

		INT nItemCount = m_listPropertyItem.GetCount();
		if (nItemCount <= 0) return -1;

		INT nRows = static_cast<INT>(ceil(static_cast<DOUBLE>(nItemCount) / static_cast<DOUBLE>(m_nFixedColCount)));
		INT nCols = m_nFixedColCount * 2;

		SetRows(nRows);
		SetCols(0, nCols);

		// Fixed Col의 색깔을 칠한다.
		COLORREF crFixed = GetBackColorFixed();
		for (INT nRow = 0; nRow < nRows; nRow++)
		{
			SetRow(nRow);

			for (INT nCol = 0; nCol < m_nFixedColCount; nCol++)
			{
				SetCol(nCol * 2);
				SetCellBackColor(crFixed);
				SetCellTextStyle(GetTextStyleFixed());
				SetColAlignment(nCol * 2, 4);
				SetCellFontBold(TRUE);
			}
		}

		// 넓이를 조정한다.
		for (INT nCol = 0; nCol < nCols; nCol++)
		{
			if (nCol % 2)
			{
				SetColWidth(nCol, 0, m_nDataColWidth * 15);
				SetColAlignment(nCol, 1);
			}
			else
				SetColWidth(nCol, 0, m_nFixedColWidth * 15);
		}
	}
	
	// Item을 Display한다.  &&  색 Display
	INT n = 0;
	CMPropertyItem *pItem = NULL;
	POSITION pos = m_listPropertyItem.GetHeadPosition();
	while (pos)
	{
		pItem = static_cast<CMPropertyItem*>(m_listPropertyItem.GetNext(pos));
		if (pItem == NULL) continue;
		
		DisplayItem(pItem, n);

		n++;		
	}

	

	return 0;
}

INT CMPropertyGridCtrl::DisplayItem(CMPropertyItem *pItem, INT nItemIdx)
{
	if (!pItem) return -1;
	if (nItemIdx < 0) return -1;
	
	INT nRow = static_cast<INT>(floor(static_cast<DOUBLE>(nItemIdx) / static_cast<DOUBLE>(m_nFixedColCount)));
	INT nMode = nItemIdx % m_nFixedColCount;

	INT nFixedCol = nMode ? (nMode * 2) : 0;
	INT nDataCol = nFixedCol + 1;

	// Get Item Data
	if (m_nCurDMRow >= 0 && !pItem->m_strLinkData.IsEmpty())
		GetItemData(pItem, m_nCurDMRow);

	// Set Fixed Text
	SetTextMatrix(nRow, nFixedCol, pItem->m_strItemName);

	// Set Data Text
	SetTextMatrix(nRow, nDataCol, pItem->m_strItemData);

	// Set Group Color
	COLORREF cr;
	if (m_mapColor.Lookup(pItem->m_strGroup, cr))
	{
		SetRow(nRow);
		SetCol(nDataCol);

		SetCellBackColor(cr);
	}

	return 0;
}

INT CMPropertyGridCtrl::GetItemData(CMPropertyItem *pItem, INT nDMRow)
{
	if (!pItem) return -1;

	CESL_DataMgr *pDM = NULL;;
	if (!m_mapGroupDMptr.Lookup(m_strCurGroupAlias, (VOID*&)pDM)) return -1;

	if (!pDM) return -1;

	if (pDM->GetCellData(pItem->m_strLinkData, nDMRow, pItem->m_strItemData) < 0) return -1;

	return GetParent()->SendMessage(WM_MPROPERTY_GRID_GET_DATA, (WPARAM)&pItem->m_strLinkData, (LPARAM)&pItem->m_strItemData);

}

INT CMPropertyGridCtrl::SetItemData(CMPropertyItem *pItem, INT nDMRow)
{
	if (!pItem) return -1;

	CESL_DataMgr *pDM = NULL;;
	if (!m_mapGroupDMptr.Lookup(m_strCurGroupAlias, (VOID*&)pDM)) return -1;

	if (!pDM) return -1;

	return pDM->SetCellData(pItem->m_strLinkData, pItem->m_strItemData, nDMRow);
}

INT CMPropertyGridCtrl::ChangeSelectedItemData(LPCTSTR lpszItemData)
{
	if (m_nSelectedRow < 0 || m_nSelectedCol < 0) return -1;

	CString strItemName = GetTextMatrix(m_nSelectedRow, m_nSelectedCol - 1);
	CMPropertyItem *pItem = GetPropertyItem(strItemName);
	if (!pItem) return -1;

	// 데이터 설정하고 Display 한다.
	pItem->m_strItemData = lpszItemData;

	// Link Data가 있으면 그곳에도 데이터를 설정한다.
	if (m_nCurDMRow >= 0 && !pItem->m_strLinkData.IsEmpty())
		SetItemData(pItem, m_nCurDMRow);

	SetTextMatrix(m_nSelectedRow, m_nSelectedCol, lpszItemData);

	GetParent()->SendMessage(WM_MPROPERTY_GRID_UPDATE, m_nCurDMRow);

	return 0;
}

INT CMPropertyGridCtrl::ShowControl(INT nMode)
{
	INT nRow = GetRow();
	INT nCol = GetCol();

	if (!(nCol % 2)) return -1;

	CRect rcControl;
	GetCellRect(rcControl);

	// Type을 결정한다.
	CString strItemName = GetTextMatrix(nRow, nCol - 1);
	CMPropertyItem *pItem = GetPropertyItem(strItemName);
	if (!pItem) return -1;

	// 리드 온리인지, 아니면 현재 설정한 그룹이 아닌지.... 체크한다.
	if (pItem->m_nItemStyle == CMPropertyItem::styleReadOnly) return -1;

	if (!m_strCurGroupAlias.IsEmpty())
		if (pItem->m_strGroup != m_strCurGroupAlias) return -1;

	if (pItem->m_nItemType == CMPropertyItem::typeEditCtrl)
		ShowControl_Edit(pItem, rcControl, nMode);
	else if (pItem->m_nItemType == CMPropertyItem::typeComboBox)
		ShowControl_ComboBox(pItem, rcControl, nMode);
	else if (pItem->m_nItemType == CMPropertyItem::typeMultiComboBox)
		ShowControl_MultiComboBox(pItem, rcControl, nMode);
	else if (pItem->m_nItemType == CMPropertyItem::typeDateEditCtrl)
		ShowControl_DateTime(pItem, rcControl, nMode);
	else 
		return -1;

	m_nSelectedRow = nRow;
	m_nSelectedCol = nCol;

	return 0;
}

INT CMPropertyGridCtrl::ShowControl_Edit(CMPropertyItem *pItem, LPRECT lpRect, INT nMode /*=0*/)
{
	if (!pItem) return -1;
	if (!m_pEditCtrl) return -1;

	m_pEditCtrl->m_nEditMode = nMode;

	m_pEditCtrl->MoveWindow(lpRect);	
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();

	// 텍스트 설정
	if (nMode == CMPropertyEdit::editCustom)
		m_pEditCtrl->SetWindowText(pItem->m_strItemData);
	else
		m_pEditCtrl->SetWindowText(NULL);

	return 0;
}

INT CMPropertyGridCtrl::ShowControl_DateTime(CMPropertyItem *pItem, LPRECT lpRect, INT nMode /*= 0*/)
{
	if (!pItem) return -1;

	lpRect->bottom = lpRect->top + 20;

	m_pDateTimeCtrl->MoveWindow(lpRect);	

	m_pDateTimeCtrl->SetData(pItem->m_strItemData);
	m_pDateTimeCtrl->ShowWindow(SW_SHOW);
	m_pDateTimeCtrl->SetFocus();
	m_pDateTimeCtrl->BringWindowToTop();

	return 0;
}

INT CMPropertyGridCtrl::ShowControl_ComboBox(CMPropertyItem *pItem, LPRECT lpRect, INT nMode /*= 0*/)
{
	if (!pItem) return -1;

	CRect rcCombo;
	m_pMultiComboBox->GetClientRect(rcCombo);
	lpRect->bottom = lpRect->top + rcCombo.Height();

	// 콤보박스 초기화
	m_pMultiComboBox->FreeComboData();
	m_pMultiComboBox->PrevRow = -1;
	m_pMultiComboBox->PrevCol = -1;
	m_pMultiComboBox->m_nMaxRowCnt = 0;

	/// 데이터 초기화
	m_pMultiComboBox->ResetContent();
	m_pMultiComboBox->InitMultiComboBox();

	INT nRow = 0;
	CString strListData = pItem->m_strListData;
	LPTSTR lpszToken = _tcstok(strListData.GetBuffer(0), _T(","));
	while (lpszToken)
	{
		m_pMultiComboBox->AddItem(lpszToken, 0, nRow++);
		lpszToken = _tcstok(NULL, _T(","));
	}

	m_pMultiComboBox->SelectString(pItem->m_strItemData);

	// 보여주기
	m_pMultiComboBox->MoveWindow(lpRect);
	m_pMultiComboBox->ShowWindow(SW_SHOW);
	m_pMultiComboBox->BringWindowToTop();

	m_pMultiComboBox->ShowDropDown();
	
	m_pMultiComboBox->Invalidate();
	
	m_pMultiComboBox->ShowWindow(SW_SHOW);
	m_pMultiComboBox->SetFocus();	

	return 0;
}

INT CMPropertyGridCtrl::ShowControl_MultiComboBox(CMPropertyItem *pItem, LPRECT lpRect, INT nMode /*=0*/)
{
	if (!pItem) return -1;

	m_pCE->WorkWnd		= GetParent();
	m_pCE->CTRL_ID		= ID_MULTICOMBOBOX;
	m_pCE->CTRL_TYPE	= _T("M");
	m_pCE->LIST_DATA	= pItem->m_strListData;
	m_pCE->LINK_DATA	= _T("");
	
	m_pMultiComboBox->ELEMENT = m_pCE;

	CRect rcCombo;
	m_pMultiComboBox->GetClientRect(rcCombo);

	lpRect->bottom = lpRect->top + rcCombo.Height();

	// 콤보박스 초기화
	m_pMultiComboBox->FreeComboData();
	m_pMultiComboBox->PrevRow = -1;
	m_pMultiComboBox->PrevCol = -1;
	m_pMultiComboBox->m_nMaxRowCnt = 0;

	/// 데이터 초기화
	m_pMultiComboBox->ResetContent();
	m_pMultiComboBox->InitMultiComboBoxRef();

	m_pMultiComboBox->SelectString(pItem->m_strItemData);

	// 보여주기
	m_pMultiComboBox->MoveWindow(lpRect);
	m_pMultiComboBox->ShowWindow(SW_SHOW);
	m_pMultiComboBox->BringWindowToTop();

	m_pMultiComboBox->ShowDropDown();
	
	m_pMultiComboBox->Invalidate();
	
	m_pMultiComboBox->ShowWindow(SW_SHOW);
	m_pMultiComboBox->SetFocus();	

	return 0;
}

INT CMPropertyGridCtrl::GetCellRect(CRect &rect)
{
	CRect gridRect;
	CRect parentRect;
	GetWindowRect(&gridRect);
	GetParent()->GetClientRect(&parentRect);

	::ClientToScreen(GetParent()->m_hWnd,(POINT*)&(parentRect.left));

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left	= gridRect.left	+ (long)((float)GetCellLeft() / 15.0f);
	rect.top	= gridRect.top	+ (long)((float)GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)GetCellHeight() / 15.0f) - 1;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyGridCtrl Member Data Operations
INT CMPropertyGridCtrl::SetFixedColCount(INT nFixedColCount)
{
	if (nFixedColCount < 1) return -1;

	m_nFixedColCount = nFixedColCount;

	return 0;
}

INT CMPropertyGridCtrl::SetFixedColWidth(INT nFixedColWidth)
{
	if (nFixedColWidth < 100) return -1;
	
	m_nFixedColWidth = nFixedColWidth;

	return 0;
}

INT CMPropertyGridCtrl::SetDataColWidth(INT nDataColWidth)
{
	if (nDataColWidth < 100) return -1;

	m_nDataColWidth = nDataColWidth;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMPropertyGridCtrl message handlers

VOID CMPropertyGridCtrl::PreSubclassWindow() 
{
	// Edit Ctrl
	m_pEditCtrl = new CMPropertyEdit;
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE, CRect(0, 0, 0, 0), GetParent(), ID_EDIT);
	m_pEditCtrl->m_pParentGrid = this;
	m_pEditCtrl->SetFont(GetParent()->GetFont());
	m_pEditCtrl->ShowWindow(SW_HIDE);

	// Date Time Ctrl
	m_pDateTimeCtrl = new CMPropertyDateTimeCtrl;
	m_pDateTimeCtrl->Create(WS_CHILD | DTS_SHORTDATEFORMAT, CRect(0, 0, 100, 100), GetParent(), 1006);
	m_pDateTimeCtrl->m_pParentGrid = this;
	m_pDateTimeCtrl->SetFont(GetParent()->GetFont());

	// MultiComboBox Ctrl
	m_pMultiComboBox = new CMPropertyComboBox(NULL);
	m_pMultiComboBox->Create(_T("COMBOBOX"), NULL, WS_CHILD | WS_VSCROLL| CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_SORT | CBS_OWNERDRAWFIXED, CRect(1, 1, 500, 300), GetParent(), ID_MULTICOMBOBOX);
	m_pMultiComboBox->m_pParentGrid = this;
	m_pMultiComboBox->SetFont(GetParent()->GetFont());
	
	CWnd::PreSubclassWindow();
}

BOOL CMPropertyGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN || pMsg->wParam == VK_F2))
		{
			ShowControl(CMPropertyEdit::editCustom);
			return TRUE;
		}

		if ((_TUCHAR)pMsg->wParam >= 0x80 || _istalnum(pMsg->wParam))
		{
			if (m_pEditCtrl)
			{
				m_pEditCtrl->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				ShowControl(CMPropertyEdit::editEasy);
				return TRUE;
			}
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

VOID CMPropertyGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);

	ShowControl(CMPropertyEdit::editCustom);
}

UINT CMPropertyGridCtrl::OnEditCtrlUpdate(WPARAM wParam, LPARAM lParam)
{
	SetFocus();
	if (m_pEditCtrl == NULL) return -1;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return -1;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			
	if (wParam != 1) return 0;

	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	ChangeSelectedItemData(strData);

	return 0;
}

UINT CMPropertyGridCtrl::OnDateTimeCtrlUpdate(WPARAM wParam, LPARAM lParam)
{
	SetFocus();

	if (m_pDateTimeCtrl == NULL) return -1;

	if (!::IsWindowVisible(m_pDateTimeCtrl->GetSafeHwnd())) return -1;

	m_pDateTimeCtrl->ShowWindow(SW_HIDE);

	if (wParam != 1) return 0;

	CString strData;
	m_pDateTimeCtrl->GetData(strData);

	ChangeSelectedItemData(strData);

	return 0;
}

UINT CMPropertyGridCtrl::OnComboBoxUpdate(WPARAM wParam, LPARAM lParam)
{
	SetFocus();

	if (m_pMultiComboBox == NULL) return -1;

	if (!::IsWindowVisible(m_pMultiComboBox->GetSafeHwnd())) return -1;
	
	m_pMultiComboBox->ShowWindow(SW_HIDE);

	if (wParam != 1) return 0;

	CString strData;
	m_pMultiComboBox->GetData(strData);

	ChangeSelectedItemData(strData);
	
	return 0;
}

BOOL CMPropertyGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	INT nRowsScrolled = 1 * zDelta / 120;

    if (nRowsScrolled >= 0)
        for (INT i = 0; i < nRowsScrolled; i++)
           this->PostMessage(WM_VSCROLL, SB_LINEUP, 0);
    else
        for (INT i = 0; i > nRowsScrolled; i--)
			this->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	return CMSHFlexGrid::OnMouseWheel(nFlags, zDelta, pt);
	
	//return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

VOID CMPropertyGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	OnEditCtrlUpdate(1, 0);
	OnDateTimeCtrlUpdate(1, 0);
	OnComboBoxUpdate(1, 0);
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

VOID CMPropertyGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	OnEditCtrlUpdate(1, 0);
	OnDateTimeCtrlUpdate(1, 0);
	OnComboBoxUpdate(1, 0);
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

VOID CMPropertyGridCtrl::OnSize(UINT nType, INT cx, INT cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (!GetSafeHwnd()) return;

	if (m_nFixedColCount == 1)
	{
		CRect rcClient;
		GetClientRect(rcClient);

		if (rcClient.Width() > m_nFixedColWidth)
		{
			if (rcClient.Width() - (m_nFixedColWidth + m_nDataColWidth) > 0)
			{
				SetColWidth(1, 0, (rcClient.Width() - m_nFixedColWidth) * 15);
			}
		}
	}
}
