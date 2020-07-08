// ESL_GridData.cpp: implementation of the CESL_GridData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_GridData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_GridData::CESL_GridData()
{
	pMgr = NULL;
	pDM = NULL;
	pGrid = NULL;
	m_strGridAlias = _T("");
	m_fpEventHandler = NULL;
	m_pRealRowCount = NULL;
	m_bProtectSelectColumn = FALSE;
}

CESL_GridData::~CESL_GridData()
{

}

INT CESL_GridData::ParseMgrAndColumn(CString link_data, CString &MgrAlias, CString &ColumnAlias)
{
EFS_BEGIN

	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;
		MgrAlias = link_data.Mid(1, mgrpos-1);
		ColumnAlias = link_data.Mid(mgrpos+1);
	}
	return 0;

EFS_END
return -1;

}

INT CESL_GridData::GridDisplayEventProc(INT nRow, INT nCol, CString &strDMFieldAlias, CString &str )
{
EFS_BEGIN

	if (m_pOwnerWnd == NULL) return -1;
	if (m_fpEventHandler == NULL) return -2;
	( m_pOwnerWnd->*m_fpEventHandler )( m_strGridAlias, nRow, nCol, strDMFieldAlias, str);
	return 0;

EFS_END
return -1;

}