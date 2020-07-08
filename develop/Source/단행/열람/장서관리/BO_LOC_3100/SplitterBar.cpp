// CSplitterBar.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SplitterBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar

IMPLEMENT_DYNAMIC(CSplitterBar, CWnd)

BEGIN_MESSAGE_MAP(CSplitterBar, CWnd)
	//{{AFX_MSG_MAP(CSplitterBar)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSplitterBar::CSplitterBar()
{
	m_bDragging					=		FALSE;

	m_pwndLeftPane				=		NULL;
	m_pwndRightPane				=		NULL;
	m_pWndStaticReturnList		=		NULL;
	m_pWndStaticReturnGroup		=		NULL;
	m_pWndstaticCtrlGuideLine	=		NULL;

	m_rectSplitter = NULL;
	m_rectDragPrev = NULL;
	m_rectDragCurt = NULL;
	m_cxLeftMost = 0;
	m_cxRightMost = 0;
	m_bHorizontal = FALSE;
}

CSplitterBar::~CSplitterBar()
{
	if( NULL !=	m_pWndStaticReturnList )
	{
		m_pWndStaticReturnList->Detach();
		delete	m_pWndStaticReturnList;
		m_pWndStaticReturnList	=	NULL;
	}

	if( NULL !=	m_pWndStaticReturnGroup)
	{
		m_pWndStaticReturnGroup->Detach();
		delete	m_pWndStaticReturnGroup;
		m_pWndStaticReturnGroup	=	NULL;
	}

	if( NULL !=	m_pWndstaticCtrlGuideLine )
	{
		m_pWndstaticCtrlGuideLine->Detach();
		delete	m_pWndstaticCtrlGuideLine;
		m_pWndstaticCtrlGuideLine	=	NULL;
	}
}

BOOL CSplitterBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bHorizontal)
{
	CWnd* pWnd		=	this;
	m_bHorizontal	=	bHorizontal;
	return pWnd->Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar message handlers

void CSplitterBar::OnPaint() 
{
	RECT rc;
	if (!GetUpdateRect(&rc)) return;

	PAINTSTRUCT paint;
	CDC *pDC = BeginPaint(&paint);

	CRect rect;
	GetClientRect(rect);
	pDC->Draw3dRect(&rect,
				  ::GetSysColor(COLOR_BTNHIGHLIGHT),
				  ::GetSysColor(COLOR_BTNSHADOW));
	EndPaint(&paint);
}

UINT CSplitterBar::OnNcHitTest(CPoint point) 
{	
	return HTCLIENT;
}

int CSplitterBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetWindowRect(&m_rectSplitter);
	SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	CRect rectParent;
	GetParent()->GetClientRect(rectParent);
	if(m_bHorizontal)
	{
		m_cxLeftMost	=	rectParent.top;
		m_cxRightMost	=	rectParent.bottom;
	}
	else
	{
	}

	return 0;
}

BOOL CSplitterBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint ptCursor=GetMessagePos();

	if(IsCursorOverSplitter(ptCursor))
	{	
		::SetCursor( AfxGetApp()->LoadCursor(m_bHorizontal?AFX_IDC_VSPLITBAR:AFX_IDC_HSPLITBAR));	
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSplitterBar::IsCursorOverSplitter( const CPoint& ptCursor )
{
	CRect rectSplitter;
	GetWindowRect(rectSplitter);
	return rectSplitter.PtInRect( ptCursor );
}

void CSplitterBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(nFlags & MK_LBUTTON && m_bDragging)
	{	
		
		DrawDraggingBar(point);
		return;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CSplitterBar::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	ClientToScreen(&point);
	if(IsCursorOverSplitter(point))
	{
		SetCapture();
		m_bDragging=TRUE;
		GetWindowRect(&m_rectSplitter);		
		ScreenToClient(&point);
		DrawDraggingBar(point,DRAG_ENTER);
		return;
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CSplitterBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		DrawDraggingBar(point,DRAG_EXIT);

		ClientToScreen(&point);

		if(m_bHorizontal)
		{
			CPoint		pointLeftMost;
			pointLeftMost.y		=	m_cxLeftMost;
			GetParent()->ClientToScreen(&pointLeftMost);
			
			CPoint		pointRightMost;
			pointRightMost.y	=	m_cxRightMost;
			GetParent()->ClientToScreen(&pointRightMost);

			if(point.y < pointLeftMost.y)
				point.y		=	pointLeftMost.y;

			if(point.y > pointRightMost.y)
				point.y		=	pointRightMost.y;

			m_rectDragCurt			=	m_rectSplitter;
			m_rectDragCurt.top		=	point.y;
			m_rectDragCurt.bottom	=	point.y+m_rectSplitter.Height();
		}
		else
		{
			CPoint pointLeftMost;
			pointLeftMost.x=m_cxLeftMost;
			GetParent()->ClientToScreen(&pointLeftMost);
			CPoint pointRightMost;
			pointRightMost.x=m_cxRightMost;
			GetParent()->ClientToScreen(&pointRightMost);

			if(point.x < pointLeftMost.x)
				point.x=pointLeftMost.x;
			if(point.x > pointRightMost.x)
				point.x=pointRightMost.x;

			m_rectDragCurt=m_rectSplitter;
			m_rectDragCurt.left=point.x;
			m_rectDragCurt.right=point.x+m_rectSplitter.Width();
		}
		GetParent()->ScreenToClient(m_rectDragCurt);
		MoveWindow(m_rectDragCurt,TRUE);
		OnPaint();

		ReleaseCapture();
		m_bDragging=FALSE;
		MovePanes();
		ReSetSplitterMoveMostValue();

		GetParent()->PostMessage(WM_3100_SPLITTER_MOVED,0,0L);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CSplitterBar::DrawDraggingBar(CPoint point,DRAGFLAG df)
{
	ClientToScreen(&point);

	CPoint		pointLeftMost;
	pointLeftMost.y		=	m_cxLeftMost;
	GetParent()->ClientToScreen(&pointLeftMost);
	
	CPoint		pointRightMost;
	pointRightMost.y	=	m_cxRightMost;
	GetParent()->ClientToScreen(&pointRightMost);

	if(point.y < pointLeftMost.y)
		point.y		=	pointLeftMost.y;

	if(point.y > pointRightMost.y)
		point.y		=	pointRightMost.y;

	m_rectDragCurt		=		m_rectSplitter;
	if(m_bHorizontal)
	{
		m_rectDragCurt.top		=	point.y;
		m_rectDragCurt.bottom	=	point.y+m_rectSplitter.Height();
	}
	else
	{
		m_rectDragCurt.left=point.x;
		m_rectDragCurt.right=point.x+m_rectSplitter.Width();
	}

	CSize size(m_rectDragCurt.Width(),m_rectDragCurt.Height());

	CWnd	*pParentWnd	=	GetParent();
	ASSERT(pParentWnd);
	CDC		*pDC		=	pParentWnd->GetDC();	
	pParentWnd->ScreenToClient(m_rectDragCurt);
	switch(df)
	{
		case DRAG_ENTER:
			 pDC->DrawDragRect(m_rectDragCurt,size,NULL,size);
			 break;
		case DRAG_EXIT:	
		default:
			 pDC->DrawDragRect(m_rectDragCurt,size,m_rectDragPrev,size);
			 break;
	}
	pParentWnd->ReleaseDC(pDC);
	m_rectDragPrev	=	m_rectDragCurt;
}

void CSplitterBar::SetPanes(CWnd *pwndLeftPane,CWnd *pwndRightPane, HWND pHwndStaticReturnList , HWND pHwndStaticReturnGroup )
{
	ASSERT(pwndLeftPane);
	ASSERT(pwndRightPane);
	ASSERT(pHwndStaticReturnList);
	ASSERT(pHwndStaticReturnGroup);

	m_pwndLeftPane				=	pwndLeftPane;
	m_pwndRightPane				=	pwndRightPane;

	m_pWndStaticReturnList		=	new CWnd;	
	m_pWndStaticReturnList->Attach( HWND(pHwndStaticReturnList) );	

	m_pWndStaticReturnGroup		=	new CWnd;	
	m_pWndStaticReturnGroup->Attach( HWND(pHwndStaticReturnGroup) );

	CRect	rectBar, rectLeft , rectRight , rectStaticReturnList , rectStaticReturnGroup;

	m_pwndLeftPane->GetWindowRect(rectLeft);		
	GetParent()->ScreenToClient(rectLeft);

	m_pwndRightPane->GetWindowRect(rectRight);	
	GetParent()->ScreenToClient(rectRight);

	m_pWndStaticReturnList->GetWindowRect(rectStaticReturnList);
	GetParent()->ScreenToClient(rectStaticReturnList);	

	m_pWndStaticReturnGroup->GetWindowRect(rectStaticReturnGroup);
	GetParent()->ScreenToClient(rectStaticReturnGroup);

	if(m_bHorizontal)
	{
		rectBar.CopyRect(rectLeft);
		rectBar.left			=		rectLeft.left;
		rectBar.right			=		rectLeft.right;
		rectBar.top				=		rectBar.bottom + 95; 
		rectBar.bottom			=		rectStaticReturnGroup.top + 95;

		INT		nBarHeight		=		GetSystemMetrics(SM_CXFRAME);
		rectBar.top				-=		(nBarHeight/2);
		rectBar.bottom			+=		(nBarHeight/2);
		
		MoveWindow(rectBar);

		MovePanes();
	}
	else
	{
		rectBar.left			=		rectBar.right;

		INT		nBarWidth		=		GetSystemMetrics(SM_CYFRAME);
		rectBar.left			-=		nBarWidth/2;
		rectBar.right			+=		nBarWidth/2;
		MoveWindow(rectBar);

		MovePanes();

		m_cxLeftMost=rectLeft.left;

		m_cxRightMost=rectRight.right;
	}
}

void CSplitterBar::MovePanes()
{
	ASSERT(m_pwndLeftPane);
	ASSERT(m_pwndRightPane);
	ASSERT(m_pWndStaticReturnList);
	ASSERT(m_pWndStaticReturnGroup);

	if(m_bHorizontal)
	{
		CRect	rectBar;
		GetWindowRect(rectBar);
		GetParent()->ScreenToClient(rectBar);

		CRect	rectRight;
		m_pwndRightPane->GetWindowRect(rectRight);
		GetParent()->ScreenToClient(rectRight);
		rectRight.top		=	rectBar.bottom - GetSystemMetrics(SM_CXBORDER) + 31;
		m_pwndRightPane->MoveWindow(rectRight);

		CRect	rectLeft;
		m_pwndLeftPane->GetWindowRect(rectLeft);
		GetParent()->ScreenToClient(rectLeft);
		rectLeft.bottom		=	rectBar.top;
		m_pwndLeftPane->MoveWindow(rectLeft);

		CRect	rectReturnGroup;
		m_pWndStaticReturnGroup->GetWindowRect(rectReturnGroup);
		GetParent()->ScreenToClient(rectReturnGroup);
		rectReturnGroup.top		=	rectLeft.bottom	+ GetSystemMetrics(SM_CXBORDER)+1;
		rectReturnGroup.bottom	=	rectReturnGroup.top + 29;
		m_pWndStaticReturnGroup->MoveWindow(rectReturnGroup);

		CRect	rectReturnList;
		m_pWndStaticReturnList->GetWindowRect(rectReturnList);
		GetParent()->ScreenToClient(rectReturnList);
		rectReturnList.top		=	rectBar.bottom + 10;
		rectReturnList.bottom	=	rectReturnList.top + 12;
		m_pWndStaticReturnList->MoveWindow(rectReturnList);

		GetParent()->Invalidate(TRUE);
	}
	else
	{
		CRect rectBar;
		GetWindowRect(rectBar);
		GetParent()->ScreenToClient(rectBar);

		CRect rectLeft;
		m_pwndLeftPane->GetWindowRect(rectLeft);
		GetParent()->ScreenToClient(rectLeft);
		rectLeft.right		=	rectBar.left+GetSystemMetrics(SM_CYBORDER);
		m_pwndLeftPane->MoveWindow(rectLeft);

		CRect rectRight;
		m_pwndRightPane->GetWindowRect(rectRight);
		GetParent()->ScreenToClient(rectRight);
		rectRight.left		=	rectBar.right-GetSystemMetrics(SM_CYBORDER);;
		m_pwndRightPane->MoveWindow(rectRight);

	}
}
INT CSplitterBar::SetCtrlPosition(CRect rcReturnGroup , CRect rcReturnGrid , CRect rcReturnCaption)
{
	HDWP hdwp = ::BeginDeferWindowPos (4);

		::DeferWindowPos (hdwp, m_pwndRightPane->GetSafeHwnd() , HWND_TOP,
						rcReturnGrid.left, rcReturnGrid.top, rcReturnGrid.Width (), rcReturnGrid.Height (),
						SWP_NOZORDER | SWP_SHOWWINDOW);
		::DeferWindowPos (hdwp, m_pWndStaticReturnList->GetSafeHwnd() , HWND_TOP,
						rcReturnCaption.left, rcReturnCaption.top, rcReturnCaption.Width (), rcReturnCaption.Height (),
						SWP_NOZORDER | SWP_SHOWWINDOW);
		::DeferWindowPos (hdwp, this->GetSafeHwnd() , HWND_TOP,
						rcReturnGroup.left, rcReturnGroup.top-2 , rcReturnGrid.Width (), 4,
						SWP_NOZORDER | SWP_SHOWWINDOW);

	::EndDeferWindowPos (hdwp);
	
	return 0;
}

void CSplitterBar::ReSetSplitterMoveMostValue()
{	
	if(m_bHorizontal)
	{
		CRect	rectParent , rectLeft , rectGroup;

		m_pwndLeftPane->GetWindowRect(rectLeft);		
		GetParent()->ScreenToClient(rectLeft);
		m_cxLeftMost	=	rectLeft.top;

		GetParent()->GetWindowRect(rectParent);
		GetParent()->ScreenToClient(rectParent);
		
		m_pWndStaticReturnGroup->GetWindowRect(rectGroup);
		GetParent()->ScreenToClient(rectGroup);
		m_cxRightMost	=	rectParent.bottom	-	rectGroup.Height() - 4;

	}
}
