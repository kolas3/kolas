// MNG_ILL_PERMISSION_LIST_DLG.cpp : implementation file
//

#include "stdafx.h"
// #include "	\ add additional includes here"
#include "MNG_ILL_PERMISSION_LIST_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_ILL_PERMISSION_LIST_DLG dialog


CMNG_ILL_PERMISSION_LIST_DLG::CMNG_ILL_PERMISSION_LIST_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_ILL_PERMISSION_LIST_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ILL_PERMISSION_LIST_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pGrid = NULL;
	m_pCM = NULL;
}


void CMNG_ILL_PERMISSION_LIST_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ILL_PERMISSION_LIST_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ILL_PERMISSION_LIST_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_ILL_PERMISSION_LIST_DLG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ILL_PERMISSION_LIST_DLG message handlers

BEGIN_EVENTSINK_MAP(CMNG_ILL_PERMISSION_LIST_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_ILL_PERMISSION_LIST_DLG)
	ON_EVENT(CMNG_ILL_PERMISSION_LIST_DLG, IDC_grid_MNG_ILL_PERMISSION_LIST, -600 /* Click */, OnClickgridMNGILLPERMISSIONLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMNG_ILL_PERMISSION_LIST_DLG::OnClickgridMNGILLPERMISSIONLIST() 
{
	// TODO: Add your control notification handler code here
	CButton* pBtn_1 = (CButton*)GetDlgItem(IDC_radILL_PERMISSION_USE_ISLOAN);
	CButton* pBtn_2 = (CButton*)GetDlgItem(IDC_radILL_PERMISSION_USER_SETTING);
//	pBtn = /* (_T("") == m_strValue) ? (CButton*)GetDlgItem(uID[0]) :*/ (CButton*)GetDlgItem(uID[1]);
	if(NULL != pBtn_1 || NULL != pBtn_1->GetSafeHwnd()) 
	{
		pBtn_1->SetCheck(FALSE);
	}
	if(NULL != pBtn_2 || NULL != pBtn_2->GetSafeHwnd()) 
	{
		pBtn_2->SetCheck(TRUE);
	}
	int nCol = (int)m_pGrid->GetCol();
	int nRow = (int)m_pGrid->GetRow();
	// FIX도 세기때문에 +1
	if(nCol == (m_pGrid->m_nSelectIdx+1))
	{
		CString strValue = m_pGrid->GetTextMatrix(nRow, nCol);
		m_pDM->SetCellData(_T("신청가능여부"), strValue, nRow-1);		
	}
	
}

BOOL CMNG_ILL_PERMISSION_LIST_DLG::OnInitDialog() 
{
	EnableThemeDialogTexture(GetSafeHwnd()); 

	int	ids = InitESL_Mgr(_T("SM_ILL_PERMISSION_LIST_MGR"));
	if(0 > ids)
	{
		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"SM_ILL_PERMISSION_LIST_MGR\")"));
		CDialog::OnCancel();
	}

	m_pDM = FindDM(_T("DM_ILL_PERMISSION_LIST_MGR"));
	m_pCM = FindCM(_T("CM_ILL_PERMISSION_LIST_MGR"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("PERMISSION_LIST_GRID"));
	
	Refresh();

	UINT uID[2] = 
	{
		IDC_radILL_PERMISSION_USE_ISLOAN, 
		IDC_radILL_PERMISSION_USER_SETTING
	};

	CButton* pBtn = NULL;
	pBtn = (_T("") == m_strValue) ? (CButton*)GetDlgItem(uID[0]) : (CButton*)GetDlgItem(uID[1]);
	if(NULL != pBtn || NULL != pBtn->GetSafeHwnd()) 
	{
		pBtn->SetCheck(TRUE);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMNG_ILL_PERMISSION_LIST_DLG::OnOK() 
{
	// TODO: Add extra validation here
	BOOL bUseIsLoan;
	CButton* pBtn_1 = (CButton*)GetDlgItem(IDC_radILL_PERMISSION_USE_ISLOAN);
	if(NULL != pBtn_1 || NULL != pBtn_1->GetSafeHwnd()) 
	{
		bUseIsLoan = pBtn_1->GetCheck();
	}

	CString strSql;
	// 대출제한설정에 따른다.
	if(TRUE == bUseIsLoan)
	{
		strSql.Format(
			_T("UPDATE ILL_MANAGE_TBL SET VALUE_1 = '' WHERE REC_KEY = %s;"),
			m_strRecKey);
	}
	// 직접 구성한다.
	else
	{
// 		CString strData;
// 		const int ROW_COUNT = m_pDM->GetRowCount();
// 		for(int nRow = 0; nRow < ROW_COUNT; nRow++)
// 		{
// 			CString strIsApl;
// 			m_pDM->GetCellData(_T("신청가능여부"), nRow, strIsApl);
// 			if(_T("") != strIsApl)
// 			{
// 				CString strCode;
// 				m_pDM->GetCellData(_T("코드"), nRow, strCode);
// 
// 				strData = strData + strCode + _T(",");
// 			}
// 		}
// 		if(_T("") == strData)
// 		{
// 			int nID = AfxMessageBox(
// 				_T("선정된 항목이 없어 모든 항목에 대해 신청이 불가능하게 설정됩니다.\r\n")
// 				_T("계속 하시겠습니까?"), MB_YESNO);
// 			if(IDNO == nID) return;
// 
// 			strData = _T(",");
// 		}
		
		// 전체선정으로 한경우 DM에 반영이 되지않은 경우가 있어 CM으로 선택여부 판별
		CString strData;
		m_pGrid->SelectMakeList();
		const INT SELECT_COUNT = m_pGrid->SelectGetCount();
		if(0 == SELECT_COUNT)
		{
 			int nID = AfxMessageBox(
 				_T("선정된 항목이 없어 모든 항목에 대해 신청이 불가능하게 설정됩니다.\r\n")
 				_T("계속 하시겠습니까?"), MB_YESNO);
 			if(IDNO == nID) return;
 
 			strData = _T(",");
		}
		else
		{
			INT nIdx = m_pGrid->SelectGetHeadPosition();
			while(0 <= nIdx && nIdx < m_pDM->GetRowCount())
			{
 				CString strCode;
 				m_pDM->GetCellData(_T("코드"), nIdx, strCode);
				
				//2013-09-08 EDIT BY BJY : 부천도서관 책두레 기능개선
				//코드가 없다면 적용안함으로 코드 셋팅
				if(_T("") == strCode)
				{
					strCode = _T("적용안함");
				}

 				strData = strData + strCode + _T(",");
				nIdx = m_pGrid->SelectGetNext();
			}
		}

		strSql.Format(
			_T("UPDATE ILL_MANAGE_TBL SET VALUE_1 = '%s' WHERE REC_KEY = %s;"),
			strData,
			m_strRecKey);
	}
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	TmpDM.StartFrame();
	TmpDM.AddFrame(strSql);
	TmpDM.SendFrame();
	TmpDM.EndFrame();

	AfxMessageBox(_T("적용되었습니다."));

	CDialog::OnOK();
}

BOOL CMNG_ILL_PERMISSION_LIST_DLG::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}



int CMNG_ILL_PERMISSION_LIST_DLG::Refresh()
{
	if(NULL == m_pDM) return -1;
	if(NULL == m_pGrid) return -2;

	CString strValueType = m_strValueType;
	strValueType.TrimLeft();
	strValueType.TrimRight();
	strValueType.TrimRight(_T(","));

	CString strClass;
	if(_T("자료실코드") == strValueType || _T("자료실") == strValueType)
		strClass = _T("19");
	else if(_T("등록구분코드") == strValueType || _T("등록구분") == strValueType)
		strClass = _T("4");
	else if(_T("별치기호코드") == strValueType || _T("별치기호") == strValueType)
		strClass = _T("11");

	if(_T("") == strClass) return -3;

	CString strOption;
	strOption.Format(_T("C.CLASS = '%s'"), strClass);

	// 2013-09-09 ADD BY BJY : 부천도서관 책두레 기능개선
	// 별치기호가 없는 자료에대한 처리 
	// 적용안함 코드는 별치기호에서만 판단한다.
	if(_T("19") == strClass || _T("4") == strClass)
	{
		strOption.Format(_T("C.CLASS = '%s' AND DESCRIPTION NOT IN ('적용안함')"), strClass);
	}

	m_pDM->RefreshDataManager(strOption);

	CString strCheckValue = m_strValue;
	CString strInCheckValue = _T("");
	for(int nCnt = 0; ; nCnt++)
	{
		if(FALSE == AfxExtractSubString(strInCheckValue, strCheckValue, nCnt, ',')) 
			break;
		
		const int ROW_COUNT = m_pDM->GetRowCount();
		for(int nRow = 0; nRow < ROW_COUNT; nRow++)
		{
			CString strCode;
			m_pDM->GetCellData(_T("코드"), nRow, strCode);
			
			// 2013-09-09 ADD BY BJY : 부천도서관 책두레 기능개선
			// 별치기호가 없는 자료에대한 처리
			if(_T("") == strCode)
			{
				strCode = _T("적용안함");
			}

			if(strCode == strInCheckValue)
			{
				m_pDM->SetCellData(_T("신청가능여부"), m_pGrid->m_sSelectString, nRow);
			}
		}
	}

	m_pGrid->Display();

	return 0;
}


