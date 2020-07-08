// SIReportPreviewView.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView

IMPLEMENT_DYNCREATE(CSIReportPreviewView, CScrollView)

CSIReportPreviewView::CSIReportPreviewView(CSIReportManager *pManager, CWnd *pDlg)
{
	m_pDlg = pDlg;
	m_pManager = pManager;
	m_page = -1;
}

CSIReportPreviewView::~CSIReportPreviewView()
{
}


BEGIN_MESSAGE_MAP(CSIReportPreviewView, CScrollView)
	//{{AFX_MSG_MAP(CSIReportPreviewView)
		ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView drawing

VOID CSIReportPreviewView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
	sizeTotal = CSize(693, 980);
	SetScrollSizes(MM_LOMETRIC, sizeTotal);

	m_page = -1;
}

VOID CSIReportPreviewView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	if (m_page >= 1) {
		m_pManager->ShowXMLFileOnePage(MANAGER_MODE_PREVIEW, pDC, m_page);
		//m_pManager->ShowXMLFilePage(MANAGER_MODE_PREVIEW, pDC, m_page, m_page);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView diagnostics

#ifdef _DEBUG
VOID CSIReportPreviewView::AssertValid() const
{
	CScrollView::AssertValid();
}

VOID CSIReportPreviewView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView message handlers

INT CSIReportPreviewView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return 0;
}

