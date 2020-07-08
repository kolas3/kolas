// ESL_GridColumnInfo.cpp: implementation of the CESL_GridColumnInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_GridColumnInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_GridColumnInfo::CESL_GridColumnInfo()
{
	IsDesc = FALSE;	
	nColumnPos = -1;
	bPasswordStyle = FALSE;
}

CESL_GridColumnInfo::~CESL_GridColumnInfo()
{

}
