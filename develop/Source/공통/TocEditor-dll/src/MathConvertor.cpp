// MathConvertor.cpp : implementation file
//

#include "stdafx.h"
//#include _T("SamDCtoBMP2.h")



#include "MathConvertor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMathConvertor


CMathConvertor::CMathConvertor()
{
/*	m_pParser	= new CEcoMathParser_XML ;
	m_pEngine	= new CEcoMathEngine     ;
	m_pParser->SetEngine(m_pEngine) ;
	pos.x = 0;
	pos.y = 0;
*/
//	pDC = new CDC;

	// KOL.UDF.022 시큐어코딩 보완
	memBitmap = NULL;
	m_pParser = NULL;
	m_pEngine = NULL;
	
		
}

CMathConvertor::~CMathConvertor()
{
//	delete m_pParser;
//	delete m_pEngine;
}

/////////////////////////////////////////////////////////////////////////////
// CMathConvertor message handlers
BOOL CMathConvertor::Close()
{
//	delete m_pParser;
//	delete m_pEngine;
	delete memBitmap;

	m_memDC.DeleteDC();
	

	return TRUE;

}

VOID CMathConvertor::RemoveObjectTag(CString *cstmp)
{
	// <Object>부분을 뺀다..
	// Header부분..제거
	INT nStart, nEnd;
//	AfxMessageBox(cstmp);
	nStart = cstmp->Find(_T("<OBJECT"), -1);
	nEnd = cstmp->Find(_T("<PARAM NAME=\"Script\" VALUE=\""), nStart+1);
	cstmp->Delete(nStart, nEnd - nStart + sizeof(_T("<PARAM NAME=\"Script\" VALUE=\"")));
	
	// Tail부분.. 제거..
	nStart = cstmp->ReverseFind('\"');
	nEnd = cstmp->GetLength();
	cstmp->Delete(nStart, nEnd - nStart);
	
//	return cstmp2;
}

BOOL CMathConvertor::Load(LPCTSTR m_Script, CDC *pDC)
{
//	CString cstmp;
//	cstmp.Format(IDS_MATH_TXT);
	CString cstmp;
	CFile cf;
	DWORD dFLength;
	TCHAR *lpBuf;

	if(!cf.Open(m_Script, CFile::modeRead | CFile::typeBinary)) return FALSE;
	TCHAR cUni;
	cf.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		cf.SeekToBegin();
	}
	
	dFLength = cf.GetLength();
	lpBuf = (TCHAR *)malloc(sizeof(TCHAR) * (dFLength+1));
	cf.Read(lpBuf, dFLength);
	lpBuf[dFLength] = '\0';
	cf.Close();
	cstmp.Format(_T("%s"), lpBuf);
	
/*
	// <Object>부분을 뺀다..
	// Header부분..제거
	INT nStart, nEnd;
//	AfxMessageBox(cstmp);
	nStart = cstmp.Find(_T("<OBJECT"), -1);
	nEnd = cstmp.Find(_T("<PARAM NAME=\"Script\" VALUE=\""), nStart+1);
	cstmp.Delete(nStart, nEnd - nStart + sizeof(_T("<PARAM NAME=\"Script\" VALUE=\"")));
	
	// Tail부분.. 제거..
	nStart = cstmp.ReverseFind('\"');
	nEnd = cstmp.GetLength();
	cstmp.Delete(nStart, nEnd - nStart);
*/	
	

	m_pParser	= new CEcoMathParser_XML ;
	m_pEngine	= new CEcoMathEngine     ;
	m_pParser->SetEngine(m_pEngine) ;
	pos.x = 0;
	pos.y = 0;
	
	
	RemoveObjectTag(&cstmp);
	m_pParser->SetScript(cstmp);
	//m_pParser->SetScript(lpBuf);
	m_pParser->Parse();	

	SIZE size = {0,0};
	
	m_pEngine->GetSize(size,pos, pDC->m_hDC);
	rect.right = size.cx;
	rect.bottom = size.cy;
	rect.left = rect.top = 0;

	rect.right += 10;
	pos.y += 10;

	memBitmap = new CBitmap;
	m_memDC.CreateCompatibleDC(pDC);
	memBitmap->CreateCompatibleBitmap(pDC,size.cx + 10, size.cy + 10);

	CBitmap* pOldBitmap = m_memDC.SelectObject(memBitmap);
	m_memDC.FillSolidRect(0,0,size.cx + 10, size.cy + 10, BKCOLOR);
	m_memDC.SetBkColor(BKCOLOR);
	m_memDC.SetBkMode(TRANSPARENT);
	m_pEngine->Draw(m_memDC.m_hDC, pos);
//	m_memDC.SelectObject(pOldBitmap);

	
//	pDC->BitBlt(0,0,1024,768, pDC,0,0,SRCCOPY);
//	pDC->Draw3dRect(pos.x, pos.y - size.cy / 2, rect.right +10, rect.bottom , RGB(200, 200, 200), RGB(1, 1, 1));

	
	return TRUE;
}

BOOL CMathConvertor::WriteWindowToDIB( LPTSTR szFile)
{
	CBitmap 	bitmap;
	CDC 		memDC;
	CDC			*pDC = &m_memDC;

	memDC.CreateCompatibleDC(pDC);


	bitmap.CreateCompatibleBitmap(pDC, rect.Width()-5,rect.Height());

	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0 , rect.Width(), rect.Height(), pDC, 2, 10, SRCCOPY);
//	memDC.BitBlt(pos.x, pos.y , pos.x + rect.Width(), pos.y + rect.Height(), pDC, 0, 0, SRCCOPY);
//	memDC.Draw3dRect(0, 0, rect.Width(), rect.Height(), RGB(0,0,0), RGB(0,0,0));

	// Create logical palette if device support a palette
	CPalette pal;
	if( pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries( *pDC, 0, 255, pLP->palPalEntry );

		// Create the palette
		pal.CreatePalette( pLP );

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = DDBToDIB( bitmap, BI_RGB, &pal );

	if( hDIB == NULL )
		return FALSE;

	// Write it to file
	WriteDIB( szFile, hDIB );

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree( hDIB );
	return TRUE;
}

HANDLE CMathConvertor::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal ) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;

	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	INT nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;
}

BOOL CMathConvertor::WriteDIB( LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if( !file.Open( szFile, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) )
		return FALSE;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	INT nColors = 1 << lpbi->biBitCount;


	hdr.bfType = 'MB';
	hdr.bfSize = sizeof( BITMAPFILEHEADER ) + lpbi->biSize + sizeof( BITMAPINFOHEADER );
	hdr.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;

	// Write the file header 
	file.Write( &hdr, sizeof(hdr) );

	// Write the DIB header and the bits 
	file.Write( lpbi, GlobalSize(hDIB) );

	return TRUE;
}
