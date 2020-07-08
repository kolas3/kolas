// BarCode.cpp:
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BarCode.h"

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aI2of5[] = {
	{ START, _T("111")        },
	{ STOP,	 _T("311")        },
	{ 3,     _T("1111111111") },
	{-1,     NULL         }
};
	 
PATTERNS a2of5[] = {
	{ '0'  , _T("111131311") },
	{ '1'  , _T("311111113") },
	{ '2'  , _T("113111113") },
	{ '3'  , _T("313111111") },
	{ '4'  , _T("111131113") },
	{ '5'  , _T("311131111") },
	{ '6'  , _T("113131111") },
	{ '7'  , _T("111111313") },
	{ '8'  , _T("311111311") },
	{ '9'  , _T("113111311") },
	{ START, _T("31311")     },
	{ STOP , _T("31113")     },
	{  -1  , NULL        }
};

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
MAPS a3of9e[] = {
	{ 0,	_T("%U") },
	{ 1,	_T("$A") },
	{ 2,	_T("$B") },
	{ 3,	_T("$C") },
	{ 4,	_T("$D") },
	{ 5,	_T("$E") },
	{ 6,	_T("$F") },
	{ 7,	_T("$G") },
	{ 8,	_T("$H") },
	{ 9,	_T("$I") },
	{ 10,	_T("$J") },
	{ 11,	_T("$K") },
	{ 12,	_T("$L") },
	{ 13,	_T("$M") },
	{ 14,	_T("$N") },
	{ 15,	_T("$O") },
	{ 16,	_T("$P") },
	{ 17,	_T("$Q") },
	{ 18,	_T("$R") },
	{ 19,	_T("$S") },
	{ 20,	_T("$T") },
	{ 21,	_T("$U") },
	{ 22,	_T("$V") },
	{ 23,	_T("$W") },
	{ 24,	_T("$X") },
	{ 25,	_T("$Y") },
	{ 26,	_T("$Z") },
	{ 27,	_T("%A") },
	{ 28,	_T("%B") },
	{ 29,	_T("%C") },
	{ 30,	_T("%D") },
	{ 31,	_T("%E") },
	{ '!',	_T("/A") },
	{ '"',	_T("/B") },
	{ '#',	_T("/C") },
	{ '$',	_T("/D") },
	{ '%',	_T("/E") },
	{ '&',	_T("/F") },
	{ 39, 	_T("/G") },
	{ '(',	_T("/H") },
	{ ')',	_T("/I") },
	{ '*',	_T("/J") },
	{ '+',	_T("/K") },
	{ ',',	_T("/L") },
	{ '/',	_T("/O") },
	{ ':',	_T("/Z") },
	{ ';',	_T("%F") },
	{ '<',	_T("%G") },
	{ '=',	_T("%H") },
	{ '>',	_T("%I") },
	{ '?',	_T("%J") },
	{ '@',	_T("%V") },
	{ '[',	_T("%K") },
	{ '\\',	_T("%L") },
	{ ']',	_T("%M") },
	{ '^',	_T("%N") },
	{ '_',	_T("%O") },
	{ 96, 	_T("%W") },
	{ 'a',	_T("+A") },
	{ 'b',	_T("+B") },
	{ 'c',	_T("+C") },
	{ 'd',	_T("+D") },
	{ 'e',	_T("+E") },
	{ 'f',	_T("+F") },
	{ 'g',	_T("+G") },
	{ 'h',	_T("+H") },
	{ 'i',	_T("+I") },
	{ 'j',	_T("+J") },
	{ 'k',	_T("+K") },
	{ 'l',	_T("+L") },
	{ 'm',	_T("+M") },
	{ 'n',	_T("+N") },
	{ 'o',	_T("+O") },
	{ 'p',	_T("+P") },
	{ 'q',	_T("+Q") },
	{ 'r',	_T("+R") },
	{ 's',	_T("+S") },
	{ 't',	_T("+T") },
	{ 'u',	_T("+U") },
	{ 'v',	_T("+V") },
	{ 'w',	_T("+W") },
	{ 'x',	_T("+X") },
	{ 'y',	_T("+Y") },
	{ 'z',	_T("+Z") },
	{ '{',	_T("%P") },
	{ '|',	_T("%Q") },
	{ '}',	_T("%R") },
	{ '~',	_T("%S") },
	{ 127,	_T("%T") },
	-1, NULL
};
	
PATTERNS a3of9[] = {
	{ '1', _T("211211112") },
	{ '2', _T("112211112") },
	{ '3', _T("212211111") },
	{ '4', _T("111221112") },
	{ '5', _T("211221111") },
	{ '6', _T("112221111") },
	{ '7', _T("111211212") },
	{ '8', _T("211211211") },
	{ '9', _T("112211211") },
	{ '0', _T("111221211") },
	{ 'A', _T("211112112") },
	{ 'B', _T("112112112") },
	{ 'C', _T("212112111") },
	{ 'D', _T("111122112") },
	{ 'E', _T("211122111") },
	{ 'F', _T("112122111") },
	{ 'G', _T("111112212") },
	{ 'H', _T("211112211") },
	{ 'I', _T("112112211") },
	{ 'J', _T("111122211") },
	{ 'K', _T("211111122") },
	{ 'L', _T("112111122") },
	{ 'M', _T("212111121") },
	{ 'N', _T("111121122") },
	{ 'O', _T("211121121") },
	{ 'P', _T("112121121") },
	{ 'Q', _T("111111222") },
	{ 'R', _T("211111221") },
	{ 'S', _T("112111221") },
	{ 'T', _T("111121221") },
	{ 'U', _T("221111112") },
	{ 'V', _T("122111112") },
	{ 'W', _T("222111111") },
	{ 'X', _T("121121112") },
	{ 'Y', _T("221121111") },
	{ 'Z', _T("122121111") },
	{ '-', _T("121111212") },
	{ '.', _T("221111211") },
	{ ' ', _T("122111211") },
	{ '*', _T("121121211") },
	{ '$', _T("121212111") },
	{ '/', _T("121211121") },
	{ '+', _T("121112121") },
	{ '%', _T("111212121") },
	{ START, _T("121121211") },
	{ STOP, _T("121121211") },
	{  -1, NULL }
};
	
PATTERNS aANSI3of9[] = {
	{ '1', _T("311311113") },
	{ '2', _T("113311113") },
	{ '3', _T("313311111") },
	{ '4', _T("111331113") },
	{ '5', _T("311331111") },
	{ '6', _T("113331111") },
	{ '7', _T("111311313") },
	{ '8', _T("311311311") },
	{ '9', _T("113311311") },
	{ '0', _T("111331311") },
	{ 'A', _T("311113113") },
	{ 'B', _T("113113113") },
	{ 'C', _T("313113111") },
	{ 'D', _T("111133113") },
	{ 'E', _T("311133111") },
	{ 'F', _T("113133111") },
	{ 'G', _T("111113313") },
	{ 'H', _T("311113311") },
	{ 'I', _T("113113311") },
	{ 'J', _T("111133311") },
	{ 'K', _T("311111133") },
	{ 'L', _T("113111133") },
	{ 'M', _T("313111131") },
	{ 'N', _T("111131133") },
	{ 'O', _T("311131131") },
	{ 'P', _T("113131131") },
	{ 'Q', _T("111111333") },
	{ 'R', _T("311111331") },
	{ 'S', _T("113111331") },
	{ 'T', _T("111131331") },
	{ 'U', _T("331111113") },
	{ 'V', _T("133111113") },
	{ 'W', _T("333111111") },
	{ 'X', _T("131131113") },
	{ 'Y', _T("331131111") },
	{ 'Z', _T("133131111") },
	{ '-', _T("131111313") },
	{ '.', _T("331111311") },
	{ ' ', _T("133111311") },
	{ '*', _T("131131311") },
	{ '$', _T("131313111") },
	{ '/', _T("131311131") },
	{ '+', _T("131113131") },
	{ '%', _T("111313131") },
	{ START, _T("131131311") },
	{ STOP, _T("131131311") },
	{  -1, NULL }
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aCode128A[] = {
	{ ' ',   _T("212221") },
	{ '!',   _T("222121") },
	{ '"',   _T("222220") },
	{ '#',   _T("121222") },
	{ '$',   _T("121321") },
	{ '%',   _T("131221") },
	{ '&',   _T("122212") },
	{  39,   _T("122311") },
	{ '(',   _T("132211") },
	{ ')',   _T("221212") },
	{ '*',   _T("221311") },
	{ '+',   _T("231211") },
	{ ',',   _T("112231") },
	{ '-',   _T("122131") },
	{ '.',   _T("122230") },
	{ '/',   _T("113221") },
	{ '0',   _T("123121") },
	{ '1',   _T("123220") },
	{ '2',   _T("223210") },
	{ '3',   _T("221131") },
	{ '4',   _T("221230") },
	{ '5',   _T("213211") },
	{ '6',   _T("223111") },
	{ '7',   _T("312130") },
	{ '8',   _T("311221") },
	{ '9',   _T("321121") },
	{ ':',   _T("321220") },
	{ ';',   _T("312211") },
	{ '<',   _T("322111") },
	{ '=',   _T("322210") },
	{ '>',   _T("212122") },
	{ '?',   _T("212320") },
	{ '@',   _T("232120") },
	{ 'A',   _T("111322") },
	{ 'B',   _T("131122") },
	{ 'C',   _T("131320") },
	{ 'D',   _T("112312") },
	{ 'E',   _T("132112") },
	{ 'F',   _T("132310") },
	{ 'G',   _T("211312") },
	{ 'H',   _T("231112") },
	{ 'I',   _T("231310") },
	{ 'J',   _T("112132") },
	{ 'K',   _T("112330") },
	{ 'L',   _T("132130") },
	{ 'M',   _T("113122") },
	{ 'N',   _T("113320") },
	{ 'O',   _T("133120") },
	{ 'P',   _T("313120") },
	{ 'Q',   _T("211330") },
	{ 'R',   _T("231130") },
	{ 'S',   _T("213112") },
	{ 'T',   _T("213310") },
	{ 'U',   _T("213130") },
	{ 'V',   _T("311122") },
	{ 'W',   _T("311320") },
	{ 'X',   _T("331120") },
	{ 'Y',   _T("312112") },
	{ 'Z',   _T("312310") },
	{ '[',   _T("332110") },
	{  92,   _T("314110") },
	{ ']',   _T("221410") },
	{ '^',   _T("431110") },
	{ '_',   _T("111223") },
	{   0,   _T("111421") },
	{   1,   _T("121123") },
	{   2,   _T("121420") },
	{   3,   _T("141121") },
	{   4,   _T("141220") },
	{   5,   _T("112213") },
	{   6,   _T("112411") },
	{   7,   _T("122113") },
	{   8,   _T("122410") },
	{   9,   _T("142111") },
	{  10,   _T("142210") },
	{  11,   _T("241210") },
	{  12,   _T("221113") },
	{  13,   _T("413110") },
	{  14,   _T("241111") },
	{  15,   _T("134110") },
	{  16,   _T("111241") },
	{  17,   _T("121141") },
	{  18,   _T("121240") },
	{  19,   _T("114211") },
	{  20,   _T("124111") },
	{  21,   _T("124210") },
	{  22,   _T("411211") },
	{  23,   _T("421111") },
	{  24,   _T("421210") },
	{  25,   _T("212140") },
	{  26,   _T("214120") },
	{  27,   _T("412120") },
	{  28,   _T("111142") },
	{  29,   _T("111340") },
	{  30,   _T("131140") },
	{  31,   _T("114112") },
	{ FNC3,  _T("114310") },
	{ FNC2,  _T("411112") },
	{ SHIFT, _T("411310") },
	{ CODEC, _T("113140") },
	{ CODEB, _T("114130") },
	{ FNC4,  _T("311140") },
	{ FNC1,  _T("411130") },
	{ START, _T("211411") },
	{ STARTB,_T("211213") }, // 128B start
	{ STARTC,_T("211231") }, // 128C start
	{ STOP,  _T("2331112") },
	{  -1, NULL }
};

PATTERNS aCode128B[] = {
	{ ' ',   _T("212221") },
	{ '!',   _T("222121") },
	{ '"',   _T("222220") },
	{ '#',   _T("121222") },
	{ '$',   _T("121321") },
	{ '%',   _T("131221") },
	{ '&',   _T("122212") },
	{  39,   _T("122311") },
	{ '(',   _T("132211") },
	{ ')',   _T("221212") },
	{ '*',   _T("221311") },
	{ '+',   _T("231211") },
	{ ',',   _T("112231") },
	{ '-',   _T("122131") },
	{ '.',   _T("122230") },
	{ '/',   _T("113221") },
	{ '0',   _T("123121") },
	{ '1',   _T("123220") },
	{ '2',   _T("223210") },
	{ '3',   _T("221131") },
	{ '4',   _T("221230") },
	{ '5',   _T("213211") },
	{ '6',   _T("223111") },
	{ '7',   _T("312130") },
	{ '8',   _T("311221") },
	{ '9',   _T("321121") },
	{ ':',   _T("321220") },
	{ ';',   _T("312211") },
	{ '<',   _T("322111") },
	{ '=',   _T("322210") },
	{ '>',   _T("212122") },
	{ '?',   _T("212320") },
	{ '@',   _T("232120") },
	{ 'A',   _T("111322") },
	{ 'B',   _T("131122") },
	{ 'C',   _T("131320") },
	{ 'D',   _T("112312") },
	{ 'E',   _T("132112") },
	{ 'F',   _T("132310") },
	{ 'G',   _T("211312") },
	{ 'H',   _T("231112") },
	{ 'I',   _T("231310") },
	{ 'J',   _T("112132") },
	{ 'K',   _T("112330") },
	{ 'L',   _T("132130") },
	{ 'M',   _T("113122") },
	{ 'N',   _T("113320") },
	{ 'O',   _T("133120") },
	{ 'P',   _T("313120") },
	{ 'Q',   _T("211330") },
	{ 'R',   _T("231130") },
	{ 'S',   _T("213112") },
	{ 'T',   _T("213310") },
	{ 'U',   _T("213130") },
	{ 'V',   _T("311122") },
	{ 'W',   _T("311320") },
	{ 'X',   _T("331120") },
	{ 'Y',   _T("312112") },
	{ 'Z',   _T("312310") },
	{ '[',   _T("332110") },
	{  92,   _T("314110") },
	{ ']',   _T("221410") },
	{ '^',   _T("431110") },
	{ '_',   _T("111223") },
	{   0,   _T("111421") },
	{ 'a',   _T("121123") },
	{ 'b',   _T("121420") },
	{ 'c',   _T("141121") },
	{ 'd',   _T("141220") },
	{ 'e',   _T("112213") },
	{ 'f',   _T("112411") },
	{ 'g',   _T("122113") },
	{ 'h',   _T("122410") },
	{ 'i',   _T("142111") },
	{ 'j',   _T("142210") },
	{ 'k',   _T("241210") },
	{ 'l',   _T("221113") },
	{ 'm',   _T("413110") },
	{ 'n',   _T("241111") },
	{ 'o',   _T("134110") },
	{ 'p',   _T("111241") },
	{ 'q',   _T("121141") },
	{ 'r',   _T("121240") },
	{ 's',   _T("114211") },
	{ 't',   _T("124111") },
	{ 'u',   _T("124210") },
	{ 'v',   _T("411211") },
	{ 'w',   _T("421111") },
	{ 'x',   _T("421210") },
	{ 'y',   _T("212140") },
	{ 'z',   _T("214120") },
	{ '{',   _T("412120") },
	{ '|',   _T("111142") },
	{ '}',   _T("111340") },
	{ '~',   _T("131140") },
	{ 127,   _T("114112") },
	{ FNC3,  _T("114310") },
	{ FNC2,  _T("411112") },
	{ SHIFT, _T("411310") },
	{ CODEC, _T("113140") },
	{ FNC4,  _T("114130") },
	{ CODEA, _T("311140") },
	{ FNC1,  _T("411130") },
	{ STARTA,_T("211411") }, // 128A start
	{ START, _T("211213") },
	{ STARTC,_T("211231") }, // 128C start
	{ STOP,  _T("2331112") },
	{  -1, NULL }
};

PATTERNS aCode128C[] = {
	{   0,   _T("212221") },
	{   1,   _T("222121") },
	{   2,   _T("222220") },
	{   3,   _T("121222") },
	{   4,   _T("121321") },
	{   5,   _T("131221") },
	{   6,   _T("122212") },
	{   7,   _T("122311") },
	{   8,   _T("132211") },
	{   9,   _T("221212") },
	{  10,   _T("221311") },
	{  11,   _T("231211") },
	{  12,   _T("112231") },
	{  13,   _T("122131") },
	{  14,   _T("122230") },
	{  15,   _T("113221") },
	{  16,   _T("123121") },
	{  17,   _T("123220") },
	{  18,   _T("223210") },
	{  19,   _T("221131") },
	{  20,   _T("221230") },
	{  21,   _T("213211") },
	{  22,   _T("223111") },
	{  23,   _T("312130") },
	{  24,   _T("311221") },
	{  25,   _T("321121") },
	{  26,   _T("321220") },
	{  27,   _T("312211") },
	{  28,   _T("322111") },
	{  29,   _T("322210") },
	{  30,   _T("212122") },
	{  31,   _T("212320") },
	{  32,   _T("232120") },
	{  33,   _T("111322") },
	{  34,   _T("131122") },
	{  35,   _T("131320") },
	{  36,   _T("112312") },
	{  37,   _T("132112") },
	{  38,   _T("132310") },
	{  39,   _T("211312") },
	{  40,   _T("231112") },
	{  41,   _T("231310") },
	{  42,   _T("112132") },
	{  43,   _T("112330") },
	{  44,   _T("132130") },
	{  45,   _T("113122") },
	{  46,   _T("113320") },
	{  47,   _T("133120") },
	{  48,   _T("313120") },
	{  49,   _T("211330") },
	{  50,   _T("231130") },
	{  51,   _T("213112") },
	{  52,   _T("213310") },
	{  53,   _T("213130") },
	{  54,   _T("311122") },
	{  55,   _T("311320") },
	{  56,   _T("331120") },
	{  57,   _T("312112") },
	{  58,   _T("312310") },
	{  59,   _T("332110") },
	{  60,   _T("314110") },
	{  61,   _T("221410") },
	{  62,   _T("431110") },
	{  63,   _T("111223") },
	{  64,   _T("111421") },
	{  65,   _T("121123") },
	{  66,   _T("121420") },
	{  67,   _T("141121") },
	{  68,   _T("141220") },
	{  69,   _T("112213") },
	{  70,   _T("112411") },
	{  71,   _T("122113") },
	{  72,   _T("122410") },
	{  73,   _T("142111") },
	{  74,   _T("142210") },
	{  75,   _T("241210") },
	{  76,   _T("221113") },
	{  77,   _T("413110") },
	{  78,   _T("241111") },
	{  79,   _T("134110") },
	{  80,   _T("111241") },
	{  81,   _T("121141") },
	{  82,   _T("121240") },
	{  83,   _T("114211") },
	{  84,   _T("124111") },
	{  85,   _T("124210") },
	{  86,   _T("411211") },
	{  87,   _T("421111") },
	{  88,   _T("421210") },
	{  89,   _T("212140") },
	{  90,   _T("214120") },
	{  91,   _T("412120") },
	{  92,   _T("111142") },
	{  93,   _T("111340") },
	{  94,   _T("131140") },
	{  95,   _T("114112") },
	{  96,   _T("114310") },
	{  97,   _T("411112") },
	{  98,   _T("411310") },
	{  99,   _T("113140") },
	{ CODEB, _T("114130") },
	{ CODEA, _T("311140") },
	{ FNC1,  _T("411130") },
	{ STARTA,_T("211411") }, // 128A start
	{ STARTB,_T("211213") }, // 128B start
	{ START, _T("211231") },
	{ STOP,  _T("2331112") },
	{  -1, NULL }
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aCodabar[] = {
	{ '0', _T("1111133") },
	{ '1', _T("1111331") },
	{ '2', _T("1113113") },
	{ '3', _T("3311111") },
	{ '4', _T("1131131") },
	{ '5', _T("3111131") },
	{ '6', _T("1311113") },
	{ '7', _T("1311311") },
	{ '8', _T("1331111") },
	{ '9', _T("3113111") },

	{ '-', _T("1113311") },
	{ '$', _T("1133111") },
	{ ':', _T("3111313") },
	{ '/', _T("3131113") },
	{ '.', _T("3131311") },
	{ '+', _T("1131313") },

	{ 'A', _T("1133131") },
	{ 'B', _T("1313113") },
	{ 'C', _T("1113133") },
	{ 'D', _T("1113331") },

	{ -1,  NULL }
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aMSI[] = {
	{ '0', _T("12121211") },
	{ '1', _T("12121220") },
	{ '2', _T("12122111") },
	{ '3', _T("12122120") },
	{ '4', _T("12211211") },
	{ '5', _T("12211220") },
	{ '6', _T("12212111") },
	{ '7', _T("12212120") },
	{ '8', _T("21121211") },
	{ '9', _T("21121220") },
	{ START, _T("2") },
	{ STOP, _T("121") },
	{  -1, NULL }
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aPostNet[] = {
	{ '0',		_T("11000") },
	{ '1',		_T("00011") },
	{ '2',		_T("00101") },
	{ '3',		_T("00110") },
	{ '4',		_T("01001") },
	{ '5',		_T("01010") },
	{ '6',		_T("01100") },
	{ '7',		_T("10001") },
	{ '8',		_T("10010") },
	{ '9',		_T("10100") },
	{ START,	_T("1") },
	{ STOP,		_T("1") },
	{ -1,		NULL }
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aEANA[] = {
	{ '0', _T("03211") },
	{ '1', _T("02221") },
	{ '2', _T("02122") },
	{ '3', _T("01411") },
	{ '4', _T("01132") },
	{ '5', _T("01231") },
	{ '6', _T("01114") },
	{ '7', _T("01312") },
	{ '8', _T("01213") },
	{ '9', _T("03112") },
	{ START, _T("111") },
	{ STOP, _T("111") },
	{ 3, _T("011111") },
	{ -1, NULL }
};

PATTERNS aEANB[] = {
	{ '0', _T("01123") },
	{ '1', _T("01222") },
	{ '2', _T("02212") },
	{ '3', _T("01141") },
	{ '4', _T("02311") },
	{ '5', _T("01321") },
	{ '6', _T("04111") },
	{ '7', _T("02131") },
	{ '8', _T("03121") },
	{ '9', _T("02113") },
	{ START, _T("111") },
	{ STOP, _T("111") },
	{ 3, _T("011111") },
	{ -1, NULL }
};

LPCTSTR abEANParity[ 10 ] = { 
	_T("AAAAAA"), 
    _T("AABABB"),
	_T("AABBAB"),
	_T("AABBBA"),
    _T("ABAABB"),
	_T("ABBAAB"),
	_T("ABBBAA"),
	_T("ABABAB"),
	_T("ABABBA"),
	_T("ABBABA") 
};

PATTERNS aEANC[] = {
	{ '0'  , _T("3211")   },
	{ '1'  , _T("2221")   },
	{ '2'  , _T("2122")   },
	{ '3'  , _T("1411")   },
	{ '4'  , _T("1132")   },
	{ '5'  , _T("1231")   },
	{ '6'  , _T("1114")   },
	{ '7'  , _T("1312")   },
	{ '8'  , _T("1213")   },
	{ '9'  , _T("3112")   },
	{ START, _T("111")    },
	{ STOP , _T("111")    },
	{    3 , _T("011111") },
	{   -1 , NULL     }
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS aUPCOdd[] = {
	{ '0', _T("03211") },
	{ '1', _T("02221") },
	{ '2', _T("02122") },
	{ '3', _T("01411") },
	{ '4', _T("01132") },
	{ '5', _T("01231") },
	{ '6', _T("01114") },
	{ '7', _T("01312") },
	{ '8', _T("01213") },
	{ '9', _T("03112") },
	{ START, _T("111") },
	{ STOP, _T("111") },
	{  3,  _T("011111") },
	{  -1, NULL }
};

PATTERNS aUPCEven[] = {
	{ '0', _T("3211") },
	{ '1', _T("2221") },
	{ '2', _T("2122") },
	{ '3', _T("1411") },
	{ '4', _T("1132") },
	{ '5', _T("1231") },
	{ '6', _T("1114") },
	{ '7', _T("1312") },
	{ '8', _T("1213") },
	{ '9', _T("3112") },
	{   START, _T("111") },
	{   STOP, _T("111") },
	{  3,  _T("011111") },
	{  -1, NULL }
};

PATTERNS aUPCEOdd[] = {
	{ '0', _T("02211") },
	{ '1', _T("01221") },
	{ '2', _T("01122") },
	{ '3', _T("411") },
	{ '4', _T("132") },
	{ '5', _T("231") },
	{ '6', _T("114") },
	{ '7', _T("312") },
	{ '8', _T("213") },
	{ '9', _T("02112") },
	{ START, _T("111") },
	{ STOP, _T("11111") },
	{  -1, NULL }
};

PATTERNS aUPCEEven[] = {
	{ '0', _T("123") },
	{ '1', _T("222") },
	{ '2', _T("01212") },
	{ '3', _T("141") },
	{ '4', _T("01311") },
	{ '5', _T("321") },
	{ '6', _T("03111") },
	{ '7', _T("01131") },
	{ '8', _T("02121") },
	{ '9', _T("01113") },
	{ START, _T("111") },
	{ STOP, _T("11111") },
	{  -1, NULL }
};

LPCTSTR aUPCEPattern[ 10 ] = {
	_T("EEEOOO"),   // 0
	_T("EEOEOO"),   // 1
	_T("EEOOEO"),   // 2
	_T("EEOOOE"),   // 3
	_T("EOEEOO"),   // 4
	_T("EOOEEO"),   // 5
	_T("EOOOEE"),   // 6
	_T("EOEOEO"),   // 7
	_T("EOEOOE"),   // 8
	_T("EOOEOE")    // 9
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
MAPS a93e[] = {
	{ 0,	_T("ûU") },
	{ 1,	_T("úA") },
	{ 2,	_T("úB") },
	{ 3,	_T("úC") },
	{ 4,	_T("úD") },
	{ 5,	_T("úE") },
	{ 6,	_T("úF") },
	{ 7,	_T("úG") },
	{ 8,	_T("úH") },
	{ 9,	_T("úI") },
	{ 10,	_T("úJ") },
	{ 11,	_T("úK") },
	{ 12,	_T("úL") },
	{ 13,	_T("úM") },
	{ 14,	_T("úN") },
	{ 15,	_T("úO") },
	{ 16,	_T("úP") },
	{ 17,	_T("úQ") },
	{ 18,	_T("úR") },
	{ 19,	_T("úS") },
	{ 20,	_T("úT") },
	{ 21,	_T("úU") },
	{ 22,	_T("úV") },
	{ 23,	_T("úW") },
	{ 24,	_T("úX") },
	{ 25,	_T("úY") },
	{ 26,	_T("úZ") },
	{ 27,	_T("ûA") },
	{ 28,	_T("ûB") },
	{ 29,	_T("ûC") },
	{ 30,	_T("ûD") },
	{ 31,	_T("ûE") },
	{ '!',	_T("üA") },
	{ '"',	_T("üB") },
	{ '#',	_T("üC") },
	{ '$',	_T("üD") },
	{ '%',	_T("üE") },
	{ '&',	_T("üF") },
	{ 39, 	_T("üG") },
	{ '(',	_T("üH") },
	{ ')',	_T("üI") },
	{ '*',	_T("üJ") },
	{ '+',	_T("üK") },
	{ ',',	_T("üL") },
	{ '/',	_T("üO") },
	{ ':',	_T("üZ") },
	{ ';',	_T("ûF") },
	{ '<',	_T("ûG") },
	{ '=',	_T("ûH") },
	{ '>',	_T("ûI") },
	{ '?',	_T("ûJ") },
	{ '@',	_T("ûV") },
	{ '[',	_T("ûK") },
	{ '\\',	_T("ûL") },
	{ ']',	_T("ûM") },
	{ '^',	_T("ûN") },
	{ '_',	_T("ûO") },
	{ 96, 	_T("ûW") },
	{ 'a',	_T("ýA") },
	{ 'b',	_T("ýB") },
	{ 'c',	_T("ýC") },
	{ 'd',	_T("ýD") },
	{ 'e',	_T("ýE") },
	{ 'f',	_T("ýF") },
	{ 'g',	_T("ýG") },
	{ 'h',	_T("ýH") },
	{ 'i',	_T("ýI") },
	{ 'j',	_T("ýJ") },
	{ 'k',	_T("ýK") },
	{ 'l',	_T("ýL") },
	{ 'm',	_T("ýM") },
	{ 'n',	_T("ýN") },
	{ 'o',	_T("ýO") },
	{ 'p',	_T("ýP") },
	{ 'q',	_T("ýQ") },
	{ 'r',	_T("ýR") },
	{ 's',	_T("ýS") },
	{ 't',	_T("ýT") },
	{ 'u',	_T("ýU") },
	{ 'v',	_T("ýV") },
	{ 'w',	_T("ýW") },
	{ 'x',	_T("ýX") },
	{ 'y',	_T("ýY") },
	{ 'z',	_T("ýZ") },
	{ '{',	_T("ûP") },
	{ '|',	_T("ûQ") },
	{ '}',	_T("ûR") },
	{ '~',	_T("ûS") },
	{ 127,	_T("ûT") },
	-1, NULL
};

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////
PATTERNS a93[] = {
	{ '0', _T("131111") },
	{ '1', _T("131111") },
	{ '2', _T("111212") },
	{ '3', _T("111410") },
	{ '4', _T("111221") },
	{ '5', _T("121211") },
	{ '6', _T("121310") },
	{ '7', _T("111113") },
	{ '8', _T("131210") },
	{ '9', _T("141110") },
	{ 'A', _T("211112") },
	{ 'B', _T("211211") },
	{ 'C', _T("211310") },
	{ 'D', _T("221111") },
	{ 'E', _T("221210") },
	{ 'F', _T("231110") },
	{ 'G', _T("112112") },
	{ 'H', _T("112211") },
	{ 'I', _T("112310") },
	{ 'J', _T("122111") },
	{ 'K', _T("132110") },
	{ 'L', _T("111122") },
	{ 'M', _T("111221") },
	{ 'N', _T("111320") },
	{ 'O', _T("121121") },
	{ 'P', _T("131120") },
	{ 'Q', _T("212111") },
	{ 'R', _T("212210") },
	{ 'S', _T("211121") },
	{ 'T', _T("211220") },
	{ 'U', _T("221120") },
	{ 'V', _T("222110") },
	{ 'W', _T("112121") },
	{ 'X', _T("112220") },
	{ 'Y', _T("122120") },
	{ 'Z', _T("123110") },
	{ '-', _T("121130") },
	{ '.', _T("311111") },
	{ ' ', _T("311210") },
	{ '$', _T("321110") },
	{ '/', _T("112130") },
	{ '+', _T("113120") },
	{ '%', _T("211130") },
	{ 250, _T("121220") },
	{ 251, _T("312110") },
	{ 252, _T("311120") },
	{ 253, _T("111140") },
	{ START, _T("111140") },
	{ STOP,  _T("1111411") },
	{  -1, NULL }
};


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
_TCHAR szStart[] = { START, 0 };
_TCHAR szStop[] = { STOP, 0 };
_TCHAR sz3[] = { 3, 0 };

struct {
	INT iStyle;
	INT fSpecial;
	INT nScale;
	PATTERNS far * pPat;
	MAPS far * pMap;
	long (*pfnCheckSumInit)( INT nLength );
	long (*pfnCheckSum)( long cs, INT iChar, INT iCount );
} aStyles[] =
{
	{ 1,  FALSE, 1, a2of5,		NULL,	NULL,			  NULL },
	{ 2,  TRUE,	 1, aI2of5,		NULL,	NULL,			  NULL },
	{ 3,  FALSE, 1, a3of9,		NULL,	NULL,			  NULL },
	{ 4,  FALSE, 1, aCodabar,	NULL,	NULL,			  NULL },
	{ 5,  FALSE, 1, a3of9,		a3of9e,	NULL,			  NULL },
	{ 6,  FALSE, 1, aCode128A,	NULL,	CheckSum128AInit, CheckSum128 },
	{ 7,  FALSE, 1, aCode128B,	NULL,	CheckSum128BInit, CheckSum128 },
	{ 8,  TRUE,	 1, aCode128C,	NULL,	CheckSum128CInit, CheckSum128 },
	{ 9,  TRUE,	 1, aUPCOdd,	NULL,	NULL,			  NULL },
	{ 10, FALSE, 1, aMSI,		NULL,	CheckSumMSIInit,  CheckSumMSI },
	{ 11, FALSE, 1, a93,		NULL,	CheckSum93Init,	  CheckSum93 },
	{ 12, FALSE, 1, a93,		a93e,	CheckSum93Init,	  CheckSum93 },
	{ 13,  TRUE, 1, aEANA,		NULL,	NULL,			  NULL },
	{ 14,  TRUE, 1, aEANA,		NULL,	NULL,			  NULL },
	{ 15,  TRUE, 1, aPostNet,	NULL,	CheckSumPNInit,	  CheckSumPostNet },
	{ 16, FALSE, 1, aANSI3of9,	NULL,	NULL,			  NULL },
	{ 17, FALSE, 1, aANSI3of9,	a3of9e,	NULL,			  NULL },
	{ 18,  TRUE, 1, aCode128A,	NULL,	CheckSum128AInit, CheckSum128 },
	{ 19,  TRUE, 1, aCode128A,	NULL,	CheckSum128AInit, CheckSum128 },
	{ 20,  TRUE, 1, aUPCEOdd,	NULL,	NULL,			  NULL },
	{ 21,  TRUE, 1, NULL,		NULL,	NULL,			  NULL },
	{ 22, FALSE, 1, aMSI,		NULL,	CheckSumMSIInit,  CheckSumMSI },
	{ 23,  TRUE, 1, aI2of5,		NULL,	NULL,			  NULL },
	{ -1, FALSE, 0, NULL,		NULL,	NULL,			  NULL }
};

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
CBarcodCtrl::CBarcodCtrl()
{
	m_enumAlignment = 0;	// left
    m_enumStyle = 3;		// Code 3 of 9
	m_enumDirection = 0;	// Left to right
	m_iBarWidth = 0;		// Auto size
	m_enumUPCNotches = 3;	// Both
}

CBarcodCtrl::~CBarcodCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBarcodCtrl::Draw - Drawing function
INT CBarcodCtrl::IsCheckSum( INT iStyle )
{
	return ( aStyles[ iStyle ].pfnCheckSum && aStyles[ iStyle ].pfnCheckSumInit );
}

long CBarcodCtrl::CheckSumInit( INT iStyle, INT nLength )
{
	if ( !IsCheckSum( iStyle ))
		return 0;
		
	return ( aStyles[ iStyle ].pfnCheckSumInit( nLength ) );
}

long CBarcodCtrl::CheckSum( INT iStyle, long cs, INT iChar, INT iCount )
{
	if ( !IsCheckSum( iStyle ))
		return 0;
		
	return ( aStyles[ iStyle ].pfnCheckSum( cs, iChar, iCount ) );
}

VOID CBarcodCtrl::DrawBar( CDC * pdc, CRect& rcBounds, CBrush * brush, INT x, INT nWidth )
{
	CRect rect;

	rect = rcBounds;

	switch ( m_enumDirection )
	{
		case 0:
			rect.left = rcBounds.left + x;
			rect.right = rcBounds.left + x + nWidth;
			break;

		case 1:
			rect.left = rcBounds.right - x - nWidth - 1;
			rect.right = rcBounds.right - x - 1;
			break;

		case 2:
			rect.top = rcBounds.top + x;
			rect.bottom = rcBounds.top + x + nWidth;
			break;

		case 3:
			rect.top = rcBounds.bottom - x - nWidth - 1;
			rect.bottom = rcBounds.bottom - x - 1;
			break;
	}

	pdc->FillRect( &rect, brush );
}

INT CBarcodCtrl::PositionBar( RECT FAR * rcBounds, INT nBarcodeWidth )
{
	INT nWidth;


	switch ( m_enumDirection )
	{
		case 0:
		case 1:
			nWidth = rcBounds->right - rcBounds->left;
			break;
			
		case 2:
		case 3:
			nWidth = rcBounds->bottom - rcBounds->top;
			break;
	}
	
	switch( m_enumAlignment )
	{
		case 0: // left
			return 0;
			
		case 1: // right
			return nWidth - nBarcodeWidth;
			
		case 2: // center
			return ( nWidth - nBarcodeWidth ) / 2;
	}

	return 0;
}


INT CBarcodCtrl::FindChar( PATTERNS far * apat, INT c )
{
	INT i;


	if ( !apat )
		return -1;
		
	for ( i = 0; apat[ i ].c != -1; i++ )
		if ( apat[ i ].c == c )
			return i;

	return -1;
}

INT CBarcodCtrl::DrawCharStr( CRect& rcBounds, PATTERNS far * apat, CDC * pdc, CBrush * brush, INT x,
				 INT nBar, INT nScale, INT c )
{
	INT ic;
	INT i;
	INT nWidth;
	INT fBar;
	INT nTotalWidth;


	if ( ( ic = FindChar( apat, c ) ) == -1 )
		return 0;

	for ( nTotalWidth = 0, fBar = 1, i = 0; apat[ ic ].szPattern[ i ]; i++, fBar = !fBar )
	{
		nWidth = ( apat[ ic ].szPattern[ i ] - '0' ) * nBar;

		if ( fBar )
			DrawBar( pdc, rcBounds, brush, x + nTotalWidth, nWidth );

		nTotalWidth += nWidth;
	}

	return nTotalWidth;
}


INT CBarcodCtrl::LenCharStr( PATTERNS far * apat, INT c )
{
	INT ic;
	INT i;
	INT nTotalWidth;


	if (( ic = FindChar( apat, c ) ) == -1 )
		return 0;

	for ( nTotalWidth = 0, i = 0; apat[ ic ].szPattern[ i ]; i++ )
		nTotalWidth += apat[ ic ].szPattern[ i ] - '0';

	return nTotalWidth;
}

LPCTSTR CBarcodCtrl::MapChar( MAPS far * pMap, LPCTSTR lpstr )
{
	INT c;
	INT i;

	if ( !pMap )
		return NULL;

	c = *lpstr;

#if 0
	if (( pMap == a3of9e ) && (( c == '$' ) || ( c == '/' ) || ( c == '+' ) || ( c == '%' )))
	{
		if (( lpstr[1] == 0 ) || (( lpstr[1] >= '0' ) && ( lpstr[1] <= '9' )))
			return NULL;

		if (( lpstr[1] == '$' ) || ( lpstr[1] == '/' ) || ( lpstr[1] == '+' ) 
				|| ( lpstr[1] == '%' ) || ( lpstr[1] == ' ' ))
			return NULL;

	}
#endif

	for ( i = 0; pMap[ i ].szMap; i++ )
		if ( pMap[ i ].c == c )
			return (TCHAR *)pMap[ i ].szMap;

	return NULL;
}

INT CBarcodCtrl::DrawChar( CRect& rcBounds, PATTERNS far * apat, MAPS far * pMap, CDC * pdc, 
	CBrush * brush, INT x, INT nBar, INT nScale, LPCTSTR lpstr )
{
	INT nLen;
	INT nLenNew;
	LPCTSTR lpstrMap;


	lpstrMap = MapChar( pMap, lpstr );


	if ( !lpstrMap )
	{
		INT c;

		c = *lpstr;

		nLen = DrawCharStr( rcBounds, apat, pdc, brush, x, nBar, nScale, c );

		if ( nLen )
			nLen += nBar;
	}
	else
	{
		nLen = 0;

		while ( *lpstrMap )
		{
			nLenNew = DrawCharStr( rcBounds, apat, pdc, brush, x + nLen, nBar, nScale, *lpstrMap );

			if ( nLenNew )
				nLen += nLenNew + nBar;

			lpstrMap += 1;
		}
	}

	return nLen;
}

INT CBarcodCtrl::LenChar( PATTERNS far * apat, MAPS far * pMap, INT nScale, LPCTSTR lpstr )
{
	INT nLen;
	INT nLenNew;
	LPCTSTR lpstrMap;


	lpstrMap = MapChar( pMap, lpstr );

	if ( !lpstrMap )
	{
		INT c;

		c = *lpstr;

		nLen = LenCharStr( apat, c );

		if ( nLen )
			nLen += 1;
	}
	else
	{
		nLen = 0;

		while ( *lpstrMap )
		{
			nLenNew = LenCharStr( apat, *lpstrMap );

			if ( nLenNew )
				nLen += nLenNew + 1;

			lpstrMap += 1;
		}
	}

	return ( nLen / nScale );
}

// get length of character (in symbols, include maps)
INT CBarcodCtrl::SymbolCountChar( PATTERNS far * apat, MAPS far * pMap, LPCTSTR lpstr )
{
	INT nLen;
	LPCTSTR lpstrMap;


	lpstrMap = MapChar( pMap, lpstr );

	if ( !lpstrMap )
	{
		INT c;

		c = *lpstr;

		nLen = ( FindChar( apat, c ) ? 1 : 0 );
	}
	else
	{
		for ( nLen = 0; *lpstrMap; lpstrMap += 1 )
			nLen += ( FindChar( apat, *lpstrMap ) ? 1 : 0 );
    }
    
	return nLen;
}

// get length of string, in symbols
INT CBarcodCtrl::SymbolCount( PATTERNS far * apat, MAPS far * pMap, CString str )
{
	INT nLen;
	INT iIndex;
	
	for ( nLen = 0, iIndex = 0; iIndex < str.GetLength(); iIndex += 1 )
		nLen += SymbolCountChar( apat, pMap, (TCHAR *)str.GetBuffer( 0 ) + iIndex );
		
	return nLen;
}


INT CBarcodCtrl::BarWidth( INT nWidth, INT nBoxWidth )
{
	INT nBar;


	if ( m_enumStyle == 15 )
		return 1;

	if ( m_iBarWidth >= 1 )
	{
		if ( nWidth * m_iBarWidth <= nBoxWidth )
			nBar = m_iBarWidth;
		else
			nBar = 0;
	}
	else
	{
		if ( nWidth == 0 )
			nBar = 4;
		else if ( nBoxWidth < nWidth )
			nBar = 0;
		else
			nBar = nBoxWidth / nWidth;
	}

	return nBar;
}


INT CBarcodCtrl::BarCodeWidth( INT iStyle, CString str )
{
	INT i;
	INT n;
	INT nWidth;
	INT nSymbolCount;
	long cs;


	if ( !aStyles[ iStyle ].fSpecial )
	{
		nWidth = LenChar( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, aStyles[ iStyle ].nScale, (TCHAR *)szStart );
		// get number of characters in string 
		nSymbolCount = SymbolCount( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, str );

		cs = CheckSumInit( iStyle, nSymbolCount );
			
		for ( i = 0; i < str.GetLength(); i++ )
		{
			n = LenChar( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, aStyles[ iStyle ].nScale, 
				(TCHAR *)str.GetBuffer(0) + i );
			
			if ( n > 1 )
			{
				cs = CheckSum( iStyle, cs, FindChar( aStyles[ iStyle ].pPat, str[ i ] ), i + 1 );
				nWidth += n;
			}
		}

		if ( IsCheckSum( iStyle ))
		{
			_TCHAR sz[2];

			sz[0] = aStyles[ iStyle ].pPat[ cs & 0xFF ].c;
			sz[1] = 0;

			nWidth += LenChar( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, 
				aStyles[ iStyle ].nScale, (TCHAR *)sz );
			
			// if this is code 93, add the second checksum character
			if (( aStyles[ iStyle ].iStyle == 11 ) || ( aStyles[ iStyle ].iStyle == 12 ))
			{
				cs = CheckSum( iStyle, cs, (INT)( cs & 0xFF ), i + 1 );

				sz[0] = aStyles[ iStyle ].pPat[ (cs>>8) & 0xFF ].c;
				sz[1] = 0;

				nWidth += LenChar( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, 
					aStyles[ iStyle ].nScale, (TCHAR *)sz);
			}
			// if this is a 2-digit MSI Code checksum
			else if ( aStyles[ iStyle ].iStyle == 22 )
			{
				// all characters have the same width in this style
				nWidth += LenChar( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, 
					aStyles[ iStyle ].nScale, (TCHAR *)sz );
			}
        }
        
		nWidth += LenChar( aStyles[ iStyle ].pPat, aStyles[ iStyle ].pMap, 
			aStyles[ iStyle ].nScale, (TCHAR *)szStop );
	}
	else
	{
		switch ( aStyles[ iStyle ].iStyle )
		{
			case 2: // Interleaved 2 of 5
				nWidth = 9 + ( ( str.GetLength() + 1 ) / 2 ) * 18;
				break;

			case 8: // code 128C
				for ( n = 0, i = 0; i < str.GetLength(); i++ )
					if ( isdigit( str[ i ] ))
						n += 1;
						
				nWidth = (( n + 1 ) / 2 ) * 11 + 36;
				break;
				
			case 9: // UPC-A
			case 13: // EAN-13
				nWidth = ( 7 * 12 ) + 11;
				break;
				
			case 14: // EAN-8
				nWidth = ( 7 * 8 ) + 11; // 8 characters plus guards
				break;
				
			case 15: // PostNet
				nWidth = ( str.GetLength() * 5 ) + 2;
				break;

			case 18: // Code 128 (auto)
				nWidth = BarWidth128( str, FALSE );
				break;

			case 19: // UCC/EAN-128
				nWidth = BarWidth128( str, TRUE );
				break;

			case 20: // UPC-E
				nWidth = ( 6 * 7 ) + 9; // 6 characters plus guards
				break;
				
			case 21: // Royal Mail RM4SCC
				nWidth = ( str.GetLength() * 8 ) + 3;
				break;

			case 23: // DUN-14
				nWidth = 9 + ( 7 * 18 );
				break;
		}
	}

	return nWidth;
}

VOID CBarcodCtrl::UPCRectShrink( CRect& rc )
{
	switch ( m_enumDirection )
	{
		case 0:
			if ( m_enumUPCNotches & 1 )
				rc.top += ( rc.bottom - rc.top ) / 10;
									
			if ( m_enumUPCNotches & 2 )
				rc.bottom -= ( rc.bottom - rc.top ) / 10;
			break;
								
		case 1:
			if ( m_enumUPCNotches & 2 )
				rc.top += ( rc.bottom - rc.top ) / 10;
									
			if ( m_enumUPCNotches & 1 )
				rc.bottom -= ( rc.bottom - rc.top ) / 10;
			break;
								
		case 2:
			if ( m_enumUPCNotches & 1 )
				rc.right -= ( rc.right - rc.left ) / 10;
									
			if ( m_enumUPCNotches & 2 )
				rc.left += ( rc.right - rc.left ) / 10;
			break;
								
		case 3:
			if ( m_enumUPCNotches & 2 )
				rc.right -= ( rc.right - rc.left ) / 10;
									
			if ( m_enumUPCNotches & 1 )
				rc.left += ( rc.right - rc.left ) / 10;
			break;
	}
						
}

INT CBarcodCtrl::NumericOnly( CString * str )
{
	INT i, j;
	
	
	// strip all non-numerics
	for ( i = 0, j = 0; i < str->GetLength(); i++ )
		if ( isdigit( (*str)[i] ))
			str->SetAt( j++, (*str)[i] );
	
	*str = str->Left( j );
	
	return j;
}

INT CBarcodCtrl::AlphaNumericOnly( CString * str )
{
	INT i, j;
	
	
	// strip all non-numerics
	for ( i = 0, j = 0; i < str->GetLength(); i++ )
		if ((( (*str)[i] >= '0' ) && ( (*str)[i] <= '9' )) || (( (*str)[i] >= 'A' ) && ( (*str)[i] <= 'Z' )))
			str->SetAt( j++, (*str)[i] );
	
	*str = str->Left( j );
	
	return j;
}

VOID CBarcodCtrl::UPCChecksum( CString& str, INT nLength, INT iSeed )
{
	INT i, j;
	
	// compute checksum
	
	for ( i = 0, j = iSeed; i < nLength; i++ )
		j += ( str[ i ] - '0' ) * (( i & 1 ) ? 1 : 3 );
	
	// add check digit
	
	if ( str.GetLength() == nLength )
		str = str + CString(_T(" "));

	str.SetAt( nLength, (_TCHAR)((( 10 - ( j % 10 )) % 10 ) + '0' ));
}

VOID CBarcodCtrl::PaintUPC( CDC * pdc, CBrush * brush, INT nLength, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint, LPPATTERNS pPat, LPMAPS pMap, LPPATTERNS pEven, LPPATTERNS pOdd )
{
	INT i, ic;
	CRect rectShrink;
	
	NumericOnly( &str );
	
	// check for proper length
	if (( str.GetLength() != nLength ) && ( str.GetLength() != nLength + 1 ))
	{
		str.Empty();
		return;
	}
	
	// compute checksum and add check digit
	UPCChecksum( str, nLength, 0 );
	
	// draw start bar
	ic = PositionBar( lprPaint, nWidth * nBar );
	ic += DrawChar( lprPaint, pPat, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)szStart );
	ic -= nBar;
	
	// create smaller rectangle for inner bars
	rectShrink = lprPaint;
	UPCRectShrink( rectShrink );
						
	// paint UPC bars
						
	for ( i = 0; i < nLength + 1; i++ )
	{
		if ( i < ( nLength + 1 ) / 2 )
			ic += DrawChar( rectShrink, pOdd, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)str.GetBuffer(0) + i );
		else
		{
			if ( i == ( nLength + 1 ) / 2 )
			{
				ic += DrawChar( lprPaint, pEven, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)sz3 );
				ic -= nBar;
			}
								
			ic += DrawChar( rectShrink, pEven, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)str.GetBuffer(0) + i );
		}
								
		ic -= nBar;
	}
	
	// draw end bar
						
	DrawChar( lprPaint, pPat, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)szStop );
}

VOID CBarcodCtrl::PaintEAN13( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint, LPPATTERNS pPat, LPMAPS pMap )
{
	INT i, ic;
	CRect rectShrink;
	
	NumericOnly( &str );
	
	// check for proper length
	if (( str.GetLength() != 12 ) && ( str.GetLength() != 13 ))
	{
		str.Empty();
		return;
	}
	
	// compute checksum and add check digit, parity digit is handled differently
    CString strPass;

	strPass = str.Mid(1);
	UPCChecksum( strPass, 11, str[0] - '0' );
	str = str.Left(1) + strPass;
	
	// draw start bar
	ic = PositionBar( lprPaint, nWidth * nBar );
	ic += DrawChar( lprPaint, pPat, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)szStart );
	ic -= nBar;
	
	// create smaller rectangle for inner bars
	rectShrink = lprPaint;
	UPCRectShrink( rectShrink );
						
	// paint UPC bars
						
	for ( i = 1; i <= 12; i++ )
	{
		if ( i <= 6 )
		{
			if ( abEANParity[ str[0] - '0' ][ i-1 ] == 'A' )
				ic += DrawChar( rectShrink, aEANA, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)str.GetBuffer(0) + i );
			else
				ic += DrawChar( rectShrink, aEANB, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)str.GetBuffer(0) + i );
		}
		else
		{
			if ( i == 7 )
			{
				ic += DrawChar( lprPaint, pPat, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)sz3 );
				ic -= nBar;
			}
								
			ic += DrawChar( rectShrink, aEANC, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)str.GetBuffer(0) + i );
		}
								
		ic -= nBar;
	}
	
	// draw end bar
						
	DrawChar( lprPaint, pPat, pMap, pdc, brush, ic, nBar, 1, (TCHAR *)szStop );
}

INT CBarcodCtrl::DigitCount( CString str )
{
	INT i;

	for ( i = 0; ( i < str.GetLength()) && isdigit( str[i] ); i++ );

	return i;
}

#define LCC_NONE		0
#define LCC_LOWERCASE	1
#define LCC_CONTROL		2

INT CBarcodCtrl::NextLCorCtrl( CString str )
{
	INT i;

	for ( i = 0; i < str.GetLength(); i++ )
	{
		if ( str[i] < 32 )
			return LCC_CONTROL;
		else if (( str[i] >= 'a' ) && ( str[i] <= 'z' ))
			return LCC_LOWERCASE;
	}

	return LCC_NONE;
}

INT CBarcodCtrl::InStr( CString str, INT iType )
{
	INT i;

	for ( i = 0; i < str.GetLength(); i++ )
	{
		if (( str[i] < 32 ) && ( iType == LCC_CONTROL ))
			return i;
		else if ((( str[i] >= 'a' ) && ( str[i] <= 'z' )) && ( iType == LCC_LOWERCASE ))
			return i;
	}

	return -1;
}

INT CBarcodCtrl::BarWidth128( CString strCaption, BOOL fUCCEAN128 )
{
	CString strResult;
	CString strDisplayed;

	Generate128Auto( strCaption, strResult, strDisplayed, fUCCEAN128 );

	return (( strResult.GetLength() * 11 ) + 2 );
}

VOID CBarcodCtrl::Generate128Auto( CString& strCaption, CString& strResult, CString& strDisplayed, BOOL fUCCEAN128 )
{
	INT i,iPos;
	long iCheckSum;
	INT index; // offset in caption
	INT iStyle; // 128 style (A, B or C)
	LPPATTERNS pPat;

	strResult = _T("");
	strDisplayed = _T("");
	iPos = InStr( strCaption, LCC_LOWERCASE );	// find 1st lowercase _TCHAR

	// determine starting style
	// If two digits, or four or more ... use Code 128C
	if ( DigitCount( strCaption ) >= 4 )
	{
		iStyle = 'C';
		pPat = aCode128C;
	}
	else if (( iPos >= 0 ) && ( InStr( strCaption, LCC_CONTROL ) < iPos ))
	{
		iStyle = 'A';
		pPat = aCode128A;
	}
	else
	{
		iStyle = 'B';
		pPat = aCode128B;
	}

	// insert the start character
	strResult = CString( (TCHAR)FindChar( pPat, START ));

	// if we're generating a UCC/EAN-128 barcode, add the FNC1
	// character after the START character
	if ( fUCCEAN128 )
		strResult += CString( (TCHAR)FindChar( pPat, FNC1 ));

	// scan through Caption string and translate into
	// Code 128 codes
	index = 0;
	while ( index < strCaption.GetLength() )
	{
		INT iCount;
		// added the following check on 10/6/98
		// because the FNC3 character was being dropped on
		// odd length programming codes.

		// is the next character a control character?
		if ( InStr( strCaption[index], LCC_CONTROL ) >= 0 )
		{
			if (iStyle=='C')	// Code C requires a SHIFT
				strResult+=CString((TCHAR)FindChar(pPat,SHIFT));	// so add a shift
			strResult+=CString((TCHAR)FindChar(pPat,strCaption[index]));	// add the _TCHAR as-is
			index+=1;	// move to next character
		}

		// get the total number of characters to follow
		iCount = DigitCount( strCaption.Mid( index ));

		// if it's more efficient to use Code 128C, use it (1,2,3)
		if ((( iCount >= 2 ) && ( iStyle == 'C' )) || ( iCount == 4 ) || ( iCount >= 6 ))
		{
			INT iNum;
			// if we're not in 128C, switch to it
			if ( iStyle != 'C' )
			{
				// if an odd number of numbers, jump to
				// Code 128C
				if ( iCount & 1 )
				{
					strResult += CString( (TCHAR)FindChar( pPat, strCaption[ index ] ));
					index += 1;
				}

				strResult += CString( (TCHAR)FindChar( pPat, CODEC ));
			}

			pPat = aCode128C;
			iStyle = 'C';

			// get code to show via 128C
			iNum = 10 * ( strCaption[ index ] - '0' ) + 
				( strCaption[ index+1 ] - '0' );

			strDisplayed += strCaption.Mid( index, 2 );
			// add character to barcode string
			strResult += CString( (TCHAR)FindChar( pPat, iNum ));

			// move to next characters to show
			index += 2;
		}
		else if (( iStyle == 'B' ) && ( strCaption[ index ] < 32 )) // (4)
		{
			// if we have a control character while under Code 128B,
			// check to see if it's more efficient to just use a
			// SHIFT or to move over to Code 128A
			if ( NextLCorCtrl( strCaption.Mid( index )) == LCC_LOWERCASE )
				// more efficient to use the SHIFT
				strResult += CString( (TCHAR)FindChar( pPat, SHIFT ));
			else
			{
				strResult += CString( (TCHAR)FindChar( pPat, CODEA ));
				iStyle = 'A';
				pPat = aCode128A;
			}

			strDisplayed += strCaption.Mid( index, 1 );

			// add character to barcode string
			strResult += CString( (TCHAR)FindChar( pPat, strCaption[ index ] ));
			index += 1;
		}
		else if (( iStyle == 'A' ) && 
			(( strCaption[ index ] >= 'a' ) && ( strCaption[ index ] <= 'z' ))) // (5)
		{
			// if we have a lower case character while under Code 128A,
			// check to see if it's more efficient to just use a
			// SHIFT or to move over to Code 128B
			if ( NextLCorCtrl( strCaption.Mid( index )) == LCC_CONTROL )
				// more efficient to use the SHIFT
				strResult += CString( (TCHAR)FindChar( pPat, SHIFT ));
			else
			{
				strResult += CString( (TCHAR)FindChar( pPat, CODEB ));
				iStyle = 'B';
				pPat = aCode128B;
			}

			strDisplayed += strCaption.Mid( index, 1 );

			// add character to barcode string
			strResult += CString( (TCHAR)FindChar( pPat, strCaption[ index ] ));
			index += 1;
		}
		else if ( iStyle == 'C' ) // (6)
		{
			// we're in Code 128C, but not enough digits
			// to keep going with 128C.  so we are going to
			// switch to the most efficient code (A or B)
			//if ( NextLCorCtrl( strCaption.Mid( index )) == LCC_LOWERCASE )


			iPos = InStr( strCaption.Mid(index),LCC_LOWERCASE);	// any lowercase chars left?
			if (( iPos >= 0 ) && ( InStr(strCaption.Mid(index),LCC_CONTROL) < iPos ))
			{
				strResult += CString( (TCHAR)FindChar( pPat, CODEA ));
				iStyle = 'A';
				pPat = aCode128A;
			}
			else		
			{
				strResult += CString( (TCHAR)FindChar( pPat, CODEB ));
				iStyle = 'B';
				pPat = aCode128B;
			}
			strDisplayed += strCaption.Mid( index, 1 );
			strResult += CString( (TCHAR)FindChar( pPat, strCaption[ index ] ));
			index += 1;
		}
		else
		{
			strDisplayed += strCaption.Mid( index, 1 );

			// nothing special to worry about, just add the character
			strResult += CString( (TCHAR)FindChar( pPat, strCaption[ index ] ));
			index += 1;
		}
	}

	for ( i = 1, iCheckSum = strResult[0]; i < strResult.GetLength(); i++ )
		iCheckSum = ((long)iCheckSum + ((long)i * (long)strResult[i])) % 103;

	strResult += CString( (TCHAR)( iCheckSum % 103 ));
	strResult += CString( (TCHAR)FindChar( pPat, STOP ));
}


VOID CBarcodCtrl::PaintI2of5( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint )
{
	INT ic;
	INT j;
	INT i;
	INT k;

	ic = PositionBar( lprPaint, nWidth * nBar );
	ic += DrawChar( lprPaint, aI2of5, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)szStart );
		
	j = NumericOnly( &str );

	if ( str.GetLength() & 1 )
		str = _T("0") + str;

	for ( i = 0; i < str.GetLength(); i += 2 )
	{
		lstrcpy( aI2of5[ 2 ].szPattern, _T("1111111111") );
		j = FindChar( a2of5, str[ i ] );

		for ( k = 0; a2of5[ j ].szPattern[ k ]; k++ )
			if ( !( k & 1 ) )
				aI2of5[ 2 ].szPattern[ k ] = a2of5[ j ].szPattern[ k ];

		j = FindChar( a2of5, str[ i + 1 ] );

		for ( k = 0; a2of5[ j ].szPattern[ k ]; k++ )
			if ( !( k & 1 ) )
				aI2of5[ 2 ].szPattern[ k + 1 ] = a2of5[ j ].szPattern[ k ];

		ic += DrawChar( lprPaint, aI2of5, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)sz3 );
		ic -= nBar;
	}

	DrawChar( lprPaint, aI2of5, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)szStop );
}

VOID CBarcodCtrl::PaintDUN14( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint )
{
	CString strOut;
	CString strTemp;

	strOut = str;

	switch ( strOut.GetLength() )
	{
		case 11: // UPC w/o checksum
			strOut = strOut.Left(1) + _T("0") + strOut.Mid(1);
		case 12: // EAN-13 w/o checksum
			strTemp = strOut.Mid(1);
			UPCChecksum( strTemp, 11, strOut[0] - '0' );
			strOut = strOut.Left(1) + strTemp;
			break;

		case 13: // do nothing
			break;

		case 14: // strip checksum digit and re-compute
			strOut = strOut.Left(13);
			break;

		default: // invalid length, paint nothing
			return;
	}

	// add checksum digit
	UPCChecksum( strOut, 13, 0 );

	PaintI2of5( pdc, brush, nWidth, nBar, strOut, lprPaint );
}

VOID CBarcodCtrl::Paint128C( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint )
{
	INT ic;
	long cs;
	INT i;
	INT j;
	_TCHAR sz[ 2 ];

	ic = PositionBar( lprPaint, nWidth * nBar );
	ic += DrawChar( lprPaint, aCode128C, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)szStart );
	
	cs = CheckSumInit( 7, SymbolCount( aCode128C, NULL, str ));
	
	j = NumericOnly( &str );
			
	if ( str.GetLength() & 1 )
		str = _T("0") + str;
		
	for ( i = 0; i < str.GetLength(); i += 2 )
	{
		// get character based on value
		j = (( str[ i ] - '0' ) * 10 ) + ( str[ i+1 ] - '0' );

		sz[0] = j;
		sz[1] = 0;

		ic += DrawChar( lprPaint, aCode128C, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)sz );
		
		cs = CheckSum( 7, cs, j, ( i / 2 ) + 1 );
	}
	
	ic += DrawCharStr( lprPaint, aCode128C, pdc, brush, ic, nBar, 1, aCode128C[ cs ].c );
	ic += nBar;
		
	DrawChar( lprPaint, aCode128C, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)szStop );
}

VOID CBarcodCtrl::Paint128Auto( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint, BOOL fUCCEAN128 )
{
	CString strResult;
	CString strDisplayed;
	INT ic;
	INT i;


	Generate128Auto( str, strResult, strDisplayed, fUCCEAN128 );

	// draw start bar
	ic = PositionBar( lprPaint, nWidth * nBar );

	for ( i = 0; i < strResult.GetLength(); i++ )
		ic += nBar + DrawCharStr( lprPaint, aCode128A, pdc, brush, 
			ic, nBar, 1, aCode128A[ (_TUCHAR)strResult[i]].c );
}

VOID CBarcodCtrl::PaintUPCE( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint )
{
	CString strUPC;
	INT iNumberSystem;
	INT iChecksum;
	INT ic;
	INT i;


	// get rid of non-numeric characters
	NumericOnly( &str );
	strUPC = str;

	// convert UPC to 10 digits, make sure str is
	// only 6 digits
	switch ( strUPC.GetLength() )
	{
		case 6: // convert from zero-suppression back to full UPC
			iNumberSystem = 0;

			switch( strUPC[5] )
			{
				case '0':
				case '1':
				case '2':
					strUPC = strUPC.Left( 2 ) + strUPC.Mid(5,1) + _T("0000") + strUPC.Mid(2,3);
					break;

				case '3':
					strUPC = strUPC.Left( 3 ) + _T("00000") + strUPC.Mid( 3, 2 );
					break;

				case '4':
					strUPC = strUPC.Left( 4 ) + _T("00000") + strUPC.Mid( 4, 1 );
					break;

				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					strUPC = strUPC.Left( 5 ) + _T("0000") + strUPC.Mid( 5, 1 );
					break;
			}
			break;

		case 10:
			iNumberSystem = 0;
			break;

		case 11:
		case 12: // strip check digit and numbering _tsystem from UPC
			iNumberSystem = strUPC[ 0 ] - '0';
			strUPC = strUPC.Mid( 1, 10 );
			break;
	}

	if ( strUPC.GetLength() != 10 )
	{
		str = _T("");
		return;
	}

	str = _T("");

	// create zero-suppression UPC code (UPC-E) from full UPC
	if (( strUPC.Mid( 2, 3 ) == _T("000") ) || ( strUPC.Mid( 2, 3 ) == _T("100") ) || ( strUPC.Mid( 2, 3 ) == _T("200") ))
	{
		// these manufacturers may have ID#s running from 000 to 999
		if ( strUPC.Mid( 5, 2 ) == _T("00") )
			str = strUPC.Left( 2 ) + strUPC.Mid( 7, 3 ) + strUPC.Mid( 2, 1 );
	}
	else if ( strUPC.Mid( 3, 2 ) == _T("00") )
	{
		// these manufacturers may have ID#s running from 00 to 99
		if ( strUPC.Mid( 5, 3 ) == _T("000") )
			str = strUPC.Left( 3 ) + strUPC.Mid( 8, 2 ) + _T("3");
	}
	else if ( strUPC[4] == '0' )
	{
		// these manufacturers may have ID#s running from 0 to 9
		if ( strUPC.Mid( 5, 4 ) == _T("0000") )
			str = strUPC.Left( 4 ) + strUPC.Mid( 9, 1 ) + _T("4");
	}
	else if (( strUPC[9] >= '5' ) && ( strUPC[9] <= '9' ) && ( strUPC.Mid( 5, 4 ) == _T("0000") ))
		str = strUPC.Left( 5 ) + strUPC.Mid( 9, 1 );

	// if we could not create a valid zero-suppression code (UPC-E) from
	// the full UPC-A code ... do not paint
	if ( str.IsEmpty() )
		return;
	else if ( str.GetLength() < 6 )
	{
		// sanity check ... should never reach here
		str = _T("");
		return;
	}

	iChecksum = iNumberSystem + strUPC[ 1 ] + strUPC[ 3 ] + strUPC[ 5 ] + strUPC[ 7 ] + strUPC[ 9 ];
	iChecksum *= 3;
	iChecksum += strUPC[ 0 ] + strUPC[ 2 ] + strUPC[ 4 ] + strUPC[ 6 ] + strUPC[ 8 ];
	iChecksum = ( 10 - ( iChecksum % 10 )) % 10;

	// draw start bar
	ic = PositionBar( lprPaint, nWidth * nBar );
	ic += DrawChar( lprPaint, aUPCEEven, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)szStart );

	for ( i = 0; i < 6; i++ )
	{
		if ( aUPCEPattern[ iChecksum ][ i ] == 'E' )
			ic += nBar + DrawCharStr( lprPaint, aUPCEEven, pdc, brush, ic, nBar, 1, str[ i ] );
		else
			ic += nBar + DrawCharStr( lprPaint, aUPCEOdd, pdc, brush, ic, nBar, 1, str[ i ] );
	}

	ic += DrawChar( lprPaint, aUPCEEven, NULL, pdc, brush, ic, nBar, 1, (TCHAR *)szStop );
}

VOID CBarcodCtrl::PaintPostNet( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint )
{
	long nxLogical, nyLogical;
	long nGap;
	CRect rcPaintTall, rcPaintShort;
	long cs;
	INT i, j;
	INT ic;
	
	// get pixels per inch
	nxLogical = pdc->GetDeviceCaps( LOGPIXELSX );
	nyLogical = pdc->GetDeviceCaps( LOGPIXELSY );
	
	// adjust for better painting 
	if ( nxLogical < 75 )
		nxLogical = 50;
	else if ( nxLogical < 100 )
		nxLogical = 100;
	
	// adjust for better painting 
	if ( nyLogical < 75 )
		nyLogical = 50;
	else if ( nyLogical < 100 )
		nyLogical = 100;
	
	// set up painting size based on direction
	switch ( m_enumDirection )
	{
		case 0: // left to right
		case 1: // right to left
			nBar = (INT)( nxLogical / 50L );
			nGap = (( nxLogical + 11 ) / 22L );
			break;
			
		case 2: // top to bottom
		case 3: // bottom to top
			nBar = (INT)( nyLogical / 50L );
			nGap = (( nyLogical + 11 ) / 22L );
			break;
	}
	
	if ( !nBar )
		nBar = 1;
		
	if ( !nGap )
		nGap = 1;
		
	// get rid of non-numeric characters
	j = NumericOnly( &str );
	
	// position initial barcode
	ic = PositionBar( lprPaint, (INT)(( nGap * ( 6 + ( j * 5 ))) + nBar ));
	
	// set up painting size based on direction
	switch ( m_enumDirection )
	{
		case 0: // left to right
			rcPaintTall = lprPaint;
			rcPaintTall.bottom = (INT)( rcPaintTall.top + ( nyLogical / 8L ));
			rcPaintShort = rcPaintTall;
			rcPaintShort.top += (INT)(( nyLogical * 75L ) / 1000L );
			break;
	
		case 1: // right to left
			rcPaintTall = lprPaint;
			rcPaintTall.bottom = (INT)( rcPaintTall.top + ( nyLogical / 8 ));
			rcPaintShort = rcPaintTall;
			rcPaintShort.bottom = (INT)( rcPaintShort.top + ( nyLogical / 20L ));
			break;
			
		case 2: // top to bottom
			rcPaintTall = lprPaint;
			rcPaintTall.right = (INT)( rcPaintTall.left + ( nxLogical / 8 ));
			rcPaintShort = rcPaintTall;
			rcPaintShort.right = (INT)( rcPaintShort.left + ( nxLogical / 20L ));
			break;
			
		case 3: // bottom to top
			rcPaintTall = lprPaint;
			rcPaintTall.right = (INT)( rcPaintTall.left + ( nxLogical / 8 ));
			rcPaintShort = rcPaintTall;
			rcPaintShort.left += (INT)(( nxLogical * 75L ) / 1000L );
			break;
	}
	
	// initialize checksum
	cs = CheckSumInit( 14, j );

	// draw start bar
	DrawBar( pdc, rcPaintTall, brush, ic, nBar );
	ic += (INT)nGap;
	
	// scan through zip code and draw
	for ( i = 0; i <= str.GetLength(); i++ )
	{
		INT iCharIndex;
		
		if ( i == str.GetLength() )
			iCharIndex = (INT)cs;
		else
			iCharIndex = FindChar( aPostNet, str[i] );
	
		if ( iCharIndex == -1 )
			continue;
	
		for ( j = 0; aPostNet[ iCharIndex ].szPattern[ j ]; j++  )
		{
			if ( aPostNet[ iCharIndex ].szPattern[ j ] == '0' )
				DrawBar( pdc, rcPaintShort, brush, ic, nBar );
			else
				DrawBar( pdc, rcPaintTall, brush, ic, nBar );

			ic += (INT)nGap;
		}
			
		if ( i != str.GetLength() )
			cs = CheckSum( 14, cs, str[ i ] - '0', i );
	}
	
	// draw end bar							
	DrawBar( pdc, rcPaintTall, brush, ic, nBar );
}

VOID CBarcodCtrl::PaintRoyalMail( CDC * pdc, CBrush * brush, INT nWidth, INT nBar, 
	CString& str, CRect& lprPaint )
{
	long nGap;
	long nxLogical, nyLogical;
	INT iCSUpper;
	INT iCSLower;
	CRect rcPaintUpper;
	CRect rcPaintLower;
	CRect rcPaintBoth;
	CRect rcPaintTrack;
	INT i, j;
	INT ic;
	
	// get pixels per inch
	nxLogical = pdc->GetDeviceCaps( LOGPIXELSX );
	nyLogical = pdc->GetDeviceCaps( LOGPIXELSY );
	
	// adjust for better painting 
	if ( nxLogical < 75 )
		nxLogical = 50;
	else if ( nxLogical < 100 )
		nxLogical = 100;
	
	// adjust for better painting 
	if ( nyLogical < 75 )
		nyLogical = 50;
	else if ( nyLogical < 100 )
		nyLogical = 100;
	
	// set up painting size based on direction
	switch ( m_enumDirection )
	{
		case 0: // left to right
		case 1: // right to left
			nBar = (INT)( nxLogical / 50L );
			nGap = (( nxLogical + 11 ) / 22L );
			break;
			
		case 2: // top to bottom
		case 3: // bottom to top
			nBar = (INT)( nyLogical / 50L );
			nGap = (( nyLogical + 11 ) / 22L );
			break;
	}
	
	if ( !nBar )
		nBar = 1;
		
	if ( !nGap )
		nGap = 1;
		
	// get rid of non-numeric characters
	str.MakeUpper();
	j = AlphaNumericOnly( &str );
	
	// position initial barcode
	ic = PositionBar( lprPaint, (INT)(( nGap * ( 6 + ( j * 5 ))) + nBar ));
	
	// set up painting size based on direction
	switch ( m_enumDirection )
	{
		case 0: // left to right
			rcPaintUpper = lprPaint;
			rcPaintUpper.bottom = (INT)( rcPaintUpper.top + (( 67L * nyLogical ) / 540L ));

			rcPaintLower = lprPaint;
			rcPaintLower.top += (INT)(( 40L * nyLogical ) / 540L );
			rcPaintLower.bottom = (INT)( rcPaintLower.top + (( 67L * nyLogical ) / 540L ));

			rcPaintBoth = lprPaint;
			rcPaintBoth.bottom = (INT)( rcPaintBoth.top + (( 107L * nyLogical ) / 540L ));

			rcPaintTrack = lprPaint;
			rcPaintTrack.top += (INT)(( 40L * nyLogical ) / 540L );
			rcPaintTrack.bottom = (INT)( rcPaintTrack.top + (( 27L * nyLogical ) / 540L ));
			break;
	
		case 1: // right to left
			rcPaintUpper = lprPaint;
			rcPaintUpper.top = (INT)( rcPaintUpper.bottom - (( 67L * nyLogical ) / 540L ));

			rcPaintLower = lprPaint;
			rcPaintLower.bottom -= (INT)(( 40L * nyLogical ) / 540L );
			rcPaintLower.top = (INT)( rcPaintLower.bottom - (( 67L * nyLogical ) / 540L ));

			rcPaintBoth = lprPaint;
			rcPaintBoth.top = (INT)( rcPaintBoth.bottom - (( 107L * nyLogical ) / 540L ));

			rcPaintTrack = lprPaint;
			rcPaintTrack.bottom -= (INT)(( 40L * nyLogical ) / 540L );
			rcPaintTrack.top = (INT)( rcPaintTrack.bottom - (( 27L * nyLogical ) / 540L ));
			break;
			
		case 2: // top to bottom
			rcPaintUpper = lprPaint;
			rcPaintUpper.left += (INT)(( 40L * nxLogical ) / 540L );
			rcPaintUpper.right = (INT)( rcPaintUpper.left + (( 67L * nxLogical ) / 540L ));

			rcPaintLower = lprPaint;
			rcPaintLower.right = (INT)( rcPaintLower.left + (( 67L * nxLogical ) / 540L ));

			rcPaintBoth = lprPaint;
			rcPaintBoth.right = (INT)( rcPaintBoth.left + (( 107L * nxLogical ) / 540L ));

			rcPaintTrack = lprPaint;
			rcPaintTrack.left += (INT)(( 40L * nxLogical ) / 540L );
			rcPaintTrack.right = (INT)( rcPaintTrack.left + (( 27L * nxLogical ) / 540L ));
			break;
			
		case 3: // bottom to top
			rcPaintUpper = lprPaint;
			rcPaintUpper.right -= (INT)(( 40L * nxLogical ) / 540L );
			rcPaintUpper.left = (INT)( rcPaintUpper.right - (( 67L * nxLogical ) / 540L ));

			rcPaintLower = lprPaint;
			rcPaintLower.left = (INT)( rcPaintLower.right - (( 67L * nxLogical ) / 540L ));

			rcPaintBoth = lprPaint;
			rcPaintBoth.left = (INT)( rcPaintBoth.right - (( 107L * nxLogical ) / 540L ));

			rcPaintTrack = lprPaint;
			rcPaintTrack.right -= (INT)(( 40L * nxLogical ) / 540L );
			rcPaintTrack.left = (INT)( rcPaintTrack.right - (( 27L * nxLogical ) / 540L ));
			break;
	}

	// draw start bar
	DrawBar( pdc, rcPaintUpper, brush, ic, nBar );
	ic += (INT)nGap;
	
	iCSUpper = 0;
	iCSLower = 0;

	// scan through zip code and draw
	for ( i = 0; i <= str.GetLength(); i++ )
	{
		INT iCharIndex;
		INT iUpperValue;
		INT iLowerValue;
		INT fUpper;
		INT fLower;

		if ( i == str.GetLength() )
		{
			iUpperValue = (( iCSUpper + 5 ) % 6 ) + 1;
			iLowerValue = (( iCSLower + 5 ) % 6 ) + 1;
		}
		else
		{
			if ( isdigit( str[i] ))
				iCharIndex = str[i] - '0';
			else
				iCharIndex = str[i] - 'A' + 10;

			iUpperValue = ( iCharIndex / 6 ) + 1;
			iLowerValue = ( iCharIndex % 6 ) + 1;

			iCSUpper += iUpperValue;
			iCSLower += iLowerValue;
		}

		for ( j = 0; j < 4; j++  )
		{
			fUpper = ( iUpperValue & ( 4 >> j )) || ((( iUpperValue == 4 ) || ( iUpperValue == 2 ) || ( iUpperValue == 1 )) && ( j == 3 ));
			fLower = ( iLowerValue & ( 4 >> j )) || ((( iLowerValue == 4 ) || ( iLowerValue == 2 ) || ( iLowerValue == 1 )) && ( j == 3 ));

			if ( fUpper && fLower )
				DrawBar( pdc, rcPaintBoth, brush, ic, nBar );
			else if ( fUpper )
				DrawBar( pdc, rcPaintUpper, brush, ic, nBar );
			else if ( fLower )
				DrawBar( pdc, rcPaintLower, brush, ic, nBar );
			else
				DrawBar( pdc, rcPaintTrack, brush, ic, nBar );

			ic += (INT)nGap;
		}
	}
	
	// draw end bar							
	DrawBar( pdc, rcPaintBoth, brush, ic, nBar );
}

VOID CBarcodCtrl::Draw( CDC * pdc, CRect rcBounds )
{
	INT fNoFit;
	INT i, j;
	INT iStyle;
	INT ic;
	long cs;
	INT nBar;
	INT nBoxWidth;
	INT nWidth;
	INT nSymbolCount;
	PATTERNS far * pPat;
	COLORREF clrBack, clrFore;                                               	
	MAPS far * pMap;

	CDC dcOut;
	CBrush * brFore;
	CBrush * brBack;
	CString strCaption;
	CBitmap bmp;
	CBitmap * bmpOld;
	CRect rect;
	CRect rectPaint;

	fNoFit = 0;
	strCaption.Empty();

	strCaption = m_strCaption;
	
	rect = rcBounds;
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;

	rectPaint = rect;
	
	dcOut.CreateCompatibleDC( pdc );
	if ( dcOut.m_hDC )
		bmp.CreateCompatibleBitmap( pdc, rect.right - rect.left, rect.bottom - rect.top );
	else
	{
		dcOut.CreateCompatibleDC( NULL );
		bmp.CreateCompatibleBitmap( pdc, rect.right - rect.left, rect.bottom - rect.top );
	}

	bmpOld = dcOut.SelectObject( &bmp );

	// get new colors
	// WSO clrFore = TranslateColor( GetForeColor() );
	// WSO clrBack = TranslateColor( GetBackColor() );

	clrFore = RGB( 0, 0, 0 ); // WSO
	clrBack = RGB( 255, 255, 255 ); // WSO

	// make brushes
	brFore = new CBrush( clrFore );
	brBack = new CBrush( clrBack );
	
	// fill background
	dcOut.FillRect( &rect, brBack );

	if ( m_enumStyle )
	{
		// WSO strCaption = InternalGetText();
		
		if ( ( m_enumDirection == 0 ) || ( m_enumDirection == 1 ) )
			nBoxWidth = rect.right - rect.left;
		else
			nBoxWidth = rect.bottom - rect.top;

		if ( nBoxWidth <= 0 )
			nBoxWidth = 1;

		for ( iStyle = 0; aStyles[ iStyle ].iStyle != -1; iStyle++ )
			if ( m_enumStyle == aStyles[ iStyle ].iStyle )
				break;

		if ( aStyles[ iStyle ].iStyle != -1 )
		{
			pPat = aStyles[ iStyle ].pPat;
			pMap = aStyles[ iStyle ].pMap;
			
			// get number of characters in string 
			nSymbolCount = SymbolCount( pPat, pMap, strCaption );
	
			nWidth = BarCodeWidth( iStyle, strCaption );
			nBar = BarWidth( nWidth, nBoxWidth );
			
			if ( nBar == 0 )
				fNoFit = 1;
			else if ( aStyles[ iStyle ].iStyle == -1 )
				strCaption = _T("");
			else if ( !aStyles[ iStyle ].fSpecial )
			{
				cs = CheckSumInit( iStyle, nSymbolCount );
				
				ic = PositionBar( rectPaint, nWidth * nBar );
				ic += DrawChar( rectPaint, pPat, pMap, &dcOut, brFore, ic, nBar, aStyles[ iStyle ].nScale, (TCHAR *)szStart );
	
				for ( j = 0, i = 0; i < strCaption.GetLength(); i++ )
				{
					nWidth = DrawChar( rectPaint, pPat, pMap, &dcOut, brFore, ic, nBar, 
						aStyles[ iStyle ].nScale, (TCHAR *)strCaption.GetBuffer(0) + i );
	
					if ( nWidth )
					{
						cs = CheckSum( iStyle, cs, FindChar( pPat, strCaption[ i ] ), j + 1 );
						
						strCaption.SetAt( j++, strCaption[ i ] );
						ic += nWidth;
					}
				}
	
				strCaption = strCaption.Left( j );
				
				if ( IsCheckSum( iStyle ))
				{
					ic += DrawCharStr( rectPaint, pPat, &dcOut, brFore, ic, nBar, 
						aStyles[ iStyle ].nScale, pPat[ cs & 0xFF ].c );

					ic += nBar;

					// if this is code 93, add the second checksum character
					if (( aStyles[ iStyle ].iStyle == 11 ) || ( aStyles[ iStyle ].iStyle == 12 ))
					{
						cs = CheckSum( iStyle, cs, (INT)( cs & 0xFF ), i + 1 );

						ic += DrawCharStr( rectPaint, pPat, &dcOut, brFore, ic, nBar, 
							aStyles[ iStyle ].nScale, pPat[ (cs>>8) & 0xFF ].c );

						ic += nBar;
					}
					else if ( aStyles[ iStyle ].iStyle == 22 )
					{
						cs = CheckSum( iStyle, cs, cs, j + 1 );

						ic += DrawCharStr( rectPaint, pPat, &dcOut, brFore, ic, nBar, 
							aStyles[ iStyle ].nScale, pPat[ cs & 0xFF ].c );

						ic += nBar;
					}
				}
									
				DrawChar( rectPaint, pPat, pMap, &dcOut, brFore, ic, nBar, 
					aStyles[ iStyle ].nScale, (TCHAR *)szStop );
			}
			else
				switch ( m_enumStyle )
				{
					case 2:				   // Interleaved 2 of 5
						PaintI2of5( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint );
						break;
	
					case 8: // Code 128-C
						Paint128C( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint );
						break;
	
					case 9:		// UPC-A
						PaintUPC( &dcOut, brFore, 11, nWidth, nBar, strCaption, rectPaint, pPat, pMap, aUPCEven, aUPCOdd );
						break;
							                
					case 13:	// EAN-13
						PaintEAN13( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint, pPat, pMap );
						break;
							                
					case 14:	// EAN-8
						PaintUPC( &dcOut, brFore, 7, nWidth, nBar, strCaption, rectPaint, pPat, pMap, aEANC, aEANA );
						break;
						
					case 15:	// PostNet
						PaintPostNet( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint );
						break;

					case 18: // Code 128 (auto)
						Paint128Auto( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint, FALSE );
						break;

					case 19: // UCC/EAN-128
						Paint128Auto( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint, TRUE );
						break;

					case 20: // UPC-E
						PaintUPCE( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint );
						break;
						
					case 21: // Royal Mail RM4SCC
						PaintRoyalMail( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint );
						break;

					case 23: // DUN-14
						PaintDUN14( &dcOut, brFore, nWidth, nBar, strCaption, rectPaint );
						break;
				}
		}
	}

	if ( fNoFit )
	{
		strCaption.Empty();
		dcOut.FillRect( &rect, brFore );
	}

	// if we're at design-time, paint the control name in the center
/*
	if ( AmbientUserMode() == FALSE )
	{
		CString strName = AmbientDisplayName();
		
		if ( !strName.IsEmpty() )
		{
			COLORREF clrForeOld, clrBackOld;
			
			clrForeOld = dcOut.SetTextColor( clrFore );
			clrBackOld = dcOut.SetBkColor( clrBack );
			
			dcOut.DrawText( strName, strName.GetLength(), &rect, 
				DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX );
				
			dcOut.SetTextColor( clrForeOld );
			dcOut.SetBkColor( clrBackOld );
		}
	}
*/
	m_strDisplayed = strCaption;

	pdc->BitBlt( rcBounds.left, rcBounds.top, rcBounds.right - rcBounds.left, 
		rcBounds.bottom - rcBounds.top, &dcOut, 0, 0, SRCCOPY );

	dcOut.SelectObject( bmpOld );
	bmp.DeleteObject();
	dcOut.DeleteDC();


	delete brFore;
	delete brBack;
}

VOID CBarcodCtrl::DICopy( CDC* pdcDest, CDC* pdcSrc, const CRect* lpRect )
{
	HANDLE hmemBmp;
	BITMAP bmp;			
	long i, iColors;
	HANDLE hmemBMI, hmemPE = NULL;
	LPPALETTEENTRY lpPE;			
	LPBITMAPINFO lpbmi;
	LPVOID lpBits;
	DWORD dwBitmapSize;
	INT xDest, yDest, cxDest, cyDest;
	CBitmap cNullBitmap;
	CBitmap *pBitmap;	
	

	cNullBitmap.CreateBitmap( 1, 1, 1, 1, NULL );	
	pBitmap = pdcSrc->SelectObject( &cNullBitmap );
	pdcSrc->SelectObject( pBitmap );
	cNullBitmap.DeleteObject();
						
	if (( pBitmap->GetObject(sizeof(BITMAP), &bmp )) > 0)
	{
		iColors = 1 << (bmp.bmBitsPixel * bmp.bmPlanes);
		
		if ( iColors != 256 )
			hmemBMI = GlobalAlloc( GMEM_MOVEABLE, 
				sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * iColors );
		else
		{
			hmemBMI = GlobalAlloc( GMEM_MOVEABLE, 
				sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * iColors );
			hmemPE = GlobalAlloc( GMEM_MOVEABLE, 
				sizeof(PALETTEENTRY) * iColors ); 
		}
												    
		if ( hmemBMI )
		{
			xDest = lpRect->left;
			yDest = lpRect->top;
			cxDest = lpRect->right - lpRect->left;
			cyDest = lpRect->bottom - lpRect->top;
			
			lpbmi = (LPBITMAPINFO) GlobalLock(hmemBMI);
							    	
			lpbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			lpbmi->bmiHeader.biWidth = bmp.bmWidth;
			lpbmi->bmiHeader.biHeight = bmp.bmHeight;
			lpbmi->bmiHeader.biPlanes = 1;
			lpbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;						
			lpbmi->bmiHeader.biCompression = BI_RGB; 
			lpbmi->bmiHeader.biSizeImage = 0;		
			lpbmi->bmiHeader.biXPelsPerMeter = 0;
			lpbmi->bmiHeader.biYPelsPerMeter = 0;
			lpbmi->bmiHeader.biClrUsed = 0;
			lpbmi->bmiHeader.biClrImportant = 0;							
			
			if ( hmemPE && ( iColors == 256 ))
			{	
				lpPE = (LPPALETTEENTRY) GlobalLock(hmemPE);							
										
				GetSystemPaletteEntries( pdcSrc->m_hDC, 0, 
					(INT) iColors, lpPE );
				
				for (i = 0; i < (INT) iColors; i++)
				{
					lpbmi->bmiColors[i].rgbRed = lpPE[i].peRed;
					lpbmi->bmiColors[i].rgbBlue = lpPE[i].peBlue;
					lpbmi->bmiColors[i].rgbGreen = lpPE[i].peGreen;								
				}							
			}

			dwBitmapSize = 
				(((DWORD) bmp.bmWidth * (DWORD) bmp.bmBitsPixel) + 31 ) / 32;
			dwBitmapSize *= 4 * (DWORD)(bmp.bmHeight);

			hmemBmp = GlobalAlloc(GMEM_MOVEABLE, dwBitmapSize);
								
			if ( hmemBmp )
			{
				lpBits = GlobalLock( hmemBmp );
				
				GetDIBits( pdcSrc->m_hDC, (HBITMAP) pBitmap->m_hObject, 
					0, bmp.bmHeight, lpBits, lpbmi, DIB_RGB_COLORS );
	
				StretchDIBits( pdcDest->m_hDC, xDest, yDest, 
					cxDest, cyDest, 0, 0, bmp.bmWidth, bmp.bmHeight,
					lpBits, lpbmi, DIB_RGB_COLORS, SRCCOPY );

				GlobalUnlock( hmemBmp );
				GlobalFree( hmemBmp );
			}

			GlobalUnlock( hmemBMI );
			GlobalUnlock( hmemPE );
		}
							
		if ( hmemBMI )
			GlobalFree( hmemBMI );
		
		if ( hmemPE )
			GlobalFree( hmemPE );
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CBarcodCtrl::OnResetState - Reset control to default state

VOID CBarcodCtrl::GetCaption( CString & String )
{
	String = m_strCaption;
}

VOID CBarcodCtrl::SetCaption( CString & String )
{
	m_strCaption = String;
}

VOID CBarcodCtrl::OnResetState()
{
	m_iBarWidth = 0;
	m_enumDirection = 0;
	m_strDisplayed = _T("");
	m_cyPrinterHeight = 0;
	m_cxPrinterLeft = 0;
	m_iPrinterScaleMode = 0;
	m_cyPrinterTop = 0;
	m_cxPrinterWidth = 0;
	m_enumStyle = 3;
	m_enumUPCNotches = 3;
	m_strExtension = _T("");
}

/////////////////////////////////////////////////////////////////////////////
// Member functions ported from VBX control
short CBarcodCtrl::GetBarWidth()
{
	return m_iBarWidth;
}

VOID CBarcodCtrl::SetBarWidth(short iBarWidth)
{
	m_iBarWidth = iBarWidth;
	// WSO InvalidateControl();

	// WSO if ( AmbientUserMode() == FALSE )
	// WSO 	BoundPropertyChanged( dispidBarWidth );
}

short CBarcodCtrl::GetDirection()
{
	return m_enumDirection;
}

VOID CBarcodCtrl::SetDirection(short enumDirection)
{
	m_enumDirection = enumDirection;
}

short CBarcodCtrl::GetStyle()
{
	return m_enumStyle;
}

VOID CBarcodCtrl::SetStyle(short enumStyle)
{
	m_enumStyle = enumStyle;
}

/////////////////////////////////////////////////////////////////////////////
// CBarcodCtrl message handlers
long CheckSumPostNet( long cs, INT cChar, INT iOffset )
{
	return ( 10 + cs - cChar ) % 10;
}

// init PostNet checksum
long CheckSumPNInit( INT nLength )
{
	// initial value is 0
	return 0;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
static _TCHAR szMSI[ 256 ];

// incrementally compute checksum 
long CheckSum93( long cs, INT cChar, INT iOffset )
{
	INT iCValue, iKValue, nLength;
	
	iCValue = (INT)( cs & 0xFF );
	iKValue = (INT)(( cs >> 8 ) & 0xFF );
	nLength = (INT)(( cs >> 24 ) & 0xFF );
	
	iCValue = ( iCValue + ( cChar * ((( nLength - iOffset ) % 20 ) + 1 ))) % 47;
	iKValue = ( iKValue + ( cChar * ((( nLength - iOffset + 1 ) % 15 ) + 1 ))) % 47;
	
	return ( iCValue | ( iKValue << 8 ) | ((long)nLength << 24 ));
}

// init code 93 checksum
long CheckSum93Init( INT nLength )
{
	// initial value is 0
	return ((long)nLength & 0xFF ) << 24;
}

long CheckSumMSIString( LPCTSTR sz, INT nLength )
{
	INT i;
	long cs;
	long total;
	
	for ( total = 0, i = ( nLength & 1 ); i <= nLength; i += 2 )
	{
		total *= 10;
		total += szMSI[ i ];
	}

	total *= 2;
	cs = 0;
	
	while ( total > 0 )
	{
		cs += total % 10;
		total /= 10;
	}
	
	for ( i = (( nLength + 1 ) & 1 ); i <= nLength; i += 2 )
		cs += szMSI[ i ];
		
	return ( 10 - ( cs % 10 )) % 10;
}

long CheckSumMSI( long cs, INT cChar, INT iOffset )
{
	if ( iOffset >= sizeof( szMSI ))
		return 0;
		
	szMSI[ iOffset ] = cChar;

	return CheckSumMSIString( (TCHAR *)szMSI, iOffset );

}

long CheckSumMSIInit( INT nLength )
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// incrementally compute checksum 
long CheckSum128( long cs, INT cChar, INT iOffset )
{
	return (( cs + ( cChar * iOffset )) % 103 );
}

// init c128 checksum
long CheckSum128AInit( INT nLength )
{
	// initial value is 0
	return 0;
}

long CheckSum128BInit( INT nLength )
{
	// initial value is 1
	return 1;
}

long CheckSum128CInit( INT nLength )
{
	// initial value is 2
	return 2;
}
