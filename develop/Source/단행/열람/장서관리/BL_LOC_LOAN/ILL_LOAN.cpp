// ILL_LOAN.cpp : implementation file
//

#include "stdafx.h"
#include "ILL_LOAN.h"
#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_LOAN dialog


CILL_LOAN::CILL_LOAN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_LOAN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pManageValue = NULL;
	m_pPreRegNoMgr = NULL;
	
	BOOL m_bConstIllLoan = FALSE;
	
	m_pParent = pParent;
}
CILL_LOAN::~CILL_LOAN()
{
}

void CILL_LOAN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_LOAN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_LOAN, CDialog)
	//{{AFX_MSG_MAP(CILL_LOAN)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btn_ILL_LOAN, OnbtnILLLOAN)
	ON_EN_SETFOCUS(IDC_edt_ILL_LOAN_REG_NO, OnSetfocusRegNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_LOAN message handlers

BOOL CILL_LOAN::OnInitDialog() 
{	
	CDialog::OnInitDialog();

	if(0 > InitESL_Mgr(_T("SM_ILL_LOAN")))
 	{
 		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"SM_ILL_LOAN\")"));
 	}
	if(NULL == m_pLoan || 0 != m_pLoan->IsSearchedUser())	
	{
		CDialog::OnCancel();
		return FALSE;
	}
	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_staILL_LOAN_USER);
	if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(NULL != pDM)
		{
			int ids = 0;
			CString strName = _T("");
			ids = pDM->GetCellData(_T("대출자이름"), 0, strName);
			if(0 > ids) return FALSE;
			
			CString strID = _T("");
			ids = pDM->GetCellData(_T("대출자ID"), 0, strID);
			if(0 > ids) return FALSE;
			
			ids = pDM->GetCellData(_T("대출자KEY"), 0, m_strUserKey);
			if(0 > ids) return FALSE;
			
			pWnd->SetWindowText(strName + _T("(") + strID + _T(")"));
			pWnd->ShowWindow(SW_SHOW);


		}
	}


	m_pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_LOAN"), _T("그리드_대출"));
	m_pDM = FindDM(_T("DM_ILL_LOAN"));

	m_strLibCode = m_pInfo->LIB_CODE;

	ResizeControl();
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	Refresh();

	GetDlgItem(IDC_edt_ILL_LOAN_REG_NO)->SetFocus();
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CILL_LOAN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

int CILL_LOAN::Refresh()
{
	int ids = 0;

	RefreshMainDM(_T(""));

	if(_T("Y") == m_pManageValue->m_sUnityLoanCntYN)
	{
		const int ROW_COUNT = m_pDM->GetRowCount();
		for(int nRow = 0; nRow < ROW_COUNT; nRow++)
		{
			CString strLibTotalLoanCanCnt;
			ids = m_pDM->GetCellData(_T("도서관_최대대출가능책수"), nRow, strLibTotalLoanCanCnt);
			
			CESL_DataMgr* pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

			CString strLibManageCode;
			ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nRow, strLibManageCode);


			CESL_DataMgr TmpDM;
			TmpDM.SetCONNECTION_INFO(_T(""));
			
			CString strQuery;
			CString strIsAppendixCnt;

			strQuery.Format(
				_T("SELECT VALUE_1 ")
				_T("FROM ESL_MANAGE_TBL ")
				_T("WHERE CLASS_ALIAS = '부록관리' AND VALUE_NAME = '부록대출권수포함여부' ")
				_T("AND MANAGE_CODE = '%s'"),
				strLibManageCode);
			TmpDM.GetOneValueQuery(strQuery, strIsAppendixCnt);
			

			int nLibLoanCnt = 0; 
			
			const int LOAN_DM_ROW_COUNT = pLoanDM->GetRowCount();
			for(int nLoanRow = 0; nLoanRow < LOAN_DM_ROW_COUNT; nLoanRow++)
			{
				CString strBookManageCode;
				pLoanDM->GetCellData(_T("관리구분"), nLoanRow, strBookManageCode);
				
				CString strStatus;
				pLoanDM->GetCellData(_T("대출상태"), nLoanRow, strStatus);

				CString strType;
				pLoanDM->GetCellData(_T("부록여부"), nLoanRow, strType);

				CString strLDevice;
				pLoanDM->GetCellData(_T("L_DEVICE"), nLoanRow, strLDevice);

				if(strLibManageCode == strBookManageCode 
					&& 
					(strStatus == _T("0")
					|| strStatus == _T("2")
					|| strStatus == _T("L")
					|| strStatus == _T("D"))
					&&
					_T("KOLASIII-이동도서관") != strLDevice
					)
				{
					if( strType.Compare( _T("A") ) == 0 )
					{
						if( strIsAppendixCnt == _T("Y"))
						{
							nLibLoanCnt++;
						}
					}
					else
					{
						nLibLoanCnt++;
					}
				}
			}

			CString strLibLoanCnt;
			strLibLoanCnt.Format(_T("%d"), nLibLoanCnt);
			ids = m_pDM->SetCellData(_T("도서관_대출책수"), strLibLoanCnt, nRow);

			CString strLibLoanCanCnt;
			strLibLoanCanCnt.Format(
				_T("%d"), _ttoi(strLibTotalLoanCanCnt) - _ttoi(strLibLoanCnt));
			ids = m_pDM->SetCellData(_T("도서관_대출가능책수"), strLibLoanCanCnt, nRow);

			CString strLibLoanState;
			strLibLoanState.Format(
				_T("%d/%d"), _ttoi(strLibLoanCnt), _ttoi(strLibTotalLoanCanCnt));
			ids = m_pDM->SetCellData(_T("도서관_대출책수상태"), strLibLoanState, nRow);
		}
	}
	else m_pGrid->SetColWidth(8, 0, 0);
	
	DisplayGrid();

	return 0;
}

int CILL_LOAN::RefreshMainDM(CString strOption)
{	
	if(NULL == m_pDM) return -1;

	CString strQuery[4];
	Get_SQL(strQuery[1], _T("MO"));
	Get_SQL(strQuery[2], _T("SE"));
	Get_SQL(strQuery[3], _T("NB"));

	CString strWhere = strOption;
	if(FALSE == strWhere.IsEmpty())
	{
		strWhere.TrimLeft();
		strWhere = _T("WHERE ") + strWhere;
	}

	strQuery[0].Format(_T("SELECT * FROM (( %s ) UNION ALL ( %s ) UNION ALL ( %s )) T %s %s"), strQuery[1], strQuery[2], strQuery[3], strWhere, _T("ORDER BY LENGTH(TRANS_NO) DESC, TRANS_NO DESC"));
	m_pDM->RestructDataManager(strQuery[0]);

	return 0;
}

int CILL_LOAN::DisplayGrid()
{
	if(NULL == m_pGrid) return -1;
	m_pGrid->Display();

	return 0;
}

void CILL_LOAN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

int CILL_LOAN::ResizeControl()
{
	CRect rDlg;
	GetClientRect(rDlg);

	// 그리드 이동
	{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid_ILL_LOAN);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -1;

 	CRect rGrid;
 	pWnd->GetWindowRect(rGrid);
 	ScreenToClient(rGrid);

	rGrid = CRect(5, rGrid.top, rDlg.right-5, rDlg.bottom-5);
	pWnd->MoveWindow(rGrid);
	}
	// 
	{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDOK);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -2;

	CRect rBtn;
 	pWnd->GetWindowRect(rBtn);
	ScreenToClient(rBtn);

	rBtn.OffsetRect(-rBtn.left, 0);
	rBtn.OffsetRect(rDlg.right - rBtn.Width() -5, 0);

	pWnd->MoveWindow(rBtn);
	}
	return 0;
}

int CILL_LOAN::Loan()
{
	// 대출자 검색여부 판단
	int ids = 0;
	if(NULL == m_pLoan) return FALSE;
	ids = m_pLoan->IsSearchedUser();
	if(0 > ids) return ids;

	CString strMANAGE_CODE = _T("");

	CString sLogMsg = _T("");
	CString strInBarCodeLibCode = _T("");
	CString strInBarCodeLibMngCode = _T("");

	// 등록번호 체크
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edt_ILL_LOAN_REG_NO);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -1;

	CString strRegNo = _T("");
	pWnd->GetWindowText(strRegNo);
	strRegNo.MakeUpper();

	SetDlgItemText(IDC_edt_ILL_LOAN_REG_NO, _T(""));
	if(_T("") != strRegNo)
	{
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		CString strQuery;
		CString strMaxLoanCnt;
		CString strLoanCnt;

		sLogMsg.Format(_T("■책두레대출 - 등록번호 입력 : %s■"), strRegNo);
		TmpDM.QueryLog(sLogMsg.GetBuffer(0));
		strQuery.Format(
			_T("SELECT MAX(VALUE_1) FROM ")
			_T("ILL_MANAGE_TBL WHERE VALUE_NAME = '책두레최대대출가능책수'"));	
		ids = TmpDM.GetOneValueQuery(strQuery, strMaxLoanCnt);
		if(0 > ids)
		{
			m_bConstIllLoan = FALSE;
		}
		else
		{
			m_bConstIllLoan = TRUE;
			strQuery.Format(
				_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE ")
 				_T("USER_NO = (SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s) ")			
				_T("AND END_DIV IS NULL AND RETURN_LIBCODE IS NULL AND LOAN_KEY IS NOT NULL")
				_T(" AND BOOK_APPENDIX_FLAG != 'A'")
 				, m_strUserKey);
			ids = TmpDM.GetOneValueQuery(strQuery, strLoanCnt);
			if(0 > ids)
			{
				return ids;
			}
			INT nMaxCnt;
			INT nLoanCnt;
			nMaxCnt = _ttoi(strMaxLoanCnt);
			nLoanCnt = _ttoi(strLoanCnt);

			if(0 >= (nMaxCnt-nLoanCnt))
			{
 				BOOL bIsOnlyAppendix = FALSE;
				if(1 == m_pGrid->SelectGetCount())
				{
					CString strAppendixFlag;
					m_pDM->GetCellData(_T("부록여부"), m_pGrid->SelectGetIdx(), strAppendixFlag);
					if(_T("") != strAppendixFlag || _T("N") != strAppendixFlag)
					{
						bIsOnlyAppendix = TRUE;
					}
				}				

				if(bIsOnlyAppendix == 0)
				{
					AfxMessageBox(_T("이용자의 책두레 대출한도가 넘어 대출할 수 없습니다."));
					return 0;
				}
			}
		}

		if ( strRegNo.GetLength() == 19 ) {
			CString strTmp = _T("");
			strTmp = strRegNo.Mid(7);			
			strInBarCodeLibCode = strRegNo.Mid(1, 6);
			strRegNo = strTmp;

			strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE=%s"), strInBarCodeLibCode);
			ids = TmpDM.GetOneValueQuery(strQuery, strInBarCodeLibMngCode);
		}
		int ids = 0;
		CString strPreRegNo;

		BOOL bFind = FALSE;
		INT nFindCnt = 0, nFindRow = 0;
		CString strGet = _T("");
		int ROW = m_pDM->GetRowCount();

		for(INT nGetRow = 0; nGetRow < ROW; nGetRow++)
		{
			strPreRegNo = strRegNo;			
			ids = m_pDM->GetCellData(_T("등록번호"), nGetRow, strGet);
			if(0 > ids) return ids;

			if ( strInBarCodeLibMngCode.GetLength() == 0 ) {
				ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nGetRow, strMANAGE_CODE);
				ids = CLocCommonAPI::MakeRegNoSpaceToZero(this, strPreRegNo, strMANAGE_CODE, m_pPreRegNoMgr);
				if(0 > ids) return ids;

				if(strGet == strPreRegNo) {				
					bFind = TRUE;
					nFindRow = nGetRow;
					nFindCnt++;
				}
			}
			else {
				ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nGetRow, strMANAGE_CODE);
				if(strGet == strPreRegNo && strMANAGE_CODE == strInBarCodeLibMngCode) {				
					bFind = TRUE;
					nFindRow = nGetRow;
					nFindCnt++;
				}
			}			
		}
		if(nFindCnt > 1) {
			AfxMessageBox( _T("같은 등록번호를 가진 자료가 여러개 존재합니다.\r\n")
						   _T("그리드에서 대출할 자료를 직접 선택한 후 대출버튼을 통해 대출해 주십시요."));
			return -2;
		}
	

		if(TRUE == bFind)
		{
			CString strTransNo;
			ids = m_pDM->GetCellData(_T("트랜잭션번호"), nFindRow, strTransNo);
			if(0 > ids) return ids;
			ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nFindRow, strMANAGE_CODE);
			if(0 > ids) return ids;

			if(_T("") != strTransNo)
			{
				if(_T("Y") == m_pManageValue->m_sUnityLoanCntYN)
				{
					CString sLoanType;
					GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("대출형태"), sLoanType);
					if(_T("1") != sLoanType)
					{
						ids = m_pDM->GetCellData(_T("도서관_대출가능책수"), nFindRow, strGet);
						if(0 == _ttoi(strGet))
						{
							AfxMessageBox(_T("도서관 대출가능권수 한도를 초과하여 대출할 수 없습니다."));
							return 0;
						}
					}
				}
				INT nIsAppendixLoan = IDNO;
				CString strSpeciesKey;
				CString strAppendixYN;
				CString strGetData;
				if(_T("Y") == m_pManageValue->m_sIsLoanAppendix)
				{
					ids = m_pDM->GetCellData(_T("종KEY"), nFindRow, strSpeciesKey);
					if(0 > ids) return ids;
					ids = m_pDM->GetCellData(_T("부록여부"), nFindRow, strAppendixYN);
					if(0 > ids) return ids;
					if(_T("Y") != strAppendixYN)
					{
						for(INT i = 0; i < m_pDM->GetRowCount(); i++)
						{
							if(i == nFindRow) continue;
							ids = m_pDM->GetCellData(_T("종KEY"), i, strGetData);
							if(0 > ids) return ids;
							if(strSpeciesKey == strGetData)
							{
								ids = m_pDM->GetCellData(_T("부록여부"), i, strGetData);
								if(0 > ids) return ids;
								if(_T("Y") == strGetData)
								{
									nIsAppendixLoan = MessageBox(_T("대출하려는 책과 같이 입수된 부록자료가 존재합니다.\r\n같이 대출을 하시겠습니까?"), _T("부록자료대출"), MB_ICONQUESTION | MB_YESNO);
									break;
								}
							}
						}
					}
				}
				CESL_DataMgr* pILLDM = FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
				if(IDNO == nIsAppendixLoan)
				{
					pILLDM->FreeData();
					pILLDM->InsertRow(-1);
					pILLDM->SetCellData(_T("트랜잭션번호"), strTransNo, 0);
					pILLDM->SetCellData(_T("관리구분_책"), strMANAGE_CODE, 0);
					pILLDM->SetCellData(_T("등록번호"), strPreRegNo, 0);

					sLogMsg.Format(_T("■ 책두레 대출 - 등록번호 : %s / 관리구분 : %s / 트랜잭션 : %s [F1]■"), strPreRegNo, strMANAGE_CODE, strTransNo);
					TmpDM.QueryLog(sLogMsg.GetBuffer(0));
					
					SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strPreRegNo);
					ids = m_pLoan->SPFExcutePath(_T("책두레대출"));
					if(0 > ids) return ids;
					Refresh();
				}
				else
				{
					ids = m_pDM->GetCellData(_T("종KEY"), nFindRow, strSpeciesKey);
					if(0 > ids) return ids;
					for(INT i = 0; i < m_pDM->GetRowCount(); i++)
					{
						ids = m_pDM->GetCellData(_T("종KEY"), i, strGetData);
						if(0 > ids) return ids;
						if(strSpeciesKey == strGetData)
						{
							ids = m_pDM->GetCellData(_T("트랜잭션번호"), i, strTransNo);
							if(0 > ids) return ids;
							ids = m_pDM->GetCellData(_T("등록번호"), i, strPreRegNo);
							if(0 > ids) return ids;
							ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), i, strMANAGE_CODE);
							ids = CLocCommonAPI::MakeRegNoSpaceToZero(this, strPreRegNo, strMANAGE_CODE, m_pPreRegNoMgr);
							if(0 > ids) return ids;
							pILLDM->FreeData();
							pILLDM->InsertRow(-1);
							pILLDM->SetCellData(_T("트랜잭션번호"), strTransNo, 0);
							pILLDM->SetCellData(_T("관리구분_책"), strMANAGE_CODE, 0);
							pILLDM->SetCellData(_T("등록번호"), strPreRegNo, 0);

							sLogMsg.Format(_T("■ 책두레 대출 - 등록번호 : %s / 관리구분 : %s / 트랜잭션 : %s [F2]■"), strPreRegNo, strMANAGE_CODE, strTransNo);
							TmpDM.QueryLog(sLogMsg.GetBuffer(0));

							SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strPreRegNo);
							ids = m_pLoan->SPFExcutePath(_T("책두레대출"));
							if(0 > ids) return ids;	
						}
					}
					Refresh();
				}

				return ids;
			}
		}

		CString strMsg;
		strMsg.Format(
			_T("입력된 등록번호(%s)를 가진 자료를 찾을 수 없습니다.\r\n")
			_T("아래사항을 확인해보시기 바랍니다.\r\n\r\n")
			_T("1. 해당 등록번호의 도서가 책두레대상자료가 아닙니다.\r\n")
			_T("2. 해당 등록번호의 도서가 입수(대출대기중)상태의 자료가 아닙니다.\r\n")
			_T("3. 해당 등록번호의 도서를 신청한 이용자와 현재 검색된 이용자가 다릅니다.\r\n")
			_T("4. 입력된 등록번호가 올바르지 않습니다.\r\n"),
			strRegNo);

		AfxMessageBox(strMsg);
		return -1;
	}
	else
	{
		BOOL bFind = FALSE;
		CString strTRANS_NO = _T("");
		CString strREG_NO = _T("");

		ids = m_pGrid->SelectMakeList();
		if(0 > ids) return ids;

		int CNT = m_pGrid->SelectGetCount();
		if(0 >= CNT) 
		{
			AfxMessageBox(_T("먼저 대출할 자료를 선정하여 주십시요."));
		}

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		CString strQuery;
		CString strMaxLoanCnt;
		CString strLoanCnt;
		strQuery.Format(

			_T("SELECT MAX(VALUE_1) FROM ")
			_T("ILL_MANAGE_TBL WHERE VALUE_NAME = '책두레최대대출가능책수'"));	
		ids = TmpDM.GetOneValueQuery(strQuery, strMaxLoanCnt);
		if(0 > ids)
		{
			m_bConstIllLoan = FALSE;
		}
		else
		{
			m_bConstIllLoan = TRUE;
			strQuery.Format(
				_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE ")
 				_T("USER_NO = (SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s) ")			
				_T("AND END_DIV IS NULL AND RETURN_LIBCODE IS NULL AND LOAN_KEY IS NOT NULL")
				_T(" AND BOOK_APPENDIX_FLAG != 'A'")
 				, m_strUserKey);
			ids = TmpDM.GetOneValueQuery(strQuery, strLoanCnt);
			if(0 > ids)
			{
				return ids;
			}
			INT nMaxCnt;
			INT nLoanCnt;
			nMaxCnt = _ttoi(strMaxLoanCnt);
			nLoanCnt = _ttoi(strLoanCnt);

			CString strTemp;
			BOOL bIsOnlyAppendix = FALSE;
			INT nIdx  = m_pGrid->SelectGetHeadPosition();
			do
			{
				ids = m_pDM->GetCellData(_T("부록여부"), nIdx, strTemp);	
				if(_T("Y") == strTemp)
				{
					bIsOnlyAppendix = TRUE;
				}
				else
					nLoanCnt++;

				nIdx = m_pGrid->SelectGetNext();
			}
			while(0 <= nIdx);

			if(0 > (nMaxCnt-nLoanCnt))
			{
				if(bIsOnlyAppendix == 0)
				{
					AfxMessageBox(_T("이용자의 책두레 대출한도가 넘어 대출할 수 없습니다."));
					return 0;
				}

			}
		}

		if(_T("Y") == m_pManageValue->m_sIsLoanAppendix)
		{
			CString strGetData;
			CString strSpeciesKey;
			CString strAppendixYN;
			INT nIdx = m_pGrid->SelectGetHeadPosition();

			do
			{
				ids = m_pDM->GetCellData(_T("종KEY"), nIdx, strSpeciesKey);
				if(0 > ids) return ids;
				ids = m_pDM->GetCellData(_T("부록여부"), nIdx, strAppendixYN);
				if(0 > ids) return ids;
				if(_T("Y") != strAppendixYN)
				{
					INT nIsAppendixLoan = IDNO;
					BOOL bShowMessageBox = FALSE;
					for(INT i = 0; i < m_pGrid->GetCount(); i++)
					{
						if(i == nIdx) continue;
						m_pGrid->GetAt(i, m_pGrid->m_nSelectIdx, strGetData);
						if(m_pGrid->m_sSelectString == strGetData) continue;
						ids = m_pDM->GetCellData(_T("종KEY"), i, strGetData);
						if(0 > ids) return ids;
						if(strSpeciesKey == strGetData)
						{
							ids = m_pDM->GetCellData(_T("부록여부"), i, strGetData);
							if(0 > ids) return ids;
							if(_T("Y") == strGetData)
							{
								if(FALSE == bShowMessageBox)
								{
									nIsAppendixLoan = MessageBox(_T("선정한 책과 같이 입수된 부록자료가 존재합니다.\r\n같이 대출을 하시겠습니까?"), _T("부록자료대출"), MB_ICONQUESTION | MB_YESNO);
									bShowMessageBox = TRUE;
									if(IDNO == nIsAppendixLoan) break;
								}
								m_pGrid->SetAt(i, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
							}
						}
					}
					if(IDNO == nIsAppendixLoan) break;
				}
				nIdx = m_pGrid->SelectGetNext();
			}while(0 <= nIdx);
			ids = m_pGrid->SelectMakeList();
			if(0 > ids) return ids;
		}

		if(_T("Y") == m_pManageValue->m_sUnityLoanCntYN)
		{
			CString sLoanType;
			GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("대출형태"), sLoanType);
			if(_T("1") != sLoanType)
			{
				if(FALSE == Is_UnityLoan())
				{
					AfxMessageBox(_T("도서관 대출가능권수 한도를 초과하여 대출할 수 없습니다."));
					return 0;
				}
			}
		}

		int nIdx = m_pGrid->SelectGetHeadPosition();

		do
		{
			ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTRANS_NO);
			if(0 > ids) return ids;
			ids = m_pDM->GetCellData(_T("등록번호"), nIdx, strREG_NO);
			if(0 > ids) return ids;
			ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nIdx, strMANAGE_CODE);
			if(0 > ids) return ids;
			if(_T("") != strTRANS_NO && _T("") != strREG_NO)
			{
				CESL_DataMgr* pILLDM = FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
				pILLDM->FreeData();
				pILLDM->InsertRow(-1);
				const int nRow = pILLDM->GetRowCount() -1;
				pILLDM->SetCellData(_T("트랜잭션번호"), strTRANS_NO, nRow);
				pILLDM->SetCellData(_T("관리구분_책"), strMANAGE_CODE, nRow);
				pILLDM->SetCellData(_T("등록번호"), strREG_NO, nRow);

				sLogMsg.Format(_T("■책두레 대출 - 등록번호 : %s / 관리구분 : %s / 트랜잭션 : %s [F3]■"), strREG_NO, strMANAGE_CODE, strTRANS_NO);
				TmpDM.QueryLog(sLogMsg.GetBuffer(0));

				ids = SetControlData( _T("CM_BO_LOC_3100_SEARCH_INFO") , _T("대출자번호"), strREG_NO );
				ids = m_pLoan->SPFExcutePath(_T("책두레대출"));
			}
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);		
	}
	Refresh();

	return 0;
}

void CILL_LOAN::OnbtnILLLOAN() 
{
	// TODO: Add your control notification handler code here
	Loan();
}

void CILL_LOAN::OnSetfocusRegNo() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edt_ILL_LOAN_REG_NO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edt_ILL_LOAN_REG_NO)->m_hWnd,hIME); 
    }
}

BOOL CILL_LOAN::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnbtnILLLOAN();

			return TRUE;
		}
	}	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

BOOL CILL_LOAN::Is_UnityLoan()
{	
	CStringArray aManageCode;
	CStringArray aManageCodeCnt;
	CStringArray aLoanCanCnt;

	m_pGrid->SelectMakeList();
	if(0 < m_pGrid->SelectGetCount())
	{
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			CString strLibManageCode;
			m_pDM->GetCellData(_T("제공도서관_관리구분"), nIdx, strLibManageCode);

			CString strApeendixYN;
			m_pDM->GetCellData(_T("부록여부"), nIdx, strApeendixYN);
			if(_T("Y") == strApeendixYN)
			{
				nIdx = m_pGrid->SelectGetNext();
				continue;
			}

			BOOL bIsFind = FALSE;
			const int MANAGE_CODE_CNT = aManageCode.GetSize();
			for(int nCnt = 0; nCnt < MANAGE_CODE_CNT; nCnt++)
			{
				if(strLibManageCode == aManageCode.GetAt(nCnt))
				{
					CString strManageCodeCnt;
					int nManageCodeCnt = _ttoi(aManageCodeCnt.GetAt(nCnt)) + 1;
					strManageCodeCnt.Format(_T("%d"), nManageCodeCnt);
					aManageCodeCnt.SetAt(nCnt, strManageCodeCnt);

					bIsFind = TRUE;
				}
			}
			if(FALSE == bIsFind)
			{
				aManageCode.Add(strLibManageCode);
				aManageCodeCnt.Add(_T("1"));

				CString strLoanCanCnt;
				m_pDM->GetCellData(_T("도서관_대출가능책수"), nIdx, strLoanCanCnt);
				if(0 == _ttoi(strLoanCanCnt)) return FALSE;
				aLoanCanCnt.Add(strLoanCanCnt);
			}

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);
	}
	else 
	{
		AfxMessageBox(_T("먼저 선정하여 주십시요"));
		return FALSE;
	};

	const int MANAGE_CODE_CNT = aManageCode.GetSize();
	for(int nCnt = 0; nCnt < MANAGE_CODE_CNT; nCnt++)
	{
		int nManageCodeCnt = _ttoi(aManageCodeCnt.GetAt(nCnt));
		int nLoanCanCnt = _ttoi(aLoanCanCnt.GetAt(nCnt));

		if(nLoanCanCnt < nManageCodeCnt) return FALSE;
	}
	return TRUE;
}



INT CILL_LOAN::Get_SQL(CString &strGetQuery, CString strMode)
{
	INT aColIndex[256];	
	CString strTblName, strWhere, strBakTblName, strBakCondition, strBakCondition2;
	Get_TblName(strTblName, strMode);
 	Get_DefaultWhere(strWhere, strMode);
	strWhere = strWhere + _T(" AND A.APL_LIBCODE = '") + m_strLibCode + _T("' AND A.USER_KEY = ") + m_strUserKey;

	strBakTblName = m_pDM->TBL_NAME;
	strBakCondition = m_pDM->CONDITION;
	strBakCondition2 = m_pDM->EXTRA_CONDITION; 

	m_pDM->TBL_NAME = strTblName;
	m_pDM->CONDITION = _T("");
	m_pDM->EXTRA_CONDITION = _T("");

	strGetQuery = m_pDM->MakeSelectQuery(strWhere, aColIndex);

	m_pDM->TBL_NAME = strBakTblName;
	m_pDM->CONDITION = strBakCondition;
	m_pDM->EXTRA_CONDITION = strBakCondition2; 

	if(_T("MO") == strMode)
	{
		strGetQuery.Replace(_T("<$청구기호$>"), _T("ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE)"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("BB.EA_ISBN"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
		strGetQuery.Replace(_T("<$부록여부$>"), _T("DECODE(BB.BOOK_APPENDIX_FLAG,'A','Y','N')"));
		strGetQuery.Replace(_T("<$종KEY$>"), _T("BB.SPECIES_KEY"));
	}
	else if(_T("SE") == strMode)
	{
		strGetQuery.Replace(_T("<$청구기호$>"), _T("ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE)"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("IB.ST_ISSN"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
		strGetQuery.Replace(_T("<$부록여부$>"), _T("DECODE(BB.BOOK_TYPE,'A','Y','N')"));
		strGetQuery.Replace(_T("<$종KEY$>"), _T("BB.SPECIES_KEY"));
	}
	else if(_T("NB") == strMode)
	{
		strGetQuery.Replace(_T("<$청구기호$>"), _T("BB.CALL_NO"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
		strGetQuery.Replace(_T("<$부록여부$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$종KEY$>"), _T("NULL"));
		strGetQuery.Replace(_T("IB.TITLE_INFO"), _T("BB.TITLE"));
	}

	return 0;
}

int CILL_LOAN::Get_TblName(CString &strTblName, CString strMode)
{
	strTblName = 
		_T("(SELECT ITL.*, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
		_T("FROM ILL_TRANS_TBL  ITL ) A");
	if(_T("MO") == strMode)
	{
		strTblName += _T(", BO_BOOK_TBL BB, IDX_BO_TBL IB, CO_LOAN_USER_TBL CLU");
	}
	else if(_T("SE") == strMode)
	{
		strTblName += _T(", SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU");
	}
	else if(_T("NB") == strMode)
	{
		strTblName += _T(", CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU");
	}

	return 0;
}


INT CILL_LOAN::Get_DefaultWhere(CString &strWhere, CString strMode)
{
	if(_T("MO") == strMode)
	{
		strWhere = _T("A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY = IB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.STATE = '0014' AND A.LOAN_KEY IS NULL");
	}
	else if(_T("SE") == strMode)
	{
		strWhere = _T("A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.VOL_KEY=V.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.STATE = '0014' AND A.LOAN_KEY IS NULL AND A.PUBLISH_FORM_CODE = 'SE'");
	}
	else if(_T("NB") == strMode)
	{
		strWhere = _T("A.BOOK_KEY = BB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.STATE = '0014' AND A.LOAN_KEY IS NULL AND A.PUBLISH_FORM_CODE = 'NB'");
	}
	return 0;
}
