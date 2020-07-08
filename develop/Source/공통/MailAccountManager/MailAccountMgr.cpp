// MailAccountMgr.cpp : implementation file
//

#include "stdafx.h"
#include "MailAccountMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailAccountMgr

CMailAccountMgr::CMailAccountMgr()
{	
	m_nMailAccountListCount = 0;
	MAIL_CONFIG_PATH = _T("..\\CFG\\MailAccount.cfg");
	MAIL_PART_CONFIG_PATH = _T("..\\CFG\\MailAccountPart.cfg");
	LoadMailAccount();
	SetMailServerInfoFromCfgFile();

}

CMailAccountMgr::~CMailAccountMgr()
{
	
}


BEGIN_MESSAGE_MAP(CMailAccountMgr, CWnd)
	//{{AFX_MSG_MAP(CMailAccountMgr)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailAccountMgr message handlers

BOOL CMailAccountMgr::GetMailAccount(CString WorkType, CString &strAccount,CString &strID, CString &Password, CString &SenderName, CString &MailAddress)
{
	for ( INT i=0 ; i<m_nMailAccountListCount ; i++ )
	{
		if ( m_pMailAccountList[i].strWorkType == WorkType )
		{
			strAccount	= m_pMailAccountList[i].strAccount;
			Password	= m_pMailAccountList[i].strPassword;
			SenderName	= m_pMailAccountList[i].strSenderName;
			strID		= strAccount.Left(strAccount.FindOneOf(_T("@")));
			if ( strAccount != _T("") && SenderName != _T("") )
			{
				MailAddress.Format(_T("\"%s\" <%s>"), SenderName, strAccount);
			}
			
			return TRUE;
		}
	}

	return FALSE;

}

VOID CMailAccountMgr::LoadMailAccount()
{	
	CStdioFile fFile;
	CString strData;	
	CString strTmp;
	CString strWork;
	CString strAccount;
	CString strPassword;
	CString strSenderName;
	CString strViewPass;

	INT		nOffset;
	INT		nIndex;
	INT		nItem;	
	fFile.Open( MAIL_PART_CONFIG_PATH, CFile::modeRead | CFile::typeBinary );	
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
	}
	
	m_nMailAccountListCount = 0;
	while( fFile.ReadString(strData) )
	{
		strData.TrimLeft(); strData.TrimRight();		
		if( strData.IsEmpty() ) continue;

		nIndex = 0;
		nItem = 0;
		strWork = _T("");
		strAccount=_T("");
		strPassword=_T("");
		strSenderName=_T("");

		while (TRUE)
		{
			nOffset = strData.FindOneOf(_T("|"));
			if ( nOffset < 0 )
			{
				strSenderName = strData;
				break;
			}
			else
			{
				strTmp = strData.Mid(nIndex, nOffset);				
			}

			switch ( nItem )
			{
				case 0:
					strWork = strTmp;					
					break;
				case 1:
					strAccount = strTmp;					
					break;
				case 2:
					strPassword = strTmp;
					break;
				default:
					break;
			}

			strData = strData.Mid(nOffset+1);
			nItem++;
		}				
		
		m_pMailAccountList[m_nMailAccountListCount].strWorkType		= strWork;
		m_pMailAccountList[m_nMailAccountListCount].strAccount		= strAccount;
		m_pMailAccountList[m_nMailAccountListCount].strPassword		= strPassword;
		m_pMailAccountList[m_nMailAccountListCount].strSenderName	= strSenderName;		
		m_nMailAccountListCount++;
	}
	fFile.Close();	
	
}

INT CMailAccountMgr::GetMailServer(CString &MailServer, CString &AccountYN)
{
	MailServer = m_strMailServer;
	AccountYN = m_strAccountYN;

	return 0;
}
/************************************************************************/
/* Config file에서 Mail Server/Account 정보 가져오기					*/
/************************************************************************/
INT CMailAccountMgr::SetMailServerInfoFromCfgFile()
{
	//메일서버 =  
	//메일계정 = 
	//발송자명 = 

	CStdioFile file;
	if (!file.Open(MAIL_CONFIG_PATH, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("메일서버설정 파일이 없습니다."));
		return -999;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
		
	m_strMailServer=_T("");
	m_strAccountYN=_T("");
	
	CString sEmailAddress;
	CString sSenderName;
	CString strAccountID;
	CString strAccountPASS;
	CString strAccountYN;
	
	CString sTemp;	
	CString strLine;
	while (file.ReadString(strLine))
	{
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
				m_strMailServer = strLine.Mid(idx+1);
				m_strMailServer.TrimLeft();
				m_strMailServer.TrimRight();
			} 			
			else if (sTemp == _T("보내는메일인증사용")) 
			{
				m_strAccountYN = strLine.Mid(idx+1);
				m_strAccountYN.TrimLeft();
				m_strAccountYN.TrimRight();				
			}			
		}
	}
	
	if ( m_strMailServer.IsEmpty() )
	{
		sTemp.Format(_T("메일서버설정 파일에서 메일서버 정보를 읽어올 수 없습니다.\n")
			         _T(" - 메일서버 : %s\n"), 					 
					 m_strMailServer );
		AfxMessageBox(sTemp);
		return -2;
	}
	//this->m_sMailAddress.Format(_T("\"%s\" <%s>"), sSenderName, sEmailAddress);	
	
	return 0;
}
