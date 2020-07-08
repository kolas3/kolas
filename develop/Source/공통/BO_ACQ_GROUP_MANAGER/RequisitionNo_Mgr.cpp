// RequisitionNo_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "RequisitionNo_Mgr.h"
#include "RequisitionNo_Divide_Dlg.h"
#include "MergeGroup_Select_Dlg.h"
#include "GroupPair.h"
#include "RequisitionNo_Insert_Dlg.h"
#include "RequisitionNo_DataView_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_Mgr dialog


CRequisitionNo_Mgr::CRequisitionNo_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequisitionNo_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequisitionNo_Mgr)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_nOpenMode = -1;
	m_pGroupCollection = NULL;
}

CRequisitionNo_Mgr::~CRequisitionNo_Mgr()
{
}

VOID CRequisitionNo_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequisitionNo_Mgr)
	DDX_Radio(pDX, IDC_radREQNO_ALLDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequisitionNo_Mgr, CDialog)
	//{{AFX_MSG_MAP(CRequisitionNo_Mgr)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnREQNO_SEARCH, OnbtnREQNOSEARCH)
	ON_BN_CLICKED(IDC_btnREQNO_INSERT, OnbtnREQNOINSERT)
	ON_BN_CLICKED(IDC_btnREQNO_UPDATE, OnbtnREQNOUPDATE)
	ON_BN_CLICKED(IDC_btnREQNO_DELETE, OnbtnREQNODELETE)
	ON_BN_CLICKED(IDC_btnREQNO_DIVIDE, OnbtnREQNODIVIDE)
	ON_BN_CLICKED(IDC_btnREQNO_MERGE, OnbtnREQNOMERGE)
	ON_BN_CLICKED(IDC_btnREQNO_COPY, OnbtnREQNOCOPY)
	ON_BN_CLICKED(IDC_btnREQNO_INFO, OnbtnREQNOINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_Mgr message handlers

BOOL CRequisitionNo_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( InitESL_Mgr(_T("K2UPGRADE_접수번호관리")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_접수번호관리"));
	m_pDM = FindDM(_T("DM_접수번호관리"));
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("접수번호관리 화면을 초기화 할 수 없습니다."));
		return FALSE;
	}
	m_nMyData = 0;
	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

VOID CRequisitionNo_Mgr::OnClose() 
{
	CDialog::OnClose();
}

INT CRequisitionNo_Mgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	return 0;
}

VOID CRequisitionNo_Mgr::DisplayGrid(CString gridAlias)
{
	CESL_Grid* pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if( pGrid == NULL )		return;
	pGrid->Display();
}

BOOL CRequisitionNo_Mgr::Init()
{
	switch(m_nOpenMode)
	{
	case 0:
		break;
	case 1:
		GetDlgItem(IDC_btnREQNO_INSERT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_UPDATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_MERGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_DIVIDE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_COPY)->ShowWindow(SW_HIDE);
		break;
	case 2:
		GetDlgItem(IDC_btnREQNO_INSERT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_UPDATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_MERGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_DIVIDE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnREQNO_COPY)->ShowWindow(SW_HIDE);
		break;
	default :
		return FALSE;
	}

	return TRUE;
}
VOID CRequisitionNo_Mgr::OnbtnREQNOSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strCreator, strAcqYear, strQuery, strOption;
	CStringArray ArrayOption;	ArrayOption.RemoveAll();
	UpdateData(TRUE);
	if(m_nMyData == 1) strCreator = m_pInfo->USER_NAME;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	if(!strCreator.IsEmpty()) 
	{
		strOption.Format(_T("RECEIPT_NO_CREATE_WORKER = '%s'"), strCreator);
		ArrayOption.Add(strOption);
	}
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T("ACQ_YEAR = '%s'"), strAcqYear);
		ArrayOption.Add(strOption);
	}
	INT cnt = ArrayOption.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strQuery.IsEmpty()) strQuery = ArrayOption.GetAt(i);
		else strQuery += _T(" AND ") + ArrayOption.GetAt(i);
	}
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("접수번호 정보를 구축할 수 없습니다."));
		return;
	}

	CGroupMgr mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempData(_T("조건"), strQuery.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	mgr.SetTempData(_T("SPECIES_TYPE"), (INT)SPECIES_TYPE_INVALID);
	mgr.SetTempData(_T("ACQ_YEAR"), strAcqYear.GetBuffer(0));
	ids = mgr.SPFExecutePath(_T("접수번호책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return ;
	}
	
	DisplayGrid(_T("MainGrid"));		
}

VOID CRequisitionNo_Mgr::OnbtnREQNOINSERT() 
{
	// TODO: Add your control notification handler code here
	if(InsertReceiptNoInfo(SINSERT) == 0)
		DisplayGrid(_T("MainGrid"));	
}

VOID CRequisitionNo_Mgr::OnbtnREQNOUPDATE() 
{
	// TODO: Add your control notification handler code here
	if(m_pDM->GetRowCount() == 0)	
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}
	UpdateReceiptNo(SUPDATE);
}

VOID CRequisitionNo_Mgr::OnbtnREQNODELETE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	
	CESL_DataMgr * pDM_DELETE = FindDM(_T("DM_접수번호삭제"));
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("DELETE_DM"), (DWORD*)pDM_DELETE);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	ids = mgr.SPFExecutePath(_T("접수번호삭제"));
	if(ids < 0)
	{
		return;
	}
	else if(ids == 1000)
	{
		ESLAfxMessageBox(_T("삭제될 접수번호 정보가 없습니다."));
		return;
	}
	else if(ids == 0) DisplayGrid(_T("MainGrid"));	
}

VOID CRequisitionNo_Mgr::OnbtnREQNODIVIDE() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CRequisitionNo_Divide_Dlg dlg(this);
	dlg.SetParentDM(m_pDM);
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	dlg.SetParentGrid(pGrid);
	dlg.DoModal();

	OnbtnREQNOSEARCH();
	
	DisplayGrid(_T("MainGrid"));
}

INT CRequisitionNo_Mgr::CheckSelectDataValid()
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

VOID CRequisitionNo_Mgr::OnbtnREQNOMERGE() 
{
	INT ids;
	CString strAcqYear;

	ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);

	CGroupCollection GroupCollection(this);
	GroupCollection.SetAcqType(DONATE);
	GroupCollection.RemoveAll();
	SetSelGroupCollection(&GroupCollection);
	
	CMergeGroup_Select_Dlg dlg(this);
	dlg.SetDM(m_pDM);
	dlg.SetGroupType(RECEIPT_NO);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetGroupCollection(&GroupCollection);
	
	if( IDOK == dlg.DoModal() )
	{
		CGroupPair mgr(this);
		mgr.SetTargetGroup(dlg.GetSelGroup());
		mgr.SetSourceGroupCollection(&GroupCollection);
		mgr.SetGroupType(RECEIPT_NO);
		mgr.SetMainDM(m_pDM);
		ids = mgr.MergeGroup();
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("선정한 자료를 통합할 수 업습니다."));
			return ;
		}
		DisplayGrid(_T("MainGrid"));
	}
}

VOID CRequisitionNo_Mgr::OnbtnREQNOCOPY() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	
	ESLAfxMessageBox(_T("등록번호가 부여된 자료는 복사되지 않습니다."));

	CString strMsg;
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	ids = pGrid->SelectMakeList();
	INT i= pGrid->SelectGetHeadPosition();
	CGroupInfo SourceGroup, TargetGroup;
	CGroupPair mgr(this);
	mgr.SetMainDM(m_pDM);
	mgr.SetGroupType(RECEIPT_NO);
	while(i >= 0)
	{
		if(InsertReceiptNoInfo(DINSERT) == 0)
		{
			SetGroupInfo(&SourceGroup, i);
			SetGroupInfo(&TargetGroup, m_pDM->GetRowCount()-1);
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

		i = pGrid->SelectGetNext();
	}
	DisplayGrid(_T("MainGrid"));
}

INT CRequisitionNo_Mgr::SetSelGroupCollection(CGroupCollection *pGroupCollection)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	while(i>=0)
	{
		pGroupCollection->AddGroupInfo(m_pDM->GetCellData(_T("수입년도"), i), m_pDM->GetCellData(_T("접수번호"), i), m_pDM->GetCellData(_T("그룹번호"), i));
		i = pGrid->SelectGetNext();
	}
	return 0;
}

INT CRequisitionNo_Mgr::InsertReceiptNoInfo(OPERATION_TYPE nOpType)
{
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	CRequisitionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetMainCM(m_pCM);
	if(IDOK == dlg.DoModal()) return 0;
	return -1;
}

INT CRequisitionNo_Mgr::SetGroupInfo(CGroupInfo *pGroup, INT idx)
{
	pGroup->SetAcqYear(m_pDM->GetCellData(_T("수입년도"), idx));
	pGroup->SetGroupInfo(m_pDM->GetCellData(_T("접수번호"), idx));
	return 0;
}

BEGIN_EVENTSINK_MAP(CRequisitionNo_Mgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CRequisitionNo_Mgr)
	ON_EVENT(CRequisitionNo_Mgr, IDC_gridREQNO_LIST, -601 /* DblClick */, OnDblClickgridREQNOLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CRequisitionNo_Mgr::OnDblClickgridREQNOLIST() 
{
	// TODO: Add your control notification handler code here
	UpdateReceiptNo(GUPDATE);
}

VOID CRequisitionNo_Mgr::UpdateReceiptNo(OPERATION_TYPE nOpType)
{
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	CRequisitionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetMainCM(m_pCM);
	dlg.DoModal();
}

VOID CRequisitionNo_Mgr::OnbtnREQNOINFO() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CRequisitionNo_DataView_Dlg dlg(this);
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();		
}

BOOL CRequisitionNo_Mgr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_edtREQNO_ACQ_YEAR)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnREQNO_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_radREQNO_MYDATA)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnREQNO_SEARCH, BN_CLICKED);
		else if (GetDlgItem(IDC_radREQNO_ALLDATA)->m_hWnd == pMsg->hwnd)
			PostMessage(WM_COMMAND, IDC_btnREQNO_SEARCH, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CRequisitionNo_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
