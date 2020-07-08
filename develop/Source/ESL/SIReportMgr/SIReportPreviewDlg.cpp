// SIReportPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportPrintDlg.h"
#include "SIReportPreviewDlg.h"
#include "..\\SIReportData\\SIReportReadBroker.h"
#include "efs.h"
#include "NewButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewDlg dialog


CSIReportPreviewDlg::CSIReportPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIReportPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIReportPreviewDlg)
	//}}AFX_DATA_INIT
	m_pView = NULL;
	m_pManager = NULL;
	bZoomMode = 1;			// 지금 크게 보임.	
}


VOID CSIReportPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportPreviewDlg)
	DDX_Control(pDX, IDC_cPAGELIST, m_cPageList);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CSIReportPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CSIReportPreviewDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bZOOM, OnbZOOM)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnFIRST, OnbtnFIRST)
	ON_BN_CLICKED(IDC_btnEND, OnbtnEND)
	ON_BN_CLICKED(IDC_bMODIFY_FORM, OnbMODIFYFORM)
	ON_BN_CLICKED(IDC_bREFRESH, OnbREFRESH)
	ON_BN_CLICKED(IDC_bMODIFY_VALUE, OnbMODIFYVALUE)
	ON_CBN_SELCHANGE(IDC_cPAGELIST, OnSelchangecPAGELIST)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_btnEXCEL, OnbtnEXCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewDlg message handlers

BOOL CSIReportPreviewDlg::OnInitDialog() 
{
EFS_BEGIN
	CDialog::OnInitDialog();
	
	SetWindowText(_T("SI Report Preview Dialog"));

	// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Broke 파일 삭제
	CSIReportReadBroker broker;
	if ( broker.Load() != -1 )
		DeleteFile(broker.BrokerFileName);
	// -----------------------------------------------------------------------------------------	

	m_pView = new CSIReportPreviewView(m_pManager, this);
	m_mapsize = CSize(1400, 1000);			// default 는 PAPER_A4_LANDSCAPE
	INT nPaperType = m_pManager->GetWorkFormPaper();
	if (nPaperType == -1) nPaperType = m_pManager->m_nDefaultPaperType;
	if (nPaperType == PAPER_A4_PORTRAIT) m_mapsize = CSize(PAPER_A4_X, PAPER_A4_Y);
	if (nPaperType == PAPER_A4_LANDSCAPE) m_mapsize = CSize(PAPER_A4_Y, PAPER_A4_X);
	if (nPaperType == PAPER_B4_PORTRAIT) m_mapsize = CSize(PAPER_B4_X, PAPER_B4_Y);
	if (nPaperType == PAPER_B4_LANDSCAPE) m_mapsize = CSize(PAPER_B4_Y, PAPER_B4_X);
	if (nPaperType == PAPER_FANFOLD_US_PORTRAIT) m_mapsize = CSize(PAPER_FALFOLD_US_X, PAPER_FALFOLD_US_Y);
	if (nPaperType == PAPER_FANFOLD_US_LANDSCAPE) m_mapsize = CSize(PAPER_FALFOLD_US_Y, PAPER_FALFOLD_US_X);

	m_viewrect = CRect( 20,85,775, 552);
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	m_pView->CreateEx( WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, m_viewrect, this, 1, NULL );	
	m_nPage = m_pView->m_page = 1;
	m_pManager->m_nCurrentPreviewPage = -1;

	m_nTotalPages = m_pManager->m_EventInfo.m_nTotalPage;
	GetDlgItem(IDC_bZOOM)->SetWindowText(_T("축소"));
	SetPageListCombo();
	ShowPageNum();

	m_pManager->pWnd = this;
	// 비트맵 버튼의 초기화
	NewButtonList.Add(IDC_btnFIRST, this, IDB_FIRST_U, IDB_FIRST_D, IDB_FIRST_F);
	NewButtonList.Add(IDC_bPREV, this, IDB_PREV_U, IDB_PREV_D, IDB_PREV_F);
	NewButtonList.Add(IDC_bNEXT, this, IDB_NEXT_U, IDB_NEXT_D, IDB_NEXT_F);
	NewButtonList.Add(IDC_btnEND, this, IDB_END_U, IDB_END_D, IDB_END_F);
	NewButtonList.Add(IDC_btnPRINT, this, IDB_PRINT_U, IDB_PRINT_D, IDB_PRINT_F);
	NewButtonList.Add(IDOK, this, IDB_CLOSE_U, IDB_CLOSE_D, IDB_CLOSE_F);
	NewButtonList.Add(IDC_bZOOM, this, IDB_ZOOMIN_U, IDB_ZOOMIN_D, IDB_ZOOMIN_F);
	NewButtonList.Add(IDC_bMODIFY_FORM, this, IDB_MODIFY_FORM_U, IDB_MODIFY_FORM_D, IDB_MODIFY_FORM_F);
	NewButtonList.Add(IDC_bMODIFY_VALUE, this, IDB_MODIFY_VALUE_U, IDB_MODIFY_VALUE_D, IDB_MODIFY_VALUE_F);
	NewButtonList.Add(IDC_bREFRESH, this, IDB_REFRESH_U, IDB_REFRESH_D, IDB_REFRESH_F);
	
	CNewButton *pButton = new CNewButton;
	if (!pButton) return -1;
	pButton->SubclassDlgItem(IDC_btnEXCEL, this);
	pButton->LoadBitmap(_T("..\\bmp\\SIReportEditor\\excel_export_f.bmp"),
		                _T("..\\bmp\\SIReportEditor\\excel_export_d.bmp"),
						_T("..\\bmp\\SIReportEditor\\excel_export_n.bmp"));
	NewButtonList.LIST.AddTail((CObject*)pButton);
	
	
	m_pManager->ShowXMLFileOnePage(MANAGER_MODE_PREVIEW, NULL, m_nPage);	

	m_pView->EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CSIReportPreviewDlg::OnDestroy() 
{
EFS_BEGIN

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pView) { 
		delete m_pView;
		m_pView = NULL;
	}

EFS_END
}

VOID CSIReportPreviewDlg::OnOK() 
{
EFS_BEGIN
	// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 출력기록 보관기능
	CDialog::OnCancel();
	// >> 2015/11/11 --MOD

EFS_END
}

VOID CSIReportPreviewDlg::OnbZOOM() 
{
EFS_BEGIN

	if (bZoomMode == 1) {		// 지금은 크게 보이니 축소를 하자
		m_pView->SetScaleToFitSize(m_mapsize);	
		bZoomMode = 2;	
		GetDlgItem(IDC_bZOOM)->SetWindowText(_T("확대"));	
		return;
	}
	if (bZoomMode == 2) {		// 지금은 작게 보이니 확대를 하자
		m_pView->SetScrollSizes( MM_TEXT, m_mapsize);		
		bZoomMode = 1;	
		GetDlgItem(IDC_bZOOM)->SetWindowText(_T("축소"));
		return;
	}		

EFS_END
}

VOID CSIReportPreviewDlg::OnbNEXT() 
{
EFS_BEGIN

	if (m_nPage >= m_nTotalPages) {
		AfxMessageBox(_T("맨 마지막 페이지 입니다."));
		return;
	}
	ShowPage(m_nPage+1);

EFS_END
}

VOID CSIReportPreviewDlg::OnbPREV() 
{
EFS_BEGIN

	if (m_nPage <= 1) {
		AfxMessageBox(_T("맨 처음 페이지 입니다."));
		return;
	}
	ShowPage(m_nPage-1);

EFS_END
}

INT CSIReportPreviewDlg::ShowPageNum(VOID)
{
EFS_BEGIN

	INT nCurPage = m_nPage;
	CString msg;
	msg.Format(_T("%d / %d 페이지"), nCurPage, m_nTotalPages);
	GetDlgItem(IDC_sPAGE)->SetWindowText(msg);

	m_cPageList.SetCurSel(nCurPage-1);
	UpdateData(false);
	return 0;

EFS_END
return -1;

}

VOID CSIReportPreviewDlg::OnbtnPRINT(VOID)
{
EFS_BEGIN

	//m_pManager->PrintDocument();
	CSIReportPrintDlg dlg(this, m_pManager);
	dlg.m_nStartPage = 1;
	dlg.m_nEndPage = m_nTotalPages;
	dlg.m_nTotalPages = m_nTotalPages;
	dlg.DoModal();
	if (dlg.IsPrint == TRUE) CDialog::OnOK();

EFS_END
}
VOID CSIReportPreviewDlg::OnbtnFIRST() 
{
EFS_BEGIN

	if (m_nPage <= 1) {
		AfxMessageBox(_T("맨 처음 페이지 입니다."));
		return;
	}
	ShowPage(1);

EFS_END
}

VOID CSIReportPreviewDlg::OnbtnEND() 
{
EFS_BEGIN

	if (m_nPage >= m_nTotalPages) {
		AfxMessageBox(_T("맨 마지막 페이지 입니다."));
		return;
	}
	ShowPage(m_nTotalPages);

EFS_END
}

VOID CSIReportPreviewDlg::OnbREFRESH() 
{
EFS_BEGIN

	INT ids;
	CSIReportReadBroker broker;
	broker.Load();
	if (broker.MODE == 1) {
		m_pManager->LoadPageXMLWorkForm(m_pManager->m_nCurrentPreviewPage);
		m_pView->Invalidate(TRUE);
		ShowPageNum();
	}
	if (broker.MODE == 2) {
		ids = m_pManager->GetSIReportFileFromDB(broker.RPTALIAS);
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}
		m_pManager->MakeXMLPageFiles();
		m_pManager->LoadPageXMLWorkForm(1);
		ShowPage(1);
	}

EFS_END
}

VOID CSIReportPreviewDlg::OnbMODIFYFORM() 
{
EFS_BEGIN

	CSIReportReadBroker broker;
	broker.SetData(m_pManager->m_strRPTPK, m_pManager->m_strRPTALIAS, 2);
	broker.Save();
	HINSTANCE instance;

	CWnd *pWnd = FindWindow(_T("SIReportEditor"), NULL);
	if (pWnd) {
		if (pWnd) ::PostMessage(pWnd->m_hWnd, WM_USER+100, NULL, NULL);
	} else {
		instance = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("SIReportEditor.exe"), m_pManager->m_strRPTPK, NULL, SW_SHOWNORMAL);
	}
	return;
/*
	HANDLE hDle = OpenMutex(NULL, TRUE, _T("SIReportEditor"));
	DWORD dwErr = GetLastError();
	if (dwErr == 2) {		// 지정된 파일을 찾을 수 없습니다. -> SIReportEditor가 안떠 있다.
		instance = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("SIReportEditor.exe"), m_pManager->m_strRPTPK, NULL, SW_SHOWNORMAL);
	} else {				//dwErr == 5, 액세스가 거부되었습니다. -> SIReportEditor가 떠있다.
		CWnd *pWnd = FindWindow(_T("SIReportEditor"), NULL);
		if (pWnd) ::PostMessage(pWnd->m_hWnd, WM_USER+100, NULL, NULL);
	}
*/
EFS_END
}

VOID CSIReportPreviewDlg::OnbMODIFYVALUE() 
{
EFS_BEGIN

	CSIReportReadBroker broker;
	broker.SetData(m_pManager->m_strRPTPK, m_pManager->m_strRPTALIAS, 1);
	broker.Save();
	HINSTANCE instance;

	CWnd *pWnd = FindWindow(_T("SIReportEditor"), NULL);
	if (pWnd) {
		if (pWnd) ::PostMessage(pWnd->m_hWnd, WM_USER+100, NULL, NULL);
	} else {
		instance = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("SIReportEditor.exe"), m_pManager->m_strRPTPK, NULL, SW_SHOWNORMAL);
	}
	return;
/*
	HANDLE hDle = OpenMutex(NULL, TRUE, _T("SIReportEditor"));
	DWORD dwErr = GetLastError();
	if (dwErr == 2) {		// 지정된 파일을 찾을 수 없습니다. -> SIReportEditor가 안떠 있다.
		instance = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("SIReportEditor.exe"), m_pManager->m_strRPTPK, NULL, SW_SHOWNORMAL);
	} else {				//dwErr == 5, 액세스가 거부되었습니다. -> SIReportEditor가 떠있다.
		CWnd *pWnd = FindWindow(_T("SIReportEditor"), NULL);
		if (pWnd) ::PostMessage(pWnd->m_hWnd, WM_USER+100, NULL, NULL);
	}
*/
EFS_END
}

VOID CSIReportPreviewDlg::SetPageListCombo(VOID)
{
EFS_BEGIN

	UpdateData(true);
	m_cPageList.ResetContent();
	UpdateData(false);

	if (!m_pManager) return;
	CString strTmp;
	for (INT i = 1; i <= m_pManager->m_EventInfo.m_nTotalPage; i++) {
		strTmp.Format(_T("%d"), i);
		m_cPageList.AddString(strTmp);
	}
	UpdateData(false);

EFS_END
}

VOID CSIReportPreviewDlg::OnSelchangecPAGELIST() 
{
EFS_BEGIN

	UpdateData(true);
	CString strPage;
	m_cPageList.GetWindowText(strPage);
	INT nPage = m_pView->m_page;

	if (strPage.GetLength() > 0) {
		nPage = _ttoi(strPage);
	}
	ShowPage(nPage);

EFS_END
}

INT CSIReportPreviewDlg::ShowPage(INT nPage)
{
EFS_BEGIN

	m_nPage = nPage;
	m_pView->m_page = m_nPage;
	//m_pManager->ShowXMLFilePage(MANAGER_MODE_PREVIEW, NULL, m_nPage, m_nPage);	
	m_pManager->ShowXMLFileOnePage(MANAGER_MODE_PREVIEW, NULL, m_nPage);
	m_pView->Invalidate(TRUE);
	ShowPageNum();	

	return 0;

EFS_END
return -1;

}
VOID CSIReportPreviewDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	RESIZE_Controls();
}

VOID CSIReportPreviewDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RESIZE_Controls();
	
}

INT CSIReportPreviewDlg::RESIZE_Controls(VOID) 
{
	CWnd *pWnd;

	CRect rcClient, rcControl;
	GetClientRect(rcClient);

	pWnd = GetDlgItem(IDC_STATIC_BORDER);
	if (pWnd) {
		rcControl = rcClient;
		rcControl.left = rcControl.left + 5;
		rcControl.right = rcControl.right - 5;
		rcControl.top = rcControl.top + 70;
		rcControl.bottom = rcControl.bottom - 5;

		pWnd->MoveWindow(rcControl, TRUE);
	}

	if (m_pView) {
		if (m_pView->GetSafeHwnd()) {
			rcControl = rcClient;
			rcControl.left = rcControl.left + 10;
			rcControl.right = rcControl.right - 10;
			rcControl.top = rcControl.top + 80;
			rcControl.bottom = rcControl.bottom - 10;

			m_pView->MoveWindow(rcControl, TRUE);
		}
	}
	return 0;
}

BOOL CSIReportPreviewDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	//###########################################################################
	//2005-02-03 leehakjoong
	m_pView->OnMouseWheel(nFlags,zDelta,pt) ;

	//###########################################################################


	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

// 2008.09.30 ADD BY PWR
// 엑셀 반출
void CSIReportPreviewDlg::OnbtnEXCEL() 
{
	

}

HBRUSH CSIReportPreviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
