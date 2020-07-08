// AcqDonateGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcqDonateGroupDlg.h"
#include "RequisitionNo_Insert_Dlg.h"
#include "RequisitionNo_DataView_Dlg.h"
#include "RequisitionNo_Divide_Dlg.h"
#include "MergeGroup_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqDonateGroupDlg dialog


CAcqDonateGroupDlg::CAcqDonateGroupDlg(CESL_Mgr* pParent /*=NULL*/)
	: CAcqGroupDlg(CAcqDonateGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqDonateGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAcqDonateGroupDlg::~CAcqDonateGroupDlg()
{
}

VOID CAcqDonateGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqDonateGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqDonateGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CAcqDonateGroupDlg)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqDonateGroupDlg message handlers

BOOL CAcqDonateGroupDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CAcqDonateGroupDlg::CMD_SEARCH()
{
	INT ids = ShowSearchDlg(this);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색화면을 생성할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CAcqDonateGroupDlg::CMD_INSERT()
{
	return ShowReceiptNoDlg(SINSERT);
}

INT CAcqDonateGroupDlg::CMD_UPDATE()
{
	if(m_pDM->GetRowCount() == 0) return 0;
	return ShowReceiptNoDlg(SUPDATE);
}

INT CAcqDonateGroupDlg::CMD_DELETE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}	
	CESL_DataMgr * pDM_DELETE = FindDM(_T("DM_접수번호삭제"));
	
	ids = DeleteGroup(pDM_DELETE, _T("접수번호삭제"));
	return ids;
}

INT CAcqDonateGroupDlg::CMD_GROUP_MERGE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	
	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(DONATE);
	GroupCollection.RemoveAll();
	SetSelGroupCollection(&GroupCollection);
	
	CMergeGroup_Select_Dlg dlg(this);
	dlg.SetDM(m_pDM);
	dlg.SetGroupType(RECEIPT_NO);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetGroupCollection(&GroupCollection);
	
	if(IDOK == dlg.DoModal())	
	{
		CGroupPair mgr(this);
		mgr.SetTargetGroup(dlg.GetSelGroup());
		mgr.SetSourceGroupCollection(&GroupCollection);
		mgr.SetGroupType(RECEIPT_NO);
		mgr.SetMainDM(m_pDM);
		INT ids = mgr.MergeGroup();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("선정한 자료를 통합할 수 업습니다."));
			return -1;
		}
		m_pGrid->Display();
	}
	return 0;
}

INT CAcqDonateGroupDlg::CMD_GROUP_DIVIDE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0; 
	}
	CRequisitionNo_Divide_Dlg dlg(this);
	dlg.SetParentDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();

	::SendMessage(this->GetSafeHwnd(), QUERY_CREATE, 0, 0);

	m_pGrid->Display();
	return 0;
}

INT CAcqDonateGroupDlg::CMD_GROUP_COPY()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}	
	ESLAfxMessageBox(_T("등록번호가 부여된 자료는 복사되지 않습니다."));
	
	CString strMsg;
	
	ids = m_pGrid->SelectMakeList();
	INT i= m_pGrid->SelectGetHeadPosition();
	CGroupInfo SourceGroup, TargetGroup;
	CGroupPair mgr(this);
	mgr.SetMainDM(m_pDM);
	mgr.SetGroupType(RECEIPT_NO);
	while(i >= 0)
	{
		if(ShowReceiptNoDlg(DINSERT) == 0)
		{
			SourceGroup.SetAcqYear(m_pDM->GetCellData(_T("수입년도"), i));
			SourceGroup.SetGroupInfo(m_pDM->GetCellData(_T("접수번호"), i));
			TargetGroup.SetAcqYear(m_pDM->GetCellData(_T("수입년도"), m_pDM->GetRowCount()-1));
			TargetGroup.SetGroupInfo(m_pDM->GetCellData(_T("접수번호"), m_pDM->GetRowCount()-1));
			
			mgr.SetTargetGroup(&TargetGroup);
			mgr.SetSourceGroup(&SourceGroup);
			ids = mgr.CopyGroup();
			if(ids < 0)
			{
				strMsg.Format(_T("%s년 %s의 접수번호를 복사할 수 없습니다."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
				ESLAfxMessageBox(strMsg);
			}
			mgr.UpdateGroupBookCnt(&TargetGroup);
			strMsg.Format(_T("%s년 %s의 접수번호를 복사하였습니다."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
			ESLAfxMessageBox(strMsg);
		}
		
		i = m_pGrid->SelectGetNext();
	}
	m_pGrid->Display();
	return 0;
}

INT CAcqDonateGroupDlg::CMD_VIEW_GROUPDATA()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	CRequisitionNo_DataView_Dlg dlg(this);
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();		
	return 0;
}

BOOL CAcqDonateGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서기증접수번호관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_수서기증접수번호관리"));
	m_pDM = FindDM(_T("DM_접수번호관리"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	m_pSearchDlg = new CAcqGroupSearchDlg(this);
	m_pSearchDlg->SetAcqGroupType(ACQ_DONATE_GROUP);

	m_pGroupMgr = new CGroupMgr(this);

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(DONATE);

	GetDlgItem(IDC_stcGROUP_ACQ_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUP_ACQ_YEAR)->ShowWindow(SW_HIDE);
	
	InitGroupInfo();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAcqDonateGroupDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 50;
	rect.bottom -= 5;
	rect.left += 5;
	rect.right -= 5;
	
	m_pGrid->MoveWindow(rect);		

	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcGROUP_ACQ_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtGROUP_ACQ_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 7, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	m_pGrid->MoveWindow(-2, 35, cx+2, cy);
}

UINT CAcqDonateGroupDlg::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	strQuery = m_pSearchDlg->GetWhereStmt();
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("접수번호 정보를 구축할 수 없습니다."));
		return -1;
	}
	
	SetGroupInfo();
	
	m_pGroupMgr->InitTempData();	m_pGroupMgr->InitTempPtr();
	m_pGroupMgr->SetTempData(_T("조건"), strQuery.GetBuffer(0));
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	m_pGroupMgr->SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
	m_pGroupMgr->SetTempData(_T("ACQ_YEAR"), m_strAcqYear.GetBuffer(0));
	ids = m_pGroupMgr->SPFExecutePath(_T("접수번호책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return -1;
	}
	
	m_pGrid->Display();
	return 0;
}

BEGIN_EVENTSINK_MAP(CAcqDonateGroupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAcqDonateGroupDlg)
	ON_EVENT(CAcqDonateGroupDlg, IDC_gridDONATE_GROUP, -601 /* DblClick */, OnDblClickgridDONATEGROUP, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAcqDonateGroupDlg::OnDblClickgridDONATEGROUP() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	if(m_pDM->GetRowCount() == 0) return ;
	ShowReceiptNoDlg(GUPDATE);
}

INT CAcqDonateGroupDlg::ShowReceiptNoDlg(OPERATION_TYPE nOpType)
{
	CRequisitionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetMainCM(m_pCM);
	if(IDOK == dlg.DoModal()) return 0;
	return -1;
}

INT CAcqDonateGroupDlg::SetSelGroupCollection(CGroupCollection *pGroupCollection)
{
	INT ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetHeadPosition();
	while(i>=0)
	{
		pGroupCollection->AddGroupInfo(m_pDM->GetCellData(_T("수입년도"), i), m_pDM->GetCellData(_T("접수번호"), i), m_pDM->GetCellData(_T("그룹번호"), i));
		i = m_pGrid->SelectGetNext();
	}
	return 0;
}

VOID CAcqDonateGroupDlg::InitGroupInfo()
{
	m_strAcqYear = GetTodayYear();
	
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	
	m_pSearchDlg->SetAcqYear(m_strAcqYear);	
}

VOID CAcqDonateGroupDlg::SetGroupInfo()
{
	m_strAcqYear = m_pSearchDlg->GetAcqYear();
	
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
}

HBRUSH CAcqDonateGroupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
