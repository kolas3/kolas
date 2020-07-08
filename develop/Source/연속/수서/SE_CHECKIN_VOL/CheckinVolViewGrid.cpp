// CheckinVolViewGrid.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolViewGrid.h"
//test
#include "CheckinVolFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolViewGrid

IMPLEMENT_DYNCREATE(CCheckinVolViewGrid, CFormView)

CCheckinVolViewGrid::CCheckinVolViewGrid()	: CFormView(CCheckinVolViewGrid::IDD)
{
	//{{AFX_DATA_INIT(CCheckinVolViewGrid)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	IsChildCreated = FALSE;
	m_pDlg = NULL;
}

CCheckinVolViewGrid::~CCheckinVolViewGrid()
{
	if(m_pProgressDlg) delete m_pProgressDlg;
	m_pProgressDlg = NULL;
//	m_Dlg_brush.DeleteObject();
}


VOID CCheckinVolViewGrid::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolViewGrid)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinVolViewGrid, CFormView)
	//{{AFX_MSG_MAP(CCheckinVolViewGrid)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolViewGrid diagnostics

#ifdef _DEBUG
VOID CCheckinVolViewGrid::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CCheckinVolViewGrid::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolViewGrid message handlers

VOID CCheckinVolViewGrid::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
	m_pProgressDlg = new CCheckinVolProgressive;
	m_pProgressDlg->Create(this);
	m_pProgressDlg->CenterWindow();
	m_pProgressDlg->BringWindowToTop();


	//child ESL dialog create
	CCheckinVolFrame* pFrame = static_cast<CCheckinVolFrame*>(GetParentFrame());
	
	//test
	pFrame->m_pCheckinVolMgr->m_pCheckinVolESLMgr->m_pProgressDlg = m_pProgressDlg;
	
	pFrame->m_pCheckinVolMgr->m_pCheckinVolESLMgr->Create(this);
	pFrame->m_pCheckinVolMgr->m_pCheckinVolESLMgr->ShowWindow(SW_SHOW);

	IsChildCreated = TRUE;
//	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));



}

VOID CCheckinVolViewGrid::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!IsChildCreated) return;
		
	CCheckinVolFrame* pFrame = static_cast<CCheckinVolFrame*>(GetParentFrame());
	if(pFrame->m_pCheckinVolMgr->m_pCheckinVolESLMgr->GetSafeHwnd() == NULL) return;	
	pFrame->m_pCheckinVolMgr->m_pCheckinVolESLMgr->MoveWindow(0, 0, cx, cy);
	
}

BOOL CCheckinVolViewGrid::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}
