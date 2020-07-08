// OtherGroupNo_List_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OtherGroupNo_List_Dlg.h"
#include "OtherGroupNo_DataView_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_List_Dlg dialog


COtherGroupNo_List_Dlg::COtherGroupNo_List_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COtherGroupNo_List_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherGroupNo_List_Dlg)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pGroupMgr = NULL;
	m_pGroupCollection = NULL;
	m_strSelInfo.Empty();
	m_strAcqCode.Empty();
	m_strAcqYear.Empty();
}

COtherGroupNo_List_Dlg::~COtherGroupNo_List_Dlg()
{
	if(m_pGroupMgr)
	{
		delete m_pGroupMgr;
		m_pGroupMgr = NULL;
	}
}

VOID COtherGroupNo_List_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherGroupNo_List_Dlg)
	DDX_Radio(pDX, IDC_radOTHER_GROUP_NOVIEW_ALLDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherGroupNo_List_Dlg, CDialog)
	//{{AFX_MSG_MAP(COtherGroupNo_List_Dlg)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NOVIEW_SEARCH, OnbtnOTHERGROUPNOVIEWSEARCH)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NOVIEW_APPLY, OnbtnOTHERGROUPNOVIEWAPPLY)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_INFO, OnbtnOTHERGROUPNOINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_List_Dlg message handlers

BOOL COtherGroupNo_List_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서그룹번호보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_수서그룹번호보기"));
	m_pDM = FindDM(_T("DM_수서그룹번호관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	
	m_pGroupMgr = new CGroupMgr(this);
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("그룹보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COtherGroupNo_List_Dlg::Init()
{
	m_nMyData = 0;
	UpdateData(FALSE);	
	
	if(m_strAcqCode.IsEmpty()) 	return FALSE;
	m_pCM->SetControlMgrData(_T("수입구분"), m_strAcqCode);
	GetDlgItem(IDC_cmbOTHER_GROUP_NO_ACQ_CODE)->EnableWindow(FALSE);

	if(!m_strAcqYear.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
		INT ids = MakeDM();
		if(ids < 0) return FALSE;
	}

	return TRUE;
}

INT COtherGroupNo_List_Dlg::MakeDM()
{
	CString strCreator;
	UpdateData(TRUE);
	if(m_nMyData == 1) strCreator = m_pInfo->USER_NAME;
	
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	if(m_strAcqYear.IsEmpty())
		m_strAcqYear = m_pGroupMgr->GetTodayYear();
	CString strQuery;
	if(strCreator.IsEmpty()) strQuery.Format(_T("ACQ_CODE='%s' AND CREATE_YEAR = '%s'"), m_strAcqCode, m_strAcqYear);
	else strQuery.Format(_T("ACQ_CODE='%s' AND CREATE_YEAR = '%s' AND GROUP_CREATOR = '%s'"), m_strAcqCode, m_strAcqYear, strCreator);
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;
	
	m_pGroupMgr->InitTempData();	m_pGroupMgr->InitTempPtr();
	m_pGroupMgr->SetTempData(_T("조건"), strQuery.GetBuffer(0));
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	m_pGroupMgr->SetTempData(_T("SPECIES_TYPE"), (INT)BO_REG_INVALID);
	m_pGroupMgr->SetTempData(_T("ACQ_YEAR"), m_strAcqYear.GetBuffer(0));
	m_pGroupMgr->SetTempData(_T("ACQ_CODE"), m_strAcqCode.GetBuffer(0));
	ids = m_pGroupMgr->SPFExecutePath(_T("수서그룹번호책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return -1;
	}
	
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pGrid->Display();
	return 0;
}

VOID COtherGroupNo_List_Dlg::OnbtnOTHERGROUPNOVIEWSEARCH() 
{
	// TODO: Add your control notification handler code here
	INT ids = MakeDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("수서 그룹정보를 검색할 수 없습니다."));
		return ;
	}
	m_pGrid->Display();	
}

VOID COtherGroupNo_List_Dlg::OnbtnOTHERGROUPNOVIEWAPPLY() 
{
	// TODO: Add your control notification handler code here
	m_pGroupMgr->InitTempData();	m_pGroupMgr->InitTempPtr();
	m_pGroupMgr->SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pGroupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	m_pGroupMgr->SetTempData(_T("InfoAlias"), _T("그룹번호"));
	m_pGroupMgr->SetTempData(_T("KeyAlias"), _T(""));
	m_pGroupMgr->SetTempData(_T("YearAlias"), _T("수입년도"));
	INT ids = m_pGroupMgr->SPFExecutePath(_T("선정자료추출"));
	if(ids < 0)
	{
		//에러처리
		return;
	}
	m_pGroupMgr->GetTempData(_T("SelInfo"), m_strSelInfo);
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	
	CDialog::OnOK();		
}

VOID COtherGroupNo_List_Dlg::OnbtnOTHERGROUPNOINFO() 
{
	// TODO: Add your control notification handler code here
	BOOL bError = FALSE;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) bError = TRUE;
	INT i = m_pGrid->SelectGetHeadPosition();
	if(i < 0) bError = TRUE;
	
	if(bError)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	COtherGroupNo_DataView_Dlg dlg(this);
	dlg.SetAcqCode(m_pDM->GetCellData(_T("수입구분"), i));
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();		
}

BEGIN_EVENTSINK_MAP(COtherGroupNo_List_Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(COtherGroupNo_List_Dlg)
	ON_EVENT(COtherGroupNo_List_Dlg, IDC_gridOTHER_GROUP_NOVIEW_LIST, -601 /* DblClick */, OnDblClickgridOTHERGROUPNOVIEWLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID COtherGroupNo_List_Dlg::OnDblClickgridOTHERGROUPNOVIEWLIST() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;
	
	if(m_pDM->GetRowCount() == 0) return ;
	
	m_pGrid->SetAt(m_pGrid->GetRow()-1, 0, _T("V"));
	OnbtnOTHERGROUPNOVIEWAPPLY();		
}

BOOL COtherGroupNo_List_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_btnOTHER_GROUP_NO_INFO)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDC_btnOTHER_GROUP_NO_INFO)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnOTHER_GROUP_NO_INFO, BN_CLICKED);
		}
		else if(GetDlgItem(IDC_btnOTHER_GROUP_NOVIEW_APPLY)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDC_btnOTHER_GROUP_NOVIEW_APPLY)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnOTHER_GROUP_NOVIEW_APPLY, BN_CLICKED);
		}
		else if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		else
		{
			GetDlgItem(IDC_btnOTHER_GROUP_NOVIEW_SEARCH)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnOTHER_GROUP_NOVIEW_SEARCH, BN_CLICKED);
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH COtherGroupNo_List_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
