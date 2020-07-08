// InputReservatoinExpireDay.cpp : implementation file
//

#include "stdafx.h"
#include "InputReservatoinExpireDay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputReservatoinExpireDay dialog


CInputReservatoinExpireDay::CInputReservatoinExpireDay(CESL_Mgr* pParent /*=NULL*/,CString sTitle)
	: CESL_Mgr(CInputReservatoinExpireDay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputReservatoinExpireDay)
	m_nDay = 0;
	//}}AFX_DATA_INIT

	m_sTitle = sTitle;

	m_bAllApply = FALSE;
}

CInputReservatoinExpireDay::~CInputReservatoinExpireDay()
{
}

VOID CInputReservatoinExpireDay::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputReservatoinExpireDay)
	DDX_Text(pDX, IDC_edtINPUT_RESERVATOIN_EXPIRE_DAY, m_nDay);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CInputReservatoinExpireDay, CDialog)
	//{{AFX_MSG_MAP(CInputReservatoinExpireDay)
	ON_BN_CLICKED(IDC_btnOK, OnbtnOK)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputReservatoinExpireDay message handlers

BOOL CInputReservatoinExpireDay::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if( pMsg->message == WM_KEYDOWN ) 
	{
		if( pMsg->wParam == VK_ESCAPE )
		{
			return FALSE;
		}
		else if( pMsg->wParam == VK_RETURN )
		{
			OnbtnOK();
			return TRUE;	
		}
	}

	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  		
    {		
        if (::GetKeyState(VK_MENU) < 0) {
			return TRUE;
        }
    }
	
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

BOOL CInputReservatoinExpireDay::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	UpdateData(FALSE);

	SetWindowText( m_sTitle );

	if( m_sTitle.Compare(_T("반납 연기일 입력")) == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkALL_APPLY))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_btnLOAN_CANCEL))->SetWindowText( _T("자동부여") );

		CRect WndPos;
		GetWindowRect(WndPos);
		MoveWindow(WndPos.left , WndPos.top , WndPos.Width() , WndPos.Height() - 22 );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());  

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CInputReservatoinExpireDay::OnbtnOK() 
{
	EFS_BEGIN

	UpdateData(TRUE);

	if( 0 == m_sTitle.Compare(_T("반납 연기일 입력")) )
	{
		if( m_nDay < 0 || m_nDay > 30 )
		{
			AfxMessageBox( _T("0에서 30사이의 정수를 입력하십시오.") );
			return;
		}
	}
	else
	{
		if( m_nDay < 0 || m_nDay > 30 )
		{
			AfxMessageBox( _T("0에서 30사이의 정수를 입력하십시오.") );
			return;
		}
	}


	if( ((CButton*)GetDlgItem(IDC_chkALL_APPLY))->GetCheck() == 1 )
		m_bAllApply = TRUE;
	else
		m_bAllApply = FALSE;
	OnOK();

	EFS_END
}

VOID CInputReservatoinExpireDay::SetDay( INT nDay )
{
	EFS_BEGIN

	m_nDay = nDay;

	EFS_END
}

VOID CInputReservatoinExpireDay::GetDay( INT &nDay )
{
	EFS_BEGIN

	nDay = m_nDay;

	EFS_END
}


bool CInputReservatoinExpireDay::GetAllApply()
{
	EFS_BEGIN

	return m_bAllApply;

	EFS_END

}

VOID CInputReservatoinExpireDay::OnbtnCANCEL() 
{
	if( m_sTitle.Compare(_T("반납 연기일 입력")) == 0 )
	{
		m_nDay = 0;
		OnCancel();
	}
	else
	{
		OnCancel();
	}
}

HBRUSH CInputReservatoinExpireDay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
