// EFSDMException.cpp: implementation of the CEFSDMException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EFSDMException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEFSDMException::CEFSDMException(CESL_DataMgr *pDM /*= NULL*/)
{
	m_pDM = pDM;
}

CEFSDMException::~CEFSDMException()
{

}
