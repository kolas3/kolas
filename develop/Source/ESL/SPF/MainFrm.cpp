// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SPF.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_COMMAND(ID_MODIFY_PROJECT, OnModifyProject)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_COMMAND(ID_TASK_NEW, OnTaskNew)
	ON_WM_TIMER()
	ON_COMMAND(ID_MODIFY_TASK, OnModifyTask)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_INIT_SQL,Init_SPF)

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
	ReadCfgFile.ReadCfgFile(_T(""));
	ProjectDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	TaskDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	GroupDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

}

CMainFrame::~CMainFrame()
{
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |CBRS_TOOLTIPS | CBRS_FLYBY ); // TODO: Remove this if you don't want tool tips


	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
    


	// STATEDLG
    if (!m_wndStateBar.Create(this, &m_StateDlg, CString(_T("CONTROL BOX")), IDD_STATEDLG))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }

	m_wndStateBar.SetBarStyle(m_wndStateBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndStateBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndStateBar,AFX_IDW_DOCKBAR_BOTTOM );
    
	
	m_wndStateBar.SetFocus();
	
	
	// TOOLDLG
   if (!m_wndCoolDialogBar.Create(this, &m_dlg, CString(_T("CONTROL BOX")), IDD_TOOLDLG))
    {
        TRACE0("Failed to create dialogbar\n");
        return -1;      // fail to create
    }

	m_wndCoolDialogBar.SetBarStyle(m_wndCoolDialogBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndCoolDialogBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndCoolDialogBar,AFX_IDW_DOCKBAR_LEFT);

	SetImageList();
//	ShowWindow(SW_SHOWMAXIMIZED);

	
	m_dlg.TaskDM = &TaskDM;
	m_dlg.GroupDM = &GroupDM;
	m_dlg.pMainFrame = this; 
	m_dlg.m_Sql = &m_Sql;
	m_dlg.m_Load_Type = &m_Load_Type;
	m_dlg.m_TASK = &m_TASK;
	m_dlg.m_TASKDESC = &m_TASKDESC;
	m_dlg.m_GroupList = &m_GroupList;

	SetTimer(0,100,NULL);

	return 0;
}

VOID CMainFrame::SetImageList()
{
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
	for (INT i = 1; i <= 10; i++)
	{
		strFileName.Format(_T("..\\bmp\\SPF\\btn%02d_n.bmp"), i);
		if (!CheckFileExist(strFileName)) return;
		
		bmpNormal.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		strFileName.Format(_T("..\\bmp\\SPF\\btn%02d_n.bmp"), i);
		if (!CheckFileExist(strFileName)) return;

		bmpHot.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		strFileName.Format(_T("..\\bmp\\SPF\\btn%02d_n.bmp"), i);
		if (!CheckFileExist(strFileName)) return;

		bmpDisabled.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		ReplaceBackgroundColor(bmpNormal);
		ReplaceBackgroundColor(bmpHot);
		ReplaceBackgroundColor(bmpDisabled);

		m_imgListToolBarNoaml.Add(&bmpNormal, RGB(0, 0, 0));
		m_imgListToolBarHot.Add(&bmpHot, RGB(0, 0, 0));
		m_imgListToolBarDisabled.Add(&bmpDisabled, RGB(0, 0, 0));
		
		bmpNormal.Detach();
		bmpHot.Detach();
		bmpDisabled.Detach();
	}

	CToolBarCtrl &rpToolBarCtrl = m_wndToolBar.GetToolBarCtrl();

	rpToolBarCtrl.SetImageList(&m_imgListToolBarNoaml);
	rpToolBarCtrl.SetHotImageList(&m_imgListToolBarHot);
	rpToolBarCtrl.SetDisabledImageList(&m_imgListToolBarDisabled);

}
BOOL CMainFrame::CheckFileExist(CString strFileName)
{
	CFile file;
	if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary))
	{
		return FALSE;
	}
	else 
		file.Close();

	return TRUE;
}
VOID CMainFrame::ReplaceBackgroundColor(CBitmap& ioBM)
{
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
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
VOID CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

VOID CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


VOID CMainFrame::OnProjectNew() 
{
	// TODO: Add your command handler code here
	CSPF_NEW_PROJECT dlg;

	if(dlg.DoModal() == IDOK)
	{
		CString Qry;

		Qry.Format(_T("INSERT INTO SPF_PROJECT_TBL VALUES ( %s.nextval , '%s' , '%s' ); "),ReadCfgFile.m_sSequence ,dlg.m_NAME,dlg.m_DESC);  

		ProjectDM.AddFrame(Qry);
		ProjectDM.SendFrame(TRUE);
		ProjectDM.EndFrame();
	}
	
}

VOID CMainFrame::OnModifyProject() 
{
	// TODO: Add your command handler code here
	CSPF_NEW_PROJECT dlg;
	
	dlg.m_NAME = m_ProjectName; 
	dlg.m_DESC = m_ProjectDesc;
	
	if(dlg.DoModal() == IDOK)
	{	
		CString Qry;

		Qry.Format(_T("UPDATE SPF_PROJECT_TBL SET  NAME = '%s' , DESCRIP = '%s' WHERE NAME = '%s'; "),dlg.m_NAME ,dlg.m_DESC,m_ProjectName);  

		ProjectDM.AddFrame(Qry);

		Qry.Format(_T("UPDATE SPF_TASK_TBL SET  PROJECT_ALIAS = '%s'  WHERE PROJECT_ALIAS = '%s'; "),dlg.m_NAME ,m_ProjectName);  

		ProjectDM.AddFrame(Qry);

		ProjectDM.SendFrame(TRUE);
		ProjectDM.EndFrame();

		m_ProjectName=dlg.m_NAME; 
		m_ProjectDesc=dlg.m_DESC;

	}


}


VOID CMainFrame::OnProjectOpen() 
{
	// TODO: Add your command handler code here
	CSPFOpenProject dlg;
	dlg.ProjectDM = &ProjectDM;
	if( dlg.DoModal() == IDOK)
	{
		
		m_ProjectName = dlg.m_ProjectName; 
		m_ProjectDesc = dlg.m_ProjectDesc;

		Load_Task();
	}

	

}

INT CMainFrame::Load_Task()
{

	m_ProjectName.TrimLeft();
	m_ProjectName.TrimRight();
	m_TASK = _T("");

	if( m_ProjectName.GetLength() != 0 )
	{
		CString Qry;
		CString Tmp,TmpQry;
		INT Count=0;
		
		Qry.Format(_T("Select  PK,ALIAS,PROJECT_ALIAS,DESCRIP ")
			_T(" from SPF_TASK_TBL WHERE PROJECT_ALIAS = '%s'"),m_ProjectName);

		TaskDM.RestructDataManager(Qry);
		
		Count = TaskDM.GetRowCount();
		if(Count == 0)
		{
			m_dlg.m_ProjectName = m_ProjectName;
			m_dlg.Init_Tree();

			return 0;
		}

		TmpQry = _T(" WHERE TASK_ALIAS in (");

		for ( INT i =0 ; i < Count ; i++)
		{
			Tmp.Format(_T(" '%s' "),TaskDM.GetCellData(i, 1));

			if( i != Count-1)
				Tmp += _T(",");

			TmpQry += Tmp;
		}

		TmpQry += _T(" )");
		
		Qry = _T("Select PK,ALIAS,TASK_ALIAS,DESCRIP,TOP,BOTTOM,LEFT,RIGHT FROM SPF_GROUP_TBL ");
		Qry += TmpQry;

		GroupDM.RestructDataManager(Qry);

		m_dlg.m_ProjectName = m_ProjectName;
		m_dlg.Init_Tree();

	}

	return 0;
}

VOID CMainFrame::OnTaskNew() 
{
	// TODO: Add your command handler code here

	m_ProjectName.TrimLeft();
	m_ProjectName.TrimRight();

	if( m_ProjectName.GetLength() == 0 )
	{

		AfxMessageBox(_T("Project 가 없습니다."));
		return ;
	}

	SPFInsertTask dlg;

	if(dlg.DoModal() == IDOK)
	{
	
		CString Qry;

		Qry.Format(_T("INSERT INTO SPF_TASK_TBL VALUES")
			_T("( %s.nextval , '%s','%s' , '%s' ); "),ReadCfgFile.m_sSequence  ,dlg.m_NAME ,m_ProjectName ,dlg.m_DESC);  

		TaskDM.AddFrame(Qry);

		TaskDM.SendFrame(TRUE);
		TaskDM.EndFrame();
		
		Load_Task();

	}

}

VOID CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);
	OnProjectOpen();
	

	CFrameWnd::OnTimer(nIDEvent);
}

VOID CMainFrame::Init_SPF( LPARAM l ,WPARAM w)
{

	CWnd * pView = GetActiveView();

	::SendMessage(pView->GetSafeHwnd(),MSG_INIT_SPF,NULL,NULL);
}

VOID CMainFrame::OnModifyTask() 
{
	// TODO: Add your command handler code here
	m_TASK.TrimLeft();
	m_TASK.TrimRight();

	if( m_ProjectName.GetLength() == 0 )
	{

		AfxMessageBox(_T("선택하신 TASK 가 없습니다."));
		return ;
	}

	SPFInsertTask dlg;

	dlg.m_NAME = m_TASK;
	dlg.m_DESC = m_TASKDESC;

	if(dlg.DoModal() == IDOK)
	{
	
		CString Qry;

		Qry.Format(_T("UPDATE SPF_TASK_TBL SET ")
			_T(" ALIAS = '%s' , DESCRIP = '%s' WHERE ALIAS = '%s' ; "),dlg.m_NAME ,dlg.m_DESC ,m_TASK);  
		TaskDM.AddFrame(Qry);

		Qry.Format(_T("update SPF_GROUP_TBL set TASK_ALIAS='%s' where TASK_ALIAS = '%s'; "),dlg.m_NAME ,m_TASK);  
		TaskDM.AddFrame(Qry);


		Qry.Format(_T("update SPF_STATE_TBL set TASK_ALIAS='%s' where TASK_ALIAS = '%s'; "),dlg.m_NAME ,m_TASK);  
		TaskDM.AddFrame(Qry);

		Qry.Format(_T("update SPF_PATH_TBL set TASK_ALIAS='%s' where TASK_ALIAS = '%s'; "),dlg.m_NAME ,m_TASK);  
		TaskDM.AddFrame(Qry);


		TaskDM.SendFrame(TRUE);
		TaskDM.EndFrame();
		
		Load_Task();

	}	

}

VOID CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CESL_DataMgr dm1;
	while(TRUE) {
		if (dm1.ORACLE_GetConnectedDB() == TRUE) {
			dm1.ORACLE_Commit();
			dm1.ORACLE_DisconnectDB();
		} else {
			break;
		}
	}	
	CFrameWnd::OnClose();
}
