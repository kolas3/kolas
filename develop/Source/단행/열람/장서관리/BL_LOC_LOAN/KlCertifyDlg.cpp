#include "stdafx.h"
#include "KlCertifyDlg.h"
#include "Imm.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKlCertifyDlg dialog


CKlCertifyDlg::CKlCertifyDlg(CESL_Mgr* pParent /*=NULL*/, INT nMode/*=0*/)
	: CESL_Mgr(CKlCertifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKlCertifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strName.Empty();
	m_strCivilNo.Empty();
	m_strUserNo.Empty();
	m_bIsRunningThread = FALSE;

	m_strFailCivilNo.Empty();
	m_strSuccessCivilNo.Empty();
	m_nMode = nMode;
}
CKlCertifyDlg::~CKlCertifyDlg()
{
	
}

void CKlCertifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKlCertifyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKlCertifyDlg, CDialog)
	//{{AFX_MSG_MAP(CKlCertifyDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnKLCertify, OnbtnKLCertify)
	ON_BN_CLICKED(IDC_btnKLCertifyClose, OnbtnKLCertifyClose)
	ON_EN_CHANGE(IDC_edtKLCertifyCvilNo1, OnChangeedtKLCertifyCvilNo1)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKlCertifyDlg message handlers

BOOL CKlCertifyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			OnbtnKLCertifyClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN)
		{
			CString strGetData;
			if(GetDlgItem(IDC_btnKLCertify)->m_hWnd == pMsg->hwnd)
			{
				OnbtnKLCertify();
			}
			else if(GetDlgItem(IDC_btnKLCertifyClose)->m_hWnd == pMsg->hwnd)
			{
				OnbtnKLCertifyClose();
			}
			else if(GetDlgItem(IDC_edtKLCertifyName)->m_hWnd == pMsg->hwnd)
			{
				GetDlgItem(IDC_edtKLCertifyName)->GetWindowText(strGetData);
				if(0 < strGetData.GetLength())
				{
					GetDlgItem(IDC_edtKLCertifyCvilNo1)->GetWindowText(strGetData);
					if(6 == strGetData.GetLength())
					{
						GetDlgItem(IDC_edtKLCertifyCvilNo2)->GetWindowText(strGetData);
						if(7 == strGetData.GetLength())
						{
							OnbtnKLCertify();
						}
						else
						{
							GetDlgItem(IDC_edtKLCertifyCvilNo2)->SetFocus();
						}
					}
					else
					{
						GetDlgItem(IDC_edtKLCertifyCvilNo1)->SetFocus();
					}
				}
			}
			else if(GetDlgItem(IDC_edtKLCertifyCvilNo1)->m_hWnd == pMsg->hwnd)
			{
				GetDlgItem(IDC_edtKLCertifyCvilNo1)->GetWindowText(strGetData);
				if(6 == strGetData.GetLength())
				{
					GetDlgItem(IDC_edtKLCertifyCvilNo2)->GetWindowText(strGetData);
					if(7 == strGetData.GetLength())
					{
						OnbtnKLCertify();
					}
					else
					{
						GetDlgItem(IDC_edtKLCertifyCvilNo2)->SetFocus();
					}
				}
			}
			else if(GetDlgItem(IDC_edtKLCertifyCvilNo2)->m_hWnd == pMsg->hwnd)
			{
				GetDlgItem(IDC_edtKLCertifyCvilNo2)->GetWindowText(strGetData);
				if(7 == strGetData.GetLength())
				{
					OnbtnKLCertify();
				}
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_BACK)
		{
			if(GetDlgItem(IDC_edtKLCertifyCvilNo2)->m_hWnd == pMsg->hwnd)
			{
				CString strGetData;
				GetDlgItem(IDC_edtKLCertifyCvilNo2)->GetWindowText(strGetData);
				if(0 == strGetData.GetLength())
				{
					GetDlgItem(IDC_edtKLCertifyCvilNo1)->SetFocus();
					return TRUE;
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CKlCertifyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKLCertifyFailMassage == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

BOOL CKlCertifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CEdit*)GetDlgItem(IDC_edtKLCertifyCvilNo1))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_edtKLCertifyCvilNo2))->SetLimitText(7);

	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_edtKLCertifyName)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence);
        ImmReleaseContext(GetDlgItem(IDC_edtKLCertifyName)->m_hWnd,hIME); 
    }

	CString strMsg = _T("");
	if(0 == m_nMode)
	{
		strMsg = _T("회원증 미지참자의 신분 확인을 위하여\r\n본인인증을 수행합니다.");
	}
	else
	{
		strMsg = _T("책이음회원 반입 이용자의 신분 확인을 위하여\r\n본인인증을 수행합니다.");
	}
	
	GetDlgItem(IDC_stcKLCertifyMassage2)->SetWindowText(strMsg);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKlCertifyDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;

	OnCancel();
	
	CDialog::OnClose();
}

void CKlCertifyDlg::OnbtnKLCertify() 
{
	if(TRUE == m_bIsRunningThread) return;

	if(0 != CheckData()) return;
	SetTimer(3, 200, NULL);
}

void CKlCertifyDlg::OnbtnKLCertifyClose() 
{
	OnClose();
}

void CKlCertifyDlg::OnChangeedtKLCertifyCvilNo1() 
{
	CString strText;
	GetDlgItemText(IDC_edtKLCertifyCvilNo1, strText);
	if(6 == strText.GetLength()) GetDlgItem(IDC_edtKLCertifyCvilNo2)->SetFocus();
}

INT CKlCertifyDlg::CheckData()
{
	m_strName.Empty();
	m_strCivilNo.Empty();

	m_strSuccessCivilNo.Empty();
	m_strFailCivilNo.Empty();

	CString strName;
	GetDlgItemText(IDC_edtKLCertifyName, strName);
	strName.TrimLeft();	strName.TrimRight();
	if(strName.IsEmpty())
	{
		GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("이름을 입력해주세요."));
		return 1;
	}
	CString strCivilNo1;
	GetDlgItemText(IDC_edtKLCertifyCvilNo1, strCivilNo1);
	if(6 != strCivilNo1.GetLength())
	{
		GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("주민번호 앞자리는 6자리입니다."));
		GetDlgItem(IDC_edtKLCertifyCvilNo1)->SetFocus();
		return 2;
	}
	CString strCivilNo2;
	GetDlgItemText(IDC_edtKLCertifyCvilNo2, strCivilNo2);
	if(7 != strCivilNo2.GetLength())
	{
		GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("주민번호 뒷자리는 7자리입니다."));
		GetDlgItem(IDC_edtKLCertifyCvilNo2)->SetFocus();
		return 3;
	}
	CString strCivilNo = strCivilNo1 + _T("-") + strCivilNo2;
	INT ids = CLocCommonAPI::CivilNoCheck(strCivilNo);
	if(0 != ids)
	{
		GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("올바른 주민등록번호를 입력해주십시오"));
		GetDlgItem(IDC_edtKLCertifyCvilNo2)->SetFocus();
		return 4;
	}

	m_strName = strName;
	m_strCivilNo = strCivilNo;

	return 0;
}

INT CKlCertifyDlg::StartCertify()
{
	m_strUserNo.Empty();

	GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("본인인증을 시작합니다."));

	CESL_DataMgr TmpDM;
	INT ids = CLocCommonAPI::KL_RealNameCertification(this, m_strName, m_strCivilNo, TmpDM);
	if(0 != ids)
	{
		CString strMsg;
		if(-1010 == ids)
		{
			CString strTemp;
			TmpDM.GetCellData(0, 0, strTemp);
			if(-1 < strTemp.Find(_T("SY04")))
			{
				strMsg.Format(_T("이름, 주민번호가 불일치합니다."));
			}
			else if(-1 < strTemp.Find(_T("SY02")))
			{
				strMsg.Format(_T("이름을 확인하십시오."));
			}
			else if(-1 < strTemp.Find(_T("SY06")))
			{
				strMsg.Format(_T("명의도용 차단한 이용자입니다."));
			}
			else
			{
				strMsg.Format(_T("미들웨어 장애가 발생하였습니다."));
			}
			GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(strMsg);
		}
		else
		{
			GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("미들웨어 접속에 실패하였습니다."));
		}

		m_strFailCivilNo = m_strCivilNo;

		return 1;
	}

	m_strSuccessCivilNo = m_strCivilNo;

	TmpDM.GetCellData(_T("통합대출자번호"), 0, m_strUserNo);

	GetDlgItem(IDC_stcKLCertifyFailMassage)->SetWindowText(_T("본인인증에 성공하였습니다."));

	return 0;
}

CString CKlCertifyDlg::GetUserNo()
{
	return m_strUserNo;
}

void CKlCertifyDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(0);
		OnOK();
	}
	else if(1 == nIDEvent)
	{
		KillTimer(1);
		OnbtnKLCertifyClose();
	}
	else if(3 == nIDEvent)
	{
		KillTimer(3);
		AfxBeginThread(CertifyThread, this);
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CKlCertifyDlg::CertifyThread(LPVOID pParam)
{
	CKlCertifyDlg* pDlg = (CKlCertifyDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->StartCertify();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 == ids) pDlg->SetTimer(0, 500, NULL);
	return 0;
}
