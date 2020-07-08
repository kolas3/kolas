// PajuSMSManager.cpp : implementation file
//

#include "stdafx.h"
#include "PajuSMSManager.h"
#include "SMSMessageConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPajuSMSManager

CPajuSMSManager::CPajuSMSManager()
{
	RegistSMSDLL();
}

CPajuSMSManager::~CPajuSMSManager()
{
}


BEGIN_MESSAGE_MAP(CPajuSMSManager, CWnd)
	//{{AFX_MSG_MAP(CPajuSMSManager)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPajuSMSManager message handlers

BOOL CPajuSMSManager::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

VOID CPajuSMSManager::RegistSMSDLL()
{ 
	m_NForum.Init();	
}


INT CPajuSMSManager::OnCreateServer()
{
	CESL_DataMgr* pDM;
	CString		strID;
	CString		strPasswd;	
	CString		strWhere;
	CString		strIP;
	CString		strPORT;

	pDM = m_pParent->FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));

	if ( pDM->GetRowCount() <= 0 )
	{
		strWhere=_T("SMS_TYPE=2");
		pDM->RefreshDataManager(strWhere);
	}

	strID		= pDM->GetCellData(_T("바인드아이디"), 0);
	strPasswd	= pDM->GetCellData(_T("바인드패스워드"), 0);	
	strIP		= pDM->GetCellData(_T("이용자접속IP"), 0);
	strPORT		= pDM->GetCellData(_T("포트"), 0);	
	
	/*
	// Bind용 ID, PASSWD
	CStdioFile	fFile;
	CString		sFilePath = _T("..\\cfg\\nForumSMS.txt");
	CString		strLine;
	CString		sTemp;
	CString		strID;
	CString		strPasswd;	
	INT			ids;
	INT			idx;

	ids = fFile.Open( sFilePath , CFile::modeRead );
	if( ids == 0 )
	{
		AfxMessageBox(_T("아이하트 SMS정보를 로드하는데 실패하였습니다."));
		ReleaseServer();
		return 1;
	}	
	
	while( fFile.ReadString( strLine ) )
	{
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;		
		if (strLine.Left(1) == _T("[") || strLine.IsEmpty()) continue;		

		idx = strLine.Find(_T("="));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if (sTemp == _T("ID")) 
			{	
				strID = strLine.Mid(idx+1);				
			}
			else if (sTemp == _T("PASSWD")) 
			{			
				strPasswd = strLine.Mid(idx+1);
			}
		}
	}
	fFile.Close();
	*/	

	TCHAR	szID[16];
	TCHAR	szPASS[16];
	TCHAR	szIP[32];
	TCHAR	szPORT[16];

	_stprintf(szID, _T("%s"), strID );
	_stprintf(szPASS, _T("%s"), strPasswd );
	_stprintf(szIP, _T("%s"), strIP );
	_stprintf(szPORT, _T("%s"), strPORT );

	// Open
	m_NForum.SMSOpen(szIP, _ttoi(szPORT));
	m_NForum.SMSBind(szID, szPASS);	

	return 0;

}

INT CPajuSMSManager::ReleaseServer()
{
	m_NForum.SMSClose();
	m_NForum.UnInit();

	return 0;
}


INT CPajuSMSManager::SendSMSMsg(
								CStringList &receiverName,
								CStringList &receiverNum,
								CString title,
								CStringList &msg,
								CString callbackNum,
								VOID *pParent )
{
	INT dCnt = 0;
	INT SMSCnt = msg.GetCount();

	m_pParent = (CESL_Mgr*) pParent;
		
	CString strReceivePhone;
	CString receiverPNum;
	CString strMSG;
	CString strReturn;
	CString strSendMsg;
	INT		ids;
	INT length = 0;
	
	INT cnt = receiverName.GetCount();
	
	// OPEN/BIND
	ids = OnCreateServer();
	if ( ids ) return 0;

	for( INT idx = 0; idx < cnt; idx++ )
	{					
		/*
		argument 
		1-- 받는 사람 숫자만 가능
		2 -- 회신 번호 숫자만 가능
		3 -- caller 보내는 사람 
		4 -- 예약일자(YYYYMMDD)
		5 -- 예약 시간(HHMM)
		6 -- serial number(client)
		7 -- 메세지		
		*/

		strReceivePhone = receiverNum.GetAt(receiverNum.FindIndex( idx ));
		strMSG = msg.GetAt(msg.FindIndex( idx ));

		//동보전송시 open과 bind후 sendtext를 loop돌리시면 됩니다.		
		dCnt = 0;
		while ( !strMSG.IsEmpty() )
		{			
			strSendMsg = GetDivideMsg(strMSG);			
			ids = m_NForum.SMSSendText(strReceivePhone.GetBuffer(0),
				callbackNum.GetBuffer(0),
				_T(""),
				_T(""),
				_T(""),
				_T("0"),
				strSendMsg.GetBuffer(0) );					
			
			dCnt++;	
		
			if( ids < 0 )
			{
				SendErrorMsg( cnt, idx, 0, -5 );
				continue;			
			}
		}

		SendErrorMsg( cnt, idx, dCnt, 0 );
	}
	
	SendErrorMsg( 0, 0, 0, 1 );

	ReleaseServer();
	
	return 0;
}

VOID CPajuSMSManager::SendErrorMsg( INT cnt, INT idx, INT dCnt, INT error )
{
	if( !m_pParent )
		return;
	
	CString statusMsg;
	statusMsg.Format( _T("%d/%d/%d/%d"), cnt, idx, dCnt, error );

	::SendMessage( m_pParent->m_hWnd, WM_USER + 1005, ( WPARAM )&statusMsg, true );
}

CString CPajuSMSManager::GetDivideMsg( CString &msg )
{
	CSMSMessageConvert ConvertDlg;
	CString sendMsg;
	INT length;	

	CHAR		*szMsg;		
	CHAR		szCutMsg[81];
	INT			nLen;

	ConvertDlg.UnicodeToMultiByte(msg.GetBuffer(0), &szMsg, &nLen );	

	if( IsCanCut( szMsg, 80 ) )
		length = 80;
	else
		length = 80 - 1;

	strncpy(szCutMsg , szMsg, length);
	szCutMsg[length] = '\0';


	TCHAR *szUnicodeMessage;
	INT		nUnicodeLen;

	ConvertDlg.MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );

	sendMsg.Format(_T("%s"), szUnicodeMessage );
	
	msg = msg.Mid( sendMsg.GetLength() );

	return sendMsg;
}

// 문자가 한글일 경우 어디서 잘라야 할지 위치 구하기
bool CPajuSMSManager::IsCanCut( CString str, INT pos )
{
	INT i, nLastPos = -1;
	//글자수 초과면 FALSE
	if( str.GetLength() <= pos )
		return TRUE;
	
	TCHAR ch = str.GetAt( pos );
	
	//마지막 글자 || 한글이 아니면 TRUE
	if( pos + 1 == str.GetLength() || ( ch & ( 0x80 ) ) != 128 )
		return true;
	
	//첫글자가 1byte문자면 TRUE 아니면 FALSE
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return true;
		else
			return false;

	for( i = 0; i < pos; i++ )
	{
		ch = str.GetAt( i );
		
		//자르는 부분에서 가장 가까운(앞쪽으로)영문의 위치
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return true;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return true;
	
	//짝수개 차이나면 TRUE
	bool result = !( ( pos - nLastPos ) % 2 );
	return result;
}