/**************************************************************************/
// Sisac.cpp -- managing SISAC BARCODE created by SICI
//
//         Park Daewoo & Son Suhyong
//         ECO KOLIS Develope Team
//         Copyright (c) 2007.12 ECO Corporation
//
/**************************************************************************/
#include    "stdafx.h"
#include    "sisac.h"
#include	"SisacInfo.h"

/**************************************************************************/
//    Contructor
/**************************************************************************/
CSisac::CSisac() : CDialog()
{
}

/**************************************************************************/
//    Destructor
/**************************************************************************/
CSisac::~CSisac()
{
}

/**************************************************************************/
// Get SISAC Barcode Infomation
// 1. Get ISSN
// 2. Get Publish Date
// 3. Get Enumeration
/**************************************************************************/
INT CSisac::GetSisacBarcodeInfo( TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo )
{
	INT		ids;

	m_BarcodeIndex = 0;
	// ================================================
	// 1. Stru Init...
	// ================================================
	ids = BarcodeInfoInit( fo_sBarcodeInfo );
	if ( ids )
		return ids;
	// ================================================
	// 2. Get ISSN
	// ================================================
	ids = GetISSNInfo( fi_szSisacBarcode, fo_sBarcodeInfo );
	if ( ids )
		return ids;
	// ================================================
	// 3. Get Publish Date
	// ================================================
	ids = GetPublishInfo( fi_szSisacBarcode, fo_sBarcodeInfo );
	if ( ids )
		return ids;

 	// ================================================
	// 4. Get Enumeration Data
	// ================================================
	ids = GetEnumInfo( fi_szSisacBarcode, fo_sBarcodeInfo );
	if ( ids )
		return ids;

	return SISAC_SUCCESS;
}

INT CSisac::BarcodeInfoInit(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	// 1.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return STRU_INIT_ERROR;
	
	// 2.Init...
	// 2-1.ISSN
	memset( fo_sBarcodeInfo->szISSN, 0x00, sizeof(fo_sBarcodeInfo->szISSN) );
	// 2-2.PubInfo
	fo_sBarcodeInfo->sPub_Info.cExist_yn = _T('N');
	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('N');
	fo_sBarcodeInfo->sPub_Info.cRange_yn = _T('N');
	memset( fo_sBarcodeInfo->sPub_Info.szPub_code, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.szPub_code) );
	memset( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szYear, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.sStart_Pub.szYear) );
	memset( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth) );
	memset( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay) );
	memset( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear) );
	memset( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth) );
	memset( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szDay, 0x00, sizeof(fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szDay) );
	// 2-3.Enum
	fo_sBarcodeInfo->sEnum_Info.cExist_yn = _T('N');
	fo_sBarcodeInfo->sEnum_Info.cMerged_yn = _T('N');
	memset( fo_sBarcodeInfo->sEnum_Info.szEnum_code, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.szEnum_code) );
	memset( fo_sBarcodeInfo->sEnum_Info.szEnum_format, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.szEnum_format) );
	memset( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szVol, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szVol) );
	memset( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szNum, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szNum) );
	memset( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szPart, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szPart) );
	memset( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szSec, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szSec) );
	memset( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szVol, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szVol) );
	memset( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szNum, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szNum) );
	memset( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szPart, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szPart) );
	memset( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szSec, 0x00, sizeof(fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szSec) );
		
	return SISAC_SUCCESS;
}

INT CSisac::GetISSNInfo( TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo )
{
	TCHAR szISSNTemp[16];

	// 1.Data Check...
	if ( fi_szSisacBarcode == NULL )
		return GET_ISSN_ERROR;
	if ( fo_sBarcodeInfo == NULL )
		return GET_ISSN_ERROR;

	// 2.Buffer Init...
	memset( szISSNTemp, 0x00, sizeof( szISSNTemp ) );

	// 3.Get ISSN
	_tcsncpy( szISSNTemp, fi_szSisacBarcode, ISSN_LEN );
	_tcsncpy( fo_sBarcodeInfo->szISSN, szISSNTemp, ISSN_LEN + 1 );

	m_BarcodeIndex += ISSN_LEN;

	return SISAC_SUCCESS;
}

INT CSisac::GetPublishInfo(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	INT		ids;
	INT		iPubCodeIdx;

	// 0.Data Check...
	if ( fi_szSisacBarcode == NULL )
		return GET_PUBINFO_ERROR;
	if ( fo_sBarcodeInfo == NULL )
		return GET_PUBINFO_ERROR;

	// 1.Get Pub Code & Code Index
	ids = GetPubCode( fi_szSisacBarcode, fo_sBarcodeInfo, &iPubCodeIdx );
	if ( ids )
		return ids;

	// 2.Get Pub Exist Y/N
	ids = GetPubExist( fo_sBarcodeInfo );
	if ( ids )
		return ids;

	// 3.Get Pub Range Y/N
	ids = GetPubRange( fo_sBarcodeInfo, iPubCodeIdx );
	if ( ids )
		return ids;
	
	// 4.Get Pun StartDate
	if ( fo_sBarcodeInfo->sPub_Info.cExist_yn == _T('Y') )
	{
		ids = GetPubStartDate( fi_szSisacBarcode, fo_sBarcodeInfo, iPubCodeIdx );
		if ( ids )
			return ids;
	}

	// 5.Get Pun EndDate
	if ( fo_sBarcodeInfo->sPub_Info.cRange_yn == _T('Y') )
	{
		ids = GetPubEndDate( fi_szSisacBarcode, fo_sBarcodeInfo, iPubCodeIdx );
		if ( ids )
			return ids;
	}
	
	// 6.Get Pub Type
	if ( fo_sBarcodeInfo->sPub_Info.cExist_yn == _T('Y') )
	{
		ids = GetPubType( fo_sBarcodeInfo, iPubCodeIdx );
		if ( ids )
			return ids;
	}

	return SISAC_SUCCESS;
}

INT	CSisac::GetPubCode(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo, INT *fo_index)
{
	INT		i;
	TCHAR	szPubCodeTemp[8];

	// 0.Data Check...
	if ( fi_szSisacBarcode == NULL )
		return GET_PUB_CODE_ERROR;
	if ( fo_sBarcodeInfo == NULL )
		return GET_PUB_CODE_ERROR;

	// 1.Buffer Init...
	memset( szPubCodeTemp, 0x00, sizeof(szPubCodeTemp) );

	// 2.Get Pub Code
	_tcsncpy( szPubCodeTemp, &fi_szSisacBarcode[ISSN_LEN], PUB_CODE_LEN );

	// 3.Date Code Error check
	if ( _ttoi( szPubCodeTemp ) < 900 || _ttoi( szPubCodeTemp ) > 999)		// if PubCode (900 ~ 949) + 50,  else Error
		return BAR_GET_PUN_CODE_ERROR;

	// 4.Set Pub Code
	_tcsncpy( fo_sBarcodeInfo->sPub_Info.szPub_code, szPubCodeTemp, ISSN_LEN + 1 );

	// 5.Get Pub Code Format index	
	if ( _ttoi( szPubCodeTemp ) > 950 )										// 950~999 -> 900~949
		_itot( ( _ttoi( szPubCodeTemp ) - 50 ), szPubCodeTemp, 10 );

	*fo_index = -1;

	for ( i = 0; i < PUB_FORMAT_CODE_CNT; i++ ) 
	{
		if ( _tcsncmp( szDATE_FORMAT_CODE[i].szCODE, szPubCodeTemp , PUB_CODE_LEN ) == 0 )
		{
			*fo_index = i;
			break;
		}
	}
	if ( *fo_index == -1 )	// Error NonMatch PubCode
		return GET_PUB_CODE_ERROR;

	m_BarcodeIndex += PUB_CODE_LEN;

	return SISAC_SUCCESS;
}

INT	CSisac::GetPubExist(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_PUB_EXIST_ERROR;
	
	// 1.Get Pub Exist check
	// if PuCode(900, 950) then No Publish
	if ( _ttoi( fo_sBarcodeInfo->sPub_Info.szPub_code ) == 900 ||
		 _ttoi( fo_sBarcodeInfo->sPub_Info.szPub_code ) == 950 )
	{
		fo_sBarcodeInfo->sPub_Info.cExist_yn = _T('N');
	}
	else
	{
		fo_sBarcodeInfo->sPub_Info.cExist_yn = _T('Y');
	}

	return SISAC_SUCCESS;
}

INT CSisac::GetPubRange(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo, INT iPubCodeIdx)
{
	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_PUB_RANGE_ERROR;

	// 1.Get Pub Range Y/N
	if ( _tcsncmp( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT2, _T("None"), 4 ) == 0 )
	{
		fo_sBarcodeInfo->sPub_Info.cRange_yn = _T('N');
	}
	else 
	{
		fo_sBarcodeInfo->sPub_Info.cRange_yn = _T('Y');
	}

	return SISAC_SUCCESS;
}

INT CSisac::GetPubStartDate(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo, INT iPubCodeIdx)
{
	INT		i;

	INT		nPubDateLen;
	INT		nPubYearLen;
	INT		nPubMonthLen;
	INT		nPubDayLen;

	TCHAR	szPubDateTemp[16];
	TCHAR	szPubYearTemp[8];
	TCHAR	szPubMonthTemp[8];
	TCHAR	szPubDayTemp[8];

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_PUB_STRARTDATE_ERROR;
	if ( fi_szSisacBarcode == NULL )
		return GET_PUB_STRARTDATE_ERROR;

	// 1.Buffer Init...
	memset( szPubDateTemp, 0x00, sizeof(szPubDateTemp) );
	memset( szPubYearTemp, 0x00, sizeof(szPubYearTemp) );
	memset( szPubMonthTemp, 0x00, sizeof(szPubMonthTemp) );
	memset( szPubDayTemp, 0x00, sizeof(szPubDayTemp) );

	nPubDateLen = nPubYearLen = nPubMonthLen = nPubDayLen = 0;

	// 2.Get Pub Start Date
	nPubDateLen = _tcsclen( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1 );
	_tcsncpy( szPubDateTemp, &fi_szSisacBarcode[m_BarcodeIndex], nPubDateLen );
	
	for ( i = 0; i < nPubDateLen; i++ )
	{
		if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1[i] == _T('y') )
			nPubYearLen++;
		else if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1[i] == _T('m') )
			nPubMonthLen++;
		else if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1[i] == _T('d') )
			nPubDayLen++;
	}

	// 3.Get Pub Year : case (4)
	_tcsncpy( szPubYearTemp, &szPubDateTemp[0], nPubYearLen);
	_tcsncpy( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szYear, szPubYearTemp, nPubYearLen + 1);

	// 4.Get Pub Month : case (0,1,2)
	_tcsncpy( szPubMonthTemp, &szPubDateTemp[nPubYearLen], nPubMonthLen);
	if ( nPubMonthLen == 1 )
		_stprintf( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, _T("0%s"), szPubMonthTemp );
	else
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, szPubMonthTemp, nPubMonthLen + 1);

	// Month Erroe Check...
	if ( nPubMonthLen == 2 )
	{
		if ( _ttoi( szPubMonthTemp ) >= 10 && _ttoi( szPubMonthTemp ) <= 12 ||		// Month
			 _ttoi( szPubMonthTemp ) >= 21 && _ttoi( szPubMonthTemp ) <= 24 ||		// Season
			 _ttoi( szPubMonthTemp ) >= 31 && _ttoi( szPubMonthTemp ) <= 34 )		// Quarter
		{ ; }
		else
			return BAR_GET_PUB_STRARTDATE_ERROR;
	}

	// 5.Get Pub Day : case (0,1,2)
	_tcsncpy( szPubDayTemp, &szPubDateTemp[nPubYearLen + nPubMonthLen], nPubDayLen);
	if ( nPubDayLen == 1 )
		_stprintf( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay, _T("0%s"), szPubDayTemp );
	else
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay, szPubDayTemp, nPubDayLen + 1);

	m_BarcodeIndex += nPubDateLen;

	return SISAC_SUCCESS;
}

INT CSisac::GetPubEndDate(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo, INT iPubCodeIdx)
{
	INT		i;

	INT		nPubDateLen;
	INT		nPubYearLen;
	INT		nPubMonthLen;
	INT		nPubDayLen;

	TCHAR	szPubDateTemp[16];
	TCHAR	szPubYearTemp[8];
	TCHAR	szPubMonthTemp[8];
	TCHAR	szPubDayTemp[8];

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_PUB_ENDDATE_ERROR;
	if ( fi_szSisacBarcode == NULL )
		return GET_PUB_ENDDATE_ERROR;

	// 1.Buffer Init...
	memset( szPubDateTemp, 0x00, sizeof(szPubDateTemp) );
	memset( szPubYearTemp, 0x00, sizeof(szPubYearTemp) );
	memset( szPubMonthTemp, 0x00, sizeof(szPubMonthTemp) );
	memset( szPubDayTemp, 0x00, sizeof(szPubDayTemp) );

	nPubDateLen = nPubYearLen = nPubMonthLen = nPubDayLen = 0;

	// 2.Get Pub EndDate
	nPubDateLen = _tcsclen( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT2 );
	_tcsncpy( szPubDateTemp, &fi_szSisacBarcode[m_BarcodeIndex], nPubDateLen );
		
	for ( i = 0; i < nPubDateLen; i++ ) 
	{
		if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT2[i] == _T('y') )
			nPubYearLen++;
		else if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT2[i] == _T('m') )
			nPubMonthLen++;
		else if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT2[i] == _T('d') )
			nPubDayLen++;
	}

	// 3.Get Pub Year : case (0,4)
	if ( nPubYearLen == 0 )
	{
		_tcsncpy( szPubYearTemp, fo_sBarcodeInfo->sPub_Info.sStart_Pub.szYear,
								_tcsclen( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szYear ) );
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear, szPubYearTemp, 
								_tcsclen( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szYear ) + 1 );
	}
	else if ( nPubYearLen == 4 )
	{
		_tcsncpy( szPubYearTemp, &szPubDateTemp[0], nPubYearLen );
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szYear, szPubYearTemp, nPubYearLen + 1 );
	}

	// 4.Get Pub Month : case (0,1,2)
	if ( nPubMonthLen == 0 )
	{
		_tcsncpy( szPubMonthTemp, fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth, 
								_tcsclen( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth ) );
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, szPubMonthTemp, 
								_tcsclen( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth ) + 1 );
	}
	else if ( nPubMonthLen == 1 )
	{
		_tcsncpy( szPubMonthTemp, &szPubDateTemp[nPubYearLen], nPubMonthLen);
		_stprintf( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, _T("0%s"), szPubMonthTemp );
	}
	else if ( nPubMonthLen == 2 )
	{
		_tcsncpy( szPubMonthTemp, &szPubDateTemp[nPubYearLen], nPubMonthLen);
		
		// Month Erroe Check...
		if ( _ttoi( szPubMonthTemp ) >= 10 && _ttoi( szPubMonthTemp ) <= 12 ||		// Month
			 _ttoi( szPubMonthTemp ) >= 21 && _ttoi( szPubMonthTemp ) <= 24 ||		// Season
			 _ttoi( szPubMonthTemp ) >= 31 && _ttoi( szPubMonthTemp ) <= 34 )		// Quarter
		{ ; }
		else
			return BAR_GET_PUB_ENDDATE_ERROR;

		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szMonth, szPubMonthTemp, nPubMonthLen + 1 );
	}

	// 5.Get Pub Day : case (0,1,2)
	if ( nPubDayLen == 0 )
	{
		_tcsncpy( szPubDayTemp, fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay, 
								_tcsclen( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay ) );
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szDay, szPubDayTemp, 
								_tcsclen( fo_sBarcodeInfo->sPub_Info.sStart_Pub.szDay ) + 1 );
	}
	else if ( nPubDayLen == 1 )
	{
		_tcsncpy( szPubDayTemp, &szPubDateTemp[nPubYearLen + nPubMonthLen], nPubDayLen );
		_stprintf( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szDay, _T("0%s"), szPubDayTemp );
	}
	else if ( nPubDayLen  == 2 )
	{
		_tcsncpy( szPubDayTemp, &szPubDateTemp[nPubYearLen + nPubMonthLen], nPubDayLen );
		_tcsncpy( fo_sBarcodeInfo->sPub_Info.sEnd_Pub.szDay, szPubDayTemp, nPubDayLen + 1);
	}
	
	m_BarcodeIndex += nPubDateLen;

	return SISAC_SUCCESS;
}

INT CSisac::GetPubType(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo, INT iPubCodeIdx)
{
	INT		i;

	INT		nPucFormatLen;
	INT		nPubYearLen;
	INT		nPubMonthLen;
	INT		nPubDayLen;

	TCHAR	szPubMonthTemp[8];

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return -GET_PUB_TYPE_ERROR;

	// 1.Buffer Init...
	memset( szPubMonthTemp, 0x00, sizeof(szPubMonthTemp) );

	nPucFormatLen = nPubYearLen = nPubMonthLen = nPubDayLen = 0;
	
	// 2.Get Pub Format Length
	nPucFormatLen = _tcsclen( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1 );
		
	for ( i = 0; i < nPucFormatLen; i++ ) 
	{
		if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1[i] == _T('y') )
			nPubYearLen++;
		else if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1[i] == _T('m') )
			nPubMonthLen++;
		else if ( szDATE_FORMAT_CODE[iPubCodeIdx].szFORMAT1[i] == _T('d') )
			nPubDayLen++;
	}

	// 3.Get Pub Type
	if ( nPubMonthLen == 2 && nPubDayLen == 0 )
	{
		_tcsncpy( szPubMonthTemp, fo_sBarcodeInfo->sPub_Info.sStart_Pub.szMonth , nPubMonthLen );

		if ( _ttoi( szPubMonthTemp ) >= 21 &&  _ttoi( szPubMonthTemp ) <= 24 )	// Season (21~24)
			fo_sBarcodeInfo->sPub_Info.cPub_type = _T('S');
		else if ( _ttoi( szPubMonthTemp ) >= 31 &&  _ttoi( szPubMonthTemp ) <= 34 )	// Quarter (31~34)
			fo_sBarcodeInfo->sPub_Info.cPub_type = _T('Q');
		else
			fo_sBarcodeInfo->sPub_Info.cPub_type = _T('M');
	}
	else if ( nPubMonthLen == 0 && nPubDayLen == 0 )	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('Y');
	else if ( nPubMonthLen == 1 && nPubDayLen == 0 )	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('M');
	else if ( nPubMonthLen == 1 && nPubDayLen == 1 )	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('D');
	else if ( nPubMonthLen == 1 && nPubDayLen == 2 )	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('D');
	else if ( nPubMonthLen == 2 && nPubDayLen == 1 )	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('D');
	else if ( nPubMonthLen == 2 && nPubDayLen == 2 )	fo_sBarcodeInfo->sPub_Info.cPub_type = _T('D');

	return SISAC_SUCCESS;
}

INT CSisac::GetEnumInfo(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	INT		ids;

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_ENUMINFO_ERROR;
	// 0.Data Check...
	if ( fi_szSisacBarcode == NULL )
		return GET_ENUMINFO_ERROR;

	// 1.Get Enum Exist Y/N
	ids = GetEnumExist( fo_sBarcodeInfo );
	if ( ids )
		return ids;

	// 2.Get Enum Code
	if ( fo_sBarcodeInfo->sEnum_Info.cExist_yn == _T('Y') )
	{
		ids = GetEnumCode( fi_szSisacBarcode, fo_sBarcodeInfo );
		if ( ids )
			return ids;
	}

	// 3.Get Enum Format
	if ( fo_sBarcodeInfo->sEnum_Info.cExist_yn == _T('Y') )
	{
		ids = GetEnumFormat( fo_sBarcodeInfo );
		if ( ids )
			return ids;
	}

	// 4.Get Enum Merge Y/N
	if ( fo_sBarcodeInfo->sEnum_Info.cExist_yn == _T('Y') )
	{
		ids = GetEnumMerge( fo_sBarcodeInfo );
		if ( ids )
			return ids;
	}
	
	// 5.Get Start Enum & Get End Enum
	if ( fo_sBarcodeInfo->sEnum_Info.cExist_yn == _T('Y') )
	{
		ids = GetStartEndEnum( fi_szSisacBarcode, fo_sBarcodeInfo );
		if ( ids )
			return ids;
	}

	return SISAC_SUCCESS;
}

INT CSisac::GetEnumExist(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_ENUM_EXIST_ERROR;

	// 1.Get Enum Exist check
	// if PubCode(950~999) then No Enum
	if ( _ttoi( fo_sBarcodeInfo->sPub_Info.szPub_code ) >= 950 &&
		 _ttoi( fo_sBarcodeInfo->sPub_Info.szPub_code ) <= 999 )
	{
		fo_sBarcodeInfo->sEnum_Info.cExist_yn = _T('N');
	}
	else if ( _ttoi( fo_sBarcodeInfo->sPub_Info.szPub_code ) >= 900 &&
			  _ttoi( fo_sBarcodeInfo->sPub_Info.szPub_code ) <= 949 )
	{
		fo_sBarcodeInfo->sEnum_Info.cExist_yn = _T('Y');
	}

	return SISAC_SUCCESS;
}

INT CSisac::GetEnumCode(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	TCHAR	szEnumCodeTemp[8];
	
	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_ENMU_CODE_ERROR;
	if ( fi_szSisacBarcode == NULL )
		return GET_ENMU_CODE_ERROR;

	// 1.Buffer Init...
	memset( szEnumCodeTemp, 0x00, sizeof(szEnumCodeTemp) );

	// 2.Get Enum Code
	_tcsncpy( szEnumCodeTemp, &fi_szSisacBarcode[m_BarcodeIndex], ENUM_CODE_LEN );
	_tcsncpy( fo_sBarcodeInfo->sEnum_Info.szEnum_code, szEnumCodeTemp, ENUM_CODE_LEN + 1 );

	m_BarcodeIndex += ENUM_CODE_LEN;

	return SISAC_SUCCESS;
}

INT CSisac::GetEnumFormat(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	INT		nTemp1, nTemp2;
	INT		iEnumFormatX;

	TCHAR	szEnumCodeTemp[8];
	TCHAR	szEnumFormatTemp[16];

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_ENUM_FORMAT_ERROR;

	// 1.Buffer Init...
	memset( szEnumCodeTemp, 0x00, sizeof(szEnumCodeTemp) );
	memset( szEnumFormatTemp, 0x00, sizeof(szEnumFormatTemp) );

	// 2.Get Enum Format Rule
	_tcsncpy( szEnumCodeTemp, fo_sBarcodeInfo->sEnum_Info.szEnum_code, ENUM_CODE_LEN );

	for ( iEnumFormatX = 0; iEnumFormatX < ENUM_FORMAT_RULE_CNT; iEnumFormatX++ )
	{
		if ( _ttoi( szEnumCodeTemp ) >= szENUM_FORMAT_RULE_TYPE[iEnumFormatX].nEnumCodeMIN &&
			 _ttoi( szEnumCodeTemp ) <= szENUM_FORMAT_RULE_TYPE[iEnumFormatX].nEnumCodeMAX )
		{
			break;
		}
	}
	
	nTemp1 = szENUM_FORMAT_RULE_TYPE[iEnumFormatX].nEnumSub;
	nTemp2 = _ttoi( szEnumCodeTemp );

	_tcsncpy( szEnumFormatTemp, szENUM_FORMAT_CODE[nTemp2 - nTemp1 - 1].szFORMAT, ENUM_FORM_LEN );
	
	// 3.Get Enum Format
	szEnumFormatTemp[0] = szENUM_FORMAT_RULE_TYPE[iEnumFormatX].cEnumFieldX;

	if ( szEnumFormatTemp[4] == _T('X') )
		szEnumFormatTemp[4] = szENUM_FORMAT_RULE_TYPE[iEnumFormatX].cEnumFieldX;
	else if ( szEnumFormatTemp[4] == _T('Y') )
		szEnumFormatTemp[4] = szENUM_FORMAT_RULE_TYPE[iEnumFormatX].cEnumFieldY;

	_tcsncpy( fo_sBarcodeInfo->sEnum_Info.szEnum_format, szEnumFormatTemp, ENUM_FORM_LEN + 1 );

	return SISAC_SUCCESS;
}

INT CSisac::GetEnumMerge(SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	INT		i;

	TCHAR	szEnumFormatTemp[16];

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_ENUM_MERGE_ERROR;

	// 1.Buffer Init...
	memset( szEnumFormatTemp, 0x00, sizeof(szEnumFormatTemp) );

	// 2.Get Enum Format
	_tcsncpy( szEnumFormatTemp, fo_sBarcodeInfo->sEnum_Info.szEnum_format, ENUM_FORM_LEN );

	// 3.Get Enum Merge
	fo_sBarcodeInfo->sEnum_Info.cMerged_yn = _T('N');

	for ( i = 4; i < ENUM_FORM_LEN; i++ )
	{
		if ( szEnumFormatTemp[i] != _T('0') )
			fo_sBarcodeInfo->sEnum_Info.cMerged_yn = _T('Y');
	}

	return SISAC_SUCCESS;
}

INT CSisac::GetStartEndEnum(TCHAR *fi_szSisacBarcode, SISAC_BARCODE_INFO_TYPE *fo_sBarcodeInfo)
{
	INT		i;

	INT		nEnumLen;

	TCHAR	szEnumTemp[16];
	TCHAR	szEnumFormatTemp[16];

	// 0.Data Check...
	if ( fo_sBarcodeInfo == NULL )
		return GET_SEENUM_ERROR;
	if ( fi_szSisacBarcode == NULL )
		return GET_SEENUM_ERROR;

	// 1.Buffer Init...
	memset( szEnumTemp, 0x00, sizeof(szEnumTemp) );
	memset( szEnumFormatTemp, 0x00, sizeof(szEnumFormatTemp) );

	nEnumLen = 0;

	// 2.Get Enum Format
	_tcsncpy( szEnumFormatTemp, fo_sBarcodeInfo->sEnum_Info.szEnum_format, ENUM_FORM_LEN );

	// 3.Get Enum
	for ( i = 0; i < ENUM_FORM_LEN; i++ )
	{
		nEnumLen = szEnumFormatTemp[i] - _T('0');
		_tcsncpy( szEnumTemp, &fi_szSisacBarcode[ m_BarcodeIndex ], nEnumLen );

		if ( i == 0 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szVol, szEnumTemp, nEnumLen + 1 );
		else if ( i == 1 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szNum, szEnumTemp, nEnumLen + 1 );
		else if ( i == 2 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szPart, szEnumTemp, nEnumLen + 1 );
		else if ( i == 3 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sStart_Enum.szSec, szEnumTemp, nEnumLen + 1 );
		else if ( i == 4 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szVol, szEnumTemp, nEnumLen + 1 );
		else if ( i == 5 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szNum, szEnumTemp, nEnumLen + 1 );
		else if ( i == 6 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szPart, szEnumTemp, nEnumLen + 1 );
		else if ( i == 7 )
			_tcsncpy( fo_sBarcodeInfo->sEnum_Info.sEnd_Enum.szSec, szEnumTemp, nEnumLen + 1 );

		memset( szEnumTemp, 0x00, sizeof(szEnumTemp) );
		m_BarcodeIndex += nEnumLen;
		
		if ( i == 3 )
			if ( fo_sBarcodeInfo->sEnum_Info.cMerged_yn == _T('N') )
				break;
	}

	return SISAC_SUCCESS;
}