// StaticButton.cpp : implementation file
//

#include "stdafx.h"
#include "StaticButton.h"
#include <winuser.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticButton


CRect CStaticButton::rect;

CStaticButton::CStaticButton()
{
	Enable = TRUE;

//	Select = CreateCursor(AfxGetInstanceHandle(), 4, 1, 32, 32, aaa, aaa);
	Select = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	No  = AfxGetApp()->LoadStandardCursor(IDC_NO);
	Region = new CRgn;
	cTool = new CToolTipCtrl;

	// KOL.UDF.022 시큐어코딩 보완
	bActive = FALSE;
	Enable = FALSE;	
	Up = NULL;
	Down = NULL;	
}

CStaticButton::~CStaticButton()
{
	delete Region;
	delete cTool;
}


BEGIN_MESSAGE_MAP(CStaticButton, CStatic)
	//{{AFX_MSG_MAP(CStaticButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticButton message handlers

VOID CStaticButton::wSetBitmap(HBITMAP hBitmapUP, HBITMAP hBitmapDOWN, BOOL bAct, CRgn* rgn)
{
	Up		  = hBitmapUP;
	Down	  = hBitmapDOWN;
	bActive = bAct;

	if(bActive)
		SetBitmap(Up);
	else
		SetBitmap(Down);

	GetClientRect(rect);
	Region->CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

	if(rgn)
		Region->CombineRgn(Region, rgn, RGN_AND);

	if(cTool->Create(this, TTS_ALWAYSTIP | WS_POPUP | WS_EX_TOOLWINDOW))
		SetToolTip(_T(""));

	cTool->SetMaxTipWidth(150);
	cTool->SetDelayTime(100);
}

HBITMAP CStaticButton::GetBitmapUp()
{
	return Up;
}
HBITMAP CStaticButton::GetBitmapDown()
{
	return Down;
}

BOOL CStaticButton::ChangeCursor(CPoint point)
{
	if(Region->PtInRegion((POINT)point))
	{
		if(Enable)
			::SetCursor(Select);
		else
			::SetCursor(No);
		return TRUE;
	}
	return FALSE;
}

VOID CStaticButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(ChangeCursor(point) && !bActive && Enable)
	{
		SetTimer(1, 50, NULL);
		SetBitmap(Up);
		bActive = TRUE;
	}
	else if(!ChangeCursor(point) && bActive)
	{
		KillTimer(1);
		SetBitmap(Down);
		bActive = FALSE;
	}

	CStatic::OnMouseMove(nFlags, point);
}

VOID CStaticButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ChangeCursor(point);
	CStatic::OnLButtonDown(nFlags, point);
}

VOID CStaticButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ChangeCursor(point);
	CStatic::OnLButtonUp(nFlags, point);
}

VOID CStaticButton::OnTimer(UINT nIDEvent) 
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(!Region->PtInRegion((POINT)point))
	{
		SetBitmap(Down);
		bActive = FALSE;
		KillTimer(1);
	}

	CStatic::OnTimer(nIDEvent);
}

VOID CStaticButton::SetRegion(CRgn* rgn)
{
	if(Region)
	{
		delete Region;
		Region = new CRgn;
		Region->CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
		Region->CombineRgn(Region, rgn, RGN_AND);
	}
}

CRgn* CStaticButton::GetRegion()
{
	return Region;
}

VOID CStaticButton::SetToolTip(LPCTSTR lpToolTip)
{
	cTool->Activate(TRUE);
	cTool->AddTool(this, lpToolTip);
}

BOOL CStaticButton::PreTranslateMessage(MSG* pMsg) 
{
	cTool->RelayEvent(pMsg);	
	return CStatic::PreTranslateMessage(pMsg);
}

VOID CStaticButton::SetCursor(BOOL bSel, HCURSOR hCur)
{
	if(bSel)
		Select = hCur;
	else
		No = hCur;
}

HCURSOR CStaticButton::GetCursor(BOOL bSel)
{
	if(bSel)
		return Select;
	else
		return No;
}

VOID CStaticButton::SetEnable(BOOL B)
{ 
	Enable = B; 
	if(Enable)
		SetBitmap(Up);
	else
		SetBitmap(Down);
};
