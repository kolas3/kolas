// Group_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "Group_Mgr.h"
#include "Group_Divide_Dlg.h"
#include "GroupPair.h"
#include "Group_Select_Dlg.h"
#include "MergeGroup_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroup_Mgr dialog


CGroup_Mgr::CGroup_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CGroup_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroup_Mgr)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_nOpenMode = -1;
	m_strSelKey = _T(""); 
	m_strSelInfo = _T("");
}

CGroup_Mgr::~CGroup_Mgr()
{
}

VOID CGroup_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroup_Mgr)
	DDX_Radio(pDX, IDC_radGROUP_ALLDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroup_Mgr, CDialog)
	//{{AFX_MSG_MAP(CGroup_Mgr)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnGROUP_SEARCH, OnbtnGROUPSEARCH)
	ON_BN_CLICKED(IDC_btnGROUP_INSERT, OnbtnGROUPINSERT)
	ON_BN_CLICKED(IDC_btnGROUP_UPDATE, OnbtnGROUPUPDATE)
	ON_BN_CLICKED(IDC_btnGROUP_DELETE, OnbtnGROUPDELETE)
	ON_BN_CLICKED(IDC_btnGROUP_APPLY, OnbtnGROUPAPPLY)
	ON_BN_CLICKED(IDC_btnGROUP_MERGE, OnbtnGROUPMERGE)
	ON_BN_CLICKED(IDC_btnGROUP_DIVIDE, OnbtnGROUPDIVIDE)
	ON_BN_CLICKED(IDC_btnGROUP_COPY, OnbtnGROUPCOPY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup_Mgr message handlers

VOID CGroup_Mgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CGroup_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_그룹관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_그룹관리"));
	m_pDM = FindDM(_T("DM_그룹관리"));
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(!pGrid) return FALSE;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("그룹관리 화면을 초기화 할 수 없습니다."));
		return FALSE;
	}
	m_nMyData = 0;
	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGroup_Mgr::Init()
{
	switch(m_nOpenMode)
	{
	case 0:
		GetDlgItem(IDC_btnGROUP_APPLY)->ShowWindow(SW_HIDE);
		break;
	case 1:
		GetDlgItem(IDC_btnGROUP_APPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_COPY)->ShowWindow(SW_HIDE);
		break;
	case 2:
		GetDlgItem(IDC_btnGROUP_INSERT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_UPDATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_MERGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_DIVIDE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnGROUP_COPY)->ShowWindow(SW_HIDE);
		if(!m_strCreateYear.IsEmpty())
			m_pCM->GetControlMgrData(_T("그룹생성년도"), m_strCreateYear);
		break;
	default :
		return FALSE;
	}

	return TRUE;
}

VOID CGroup_Mgr::OnbtnGROUPSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strCreator;
	UpdateData(TRUE);
	if(m_nMyData == 1) strCreator = m_pInfo->USER_NAME;
	m_pCM->GetControlMgrData(_T("그룹생성년도"), m_strCreateYear);
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempData(_T("Creator"), strCreator.GetBuffer(0));
	INT ids = mgr.SPFExecutePath(_T("그룹검색"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("그룹정보를 구축할 수 없습니다."));
		return;
	}
	CString strQuery;
	mgr.GetTempData(_T("조건"), strQuery);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempData(_T("조건"), strQuery.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	if(m_nOpenMode == 1) mgr.SetTempData(_T("SPECIES_TYPE"), (INT)BO_RECYCLE);
	else mgr.SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
	mgr.SetTempData(_T("ACQ_YEAR"), m_strCreateYear.GetBuffer(0));
	ids = mgr.SPFExecutePath(_T("그룹책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return;
	}

	DisplayGrid(_T("MainGrid"));
}

VOID CGroup_Mgr::OnbtnGROUPINSERT() 
{
	// TODO: Add your control notification handler code here
	InsertGroupInfo(SINSERT);
	OnbtnGROUPSEARCH();
}

VOID CGroup_Mgr::OnbtnGROUPUPDATE() 
{
	// TODO: Add your control notification handler code here
	if(m_pDM->GetRowCount() == 0)	
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}
	UpdateGroupInfo(SUPDATE);
}

VOID CGroup_Mgr::OnbtnGROUPDELETE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	
	CESL_DataMgr * pDM_DELETE = FindDM(_T("DM_그룹삭제"));
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("DELETE_DM"), (DWORD*)pDM_DELETE);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	mgr.SetTempData(_T("그룹모드"), m_nOpenMode);
	ids = mgr.SPFExecutePath(_T("그룹삭제"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("정상적으로 삭제하지 못했습니다."));
		return;
	}
	else if(ids == 0) DisplayGrid(_T("MainGrid"));	
	OnbtnGROUPSEARCH();
}

INT CGroup_Mgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	return 0;
}

VOID CGroup_Mgr::DisplayGrid(CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if(!pGrid) return;
	pGrid->Display();
}

VOID CGroup_Mgr::OnbtnGROUPAPPLY() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	mgr.SetTempData(_T("InfoAlias"), _T("그룹이름"));
	mgr.SetTempData(_T("KeyAlias"), _T("그룹번호"));
	mgr.SetTempData(_T("YearAlias"), _T("수입년도"));
	INT ids = mgr.SPFExecutePath(_T("선정자료추출"));
	if(ids < 0)
	{
		return;
	}
	mgr.GetTempData(_T("SelInfo"), m_strSelInfo);

	CDialog::OnOK();
}

VOID CGroup_Mgr::OnbtnGROUPMERGE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("그룹생성년도"), strAcqYear);

	CGroupCollection GroupCollection(this);
	GroupCollection.RemoveAll();
	SetSelGroupCollection(&GroupCollection);
	
	CMergeGroup_Select_Dlg dlg(this);
	dlg.SetDM(m_pDM);
	dlg.SetGroupType(GROUP_NO);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetGroupCollection(&GroupCollection);
	
	if(IDOK == dlg.DoModal())	
	{
		CGroupPair mgr(this);
		mgr.SetTargetGroup(dlg.GetSelGroup());
		mgr.SetSourceGroupCollection(&GroupCollection);
		mgr.SetGroupType(GROUP_NO);
		if(m_nOpenMode == 1) mgr.SetDeleteDataYN(TRUE);
		mgr.SetMainDM(m_pDM);
		
		INT ids = mgr.MergeGroup();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("선정한 자료를 통합할 수 업습니다."));
			return ;
		}
		DisplayGrid(_T("MainGrid"));
	}
}

VOID CGroup_Mgr::OnbtnGROUPDIVIDE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CGroup_Divide_Dlg dlg(this);
	dlg.SetParentDM(m_pDM);
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	dlg.SetParentGrid(pGrid);
	if(m_nOpenMode == 1) dlg.SetDeleteDataYN(TRUE);
	dlg.DoModal();
	DisplayGrid(_T("MainGrid"));
}

VOID CGroup_Mgr::OnbtnGROUPCOPY() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CString strMsg;
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	ids = pGrid->SelectMakeList();
	INT i= pGrid->SelectGetHeadPosition();
	CGroupInfo SourceGroup, TargetGroup;
	CGroupPair mgr(this);
	mgr.SetGroupType(GROUP_NO);
	mgr.SetMainDM(m_pDM);
	while(i >= 0)
	{
		if(InsertGroupInfo(DINSERT) == 0)
		{
			SetGroupInfo(&SourceGroup, i);
			SetGroupInfo(&TargetGroup, m_pDM->GetRowCount()-1);
			mgr.SetTargetGroup(&TargetGroup);
			mgr.SetSourceGroup(&SourceGroup);
			ids = mgr.CopyGroup();
			if(ids < 0)
			{
				strMsg.Format(_T("%s년 %s 그룹을 복사할 수 없습니다."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
				ESLAfxMessageBox(strMsg);
			}
			mgr.UpdateGroupBookCnt(&TargetGroup);
			strMsg.Format(_T("%s년 %s 그룹을 복사하였습니다."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
		}

		i = pGrid->SelectGetNext();
	}
	DisplayGrid(_T("MainGrid"));
}

INT CGroup_Mgr::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;

	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(!pGrid) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

INT CGroup_Mgr::InsertGroupInfo(OPERATION_TYPE nOpType)
{
	CString strCreateYear;
	m_pCM->GetControlMgrData(_T("그룹생성년도"), strCreateYear);
	CGroup_Insert_Dlg dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetAcqYear(strCreateYear);
	dlg.SetMainCM(m_pCM);
	if(IDOK == dlg.DoModal()) return 0;
	return -1;
}

INT CGroup_Mgr::SetGroupInfo(CGroupInfo *pGroup, INT idx)
{
	pGroup->SetAcqYear(m_pDM->GetCellData(_T("그룹생성년도"), idx));
	pGroup->SetGroupInfo(m_pDM->GetCellData(_T("그룹이름"), idx));
	pGroup->SetGroupNo(m_pDM->GetCellData(_T("그룹번호"), idx));
	pGroup->SetLastSerialNo(_ttoi(m_pDM->GetCellData(_T("마지막일련번호"), idx).GetBuffer(0)));
	return 0;
}

INT CGroup_Mgr::SetSelGroupCollection(CGroupCollection *pGroupCollection)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	while(i>=0)
	{
		pGroupCollection->AddGroupInfo(m_pDM->GetCellData(_T("그룹생성년도"), i), m_pDM->GetCellData(_T("그룹이름"), i), m_pDM->GetCellData(_T("그룹번호"), i));
		i = pGrid->SelectGetNext();
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CGroup_Mgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CGroup_Mgr)
	ON_EVENT(CGroup_Mgr, IDC_gridGROUP_LIST, -601 /* DblClick */, OnDblClickgridGROUPLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CGroup_Mgr::OnDblClickgridGROUPLIST() 
{
	// TODO: Add your control notification handler code here
	UpdateGroupInfo(GUPDATE);
}

VOID CGroup_Mgr::UpdateGroupInfo(OPERATION_TYPE nOpType)
{
	CString strCreateYear;
	m_pCM->GetControlMgrData(_T("그룹생성년도"), strCreateYear);
	CGroup_Insert_Dlg dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetAcqYear(strCreateYear);
	dlg.SetMainCM(m_pCM);
	dlg.DoModal();	
}

BOOL CGroup_Mgr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_edtGROUP_CREATE_YEAR)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnGROUP_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_radGROUP_MYDATA)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnGROUP_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_radGROUP_ALLDATA)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnGROUP_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_edtGROUP_NAME)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnGROUP_SEARCH, BN_CLICKED);
	
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CGroup_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
