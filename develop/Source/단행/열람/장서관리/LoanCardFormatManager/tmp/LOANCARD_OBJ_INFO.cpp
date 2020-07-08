// LOANCARD_OBJ_INFO.cpp: implementation of the LOANCARD_OBJ_INFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LOANCARD_OBJ_INFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LOANCARD_OBJ_INFO::LOANCARD_OBJ_INFO()
{
	// KOL.UDF.022
		REC_KEY = _T("");
		ALIAS = _T("");
		BOX_SIZE = _T("");
		TYPE = _T("");
		USE_YN = FALSE;
		FIX = FALSE;
		CONTENT = _T("");
		FILL_COLOR = _T("");
		STRING_LEN = 0;
		STRING_MAX = 0;
		HORZ_ALIGN = _T("");	// 정렬( 왼쪽:ALIGN_LEFT, 중앙:ALIGN_CENTER, 오른쪽:ALIGN_RIGHT )
		VERT_ALIGN = _T("");	// 정렬( 위:ALIGN_TOP, 가운데:ALIGN_VCENTER, 아래:ALIGN_BOTTOM )
		HORZ_LINE = FALSE;
		VERT_LINE = FALSE;
		FONT = _T("");
		FONT_SIZE = 0;
}

LOANCARD_OBJ_INFO::~LOANCARD_OBJ_INFO()
{

}
