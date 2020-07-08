#include "stdafx.h"
#include "UserEditSearchDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUserEditSearchDlg::CUserEditSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserEditSearchDlg::IDD, pParent)
{	
	m_strUserKey = _T("");
}


void CUserEditSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CUserEditSearchDlg)
	//}}AFX_DATA_MAP		
	
}


BEGIN_MESSAGE_MAP(CUserEditSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CUserEditSearchDlg)
	ON_BN_CLICKED(IDC_btnUES_Search, OnbtnUESSearch)
	ON_BN_CLICKED(IDC_btnUES_Close, OnbtnUESClose)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CUserEditSearchDlg::OnbtnUESSearch() 
{
	SetTimer(1, 100, NULL);
}

void CUserEditSearchDlg::OnbtnUESClose() 
{
	OnCancel();
}

BOOL CUserEditSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;
	ids = InitESL_Mgr(_T("SM_USER_EDIT_SEARCH"));
	if(0 > ids) return FALSE;
	
	ids = InitControl();
	if(0 > ids) return FALSE;

	SetTimer(1, 100, NULL);
	
	return TRUE;  
	              
}

BOOL CUserEditSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnUESClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnUES_Search)->m_hWnd == pMsg->hwnd )
			{
				OnbtnUESSearch();
			}
			else if(GetDlgItem(IDC_btnUES_Close)->m_hWnd == pMsg->hwnd )
			{
				OnbtnUESClose();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CUserEditSearchDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SearchStart();
	}
	
	CDialog::OnTimer(nIDEvent);
}

INT CUserEditSearchDlg::InitControl()
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

	SetControlData(_T("CM_USER_EDIT_SEARCH"), _T("조회기간1"), strBefore);
	SetControlData(_T("CM_USER_EDIT_SEARCH"), _T("조회기간2"), strCurrent);

	return 0;
}

INT CUserEditSearchDlg::SearchStart()
{
	CString strBefore, strAfter;
	GetControlData(_T("CM_USER_EDIT_SEARCH"), _T("조회기간1"), strBefore);
	GetControlData(_T("CM_USER_EDIT_SEARCH"), _T("조회기간2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_USER_EDIT_SEARCH"), _T("조회기간1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_USER_EDIT_SEARCH"), _T("조회기간2"), strAfter);
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

	if(strWhere.IsEmpty())
	{
		strWhere.Format(_T("USER_KEY=%s"), m_strUserKey);
	}
	else
	{
		CString strValue;
		strValue.Format(_T(" AND USER_KEY=%s"), m_strUserKey);
		strWhere += strValue;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_USER_EDIT_SEARCH"));
	if(NULL == pDM) return -1;
	pDM->RefreshDataManager(strWhere);
	ControlDisplay(_T("CM_USER_EDIT_SEARCH"), _T("MainGrid"));

	return 0;
}

BEGIN_EVENTSINK_MAP(CUserEditSearchDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(CUserEditSearchDlg)
	ON_EVENT(CUserEditSearchDlg, IDC_gridUES_UserDelList, -604 /* KeyUp */, OnKeyUpgridUESUserDelList, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserEditSearchDlg::OnKeyUpgridUESUserDelList(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}
