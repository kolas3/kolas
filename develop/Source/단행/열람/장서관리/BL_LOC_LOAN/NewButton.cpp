// NewButton.cpp : implementation file
//

#include "stdafx.h"
#include "NewButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CNewButton

CNewButton::CNewButton()
{
	m_pBitmapUp = NULL;
	m_pBitmapDown = NULL;
	m_pBitmapFocus = NULL;

	m_bHover = FALSE;
	m_bTracking = FALSE;
}

CNewButton::~CNewButton()
{
	FreeBitmaps();
}


BEGIN_MESSAGE_MAP(CNewButton, CBitmapButton)
	//{{AFX_MSG_MAP(CNewButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewButton message handlers

VOID CNewButton::FreeBitmaps(VOID)
{
EFS_BEGIN

	if (m_pBitmapUp) {
		delete m_pBitmapUp;
		m_pBitmapUp = NULL;
	}
	if (m_pBitmapDown) {
		delete m_pBitmapDown;
		m_pBitmapDown = NULL;
	}
	if (m_pBitmapFocus) {
		delete m_pBitmapFocus;
		m_pBitmapFocus = NULL;
	}

EFS_END
}

BOOL CNewButton::LoadBitmap(CString bitmapup, CString bitmapdown, CString bitmapfocus)
{
EFS_BEGIN

	FreeBitmaps();
	m_pBitmapUp = new CBitmap;
	m_pBitmapDown = new CBitmap;
	m_pBitmapFocus = new CBitmap;

	m_pBitmapUp->Attach(::LoadImage(NULL,bitmapup, IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	m_pBitmapDown->Attach(::LoadImage(NULL,bitmapdown, IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	m_pBitmapFocus->Attach(::LoadImage(NULL,bitmapfocus, IMAGE_BITMAP,0,0,LR_LOADFROMFILE));

	BITMAP	bitmapbits;
	// 2010.07.16 UPDATE BY PWR : GetBitmap 에러수정
	if(NULL == m_pBitmapUp->m_hObject)
	{
		if(NULL == m_pBitmapDown->m_hObject)
		{
			if(NULL == m_pBitmapFocus->m_hObject)
			{
				return FALSE;
			}
			else
			{
				m_pBitmapFocus->GetBitmap(&bitmapbits);
			}
		}
		else
		{
			m_pBitmapDown->GetBitmap(&bitmapbits);
		}
	}
	else
	{
		m_pBitmapUp->GetBitmap(&bitmapbits);
	}
	//m_pBitmapUp->GetBitmap(&bitmapbits);
	m_ButtonSize.cy=bitmapbits.bmHeight;
	m_ButtonSize.cx=bitmapbits.bmWidth;
	SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER   );

	return TRUE;

EFS_END
return FALSE;

}

BOOL CNewButton::LoadBitmap(UINT bitmapup, UINT bitmapdown, UINT bitmapfocus)
{
EFS_BEGIN

	FreeBitmaps();
	m_pBitmapUp = new CBitmap;
	m_pBitmapDown = new CBitmap;
	m_pBitmapFocus = new CBitmap;

	m_pBitmapUp->LoadBitmap(bitmapup);
	m_pBitmapDown->LoadBitmap(bitmapdown);
	m_pBitmapFocus->LoadBitmap(bitmapfocus);

	BITMAP	bitmapbits;
	// 2010.07.16 UPDATE BY PWR : GetBitmap 에러수정
	if(NULL == m_pBitmapUp->m_hObject)
	{
		if(NULL == m_pBitmapDown->m_hObject)
		{
			if(NULL == m_pBitmapFocus->m_hObject)
			{
				return FALSE;
			}
			else
			{
				m_pBitmapFocus->GetBitmap(&bitmapbits);
			}
		}
		else
		{
			m_pBitmapDown->GetBitmap(&bitmapbits);
		}
	}
	else
	{
		m_pBitmapUp->GetBitmap(&bitmapbits);
	}
// 	m_pBitmapUp->GetBitmap(&bitmapbits);
	m_ButtonSize.cy=bitmapbits.bmHeight;
	m_ButtonSize.cx=bitmapbits.bmWidth;
	SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER   );
	
	return TRUE;

EFS_END
return FALSE;

}

VOID CNewButton::OnMouseMove(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}	
	CBitmapButton::OnMouseMove(nFlags, point);

EFS_END
}

VOID CNewButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	Invalidate();

EFS_END
}


LRESULT CNewButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

BOOL CNewButton::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CBitmapButton::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

VOID CNewButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
EFS_BEGIN

	// TODO: Add your code to draw the specified item

	CDC *mydc=CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(mydc);

	CBitmap * pOldBitmap;
	
	CPoint point(0,0);	
		
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		pOldBitmap = pMemDC -> SelectObject(m_pBitmapDown);
//		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		
	}
	else
	{
		if(m_bHover)
		{
			pOldBitmap = pMemDC -> SelectObject(m_pBitmapFocus);
//			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		}else
		{
			pOldBitmap = pMemDC -> SelectObject(m_pBitmapUp);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		}	
	}

	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;

EFS_END
}
