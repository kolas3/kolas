// UNION_RETURN_STATICS_RUN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_STATICS_RUN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_STATICS_RUN dialog


CUNION_RETURN_STATICS_RUN::CUNION_RETURN_STATICS_RUN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pdReturnDateStart = NULL;
	m_pdReturnDateEnd   = NULL;
	m_nStaticsMode     = 0;	
}

CUNION_RETURN_STATICS_RUN::~CUNION_RETURN_STATICS_RUN()
{
	if( NULL != m_pdReturnDateStart )
	{
		delete m_pdReturnDateStart;
		m_pdReturnDateStart = NULL;
	}
	if( NULL != m_pdReturnDateEnd )
	{
		delete m_pdReturnDateEnd;
		m_pdReturnDateEnd = NULL;
	}
}


void CUNION_RETURN_STATICS_RUN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_STATICS_RUN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_STATICS_RUN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_STATICS_RUN)
	ON_BN_CLICKED(IDC_BTN_STATICS, OnBtnStatics)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_STATICS_RUN message handlers

BOOL CUNION_RETURN_STATICS_RUN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	// 기본값 설정
	CButton* pBtn = (CButton*)GetDlgItem(IDC_RAD_REQUEST_RETURN);
	pBtn->SetCheck(BST_CHECKED);

	// 컨트롤 설정
	//CECO_ControlDateEdit* m_pdReturnDateEnd;
	if( NULL == m_pdReturnDateStart )
	{
		m_pdReturnDateStart = new CECO_ControlDateEdit(NULL);
		m_pdReturnDateStart->SubclassDlgItem(IDC_DAT_START, this);
		m_pdReturnDateStart->m_ControlStyle = 0;
		m_pdReturnDateStart->InitEdit();
	}

	if( NULL == m_pdReturnDateEnd )
	{
		m_pdReturnDateEnd = new CECO_ControlDateEdit(NULL);
		m_pdReturnDateEnd->SubclassDlgItem(IDC_DAT_END, this);
		m_pdReturnDateEnd->m_ControlStyle = 0;
		m_pdReturnDateEnd->InitEdit();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUNION_RETURN_STATICS_RUN::OnBtnStatics() 
{
	// 반납일 입력값 확인
	CString strReturnDateStart;
	m_pdReturnDateStart->GetData(strReturnDateStart);

	if( strReturnDateStart.IsEmpty() )
	{
		AfxMessageBox(_T("반납일을 입력해야 합니다."));
		return;
	}

	// 선택한 값을 멤버에 저장한다.
	// 통계 종류
	CButton* pBtn = (CButton*)GetDlgItem(IDC_RAD_REQUEST_RETURN);
	m_nStaticsMode = ( BST_CHECKED == pBtn->GetCheck() ) ? 0 : 1;
	
	m_pdReturnDateStart->GetData(m_strReturnDateStart);
	m_pdReturnDateEnd->GetData(m_strReturnDateEnd);

	if( m_strReturnDateEnd.IsEmpty() )
	{
		// 2009.03.02 UPDATE BY KSJ : 날짜 검색 조건을 입력 안 했을 경우, 당일 날짜까지 검색
		//m_strReturnDateEnd = m_strReturnDateStart;
		SYSTEMTIME time;
		GetLocalTime(&time);

		m_strReturnDateEnd.Format( _T("%04d/%02d/%02d") , time.wYear , time.wMonth , time.wDay );
	}

	OnOK();
}

void CUNION_RETURN_STATICS_RUN::OnBtnClose() 
{
	OnCancel();	
}

BOOL CUNION_RETURN_STATICS_RUN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{			
			OnBtnStatics();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
