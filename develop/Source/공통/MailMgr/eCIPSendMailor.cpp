// Document modified at : Monday, June 10, 2002 12:30:56 PM , by user : Administrator , from computer : GJPARK
// eCIPSendMailor.cpp : implementation file
//

#include "stdafx.h"
#include "eCIPSendMailor.h"

#include "RegKey.h"
#include "SelectMailServer.h"
#include "Smtp.h"

#include "RotatingLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CeCIPSendMailor dialog


CeCIPSendMailor::CeCIPSendMailor(CWnd* pParent /*=NULL*/)
	: CSendMailMgr(pParent)
{
	//{{AFX_DATA_INIT(CeCIPSendMailor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	this->m_pRecord      = NULL;
	
	this->m_SystemOS		 = INVALID;
	this->m_sMailAccountPath = _T("");
}

CeCIPSendMailor::~CeCIPSendMailor()
{
	
}



VOID CeCIPSendMailor::DoDataExchange(CDataExchange* pDX)
{
	CSendMailMgr::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CeCIPSendMailor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CeCIPSendMailor, CSendMailMgr)
	//{{AFX_MSG_MAP(CeCIPSendMailor)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CeCIPSendMailor message handlers


INT CeCIPSendMailor::GetSystemInfo()
{
	if (0 > this->GetSystemVersion()  ) return -1;
	if (0 > this->GetMailAccountPath()) return -2;
	return 0;
}

INT CeCIPSendMailor::GetSystemVersion()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if ( !( bOsVersionInfoEx = GetVersionEx ( (OSVERSIONINFO*)&osvi) ) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return -1;
	}

	switch (osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_NT:			this->m_SystemOS = WINDOWS2K; break;
		case VER_PLATFORM_WIN32_WINDOWS:	this->m_SystemOS = WINDOWS98; break;
	}
		
	return 0;	
}

INT CeCIPSendMailor::GetMailAccountPath()
{
	//##################################################
	//Windows 98 System
	//##################################################
	
	if (WINDOWS98 == this->m_SystemOS) {
		//-----------------------------------------------------------------------
		//HKEY_USERS \ .DEFAULT \ SOFTWARE \ Microsoft \ Internet Account Manager
		//-----------------------------------------------------------------------
		this->m_sMailAccountPath = _T(".DEFAULT\\SOFTWARE\\Microsoft\\Internet Account Manager");
		return 0;
	}

	
	//##################################################
	//Windows NT/2000 System
	//##################################################
	
	LONG lRet;
	HKEY hKey;
	
	#define SID_SIZE 1024
	TCHAR	szSID[SID_SIZE] = _T("");
	DWORD	dwSIDLen = SID_SIZE;
	
	//--------------------------------------------------------------------------------------
	//SID 구하기
	//	: HKEY_CURRENT_USER \ Software \ Microsoft \ Protected Storage System Provider \ _T("KEY값")
	//--------------------------------------------------------------------------------------
	
	lRet = RegOpenKeyEx(HKEY_CURRENT_USER,
						TEXT(_T("Software\\Microsoft\\Protected Storage System Provider")),
						0, KEY_ENUMERATE_SUB_KEYS, 
						&hKey);
	if (ERROR_SUCCESS != lRet) return -1;
	
	lRet = RegEnumKeyEx(hKey, 0, szSID, &dwSIDLen, NULL, NULL, NULL, NULL);
	
	RegCloseKey(hKey);
	
	if (ERROR_SUCCESS != lRet) return -2;
	
	//------------------------------------------------------------------
	//HKEY_USERS \ SID \ Software \ Microsoft \ Internet Account Manager
	//------------------------------------------------------------------
	//*(szSID+dwSIDLen+1) = 0;
	this->m_sMailAccountPath.Format(_T("%s\\Software\\Microsoft\\Internet Account Manager"), szSID);
	
	return 0;
}

INT CeCIPSendMailor::GetDefaultMailServerInfo()
{
	CRegKeyMgr RegKey;

	//CString sMailAccountPath = _T(".DEFAULT\\SOFTWARE\\Microsoft\\Internet Account Manager");
	CString sMailAccountPath = this->m_sMailAccountPath;

	CString sDefalutMailAccount;
	CString sSMTPServer;
	CString sSMTPEmailAddress;
	CString sSMTPDisplayName;
	CString sEmailAddress;

	LONG	lRet;
	HKEY    hKeyRoot = HKEY_USERS;

	sDefalutMailAccount.Empty();

	lRet = RegKey.Open(hKeyRoot, sMailAccountPath);
	if (ERROR_SUCCESS != lRet) return -1;
	
	lRet = RegKey.Read(_T("Default Mail Account"), sDefalutMailAccount);
	RegKey.Close();	
	if (ERROR_SUCCESS != lRet) return -2;

	if (sDefalutMailAccount.IsEmpty()) {
		MessageBox(_T("메일서버 정보가 없습니다.\n\n")
				   _T("메일서버 정보는 메일클라이언트 프로그램에서 설정하실 수 있습니다."), 
				   _T("경고"), MB_ICONEXCLAMATION);
		return 1;
	}


	sMailAccountPath += _T("\\Accounts\\") + sDefalutMailAccount;
	lRet = RegKey.Open(hKeyRoot, sMailAccountPath);
	if (ERROR_SUCCESS != lRet)	return -3;
	
	lRet = RegKey.Read(_T("SMTP Server"), sSMTPServer);
	if (ERROR_SUCCESS != lRet) { RegKey.Close(); return -4; }
	
	lRet = RegKey.Read(_T("SMTP Email Address"), sSMTPEmailAddress);
	if (ERROR_SUCCESS != lRet) { RegKey.Close(); return -5; }
	
	lRet = RegKey.Read(_T("SMTP Display Name"),	sSMTPDisplayName);
	
	RegKey.Close();
	if (ERROR_SUCCESS != lRet) return -6;
	
	sEmailAddress.Format(_T("\"%s\" <%s>"), sSMTPDisplayName, sSMTPEmailAddress);

	this->m_sMailServer  = sSMTPServer;
	this->m_sMailAddress = sEmailAddress;

	this->SetDefaultMailServer(this->m_sMailServer, this->m_sMailAddress);

	UpdateData(FALSE);

	return 0;
}




BOOL CeCIPSendMailor::OnInitDialog() 
{
	CDialog::OnInitDialog();


	//CSendMailMgr::OnInitDialog();
	{
		// ListCtrl 초기화
		m_ctrlMail.InsertColumn(0, _T("제목")    , LVCFMT_LEFT, 200);
		m_ctrlMail.InsertColumn(1, _T("받는사람"), LVCFMT_LEFT, 300);
		m_ctrlMail.InsertColumn(2, _T("인코딩")  , LVCFMT_LEFT, 100);
		//m_ctrlMail.InsertColumn(3, _T("전송여부"), LVCFMT_LEFT, 100);
		
		// HTMLCtrl 초기화
		m_Html.CreateFromStatic(IDC_HTML_STATIC, this);
		m_Html.ModifyStyle(0, WS_EX_CLIENTEDGE);
		
		// SplitterWnd 초기화
		m_ySplitter.BindWithControl(this, IDC_YBAR);
		m_ySplitter.SetMinHeight(100, 100);
		
		m_ySplitter.AttachAsAbovePane(IDC_lMAIL      );
		m_ySplitter.AttachAsBelowPane(IDC_HTML_STATIC);
		
		m_ySplitter.RecalcLayout();
		
		// Button Bitmap Load
		//m_ctrlSelectSTMPServer.LoadBitmaps(IDB_SMTP_N, IDB_SMTP_S, IDB_SMTP_N, IDB_SMTP_D);
		//m_ctrlDeleteMat.LoadBitmaps(IDB_DELETE_N, IDB_DELETE_S, IDB_DELETE_N, IDB_DELETE_D);
		//m_ctrlSend.LoadBitmaps(IDB_SEND_N, IDB_SEND_S, IDB_SEND_N, IDB_SEND_D);
		//m_ctrlClose.LoadBitmaps(IDB_CLOSE_N, IDB_CLOSE_S, IDB_CLOSE_N, IDB_CLOSE_D);
		
		// Display
		DisplaySendMailRecord();		
	}

	
	// TODO: Add extra initialization here
	INT ids;

	(this->GetDlgItem(IDC_bSELECT_SMTP_SERVER)) -> SetWindowText(_T("메일서버선택"));

	ids = this->GetSystemInfo();
	if (0>ids) {
		this->EndDialog(IDCANCEL);
		return FALSE;
	}

	ids = this->GetDefaultMailServerInfo();
	if (0!=ids) {
		if (0>ids) {
			AfxMessageBox(_T("메일서버 정보를 설정할 수 없습니다."));
			this->EndDialog(IDCANCEL);
		}
		else
			this->EndDialog(IDCANCEL);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CeCIPSendMailor::DisplaySendMailRecord()
{
	// 초기화
	m_ctrlMail.DeleteAllItems();
	m_ctrlMail.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	
	LV_ITEM lvitem;
	
	lvitem.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	//lvitem.mask      = LVIF_TEXT | LVIF_STATE;
	lvitem.iSubItem  = 0;
	lvitem.iImage    = 0;
	lvitem.stateMask = LVIS_STATEIMAGEMASK;
	lvitem.state     = INDEXTOSTATEIMAGEMASK(1);
	
	CString sRecipient;
	CString sCC;
	
	for (INT i = 0; i < m_nRecordCount; i++)
	{
		sRecipient.Format(_T("\"%s\" <%s>"), m_pRecord[i].m_sRecipientName, m_pRecord[i].m_sRecipientAddress);
		if (!m_pRecord[i].m_sCCName.IsEmpty() && !m_pRecord[i].m_sCCAddress.IsEmpty()) {
			sCC.Format(_T("\"%s\" <%s>"), m_pRecord[i].m_sCCName, m_pRecord[i].m_sCCAddress);
			sRecipient += _T(" ; ") + sCC;
		}
		lvitem.iItem   = m_ctrlMail.GetItemCount();
		lvitem.pszText = (LPCTSTR)(LPCTSTR) m_pRecord[i].m_sSubject;
		
		// 새로운 행 등록
		m_ctrlMail.InsertItem(&lvitem);
		
		// 각 칼럼의 텍스트 설정
		m_ctrlMail.SetItemText(lvitem.iItem, 1, sRecipient);
		m_ctrlMail.SetItemText(lvitem.iItem, 2, m_pRecord[i].m_sEncoding);
	}
}



VOID CeCIPSendMailor::OnDestroy() 
{
	CSendMailMgr::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


INT CeCIPSendMailor::AddRecord(SendMailRecord* pMailRecord)
{
	SendMailRecord* pTemp = new SendMailRecord[m_nRecordCount + 1];
	if (pTemp == NULL) {
		return -1;
	}

	for (INT i = 0; i < m_nRecordCount; i++) {
		pTemp[i].m_sSubject          = m_pRecord[i].m_sSubject;
		pTemp[i].m_sRecipientName    = m_pRecord[i].m_sRecipientName;
		pTemp[i].m_sRecipientAddress = m_pRecord[i].m_sRecipientAddress;
		pTemp[i].m_sBody             = m_pRecord[i].m_sBody;
		pTemp[i].m_sEncoding         = m_pRecord[i].m_sEncoding;
		pTemp[i].m_pFileAttach		 = m_pRecord[i].m_pFileAttach;
		pTemp[i].m_nFileAttachCnt	 = m_pRecord[i].m_nFileAttachCnt;
		
		pTemp[i].m_sCCName 			 = m_pRecord[i].m_sCCName;
		pTemp[i].m_sCCAddress		 = m_pRecord[i].m_sCCAddress;
	}

	delete [] m_pRecord;
	m_pRecord = NULL;

	m_pRecord = pTemp;

	m_pRecord[m_nRecordCount].m_sSubject          = pMailRecord->m_sSubject;
	m_pRecord[m_nRecordCount].m_sRecipientName    = pMailRecord->m_sRecipientName;
	m_pRecord[m_nRecordCount].m_sRecipientAddress = pMailRecord->m_sRecipientAddress;
	m_pRecord[m_nRecordCount].m_sBody             = pMailRecord->m_sBody;
	m_pRecord[m_nRecordCount].m_sEncoding         = pMailRecord->m_sEncoding;
	m_pRecord[m_nRecordCount].m_pFileAttach		  = pMailRecord->m_pFileAttach;
	m_pRecord[m_nRecordCount].m_nFileAttachCnt    = pMailRecord->m_nFileAttachCnt;

	m_pRecord[m_nRecordCount].m_sCCName			  = pMailRecord->m_sCCName;
	m_pRecord[m_nRecordCount].m_sCCAddress		  = pMailRecord->m_sCCAddress;

	m_nRecordCount++;

	return 0;
}

INT CeCIPSendMailor::AddRecord(CString sSubject, CString sRecipientName, CString sRecipientAddress, CString sBody, CString sEncoding, CString pFileAttach, INT nFileAttachCnt)
{
	return CSendMailMgr::AddRecord(sSubject, sRecipientName, sRecipientAddress, sBody, sEncoding, pFileAttach, nFileAttachCnt);
}

CString CeCIPSendMailor::GetWindowTempDirectory()
{
/*
	CRegKeyMgr RegKey;

	CString sWindowTempDir;	
	if (WINDOWS98 == this->m_SystemOS)		sWindowTempDir = _T("Windows");
	else if (WINDOWS2K == this->m_SystemOS) sWindowTempDir = _T("Windows NT");

	CString sWindowTempDirPath;
	sWindowTempDirPath.Format(_T("Software\\Microsoft\\%s\\CurrentVersion"), sWindowTempDir);
	
	HKEY    hKeyRoot = HKEY_LOCAL_MACHINE;
	
	if (RegKey.Open(hKeyRoot, sWindowTempDirPath) == ERROR_SUCCESS) 
	{
		RegKey.Read(_T("SystemRoot"), sWindowTempDir);
	}
	
	 return sWindowTempDir;

*/

	DWORD dwResult; 

	#define  BUFFER_SIZE 1024
	TCHAR tchWindowTempDir[BUFFER_SIZE];
	*tchWindowTempDir = 0; 

	dwResult = GetSystemDirectory(tchWindowTempDir, BUFFER_SIZE); 
	if (0==dwResult) return _T("");

	//dwResult = ExpandEnvironmentStrings(_T("TMP=%TMP%"), tchWindowTempDir, BUFFER_SIZE);	
	//if (0==dwResult) return _T("");

	return CString(tchWindowTempDir);
}


VOID CeCIPSendMailor::OnOK() 
{
	//CSendMailMgr::OnOK();

	{
		CSmtp mail;
		CSmtpMessage msg;
		CSmtpAddress cc;
		CSmtpMessageBody body;
		CSmtpAttachment attach;
		
		BOOL bFail = FALSE;
		
		UpdateData(TRUE);
		
		if (m_sMailServer.IsEmpty() == TRUE) {
			MessageBox(_T("메일서버가 지정되지 않았습니다."), _T("경고"), MB_ICONEXCLAMATION);
			return;
		}
		
		if (m_sMailAddress.IsEmpty() == TRUE) {
			MessageBox(_T("보내는 사람이 지정되지 않았습니다."), _T("경고"), MB_ICONEXCLAMATION);
			return;
		}
		
		for (INT i = 0; i < m_nRecordCount; i++) {
			CString sName;
			CString sAddress;
			
			CutNameAndAddress(m_sMailAddress, sName, sAddress);
			
			msg.Subject           = m_pRecord[i].m_sSubject;
			msg.Sender.Name       = sName;
			msg.Sender.Address    = sAddress;
			msg.Recipient.Name    = m_pRecord[i].m_sRecipientName;
			msg.Recipient.Address = m_pRecord[i].m_sRecipientAddress;
			
			if (!m_pRecord[i].m_sCCName.IsEmpty()) {
				cc.Name           = m_pRecord[i].m_sCCName;
				cc.Address        = m_pRecord[i].m_sCCAddress;
				msg.CC.Add(cc);
			}
			
			//
			body                  = m_pRecord[i].m_sBody;
			body.Encoding         = m_pRecord[i].m_sEncoding;
			msg.Message.Add(body);
			
			// Here are some samples of adding attachments to the message
			INT next = 0;
			INT pre = 0;
			
			CString tempstr = m_pRecord[i].m_pFileAttach;
			INT tempstrlength = tempstr.GetLength();
			CString prestr;	
			
			while ( ( pre = (tempstr.FindOneOf(_T(","))) ) != -1) {
				prestr = tempstr.Mid(0,pre);
				attach = prestr;
				msg.Attachments.Add( attach );
				next = pre+1;
				tempstr = tempstr.Mid(next,tempstrlength);
			}
			attach = tempstr;
			msg.Attachments.Add(attach);
			
			//###################################################
			// [6/3/2002] if ( mail.Connect((LPCTSTR)(LPCTSTR) m_sMailServer, NULL) ) {
			CRotatingLog Log(_T("..\\tunning\\filelog\\SmtpLog.txt"), DATE_FILE_NAME);
			Log.StartLogging();
			Log.AddLine(_T("\n\n[MailMgr][CONNECT][00][-] Connecting to SMTP Sever..."));
			Log.StopLogging();
			if ( mail.Connect((LPCTSTR)(LPCTSTR) m_sMailServer) ) {
				Log.StartLogging();
				Log.AddLine(_T("[MailMgr][CONNECT][01][O] Connected !!!"));
				Log.AddLine(_T("[MailMgr][SENDING][02][-] Sending..."));
				Log.StopLogging();
				mail.SendMessage(msg);
				Log.StartLogging();
				Log.AddLine(_T("[MailMgr][SENDING][03][O] Sended ?!!"));
				Log.AddLine(_T("[MailMgr][SENDING][04][-] Closing..."));
				Log.StopLogging();
				mail.Close();
				Log.StartLogging();
				Log.AddLine(_T("[MailMgr][SENDING][04][-] Closed !!!"));
				Log.StopLogging();
			} else {
				MessageBox(_T("메일 보내기를 실패하였습니다."), _T("경고"), MB_ICONEXCLAMATION);
				bFail = TRUE;
				m_MailSendYn = 'N';
				Log.StartLogging();
				Log.AddLine(_T("[MailMgr][CONNECT][01][O] Not connected !!! ### EXIT ###"));
				Log.StopLogging();
				break;
			}
			//###################################################
		}
		
		if (!bFail) {
			MessageBox(_T("메일이 보내졌습니다."), _T("확인"), MB_ICONINFORMATION);
			m_MailSendYn = 'Y';
		}
	}

// [6/10/2002] 	SMTPLog.Close();

	this->EndDialog(IDOK);
}

VOID CeCIPSendMailor::OnCancel() 
{
	CSendMailMgr::OnCancel();
	this->EndDialog(IDCANCEL);
}


/*
VOID CeCIPSendMailor::OnClicklMAIL(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CSendMailMgr::OnDestroy();
}

VOID CeCIPSendMailor::OnbSELECTSMTPSERVER() 
{
	CSendMailMgr::OnbSELECTSMTPSERVER();
}

VOID CeCIPSendMailor::OnbDELETEMAT() 
{
	CSendMailMgr::OnbDELETEMAT();
}


LONG CeCIPSendMailor::OnWmSplitterMoved(UINT wParam, LONG lParam)
{
	DisplayMailBody(m_nRecordIndex);
    return 1L;
}
*/

VOID CeCIPSendMailor::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CSendMailMgr::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	DisplayMailBody(0);	
}

HBRUSH CeCIPSendMailor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
