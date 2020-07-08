// BO_DeleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_DeleteDlg.h"
#include "../BO_ACQ_GROUP_MANAGER/Group_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_DeleteDlg dialog


CBO_DeleteDlg::CBO_DeleteDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_DeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_DeleteDlg)
	//}}AFX_DATA_INIT
	m_pMainDM = NULL;
	m_pMainGrid = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_pArrSpeciesKey = NULL;
	m_pArrAcqKey = NULL;
	m_pArrVolSortNo = NULL;	
	m_nDeleteOption = 0;
	m_nDeleteType = SPECIES_DELETE;
	m_pDeleteApi = NULL;
	m_bDeleteFlag = FALSE;

	m_pMainBookDM = NULL;
	m_strBookCntAlias = _T("");
	m_strPriceAlias = _T("");	
}

CBO_DeleteDlg::~CBO_DeleteDlg()
{
	if(m_arrParentIdx.GetSize() > 0)
	{
		CString strDelOption;
		if(m_nDeleteOption < 0) 
		{
			strDelOption = _T("중복자료삭제");
			m_pDeleteApi->DeleteSelectRow(m_pMainDM, &m_arrParentIdx, strDelOption);
		}
		else 
			m_pDeleteApi->DeleteSelectRowAndBookDM(m_pMainDM, m_pMainBookDM, &m_arrParentIdx, m_nDeleteType);
	}
	if(m_pDeleteApi)
	{
		delete m_pDeleteApi;
		m_pDeleteApi= NULL;
	}
}

VOID CBO_DeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_DeleteDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_DeleteDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_DeleteDlg)
	ON_BN_CLICKED(IDC_btnDB_DELETE, OnbtnDBDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_DeleteDlg message handlers
BOOL CBO_DeleteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_삭제자료확인")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_삭제자료확인"));
	if(m_nDeleteType == SPECIES_DELETE)
	{
		m_pDM = FindDM(_T("DM_삭제자료확인_종"));
		if(m_nDeleteOption > 0)
		{
			m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Species"));
			GetDlgItem(IDC_gridDELETE_DATA_VOLUME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_DUPLICATE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_BOOK)->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Duplicate"));
			GetDlgItem(IDC_gridDELETE_DATA_SPECIES)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_VOLUME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_BOOK)->ShowWindow(SW_HIDE);
		}
	}
	else if(m_nDeleteType == VOLUME_DELETE)
	{
		m_pDM = FindDM(_T("DM_삭제자료확인_권"));
		m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Volume"));
		GetDlgItem(IDC_gridDELETE_DATA_SPECIES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_DUPLICATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_BOOK)->ShowWindow(SW_HIDE);
	}
	else if(m_nDeleteType == BOOK_DELETE)
	{
		m_pDM = FindDM(_T("DM_삭제자료확인_책"));
		m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Book"));
		GetDlgItem(IDC_gridDELETE_DATA_SPECIES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_DUPLICATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_VOLUME)->ShowWindow(SW_HIDE);
	}
	if(!Init())
	{
		ESLAfxMessageBox(_T("삭제자료확인 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	m_pDeleteApi = new CBO_ACQ_Business_Api(this);
	
	if(m_nSpeciesType == BO_IMPORT || m_nSpeciesType == BO_BASIC)
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_DeleteDlg::DisplayGrid()
{
	INT ids = m_pDM->GetRowCount();
	CString strMsg;
	strMsg.Format(_T("%d건의 자료가 선택되었습니다."), ids);
	GetDlgItem(IDC_stcDELETE_INFO)->SetWindowText(strMsg);
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

BOOL CBO_DeleteDlg::Init()
{
	CString strTmpData;
	if(m_pMainDM == NULL || m_pMainGrid == NULL) return FALSE;
	CString strAlias_Speices[6];
	CString strAlias_Volume[8];
	CString strAlias_Book[8];
	CString strAlias_Extra[2];

	CString strData_Speices[6];
	CString strData_Volume[8];
	CString strData_Book[8];
	CString strData_Extra[2];
	
	strAlias_Speices[0] = _T("IBS_본표제");	strAlias_Speices[1] = _T("IBS_저작자");	strAlias_Speices[2] = _T("IBS_발행자");
	strAlias_Speices[3] = _T("IBS_발행년");	strAlias_Speices[4] = _T("IBS_종KEY");	strAlias_Speices[5] = _T("BP_수서정보KEY");
	strAlias_Extra[0] = m_strPriceAlias;	strAlias_Extra[1] = m_strBookCntAlias;
	strAlias_Volume[0] = _T("IBS_본표제");	strAlias_Volume[1] = _T("IBS_저작자");	strAlias_Volume[2] = _T("IBS_발행자");
	strAlias_Volume[3] = _T("IBS_발행년");	strAlias_Volume[4] = _T("IBS_종KEY");	strAlias_Volume[5] = _T("BP_수서정보KEY");
	strAlias_Volume[6] = _T("BB_권일련번호");	strAlias_Volume[7] = _T("BB_편권차");
	strAlias_Book[0] = _T("IBS_본표제");	strAlias_Book[1] = _T("IBS_저작자");	strAlias_Book[2] = _T("IBS_발행자");
	strAlias_Book[3] = _T("IBS_발행년");	strAlias_Book[4] = _T("IBS_종KEY");		strAlias_Book[5] = _T("BP_수서정보KEY");
	strAlias_Book[6] = _T("BB_편권차");		strAlias_Book[7] = _T("BB_책KEY");

	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	INT nCurRow;
	while(ids >= 0)
	{
		m_pDM->InsertRow(-1);
		nCurRow = m_pDM->GetRowCount()-1;
		if(m_nDeleteType == SPECIES_DELETE)
		{
			m_pMainDM->GetCellData(strAlias_Speices, 6, strData_Speices, ids);
			m_pDM->SetCellData(strAlias_Speices, 6, strData_Speices, nCurRow);
		}
		else if(m_nDeleteType == VOLUME_DELETE)
		{
			m_pMainDM->GetCellData(strAlias_Volume, 8, strData_Volume, ids);
			m_pDM->SetCellData(strAlias_Volume, 8, strData_Volume, nCurRow);
		}		
		else if(m_nDeleteType == BOOK_DELETE)
		{
			m_pMainDM->GetCellData(strAlias_Book, 8, strData_Book, ids);
			m_pDM->SetCellData(strAlias_Book, 8, strData_Book, nCurRow);
		}
		
		m_pMainDM->GetCellData(strAlias_Extra, 2, strData_Extra, ids);
		m_pDM->SetCellData(_T("가격"), strData_Extra[0], nCurRow);
		m_pDM->SetCellData(_T("책수"), strData_Extra[1], nCurRow);
		strTmpData.Format(_T("%d"), ids);
		m_pDM->SetCellData(_T("부모인덱스"), strTmpData, nCurRow);
		ids = m_pMainGrid->SelectGetNext();
	}
	DisplayGrid();
	ids = m_pDM->GetRowCount();
	for(INT i=0 ; i<ids ; i++)
	{
		m_pGrid->SetAt(i, 0, _T("V"));
	}
	
	return TRUE;
}

VOID CBO_DeleteDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = -1;

	CGroupInfo pTargetGroup;
	pTargetGroup.SetAcqYear(m_pDeleteApi->GetTodayYear());
	pTargetGroup.SetGroupInfo(_T(""));
	pTargetGroup.SetGroupNo(_T("0"));

	CGroup_Select_Dlg dlg(this);
	dlg.SetSpeciesType(BO_RECYCLE);
	dlg.SetAcqYear(m_pDeleteApi->GetTodayYear());

	if(IDCANCEL == dlg.DoModal())
		return ;
		
	pTargetGroup.SetAcqYear(dlg.GetSelGroup()->GetAcqYear());
	pTargetGroup.SetGroupInfo(dlg.GetSelGroup()->GetGroupInfo());
	pTargetGroup.SetGroupNo(dlg.GetSelGroup()->GetGroupNo());

	if(m_nDeleteType == SPECIES_DELETE)
		ids = DeteleSpeciesData(TRUE, &pTargetGroup);
	else if(m_nDeleteType == VOLUME_DELETE)
		ids = DeleteVolumeData(TRUE, &pTargetGroup);
	else if(m_nDeleteType == BOOK_DELETE)
		ids = DeleteBookData(TRUE, &pTargetGroup);

	CString strDelOption;
	if(m_nDeleteOption > 0) strDelOption = _T("");
	else if(m_nDeleteOption < 0) strDelOption = _T("중복자료삭제");
	if(m_arrIdx.GetSize() > 0) m_bDeleteFlag = TRUE;
	MakeParentDeleteRowArray();
	m_pDeleteApi->DeleteSelectRow(m_pDM, &m_arrIdx, strDelOption);
	
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정하신 자료를 삭제할 수 없습니다."));
		return;
	}	
	else if(ids == 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제하였습니다."));
	}
	DisplayGrid();	
	m_arrIdx.RemoveAll();
}

VOID CBO_DeleteDlg::OnbtnDBDELETE() 
{
	// TODO: Add your control notification handler code here
	INT ids = -1;
	if(m_nDeleteType == SPECIES_DELETE)
		ids = DeteleSpeciesData(FALSE);
	else if(m_nDeleteType == VOLUME_DELETE)
		ids = DeleteVolumeData(FALSE);
	else if(m_nDeleteType == BOOK_DELETE)
		ids = DeleteBookData(FALSE);

	CString strDelOption;
	if(m_nDeleteOption > 0) strDelOption = _T("");
	else if(m_nDeleteOption < 0) strDelOption = _T("중복자료삭제");
	if(m_arrIdx.GetSize() > 0) m_bDeleteFlag = TRUE;
	MakeParentDeleteRowArray();
	m_pDeleteApi->DeleteSelectRow(m_pDM, &m_arrIdx, strDelOption);

	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정하신 자료를 삭제할 수 없습니다."));
		return;
	}	
	else if(ids == 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제하였습니다."));
	}
	DisplayGrid();	
	m_arrIdx.RemoveAll();
}

INT CBO_DeleteDlg::DeteleSpeciesData(BOOL bRecycleYN, CGroupInfo * pGroup/*=NULL*/)
{
	m_pDeleteApi->InitTempData();
	m_pDeleteApi->InitTempPtr();
	m_pDeleteApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDeleteApi->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pDeleteApi->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pDeleteApi->SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_pDeleteApi->SetTempData(_T("종키알리아스"), _T("IBS_종KEY"));
	m_pDeleteApi->SetTempData(_T("수서정보키알리아스"), _T("BP_수서정보KEY"));
	m_pDeleteApi->SetTempData(_T("재활용처리여부"), bRecycleYN);
	m_pDeleteApi->SetTempPtr(_T("삭제인덱스"), (DWORD*)&m_arrIdx);
	m_pDeleteApi->SetTempPtr(_T("종키리스트"), (DWORD*)m_pArrSpeciesKey);
	m_pDeleteApi->SetTempPtr(_T("수서정보키리스트"), (DWORD*)m_pArrAcqKey);
	m_pDeleteApi->SetTempPtr(_T("그룹정보"), (DWORD*)pGroup);
	INT ids = m_pDeleteApi->SPFExecutePath(_T("종삭제"));

	return ids;
}

INT CBO_DeleteDlg::DeleteVolumeData(BOOL bRecycleYN, CGroupInfo * pGroup/*=NULL*/)
{
	m_pDeleteApi->InitTempData();
	m_pDeleteApi->InitTempPtr();
	m_pDeleteApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDeleteApi->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pDeleteApi->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pDeleteApi->SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_pDeleteApi->SetTempData(_T("종키알리아스"), _T("IBS_종KEY"));
	m_pDeleteApi->SetTempData(_T("수서정보키알리아스"), _T("BP_수서정보KEY"));
	m_pDeleteApi->SetTempData(_T("권일련번호알리아스"), _T("BB_권일련번호"));
	m_pDeleteApi->SetTempData(_T("재활용처리여부"), bRecycleYN);
	m_pDeleteApi->SetTempPtr(_T("삭제인덱스"), (DWORD*)&m_arrIdx);
	m_pDeleteApi->SetTempPtr(_T("종키리스트"), (DWORD*)m_pArrSpeciesKey);
	m_pDeleteApi->SetTempPtr(_T("수서정보키리스트"), (DWORD*)m_pArrAcqKey);
	m_pDeleteApi->SetTempPtr(_T("권일련번호리스트"), (DWORD*)m_pArrVolSortNo);
	m_pDeleteApi->SetTempPtr(_T("그룹정보"), (DWORD*)pGroup);

	INT ids;
	if(m_nSpeciesType == (INT)BO_MISSING)
		ids = m_pDeleteApi->SPFExecutePath(_T("미납자료삭제"));
	else
		ids = m_pDeleteApi->SPFExecutePath(_T("권삭제"));

	return ids;
}

INT CBO_DeleteDlg::MakeParentDeleteRowArray()
{
	INT nTmpData;
	INT nCnt = m_arrIdx.GetSize();

	CString strTmpData;
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpData = m_pDM->GetCellData(_T("부모인덱스"), m_arrIdx.GetAt(i));
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		AddParentIdxBySort(&m_arrParentIdx, nTmpData);
	}
	return 0;
}

INT CBO_DeleteDlg::AddParentIdxBySort(CArray<INT, INT> * pArrParentIdx, INT nData)
{
	INT nTmpData;
	INT nCnt = pArrParentIdx->GetSize();
	if(nCnt == 0)
	{
		pArrParentIdx->Add(nData);
		return 0;
	}
	for(INT i=0 ; i<nCnt ; i++)
	{
		nTmpData = pArrParentIdx->GetAt(i);
		if(nTmpData > nData)
		{
			pArrParentIdx->SetAtGrow(i, nData);
			return 0;
		}
	}
	pArrParentIdx->Add(nData);
	return 0;
}

INT CBO_DeleteDlg::DeleteBookData(BOOL bRecycleYN, CGroupInfo * pGroup/*=NULL*/)
{
	m_pDeleteApi->InitTempData();
	m_pDeleteApi->InitTempPtr();
	m_pDeleteApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDeleteApi->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pDeleteApi->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pDeleteApi->SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_pDeleteApi->SetTempData(_T("종키알리아스"), _T("IBS_종KEY"));
	m_pDeleteApi->SetTempData(_T("수서정보키알리아스"), _T("BP_수서정보KEY"));
	m_pDeleteApi->SetTempData(_T("책키알리아스"), _T("BB_책KEY"));
	m_pDeleteApi->SetTempData(_T("재활용처리여부"), bRecycleYN);
	m_pDeleteApi->SetTempPtr(_T("삭제인덱스"), (DWORD*)&m_arrIdx);
	m_pDeleteApi->SetTempPtr(_T("종키리스트"), (DWORD*)m_pArrSpeciesKey);
	m_pDeleteApi->SetTempPtr(_T("수서정보키리스트"), (DWORD*)m_pArrAcqKey);
	m_pDeleteApi->SetTempPtr(_T("권일련번호리스트"), (DWORD*)m_pArrVolSortNo);
	m_pDeleteApi->SetTempPtr(_T("그룹정보"), (DWORD*)pGroup);
	INT ids = m_pDeleteApi->SPFExecutePath(_T("책삭제"));

	return ids;
}

HBRUSH CBO_DeleteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
