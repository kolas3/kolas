// RCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "RCDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCDlg dialog


CRCDlg::CRCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = new CESL_Grid(NULL);

	m_strConfFilePath = _T("..\\cfg\\favorite.cfg");

	m_nWindowHeight = 0;

	m_bIsMinimize = FALSE;
	m_bISMoveWindow = FALSE;

	m_bmpTop.LoadBMP(_T("..\\bmp\\mark-hback.bmp"));
	m_bmpBottom.LoadBMP(_T("..\\bmp\\mark-eback.bmp"));

	m_ListFAVORITE.RemoveAll();
	m_pMenu = NULL;
}

CRCDlg::~CRCDlg()
{
	if (m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

	INT nCount = m_listNewButton.GetCount();
	if (nCount == 0) return;
	CNewButton *pNewButton;
	for (INT i = 0; i < nCount; i++) 
	{
		pNewButton = NULL;
		pNewButton = (CNewButton*)m_listNewButton.RemoveHead();
		if (pNewButton) delete pNewButton;
	}
	m_listNewButton.RemoveAll();
	m_TrayIcon.RemoveIcon();
	FreeFAVORITEList();
	FreeMenu();
}

VOID CRCDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CRCDlg, CDialog)
	//{{AFX_MSG_MAP(CRCDlg)
	ON_BN_CLICKED(IDC_bADD_FAVORITE, OnbADDFAVORITE)
	ON_BN_CLICKED(IDC_bDELETE_FAVORITE, OnbDELETEFAVORITE)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bMAXIMIZE, OnbMAXIMIZE)
	ON_BN_CLICKED(IDC_bMINIMIZE, OnbMINIMIZE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(RC_TRAY_ICON_NOTIFY, OnTrayIconNotify)
	ON_COMMAND_RANGE(4000,4001,OnUserMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCDlg Operations

INT CRCDlg::Display()
{
	EFS_BEGIN

	CRect rcWnd;
	GetWindowRect(rcWnd);

	// Button
	if (m_bIsMinimize)
	{
		CRect rcTitle;
		GetDlgItem(IDC_STATIC_MAIN_TITLE)->GetWindowRect(rcTitle);
		rcWnd.bottom = rcWnd.top + rcTitle.Height();
		MoveWindow(rcWnd);

		GetDlgItem(IDC_bMINIMIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bMAXIMIZE)->ShowWindow(SW_SHOW);
	}
	else 
	{
		
		rcWnd.bottom = rcWnd.top + m_nWindowHeight;
		MoveWindow(rcWnd);

		GetDlgItem(IDC_bMINIMIZE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bMAXIMIZE)->ShowWindow(SW_HIDE);
	}

	// Grid
	if (m_pGrid == NULL) return -1;

	m_pGrid->Clear();
	m_pGrid->SetCols(0, 3);
	m_pGrid->SetColumnInfo(0, _T("메뉴 Alias"), 80, 4);
	m_pGrid->SetColumnInfo(1, _T("메뉴 제목"), 205, 1);
	m_pGrid->SetColWidth(1, 0, 0);
	m_pGrid->SetColWidth(0, 0, 0);
	
	INT nCount = m_ListFAVORITE.GetCount();
	if (nCount <= 0) return -1;

	m_pGrid->SetRows(nCount + 1);
	m_pGrid->SetReverse(0);

	CString strIdx;

	INT idx, nRow = 0;
	FAVORITE *pFavorite;
	POSITION pos = m_ListFAVORITE.GetHeadPosition();
	for (idx = 0; idx < nCount; idx++)
	{
		pFavorite = NULL;
		pFavorite = (FAVORITE*)m_ListFAVORITE.GetNext(pos);
		if (pFavorite == NULL) continue;
		strIdx.Format(_T("%d"), nRow + 1);

		m_pGrid->SetTextMatrix(nRow + 1, 0, strIdx);
		m_pGrid->SetTextMatrix(nRow + 1, 1, pFavorite->MENU_ALIAS);
		m_pGrid->SetTextMatrix(nRow + 1, 2, pFavorite->MENU_TITLE);
		m_pGrid->SetRowHeight(nRow+1, 500);

		nRow++;
	}
	return 0;


	EFS_END
	return -1;
}

INT CRCDlg::ExecuteMenu()
{
	EFS_BEGIN

	if (m_pParentView == NULL) return -1;

	INT nRow = m_pGrid->GetRow();

	if (nRow < 1) return -1;
	CString str;
	str = m_pGrid->GetTextMatrix(nRow, 1);

	//GetParentFrame()->GetActiveView()->SendMessage(RC_SHOW, 0, (LPARAM)&str);
	if ( m_pParentView->GetSafeHwnd() != NULL ) m_pParentView->SendMessage(RC_SHOW, 0, (LPARAM)&str);

	return 0;

	EFS_END
	return -1;
}

INT CRCDlg::FreeFAVORITEList()
{
	INT nCount = m_ListFAVORITE.GetCount();
	if (nCount == 0) return 0;
	FAVORITE *pFavorite;
	for (INT i = 0; i < nCount; i++) 
	{
		pFavorite = NULL;
		pFavorite = (FAVORITE*)m_ListFAVORITE.RemoveHead();
		if (pFavorite) delete pFavorite;
	}
	m_ListFAVORITE.RemoveAll();
	return 0;
}

INT CRCDlg::LoadFAVORITEList()
{
EFS_BEGIN
	FreeFAVORITEList();
	CStdioFile file;
	if (!file.Open(m_strConfFilePath, CFile::modeRead|CFile::typeBinary))
		return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	INT ids = -1;
	CString strLine;

	INT nCutPos;
	FAVORITE *pFavorite;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		if (strLine.GetLength() == 0) continue;
		if (strLine.Left(1) == _T("#") || strLine.Left(1) == _T(".")) continue;

		pFavorite = NULL;
		pFavorite = new FAVORITE;
		if (pFavorite == NULL) continue;
		
		nCutPos = strLine.Find(':', 0);
		if (nCutPos == -1) continue;
		pFavorite->MENU_ALIAS = strLine.Mid(0, nCutPos);
		pFavorite->MENU_ALIAS.TrimLeft();		pFavorite->MENU_ALIAS.TrimRight();
		pFavorite->MENU_TITLE = strLine.Mid(nCutPos+1);
		pFavorite->MENU_TITLE.TrimLeft();		pFavorite->MENU_TITLE.TrimRight();
		
		m_ListFAVORITE.AddTail((CObject*)pFavorite);
	}

	file.Close();
	return 0;
EFS_END
return -1;
}

INT CRCDlg::SaveFAVORITEList()
{
EFS_BEGIN
	CStdioFile file;
if (!file.Open(m_strConfFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	

	CString strLine;
	INT nCount = m_ListFAVORITE.GetCount();
	FAVORITE *pFavorite;
	POSITION pos = m_ListFAVORITE.GetHeadPosition();
	for (INT i = 0; i < nCount; i++) 
	{
		pFavorite = NULL;
		pFavorite = (FAVORITE*)m_ListFAVORITE.GetNext(pos);
		if (pFavorite == NULL) continue;
		strLine.Format(_T("%s\t:\t%s\r\n"), pFavorite->MENU_ALIAS, pFavorite->MENU_TITLE);
		file.WriteString(strLine);
	}
	file.Close();
	return 0;
EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CRCDlg message handlers

BOOL CRCDlg::Create(CWnd* pParentWnd, CView *pParentView) 
{
	EFS_BEGIN

	m_pParentView = pParentView;

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

BOOL CRCDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	GetWindowRect(m_rcWnd);
	m_nWindowHeight = m_rcWnd.Height();
	
	// Subclass Grid
	if (!m_pGrid->SubclassDlgItem(IDC_gFAVORITE, this))
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Subclass Button
	CNewButton *pNewButton = new CNewButton;
	pNewButton->SubclassDlgItem(IDC_bMINIMIZE, this);
	pNewButton->LoadBitmap(_T("..\\bmp\\mark-up.bmp"), _T("..\\bmp\\mark-up-clic.bmp"), _T("..\\bmp\\mark-up.bmp"));
	m_listNewButton.AddTail((CObject*)pNewButton);

	pNewButton = new CNewButton;
	pNewButton->SubclassDlgItem(IDC_bMAXIMIZE, this);
	pNewButton->LoadBitmap(_T("..\\bmp\\mark-down.bmp"), _T("..\\bmp\\mark-down-clic.bmp"), _T("..\\bmp\\mark-down.bmp"));
	m_listNewButton.AddTail((CObject*)pNewButton);

	pNewButton = new CNewButton;
	pNewButton->SubclassDlgItem(IDC_bCLOSE, this);
	pNewButton->LoadBitmap(_T("..\\bmp\\mark-exit.bmp"), _T("..\\bmp\\mark-exit-clic.bmp"), _T("..\\bmp\\mark-exit.bmp"));
	m_listNewButton.AddTail((CObject*)pNewButton);

	pNewButton = new CNewButton;
	pNewButton->SubclassDlgItem(IDC_bADD_FAVORITE, this);
	pNewButton->LoadBitmap(_T("..\\bmp\\mark-add-off.bmp"), _T("..\\bmp\\mark-add-on.bmp"), _T("..\\bmp\\mark-add-off.bmp"));
	m_listNewButton.AddTail((CObject*)pNewButton);

	pNewButton = new CNewButton;
	pNewButton->SubclassDlgItem(IDC_bDELETE_FAVORITE, this);
	pNewButton->LoadBitmap(_T("..\\bmp\\mark-del-off.bmp"), _T("..\\bmp\\mark-del-on.bmp"), _T("..\\bmp\\mark-del-off.bmp"));
	m_listNewButton.AddTail((CObject*)pNewButton);

	LoadFAVORITEList();

	Display();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}


BEGIN_EVENTSINK_MAP(CRCDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRCDlg)
	ON_EVENT(CRCDlg, IDC_gFAVORITE, -605 /* MouseDown */, OnMouseDowngFAVORITE, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CRCDlg, IDC_gFAVORITE, -601 /* DblClick */, OnDblClickgFAVORITE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CRCDlg::OnbADDFAVORITE() 
{
	EFS_BEGIN

	if (m_pParentView == NULL) return;

	CString strMenuAlias;
	CString strMenuTitle;

	//LRESULT rt = GetParentFrame()->GetActiveView()->SendMessage(RC_GET_CUR_MENU, (WPARAM)&strMenuAlias, (LPARAM)&strMenuCaption);
	if ( m_pParentView->GetSafeHwnd() != NULL ) 
	{
		LRESULT rt = m_pParentView->SendMessage(RC_GET_CUR_MENU, (WPARAM)&strMenuAlias, (LPARAM)&strMenuTitle);
		if (rt == -1) return;
	}
		
	if (strMenuAlias.IsEmpty(), strMenuTitle.IsEmpty()) return;
	FAVORITE *pFavorite = NULL;
	INT IsDuplicated = FindByMenuAlias(strMenuAlias);
	if (IsDuplicated == -1) {
		pFavorite = new FAVORITE;
	} else {
		POSITION pos = m_ListFAVORITE.FindIndex(IsDuplicated);
		if (pos == NULL) return;
		pFavorite = (FAVORITE*)m_ListFAVORITE.GetAt(pos);
	}
	if (pFavorite == NULL) return;

	pFavorite->MENU_ALIAS = strMenuAlias;
	pFavorite->MENU_TITLE = strMenuTitle;	
	if (IsDuplicated == -1) m_ListFAVORITE.AddTail((CObject*)pFavorite);

	Display();
	SaveFAVORITEList();

	EFS_END
}

VOID CRCDlg::OnbDELETEFAVORITE() 
{
EFS_BEGIN
	INT nRow = m_pGrid->GetRow();
	if (nRow < 1) return;
	if (m_ListFAVORITE.GetCount() == 0) return;

	INT nIdx = nRow-1;
	if (m_ListFAVORITE.GetCount() <= nIdx) return;
	POSITION pos = m_ListFAVORITE.FindIndex(nIdx);
	if (pos == NULL) return;
	FAVORITE *pFavorite = (FAVORITE*)m_ListFAVORITE.GetAt(pos);
	if (pFavorite) {
		delete pFavorite;
		pFavorite = NULL;
	}
	m_ListFAVORITE.RemoveAt(pos);

	Display();
	SaveFAVORITEList();
EFS_END
}

VOID CRCDlg::OnMouseDowngFAVORITE(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	m_pGrid->SetReverse(m_pGrid->GetRow() -1);	

	if (Button == 2) {		// 오른쪽 버튼일 경우
		INT nCount = m_ListFAVORITE.GetCount();
		if (nCount == 0) return;
		if (m_pGrid == NULL) return;
		if (m_pGrid->GetMouseRow() == 0) return;
		if (m_pGrid->GetRow() == 0) return;
		INT nRow = m_pGrid->GetRow()-1;
		FreeMenu();
		m_pMenu = new CMenu;
		if (m_pMenu) {
			m_pMenu->CreatePopupMenu();
			if (nRow > 0) m_pMenu->AppendMenu( MF_STRING , 4000 , _T("위로 이동") );
			if (nRow < nCount-1) m_pMenu->AppendMenu( MF_STRING , 4001 , _T("아래로 이동") );
				
			CPoint pos;
			GetCursorPos(&pos);
			SetForegroundWindow();
			m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}
	}
	EFS_END
}

VOID CRCDlg::OnDblClickgFAVORITE() 
{
	EFS_BEGIN

	ExecuteMenu();	

	EFS_END
}

VOID CRCDlg::OnClose() 
{
	EFS_BEGIN

	SaveFAVORITEList();
	
	CDialog::OnClose();

	EFS_END
}

VOID CRCDlg::OnbMAXIMIZE() 
{
	EFS_BEGIN

	m_bIsMinimize = FALSE;
	Display();
	

	EFS_END
}

VOID CRCDlg::OnbMINIMIZE() 
{
	EFS_BEGIN

	m_bIsMinimize = TRUE;
	Display();	

	EFS_END
}

VOID CRCDlg::OnbCLOSE() 
{
	EFS_BEGIN

	ShowWindow(SW_HIDE);

	m_TrayIcon.ShowIcon();

	EFS_END
}

VOID CRCDlg::OnPaint() 
{
	EFS_BEGIN

	CPaintDC dc(this); // device context for painting
	
	CRect rcWnd;
	GetClientRect(rcWnd);

	m_bmpTop.DrawDIB(&dc, 0, 0);

	if (!m_bIsMinimize)
		m_bmpBottom.DrawDIB(&dc, 0, rcWnd.bottom - 45);
	
	// Do not call CDialog::OnPaint() for painting messages

	EFS_END
}

VOID CRCDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_MAIN_TITLE)->GetWindowRect(rcTitle);
	ScreenToClient(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		m_bISMoveWindow = TRUE;
		SetCapture();
		GetCursorPos(&m_CusorPoint);
		GetWindowRect(m_rcWnd);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
EFS_END
}

VOID CRCDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bISMoveWindow)
	{
		m_bISMoveWindow = FALSE;
		ReleaseCapture();
	}
	
	CDialog::OnLButtonUp(nFlags, point);

	EFS_END
}

VOID CRCDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bISMoveWindow)
	{
		CPoint point;
		GetCursorPos(&point);

		INT x = point.x - m_CusorPoint.x;
		INT y = point.y - m_CusorPoint.y;


		SetWindowPos(NULL, m_rcWnd.left + x, m_rcWnd.top + y, 1, 1, SWP_NOSIZE | SWP_NOZORDER);

		GetWindowRect(m_rcWnd);
		m_CusorPoint = point;

		return;
	}
	
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}

INT CRCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	EFS_BEGIN

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_pParentView == NULL) return -1;

	SetWindowText(_T("즐겨찾기"));
	UINT nID;
	nID = IDR_MAINFRAME;
	SetIcon(::AfxGetApp()->LoadIcon(nID), FALSE);

	if (!m_TrayIcon.Create(this, RC_TRAY_ICON_NOTIFY, _T("KOLAS2UP 즐겨찾기"), NULL, nID))
		return -1;

	m_TrayIcon.SetIcon(nID);
	m_TrayIcon.HideIcon();

	return 0;

	EFS_END
	return -1;
}

LRESULT CRCDlg::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
	{
		ShowWindow(SW_SHOW);
		m_TrayIcon.HideIcon();
	}


	return 1;

	EFS_END
	return NULL;
}

BOOL CRCDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) {
		if (::GetKeyState(VK_MENU) < 0) return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CRCDlg::FreeMenu(VOID)
{
	if (m_pMenu == NULL) return;
	delete m_pMenu;
	m_pMenu = NULL;
	return;
}

VOID CRCDlg::OnUserMenu( UINT  nIDC )
{
	if (m_pGrid == NULL) return;
	INT nRow = m_pGrid->GetRow()-1;
	if (nIDC == 4000) {			// 위로
		SwapMenuIdx(nRow, -1);
	}
	if (nIDC == 4001) {			// 아래로
		SwapMenuIdx(nRow, +1);
	}
	return;
}

// nDirection == -1 : 위로
// nDirection == 1 : 아래로
VOID CRCDlg::SwapMenuIdx(INT nRow, INT nDirection)
{
	INT nRows = m_ListFAVORITE.GetCount();
	if (nRows == 0) return;
	
	INT nTargetRow = nRow + nDirection;
	if (nRow < 0 || nRow >= nRows) return;
	if (nTargetRow < 0 || nTargetRow >= nRows) return;

	POSITION pos = m_ListFAVORITE.FindIndex(nRow);
	POSITION posTarget = m_ListFAVORITE.FindIndex(nTargetRow);
	if (pos == NULL || posTarget == NULL) return;

	FAVORITE *pFavorite = (FAVORITE *)m_ListFAVORITE.GetAt(pos);
	FAVORITE *pFavoriteTarget = (FAVORITE *)m_ListFAVORITE.GetAt(posTarget);
	if (pFavorite == NULL || pFavoriteTarget == NULL) return;

	m_ListFAVORITE.SetAt(pos, (CObject*)pFavoriteTarget);
	m_ListFAVORITE.SetAt(posTarget, (CObject*)pFavorite);

	SaveFAVORITEList();
	Display();
	return;
}

INT CRCDlg::FindByMenuAlias(CString strMenuAlias)
{
EFS_BEGIN
	INT nCount = m_ListFAVORITE.GetCount();
	if (nCount <= 0) return -1;

	FAVORITE *pFavorite;
	POSITION pos = m_ListFAVORITE.GetHeadPosition();
	for (INT i = 0; i < nCount; i++)
	{
		pFavorite = NULL;
		pFavorite = (FAVORITE*)m_ListFAVORITE.GetNext(pos);
		if (pFavorite == NULL) continue;

		if (pFavorite->MENU_ALIAS == strMenuAlias) return i;
	}
	return -1;
EFS_END
return -1;
}
