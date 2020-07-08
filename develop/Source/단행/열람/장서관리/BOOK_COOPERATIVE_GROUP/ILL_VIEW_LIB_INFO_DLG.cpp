


#include "stdafx.h"
#include "resource.h"
#include "ILL_VIEW_LIB_INFO_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_VIEW_LIB_INFO_DLG::CILL_VIEW_LIB_INFO_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	
	m_nIdx = -1;

	mCtrl_pGrid = NULL;
	
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
}
CILL_VIEW_LIB_INFO_DLG::~CILL_VIEW_LIB_INFO_DLG()
{
}

void CILL_VIEW_LIB_INFO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
		
	
}


BEGIN_MESSAGE_MAP(CILL_VIEW_LIB_INFO_DLG, CDialog)
	//{{AFX_MSG_MAP(CILL_VIEW_LIB_INFO_DLG)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




int CILL_VIEW_LIB_INFO_DLG::Init_Grid()
{
	if(0 >= ROW_COUNT) return -1;
	
	
	
	
	
	
	
	mCtrl_pGrid->m_bFroceReverse = FALSE;
	mCtrl_pGrid->RemoveAll();	
	mCtrl_pGrid->SetFixedRows(0);
	mCtrl_pGrid->SetFixedCols(0);
	mCtrl_pGrid->SetRows(ROW_COUNT);
	mCtrl_pGrid->SetCols(0, 2);
	mCtrl_pGrid->SetColWidth(0, 0, HEADER_WIDTH);
	mCtrl_pGrid->SetColWidth(1, 0, DATA_WIDTH);	
	mCtrl_pGrid->SetRowHeight(-1, HEIGHT);
	mCtrl_pGrid->SetRowHeight(0, HEIGHT);
	
	
	
	
	COLORREF cr_Tag_Bk[2] = { RGB(255, 162, 99), RGB(255, 223, 206) };
	COLORREF cr_Tag_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };
	COLORREF cr_Data_Bk[2] = { RGB(189, 227, 231), RGB(239, 247, 247) };
	COLORREF cr_Data_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };


 	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
 	{
 		int nIdx = (0 != nRow) ? (nRow % 2) : 0;
 		SetGridCellColor(mCtrl_pGrid, nRow, 0, cr_Tag_Bk[nIdx], cr_Tag_Fk[nIdx]);
 		SetGridCellColor(mCtrl_pGrid, nRow, 1, cr_Data_Bk[nIdx], cr_Data_Fk[nIdx]);
 	}

	return 0;
}
VOID CILL_VIEW_LIB_INFO_DLG::SetGridCellColor(CESL_Grid *pGrid, INT nRow, INT nCol, COLORREF bk, COLORREF fk)
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

BOOL CILL_VIEW_LIB_INFO_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	mCtrl_pGrid = new CESL_Grid(NULL);
	mCtrl_pGrid->SubclassDlgItem(IDC_grid_ILL_LIB_VIEW_INFO, this);
	mCtrl_pGrid->InitToolTip();

	Init_Grid();
	SetData(m_nIdx);
	ResizeControl();

	return TRUE;  
	              
}




























int CILL_VIEW_LIB_INFO_DLG::ResizeControl()
{
	
	
	

	CRect rGrid;
	mCtrl_pGrid->GetWindowRect(rGrid);	
	ScreenToClient(rGrid);
	const int ROW = ROW_COUNT;
	rGrid.bottom = rGrid.top + (ROW * (HEIGHT/15) + 5);

	CRect rDlg_Clint;
	GetClientRect(rDlg_Clint);
	
	CRect rDlg_Window;
	GetWindowRect(rDlg_Window);
	ScreenToClient(rDlg_Window);

	const int _WIDTH = rDlg_Window.Width() - rDlg_Clint.Width();
	const int _HEIGHT = rDlg_Window.Height() - rDlg_Clint.Height();
	const int _SPACE_ = 10; 

	rDlg_Window.bottom = rDlg_Window.top + rGrid.Height() + rGrid.top + _SPACE_;
	rDlg_Window.bottom += _HEIGHT;	
	
	CWnd* pBtn = (CWnd*)GetDlgItem(IDOK);
	if(NULL == pBtn || NULL == pBtn->GetSafeHwnd()) return -1;
	CRect rBtn;
	pBtn->GetWindowRect(rBtn);
	ScreenToClient(rBtn);
	rBtn.OffsetRect(-rBtn.left, -rBtn.top);
	rBtn.OffsetRect(rDlg_Window.left, rDlg_Window.bottom+5);
	rBtn.OffsetRect(rDlg_Window.Width()-rBtn.Width()-7, 0);
	pBtn->MoveWindow(rBtn);

	{
		CWnd* _pBtn = (CWnd*)GetDlgItem(IDC_btnPREV);
		if(NULL == _pBtn || NULL == _pBtn->GetSafeHwnd()) return -1;

		CRect _rBtn;
		_pBtn->GetWindowRect(_rBtn);
		ScreenToClient(_rBtn);

		_rBtn.OffsetRect(0, -_rBtn.top);
		_rBtn.OffsetRect(0, rBtn.top);
		_pBtn->MoveWindow(_rBtn);
	}
	{
		CWnd* _pBtn = (CWnd*)GetDlgItem(IDC_btnNEXT);
		if(NULL == _pBtn || NULL == _pBtn->GetSafeHwnd()) return -1;

		CRect _rBtn;
		_pBtn->GetWindowRect(_rBtn);
		ScreenToClient(_rBtn);

		_rBtn.OffsetRect(0, -_rBtn.top);
		_rBtn.OffsetRect(0, rBtn.top);
		_pBtn->MoveWindow(_rBtn);
	}

	rDlg_Window.bottom += (rBtn.Height() + 15);
	ClientToScreen(rDlg_Window);
	MoveWindow(rDlg_Window);
	mCtrl_pGrid->MoveWindow(rGrid);	

	return 0;
}

void CILL_VIEW_LIB_INFO_DLG::OnOk() 
{
	
	CDialog::OnOK();
}































BOOL CILL_VIEW_LIB_INFO_DLG::PreTranslateMessage(MSG* pMsg) 
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

int CILL_VIEW_LIB_INFO_DLG::SetData(int nIdx)
{
	if(NULL == m_pParentDM) return -1;
	if(NULL == mCtrl_pGrid) return -2;
	if(m_pParentDM->GetRowCount() <= nIdx) return -8;
			
	int ids = 0;
	CString strGet;

	ids = m_pParentDM->GetCellData(m_strLibAlias, nIdx, strGet);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentDM->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pParentDM->MANAGE_CODE);

	CString strQuery = _T("");
	strQuery.Format(   
		_T("SELECT A.LIB_NAME, A.LIB_TEL, A.LIB_FAX, A.LIB_ADDRESS, A.LIB_URL ")
		_T("FROM MN_LIBINFO2_TBL A ")
		_T("WHERE A.LIB_CODE = '%s'"),
		strGet
		);
	ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids) return 9;

	const INT ITEM_COUNT = 5;
	CString strAlias[ITEM_COUNT] = 
	{
		_T("도서관명"),
		_T("전화번호"),
		_T("팩스번호"),
		_T("주소"),
		_T("홈페이지")
	};
		
	for(int nCnt = 0; nCnt < ITEM_COUNT; nCnt++)
	{
		mCtrl_pGrid->SetTextMatrix(nCnt, 0, strAlias[nCnt]);

		CString strData;
		TmpDM.GetCellData(0, nCnt, strData);
		mCtrl_pGrid->SetTextMatrix(nCnt, 1, strData);
	}

	m_pParentGrid->SetReverse(nIdx);

	return 0;	
}

void CILL_VIEW_LIB_INFO_DLG::OnbtnPREV() 
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

void CILL_VIEW_LIB_INFO_DLG::OnbtnNEXT() 
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
