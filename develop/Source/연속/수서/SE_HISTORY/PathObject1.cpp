// PathObject1.cpp: implementation of the CPathObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PathObject1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathObject::CPathObject()
{

}

CPathObject::~CPathObject()
{

}

INT CPathObject::DrawPathObject(CDC* pDC, POINT pt1, POINT pt2, POINT pt3)
{
      
    m_ControlPoints.Add(pt1);
    m_ControlPoints.Add(pt2);
	m_ControlPoints.Add(pt3);
    	
	Spline spline(m_ControlPoints.GetData(), m_ControlPoints.GetSize());
	spline.Generate();
	
	m_CurvePoints.SetSize(spline.GetCurveCount());
	INT PointCount = 0;
	spline.GetCurve(m_CurvePoints.GetData(), PointCount);
			
	UpdateObject(pDC);
		
    m_ControlPoints.RemoveAll();
    m_ControlPoints.FreeExtra();
	
	m_CurvePoints.RemoveAll();
    m_CurvePoints.FreeExtra();
   
	return 0;
}


VOID CPathObject::UpdateObject(CDC* pDC)
{

	if(m_CurvePoints.GetSize()>1)
	{	
	   pDC->Polyline(m_CurvePoints.GetData(), m_CurvePoints.GetSize());
	}
    
}