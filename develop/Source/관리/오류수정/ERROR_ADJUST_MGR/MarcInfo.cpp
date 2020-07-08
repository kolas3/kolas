// MarcInfo.cpp: implementation of the CMarcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcInfo::CMarcInfo(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
}

CMarcInfo::~CMarcInfo()
{

}
