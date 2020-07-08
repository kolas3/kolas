#include    <StdAfx.h>
#include    <windows.h>
#include    <stdio.h>
#include    <malloc.h>
#include    <math.h>
#include    "MarcRawApi.h"

static TCHAR s_MarcData[50000];
static TCHAR s_FieldData[30000];

#define     EX_DEL_CHAR_LIST_CNT 39

typedef struct _EX_DEL_CHAR_LISR {
    _TCHAR          Comment[64];
    _TCHAR          StartChar;
    _TCHAR          LastChar;
} tEX_DEL_CHAR_LISR;

tEX_DEL_CHAR_LISR EX_DEL_CHAR_LISR[EX_DEL_CHAR_LIST_CNT] = {
	{ _T("ETC")                            , 0x0000 , 0x001F }, // 
    { _T("LATIN")                          , 0x0020 , 0x002F }, //	 	!	"	#	$	%	&	'	(	)	*	+	,	-	.	/
    { _T("LATIN")                          , 0x003A , 0x0040 }, // :	;	<	=	>	?  @
    { _T("LATIN")                          , 0x005B , 0x0060 }, // [	\	]	^	_  `
    { _T("LATIN")                          , 0x007B , 0x00BF }, // {	|	}	~   ¡	￠	￡	¤	￥	|	§	¨	ⓒ	ª	≪	￢	­	®	? °	±	²	³	´	μ	¶	·	¸	¹	º	≫	¼	½	¾	¿
    { _T("LATIN")                          , 0x0220 , 0x0221 }, // 
    { _T("SPACING MODIFIER LETTERS")       , 0x02B9 , 0x02BF }, // 
    { _T("SPACING MODIFIER LETTERS")       , 0x02C2 , 0x02DF }, // 	?	ˇ	ː˘	˙	˚	˛	˝
    { _T("SPACING MODIFIER LETTERS")       , 0x02E5 , 0x02EE }, // 
    { _T("COMBINING DIACRITICAL MARKS")    , 0x0300 , 0x036F }, // 
    { _T("GETERAL PUNCTUATION")            , 0x2000 , 0x206F }, //	―	‘	’	“	”	†	‡	‥	…		‰	′	″	※	
    { _T("SUPERSCRIPTS AND SUBSCRIPTS")    , 0x2070 , 0x209F }, // 	⁴ⁿ₁	₂	₃	₄
    { _T("CURRENCY")                       , 0x20A0 , 0x20CF }, // \ €
    { _T("DIACRITICS SYMBOLS")             , 0x20D0 , 0x20FF }, // 
    { _T("ARROW")                          , 0x2190 , 0x21FF }, // ←	↑	→	↓	↔	↕	↖	↗	↘	↙	⇒	⇔
    { _T("MATHMETICAL SYMBOLS")            , 0x2200 , 0x22FF }, // ∀	∂	∃	∇	∈	∋	∏	∑	√	∝	∞	∠	∥	∧	∨	∩	∪	∫	∬	∮	∴	∵	∼	∽	≒	≠	≡	≤	≥	≪	≫ ⊂	⊃	⊆	⊇	⊙	⊥	
    { _T("MISCELLANEOUS SYMBOLS")          , 0x2300 , 0x23FF }, // ⌒	
    { _T("CONTROL PICTURES")               , 0x2400 , 0x243F }, // 
    { _T("OCR")                            , 0x2440 , 0x245F }, // 
    { _T("ENCLOSED ALPHANUMERICS")         , 0x2460 , 0x24FF }, // 
    { _T("BOX DRAWING")                    , 0x2500 , 0x257F }, // 
    { _T("BLOCKS")                         , 0x2580 , 0x259F }, // 
    { _T("GEOMETRIC SHAPES")               , 0x25A0 , 0x25FF },
    { _T("MISCELLANEOUS SYMBOLS")          , 0x2600 , 0x26FF },
    { _T("DINGBATS")                       , 0x2700 , 0x27CF },
    { _T("BRAILLE PATTERNS")               , 0x2800 , 0x28FF },
    { _T("CJK SYMBOL & PUNCTUATION")       , 0x3000 , 0x303F },
    { _T("ENCLOSED CJK LETTERS & MONTHS")  , 0x3200 , 0x32FF },
    { _T("YI SYLLABLES")                   , 0xA000 , 0xA48F },
    { _T("YI RADICALS")                    , 0xA490 , 0xA4CF },
    { _T("ETC")                            , 0xF000 , 0xF0FF },
    { _T("COMBING HALF FORMS")             , 0xFE20 , 0xFE2F },
    { _T("CJK COMPATIBILTY FORMS")         , 0xFE30 , 0xFE4F },
    { _T("SMALL FORM VARIANTS")            , 0xFE50 , 0xFE6F },
    { _T("HALFWIDTH & FULLWIDTH FORMS")    , 0xFF00 , 0xFF0F },
    { _T("HALFWIDTH & FULLWIDTH FORMS")    , 0xFF1A , 0xFF20 },
    { _T("HALFWIDTH & FULLWIDTH FORMS")    , 0xFF3B , 0xFF40 },
    { _T("HALFWIDTH & FULLWIDTH FORMS")    , 0xFF5B , 0xFF65 },
    { _T("HALFWIDTH & FULLWIDTH FORMS")    , 0xFFE0 , 0xFFFF }
};

INT ReadMarcStruFromStreamMarc( TCHAR       * f_pStreamMarc, 
							    tMARC_STRU * f_pMarcStru )
{
    INT             s_FieldBasePos, ids, i;
    INT             ReadPos, s_FieldDataLength;
	INT             Control_DataField_Flag ;
    tDIRECTORY_STRU s_MarcDir;

	TCHAR*ctmp  = (TCHAR*)malloc( sizeof(TCHAR)*(1000+1) );

    ReadPos = s_FieldDataLength = s_FieldBasePos = 0;
    f_pMarcStru->RecordTerm = RT;

	// 1. 리더 초기화
	// 씨바 MEMSET 안된다.그래서 졸라 무식하게 했다
	// null 이외는 안된다.
	//    memset( &f_pMarcStru->MarcLeader, _T(' '), sizeof(tLEADER_STRU) );
	
	_stprintf( f_pMarcStru->MarcLeader.RecLength,     _T("     "));
	f_pMarcStru->MarcLeader.RecStatus               = _T(' ');
	f_pMarcStru->MarcLeader.RecType                 = _T(' ');
	f_pMarcStru->MarcLeader.SeoJiLevel              = _T(' ');
	f_pMarcStru->MarcLeader.Control_type            = _T(' ');
	f_pMarcStru->MarcLeader.CharType                = _T(' ');
	f_pMarcStru->MarcLeader.IndicatorLength         = _T(' ');
	f_pMarcStru->MarcLeader.SubFieldCodeLengh       = _T(' ');
	_stprintf(f_pMarcStru->MarcLeader.FieldBasePos,   _T("     "));
	f_pMarcStru->MarcLeader.InputLevel              = _T(' ');
	f_pMarcStru->MarcLeader.MokRok_WriteType        = _T(' ');
	f_pMarcStru->MarcLeader.RelationRecordCondition = _T(' ');
	_stprintf(f_pMarcStru->MarcLeader.EntryMap,       _T("    "));

    if ( f_pStreamMarc[0] == _T('\0') ) {

        /// 1. MarcStru 초기화
		memset( f_pMarcStru, _T('\0'), sizeof(tMARC_STRU) );
		f_pMarcStru->RecordTerm = RT;
        _stprintf( f_pMarcStru->MarcLeader.RecLength,     _T("     "));
		f_pMarcStru->MarcLeader.RecStatus               = _T(' ');
		f_pMarcStru->MarcLeader.RecType                 = _T(' ');
		f_pMarcStru->MarcLeader.SeoJiLevel              = _T(' ');
		f_pMarcStru->MarcLeader.Control_type            = _T(' ');
		f_pMarcStru->MarcLeader.CharType                = _T(' ');
		f_pMarcStru->MarcLeader.IndicatorLength         = _T(' ');
		f_pMarcStru->MarcLeader.SubFieldCodeLengh       = _T(' ');
		_stprintf(f_pMarcStru->MarcLeader.FieldBasePos,   _T("     "));
		f_pMarcStru->MarcLeader.InputLevel              = _T(' ');
		f_pMarcStru->MarcLeader.MokRok_WriteType        = _T(' ');
		f_pMarcStru->MarcLeader.RelationRecordCondition = _T(' ');
		_stprintf(f_pMarcStru->MarcLeader.EntryMap,       _T("    "));

		free( ctmp );

		return 0;
    }

    /// 1. Marc Leader Read
    ids = GetMarcLeaderFromStreamMarc( f_pStreamMarc, &f_pMarcStru->MarcLeader );
    if ( ids ) {
		free( ctmp );
		return ids;
	}

    /// 2. 디렉토리의 길이를 구하기위해 필드가 시작되는 위치를 구한다.
    memcpy( ctmp, &f_pMarcStru->MarcLeader.FieldBasePos[0], 5*sizeof(TCHAR) ); ctmp[5]= 0;
    s_FieldBasePos = _ttoi( ctmp );   //  BasePose Of DataField

    /// 3. 디렉토리 데이터를 구한다. ( MarcDir.pEntries Mem_Allocate )
    ids = GetMarcDirectoryFromStreamMarc( LEARDER_SIZE , s_FieldBasePos-1, f_pStreamMarc, &s_MarcDir );
    if ( ids ) {
		free( ctmp );
		return ids;
	}

	/// 4  필드 갯수와 필드를 구한다.

	f_pMarcStru->MarcFieldCount = s_MarcDir.EntryCount ;

	f_pMarcStru->pMarcField = (tFIELD_STRU *)malloc(
						     sizeof(tFIELD_STRU) * f_pMarcStru->MarcFieldCount );
	if ( f_pMarcStru->pMarcField == NULL ) {
		free( ctmp );
		free( s_MarcDir.pEntries );
	    return -222;
	}

	for ( i=0 ; i < f_pMarcStru->MarcFieldCount ; i++ )
		f_pMarcStru->pMarcField[i].pFldData_ElementStr = NULL ;

	//for( i = 0; i < _tcslen(f_pStreamMarc); i++ ) {
		//if ( f_pStreamMarc[i] == FT ) { ReadPos = i+1; break; }
	//}

	TCHAR *pdest;
	TCHAR  pStrData[64];

	_stprintf( pStrData, _T("%c"), FT );
	pdest = _tcsstr( &f_pStreamMarc[ReadPos], pStrData );
	if ( pdest != NULL ) {
		ReadPos = pdest - &f_pStreamMarc[ReadPos] + 1;
	}

	// 4.1 할당된 메모리에 필드자료를 저장한다.
	ids = 0;
    for ( i=0 ; i < f_pMarcStru->MarcFieldCount; i++ ) 
	{
	    /// 4.2.1 각 필드의 길이를 구한	후 (지시기호/)종단기호를 제외한 나머지 길이구함
		s_FieldDataLength = 0;
		//for( int k = ReadPos; k < _tcslen(f_pStreamMarc); k++ ) {
			//s_FieldDataLength++;
			//if ( f_pStreamMarc[k] == FT ) { break; }
		//}

		pdest = _tcsstr( &f_pStreamMarc[ReadPos], pStrData );
		if ( pdest != NULL ) {
			s_FieldDataLength = pdest - &f_pStreamMarc[ReadPos] + 1;
		}

	    if ( s_FieldDataLength <= 0 )  // By HwangKumHee 99.09.16
		{
			ids = -200;
			return ids;
		}

	    /// 4.2.3 TagNo을 저장한다
	    memcpy( ctmp, s_MarcDir.pEntries[i].TagNo, 3*sizeof(TCHAR) ); ctmp[3] = 0;

		if ( tagno_integer_check ( ctmp ) )  
		{
			ids = -201 ;  
			return ids;
		}

        f_pMarcStru->pMarcField[i].TagNo = _ttoi( ctmp );

		Control_DataField_Flag = 1;
		// KOL.UDF.022 시큐어코딩 보완
		CString sTagNo = _T("");
		sTagNo.Format(_T("%s"), s_MarcDir.pEntries[i].TagNo[0]);
		if( f_pMarcStru->pMarcField[i].TagNo < 10 || sTagNo.Compare(_T("908")) == 0 ) Control_DataField_Flag = 0;			
		/*
        if( f_pMarcStru->pMarcField[i].TagNo < 10 || sTagNo == _T("908")
			memcmp( &s_MarcDir.pEntries[1].TagNo[0], _T("908"), sizeof(TCHAR)*3 ) == 0 ) Control_DataField_Flag = 0;*/

		switch ( Control_DataField_Flag )
		{
   			case  0 :  // Control Field

			   if ( s_FieldDataLength <= 1 )
			   {
					ids = -115;
					return ids;
			   }

			   /// 4.2.3 요소데이터 메모리 확보(NULL 포함)
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR *)malloc( sizeof(TCHAR)*(s_FieldDataLength+1) );
			   if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) {
				  ids = -112;
				  return ids;
			   }

			   /// 4.2.4 데이터 Copy (필드종단기호 제외)
			   memcpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[0],
					   &f_pStreamMarc[ReadPos], (s_FieldDataLength-1)*sizeof(TCHAR) );
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr[s_FieldDataLength-1] = 0;

			   /// 4.2.5 필드종단기호 Assign
			   ReadPos += s_FieldDataLength;
              
			   break;

			default :  // Data    Field

			   /*
			   if ( s_FieldDataLength <= 3 )
			   {
					ids = -116;
					goto EBOT ;
			   }
			   */

   		       /// 4.2.3 제1/2 지시기호를 구한다.
			   f_pMarcStru->pMarcField[i].fIndicator = f_pStreamMarc[ReadPos++];
			   f_pMarcStru->pMarcField[i].sIndicator = f_pStreamMarc[ReadPos++];

			   /// 4.2.4 요소데이터 메모리 확보(NULL 포함)
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr = (TCHAR *)malloc( sizeof(TCHAR)*(s_FieldDataLength+1) );
			   if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr == NULL ) {
				  ids = -113;
				  return ids;
			   }

			   /// 4.2.5 데이터 Copy (지시기호/필드종단기호 제외)
			   memcpy( &f_pMarcStru->pMarcField[i].pFldData_ElementStr[0],
					   &f_pStreamMarc[ReadPos], (s_FieldDataLength-3)*sizeof(TCHAR) );
			   f_pMarcStru->pMarcField[i].pFldData_ElementStr[s_FieldDataLength-3] = 0;

			   if ( _tcsclen(f_pMarcStru->pMarcField[i].pFldData_ElementStr) != (UINT)(s_FieldDataLength-3) )
			   {
				   ids = -202;
				   return ids;
			   }

			   /*
			   if ( f_pMarcStru->pMarcField[i].pFldData_ElementStr[0] != (TCHAR)GT )
			   {
				   ids = -204;
				   goto EBOT;
			   }
			   */

			   /// 4.2.6 필드종단기호 Assign
			   ReadPos +=  (s_FieldDataLength-2);
		}

		//  필드종단기호
	    f_pMarcStru->pMarcField[i].FieldTerm = f_pStreamMarc[ReadPos-1];		
	}

	// 5. RecordTerm Assign
	if ( ReadPos == 0 ) {
		ReadPos = s_FieldBasePos;
	}
	if ( f_pStreamMarc[ ReadPos ] != (TCHAR)RT ){
		ids = -114;
		return ids;
	}
	f_pMarcStru->RecordTerm = f_pStreamMarc[ ReadPos ];

	free( ctmp );
	free ( s_MarcDir.pEntries ); 
    return 0;
}

INT tagno_integer_check( TCHAR * str )
{
    for( ; *str != '\0' && (*str >= '0' && *str <= '9') ; str++ )
		;
	if ( *str != '\0' )
		return -1 ;

    return 0; 
}

INT GetMarcDirectoryFromStreamMarc(  INT               f_DirStartPos, 
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

INT GetMarcLeaderFromStreamMarc( TCHAR         * f_pStreamMarc, 
	  							 tLEADER_STRU * f_pLeader )
{

    if ( f_pStreamMarc[0] == 0 ) return -1;

    memcpy( f_pLeader, f_pStreamMarc, LEARDER_SIZE*sizeof(TCHAR) );


    if( f_pLeader->IndicatorLength == ' ' ){
        f_pLeader->IndicatorLength = '2';
    }

    if( f_pLeader->SubFieldCodeLengh == ' ' ){
        f_pLeader->SubFieldCodeLengh = '2';
    }

    if( f_pLeader->MokRok_WriteType == ' ' ){
        f_pLeader->MokRok_WriteType = 'c';
    }

    if( f_pLeader->EntryMap[0] == ' ' ){
        _tcsnccpy(f_pLeader->EntryMap , _T("4500") , 4 );
    }

    return 0;
}

INT WriteMarcStruToStreamMarc( tMARC_STRU  f_MarcStru,
							   _TCHAR       *f_pStreamMarc,
							   INT          f_inCharSet)
{
    tMARC_STRU  s_MarcInfo;
	CHAR	*s_pFieldData;
	INT s_Length;
    INT     s_WritePos, s_FieldBasePos, s_FieldStartPos, s_FieldDataLength;
    INT     i,ids ;

	INT  inCharSet;

	inCharSet = f_inCharSet;

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
    //memset( f_pStreamMarc, ' ', MAX_MARC_SIZE );

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
        f_MarcStru.MarcLeader.MokRok_WriteType = 'c';
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

	if( f_MarcStru.MarcLeader.Control_type == ' ' ){
		f_MarcStru.MarcLeader.Control_type = ' ';
	}

	if( f_MarcStru.MarcLeader.CharType == ' ' ){
		f_MarcStru.MarcLeader.CharType = 'a';
	}

	_tcsnccpy( &f_pStreamMarc[0] , f_MarcStru.MarcLeader.RecLength, 5 );
	f_pStreamMarc[5] = f_MarcStru.MarcLeader.RecStatus;
	f_pStreamMarc[6] = f_MarcStru.MarcLeader.RecType;
	f_pStreamMarc[7] = f_MarcStru.MarcLeader.SeoJiLevel;
	f_pStreamMarc[8] = f_MarcStru.MarcLeader.Control_type;
	f_pStreamMarc[9] = f_MarcStru.MarcLeader.CharType;
	 f_pStreamMarc[10] = f_MarcStru.MarcLeader.IndicatorLength;
	f_pStreamMarc[11] = f_MarcStru.MarcLeader.SubFieldCodeLengh;
	_tcsnccpy( &f_pStreamMarc[12] , f_MarcStru.MarcLeader.FieldBasePos, 5 );
	f_pStreamMarc[17] = f_MarcStru.MarcLeader.InputLevel;
	f_pStreamMarc[18] = f_MarcStru.MarcLeader.MokRok_WriteType;
	f_pStreamMarc[19] = f_MarcStru.MarcLeader.RelationRecordCondition;
	_tcsnccpy( &f_pStreamMarc[20] , f_MarcStru.MarcLeader.EntryMap, 4 );
	f_MarcStru.RecordTerm = RT;

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

		if ( inCharSet == CP_UTF8 ) {
			ids = UnicodeToUTF8( s_MarcInfo.pMarcField[i].pFldData_ElementStr, &s_pFieldData, &s_Length );
		} else if ( inCharSet == CP_UTF16 ) {
			s_pFieldData = NULL;
			s_Length     = _tcslen(s_MarcInfo.pMarcField[i].pFldData_ElementStr)*2;
		} else {
			ids = UnicodeToMultiByte( s_MarcInfo.pMarcField[i].pFldData_ElementStr , &s_pFieldData, &s_Length ,CP_ACP,0,NULL,NULL );
		}
		if( ids ){            
			return -3;
		}

		if ( s_pFieldData != NULL ) free( s_pFieldData ); 
        
		s_FieldDataLength = s_Length;

		if ( inCharSet == CP_UTF16 ) {
			if ( f_MarcStru.pMarcField[i].TagNo < 10 || s_MarcInfo.pMarcField[i].TagNo == 908 ) // Control Field
			   s_FieldDataLength += 2;   // FT
			else
			   s_FieldDataLength += 6;   // JE1/2JISIGIHO, FT
		} else {
			if ( f_MarcStru.pMarcField[i].TagNo < 10 || s_MarcInfo.pMarcField[i].TagNo == 908 ) // Control Field
			   s_FieldDataLength += 1;   // FT
			else
			   s_FieldDataLength += 3;   // JE1/2JISIGIHO, FT
		}

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

	if ( inCharSet == CP_UTF8 ) {
		ids = UnicodeToUTF8( f_pStreamMarc, &s_pFieldData, &s_Length );
	} else if ( inCharSet == CP_UTF16 ) {
		s_pFieldData = NULL;
		s_Length     = _tcslen(f_pStreamMarc)*2;
	} else {
		ids = UnicodeToMultiByte( f_pStreamMarc, &s_pFieldData, &s_Length ,CP_ACP,0,NULL,NULL );
	}
	if( ids ){            
		return -3;
	}

	if ( s_pFieldData != NULL ) free( s_pFieldData ); 

    /// 5. Leader : RecordLength Write
    _stprintf(s_MarcData, _T("%05d"), s_Length );
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
        f_pStreamMarc[18] = 'c';
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

	if( f_MarcStru.MarcLeader.Control_type == ' ' ){
		f_pStreamMarc[8] = ' ';
	}

	if( f_MarcStru.MarcLeader.CharType == ' ' ){
		f_pStreamMarc[9] = 'a';
	}

    return 0;
}

INT	UnicodeToMultiByte(_TCHAR *fi_pSourceData        , 
                        CHAR **fo_pDestinationData   ,
                        INT   *fo_nByteDes           ,
                        INT    f_DestinationCodePage ,
                        DWORD  f_CFlags              ,
                        CHAR  *f_pDefaultChar        ,
                        BOOL   f_UsedDefaultChar     )
{
	CHAR	*pszDestinationData;
	INT		nWCharSource;
	INT		nBytesNeeded;
	
	nWCharSource   = _tcslen(fi_pSourceData);
	
	/* Query the number of bytes required to store the Dest string */
	nBytesNeeded = WideCharToMultiByte( f_DestinationCodePage 	, 
										f_CFlags				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										f_pDefaultChar			, 
										&f_UsedDefaultChar      );
	
	/* Allocate the required amount of space */
	if (nBytesNeeded == 0) 
    { return -1 ; }
		
	/* We need more 1 byte for '\0' */
	pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
		
	/* Do the conversion */
	nBytesNeeded = WideCharToMultiByte     (f_DestinationCodePage		, 
											f_CFlags					,
											(LPWSTR)fi_pSourceData		, 
											nWCharSource				,
											(CHAR*)pszDestinationData	, 
											nBytesNeeded				, 
											f_pDefaultChar				, 
											&f_UsedDefaultChar );		
	if (nBytesNeeded == 0) 
    { return -2 ; }

	*(LPSTR)((LPSTR)pszDestinationData + nBytesNeeded) = '\0';

	*fo_pDestinationData = pszDestinationData;
	*fo_nByteDes         = nBytesNeeded;


	return 0;
}


CHAR*	UnicodeToUTF8(_TCHAR uc, CHAR* UTF8)
{
	if (uc <= 0x7f)
	{		
		UTF8[0] = (CHAR) uc;
		UTF8[1] = (CHAR) '\0';
	}
	else if (uc <= 0x7ff)
	{
		UTF8[0] = (CHAR) 0xc0 + uc / (_TCHAR) pow(2, 6);
		UTF8[1] = (CHAR) 0x80 + uc % (_TCHAR) pow(2, 6);
		UTF8[2] = (CHAR) '\0';
	}
	else if (uc <= 0xffff)
	{
		UTF8[0] = (CHAR) 0xe0 + uc / (_TCHAR) pow(2, 12);
		UTF8[1] = (CHAR) 0x80 + uc / (_TCHAR) pow(2, 6) % (_TCHAR) pow(2, 6);
		UTF8[2] = (CHAR) 0x80 + uc % (_TCHAR) pow(2, 6);
		UTF8[3] = (CHAR) '\0';
	}

	return UTF8;
}

INT	UnicodeToUTF8 (_TCHAR* fi_UString, CHAR** fo_UTF8String, INT* fo_nBytes )
{
	INT		nLength;

	nLength = WideStrToUTF8Str(fi_UString, fo_UTF8String);
	fo_UTF8String[0][nLength] = '\0';
	*fo_nBytes = nLength;

	return 0;
}

INT   MarcStruSort( tMARC_STRU  f_RawMarcInfo , tMARC_STRU  *f_pWorkMarcInfo )
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

VOID FreeMarcStru( tMARC_STRU f_MarcStru )
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


INT WideStrToUTF8Str(_TCHAR *szUni, CHAR **szUTF8)
{
	INT		i;
	INT		nULen;
	CHAR		TempUTF8[10];

	
	nULen     = _tcslen(szUni)*3;
    (*szUTF8) = (CHAR*)malloc((nULen+1)*sizeof(CHAR));
	memset((*szUTF8), 0x00, nULen);

	for(i=0;szUni[i];i++) 
    {
		TempUTF8[0] = '\0';
		UnicodeToUTF8(szUni[i], TempUTF8);
		strcat((*szUTF8), TempUTF8);
	}
	return strlen((*szUTF8));
}

INT     InsertFieldDataToMarcStru(
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

INT     InsertFieldStruToMarcStru( 
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
                                                      sizeof(tFIELD_STRU) * (f_pMarcStru->MarcFieldCount+1) );
    if ( f_pMarcStru->pMarcField == NULL ) {
       f_pMarcStru->pMarcField = s_pFieldStru;
       return -1;
    }
    memmove( &f_pMarcStru->pMarcField[InsertPos+1], 
             &f_pMarcStru->pMarcField[InsertPos], 
             sizeof(tFIELD_STRU) * (f_pMarcStru->MarcFieldCount-InsertPos) );
    f_pMarcStru->MarcFieldCount++;

    f_pMarcStru->pMarcField[InsertPos].pFldData_ElementStr = (TCHAR*)malloc( sizeof(TCHAR)*(3+_tcsclen(f_pFldData_ElementStr)) );
    if ( f_pMarcStru->pMarcField[InsertPos].pFldData_ElementStr == NULL ) { return -2; }

    f_pMarcStru->pMarcField[InsertPos].TagNo = f_TagNo;
    f_pMarcStru->pMarcField[InsertPos].fIndicator = f_fIndicator;
    f_pMarcStru->pMarcField[InsertPos].sIndicator = f_sIndicator;
    _stprintf( f_pMarcStru->pMarcField[InsertPos].pFldData_ElementStr, _T("%s"), f_pFldData_ElementStr );
    f_pMarcStru->pMarcField[InsertPos].FieldTerm = f_FieldTerm;
    
    return 0;

    
}

INT     InsertSubFieldDataToMarcStru(
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

    //if( f_fIndicator == -1 && f_fIndicator == -1 ){
	if( f_fIndicator == _T(' ') && f_fIndicator == _T(' ') ){

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
                        f_pMarcStru->pMarcField[i].pFldData_ElementStr , sizeof(TCHAR)*s_AllocLen );
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

VOID FreeTwoDim( INT    f_Cnt, TCHAR **f_pData )
{
    

    INT     i;

    if ( f_pData != NULL ) {
       for ( i=0; i < f_Cnt; i++ )
           if ( f_pData[i] != NULL ) free( f_pData[i] );

       free( f_pData );
    }
    return;

    
}

INT  GetTokenedElementFromElementString( TCHAR      *f_pElementString     ,
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
                                           sizeof( TCHAR *) * (*f_pElementTokenCount +1) );
        if ( s_pTmpElementTokenData == NULL ) return -1;

        s_pTmpElementTokenData[*f_pElementTokenCount] = (TCHAR *)malloc( sizeof(TCHAR)*(_tcsclen(stoken) + 2) ); // 2 : $ + NULL
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

void DelSpecialChar( TCHAR *f_pRawStr , TCHAR *f_pWorkStr , BOOL f_Flag )
{
    

    INT           i,j,idx;
	_TCHAR       *s_tmpbeffer;
    _TCHAR        s_tmp[32];

    s_tmp[0] = '\0';

    //_stprintf ( s_tmp , _T("%c%c%c"), RT , FT , GT );
    //_tcstok( f_pRawStr , s_tmp );


    //s_tmpbeffer = new _TCHAR[5000];
	s_tmpbeffer = (TCHAR*)malloc( (sizeof( TCHAR ) * _tcslen( f_pRawStr )) + 2 ) ;

    idx = 0;
    if( &f_pRawStr == NULL )
	{
		f_pWorkStr = NULL  ;
		free( s_tmpbeffer );
		return;
	}

    for( i = 0; f_pRawStr[i] != _T('\0'); i++ ){
        
		for( j =0; j < EX_DEL_CHAR_LIST_CNT; j++){
            if( ( f_pRawStr[i] >= EX_DEL_CHAR_LISR[j].StartChar && f_pRawStr[i] <= EX_DEL_CHAR_LISR[j].LastChar ) ||
                ( f_Flag == TRUE && f_pRawStr[i] == _T(' ') ) ){

                if( ( f_Flag == FALSE && f_pRawStr[i] == _T('.') ) || ( f_Flag == FALSE && f_pRawStr[i] == _T(' ') ) || ( f_Flag == FALSE && f_pRawStr[i] == _T('"') ) ) continue;
                else break;
            }

			if ( f_pRawStr[i] == RT ) {
				break;
			}

			if ( f_pRawStr[i] == FT ) {
				break;
			}

			if ( f_pRawStr[i] == GT ) {
				break;
			}
        }

        if( j >= EX_DEL_CHAR_LIST_CNT ){
            s_tmpbeffer[idx] = f_pRawStr[i];
            idx++;
        }
    }

    s_tmpbeffer[idx] = _T('\0');
    _tcscpy( f_pWorkStr , s_tmpbeffer );


	free( s_tmpbeffer );

    
}
