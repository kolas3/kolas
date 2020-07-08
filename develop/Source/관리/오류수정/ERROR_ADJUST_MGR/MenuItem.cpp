// MenuItem.cpp: implementation of the CMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuItem::CMenuItem()
{
	m_nID = 0;
	m_nIndex = 0;
}

CMenuItem::~CMenuItem()
{

}
