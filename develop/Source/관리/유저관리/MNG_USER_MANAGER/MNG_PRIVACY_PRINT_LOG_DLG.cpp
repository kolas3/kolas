// MNG_PRIVACY_PRINT_LOG_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_PRIVACY_PRINT_LOG_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_PRIVACY_PRINT_LOG_DLG dialog


CMNG_PRIVACY_PRINT_LOG_DLG::CMNG_PRIVACY_PRINT_LOG_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_PRIVACY_PRINT_LOG_DLG::IDD, pParent)
{
	m_pParent = pParent;
	m_strUserKey = _T("");

	m_nMode = 0;
	
	m_whBrush.CreateSolidBrush(RGB(255, 255, 255));

	m_strCondition_1 = _T("");
	m_strCondition_2 = _T("");
	m_strCondition_3 = _T("");
	m_strCondition_4 = _T("");
}

CMNG_PRIVACY_PRINT_LOG_DLG::~CMNG_PRIVACY_PRINT_LOG_DLG()
{
	m_whBrush.DeleteObject();
}

void CMNG_PRIVACY_PRINT_LOG_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_PRIVACY_PRINT_LOG_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_PRIVACY_PRINT_LOG_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_PRIVACY_PRINT_LOG_DLG)
	ON_NOTIFY(TCN_SELCHANGE, IDC_Tab_Grid, OnSelchangeTabGrid)
	ON_BN_CLICKED(IDC_Button_LogDelete, OnButtonLogDelete)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_PRIVACY_PRINT_LOG_DLG message handlers

BOOL CMNG_PRIVACY_PRINT_LOG_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(0 > InitESL_Mgr(_T("SM_MNG_PRIVACY_PRINT_LOG_DLG")))
	{
		AfxMessageBox(_T("InitESL Error!!!"));
		return FALSE;
	}

	if(0 == m_nMode && _T("") == m_strUserKey)
	{
		AfxMessageBox(_T("데이터가 잘못되었습니다."));
		return FALSE;
	}

	GetWindowRect(&m_rcInitial);
	((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->DeleteAllItems();
	((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->InsertItem(0, _T("개인정보 처리내역"));
	((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->InsertItem(1, _T("개인정보 출력내역"));
	((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->InsertItem(2, _T("개인정보 처리권한 수정내역"));

	Refresh();

	EnableThemeDialogTexture(GetSafeHwnd()); 


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_PRIVACY_PRINT_LOG_DLG::Refresh() 
{	
EFS_BEGIN
	CString strOption;
	
	CESL_DataMgr *pAccessDM = FindDM(_T("DM_USER_PRIVACY_ACCESS_INFO"));
	if(0 == m_nMode) strOption.Format(_T("WORKER_ID = (SELECT ID FROM MN_USER_TBL WHERE REC_KEY = %s AND ROWNUM = 1)"), m_strUserKey);
	else strOption = m_strCondition_1;
	pAccessDM->RefreshDataManager(strOption);
	
	CESL_DataMgr *pPrintDM = FindDM(_T("DM_MNG_PRIVACY_PRINT_LOG_DLG"));
	if(0 == m_nMode) strOption.Format(_T("PRINT_USER_KEY = %s"), m_strUserKey);
	else strOption = m_strCondition_2;
	pPrintDM->RefreshDataManager(strOption);

	CESL_DataMgr *pMnUserAuthDM = FindDM(_T("DM_MNG_USER_MANAGER_AUTH_LOG"));
	if(0 == m_nMode) strOption.Format(_T("USER_KEY = %s"), m_strUserKey);
	else strOption = m_strCondition_3;
	pMnUserAuthDM->RefreshDataManager(strOption);
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_PRIVACY_PRINT_LOG_DLG"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("AccessGrid") );
	if(1 == m_pInfo->m_nPrivacyPrintLevel)
	{	
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}	
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("사용자별 개인정보 처리로그 조회");
	}
	
	pCM->AllControlDisplay();
	
	ShowLogGridInfo(0);
EFS_END
}

void CMNG_PRIVACY_PRINT_LOG_DLG::OnSelchangeTabGrid(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN
	INT ids = 0;
	ids = ((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->GetCurSel();
	ShowLogGridInfo(ids);
EFS_END
}

void CMNG_PRIVACY_PRINT_LOG_DLG::ShowLogGridInfo(INT nMode) 
{
EFS_BEGIN
	if(0 == nMode)
	{
		GetDlgItem(IDC_Grid_PrivacyAccessLog)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Grid_PrivacyPrintLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyAssignLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyAssignGiveLog)->ShowWindow(SW_HIDE);
	}
	else if(1 == nMode)
	{
		GetDlgItem(IDC_Grid_PrivacyAccessLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyPrintLog)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Grid_PrivacyAssignLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyAssignGiveLog)->ShowWindow(SW_HIDE);
	}
	else if(2 == nMode)
	{
		GetDlgItem(IDC_Grid_PrivacyAccessLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyPrintLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyAssignLog)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Grid_PrivacyAssignGiveLog)->ShowWindow(SW_HIDE);
	}
	else if(3 == nMode)
	{
		GetDlgItem(IDC_Grid_PrivacyAccessLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyPrintLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyAssignLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Grid_PrivacyAssignGiveLog)->ShowWindow(SW_SHOW);
	}
EFS_END
}

void CMNG_PRIVACY_PRINT_LOG_DLG::OnButtonLogDelete() 
{
EFS_BEGIN
	
	INT ids = 0;
	ids = ((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->GetCurSel();

	CString strMsg;
	CString strLogName;
	if(0 == ids) strLogName = _T("개인정보 처리내역");
	else if(1 == ids) strLogName = _T("개인정보 출력내역");
	else if(2 == ids) strLogName = _T("개인정보 처리권한 수정내역");
	strMsg.Format(_T("최근 6개월의 로그를 제외한 모든 도서관의 [%s]을 삭제합니다.\r\n삭제된 로그는 복구가 불가능합니다.\r\n정말 삭제하시겠습니까?"), strLogName);

	if(IDYES == MessageBox(strMsg, _T("개인정보 처리내역"), MB_YESNO|MB_ICONQUESTION))
	{
		DeleteLog(6, ids);
		AfxMessageBox(_T("삭제가 완료되었습니다."));
		Refresh();
		((CTabCtrl*)GetDlgItem(IDC_Tab_Grid))->SetCurSel(ids);	
		ShowLogGridInfo(ids);	
	}
EFS_END
}

VOID CMNG_PRIVACY_PRINT_LOG_DLG::DeleteLog(INT nMonthAgo, INT nType) 
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pParent->m_pInfo->CONNECTION_INFO);

	CString strQuery;
	if(0 == nType)
	{
		dm.StartFrame();
		
		strQuery.Format(_T("DELETE CO_LOAN_USER_EDIT_TBL WHERE (TRUNC(SYSDATE, 'DD') - TRUNC(WORK_DATE, 'DD')) > %d ;"), nMonthAgo*30);
		dm.AddFrame(strQuery);

		strQuery.Format(_T("DELETE CO_LOAN_USER_DEL_TBL WHERE (TRUNC(SYSDATE, 'DD') - TRUNC(WORK_DATE, 'DD')) > %d;"), nMonthAgo*30);
		dm.AddFrame(strQuery);

		strQuery.Format(_T("DELETE CO_LOAN_USER_READ_TBL WHERE (TRUNC(SYSDATE, 'DD') - TRUNC(WORK_DATE, 'DD')) > %d;"), nMonthAgo*30);
		dm.AddFrame(strQuery);

		dm.SendFrame();
		dm.EndFrame();
	}
	else if(1 == nType)
	{
		dm.StartFrame();
		strQuery.Format(_T("DELETE MN_PRIVACY_PRINT_LOG_TBL WHERE (TRUNC(SYSDATE, 'DD') - TRUNC(PRINT_DATE, 'DD')) > %d;"), nMonthAgo*30);
		dm.AddFrame(strQuery);
		strQuery.Format(_T("DELETE MN_USER_PRINT_LOG_TBL WHERE (TRUNC(SYSDATE, 'DD') - TRUNC(WORK_DATE, 'DD')) > %d;"), nMonthAgo*30);
		dm.AddFrame(strQuery);	
		dm.SendFrame();
		dm.EndFrame();
	}
	else if(2 == nType)
	{
		dm.StartFrame();
		strQuery.Format(_T("DELETE MN_USER_AUTH_LOG_TBL WHERE (TRUNC(SYSDATE, 'DD') - TRUNC(WORK_DATE, 'DD')) > %d;"), nMonthAgo*30);
		dm.AddFrame(strQuery);			
		dm.SendFrame();
		dm.EndFrame();
	}
}

void CMNG_PRIVACY_PRINT_LOG_DLG::OnOK() 
{	
	CDialog::OnOK();
}

HBRUSH CMNG_PRIVACY_PRINT_LOG_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0, 0, 0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(255, 255, 255));		
		
		return m_whBrush;		
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CMNG_PRIVACY_PRINT_LOG_DLG::OnSize(UINT nType, int cx, int cy) 
{

	CESL_Grid* pGrid_Access = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("AccessGrid") );
	if(pGrid_Access->GetSafeHwnd() == NULL) return;

	CESL_Grid* pGrid_print = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("PrintGrid") );
	if(pGrid_print->GetSafeHwnd() == NULL) return;

	CESL_Grid* pGrid_Assign = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("AssignGrid") );
	if(pGrid_Assign->GetSafeHwnd() == NULL) return;
	
	CWnd* pDelButton = GetDlgItem(IDC_Button_LogDelete);
	if( pDelButton->GetSafeHwnd() == NULL ) return;

	CWnd* pOkbutton = GetDlgItem(IDOK);
	if( pOkbutton->GetSafeHwnd() == NULL ) return;

	CWnd* pTab = ((CTabCtrl*)GetDlgItem(IDC_Tab_Grid));
	if( pTab->GetSafeHwnd() == NULL ) return;

	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	CRect pTabrect,btnPos;
	
	pDelButton->GetWindowRect(btnPos);
	
	pTab->MoveWindow( 9 , 7,  cx-20 , 20  );


	pGrid_Access = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("AccessGrid") );
	pGrid_Access->MoveWindow( 9 , 27 , cx-20, cy-70 );
	pGrid_print = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("PrintGrid") );
	pGrid_print->MoveWindow( 9 , 27 , cx-20, cy-70 );
	pGrid_Assign = (CESL_Grid*)FindControl( _T("CM_MNG_PRIVACY_PRINT_LOG_DLG") , _T("AssignGrid") );
	pGrid_Assign->MoveWindow( 9 , 27 , cx-20, cy-70  );

	pOkbutton->MoveWindow( cx -100 ,cy -29, btnPos.Width(), btnPos.Height());
	pDelButton->MoveWindow( cx -btnPos.Width() -108 ,cy -29, btnPos.Width(), btnPos.Height());

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
