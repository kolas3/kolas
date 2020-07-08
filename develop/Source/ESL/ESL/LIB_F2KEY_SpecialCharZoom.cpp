// LIB_F2KEY_SpecialCharZoom.cpp : implementation file
//

#include "stdafx.h"
#include"except.h"
#include "LIB_F2KEY_SpecialCharZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLIB_F2KEY_SpecialCharZoom

CLIB_F2KEY_SpecialCharZoom::CLIB_F2KEY_SpecialCharZoom( )
{
}

CLIB_F2KEY_SpecialCharZoom::~CLIB_F2KEY_SpecialCharZoom()
{
}

BEGIN_MESSAGE_MAP(CLIB_F2KEY_SpecialCharZoom, CStatic)
	//{{AFX_MSG_MAP(CLIB_F2KEY_SpecialCharZoom)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLIB_F2KEY_SpecialCharZoom message handlers

VOID CLIB_F2KEY_SpecialCharZoom::Get_Font(LOGFONT * LogFont)
{

	LogFont->lfHeight         = -60;
	LogFont->lfWidth          = 0;
	LogFont->lfEscapement     = 0;
	LogFont->lfOrientation    = 0;
	LogFont->lfWeight         = 400;
	LogFont->lfItalic         = 0;
	LogFont->lfUnderline      = 0;
	LogFont->lfStrikeOut      = 0;
	LogFont->lfCharSet        = 129;
	LogFont->lfOutPrecision   = 3;
	LogFont->lfClipPrecision  = 2;
	LogFont->lfQuality        = 1;
	LogFont->lfPitchAndFamily = 49;
	_tcscpy(LogFont->lfFaceName,_T("돋음체"));

}

VOID CLIB_F2KEY_SpecialCharZoom::OnPaint() 
{

	CPaintDC dc(this); // device context for painting

    LOGFONT LogFont;
	CFont   Font, * pOldFont;
    CBrush  Brush( RGB( 255, 255, 255 ) ), *pBrush;
    CRect   Rect;
	
    // $1. Font를 Setting하고 만든다.
	Get_Font(&LogFont);
    Font.CreateFontIndirect(&LogFont); 
    pOldFont = dc.SelectObject(&Font);

    // $2. 영역의 좌표를 구한다.
    GetClientRect( Rect );
    
    // $3. 영역을 하얗게 칠한다.
    pBrush = dc.SelectObject( &Brush );
    dc.Rectangle( &Rect );
    dc.SelectObject( pBrush );

    // $4. Data를 화면에 Display한다.
    dc.DrawText( GetData, &Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
	dc.SelectObject(pOldFont);

}

VOID CLIB_F2KEY_SpecialCharZoom::ViewChar( CString & strData )
{

    GetData = strData;
    
    Invalidate();

}

