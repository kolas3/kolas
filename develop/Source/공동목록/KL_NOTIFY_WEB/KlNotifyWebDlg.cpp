// KlNotifyWebDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KlNotifyWebDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKlNotifyWebDlg dialog


CKlNotifyWebDlg::CKlNotifyWebDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlNotifyWebDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKlNotifyWebDlg)
	//}}AFX_DATA_INIT
	m_bUpdateEnable=FALSE;
	m_bForwardEnable=FALSE;
	m_bBackEnable=FALSE;
}

CKlNotifyWebDlg::~CKlNotifyWebDlg()
{
}

void CKlNotifyWebDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKlNotifyWebDlg)
	DDX_Control(pDX, IDC_KL_NOTIFY_WEB_Browser, m_Browser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKlNotifyWebDlg, CDialog)
	//{{AFX_MSG_MAP(CKlNotifyWebDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKlNotifyWebDlg message handlers

BOOL CKlNotifyWebDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = ReadURL();
	if(0 > ids) return FALSE;
	
	m_Browser.Navigate2(&m_URL, NULL, NULL, NULL, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKlNotifyWebDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CKlNotifyWebDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (m_Browser.GetSafeHwnd() == NULL) return;
	m_Browser.Invalidate(TRUE);
	m_Browser.MoveWindow(-2,-2,cx+4,cy+4);
}

BEGIN_EVENTSINK_MAP(CKlNotifyWebDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKlNotifyWebDlg)
	ON_EVENT(CKlNotifyWebDlg, IDC_KL_NOTIFY_WEB_Browser, 105 /* CommandStateChange */, OnCommandStateChangeKlNotifyWebBrowser, VTS_I4 VTS_BOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

int CKlNotifyWebDlg::ReadURL()
{
	CString strURL, strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='통합도서서비스' AND VALUE_NAME='공지사항URL' AND MANAGE_CODE=UDF_MANAGE_CODE");
	dm.GetOneValueQuery(strQuery, strURL);
	strURL.TrimLeft();	strURL.TrimRight();	
	if (strURL.IsEmpty()) {
		strURL = _T("http://kolas.nl.go.kr/nltech/plib/notice/noticeList.do");
	}
	m_URL = strURL;

	return 0;
}

void CKlNotifyWebDlg::EditLoginInfo()
{
	ReadURL();
	m_Browser.Navigate2(&m_URL, NULL, NULL, NULL, NULL);
}

void CKlNotifyWebDlg::GoBack()
{
	if(m_bBackEnable) m_Browser.GoBack();
}

void CKlNotifyWebDlg::GoForward()
{
	if(m_bForwardEnable) m_Browser.GoForward();
}

void CKlNotifyWebDlg::Stop()
{
	m_Browser.Stop();
}

void CKlNotifyWebDlg::Refresh()
{
	m_Browser.Refresh();
}

void CKlNotifyWebDlg::GoHome()
{
	m_Browser.Navigate2(&m_URL,NULL,NULL, NULL, NULL);
}

void CKlNotifyWebDlg::OnCommandStateChangeKlNotifyWebBrowser(long Command, BOOL Enable) 
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
