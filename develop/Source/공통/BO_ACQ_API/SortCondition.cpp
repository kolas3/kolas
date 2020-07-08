// SortCondition.cpp: implementation of the CSortCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortCondition::CSortCondition()
{
	m_nFirstType = -1;
	m_nSecondType = -1;
	m_nThirdType = -1;
	m_nFourthType = -1;
}

CSortCondition::~CSortCondition()
{

}
