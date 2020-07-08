// Tns.cpp: implementation of the CTns class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tns.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTns::CTns()
{
	m_strLIB_PK = _T("");	 
	m_strKEY_VALUE = _T("");	
	m_strHOST = _T("0.0.0.0");
	m_strPORT = _T("1521");
	m_strSERVICE_NAME = _T("K2UP");
	m_strFIREWALL_YN = _T("N");
}

CTns::~CTns()
{

}
