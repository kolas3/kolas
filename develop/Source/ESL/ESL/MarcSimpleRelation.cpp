// MarcSimpleRelation.cpp: implementation of the CMarcSimpleRelation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcSimpleRelation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcSimpleRelation::CMarcSimpleRelation()
{
	m_strGroupName.Empty();
}

CMarcSimpleRelation::~CMarcSimpleRelation()
{
	m_mapTag.RemoveAll();
	m_mapRepeatTag.RemoveAll();
	m_mapSubfiled.RemoveAll();
	m_mapIndicator.RemoveAll();
	m_mapDefaultIndicator.RemoveAll();
	m_mapRepeatSubfield.RemoveAll();
	m_mapPunctuationMark.RemoveAll();
}
