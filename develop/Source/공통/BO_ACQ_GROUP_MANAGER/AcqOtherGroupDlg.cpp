// AcqOtherGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcqOtherGroupDlg.h"
#include "OtherGroupNo_Insert_Dlg.h"
#include "OtherGroupNo_DataView_Dlg.h"
#include "OtherGroupNo_Divide_Dlg.h"
#include "MergeGroup_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqOtherGroupDlg dialog


CAcqOtherGroupDlg::CAcqOtherGroupDlg(CESL_Mgr* pParent /*=NULL*/)
	: CAcqGroupDlg(CAcqOtherGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqOtherGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAcqOtherGroupDlg::~CAcqOtherGroupDlg()
{
}

VOID CAcqOtherGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqOtherGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqOtherGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CAcqOtherGroupDlg)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqOtherGroupDlg message handlers

BOOL CAcqOtherGroupDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CAcqOtherGroupDlg::CMD_SEARCH()
{
	INT ids = ShowSearchDlg(this);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("검색화면을 생성할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CAcqOtherGroupDlg::CMD_INSERT()
{
	return ShowOtherGroupDlg(SINSERT);
}

INT CAcqOtherGroupDlg::CMD_UPDATE()
{
	if(m_pDM->GetRowCount() == 0) return 0;
	return ShowOtherGroupDlg(SUPDATE);
}

INT CAcqOtherGroupDlg::CMD_DELETE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}	
	CESL_DataMgr * pDM_DELETE = FindDM(_T("DM_그룹삭제"));
	ids = DeleteGroup(pDM_DELETE, _T("수서그룹삭제"));
	return ids;
}

INT CAcqOtherGroupDlg::CMD_GROUP_MERGE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	
	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(OTHER_ACQ_TYPE);
	GroupCollection.RemoveAll();
	GroupCollection.SetAcqCode(m_strAcqCode);
	SetSelGroupCollection(&GroupCollection);
	
	CMergeGroup_Select_Dlg dlg(this);
	dlg.SetDM(m_pDM);
	dlg.SetGroupType(OTHER_GROUP_NO);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetGroupCollection(&GroupCollection);
	
	if(IDOK == dlg.DoModal())	
	{
		CGroupPair mgr(this);
		mgr.SetTargetGroup(dlg.GetSelGroup());
		mgr.SetSourceGroupCollection(&GroupCollection);
		mgr.SetGroupType(OTHER_GROUP_NO);
		mgr.SetAcqCode(m_strAcqCode);
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

INT CAcqOtherGroupDlg::CMD_GROUP_DIVIDE()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0; 
	}
	COtherGroupNo_Divide_Dlg dlg(this);
	dlg.SetParentDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	
	::SendMessage(this->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	
	m_pGrid->Display();
	return 0;
}

INT CAcqOtherGroupDlg::CMD_GROUP_COPY()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	CString strMsg;
	ids = m_pGrid->SelectMakeList();
	INT i= m_pGrid->SelectGetHeadPosition();
	CGroupInfo SourceGroup, TargetGroup;
	CGroupPair mgr(this);
	mgr.SetGroupType(OTHER_GROUP_NO);
	mgr.SetMainDM(m_pDM);
	while(i >= 0)
	{
		if(ShowOtherGroupDlg(DINSERT) == 0)
		{
			SourceGroup.SetAcqYear(m_pDM->GetCellData(_T("그룹생성년도"), i));
			SourceGroup.SetGroupInfo(m_pDM->GetCellData(_T("그룹번호"), i));
			SourceGroup.SetGroupNo(m_pDM->GetCellData(_T("수입구분"), i));
			SourceGroup.SetLastSerialNo(_ttoi(m_pDM->GetCellData(_T("마지막일련번호"), i).GetBuffer(0)));
			
			TargetGroup.SetAcqYear(m_pDM->GetCellData(_T("그룹생성년도"), m_pDM->GetRowCount()-1));
			TargetGroup.SetGroupInfo(m_pDM->GetCellData(_T("그룹번호"), m_pDM->GetRowCount()-1));
			TargetGroup.SetGroupNo(m_pDM->GetCellData(_T("수입구분"), m_pDM->GetRowCount()-1));
			TargetGroup.SetLastSerialNo(_ttoi(m_pDM->GetCellData(_T("마지막일련번호"), m_pDM->GetRowCount()-1).GetBuffer(0)));
			
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
		
		i = m_pGrid->SelectGetNext();
	}
	m_pGrid->Display();
	return 0;
}

INT CAcqOtherGroupDlg::CMD_VIEW_GROUPDATA()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	COtherGroupNo_DataView_Dlg dlg(this);
	dlg.SetAcqCode(m_strAcqCode);
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();		
	return 0;
}
BOOL CAcqOtherGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서그룹관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_수서그룹관리"));
	m_pDM = FindDM(_T("DM_수서그룹번호관리"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	m_pSearchDlg = new CAcqGroupSearchDlg(this);
	m_pSearchDlg->SetAcqGroupType(ACQ_OTHER_GROUP);

	m_pGroupMgr = new CGroupMgr(this);

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(OTHER_ACQ_TYPE);
	
	GetDlgItem(IDC_stcGROUP_ACQ_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUP_ACQ_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcGROUP_ACQ_CODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtGROUP_ACQ_CODE)->ShowWindow(SW_HIDE);
	
	InitGroupInfo();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAcqOtherGroupDlg::OnSize(UINT nType, INT cx, INT cy) 
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

	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcGROUP_ACQ_CODE);
 	if(pBtn->GetSafeHwnd() == NULL) return;
 	pBtn->GetWindowRect(&rect);
 	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
 	pBtn->ShowWindow(SW_SHOW);
 	
 	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtGROUP_ACQ_CODE);
 	pEdt->GetWindowRect(&rect);
 	pEdt->MoveWindow(80, 7, rect.Width(), rect.Height());
 	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_stcGROUP_ACQ_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(192, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	pEdt = (CEdit*)GetDlgItem(IDC_edtGROUP_ACQ_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(252, 7, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	m_pGrid->MoveWindow(-2, 35, cx+2, cy);	
}

UINT CAcqOtherGroupDlg::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	strQuery = m_pSearchDlg->GetWhereStmt();
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("수서그룹정보를 검색할 수 없습니다."));
		return -1;
	}
	
	SetGroupInfo();
	
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
	
	m_pGrid->Display();
	return 0;
}
BEGIN_EVENTSINK_MAP(CAcqOtherGroupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAcqOtherGroupDlg)
	ON_EVENT(CAcqOtherGroupDlg, IDC_gridOTHER_GROUP, -601 /* DblClick */, OnDblClickgridOTHERGROUP, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAcqOtherGroupDlg::OnDblClickgridOTHERGROUP() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return;
	if(m_pDM->GetRowCount() == 0) return ;
	ShowOtherGroupDlg(GUPDATE);
}

INT CAcqOtherGroupDlg::ShowOtherGroupDlg(OPERATION_TYPE nOpType)
{
	COtherGroupNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetAcqCode(m_strAcqCode);
	dlg.SetAcqYear(m_strAcqYear);
	if(IDOK == dlg.DoModal()) return 0;
	return -1;
}

INT CAcqOtherGroupDlg::SetSelGroupCollection(CGroupCollection * pGroupCollection)
{
	CString strMsg, strAcqYear, strGroupInfo, strAcqCode, strSrcAcqCode;
	INT ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetHeadPosition();
	
	strSrcAcqCode = pGroupCollection->GetAcqCode();
	if(strSrcAcqCode.IsEmpty()) return -1;
	
	while(i>=0)
	{
		strAcqYear = m_pDM->GetCellData(_T("수입년도"), i);
		strGroupInfo = m_pDM->GetCellData(_T("그룹번호"), i);
		strAcqCode = m_pDM->GetCellData(_T("수입구분"), i);
		if(strSrcAcqCode != strAcqCode)
		{
			strMsg.Format(_T("%s-%s-%s 그룹번호는 통합될 수 없습니다."), strAcqYear, strAcqCode, strGroupInfo);
			ESLAfxMessageBox(strMsg);
		}
		else 
			pGroupCollection->AddGroupInfo(strAcqYear, strGroupInfo, strAcqCode);
		
		i = m_pGrid->SelectGetNext();
	}
	return 0;
}

INT CAcqOtherGroupDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("수입구분"))
	{	
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), str, str);
	}
	return 0;
}

VOID CAcqOtherGroupDlg::InitGroupInfo()
{
	m_strAcqYear = GetTodayYear();
	
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
			break;
		}
	}
	
	m_strAcqCode = code;
	
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pCM->SetControlMgrData(_T("수입구분"), desc);
	
	m_pSearchDlg->SetAcqYear(m_strAcqYear);
	m_pSearchDlg->SetAcqCode(m_strAcqCode);
}

VOID CAcqOtherGroupDlg::SetGroupInfo()
{
	m_strAcqYear = m_pSearchDlg->GetAcqYear();
	m_strAcqCode = m_pSearchDlg->GetAcqCode();
	
	CString strTmpData;
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), m_strAcqCode, strTmpData);
	m_pCM->SetControlMgrData(_T("수입구분"), strTmpData);
}

HBRUSH CAcqOtherGroupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
