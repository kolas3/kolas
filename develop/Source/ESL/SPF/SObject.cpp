// SObject.cpp: implementation of the CSObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF.h"
#include "SObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSObject::CSObject()
{
	m_Selected = 0;
    OtherGroupFlag = 0;
	
	// KOL.UDF.022 시큐어코딩 보완
	m_ObjectType = _T("");  
	m_ALIAS = _T("");
	m_PK = _T("");
	m_DESCRIP = _T("");
    m_ObjectRect = NULL;
	m_Developer = _T("");
	m_DvmAlias = _T("");
    m_PARENT_GROUP_ALIAS = _T("");
	m_GROUP_ALIAS = _T("");
	m_GROUP_TYPE = _T("");
	m_pt1 = NULL;
	m_pt2 = NULL;
	m_pt3 = NULL;
	m_PATH_TYPE = _T("");
	m_SOURCE_FILE_NAME = _T("");
	m_SOURCE_FUNCTION_NAME = _T("");
	m_START_STATE_ALIAS = _T("");
    m_END_STATE_ALIAS = _T("");
	m_StartPos = NULL;
	pDM = NULL;
	m_Activated = _T("");
	m_TASK = _T("");
	m_ObjectList.RemoveAll();
	IDX = 0;
	pOldPen = NULL;
    pOldBrush = NULL;
	OldROP = 0;
	m_List = NULL;
}

CSObject::~CSObject()
{

}

INT CSObject::DrawObject(CDC *pDC)
{

	return 0;
}

INT CSObject::UnDrawObject(CDC *pDC)
{

	return 0;
}

INT CSObject::MoveObject(CPoint Pos )
{

	return 0;
}

INT CSObject::ReSizeObject(CPoint Pos , INT PickIndex)
{

	return 0;
}

INT CSObject::IsSeleted(CPoint Cur_Pos)
{

	return 0;
}
INT CSObject::MoveObject2(CPoint Pos,INT PickIndex)
{

	return 0;
}
INT CSObject::IsRelated(CPoint Cur_Pos)
{

	return 0;
}

INT CSObject::IsPickPoint(CPoint Cur_Pos)
{
	return 0;
}
INT CSObject::SetActivatedObject(CDC *pDC)
{
    
    m_pen.CreatePen( PS_SOLID , 1 , RGB(0,0,255));
	pOldPen = pDC->SelectObject(&m_pen);
      
	return 0;
}

INT CSObject::SetDrawObject(CDC *pDC)
{
    m_pen.CreatePen( PS_SOLID , 1 , RGB(0,0,0));
	pOldPen = pDC->SelectObject(&m_pen);
//	OldROP = pDC->SetROP2(R2_NOT);
	//TRACE(_T("m_pen.CreatePen(); \n"));

	return 0;
}

INT CSObject::ReturnDrawObject(CDC *pDC)
{

	pDC->SelectObject(&pOldPen);
	m_pen.DeleteObject();
	//TRACE(_T("m_pen.DeleteObject(); \n"));
	return 0;
}

INT CSObject::SetUnDrawObject(CDC *pDC)
{

    m_pen.CreatePen( PS_SOLID , 2 , RGB(255,255,255));
	pOldPen = pDC->SelectObject(&m_pen);
//	pDC->SetROP2(OldROP);

	return 0;
}

INT CSObject::SetAllEnv(CPoint Cur_Pos)
{

	return 0;
}

INT CSObject::CheckAllEnv(CPoint point)
{
	return 0;
}

INT CSObject::SetDrawRelatedObject(CDC *pDC)
{
    m_pen.CreatePen( PS_SOLID , 1 , RGB(0,0,255));
	pOldPen = pDC->SelectObject(&m_pen);

	return 0;
}

INT CSObject::SaveToDM()
{
	return 0;
}

INT CSObject::InsertToDM()
{
	return 0;
}
INT CSObject::DeleteToDM()
{
	return 0;
}
INT CSObject::MakeObjList()
{
	return 0;
}

INT CSObject::DrawTextWithFont(CDC *dc, CString str, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize = logfont.lfHeight;
	CFont font;

	logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);


	CRect tmpRect;
	tmpRect = rect;
	if (tmpRect.top > tmpRect.bottom) {
		tmpRect.top = rect.bottom;
		tmpRect.bottom = rect.top;
	}
	if (tmpRect.left > tmpRect.right) {
		tmpRect.left = rect.right;
		tmpRect.right = rect.left;
	}
		
	height = dc->DrawText(str, tmpRect, nFormat);
	dc->SelectObject(pOldFont);
	return height;
}