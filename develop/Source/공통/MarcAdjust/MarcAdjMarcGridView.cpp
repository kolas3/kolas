// MarcAdjMarcGridView.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjMarcGridView.h"
#include "MarcAdjFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KEY_DOWN(vk) ( (GetAsyncKeyState(vk) & 0x8000) ? 1:0)
#define KEY_UP(vk)	 ( (GetAsyncKeyState(vk) & 0x8000) ? 0:1)

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcGridView

IMPLEMENT_DYNCREATE(CMarcAdjMarcGridView, CFormView)

CMarcAdjMarcGridView::CMarcAdjMarcGridView()
	: CFormView(CMarcAdjMarcGridView::IDD)
{
	//{{AFX_DATA_INIT(CMarcAdjMarcGridView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarcAdjMgr = NULL;
}

CMarcAdjMarcGridView::~CMarcAdjMarcGridView()
{
}

VOID CMarcAdjMarcGridView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAdjMarcGridView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	keybd_event(VK_TAB, MapVirtualKey(VK_TAB,0),0,0);
	keybd_event(VK_SHIFT, MapVirtualKey(VK_SHIFT,0),0,0);
	keybd_event(VK_SHIFT, MapVirtualKey(VK_SHIFT,0),KEYEVENTF_KEYUP,0);
	keybd_event(VK_TAB, MapVirtualKey(VK_TAB,0),KEYEVENTF_KEYUP,0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcAdjMarcGridView, CFormView)
	//{{AFX_MSG_MAP(CMarcAdjMarcGridView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER + 1003, OnSetFocusGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcGridView diagnostics

#ifdef _DEBUG
VOID CMarcAdjMarcGridView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CMarcAdjMarcGridView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcGridView Operations

VOID CMarcAdjMarcGridView::InitMarcGridCtrl()
{
	if (m_pMarcAdjMgr == NULL) return;
	CMarcAdjResourceMgr *pResourceMgr = m_pMarcAdjMgr->GetResourceMgr();
	pResourceMgr->m_MarcEditor.SubclassDlgItem(-1, IDC_GRID_MARC_ADJ, this, GetParentFrame());
	pResourceMgr->m_MarcEditor.Display();
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcGridView message handlers

VOID CMarcAdjMarcGridView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT,CSize(1,1));

	m_pMarcAdjMgr = static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr();

	InitMarcGridCtrl();
}

VOID CMarcAdjMarcGridView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CWnd *pWnd  = GetDlgItem(IDC_GRID_MARC_ADJ);
	if (pWnd->GetSafeHwnd())
	{
		pWnd->MoveWindow(0, 0, cx, cy);
	}
}

BEGIN_EVENTSINK_MAP(CMarcAdjMarcGridView, CFormView)
    //{{AFX_EVENTSINK_MAP(CMarcAdjMarcGridView)
	ON_EVENT(CMarcAdjMarcGridView, IDC_GRID_MARC_ADJ, -604 /* KeyUp */, OnKeyUpGridMarcAdj, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcAdjMarcGridView::OnKeyUpGridMarcAdj(short FAR* KeyCode, short Shift) 
{

}

BOOL CMarcAdjMarcGridView::PreTranslateMessage(MSG* pMsg) 
{   

	return CFormView::PreTranslateMessage(pMsg);
}

VOID CMarcAdjMarcGridView::OnSetFocusGrid()
{
	
}