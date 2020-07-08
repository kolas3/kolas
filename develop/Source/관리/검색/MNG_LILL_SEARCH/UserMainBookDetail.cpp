// UserMainBookDetail.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainBookDetail.h"
#include "..\\..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookDetail dialog


CUserMainBookDetail::CUserMainBookDetail(CUserMainBookSearchDlg* pParent /*=NULL*/, CRect rect)
	: CESL_Mgr(CUserMainBookDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainBookDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_rect = rect;
	m_bIsRunningThread = FALSE;
	m_pParent = pParent;
}
CUserMainBookDetail::~CUserMainBookDetail()
{
}

void CUserMainBookDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainBookDetail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainBookDetail, CDialog)
	//{{AFX_MSG_MAP(CUserMainBookDetail)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Detail_btnClose, OnDetailbtnClose)
	ON_BN_CLICKED(IDC_Detail_btnClear, OnDetailbtnClear)
	ON_BN_CLICKED(IDC_Detail_btnSearch, OnDetailbtnSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookDetail message handlers

BOOL CUserMainBookDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0 > InitESL_Mgr(_T("SM_LILL_DETAIL_SEARCH")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
 	}

	// 도서관설정
	CComboBox* cmbLib = (CComboBox*)GetDlgItem(IDC_Detail_cmbLIBRARY);
	cmbLib->AddString(_T("전체"));
	for (int i = 0; i < m_pParent->m_saLibrary.GetSize(); i++) {
		cmbLib->AddString(m_pParent->m_saLibrary.GetAt(i));
	}
	
	clearControl();

	SetDlgPos();

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_LILL_DETAIL_SEARCH"), _T("등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);	

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------		

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserMainBookDetail::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

VOID CUserMainBookDetail::SetDlgPos()
{
	CRect rDlg;
    GetWindowRect(rDlg);
    MoveWindow(m_rect.left+40, m_rect.bottom+2, rDlg.Width(), rDlg.Height());
}

void CUserMainBookDetail::clearControl()
{
	INT ids;
	CESL_ControlMgr *pCM = FindCM(_T("CM_LILL_DETAIL_SEARCH"));
	if(NULL == pCM)
	{
		return;
	}
	ids = pCM->AllControlClear();
	if(0 > ids)
	{
		return ;
	}

	CComboBox* cmbLib = (CComboBox*)GetDlgItem(IDC_Detail_cmbLIBRARY);
	cmbLib->SetCurSel(0);
	
	((CEdit*)GetDlgItem(IDC_Detail_edtTitle))->SetFocus();
}

void CUserMainBookDetail::OnDetailbtnClose() 
{
	OnCancel();
}

void CUserMainBookDetail::OnDetailbtnClear() 
{
	clearControl();
}

void CUserMainBookDetail::OnDetailbtnSearch() 
{
	// 전체
	CString strAll = _T("");
	GetDlgItemText(IDC_Detail_edtAll, strAll);
	// 제목
	CString strTitle = _T("");
	GetDlgItemText(IDC_Detail_edtTitle, strTitle);
	// 키워드
	CString strKeyword = _T("");
	GetDlgItemText(IDC_Detail_edtKeyword, strKeyword);
	// 저자
	CString strAuthor = _T("");
	GetDlgItemText(IDC_Detail_edtAuthor, strAuthor);
	// 발행자
	CString strPublisher = _T("");
	GetDlgItemText(IDC_Detail_edtPublisher, strPublisher);
	// 등록번호
	CString strRegNo = _T("");
	GetDlgItemText(IDC_Detail_edtRegNo, strRegNo);
	if (FALSE == strRegNo.IsEmpty()) {
		CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
	}
	// ISBN
	CString strISBN = _T("");
	GetDlgItemText(IDC_Detail_edtISBN, strISBN);
	// 발행년도1
	CString strPublishYear1 = _T("");
	GetDlgItemText(IDC_Detail_edtPublishYear1, strPublishYear1);
	// 발행년도2
	CString strPublishYear2 = _T("");
	GetDlgItemText(IDC_Detail_edtPublishYear2, strPublishYear2);
	// 검색어 확인
	if (TRUE == strAll.IsEmpty()) {
		if (strTitle.IsEmpty() && strKeyword.IsEmpty() && strAuthor.IsEmpty() && strPublisher.IsEmpty() && strRegNo.IsEmpty() && strISBN.IsEmpty()) {
			MessageBox(_T("검색어를 입력해 주십시오."), _T("상세검색"), MB_ICONEXCLAMATION);
			return;
		}
	}
	// 검색페이지
	m_pParent->m_strCurrentPage = _T("1");
	// 도서관
	CComboBox* cmbLib = (CComboBox*)GetDlgItem(IDC_Detail_cmbLIBRARY);
	m_pParent->m_strLibrary = _T("");
	CString strTemp = _T("");
	if (0 == cmbLib->GetCurSel()) {
		for (int i = 1; i < cmbLib->GetCount(); i++) {
			cmbLib->GetLBText(i, strTemp);
			if (FALSE == m_pParent->m_strLibrary.IsEmpty()) m_pParent->m_strLibrary += _T(","); 
			m_pParent->m_strLibrary += strTemp.Left(6);
		}
	} else {
		cmbLib->GetLBText(cmbLib->GetCurSel(), strTemp);
		m_pParent->m_strLibrary = strTemp.Left(6);
	}
	// 검색 대상 확인
	m_pParent->m_strAll = strAll;
	m_pParent->m_strTitle = strTitle;
	m_pParent->m_strKeyword = strKeyword;
	m_pParent->m_strAuthor = strAuthor;
	m_pParent->m_strPublisher = strPublisher;
	m_pParent->m_strRegNo = strRegNo;
	m_pParent->m_strISBN = strISBN;
	m_pParent->m_strPublishYear1 = strPublishYear1;
	m_pParent->m_strPublishYear2 = strPublishYear2;
	// 검색모드 (상세)
	m_pParent->m_nSearchMode = 1;
	// 검색
   	m_pParent->SetTimer(200, 100, NULL);
	OnOK();
}

BOOL CUserMainBookDetail::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{		
		if(VK_RETURN == pMsg->wParam)
		{
			if( GetDlgItem(IDC_Detail_edtAll)->GetSafeHwnd() == pMsg->hwnd || GetDlgItem(IDC_Detail_edtTitle)->GetSafeHwnd() == pMsg->hwnd ||
				GetDlgItem(IDC_Detail_edtKeyword)->GetSafeHwnd() == pMsg->hwnd || GetDlgItem(IDC_Detail_edtAuthor)->GetSafeHwnd() == pMsg->hwnd ||
				GetDlgItem(IDC_Detail_edtPublisher)->GetSafeHwnd() == pMsg->hwnd || GetDlgItem(IDC_Detail_edtRegNo)->GetSafeHwnd() == pMsg->hwnd ||
				GetDlgItem(IDC_Detail_edtISBN)->GetSafeHwnd() == pMsg->hwnd || GetDlgItem(IDC_Detail_edtPublishYear1)->GetSafeHwnd() == pMsg->hwnd ||
				GetDlgItem(IDC_Detail_edtPublishYear2)->GetSafeHwnd() == pMsg->hwnd || GetDlgItem(IDC_Detail_btnSearch)->GetSafeHwnd() == pMsg->hwnd) 
			{
				OnDetailbtnSearch();
			} 
			else if(GetDlgItem(IDC_Detail_btnClose)->GetSafeHwnd() == pMsg->hwnd) 
			{
				OnDetailbtnClose();
			}
			else if(GetDlgItem(IDC_Detail_btnClear)->GetSafeHwnd() == pMsg->hwnd) 
			{
				OnDetailbtnClear();
			}
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam)
		{
			OnCancel();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
