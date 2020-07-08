// CodeChangeCondition.cpp: implementation of the CCodeChangeCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CodeChangeCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCodeChangeCondition::CCodeChangeCondition()
{
	m_bClassNoType = FALSE;
	m_bControlCode = FALSE;
	m_bManageCode = FALSE;
	m_bMatCode = FALSE;
	m_bMediaCode = FALSE;
	m_bRegCode = FALSE;
	m_bShelfCode = FALSE;
	m_bSubRegCode = FALSE;
	m_bSubjectCode = FALSE;
	m_bUseLimitCode = FALSE;
	m_bUseObjCode = FALSE;
	m_bWorkCode = FALSE;
	m_bUserDefineCode1 = FALSE;
	m_bUserDefineCode2 = FALSE;
}

CCodeChangeCondition::~CCodeChangeCondition()
{

}
