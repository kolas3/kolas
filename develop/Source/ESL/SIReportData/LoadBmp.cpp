// LoadBmp.cpp: implementation of the CLoadBmp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadBmp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadBmp::CLoadBmp()
{
	m_hDIB = NULL;
	m_pPal = NULL;
}

CLoadBmp::~CLoadBmp()
{
	if (m_hDIB) GlobalFree(m_hDIB);
	if (m_pPal) delete m_pPal;
}

VOID CLoadBmp::DrawDIB(CDC* pDC, INT sx, INT sy, INT dx, INT dy)
{
EFS_BEGIN

	if (!m_hDIB) return;
	LPVOID	lpDIBBits;		// Pointer to DIB bits
	BOOL	bSuccess=FALSE; 	// Success/fail flag

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
	INT nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors +
			bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	
	if( m_pPal && (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE) )
	{
		pDC->SelectPalette(m_pPal, FALSE);
		pDC->RealizePalette();
	}
	::StretchDIBits(
		pDC->m_hDC,                      // handle to DC
		sx,                    // x-coord of destination upper-left corner
		sy,                    // y-coord of destination upper-left corner
		dx*2,               // width of destination rectangle
		dy*2,              // height of destination rectangle
		0,                     // x-coord of source upper-left corner
		0,                     // y-coord of source upper-left corner
		dx,                // width of source rectangle
		dy,               // height of source rectangle
		lpDIBBits,           // bitmap bits
		(LPBITMAPINFO)m_hDIB, // bitmap data
		DIB_RGB_COLORS,                  // usage options
		SRCCOPY                   // raster operation code
	);

EFS_END
}

HBITMAP CLoadBmp::DIBToDDB( HANDLE hDIB )
{
	LPBITMAPINFOHEADER	lpbi;
	HBITMAP 		hbm;
	CPalette		pal;
	CPalette*		pOldPal;
	CClientDC		dc(NULL);

	if (hDIB == NULL)
		return NULL;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	INT nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 
						1 << lpbi->biBitCount;

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + 
			bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create and select a logical palette if needed
	if( nColors <= 256 && dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( INT i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		pal.CreatePalette( pLP );

		delete[] pLP;

		// Select and realize the palette
		pOldPal = dc.SelectPalette( &pal, FALSE );
		dc.RealizePalette();
	}

	
	hbm = CreateDIBitmap(dc.GetSafeHdc(),		// handle to device context
			(LPBITMAPINFOHEADER)lpbi,	// pointer to bitmap info header 
			(LONG)CBM_INIT,			// initialization flag
			lpDIBBits,			// pointer to initialization data 
			(LPBITMAPINFO)lpbi,		// pointer to bitmap info
			DIB_RGB_COLORS );		// color-data usage 

	if (pal.GetSafeHandle())
		dc.SelectPalette(pOldPal,FALSE);

	return hbm;
}

VOID CLoadBmp::DrawDIB(CDC* pDC, INT sx, INT sy)
{
EFS_BEGIN

	CRect size = GetImageSize();
	DrawDIB(pDC, sx, sy, size.Width(),size.Height());
	//DrawDIB(pDC, 0,0,50,50);

EFS_END
}

BOOL CLoadBmp::LoadBMP( LPCTSTR sBMPFile)
{
EFS_BEGIN

	if (m_hDIB) {
		GlobalFree(m_hDIB);
		m_hDIB = NULL;
	}
	if (m_pPal) {
		delete m_pPal;
		m_pPal = NULL;
	}
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead | CFile::typeBinary) )
		return FALSE;

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	BITMAPFILEHEADER bmfHeader;
	long nFileLen;

	nFileLen = file.GetLength();


	// Read file header
	if (file.Read((LPTSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	m_hDIB = ::GlobalAlloc(GMEM_FIXED, nFileLen);
	if (m_hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.ReadHuge((LPTSTR)m_hDIB, nFileLen - sizeof(BITMAPFILEHEADER)) !=
		nFileLen - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalFree(m_hDIB);
		return FALSE;
	}


	
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;

	INT nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;
	// Create the palette
	if( nColors <= 256 )
	{
		m_pPal = new CPalette;
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( INT i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		m_pPal->CreatePalette( pLP );

		delete[] pLP;
	} else {
		if (m_pPal) {
			delete m_pPal;
			m_pPal = NULL;
		}
	}

	return TRUE;

EFS_END
return FALSE;

}

VOID CLoadBmp::MakePallette(CPalette *pal)
{
EFS_BEGIN

	CWnd *pWnd = (CWnd*)GetDesktopWindow();
	CRect		rect;
	pWnd->GetWindowRect(rect);
	CWindowDC	dc(pWnd);

	if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = GetSystemPaletteEntries( dc, 0, 255, pLP->palPalEntry );
		pal->CreatePalette( pLP );
		delete[] pLP;
	}

EFS_END
}

CRect CLoadBmp::GetImageSize(VOID)
{
	CRect result;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
	if (!m_hDIB) return result;
	result.SetRect(0, 0, bmInfo.bmiHeader.biWidth, bmInfo.bmiHeader.biHeight);
	return result;
}	

INT CLoadBmp::GetMemorySize(VOID)
{
EFS_BEGIN

	if (!m_hDIB) return 0;
	return (GlobalSize(m_hDIB));

EFS_END
return -1;

}

INT CLoadBmp::DrawDIB2(CDC*pDC, INT left, INT top, INT width, INT height)
{
EFS_BEGIN

	if (!m_hDIB) return -1;
	LPVOID	lpDIBBits;		// Pointer to DIB bits
	BOOL	bSuccess=FALSE; 	// Success/fail flag

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
	INT nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors +
			bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	
	if( m_pPal && (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE) )
	{
		pDC->SelectPalette(m_pPal, FALSE);
		pDC->RealizePalette();
	}
	CRect r = GetImageSize();
	::StretchDIBits(
		pDC->m_hDC,                      // handle to DC
		left,                    // x-coord of destination upper-left corner
		top,                    // y-coord of destination upper-left corner
		width,               // width of destination rectangle
		height,              // height of destination rectangle
		0,                     // x-coord of source upper-left corner
		0,                     // y-coord of source upper-left corner
		r.Width(),                // width of source rectangle
		r.Height(),               // height of source rectangle
		lpDIBBits,           // bitmap bits
		(LPBITMAPINFO)m_hDIB, // bitmap data
		DIB_RGB_COLORS,                  // usage options
		SRCCOPY                   // raster operation code
	);
	return 0;

EFS_END
return -1;

}
