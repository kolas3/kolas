// KL_UserImportSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_UserImportSetDlg.h"
#include "LibListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_UserImportSetDlg dialog


CKL_UserImportSetDlg::CKL_UserImportSetDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_UserImportSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_UserImportSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoan = NULL;
	m_bIsRunningThread = FALSE;
	m_strLibCode = _T("");
	m_strLibName = _T("");
	m_strUserNo = _T("");
}
CKL_UserImportSetDlg::~CKL_UserImportSetDlg()
{
}

void CKL_UserImportSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_UserImportSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_UserImportSetDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserImportSetDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnKUIS_LibCode, OnbtnKUISLibCode)
	ON_BN_CLICKED(IDC_btnKUIS_Import, OnbtnKUISImport)
	ON_BN_CLICKED(IDC_btnKUIS_Close, OnbtnKUISClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_UserImportSetDlg message handlers

BOOL CKL_UserImportSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKUIS_LibCode)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUISLibCode();
			}
			else if(GetDlgItem(IDC_btnKUIS_Import)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUISImport();
			}
			else if(GetDlgItem(IDC_btnKUIS_Close)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUISClose();
			}
			else if(GetDlgItem(IDC_edtKUIS_UserNo)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUISImport();
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKUISClose();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKL_UserImportSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_UserImportSetDlg::OnTimer(UINT nIDEvent) 
{
	CString strMsg;
	if(0 == nIDEvent)
	{
		KillTimer(0);
		AfxBeginThread(ImportThread, this);
	}
	else if(1 == nIDEvent)
	{
		KillTimer(1);
		AfxMessageBox(_T("반입에 성공하였습니다."), MB_OK|MB_ICONINFORMATION);
		OnOK();
	}
	else if(2 == nIDEvent)
	{
		KillTimer(2);
		AfxMessageBox(_T("반입이 취소되었습니다."), MB_OK|MB_ICONINFORMATION);
		OnCancel();
	}
	else if(3 == nIDEvent)
	{
		KillTimer(3);
		strMsg = _T("도서관이 선택되지 않았습니다.\r\n도서관을 선택하십시오.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUIS_LibCode)->SetFocus();
	}
	else if(4 == nIDEvent)
	{
		KillTimer(4);
		strMsg = _T("대출자번호가 입력되지 않았습니다.\r\n대출자번호를 입력하십시오.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_edtKUIS_UserNo)->SetFocus();
	}
	else if(5 == nIDEvent)
	{
		KillTimer(5);
		strMsg = _T("미들웨어 접속 시도중입니다.\r\n잠시후에 재시도 하십시오.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUIS_Import)->SetFocus();
	}
	else if(6 == nIDEvent)
	{
		KillTimer(6);
		strMsg = _T("이용자 조회에 실패하였습니다.\r\n반입 버튼을 눌러 재시도 하십시오.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUIS_Import)->SetFocus();
	}
	else if(7 == nIDEvent)
	{
		KillTimer(7);
		strMsg = _T("조회된 이용자가 없습니다.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_edtKUIS_UserNo)->SetFocus();
	}
	else if(8 == nIDEvent)
	{
		KillTimer(8);
		strMsg = _T("2명이상의 이용자가 조회되어\r\n반입을 할 수 없습니다.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUIS_Import)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUIS_Close)->SetFocus();
	}
	else if(9 == nIDEvent)
	{
		KillTimer(9);
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(_T("개인정보 제3자제공에 대한 이용자의 동의를 확인하십시오."));
	}
	else if(100 == nIDEvent)
	{
		KillTimer(100);
		strMsg = _T("시스템에 문제가 발생하였습니다.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUIS_Import)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUIS_Close)->SetFocus();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKL_UserImportSetDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}

void CKL_UserImportSetDlg::OnbtnKUISLibCode() 
{
	if(TRUE == m_bIsRunningThread) return;
	LibSearch();
}

void CKL_UserImportSetDlg::OnbtnKUISImport() 
{
	if(TRUE == m_bIsRunningThread) return;

	if(TRUE != ((CButton*)GetDlgItem(IDC_chkKUIS_Confirm))->GetCheck())
	{
		SetTimer(9, 100, NULL);
		return;
	}

	SetTimer(0, 100, NULL);
}

void CKL_UserImportSetDlg::OnbtnKUISClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	OnCancel();	
}

INT CKL_UserImportSetDlg::LibSearch()
{
	m_bIsRunningThread = TRUE;
	
	CLibListDlg dlg(this, m_pLoan);
	dlg.SetMode(2);
	if(IDOK == dlg.DoModal())
	{
		SetLibCodeControl(dlg.m_strLibCode, dlg.m_strLibName);
	}

	m_bIsRunningThread = FALSE;

	return 0;
}

VOID CKL_UserImportSetDlg::SetLibCodeControl(CString strLibCode, CString strLibName)
{
	CString strValue;
	strValue.Format(_T("%s(%s)"), strLibCode, strLibName);
	SetDlgItemText(IDC_edtKUIS_LibCode, strValue);
	m_strLibCode = strLibCode;
	m_strLibName = strLibName;
	CString strUserNo;
	GetDlgItem(IDC_edtKUIS_UserNo)->GetWindowText(strUserNo);
	CString strMsg;
	GetDlgItem(IDC_btnKUIS_Import)->EnableWindow(TRUE);
	if(strUserNo.IsEmpty())
	{
		strMsg = _T("반입할 이용자의 대출자번호를 입력하세요.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_edtKUIS_UserNo)->SetFocus();
	}
	else
	{
		strMsg = _T("반입 버튼을 선택하시면 반입을 시작합니다.");
		GetDlgItem(IDC_stcKUIS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUIS_Import)->SetFocus();
	}

	return;
}

HBRUSH CKL_UserImportSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKUIS_Status == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

UINT CKL_UserImportSetDlg::ImportThread(LPVOID pParam)
{
	CKL_UserImportSetDlg* pDlg = (CKL_UserImportSetDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->Import();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 == ids) 
	{
		pDlg->SetTimer(1, 100, NULL);
	}
	else if(1 == ids)
	{
		pDlg->SetTimer(2, 100, NULL);
	}
	else if(10 == ids)
	{
		pDlg->SetTimer(3, 100, NULL);
	}
	else if(11 == ids)
	{
		pDlg->SetTimer(4, 100, NULL);
	}
	else if(100 == ids)
	{
		pDlg->SetTimer(5, 100, NULL);
	}
	else if(101 == ids)
	{
		pDlg->SetTimer(6, 100, NULL);
	}
	else if(102 == ids)
	{
		pDlg->SetTimer(7, 100, NULL);
	}
	else if(103 == ids)
	{
		pDlg->SetTimer(8, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(100, 100, NULL);
	}

	return 0;
}

INT CKL_UserImportSetDlg::Import()
{
	INT ids = 0;
	if(m_strLibCode.IsEmpty() || m_strLibName.IsEmpty())
	{
		return 10;
	}
	CString strUserNo;
	GetDlgItem(IDC_edtKUIS_UserNo)->GetWindowText(strUserNo);
	if(strUserNo.IsEmpty())
	{
		return 11;
	}
	ids = m_pLoan->KL_OtherLibUserImport(m_strLibCode, m_strLibName, strUserNo);
	m_strUserNo = strUserNo;

	return ids;
}
