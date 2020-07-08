// Kolas2up_ImageEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kolas2up_ImageEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Kolas2up_ImageEditorResizeDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorDlg dialog


CKolas2up_ImageEditorDlg::CKolas2up_ImageEditorDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKolas2up_ImageEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKolas2up_ImageEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pView = NULL;
	m_nCropWidth = 100;
	m_nCropHeight = 100;

	m_nLineThick = 1;
	m_cLineColor = RGB(0, 0, 255);

	m_fZoomRatio = 110;
}

CKolas2up_ImageEditorDlg::~CKolas2up_ImageEditorDlg()
{
	
}



VOID CKolas2up_ImageEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKolas2up_ImageEditorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKolas2up_ImageEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CKolas2up_ImageEditorDlg)
	ON_BN_CLICKED(IDC_bRESIZE, OnbRESIZE)
	ON_BN_CLICKED(IDC_bCROP, OnbCROP)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bZOOMIN, OnbZOOMIN)
	ON_BN_CLICKED(IDC_bZOOMOUT, OnbZOOMOUT)
	ON_BN_CLICKED(IDC_bRELOAD, OnbRELOAD)
	ON_BN_CLICKED(IDC_bUNDO, OnbUNDO)
	ON_WM_MOUSEWHEEL()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+32000, OnShowPopupMenu)
	ON_COMMAND_RANGE(6000, 6005, PopupMenuProc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorDlg message handlers

BOOL CKolas2up_ImageEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 비트맵 버튼 초기화
	m_BitmapButtonList.Add(IDC_bRESIZE, this, _T("..\\BMP\\image_btn01_off.bmp"), _T("..\\BMP\\image_btn01_on.bmp"), _T("..\\BMP\\image_btn01_off.bmp"));
	m_BitmapButtonList.Add(IDC_bZOOMIN, this, _T("..\\BMP\\image_btn02_off.bmp"), _T("..\\BMP\\image_btn02_on.bmp"), _T("..\\BMP\\image_btn02_off.bmp"));
	m_BitmapButtonList.Add(IDC_bZOOMOUT, this, _T("..\\BMP\\image_btn03_off.bmp"), _T("..\\BMP\\image_btn03_on.bmp"), _T("..\\BMP\\image_btn03_off.bmp"));
	m_BitmapButtonList.Add(IDC_bCROP, this, _T("..\\BMP\\image_btn04_off.bmp"), _T("..\\BMP\\image_btn04_on.bmp"), _T("..\\BMP\\image_btn04_off.bmp"));
	m_BitmapButtonList.Add(IDC_bRELOAD, this, _T("..\\BMP\\image_btn05_off.bmp"), _T("..\\BMP\\image_btn05_on.bmp"), _T("..\\BMP\\image_btn05_off.bmp"));
	m_BitmapButtonList.Add(IDC_bUNDO, this, _T("..\\BMP\\image_btn06_off.bmp"), _T("..\\BMP\\image_btn06_on.bmp"), _T("..\\BMP\\image_btn06_off.bmp"));
	m_BitmapButtonList.Add(IDC_bOK, this, _T("..\\BMP\\image_btn07_off.bmp"), _T("..\\BMP\\image_btn07_on.bmp"), _T("..\\BMP\\image_btn07_off.bmp"));
	m_BitmapButtonList.Add(IDC_bCANCEL, this, _T("..\\BMP\\image_btn08_off.bmp"), _T("..\\BMP\\image_btn08_on.bmp"), _T("..\\BMP\\image_btn08_off.bmp"));

	m_pView = new CKolas2up_ImageEditorView;
	m_pView->m_pWnd = this;
	m_mapsize = CSize(1400, 1000);			
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	m_viewrect = CRect( 10,65,510, 355 );
	m_pView->CreateEx( WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, m_viewrect, this, 1, NULL );	
	m_pView->ShowWindow(TRUE);

	m_pView->LoadBitmap(m_strInFileName);
	
	SIZE s = m_pView->GetSize();
	m_mapsize = CSize(s.cx, s.cy);			
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	m_pView->m_nCropWidth = m_nCropWidth;
	m_pView->m_nCropHeight = m_nCropHeight;
	m_pView->m_nLineThick = m_nLineThick;
	m_pView->m_cLineColor = m_cLineColor;

	// 오른쪽버튼 popup 메뉴 초기화
	InitPopupMenu();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKolas2up_ImageEditorDlg::DestroyWindow() 
{
	if (m_pView) {
		delete m_pView;
		m_pView = NULL;
	}

	return CDialog::DestroyWindow();
}


VOID CKolas2up_ImageEditorDlg::OnbRESIZE() 
{
	if (m_pView == NULL) return;
	if (m_pView->m_pBmp == NULL) return;

	SIZE s = m_pView->GetSize();

	CKolas2up_ImageEditorResizeDlg dlg;
	dlg.m_nWidth = s.cx;
	dlg.m_nHeight = s.cy;
	dlg.m_fRatio = (double)s.cy / (double)s.cx;
	dlg.m_strRatio.Format(_T("[비율] 가로 : 세로 = 1 : %g"), dlg.m_fRatio);

	if (dlg.DoModal() != IDOK) return;

	// Undo 자료를 저장한다.
	m_pView->SaveUndoBitmap();
	// Resize 한다.
	RESIZE(dlg.m_nWidth, dlg.m_nHeight);
}

INT CKolas2up_ImageEditorDlg::RESIZE(INT cx, INT cy)
{
	m_pView->Resize(cx, cy);
	SIZE s = m_pView->GetSize();
	m_mapsize = CSize(s.cx, s.cy);			
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	return 0;
}

VOID CKolas2up_ImageEditorDlg::OnbCROP() 
{
	if (m_pView->IsCurrentSel() == FALSE) {
		AfxMessageBox(_T("먼저 영역을 선택하십시오."));
		return;
	}
	// Undo 자료를 저장한다.
	m_pView->SaveUndoBitmap();
	// Crop 한다.
	m_pView->Crop(m_pView->m_rCurrentSel.left, m_pView->m_rCurrentSel.top, m_pView->m_rCurrentSel.right, m_pView->m_rCurrentSel.bottom);	

	SIZE s = m_pView->GetSize();
	m_mapsize = CSize(s.cx, s.cy);			
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
}

VOID CKolas2up_ImageEditorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	RESIZE_Controls();
	
}

VOID CKolas2up_ImageEditorDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RESIZE_Controls();
}

VOID CKolas2up_ImageEditorDlg::RESIZE_Controls(VOID)
{
	if (GetSafeHwnd() == NULL) return;
	CWnd *pWnd = NULL;

	CRect rcDlg, rcControl;
	GetClientRect(rcDlg);

	pWnd = GetDlgItem(IDC_STATIC_BORDER);
	if (pWnd) {
		rcControl = rcDlg;
		rcControl.top = rcControl.top + 5;
		rcControl.left = rcControl.left + 5;
		rcControl.bottom = rcControl.bottom - 5;
		rcControl.right = rcControl.right - 5;
		
		pWnd->MoveWindow(rcControl);
	}
	if (m_pView) {
		if (m_pView->GetSafeHwnd()) {
			rcControl = rcDlg;
			rcControl.top = rcControl.top + 65;
			rcControl.left = rcControl.left + 10;
			rcControl.bottom = rcControl.bottom - 10;
			rcControl.right = rcControl.right - 10;
			
			m_pView->MoveWindow(rcControl);
		}
	}
	return;
}

VOID CKolas2up_ImageEditorDlg::OnbOK() 
{
	OnbSAVE();

	CDialog::OnOK();	
}

VOID CKolas2up_ImageEditorDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

VOID CKolas2up_ImageEditorDlg::OnbSAVE() 
{
	INT nPos = m_strOutFileName.ReverseFind('.');
	CString strExtension = _T("");
	if (nPos != -1) {
		strExtension = m_strOutFileName.Mid(nPos+1);
	}
	strExtension.TrimLeft();		strExtension.TrimRight();
	strExtension.MakeUpper();

	INT ids;
	if (strExtension == _T("JPG") || strExtension == _T("JPEG")) {
		ids = m_pView->SaveJpeg(m_strOutFileName);	
	} else {
		ids = m_pView->SaveBitmap(m_strOutFileName);	
	}
	
	if (ids < 0) {
		AfxMessageBox(_T("데이타를 저장하는데 실패하였습니다."));
	}
}

VOID CKolas2up_ImageEditorDlg::SetCropSize(INT cx, INT cy)
{
	m_nCropWidth = cx;
	m_nCropHeight = cy;
	return;
}

VOID CKolas2up_ImageEditorDlg::SetLineColor(UINT nR, UINT nG, UINT nB)
{
	m_cLineColor = RGB(nR, nG, nB);
	if (m_pView) m_pView->m_cLineColor = m_cLineColor;
	return;
}

VOID CKolas2up_ImageEditorDlg::SetLineThick(UINT nThick)
{
	m_nLineThick = nThick;
	if (m_pView) m_pView->m_nLineThick = m_nLineThick;
	return;
}

VOID CKolas2up_ImageEditorDlg::OnbZOOMIN() 
{
	// 확대	
	SIZE s = m_pView->GetSize();
	SIZE newSize;
	newSize.cx = (INT)(s.cx * (m_fZoomRatio / 100));
	newSize.cy = (INT)(s.cy * (m_fZoomRatio / 100));

	// Undo 자료를 저장한다.
	m_pView->SaveUndoBitmap();
	// 확대(RESIZE) 한다.
	RESIZE(newSize.cx, newSize.cy);
}

VOID CKolas2up_ImageEditorDlg::OnbZOOMOUT() 
{
	// 축소
	SIZE s = m_pView->GetSize();
	SIZE newSize;
	newSize.cx = (INT)(s.cx * (100 / m_fZoomRatio));
	newSize.cy = (INT)(s.cy * (100 / m_fZoomRatio));

	// Undo 자료를 저장한다.
	m_pView->SaveUndoBitmap();
	// 축소(RESIZE) 한다.
	RESIZE(newSize.cx, newSize.cy);
}

INT CKolas2up_ImageEditorDlg::SetZoomRatio(double fZoomRatio)
{
	if (fZoomRatio <= 100) return -1;
	m_fZoomRatio = fZoomRatio;

	return 0;
}

VOID CKolas2up_ImageEditorDlg::OnbRELOAD() 
{
	m_pView->LoadBitmap(m_strInFileName);
	
	m_pView->Invalidate(TRUE);
}

VOID CKolas2up_ImageEditorDlg::OnbUNDO() 
{
	m_pView->LoadUndoBitmap();		
}

BOOL CKolas2up_ImageEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == 'Z' ) {
		OnbUNDO();
		return TRUE;		
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKolas2up_ImageEditorDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	m_pView->OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
	// return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

VOID CKolas2up_ImageEditorDlg::InitPopupMenu(VOID)
{
	m_Menu.CreatePopupMenu();
	m_Menu.AppendMenu(MF_STRING | MF_ENABLED, 6000, _T("크기변경"));
	m_Menu.AppendMenu(MF_STRING | MF_ENABLED, 6001, _T("자르기"));
	m_Menu.AppendMenu(MF_STRING | MF_ENABLED, 6002, _T("확대"));
	m_Menu.AppendMenu(MF_STRING | MF_ENABLED, 6003, _T("축소"));
	m_Menu.AppendMenu(MF_SEPARATOR);
	m_Menu.AppendMenu(MF_STRING | MF_ENABLED, 6004, _T("되돌리기"));
	m_Menu.AppendMenu(MF_SEPARATOR);
	m_Menu.AppendMenu(MF_STRING | MF_ENABLED, 6005, _T("원상태로복원"));
	return;
}

VOID CKolas2up_ImageEditorDlg::OnShowPopupMenu(VOID)
{
	if (m_pView == NULL) return;

	CRect rect;
	m_pView->GetWindowRect(&rect);
	m_Menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + m_pView->m_point.x, rect.top + m_pView->m_point.y, this);

	return;
}

VOID CKolas2up_ImageEditorDlg::PopupMenuProc(UINT  nIDC)
{
	if (nIDC == 6000) {	// 크기변경
		OnbRESIZE();
	}
	if (nIDC == 6001) {	// 자르기
		OnbCROP();
	}
	if (nIDC == 6002) {	// 확대
		OnbZOOMIN();
	}
	if (nIDC == 6003) {	// 축소
		OnbZOOMOUT();
	}
	if (nIDC == 6004) {	// 되돌리기
		OnbUNDO();
	}
	if (nIDC == 6005) {	// 원상태로 복원
		OnbRELOAD();
	}

	return;
}

HBRUSH CKolas2up_ImageEditorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


