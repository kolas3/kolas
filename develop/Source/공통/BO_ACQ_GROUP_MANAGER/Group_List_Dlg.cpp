// Group_List_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Group_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroup_List_Dlg dialog


CGroup_List_Dlg::CGroup_List_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CGroup_List_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroup_List_Dlg)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pGroupMgr = NULL;
	m_pCM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pGroupCollection = NULL;
	m_pComboMgr = NULL;
	m_arrDataStatus.RemoveAll();
	m_nSpeciesType = SPECIES_TYPE_INVALID;
}

CGroup_List_Dlg::~CGroup_List_Dlg()
{
	if(m_pGroupMgr)
	{
		delete m_pGroupMgr;
		m_pGroupMgr = NULL;
	}
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CGroup_List_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroup_List_Dlg)
	DDX_Radio(pDX, IDC_radGROUPVIEW_ALLDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroup_List_Dlg, CDialog)
	//{{AFX_MSG_MAP(CGroup_List_Dlg)
	ON_BN_CLICKED(IDC_btnGROUPVIEW_SEARCH, OnbtnGROUPVIEWSEARCH)
	ON_BN_CLICKED(IDC_btnGROUPVIEW_APPLY, OnbtnGROUPVIEWAPPLY)
	ON_CBN_SELCHANGE(IDC_cmbGROUPVIEW_DATA_STATUS, OnSelchangecmbGROUPVIEWDATASTATUS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup_List_Dlg message handlers

BOOL CGroup_List_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_그룹보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_그룹보기"));
	m_pDM = FindDM(_T("DM_그룹관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_nMyData = 0;
	UpdateData(FALSE);

	if(!Init())
	{
		ESLAfxMessageBox(_T("그룹관리 화면을 초기화 할 수 없습니다."));
		return FALSE;
	}
	
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGroup_List_Dlg::Init()
{
	INT ids = SetDataStatus();
	if(ids < 0) return FALSE;

	if(!m_strAcqYear.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
		ids = MakeDM();
		if(ids < 0) return FALSE;
	}		
	
	return TRUE;
}

VOID CGroup_List_Dlg::OnbtnGROUPVIEWSEARCH() 
{
	// TODO: Add your control notification handler code here
	MakeDM();
	m_pGrid->Display();
}

VOID CGroup_List_Dlg::OnbtnGROUPVIEWAPPLY() 
{
	// TODO: Add your control notification handler code here
	m_pGroupMgr->InitTempData();
	m_pGroupMgr->SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pGroupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	m_pGroupMgr->SetTempData(_T("InfoAlias"), _T("그룹이름"));
	m_pGroupMgr->SetTempData(_T("KeyAlias"), _T("그룹번호"));
	m_pGroupMgr->SetTempData(_T("YearAlias"), _T("그룹생성년도"));
	INT ids = m_pGroupMgr->SPFExecutePath(_T("선정자료추출"));
	if(ids < 0)
	{
		return;
	}
	m_pGroupMgr->GetTempData(_T("SelInfo"), m_strSelInfo);

	CDialog::OnOK();		
}

CString CGroup_List_Dlg::GetSelInfo()
{
	return m_strSelInfo;
}

BEGIN_EVENTSINK_MAP(CGroup_List_Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CGroup_List_Dlg)
	ON_EVENT(CGroup_List_Dlg, IDC_gridGROUPVIEW_LIST, -601 /* DblClick */, OnDblClickgridGROUPVIEWLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CGroup_List_Dlg::OnDblClickgridGROUPVIEWLIST() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;

	if(m_pDM->GetRowCount() == 0) return ;

	m_pGrid->SetAt(m_pGrid->GetRow()-1, 0, _T("V"));
	OnbtnGROUPVIEWAPPLY();	
}

INT CGroup_List_Dlg::SetDataStatus()
{
	CString strDataStatus;

	m_arrDataStatus.Add(_T("전체자료"));
	m_arrDataStatus.Add(_T("반입자료"));
	m_arrDataStatus.Add(_T("재활용자료"));

	if(m_nSpeciesType == SPECIES_TYPE_INVALID)	strDataStatus = _T("전체자료");
	else if(m_nSpeciesType == BO_BASIC)			strDataStatus = _T("기초자료");
	else if(m_nSpeciesType == BO_IMPORT)		strDataStatus = _T("반입자료");
	else if(m_nSpeciesType == BO_RECYCLE)		strDataStatus = _T("재활용자료");
	else if(m_nSpeciesType == BO_DATA_IMPORT)	strDataStatus = _T("기초반입자료");
	
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strDataStatus, &m_arrDataStatus, _T("자료상태"));

	if(strDataStatus == _T("전체자료")) GetDlgItem(IDC_cmbGROUPVIEW_DATA_STATUS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbGROUPVIEW_DATA_STATUS)->EnableWindow(TRUE);

	return 0;
}

INT CGroup_List_Dlg::MakeDM()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료상태"));
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel < 0) return -1;

	if(nCurSel == 0)		m_nSpeciesType = SPECIES_TYPE_INVALID; 
	else if(nCurSel == 1)	m_nSpeciesType = BO_IMPORT; 
	else if(nCurSel == 2)	m_nSpeciesType = BO_RECYCLE; 

	SetBookCountDMField();
	CString strCreator;
	UpdateData(TRUE);
	if(m_nMyData == 1) strCreator = m_pInfo->USER_NAME;
	
	if(m_pGroupMgr == NULL) m_pGroupMgr = new CGroupMgr(this);
	m_pCM->GetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
	if(m_strAcqYear.IsEmpty())
		m_strAcqYear = m_pGroupMgr->GetTodayYear();
	CString strQuery;
	if(strCreator.IsEmpty()) strQuery.Format(_T("CREATE_YEAR = '%s'"), m_strAcqYear);
	else strQuery.Format(_T("CREATE_YEAR = '%s' AND GROUP_CREATOR='%s'"), m_strAcqYear, strCreator);
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	
	m_pGroupMgr->InitTempData();	m_pGroupMgr->InitTempPtr();
	m_pGroupMgr->SetTempData(_T("조건"), strQuery.GetBuffer(0));
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	m_pGroupMgr->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pGroupMgr->SetTempData(_T("ACQ_YEAR"), m_strAcqYear.GetBuffer(0));
	ids = m_pGroupMgr->SPFExecutePath(_T("그룹책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return -1;
	}

	m_pCM->SetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
	m_pGrid->Display();
	return 0;
}

VOID CGroup_List_Dlg::OnSelchangecmbGROUPVIEWDATASTATUS() 
{
	// TODO: Add your control notification handler code here
	MakeDM();
}

INT CGroup_List_Dlg::SetBookCountDMField()
{
	CString strFunction;
	if(m_nSpeciesType == SPECIES_TYPE_INVALID)	strFunction = _T("GetGroupBookCount('0', CREATE_YEAR, GROUP_SEQ)");
	else if(m_nSpeciesType == BO_BASIC)			strFunction = _T("GetAcqGroupBookCount('0', CREATE_YEAR, GROUP_SEQ, 'BOT111O')");
	else if(m_nSpeciesType == BO_IMPORT)		strFunction = _T("GetAcqGroupBookCount('0', CREATE_YEAR, GROUP_SEQ, 'BOT112O')");
	else if(m_nSpeciesType == BO_RECYCLE)		strFunction = _T("GetAcqGroupBookCount('0', CREATE_YEAR, GROUP_SEQ, NULL)");
	else if(m_nSpeciesType == BO_DATA_IMPORT)	strFunction = _T("GetAcqGroupBookCount('0', CREATE_YEAR, GROUP_SEQ, 'BOT')");
	
	INT nColCnt = m_pDM->m_nCols;
	if(nColCnt < 0) nColCnt = m_pDM->GetRefColumnCount();

	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT col = m_pDM->FindColumn(_T("그룹책수"));
	if(col < 0 ) return -1;
	INT ids = m_pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
	if(ids != 0) return -1;
	FIELD_NAME = strFunction;
	ids = m_pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
	if(ids < 0 ) return -1;
		
	return 0;
}

BOOL CGroup_List_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_btnGROUPVIEW_APPLY)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDC_btnGROUPVIEW_APPLY)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnGROUPVIEW_APPLY, BN_CLICKED);
		}
		else if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		else
		{
			GetDlgItem(IDC_btnGROUPVIEW_SEARCH)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnGROUPVIEW_SEARCH, BN_CLICKED);
		}
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CGroup_List_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
