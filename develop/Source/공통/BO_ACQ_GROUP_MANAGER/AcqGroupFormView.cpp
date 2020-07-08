// AcqGroupFormView.cpp : implementation file
//

#include "stdafx.h"
#include "AcqGroupFormView.h"
#include "AcqGroupFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupFormView

IMPLEMENT_DYNCREATE(CAcqGroupFormView, CFormView)

CAcqGroupFormView::CAcqGroupFormView()
	: CFormView(CAcqGroupFormView::IDD)
{
	//{{AFX_DATA_INIT(CAcqGroupFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	IsChildCreated = FALSE;
}

CAcqGroupFormView::~CAcqGroupFormView()
{
}

VOID CAcqGroupFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqGroupFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqGroupFormView, CFormView)
	//{{AFX_MSG_MAP(CAcqGroupFormView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupFormView diagnostics

#ifdef _DEBUG
VOID CAcqGroupFormView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CAcqGroupFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupFormView message handlers

VOID CAcqGroupFormView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if(IsChildCreated == FALSE) return;
	
	CAcqGroupFrame* pFrame = static_cast<CAcqGroupFrame*>(GetParentFrame());
	if(pFrame->m_pAcqGroupMgr->GetAcqGroupDlgMgr()->GetAcqGroupDlg()->GetSafeHwnd() == NULL) return;	
	pFrame->m_pAcqGroupMgr->GetAcqGroupDlgMgr()->GetAcqGroupDlg()->MoveWindow(0, 0, cx, cy);		
}

VOID CAcqGroupFormView::OnInitialUpdate() 
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

INT CAcqGroupFormView::CreateChildWindow()
{
	CAcqGroupFrame * pFrame = static_cast<CAcqGroupFrame*>(GetParentFrame());
	if(!pFrame->m_pAcqGroupMgr->GetAcqGroupDlgMgr()->GetAcqGroupDlg()->Create(this))
	{
		IsChildCreated = FALSE;
		return -1;
	}
	pFrame->m_pAcqGroupMgr->GetAcqGroupDlgMgr()->GetAcqGroupDlg()->ShowWindow(SW_SHOW);
	CRect rect;
	GetWindowRect(&rect);
	pFrame->m_pAcqGroupMgr->GetAcqGroupDlgMgr()->GetAcqGroupDlg()->MoveWindow(0, 0, rect.Width(), rect.Height());
	IsChildCreated = TRUE;
	
	return 0;
}
