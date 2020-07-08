// ToolbarItem.cpp: implementation of the CToolbarItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBarItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolBarItem::CToolBarItem()
{
	m_nWidth = 0;
	m_nID = 0;
	m_nIndex = 0;
	m_bIsVisible = TRUE;
}

CToolBarItem::~CToolBarItem()
{

}
