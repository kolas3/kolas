// ESL_CodeItem.cpp: implementation of the CESL_CodeItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_CodeItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_CodeItem::CESL_CodeItem()
{
	CODE = _T("");
	DESCRIPTION = _T("");
	ACTIVE = 'Y';
}

CESL_CodeItem::~CESL_CodeItem()
{

}
