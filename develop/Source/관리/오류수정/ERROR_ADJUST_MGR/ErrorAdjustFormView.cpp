// ErrorAdjustFormView.cpp : implementation file
//

#include "stdafx.h"
#include "ErrorAdjustFormView.h"
#include "ErrorAdjustFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustFormView

IMPLEMENT_DYNCREATE(CErrorAdjustFormView, CFormView)

CErrorAdjustFormView::CErrorAdjustFormView()
	: CFormView(CErrorAdjustFormView::IDD)
{
	//{{AFX_DATA_INIT(CErrorAdjustFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	IsChildCreated = FALSE;
}

CErrorAdjustFormView::~CErrorAdjustFormView()
{
}

VOID CErrorAdjustFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorAdjustFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorAdjustFormView, CFormView)
	//{{AFX_MSG_MAP(CErrorAdjustFormView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustFormView diagnostics

#ifdef _DEBUG
VOID CErrorAdjustFormView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CErrorAdjustFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustFormView message handlers

VOID CErrorAdjustFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	INT ids = CreateChildWindow();
	if(ids < 0)
	{
		MessageBox(_T("View Dialog를 생성할 수 없습니다."), _T("에러"));
		return;
	}
}

VOID CErrorAdjustFormView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if(IsChildCreated == FALSE) return;
	
	CErrorAdjustFrame* pFrame = static_cast<CErrorAdjustFrame*>(GetParentFrame());
	if(pFrame->m_pErrorAdjustMgr->GetErrorAdjustDlgMgr()->GetErrorAdjustDlg()->GetSafeHwnd() == NULL) return;	
	pFrame->m_pErrorAdjustMgr->GetErrorAdjustDlgMgr()->GetErrorAdjustDlg()->MoveWindow(0, 0, cx, cy);	
}

INT CErrorAdjustFormView::CreateChildWindow()
{
	CErrorAdjustFrame * pFrame = static_cast<CErrorAdjustFrame*>(GetParentFrame());
	if(!pFrame->m_pErrorAdjustMgr->GetErrorAdjustDlgMgr()->GetErrorAdjustDlg()->Create(this))
	{
		IsChildCreated = FALSE;
		return -1;
	}
	pFrame->m_pErrorAdjustMgr->GetErrorAdjustDlgMgr()->GetErrorAdjustDlg()->ShowWindow(SW_SHOW);
	CRect rect;
	GetWindowRect(&rect);
	pFrame->m_pErrorAdjustMgr->GetErrorAdjustDlgMgr()->GetErrorAdjustDlg()->MoveWindow(0, 0, rect.Width(), rect.Height());
	IsChildCreated = TRUE;

	return 0;
}
