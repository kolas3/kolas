// MenuBar.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "MenuBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuBar dialog


CMenuBar::CMenuBar(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMenuBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_pView = NULL;
	m_MenuTreeBar = NULL;
}
CMenuBar::~CMenuBar()
{
	if ( m_MenuTreeBar != NULL ) delete m_MenuTreeBar;
}

void CMenuBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_btnSearch, m_btnSearch);
	DDX_Control(pDX, IDC_bLock, m_btnLock);
}


BEGIN_MESSAGE_MAP(CMenuBar, CDialog)
	//{{AFX_MSG_MAP(CMenuBar)
	ON_BN_CLICKED(IDC_btnSearch, OnbtnSearch)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_bLock, OnbLock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBar message handlers

BOOL CMenuBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_pParent = pParentWnd;
	
	return CDialog::Create(IDD, pParentWnd);
}

void CMenuBar::OnbtnSearch() 
{
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1020, NULL, NULL);	
}

BOOL CMenuBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnSearch.LoadBitmaps("..\\BMP\\IeSearch.bmp", RGB(0,0,0));
	m_btnLock.LoadBitmaps("..\\BMP\\lock.bmp", RGB(0,0,0));
	m_Dlg_brush.CreateSolidBrush(RGB(246, 246, 246)); 	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CMenuBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC ) 
	{		
		//pDC->SetTextColor(RGB(0,0,0));		
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(243,243,243));		
		
		return m_Dlg_brush;		
	}

	return hbr;
}

void CMenuBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CButton *pBtn = ( (CButton*) GetDlgItem (IDC_btnSearch) );
	if ( pBtn->GetSafeHwnd() )
	{
		CRect rect;
		pBtn->GetClientRect(rect);		
		ClientToScreen(rect);
		rect.top = 3;
		rect.bottom = 26;
		rect.left = cx - ( rect.right - rect.left)-5;
		rect.right = cx -5;
		pBtn->MoveWindow(rect);	
		
		rect.top += 5;
		rect.bottom -= 4;
		rect.left -= 30;
		rect.right -= 20;
		( GetDlgItem (IDC_COUNT_STATIC) )->MoveWindow(rect);
		if ( m_pView == NULL )
		{
			m_pView = m_pFrameWnd->GetActiveView();
		}

		if ( m_MenuTreeBar->GetSafeHwnd() )
		{
			CRect MenuRect(40,0,cx-60,29);
			m_MenuTreeBar->MoveWindow(MenuRect);
		}
	}	
}

void CMenuBar::SetCount(CString strCount)
{	
	m_strCount = strCount;

	CRect rec;
	( GetDlgItem (IDC_COUNT_STATIC) )->GetWindowRect(&rec);
	ScreenToClient(rec);
	InvalidateRect(rec, TRUE);
	( GetDlgItem (IDC_COUNT_STATIC) )->SetWindowText(m_strCount);	
}

VOID CMenuBar::SetMenuDir(CString strMenuID)
{
	//( (CStatic*) GetDlgItem (IDC_sMENU) )->SetWindowText(strMenuDir);
	if ( m_MenuTreeBar != NULL ) 
	{
		m_MenuTreeBar->MakeLineMenu( _ttoi(strMenuID) , TRUE, TRUE );		
	}
}

VOID CMenuBar::FillBackground()
{
	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	
	TRIVERTEX data1[2];
	COLORREF TopRectRGB1 = RGB(252, 253, 253);
	COLORREF TopRectRGB2 = RGB(231, 231, 231);	

	data1[0].x = rect.left;	
	data1[0].y = rect.top+1;		
	data1[0].Red   = GetRValue(TopRectRGB1) * 255;
	data1[0].Green = GetGValue(TopRectRGB1) * 255;
	data1[0].Blue  = GetBValue(TopRectRGB1) * 255;
	data1[0].Alpha = 0x0000;

	data1[1].Red   = GetRValue(TopRectRGB2) * 255;
	data1[1].Green = GetGValue(TopRectRGB2) * 255;
	data1[1].Blue  = GetBValue(TopRectRGB2) * 255;
	data1[1].Alpha = 0x0000;
	data1[1].x  = rect.right;
	data1[1].y  = rect.bottom;

	GRADIENT_RECT draw_rect1;
	draw_rect1.UpperLeft  = 0;
	draw_rect1.LowerRight = 1;		

	::GradientFill(pDC->m_hDC, data1, 2, &draw_rect1, 1, GRADIENT_FILL_RECT_V);

	ReleaseDC(pDC);
}

BOOL CMenuBar::OnEraseBkgnd(CDC* pDC) 
{		
	FillBackground();	
	( GetDlgItem (IDC_COUNT_STATIC) )->SetWindowText(m_strCount);	
 	Invalidate(FALSE);
	return TRUE;
}

VOID CMenuBar::OnbLock() 
{
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+99, NULL, NULL);		
}

VOID CMenuBar::SetFrameWnd(CFrameWnd* pFrameWnd)
{
	m_pFrameWnd = pFrameWnd;	
}

VOID CMenuBar::CreateMenuTreeBar(CObList *pMenuItemList)
{	
	INT nItemCount = pMenuItemList->GetCount();
	m_MenuTreeBar = new CMenuBarDlg(nItemCount, m_pParent);			
	m_MenuTreeBar->InputDate(pMenuItemList);
	CRect rect;
	GetWindowRect(rect);
	CRect MenuRect(40,0,rect.right-rect.left-80,29);
	m_MenuTreeBar->Create(_T(""), _T(""), WS_CHILD|WS_VISIBLE, MenuRect, this, 0 );
	m_MenuTreeBar->MoveWindow(MenuRect);
	m_MenuTreeBar->MakeLineMenu(1000);
	m_MenuTreeBar->ShowWindow(SW_SHOW);
}

VOID CMenuBar::SetParent(CWnd* pWnd)
{
	m_pParent = pWnd;
}

BOOL CMenuBar::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMenuBar::OnPaint() 
{	
	CDialog::OnPaint();	
}