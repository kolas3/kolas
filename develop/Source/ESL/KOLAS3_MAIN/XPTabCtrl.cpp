/***********************************************************************************************************/
// XPTabCtrl.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
//	Compiler:	Visual C++, Service Pack 5, Windows NT 
//	Tested on:	Windows NT, Windows XP
//
//	Created:	25/February/2004
//	Updated:	
//
//	Author:		Adi DEDIC
//  e-mail:		adi_dedic@hotmail.com
//  www:		http://web.onetel.net.uk/~adidedic/
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE ? (ONE GB PUND) TO THE AUTHOR
/***********************************************************************************************************/
#include "StdAfx.h"
#include "XPTabCtrl.h"
#include "MToolBar.h"

//#define USE_DEFAULT_XP_TOPTAB		// XP top tab is drawn only for test purpose. To use default, uncoment this line

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***********************************************************************************************************/
// constant string definitions here (or you can put it into resource string table)
#define IDS_UTIL_TAB            _T("TAB")
#define IDS_UTIL_UXTHEME        _T("UxTheme.dll")
#define IDS_UTIL_THEMEACT       "IsThemeActive"
#define IDS_UTIL_THEMEOPN       "OpenThemeData"
#define IDS_UTIL_THEMEBCKG      "DrawThemeBackground"
/***********************************************************************************************************/
// CXPTabCtrl
/***********************************************************************************************************/
CXPTabCtrl::CXPTabCtrl()
{
	m_bTabExtended=TRUE;			// default is to use extended tab (if it is Themes XP)
	m_eTabOrientation=e_tabBottom;	// default initial orientation is: bottom
	m_ixSelOld=-1;
	m_pButton = NULL;

	m_check_flag = 0;
	m_old_mouse_index = -1;
	// KOL.UDF.022
	m_pParent = NULL;
}
CXPTabCtrl::~CXPTabCtrl()
{
	m_Dlg_brush.DeleteObject();
	m_pBkBrush.DeleteObject();
	if ( m_pButton != NULL ) 
	{
		delete m_pButton;
	}	
}
//----------------------------------------------------------------------------------------------------------
// nBitmapID Resource IDs of the bitmap to be associated with the image list.
void CXPTabCtrl::InitImageList(UINT nBitmapID)
{
	if(!::IsWindow(GetSafeHwnd()) || m_ilTabs.operator HIMAGELIST())
		{ ASSERT(FALSE); return; }				// tab control has to be created first and image list can be created only once

	CBitmap bm;
    bm.LoadBitmap(IDB_TABIMAGES);
    
    m_ilTabs.Create(16, 21, ILC_COLOR24 | ILC_MASK, 0, 2);
    m_ilTabs.Add( &bm, RGB(0xFF,0x00,0xFF) );
	SetImageList(&m_ilTabs);

	m_Dlg_brush.CreateSolidBrush(RGB(240, 243, 249));
	m_pBkBrush.CreateSolidBrush(RGB(252,252,252));	
	/*
	if(m_ilTabs.Create(nBitmapID, 16, 1, RGB(0xFF,0,0xFF)))	// add an images list with appropriate background (transparent) color
		SetImageList(&m_ilTabs);
	*/
}
//----------------------------------------------------------------------------------------------------------
// only three messages used
BEGIN_MESSAGE_MAP(CXPTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CXPTabCtrl)
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TCN_SELCHANGING,OnTabSelChanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE,  OnTabSelChanged)
	ON_BN_CLICKED(IDC_TABBUTTON, OnCloseTab)
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//----------------------------------------------------------------------------------------------------------
void CXPTabCtrl::OnPaint() 
{
	EFS_BEGIN

	if(!IsExtendedTabTheamedXP())			// if it is not XP or it is not Themes, behave as default
		{ Default(); return; }	
	
	CPaintDC dc(this);						// device context for painting

	CRect rcClip; rcClip.SetRectEmpty();
	dc.GetClipBox(rcClip);

	// Bk Paint rect
	CRect rectTabTopLeftBk;
	CRect rectTabTopRightBk;
	CRect rectTabBk;

	// 1st paint the tab body
	CRect rcPage,rcItem,rcClient, rcXItem;
	GetClientRect(&rcPage);
	rcClient=rcPage;
	AdjustRect(FALSE,rcPage);
	
	switch(m_eTabOrientation)
	{	case e_tabTop:	  rcClient.top   =rcPage.top   -2; break;
		case e_tabBottom: rcClient.bottom=rcPage.bottom+3; break;
		case e_tabLeft:	  rcClient.left  =rcPage.left  -1; break;
		case e_tabRight:  rcClient.right =rcPage.right +3; break;
		default: ASSERT(FALSE); return;
	}	
	UINT uiVertBottm;
	uiVertBottm =(m_eTabOrientation&1)? 8:0;		//  8=bottom
	uiVertBottm|=(m_eTabOrientation&2)?16:0;		// 16=vertical
	UINT uiFlags=1|uiVertBottm;						//  1=body		
	DrawThemesXpTabItem(&dc, -1, rcClient,uiFlags);	// TABP_PANE=9,0,'TAB'

	rectTabBk.right = rcClient.right;
	
	int nTab=GetItemCount();						// paint the tabs first and then the borders
	if(!nTab) return;								// no tab pages added

	// 2nd paint the inactive tabs
	CRect ;
	TCHITTESTINFO hti;	hti.flags=0;
	::GetCursorPos(&hti.pt); ScreenToClient(&hti.pt);
	int ixHot=HitTest(&hti);
	int ixSel=GetCurSel();
		
	for(int ixTab=0; ixTab<nTab; ixTab++)
	{
		if ( ixTab == 0 && ixSel == 0 )
		{
			if ( m_pButton != NULL ) 
			{
				m_pButton->ShowWindow(SW_HIDE);
			}
		}
		if(ixTab==ixSel) 
		{
			VERIFY(GetItemRect(ixTab, &rcItem));						
			if ( ixSel != 0 )
			{				
				rcXItem.left	= rcItem.right - 20;
				rcXItem.right	= rcItem.right - 3;				
				rcXItem.bottom	= rcItem.bottom-3;
				rcXItem.top		= rcItem.top + 5;
				if ( m_pButton == NULL ) 
				{
					m_pButton = new CYSButton();
					m_pButton->Create(_T("x"),WS_VISIBLE|WS_CHILD|BS_OWNERDRAW, rcXItem, this, IDC_TABBUTTON);
					m_pButton->LoadBitmaps("..\\BMP\\txbtn.bmp", RGB(0,0,0));
				}
				m_pButton->MoveWindow(rcXItem);
				m_pButton->ShowWindow(SW_SHOW);
			}					
			// left top Tab Bk rect
			rectTabTopLeftBk.right = rcItem.left-2;	
			// right top Tab Bk rect
			rectTabTopRightBk.left = rcItem.right;		
			continue;
		}
		VERIFY(GetItemRect(ixTab, &rcItem));		
		if(m_eTabOrientation==e_tabLeft) rcItem.right++;
		uiFlags=uiVertBottm|(ixTab==ixHot?4:0);		// 4= hot
		DrawThemesXpTabItem(&dc,ixTab,rcItem,uiFlags);
	}

	// Bk Fill Rect
	rectTabTopLeftBk.left = 0;
	rectTabTopLeftBk.top = 0;	
	rectTabTopLeftBk.bottom = 2;

	rectTabTopRightBk.top = 0;	
	rectTabTopRightBk.bottom = 2;
	rectTabTopRightBk.right = rcItem.right;
	
	rectTabBk.left = rcItem.right;	
	rectTabBk.top = 0;
	rectTabBk.bottom = rcItem.bottom;	

	// 3rd paint the active selected tab
	if ( ixSel == -1 ) return;	
	VERIFY(GetItemRect(ixSel, &rcItem));			// now selected tab
	rcItem.InflateRect(2,2);
	if(m_eTabOrientation==e_tabTop) rcItem.bottom--;

	uiFlags=uiVertBottm|2;							// 2= selected
	DrawThemesXpTabItem(&dc, ixSel, rcItem,uiFlags);

	
	if ( ixSel == nTab-1 )
	{
		rectTabBk.left += 2;
		rectTabTopRightBk.left += 2;
	}	
	
	if ( nTab > 1 )
	{
		if ( ixSel == 0 )
		{
			rectTabTopRightBk.left += 2;
			dc.FillRect(rectTabTopRightBk, &m_pBkBrush);
		}
		else if ( nTab-1 == ixSel )
		{
			dc.FillRect(rectTabTopLeftBk, &m_pBkBrush);
			dc.FillRect(CRect(0,0,2,26), &m_pBkBrush);
		}
		else
		{
			rectTabTopRightBk.left += 2;
			dc.FillRect(rectTabTopLeftBk, &m_pBkBrush);
			dc.FillRect(rectTabTopRightBk, &m_pBkBrush);
			dc.FillRect(CRect(0,0,2,26), &m_pBkBrush);
		}		
	}
	
	dc.FillRect(rectTabBk, &m_pBkBrush);

	/*
	CRect scrollRect;
	GetWindowRect(scrollRect);	
	scrollRect.left = scrollRect.right-32;
	dc.FillRect(scrollRect, &m_pBkBrush);
	*/

	EFS_END
}
//----------------------------------------------------------------------------------------------------------
// This function draws Themes Tab control parts: a) Tab-Body and b) Tab-tabs
void CXPTabCtrl::DrawThemesXpTabItem(CDC* pDC, int ixItem, const CRect& rcItem, UINT uiFlag) 
{			
	EFS_BEGIN

	if ( pDC == NULL ) return;
	BOOL bBody  =(uiFlag& 1)?TRUE:FALSE;
	BOOL bSel   =(uiFlag& 2)?TRUE:FALSE;
	BOOL bHot   =(uiFlag& 4)?TRUE:FALSE;
	BOOL bBottom=(uiFlag& 8)?TRUE:FALSE;	// mirror
	BOOL bVertic=(uiFlag&16)?TRUE:FALSE;	// rotate
	BOOL bLeftTab=!bBottom && bVertic && !bBody;
	
	CSize szBmp=rcItem.Size();
	if(bVertic) SwapVars(szBmp.cx,szBmp.cy);
	
	// 1st draw background
	CDC     dcMem;	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmpMem; bmpMem.CreateCompatibleBitmap(pDC,szBmp.cx,szBmp.cy);
	CBitmap* pBmpOld=dcMem.SelectObject(&bmpMem);
	CRect rectOutLine;
	CPen pen;
	//if ( uiFlag == 1 ) szBmp.cy = 1;	
	CRect rcMem(CPoint(0,0),szBmp); if(bSel) rcMem.bottom++;	
	if(bBody)
	{
		 CRect rectOutLine;
		 rectOutLine = rcMem;		 

		 rectOutLine.top += 1;
		 DrawThemesPart(dcMem.GetSafeHdc(), 9, 0, IDS_UTIL_TAB, &rcMem);	// TABP_PANE=9,  0, 'TAB'
		 
		 dcMem.FillSolidRect(rectOutLine, RGB(184, 202, 245));	
		 
		 rectOutLine.top = rcMem.bottom-1;
		 dcMem.FillSolidRect(rectOutLine, RGB(139, 145, 159));
	}
	else 
	{
		int iStateId = bSel?3:(bHot?2:1);
		TRIVERTEX data1[2], data2[2];
		GRADIENT_RECT draw_rect1, draw_rect2;		
		
		if ( iStateId == 1 )
		{			
			MakeFillColor(rcMem, RGB(242, 245, 250), RGB(229, 234, 245), RGB(207, 215, 235), RGB(236, 238, 252), data1, data2);
		}
		else if ( iStateId == 2 )
		{			
			if ( bHot == TRUE )
			{
				MakeFillColor(rcMem, RGB(252, 253, 253), RGB(183, 221, 247), RGB(153, 198, 238), RGB(217, 233, 249), data1, data2);
			}
			else
			{
				MakeFillColor(rcMem, RGB(252, 253, 253), RGB(183, 221, 247), RGB(153, 198, 238), RGB(157, 233, 249), data1, data2);
			}
		}
		else  if ( iStateId == 3 )
		{			
			MakeFillColor(rcMem, RGB(252, 253, 253), RGB(231, 245, 251), RGB(207, 231, 250), RGB(183, 200, 246), data1, data2);			
		}
		
		draw_rect1.UpperLeft  = 0;
		draw_rect1.LowerRight = 1;		
		::GradientFill(dcMem.m_hDC, data1, 2, &draw_rect1, 1, GRADIENT_FILL_RECT_V);
		
		draw_rect2.UpperLeft  = 0;
		draw_rect2.LowerRight = 1;
		::GradientFill(dcMem.m_hDC, data2, 2, &draw_rect2, 1, GRADIENT_FILL_RECT_V);	

		// OutLine
		pen.CreatePen(PS_SOLID, 1, RGB(139, 145, 159));		
		dcMem.SelectObject(pen);
		rectOutLine = rcMem;

		// Start
		dcMem.MoveTo(rectOutLine.left, rectOutLine.bottom);
		dcMem.LineTo(rectOutLine.left, rectOutLine.top+3);
		dcMem.LineTo(rectOutLine.left+3, rectOutLine.top);
		dcMem.LineTo(rectOutLine.right-4, rectOutLine.top);
		dcMem.LineTo(rectOutLine.right-1, rectOutLine.top+3);
		dcMem.LineTo(rectOutLine.right-1, rectOutLine.bottom);
	}
																// TABP_TABITEM=1, TIS_SELECTED=3:TIS_HOT=2:TIS_NORMAL=1, 'TAB'
	// 2nd init some extra parameters
	BITMAPINFO biOut; ZeroMemory(&biOut,sizeof(BITMAPINFO));	// Fill local pixel arrays
	BITMAPINFOHEADER& bihOut=biOut.bmiHeader;
	bihOut.biSize  =sizeof (BITMAPINFOHEADER);
	bihOut.biCompression=BI_RGB;
	bihOut.biPlanes=1;		  bihOut.biBitCount=24;	// force as RGB: 3 bytes,24 bits -> good for rotating bitmap in any resolution
	bihOut.biWidth =szBmp.cx; bihOut.biHeight=szBmp.cy;

	int nBmpWdtPS=DWordAlign(szBmp.cx*3);
	int nSzBuffPS=((nBmpWdtPS*szBmp.cy)/8+2)*8;
	LPBYTE pcImg=NULL;
	if(bBottom || bVertic) { pcImg=new BYTE[nSzBuffPS]; ASSERT(pcImg); }
	int nStart=0,nLenSub=0;
	if(bBody && bBottom && !bVertic) nStart=3,nLenSub=4;	// if bottom oriented flip the body contest only (no shadows were flipped)
	// 3rd if it is left oriented tab, draw tab context before mirroring or rotating (before GetDIBits)
	if(bVertic)
	{	if(bBody || !bBottom) bihOut.biHeight=-szBmp.cy;
		if(!bBottom && !bBody && ixItem>=0)					// 
		{	if(bSel) rcMem.bottom--;
			DrawTabItem(&dcMem, ixItem, rcMem, uiFlag); ixItem=-1;
	}	}														// rotate or mirror
	// 4th get bits (for rotate) and mirror: body=(all except top) tab=(all except top)
	if(bVertic || bBottom)										// get bits: 
	{	GetDIBits(*pDC, bmpMem.operator HBITMAP(),nStart,szBmp.cy-nLenSub,pcImg,&biOut,DIB_RGB_COLORS);
		if(bBottom)									// mirror: body=(bottom and right) tab=(bottom and right)
		{	bihOut.biHeight=-szBmp.cy; 				// to mirror bitmap is eough to use negative height between Get/SetDIBits
			SetDIBits(*pDC, bmpMem.operator HBITMAP(),nStart,szBmp.cy-nLenSub,pcImg,&biOut,DIB_RGB_COLORS);
			if(bBody && bVertic)					// when it is right oriented body -> flip twice, first flip border shadows, than flip shaded inside body again
			{	nStart=2; nLenSub=4; bihOut.biHeight=szBmp.cy;
				GetDIBits(*pDC, bmpMem.operator HBITMAP(),nStart,szBmp.cy-nLenSub,pcImg,&biOut,DIB_RGB_COLORS);
				bihOut.biHeight=-szBmp.cy;			// to mirror bitmap is eough to use negative height between Get/SetDIBits
				SetDIBits(*pDC, bmpMem.operator HBITMAP(),nStart,szBmp.cy-nLenSub,pcImg,&biOut,DIB_RGB_COLORS);
	}	}	}
	// 5th if it is bottom or right oriented tab, draw after mirroring background (do GetDIBits again)
	if(!bBody && ixItem>=0)							// 
	{	if(bSel) rcMem.bottom--;		
		DrawTabItem(&dcMem, ixItem, rcMem, uiFlag);
		if(bVertic)											// if it is right tab, do GetDIBits again
		{	bihOut.biHeight=-szBmp.cy;
			GetDIBits(*pDC, bmpMem.operator HBITMAP(),nStart,szBmp.cy-nLenSub,pcImg,&biOut,DIB_RGB_COLORS);
	}	}
	// 6th: do rotate now, finaly
	if(bVertic)							// force rotating bitmap as RGB -> good for any resolution
	{	SwapVars(szBmp.cx,szBmp.cy);

		int nBmpWdtPD=DWordAlign(szBmp.cx*3);
		int nPadD=nBmpWdtPD-szBmp.cx*3;
		int nSzBuffPD=((nBmpWdtPD*szBmp.cy)/8+2)*8;
		LPBYTE pcImgRotate=new BYTE[nSzBuffPD]; ASSERT(pcImgRotate);
		int nWidth,nHeight=szBmp.cy,nHeight1=nHeight-1;
		//====================================
		//------------------------------------
								// here is the example how to speed up lengthy repeatetive processing by using inline assembler
	#define __USE_MASM__		// the same processing is in C and in asm. To use C -> comment the beginning of this line
								// Do the actual whole RGB bitmap rotating in C or assembler
	#ifndef __USE_MASM__
		LPBYTE pcImgS=pcImg;
		LPBYTE pcImgD=pcImgRotate;
		int ixHeight=0;
		BOOL bLast=FALSE;
		while(ixHeight<nHeight)					// for all destination height lines
		{	nWidth=szBmp.cx;
			if(ixHeight==nHeight1) { bLast=TRUE; nWidth--; }
			
			while(nWidth--)
			{	*(PDWORD)pcImgD=*(PDWORD)pcImgS; // do all Rgb triplets read/write qicker as DWORD
				pcImgS+=nBmpWdtPS;	// increment source in padded source lines
				pcImgD+=3;			// increment destination in rgb triplets
			}
			if(bLast)				// when the last line, the last pixel - colud be a problem if bitmap DWORD alligned 
				for(int c=3;c;c--) *pcImgD++=*pcImgS++;		// (only last three bytes available->could not read/write DWORD)!!
			else
			{	ixHeight++;
				pcImgD+=nPadD;				// destination bitmap horizontal padding to DWORD
				pcImgS=pcImg+(ixHeight*3);	// reset the source to the begining of the next vertical line
		}	}
	#else	// __USE_MASM__
			nBmpWdtPS-=4;					// adjust esi increment (due to esi self-incrementing by movsd)
			nWidth=szBmp.cx;
		__asm
		{		mov		esi, pcImg			// source index
				mov		edi, pcImgRotate	// destination index
				xor		ebx, ebx			// vertical counter
			loop_height:
				mov		ecx, nWidth			// horizontal counter
				cmp		ebx, nHeight1		// check is it the last line
				jne		loop_width
				dec		ecx					// if it is decremnt for the last pixel

			loop_width:
				movsd						// copies 4 bytes and increments source and destination by 4 (we need only 3 bytes copied 'one pixel' RGB triplet)
				dec		edi					// adjust edi to 'as incremented by 3'
				add		esi,nBmpWdtPS		// adjust esi to the next source line
				loop	loop_width			// loop one hotizontal destination line 

				cmp		ebx, nHeight1		// check is it the last line
				je		do_last				// if not last, do incrementing here
											
				inc		ebx					// increment vertical counter
				add		edi, nPadD			// adjust destination index by possible padding to DWORD
				mov		esi, ebx			// reset the source index: add vertical counter * 3 
				shl		esi, 1				// (is the same as * 2 +1*)
				add		esi, ebx			// +1*
				add		esi, pcImg			// add to the beginning of the source
				jmp		loop_height			// loop whole height

			do_last:						// the last pixel is done by
				movsw						// moving first two bytes
				movsb						// and than by moving the very last byte
		}
	#endif // 	__USE_MASM__
		dcMem.SelectObject(pBmpOld); bmpMem.DeleteObject();		// recreate rotated bitmap
		bmpMem.CreateCompatibleBitmap(pDC,szBmp.cx,szBmp.cy);
		dcMem.SelectObject(&bmpMem);
		bihOut.biWidth =szBmp.cx; bihOut.biHeight=bBody?-szBmp.cy:szBmp.cy;
		SetDIBits(*pDC, bmpMem.operator HBITMAP(),0,szBmp.cy,pcImgRotate,&biOut,DIB_RGB_COLORS); // set rotated bitmap bits
		// KOL.UDF.022
		//delete pcImgRotate;
		delete[] pcImgRotate;
	}
	// KOL.UDF.022
	//if(pcImg) delete pcImg;
	if(pcImg) delete[] pcImg;
	// 6th blit mirrored/rotated image to the screen
	pDC->BitBlt(rcItem.left,rcItem.top,szBmp.cx,szBmp.cy,&dcMem,0,0,SRCCOPY); // 
	dcMem.SelectObject(pBmpOld);

	CRect rect;
	GetWindowRect(rect);
	rect.left = rect.right-80;
	pDC->FillRect(rect, &m_pBkBrush);

	EFS_END
}
//----------------------------------------------------------------------------------------------------------
// draw tab item context: possible icon and text
void CXPTabCtrl::DrawTabItem(CDC* pDC, int ixItem, const CRect& rcItemC, UINT uiFlags)
{
	EFS_BEGIN
	
	TC_ITEM tci;   CString sText;
	tci.mask      =TCIF_TEXT | TCIF_IMAGE;
	tci.pszText   =sText.GetBuffer(128);
	tci.cchTextMax=127;
	GetItem(ixItem,&tci);
	sText.ReleaseBuffer();

	BOOL bSel   =(uiFlags&2)?TRUE:FALSE;
	BOOL bBottom=(uiFlags&8)?TRUE:FALSE;
	CRect rcItem=rcItemC;
	if(bSel) rcItem.bottom -= 1;
	else	 rcItem.bottom += 2;
	rcItem.left+=(bBottom?3:6);					// text & icon
	rcItem.top +=(bBottom?3:2) + (bSel ? 1 : (bBottom?0:3));

	int nOldMode=pDC->SetBkMode(TRANSPARENT);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());	// icon
	if(hilTabs && tci.iImage>=0)
	{	ImageList_Draw(hilTabs, tci.iImage, *pDC, rcItem.left+(bSel?2:0), rcItem.top+(bSel?0:-2), ILD_TRANSPARENT);
		rcItem.left+=19;
	}
	else rcItem.OffsetRect(-2,0);

	if(sText.GetLength())
	{	CFont* pOldFont=pDC->SelectObject(GetFont());		// prepare dc
		rcItem.right-=3;						// text
		CRect rect(0,0,rcItem.Width(),20);
		::DrawText(pDC->GetSafeHdc(),sText.GetBuffer(sText.GetLength()+4),-1,rect,DT_CALCRECT|DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS);
		sText.ReleaseBuffer();

		rcItem.OffsetRect((bBottom?-1:0),(bSel?1:-1));
		rcItem.left += 5;
		rcItem.top += 5;
		rcItem.bottom += 5;

		pDC->DrawText(sText, rcItem, DT_NOPREFIX|DT_CENTER);
		pDC->SelectObject(pOldFont);
	}
	pDC->SetBkMode(nOldMode);

	EFS_END	
}
//----------------------------------------------------------------------------------------------------------
BOOL CXPTabCtrl::IsExtendedTabTheamedXP()
{
	if(!m_bTabExtended || !::IsWindow(GetSafeHwnd()))
		return FALSE;
	DWORD dwStyle=GetStyle();
	m_eTabOrientation=(dwStyle&TCS_BOTTOM)?e_tabBottom:e_tabTop;
	if(dwStyle&TCS_VERTICAL)
		m_eTabOrientation=(m_eTabOrientation==e_tabTop)?e_tabLeft:e_tabRight;
#ifdef USE_DEFAULT_XP_TOPTAB
	if(m_eTabOrientation==e_tabTop)	return FALSE;
#endif
	return IsThemeActiveXP()?TRUE:FALSE;
}
//==========================================================================================================
// these two messages are necessary only to properly redraw deselected tab background, because 
void CXPTabCtrl::OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult)	// selected rect was inflated by 2 points
{
    UNUSED_ALWAYS(pNMHDR);
	m_ixSelOld=GetCurSel();
	*pResult=0L;
}
//----------------------------------------------------------------------------------------------------------
void CXPTabCtrl::OnTabSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    UNUSED_ALWAYS(pNMHDR);
	if(m_ixSelOld>=0 && m_ixSelOld!=GetCurSel()  && IsExtendedTabTheamedXP())			// else  
	{	
		CWnd* pWndParent=GetParent();
		if(pWndParent)
		{	
			CRect rcOldSel; GetItemRect(m_ixSelOld, rcOldSel); rcOldSel.InflateRect(2,2);
			ClientToScreen(&rcOldSel); pWndParent->ScreenToClient(&rcOldSel);
			pWndParent->InvalidateRect(rcOldSel);
		}	
	}
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1009, NULL, NULL);
	*pResult=1L;
}
/***********************************************************************************************************/
// Helper functions
/***********************************************************************************************************/
int DWordAlign(int n)
	{ const int rem=n%4; if(rem) n+=(4-rem); return n; }
//----------------------------------------------------------------------------------------------------------
BOOL IsThemeActiveEx()
{								// check theme activity always (could change during application running)
	HINSTANCE hDll=LoadLibrary(IDS_UTIL_UXTHEME);							// 'UxTheme.dll'
	if(hDll==NULL) return FALSE;				// the DLL won't be available on anything except Windows XP
	UINT (PASCAL *pfnIsThemeActive)();
	//(FARPROC&)pfnIsThemeActive=GetProcAddress(hDll,CString((LPCTSTR)IDS_UTIL_THEMEACT));	// 'IsThemeActive'
	(FARPROC&)pfnIsThemeActive=GetProcAddress(hDll,IDS_UTIL_THEMEACT);	// 'IsThemeActive'
	UINT uiThemeActive=0;
	if(pfnIsThemeActive)
		uiThemeActive=pfnIsThemeActive();			
	FreeLibrary(hDll);
	return uiThemeActive?TRUE:FALSE;
}
//----------------------------------------------------------------------------------------------------------
#define PACKVERSION(major,minor) MAKELONG(minor,major)
DWORD GetWinVersion()
{
	static DWORD c_dwWinVers=0;	// check win version only once (will not change during application)
	if(!c_dwWinVers)
	{	OSVERSIONINFO osvi;	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));	// Initialize the OSVERSIONINFO structure.
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);
		c_dwWinVers=PACKVERSION(osvi.dwMajorVersion,osvi.dwMinorVersion);
	}
	return c_dwWinVers;
}
//----------------------------------------------------------------------------------------------------------
BOOL IsWinXP()         { return GetWinVersion()>=PACKVERSION(5,1)?TRUE:FALSE; }

//----------------------------------------------------------------------------------------------------------
BOOL IsThemeActiveXP() { return (IsWinXP()  && IsThemeActiveEx())?TRUE:FALSE; }

//----------------------------------------------------------------------------------------------------------
#define WPART_NAME_SZ 128
HRESULT DrawThemesPart(HDC hDC, int iPartId, int iStateId, LPCTSTR uiPartNameID, LPRECT prcBox)
{
	if(!IsWinXP()) return E_FAIL;
	HINSTANCE hDll=LoadLibrary(IDS_UTIL_UXTHEME);								// 'UxTheme.dll'
	if(!hDll) return E_FAIL;
	BOOL (PASCAL* pfnIsThemeActive)();	UINT hTheme=0;
	//(FARPROC&)pfnIsThemeActive=GetProcAddress(hDll,CString((LPCTSTR)IDS_UTIL_THEMEACT));		// 'IsThemeActive'
	(FARPROC&)pfnIsThemeActive=GetProcAddress(hDll,IDS_UTIL_THEMEACT);		// 'IsThemeActive'
	HRESULT hResult=E_FAIL;
	if(pfnIsThemeActive && pfnIsThemeActive())
	{	
		if(_tcslen(uiPartNameID)>0)
		{	//WCHAR swPartName[WPART_NAME_SZ];
			//MultiByteToWideChar(CP_ACP,0,sPartName,-1,swPartName,sizeof(swPartName)/sizeof(WCHAR));
			UINT (PASCAL* pfnOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
			//(FARPROC&)pfnOpenThemeData=GetProcAddress(hDll,CString((LPCTSTR)IDS_UTIL_THEMEOPN));// 'OpenThemeData'
			(FARPROC&)pfnOpenThemeData=GetProcAddress(hDll,IDS_UTIL_THEMEOPN);// 'OpenThemeData'
			if(pfnOpenThemeData && (hTheme=pfnOpenThemeData(NULL, uiPartNameID))!=0)
			{	UINT (PASCAL* pfnDrawThemeBackground)(UINT htheme,HDC hdc,int iPartID,int iStateID,const RECT* prcBx,const RECT* prcClip);
				//(FARPROC&)pfnDrawThemeBackground=GetProcAddress(hDll,CString((LPCTSTR)IDS_UTIL_THEMEBCKG));	// 'DrawThemeBackground'
			(FARPROC&)pfnDrawThemeBackground=GetProcAddress(hDll,IDS_UTIL_THEMEBCKG);	// 'DrawThemeBackground'
				if(pfnDrawThemeBackground)
					hResult=pfnDrawThemeBackground(hTheme, hDC, iPartId, iStateId, prcBox, NULL);
	}	}	}
	FreeLibrary(hDll);
	return hResult;
}
//----------------------------------------------------------------------------------------------------------


VOID CXPTabCtrl::SetParent(CWnd	*pParent)
{
	m_pParent = pParent;
}

BOOL CXPTabCtrl::PreTranslateMessage(MSG* pMsg) 
{		
	return CTabCtrl::PreTranslateMessage(pMsg);
}

VOID CXPTabCtrl::OnCloseTab()
{
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1011, NULL, NULL);
}

CToolBar *CXPTabCtrl::CreateToolBar(UINT nDockBarID /*=0*/)
{	
	CMToolBar * pToolBar = NULL;
	pToolBar = new CMToolBar();	
	
	if (!pToolBar->CreateEx(this->GetDesktopWindow(), TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ))
	{
		delete pToolBar;
		return NULL;
	}
	pToolBar->MoveWindow(CRect(1,12,800,22));
	
	
	//pToolBar->EnableDocking(CBRS_ALIGN_ANY);
	//pFrame->DockControlBar(pToolBar, nDockBarID);

	return pToolBar;
}

CString CXPTabCtrl::GetSelText(INT nIdx)
{
	TC_ITEM tci;   CString sText;
	tci.mask      =TCIF_TEXT | TCIF_IMAGE;
	tci.pszText   =sText.GetBuffer(128);
	tci.cchTextMax=127;
	GetItem(nIdx,&tci);
	sText.ReleaseBuffer();

	if ( nIdx > 0 )
	{
		sText = sText.Left(sText.GetLength()-5);
	}

	return sText;
}

VOID CXPTabCtrl::SetSelText(INT nIdx, CString strText)
{
	TC_ITEM tci;   CString sText;
	tci.mask      =TCIF_TEXT | TCIF_IMAGE;
	tci.pszText   =sText.GetBuffer(128);
	tci.cchTextMax=127;
	GetItem(nIdx, &tci);
	sText.ReleaseBuffer();

	if ( strText == sText ) return;
	
	tci.pszText = strText.GetBuffer(0);
	_tcscpy(tci.pszText, strText );
	strText.ReleaseBuffer();
	SetItem(nIdx, &tci);
	m_pButton->Invalidate();
}

void CXPTabCtrl::MakeFillColor(CRect ItemRect, COLORREF TopRectRGB1, COLORREF TopRectRGB2, 
							                  COLORREF BottomRecRGB1, COLORREF BottomRecRGB2, 
											  TRIVERTEX* TopRect, TRIVERTEX* BottommRect)
{
	// 위 사각형 셋팅.
	TopRect[0].x  = ItemRect.left;
	TopRect[0].y  = ItemRect.top;

	TopRect[0].Red   = GetRValue(TopRectRGB1) * 255;
	TopRect[0].Green = GetGValue(TopRectRGB1) * 255;
	TopRect[0].Blue  = GetBValue(TopRectRGB1) * 255;
	TopRect[0].Alpha = 0x0000;

	TopRect[1].x  = ItemRect.right;
	TopRect[1].y  = ItemRect.bottom/3*1;

	TopRect[1].Red   = GetRValue(TopRectRGB2) * 255;
	TopRect[1].Green = GetGValue(TopRectRGB2) * 255;
	TopRect[1].Blue  = GetBValue(TopRectRGB2) * 255;
	TopRect[1].Alpha = 0x0000;

	// 아래 사각형 셋팅
	BottommRect[0].x = ItemRect.left;
	BottommRect[0].y = ItemRect.bottom/3*1;

	BottommRect[0].Red   = GetRValue(BottomRecRGB1) * 255;
	BottommRect[0].Green = GetGValue(BottomRecRGB1) * 255;
	BottommRect[0].Blue  = GetBValue(BottomRecRGB1) * 255;
	BottommRect[0].Alpha = 0x0000;

	BottommRect[1].x = ItemRect.right;
	BottommRect[1].y = ItemRect.bottom; 

	BottommRect[1].Red   = GetRValue(BottomRecRGB2) * 255;
	BottommRect[1].Green = GetGValue(BottomRecRGB2) * 255;
	BottommRect[1].Blue  = GetBValue(BottomRecRGB2) * 255;
	BottommRect[1].Alpha = 0x0000;
}

void CXPTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	::Beep(1000,50);
}

void CXPTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	/*
	if(m_check_flag == 0){ 
        m_check_flag = 1; 
        // 마우스 커서가 탭 영역을 벗어나더라도 마우스 메시지를 받도록 설정한다. 
        SetCapture(); 
    }
	
	INT MouseUp_index = -1;
	TCHITTESTINFO Info; 
    memset(&Info, 0x00, sizeof(Info)); 
    ::GetCursorPos(&Info.pt); 
    ScreenToClient(&Info.pt); 
    MouseUp_index = HitTest(&Info); 
	
	if ( MouseUp_index < 0 )
	{
		m_old_mouse_index = -1;
		//this->Invalidate(); 
	}	

	if(m_old_mouse_index != MouseUp_index)
	{ 
        // 마우스가 위치한 탭인덱스에 변화가 있으면 해당 인덱스값을 저장한다. 
        m_old_mouse_index = MouseUp_index; 
        // 컨트롤을 갱신한다. 
        this->Invalidate(); 
    }
	
	if(MouseUp_index == -1 && m_check_flag == 1){ 
        m_check_flag = 0; 
        // 마우스 메시지를 받도록 설정한 기능을 해제한다. 
        ReleaseCapture(); 
    } 		
	*/
	CTabCtrl::OnMouseMove(nFlags, point);
}

void CXPTabCtrl::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTabCtrl::OnMButtonUp(nFlags, point);

	FILE *fp = _tfopen(_T("..\\cfg\\_DEBUG"), _T("rb"));
	if (NULL != fp)
	{
		fclose(fp);
		
		TCHITTESTINFO Info;
		memset(&Info, 0x00, sizeof(Info));
		::GetCursorPos(&Info.pt);
		ScreenToClient(&Info.pt);
		INT nIndex = HitTest(&Info);
		
		if(0 < nIndex)
		{
			SetCurSel(nIndex);
			m_ixSelOld = nIndex;
			LRESULT Result = 0;
			OnTabSelChanged(NULL, &Result);
			OnCloseTab();
		}
	}
}
