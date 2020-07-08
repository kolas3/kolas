#include "stdafx.h"
#include "KlIdPassViewDlg.h"
#include "FrameManager.h"
#include "KlEditIdDlg.h"
#include "KlEditPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CKlIdPassViewDlg::CKlIdPassViewDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlIdPassViewDlg::IDD, pParent)
{
	m_strUserNo = _T("");

	m_bIsRunningThread = FALSE;
	m_nMode = 0;
}
CKlIdPassViewDlg::~CKlIdPassViewDlg()
{
}

void CKlIdPassViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKlIdPassViewDlg) 
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CKlIdPassViewDlg, CDialog)
	//{{AFX_MSG_MAP(CKlIdPassViewDlg)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnKIV_CLOSE, OnbtnKIVCLOSE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnKIV_RETRY, OnbtnKIVRETRY)
	ON_BN_CLICKED(IDC_btnKIV_EDIT_ID, OnbtnKIVEDITID)
	ON_BN_CLICKED(IDC_btnKIV_EDIT_PASS, OnbtnKIVEDITPASS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CKlIdPassViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());	
	((CButton*)GetDlgItem(IDC_btnKIV_EDIT_PASS))->ShowWindow(SW_HIDE);	

	OnbtnKIVRETRY();

	return TRUE;  
	              
}

BOOL CKlIdPassViewDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKIVCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKIV_EDIT_ID)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKIVEDITID();
			}
			else if(GetDlgItem(IDC_btnKIV_EDIT_PASS)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKIVEDITPASS();
			}
			else if(GetDlgItem(IDC_btnKIV_RETRY)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKIVRETRY();
			}
			else if(GetDlgItem(IDC_btnKIV_CLOSE)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKIVCLOSE();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CKlIdPassViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKIV_MSG == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CKlIdPassViewDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}

void CKlIdPassViewDlg::OnbtnKIVCLOSE() 
{
	if(TRUE == m_bIsRunningThread) return;

	OnCancel();
}

void CKlIdPassViewDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKIV_RETRY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnKIV_EDIT_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKIV_EDIT_PASS)->EnableWindow(FALSE);
		SetDlgItemText(IDC_stcKIV_MSG, _T("통합 아이디/비밀번호를 가져오는 중입니다."));
		AfxBeginThread(SearchIdPassThread, this);
	}
	else if(100 == nIDEvent)
	{
		KillTimer(nIDEvent);
		AfxMessageBox(_T("GPIN_HASH가 없습니다."));
		OnCancel();
	}
	else if(101 == nIDEvent)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_btnKIV_RETRY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnKIV_EDIT_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKIV_EDIT_PASS)->EnableWindow(FALSE);
		SetDlgItemText(IDC_stcKIV_MSG, _T("정보 조회에 실패하였습니다."));
	}
	else if(102 == nIDEvent)
	{
		KillTimer(nIDEvent);
		AfxMessageBox(_T("검색된 이용자가 없습니다."));
		OnCancel();
	}
	else if(103 == nIDEvent)
	{
		KillTimer(nIDEvent);
		
		AfxMessageBox(_T("책이음회원 중복이 발생하였습니다."));
		OnCancel();
	}
	else if(104 == nIDEvent)
	{
		KillTimer(nIDEvent);
		AfxMessageBox(_T("통합 아이디가 없는 이용자입니다."));
		OnCancel();
	}
	else if(105 == nIDEvent)
	{
		KillTimer(nIDEvent);
		if(1 != m_nMode && 3 != m_nMode)
		{
			GetDlgItem(IDC_btnKIV_EDIT_ID)->EnableWindow(FALSE);
			GetDlgItem(IDC_btnKIV_EDIT_PASS)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_btnKIV_EDIT_ID)->EnableWindow(TRUE);
			GetDlgItem(IDC_btnKIV_EDIT_PASS)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_btnKIV_RETRY)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_stcKIV_MSG, _T("조회에 성공하였습니다."));
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKlIdPassViewDlg::OnbtnKIVRETRY() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(0, 100, NULL);
}

UINT CKlIdPassViewDlg::SearchIdPassThread(LPVOID pParam)
{
EFS_BEGIN

	CKlIdPassViewDlg* pDlg = (CKlIdPassViewDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->SearchIdPass();
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
		pDlg->SetTimer(105, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

INT CKlIdPassViewDlg::SearchIdPass()
{
EFS_BEGIN

	
	if(m_strUserNo.IsEmpty()) return 100;


	INT ids;
	CString strMsg;
	CESL_DataMgr TempDM;
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	CFrameWriter Writer(m_pInfo);
	CString strCommand = _T("IL_K38_LOC_B01_SERVICE");
	Writer.setCommand(strCommand);
	Writer.addRecord();
	
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);

	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		strMsg.Format(_T("통합아이디/비밀번호조회(38) ERROR CODE : %d"), ids);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		return 101;
	}
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strTemp;
		Reader.getError(strTemp, strMsg);
		if(-1 < strTemp.Find(_T("MK38TCTB01DT01")))
		{ 
			return 102;
		}
		if(-1 < strTemp.Find(_T("MK38TCTB01DT02")))
		{ 
			return 103;
		}
		if(strMsg.IsEmpty())
		{
			strTemp = _T("null");
		}
		strMsg.Format(_T("통합아이디/비밀번호조회(38) ERROR CODE : %s"), strTemp);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		return 101;
	}
	if(1 > TempDM.GetRowCount())
	{
		TempDM.QueryLog(_T("통합아이디/비밀번호조회(38) ERROR (CNT=0)"));
		return 101;
	}
	CString strID, strPASS;
	TempDM.GetCellData(_T("통합아이디"), 0, strID);
	if(strID.IsEmpty()) return 104;
	SetDlgItemText(IDC_edtKIV_ID, strID);
	TempDM.GetCellData(_T("통합비밀번호"), 0, strPASS);
	SetDlgItemText(IDC_edtKIV_PASS, strPASS);

	m_strGetID = strID;
	m_strGetPASS = strPASS;

	return 0;
	
EFS_END
return -1;
}

void CKlIdPassViewDlg::OnbtnKIVEDITID() 
{
	
	if(m_strUserNo.IsEmpty() || m_strGetID.IsEmpty()) return;


	CKlEditIdDlg dlg(this);
	
	dlg.m_strUserNo = m_strUserNo;

	dlg.m_strGetID = m_strGetID;
	if(IDOK == dlg.DoModal())
	{
		SetDlgItemText(IDC_edtKIV_ID, dlg.m_strCtlID);
		m_strGetID = dlg.m_strCtlID;
		SetDlgItemText(IDC_stcKIV_MSG, _T("아이디가 변경되었습니다."));
	}
}

void CKlIdPassViewDlg::OnbtnKIVEDITPASS() 
{
	
	if(m_strUserNo.IsEmpty()) return;


	CKlEditPassDlg dlg(this);
	
	dlg.m_strUserNo = m_strUserNo;

	if(IDOK == dlg.DoModal())
	{
		SetDlgItemText(IDC_edtKIV_PASS, dlg.m_strCtlPASS);
		m_strGetPASS = dlg.m_strCtlPASS;
		SetDlgItemText(IDC_stcKIV_MSG, _T("비밀번호가 변경되었습니다."));
	}
}
