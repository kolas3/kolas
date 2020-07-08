#include "stdafx.h"
#include "resource.h"
#include "ILL_COMPARE.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CILL_COMPARE::CILL_COMPARE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;
	m_pGridDesc = NULL;
	m_pDlg_Process = NULL;
	m_bIsRunningSearchThread = FALSE;
	m_bIsCancelThread = FALSE;
}
CILL_COMPARE::~CILL_COMPARE()
{
	delete m_pDlg_Process;
	m_pDlg_Process = NULL;
}

void CILL_COMPARE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_COMPARE)
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CILL_COMPARE, CDialog)
	//{{AFX_MSG_MAP(CILL_COMPARE)
	ON_BN_CLICKED(IDC_ILL_COMPARE_ISBN, OnILLCompareIsbn)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(JOB_CANCEL_MSG, StopSearchThread)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CILL_COMPARE, CDialog)
	//{{AFX_EVENTSINK_MAP(CILL_COMPARE)
	ON_EVENT(CILL_COMPARE, IDC_grid_ILL_COMPARE, -600 /* Click */, OnClickgridILLCOMPARE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CILL_COMPARE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int ids = -1;
	CString strFuncName = _T("CILL_COMPARE::OnInitDialog");
	
	((CButton*)GetDlgItem(IDC_ILL_COMPARE_ISBN))->SetCheck(TRUE);

	ids = InitESL_Mgr(_T("ILL_COMPARE"));
	if(0 > ids) AfxMessageBox(_T("ERROR : InitESL_Mgr(\"ILL_COMPARE\")"));

	
	{
	CString strDMAlias;
	CString strCMAlias;
	CString strGridAlias;
	if(IDX_REQUEST == m_Mode)
	{
		strDMAlias = _T("DM_BOOK_COOPERATIVE_GROUP_REQUEST");
		strCMAlias = _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST");
		strGridAlias = _T("그리드_요청자료");
	}
	else if(IDX_OFFER == m_Mode)
	{
		strDMAlias = _T("DM_BOOK_COOPERATIVE_GROUP_OFFER");
		strCMAlias = _T("CM_BOOK_COOPERATIVE_GROUP_OFFER");
		strGridAlias = _T("그리드_제공자료");
	}	
	
	m_pParentDM = FindDM(strDMAlias);
	m_pParentGrid = (CESL_Grid*)FindControl(strCMAlias, strGridAlias);
	if(NULL == m_pParentDM || NULL == m_pParentGrid) CANCEL_DIALOG(-1, strFuncName);

	}
		
	{ 
	CString strBtnCaption;
	if(IDX_REQUEST == m_Mode)
	{
		strBtnCaption = _T("요청거절");
	}
	else if(IDX_OFFER == m_Mode)
	{
		strBtnCaption = _T("발송거절");
	}	
	CWnd* pWnd = GetDlgItem(IDOK);
	if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->SetWindowText(strBtnCaption);
		pWnd->ShowWindow(SW_SHOW);
	}	
	} 
	
	m_pDlg_Process = new CDataProcessingDlg(this);
	m_pDlg_Process->Create(this);

	
	m_pDM = FindDM(_T("DM_ILL_COMPARE"));
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_COMPARE"), _T("그리드_목록"));
	m_pGrid->m_bFroceReverse = FALSE;
	m_pGrid->m_bCheck = FALSE;

	Init_GridDesc();

	RunSearchThread();	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  
	              
}

void CILL_COMPARE::OnILLCompareIsbn() 
{
	
	((CButton*)GetDlgItem(IDC_ILL_COMPARE_ISBN))->SetCheck(TRUE);
}


int CILL_COMPARE::CompareSearch(int nRow)
{
	int ids = -1;
	CString strFuncName = _T("CILL_COMPARE::CompareSearch");

	CString strPublishFormCode;
	m_pParentDM->GetCellData(_T("발행형태구분"), nRow, strPublishFormCode);

	CString strDMAlias = (_T("MO") == strPublishFormCode) ? 
		_T("DM_ILL_COMPARE_MO") : _T("DM_ILL_COMPARE_SE");
	
	CESL_DataMgr* pSearchDM = FindDM(strDMAlias);
	if(NULL == pSearchDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CString strWhere;
	ids = Get_SQLWhere(nRow, strWhere);
	if(0 != ids) return ids;

	ids = pSearchDM->RefreshDataManager(strWhere);
	if(0 > ids) return ids;

	if(0 < pSearchDM->GetRowCount())
	{
		Copy_ParentDM_To_CompareDM(nRow);
		Copy_SearchDM_To_CompareDM(nRow);
	}

	return 0;
}

BOOL CILL_COMPARE::PreTranslateMessage(MSG* pMsg) 
{

	return CESL_Mgr::PreTranslateMessage(pMsg);
}


int CILL_COMPARE::Get_SQLWhere(int nRow, CString &strWhere)
{
	int ids = -1;
	CString strFuncName = _T("CILL_COMPARE::GetSQLWhere");

	CString strPublishFormCode;
	ids = m_pParentDM->GetCellData(_T("발행형태구분"), nRow, strPublishFormCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CString strComapreAlias;
	if(_T("MO") == strPublishFormCode) { strComapreAlias = _T("ISBN"); }
	else if(_T("SE") == strPublishFormCode) { strComapreAlias = _T("ISSN"); }
	else if(_T("NB") == strPublishFormCode) return 1;

	CString strComapreCode;
	m_pParentDM->GetCellData(strComapreAlias, nRow, strComapreCode);
	if(TRUE == strComapreCode.IsEmpty()) return 2;

	CString strBookManageCode;
	ids = m_pParentDM->GetCellData(_T("요청도서관_관리구분"), nRow, strBookManageCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	if(IDX_REQUEST == m_Mode)
	{
		if(strBookManageCode != m_pInfo->MANAGE_CODE) return 3;
	}
	else if(IDX_OFFER == m_Mode)
	{
	}
	else ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

	if(_T("MO") == strPublishFormCode)
	{
		strWhere.Format(
			_T("IB.REC_KEY IN (SELECT DISTINCT ECO_KEY FROM ECO$V_BOSTCODE$I WHERE (TOKEN ='%s' OR TOKEN ='978%s')) AND BB.MANAGE_CODE = '%s'"), 
			strComapreCode, strComapreCode, strBookManageCode);
	}
	else if(_T("SE") == strPublishFormCode)
	{
		strWhere.Format(
			_T("IB.REC_KEY IN (SELECT DISTINCT ECO_KEY FROM ECO$V_SESTISSN$I WHERE (TOKEN ='%s')) AND BB.MANAGE_CODE = '%s'"), 
			strComapreCode, strBookManageCode);
	}

	return 0;
}


int CILL_COMPARE::Copy_ParentDM_To_CompareDM(int nRow)
{
	int ids = -1;
	CString strFuncName = _T("CILL_COMPARE::Copy_ParentDM_To_CompareDM");

	CESL_DataMgr* pCompareDM = FindDM(_T("DM_ILL_COMPARE"));
	if(NULL == pCompareDM) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	
	int nCompareDMRow = pCompareDM->GetRowCount();
	pCompareDM->InsertRow(-1);

	
	
	

	const int ALIAS_COUNT = 7;
	CString strParentDMAlias[ALIAS_COUNT] = 
	{
		_T("트랜잭션번호"), _T("서명"), _T("저자정보"), _T("발행자정보"), _T("발행년도"), _T("등록번호"), _T("대출가능여부")
	};
	CString strCompareDMAlias[ALIAS_COUNT] = 
	{
		_T("트랜잭션번호"), _T("서명"), _T("저자"), _T("발행자"), _T("발행년도"), _T("등록번호"), _T("대출가능여부")
	};










	
	for(int nCnt = 0; nCnt < ALIAS_COUNT; nCnt++)
	{
		CString strGet;
		ids = m_pParentDM->GetCellData(strParentDMAlias[nCnt], nRow, strGet);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		ids = pCompareDM->SetCellData(strCompareDMAlias[nCnt], strGet, nCompareDMRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);
	}

	CString strWorkingStatus;
	ids = m_pParentDM->GetCellData(_T("책_자료상태"), nRow, strWorkingStatus);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}

	ids = CLocCommonAPI::GetWorkingStatusDescToCode(strWorkingStatus, 1);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	}

	ids = pCompareDM->SetCellData(_T("자료상태_설명"), strWorkingStatus, nCompareDMRow);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}




































	return 0;
}


int CILL_COMPARE::Copy_SearchDM_To_CompareDM(int nRow)
{
	int ids = -1;
	CString strFuncName = _T("CILL_COMPARE::Copy_ParentDM_To_CompareDM");

	CESL_DataMgr* pCompareDM = FindDM(_T("DM_ILL_COMPARE"));
	if(NULL == pCompareDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	
	CString strPublishFormCode;
	m_pParentDM->GetCellData(_T("발행형태구분"), nRow, strPublishFormCode);

	CString strDMAlias = (_T("MO") == strPublishFormCode) ? 
		_T("DM_ILL_COMPARE_MO") : _T("DM_ILL_COMPARE_SE");
	
	CESL_DataMgr* pSearchDM = FindDM(strDMAlias);

	const int ADD_ROW_COUNT = pSearchDM->GetRowCount();
	for(int nAddRow = 0; nAddRow < ADD_ROW_COUNT; nAddRow++)
	{
		int nCompareDMRow = pCompareDM->GetRowCount();
		pCompareDM->InsertRow(-1);

		
		

		const int ALIAS_COUNT = 7;
		CString strSearchDMAlias[ALIAS_COUNT] = 
		{
			_T("서명"), _T("저자"), _T("발행자"), _T("발행년도"), _T("등록번호"), _T("자료상태_코드"), _T("대출가능여부")
		};
		CString strCompareDMAlias[ALIAS_COUNT] = 
		{
			_T("서명"), _T("저자"), _T("발행자"), _T("발행년도"), _T("등록번호"), _T("자료상태_코드"), _T("대출가능여부")
		};











		for(int nCnt = 0; nCnt < ALIAS_COUNT; nCnt++)
		{
			CString strGet;
			ids = pSearchDM->GetCellData(strSearchDMAlias[nCnt], nAddRow, strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

			ids = pCompareDM->SetCellData(strCompareDMAlias[nCnt], strGet, nCompareDMRow);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);
		}
		ids = pCompareDM->SetCellData(_T("트랜잭션번호"), _T("**"), nCompareDMRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

		CString strDescWorkingStatus;
		ids = pCompareDM->GetCellData(_T("자료상태_코드"), nCompareDMRow, strDescWorkingStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);

		ids = CLocCommonAPI::GetWorkingStatusDescToCode(strDescWorkingStatus, 1);

		ids = pCompareDM->SetCellData(_T("자료상태_설명"), strDescWorkingStatus, nCompareDMRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);

	}


	return 0;
}


int CILL_COMPARE::Set_GridColor()
{
	const int ROW_COUNT = m_pDM->GetRowCount();

	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		CString strGet;
		m_pDM->GetCellData(_T("트랜잭션번호"), nRow, strGet);

		if(_T("**") != strGet)
		{
			m_pGrid->SetRow(nRow+1);
			const int COL_COUNT = m_pGrid->GetColCount();
			for(int nCol = 1; nCol <= COL_COUNT; nCol++)
			{
				m_pGrid->SetCol(nCol);
				m_pGrid->SetCellBackColor(RGB(240, 128, 128));
			}
		}
	}

	return 0;
}



void CILL_COMPARE::OnClickgridILLCOMPARE() 
{
	
	int nRow = m_pGrid->GetRow()-1;
	int nCol = m_pGrid->GetCol()-1;
	if(nCol != m_pGrid->m_nSelectIdx) return;
	
	CString strTransNo;
	m_pDM->GetCellData(_T("트랜잭션번호"), nRow, strTransNo);
	if(_T("**") != strTransNo)
	{
		CString strValue = (_T("") == m_pGrid->GetAt(nRow, m_pGrid->m_nSelectIdx)) ? 
			m_pGrid->m_sSelectString : _T("");
		m_pGrid->SetAt(nRow, m_pGrid->m_nSelectIdx, strValue);
	}
}



void CILL_COMPARE::OnOK() 
{
	
	if(TRUE == m_bIsRunningSearchThread) return;

	
	int ids = m_pGrid->SelectMakeList();
	if(0 > ids) return ;
	
	if(0 >= m_pGrid->SelectGetCount())
	{
		CString strMsg;
		strMsg.Format(_T("먼저 항목을 선정하여 주십시요."));
		AfxMessageBox(strMsg);
		
		return;
	}

	
	ids = m_pParentGrid->SelectMakeList();
	if(0 > ids) return ;
	
	if(0 < m_pParentGrid->SelectGetCount())
	{
		int nIdx = m_pParentGrid->SelectGetHeadPosition();
		do
		{
			m_pParentGrid->SetAt(
				nIdx,
				m_pParentGrid->m_nSelectIdx,
				_T("")
				);
			nIdx = m_pParentGrid->SelectGetNext();
		}while(0 <= nIdx);
	}

	
	ids = m_pGrid->SelectMakeList();
	if(0 > ids) return ;
	
	if(0 < m_pGrid->SelectGetCount())
	{
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			CString strCompareTransNo;
			m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strCompareTransNo);

			const int ROW_COUNT = m_pParentDM->GetRowCount();
			for(int nRow = 0; nRow < ROW_COUNT; nRow++)
			{
				CString strTransNo;
				m_pParentDM->GetCellData(_T("트랜잭션번호"), nRow, strTransNo);
				
				if(strTransNo == strCompareTransNo)
				{
					m_pParentGrid->SetAt(
						
						
						nRow,
						
						m_pParentGrid->m_nSelectIdx, 
						m_pParentGrid->m_sSelectString
						);
					break;
				}
			}
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);
	}

	CDialog::OnOK();
}

int CILL_COMPARE::SetMode(MODE Mode)
{
	m_Mode = Mode;

	return 0;
}

int CILL_COMPARE::Init_GridDesc()
{
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_grid_ILL_COMPARE_DESC);
	pGrid->SetCol(0);
	pGrid->SetRow(0);
	pGrid->SetCellBackColor(RGB(240, 128, 128));
	pGrid->SetColWidth(1, 0, 4200);

	if(IDX_REQUEST == m_Mode)
	{
		pGrid->SetTextMatrix(0, 1, _T("신청된 자료"));
		pGrid->SetTextMatrix(1, 1, _T("자관의 해당자료 소장목록"));
	}
	else if(IDX_OFFER == m_Mode)
	{
		pGrid->SetTextMatrix(0, 1, _T("요청된 자료"));
		pGrid->SetTextMatrix(1, 1, _T("해당자료 요청도서관의 해당자료 소장목록"));
	}

	return 0;	
}

UINT CILL_COMPARE::SearchThread(LPVOID pParam)
{
	CILL_COMPARE* pDlg = (CILL_COMPARE*)pParam;
	int ids = pDlg->Search();
	if(0 > ids)
	{
		pDlg->SetTimer(IDT_FAIL_SEARCHTHREAD, 100, NULL);
		return ids;
	}

	pDlg->SetTimer(IDT_END_SEARCHTHREAD, 100, NULL);
	
	return 0;
}


int CILL_COMPARE::Search()
{
	int ids = -1;
	CString strFuncName = _T("CILL_COMPARE::Search");
	
	{

	m_pDM->FreeData();
	m_pDlg_Process->CenterWindow();
	m_pDlg_Process->ShowWindow(SW_SHOW);
	m_pDlg_Process->UpdateWindow();	
	m_pDlg_Process->ShowHideCancelButton(TRUE);
	m_pDlg_Process->GetProgressCtrl()->SetRange(0, 0);
	m_pDlg_Process->GetProgressCtrl()->SetPos(0);

	const int COMPARE_CNT = m_pParentGrid->SelectGetCount();
	m_pDlg_Process->SetMessage(_T("소장여부를 조사중입니다."));
	m_pDlg_Process->GetProgressCtrl()->SetRange(0, COMPARE_CNT);
	int nCnt = 0;
	
	m_pParentGrid->SelectGetHeadPosition();
	do
	{
		if(TRUE == m_bIsCancelThread)
		{
			m_pDlg_Process->SetMessage(_T("검색을 취소중입니다."));
			return -2;
		}
		m_pDlg_Process->GetProgressCtrl()->SetPos(nCnt++);
		ids = CompareSearch(m_pParentGrid->SelectGetIdx());
		if(0 > ids) return ids;
	}while(0 < m_pParentGrid->SelectGetNext());

	m_pDlg_Process->ShowWindow(SW_HIDE);
	}
	

	return 0;
}

int CILL_COMPARE::Display()
{
	m_pGrid->Display();
	Set_GridColor();

	return 0;
}

void CILL_COMPARE::OnTimer(UINT nIDEvent) 
{
	
	
	
	
	
	if(IDT_END_SEARCHTHREAD == nIDEvent || IDT_FAIL_SEARCHTHREAD == nIDEvent)
	{	
		KillTimer(nIDEvent);
		
		m_bIsRunningSearchThread = FALSE;
		m_bIsCancelThread = FALSE;
		
		Display();
	}
	
	else if(IDT_DELAY_CANCEL_DIALOG == nIDEvent)
	{
		KillTimer(IDT_DELAY_CANCEL_DIALOG);
		OnCancel();
	}	

	CDialog::OnTimer(nIDEvent);
}

int CILL_COMPARE::RunSearchThread()
{
	if(TRUE == m_bIsRunningSearchThread) return -1;

	m_bIsRunningSearchThread = TRUE;
	m_bIsCancelThread = FALSE;

	CWinThread * Thread;
	Thread = AfxBeginThread(SearchThread, this);	
	
	return 0;
}

int CILL_COMPARE::StopSearchThread()
{
	if(FALSE == m_bIsRunningSearchThread) return -1;
	m_bIsCancelThread = TRUE;

	return 0;
}

void CILL_COMPARE::OnCancel() 
{
	
	
	if(TRUE == m_bIsRunningSearchThread)
	{
		StopSearchThread();
		SetTimer(IDT_DELAY_CANCEL_DIALOG, 1000, NULL);
		return;
	}

	CDialog::OnCancel();
}
