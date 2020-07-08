// SPF_TOOL_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "flowmaker.h"
#include "SPF_TOOL_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPF_TOOL_INSERT dialog


CSPF_TOOL_INSERT::CSPF_TOOL_INSERT(CWnd* pParent /*=NULL*/)
	: CDialog(CSPF_TOOL_INSERT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPF_TOOL_INSERT)
	m_TOOL_ALIAS = _T("");
	//}}AFX_DATA_INIT
	ReadCfgFile.ReadCfgFile(_T(""));
	ToolDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// KOL.UDF.022
	m_pGrid = NULL;
 	m_TOOL_NAME = _T("");
	m_MENU_NAME = _T("");
	m_PROJECT_ALIAS = _T("");
	m_GROUP_ALIAS = _T("");
	m_PATH_ALIAS = _T("");
}

CSPF_TOOL_INSERT::~CSPF_TOOL_INSERT()
{
	delete m_pGrid;
}


VOID CSPF_TOOL_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPF_TOOL_INSERT)
	DDX_Text(pDX, IDC_TOOL_NAME, m_TOOL_ALIAS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPF_TOOL_INSERT, CDialog)
	//{{AFX_MSG_MAP(CSPF_TOOL_INSERT)
	ON_BN_CLICKED(IDC_PROJECT_SEARCH, OnProjectSearch)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPF_TOOL_INSERT message handlers

BOOL CSPF_TOOL_INSERT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gBUTTON,this);
	
	Init_Grid();
	Load_Tool(_T(""));
	ToolDMtoGrid();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSPF_TOOL_INSERT::Load_Tool(CString Qry)
{
	
	CString strQry,strQry1;
	
	strQry1.Format(_T("select MENU_ALIAS FROM SPF_PATH_RESOURCE_TBL WHERE PROJECT_ALIAS = '%s' AND GROUP_ALIAS = '%s' AND PATH_ALIAS = '%s' "),m_PROJECT_ALIAS,m_GROUP_ALIAS,m_PATH_ALIAS);

	strQry.Format(_T("Select  PK,ALIAS,CAPTION,MENU_ALIAS from ESL_TOOLBAR_TBL WHERE USER_ID='Administrator' AND MENU_ALIAS IN ( %s ) "),strQry1);

	Qry.TrimLeft();
	Qry.TrimRight();

	if( Qry.GetLength() != 0 )
	{

		strQry += _T(" AND ALIAS LIKE '%");
		strQry += Qry;
		strQry += _T("%'");

	}


	ToolDM.RestructDataManager(strQry);

}

VOID CSPF_TOOL_INSERT::ToolDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = ToolDM.GetRowCount();                                                                                                                                                                                          
	if( !Count )
	{
		m_pGrid->SetRows(2);

		m_pGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pGrid->SetTextMatrix(i+1, 2, _T(""));
		m_pGrid->SetTextMatrix(i+1, 3, _T(""));
		
		return ;
	}	

	m_pGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGrid->SetTextMatrix(i+1, 1, ToolDM.GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, ToolDM.GetCellData(i, 2));
		m_pGrid->SetTextMatrix(i+1, 3, ToolDM.GetCellData(i, 3));
	}	
	
	return ;

}

VOID CSPF_TOOL_INSERT::Init_Grid()
{
	m_pGrid->SetCols(0,4);
	m_pGrid->SetColumnInfo(0-1, _T("")				 ,0		,		4);
	m_pGrid->SetColumnInfo(1-1, _T("BUTTON ALIAS")	 ,130	,		1);
	m_pGrid->SetColumnInfo(2-1, _T("CAPTION")		 ,120	,		4);
	m_pGrid->SetColumnInfo(3-1, _T("")		 ,0	,		1);
}

VOID CSPF_TOOL_INSERT::OnProjectSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	Load_Tool(m_TOOL_ALIAS);
	ToolDMtoGrid();
}

VOID CSPF_TOOL_INSERT::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	
	m_TOOL_NAME.TrimLeft();
	m_TOOL_NAME.TrimRight();

	if(m_MENU_NAME != _T(""))
	{
		ToolDM.StartFrame();

		CString Qry;
		Qry.Format(_T("INSERT INTO SPF_PATH_RESOURCE_TBL(PK,PROJECT_ALIAS,GROUP_ALIAS,PATH_ALIAS,RESOURCE_TYPE,MENU_ALIAS,TOOL_BUTTON_ALIAS) VALUES ( %s.nextval , '%s' , '%s' ,'%s','%s','%s','%s' ); "),ReadCfgFile.m_sSequence ,m_PROJECT_ALIAS,m_GROUP_ALIAS,m_PATH_ALIAS,_T("T"),m_MENU_NAME,m_TOOL_NAME);  

		ToolDM.AddFrame(Qry);
		ToolDM.SendFrame(TRUE);
		ToolDM.EndFrame();
	}
	else
	{
		AfxMessageBox(_T("Menu is Not Selected !!!"));	
		
	}

	CDialog::OnOK();



	
}

BEGIN_EVENTSINK_MAP(CSPF_TOOL_INSERT, CDialog)
    //{{AFX_EVENTSINK_MAP(CSPF_TOOL_INSERT)
	ON_EVENT(CSPF_TOOL_INSERT, IDC_gBUTTON, -600 /* Click */, OnClickgBUTTON, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSPF_TOOL_INSERT::OnClickgBUTTON() 
{
	// TODO: Add your control notification handler code here
	m_MENU_NAME = _T("");
	m_TOOL_NAME = _T("");
	
	INT row = m_pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		m_pGrid->SetReverse(row-1);
		m_TOOL_NAME = m_pGrid->GetAt(row-1,0);
		m_MENU_NAME = m_pGrid->GetAt(row-1,2);
	}
	
}
