// SIReportPreviewView.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportPreviewView.h"

#include "efs.h"

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
	ON_WM_MOUSEWHEEL()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView drawing

VOID CSIReportPreviewView::OnInitialUpdate()
{
EFS_BEGIN

	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
	sizeTotal = CSize(PAPER_A4_X, PAPER_A4_Y);
	SetScrollSizes(MM_LOMETRIC, sizeTotal);

	m_page = -1;

	EnableThemeDialogTexture(GetSafeHwnd());

EFS_END
}

VOID CSIReportPreviewView::OnDraw(CDC* pDC)
{
EFS_BEGIN

	CDocument* pDoc = GetDocument();
	if (m_page >= 1) {
		m_pManager->ShowXMLFileOnePage(MANAGER_MODE_PREVIEW, pDC, m_page);
		//m_pManager->ShowXMLFilePage(MANAGER_MODE_PREVIEW, pDC, m_page, m_page);
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView diagnostics

#ifdef _DEBUG
VOID CSIReportPreviewView::AssertValid() const
{
EFS_BEGIN

	CScrollView::AssertValid();

EFS_END
}

VOID CSIReportPreviewView::Dump(CDumpContext& dc) const
{
EFS_BEGIN

	CScrollView::Dump(dc);

EFS_END
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewView message handlers

INT CSIReportPreviewView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
EFS_BEGIN

	return 0;

EFS_END
return -1;

}


BOOL CSIReportPreviewView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	

	INT nPos = GetScrollPos(SB_VERT) ;
	INT nMinPos,nMaxPos ;
	GetScrollRange(SB_VERT,&nMinPos,&nMaxPos);

	if(zDelta < 0 )
	{
		if(nPos+100> nMaxPos)
		{
			SetScrollPos(SB_VERT,nMaxPos,true);
		}
		else
		{
			SetScrollPos(SB_VERT,nPos+100,true);
		}
	}
	if(zDelta > 0)
	{
		if(nPos-100 < 0 )
		{
			SetScrollPos(SB_VERT,0,true);
		}
		else
		{
			SetScrollPos(SB_VERT,nPos-100,true);
		}
	}

	// 2005.02.18 ADD BY PDJ
	// 깜박임 제거
	INT nPos2 = GetScrollPos(SB_VERT) ;
	if ( nPos != nPos2 ) Invalidate();
	

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

HBRUSH CSIReportPreviewView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

// 2008.11.10 ADD BY PWR
// 다이얼로그 XP테마적용
VOID CSIReportPreviewView::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));	// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);	// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}