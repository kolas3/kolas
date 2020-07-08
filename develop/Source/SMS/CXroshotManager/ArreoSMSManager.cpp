// ArreoSMSManager.cpp : implementation file
//

#include "stdafx.h"
#include "ArreoSMSManager.h"
#include "SMSMessageConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArreoSMSManager

CArreoSMSManager::CArreoSMSManager()
{
}

CArreoSMSManager::~CArreoSMSManager()
{
}


BEGIN_MESSAGE_MAP(CArreoSMSManager, CWnd)
	//{{AFX_MSG_MAP(CArreoSMSManager)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArreoSMSManager message handlers

// 통신사   | URL | SEP | SMS | TOT |
// SKT      | -   | 1   | -   | 80  |
// KTF      | -   | 1   | 80  | 80  |
// KTF(018) | 50  | 1   | 20  | 71  |
// LGT      | 80  | 1   | 80  | 98  |
// 가변적이지만 010으로 통합후 번호로 통신사를 알 수 없음.
// 80 Byte 일괄 적용함.		
INT CArreoSMSManager::SendSMSMsg(
								CStringList &receiverName,
								CStringList &receiverNum,
								CString title,
								CStringList &msg,
								CString callbackNum,
								VOID *pParent )
{	
	m_pParent = (CESL_Mgr*) pParent;
	CESL_DataMgr *pDM = m_pParent->FindDM( _T("DM_SMSSendDlg") );		
	
	CString receiverPNum;
	CString strQuery;
	CString strMsgID;
	CString strCallBackNum, strURL, strMSGGB;
	CString strReceivePhone;
	CString strMSG, strURLMSG;
	CString strTmp;
	CString strSendMsg;

	GetSMSEnv(strCallBackNum, strURL, strMSGGB);

	INT length = 0, ids;
	INT idx;
	INT SMSCnt = msg.GetCount();	
	INT dCnt;	
	
	ids = pDM->StartFrame();
	if( ids < 0 ) return ids;

	if ( strMSGGB.IsEmpty() ) strMSGGB=_T("A");
	
	INT cnt = receiverName.GetCount();
	for( idx = 0; idx < cnt; idx++ )
	{
		dCnt = 0;
		// url message구분방법?
		strReceivePhone = receiverNum.GetAt(receiverNum.FindIndex( idx ));
		strMSG = msg.GetAt(msg.FindIndex( idx ));		
		
		if ( strMSGGB == _T("B") || strMSGGB == _T("D") )
		{
			strTmp = strURL + _T(" ") + strMSG;
			strMSG = strTmp;
		}			
		
		// URL과 MSG 구분자 '공백' 포함
		dCnt = 0;
		while ( !strMSG.IsEmpty() )
		{
			strSendMsg = GetDivideMsg(strMSG);
			// 2009.02.20 DEL BY PWR : 임시로 ARREO를 I-HEART로 쓴다
			//MakeMsgID(strMsgID);
			SendMsgInDB(pDM, strMsgID, strMSGGB, strMsgID.Left(14), strCallBackNum, strReceivePhone, strCallBackNum, strSendMsg);
			
			if ( strMSGGB == _T("B") ) strMSGGB = _T("A");
			if ( strMSGGB == _T("D") ) strMSGGB = _T("C");	
			dCnt++;
		}		
		SendErrorMsg( cnt, idx, dCnt, 0 );
	}

	ids = pDM->SendFrame();
	if( ids < 0 ) 
	{
		// 2007.08.21 add by pdw
		// 에러 로그 보강
		for( idx = 0; idx < cnt; idx++ )
		{
			SendErrorMsg( cnt, idx, dCnt, -5);
		}
		return ids;
	}
	ids = pDM->EndFrame();
	if( ids < 0 )
	{
		// 2007.08.21 add by pdw
		// 에러 로그 보강
		for( idx = 0; idx < cnt; idx++ )
		{
			SendErrorMsg( cnt, idx, dCnt, -5);
		}
		return ids;
	}
	
	SendErrorMsg( 0, 0, 0, 1 );	
	
	return 0;
}

INT CArreoSMSManager::MakeMsgID(CString &f_pMsgID)
{
	// 년도(4)+월(2)+일(2)+시간(2)+분(2)+일련번호(8)
	// 마지막 일련번호 관리 방법?	
	CString strDateTime;
	CString strSqeID;
	CString strSeqWhere;
	CString strTmp;
	CTime tCurrent;
	
	CESL_DataMgr *pDM = m_pParent->FindDM( _T("DM_SMSSendDlg") );

	pDM->GetDBTime();	
	tCurrent = pDM->GetDBTime();
	//tCurrent = CTime::GetCurrentTime();	
	strDateTime.Format(_T("%04d%02d%02d%02d%02d"),tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), tCurrent.GetHour(), tCurrent.GetMinute() );

	strSeqWhere.Format(_T("SELECT ARREO_SEQ.NEXTVAL FROM DUAL"));
	pDM->GetOneValueQuery(strSeqWhere, strTmp );

	strSqeID.Format(_T("%08s"), strTmp);	
	f_pMsgID = strDateTime+strSqeID;

	return 0;
}

INT CArreoSMSManager::GetSMSEnv(CString &f_strPhone, CString &f_strURL, CString &f_strMSGType)
{
	CESL_DataMgr *pDM = m_pParent->FindDM( _T("DM_SMSSendDlg") );

	CString strTmp;
	CString strPhone, strURL, strMSGType;
	CString strWhere;

/*
	strWhere.Format(_T("SELECT MIN(CALLBACK_PHONE) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='1'"));
	pDM->GetOneValueQuery(strWhere, strTmp);
	strTmp.Replace(_T("-"), _T(""));	
	strPhone = strTmp;
*/
	// 2007.08.13 add by pdw
	// 관리구분별 회신정보 지원
	// 1. 관리구분별 도서관관리 기능을 사용하는지 확인


	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString query;
	query.Format(_T("SELECT SMS_CALLBACK_PHONE ")
				 _T("FROM MN_LIBINFO2_TBL ")
		         _T("WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
//	CString query, value;
//	query.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
//		          _T(" WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='관리구분별도서관설정'"));
//	pDM->GetOneValueQuery( query, value );
//	value.TrimLeft();value.TrimRight();
//	value.MakeUpper();
//
//	// 2. 쿼리 생성
//	if( _T("Y") == value )
//	{
//		/*
//		SELECT SMS_CALLBACK_PHONE,SMS_LIB_NAME 
//		FROM MN_LIBINFO2_TBL 
//		WHERE MANAGE_CODE=(SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY=%%);
//		*/
//		query.Format(_T("SELECT SMS_CALLBACK_PHONE ")
//					  _T(" FROM MN_LIBINFO2_TBL ")
//		             _T(" WHERE MANAGE_CODE=(SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY=%s)"),m_pParent->m_pInfo->USER_PK);		
//	}
//	else
//	{
//		/*
//		SELECT SMS_CALLBACK_PHONE,SMS_LIB_NAME 
//		FROM MN_LIBINFO2_TBL 
//		WHERE MANAGE_CODE IS NULL
//		*/
//		query.Format(_T("SELECT MAX(SMS_CALLBACK_PHONE) ")
//					  _T(" FROM MN_LIBINFO2_TBL ")
//					 _T(" WHERE MANAGE_CODE IS NULL OR MAnAGE_CODE='MA'") );
//	}
	//--2008.10.15 UPDATE BY PWR --}}

	pDM->GetOneValueQuery(query, strTmp);
	strTmp.Replace(_T("-"), _T(""));	
	strPhone = strTmp;

	strWhere.Format(_T("SELECT MIN(URL) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='1' AND MANAGE_CODE=UDF_MANAGE_CODE"));
	pDM->GetOneValueQuery(strWhere, strTmp);
	strURL = strTmp;
	
	strWhere.Format(_T("SELECT MIN(MSG_GB) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='1' AND MANAGE_CODE=UDF_MANAGE_CODE"));
	pDM->GetOneValueQuery(strWhere, strTmp);
	strMSGType = strTmp;	
	
	f_strPhone = strPhone;
	f_strURL = strURL;
	f_strMSGType = strMSGType;

	return 0;
}


// 부모에게 전송결과를 보낸다.
VOID CArreoSMSManager::SendErrorMsg( INT cnt, INT idx, INT dCnt, INT error )
{
	if( !m_pParent )
		return;
	
	CString statusMsg;
	statusMsg.Format( _T("%d/%d/%d/%d"), cnt, idx, dCnt, error );

	::SendMessage( m_pParent->m_hWnd, WM_USER + 1005, ( WPARAM )&statusMsg, true );
}

INT CArreoSMSManager::SendMsgInDB(CESL_DataMgr *pDM, CString strMsgID, CString strMSGGB, CString strMSGDTTM, CString strSendPhoneNum, CString strReceivePhone, CString strCallBackNum, CString strMsg)
{
	// 2009.02.20 UPDATE BY PWR : 임시로 ARREO를 I-HEART로 쓴다
	pDM->InitDBFieldData();	
	pDM->AddDBFieldData( _T("MSG_SEQ")	, _T("NUMERIC"), _T("MSG_DATA_SEQ.NEXTVAL") );
	pDM->AddDBFieldData( _T("CUR_STATE"), _T("NUMERIC"), _T("0") );
	pDM->AddDBFieldData( _T("CALL_TO")	, _T("STRING"), strReceivePhone );
	pDM->AddDBFieldData( _T("CALL_FROM"), _T("STRING"), strCallBackNum );
	pDM->AddDBFieldData( _T("SMS_TXT")	, _T("STRING"), strMsg );
	pDM->AddDBFieldData( _T("MSG_TYPE")	, _T("NUMERIC"), _T("4") );
	pDM->MakeInsertFrame(_T("MSG_DATA"));
	//pDM->InitDBFieldData();	
	//pDM->AddDBFieldData( _T("CMP_MSG_ID")	, _T("STRING") , strMsgID );
	//pDM->AddDBFieldData( _T("CMP_USR_ID")	, _T("STRING") , _T("00000") );
	//pDM->AddDBFieldData( _T("ODR_FG")		, _T("STRING") , _T("1") );
	//pDM->AddDBFieldData( _T("SMS_GB")		, _T("STRING") , _T("1") );
	//pDM->AddDBFieldData( _T("USED_CD")		, _T("STRING") , _T("00") );		
	//pDM->AddDBFieldData( _T("MSG_GB")		, _T("STRING") , strMSGGB );  //A :SMS, B:URL, C:해외SMS, D:해외URL
	//pDM->AddDBFieldData( _T("SND_DTTM")		, _T("STRING") , strMsgID.Left(14) );
	//pDM->AddDBFieldData( _T("SND_PHN_ID")	, _T("STRING") , strCallBackNum );		
	//pDM->AddDBFieldData( _T("RCV_PHN_ID")	, _T("STRING") , strReceivePhone );
	//pDM->AddDBFieldData( _T("CALLBACK")		, _T("STRING") , strCallBackNum );//callbackNum );
	//pDM->AddDBFieldData( _T("SND_MSG")		, _T("STRING") , strMsg );	
	//pDM->AddDBFieldData( _T("EXPIRE_VAL")	, _T("STRING") , _T("0") );
	//pDM->AddDBFieldData( _T("SMS_ST")		, _T("STRING") , _T("0") );
	//pDM->AddDBFieldData( _T("RSLT_VAL")		, _T("STRING") , _T("99") );
	//pDM->MakeInsertFrame( _T("ARREO_SMS"));	

	return 0;
}


CString CArreoSMSManager::GetDivideMsg( CString &msg )
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
bool CArreoSMSManager::IsCanCut( CString str, INT pos )
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

