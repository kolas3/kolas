// SMS_Env_Info.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_Env_Info.h"
#include "CfgFileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_Env_Info dialog


CSMS_Env_Info::CSMS_Env_Info(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_Env_Info)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	InitESL_Mgr( _T("CO_SMS_ENV_INFO") );
}


VOID CSMS_Env_Info::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_Env_Info)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_Env_Info, CDialog)
	//{{AFX_MSG_MAP(CSMS_Env_Info)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_Env_Info message handlers

INT CSMS_Env_Info::MakeSMSEnv()
{
	EFS_BEGIN

	CString strWhere;
	INT ids;

	CESL_DataMgr* pDM;

	pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));

	CString strManageCode;
	if(NULL != m_pInfo && TRUE != m_pInfo->MANAGE_CODE.IsEmpty())
	{
		strManageCode = _T("'") + m_pInfo->MANAGE_CODE + _T("'");
	}
	else
	{
		strManageCode = _T("UDF_MANAGE_CODE");
	}
	strWhere.Format(
		_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
		_T("WHERE CLASS_ALIAS='SMS시스템정보' AND MANAGE_CODE=%s"),
		strManageCode);

	pDM->GetOneValueQuery(strWhere, m_strSMSSystemInfo);

	if ( m_strSMSSystemInfo.IsEmpty() )
	{
		strWhere = _T("");
	}
	else
	{
		strWhere.Format(_T("SMS_TYPE='%s'"), m_strSMSSystemInfo );
	}

	const INT nCnt = 2;
	CString sDMAlias[nCnt] =
	{
		_T("DM_MNG_SMS_TYPE_ENV") , _T("DM_MNG_SMS_TOTAL_ENV") 
	};		
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pDM = FindDM(sDMAlias[i]);
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSMSEnv") );

		if ( sDMAlias[i] == _T("DM_MNG_SMS_TOTAL_ENV") )
		{
			ids = pDM->RefreshDataManager(strWhere);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSMSEnv") );
		}
		else
		{
			ids = pDM->RefreshDataManager(_T(""));
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSMSEnv") );
		}		
	}

	strWhere.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구사용유무' AND MANAGE_CODE=%s"),strManageCode);
	pDM->GetOneValueQuery(strWhere, m_str_Katok_Use);

	strWhere.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구에이전트사' AND MANAGE_CODE=%s"),strManageCode);
	pDM->GetOneValueQuery(strWhere, m_str_Katok_Agent);

	strWhere.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구SVI' AND MANAGE_CODE=%s"),strManageCode);
	pDM->GetOneValueQuery(strWhere, m_str_Katok_SVI);

	strWhere.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='SMS연도표기방식' AND value_name='연도자릿수설정' AND MANAGE_CODE=%s"),strManageCode);
	pDM->GetOneValueQuery(strWhere, m_str_SmsYearLength);


	return 0;
	
	EFS_END
		
	return -1;
	
}

INT CSMS_Env_Info::GetSMSEnv( CString sSMSType , SMS_CONNECTION_INFO* pSMS_ConnectionInfo , SMS_MSG_INFO* pSMS_MsgInfo )
{
	EFS_BEGIN

	GetSMSConnectionEnv( pSMS_ConnectionInfo , pSMS_MsgInfo );

	GetSMSMsgEnv( sSMSType , pSMS_MsgInfo);

	return 0;

	EFS_END

	return -1;
}

INT CSMS_Env_Info::GetSMSConnectionEnv(SMS_CONNECTION_INFO* pSMS_ConnectionInfo , SMS_MSG_INFO* pSMS_MsgInfo )
{
	EFS_BEGIN

	INT ids;
		
	CString sDMAlias = _T("DM_MNG_SMS_TOTAL_ENV");

	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetSMSConnectionEnv") );
	}
	
	INT		nRowCnt	=	pDM->GetRowCount();
	if( 0 == nRowCnt ) 
	{
		return 1;
	}

	const INT nFieldCnt = 12;
	CString sDMFieldAlias[nFieldCnt] =
	{
		_T("바인드아이디") , _T("바인드패스워드") ,			_T("이용자아이디") , 
		_T("이용자도메인") , _T("이용자번호") ,				_T("이용자접속IP") , 
		_T("문자구성정보") , _T("나누어보내기허용여부") ,	_T("회신전화번호") , 
		_T("도서관명") ,	 _T("TITLE_SIZE"),				_T("LMS사용여부")

	};

	CString sGetData[nFieldCnt];

	INT		i;
	for( i=0 ; i<nFieldCnt ; i++ )
	{
		ids = pDM->GetCellData( sDMFieldAlias[i] , 0 , sGetData[i] );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("GetSMSConnectionEnv") );
		}
	}

	pSMS_ConnectionInfo->sBindId = sGetData[0];
	pSMS_ConnectionInfo->sBindPwd = sGetData[1];
	pSMS_ConnectionInfo->sUserId = sGetData[2];
	pSMS_ConnectionInfo->sUserDomain = sGetData[3];
	pSMS_ConnectionInfo->sUserPhoneId = sGetData[4];
	pSMS_ConnectionInfo->sUserAccessIp = sGetData[5];
	pSMS_ConnectionInfo->sCfgConFilePath = _T("..\\cfg\\x_msggate.conf");

	
	pSMS_MsgInfo->sMsgComponentInfo = sGetData[6];
	pSMS_MsgInfo->sDivideYN = sGetData[7];
	pSMS_MsgInfo->sLmsUseYN = sGetData[11];
	pSMS_MsgInfo->sSMSSystemInfo = m_strSMSSystemInfo;

	CString strManageCode;
	if( NULL != m_pInfo && TRUE != m_pInfo->MANAGE_CODE.IsEmpty() )
	{
		strManageCode = _T("'") + m_pInfo->MANAGE_CODE + _T("'");
	}
	else
	{
		strManageCode = _T("UDF_MANAGE_CODE");
	}

	CString query;
	query.Format(
		_T("SELECT SMS_CALLBACK_PHONE,SMS_LIB_NAME ")
		_T("FROM MN_LIBINFO2_TBL ")
		_T("WHERE MANAGE_CODE=%s"),
		strManageCode);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	ids = TmpDM.RestructDataManager( query );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("GetSMSConnectionEnv") );
	}

	pSMS_MsgInfo->sCallBackNumber = TmpDM.GetCellData(0,0);
	pSMS_MsgInfo->sLibName = TmpDM.GetCellData(0,1);

	CCfgFileApi cfgFileApi;
	CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
	cfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(ff.FindFile(strCfgFilePath))
	{
		CString strCallbackTarget;
		cfgFileApi.GetData(_T("회신번호설정적용대상"), strCallbackTarget);
		CString strValue;
		if(_T("1") == strCallbackTarget)
		{
			cfgFileApi.GetData(_T("회신번호"), strValue);
			if(!strValue.IsEmpty()) pSMS_MsgInfo->sCallBackNumber = strValue;
		}
		CString strLibNameTarget;
		cfgFileApi.GetData(_T("도서관명설정적용대상"), strLibNameTarget);
		if(_T("1") == strLibNameTarget)
		{
			cfgFileApi.GetData(_T("도서관명"), strValue);
			if(!strValue.IsEmpty()) pSMS_MsgInfo->sLibName = strValue;
		}
	}

	INT		nTitleSize	=	_ttoi( sGetData[10] );
	pSMS_MsgInfo->nTitleSize	=	nTitleSize;

	pSMS_MsgInfo->m_str_Katok_Use = m_str_Katok_Use;
	pSMS_MsgInfo->m_str_Katok_Agent = m_str_Katok_Agent;
	pSMS_MsgInfo->m_str_Katok_SVI = m_str_Katok_SVI;

	pSMS_MsgInfo->m_str_SmsYearLength = m_str_SmsYearLength;

	return 0;
	
	EFS_END
		
	return -1;
}

INT CSMS_Env_Info::GetSMSMsgEnv( CString sSMSType , SMS_MSG_INFO* pSMS_MsgInfo )
{
	EFS_BEGIN

	INT ids;
		
	CString sDMAlias = _T("DM_MNG_SMS_TYPE_ENV");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSMSConnectionEnv") );
	
	if( pDM->GetRowCount() == 0 ) return 1;
	
	const INT nFieldCnt = 2;
	CString sDMFieldAlias[nFieldCnt] =
	{
		_T("종류") , _T("기본문자") 
	};
	
	CString sGetData[nFieldCnt];
	
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = CLocCommonAPI::GetCellData(  this , pDM , _T("타입") , sSMSType , sDMFieldAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSMSConnectionEnv") );
	}
	
	pSMS_MsgInfo->sTitle = sGetData[0];
	pSMS_MsgInfo->sDefaultMsg = sGetData[1];
	
	pSMS_MsgInfo->sSMSType = sSMSType;
			
	return 0;
	
	EFS_END
		
	return -1;

}


VOID CSMS_Env_Info::InitSMSFieldData()
{
	m_USER_KEY.RemoveAll();
	m_NAME.RemoveAll();
	m_SEND_PHONE_NUMBER.RemoveAll();
	m_SEND_MESSAGE.RemoveAll();
	m_SMS_TYPE.RemoveAll();
	m_SEND_DIVIDE_CNT.RemoveAll();
	m_SEND_TIME.RemoveAll();
	m_SEND_WORKER.RemoveAll();
	m_SEND_SUCCESS_YN.RemoveAll();
	m_SEND_FAIL_REASON.RemoveAll();
	m_FIRST_WORK.RemoveAll();
	m_LAST_WORK.RemoveAll();
}
VOID CSMS_Env_Info::AddSMSDBFieldData( 
		CString userKey, 
		CString name,
		CString sendPhoneNum, 
		CString msg, 
		CString type, 
		INT divideCnt, 
		CString time, 
		CString worker, 
		CString successYN, 
		CString failReason, 
		CString firstWork, 
		CString lastWork )
{
	m_USER_KEY.AddTail( userKey );
	m_NAME.AddTail( name );
	m_SEND_PHONE_NUMBER.AddTail( sendPhoneNum );
	m_SEND_MESSAGE.AddTail( msg );
	m_SMS_TYPE.AddTail( type );
	m_SEND_DIVIDE_CNT.Add( divideCnt );
	m_SEND_TIME.AddTail( time );
	m_SEND_WORKER.AddTail( worker );
	m_SEND_SUCCESS_YN.AddTail( successYN );
	m_SEND_FAIL_REASON.AddTail( failReason );
	m_FIRST_WORK.AddTail( firstWork );
	m_LAST_WORK.AddTail( lastWork );
}

VOID CSMS_Env_Info::InsertSMSLogData()
{
	CString sDMAlias = _T("DM_MNG_SMS_TOTAL_ENV");
	CESL_DataMgr* pDM = FindDM( sDMAlias );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	const INT cnt = 13;
	CString fieldName[ cnt ] = {
		_T("REC_KEY"), 
		_T("USER_KEY"), 
		_T("NAME"), 
		_T("SEND_PHONE_NUMBER"), 
		_T("SEND_MESSAGE"), 
		_T("SMS_TYPE"), 
		_T("SEND_DIVIDE_CNT"), 
		_T("SEND_TIME"), 
		_T("SEND_WORKER"), 
		_T("SEND_SUCCESS_YN"), 
		_T("SEND_FAIL_REASON"), 
		_T("FIRST_WORK"), 
		_T("LAST_WORK") };
	CString fieldType[ cnt ] = {
		_T("NUMERIC"), 
		_T("NUMERIC"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("NUMERIC"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING") };
	CString data[ cnt ];	
	
	INT rowCnt = m_USER_KEY.GetCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		GetFieldData( i, data );
		for( INT j = 0; j < cnt; j++ )
		{
			if( !data[ j ].IsEmpty() )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ], true );
		}
		pDM->MakeInsertFrame( _T("MN_SMS_LOG_TBL") );
	}

	pDM->SendFrame();
	pDM->EndFrame();
}

VOID CSMS_Env_Info::GetFieldData( INT idx, CString data[] )
{
	data[  0 ] = _T("ESL_SMS_SEQ.NEXTVAL");
	data[  1 ] = m_USER_KEY.GetAt( m_USER_KEY.FindIndex( idx ) );
	data[  2 ] = m_NAME.GetAt( m_NAME.FindIndex( idx ) );
	data[  3 ] = m_SEND_PHONE_NUMBER.GetAt( m_SEND_PHONE_NUMBER.FindIndex( idx ) );
	data[  4 ] = m_SEND_MESSAGE.GetAt( m_SEND_MESSAGE.FindIndex( idx ) );
	data[  5 ] = m_SMS_TYPE.GetAt( m_SMS_TYPE.FindIndex( idx ) );
	data[  6 ].Format( _T("%d"), m_SEND_DIVIDE_CNT.GetAt( idx ) );
	data[  7 ] = m_SEND_TIME.GetAt( m_SEND_TIME.FindIndex( idx ) );
	data[  8 ] = m_SEND_WORKER.GetAt( m_SEND_WORKER.FindIndex( idx ) );
	data[  9 ] = m_SEND_SUCCESS_YN.GetAt( m_SEND_SUCCESS_YN.FindIndex( idx ) );
	data[ 10 ] = m_SEND_FAIL_REASON.GetAt( m_SEND_FAIL_REASON.FindIndex( idx ) );
	data[ 11 ] = data[ 12 ] = GetWorkLogMsg( _T("SMS관리"), __WFILE__, __LINE__ );
}

int		SMS_MSG_INFO::CheckUseKatok()
{
	if( m_str_Katok_Use.Find( _T('Y')) > -1 ) return 1;
	return 0;
}

