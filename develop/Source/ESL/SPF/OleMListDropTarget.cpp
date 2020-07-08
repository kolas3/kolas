// OleMListDropTarget.cpp: implementation of the COleMListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleMListDropTarget.h"
#include "ToolDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleMListDropTarget::COleMListDropTarget()
{
	m_pImageList = NULL;
}

COleMListDropTarget::~COleMListDropTarget()
{
	if (m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}


DROPEFFECT COleMListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
   	CToolDlg* pToolDlg = (CToolDlg*) pWnd;
 
	
	if (m_pImageList)
	{
		m_pImageList->EndDrag();
		m_pImageList->DeleteImageList();
		delete m_pImageList;
		m_pImageList = NULL;
	}


	pToolDlg->m_List.CreateDragImage(0,0);
	if (pToolDlg->m_nSelectIndex == -1) return DROPEFFECT_NONE;
	m_pImageList = pToolDlg->m_List.CreateDragImage(pToolDlg->m_nSelectIndex, &CPoint(16, 16));
		
	m_pImageList->BeginDrag(0, CPoint(16, 16));
	m_pImageList->DragEnter(pWnd, point);
	
	   
        return DROPEFFECT_MOVE; // Move source text   
}


VOID COleMListDropTarget::OnDragLeave(CWnd* pWnd)
{
   	CToolDlg* pToolDlg = (CToolDlg*) pWnd;
	
	m_pImageList->DragLeave(pWnd);
	

    COleDropTarget:: OnDragLeave(pWnd);
}


DROPEFFECT COleMListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{                              

	CToolDlg* pToolDlg = (CToolDlg*) pWnd;

	// If local Drag & Drop not allowed and drop point is inside 
	// of the source list ctrl, don't allow a drop.
	

	m_pImageList->DragMove(point);
	
   
    return DROPEFFECT_MOVE;    
}


BOOL COleMListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{
	 CToolDlg* pToolDlg = (CToolDlg*) pWnd;
  
	 pToolDlg->m_List.Invalidate();
	
	if (m_pImageList)
	{
		m_pImageList->DragLeave(pWnd);
		m_pImageList->EndDrag();

		delete m_pImageList;
		m_pImageList = NULL;
	}

    
    return TRUE;
}