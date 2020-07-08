// SMS_MakeMsg.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_MakeMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_MakeMsg dialog


CSMS_MakeMsg::CSMS_MakeMsg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_MakeMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = (CESL_Mgr*)pParent;

	m_nType_SendPlatform = 0;
}


VOID CSMS_MakeMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_MakeMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_MakeMsg, CDialog)
	//{{AFX_MSG_MAP(CSMS_MakeMsg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_MakeMsg message handlers

INT CSMS_MakeMsg::MakeSMSMsg( 
			   CString	sSMSType ,
			   SMS_MSG_INFO* pSMS_MsgInfo ,	
			   CString	sReceiptName ,		
			   CString	sDateInfo ,			
			   CString	sBookTitleInfo ,		
			   CString	sBookCnt ,			
			   CString	&sMsg
				,CString* psMsg_Katok
			   )
{
EFS_BEGIN

	// ----------------------------------------------------------------------------
	// 1. 문자 구성 정보      
	// ----------------------------------------------------------------------------	
	CString sSMsgComponentInfo = pSMS_MsgInfo->sMsgComponentInfo;

	BOOL bIsShowLibName = TRUE;
	BOOL bIsShowUserName = TRUE;
	BOOL bIsShowDefaultMsg = TRUE;
	BOOL bIsShowDateInfo = TRUE;
	BOOL bIsShowTitleInfo = TRUE;

	if( '0' == sSMsgComponentInfo.GetAt(4) )
	{
		bIsShowLibName = FALSE;
	}

	if( '0' == sSMsgComponentInfo.GetAt(0) )
	{
		bIsShowUserName = FALSE;
	}

	if( '0' == sSMsgComponentInfo.GetAt(1) )
	{
		bIsShowDefaultMsg = FALSE;
	}

	if( '0' == sSMsgComponentInfo.GetAt(2) )
	{
		bIsShowDateInfo = FALSE;
	}

	if( '0' == sSMsgComponentInfo.GetAt(3) )
	{
		bIsShowTitleInfo = FALSE;
	}
	

	if( TRUE != bIsShowLibName ) 
	{
		pSMS_MsgInfo->sLibName.Empty();
	}

	if( TRUE != bIsShowUserName ) 
	{
		sReceiptName.Empty();
	}

	if( TRUE != bIsShowDefaultMsg ) 
	{
		pSMS_MsgInfo->sDefaultMsg.Empty();
	}

	if( TRUE != bIsShowDateInfo ) 
	{
		sDateInfo.Empty();
	}

	if( TRUE != bIsShowTitleInfo ) 
	{
		sBookTitleInfo.Empty();
	}

	CString sSTitle;
	if( FALSE == pSMS_MsgInfo->sLibName.IsEmpty() )
	{
		sSTitle = _T("[") + pSMS_MsgInfo->sLibName + _T("]");
	}

	CString sSUserName;
	sSUserName = sReceiptName;
	sSUserName.TrimLeft(); sSUserName.TrimRight();

	CString sSDefaultMsg;
	sSDefaultMsg = pSMS_MsgInfo->sDefaultMsg;

	if( pSMS_MsgInfo->m_str_SmsYearLength == _T("Y") && !sDateInfo.IsEmpty() )
	{
		if (sDateInfo.GetLength() == 10)
		{
			if( sDateInfo.Find(_T("/")) == 4 )
			{
				sDateInfo = sDateInfo.Right(8);
			}
		}
	}
	
	CString sSDateInfo;
	if( !sDateInfo.IsEmpty() )
	{
		if( sSMSType.Compare(_T("SMS01")) == 0 )
		{
			sSDateInfo.Format( _T("반납예정일:%s") , sDateInfo );
		}
		else if( sSMSType.Compare(_T("SMS02")) == 0 ) 
		{
			sSDateInfo.Format( _T("반납예정일:%s") , sDateInfo );
		}
		else if( 0 == sSMSType.Compare(_T("SMS03")) || 0 == sSMSType.Compare(_T("NRL03")) ) 
		{
			sSDateInfo.Format( _T("예약만기일:%s") , sDateInfo );
		}
		else if( 0 == sSMSType.Compare(_T("SMS12"))) 
		{
			sSDateInfo.Format( _T("대출만기일:%s") , sDateInfo );
		}
		else if( 0 == sSMSType.Compare(_T("SMS16"))) 
		{
			sSDateInfo.Format( _T("만료일:%s") , sDateInfo );
		}
		else if( 0 == sSMSType.Compare(_T("SMS17"))) 
		{
			sSDateInfo.Format( _T("만료예정일:%s") , sDateInfo );
		}

	}

	sBookTitleInfo.Replace( _T(" ") , _T("") );
	
	CString		sBookTitleInfo_katok = sBookTitleInfo;
	CString		sSBookTitleInfo_katok;

	sBookTitleInfo = GetDivideMsg( sBookTitleInfo, pSMS_MsgInfo->nTitleSize );

	CString sSBookTitleInfo;
	if( !sBookTitleInfo.IsEmpty() )
	{
		if(_ttoi(sBookCnt) > 1){
			sSBookTitleInfo.Format( _T("('%s..'등%d권)") , sBookTitleInfo , _ttoi(sBookCnt));
		}
		else {
			sSBookTitleInfo.Format( _T("('%s..')") , sBookTitleInfo  );
		}

		if(_ttoi(sBookCnt) > 1){
			sSBookTitleInfo_katok.Format( _T("('%s..'등%d권)") , sBookTitleInfo_katok , _ttoi(sBookCnt));
		}
		else {
			sSBookTitleInfo_katok.Format( _T("('%s')") , sBookTitleInfo_katok  );
		}
	}

	sMsg = sSTitle;

	sMsg += sSUserName + sSDefaultMsg;
	
	if( !sSDateInfo.IsEmpty() )
	{
		if(		sSMSType.Compare(_T("SMS01")) == 0 
			||	sSMSType.Compare(_T("SMS02")) == 0 
			||	sSMSType.Compare(_T("SMS03")) == 0
			||	sSMSType.Compare(_T("NRL03")) == 0  
			||	sSMSType.Compare(_T("SMS12")) == 0 
			) 
		{
			if( !sMsg.IsEmpty() ) sMsg += _T(",");
			sMsg += sSDateInfo;
		}
	}	

	if(sSMSType.Compare(_T("SMS16")) == 0 || sSMSType.Compare(_T("SMS17")) == 0)
	{
		sMsg.Replace(_T("%만료일%"), sDateInfo);
	}
	if(sSMSType.Compare(_T("TLL01")) == 0 )
	{
		sMsg.Replace(_T("%대출대기일%"), sDateInfo);
	}

	if( !sSBookTitleInfo.IsEmpty() )
	{
		if(		sSMSType.Compare(_T("SMS01")) == 0 
			||	sSMSType.Compare(_T("SMS02")) == 0 
			||	sSMSType.Compare(_T("SMS03")) == 0 
			||	sSMSType.Compare(_T("SMS04")) == 0 
			||	sSMSType.Compare(_T("SMS07")) == 0 
			||	sSMSType.Compare(_T("SMS08")) == 0 
			||	sSMSType.Compare(_T("TLL01")) == 0 
			||	sSMSType.Compare(_T("TLL02")) == 0 
			||	sSMSType.Compare(_T("TLL03")) == 0
			||	sSMSType.Compare(_T("NRL03")) == 0  
			||	sSMSType.Compare(_T("SMS12")) == 0
			||	sSMSType.Compare(_T("SMS13")) == 0
			||	sSMSType.Compare(_T("SMS14")) == 0  			
			||	sSMSType.Compare(_T("SMS15")) == 0  			
			||	sSMSType.Compare(_T("SMS18")) == 0  			
			||	sSMSType.Compare(_T("SMS19")) == 0  			
			) 
		{
			if( psMsg_Katok != NULL )
			{
				(*psMsg_Katok) =	sMsg;
				(*psMsg_Katok) +=	sSBookTitleInfo_katok;
			}

			sMsg += sSBookTitleInfo;
		}
	}else{

		if( psMsg_Katok != NULL ) {
			(*psMsg_Katok) =	sMsg;
		}
		
	}	

	if( pSMS_MsgInfo->CheckUseKatok() == 0 )
	{
	CString m_strSMSSystemInfo = pSMS_MsgInfo->sSMSSystemInfo;
	
	if(	m_strSMSSystemInfo == _T("0") || m_strSMSSystemInfo == _T("1") || m_strSMSSystemInfo == _T("2") || 
		m_strSMSSystemInfo == _T("3") || m_strSMSSystemInfo == _T("4") || m_strSMSSystemInfo == _T("5") ||
		m_strSMSSystemInfo == _T("6"))
	{
		if ( pSMS_MsgInfo->sDivideYN == _T("N") && (GetSMSLength(sMsg.GetBuffer(0))> 80) )
		{
			sMsg = GetDivideMsg(sMsg, 80);
		}
	}

	else 	
	{
		if(pSMS_MsgInfo->sLmsUseYN ==_T("N"))
		{
			if ( pSMS_MsgInfo->sDivideYN == _T("N") && (GetSMSLength(sMsg.GetBuffer(0))> 90) )
			{
				sMsg = GetDivideMsg(sMsg, 90);
			}
		}
		else
		{
			sMsg = GetDivideMsg(sMsg, 2000);
		}
	}
	}else{
		sMsg = GetDivideMsg(sMsg, 80);
	}

	INT nn = sMsg.GetLength();
	
	return 0;

EFS_END
	return -1;
}


BOOL CSMS_MakeMsg::IsCanCut(CHAR *str, INT pos)
{
	INT i, nLastPos = -1;
	if( strlen(str) <= pos )
		return TRUE;
	
	TCHAR ch = str[pos];
	
	if( pos + 1 == strlen(str) || ( ch & ( 0x80 ) ) != 128 )
		return true;
	
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return true;
		else
			return false;

	for( i = 0; i < pos; i++ )
	{
		ch = str[i];
		
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return true;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return true;
	
	BOOL result = !( ( pos - nLastPos ) % 2 );
	return result;
}

CString CSMS_MakeMsg::GetDivideMsg(CString msg, INT nCut)
{
	CString sendMsg;
	INT length;	

	CHAR		*szMsg = NULL;
	CHAR		szCutMsg[2048];
	INT			nLen = 0;

	UnicodeToMultiByte(msg.GetBuffer(0), &szMsg, &nLen );	

	if(NULL == szMsg) return _T("");

	if( IsCanCut( szMsg, nCut - 1 ) )
		length = nCut;
	else
		length = nCut - 1;

	strncpy(szCutMsg , szMsg, length);
	szCutMsg[length] = '\0';


	TCHAR *szUnicodeMessage;
	INT		nUnicodeLen;

	MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );

	sendMsg.Format(_T("%s"), szUnicodeMessage );
	
	if(szMsg != NULL)
	{
		delete szMsg;
	}
	if(szUnicodeMessage != NULL)
	{
		delete szUnicodeMessage;
	}

	return sendMsg;
}

INT	CSMS_MakeMsg::GetSMSLength(_TCHAR *fi_pSourceData)
{
	INT		nWCharSource;
	INT		nBytesNeeded;

	nWCharSource   = wcslen(fi_pSourceData);

	nBytesNeeded = WideCharToMultiByte( CP_ACP 	, 
										0				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL, 
										NULL );
	return nBytesNeeded;
}

INT	CSMS_MakeMsg::UnicodeToMultiByte(_TCHAR *fi_pSourceData, CHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	CHAR	*pszDestinationData;
	INT		nWCharSource;
	INT		nBytesNeeded;

	
	nWCharSource   = wcslen(fi_pSourceData);

	if ( nWCharSource <= 0 ) {
		*fo_nByteDes = 0;
		return -100;
	}
	
	nBytesNeeded = WideCharToMultiByte( CP_ACP 	, 
										0				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL, 
										NULL );
	
	if (nBytesNeeded == 0) 
	{
		*fo_nByteDes = 0;
		DWORD dw = GetLastError();
		return -100 ;
	}
		
	pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
	
	nBytesNeeded = WideCharToMultiByte     (CP_ACP		, 
											0					,
											(LPWSTR)fi_pSourceData		, 
											nWCharSource				,
											(CHAR*)pszDestinationData	, 
											nBytesNeeded				, 
											NULL			    , 
										    NULL );
	if (nBytesNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -100 ;
	}

	*(LPSTR)((LPSTR)pszDestinationData + nBytesNeeded) = '\0';	

	*fo_pDestinationData = pszDestinationData;
	*fo_nByteDes         = nBytesNeeded;

	return 0;
}



INT	CSMS_MakeMsg::MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	

	nSouceDataByte = strlen(fi_pSourceData);
	
	nWCharNeeded = MultiByteToWideChar( CP_ACP , 
										(UINT)0  , 
										fi_pSourceData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		return -100 ;
	}
	
	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));	
		
	nWCharNeeded = MultiByteToWideChar(	CP_ACP				, 
										0						,
										fi_pSourceData					, 
										nSouceDataByte			   		,
										pszDestinationData          	, 
										nWCharNeeded);
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -100 ;
	}	

	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
	
	*fo_nByteDes = 2*nWCharNeeded ;	

	*fo_pDestinationData = pszDestinationData;


	return 0;
}