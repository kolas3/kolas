// BO_MARCIMPORT_TOOL.cpp: implementation of the BO_MARCIMPORT_TOOL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\include\공동목록\BO_MARCIMPORT_TOOL.h"
#include "direct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


/// MARC의 Delimiter




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BO_MARCIMPORT_TOOL::BO_MARCIMPORT_TOOL()
{

}

BO_MARCIMPORT_TOOL::~BO_MARCIMPORT_TOOL()
{

}

//----------------------------------------------------------------------------------
//  파일 선택
//  인자 : fi_bOpenFileDialog : TRUE  : 읽기 전용
//                              FALSE : 쓰기 전용
//----------------------------------------------------------------------------------
INT BO_MARCIMPORT_TOOL::FileSelect( BOOL    fi_bOpenFileDialog  ,
								    TCHAR  * fo_sSelectFileName  )
{
	TCHAR    sCurrentDirectory[_MAX_PATH];
	TCHAR    sExtension[16];
    CString sFileName;

	//---------------------------------------------------------------------
	//  현 디렉토리를 저장한다.
    _tgetcwd( sCurrentDirectory, _MAX_PATH ) ;

	//---------------------------------------------------------------------
	//  파일 선택
	if ( fi_bOpenFileDialog == TRUE ) // FILE OPEN
	{
		_tcscpy ( sExtension , _T("*.*") );
	}
	else
	{
		_tcscpy ( sExtension , _T("") );
	}

	CFileDialog dlg( fi_bOpenFileDialog, sExtension, sExtension ,
		            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,
//			        _T("파일형식(*.*)|All Files(*.*)|*.*||") );
			        sExtension );

	//---------------------------------------------------------------------
	// 원래 디렉토리로 복귀

	if ( IDOK != dlg.DoModal() ) 
	{
		_tchdir( sCurrentDirectory ) ;
		return -1;
	}
	_tchdir( sCurrentDirectory ) ;

	//---------------------------------------------------------------------
    // 파일명이 선택되지않고 '확인'을 click했을 경우.
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();

	_stprintf ( fo_sSelectFileName , sFileName.GetBuffer(0) );

	return 0;
}

/*========================================================================*/
/*  자료구분에 합당한 등록구분인지를 확인한다.
/*========================================================================*/
INT BO_MARCIMPORT_TOOL::Check_MatClassRegClass ( TCHAR * fi_sControlNo , 
							                     TCHAR * fi_sRegClass )
{
	//  비도서
	if ( !_tcsnicmp ( fi_sControlNo+1 , _T("VM") , 2 ) ||
	     !_tcsnicmp ( fi_sControlNo+1 , _T("MU") , 2 ) ||
	     !_tcsnicmp ( fi_sControlNo+1 , _T("CF") , 2 ) ||
	     !_tcsnicmp ( fi_sControlNo+1 , _T("MP") , 2 ) )
	{
		if ( _tcscmp ( fi_sRegClass , _T("AV") ) )
			return -1;
	}
	else if (  !_tcsnicmp ( fi_sControlNo+1 , _T("OL") , 2 ) )
	{
		if ( !_tcscmp ( fi_sRegClass , _T("OL") ) ||
			 !_tcscmp ( fi_sRegClass , _T("OK") ) )
			return 0 ;
		else
			return -1;
	}
	else
	{
		if ( !_tcscmp ( fi_sRegClass , _T("AV") ) ||
			 !_tcscmp ( fi_sRegClass , _T("OL") ) ||
			 !_tcscmp ( fi_sRegClass , _T("OK") ) )
			return -1;
	}

	return 0;
}

//==================================================================================
// 권책기호 확인 : 049 Tag $v
//  - 권책기호에 숫자, 점, 영문 , 빈칸, 괄호, 슬래쉬, 하이픈 , 쉼표 이외의 값 확인
//==================================================================================
INT BO_MARCIMPORT_TOOL::VolCode_Check( TCHAR *str )
{
    for( ; *str !=  '\0' ; str++ )
    {
       if ( ! ( ( *str >= '0' && *str <= '9' ) ||
		        *str == '.'                    ||
		        ( *str >= 'a' && *str <= 'z' ) ||
		        ( *str >= 'A' && *str <= 'Z' ) ||
				*str == ' '                    ||
		        ( *str == '(' || *str == ')' ) || 
				( *str == '/' || *str == '-' ) ||
				* str == ','             
              ) 
          )
          return -1; 
    } 
    return 0;
}

//==================================================================================
// 분류 기호 확인
// 숫자 , 점 만 있을 경우 정상
// 01-01 朝01 古朝01 N01 B01 082 K 古7-01 1-00
// 고서일 경우 
//==================================================================================
INT BO_MARCIMPORT_TOOL::ClassCode_Check( TCHAR *fi_str , BOOL OL_DATA_FLAG )
{
	INT  i , ClassCode_Length ;
	TCHAR ClassCode[5][16] = { _T("朝"), _T("古朝") , _T("N") , _T("B") , _T("K-") };
	TCHAR tmp[256];
	TCHAR str[1024];

	str_rtrim ( fi_str , str , ' ' );

	if ( str[0] == '\0' )
		return -1;

	// '.' 만 있는 분류기호
	if ( str[0] == '.' && str[1] == '\0' )
		return -1;

	// 숫자와 '.'
	for (i=0 ; str[i] ; i++ )
	{
		if ( ! ( str[i] >= '0' && str[i] <= '9' ||
			     str[i] == '.' ) ) 
		   break ;
	}

	if ( str[i] == '\0' )
		return 0;

	// 영문 한 바이트
	if ( str[1] == '\0' &&
		 ( str[0] >= 'a' && str[0] <= 'z' ||
		   str[0] >= 'A' && str[0] <= 'Z' ) )
		return 0;

	// Point (.) 가 두개인 분류기호
	INT point_cnt = 0 ;
	for ( i=0 ; str[i] ; i++ )
	{
		if ( str[i] == '.' )
			point_cnt ++ ;
	}
	if ( point_cnt > 1 )
		return -999 ;

	// 고서 일 경우에만 해당되는 분류기호
	// '2511 , '3648' '2518' 다음에 해당되는 분류기호
    if ( OL_DATA_FLAG == TRUE )
	{
		if ( !_tcsncmp ( str , _T("2511-") , 5 ) ||
		 	 !_tcsncmp ( str , _T("3648-") , 5 ) ||
 		 	 !_tcsncmp ( str , _T("2518-") , 5 ) )
		{
			for ( i=5 ; str[i] && str[i] != '-' ; i++ )	;
			if ( i == 7 )
			{
				_stprintf ( tmp , _T("%.2s") , str + 5 );
				if ( IntegerCheck ( tmp ) == 0 )
				{
					if ( str[i] == '\0' )
						return 0;
					if ( str[i] == '-' )
					{
						if ( IntegerCheck ( str + (i+1) ) == 0 )
							return 0;
					}
				}
			}
		}
	}

	// _T("朝"), _T("古朝") , _T("N") , _T("B") , _T("K-")
	// 는 각 기호다음 부터 숫자 2 Byte 가 온다
	// N00 ~ N99
	for ( i=0 ; i < 5 ; i ++ )
	{
		ClassCode_Length = _tcsclen ( ClassCode[i] );
		if ( !_tcsncmp ( str , ClassCode[i] , ClassCode_Length) )
		{
			if ( _tcsclen ( str + ClassCode_Length ) > 2 )
				return -1;
            if ( ! ( str[ClassCode_Length] >= '0' && str[ClassCode_Length] <= '9' ) )
				return -1;
            if ( ! ( str[ClassCode_Length+1] >= '0' && str[ClassCode_Length+1] <= '9' ) )
				return -1;
			return 0;
		}
	}

	// _T("古1-00") ~ _T("古10-99")  ,  _T("1-00")  , _T("10-99")

	// '-'를 찾는다.
	for ( i=0; str[i] && str[i] != '-' ; i++ ) 
		tmp[i] = str[i];
	tmp[i] = '\0';
	if ( str[i] == '\0' || i > 4 ) return -1;	// '-' 가 없다.
	if ( str[i+1] == '\0' )		   return -1;   // '-' 까지만 있다.

	// '-' 를 기준으로 값 확인

	// '-' 뒷 숫자 확인
	if ( _tcsclen( &str[i+1] ) > 2 ) return -1;	// 2자리 확인

	if ( IntegerCheck ( &str[i+1] ) ) return -1;	// 뒷자리의 숫자 확인

	// '-' 앞 부분 확인

	// _T("古")
	if ( !_tcsncmp ( tmp , _T("古") , 2 ) )
	{
		if ( tmp[2] >= '1' && tmp[2] <= '9' )
        {
			if ( tmp[3] == '\0')     return 0;
			if ( _tcsclen(tmp+3) > 1 ) return -1;
			if ( tmp[2] == '1' && tmp[3] == '0' )     return 0;
			else                     return -1;
		}
		else                         return -1;
	}

	// 1-00 ~ 10-99
	if ( _tcsclen(tmp) > 2 )			return -1;
	if ( tmp[0] >= '1' && tmp[0] <= '9' )
	{
		if ( tmp[1] == '\0' )       return 0;
		if ( tmp[0] == '1' && tmp[1] == '0' ) return 0;
		else                        return -1;
	}

	return -1;
}

//==================================================================================
// 082 TAG 분류 기호 확인
// 숫자 , 점, 슬래쉬  만 있을 경우 정상
// 01-01 朝01 古朝01 N01 B01 082 K 古7-01 1-00
//==================================================================================
INT BO_MARCIMPORT_TOOL::ClassCode082_Check( TCHAR * fi_str  )
{
	INT  i , ClassCode_Length ;
	TCHAR ClassCode[5][16] = { _T("朝"), _T("古朝") , _T("N") , _T("B") , _T("K-") };
	TCHAR tmp[256];
	TCHAR str[1024];

	str_rtrim ( fi_str , str , ' ' );

	if ( str[0] == '\0' )
		return -1;

	// '.' 만 있는 분류기호
	if ( ( str[0] == '.' || str[0] == '/' ) && str[1] == '\0' )
		return -1;

	// 숫자 , . , / 로 된 분류기호
	for (i=0 ; str[i] ; i++ )
	{
		if ( ! ( str[i] >= '0' && str[i] <= '9' ||
			     str[i] == '.' || str[i] == '/'
			   )
		   ) 
		   break ;
	}

	if ( str[i] == '\0' )
		return 0;

	// 영문 한 바이트 
	if ( str[1] == '\0' &&
		 ( str[0] >= 'a' && str[0] <= 'z' ||
		   str[0] >= 'A' && str[0] <= 'Z' ) )
		return 0;		   


	// Point (.) 가 두개인 분류기호
	INT point_cnt = 0 ;
	for ( i=0 ; str[i] ; i++ )
	{
		if ( str[i] == '.' )
			point_cnt ++ ;
	}
	if ( point_cnt > 1 )
		return -999 ;

	// _T("朝"), _T("古朝") , _T("N") , _T("B") , _T("K-")
	// 는 각 기호다음 부터 숫자 2 Byte 가 온다
	// N00 ~ N99
	for ( i=0 ; i < 5 ; i ++ )
	{
		ClassCode_Length = _tcsclen ( ClassCode[i] );
		if ( !_tcsncmp ( str , ClassCode[i] , ClassCode_Length) )
		{
			if ( _tcsclen ( str + ClassCode_Length ) > 2 )
				return -1;
            if ( ! ( str[ClassCode_Length] >= '0' && str[ClassCode_Length] <= '9' ) )
				return -1;
            if ( ! ( str[ClassCode_Length+1] >= '0' && str[ClassCode_Length+1] <= '9' ) )
				return -1;
			return 0;
		}
	}

	// _T("古1-00") ~ _T("古10-99")  ,  _T("1-00")  , _T("10-99")

	// '-'를 찾는다.
	for ( i=0; str[i] && str[i] != '-' ; i++ ) 
		tmp[i] = str[i];
	tmp[i] = '\0';
	if ( str[i] == '\0' || i > 4 ) return -1;	// '-' 가 없다.
	if ( str[i+1] == '\0' )		   return -1;   // '-' 까지만 있다.

	// '-' 를 기준으로 값 확인

	// '-' 뒷 숫자 확인
	if ( _tcsclen( &str[i+1] ) > 2 ) return -1;	// 2자리 확인

	if ( IntegerCheck ( &str[i+1] ) ) return -1;	// 뒷자리의 숫자 확인

	// '-' 앞 부분 확인

	// _T("古")
	if ( !_tcsncmp ( tmp , _T("古") , 2 ) )
	{
		if ( tmp[2] >= '1' && tmp[2] <= '9' )
        {
			if ( tmp[3] == '\0')     return 0;
			if ( _tcsclen(tmp+3) > 1 ) return -1;
			if ( tmp[2] == '1' && tmp[3] == '0' )     return 0;
			else                     return -1;
		}
		else                         return -1;
	}

	// 1-00 ~ 10-99
	if ( _tcsclen(tmp) > 2 )			return -1;
	if ( tmp[0] >= '1' && tmp[0] <= '9' )
	{
		if ( tmp[1] == '\0' )       return 0;
		if ( tmp[0] == '1' && tmp[1] == '0' ) return 0;
		else                        return -1;
	}

	return -1;
}


/*========================================================================*/
/*  문자열에 Space 가 포함되어 있는지 확인한다.                           */ 
/*========================================================================*/
INT BO_MARCIMPORT_TOOL::CheckSpaceChar( TCHAR * fi_sStr )
{
    for ( ; *fi_sStr != '\0' ; fi_sStr++ )
        if ( *fi_sStr == ' ' )
           return -1;
		
    return 0;
}

//----------------------------------------------------------------------------------
//  문자열이 모두 숫자인지 확인 
//----------------------------------------------------------------------------------
INT BO_MARCIMPORT_TOOL::IntegerCheck ( TCHAR * fi_sStr )
{
    for( ; *fi_sStr != '\0' ; fi_sStr++ )
    {
       if ( !(*fi_sStr >= '0' && *fi_sStr <= '9') )
          return -1; 
    } 

    return 0; 
}

/*========================================================================*/
/* 년도를 확인한다.                                                       */ 
/*========================================================================*/
INT BO_MARCIMPORT_TOOL::CheckYear( TCHAR * fi_sStr )
{
    INT i_year ;

    if ( IntegerCheck( fi_sStr ) )
       return -1;

    i_year = _ttoi( fi_sStr );

    return 0;
}

/*========================================================================*/
/*  문자열에서 특수 문자 삭제                                             */
/*========================================================================*/
VOID BO_MARCIMPORT_TOOL::str_ltrim ( TCHAR *str_in , TCHAR *str_out , TCHAR var )
{
    INT i=0;

    for ( i = 0; str_in[i] != '\0' ; i++ ){
        if(str_in[i] != var) {
            _tcscpy( str_out, &str_in[i] );
            return;
        }
    }
    str_out[0] = '\0';
}

VOID BO_MARCIMPORT_TOOL::str_rtrim ( TCHAR *str_in , TCHAR *str_out , TCHAR var )
{
    INT i = 0 ;

    _tcscpy( str_out, str_in );
    for ( i = _tcsclen(str_in)-1; i >= 0; i-- ){
        if(str_in[i] != var) {
            str_out[i+1] = '\0';
            return;
        }
    }
    str_out[0] = '\0';
}

VOID BO_MARCIMPORT_TOOL::str_trimall ( TCHAR *str_in , TCHAR *str_out , TCHAR var )
{
    INT     i , j;

    for ( i = 0, j = 0 ; str_in[i] != '\0' ; i++ ) {
        if ( str_in[i] == var );
        else { str_out[j] = str_in[i]; j++; }
    }
    str_out[j] ='\0';
}

/*========================================================================*/
/*  문자열중 소문자 확인                                                  */
/*========================================================================*/
bool BO_MARCIMPORT_TOOL::small_letter( TCHAR *str )
{
    INT i , str_length ;

    str_length = _tcsclen( str );

    for ( i=0; i < str_length ; i++ )
        if ( 'a' <= str[i] && str[i] <= 'z' )
            return TRUE ;

    return FALSE;
}

//---------------------------------------------------------------------------------
//  문자열을 일정 길이만큼만 저장하게 한다.
//---------------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::String_Cutting ( TCHAR  * str , INT Length )
{
	INT  i , two_byte_flag;

	two_byte_flag = 0;
	for ( i=0 ; str[i] && i< Length ; i++ )
	{
		if ( two_byte_flag )
			two_byte_flag = 0;
		else
		{
			if ( ( (_TUCHAR) str[i] & 0x80 ) )
				two_byte_flag = 1;
		}
	}

    i = ( i < Length ) ? i : Length ;

    if ( two_byte_flag == 1 )
       str[i-1] = '\0' ;     
    else
       str[i]   = '\0';

	return ;
}

//========================================================================
//  에러 메세지를 하나의 버퍼에 저장한다.
//========================================================================
VOID BO_MARCIMPORT_TOOL::Save_ErrorMsg ( TCHAR * fo_pSaveErrorMsg , TCHAR * fi_pErrorMsg )
{
	if ( fo_pSaveErrorMsg[0] != '\0' )
		_stprintf ( fo_pSaveErrorMsg + _tcsclen(fo_pSaveErrorMsg) , _T("%c%s") , 
		          ERROR_MSG_SEPERATOR , fi_pErrorMsg );
	else
		_stprintf ( fo_pSaveErrorMsg , _T("%s") , fi_pErrorMsg );
}

//========================================================================
//  에러 메세지의 구분값을 Enter 로 치환
//========================================================================
VOID BO_MARCIMPORT_TOOL::ErrorMsgSeperatorToEnter( TCHAR * fio_pSaveErrorMsg  )
{
	INT i ;

	for ( i=0 ; fio_pSaveErrorMsg[i] ; i++ )
	{
		if ( fio_pSaveErrorMsg[i] == (TCHAR)ERROR_MSG_SEPERATOR )
			fio_pSaveErrorMsg[i] = '\n' ;
	}
}

//========================================================================
//  파일에서 견본 자료를 얻는다.
//========================================================================
INT BO_MARCIMPORT_TOOL::SamplingFromSelectFile ( TCHAR * fi_sFileName   , 
												 TCHAR * fo_sSampleData )
{
	FILE * fp ;

	// FILE Open

	fp = _tfopen ( fi_sFileName , _T("rb") );
	if ( fp == NULL )
	{
		MessageBox ( NULL , _T("File Open Error") , _T("MARC 반입") , MB_ICONERROR );
		return -1;
	}	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------

	// 100000 Byte Read
	for ( INT i=0; i < 100000; i++ )
	{
		fo_sSampleData[i] = _fgettc ( fp );
		if ( fo_sSampleData[i] == _TEOF ) break ;
	}
	fo_sSampleData[i] = '\0' ;

	// FILE Close
	fclose ( fp );
	return 0;
}


//-----------------------------------------------------------------------------------
//  외부에서 구축된 자료인지 확인한다.
//-----------------------------------------------------------------------------------
INT BO_MARCIMPORT_TOOL::GetTotalRecordCount ( INT    fi_nImportFileType , 
											  TCHAR * fi_sFileName       ,
											  INT  * fo_nRecTotCnt      )
{

	if ( fi_nImportFileType == MASTER_TYPE )
	{
		if ( Get_MasterTypeRecordCount( fi_sFileName , fo_nRecTotCnt ) )
			return -1;
	}
	else if ( fi_nImportFileType == OUTER_TYPE )
	{
		if ( Get_OuterTypeRecordCount( fi_sFileName , fo_nRecTotCnt ) )
			return -1;
	}

	return 0 ;
}

//-----------------------------------------------------------------------------------
//  전산실에서 구축된 자료의 레코드 건수
//-----------------------------------------------------------------------------------
#ifndef RECORDCNT_CHECK
INT BO_MARCIMPORT_TOOL::Get_MasterTypeRecordCount( TCHAR * fi_sFileName ,
												   INT  * fo_nRecTotCnt )
{
	FILE * fp ;
	TCHAR * ptr   ;
	TCHAR   buffer[102400] ;

	// FILE Open
	fp = _tfopen ( fi_sFileName , _T("rb") );
	if ( fp == NULL )
	{
		MessageBox ( NULL , fi_sFileName , _T("MARC 반입 : File Open Error") , MB_ICONERROR );		
		return -1;
	}
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	*fo_nRecTotCnt = 0;

    while ( (ptr = _fgetts( buffer , 102400 , fp )) != NULL )
    {
        for (                                    ;
              ptr != NULL && buffer[13] == '1'   ;
              ptr = _fgetts( buffer , 102400 , fp ) )
			;

		// 레코드 증가
		if ( !( buffer[0] == (TCHAR) 10 && buffer[1] == '\0') )
		{
			(*fo_nRecTotCnt) ++ ;
		}

	}
	fclose ( fp );

	return 0 ;
}
#else
INT BO_MARCIMPORT_TOOL::Get_MasterTypeRecordCount( TCHAR * fi_sFileName ,
												   INT  * fo_nRecTotCnt )
{
	FILE * fp;
	TCHAR * ptr ;
	TCHAR   buffer[102400];

	// FILE Open
	fp = _tfopen ( fi_sFileName , _T("rb") );
	if ( fp == NULL )
	{
		MessageBox ( m_sFileName , _T("MARC 반입 : File Open Error") , MB_ICONERROR );		
		return -1;
	}	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------

	*fo_nRecTotCnt = 0;

    while ( (ptr = _fgetts( buffer , 102400 , fp )) != NULL )
    {
        for (                                  ;
              ptr != NULL && buffer[13] == '1' ;
              ptr = _fgetts( buffer , 102400 , fp ) )
			;

		// 레코드 증가
		(*fo_nRecTotCnt) ++ ;

	}
	fclose ( fp );

	return 0 ;
}
#endif RECORDCNT_CHECK

//-----------------------------------------------------------------------------------
//  외부에서 구축된 자료의 레코드 건수 
//-----------------------------------------------------------------------------------
INT BO_MARCIMPORT_TOOL::Get_OuterTypeRecordCount( TCHAR * fi_sFileName ,
												  INT  * fo_nRecTotCnt )
{
	FILE * fp;
	INT    c ;

	// FILE Open
	fp = _tfopen ( fi_sFileName , _T("rb") );
	if ( fp == NULL )
	{
		MessageBox ( NULL , fi_sFileName , _T("MARC 반입 : File Open Error") , MB_ICONERROR );		
		return -1;
	}	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------

	// Initial
	*fo_nRecTotCnt = 0; 

    while ( 1 )
    {
		// 레코드 종단기호 ( RT ) 를 기준으로 
		while ( (c = _fgettc ( fp )) != _TEOF && c != RT )
			;

		if ( c == _TEOF ) break ;

		(*fo_nRecTotCnt) ++ ;
	}

	fclose ( fp );

	return 0;
}

//-----------------------------------------------------------------------------------
//  임시 파일명을 만든다.
//  - 원 파일명에 확장자 .swp 를 붙인다.
//-----------------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::CreateTempFileName ( TCHAR * fi_sOriginalFileName , 
											  TCHAR * fo_sTempFileName     )
{
	_stprintf ( fo_sTempFileName , _T("%s.imsi") , fi_sOriginalFileName );
}

//-----------------------------------------------------------------------------------
// Bubble Sort : 오름 차순 
//-----------------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::Bubble_Sort( TCHAR ** fi_asArray , 
									  INT fi_nTotalCnt   )
{
	INT i , j , cmp_value ;
	TCHAR * ptr ;

    for ( i=0 ; i < fi_nTotalCnt-1 ; i++ )
    {
        for ( j=i+1 ; j < fi_nTotalCnt ; j++ )
        {
			cmp_value = _tcscmp( fi_asArray[i] , fi_asArray[j] );

			if ( cmp_value == 0 )   // Source_Idx == Dest_Idx
				continue ;

			if ( cmp_value < 0 )   // Source_Idx < Dest_Idx
				continue ;

			if ( cmp_value > 0 )   // Source_Idx > Dest_Idx
			{
				ptr           = fi_asArray[j] ;
				fi_asArray[j] = fi_asArray[i] ;
				fi_asArray[i] = ptr           ; 
			}
        }
    }	
}

INT BO_MARCIMPORT_TOOL::ISUseFile( TCHAR * fi_sFileName )
{
	HANDLE hFileHandle ;

	hFileHandle = CreateFile ( fi_sFileName , GENERIC_READ | GENERIC_WRITE , 
		                       0 , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , 0 );
	if ( hFileHandle == INVALID_HANDLE_VALUE )
	{
		CloseHandle(hFileHandle);
		MessageBox ( NULL , _T("열려있는 파일을 닫아주십시요") , _T("MARC 반입") , MB_OK );
		return -1;
	}
	CloseHandle(hFileHandle);
	return 0 ;
}


/* ######################################  MARC ################################## */

// MARC 구조 : 
// 리더 ( 24 Byte )
// 디렉토리 ( 다음과 같은 구조 반복 )
//    TAG 번호       ( 3 Byte )
//    필드 길이      ( 4 Byte )
//    필드 시작 위치 ( 5 Byte )
// 필드 종단 기호    ( 1 Byte )
// 각 TAG 에 대한 자료 
// 필드 종단 기호
// 레코드 종단 기호
//-------------------------------------------------------------------------------------
//  FIELD DATA 를 구한다.
//-------------------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::Get_FieldData ( INT    fi_nTagNo      ,
										 TCHAR * fi_sStreamMarc ,
									     TCHAR * fo_sFieldData  )
{
	TCHAR sTagNo[8];
	TCHAR sDirectory[512];
	TCHAR sFieldData[10000];
	INT  i , j , nMarcOffset , nTagIndex ;

	// Output 초기화
	fo_sFieldData[0] = '\0' ;

	// TagNo : Integer => String
	_stprintf ( sTagNo , _T("%03d") , fi_nTagNo );

	// 1. 디렉토리를 구한다.
	for ( nMarcOffset = LEADER_LENGTH , j=0       ; 
	      fi_sStreamMarc[nMarcOffset] != (TCHAR)FT ; 
		  nMarcOffset++ ,j++                      )
		sDirectory [j] = fi_sStreamMarc[nMarcOffset] ;
	sDirectory [j] = '\0' ;

	// 2. 디렉토리에서 TAG 이 몇번째 위치를 구한다.
	j = _tcsclen ( sDirectory );
	for ( i=0 , nTagIndex=0 ; i < j ; i+=12 , nTagIndex++ )
	{
		if ( !_tcsncmp ( sDirectory+i , sTagNo , 3 ) )
			break ;
	}

	// 디렉토리에서 TAG 이 없다.
	if ( i >= j )
		return ;

	// 가변길이 필드에서 TAG 를 구한다.
	// nMarcOffset 는 디렉토리의 필드종단기호를 가리키고 있다.
	i=0;
	do 
	{
		for ( j = 0 , nMarcOffset ++                      ;
		      fi_sStreamMarc[nMarcOffset] != (TCHAR)FT     ;
		      j++ , nMarcOffset ++                        )
		{
			sFieldData[j] = fi_sStreamMarc[nMarcOffset] ;
		}
		sFieldData[j] = '\0' ;
		i++ ;		
	}while ( i <= nTagIndex ) ;

	_tcscpy ( fo_sFieldData , sFieldData );
}

//--------------------------------------------------------------------------
// FieldData 에서 SubFieldData
//--------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::Get_SubFieldData ( TCHAR * fi_pFieldData    ,
											TCHAR * fi_pSubFieldCode ,
											TCHAR * fo_pSubFieldData )
{
    TCHAR *preptr  , * strptr ;

	fo_pSubFieldData[0] = '\0' ;

    if ( fi_pFieldData[0] == '\0' || fi_pSubFieldCode[0] == '\0' )
    {
        fo_pSubFieldData[0] = '\0';
        return ;
    }
   
    preptr = _tcsstr ( fi_pFieldData , fi_pSubFieldCode );
    if ( preptr == NULL )
    {
        fo_pSubFieldData[0] = '\0';
        return ;
    }
    preptr += 2 ;
     
    for ( strptr = preptr; *strptr && *strptr != (TCHAR)GT ; strptr ++ ) 
        ;
    _stprintf ( fo_pSubFieldData , _T("%.*s") , strptr - preptr , preptr );

    return ;
}

//--------------------------------------------------------------------------
//  하나만 값만 존재하는 SubFieldData 를 구한다.
//--------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::Get_SingleSubFieldData ( INT    fi_nTagNo        ,
												  TCHAR * fi_sSubFieldCode ,
										          TCHAR * fi_sStreamMarc   ,
									              TCHAR * fo_sSubFieldData )
{
	TCHAR sFieldData[10000];

	// 1. Output Initial
	fo_sSubFieldData[0] = '\0' ;

	// 2. Get FieldData 
	Get_FieldData ( fi_nTagNo , fi_sStreamMarc , sFieldData );
	if ( sFieldData[0] == '\0' )
		return ;

	// 2. Get FieldData 
	Get_SubFieldData ( sFieldData       ,
					   fi_sSubFieldCode ,
					   fo_sSubFieldData );
}

//-------------------------------------------------------------------------------------
//  StreamMarc => Edit Marc 로 변환
//-------------------------------------------------------------------------------------
VOID BO_MARCIMPORT_TOOL::StreamMarcToEditMarc ( TCHAR * fi_sStreamMarc ,
									            TCHAR * fo_sEditMarc   )
{
	TCHAR sDirectory[512];
	TCHAR sFieldData[10000];
	INT  i , j , nMarcOffset ;

	// Output 초기화
	fo_sEditMarc[0] = '\0' ; 

	// 1. 디렉토리를 구한다.
	for ( nMarcOffset = LEADER_LENGTH , j=0       ; 
	      fi_sStreamMarc[nMarcOffset] != (TCHAR)FT ; 
		  nMarcOffset++ ,j++                      )
		sDirectory [j] = fi_sStreamMarc[nMarcOffset] ;
	sDirectory [j] = '\0' ;

	// nMarcOffset 는 디렉토리의 필드종단기호를 가리키고 있다.
	i=0;
	do 
	{
		// Get FieldData
		for ( j = 0 , nMarcOffset ++                      ;
		      fi_sStreamMarc[nMarcOffset] != (TCHAR)FT &&
			  fi_sStreamMarc[nMarcOffset] != (TCHAR)RT 	  ;
		      j++ , nMarcOffset ++                        )
		{
			sFieldData[j] = fi_sStreamMarc[nMarcOffset] ;
		}
		sFieldData[j] = '\0' ;

		// TAG			  
		_tcsncat ( fo_sEditMarc , sDirectory + (i*12) , 3 );
		_tcscat  ( fo_sEditMarc , _T(" ") );
		// FieldData 
		_tcscat  ( fo_sEditMarc , sFieldData );
		_tcscat  ( fo_sEditMarc , _T("\n") );

		i++;

	}while ( fi_sStreamMarc[nMarcOffset] != (TCHAR)RT ) ;
}

//-------------------------------------------------------------------------------------
//  001 TAG 를 구하다.
//-------------------------------------------------------------------------------------


VOID BO_MARCIMPORT_TOOL::Get_001Tag ( TCHAR * fi_sStreamMarc ,
									  TCHAR * fo_sControlNo  )
{
	Get_FieldData ( 1 , fi_sStreamMarc , fo_sControlNo );
	str_rtrim ( fo_sControlNo , fo_sControlNo , ' ' );
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------



INT BO_MARCIMPORT_TOOL::GetCodeValue( TCHAR          *f_StreamMarc        ,
                                      TCHAR          *f_pPublisheFormCode ,
                                      TCHAR          *f_pMatCode          ,
                                      TCHAR          *f_pFormCode         ,
                                      TCHAR          *f_pMediaCode        )
{   

	


    TCHAR s_001_FieldData[128],s_008_FieldData[128],s_027_FieldData[128],s_088_FieldData[128],s_502_FieldData[128];
    TCHAR s_PubFormCode,s_RecType;
    INT  ids;   
    

    s_001_FieldData[0] = s_008_FieldData[0] = s_502_FieldData[0] = s_027_FieldData[0] = s_088_FieldData[0] = '\0';
	f_pPublisheFormCode[0] = '\0';
    f_pMatCode[0]          = '\0';
    f_pFormCode[0]         = '\0';
    f_pMediaCode[0]        = '\0';

    
    tmp_pStreamMarc.Format (_T("%s"),f_StreamMarc);
    
    ids = m_pMarcMgr->Decoding(tmp_pStreamMarc , &s_MarcStru);
	if( ids ){
		return -1;
	}
      
	INT nTagNo;
    POSITION pos = s_MarcStru.m_listTag.GetHeadPosition () ;
    while( pos != NULL) 
	{   

		CString strField ;
		CTag *pTag = (CTag*)s_MarcStru.m_listTag.GetNext (pos) ;
		if (pTag == NULL) continue;

		nTagNo = _ttoi(pTag->GetTagCode());

        

		
		if( nTagNo  == 1 ){
			
			CString tmp_001;
		    m_pMarcMgr->GetItem(&s_MarcStru,_T("001*"),tmp_001);
		    _tcscpy( s_001_FieldData , tmp_001 );
		}else				
		if( nTagNo  == 8 ){
            CString tmp_008 ;
			m_pMarcMgr->GetItem (&s_MarcStru,_T("008*"),tmp_008);
			_tcscpy( s_008_FieldData , tmp_008);
		}else
		if( nTagNo  == 27 ){
			CString tmp_027;
			m_pMarcMgr->GetField  (&s_MarcStru,pTag->GetTagCode (),tmp_027);
			_tcscpy( s_027_FieldData , tmp_027 );
		}else
		if( nTagNo  == 88 ){
			CString tmp_088 ;
			m_pMarcMgr->GetField (&s_MarcStru,pTag->GetTagCode (),tmp_088);
			_tcscpy( s_088_FieldData , tmp_088 );
		}else
		if( nTagNo  == 502 ){
			CString tmp_052 ;
			m_pMarcMgr->GetField (&s_MarcStru,pTag->GetTagCode (),tmp_052);
			_tcscpy( s_502_FieldData , tmp_052 );				
		}
	}
    

	
	s_PubFormCode = f_StreamMarc[7];
	s_RecType = f_StreamMarc[6];
    
	/*===========================================================================================*/
	// 1.발행형태구분값결정
	/*===========================================================================================*/
	if( s_PubFormCode == 'a' ||
		s_PubFormCode == 'c' ||
		s_PubFormCode == 'm' ){
		_tcscpy( f_pPublisheFormCode , _T("MO"));
	}else
	if( s_PubFormCode == 'b' ||
		s_PubFormCode == 's' ){
		_tcscpy( f_pPublisheFormCode , _T("SE"));
	}else{
		if( s_001_FieldData[0] != '\0'){
			if( _tcsncmp( &s_001_FieldData[1] , _T("SE"), 2 ) == 0 ){
				_tcscpy( f_pPublisheFormCode , _T("SE"));
			}
			else{
				_tcscpy( f_pPublisheFormCode , _T("MO"));
			}
		}
		else{
			_tcscpy( f_pPublisheFormCode , _T("MO"));
		}
	}
	/*===========================================================================================*/



	/*===========================================================================================*/
	// 2.자료구분값결정
	/*===========================================================================================*/
	if( s_001_FieldData[0] != '\0'){
		if( _tcsncmp( s_001_FieldData , _T("KOL"), 3 ) == 0 ||
			_tcsncmp( s_001_FieldData , _T("EOL"), 3 ) == 0 ||
			_tcsncmp( s_001_FieldData , _T("COL"), 3 ) == 0 ||
			_tcsncmp( s_001_FieldData , _T("JOL"), 3 ) == 0 ){
			_tcscpy( f_pMatCode , _T("RB"));
		}else
		if( s_502_FieldData[0] != '\0'){
			if( s_502_FieldData[0] == '0' ){
				_tcscpy( f_pMatCode , _T("DM"));
			}
			else{
				_tcscpy( f_pMatCode , _T("DP"));
			}		
		}else
		if( _tcsncmp( s_001_FieldData , _T("KTX"), 3 ) == 0 ){
			_tcscpy( f_pMatCode , _T("HA"));
		}else
		if( _tcsncmp( &s_001_FieldData[1] , _T("SE"), 2 ) == 0 && 
			( s_008_FieldData[21] == ' ' ||
			  s_008_FieldData[21] == 'p'  ) ){
			_tcscpy( f_pMatCode , _T("MA"));

		}else
		if( ( _tcsncmp( &s_001_FieldData[1] , _T("SE"), 2 ) == 0 ||
			  _tcsncmp( &s_001_FieldData[1] , _T("MO"), 2 ) == 0  ) && 
			s_008_FieldData[29] == '1' ){
			_tcscpy( f_pMatCode , _T("CP"));
		}

	}
	
	if( f_pMatCode[0] == '\0' ){
		if( s_027_FieldData[0] != '\0' &&
			s_088_FieldData[0] != '\0' ){
			_tcscpy( f_pMatCode , _T("RP"));
		}else{
			_tcscpy( f_pMatCode , _T("GM"));
		}
	}	
	/*===========================================================================================*/



	/*===========================================================================================*/
	// 3.형식구분값결정
	/*===========================================================================================*/
	if( s_001_FieldData[0] != '\0'){
		if( _tcsncmp( &s_001_FieldData[1] , _T("CF"), 2 ) == 0 ){
			_tcscpy( f_pFormCode , _T("CF"));
		}else
		if( _tcsncmp( &s_001_FieldData[1] , _T("MU"), 2 ) == 0 ){
			_tcscpy( f_pFormCode , _T("MU"));
		}else
		if( _tcsncmp( &s_001_FieldData[1] , _T("VM"), 2 ) == 0 ){
			_tcscpy( f_pFormCode , _T("VM"));
		}else
		if( _tcsncmp( &s_001_FieldData[1] , _T("MP"), 2 ) == 0 ){
			_tcscpy( f_pFormCode , _T("MP"));
		}
	}

	if( f_pFormCode[0] == '\0' ){
        if( s_RecType == 'm' ){
			_tcscpy( f_pFormCode , _T("CF"));
		}else
        if(	s_RecType == 'c' ||
			s_RecType == 'd' ||
			s_RecType == 'i' ||
			s_RecType == 'j'  ){
			_tcscpy( f_pFormCode , _T("MU"));
		}else
		if( s_RecType == 'e' ||
			s_RecType == 'f' ||
			s_RecType == 'h' ||
			s_RecType == 'g' ||
			s_RecType == 'k' ||
			s_RecType == 'o' ||
			s_RecType == 'r' ){
			_tcscpy( f_pFormCode , _T("MP"));
		}else{
			_tcscpy( f_pFormCode , _T("BK"));
		}
		
	}

	/*===========================================================================================*/



	/*===========================================================================================*/
	// 4.매체구분값결정
	/*===========================================================================================*/
	if( s_RecType == 'a' ){
		_tcscpy( f_pMediaCode , _T("PR"));
	}else
	if( s_RecType == 'c' || s_RecType == 'd'){
		_tcscpy( f_pMediaCode , _T("NA"));
	}else
	{
	    _tcscpy( f_pMediaCode , _T("ZZ"));
	}

	/*===========================================================================================*/

	return 0;
}