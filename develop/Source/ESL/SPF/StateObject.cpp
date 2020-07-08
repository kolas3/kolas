// StateObject.cpp: implementation of the CStateObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF.h"
#include "StateObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStateObject::CStateObject()
{
	// KOL.UDF.022 시큐어코딩 보완
	pDC = NULL;
	STATE_NAME = _T("");
	PK = _T("");
	RECT = NULL;
	DESCRIP = _T("");
	DEFAULT_PATH_ALIAS = _T("");
   
}

CStateObject::~CStateObject()
{
    

}


VOID CStateObject::DrawItself(CDC *pDC,CPoint point)
{
   
   CRect rect;
   rect.top = point.y+25;
   rect.bottom = point.y-25;
   rect.left = point.x-25;
   rect.right = point.x+25;

   
   RECT = rect; 
    
   pDC->Ellipse(&rect);
   
   //pDC->TextOut(rect.left+65,rect.top+40,sNAME);
}


VOID CStateObject::DrawItself()
{

    pDC->Ellipse(&RECT);

}






