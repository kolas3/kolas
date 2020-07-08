// MNG_LIB_HOMEPAGE_MGR_01.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIB_HOMEPAGE_MGR_01.h"
#include "..\..\..\ESL\KOLAS3_MAIN\KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOMEPAGE_MGR_01 dialog


CMNG_LIB_HOMEPAGE_MGR_01::CMNG_LIB_HOMEPAGE_MGR_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_HOMEPAGE_MGR_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_LIB_HOMEPAGE_MGR_01::~CMNG_LIB_HOMEPAGE_MGR_01()
{
}
BOOL CMNG_LIB_HOMEPAGE_MGR_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_HOMEPAGE_MGR_01)
	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_HOMEPAGE_MGR_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_HOMEPAGE_MGR_01)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOMEPAGE_MGR_01 message handlers

VOID CMNG_LIB_HOMEPAGE_MGR_01::GoBack()
{
	if (m_bBackEnable)
		m_Browser.GoBack();
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::GoForward()
{
	if (m_bForwardEnable)
		m_Browser.GoForward();
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::Stop()
{
	m_Browser.Stop();
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::Refresh()
{
	//m_Browser.Refresh2(REFRESH_COMPLETELY);
	m_Browser.Refresh();
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::GoHome()
{
	m_Browser.Navigate2(&m_URL,NULL,NULL, NULL, NULL);
}

BEGIN_EVENTSINK_MAP(CMNG_LIB_HOMEPAGE_MGR_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_LIB_HOMEPAGE_MGR_01)
	ON_EVENT(CMNG_LIB_HOMEPAGE_MGR_01, IDC_EXPLORER1, 108 /* ProgressChange */, OnProgressChangeExplorer1, VTS_I4 VTS_I4)
	ON_EVENT(CMNG_LIB_HOMEPAGE_MGR_01, IDC_EXPLORER1, 105 /* CommandStateChange */, OnCommandStateChangeExplorer1, VTS_I4 VTS_BOOL)
	ON_EVENT(CMNG_LIB_HOMEPAGE_MGR_01, IDC_EXPLORER1, 106 /* DownloadBegin */, OnDownloadBeginExplorer1, VTS_NONE)
	ON_EVENT(CMNG_LIB_HOMEPAGE_MGR_01, IDC_EXPLORER1, 104 /* DownloadComplete */, OnDownloadCompleteExplorer1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_LIB_HOMEPAGE_MGR_01::OnProgressChangeExplorer1(long Progress, long ProgressMax) 
{
	if (Progress < 0) Progress = 0;
	if (ProgressMax <= 0) ProgressMax = 10;
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, Progress, ProgressMax);
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::OnCommandStateChangeExplorer1(long Command, BOOL Enable) 
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

VOID CMNG_LIB_HOMEPAGE_MGR_01::OnDownloadBeginExplorer1() 
{
	CString strText = _T(" ");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_SHOW, TRUE);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_TEXT, (WPARAM)&strText);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, 0, 10);
}

VOID CMNG_LIB_HOMEPAGE_MGR_01::OnDownloadCompleteExplorer1() 
{
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, 10, 10);
	Sleep(300);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_SHOW, FALSE);
}

BOOL CMNG_LIB_HOMEPAGE_MGR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strURL;

	CStdioFile file;
	if (!file.Open(_T("..\\cfg\\work_data_search.cfg"), CFile::modeRead | CFile::typeBinary)) return -1;
	
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
	
	CString strLine;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == '.') continue;
		
		strURL = strLine;
		break;

	}

	file.Close();

	if (strURL.IsEmpty()) return -1;

	COleVariant vURL;
	m_URL = strURL;

	m_Browser.Navigate2(&m_URL,NULL,NULL, NULL, NULL);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMNG_LIB_HOMEPAGE_MGR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
