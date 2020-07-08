// SPF_NEW_PROJECT.cpp : implementation file
//

#include "stdafx.h"
#include "SPF_NEW_PROJECT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPF_NEW_PROJECT dialog


CSPF_NEW_PROJECT::CSPF_NEW_PROJECT(CWnd* pParent /*=NULL*/)
	: CDialog(CSPF_NEW_PROJECT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPF_NEW_PROJECT)
	m_DESC = _T("");
	m_NAME = _T("");
	//}}AFX_DATA_INIT
}


VOID CSPF_NEW_PROJECT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPF_NEW_PROJECT)
	DDX_Text(pDX, IDC_PROJECT_DESC, m_DESC);
	DDX_Text(pDX, IDC_PROJECT_NAME, m_NAME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPF_NEW_PROJECT, CDialog)
	//{{AFX_MSG_MAP(CSPF_NEW_PROJECT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPF_NEW_PROJECT message handlers

BOOL CSPF_NEW_PROJECT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSPF_NEW_PROJECT::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	CReadCfgFile ReadCfgFile;
	ReadCfgFile.ReadCfgFile(_T(""));

	CESL_DataMgr ProjectDM;
	ProjectDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	m_NAME.TrimLeft();
	m_NAME.TrimRight();

	if(m_NAME != _T(""))
	{
		ProjectDM.StartFrame();

		CString Qry;
		Qry.Format(_T("INSERT INTO SPF_PROJECT_TBL VALUES ( %s.nextval , '%s' , '%s' ); "),ReadCfgFile.m_sSequence ,m_NAME,m_DESC);  

		ProjectDM.AddFrame(Qry);
		ProjectDM.SendFrame(TRUE);
		ProjectDM.EndFrame();
	}
	else
	{
		AfxMessageBox(_T("Project Name is Empty !!!"));	
		
	}

	CDialog::OnOK();
}

VOID CSPF_NEW_PROJECT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
