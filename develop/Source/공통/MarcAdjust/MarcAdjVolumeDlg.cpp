// MarcAdjVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjVolumeDlg.h"
#include "MarcAdjFrame.h"
#include "CatESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjVolumeDlg dialog


CMarcAdjVolumeDlg::CMarcAdjVolumeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcAdjVolumeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;

	m_pGrid_Book = NULL; 
	m_pGrid_Volume = NULL; 
	

	m_pDM_Book = NULL;
	m_pDM_Volume = NULL;

	m_pEditCtrl = NULL;
	m_pComboBox = NULL;

	m_nSelectedGridRow = 0;
	m_nSelectedGridCol = 0;

	m_nSelectedDMRow = 0;
	m_pParent = pParent;

	m_pGrid_Vol_Code = NULL;
	m_nModeMONO_SERIAL = -1;
	m_pParent = NULL;
}

CMarcAdjVolumeDlg::~CMarcAdjVolumeDlg()
{
	
}

VOID CMarcAdjVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAdjVolumeDlg)
	DDX_Control(pDX, IDC_TAB_VOLUME, m_TabCtrl);
	DDX_Control(pDX, IDC_GROUP_BOX, m_GroupBoxCtrl);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcAdjVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcAdjVolumeDlg)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VOLUME, OnSelchangeTabVolume)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_VOLUME, OnSelchangingTabVolume)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDIT_MODIFY, OnEditCtrlUpdate)
	ON_MESSAGE(WM_COMBO_MODIFY, OnComboBoxUpdate)
	ON_MESSAGE(MSG_GRID_DISPLAY, OnGridDisplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjVolumeDlg Operations

INT CMarcAdjVolumeDlg::Init()
{
	EFS_BEGIN

	m_nModeMONO_SERIAL = static_cast<CCatESL_Mgr*>(pParentMgr)->m_nModeMONO_SERIAL;

	CString strSMAlias;
	CString strCMAlias;
	if (m_nModeMONO_SERIAL == MODE_MONO)
	{
		strSMAlias = _T("SM_마크정리_권별정보_단행");
		strCMAlias = _T("CM_마크정리_권별정보_단행");

		m_pDM_Book	= static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_Book;
		m_pDM_Volume	= static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_Volume;
	}
	else
	{
		strSMAlias = _T("SM_마크정리_권별정보_연속");
		strCMAlias = _T("CM_마크정리_권별정보_연속");

		m_pDM_Book	= static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_Book;
		m_pDM_Volume	= static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_HoldBook;
	}

	if (m_pDM_Book == NULL || m_pDM_Volume == NULL) return -1;

	if (InitESL_Mgr(strSMAlias) < 0)
		return -1;

	m_pCM = FindCM(strCMAlias);
	if (m_pCM == NULL) return -1;
	
	m_pGrid_Book	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID_1")));
	m_pGrid_Volume	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID_2")));
	

	if (m_pGrid_Book == NULL || m_pGrid_Volume == NULL) return -1;

	m_pGrid_Book->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Volume->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	m_arrStrComboItemList[0][0] = _T("관리구분_0");
	m_arrStrComboItemList[0][1] = _T("!관리구분.코드,!관리구분.설명");
	m_arrStrComboItemList[1][0] = _T("이용제한구분_0");
	m_arrStrComboItemList[1][1] = _T("!이용제한구분.코드,!이용제한구분.설명");
	m_arrStrComboItemList[2][0] = _T("자료실_0");
	m_arrStrComboItemList[2][1] = _T("!자료실구분.코드,!자료실구분.설명");
	m_arrStrComboItemList[3][0] = _T("별치기호_0");
	m_arrStrComboItemList[3][1] = _T("!별치기호.코드,!별치기호.설명");
	m_arrStrComboItemList[4][0] = _T("관리구분_1");
	m_arrStrComboItemList[4][1] = _T("!관리구분.코드,!관리구분.설명");
	m_arrStrComboItemList[5][0] = _T("이용제한구분_1");
	m_arrStrComboItemList[5][1] = _T("!이용제한구분.코드,!이용제한구분.설명");
	m_arrStrComboItemList[6][0] = _T("자료실_1");
	m_arrStrComboItemList[6][1] = _T("!자료실구분.코드,!자료실구분.설명");
	m_arrStrComboItemList[7][0] = _T("별치기호_1");
	m_arrStrComboItemList[7][1] = _T("!별치기호.코드,!별치기호.설명");

	m_arrStrCanModifyGridColAlias.RemoveAll();
	m_arrStrCanModifyGridColAlias.Add(_T("복본기호_0"));
	m_arrStrCanModifyGridColAlias.Add(_T("별치기호_0"));
	m_arrStrCanModifyGridColAlias.Add(_T("권연차기호_0"));
	m_arrStrCanModifyGridColAlias.Add(_T("권책기호_0"));
	m_arrStrCanModifyGridColAlias.Add(_T("자료실_0"));
	m_arrStrCanModifyGridColAlias.Add(_T("복본기호_1"));
	m_arrStrCanModifyGridColAlias.Add(_T("별치기호_1"));
	m_arrStrCanModifyGridColAlias.Add(_T("권연차기호_1"));
	m_arrStrCanModifyGridColAlias.Add(_T("권책기호_1"));
	m_arrStrCanModifyGridColAlias.Add(_T("자료실_1"));

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::InitTabCtrl()
{
	EFS_BEGIN

	if (m_nModeMONO_SERIAL == MODE_MONO)
	{
		m_TabCtrl.InsertItem(0, _T("책정보"));
		m_TabCtrl.InsertItem(1, _T("권정보"));
	}
	else
	{
		m_TabCtrl.InsertItem(0, _T("책정보"));
		m_TabCtrl.InsertItem(1, _T("소장책정보"));
	}	

	return 0;

	EFS_END
	return -1;
}
	
INT CMarcAdjVolumeDlg::DisplayGrids()
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return -1;

	CRect rcClient;
	m_GroupBoxCtrl.GetWindowRect(rcClient);
	ScreenToClient(rcClient);

	if (nCurSel == 0)
	{
		m_pGrid_Book->MoveWindow(rcClient);
		m_pGrid_Book->ShowWindow(SW_SHOW);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
	}
	else 
	{
		m_pGrid_Volume->MoveWindow(rcClient);
		m_pGrid_Volume->ShowWindow(SW_SHOW);
		m_pGrid_Book->ShowWindow(SW_HIDE);
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::Display()
{
	EFS_BEGIN

	if (m_nModeMONO_SERIAL == MODE_MONO)
		Display_BO();
	else if (m_nModeMONO_SERIAL == MODE_SERIAL)
		Display_SE();

	m_pCM->AllControlDisplay();

	OnGridDisplay(0, 0);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::Display_BO()
{	
	EFS_BEGIN

	static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi()->ConvertBookToVolume(m_pDM_Book, m_pDM_Volume);

	m_pDM_Book->SORT(_T("BB_권일련번호"));
	m_pDM_Volume->SORT(_T("BB_권일련번호"));

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::Display_SE()
{
	EFS_BEGIN

	m_pDM_Book->SORT(_T("SV_권일련번호"));
	m_pDM_Volume->SORT(_T("SV_권일련번호"));

	return 0;

	EFS_END
	return -1;
}

CESL_GridColumnInfo *CMarcAdjVolumeDlg::GetGridColumnInfo(CESL_Grid *pGrid, INT nCol)
{
	EFS_BEGIN

	if (pGrid == NULL || nCol < 0) return NULL;
	CESL_GridColumnInfo *pGridColInfo = NULL;
	POSITION pos = pGrid->m_arrColumnInfo.FindIndex(nCol);
	if (pos == NULL) return NULL;

	return reinterpret_cast<CESL_GridColumnInfo*>(pGrid->m_arrColumnInfo.GetAt(pos));	

	EFS_END
	return NULL;
}

INT CMarcAdjVolumeDlg::ShowControl()
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	
	CESL_Grid *pGrid = GetActiveGrid();
	if (pGrid == NULL) return -1;

	INT nCol = pGrid->GetCol();
	INT nRow = pGrid->GetRow();

	m_sOldGirdData = pGrid->GetTextMatrix(nRow, nCol);

	CString strText;
	strText.Format(_T("%s_%d"), pGrid->GetTextMatrix(0, nCol), nCurSel);

	if (!static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi()->IsStringInArray(&m_arrStrCanModifyGridColAlias, strText)) return -1;
	
	for (INT idx = 0; idx < g_nComboBoxAliasCnt; idx++)
	{
		if (strText == m_arrStrComboItemList[idx][0])
		{
			CESL_GridColumnInfo *pColInfo = GetGridColumnInfo(pGrid, nCol - 1);
			
			CESL_Control_Element *pCE = new CESL_Control_Element(this);
			pCE->WorkWnd		= this;
			pCE->CM_ALIAS		= m_pCM->CM_ALIAS;
			pCE->CTRL_ID		= IDC_MUUTICOMBO_BOX;
			pCE->CTRL_TYPE		= _T("M");
			pCE->LIST_DATA		= m_arrStrComboItemList[idx][1];
			pCE->LINK_DATA		= pColInfo->strColumnDMFieldAlias;
			
			m_pComboBox->ELEMENT = pCE;
			
			ShowComboBox(pGrid);

			delete pCE;

			return 0;
		}
	}
	
	ShowEditBox(pGrid);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::ShowEditBox(CESL_Grid *pGrid)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	m_pEditCtrl->SetWindowText(pGrid->GetTextMatrix(nRow, nCol));

	m_pEditCtrl->MoveWindow(rcCell);
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();

	m_nSelectedGridRow = nRow;
	m_nSelectedGridCol = nCol;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::ShowComboBox(CESL_Grid *pGrid)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	CRect rcCombo;
	m_pComboBox->GetClientRect(rcCombo);

	rcCell.bottom = rcCell.top + rcCombo.Height();

	m_pComboBox->FreeComboData();
	m_pComboBox->PrevRow = -1;
	m_pComboBox->PrevCol = -1;
	m_pComboBox->m_nMaxRowCnt = 0;
	
	m_pComboBox->ResetContent();
	m_pComboBox->InitMultiComboBoxRef();

	m_pComboBox->SelectString(pGrid->GetTextMatrix(pGrid->GetRow(), pGrid->GetCol()));
	
	m_pComboBox->MoveWindow(rcCell);
	m_pComboBox->ShowWindow(SW_SHOW);
	m_pComboBox->BringWindowToTop();

	m_pComboBox->ShowDropDown();
	m_pComboBox->Invalidate();
	m_pComboBox->ShowWindow(SW_SHOW);
	m_pComboBox->SetFocus();

	m_nSelectedGridRow = nRow;
	m_nSelectedGridCol = nCol;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

	EFS_END
	return -1;
}

CESL_DataMgr *CMarcAdjVolumeDlg::GetActiveDM()
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0)
		return NULL;

	if (nCurSel == 0)
		return m_pDM_Book;
	else if (nCurSel == 1)
		return m_pDM_Volume;

	return NULL;

	EFS_END
	return NULL;
}

CESL_Grid *CMarcAdjVolumeDlg::GetActiveGrid()
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0)
		return NULL;

	if (nCurSel == 0)
		return m_pGrid_Book;
	else if (nCurSel == 1)
		return m_pGrid_Volume;

	return NULL;

	EFS_END
	return NULL;
}

INT CMarcAdjVolumeDlg::ApplyDataChange(LPCTSTR lpszData)
{
	EFS_BEGIN

	if (lpszData == NULL) return -1;

	CESL_DataMgr *pDM = GetActiveDM();
	CESL_Grid *pGrid = GetActiveGrid();
	CCatApi *pCatApi = static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi();

	if (pDM == NULL || pGrid == NULL) return -1;

	pGrid->SetFocus();
	pGrid->SetTextMatrix(m_nSelectedGridRow, m_nSelectedGridCol, lpszData);
	
	CESL_GridColumnInfo *pGridColInfo = GetGridColumnInfo(pGrid, m_nSelectedGridCol - 1);
	if (pGridColInfo == NULL) return -1;

	pDM->SetCellData(pGridColInfo->strColumnDMFieldAlias, lpszData, m_nSelectedGridRow - 1);
	
	if ( pGridColInfo->strColumnDMFieldAlias == _T("BB_권책기호"))
		pDM->SetCellData(_T("BB_편권차"), lpszData, m_nSelectedGridRow - 1);
	
	if (m_TabCtrl.GetCurSel() == 1)
		pCatApi->ApplyVolumeUpdate(m_pDM_Volume, m_pDM_Book, m_nSelectedGridRow - 1, pGridColInfo->strColumnDMFieldAlias, lpszData);

	CString strQueryFlag = pCatApi->GetQueryFlag(MNG_MODIFY, static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() );
	
	if (pDM->GetCellData(_T("BB_DB_FLAG"), m_nSelectedGridRow - 1) != _T("I"))
	{
		pDM->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, m_nSelectedGridRow - 1);
	}

	GetParentFrame()->PostMessage(WM_VOLUME_UPDATE, 0, 0);

	Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	INT nRows = pGrid->GetRows();
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
	}
	
	pGrid->SetCol(nCol);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjVolumeDlg::ChangeGridLinkDMtoSerial()
{
	EFS_BEGIN

	CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
	strLinkData.Replace(_T("단행"), _T("연속"));
	m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
	m_pGrid_Book->InitColumnLinkDMAndAlias();
	m_pGrid_Book->InitColumnInfo();

	strLinkData = m_pGrid_Volume->ELEMENT->LINK_DATA;
	strLinkData.Replace(_T("단행"), _T("연속"));
	m_pGrid_Volume->ELEMENT->LINK_DATA = strLinkData;
	m_pGrid_Volume->InitColumnLinkDMAndAlias();
	m_pGrid_Volume->InitColumnInfo();

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjVolumeDlg message handlers

BOOL CMarcAdjVolumeDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	if (Init() < 0) goto ERR;
	
	if (InitTabCtrl() < 0) goto ERR;

	m_pComboBox->SubclassDlgItem(IDC_cCODE_CAT, this);
	
	if (!m_pEditCtrl->GetSafeHwnd() || !m_pComboBox->GetSafeHwnd()) goto ERR;

	m_pEditCtrl->SetFont(GetFont());
	m_pComboBox->SetFont(GetFont());

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	MessageBox(_T("다이얼로그 초기화 실패"));
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

INT CMarcAdjVolumeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	EFS_BEGIN

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pEditCtrl = new CMarcAdjEdit;
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE, CRect(0, 0, 0, 0), this, IDC_EDIT_VOL);
	m_pEditCtrl->ShowWindow(SW_HIDE);

	m_pComboBox = new CMarcAdjComboBox(NULL);
				
	return 0;

	EFS_END
	return -1;
}


VOID CMarcAdjVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;

	if (m_pGrid_Book == NULL || m_pGrid_Volume == NULL) return;

	CRect rcClient;
	GetClientRect(rcClient);

	m_TabCtrl.MoveWindow(rcClient);

	CRect rcTab;
	m_TabCtrl.GetItemRect(0, rcTab);

	rcClient.top += rcTab.Height();

	rcClient.DeflateRect(10, 10);
	m_GroupBoxCtrl.MoveWindow(rcClient);

	DisplayGrids();

	EFS_END
}

VOID CMarcAdjVolumeDlg::OnSelchangeTabVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	DisplayGrids();
	
	*pResult = 0;

	EFS_END
}

VOID CMarcAdjVolumeDlg::OnSelchangingTabVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	if (m_pEditCtrl->IsWindowVisible())
	{
		OnEditCtrlUpdate(1, 0);
	}
	
	*pResult = 0;

	EFS_END
}


BEGIN_EVENTSINK_MAP(CMarcAdjVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcAdjVolumeDlg)
	ON_EVENT(CMarcAdjVolumeDlg, IDC_GRID_BOOK_ADJ, -605 /* MouseDown */, OnMouseDownGridBookAdj, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcAdjVolumeDlg, IDC_GRID_BOOK_ADJ, -601 /* DblClick */, OnDblClickGridBookAdj, VTS_NONE)
	ON_EVENT(CMarcAdjVolumeDlg, IDC_GRID_VOLUME_ADJ, -601 /* DblClick */, OnDblClickGridVolumeAdj, VTS_NONE)
	ON_EVENT(CMarcAdjVolumeDlg, IDC_GRID_BOOK_ADJ, -607 /* MouseUp */, OnMouseUpGridBookAdj, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

LRESULT CMarcAdjVolumeDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	if (m_nModeMONO_SERIAL == MODE_MONO)
	{
		INT nRows = m_pDM_Book->GetRowCount();
		for (INT idx = 0; idx < nRows; idx++)
		{
			if (m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Book->SetRowHeight(idx + 1, 0);			
			}
		}

		INT nIdx;
		static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi()->GetRepresentationBook(m_pDM_Book, _T("BB"), nIdx);

		m_nSelectedDMRow = 0;
		m_nSelectedGridRow = m_nSelectedDMRow + 1;

		if (nIdx >= 0 && m_TabCtrl.GetCurSel() == 0)
			SetGridRowBackColor(m_pGrid_Book, nIdx);

		if (nIdx != m_nSelectedDMRow)
			m_pGrid_Book->SetReverse(m_nSelectedDMRow);
		else
			m_pGrid_Book->m_nReverse = -1;
	}
	else if (m_nModeMONO_SERIAL == MODE_SERIAL)
	{
		INT nRows = m_pDM_Book->GetRowCount();
		for (INT idx = 0; idx < nRows; idx++)
		{
			if (m_pDM_Book->GetCellData(_T("SB_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Book->SetRowHeight(idx + 1, 0);			
			}
		}
		nRows = m_pDM_Volume->GetRowCount();
		for (idx = 0; idx < nRows; idx++)
		{
			if (m_pDM_Volume->GetCellData(_T("SB_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Volume->SetRowHeight(idx + 1, 0);			
			}
		}
	}


	return 0;

	EFS_END
	return NULL;
}

INT CMarcAdjVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	EFS_BEGIN

	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	
	if (strDMFieldAlias.Find(_T("자료상태")) >= 0)
		str = static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi()->GetWorkingStatusDesc(str);
	
	return 0;

	EFS_END
	return -1;
}

VOID CMarcAdjVolumeDlg::OnMouseDownGridBookAdj(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	m_nSelectedDMRow = m_pGrid_Book->GetRow() - 1;

	EFS_END
}

VOID CMarcAdjVolumeDlg::OnMouseUpGridBookAdj(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	m_nSelectedDMRow = m_pGrid_Book->GetRow() - 1;

	INT nIdx;
	if (m_nModeMONO_SERIAL == MODE_MONO)
		static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi()->GetRepresentationBook(m_pDM_Book, _T("BB"), nIdx);
	else static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi()->GetRepresentationBook(m_pDM_Book, _T("SB"), nIdx);

	if (m_nSelectedDMRow == nIdx)
		SetGridRowBackColor(m_pGrid_Book, m_nSelectedDMRow);

	EFS_END
}

VOID CMarcAdjVolumeDlg::OnDblClickGridBookAdj() 
{
	EFS_BEGIN

	if (m_pGrid_Book->GetMouseRow() == 0) return;
	ShowControl();

	EFS_END
}

VOID CMarcAdjVolumeDlg::OnDblClickGridVolumeAdj() 
{
	EFS_BEGIN

	if (m_pGrid_Volume->GetMouseRow() == 0) return;
	ShowControl();

	EFS_END
}

UINT CMarcAdjVolumeDlg::OnEditCtrlUpdate(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	SetFocus();

	if (m_pEditCtrl == NULL) return -1;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return -1;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			
	if (wParam != 1) return 0;

	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	if ( m_sOldGirdData != strData )
		ApplyDataChange(strData);

	return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjVolumeDlg::OnComboBoxUpdate(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	SetFocus();

	if (m_pComboBox == NULL) return -1;

	if (!::IsWindowVisible(m_pComboBox->GetSafeHwnd())) return -1;
	
	m_pComboBox->ShowWindow(SW_HIDE);

	if (wParam != 1) return 0;

	CString strData;
	m_pComboBox->GetData(strData);

	ApplyDataChange(strData);

	
	return 0;

	EFS_END
	return -1;
}

BOOL CMarcAdjVolumeDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	CString strKey;
    INT nIds;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_TAB)
		{
			m_pParent->SendMessage(WM_USER+1001, 0,0);
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			ShowControl();
			return TRUE;
		}
		if ( GetKeyState(VK_CONTROL) < 0 )
		{
			strKey.Format(_T("CTRL+%c"), pMsg->wParam);
			if (strKey == _T("CTRL+S")) 
			{
				pMsg->wParam = 0;
				nIds = static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->CMD_Save();
				if( 0 <= nIds )
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
		
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CMarcAdjVolumeDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pComboBox)
	{ 
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	if (m_pEditCtrl)
	{
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;
	}

	EFS_END
}

HBRUSH CMarcAdjVolumeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
