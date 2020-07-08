// FenceObject.cpp: implementation of the CFenceObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "FenceObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFenceObject::CFenceObject()
{
	m_pLeftBrace = NULL;
	m_pRightBrace = NULL;
	SetSize(20, 20);
	m_bVisible = false;

	// KOL.UDF.022 시큐어코딩 보완
	m_pRightFence = NULL;
	m_iType = -1;
	m_bIsLeft = FALSE;
}

CFenceObject::~CFenceObject()
{
//	if(m_pLeftBrace != NULL) delete m_pLeftBrace;
//	if(m_pRightBrace != NULL) delete m_pRightBrace;
}

VOID CFenceObject::SetType(INT type)
{
	m_iType = type;
}

bool CFenceObject::IsVisible()
{
	return m_bVisible;
}

VOID CFenceObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

VOID CFenceObject::DrawObject(CDC *pDC, INT iX, INT iY)
{
	CFont Font, *oldFont;
	CString msg;
	INT fontSize = 20;
	CString strFont = _T("Symbol");

	if(m_iHeight > 0) {
		fontSize = m_iHeight;

		Font.CreateFont(m_iHeight, m_iWidth, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);
	}
	else {
		Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH, strFont);
	}

	oldFont = pDC ->SelectObject(&Font);

	/////////////////////////////////////////////////////////////////

	if(m_bIsLeft == true) {
		if(m_iType == 0) {
			msg.Format(_T("%c"), 0x28);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 1) {
			msg.Format(_T("%c"), 0x5B);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 2) {
			msg.Format(_T("%c"), 0x7B);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 3) {
			msg.Format(_T("%c"), 0xE1);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 4) {
			msg.Format(_T("%c"), 0x7C);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 5) {
			pDC ->SelectObject(oldFont);
			Font.DeleteObject();

			strFont = _T("MT Extra");

			Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						PROOF_QUALITY, DEFAULT_PITCH, strFont);

			oldFont = pDC ->SelectObject(&Font);


			msg.Format(_T("%c"), 0x50);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 6) {
			msg.Format(_T("%c"), 0xEB);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 7) {
			msg.Format(_T("%c"), 0xE9);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 8) {
			INT xAlpha, yAlpha;
			pDC ->SelectObject(oldFont);
			Font.DeleteObject();

			strFont = _T("MT Extra");
			fontSize = 10;

			Font.CreateFont(fontSize + 10, fontSize + 5, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							PROOF_QUALITY, DEFAULT_PITCH, strFont);

			oldFont = pDC ->SelectObject(&Font);

			INT from_height, to_height;
			xAlpha = 0;
			yAlpha = 10;

			
			from_height = 0; to_height = 10;

			while(true) {
				xAlpha += 10;
				if((m_iHeight >= from_height) && m_iHeight < to_height) {
					break;
				}
				from_height = to_height;
				to_height += 20;
			}

			msg.Format(_T("%c"), 0x36);
			pDC ->TextOut(iX, iY - yAlpha, msg);
			msg.Format(_T("%c"), 0x37);
			pDC ->TextOut(iX + m_iWidth - 5, iY - yAlpha, msg);
			msg.Format(_T("%c"), 0x38);
			pDC ->TextOut(iX + (m_iWidth - 5)* 2, iY - yAlpha, msg);

			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX + fontSize + 10, iY + 2);
			pDC ->LineTo(iX + m_iWidth - 5, iY + 2);

			pDC ->MoveTo(iX + m_iWidth - 5 + fontSize + 10, iY + 2);
			pDC ->LineTo(iX + (m_iWidth - 5)* 2, iY + 2);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();
		}
		else if(m_iType == 9) {
			INT xAlpha, yAlpha;
			pDC ->SelectObject(oldFont);
			Font.DeleteObject();

			strFont = _T("MT Extra");
			fontSize = 10;

			Font.CreateFont(fontSize + 10, fontSize + 5, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							PROOF_QUALITY, DEFAULT_PITCH, strFont);

			oldFont = pDC ->SelectObject(&Font);

			INT from_height, to_height;
			xAlpha = 0;
			yAlpha = 10;

			
			from_height = 0; to_height = 10;

			while(true) {
				xAlpha += 10;
				if((m_iHeight >= from_height) && m_iHeight < to_height) {
					break;
				}
				from_height = to_height;
				to_height += 20;
			}

			msg.Format(_T("%c"), 0x31);
			pDC ->TextOut(iX, iY - yAlpha, msg);
			msg.Format(_T("%c"), 0x32);
			pDC ->TextOut(iX + m_iWidth - 5, iY - yAlpha, msg);
			msg.Format(_T("%c"), 0x33);
			pDC ->TextOut(iX + (m_iWidth - 5)* 2, iY - yAlpha, msg);

			CPen pen, *oldPen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = pDC ->SelectObject(&pen);

			pDC ->MoveTo(iX + fontSize + 10, iY + 2);
			pDC ->LineTo(iX + m_iWidth - 5, iY + 2);

			pDC ->MoveTo(iX + m_iWidth - 5 + fontSize + 10, iY + 2);
			pDC ->LineTo(iX + (m_iWidth - 5)* 2, iY + 2);

			pDC ->SelectObject(oldPen);
			pen.DeleteObject();



		}
		else if(m_iType == 10) {
			msg.Format(_T("%c"), 0x28);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 11) {

		}
		else if(m_iType == 12) {
			msg.Format(_T("%c"), 0x5B);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 13) {

		}
		else if(m_iType == 14) {
			msg.Format(_T("%c"), 0x7B);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 15) {

		}
		else if(m_iType == 16) {
			msg.Format(_T("%c"), 0xE1);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 17) {

		}
		else if(m_iType == 18) {
			msg.Format(_T("%c"), 0x7C);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 19) {

		}
		else if(m_iType == 20) {
			pDC ->SelectObject(oldFont);
			Font.DeleteObject();

			strFont = _T("MT Extra");

			Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						PROOF_QUALITY, DEFAULT_PITCH, strFont);

			oldFont = pDC ->SelectObject(&Font);


			msg.Format(_T("%c"), 0x50);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 21) {

		}	
	}
	else if(m_bIsLeft == false) {
		if(m_iType == 0) {
			msg.Format(_T("%c"), 0x29);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 1) {
			msg.Format(_T("%c"), 0x5D);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 2) {
			msg.Format(_T("%c"), 0x7D);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 3) {
			msg.Format(_T("%c"), 0xF1);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 4) {
			msg.Format(_T("%c"), 0x7C);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 5) {
			pDC ->SelectObject(oldFont);
			Font.DeleteObject();

			strFont = _T("MT Extra");

			Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						PROOF_QUALITY, DEFAULT_PITCH, strFont);

			oldFont = pDC ->SelectObject(&Font);

			msg.Format(_T("%c"), 0x50);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 6) {
			msg.Format(_T("%c"), 0xFB);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 7) {
			msg.Format(_T("%c"), 0xF9);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 8) {

		}
		else if(m_iType == 9) {

		}
		else if(m_iType == 10) {

		}
		else if(m_iType == 11) {
			msg.Format(_T("%c"), 0x29);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 12) {

		}
		else if(m_iType == 13) {
			msg.Format(_T("%c"), 0x5D);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 14) {

		}
		else if(m_iType == 15) {
			msg.Format(_T("%c"), 0x7D);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 16) {

		}
		else if(m_iType == 17) {
			msg.Format(_T("%c"), 0xF1);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 18) {

		}
		else if(m_iType == 19) {
			msg.Format(_T("%c"), 0x7C);
			pDC ->TextOut(iX, iY, msg);
		}
		else if(m_iType == 20) {

		}
		else if(m_iType == 21) {
			pDC ->SelectObject(oldFont);
			Font.DeleteObject();

			strFont = _T("MT Extra");

			Font.CreateFont(fontSize, fontSize, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						PROOF_QUALITY, DEFAULT_PITCH, strFont);

			oldFont = pDC ->SelectObject(&Font);


			msg.Format(_T("%c"), 0x50);
			pDC ->TextOut(iX, iY, msg);
		}	
	}
	/////////////////////////////////////////////////////////////////
	pDC ->SelectObject(oldFont);
	Font.DeleteObject();
}

CString CFenceObject::GetObject()
{
	CString object = _T("Not Implemented[FenceObject]");

	if(m_iType == 0) {
		if(m_bIsLeft == true)
			object = _T("\\left( { ");
		else
			object = _T(" } \\right)");
	}
	else if(m_iType == 1) {
		if(m_bIsLeft == true) 
			object = _T("\\left[ { ");
		else
			object = _T(" } \\right]");
	}
	else if(m_iType == 2) {
		if(m_bIsLeft == true) 
			object = _T("\\left{ { ");
		else
			object = _T(" } \\right}");
	}
	else if(m_iType == 3) {
		if(m_bIsLeft == true) 
			object = _T("\\left< { ");
		else
			object = _T(" } \\right>");
	}
	else if(m_iType == 4) {
		if(m_bIsLeft == true) 
			object = _T("\\left| { ");
		else
			object = _T(" } \\right|");
	}
	else if(m_iType == 5) {
		if(m_bIsLeft == true) 
			object = _T("\\left\\| { ");
		else
			object = _T(" } \\right\\|");
	}
	else if(m_iType == 6) {
		if(m_bIsLeft == true) 
			object = _T("\\left\\lfloor { ");
		else
			object = _T(" } \\right\\rfloor");
	}
	else if(m_iType == 7) {
		if(m_bIsLeft == true) 
			object = _T("\\left\\lceil { ");
		else
			object = _T(" } \\right\\rceil");
	}
	else if(m_iType == 8) {
		if(m_bIsLeft == true) 
			object = _T("\\overbrace ");
		else
			object = _T(" ^");
	}
	else if(m_iType == 9) {
		if(m_bIsLeft == true) 
			object = _T("\\underbrace ");
		else
			object = _T(" _");
	}
	else if(m_iType == 10) {
		if(m_bIsLeft == true) 
			object = _T("\\left( { ");
		else
			object = _T(" } \\right.");
	}
	else if(m_iType == 11) {
		if(m_bIsLeft == true) 
			object = _T("\\left. { ");
		else
			object = _T(" } \\right)");
	}
	else if(m_iType == 12) {
		if(m_bIsLeft == true) 
			object = _T("\\left[ { ");
		else
			object = _T(" } \\right.");
	}
	else if(m_iType == 13) {
		if(m_bIsLeft == true) 
			object = _T("\\left. { ");
		else
			object = _T(" } \\right]");
	}
	else if(m_iType == 14) {
		if(m_bIsLeft == true) 
			object = _T("\\left{ { ");
		else
			object = _T(" } \\right.");
	}
	else if(m_iType == 15) {
		if(m_bIsLeft == true) 
			object = _T("\\left. { ");
		else
			object = _T(" } \\right}");
	}
	else if(m_iType == 16) {
		if(m_bIsLeft == true) 
			object = _T("\\left\\langle { ");
		else
			object = _T(" } \\right.");
	}
	else if(m_iType == 17) {
		if(m_bIsLeft == true) 
			object = _T("\\left. { ");
		else
			object = _T(" } \\right\\langle");
	}
	else if(m_iType == 18) {
		if(m_bIsLeft == true) 
			object = _T("\\left| { ");
		else
			object = _T(" } \\right.");
	}
	else if(m_iType == 19) {
		if(m_bIsLeft == true) 
			object = _T("\\left. { ");
		else
			object = _T(" } \\right|");
	}
	else if(m_iType == 20) {
		if(m_bIsLeft == true) 
			object = _T("\\left\\| { ");
		else
			object = _T(" } \\right.");
	}
	else if(m_iType == 21) {
		if(m_bIsLeft == true) 
			object = _T("\\left. { ");
		else
			object = _T(" } \\right\\|");
	}

	return object;
}

VOID CFenceObject::GetSize(RECT &rect)
{
	rect.bottom = 0;
	rect.left = 0;
	rect.right = 40;
	rect.top = 20;

	if((m_iType == 8) && (m_bIsLeft == false)) {
		rect.right = 0;
		rect.top = 0;	
	}
}


VOID CFenceObject::SetIsLeft(bool bLeft)
{
	m_bIsLeft = bLeft;
}
