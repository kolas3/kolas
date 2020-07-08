// DivisionNo_Status_Change_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Status_Change_Dlg.h"
#include "GroupMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Status_Change_Dlg dialog


CDivisionNo_Status_Change_Dlg::CDivisionNo_Status_Change_Dlg(CESL_Mgr * pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Status_Change_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Status_Change_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pArrSelIdx = NULL;
}

CDivisionNo_Status_Change_Dlg::~CDivisionNo_Status_Change_Dlg()
{
}

VOID CDivisionNo_Status_Change_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Status_Change_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Status_Change_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Status_Change_Dlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Status_Change_Dlg message handlers

BOOL CDivisionNo_Status_Change_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_차수상태변경")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_차수상태변경"));	

	if(!Init())
	{
		ESLAfxMessageBox(_T("차수상태변경 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDivisionNo_Status_Change_Dlg::Init()
{
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), m_strWorkCode, m_strWorkCode);
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pCM->SetControlMgrData(_T("업무구분"), m_strWorkCode);
	m_pCM->SetControlMgrData(_T("이전자료상태"), m_strWorkStatus);
	m_pCM->SetControlMgrData(_T("차수"), m_strDivNo);
	CMulitComboBoxManager mgr(this);
	CStringArray arrData;	arrData.RemoveAll();
	if(m_strWorkStatus == _T("구입대상"))
	{
		arrData.Add(_T("주문"));
		arrData.Add(_T("검수완료"));
	}
	else if(m_strWorkStatus == _T("주문"))
	{
		arrData.Add(_T("구입대상"));
		arrData.Add(_T("검수완료"));
	}
	else if(m_strWorkStatus == _T("검수완료"))
	{
		arrData.Add(_T("구입대상"));
		arrData.Add(_T("주문"));
	}
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, arrData.GetAt(0), &arrData, _T("변경자료상태"));

	return TRUE;
}

VOID CDivisionNo_Status_Change_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strChangeStatus, strWorkCode;
	m_pCM->GetControlMgrData(_T("변경자료상태"), strChangeStatus);
	if(strChangeStatus == _T("구입대상")) strChangeStatus = _T("0");
	else if(strChangeStatus == _T("주문")) strChangeStatus = _T("1");
	else if(strChangeStatus == _T("검수완료")) strChangeStatus = _T("2");
	m_pInfo->pCodeMgr->ConvertDescToCode(_T("업무구분"), m_strWorkCode, strWorkCode);
	CGroupMgr mgr(this);
	mgr.InitTempData();	
	mgr.InitTempPtr();

	mgr.SetTempData(_T("수입년도"), m_strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("업무구분"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("선정차수"), m_strDivNo.GetBuffer(0));
	mgr.SetTempData(_T("변경상태"), strChangeStatus.GetBuffer(0));
	mgr.SetTempPtr(_T("SELECT_INDEX"), (DWORD*)m_pArrSelIdx);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = mgr.SPFExecutePath(_T("차수상태변경"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 차수의 상태를 변경할 수 없습니다."));
		return;
	}
	else if(ids == 1000)
	{
		ESLAfxMessageBox(_T("선정된 차수에 포함된 자료가 존재합니다. 재 검색하여 확인해 주십시요."));
	}
	CDialog::OnOK();
}

HBRUSH CDivisionNo_Status_Change_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
