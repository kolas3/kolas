// SPFMenuInsert.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "SPFMenuInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPFMenuInsert dialog


CSPFMenuInsert::CSPFMenuInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CSPFMenuInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPFMenuInsert)
	m_Alias = _T("");
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_pMenuGrid = NULL;
	m_Seleted.RemoveAll();
}

CSPFMenuInsert::~CSPFMenuInsert()
{
	delete m_pMenuGrid;
}


VOID CSPFMenuInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPFMenuInsert)
	DDX_Text(pDX, IDC_Search_Aias, m_Alias);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPFMenuInsert, CDialog)
	//{{AFX_MSG_MAP(CSPFMenuInsert)
	ON_BN_CLICKED(IDC_MENU_SEARCH, OnMenuSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPFMenuInsert message handlers

BOOL CSPFMenuInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	INT ids;
	// Grid 초기화
	m_pMenuGrid = new CESL_Grid(NULL);
	m_pMenuGrid->SubclassDlgItem(IDC_gMENU, this);
	ids=m_pMenuGrid->SetSelectionInfo('S',_T("V"), 0);
	m_pMenuGrid->m_bCheck = true;
	// DM 초기화 
	ReadCfgFile.ReadCfgFile(_T(""));
	m_MenuDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	

	InitGrid();

	InitDataMgr(_T(""));

	MenuDMtoGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CSPFMenuInsert::InitGrid()
{

	m_pMenuGrid->SetCols(0,6);
	m_pMenuGrid->SetColumnInfo(0-1, _T(""),			0,				4);
	m_pMenuGrid->SetColumnInfo(1-1, _T("선택"),			30,			4);
	m_pMenuGrid->SetColumnInfo(2-1, _T("Alias"),		150,		1);
	m_pMenuGrid->SetColumnInfo(3-1, _T("Group_Alias"),	200,		1);
	m_pMenuGrid->SetColumnInfo(4-1, _T("Caption"),		150,		1);
	m_pMenuGrid->SetColumnInfo(5-1, _T("Action"),		50,			4);

	return 0;
}

INT CSPFMenuInsert::InitDataMgr(CString m_str)
{

	CString strQry;

	strQry.Format(_T("Select  PK,ALIAS,GROUP_ALIAS,CAPTION,ACTION from ESL_MENU_TBL WHERE USER_ID = 'Administrator' "));

	m_str.TrimLeft();
	m_str.TrimRight();

	if( m_str.GetLength() != 0 )
	{

		strQry += _T(" AND ALIAS LIKE '%");
		strQry += m_str;
		strQry += _T("%'");

	}



	m_MenuDM.RestructDataManager(strQry);
	return 0;
}

INT CSPFMenuInsert::MenuDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = m_MenuDM.GetRowCount();
	if( !Count )
	{
		m_pMenuGrid->SetRows(2);

		m_pMenuGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 2, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 3, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 4, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 5, _T(""));
		
		return 0;
	}	

	m_pMenuGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pMenuGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pMenuGrid->SetTextMatrix(i+1, 2, m_MenuDM.GetCellData(i, 1));
		m_pMenuGrid->SetTextMatrix(i+1, 3, m_MenuDM.GetCellData(i, 2));
		m_pMenuGrid->SetTextMatrix(i+1, 4, m_MenuDM.GetCellData(i, 3));
		m_pMenuGrid->SetTextMatrix(i+1, 5, m_MenuDM.GetCellData(i, 4));

	}	
	
	return 0;
}

VOID CSPFMenuInsert::OnMenuSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	InitDataMgr(m_Alias);

	MenuDMtoGrid();
}

VOID CSPFMenuInsert::OnOK() 
{
	// TODO: Add extra validation here
	INT ids;

	ids = m_pMenuGrid->GetCount();

	for( INT i = 0 ; i < m_pMenuGrid->GetCount() ; i++)
	{

		if( m_pMenuGrid->GetAt(i , 0) == _T("V") )
		{
				CString Menu_Alias = m_MenuDM.GetCellData(i,1);
				m_Seleted.AddTail( Menu_Alias );
		}

	}

	CDialog::OnOK();
}
