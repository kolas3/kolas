// SPF_MENU_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "flowmaker.h"
#include "SPF_MENU_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPF_MENU_INSERT dialog


CSPF_MENU_INSERT::CSPF_MENU_INSERT(CWnd* pParent /*=NULL*/)
	: CDialog(CSPF_MENU_INSERT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPF_MENU_INSERT)
	m_MENU_ALIAS = _T("");
	//}}AFX_DATA_INIT
	ReadCfgFile.ReadCfgFile(_T(""));
	MenuDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// KOL.UDF.022
	m_pGrid = NULL;
	m_MENU_NAME = _T("");
	m_PROJECT_ALIAS = _T("");
	m_GROUP_ALIAS = _T("");
	m_PATH_ALIAS = _T("");
}

CSPF_MENU_INSERT::~CSPF_MENU_INSERT()
{
	delete m_pGrid;
}


VOID CSPF_MENU_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPF_MENU_INSERT)
	DDX_Text(pDX, IDC_MENU_ALIAS, m_MENU_ALIAS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPF_MENU_INSERT, CDialog)
	//{{AFX_MSG_MAP(CSPF_MENU_INSERT)
	ON_BN_CLICKED(IDC_PROJECT_SEARCH, OnProjectSearch)
	ON_BN_CLICKED(IDC_MENU_INSERT, OnMenuInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPF_MENU_INSERT message handlers

BOOL CSPF_MENU_INSERT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gMENU,this);
	
	Init_Grid();
	Load_Menu(_T(""));
	MenuDMtoGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSPF_MENU_INSERT::Load_Menu(CString Qry)
{
	
	CString strQry;

	strQry.Format(_T("Select  PK,ALIAS,CAPTION from ESL_MENU_TBL WHERE USER_ID='Administrator' "));

	Qry.TrimLeft();
	Qry.TrimRight();

	if( Qry.GetLength() != 0 )
	{

		strQry += _T(" AND ALIAS LIKE '%");
		strQry += Qry;
		strQry += _T("%'");

	}


	MenuDM.RestructDataManager(strQry);

}

VOID CSPF_MENU_INSERT::MenuDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = MenuDM.GetRowCount();                                                                                                                                                                                          
	if( !Count )
	{
		m_pGrid->SetRows(2);

		m_pGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pGrid->SetTextMatrix(i+1, 2, _T(""));
		
		return ;
	}	

	m_pGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGrid->SetTextMatrix(i+1, 1, MenuDM.GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, MenuDM.GetCellData(i, 2));

	}	
	
	return ;

}

VOID CSPF_MENU_INSERT::Init_Grid()
{
	m_pGrid->SetCols(0,3);
	m_pGrid->SetColumnInfo(0-1, _T(""),			0,		4);
	m_pGrid->SetColumnInfo(1-1, _T("MENU ALIAS"),	130,		1);
	m_pGrid->SetColumnInfo(2-1, _T("CAPTION")		   ,120,	4);

}

VOID CSPF_MENU_INSERT::OnProjectSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	Load_Menu(m_MENU_ALIAS);
	MenuDMtoGrid();


}

BEGIN_EVENTSINK_MAP(CSPF_MENU_INSERT, CDialog)
    //{{AFX_EVENTSINK_MAP(CSPF_MENU_INSERT)
	ON_EVENT(CSPF_MENU_INSERT, IDC_gMENU, -600 /* Click */, OnClickgMENU, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSPF_MENU_INSERT::OnClickgMENU() 
{
	// TODO: Add your control notification handler code here
	m_MENU_NAME = _T("");
	
	INT row = m_pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		m_pGrid->SetReverse(row-1);
		m_MENU_NAME = m_pGrid->GetAt(row-1,0);
	}
		
}

VOID CSPF_MENU_INSERT::OnMenuInsert() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	

	m_MENU_NAME.TrimLeft();
	m_MENU_NAME.TrimRight();

	if(m_MENU_NAME != _T(""))
	{
		MenuDM.StartFrame();

		CString Qry;
		Qry.Format(_T("INSERT INTO SPF_PATH_RESOURCE_TBL(PK,PROJECT_ALIAS,GROUP_ALIAS,PATH_ALIAS,RESOURCE_TYPE,MENU_ALIAS) VALUES ( %s.nextval , '%s' , '%s' ,'%s','%s','%s' ); "),ReadCfgFile.m_sSequence ,m_PROJECT_ALIAS,m_GROUP_ALIAS,m_PATH_ALIAS,_T("M"),m_MENU_NAME);  

		MenuDM.AddFrame(Qry);
		MenuDM.SendFrame(TRUE);
		MenuDM.EndFrame();
	}
	else
	{
		AfxMessageBox(_T("Menu is Not Selected !!!"));	
		
	}

	CDialog::OnOK();


}

BOOL CSPF_MENU_INSERT::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {
		OnProjectSearch();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
