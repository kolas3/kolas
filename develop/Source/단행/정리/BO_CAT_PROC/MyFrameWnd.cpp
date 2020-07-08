// MyFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFrameWnd

IMPLEMENT_DYNCREATE(CMyFrameWnd, CFrameWnd)

CMyFrameWnd::CMyFrameWnd()
{
}

CMyFrameWnd::~CMyFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMyFrameWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFrameWnd message handlers

INT CMyFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// Docking dialog bar --> cooldialogbar
	if (!m_wndTestDialogBar.Create(this, &m_dlgTest, CString(_T("TEST")), IDD_TEST))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }
/*
	if (!m_wndTestDialogBar2.Create(this, &m_dlgTest2, CString(_T("TEST")), IDD_TEST))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }
	
	if (!m_wndTestDialogBar3.Create(this, &m_dlgTest3, CString(_T("TEST")), IDD_TEST))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }
	if (!m_wndTestDialogBar4.Create(this, &m_dlgTest4, CString(_T("TEST")), IDD_TEST))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }
	
	if (!m_wndTestDialogBar5.Create(this, &m_dlgTest5, CString(_T("TEST")), IDD_TEST))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }
	if (!m_wndTestDialogBar6.Create(this, &m_dlgTest6, CString(_T("TEST")), IDD_TEST))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }
*/
	if (!m_wndMarcEditDialogBar.Create(this, &m_dlgMarcEdit, CString(_T("Marc Edit")), IDD_MARCEDIT))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }

	EnableDocking(CBRS_ALIGN_ANY);

	m_wndTestDialogBar.SetBarStyle(m_wndTestDialogBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTestDialogBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndTestDialogBar, AFX_IDW_DOCKBAR_TOP);
	

	m_wndMarcEditDialogBar.SetBarStyle(m_wndMarcEditDialogBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndMarcEditDialogBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndMarcEditDialogBar, AFX_IDW_DOCKBAR_TOP);
/*
	m_wndTestDialogBar2.SetBarStyle(m_wndTestDialogBar2.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTestDialogBar2.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndTestDialogBar2, AFX_IDW_DOCKBAR_TOP);

	m_wndTestDialogBar3.SetBarStyle(m_wndTestDialogBar3.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTestDialogBar3.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndTestDialogBar3, AFX_IDW_DOCKBAR_TOP);

	m_wndTestDialogBar4.SetBarStyle(m_wndTestDialogBar4.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTestDialogBar4.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndTestDialogBar4, AFX_IDW_DOCKBAR_TOP);

	m_wndTestDialogBar5.SetBarStyle(m_wndTestDialogBar5.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTestDialogBar5.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndTestDialogBar5, AFX_IDW_DOCKBAR_TOP);

	m_wndTestDialogBar6.SetBarStyle(m_wndTestDialogBar6.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTestDialogBar6.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndTestDialogBar6, AFX_IDW_DOCKBAR_TOP);
*/
	return 0;
}
