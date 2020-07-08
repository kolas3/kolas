// CheckHoldDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckHoldDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckHoldDataDlg dialog


CCheckHoldDataDlg::CCheckHoldDataDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckHoldDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckHoldDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pGroupCollection = NULL;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_bOccurError = TRUE;
}

CCheckHoldDataDlg::~CCheckHoldDataDlg()
{
}

VOID CCheckHoldDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckHoldDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckHoldDataDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckHoldDataDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckHoldDataDlg message handlers

BOOL CCheckHoldDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_소장자료확인")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_소장자료확인"));
	m_pDM = FindDM(_T("DM_소장자료확인"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));

	if(!Init())
	{
		ESLAfxMessageBox(_T("그룹내 소장자료 확인 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCheckHoldDataDlg::Init()
{
	if(m_pGroupCollection == NULL)
		return FALSE;
	m_nAcqType = m_pGroupCollection->GetAcqType();
	InitCtrl();
	INT ids = CheckHoldData();
	if(ids < 0)
		return FALSE;
	return TRUE;
}

INT CCheckHoldDataDlg::CheckHoldData()
{
	INT nGroupCount = m_pGroupCollection->GetGroupCount();
	if(nGroupCount == 0) return 0;

	INT ids;
	m_pDM->FreeData();
	CGroupInfo * pGroupInfo = NULL;
	for(INT i=0 ; i<nGroupCount ; i++)
	{
		pGroupInfo = m_pGroupCollection->GetGroupInfo(i);
		if(pGroupInfo == NULL) continue;
		ids = MakeHoldDataInfo(m_pDM, pGroupInfo);
		if(ids < 0) return -1;
	}

	CString strTmpData;
	for(i=0 ; i<m_pDM->GetRowCount() ; i++)
	{
		m_pGrid->DisplayLine(i);
		strTmpData = m_pDM->GetCellData(_T("수서이외종수"), i);
		if(!strTmpData.IsEmpty() && strTmpData != _T("0"))
			SetGridRowBackColor(m_pGrid, i);
	}
	m_pGrid->DisplayNumber();
	
	return 0;
}

INT CCheckHoldDataDlg::MakeHoldDataInfo(CESL_DataMgr *pDM, CGroupInfo *pGroupInfo)
{
	if(pDM == NULL || pGroupInfo == NULL) return -1;
	
	INT ids = -1;
	if(m_nAcqType == ACQ_TYPE_INVALID) return 0;
	else if(m_nAcqType == PURCHASE)
		ids = MakePurchaseHoldDataInfo(pDM, pGroupInfo);
	else if(m_nAcqType == DONATE)
		ids = MakeDonateHoldDataInfo(pDM, pGroupInfo);
	else if(m_nAcqType == OTHER_ACQ_TYPE)
		ids = MakeOtherGroupDataInfo(pDM, pGroupInfo, m_pGroupCollection->GetAcqCode());

	return ids;
}

INT CCheckHoldDataDlg::MakePurchaseHoldDataInfo(CESL_DataMgr *pDM, CGroupInfo *pGroupInfo)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strQuery, strWorkCode, strGroupInfo;
	strWorkCode = pGroupInfo->GetWorkCode();
	strGroupInfo = pGroupInfo->GetGroupInfo();
	if(strWorkCode.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT I.REC_KEY, B.REC_KEY, B.WORKING_STATUS FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE ")
			_T("I.REC_KEY = B.SPECIES_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_YEAR='%s' AND ")
			_T("B.SEQ_NO=%s AND B.ACQ_CODE='1' AND I.WORK_CODE IS NULL"),
			pGroupInfo->GetAcqYear(), strGroupInfo);
	}
	else
	{
		strQuery.Format(
			_T("SELECT I.REC_KEY, B.REC_KEY, B.WORKING_STATUS FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE ")
			_T("I.REC_KEY = B.SPECIES_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_YEAR='%s' AND ")
			_T("B.SEQ_NO=%s AND B.ACQ_CODE='1' AND I.WORK_CODE='%s'"),
			pGroupInfo->GetAcqYear(), strGroupInfo, pGroupInfo->GetWorkCode());
	}

	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	CMapStringToString SpeciesMap;
	CMapStringToString HoldSpeciesMap;
	SpeciesMap.RemoveAll();
	HoldSpeciesMap.RemoveAll();
	INT nHoldBookCnt = 0;
	CString strWorkingStatus, strSpeciesKey, strTmpData;
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strWorkingStatus = tmpDM.GetCellData(i, 2);
		strSpeciesKey = tmpDM.GetCellData(i, 0);
		if(strWorkingStatus.Left(3) != _T("BOA"))
		{
			nHoldBookCnt++;
			HoldSpeciesMap.SetAt(strSpeciesKey, _T(""));
		}
		SpeciesMap.SetAt(strSpeciesKey, _T(""));
	}
	
	m_pDM->InsertRow(-1);
	INT nCurIdx = m_pDM->GetRowCount()-1;
	m_pDM->SetCellData(_T("수입년도"), pGroupInfo->GetAcqYear(), nCurIdx);
	m_pDM->SetCellData(_T("그룹번호"), strGroupInfo, nCurIdx);
	m_pDM->SetCellData(_T("업무구분"), strWorkCode, nCurIdx);
	strTmpData.Format(_T("%d"), SpeciesMap.GetCount());
	m_pDM->SetCellData(_T("종수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), nRowCnt);
	m_pDM->SetCellData(_T("책수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), HoldSpeciesMap.GetCount());
	m_pDM->SetCellData(_T("수서이외종수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), nHoldBookCnt);
	m_pDM->SetCellData(_T("수서이외책수"), strTmpData, nCurIdx);
	if(nHoldBookCnt > 0)
		m_pDM->SetCellData(_T("삭제가능여부"), _T("N"), nCurIdx);
	else
		m_pDM->SetCellData(_T("삭제가능여부"), _T("Y"), nCurIdx);
	return 0;
}

INT CCheckHoldDataDlg::MakeDonateHoldDataInfo(CESL_DataMgr *pDM, CGroupInfo *pGroupInfo)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString strQuery, strGroupInfo;
	strGroupInfo = pGroupInfo->GetGroupInfo();
	strQuery.Format(
		_T("SELECT I.REC_KEY, B.REC_KEY, B.WORKING_STATUS FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE ")
		_T("I.REC_KEY = B.SPECIES_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_YEAR='%s' AND ")
		_T("B.SEQ_NO=%s AND B.ACQ_CODE='2'"),
		pGroupInfo->GetAcqYear(), strGroupInfo);
	
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	
	CMapStringToString SpeciesMap;
	CMapStringToString HoldSpeciesMap;
	SpeciesMap.RemoveAll();
	HoldSpeciesMap.RemoveAll();
	INT nHoldBookCnt = 0;
	CString strWorkingStatus, strSpeciesKey, strTmpData;
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strWorkingStatus = tmpDM.GetCellData(i, 2);
		strSpeciesKey = tmpDM.GetCellData(i, 0);
		if(strWorkingStatus.Left(3) != _T("BOA"))
		{
			nHoldBookCnt++;
			HoldSpeciesMap.SetAt(strSpeciesKey, _T(""));
		}
		SpeciesMap.SetAt(strSpeciesKey, _T(""));
	}
	
	m_pDM->InsertRow(-1);
	INT nCurIdx = m_pDM->GetRowCount()-1;
	m_pDM->SetCellData(_T("수입년도"), pGroupInfo->GetAcqYear(), nCurIdx);
	m_pDM->SetCellData(_T("그룹번호"), strGroupInfo, nCurIdx);
	strTmpData.Format(_T("%d"), SpeciesMap.GetCount());
	m_pDM->SetCellData(_T("종수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), nRowCnt);
	m_pDM->SetCellData(_T("책수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), HoldSpeciesMap.GetCount());
	m_pDM->SetCellData(_T("수서이외종수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), nHoldBookCnt);
	m_pDM->SetCellData(_T("수서이외책수"), strTmpData, nCurIdx);
	if(nHoldBookCnt > 0)
		m_pDM->SetCellData(_T("삭제가능여부"), _T("N"), nCurIdx);
	else
		m_pDM->SetCellData(_T("삭제가능여부"), _T("Y"), nCurIdx);
	return 0;
}

INT CCheckHoldDataDlg::DeleteGroupHaveHoldData()
{
	INT ids;
	INT nRowCnt = m_pDM->GetRowCount();
	CString strHoldDataCnt, strAcqYear, strGroupInfo, strWorkCode;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strHoldDataCnt = m_pDM->GetCellData(_T("수서이외종수"), i);
		if(strHoldDataCnt != _T("0"))
		{
			strGroupInfo = m_pDM->GetCellData(_T("그룹번호"), i);
			strAcqYear = m_pDM->GetCellData(_T("수입년도"), i);
			if(m_pGroupCollection->GetAcqType() == PURCHASE)
				strWorkCode = m_pDM->GetCellData(_T("업무구분"), i);

			ids = DeleteGroupData(strAcqYear, strGroupInfo, strWorkCode);
			if(ids < 0) return -1;
		}
	}
	return 0;
}

INT CCheckHoldDataDlg::DeleteGroupData(CString strAcqYear, CString strGroupInfo, CString strWorkCode)
{
	INT nGroupCount = m_pGroupCollection->GetGroupCount();
	if(nGroupCount == 0) return 0;
	m_pGroupCollection->SetCurSel(0);
	
	CString strSrcAcqYear, strSrcGroupInfo, strSrcWorkCode;
	CGroupInfo * pGroupInfo = NULL;
	for(INT i=nGroupCount-1 ; i>=0 ; i--)
	{
		pGroupInfo = m_pGroupCollection->GetGroupInfo(i);
		if(pGroupInfo == NULL) continue;
	
		strSrcAcqYear = pGroupInfo->GetAcqYear();
		strSrcGroupInfo = pGroupInfo->GetGroupInfo();
		if(strAcqYear == strSrcAcqYear && strSrcGroupInfo == strGroupInfo)
		{
			if(m_pGroupCollection->GetAcqType() == PURCHASE)
			{
				strSrcWorkCode = pGroupInfo->GetWorkCode();
				if(strSrcWorkCode == strWorkCode)
				{
					m_pGroupCollection->RemoveAt(i);
					return 0;
				}
			}
			else
			{
				m_pGroupCollection->RemoveAt(i);
			}
		}
	}
	return 0;
}

VOID CCheckHoldDataDlg::InitCtrl()
{
	if(m_nAcqType == PURCHASE)
	{
		this->SetWindowText(_T("차수번호내 자료 확인"));
		GetDlgItem(IDC_stcCHECK_INFO)->SetWindowText(_T("수서이외 자료가 포함된 차수정보는 삭제될 수 없습니다.\n삭제하시려면 수서이외 자료를 다른 차수로 분리하여야 합니다."));
	}
	else if(m_nAcqType == DONATE)
	{
		this->SetWindowText(_T("접수번호내 자료 확인"));
		GetDlgItem(IDC_stcCHECK_INFO)->SetWindowText(_T("수서이외 자료가 포함된 접수번호정보는 삭제될 수 없습니다.\n삭제하시려면 수서이외 자료를 다른 접수번호로 분리하여야 합니다."));
	}
	else if(m_nAcqType == OTHER_ACQ_TYPE)
	{
		this->SetWindowText(_T("그룹번호내 자료 확인"));
		GetDlgItem(IDC_stcCHECK_INFO)->SetWindowText(_T("등록번호 부여된 자료가 포함된 그룹번호정보는 삭제될 수 없습니다.\n삭제하시려면 등록번호 부여된 자료를 다른 그룹번호로 분리하여야 합니다."));
	}
}

INT CCheckHoldDataDlg::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;
	
	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);
	
	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
	}
	
	pGrid->SetCol(nCol);
	
	return 0;
}

INT CCheckHoldDataDlg::MakeOtherGroupDataInfo(CESL_DataMgr *pDM, CGroupInfo *pGroupInfo, CString strAcqCode)
{
	if(pDM == NULL || pGroupInfo == NULL || strAcqCode.IsEmpty()) return -1;

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	CString strQuery, strGroupInfo;
	strGroupInfo = pGroupInfo->GetGroupInfo();
	strQuery.Format(
		_T("SELECT I.REC_KEY, B.REC_KEY, B.WORKING_STATUS FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE ")
		_T("I.REC_KEY = B.SPECIES_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_CODE = '%s' AND B.ACQ_YEAR='%s' AND ")
		_T("B.SEQ_NO=%s "),
		strAcqCode, pGroupInfo->GetAcqYear(), strGroupInfo);
	
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	
	CMapStringToString SpeciesMap;
	CMapStringToString HoldSpeciesMap;
	SpeciesMap.RemoveAll();
	HoldSpeciesMap.RemoveAll();
	INT nHoldBookCnt = 0;
	CString strWorkingStatus, strSpeciesKey, strTmpData;
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strWorkingStatus = tmpDM.GetCellData(i, 2);
		strSpeciesKey = tmpDM.GetCellData(i, 0);
		if(strWorkingStatus != _T("BOR111N"))
		{
			nHoldBookCnt++;
			HoldSpeciesMap.SetAt(strSpeciesKey, _T(""));
		}
		SpeciesMap.SetAt(strSpeciesKey, _T(""));
	}
	
	m_pDM->InsertRow(-1);
	INT nCurIdx = m_pDM->GetRowCount()-1;
	m_pDM->SetCellData(_T("수입년도"), pGroupInfo->GetAcqYear(), nCurIdx);
	m_pDM->SetCellData(_T("그룹번호"), strGroupInfo, nCurIdx);
	strTmpData.Format(_T("%d"), SpeciesMap.GetCount());
	m_pDM->SetCellData(_T("종수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), nRowCnt);
	m_pDM->SetCellData(_T("책수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), HoldSpeciesMap.GetCount());
	m_pDM->SetCellData(_T("수서이외종수"), strTmpData, nCurIdx);
	strTmpData.Format(_T("%d"), nHoldBookCnt);
	m_pDM->SetCellData(_T("수서이외책수"), strTmpData, nCurIdx);
	if(nHoldBookCnt > 0)
		m_pDM->SetCellData(_T("삭제가능여부"), _T("N"), nCurIdx);
	else
		m_pDM->SetCellData(_T("삭제가능여부"), _T("Y"), nCurIdx);
	return 0;
}

VOID CCheckHoldDataDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = DeleteGroupHaveHoldData();
	if(ids < 0) m_bOccurError = TRUE;
	else m_bOccurError = FALSE;	
	
	CDialog::OnOK();
}

HBRUSH CCheckHoldDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}
