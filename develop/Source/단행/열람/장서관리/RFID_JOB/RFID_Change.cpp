// RFID_Change.cpp: implementation of the CRFID_Change class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFID_Change.h"
#include "CharConvert.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRFID_Change::CRFID_Change(CESL_Mgr* pParentMgr)
{
	m_pParentMgr = pParentMgr;
}

CRFID_Change::~CRFID_Change()
{

}
INT CRFID_Change::Change( INT nMode )
{
	INT				ids;
	CRFID_Interface RFID;
	INT				nStatus;
	INT				nTagCnt;
	CStringArray	SERIAL_LIST;
	CString			strFuncInfo;

	nStatus = RFID.GET_STATUS();
	CLocCommonAPI::MakeRFIDLog( _T("RFID_GetStatus()"), _T("RFID 동기화"), __WFILE__, __LINE__, nStatus );

	if( 0 != nStatus )
	{
		AfxMessageBox(_T("발견된 리더기가 없습니다."));
		return 1;
	}

	ids = RFID.GetTagSerial(SERIAL_LIST );
	nTagCnt = SERIAL_LIST.GetSize();
	
	strFuncInfo.Format( _T("RFID_GetTagSerials( SerialList, %d )"), nTagCnt );
	CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID Tag 동기화"), __WFILE__, __LINE__, ids );

	for( INT i = 0;i < nTagCnt;i++ )
	{		
		if( nMode%10 == 1 )
		{
			ids = RFID.ChangeTagLoanProcDataByUID( SERIAL_LIST.GetAt(i), 1 );
			strFuncInfo.Format( _T("RFID_PutTagDataBYSerial( SerialList[i], %s, %s )"),  _T("IDX_LOAN") , _T("1") );
			CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID 동기화"), __WFILE__, __LINE__, ids );
		}
		else if( nMode%10 == 2 )
		{
			ids = RFID.ChangeTagLoanProcDataByUID( SERIAL_LIST.GetAt(i), 0 );
			strFuncInfo.Format( _T("RFID_PutTagDataBYSerial( SerialList[i], %s, %s )"),  _T("IDX_LOAN") , _T("0") );
			CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID 동기화"), __WFILE__, __LINE__, ids );
		}

		if( nMode/10 == 1 )
		{
			TAGDATASTRU pTagStru;
			memset( &pTagStru, 0x00, sizeof( TAGDATASTRU ) );

			ids = RFID.GetTagAllDataByUID( SERIAL_LIST.GetAt(i), &pTagStru );

			CharConvert cv;
			TCHAR*      szTmp = NULL;
			INT         len   = 0;
			
			if(pTagStru.bValue)
			{
				ids = RFID.ChangeTagLoanProcDataByUID( SERIAL_LIST.GetAt(i), 1 );
				strFuncInfo.Format( _T("RFID_PutTagDataBYSerial( SerialList[i], %s, %s )"),  _T("IDX_LOAN") , _T("1") );
				CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID 동기화"), __WFILE__, __LINE__, ids );
			}
			
				
			// ---------------------------------------------------------------	
		}
		else if( nMode/10 == 2 )
		{
			TAGDATASTRU pTagStru;
			memset( &pTagStru, 0x00, sizeof( TAGDATASTRU ) );

			ids = RFID.GetTagAllDataByUID( SERIAL_LIST.GetAt(i), &pTagStru );

			if(!pTagStru.bValue)
			{
				ids = RFID.ChangeTagLoanProcDataByUID( SERIAL_LIST.GetAt(i), 0 );
				strFuncInfo.Format( _T("RFID_PutTagDataBYSerial( SerialList[i], %s, %s )"),  _T("IDX_LOAN") , _T("1") );
				CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID 동기화"), __WFILE__, __LINE__, ids );
			}
		}
	
	}
	return ids;
}

INT CRFID_Change::RecordRFIDFromUserInfo( CString sLibCode , CString sUserNo , CString sPassWord , CString& sSerialNo )
{
	INT				ids;
	CRFID_Interface	RFID( this->m_pParentMgr );
	INT				nStatus;
	INT				nTagCnt;
	CStringArray	SERIAL_LIST;
	CString			strFuncInfo;
	USERDATASTRU	pUserStru;

	nStatus = RFID.GET_STATUS();
	CLocCommonAPI::MakeRFIDLog( _T("RFID_GetStatus()"), _T("RFID 동기화"), __WFILE__, __LINE__, nStatus );
		if( 0 != nStatus )
	{
		AfxMessageBox(_T("발견된 리더기가 없습니다."));
		return 1;
	}
		
	ids = RFID.GetTagSerial( SERIAL_LIST );
	nTagCnt = SERIAL_LIST.GetSize();
	
	strFuncInfo.Format( _T("RFID_GetTagSerials( SerialList, %d )"), nTagCnt );
	CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID Tag 동기화"), __WFILE__, __LINE__, ids );

	if( 1 != nTagCnt )
	{
		if( 0 == nTagCnt )
			AfxMessageBox( _T("회원증이 존재하지 않습니다!!") );
		else if( 1 < nTagCnt )
			AfxMessageBox( _T("하나의 회원증만 리더기에 올려놓으십시오!!") );
		return 2;
	}

	memset( &pUserStru, 0x00, sizeof( USERDATASTRU ) );

	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;
	cv.UnicodeToMultiByte( (TCHAR*)(LPCTSTR)SERIAL_LIST.GetAt(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pUserStru.strSerial,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( sLibCode.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pUserStru.strLibCode,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( sUserNo.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pUserStru.strUserNo,szTmp);
		delete [] szTmp;
	}

	ids = RFID.PutTagAllUserData( &pUserStru );
	CLocCommonAPI::MakeRFIDLog( _T("RFID_PutTagAllDataBYSerial( RFM_DATA_STRU *pTagData )"), _T("RFID 동기화"), __WFILE__, __LINE__, ids );
	if( 0 != ids ) return ids;
	
	sSerialNo.Format( _T("%s"), SERIAL_LIST.GetAt(0) );

	return ids;
}

INT CRFID_Change::GetWriteInfo(CString strSerial,TAGDATASTRU* pTagStru)
{
	INT					ids;
	CMakeRFIDInfoFromDB MakeRfid(m_pParentMgr);
	ids = MakeRfid.MakeRFIDInfoFromDB(strSerial,pTagStru);

	return ids;
}
