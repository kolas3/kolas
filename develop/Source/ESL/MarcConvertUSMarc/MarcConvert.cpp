// MarcConvert.cpp: implementation of the CMarcConvert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcConvert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define ALA_CHAR_COUNT 29

typedef struct _ALA_CONVERT_KS5601{
    _TUCHAR  AlaChar;
    _TUCHAR  s_HighChar;
    _TUCHAR  s_LowChar;
}tALA_CONVERT_KS5601;

tALA_CONVERT_KS5601 AlaConvertKS5601[ALA_CHAR_COUNT] = {
    { 0xE0 , 0xA8 , 0xE7 } ,
    { 0xE1 , 0xA3 , 0xE0 } ,
    { 0xE2 , 0xA2 , 0xA5 } ,
    { 0xE3 , 0xA3 , 0xDE } ,
    { 0xE4 , 0xA2 , 0xA6 } ,
    { 0xE5 , 0xA3 , 0xFE } ,
    { 0xE6 , 0xA2 , 0xA8 } ,
    { 0xE7 , 0xA2 , 0xAB } ,
    { 0xE8 , 0xA1 , 0xA7 } ,
    { 0xE9 , 0xA2 , 0xA7 } ,
    { 0xEA , 0xA2 , 0xAA } ,
    { 0xEB , 0xA8 , 0xE8 } ,
    { 0xEC , 0xA8 , 0xE9 } ,
    { 0xED , 0xA8 , 0xEA } ,
    { 0xEE , 0xA2 , 0xA9 } ,
    { 0xEF , 0xA8 , 0xEB } ,
    { 0xF0 , 0xA2 , 0xAC } ,
    { 0xF1 , 0xA8 , 0xEC } ,
    { 0xF2 , 0xA3 , 0xAE } ,
    { 0xF3 , 0xA1 , 0xA5 } ,
    { 0xF4 , 0xA1 , 0xA3 } ,
    { 0xF5 , 0xA8 , 0xED } ,
    { 0xF6 , 0xA3 , 0xDF } ,
    { 0xF7 , 0xA8 , 0xEE } ,
    { 0xF8 , 0xA2 , 0xAD } ,
    { 0xF9 , 0xA8 , 0xEF } ,
    { 0xFA , 0xA8 , 0xF0 } ,
    { 0xFB , 0xA8 , 0xF1 } ,
    { 0xFE , 0xA8 , 0xF2 }    
};

static TCHAR s_FieldData[30000];
static TCHAR s_MarcData[100000];
static TCHAR s_FieldData2[30000];

#define     SPECIAL_CHAR_COUNT 27

typedef struct _SPECIAL_CHAR_STRU {
    TCHAR          Comment[64];
    _TUCHAR s_FirstChar;
    _TUCHAR s_SecondChar;
    _TUCHAR e_FirstChar;
    _TUCHAR e_SecondChar;
} tSPECIAL_CHAR_STRU;

tSPECIAL_CHAR_STRU s_DelCharInfo[SPECIAL_CHAR_COUNT] = {
    { _T("한글")       , 0xB0 , 0xA1 , 0xC8 , 0xFE },
    { _T("한자")       , 0xCA , 0xA1 , 0xFD , 0xFE },
    { _T("일어")       , 0xAA , 0xA1 , 0xAA , 0xF3 },
    { _T("일어")       , 0xAB , 0xA1 , 0xAB , 0xF6 },
    { _T("그리스어")   , 0xA5 , 0xC1 , 0xA5 , 0xD8 },
    { _T("그리스어")   , 0xA5 , 0xE1 , 0xA5 , 0xF8 },
    { _T("라틴어")     , 0xA8 , 0xA1 , 0xA8 , 0xA4 },
    { _T("라틴어")     , 0xA8 , 0xA6 , 0xA8 , 0xA6 },
    { _T("라틴어")     , 0xA8 , 0xA8 , 0xA8 , 0xAF },
    { _T("라틴어")     , 0xA9 , 0xA1 , 0xA9 , 0xAF },  // AA => A1
    { _T("러시아어")   , 0xAC , 0xA1 , 0xAC , 0xC1 },
    { _T("러시아어")   , 0xAC , 0xD1 , 0xAC , 0xF1 },
    { _T("한글자모")   , 0xA4 , 0xA1 , 0xA4 , 0xD3 },
    { _T("한글고어1")  , 0xA4 , 0xD5 , 0xA4 , 0xFC }, // A4FD -> 고어 'ㆍ' 문자는 삭제대상이다. 2003.10.28 적용
    { _T("한글고어2")  , 0xA4 , 0xFE , 0xA4 , 0xFE },
    { _T("숫자")       , 0xA3 , 0xB0 , 0xA3 , 0xB9 },
    { _T("영문대문자") , 0xA3 , 0xC1 , 0xA3 , 0xDA },
    { _T("영문소문자") , 0xA3 , 0xE1 , 0xA3 , 0xFA },
	{ _T("그림문자")   , 0xA1 , 0xA1 , 0xA1 , 0xFE }, // 그림문자1
    { _T("그림문자")   , 0xA2 , 0xA1 , 0xA2 , 0xE5 }, // 그림문자2
    { _T("그림문자")   , 0xA3 , 0xA1 , 0xA3 , 0xFE }, // 그림문자3
    { _T("그림문자")   , 0xA5 , 0xA1 , 0xA5 , 0xAA }, // 그림문자4 
    { _T("그림문자")   , 0xA5 , 0xB0 , 0xA5 , 0xB9 }, // 그림문자5
    { _T("그림문자")   , 0xA7 , 0xA1 , 0xA7 , 0xFE }, // 그림문자6
    { _T("그림문자")   , 0xA8 , 0xB1 , 0xA8 , 0xFE }, // 그림문자7
    { _T("그림문자")   , 0xA9 , 0xB0 , 0xA9 , 0xFE }, // 그림문자8
    { _T("한글확장")   , 0x81 , 0x20 , 0xC5 , 0xFF }  // 확장한글 => 한글확장    
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcConvert::CMarcConvert()
{
    m_MarcClass = 'M';
    m_FormClass[0] = '\0';
}

CMarcConvert::~CMarcConvert()
{

}

INT      CMarcConvert::ConvertUSMarcToKormarc( TCHAR *f_pUsMarc , TCHAR *f_pKorMarc )
{
    

    tMARC_STRU s_MarcStru,s_tmpMarcstru;
    TCHAR       s_Leader[25];
    INT        ids;

    if( f_pUsMarc == NULL || f_pKorMarc == NULL ) return -1;

    // 1.LEADER를 확인하여 MARC를 구분한다.
    _tcsnccpy( s_Leader , f_pUsMarc , 24 );
    s_Leader[24] = '\0';

    if( s_Leader[7] == 'b' || s_Leader[7] == 's' ){
        m_MarcClass = 'S';
    }
    else{
        switch( s_Leader[6] ){
        // 도서
        case 'a' :
            m_MarcClass = 'M';
            break;

        // 비도서(컴퓨터파일)
        case 'm' :
            m_MarcClass = 'N';
            _tcscpy( m_FormClass , _T("CF"));
            break;

        // 비도서(지도자료)
        case 'e' :
        case 'f' :
        case 'h' :
            m_MarcClass = 'N';
            _tcscpy( m_FormClass , _T("MP"));
            break;

        // 비도서(녹음자료)
        case 'c' :
        case 'd' :
        case 'i' :
        case 'j' :
            m_MarcClass = 'N';
            _tcscpy( m_FormClass , _T("MU"));
            break;

        // 비도서(시청각자료)
        case 'g' :
        case 'k' :
        case 'o' :
        case 'r' :
            m_MarcClass = 'N';
            _tcscpy( m_FormClass , _T("VM"));
            break;

        // ERROR
        default :
            break;
        }

    }

    ids = ReadMarcStruFromStreamMarc( f_pUsMarc , &s_MarcStru );
    if( ids ) return ids;

    ids = ReadMarcStruFromStreamMarc( f_pUsMarc , &s_tmpMarcstru );
    if( ids ) {
        FreeMarcStru( s_MarcStru );
        return ids;
    }

    ids = ExcuteConvertFunc( &s_MarcStru , &s_tmpMarcstru );
    if( ids ) {
        FreeMarcStru( s_tmpMarcstru );
        FreeMarcStru( s_MarcStru );
        return ids;
    }

    ids = ConvertUSLeaderToKorLeader( &s_MarcStru );
    if( ids ) {
        FreeMarcStru( s_tmpMarcstru );
        FreeMarcStru( s_MarcStru );
        return ids;
    }

    ids = WriteMarcStruToStreamMarc( s_MarcStru, f_pKorMarc );
    FreeMarcStru( s_tmpMarcstru );
    FreeMarcStru( s_MarcStru );
    if( ids ) return ids;

    // OCLC특수문자를 변환한다.
    ids = ConvertALACharToKS5601Char( f_pKorMarc , f_pKorMarc );
    if( ids ) return ids;

    return 0;

    
}

INT      CMarcConvert::ExcuteConvertFunc( tMARC_STRU *f_pMarcStru , tMARC_STRU *f_ptmpMarcStru )
{
    

    TCHAR s_SubFieldCode[3];
    INT  i,ids;


    for( i = 0; i < f_ptmpMarcStru->MarcFieldCount; i++){
        
                      
        switch( f_ptmpMarcStru->pMarcField[i].TagNo ){
 
        
        case 3  :
        case 18 :
        case 25 :
        case 32 :
        case 36 :
        case 49 :
        case 51 :        
        case 55 :
        case 61 :
        case 66 :
        case 70 :
        case 71 :
        case 72 :
        case 84 :
        case 85 :
        case 242 :
        case 243 :
        case 261 :
        case 262 :
        case 263 :
        case 357 :
        case 400 :
        case 410 :
        case 411 :
        case 507 :
        case 513 :
        case 522 :
		case 530 :
        case 562 :
        case 567 :
        case 584 :
        case 654 :
        case 655 :
        case 656 :
        case 657 :
        case 658 :
        case 752 :
        case 754 :
        case 755 :
        case 800 :
        case 810 :
        case 811 :
        case 841 :
        case 843 :
        case 845 :
        case 852 :
        case 853 :
        case 854 :
        case 855 :
        case 863 :
        case 864 :
        case 865 :
        case 866 :
        case 867 :
        case 868 :
        case 876 :
        case 877 :
        case 878 :
        case 880 :
        case 886 :
           
            ids = RemoveField( '1'                                 ,
                               f_ptmpMarcStru->pMarcField[i].TagNo ,
                               f_pMarcStru                         );
            if( ids ) return  ids;
            break;

        case 5 :
            ids = Convert005_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 7 :
            ids = Convert007_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 8 :
            ids = Convert008_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 10 :
            ids = Convert010_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;
		
		case 22 :
			ids = Convert022_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
			break;

        case 28 :
            ids = Convert028_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 33 :
            ids = Convert033_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 50 :
            ids = Convert050_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 60 :
            ids = Convert060_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 82 :
            ids = Convert082_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 86 :
            ids = Convert086_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 100 :
            ids = Convert100_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 110 :
            ids = Convert110_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 111 :
            ids = Convert111_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 130 :
            ids = Convert130_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 222 :
            ids = Convert222_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 240 :
            ids = Convert240_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 245 :
            ids = Convert245_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 246 :
            ids = Convert246_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;
          
        case 247 :
            ids = Convert247_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 300 :
            ids = Convert300_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;
        
        case 340 :
            ids = Convert340_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 351 :
            ids = Convert351_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 355 :
            ids = Convert355_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 440 :
            ids = Convert440_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 490 :
            ids = Convert490_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 500 :
            ids = Convert500_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 502 :
            ids = Convert502_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 504 :
            ids = Convert504_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 505 :
            ids = Convert505_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 506 :
            ids = Convert506_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 510 :
            ids = Convert510_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 511 :
            ids = Convert511_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 516 :
            ids = Convert516_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 518 :
            ids = Convert518_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 520 :
            ids = Convert520_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 521 :
            ids = Convert521_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 524 :
            ids = Convert524_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

			// 2005.07.18 REM BY PDJ
			/* - K2UP에는 없음. 통합마크(비도서)에만 있음
        case 530 :
            ids = Convert530_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;
			*/

        case 533 :
            ids = Convert533_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 535 :
            ids = Convert535_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 536 :
            ids = Convert536_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 540 :
            ids = Convert540_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 541 :
            ids = Convert541_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 544 :
            ids = Convert544_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 546 :
            ids = Convert546_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 550 :
            ids = Convert550_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 555 :
            ids = Convert555_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 561 :
            ids = Convert561_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 565 :
            ids = Convert565_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 581 :
            ids = Convert581_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 583 :
            ids = Convert583_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 585 :
            ids = Convert585_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 586 :
            ids = Convert586_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 600 :
            ids = Convert600_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 610 :
            ids = Convert610_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 611 :
            ids = Convert611_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 630 :
            ids = Convert630_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 650 :
            ids = Convert650_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 651 :
            ids = Convert651_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 653 :
            ids = Convert653_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 700 :
            ids = Convert700_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 710 :
            ids = Convert710_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 711 :
            ids = Convert711_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 730 :
            ids = Convert730_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 740 :
            ids = Convert740_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 830 :
            ids = Convert830_USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;

        case 350 :
        case 950 :
            ids = Convert350_950USMarcToKormarc( f_pMarcStru );
            if( ids ) return ids;
            break;
        }

        // 모든 $6,$8을 삭제한다.
        _stprintf( s_SubFieldCode ,_T("%c6"),GT);
        ids = RemoveElement( f_pMarcStru->pMarcField[i].TagNo ,
                             s_SubFieldCode                 ,
                             f_pMarcStru                    );
        if( ids ) return ids;
    
        _stprintf( s_SubFieldCode ,_T("%c8"),GT);
        ids = RemoveElement( f_pMarcStru->pMarcField[i].TagNo ,
                             s_SubFieldCode                 ,
                             f_pMarcStru                    );
        if( ids ) return ids;
    }

    TCHAR   s_SubFieldData[1024];
    TCHAR **s_pSubFieldData;
    TCHAR   s_PuncMark;
    INT    j,s_SubFieldCount;

    s_SubFieldData[0] = '\0';
      

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo <= 10 ) continue;

        if( f_pMarcStru->pMarcField[i].TagNo >= 245 && 
            f_pMarcStru->pMarcField[i].TagNo <= 490 ){
            if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)-1] == '.'){
                f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)-1] = '\0';
            }
        }

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount; j++){
            s_PuncMark = s_pSubFieldData[j][_tcsclen(s_pSubFieldData[j])-1];
            if( s_PuncMark == ';' || s_PuncMark == ':' ||
                s_PuncMark == ',' || s_PuncMark == '.' ||
                s_PuncMark == '/' || s_PuncMark == '+' ||
                s_PuncMark == '=' || s_PuncMark == '-' ){
                if( s_pSubFieldData[j][_tcsclen(s_pSubFieldData[j])-2] == ' '){
                    _tcsnccpy( s_SubFieldData , &s_pSubFieldData[j][2], _tcsclen(s_pSubFieldData[j])- 4);
                    s_SubFieldData[_tcsclen(s_pSubFieldData[j])- 4] = '\0';
                    
                    _stprintf( &s_SubFieldData[_tcsclen(s_SubFieldData)] ,_T("%c"),s_PuncMark);
                    _tcsnccpy( s_SubFieldCode , s_pSubFieldData[j] , 2);
                    s_SubFieldCode[2] = '\0';

                    ids = UpdateSubFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                                        s_SubFieldCode                   ,
                                                       &s_pSubFieldData[j][2]            ,
                                                        s_SubFieldData                   ,
                                                        f_pMarcStru                      );
                    if( ids ) {
                        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                        return ids;
                    }
                }                
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }
    return 0;

    
}

INT      CMarcConvert::ConvertUSLeaderToKorLeader( tMARC_STRU *f_pMarcStru )
{
    

    // 1.레코드 형태
    if( f_pMarcStru->MarcLeader.RecStatus == 'p'){
	    f_pMarcStru->MarcLeader.RecStatus = 'o';
    }else
    if( f_pMarcStru->MarcLeader.RecStatus == 't'){
	    f_pMarcStru->MarcLeader.RecStatus = 'a';
    }

    // 2.서지수준
    if( f_pMarcStru->MarcLeader.SeoJiLevel == 'd'){
	    f_pMarcStru->MarcLeader.SeoJiLevel = 'c';
    }

    // 3.통제유형
    if( f_pMarcStru->MarcLeader.Control_type == 'a'){
	    f_pMarcStru->MarcLeader.Control_type = ' ';
    }
    
    // 4.입력수준
    if( f_pMarcStru->MarcLeader.InputLevel == '7' || 
        f_pMarcStru->MarcLeader.InputLevel == '3' ||
        f_pMarcStru->MarcLeader.InputLevel == '4' ){
        f_pMarcStru->MarcLeader.InputLevel = '2';
    }else
    if( f_pMarcStru->MarcLeader.InputLevel == '8' ){
        f_pMarcStru->MarcLeader.InputLevel = '3';
    }else
    if( f_pMarcStru->MarcLeader.InputLevel == '5' ||
        f_pMarcStru->MarcLeader.InputLevel == 'z' ){
        f_pMarcStru->MarcLeader.InputLevel = 'u';
    }

    f_pMarcStru->MarcLeader.MokRok_WriteType = 'u';

//    if( f_pMarcStru->MarcLeader.MokRok_WriteType != 'k' ){
//		f_pMarcStru->MarcLeader.MokRok_WriteType = 'u';
//	}
//	else{
//		return 100;
//
//	}

    return 0;

    
}

INT      CMarcConvert::Convert005_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR s_FieldData[128];
    INT  i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 5 ) continue;
        _tcsnccpy( s_FieldData , f_pMarcStru->pMarcField[i].pFldData_ElementStr ,14 );
        s_FieldData[14] = '\0';

        ids= UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo               ,
                                        f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                        s_FieldData                                    ,
                                        f_pMarcStru                                    );  
        if( ids ) return ids;
        break;
    }

    return 0;

    
}

INT      CMarcConvert::Convert007_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR s_FieldData[128];
    INT  i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 7 ) continue;
        _tcscpy( s_FieldData , f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( s_FieldData[0] == 'r' ){
            ids = RemoveField( 'A'                               ,
                                f_pMarcStru->pMarcField[i].TagNo ,
                                f_pMarcStru                      );
            if( ids ) return ids;
        }else
        if( s_FieldData[0] == 'a' ){
            if( s_FieldData[6] == 'c' ){
                s_FieldData[6] = 'z';
                ids= UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo               ,
                                                f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                s_FieldData                                    ,
                                                f_pMarcStru                                    );
                if( ids ) return ids;
            }
        }else
        if( s_FieldData[0] == 'm' ){
            if( s_FieldData[15] == 'b' || s_FieldData[15] == 'c' || 
                s_FieldData[15] == 'd' || s_FieldData[15] == 'e' || 
                s_FieldData[15] == 'f' || s_FieldData[15] == 'g' || 
                s_FieldData[15] == 'h' ){
                s_FieldData[15] = 'b';
            }else
            if( s_FieldData[15] == 'k' || s_FieldData[15] == 'l' || 
                s_FieldData[15] == 'm' ){
                s_FieldData[15] = 'k';
            }
            else{
                break;
            }

            ids= UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo               ,
                                            f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                            s_FieldData                                    ,
                                            f_pMarcStru                                    );
            if( ids ) return ids;
        }
        break;
    }

    return 0;

    
}

INT      CMarcConvert::Convert008_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR s_FieldData[128];
    INT  i,j,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 8 ) continue;
        _tcscpy( s_FieldData , f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        
        // 1.008의 17번째 발행국명('u'->_T("us "), 'k'->_T("uk ") , 'c'->_T("cn ")로 변경)
        if( s_FieldData[17] == 'u'){
            _tcsnccpy( &s_FieldData[15] , _T("us "), 3 );
        }else
        if( s_FieldData[17] == 'k'){
            _tcsnccpy( &s_FieldData[15] , _T("uk "), 3 );
        }else
        if( s_FieldData[17] == 'c'){
            _tcsnccpy( &s_FieldData[15] , _T("cn "), 3 );
        }
        

        // 2.008의 38번째 수정레코드를 28번째로 변경( 'd'->'#', 'o'->'r', 'x'->'b'로 변경)
        if( s_FieldData[38] == 'd' ){
            s_FieldData[28] = ' ';
        }else
        if( s_FieldData[38] == 'o' ){
            s_FieldData[28] = 'r';
        }else
        if( s_FieldData[38] == 'x' ){
            s_FieldData[28] = 'b';
        }else
        if( s_FieldData[38] == ' ' || s_FieldData[38] == 's' || s_FieldData[38] == 'r'){
            s_FieldData[28] = s_FieldData[38];
        }
        

        // 3.008의 32번째 목록전거(' '로 변경)
        s_FieldData[32] = ' ';

        // 4.008의 38~39번째 정부기관부호변경
        s_FieldData[38] = ' ';
        s_FieldData[39] = ' ';


        // 5.각 MARC에 따라 다르게 변경한다.
        switch( m_MarcClass ){
        // 5.1.도서
        case 'M' :

            // 5.1.1.한국대학부호(26~27)
            s_FieldData[26] = ' ';
            s_FieldData[27] = ' ';
            
            // 5.1.2.삽도(18~21)
            for( j = 0; j < 3 ; j++){
                if( s_FieldData[18+j] == 'o' || s_FieldData[18+j] == 'p' ){
                    s_FieldData[18+j] = 'a';
                }
            }

            // 5.1.3.이용대상자수준
            if( s_FieldData[22] == 'j' ){
                s_FieldData[22] = 'a';
            }

            // 5.1.4.문학형식
            if( s_FieldData[33] == '0' ){
                s_FieldData[33] = ' ';
            }else
            if( s_FieldData[33] == '1' ){
                s_FieldData[33] = 'f';
            }

            break;

        // 5.2.비도서
        case 'N' :

            // 5.2.1.컴퓨터파일
            if( _tcscmp( m_FormClass , _T("CF") ) == 0 ){

                // 5.2.1.1.한국대학부호(26~27)
                s_FieldData[25] = s_FieldData[26];
                s_FieldData[26] = ' ';
                s_FieldData[27] = ' ';
                
                // 5.2.1.2.빈칸(18~21)
                s_FieldData[18] = ' ';
                s_FieldData[19] = ' ';
                s_FieldData[20] = ' ';
                s_FieldData[21] = ' ';

                // 5.2.1.3.이용대상자수준
                if( s_FieldData[22] == ' ' ){
                    s_FieldData[22] = 'z';
                }else
                if( s_FieldData[22] == 'g' ){
                    s_FieldData[22] = ' ';
                }

                // 5.2.1.4.빈칸(29~31,33~34)
                s_FieldData[29] = ' ';
                s_FieldData[30] = ' ';
                s_FieldData[31] = ' ';

                s_FieldData[33] = ' ';
                s_FieldData[34] = ' ';

            }else

            // 5.2.2.지도자료
            if( _tcscmp( m_FormClass , _T("MP") ) == 0 ){

                // 5.2.2.1.한국대학부호(26~27)
                s_FieldData[26] = ' ';
                s_FieldData[27] = ' ';

                // 5.2.2.2.모형도
                for( j = 0; j < 4 ; j++){
                    if( s_FieldData[18+j] == 'k' ){
                        s_FieldData[18+j] = 'e';
                    }
                }

                // 5.2.2.3.빈칸
                s_FieldData[29] = ' ';
                s_FieldData[30] = ' ';

            }else

            // 5.2.3.녹음자료
            if( _tcscmp( m_FormClass , _T("MU") ) == 0 ){

                // 5.2.3.1.한국대학부호(26~27)
                s_FieldData[26] = ' ';
                s_FieldData[27] = ' ';

                // 5.2.3.2.빈칸
                s_FieldData[21] = ' ';

                // 5.2.3.3.빈칸
                s_FieldData[29] = ' ';

                // 5.2.3.4.빈칸
                s_FieldData[33] = ' ';
                s_FieldData[34] = ' ';

            }else

            // 5.2.4.시청각자료
            if( _tcscmp( m_FormClass , _T("VM") ) == 0 ){

                // 5.2.4.1.한국대학부호(26~27)
                s_FieldData[26] = ' ';
                s_FieldData[27] = ' ';

                // 5.2.4.2.이용대상자수준
                if( s_FieldData[22] == ' ' ){
                    s_FieldData[22] = 'z';
                }else
                if( s_FieldData[22] == 'g' ){
                    s_FieldData[22] = ' ';
                }
            }
            // 5.2.5.ERROR
            else{

            }

            break;

        // 5.3.연속간행자료
        case 'S' :

            // 5.3.1.한국대학부호(26~27)
            s_FieldData[26] = ' ';
            s_FieldData[27] = ' ';

            // 5.3.2.빈칸
            s_FieldData[21] = ' ';
            

            // 5.3.3.이용대상자수준
            if( s_FieldData[22] == ' ' ){
                s_FieldData[22] = 'z';
            }else
            if( s_FieldData[22] == 'g' ){
                s_FieldData[22] = ' ';
            }

            // 5.3.4.빈칸
            s_FieldData[29] = ' ';
            s_FieldData[30] = ' ';
            s_FieldData[31] = ' ';
            

            break;

        default :
            break;
        }


        ids= UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo               ,
                                        f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                        s_FieldData                                    ,
                                        f_pMarcStru                                    );  
        if( ids ) return ids;
        break;
    }

    return 0;

    
}

// $b를 삭제한다.
INT      CMarcConvert::Convert010_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR s_SubFieldCode[3];
    INT  ids;

    _stprintf( s_SubFieldCode , _T("%cb") , GT );
    ids = RemoveElement( 10             ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    
    return 0;

    
}

// $제1,2지시기호를 변경
// blank로 변경
INT      CMarcConvert::Convert022_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    INT  i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 22 ) continue;        
		f_pMarcStru->pMarcField[i].fIndicator = ' ';        
		f_pMarcStru->pMarcField[i].fIndicator = ' ';        
    }
    
    return 0;    
}

// $제2지시기호를 변경
// 0,3 -> 0  1,2->1로 변환.
INT      CMarcConvert::Convert028_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    INT  i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 28 ) continue;
        if( f_pMarcStru->pMarcField[i].fIndicator == '3' ){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }else
        if( f_pMarcStru->pMarcField[i].fIndicator == '2' ){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }
    }
    
    return 0;

    
}


/*==========================================================================
    제2지시시호 변경 : '0,'1','2' -> '#'.
    식별기호 $c,$3은 삭제
==========================================================================*/
INT      CMarcConvert::Convert033_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR s_SubFieldCode[3];
    INT  i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 33 ) continue;
        if( f_pMarcStru->pMarcField[i].sIndicator == '0' ||
            f_pMarcStru->pMarcField[i].sIndicator == '1' ||
            f_pMarcStru->pMarcField[i].sIndicator == '2' ){
            f_pMarcStru->pMarcField[i].sIndicator = ' ';
        }
    }

    _stprintf( s_SubFieldCode , _T("%cc") , GT );
    ids = RemoveElement( 30             ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 30             ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    
    return 0;

    
}

/*==========================================================================
    식별기호변경     : $a  -> $l로 변경
==========================================================================*/
INT      CMarcConvert::Convert049_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 49 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == ' '){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a'){
                s_pSubFieldData[j][1] = 'l';
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
            else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
                
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );

    }

    return 0;

    
}

/*==========================================================================
    제1지시시호,제2지시시호 변경 : '##'
    식별기호 $3은 삭제
==========================================================================*/
INT      CMarcConvert::Convert050_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 50 ) continue;
        
        f_pMarcStru->pMarcField[i].fIndicator = ' ';
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 50             ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;


    return 0;

    
}

/*==========================================================================
    $a의 DATA를 가지고 $2에 _T("NLM")을 추가한 FieldData를 085에 추가한다.
    TAG은 삭제
==========================================================================*/
INT      CMarcConvert::Convert060_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
    
    
    TCHAR  s_SubFieldCode[3],s_SubFieldData[1024],s_FieldData[1024];
    INT   i,ids;

    _stprintf( s_SubFieldCode ,_T("%ca"), GT );

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 60 ) continue;
        
        ids = GetSubFieldDataFromDataField ( s_SubFieldCode                                , 
                                             f_pMarcStru->pMarcField[i].pFldData_ElementStr,
                                             s_SubFieldData                                );
        if( ids ) return ids;

        _stprintf( s_FieldData ,_T("  %s%s%c2NLM") , s_SubFieldCode , s_SubFieldData , GT );
        
        ids = InsertFieldDataToMarcStru( 85          ,
                                         s_FieldData ,
                                         f_pMarcStru );
        if( ids < 0 ) return ids;
    }

    ids = RemoveField( 'A'          ,
                        60          ,
                        f_pMarcStru );
    if( ids ) return ids;

    return 0;    
}

/*==========================================================================
    제1지시기호 변경 : '0','1' -> ' '
    제2지시기호 변경 : '0','4' -> ' '
    식별기호 삭제    :  $b를 삭제
==========================================================================*/
INT      CMarcConvert::Convert082_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3];
    INT    i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 82 ) continue;
        
        if( f_pMarcStru->pMarcField[i].fIndicator == '0' ||
            f_pMarcStru->pMarcField[i].fIndicator == '1' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == '0' ||
            f_pMarcStru->pMarcField[i].sIndicator == '4' ){
            f_pMarcStru->pMarcField[i].sIndicator = ' ';
        }
    }

    _stprintf( s_SubFieldCode , _T("%cb") , GT );
    ids = RemoveElement( 82             ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : '0','1' -> ' '
==========================================================================*/
INT      CMarcConvert::Convert086_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    INT   i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 86 ) continue;
        
        f_pMarcStru->pMarcField[i].fIndicator = ' ';
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : '2' -> '1'
    식별기호 삭제    :  $f,$g,$k,$t,$u,$4를 삭제
==========================================================================*/
INT      CMarcConvert::Convert100_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 100 ) continue;
        
        if( f_pMarcStru->pMarcField[i].fIndicator == '2' ){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }
    }

    _tcscpy( s_tmpbur , _T("fgktu4"));
    for( i = 0; i < 6; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 100            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' '
    식별기호 삭제    :  $f,$t,$u,$4를 삭제
==========================================================================*/
INT      CMarcConvert::Convert110_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[5];
    INT   i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 110 ) continue;

        f_pMarcStru->pMarcField[i].fIndicator = ' ';
    }

    _tcscpy( s_tmpbur , _T("ftu4"));
    for( i = 0; i < 4; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 110            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' '
    식별기호 삭제    :  $f,$k,$q,$t,$u,$4를 삭제
==========================================================================*/
INT      CMarcConvert::Convert111_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[7];
    INT   i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 111 ) continue;

        f_pMarcStru->pMarcField[i].fIndicator = ' ';
    }

    _tcscpy( s_tmpbur , _T("fkqtu4"));
    for( i = 0; i < 6; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 111            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' '
    제2지시기호 변경 : 제1지시기호->제2지시기호로변환.
    식별기호 삭제    :  $d,$t를 삭제
==========================================================================*/
INT      CMarcConvert::Convert130_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[6],s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

    s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 130 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].sIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].sIndicator - 48;
        }
        
        f_pMarcStru->pMarcField[i].sIndicator = f_pMarcStru->pMarcField[i].fIndicator;
        if( f_pMarcStru->pMarcField[i].sIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].sIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
        else{
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
        f_pMarcStru->pMarcField[i].fIndicator = ' ';
        
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a' ){
                if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca("), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(")%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("dt"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 130            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제2지시기호 변경 : 1~9 -> '1'로변환.
==========================================================================*/
INT      CMarcConvert::Convert222_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

    s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 222 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].sIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].sIndicator - 48;
        }
        
        if( f_pMarcStru->pMarcField[i].sIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].sIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
        else{
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a' ){
                if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca("), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(")%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    
    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' '
    제2지시기호 변경 : 제1지시기호->제2지시기호로변환.
    식별기호 삭제    : $m,$o,$r를 삭제
==========================================================================*/
INT      CMarcConvert::Convert240_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[4],s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

	s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 240 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].sIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].sIndicator - 48;
        }
        
        if( f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            f_pMarcStru->pMarcField[i].sIndicator = ' ';
        }
        
        if( f_pMarcStru->pMarcField[i].sIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].sIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }

        f_pMarcStru->pMarcField[i].fIndicator = ' ';
        
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a' ){
                if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca"), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
				else{
					_tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
				}
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("mor"));
    for( i = 0; i < 3; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 240            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제2지시기호 변경 : 1~9 -> 1로 변환.
    식별기호 삭제    : $f,$g,$k,$s를 삭제
==========================================================================*/
INT      CMarcConvert::Convert245_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[6],s_FieldData[10000],s_OldFieldData[10000],s_SubFieldData[1024];
    TCHAR **s_pSubFieldData,*s_pStrptr;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;
    BOOL   s_InitFlag;

    s_BracePos = 0;
    
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 245 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        
        if( f_pMarcStru->pMarcField[i].sIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].sIndicator != ' ' ){
            s_BracePos = f_pMarcStru->pMarcField[i].sIndicator - 48;
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a' ){
                if( s_BracePos != 0 ){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca("), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(")%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else
            if( s_pSubFieldData[j][1] == 'c'){
                s_pSubFieldData[j][1] = 'd';
                s_pStrptr = _tcschr( s_pSubFieldData[j] , ';');
                s_InitFlag = FALSE;
                if( s_pStrptr != NULL){
                    _tcscpy( s_SubFieldData , &s_pSubFieldData[j][2] );
                    
					s_pStrptr = _tcstok( s_SubFieldData , _T(";"));
                    while(s_pStrptr != NULL ){
						str_rtrim( s_pStrptr , s_pStrptr , ' ' );
						str_ltrim( s_pStrptr , s_pStrptr , ' ' );
                        if( s_pStrptr == NULL ) break;
                        if( s_InitFlag == FALSE ){							
                            _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%cd%s"), GT , s_pStrptr );
                        }
                        else{
                            if( s_pStrptr[1] == ' ' ){								
                                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(";%ce%s"), GT , &s_pStrptr[1] );
                            }
                            else{
                                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(";%ce%s"), GT , s_pStrptr);
                            }
                        }
                        s_pStrptr = _tcstok( NULL ,_T(";"));
						s_InitFlag = TRUE;
                    }
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("fgks"));
    for( i = 0; i < 5; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 245            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : 0,1,3 -> 1, 2 -> 0으로 변환.
    제2지시기호 변경 : # -> '3'로 변환.
    식별기호 삭제    : $i,$n,$p,$5를 삭제
==========================================================================*/
INT      CMarcConvert::Convert246_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;
    
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 246 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }
        if( f_pMarcStru->pMarcField[i].fIndicator == '0' ||
            f_pMarcStru->pMarcField[i].fIndicator == '3' ){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }else
        if( f_pMarcStru->pMarcField[i].fIndicator == '2' ){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }
        
        if( f_pMarcStru->pMarcField[i].sIndicator == ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '3';
        }
    }

    _tcscpy( s_tmpbur , _T("inp5"));
    for( i = 0; i < 4; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 246            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호 삭제    : $n,$p를 삭제
==========================================================================*/
INT      CMarcConvert::Convert247_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;
    
    _tcscpy( s_tmpbur , _T("np"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 247            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 247 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }
    }
     

    return 0;

    
}

/*==========================================================================
    식별기호 삭제    : $f,$g,$3를 삭제
==========================================================================*/
INT      CMarcConvert::Convert300_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[4];
    INT   i,ids;

    _tcscpy( s_tmpbur , _T("fg3"));
    for( i = 0; i < 3; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 300            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 300 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }
    }

    return 0;

    
}

/*==========================================================================
    식별기호 삭제    : $3를 삭제
==========================================================================*/
INT      CMarcConvert::Convert340_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   i,ids;

    
    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 340            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 340 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }
    }

    return 0;

    
}

/*==========================================================================
    식별기호 삭제    : $3를 삭제
==========================================================================*/
INT      CMarcConvert::Convert351_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   i,ids;

    
    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 351            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 351 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : '8' -> '5'
==========================================================================*/
INT      CMarcConvert::Convert355_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    INT   i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 355 ) continue;
        
        if( f_pMarcStru->pMarcField[i].fIndicator == '8' ){
            f_pMarcStru->pMarcField[i].fIndicator = '5';
        }
        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' ' -> '0'
    제2지시기호 변경 : 1~9 -> '1'
    식별기호변경     : $x  -> $s
==========================================================================*/
INT      CMarcConvert::Convert440_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

    s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 440 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].sIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].sIndicator - 48;
        }
        
        if( f_pMarcStru->pMarcField[i].sIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].sIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
        if( f_pMarcStru->pMarcField[i].fIndicator == ' '){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }
        
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a' ){
               if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca("), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(")%s"), &s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else
            if( s_pSubFieldData[j][1] == 'x'){
                s_pSubFieldData[j][1] = 's';
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                  ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    return 0;

    
}

/*==========================================================================
    제2지시기호 변경 : ' ' -> '0'
    식별기호변경     : $x  -> $s
    식별기호삭제     : $l
==========================================================================*/
INT      CMarcConvert::Convert490_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 490 ) continue;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] == '.'){
            f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen( f_pMarcStru->pMarcField[i].pFldData_ElementStr) - 1] = '\0';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == ' '){
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator  );
        
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'x'){
                s_pSubFieldData[j][1] = 's';
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }
        f_pMarcStru->pMarcField[i].TagNo = 440;

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _stprintf( s_SubFieldCode , _T("%cl") , GT );
    ids = RemoveElement( 440            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' ' -> '0'
    제2지시기호 변경 : ' ' -> '0'
    식별기호삭제     : $3,$5
==========================================================================*/
INT      CMarcConvert::Convert500_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[3];
    INT   i,ids;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 500 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == ' '){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == ' '){
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
    }

    _tcscpy( s_tmpbur , _T("35"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 500            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $5
==========================================================================*/
INT      CMarcConvert::Convert501_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   ids;

    _stprintf( s_SubFieldCode , _T("%c5") , GT );
    ids = RemoveElement( 500            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    
    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' ' -> '0'    
==========================================================================*/
INT      CMarcConvert::Convert502_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     
    
    INT   i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 502 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == ' '){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $b
==========================================================================*/
INT      CMarcConvert::Convert504_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   ids;
    
    _stprintf( s_SubFieldCode , _T("%cb") , GT );
    ids = RemoveElement( 504            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 : ' ' -> '0'
    제2지시기호 변경 : ' ' -> '0'
    식별기호변경     : $r  -> $d ,$g -> $n으로 변경
==========================================================================*/
INT      CMarcConvert::Convert505_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 505 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '8'){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == ' ' || 
            f_pMarcStru->pMarcField[i].sIndicator == '0' ){
            f_pMarcStru->pMarcField[i].sIndicator = '2';
        }

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator  );
        
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'g'){
                if( j == 0 ){
                    s_pSubFieldData[j][1] = 'n';
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
                else{
                    if( _tcsncmp(&s_pSubFieldData[j][_tcsclen(s_pSubFieldData[j]) -2] ,_T("--"),2) == 0){
                        s_pSubFieldData[j][1] = 'n';
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                    }
                    else{
                        _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                    }
                }
            }else
            if( s_pSubFieldData[j][1] == 'r'){
                s_pSubFieldData[j][1] = 'd';
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
                
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );

    }

    return 0;

    
}

/*==========================================================================
    식별기호변경     : $r,$c,$d,$e를 공백으로 변경.
    식별기호삭제     : $3,$5
==========================================================================*/
INT      CMarcConvert::Convert506_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_FieldData[10000],s_OldFieldData[10000],s_tmpbur[8],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 506 ) continue;

        
        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator  );
        
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'b' || s_pSubFieldData[j][1] == 'c' ||
                s_pSubFieldData[j][1] == 'd' || s_pSubFieldData[j][1] == 'e' ){
                
                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(" %s"), &s_pSubFieldData[j][2] );
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
                
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );

    }

    _tcscpy( s_tmpbur , _T("35"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 500            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }
    return 0;

    
}

/*==========================================================================
    식별기호변경     : $c를 공백으로 변경.
    식별기호DATA변경 : $a 
      제1지시기호 0  : _T("Indexed by: ")추가
      제1지시기호 1  : _T("Indexed in its entirety by: ")추가
      제1지시기호 2  : _T("Indexed selectively by: ")추가
      제1지시기호 3,4: _T("Reference: ")추가
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert510_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 510 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );
        
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a'){
                if( f_pMarcStru->pMarcField[i].fIndicator == '0'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caIndexed by: %s"),GT, &s_pSubFieldData[j][2] );
                }else
                if( f_pMarcStru->pMarcField[i].fIndicator == '1'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caIndexed in its entirety by: %s"),GT ,&s_pSubFieldData[j][2] );
                }else
                if( f_pMarcStru->pMarcField[i].fIndicator == '2'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caIndexed selectively by: %s"),GT ,&s_pSubFieldData[j][2] );
                }else
                if( f_pMarcStru->pMarcField[i].fIndicator == '3' || f_pMarcStru->pMarcField[i].fIndicator == '4' ){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caReference: %s"),GT , &s_pSubFieldData[j][2] );
                }
            }else
            if( s_pSubFieldData[j][1] == 'c'){
                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(" %s"), &s_pSubFieldData[j][2] );
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 510            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}


/*==========================================================================
    제1지시기호변경 : ' ' (공백)
    제2지시기호변경 : ' ' -> '0'으로 변환   
==========================================================================*/
INT      CMarcConvert::Convert511_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    
    INT    i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 511 ) continue;

        f_pMarcStru->pMarcField[i].fIndicator = ' ';

        if( f_pMarcStru->pMarcField[i].sIndicator == ' '){
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
    }
    
    return 0;

    
}

/*==========================================================================
    제1지시기호변경    : 8 -> ' '
    식별기호DATA변경   : $a 
      제1지시기호 ' '  : _T("Type of file: ") $a에 추가
==========================================================================*/
INT      CMarcConvert::Convert516_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){

        if( f_pMarcStru->pMarcField[i].TagNo != 516 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '8' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }else
        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                              f_pMarcStru->pMarcField[i].sIndicator          ,
                                              f_pMarcStru->pMarcField[i].pFldData_ElementStr );

            _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                          f_pMarcStru->pMarcField[i].sIndicator  );

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a' ){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caType of file: %s"), GT , &s_pSubFieldData[j][2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
                
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );


            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                   ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
        }
    }
    
    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert518_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3];
    INT    ids;

    
    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 518            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    제1지시기호변경  : 0,1,8 -> ' '
    식별기호변경     : $b를 공백으로 변경.
    식별기호DATA변경 : $a 
      제1지시기호 0  : _T("Summary: ")추가
      제1지시기호 1  : _T("Subject: ")추가
      제1지시기호 2  : _T("Review: ")추가

    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert520_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 520 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' || 
            f_pMarcStru->pMarcField[i].fIndicator == '0' ||
            f_pMarcStru->pMarcField[i].fIndicator == '1'  ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a'){
                    if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caSummary: %s"), GT ,&s_pSubFieldData[j][2] );
                    }else
                    if( f_pMarcStru->pMarcField[i].fIndicator == '0' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caSubject: %s"), GT, &s_pSubFieldData[j][2] );
                    }else
                    if( f_pMarcStru->pMarcField[i].fIndicator == '1' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caReview: %s"), GT , &s_pSubFieldData[j][2] );
                    }else{
                        _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                    }
                }else
                if( s_pSubFieldData[j][1] == 'b'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(" %s") , &s_pSubFieldData[j][2] );
                }else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                   ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
        }
        else{
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 520            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    제1지시기호변경  : 0,1,2,3,4,8 -> ' '
    식별기호DATA변경 : $a 
      제1지시기호 8이 아닌 경우 : _T("Audience: ")추가
    식별기호삭제     : $b,$3
==========================================================================*/
INT      CMarcConvert::Convert521_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_FieldData[10000],s_OldFieldData[10000],s_tmpbur[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 521 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        if( f_pMarcStru->pMarcField[i].fIndicator != '8' ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caAudience: %s"), GT , &s_pSubFieldData[j][2] );
                }else{
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") , s_pSubFieldData[j] );
                }
            }
            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                   ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
        }
        else{
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }

    _tcscpy( s_tmpbur , _T("b3"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 521            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호변경  : 8 -> ' '
    식별기호DATA변경 : $a 
      제1지시기호 ' ' : _T("Cite as: ")추가
==========================================================================*/
INT      CMarcConvert::Convert524_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 524 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caCite as: %s"), GT , &s_pSubFieldData[j][2] );
                }else{
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") , s_pSubFieldData[j] );
                }
            }
            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                   ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
        }
        else{
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }

    
    return 0;

    
}

/*==========================================================================
    식별기호변경     : $b,$c,$d를 공백으로 변경.
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert530_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){

        
        if( f_pMarcStru->pMarcField[i].TagNo != 530 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        
        _stprintf( s_FieldData ,_T("%c%c")  ,f_pMarcStru->pMarcField[i].fIndicator          ,
                                       f_pMarcStru->pMarcField[i].sIndicator          );
        

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'b' || s_pSubFieldData[j][1] == 'c' ||
                s_pSubFieldData[j][1] == 'd'){
                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(" %s"), &s_pSubFieldData[j][2] );
            }else{
                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 530            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $3,$n,$7
==========================================================================*/
INT      CMarcConvert::Convert533_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;
    
    
    _tcscpy( s_tmpbur , _T("n37"));
    for( i = 0; i < 3; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 533            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}


/*==========================================================================
    식별기호삭제     : $b,$d,$g,$3
==========================================================================*/
INT      CMarcConvert::Convert535_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;
    
    
    _tcscpy( s_tmpbur , _T("bdg3"));
    for( i = 0; i < 4; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 535            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $b,$c,$d
==========================================================================*/
INT      CMarcConvert::Convert536_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[4];
    INT   i,ids;

    _tcscpy( s_tmpbur , _T("bcd"));
    for( i = 0; i < 3; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 536            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호변경     : $b,$c,$d를 공백으로 변경.
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert540_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3],s_tmpbur[8];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 540 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        
        _stprintf( s_FieldData ,_T("%c%c")  ,f_pMarcStru->pMarcField[i].fIndicator          ,
                                       f_pMarcStru->pMarcField[i].sIndicator          );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'b' || s_pSubFieldData[j][1] == 'c' ||
                s_pSubFieldData[j][1] == 'd'){
                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(" %s"), &s_pSubFieldData[j][2] );
            }else{
                _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("35"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 540            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $e,$n,$o,$3,$5
==========================================================================*/
INT      CMarcConvert::Convert541_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;

    _tcscpy( s_tmpbur , _T("bcd"));
    for( i = 0; i < 3; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 541            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert544_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   ids;
    
    _stprintf( s_SubFieldCode , _T("%c3") , GT  );
    ids = RemoveElement( 544            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $b,$3
==========================================================================*/
INT      CMarcConvert::Convert546_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;

    _tcscpy( s_tmpbur , _T("b3"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 546            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    지시기호변경     : ' ' -> '0'
==========================================================================*/
INT      CMarcConvert::Convert550_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    INT    i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 550 ) continue;
        
        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
            f_pMarcStru->pMarcField[i].fIndicator = '0';
        }
    }
    
    return 0;

    
}

/*==========================================================================
    제1지시기호변경  : 0,8 -> ' '
    식별기호DATA변경 : $a 
      제1지시기호 ' ' : _T("Indexes: ")추가
      제1지시기호 '0' : _T("Finding aids: ")추가
    식별기호변경     : $b,$c,$d를 공백으로 변경.
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert555_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 555 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' || 
            f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a'){
                    if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caIndexes: %s"), GT , &s_pSubFieldData[j][2] );
                    }else
                    if( f_pMarcStru->pMarcField[i].fIndicator == '0' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caFinding aids: %s"), GT , &s_pSubFieldData[j][2] );
                    }else{
                        _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                    }
                }else
                if( s_pSubFieldData[j][1] == 'b' || s_pSubFieldData[j][1] == 'c' || s_pSubFieldData[j][1] == 'd'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T(" %s") , &s_pSubFieldData[j][2] );
                }else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                   ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
        }
        else{
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 555            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}


/*==========================================================================
    식별기호삭제     : $b,$3,$5
==========================================================================*/
INT      CMarcConvert::Convert561_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;

    _tcscpy( s_tmpbur , _T("b53"));
    for( i = 0; i < 3; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 561            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호변경  : 0,8 -> ' '
    식별기호DATA변경 : $a 
      제1지시기호 ' ' : _T("File size: ")추가
      제1지시기호 '0' : _T("Case file charateristics: ")추가
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert565_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 565 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' || 
            f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a'){
                    if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caFile size: %s"), GT ,&s_pSubFieldData[j][2] );
                    }else
                    if( f_pMarcStru->pMarcField[i].fIndicator == '0' ){
                        _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caCase file characteristics: %s"), GT , &s_pSubFieldData[j][2] );
                    }else{
                        _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                    }
                }else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                   ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
        }
        else{
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 565            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;

    return 0;

    
}

/*==========================================================================
    제1지시기호변경  : 0,8 -> ' '
    식별기호DATA변경 : $a 
      제1지시기호 ' ' : _T("Publications: ")추가
    식별기호삭제     : $z,$3
==========================================================================*/
INT      CMarcConvert::Convert581_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_FieldData[10000],s_OldFieldData[10000],s_SubFieldCode[3],s_tmpbur[8];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 581 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        
        
        if( f_pMarcStru->pMarcField[i].fIndicator == '8' ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );
        }
        else{
            _stprintf( s_FieldData , _T("%c%c"), f_pMarcStru->pMarcField[i].fIndicator,
                                           f_pMarcStru->pMarcField[i].sIndicator );
        }
            

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a'){
                if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caPublications: %s"), GT ,&s_pSubFieldData[j][2] );
                }else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }

        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                   ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("z3"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 581            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $5
==========================================================================*/
INT      CMarcConvert::Convert583_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3];
    INT   ids;

    _stprintf( s_SubFieldCode , _T("%c5") , GT);
    ids = RemoveElement( 583            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $3,$5
==========================================================================*/
INT      CMarcConvert::Convert585_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[8];
    INT   i,ids;

    _tcscpy( s_tmpbur , _T("53"));
    for( i = 0; i < 2; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 585            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert586_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 586 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
            _stprintf( s_FieldData , _T(" %c"),f_pMarcStru->pMarcField[i].sIndicator );

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] == 'a'){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%caAward: %s"), GT ,&s_pSubFieldData[j][2] );
                }else{
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") , s_pSubFieldData[j] );
                }
            }
            ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                             s_OldFieldData                  ,
                                             s_FieldData                      ,
                                             f_pMarcStru );
            if( ids ) {
                FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                return ids;
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
        }
        else{
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }
    
    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 586            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    
    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  0,1,2 -> ' '
    식별기호변경     : $v -> $x
    식별기호삭제     : $e,$f,$g,$h,$k,$o,$s,$u,$3,$4
==========================================================================*/
INT      CMarcConvert::Convert600_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR    s_SubFieldCode[3],s_tmpbur[15];
    TCHAR  **s_pSubFieldData;
    INT     i,j,ids,s_SubFieldCount;


    _tcscpy( s_tmpbur , _T("efghkosu34"));
    for( i = 0; i < 10; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 600            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 600 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '2' ){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'v'){
                _stprintf( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , _T("%cx%s") ,GT,&s_pSubFieldData[j][2] );
            }else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  0,1,2 -> ' '
    식별기호변경     : $v -> $x
    식별기호삭제     : $e,$f,$h,$o,$s,$u,$3,$4
==========================================================================*/
INT      CMarcConvert::Convert610_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[15];
    TCHAR  **s_pSubFieldData;
    INT     i,j,ids,s_SubFieldCount;

    _tcscpy( s_tmpbur , _T("efhosu34"));
    for( i = 0; i < 8; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 610            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 610 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '2' ||
            f_pMarcStru->pMarcField[i].fIndicator == '1' ||
            f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'v'){
                _stprintf( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , _T("%cx%s") ,GT,&s_pSubFieldData[j][2] );
            }else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  0,1,2 -> ' '
    식별기호변경     : $v -> $x
    식별기호삭제     : $e,$f,$h,$o,$s,$u,$3,$4
==========================================================================*/
INT      CMarcConvert::Convert611_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR  s_SubFieldCode[3],s_tmpbur[15];
    TCHAR  **s_pSubFieldData;
    INT     i,j,ids,s_SubFieldCount;

    _tcscpy( s_tmpbur , _T("fhkqsu34"));
    for( i = 0; i < 8; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 611            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 611 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '2' ||
            f_pMarcStru->pMarcField[i].fIndicator == '1' ||
            f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'v'){
                _stprintf( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , _T("%cx%s") ,GT,&s_pSubFieldData[j][2] );
            }else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  1~9 -> '1'
    식별기호변경     : $v -> $x
    식별기호삭제     : $d,$h,$o,$v,$3
==========================================================================*/
INT      CMarcConvert::Convert630_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR    s_SubFieldCode[3],s_tmpbur[15];
    TCHAR  **s_pSubFieldData;
    INT     i,j,ids,s_SubFieldCount;

    
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 630 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator >= '0' &&
            f_pMarcStru->pMarcField[i].fIndicator <= '9' ){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'v'){
                _stprintf( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , _T("%cx%s") ,GT,&s_pSubFieldData[j][2] );
            }else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("dhov3"));
    for( i = 0; i < 5; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 630            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;
   

    
}

/*==========================================================================
    제1지시기호 변경 :  1~9 -> '1'
    식별기호변경     : $v -> $x
    식별기호삭제     : $b,$c,$d,$e,$v,$3
==========================================================================*/
INT      CMarcConvert::Convert650_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR    s_SubFieldCode[3],s_tmpbur[15];
    TCHAR  **s_pSubFieldData;
    INT     i,j,ids,s_SubFieldCount;

    
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 650 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator >= '0' &&
            f_pMarcStru->pMarcField[i].fIndicator <= '9' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'v'){
                _stprintf( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , _T("%cx%s") ,GT,&s_pSubFieldData[j][2] );
            }else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("bcdev3"));
    for( i = 0; i < 6; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 650            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;
   

    
}

/*==========================================================================
    식별기호변경     : $v -> $x
    식별기호삭제     : $3
==========================================================================*/
INT      CMarcConvert::Convert651_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR    s_SubFieldCode[3];
    TCHAR  **s_pSubFieldData;
    INT     i,j,ids,s_SubFieldCount;

    
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 651 ) continue;

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = '\0';
        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'v'){
                _stprintf( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , _T("%cx%s") ,GT,&s_pSubFieldData[j][2] );
            }else{
                _tcscpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[_tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr)] , s_pSubFieldData[j] );
            }
        }
        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _stprintf( s_SubFieldCode , _T("%c3") , GT );
    ids = RemoveElement( 651            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;


    return 0;
   

    
}

/*==========================================================================
    제1지시기호 변경 :  0,1,2, -> ' '    
==========================================================================*/
INT      CMarcConvert::Convert653_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    INT   i;

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 653 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '0' ||
            f_pMarcStru->pMarcField[i].fIndicator == '1' ||
            f_pMarcStru->pMarcField[i].fIndicator == '2' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  2 -> '1'
    제2지시기호 변경 :  2 -> ' '
    식별기호삭제     :  $f,$g,$h,$k,$s,$u,$x,$3,$4,$5
==========================================================================*/
INT      CMarcConvert::Convert700_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[14];
    INT   i,ids;
    
    _tcscpy( s_tmpbur , _T("fghksux345"));
    for( i = 0; i < 10; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 700            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 700 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '2'){
            f_pMarcStru->pMarcField[i].fIndicator = '1';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == '2'){
            f_pMarcStru->pMarcField[i].sIndicator = ' ';
        }
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  0,1,2 -> ' '
    제2지시기호 변경 :  2 -> ' '
    식별기호삭제     :  $f,$h,$s,$u,$x,$3,$4,$5
==========================================================================*/
INT      CMarcConvert::Convert710_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[14];
    INT   i,ids;
    
    _tcscpy( s_tmpbur , _T("fhsux345"));
    for( i = 0; i < 8; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 710            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 710 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '2' || 
            f_pMarcStru->pMarcField[i].fIndicator == '1' ||
            f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == '2'){
            f_pMarcStru->pMarcField[i].sIndicator = ' ';
        }
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  0,1,2 -> ' '
    제2지시기호 변경 :  2 -> ' '
    식별기호삭제     :  $f,$k,$q,$s,$u,$x,$3,$4,$5
==========================================================================*/
INT      CMarcConvert::Convert711_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[14];
    INT   i,ids;
    
    _tcscpy( s_tmpbur , _T("fkqsux345"));
    for( i = 0; i < 9; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 711            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 711 ) continue;

        if( f_pMarcStru->pMarcField[i].fIndicator == '2' || 
            f_pMarcStru->pMarcField[i].fIndicator == '1' ||
            f_pMarcStru->pMarcField[i].fIndicator == '0' ){
            f_pMarcStru->pMarcField[i].fIndicator = ' ';
        }

        if( f_pMarcStru->pMarcField[i].sIndicator == '2'){
            f_pMarcStru->pMarcField[i].sIndicator = ' ';
        }
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  ' '
    제2지시기호 변경 :  1~9 -> '1'
    식별기호삭제     :  $d,$x,$3,$5
==========================================================================*/
INT      CMarcConvert::Convert730_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[7],s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

    s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 730 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].fIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].fIndicator - 48;
        }
        
        f_pMarcStru->pMarcField[i].sIndicator = f_pMarcStru->pMarcField[i].fIndicator;
        if( f_pMarcStru->pMarcField[i].fIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].fIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
        else{
            f_pMarcStru->pMarcField[i].sIndicator = f_pMarcStru->pMarcField[i].fIndicator;
        }

        if( f_pMarcStru->pMarcField[i].fIndicator == ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
        f_pMarcStru->pMarcField[i].fIndicator = '0';
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] == 'a'){
                if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca"), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                  ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );        
    }

    _tcscpy( s_tmpbur , _T("dx35"));
    for( i = 0; i < 4; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 730            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

/*==========================================================================
    제1지시기호 변경 :  ' '
    제2지시기호 변경 :  1~9 -> '1'
    식별기호삭제     :  $5
==========================================================================*/
INT      CMarcConvert::Convert740_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

    s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 740 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].fIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].fIndicator - 48;
        }
        
        
        if( f_pMarcStru->pMarcField[i].fIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].fIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
        else{
            f_pMarcStru->pMarcField[i].sIndicator = f_pMarcStru->pMarcField[i].fIndicator;
        }

        if( f_pMarcStru->pMarcField[i].fIndicator == '0' ){  
            f_pMarcStru->pMarcField[i].sIndicator = '0';
        }
        
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] != 'a'){
                if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca"), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                  ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    
    _stprintf( s_SubFieldCode , _T("%c5") , GT );
    ids = RemoveElement( 740            ,
                         s_SubFieldCode ,
                         f_pMarcStru    );
    if( ids ) return ids;
    
    return 0;

    
}

INT      CMarcConvert::Convert830_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_tmpbur[13],s_FieldData[10000],s_OldFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount,s_BracePos,s_strlen;

    s_BracePos = 0;
    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 830 ) continue;

        _stprintf( s_OldFieldData ,_T("%c%c%s"),f_pMarcStru->pMarcField[i].fIndicator          ,
                                          f_pMarcStru->pMarcField[i].sIndicator          ,
                                          f_pMarcStru->pMarcField[i].pFldData_ElementStr );
        if( f_pMarcStru->pMarcField[i].sIndicator != ' '){
            s_BracePos = f_pMarcStru->pMarcField[i].sIndicator - 48;
        }
                
        if( f_pMarcStru->pMarcField[i].sIndicator != '0'  && 
            f_pMarcStru->pMarcField[i].sIndicator != ' ' ){
            f_pMarcStru->pMarcField[i].sIndicator = '1';
        }
                
        
        _stprintf( s_FieldData , _T("%c%c"),f_pMarcStru->pMarcField[i].fIndicator ,
                                      f_pMarcStru->pMarcField[i].sIndicator );

        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                 &s_SubFieldCount                                ,
                                                 &s_pSubFieldData                                );
        if( ids ) return ids;

        for( j = 0; j < s_SubFieldCount ; j++ ){
            if( s_pSubFieldData[j][1] != 'a'){
                if( s_BracePos != 0 && f_pMarcStru->pMarcField[i].sIndicator != ' '){
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%ca"), GT );
                    s_strlen = _tcsclen(s_FieldData);
                    _tcsnccpy( &s_FieldData[s_strlen] , &s_pSubFieldData[j][2] , s_BracePos );
                    if( s_FieldData[s_strlen + s_BracePos-1] == ' '){
                        s_FieldData[s_strlen + s_BracePos-1] = '\0';
                    }
                    else{
                         s_FieldData[s_strlen + s_BracePos] = '\0';
                    }
                    _stprintf( &s_FieldData[_tcsclen(s_FieldData)] , _T("%s") ,&s_pSubFieldData[j][s_BracePos+2] );
                }
                else{
                    _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
                }
            }
            else{
                _tcscpy( &s_FieldData[_tcsclen(s_FieldData)] , s_pSubFieldData[j] );
            }
        }
        ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                         s_OldFieldData                  ,
                                         s_FieldData                      ,
                                         f_pMarcStru );
        if( ids ) {
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
            return ids;
        }

        FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
    }

    _tcscpy( s_tmpbur , _T("dfghklmorst"));
    for( i = 0; i < 11; i++){
        _stprintf( s_SubFieldCode , _T("%c%c") , GT, s_tmpbur[i] );
        ids = RemoveElement( 830            ,
                             s_SubFieldCode ,
                             f_pMarcStru    );
        if( ids ) return ids;
    }

    return 0;

    
}

// 350,950의 화폐구분의 값을 변환한다.
// $ -> US$, 영국 파운드(0xb9) -> 0xA1CC로 변환.
INT      CMarcConvert::Convert350_950USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
    

    TCHAR s_SubFieldCode[3],s_OldSubFieldData[1024],s_NewSubFieldData[1024];
	TCHAR s_OldFieldData[1024],s_NewFieldData[1024];
	TCHAR **s_pSubFieldData;
    INT  i,j,ids,s_SubFieldDataCnt;

    s_NewSubFieldData[0] = '\0';

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo != 350 &&
            f_pMarcStru->pMarcField[i].TagNo != 950  ) continue;
        if( f_pMarcStru->pMarcField[i].TagNo == 350 ){
            _stprintf( s_SubFieldCode , _T("%ca") , GT );
        }else
        if( f_pMarcStru->pMarcField[i].TagNo == 950 ){
            _stprintf( s_SubFieldCode , _T("%cb") , GT );
        }

        ids = GetSubFieldDataFromDataField( s_SubFieldCode                                 ,
                                            f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                            s_OldSubFieldData );
        if( ids ) return -1;

        
        // $ -> US$로 변환
        if( s_OldSubFieldData[0] == '$' ){
            _stprintf( s_NewSubFieldData , _T("US$%s"), &s_OldSubFieldData[1] );
        }else
        // 영국 파운드화(0xb9)인경우 0xA1,0xCC로 변환한다.
        if( (_TUCHAR)s_OldSubFieldData[0] == 0xb9 ){
            _stprintf( s_NewSubFieldData , _T("%c%c%s"), 0xA1 , 0xCC , &s_OldSubFieldData[1] );
        }

        _tcsupr( s_NewSubFieldData );
        // 바뀐 Data가 있는 경우에만 갱신한다.
        if( s_NewSubFieldData[0] != '\0'){
            ids = UpdateElement( f_pMarcStru->pMarcField[i].TagNo ,
	    		                 s_OldSubFieldData                ,
				                 s_NewSubFieldData                ,
				                 f_pMarcStru                      );
            if( ids ) return -2;
        }

		if( f_pMarcStru->pMarcField[i].TagNo == 950 ){
			s_SubFieldDataCnt = 0;
			s_pSubFieldData = NULL;
			_stprintf( s_OldFieldData , _T("%c%c%s"), 
				                     f_pMarcStru->pMarcField[i].fIndicator ,
									 f_pMarcStru->pMarcField[i].sIndicator ,
									 f_pMarcStru->pMarcField[i].pFldData_ElementStr );

			_stprintf( s_NewFieldData , _T("%c%c"), 
				                     f_pMarcStru->pMarcField[i].fIndicator ,
									 f_pMarcStru->pMarcField[i].sIndicator );


			ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldDataCnt                              ,
                                                     &s_pSubFieldData                                );
			if( ids ){
				return -3;
			}
			for( j = 0; j < s_SubFieldDataCnt; j++){
				if( f_pMarcStru->pMarcField[i].fIndicator == '0' && s_pSubFieldData[j][1] == 'a' ){
					_stprintf( &s_NewFieldData[_tcsclen(s_NewFieldData)], _T("%cb%s"), GT, &s_pSubFieldData[j][2] );
				}else
				if( f_pMarcStru->pMarcField[i].fIndicator == '1' && s_pSubFieldData[j][1] == 'b' ){
					_stprintf( &s_NewFieldData[_tcsclen(s_NewFieldData)], _T("%ca%s"), GT, &s_pSubFieldData[j][2] );
				}
				else{
					_stprintf( &s_NewFieldData[_tcsclen(s_NewFieldData)], _T("%s"), s_pSubFieldData[j] );
				}
			}

			FreeTwoDim( s_SubFieldDataCnt , s_pSubFieldData );

			if( _tcscmp( s_NewFieldData , s_OldFieldData ) != 0 ){
				ids = UpdateFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
											     s_OldFieldData                   ,
												 s_NewFieldData                   ,
												 f_pMarcStru                      );
				if( ids ){
					return -4;
				}
			}
		}
    }

    return 0;

    
}

INT      CMarcConvert::Convert6XXIndicator_USMarcToKormarc( tMARC_STRU *f_pMarcStru )
{
     

    TCHAR   s_SubFieldCode[3],s_SubFieldData[10000];
    TCHAR **s_pSubFieldData;
    INT    i,j,ids,s_SubFieldCount;


    _stprintf( s_SubFieldCode ,_T("%c2"), GT );

    for( i = 0; i < f_pMarcStru->MarcFieldCount; i++){
        if( f_pMarcStru->pMarcField[i].TagNo == 600 || f_pMarcStru->pMarcField[i].TagNo == 610 ||
            f_pMarcStru->pMarcField[i].TagNo == 611 || f_pMarcStru->pMarcField[i].TagNo == 630 ||
            f_pMarcStru->pMarcField[i].TagNo == 650 || f_pMarcStru->pMarcField[i].TagNo == 651 ){
            
            if( f_pMarcStru->pMarcField[i].fIndicator == '5' ){
                _tcscpy( s_SubFieldData , _T("Canadian subject Heading"));
                f_pMarcStru->pMarcField[i].fIndicator = '7';
                ids = InsertSubFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                                    s_SubFieldCode                   ,
                                                    s_SubFieldData                   ,
                                                    f_pMarcStru                      ,  
                                                    FALSE                            ,
                                                    ' '                              );
                if( ids < 0 ) return ids;
            }else
            if( f_pMarcStru->pMarcField[i].fIndicator == '6' ){
                _tcscpy( s_SubFieldData , _T("Repertoire desvedttesmatiere"));
                f_pMarcStru->pMarcField[i].fIndicator = '7';
                ids = InsertSubFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                                    s_SubFieldCode                   ,
                                                    s_SubFieldData                   ,
                                                    f_pMarcStru                      ,  
                                                    FALSE                            ,
                                                    ' '                              );
                if( ids < 0 ) return ids;
            }

            ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                     &s_SubFieldCount                                ,
                                                     &s_pSubFieldData                                );
            if( ids ) return ids;

            for( j = 0; j < s_SubFieldCount ; j++ ){
                if( s_pSubFieldData[j][1] != 'v') continue;
                _stprintf( s_SubFieldCode , _T("%cx"),GT);
                ids = InsertSubFieldDataToMarcStru( f_pMarcStru->pMarcField[i].TagNo ,
                                                    s_SubFieldCode                   ,
                                                    &s_pSubFieldData[j][2]           ,
                                                    f_pMarcStru                      ,  
                                                    FALSE                            ,
                                                    ' '                              );
                if( ids < 0 ) {
                    FreeTwoDim( s_SubFieldCount , s_pSubFieldData );
                    return ids;
                }
            }
            FreeTwoDim( s_SubFieldCount , s_pSubFieldData );

            _stprintf( s_SubFieldCode , _T("%cv") , GT );
            ids = RemoveElement( f_pMarcStru->pMarcField[i].TagNo ,
                                 s_SubFieldCode ,
                                 f_pMarcStru    );
            if( ids ) return ids;
        }
    }

    return 0;

    
}

// OCLC에서 쓰는 특수문자들을 KS5601에 맞게 변경한다.
INT      CMarcConvert::ConvertALACharToKS5601Char( TCHAR *f_pRawStr , TCHAR *f_pConvertStr )
{
    

    tMARC_STRU s_MarcStru;
    TCHAR       s_ConvertStr[10000],s_OldFieldData[10000];
    INT        i,j,k,idx,ids;
    
    ids = ReadMarcStruFromStreamMarc( f_pRawStr , &s_MarcStru );
    if( ids ) return ids;


    for( i = 0; i < s_MarcStru.MarcFieldCount; i++){

		idx = 0;
		s_ConvertStr[0] = '\0';

		if( s_MarcStru.pMarcField[i].TagNo < 10 ){
			_stprintf( s_OldFieldData , _T("%s") , s_MarcStru.pMarcField[i].pFldData_ElementStr );
		}
		else{
			_stprintf( s_OldFieldData , _T("%c%c%s") , s_MarcStru.pMarcField[i].fIndicator ,
												 s_MarcStru.pMarcField[i].sIndicator ,
												 s_MarcStru.pMarcField[i].pFldData_ElementStr );
		}

        for( j = 0; j < (INT)_tcsclen(s_OldFieldData) ; j++ ){
            for( k = 0; k < ALA_CHAR_COUNT; k++ ){
                if( (_TUCHAR)s_OldFieldData[j] != AlaConvertKS5601[k].AlaChar ) continue;
                s_ConvertStr[idx] = AlaConvertKS5601[k].s_HighChar;
                s_ConvertStr[idx+1] = AlaConvertKS5601[k].s_LowChar;
                idx++;
                break;
            }

            if( k >= ALA_CHAR_COUNT ){
                if( s_OldFieldData[j] == '|' ){
                    s_ConvertStr[idx] = ' ';
                }
                else{
                    s_ConvertStr[idx] = s_OldFieldData[j];
                }
            }
            idx++;
        }    
        s_ConvertStr[idx] = '\0';
        
        if( _tcscmp( s_OldFieldData , s_ConvertStr ) != 0 ){
            ids = UpdateFieldDataToMarcStru( s_MarcStru.pMarcField[i].TagNo ,
                                             s_OldFieldData ,
                                             s_ConvertStr ,
                                            &s_MarcStru );
            if( ids ){
                FreeMarcStru( s_MarcStru );
                return ids;
            }
        }
    }

    ids = WriteMarcStruToStreamMarc( s_MarcStru, f_pConvertStr );
    FreeMarcStru( s_MarcStru );
    if( ids ) return ids;

    return 0;

    
}


/*================================================================================
  StreamMarc의 데이타를 각 구조체 ( 리더,디렉토리,데이타필드 ) 에 저장한다.
  IN  : StreamMarc
  OUT : MARC_STRU 
================================================================================*/ 
INT CMarcConvert::ReadMarcStruFromStreamMarc( TCHAR       * f_pStreamMarc, 
							    tMARC_STRU * f_pMarcStru )
{
    

    TCHAR            ctmp[10000];
    INT             s_FieldBasePos, ids, i , j;
    INT             ReadPos, s_FieldDataLength, s_FieldStartPos;
	INT             Control_DataField_Flag ;
    tDIRECTORY_STRU s_MarcDir;

    ReadPos = s_FieldDataLength = s_FieldStartPos = s_FieldBasePos = 0;
   // f_pMarcStru->RecordTerm = RT;
	// 1. 리더 초기화
    memset( &f_pMarcStru->MarcLeader, ' ', sizeof(tLEADER_STRU) );

    if ( f_pStreamMarc[0] == 0 ) {
        /// 1. MarcStru 초기화
        memset( f_pMarcStru, 0, sizeof(tMARC_STRU) );
        memset( &f_pMarcStru->MarcLeader, ' ', sizeof(tLEADER_STRU) );
        f_pMarcStru->RecordTerm = RT;
		return 0;
    }

    /// 1. Marc Leader Read
    ids = GetMarcLeaderFromStreamMarc( f_pStreamMarc, &f_pMarcStru->MarcLeader );
    if ( ids ) return ids;

    /// 2. 디렉토리의 길이를 구하기위해 필드가 시작되는 위치를 구한다.
    wmemcpy( ctmp, &f_pMarcStru->MarcLeader.FieldBasePos[0], 5 ); ctmp[5]= 0;
    s_FieldBasePos = _ttoi( ctmp );   //  BasePose Of DataField

    /// 3. 디렉토리 데이터를 구한다. ( MarcDir.pEntries Mem_Allocate )
    ids = GetMarcDirectoryFromStreamMarc( LEARDER_SIZE , s_FieldBasePos-1, f_pStreamMarc, &s_MarcDir );
    if ( ids ) return ids;

	/// 4  필드 갯수와 필드를 구한다.

	f_pMarcStru->MarcFieldCount = s_MarcDir.EntryCount ;

	f_pMarcStru->pMarcField = (tFIELD_STRU *)malloc(
						     sizeof(tFIELD_STRU) * f_pMarcStru->MarcFieldCount );
	if ( f_pMarcStru->pMarcField == NULL ) {
		free( s_MarcDir.pEntries );
	    return -222;
	}

	for ( i=0 ; i < f_pMarcStru->MarcFieldCount ; i++ )
		f_pMarcStru->pMarcField[i].pFldData_ElementStr = NULL ;

	// 4.1 할당된 메모리에 필드자료를 저장한다.
	ids = 0;
    for ( i=0 ; i < f_pMarcStru->MarcFieldCount; i++ ) 
	{
	    /// 4.2.1 각 필드의 길이를 구한	후 (지시기호/)종단기호를 제외한 나머지 길이구함
	    wmemcpy( ctmp, s_MarcDir.pEntries[i].FieldLength, 4 ); ctmp[4] = 0;
//	    s_FieldDataLength = _ttoi( ctmp );                // By HwangKumHee 99.09.16
	    if ( (s_FieldDataLength = _ttoi( ctmp )) <= 0 )  // By HwangKumHee 99.09.16
		{
			ids = -200;
			goto EBOT;
		}

	    /// 4.2.2 각 필드 시작위치(BaseFieldPos를 기준으로한 상대적인 위치)를 구한후
	    ///     버퍼내의 절대 위치를 구한다.
	    wmemcpy( ctmp, s_MarcDir.pEntries[i].FieldStartPos, 5 ); ctmp[5] = 0;
	    s_FieldStartPos = _ttoi( ctmp );
	    ReadPos = s_FieldBasePos + s_FieldStartPos;

	    /// 4.2.3 TagNo을 저장한다
	    wmemcpy( ctmp, s_MarcDir.pEntries[i].TagNo, 3 ); ctmp[3] = 0;

		//--------------- By HwangKumHee 99.09.16

		if ( tagno_integer_check ( ctmp ) )  
		{
			ids = -201 ;  
			goto EBOT  ;
		}

		//+++++++++++++++ By HwangKumHee 99.09.16

        f_pMarcStru->pMarcField[i].TagNo = _ttoi( ctmp );

		Control_DataField_Flag = wmemcmp( &s_MarcDir.pEntries[i].TagNo[0], _T("00"), 2 ); 
        if(wmemcmp( &s_MarcDir.pEntries[i].TagNo[0], _T("908"), 3 ) == 0) Control_DataField_Flag = 0;
		switch ( Control_DataField_Flag )
		{
   			case  0 :  // Control Field

				// Append By HwangKumHee 99.09.15
			   if ( s_FieldDataLength <= 1 )
			   {
					ids = -115;
					goto EBOT ;
			   }
				//++++ Append By HwangKumHee 99.09.15

			   /// 4.2.3 요소데이터 메모리 확보(NULL 포함)
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR *)malloc( s_FieldDataLength+sizeof(TCHAR) );
			   if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) {
				  ids = -112;
				  goto  EBOT;
			   }

			   /// 4.2.4 데이터 Copy (필드종단기호 제외)
			   wmemcpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[0],
					   &f_pStreamMarc[ReadPos], s_FieldDataLength-1 );
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr[s_FieldDataLength-1] = 0;

			   /// 4.2.5 필드종단기호 Assign
			   ReadPos = s_FieldBasePos + s_FieldStartPos + (s_FieldDataLength-1);
              
			   break;

			default :  // Data    Field

				//---- Append By HwangKumHee 99.09.15
			   if ( s_FieldDataLength <= 3 )
			   {
					ids = -116;
					goto EBOT ;
			   }
				//++++ Append By HwangKumHee 99.09.15

   		       /// 4.2.3 제1/2 지시기호를 구한다.
			   f_pMarcStru->pMarcField[i].fIndicator = f_pStreamMarc[ReadPos++];
			   f_pMarcStru->pMarcField[i].sIndicator = f_pStreamMarc[ReadPos++];

			   /// 4.2.4 요소데이터 메모리 확보(NULL 포함)
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR *)malloc( s_FieldDataLength+sizeof(TCHAR));
			   if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) {
				  ids = -113;
				  goto  EBOT;
			   }

			   /// 4.2.5 데이터 Copy (지시기호/필드종단기호 제외)
			   wmemcpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[0],
					   &f_pStreamMarc[ReadPos], s_FieldDataLength-3 );
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr[s_FieldDataLength-3] = 0;

			   //------- Append By HwangKumHee 99.09.16

			   if ( _tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr) != (UINT)(s_FieldDataLength-3) )
			   {
				   ids = -202;
				   goto EBOT;
			   }
			   if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] != (TCHAR)GT )
			   {
				   ids = -204;
				   goto EBOT;
			   }

			   //+++++++ Append By HwangKumHee 99.09.16

			   /// 4.2.6 필드종단기호 Assign
			   ReadPos +=  (s_FieldDataLength-3);
		}

		//  필드종단기호
	    f_pMarcStru->pMarcField[i].FieldTerm = f_pStreamMarc[ReadPos];		
	}

	// 5. RecordTerm Assign
    ReadPos = s_FieldBasePos + s_FieldStartPos + s_FieldDataLength ;
	if ( f_pStreamMarc[ ReadPos ] != (TCHAR)RT ){
		ids = -114;
		goto EBOT;
	}
	f_pMarcStru->RecordTerm = f_pStreamMarc[ ReadPos ];

	free ( s_MarcDir.pEntries ); 
    return 0;


EBOT :
           // MarcStru->MarcDir->pEntries
	FILE *s_pErrorMarcFP;
	free( s_MarcDir.pEntries );
           // MarcStru->pMarcField[i].pFldData_ElementStr
	SYSTEMTIME Time; 
   	GetLocalTime(&Time);

    s_pErrorMarcFP = _tfopen( _T("..\\tunning\\ErrorLog.txt"),_T("a+b"));
    if ( s_pErrorMarcFP == NULL )
        goto FILENULL;


		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(s_pErrorMarcFP, 0, SEEK_END);		
		if( 0 == ftell(s_pErrorMarcFP) )
		{
			/*UNCHANGE*/fputc(0xFF, s_pErrorMarcFP);
			/*UNCHANGE*/fputc(0xFE, s_pErrorMarcFP);
		}
		// ------------------------------------------------------------------------------
		
	switch( ids ){
	case -112:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 제어필드 메모리 할당 오류%c%c") ,
			                      Time.wMonth,Time.wDay                                              ,
								  Time.wHour, Time.wMinute                                           ,
								  Time.wSecond ,13,10 		                                         );
		break;
	case -113:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 DATA필드 메모리 할당 오류%c%c") ,
			                      Time.wMonth,Time.wDay                                              ,
								  Time.wHour, Time.wMinute                                           ,
								  Time.wSecond ,13,10 		                                         );
		break;
	case -114:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 레코드 종단기호 위치 오류%c%c") ,
			                      Time.wMonth,Time.wDay                                              ,
								  Time.wHour, Time.wMinute                                           ,
								  Time.wSecond ,13,10 		                                         );
		break;
	case -115:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 [TAGNO] %d 제어필드길이가 1바이트 이하입니다.%c%c") ,
			                      Time.wMonth,Time.wDay                                                 ,
								  Time.wHour, Time.wMinute                                              ,
								  Time.wSecond                                                          ,
								  f_pMarcStru->pMarcField[i].TagNo                                      ,
								  13,10 		                                                        );
		break;
	case -116:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 [TAGNO] %d DATA필드길이가 3바이트 이하입니다.%c%c") ,
			                      Time.wMonth,Time.wDay                                                 ,
								  Time.wHour, Time.wMinute                                              ,
								  Time.wSecond                                                          ,
								  f_pMarcStru->pMarcField[i].TagNo                                      ,
								  13,10 		                                                        );
		break;
	case -200:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 디렉토리 길이가 숫자가 아닙니다.%c%c") ,
			                      Time.wMonth,Time.wDay                                                 ,
								  Time.wHour, Time.wMinute                                              ,
								  Time.wSecond                                                          ,								  
								  13,10 		                                                        );
		break;
	case -201:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 [TAGNO] %s TAGNO가 숫자가 아닙니다.%c%c") ,
			                      Time.wMonth,Time.wDay                                                 ,
								  Time.wHour, Time.wMinute                                              ,
								  Time.wSecond                                                          ,
								  ctmp                                                                  ,
								  13,10 		                                                        );
		break;
	case -202:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 [TAGNO] %d DATA길이와 디렉토리의 필드 길이의 정보가 다릅니다.%c%c") ,
			                      Time.wMonth,Time.wDay                                                 ,
								  Time.wHour, Time.wMinute                                              ,
								  Time.wSecond                                                          ,
								  f_pMarcStru->pMarcField[i].TagNo                                      ,
								  13,10 		                                                        );
		break;
	case -204:
		_ftprintf( s_pErrorMarcFP , _T("%02d월 %02d일 %02d시 %02d분 %02d초 [TAGNO] %d 식별기호의 위치가 잘못되었읍니다.%c%c") ,
			                      Time.wMonth,Time.wDay                                                 ,
								  Time.wHour, Time.wMinute                                              ,
								  Time.wSecond                                                          ,
								  f_pMarcStru->pMarcField[i].TagNo                                    ,
								  13,10 		                                                        );
		break;
	}
    
    fclose(s_pErrorMarcFP);

FILENULL:
    for ( j=0; j < i ; j++ )
		  free( f_pMarcStru->pMarcField[j].pFldData_ElementStr );
    free ( f_pMarcStru->pMarcField );
           
	
	return ids;

    
}


VOID CMarcConvert::FreeMarcStru( tMARC_STRU f_MarcStru )
{
    

    INT     i;

    if ( f_MarcStru.pMarcField != NULL ) {
       for ( i=0; i < f_MarcStru.MarcFieldCount ; i++ ) {
           if ( f_MarcStru.pMarcField[i].pFldData_ElementStr != NULL )
              free(f_MarcStru.pMarcField[i].pFldData_ElementStr);
       }
       free(f_MarcStru.pMarcField);
    }
    return;    
}

/*================================================================================
  MARC_STRU 구조체의 내용을 참조하여 StreamMarc를 만든다.
  IN  : MARC_STRU 
  OUT : StreamMarc
================================================================================*/ 
INT CMarcConvert::WriteMarcStruToStreamMarc( tMARC_STRU  f_MarcStru, 
							   TCHAR       *f_pStreamMarc )
{
    tMARC_STRU  s_MarcInfo;
    INT     s_WritePos, s_FieldBasePos, s_FieldStartPos, s_FieldDataLength;
    INT     i,ids ;

    ids = ReadMarcStruFromStreamMarc( _T("") , &s_MarcInfo );
    if ( ids ) {
        FreeMarcStru( s_MarcInfo );
        return ids;
    }
    // 2000.2.22.수정. 049 Tag Data 정렬문제.
    // TagNO순으로 SORT
    ids = MarcStruSort( f_MarcStru , &s_MarcInfo );
    if( ids ){
        FreeMarcStru( s_MarcInfo );
    }	

    /// 1. Buffer Reset
    wmemset( f_pStreamMarc, ' ', MAX_MARC_SIZE );

    ///======== 2. Leader Structure Write
    s_WritePos = 0;
    memcpy( &f_pStreamMarc[s_WritePos], &f_MarcStru.MarcLeader, sizeof(tLEADER_STRU) );
	if( f_MarcStru.MarcLeader.IndicatorLength == ' ' ){
        f_MarcStru.MarcLeader.IndicatorLength = '2';
    }

    if( f_MarcStru.MarcLeader.SubFieldCodeLengh == ' ' ){
        f_MarcStru.MarcLeader.SubFieldCodeLengh = '2';
    }

    if( f_MarcStru.MarcLeader.MokRok_WriteType == ' ' ){
        f_MarcStru.MarcLeader.MokRok_WriteType = 'k';
    }

    if( f_MarcStru.MarcLeader.EntryMap[0] == ' ' ){
        _tcsnccpy(f_MarcStru.MarcLeader.EntryMap , _T("4500") , 4 );
    }

	if( f_MarcStru.MarcLeader.RecStatus == ' ' ){
		f_MarcStru.MarcLeader.RecStatus = 'n';
	}

	if( f_MarcStru.MarcLeader.RecType == ' ' ){
		f_MarcStru.MarcLeader.RecType = 'a';
	}

	if( f_MarcStru.MarcLeader.SeoJiLevel == ' ' ){
		f_MarcStru.MarcLeader.SeoJiLevel = 'm';
	}


    s_WritePos += LEARDER_SIZE ;

    ///======== 3. Directory Write
    if ( s_MarcInfo.MarcFieldCount > 0 )
        s_FieldBasePos = LEARDER_SIZE + 
		                (s_MarcInfo.MarcFieldCount * DIRECTORY_ENTRY_SIZE ) + 1; // 1 : FT
    else
        s_FieldBasePos = LEARDER_SIZE ;

    s_FieldStartPos = 0;
    for ( i=0; i < s_MarcInfo.MarcFieldCount ; i++ ) 
	{
        // 3.1 Tag No Write
		_stprintf( &f_pStreamMarc[s_WritePos] , _T("%03d") , s_MarcInfo.pMarcField[i].TagNo );
        s_WritePos += 3;

        // 3.2 Field Length Write
        // 제어필드와 데이터를 나누어 처리한다.
		s_FieldDataLength = _tcsclen(s_MarcInfo.pMarcField[i].pFldData_ElementStr);
        if ( s_MarcInfo.pMarcField[i].TagNo < 10 || s_MarcInfo.pMarcField[i].TagNo == 908 ) // Control Field 
           s_FieldDataLength += 1;   // FT
        else
		   s_FieldDataLength += 3;   // 제1/2지시기호, FT

        _stprintf( &f_pStreamMarc[s_WritePos], _T("%04d"), s_FieldDataLength );
        s_WritePos += 4;

        // 3.3 Field Start Pos Write ... 데이터기본번지(FieldBasePos) : 0
        _stprintf( &f_pStreamMarc[s_WritePos], _T("%05d"), s_FieldStartPos );
        s_FieldStartPos += s_FieldDataLength;
        s_WritePos += 5;
    }

    /// 3.4. FT Write
    if ( s_MarcInfo.MarcFieldCount != 0 )
       _stprintf( &f_pStreamMarc[s_WritePos++], _T("%c"), s_MarcInfo.pMarcField[0].FieldTerm );

    ///======== 4. DATA FIELD Write
    for ( i=0; i < s_MarcInfo.MarcFieldCount; i++ ) {

        //  4.1 제어필드와 데이터를 나누어 처리한다.
        if ( s_MarcInfo.pMarcField[i].TagNo < 10 || s_MarcInfo.pMarcField[i].TagNo == 908 ) {  // Control
            _tcscpy( &f_pStreamMarc[s_WritePos], &s_MarcInfo.pMarcField[i].pFldData_ElementStr[0] );
            s_WritePos += _tcsclen( &s_MarcInfo.pMarcField[i].pFldData_ElementStr[0] );
        } else {
            f_pStreamMarc[s_WritePos++] = s_MarcInfo.pMarcField[i].fIndicator;
            f_pStreamMarc[s_WritePos++] = s_MarcInfo.pMarcField[i].sIndicator;
            _tcscpy( &f_pStreamMarc[s_WritePos], &s_MarcInfo.pMarcField[i].pFldData_ElementStr[0] );
            s_WritePos += _tcsclen( &s_MarcInfo.pMarcField[i].pFldData_ElementStr[0] );
        }

        _stprintf( &f_pStreamMarc[s_WritePos++], _T("%c"), s_MarcInfo.pMarcField[i].FieldTerm );
    }

    _stprintf( &f_pStreamMarc[s_WritePos], _T("%c"),s_MarcInfo.RecordTerm );

    /// 5. Leader : RecordLength Write
    _stprintf(s_MarcData, _T("%05d"), s_WritePos+1 );
    _tcsnccpy( &f_pStreamMarc[0], s_MarcData, 5 );

    /// 6. Leader : FieldBasePos Write
    _stprintf(s_MarcData, _T("%05d"), s_FieldBasePos );
    _tcsnccpy( &f_pStreamMarc[12], s_MarcData, 5 );

    FreeMarcStru( s_MarcInfo );

	if( f_MarcStru.MarcLeader.IndicatorLength == ' ' ){
        f_pStreamMarc[10] = '2';
    }

    if( f_MarcStru.MarcLeader.SubFieldCodeLengh == ' ' ){
        f_pStreamMarc[11] = '2';
    }

    if( f_MarcStru.MarcLeader.MokRok_WriteType == ' ' ){
        f_pStreamMarc[18] = 'k';
    }

    if( f_MarcStru.MarcLeader.EntryMap[0] == ' ' ){
        _tcsnccpy(&f_pStreamMarc[20] , _T("4500") , 4 );
    }

	if( f_MarcStru.MarcLeader.RecStatus == ' ' ){
		f_pStreamMarc[5] = 'n';
	}

	if( f_MarcStru.MarcLeader.RecType == ' ' ){
		f_pStreamMarc[6] = 'a';
	}

	if( f_MarcStru.MarcLeader.SeoJiLevel == ' ' ){
		f_pStreamMarc[7] = 'm';
	}

    return 0;    
}


/*================================================================================
  MarcStru에서 TagNo의 데이타 필드를 삭제한다.
  IN      :  INT  Flag
                  'A' : Marc내의 반복 Tag 모두 삭제
                  '1' : Marc내의 첫번째 Tag만 삭제
             INT  TagNo
  IN/OUT  :  tMARC_STRU * MarcStru
================================================================================*/
INT CMarcConvert::RemoveField( TCHAR          f_Flag   ,
                     INT           f_TagNo  ,
                     tMARC_STRU  * f_pMarcStru )
{
    

    INT         i, j;

    /// 1. TAG을 삭제한다.
    

    for ( i=0 ; i < f_pMarcStru->MarcFieldCount ; i++ )
    {
        if ( f_TagNo != f_pMarcStru->pMarcField[i].TagNo )
           continue;

        // 2.1 MarcField Left Shift
        for ( j=i+1 ; j< f_pMarcStru->MarcFieldCount ; j++ )
        {
            f_pMarcStru->pMarcField[j-1].TagNo = f_pMarcStru->pMarcField[j].TagNo;
            f_pMarcStru->pMarcField[j-1].fIndicator = f_pMarcStru->pMarcField[j].fIndicator;
            f_pMarcStru->pMarcField[j-1].sIndicator = f_pMarcStru->pMarcField[j].sIndicator;
            f_pMarcStru->pMarcField[j-1].FieldTerm  = f_pMarcStru->pMarcField[j].FieldTerm ;
            f_pMarcStru->pMarcField[j-1].pFldData_ElementStr = 
                          (TCHAR*)realloc( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr,
                                          _tcsclen( f_pMarcStru->pMarcField[j].pFldData_ElementStr)+sizeof(TCHAR));
            if ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr == NULL )
            {
               return -1;
            } 
            _stprintf ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr , _T("%s") ,
                      f_pMarcStru->pMarcField[j].pFldData_ElementStr ); 
        }

        // 2.2 마지막 필드의 데이타의 메모리 해제
        free ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr );

        // 2.3 MarcFieldCount Sub
        f_pMarcStru->MarcFieldCount--;
        i--;

        if ( f_Flag == '1' )
           break;
    }

    /// 3. MarcField 를 삭제된 수 만큼 메모리에서 제거한다.
    f_pMarcStru->pMarcField = ( tFIELD_STRU *) realloc( f_pMarcStru->pMarcField,
		                                            f_pMarcStru->MarcFieldCount * sizeof( tFIELD_STRU ) );
    if ( f_pMarcStru->pMarcField == NULL )
    {
        if ( f_pMarcStru->MarcFieldCount == 0 ) return 0;
		return -2;
    }

    return 0;

    
}

/*================================================================================
  StreamMarc에서 TagNo의 데이타 필드를 삭제한다.
  IN      :  INT  Flag
                  'A' : Marc내의 반복 Tag 모두 삭제
                  '1' : Marc내의 첫번째 Tag만 삭제
             INT  TagNo
  IN/OUT  :  TCHAR *StreamMarc
================================================================================*/
INT CMarcConvert::RemoveField( TCHAR       f_Flag  ,
                     INT        f_TagNo ,
                     TCHAR     * f_pStreamMarc )
{
    INT         i, j, ids;
    tMARC_STRU  s_MarcStru;

    /// 1. MarcData를 분석한다.
    ids = ReadMarcStruFromStreamMarc( f_pStreamMarc, &s_MarcStru );
    if ( ids ) return ids;

    /// 2. TAG을 삭제한다.
    for ( i=0 ; i < s_MarcStru.MarcFieldCount ; i++ )
    {
        if ( f_TagNo != s_MarcStru.pMarcField[i].TagNo )
           continue;

        // 2.1 MarcField Left Shift
        for ( j=i+1 ; j< s_MarcStru.MarcFieldCount ; j++ )
        {
            s_MarcStru.pMarcField[j-1].TagNo = s_MarcStru.pMarcField[j].TagNo;
            s_MarcStru.pMarcField[j-1].fIndicator = s_MarcStru.pMarcField[j].fIndicator;
            s_MarcStru.pMarcField[j-1].sIndicator = s_MarcStru.pMarcField[j].sIndicator;
            s_MarcStru.pMarcField[j-1].FieldTerm  = s_MarcStru.pMarcField[j].FieldTerm ;
            s_MarcStru.pMarcField[j-1].pFldData_ElementStr = 
                          (TCHAR*)realloc( s_MarcStru.pMarcField[j-1].pFldData_ElementStr,
                                          _tcsclen( s_MarcStru.pMarcField[j].pFldData_ElementStr)+sizeof(TCHAR));
            if ( s_MarcStru.pMarcField[j-1].pFldData_ElementStr == NULL )
            {
               FreeMarcStru ( s_MarcStru );
               return -2;
            } 
            _stprintf ( s_MarcStru.pMarcField[j-1].pFldData_ElementStr , _T("%s") ,
                      s_MarcStru.pMarcField[j].pFldData_ElementStr ); 
        }

        // 2.2 마지막 필드의 데이타의 메모리 해제
        free ( s_MarcStru.pMarcField[j-1].pFldData_ElementStr );

        // 2.3 MarcFieldCount Sub
        s_MarcStru.MarcFieldCount--;
        i--;

        if ( f_Flag == '1' )
           break;
    }

    /// 3. MarcField 를 삭제된 수 만큼 메모리에서 제거한다.
    s_MarcStru.pMarcField = ( tFIELD_STRU *) realloc( s_MarcStru.pMarcField,
		                                              s_MarcStru.MarcFieldCount * sizeof( tFIELD_STRU ) );
    if ( s_MarcStru.pMarcField == NULL )
    {
        if ( s_MarcStru.MarcFieldCount == 0 ) return 0;
        return -3;
    }

    /// 4. MarcData를 생성
    ids = WriteMarcStruToStreamMarc( s_MarcStru, f_pStreamMarc );

    /// 5. Resource Free
    FreeMarcStru( s_MarcStru );

    return ids;
}

/*================================================================================
  MarcStru에서 TagNo의 식별기호 에 해당하는 Element 를 삭제한다.
  IN     : INT          TagNo
           TCHAR       * SubFieldCode
  IN/OUT : tMARC_STRU * MarcStru
================================================================================*/
INT CMarcConvert::RemoveElement( INT          f_TagNo        ,
                       TCHAR       * f_pSubFieldCode ,
                       tMARC_STRU * f_pMarcStru )
{
    

    TCHAR        **s_pElementTokenData, *s_ptr;
    INT         i, j, ids, s_ElementTokenCount;

    /// 1. 해당식별자료를 찾아내어 모두삭제한다.
    for ( i=0; i < f_pMarcStru->MarcFieldCount ; i++ )
    {
        // 1.1 TagNo Find
        if( f_TagNo < 10 ) continue;
        if ( f_TagNo != f_pMarcStru->pMarcField[i].TagNo ) continue;

        // 1.2 Tokened ElementString             
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                  &s_ElementTokenCount                          ,
                                                  &s_pElementTokenData );
        if ( ids )
        {
            return -1;
        }                

        // 1.3 SubFieldCode 에 해당하는 Element 를 삭제하여 pFldData_ElementStr 에 저장한다.
        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = 0;
        s_ptr = f_pMarcStru->pMarcField[i].pFldData_ElementStr;
        for ( j=0; j < s_ElementTokenCount ; j++ )
        {
            if ( s_pElementTokenData[j][1] == f_pSubFieldCode[1] ) continue;
            _stprintf( &s_ptr[_tcsclen(s_ptr)], _T("%s") , s_pElementTokenData[j] );
        }

        //if( j >= s_ElementTokenCount ) return 0;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] == '\0' ){
            // 2.2 메모리 이동
            for ( j=i+1 ; j< f_pMarcStru->MarcFieldCount ; j++ ){
                f_pMarcStru->pMarcField[j-1].TagNo      = f_pMarcStru->pMarcField[j].TagNo;
                f_pMarcStru->pMarcField[j-1].fIndicator = f_pMarcStru->pMarcField[j].fIndicator;
                f_pMarcStru->pMarcField[j-1].sIndicator = f_pMarcStru->pMarcField[j].sIndicator;
                f_pMarcStru->pMarcField[j-1].FieldTerm  = f_pMarcStru->pMarcField[j].FieldTerm ;
                f_pMarcStru->pMarcField[j-1].pFldData_ElementStr = 
                              (TCHAR*)realloc( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr,
                                              _tcsclen( f_pMarcStru->pMarcField[j].pFldData_ElementStr)+sizeof(TCHAR));
                if ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr == NULL )return -1;
                
                _stprintf ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr , _T("%s") ,
                          f_pMarcStru->pMarcField[j].pFldData_ElementStr ); 
            }
            // 2.2 마지막 필드의 데이타의 메모리 해제
            free ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr );
            // 2.3 MarcFieldCount Sub
            f_pMarcStru->MarcFieldCount--;

            /// 3. MarcField 를 삭제된 수 만큼 메모리에서 제거한다.
            f_pMarcStru->pMarcField = ( tFIELD_STRU *)realloc( f_pMarcStru->pMarcField,
		                                                       f_pMarcStru->MarcFieldCount * sizeof( tFIELD_STRU ) );
        }
        // 1.4 Tokened 된 Element 의 메모리를 해제한다.
        FreeTwoDim( s_ElementTokenCount , s_pElementTokenData );
    }

    return 0;

    
}

/*================================================================================
  MarcStru에서 TagNo의 식별기호 에 해당하는 SubFieldData를 삭제한다.
  IN     : INT          TagNo
           TCHAR       * SubFieldCode
           TCHAR       * SubFieldData

  IN/OUT : tMARC_STRU * MarcStru
================================================================================*/
INT CMarcConvert::RemoveElement( INT         f_TagNo         ,
                       TCHAR       *f_pSubFieldCode ,
                       TCHAR       *f_pSubFieldData ,
                       tMARC_STRU *f_pMarcStru )
{
    

    TCHAR        s_SubField[1024];
    TCHAR        **s_pElementTokenData, *s_ptr,*s_pStrptr;
    INT         i, j, ids, s_ElementTokenCount;

    _stprintf( s_SubField ,_T("%s%s"),f_pSubFieldCode,f_pSubFieldData);

    /// 1. 해당식별자료를 찾아내어 모두삭제한다.
    for ( i=0; i < f_pMarcStru->MarcFieldCount ; i++ )
    {
        // 1.1 TagNo Find
        if( f_TagNo < 10 ) continue;
        if ( f_TagNo != f_pMarcStru->pMarcField[i].TagNo ) continue;

        // 1.2 Tokened ElementString             
        ids = GetTokenedElementFromElementString( f_pMarcStru->pMarcField[i].pFldData_ElementStr ,
                                                  &s_ElementTokenCount                          ,
                                                  &s_pElementTokenData );
        if ( ids )
        {
            return -1;
        }                

        // 1.3 SubFieldCode 에 해당하는 Element 를 삭제하여 pFldData_ElementStr 에 저장한다.
        f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] = 0;
        s_ptr = f_pMarcStru->pMarcField[i].pFldData_ElementStr;
        for ( j=0; j < s_ElementTokenCount ; j++ )
        {
            s_pStrptr = _tcsstr( s_pElementTokenData[j] , s_SubField );
            if ( s_pStrptr == NULL ){
                _stprintf( &s_ptr[_tcsclen(s_ptr)], _T("%s") , s_pElementTokenData[j] );
            }
        }
         

        //if( j >= s_ElementTokenCount ) return 0;

        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] == '\0' ){
            // 2.2 메모리 이동
            for ( j=i+1 ; j< f_pMarcStru->MarcFieldCount ; j++ ){
                f_pMarcStru->pMarcField[j-1].TagNo      = f_pMarcStru->pMarcField[j].TagNo;
                f_pMarcStru->pMarcField[j-1].fIndicator = f_pMarcStru->pMarcField[j].fIndicator;
                f_pMarcStru->pMarcField[j-1].sIndicator = f_pMarcStru->pMarcField[j].sIndicator;
                f_pMarcStru->pMarcField[j-1].FieldTerm  = f_pMarcStru->pMarcField[j].FieldTerm ;
                f_pMarcStru->pMarcField[j-1].pFldData_ElementStr = 
                              (TCHAR*)realloc( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr,
                                              _tcsclen( f_pMarcStru->pMarcField[j].pFldData_ElementStr)+sizeof(TCHAR));
                if ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr == NULL )return -1;
                
                _stprintf ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr , _T("%s") ,
                          f_pMarcStru->pMarcField[j].pFldData_ElementStr ); 
            }
            // 2.2 마지막 필드의 데이타의 메모리 해제
            free ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr );
            // 2.3 MarcFieldCount Sub
            f_pMarcStru->MarcFieldCount--;

            /// 3. MarcField 를 삭제된 수 만큼 메모리에서 제거한다.
            f_pMarcStru->pMarcField = ( tFIELD_STRU *)realloc( f_pMarcStru->pMarcField,
		                                                       f_pMarcStru->MarcFieldCount * sizeof( tFIELD_STRU ) );
        }
        // 1.4 Tokened 된 Element 의 메모리를 해제한다.
        FreeTwoDim( s_ElementTokenCount , s_pElementTokenData );
    }

    return 0;

    
}

/*================================================================================
  StreamMarc에서 TagNo의 식별기호 에 해당하는 Element 를 삭제한다.
  IN     : INT    TagNo
           TCHAR * SubFieldCode
  IN/OUT : TCHAR * StreamMarc
================================================================================*/
INT CMarcConvert::RemoveElement( INT        f_TagNo   ,
                       TCHAR     * f_pSubFieldCode,
                       TCHAR     * f_pStreamMarc )
{
    

    INT         ids;
    tMARC_STRU  s_MarcStru;

    /// 1. MarcData를 분석한다.
    ids = ReadMarcStruFromStreamMarc( f_pStreamMarc, &s_MarcStru );
    if ( ids ) return ids;
 
    /// 2. 식별기호에 해당하는 자료를 삭제
    ids =  RemoveElement( f_TagNo ,
                          f_pSubFieldCode ,
                          &s_MarcStru );
    if ( ids ) { FreeMarcStru( s_MarcStru ); return ids; }

    /// 3. StreamMarc 를 다시 만든다.
    ids = WriteMarcStruToStreamMarc( s_MarcStru, f_pStreamMarc );

    /// 4. ReadMarcStruToFromStreamMarc 에서 할당된 메모리를 해제한다. 
    FreeMarcStru( s_MarcStru );
 
    return ids;

    
}

/*================================================================================
  인자 ElementString에서 Element를 토큰한다.
  
  IN  : TCHAR      *ElementString    : 식별기호+식별자료.....
  OUT : INT       *ElementTokenCount
        TCHAR    ***ElementTokenData 

  // @ ElementString[]    : [$a...%b...$c...]
  // @ ElementTokenData[] : [$a...]
  //                        [$b...]
  //                        [$c...]

================================================================================*/
INT  CMarcConvert::GetTokenedElementFromElementString( TCHAR      *f_pElementString     ,
                                         INT       *f_pElementTokenCount ,
                                         TCHAR    ***f_pElementTokenData  ,
                                         BOOL       f_DelSpecialCharFlag )
{    
    TCHAR    *stoken, **s_pTmpElementTokenData, s_ElementTokenStr[64];
    INT     i;

    _tcscpy( s_FieldData, f_pElementString );

    (TCHAR**)s_pTmpElementTokenData = NULL;
    *f_pElementTokenCount = 0;


	
    /// 1. Tag 자료를 식별자료 단위로 Token
    _stprintf( s_ElementTokenStr,_T("%c"), GT );
    stoken = _tcstok( &s_FieldData[0], &s_ElementTokenStr[0] );
    while( stoken != NULL ) {
        s_pTmpElementTokenData = (TCHAR **)realloc( s_pTmpElementTokenData,
                                           sizeof( TCHAR *) * (*f_pElementTokenCount +sizeof(TCHAR)) );
        if ( s_pTmpElementTokenData == NULL ) return -1;

        s_pTmpElementTokenData[*f_pElementTokenCount] = (TCHAR *)malloc( _tcsclen(stoken) + (sizeof(TCHAR)* (sizeof(TCHAR)*4)) ); // 4 : $ + NULL
        if ( s_pTmpElementTokenData[*f_pElementTokenCount] == NULL ) {
           for ( i=0; i < *f_pElementTokenCount + 1 ; i++ )
               if ( s_pTmpElementTokenData[i] != NULL ) free(s_pTmpElementTokenData[i]);
           free(s_pTmpElementTokenData);

		   return -2;
        }

        _stprintf( &s_pTmpElementTokenData[*f_pElementTokenCount][0], _T("%c"), GT );
        _tcscpy ( &s_pTmpElementTokenData[*f_pElementTokenCount][1], stoken );
        stoken = _tcstok( NULL, &s_ElementTokenStr[0] );

        (*f_pElementTokenCount)++;
    }

    /// 2. Data를 반환 메모리에 저장
    *f_pElementTokenData = s_pTmpElementTokenData;

    if( f_DelSpecialCharFlag == FALSE ){
        for( i = 0; i < (*f_pElementTokenCount); i++){
            DelSpecialChar( f_pElementTokenData[0][i] , f_pElementTokenData[0][i] , FALSE );
        }
    }
    return 0;
}

/*================================================================================
  StreamMarc에서 OldElement를 NewElement로 수정한다.
  IN      :  INT          f_TagNo
	         TCHAR       * f_pSubFieldCode   ,
             TCHAR       * f_pOldSubFieldData,
             TCHAR       * f_pNewSubFieldData,
  IN/OUT  :  tMARC_STRU * MarcStru
================================================================================*/
INT CMarcConvert::UpdateSubFieldDataToMarcStru( INT          f_TagNo           ,
				                  TCHAR       * f_pSubFieldCode   ,
                                  TCHAR       * f_pOldSubFieldData,
                                  TCHAR       * f_pNewSubFieldData,
                                  tMARC_STRU * f_pMarcStru )
{
    


    //GOLD SOURCE
    INT      ids;
	TCHAR     s_OldElement[4096], s_NewElement[4096];

	_stprintf( s_OldElement , _T("%s%s") , f_pSubFieldCode , f_pOldSubFieldData );

    if ( f_pNewSubFieldData[0] == 0 )
    	s_NewElement[0] = 0;
    else
    	_stprintf( s_NewElement , _T("%s%s") , f_pSubFieldCode , f_pNewSubFieldData );

	ids = UpdateElement( f_TagNo,
                         s_OldElement,
                         s_NewElement,
                         f_pMarcStru );

    if( ids ) return ids;
    

	return 0;
 
}


VOID CMarcConvert::FreeTwoDim( INT    f_Cnt, TCHAR **f_pData )
{    
    INT     i;

    if ( f_pData != NULL ) {
       for ( i=0; i < f_Cnt; i++ )
           if ( f_pData[i] != NULL ) free( f_pData[i] );

       free( f_pData );
    }
    return;
}

//////////////////////////////////////////////////////////////////////////////////
//  DESC : FIELD 자료를 수정한다. FieldData는 필드중에서 %만 뺀것
//  FieldData :  [KMO000000001]
//               [00$a..........$b............] 
//////////////////////////////////////////////////////////////////////////////////
INT     CMarcConvert::UpdateFieldDataToMarcStru(
                    INT          f_TagNo        ,
                    TCHAR        *f_pOldFieldData,
                    TCHAR        *f_pNewFieldData   ,
                    tMARC_STRU  *f_pMarcStru )
{
    

    
    TCHAR     *ptr;
    TCHAR     s_fIndicator, s_sIndicator;
    INT      i;


    /// 1. 
    for ( i=0; i < f_pMarcStru->MarcFieldCount; i++ ) {
        if ( f_TagNo != f_pMarcStru->pMarcField[i].TagNo ) continue;
        if ( f_TagNo < 10 || f_TagNo == 908 ) {
            _tcscpy( s_FieldData2, f_pNewFieldData );
            _tcscpy( s_FieldData, f_pOldFieldData );
            s_fIndicator = s_sIndicator = ' ';
        } else {
            _tcscpy( s_FieldData2, &f_pNewFieldData[2] );
            _tcscpy( s_FieldData, &f_pOldFieldData[2] );
            s_fIndicator = f_pNewFieldData[0];
            s_sIndicator = f_pNewFieldData[1];
        }
        if ( _tcsicmp( f_pMarcStru->pMarcField[i].pFldData_ElementStr, s_FieldData ) == 0 ) {
           f_pMarcStru->pMarcField[i].fIndicator = s_fIndicator;
           f_pMarcStru->pMarcField[i].sIndicator = s_sIndicator;
           ptr = f_pMarcStru->pMarcField[i].pFldData_ElementStr;
           f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR*)realloc(
                                       f_pMarcStru->pMarcField[i].pFldData_ElementStr,
                                       _tcsclen(s_FieldData2)+sizeof(TCHAR) );
           if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) {
              f_pMarcStru->pMarcField[i].pFldData_ElementStr = ptr;
              return -1;
           }
           _tcscpy( f_pMarcStru->pMarcField[i].pFldData_ElementStr, s_FieldData2 );
           break;
        }
    }
    if ( i >= f_pMarcStru->MarcFieldCount ) return -2;

    return 0;

    
}

INT CMarcConvert::GetSubFieldDataFromDataField( TCHAR   *f_pSubFieldCode ,     // 식별기호
                                  TCHAR   *f_pDataField    ,
                                  TCHAR   *f_pSubFieldData ,
                                  TCHAR    f_pDelFlag      )
{   

    TCHAR    *stoken, *gtoken;
    TCHAR    SBTokenStr[64],s_tmpSubFieldCode[3];
    TCHAR    s_PuncMark;
	BOOL    s_Flag = TRUE;
    _tcscpy( s_FieldData, f_pDataField );
    //////////////////////////////////////////////////////////////////////
    // 1. 해당 식별기호를 검색한다.($)
    //////////////////////////////////////////////////////////////////////
    stoken = _tcsstr( s_FieldData, f_pSubFieldCode );
    if ( stoken == NULL ) { 
        f_pSubFieldData[0] = 0;
        return 0; 
    }

    s_tmpSubFieldCode[0] = '\0';
    s_tmpSubFieldCode[1] = '\0';

    //////////////////////////////////////////////////////////////////////
    // 2. 해당 식별기호의 자료만 구할수 있도록 token을 수행한다.
    //////////////////////////////////////////////////////////////////////
    _stprintf( SBTokenStr,_T("%c%c%c"), GT, FT, RT );
	if( stoken[_tcsclen(stoken)-1] == RT ||
		stoken[_tcsclen(stoken)-1] == FT ){
		s_Flag = FALSE;
	}
    gtoken = _tcstok( stoken, SBTokenStr );  // gtoken = $a123 에서 a부터 가리킴.
	_stprintf ( f_pSubFieldData , _T("%s") , gtoken+1 );

	if( s_Flag == TRUE && f_pDelFlag != 'A' ){
		s_PuncMark = f_pSubFieldData[_tcsclen(f_pSubFieldData) -1];
		if( s_PuncMark == ';' || s_PuncMark == ':' ||
			s_PuncMark == '/' || s_PuncMark == '-' ||
			s_PuncMark == '=' || s_PuncMark == ',' ||
			s_PuncMark == '.' || s_PuncMark == '+' ||
			s_PuncMark == '%' ){
			f_pSubFieldData[_tcsclen(f_pSubFieldData) -1] = '\0';
		}
	}
    else{
        gtoken = _tcstok( NULL, SBTokenStr );  // gtoken = $a123 에서 a부터 가리킴.
        if( gtoken != NULL ){
            _tcsnccpy( s_tmpSubFieldCode , gtoken , 2 );
            s_tmpSubFieldCode[2] = '\0';
        }

        if( s_tmpSubFieldCode[0] == 'b'|| s_tmpSubFieldCode[0] == 'l'||
            s_tmpSubFieldCode[0] == 'n'|| s_tmpSubFieldCode[0] == 't'||
            s_tmpSubFieldCode[0] == 'g'|| s_tmpSubFieldCode[0] == 'k'||
            s_tmpSubFieldCode[0] == 'e' ){
            s_PuncMark = f_pSubFieldData[_tcsclen(f_pSubFieldData) -1];
            if( s_PuncMark == '.' ) f_pSubFieldData[_tcsclen(f_pSubFieldData) -1] = '\0';
        }else
        if( s_tmpSubFieldCode[0] == 'c' || s_tmpSubFieldCode[0] == 'd' ||
            s_tmpSubFieldCode[0] == 'p' || s_tmpSubFieldCode[0] == 'f' ||
            s_tmpSubFieldCode[0] == 'g' ){
            s_PuncMark = f_pSubFieldData[_tcsclen(f_pSubFieldData) -1];
            if( s_PuncMark == ',' ) f_pSubFieldData[_tcsclen(f_pSubFieldData) -1] = '\0';
        }else
        if( s_tmpSubFieldCode[0] == 'h'  ){
            s_PuncMark = f_pSubFieldData[_tcsclen(f_pSubFieldData) -1];
            if( s_PuncMark == '=' ) f_pSubFieldData[_tcsclen(f_pSubFieldData) -1] = '\0';
        }else
        if( s_tmpSubFieldCode[0] == 'x' || s_tmpSubFieldCode[0] == 'y' ||
            s_tmpSubFieldCode[0] == 'z' ){
            s_PuncMark = f_pSubFieldData[_tcsclen(f_pSubFieldData) -1];
            if( s_PuncMark == '-' ) f_pSubFieldData[_tcsclen(f_pSubFieldData) -1] = '\0';
        }
    }

    return 0;

    
}


VOID    CMarcConvert::str_ltrim	(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
							)
{
 
	INT i = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ){
        if(strln[i] != var) {
            _tcscpy( strline1, &strln[i] );
            return;
        }
    }
    strline1[0] = '\0';

}

VOID    CMarcConvert::str_rtrim	(
								TCHAR   *strln   ,
		                        TCHAR   *strline1,
		                        TCHAR    var
							)
{
 
	INT i = 0;

    _tcscpy( strline1, strln );
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
        if(strln[i] != var) {
            strline1[i+1] = '\0';
            return;
        }
    }
    strline1[0] = '\0';

}

INT CMarcConvert::InsertFieldDataToMarcStru(
                    INT          f_TagNo      ,
                    TCHAR        *f_pFieldData ,
                    tMARC_STRU  *f_pMarcStru )
{
    INT                i, ids;

    if( f_pFieldData[0] == '\0') return 0;
    /// 1. 
    for ( i = 0; i < f_pMarcStru->MarcFieldCount; i++ ) {
        if ( f_TagNo < f_pMarcStru->pMarcField[i].TagNo ) {
            if ( f_TagNo < 10 || f_TagNo == 908 ) {
                ids = InsertFieldStruToMarcStru( f_TagNo, ' ', ' ', f_pFieldData, FT, i, f_pMarcStru );
                if ( ids ) {return ids;}
            } 
            else {
                ids = InsertFieldStruToMarcStru( f_TagNo, f_pFieldData[0], f_pFieldData[1],
                                                &f_pFieldData[2], FT, i, f_pMarcStru );
                if ( ids ) {return ids;}
            }
            break;
        }
    }
    if ( i >= f_pMarcStru->MarcFieldCount ) {
        if ( f_TagNo < 10 || f_TagNo == 908 ) {
            ids = InsertFieldStruToMarcStru( f_TagNo, ' ', ' ', f_pFieldData , FT, i, f_pMarcStru );
            if ( ids ) {return ids;}
        } else {
            ids = InsertFieldStruToMarcStru( f_TagNo, f_pFieldData[0], f_pFieldData[1],
                                            &f_pFieldData[2], FT, i, f_pMarcStru );
            if ( ids ) {return ids;}
        }
    }

    return 0;
}

INT CMarcConvert::UpdateElement( INT          f_TagNo     ,
	    		   TCHAR       * f_pOldElement,
				   TCHAR       * f_pNewElement,
				   tMARC_STRU * f_pMarcStru )
{   
    
    INT         i,j;
    TCHAR        *s_ptr, *stoken,*s_pstrptr;
    BOOL        s_RemoveFlag;
    TCHAR        s_SubFieldCode[3];


    s_RemoveFlag = FALSE;
    /*
    s_InsertEleco   eementLength = s_TotalMarcLength = 0;
    // 1.Marc길이가 100000바이트가 넘는지를 검사
    ids = WriteMarcStruToStreamMarc( *f_pMarcStru , s_MarcData );
    if( ids < 0 ) return -1;

    s_InsertElementLength = _tcsclen(f_pNewElement) - _tcsclen(f_pOldElement);

    s_TotalMarcLength = s_InsertElementLength + _tcsclen( s_MarcData );
    if( s_TotalMarcLength > 100000 ){
        AfxMessageBox(_T("MARC 의 길이가 100000 바이트를 초과 했습니다."));
        return 100;
    }*/


    /// 1. TAG의 OldElement를 찾아서 NewElementData를  Update한다.
    for ( i=0; i < f_pMarcStru->MarcFieldCount ; i++ ) {
		if ( f_TagNo != f_pMarcStru->pMarcField[i].TagNo ) continue;

		_stprintf( s_FieldData2 , _T("%s") , f_pMarcStru->pMarcField[i].pFldData_ElementStr );

        stoken = _tcsstr( s_FieldData2 , f_pOldElement );
        if ( stoken == NULL ) {
            continue;
        }
        

		// Element Update
		_tcsnccpy( s_FieldData , s_FieldData2 , stoken-s_FieldData2 );
        if ( f_pNewElement[0] == 0 && 
             ( *(s_FieldData + (stoken-s_FieldData2)-1) == '.' || 
               *(s_FieldData + (stoken-s_FieldData2)-1) == ',' || 
               *(s_FieldData + (stoken-s_FieldData2)-1) == '=' || 
               *(s_FieldData + (stoken-s_FieldData2)-1) == ':' || 
               *(s_FieldData + (stoken-s_FieldData2)-1) == ';' ||
               *(s_FieldData + (stoken-s_FieldData2)-1) == '+' ||
               *(s_FieldData + (stoken-s_FieldData2)-1) == '/' ||
               *(s_FieldData + (stoken-s_FieldData2)-1) == '%') ) {
		    _tcscpy( s_FieldData + (stoken-s_FieldData2)-1 , f_pNewElement );
        } else {
            if(f_TagNo == 245 && f_pOldElement[1] == 'p' ){
                _stprintf( s_SubFieldCode , _T("%cn") , GT );
                s_pstrptr = _tcsstr( s_FieldData2 , s_SubFieldCode );
                if( s_pstrptr != NULL ){
                    _stprintf( s_FieldData + (stoken-s_FieldData2)-1 ,_T(",%s") , f_pNewElement ) ;

                }
                else{
                    _stprintf( s_FieldData + (stoken-s_FieldData2)-1 , _T(".%s") , f_pNewElement ) ;

                }
            }
            else{
		        _tcscpy( s_FieldData + (stoken-s_FieldData2), f_pNewElement );
            }
        }

		_tcscpy( s_FieldData + _tcsclen(s_FieldData) ,
				    s_FieldData2 + ( stoken-s_FieldData2) + _tcsclen(f_pOldElement) ) ;
        
        
        if ( s_FieldData[0] == '.' || s_FieldData[0] == ',' ||
             s_FieldData[0] == '=' || s_FieldData[0] == ':' ||
             s_FieldData[0] == ';' || s_FieldData[0] == '/' ||
             s_FieldData[0] == '+' || s_FieldData[0] == '%' ) {
            _tcscpy( &s_FieldData[0], &s_FieldData[1] );
        }

        // Update CookElementString
        s_ptr = f_pMarcStru->pMarcField[i].pFldData_ElementStr;
        f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR *)realloc(
                                    f_pMarcStru->pMarcField[i].pFldData_ElementStr,
                                    _tcsclen(s_FieldData) +sizeof(TCHAR) );
		if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) 
		{
			f_pMarcStru->pMarcField[i].pFldData_ElementStr = s_ptr;
			return -2;
		}
        _tcscpy( f_pMarcStru->pMarcField[i].pFldData_ElementStr, s_FieldData );
        
        if( f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] == '\0' ){
            s_RemoveFlag = TRUE;
            // 2.2 메모리 이동
            for ( j=i+1 ; j< f_pMarcStru->MarcFieldCount ; j++ ){
                f_pMarcStru->pMarcField[j-1].TagNo      = f_pMarcStru->pMarcField[j].TagNo;
                f_pMarcStru->pMarcField[j-1].fIndicator = f_pMarcStru->pMarcField[j].fIndicator;
                f_pMarcStru->pMarcField[j-1].sIndicator = f_pMarcStru->pMarcField[j].sIndicator;
                f_pMarcStru->pMarcField[j-1].FieldTerm  = f_pMarcStru->pMarcField[j].FieldTerm ;
                f_pMarcStru->pMarcField[j-1].pFldData_ElementStr = 
                              (TCHAR*)realloc( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr,
                                              _tcsclen( f_pMarcStru->pMarcField[j].pFldData_ElementStr)+sizeof(TCHAR));
                if ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr == NULL )return -4;
                
                _stprintf ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr , _T("%s") ,
                          f_pMarcStru->pMarcField[j].pFldData_ElementStr ); 
            }
            // 2.2 마지막 필드의 데이타의 메모리 해제
            free ( f_pMarcStru->pMarcField[j-1].pFldData_ElementStr );
            // 2.3 MarcFieldCount Sub
            f_pMarcStru->MarcFieldCount--;

            /// 3. MarcField 를 삭제된 수 만큼 메모리에서 제거한다.
            f_pMarcStru->pMarcField = ( tFIELD_STRU *)realloc( f_pMarcStru->pMarcField,
		                                                       f_pMarcStru->MarcFieldCount * sizeof( tFIELD_STRU ) );
        }
        
	    break;
	}

    if ( i >= f_pMarcStru->MarcFieldCount && s_RemoveFlag != TRUE ) {
        TCHAR ErrorStr[1024];
        _stprintf( ErrorStr , 
			     _T("[수정에러] OLD DATA정보가 다릅니다.\n%03d FIELD DATA : [%s]\n       OLD DATA : [%s]\n    UPDATE DATA : [%s]"), 
			     f_TagNo , s_FieldData2, f_pOldElement,f_pNewElement);
        AfxMessageBox( ErrorStr );
        return -5;
    }

    return 0;    
}

INT CMarcConvert::InsertSubFieldDataToMarcStru(
                    INT          f_TagNo         ,
                    TCHAR        *f_pSubFieldCode ,
                    TCHAR        *f_pSubFieldData ,
                    tMARC_STRU  *f_pMarcStru     ,
                    BOOL         f_PuncMarkUsageFlag,  // 구두점 적용여부
                    TCHAR         f_PuncMark       ,    // 구두점,
                    TCHAR         f_fIndicator     ,    // 제1지시기호
                    TCHAR         f_sIndicator     )    // 제2지시기호
{   
    bool               s_PuncMarkExistFlag;
    TCHAR             **s_pTokenedElements, *s_pFldData_ElementStr;
    TCHAR               s_PostFuncMark,s_fIndicator,s_sIndicator;
    INT                s_TokenedElementCount, i, j, k, ids;
    INT                s_AllocLen,s_InsertSubFieldLength,s_TotalMarcLength;


    if( f_pSubFieldData[0] == '\0' ) return 0;
    s_InsertSubFieldLength = s_TotalMarcLength = 0;
    // 1.Marc길이가 100000바이트가 넘는지를 검사
    ids = WriteMarcStruToStreamMarc( *f_pMarcStru , s_MarcData );
    if( ids < 0 ) return -1;

    if( f_PuncMarkUsageFlag == TRUE){
        // 추가하는 SubField의 길이 = SubFiedCode(2Byte) + SubFieldData
        s_InsertSubFieldLength = _tcsclen(f_pSubFieldData) + 2;
    }
    else{
        // 추가하는 SubField의 길이 = 구두점(1Byte) + SubFiedCode(2Byte) + SubFieldData
        s_InsertSubFieldLength = _tcsclen(f_pSubFieldData) + 3;
    }

    s_TotalMarcLength = s_InsertSubFieldLength + _tcsclen( s_MarcData );
    if( s_TotalMarcLength > 100000 ){
        AfxMessageBox(_T("MARC 의 길이가 100000 바이트를 초과 했습니다."));
        return 1000;
    }

    if( f_fIndicator == -1 && f_fIndicator == -1 ){
        switch( f_TagNo ){

        case  20 :
            s_fIndicator = ' ';
            s_sIndicator = ' ';
            break;

        case  24 :
        case  41 :
        case  49 :
        case  85 :
        case 100 :
        case 502 : 
        case 765 :
        case 767 :
        case 770 :
        case 772 :
        case 773 :
        case 775 :
        case 776 :
        case 787 :
        case 950 :

            s_fIndicator = '0';
            s_sIndicator = ' ';
            break;

        case 50 :
        case 52 :

            s_fIndicator = '0';
            s_sIndicator = '1';
            break;

        case 245 :
        case 440 :
        case 490 :
        case 500 :
        case 507 :
        case 511 :
        case 600 :
        case 630 :
        case 900 :
            s_fIndicator = '0';
            s_sIndicator = '0';
            break;
    
        case 130 :
        case 610 :
        case 611 :
        case 650 :
        case 651 :
        case 700 :
        case 730 :
        case 740 :
        case 830 :
        case 911 :
            s_fIndicator = ' ';
            s_sIndicator = '0';
            break;

        case 780 :
        case 785 :
            s_fIndicator = '0';
            s_sIndicator = '2';
            break;

        case 246 :
            s_fIndicator = ' ';
            s_sIndicator = '1';
            break;

        default : 
            s_fIndicator = ' ';
            s_sIndicator = ' ';
            break;
        }
    }
    else{
        s_fIndicator = f_fIndicator;
        s_sIndicator = f_sIndicator;
    }

    /// 1. 
    for ( i=0; i < f_pMarcStru->MarcFieldCount; i++ ) {
        if ( f_TagNo > f_pMarcStru->pMarcField[i].TagNo )  continue;
        else
        if ( f_TagNo == f_pMarcStru->pMarcField[i].TagNo ) {
           ids = GetTokenedElementFromElementString( 
                     f_pMarcStru->pMarcField[i].pFldData_ElementStr,
                    &s_TokenedElementCount ,
                    &s_pTokenedElements );
           if ( ids ) {return -3;}

           ///////////////////////////////////////////////////////////////////
           // 4 : 식별기호 : 2 , NULL : 1,  나머지 1 : 구두점 적용을 위해 ALLOC
           //     하지만 이것은 구두점을 적용하지 않으면 사용되지않는다.
           ///////////////////////////////////////////////////////////////////
           if ( s_TokenedElementCount == 0 ) {
               s_AllocLen = 4 + _tcsclen(f_pSubFieldData);
           } else {
               s_pFldData_ElementStr = f_pMarcStru->pMarcField[i].pFldData_ElementStr;
               s_AllocLen = _tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr) + 4 + _tcsclen(f_pSubFieldData);
           }
           f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR*)realloc(
                        f_pMarcStru->pMarcField[i].pFldData_ElementStr , s_AllocLen );
           if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) {
              f_pMarcStru->pMarcField[i].pFldData_ElementStr = s_pFldData_ElementStr;
              FreeTwoDim( s_TokenedElementCount, s_pTokenedElements );
              return -4;
           }
           
           s_pFldData_ElementStr = f_pMarcStru->pMarcField[i].pFldData_ElementStr;
           s_pFldData_ElementStr[0] = 0;
           for ( j=0; j < s_TokenedElementCount; j++ ) {
               if ( f_pSubFieldCode[1] < s_pTokenedElements[j][1] ) {
                   for ( k=0; k < j; k++ )
                        _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%s"),
                                                         &s_pTokenedElements[k][0] );
                   // 자료의 마지막이 구두점인 검사하여 분리, 구두점 플래그선정
                   s_PostFuncMark = s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)-1];                           
                   if ( s_PostFuncMark == '.' || s_PostFuncMark == ',' ||
                        s_PostFuncMark == '=' || s_PostFuncMark == ':' ||
                        s_PostFuncMark == ';' || s_PostFuncMark == '/' ||
                        s_PostFuncMark == '%' || s_PostFuncMark == '+' ) {
                       s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)-1] = 0;
                       s_PuncMarkExistFlag = TRUE;
                   } else
                       s_PuncMarkExistFlag = FALSE;

                   if ( f_PuncMarkUsageFlag == TRUE && _tcsclen(s_pFldData_ElementStr) != 0 ) {
                      _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%c%s%s"),
                                  f_PuncMark,f_pSubFieldCode, f_pSubFieldData );
                   } else {
                       _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%s%s"),
                                                     f_pSubFieldCode, f_pSubFieldData );
                   }
                   for ( k=j; k < s_TokenedElementCount; k++ )
                       if ( s_PuncMarkExistFlag == TRUE && k == j ) 
                          _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%c%s"),
                                                 s_PostFuncMark, &s_pTokenedElements[k][0] );
                       else
                          _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%s"),
                                                         &s_pTokenedElements[k][0] );

                   break;
               }
           }
           if ( j >= s_TokenedElementCount ) {
               for ( k=0; k < s_TokenedElementCount; k++ )
                    _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%s"),
                                                     &s_pTokenedElements[k][0] );
               if ( f_PuncMarkUsageFlag == TRUE  && _tcsclen(s_pFldData_ElementStr) != 0 ) {
                   _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%c%s%s"),
                                                 f_PuncMark,f_pSubFieldCode, f_pSubFieldData );
               } else {
                   _stprintf( &s_pFldData_ElementStr[_tcsclen(s_pFldData_ElementStr)], _T("%s%s"),
                                                 f_pSubFieldCode, f_pSubFieldData );
               }
           }
        } else
        if ( f_TagNo < f_pMarcStru->pMarcField[i].TagNo ) {
           _stprintf( s_FieldData, _T("%s%s"), f_pSubFieldCode, f_pSubFieldData );
           ids = InsertFieldStruToMarcStru( f_TagNo, s_fIndicator, s_sIndicator,
                                                s_FieldData, FT, i, f_pMarcStru );
           if ( ids ) {return ids;}
        }  // End If
        break;
    }
    if ( i >= f_pMarcStru->MarcFieldCount ) {
        _stprintf( s_FieldData, _T("%s%s"), f_pSubFieldCode, f_pSubFieldData );
        ids = InsertFieldStruToMarcStru( f_TagNo, s_fIndicator, s_sIndicator,
                                           s_FieldData, FT, i, f_pMarcStru );
        if ( ids ) {return ids;}
    }
        
    return i;     /// SubFieldData 삽입위치
}

INT CMarcConvert::tagno_integer_check( TCHAR * str )
{
    for( ; *str != '\0' && (*str >= '0' && *str <= '9') ; str++ )
		;
	if ( *str != '\0' )
		return -1 ;

    return 0; 
}

INT CMarcConvert::GetMarcDirectoryFromStreamMarc(  INT               f_DirStartPos, 
	        						    INT               f_DirEndPos,
                                        TCHAR            * f_pStreamMarc , 
    									tDIRECTORY_STRU * f_pDir )
{
    

    INT     s_DirLength;

    if ( f_DirEndPos >= f_DirStartPos ) 
	{
        /// . DirEndPos-1 :  필드종단기호가 포함되어 있으므로 -1 을 한다.
        s_DirLength = (f_DirEndPos-1) - f_DirStartPos + 1;
        if ( s_DirLength % DIRECTORY_ENTRY_SIZE ) 
			 return -1;

        f_pDir->EntryCount = s_DirLength / DIRECTORY_ENTRY_SIZE ;
        f_pDir->pEntries   = (tDIRECTORY_ENTRY_STRU *)malloc( sizeof(tDIRECTORY_ENTRY_STRU) * f_pDir->EntryCount );
        if ( f_pDir-> pEntries == NULL ) 
			return -2;

        memcpy( f_pDir->pEntries, &f_pStreamMarc[f_DirStartPos], sizeof(tDIRECTORY_ENTRY_STRU) * f_pDir->EntryCount );

		f_pDir->FieldTerm = f_pStreamMarc[f_DirEndPos];
    } else 
	{
        f_pDir->EntryCount = 0;
        f_pDir->pEntries = NULL;
    }

    return 0;

    
}

INT CMarcConvert::GetMarcLeaderFromStreamMarc( TCHAR         * f_pStreamMarc, 
	  							 tLEADER_STRU * f_pLeader )
{
    if ( f_pStreamMarc[0] == 0 ) return -1;

    memcpy( f_pLeader, f_pStreamMarc, LEARDER_SIZE );


    if( f_pLeader->IndicatorLength == ' ' ){
        f_pLeader->IndicatorLength = '2';
    }

    if( f_pLeader->SubFieldCodeLengh == ' ' ){
        f_pLeader->SubFieldCodeLengh = '2';
    }

    if( f_pLeader->MokRok_WriteType == ' ' ){
        f_pLeader->MokRok_WriteType = 'k';
    }

    if( f_pLeader->EntryMap[0] == ' ' ){
        _tcsnccpy(f_pLeader->EntryMap , _T("4500") , 4 );
    }

    return 0;    
}


INT CMarcConvert::MarcStruSort( tMARC_STRU  f_RawMarcInfo , tMARC_STRU  *f_pWorkMarcInfo )
{    
    INT  i,ids;

    for( i = 0; i < f_RawMarcInfo.MarcFieldCount; i++ ) {
        if( f_RawMarcInfo.pMarcField[i].TagNo < 10 || f_RawMarcInfo.pMarcField[i].TagNo == 908 ){
            _tcscpy( s_FieldData , f_RawMarcInfo.pMarcField[i].pFldData_ElementStr );
        }
        else{ 
            _stprintf( s_FieldData , _T("%c%c%s") , f_RawMarcInfo.pMarcField[i].fIndicator            ,
                                              f_RawMarcInfo.pMarcField[i].sIndicator            ,
                                              f_RawMarcInfo.pMarcField[i].pFldData_ElementStr );
        }

        ids = InsertFieldDataToMarcStru( f_RawMarcInfo.pMarcField[i].TagNo ,
                                         s_FieldData                       ,
                                         f_pWorkMarcInfo );
        if( ids < 0 ) return 0;
    }
       

    return 0;    
}

VOID CMarcConvert::DelSpecialChar( TCHAR *f_pRawStr , TCHAR *f_pWorkStr , BOOL f_Flag )
{
    

    INT           i,j,idx; 
    _TUCHAR ch1,ch2;

	TCHAR * s_tmpbeffer = (TCHAR*)malloc(sizeof(TCHAR)*(_tcsclen(f_pRawStr)+sizeof(TCHAR)));
    // KOL.UDF.022
	CString sComment = _T("");

    idx = 0;
    if( f_pRawStr == NULL ) 
	{
		f_pWorkStr = NULL  ;
		return;
	}

    _tcscpy( s_tmpbeffer , f_pRawStr );
	f_pWorkStr[0] = '\0';

	idx = 0;

    for( i = 0; i < (INT)_tcsclen(s_tmpbeffer); i++ ){
        // 1. 2byte문자인 경우는 2byte를 복사.
        if( (_TUCHAR)s_tmpbeffer[i] >= 0x80 ){
            
            if( i == (INT)_tcsclen(s_tmpbeffer) - 1 ){
                f_pWorkStr[idx] = '\0';
                break;
            }

            ch1 = (_TUCHAR)s_tmpbeffer[i];
            ch2 = (_TUCHAR)s_tmpbeffer[i+1];
            
			// KOL.UDF.022
            //for( j = 0; j <= SPECIAL_CHAR_COUNT ;j++) {
			for( j = 0; j < SPECIAL_CHAR_COUNT ;j++) {
				// KOL.UDF.022
				sComment.Format(_T("%s"),s_DelCharInfo[j].Comment);
				
				// KOL.UDF.022
                //if( _tcscmp( s_DelCharInfo[j].Comment , _T("그림문자"))  == 0 && f_Flag == TRUE ){ 
				if( sComment.Compare(_T("그림문자"))  == 0 && f_Flag == TRUE ){ 
                    continue;
                }

                if( ch1 >= s_DelCharInfo[j].s_FirstChar && ch2 >= s_DelCharInfo[j].s_SecondChar &&
                    ch1 <= s_DelCharInfo[j].e_FirstChar && ch2 <= s_DelCharInfo[j].e_SecondChar ){

                    //if( _tcscmp( s_DelCharInfo[j].Comment , _T("숫자"))  == 0 ){
					if( sComment.Compare(_T("숫자"))  == 0 ){
                        f_pWorkStr[idx++] = ch2 - 0x80;
                    }else
                    if( sComment.Compare(_T("영문대문자"))  == 0 ){
                        f_pWorkStr[idx++] = ch2 - 0x80;
                    }else
                    if( sComment.Compare(_T("영문소문자"))  == 0 ){
                        f_pWorkStr[idx++] = ch2 - 0xA0;
                    }else
                    if( sComment.Compare(_T("한글확장"))  == 0 ){
                        if( ch2 <= 0x7F ){
                            if( ( ch2 >= 0x41 && ch2 <= 0x5A ) || ( ch2 >= 0x61 && ch2 <= 0x7A ) ){
                                f_pWorkStr[idx++] = ch1;
                                f_pWorkStr[idx++] = ch2;
                            }
                        }
                        else{
                            if( ( ch1 <= 0xA0 ) && ( ch2 >= 0x81 && ch2 <= 0xFE ) ){
                                f_pWorkStr[idx++] = ch1;
                                f_pWorkStr[idx++] = ch2;
                            }else
                            if( ( ch1 >= 0xA1 ) && ( ch2 >= 0x81 && ch2 <= 0xA0 ) ){
                                f_pWorkStr[idx++] = ch1;
                                f_pWorkStr[idx++] = ch2;
                            }
                        }
                    }
                    else{
                        f_pWorkStr[idx++] = ch1;
                        f_pWorkStr[idx++] = ch2;
                    }
					break ;
                }
            }
            i++;
       }
       else{
            f_pWorkStr[idx++] = s_tmpbeffer[i];
       }
    }
	f_pWorkStr[idx] = '\0';

	free( s_tmpbeffer );

    
}

INT CMarcConvert::InsertFieldStruToMarcStru( 
                                  INT   f_TagNo,
                                  TCHAR  f_fIndicator,
                                  TCHAR  f_sIndicator,
                                  TCHAR  *f_pFldData_ElementStr,
                                  TCHAR  f_FieldTerm  , 
                                  INT   InsertPos, 
                                  tMARC_STRU  *f_pMarcStru )
{
    
    tFIELD_STRU       *s_pFieldStru;    
    INT                s_InsertFieldLength,s_TotalMarcLength;

    
    s_InsertFieldLength = s_TotalMarcLength = 0;
    // 1.Marc길이가 100000바이트가 넘는지를 검사
    //ids = WriteMarcStruToStreamMarc( *f_pMarcStru , s_MarcData );
    //if( ids < 0 ) return -1;

    if( f_pFldData_ElementStr[0] == '\0' ) return -1;
    if( f_fIndicator == '\0' || f_pFldData_ElementStr[0] == '\0' ) {
        //AfxMessageBox(_T("DATA가 없습니다."));
        return 0;
    }
    // 2.TagNo가 10 이하인 경우 
    if( f_TagNo < 10 || f_TagNo == 908 ){
        // 추가하는 문자열의 길이 = TagNo(3Byte) + 필드길이 + 필드종단기호(1Byte)
        s_InsertFieldLength = _tcsclen(f_pFldData_ElementStr) + 4;
    }
    // 3.TagNo가 10 이상인 경우 
    else{
        // 추가하는 문자열의 길이 = TagNo(3Byte) + 지시기호길이(2Byte) + 필드길이 + 필드종단기호(1Byte)
        s_InsertFieldLength = _tcsclen(f_pFldData_ElementStr) + 5;
    }

    s_TotalMarcLength = s_InsertFieldLength + _tcsclen( s_MarcData );
    if( s_TotalMarcLength > 100000 ){
        AfxMessageBox(_T("MARC 의 길이가 100000 바이트를 초과 했습니다."));
        return 1000;
    }

    s_pFieldStru = f_pMarcStru->pMarcField;
    f_pMarcStru->pMarcField = (tFIELD_STRU *)realloc( f_pMarcStru->pMarcField,
                                                      sizeof(tFIELD_STRU) * (f_pMarcStru->MarcFieldCount+sizeof(TCHAR)) );
    if ( f_pMarcStru->pMarcField == NULL ) {
       f_pMarcStru->pMarcField = s_pFieldStru;
       return -1;
    }
    memmove( &f_pMarcStru->pMarcField[InsertPos+1], 
             &f_pMarcStru->pMarcField[InsertPos], 
             sizeof(tFIELD_STRU) * (f_pMarcStru->MarcFieldCount-InsertPos) );
    f_pMarcStru->MarcFieldCount++;

    f_pMarcStru->pMarcField[InsertPos].pFldData_ElementStr = (TCHAR*)malloc(_tcsclen(f_pFldData_ElementStr) + (sizeof(TCHAR)*3));
    if ( f_pMarcStru->pMarcField[InsertPos].pFldData_ElementStr == NULL ) { return -2; }

    f_pMarcStru->pMarcField[InsertPos].TagNo = f_TagNo;
    f_pMarcStru->pMarcField[InsertPos].fIndicator = f_fIndicator;
    f_pMarcStru->pMarcField[InsertPos].sIndicator = f_sIndicator;
    _stprintf( f_pMarcStru->pMarcField[InsertPos].pFldData_ElementStr, _T("%s"), f_pFldData_ElementStr );
    f_pMarcStru->pMarcField[InsertPos].FieldTerm = f_FieldTerm;
    
    return 0;

    
}
