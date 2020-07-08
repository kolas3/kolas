// WorkDataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorkDataSearchDlg.h"
#include "..\..\..\ESL\KOLAS3_MAIN\KOLAS3_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkDataSearchDlg dialog


CWorkDataSearchDlg::CWorkDataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorkDataSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bUpdateEnable = FALSE;
	m_bForwardEnable = FALSE;
	m_bBackEnable = FALSE;
}
CWorkDataSearchDlg::~CWorkDataSearchDlg()
{
	
}

BOOL CWorkDataSearchDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CWorkDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkDataSearchDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CWorkDataSearchDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkDataSearchDlg message handlers
void CWorkDataSearchDlg::GoBack()
{
	if (m_bBackEnable)
		m_Browser.GoBack();
}

void CWorkDataSearchDlg::GoForward()
{
	if (m_bForwardEnable)
		m_Browser.GoForward();
}

void CWorkDataSearchDlg::Stop()
{
	m_Browser.Stop();
}

void CWorkDataSearchDlg::Refresh()
{
	//m_Browser.Refresh2(REFRESH_COMPLETELY);
	m_Browser.Refresh();
}

void CWorkDataSearchDlg::GoHome()
{
	m_Browser.Navigate2(&m_URL,NULL,NULL, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CWorkDataSearchDlg message handlers

BOOL CWorkDataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = ReadURL();
	if (ids >= 0) m_Browser.Navigate2(&m_URL,NULL,NULL, NULL, NULL);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CWorkDataSearchDlg::ReadURL(void)
{	
	CString strURL;
	CString strTemp;
	CString strQuery;
	CString strLibcode;
	CString strUserID;
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);	

	GetLibraryInfo(strLibcode);		
	
	//++2008.10.22 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
					 _T("WHERE GROUP_1='L' AND GROUP_2='기타' AND CLASS_ALIAS='상호대차관리' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"));
// 	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND CLASS_ALIAS = '상호대차관리'"));
	//--2008.10.22 UPDATE BY PWR --}}
	dm.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft();				
	strTemp.TrimRight();	

	if (strTemp.GetLength() == 0 ) 
	{
		AfxMessageBox(_T("상호대차관리용 URL 정보를 찾을 수 없습니다."));
		return -1;
	}
	
	strURL.Format(_T("%s%s"), strTemp, strLibcode);
	//strURL.Format(_T("%s011001"), strTemp);
	m_URL = strURL;
	return 0;
}

void CWorkDataSearchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (GetSafeHwnd() == NULL) return;

	CWnd * pWnd = GetDlgItem(IDC_EXPLORER1);
	if (pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow( -2 , -2 , cx+4 , cy+4 );
}

BEGIN_EVENTSINK_MAP(CWorkDataSearchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CWorkDataSearchDlg)
	ON_EVENT(CWorkDataSearchDlg, IDC_EXPLORER1, 108 /* ProgressChange */, OnProgressChangeExplorer1, VTS_I4 VTS_I4)
	ON_EVENT(CWorkDataSearchDlg, IDC_EXPLORER1, 105 /* CommandStateChange */, OnCommandStateChangeExplorer1, VTS_I4 VTS_BOOL)
	ON_EVENT(CWorkDataSearchDlg, IDC_EXPLORER1, 106 /* DownloadBegin */, OnDownloadBeginExplorer1, VTS_NONE)
	ON_EVENT(CWorkDataSearchDlg, IDC_EXPLORER1, 104 /* DownloadComplete */, OnDownloadCompleteExplorer1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CWorkDataSearchDlg::OnCommandStateChangeExplorer1(long Command, BOOL Enable) 
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

void CWorkDataSearchDlg::OnDownloadBeginExplorer1() 
{
	CString strText = " ";
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_SHOW, TRUE);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_TEXT, (WPARAM)&strText);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, 0, 10);
}

void CWorkDataSearchDlg::OnProgressChangeExplorer1(long Progress, long ProgressMax) 
{
	if (Progress < 0) Progress = 0;
	if (ProgressMax <= 0) ProgressMax = 10;
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, Progress, ProgressMax);
}

void CWorkDataSearchDlg::OnDownloadCompleteExplorer1() 
{
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, 10, 10);
	Sleep(300);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_PROGRESS_SHOW, FALSE);
}


INT CWorkDataSearchDlg:: GetLibraryInfo(CString &lib_Code) 
{

    CESL_DataMgr pDM;
    CString sPath ;	
	pDM.SetCONNECTION_INFO (m_pInfo->CONNECTION_INFO) ;
	
	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString sQuery_libcode ;
	sQuery_libcode = _T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");   
//	CString str_user_id ;
//	str_user_id = GetUserID () ;
//
//    CString sQuery_manage ;
//	sQuery_manage = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '관리구분별도서관설정' ") ;
//    
//	CString Check_YN ;
//    pDM.GetOneValueQuery (sQuery_manage,Check_YN) ; 
//
//	 CString sQuery_libcode ;
//    if(Check_YN == _T("Y"))
//	{	
//		CString sQuery ;
//		sQuery.Format (_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID = '%s' "),str_user_id) ;		
//		CString sManage_Code ;
//		pDM.GetOneValueQuery (sQuery,sManage_Code) ;
//        
//        if(sManage_Code.IsEmpty ())
//		{
//			sQuery_libcode = _T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL") ; 
//		}
//        else
//		{
//             sQuery_libcode.Format (_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"),sManage_Code) ;   
//		}
//	}
//	else
//	{
//        sQuery_libcode = _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL") ;   
//	}
	//--2008.10.15 UPDATE BY PWR --}}
     
    CString tmp_Lib_Code ;
	pDM.GetOneValueQuery (sQuery_libcode,tmp_Lib_Code) ; 

    if(tmp_Lib_Code.IsEmpty ())
	{
		AfxMessageBox (_T("도서관 부호가 설정 되어 있지 않습니다.")) ;
		return -100 ;
	}

	lib_Code = tmp_Lib_Code;
	
	return 0 ;
}

HBRUSH CWorkDataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
