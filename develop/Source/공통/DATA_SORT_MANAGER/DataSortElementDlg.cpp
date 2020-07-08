// DataSortElementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataSortElementDlg.h"
#include "SortMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSortElementDlg dialog


CDataSortElementDlg::CDataSortElementDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDataSortElementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSortElementDlg)
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pItemSet = NULL;
	m_pComboBox = NULL;
}

CDataSortElementDlg::~CDataSortElementDlg()
{
}

VOID CDataSortElementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSortElementDlg)
	DDX_Control(pDX, IDC_lstSORT_ELEMENT, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSortElementDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSortElementDlg)
	ON_MESSAGE(SORT_ELEMENT_CHECK, CheckSortElement)
	ON_MESSAGE(SORT_DIRECTION_CHECK, CheckSortDirection)
	ON_MESSAGE(SORT_PRIORITY_CHECK, CheckSortPriority)
	ON_MESSAGE(SORT_ITEM_MODIFY, SortItemModify)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSortElementDlg message handlers

BOOL CDataSortElementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_정렬항목설정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_정렬항목설정"));
	m_pDM = FindDM(_T("DM_정렬항목설정"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));	
	m_pGrid->SetSort(FALSE);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("정렬항목 설정 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataSortElementDlg::Init()
{
	if(m_pItemSet == NULL) return FALSE;
	CSortMgr mgr(this);
	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("ITEMSET"), (DWORD*)m_pItemSet);
	INT ids = mgr.SPFExecutePath(_T("정렬항목DM설정"));
	if(ids < 0) return FALSE;
	
	InitListCtrl();
	Display();
	return TRUE;
}

VOID CDataSortElementDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_pItemSet->RemoveAll();
	INT ids = m_pItemSet->SetSortItem(m_pDM, m_pGrid);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정하신 정렬항목을 반영할 수 없습니다."));
		return ;
	}

	ids = m_pItemSet->WriteSortItem();
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("변경내용을 저장할 수 없습니다."));
		return ;
	}
	
	ESLAfxMessageBox(_T("변경내용을 저장하였습니다."));

	CDialog::OnOK();
}

VOID CDataSortElementDlg::Display()
{
	m_pGrid->Display();
	ListCtrlDisplay();
}

VOID CDataSortElementDlg::ListCtrlDisplay()
{
	INT nRowCount = m_pDM->GetRowCount();
	if(nRowCount ==  0) return ;
	INT nListBoxIdx = 0;
	INT nGridRows = m_pGrid->GetRows();
	CString strTmpData;
	m_ListCtrl.DeleteAllItems();
	
	for(INT i=1 ; i<nGridRows ; i++)
	{
		if(!CheckSelectedRow(i, 2)) continue;
		m_ListCtrl.InsertItem( nListBoxIdx,    m_pDM->GetCellData(_T("이름"), i-1));
		m_ListCtrl.SetItemText(nListBoxIdx, 1, m_pDM->GetCellData(_T("우선순위"), i-1));
		strTmpData = m_pDM->GetCellData(_T("정렬방법"), i-1);
		if(_ttoi(strTmpData.GetBuffer(0)) == ESL_DATAMGR_SORT_ASC) strTmpData = _T("오름차순");
		else if(_ttoi(strTmpData.GetBuffer(0)) == ESL_DATAMGR_SORT_DESC) strTmpData = _T("내림차순");
		m_ListCtrl.SetItemText(nListBoxIdx, 2, strTmpData) ;
		nListBoxIdx++;
	}	
}

BEGIN_EVENTSINK_MAP(CDataSortElementDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataSortElementDlg)
	ON_EVENT(CDataSortElementDlg, IDC_gridSORT_ELEMENT, -600 /* Click */, OnClickgridSORTELEMENT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDataSortElementDlg::OnClickgridSORTELEMENT() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return;
	
	if(m_pGrid->GetCol() == 2) 
		PostMessage(SORT_ELEMENT_CHECK);
	else if(m_pGrid->GetCol() == 3) 
		PostMessage(SORT_PRIORITY_CHECK);
	else if(m_pGrid->GetCol() == 4) 
		PostMessage(SORT_DIRECTION_CHECK);
}

UINT CDataSortElementDlg::CheckSortElement(LPARAM lParam, WPARAM wParam)
{
	INT ids;
	CString strCheck = m_pGrid->GetTextMatrix(m_pGrid->GetRow(), 1);
	if(strCheck == _T("선정")) return 0;

	if(!CheckSelectedRow(m_pGrid->GetRow(), m_pGrid->GetCol()))
	{
		ids = SortItemInsert(m_pGrid->GetRow());
	}
	else
	{
		ids = SortItemClear(m_pGrid->GetRow());
	}
	PostMessage(SORT_ITEM_MODIFY, m_pGrid->GetRow()-1, 0);
	return ids;
}

UINT CDataSortElementDlg::CheckSortDirection(LPARAM lParam, WPARAM wParam)
{
	if(!CheckSelectedRow(m_pGrid->GetRow(), 2)) return 0;
	CreateComboBoxInGridSell(SORT_DIRECTION_CHECK, SORT_ITEM_MODIFY);
	return 0;
}

UINT CDataSortElementDlg::CheckSortPriority(LPARAM lParam, WPARAM wParam)
{
	if(!CheckSelectedRow(m_pGrid->GetRow(), 2)) return 0;
	CreateComboBoxInGridSell(SORT_PRIORITY_CHECK, SORT_ITEM_MODIFY);
	return 0;
}

UINT CDataSortElementDlg::SortItemModify(LPARAM lParam, WPARAM wParam)
{
	CString strTitle, strPriority;
	strPriority = m_pDM->GetCellData(_T("우선순위"), lParam);
	INT nPriority = _ttoi(strPriority.GetBuffer(0));
	
	if(wParam != 0) RestructPriority(lParam, nPriority);
	
	ListCtrlDisplay();
	return 0;
}

BOOL CDataSortElementDlg::CheckSelectedRow(LONG nRow, LONG nCol)
{
	CString strCheck = m_pGrid->GetTextMatrix(nRow, nCol);
	if(strCheck == _T("V")) return TRUE;
	return FALSE;
}

VOID CDataSortElementDlg::CreateComboBoxInGridSell(UINT nMsg, UINT nPostMessage)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;
	
	INT ids, nColIdx;
	INT nRow = m_pGrid->GetRow();
	CString strData, strAlias;
	CStringArray ArrData;

	if(nMsg == SORT_PRIORITY_CHECK)
	{
		strAlias = _T("우선순위");
		strData = m_pDM->GetCellData(strAlias, nRow-1);
		nColIdx = 3;
		ids = SetPriorityComboListData(&ArrData);
		if(ids < 0) return;
	}
	else if(nMsg == SORT_DIRECTION_CHECK)
	{
		strAlias = _T("정렬방법");
		strData = m_pDM->GetCellData(strAlias, nRow-1);
		nColIdx = 4;
		ids = SetDirectionComboListData(&ArrData);
		if(ids < 0) return;
	}
	
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	m_pComboBox = new CSortComboBox;
	if(!m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450))
	{
		ESLAfxMessageBox(_T("콤보박스 컨트롤을 생성할 수 없습니다."));
		return;
	}

	m_pComboBox->Init(&ArrData);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM);
	m_pComboBox->SetParent(m_pGrid);
	m_pComboBox->SetCurSelData(strData);
	m_pComboBox->SetMessage(nPostMessage);
	m_pComboBox->SetColIndex(nColIdx);
	m_pComboBox->SetMainMgr(this);
	m_pComboBox->SetAlias(strAlias);
	
	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	m_pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);
	
	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - 3;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - 3;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);
	
	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	
	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CDataSortElementDlg::SetPriorityComboListData(CStringArray *pData)
{
	if(pData == NULL) return -1;

	pData->RemoveAll();
	INT nRowCnt = m_pDM->GetRowCount();
	INT nMaxPriority = GetMaxPriority(m_pDM);
	CString strData;
	for(INT i=1 ; i<=nMaxPriority ; i++)
	{
		strData.Format(_T("%d"), i);
		pData->Add(strData);
	}
	return 0;
}

INT CDataSortElementDlg::SetDirectionComboListData(CStringArray *pData)
{
	if(pData == NULL) return -1;
	
	pData->RemoveAll();
	pData->Add(_T("오름차순"));
	pData->Add(_T("내림차순"));
	return 0;
}

INT CDataSortElementDlg::SortItemClear(INT nGridMouseRow)
{
	if(nGridMouseRow > m_pDM->GetRowCount() || nGridMouseRow < 1)
		return 0;
	m_pDM->SetCellData(_T("우선순위"), _T(""), nGridMouseRow-1);
	m_pGrid->SetTextMatrix(nGridMouseRow, 3, _T(""));
	m_pDM->SetCellData(_T("정렬방법"), _T(""), nGridMouseRow-1);
	m_pGrid->SetTextMatrix(nGridMouseRow, 4, _T(""));
	m_pGrid->SetTextMatrix(nGridMouseRow, 2, _T(""));
	return 0;
}

INT CDataSortElementDlg::SortItemInsert(INT nGridMouseRow)
{
	if(nGridMouseRow > m_pDM->GetRowCount() || nGridMouseRow < 1)
		return 0;
	CString strTmpData;
	m_pGrid->SetTextMatrix(nGridMouseRow, 2, _T("V"));
	strTmpData.Format(_T("%d"), GetMaxPriority(m_pDM)+1);
	m_pDM->SetCellData(_T("우선순위"), strTmpData, nGridMouseRow-1);
	m_pGrid->SetTextMatrix(nGridMouseRow, 3, strTmpData);
	strTmpData.Format(_T("%d"), ESL_DATAMGR_SORT_ASC);
	m_pDM->SetCellData(_T("정렬방법"), strTmpData, nGridMouseRow-1);
	m_pGrid->SetTextMatrix(nGridMouseRow, 4, _T("오름차순"));
	return 0;
}

INT CDataSortElementDlg::GetMaxPriority(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	
	INT nRowCnt = pDM->GetRowCount();
	if(pDM->GetRowCount() == 0) return 0;
	
	INT nMaxPriority, nTmpData;
	nTmpData = 0;	nMaxPriority = -1;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		nTmpData = _ttoi(pDM->GetCellData(_T("우선순위"), i).GetBuffer(0));
		if(nTmpData > nMaxPriority) nMaxPriority = nTmpData;
	}
	
	return nMaxPriority;
}

INT CDataSortElementDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("정렬방법"))
	{	
		strTmpData = m_pDM->GetCellData(_T("정렬방법"), nRow);
		if(_ttoi(strTmpData.GetBuffer(0)) == ESL_DATAMGR_SORT_DESC)
			str = _T("내림차순");
		else if(_ttoi(strTmpData.GetBuffer(0)) == ESL_DATAMGR_SORT_ASC)
			str = _T("오름차순");
		else
			str = _T("");
	}
	else if(strDMFieldAlias == _T("이름"))
	{
		if(m_pItemSet->IsExist(str))
			m_pGrid->SetTextMatrix(nRow+1, 2, _T("V"));
	}
	return 0;
}

INT CDataSortElementDlg::InitListCtrl()
{	
	//리스트컨트롤 헤더컬럼설정
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	INT nColInterval = rect.Width()/3;
	
	m_ListCtrl.InsertColumn(0, _T("이름"), LVCFMT_CENTER, nColInterval*1);
	m_ListCtrl.InsertColumn(1, _T("우선순위"), LVCFMT_CENTER, nColInterval*1);
	m_ListCtrl.InsertColumn(2, _T("정렬방법"), LVCFMT_CENTER, rect.Width()-2*nColInterval);
	return 0;
}

BOOL CDataSortElementDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CDataSortElementDlg::RestructPriority(INT nRow, INT nPriority)
{
	CString strTmpData;
	INT nRowCnt = m_pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(i == nRow) continue;
		if(_ttoi(m_pDM->GetCellData(_T("우선순위"), i).GetBuffer(0)) == nPriority)
		{
			strTmpData.Format(_T("%d"), nPriority+1);
			m_pDM->SetCellData(_T("우선순위"), strTmpData, i);
			m_pGrid->SetTextMatrix(i+1, 3, strTmpData);
			return RestructPriority(i, nPriority+1);
		}
	}

	return 0;
}

HBRUSH CDataSortElementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
