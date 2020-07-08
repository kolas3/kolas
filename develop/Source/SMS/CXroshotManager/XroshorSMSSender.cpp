// XroshorSMSSender.cpp: implementation of the CXroshorSMSSender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XroshorSMSSender.h"
#include "SMSSendDlg.h"
#include "SMSMessageConvert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXroshorSMSSender::CXroshorSMSSender( CESL_Mgr *pParent )
{
	m_initSet = false;
	m_bind = false;
	b_submit_ack = false;
	b_report_ack = false;
	user_type = USER_TYPE_PERSONAL;
	user_access_ip = _T("127.0.0.1");
	m_pParent = pParent;
}

CXroshorSMSSender::~CXroshorSMSSender()
{
	UnbindXShot();	
}

// 최초 CXroshorSMSSender 초기화 한다. 이걸 하지 않으면 메세지 보낼 수 없음!
INT CXroshorSMSSender::InitSetXShot(
								CString bind_id,
								CString bind_pwd,
								CString bind_domain,
								CString user_phone_id,
								CString call_back,
								CString confFilePath )
{
	this->bind_id = bind_id;
	this->bind_pwd = bind_pwd;
	this->bind_domain = bind_domain;
	this->user_phone_id = user_phone_id;
	this->call_back = call_back;

	//++2009.01.19 ADD BY CJY {{++
	//DEL 크로샷API는 ANSI기반 함수이다.
	CString strConvert = confFilePath;
	m_confFilePath = confFilePath;
	
	int len; 
	len = WideCharToMultiByte(CP_ACP, 0, strConvert, -1, NULL, 0, NULL,NULL);
	char* ctmp = new char[len]; 
	// 실제 변환
	WideCharToMultiByte(CP_ACP, 0, strConvert, -1, ctmp, len, NULL, NULL);			
	if(FALSE == x_msggate_conf_init(ctmp))
	{
		return -1;
	}
	delete []ctmp;
//DEL	이전소스
//DEL 	m_confFilePath = confFilePath;
//DEL 	if( !x_msggate_conf_init( m_confFilePath.GetBuffer( 0 ) ) )
//DEL 		return -1;	// x_msggate.conf 를 읽지 못함
	//--2009.01.19 ADD BY CJY --}}

	m_initSet = true;

	return 0;
}

// Bind 하기.
BOOL CXroshorSMSSender::BindXShot()
{
	UnbindXShot();
	
	//++2009.01.19 ADD BY CJY {{++
	//DEL 크로샷API는 ANSI기반 함수이다.
	const int CONVERT_STRING_COUNT = 3;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_pwd, bind_domain
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}
	INT ids = fn_x_bind(
		user_type,
		ctmp[0],		// bind_id
		ctmp[1],		// bind_pwd
		ctmp[2],		// bind_domain
		b_submit_ack,
		lpfn_submit,
		b_report_ack,
		lpfn_report,
		&x_ret_code );

	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}	
//DEL	이전소스
//DEL 	INT ids = fn_x_bind(
//DEL 						user_type,
//DEL 						bind_id.GetBuffer( 0 ),
//DEL 						bind_pwd.GetBuffer( 0 ),
//DEL 						bind_domain.GetBuffer( 0 ),
//DEL 						b_submit_ack,
//DEL 						lpfn_submit,
//DEL 						b_report_ack,
//DEL 						lpfn_report,
//DEL 						&x_ret_code );
	//--2009.01.19 ADD BY CJY --}}

	if( X_RET_FUNC_SUCCESS == ids )
		return m_bind = true;
	return false;
}

// Unbind 하기.
BOOL CXroshorSMSSender::UnbindXShot()
{
	if( !m_bind )
		return false;
	
	//++2009.01.19 ADD BY CJY {{++
	//DEL 크로샷API는 ANSI기반 함수이다.
	const int CONVERT_STRING_COUNT = 2;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_pwd
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}
	int ids = fn_x_unbind(
		ctmp[0],		// bind_id
		ctmp[1]			// bind_pwd
		);
	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}	
//DEL	이전소스
//DEL 	INT ids = fn_x_unbind(
//DEL 		bind_id.GetBuffer( 0 ),
//DEL 		bind_pwd.GetBuffer( 0 ) );	
	//--2009.01.19 ADD BY CJY --}}

	if( X_RET_FUNC_SUCCESS == ids )
	{
		m_bind = false;
		return true;
	}
	return false;
}

// Money Status 얻기.
BOOL CXroshorSMSSender::GetMoneyStatus()
{
	INT pro_status;
	INT bill_main_code;
	INT	bill_sub_code;
	INT	avail_money;
	INT	reserved_money;
	INT	sms_price;
	INT	fax_price;
	INT	voice_cel_unit;
	INT	voice_cel_price;
	INT	voice_tel_unit;
	INT	voice_tel_price;

	//++2009.01.19 ADD BY CJY {{++
	//DEL 크로샷API는 ANSI기반 함수이다.
	const int CONVERT_STRING_COUNT = 4;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_domain, user_phone_id, user_access_ip
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL, NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}
	INT ids = fn_x_money_status(
								ctmp[0],		// bind_id
								ctmp[1],		// bind_domain
								ctmp[2],		// user_phone_id
								ctmp[3],		// user_access_ip
								&bill_main_code,
								&bill_sub_code,
								&pro_status,
								&avail_money,
								&reserved_money,
								&sms_price,
								&fax_price,
								&voice_cel_unit,
								&voice_cel_price,
								&voice_tel_unit,
								&voice_tel_price,
								&x_ret_code );

	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}
//DEL	이전소스	
//DEL 	INT ids = fn_x_money_status(
//DEL 								bind_id.GetBuffer( 0 ),
//DEL 								bind_domain.GetBuffer( 0 ),
//DEL 								user_phone_id.GetBuffer( 0 ),
//DEL 								user_access_ip.GetBuffer( 0 ),
//DEL 								&bill_main_code,
//DEL 								&bill_sub_code,
//DEL 								&pro_status,
//DEL 								&avail_money,
//DEL 								&reserved_money,
//DEL 								&sms_price,
//DEL 								&fax_price,
//DEL 								&voice_cel_unit,
//DEL 								&voice_cel_price,
//DEL 								&voice_tel_unit,
//DEL 								&voice_tel_price,
//DEL 								&x_ret_code );
	//--2009.01.19 ADD BY CJY --}}

	if( X_RET_FUNC_SUCCESS == ids )
		return true;
	return false;
}

// 현재날짜시간 얻기.
CString CXroshorSMSSender::GetNowTime()
{
	CTime time = CTime::GetCurrentTime();
	CString now;
	now.Format( _T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
	return now;
}

// 이미 bind 되어 있는 상태에서 실제 문자 날리는 함수
INT CXroshorSMSSender::SendSMSMsg(
								CString receiverName,
								CString receiverNum,
								CString title,
								CString msg,
								CString callbackNum )
{
	if( callbackNum.IsEmpty() )
		callbackNum = call_back;
	
	CString receiver;
	receiver.Format( _T("%s:%s;"), receiverName, receiverNum );

	//++2009.01.19 ADD BY CJY {{++
	//DEL 크로샷API는 ANSI기반 함수이다.
	const int CONVERT_STRING_COUNT = 9;
	CString strConvert[CONVERT_STRING_COUNT] = 
	{
		bind_id, bind_domain, user_phone_id, user_access_ip, title, GetNowTime(), callbackNum, receiver, msg
	};
	char* ctmp[CONVERT_STRING_COUNT] = 
	{
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	};
	
	for(int nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, NULL, 0, NULL,NULL);
		ctmp[nCnt] = new char[len];
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strConvert[nCnt], -1, ctmp[nCnt] , len, NULL, NULL);					
	}

	INT ids = fn_x_submit_2(
							X_METHOD_SMS,
							X_SUBTYPE_TTS,
							1,
							ctmp[0],		// bind_id
							ctmp[1],		// bind_domain
							ctmp[2],		// user_phone_id
							ctmp[3],		// user_access_ip
							WM_USER + 1010,
							ctmp[4],		// title
							ctmp[5],		// GetNowTime()
							ctmp[6],		// callbackNum
							0,
							1,
							ctmp[7],		// receiver
							ctmp[8],		// msg
							"",
							0,
							"");

	for(nCnt = 0; nCnt < CONVERT_STRING_COUNT; nCnt++)
	{
		delete []ctmp[nCnt];
	}


//DEL 	INT ids = fn_x_submit_2(
//DEL 							X_METHOD_SMS,
//DEL 							X_SUBTYPE_TTS,
//DEL 							1,
//DEL 							bind_id.GetBuffer( 0 ),
//DEL 							bind_domain.GetBuffer( 0 ),
//DEL 							user_phone_id.GetBuffer( 0 ),
//DEL 							user_access_ip.GetBuffer( 0 ),
//DEL 							WM_USER + 1010,
//DEL 							title.GetBuffer( 0 ),
//DEL 							GetNowTime().GetBuffer( 0 ),
//DEL 							callbackNum.GetBuffer( 0 ),
//DEL 							0,
//DEL 							1,
//DEL 							receiver.GetBuffer( 0 ),
//DEL 							msg.GetBuffer( 0 ),
//DEL 							_T(""),
//DEL 							0,
//DEL 							_T("") );
	
	if( 1 == ids )
		return 0;
	else
		return ids;
}

// 문자 한개 최대 길이 만큼 전송 문자 자르기
// msg : 최초 문자 -> 자른후 나머지 문자
CString CXroshorSMSSender::GetDivideMsg( CString &msg )
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
bool CXroshorSMSSender::IsCanCut( CString str, INT pos )
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

// 수신자 전화번호가 옳바른 형태인지 검사
INT CXroshorSMSSender::CheckPhoneNum( CString phoneNum )
{
	phoneNum.Replace( _T("-"), _T("") );
	
	CString tmp;
	TCHAR num;
	for( INT i = 0; i < phoneNum.GetLength(); i++ )
	{
		num = phoneNum.GetAt( i );

		if( num > 57 || num < 48 )
			continue;

		tmp += phoneNum.Mid( i, 1 );
	}
	phoneNum = tmp;

	INT length = phoneNum.GetLength();
	if( 10 != length && 11 != length )
		return -1;

	CStringList hpNumList;
	if( CLocCommonAPI::GetHPCorpNumList( hpNumList ) )
		CLocCommonAPI::MakeDefaultHPNumList( hpNumList );
	
	bool flag = false;
	for( i = 0; i < hpNumList.GetCount(); i++ )
	{
		CString pNum = hpNumList.GetAt( hpNumList.FindIndex( i ) );
		if( phoneNum.Left( pNum.GetLength() ) == pNum )
		{
			flag = true;
			break;
		}
	}
	if( !flag )
		return -2;
	
	return 0;
}

// SMS 를 전송할 준비가 되어있는지 검사하고 bind 한후 money status 가져오기
INT CXroshorSMSSender::ReadySMS()
{
	if( m_bind )
		return 0;
	
	if( !m_initSet )
		return -1;		// 초기화가 되어있지 않은 상태

 	if( !BindXShot() )
 		return -2;		// Binding에 실패

	if( !GetMoneyStatus() )
		return -3;		// money Status 를 가져오지 못함.

	return 0;
}

// 문자 날리기 - 1회 문자 최대 길이를 넘을 경우 잘라서 계속 보낸다.
INT CXroshorSMSSender::SendMessage( CString receiverName, CString receiverNum, CString title, CString msg, CString callbackNum, INT &divideCnt )
{
	divideCnt = 1;
	while( true )
	{
		if( SendSMSMsg( receiverName, receiverNum, title, GetDivideMsg( msg ), callbackNum ) )
			return -5;

		if( msg.IsEmpty() )
			break;
	
		divideCnt++;
	}

	return 0;

}

// 문자 한개만 날리기 - 문자 길이가 80bytes를 넘을때 짤라 보낸다. 
INT CXroshorSMSSender::SendSMSMsg(
								CString receiverName,
								CString receiverNum,
								CString title,
								CString msg,
								INT &divideCnt,
								CString callbackNum )
{
	INT ids;
	ids = ReadySMS();
	if( ids )
		return ids;
	
	if( CheckPhoneNum( receiverNum ) )
		return -4;
	
	if( SendMessage( receiverName, receiverNum, title, msg, callbackNum, divideCnt ) )
		return -5;

	if( !UnbindXShot() )
		return -6;		// Unbinding에 실패


	return 0;
}

// SMS 전송 리스트를 쓰레드로 돌리서 문자 날린다.
INT CXroshorSMSSender::SendSMSMsgThread( LPVOID pParam )
{
	struct SMS_LIST *pSMSList = ( SMS_LIST * )pParam;
	return SendSMSMsg(
					pSMSList->receiverName,
					pSMSList->receiverNum,
					pSMSList->title,
					pSMSList->msg );
}

// 여러개의 문자를 루프 돌며 날리기
INT CXroshorSMSSender::SendSMSMsg(
								CStringList &receiverName,
								CStringList &receiverNum,
								CString title,
								CStringList &msg,
								CString callbackNum,
								VOID *pParent )
{
	INT dCnt = 0;
	INT SMSCnt = msg.GetCount();
	
	CString receiverPNum;
	INT length = 0, ids;
	
	INT cnt = receiverName.GetCount();
	for( INT idx = 0; idx < cnt; idx++ )
	{
		// SMS 전송 중지인지 확인
		if( GetStopStatus( pParent ) )
		{
			SendErrorMsg( 0, 0, 0, 2 );
			UnbindXShot();
			return 0;
		}
		
		// 수신자 핸드폰 번호가 옳바른지 검사
		receiverPNum = receiverNum.GetAt( receiverNum.FindIndex( idx ) );
		if( CheckPhoneNum( receiverPNum ) )
		{
			SendErrorMsg( cnt, idx, 0, -4 );
			continue;
		}
		
		// 최초 bind 를 한다. 
		if( !idx )
		{
			ids = ReadySMS();
			if( ids )
			{
				SendErrorMsg( cnt, idx, 0, ids );
				continue;
			}
		}

		// 1회 bind 후 보낼 수 있는 문자열 길이를 초과하면 unbind 후 다시 bind 한다 
		length += msg.GetAt( msg.FindIndex( idx ) ).GetLength();
		if( length > SMS_TOTAL_LENGTH )
		{
			length = 0;
			UnbindXShot();

			ids = ReadySMS();
			if( ids )
			{
				SendErrorMsg( cnt, idx, 0, ids );
				continue;
			}		
		}
		
		// SMS 전송
		ids = this->SendMessage( 
						receiverName.GetAt( receiverName.FindIndex( idx ) ),
						receiverPNum,
						title,
						msg.GetAt( msg.FindIndex( idx ) ),
						callbackNum,
						dCnt );
		if( ids )
		{
			SendErrorMsg( cnt, idx, 0, -5 );
			continue;
		}
		
		SendErrorMsg( cnt, idx, dCnt, 0 );
	}
	
	SendErrorMsg( 0, 0, 0, 1 );
	
	ids = UnbindXShot();
	if( ids )
		return -6;
	
	return 0;
}

// 부모에게 전송결과를 보낸다.
VOID CXroshorSMSSender::SendErrorMsg( INT cnt, INT idx, INT dCnt, INT error )
{
	if( !m_pParent )
		return;
	
	CString statusMsg;
	statusMsg.Format( _T("%d/%d/%d/%d"), cnt, idx, dCnt, error );

	::SendMessage( m_pParent->m_hWnd, WM_USER + 1005, ( WPARAM )&statusMsg, true );
}

// 부모로 부터 문자전송 중지 이벤트가 왔는지 검사
bool CXroshorSMSSender::GetStopStatus( VOID *pParent )
{
	if( !pParent )
		return false;

	CSMSSendDlg *parentDlg;
	parentDlg = NULL;
	parentDlg = ( CSMSSendDlg * )pParent;
	return parentDlg->m_bSMSStop;
}
