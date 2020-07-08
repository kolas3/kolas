// SPFOpenProject.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "SPFOpenProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPFOpenProject dialog


CSPFOpenProject::CSPFOpenProject(CWnd* pParent /*=NULL*/)
	: CDialog(CSPFOpenProject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPFOpenProject)
	m_NAME = _T("");
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	ProjectDM = NULL;
	m_ProjectName = _T("");
	m_ProjectDesc = _T("");
	m_pGrid = NULL;
}
CSPFOpenProject::~CSPFOpenProject()
{

	delete m_pGrid;
}

VOID CSPFOpenProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPFOpenProject)
	DDX_Text(pDX, IDC_PROJECT_NAME, m_NAME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPFOpenProject, CDialog)
	//{{AFX_MSG_MAP(CSPFOpenProject)
	ON_BN_CLICKED(IDC_PROJECT_SEARCH, OnProjectSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPFOpenProject message handlers

BOOL CSPFOpenProject::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Grid 초기화
	m_pGrid = new CESL_Grid(NULL);

	m_pGrid->SubclassDlgItem(IDC_gPROJECT, this);

	InitGrid();

	InitDataMgr(_T(""));

	ProjectDMtoGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSPFOpenProject::InitGrid()
{

	m_pGrid->SetCols(0,3);
	m_pGrid->SetColumnInfo(0-1, _T(""),			0,				4);
	m_pGrid->SetColumnInfo(1-1, _T("PROJECT NAME"),	160,		4);
	m_pGrid->SetColumnInfo(2-1, _T("DESC")		   ,240,		4);

	return 0;
}

INT CSPFOpenProject::ProjectDMtoGrid()
{
	INT Count;
	INT i=0;

	Count = ProjectDM->GetRowCount();
	if( !Count )
	{
		m_pGrid->SetRows(2);

		m_pGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGrid->SetTextMatrix(i+1, 1, _T(""));
		m_pGrid->SetTextMatrix(i+1, 2, _T(""));
		
		return 0;
	}	

	m_pGrid->SetRows(Count+1);

	for (  i=0 ; i < Count ; i++)
	{
		m_pGrid->SetTextMatrix(i+1, 0, _T(""));
		m_pGrid->SetTextMatrix(i+1, 1, ProjectDM->GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, ProjectDM->GetCellData(i, 2));

	}	
	
	return 0;
}


INT CSPFOpenProject::InitDataMgr(CString m_str)
{

	CString strQry;

	strQry.Format(_T("Select  PK,NAME,DESCRIP from SPF_PROJECT_TBL"));

	m_str.TrimLeft();
	m_str.TrimRight();

	if( m_str.GetLength() != 0 )
	{

		strQry += _T(" WHERE NAME LIKE '%");
		strQry += m_str;
		strQry += _T("%'");

	}



	ProjectDM->RestructDataManager(strQry);
	return 0;
}

VOID CSPFOpenProject::OnProjectSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	InitDataMgr(m_NAME);

	ProjectDMtoGrid();

}

BEGIN_EVENTSINK_MAP(CSPFOpenProject, CDialog)
    //{{AFX_EVENTSINK_MAP(CSPFOpenProject)
	ON_EVENT(CSPFOpenProject, IDC_gPROJECT, -600 /* Click */, OnClickgPROJECT, VTS_NONE)
	ON_EVENT(CSPFOpenProject, IDC_gPROJECT, -601 /* DblClick */, OnDblClickgPROJECT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSPFOpenProject::OnClickgPROJECT() 
{
	// TODO: Add your control notification handler code here
	m_ProjectName = _T("");
	
	INT row = m_pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		m_pGrid->SetReverse(row-1);
		m_ProjectName = m_pGrid->GetAt(row-1,0);
		m_ProjectDesc = m_pGrid->GetAt(row-1,1);
	}
	

}



VOID CSPFOpenProject::OnDblClickgPROJECT() 
{
	// TODO: Add your control notification handler code here
	OnClickgPROJECT();
	CDialog::OnOK();
}


