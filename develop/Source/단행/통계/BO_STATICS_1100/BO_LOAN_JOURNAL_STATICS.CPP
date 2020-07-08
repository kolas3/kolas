#include "Stdafx.h"
#include "BO_LOAN_JOURNAL_STATICS.h"
#include "..\..\열람\장서관리\LOC_MACRO\ErrorMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOAN_JOURNAL_DIRECTION_HORI		0
#define LOAN_JOURNAL_DIRECTION_VERT		1

#define LOAN_JOURNAL_FIXED_COL_INDEX	0

#define LOAN_JOURNAL_GET_CODE_FLAG_CODE	0
#define LOAN_JOURNAL_GET_CODE_FLAG_DESC	1

#define LOAN_JOURNAL_CODE_MAT_CODE			_T("18")
#define LOAN_JOURNAL_CODE_MEDIA_CODE		_T("5")
#define LOAN_JOURNAL_CODE_USER_CLASS_CODE	_T("31")
#define LOAN_JOURNAL_CODE_SHELF_LOC_CODE	_T("19")
#define LOAN_JOURNAL_CODE_MANAGE_CODE		_T("1")

#define LOAN_JOURNAL_KEYWORD_CLASS			_T("구분")
#define LOAN_JOURNAL_KEYWORD_NULL			_T("적용안함")
#define LOAN_JOURNAL_KEYWORD_ETC			_T("기타")
#define LOAN_JOURNAL_KEYWORD_TOTAL			_T("계")
#define LOAN_JOURNAL_KEYWORD_MONTH_TOTAL	_T("월 누계")
#define LOAN_JOURNAL_KEYWORD_YEAR_TOTAL		_T("년 누계")

#define LOAN_JOURNAL_ILL_FLAG_LOANCOUNT		0
#define LOAN_JOURNAL_ILL_FLAG_RETURNCOUNT	1

#define LOAN_JOURNAL_OVERDUE_FLAG_USERCOUNT	0
#define LOAN_JOURNAL_OVERDUE_FLAG_BOOKCOUNT	1

/////////////////////////////////////////////////////////////////////////////
// CBO_LOAN_JOURNAL_STATICS dialog

CBO_LOAN_JOURNAL_STATICS::CBO_LOAN_JOURNAL_STATICS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_LOAN_JOURNAL_STATICS::IDD, pParent)
{
	m_pXLAutomation = NULL;
}
CBO_LOAN_JOURNAL_STATICS::~CBO_LOAN_JOURNAL_STATICS()
{	
	if (m_pXLAutomation != NULL)
		delete m_pXLAutomation;
}

VOID CBO_LOAN_JOURNAL_STATICS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOAN_JOURNAL_STATICS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBO_LOAN_JOURNAL_STATICS, CDialog)
	//{{AFX_MSG_MAP(CBO_LOAN_JOURNAL_STATICS)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_CLASS_STAT, IDC_CHK_USE_CLASS_STAT, OnChkStatics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_MEDIA_STAT, IDC_CHK_USE_MEDIA_STAT, OnChkStatics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_MAT_STAT, IDC_CHK_USE_MAT_STAT, OnChkStatics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_REG_USER_STAT, IDC_CHK_USE_REG_USER_STAT, OnChkStatics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_LOAN_STAT, IDC_CHK_USE_LOAN_STAT, OnChkStatics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_RETURN_STAT, IDC_CHK_USE_RETURN_STAT, OnChkStatics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_USE_DELAY_STAT, IDC_CHK_USE_DELAY_STAT, OnChkStatics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOAN_JOURNAL_STATICS message handlers

BOOL CBO_LOAN_JOURNAL_STATICS::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	INT ids = InitESL_Mgr(_T("SM_BO_LOAN_JOURNAL_STATICS"));
	if (ids < 0)
	{
		AfxMessageBox(_T("OnInitDialog Error"));
		return FALSE;
	}

	InitControl();
	InitCodeInfo();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOAN_JOURNAL_STATICS::OnOK()
{
	CString strTerm1;
	CString strTerm2;

	if (!GetSearchTerm(strTerm1, strTerm2))
	{
		ASSERT(FALSE);
		return;
	}

	if (strTerm2 < strTerm1)
	{
		CString strMsg;
		strMsg.Format(_T("집계기간 시작일을 마지막일보다 크게 입력할 수 없습니다."));
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return;
	}

	{
		CString strTmp1 = strTerm1;
		CString strTmp2 = strTerm2;

		strTmp1.Replace(_T("/"), _T(""));
		strTmp2.Replace(_T("/"), _T(""));

		INT nTmp1 = _ttoi(strTmp1);
		INT nTmp2 = _ttoi(strTmp2);

		if ((nTmp2-nTmp1) > 31)
		{
			if (IDNO == AfxMessageBox(_T("집계기간이 한달을 초과하는 경우 통계 산출에 많은 시간이 소요될 수 있습니다.\r\n통계를 산출하시겠습니까?"), MB_ICONINFORMATION|MB_YESNO))
			{
				return;
			}
		}
	}

	INT ids = CalcBookStatistics(strTerm1, strTerm2);
	if (0 > ids)
	{		
		CString strMsg;
		strMsg.Format(_T("서지 통계를 산출하는 도중에 오류가 발생하였습니다 : %d"), ids);
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return;
	}

	ids = CalcLoanUserStatistics(strTerm1, strTerm2);
	if (0 > ids)
	{	
		CString strMsg;
		strMsg.Format(_T("이용자 통계를 산출하는 도중에 오류가 발생하였습니다 : %d"), ids);
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return;
	}

	ids = CalcLoanHistoryStatistics(strTerm1, strTerm2);
	if (0 > ids)
	{		
		CString strMsg;
		strMsg.Format(_T("대출반납 통계를 산출하는 도중에 오류가 발생하였습니다 : %d"), ids);
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return;
	}

	AfxMessageBox(_T("관내업무일지 통계 산출이 완료되었습니다."), MB_ICONINFORMATION);

	CDialog::OnOK();
}

VOID CBO_LOAN_JOURNAL_STATICS::OnChkStatics(UINT nCtrlID)
{
	CButton* pWnd = (CButton*)GetDlgItem(nCtrlID);
	ASSERT(pWnd);

	INT nChecked = pWnd->GetCheck();

	switch (nCtrlID)
	{
	case IDC_CHK_USE_CLASS_STAT:
	case IDC_CHK_USE_MEDIA_STAT:
	case IDC_CHK_USE_MAT_STAT:
		{
			const INT nSiblingCount = 3;
			INT nSiblingID[nSiblingCount] = {
				IDC_CHK_USE_CLASS_STAT, IDC_CHK_USE_MEDIA_STAT, IDC_CHK_USE_MAT_STAT
			};

			for (INT i=0; i<nSiblingCount; i++) 
			{
				CButton* pSibling = (CButton*)GetDlgItem(nSiblingID[i]);
				ASSERT(pSibling);

				if (nChecked != pSibling->GetCheck()) break;
			}

			if (i >= nSiblingCount || nChecked == BST_CHECKED) 
			{
				const INT nChildCount = 2;
				INT nChildID[nChildCount] = {
					IDC_CHK_MEDIA_STAT_IMP_APPENDIX, IDC_CHK_CLASS_STAT_IMP_WORKING
				};

				for (INT j=0; j<nChildCount; j++) 
				{
					CButton* pChild = (CButton*)GetDlgItem(nChildID[j]);
					ASSERT(pChild);
					pChild->EnableWindow((nChecked == BST_CHECKED));
					if (nChecked == BST_UNCHECKED) pChild->SetCheck(BST_UNCHECKED);
				}
			}
		}
		break;
	case IDC_CHK_USE_REG_USER_STAT:
		{		
			const INT nChildCount = 3;
			INT nChildID[nChildCount] = {
				IDC_CHK_REG_USER_STAT_IMP_STATUS, IDC_CHK_REG_USER_STAT_IMP_EXPELLED, IDC_CHK_REG_USER_STAT_IMP_YEAR_RESULT
			};

			for (INT i=0; i<nChildCount; i++) 
			{
				CButton* pChild = (CButton*)GetDlgItem(nChildID[i]);
				ASSERT(pChild);
				pChild->EnableWindow((nChecked == BST_CHECKED));
				if (nChecked == BST_UNCHECKED) pChild->SetCheck(BST_UNCHECKED);
			}
		}
		break;
	case IDC_CHK_USE_LOAN_STAT:
	case IDC_CHK_USE_RETURN_STAT:
	case IDC_CHK_USE_ILL_STAT:
		{
			const INT nSiblingCount = 3;
			INT nSiblingID[nSiblingCount] = {
				IDC_CHK_USE_LOAN_STAT, IDC_CHK_USE_RETURN_STAT, IDC_CHK_USE_ILL_STAT
			};

			for (INT i=0; i<nSiblingCount; i++) 
			{
				CButton* pSibling = (CButton*)GetDlgItem(nSiblingID[i]);
				ASSERT(pSibling);
				if (!pSibling->IsWindowVisible()) continue;
				if (nChecked != pSibling->GetCheck()) break;
			}

			if (i >= nSiblingCount || nChecked == BST_CHECKED) 
			{
				const INT nChildCount = 1;
				INT nChildID[nChildCount] = {
					IDC_CHK_LOAN_STAT_IMP_YEAR_RESULT
				};

				for (INT j=0; j<nChildCount; j++) 
				{
					CButton* pChild = (CButton*)GetDlgItem(nChildID[j]);
					ASSERT(pChild);
					pChild->EnableWindow((nChecked == BST_CHECKED));
					if (nChecked == BST_UNCHECKED) pChild->SetCheck(BST_UNCHECKED);
				}
			}
		}
		break;
	case IDC_CHK_USE_DELAY_STAT:
		break;
	}
}

VOID CBO_LOAN_JOURNAL_STATICS::InitControl()
{
	const INT nCheckBoxCount = 7;
	INT nResourceID[nCheckBoxCount] = {
		IDC_CHK_USE_CLASS_STAT, IDC_CHK_USE_MEDIA_STAT, IDC_CHK_USE_MAT_STAT, 
		IDC_CHK_USE_REG_USER_STAT, IDC_CHK_USE_LOAN_STAT, IDC_CHK_USE_RETURN_STAT, IDC_CHK_USE_DELAY_STAT
	};

	for (INT i=0; i<nCheckBoxCount; i++) 
	{
		CButton* pWnd = (CButton*)GetDlgItem(nResourceID[i]);
		ASSERT(pWnd);
		pWnd->SetCheck(BST_CHECKED);
	}

	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	

	CString strQuery;
	strQuery.Format(_T("SELECT NVL(VALUE_1,'N') FROM ESL_MANAGE_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE AND VALUE_NAME='책두레사용여부'"));

	CString strValue;
	pQueryDM->GetOneValueQuery(strQuery,strValue);
	if (strValue.CompareNoCase(_T("Y")) == 0) 
	{
		CButton* pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_ILL_STAT);
		ASSERT(pWnd);
		pWnd->ShowWindow(SW_SHOW);
		pWnd->SetCheck(BST_CHECKED);
	}

}

VOID CBO_LOAN_JOURNAL_STATICS::InitCodeInfo()
{
	CString strQuery = _T("SELECT CLASS, TRIM(CODE), DESCRIPTION FROM CD_CODE_TBL ORDER BY CLASS, CODE");

	m_DMCodeInfo.SetCONNECTION_INFO(_T(""));
	m_DMCodeInfo.InitDataMgrRef(3);
	m_DMCodeInfo.SetDataMgrRef(0, _T("CLASS"), _T("CLASS"), _T("str"), _T(""), _T(""));
	m_DMCodeInfo.SetDataMgrRef(1, _T("CODE"), _T("CODE"), _T("str"), _T(""), _T(""));
	m_DMCodeInfo.SetDataMgrRef(2, _T("DESCRIPTION"), _T("DESCRIPTION"), _T("str"), _T(""), _T(""));
	m_DMCodeInfo.RestructDataManager(strQuery);
}

BOOL CBO_LOAN_JOURNAL_STATICS::GetSearchTerm(CString &strTerm1, CString &strTerm2, BOOL bRefresh/*=TRUE*/)
{
	if (!m_strTerm1.IsEmpty() && !m_strTerm2.IsEmpty() && !bRefresh)
	{
		strTerm1 = m_strTerm1;
		strTerm2 = m_strTerm2;
		return TRUE;
	}

	CDateTimeCtrl* pCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DAT_TOTAL1);
	ASSERT(pCtrl);

	SYSTEMTIME time;
	if (pCtrl->GetTime(&time) == GDT_VALID)
	{
		m_strTerm1.Format(_T("%04d/%02d/%02d"), time.wYear, time.wMonth, time.wDay);
		strTerm1 = m_strTerm1;
	}

	pCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DAT_TOTAL2);
	ASSERT(pCtrl);

	if (pCtrl->GetTime(&time) == GDT_VALID)
	{
		m_strTerm2.Format(_T("%04d/%02d/%02d"), time.wYear, time.wMonth, time.wDay);
		strTerm2 = m_strTerm2;
	}

	return (!m_strTerm1.IsEmpty() && !m_strTerm2.IsEmpty());
}


VOID CBO_LOAN_JOURNAL_STATICS::InitPrintDM(INT nColCount/*=20*/, CESL_DataMgr* pPrintDM/*=NULL*/)
{
	if (!pPrintDM)
	{
		pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT"));
		ASSERT(pPrintDM);
	}

	ASSERT(pPrintDM->FreeData() > -1);
	ASSERT(pPrintDM->FreeDMRef() > -1);

	ASSERT(pPrintDM->InitDataMgrRef(nColCount) > -1);
	pPrintDM->m_nCols = nColCount;
}

INT CBO_LOAN_JOURNAL_STATICS::MakePrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, CStringArray &arrFixedColum, CStringArray &arrFixedRowColum, BOOL bInitPrintDM/*=TRUE*/)
{
	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT"));
	ASSERT(pPrintDM);

	INT nFixedColCount = arrFixedColum.GetSize();

	if (bInitPrintDM)
		InitPrintDM(nFixedColCount);

	pPrintDM->InsertRow(-1);
	INT nRowIndex = pPrintDM->GetRowCount() - 1;
	ASSERT(nRowIndex > -1);
	pPrintDM->SetCellData(nRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, strTitle);

	INT nTitleIndex = nRowIndex;
	
	pPrintDM->InsertRow(-1); 
	nRowIndex = pPrintDM->GetRowCount() - 1;
	
	INT nFixedRowIndex = nRowIndex;
	for (INT i = 0; i < nFixedColCount; i++)
		pPrintDM->SetCellData(nFixedRowIndex, i, arrFixedColum.GetAt(i));

	if (!strTerm1.IsEmpty() && !strTerm2.IsEmpty())
	{
		CString strTermDisp;
		strTermDisp.Format(_T("집계기간:%s ~ %s"), strTerm1, strTerm2);
		pPrintDM->SetCellData(nTitleIndex, nFixedColCount-1, strTermDisp);
	}
	
	INT nFixedRowCount = arrFixedRowColum.GetSize();
	for (i = 0; i < nFixedRowCount; i++)
	{
		pPrintDM->InsertRow(-1);
		nRowIndex = pPrintDM->GetRowCount() - 1;

		pPrintDM->SetCellData(nRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, arrFixedRowColum.GetAt(i));
	}

	return nFixedRowIndex; 
}

INT CBO_LOAN_JOURNAL_STATICS::MakeBookClassNoStaticsPrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, BOOL bInitPrintDM/*=TRUE*/)
{	
	const INT nFixedColCount = 14;
	CString strFixedColName[nFixedColCount] = {
		LOAN_JOURNAL_KEYWORD_CLASS, _T("총류"), _T("철학"), _T("종교"), _T("사회과학"), _T("자연과학"), _T("기술과학"), _T("예술"), _T("언어"), _T("문학"), _T("역사"), LOAN_JOURNAL_KEYWORD_NULL, LOAN_JOURNAL_KEYWORD_ETC, LOAN_JOURNAL_KEYWORD_TOTAL
	};
	
	CStringArray arrFixedColum;
	for (INT i = 0; i < nFixedColCount; i++)
		arrFixedColum.Add(strFixedColName[i]);
	
	const INT nFixedRowCount = 5;
	CString strFixedRowName[nFixedRowCount] = {
		_T("구입"), _T("기증"), LOAN_JOURNAL_KEYWORD_NULL, LOAN_JOURNAL_KEYWORD_ETC, LOAN_JOURNAL_KEYWORD_TOTAL
	};

	CStringArray arrFixedRowColum;
	for (i = 0; i < nFixedRowCount; i++)
		arrFixedRowColum.Add(strFixedRowName[i]);

	return MakePrintDMOutLine(strTerm1, strTerm2, strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM);
}

INT CBO_LOAN_JOURNAL_STATICS::MakeBookMatCodeStaticsPrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, BOOL bInitPrintDM/*=TRUE*/)
{	
	CStringArray arrFixedColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_MAT_CODE, arrFixedColum);

	const INT nFixedRowCount = 5;
	CString strFixedRowName[nFixedRowCount] = {
		_T("구입"), _T("기증"), LOAN_JOURNAL_KEYWORD_NULL, LOAN_JOURNAL_KEYWORD_ETC, LOAN_JOURNAL_KEYWORD_TOTAL
	};

	CStringArray arrFixedRowColum;
	for (INT i = 0; i < nFixedRowCount; i++)
		arrFixedRowColum.Add(strFixedRowName[i]);

	return MakePrintDMOutLine(strTerm1, strTerm2, strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM); // 현재까지는 코드로 구성되어 있다
}

INT CBO_LOAN_JOURNAL_STATICS::MakeBookMediaCodeStaticsPrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, BOOL bInitPrintDM/*=TRUE*/)
{
	CStringArray arrFixedColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_MEDIA_CODE, arrFixedColum);

	const INT nFixedRowCount = 5;
	CString strFixedRowName[nFixedRowCount] = {
		_T("구입"), _T("기증"), LOAN_JOURNAL_KEYWORD_NULL, LOAN_JOURNAL_KEYWORD_ETC, LOAN_JOURNAL_KEYWORD_TOTAL
	};

	CStringArray arrFixedRowColum;
	for (INT i = 0; i < nFixedRowCount; i++)
		arrFixedRowColum.Add(strFixedRowName[i]);

	return MakePrintDMOutLine(strTerm1, strTerm2, strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM);
}

INT CBO_LOAN_JOURNAL_STATICS::MakeLoanUserStaticsPrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, BOOL bTotalSumUsed, BOOL bInitPrintDM/*=TRUE*/)
{
	CStringArray arrFixedColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_USER_CLASS_CODE, arrFixedColum);
	arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL);
	if (bTotalSumUsed)
		arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL);
	
	CStringArray arrFixedRowColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_SHELF_LOC_CODE, arrFixedRowColum, LOAN_JOURNAL_GET_CODE_FLAG_CODE, FALSE);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_ETC);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_TOTAL);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL);
	if (bTotalSumUsed)
		arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL);

	return MakePrintDMOutLine(strTerm1, strTerm2, strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM); // 현재까지는 코드로 구성되어 있다	
}

INT CBO_LOAN_JOURNAL_STATICS::MakeLoanHistoryStaticsPrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, BOOL bTotalSumUsed, BOOL bInitPrintDM/*=TRUE*/)
{
	const INT nFixedColCount = 15;
	CString strFixedColName[nFixedColCount] = {
		LOAN_JOURNAL_KEYWORD_CLASS, _T("총류"), _T("철학"), _T("종교"), _T("사회과학"), _T("자연과학"), _T("기술과학"), _T("예술"), _T("언어"), _T("문학"), _T("역사"), LOAN_JOURNAL_KEYWORD_NULL, LOAN_JOURNAL_KEYWORD_ETC, LOAN_JOURNAL_KEYWORD_TOTAL, LOAN_JOURNAL_KEYWORD_MONTH_TOTAL
	};

	CStringArray arrFixedColum;
	for (INT i = 0; i < nFixedColCount; i++)
		arrFixedColum.Add(strFixedColName[i]);
	if (bTotalSumUsed)
		arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL);

	CStringArray arrFixedRowColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_SHELF_LOC_CODE, arrFixedRowColum, LOAN_JOURNAL_GET_CODE_FLAG_CODE, FALSE);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_ETC);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_TOTAL);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL);
	if (bTotalSumUsed)
		arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL);

	return MakePrintDMOutLine(strTerm1, strTerm2, strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM);
}

INT CBO_LOAN_JOURNAL_STATICS::MakeIllStaticsPrintDMOutLine(CString strTerm1, CString strTerm2, CString strTitle, INT nIllFlag, BOOL bTotalSumUsed, BOOL bInitPrintDM/*=TRUE*/)
{
	CStringArray arrFixedColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_MANAGE_CODE, arrFixedColum, LOAN_JOURNAL_GET_CODE_FLAG_CODE, FALSE);
	arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_ETC);
	arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_TOTAL);
	arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL);
	if (bTotalSumUsed)
		arrFixedColum.Add(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL);

	CStringArray arrFixedRowColum;
	arrFixedRowColum.Append(arrFixedColum);

	CString strClassAlias;
	switch (nIllFlag)
	{
	case LOAN_JOURNAL_ILL_FLAG_LOANCOUNT:	strClassAlias = _T("제공＼신청"); break;
	case LOAN_JOURNAL_ILL_FLAG_RETURNCOUNT:	strClassAlias = _T("반납＼소장"); break;
	}

	arrFixedColum.InsertAt(0, strClassAlias);

	return MakePrintDMOutLine(strTerm1, strTerm2, strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM); // 현재까지는 코드로 구성되어 있다
}

INT CBO_LOAN_JOURNAL_STATICS::MakeOverdueStaticsPrintDMOutLine(CString strTitle, BOOL bInitPrintDM/*=TRUE*/)
{
	const INT nFixedColCount = 3;
	CString strFixedColName[nFixedColCount] = {
		LOAN_JOURNAL_KEYWORD_CLASS, _T("연체자수"), _T("연체권수")
	};

	CStringArray arrFixedColum;
	for (INT i = 0; i < nFixedColCount; i++)
		arrFixedColum.Add(strFixedColName[i]);

	CStringArray arrFixedRowColum;
	_GetCodeInfos(LOAN_JOURNAL_CODE_SHELF_LOC_CODE, arrFixedRowColum, LOAN_JOURNAL_GET_CODE_FLAG_CODE, FALSE);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_ETC);
	arrFixedRowColum.Add(LOAN_JOURNAL_KEYWORD_TOTAL);

	return MakePrintDMOutLine(_T(""), _T(""), strTitle, arrFixedColum, arrFixedRowColum, bInitPrintDM); // 현재까지는 코드로 구성되어 있다
}

INT CBO_LOAN_JOURNAL_STATICS::MakePrintDMData(INT nFixedRowIndex, INT nFixedColIndex, INT nQueryDMColDataIndex, INT nQueryDMRowDataIndex, INT nQueryDMSumDataIndex, INT nQueryDMReplaceIndex/*=-1*/)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY"));
	ASSERT(pQueryDM);

	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT"));
	ASSERT(pPrintDM);

	if (nQueryDMColDataIndex < 0 && nQueryDMRowDataIndex < 0)
		return -1; 

	INT nTotalColCount = pPrintDM->GetColCount();
	INT nExtraColIndex = -1;

	CStringArray arrFixedColum;
	if (nQueryDMColDataIndex > -1)
	{
		for (INT i = nFixedColIndex; i < nTotalColCount; i++)
		{
			CString strData = pPrintDM->GetCellData(nFixedRowIndex, i);
			arrFixedColum.Add(strData);

			if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_ETC) == 0)
				nExtraColIndex = i;
		}
	}

	INT nTotalRowCount = pPrintDM->GetRowCount();
	INT nExtraRowIndex = -1;

	CStringArray arrFixedRowColum;
	if (nQueryDMRowDataIndex > -1)
	{
		for (INT i = nFixedRowIndex; i < nTotalRowCount; i++)
		{
			CString strData = pPrintDM->GetCellData(i, nFixedColIndex);
			arrFixedRowColum.Add(strData);

			if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_ETC) == 0)
				nExtraRowIndex = i;
		}
	}

	INT nDataRowCount = pQueryDM->GetRowCount();
	for (INT i = 0; i < nDataRowCount; i++)
	{
		INT nColIdx = -1;
		INT nRowIdx = -1;

		if (nQueryDMColDataIndex > -1)
		{
			CString strColSeparator = pQueryDM->GetCellData(i, nQueryDMColDataIndex);
			nColIdx = _GetArrayItemIndex(arrFixedColum, strColSeparator) + nFixedColIndex; 
		}
		else
		{
			nColIdx = nQueryDMReplaceIndex;
		}

		if (nQueryDMRowDataIndex > -1)
		{
			CString strRowSeparator = pQueryDM->GetCellData(i, nQueryDMRowDataIndex);
			nRowIdx = _GetArrayItemIndex(arrFixedRowColum, strRowSeparator) + nFixedRowIndex;
		}
		else
		{
			nRowIdx = nQueryDMReplaceIndex; 
		}

		CString strSumData = pQueryDM->GetCellData(i, nQueryDMSumDataIndex);

		if (nColIdx < 0 && nExtraColIndex > -1) nColIdx = nExtraColIndex;
		if (nRowIdx < 0 && nExtraRowIndex > -1) nRowIdx = nExtraRowIndex;

		if (nColIdx > -1 && nRowIdx > -1)
		{
			if (nColIdx == nExtraColIndex)
			{
				CString strOldData = pPrintDM->GetCellData(nRowIdx, nColIdx);
				if (!strOldData.IsEmpty())
				{
					INT nNewSumData = _ttoi(strOldData) + _ttoi(strSumData);
					strSumData.Format(_T("%d"), nNewSumData);
				}
			}
			else
				pPrintDM->SetCellData(nRowIdx, nColIdx, strSumData);
		}
	}

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakePrintDMSumData(INT nFixedRowIndex, INT nFixedColIndex, INT nDirection, INT nSumCellIndex, 
													BOOL bMonthSumUsed/*=FALSE*/, BOOL bYearSumUsed/*=FALSE*/, INT nDMExtractColIdx/*=-1*/, INT nDMSumColIdx/*=-1*/)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY"));
	ASSERT(pQueryDM);

	CESL_DataMgr* pMonthQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_MONTH_QUERY"));
	ASSERT(pMonthQueryDM);

	CESL_DataMgr* pYearQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_YEAR_QUERY"));
	ASSERT(pYearQueryDM);

	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT"));
	ASSERT(pPrintDM);

	INT nRowCount = pPrintDM->GetRowCount();
	INT nColCount = pPrintDM->GetColCount();

	switch (nDirection)
	{
	case LOAN_JOURNAL_DIRECTION_HORI:
		{
			if (nSumCellIndex < 0)
			{
				for (INT j=nFixedColIndex+1; j<nColCount; j++)
				{
					CString strData = pPrintDM->GetCellData(nFixedRowIndex, j);
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0)
						nSumCellIndex = j;
				}
			}

			INT nFixedMonthSumRowIdx = -1;
			if (bMonthSumUsed)
			{
				for (INT j=nFixedRowIndex+1; j<nRowCount; j++)
				{
					CString strData = pPrintDM->GetCellData(j, nFixedColIndex);
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL) == 0)
						nFixedMonthSumRowIdx = j;
				}
			}

			for (INT i=nFixedRowIndex+1; i<nRowCount; i++)
			{
				if (bMonthSumUsed && nFixedMonthSumRowIdx > -1 && nFixedMonthSumRowIdx == i)					
					break;

				INT nSumData = 0;
				for (INT j=nFixedColIndex+1; j<nColCount; j++)
				{
					if (j == nSumCellIndex) break;

					CString strData = pPrintDM->GetCellData(i, j);
					if (strData.IsEmpty())
						pPrintDM->SetCellData(i, j, _T("0"));
					else
						nSumData += _ttoi(strData);
				}

				CString strSumData;
				strSumData.Format(_T("%d"), (nSumData > 0) ? nSumData : 0);

				pPrintDM->SetCellData(i, nSumCellIndex, strSumData);
			}

			if (bMonthSumUsed)
			{
				INT nMonthSumCellIndex = -1;
				INT nYearSumCellIndex  = -1;
				for (i=nFixedColIndex+1; i<nColCount; i++)
				{
					CString strData = pPrintDM->GetCellData(nFixedRowIndex, i);
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL) == 0)
						nMonthSumCellIndex = i;
					if (bYearSumUsed && strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL) == 0)
						nYearSumCellIndex = i;
				}

				CStringArray arrFixedColumns;
				for (i=nFixedRowIndex+1; i<nRowCount; i++)
				{
					CString strData = pPrintDM->GetCellData(i, nFixedColIndex);
					strData.TrimLeft();strData.TrimRight();
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0)
						break;
					arrFixedColumns.Add(strData);
				}

				if (nMonthSumCellIndex > -1)
				{
					INT nMonthRowCount = pMonthQueryDM->GetRowCount();
					for (i=0; i<nMonthRowCount; i++)
					{
						CString strCode = pMonthQueryDM->GetCellData(i, nDMExtractColIdx);
						strCode.TrimLeft();strCode.TrimRight();
						CString strData = pMonthQueryDM->GetCellData(i, nDMSumColIdx);
						strData.TrimLeft();strData.TrimRight();
						if (strData.IsEmpty()) continue;

						INT nFindMonthRowIdx = _GetArrayItemIndex(arrFixedColumns, strCode)+nFixedRowIndex+1;
						if (nFindMonthRowIdx > -1)
						{
							CString strBeforeData = pPrintDM->GetCellData(nFindMonthRowIdx, nMonthSumCellIndex);
							if (!strBeforeData.IsEmpty())
								strData.Format(_T("%d"), _ttoi(strBeforeData) + _ttoi(strData));
							pPrintDM->SetCellData(nFindMonthRowIdx, nMonthSumCellIndex, strData);
						}
					}

					INT nMonthSumTotal = 0;
					for (INT j=nFixedRowIndex+1; j<nRowCount; j++)
					{
						CString strData = pPrintDM->GetCellData(j, nFixedColIndex);
						if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0) 
						{
							strData.Format(_T("%d"), nMonthSumTotal);
							pPrintDM->SetCellData(j, nMonthSumCellIndex, strData);
							break;
						}

						strData = pPrintDM->GetCellData(j, nMonthSumCellIndex);
						if (strData.IsEmpty()) 
							pPrintDM->SetCellData(j, nMonthSumCellIndex, _T("0"));
						else
							nMonthSumTotal += _ttoi(strData);
					}
				}

				if (bYearSumUsed && nYearSumCellIndex > -1)
				{
					INT nYearRowCount = pYearQueryDM->GetRowCount();
					for (i=0; i<nYearRowCount; i++)
					{
						CString strCode = pYearQueryDM->GetCellData(i, nDMExtractColIdx);
						strCode.TrimLeft();strCode.TrimRight();
						CString strData = pYearQueryDM->GetCellData(i, nDMSumColIdx);
						strData.TrimLeft();strData.TrimRight();
						if (strData.IsEmpty()) continue;

						INT nFindYearRowIdx = _GetArrayItemIndex(arrFixedColumns, strCode)+nFixedRowIndex+1;
						if (nFindYearRowIdx > -1)
						{
							CString strBeforeData = pPrintDM->GetCellData(nFindYearRowIdx, nYearSumCellIndex);
							if (!strBeforeData.IsEmpty())
								strData.Format(_T("%d"), _ttoi(strBeforeData) + _ttoi(strData));
							pPrintDM->SetCellData(nFindYearRowIdx, nYearSumCellIndex, strData);
						}
					}

					INT nYearSumTotal = 0;
					for (INT j=nFixedRowIndex+1; j<nRowCount; j++)
					{
						CString strData = pPrintDM->GetCellData(j, nFixedColIndex);
						if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0) 
						{
							strData.Format(_T("%d"), nYearSumTotal);
							pPrintDM->SetCellData(j, nYearSumCellIndex, strData);
							break;
						}

						strData = pPrintDM->GetCellData(j, nYearSumCellIndex);
						if (strData.IsEmpty()) 
							pPrintDM->SetCellData(j, nYearSumCellIndex, _T("0"));
						else
							nYearSumTotal += _ttoi(strData);
					}
				}
			}
		}
		break;
	case LOAN_JOURNAL_DIRECTION_VERT:
		{
			if (nSumCellIndex < 0)
			{
				for (INT j=nFixedRowIndex+1; j<nRowCount; j++)
				{
					CString strData = pPrintDM->GetCellData(j, nFixedColIndex);
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0)
						nSumCellIndex = j;
				}
			}

			INT nFixedMonthSumColIdx = -1;
			if (bMonthSumUsed)
			{
				for (INT j=nFixedColIndex+1; j<nColCount; j++)
				{
					CString strData = pPrintDM->GetCellData(nFixedRowIndex, j);
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL) == 0)
						nFixedMonthSumColIdx = j;
				}
			}

			for (INT i = nFixedColIndex+1; i < nColCount; i++)
			{
				if (bMonthSumUsed && nFixedMonthSumColIdx > -1 && nFixedMonthSumColIdx == i)					
					break;

				INT nSumData = 0;
				for (INT j = nFixedRowIndex+1; j < nRowCount; j++)
				{
					if (j == nSumCellIndex) break;

					CString strData = pPrintDM->GetCellData(j, i);
					if (strData.IsEmpty())
						pPrintDM->SetCellData(j, i, _T("0"));
					else
						nSumData += _ttoi(strData);
				}

				CString strSumData;
				strSumData.Format(_T("%d"), (nSumData > 0) ? nSumData : 0);

				pPrintDM->SetCellData(nSumCellIndex, i, strSumData);
			}

			if (bMonthSumUsed)
			{
				INT nMonthSumCellIndex = -1;
				INT nYearSumCellIndex  = -1;
				for (i=nFixedRowIndex+1; i<nRowCount; i++)
				{
					CString strData = pPrintDM->GetCellData(i, nFixedColIndex);
					if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_MONTH_TOTAL) == 0)
						nMonthSumCellIndex = i;
					if (bYearSumUsed && strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_YEAR_TOTAL) == 0)
						nYearSumCellIndex = i;
				}

				CStringArray arrFixedColumns;
				for (i=nFixedColIndex+1; i<nColCount; i++)
				{
					CString strData = pPrintDM->GetCellData(nFixedRowIndex, i);
					strData.TrimLeft();strData.TrimRight();
					arrFixedColumns.Add(strData);
				}

				if (nMonthSumCellIndex > -1)
				{					
					INT nMonthRowCount = pMonthQueryDM->GetRowCount();
					for (i=0; i<nMonthRowCount; i++)
					{
						CString strCode = pMonthQueryDM->GetCellData(i, nDMExtractColIdx);
						strCode.TrimLeft();strCode.TrimRight();
						CString strData = pMonthQueryDM->GetCellData(i, nDMSumColIdx);
						strData.TrimLeft();strData.TrimRight();
						if (strData.IsEmpty()) continue;

						INT nFindMonthColIdx = _GetArrayItemIndex(arrFixedColumns, strCode)+nFixedColIndex+1;
						if (nFindMonthColIdx > -1)
						{
							CString strBeforeData = pPrintDM->GetCellData(nMonthSumCellIndex, nFindMonthColIdx);
							if (!strBeforeData.IsEmpty())
								strData.Format(_T("%d"), _ttoi(strBeforeData) + _ttoi(strData));
							pPrintDM->SetCellData(nMonthSumCellIndex, nFindMonthColIdx, strData);
						}
					}

					INT nMonthSumTotal = 0;
					for (INT j=nFixedColIndex+1; j<nColCount; j++)
					{
						CString strData = pPrintDM->GetCellData(nFixedRowIndex, j);
						if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0) 
						{
							strData.Format(_T("%d"), nMonthSumTotal);
							pPrintDM->SetCellData(nMonthSumCellIndex, j, strData);
							break;
						}

						strData = pPrintDM->GetCellData(nMonthSumCellIndex, j);
						if (strData.IsEmpty()) 
							pPrintDM->SetCellData(nMonthSumCellIndex, j, _T("0"));
						else
							nMonthSumTotal += _ttoi(strData);
					}

					if (bYearSumUsed && nYearSumCellIndex > -1)
					{
						INT nYearRowCount = pYearQueryDM->GetRowCount();
						for (i=0; i<nYearRowCount; i++)
						{
							CString strCode = pYearQueryDM->GetCellData(i, nDMExtractColIdx);
							strCode.TrimLeft();strCode.TrimRight();
							CString strData = pYearQueryDM->GetCellData(i, nDMSumColIdx);
							strData.TrimLeft();strData.TrimRight();
							if (strData.IsEmpty()) continue;

							INT nFindYearColIdx = _GetArrayItemIndex(arrFixedColumns, strCode)+nFixedColIndex+1;
							if (nFindYearColIdx > -1)
							{
								CString strBeforeData = pPrintDM->GetCellData(nYearSumCellIndex, nFindYearColIdx);
								if (!strBeforeData.IsEmpty())
									strData.Format(_T("%d"), _ttoi(strBeforeData) + _ttoi(strData));
								pPrintDM->SetCellData(nYearSumCellIndex, nFindYearColIdx, strData);
							}
						}

						INT nYearSumTotal = 0;
						for (INT j=nFixedColIndex+1; j<nColCount; j++)
						{
							CString strData = pPrintDM->GetCellData(nFixedRowIndex, j);
							if (strData.CompareNoCase(LOAN_JOURNAL_KEYWORD_TOTAL) == 0) 
							{
								strData.Format(_T("%d"), nYearSumTotal);
								pPrintDM->SetCellData(nYearSumCellIndex, j, strData);
								break;
							}

							strData = pPrintDM->GetCellData(nYearSumCellIndex, j);
							if (strData.IsEmpty()) 
								pPrintDM->SetCellData(nYearSumCellIndex, j, _T("0"));
							else
								nYearSumTotal += _ttoi(strData);
						}
					}
				}
			}
		}
		break;
	}

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeBookClassNoStaticsPrintDMData(INT nFixedRowIndex)
{
	MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, 0, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_HORI, 13);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+5);

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeBookMatCodeStaticsPrintDMData(INT nFixedRowIndex)
{
	MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, 0, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_HORI, -1);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+5);

	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_MAT_CODE, nFixedRowIndex, LOAN_JOURNAL_DIRECTION_HORI);

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeBookMediaCodeStaticsPrintDMData(INT nFixedRowIndex)
{
	MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, 0, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_HORI, -1);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+5);

	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_MEDIA_CODE, nFixedRowIndex, LOAN_JOURNAL_DIRECTION_HORI);

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeLoanUserStaticsPrintDMData(INT nFixedRowIndex, BOOL bImpYearSum)
{
	MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, 0, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_HORI, -1, TRUE, bImpYearSum, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, -1, TRUE, bImpYearSum, 0, 2);

	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_USER_CLASS_CODE, nFixedRowIndex, LOAN_JOURNAL_DIRECTION_HORI);
	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_SHELF_LOC_CODE, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+1);

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeLoanHistoryStaticsPrintDMData(INT nFixedRowIndex, BOOL bImpYearSum)
{
	MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, 0, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_HORI, -1, TRUE, bImpYearSum, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, -1, TRUE, bImpYearSum, 0, 2);

	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_SHELF_LOC_CODE, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+1);

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeIllHistoryStaticsPrintDMData(INT nFixedRowIndex, BOOL bImpYearSum)
{
	MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, 0, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_HORI, -1, TRUE, bImpYearSum, 1, 2);
	MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, -1, TRUE, bImpYearSum, 0, 2);

	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_MANAGE_CODE, nFixedRowIndex, LOAN_JOURNAL_DIRECTION_HORI);
	_ConvertDescPrintDM(LOAN_JOURNAL_CODE_MANAGE_CODE, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+1);

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::MakeOverdueStaticsPrintDMData(INT nFixedRowIndex, INT nOverdueFlag, BOOL bLastProc/*=FALSE*/)
{
	switch (nOverdueFlag)
	{
	case LOAN_JOURNAL_OVERDUE_FLAG_USERCOUNT:
		{
			MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, -1, 0, 1, 1);
		}
		break;
	case LOAN_JOURNAL_OVERDUE_FLAG_BOOKCOUNT:
		{
			MakePrintDMData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, -1, 0, 1, 2);
		}
		break;
	}

	if (bLastProc)
	{
		MakePrintDMSumData(nFixedRowIndex, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, -1);
		_ConvertDescPrintDM(LOAN_JOURNAL_CODE_SHELF_LOC_CODE, LOAN_JOURNAL_FIXED_COL_INDEX, LOAN_JOURNAL_DIRECTION_VERT, nFixedRowIndex+1);
	}

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::_GetArrayItemIndex(CStringArray &arrItems, CString strItem)
{
	INT nCount = arrItems.GetSize();
	for (INT i=0; i<nCount; i++)
	{
		if (strItem.CompareNoCase(arrItems.GetAt(i)) == 0)
			return i;
	}

	return -1;
}

INT CBO_LOAN_JOURNAL_STATICS::_GetCodeInfos(CString strClass, CStringArray &arrItems, INT nFlag/*=LOAN_JOURNAL_GET_CODE_FLAG_CODE*/, BOOL bAppendExtraItem/*=TRUE*/)
{
	INT nRowCount = m_DMCodeInfo.GetRowCount();

	BOOL bFindFlag = FALSE;
	BOOL bFindNullCode = FALSE;
	for (INT i=0; i<nRowCount; i++)
	{
		CString strValue;
		m_DMCodeInfo.GetCellData(_T("CLASS"), i, strValue);
		if (strValue.CompareNoCase(strClass) == 0)
		{
			CString strTarget;
			if (nFlag == LOAN_JOURNAL_GET_CODE_FLAG_CODE)
				strTarget = _T("CODE");
			else
				strTarget = _T("DESCRIPTION");
			
			m_DMCodeInfo.GetCellData(strTarget, i, strValue);
			strValue.TrimLeft();strValue.TrimRight();
			if (strValue.IsEmpty() || strValue.CompareNoCase(LOAN_JOURNAL_KEYWORD_NULL) == 0)
				bFindNullCode = TRUE;
			else
				arrItems.Add(strValue);
		}
		else
		{
			if (bFindFlag) break;
		}
	}

	if (bFindNullCode)
	{
		if (nFlag == 0)
			arrItems.Add(_T(""));
		else
			arrItems.Add(LOAN_JOURNAL_KEYWORD_NULL);
	}

	if (bAppendExtraItem && arrItems.GetSize() > 0)
	{
		arrItems.InsertAt(0, LOAN_JOURNAL_KEYWORD_CLASS);
		arrItems.Add(LOAN_JOURNAL_KEYWORD_ETC);
		arrItems.Add(LOAN_JOURNAL_KEYWORD_TOTAL);
	}

	return arrItems.GetSize();
}

INT CBO_LOAN_JOURNAL_STATICS::_ConvertDescPrintDM(CString strClass, INT nFixedIndex, INT nDirection, INT nStartRowIdx/*=0*/)
{
	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT"));
	ASSERT(pPrintDM);

	INT nRowCount = m_DMCodeInfo.GetRowCount();

	BOOL bFindFlag = FALSE;
	for (INT i=0; i<nRowCount; i++)
	{
		CString strValue;
		m_DMCodeInfo.GetCellData(_T("CLASS"), i, strValue);
		if (strValue.CompareNoCase(strClass) == 0)
		{
			m_DMCodeInfo.GetCellData(_T("CODE"), i, strValue);
			strValue.TrimLeft();strValue.TrimRight();

			switch (nDirection)
			{
			case LOAN_JOURNAL_DIRECTION_HORI:
				{
					INT nCount = pPrintDM->GetColCount();
					for (INT j=0; j<nCount; j++)
					{
						CString strData = pPrintDM->GetCellData(nFixedIndex, j);
						strData.TrimLeft();strData.TrimRight();
						if (strData.CompareNoCase(strValue) == 0)
						{
							m_DMCodeInfo.GetCellData(_T("DESCRIPTION"), i, strValue);
							pPrintDM->SetCellData(nFixedIndex, j, strValue);
							break;
						}
					}
				}
				break;
			case LOAN_JOURNAL_DIRECTION_VERT:
				{
					INT nCount = pPrintDM->GetRowCount();
					for (INT j=nStartRowIdx; j<nCount; j++)
					{
						CString strData = pPrintDM->GetCellData(j, nFixedIndex);
						strData.TrimLeft();strData.TrimRight();
						if (strData.CompareNoCase(strValue) == 0)
						{
							m_DMCodeInfo.GetCellData(_T("DESCRIPTION"), i, strValue);
							pPrintDM->SetCellData(j, nFixedIndex, strValue);
							break;
						}
					}
				}
				break;
			}
		}
		else
		{
			if (bFindFlag) break;
		}
	}

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::CalcBookStatistics(CString strTerm1, CString strTerm2)
{	
	CButton* pWnd = NULL;

	pWnd = (CButton*)GetDlgItem(IDC_CHK_MEDIA_STAT_IMP_APPENDIX);
	ASSERT(pWnd);
	BOOL bImpAppendix = (pWnd->GetCheck() == BST_CHECKED);

	pWnd = (CButton*)GetDlgItem(IDC_CHK_CLASS_STAT_IMP_WORKING);
	ASSERT(pWnd);
	BOOL bImpWorking = (pWnd->GetCheck() == BST_CHECKED);

	BOOL bCalcStatistics = FALSE;
	
	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_CLASS_STAT);
	ASSERT(pWnd);

	INT ids = 0;
	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcBookClassNoStatistics(strTerm1, strTerm2, bImpAppendix, bImpWorking);
		if (0 > ids) return ids;
	}

	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_MEDIA_STAT);
	ASSERT(pWnd);
	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcBookMediaCodeStatistics(strTerm1, strTerm2, bImpAppendix, bImpWorking);
		if (0 > ids) return ids;
	}

	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_MAT_STAT);
	ASSERT(pWnd);
	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcBookMatCodeStatistics(strTerm1, strTerm2, bImpAppendix, bImpWorking);
		if (0 > ids) return ids;
	}

	return 0;
}

#define JOURNAL_STATICS_BOOK_DEFAULT_QUERY \
	_T("SELECT %s, ACQ_CODE_DESC, COUNT(1) FROM ( ")	\
		_T("SELECT ")	\
		_T("%s, ")	\
		_T("CASE ")	\
			_T("WHEN B.ACQ_CODE = '1' THEN '구입' ")	\
			_T("WHEN B.ACQ_CODE = '2' THEN '기증' ")	\
			_T("WHEN B.ACQ_CODE IS NULL THEN '적용안함' ")	\
			_T("ELSE '기타'	")	\
		_T("END AS ACQ_CODE_DESC ")	\
		_T("FROM %s	")	\
		_T("WHERE %s ) ")	\
		_T("GROUP BY %s, ACQ_CODE_DESC ")	\
		_T("ORDER BY %s, DECODE(ACQ_CODE_DESC,'구입',0,'기증',1,'적용안함',2,'기타',3) ")

INT CBO_LOAN_JOURNAL_STATICS::CalcBookClassNoStatistics(CString strTerm1, CString strTerm2, BOOL bImpAppendix, BOOL bImpWorking)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT")); // 
	ASSERT(pPrintDM);

	CString strAlias;
	CString strField;
	CString strTable;
	CString strWhere;		
	CString strOrder;

	strAlias.Format(_T("CLASS_NO_DESC"));
	strField.Format(_T("CASE ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='0' THEN '총류' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='1' THEN '철학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='2' THEN '종교' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='3' THEN '사회과학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='4' THEN '자연과학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='5' THEN '기술과학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='6' THEN '예술' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='7' THEN '언어' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='8' THEN '문학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='9' THEN '역사' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1) IS NULL THEN '적용안함' ")
						_T("ELSE '기타' ")
					_T("END AS %s"), strAlias);
	strTable.Format(_T("BO_SPECIES_TBL S, BO_BOOK_TBL B"));	
	strOrder.Format(_T("DECODE(%s,'총류',0,'철학',1,'종교',2,'사회과학',3,'자연과학',4,'기술과학',5,'예술',6,'언어',7,'문학',8,'역사',9,'적용안함',10,'기타',11)"), strAlias);

	CString strAppendix;
	if (!bImpAppendix)
		strAppendix.Format(_T(" AND B.BOOK_APPENDIX_FLAG='B'"));

	CString strTermWhere;
	if (bImpWorking) 
		strTermWhere.Format(_T(" AND B.INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2); // 입력일로 산출
	else
		strTermWhere.Format(_T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2); // 배가일로 산출

	strWhere.Format(_T("S.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE=UDF_MANAGE_CODE%s%s"), strAppendix, strTermWhere);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_BOOK_DEFAULT_QUERY, strAlias, strField, strTable, strWhere, strAlias, strOrder);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1111 , _T("CalcBookClassNoStatistics"));

	INT nFixedRowIndex = MakeBookClassNoStaticsPrintDMOutLine(strTerm1, strTerm2, _T("단행자료 분류별 수입구분별 통계"));
	MakeBookClassNoStaticsPrintDMData(nFixedRowIndex);
	
	strAlias.Format(_T("CLASS_NO_DESC"));
	strField.Format(_T("CASE ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='0' THEN '총류' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='1' THEN '철학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='2' THEN '종교' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='3' THEN '사회과학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='4' THEN '자연과학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='5' THEN '기술과학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='6' THEN '예술' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='7' THEN '언어' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='8' THEN '문학' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1)='9' THEN '역사' ")
						_T("WHEN SUBSTR(B.CLASS_NO,0,1) IS NULL THEN '적용안함' ")
						_T("ELSE '기타' ")
					_T("END AS %s"), strAlias);
	strTable.Format(_T("SE_SPECIES_TBL S, SE_BOOK_TBL B"));	
	strOrder.Format(_T("DECODE(%s,'총류',0,'철학',1,'종교',2,'사회과학',3,'자연과학',4,'기술과학',5,'예술',6,'언어',7,'문학',8,'역사',9,'적용안함',10,'기타',11)"), strAlias);

	if (bImpAppendix)
		strAppendix.Format(_T(" AND B.BOOK_TYPE IN ('B', 'C', 'A')")); // 결호, 지연은 제외
	else
		strAppendix.Format(_T(" AND B.BOOK_TYPE IN ('B', 'C')"));

	if (bImpWorking) 
		strTermWhere.Format(_T(" AND B.INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2); // 입력일로 산출
	else
		strTermWhere.Format(_T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2); // 배가일로 산출

	strWhere.Format(_T("S.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE=UDF_MANAGE_CODE%s%s"), strAppendix, strTermWhere);

	strQuery.Format(JOURNAL_STATICS_BOOK_DEFAULT_QUERY, strAlias, strField, strTable, strWhere, strAlias, strOrder);

	ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1112 , _T("CalcBookClassNoStatistics"));
	
	pPrintDM->InsertRow(-1);
	nFixedRowIndex = MakeBookClassNoStaticsPrintDMOutLine(strTerm1, strTerm2, _T("연속자료 분류별 수입구분별 통계"), FALSE);
	MakeBookClassNoStaticsPrintDMData(nFixedRowIndex);

	OutputResult(_T("분류별 수입구분별 통계"));

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::CalcBookMatCodeStatistics(CString strTerm1, CString strTerm2, BOOL bImpAppendix, BOOL bImpWorking)
{	
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT")); // 
	ASSERT(pPrintDM);

	CString strAlias;
	CString strField;
	CString strTable;
	CString strWhere;		
	CString strOrder;

	strAlias.Format(_T("MAT_CODE"));
	strField.Format(_T("S.MAT_CODE AS %s"), strAlias);
	strTable.Format(_T("BO_SPECIES_TBL S, BO_BOOK_TBL B"));	
	strOrder.Format(_T("%s"), strAlias);

	CString strAppendix;
	if (!bImpAppendix)
		strAppendix.Format(_T(" AND B.BOOK_APPENDIX_FLAG='B'"));

	CString strTermWhere;
	if (bImpWorking) 
		strTermWhere.Format(_T(" AND B.INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);
	else
		strTermWhere.Format(_T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);

	strWhere.Format(_T("S.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE=UDF_MANAGE_CODE%s%s"), strAppendix, strTermWhere);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_BOOK_DEFAULT_QUERY, strAlias, strField, strTable, strWhere, strAlias, strOrder);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1121 , _T("CalcBookMatCodeStatistics"));

	INT nFixedRowIndex = MakeBookMatCodeStaticsPrintDMOutLine(strTerm1, strTerm2, _T("단행자료 자료구분별 수입구분별 통계"));
	MakeBookMatCodeStaticsPrintDMData(nFixedRowIndex);
	
	strAlias.Format(_T("MAT_CODE"));
	strField.Format(_T("S.MAT_CODE AS %s"), strAlias);
	strTable.Format(_T("SE_SPECIES_TBL S, SE_BOOK_TBL B"));	
	strOrder.Format(_T("%s"), strAlias);

	if (bImpAppendix)
		strAppendix.Format(_T(" AND B.BOOK_TYPE IN ('B', 'C', 'A')"));
	else
		strAppendix.Format(_T(" AND B.BOOK_TYPE IN ('B', 'C')"));

	if (bImpWorking) 
		strTermWhere.Format(_T(" AND B.INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);
	else
		strTermWhere.Format(_T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);

	strWhere.Format(_T("S.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE=UDF_MANAGE_CODE%s%s"), strAppendix, strTermWhere);

	strQuery.Format(JOURNAL_STATICS_BOOK_DEFAULT_QUERY, strAlias, strField, strTable, strWhere, strAlias, strOrder);

	ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1122 , _T("CalcBookMatCodeStatistics"));

	pPrintDM->InsertRow(-1);
	nFixedRowIndex = MakeBookMatCodeStaticsPrintDMOutLine(strTerm1, strTerm2, _T("연속자료 자료구분별 수입구분별 통계"), FALSE);
	MakeBookMatCodeStaticsPrintDMData(nFixedRowIndex);

	OutputResult(_T("자료구분별 수입구분별 통계"));

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::CalcBookMediaCodeStatistics(CString strTerm1, CString strTerm2, BOOL bImpAppendix, BOOL bImpWorking)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY"));
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT"));
	ASSERT(pPrintDM);

	CString strAlias;
	CString strField;
	CString strTable;
	CString strWhere;		
	CString strOrder;

	strAlias.Format(_T("MEDIA_CODE"));
	strField.Format(_T("S.MEDIA_CODE AS %s"), strAlias);
	strTable.Format(_T("BO_SPECIES_TBL S, BO_BOOK_TBL B"));	
	strOrder.Format(_T("%s"), strAlias);

	CString strAppendix;
	if (!bImpAppendix)
		strAppendix.Format(_T(" AND B.BOOK_APPENDIX_FLAG='B'"));

	CString strTermWhere;
	if (bImpWorking) 
		strTermWhere.Format(_T(" AND B.INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);
		strTermWhere.Format(_T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);

	strWhere.Format(_T("S.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE=UDF_MANAGE_CODE%s%s"), strAppendix, strTermWhere);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_BOOK_DEFAULT_QUERY, strAlias, strField, strTable, strWhere, strAlias, strOrder);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1131 , _T("CalcBookMediaCodeStatistics"));

	INT nFixedRowIndex = MakeBookMediaCodeStaticsPrintDMOutLine(strTerm1, strTerm2, _T("단행자료 매체구분별 수입구분별 통계"));
	MakeBookMediaCodeStaticsPrintDMData(nFixedRowIndex);

	strAlias.Format(_T("MEDIA_CODE"));
	strField.Format(_T("S.MEDIA_CODE AS %s"), strAlias);
	strTable.Format(_T("SE_SPECIES_TBL S, SE_BOOK_TBL B"));	
	strOrder.Format(_T("%s"), strAlias);

	if (bImpAppendix)
		strAppendix.Format(_T(" AND B.BOOK_TYPE IN ('B', 'C', 'A')"));
	else
		strAppendix.Format(_T(" AND B.BOOK_TYPE IN ('B', 'C')"));

	if (bImpWorking) 
		strTermWhere.Format(_T(" AND B.INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);
	else
		strTermWhere.Format(_T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s','YYYY/MM/DD')"), strTerm1, strTerm2);

	strWhere.Format(_T("S.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE=UDF_MANAGE_CODE%s%s"), strAppendix, strTermWhere);

	strQuery.Format(JOURNAL_STATICS_BOOK_DEFAULT_QUERY, strAlias, strField, strTable, strWhere, strAlias, strOrder);

	ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1132 , _T("CalcBookMediaCodeStatistics"));
	
	pPrintDM->InsertRow(-1);
	nFixedRowIndex = MakeBookMediaCodeStaticsPrintDMOutLine(strTerm1, strTerm2, _T("연속자료 매체구분별 수입구분별 통계"), FALSE);
	MakeBookMediaCodeStaticsPrintDMData(nFixedRowIndex);
	
	OutputResult(_T("매체구분별 수입구분별 통계"));

	return 0;
}

#define JOURNAL_STATICS_LOANUSER_DEFAULT_QUERY \
	_T("SELECT USER_CLASS_CODE, SHELF_LOC_CODE, COUNT(1) FROM CO_LOAN_USER_TBL ")	\
	 _T("WHERE %s ")	\
	 _T("GROUP BY USER_CLASS_CODE, SHELF_LOC_CODE ")	\
	 _T("ORDER BY USER_CLASS_CODE, SHELF_LOC_CODE ")

INT CBO_LOAN_JOURNAL_STATICS::CalcLoanUserStatistics(CString strTerm1, CString strTerm2)
{
	CButton* pWnd = NULL;

	pWnd = (CButton*)GetDlgItem(IDC_CHK_REG_USER_STAT_IMP_STATUS);
	ASSERT(pWnd);
	BOOL bImpNoMember = (pWnd->GetCheck() == BST_CHECKED);

	pWnd = (CButton*)GetDlgItem(IDC_CHK_REG_USER_STAT_IMP_EXPELLED);
	ASSERT(pWnd);
	BOOL bImpExpelled = (pWnd->GetCheck() == BST_CHECKED);

	pWnd = (CButton*)GetDlgItem(IDC_CHK_REG_USER_STAT_IMP_YEAR_RESULT);
	ASSERT(pWnd);
	BOOL bImpYearSum = (pWnd->GetCheck() == BST_CHECKED);

	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CString strNoMember;
	if (!bImpNoMember)
		strNoMember.Format(_T(" AND MEMBER_CLASS = '0'"));

	CString strExpelled;
	if (bImpExpelled)
		strExpelled.Format(_T(" AND USER_CLASS IN ('0', '1', '2')"));
	else
		strExpelled.Format(_T(" AND USER_CLASS IN ('0', '1')"));
	
	CString strWhere;
	strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND REG_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')%s%s"), 
		strTerm1, strTerm2, strNoMember, strExpelled);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_LOANUSER_DEFAULT_QUERY, strWhere);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1201 , _T("CalcLoanUserStatistics"));	
	
	CString strTermMonth1 = strTerm1.Left(7);
	CString strTermMonth2 = strTerm2.Left(7);

	if (strTermMonth1.CompareNoCase(strTermMonth2) == 0)	
		strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND TO_CHAR(REG_DATE, 'YYYY/MM') = '%s'%s%s"), strTermMonth1, strNoMember, strExpelled);
	else
	{
		strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND TO_CHAR(REG_DATE, 'YYYY/MM') BETWEEN '%s' AND '%s'%s%s"), 
			strTermMonth1, strTermMonth2, strNoMember, strExpelled);
	}

	strQuery.Format(JOURNAL_STATICS_LOANUSER_DEFAULT_QUERY, strWhere);

	CESL_DataMgr* pMonthQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_MONTH_QUERY"));
	ASSERT(pMonthQueryDM);

	ids = pMonthQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1202 , _T("CalcLoanUserStatistics"));

	if (bImpYearSum)
	{
		CString strTermYear1 = strTerm1.Left(4);
		CString strTermYear2 = strTerm2.Left(4);

		if (strTermYear1.CompareNoCase(strTermYear2) == 0)
			strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND TO_CHAR(REG_DATE, 'YYYY') = '%s'%s%s"), strTermYear1, strNoMember, strExpelled);
		else
		{
			strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND TO_CHAR(REG_DATE, 'YYYY') BETWEEN '%s' AND '%s'%s%s"), 
				strTermYear1, strTermYear2, strNoMember, strExpelled);
		}

		strQuery.Format(JOURNAL_STATICS_LOANUSER_DEFAULT_QUERY, strWhere);

		CESL_DataMgr* pYearQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_YEAR_QUERY"));
		ASSERT(pYearQueryDM);

		ids = pYearQueryDM->RestructDataManager(strQuery);
		if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1203 , _T("CalcLoanUserStatistics"));
	}
	
	INT nFixedRowIndex = MakeLoanUserStaticsPrintDMOutLine(strTerm1, strTerm2, _T("이용자 가입현황 통계"), bImpYearSum);
	MakeLoanUserStaticsPrintDMData(nFixedRowIndex, bImpYearSum);

	OutputResult(_T("이용자 가입현황 통계"));

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::CalcLoanHistoryStatistics(CString strTerm1, CString strTerm2)
{
	CButton* pWnd = NULL;

	pWnd = (CButton*)GetDlgItem(IDC_CHK_LOAN_STAT_IMP_YEAR_RESULT);
	ASSERT(pWnd);
	BOOL bImpYearSum = (pWnd->GetCheck() == BST_CHECKED);

	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_LOAN_STAT);
	ASSERT(pWnd);

	INT ids = 0;
	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcLoanStatistics(strTerm1, strTerm2, bImpYearSum);
		if (0 > ids) return ids;
	}

	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_RETURN_STAT);
	ASSERT(pWnd);

	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcReturnStatistics(strTerm1, strTerm2, bImpYearSum);
		if (0 > ids) return ids;
	}

	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_ILL_STAT);
	ASSERT(pWnd);

	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcIllLoanStatistics(strTerm1, strTerm2, bImpYearSum);
		if (0 > ids) return ids;

		ids = CalcIllReturnStatistics(strTerm1, strTerm2, bImpYearSum);
		if (0 > ids) return ids;
	}

	pWnd = (CButton*)GetDlgItem(IDC_CHK_USE_DELAY_STAT);
	ASSERT(pWnd);

	if (pWnd->GetCheck() == BST_CHECKED)
	{
		ids = CalcOverdueStatistics();
		if (0 > ids) return ids;
	}

	return 0;
}

#define JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY \
	_T("SELECT CLASS_NO_DESC, SHELF_LOC_CODE, COUNT(1) FROM ( ")	\
		_T("SELECT ")	\
			_T("CASE ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '0' THEN '총류' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '1' THEN '철학' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '2' THEN '종교' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '3' THEN '사회과학' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '4' THEN '자연과학' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '5' THEN '기술과학' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '6' THEN '예술' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '7' THEN '언어' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '8' THEN '문학' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) = '9' THEN '역사' ")	\
				_T("WHEN SUBSTR(CLASS_NO,0,1) IS NULL THEN '적용안함' ")	\
				_T("ELSE '기타' ")	\
			_T("END AS CLASS_NO_DESC, SHELF_LOC_CODE ")	\
			_T("FROM %s ")	\
			_T("WHERE %s ) ")	\
			_T("GROUP BY CLASS_NO_DESC, SHELF_LOC_CODE ")	\
			_T("ORDER BY DECODE(CLASS_NO_DESC,'총류',0,'철학',1,'종교',2,'사회과학',3,'자연과학',4,'기술과학',5,'예술',6,'언어',7,'문학',8,'역사',9,'적용안함',10,'기타',11), SHELF_LOC_CODE ")

INT CBO_LOAN_JOURNAL_STATICS::CalcLoanStatistics(CString strTerm1, CString strTerm2, BOOL bImpYearSum)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CString strWhere;
	strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS IN ('0', '1', '2') AND LOAN_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')"), strTerm1, strTerm2);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY, _T("LS_WORK_T01"), strWhere);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1301 , _T("CalcLoanStatistics"));	

	CString strTermMonth1 = strTerm1.Left(7);
	CString strTermMonth2 = strTerm2.Left(7);

	if (strTermMonth1.CompareNoCase(strTermMonth2) == 0)
		strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS IN ('0', '1', '2') AND TO_CHAR(LOAN_DATE, 'YYYY/MM') = '%s'"), strTermMonth1);
	else
		strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS IN ('0', '1', '2') AND TO_CHAR(LOAN_DATE, 'YYYY/MM') BETWEEN '%s' AND '%s'"), strTermMonth1, strTermMonth2);
	
	strQuery.Format(JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY, _T("LS_WORK_T01"), strWhere);

	CESL_DataMgr* pMonthQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_MONTH_QUERY"));
	ASSERT(pMonthQueryDM);
	pMonthQueryDM->FreeData();

	ids = pMonthQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1302 , _T("CalcLoanStatistics"));

	if (bImpYearSum)
	{
		CString strTermYear1 = strTerm1.Left(4);
		CString strTermYear2 = strTerm2.Left(4);

		if (strTermYear1.CompareNoCase(strTermYear2) == 0)
			strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS IN ('0', '1', '2') AND TO_CHAR(LOAN_DATE, 'YYYY') = '%s'"), strTermYear1);
		else
			strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS IN ('0', '1', '2') AND TO_CHAR(LOAN_DATE, 'YYYY') BETWEEN '%s' AND '%s'"), strTermYear1, strTermYear2);
		
		strQuery.Format(JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY, _T("LS_WORK_T01"), strWhere);

		CESL_DataMgr* pYearQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_YEAR_QUERY"));
		ASSERT(pYearQueryDM);
		pYearQueryDM->FreeData();

		ids = pYearQueryDM->RestructDataManager(strQuery);
		if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1303 , _T("CalcLoanStatistics"));
	}

	INT nFixedRowIndex = MakeLoanHistoryStaticsPrintDMOutLine(strTerm1, strTerm2, _T("대출현황 통계"), bImpYearSum);
	MakeLoanHistoryStaticsPrintDMData(nFixedRowIndex, bImpYearSum);

	OutputResult(_T("대출현황 통계"));

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::CalcReturnStatistics(CString strTerm1, CString strTerm2, BOOL bImpYearSum)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CString strWhere;
	strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS = '1' AND RETURN_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')"), strTerm1, strTerm2);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY, _T("LH_STORAGE_V01"), strWhere);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1311 , _T("CalcReturnStatistics"));

	CString strTermMonth1 = strTerm1.Left(7);
	CString strTermMonth2 = strTerm2.Left(7);

	if (strTermMonth1.CompareNoCase(strTermMonth2) == 0)
		strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS = '1' AND TO_CHAR(RETURN_DATE, 'YYYY/MM') = '%s'"), strTermMonth1);
	else
		strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS = '1' AND TO_CHAR(RETURN_DATE, 'YYYY/MM') BETWEEN '%s' AND '%s'"), strTermMonth1, strTermMonth2);

	strQuery.Format(JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY, _T("LH_STORAGE_V01"), strWhere);

	CESL_DataMgr* pMonthQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_MONTH_QUERY"));
	ASSERT(pMonthQueryDM);
	pMonthQueryDM->FreeData();

	ids = pMonthQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1312 , _T("CalcLoanStatistics"));
	
	if (bImpYearSum)
	{
		CString strTermYear1 = strTerm1.Left(4);
		CString strTermYear2 = strTerm2.Left(4);

		if (strTermYear1.CompareNoCase(strTermYear2) == 0)
			strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS = '1' AND TO_CHAR(RETURN_DATE, 'YYYY') = '%s'"), strTermYear1);
		else
			strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE AND STATUS = '1' AND TO_CHAR(RETURN_DATE, 'YYYY') BETWEEN '%s' AND '%s'"), strTermYear1, strTermYear2);

		strQuery.Format(JOURNAL_STATICS_LOANHISTORY_DEFAULT_QUERY, _T("LH_STORAGE_V01"), strWhere);

		CESL_DataMgr* pYearQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_YEAR_QUERY"));
		ASSERT(pYearQueryDM);
		pYearQueryDM->FreeData();

		ids = pYearQueryDM->RestructDataManager(strQuery);
		if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1313 , _T("CalcReturnStatistics"));		
	}
	
	INT nFixedRowIndex = MakeLoanHistoryStaticsPrintDMOutLine(strTerm1, strTerm2, _T("반납현황 통계"), bImpYearSum);
	MakeLoanHistoryStaticsPrintDMData(nFixedRowIndex, bImpYearSum);

	OutputResult(_T("반납현황 통계"));

	return 0;
}

#define JOURNAL_STATICS_ILL_DEFAULT_QUERY \
	_T("SELECT XFIELD, YFIELD, SUM(CNT) FROM ( ")	\
		_T("SELECT ")	\
		_T("TV.%s AS XFIELD, ")	\
		_T("TV.%s AS YFIELD, ")	\
		_T("COUNT(1) AS CNT ")	\
		_T("FROM CO_LOAN_USER_TBL CLU, ILL_TRANS_TBL IT, BO_BOOK_TBL B, TRANS_VIEW TV ")	\
		_T("WHERE IT.TRANS_NO=TV.TRANS_NO AND CLU.REC_KEY=TV.USER_KEY AND B.REC_KEY=TV.BOOK_KEY ")	\
		  _T("AND %s")	\
		_T("GROUP BY TV.%s, TV.%s, %s ) V ")	\
	_T("WHERE 1=1 GROUP BY XFIELD, YFIELD ")	

INT CBO_LOAN_JOURNAL_STATICS::CalcIllLoanStatistics(CString strTerm1, CString strTerm2, BOOL bImpYearSum)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CString strXField = _T("APL_MANAGE_CODE");
	CString strYField = _T("GIVE_MANAGE_CODE");
	CString strGroupBy = _T("TO_CHAR(IT.LS_LOAN_DATE, 'YYYY/MM/DD')");

	CString strWhere;
	strWhere.Format(_T("IT.LS_LOAN_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')"), strTerm1, strTerm2);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_ILL_DEFAULT_QUERY, strXField, strYField, strWhere, strXField, strYField, strGroupBy);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1411 , _T("CalcIllLoanStatistics"));
	
	CString strTermMonth1 = strTerm1.Left(7);
	CString strTermMonth2 = strTerm2.Left(7);

	if (strTermMonth1.CompareNoCase(strTermMonth2) == 0)
		strWhere.Format(_T("TO_CHAR(IT.LS_LOAN_DATE, 'YYYY/MM') = '%s'"), strTermMonth1);
	else
		strWhere.Format(_T("TO_CHAR(IT.LS_LOAN_DATE, 'YYYY/MM') BETWEEN '%s' AND '%s'"), strTermMonth1, strTermMonth2);

	strQuery.Format(JOURNAL_STATICS_ILL_DEFAULT_QUERY, strXField, strYField, strWhere, strXField, strYField, strGroupBy);

	CESL_DataMgr* pMonthQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_MONTH_QUERY"));
	ASSERT(pMonthQueryDM);
	pMonthQueryDM->FreeData();

	ids = pMonthQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1412 , _T("CalcIllLoanStatistics"));
	
	if (bImpYearSum)
	{
		CString strTermYear1 = strTerm1.Left(4);
		CString strTermYear2 = strTerm2.Left(4);

		if (strTermYear1.CompareNoCase(strTermYear2) == 0)
			strWhere.Format(_T("TO_CHAR(IT.LS_LOAN_DATE, 'YYYY') = '%s'"), strTermYear1);
		else
			strWhere.Format(_T("TO_CHAR(IT.LS_LOAN_DATE, 'YYYY') BETWEEN '%s' AND '%s'"), strTermYear1, strTermYear2);

		strQuery.Format(JOURNAL_STATICS_ILL_DEFAULT_QUERY, strXField, strYField, strWhere, strXField, strYField, strGroupBy);

		CESL_DataMgr* pYearQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_YEAR_QUERY"));
		ASSERT(pYearQueryDM);
		pYearQueryDM->FreeData();

		ids = pYearQueryDM->RestructDataManager(strQuery);
		if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1413 , _T("CalcIllLoanStatistics"));		
	}
	
	INT nFixedRowIndex = MakeIllStaticsPrintDMOutLine(strTerm1, strTerm2, _T("책두레 상호대차 대출현황 통계"), LOAN_JOURNAL_ILL_FLAG_LOANCOUNT, bImpYearSum);
	MakeIllHistoryStaticsPrintDMData(nFixedRowIndex, bImpYearSum);

	OutputResult(_T("책두레 상호대차 대출현황 통계"));

	return 0;
}

INT CBO_LOAN_JOURNAL_STATICS::CalcIllReturnStatistics(CString strTerm1, CString strTerm2, BOOL bImpYearSum)
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CString strXField = _T("GIVE_MANAGE_CODE");
	CString strYField = _T("RETURN_MANAGE_CODE");
	CString strGroupBy = _T("TO_CHAR(IT.LS_RETURN_DATE, 'YYYY/MM/DD')");

	CString strWhere;
	strWhere.Format(_T("IT.LS_RETURN_DATE BETWEEN TO_DATE('%s', 'YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')"), strTerm1, strTerm2);

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_ILL_DEFAULT_QUERY, strXField, strYField, strWhere, strXField, strYField, strGroupBy);

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1511 , _T("CalcIllLoanStatistics"));

	CString strTermMonth1 = strTerm1.Left(7);
	CString strTermMonth2 = strTerm2.Left(7);

	if (strTermMonth1.CompareNoCase(strTermMonth2) == 0)
		strWhere.Format(_T("TO_CHAR(IT.LS_RETURN_DATE, 'YYYY/MM') = '%s'"), strTermMonth1);
	else
		strWhere.Format(_T("TO_CHAR(IT.LS_RETURN_DATE, 'YYYY/MM') BETWEEN '%s' AND '%s'"), strTermMonth1, strTermMonth2);

	strQuery.Format(JOURNAL_STATICS_ILL_DEFAULT_QUERY, strXField, strYField, strWhere, strXField, strYField, strGroupBy);

	CESL_DataMgr* pMonthQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_MONTH_QUERY"));
	ASSERT(pMonthQueryDM);
	pMonthQueryDM->FreeData();

	ids = pMonthQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1512 , _T("CalcIllLoanStatistics"));

	if (bImpYearSum)
	{
		CString strTermYear1 = strTerm1.Left(4);
		CString strTermYear2 = strTerm2.Left(4);

		if (strTermYear1.CompareNoCase(strTermYear2) == 0)
			strWhere.Format(_T("TO_CHAR(IT.LS_RETURN_DATE, 'YYYY') = '%s'"), strTermYear1);
		else
			strWhere.Format(_T("TO_CHAR(IT.LS_RETURN_DATE, 'YYYY') BETWEEN '%s' AND '%s'"), strTermYear1, strTermYear2);

		strQuery.Format(JOURNAL_STATICS_ILL_DEFAULT_QUERY, strXField, strYField, strWhere, strXField, strYField, strGroupBy);

		CESL_DataMgr* pYearQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_YEAR_QUERY"));
		ASSERT(pYearQueryDM);
		pYearQueryDM->FreeData();

		ids = pYearQueryDM->RestructDataManager(strQuery);
		if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1513 , _T("CalcIllLoanStatistics"));		
	}

	INT nFixedRowIndex = MakeIllStaticsPrintDMOutLine(strTerm1, strTerm2, _T("책두레 상호대차 반납현황 통계"), LOAN_JOURNAL_ILL_FLAG_RETURNCOUNT, bImpYearSum);
	MakeIllHistoryStaticsPrintDMData(nFixedRowIndex, bImpYearSum);

	OutputResult(_T("책두레 상호대차 반납현황 통계"));

	return 0;
}

#define JOURNAL_STATICS_OVERDUE_DEFAULT_QUERY \
	_T("SELECT SHELF_LOC_CODE, %s FROM LS_WORK_T01 ")	\
	 _T("WHERE STATUS IN ('0', '2') AND RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE+1, 'YYYY/MM/DD'), 'YYYY/MM/DD') ")	\
	 _T("GROUP BY SHELF_LOC_CODE ")	\
	 _T("ORDER BY SHELF_LOC_CODE ")

INT CBO_LOAN_JOURNAL_STATICS::CalcOverdueStatistics()
{
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_QUERY")); // 
	ASSERT(pQueryDM);
	pQueryDM->FreeData();

	CString strQuery;
	strQuery.Format(JOURNAL_STATICS_OVERDUE_DEFAULT_QUERY, _T("COUNT(DISTINCT USER_KEY)"));

	INT ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1331 , _T("CalcOverdueStatistics"));
	
	INT nFixedRowIndex = MakeOverdueStaticsPrintDMOutLine(_T("연체현황 통계"));
	MakeOverdueStaticsPrintDMData(nFixedRowIndex, LOAN_JOURNAL_OVERDUE_FLAG_USERCOUNT);
	
	strQuery.Format(JOURNAL_STATICS_OVERDUE_DEFAULT_QUERY, _T("COUNT(1)"));	

	ids = pQueryDM->RestructDataManager(strQuery);
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1332 , _T("CalcOverdueStatistics"));
	
	MakeOverdueStaticsPrintDMData(nFixedRowIndex, LOAN_JOURNAL_OVERDUE_FLAG_BOOKCOUNT, TRUE);
	
	OutputResult(_T("연체현황 통계"));

	return 0;
}

BOOL CBO_LOAN_JOURNAL_STATICS::OutputResult(CString strSheetName, BOOL bNewWorkSheet/*=TRUE NOT USED*/)
{
	CESL_DataMgr* pPrintDM = FindDM(_T("DM_BO_LOAN_JOURNAL_STATICS_PRINT")); // 
	ASSERT(pPrintDM);

	if (m_pXLAutomation == NULL)
	{
		CLSID clsExcelApp;
		
		BOOL bExcelUsed = TRUE;
		if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
		{
			if (FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
			{
				bExcelUsed = FALSE;
			}
		}

		m_pXLAutomation = new CXLAutomationForJournal(bExcelUsed);	
	}
	else
	{
		m_pXLAutomation->CreateJournalWorkSheet();
	}

	m_pXLAutomation->SetWorkSheetName(strSheetName);

	INT nRowCount = pPrintDM->GetRowCount();
	INT nColCount = pPrintDM->GetColCount();

	CString strTotalData;
	for (INT i=0; i<nRowCount; i++)
	{
		CString strLineData;
		for (INT j=0; j<nColCount; j++)
		{
			strLineData = strLineData + _T("\"") + pPrintDM->GetCellData(i, j) + _T("\"") + _T("\t");
		}

		strLineData = strLineData + _T("\n");		
		strLineData.Replace(_T("\r\n"), _T(" "));

		strTotalData += strLineData;

	}

	m_pXLAutomation->PasteStringToWorksheet(strTotalData);

	return FALSE;
}
