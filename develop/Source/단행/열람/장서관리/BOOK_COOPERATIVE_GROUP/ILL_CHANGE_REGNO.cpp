#include "stdafx.h"
#include "resource.h"
#include "ILL_CHANGE_REGNO.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#include "ILL_SEARCH_BOOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CILL_CHANGE_REGNO::CILL_CHANGE_REGNO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	mCtrl_pOldBookInfoGrid = NULL;
	mCtrl_pNewBookInfoGrid = NULL;
	mCtrl_pParentBookInfoGrid = NULL;
	m_pLoanShareManager = NULL;
	m_strState = _T("");	
	m_strNewSpeciesKey = _T("");
	m_strNewBookKey = _T("");
}


void CILL_CHANGE_REGNO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_CHANGE_REGNO)
	DDX_Control(pDX, IDC_edtILL_CHANGE_REGNO_OLD_REGNO, mCtrl_edtOldRegNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_CHANGE_REGNO, CDialog)
	//{{AFX_MSG_MAP(CILL_CHANGE_REGNO)
	ON_BN_CLICKED(IDC_btnILL_CHANGE_REGNO_INPUT_REGNO, OnbtnILLCHANGEREGNOINPUTREGNO)
	ON_BN_CLICKED(IDC_btnILL_SEARCH_BOOK, OnbtnILLSEARCHBOOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CILL_CHANGE_REGNO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	mCtrl_pOldBookInfoGrid = (CMSHFlexGrid*)GetDlgItem(IDC_grid_ILL_CHANGE_REGNO_OLDBOOKINFO);
	mCtrl_pNewBookInfoGrid = (CMSHFlexGrid*)GetDlgItem(IDC_grid_ILL_CHANGE_REGNO_NEWBOOKINFO);

	Init_Grid();

	if(NULL == mCtrl_pParentBookInfoGrid)
	{
		CDialog::OnCancel();
	}
	
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		mCtrl_pOldBookInfoGrid->SetTextMatrix(
			nRow,
			1,
			mCtrl_pParentBookInfoGrid->GetTextMatrix(nRow, 1));
	}
	mCtrl_edtOldRegNo.SetWindowText(mCtrl_pOldBookInfoGrid->GetTextMatrix(1, 1));
		
	
 	if(_T("제공") == m_strState)
	{	
		GetDlgItem(IDC_btnILL_SEARCH_BOOK)->ShowWindow(SW_SHOW);
	}


	return TRUE;  
	              
}
int CILL_CHANGE_REGNO::Init_Grid()
{
	
	if(0 >= ROW_COUNT) return -1;

	const int GRID_COUNT = 2;
	CMSHFlexGrid* pGrid[GRID_COUNT] = 
	{
		mCtrl_pOldBookInfoGrid,
		mCtrl_pNewBookInfoGrid
	};

	for(int nCnt = 0; nCnt < GRID_COUNT; nCnt++)
	{
		pGrid[nCnt]->SetFixedRows(0);
		pGrid[nCnt]->SetFixedCols(0);
		pGrid[nCnt]->SetRows(ROW_COUNT);
		pGrid[nCnt]->SetCols(0, 2);
 		pGrid[nCnt]->SetColWidth(0, 0, HEADER_WIDTH);
		pGrid[nCnt]->SetColWidth(1, 0, DATA_WIDTH);	
		pGrid[nCnt]->SetRowHeight(-1, HEIGHT);
		pGrid[nCnt]->SetRowHeight(0, HEIGHT);		
		
		COLORREF cr_Tag_Bk[2] = { RGB(255, 162, 99), RGB(255, 223, 206) };
		COLORREF cr_Tag_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };
		COLORREF cr_Data_Bk[2] = { RGB(189, 227, 231), RGB(239, 247, 247) };
		COLORREF cr_Data_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };

		CString Alias[ROW_COUNT] = 
		{
			_T("제공도서관"), _T("등록번호"), _T("서명"), _T("저자"), _T("출판사"), _T("출판년도"), _T("권정보"), _T("청구기호"), _T("발행형태구분")
		};
 		for(int nRow = 0; nRow < ROW_COUNT; nRow++)
 		{
 			int nIdx = nRow % 2;
  			SetGridCellColor(pGrid[nCnt], nRow, 0, cr_Tag_Bk[nIdx], cr_Tag_Fk[nIdx]);
  			SetGridCellColor(pGrid[nCnt], nRow, 1, cr_Data_Bk[nIdx], cr_Data_Fk[nIdx]);
			pGrid[nCnt]->SetTextMatrix(nRow, 0, Alias[nRow]);
 		}
	}

	return 0;
}
void CILL_CHANGE_REGNO::SetGridCellColor(CMSHFlexGrid *pGrid, INT nRow, INT nCol, COLORREF bk, COLORREF fk)
{
	if ( pGrid )
	{
		pGrid->SetRow(nRow);
		pGrid->SetCol(nCol);
		pGrid->SetCellBackColor(bk);
		pGrid->SetCellForeColor(fk);
		if(0 == nCol) pGrid->SetCellAlignment(4);
		else pGrid->SetCellAlignment(1);	
	}

	return;
}
void CILL_CHANGE_REGNO::OnbtnILLCHANGEREGNOINPUTREGNO() 
{
	
	CWnd* pWnd = GetDlgItem(IDC_edtILL_CHANGE_REGNO_NEW_REGNO);
	if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		CString strRegNo;
		pWnd->GetWindowText(strRegNo);
		strRegNo.MakeUpper();

		CLocCommonAPI::MakeRegNoSpaceToZero(this, strRegNo, NULL);
		
		
		
		CString strOption;
		
		if(_T("요청") == m_strState)
		{	
			strOption.Format(
				_T("BB.REG_NO = '%s' AND BB.WORKING_STATUS IN ('BOL112N', 'SEL112N','SEL212N') AND ROWNUM = 1"),
				strRegNo,
				m_pInfo->MANAGE_CODE
 			);
		}
		else
		{
			strOption.Format(				
				_T("BB.REG_NO = '%s' AND BB.WORKING_STATUS IN ('BOL112N', 'SEL112N','SEL212N') AND ROWNUM = 1"),
				strRegNo,
				m_pInfo->MANAGE_CODE
 			);
		}

		CESL_DataMgr TmpDM;
		TmpDM.InitDMPKFromFile(_T("DM_ILL_SEARCH_BOOK_LIST_BO"));
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		TmpDM.RefreshDataManager(strOption);		
		
		if(0 >= TmpDM.GetRowCount())
		{

			TmpDM.InitDMPKFromFile(_T("DM_ILL_SEARCH_BOOK_LIST_SE"));
			TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			TmpDM.RefreshDataManager(strOption);
		}

		if(0 >= TmpDM.GetRowCount())
		{
			AfxMessageBox(_T("비치중인 책중에 검색한 등록번호에 해당하는 책이 없습니다."));
		}
		else
		{			
			CString strGet;
			TmpDM.GetCellData(_T("도서관_이름"), 0, strGet);
			mCtrl_pNewBookInfoGrid->SetTextMatrix(0, 1, strGet);
			
			TmpDM.GetCellData(_T("등록번호"), 0, strGet);			
			mCtrl_pNewBookInfoGrid->SetTextMatrix(1, 1, strGet);
			
			TmpDM.GetCellData(_T("서명"), 0, strGet);
			mCtrl_pNewBookInfoGrid->SetTextMatrix(2, 1, strGet);
			
			TmpDM.GetCellData(_T("저작자"), 0, strGet);					
			mCtrl_pNewBookInfoGrid->SetTextMatrix(3, 1, strGet);
			
			TmpDM.GetCellData(_T("발행자"), 0, strGet);			
			mCtrl_pNewBookInfoGrid->SetTextMatrix(4, 1, strGet);
			
			TmpDM.GetCellData(_T("발행년도"), 0, strGet);			
			mCtrl_pNewBookInfoGrid->SetTextMatrix(5, 1, strGet);
			
			TmpDM.GetCellData(_T("편권차"), 0, strGet);
			mCtrl_pNewBookInfoGrid->SetTextMatrix(6, 1, strGet);
			
			TmpDM.GetCellData(_T("청구기호"), 0, strGet);
			mCtrl_pNewBookInfoGrid->SetTextMatrix(7, 1, strGet);
			
			TmpDM.GetCellData(_T("단행/연속구분"), 0, strGet);
			if(_T("MO") == strGet)
			{
				strGet = _T("단행");
			}
			else if(_T("SE") == strGet)
			{
				strGet = _T("연속");
			}
			else if(_T("NB") == strGet)
			{
				strGet = _T("긴급대출");
			}
			mCtrl_pNewBookInfoGrid->SetTextMatrix(8, 1, strGet);

			TmpDM.GetCellData(_T("책KEY"), 0, m_strNewBookKey);			

			
			TmpDM.GetCellData(_T("종KEY"), 0, m_strNewSpeciesKey);
		}
		
	}
	else AfxMessageBox(_T("새 서지정보의 등록번호 에디트박스를 찾을 수 없습니다."));
}

BOOL CILL_CHANGE_REGNO::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
		else if(VK_RETURN == pMsg->wParam)
		{
			OnbtnILLCHANGEREGNOINPUTREGNO();
			return TRUE;
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CILL_CHANGE_REGNO::OnOK() 
{
	
	CString strRegNo = mCtrl_pNewBookInfoGrid->GetTextMatrix(1, 1);
	if(_T("") == strRegNo)
	{
		AfxMessageBox(_T("먼저 등록번호를 입력하여 주십시요."));
		return;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.StartFrame();

	int ids = 0;	
	CString strQuery;
	ids = Get_Sql_TransUpdate(strQuery);
	if(0 > ids) { AfxMessageBox(_T("트랜잭션업데이트를 위한 정보구성에 실패하였습니다.")); return; }
	TmpDM.AddFrame(strQuery);

	ids = Get_Sql_OldBookUpdate(strQuery);
	if(0 > ids) { AfxMessageBox(_T("이전 책정보 업데이트를 위한 정보구성에 실패하였습니다.")); return; }
	TmpDM.AddFrame(strQuery);

	ids = Get_Sql_NewBookUpdate(strQuery);
	if(0 > ids) { AfxMessageBox(_T("새 책정보 업데이트를 위한 정보구성에 실패하였습니다.")); return; }
	TmpDM.AddFrame(strQuery);

	ids = TmpDM.SendFrame();
	TmpDM.EndFrame();

	if(0 > ids) { AfxMessageBox(_T("등록번호 변경에 실패하였습니다.")); return; }
	
 	if(_T("D") == m_sBookCooperativeMode && ids >= 0)
 	{
 		UPLOAD_SHARE_INFO UploadShareInfo;	
 		UploadShareInfo.nType = 0;
 
 		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("71");
 		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");	
 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("ILL_TRANS_TBL");
 		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;
 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = m_strTransNo;
 		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("BOOK_KEY,REG_NO,TITLE_INFO,AUTHOR_INFO,PUBLISHER_INFO,PUB_YEAR_INFO,VOL_INFO,CALL_NO,PUBLISH_FORM_CODE");
 		m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
 	}

	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		mCtrl_pParentBookInfoGrid->SetTextMatrix(nRow,1,
			mCtrl_pNewBookInfoGrid->GetTextMatrix(nRow, 1));
	}

	AfxMessageBox(_T("적용되었습니다."));

	CDialog::OnOK();
}
	













































































int CILL_CHANGE_REGNO::Get_Sql_TransUpdate(CString &strQuery)
{
	CString strTransHeadSql = _T("UPDATE ILL_TRANS_TBL SET ");
	
	CString strTransMidSql;
	CString DBAlias[ROW_COUNT] = 
	{
		_T("GIVE_LIBCODE"), _T("REG_NO"), _T("TITLE_INFO"), _T("AUTHOR_INFO"), _T("PUBLISHER_INFO"), _T("PUB_YEAR_INFO"), _T("VOL_INFO"), _T("CALL_NO"), _T("PUBLISH_FORM_CODE")
	};
	CString DBValue[ROW_COUNT] = 
	{
		_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")
	};
	for(int nCnt = 0; nCnt < ROW_COUNT; nCnt++)
	{		
		CString strValue = mCtrl_pNewBookInfoGrid->GetTextMatrix(nCnt, 1);
		if(nCnt == 0)
		{
			CString strRegNo = mCtrl_pNewBookInfoGrid->GetTextMatrix(1, 1);
			CESL_DataMgr TmpDM;
			TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			TmpDM.StartFrame();
			CString strManageQuery = _T("");
			strManageQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = (SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REG_NO = '%s' AND ROWNUM = 1)"),strRegNo);
			TmpDM.GetOneValueQuery(strManageQuery,strValue);
			TmpDM.EndFrame();
		}
		strValue.Replace(_T("'"), _T("''"));
		DBValue[nCnt].Format(_T("'%s'"), strValue);
	}
	if(_T("'단행'") == DBValue[ROW_COUNT-1])
	{
		DBValue[ROW_COUNT-1] = _T("'MO'");
	}
	else if(_T("'연속'") == DBValue[ROW_COUNT-1])
	{
		DBValue[ROW_COUNT-1] = _T("'SE'");
	}	
	else if(_T("'긴급대출'") == DBValue[ROW_COUNT-1])
	{
		DBValue[ROW_COUNT-1] = _T("'NB'");
	}	
	
	for(nCnt = 0; nCnt < ROW_COUNT; nCnt++)
	{		
		strTransMidSql = strTransMidSql + DBAlias[nCnt] + _T("=") + DBValue[nCnt];
		strTransMidSql = strTransMidSql + _T(", ");
	}
	
	strTransMidSql.TrimRight(_T(", "));
	strTransMidSql = strTransMidSql + _T(", BOOK_KEY = ") + m_strNewBookKey;

	
	strTransMidSql = strTransMidSql + _T(", SPECIES_KEY = ") + m_strNewSpeciesKey;
	
	CString strCmAlias, strGridAlas;
	if(_T("요청") == m_strState)
	{
		strCmAlias = _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST");
		strGridAlas = _T("그리드_요청자료");
	}
	else
	{
		strCmAlias = _T("CM_BOOK_COOPERATIVE_GROUP_OFFER");
		strGridAlas = _T("그리드_제공자료");
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(strCmAlias, strGridAlas);
		if(NULL == pGrid) return -1;
	}
	
	
	
	
	
	
	
	
	
	
	if(_T("") == m_strTransNo) return -2;
	CString strTransTailSql = _T(" WHERE TRANS_NO = '") + m_strTransNo + _T("';");	
	
	strQuery = strTransHeadSql + strTransMidSql + strTransTailSql;
	
	return 0;
}



int CILL_CHANGE_REGNO::Get_Sql_OldBookUpdate(CString &strQuery)
{















	CString strRegNo = mCtrl_pOldBookInfoGrid->GetTextMatrix(1, 1);
	CString strManageCode = m_pInfo->MANAGE_CODE;
	CString strPublishFormCode = mCtrl_pOldBookInfoGrid->GetTextMatrix(8, 1);
	
	CString strTblName;
	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
		strTblName = _T("BO_BOOK_TBL");
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
		strTblName = _T("SE_BOOK_TBL"); 
	else if(_T("NB") == strPublishFormCode || _T("긴급대출") == strPublishFormCode)
		strTblName = _T("CO_NON_DB_BOOK_TBL");

	CString strWorkingStatus;
	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
		strWorkingStatus = _T("BOL411O");
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
		strWorkingStatus = _T("SEL411O");
	else if(_T("NB") == strPublishFormCode || _T("긴급대출") == strPublishFormCode) 
		strWorkingStatus = _T("BOL411O");

	strQuery.Format(
		_T("UPDATE ")
			_T("%s ") 
		_T("SET ")
			_T("WORKING_STATUS = PREV_WORKING_STATUS, ")
			_T("PREV_WORKING_STATUS = '%s', ") 
			_T("LAST_WORK = '%s' ")
		_T("WHERE ")
			_T("REC_KEY = %s AND MANAGE_CODE = '%s' AND WORKING_STATUS = '%s';"),

			strTblName,
			strWorkingStatus,
			GetWorkLogMsg(_T("책두레등록번호변경"), __WFILE__, __LINE__),
			m_strOldBookKey,
			strManageCode,
			strWorkingStatus
			);

	return 0;
}

int CILL_CHANGE_REGNO::Get_Sql_NewBookUpdate(CString &strQuery)
{
	CString strRegNo = mCtrl_pNewBookInfoGrid->GetTextMatrix(1, 1);
	
	
	
	CESL_DataMgr TmpDM;
	CString strManageCode = _T("");
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.StartFrame();
	CString strManageQuery = _T("");
	strManageQuery.Format(_T("SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REG_NO = '%s' AND ROWNUM=1"),strRegNo);
	TmpDM.GetOneValueQuery(strManageQuery,strManageCode);
	TmpDM.EndFrame();
	
	CString strPublishFormCode = mCtrl_pNewBookInfoGrid->GetTextMatrix(8, 1);

	CString strTblName;
	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
		strTblName = _T("BO_BOOK_TBL");
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
		strTblName = _T("SE_BOOK_TBL");
	else if(_T("NB") == strPublishFormCode || _T("긴급") == strPublishFormCode)
		strTblName = _T("CO_NON_DB_BOOK_TBL");

	CString strWorkingStatus;
	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
		strWorkingStatus = _T("BOL411O");
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
		strWorkingStatus = _T("SEL411O");
	else if(_T("NB") == strPublishFormCode || _T("긴급대출") == strPublishFormCode) 
		strWorkingStatus = _T("BOL411O");

	strQuery.Format(
		_T("UPDATE ")
			_T("%s ") 
		_T("SET ")
			_T("PREV_WORKING_STATUS = WORKING_STATUS, ")
			_T("WORKING_STATUS = '%s', ") 
			_T("LAST_WORK = '%s' ")
		_T("WHERE ")
			_T("REC_KEY = %s AND MANAGE_CODE = '%s';"),

			strTblName,
			strWorkingStatus,
			GetWorkLogMsg(_T("책두레등록번호변경"), __WFILE__, __LINE__),			
			m_strNewBookKey, 
			strManageCode			
		);
	return 0;
}

void CILL_CHANGE_REGNO::OnbtnILLSEARCHBOOK() 
{
	
	CILL_SEARCH_BOOK Dlg(this);
	Dlg.m_pInfo = m_pInfo;
	Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;
	Dlg.m_nSearchMode = 1;
	Dlg.DoModal();	
	if(_T("") != Dlg.m_strSelectedBookRegNo)
	{
		GetDlgItem(IDC_edtILL_CHANGE_REGNO_NEW_REGNO)->SetWindowText(Dlg.m_strSelectedBookRegNo);
		OnbtnILLCHANGEREGNOINPUTREGNO();
	}
}
