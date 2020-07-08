// SIReportObject_Text.cpp: implementation of the CSIReportObject_Text class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject_Text, CObject)
CSIReportObject_Text::CSIReportObject_Text()
:CObject()
{
	ZeroMemory(&font,sizeof(font));
	font.lfCharSet = DEFAULT_CHARSET;
	_tcscpy(font.lfFaceName,_T("±¼¸²Ã¼"));
	//font.lfHeight = -MulDiv(10, GetDeviceCaps(GetWindowDC(GetDesktopWindow()), LOGPIXELSY), 72);
	font.lfHeight = -13;
	color = RGB(0, 0, 0);
	size = 10;
	text = _T("");
	style = 0;
}

CSIReportObject_Text& CSIReportObject_Text::operator =(CSIReportObject_Text& rhs)
{
	if (this == &rhs)
		return *this;
	
	color = rhs.color;
	font = rhs.font;
	size = rhs.size;
	style = rhs.style;
	text = rhs.text;

	return *this;
}

CSIReportObject_Text::~CSIReportObject_Text()
{

}
