#include "stdafx.h"
#include "KlEditPassDlg.h"
#include "FrameManager.h"

#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CKlEditPassDlg::CKlEditPassDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlEditPassDlg::IDD, pParent)
{
		
	m_strUserNo = _T("");

	m_strCtlPASS = _T("");
	m_bIsRunningThread = FALSE;
	
	m_nMode = 0;
	m_strUserKey = _T("");

	
	m_pParent = (CBO_LOC_3320*)pParent;
}
CKlEditPassDlg::~CKlEditPassDlg()
{
}

void CKlEditPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKlEditPassDlg) 
	//}}AFX_DATA_MAP		
	
}


BEGIN_MESSAGE_MAP(CKlEditPassDlg, CDialog)
	//{{AFX_MSG_MAP(CKlEditPassDlg)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnKEP_EDIT, OnbtnKEPEDIT)
	ON_BN_CLICKED(IDC_btnKEP_CANCEL, OnbtnKEPCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CKlEditPassDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKEPCANCEL();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKEP_EDIT)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKEPEDIT();
			}
			else if(GetDlgItem(IDC_btnKEP_CANCEL)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKEPCANCEL();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CKlEditPassDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}

HBRUSH CKlEditPassDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKEP_MSG == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

BOOL CKlEditPassDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	
	((CEdit*)GetDlgItem(IDC_edtKEP_PASS1))->SetLimitText(20);
	((CEdit*)GetDlgItem(IDC_edtKEP_PASS2))->SetLimitText(20);
	if(2 == m_nMode)
	{
		((CEdit*)GetDlgItem(IDC_edtKEP_PASS2))->ModifyStyle(ES_NUMBER, 0);
	}	
	return TRUE;  	              
}

void CKlEditPassDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(nIDEvent);
		SetDlgItemText(IDC_stcKEP_MSG, _T("비밀번호 수정중입니다."));
		AfxBeginThread(SavePassThread, this);
	}
	else if(101 == nIDEvent)
	{
		KillTimer(nIDEvent);
		SetDlgItemText(IDC_stcKEP_MSG, _T("수정에 실패하였습니다."));
	}
	else if(102 == nIDEvent)
	{
		KillTimer(nIDEvent);
		AfxMessageBox(_T("수정되었습니다."));
		OnOK();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKlEditPassDlg::OnbtnKEPEDIT() 
{
	
	if(TRUE == m_bIsRunningThread) return;

	CString strPass1;
	GetDlgItemText(IDC_edtKEP_PASS1, strPass1);
	strPass1.Replace(_T(" "), _T(""));
	if(strPass1.IsEmpty())
	{
		SetDlgItemText(IDC_stcKEP_MSG, _T("비밀번호를 입력해 주십시오."));
		((CEdit*)GetDlgItem(IDC_edtKEP_PASS1))->SetFocus();
		return;
	}
	CString strPass2;
	GetDlgItemText(IDC_edtKEP_PASS2, strPass2);
	strPass2.Replace(_T(" "), _T(""));
	if(strPass2.IsEmpty())
	{
		SetDlgItemText(IDC_stcKEP_MSG, _T("비밀번호확인을 입력해 주십시오."));
		((CEdit*)GetDlgItem(IDC_edtKEP_PASS2))->SetFocus();
		return;
	}
	if(strPass1 != strPass2)
	{
		SetDlgItemText(IDC_stcKEP_MSG, _T("비밀번호를 정확하게 입력해 주십시오."));
		((CEdit*)GetDlgItem(IDC_edtKEP_PASS2))->SetFocus();
		return;
	}

	m_strCtlPASS = strPass1;

	
	if(0 == m_nMode)
	{
		SetTimer(1, 100, NULL);
	}	
	
	else if(1 == m_nMode)
	{	
		
		if(m_pInfo->m_bVisitorPwEncryption)
		{
			m_strCtlPASS = CLocCommonAPI::StringToSHA256(m_strCtlPASS);
		}
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		CString strQuery;
		strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET PASSWORD = '%s' WHERE REC_KEY = %s;"), m_strCtlPASS, m_strUserKey);
		dm.StartFrame();
		dm.AddFrame(strQuery);
		dm.SendFrame();
		dm.EndFrame();
		AfxMessageBox(_T("수정되었습니다."));

		
		m_pParent->m_strCryptoPassword = m_strCtlPASS;
		
		OnOK();
	}
	
	else if(2 == m_nMode)
	{			
		if(m_pInfo->m_bCardPwEncryption)
		{
			m_strCtlPASS = CLocCommonAPI::StringToSHA256(m_strCtlPASS);
		}
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		CString strQuery;
		strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET CARD_PASSWORD = '%s' WHERE REC_KEY = %s;"), m_strCtlPASS, m_strUserKey);
		dm.StartFrame();
		dm.AddFrame(strQuery);
		dm.SendFrame();
		dm.EndFrame();
		AfxMessageBox(_T("수정되었습니다."));
		
		m_pParent->m_strCryptoCardPassword = m_strCtlPASS;
		
		OnOK();
	}
	
}

void CKlEditPassDlg::OnbtnKEPCANCEL() 
{
	if(TRUE == m_bIsRunningThread) return;

	OnCancel();
}

UINT CKlEditPassDlg::SavePassThread(LPVOID pParam)
{
EFS_BEGIN

	CKlEditPassDlg* pDlg = (CKlEditPassDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->SavePass();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		return -1;
	}
	else if(0 < ids)
	{
		pDlg->SetTimer(ids, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(102, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

INT CKlEditPassDlg::SavePass()
{
EFS_BEGIN
	
	INT ids;
	CString strMsg;
	CESL_DataMgr TempDM;
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	CFrameWriter Writer(m_pInfo);
	CString strCommand = _T("IL_K40_LOC_B01_SERVICE");
	Writer.setCommand(strCommand);
	Writer.addRecord();
	
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);

	Writer.addElement(_T("통합비밀번호"), m_strCtlPASS);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		strMsg.Format(_T("통합아이디비밀번호수정(40) ERROR CODE : %d"), ids);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		return 101;
	}
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strTemp;
		Reader.getError(strTemp, strMsg);
		if(strMsg.IsEmpty())
		{
			strTemp = _T("null");
		}
		strMsg.Format(_T("통합아이디비밀번호수정(40) ERROR CODE : %s"), strTemp);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		return 101;
	}
	if(1 > TempDM.GetRowCount())
	{
		TempDM.QueryLog(_T("통합아이디비밀번호수정(40) ERROR (CNT=0)"));
		return 101;
	}
	CString strGetData;
	TempDM.GetCellData(_T("처리결과"), 0, strGetData);
	if(strGetData.IsEmpty()) return 101;
	if(_T("OK") != strGetData)
	{
		return 101;
	}
	return 0;
	
EFS_END
return -1;
}
