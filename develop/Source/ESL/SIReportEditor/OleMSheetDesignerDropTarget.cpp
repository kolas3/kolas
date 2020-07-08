// OleMSheetDesignerDropTarget.cpp: implementation of the COleMSheetDesignerDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sireporteditor.h"
#include "OleMSheetDesignerDropTarget.h"

#include "MSheetDesigner.h"
#include "MListCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleMSheetDesignerDropTarget::COleMSheetDesignerDropTarget()
{
	m_strObjectType.Empty();

}

COleMSheetDesignerDropTarget::~COleMSheetDesignerDropTarget()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// COleMSheetDesignerDropTarget message handlers

DROPEFFECT COleMSheetDesignerDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	m_strObjectType.Empty();
   	CMSheetDesigner* pMSheetDesigner = (CMSheetDesigner*) pWnd;

	HANDLE hData = pDataObject->GetGlobalData(pMSheetDesigner->GetOleFormat());
	if (hData)
	{
		DRAG_PARAM *pData = (DRAG_PARAM*)::GlobalLock(hData);
		m_strObjectType = pData->szItemName;
		::GlobalUnlock(hData);
	}
	else
		return DROPEFFECT_NONE;
 
	// Check if the Control key was pressed
    if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY; // Copy source text
    else
        return DROPEFFECT_MOVE; // Move source text   
}

VOID COleMSheetDesignerDropTarget::OnDragLeave(CWnd* pWnd)
{
EFS_BEGIN

   	CMSheetDesigner* pMSheetDesigner = (CMSheetDesigner*) pWnd;
	
	//pMSheetDesigner->DrawDragImage(m_point, TRUE);
	
		
    COleDropTarget:: OnDragLeave(pWnd);

EFS_END
}

DROPEFFECT COleMSheetDesignerDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{                              

	CMSheetDesigner* pMSheetDesigner = (CMSheetDesigner*) pWnd;

	// If local Drag & Drop not allowed and drop point is inside 
	// of the source list ctrl, don't allow a drop.
	
	//pWnd->ClientToScreen(&point);
	/*pMSheetDesigner->DrawDragImage(point);
	

	m_point = point;
	*/

	if (m_strObjectType.IsEmpty()) return DROPEFFECT_NONE;

	
   if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;    
}

BOOL COleMSheetDesignerDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
EFS_BEGIN
	CMSheetDesigner* pMSheetDesigner = (CMSheetDesigner*) pWnd;

	INT nObjType = -1;
	if (!m_strObjectType.IsEmpty())
	{
		if (m_strObjectType == _T("테이블"))
		{
			nObjType = OBJECT_TABLE;
		}
		else if (m_strObjectType == _T("텍스트상자"))
		{
			nObjType = OBJECT_TEXT_BOX;
		}
		else if (m_strObjectType == _T("선"))
		{
			nObjType = OBJECT_LINE;
		}
		else if (m_strObjectType == _T("바코드"))
		{
			nObjType = OBJECT_BARCODE;
		}
		else if (m_strObjectType == _T("비트맵"))
		{
			nObjType = OBJECT_BITMAP;
		}
	}
	
	pMSheetDesigner->AddObject(nObjType, point);
	pMSheetDesigner->Invalidate();
	pMSheetDesigner->UpdateWindow();
	return TRUE;

EFS_END
return FALSE;

}