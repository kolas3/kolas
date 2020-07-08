// SIReportPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SIReportPrintDlg.h"
#include "..\\SIReportData\\SIReportReadBroker.h"

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
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportPreviewDlg)
	DDX_Control(pDX, IDC_cPAGELIST, m_cPageList);
	//}}AFX_DATA_MAP
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportPreviewDlg message handlers

BOOL CSIReportPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetWindowText(_T("SI Report Preview Dialog"));

	m_pView = new CSIReportPreviewView(m_pManager, this);
	m_mapsize = CSize(1400, 1000);			// default 는 PAPER_A4_LANDSCAPE
	if (m_pManager->GetWorkFormPaper() == PAPER_A4_PORTRAIT) m_mapsize = CSize(693, 980);
	if (m_pManager->GetWorkFormPaper() == PAPER_A4_LANDSCAPE) m_mapsize = CSize(980, 693);

	m_viewrect = CRect( 20,85,775, 552);
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	m_pView->CreateEx( WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, m_viewrect, this, 1, NULL );	
	m_nPage = m_pView->m_page = 1;
	m_pManager->m_nCurrentPreviewPage = -1;
	//m_pManager->ShowXMLFileOnePage(MANAGER_MODE_PREVIEW, NULL, 1, 1);
	//m_pManager->ShowXMLFilePage(MANAGER_MODE_PREVIEW, NULL, m_nPage, m_nPage);

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


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSIReportPreviewDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pView) { 
		delete m_pView;
		m_pView = NULL;
	}
}

VOID CSIReportPreviewDlg::OnOK() 
{
	CDialog::OnOK();
}

VOID CSIReportPreviewDlg::OnbZOOM() 
{
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
}

VOID CSIReportPreviewDlg::OnbNEXT() 
{
	if (m_nPage >= m_nTotalPages) {
		AfxMessageBox(_T("맨 마지막 페이지 입니다."));
		return;
	}
	ShowPage(m_nPage+1);
}

VOID CSIReportPreviewDlg::OnbPREV() 
{
	if (m_nPage <= 1) {
		AfxMessageBox(_T("맨 처음 페이지 입니다."));
		return;
	}
	ShowPage(m_nPage-1);
}

INT CSIReportPreviewDlg::ShowPageNum(VOID)
{
	INT nCurPage = m_nPage;
	CString msg;
	msg.Format(_T("%d / %d 페이지"), nCurPage, m_nTotalPages);
	GetDlgItem(IDC_sPAGE)->SetWindowText(msg);

	m_cPageList.SetCurSel(nCurPage-1);
	UpdateData(false);
	return 0;
}

VOID CSIReportPreviewDlg::OnbtnPRINT(VOID)
{
	//m_pManager->PrintDocument();
	CSIReportPrintDlg dlg(this, m_pManager);
	dlg.m_nStartPage = 1;
	dlg.m_nEndPage = m_nTotalPages;
	dlg.m_nTotalPages = m_nTotalPages;
	dlg.DoModal();
	if (dlg.IsPrint == TRUE) CDialog::OnOK();
}
VOID CSIReportPreviewDlg::OnbtnFIRST() 
{
	if (m_nPage <= 1) {
		AfxMessageBox(_T("맨 처음 페이지 입니다."));
		return;
	}
	ShowPage(1);
}

VOID CSIReportPreviewDlg::OnbtnEND() 
{
	if (m_nPage >= m_nTotalPages) {
		AfxMessageBox(_T("맨 마지막 페이지 입니다."));
		return;
	}
	ShowPage(m_nTotalPages);
}

VOID CSIReportPreviewDlg::OnbREFRESH() 
{
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
}

VOID CSIReportPreviewDlg::OnbMODIFYFORM() 
{
	CSIReportReadBroker broker;
	broker.SetData(m_pManager->m_strRPTPK, m_pManager->m_strRPTALIAS, 2);
	broker.Save();
	HINSTANCE instance;

	HANDLE hDle = OpenMutex(NULL, TRUE, _T("SIReportEditor"));
	DWORD dwErr = GetLastError();
	if (dwErr == 2) {		// 지정된 파일을 찾을 수 없습니다. -> SIReportEditor가 안떠 있다.
		instance = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("SIReportEditor.exe"), m_pManager->m_strRPTPK, NULL, SW_SHOWNORMAL);

	} else {				//dwErr == 5, 액세스가 거부되었습니다. -> SIReportEditor가 떠있다.
		CWnd *pWnd = FindWindow(_T("SIReportEditor"), NULL);
		if (pWnd) ::PostMessage(pWnd->m_hWnd, WM_USER+100, NULL, NULL);
	}
}

VOID CSIReportPreviewDlg::OnbMODIFYVALUE() 
{
	CSIReportReadBroker broker;
	broker.SetData(m_pManager->m_strRPTPK, m_pManager->m_strRPTALIAS, 1);
	broker.Save();
	HINSTANCE instance;

	HANDLE hDle = OpenMutex(NULL, TRUE, _T("SIReportEditor"));
	DWORD dwErr = GetLastError();
	if (dwErr == 2) {		// 지정된 파일을 찾을 수 없습니다. -> SIReportEditor가 안떠 있다.
		instance = ShellExecute(this->GetSafeHwnd(), _T("open"), _T("SIReportEditor.exe"), m_pManager->m_strRPTPK, NULL, SW_SHOWNORMAL);
	} else {				//dwErr == 5, 액세스가 거부되었습니다. -> SIReportEditor가 떠있다.
		CWnd *pWnd = FindWindow(_T("SIReportEditor"), NULL);
		if (pWnd) ::PostMessage(pWnd->m_hWnd, WM_USER+100, NULL, NULL);
	}
}

VOID CSIReportPreviewDlg::SetPageListCombo(VOID)
{
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
}

VOID CSIReportPreviewDlg::OnSelchangecPAGELIST() 
{
	UpdateData(true);
	CString strPage;
	m_cPageList.GetWindowText(strPage);
	INT nPage = m_pView->m_page;

	if (strPage.GetLength() > 0) {
		nPage = _ttoi(strPage);
	}
	ShowPage(nPage);
}

INT CSIReportPreviewDlg::ShowPage(INT nPage)
{
	m_nPage = nPage;
	m_pView->m_page = m_nPage;
	//m_pManager->ShowXMLFilePage(MANAGER_MODE_PREVIEW, NULL, m_nPage, m_nPage);	
	m_pManager->ShowXMLFileOnePage(MANAGER_MODE_PREVIEW, NULL, m_nPage);
	m_pView->Invalidate(TRUE);
	ShowPageNum();	

	return 0;
}