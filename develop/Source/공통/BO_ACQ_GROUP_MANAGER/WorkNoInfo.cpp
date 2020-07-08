// WorkNoInfo.cpp: implementation of the CWorkNoInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkNoInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkNoInfo::CWorkNoInfo()
{
	m_nLastSerialNo = -1;
	m_strWorkNo = _T("");
}

CWorkNoInfo::~CWorkNoInfo()
{

}
