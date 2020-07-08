// SamsungNetworks.cpp: implementation of the CSamsungNetworks class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SamsungNetworks.h"
#include "SMSMessageConvert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSamsungNetworks::CSamsungNetworks()
{

}

CSamsungNetworks::~CSamsungNetworks()
{

}

bool CSamsungNetworks::IsCanCut( CString str, INT pos )
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

CString CSamsungNetworks::GetDivideMsg( CString &msg )
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

INT CSamsungNetworks::GetSMSEnv(CString &f_strPhone, CString &f_strURL, CString &f_strMSGType)
{
	CESL_DataMgr *pDM = m_pParent->FindDM( _T("DM_SMSSendDlg") );

	CString strTmp;
	CString strPhone, strURL, strMSGType;
	CString strWhere;


/*	strWhere.Format(_T("SELECT MIN(CALLBACK_PHONE) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='5'"));
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

	strWhere.Format(_T("SELECT MIN(URL) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='5' AND MANAGE_CODE=UDF_MANAGE_CODE"));
	pDM->GetOneValueQuery(strWhere, strTmp);
	strURL = strTmp;
	
	strWhere.Format(_T("SELECT MIN(MSG_GB) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='5' AND MANAGE_CODE=UDF_MANAGE_CODE"));
	pDM->GetOneValueQuery(strWhere, strTmp);
	strMSGType = strTmp;	
	
	f_strPhone = strPhone;
	f_strURL = strURL;
	f_strMSGType = strMSGType;

	return 0;
}

INT CSamsungNetworks::MakeMsgID(CString &f_pMsgID)
{	
	CString strSqeID;
	CString strSeqWhere;	
		
	CESL_DataMgr *pDM = m_pParent->FindDM( _T("DM_SMSSendDlg") );	

	strSeqWhere.Format(_T("SELECT MT_TRAN_CODE_SEQ.NEXTVAL FROM DUAL"));
	pDM->GetOneValueQuery(strSeqWhere, strSqeID );
	
	f_pMsgID = strSqeID;

	return 0;
}

VOID CSamsungNetworks::SendErrorMsg(INT cnt, INT idx, INT dCnt, INT error)
{
	if( !m_pParent )
		return;
	
	CString statusMsg;
	statusMsg.Format( _T("%d/%d/%d/%d"), cnt, idx, dCnt, error );

	::SendMessage( m_pParent->m_hWnd, WM_USER + 1005, ( WPARAM )&statusMsg, true );
}
INT CSamsungNetworks::SendMsgInDB(CESL_DataMgr *pDM, CString strMsgID, CString strURL, CString strReceivePhone, CString strCallBackNum, CString strMsg)
{
	// ===============================
	// IMO 형태 하픈 없애기 
	strReceivePhone.Replace(_T("-"),_T(""));

	pDM->InitDBFieldData();	
	pDM->AddDBFieldData( _T("TRAN_CODE")	, _T("NUMERIC"), strMsgID );
	pDM->AddDBFieldData( _T("RECEIVER")	    , _T("STRING") , strReceivePhone);
	pDM->AddDBFieldData( _T("SENDER")       , _T("STRING") , strCallBackNum );
	pDM->AddDBFieldData( _T("STD_DATE")  	, _T("NUMERIC") , _T("TO_CHAR(SYSDATE,'YYYYMMDDHH24MISS')"));		
	pDM->AddDBFieldData( _T("MESSAGE") 		, _T("STRING") , strMsg ); 
	pDM->AddDBFieldData( _T("URL") 	    	, _T("STRING") , strURL );
	pDM->MakeInsertFrame( _T("MT_SENDQUEUE") );

	return 0;
}
INT CSamsungNetworks::SendSMSMsg(
							   CStringList &receiverName, 
							   CStringList &receiverNum, 
							   CString title, 
							   CStringList &msg, 
							   CString callbackNum, 
							   VOID *pParent)
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
	
	if ( strMSGGB.IsEmpty() ) strMSGGB = _T("0");

	INT cnt = receiverName.GetCount();
	for( idx = 0; idx < cnt; idx++ )
	{
		dCnt = 0;
		
		strReceivePhone = receiverNum.GetAt(receiverNum.FindIndex( idx ));
		strMSG = msg.GetAt(msg.FindIndex( idx ));

		while ( !strMSG.IsEmpty() )
		{
			strSendMsg = GetDivideMsg(strMSG);	
			MakeMsgID(strMsgID);
			SendMsgInDB(pDM, strMsgID, strURL, strReceivePhone, strCallBackNum, strSendMsg);
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