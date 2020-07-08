// SendMailMgr.cpp : implementation file
//

#include "stdafx.h"
#include "SendMailMgr.h"
#include "RegKey.h"
#include "SelectMailServer.h"
#include "Smtp.h"
#include <Mmsystem.h>

#include "RotatingLog.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define LOG_FILE_PATH _T("..\\tunning\\SmtpLog.txt")

/////////////////////////////////////////////////////////////////////////////
// CSendMailMgr dialog


CSendMailMgr::CSendMailMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	//this->m_MailSendYn   = 'O';
	this->m_nRecordCount = 0;
	this->m_nRecordIndex = -1;
	this->m_pRecord      = NULL;
	
	this->m_SystemOS		 = INVALID;
	this->m_sMailAccountPath = _T("");
	
	this->m_nCntSended = 0;
	this->m_nCntFailed = 0;

	this->m_sCfgFilepath = CFG_FILE_PATH;
	this->m_sLogFilepath = this->m_sBackupLogFilepath = LOG_FILE_PATH;

	this->m_bIsConnectedToSMTP = FALSE;

	//{{AFX_DATA_INIT(CSendMailMgr)
	m_sMailAddress = _T("");
	m_sMailServer = _T("");
	//}}AFX_DATA_INIT
}

CSendMailMgr::~CSendMailMgr()
{
	//delete [] m_pRecord;
	//m_pRecord = NULL;
	DeleteAllMailRecord();
	m_Dlg_brush.DeleteObject();
}

VOID CSendMailMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendMailMgr)
	DDX_Control(pDX, IDC_lMAIL, m_ctrlMail);
	DDX_Text(pDX, IDC_eMAIL_ADDRESS, m_sMailAddress);
	DDX_Text(pDX, IDC_eSMTP_SERVER, m_sMailServer);
	//}}AFX_DATA_MAP

/*
	DDX_Control(pDX, IDCANCEL, m_ctrlClose);
	DDX_Control(pDX, IDOK, m_ctrlSend);
	DDX_Control(pDX, IDC_bDELETE_MAT, m_ctrlDeleteMat);
	DDX_Control(pDX, IDC_bSELECT_SMTP_SERVER, m_ctrlSelectSTMPServer);
*/
}


BEGIN_MESSAGE_MAP(CSendMailMgr, CDialog)
	//{{AFX_MSG_MAP(CSendMailMgr)
	ON_NOTIFY(NM_CLICK, IDC_lMAIL, OnClicklMAIL)
	ON_BN_CLICKED(IDC_bSELECT_SMTP_SERVER, OnbSELECTSMTPSERVER)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPLITTER_MOVED, OnWmSplitterMoved)
END_MESSAGE_MAP()

//ON_BN_CLICKED(IDC_bDELETE_MAT, OnbDELETEMAT)
//ON_WM_CANCELMODE()


/////////////////////////////////////////////////////////////////////////////
// CSendMailMgr message handlers

BOOL CSendMailMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_ImageList.Create(IDB_MAIL, 20, 1, RGB(0, 128, 128));
	//m_ctrlMail.SetImageList(&m_ImageList, LVSIL_SMALL);

	// ListCtrl 초기화
	//ListView_SetExtendedListViewStyle(m_ctrlMail.m_hWnd, LVS_EX_CHECKBOXES |  LVS_EX_FULLROWSELECT);

	m_ctrlMail.InsertColumn(0, _T("제목")    , LVCFMT_LEFT, 200);
	m_ctrlMail.InsertColumn(1, _T("받는사람"), LVCFMT_LEFT, 150);
	m_ctrlMail.InsertColumn(2, _T("인코딩")  , LVCFMT_LEFT, 70);
	m_ctrlMail.InsertColumn(3, _T("첨부파일"), LVCFMT_LEFT, 180);
	m_ctrlMail.InsertColumn(4, _T("전송여부"), LVCFMT_LEFT, 150);
	
	// HTMLCtrl 초기화
	//JOB.2019.0073
	/*
	m_Html.CreateFromStatic(IDC_HTML_STATIC, this);
	m_Html.ModifyStyle(0, WS_EX_CLIENTEDGE);
	*/
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

	// TODO: Add extra initialization here

/*
	INT ids;
	ids = this->GetDefaultMailServerInfo();
	if (0!=ids) 
	{
		if (0>ids) 
		{
			AfxMessageBox(_T("메일서버 정보를 설정할 수 없습니다."));
			this->EndDialog(IDCANCEL);
		}
		else
		{
			this->EndDialog(IDCANCEL);
		}
	}
*/

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CSendMailMgr::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	DisplayMailBody(0);		
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
INT CSendMailMgr::DoModal(_MAIL_SERVER_INFO nWhere, 
						  const CString sMailServer/*=_T("")*/, 
						  const CString sMailAccount/*=_T("")*/, 
						  const CString sAccountYN, 
						  const CString sAccountID,
						  const CString sAccountPASS)
{
	// TODO: Add your specialized code here and/or call the base class
	if (MAIL_SERVER_USER == nWhere) 
	{
		if (sMailServer.IsEmpty() || sMailAccount.IsEmpty()) return -1;
		else SetDefaultMailServer(sMailServer, sMailAccount, sAccountYN, sAccountID, sAccountPASS);
	}
	else if (MAIL_SERVER_REGISTRY == nWhere)
	{
		if (0 > SetMailServerInfoFromRegistry(TRUE)) return -1;
	}
	else if (MAIL_SERVER_CFGFILE == nWhere)
	{
		if (0 > SetMailServerInfoFromCfgFile(TRUE)) return -1;
	}
	else
	{
		AfxMessageBox(_T("CSendMailMgr::DoModal(_MAIL_SERVER_INFO nWhere:0..2) Error !!!"));
		return -1;
	}
	return CDialog::DoModal();
}


VOID CSendMailMgr::OnClicklMAIL(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	m_nRecordIndex = pNMListView->iItem;
	
	if (m_nRecordCount < m_nRecordIndex + 1 || m_nRecordIndex == -1) return;
	
	DisplayMailBody(m_nRecordIndex);
	
	*pResult = 0;
}




VOID CSendMailMgr::OnOK() 
{
	this->SendMail(TRUE);
}


VOID CSendMailMgr::OnCancel() 
{
	CDialog::OnCancel();
}




VOID CSendMailMgr::OnDestroy() 
{
	m_ySplitter.Unbind();
	CDialog::OnDestroy();
}

LONG CSendMailMgr::OnWmSplitterMoved(UINT wParam, LONG lParam)
{
	DisplayMailBody(m_nRecordIndex);
    return 1L;
}


INT CSendMailMgr::AddMailRecord(SendMailRecord* pMailRecord)
{
	return AddMailRecord(pMailRecord->m_sSubject, 
						 pMailRecord->m_sRecipientName, 
						 pMailRecord->m_sRecipientAddress, 
						 pMailRecord->m_sBody, 
						 pMailRecord->m_sEncoding, 
						 pMailRecord->m_pFileAttach, 
						 pMailRecord->m_nFileAttachCnt,
						 pMailRecord->m_sCCName,
						 pMailRecord->m_sCCAddress,
						 pMailRecord->m_sMailInfoKey,
						 pMailRecord->m_sLoanKeyList, 
						 pMailRecord->m_sUserKey,
						 pMailRecord->m_sLibName,
						 pMailRecord->m_sMailClass,
						 pMailRecord->m_sManageCode,						 
						 pMailRecord->TO_EMP_CODE
						 );
}


INT  CSendMailMgr::AddMailRecord(CString sSubject, 
								 CString sRecipientName, 
								 CString sRecipientAddress, 
								 CString sBody, 
								 CString sEncoding, 
								 CString pFileAttach, 
								 INT	 nFileAttachCnt,
								 CString sCCName,
								 CString sCCAddress,								 
								 CString sMailInfoKey,
								 CString sLoanKeyList, 
								 CString sUserKey,
								 CString sLibName,
								 CString sMailClass,
								 CString sManageCode,
								 CString strTO_EMP_CODE
								 )
{
	SendMailRecord* pTemp = new SendMailRecord[m_nRecordCount + 1];
	if (pTemp == NULL) {
		AfxMessageBox(_T("메모리가 부족합니다. [AddMailRecord]"));
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
		pTemp[i].m_sCCName			 = m_pRecord[i].m_sCCName;
		pTemp[i].m_sCCAddress		 = m_pRecord[i].m_sCCAddress;
		pTemp[i].m_SendState		 = m_pRecord[i].m_SendState;

			//2004-12-15 
		pTemp[i].TO_EMP_CODE		 = m_pRecord[i].TO_EMP_CODE;

		// 2009.03.03
		pTemp[i].m_sMailInfoKey		 = m_pRecord[i].m_sMailInfoKey;
		pTemp[i].m_sLoanKeyList		 = m_pRecord[i].m_sLoanKeyList;
		pTemp[i].m_sUserKey			 = m_pRecord[i].m_sUserKey;
		pTemp[i].m_sLibName			 = m_pRecord[i].m_sLibName;
		pTemp[i].m_sMailClass		 = m_pRecord[i].m_sMailClass;
		pTemp[i].m_sManageCode		 = m_pRecord[i].m_sManageCode;
	}

	delete [] m_pRecord;
	m_pRecord = NULL;
	m_pRecord = pTemp;

	m_pRecord[m_nRecordCount].m_sSubject          = sSubject;
	m_pRecord[m_nRecordCount].m_sRecipientName    = sRecipientName;
	m_pRecord[m_nRecordCount].m_sRecipientAddress = sRecipientAddress;
	m_pRecord[m_nRecordCount].m_sBody             = sBody;
	m_pRecord[m_nRecordCount].m_sEncoding         = sEncoding;
	m_pRecord[m_nRecordCount].m_pFileAttach		  = pFileAttach;
	m_pRecord[m_nRecordCount].m_nFileAttachCnt    = nFileAttachCnt;
	m_pRecord[m_nRecordCount].m_sCCName			  = sCCName;
	m_pRecord[m_nRecordCount].m_sCCAddress		  = sCCAddress;
	m_pRecord[m_nRecordCount].m_SendState		  = MAIL_SEND_NOT;

	// 2009.03.03
	m_pRecord[m_nRecordCount].m_sMailInfoKey		 = sMailInfoKey;
	m_pRecord[m_nRecordCount].m_sLoanKeyList		 = sLoanKeyList;
	m_pRecord[m_nRecordCount].m_sUserKey			 = sUserKey;
	m_pRecord[m_nRecordCount].m_sLibName			 = sLibName;
	m_pRecord[m_nRecordCount].m_sMailClass		 = sMailClass;
	m_pRecord[m_nRecordCount].m_sManageCode		 = sManageCode;

		//2004-12-15 
	m_pRecord[m_nRecordCount].TO_EMP_CODE		  = strTO_EMP_CODE ;
	
	m_nRecordCount++;

	return 0;
}

VOID CSendMailMgr::DeleteAllMailRecord()
{
	delete [] m_pRecord;
	m_pRecord = NULL;
	m_nRecordCount = 0;
}

INT CSendMailMgr::DeleteMailRecord(INT nIndex, BOOL bBackground/*=FALSE*/)
{
	if (-1 == nIndex) 
	{
		DeleteAllMailRecord();
		return 0;
	}

	SendMailRecord* pTemp = new SendMailRecord[m_nRecordCount - 1];
	if (pTemp == NULL)
	{
		AfxMessageBox(_T("메모리가 부족합니다. [DeleteMailRecord]"));
		return -999;
	}
	
	INT nCurIdx=0;
	for (INT i = 0 ; i < m_nRecordCount ; i++)
	{
		if (nIndex == i) continue;
		
		pTemp[nCurIdx].m_sSubject			= m_pRecord[i].m_sSubject;
		pTemp[nCurIdx].m_sRecipientName		= m_pRecord[i].m_sRecipientName;
		pTemp[nCurIdx].m_sRecipientAddress	= m_pRecord[i].m_sRecipientAddress;
		pTemp[nCurIdx].m_sBody				= m_pRecord[i].m_sBody;
		pTemp[nCurIdx].m_sEncoding			= m_pRecord[i].m_sEncoding;
		pTemp[nCurIdx].m_pFileAttach		= m_pRecord[i].m_pFileAttach;
		pTemp[nCurIdx].m_nFileAttachCnt		= m_pRecord[i].m_nFileAttachCnt;
		pTemp[nCurIdx].m_sCCName			= m_pRecord[i].m_sCCName;
		pTemp[nCurIdx].m_sCCAddress			= m_pRecord[i].m_sCCAddress;
		pTemp[nCurIdx].m_SendState			= m_pRecord[i].m_SendState;
				
		// 2009.03.03
		pTemp[nCurIdx].m_sMailInfoKey		 = m_pRecord[i].m_sMailInfoKey;
		pTemp[nCurIdx].m_sLoanKeyList		 = m_pRecord[i].m_sLoanKeyList;
		pTemp[nCurIdx].m_sUserKey			 = m_pRecord[i].m_sUserKey;
		pTemp[nCurIdx].m_sLibName			 = m_pRecord[i].m_sLibName;
		pTemp[nCurIdx].m_sMailClass			 = m_pRecord[i].m_sMailClass;
		pTemp[nCurIdx].m_sManageCode		 = m_pRecord[i].m_sManageCode;
		pTemp[nCurIdx].TO_EMP_CODE			 = m_pRecord[i].TO_EMP_CODE ;

		nCurIdx++;
	}
	
	delete [] m_pRecord;
	m_pRecord = NULL;
	
	m_pRecord = pTemp;
	m_nRecordCount--;
	
	if (!bBackground) 
	{
		m_ctrlMail.DeleteItem(nIndex);
		ButtonStatus();
	}

	return 0;
}

VOID CSendMailMgr::DisplaySendMailRecord()
{
	// 초기화
	m_ctrlMail.DeleteAllItems();
	m_ctrlMail.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	
	LV_ITEM lvitem;
	
	lvitem.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvitem.iSubItem  = 0;
	lvitem.iImage    = 0;
	lvitem.stateMask = LVIS_STATEIMAGEMASK;
	lvitem.state     = INDEXTOSTATEIMAGEMASK(1);
	
	CString sRecipient;
	CString sCC;

	for (INT i = 0; i < m_nRecordCount; i++)
	{
		sRecipient.Format(_T("\"%s\" <%s>"), 
						  m_pRecord[i].m_sRecipientName, 
						  m_pRecord[i].m_sRecipientAddress);
		
		if ( !m_pRecord[i].m_sCCName.IsEmpty() && !m_pRecord[i].m_sCCAddress.IsEmpty() ) 
		{
			sCC.Format(_T("\"%s\" <%s>"), m_pRecord[i].m_sCCName, m_pRecord[i].m_sCCAddress);
			sRecipient += _T(" ; ") + sCC;
		}
		lvitem.iItem   = m_ctrlMail.GetItemCount();
		lvitem.pszText = (LPTSTR)(LPCTSTR) m_pRecord[i].m_sSubject;
		
		// 새로운 행 등록
		m_ctrlMail.InsertItem(&lvitem);
		if (0 == m_ctrlMail.SetItemText(lvitem.iItem,1,_T("0"))) 
		{
			AfxMessageBox(_T("fail insert"));
		}	
		
		// 각 칼럼의 텍스트 설정
		m_ctrlMail.SetItemText(lvitem.iItem, 1, sRecipient);
		m_ctrlMail.SetItemText(lvitem.iItem, 2, m_pRecord[i].m_sEncoding);
		m_ctrlMail.SetItemText(lvitem.iItem, 3, m_pRecord[i].m_pFileAttach);
		this->m_ctrlMail.SetCheck(i, TRUE);
	}
}

VOID CSendMailMgr::DisplayMailBody(INT nIndex)
{
	if (nIndex < 0) return;

	CString sExt;
	if (m_pRecord[nIndex].m_sEncoding.Compare(_T("text/html")) == 0)
		sExt = _T("html");
	else
		sExt = _T("txt");

	// 임시 파일을 만든다
	FILE* fp;

	CString sFileName      = _T("SendMailMgrTmp.");
	CString sWindowTempDir = GetWindowTempDirectory();
	if (sWindowTempDir.IsEmpty()) {
		MessageBox(_T("TEMP 파일을 열지 못했습니다.[D]"), _T("경고"), MB_ICONEXCLAMATION);
		return;
	}
	
	sFileName += sExt;
	//sFileName = _T("\\Temp\\") + sFileName;
	sFileName = _T("\\") + sFileName;
	sFileName = sWindowTempDir + sFileName;

	if (NULL == (fp = _tfopen(sFileName.GetBuffer(0), _T("wb"))))
	{
		MessageBox(_T("TEMP 파일을 열지 못했습니다.[F]"), _T("경고"), MB_ICONEXCLAMATION);
		return;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	_fputts(m_pRecord[nIndex].m_sBody, fp);
	fclose(fp);
	//JOB.2019.0073
	//m_Html.Navigate2(sFileName);
		
	// 파일을 지운다
}


INT CSendMailMgr::SendMail(BOOL bSendMailChecked/*=TRUE*/, BOOL bBackground/*=FALSE*/)
{

	return SendMail_SMTP(bSendMailChecked, bBackground);

//	2009.03.04 ADD BY CJY : 데몬이 완성될때까지 임시로 SMTP로만 전송
// 	CFileFind FindFile;
// 	if(TRUE == FindFile.FindFile(_T("..\\cfg\\_smtp")))
// 	{
// 		return SendMail_SMTP(bSendMailChecked, bBackground);
// 	}
// 	else
// 	{
// 		return SendMail_DAEMON(bSendMailChecked, bBackground);
// 	}
	return 0;
}


/** 각 메일레코드 전송여부확인.
	 @param nRecordIdx 메일레코드의 인덱스(zero-based)
	 @return -9 : 잘못된 메일레코드 인덱스(zero-based)
	 @return -3 : MAIL_SEND_NOTCONNECT(메일서버 접속실패 - 이후 메일 레코드는 더이상 전송하지 않음)
	 @return -2 : MAIL_SEND_NONRECIP(메일수신자 없음 - 전송하지 않음)
	 @return -1 : MAIL_SEND_FAIL(메일레코드 전송실패 - SmtpLog.txt파일을 확인하여야 함)
	 @return  0 : MAIL_SEND_SUCCESS(메일레코드 전송성공)
	 @return  1 : MAIL_SEND_NOT(메일레코드 전송하지 않음 - 서버 접속실패로 전송 중지 했거나, 
								 사용자가 메일 레코드 전송 체크를 해제함)	
*/
INT CSendMailMgr::GetMailRecordSendState(INT nRecordIdx)
{
	if (0>nRecordIdx || nRecordIdx>=this->m_nRecordCount) return -9;
	return this->m_pRecord[nRecordIdx].m_SendState;
}



CString CSendMailMgr::GetWindowTempDirectory()
{
	DWORD dwResult; 

	#define  BUFFER_SIZE 1024
	TCHAR tchWindowTempDir[BUFFER_SIZE];
	*tchWindowTempDir = 0; 

	dwResult = GetSystemDirectory(tchWindowTempDir, BUFFER_SIZE); 
	if (0==dwResult) return _T("");

	return CString(tchWindowTempDir);
}


INT CSendMailMgr::GetSystemInfo()
{
	if (0 > this->GetSystemVersion()  ) return -1;
	if (0 > this->GetMailAccountPath()) return -2;
	return 0;
}


INT CSendMailMgr::GetSystemVersion()
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

INT CSendMailMgr::GetMailAccountPath()
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
						_T("Software\\Microsoft\\Protected Storage System Provider"),
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


VOID CSendMailMgr::CutNameAndAddress(CString sCutObject, CString& sName, CString& sAddress)
{
	INT  n1stIndex;
	INT  n2ndIndex;

	// _T("이름") <주소>
	n1stIndex = sCutObject.Find(_T("\""));
	n2ndIndex = sCutObject.Find(_T("\""), n1stIndex + 1);

	if (n1stIndex == -1)	// 없을 경우
	{
		sName    = _T("");
		sAddress = sCutObject;

		return;
	}

	sName = sCutObject.Mid(n1stIndex + 1, n2ndIndex - n1stIndex - 1);

	n1stIndex = sCutObject.Find(_T("<"));
	n2ndIndex = sCutObject.Find(_T(">"));

	if (n1stIndex == -1)
	{
		sAddress = _T("");
		
		return;
	}

	sAddress = sCutObject.Mid(n1stIndex + 1, n2ndIndex - n1stIndex - 1);
}

VOID CSendMailMgr::ButtonStatus()
{
	if (m_nRecordCount > 0)
	{
		GetDlgItem(IDC_bSELECT_SMTP_SERVER)->EnableWindow(TRUE );
		GetDlgItem(IDC_bDELETE_MAT        )->EnableWindow(TRUE );
		GetDlgItem(IDOK                   )->EnableWindow(TRUE );
	}
	else
	{
		GetDlgItem(IDC_bSELECT_SMTP_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE_MAT        )->EnableWindow(FALSE);
		GetDlgItem(IDOK                   )->EnableWindow(FALSE);
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID CSendMailMgr::OnbSELECTSMTPSERVER() 
{
	CSelectMailServer dlg(this);
	
	if (!m_sDefaultMailServer.IsEmpty() && !m_sDefaultMailAddress.IsEmpty())
	{
		dlg.AddMailServerRecord(m_sDefaultMailServer, m_sDefaultMailAddress);
	}
	
	if (dlg.DoModal() == IDCANCEL) return;
	
	dlg.GetMailAccount(m_sMailServer, m_sMailAddress);
	
	UpdateData(FALSE);
	
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID CSendMailMgr::SetDefaultMailServer(CString  sMailServer, 
										CString  sMailAddress, 
										CString  sAccountYN,
										CString  sSendID,
										CString  sSendPASS)
{
	this->m_sMailServer  = this->m_sDefaultMailServer  = sMailServer;
	this->m_sMailAddress = this->m_sDefaultMailAddress = sMailAddress;
	this->m_strAccountYN = this->m_strAccountYN = sAccountYN;
	this->m_strAccountID = this->m_strAccountID = sSendID;
	this->m_strAccountPASS = this->m_strAccountPASS = sSendPASS;
}

/************************************************************************/
/* Registry에서 Mail Server/Account 정보 가져오기.						*/
/************************************************************************/
INT CSendMailMgr::SetMailServerInfoFromRegistry(BOOL bBackground/*=FALSE*/)
{
	LONG lRet;

	lRet = this->GetSystemInfo();
	if (0>lRet) 
	{
		CString sTemp;
		sTemp.Format(_T("메일서버 정보를 설정할 수 없습니다. [GetSystemInfo() = %l]"), lRet);
		AfxMessageBox(sTemp);
		return -999;
	}
	
	CRegKeyMgr RegKey;
	
	//CString sMailAccountPath = _T(".DEFAULT\\SOFTWARE\\Microsoft\\Internet Account Manager");
	CString sMailAccountPath = this->m_sMailAccountPath;
	
	CString sDefalutMailAccount;
	CString sSMTPServer;
	CString sSMTPEmailAddress;
	CString sSMTPDisplayName;
	CString sEmailAddress;
	
	//LONG	lRet;
	HKEY    hKeyRoot = HKEY_USERS;
	
	sDefalutMailAccount.Empty();
	
	lRet = RegKey.Open(hKeyRoot, sMailAccountPath);
	if (ERROR_SUCCESS != lRet) return -1;
	
	lRet = RegKey.Read(_T("Default Mail Account"), sDefalutMailAccount);
	RegKey.Close();	
	if (ERROR_SUCCESS != lRet) return -2;
	
	if (sDefalutMailAccount.IsEmpty()) {
		MessageBox( _T("메일서버 정보가 없습니다.\n\n")
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
	
	this->SetDefaultMailServer(this->m_sMailServer, 
							   this->m_sMailAddress, 
							   this->m_strAccountYN, 
							   this->m_strAccountID,
							   this->m_strAccountPASS);
	
	if (!bBackground) UpdateData(FALSE);
	
	return 0;
}

/************************************************************************/
/* Config file에서 Mail Server/Account 정보 가져오기					*/
/************************************************************************/
INT CSendMailMgr::SetMailServerInfoFromCfgFile(BOOL bBackground/*=FALSE*/)
{
	//메일서버 =  
	//메일계정 = 
	//발송자명 = 

	CFile file;
	if (!file.Open(m_sCfgFilepath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("메일서버설정 파일이 없습니다."));
		return -999;
	}
	//++2008.11.19 UPDATE BY CJY {{++
	// 변경사항 : 파일을 ReadString으로 읽지 않는다.
	// 파일을 읽어올 때, ANSI의 깨진 글자로 읽어오기 때문에
	// ReadString 함수가 제대로 불러오지 못한다.
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}		

	this->m_sMailServer.Empty();
	this->m_sMailAddress.Empty();
	
	CString sEmailAddress;
	CString sSenderName;
	CString strAccountID;
	CString strAccountPASS;
	CString strAccountYN;
	
	CString sTemp;	
	CString strLine;

	BOOL bIsEnd = FALSE;

	TCHAR lpBuffer[4096];
	DWORD dwPos = file.GetPosition();
	while(TRUE)
	{
		memset(lpBuffer, '\0', 4096);
		dwPos = file.GetPosition();
		INT nCount = file.Read(lpBuffer, 4096);
		DWORD dw_Pos = file.GetPosition();
		if(!nCount)
		{
			break;
		}
		strLine.Format(_T("%s"), lpBuffer);
		INT nFindPos = strLine.Find(_T("\n"));
		if(-1 == nFindPos)
		{
			if(FALSE == bIsEnd)
			{
				strLine = strLine + _T("\r\n");
				nFindPos = strLine.Find(_T("\n"));

				bIsEnd = TRUE;
			}
			else break; 
		}
		strLine = strLine.Left(nFindPos);
		file.Seek(dwPos + (DWORD)(nFindPos*sizeof(TCHAR))+sizeof(TCHAR), CFile::begin);	
	
//DEL	이전소스	
//DEL 	CStdioFile file;
//DEL 	if (!file.Open(m_sCfgFilepath, CFile::modeRead | CFile::typeBinary))
//DEL 	{
//DEL 		AfxMessageBox(_T("메일서버설정 파일이 없습니다."));
//DEL 		return -999;
//DEL 	}
//DEL 	TCHAR cUni;
//DEL 	file.Read( &cUni, sizeof(TCHAR) );
//DEL 	if( 0xFEFF != cUni )
//DEL 	{
//DEL 		file.SeekToBegin();
//DEL 	}
	//--2008.11.19 UPDATE BY CJY --}}

//DEL 	this->m_sMailServer.Empty();
//DEL 	this->m_sMailAddress.Empty();
//DEL 	
//DEL 	CString sEmailAddress;
//DEL 	CString sSenderName;
//DEL 	CString strAccountID;
//DEL 	CString strAccountPASS;
//DEL 	CString strAccountYN;
//DEL 	
//DEL 	CString sTemp;	
//DEL 	CString strLine;
//DEL 	while (file.ReadString(strLine))
//DEL 	{
	//--2008.11.19 UPDATE BY CJY --}}
		strLine.TrimLeft();
		if (strLine.Left(1) == _T(".") || strLine.IsEmpty()) continue;
		
		INT idx;
		idx = strLine.Find(_T("="));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if (sTemp == _T("메일서버")) 
			{
				this->m_sMailServer = strLine.Mid(idx+1);
				this->m_sMailServer.TrimLeft();
				this->m_sMailServer.TrimRight();
			} 
			else if (sTemp == _T("메일계정")) 
			{
				sEmailAddress = strLine.Mid(idx+1);
				sEmailAddress.TrimLeft();
				sEmailAddress.TrimRight();
			}
			else if (sTemp == _T("발송자명")) 
			{
				sSenderName = strLine.Mid(idx+1);
				sSenderName.TrimLeft();
				sSenderName.TrimRight();
			}
			else if (sTemp == _T("보내는메일인증사용")) 
			{
				strAccountYN = strLine.Mid(idx+1);
				strAccountYN.TrimLeft();
				strAccountYN.TrimRight();				
			}
			else if (sTemp == _T("인증계정")) 
			{
				strAccountID = strLine.Mid(idx+1);
				strAccountID.TrimLeft();
				strAccountID.TrimRight();
			}
			else if (sTemp == _T("인증암호")) 
			{
				strAccountPASS = strLine.Mid(idx+1);
				strAccountPASS.TrimLeft();
				strAccountPASS.TrimRight();
			}
		}
	}
	
	if (this->m_sMailServer.IsEmpty() || sEmailAddress.IsEmpty())
	{
		sTemp.Format(_T("메일서버설정 파일에서 메일서버 정보를 읽어올 수 없습니다.\n")
			         _T(" - 메일서버 : %s\n")
					 _T(" - 메일계정 : %s"),
					 m_sMailServer, sEmailAddress
					 );
		AfxMessageBox(sTemp);
		return -2;
	}
	this->m_sMailAddress.Format(_T("\"%s\" <%s>"), sSenderName, sEmailAddress);
	this->m_strAccountYN = strAccountYN;
	this->m_strAccountID = strAccountID;
	this->m_strAccountPASS = strAccountPASS;
	this->SetDefaultMailServer(this->m_sMailServer,
							   this->m_sMailAddress,
							   this->m_strAccountYN,
							   this->m_strAccountID,
							   this->m_strAccountPASS);

	if (!bBackground) UpdateData(FALSE);

	return 0;
}



BOOL CSendMailMgr::Make_5163_MsgFile(CString strFilePath,CString strBody)
{

	CStdioFile msgFile ;
	if(!msgFile.Open (strFilePath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
	{
		AfxMessageBox(_T("메일발송을 위한 내용파일을  생성하지 못하였습니다.")) ;
		return FALSE ;
	}
		
	if( 0 == msgFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		msgFile.Write( &cUni, sizeof(TCHAR));
	}
    
	//==========================================================================
	// msg 파일 정보 입력 
	CString strTemp = _T("")  ;
	
	//1.SLET
	//strTemp.Format (_T("SLET=MailSend \r\n")) ;
	//msgFile.WriteString (strTemp) ;
	//strTemp.Format (_T("SENDER_EMP_CODE=72186 \r\n")) ;
	msgFile.WriteString (strBody) ;

	msgFile.Close () ;
	
    
	return TRUE ;
}

BOOL CSendMailMgr::Make_5163_InfoFile(CString strSender,CString strTo,CString strSubject,CString strBody,INT nIdx)
{

	CString strInfoFilePath = _T("");
	CString strMsgFilePath = _T("") ;
	CString strMsgFileName = _T("") ;
	CString strInfoFileName = _T("") ;
	//100001200412141613042.msg
	//100001 200412141613 042.msg
	
    //2004-12-23
	// 2005.02.25 REM BY PDJ
	//Sleep (1000) ;

	CString strCurrentDate = _T("");	
	LONG Time1=0;
	CString	strYear;


	// 2005.02.25 3st UPDATE BY PDJ
	CTime t = CTime::GetCurrentTime () ;
	Time1 = timeGetTime();
	strYear.Format(_T("%d"), t.GetYear ());
	CString	strTimeKey;
	
	strTimeKey.Format(_T("%08d"), Time1 );
	strTimeKey = strTimeKey.Left(8);

	//2004-12-23	
	// 2005.02.25 3st UPDATE BY PDJ
	// 기존형식으로는 sleep을 줄수 밖에 없다.
	strCurrentDate.Format (_T("%s%02d%02d%08d"),strYear.Right(2),t.GetMonth (),t.GetDay (),_ttoi(strTimeKey)) ;
	// 2. strCurrentDate.Format (_T("%04d%02d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (),t.GetHour (),t.GetMinute (), t.GetSecond ()+1) ;
	// 1. strCurrentDate.Format (_T("%04d%02d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (),t.GetHour (),t.GetMinute (),t.GetSecond ()) ;

    CTime resultTime = t + CTimeSpan(4, 0, 0, 0);
	CString strWASTE_DATE ;
	//strWASTE_DATE.Format (_T("%04d%02d%02d%02d%02d%02d"),resultTime.GetYear (),resultTime.GetMonth (),resultTime.GetDay (),resultTime.GetHour (),resultTime.GetMinute (),resultTime.GetSecond ()) ;
    strWASTE_DATE.Format (_T("%04d%02d%02d2300"),resultTime.GetYear (),resultTime.GetMonth (),resultTime.GetDay ()) ;

	CString strMakeFilePath = _T("") ;
	strMakeFilePath = GetMakeFilePath () ;
	if(strMakeFilePath.IsEmpty ())
	{
		AfxMessageBox (_T("파일 생성을 위한 PATH 를 찾지 못하였습니다.")) ;
		return FALSE ;
	}

	/*
	strInfoFilePath.Format (_T("%s\\100001%s%03d.msg"),strMakeFilePath,strCurrentDate ,nIdx) ;
    strMsgFilePath.Format  (_T("%s\\200001%s%03d.txt"),strMakeFilePath,strCurrentDate ,nIdx) ;	
	strMsgFileName.Format (_T("200001%s%03d.txt"),strCurrentDate ,nIdx) ;	
	strInfoFileName.Format (_T("100001%s%03d.txt"),strCurrentDate ,nIdx) ;	
    */

	//2004-12-23
	// 2005.02.17 ADD BY PDJ
	// MSG파일의 개수가 너무 많아 날짜별로 폴더를 만들어서 생성.
	CString	strDate;
	strDate.Format(_T("%s"), CTime::GetCurrentTime().Format(_T("%Y_%m_%d")) );
	strInfoFilePath.Format (_T("%s\\%s\\100001%s%d.msg"),strMakeFilePath,strDate,strCurrentDate ,0) ;
    strMsgFilePath.Format  (_T("%s\\%s\\200001%s%02d.txt"),strMakeFilePath,strDate,strCurrentDate ,0) ;	
	strMsgFileName.Format (_T("200001%s%02d.txt"),strCurrentDate ,0) ;	
	strInfoFileName.Format (_T("100001%s%d.msg"),strCurrentDate ,0) ;	

	// 날짜 폴더 만들기
	TCHAR szPath[1024];
	_stprintf(szPath, _T("%s\\%s"), strMakeFilePath,strDate );
	_tmkdir(szPath);


	CStdioFile infoFile ;
	if(!infoFile.Open (strInfoFilePath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
	{
		AfxMessageBox(_T("메일발송을 위한 파일 정보를 생성하지 못하였습니다.")) ;
		return FALSE ;
	}
	
	if( 0 == infoFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		infoFile.Write( &cUni, sizeof(TCHAR));
	}
		
	//==========================================================================
	// msg 파일 정보 입력 
	CString strTemp = _T("")  ;
	
	//1.SLET
	strTemp.Format (_T("SLET=MailSend\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//2.SENDER_EMP_CODE=72186
	strTemp.Format (_T("SENDER_EMP_CODE=%s\r\n"),m_str5163SendEmpCode) ;
	infoFile.WriteString (strTemp) ;

	//3.TO_EMP_CODE=25364
	strTemp.Format (_T("TO_EMP_CODE=%s\r\n"),strTo ) ;
	infoFile.WriteString (strTemp) ;

	//4.SUBJECT=신청하신 자료 입수 되었습니다.
	strTemp.Format (_T("SUBJECT=%s\r\n"),strSubject ) ;
	infoFile.WriteString (strTemp) ;

	//5.BODY_SRC_FILE_NAME=temp.txt
	strTemp.Format (_T("BODY_SRC_FILE_NAME=temp.txt\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//6.BODY_PHY_FILE_NAME=2000012000412121010420.txt
	strTemp.Format (_T("BODY_PHY_FILE_NAME=%s\r\n"),strMsgFileName) ;
	infoFile.WriteString (strTemp) ;

	//7.PRINT_DISABLED=0
	strTemp.Format (_T("PRINT_DISABLED=0\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//8.DOWNLOAD_DISABLED=0
	strTemp.Format (_T("DOWNLOAD_DISABLED=0\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//9.FORWARD_DISABLED=0
	strTemp.Format (_T("FORWARD_DISABLED=0\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//10.EMERGENCY=0
	strTemp.Format (_T("EMERGENCY=0\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//11.SECURITY=0
	strTemp.Format (_T("SECURITY=0\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//12.SEND_DATE=200412091727
	CString strCurTime;
	t = CTime::GetCurrentTime () ;
	strCurTime.Format (_T("%04d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (),t.GetHour (),t.GetMinute () ) ;
	strTemp.Format (_T("SEND_DATE=%s\r\n"),strCurTime) ;
	infoFile.WriteString (strTemp) ;

	//13.WASTE_DATE=200412132300
	strTemp.Format (_T("WASTE_DATE=%s\r\n"),strWASTE_DATE) ;
	infoFile.WriteString (strTemp) ;

	//14.MAIL_TYPE=문헌정보
	strTemp.Format (_T("MAIL_TYPE=문헌정보\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//15.ALL_CNT=0
	strTemp.Format (_T("ATT_CNT=0\r\n")) ;
	infoFile.WriteString (strTemp) ;

	//==========================================================================
	infoFile.Close () ;
	
    
	BOOL bCheck = Make_5163_MsgFile(strMsgFilePath,strBody) ;
	if(bCheck == FALSE) 
	{
		return FALSE ;
	}
	
	//메일 프로그램 실행 
	//bCheck = EXE_5163PROCESS(strInfoFileName ,strMsgFileName ) ;
	bCheck = EXE_5163PROCESS(strInfoFilePath ,strMsgFilePath ) ;
	if(bCheck == FALSE) 
	{
		return FALSE ;
	}


	return TRUE ;
}

CString CSendMailMgr::GetMakeFilePath()
{
	CString strPath ;
	
	CString strTemp;
	strTemp = _T("..\\cfg\\MailSystem.cfg") ;
	CStdioFile file ;
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) return _T("") ;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine ;
	CString sTemp ;
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;	

		INT idx;
		idx = strLine.Find(_T("="));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if (sTemp == _T("OUTPUT_FILE_PATH")) 
			{
				strPath = strLine.Mid(idx+1);
				strPath.TrimLeft();
				strPath.TrimRight();
			} 
		}
	}

	file.Close () ;

	return strPath ;
}

BOOL CSendMailMgr::Check_5163MailSystem()
{

	CString strCheck ;
	
	CString strTemp;
	strTemp = _T("..\\cfg\\MailSystem.cfg") ;
	CStdioFile file ;
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) return FALSE ;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine ;
	CString sTemp ;
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;	

		INT idx;
		idx = strLine.Find(_T("["));
		if (0 <= idx)
		{
			INT nPos = strLine.Find (_T(":")) ;

			if(0 <= nPos) 
			{
				//sTemp = strLine.Left (nPos) ;
				sTemp = strLine.Mid (idx+1,nPos-1) ;
				sTemp.TrimLeft();
				sTemp.TrimRight();
				if (sTemp == _T("5163")) 
				{
					strCheck = strLine.Mid(nPos+1);	
					INT nEnd = strCheck.Find (_T("]")) ;
					strCheck = strCheck.Mid (0,nEnd);
					strCheck.TrimLeft();
					strCheck.TrimRight();
				}
				else continue ;
			}
		}

		idx = strLine.Find(_T("="));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if (sTemp == _T("EXE_PATH")) 
			{
				m_str5163MailProgPath = strLine.Mid(idx+1);
				m_str5163MailProgPath.TrimLeft();
				m_str5163MailProgPath.TrimRight();
			} 
			if (sTemp == _T("EXE_FILE_NAME")) 
			{
				m_str5163MailProgName  = strLine.Mid(idx+1);
				m_str5163MailProgName .TrimLeft();
				m_str5163MailProgName .TrimRight();
			} 
			if (sTemp == _T("SENDER_EMP_CODE")) 
			{
				m_str5163SendEmpCode  = strLine.Mid(idx+1);
				m_str5163SendEmpCode .TrimLeft();
				m_str5163SendEmpCode .TrimRight();
			} 
			//2004-12-23 leehakjoong
			if (sTemp == _T("EXE_PROGRAM_SHOW")) 
			{
				m_str5163EXEProg  = strLine.Mid(idx+1);
				m_str5163EXEProg .TrimLeft();
				m_str5163EXEProg .TrimRight();
			} 


		}
		
		


	}
	file.Close () ;

	if(strCheck == _T("TRUE") )
	{
		return TRUE ;
	}
	else
	{
		return FALSE ;
	}


	return TRUE ;
}

INT CSendMailMgr::SendMail_5163(BOOL bSendMailChecked/*=TRUE*/, BOOL bBackground/*=FALSE*/)
{
	
	//Make_5163_InfoFile(NULL,1) ;
	

	BOOL bMailChecked = FALSE;
	
	///리스트 컨트롤의 메일전송여부 컬럼
	const INT nCol_SendYN = 4;
	
	this->m_nCntSended = 0;
	this->m_nCntFailed = 0;
	
	if (!bBackground) UpdateData(TRUE);
	

	if (m_sMailAddress.IsEmpty() == TRUE) {
		MessageBox(_T("메일 보내는 사람이 지정되지 않았습니다."), _T("경고"), MB_ICONEXCLAMATION);
		return -2;
	}
	
	//###################################################
	CRotatingLog Log((LPCTSTR)m_sBackupLogFilepath, DATE_FILE_NAME);
	m_sLogFilepath = Log.GetLogFilepath();
	
	for (INT i = 0; i < m_nRecordCount; i++) 
	{
		
		CString strSENDER_EMP_CODE ;
		CString strTO_EMP_CODE ;
		CString strSUBJECT ;
		CString strBODY ;


		m_pRecord[i].m_SendState = MAIL_SEND_NOT;
		
		if (!bBackground) 
		{
			m_ctrlMail.SetItemText(i, nCol_SendYN, _T(""));
			bMailChecked = this->m_ctrlMail.GetCheck(i);
			if (bSendMailChecked && !bMailChecked) continue;
			
			m_ctrlMail.SetItemText(i, nCol_SendYN, _T("전송시작..."));
			this->DisplayMailBody(i);
		}
		
//		if (m_pRecord[i].m_sRecipientAddress.IsEmpty() && 
//			m_pRecord[i].m_sCCAddress.IsEmpty()			) 
//		{
//			if (!bBackground) 
//			{
//				m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일수신자없음"));
//				this->DisplayMailBody(i);
//			}
//			m_pRecord[i].m_SendState = MAIL_SEND_NONRECIP;
//			this->m_nCntFailed++;
//			continue;
//		}
	
	

		strSUBJECT         = m_pRecord[i].m_sSubject;
		strSENDER_EMP_CODE = m_pRecord[i].SENDER_EMP_CODE ;
		strTO_EMP_CODE     = m_pRecord[i].TO_EMP_CODE ;
		strBODY            = m_pRecord[i].m_sBody;
        
		//2005-01-03
		//strBODY.Replace (_T("<br>"),_T("\r\n")) ;
		strBODY.Replace (_T("<br>"),_T("\r\n")) ;	
		strBODY.Replace (_T("<BR>"),_T("\r\n")) ;	


		// Here are some samples of adding attachments to the message
		INT next = 0;
		INT pre = 0;
		

		m_pRecord[i].m_SendState = MAIL_SEND_NOTCONNECT;
		
		if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일파일 생성중.."));		
	

		
		BOOL nFail = Make_5163_InfoFile (strSENDER_EMP_CODE,strTO_EMP_CODE,strSUBJECT ,strBODY,i) ;
		if (nFail == TRUE) 
		{
			this->m_nCntSended++;
			if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일파일 생성 성공"));
			m_pRecord[i].m_SendState = MAIL_SEND_SUCCESS;
			
			Log.StartAddLineStop(_T("[MailMgr][SENDING ][03][O] 메일파일 생성 성공 !!!"));
		}
		else
		{
			this->m_nCntFailed++;
			if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일 파일 생성 실패"));
			m_pRecord[i].m_SendState = MAIL_SEND_FAIL;
			
			Log.StartAddLineStop(_T("[MailMgr][SENDING ][03][X] Send fail !!!"));
		}
		//mail.Close();

		//###################################################
	}
	
	if (!bBackground) 
	{
		CString sTemp;
		sTemp.Format(_T("메일전송이 완료되었습니다. [전송:%d / 실패:%d]"), this->m_nCntSended, this->m_nCntFailed);
		if ( this->m_nCntFailed > 0 )
		{
			sTemp += _T("\r\n\r\n* 메일전송이 실패한 경우 다음 사항을 확인하세요\r\n")
				_T("· 계정의 등록여부 및 패스워드, SMTP인증 사용여부\r\n")
				_T("· 스파이웨어, 방화벽, 안티바이러스 프로그램의 차단여부\r\n")
				_T("· SMTP서버의 스팸방지 여부");
		}
		MessageBox(sTemp, _T("확인"), MB_ICONINFORMATION);
	}
	
	Log.StopLogging();
	
	
	return 0 ;
}

BOOL CSendMailMgr::EXE_5163PROCESS(CString strInfoFile, CString strMsgFile)
{
   	//2004-12-23 LEEHAKJOONG
   	INT ids ;
	
	CString strEXEFileName;
	CString strEXEFileName1;
	//strEXEFileName.Format(_T("%s %s"), m_str5163MailProgName, strMsgFile);
	strEXEFileName.Format(_T("%s %s"), m_str5163MailProgName, strInfoFile);
	strEXEFileName1.Format(_T("%s %s"), m_str5163MailProgName, strMsgFile);
	
	if(m_str5163EXEProg == _T("FALSE")) 
	{
#ifdef _UNICODE
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strEXEFileName1.GetBuffer(0), -1, NULL, 0, NULL,NULL);
		char* ctmp = new char[len]; 
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strEXEFileName1.GetBuffer(0), -1, ctmp, len, NULL, NULL);
		ids = ::WinExec(ctmp, SW_HIDE);
		delete []ctmp;

		len = WideCharToMultiByte(CP_ACP, 0, strEXEFileName.GetBuffer(0), -1, NULL, 0, NULL,NULL);
		ctmp = new char[len]; 
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strEXEFileName.GetBuffer(0), -1, ctmp, len, NULL, NULL);
		ids = ::WinExec(ctmp, SW_HIDE);
		delete []ctmp;		
#else
		ids = ::WinExec(strEXEFileName1, SW_HIDE);
		ids = ::WinExec(strEXEFileName, SW_HIDE);
#endif
		
	}
	else
	{
#ifdef _UNICODE
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strEXEFileName1.GetBuffer(0), -1, NULL, 0, NULL,NULL);
		char* ctmp = new char[len]; 
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strEXEFileName1.GetBuffer(0), -1, ctmp, len, NULL, NULL);
		ids = ::WinExec(ctmp, SW_SHOW);
		delete []ctmp;

		len = WideCharToMultiByte(CP_ACP, 0, strEXEFileName.GetBuffer(0), -1, NULL, 0, NULL,NULL);
		ctmp = new char[len]; 
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strEXEFileName.GetBuffer(0), -1, ctmp, len, NULL, NULL);
		ids = ::WinExec(ctmp, SW_SHOW);
		delete []ctmp;
#else
		ids = ::WinExec(strEXEFileName1, SW_SHOW);
	    ids = ::WinExec(strEXEFileName, SW_SHOW);
#endif		
	}
	// If the WinExec function succeeds, the return value is greater than 31.
	if (ids <= 31) {
		CString sTemp;
		sTemp.Format(_T("프로그램을 실행하던중 예상치 못한 에러가 발생하였습니다. [%d]"), ids);
		AfxMessageBox(sTemp);

		return FALSE ;
	}

	return TRUE ;
}

HBRUSH CSendMailMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = NULL;
	if(pWnd->m_hWnd == this->m_hWnd) hbr = HBRUSH(m_Dlg_brush);
	else hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(239, 239, 239));

		return m_Dlg_brush;
	}

	return hbr;
}


int CSendMailMgr::SendMail_DAEMON(BOOL bSendMailChecked, BOOL bBackground)
{
	int ids = 0;
	BOOL bMailChecked = FALSE;
	
	///리스트 컨트롤의 메일전송여부 컬럼
	const INT nCol_SendYN = 4;
	
	this->m_nCntSended = 0;
	this->m_nCntFailed = 0;
	
	CRotatingLog Log((LPCTSTR)m_sBackupLogFilepath, DATE_FILE_NAME);
	m_sLogFilepath = Log.GetLogFilepath();

	if (!bBackground) UpdateData(TRUE);

	for (INT i = 0; i < m_nRecordCount; i++) 
	{
		if (!bBackground) 
		{
			m_ctrlMail.SetItemText(i, nCol_SendYN, _T(""));
			bMailChecked = this->m_ctrlMail.GetCheck(i);
			if (bSendMailChecked && !bMailChecked) continue;
			
			m_ctrlMail.SetItemText(i, nCol_SendYN, _T("전송시작..."));
			this->DisplayMailBody(i);
		}

		m_pRecord[i].m_SendState = MAIL_SEND_NOT;
		
		// 2005.03.22 ADD BY PDJ
		// 5163일 경우 메일 주소가 없어도 발송이 가능하다.
		// 메일 시스템이 다름.
		if ( Check_5163MailSystem() == FALSE )
		{
			if (
 				m_pRecord[i].m_sRecipientAddress.IsEmpty() && 
 				m_pRecord[i].m_sCCAddress.IsEmpty()			) 
			{
				if (!bBackground) 
				{
					m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일수신자없음"));
					this->DisplayMailBody(i);
				}	
				m_nCntFailed++;
				continue;
			}
		}
		
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));

		TmpDM.StartFrame();
		TmpDM.InitDBFieldData();

		if(_T("") == m_pRecord[i].m_sManageCode) m_pRecord[i].m_sManageCode = _T("UDF_MANAGE_CODE");
		/// Temp에 INSERT하기
		TmpDM.InitDBFieldData();
		TmpDM.AddDBFieldData( _T("REC_KEY"),			_T("NUMERIC"),	_T("ESL_MAIL_SEQ.NEXTVAL") );
		TmpDM.AddDBFieldData( _T("MAIL_KEY"),			_T("NUMERIC"),	m_pRecord[i].m_sMailInfoKey);
		TmpDM.AddDBFieldData( _T("LOAN_KEY_LIST"),		_T("STRING"),	m_pRecord[i].m_sLoanKeyList);		
		TmpDM.AddDBFieldData( _T("LIB_NAME"),			_T("STRING"),	m_pRecord[i].m_sLibName);
		TmpDM.AddDBFieldData( _T("MAIL_CLASS"),			_T("STRING"),	m_pRecord[i].m_sMailClass);
		TmpDM.AddDBFieldData( _T("MANAGE_CODE"),		_T("STRING"),	m_pRecord[i].m_sManageCode);

		if(TRUE == m_pRecord[i].m_sUserKey.IsEmpty())
		{
			TmpDM.AddDBFieldData( _T("RECV_EMAIL_ADDRESS"),	_T("STRING"),	m_pRecord[i].m_sRecipientAddress);
		}
		else
		{
			TmpDM.AddDBFieldData( _T("USER_KEY"),			_T("NUMERIC"),	m_pRecord[i].m_sUserKey);
		}		
		TmpDM.AddDBFieldData( _T("TITLE"),				_T("STRING"),	m_pRecord[i].m_sSubject);
		TmpDM.AddDBFieldData( _T("RECV_NAME"),			_T("STRING"),	m_pRecord[i].m_sRecipientName);
		TmpDM.AddDBFieldData( _T("CONTENTS"),			_T("STRING"),	m_pRecord[i].m_sBody);
		TmpDM.MakeInsertFrame(_T("MN_MAIL_TEMP_TBL"));

		ids = TmpDM.SendFrame();

		if (FALSE == bBackground) 
		{			
			if(0 > ids)
			{
				m_ctrlMail.SetItemText(i, nCol_SendYN, _T("실패"));
				m_pRecord[i].m_SendState = MAIL_SEND_FAIL;
				m_nCntFailed++;
				break;
			}
			else
			{
				m_ctrlMail.SetItemText(i, nCol_SendYN, _T("성공"));
				m_pRecord[i].m_SendState = MAIL_SEND_SUCCESS;
				m_nCntSended++;
			}
		}
	}


	if (FALSE == bBackground) 
	{
		CString strMsg;
		strMsg.Format(
			_T("메일전송이 완료되었습니다. [성공: %d]"),
			m_nCntSended);
		AfxMessageBox(strMsg);
	}

	return 0;
}
int CSendMailMgr::SendMail_SMTP(BOOL bSendMailChecked, BOOL bBackground)
{

	if(Check_5163MailSystem() == FALSE)
	{		
		BOOL bMailChecked = FALSE;
		
		///리스트 컨트롤의 메일전송여부 컬럼
		const INT nCol_SendYN = 4;
		
		this->m_nCntSended = 0;
		this->m_nCntFailed = 0;
		
		if (!bBackground) UpdateData(TRUE);
		
		if (m_sMailServer.IsEmpty() == TRUE) {
			MessageBox(_T("메일서버가 지정되지 않았습니다."), _T("경고"), MB_ICONEXCLAMATION);
			return -1;
		}
		
		if (m_sMailAddress.IsEmpty() == TRUE) {
			MessageBox(_T("메일 보내는 사람이 지정되지 않았습니다."), _T("경고"), MB_ICONEXCLAMATION);
			return -2;
		}
		
		//###################################################
		CRotatingLog Log((LPCTSTR)m_sBackupLogFilepath, DATE_FILE_NAME);
		m_sLogFilepath = Log.GetLogFilepath();
		
		for (INT i = 0; i < m_nRecordCount; i++) 
		{
			CSmtp			 mail;
			CSmtpMessage	 msg;
			CSmtpAddress	 cc;
			CSmtpMessageBody body;
			CSmtpAttachment  attach;
			
			m_pRecord[i].m_SendState = MAIL_SEND_NOT;
			
			if (!bBackground) 
			{
				m_ctrlMail.SetItemText(i, nCol_SendYN, _T(""));
				bMailChecked = this->m_ctrlMail.GetCheck(i);
				if (bSendMailChecked && !bMailChecked) continue;
				
				m_ctrlMail.SetItemText(i, nCol_SendYN, _T("전송시작..."));
				this->DisplayMailBody(i);
			}

			// 2005.03.22 ADD BY PDJ
			// 5163일 경우 메일 주소가 없어도 발송이 가능하다.
			// 메일 시스템이 다름.
			if ( Check_5163MailSystem() == FALSE )
			{
				if (m_pRecord[i].m_sRecipientAddress.IsEmpty() && 
					m_pRecord[i].m_sCCAddress.IsEmpty()			) 
				{
					if (!bBackground) 
					{
						m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일수신자없음"));
						this->DisplayMailBody(i);
					}
					m_pRecord[i].m_SendState = MAIL_SEND_NONRECIP;
					this->m_nCntFailed++;
					continue;
				}
			}
			
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
			
			while ( ( pre = (tempstr.FindOneOf(_T(","))) ) != -1) 
			{
				prestr = tempstr.Mid(0,pre);
				prestr.TrimLeft();
				prestr.TrimRight();
				attach = prestr;
				msg.Attachments.Add( attach );
				next = pre+1;
				tempstr = tempstr.Mid(next,tempstrlength);
			}
			tempstr.TrimLeft();
			tempstr.TrimRight();
			attach = tempstr;
			msg.Attachments.Add(attach);
			
			Log.StartAddLine(_T("\r\n\r\n"), FALSE, FALSE);
			Log.AddLine(_T("============================================================================="));
			Log.AddLineStop (_T("[MailMgr][CONNECT ][00][-] Connecting to SMTP Sever..."));
			
			m_pRecord[i].m_SendState = MAIL_SEND_NOTCONNECT;
			
			// 2005.12.16 ADD BY PDJ
			// 인증정보 넘기기.
			if ( m_strAccountYN == _T("Y") )
			{
				mail.m_strUser = m_strAccountID;
				mail.m_strPass = m_strAccountPASS;
				mail.m_strAccountYN = m_strAccountYN;				
			}
			else
			{
				mail.m_strUser = _T("");
				mail.m_strPass = _T("");
				mail.m_strAccountYN =_T("");
			}

			if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일서버접속중..."));		
			BOOL bConnected = mail.Connect((LPTSTR)(LPCTSTR)m_sMailServer);			
			if ( bConnected ) 
			{
				Log.StartAddLine(_T("[MailMgr][CONNECT ][01][O] Connected !!!"));
				Log.AddLineStop (_T("[MailMgr][SENDING ][02][-] Sending..."));
				
				INT nFail = mail.SendMessage(msg);
				if (!nFail) 
				{
					this->m_nCntSended++;
					if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("전송됨"));
					m_pRecord[i].m_SendState = MAIL_SEND_SUCCESS;
					
					Log.StartAddLineStop(_T("[MailMgr][SENDING ][03][O] Sended !!!"));
				}
				else
				{
					this->m_nCntFailed++;
					if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("전송실패"));
					m_pRecord[i].m_SendState = MAIL_SEND_FAIL;
					
					Log.StartAddLineStop(_T("[MailMgr][SENDING ][03][X] Send fail !!!"));
				}
				//mail.Close();
			} 
			else 
			{
				//MessageBox(_T("메일 보내기를 실패하였습니다."), _T("경고"), MB_ICONEXCLAMATION);
				//bFail = TRUE;
				//m_MailSendYn = 'N';
				Log.StartAddLineStop(_T("[MailMgr][CONNECT ][01][X] Not connected !!! ### EXIT ###"));
				
				this->m_nCntFailed++;
				if (!bBackground) m_ctrlMail.SetItemText(i, nCol_SendYN, _T("메일서버접속실패"));
				m_pRecord[i].m_SendState = MAIL_SEND_NOTCONNECT;
				break;
			}
			//###################################################
	}
	
	//if (!bFail) {
	//	MessageBox(_T("메일이 보내졌습니다."), _T("확인"), MB_ICONINFORMATION);
	//	m_MailSendYn = 'Y';
	//}
	
	if (!bBackground) 
	{
		CString sTemp;
		sTemp.Format(_T("메일전송이 완료되었습니다. [전송:%d / 실패:%d]"), this->m_nCntSended, this->m_nCntFailed);
		if ( this->m_nCntFailed > 0 )
		{
			sTemp += _T("\r\n\r\n* 메일전송이 실패한 경우 다음 사항을 확인하세요\r\n")
				_T("· 계정의 등록여부 및 패스워드, SMTP인증 사용여부\r\n")
				_T("· 스파이웨어, 방화벽, 안티바이러스 프로그램의 차단여부\r\n")
				_T("· SMTP서버의 스팸방지 여부");
		}
		MessageBox(sTemp, _T("확인"), MB_ICONINFORMATION);
	}
	
	Log.StopLogging();
	
    }
	else
	{
		SendMail_5163( bSendMailChecked, bBackground) ;
	}
	return 0;
}