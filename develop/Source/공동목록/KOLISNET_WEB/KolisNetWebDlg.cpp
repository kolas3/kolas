// KolisNetWebDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KolisNetWebDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKolisNetWebDlg dialog


CKolisNetWebDlg::CKolisNetWebDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKolisNetWebDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKolisNetWebDlg)
	//}}AFX_DATA_INIT
	m_bUpdateEnable=FALSE;
	m_bForwardEnable=FALSE;
	m_bBackEnable=FALSE;
}

CKolisNetWebDlg::~CKolisNetWebDlg()
{
}

void CKolisNetWebDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKolisNetWebDlg)
	DDX_Control(pDX, IDC_KOLISNET_WEB_Browser, m_Browser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKolisNetWebDlg, CDialog)
	//{{AFX_MSG_MAP(CKolisNetWebDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKolisNetWebDlg message handlers

BOOL CKolisNetWebDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = ReadURL();
	if(0 > ids) return FALSE;
	
	m_Browser.Navigate2(&m_URL, NULL, NULL, NULL, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKolisNetWebDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CKolisNetWebDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (m_Browser.GetSafeHwnd() == NULL) return;
	m_Browser.Invalidate(TRUE);
	m_Browser.MoveWindow(-2,-2,cx+4,cy+4);
}

BEGIN_EVENTSINK_MAP(CKolisNetWebDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKolisNetWebDlg)
	ON_EVENT(CKolisNetWebDlg, IDC_KOLISNET_WEB_Browser, 105 /* CommandStateChange */, OnCommandStateChangeKolisNetWebBrowser, VTS_I4 VTS_BOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

int CKolisNetWebDlg::ReadURL()
{
	CString strURL, strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLIS-NET' AND CLASS_DESC='국가자료종합목록' AND MANAGE_CODE=UDF_MANAGE_CODE");
	dm.GetOneValueQuery(strQuery, strURL);

	strURL.TrimLeft();	strURL.TrimRight();	
	if (strURL.GetLength() == 0 ) 
	{
		AfxMessageBox(_T("국가자료종합목록 URL 정보를 찾을 수 없습니다."));
		return -1;
	}
	
	m_URL = strURL;
	return 0;
}

void CKolisNetWebDlg::EditLoginInfo()
{
	ReadURL();
	m_Browser.Navigate2(&m_URL, NULL, NULL, NULL, NULL);
}

void CKolisNetWebDlg::GoBack()
{
	if(m_bBackEnable) m_Browser.GoBack();
}

void CKolisNetWebDlg::GoForward()
{
	if(m_bForwardEnable) m_Browser.GoForward();
}

void CKolisNetWebDlg::Stop()
{
	m_Browser.Stop();
}

void CKolisNetWebDlg::Refresh()
{
	m_Browser.Refresh();
}

void CKolisNetWebDlg::GoHome()
{
	m_Browser.Navigate2(&m_URL,NULL,NULL, NULL, NULL);
}

void CKolisNetWebDlg::OnCommandStateChangeKolisNetWebBrowser(long Command, BOOL Enable) 
{
	if (Command == CSC_UPDATECOMMANDS)
	{
		m_bUpdateEnable = Enable;
	}
	else if (Command == CSC_NAVIGATEFORWARD)
	{
		m_bForwardEnable = Enable;
	}
	else if (Command == CSC_NAVIGATEBACK)
	{
		m_bBackEnable = Enable;
	}
}
