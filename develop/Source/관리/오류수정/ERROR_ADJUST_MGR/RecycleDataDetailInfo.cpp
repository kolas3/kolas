// RecycleDataDetailInfo.cpp : implementation file
//

#include "stdafx.h"
#include "RecycleDataDetailInfo.h"
#include "AccessionNoSearchDlg.h"
#include "SetUpRegNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataDetailInfo dialog


CRecycleDataDetailInfo::CRecycleDataDetailInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRecycleDataDetailInfo::IDD, pParent)
{
	AfxInitRichEdit();
	//{{AFX_DATA_INIT(CRecycleDataDetailInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGrid_Book = NULL;
	m_pDM_INDEX = NULL;
	m_pDM_SPECIES = NULL;
	m_pDM_BOOK = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_pApi = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_nOpenMode = 0;
	m_bGridSelectYN = FALSE;
}

CRecycleDataDetailInfo::~CRecycleDataDetailInfo()
{
	
}


VOID CRecycleDataDetailInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecycleDataDetailInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecycleDataDetailInfo, CDialog)
	//{{AFX_MSG_MAP(CRecycleDataDetailInfo)
	ON_BN_CLICKED(IDC_btnDVIEWER_PREV, OnbtnDVIEWERPREV)
	ON_BN_CLICKED(IDC_btnDVIEWER_NEXT, OnbtnDVIEWERNEXT)
	ON_BN_CLICKED(IDC_btnSET_ACCESSION_NO, OnbtnSETACCESSIONNO)
	ON_BN_CLICKED(IDC_btnMARC_UPDATE, OnbtnMARCUPDATE)
	ON_BN_CLICKED(IDC_btnRESTORE, OnbtnRESTORE)
	ON_BN_CLICKED(IDC_btnSET_REG_NO, OnbtnSETREGNO)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecycleDataDetailInfo message handlers

BOOL CRecycleDataDetailInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_재활용복원상세정보")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_재활용자료_상세보기"));
	m_pDM_INDEX = FindDM(_T("DM_재활용자료_색인정보"));
	m_pDM_SPECIES = FindDM(_T("DM_재활용자료_종정보"));
	m_pDM_BOOK = FindDM(_T("DM_재활용자료_책정보"));
	m_pGrid_Book = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Book"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("재활용자료 상세보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	Display(0);	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRecycleDataDetailInfo::Init()
{
	if(m_pParentDM == NULL || m_pParentDM->GetRowCount() == 0) return FALSE;
	if(m_pParentGrid == NULL) return FALSE;
	if(m_strSpeciesKeyAlias.IsEmpty()) m_strSpeciesKeyAlias = _T("IBS_종KEY");
	
	m_nIdx = m_pParentGrid->GetRow() - 1;
	if(m_nIdx < 0) m_nIdx = 0;
	
	return TRUE;
}

VOID CRecycleDataDetailInfo::Display(INT idx)
{
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = GetNextSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		return ;
	}
	SetDetailInfo();
	m_marcEditor.Display();
	m_pCM->AllControlDisplay(0);
	m_pGrid_Book->Display();
	SetWorkingStatusDesc();
	m_pParentGrid->SetReverse(m_nIdx);
	m_pGrid_Book->SetReverse(0);
}

VOID CRecycleDataDetailInfo::SetWorkingStatusDesc()
{
	CString strWorkStatus = m_pDM_INDEX->GetCellData(_T("IBS_작업상태INFO"), 0);
	strWorkStatus = strWorkStatus.Left(3);
	if(strWorkStatus == _T("BOA")) strWorkStatus = _T("수서자료");
	else if(strWorkStatus == _T("BOR")) strWorkStatus = _T("등록자료");
	else if(strWorkStatus == _T("BOC")) strWorkStatus = _T("정리자료");
	else if(strWorkStatus == _T("BOL")) strWorkStatus = _T("소장자료");
	else strWorkStatus = _T("수서자료");
	
	m_pCM->SetControlMgrData(_T("IBS_자료상태INFO"), strWorkStatus);
}

VOID CRecycleDataDetailInfo::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	ids = m_pParentGrid->SelectGetHeadPosition();

	if(m_pParentDM->GetRowCount() == 0)
	{
		EndDialog(IDCANCEL);
		return;
	}
	
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pParentDM->GetRowCount();
		m_nFirstIdx = 0;
		m_nLastIdx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
		m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	}
}

INT CRecycleDataDetailInfo::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;

	if(m_nOpenMode == 2000 || m_nOpenMode == -2000)
	{
		nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == 1000 || m_nOpenMode == -1000)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nFirstIdx;
		else
			nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CRecycleDataDetailInfo::GetPrevSelectIndex()
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pParentGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pParentGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nFirstIdx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CRecycleDataDetailInfo::GetNextSelectIndex()
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pParentGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pParentGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nLastIdx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}

VOID CRecycleDataDetailInfo::ShowNextPrevButton(INT idx)
{
	CButton * pBtn1 = (CButton * )GetDlgItem(IDC_btnDVIEWER_PREV);
	CButton * pBtn2 = (CButton * )GetDlgItem(IDC_btnDVIEWER_NEXT);
	pBtn1->EnableWindow(TRUE);
	pBtn2->EnableWindow(TRUE);
	
	if((m_nFirstIdx >= idx || idx < 0) && pBtn1)
		pBtn1->EnableWindow(FALSE);
	if((m_nLastIdx <= idx || idx < 0) && pBtn2)
		pBtn2->EnableWindow(FALSE);
}

INT CRecycleDataDetailInfo::SetDetailInfo()
{
	//색인/종/책 정보 설정
	CString strQuery, strSpeciesKey;
	strSpeciesKey = m_pParentDM->GetCellData(m_strSpeciesKeyAlias, m_nIdx);
	strQuery.Format(_T("REC_KEY=%s"), strSpeciesKey);
	m_pDM_INDEX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	strQuery.Format(_T("SPECIES_KEY=%s ORDER BY VOL_SORT_NO"), strSpeciesKey);
	m_pDM_BOOK->RefreshDataManager(strQuery);

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BB_편권차");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pDM_BOOK->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	SetMarcInfoAndCtrl();
	return 0;
}

INT CRecycleDataDetailInfo::SetMarcInfoAndCtrl()
{
	//마크와 그리드 컨트롤 연결
	m_strStreamMarc = m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);
	m_marcEditor.SubclassDlgItem(IDC_edtMARC_INFO, -1, this);
	m_marcEditor.InitUserAlias();
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	m_pApi->RestructBookBy049MarcTag(m_pDM_BOOK, &m_marc);

	return 0;
}

VOID CRecycleDataDetailInfo::OnbtnDVIEWERPREV() 
{
	// TODO: Add your control notification handler code here
	m_strAccessionNo.Empty();
	m_strAccessionRecKey.Empty();
	Display(-1);	
}

VOID CRecycleDataDetailInfo::OnbtnDVIEWERNEXT() 
{
	// TODO: Add your control notification handler code here
	m_strAccessionNo.Empty();
	m_strAccessionRecKey.Empty();
	Display(1);
}

VOID CRecycleDataDetailInfo::OnbtnSETACCESSIONNO() 
{
	// TODO: Add your control notification handler code here
	CAccessionNoSearchDlg dlg(this);
	if(IDCANCEL == dlg.DoModal())
		return;

	m_strAccessionNo = dlg.GetAccessionNo();
	m_strAccessionRecKey = dlg.GetAccessionRecKey();
	
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_원부번호"), m_strAccessionNo, i);
		m_pDM_BOOK->SetCellData(_T("BB_원부번호KEY"), m_strAccessionRecKey, i);
	}
	m_pGrid_Book->Display();
}

VOID CRecycleDataDetailInfo::OnbtnMARCUPDATE() 
{
	// TODO: Add your control notification handler code here
	if(m_pApi == NULL) return;

	INT ids = m_pApi->Restruct049MarcTag(m_pDM_BOOK, &m_marc);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("049마크를 갱신할 수 없습니다."));
		return;
	}
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strMarc);
	m_pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, 0);
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();
	m_pGrid_Book->Display();
}

VOID CRecycleDataDetailInfo::OnbtnRESTORE() 
{
	// TODO: Add your control notification handler code here
	if(m_pApi == NULL)	 return;

	m_pApi->InitTempData();	m_pApi->InitTempPtr();
	m_pApi->SetTempPtr(_T("색인DM"), (DWORD*)this->m_pDM_INDEX);
	m_pApi->SetTempPtr(_T("종DM"), (DWORD*)this->m_pDM_SPECIES);
	m_pApi->SetTempPtr(_T("책DM"), (DWORD*)this->m_pDM_BOOK);
	INT ids = m_pApi->SPFExecutePath(_T("재활용자료복원"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정하신 재활용자료를 복원할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정하신 재활용자료를 복원 하였습니다."));
	m_pParentDM->DeleteRow(m_nIdx);
	m_pParentGrid->RemoveAt(m_nIdx);
	Display(0);
}

VOID CRecycleDataDetailInfo::OnbtnSETREGNO() 
{
	// TODO: Add your control notification handler code here
	CSetUpRegNoDlg dlg(this);
	dlg.SetDM(m_pDM_BOOK);
	dlg.SetDM_INDEX(m_pDM_INDEX);
	dlg.SetApi(m_pApi);
	dlg.DoModal();
}


HBRUSH CRecycleDataDetailInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}




















