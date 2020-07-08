// Group_Select_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Group_Select_Dlg.h"
#include "Group_Insert_Dlg.h"
#include "Group_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroup_Select_Dlg dialog


CGroup_Select_Dlg::CGroup_Select_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CGroup_Select_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroup_Select_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGroupCollection = NULL;
	m_pGroup = NULL;
	m_strAcqYear = _T("");
	m_bType = FALSE;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
}

CGroup_Select_Dlg::~CGroup_Select_Dlg()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	if(m_bType && m_pDM)
	{
		delete m_pDM;
		m_pDM = NULL;
	}
}

VOID CGroup_Select_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroup_Select_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroup_Select_Dlg, CDialog)
	//{{AFX_MSG_MAP(CGroup_Select_Dlg)
	ON_EN_KILLFOCUS(IDC_edtACQ_YEAR, OnKillfocusedtACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtGROUP_NAME, OnKillfocusedtGROUPNAME)
	ON_BN_CLICKED(IDC_btnVIEW_GROUP, OnbtnVIEWGROUP)
	ON_BN_CLICKED(IDC_btnGROUP_CREATE, OnbtnGROUPCREATE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup_Select_Dlg message handlers

BOOL CGroup_Select_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_그룹_선택")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_그룹_선택"));
	m_pDM = FindDM(_T("DM_그룹관리"));

	if(m_pDM == NULL)
	{
		m_bType = TRUE;
		CGroupMgr mgr(this);
		m_pDM = new CESL_DataMgr;
		m_pDM->SetCONNECTION_INFO(this->CONNECTION_INFO);
		m_pDM->InitDMPKFromFile(_T("DM_그룹관리"));
		m_pDM->SetMarcMgr(this->m_pInfo->pMarcMgr);			
	}
	
	if(m_strAcqYear.IsEmpty()) m_strAcqYear = GetTodayYear();
	m_pCM->SetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(ACQ_TYPE_INVALID);

	if(m_nSpeciesType == BO_RECYCLE)
		SetWindowText(_T("재활용자료 그룹선택"));
	else if(m_nSpeciesType == BO_BASIC)
		SetWindowText(_T("기초자료 그룹선택"));
	else if(m_nSpeciesType == BO_IMPORT)
		SetWindowText(_T("반입자료 그룹선택"));
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CGroup_Select_Dlg::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

VOID CGroup_Select_Dlg::OnKillfocusedtACQYEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
	OnKillfocusedtGROUPNAME();	
}

VOID CGroup_Select_Dlg::OnKillfocusedtGROUPNAME() 
{
	// TODO: Add your control notification handler code here
	CString strGroupName;
	m_pCM->GetControlMgrData(_T("그룹이름"), strGroupName);
	if(strGroupName.IsEmpty()) return;
	m_pCM->GetControlMgrData(_T("그룹생성년도"), m_strAcqYear);
	if(m_strAcqYear.IsEmpty()) return ;
	
	INT pos = strGroupName.Find(_T(","), 0);
	if(pos >= 0)
	{
		strGroupName = strGroupName.Mid(0, pos);
		strGroupName.TrimLeft();	strGroupName.TrimRight();
	}

	if(m_pGroup != NULL && m_pGroup->GetGroupInfo() == strGroupName) return ;

	INT ids = m_pGroupCollection->CheckValidate(strGroupName, m_strAcqYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("입력하신 그룹이름이 올바르지 않습니다."));
		m_pCM->SetControlMgrData(_T("그룹이름"), _T("")); 
		return;
	}
	CString strGroupNo;
	m_pGroupCollection->RemoveAll();
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT GROUP_SEQ FROM BO_ACQ_GROUP_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR='%s' AND GROUP_NAME='%s'"),
		m_strAcqYear, strGroupName);
	tmpDM.RestructDataManager(strQuery);
	strGroupNo = tmpDM.GetCellData(0, 0);
	m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupName, strGroupNo);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
}

VOID CGroup_Select_Dlg::OnbtnVIEWGROUP() 
{
	// TODO: Add your control notification handler code here
	CString strGroupName, strGroupNo;
	CGroup_List_Dlg dlg(this);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetGroupCollection(m_pGroupCollection);
	dlg.SetSpeciesType(m_nSpeciesType);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	m_pGroupCollection->SetCurSel(0);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	m_pCM->SetControlMgrData(_T("그룹이름"), m_pGroup->GetGroupInfo());
	m_pCM->SetControlMgrData(_T("그룹생성년도"), m_pGroup->GetAcqYear());
	OnKillfocusedtGROUPNAME();		
}

VOID CGroup_Select_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pGroup)
	{
		ESLAfxMessageBox(_T("선정된 그룹정보가 올바르지 않습니다."));
		return ;
	}	
	CDialog::OnOK();
}

VOID CGroup_Select_Dlg::OnbtnGROUPCREATE() 
{
	// TODO: Add your control notification handler code here
	CGroup_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(m_strAcqYear);
	if(IDOK == dlg.DoModal())
	{
		CString strAddGroupNo, strAddGroupName, strAddLastSerialNo;
		dlg.GetInsertInfo(strAddGroupNo, strAddGroupName, strAddLastSerialNo);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupName, strAddGroupNo);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		m_pCM->SetControlMgrData(_T("그룹이름"), m_pGroup->GetGroupInfo());
	}	
}

CGroupInfo * CGroup_Select_Dlg::GetSelGroup()
{
	return m_pGroup;
}

HBRUSH CGroup_Select_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
