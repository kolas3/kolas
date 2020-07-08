// BackGround.cpp: implementation of the CBackGround class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BackGround.h"
#include "CharConvert.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackGround::CBackGround()
{

}

CBackGround::~CBackGround()
{

}



INT CBackGround::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex , INT nMode  )
{
	const INT nCnt = 1;
	CString sAlias[nCnt] = 
	{
		_T("백그라운드컬러")
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , nIndex );
	
	dBackGroundColor = _ttoi(sResult[0]);

	CBitMap::InsertValue( pCardDM , nIndex , nMode );
	
	return 0;
}

INT CBackGround::DrawDC( CDC* pDC )
{
	if( sFilePath.IsEmpty() ) return 0;
	
	INT ids;
	
	CFile fFile;
	ids = fFile.Open( sFilePath , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 )
	{
		return 0;
	}

	DWORD nFLength = fFile.GetLength();
	
	fFile.Close();

	CHAR		*szFileName;	
	CharConvert cvt;
	INT			nLen;
	
	cvt.UnicodeToMultiByte(sFilePath.GetBuffer(0), &szFileName, &nLen );
	
	/// bmp 생성
	m_Decoder.MakeBmpFromFile (szFileName, m_pBmp);
	
	if( m_nMode == CUC_MANAGER_MODE_PREVIEW   )
	{		
		m_pBmp->StretchDraw( pDC->m_hDC , rLocation.left , rLocation.top , rLocation.Width() , rLocation.Height() );
	}
	else
	{
		/// 실제 사이즈 가져오기
		CSize Size = m_pBmp->GetSize();
		
		HGLOBAL m_hDIB;
		m_hDIB = ::GlobalAlloc(GMEM_FIXED, nFLength );
		/// 실제 뿌려주는 DC에 잘라서 뿌려주기		
		BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
		
		CopyBitMapInfo( m_pBmp->GetBMI() , &(bmInfo.bmiHeader) );
		
		for( INT i=0;i<256;i++)
		{
			bmInfo.bmiColors[i].rgbRed=(BYTE)i;
			bmInfo.bmiColors[i].rgbGreen=(BYTE)i;
			bmInfo.bmiColors[i].rgbBlue=(BYTE)i;
		}
		
		
		::StretchDIBits( pDC->m_hDC , rLocation.left , rLocation.top  , rLocation.Width() , rLocation.Height(), 
			0 , 0 , Size.cx, Size.cy, m_pBmp->GetBits() , (LPBITMAPINFO)m_hDIB  , DIB_RGB_COLORS , SRCCOPY );
		
		::GlobalFree(m_hDIB);
		
		
		
	}
	
	
	
	return 0;	
}