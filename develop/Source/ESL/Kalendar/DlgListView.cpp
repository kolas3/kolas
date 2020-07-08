// DlgListView.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "DlgListView.h"
#include "AlarmTime.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgListView dialog


CDlgListView::CDlgListView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgListView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgListView)
	//}}AFX_DATA_INIT
	m_uYear = 0;
	m_uMonth = 0;

}


void CDlgListView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgListView)
	DDX_Control(pDX, IDC_STATIC_YEAR, m_StaticYear);
	DDX_Control(pDX, IDC_EDIT_YEAR, m_EditSearchYear);
	DDX_Control(pDX, IDC_EDIT_SEARCH_TEXT, m_EditSearchText);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Control(pDX, IDC_STATIC_DATE, m_StaticDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgListView, CDialog)
	//{{AFX_MSG_MAP(CDlgListView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgListView message handlers

void CDlgListView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	/*
	m_FileMgr.SearchTextAndAddResultInSearchMemoList(2008, 2, _T("아"));
	int Max = m_FileMgr.GetSearchMemoListCount();
	UINT uYear, uMonth, uDay;
	CMemo* pMemo;

	m_HTMLListCtrl.DeleteAllItems();
	for(int i=0;i<Max;i++)
	{
		pMemo = m_FileMgr.GetSearchMemoListAt(i);
		pMemo->GetDate(&uYear, &uMonth, &uDay);
		AddList(uYear, uMonth, uDay, pMemo->GetText());
	}
	*/
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgListView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	this->ShowWindow(false);
//	CDialog::OnClose();
}

void CDlgListView::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(false);
//	CDialog::OnOK();
}

void CDlgListView::OnCancel() 
{
	// TODO: Add extra cleanup here	
	this->ShowWindow(false);
//	CDialog::OnCancel();
}

BOOL CDlgListView::OnInitDialog() 
{
	CDialog::OnInitDialog();		
	// TODO: Add extra initialization here	
	InitBG();
	CTime t = CTime::GetCurrentTime();
	CString stYear;
	stYear.Format(_T("%d"), t.GetYear());
	m_EditSearchYear.SetWindowText(stYear);

	InitHTMLListCtrl();
	m_btnSearch.LoadBitmaps(IDB_BITMAP_SEARCH);	
	m_btnClose.LoadBitmaps(IDB_BITMAP_BUTTON_CLOSE);


	CRect rDlg;
	GetWindowRect(rDlg);
	ScreenToClient(rDlg);

	LPPOINT lpPoint = new CPoint[8];
//DEL 	{ 
	lpPoint[0] = CPoint(0, 2);
 	lpPoint[1] = CPoint(2, 0);
 	lpPoint[2] = CPoint(rDlg.right -2, 0);
 	lpPoint[3] = CPoint(rDlg.right, 2);
 	lpPoint[4] = CPoint(rDlg.right, rDlg.bottom -3);
 	lpPoint[5] = CPoint(rDlg.right-3, rDlg.bottom);
 	lpPoint[6] = CPoint(3, rDlg.bottom);
 	lpPoint[7] = CPoint(0, rDlg.bottom-3);
//DEL 	};
	CRgn Rgn;
 	Rgn.CreatePolygonRgn(lpPoint, 8, WINDING);
	delete [] lpPoint;

	SetWindowRgn(Rgn, TRUE);
	Rgn.DeleteObject();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//DEL void CDlgListView::AddList(CString stDay, CString stText)
//DEL {	
//DEL 	CString stInsertText;
//DEL 	stInsertText = stDay + '\n' + stText;
//DEL 	m_HTMLListCtrl.InsertItem(stInsertText, 0, NORMAL_TEXT);
//DEL }


void CDlgListView::ResetList()
{	
	m_HTMLListCtrl.DeleteAllItems();	
	m_FileMgr.ResetSearchMemoList();
	m_uYear = 0;
	m_uMonth = 0;
	m_StaticDate.ShowWindow(false);
}

void CDlgListView::InitHTMLListCtrl()
{
	m_HTMLListCtrl.Create(this, CRect( 8, 36, 292, 490), IDC_LIST_MEMO);
	m_HTMLListCtrl.ModifyStyle(WS_BORDER, NULL);
	m_HTMLListCtrl.SetExtendedStyle(HTMLLIST_STYLE_GRIDLINES);
}

void CDlgListView::SetDate(UINT uYear, UINT uMonth)
{
	CString st;
	st.Format(_T("%d년 %d월"), uYear, uMonth);
	m_StaticDate.SetWindowText(st);
	m_StaticDate.ShowWindow(true);
	m_uYear = uYear;
	m_uMonth = uMonth;
}

// CHTMLList에서 클릭된 아이템이 넘어온다.
BOOL CDlgListView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	NM_HTMLLISTCTRL* pList = (NM_HTMLLISTCTRL *)lParam;	

	if(-1==pList->nItemNo)
		return FALSE;
	UINT uYear, uMonth, uDay;
	GetDateFromString(pList->sItemText, &uYear, &uMonth, &uDay);	

	CData* pData = new CData;
	pData->SetData(uYear, uMonth, uDay, _T(""));
	GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
	delete pData;

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgListView::AddList(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	CString stInsert = GetInsertText(uYear, uMonth, uDay, stText);
	m_HTMLListCtrl.InsertItem(stInsert, 0,NORMAL_TEXT);			
}

void CDlgListView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nFlags & MK_LBUTTON ) // L 마우스 버튼이 눌러져 있구나..
	{
		this->PostMessage(
			WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		return;				
	}	
	CDialog::OnMouseMove(nFlags, point);
}

HBRUSH CDlgListView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,255));
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		return (HBRUSH)::GetStockObject (NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgListView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
	CRect rect; 
	GetClientRect( &rect );	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, 0, 0, SRCCOPY);				
	return TRUE;
}

void CDlgListView::InvalidateHTMLList(BOOL bErase)
{
	m_HTMLListCtrl.Invalidate(bErase);
}

BOOL CDlgListView::IsSameDate(UINT uYear, UINT uMonth)
{
	if(m_uYear != uYear)
		return FALSE;
	if(m_uMonth != uMonth)
		return FALSE;
	return TRUE;
}



BOOL CDlgListView::ModifyItem(UINT uYear, UINT uMonth, UINT uDay, CString stModifyText)
{
	CString st;
	UINT uGetYear=0, uGetMonth=0, uGetDay=0;
	int iModifyItemNo = -1;
	for(int i=0;i<m_HTMLListCtrl.GetItemCount();i++)
	{
		st = m_HTMLListCtrl.GetItemText(i);
		GetDateFromString(st, &uGetYear, &uGetMonth, &uGetDay);	
		
		if(uGetYear == uYear)
			if(uGetMonth == uMonth)
				if(uGetDay == uDay)
				{
					iModifyItemNo = i;
					break;
				}
	}	
	if(-1 == iModifyItemNo)
		return FALSE;
	CString stText = GetInsertText(uYear, uMonth, uDay, stModifyText);
	m_HTMLListCtrl.SetItemText(iModifyItemNo, stText, TRUE);	
//	InvalidateHTMLList(TRUE);
	return TRUE;
}

void CDlgListView::GetDateFromString(CString stDate, UINT* uYear, UINT* uMonth, UINT* uDay)
{	
	CString stTemp = stDate;
	*uYear = CAlarmTime::GetConvertStringToUINT(stTemp, 4);
	stTemp = stTemp.Mid(5);
	*uMonth = CAlarmTime::GetConvertStringToUINT(stTemp, 2);
	stTemp = stTemp.Mid(3);
	*uDay = CAlarmTime::GetConvertStringToUINT(stTemp, 2);
}

CString CDlgListView::GetInsertText(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{	
	CString stInsert, stYear, stMonth, stDay;
	stYear.Format(_T("%d"), uYear);
	stMonth.Format(_T("%d"), uMonth);
	stDay.Format(_T("%d"), uDay);
	
	if(10>uMonth)
		stMonth = '0' + stMonth;
	if(10>uDay)
		stDay = '0' + stDay;
	
	stInsert = stYear + '/' + stMonth + '/' + stDay;
	
	if(100<stText.GetLength())
		stText = stText.Left(100) + "...";

	stInsert = stInsert + _T("\n") + stText;
	return stInsert;
}

// 검색버튼 눌렀을 시
void CDlgListView::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	ResetList();
	CString stSearchText;
	CString stSearchYear;
	UINT uSearchYear;
	m_EditSearchText.GetWindowText(stSearchText);
	m_EditSearchYear.GetWindowText(stSearchYear);
	uSearchYear = CAlarmTime::GetConvertStringToUINT(stSearchYear, 4);
	m_FileMgr.SearchTextAndAddResultInSearchMemoList(uSearchYear, stSearchText);
	int Max = m_FileMgr.GetSearchMemoListCount();
	UINT uYear, uMonth, uDay;
	CMemo* pMemo;
	m_HTMLListCtrl.DeleteAllItems();
	for(int i=0;i<Max;i++)
	{
		pMemo = m_FileMgr.GetSearchMemoListAt(i);
		pMemo->GetDate(&uYear, &uMonth, &uDay);
		AddList(uYear, uMonth, uDay, pMemo->GetText());
	}
	m_HTMLListCtrl.Invalidate(FALSE);
}

// 배경이미지 초기화
void CDlgListView::InitBG()
{
	CDC* pDC = GetDC();
	m_BGDC.CreateCompatibleDC(pDC);		
	m_BGBitmap.LoadBitmap(IDB_BITMAP_LIST_BACKIMAGE);
	m_BGDC.SelectObject(&m_BGBitmap);
	ReleaseDC(pDC);
}
