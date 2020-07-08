// GroupInfo.cpp: implementation of the CGroupInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupInfo::CGroupInfo()
{
	m_strAcqYear = _T("");
	m_strGroupNo = _T("");
	m_strWorkNo = _T("");
	m_nLastSerialNo = -1;
}

CGroupInfo::~CGroupInfo()
{

}
