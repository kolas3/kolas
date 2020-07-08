// UserMainBookSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainBookSearchDlg.h"
#include "UserMainBookDetail.h"
#include "..\\..\\..\\공통\\IndexToolKit\\IndexTool.h"
#include "..\\..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookSearchDlg dialog


CUserMainBookSearchDlg::CUserMainBookSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainBookSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bIsRunningThread = FALSE;

	m_bkR = 173;
	m_bkG = 224;
	m_bkB = 173;
	m_BkBrush.CreateSolidBrush(RGB(m_bkR, m_bkG, m_bkB));

	m_saLibrary.FreeExtra();
	m_saLibrary.RemoveAll();
	m_nSearchMode = 0;
	m_strCurrentPage = _T("");
	m_strPageCnt = _T("");
	m_strLibrary = _T("");
	m_strAll = _T("");
	m_strTitle = _T("");
	m_strKeyword = _T("");
	m_strAuthor = _T("");
	m_strPublisher = _T("");
	m_strRegNo = _T("");
	m_strISBN = _T("");
	m_strPublishYear1 = _T("");
	m_strPublishYear2 = _T("");
	
	// 16.10.27 JMJ KOL.RED.009
	m_bIsReservRunningThread = FALSE;
	m_nRow = 0;	
}

CUserMainBookSearchDlg::~CUserMainBookSearchDlg()
{
	if(NULL != m_pUserMainWait)
	{
		delete m_pUserMainWait;
		m_pUserMainWait = NULL;
	}		

	m_BkBrush.DeleteObject();
}

void CUserMainBookSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainBookSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainBookSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMainBookSearchDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BOOK_SEARCH_btnSHOW, OnBOOKSEARCHbtnSHOW)
	ON_BN_CLICKED(IDC_SEARCH_btnSearch2, OnSEARCHbtnSearch2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SEARCH_btnClear, OnSEARCHbtnClear)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_cmbCURRENTPAGE, OnSelchangecmbCURRENTPAGE)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_GRID_DISPLAY, OnGridDisplay)
	ON_MESSAGE(WM_USER_SET_MSG_WHERE, OnSetMsgWhere)
	ON_MESSAGE(WM_USER_SHOWRESEARCH, OnShowReSearchControl)
	ON_MESSAGE(MSG_SHOW_WAITDISPLAY, OnShowWaitDisplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookSearchDlg message handlers

BOOL CUserMainBookSearchDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CUserMainBookSearchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (TRUE == bShow) {
		((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->Invalidate(TRUE);
		((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetFocus();
	}
}

void CUserMainBookSearchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_BOOK_SEARCH_grid);
	if(pGrid->GetSafeHwnd() == NULL)	return;
	CWnd* pEdtResult = GetDlgItem(IDC_SEARCH_edtResult);
	if(pEdtResult->GetSafeHwnd() == NULL)	return;
	CWnd* pStcGuide = GetDlgItem(IDC_SEARCH_stcGuide);
	if(pStcGuide->GetSafeHwnd() == NULL)	return;
	CWnd* pStcPageCnt = GetDlgItem(IDC_stcPAGECNT);
	if(pStcPageCnt->GetSafeHwnd() == NULL)	return;
	CWnd* pCmdPageCnt = GetDlgItem(IDC_cmbPAGECNT);
	if(pCmdPageCnt->GetSafeHwnd() == NULL)	return;
	CWnd* pStcCurrentPate = GetDlgItem(IDC_stcCURRENTPAGE);
	if(pStcCurrentPate->GetSafeHwnd() == NULL)	return;
	CWnd* pCmdCurrentPate = GetDlgItem(IDC_cmbCURRENTPAGE);
	if(pCmdCurrentPate->GetSafeHwnd() == NULL)	return;

	CRect gridRect, btnReservationRect , rWhere, rResult, rGuide, rDlg, rStcPageCnt, rCmdPageCnt, rStcCurrentPate, rCmdCurrentPate;

	GetWindowRect(rDlg);
	pEdtResult->GetWindowRect(&rResult);
	pStcGuide->GetWindowRect(&rGuide);
	pStcPageCnt->GetWindowRect(&rStcPageCnt);
	pCmdPageCnt->GetWindowRect(&rCmdPageCnt);
	pStcCurrentPate->GetWindowRect(&rStcCurrentPate);
	pCmdCurrentPate->GetWindowRect(&rCmdCurrentPate);

	ScreenToClient(rWhere);
	ScreenToClient(rResult);
	ScreenToClient(rGuide);
	ScreenToClient(btnReservationRect);
	ScreenToClient(rStcPageCnt);
	ScreenToClient(rCmdPageCnt);
	ScreenToClient(rStcCurrentPate);
	ScreenToClient(rCmdCurrentPate);

	pEdtResult->MoveWindow(rResult.left, rResult.top, rDlg.Width()-(rResult.left+20), rResult.Height());
	

	pGrid->MoveWindow(rGuide.left, rGuide.bottom, rDlg.Width(), rDlg.Height()-(rGuide.bottom+30));
	pGrid->GetWindowRect(&gridRect);

	ScreenToClient(gridRect);
	
	pStcPageCnt->MoveWindow(rStcPageCnt.left, gridRect.bottom + 9, rStcPageCnt.Width(), rStcPageCnt.Height());
	pCmdPageCnt->MoveWindow(rCmdPageCnt.left, gridRect.bottom + 5, rCmdPageCnt.Width(), rCmdPageCnt.Height());
	pStcCurrentPate->MoveWindow(rStcCurrentPate.left, gridRect.bottom + 9, rStcCurrentPate.Width(), rStcCurrentPate.Height());
	pCmdCurrentPate->MoveWindow(rCmdCurrentPate.left, gridRect.bottom + 5, rCmdCurrentPate.Width(), rCmdCurrentPate.Height());
}

BOOL CUserMainBookSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_LILL_BOOKSEARCH")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error!"));
 		return FALSE;
 	}
	
	m_pDM.SetCONNECTION_INFO(_T(""));
 
	GridDisplay(_T(""));

	ShowReSearchControl(FALSE);

	ControlClear();

	m_pUserMainWait = new CUserMainWait(this);
 	m_pUserMainWait->Create(CUserMainWait::IDD, this);

	m_pReservDM = FindDM(_T("DM_LILL_BOOKSEARCH"));

	SetTimer(100, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CUserMainBookSearchDlg::ControlClear() 
{
	// 검색 대상 설정
	CComboBox* cmb = (CComboBox*)GetDlgItem(IDC_cmbTARGET);
	cmb->SetCurSel(0);
	
	// 페이지당 출력건수 설정
	CComboBox* cmbPage = (CComboBox*)GetDlgItem(IDC_cmbPAGECNT);
	cmbPage->SetCurSel(0);

	// 도서관 설정
	CComboBox* cmbLib = (CComboBox*)GetDlgItem(IDC_cmbLIBRARY);
	cmbLib->SetCurSel(0);
	
	// 현재 페이지
	CComboBox* cmbPageCnt = (CComboBox*)GetDlgItem(IDC_cmbCURRENTPAGE);
	cmbPageCnt->ResetContent();
	GetDlgItem(IDC_stcCURRENTPAGE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cmbCURRENTPAGE)->ShowWindow(SW_HIDE);

	INT ids;
	CESL_ControlMgr *pCM = FindCM(_T("CM_LILL_BOOKSEARCH"));
	if(NULL == pCM)
	{
		return;
	}
	ids = pCM->AllControlClear();
	if(0 > ids)
	{
		return ;
	}

	((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetFocus();
}

INT CUserMainBookSearchDlg::GridDisplay(CString sResult)
{
	INT ids;
	GetDlgItem(IDC_BOOK_SEARCH_grid)->ShowWindow(SW_SHOW);
	CESL_Grid *pGrid1 = (CESL_Grid*)GetDlgItem(IDC_BOOK_SEARCH_grid);
	pGrid1->m_nHeight = 30;
	ids = ControlDisplay(_T("CM_LILL_BOOKSEARCH_GRID"), _T("Grid"));
	if(0 > ids)
	{
		return -1;
	}

	GetDlgItem(IDC_SEARCH_edtResult)->SetWindowText(sResult);
	
	return 0;
}

VOID CUserMainBookSearchDlg::ShowReSearchControl(BOOL bShow)
{
	LockWindowUpdate();
	CRect rGuide;
	CWnd* pGuideLine = GetDlgItem(IDC_SEARCH_stcGuide);
	if(TRUE == bShow)
	{
		pGuideLine->GetWindowRect(rGuide);
		ScreenToClient(rGuide);
		rGuide.bottom = 123;
		pGuideLine->MoveWindow(rGuide);

		GetDlgItem(IDC_BOOK_SEARCH_stcResult)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SEARCH_edtResult)->ShowWindow(SW_SHOW);

		((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetWindowText(_T(""));
	}
	else
	{
		pGuideLine->GetWindowRect(rGuide);
		ScreenToClient(rGuide);
		rGuide.bottom = 82;
		pGuideLine->MoveWindow(rGuide);

		GetDlgItem(IDC_BOOK_SEARCH_stcResult)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SEARCH_edtResult)->ShowWindow(SW_HIDE);

		((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetFocus();
	}
	UnlockWindowUpdate();
 	CRect rc;
 	this->GetClientRect(rc);
 	this->PostMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
}

void CUserMainBookSearchDlg::OnBOOKSEARCHbtnSHOW() 
{
	GetDlgItem(IDC_SEARCH_edtAllSearch)->SetWindowText(_T(""));

	CRect rDlg, rEdtTotal;
	CWnd* pEdtTotal = GetDlgItem(IDC_SEARCH_edtAllSearch);
	pEdtTotal->GetWindowRect(rEdtTotal);

	CUserMainBookDetail dlg(this, rEdtTotal);
	dlg.DoModal();
	((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetFocus();

	return;	
}


LRESULT CUserMainBookSearchDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	CString *sResult = (CString*)lParam;
	GridDisplay(*sResult);
	return 0;
}

LRESULT CUserMainBookSearchDlg::OnShowReSearchControl(WPARAM wParam, LPARAM lParam)
{
	BOOL bShow = (BOOL)wParam;
	ShowReSearchControl(bShow);
	return 0;
}

LRESULT CUserMainBookSearchDlg::OnSetMsgWhere(WPARAM wParam, LPARAM lParam)
{
	CString *sWhere = (CString*)lParam;
	m_strMsgWhereView = *sWhere;
	return 0;
}

LRESULT CUserMainBookSearchDlg::OnShowWaitDisplay(WPARAM wParam, LPARAM lParam)
{	
	if(TRUE == wParam)
	{		
		m_pUserMainWait->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pUserMainWait->ShowWindow(SW_HIDE);
	}
	return 0;
}

void CUserMainBookSearchDlg::OnSEARCHbtnSearch2() 
{
	// 검색어
	CString strSearchWord = _T("");
	GetControlData(_T("CM_LILL_BOOKSEARCH"), _T("전체_간략"), strSearchWord);
	strSearchWord.TrimLeft(); strSearchWord.TrimRight();
	if (FALSE == strSearchWord.IsEmpty()) {
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSearchWord);
	}
	if (TRUE == strSearchWord.IsEmpty()) {
		EndWaitCursor();
		MessageBox(_T("검색어를 입력해 주십시오."), _T("통합검색"), MB_ICONEXCLAMATION);
		return;
	}
	// 검색페이지
	m_strCurrentPage = _T("1");
	// 도서관
	CComboBox* cmbLib = (CComboBox*)GetDlgItem(IDC_cmbLIBRARY);
	m_strLibrary = _T("");
	CString strTemp = _T("");
	if (0 == cmbLib->GetCurSel()) {
		for (int i = 1; i < cmbLib->GetCount(); i++) {
			cmbLib->GetLBText(i, strTemp);
			if (FALSE == m_strLibrary.IsEmpty()) m_strLibrary += _T(","); 
			m_strLibrary += strTemp.Left(6);
		}
	} else {
		cmbLib->GetLBText(cmbLib->GetCurSel(), strTemp);
		m_strLibrary = strTemp.Left(6);
	}
	// 검색 대상 확인
	m_strAll = _T("");
	m_strTitle = _T("");
	m_strKeyword = _T("");
	m_strAuthor = _T("");
	m_strPublisher = _T("");
	m_strRegNo = _T("");
	m_strISBN = _T("");
	m_strPublishYear1 = _T("");
	m_strPublishYear2 = _T("");
	// 0:전체, 1:제목, 2:키워드, 3:저자, 4:발행자
	CComboBox* cmb = (CComboBox*)GetDlgItem(IDC_cmbTARGET);
	INT nMode = cmb->GetCurSel();
	// 검색모드
	if (0 == nMode) {
		m_nSearchMode = 0; // 간략
	} else {
		m_nSearchMode = 1; // 상세
	}
	if (1 == nMode) { // 제목
		m_strTitle = strSearchWord;
	} else if (2 == nMode) { // 키워드
		m_strKeyword = strSearchWord;
	} else if (3 == nMode) { // 저자
		m_strAuthor = strSearchWord;
	} else if (4 == nMode) { // 발행자
		m_strPublisher = strSearchWord;
	} else { // 전체
		m_strAll = strSearchWord;
	}
	// 검색
   	SetTimer(200, 100, NULL);
}

UINT CUserMainBookSearchDlg::SearchThread(LPVOID pParam)
{
	CUserMainBookSearchDlg* pDlg = (CUserMainBookSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread)
	{
		return 0;
	}
	pDlg->m_bIsRunningThread = TRUE;	
	
	pDlg->SendMessage(MSG_SHOW_WAITDISPLAY, TRUE, NULL);
	pDlg->EnableWindow(FALSE);

	pDlg->SearchBookInfo();

	pDlg->EnableWindow(TRUE);
	pDlg->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
	
	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CUserMainBookSearchDlg::SearchBookInfo()
{
	INT ids;
	CString sBoWhere,sSeWhere;

	BeginWaitCursor();

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();

	// 페이지당 출력건수 설정
	// 0:15, 1:30, 2:50, 3:100, 4:200, 5:500
	CComboBox* cmbPage = (CComboBox*)GetDlgItem(IDC_cmbPAGECNT);
	cmbPage->GetLBText(cmbPage->GetCurSel(), m_strPageCnt);
	if (15 > _ttoi(m_strPageCnt)) {
		m_strPageCnt = _T("15");
	}

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K11_LOC_B01_SERVICE"));
	Writer.addRecord();
	if (0 == m_nSearchMode) {
		Writer.addElement(_T("OPTION"), _T("NOMAL"));
		Writer.addElement(_T("SEARCH_WORD"), m_strAll);
	} else {
		Writer.addElement(_T("OPTION"), _T("DETAIL"));
		Writer.addElement(_T("TITLE"), m_strTitle);
		Writer.addElement(_T("KEYWORD"), m_strKeyword);
		Writer.addElement(_T("AUTHOR"),m_strAuthor);
		Writer.addElement(_T("PUBLISHER"), m_strPublisher);
		Writer.addElement(_T("ISBN"), m_strISBN);
		Writer.addElement(_T("REG_NO"), m_strRegNo);
		Writer.addElement(_T("START_YEAR"), m_strPublishYear1);
		Writer.addElement(_T("END_YEAR"), m_strPublishYear2);
	}
	Writer.addElement(_T("LIB_CODE"), m_strLibrary);
	Writer.addElement(_T("PAGE_NO"), m_strCurrentPage);
	Writer.addElement(_T("DISPLAY"), m_strPageCnt);

	CString strMsg;
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) {
		EndWaitCursor();
		strMsg.Format(_T("조회에 실패하였습니다. (%d)"), ids);
		MessageBox(strMsg, _T("통합검색"), MB_ICONEXCLAMATION);
		return 200;
	}

	nFINISH = GetTickCount();
	CString strTime;
	strTime.Format(_T("%d.%d sec."), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_LILL_BOOKSEARCH"));
	if(NULL == pDM) {
		EndWaitCursor();
		return -2;
	}

	CString strTemp;
	Reader.makeDM(pDM);
	strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp) {
		EndWaitCursor();
		strTemp = Reader.getElement(0, _T("ID"));
		strMsg.Format(_T("조회에 실패하였습니다. (ID : %s)"), strTemp);
		MessageBox(strMsg, _T("통합검색"), MB_ICONEXCLAMATION);
		return 200;
	}

	int nTotalCnt = 0;
	CString strResult = _T("");
	CStringArray saData;
	for (int i = 0; i < Reader.getColumnCountNew(); i++) {
		if (FALSE == strResult.IsEmpty()) strResult += _T(", ");
		strTemp = Reader.getElementNew(0, i);
		if (FALSE == strTemp.IsEmpty()) {
			CLocCommonAPI::MakeArrayList(strTemp, _T(","), saData);
			strTemp = saData.GetAt(1) + _T("(") + saData.GetAt(2) + _T(")");
			strResult += strTemp;
			nTotalCnt += _ttoi(saData.GetAt(2));
		}
	}
	if (FALSE == strResult.IsEmpty()) {
		strTemp.Format(_T("전체(%d), %s   - %s"), nTotalCnt, strResult, strTime);
		strResult = strTemp;
		// 현재 페이지 설정
		if (_T("1") == m_strCurrentPage) {
			SetPage(nTotalCnt);
		}
	} else {
		strResult = _T("자료가 존재하지 않습니다.");
		// show
		GetDlgItem(IDC_stcCURRENTPAGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCURRENTPAGE)->ShowWindow(SW_HIDE);
	}
	
	this->SendMessage(WM_USER_GRID_DISPLAY, 0, (LPARAM)&strResult);
	this->SendMessage(WM_USER_SHOWRESEARCH, TRUE);
	
	EndWaitCursor();
	
	return 0;
}

BOOL CUserMainBookSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{		
		if(VK_RETURN == pMsg->wParam)
		{
			if(GetDlgItem(IDC_SEARCH_edtAllSearch)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnSEARCHbtnSearch2();
			}
			else if(GetDlgItem(IDC_SEARCH_btnSearch2)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnSEARCHbtnSearch2();
			}
			else if(GetDlgItem(IDC_BOOK_SEARCH_btnSHOW)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnBOOKSEARCHbtnSHOW();
			}
			
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam)
		{
			OnSEARCHbtnClear();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CUserMainBookSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_SEARCH_edtAllSearch == pWnd->GetDlgCtrlID())
 	{
 		pDC->SetBkMode(OPAQUE);
 		pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
 		return m_BkBrush;
 	}

	return hbr;
}

BEGIN_EVENTSINK_MAP(CUserMainBookSearchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUserMainBookSearchDlg)
	ON_EVENT(CUserMainBookSearchDlg, IDC_BOOK_SEARCH_grid, -600 /* Click */, OnClickBOOKSEARCHgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserMainBookSearchDlg::OnSEARCHbtnClear() 
{
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_LILL_BOOKSEARCH"));
	pDM->FreeData();
	GridDisplay(_T(""));
	ShowReSearchControl(FALSE);
	ControlClear();

	m_strMsgWhere = _T("");
	m_strMsgWhereBefore = _T("");
	m_strBoWhere = _T("");
	m_strSeWhere = _T("");
	m_strMsgWhereView = _T("");
}

void CUserMainBookSearchDlg::OnTimer(UINT nIDEvent) 
{
	if(100 == nIDEvent)
	{ // 도서관 설정
		KillTimer(nIDEvent);
		AfxBeginThread(getLibListThread, this);
	} else if (200 == nIDEvent) { // 검색
		KillTimer(nIDEvent);
		AfxBeginThread(SearchThread, this);
	// 16.10.27 JMJ KOL.RED.009
	} else if (300 == nIDEvent) {//예약현황
		KillTimer(nIDEvent);
		AfxBeginThread(ReservCheckThread, this);
	} else if(400 == nIDEvent){
		KillTimer(nIDEvent);
		GridDisplay(_T(""));
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CUserMainBookSearchDlg::getLibListThread(LPVOID pParam)
{
	CUserMainBookSearchDlg* pDlg = (CUserMainBookSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	
	pDlg->m_bIsRunningThread = TRUE;
	pDlg->EnableWindow(FALSE);
	INT ids = pDlg->startGetLibList();
	pDlg->EnableWindow(TRUE);
	pDlg->m_bIsRunningThread = FALSE;
	
	return 0;
}

INT CUserMainBookSearchDlg::startGetLibList()
{
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K05_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("LIB_CODE"), _T("ALL"));
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	INT ids = 0;
	INT cnt = 0;
	while (TRUE) {
		cnt++;
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (-200 == ids) {
			Sleep(300);
		} else {
			break;
		}
		if (3 < cnt) break;
	}
	if (0 > ids) return ids;

	CComboBox* cmbLib = (CComboBox*)GetDlgItem(IDC_cmbLIBRARY);
	CString strPageCnt = _T("");
	cmbLib->AddString(_T("전체"));
	CString strSetData = _T("");
	for (INT i = 0; i < Reader.getRecordCount(); i++) {
		strSetData = Reader.getElement(i, _T("LIB_CODE")) + _T(" : ") + Reader.getElement(i, _T("LIB_NAME"));
		cmbLib->AddString(strSetData);
		m_saLibrary.Add(strSetData);
	}
	cmbLib->SetCurSel(0);

	((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->Invalidate(TRUE);
	((CEdit*)GetDlgItem(IDC_SEARCH_edtAllSearch))->SetFocus();

	return 0;
}

void CUserMainBookSearchDlg::OnSelchangecmbCURRENTPAGE() 
{
	CComboBox* cmbPage = (CComboBox*)GetDlgItem(IDC_cmbCURRENTPAGE);
	m_strCurrentPage.Format(_T("%d"), cmbPage->GetCurSel()+1);
   	SetTimer(200, 100, NULL);
}

void CUserMainBookSearchDlg::SetPage(INT nTotalCnt) {
	int nCut = nTotalCnt/_ttoi(m_strPageCnt);
	int nRest = nTotalCnt%_ttoi(m_strPageCnt);
	int nTotPageCnt = 0;
	if (0 < nRest) {
		nTotPageCnt = nCut + 1;
	} else {
		nTotPageCnt = nCut;
	}
	if (1 > nTotPageCnt) nTotPageCnt = 1;
	CComboBox* cmbPageCnt = (CComboBox*)GetDlgItem(IDC_cmbCURRENTPAGE);
	cmbPageCnt->ResetContent();
	CString strTemp = _T("");
	for (int i = 0; i < nTotPageCnt; i++) {
		strTemp.Format(_T("%d"), i+1);
		cmbPageCnt->AddString(strTemp);
	}
	cmbPageCnt->SetCurSel(0);
	// show
	GetDlgItem(IDC_stcCURRENTPAGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_cmbCURRENTPAGE)->ShowWindow(SW_SHOW);
}

// 16.10.27 JMJ KOL.RED.009
VOID CUserMainBookSearchDlg::GetReservationCheck()
{
	CString sBookLibCode	= _T(""),
			sBookRecKey		= _T(""),
			sBookRegNo		= _T(""),
			sMsg			= _T(""),
			sCellData		= _T("");
	
	m_pReservDM->GetCellData(_T("LIB_CODE"), m_nRow, sBookLibCode);
	m_pReservDM->GetCellData(_T("LOCAL_KEY"), m_nRow, sBookRecKey);
	m_pReservDM->GetCellData(_T("REG_NO"), m_nRow, sBookRegNo);

	if(sBookLibCode.IsEmpty() || sBookRecKey.IsEmpty()){
		sMsg.Format(_T("등록번호 [%s] 자료의 데이터가 올바르지 않습니다."), sBookRegNo);
		AfxMessageBox(sMsg);
		return;
	}


	// 미들웨어 통신
	CFrameWriter Writer(m_pInfo);
	Writer.clear();
	Writer.setCommand(_T("LILL_K17_LOC_B01_SERVICE"));		
	Writer.addRecord();			
	Writer.addElement(_T("LIB_CODE"), sBookLibCode);
	Writer.addElement(_T("LOCAL_KEY"), sBookRecKey);
	
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	Reader.clear();
	INT ids = 0;
	INT cnt = 0;
	while (TRUE) {
		cnt++;
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (-200 == ids) {
			Sleep(300);
		} else {
			break;
		}
		if (3 < cnt) break;
	}
	if (0 > ids) return;
	
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		return;
	}
	else
	{
		sCellData = Reader.getElement(0, _T("예약중건수"));
	}

	if(sCellData.IsEmpty() == TRUE || sCellData.Compare(_T("")) == 0){
		AfxMessageBox(_T("예약건수를 조회하는데에 실패하였습니다.\r\n\r\n잠시후에 다시 시도하여 주십시오."));
		sCellData = _T("[조회]");
	}else{
		sCellData += _T("건");
	}
	//CString sCellData = _T("조회됨");
	m_pReservDM->SetCellData(_T("RESERVATION_CHECK"),sCellData, m_nRow);
		
	SetTimer(400, 100, NULL);

}
// 16.10.27 JMJ KOL.RED.009
void CUserMainBookSearchDlg::OnClickBOOKSEARCHgrid() 
{
	CString sCellData = _T("");
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_BOOK_SEARCH_grid);
	CESL_ControlMgr *pCM = FindCM(_T("CM_LILL_BOOKSEARCH_GRID"));
	

	m_nRow = pGrid->GetRowSel()-1;
	sCellData = pGrid->GetAt(m_nRow, pGrid->GetColSel()-1);

	// DM Default Value
	if(sCellData.Compare(_T("[조회]")) == 0){
		SetTimer(300, 100, NULL);				
	}
	
}
// 16.10.27 JMJ KOL.RED.009
UINT CUserMainBookSearchDlg::ReservCheckThread(LPVOID pParam)
{
	CUserMainBookSearchDlg* pDlg = (CUserMainBookSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsReservRunningThread)
	{
		return 0;
	}
	pDlg->m_bIsReservRunningThread = TRUE;	
	
	pDlg->SendMessage(MSG_SHOW_WAITDISPLAY, TRUE, NULL);
	pDlg->EnableWindow(FALSE);
	
	pDlg->GetReservationCheck();
	
	pDlg->EnableWindow(TRUE);
	pDlg->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
	
	pDlg->m_bIsReservRunningThread = FALSE;
	
	return 0;
}