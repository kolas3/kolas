#include "stdafx.h"
#include "CardpasswdDlg.h"

#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCardpasswdDlg::CCardpasswdDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCardpasswdDlg::IDD, pParent)
{
	
	m_strInputData = _T("");
	m_strMsg = _T("[비밀번호를 입력하세요]");
	m_nStatus = 0;
	
	m_bReg = FALSE;
	m_nDiscord = 0;
	m_strCardpasswd = _T("");

	
	m_pParent = pParent;
}

CCardpasswdDlg::~CCardpasswdDlg()
{	
}
VOID CCardpasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_MSG_MAP(CCardpasswdDlg)
	DDX_Text(pDX, IDC_EDT_CARDPASSWD, m_strInputData);
	DDX_Text(pDX, IDC_SttMSG_CARDPASSWD, m_strMsg);	
	DDV_MaxChars(pDX, m_strInputData, 20);
	//}}AFX_MSG_MAP
}


BEGIN_MESSAGE_MAP(CCardpasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CCardpasswdDlg)
	ON_BN_CLICKED(IDC_btnREG_CARDPASSWD, OnReg)
	ON_BN_CLICKED(IDOK_CARDPASSWD, OnConfirm)
	ON_BN_CLICKED(IDC_btnIgnore, OnIgnore)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDT_CARDPASSWD, OnChangeEdtCardpasswd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CCardpasswdDlg::OnReg() 
{
	if( m_bReg == TRUE )
	{
		return;
	}
	else
	{
		if( CheckCardpasswd() == FALSE )	return;

		m_bReg = TRUE;

		GetDlgItem(IDC_btnREG_CARDPASSWD)->EnableWindow(FALSE);
		GetDlgItem(IDOK_CARDPASSWD)->EnableWindow(TRUE);

		m_strCardpasswd = m_strInputData;
		
		if(TRUE == m_pParent->m_pInfo->m_bCardPwEncryption && 64 > m_strCardpasswd.GetLength())

		{
			m_strCardpasswd = CLocCommonAPI::StringToSHA256(m_strCardpasswd);
		}

		m_strInputData.Empty();

		m_strMsg = _T("[변경된 비밀번호를 한번 더 입력하세요]");
		GetDlgItem(IDC_EDT_CARDPASSWD)->SetFocus();
		UpdateData(FALSE);

		m_nStatus = 1;
	}
}

VOID CCardpasswdDlg::OnConfirm()  
{
	if( CheckCardpasswd() == FALSE )	return;	
	
	if(TRUE == m_pParent->m_pInfo->m_bCardPwEncryption && 64 > m_strInputData.GetLength())
	{
		m_strInputData = CLocCommonAPI::StringToSHA256(m_strInputData);
	}

	if( m_strCardpasswd != m_strInputData )
	{		
		m_nDiscord++;
		m_strMsg.Format(_T("[비밀번호가 일치하지 않습니다(%d/3)]"), m_nDiscord);

		m_strInputData.Empty();

		if( m_nDiscord == 3 )
		{
			m_nDiscord = 0;

			if( IDYES == AfxMessageBox(_T("비밀번호가 일치하지 않습니다.\n\n새로운 비밀번호로 등록하시겠습니까?"), MB_ICONQUESTION | MB_YESNO ) )
			{
				GetDlgItem(IDC_btnREG_CARDPASSWD)->EnableWindow(TRUE);
				GetDlgItem(IDOK_CARDPASSWD)->EnableWindow(FALSE);
				
				m_strMsg = _T("[새로운 비밀번호를 입력하세요]");
				SetDlgItemText(IDC_btnREG_CARDPASSWD, _T("입력"));
			}
			else
			{
				m_strMsg = _T("[비밀번호를 입력하세요]");
			}
		}

		UpdateData(FALSE);
	}
	else	
	{
		m_strMsg = _T("[비밀번호를 확인하였습니다]");
		
		
		if(FALSE == m_pParent->m_pInfo->m_bCardPwEncryption)
		{
			UpdateData(FALSE);
		}
		CDialog::OnOK();
	}
}

BOOL CCardpasswdDlg::CheckCardpasswd()
{
	UpdateData(TRUE);
	
	INT		nLength		=	m_strInputData.GetLength();
	if( 4 > nLength || 20 < nLength )
	{
		AfxMessageBox(_T("회원증 비밀번호는 4자리 이상, 20자리 이하로 입력하여 주십시요."), MB_ICONWARNING);
		m_strInputData.Empty();
		UpdateData(FALSE);
		
		return FALSE;
	}

	return TRUE;
}

BOOL CCardpasswdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nStatus = 0;
	m_nDiscord = 0;
	m_bReg = FALSE;

	if( m_strCardpasswd.IsEmpty() == TRUE )
	{
		m_strMsg = _T("[새로운 비밀번호를 입력하세요]");
		SetDlgItemText(IDC_SttMSG_CARDPASSWD, m_strMsg);

		GetDlgItem(IDC_btnREG_CARDPASSWD)->EnableWindow(TRUE);
		GetDlgItem(IDOK_CARDPASSWD)->EnableWindow(FALSE);

		SetDlgItemText(IDC_btnREG_CARDPASSWD, _T("입력"));
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	AfxGetMainWnd()->SetForegroundWindow();

	return TRUE;
}

BOOL CCardpasswdDlg::PreTranslateMessage(MSG* pMsg) 
{

	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnCancel();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CCardpasswdDlg::OnIgnore() 
{
	CDialog::OnOK();
}

HBRUSH CCardpasswdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

void CCardpasswdDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

void CCardpasswdDlg::OnChangeEdtCardpasswd() 
{
	
}
