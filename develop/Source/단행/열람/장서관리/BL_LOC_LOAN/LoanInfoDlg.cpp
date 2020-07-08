// LoanInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanInfoDlg.h"
#include "..\\..\\..\\..\\공통\\K2UP_Common\\MakeSearchData.h"
#include "BO_LOC_RECEIPT_PRINT_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanInfoDlg dialog

UINT SearchLoanInfoThread(LPVOID lParam);

CLoanInfoDlg::CLoanInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pManageValue = NULL;
	m_pGrid = NULL;
	m_nMode = 0;
	m_nIdx = 0;
	m_bIsRunningThread = FALSE;
	m_bIsPrinting = FALSE;

	m_strUserKey = _T("");
	m_pUserDM = NULL;	
	m_astrSearchLoanQuery[0] = _T("");
	m_astrSearchLoanQuery[1] = _T("");
}

CLoanInfoDlg::~CLoanInfoDlg()
{

}

void CLoanInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanInfoDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_BUTTON_LOAN_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_RADIO_LOAN1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO_LOAN2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO_LOAN3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO_LOAN4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO_LOAN5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO_LOAN6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO_LOAN7, OnRadio7)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnLOANPRINT, OnbtnPRINT)
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP	
	ON_MESSAGE(WM_USER + 1029, OnPrint)
	ON_COMMAND_RANGE(4000, 4005, UserSelectProc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanInfoDlg message handlers

BOOL CLoanInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			if(IDC_EDIT_LOAN2 == GetFocus()->GetDlgCtrlID())
			{
				OnButton5();
				return TRUE;
			}
			
			OnbtnSEARCH();
			return TRUE;
		}		
		else if(VK_PRIOR == pMsg->wParam || VK_UP == pMsg->wParam)
		{
			OnButton2();
			return TRUE;
		}
		else if(VK_NEXT == pMsg->wParam || VK_DOWN == pMsg->wParam)
		{
			OnButton3();
			return TRUE;
		}
		else if(VK_HOME == pMsg->wParam/* || VK_LEFT == pMsg->wParam*/)
		{
			DisplayGrid(0);
		}
		else if(VK_END == pMsg->wParam/* || VK_RIGHT == pMsg->wParam*/)
		{
			DisplayGrid(-1);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLoanInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());
	m_pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	
	// TODO: Add extra initialization here
	m_pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_grid1);

	m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	if(m_nMode)
	{
		SetWindowText(_T("연체자료안내"));

		SetDlgItemText(IDC_RADIO_LOAN4, _T("최근 1달"));
		SetDlgItemText(IDC_RADIO_LOAN5, _T("최근 2달"));
		SetDlgItemText(IDC_RADIO_LOAN6, _T("최근 6달"));
	}
	else
	{
		SetWindowText(_T("대출이력안내"));
	}

	m_astrSearchLoanQuery[0] = 		

		_T("SELECT DECODE(BB.BOOK_APPENDIX_FLAG,'A',BB.VOL_TITLE,'B',IB.TITLE_INFO),")
			_T("IB.AUTHOR_INFO, ")
			_T("CL.LOAN_DATE, CL.RETURN_PLAN_DATE, ")
			_T("CL.STATUS, CL.REC_KEY, CL.RESERVATION_DATE, ")
			_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE, ")
			_T("DECODE(CL.LOAN_DATE, NULL, CL.RESERVATION_DATE, CL.LOAN_DATE) AS SORT_DAY, ")
			_T("DECODE(CL.STATUS, '0', '대출', '1', '반납', '2', '반납연기', '3', '예약', '4', '예약취소', '5', '무인반납중', '6', '분관반납중', '7', '타관반납중', 'L', '책두레대출', 'R', '책두레반납', 'D', '책두레반납연기'), ")
			_T("CL.RETURN_DATE, ")
			_T("<?DELAY_DAY?>, ")
			_T("(SELECT MN.LIB_NAME FROM MN_LIBINFO2_TBL MN WHERE MN.MANAGE_CODE = CL.MANAGE_CODE), ")
			_T("(SELECT NVL(MAX(CD.DESCRIPTION), CL.SHELF_LOC_CODE) FROM CD_CODE_TBL CD WHERE CD.CLASS = 19 AND CD.CODE = CL.SHELF_LOC_CODE) ")
		_T("FROM LS_WORK_T01 CL, BO_BOOK_TBL BB, IDX_BO_TBL IB ")
		_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
			_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
			_T("AND CL.USER_KEY=<?USER_KEY?> ")
			_T("<?WHERE_QUERY?> ")
			_T("<?BO_QUERY?> ")
			_T("AND CL.PUBLISH_FORM_CODE='MO'")
		_T(" UNION ALL ")
		_T("SELECT DECODE(BB.BOOK_APPENDIX_FLAG,'A',BB.VOL_TITLE,'B',IB.TITLE_INFO),")
			_T("IB.AUTHOR_INFO, ")
			_T("CL.LOAN_DATE, CL.RETURN_PLAN_DATE, ")
			_T("CL.STATUS, CL.REC_KEY, CL.RESERVATION_DATE, ")
			_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE, ")
			_T("DECODE(CL.LOAN_DATE, NULL, CL.RESERVATION_DATE, CL.LOAN_DATE) AS SORT_DAY, ")
			_T("DECODE(CL.STATUS, '0', '대출', '1', '반납', '2', '반납연기', '3', '예약', '4', '예약취소', '5', '무인반납중', '6', '분관반납중', '7', '타관반납중', 'L', '책두레대출', 'R', '책두레반납', 'D', '책두레반납연기'), ")
			_T("CL.RETURN_DATE, ")
			_T("<?DELAY_DAY?>, ")
			_T("(SELECT MN.LIB_NAME FROM MN_LIBINFO2_TBL MN WHERE MN.MANAGE_CODE = CL.MANAGE_CODE), ")
			_T("(SELECT NVL(MAX(CD.DESCRIPTION), CL.SHELF_LOC_CODE) FROM CD_CODE_TBL CD WHERE CD.CLASS = 19 AND CD.CODE = CL.SHELF_LOC_CODE) ")
		_T("FROM CO_LOAN_TBL CL, BO_BOOK_TBL BB, IDX_BO_TBL IB ")
		_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
			_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
			_T("AND CL.USER_KEY=<?USER_KEY?> ")
 			_T("<?WHERE_QUERY?> ") 			
			_T("<?BO_QUERY?> ")
			_T("AND CL.PUBLISH_FORM_CODE='MO'")
		_T(" UNION ALL ")
		_T("SELECT DECODE(BB.BOOK_TYPE,'A',V.VOL_TITLE,'B',IB.TITLE_INFO,'C',IB.TITLE_INFO),")
			_T("IB.AUTHOR_INFO, ")
			_T("CL.LOAN_DATE, CL.RETURN_PLAN_DATE, ")
			_T("CL.STATUS, CL.REC_KEY, CL.RESERVATION_DATE, ")
			_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE, ")
			_T("DECODE(CL.LOAN_DATE, NULL, CL.RESERVATION_DATE, CL.LOAN_DATE) AS SORT_DAY, ")
			_T("DECODE(CL.STATUS, '0', '대출', '1', '반납', '2', '반납연기', '3', '예약', '4', '예약취소', '5', '무인반납중', '6', '분관반납중', '7', '타관반납중', 'L', '책두레대출', 'R', '책두레반납', 'D', '책두레반납연기'), ")
			_T("CL.RETURN_DATE, ")
			_T("<?DELAY_DAY?>, ")
			_T("(SELECT MN.LIB_NAME FROM MN_LIBINFO2_TBL MN WHERE MN.MANAGE_CODE = CL.MANAGE_CODE), ")
			_T("(SELECT NVL(MAX(CD.DESCRIPTION), CL.SHELF_LOC_CODE) FROM CD_CODE_TBL CD WHERE CD.CLASS = 19 AND CD.CODE = CL.SHELF_LOC_CODE) ")
		_T("FROM LS_WORK_T01 CL, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V ")
		_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
			_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
			_T("AND BB.VOL_KEY=V.REC_KEY ")
			_T("AND CL.USER_KEY=<?USER_KEY?> ")
 			_T("<?WHERE_QUERY?> ") 			
			_T("<?SE_QUERY?> ")
			_T("AND CL.PUBLISH_FORM_CODE='SE'")
		_T(" UNION ALL ")
		_T("SELECT DECODE(BB.BOOK_TYPE,'A',V.VOL_TITLE,'B',IB.TITLE_INFO,'C',IB.TITLE_INFO),")
			_T("IB.AUTHOR_INFO, ")
			_T("CL.LOAN_DATE, CL.RETURN_PLAN_DATE, ")
			_T("CL.STATUS, CL.REC_KEY, CL.RESERVATION_DATE, ")
			_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE, ")
			_T("DECODE(CL.LOAN_DATE, NULL, CL.RESERVATION_DATE, CL.LOAN_DATE) AS SORT_DAY, ")
			_T("DECODE(CL.STATUS, '0', '대출', '1', '반납', '2', '반납연기', '3', '예약', '4', '예약취소', '5', '무인반납중', '6', '분관반납중', '7', '타관반납중', 'L', '책두레대출', 'R', '책두레반납', 'D', '책두레반납연기'), ")
			_T("CL.RETURN_DATE, ")
			_T("<?DELAY_DAY?>, ")
			_T("(SELECT MN.LIB_NAME FROM MN_LIBINFO2_TBL MN WHERE MN.MANAGE_CODE = CL.MANAGE_CODE), ")
			_T("(SELECT NVL(MAX(CD.DESCRIPTION), CL.SHELF_LOC_CODE) FROM CD_CODE_TBL CD WHERE CD.CLASS = 19 AND CD.CODE = CL.SHELF_LOC_CODE) ")
		_T("FROM CO_LOAN_TBL CL, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V ")
		_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
			_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
			_T("AND BB.VOL_KEY=V.REC_KEY ")
			_T("AND CL.USER_KEY=<?USER_KEY?> ")
 			_T("<?WHERE_QUERY?> ") 			
			_T("<?SE_QUERY?> ")
			_T("AND CL.PUBLISH_FORM_CODE='SE'")
		_T(" UNION ALL ")
		_T("SELECT NB.TITLE,")
			_T("NB.AUTHOR,")
			_T("CL.LOAN_DATE, CL.RETURN_PLAN_DATE, ")
			_T("CL.STATUS, CL.REC_KEY, CL.RESERVATION_DATE, ")
			_T("REPLACE(NB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE, ")
			_T("DECODE(CL.LOAN_DATE, NULL, CL.RESERVATION_DATE, CL.LOAN_DATE) AS SORT_DAY, ")
			_T("DECODE(CL.STATUS, '0', '대출', '1', '반납', '2', '반납연기', '3', '예약', '4', '예약취소', '5', '무인반납중', '6', '분관반납중', '7', '타관반납중', 'L', '책두레대출', 'R', '책두레반납', 'D', '책두레반납연기'), ")
			_T("CL.RETURN_DATE, ")
			_T("<?DELAY_DAY?>, ")
			_T("(SELECT MN.LIB_NAME FROM MN_LIBINFO2_TBL MN WHERE MN.MANAGE_CODE = CL.MANAGE_CODE), ")
			_T("(SELECT NVL(MAX(CD.DESCRIPTION), CL.SHELF_LOC_CODE) FROM CD_CODE_TBL CD WHERE CD.CLASS = 19 AND CD.CODE = CL.SHELF_LOC_CODE) ")
		_T("FROM LS_WORK_T01 CL, CO_NON_DB_BOOK_TBL NB ")
		_T("WHERE CL.BOOK_KEY=NB.REC_KEY ")
			_T("AND CL.USER_KEY=<?USER_KEY?> ")
 			_T("<?WHERE_QUERY?> ") 	
			_T("<?NB_QUERY?> ")
			_T("AND CL.PUBLISH_FORM_CODE='NB' ")
		_T(" UNION ALL ")
		_T("SELECT NB.TITLE,")
			_T("NB.AUTHOR,")
			_T("CL.LOAN_DATE, CL.RETURN_PLAN_DATE, ")
			_T("CL.STATUS, CL.REC_KEY, CL.RESERVATION_DATE, ")
			_T("REPLACE(NB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE, ")
			_T("DECODE(CL.LOAN_DATE, NULL, CL.RESERVATION_DATE, CL.LOAN_DATE) AS SORT_DAY, ")
			_T("DECODE(CL.STATUS, '0', '대출', '1', '반납', '2', '반납연기', '3', '예약', '4', '예약취소', '5', '무인반납중', '6', '분관반납중', '7', '타관반납중', 'L', '책두레대출', 'R', '책두레반납', 'D', '책두레반납연기'), ")
			_T("CL.RETURN_DATE, ")
			_T("<?DELAY_DAY?>, ")
			_T("(SELECT MN.LIB_NAME FROM MN_LIBINFO2_TBL MN WHERE MN.MANAGE_CODE = CL.MANAGE_CODE), ")
			_T("(SELECT NVL(MAX(CD.DESCRIPTION), CL.SHELF_LOC_CODE) FROM CD_CODE_TBL CD WHERE CD.CLASS = 19 AND CD.CODE = CL.SHELF_LOC_CODE) ")
		_T("FROM LS_WORK_T01 CL, CO_NON_DB_BOOK_TBL NB ")
		_T("WHERE CL.BOOK_KEY=NB.REC_KEY ")
			_T("AND CL.USER_KEY=<?USER_KEY?> ")
 			_T("<?WHERE_QUERY?> ") 
			_T("<?NB_QUERY?> ")
			_T("AND CL.PUBLISH_FORM_CODE='NB' ")
		_T("ORDER BY SORT_DAY DESC, STATUS, REC_KEY DESC");
	
	InitGrid();	

	((CButton*)GetDlgItem(IDC_RADIO_LOAN1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_LOAN3))->SetCheck(TRUE);

	((CEdit*)GetDlgItem(IDC_EDIT_LOAN2))->SetLimitText(5);
	SetDlgItemText(IDC_EDIT_LOAN2, _T("0"));


	SetDlgItemText(IDC_EDIT_LOAN4, 
		_T("단축키 : ↑/PageUp(이전 페이지)    ↓/PageDown(다음 페이지)\r\n")
		_T("            Home(첫 페이지)    End(마지막 페이지)")
		);	

	if(_T("Y") == m_pManageValue->m_sUseReceipt)
	{
		GetDlgItem(IDC_btnLOANPRINT)->ShowWindow(SW_SHOW);
	}

	OnbtnSEARCH();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoanInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CLoanInfoDlg::SearchLoanInfo()
{
	if(m_bIsRunningThread)
	{
		return;
	}	
	AfxBeginThread(SearchLoanInfoThread, this);	
}

UINT SearchLoanInfoThread(LPVOID lParam)
{	
	CLoanInfoDlg* pDlg = (CLoanInfoDlg*)lParam;
	pDlg->m_bIsRunningThread = TRUE;
	pDlg->GetDlgItem(IDC_btnSEARCH)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	
	pDlg->SearchLoanInfo(TRUE);

	pDlg->GetDlgItem(IDC_btnSEARCH)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CLoanInfoDlg::SearchLoanInfo(BOOL bInvalidate)
{
	if(0 == m_pUserDM->GetRowCount())
	{
		return 1000;
	}

	CString strQuery;
	INT ids = MakeQuery(strQuery);
	
	if(0 != ids)
	{
		return ids;
	}

	ShowMessage(_T("검색중입니다."), INFINITE);
	m_DM.RestructDataManager(strQuery); 

	INT nMax = m_DM.GetRowCount();
	if(0 == nMax)
	{
		FreeGrid();
		if(TRUE == bInvalidate)
		{
			Invalidate();
		}		
		ShowMessage(_T("검색된 결과가 없습니다."));

		return 0;
	}

	DisplayGrid(0);

	if(TRUE == bInvalidate)
	{
		Invalidate();
	}
	ShowMessage(_T("검색되었습니다."));

	return 0;
}

void CLoanInfoDlg::FreeGrid()
{
	m_pGrid->SetRedraw(FALSE);

	m_pGrid->SetRows(2);
	INT cols = m_pGrid->GetCols(0);
	for (INT j = 0 ; j < cols ; j++) 
	{
		if(0 != j)
		{
			m_pGrid->SetCol(j);
			m_pGrid->SetCellBackColor(RGB(255,255,255));
			m_pGrid->SetCellForeColor(RGB(0,0,0));
			m_pGrid->SetCellFontBold(FALSE);
			m_pGrid->SetCellFontItalic(FALSE);
		}
		m_pGrid->SetTextMatrix(1, j, _T(""));
	}
	m_pGrid->Refresh();
	m_pGrid->SetRedraw(TRUE);
}

void CLoanInfoDlg::OnbtnSEARCH() 
{
	// TODO: Add your control notification handler code here
	SearchLoanInfo();
}

void CLoanInfoDlg::InitGrid()
{

	COleFont font = m_pGrid->GetFont();
	CY cy = font.GetSize();
	cy.Lo = 90000;
	font.SetName(_T("새굴림"));
	font.SetItalic(FALSE);	
	font.SetUnderline(FALSE);
	font.SetStrikethrough(FALSE);		
	font.SetSize(cy);
	font.SetBold(FALSE);
	m_pGrid->SetRefFont(font);
	
	m_pGrid->SetRowHeight(-1, 430);	
	m_pGrid->SetRowHeight(0, 391);

	if(0 == m_nMode)
	{
		const INT COL_CNT = 8;
		CString strCaption[COL_CNT] =
		{
	 		_T(""), _T("선정"), _T("서명"), _T("대출일"), _T("반납예정일"), _T("상태"), _T("관리구분"), _T("자료실")
		};
 		INT nColWidth[COL_CNT] = { 30, 0, 200, 80, 80, 100, 100, 100 };
		if(_T("Y") == m_pManageValue->m_sUseReceipt)
		{
			nColWidth[1] = 20;
		}
	 	INT nColAlignment[COL_CNT] = {7,4,1,4,4,4,1,1};

		m_pGrid->SetCols(0, COL_CNT); 
		m_pGrid->SetRow(0);
		INT nColCnt = (INT)m_pGrid->GetCols(0);
		for(INT nCnt = 0; nCnt < nColCnt; nCnt++)
		{
			m_pGrid->SetCol(nCnt);
			m_pGrid->SetColWidth(nCnt, 0, nColWidth[nCnt] * 15);
			if(0 == nCnt)
			{
				m_pGrid->SetColAlignmentFixed(nCnt, 7);
			}
			else
			{
				m_pGrid->SetColAlignmentFixed(nCnt, 4);
			}
			m_pGrid->SetColAlignment(nCnt, nColAlignment[nCnt]);
			m_pGrid->SetTextMatrix(0, nCnt, strCaption[nCnt]);
		}
	}
	else
	{
		const INT COL_CNT = 9;
		CString strCaption[COL_CNT] =
		{
	 		_T(""), _T("선정"), _T("서명"), _T("대출일"), _T("반납예정일"), _T("반납일"), _T("연체일"), _T("관리구분"), _T("자료실")
		};
 		INT nColWidth[COL_CNT] = { 30, 0, 170, 80, 80, 80, 50, 100, 100 };
		if(_T("Y") == m_pManageValue->m_sUseReceipt)
		{
			nColWidth[1] = 20;
		}
	 	INT nColAlignment[COL_CNT] = {7, 4, 1, 4, 4, 4, 7, 1};

		m_pGrid->SetCols(0, COL_CNT); 
		m_pGrid->SetRow(0);
		INT nColCnt = (INT)m_pGrid->GetCols(0);
		for(INT nCnt = 0; nCnt < nColCnt; nCnt++)
		{
			m_pGrid->SetCol(nCnt);
			m_pGrid->SetColWidth(nCnt, 0, nColWidth[nCnt] * 15);
			if(0 == nCnt)
			{
				m_pGrid->SetColAlignmentFixed(nCnt, 7);
			}
			else
			{
				m_pGrid->SetColAlignmentFixed(nCnt, 4);
			}
			m_pGrid->SetColAlignment(nCnt, nColAlignment[nCnt]);
			m_pGrid->SetTextMatrix(0, nCnt, strCaption[nCnt]);
		}
	}

	m_pManageValue->m_sReserveMode;
}

void CLoanInfoDlg::SetMode(INT nMode)
{
	m_nMode = nMode;
}

void CLoanInfoDlg::DisplayGrid(INT nIdx)
{	
	const INT nMax = m_DM.GetRowCount();
	INT nStart = nIdx * MAX_GRID_ROW2;
	INT nEnd = nStart+(MAX_GRID_ROW2-1);
	
	if(0 != nMax)
	{
		if(-1 == nIdx)
		{
			nIdx = nMax / MAX_GRID_ROW2;
			if(0 == nMax % MAX_GRID_ROW2)
			{
				nIdx--;
			}
			nStart = nIdx * MAX_GRID_ROW2;
			nEnd = nStart+(MAX_GRID_ROW2-1);
		}	
		if(0 > nStart || nStart >= nMax)
		{
			return;
		}
		if(nEnd >= nMax)// && 0 != nMax % MAX_GRID_ROW2)
		{
			m_pGrid->SetRows(MAX_GRID_ROW2 - (nEnd - nMax));
			nEnd = nMax-1;
		}
		else
		{
			m_pGrid->SetRows(MAX_GRID_ROW2+1);
		}
	}
	else
	{
		FreeGrid();
	}

	m_pGrid->SetRowHeight(-1, 430);	
	m_pGrid->SetRowHeight(0, 391);

	CString strNo;
	for(INT nRow = nStart, nCnt = 0; nRow <= nEnd; nRow++, nCnt++)
	{
		if(0 == m_nMode)
		{
			strNo.Format(_T("%d"), nRow+1);
			m_pGrid->SetTextMatrix(nCnt+1, 0, strNo);
 			m_pGrid->SetTextMatrix(nCnt+1, 1, _T(""));
 			m_pGrid->SetTextMatrix(nCnt+1, 2, m_DM.GetCellData(nRow, 0));
 			m_pGrid->SetTextMatrix(nCnt+1, 3, m_DM.GetCellData(nRow, 2));
			m_pGrid->SetTextMatrix(nCnt+1, 4, m_DM.GetCellData(nRow, 3));
 			m_pGrid->SetTextMatrix(nCnt+1, 5, m_DM.GetCellData(nRow, 9));	
			m_pGrid->SetTextMatrix(nCnt+1, 6, m_DM.GetCellData(nRow, 12));
			m_pGrid->SetTextMatrix(nCnt+1, 7, m_DM.GetCellData(nRow, 13));
		}
		else
		{
			strNo.Format(_T("%d"), nRow+1);
			m_pGrid->SetTextMatrix(nCnt+1, 0, strNo);
 			m_pGrid->SetTextMatrix(nCnt+1, 1, _T(""));
 			m_pGrid->SetTextMatrix(nCnt+1, 2, m_DM.GetCellData(nRow, 0));
 			m_pGrid->SetTextMatrix(nCnt+1, 3, m_DM.GetCellData(nRow, 2));
 			m_pGrid->SetTextMatrix(nCnt+1, 4, m_DM.GetCellData(nRow, 3));
 			m_pGrid->SetTextMatrix(nCnt+1, 5, m_DM.GetCellData(nRow, 10));	
			m_pGrid->SetTextMatrix(nCnt+1, 6, m_DM.GetCellData(nRow, 11));
			m_pGrid->SetTextMatrix(nCnt+1, 7, m_DM.GetCellData(nRow, 12));
			m_pGrid->SetTextMatrix(nCnt+1, 8, m_DM.GetCellData(nRow, 13));
		}
	}

	INT nMaxIdx = nMax / MAX_GRID_ROW2;
	INT nTmp = nMax % MAX_GRID_ROW2;

	CString strIdx;
	strIdx.Format(_T("%d / %d"), nIdx+1, (0 == nTmp) ? nMaxIdx : nMaxIdx+1);

	SetDlgItemText(IDC_STATIC2, strIdx);

	if(0 >= nIdx)
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH)->SetFocus();		
	}
	else
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
	if(nIdx >= nMaxIdx || (nIdx >= nMaxIdx-1 && 0 == nTmp))
	{
		GetDlgItem(IDC_BUTTON_LOAN_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH)->SetFocus();		
	}
	else
	{
		GetDlgItem(IDC_BUTTON_LOAN_3)->EnableWindow(TRUE);
	}
	m_nIdx = nIdx;
	if(0 == m_nMode)
	{
		ViewGridThread();
	}
}

void CLoanInfoDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if(GetDlgItem(IDC_BUTTON_LOAN_3)->IsWindowEnabled())
	{
		DisplayGrid(m_nIdx+1);
	}
}

void CLoanInfoDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	if(GetDlgItem(IDC_BUTTON2)->IsWindowEnabled())
	{
		DisplayGrid(m_nIdx-1);
	}
}

void CLoanInfoDlg::OnRadio1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_LOAN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);

	GetDlgItem(IDC_RADIO_LOAN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LOAN4)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LOAN5)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LOAN6)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LOAN7)->EnableWindow(TRUE);	
}

void CLoanInfoDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_LOAN1)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC1)->EnableWindow(TRUE);

	GetDlgItem(IDC_RADIO_LOAN3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LOAN4)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LOAN5)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LOAN6)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LOAN7)->EnableWindow(FALSE);	
}

void CLoanInfoDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	
}

void CLoanInfoDlg::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	
}

void CLoanInfoDlg::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	
}

void CLoanInfoDlg::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	
}

void CLoanInfoDlg::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	
}

void CLoanInfoDlg::OnButton5()
{
	// TODO: Add your control notification handler code here
	UINT uIdx = GetDlgItemInt(IDC_EDIT_LOAN2, NULL, FALSE);
	if(0 >= uIdx)
	{

	}
	else
	{
		DisplayGrid(uIdx-1);
	}
}


INT CLoanInfoDlg::MakeQuery(CString& strQuery)
{
	m_pUserDM->GetCellData(_T("대출자KEY"), 0, m_strUserKey);

	strQuery = m_astrSearchLoanQuery[0];
	strQuery.Replace(_T("<?USER_KEY?>"), m_strUserKey);
	
	CString strWhere, strBOWhere, strSEWhere, strNBWhere;
	if(0 == m_nMode)
	{
		strWhere = _T("AND CL.STATUS IN('0','1','2','3','4','L','R','D') ");
	}
	else
	{
		strWhere = _T("AND CL.STATUS IN('0','1','2','L','R','D') ");
	}

	CString sLoanDate;
	GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), sLoanDate);
	
	if(sLoanDate.IsEmpty())
	{
		sLoanDate = _T("TRUNC(SYSDATE)");
	}
	else
	{
		sLoanDate = _T("TO_DATE('") + sLoanDate +_T("')");
	}

	CString strTmp;
	if(TRUE == ((CButton*)GetDlgItem(IDC_RADIO_LOAN1))->GetCheck())
	{
		if(TRUE == ((CButton*)GetDlgItem(IDC_RADIO_LOAN3))->GetCheck())
		{
			if(0 == m_nMode)
			{
				strWhere = _T("AND CL.STATUS IN('0','2','3','L','D') ");
			}
			else
			{
				strWhere.Format(_T("AND CL.STATUS IN('0','2','L','D') AND CL.RETURN_PLAN_DATE < %s "), sLoanDate);
			}			
		}
		else if(TRUE == ((CButton*)GetDlgItem(IDC_RADIO_LOAN4))->GetCheck())
		{
			if(0 == m_nMode)
			{
				strTmp.Format(_T("AND ( (CL.LOAN_DATE BETWEEN TRUNC(SYSDATE-6) AND TRUNC(SYSDATE)+0.99999421) OR (CL.RESERVATION_DATE BETWEEN TRUNC(SYSDATE-6) AND TRUNC(SYSDATE)+0.99999421) )"));
			}
			else
			{
				strTmp.Format(_T("AND ( ( (CL.LOAN_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-1) AND TRUNC(SYSDATE)+0.99999421) AND (CL.RETURN_PLAN_DATE < TRUNC(CL.RETURN_DATE)) ) OR (CL.RETURN_DATE IS NULL AND CL.RETURN_PLAN_DATE < %s) )"), sLoanDate);
			}
			strWhere += strTmp;
		}
		else if(TRUE == ((CButton*)GetDlgItem(IDC_RADIO_LOAN5))->GetCheck())
		{
			if(0 == m_nMode)
			{
				strTmp.Format(_T("AND ( (CL.LOAN_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-1) AND TRUNC(SYSDATE)+0.99999421) OR (CL.RESERVATION_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-1) AND TRUNC(SYSDATE)+0.99999421) )"));
			}
			else
			{
				strTmp.Format(_T("AND ( ( (CL.LOAN_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-2) AND TRUNC(SYSDATE)+0.99999421) AND (CL.RETURN_PLAN_DATE < TRUNC(CL.RETURN_DATE)) ) OR (CL.RETURN_DATE IS NULL AND CL.RETURN_PLAN_DATE < %s) )"), sLoanDate);
			}
			strWhere += strTmp;
		}
		else if(TRUE == ((CButton*)GetDlgItem(IDC_RADIO_LOAN6))->GetCheck())
		{
			if(0 == m_nMode)
			{
				strTmp.Format(_T("AND ( (CL.LOAN_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-12) AND TRUNC(SYSDATE)+0.99999421) OR (CL.RESERVATION_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-12) AND TRUNC(SYSDATE)+0.99999421) )"));
			}
			else
			{
				strTmp.Format(_T("AND ( ( (CL.LOAN_DATE BETWEEN ADD_MONTHS(TRUNC(SYSDATE+1),-6) AND TRUNC(SYSDATE)+0.99999421) AND (CL.RETURN_PLAN_DATE < TRUNC(CL.RETURN_DATE)) ) OR (CL.RETURN_DATE IS NULL AND CL.RETURN_PLAN_DATE < %s) )"), sLoanDate);
			}
			strWhere += strTmp;
		}
		else if(TRUE == ((CButton*)GetDlgItem(IDC_RADIO_LOAN7))->GetCheck())
		{
			if(0 == m_nMode)
			{
			}
			else
			{
				strTmp.Format(_T("AND ( (CL.RETURN_PLAN_DATE < TRUNC(CL.RETURN_DATE)) OR (CL.RETURN_DATE IS NULL AND CL.RETURN_PLAN_DATE < %s) ) "), sLoanDate);
				strWhere += strTmp;
			}
		}
	}
	else
	{
		CMakeSearchData MakeSearchData(this);	
		
		BOOL bIsEqualSearch = FALSE;
		
		CString strData;
		GetDlgItemText(IDC_EDIT_LOAN1, strData);

		if( !strData.IsEmpty() )
		{
			strNBWhere.Format(_T("AND NB.TITLE = '%s' "), strData);

			if( strData.GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(strData);
			if( !bIsEqualSearch ) 
				strData += _T("*");
			
			if(1 == m_pInfo->GetSearchIndexType())
			{
				FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));
				if(fd != NULL)
				{
					fclose(fd);
					strSEWhere = _T("AND IB.REC_KEY IN &ECOSEARCH(IB.REC_KEY, SE_IDX_TITLE, '") + strData + _T("') ");
					strBOWhere = _T("AND IB.REC_KEY IN &ECOSEARCH(IB.REC_KEY, BO_IDX_TITLE, '") + strData + _T("') ");
				}
				else
				{
					strSEWhere = _T("AND IB.REC_KEY IN &ECOSEARCH( , SE_IDX_TITLE, '") + strData + _T("') ");
					strBOWhere = _T("AND IB.REC_KEY IN &ECOSEARCH( , BO_IDX_TITLE, '") + strData + _T("') ");					
				}
			}
			else
			{
				strSEWhere = _T("AND IB.REC_KEY IN CATSEARCH(IB.IDX_TITLE, '") + strData + _T("',NULL) > 0 ");
				strBOWhere = _T("AND IB.REC_KEY IN CATSEARCH(IB.IDX_TITLE, '") + strData + _T("',NULL) > 0 ");
			}
		}
		else
		{
			ShowMessage(_T("검색조건을 입력하여 주십시요."));
			return 1000;
		}
	}

	if(1 == m_nMode)
	{

	}

	strTmp.Format(_T("DECODE(CL.RETURN_DATE, NULL, %s-TRUNC(CL.RETURN_PLAN_DATE), TRUNC(CL.RETURN_DATE)-TRUNC(CL.RETURN_PLAN_DATE))"), sLoanDate);

	strQuery.Replace(_T("<?WHERE_QUERY?>"), strWhere);
	strQuery.Replace(_T("<?BO_QUERY?>"), strBOWhere);
	strQuery.Replace(_T("<?SE_QUERY?>"), strSEWhere);
	strQuery.Replace(_T("<?NB_QUERY?>"), strNBWhere);
	strQuery.Replace(_T("<?DELAY_DAY?>"), strTmp);	

	return 0;
}

void CLoanInfoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		KillTimer(1);
		ShowMessage(_T(""), INFINITE);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CLoanInfoDlg::ShowMessage(LPCTSTR lpszText, UINT uTime) 
{
	SetDlgItemText(IDC_EDIT_LOAN3, lpszText);
	if(INFINITE != uTime)
	{
		SetTimer(1, uTime, NULL);
	}
}

INT CLoanInfoDlg::ViewGridThread()
{
EFS_BEGIN

	INT ids;
	CString sStatusDesc;
	CString sBgColor;
	CString sFontColor;
	COLORREF crBgColor, crFontColor;


	CString sColorDMAlias = _T("DM_BO_LOC_3100_COLOR_SETTING");
	CESL_DataMgr *pColorDM = FindDM(sColorDMAlias);
	if(NULL == pColorDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("ViewGridThread"));
	}
	

	if(0 == m_DM.GetRowCount())
	{
		return 0;
	}


	m_pGrid->SetRedraw(FALSE);

	INT nPreCol = m_pGrid->GetCol();
	for(INT i = 1; i < m_pGrid->GetRows(); i++)
	{
		m_pGrid->SetRow(i);

		CString sStatus;
		CString sDelayDay;
		ids = m_DM.GetCellData(i-1 + (MAX_GRID_ROW2*m_nIdx), 9, sStatus);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("ViewGridThread"));
		}
		ids = m_DM.GetCellData(i-1 + (MAX_GRID_ROW2*m_nIdx), 11, sDelayDay);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("ViewGridThread"));
		}

		if(_T("0") == sStatus || _T("대출") == sStatus)
		{
			pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("대출_바탕색"), 0, sBgColor); 
		}
		else if(_T("1") == sStatus || _T("반납") == sStatus)
		{
			pColorDM->GetCellData(_T("반납_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("반납_바탕색"), 0, sBgColor); 
		}
		else if(_T("2") == sStatus || _T("반납연기") == sStatus)
		{
			pColorDM->GetCellData(_T("반납연기_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("반납연기_바탕색"), 0, sBgColor); 
		}
		else if(_T("3") == sStatus || _T("예약") == sStatus)
		{
			pColorDM->GetCellData(_T("예약_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("예약_바탕색"), 0, sBgColor); 
		}
		else if(_T("4") == sStatus || _T("예약취소") == sStatus)
		{
			pColorDM->GetCellData(_T("예약취소_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("예약취소_바탕색"), 0, sBgColor); 
		}
		else if(_T("L") == sStatus || _T("책두레대출") == sStatus)
		{
			pColorDM->GetCellData(_T("책두레대출_글자색"), 0, sFontColor);
			pColorDM->GetCellData(_T("책두레대출_바탕색"), 0, sBgColor);

			if(_T("") == sFontColor)
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(250, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else if(_T("D") == sStatus || _T("책두레반납연기") == sStatus)
		{
			pColorDM->GetCellData(_T("책두레반납연기_글자색"), 0, sFontColor);
			pColorDM->GetCellData(_T("책두레반납연기_바탕색"), 0, sBgColor);
			if(_T("") == sFontColor) 
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(230, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else if(_T("O") == sStatus || _T("책두레예약") == sStatus)
		{
			pColorDM->GetCellData(_T("책두레예약_글자색"), 0, sFontColor);
			pColorDM->GetCellData(_T("책두레예약_바탕색"), 0, sBgColor);
			if(_T("") == sFontColor)
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(210, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else
		{
			COLORREF cr = RGB(255, 255, 255);
			sBgColor.Format(_T("%d"), cr);

			cr = RGB(0,0,0);
			sFontColor.Format(_T("%d"), cr);
		}

		if(0 < _ttoi(sDelayDay)	&& 
			(_T("0") == sStatus || _T("대출") == sStatus || _T("2") == sStatus || _T("반납연기") == sStatus
			|| _T("L") == sStatus || _T("책두레대출") == sStatus || _T("D") == sStatus || _T("책두레반납연기") == sStatus))
		{
			pColorDM->GetCellData(_T("연체_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("연체_바탕색"), 0, sBgColor);
		}
		
		crFontColor = _ttoi(sFontColor);
		crBgColor = _ttoi(sBgColor);

		INT cols;
		cols = m_pGrid->GetCols(0);

		m_pGrid->SetCol(1);
		m_pGrid->GetCellFontBold();
		if(crBgColor != m_pGrid->GetCellBackColor()) 
		{				
			for (INT j = 1 ; j < cols ; j++) 
			{
				m_pGrid->SetCol(j);
				m_pGrid->SetCellBackColor(crBgColor);
				m_pGrid->SetCellForeColor(crFontColor);
				m_pGrid->SetCellFontBold(FALSE);
				m_pGrid->SetCellFontItalic(FALSE);
			}
		}
	}
	
	m_pGrid->Refresh();
	m_pGrid->SetRedraw(TRUE);

	return 0;

EFS_END
return -1;
}
void CLoanInfoDlg::OnbtnPRINT() 
{
	// TODO: Add your control notification handler code here
	OnPrint(NULL, NULL);
}

BOOL CLoanInfoDlg::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnNcActivate(bActive);
}

LRESULT CLoanInfoDlg::OnPrint(WPARAM wParam, LPARAM lParam)
{
	if(m_bIsPrinting)
	{
		return 1020;
	}

	if(TRUE == wParam)
	{
		CWnd* pWnd = GetDlgItem(IDC_btnLOANPRINT);
		if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsWindowEnabled() && pWnd->IsWindowVisible())
		{

		}
		else
		{
			return 1000;
		}
	}

	if(m_pGrid->GetTextMatrix(1, 0).IsEmpty())
	{
		return 1010;
	}

	m_bIsPrinting = TRUE;

	CBO_LOC_RECEIPT_PRINT_DLG Dlg(-1, this, m_pManageValue);	

	CString strTitle;
	GetWindowText(strTitle);

	const INT COL_CNT = 5;
	INT aPrintColNoList[COL_CNT] = {2,3,4,5,6};
	Dlg.Print(strTitle, m_pGrid, aPrintColNoList, (0 == m_nMode) ? 4:5);

	m_bIsPrinting = FALSE;

	return 0;
}

BEGIN_EVENTSINK_MAP(CLoanInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLoanInfoDlg)
	ON_EVENT(CLoanInfoDlg, IDC_grid1, -600 /* Click */, OnClickgrid1, VTS_NONE)
	ON_EVENT(CLoanInfoDlg, IDC_grid1, -607 /* MouseUp */, OnMouseUpgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CLoanInfoDlg::OnClickgrid1() 
{
	// TODO: Add your control notification handler code here
}

void CLoanInfoDlg::OnMouseUpgrid1(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow() || 0 == m_pGrid->GetMouseCol())
	{
		return;
	}

	
	INT nRow = m_pGrid->GetMouseRow();
	INT nCol = m_pGrid->GetMouseCol();	

	if(0 < nRow && 1 == nCol)
	{
		if(1 == Button)
		{
			if(m_pGrid->GetTextMatrix(1, 0).IsEmpty())
			{
				return;
			}

			if(_T("V") == m_pGrid->GetTextMatrix(nRow, nCol))
			{
				m_pGrid->SetTextMatrix(nRow, nCol, _T(""));			
			}
			else
			{
				m_pGrid->SetTextMatrix(nRow, nCol, _T("V"));
			}
		}
		else if(2 == Button)
		{
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, 4000, _T("선정"));
			menu.AppendMenu(MF_STRING, 4001, _T("비선정"));
			menu.AppendMenu(MF_STRING, 4002, _T("전체선정"));
			menu.AppendMenu(MF_STRING, 4003, _T("전체비선정"));
			
			CRect rect;
			m_pGrid->GetWindowRect(rect);
			CPoint point(x, y);
			menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + point.x, rect.top + point.y, this);
		}
	}
	else
	{
	}
}

void CLoanInfoDlg::UserSelectProc(UINT  nIDC)
{
	
		INT i;
		
		INT top, bottom;
		top = m_pGrid->GetRow();
		bottom = m_pGrid->GetRowSel();
		
		INT tmp;
		if ( top < bottom ) {
			tmp = bottom;
			bottom = top;
			top = tmp;
		}
		
		INT max;
		max = m_pGrid->GetRows();
		
		switch( nIDC )
		{
		case 4000:
			{
				for ( i=bottom ; i<=top ; i++ )
				{
					m_pGrid->SetTextMatrix(i, 1, _T("V"));					
				}
			}
			break;
			
		case 4001:
			{
				for ( i=bottom ; i<=top ; i++ )
				{
					m_pGrid->SetTextMatrix(i, 1, _T(""));
				}
			}
			break;
			
		case 4002:
			{
				for ( i=1 ; i<max ; i++ )
				{
					m_pGrid->SetTextMatrix(i, 1, _T("V"));
				}
			}
			break;
			
		case 4003:
			{
				for ( i=1 ; i<max ; i++ )
				{
					m_pGrid->SetTextMatrix(i, 1, _T(""));
				}
			}
			break;
			
		default:
			break;
		}
		
}