// OleMListDropTarget.cpp: implementation of the COleMListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sireporteditor.h"
#include "OleMListDropTarget.h"

#include "MListCtrl.h"

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

/////////////////////////////////////////////////////////////////////////////
// COleMListDropTarget message handlers

//-----------------------------------------------------------------------------------
// DROPEFFECT COleListCtrlDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
//           pDataObject, DWORD dwKeyState, CPoint point )
// 
// Called by OLE dll's when drag cursor FIRST enters a window that is REGISTERED
//  with the OLE dll's
//
//Parameters
//----------
//
// pWnd        -- Points to the window the cursor is entering.
// pDataObject -- Points to the data object containing the data
//                that can be dropped.
// dwKeyState  -- Contains the state of the modifier keys.
//                This is a combination of any number of the following:
//                MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
//                MK_MBUTTON, and MK_RBUTTON.
//
// point       -- Contains the current location of the cursor in
//                client coordinates.
//
// Return Value : The effect that would result if a drop were 
//-------------   attempted at the location specified by point.
//                It can be one or more of the following: 
//
//     DROPEFFECT_NONE   -A drop would not be allowed.
//     DROPEFFECT_COPY   -A copy operation would be performed.
//     DROPEFFECT_MOVE   -A move operation would be performed.
//     DROPEFFECT_LINK   -A link from the dropped data to the 
//                        original data would be established.
//     DROPEFFECT_SCROLL -A drag scroll operation is about to occur 
//                        or is occurring in the target. 
//-----------------------------------------------------------------------------------
DROPEFFECT COleMListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
   	CMListCtrl* pMListCtrl = (CMListCtrl*) pWnd;
 
	// If local Drag&Drop not allowed and point is inside of the
	// source list ctrl, don't allow a drop
	
	if (m_pImageList)
	{
		m_pImageList->EndDrag();
		m_pImageList->DeleteImageList();
		delete m_pImageList;
		m_pImageList = NULL;
	}

	//pWnd->ClientToScreen(&point);
	pMListCtrl->CreateDragImage(0, 0);
	if (pMListCtrl->m_nSelectIndex == -1) return DROPEFFECT_NONE;
	
	m_pImageList = new CImageList;
	m_pImageList->Create(48, 48, ILC_COLOR24, 0, 4);

	//m_pImageList->Create(pMListCtrl->CreateDragImage(pMListCtrl->m_nSelectIndex, &CPoint(16, 16)));

	CBitmap bmp;
	CString filename;
	filename.Format(_T("..\\bmp\\SIReportEditor\\icon48_%02d.bmp"), pMListCtrl->m_nSelectIndex+1);
	bmp.Attach(::LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
	
	m_pImageList->Add(&bmp, RGB(0, 0, 0));
	bmp.Detach();


	m_pImageList->BeginDrag(0, CPoint(16, 16));
	m_pImageList->DragEnter(pWnd, point);
	
	// Check if the Control key was pressed
    if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY; // Copy source text
    else
        return DROPEFFECT_MOVE; // Move source text   
}

//---------------------------------------------------------------------------------
// VOID COleListCtrlDropTarget::OnDragLeave(CWnd* pWnd)
//
// Called by OLE dll's when drag cursor leaves a window that is REGISTERed with 
// the OLE dll's.
//
// pWnd points to the window the cursor is leaving.
//---------------------------------------------------------------------------------
VOID COleMListDropTarget::OnDragLeave(CWnd* pWnd)
{
   	CMListCtrl* pMListCtrl = (CMListCtrl*) pWnd;
	
	m_pImageList->DragLeave(pWnd);
	

    COleDropTarget:: OnDragLeave(pWnd);
}

//----------------------------------------------------------------------------------
// DROPEFFECT COleListCtrlDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
//           pDataObject, DWORD dwKeyState, CPoint point )
//
// Called by OLE dll's when cursor is dragged over a window that is REGISTERed with
// the OLE dll's.
//
//Parameters
//----------
// pWnd        -- points to the window that the cursor is over.
// pDataObject -- points to the data object that contains the data
//                to be dropped.
// dwKeyState  -- Contains the state of the modifier keys.
//                This is a combination of any number of the
//                following: 
//                MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
//                MK_MBUTTON, and MK_RBUTTON.
// point       -- Contains the current location of the cursor in 
//                client coordinates.
// 
//Return Value:
//------------   The effect that would result if a drop were attempted
//               at the location specified by point. 
//               It can be one or more of the following: 
//
//DROPEFFECT_NONE   A drop would not be allowed.
//
//DROPEFFECT_COPY   A copy operation would be performed.
//
//DROPEFFECT_MOVE   A move operation would be performed.
//
//DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
//
//DROPEFFECT_SCROLL   Indicates that a drag scroll operation is about to occur or is occurring in the target. 
//
// Note: OnDragOver is like WM_MOUSEMOVE message, in that CALLS TO IT
// ARE GENERATED REPEATEDLY as the pointer moves around the window
//----------------------------------------------------------------------------------
DROPEFFECT COleMListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{                              

	CMListCtrl* pMListCtrl = (CMListCtrl*) pWnd;

	// If local Drag & Drop not allowed and drop point is inside 
	// of the source list ctrl, don't allow a drop.
	
	//pWnd->ClientToScreen(&point);
	m_pImageList->DragMove(point);
	
   if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;    
}

//---------------------------------------------------------------------------------- 
// BOOL COleListCtrlDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
//                 DROPEFFECT dropEffect, CPoint point )
//
// Called by OLE dll's when item is dropped in a window that is REGISTERed with the
// OLE dll's.
//
//Parameters
//----------
// pWnd        -- Points to the window the cursor is currently over.
//
// pDataObject -- points to the data object that contains the data
//                to be dropped.
//
// dropEffect  -- The effect that the user chose for the drop operation. It can be one or more of the following: 
//
//DROPEFFECT_COPY   A copy operation would be performed.
//
//DROPEFFECT_MOVE   A move operation would be performed.
//
//DROPEFFECT_LINK   A link from the dropped data to the original data would be established. 
//
// point       -- Contains the location of the cursor, in pixels,
//                relative to the screen.
//---------------------------------------------------------------------------------- 
BOOL COleMListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{
	CMListCtrl* pMListCtrl = (CMListCtrl*) pWnd;
	pMListCtrl->Invalidate();
	
	if (m_pImageList)
	{
		m_pImageList->DragLeave(pWnd);
		m_pImageList->EndDrag();

		delete m_pImageList;
		m_pImageList = NULL;
	}


    return TRUE;
}