// MailMgrTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MailMgrTest.h"
#include "MailMgrTestDlg.h"
#include "Windows.h"
//#include <imm.h>        

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailMgrTestDlg dialog

CMailMgrTestDlg::CMailMgrTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailMgrTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailMgrTestDlg)
	m_ctvRecipientName = _T("");
	m_ctvRecipientEmail = _T("");
	m_ctvBackground = FALSE;
	m_ctvCleareLog = FALSE;
	m_ctvConfigFilepath = CFG_FILE_PATH;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_bLogOutput = FALSE;
}

VOID CMailMgrTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailMgrTestDlg)
	DDX_Control(pDX, IDC_chkDELETE_LOG_FILE, m_ctrlDeleteLogFile);
	DDX_Control(pDX, IDC_chkCLEAR, m_ctrlClear);
	DDX_Control(pDX, IDC_chkBACKGROUND, m_ctrlBackground);
	DDX_Control(pDX, IDC_eRECIPIENT_EMAIL, m_ctrlRecipientEmail);
	DDX_Control(pDX, IDC_eLOG, m_ctrlLog);
	DDX_Control(pDX, IDC_eRECIPIENT_NAME, m_ctrlRecipientName);
	DDX_Control(pDX, IDC_rSEND, m_ctrlRadioSend);
	DDX_Control(pDX, IDC_rRESULT, m_ctrlRadioResult);
	DDX_Text(pDX, IDC_eRECIPIENT_NAME, m_ctvRecipientName);
	DDX_Text(pDX, IDC_eRECIPIENT_EMAIL, m_ctvRecipientEmail);
	DDX_Check(pDX, IDC_chkBACKGROUND, m_ctvBackground);
	DDX_Check(pDX, IDC_chkCLEAR, m_ctvCleareLog);
	DDX_Text(pDX, IDC_eCFG_FILE_PATH, m_ctvConfigFilepath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailMgrTestDlg, CDialog)
	//{{AFX_MSG_MAP(CMailMgrTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_rSEND, OnrSEND)
	ON_BN_CLICKED(IDC_rRESULT, OnrRESULT)
	ON_EN_CHANGE(IDC_eRECIPIENT_NAME, OnChangeeRECIPIENTNAME)
	ON_EN_CHANGE(IDC_eRECIPIENT_EMAIL, OnChangeeRECIPIENTEMAIL)
	ON_BN_CLICKED(IDC_chkBACKGROUND, OnchkBACKGROUND)
	ON_BN_CLICKED(IDC_btnMAIL_SEND, OnbtnMAILSEND)
	ON_EN_CHANGE(IDC_eLOG, OnChangeeLOG)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_chkCLEAR, OnchkCLEAR)
	ON_BN_CLICKED(IDC_btnMAIL_SERVER, OnbtnMAILSERVER)
	ON_BN_CLICKED(ID_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_chkDELETE_LOG_FILE, OnchkDELETELOGFILE)
	ON_EN_KILLFOCUS(IDC_eRECIPIENT_NAME, OnKillfocuseRecipientNameOrEmail)
	ON_EN_SETFOCUS(IDC_eRECIPIENT_EMAIL, OnSetfocuseRECIPIENTEMAIL)
	ON_BN_CLICKED(IDC_btnMAIL_SERVER_VIEW, OnbtnMAILSERVERVIEW)
	ON_EN_KILLFOCUS(IDC_eRECIPIENT_EMAIL, OnKillfocuseRecipientNameOrEmail)
	ON_BN_CLICKED(IDC_chkVIEW_LOG_FILE, OnchkVIEWLOGFILE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailMgrTestDlg message handlers

BOOL CMailMgrTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	this->m_ctrlClear.SetCheck(1);
	OnrSEND();
	m_ctrlRadioSend.SetFocus();
	m_ctrlRecipientName.SetFocus();
	
    long	ltime;
    tm		*t;
    time(&ltime);
    t = localtime (&ltime);
	m_sLogFilepath.Format(_T("..\\tunning\\(%04d%02d%02d)SmtpLog.txt"), 1900+t->tm_year, t->tm_mon+1, t->tm_mday);
	
	((CWnd*)(GetDlgItem(IDC_btnMAIL_SEND)))->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CMailMgrTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CMailMgrTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMailMgrTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CMailMgrTestDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	OnrSEND();
	m_ctrlRadioSend.SetFocus();
	m_ctrlRecipientName.SetFocus();
}

VOID CMailMgrTestDlg::OnrSEND() 
{
	// TODO: Add your control notification handler code here
	m_ctrlRadioSend.SetCheck(1);
	m_ctrlRadioResult.SetCheck(0);
	m_ctrlRecipientName.SetFocus();
}

VOID CMailMgrTestDlg::OnrRESULT() 
{
	// TODO: Add your control notification handler code here
	m_ctrlRadioSend.SetCheck(0);
	m_ctrlRadioResult.SetCheck(1);
	//m_ctrlLog.SetWindowText(_T(""));
	m_ctrlLog.SetFocus();
}

VOID CMailMgrTestDlg::OnChangeeRECIPIENTNAME() 
{
	OnrSEND();	
	m_ctrlRecipientName.SetFocus();
	OnKillfocuseRecipientNameOrEmail();
}

VOID CMailMgrTestDlg::OnChangeeRECIPIENTEMAIL() 
{
	OnrSEND();	
	//OnSetfocuseRECIPIENTEMAIL();
	OnKillfocuseRecipientNameOrEmail();
	m_ctrlRecipientEmail.SetFocus();
}

VOID CMailMgrTestDlg::OnchkBACKGROUND() 
{
	OnrSEND();	
	m_ctrlBackground.SetFocus();
}

VOID CMailMgrTestDlg::OnchkCLEAR() 
{
	OnrRESULT();
	this->m_ctrlLog.SetFocus();
}

VOID CMailMgrTestDlg::OnChangeeLOG() 
{
	OnrRESULT();
	this->m_ctrlLog.SetFocus();
}


VOID CMailMgrTestDlg::OnbtnMAILSEND() 
{
	OnrSEND();	

	UpdateData(TRUE);
	
	INT rc;
	if (m_sTestCfgFile.IsEmpty())
	{
		rc = MakeTestMailAccountCfgFile(m_ctvConfigFilepath, FALSE);
		if (0>rc) 
		{
			AfxMessageBox(_T("Mail 전송 테스트를 위한 환경 파일을 생성하지 못했습니다."));
			return;
		}
		ASSERT(!m_sTestCfgFile.IsEmpty());
	}
	
	if (this->m_ctvCleareLog) OnchkDELETELOGFILE();
	this->m_ctrlLog.SetReadOnly();

	CString sTemp;

	
	//--------------------------------------------------------------------------------------
	///Mail Manager 객체 생성
	//--------------------------------------------------------------------------------------
	CESL_Mgr *pESLMgr;

	pESLMgr = new CESL_Mgr(0,NULL);
	CSendMailMgr MailMgr(pESLMgr);

	

	//--------------------------------------------------------------------------------------
	///보내는 사람 메일서버/계정 정보를 Config file에서 가져오는 경우 config file 경로 설정
	///MailMgr default 설정값 = _T("..\\cfg\\MailAccount.cfg")
	///위의 default 설정값과 다른 경우는 반드시 설정해야 한다.
	//--------------------------------------------------------------------------------------

	//MailMgr.SetCfgFilepath(_T("..\\cfg\\MailAccount.cfg"));
	//MailMgr.SetCfgFilepath(m_ctvConfigFilepath);
	MailMgr.SetCfgFilepath(this->m_sTestCfgFile);

	
	//--------------------------------------------------------------------------------------
	///메일 전송 로그 파일 경로 설정
	///MailMgr default 설정값 = _T("..\\tunning\\SmtpLog.txt")
	///위의 default 설정값과 다른 경우는 반드시 설정해야 한다.
	///로그파일명은 설정한 파일명(SmtpLog.txt) 앞에 날짜를 붙여(YYYYMMDD) 생성한다.
	///즉, ..\\tunning\\(20030227)SmtpLog.txt 이런 식으로
	//--------------------------------------------------------------------------------------

	//MailMgr.SetLogFilepath(_T("..\\tunning\\SmtpLog.txt"));


	

	//--------------------------------------------------------------------------------------
	///Mail Record 추가 -> MailMgr.AddMailRecord() 를 사용 -> AddTwoMailRecord 함수 참고
	//--------------------------------------------------------------------------------------
	rc = AddTwoMailRecord(&MailMgr);
	if (0>rc) 
	{
		sTemp.Format(_T("AddTwoMailRecord(MailMgr) Error [%d] !!!"), rc);
		AfxMessageBox(sTemp);
		return;
	}


	//--------------------------------------------------------------------------------------
	///MailMgr Dialog를 띄워서 메일 전송을 하는 경우
	//--------------------------------------------------------------------------------------
	if (!m_ctvBackground)
	{
		//-------------------------------------------
		///메일 서버/계정 정보 설정하고 Dialog를 띄움
		//-------------------------------------------

		///직접 메일서버와 보내는 사람을 설정하는 경우
		//MailMgr.DoModal(MAIL_SERVER_USER, _T("mail.eco.co.kr"), _T("\"도서관서비스\"<foo@bar.baz>"));
		
		///Registry Mail Account 정보를 사용하는 경우
		//MailMgr.DoModal(MAIL_SERVER_REGISTRY);

		///Config file에서 메일서버와 보내는 사람을 설정하는 경우
		MailMgr.DoModal(MAIL_SERVER_CFGFILE);
	}

	
	//--------------------------------------------------------------------------------------
	///MailMgr Dialog를 띄우지 않고 Background로 메일 전송을 하는 경우
	//--------------------------------------------------------------------------------------
	else
	{
		//-------------------------------------------
		///메일 서버/계정 정보 설정
		//-------------------------------------------
		
		///직접 메일서버와 보내는 사람을 설정하는 경우
		//MailMgr.SetDefaultMailServer(_T("mail.eco.co.kr"), _T("\"도서관서비스\"<foo@bar.baz>"));
		
		///Registry Mail Account 정보를 사용하는 경우
		//MailMgr.SetMailServerInfoFromRegistry(TRUE);
		
		///Config file에서 메일서버와 보내는 사람을 설정하는 경우
		rc = MailMgr.SetMailServerInfoFromCfgFile(TRUE);	// <<< 반드시 TRUE로 매개변수를 넘겨야 한다.
		if (0>rc)
		{
			AfxMessageBox(_T("메일서버를 설정할 수 없습니다."));
			return;
		}
		
		//-------------------------------------------------------------------
		///Background 모드시 직접 메일 전송
		//-------------------------------------------------------------------
		rc = MailMgr.SendMail(FALSE, TRUE);	// <<< 반드시 두번째 매개변수(Background여부)는 TRUE로 넘겨야 한다.
		if (0>rc)
		{
			AfxMessageBox(_T("메일전송 중 내부오류가 발생하였습니다."));
			return;
		}
	}

	///실제 생성된 log file path 가져오기
	this->m_sLogFilepath = MailMgr.GetLogFilepath();
	ShowMailLog(this->m_sLogFilepath);

	//-------------------------------------------------------------------
	///결과 확인 - 전송성공/실패한 메일 개수 확인
	//-------------------------------------------------------------------
	if (m_ctvBackground)
	{
		sTemp.Format(_T("메일전송이 완료되었습니다. [전송:%d / 실패:%d]"), 
						MailMgr.GetCountMailSended(),		///전송성공한 메일 개수
						MailMgr.GetCountMailFailed()		///전송실패한 메일 개수
			);
		MessageBox(sTemp, _T("확인"), MB_ICONINFORMATION);
	}
	

	//--------------------------------------------------------------------------------------
	///결과 확인 - 각 메일 레코드별로 확인
	///  - Return	-9 : 메일 레코드 인덱스 오류 (zero based)
	///				-1 : 전송실패
	///				 0 : 전송성공
	///				 1 : 전송안함
	//--------------------------------------------------------------------------------------
	ShowMailRecordState( MailMgr.GetMailRecordSendState(0),	///첫번째 메일 레코드 전송결과 확인
	  				     MailMgr.GetMailRecordSendState(1)	///두번째 메일 레코드 전송결과 확인
						 );

}


/************************************************************************/
/* Mail record 추가														*/
/************************************************************************/
INT CMailMgrTestDlg::AddTwoMailRecord(CSendMailMgr* pMailMgr)
{
	INT rc;



	//--------------------------------------------------------------------------------------
	///Mail Record 객체 생성
	//--------------------------------------------------------------------------------------
	SendMailRecord MailRecord;
	

	
	//--------------------------------------------------------------------------------------
	///Mail Record 객체 설정
	//--------------------------------------------------------------------------------------
	
	///첨부파일 개수 설정, 없으면 0
	MailRecord.m_nFileAttachCnt = 2;
	///첨부파일 path 설정. 여러개인 경우 ','(콤마)로 분리한다.
	//MailRecord.m_pFileAttach = _T("Loader.exe,Mail환경설정및테스트.exe");
	MailRecord.m_pFileAttach = _T("..\\Cfg\\MailAccount.cfg,..\\Cfg\\MailAccount.4test.cfg");
	///메일 Body - Encoding이 'text/html' 인 경우 HTML 형식으로
	///          - Encoding이 'text/html' 이 아닌 경우 TEXT 형식으로 
	MailRecord.m_sBody = _T("<head><title>첫번째</title></head><body>첫번째 내용</body>");
	///메일 Encoding 형식
	MailRecord.m_sEncoding = _T("text/html");
	///받는 사람 이름 - '받는사람 이름'
	MailRecord.m_sRecipientName	   = this->m_ctvRecipientName;	
	///받는 사람 전자우편 - 'foo@bar.baz'
	MailRecord.m_sRecipientAddress = this->m_ctvRecipientEmail;	
	///메일 제목(Subject)
	MailRecord.m_sSubject = _T("메일제목 - 첫번째");
	///참조수신자 이름 - '참조수신자 이름'
	MailRecord.m_sCCName    = _T("");
	///참조수신자 전자우편 - 'foooo@baaar.baaaz'
	MailRecord.m_sCCAddress = _T("");

	//--------------------------------------------------------------------------------------
	///Mail Record 객체 추가 - SendMailRecord 객체를 생성하여 추가
	//--------------------------------------------------------------------------------------
	rc = pMailMgr->AddMailRecord(&MailRecord);
	if (0>rc) return -1;
	

	//--------------------------------------------------------------------------------------
	///Mail Record 추가 - 메일레코드 내용을 직접 설정하여 추가
	//--------------------------------------------------------------------------------------
 	rc = pMailMgr->AddMailRecord(_T("메일제목 - 두번째"),
 								 this->m_ctvRecipientName,
 								 this->m_ctvRecipientEmail,
 								 _T("두번째 내용"),
 								 _T("text"),
 								 _T("..\\Cfg\\MailAccount.4test.cfg"),	//_T("Mail환경설정및테스트.exe"),
 								 1,
 								 _T(""),
 								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""),
								 _T(""));
 	if (0>rc) return -2;

	return 0;
}

VOID CMailMgrTestDlg::ShowMailLog(const CString sFielpath)
{
	if (this->m_ctvCleareLog) this->m_ctrlLog.SetWindowText(_T(""));

	CStdioFile file;
	if (!file.Open(sFielpath, CFile::modeRead | CFile::typeBinary))
	{
		//AfxMessageBox(_T("Log 파일이 없습니다."));
		return;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString sLine;

	while (file.ReadString(sLine))
	{
		this->AppendText(sLine + _T("\r\n"));
	}
	file.Close();

	this->m_ctrlLog.SetReadOnly();
	this->m_bLogOutput = TRUE;
}

VOID CMailMgrTestDlg::ShowMailRecordState(INT nRecord1State, INT nRecord2State)
{
	AppendText(_T("\r\n\r\n"));
	AppendText(_T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"));
	this->AppendText(_T("+ 첫번째 메일 : ") + GetMailRecordSendState(nRecord1State) + _T("\r\n"));
	this->AppendText(_T("+ 두번째 메일 : ") + GetMailRecordSendState(nRecord2State) + _T("\r\n"));
	AppendText(_T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"));
}

CString CMailMgrTestDlg::GetMailRecordSendState(INT nState)
{
/** 각 메일레코드 전송여부확인.
@param nRecordIdx 메일 레코드 인덱스(zero-based)
@return -9 : 잘못된 메일레코드 인덱스(zero-based)
@return -3 : MAIL_SEND_NOTCONNECT
             (메일서버 접속실패 - 이후 메일 레코드는 더이상 전송하지 않음)
@return -2 : MAIL_SEND_NONRECIP(메일수신자 없음 - 전송하지 않음)
@return -1 : MAIL_SEND_FAIL(메일레코드 전송실패 - SmtpLog.txt파일을 확인하여야 함)
@return 0  : MAIL_SEND_SUCCESS(메일레코드 전송성공)
@return 1  : MAIL_SEND_NOT(메일레코드 전송하지 않음 
             - 서버 접속실패로 전송 중지 했거나, 사용자가 메일 레코드 전송 체크를 해제함) 
*/

	CString sState;
	switch (nState) {
	case -9 :
		sState = _T("첨자오류");
		break;
	case -3 :
		sState = _T("메일서버접속실패");
		break;
	case -2:
		sState = _T("메일 수신자 없음");
		break;
	case -1:
		sState = _T("전송실패");
		break;
	case 0:
		sState = _T("전송성공");
		break;
	case 1:
		sState = _T("전송안함");
		break;
	default:
		sState.Format(_T("%d:정의안됨"), nState);
	}
	return sState;
}

VOID CMailMgrTestDlg::AppendText(LPCTSTR sLog)
{
	INT Length = this->m_ctrlLog.GetWindowTextLength();
	
	this->m_ctrlLog.SetSel(Length, Length);
	this->m_ctrlLog.ReplaceSel(sLog);
	this->m_ctrlLog.LineScroll( this->m_ctrlLog.GetLineCount() );
}

VOID CMailMgrTestDlg::OnbtnMAILSERVERVIEW() 
{
	UpdateData(TRUE);
	
	UpdateCtrlState(FALSE);
	this->m_ctrlRadioResult.SetWindowText(_T("메일서버설정 보기"));
	SetDlgItemText(IDC_btnMAIL_SERVER, _T("메일테스트용 설정"));

	this->m_ctrlLog.GetWindowText(m_sLogBackup);
	this->m_ctrlLog.SetWindowText(_T(""));
	
	INT nLineCnt=0;
	BOOL bExistServerInfo=FALSE;
	BOOL bExistAccountInfo=FALSE;
	BOOL bExistSenderInfo=FALSE;
	CString sTemp;

	CStdioFile file;
	
	BOOL bOpen = file.Open(m_ctvConfigFilepath, CFile::modeRead | CFile::typeBinary);	//_T("..\\Cfg\\MailAccount.cfg")
	if (!bOpen) 
	{
		AfxMessageBox(_T("[메일 서버 정보파일] 에 지정된 파일을 찾을 수 없습니다."));
		return;
	}
	else 
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		CString sLine;
		
		while (file.ReadString(sLine))
		{
			this->AppendText(sLine + _T("\r\n"));
			nLineCnt++;
			sLine.TrimLeft();
			if (sLine.IsEmpty()) continue;
			if (_T(".") != sLine.Left(1))
			{
				INT idx;
				idx = sLine.Find(_T("="));
				if (0 <= idx)
				{
					sTemp = sLine.Left(idx);
					sTemp.TrimLeft();
					sTemp.TrimRight();
					if (sTemp == _T("메일서버")) 
					{
						sTemp = sLine.Mid(idx+1);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (!sTemp.IsEmpty()) bExistServerInfo = TRUE;
					} 
					else if (sTemp == _T("메일계정")) 
					{
						sTemp = sLine.Mid(idx+1);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (!sTemp.IsEmpty()) bExistAccountInfo = TRUE;
					}
					else if (sTemp == _T("발송자명")) 
					{
						sTemp = sLine.Mid(idx+1);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (!sTemp.IsEmpty()) bExistSenderInfo = TRUE;
					}
				}
				
			}
		}
		file.Close();
		this->m_ctrlLog.SetReadOnly();
	}
	
	if (0==nLineCnt)  
	{
		AfxMessageBox(_T("MailAccount.cfg 파일에 내용이 없습니다."));
		return;
	}
	
	this->m_bLogOutput = FALSE;

	if (bOpen)
	{
		if (!bExistServerInfo || !bExistAccountInfo || !bExistSenderInfo)
		{
			sTemp = _T("[ ");
			if (!bExistServerInfo)  sTemp  += _T("메일서버");
			if (!bExistAccountInfo) 
				(sTemp == _T("[ ")) ? 
					sTemp += _T("메일계정")  :
					sTemp += _T("/메일계정") ;
			if (!bExistSenderInfo)  
				(sTemp == _T("[ ")) ? 
					sTemp += _T("발송자명")  :
					sTemp += _T("/발송자명") ;
			
			AfxMessageBox(_T("MailAccount.cfg 파일에 ") + sTemp + _T(" ] 정보가 없습니다."));
			return;
		}
	}

	//임시 - 테스트
	//MakeTestMailAccountCfgFile(m_ctvConfigFilepath);
}


VOID CMailMgrTestDlg::OnbtnMAILSERVER() 
{
	UpdateData(TRUE);

	this->m_ctrlLog.SetReadOnly(FALSE);
	
	CStdioFile file;

	INT rc = MakeTestMailAccountCfgFile(m_ctvConfigFilepath, FALSE);
	if (0>rc) 
	{
		AfxMessageBox(_T("Mail 전송 테스트를 위한 환경 파일을 생성하지 못했습니다."));
		return;
	}
	ASSERT(!m_sTestCfgFile.IsEmpty());
	
	CString sBtnName;
	GetDlgItemText(IDC_btnMAIL_SERVER, sBtnName);
	if (sBtnName == _T("메일테스트용 설정")) 
	{
		UpdateCtrlState(FALSE);
		SetDlgItemText(IDC_btnMAIL_SERVER, _T("테스트용 설정 저장"));
		this->m_ctrlLog.GetWindowText(m_sLogBackup);
		this->m_ctrlLog.SetWindowText(_T(""));

		INT nLineCnt=0;
		BOOL bExistServerInfo=FALSE;
		BOOL bExistAccountInfo=FALSE;
		BOOL bExistSenderInfo=FALSE;
		CString sTemp;

		//BOOL bOpen = file.Open(m_ctvConfigFilepath, CFile::modeRead);	//_T("..\\Cfg\\MailAccount.cfg")
		BOOL bOpen = file.Open(this->m_sTestCfgFile, CFile::modeRead | CFile::typeBinary);	//_T("..\\Cfg\\MailAccount.cfg")
		if (bOpen)
		{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
			
			CString sLine;
			
			while (file.ReadString(sLine))
			{
				this->AppendText(sLine + _T("\r\n"));
				nLineCnt++;
				sLine.TrimLeft();
				if (sLine.IsEmpty()) continue;
				if (_T(".") != sLine.Left(1))
				{
					//sTemp = sLine.Left(8);
					//if (sTemp == _T("메일서버")) bExistServerInfo = TRUE;
					//if (sTemp == _T("메일계정")) bExistAccountInfo = TRUE;
					//if (sTemp == _T("발송자명")) bExistSenderInfo = TRUE;

					INT idx;
					idx = sLine.Find(_T("="));
					if (0 <= idx)
					{
						sTemp = sLine.Left(idx);
						sTemp.TrimLeft();
						sTemp.TrimRight();
						if (sTemp == _T("메일서버")) 
						{
							sTemp = sLine.Mid(idx+1);
							sTemp.TrimLeft();
							sTemp.TrimRight();
							if (!sTemp.IsEmpty()) bExistServerInfo = TRUE;
						} 
						else if (sTemp == _T("메일계정")) 
						{
							sTemp = sLine.Mid(idx+1);
							sTemp.TrimLeft();
							sTemp.TrimRight();
							if (!sTemp.IsEmpty()) bExistAccountInfo = TRUE;
						}
						else if (sTemp == _T("발송자명")) 
						{
							sTemp = sLine.Mid(idx+1);
							sTemp.TrimLeft();
							sTemp.TrimRight();
							if (!sTemp.IsEmpty()) bExistSenderInfo = TRUE;
						}
					}
					
				}
			}
			file.Close();
		}

		if (!bOpen || 0==nLineCnt)
		{
			if (!bOpen)
			{
				AfxMessageBox(_T("MailAccount.cfg 파일이 없습니다.\n\n")
							  _T("메일보내기 테스트를 위한 서버설정 정보를 입력한 후\n\n")
							  _T("[테스트용 설정 저장] 버튼을 눌러 저장하여 주십시오."));
			}
			this->AppendText(_T(".\r\n"));
			this->AppendText(_T(".파일형식---------------------------\r\n"));
			this->AppendText(_T("....................................\r\n"));
			this->AppendText(_T(". 메일서버 = 메일서버주소\r\n"));
			this->AppendText(_T(". 메일계정 = foo@bar.baz\r\n"));
			this->AppendText(_T(". 발송자명 = 백범\r\n"));
			this->AppendText(_T("....................................\r\n"));
			this->AppendText(_T(".\r\n"));
			this->AppendText(_T("메일서버 = \r\n"));
			this->AppendText(_T("메일계정 = \r\n"));
			this->AppendText(_T("발송자명 = \r\n"));
		}
		if (bOpen)
		{
			this->m_ctrlLog.GetWindowText(sTemp);
			if (!bExistServerInfo || !bExistAccountInfo || !bExistSenderInfo)
				this->AppendText(_T("\r\n.======================================\r\n"));
			if (!bExistServerInfo)  
				this->AppendText(_T(".메일서버 정보가 없습니다.\r\n"));
			if (!bExistAccountInfo) 
				this->AppendText(_T(".메일계정 정보가 없습니다.\r\n"));
			if (!bExistSenderInfo)  
				this->AppendText(_T(".발송자명 정보가 없습니다.\r\n"));
		}
	}
	else if (sBtnName == _T("테스트용 설정 저장")) 
	{
		
		//if (!file.Open(m_ctvConfigFilepath, CFile::modeCreate | CFile::modeWrite))
		if (!file.Open(this->m_sTestCfgFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			AfxMessageBox(_T("메일보내기 테스트를 위한 Config 파일을 저장할 수 없습니다."));
			return;
		}
		
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		CString sText;
		this->m_ctrlLog.GetWindowText(sText);
		sText.Replace(_T("\r"), NULL);
		file.WriteString(sText);
		//AfxMessageBox(sText);
		file.Close();

		SetDlgItemText(IDC_btnMAIL_SERVER, _T("메일테스트용 설정"));
		if (this->m_bLogOutput) this->m_ctrlLog.SetWindowText(this->m_sLogBackup);
		this->m_ctrlLog.SetWindowText(_T(""));
		UpdateCtrlState(TRUE);
		OnKillfocuseRecipientNameOrEmail();
	}
}


VOID CMailMgrTestDlg::OnbtnClose() 
{
	EndDialog(IDCANCEL);
}

VOID CMailMgrTestDlg::UpdateCtrlState(BOOL bEnable/*=TRUE*/)
{
	if (bEnable)
	{
		this->m_ctrlRadioResult.SetWindowText(_T("메일 전송 결과"));
		OnrSEND();
	}
	else
	{
		this->m_ctrlRadioResult.SetWindowText(_T("메일 서버 설정"));
		//((CWnd*)(GetDlgItem(IDC_btnMAIL_SEND)))->EnableWindow(FALSE);
		this->OnKillfocuseRecipientNameOrEmail();
		OnrRESULT();
	}
}

INT CMailMgrTestDlg::MakeTestMailAccountCfgFile(CString sMailAccountFile, BOOL bReCreate/*=TRUE*/) 
{
	ASSERT(!sMailAccountFile.IsEmpty());

	CString sTestCfgFile;
	sTestCfgFile = sMailAccountFile;	// + _T(".4test.cfg");
	sTestCfgFile.Replace(_T(".cfg"), _T(".4test.cfg"));

	CFileStatus status;
	if ( !CFile::GetStatus( sTestCfgFile, status ) || bReCreate )
	{
		BOOL bTestFileCreated = FALSE;
		bTestFileCreated = CopyFile(sMailAccountFile, sTestCfgFile, FALSE);
		if (!bTestFileCreated)
		{
			this->m_sTestCfgFile.Empty();
			return -1;
		}
		//this->m_sTestCfgFile = sTestCfgFile;
	}
	this->m_sTestCfgFile = sTestCfgFile;
		
	return 0;
}


VOID CMailMgrTestDlg::OnchkDELETELOGFILE() 
{
	if (m_sLogFilepath.IsEmpty()) return;
	CFileStatus status;
	if ( CFile::GetStatus( m_sLogFilepath, status ) )
	{
		CFile::Remove(this->m_sLogFilepath);	
	}

	this->m_ctrlDeleteLogFile.SetCheck(0);
}

VOID CMailMgrTestDlg::OnKillfocuseRecipientNameOrEmail() 
{
	UpdateData();
	m_ctvRecipientName.TrimLeft(); 
	m_ctvRecipientName.TrimRight();
	m_ctvRecipientEmail.TrimLeft();
	m_ctvRecipientEmail.TrimRight();

	CString sBtnName;
	this->GetDlgItemText(IDC_btnMAIL_SERVER, sBtnName);

	BOOL bEnable = ( !m_ctvRecipientName.IsEmpty()			&& 
		             !m_ctvRecipientEmail.IsEmpty()			&& 
					 0<m_ctvRecipientEmail.Find(_T("@"))		&& 
					 m_ctvRecipientEmail.Right(1) != _T("@")	&&
					 sBtnName.Compare(_T("테스트용 설정 저장"))		);
	((CWnd*)(GetDlgItem(IDC_btnMAIL_SEND)))->EnableWindow(bEnable);
}

VOID CMailMgrTestDlg::OnSetfocuseRECIPIENTEMAIL() 
{
/*
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	HWND hWnd = ((CEdit*)(GetDlgItem(IDC_eRECIPIENT_EMAIL)))->GetSafeHwnd();	
	hIME = ::ImmGetContext(hWnd);
	if(hIME) 
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_ALPHANUMERIC; 
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		ImmReleaseContext(hWnd, hIME); 
	}
*/
}


VOID CMailMgrTestDlg::OnchkVIEWLOGFILE() 
{
	SetDlgItemText(IDC_btnMAIL_SERVER, _T("메일테스트용 설정"));
	ShowMailLog(this->m_sLogFilepath);
}
