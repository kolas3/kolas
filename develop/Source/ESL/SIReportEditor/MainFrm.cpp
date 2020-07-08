// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SIReportEditor.h"

#include "MainFrm.h"

#include "SIReportEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAREDIT, OnViewToolbaredit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAREDIT, OnUpdateViewToolbaredit)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_TOOLBARTABLE, OnViewToolbartable)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARTABLE, OnUpdateViewToolbartable)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_RELOAD_BROKER, OnReloadBroker)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pDlgProperty = NULL;
	m_RegClass = FALSE;
}

CMainFrame::~CMainFrame()
{
	if (m_pDlgProperty != NULL)
	{
		delete m_pDlgProperty;
		m_pDlgProperty = NULL;
	}
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
EFS_BEGIN

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBarEdit.CreateEx(this) ||
		!m_wndToolBarEdit.LoadToolBar(IDR_TOOLBAREDIT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBarTable.CreateEx(this) ||
		!m_wndToolBarTable.LoadToolBar(IDR_TOOLBAR_TABLE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	/*if (!m_wndDlgBar.Create(this, IDD_TOOLBAR, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
	*/

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndToolBarEdit)) //||
		//!m_wndReBar.AddBar(&m_wndDlgBar)) 
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Docking dialog bar --> cooldialogbar
	if (!m_wndCoolDialogBar.Create(this, &m_dlgTab, CString(_T("TAB VIEW")), IDD_TAB_DIALOG))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }

	EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |CBRS_TOOLTIPS | CBRS_FLYBY ); // TODO: Remove this if you don't want tool tips
	m_wndToolBarEdit.SetBarStyle(m_wndToolBarEdit.GetBarStyle() |CBRS_TOOLTIPS | CBRS_FLYBY ); // TODO: Remove this if you don't want tool tips
	m_wndToolBarTable.SetBarStyle(m_wndToolBarTable.GetBarStyle() |CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndCoolDialogBar.SetBarStyle(m_wndCoolDialogBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBarTable.EnableDocking(CBRS_FLOAT_MULTI | CBRS_ALIGN_ANY);
	FloatControlBar(&m_wndToolBarTable, CPoint(700, 100), CBRS_ALIGN_BOTTOM);
	//DockControlBar(&m_wndToolBarTable, AFX_IDW_DOCKBAR_BOTTOM);
		
	m_wndCoolDialogBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndCoolDialogBar);

	SetImageList();

	ShowWindow(SW_SHOWMAXIMIZED);

	return 0;

EFS_END
return -1;

}

VOID CMainFrame::SetImageList()
{
EFS_BEGIN

	// SetImageList
	m_imgListToolBarNoaml.Create(36, 36, ILC_COLOR24, 0, 4);
	m_imgListToolBarHot.Create(36, 36, ILC_COLOR24, 0, 4);
	m_imgListToolBarDisabled.Create(36, 36, ILC_COLOR24, 0, 4);
	
	m_imgListToolBarEditNoaml.Create(36, 36, ILC_COLOR24, 0, 4);
	m_imgListToolBarEditHot.Create(36, 36, ILC_COLOR24, 0, 4);
	m_imgListToolBarEditDisabled.Create(36, 36, ILC_COLOR24, 0, 4);

	CBitmap bmpNormal;
	CBitmap bmpHot;
	CBitmap bmpDisabled;
	
	// Edit Tool Bar
	CString strFileName;
	for (INT i = 1; i <= 23; i++)
	{
		strFileName.Format(_T("..\\bmp\\SIReportEditor\\btn%02d_n.bmp"), i);
		if (!CheckFileExist(strFileName)) return;
		
		bmpNormal.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		strFileName.Format(_T("..\\bmp\\SIReportEditor\\btn%02d_f.bmp"), i);
		if (!CheckFileExist(strFileName)) return;

		bmpHot.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		strFileName.Format(_T("..\\bmp\\SIReportEditor\\btn%02d_d.bmp"), i);
		if (!CheckFileExist(strFileName)) return;

		bmpDisabled.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		ReplaceBackgroundColor(bmpNormal);
		ReplaceBackgroundColor(bmpHot);
		ReplaceBackgroundColor(bmpDisabled);

		if (i < 9)
		{
			m_imgListToolBarNoaml.Add(&bmpNormal, RGB(0, 0, 0));
			m_imgListToolBarHot.Add(&bmpHot, RGB(0, 0, 0));
			m_imgListToolBarDisabled.Add(&bmpDisabled, RGB(0, 0, 0));
		}
		else
		{
			m_imgListToolBarEditNoaml.Add(&bmpNormal, RGB(0, 0, 0));
			m_imgListToolBarEditHot.Add(&bmpHot, RGB(0, 0, 0));
			m_imgListToolBarEditDisabled.Add(&bmpDisabled, RGB(0, 0, 0));
		}
		
		bmpNormal.Detach();
		bmpHot.Detach();
		bmpDisabled.Detach();
	}

	CToolBarCtrl &rpToolBarCtrl = m_wndToolBar.GetToolBarCtrl();
	CToolBarCtrl &rpToolBarEditCtrl = m_wndToolBarEdit.GetToolBarCtrl();

	rpToolBarCtrl.SetImageList(&m_imgListToolBarNoaml);
	rpToolBarCtrl.SetHotImageList(&m_imgListToolBarHot);
	rpToolBarCtrl.SetDisabledImageList(&m_imgListToolBarDisabled);

	rpToolBarEditCtrl.SetImageList(&m_imgListToolBarEditNoaml);
	rpToolBarEditCtrl.SetHotImageList(&m_imgListToolBarEditHot);
	rpToolBarEditCtrl.SetDisabledImageList(&m_imgListToolBarEditDisabled);

EFS_END
}

BOOL CMainFrame::CheckFileExist(CString strFileName)
{
EFS_BEGIN

	CFile file;
	if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary))
	{
		return FALSE;
	}
	else 
		file.Close();

	return TRUE;

EFS_END
return FALSE;

}

VOID CMainFrame::ReplaceBackgroundColor(CBitmap& ioBM)
{
EFS_BEGIN

	 // figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;

	VERIFY (ioBM.GetBitmap (&bmInfo));
	
	 // add support for additional bit depths here if you choose
	if (bmInfo.bmBitsPixel != 24) return;
	VERIFY (bmInfo.bmBitsPixel == 24);
	VERIFY (bmInfo.bmWidthBytes == (bmInfo.bmWidth * 3));

	const UINT		numPixels (bmInfo.bmHeight * bmInfo.bmWidth);

	 // get a pointer to the pixels
    DIBSECTION  ds;
	VERIFY (ioBM.GetObject (sizeof (DIBSECTION), &ds) == sizeof (DIBSECTION));

	RGBTRIPLE*		pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	VERIFY (pixels != NULL);

	 // get the user's preferred button color from the _tsystem
	const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const RGBTRIPLE		userBackgroundColor = {
		GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};

	RGBTRIPLE	kBackgroundColor = {192, 192, 192};
	//RGBTRIPLE	kBackgroundColor = {200, 208, 212};
	 // search through the pixels, substituting the user's button
	 // color for any pixel that has the magic background color
	for (UINT i = 0; i < numPixels; ++i)
	{
		if (pixels [i].rgbtBlue == kBackgroundColor.rgbtBlue &&
			pixels [i].rgbtGreen == kBackgroundColor.rgbtGreen &&
			pixels [i].rgbtRed == kBackgroundColor.rgbtRed)
		{
			pixels [i] = userBackgroundColor;
		}
	}

EFS_END
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
EFS_BEGIN

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or style0s here by modifying
	//  the CREATESTRUCT cs

	if(!m_RegClass)
    {
        WNDCLASS wc;
        GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wc);
        wc.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
        wc.lpszClassName = _T("SIReportEditor");
        AfxRegisterClass(&wc);
        m_RegClass = TRUE;
    }
	cs.lpszClass = _T("SIReportEditor");

	return TRUE;

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
VOID CMainFrame::AssertValid() const
{
EFS_BEGIN

	CFrameWnd::AssertValid();

EFS_END
}

VOID CMainFrame::Dump(CDumpContext& dc) const
{
EFS_BEGIN

	CFrameWnd::Dump(dc);

EFS_END
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/*VOID CMainFrame::OnbZOOM2() 
{
EFS_BEGIN

	CView *pView = GetActiveView();
	pView->SendMessage(WM_COMMAND, IDC_bZOOM2, 0);

EFS_END
return NULL;

}
*/

VOID CMainFrame::OnViewToolbar() 
{
EFS_BEGIN

	//CWnd a;
	//a.IsWindowVisible
	if (m_wndToolBar.IsWindowVisible())
		ShowControlBar(&m_wndToolBar, FALSE, FALSE);
	else
		ShowControlBar(&m_wndToolBar, TRUE, FALSE);

EFS_END
}

VOID CMainFrame::OnViewToolbaredit() 
{
EFS_BEGIN

	if (m_wndToolBarEdit.IsWindowVisible())
		ShowControlBar(&m_wndToolBarEdit, FALSE, FALSE);
	else
		ShowControlBar(&m_wndToolBarEdit, TRUE, FALSE);	

EFS_END
}

VOID CMainFrame::OnUpdateViewToolbaredit(CCmdUI* pCmdUI) 
{
EFS_BEGIN

	if (m_wndToolBarEdit.IsWindowVisible())
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);

EFS_END
}


VOID CMainFrame::OnViewWorkspace() 
{
EFS_BEGIN

	if (m_wndCoolDialogBar.IsWindowVisible())
		ShowControlBar(&m_wndCoolDialogBar, FALSE, FALSE);
	else
		ShowControlBar(&m_wndCoolDialogBar, TRUE, FALSE);	

EFS_END
}

VOID CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
EFS_BEGIN

	if (m_wndCoolDialogBar.IsWindowVisible())
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);

EFS_END
}

VOID CMainFrame::OnViewToolbartable() 
{
EFS_BEGIN

	if (m_wndToolBarTable.IsWindowVisible())
		ShowControlBar(&m_wndToolBarTable, FALSE, FALSE);
	else
		ShowControlBar(&m_wndToolBarTable, TRUE, FALSE);

EFS_END
}

VOID CMainFrame::OnUpdateViewToolbartable(CCmdUI* pCmdUI) 
{
EFS_BEGIN

	if (m_wndToolBarTable.IsWindowVisible())
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);

EFS_END
}

UINT CMainFrame::OnReloadBroker(LPARAM lParam, WPARAM wParam)
{
EFS_BEGIN

	((CSIReportEditorView*)GetActiveView())->InitSIReportEditor();
	return 0;

EFS_END
return 0;
}
VOID CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CESL_DataMgr dm1;
	dm1.ORACLE_DisconnectDB();

	CFrameWnd::OnClose();
}
