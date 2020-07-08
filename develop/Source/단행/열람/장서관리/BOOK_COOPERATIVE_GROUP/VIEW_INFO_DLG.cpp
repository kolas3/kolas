


#include "stdafx.h"
#include "resource.h"
#include "VIEW_INFO_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CVIEW_INFO_DLG::CVIEW_INFO_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	
	m_strCaption = _T("");
	mCtrl_pGrid = NULL;
}
CVIEW_INFO_DLG::~CVIEW_INFO_DLG()
{
}

void CVIEW_INFO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
		
	
}


BEGIN_MESSAGE_MAP(CVIEW_INFO_DLG, CDialog)
	//{{AFX_MSG_MAP(CVIEW_INFO_DLG)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




int CVIEW_INFO_DLG::Init_Grid()
{
	const int ROW_COUNT = (m_aHeader.GetSize() < m_aData.GetSize()) ? 
		m_aHeader.GetSize() : m_aData.GetSize();
	if(0 >= ROW_COUNT) return -1;
	
	
	
	
	
	
	
	mCtrl_pGrid->m_bFroceReverse = FALSE;
	mCtrl_pGrid->RemoveAll();	
	mCtrl_pGrid->SetFixedRows(1);
	mCtrl_pGrid->SetFixedCols(0);
	mCtrl_pGrid->SetRows(ROW_COUNT);
	mCtrl_pGrid->SetCols(0, 2);
	mCtrl_pGrid->SetColWidth(0, 0, HEADER_WIDTH);
	mCtrl_pGrid->SetColWidth(1, 0, DATA_WIDTH);	
	mCtrl_pGrid->SetRowHeight(-1, HEIGHT);
	mCtrl_pGrid->SetRowHeight(0, HEIGHT);
	
	
	
	
	COLORREF cr_Tag_Bk[2] = { RGB(242, 222, 176), RGB(247, 238, 218) };
	COLORREF cr_Tag_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };
	COLORREF cr_Data_Bk[2] = { RGB(237, 237, 237), RGB(247, 238, 218) };
	COLORREF cr_Data_Fk[2] = { RGB(0, 0, 0), RGB(0, 0, 0) };

	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		int nIdx = (0 != nRow) ? (nRow % 2) : 0;
		SetGridCellColor(mCtrl_pGrid, nRow, 0, cr_Tag_Bk[nIdx], cr_Tag_Fk[nIdx]);
		SetGridCellColor(mCtrl_pGrid, nRow, 1, cr_Data_Bk[nIdx], cr_Data_Fk[nIdx]);
		mCtrl_pGrid->SetTextMatrix(nRow, 0, m_aHeader.GetAt(nRow));		
		mCtrl_pGrid->SetTextMatrix(nRow, 1, m_aData.GetAt(nRow));		
	}

	return 0;
}
VOID CVIEW_INFO_DLG::SetGridCellColor(CESL_Grid *pGrid, INT nRow, INT nCol, COLORREF bk, COLORREF fk)
{
	if ( pGrid )
	{
		if(0 != nRow)
		{
			pGrid->SetRow(nRow);
			pGrid->SetCol(nCol);
			pGrid->SetCellBackColor(bk);
			pGrid->SetCellForeColor(fk);
			pGrid->SetCellAlignment(1);	
		}		
		else
		{
			pGrid->SetCellAlignment(4);
		}
	}

	return;
}

BOOL CVIEW_INFO_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	mCtrl_pGrid = new CESL_Grid(NULL);
	mCtrl_pGrid->SubclassDlgItem(IDC_grid_VIEW_INFO, this);

	Init_Grid();
	ResizeControl();

	SetWindowText(m_strCaption);

	return TRUE;  
	              
}


int CVIEW_INFO_DLG::SetData(CString strCaption, CStringArray& aHeader, CStringArray& aData)
{
	const int COUNT = (aHeader.GetSize() < aData.GetSize()) ?
		aHeader.GetSize() : aData.GetSize();
	
	m_aHeader.RemoveAll();
	m_aData.RemoveAll();

	m_aHeader.Add(_T("항목"));
	m_aData.Add(_T("정보"));
	for(int nCnt = 0; nCnt < COUNT; nCnt++)
	{
		m_aHeader.Add(aHeader.GetAt(nCnt));
		m_aData.Add(aData.GetAt(nCnt));
	}

	m_strCaption = strCaption;

	return 0;
}

HBRUSH CVIEW_INFO_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

int CVIEW_INFO_DLG::ResizeControl()
{
	
	
	

	CRect rGrid;
	mCtrl_pGrid->GetWindowRect(rGrid);
	ScreenToClient(rGrid);
	const int ROW_COUNT = (int)mCtrl_pGrid->GetRow() + (int)mCtrl_pGrid->GetFixedRows();
	rGrid.bottom = rGrid.top + (ROW_COUNT * (HEIGHT/15) + 5);

	CRect rDlg_Clint;
	GetClientRect(rDlg_Clint);
	
	CRect rDlg_Window;
	GetWindowRect(rDlg_Window);
	ScreenToClient(rDlg_Window);

	const int _WIDTH = rDlg_Window.Width() - rDlg_Clint.Width();
	const int _HEIGHT = rDlg_Window.Height() - rDlg_Clint.Height();
	
	rDlg_Window.bottom = rDlg_Window.top + rGrid.Height();
	rDlg_Window.bottom += _HEIGHT;	
	
	CWnd* pBtn = (CWnd*)GetDlgItem(IDC_OK);
	if(NULL == pBtn || NULL == pBtn->GetSafeHwnd()) return -1;
	CRect rBtn;
	pBtn->GetWindowRect(rBtn);
	ScreenToClient(rBtn);
	rBtn.OffsetRect(-rBtn.left, -rBtn.top);
	rBtn.OffsetRect(rDlg_Window.left, rDlg_Window.bottom+5);
	rBtn.OffsetRect(rDlg_Window.Width()-rBtn.Width()-7, 0);
	pBtn->MoveWindow(rBtn);

	rDlg_Window.bottom += (rBtn.Height() + 15);
	ClientToScreen(rDlg_Window);
	MoveWindow(rDlg_Window);
	mCtrl_pGrid->MoveWindow(rGrid);		

	return 0;
}

void CVIEW_INFO_DLG::OnOk() 
{
	
	CDialog::OnOK();
}

int CVIEW_INFO_DLG::SetData(CString strCaption, CESL_DataMgr *pDM)
{
	if(NULL == pDM) return -1;

	m_aHeader.RemoveAll();
	m_aData.RemoveAll();
	
	m_aHeader.Add(_T("항목"));
	m_aData.Add(_T("정보"));

	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	const int ROW_COUNT = pDM->GetRowCount();
	const int COL_COUNT = pDM->GetColCount();
	for(int nCnt = 0; nCnt < COL_COUNT; nCnt++)
	{
		pDM->GetColumnRefInfo(
			nCnt, 
			FIELD_ALIAS, 
			FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);

		m_aHeader.Add(FIELD_ALIAS);
		m_aData.Add(pDM->GetCellData(FIELD_ALIAS, 0));
	}

	m_strCaption = strCaption;

	return 0;
}

BOOL CVIEW_INFO_DLG::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam || VK_F12 == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
