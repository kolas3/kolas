// AcqGroupSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcqGroupSearchDlg.h"
#include "../K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupSearchDlg dialog

CAcqGroupSearchDlg::CAcqGroupSearchDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(CAcqGroupSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqGroupSearchDlg)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_nAcqGroupType = ACQ_GROUP_INVALID;
	m_pCM = NULL;
	m_bCreate = FALSE;
}

CAcqGroupSearchDlg::~CAcqGroupSearchDlg()
{
}

VOID CAcqGroupSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqGroupSearchDlg)
	DDX_Radio(pDX, IDC_radACQ_GROUP_MYDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqGroupSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CAcqGroupSearchDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupSearchDlg message handlers

BOOL CAcqGroupSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서그룹검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_수서그룹검색"));

	m_nMyData = 0;
	UpdateData(FALSE);

	GetWindowRect(&m_rcWindow);
	
	CString strData[3][2] = {		
		_T("0"), _T("구입대상"),
		_T("1"), _T("주문"),
		_T("2"), _T("검수완료")
	};
	
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("검수완료"), strData, _T("작업상태"), 3);

	GetWindowRect(m_rcWindow);
	SetAcqCodeComboCtrl();

	ShowCtrlByGroupType();

	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pCM->SetControlMgrData(_T("수입구분"), m_strAcqCode);
	m_pCM->SetControlMgrData(_T("업무구분"), m_strWorkCode);
	m_pCM->SetControlMgrData(_T("작업상태"), m_strWorkStatus);

	CComboBox * pCmb = (CComboBox*) GetDlgItem(IDC_cmbACQ_GROUP_WORK_STATUS);
	pCmb->ShowWindow(SW_HIDE);
	
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_stcGROUP_SEARCH_WORK_STATUS);
	pEdit->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAcqGroupSearchDlg::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CAcqGroupSearchDlg::ShowCtrlByGroupType()
{
	if(m_nAcqGroupType < 0) return ;

	BOOL bPurchaseYN = FALSE;
	if(m_nAcqGroupType == ACQ_PURCHASE_GROUP)
	{
		SetWindowText(_T("수서구입차수 검색"));
		bPurchaseYN = TRUE;
	}
	else if(m_nAcqGroupType == ACQ_OTHER_GROUP)
	{
		SetWindowText(_T("수서그룹 검색"));
		bPurchaseYN = TRUE;
	}
	else if(m_nAcqGroupType == ACQ_DONATE_GROUP)
	{
		SetWindowText(_T("수서기증접수번호 검색"));
		bPurchaseYN = FALSE;
	}
	else if(m_nAcqGroupType == ACQ_OBJ_GROUP)
	{
		SetWindowText(_T("수서대상그룹 검색"));
		bPurchaseYN = FALSE;
	}
	
	INT i = 0;
	INT nCodeResourcesHeight = 45;

	CRect rcWindow, rcCtrl;
	
	CWnd *pWnd = NULL;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	INT nArrBtnResourceID[256];

	nArrCodeResourceID[nCount++] = IDC_cmbACQ_GROUP_WORK_STATUS;
	nArrCodeResourceID[nCount++] = IDC_cmbACQ_GROUP_WORK_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbGROUP_SEARCH_ACQ_CODE;
	nArrCodeResourceID[nCount++] = IDC_stcGROUP_SEARCH_WORK_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcGROUP_SEARCH_WORK_CODE;
	nArrCodeResourceID[nCount++] = IDC_stcGROUP_SEARCH_ACQ_CODE;
	nArrCodeResourceID[nCount++] = IDC_stcGROUP_SEARCH_GROUP1;
	
	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (bPurchaseYN)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	if(bPurchaseYN)
	{
		if(m_nAcqGroupType == ACQ_PURCHASE_GROUP)
		{
			GetDlgItem(IDC_cmbGROUP_SEARCH_ACQ_CODE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_stcGROUP_SEARCH_ACQ_CODE)->ShowWindow(SW_HIDE);
		}
		else if(m_nAcqGroupType == ACQ_OTHER_GROUP)
		{
			GetDlgItem(IDC_cmbACQ_GROUP_WORK_STATUS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_stcGROUP_SEARCH_WORK_STATUS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbACQ_GROUP_WORK_CODE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_stcGROUP_SEARCH_WORK_CODE)->ShowWindow(SW_HIDE);
		}
	}
		
	if (bPurchaseYN)
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
		
	INT nOffSet = 45;

	nCount = 0;
	nArrBtnResourceID[nCount++] = IDC_stcGROUP_SEARCH_GROUP2;
	nArrBtnResourceID[nCount++] = IDOK;
	nArrBtnResourceID[nCount++] = IDCANCEL;
	for(i=0 ; i<nCount ; i++)
	{
		pWnd = GetDlgItem(nArrBtnResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(&rcCtrl);
			ScreenToClient(rcCtrl);

			if (!bPurchaseYN)
			{
				rcCtrl.top -= nOffSet;
				rcCtrl.bottom -= nOffSet;
				pWnd->MoveWindow(rcCtrl);
			}
		}		
	}
}

VOID CAcqGroupSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pCM->GetControlMgrData(_T("작업상태"), m_strWorkStatus);
	m_pCM->GetControlMgrData(_T("업무구분"), m_strWorkCode);
	m_pCM->GetControlMgrData(_T("수입구분"), m_strAcqCode);

	if(m_strAcqYear.IsEmpty()) 
	{
		ESLAfxMessageBox(_T("수입년도 항목을 설정해 주십시요."));
		return ;
	}

	CString strQuery, strOption;
	if(!m_strAcqYear.IsEmpty())
	{
		if(m_nAcqGroupType == ACQ_OBJ_GROUP || m_nAcqGroupType == ACQ_OTHER_GROUP)
			strQuery.Format(_T("CREATE_YEAR='%s'"), m_strAcqYear);
		else if(m_nAcqGroupType == ACQ_PURCHASE_GROUP || m_nAcqGroupType == ACQ_DONATE_GROUP)
			strQuery.Format(_T("ACQ_YEAR='%s'"), m_strAcqYear);
		strQuery += strOption;
	}
	if(m_nMyData == 0)
	{
		if(m_nAcqGroupType == ACQ_OBJ_GROUP || m_nAcqGroupType == ACQ_OTHER_GROUP)
			strOption.Format(_T(" AND GROUP_CREATOR='%s'"), this->m_pInfo->USER_NAME);
		else if(m_nAcqGroupType == ACQ_PURCHASE_GROUP)
			strOption.Format(_T(" AND SEQ_NO_CREATE_WORKER='%s'"), this->m_pInfo->USER_NAME);
		else if(m_nAcqGroupType == ACQ_DONATE_GROUP)
			strOption.Format(_T(" AND RECEIPT_NO_CREATE_WORKER='%s'"), this->m_pInfo->USER_NAME);
		strQuery += strOption;
	}
	if(m_nAcqGroupType == ACQ_PURCHASE_GROUP && !m_strWorkStatus.IsEmpty())
	{
		strOption.Format(_T(" AND WORKING_STATUS='%s'"), m_strWorkStatus);
		strQuery += strOption;
	}
	if(m_nAcqGroupType == ACQ_PURCHASE_GROUP && !m_strWorkCode.IsEmpty())
	{
		strOption.Format(_T(" AND WORK_CODE='%s'"), m_strWorkCode);
		strQuery += strOption;
	}
	if(m_nAcqGroupType == ACQ_OTHER_GROUP && !m_strAcqCode.IsEmpty())
	{
		strOption.Format(_T(" AND ACQ_CODE='%s'"), m_strAcqCode);
		strQuery += strOption;
	}

	m_strWhere = strQuery;

	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

BOOL CAcqGroupSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if(GetDlgItem(IDCANCEL)->GetSafeHwnd() == pMsg->hwnd)
		{
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		else
		{
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CAcqGroupSearchDlg::SetAcqCodeComboCtrl()
{
	CString strData[10][2];

	CString codes[6];
	CString code, desc;
	INT startrow = 0;
	INT k=0;

	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("입수구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("입수구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		startrow++;
		
		if(code == _T("") || code == _T("1") || code == _T("2"))
			continue;
		else
		{
			strData[k][0] = code;
			strData[k][1] = desc;
			k++;
		}
	}
	
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T(""), strData, _T("수입구분"), k);
}

HBRUSH CAcqGroupSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
