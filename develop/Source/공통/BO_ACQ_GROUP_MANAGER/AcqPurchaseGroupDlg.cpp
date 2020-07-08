// AcqPurchaseGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcqPurchaseGroupDlg.h"
#include "DivisionNo_Status_Change_Dlg.h"
#include "DivisionNo_Insert_Dlg.h"
#include "DivisionNo_Update_Dlg.h"
#include "DivisionNo_DataView_Dlg.h"
#include "DivisionNo_Divide_Dlg.h"
#include "MergeGroup_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqPurchaseGroupDlg dialog


CAcqPurchaseGroupDlg::CAcqPurchaseGroupDlg(CESL_Mgr* pParent /*=NULL*/)
	: CAcqGroupDlg(CAcqPurchaseGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqPurchaseGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAcqPurchaseGroupDlg::~CAcqPurchaseGroupDlg()
{
}

VOID CAcqPurchaseGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqPurchaseGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqPurchaseGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CAcqPurchaseGroupDlg)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqPurchaseGroupDlg message handlers

BOOL CAcqPurchaseGroupDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CAcqPurchaseGroupDlg::CMD_SEARCH()
{
	INT ids = ShowSearchDlg(this);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색화면을 생성할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CAcqPurchaseGroupDlg::CMD_INSERT()
{
	return InsertDivNoInfo(SINSERT);
}

INT CAcqPurchaseGroupDlg::CMD_UPDATE()
{
	if(m_pDM->GetRowCount() == 0) return 0;
	return UpdateDivNoInfo(SUPDATE);
}

INT CAcqPurchaseGroupDlg::CMD_DELETE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}	
	
	CESL_DataMgr * pDM_DELETE = FindDM(_T("DM_구입차수삭제"));
	
	ids = DeleteGroup(pDM_DELETE, _T("차수삭제"));
	return ids;
}

INT CAcqPurchaseGroupDlg::CMD_GROUP_MERGE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}	
	
	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(PURCHASE);
	GroupCollection.RemoveAll();
	SetSelGroupCollection(&GroupCollection);
	
	CString strWorkStatrus, strAcqYear;
	m_pCM->GetControlMgrData(_T("작업상태"), strWorkStatrus);
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	CMergeGroup_Select_Dlg dlg(this);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkStatus(strWorkStatrus);
	dlg.SetDM(m_pDM);
	dlg.SetGroupType(DIVNO);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDOK == dlg.DoModal())	
	{
		CGroupPair mgr(this);
		mgr.SetTargetGroup(dlg.GetSelGroup());
		mgr.SetSourceGroupCollection(&GroupCollection);
		mgr.SetGroupType(DIVNO);
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

INT CAcqPurchaseGroupDlg::CMD_GROUP_DIVIDE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	CString strWorkStatus;
	if(m_strWorkStatus == _T("0")) strWorkStatus = _T("구입대상");
	else if(m_strWorkStatus == _T("1")) strWorkStatus = _T("주문");
	else if(m_strWorkStatus == _T("2")) strWorkStatus = _T("검수완료");
	else return -1;
	
	CDivisionNo_Divide_Dlg	dlg(this);
	dlg.SetParentDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.SetWorkingStatus(strWorkStatus);
	dlg.DoModal();
	
	::SendMessage(this->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	
	m_pGrid->Display();
	return 0;
}

INT CAcqPurchaseGroupDlg::CMD_GROUP_COPY()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}
	
	CString strMsg;
	if(m_strWorkStatus == _T("2"))
		ESLAfxMessageBox(_T("등록번호가 부여된 자료는 복사되지 않습니다."));
	
	ids = m_pGrid->SelectMakeList();
	INT i= m_pGrid->SelectGetHeadPosition();
	CGroupInfo SourceGroup, TargetGroup;
	CGroupPair mgr(this);
	mgr.SetGroupType(DIVNO);
	mgr.SetMainDM(m_pDM);
	while(i >= 0)
	{
		if(InsertDivNoInfo(DINSERT) == 0)
		{
			SetGroupInfo(&SourceGroup, i);
			SourceGroup.SetRecKey(_T("1"), m_pDM);
			SetGroupInfo(&TargetGroup, m_pDM->GetRowCount()-1);
			mgr.SetTargetGroup(&TargetGroup);
			mgr.SetSourceGroup(&SourceGroup);
			ids = mgr.CopyGroup();
			if(ids < 0)
			{
				strMsg.Format(_T("%s년 %s의 차수를 복사할 수 없습니다."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
				ESLAfxMessageBox(strMsg);
			}
			mgr.UpdateGroupBookCnt(&TargetGroup);
			strMsg.Format(_T("%s년 %s의 차수를 복사하였습니다."), SourceGroup.GetAcqYear(), SourceGroup.GetGroupInfo());
			ESLAfxMessageBox(strMsg);
		}
		
		i = m_pGrid->SelectGetNext();
	}
	
	m_pGrid->Display();
	return 0;
}

INT CAcqPurchaseGroupDlg::CMD_VIEW_GROUPDATA()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	CDivisionNo_DataView_Dlg dlg(this);
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();	
	return 0;
}

INT CAcqPurchaseGroupDlg::CMD_GROUP_CHANGE_STATUS()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}
	CArray<INT, INT> arrIdx;	arrIdx.RemoveAll();
	CString strTmpData, strDivNo, strAcqYear, strSelDivNo, strWorkCode, strWorkStatus;
	INT nTmpData = -1;
	INT i = m_pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		strTmpData = m_pDM->GetCellData(_T("책수"), i);
		strDivNo = m_pDM->GetCellData(_T("차수번호"), i);
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		if(nTmpData != 0) 
		{
			m_pGrid->SetReverse(i);
			strTmpData.Format(_T("선정하신 %s차수는 빈차수가 아니므로 상태변경할 수 없습니다."), strDivNo);
			m_pGrid->SetAt(i, 0, _T(""));
			ESLAfxMessageBox(strTmpData);
			i = m_pGrid->SelectGetNext();
			continue;
		}
		if(strSelDivNo.IsEmpty()) strSelDivNo = strDivNo;
		else strSelDivNo += _T(", ") + strDivNo;
		arrIdx.Add(i);
		i = m_pGrid->SelectGetNext();
	}
	
	if(arrIdx.GetSize() == 0)
	{
		ESLAfxMessageBox(_T("상태변경할 차수가 없습니다."));
		return -1;
	}

	if(m_strWorkStatus == _T("0")) strWorkStatus = _T("구입대상");
	else if(m_strWorkStatus == _T("1")) strWorkStatus = _T("주문");
	else if(m_strWorkStatus == _T("2")) strWorkStatus = _T("검수완료");
	else return -1;
	
	CDivisionNo_Status_Change_Dlg dlg(this);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetWorkCode(m_strWorkCode);
	dlg.SetWorkStatus(strWorkStatus);
	dlg.SetSelIdx(&arrIdx);
	dlg.SetDM(m_pDM);
	dlg.SetDivNo(strSelDivNo);
	if(IDOK == dlg.DoModal())
	{
		ESLAfxMessageBox(_T("선정하신 차수의 상태를 변경하였습니다."));
		m_pGrid->Display();
	}
	return 0;
}

BOOL CAcqPurchaseGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서구입차수관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_수서구입차수관리"));
	m_pDM = FindDM(_T("DM_구입차수관리"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	m_pSearchDlg = new CAcqGroupSearchDlg(this);
	m_pSearchDlg->SetAcqGroupType(ACQ_PURCHASE_GROUP);

	m_pGroupMgr = new CGroupMgr(this);

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(PURCHASE);
	
	GetDlgItem(IDC_stcGROUP_ACQ_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUP_ACQ_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcGROUP_WORK_CODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUP_WORK_CODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcGROUP_WORK_STATUS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUP_WORK_STATUS)->ShowWindow(SW_HIDE);

	InitGroupInfo();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAcqPurchaseGroupDlg::OnSize(UINT nType, INT cx, INT cy) 
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

	CButton *pBtn = (CButton *)GetDlgItem(IDC_edtGROUP_ACQ_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;	
	
	pBtn = (CButton*)GetDlgItem(IDC_stcGROUP_ACQ_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtGROUP_ACQ_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 7, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcGROUP_WORK_CODE);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(180, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	pEdt = (CEdit*)GetDlgItem(IDC_edtGROUP_WORK_CODE);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(240, 7, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcGROUP_WORK_STATUS);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(360, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_HIDE);
	
	pEdt = (CEdit*)GetDlgItem(IDC_edtGROUP_WORK_STATUS);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(420, 7, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_HIDE);

	m_pGrid->MoveWindow(-2, 35, cx+2, cy);	
}

INT CAcqPurchaseGroupDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("UDF_작업상태설명"))
	{	
		strTmpData = m_pDM->GetCellData(_T("작업상태"), nRow);
		if(strTmpData == _T("0")) str = _T("구입대상");
		else if(strTmpData == _T("1")) str = _T("주문");
		else if(strTmpData == _T("2")) str = _T("검수완료");
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("업무구분"))
	{	
		if(str.IsEmpty()) str = _T("적용안함");
		else if(str == _T("1")) str = _T("동양서");
		else if(str == _T("2")) str = _T("서양서");
		else if(str == _T("3")) str = _T("비도서");
	}
	return 0;
}

INT CAcqPurchaseGroupDlg::SetGroupInfo(CGroupInfo *pGroup, INT idx)
{
	pGroup->SetAcqYear(m_pDM->GetCellData(_T("수입년도"), idx));
	pGroup->SetGroupInfo(m_pDM->GetCellData(_T("차수번호"), idx));
	pGroup->SetLastWorkNo(m_pDM->GetCellData(_T("마지막작업번호"), idx));
	pGroup->SetWorkCode(m_pDM->GetCellData(_T("업무구분"), idx));
	return 0;
}

INT CAcqPurchaseGroupDlg::InsertDivNoInfo(OPERATION_TYPE nOpType)
{
	CString strWorkStatus;
	if(m_strWorkStatus == _T("0")) strWorkStatus = _T("구입대상");
	else if(m_strWorkStatus == _T("1")) strWorkStatus = _T("주문");
	else if(m_strWorkStatus == _T("2")) strWorkStatus = _T("검수완료");
	else if(m_strWorkStatus.IsEmpty())
	{
		ESLAfxMessageBox(_T("전체자료인 상태에서 차수를 추가할 수 없습니다."));
		return -1;
	}

	CESL_DataMgr * pWorkNoDM = FindDM(_T("DM_작업번호관리"));
	CDivisionNo_Insert_Dlg dlg(this);
	dlg.SetDivNoDMPtr(m_pDM);
	dlg.SetWorkNoDMPtr(pWorkNoDM);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetWorkStatus(strWorkStatus);
	dlg.SetWorkCode(m_strWorkCode);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetParentGrid(m_pGrid);
	dlg.SetOpenMode(nOpType);
	if(IDOK == dlg.DoModal()) return 0;
	return -1;
}

UINT CAcqPurchaseGroupDlg::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	strQuery = m_pSearchDlg->GetWhereStmt();
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("구입차수를 검색할 수 없습니다."));
		return -1;
	}

	SetGroupInfo();

	m_pGroupMgr->InitTempData();	m_pGroupMgr->InitTempPtr();
	m_pGroupMgr->SetTempData(_T("조건"), strQuery.GetBuffer(0));
	m_pGroupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pGroupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	m_pGroupMgr->SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
	m_pGroupMgr->SetTempData(_T("WORK_CODE"), m_strWorkCode.GetBuffer(0));
	m_pGroupMgr->SetTempData(_T("ACQ_YEAR"), m_strAcqYear.GetBuffer(0));
	ids = m_pGroupMgr->SPFExecutePath(_T("차수번호책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return -1;
	}
	
	m_pGrid->Display();
	return 0;
}
BEGIN_EVENTSINK_MAP(CAcqPurchaseGroupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAcqPurchaseGroupDlg)
	ON_EVENT(CAcqPurchaseGroupDlg, IDC_gridPURCHASE_GROUP, -601 /* DblClick */, OnDblClickgridPURCHASEGROUP, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAcqPurchaseGroupDlg::OnDblClickgridPURCHASEGROUP() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return;
	if(m_pDM->GetRowCount() == 0) return ;
	UpdateDivNoInfo(GUPDATE);
}

INT CAcqPurchaseGroupDlg::UpdateDivNoInfo(OPERATION_TYPE nOpType)
{
	CDivisionNo_Update_Dlg dlg(this);
	dlg.SetOpType(nOpType);
	dlg.SetMainGrid(m_pGrid);
	dlg.DoModal();
	return 0;
}

INT CAcqPurchaseGroupDlg::SetSelGroupCollection(CGroupCollection * pGroupCollection)
{
	CString strMsg, strAcqYear, strGroupInfo, strWorkCode;
	
	INT ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetHeadPosition();
	while(i>=0)
	{
		strAcqYear = m_pDM->GetCellData(_T("수입년도"), i);
		strGroupInfo = m_pDM->GetCellData(_T("차수번호"), i);
		strWorkCode = m_pDM->GetCellData(_T("업무구분"), i);
		if(!pGroupCollection->IsSameWorkCode(m_pDM->GetCellData(_T("업무구분"), i)))
		{
			strMsg.Format(_T("%s-%s-%s 차수는 통합될 수 없습니다."), strAcqYear, strWorkCode, strGroupInfo);
			ESLAfxMessageBox(strMsg);
		}
		else 
			pGroupCollection->AddGroupInfo(strAcqYear, strGroupInfo, strWorkCode);
		
		i = m_pGrid->SelectGetNext();
	}
	return 0;
}

VOID CAcqPurchaseGroupDlg::InitGroupInfo()
{
	m_strAcqYear = GetTodayYear();
	m_pInfo->pCodeMgr->GetDefaultCode(_T("업무구분"), m_strWorkCode);
	m_strWorkStatus = _T("2");

	CString strTmpData;
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), m_strWorkCode, strTmpData);
	m_pCM->SetControlMgrData(_T("업무구분"), strTmpData);
	m_pCM->SetControlMgrData(_T("자료상태"), _T("검수완료"));

	m_pSearchDlg->SetAcqYear(m_strAcqYear);
	m_pSearchDlg->SetWorkCode(m_strWorkCode);
	m_pSearchDlg->SetWorkStatus(m_strWorkStatus);
}

VOID CAcqPurchaseGroupDlg::SetGroupInfo()
{
	m_strAcqYear = m_pSearchDlg->GetAcqYear();
	m_strWorkCode = m_pSearchDlg->GetWorkCode();
	m_strWorkStatus = m_pSearchDlg->GetWorkStatus();

	CString strTmpData;
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), m_strWorkCode, strTmpData);
	m_pCM->SetControlMgrData(_T("업무구분"), strTmpData);
	if(m_strWorkStatus.IsEmpty()) strTmpData = _T("전체자료");
	else if(m_strWorkStatus == _T("0")) strTmpData = _T("구입대상");
	else if(m_strWorkStatus == _T("1")) strTmpData = _T("주문");
	else if(m_strWorkStatus == _T("2")) strTmpData = _T("검수완료");
	m_pCM->SetControlMgrData(_T("자료상태"), strTmpData);
}

HBRUSH CAcqPurchaseGroupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}
