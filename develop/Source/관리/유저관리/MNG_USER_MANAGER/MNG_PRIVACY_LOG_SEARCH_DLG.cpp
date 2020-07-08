// MNG_PRIVACY_LOG_SEARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_PRIVACY_LOG_SEARCH_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_PRIVACY_LOG_SEARCH_DLG dialog


CMNG_PRIVACY_LOG_SEARCH_DLG::CMNG_PRIVACY_LOG_SEARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_PRIVACY_LOG_SEARCH_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_PRIVACY_LOG_SEARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

CMNG_PRIVACY_LOG_SEARCH_DLG::~CMNG_PRIVACY_LOG_SEARCH_DLG()
{
}

void CMNG_PRIVACY_LOG_SEARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_PRIVACY_LOG_SEARCH_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_PRIVACY_LOG_SEARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_PRIVACY_LOG_SEARCH_DLG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_PRIVACY_LOG_SEARCH_DLG message handlers

BOOL CMNG_PRIVACY_LOG_SEARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( InitESL_Mgr( _T("SM_MNG_PRIVACY_LOG_SEARCH_DLG") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
		return false;
	}

	if( m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("S")) != 0 )
	{	
		CString strManageCode = _T("");
		GetControlData( _T("CM_MNG_PRIVACY_LOG_SEARCH_DLG"), _T("包府备盒"), strManageCode );
		
		if (strManageCode != m_pInfo->MANAGE_CODE ){
			SetControlData(_T("CM_MNG_PRIVACY_LOG_SEARCH_DLG"),_T("包府备盒"), m_pInfo->MANAGE_CODE );
		}

		CWnd* pWnd = (CWnd*)GetDlgItem(IDC_COMBO_MANAGECODE);
		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd()) pWnd->EnableWindow(FALSE);
	}
	//


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMNG_PRIVACY_LOG_SEARCH_DLG::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMNG_PRIVACY_LOG_SEARCH_DLG::OnOK() 
{
EFS_BEGIN	
	MakeAndSetSearchQuery();
EFS_END
}

void CMNG_PRIVACY_LOG_SEARCH_DLG::OnCancel() 
{
	m_pParent->EndModalState();
	
	CDialog::OnCancel();
}

VOID CMNG_PRIVACY_LOG_SEARCH_DLG::Init() 
{


}


VOID CMNG_PRIVACY_LOG_SEARCH_DLG::MakeAndSetSearchQuery() 
{
	CMNG_PRIVACY_PRINT_LOG_DLG dlg(m_pParent);
	dlg.m_nMode = 1;

	CString strUserID = _T("");
	CString strManageCode = _T("");
	CString strDate_1 = _T("");
	CString strDate_2 = _T("");

	GetDlgItem(IDC_Edit_ID)->GetWindowText(strUserID);
	GetControlData( _T("CM_MNG_PRIVACY_LOG_SEARCH_DLG"), _T("包府备盒"), strManageCode );
	strDate_1 = GetStringDateFromDateTimeCtrl(IDC_DATETIMEPICKER_1);
	strDate_2 = GetStringDateFromDateTimeCtrl(IDC_DATETIMEPICKER_2);

	CString strCondition_1 = _T("");
	CString strCondition_2 = _T("");
	CString strCondition_3 = _T("");
	CString strCondition_4 = _T("");
	CString strTemp;

	if(_T("") != strUserID) 
	{
		strTemp.Format(_T("WORKER_ID = '%s'"), strUserID);
		if(_T("") == strCondition_1) strCondition_1 = strTemp;
		else strCondition_1 = strCondition_1 + _T(" AND ") + strTemp;

		strTemp.Format(_T("PRINT_USER_ID = '%s'"), strUserID);
		if(_T("") == strCondition_2) strCondition_2 = strTemp;
		else strCondition_2 = strCondition_2 + _T(" AND ") + strTemp;
		
		strTemp.Format(_T("WORKER_ID = '%s'"), strUserID);
		if(_T("") == strCondition_3) strCondition_3 = strTemp;
		else strCondition_3 = strCondition_3 + _T(" AND ") + strTemp;

	}
	if(_T("") != strManageCode)
	{
		strTemp.Format(_T("WORKER_ID IN (SELECT ID FROM MN_USER_TBL WHERE MANAGE_CODE = '%s')"), strManageCode);
		if(_T("") == strCondition_1) strCondition_1 = strTemp;
		else strCondition_1 = strCondition_1 + _T(" AND ") + strTemp;
		
		strTemp.Format(_T("MANAGE_CODE = '%s'"), strManageCode);
		if(_T("") == strCondition_2) strCondition_2 = strTemp;
		else strCondition_2 = strCondition_2 + _T(" AND ") + strTemp;

		strTemp.Format(_T("WORKER_ID IN (SELECT ID FROM MN_USER_TBL WHERE MANAGE_CODE = '%s')"), strManageCode);
		if(_T("") == strCondition_3) strCondition_3 = strTemp;
		else strCondition_3 = strCondition_3 + _T(" AND ") + strTemp;
	}
	strTemp.Format(_T("WORK_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), strDate_1, strDate_2);
	if(_T("") == strCondition_1) strCondition_1 = strTemp;
	else strCondition_1 = strCondition_1 + _T(" AND ") + strTemp;
	
	strTemp.Format(_T("PRINT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), strDate_1, strDate_2);
	if(_T("") == strCondition_2) strCondition_2 = strTemp;
	else strCondition_2 = strCondition_2 + _T(" AND ") + strTemp;
	
	strTemp.Format(_T("WORK_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), strDate_1, strDate_2);
	if(_T("") == strCondition_3) strCondition_3 = strTemp;
	else strCondition_3 = strCondition_3 + _T(" AND ") + strTemp;
	
	dlg.m_strCondition_1 = strCondition_1;
	dlg.m_strCondition_2 = strCondition_2;
	dlg.m_strCondition_3 = strCondition_3;


	m_pParent->BeginModalState();
	BeginModalState();
	dlg.DoModal();
	EndModalState();
	m_pParent->BeginModalState();
}


CString CMNG_PRIVACY_LOG_SEARCH_DLG::GetStringDateFromDateTimeCtrl(INT nID) 
{
	CTime time; 
	CString strDate;
	DWORD dwResult = ((CDateTimeCtrl*)GetDlgItem(nID))->GetTime(time);
	strDate = time.Format("%Y/%m/%d");
	return strDate;
}