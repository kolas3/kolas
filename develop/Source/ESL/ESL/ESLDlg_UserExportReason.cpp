// ESLDlg_UserExportReason.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ESLDlg_UserExportReason.h"
//#include "afxdialogex.h"
#include "imm.h"

// CESLDlg_UserExportReason 대화 상자입니다.

IMPLEMENT_DYNAMIC(CESLDlg_UserExportReason, CDialog)

CESLDlg_UserExportReason::CESLDlg_UserExportReason(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_UserExportReason::IDD, pParent)
{

}

CESLDlg_UserExportReason::~CESLDlg_UserExportReason()
{
}

void CESLDlg_UserExportReason::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CESLDlg_UserExportReason, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDT_USER_EXPORT_REASON, OnSetfocusEdtUserExportReason)
END_MESSAGE_MAP()


// CESLDlg_UserExportReason 메시지 처리기입니다.


void CESLDlg_UserExportReason::OnBnClickedOk()
{
	GetDlgItem(IDC_EDT_USER_EXPORT_REASON)->GetWindowText(m_strReason);
	if(m_strReason.IsEmpty() == TRUE)
	{
		CString strMsg;
		strMsg.Format(_T("출력을 하시려면 사유를 적어주셔야 합니다."));
		AfxMessageBox(strMsg);
		{
			return;
		}
	}
	else if (m_strReason.GetLength() < 3)
	{
		CString strMsg;
		strMsg.Format(_T("3글자 이상 입력해주시기 바랍니다."));
		AfxMessageBox(strMsg);
		return;
		
	}

	wchar_t c;
	for (INT i = 0; i < m_strReason.GetLength() ; i++)
	{
		c = m_strReason.GetAt(i);
		if( c > 32 && c < 127 
			&& !((c >= 'a' && c <= 'z') || (c >= 'A'&& c <= 'Z'))
			&& !( c >= '0' && c <= '9') ) 
		{
			CString strMsg;
			strMsg.Format(_T("특수문자는 입력할 수 없습니다."));
			AfxMessageBox(strMsg);
			return;
		}
	}
	
	CDialog::OnOK();
}


void CESLDlg_UserExportReason::OnSetfocusEdtUserExportReason()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
	
    hIME = ::ImmGetContext(GetDlgItem(IDC_EDT_USER_EXPORT_REASON)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
        ImmReleaseContext(GetDlgItem(IDC_EDT_USER_EXPORT_REASON)->m_hWnd,hIME); 
    }
}
