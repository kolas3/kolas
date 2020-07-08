// CreateAutorCode.cpp: implementation of the CCreateAutorCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateAutorCode.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//=====================================================
//2008.09.23 ADD BY PJW : K3에서 한자변환 메모리에서 읽어옴
#include "CharConvert.h"
//=====================================================

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateAutorCode::CCreateAutorCode()
{
	//=====================================================
	//2008.09.23 REM BY PJW : K3에서 한자변환 파일읽어오는 부분 제거됨 -> 메모리에서 읽어옴
//  	m_CharSetMgr_KorToChi.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
	//=====================================================
	m_CharSetMgr_KorToConsonant.InitData(_T("한글"), _T("자음"));
}

CCreateAutorCode::~CCreateAutorCode()
{

}

/*==============================================================================
 2000.10.2. 인천지역(서구,중앙,화도진) 도서관 요구사항으로 추가

 Elord저자기기호를 가져다 준다.
 1.저자기호생성시 저자 성명의 3글자와 서명의 1글자를 가져다가 생성
 2.저자가 2글자 또는 1글자의 경우는 저자의 글자의 끝에 ','를 붙이고 서명의 첫글자를 부여한다.
 3.서명에 괄호가 있는 경우 괄호의 Data를 무시하고 그 다음의 서명을 가지고 작성.(관칭을 제외)
 4.저자가 다수이거나 불확실할 경우 또는 총서명일 경우는 서명의 첫 3글자와 출판사의 첫글자를 부여한다.
  * 한글을 글자자수의 기본으로 하며 영문 및 숫자, 특수 기호는 제외한다.

IN :    f_pSubFieldData - 저자명
        f_AuthorAddData - 저자기호 부가 기호
IN :    f_pElordAuthorData - Elord저자명기호명   
==============================================================================*/
INT CCreateAutorCode::GetElordAuthorDate( TCHAR         *f_pSubFieldData    , 
                                       TCHAR         *f_AuthorAddData    ,
                                       TCHAR         *f_pElordAuthorData )
{
	EFS_BEGIN

	//if (m_CharSetMgr_AutorCode.InitData(_T("Elord"), _T("Elord_코드")) < 0) return -1;

    TCHAR  s_AuthorData[1024],s_tmpAuthorData[32];
    TCHAR *s_pStrptr;
    INT   i,j,s_Strlen;

    _tcscpy( s_AuthorData , f_pSubFieldData );

    s_pStrptr = _tcstok(  s_AuthorData , _T(" ") );
    if( s_pStrptr == NULL ) return -1;
        
    s_Strlen = _tcsclen( s_pStrptr );

    j = 0;
    for( i = 0; i < s_Strlen ; i++ ){
        if( (_TUCHAR)s_pStrptr[i] >  0x80 ){
            s_tmpAuthorData[j] = s_pStrptr[i];
            j++;
        }
    }

    s_tmpAuthorData[j] = '\0';

    f_pElordAuthorData[0] = '\0';
    
	// 채정호
	//=====================================================
	//2009.02.16 UPDATE BY PJW : ELOAD는 서명 첫3글자를 이용한다.
// 	if( _tcsclen(s_tmpAuthorData) >= 6 ){
//         _tcsnccpy( f_pElordAuthorData , s_tmpAuthorData , 6 );
//         f_pElordAuthorData[6] = '\0';
//     }
	if( _tcsclen(s_tmpAuthorData) >= 3 ){
        _tcsnccpy( f_pElordAuthorData , s_tmpAuthorData , 3 );
        f_pElordAuthorData[3] = '\0';
    }
	//=====================================================
    else{
        _tcsnccpy( f_pElordAuthorData , s_tmpAuthorData , _tcsclen(s_tmpAuthorData) );
        f_pElordAuthorData[_tcsclen(s_tmpAuthorData)] = '\0';
		//=====================================================
		//2009.01.07 REM BY PJW : ELOAD는 저자와 서명 첫자 사이에 ','가 들어가지 않는다.
		//                        왜추가 했는지는 모르겠음 -> 알아보기
		//2009.07.17 UPDATE BY PJW : ELOAD는 서명이 2자 이하이면 ','를 넣는다.		
		_tcscpy( &f_pElordAuthorData[_tcsclen(f_pElordAuthorData)] , _T(","));
		//=====================================================
    }

	// 채정호
    if( f_AuthorAddData[0] == '(' ){
        s_pStrptr = _tcstok( f_AuthorAddData , _T(")") );
        if( s_pStrptr == NULL ){
            return -2;
        }
        s_pStrptr = _tcstok( NULL, _T(")") );
        if( s_pStrptr == NULL ){
            return -2;
        }
		str_ltrim( s_pStrptr , s_pStrptr , ' ' );
    } else {
		s_pStrptr = f_AuthorAddData;
	}

    s_Strlen = _tcsclen(f_pElordAuthorData);
    for( i = 0 ; i < (INT)_tcsclen(s_pStrptr); i++ ){
        if( (_TUCHAR)s_pStrptr[i] >  0x80 ){
            _tcsnccpy( &f_pElordAuthorData[s_Strlen] , &s_pStrptr[i] , 2 );
			f_pElordAuthorData[s_Strlen+2] = '\0';
            break;
        }
    }

    return 0;

	EFS_END
	return -1;
  
}



// 장일세저자기호 생성
INT CCreateAutorCode::GetJangIlSeaAuthorData(
                                         TCHAR         *f_pSubFieldData        ,  // 저자명
                                         TCHAR         *f_AuthorAddData        ,  // 부가기호
                                         TCHAR         *f_pJangIlSeaAuthorData )  // 장일세저자기호
{
	EFS_BEGIN

	if (m_CharSetMgr_AutorCode.InitData(_T("장일세"), _T("장일세_코드")) < 0) return -1;

    TCHAR  s_AuthorData[1024],s_tmpAuthorData[32],s_Data[32],s_TmpData[128];
    INT   ids;

    str_ltrim( f_pSubFieldData , f_pSubFieldData , ' ' );
    str_rtrim( f_pSubFieldData , f_pSubFieldData , ' ' );

    str_ltrim( f_AuthorAddData , f_AuthorAddData , ' ' );
    str_rtrim( f_AuthorAddData , f_AuthorAddData , ' ' );


    _tcscpy( s_TmpData, f_pSubFieldData );

    //=====================================================
	//2009.02.04 UPDATE BY PJW : UNICODE로 가면서 LENGTH변경됨
// 	_tcsnccpy( s_AuthorData , f_pSubFieldData , 2 );
//     s_AuthorData[2] = '\0';
	_tcsnccpy( s_AuthorData , f_pSubFieldData , 1 );
    s_AuthorData[1] = '\0';
	//=====================================================
    s_Data[0] ='\0';
    f_pJangIlSeaAuthorData[0] = '\0';


    // 1.한글인 경우에만 자음을 붙인다.
    if( (_TUCHAR)s_AuthorData[0] >= 0x80 ){
		ids = m_CharSetMgr_KorToConsonant.Find(s_AuthorData, s_Data);
        if( ids < 0 ){            
            return -1;
        }
        if( s_Data[0] != '\0' ){
            _tcscpy( &f_pJangIlSeaAuthorData[_tcsclen(f_pJangIlSeaAuthorData)] , s_Data );
        }
    }    

    s_tmpAuthorData[0] = '\0';
    /// 2.한자가 있는 경우 한자를 한글로 변환.
	//=====================================================
	//2008.09.23 UPDATE BY PJW : K3에서 한자변환 파일읽어오는 부분 제거됨 -> 메모리에서 읽어옴  
//    if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
//		m_CharSetMgr_KorToChi.Find(s_TmpData, s_tmpAuthorData);
//        if( ids < 0 ) {
//            return -2;
//        }
//        if( s_tmpAuthorData[0] != '\0'){
//            _tcscpy( s_TmpData , s_tmpAuthorData );
//        }
//    }
	if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
		CString strTemp ;
		strTemp.Format (_T("%s"),s_AuthorData );
		ConvertHanjaToHangul(strTemp);
		_tcscpy(s_TmpData,strTemp.GetBuffer (0));
	}
	//=====================================================
    


    /// 3.장일세 저자기호표에서 저자기호를 가져온다.
	for (INT i = 0; i < m_CharSetMgr_AutorCode.nData; i++) {
		ids = _tcscmp(s_TmpData, m_CharSetMgr_AutorCode.pData[i].szFrom);
		if (ids == 0)
		{
			_stprintf(s_Data, m_CharSetMgr_AutorCode.pData[i].szTo);
			break;
		}
		else if (ids < 0 && i != 0)
		{
			_stprintf(s_Data, m_CharSetMgr_AutorCode.pData[i - 1].szTo);
			break;
		}
	}

	//ids = m_CharSetMgr_AutorCode.Find(s_TmpData, s_Data);
    /*ids = m_pUserManager->GetCodeData( m_AuthorCodeValue  ,
                                       _T("코드")        ,
                                       s_TmpData     ,
                                       _T("번호")        ,
                                       s_Data        );
									   */
    //if( ids < 0 ) return 100;

    if( s_Data[0] == 0 ){
        AfxMessageBox(_T("저자기호가 잘못되었습니다."));
        return -3;
    }
    _tcscpy( &f_pJangIlSeaAuthorData[_tcsclen(f_pJangIlSeaAuthorData)] , s_Data );



    // 4.서명의 첫자의 자음을 붙인다.
    _tcsnccpy( s_AuthorData , f_AuthorAddData , 2 );
    s_AuthorData[2] = '\0';
    
    
    if( (_TUCHAR)s_AuthorData[0] >= 0x80 ){
		ids = m_CharSetMgr_KorToConsonant.Find(s_AuthorData, s_Data);
        if( ids < 0 ){            
            return -1;
        }
        if( s_Data[0] != '\0' ){
            _tcscpy( &f_pJangIlSeaAuthorData[_tcsclen(f_pJangIlSeaAuthorData)] , s_Data );
        }
    }    

    return 0;
	
	EFS_END
	return -1;
  

    
}

// 정필모저자기호 생성
INT CCreateAutorCode::GetJungPilMoAuthorData(
                                         TCHAR         *f_pSubFieldData        ,  // 저자명
                                         TCHAR         *f_AuthorAddData        ,  // 부가기호
                                         TCHAR         *f_pJungPilMoAuthorData )  // 정필모저자기호
{
EFS_BEGIN
/*
	if (m_CharSetMgr_AutorCode.InitData(_T("정필모"), _T("정필모_코드")) < 0) return -1;
    
	TCHAR  s_AuthorData[1024],s_tmpAuthorData[32],s_Data[32],s_TmpData[128];
    INT   ids;

    str_ltrim( f_pSubFieldData , f_pSubFieldData , ' ' );
    str_rtrim( f_pSubFieldData , f_pSubFieldData , ' ' );

    str_ltrim( f_AuthorAddData , f_AuthorAddData , ' ' );
    str_rtrim( f_AuthorAddData , f_AuthorAddData , ' ' );


    _tcscpy( s_TmpData, f_pSubFieldData );

    _tcsnccpy( s_AuthorData , f_pSubFieldData , 2 );
    s_AuthorData[2] = '\0';
    s_Data[0] ='\0';
    f_pJungPilMoAuthorData[0] = '\0';



    s_tmpAuthorData[0] = '\0';
    /// 2.한자가 있는 경우 한자를 한글로 변환.
    if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
		ids = m_CharSetMgr_KorToChi.Find(s_TmpData, s_tmpAuthorData);
        if( ids < 0 ) {
            return -2;
        }
        if( s_tmpAuthorData[0] != '\0'){
            _tcscpy( s_TmpData , s_tmpAuthorData );
        }
    }


    /// 3.정필모 저자기호표에서 저자기호를 가져온다.

	ids = m_CharSetMgr_AutorCode.Find(s_TmpData, s_Data);
    /*ids = m_pUserManager->GetCodeData( m_AuthorCodeValue  ,
                                       _T("코드")        ,
                                       s_TmpData     ,
                                       _T("번호")        ,
                                       s_Data        );

    if( ids < 0 ) return 100;

    if( s_Data[0] == 0 ){
        AfxMessageBox(_T("저자기호가 잘못되었습니다."));
        return -3;
    }
    _tcscpy( f_pJungPilMoAuthorData , s_Data );

    return 0;
*/

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;
	
	sAuthorNo = _T("");

		//파일경로
		CString strFilePath = _T("..\\CFG\\정필모.txt");
		
		FILE *fp;
		fp = _tfopen( strFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("저자기호설정 파일이 없습니다.") );
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
		
		INT iLenAuthor = sData.GetLength();
		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{
				
				CString sLine;
				sLine.Format( _T("%s"), cLine );
				CString tnp = sLine.Mid(0,8);
				tnp.Replace( _T(" "), NULL );
				if ( sData >= tnp ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.Replace( _T(" "), NULL );
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("저자기호를 찾을 수 없습니다. 저자기호설정 파일을 확인하세요.") );
	}
	
	_tcscpy( f_pJungPilMoAuthorData , sAuthorNo );
	return 0;
	
	EFS_END
	return -1;
  
}

INT CCreateAutorCode::GetLeeJaeChulAuthorData(				 
							 TCHAR          f_VolCode              ,  // 판표시
                             TCHAR         *f_pSubFieldData        ,  // 저자명                             
                             TCHAR         *f_pJangIlSeaAuthorData ) // 이재철저자기호
{
	EFS_BEGIN

	if (m_CharSetMgr_AutorCode.InitData(_T("리재철2표"), _T("리재철2표_코드")) < 0) return -1;
	
	TCHAR  s_AuthorData[1024],s_tmpAuthorData[32],s_Data[32],s_TmpData[128];
    TCHAR  s_FirstChar[3],s_AuthorCode[32];
    TCHAR *s_pStrptr;
    INT   ids;

	str_ltrim( f_pSubFieldData , f_pSubFieldData , ' ' );
    str_rtrim( f_pSubFieldData , f_pSubFieldData , ' ' );

    _tcscpy( s_TmpData, f_pSubFieldData );

    
    s_Data[0] ='\0';
    f_pJangIlSeaAuthorData[0] = '\0';
    
    s_tmpAuthorData[0] = '\0';
    /// 1.한자가 있는 경우 한자를 한글로 변환.
	//=====================================================
	//2008.09.23 REM BY PJW : K3에서 한자변환 파일읽어오는 부분 제거됨 -> 메모리에서 읽어옴  
//	if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
//        ids = m_CharSetMgr_KorToChi.Find(s_TmpData, s_tmpAuthorData);
//        if( ids < 0 ) {
//            return -2;
//        }
//        if( s_tmpAuthorData[0] != '\0'){
//            _tcscpy( s_TmpData , s_tmpAuthorData );
//        }
//    }
	if( (_TUCHAR)f_pSubFieldData[0] <= 0xFE && (_TUCHAR)f_pSubFieldData[0] >= 0xCA ){
		CString strTemp ;
		strTemp.Format (_T("%s"),s_AuthorData );
		ConvertHanjaToHangul(strTemp);
		_tcscpy(s_TmpData,strTemp.GetBuffer (0));
	}
	//=====================================================
    
    _tcsnccpy( s_FirstChar , s_TmpData ,2 );
    s_FirstChar[2] = '\0';

    // 2. 3 번째 바이트가 한글이 아닌 경우
	s_pStrptr = _tcschr( f_pSubFieldData , ',');
    if( s_pStrptr != NULL && f_pSubFieldData[2] == ',' ){
        
        str_trimall( s_pStrptr , s_pStrptr , ' ' );
        //s_FileIO.str_rtrim( s_pStrptr , s_pStrptr , ' ' );

		//_tcscpy( s_AuthorData , _T(",") );
        _tcsnccpy( s_AuthorData , s_pStrptr , 3 );
        s_AuthorData[3] = '\0';

        /// 2.2. 이재철 저자기호표에서 저자가호를 가져온다.
		ids = m_CharSetMgr_AutorCode.Find(s_AuthorData, s_Data);
        if( ids < 0 ) return 100;

        if( s_Data[0] == 0 ){
            AfxMessageBox(_T("저자기호가 잘못되었습니다."));
            return -3;
        }
        _stprintf( s_AuthorCode , _T(",%s"), s_Data );
    }
    else{
        _tcsnccpy( s_AuthorData , &f_pSubFieldData[2] , 2 );
        s_AuthorData[2] = '\0';
        /// 2.2. 이재철 저자기호표에서 저자가호를 가져온다.
		ids = m_CharSetMgr_AutorCode.Find(s_AuthorData, s_Data);
        if( ids < 0 ) return 100;

        if( s_Data[0] == 0 ){
            AfxMessageBox(_T("저자기호가 잘못되었습니다."));
            return -3;
        }
        _tcscpy( s_AuthorCode , s_Data );
    }
    
    _stprintf( f_pJangIlSeaAuthorData , _T("%s%s"), s_FirstChar, s_AuthorCode );

	return 0;
	
	EFS_END
	return -1;
  



}

VOID    CCreateAutorCode::str_trimall(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
		                    )
{
	EFS_BEGIN

	INT     i = 0, j = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ) {
        if ( strln[i] == var );
        else { strline1[j] = strln[i]; j++; }
    }
    strline1[j] ='\0';
	
	EFS_END
  
}

VOID    CCreateAutorCode::str_ltrim	(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
							)
{
	EFS_BEGIN

	INT i = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ){
        if(strln[i] != var) {
            _tcscpy( strline1, &strln[i] );
            return;
        }
    }
    strline1[0] = '\0';
	
	
	EFS_END

  
}

VOID    CCreateAutorCode::str_rtrim	(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
							)
{
	EFS_BEGIN

	INT i = 0;

    _tcscpy( strline1, strln );
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
        if(strln[i] != var) {
            strline1[i+1] = '\0';
            return;
        }
    }
    strline1[0] = '\0';
	EFS_END

  
}
// 동양서 저자기호 생성
// 생성방법 
// 1. 관련 Tag에서 구한 대상어가 저자기호 테이블의 저자보다 작거나 같고 대상어에서 뒤로부터 
//    한개의 글자를 빼고 조사하여 발견된 자료중 저자기호가 가장 큰 값을 취한다
CString CCreateAutorCode::GetOrientBookData(
											TCHAR *f_pSubFieldData,  //저자명
											TCHAR *f_pAuthorAddData, //저자기호 부가기호
											TCHAR *f_pOrientBookData //동양서 부가기호
											)
{
	
	EFS_BEGIN

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;
	
	sAuthorNo = _T("");

		
		//파일경로
		CString strFilePath = _T("..\\CFG\\동양서저자기호.txt");
		
		FILE *fp;
		fp = _tfopen( strFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("저자기호설정 파일이 없습니다.") );
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
		
		INT iLenAuthor = sData.GetLength();
		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{
				
				CString sLine;
				sLine.Format( _T("%s"), cLine );
				CString tnp = sLine.Mid(0,14);
				tnp.Replace( _T(" "), NULL );
				if ( sData >= tnp ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.Replace( _T(" "), NULL );
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("저자기호를 찾을 수 없습니다. 저자기호설정 파일을 확인하세요.") );
	}
	
	CString strItemData = sData.Left(1) + sAuthorNo;
	return strItemData;

EFS_END
	return _T("");
}

/*==============================================================================
 카터셈본 저자기호 생성

 1.저자기호생성시 저자 성명의 영문자 1글자 파일에서 검색된 숫자를 연결하여 생성한다.

IN :    f_pSubFieldData - 저자명
        f_AuthorAddData - 저자기호 부가 기호
IN :    f_pElordAuthorData - Elord저자명기호명   
==============================================================================*/
CString CCreateAutorCode::GetCutterSanbornAuthorData(
													 TCHAR *f_pSubFieldData, 
													 TCHAR *f_pAuthorAddData, 
													 TCHAR *f_pOrientBookData
													 )
{
EFS_BEGIN

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;

	sAuthorNo = _T("");
	
	//파일경로
	CString strFilePath = _T("..\\CFG\\카터셈본.TXT");
	
	FILE *fp;
	fp = _tfopen( strFilePath , _T("rb") );
	
	if ( fp == NULL ) {
		AfxMessageBox( _T("저자기호설정 파일이 없습니다.") );
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
	
/*
	TCHAR cLine[256];
	while( _fgetts( cLine, 256, fp ) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		CString tnp = sLine.Mid(0,sData.GetLength());
		tnp.Replace( _T(" "), NULL );
		if ( sData <= tnp ) {
			break;
		} else {
			INT iPos = sLine.Find(_T("|"));
			sAuthorNo = sLine.Mid( iPos+1, 3 );
			sAuthorNo.Replace( _T(" "), NULL );
		}
		if ( sData.Left(1) == _T(",") ) {
			sAuthorNo = _T(",") + sAuthorNo;						
		}
*/
		INT iLenAuthor = sData.GetLength();
		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{
				
				CString sLine;
				sLine.Format( _T("%s"), cLine );
				CString tnp = sLine.Mid(0,14);
				
				tnp.TrimLeft();
				tnp.TrimRight();

				//===================================================
				//2009.06.08 ADD BY PJW : 카터셈본 글자 비교시 " "는 제거 하고 비교한다.
				// 2009.11.11 UPDATE BY PDJ : 공백을 제거하면 정상적인 비교를 할 수 없다.
				// ex) 'ROSEM'과 'ROSE M'은 서로 다른 것이다.
				//2009.12.14 UPDATE BY PJW : 교육지원팀 강미화씨 요구사항으로 기존으로 돌림(2009/12/14일자로 메일 받음)
				//2010.05.25 UPDATE BY PJW : 교육지원팀 조영희 과장님 요구사항(2010.05.25 : 5월 정기 패치)으로 SPACE 제거하지 않도록 함
//				tnp.Replace(_T(" "),_T(""));
				//===================================================

				if ( sData >= tnp ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.TrimLeft();
					sAuthorNo.TrimRight();
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}				
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("저자기호를 찾을 수 없습니다. 저자기호설정 파일을 확인하세요.") );
	}

	CString strItemData = sData.Left(1) + sAuthorNo;
	return strItemData;

EFS_END
	return _T("");
}

/*==============================================================================
 카터셈본2 저자기호 생성

 1.저자기호생성시 저자 성명의 영문자 1글자 파일에서 검색된 숫자를 연결하여 생성한다.

IN :    f_pSubFieldData - 저자명
        f_AuthorAddData - 저자기호 부가 기호
IN :    f_pElordAuthorData - Elord저자명기호명   
==============================================================================*/
CString CCreateAutorCode::GetCutterSanborn2AuthorData(
													 TCHAR *f_pSubFieldData, 
													 TCHAR *f_pAuthorAddData, 
													 TCHAR *f_pOrientBookData
													 )
{
EFS_BEGIN

	CString sData = f_pSubFieldData;
	CString sAuthorMid, sAuthorNo;

	sAuthorNo = _T("");
	
	//파일경로
	CString strFilePath = _T("..\\CFG\\카터셈본2.TXT");
	
	FILE *fp;
	fp = _tfopen( strFilePath , _T("rb") );
	
	if ( fp == NULL ) {
		AfxMessageBox( _T("저자기호설정 파일이 없습니다.") );
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
	
/*
	TCHAR cLine[256];
	while( _fgetts( cLine, 256, fp ) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		CString tnp = sLine.Mid(0,sData.GetLength());
		tnp.Replace( _T(" "), NULL );
		if ( sData <= tnp ) {
			break;
		} else {
			INT iPos = sLine.Find(_T("|"));
			sAuthorNo = sLine.Mid( iPos+1, 3 );
			sAuthorNo.Replace( _T(" "), NULL );
		}
		if ( sData.Left(1) == _T(",") ) {
			sAuthorNo = _T(",") + sAuthorNo;						
		}
*/
		INT iLenAuthor = sData.GetLength();
		CString strAuthorCode;
		CString strKeywordAuthorCode;	
		CString tnp;
		CString sLine;

		while ( sAuthorNo == _T("") ) 
		{	
			TCHAR cLine[256];
			while( _fgetts( cLine, 256, fp ) ) 
			{				
				
				sLine.Format( _T("%s"), cLine );
				tnp = sLine.Mid(0,14);
				tnp.TrimLeft();
				tnp.TrimRight();
				strKeywordAuthorCode = tnp;
				strKeywordAuthorCode.MakeUpper();
				if ( sData >= strKeywordAuthorCode ) 
				{
					INT iPos = sLine.Find(_T("|"));
					sAuthorNo = sLine.Mid( iPos+1, 5 );
					sAuthorNo.TrimLeft();
					sAuthorNo.TrimRight();
					if ( sAuthorMid.Left(1) == _T(",") ) 
					{
						sAuthorNo = _T(",") + sAuthorNo;						
					}
					strAuthorCode = tnp;					
				}else
					break;
			}			
			INT result = fseek( fp, 256, SEEK_SET);
	}				
	
	if ( sAuthorNo.IsEmpty())
	{
		AfxMessageBox( _T("저자기호를 찾을 수 없습니다. 저자기호설정 파일을 확인하세요.") );
	}

	// 2005.08.02 ADD BY PDJ
	// 카터셈본2인경우 시작글자의 유형에 따라 코드의 자릿수가 결정
	INT nCodeLen;
	nCodeLen = 1;
	if ( sData.Left(1) == _T("A")  || sData.Left(1) == _T("E")  || 
		 sData.Left(1) == _T("I")  || sData.Left(1) == _T("O")  || 
		 sData.Left(1) == _T("U")  || sData.Left(1) == _T("S")  )
	{
		nCodeLen=2;
	}
	if ( sData.Left(2) == _T("SC") )
	{
		nCodeLen=3;
	}

	if ( sData.GetLength() < nCodeLen )
	{
		nCodeLen = sData.GetLength();
	}	

	CString strItemData = strAuthorCode.Left(nCodeLen) + sAuthorNo;
	return strItemData;

EFS_END
	return _T("");
}

//=====================================================
//2008.09.23 REM BY PJW : K3에서 한자변환 메모리에서 읽어옴 
INT CCreateAutorCode::ConvertHanjaToHangul(CString &strSrc)	
{
	EFS_BEGIN	

	if ( strSrc == "" ) return 0;

	CharConvert	pCharConvert;
	
	CHAR		*UTF8String;
	_TCHAR		*OutUTF16;
	INT			ids;
	INT			nLen;
	
	CharConvert dlg;	
	
	ids = pCharConvert.UnicodeToUTF8(strSrc.GetBuffer(0) , &UTF8String, &nLen);
	ids = pCharConvert.Hanja2Hangul(UTF8String);
	ids = pCharConvert.UTF8ToUnicode(UTF8String, &OutUTF16, &nLen);
	
	strSrc.Format(_T("%s"), OutUTF16 );
	
	free(UTF8String);
	free(OutUTF16);

	
	return 0;

	EFS_END
	return -1;

}
//=====================================================