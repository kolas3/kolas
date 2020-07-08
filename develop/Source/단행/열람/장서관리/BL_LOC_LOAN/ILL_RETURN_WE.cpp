// ILL_RETURN_WE.cpp : implementation file
//

#include "stdafx.h"
#include "ILL_RETURN_WE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_RETURN_WE dialog


CILL_RETURN_WE::CILL_RETURN_WE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_RETURN_WE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pPreRegNoMgr = NULL;
}
CILL_RETURN_WE::~CILL_RETURN_WE()
{
}

void CILL_RETURN_WE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_RETURN_WE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_RETURN_WE, CDialog)
	//{{AFX_MSG_MAP(CILL_RETURN_WE)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_RETURN_WE message handlers

BOOL CILL_RETURN_WE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(0 > InitESL_Mgr(_T("SM_ILL_RETURN_WE")))
 	{
 	}

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_RETURN_WE"), _T("그리드_반납"));
	m_pDM = FindDM(_T("DM_ILL_RETURN_WE"));

	ResizeControl();
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CILL_RETURN_WE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

int CILL_RETURN_WE::Refresh()
{
	RefreshMainDM(_T(""));
	DisplayGrid();

	return 0;
}

int CILL_RETURN_WE::RefreshMainDM(CString strOption)
{	
	if(NULL == m_pDM) return -1;

	if(m_strUserKey.IsEmpty())
	{
		m_pDM->FreeData();
		return 1000;
	}

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

int CILL_RETURN_WE::DisplayGrid()
{
	if(NULL == m_pGrid) return -1;
	m_pGrid->Display();

	return 0;
}

void CILL_RETURN_WE::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

int CILL_RETURN_WE::ResizeControl()
{
	CRect rDlg;
	GetClientRect(rDlg);

	{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid_ILL_RETURN_WE);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -1;

	pWnd->MoveWindow(rDlg);
	}
	return 0;
}

BOOL CILL_RETURN_WE::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

BOOL CILL_RETURN_WE::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

int CILL_RETURN_WE::FindRegNo(CString strRegNo)
{
	int ids = 0;
	int ROW_COUNT = m_pDM->GetRowCount();
	CString strPrevRegNo;
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{		
		CString strGet;
		strPrevRegNo = strRegNo;
		ids = m_pDM->GetCellData(_T("등록번호"), nRow, strGet);
		if(0 > ids) return ids;

		CString strManageCode;
		ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), 0, strManageCode);
		ids = CLocCommonAPI::MakeRegNoSpaceToZero(this, strPrevRegNo, strManageCode, m_pPreRegNoMgr);

		if(strGet != strPrevRegNo)
		{
			continue;
		}

		return nRow;
	}
	return -1;
}

INT CILL_RETURN_WE::Get_SQL(CString &strGetQuery, CString strMode)
{
	INT aColIndex[256];	
	CString strTblName, strWhere, strBakTblName, strBakCondition, strBakCondition2;
	Get_TblName(strTblName, strMode);
 	Get_DefaultWhere(strWhere, strMode);

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
	}
	else if(_T("SE") == strMode)
	{
		strGetQuery.Replace(_T("<$청구기호$>"), _T("ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE)"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("IB.ST_ISSN"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
	}
	else if(_T("NB") == strMode)
	{
		strGetQuery.Replace(_T("<$청구기호$>"), _T("BB.CALL_NO"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
		strGetQuery.Replace(_T("IB.TITLE_INFO"),		_T("BB.TITLE"));
	}

	return 0;
}

int CILL_RETURN_WE::Get_TblName(CString &strTblName, CString strMode)
{
	strTblName = 
		_T("(SELECT ITL.*, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
		_T("FROM ILL_TRANS_TBL  ITL ) A");
	if(_T("MO") == strMode)
	{
		strTblName += _T(", BO_BOOK_TBL BB, IDX_BO_TBL IB, CO_LOAN_USER_TBL CLU, LS_WORK_T01 B");
	}
	else if(_T("SE") == strMode)
	{
		strTblName += _T(", SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU, LS_WORK_T01 B");
	}
	else if(_T("NB") == strMode)
	{
		strTblName += _T(", CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU, LS_WORK_T01 B");
	}


	return 0;
}


INT CILL_RETURN_WE::Get_DefaultWhere(CString &strWhere, CString strMode)
{
	if(_T("MO") == strMode)
	{
		strWhere.Format(
			_T("A.LOAN_KEY = B.REC_KEY AND A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY = IB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.STATE = '0014' AND B.STATUS IN ('L', 'D') AND ")
			_T("A.APL_LIBCODE = '%s' AND A.USER_KEY = %s"),
 		m_strLibCode, m_strUserKey);
	}
	else if(_T("SE") == strMode)
	{
		strWhere.Format(_T("A.LOAN_KEY = B.REC_KEY AND A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.VOL_KEY=V.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.STATE = '0014' AND B.STATUS IN ('L', 'D') AND A.PUBLISH_FORM_CODE = 'SE' AND ")
		_T("A.APL_LIBCODE = '%s' AND A.USER_KEY = %s"),
 		m_strLibCode, m_strUserKey);

	}
	else if(_T("NB") == strMode)
	{
		strWhere.Format(_T("A.LOAN_KEY = B.REC_KEY AND A.BOOK_KEY = BB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.STATE = '0014' AND B.STATUS IN ('L', 'D') AND A.PUBLISH_FORM_CODE = 'NB' AND ")
		_T("A.APL_LIBCODE = '%s' AND A.USER_KEY = %s"),
 		m_strLibCode, m_strUserKey);
	}

	return 0;
}
