#include "Stdafx.h"
#include "CertifyMainDlg.h"

#include "UserDuplecateDlg.h"
#include "KL_UserManagerDlg.h"
#include "..\\LOC_CommonAPI\\CfgFileApi.h"
#include "..\\..\\..\\..\\공통\\IndexToolKit\\Index.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CERTIFY_CFG_FILE	_T("..\\cfg\\LoanReturn\\CertifyService.txt")
#define CERTIFY_CFG_SERVICE	_T("인증서비스")

enum CERTIFY_SERVICE_TYPE
{
	CERTIFY_SERVICE_NONE  = 0x0000,
	CERTIFY_SERVICE_MYPIN = 0x0001,
	CERTIFY_SERVICE_PHONE = 0x0010,
};

#define CERTIFY_RESULT_SUCCESS	0
#define CERTIFY_RESULT_FAIL		1




CCertifyMainDlg::CCertifyMainDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCertifyMainDlg::IDD, pParent), m_viewMypin(this), m_viewPhone(this)
{
	m_nUsedFlag  = CERTIFY_SERVICE_NONE;
	m_nViewIndex = 0;
	
	m_strTitle = _T("본인인증");
}

CCertifyMainDlg::~CCertifyMainDlg()
{
}

void CCertifyMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCertifyMainDlg)
	DDX_Control(pDX, IDC_TAB_CERTIFY_SERVICE, m_tabCertifyService);
	DDX_Control(pDX, IDC_EDT_FAIL_MSG, m_staFailMsg);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CCertifyMainDlg, CDialog)
	//{{AFX_MSG_MAP(CCertifyMainDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CERTIFY, OnbtnCertify)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CERTIFY_SERVICE, OnSelChangeService)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CCertifyMainDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbtnCertify();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCertifyMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_nUsedFlag == CERTIFY_SERVICE_NONE)
	{
		
		ASSERT(FALSE);
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	
	InitTabView();
	
	LoadConfigure();
	
	ShowTabView();

	
	SetWindowText(m_strTitle);

	return TRUE; 
}

HBRUSH CCertifyMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nID = pWnd->GetDlgCtrlID();
	switch (nID)
	{
	case IDC_EDT_FAIL_MSG:
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	case IDC_STA_SUCCESS_MSG:
		{
			pDC->SetTextColor(RGB(0,0,255));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	}

	return hbr;
}

VOID CCertifyMainDlg::OnTimer(UINT nIDEvent)
{	
	switch (nIDEvent)
	{
	case CERTIFY_RESULT_SUCCESS:
		{
			KillTimer(CERTIFY_RESULT_SUCCESS);
			OnOK();
		}
		break;
	case CERTIFY_RESULT_FAIL:
		{
			KillTimer(CERTIFY_RESULT_FAIL);
			OnCancel();
		}
		break;
	}	

	CDialog::OnTimer(nIDEvent);
}

VOID CCertifyMainDlg::OnbtnCertify()
{
	
	SaveConfigure();

	
	CCertifyView* pView = (CCertifyView*)GetTabView(m_nViewIndex);
	ASSERT(pView);

	CString strMsg;
	if (!pView->Certify(strMsg))
	{
		
		m_staFailMsg.SetWindowText(strMsg);
		return;
	}

	
	if (GetUserMode() == 2)
	{
		SetTimer(CERTIFY_RESULT_SUCCESS, 1500, NULL);
		return;
	}
	
	
	if (IsDuplicateUser())
	{
		SetCertifyCI(_T(""));
		SetCertifyFailCode(_T(""));
		SetCertifyDate(_T(""));
		SetCertifyWorker(_T(""));

		
		SetTimer(CERTIFY_RESULT_FAIL, 100, NULL);
		return;
	}

	
	if (m_pInfo->m_bKLUse)
	{
		SetKLMemberUserNo(_T(""));
		SetKLMemberSearch(FALSE);

		CString strUserNo;
		CKL_UserManagerDlg dlg(this);
		if (dlg.GetKlUserSearch(GetCertifyCI(), _T("000000-0000000"), strUserNo) == 0)
		{
			
			SetKLMemberUserNo(strUserNo);
			SetKLMemberSearch(TRUE);
		}
		else
		{
			SetKLMemberSearch(FALSE);
		}
	}

	GetDlgItem(IDC_STA_SUCCESS_MSG)->ShowWindow(SW_SHOW);

	
	if (GetUserMode() == 1)
	{
		CESL_DataMgr TempDM;
		TempDM.SetCONNECTION_INFO(_T(""));

		TempDM.StartFrame();
		TempDM.InitDBFieldData();
		TempDM.AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), GetCertifyCI());
			
		CString strDateFormat;
		strDateFormat.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), GetCertifyDate());
		TempDM.AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), strDateFormat);

		SetCertifyFailCode(_T(""));
		TempDM.AddDBFieldData(_T("IPIN_RESULT"), _T("STRING"), _T(""));
		TempDM.AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_pInfo->USER_ID);
		TempDM.AddDBFieldData(_T("NAME"), _T("STRING"), GetUserName());
		TempDM.AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), GetCertifyGender());

		CIndex index;
		index.MakeIndex(GetUserName());
		TempDM.AddDBFieldData(_T("IDX_USER_NAME"), _T("STRING"), GetUserName());

		if (!GetBirthDay().IsEmpty())
		{
			TempDM.AddDBFieldData(_T("BIRTHDAY"), _T("STRING"), GetBirthDay());
		}

		
		if (pView == &m_viewPhone)
		{
			TempDM.AddDBFieldData(_T("HANDPHONE"), _T("STRING"), GetHandPhone());
		}
			
		TempDM.AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("본인인증(이용자수정)"), __WFILE__, __LINE__));
		TempDM.MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), GetUserKey());

		if (0 > TempDM.SendFrame())
		{
			m_staFailMsg.SetWindowText(_T("본인인증 정보 저장 실패"));
			return;
		}
		TempDM.EndFrame();
	}

	SetTimer(CERTIFY_RESULT_SUCCESS, 1500, NULL);
}

VOID CCertifyMainDlg::OnSelChangeService(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nViewIndex = m_tabCertifyService.GetCurSel();
	
	ShowTabView();
	
	*pResult = 0;
}

VOID CCertifyMainDlg::SetUsedMypinService(BOOL bUsed)
{
	if (bUsed)
	{
		if (!IsUsedService(CERTIFY_SERVICE_MYPIN))
			m_nUsedFlag += CERTIFY_SERVICE_MYPIN;
	}
	else
	{
		if (IsUsedService(CERTIFY_SERVICE_MYPIN))
			m_nUsedFlag -= CERTIFY_SERVICE_MYPIN;
	}	
}

VOID CCertifyMainDlg::SetUsedPhoneService(BOOL bUsed)
{
	if (bUsed)
	{
		if (!IsUsedService(CERTIFY_SERVICE_PHONE))
			m_nUsedFlag += CERTIFY_SERVICE_PHONE;
	}
	else
	{
		if (IsUsedService(CERTIFY_SERVICE_PHONE))
			m_nUsedFlag -= CERTIFY_SERVICE_PHONE;
	}
}

VOID CCertifyMainDlg::LoadConfigure()
{
	
	FILE* fp = NULL;
	if ((fp = _tfopen(CERTIFY_CFG_FILE, _T("r"))) == NULL)
	{
		return;
	}
	fclose(fp);

	CCfgFileApi CfgApi;
	CfgApi.SetFile(CERTIFY_CFG_FILE);
	
	CString strValue;
	CfgApi.GetData(CERTIFY_CFG_SERVICE, strValue);

	if (!strValue.IsEmpty())
		m_nViewIndex = _ttoi(strValue);

	if (m_tabCertifyService.GetSafeHwnd())
		m_tabCertifyService.SetCurSel(m_nViewIndex);
}

VOID CCertifyMainDlg::SaveConfigure()
{
	CCfgFileApi CfgApi;
	CfgApi.SetFile(CERTIFY_CFG_FILE);

	CString strValue;
	strValue.Format(_T("%s|%d"), CERTIFY_CFG_SERVICE, m_nViewIndex);

	CStringList List;
	List.AddTail(strValue);

	CfgApi.WriteFile(List);	
}

VOID CCertifyMainDlg::InitTabView()
{
	CRect rc;
	GetDlgItem(IDC_TAB_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);
	
	
	if (IsUsedService(CERTIFY_SERVICE_MYPIN))
	{
		m_viewMypin.Create(CCertifyMypinView::IDD, this);
		m_viewMypin.MoveWindow(rc);
		m_viewMypin.ShowWindow(SW_HIDE);
		
		m_tabCertifyService.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("  마이핀 인증  "), -1, (LPARAM)&m_viewMypin);
	}
	
	if (IsUsedService(CERTIFY_SERVICE_PHONE))
	{
		m_viewPhone.Create(CCertifyPhoneView::IDD, this);
		m_viewPhone.MoveWindow(rc);
		m_viewPhone.ShowWindow(SW_HIDE);
		
		m_tabCertifyService.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("  휴대폰 인증  "), -1, (LPARAM)&m_viewPhone);
	}
}

VOID CCertifyMainDlg::ShowTabView()
{
	
	if (IsUsedService(CERTIFY_SERVICE_MYPIN))	m_viewMypin.ShowWindow(SW_HIDE);
	if (IsUsedService(CERTIFY_SERVICE_PHONE))	m_viewPhone.ShowWindow(SW_HIDE);

	
	INT nCount = m_tabCertifyService.GetItemCount();
	if (m_nViewIndex >= nCount)
	{
		m_nViewIndex = nCount-1;
	}
	if (m_nViewIndex < 0)
	{
		m_nViewIndex = 0;
	}

	
	GetTabView(m_nViewIndex)->ShowWindow(SW_SHOW);

	
	m_staFailMsg.SetWindowText(_T(""));
}

CWnd* CCertifyMainDlg::GetTabView(INT nIdx)
{
	TCITEM tcItem;
	tcItem.mask = TCIF_PARAM;

	m_tabCertifyService.GetItem(nIdx, &tcItem);

	CWnd* pWnd = (CWnd*)tcItem.lParam;
	ASSERT(pWnd);

	return pWnd;
}

BOOL CCertifyMainDlg::IsDuplicateUser()
{
	CString strCertifyCI = GetCertifyCI();
	if (strCertifyCI.IsEmpty())
	{
		
		return FALSE;
	}

	CString strWhere; 
	if (GetUserMode() == 0)
	{
		strWhere.Format(_T("IPIN_HASH='%s'"), strCertifyCI);		
	}
	else
	{
		CString strUserKey = GetUserKey();
		strWhere.Format(_T("(IPIN_HASH='%s' AND REC_KEY<>%s)"), strCertifyCI, strUserKey);
	}

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE %s"), strWhere);

	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(_T(""));

	CString strValue;
	int ids = TempDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE; 
	if(0 < _ttoi(strValue))
	{
		CUserDuplecateDlg dlg(this);
		dlg.m_nMode = GetUserMode();
		dlg.m_strUserKey = GetUserKey();
		dlg.m_strCertifyCode = strCertifyCI;

		if (IDOK != dlg.DoModal())
		{
			return TRUE;
		}
	}
	
	return FALSE;
}