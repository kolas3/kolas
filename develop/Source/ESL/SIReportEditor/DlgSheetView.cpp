// DlgSheetView.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgSheetView.h"

#include "SIReportEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

#define PAGE_NAME _T("페이지")
#define FORM_NAME _T("워크폼")

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetView dialog

CDlgSheetView::CDlgSheetView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSheetView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSheetView)
	//}}AFX_DATA_INIT

}

VOID CDlgSheetView::InitTreeCtrl()
{
EFS_BEGIN

	m_ctrlTree.DeleteAllItems();

	m_ctrlTree.InsertItem(PAGE_NAME);
	m_ctrlTree.InsertItem(FORM_NAME);

EFS_END
}

VOID CDlgSheetView::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSheetView)
	DDX_Control(pDX, IDC_TREE, m_ctrlTree);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgSheetView, CDialog)
	//{{AFX_MSG_MAP(CDlgSheetView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgSheetView Operations

VOID CDlgSheetView::InitSheet(CString strSheetName /*= _T("")*/)
{
EFS_BEGIN

	HTREEITEM hRoot = m_ctrlTree.GetRootItem();

	HTREEITEM hSheet = m_ctrlTree.GetNextSiblingItem(hRoot);

	m_ctrlTree.DeleteItem(hSheet);
	
	m_ctrlTree.InsertItem(FORM_NAME);

EFS_END
}

VOID CDlgSheetView::AddSheet(CString strSheetName)
{
EFS_BEGIN

	HTREEITEM hRoot = m_ctrlTree.GetRootItem();

	HTREEITEM hSheet = m_ctrlTree.GetNextSiblingItem(hRoot);

	m_ctrlTree.InsertItem(strSheetName, 1, 1, hSheet);

EFS_END
}

VOID CDlgSheetView::AddPage(CString strPageName, CString strFileName)
{
EFS_BEGIN

	HTREEITEM hRoot = m_ctrlTree.GetRootItem();

	TVINSERTSTRUCT tvInsert;
	ZeroMemory(&tvInsert, sizeof(tvInsert));

	tvInsert.hParent = hRoot;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvInsert.item.pszText =  strPageName.GetBuffer(0);
	tvInsert.item.iImage = 1;
	tvInsert.item.iSelectedImage = 1;
		
	m_ctrlTree.InsertItem(&tvInsert);

EFS_END
}

VOID CDlgSheetView::ReadFirstPage()
{
EFS_BEGIN

	
	HTREEITEM hRoot = m_ctrlTree.GetRootItem();
	
	if (m_ctrlTree.GetItemText(hRoot) != PAGE_NAME) return;

	HTREEITEM hItem = m_ctrlTree.GetChildItem(hRoot);
	
	CString strOpenFileName;
	strOpenFileName.Format(_T("%s.xml"), m_ctrlTree.GetItemText(hItem));
	((CSIReportEditorView*)GetParentFrame()->GetActiveView())->EditPage(strOpenFileName);

EFS_END
}

VOID CDlgSheetView::ReadFirstSheet()
{
EFS_BEGIN

	
	HTREEITEM hRoot = m_ctrlTree.GetRootItem();
	HTREEITEM hSheet = m_ctrlTree.GetNextSiblingItem(hRoot);
	
	if (m_ctrlTree.GetItemText(hSheet) != FORM_NAME) return;

	HTREEITEM hItem = m_ctrlTree.GetChildItem(hSheet);
	((CSIReportEditorView*)GetParentFrame()->GetActiveView())->EditWorkSheet(m_ctrlTree.GetItemText(hItem));	

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetView message handlers
BOOL CDlgSheetView::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_imageList.Create(IDB_SHEET, 16, 1, RGB(0, 128, 128));
	m_ctrlTree.SetImageList(&m_imageList, TVSIL_NORMAL);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CDlgSheetView::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (!m_ctrlTree.GetSafeHwnd()) return;
	
	CRect rect;
	GetClientRect(rect);
	m_ctrlTree.MoveWindow(rect);

EFS_END
}

BOOL CDlgSheetView::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

VOID CDlgSheetView::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();
	
	if ((hItem != NULL) && !m_ctrlTree.ItemHasChildren(hItem))
	{
		if (m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(hItem)) == FORM_NAME)
		{
			((CSIReportEditorView*)GetParentFrame()->GetActiveView())->EditWorkSheet(m_ctrlTree.GetItemText(hItem));
		}
		else if (m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(hItem)) == PAGE_NAME)
		{
			CString strOpenFileName;
			strOpenFileName.Format(_T("%s.xml"), m_ctrlTree.GetItemText(hItem));
			((CSIReportEditorView*)GetParentFrame()->GetActiveView())->EditPage(strOpenFileName);
		}
	}

	*pResult = 0;

EFS_END
}
