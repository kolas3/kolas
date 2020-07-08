// KL_OTHER_LOAN_DEFAULT.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_LOAN_DEFAULT.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_DEFAULT dialog

#include "FrameManager.h"
CKL_OTHER_LOAN_DEFAULT::CKL_OTHER_LOAN_DEFAULT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_DEFAULT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pGrid = NULL;
	m_bIsSearched = FALSE;
}

CKL_OTHER_LOAN_DEFAULT::~CKL_OTHER_LOAN_DEFAULT()
{

}
void CKL_OTHER_LOAN_DEFAULT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_DEFAULT)

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_DEFAULT, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_LOAN_DEFAULT)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_DEFAULT message handlers

BOOL CKL_OTHER_LOAN_DEFAULT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!m_strSMAlias.IsEmpty())
	{
		InitESL_Mgr(m_strSMAlias);

		m_pDM = FindDM(m_strDMAlias);
		m_pGrid = (CESL_Grid*)FindControl(m_strCMAlias, _T("GRID"));

		CString strGetData, strBefore;
		CTime tGetTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
		CString strCurrent = strGetData.Left(10);
		CTime tCurrent(_ttoi(strCurrent.Mid(0, 4)), _ttoi(strCurrent.Mid(5, 2)), _ttoi(strCurrent.Mid(8, 2)), 0, 0, 0);
		CTimeSpan tOneDay(30, 0, 0, 0);
		CTime tBefore = tCurrent - tOneDay;
		strBefore.Format(_T("%04d/%02d/%02d"), tBefore.GetYear(), tBefore.GetMonth(), tBefore.GetDay());
		SetControlData(m_strCMAlias, _T("복귀일자1"), strBefore);
		SetControlData(m_strCMAlias, _T("복귀일자2"), strCurrent);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_LOAN_DEFAULT::Create(CWnd* pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

INT CKL_OTHER_LOAN_DEFAULT::UnCheckGrid(CStringArray& aCheckArray)
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_LOAN_DEFAULT::UnCheckGrid(CStringArray&)");

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount()) return 0;
	
	int SIZE = aCheckArray.GetSize();
	CString strCheck = _T("");
	CString strCheckAlias = _T(""); // DM에서 비교할 칼럼 알리아스
	CString strCheckStyle = _T(""); // DM에서 비교할 칼럼 알리아스
	CString strCheckValue = _T(""); // DM의 값과 비교할 값
	CString strGetValue = _T("");	// DM에서 얻어온 값

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{			
		for(int nCnt = 0; nCnt < SIZE; nCnt++)
		{
			strCheck = aCheckArray.GetAt(nCnt);
			AfxExtractSubString(strCheckAlias, strCheck, 0, '|');
			AfxExtractSubString(strCheckStyle, strCheck, 1, '|');
			AfxExtractSubString(strCheckValue, strCheck, 2, '|');
// 			strCheckValue = Get_AliasToValue(strCheckValue);

			ids = m_pDM->GetCellData(strCheckAlias, nIdx, strGetValue);
			if(ids)
			{
				return ids;
			}

			BOOL bIsUnCheck = FALSE;
			if(_T("==") == strCheckStyle)
			{
				if(strGetValue == strCheckValue) bIsUnCheck = TRUE;
			}
			else if(_T("!=") == strCheckStyle)
			{
				if(strGetValue != strCheckValue) bIsUnCheck = TRUE;
			}
			if(TRUE == bIsUnCheck)
			{
				// 선정해제
				m_pGrid->SetAt(
					nIdx,
					m_pGrid->m_nSelectIdx,
					_T("")
					);
				break;
			}
		}
		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return 0;
}

CString CKL_OTHER_LOAN_DEFAULT::GetComboBoxState()
{
	int ids = -1;
	CString strFuncName = _T("CKL_OTHER_LOAN_DEFAULT::Get_TblName");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(m_strCMAlias, _T("멀티콤보_진행상태"));
	if(NULL == pCMB)
	{
		return -1000;
	}

	CString strStateCode, strStateDesc;
	const int nIdx = pCMB->GetCurSel();
	if(0 <= nIdx)
	{
		strStateCode = pCMB->GetItemDataString(nIdx, 0);
		strStateDesc = pCMB->GetItemDataString(nIdx, 1);
	}

	return strStateCode;
}

CString CKL_OTHER_LOAN_DEFAULT::GetBeforeDate()
{
	CString strGetDate;
	GetControlData(m_strCMAlias, _T("복귀일자1"), strGetDate);
	if(10 > strGetDate.GetLength())
	{
		SetControlData(m_strCMAlias, _T("복귀일자1"), _T(""));
		strGetDate = _T("");
	}

	return strGetDate;
}

CString CKL_OTHER_LOAN_DEFAULT::GetAfterDate()
{
	CString strGetDate;
	GetControlData(m_strCMAlias, _T("복귀일자2"), strGetDate);
	if(10 > strGetDate.GetLength())
	{
		SetControlData(m_strCMAlias, _T("복귀일자2"), _T(""));
		strGetDate = _T("");
	}

	return strGetDate;
}

void CKL_OTHER_LOAN_DEFAULT::SetDateControlView()
{
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(m_strCMAlias, _T("멀티콤보_진행상태"));
	if(NULL == pCMB) return;

	CString strStateCode;
	const int nIdx = pCMB->GetCurSel();
	if(0 <= nIdx)
	{
		strStateCode = pCMB->GetItemDataString(nIdx, 0);
		if(_T("3") == strStateCode)
		{
			GetDlgItem(IDC_stcDate1)->ShowWindow(TRUE);
			GetDlgItem(IDC_stcDate2)->ShowWindow(TRUE);
			GetDlgItem(IDC_Date1)->ShowWindow(TRUE);
			GetDlgItem(IDC_Date2)->ShowWindow(TRUE);
			//2018-11-14 EDIT BY KYJ
			//JOB.2018.0082 신청상태도 일자로 조건 추가 할 수 있도록
			//=============================================
			GetDlgItem(IDC_stcDate3)->ShowWindow(FALSE);
			//=============================================
		}
		else if(_T("2") == strStateCode)
		{
			GetDlgItem(IDC_stcDate1)->ShowWindow(TRUE);
			GetDlgItem(IDC_stcDate2)->ShowWindow(TRUE);
			GetDlgItem(IDC_Date1)->ShowWindow(TRUE);
			GetDlgItem(IDC_Date2)->ShowWindow(TRUE);
			//2018-11-14 EDIT BY KYJ
			//JOB.2018.0082 신청상태도 일자로 조건 추가 할 수 있도록
			//=============================================
			GetDlgItem(IDC_stcDate3)->ShowWindow(FALSE);
			//=============================================
		}
		//2018-11-14 EDIT BY KYJ
		//JOB.2018.0082 신청상태도 일자로 조건 추가 할 수 있도록
		//=================================================
		else if (_T("0") == strStateCode)
		{
			GetDlgItem(IDC_stcDate1)->ShowWindow(FALSE);
			GetDlgItem(IDC_stcDate3)->ShowWindow(TRUE);
			GetDlgItem(IDC_stcDate2)->ShowWindow(TRUE);
			GetDlgItem(IDC_Date1)->ShowWindow(TRUE);
			GetDlgItem(IDC_Date2)->ShowWindow(TRUE);
		}
		//=================================================
		else
		{
			GetDlgItem(IDC_stcDate1)->ShowWindow(FALSE);
			GetDlgItem(IDC_stcDate2)->ShowWindow(FALSE);
			GetDlgItem(IDC_Date1)->ShowWindow(FALSE);
			GetDlgItem(IDC_Date2)->ShowWindow(FALSE);
			//2018-11-14 EDIT BY KYJ
			//JOB.2018.0082 신청상태도 일자로 조건 추가 할 수 있도록
			//=============================================
			GetDlgItem(IDC_stcDate3)->ShowWindow(FALSE);
			//=============================================
		}
		
	}
}

void CKL_OTHER_LOAN_DEFAULT::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		KillTimer(1);		

		if(m_pGrid)
		{
			m_pGrid->m_nHeight = 27;
			m_pGrid->Display();
		}
		m_bIsSearched = FALSE;
		EnableWindow(TRUE);
	}

	CDialog::OnTimer(nIDEvent);
}

INT CKL_OTHER_LOAN_DEFAULT::Refresh()
{
	return 0;
}

INT CKL_OTHER_LOAN_DEFAULT::DoRefresh()
{
	
	return 0;
}

BOOL CKL_OTHER_LOAN_DEFAULT::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);
}


INT CKL_OTHER_LOAN_DEFAULT::SetDescDM()
{
	
	CString strGetData, strSetData;
	CESL_DataMgr *pDM = FindDM(m_strDMAlias);
	const INT ROW_CNT = pDM->GetRowCount();
	
	for(INT nRow = 0; nRow < ROW_CNT; nRow++)
	{
		strGetData = pDM->GetCellData(_T("대출도서관"), nRow);
		strSetData = strGetData;
	
		pDM->SetCellData(_T("대출도서관"), strSetData, nRow);
	}
	
	
	return 0;
}
