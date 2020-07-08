// SPFButtonInsert.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "SPFButtonInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPFButtonInsert dialog


CSPFButtonInsert::CSPFButtonInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CSPFButtonInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPFButtonInsert)
	m_Alias = _T("");
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_pButtonGrid = NULL;
	m_Seleted.RemoveAll();
	m_MenuList.RemoveAll();
}

CSPFButtonInsert::~CSPFButtonInsert()
{
	delete m_pButtonGrid;
}

VOID CSPFButtonInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPFButtonInsert)
	DDX_Text(pDX, IDC_Search_Aias, m_Alias);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPFButtonInsert, CDialog)
	//{{AFX_MSG_MAP(CSPFButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPFButtonInsert message handlers

BOOL CSPFButtonInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	INT ids;
	// Grid 초기화
	m_pButtonGrid = new CESL_Grid(NULL);
	m_pButtonGrid->SubclassDlgItem(IDC_gBUTTON, this);
	ids=m_pButtonGrid->SetSelectionInfo('S',_T("V"), 0);
	m_pButtonGrid->m_bCheck = true;
	// DM 초기화 
	ReadCfgFile.ReadCfgFile(_T(""));
	m_ButtonDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	

	InitGrid();

	InitDataMgr(_T(""));

	ButtonDMtoGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
INT CSPFButtonInsert::InitGrid()
{

	m_pButtonGrid->SetCols(0,8);
	m_pButtonGrid->SetColumnInfo(0-1, _T(""),				0,			4);
	m_pButtonGrid->SetColumnInfo(1-1, _T("선택"),			30,			4);
	m_pButtonGrid->SetColumnInfo(2-1, _T("Alias"),			80,			1);
	m_pButtonGrid->SetColumnInfo(3-1, _T("Menu_Alias"),		80,			4);
	m_pButtonGrid->SetColumnInfo(4-1, _T("Caption"),		80,			1);
	m_pButtonGrid->SetColumnInfo(5-1, _T("STYLE"),			50,			4);
	m_pButtonGrid->SetColumnInfo(6-1, _T("SM"),				150,		1);
	m_pButtonGrid->SetColumnInfo(7-1, _T("TOOLTIP"),		200,		1);


	return 0;
}

INT CSPFButtonInsert::InitDataMgr(CString m_str)
{

	CString strQry;

	strQry.Format(_T("Select  PK,ALIAS,MENU_ALIAS,CAPTION,STYLE,SM,TOOLTIP from ESL_TOOLBAR_TBL WHERE USER_ID = 'Administrator' "));

	m_str.TrimLeft();
	m_str.TrimRight();


	if(m_MenuList.GetCount() == 0)
	{
		m_ButtonDM.FreeData();
		return 0;
	}

	CString ToolGroup = _T("Select ALIAS from ESL_TOOLGROUP_TBL where ");

	for( INT i =0 ; i < m_MenuList.GetCount() ; i++)
	{

		CString TmpStr;
		TmpStr=m_MenuList.GetAt(m_MenuList.FindIndex(i));

		TmpStr.TrimLeft();
		TmpStr.TrimRight();

		if( i == 0)
		{
			ToolGroup += _T("MENU_ALIAS IN(");
		}

		ToolGroup += _T("'");
		ToolGroup +=TmpStr;

		if( i == m_MenuList.GetCount() -1 )
		{
			ToolGroup += _T("')");
		}
		else
		{
			ToolGroup += _T("',");

		}

	}

	m_ButtonDM.RestructDataManager(ToolGroup);



	for ( INT j = 0 ; j < m_ButtonDM.GetRowCount() ; j++)
	{
			CString TmpStr;
			TmpStr = m_ButtonDM.GetCellData(j,0);

			if( j == 0)
			{
				strQry += _T(" AND ");	
				strQry += _T("MENU_ALIAS IN(");
			}
			

			strQry += _T("'");
			strQry +=TmpStr;

			if( j == m_ButtonDM.GetRowCount() -1 )
				strQry += _T("')");
			else
				strQry += _T("',");

	}

	if ( j != 0 )
			m_ButtonDM.RestructDataManager(strQry);
	else
			m_ButtonDM.FreeData();


	if( m_str.GetLength() != 0 )
	{

		strQry += _T(" AND ALIAS LIKE '%");
		strQry += m_str;
		strQry += _T("%'");

	}



	m_ButtonDM.RestructDataManager(strQry);
	return 0;
}

INT CSPFButtonInsert::ButtonDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = m_ButtonDM.GetRowCount();
	if( !Count )
	{
		m_pButtonGrid->SetRows(2);

		m_pButtonGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 2, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 3, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 4, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 5, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 6, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 7, _T(""));
		
		return 0;
	}	

	m_pButtonGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pButtonGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pButtonGrid->SetTextMatrix(i+1, 2, m_ButtonDM.GetCellData(i, 1));
		m_pButtonGrid->SetTextMatrix(i+1, 3, m_ButtonDM.GetCellData(i, 2));
		m_pButtonGrid->SetTextMatrix(i+1, 4, m_ButtonDM.GetCellData(i, 3));
		m_pButtonGrid->SetTextMatrix(i+1, 5, m_ButtonDM.GetCellData(i, 4));
		m_pButtonGrid->SetTextMatrix(i+1, 6, m_ButtonDM.GetCellData(i, 5));
		m_pButtonGrid->SetTextMatrix(i+1, 7, m_ButtonDM.GetCellData(i, 6));

	}	
	
	return 0;
}

VOID CSPFButtonInsert::OnOK() 
{
	// TODO: Add extra validation here
	INT ids;

	ids = m_pButtonGrid->GetCount();

	for( INT i = 0 ; i < m_pButtonGrid->GetCount() ; i++)
	{

		if( m_pButtonGrid->GetAt(i , 0) == _T("V") )
		{
				CString Button_Alias = m_ButtonDM.GetCellData(i,1);
				m_Seleted.AddTail( Button_Alias );
		}

	}

	CDialog::OnOK();
}

VOID CSPFButtonInsert::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	InitDataMgr(m_Alias);

	ButtonDMtoGrid();
}
