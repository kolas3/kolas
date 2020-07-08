// MarcInstruction.cpp: implementation of the CMarcInstruction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcInstruction.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcInstruction::CMarcInstruction()
{
	ZeroMemory(m_pszGroupName, sizeof(TCHAR) * 40);
	ZeroMemory(m_pszOperator, sizeof(TCHAR) * 20);
	ZeroMemory(m_pszBasicParam, sizeof(TCHAR) * 50);
	ZeroMemory(m_pszParam, sizeof(TCHAR) * 50);
	ZeroMemory(m_pszDesc, sizeof(TCHAR) * 100);
}

CMarcInstruction::~CMarcInstruction()
{

}

VOID CMarcInstruction::SetGroupName(TCHAR *pSource, INT nSize)
{
	EFS_BEGIN

	wmemcpy(m_pszGroupName, pSource, nSize);
	m_pszGroupName[nSize] = '\0';
	
	EFS_END
}

VOID CMarcInstruction::SetOperator(TCHAR *pSource, INT nSize)
{
	EFS_BEGIN

	wmemcpy(m_pszOperator, pSource, nSize);
	m_pszOperator[nSize] = '\0';

	EFS_END
}

VOID CMarcInstruction::SetBasicParam(TCHAR *pSource, INT nSize)
{
	EFS_BEGIN

	wmemcpy(m_pszBasicParam, pSource, nSize);
	m_pszBasicParam[nSize] = '\0';

	EFS_END
}

VOID CMarcInstruction::SetParam(TCHAR *pSource, INT nSize)
{
	EFS_BEGIN

	wmemcpy(m_pszParam, pSource, nSize);
	m_pszParam[nSize] = '\0';

	EFS_END
}

VOID CMarcInstruction::SetDesc(TCHAR *pSource, INT nSize)
{
	EFS_BEGIN

	wmemcpy(m_pszDesc, pSource, nSize);
	m_pszDesc[nSize] = '\0';

	EFS_END
}
