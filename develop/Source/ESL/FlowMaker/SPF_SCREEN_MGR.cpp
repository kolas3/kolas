// SPF_SCREEN_MGR.cpp : implementation file
//

#include "stdafx.h"
#include "flowmaker.h"
#include "SPF_SCREEN_MGR.h"
#include "SPF_MENU_INSERT.h"
#include "SPF_TOOL_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPF_SCREEN_MGR dialog


CSPF_SCREEN_MGR::CSPF_SCREEN_MGR(CWnd* pParent /*=NULL*/)
	: CDialog(CSPF_SCREEN_MGR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPF_SCREEN_MGR)
	m_PATH_VIEW = _T("");
	//}}AFX_DATA_INIT
	ReadCfgFile.ReadCfgFile(_T(""));
	MenuDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	ToolDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// KOL.UDF.022
	m_PROJECT_ALIAS = _T("");
 	m_GROUP_ALIAS = _T("");
	m_PATH_ALIAS = _T("");
	m_MENU_KEY = _T("");
	m_BUTTON_KEY = _T("");
	
	m_pMenuGrid = NULL;
	m_pToolGrid = NULL;

}

CSPF_SCREEN_MGR::~CSPF_SCREEN_MGR()
{
	delete m_pMenuGrid;
	delete m_pToolGrid;
}


VOID CSPF_SCREEN_MGR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPF_SCREEN_MGR)
	DDX_Text(pDX, IDC_PATH_NAME, m_PATH_VIEW);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPF_SCREEN_MGR, CDialog)
	//{{AFX_MSG_MAP(CSPF_SCREEN_MGR)
	ON_BN_CLICKED(IDC_INSERT_MENU, OnInsertMenu)
	ON_BN_CLICKED(IDC_INSERT_TOOL, OnInsertTool)
	ON_BN_CLICKED(IDC_DELETE_MENU, OnDeleteMenu)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPF_SCREEN_MGR message handlers

BOOL CSPF_SCREEN_MGR::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pMenuGrid = new CESL_Grid(NULL);
	m_pMenuGrid->SubclassDlgItem(IDC_gridMENU,this);

	m_pToolGrid = new CESL_Grid(NULL);
	m_pToolGrid->SubclassDlgItem(IDC_gridTOOL,this);
	

	Init_Menu_Grid();
	LOAD_MENU();
	MenuDMtoGrid();

	Init_TOOL_Grid();
	LOAD_TOOL();
	ToolDMtoGrid();

	m_PATH_VIEW = m_PATH_ALIAS;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSPF_SCREEN_MGR::OnInsertMenu() 
{
	// TODO: Add your control notification handler code here

	CSPF_MENU_INSERT dlg;
	
	dlg.m_PROJECT_ALIAS = m_PROJECT_ALIAS;
	dlg.m_GROUP_ALIAS	= m_GROUP_ALIAS;	
	dlg.m_PATH_ALIAS	= m_PATH_ALIAS;

	if( dlg.DoModal() == IDOK)
	{
		LOAD_MENU();
		MenuDMtoGrid();
	}

}


VOID CSPF_SCREEN_MGR::OnInsertTool() 
{
	// TODO: Add your control notification handler code here
	CSPF_TOOL_INSERT dlg;

	dlg.m_PROJECT_ALIAS = m_PROJECT_ALIAS;
	dlg.m_GROUP_ALIAS	= m_GROUP_ALIAS;	
	dlg.m_PATH_ALIAS	= m_PATH_ALIAS;

	if( dlg.DoModal() == IDOK)
	{
		LOAD_TOOL();
		ToolDMtoGrid();
	}	
}

VOID CSPF_SCREEN_MGR::LOAD_MENU()
{
	CString strQry;

	strQry.Format(_T("Select  PK,MENU_ALIAS from SPF_PATH_RESOURCE_TBL WHERE PROJECT_ALIAS='%s' AND GROUP_ALIAS = '%s' AND PATH_ALIAS = '%s' AND RESOURCE_TYPE='M' ") ,m_PROJECT_ALIAS,m_GROUP_ALIAS,m_PATH_ALIAS);

	MenuDM.RestructDataManager(strQry);
}
VOID CSPF_SCREEN_MGR::LOAD_TOOL()
{
	CString strQry;

	strQry.Format(_T("Select  PK,MENU_ALIAS,TOOL_BUTTON_ALIAS from SPF_PATH_RESOURCE_TBL WHERE PROJECT_ALIAS='%s' AND GROUP_ALIAS = '%s' AND PATH_ALIAS = '%s' AND RESOURCE_TYPE='T' "),m_PROJECT_ALIAS,m_GROUP_ALIAS,m_PATH_ALIAS);

	ToolDM.RestructDataManager(strQry);
}

VOID CSPF_SCREEN_MGR::MenuDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = MenuDM.GetRowCount();                                                                                                                                                                                          
	if( !Count )
	{
		m_pMenuGrid->SetRows(2);

		m_pMenuGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 2, _T(""));
		
		return ;
	}	

	m_pMenuGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pMenuGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 1, MenuDM.GetCellData(i, 1));
		m_pMenuGrid->SetTextMatrix(i+1, 2, MenuDM.GetCellData(i, 0));

	}	
	
	return ;

}


VOID CSPF_SCREEN_MGR::ToolDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = ToolDM.GetRowCount();                                                                                                                                                                                          
	if( !Count )
	{
		m_pToolGrid->SetRows(2);

		m_pToolGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pToolGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pToolGrid->SetTextMatrix(i+1, 2, _T(""));
		m_pToolGrid->SetTextMatrix(i+1, 3, _T(""));
		
		return ;
	}	

	m_pToolGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pToolGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pToolGrid->SetTextMatrix(i+1, 1, ToolDM.GetCellData(i, 1));
		m_pToolGrid->SetTextMatrix(i+1, 2, ToolDM.GetCellData(i, 2));
		m_pToolGrid->SetTextMatrix(i+1, 3, ToolDM.GetCellData(i, 0));
	}	
	
	return ;

}

VOID CSPF_SCREEN_MGR::Init_Menu_Grid()
{
	m_pMenuGrid->SetCols(0,3);
	m_pMenuGrid->SetColumnInfo(0-1, _T(""),			0,		4);
	m_pMenuGrid->SetColumnInfo(1-1, _T("MENU ALIAS"),	200,		1);
	m_pMenuGrid->SetColumnInfo(2-1, _T(""),	0,		1);

}
VOID CSPF_SCREEN_MGR::Init_TOOL_Grid()
{
	m_pToolGrid->SetCols(0,4);
	m_pToolGrid->SetColumnInfo(0-1, _T(""),			0,		4);
	m_pToolGrid->SetColumnInfo(1-1, _T("MENU ALIAS"),	200,	1);
	m_pToolGrid->SetColumnInfo(2-1, _T("BUTTON ALIAS"), 200,	1);
	m_pToolGrid->SetColumnInfo(3-1, _T(""), 0,	1);

}

VOID CSPF_SCREEN_MGR::OnDeleteMenu() 
{
	// TODO: Add your control notification handler code here
	


	if(m_MENU_KEY != _T(""))
	{

		if(AfxMessageBox(_T("해당 OBJECT를 삭제 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			MenuDM.StartFrame();

			CString Qry;
			Qry.Format(_T("DELETE FROM SPF_PATH_RESOURCE_TBL WHERE PK = %s;"),m_MENU_KEY);  			
			MenuDM.AddFrame(Qry);
			
			//원상복귀 시작  
			Qry.Format(_T("UPDATE ESL_MENU_TBL SET SPF_USE_FLAG = 'y';"));  			
			MenuDM.AddFrame(Qry);

			Qry.Format(_T("UPDATE ESL_TOOLBAR_TBL SET SPF_USE_FLAG = 'y';"));  			
			MenuDM.AddFrame(Qry);
			//원상복귀 끝

			MenuDM.SendFrame(TRUE);
			MenuDM.EndFrame();

			LOAD_MENU();
			MenuDMtoGrid();

		}

	}
	else
	{
		AfxMessageBox(_T("MENU is Not Selected !!!"));	
		
	}


}

VOID CSPF_SCREEN_MGR::OnDeleteButton() 
{
	// TODO: Add your control notification handler code here
	if(m_BUTTON_KEY != _T(""))
	{

		if(AfxMessageBox(_T("해당 OBJECT를 삭제 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			MenuDM.StartFrame();
		
			CString Qry;
			Qry.Format(_T("DELETE FROM SPF_PATH_RESOURCE_TBL WHERE PK = %s;"),m_BUTTON_KEY);  
			
			MenuDM.AddFrame(Qry);
			//원상복귀 시작  
			Qry.Format(_T("UPDATE  ESL_MENU_TBL SET SPF_USE_FLAG = 'y';"));  			
			MenuDM.AddFrame(Qry);

			Qry.Format(_T("UPDATE  ESL_TOOLBAR_TBL SET SPF_USE_FLAG = 'y';"));  			
			MenuDM.AddFrame(Qry);
			//원상복귀 끝

			MenuDM.SendFrame(TRUE);
			MenuDM.EndFrame();

			LOAD_TOOL();
			ToolDMtoGrid();

		}


	}
	else
	{
		AfxMessageBox(_T("MENU is Not Selected !!!"));	
		
	}

	
}

BEGIN_EVENTSINK_MAP(CSPF_SCREEN_MGR, CDialog)
    //{{AFX_EVENTSINK_MAP(CSPF_SCREEN_MGR)
	ON_EVENT(CSPF_SCREEN_MGR, IDC_gridMENU, -600 /* Click */, OnClickgridMENU, VTS_NONE)
	ON_EVENT(CSPF_SCREEN_MGR, IDC_gridTOOL, -600 /* Click */, OnClickgridTOOL, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSPF_SCREEN_MGR::OnClickgridMENU() 
{
	// TODO: Add your control notification handler code here
		m_MENU_KEY = _T("");
		
		INT row = m_pMenuGrid->GetRow();
		
		if (row == 0) return;
		if (row > 0) {
			m_pMenuGrid->SetReverse(row-1);
			m_MENU_KEY = m_pMenuGrid->GetAt(row-1,1);
		}
}

VOID CSPF_SCREEN_MGR::OnClickgridTOOL() 
{
	// TODO: Add your control notification handler code here
	m_BUTTON_KEY = _T("");
	
	INT row = m_pToolGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		m_pToolGrid->SetReverse(row-1);
		m_BUTTON_KEY = m_pToolGrid->GetAt(row-1,2);
	}
}
