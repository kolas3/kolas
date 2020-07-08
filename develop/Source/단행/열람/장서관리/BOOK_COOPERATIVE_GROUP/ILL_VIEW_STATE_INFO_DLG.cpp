


#include "stdafx.h"
#include "resource.h"
#include "ILL_VIEW_STATE_INFO_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_VIEW_STATE_INFO_DLG::CILL_VIEW_STATE_INFO_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	
	m_nIdx = -1;
	
	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	for(int nCnt = 0; nCnt < GRID_COUNT; nCnt++)
	{
		mCtrl_pGrid[nCnt] = NULL;
	}
}
CILL_VIEW_STATE_INFO_DLG::~CILL_VIEW_STATE_INFO_DLG()
{
	for(int nCnt = 0; nCnt < GRID_COUNT; nCnt++)
	{
		if(NULL != mCtrl_pGrid[nCnt]->GetSafeHwnd()) mCtrl_pGrid[nCnt]->DestroyWindow();
		delete mCtrl_pGrid[nCnt];
	}
}

void CILL_VIEW_STATE_INFO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
		
	
}


BEGIN_MESSAGE_MAP(CILL_VIEW_STATE_INFO_DLG, CDialog)
	//{{AFX_MSG_MAP(CILL_VIEW_STATE_INFO_DLG)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




int CILL_VIEW_STATE_INFO_DLG::Init_Grid()
{	
	const int ROW_COUNT[GRID_COUNT] = 
	{
		1, 4, 2, 2, 3, 2
	};
	const int GRID_ID[GRID_COUNT] = 
	{
		IDC_grid_ILL_STATE_VIEW_INFO,
		IDC_grid_ILL_STATE_VIEW_INFO2,
		IDC_grid_ILL_STATE_VIEW_INFO3,
		IDC_grid_ILL_STATE_VIEW_INFO4,
		IDC_grid_ILL_STATE_VIEW_INFO5,
		IDC_grid_ILL_STATE_VIEW_INFO6
	};
	for(int nCnt = 0; nCnt < GRID_COUNT; nCnt++)
	{
		CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(GRID_ID[nCnt]);
		
		
		


		pGrid->SetFixedRows(0);
		pGrid->SetFixedCols(0);
		pGrid->SetRows(ROW_COUNT[nCnt]);
		pGrid->SetCols(0, 2);
		pGrid->SetColWidth(0, 0, HEADER_WIDTH);
		pGrid->SetColWidth(1, 0, DATA_WIDTH);	
		pGrid->SetRowHeight(-1, HEIGHT);
		pGrid->SetRowHeight(0, HEIGHT);
		
		
		
		
		COLORREF cr_Tag_Bk[2] = { RGB(255, 162, 99), RGB(255, 223, 206) };
		COLORREF cr_Tag_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };
		COLORREF cr_Data_Bk[2] = { RGB(189, 227, 231), RGB(239, 247, 247) };
		COLORREF cr_Data_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };
		
		for(int nRow = 0; nRow < ROW_COUNT[nCnt]; nRow++)
		{
			int nIdx = nRow % 2;
			SetGridCellColor(pGrid, nRow, 0, cr_Tag_Bk[nIdx], cr_Tag_Fk[nIdx]);
			SetGridCellColor(pGrid, nRow, 1, cr_Data_Bk[nIdx], cr_Data_Fk[nIdx]);
		}
	}
	return 0;
}
VOID CILL_VIEW_STATE_INFO_DLG::SetGridCellColor(CMSHFlexGrid *pGrid, INT nRow, INT nCol, COLORREF bk, COLORREF fk)
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

BOOL CILL_VIEW_STATE_INFO_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	const int GRID_ID[GRID_COUNT] = 
	{
		IDC_grid_ILL_STATE_VIEW_INFO,
		IDC_grid_ILL_STATE_VIEW_INFO2,
		IDC_grid_ILL_STATE_VIEW_INFO3,
		IDC_grid_ILL_STATE_VIEW_INFO4,
		IDC_grid_ILL_STATE_VIEW_INFO5,
		IDC_grid_ILL_STATE_VIEW_INFO6
	};
	for(int nCnt = 0; nCnt < GRID_COUNT; nCnt++)
	{
		mCtrl_pGrid[nCnt] = new CESL_Grid(NULL);
		mCtrl_pGrid[nCnt]->SubclassDlgItem(GRID_ID[nCnt], this);
		mCtrl_pGrid[nCnt]->InitToolTip();
	}
	Init_Grid();
	SetData(m_nIdx);

	return TRUE;  
	              
}

void CILL_VIEW_STATE_INFO_DLG::OnOk() 
{
	
	CDialog::OnOK();
}

BOOL CILL_VIEW_STATE_INFO_DLG::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam || VK_F12 == pMsg->wParam || VK_F11 == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

int CILL_VIEW_STATE_INFO_DLG::SetData(int nIdx)
{
	if(NULL == m_pParentDM) return -1;	
	if(m_pParentDM->GetRowCount() <= nIdx) return -8;
			
	int ids = 0;
	CString strGet;

	ids = m_pParentDM->GetCellData(_T("트랜잭션번호"), nIdx, strGet);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentDM->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pParentDM->MANAGE_CODE);

	CString strQuery = _T("");
	if(_T("D") != m_sBookCooperativeMode)
	{
		strQuery.Format(
			_T("SELECT ")
			
			_T("(SELECT B.DESCRIPTION FROM ILL_CODE_TBL B WHERE B.CODE = A.STATE AND B.CLASS = 'i05') AS CSTATE, ")
			
			_T("(SELECT DESCRIPTION FROM ILL_CODE_TBL WHERE CLASS = 'i01' AND CODE = A.APL_DIV) AS APL_DESCRIPTION, ")
			
			_T("TO_CHAR(A.APL_DATE, 'YYYY/MM/DD HH24:MI:SS') AS APL, ")
			
			_T("A.APL_CONTENT, ")
			
			_T("A.APL_USER_CONTENT, ")
			
			_T("TO_CHAR(A.APLREQ_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || (SELECT B.NAME FROM MN_USER_TBL B WHERE B.ID = A.APLREQ_USERID) AS APLREQ, ")
			
			_T("A.APLREQ_CONTENT, ")
			
			_T("TO_CHAR(A.GIVE_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || (SELECT B.NAME FROM MN_USER_TBL B WHERE B.ID = A.GIVE_USERID) AS GIVE, ")
			
			_T("A.GIVE_CONTENT, ")
			
			_T("TO_CHAR(A.TAKE_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || (SELECT B.NAME FROM MN_USER_TBL B WHERE B.ID = A.TAKE_USERID) AS TAKE, ")
			
			_T("A.RETURN_PLAN_DATE, ")
			
			_T("TO_CHAR(A.RETURN_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || (SELECT B.NAME FROM MN_USER_TBL B WHERE B.ID = A.RETURN_USERID) AS RETURN, ")
			
			_T("TO_CHAR(A.END_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || (SELECT B.NAME FROM MN_USER_TBL B WHERE B.ID = A.END_USERID) AS END, ")
			
			_T("A.END_CONTENT ")
			_T("FROM ILL_TRANS_TBL A ")
			_T("WHERE A.TRANS_NO = '%s'"),
			strGet
			);
	}
	else
	{
		strQuery.Format(
			_T("SELECT ")
			
			_T("(SELECT B.DESCRIPTION FROM ILL_CODE_TBL B WHERE B.CODE = A.STATE AND B.CLASS = 'i05') AS CSTATE, ")
			
			_T("(SELECT DESCRIPTION FROM ILL_CODE_TBL WHERE CLASS = 'i01' AND CODE = A.APL_DIV) AS APL_DESCRIPTION, ")
			
			_T("TO_CHAR(A.APL_DATE, 'YYYY/MM/DD HH24:MI:SS') AS APL, ")
			
			_T("A.APL_CONTENT, ")
			
			_T("A.APL_USER_CONTENT, ")
			
			_T("TO_CHAR(A.APLREQ_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || A.APLREQ_USERID AS APLREQ, ")
			
			_T("A.APLREQ_CONTENT, ")
			
			_T("TO_CHAR(A.GIVE_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || A.GIVE_USERID AS GIVE, ")
			
			_T("A.GIVE_CONTENT, ")
			
			_T("TO_CHAR(A.TAKE_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || A.TAKE_USERID AS TAKE, ")
			
			_T("A.RETURN_PLAN_DATE, ")
			
			_T("TO_CHAR(A.RETURN_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || A.RETURN_USERID AS RETURN, ")
			
			_T("TO_CHAR(A.END_DATE, 'YYYY/MM/DD HH24:MI:SS') || '/' || A.END_USERID AS END, ")
			
			_T("A.END_CONTENT ")
			_T("FROM ILL_TRANS_TBL A ")
			_T("WHERE A.TRANS_NO = '%s'"),
			strGet
			);
	}
	ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids) return 9;

	const INT ITEM_COUNT = 14;
	CString strAlias[ITEM_COUNT] = 
	{
		_T("현재상태"),
		_T("신청유형"),
		_T("신청일"),
		_T("신청비고"),
		_T("신청 이용자 의견"),
		_T("요청일/요청처리자"),
		_T("요청비고"),
		_T("발송일/발송처리자"),
		_T("발송비고"),
		_T("입수일/입수처리자"),
		_T("복귀예정일"),
		_T("복귀일/복귀처리자"),
		_T("완료일/완료처리자"),
		_T("완료비고")		
	};
		
	const int ROW_COUNT[GRID_COUNT] = 
	{
		1, 4, 2, 2, 3, 2
	};
	const int GRID_ID[GRID_COUNT] = 
	{
		IDC_grid_ILL_STATE_VIEW_INFO,
		IDC_grid_ILL_STATE_VIEW_INFO2,
		IDC_grid_ILL_STATE_VIEW_INFO3,
		IDC_grid_ILL_STATE_VIEW_INFO4,
		IDC_grid_ILL_STATE_VIEW_INFO5,
		IDC_grid_ILL_STATE_VIEW_INFO6
	};	


	for(int nCnt = 0, nRowCnt = 0, nGridCount = 0; nCnt < ITEM_COUNT; nCnt++, nRowCnt++)
	{
		if(ROW_COUNT[nGridCount] <= nRowCnt)
		{
			nGridCount++;
			nRowCnt = 0;
		}
		CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(GRID_ID[nGridCount]);
		pGrid->SetTextMatrix(nRowCnt, 0, strAlias[nCnt]);

		CString strData;
		TmpDM.GetCellData(0, nCnt, strData);
		pGrid->SetTextMatrix(nRowCnt, 1, strData);
	}

	m_pParentGrid->SetReverse(nIdx);

	return 0;	
}

void CILL_VIEW_STATE_INFO_DLG::OnbtnPREV() 
{
	
	if(NULL == m_pParentGrid) return;
	int nIdx = m_pParentGrid->SelectGetPrev();
	if(0 > nIdx) 
	{
		m_pParentGrid->SelectGetHeadPosition();
		AfxMessageBox(_T("이전 항목이 없습니다."));
		return ;
	}
	
	SetData(nIdx);

}

void CILL_VIEW_STATE_INFO_DLG::OnbtnNEXT() 
{
	
	if(NULL == m_pParentGrid) return;
	int nIdx = m_pParentGrid->SelectGetNext();
	if(0 > nIdx) 
	{
		m_pParentGrid->SelectGetTailPosition();
		AfxMessageBox(_T("다음 항목이 없습니다."));
		return ;
	}

	SetData(nIdx);
}
