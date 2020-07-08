// KL_CryptoMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoMainDlg.h"
#include "KL_CryptoInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CKL_CryptoInfo* theInfo;

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoMainDlg dialog


CKL_CryptoMainDlg::CKL_CryptoMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKL_CryptoMainDlg::IDD, pParent)
{
#ifdef CRYPTO_DEV
	m_nStepPage = (UINT)STEP_PAGE_CONN;
#else
	m_nStepPage = (UINT)STEP_PAGE_DESC;
#endif
}

void CKL_CryptoMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoMainDlg)
	DDX_Control(pDX, IDC_BTN_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_CryptoMainDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_CryptoMainDlg)
	ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoMainDlg message handlers

/*=============================================================
	@ Function    :	OnInitDialog
	@ Description :
=============================================================*/
BOOL CKL_CryptoMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitStepPage();
	
	return TRUE;
}

/*=============================================================
	@ Function    :	PreTranslateMessage
	@ Description :
=============================================================*/
BOOL CKL_CryptoMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_Step4Exec.IsRunThread() && pMsg->message == WM_KEYDOWN)
	{
		// 쓰레드 중에는 입력을 받지 않는다
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/*=============================================================
	@ Function    :	OnBtnPrev
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::OnBtnPrev() 
{
	m_nStepPage--;
	ShowStepPage((STEP_PAGE_TYPE)m_nStepPage);
}

/*=============================================================
	@ Function    :	OnBtnNext
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::OnBtnNext() 
{
	// 페이지 이동 전에 필수 항목들을 입력했는지 확인해본다.
	switch (m_nStepPage)
	{
	case STEP_PAGE_DESC:
		// 확인할 내용이 없다.
		break;
	case STEP_PAGE_CONN:
		// 접속 성공 유무를 확인한다
		{
			if (theInfo->GetConnectInfo().IsEmpty() || theInfo->GetKolasUserInfo().IsEmpty())
			{
				MessageBox(_T("연결테스트가 성공한 경우에만 다음 단계로 진행할 수 있습니다"), _T("접속로그 일괄구축 프로그램"), MB_ICONEXCLAMATION);
				return;
			}
		}
		break;
	case STEP_PAGE_EXEC:
		// 확인할 내용이 없다.
		break;
	}

	m_nStepPage++;
	ShowStepPage((STEP_PAGE_TYPE)m_nStepPage);
}

/*=============================================================
	@ Function    :	InitStepPage
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::InitStepPage()
{
	CRect rc;
	GetDlgItem(IDC_STATIC_PROPERTY_PAGE)->GetWindowRect(rc);
	ScreenToClient(rc);

	// 페이지 생성
	m_Step1Desc.Create(CKL_CryptoDescPage::IDD, this);
	m_Step2Conn.Create(CKL_CryptoConnPage::IDD, this);	
	m_Step4Exec.Create(CKL_CryptoExecPage::IDD, this);

	// 위치 조정
	m_Step1Desc.MoveWindow(rc);
	m_Step2Conn.MoveWindow(rc);	
	m_Step4Exec.MoveWindow(rc);

	ShowStepPage((STEP_PAGE_TYPE)m_nStepPage);
}

/*=============================================================
	@ Function    :	ShowStepPage
	@ Description :
=============================================================*/
void CKL_CryptoMainDlg::ShowStepPage(STEP_PAGE_TYPE enStepPage)
{
	m_Step1Desc.ShowWindow(FALSE);
	m_Step2Conn.ShowWindow(FALSE);
	m_Step4Exec.ShowWindow(FALSE);

	m_btnPrev.EnableWindow(TRUE);
	m_btnNext.EnableWindow(TRUE);

	switch (enStepPage)
	{
	case STEP_PAGE_DESC:
		m_Step1Desc.ShowWindow(TRUE);
		m_btnPrev.EnableWindow(FALSE);	// 맨 앞 페이지라면 이전 버튼 사용불가
		break;
	case STEP_PAGE_CONN:
		m_Step2Conn.ShowWindow(TRUE);
		break;
	case STEP_PAGE_EXEC:
		m_Step4Exec.ShowWindow(TRUE);
		m_btnNext.EnableWindow(FALSE);	// 맨 뒤 페이지라면 다음 버튼 사용불가
		break;
	}
}
