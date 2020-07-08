// SPF_NEW_PATH.cpp : implementation file
//

#include "stdafx.h"
#include "flowmaker.h"
#include "SPF_NEW_PATH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPF_NEW_PATH dialog


CSPF_NEW_PATH::CSPF_NEW_PATH(CWnd* pParent /*=NULL*/)
	: CDialog(CSPF_NEW_PATH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPF_NEW_PATH)
	m_PATH_DESC = _T("");
	m_PATH_NAME = _T("");
	//}}AFX_DATA_INIT

	Modify_Flag = 0;
}


VOID CSPF_NEW_PATH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPF_NEW_PATH)
	DDX_Text(pDX, IDC_PATH_DESC, m_PATH_DESC);
	DDX_Text(pDX, IDC_PATH_NAME, m_PATH_NAME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPF_NEW_PATH, CDialog)
	//{{AFX_MSG_MAP(CSPF_NEW_PATH)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPF_NEW_PATH message handlers

BOOL CSPF_NEW_PATH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( Modify_Flag == 1)
	{

		UpdateData(false);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSPF_NEW_PATH::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CReadCfgFile ReadCfgFile;
	ReadCfgFile.ReadCfgFile(_T(""));

	CESL_DataMgr PathDM;
	PathDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	m_PATH_NAME.TrimLeft();
	m_PATH_NAME.TrimRight();

	if(Modify_Flag == 0)
	{

		if(m_PATH_NAME != _T(""))
		{
			PathDM.StartFrame();

			CString Qry;
			Qry.Format(_T("INSERT INTO SPF_PATH_TBL(PK,ALIAS, PROJECT_ALIAS,GROUP_ALIAS ,DESCRIP,ISACTIVATED) VALUES ( %s.nextval , '%s' , '%s' ,'%s','%s','T' ); "),ReadCfgFile.m_sSequence ,m_PATH_NAME,m_PROJECT_NAME,m_GROUP_NAME,m_PATH_DESC);  

			PathDM.AddFrame(Qry);
			PathDM.SendFrame(TRUE);
			PathDM.EndFrame();
		}
		else
		{
			AfxMessageBox(_T("Path Name is Empty !!!"));	
			
		}
	}
	else
	{
		if(m_PATH_NAME != _T(""))
		{
			PathDM.StartFrame();

			CString Qry,Qry1;
			Qry.Format(_T("UPDATE SPF_PATH_TBL SET ALIAS='%s', DESCRIP='%s' WHERE PROJECT_ALIAS = '%s' AND GROUP_ALIAS = '%s' AND ALIAS ='%s' ; "),m_PATH_NAME,m_PATH_DESC,m_PROJECT_NAME,m_GROUP_NAME,m_MODIFY_ALIAS);  
			PathDM.AddFrame(Qry);
			Qry1.Format(_T("UPDATE SPF_PATH_RESOURCE_TBL SET PATH_ALIAS='%s' WHERE PROJECT_ALIAS = '%s' AND GROUP_ALIAS = '%s' AND PATH_ALIAS ='%s' ; "),m_PATH_NAME,m_PROJECT_NAME,m_GROUP_NAME,m_MODIFY_ALIAS);
			PathDM.AddFrame(Qry1);
			PathDM.SendFrame(TRUE);
			PathDM.EndFrame();
		}
		else
		{
			AfxMessageBox(_T("Path Name is Empty !!!"));	
		}


	}

	CDialog::OnOK();
	
}
