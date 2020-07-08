// SIReportObject_Bitmap.cpp: implementation of the CSIReportObject_Bitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Bitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject_Bitmap, CSIReportObject)
CSIReportObject_Bitmap::CSIReportObject_Bitmap()
{
	SetObjType(OBJECT_BITMAP);
	m_strName = _T("BITMAP");
	m_pBitmap = NULL;
	m_bOrigin = FALSE;
}

VOID CSIReportObject_Bitmap::Clone(CSIReportObject *pObject)
{
EFS_BEGIN

	CSIReportObject::Clone(pObject);

	((CSIReportObject_Bitmap*)pObject)->file_name = file_name;

EFS_END
}

CSIReportObject_Bitmap::~CSIReportObject_Bitmap()
{
	FreeBitmap();
}

INT CSIReportObject_Bitmap::FreeBitmap(VOID)
{
EFS_BEGIN

	if (m_pBitmap) {
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	m_bOrigin = TRUE;
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Bitmap::LoadBitmap(VOID) 
{
EFS_BEGIN

	if (m_pBitmap) return 1;		// 이미 로드된경우는 다시 로드하지 않는다.
	m_pBitmap = new CLoadBmp;
	if (!m_pBitmap) return -1;

	if (file_name.GetLength() == 0) return -2;
	FILE *fd = _tfopen(file_name, _T("rb"));
	if (fd == NULL) {
		//return -3;		// 파일이 존재하지 않음
		m_pBitmap->LoadBMP(_T("..\\Bmp\\SiReportEditor\\ImageNotFound.bmp"));
	} else {
		fclose(fd);
		m_pBitmap->LoadBMP(file_name);
	}
	
	if (m_bOrigin)
	{
		CRect rect = GetRect();
		CRect rectImage = m_pBitmap->GetImageSize();
		rect.right = rect.left + rectImage.Width();
		rect.bottom = rect.top + rectImage.Height();
		SetRect(rect);
	}
	return 0;						// 성공

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CMObject Draw
VOID CSIReportObject_Bitmap::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	CRect rect = GetRect();
	pDC->Rectangle(rect);
	LoadBitmap();
	
	if (m_pBitmap == NULL) return;
	m_pBitmap->DrawDIB2(pDC, rect.left, rect.top, rect.Width(), rect.Height());

EFS_END
}
