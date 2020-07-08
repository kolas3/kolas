// SPF_NEW_GROUP.cpp : implementation file
//

#include "stdafx.h"
#include "FlowMaker.h"
#include "SPF_NEW_GROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPF_NEW_GROUP dialog


CSPF_NEW_GROUP::CSPF_NEW_GROUP(CWnd* pParent /*=NULL*/)
	: CDialog(CSPF_NEW_GROUP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPF_NEW_GROUP)
	m_GROUP_DESC = _T("");
	m_GROUP_NAME = _T("");
	m_GROUP_TYPE = FALSE;
	//}}AFX_DATA_INIT
}


VOID CSPF_NEW_GROUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPF_NEW_GROUP)
	DDX_Text(pDX, IDC_GROUP_DESC, m_GROUP_DESC);
	DDX_Text(pDX, IDC_GROUP_NAME, m_GROUP_NAME);
	DDX_Check(pDX, IDC_GROUP_TYPE, m_GROUP_TYPE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPF_NEW_GROUP, CDialog)
	//{{AFX_MSG_MAP(CSPF_NEW_GROUP)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPF_NEW_GROUP message handlers

BOOL CSPF_NEW_GROUP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSPF_NEW_GROUP::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CReadCfgFile ReadCfgFile;
	ReadCfgFile.ReadCfgFile(_T(""));

	CESL_DataMgr GroupDM;
	GroupDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	m_GROUP_NAME.TrimLeft();
	m_GROUP_NAME.TrimRight();

	if(m_GROUP_NAME != _T(""))
	{
		GroupDM.StartFrame();

		CString Qry;
		Qry.Format(_T("INSERT INTO SPF_GROUP_TBL(PK,ALIAS,PROJECT_ALIAS,DESCRIP,GROUP_TYPE) VALUES ( %s.nextval , '%s' , '%s' ,'%s','%s' ); "),ReadCfgFile.m_sSequence ,m_GROUP_NAME,m_PROJECT_NAME,m_GROUP_DESC,_T("T"));  

		GroupDM.AddFrame(Qry);
		GroupDM.SendFrame(TRUE);
		GroupDM.EndFrame();
	}
	else
	{
		AfxMessageBox(_T("GroupDM Name is Empty !!!"));	
		
	}

	CDialog::OnOK();
	
}
