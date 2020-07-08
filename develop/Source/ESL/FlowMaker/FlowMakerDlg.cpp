// FlowMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FlowMaker.h"
#include "FlowMakerDlg.h"
#include "SPFOpenProject.h"
#include "SPF_NEW_GROUP.h"
#include "SPF_NEW_PATH.h"
#include "SPF_SCREEN_MGR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlowMakerDlg dialog

CFlowMakerDlg::CFlowMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlowMakerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlowMakerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ReadCfgFile.ReadCfgFile(_T(""));
	ProjectDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	GroupDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	PathDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// KOL.UDF.022
    m_ProjectName = _T("");
	m_ProjectDesc = _T("");
	m_GroupName = _T("");
	m_TaskName = _T("");
	m_PathName = _T("");
	m_PATH_DESC = _T("");
	m_pGroupGrid = NULL;
	m_pPathGrid = NULL;
}

CFlowMakerDlg::~CFlowMakerDlg()
{
	CESL_DataMgr dm1;
	dm1.ORACLE_DisconnectDB();

	delete m_pPathGrid ;
	delete m_pGroupGrid;
}


VOID CFlowMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlowMakerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlowMakerDlg, CDialog)
	//{{AFX_MSG_MAP(CFlowMakerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_PROJECT, OnOpenProject)
	ON_BN_CLICKED(IDC_INSERT_GROUP, OnInsertGroup)
	ON_BN_CLICKED(IDC_INSERT_PATH, OnInsertPath)
	ON_BN_CLICKED(IDC_RESOURCE_MGR, OnResourceMgr)
	ON_BN_CLICKED(IDC_DELETE_PATH, OnDeletePath)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE_GROUP, OnDeleteGroup)
	ON_BN_CLICKED(IDC_MODIFY_PATH, OnModifyPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlowMakerDlg message handlers

BOOL CFlowMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_pGroupGrid = new CESL_Grid(NULL);
	m_pPathGrid = new CESL_Grid(NULL);

	m_pGroupGrid->SubclassDlgItem(IDC_GRID_GROUP, this);
	m_pPathGrid->SubclassDlgItem(IDC_GRID_PATH, this);
	
	Init_Grid_Group();
	Init_Grid_Path();

	CSPFOpenProject dlg;
	dlg.ProjectDM = &ProjectDM;
	if( dlg.DoModal() == IDOK)
	{
		
		m_ProjectName = dlg.m_ProjectName; 
		m_ProjectDesc = dlg.m_ProjectDesc;

		Load_GROUP();
	}



	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CFlowMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CFlowMakerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlowMakerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CFlowMakerDlg::OnOpenProject() 
{
	// TODO: Add your control notification handler code here
	CSPFOpenProject dlg;
	dlg.ProjectDM = &ProjectDM;
	if( dlg.DoModal() == IDOK)
	{
		
		m_ProjectName = dlg.m_ProjectName; 
		m_ProjectDesc = dlg.m_ProjectDesc;

		Load_GROUP();
	}

	
}

VOID CFlowMakerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

INT CFlowMakerDlg::Load_GROUP()
{
	CString strQry;

	strQry.Format(_T("Select  PK,ALIAS,DESCRIP,GROUP_TYPE,TASK_ALIAS from SPF_GROUP_TBL "));

	m_ProjectName.TrimLeft();
	m_ProjectName.TrimRight();

	if( m_ProjectName.GetLength() != 0 )
	{
//		strQry += _T(" where TASK_ALIAS IN(select ALIAS from SPF_TASK_TBL where PROJECT_ALIAS ='")+m_ProjectName+_T("' )");
		strQry += _T(" where PROJECT_ALIAS  ='")+m_ProjectName+_T("'");

		GroupDM.RestructDataManager(strQry);

		GroupDMtoGrid();
	}



	return 0;
}

VOID CFlowMakerDlg::Init_Grid_Group()
{

	m_pGroupGrid->SetCols(0,5);
	m_pGroupGrid->SetColumnInfo(0-1, _T(""),			0,		4);
	m_pGroupGrid->SetColumnInfo(1-1, _T("GROUP ALIAS"),	100,		4);
	m_pGroupGrid->SetColumnInfo(2-1, _T("DESC")		   ,110,	4);
	m_pGroupGrid->SetColumnInfo(3-1, _T("TYPE")        ,40,		4);
	m_pGroupGrid->SetColumnInfo(4-1, _T("")        ,0,		4);

}
VOID CFlowMakerDlg::Init_Grid_Path()
{

	m_pPathGrid->SetCols(0,5);
	m_pPathGrid->SetColumnInfo(0-1, _T(""),				0,		4);
	m_pPathGrid->SetColumnInfo(1-1, _T("선택"),			40,		4);
	m_pPathGrid->SetColumnInfo(2-1, _T("PATH ALIAS"),	110,	4);
	m_pPathGrid->SetColumnInfo(3-1, _T("DESC")		   ,80,	4);
	m_pPathGrid->SetColumnInfo(4-1, _T("")				 ,0,		4);

}

INT CFlowMakerDlg::GroupDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = GroupDM.GetRowCount();
	if( !Count )
	{
		m_pGroupGrid->SetRows(2);

		m_pGroupGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGroupGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pGroupGrid->SetTextMatrix(i+1, 2, _T(""));
		m_pGroupGrid->SetTextMatrix(i+1, 3, _T(""));
		m_pGroupGrid->SetTextMatrix(i+1, 4, _T(""));
		
		return 0;
	}	

	m_pGroupGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pGroupGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGroupGrid->SetTextMatrix(i+1, 1, GroupDM.GetCellData(i, 1));
		m_pGroupGrid->SetTextMatrix(i+1, 2, GroupDM.GetCellData(i, 2));
		m_pGroupGrid->SetTextMatrix(i+1, 3, GroupDM.GetCellData(i, 3));
		m_pGroupGrid->SetTextMatrix(i+1, 4, GroupDM.GetCellData(i, 4));

	}	
	
	return 0;


}
INT CFlowMakerDlg::PathDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = PathDM.GetRowCount();                                                                                                                                                                                          
	if( !Count )
	{
		m_pPathGrid->SetRows(2);

		m_pPathGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pPathGrid->SetTextMatrix(i+1, 2, _T(""));
		m_pPathGrid->SetTextMatrix(i+1, 3, _T(""));
		m_pPathGrid->SetTextMatrix(i+1, 4, _T(""));
		
		return 0;
	}	

	m_pPathGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pPathGrid->SetTextMatrix(i+1, 1, PathDM.GetCellData(i, 4));
		m_pPathGrid->SetTextMatrix(i+1, 2, PathDM.GetCellData(i, 1));
		m_pPathGrid->SetTextMatrix(i+1, 3, PathDM.GetCellData(i, 2));
		m_pPathGrid->SetTextMatrix(i+1, 4, PathDM.GetCellData(i, 3));

	}	
	
	return 0;


}

BEGIN_EVENTSINK_MAP(CFlowMakerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFlowMakerDlg)
	ON_EVENT(CFlowMakerDlg, IDC_GRID_GROUP, -600 /* Click */, OnClickGridGroup, VTS_NONE)
	ON_EVENT(CFlowMakerDlg, IDC_GRID_GROUP, -601 /* DblClick */, OnDblClickGridGroup, VTS_NONE)
	ON_EVENT(CFlowMakerDlg, IDC_GRID_PATH, -605 /* MouseDown */, OnMouseDownGridPath, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CFlowMakerDlg::OnClickGridGroup() 
{
	// TODO: Add your control notification handler code here
	m_GroupName = _T("");
	
	INT row = m_pGroupGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		m_pGroupGrid->SetReverse(row-1);
		m_GroupName = m_pGroupGrid->GetAt(row-1,0);
		m_TaskName = m_pGroupGrid->GetAt(row-1,3);
	}

	Load_PATH();
}

VOID CFlowMakerDlg::OnDblClickGridGroup() 
{
/*
	// TODO: Add your control notification handler code here
	m_GroupName.TrimLeft();
	m_GroupName.TrimRight();

	if( m_GroupName != _T(""))
	{
		Load_PATH();
	}
*/
}

INT CFlowMakerDlg::Load_PATH()
{
	CString strQry;

	strQry.Format(_T("Select  PK,ALIAS,DESCRIP,TASK_ALIAS,ISACTIVATED,DESCRIP from SPF_PATH_TBL "));

	m_GroupName.TrimLeft();
	m_GroupName.TrimRight();

	if( m_GroupName.GetLength() != 0 )
	{
		strQry += _T(" where PROJECT_ALIAS ='")+m_ProjectName+_T("' AND GROUP_ALIAS ='") + m_GroupName +_T("'");

		PathDM.RestructDataManager(strQry);

		PathDMtoGrid();
	}


	return 0;
}

VOID CFlowMakerDlg::OnMouseDownGridPath(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	CString Tmp;
	INT nRow=0;
	INT nCol=0;
	
	nRow = m_pPathGrid->GetRow();
	nCol = m_pPathGrid->GetCol();

	if ( nCol == 1 )
	{
		Tmp = m_pPathGrid->GetAt(nRow-1,nCol-1);

		if( Tmp == _T("T"))
			m_pPathGrid->SetTextMatrix(nRow, nCol, _T("F"));
		else 
			m_pPathGrid->SetTextMatrix(nRow, nCol, _T("T"));

	}
	
	if (nRow == 0) return;
	if (nRow > 0) {
		m_pPathGrid->SetReverse(nRow-1);
		m_PathName = m_pPathGrid->GetAt(nRow-1,1);
		m_PATH_DESC =  PathDM.GetCellData(nRow-1, 5);
	}


}

VOID CFlowMakerDlg::OnInsertGroup() 
{
	// TODO: Add your control notification handler code here
	CSPF_NEW_GROUP Dlg;
	Dlg.m_PROJECT_NAME = m_ProjectName;
	
	if( Dlg.DoModal() == IDOK)
	{
		Load_GROUP();
	}

}

VOID CFlowMakerDlg::OnInsertPath() 
{
	// TODO: Add your control notification handler code here
	CSPF_NEW_PATH Dlg;
	Dlg.m_PROJECT_NAME = m_ProjectName;
	Dlg.m_GROUP_NAME = m_GroupName;

	if( Dlg.DoModal() == IDOK)
	{
		Load_PATH();
	}
	
}

VOID CFlowMakerDlg::OnResourceMgr() 
{
	// TODO: Add your control notification handler code here

	if( m_PathName == _T("") )
	{
		AfxMessageBox(_T("PATH를 선택 하여 주십시요")); 
		return;
	}
	
	CSPF_SCREEN_MGR Dlg;

	Dlg.m_PROJECT_ALIAS = m_ProjectName;
	Dlg.m_GROUP_ALIAS = m_GroupName;
	Dlg.m_PATH_ALIAS = m_PathName;


	if( Dlg.DoModal() == IDOK)
	{

	}
	
}

VOID CFlowMakerDlg::OnDeletePath() 
{
	// TODO: Add your control notification handler code here
	
	if(m_PathName != _T(""))
	{

		if(AfxMessageBox(_T("해당 OBJECT와 관련 메뉴 리소스 전체가 삭제 됩니다. 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			PathDM.StartFrame();
			CString Qry,Qry1;
			Qry.Format(_T("DELETE FROM SPF_PATH_TBL WHERE PROJECT_ALIAS = '%s' AND GROUP_ALIAS = '%s' AND ALIAS = '%s' ;"),m_ProjectName,m_GroupName,m_PathName);  
			PathDM.AddFrame(Qry);
			Qry1 = _T("DELETE FROM SPF_PATH_RESOURCE_TBL WHERE PATH_ALIAS ='")+ m_PathName + _T("' AND PROJECT_ALIAS='")+m_ProjectName+_T("';");
			PathDM.AddFrame(Qry1);
			//원상복귀 시작  
			Qry.Format(_T("UPDATE ESL_MENU_TBL SET SPF_USE_FLAG = 'y';"));  			
			PathDM.AddFrame(Qry);

			Qry.Format(_T("UPDATE ESL_TOOLBAR_TBL SET SPF_USE_FLAG = 'y';"));  			
			PathDM.AddFrame(Qry);
			//원상복귀 끝			
			PathDM.SendFrame(TRUE);
			PathDM.EndFrame();

			Load_PATH();
		}


	}
	else
	{
		AfxMessageBox(_T("PATH is Not Selected !!!"));	
		
	}



}

VOID CFlowMakerDlg::OnSave() 
{
	// TODO: Add your control notification handler code here

	if( m_ProjectName == _T("") && m_GroupName == _T(""))
	{
		AfxMessageBox(_T("선택을 해 주십시요"));
	}


	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	CString Qry,Tmp,strPATH;
	INT Count;
	INT i=0;

	Count = PathDM.GetRowCount();                                                                                                                                                                                          
	if( !Count )
	{
		return ;
	}	

	DM.StartFrame();

	for (  i=0 ; i < Count ; i++)
	{
		Tmp = m_pPathGrid->GetAt(i,0);
		strPATH = m_pPathGrid->GetAt(i,1);
		Qry = _T("UPDATE SPF_PATH_TBL SET ISACTIVATED='")+Tmp+_T("'");
		Tmp = _T(" WHERE PROJECT_ALIAS ='")+m_ProjectName+_T("' AND GROUP_ALIAS ='") + m_GroupName+_T("' AND ALIAS ='")+strPATH+_T("';");
		Qry += Tmp;

		DM.AddFrame(Qry);
	}	
	
	DM.SendFrame();
	DM.EndFrame();

	SaveToAccess();

	AfxMessageBox(_T("저장되었습니다."));

	return ;

}

VOID CFlowMakerDlg::SaveToAccess()
{

	INT Count =0;
	CString strQry,Tmp;
	CESL_DataMgr DM;

	DM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);


	//리소스를 초기화 시킨다. 
	strQry = _T("SELECT MENU_ALIAS,TOOL_BUTTON_ALIAS,RESOURCE_TYPE FROM SPF_PATH_RESOURCE_TBL ");
	Tmp = _T(" where PROJECT_ALIAS  ='")+m_ProjectName+_T("'");
	strQry += Tmp;

	DM.RestructDataManager(strQry);
	Count = DM.GetRowCount(); 

	DM.StartFrame();

	for (INT i = 0 ; i < Count ; i++)
	{
		CString strType,strMenu,strButton;

		strMenu = DM.GetCellData(i,0);
		strButton = DM.GetCellData(i,1);
		strType = DM.GetCellData(i,2);

		if( strType == _T("M"))
		{
			strQry.Format(_T(" Update ESL_MENU_TBL SET SPF_USE_FLAG = 'n' WHERE ALIAS = '%s' ;"),strMenu);
			DM.AddFrame(strQry);
		}
		if( strType == _T("T"))
		{
			strQry.Format(_T(" Update ESL_TOOLBAR_TBL SET SPF_USE_FLAG = 'n' WHERE MENU_ALIAS = '%s' AND ALIAS = '%s' ;"),strMenu,strButton);
			DM.AddFrame(strQry);
		}

	}

	DM.SendFrame();
	DM.EndFrame();


	//사용 하는 리소스를 설정 한다.
	strQry = _T("SELECT R.MENU_ALIAS,R.TOOL_BUTTON_ALIAS,R.RESOURCE_TYPE FROM SPF_PATH_RESOURCE_TBL R , SPF_PATH_TBL P ");
	Tmp = _T(" where R.PROJECT_ALIAS  ='")+m_ProjectName+_T("' AND P.ISACTIVATED = 'T' AND P.ALIAS = R.PATH_ALIAS AND R.PROJECT_ALIAS = P.PROJECT_ALIAS AND R.GROUP_ALIAS = P.GROUP_ALIAS");
	strQry += Tmp;

	DM.RestructDataManager(strQry);
	Count = DM.GetRowCount(); 

	DM.StartFrame();

	for (i = 0 ; i < Count ; i++)
	{
		CString strType,strMenu,strButton;

		strMenu = DM.GetCellData(i,0);
		strButton = DM.GetCellData(i,1);
		strType = DM.GetCellData(i,2);

		if( strType == _T("M"))
		{
			strQry.Format(_T(" Update ESL_MENU_TBL SET SPF_USE_FLAG = 'y' WHERE ALIAS = '%s' ;"),strMenu);
			DM.AddFrame(strQry);
		}
		if( strType == _T("T"))
		{
			strQry.Format(_T(" Update ESL_TOOLBAR_TBL SET SPF_USE_FLAG = 'y' WHERE MENU_ALIAS = '%s' AND ALIAS = '%s' ;"),strMenu,strButton);
			DM.AddFrame(strQry);
		}

	}

	DM.SendFrame();
	DM.EndFrame();


}

VOID CFlowMakerDlg::OnDeleteGroup() 
{
	// TODO: Add your control notification handler code here
	if(m_GroupName != _T(""))
	{

		if(AfxMessageBox(_T("해당 OBJECT와 관련 메뉴 리소스 전체가 삭제 됩니다. 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			PathDM.StartFrame();

			CString Qry,Qry1,Qry2;
			Qry.Format(_T("DELETE FROM SPF_GROUP_TBL WHERE PROJECT_ALIAS = '%s' AND ALIAS = '%s' ;"),m_ProjectName,m_GroupName);  
			PathDM.AddFrame(Qry);
			
			Qry1 = _T(" DELETE FROM SPF_PATH_TBL WHERE GROUP_ALIAS ='")+ m_GroupName + _T("' AND PROJECT_ALIAS='")+m_ProjectName+_T("';");
			PathDM.AddFrame(Qry1);
			
			Qry2 = _T(" DELETE FROM SPF_PATH_RESOURCE_TBL WHERE GROUP_ALIAS ='")+ m_GroupName + _T("' AND PROJECT_ALIAS='")+m_ProjectName+_T("';");
			PathDM.AddFrame(Qry2);
			//원상복귀 시작  
			Qry.Format(_T("UPDATE FROM ESL_MENU_TBL WHERE SPF_USE_FLAG = 'y';"));  			
			PathDM.AddFrame(Qry);

			Qry.Format(_T("UPDATE FROM ESL_TOOLBAR_TBL WHERE SPF_USE_FLAG = 'y';"));  			
			PathDM.AddFrame(Qry);
			//원상복귀 끝

			PathDM.SendFrame(TRUE);
			PathDM.EndFrame();

			Load_GROUP();
			Load_PATH();
		}


	}
	else
	{
		AfxMessageBox(_T("PATH is Not Selected !!!"));	
		
	}
	
}

VOID CFlowMakerDlg::OnModifyPath() 
{
	// TODO: Add your control notification handler code here

	CSPF_NEW_PATH Dlg;
	Dlg.Modify_Flag = 1;
	Dlg.m_PROJECT_NAME = m_ProjectName;
	Dlg.m_GROUP_NAME = m_GroupName;
	Dlg.m_PATH_NAME = m_PathName;
	Dlg.m_PATH_DESC = m_PATH_DESC;
	Dlg.m_MODIFY_ALIAS = m_PathName;

	if( Dlg.DoModal() == IDOK)
	{
		Load_PATH();
		m_PathName = Dlg.m_PATH_NAME;
	}	

}
