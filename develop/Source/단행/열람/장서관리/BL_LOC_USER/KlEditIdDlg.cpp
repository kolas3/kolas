#include "stdafx.h"
#include "KlEditIdDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKlEditIdDlg::CKlEditIdDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlEditIdDlg::IDD, pParent)
{
	m_strUserNo = _T("");
	m_strGetID = _T("");
	m_strCtlID = _T("");
	m_bIsRunningThread = FALSE;
}
CKlEditIdDlg::~CKlEditIdDlg()
{
}

void CKlEditIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKlEditIdDlg) 
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CKlEditIdDlg, CDialog)
	//{{AFX_MSG_MAP(CKlEditIdDlg)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnKEI_CANCEL, OnbtnKEICANCEL)
	ON_BN_CLICKED(IDC_btnKEI_CHECK, OnbtnKEICHECK)
	ON_BN_CLICKED(IDC_btnKEI_EDIT, OnbtnKEIEDIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CKlEditIdDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKEICANCEL();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKEI_CHECK)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKEICHECK();
			}
			else if(GetDlgItem(IDC_btnKEI_CANCEL)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKEICANCEL();
			}
			else if(GetDlgItem(IDC_btnKEI_EDIT)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKEIEDIT();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CKlEditIdDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}

HBRUSH CKlEditIdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKEI_MSG == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

BOOL CKlEditIdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	((CEdit*)GetDlgItem(IDC_edtKEI_ID))->SetLimitText(16);
	GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(FALSE);
	
	return TRUE;  
	              
}

void CKlEditIdDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_stcKEI_MSG, _T("아이디 중복 확인중입니다."));
		AfxBeginThread(CheckIdThread, this);
	}
	else if(101 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_stcKEI_MSG, _T("중복확인에 실패하였습니다."));
	}
	else if(102 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_stcKEI_MSG, _T("이미 사용중인 아이디입니다."));
	}
	else if(103 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(TRUE);
		SetDlgItemText(IDC_stcKEI_MSG, _T("사용 가능한 아이디입니다."));
	}
	else if(2 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(TRUE);
		SetDlgItemText(IDC_stcKEI_MSG, _T("아이디 수정중입니다."));
		AfxBeginThread(SaveIdThread, this);
	}
	else if(201 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(TRUE);
		SetDlgItemText(IDC_stcKEI_MSG, _T("수정에 실패하였습니다."));
	}
	else if(202 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKEI_EDIT)->EnableWindow(TRUE);
		AfxMessageBox(_T("수정되었습니다."));
		OnOK();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKlEditIdDlg::OnbtnKEICANCEL() 
{
	if(TRUE == m_bIsRunningThread) return;

	OnCancel();	
}

void CKlEditIdDlg::OnbtnKEICHECK() 
{
	if(TRUE == m_bIsRunningThread) return;

	GetDlgItemText(IDC_edtKEI_ID, m_strCtlID);
	m_strCtlID.Replace(_T(" "), _T(""));
	if(m_strCtlID.IsEmpty())
	{
		SetDlgItemText(IDC_stcKEI_MSG, _T("아이디를 입력해 주십시오."));
		((CEdit*)GetDlgItem(IDC_edtKEI_ID))->SetFocus();
		return;
	}
	if(m_strCtlID == m_strGetID)
	{
		SetDlgItemText(IDC_stcKEI_MSG, _T("새로운 아이디를 입력해 주십시오."));
		((CEdit*)GetDlgItem(IDC_edtKEI_ID))->SetFocus();
		return;
	}

	SetTimer(1, 100, NULL);
}

UINT CKlEditIdDlg::CheckIdThread(LPVOID pParam)
{
EFS_BEGIN

	CKlEditIdDlg* pDlg = (CKlEditIdDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->CheckId();
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
		pDlg->SetTimer(103, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

UINT CKlEditIdDlg::SaveIdThread(LPVOID pParam)
{
EFS_BEGIN

	CKlEditIdDlg* pDlg = (CKlEditIdDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->SaveId();
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
		pDlg->SetTimer(202, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

INT CKlEditIdDlg::CheckId()
{
EFS_BEGIN

	INT ids;
	CString strMsg;
	CESL_DataMgr TempDM;
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	CFrameWriter Writer(m_pInfo);
	CString strCommand = _T("IL_K39_LOC_B01_SERVICE");
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합아이디"), m_strCtlID);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		strMsg.Format(_T("통합아이디중복조회(39) ERROR CODE : %d"), ids);
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
		strMsg.Format(_T("통합아이디중복조회(39) ERROR CODE : %s"), strTemp);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		return 101;
	}
	if(1 > TempDM.GetRowCount())
	{
		TempDM.QueryLog(_T("통합아이디중복조회(39) ERROR (CNT=0)"));
		return 101;
	}
	CString strGetData;
	TempDM.GetCellData(_T("중복여부"), 0, strGetData);
	if(strGetData.IsEmpty()) return 101;
	if(_T("N") != strGetData)
	{
		return 102;
	}

	return 0;
	
EFS_END
return -1;
}

INT CKlEditIdDlg::SaveId()
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

	Writer.addElement(_T("통합아이디"), m_strCtlID);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		strMsg.Format(_T("통합아이디비밀번호수정(40) ERROR CODE : %d"), ids);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		return 201;
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
		return 201;
	}
	if(1 > TempDM.GetRowCount())
	{
		TempDM.QueryLog(_T("통합아이디비밀번호수정(40) ERROR (CNT=0)"));
		return 201;
	}
	CString strGetData;
	TempDM.GetCellData(_T("처리결과"), 0, strGetData);
	if(strGetData.IsEmpty()) return 101;
	if(_T("OK") != strGetData)
	{
		return 201;
	}

	return 0;
	
EFS_END
return -1;
}

void CKlEditIdDlg::OnbtnKEIEDIT() 
{
	if(TRUE == m_bIsRunningThread) return;

	SetTimer(2, 100, NULL);
}
