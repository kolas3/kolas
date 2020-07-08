// PlusIN049Tag.cpp: implementation of the CPlusIN049Tag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\include\공동목록\PlusIN049Tag.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlusIN049Tag::CPlusIN049Tag()
{

}

CPlusIN049Tag::~CPlusIN049Tag()
{

}

//--------------------------------------------------------------------------
// 문자열 '+' 가 있는지 확인한다.
//--------------------------------------------------------------------------
BOOL CPlusIN049Tag::CheckPlusExist ( TCHAR * fi_pOneBookInfo )
{
    TCHAR * ptr;

    if ( fi_pOneBookInfo == NULL )
        return FALSE ;

    for ( ptr = fi_pOneBookInfo ; *ptr && *ptr != '+' ; ptr++ )
        ;

    if ( *ptr == '+' )
        return TRUE ;

    return FALSE ;
}

//##########################################################################
//                           에러 확인 용    
//##########################################################################

//--------------------------------------------------------------------------
// 등록번호 '+' 에러 확인  ( 오류 확인 용 )
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_AccessionPlus ( TCHAR * fi_pPreBookInfo  ,
									     TCHAR * fi_pNextBookInfo ,
									     TCHAR * fo_pErrorMsg     )
{
	TCHAR AccessionNo[2][64] , VolCode[2][64] , CopyCode[2][64];
	TCHAR sTemp[128];
	TCHAR SubFieldCode[8] ;
	TCHAR *ptr ;

	INT  Plus_Type , ExpectAccessionNo_Cnt , i ;
	BOOL BraceHypen_Flag ;

	//--------------------------------------------------------------------------
	// 등록번호 , 권책기호 , 복본기호를 각 버퍼에 저장한다.
	//--------------------------------------------------------------------------
	for ( i=0; i<2 ;i++)
	{
		if ( i == 0 ) ptr = fi_pPreBookInfo  ;
		else          ptr = fi_pNextBookInfo ;

		// 등록번호
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'l' );
		CConverTool->Get_SubFieldData ( ptr             ,
		                                SubFieldCode    ,
										AccessionNo[i]  );
		// 권책기호
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'v' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										VolCode[i]      );

		// 복본기호
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'c' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										CopyCode[i]      );
	}

	if ( AccessionNo[0][0] == '\0' || AccessionNo[1][0] == '\0' )
	{
		_stprintf ( sTemp , _T("'+' 등록번호가 없습니다 [%s] [%s] ") , AccessionNo[0] , AccessionNo[1] );
		CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
		return -1;
	}

	if ( _tcsncmp ( AccessionNo[0] , AccessionNo[1] , 2 ) )
	{
		_stprintf ( sTemp , _T("'+' 등록구분이 다릅니다. [%.2s]:[%.2s]") , AccessionNo[0] , AccessionNo[1] );
		CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
		return -1;
	}

	if ( CConverTool->IntegerCheck ( AccessionNo[0]+2 ) || CConverTool->IntegerCheck( AccessionNo[1] + 2 ) )
	{
		_stprintf ( sTemp , _T("'+' 등록번호의 일련번호 오류 [%s] [%s] ") , AccessionNo[0] , AccessionNo[1] );
		CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
		return -1 ;
	}

	//----------------------------------------------------------------------------
	//  복본기호 , 권책기호 모두 없다.
	//----------------------------------------------------------------------------
	BraceHypen_Flag = FALSE ;

	Plus_Type = Exist_ValueCheck ( VolCode[0]  , VolCode[1]  ,  
		                           CopyCode[0] , CopyCode[1] ,
								   &BraceHypen_Flag         );
	switch ( Plus_Type )
	{

		//-------------------------------------------------------------------------------
		// 등록번호만 있다.
		//-------------------------------------------------------------------------------
		case 1 :
				if ( Expect_OnlyAccessionNo ( AccessionNo[0] ,
					                          AccessionNo[1] ,
											  &ExpectAccessionNo_Cnt ) )
					goto EBOT;
				break ;

		//-------------------------------------------------------------------------------
		// 권책기호만 있다.
		//-------------------------------------------------------------------------------
		case 2 :
			    // 예상되는 등록번호의 수를 구한다.
			    if ( Expect_OnlyVolCode_AccessionNo_Cnt ( VolCode[0]             ,
					                                      VolCode[1]             ,
					                                      BraceHypen_Flag        ,
						  								  &ExpectAccessionNo_Cnt ) )
					goto EBOT;

				// 예상되는 등록번호 수와 실 등록번호를 확인한다.
				if ( Check_AccessionNoCnt ( AccessionNo[0] , 
					                        AccessionNo[1] , 
										    ExpectAccessionNo_Cnt ) )
					goto EBOT;
				break;

		//-------------------------------------------------------------------------------
		// 복본기호만 있다. 또는  권책기호는 같고 복본기호는 증가됨
		//-------------------------------------------------------------------------------
		case 3 :	
			    if ( Check_OnlyCopyCode ( CopyCode[0] , CopyCode[1] ,
					                      &ExpectAccessionNo_Cnt ) )
					goto EBOT;

				if ( Check_AccessionNoCnt ( AccessionNo[0] , 
					                        AccessionNo[1] , 
									  	    ExpectAccessionNo_Cnt ) )
					goto EBOT;
				break;

		//-------------------------------------------------------------------------------
		// 둘다 권책기호는 있고 복본기호는 뒤에만 있는 경우
		//-------------------------------------------------------------------------------
		case 4 :
			    if ( Check_AddVolCode_CopyCode2thExist( VolCode[0]             ,
					                                    VolCode[1]             ,
												        CopyCode[1]            ,
			                                            BraceHypen_Flag        ,
					                                    &ExpectAccessionNo_Cnt ) )
					goto EBOT;

			    if ( Check_AccessionNoCnt ( AccessionNo[0] ,
					                        AccessionNo[1] , 
									        ExpectAccessionNo_Cnt-1 ) )
					goto EBOT;
				
				break;

		//-------------------------------------------------------------------------------
		// 복본기호가 같고 권책기호에 값이 있을 경우
		//-------------------------------------------------------------------------------
		case 5 :
			    if ( Check_SameCopyCodeAddVolCode( VolCode[0]             ,
					                               VolCode[1]             ,
												   CopyCode[0]            ,
			                                       BraceHypen_Flag        ,
					                               &ExpectAccessionNo_Cnt ) )
					/*goto EBOTessionNo_Cnt ) )
					goto EBOTNoCnt ( AccessionNo[0] ,
					                        AccessionNo[1] ,
									        ExpectAccessionNo_Cnt ) )*/
					goto EBOT;

				ExpectAccessionNo_Cnt--;

				break;
		default :
				_stprintf ( sTemp , _T("처리 불가능한 '+' 형태입니다. [%s] [%s] ") , fi_pPreBookInfo , fi_pNextBookInfo );
				CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
				return -1;
	}

	return 0;

EBOT:
	_stprintf ( sTemp , _T("'+'가 포함된 등록번호 처리중 오류발생 [%s] [%s] ") , fi_pPreBookInfo , fi_pNextBookInfo );
	CConverTool->Save_ErrorMsg ( fo_pErrorMsg , sTemp );
	return -1;

}

//----------------------------------------------------------------------------
//  '+' 의 유형을 정한다.
//----------------------------------------------------------------------------
INT CPlusIN049Tag::Exist_ValueCheck ( TCHAR * VolCode1  , TCHAR * VolCode2  , 
								      TCHAR * CopyCode1 , TCHAR * CopyCode2 ,
								      INT  * BraceHypen_Flag              )
{
	INT  i ,j ;
	TCHAR *ptr ;
	INT  BraceHypen1_Flag , BraceHypen2_Flag ;

	INT  Accession_Plus_Type ;

	//------------------------------------------------------------------------
	// 등록번호만 있다.  [ 정상 ] 
	//------------------------------------------------------------------------
	if ( VolCode1[0] == '\0'  && VolCode2[0]  == '\0' &&
		 CopyCode1[0] == '\0' && CopyCode2[0] == '\0' )
		 return 1;

	// 둘 중에 하나만 있는 권책 기호 => 에러
	if ( ( VolCode1[0] == '\0' && VolCode2[0] != '\0' ) ||
		 ( VolCode1[0] != '\0' && VolCode2[0] == '\0' ) )
		 return -1;

	// 앞에는 복본기호가 있고 뒤에는 없다. => 에러
	if ( CopyCode1[0] != '\0' && CopyCode2[0] == '\0' )
		return -1;

	/* 권책기호 없이 복본기호가 1 부터 있을 경우에는 이런 경우가 발생
	// 권책기호 없는 상태에서 복본기호가 뒤에만 있는 경우
	if ( VolCode1[0]  == '\0' && VolCode2[0] == '\0' && 
		 CopyCode1[0] == '\0' && CopyCode2[0] != '\0' )
		 return -1;
	 */

	// 복본기호 값 확인 ( 숫자이외의 값 => 에러 )
	if ( CopyCode1[0] != '\0' || CopyCode2[0] != '\0' )
	{
		for ( i=0 ; i < 2 ; i++ )
		{
			if ( i== 0 ) ptr = CopyCode1 ;
			else         ptr = CopyCode2 ;

			if ( *ptr == '\0' ) continue ;

			if ( *ptr == '0' && *(ptr+1) == '\0' )
				return -1;

			for ( ; *ptr ; ptr++ )
			{
				if ( ! (*ptr >= '0' && *ptr <= '9' ) )
					return -1;
			}
		}
	}

	//=================================================================

	Accession_Plus_Type = 0 ;
	//------------------------------------------------------------------------
	// 권책기호만 있다.									[ 정상 ]
	// [ 권책기호를 확인해야 한다. ]
	//------------------------------------------------------------------------
	if ( CopyCode1[0] == '\0' && CopyCode2[0] == '\0' &&
		 VolCode1[0]  != '\0' && VolCode2[0]  != '\0' )
		 Accession_Plus_Type = 2;

	//------------------------------------------------------------------------
	// 복본기호만 있다.									[ 정상 ]
	//------------------------------------------------------------------------
	if ( VolCode1[0]  == '\0' && VolCode2[0]  == '\0' &&
		 ( ( CopyCode1[0] != '\0' && CopyCode2[0] != '\0') ||
		   ( CopyCode1[0] == '\0' && CopyCode2[0] != '\0') ) )
		 return 3;

	//------------------------------------------------------------------------
	// 권책기호는 같고 복본기호가 다르다.				[ 정상 ]
	//------------------------------------------------------------------------
	if ( !_tcscmp ( VolCode1  , VolCode2  ) &&
		  _tcscmp ( CopyCode1 , CopyCode2 )  )
         return 3;

	//------------------------------------------------------------------------
	// 권책기호가 증가되고 복본기호는 뒤에만 있는 경우	[ 정상 ]
	// [ 권책기호를 확인해야 한다. ]
	//------------------------------------------------------------------------
	if ( _tcscmp ( VolCode1   , VolCode2  ) &&
		 CopyCode1[0] == '\0' && CopyCode2[0] != '\0' )
		 Accession_Plus_Type = 4;

	//------------------------------------------------------------------------
	// 복본기호가 같고 권책기호가 증가됨				[ 정상 ]
	// [ 권책기호를 확인해야 한다. ]
	//------------------------------------------------------------------------
	if (  _tcscmp ( VolCode1  , VolCode2  ) &&
		  CopyCode1[0] != '\0' &&
		  CopyCode2[0] != '\0' &&
		 !_tcscmp ( CopyCode1 , CopyCode2 )  )
         Accession_Plus_Type = 5;

	//------------------------------------------------------------------------
	// 처리가능한 유형에서 벗어남.
	//------------------------------------------------------------------------
	if ( Accession_Plus_Type == 0 )
		return 0;

	//------------------------------------------------------------------------
	// 권책기호를 확인한다.
	//------------------------------------------------------------------------

	// 권책기호 값 확인 ( 숫자 ) , 영문 ( ) -
	BraceHypen1_Flag = BraceHypen2_Flag = 0 ;
	*BraceHypen_Flag = 0 ;
	if ( VolCode1[0] != '\0' )
	{
		for ( i=0 ; i < 2 ; i++ )
		{
			if ( i== 0 ) ptr = VolCode1 ;
			else         ptr = VolCode2 ;

			for (j=0 ; ptr[j] ; j++ )
			{
				if ( ! (  ( ptr[j] >= '0' && ptr[j] <= '9' ) ||
					      ( ptr[j] >= 'a' && ptr[j] <= 'z' ) ||
						  ( ptr[j] >= 'A' && ptr[j] <= 'Z' ) ||
						  ( ptr[j] == '(' || ptr[j] == ')' ) ||
						  ( ptr[j] == '-'                )  ) )
					return -1;

				if ( ptr[j] == '(' || ptr[j] == ')' )
				{
					if ( i==0 ) BraceHypen1_Flag = 1 ;
					else        BraceHypen2_Flag = 1 ;
				}
				if ( ptr[j] == '-' )
				{
					if ( i==0 ) BraceHypen1_Flag = 2 ;
					else        BraceHypen2_Flag = 2 ;
				}
			}
		}

		if ( BraceHypen1_Flag != BraceHypen2_Flag )
			return -1 ;

		if ( BraceHypen1_Flag != 0 &&
			 BraceHypen1_Flag == BraceHypen2_Flag  )
    		*BraceHypen_Flag = BraceHypen1_Flag ;

		if ( *BraceHypen_Flag == 0 )
		{
   			// 영문은 한 바이트어야 한다.
			if ( ! ( VolCode1[0] >= '0' && VolCode1[0] <= '9' ) )
			{
				if ( ( VolCode1[0] >= 'a' && VolCode1[0] <= 'z' || 
					   VolCode1[0] >= 'A' && VolCode1[0] <= 'Z' ) && VolCode1[1] == '\0' &&
					 ( VolCode2[0] >= 'a' && VolCode2[0] <= 'z' ||
					   VolCode2[0] >= 'A' && VolCode2[0] <= 'Z' ) && VolCode2[1] == '\0'
				   )	 
				   ;
				else
					return -1;
			}
		}
	}

	return Accession_Plus_Type ;
}

//-------------------------------------------------------------------------------
// 등록번호만 있는 경우
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Expect_OnlyAccessionNo ( TCHAR * fi_pAccessionNo1          ,
							                TCHAR * fi_pAccessionNo2          ,
										    INT  * fo_pExpectAccessionNo_Cnt )
{

	UINT AccessionNo_SerialNo1 , AccessionNo_SerialNo2 ;

	AccessionNo_SerialNo1 = _ttol ( fi_pAccessionNo1 + 2 );
	AccessionNo_SerialNo2 = _ttol ( fi_pAccessionNo2 + 2 );

	* fo_pExpectAccessionNo_Cnt = (INT)( AccessionNo_SerialNo2 - AccessionNo_SerialNo1 ) ;
	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//-------------------------------------------------------------------------------
// 권책기호만 있을 경우 예상되는 등록번호 수를 구한다.
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Expect_OnlyVolCode_AccessionNo_Cnt ( TCHAR * fi_pVolCode1              ,
													    TCHAR * fi_pVolCode2              ,
										                BOOL   fi_BraceHypen_Flag        ,
	   												    INT  * fo_pExpectAccessionNo_Cnt ) 
{
	TCHAR VolCode1[2][64] , VolCode2[2][64] ;
	TCHAR *ptr;
	INT  j , rtn ;

	* fo_pExpectAccessionNo_Cnt = -1;

	if ( fi_BraceHypen_Flag == FALSE )
	{
		rtn = Check_Two_VolCode ( fi_pVolCode1              ,
			 		              fi_pVolCode2              ,
						          fo_pExpectAccessionNo_Cnt );
		if ( rtn < 0 || *fo_pExpectAccessionNo_Cnt <= 0 || *fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
			return -1;

		return 0;
	}

	//TCHAR VolCode1[2][64] , VolCode2[2][64] ;
	// Brace , '-' 이 있는 상태

	for ( ptr = fi_pVolCode1 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
		VolCode1[0][j] = *ptr ;
	VolCode1[0][j] = '\0';

	if ( *ptr == '(' )
	{
		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode1[1][j] = *ptr;
		if ( *(ptr-1) != ')' )
			return -1;
		VolCode1[1][j-1] = '\0';

		for ( ptr = fi_pVolCode2 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
			VolCode2[0][j] = *ptr ;
		VolCode2[0][j] = '\0';
		if ( *ptr == '\0' )
			return -1;

		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode2[1][j] = *ptr;
		if ( *(ptr-1) != ')' )
			return -1;
		VolCode2[1][j-1] = '\0';
	}
	else
	{
		//-------------------------------------------------------------------------------
		// 분할 저장
		//-------------------------------------------------------------------------------

		// '-' 이다.
		//-------------------------------
		for ( ptr = fi_pVolCode1 , j=0 ; *ptr && *ptr != '-' ; 
		      ptr++ , j++ )
			VolCode1[0][j] = *ptr ;
		if ( *ptr == '\0' )	return -1;
		VolCode1[0][j] = '\0';

		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode1[1][j] = *ptr ;
		VolCode1[1][j] = '\0';

		//-------------------------------
		for ( ptr = fi_pVolCode2, j=0 ; *ptr && *ptr != '-' ; ptr++ , j++ )
			VolCode2[0][j] = *ptr ;
		if ( *ptr == '\0' )	return -1;
		VolCode2[0][j] = '\0';

		for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
			VolCode2[1][j] = *ptr ;
		VolCode2[1][j] = '\0';
	}

	//-------------------------------------------------------------------------------
	// 둘 중의 하나는 값이 같아야 한다.
	//-------------------------------------------------------------------------------
    if ( !_tcscmp ( VolCode1[0] , VolCode2[0] ) &&
          _tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		rtn = Check_Two_VolCode ( VolCode1[1]               ,
			                      VolCode2[1]               ,
								  fo_pExpectAccessionNo_Cnt );
	}	
	else if (  _tcscmp ( VolCode1[0] , VolCode2[0] ) && 
              !_tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		rtn = Check_Two_VolCode ( VolCode1[0]               , 
			                      VolCode2[0]               ,
								  fo_pExpectAccessionNo_Cnt );
	}
	else
		return -1;

	if ( rtn < 0 || *fo_pExpectAccessionNo_Cnt <= 0 || *fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0 ;
}

//--------------------------------------------------------------------------
// 등록번호 차이와 예상되는 등록번호의 갯수를 비교한다.
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Check_AccessionNoCnt ( TCHAR * AccessionNo1          ,
								           TCHAR * AccessionNo2          ,
										   INT    ExpectAccessionNo_Cnt )
{
	UINT AccessionNo_SerialNo1 , AccessionNo_SerialNo2 ;

	// 예상되는 등록번호의 수 확인
	if ( ExpectAccessionNo_Cnt <= 0 )
		return -1;

	AccessionNo_SerialNo1 = _ttol ( AccessionNo1 + 2 );
	AccessionNo_SerialNo2 = _ttol ( AccessionNo2 + 2 );

	// 하나의 '+' 에 저장될 수 있는 등록번호의 초과
	if ( ExpectAccessionNo_Cnt                          > MAX_PLUS_ACCESSION_NO_CNT ||
		 AccessionNo_SerialNo2 - AccessionNo_SerialNo1  > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	if ( ExpectAccessionNo_Cnt == (INT)( AccessionNo_SerialNo2 - AccessionNo_SerialNo1 ) )
		return 0;

	return -1;
}

//--------------------------------------------------------------------------
//  권책기호 전과 후의 값 확인을 한다.
//  - 전과 후의 값 형태가 숫가이거나 영문 이여야 한다.
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_Two_VolCode ( TCHAR * fi_pVolCode1 ,
				   			           TCHAR * fi_pVolCode2 ,
									   INT  * Subtract     )
{
	INT rtn1, rtn2;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( rtn1 < 0 || rtn2 < 0 )
		return -1;

	if ( rtn1 == rtn2 )
	{
		if ( rtn1 == 1 ) // 숫자
			*Subtract = _ttoi(fi_pVolCode2) - _ttoi(fi_pVolCode1) ;
		else if ( rtn1 == 2 ) // 영문
			*Subtract = fi_pVolCode2[0] - fi_pVolCode1[0] ;

		if ( *Subtract <= 0 )
			return -1;

		return rtn1;
	}

	return -1;
}

//--------------------------------------------------------------------------
// 권책기호의 값 확인 
// 숫자이거나 영문 한 Byte
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_VolCode_Eng_Integer ( TCHAR *fi_pStr )
{
	INT i;

	// 숫자 
	for ( i=0 ; fi_pStr[i] ; i++ )
	{
		if ( fi_pStr[i] >='0' && fi_pStr[i] <= '9' )
			;
		else
			break;
	}

	if ( fi_pStr[i] == '\0' )
		return 1 ;

	// 영문 1 byte

	if ( ( ( fi_pStr[0] >= 'a' && fi_pStr[0] <= 'z') ||
		   ( fi_pStr[0] >= 'A' && fi_pStr[0] <= 'Z') )&&
		 fi_pStr[i+1] == '\0' )
		 return 2;

	return -1;
}

//--------------------------------------------------------------------------
// 복본기호만 있을 경우에 예상 등록번호 갯수를 구한다.
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_OnlyCopyCode ( TCHAR * fi_pCopyCode1            ,
									    TCHAR * fi_pCopyCode2            ,
									    INT  * fo_pExpectAccessionNo_Cnt ) 
{
	INT First_CopyCode ;

	First_CopyCode = _ttoi( fi_pCopyCode1 );
	if ( First_CopyCode == 0 ) 
		First_CopyCode = 1;
	
	* fo_pExpectAccessionNo_Cnt = _ttoi(fi_pCopyCode2) - First_CopyCode ;
	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//--------------------------------------------------------------------------
//   복본기호와 권책기호가 있을 경우
// - 복본기호는 같고 권책기호가 증가
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_SameCopyCodeAddVolCode( TCHAR * fi_pVolCode1               ,
							                     TCHAR * fi_pVolCode2               ,
											     TCHAR * fi_pCopyCode               ,
						                         BOOL   fi_BraceHypen_Flag         ,
								                 INT  * fo_pExpectAccessionNo_Cnt  )
{
	INT rtn;
	INT Expect_VolAccessionNoCnt ;

	//------------------------------------------------------------------------
	// 권책기호 차이에 대한 등록번호 예상 수 
	//------------------------------------------------------------------------
	rtn = Expect_OnlyVolCode_AccessionNo_Cnt ( fi_pVolCode1              ,
		                                       fi_pVolCode2              ,
											   fi_BraceHypen_Flag        ,
										       &Expect_VolAccessionNoCnt ) ;
	if (rtn)
		return -1;

	//------------------------------------------------------------------------
	// 예상 등록번호 수
	//------------------------------------------------------------------------
	* fo_pExpectAccessionNo_Cnt = Expect_VolAccessionNoCnt * _ttoi(fi_pCopyCode);

	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//--------------------------------------------------------------------------
//   복본기호와 권책기호가 있을 경우
// - 권책기호는 증가되고 복본기호는 뒤에만 있는 경우
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Check_AddVolCode_CopyCode2thExist(TCHAR * fi_pVolCode1               ,
							                         TCHAR * fi_pVolCode2               ,
											         TCHAR * fi_pCopyCode               ,
											   	     BOOL   fi_BraceHypen_Flag         ,
												     INT  * fo_pExpectAccessionNo_Cnt  ) 
{
	INT rtn;
	INT Expect_VolAccessionNoCnt ;

	//------------------------------------------------------------------------
	// 권책기호 차이에 대한 등록번호 예상 수 
	//------------------------------------------------------------------------
	rtn = Expect_OnlyVolCode_AccessionNo_Cnt ( fi_pVolCode1              ,
		                                       fi_pVolCode2              ,
											   fi_BraceHypen_Flag        ,
										       &Expect_VolAccessionNoCnt ) ;
	if (rtn)
		return -1;

	//------------------------------------------------------------------------
	// 예상 등록번호 수
	//------------------------------------------------------------------------
	* fo_pExpectAccessionNo_Cnt = ( Expect_VolAccessionNoCnt + 1 )* _ttoi(fi_pCopyCode);

	if ( * fo_pExpectAccessionNo_Cnt <= 0 || * fo_pExpectAccessionNo_Cnt > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	return 0;
}

//##########################################################################
//               '+' 에서 등록번호 , 권책기호 , 복본 기호 추출 
//##########################################################################

//===============================================================================
//  '+' 기호가 있는 등록번호 처리 
//===============================================================================
INT CPlusIN049Tag::Save_Plus_BookInfo  (  TCHAR * fi_pPreBookInfo  ,
									      TCHAR * fi_pNextBookInfo ,
 										  TCHAR * fo_pAccessionNo  , 
										  TCHAR * fo_pVolCode      , 
										  TCHAR * fo_pCopyCode     , 
                                          TCHAR * fo_pShelfCode    )
{
	TCHAR AccessionNo[2][64] , VolCode[2][64] , CopyCode[2][64];
	TCHAR ShelfCode[2][16];
	TCHAR SubFieldCode[8] ;
	TCHAR *ptr ;

	INT  Plus_Type , i ;
	INT  BraceHypen_Flag ;

	TCHAR SameVolCode[256] , AddVolStart[256] , AddVolEnd[256];
	INT  AddLocate ;

	//--------------------------------------------------------------------------
	// 등록번호 , 권책기호 , 복본기호를 각 버퍼에 저장한다.
	//--------------------------------------------------------------------------
	for ( i=0; i<2 ;i++)
	{
		if ( i == 0 ) ptr = fi_pPreBookInfo  ;
		else          ptr = fi_pNextBookInfo ;

		// 등록번호
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'l' );
		CConverTool->Get_SubFieldData ( ptr             ,
		                                SubFieldCode    ,
										AccessionNo[i]    );
		// 권책기호
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'v' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										VolCode[i]      );

		// 복본기호
	    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'c' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
										CopyCode[i]     );

		_stprintf ( SubFieldCode , _T("%c%c") , GT , 'f' );
		CConverTool->Get_SubFieldData ( ptr             ,
										SubFieldCode    ,
									    ShelfCode[i]   );
	}

	//----------------------------------------------------------------------------
	//  복본기호 , 권책기호 모두 없다.
	//  BraceHypen_Flag == 1 : ( )   == 2 : - 
	//----------------------------------------------------------------------------
	BraceHypen_Flag = FALSE ;

	Plus_Type = Exist_ValueCheck ( VolCode[0]  , VolCode[1]  ,
		                           CopyCode[0] , CopyCode[1] ,
								   &BraceHypen_Flag         );
	switch ( Plus_Type )
	{
		//-------------------------------------------------------------------------------
		// 등록번호만 있다.
		//-------------------------------------------------------------------------------
		case 1 :
				if ( Save_OnlyAccessionNo ( AccessionNo[0]     ,
					                        AccessionNo[1]     ,
											fo_pAccessionNo    ,
											fo_pVolCode        ,
											fo_pCopyCode       ) )
					return -1;

				break;

		//-------------------------------------------------------------------------------
		// 권책기호만 있다.
		//-------------------------------------------------------------------------------
		case 2 :
				//-----------------------------------------------------------------------
				// 분리기호가 없을 경우
				//-----------------------------------------------------------------------
			    if ( BraceHypen_Flag == 0 )
				{
					if ( Save_OnlyVolCode_NoBraceHypen ( VolCode[0]       ,
						                                 VolCode[1]       ,
														 AccessionNo[0]   ,
														 AccessionNo[1]   ,
 								                         fo_pAccessionNo  , 
		                                                 fo_pVolCode      , 
                                                         fo_pCopyCode     ) )
						return -1;
					break ;
				}

				//-----------------------------------------------------------------------
				// 분리기호가 있을 경우
				//-----------------------------------------------------------------------

				//-----------------------------------------------------------------------
				// 권책기호를 분석하여 증가될 값의 위치 , 값 , 같을 값을 구한다.
				//-----------------------------------------------------------------------
				if ( ExistBraceHypen_Separation_VolCode ( VolCode[0]      ,
										                  VolCode[1]      ,
										                  BraceHypen_Flag ,
										                  SameVolCode     ,
										                  &AddLocate      ,
										                  AddVolStart     ,
										                  AddVolEnd       ) )
					return -1;

				//-----------------------------------------------------------------------
				// 등록번호 , 권책기호 , 복본기호를 구한다.
				//-----------------------------------------------------------------------
				if ( Save_OnlyVolCode_ExistBraceHypen ( AccessionNo[0]  ,
					                                    AccessionNo[1]  ,
					                                    BraceHypen_Flag , 
														SameVolCode     ,
														AddLocate       ,
														AddVolStart     ,
														AddVolEnd       ,
 								                        fo_pAccessionNo , 
		                                                fo_pVolCode     , 
                                                        fo_pCopyCode    ) )
					return -1;
				break;

		//-------------------------------------------------------------------------------
		// 복본기호만 있다. 또는  권책기호는 같고 복본기호는 증가됨
		//-------------------------------------------------------------------------------
		case 3 :	
			    if ( Save_OnlyCopyCode ( AccessionNo[0]   ,
					                     AccessionNo[1]   ,
					                     VolCode[0]       ,
					                     CopyCode[0]      ,
					                     CopyCode[1]      ,
		 	                             fo_pAccessionNo  ,
                                         fo_pVolCode      , 
                                         fo_pCopyCode     ) )
					return -1;

				break;

		//-------------------------------------------------------------------------------
		// 권책기호가 증가되고 복본기호는 두번째에만 있다.
		//-------------------------------------------------------------------------------
		case 4 :
			    if ( BraceHypen_Flag == 0 )
				{
					if ( Save_AddVolCode2thCopyCodeExist_NoBraceHypen ( VolCode[0]       ,
						                                                VolCode[1]       ,
															      		CopyCode[1]      ,
														                AccessionNo[0]   ,
														                AccessionNo[1]   ,
 								                                        fo_pAccessionNo  , 
		                                                                fo_pVolCode      , 
                                                                        fo_pCopyCode     ) )
						return -1;
					break ;
				}

				//-----------------------------------------------------------------------
				// 권책기호를 분석하여 증가될 값의 위치 , 값 , 같을 값을 구한다.
				//-----------------------------------------------------------------------
				if ( ExistBraceHypen_Separation_VolCode ( VolCode[0]      ,
										                  VolCode[1]      ,
										                  BraceHypen_Flag ,
										                  SameVolCode     ,
										                  &AddLocate      ,
										                  AddVolStart     ,
										                  AddVolEnd       ) )
					return -1;

				//-----------------------------------------------------------------------
				// 등록번호 , 권책기호 , 복본기호를 구한다.
				//-----------------------------------------------------------------------
				if ( Save_AddVolCode2thCopyCodeExist_ExistBraceHypen ( AccessionNo[0]  ,
					                                                   AccessionNo[1]  ,
					                                                   BraceHypen_Flag , 
												          		       SameVolCode     ,
														               AddLocate       ,
														               AddVolStart     ,
														               AddVolEnd       ,
																       CopyCode[1]     ,
 								                                       fo_pAccessionNo , 
		                                                               fo_pVolCode     , 
                                                                       fo_pCopyCode    ) )
					return -1;
				break;

		//-------------------------------------------------------------------------------
		// 복본기호가 같고 권책기호에 값이 있을 경우
		//-------------------------------------------------------------------------------
		case 5 :
			    if ( BraceHypen_Flag == 0 )
				{
					if ( Save_SameCopyCodeAddVolCode_NoBraceHypen ( VolCode[0]       ,
						                                            VolCode[1]       ,
																	CopyCode[0]      ,
														            AccessionNo[0]   ,
														            AccessionNo[1]   ,
 								                                    fo_pAccessionNo  , 
		                                                            fo_pVolCode      , 
                                                                    fo_pCopyCode     ) )
						return -1;
					break ;
				}

				//-----------------------------------------------------------------------
				// 권책기호를 분석하여 증가될 값의 위치 , 값 , 같을 값을 구한다.
				//-----------------------------------------------------------------------
				if ( ExistBraceHypen_Separation_VolCode ( VolCode[0]      ,
										                  VolCode[1]      ,
										                  BraceHypen_Flag ,
										                  SameVolCode     ,
										                  &AddLocate      ,
										                  AddVolStart     ,
										                  AddVolEnd       ) )
					return -1;

				//-----------------------------------------------------------------------
				// 등록번호 , 권책기호 , 복본기호를 구한다.
				//-----------------------------------------------------------------------
				_stprintf( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u"),
						 AccessionNo[0]  , _ttol( AccessionNo[0] + 2 ) ) ;
				_stprintf( fo_pVolCode+_tcsclen(fo_pVolCode)   , _T("%s#") , VolCode[0] );
				_stprintf( fo_pCopyCode+_tcsclen(fo_pCopyCode) , _T("%s#") , CopyCode[0] );

				if ( Save_SameCopyCodeAddVolCode_ExistBraceHypen ( AccessionNo[0]  ,
					                                               AccessionNo[1]  ,
					                                               BraceHypen_Flag , 
												          		   SameVolCode     ,
														           AddLocate       ,
														           AddVolStart     ,
														           AddVolEnd       ,
																   CopyCode[0]     ,
 								                                   fo_pAccessionNo , 
		                                                           fo_pVolCode     , 
                                                                   fo_pCopyCode    ) )
					return -1;						
				break;
		default :
				return -1;
	}

	//----------------------------------------------------------------------------
	// 각 권에 별치기호 저장
	//----------------------------------------------------------------------------
	Save_ShelfCode ( AccessionNo[0] , AccessionNo[1] , 
		             ShelfCode[0]   , ShelfCode[1]   , 
					 fo_pShelfCode );

	return 0;
}


//----------------------------------------------------------------------------
// 각 권에 별치기호 저장
// 규칙 : 첫번째 별치기호 : 맨 마지막 - 1 번째 등록번호에 저장
//        두번째 별치기호 : 맨 마지막번째 등록번호에 저장
//----------------------------------------------------------------------------
VOID CPlusIN049Tag:: Save_ShelfCode ( TCHAR * fi_pAccessionNo1 ,
                                     TCHAR * fi_pAccessionNo2 ,
					                 TCHAR * fi_pShelfCode1   ,
                                     TCHAR * fi_pShelfCode2   ,
					                 TCHAR * fo_pShelfCode    )
{
	INT s_nLastIndex , i ;
	INT s_nOffset = 0 ;
	TCHAR temp[16];

	s_nLastIndex = _ttoi ( fi_pAccessionNo2 + 2 ) - _ttoi ( fi_pAccessionNo1 + 2 ) ;
	s_nOffset = _tcsclen ( fo_pShelfCode );

	// 1. 맨 마지막 - 1 전 까지는  ' ' 로 채운다
	for ( i = 0 ; i < s_nLastIndex - 1 ; i ++ )
	{
		s_nOffset += _stprintf ( fo_pShelfCode + s_nOffset , _T(" #") );
	}

	// 2. 첫번째 별치기호에 값이 있을 경우 마지막의 두번째에 저장
	CConverTool->str_rtrim ( fi_pShelfCode1 , temp , ' ' );
	s_nOffset += _stprintf ( fo_pShelfCode + s_nOffset , _T("%s#") ,
		                   (temp[0] != '\0') ? temp : _T(" ") );

	// 3. 마지막 별치기호에 값이 있을 경우 마지막의 두번째에 저장
	CConverTool->str_rtrim ( fi_pShelfCode2 , temp , ' ' );
	s_nOffset += _stprintf ( fo_pShelfCode + s_nOffset , _T("%s#") ,
		                   (temp[0] != '\0') ? temp : _T(" ") );
}



//----------------------------------------------------------------------------
//  OneBookInfo : $lEM9584736$v123$c2
//----------------------------------------------------------------------------
VOID CPlusIN049Tag::Save_BookInfo  (  TCHAR * fi_pOneBookInfo , 
						              TCHAR * fo_pAccessionNo , 
									  TCHAR * fo_pVolCode     , 
									  TCHAR * fo_pCopyCode    ,
                                      TCHAR * fo_pShelfCode   )
{
    TCHAR Temp[256] , SubFieldCode[8];

	//----------------------------------------------------------------------------
	//  등록번호
	//----------------------------------------------------------------------------
    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'l' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
									SubFieldCode     ,
									Temp             );
    
    _stprintf( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%.2s") , Temp );
    _stprintf( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%010u") , _ttol( Temp+2 ) );

	//----------------------------------------------------------------------------
	//  권책기호
	//----------------------------------------------------------------------------
    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'v' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
								    SubFieldCode     ,
									Temp             );

    _stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ), _T("%s#") , (Temp[0] == '\0') ? _T(" ") : Temp );

	//----------------------------------------------------------------------------
	//  복본기호
	//----------------------------------------------------------------------------

    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'c' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
									SubFieldCode     ,
									Temp             );

    _stprintf ( fo_pCopyCode + _tcsclen( fo_pCopyCode ), _T("%s#") , (Temp[0] == '\0') ? _T(" ") : Temp );

	//----------------------------------------------------------------------------
	//  별치기호
	//----------------------------------------------------------------------------
    _stprintf ( SubFieldCode , _T("%c%c") , GT , 'f' );
    CConverTool->Get_SubFieldData ( fi_pOneBookInfo  ,
									SubFieldCode     ,
									Temp             );
	CConverTool->str_rtrim ( Temp , Temp , ' ' );
    _stprintf ( fo_pShelfCode + _tcsclen( fo_pShelfCode ), _T("%s#") , (Temp[0] == '\0') ? _T(" ") : Temp );

    return ;
}

//-------------------------------------------------------------------------------
// 등록번호만 있다.
//-------------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_OnlyAccessionNo ( TCHAR * fi_pAccessionNo1 , 
									       TCHAR * fi_pAccessionNo2 ,
									 	   TCHAR * fo_pAccessionNo  ,
										   TCHAR * fo_pVolCode      ,
										   TCHAR * fo_pCopyCode     )
{
	INT i;
	UINT AccessionNo ;
	UINT AccessionNo1 , AccessionNo2 ;

	AccessionNo1 = _ttol ( fi_pAccessionNo1 + 2 );
	AccessionNo2 = _ttol ( fi_pAccessionNo2 + 2 );

	if ( AccessionNo2 - AccessionNo1 <= 0  ||
		 AccessionNo2 - AccessionNo1 + 1 > MAX_PLUS_ACCESSION_NO_CNT )
		return -1;

	for ( AccessionNo = AccessionNo1 , i = 0 ; AccessionNo <= AccessionNo2 ; AccessionNo++ , i++ )
	{
		// 등록번호
		_stprintf ( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%.2s%010u") , fi_pAccessionNo1 ,
				                                                         _ttol(fi_pAccessionNo1 + 2) + i );

		// 권책기호
		_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode )   , _T("%c#") , ' ' );

		// 복본기호
		_stprintf ( fo_pCopyCode + _tcsclen ( fo_pCopyCode ) , _T("%c#") , ' ' );
	}

	return 0;
}

//-------------------------------------------------------------------------------
// 권책기호만 있고 () , - 가 없는 상태에서 등록번호, 권책기호, 복본기호
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Save_OnlyVolCode_NoBraceHypen ( TCHAR * fi_pVolCode1    ,
	 											   TCHAR * fi_pVolCode2    ,
												   TCHAR * fi_pAccessionNo1,
												   TCHAR * fi_pAccessionNo2,
 												   TCHAR * fo_pAccessionNo , 
												   TCHAR * fo_pVolCode     , 
												   TCHAR * fo_pCopyCode    )
{
	// KOL.UDF.022 시큐어코딩 보완
	INT rtn1 = 0, rtn2 = 0 , i = 0;
	INT Start_Vol = 0 , End_Vol = 0 , VolCode = 0;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
	rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( rtn1 < 0 || rtn2 < 0 || rtn1 != rtn2 )
		return -1;

	if ( rtn1 == 1 )
	{
		Start_Vol = _ttoi ( fi_pVolCode1 );   End_Vol = _ttoi ( fi_pVolCode2 );
	}else if ( rtn1 == 2 )
	{
		Start_Vol = fi_pVolCode1[0] ;   End_Vol = fi_pVolCode2[0] ;
	}

	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
			                    End_Vol - Start_Vol ) )
		return -1;

	for ( VolCode = Start_Vol , i = 0 ; VolCode <= End_Vol ; VolCode++ , i++ )
	{
		// 등록번호
		_stprintf ( fo_pAccessionNo + _tcsclen( fo_pAccessionNo ) , _T("%.2s%010u") , fi_pAccessionNo1 , 
				                                                         _ttol(fi_pAccessionNo1 + 2) + i );

		// 권책기호
		if ( rtn1 == 1 ) // 숫자
			_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d#") , VolCode );
		else if ( rtn1 == 2 ) // 영문
			_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c#") , VolCode );

		// 복본기호
		_stprintf ( fo_pCopyCode + _tcsclen ( fo_pCopyCode ) , _T("%c#") , ' ' );
	}

	return 0;
}

//-------------------------------------------------------------------------------
// 권책기호에 분리기호 ( '()' '-' ) 가 있을 경우 그 값을 분석하여
// 권책기호가 증가할 수 있도록 한다.
// 인자
//  fi_pVolCode1     : 첫 권책기호
//	fi_pVolCode2     : 끝 권책기호
//	BraceHypen_Flag  : 분리기호의 종류    ( 1 : ()   2 : - )
//	fo_pSameVolCode  : 변하지 않는 값 
//	fo_AddLocate     : 증가되는 값의 위치 ( 1 : 앞   2 ; 뒤 )
//	fo_pAddVolStart  : 증가되는 값의 시작 값
//	fo_pAddVolEnd    : 증가되는 값의 끝  값
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::ExistBraceHypen_Separation_VolCode ( TCHAR * fi_pVolCode1    ,
														TCHAR * fi_pVolCode2    ,
														INT	BraceHypen_Flag    ,
														TCHAR * fo_pSameVolCode ,
														INT  * fo_pAddLocate   ,
														TCHAR * fo_pAddVolStart ,
														TCHAR * fo_pAddVolEnd   )
{
	TCHAR VolCode1[2][64] , VolCode2[2][64] ;
	TCHAR *ptr;
	INT  j ;

	//------------------------------------------------------------------------
	//  값 분할 저장
	//------------------------------------------------------------------------

	switch ( BraceHypen_Flag )
	{
		case 1 :	//	'( )'

				// Prev
				for ( ptr = fi_pVolCode1 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
					VolCode1[0][j] = *ptr ;
				VolCode1[0][j] = '\0';
				if ( *ptr != '(' ) 
					return -1;

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode1[1][j] = *ptr;
				if ( *(ptr-1) != ')' )
					return -1;
				VolCode1[1][j-1] = '\0';
		
				// Next
				for ( ptr = fi_pVolCode2 , j= 0; *ptr && *ptr != '(' ; ptr++ , j++ )
					VolCode2[0][j] = *ptr ;
				VolCode2[0][j] = '\0';
				if ( *ptr == '\0' )
					return -1;

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode2[1][j] = *ptr;
				if ( *(ptr-1) != ')' )
					return -1;
				VolCode2[1][j-1] = '\0';

			break;
		case 2 :	//  ' - '

				for ( ptr = fi_pVolCode1 , j=0 ; *ptr && *ptr != '-' ; 
					  ptr++ , j++ )
					VolCode1[0][j] = *ptr ;
				if ( *ptr == '\0' )	return -1;
				VolCode1[0][j] = '\0';

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode1[1][j] = *ptr ;
				VolCode1[1][j] = '\0';

				//-------------------------------
				for ( ptr = fi_pVolCode2, j=0 ; *ptr && *ptr != '-' ; ptr++ , j++ )
					VolCode2[0][j] = *ptr ;
				if ( *ptr == '\0' )	return -1;
				VolCode2[0][j] = '\0';

				for ( ptr++, j=0 ; *ptr ; ptr++ , j++ )
					VolCode2[1][j] = *ptr ;
				VolCode2[1][j] = '\0';

			break;
		default :
			return -1;
	}

	//-------------------------------------------------------------------------------
	// 둘 중의 하나는 값이 같아야 한다.
	//-------------------------------------------------------------------------------

	// 앞 쪽이 같을 경우
    if ( !_tcscmp ( VolCode1[0] , VolCode2[0] ) &&
          _tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		*fo_pAddLocate = 2 ; 
		_stprintf ( fo_pSameVolCode , _T("%s") , VolCode1[0] );
		_stprintf ( fo_pAddVolStart , _T("%s") , VolCode1[1] );
		_stprintf ( fo_pAddVolEnd   , _T("%s") , VolCode2[1] );

	}	
	// 뒷 쪽이 같을 경우
	else if (  _tcscmp ( VolCode1[0] , VolCode2[0] ) && 
              !_tcscmp ( VolCode1[1] , VolCode2[1] ) )
	{
		*fo_pAddLocate = 1 ; 
		_stprintf ( fo_pSameVolCode , _T("%s") , VolCode1[1] );
		_stprintf ( fo_pAddVolStart , _T("%s") , VolCode1[0] );
		_stprintf ( fo_pAddVolEnd   , _T("%s") , VolCode2[0] );
	}
	else
		return -1;

	return 0;
}

//-----------------------------------------------------------------------
// 권책기호만 있는 상태에서 
// 등록번호 , 권책기호 , 복본기호를 구한다.
//-----------------------------------------------------------------------
INT CPlusIN049Tag::Save_OnlyVolCode_ExistBraceHypen ( TCHAR * fi_pAccessionNo1   ,
													  TCHAR * fi_pAccessionNo2   ,
													  INT    fi_BraceHypen_Flag ,
													  TCHAR * fi_pSameVolCode    ,
													  INT    fi_AddLocate       ,
													  TCHAR * fi_pAddVolStart    ,
													  TCHAR * fi_pAddVolEnd      ,
 													  TCHAR * fo_pAccessionNo    ,
													  TCHAR * fo_pVolCode        ,
												  	  TCHAR * fo_pCopyCode       )
{
	INT rtn1 , rtn2         ;
	INT VolCode , Start_Vol , End_Vol ;
	INT i;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pAddVolStart );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pAddVolEnd   );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	switch ( rtn1 )
	{
		case 1 :		// 숫자
				Start_Vol = _ttoi( fi_pAddVolStart ) ;
				End_Vol   = _ttoi( fi_pAddVolEnd )   ;
				break;

		case 2 :		// 영문
				Start_Vol = fi_pAddVolStart[0];
				End_Vol   = fi_pAddVolEnd[0]  ;
				break;

		default :
			return -1;
	}

	// 개수 확인
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        End_Vol - Start_Vol ) )
		return -1;								

	//-------------------------------------------------------------------
	// 등록번호, 복본기호, 권책기호 저장
	//-------------------------------------------------------------------
	for ( VolCode = Start_Vol , i = 0  ; VolCode <= End_Vol ; VolCode ++ , i++ )
	{
		// 등록번호
		_stprintf ( fo_pAccessionNo + _tcsclen ( fo_pAccessionNo ) , _T("%.2s%010u") , 
			                                                     fi_pAccessionNo1 ,
																 _ttol(fi_pAccessionNo1 + 2 ) + i );
		// 권책기호
		if ( rtn1 == 1 )  // 숫자
		{
			if ( fi_AddLocate == 1 ) // 앞이 증가
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d%c%s%s"),
																 VolCode   ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 fi_pSameVolCode ,
																 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
			if ( fi_AddLocate == 2 ) // 뒤가 증가
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%d%s"),
																 fi_pSameVolCode                      ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 VolCode                              ,
				                                                 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
		}
		else if ( rtn1 == 2 )
		{
			if ( fi_AddLocate == 1 ) // 앞이 증가
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c%c%s%s"),
																 VolCode   ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 fi_pSameVolCode ,
																 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
			if ( fi_AddLocate == 2 ) // 뒤가 증가
			{
				_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%c%s"),
																 fi_pSameVolCode                      ,
																 ( fi_BraceHypen_Flag == 1 ) ? '(' : '-' ,
																 VolCode                              ,
				                                                 ( fi_BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
			}
		}

		// 복본기호
		_stprintf ( fo_pCopyCode + _tcsclen ( fo_pCopyCode ) , _T("%c#") , ' ' );
	}

	return 0;
}

//-------------------------------------------------------------------------------
// 복본기호만 있다. 또는  권책기호는 같고 복본기호는 증가됨
//-------------------------------------------------------------------------------
INT CPlusIN049Tag::Save_OnlyCopyCode ( TCHAR * fi_pAccessionNo1 ,
									   TCHAR * fi_pAccessionNo2 ,
									   TCHAR * fi_pVolCode      ,
									   TCHAR * fi_pCopyCode1    ,
									   TCHAR * fi_pCopyCode2    ,
		 							   TCHAR * fo_pAccessionNo  ,
									   TCHAR * fo_pVolCode      , 
									   TCHAR * fo_pCopyCode     )
{ 
	INT CopyCode , StartCopy , EndCopy ;
	INT i ;

	StartCopy = _ttoi ( fi_pCopyCode1 );
	EndCopy   = _ttoi ( fi_pCopyCode2 );

	if ( StartCopy == 0 ) 
		StartCopy = 1;

	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        EndCopy - StartCopy ) )
		return -1;								

	for ( CopyCode = StartCopy , i = 0 ; CopyCode <= EndCopy ; CopyCode++ , i++ )
	{
		// 등록번호
		_stprintf ( fo_pAccessionNo + _tcsclen (fo_pAccessionNo) , _T("%.2s%010u") , fi_pAccessionNo1  ,
	              _ttol(fi_pAccessionNo1+2) + i );
		// 권책기호
		_stprintf ( fo_pVolCode + _tcsclen(fo_pVolCode) , _T("%s#") ,
	              ( fi_pVolCode[0] != '\0' ) ? fi_pVolCode : _T(" ") );
		// 복본기호
		if ( CopyCode > 1 )
    		_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#") , CopyCode );
		else
    		_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%c#") , ' ' );
	}

	return 0;
}

//--------------------------------------------------------------------------
// 분리기호가 없는 상태에서 복본기호는 같고 권책기호가 증가된 상태
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_SameCopyCodeAddVolCode_NoBraceHypen ( TCHAR * fi_pVolCode1     ,
						                                       TCHAR * fi_pVolCode2     ,
												               TCHAR * fi_pCopyCode     ,
												               TCHAR * fi_pAccessionNo1 ,
												               TCHAR * fi_pAccessionNo2 ,
 															   TCHAR * fo_pAccessionNo  , 
															   TCHAR * fo_pVolCode      , 
															   TCHAR * fo_pCopyCode     )
{
	INT rtn1 , rtn2;
	INT Start_Vol , End_Vol , AddVolCode , i ;
	INT SameCopyCode , CopyCode;

	// 권책기호 , 복본기호 

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	if ( rtn1 == 1 )
	{
	   Start_Vol = _ttoi ( fi_pVolCode1 );
	   End_Vol   = _ttoi ( fi_pVolCode2 );
	}
	else 
	{
	   Start_Vol = fi_pVolCode1[0] ;
	   End_Vol   = fi_pVolCode2[0] ;
	}

	SameCopyCode = _ttoi ( fi_pCopyCode );

	// 개수 확인
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        ( End_Vol - Start_Vol) * SameCopyCode ) )
		return -1;								

	// 첫 책정보 저장

	_stprintf( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u"),
			 fi_pAccessionNo1  , _ttol( fi_pAccessionNo1 + 2 ) ) ;
	_stprintf( fo_pVolCode+_tcsclen(fo_pVolCode)   , _T("%s#") , fi_pVolCode1 );
	_stprintf( fo_pCopyCode+_tcsclen(fo_pCopyCode) , _T("%s#") , fi_pCopyCode );

	//-----------------------------------------------------------------------------------
	// 등록번호 , 복본기호 , 권책기호
	//-----------------------------------------------------------------------------------
	for ( AddVolCode = Start_Vol + 1 , i=1 ; AddVolCode <= End_Vol ; AddVolCode ++  )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode++ , i++ )
		{
			// 등록번호
			_stprintf ( fo_pAccessionNo + _tcsclen(fo_pAccessionNo) , _T("%.2s%010u") ,
				      fi_pAccessionNo1 ,
					  _ttol( fi_pAccessionNo1 + 2) + i );

			// 권책기호
			if ( rtn1 == 1 ) // 숫자
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%d#") , AddVolCode );
			else
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%c#") , AddVolCode );

			// 복본기호
			if ( CopyCode == 1 )
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" "));
			else
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"), CopyCode);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------
// 분리기호가 있는 상태에서 복본기호가 같고 권책기호가 
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_SameCopyCodeAddVolCode_ExistBraceHypen ( TCHAR * fi_pAccessionNo1,
												   TCHAR * fi_pAccessionNo2,
												   INT    BraceHypen_Flag , 
											       TCHAR * fi_pSameVolCode ,
												   INT    fi_AddLocate    ,
												   TCHAR * fi_pAddVolStart ,
												   TCHAR * fi_pAddVolEnd   ,
												   TCHAR * fi_pCopyCode    ,
 								                   TCHAR * fo_pAccessionNo , 
		                                           TCHAR * fo_pVolCode     , 
                                                   TCHAR * fo_pCopyCode    )
{
	INT rtn1 , rtn2         ;
	INT AddVolCode , Start_Vol , End_Vol ;
	INT CopyCode , SameCopyCode ;
	INT i;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pAddVolStart );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pAddVolEnd   );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	switch ( rtn1 )
	{
		case 1 :		// 숫자
				Start_Vol = _ttoi( fi_pAddVolStart ) ;
				End_Vol   = _ttoi( fi_pAddVolEnd )   ;
				break;

		case 2 :		// 영문
				Start_Vol = fi_pAddVolStart[0] ;
				End_Vol   = fi_pAddVolEnd[0]   ;
				break;

		default :
			return -1;
	}

	SameCopyCode = _ttoi( fi_pCopyCode );

	// 개수 확인
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        ( End_Vol - Start_Vol) * SameCopyCode ) )
		return -1;								

	for ( AddVolCode = Start_Vol + 1 , i = 1; AddVolCode <= End_Vol ; AddVolCode ++ )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode ++ , i++ )
		{
			// 등록번호
			_stprintf ( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u") , 
					  fi_pAccessionNo1 , 
					  _ttol(fi_pAccessionNo1+2) + i );

			// 권책기호
			if ( rtn1 == 1 )  // 숫자
			{
				if ( fi_AddLocate == 1 ) // 앞이 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // 뒤가 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%d%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}
			else if ( rtn1 == 2 )
			{
				if ( fi_AddLocate == 1 ) // 앞이 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // 뒤가 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%c%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}

			// 복본기호
			if ( CopyCode == 1 )
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" ") );
			else
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"),CopyCode );

		}
	}

	return 0;
}

//--------------------------------------------------------------------------
// 권책기호가 증가되고 복본기호는 뒤에만 있을 경우 && 분리기호 없음 
//--------------------------------------------------------------------------
INT CPlusIN049Tag::Save_AddVolCode2thCopyCodeExist_NoBraceHypen ( TCHAR * fi_pVolCode1      ,
																  TCHAR * fi_pVolCode2      ,
																  TCHAR * fi_pCopyCode      ,
																  TCHAR * fi_pAccessionNo1  ,
																  TCHAR * fi_pAccessionNo2  ,
 																  TCHAR * fo_pAccessionNo   , 
																  TCHAR * fo_pVolCode       , 
																  TCHAR * fo_pCopyCode      )
{
	INT rtn1 , rtn2;
	INT Start_Vol , End_Vol , AddVolCode , i ;
	INT SameCopyCode , CopyCode;

	// 권책기호 , 복본기호 

	rtn1 = Check_VolCode_Eng_Integer ( fi_pVolCode1 );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pVolCode2 );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	if ( rtn1 == 1 )
	{
	   Start_Vol = _ttoi ( fi_pVolCode1 );
	   End_Vol   = _ttoi ( fi_pVolCode2 );
	}
	else 
	{
	   Start_Vol = fi_pVolCode1[0] ;
	   End_Vol   = fi_pVolCode2[0] ;
	}

	SameCopyCode = _ttoi ( fi_pCopyCode );

	// 개수 확인
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        ( ( End_Vol - Start_Vol) + 1 ) * SameCopyCode - 1 ) )
		return -1;								

	//-----------------------------------------------------------------------------------
	// 등록번호 , 복본기호 , 권책기호
	//-----------------------------------------------------------------------------------
	for ( AddVolCode = Start_Vol , i=0 ; AddVolCode <= End_Vol ; AddVolCode ++  )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode++ , i++ )
		{
			// 등록번호
			_stprintf ( fo_pAccessionNo + _tcsclen(fo_pAccessionNo) , _T("%.2s%010u") ,
				      fi_pAccessionNo1 ,
					  _ttol( fi_pAccessionNo1 + 2) + i );

			// 권책기호
			if ( rtn1 == 1 ) // 숫자
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%d#") , AddVolCode );
			else
				_stprintf ( fo_pVolCode + _tcsclen( fo_pVolCode ) , _T("%c#") , AddVolCode );

			// 복본기호
			if ( CopyCode == 1 )
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" "));
			else
			    _stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"), CopyCode);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------
// 분리기호가 있는 상태에서 권책기호가 증가되고 복본기호는 뒤에만 있음
//--------------------------------------------------------------------------
INT  CPlusIN049Tag::Save_AddVolCode2thCopyCodeExist_ExistBraceHypen ( TCHAR * fi_pAccessionNo1,
													      TCHAR * fi_pAccessionNo2,
												          INT    BraceHypen_Flag , 
											              TCHAR * fi_pSameVolCode ,
												          INT    fi_AddLocate    ,
												          TCHAR * fi_pAddVolStart ,
												          TCHAR * fi_pAddVolEnd   ,
												          TCHAR * fi_pCopyCode    ,
 								                          TCHAR * fo_pAccessionNo , 
		                                                  TCHAR * fo_pVolCode     , 
                                                          TCHAR * fo_pCopyCode    )
{
	INT rtn1 , rtn2         ;
	INT AddVolCode , Start_Vol , End_Vol ;
	INT CopyCode , SameCopyCode ;
	INT i;

	rtn1 = Check_VolCode_Eng_Integer ( fi_pAddVolStart );
    rtn2 = Check_VolCode_Eng_Integer ( fi_pAddVolEnd   );

	if ( ( rtn1 < 0 || rtn2 < 0 ) || ( rtn1 != rtn2 ) )
		return -1;

	switch ( rtn1 )
	{
		case 1 :		// 숫자
				Start_Vol = _ttoi( fi_pAddVolStart ) ;
				End_Vol   = _ttoi( fi_pAddVolEnd )   ;
				break;

		case 2 :		// 영문
				Start_Vol = fi_pAddVolStart[0] ;
				End_Vol   = fi_pAddVolEnd[0]   ;
				break;

		default :
			return -1;
	}

	SameCopyCode = _ttoi( fi_pCopyCode );

	// 개수 확인
	if ( Check_AccessionNoCnt ( fi_pAccessionNo1 , fi_pAccessionNo2 ,
		                        (( End_Vol - Start_Vol) + 1 ) * SameCopyCode - 1 ) )
		return -1;								

	for ( AddVolCode = Start_Vol , i = 0; AddVolCode <= End_Vol ; AddVolCode ++ )
	{
		for ( CopyCode = 1 ; CopyCode <= SameCopyCode ; CopyCode ++ , i++ )
		{
			// 등록번호
			_stprintf ( fo_pAccessionNo+_tcsclen(fo_pAccessionNo) , _T("%.2s%010u") ,
					  fi_pAccessionNo1 ,
					  _ttol(fi_pAccessionNo1+2) + i );

			// 권책기호
			if ( rtn1 == 1 )  // 숫자
			{
				if ( fi_AddLocate == 1 ) // 앞이 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%d%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // 뒤가 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%d%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}
			else if ( rtn1 == 2 )
			{
				if ( fi_AddLocate == 1 ) // 앞이 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%c%c%s%s"),
																	 AddVolCode   ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 fi_pSameVolCode ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
				if ( fi_AddLocate == 2 ) // 뒤가 증가
				{
					_stprintf ( fo_pVolCode + _tcsclen ( fo_pVolCode ) , _T("%s%c%c%s"),
																	 fi_pSameVolCode                      ,
																	 ( BraceHypen_Flag == 1 ) ? '(' : '-' ,
																	 AddVolCode                           ,
																	 ( BraceHypen_Flag == 1 ) ? _T(")#") : _T("#") );
				}
			}

			// 복본기호
			if ( CopyCode == 1 )
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%s#"), _T(" ") );
			else
				_stprintf ( fo_pCopyCode + _tcsclen(fo_pCopyCode) , _T("%d#"),CopyCode );
		}
	}

	return 0;
}

//===============================================================================
// 등록번호의 중복을 확인한다.
// - fo_psDupAccno_List 에 ' '로 구분지어 저장된다.
//===============================================================================
INT CPlusIN049Tag::Dup_AccessionNo ( TCHAR * fi_pAccessionNo    ,
								     TCHAR * fo_psDupAccno_List /*=NULL*/)
{
	INT  nAccessionCnt           ;
	INT  nSameAccessionCnt = 0   ;
	BOOL DupAccession_Exist = FALSE ;

	if ( fo_psDupAccno_List ) fo_psDupAccno_List[0] = '\0' ;

	nAccessionCnt = _tcsclen ( fi_pAccessionNo ) / 12 ;
	//-----------------------------------------------------------------
	//  리스트 출력
	//-----------------------------------------------------------------
	for ( INT i=0 , j ; i < nAccessionCnt - 1 ; i++ )
	{
		for ( j = i+1 , nSameAccessionCnt=1 ; j < nAccessionCnt ; j++ )
		{
			if ( !_tcsncmp ( fi_pAccessionNo + (i*12) ,
				            fi_pAccessionNo + (j*12) ,
							12 ) )
			{
				nSameAccessionCnt++ ;
			}
		}

		if ( nSameAccessionCnt > 1 )
		{
			DupAccession_Exist = TRUE ;
			if ( fo_psDupAccno_List )
			{
				if ( fo_psDupAccno_List[0] != '\0' )
					_tcscat ( fo_psDupAccno_List , _T(" ") ) ;
				_stprintf ( fo_psDupAccno_List+_tcsclen(fo_psDupAccno_List), _T("%-12.12s") , fi_pAccessionNo+(i*12) );
			}
		}
	}

	if ( DupAccession_Exist )
		return -1;

	return 0;
}

/*
	INT nAccessionCnt           ;
	INT nSameAccessionCnt = 0   ;

	fo_pDup_AccnoList[0] = '\0' ;

	nAccessionCnt = _tcsclen ( fi_pAccessionNoList ) / 12 ;
	//-----------------------------------------------------------------
	//  리스트 출력
	//-----------------------------------------------------------------
	for ( INT i=0 , j ; i < nAccessionCnt - 1 ; i++ )
	{
		for ( j = i+1 , nSameAccessionCnt=1 ; j < nAccessionCnt ; j++ )
		{
			if ( !_tcsncmp ( fi_pAccessionNoList + (i*12) ,
				            fi_pAccessionNoList + (j*12) ,
							12 ) )
			{
				nSameAccessionCnt++ ;
			}
		}

		if ( nSameAccessionCnt > 1 )
		{
			_ftprintf ( put_fp , _T("%s")         , fi_pControlNo              );
			_ftprintf ( put_fp , _T("  %-12.12s") , fi_pAccessionNoList+(i*12) );
			_ftprintf ( put_fp , _T("  %d")       , nSameAccessionCnt          );
			_ftprintf ( put_fp , _T("  %s\n")     , fi_pColNumber              );

			if ( fo_pDup_AccnoList[0] != '\0' )
				_tcscat ( fo_pDup_AccnoList , _T(" ") ) ;
			_stprintf ( fo_pDup_AccnoList+_tcsclen(fo_pDup_AccnoList), _T("%-12.12s") , fi_pAccessionNoList+(i*12) );
		}
	}
	_fputts ( _T("\n") , put_fp );

*/




