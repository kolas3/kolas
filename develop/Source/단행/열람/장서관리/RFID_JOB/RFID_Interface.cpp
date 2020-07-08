// RFID_Interface.cpp: implementation of the CRFID_Interface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFID_Interface.h"
#include "CharConvert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRFID_Interface::CRFID_Interface()
{
	m_pParentMgr = NULL;
	init();
}
CRFID_Interface::CRFID_Interface(CESL_Mgr* pParentMgr)
{
	m_pParentMgr    = pParentMgr;
	CONNECTION_INFO = pParentMgr->m_pInfo->CONNECTION_INFO;
	init();
}
CRFID_Interface::~CRFID_Interface()
{

}

CRFID_Interface::init()
{
	g_RFIDIF.hDriverDLL = LoadLibrary(_T("RFID_API.dll"));
	g_RFIDIF.fpRFID_GetStatus	= (tpRFID_GetStatus)GetProcAddress( LoadLibrary(_T("RFID_API.dll")), "RFID_GetStatus");
	g_RFIDIF.fpRFID_GetSerial	= (tpRFID_GetSerial)GetProcAddress( LoadLibrary(_T("RFID_API.dll")), "RFID_GetSerial");
	g_RFIDIF.fpRFID_SetSecurity	= (tpRFID_SetSecurity)GetProcAddress( LoadLibrary(_T("RFID_API.dll") ), "RFID_SetSecurity");
	g_RFIDIF.fpRFID_GetSecurity	= (tpRFID_GetSecurity)GetProcAddress( LoadLibrary( _T("RFID_API.dll") ), "RFID_GetSecurity");
	g_RFIDIF.fpRFID_SetLicense	= (tpRFID_SetLicense)GetProcAddress( LoadLibrary( _T("RFID_API.dll") ), "RFID_SetLicense");
	g_RFIDIF.fpRFID_SetBookInfo	= (tpRFID_SetBookInfo)GetProcAddress( LoadLibrary( _T("RFID_API.dll") ), "RFID_SetBookInfo");
	g_RFIDIF.fpRFID_GetBookInfo	= (tpRFID_GetBookInfo)GetProcAddress( LoadLibrary( _T("RFID_API.dll") ), "RFID_GetBookInfo");

}
//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  RFID_GetStatus Interface
//////////////////////////////////////////////////////////////////////
INT CRFID_Interface::GET_STATUS(CHAR* strErrorMsg)
{
	if (g_RFIDIF.hDriverDLL == NULL)	
	{
		return -1;
	}
	return g_RFIDIF.fpRFID_GetStatus(strErrorMsg);
	
}
INT CRFID_Interface::GET_STATUS()
{
	if (g_RFIDIF.hDriverDLL == NULL)	
	{
		return -1;
	}
	CHAR strErrorMsg[1024];
	memset(strErrorMsg,0x00,sizeof(strErrorMsg));
	return g_RFIDIF.fpRFID_GetStatus(strErrorMsg);
}
//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  RFID_GetTagSerial Interface
//////////////////////////////////////////////////////////////////////
INT CRFID_Interface::GetTagSerial(CHAR* strSerials, INT& nTagCnt, CHAR* strErrorMsg)
{
	return g_RFIDIF.fpRFID_GetSerial(strSerials, &nTagCnt, strErrorMsg);
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ER200_GetTagAllDataByUID Extension
//////////////////////////////////////////////////////////////////////
INT CRFID_Interface::GetTagAllDataByUID(TAGDATASTRU* pTagStru)
{
	INT ids =0;	
	CHAR strErrorMsg[1024];
	memset(strErrorMsg,0x00,sizeof(strErrorMsg));

	ids = g_RFIDIF.fpRFID_GetBookInfo(	(char*)pTagStru->strSerial, 
										(char*)pTagStru->strLibCode, 
										(char*)pTagStru->strRegNo, 
										(char*)pTagStru->strISBN, 
										&pTagStru->bValue,
										strErrorMsg );

	if( ids != 0 ) return ShowErrorMsg( _T("CRFID_Interface::GetTagAllDataByUID"), 1, ids,strErrorMsg );

	return 0;
}

INT CRFID_Interface::GetTagAllDataByUID( CString SERIAL, TAGDATASTRU* pTagStru)
{
	memset(pTagStru,0,sizeof(TAGDATASTRU));	
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;
	
	cv.UnicodeToMultiByte( SERIAL.GetBuffer(0), &szTmp, &len );
	if( 0 < len)
	{
		strcpy((char*)pTagStru->strSerial,szTmp);
		delete [] szTmp;
	}
	
	return ( this->GetTagAllDataByUID( pTagStru ) );
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ER200_PutTagAllDataByUID Interface
//////////////////////////////////////////////////////////////////////
INT CRFID_Interface::PutTagAllDataByUID(TAGDATASTRU* pTagStru)
{
	CString strISBN;
	CHAR strErrorMsg[1024];
	
	CharConvert cv;
	TCHAR*      szUTmp = NULL;
	INT         len   = 0;
	
	if( 0 == cv.MultiByteToUnicode( (char*)pTagStru->strISBN, &szUTmp, &len) )
	{
		strISBN.Format(_T("%s"), szUTmp);
		delete [] szUTmp;
	}	
	
	if( 10 == strISBN.GetLength() )
		this->ConvertEAN13( strISBN, strISBN );
		
	CHAR*       szTmp = NULL;
	cv.UnicodeToMultiByte( strISBN.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{
		strcpy((char*)pTagStru->strISBN,szTmp);
		delete [] szTmp;
	}
	memset(strErrorMsg,0x00,sizeof(strErrorMsg));
	INT ids = g_RFIDIF.fpRFID_SetBookInfo( (char*)pTagStru->strSerial, (char*)pTagStru->strLibCode, (char*)pTagStru->strRegCode, (char*)pTagStru->strRegNo, (char*)pTagStru->strISBN, strErrorMsg );
	if( ids != 0 ) return ShowErrorMsg( _T("CRFID_Interface::PutTagAllDataByUID"), 1, ids,strErrorMsg );
	
	return ids;
}

INT CRFID_Interface::GetTagSerial(CStringArray& SERIAL_LIST)
{
	CHAR strSerials[1024];
	CHAR strErrorMsg[1024];
	char* token;
	INT nTagCnt = 0 ;
	INT	ids;

	memset(strSerials,0x00,sizeof(strSerials));
	memset(strErrorMsg,0x00,sizeof(strErrorMsg));

	ids = this->GetTagSerial(strSerials, nTagCnt, strErrorMsg );

	if( 0 != ids ) return ShowErrorMsg( _T("CRFID_Interface::GetTagSerial"), 1, ids,strErrorMsg );

	token = strtok(strSerials,",");

	for( INT i=0;i<nTagCnt;i++ )
	{	
		CharConvert cv;
		TCHAR*      szTmp = NULL;
		INT         len   = 0;
		if( 0 == cv.MultiByteToUnicode((char*)token,&szTmp,&len) )
		{
			SERIAL_LIST.Add( CString(szTmp) );
			delete [] szTmp;
		}
		token = strtok(NULL,",");
	}	
	return 0;
}

INT CRFID_Interface::CheckRecycle(UCHAR*  pszSerial)
{
	return 1;
}

INT CRFID_Interface::CheckRecycle(CString strSerial)
{	
	UCHAR	STR_SERIAL[17];
		
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strSerial.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{
		strcpy((char*)STR_SERIAL,szTmp);
		delete [] szTmp;
	}

	return ( this->CheckRecycle( STR_SERIAL ) );
}

INT CRFID_Interface::ChangeTagLoanProcDataOnReader(INT nMode)
{
	INT          ids;
	CStringArray SERIAL_LIST;

	ids = this->GetTagSerial(SERIAL_LIST );
	if( 0 > ids ) return ids;
	if( !SERIAL_LIST.GetSize() )	return -2010;

	for( INT i=0;i<SERIAL_LIST.GetSize();i++ )
	{
		ids = this->ChangeTagLoanProcDataByUID( SERIAL_LIST.GetAt(i), nMode );
		if( 0 > ids ) return ids;
	}

	return 0;
}

INT CRFID_Interface::ChangeTagLoanProcDataByUID(CString strSerial, INT nMode)
{
	INT   ids;	
	UCHAR szSerial[17];	
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	CHAR*       strErrorMsg = NULL;
	INT         len   = 0;
	BOOL		bValue;


	cv.UnicodeToMultiByte( strSerial.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)szSerial,szTmp);
		delete [] szTmp;
	}
	
	if (nMode ==1) {
		bValue = FALSE;
	} else {
		bValue = TRUE;
	}

	ids = g_RFIDIF.fpRFID_SetSecurity( (char*)szSerial, bValue, strErrorMsg);
	
	if ( 0 != ids )
	{
		ids = g_RFIDIF.fpRFID_SetSecurity( (char*)szSerial, bValue, strErrorMsg);
	}

	if( 0 != ids ) return ( ShowErrorMsg( _T("CRFID_Interface::ChangeTagLoanProcDataByUID"), 1, ids,strErrorMsg) );	

	return 0;
}

INT CRFID_Interface::ChangeTagUserDataOnReader( CString strLibCode,  CString strUserNo, 
											    CString strPassWord, CString& strSerialNo)
{
	INT          ids;
	CStringArray SERIAL_LIST;
	USERDATASTRU pUserStru;
	memset( &pUserStru, 0x00, sizeof( USERDATASTRU ) );

	ids = this->GetTagSerial(SERIAL_LIST );
	if( 0 > ids )	return ids;
	if( 0 == SERIAL_LIST.GetSize() )	return -1010;
	else if ( 1 < SERIAL_LIST.GetSize() )	return -1015;

	ids = this->GetSerialCountFromUserTable( SERIAL_LIST.GetAt(0) );
	if( 0 > ids )	return ids;
	if( 0 < ids )	return -1020;
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strLibCode.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pUserStru.strLibCode,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( strUserNo.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pUserStru.strUserNo,szTmp);
		delete [] szTmp;
	}

	ids = this->PutTagAllUserData(&pUserStru);
	if( 0 > ids )	return ids;
	strSerialNo.Format( _T("%s"), SERIAL_LIST.GetAt(0) );

	return 0;
}

INT CRFID_Interface::PutTagAllUserData(USERDATASTRU* pUserStru)
{
	INT ids = 0;
	CHAR strErrorMsg[1024];
	memset(strErrorMsg,0x00,sizeof(strErrorMsg));

	strtok((char*)pUserStru->strUserNo,":");

	ids = g_RFIDIF.fpRFID_SetLicense( (char*)pUserStru->strSerial, (char*)pUserStru->strLibCode, (char*)pUserStru->strUserNo, strErrorMsg );
	if( ids != 0 ) return ShowErrorMsg( _T("CRFID_Interface::PutTagAllUserData"), 1, ids,strErrorMsg );
		
	return ids;
}

INT CRFID_Interface::GetSerialCountFromBookTable( CString strSerial )
{
	INT          ids;
	CESL_DataMgr DM;
	CString      strQuery;
	CString      strResult;
	INT          nBoCount;
	INT          nSeCount;
	CString      strErrorMsg;

	strQuery.Format( _T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE WORKNO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE ")
		             _T("UNION ALL ")
					 _T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE WORKNO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE "), strSerial, strSerial );
	
	DM.SetCONNECTION_INFO( this->CONNECTION_INFO );
	ids = DM.RestructDataManager( strQuery );
	if( 0 > ids )	return ( ShowErrorMsg( _T("CRFID_Interface::GetSerialCountFromBookTable"), 1, ids) );	

	ids = DM.GetCellData( 0, 0, strResult );
	if( 0 > ids )	return ( ShowErrorMsg( _T("CRFID_Interface::GetSerialCountFromBookTable"), 2, ids ) );
	nBoCount = _ttoi( strResult );

	ids = DM.GetCellData( 1, 0, strResult );
	if( 0 > ids )	return ( ShowErrorMsg( _T("CRFID_Interface::GetSerialCountFromBookTable"), 3, ids ) );
	nSeCount = _ttoi( strResult );

	return ( nBoCount + nSeCount );
}

INT CRFID_Interface::GetSerialCountFromUserTable(CString strSerial)
{
	INT          ids;
	CESL_DataMgr DM;
	CString      strQuery;
	CString      strResult;
	
	strQuery.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE WORKNO='%s' "), strSerial );

	DM.SetCONNECTION_INFO( this->CONNECTION_INFO );
	ids = DM.RestructDataManager( strQuery );
	if( 0 > ids ) return ( ShowErrorMsg( _T("CRFID_Interface::GetSerialCountFromUserTable"), 1, ids ) );

	ids = DM.GetCellData( 0, 0, strResult );
	if( 0 > ids ) return ( ShowErrorMsg( _T("CRFID_Interface::GetSerialCountFromUserTable"), 2, ids ) );

	return _ttoi( strResult );
}

INT CRFID_Interface::GetSerialCountDB(CString strSerial)
{	
	INT ids = 0;

	ids = GetSerialCountFromUserTable(strSerial); 
	if( 0 > ids )	return 1;
	if( 0 < ids )	return -1020;

	ids = GetSerialCountFromBookTable(strSerial); 
	if( 0 > ids )	return 2;
	if( 0 < ids )	return -1020;

	return 0;
}

INT CRFID_Interface::GetTagDataFromBookTable(CString strSerial, TAGDATASTRU* pTagStru)
{
	INT			 ids;
	CString		 strQuery;
	CESL_DataMgr DM;

	ids = this->GetSerialCountFromBookTable( strSerial );
	if ( 0 > ids )       return ids;
	else if ( 0 == ids ) return -1010;
	else if ( 1 < ids )  return -1015;

	strQuery.Format(_T("SELECT BB.EA_ISBN, ")
						  _T(" BB.REG_CODE, ")
						  _T(" BB.REG_NO, ")
						  _T(" FROM IDX_BO_TBL IB, BO_SPECIES_TBL BS, BO_BOOK_TBL BB")
						  _T(" WHERE IB.REC_KEY = BS.REC_KEY ")
						  _T(" AND BS.REC_KEY = BB.SPECIES_KEY ")
						  _T(" AND BB.WORKNO='%s' AND IB.MANAGE_CODE = UDF_MANAGE_CODE ")
				    _T(" UNION ALL ")
					_T("SELECT IB.ST_ISSN, ")
						  _T(" BB.REG_CODE, ")
						  _T(" BB.REG_NO, ")
						  _T(" FROM IDX_SE_TBL IB, SE_SPECIES_TBL BS, SE_BOOK_TBL BB")
						  _T(" WHERE IB.REC_KEY = BS.REC_KEY ")
						  _T(" AND BS.REC_KEY = BB.SPECIES_KEY ")
						  _T(" AND BB.WORKNO='%s' AND IB.MANAGE_CODE = UDF_MANAGE_CODE "), strSerial, strSerial );

	DM.SetCONNECTION_INFO( this->CONNECTION_INFO );
	ids = DM.RestructDataManager( strQuery );
	if ( 0 > ids ) return ids;

	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strSerial.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pTagStru->strSerial,szTmp);
		delete [] szTmp;
	}
	
	cv.UnicodeToMultiByte( (DM.GetCellData( 0, 0 )).GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pTagStru->strISBN,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( LIB_CODE.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pTagStru->strLibCode,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( (DM.GetCellData( 0, 1 )).GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pTagStru->strRegCode,szTmp);
		delete [] szTmp;
	}

	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	cv.UnicodeToMultiByte( ((DM.GetCellData(0, 2)).Mid(nRegCodeLength)).GetBuffer(0), &szTmp, &len );
	if( 0 < len)
	{
		strcpy((char*)pTagStru->strRegNo,szTmp);
		delete [] szTmp;
	}

	return 0;
}
INT CRFID_Interface::ConvertEAN13(CString strOldISBN, CString &strNewISNB)
{
	if( 10 != strOldISBN.GetLength() )	return -1;

	strOldISBN = _T("978") + strOldISBN;

	if( _T("89") != strOldISBN.Mid(3,2) )	return -1;

	INT		nCheckSum = 0;
	TCHAR   szTemp[1];
	TCHAR	szReturnValue[1];
	INT		nCheckRest;

	for( INT i=0; i<12; i++ )
	{
		szTemp[0] = strOldISBN.GetAt( i );

		if( i%2 )	nCheckSum += _ttoi(szTemp) * 3;
		else		nCheckSum += _ttoi(szTemp) * 1;
	}

	nCheckRest = nCheckSum%10;
	if( nCheckRest != 0 )
	{
		_stprintf( szReturnValue, _T("%d"), (10-nCheckRest) );				
	}
	else
	{
		_stprintf( szReturnValue, _T("%d"), nCheckRest );
	}
	
	strNewISNB.Format( _T("%s%s"), strOldISBN.Mid(0,12), szReturnValue );

	return 0;
}

INT CRFID_Interface::MakeCallCode( CString SEPARATE_SHELF_CODE, CString CLASS_NO, 
								   CString BOOK_CODE,           CString VOL_CODE, 
								   CString COPY_CODE,           CString& CALL_CODE )
{
	const INT nCnt = 5;
	CString   strShelfCode;		
	CString   sTmpData[nCnt];
	CString   sTmpCode;

	sTmpData[0] = SEPARATE_SHELF_CODE;
	sTmpData[1] = CLASS_NO;
	sTmpData[2] = BOOK_CODE;
	sTmpData[3] = VOL_CODE;
	sTmpData[4] = COPY_CODE;

	for( INT j = 0;j < nCnt;j++ )
	{
		sTmpCode = sTmpData[j];
		sTmpCode.TrimLeft(); sTmpCode.TrimRight();
		
		if( j == 0 && !sTmpCode.IsEmpty() && !m_pParentMgr )
		{
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호") , sTmpCode , sTmpCode );
		}
		
		if( !sTmpCode.IsEmpty() ) 
		{
			if( CALL_CODE.GetLength() > 1 )
			{
				if( j == 1 )
					CALL_CODE += _T(" ");
				else if( j == 4 )
					CALL_CODE += _T("=");
				else
					CALL_CODE += _T("-");
			}
			
			CALL_CODE += sTmpCode;
		}
	}

	return 0;
}

INT CRFID_Interface::MakeRegSeqNo(CString strRegNo, CString &strRegSeqNo)
{
	if ( 12 != strRegNo.GetLength() )
		return -1;
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	strRegSeqNo = strRegNo.Mid(nRegCodeLength);
	
	return 0;
}

INT CRFID_Interface::MakeRegSeqNo(CString strRegNo, UCHAR* pszRegSeqNo)
{
	CString strRegSeqNo;

	if ( 12 != strRegNo.GetLength() )
		return -1;
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	strRegSeqNo = strRegNo.Mid(nRegCodeLength);
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strRegSeqNo.GetBuffer(0), &szTmp, &len );	
	if( 0 < len)
	{
		strcpy((char*)pszRegSeqNo,szTmp);
		delete [] szTmp;	
	}
	
	return 0;
}

INT CRFID_Interface::ShowErrorMsg(CString strFuncName, INT nPosition, INT nCode,CString strErrorMsg)
{
	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_DEBUG"));
	if ( bFind == FALSE ) return nCode;

	CString strMessage;
	strMessage.Format( _T("Func[ %s ] ERROR!!\n")
					   _T("POSITION : %d\n")
					   _T("CODE     : %d\n")
					   _T("MSG      : %s\n"), strFuncName, nPosition, nCode,strErrorMsg );
	AfxMessageBox( strMessage );
	return nCode;
}
INT CRFID_Interface::ShowErrorMsg(CString strFuncName, INT nPosition, INT nCode)
{
	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_DEBUG"));
	if ( bFind == FALSE ) return nCode;

	CString strMessage;
	strMessage.Format(	_T("Func[ %s ] ERROR!!\n")	
						_T("POSITION : %d\n")
						_T("CODE     : %d\n"), strFuncName, nPosition, nCode);
	AfxMessageBox( strMessage );
	return nCode;
}
INT CRFID_Interface::SetRFIDMapNSecurity(TCHAR* strMap, TCHAR* strSecType, INT nFirst, INT nSecond)
{
	return 0;
}


