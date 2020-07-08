


#include "stdafx.h"
#include "resource.h"
#include "ILL_PERMISSION_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_PERMISSION_LIST::CILL_PERMISSION_LIST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	

	m_pDM = NULL;
	m_pGrid = NULL;
}


void CILL_PERMISSION_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
		
	
}


BEGIN_MESSAGE_MAP(CILL_PERMISSION_LIST, CDialog)
	//{{AFX_MSG_MAP(CILL_PERMISSION_LIST)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()




BOOL CILL_PERMISSION_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	int	ids = InitESL_Mgr(_T("SM_ILL_PERMISSION_LIST"));
	if(0 > ids)
	{
		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"SM_ILL_PERMISSION_LIST\")"));
		CDialog::OnCancel();
	}

	m_pDM = FindDM(_T("DM_ILL_PERMISSION_LIST"));
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_PERMISSION_LIST"), _T("그리드_신청허용_리스트"));
	
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
	

	return TRUE;  
	              
}

BOOL CILL_PERMISSION_LIST::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

int CILL_PERMISSION_LIST::Refresh()
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
			if(strCode == strInCheckValue)
			{
				m_pDM->SetCellData(_T("신청가능여부"), m_pGrid->m_sSelectString, nRow);
			}
		}
	}

	m_pGrid->Display();

	return 0;
}

BEGIN_EVENTSINK_MAP(CILL_PERMISSION_LIST, CDialog)
	//{{AFX_EVENTSINK_MAP(CILL_PERMISSION_LIST)
	ON_EVENT(CILL_PERMISSION_LIST, IDC_grid_ILL_PERMISSION_LIST, -600 /* Click */, OnClickgridILLPERMISSIONLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CILL_PERMISSION_LIST::OnClickgridILLPERMISSIONLIST() 
{
	
	
	CButton* pBtn_1 = (CButton*)GetDlgItem(IDC_radILL_PERMISSION_USE_ISLOAN);
	CButton* pBtn_2 = (CButton*)GetDlgItem(IDC_radILL_PERMISSION_USER_SETTING);
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
	
	if(nCol == (m_pGrid->m_nSelectIdx+1))
	{
		CString strValue = m_pGrid->GetTextMatrix(nRow, nCol);
		m_pDM->SetCellData(_T("신청가능여부"), strValue, nRow-1);		
	}
}

void CILL_PERMISSION_LIST::OnOK() 
{
	
	BOOL bUseIsLoan;
	CButton* pBtn_1 = (CButton*)GetDlgItem(IDC_radILL_PERMISSION_USE_ISLOAN);
	if(NULL != pBtn_1 || NULL != pBtn_1->GetSafeHwnd()) 
	{
		bUseIsLoan = pBtn_1->GetCheck();
	}

	CString strSql;
	
	if(TRUE == bUseIsLoan)
	{
		strSql.Format(
			_T("UPDATE ILL_MANAGE_TBL SET VALUE_1 = '' WHERE REC_KEY = %s;"),
			m_strRecKey);
	}
	
	else
	{
		CString strData;
		const int ROW_COUNT = m_pDM->GetRowCount();
		for(int nRow = 0; nRow < ROW_COUNT; nRow++)
		{
			CString strIsApl;
			m_pDM->GetCellData(_T("신청가능여부"), nRow, strIsApl);
			if(_T("") != strIsApl)
			{
				CString strCode;
				m_pDM->GetCellData(_T("코드"), nRow, strCode);

				strData = strData + strCode + _T(",");
			}
		}

		if(_T("") == strData)
		{
			int nID = AfxMessageBox(
				_T("선정된 항목이 없어 모든 항목에 대해 신청이 불가능하게 설정됩니다.\r\n")
				_T("계속 하시겠습니까?"), MB_YESNO);
			if(IDNO == nID) return;

			strData = _T(",");
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

