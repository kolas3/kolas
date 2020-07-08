// LibFileManager.cpp: implementation of the CLibFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LibFileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLibFileManager::CLibFileManager()
{	
	m_libHolidayFilePath.Format( _T("..\\CFG\\휴관일.txt") );
}

CLibFileManager::~CLibFileManager()
{

}

INT CLibFileManager::MakeLibHoliDayInfoFile( CStringList &year, CStringList &mon, CStringList &day )
{
	CString output;
	
	FILE *file;
	file = _tfopen( m_libHolidayFilePath, _T("wb") );
	if( !file )
		return -1;
	fseek(file, 0, SEEK_END);		
	if( 0 == ftell(file) )
	{
		/*UNCHANGE*/fputc(0xFF, file);
		/*UNCHANGE*/fputc(0xFE, file);
	}
	
	POSITION yPos = year.GetHeadPosition();
	POSITION mPos =  mon.GetHeadPosition();
	POSITION dPos =  day.GetHeadPosition();
	
	year.GetCount();
	while( yPos )
	{
		output.Format( _T("%s/%s/%s\r\n"), year.GetNext( yPos ), mon.GetNext( mPos ), day.GetNext( dPos ) );
		_ftprintf( file, output );
	}

	fclose( file );
	
	return 0;
}

INT CLibFileManager::GetLibHoliDayInfo( CStringList &year, CStringList &mon, CStringList &day )
{
	CStdioFile fp;
	fp.Open( m_libHolidayFilePath, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( !fp )
		return -1;
		
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}

	CString output, y, m, d;
	INT locate = 0;
	while( fp.ReadString( output ) )
	{

		locate = output.Find( _T("/"), 0 );
		y = output.Left( locate );
		output.Replace( y + _T("/"), _T("") );

		locate = output.Find( _T("/"), 0 );
		m = output.Left( locate );
		output.Replace( m + _T("/"), _T("") );

		d = output;
			
		year.AddTail( y );
		mon.AddTail(  m );
		day.AddTail(  d );
	}
	
	fp.Close();

	return 0;
}

INT CLibFileManager::SaveDataToFile(CESL_DataMgr *pDM, CString strUSER_ID)
{
	if (pDM == NULL) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString strQuery;
	strQuery = _T("SELECT YEAR, MONTH, DAYS FROM MN_LIB_HOLIDAY_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");
//	TCHAR cUseMANAGE_CODE;
//	CString strTmp, strQuery;
//	strTmp = _T("");
//	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='관리구분별도서관설정'"));
//	dm.GetOneValueQuery(strQuery, strTmp);
//	strTmp.TrimLeft();				strTmp.TrimRight();
//	strTmp.MakeUpper();
//	if (strTmp == _T("N")) cUseMANAGE_CODE = 'N';
//		else cUseMANAGE_CODE = 'Y';
//	
//	// 관리구분별도서관설정 이 _T("Y") 인 경우는 사용자의 관리구분을 읽어온다
//	CString strMANAGE_CODE;
//	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), strUSER_ID);
//	dm.GetOneValueQuery(strQuery, strMANAGE_CODE);
//	strMANAGE_CODE.TrimLeft();				strMANAGE_CODE.TrimRight();
//
//	// 이제 드디어 데이타를 읽어오자.
//	if (cUseMANAGE_CODE == 'N') {
//		strQuery.Format(_T("SELECT YEAR, MONTH, DAYS FROM MN_LIB_HOLIDAY_TBL WHERE MANAGE_CODE IS NULL"));
//	} else {
//		if (strMANAGE_CODE.GetLength() == 0) {
//			strQuery.Format(_T("SELECT YEAR, MONTH, DAYS FROM MN_LIB_HOLIDAY_TBL WHERE MANAGE_CODE IS NULL"));
//		} else {
//			strQuery.Format(_T("SELECT YEAR, MONTH, DAYS FROM MN_LIB_HOLIDAY_TBL WHERE MANAGE_CODE='%s'"), strMANAGE_CODE);
//		}
//	}
	//--2008.10.15 UPDATE BY PWR --}}

	// 2009.11.19 UPDATE BY PWR : 실패시 파일에 저장되지 않도록 한다.
	INT ids;
	ids = dm.RestructDataManager(strQuery);
	if(0 > ids)
	{
		return -1;
	}
// 	dm.RestructDataManager(strQuery);

	CString strYEAR, strMONTH, strDAYS;
	CStringList year, mon, day;
	for( INT i = 0; i < dm.GetRowCount(); i++ )
	{
		dm.GetCellData(i, 0, strYEAR);
		dm.GetCellData(i, 1, strMONTH);
		dm.GetCellData(i, 2, strDAYS);
		
		year.AddTail( strYEAR );
		mon.AddTail(  strMONTH );
		day.AddTail(  strDAYS );
	}

	CLibFileManager fileMgr;
	fileMgr.MakeLibHoliDayInfoFile( year, mon, day );

	return 0;
}
