// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Line.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{

}

CLine::~CLine()
{

}


INT CLine::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex , INT nMode  )
{
	const INT nCnt = 2;
	CString sAlias[nCnt] = 
	{
		_T("라인굵기") , _T("라인종류")
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , nIndex );
	
	nLineHeight  = _ttoi(sResult[0]);
	nLineType  = _ttoi(sResult[1]);
	
	CControlObject::InsertValue( pCardDM , nIndex , nMode );
	
	return 0;
}

INT CLine::DrawDC( CDC* pDC )
{

	return 0;	
}