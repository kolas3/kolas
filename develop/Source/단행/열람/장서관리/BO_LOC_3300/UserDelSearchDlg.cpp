// UserDelSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserDelSearchDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDelSearchDlg dialog


CUserDelSearchDlg::CUserDelSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserDelSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserDelSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strCivilNoHideYN = _T("N");
	m_sIsUnityLoanService = _T("N");
}


void CUserDelSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserDelSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserDelSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CUserDelSearchDlg)
	ON_BN_CLICKED(IDC_btnUDS_Search, OnbtnUDSSearch)
	ON_BN_CLICKED(IDC_btnUDS_Close, OnbtnUDSClose)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserDelSearchDlg message handlers

BOOL CUserDelSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;
	ids = InitESL_Mgr(_T("SM_USER_DEL_SEARCH"));
	if(0 > ids) return FALSE;
	
	ids = InitControl();
	if(0 > ids) return FALSE;

	CESL_ControlMgr* pCM = FindCM(_T("CM_USER_DEL_SEARCH"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if(!m_pInfo->m_bUserInfoExportYN)
	{	
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("삭제이력보기");
	}

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserDelSearchDlg::OnbtnUDSSearch() 
{
	SetTimer(1, 100, NULL);
}

void CUserDelSearchDlg::OnbtnUDSClose() 
{
	OnCancel();	
}

INT CUserDelSearchDlg::InitControl()
{
	CString strGetData;
	CTime tGetTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CString strCurrent = strGetData.Left(10);
	CTime tCurrent(_ttoi(strCurrent.Mid(0, 4)), _ttoi(strCurrent.Mid(5, 2)), _ttoi(strCurrent.Mid(8, 2)), 0, 0, 0);

	CTimeSpan tOneYear(31579200);
	CTime tBefore;
	CString strBefore;
	tBefore = tCurrent - tOneYear;
	strBefore.Format(_T("%04d/%02d/%02d"), tBefore.GetYear(), tBefore.GetMonth(), tBefore.GetDay());

	SetControlData(_T("CM_USER_DEL_SEARCH"), _T("조회기간1"), strBefore);
	SetControlData(_T("CM_USER_DEL_SEARCH"), _T("조회기간2"), strCurrent);

	return 0;
}

void CUserDelSearchDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SearchStart();
	}
	
	CDialog::OnTimer(nIDEvent);
}

INT CUserDelSearchDlg::SearchStart()
{
	CString strBefore, strAfter;
	GetControlData(_T("CM_USER_DEL_SEARCH"), _T("조회기간1"), strBefore);
	GetControlData(_T("CM_USER_DEL_SEARCH"), _T("조회기간2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_USER_DEL_SEARCH"), _T("조회기간1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_USER_DEL_SEARCH"), _T("조회기간2"), strAfter);
	}

	CString strWhere;
	if(!strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("WORK_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
	}
	else if(!strBefore.IsEmpty() && strAfter.IsEmpty())
	{
		strWhere.Format(_T("WORK_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
	}
	else if(strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("WORK_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
	}
	else
	{
		strWhere = _T("");
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_USER_DEL_SEARCH"));
	if(NULL == pDM) return -1;
	pDM->RefreshDataManager(strWhere);

	CString strGetData = _T("");
	if(_T("Y") != m_strCivilNoHideYN)
	{
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("대출자번호"), i, strGetData);
			CLocCommonAPI::ChangeSecuriyUserNo(this, strGetData, m_sIsUnityLoanService);
			pDM->SetCellData(_T("대출자번호"), strGetData, i);
		}
	}

	ControlDisplay(_T("CM_USER_DEL_SEARCH"), _T("MainGrid"));

	return 0;
}

BOOL CUserDelSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnUDSClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnUDS_Search)->m_hWnd == pMsg->hwnd )
			{
				OnbtnUDSSearch();
			}
			else if(GetDlgItem(IDC_btnUDS_Close)->m_hWnd == pMsg->hwnd )
			{
				OnbtnUDSClose();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CUserDelSearchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUserDelSearchDlg)
	ON_EVENT(CUserDelSearchDlg, IDC_gridUDS_UserDelList, -604 /* KeyUp */, OnKeyUpgridUDSUserDelList, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserDelSearchDlg::OnKeyUpgridUDSUserDelList(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}
