// Index.cpp: implementation of the CIndex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Index.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define RT          29
#define FT          30
#define GT          31

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
    { _T("라틴어")     , 0xA9 , 0xA1 , 0xA9 , 0xAF },
    { _T("러시아어")   , 0xAC , 0xA1 , 0xAC , 0xC1 },
    { _T("러시아어")   , 0xAC , 0xD1 , 0xAC , 0xF1 },
    { _T("한글자모")   , 0xA4 , 0xA1 , 0xA4 , 0xD3 },
    { _T("한글고어1")  , 0xA4 , 0xD5 , 0xA4 , 0xFC },
    { _T("한글고어2")  , 0xA4 , 0xFE , 0xA4 , 0xFE },
    { _T("숫자")       , 0xA3 , 0xB0 , 0xA3 , 0xB9 },
    { _T("영문대문자") , 0xA3 , 0xC1 , 0xA3 , 0xDA },
    { _T("영문소문자") , 0xA3 , 0xE1 , 0xA3 , 0xFA },
	{ _T("그림문자")   , 0xA1 , 0xA1 , 0xA1 , 0xFE },
    { _T("그림문자")   , 0xA2 , 0xA1 , 0xA2 , 0xE5 },
    { _T("그림문자")   , 0xA3 , 0xA1 , 0xA3 , 0xFE },
    { _T("그림문자")   , 0xA5 , 0xA1 , 0xA5 , 0xAA },
    { _T("그림문자")   , 0xA5 , 0xB0 , 0xA5 , 0xB9 },
    { _T("그림문자")   , 0xA7 , 0xA1 , 0xA7 , 0xFE },
    { _T("그림문자")   , 0xA8 , 0xB1 , 0xA8 , 0xFE },
    { _T("그림문자")   , 0xA9 , 0xB0 , 0xA9 , 0xFE },
    { _T("한글확장")   , 0x81 , 0x20 , 0xC5 , 0xFF }
};

#define CARD_DEL_CHAR_CNT 29

_TUCHAR DelChar[CARD_DEL_CHAR_CNT] = {
    0xE0 , 0xE1 , 0xE2 , 0xE3 , 0xE4 ,
    0xE5 , 0xE6 , 0xE7 , 0xE8 , 0xE9 ,
    0xEA , 0xEB , 0xEC , 0xED , 0xEE ,
    0xEF , 0xF0 , 0xF1 , 0xF2 , 0xF3 ,
    0xF4 , 0xF5 , 0xF6 , 0xF7 , 0xF8 ,
    0xF9 , 0xFA , 0xFB , 0xFE  };

#define     EX_DEL_CHAR_LIST_CNT 40

typedef struct _EX_DEL_CHAR_LISR {
    _TCHAR          Comment[64];
    _TCHAR          StartChar;
    _TCHAR          LastChar;
} tEX_DEL_CHAR_LISR;

tEX_DEL_CHAR_LISR EX_DEL_CHAR_LISR[EX_DEL_CHAR_LIST_CNT] = {
	{ _T("ETC")                            , 0x0000 , 0x001F }, 
    { _T("LATIN")                          , 0x0020 , 0x002F }, 
    { _T("LATIN")                          , 0x003A , 0x0040 }, 
    { _T("LATIN")                          , 0x005B , 0x0060 }, 
    { _T("LATIN")                          , 0x007B , 0x00BF }, 
    { _T("LATIN")                          , 0x0220 , 0x0221 }, 
    { _T("SPACING MODIFIER LETTERS")       , 0x02B9 , 0x02BF }, 
    { _T("SPACING MODIFIER LETTERS")       , 0x02C2 , 0x02DF }, 
    { _T("SPACING MODIFIER LETTERS")       , 0x02E5 , 0x02EE }, 
    { _T("COMBINING DIACRITICAL MARKS")    , 0x0300 , 0x036F }, 
    { _T("GETERAL PUNCTUATION")            , 0x2000 , 0x206F }, 
    { _T("SUPERSCRIPTS AND SUBSCRIPTS")    , 0x2070 , 0x209F }, 
    { _T("CURRENCY")                       , 0x20A0 , 0x20CF }, 
    { _T("DIACRITICS SYMBOLS")             , 0x20D0 , 0x20FF }, 	
	{ _T("Letterlike Symbols")             , 0x2100 , 0x214F },	
    { _T("ARROW")                          , 0x2190 , 0x21FF },
    { _T("MATHMETICAL SYMBOLS")            , 0x2200 , 0x22FF },
    { _T("MISCELLANEOUS SYMBOLS")          , 0x2300 , 0x23FF },
    { _T("CONTROL PICTURES")               , 0x2400 , 0x243F },
    { _T("OCR")                            , 0x2440 , 0x245F },
    { _T("ENCLOSED ALPHANUMERICS")         , 0x2460 , 0x24FF },
    { _T("BOX DRAWING")                    , 0x2500 , 0x257F },
    { _T("BLOCKS")                         , 0x2580 , 0x259F },
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

#define     EX_MODIFY_CHAR_LIST_CNT 103

typedef struct _EX_MODIFY_CHAR_LIST {
    _TCHAR          Comment[64];
    _TCHAR          RawChar;
    _TCHAR          ModifyChar;
} tEX_MODIFY_CHAR_LIST;

tEX_MODIFY_CHAR_LIST EX_MODIFY_CHAR_LIST[EX_MODIFY_CHAR_LIST_CNT] = {
    { _T("LATIN")      , 0x00C0 , 0x0041 },
    { _T("LATIN")      , 0x00C1 , 0x0041 },
    { _T("LATIN")      , 0x00C2 , 0x0041 },
    { _T("LATIN")      , 0x00C3 , 0x0041 },
	{ _T("LATIN")      , 0x00C4 , 0x0041 },
	{ _T("LATIN")      , 0x00C5 , 0x0041 },
	{ _T("LATIN")      , 0x00C6 , 0x0041 },
	{ _T("LATIN")      , 0x00C7 , 0x0043 },
	{ _T("LATIN")      , 0x00C8 , 0x0045 },
	{ _T("LATIN")      , 0x00C9 , 0x0045 },
	{ _T("LATIN")      , 0x00CA , 0x0045 },
	{ _T("LATIN")      , 0x00CB , 0x0045 },
	{ _T("LATIN")      , 0x00CC , 0x0049 },
	{ _T("LATIN")      , 0x00CD , 0x0049 },
	{ _T("LATIN")      , 0x00CE , 0x0049 },
	{ _T("LATIN")      , 0x00CF , 0x0049 },
	{ _T("LATIN")      , 0x00D0 , 0x0044 },
	{ _T("LATIN")      , 0x00D1 , 0x004E },
	{ _T("LATIN")      , 0x00D2 , 0x004F },
	{ _T("LATIN")      , 0x00D3 , 0x004F },
	{ _T("LATIN")      , 0x00D4 , 0x004F },
	{ _T("LATIN")      , 0x00D5 , 0x004F },
	{ _T("LATIN")      , 0x00D6 , 0x004F },
	{ _T("LATIN")      , 0x00D8 , 0x004F },
	{ _T("LATIN")      , 0x00D9 , 0x0055 },
	{ _T("LATIN")      , 0x00DA , 0x0055 },
	{ _T("LATIN")      , 0x00DB , 0x0055 },
	{ _T("LATIN")      , 0x00DC , 0x0055 },
	{ _T("LATIN")      , 0x00E0 , 0x0061 },
	{ _T("LATIN")      , 0x00E1 , 0x0061 },
	{ _T("LATIN")      , 0x00E2 , 0x0061 },
	{ _T("LATIN")      , 0x00E3 , 0x0061 },
	{ _T("LATIN")      , 0x00E4 , 0x0061 },
	{ _T("LATIN")      , 0x00E5 , 0x0061 },
	{ _T("LATIN")      , 0x00E6 , 0x0063 },
	{ _T("LATIN")      , 0x00E7 , 0x0063 },
	{ _T("LATIN")      , 0x00E8 , 0x0065 },
	{ _T("LATIN")      , 0x00E9 , 0x0065 },
	{ _T("LATIN")      , 0x00EA , 0x0065 },
	{ _T("LATIN")      , 0x00EB , 0x0065 },
	{ _T("LATIN")      , 0x00EC , 0x0069 },
    { _T("LATIN")      , 0x00ED , 0x0069 },
    { _T("LATIN")      , 0x00EE , 0x0069 },
    { _T("LATIN")      , 0x00EF , 0x0069 },
	{ _T("LATIN")      , 0x00F0 , 0x0064 },
	{ _T("LATIN")      , 0x00F1 , 0x006E },
	{ _T("LATIN")      , 0x00F2 , 0x006F },
	{ _T("LATIN")      , 0x00F3 , 0x006F },
	{ _T("LATIN")      , 0x00F4 , 0x006F },
	{ _T("LATIN")      , 0x00F5 , 0x006F },
	{ _T("LATIN")      , 0x00F6 , 0x006F },
	{ _T("LATIN")      , 0x00F7 , 0x006F },
	{ _T("LATIN")      , 0x00F8 , 0x006F },
	{ _T("LATIN")      , 0x00F9 , 0x0075 },
	{ _T("LATIN")      , 0x00FA , 0x0075 },
	{ _T("LATIN")      , 0x00FB , 0x0075 },
	{ _T("LATIN")      , 0x00FC , 0x0075 },
	{ _T("LATIN")      , 0x00FD , 0x0079 },
	{ _T("LATIN")      , 0x00FF , 0x0079 },
	{ _T("CYRILLIC")   , 0x0400 , 0x0415 },
	{ _T("CYRILLIC")   , 0x0401 , 0x0415 },
	{ _T("CYRILLIC")   , 0x0403 , 0x0413 },
	{ _T("CYRILLIC")   , 0x040C , 0x041A },
	{ _T("CYRILLIC")   , 0x040D , 0x0418 },
	{ _T("CYRILLIC")   , 0x040E , 0x0423 },
	{ _T("CYRILLIC")   , 0x0450 , 0x0435 },
	{ _T("CYRILLIC")   , 0x0451 , 0x0435 },
	{ _T("CYRILLIC")   , 0x0453 , 0x0433 },
	{ _T("CYRILLIC")   , 0x045C , 0x043A },
	{ _T("CYRILLIC")   , 0x045D , 0x0438 },
	{ _T("CYRILLIC")   , 0x045E , 0x0443 },
	{ _T("CYRILLIC")   , 0x0478 , 0x0423 },
	{ _T("CYRILLIC")   , 0x04AF , 0x0423 },
	{ _T("CYRILLIC")   , 0x04C1 , 0x0416 },
	{ _T("CYRILLIC")   , 0x04C2 , 0x0436 },
	{ _T("CYRILLIC")   , 0x04D0 , 0x0410 },
	{ _T("CYRILLIC")   , 0x04D1 , 0x0435 },
	{ _T("CYRILLIC")   , 0x04D2 , 0x0410 },
	{ _T("CYRILLIC")   , 0x04D3 , 0x0430 },
	{ _T("CYRILLIC")   , 0x04D6 , 0x0415 },
	{ _T("CYRILLIC")   , 0x04D7 , 0x0435 },
	{ _T("CYRILLIC")   , 0x04DC , 0x0416 },
	{ _T("CYRILLIC")   , 0x04DD , 0x0436 },
	{ _T("CYRILLIC")   , 0x04DE , 0x0417 },
	{ _T("CYRILLIC")   , 0x04DF , 0x0437 },
	{ _T("CYRILLIC")   , 0x04E2 , 0x0418 },
	{ _T("CYRILLIC")   , 0x04E3 , 0x0438 },
	{ _T("CYRILLIC")   , 0x04E4 , 0x0418 },
	{ _T("CYRILLIC")   , 0x04E5 , 0x0438 },
	{ _T("CYRILLIC")   , 0x04E6 , 0x041E },
	{ _T("CYRILLIC")   , 0x04E7 , 0x043E },
	{ _T("CYRILLIC")   , 0x04EC , 0x042D },
	{ _T("CYRILLIC")   , 0x04ED , 0x044D },
	{ _T("CYRILLIC")   , 0x04EE , 0x0423 },
	{ _T("CYRILLIC")   , 0x04EF , 0x0443 },
	{ _T("CYRILLIC")   , 0x04F0 , 0x0423 },
	{ _T("CYRILLIC")   , 0x04F1 , 0x0443 },
	{ _T("CYRILLIC")   , 0x04F2 , 0x0423 },
	{ _T("CYRILLIC")   , 0x04F3 , 0x0443 },
	{ _T("CYRILLIC")   , 0x04F4 , 0x0427 },
	{ _T("CYRILLIC")   , 0x04F5 , 0x0447 },
	{ _T("CYRILLIC")   , 0x04F8 , 0x042B },
	{ _T("CYRILLIC")   , 0x04F9 , 0x044B }
};

unsigned short int CIndex::ConvertSourceMap[] = {
	0X00E1,0X00E0,0X1EA3,0X00E3,0X1EA1,0X00E2,       0X0103,			
	0X00E9,0X00E8,0X1EBB,0X1EBD,0X1EB9,0X00EA,							
	0X00ED,0X00EC,0X1EC9,0X0129,0X1ECB,									
	0X00F3,0X00F2,0X1ECF,0X00F5,0X1ECD,0X00F4,0X01A1,					
	0X00FA,0X00F9,0X1EE7,0X0169,0X1EE5,		  0X01B0,					
	0X00FD,0X12F3,0X1EF7,0X1EF9,0X1EF5,                                 
															 0X0111,	     
	0X1EA5,0X1EA7,0X1EA9,0X1EAB,0X1EAD,0X1EAF,0X1EB1,0X1EB3,0X1EB5,0X1EB7,
	0X1EBF,0X1EC1,0X1EC3,0X1EC5,0X1EC7,                                   
	0X1ED1,0X1ED3,0X1ED5,0X1ED7,0X1ED9,                                   	
	0X0430,0X0431,0X0432,0X0433,0X0434,									
	0X0435,0X0436,0X0437,0X0438,0X0439,									
	0X043A,0X043B,0X043C,0X043D,0X043E,									
	0X043F,0X0440,0X0441,0X0442,0X0443,									
	0X0444,0X0445,0X0446,0X0447,0X0448,									
	0X0449,0X044A,0X044B,0X044C,0X044D,									
	0X044E,0X044F														
};


unsigned short int CIndex::ConvertTargetMap[] = {	
	0X0041,0X0041,0X0041,0X0041,0X0041,0X0041,		 0X0041,
	0X0045,0X0045,0X0045,0X0045,0X0045,0X0045,				
	0X0049,0X0049,0X0049,0X0049,0X0049,						
	0X004F,0X004F,0X004F,0X004F,0X004F,0X004F,0X004F,		
	0X0055,0X0055,0X0055,0X0055,0X0055,		  0X0055,		
	0X0059,0X0059,0X0059,0X0059,0X0059,						
															 0X0044,
	0X0041,0X0041,0X0041,0X0041,0X0041,0X0041,0X0041,0X0041,0X0041,0X0041,
	0X0045,0X0045,0X0045,0X0045,0X0045,                                   
	0X004F,0X004F,0X004F,0X004F,0X004F,                                   	
	0X0410,0X0411,0X0412,0X0413,0X0414,									
	0X0415,0X0416,0X0417,0X0418,0X0419,									
	0X041A,0X041B,0X041C,0X041D,0X041E,									
	0X041F,0X0420,0X0421,0X0422,0X0423,									
	0X0424,0X0425,0X0426,0X0427,0X0428,									
	0X0429,0X042A,0X042B,0X042C,0X042D,									
	0X042E,0X042F														
};

CIndex::CIndex()
{	
	m_CharSetMgr_RoleWord.InitData(_T("역할어"), _T("역할어"));
}

CIndex::~CIndex()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////
VOID CIndex::Init(CMarcMgr *pMarcMgr, CMarc *pMarc)
{
	EFS_BEGIN

	SetMarcMgr(pMarcMgr);
	SetMarc(pMarc);

	EFS_END
}

INT CIndex::GetIndex(CString strIndexAlias, CString &strIndexItem)
{
	EFS_BEGIN

	if (GetMarcMgr() == NULL) 
	{
		m_strLastError = _T("m_pMarcMgr is NULL");
		return -1;
	}
	if (GetMarc() == NULL)
	{
		m_strLastError = _T("m_pMarc is NULL");
		return -1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CIndex::TrimAll(CString &strSrc)
{
	EFS_BEGIN

	strSrc.TrimLeft();
	strSrc.TrimRight();

	return 0;

	EFS_END
	return -1;

}

INT CIndex::RemoveSpace(CString &strSrc)
{
	EFS_BEGIN

	return RemoveChar(strSrc, ' ');

	EFS_END
	return -1;

}

INT CIndex::RemoveChar(CString &strSrc, TCHAR tch)
{
	EFS_BEGIN

	CString strReturn;	
	
	INT nCount = strSrc.GetLength();
	for (INT i = 0; i < nCount; i++)
	{
		TCHAR ch = strSrc.GetAt(i);
		if (ch != tch)
		{
			strReturn += ch;
		}
	}

	strSrc = strReturn;

	return 0;

	EFS_END
	return -1;

}

INT  CIndex::RemoveSpecialChar(CString &strSrc, BOOL f_Flag )
{
	EFS_BEGIN	

	 INT           i,j,idx;
	_TCHAR       *s_tmpbeffer;
    _TCHAR        s_tmp[32];	

    s_tmp[0] = '\0';    
	s_tmpbeffer = (TCHAR*)malloc( (sizeof( TCHAR ) * strSrc.GetLength()) + sizeof( TCHAR ) ) ;

    idx = 0;
    if( strSrc == "" )
	{		
		free( s_tmpbeffer );
		return -100;
	}
	

    for( i = 0; i < strSrc.GetLength() ; i++ ){		
		for( j =0; j < EX_DEL_CHAR_LIST_CNT; j++){
            if( ( strSrc.GetAt(i) >= EX_DEL_CHAR_LISR[j].StartChar && strSrc.GetAt(i) <= EX_DEL_CHAR_LISR[j].LastChar ) ||
                ( f_Flag == TRUE && strSrc.GetAt(i) == _T(' ') ) ){			
				if( ( f_Flag == FALSE && strSrc.GetAt(i) == _T(' ') ) || ( f_Flag == FALSE && strSrc.GetAt(i) == _T('"') ) ) continue;
				
                else break;
            }

			if ( strSrc.GetAt(i) == RT ) {
				break;
			}

			if ( strSrc.GetAt(i) == FT ) {
				break;
			}

			if ( strSrc.GetAt(i) == GT ) {
				break;
			}
        }

        if( j >= EX_DEL_CHAR_LIST_CNT ){
            s_tmpbeffer[idx] = strSrc.GetAt(i);
            idx++;
        }
    }

    s_tmpbeffer[idx] = _T('\0');
    strSrc.Format(_T("%s"), s_tmpbeffer);

	free( s_tmpbeffer );

	return 0;
	EFS_END
	return -1;

}

INT CIndex::RemoveAllParenthesis(CString &strSrc)
{
	EFS_BEGIN

	CString strReturn;

	INT nCount = strSrc.GetLength();
	TCHAR findTch = '(';
	BOOL bFind = FALSE;
	for (INT i = 0; i < nCount; i++)
	{
		TCHAR tch = strSrc.GetAt(i);
		if (tch == findTch)
		{
			bFind = TRUE;
			findTch == ')' ? findTch = '(' : findTch = ')';
			continue;
		}

		if (findTch != ')')
		{
			strReturn += tch;
		}
	}

	strSrc = strReturn;

	if (bFind)
		return 1;
	else 
		return 0;
	

	EFS_END
	return -1;

}

INT CIndex::RemoveOneParenthesis(CString &strSrc)
{
	EFS_BEGIN

	CString strReturn;

	INT nCount = strSrc.GetLength();
	TCHAR findTch = '(';
	BOOL bFind = FALSE;
	for (INT i = 0; i < nCount; i++)
	{
		TCHAR tch = strSrc.GetAt(i);
		if (!bFind && tch == findTch)
		{
			findTch == ')' ? findTch = '(' : findTch = ')';
			continue;
		}

		if (findTch != ')')
		{
			bFind = TRUE;
			strReturn += tch;
		}
	}

	strSrc = strReturn;

	if (bFind)
		return 1;
	else 
		return 0;

	EFS_END
	return -1;

}

INT CIndex::ConvertHanjaToHangul(CString &strSrc)
{
	EFS_BEGIN	

	if ( strSrc == "" ) return 0;
	
	CHAR		*UTF8String;
	_TCHAR		*OutUTF16;
	INT			ids;
	INT			nLen;
	
	CharConvert dlg;	
	
	ids = m_charConvert.UnicodeToUTF8(strSrc.GetBuffer(0) , &UTF8String, &nLen);
	ids = m_charConvert.Hanja2Hangul(UTF8String);
	ids = m_charConvert.UTF8ToUnicode(UTF8String, &OutUTF16, &nLen);
	
	strSrc.Format(_T("%s"), OutUTF16 );
	
	free(UTF8String);
	free(OutUTF16);

	
	return 0;

	EFS_END
	return -1;

}

INT CIndex::ConvertHanjaToHangul(TCHAR *pUniTemp, TCHAR *OutUTF16)
{
	EFS_BEGIN	
	
	CHAR		*UTF8String;	
	INT			ids;
	INT			nLen;
	
	CharConvert dlg;	
	
	ids = dlg.UnicodeToUTF8(pUniTemp , &UTF8String, &nLen);	
	ids = dlg.Hanja2Hangul(UTF8String);	
	ids = dlg.UTF8ToUnicode(UTF8String, &OutUTF16, &nLen);	
	free(UTF8String);		

	
	return 0;

	EFS_END
	return -1;

}

INT CIndex::MakeIndex(CString &strSrc)
{
	EFS_BEGIN

	strSrc.MakeUpper();
	RemoveSpace(strSrc);
	RemoveSpecialChar(strSrc);		
	UnicodeSpecialConvert(strSrc);		
	ConvertHanjaToHangul(strSrc);
	TrimAll(strSrc);

	return 0;

	EFS_END
	return -1;

}

INT CIndex::MakeIndex_NonSpace(CString &strSrc)
{
	EFS_BEGIN

	strSrc.MakeUpper();	
	RemoveSpecialChar(strSrc);
	// 2009.03.20 ADD BY PDJ : Unicode Special Convert
	UnicodeSpecialConvert(strSrc);
	TrimAll(strSrc);

	return 0;

	EFS_END
	return -1;

}

VOID CIndex::UnicodeSpecialConvert(CString &strText)
{	
	TCHAR szChar;		    

	for ( int i=0 ; i<strText.GetLength() ; i++ )
	{
		szChar = (TCHAR)strText.GetAt(i);
		
		for ( int y=0 ; y<89 ; y++ )	
		{
			if ( szChar == ConvertSourceMap[y] )
			{
				strText.SetAt(i, ConvertTargetMap[y]);				
				break;
			}
		}
	}
}

INT CIndex::MakeIndex(TCHAR* srcData, TCHAR* keywordData)
{
	EFS_BEGIN

	DelSpecialChar( srcData, keywordData  );
    ModifySpecialChar( keywordData, keywordData );
    _tcsupr( keywordData );

	return 0;

	EFS_END
	return -1;

}

VOID CIndex::DelSpecialChar( TCHAR *f_pRawStr , TCHAR *f_pWorkStr , BOOL f_Flag )
{
    EFS_BEGIN

    INT           i,j,idx;
	_TCHAR       *s_tmpbeffer;
    _TCHAR        s_tmp[32];

    s_tmp[0] = '\0';

	s_tmpbeffer = (TCHAR*)malloc( (sizeof( TCHAR ) * _tcslen( f_pRawStr )) + 2 ) ;

    idx = 0;
    if( f_pRawStr == NULL )
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

    EFS_END
}

VOID CIndex::ModifySpecialChar( _TCHAR *f_pRawStr , _TCHAR *f_pWorkStr )
{
EFS_BEGIN

    INT           i,j,idx;
	_TCHAR       *s_tmpbeffer;


	s_tmpbeffer = (TCHAR*)malloc( (sizeof( TCHAR ) * _tcslen( f_pRawStr )) + 2 ) ;

    idx = 0;
    if( f_pRawStr == NULL )
	{
		f_pWorkStr = NULL  ;
		free( s_tmpbeffer );
		return;
	}

    for( i = 0; f_pRawStr[i] != _T('\0'); i++ ){
        for( j =0; j < EX_MODIFY_CHAR_LIST_CNT; j++){
            if( f_pRawStr[i] == EX_MODIFY_CHAR_LIST[j].RawChar){
                s_tmpbeffer[i] = EX_MODIFY_CHAR_LIST[j].ModifyChar;
                break;
            }
        }

        if( j >= EX_MODIFY_CHAR_LIST_CNT ){
            s_tmpbeffer[i] = f_pRawStr[i];
        }
    }

    s_tmpbeffer[i] = _T('\0');
    _tcscpy( f_pWorkStr , s_tmpbeffer );

	free( s_tmpbeffer );

EFS_END
}

void CIndex::ChangeBlankSpecialChar( TCHAR *f_pRawStr , TCHAR *f_pWorkStr )
{
    EFS_BEGIN

    INT           i,j,idx;
	_TCHAR       *s_tmpbeffer;    

	if( f_pRawStr == NULL )
	{		
		return;
	}

	s_tmpbeffer = (TCHAR*)malloc( sizeof( TCHAR )* (_tcslen( f_pRawStr )+1) );
	if( s_tmpbeffer == NULL ){
		return;
	}

    idx = 0;
    for( i = 0; f_pRawStr[i] != _T('\0'); i++ ){
        
		for( j =0; j < EX_DEL_CHAR_LIST_CNT; j++){
            if( f_pRawStr[i] >= EX_DEL_CHAR_LISR[j].StartChar && f_pRawStr[i] <= EX_DEL_CHAR_LISR[j].LastChar ) {
				break;
            }
        }

        if( j >= EX_DEL_CHAR_LIST_CNT ){
            s_tmpbeffer[idx] = f_pRawStr[i];
            idx++;
        } else {
			s_tmpbeffer[idx] = _T(' ');
			idx++;
		}
    }
	
    s_tmpbeffer[idx] = _T('\0');

	f_pWorkStr = (TCHAR*)realloc( f_pWorkStr, sizeof( TCHAR )* (_tcslen(s_tmpbeffer)+1 ) );
	if( f_pWorkStr == NULL ){
		free( s_tmpbeffer );
		return;
	}

    _tcscpy( f_pWorkStr , s_tmpbeffer );	
	free( s_tmpbeffer );	

    EFS_END
}


VOID CIndex::TrimRoleWord(CString &strAuthor)
{
	INT nFind = strAuthor.ReverseFind(' ');
	if (nFind > 0)
	{
		CString str1 = strAuthor.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();		
		
		if (m_CharSetMgr_RoleWord.Find(str1, str2) == 0)
		{
			strAuthor.Delete(nFind, strAuthor.GetLength() - nFind);
		}
	}
}


VOID CIndex::DupIndexRemove(CString &strIndex)
{
	CStringArray	strArrayIndexItem;
	CString			strTotalItem;
	CString			strItem;
	INT				nIndex;
	INT				nOffset;
	BOOL			bDup;

	nOffset =0;
	nIndex = strIndex.Find(_T(" "), nOffset );
	if ( nIndex == -1 ) return;
	
	nOffset = 0;
	nIndex = 0;
	strArrayIndexItem.RemoveAll();
	while ( TRUE )
	{
		nIndex = strIndex.Find(_T(" "), nOffset );
		if ( nIndex == -1 )
		{
			strItem = strIndex.Mid(nOffset);			
		}
		else
		{
			strItem = strIndex.Mid(nOffset, nIndex-nOffset);
		}
		
		nOffset = nIndex+1;
		
		bDup = FALSE;
		for ( int i=0 ; i<strArrayIndexItem.GetSize() ; i++ )
		{
			if ( strItem == strArrayIndexItem.GetAt(i) )
			{
				bDup = TRUE;
				break;
			}
		}
		
		if ( bDup == FALSE )
		{
			strArrayIndexItem.Add(strItem);			
		}	
		
		if ( nIndex == -1 ) break;
	}
	

	strTotalItem = _T("");
	for ( int i=0 ; i<strArrayIndexItem.GetSize() ; i++ )
	{
		if ( !strTotalItem.IsEmpty() ) strTotalItem += _T(" ");
		strTotalItem += strArrayIndexItem.GetAt(i);
	}

	INT nUTF8Len;
	nUTF8Len = GetUTF8Length(strTotalItem.GetBuffer(0));

	if ( nUTF8Len > 3999 ) 
	{
		INT nRealCutPos;

		nRealCutPos = GetUTF8Length(strTotalItem.GetBuffer(0), 3999);
		strIndex = strTotalItem.Left(nRealCutPos);

	}
	else
	{
		strIndex = strTotalItem;
	}	
}


INT	CIndex::GetUTF8Length(_TCHAR *fi_pSourceData)
{
	INT		nWCharSource;
	INT		nBytesNeeded;

	nWCharSource   = wcslen(fi_pSourceData);

	nBytesNeeded = WideCharToMultiByte( CP_UTF8 	, 
										0				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL, 
										NULL );
	return nBytesNeeded;
}


INT	CIndex::GetUTF8Length(_TCHAR *fi_pSourceData, INT nCutLength)
{
	CHAR	*pszDestinationData;
	INT		nWCharSource;
	INT		nBytesNeeded;	
	
	nWCharSource   = wcslen(fi_pSourceData);
	if ( nWCharSource <= 0 ) {		
		return 0;
	}
	
	/* Query the number of bytes required to store the Dest string */
	nBytesNeeded = WideCharToMultiByte( CP_UTF8				 	, 
										0						,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL					, 
										NULL					 );
	
	/* Allocate the required amount of space */
	if (nBytesNeeded == 0) 
	{		
		return 0 ;
	}
		
	/* We need more 1 byte for '\0' */
	pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
		
	/* Do the conversion */
	nBytesNeeded = WideCharToMultiByte     (CP_UTF8		, 
											0					,
											(LPWSTR)fi_pSourceData		, 
											nWCharSource				,
											(CHAR*)pszDestinationData	, 
											nBytesNeeded				, 
											NULL			    , 
										    NULL );
	if (nBytesNeeded == 0) 
	{		
		free(pszDestinationData);
		return 0 ;
	}

	INT nSouceDataByte;
	INT	nWCharNeeded;
	nSouceDataByte = nBytesNeeded;
	if ( nBytesNeeded > nCutLength )
	{
		*(LPSTR)((LPSTR)pszDestinationData + nCutLength) = '\0';	
		nSouceDataByte = nCutLength;
	}
	else
	{
		*(LPSTR)((LPSTR)pszDestinationData + nBytesNeeded) = '\0';			
	}	

	nWCharNeeded = MultiByteToWideChar( CP_UTF8 , 
										0  , 
										pszDestinationData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );

	free(pszDestinationData);

	return nWCharNeeded;
}
