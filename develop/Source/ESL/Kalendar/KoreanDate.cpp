// KoreanDate.cpp: implementation of the CKoreanDate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KoreanDate.h"
#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CKoreanDate::BASE_YEAR = 1841;      // 음력 변환 가능 시작 년도
const int CKoreanDate::MAX_YEAR  = 2043;      // 음력 변환 가능 마지막 년도

// 음력 1841년 1월 1일이 양력으로 1841년 1월 23일이다
// BASE_PASSED에 1년 1월 1일부터 1841년 1월 22일 까지의 일수를 저장한다.
// 그러므로 1년 1월 1일에 672068일을 더하면 1841년 1월 23일이 된다.
const int CKoreanDate::BASE_PASSED = 672068;  // 양력, 음력 변환 시에 사용


int CKoreanDate::m_narrSolarDays[]        // 각 월의 기본 날짜 수
		= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 양력 공휴일 및 기념일
TDayInfo CKoreanDate::m_arrDayInfoSolar[] = {
	{ 1,  1, TRUE,  TYPE_SOLAR, _T("신정\0")},
	{ 3,  1, TRUE,  TYPE_SOLAR, _T("3.1절\0")},
	{ 3,  3, FALSE, TYPE_SOLAR, _T("납세자의 날\0")},
	{ 3, 19, FALSE, TYPE_SOLAR, _T("상공의 날\0")},
	{ 3, 22, FALSE, TYPE_SOLAR, _T("물의 날\0")},
	{ 4,  5, TRUE,  TYPE_SOLAR, _T("식목일\0")},
	{ 4,  7, FALSE, TYPE_SOLAR, _T("보건의 날\0")},
	{ 4, 13, FALSE, TYPE_SOLAR, _T("임시정부수립\0")},
	{ 4, 19, FALSE, TYPE_SOLAR, _T("4.19혁명\0")},
	{ 4, 20, FALSE, TYPE_SOLAR, _T("장애인의 날\0")},
	{ 4, 21, FALSE, TYPE_SOLAR, _T("과학의 날\0")},
	{ 4, 22, FALSE, TYPE_SOLAR, _T("정보통신의 날\0")},
	{ 5,  1, FALSE, TYPE_SOLAR, _T("근로자의 날\0")},
	{ 5,  5, TRUE,  TYPE_SOLAR, _T("어린이날\0")},
	{ 5,  8, FALSE, TYPE_SOLAR, _T("어버이날\0")},
	{ 5, 15, FALSE, TYPE_SOLAR, _T("스승의 날\0")},
	{ 5, 18, FALSE, TYPE_SOLAR, _T("5.18민주화운동\0")},
	{ 5, 19, FALSE, TYPE_SOLAR, _T("성년의 날\0")},
	{ 5, 25, FALSE, TYPE_SOLAR, _T("방재의 날\0")},
	{ 5, 31, FALSE, TYPE_SOLAR, _T("바다의 날\0")},
	{ 6,  5, FALSE, TYPE_SOLAR, _T("환경의 날\0")},
	{ 6,  6, TRUE,  TYPE_SOLAR, _T("현충일\0")},
	{ 6, 25, FALSE, TYPE_SOLAR, _T("6.25사변일\0")},
	{ 7, 17, TRUE,  TYPE_SOLAR, _T("제헌절\0")},
	{ 8, 15, TRUE,  TYPE_SOLAR, _T("광복절\0")},
	{ 9, 18, FALSE, TYPE_SOLAR, _T("철도의 날\0")},
	{10,  1, FALSE, TYPE_SOLAR, _T("국군의 날\0")},
	{10,  2, FALSE, TYPE_SOLAR, _T("노인의 날\0")},
	{10,  3, TRUE,  TYPE_SOLAR, _T("개천절\0")},
	{10,  8, FALSE, TYPE_SOLAR, _T("재향군인의 날\0")},
	{10,  9, FALSE, TYPE_SOLAR, _T("한글날\0")},
	{10, 15, FALSE, TYPE_SOLAR, _T("체육의 날\0")},
	{10, 20, FALSE, TYPE_SOLAR, _T("문화의 날\0")},
	{10, 21, FALSE, TYPE_SOLAR, _T("경찰의 날\0")},
	{10, 24, FALSE, TYPE_SOLAR, _T("국제연합일\0")},
	{10, 28, FALSE, TYPE_SOLAR, _T("저축의 날\0")},
	{11,  3, FALSE, TYPE_SOLAR, _T("학생의 날\0")},
	{11,  9, FALSE, TYPE_SOLAR, _T("소방의 날\0")},
	{11, 11, FALSE, TYPE_SOLAR, _T("농업인의 날\0")},
	{11, 17, FALSE, TYPE_SOLAR, _T("순국선열의 날\0")},
	{11, 30, FALSE, TYPE_SOLAR, _T("무역의 날\0")},
	{12,  3, FALSE, TYPE_SOLAR, _T("소비자의 날\0")},
	{12,  5, FALSE, TYPE_SOLAR, _T("국민교육헌장선포일\0")},
	{12, 10, FALSE, TYPE_SOLAR, _T("세계인권선언기념일\0")},
	{12, 25, TRUE,  TYPE_SOLAR, _T("성탄절\0")},
};

// 음력 공휴일 및 세시풍속일
// 24절기는 날짜가 정해져 있지 않은 것 같다.
TDayInfo CKoreanDate::m_arrDayInfoLunar[] = {
	{ 1,  1, TRUE,  TYPE_LUNAR, _T("설날\0")},
	{ 1,  2, TRUE,  TYPE_LUNAR, _T("\0")},               // 설날 다음날
	{ 1, 15, FALSE, TYPE_LUNAR, _T("정월대보름\0")},
	{ 3, 27, FALSE, TYPE_LUNAR, _T("충무공탄신일\0")},
	{ 4,  8, TRUE,  TYPE_LUNAR, _T("석가탄신일\0")},
	{ 5,  5, FALSE, TYPE_LUNAR, _T("단오\0")},
	{ 7,  7, FALSE, TYPE_LUNAR, _T("칠석\0")},
	{ 8, 14, TRUE,  TYPE_LUNAR, _T("\0")},               // 추석 전날
	{ 8, 15, TRUE,  TYPE_LUNAR, _T("추석\0")},
	{ 8, 16, TRUE,  TYPE_LUNAR, _T("\0")},               // 추석 다음날
	{12,  0, TRUE,  TYPE_LUNAR, _T("\0")},               // 설날 전날. 말일이므로 날짜를 알수 없다.
};

/*  아래의 음력 데이터는
    
	1 -> 29일짜리 달을 말함
	2 -> 30일짜리 달을 말함
	3 -> 29일짜리 달이고 그 뒤 29일짜리 윤달이 있음을 말함
	4 -> 29일짜리 달이고 그 뒤 30일짜리 윤달이 있음을 말함
	5 -> 30일짜리 달이고 그 뒤 29일짜리 윤달이 있음을 말함
	6 -> 30일짜리 달이고 그 뒤 30일짜리 윤달이 있음을 말함

	이렇게 하면 1년의 데이터가 모두 12개이면 된다.
	또한 어느 달이 윤달인지도 쉽게 알 수 있다.  */		
TCHAR CKoreanDate::m_szLunarTable[203][12] = {
	/* 1841 */      1, 2, 4, 1, 1, 2,    1, 2, 1, 2, 2, 1,
					2, 2, 1, 2, 1, 1,    2, 1, 2, 1, 2, 1,
					2, 2, 2, 1, 2, 1,    4, 1, 2, 1, 2, 1,
					2, 2, 1, 2, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 2, 1,    2, 1, 2, 1, 2, 1,
					2, 1, 2, 1, 5, 2,    1, 2, 2, 1, 2, 1,
					2, 1, 1, 2, 1, 2,    1, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 2, 3, 2, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 1, 2,    1, 1, 2, 2, 1, 2,
	/* 1851 */      2, 2, 1, 2, 1, 1,    2, 1, 2, 1, 5, 2,
					2, 1, 2, 2, 1, 1,    2, 1, 2, 1, 1, 2,
					2, 1, 2, 2, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 1, 2,    5, 2, 1, 2, 1, 2,
					1, 1, 2, 1, 2, 2,    1, 2, 2, 1, 2, 1,
					2, 1, 1, 2, 1, 2,    1, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 5, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 1, 2, 1,    1, 2, 2, 1, 2, 2,
					2, 1, 2, 1, 1, 2,    1, 1, 2, 1, 2, 2,
					2, 1, 6, 1, 1, 2,    1, 1, 2, 1, 2, 2,
	/* 1861 */      1, 2, 2, 1, 2, 1,    2, 1, 2, 1, 1, 2,
					2, 1, 2, 1, 2, 2,    1, 2, 2, 3, 1, 2,
					1, 2, 2, 1, 2, 1,    2, 2, 1, 2, 1, 2,
					1, 1, 2, 1, 2, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 1, 2, 4, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 1, 2, 1, 1,    2, 2, 1, 2, 2, 2,
					1, 2, 1, 1, 2, 1,    1, 2, 1, 2, 2, 2,
					1, 2, 2, 3, 2, 1,    1, 2, 1, 2, 2, 1,
					2, 2, 2, 1, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 2, 2, 1, 2, 1,    2, 1, 1, 5, 2, 1,
	/* 1871 */      2, 2, 1, 2, 2, 1,    2, 1, 2, 1, 1, 2,
					1, 2, 1, 2, 2, 1,    2, 1, 2, 2, 1, 2,
					1, 1, 2, 1, 2, 4,    2, 1, 2, 2, 1, 2,
					1, 1, 2, 1, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 1, 2, 1, 1,    2, 1, 2, 2, 2, 1,
					2, 2, 1, 1, 5, 1,    2, 1, 2, 2, 1, 2,
					2, 2, 1, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 2, 1, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 2, 4, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 1, 2, 2, 1, 2,    2, 1, 2, 1, 1, 2,
	/* 1881 */      1, 2, 1, 2, 1, 2,    5, 2, 2, 1, 2, 1,
					1, 2, 1, 2, 1, 2,    1, 2, 2, 1, 2, 2,
					1, 1, 2, 1, 1, 2,    1, 2, 2, 2, 1, 2,
					2, 1, 1, 2, 3, 2,    1, 2, 2, 1, 2, 2,
					2, 1, 1, 2, 1, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 2, 1, 5, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 1, 2, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 1, 2, 2, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 5, 2, 1, 2, 2,    1, 2, 1, 2, 1, 2,
	/* 1891 */      1, 2, 1, 2, 1, 2,    1, 2, 2, 1, 2, 2,
					1, 1, 2, 1, 1, 5,    2, 2, 1, 2, 2, 2,
					1, 1, 2, 1, 1, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 2, 1, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 5, 1,    2, 1, 2, 1, 2, 1,
					2, 2, 2, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					1, 2, 2, 1, 2, 1,    2, 1, 2, 1, 2, 1,
					2, 1, 5, 2, 2, 1,    2, 1, 2, 1, 2, 1,
					2, 1, 2, 1, 2, 1,    2, 2, 1, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 5, 2, 2, 1, 2,
	/* 1901 */      1, 2, 1, 1, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 2, 1, 1, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 2, 3, 2,    1, 1, 2, 2, 1, 2,
					2, 2, 1, 2, 1, 1,    2, 1, 1, 2, 2, 1,
					2, 2, 1, 2, 2, 1,    1, 2, 1, 2, 1, 2,
					1, 2, 2, 4, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 1, 2,    2, 1, 2, 1, 2, 1,
					2, 1, 1, 2, 2, 1,    2, 1, 2, 2, 1, 2,
					1, 5, 1, 2, 1, 2,    1, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 1, 2, 2, 2, 1,
	/* 1911 */      2, 1, 2, 1, 1, 5,    1, 2, 2, 1, 2, 2,
					2, 1, 2, 1, 1, 2,    1, 1, 2, 2, 1, 2,
					2, 2, 1, 2, 1, 1,    2, 1, 1, 2, 1, 2,
					2, 2, 1, 2, 5, 1,    2, 1, 2, 1, 1, 2,
					2, 1, 2, 2, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 1, 2,    2, 1, 2, 1, 2, 1,
					2, 3, 2, 1, 2, 2,    1, 2, 2, 1, 2, 1,
					2, 1, 1, 2, 1, 2,    1, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    5, 2, 2, 1, 2, 2,
					1, 2, 1, 1, 2, 1,    1, 2, 2, 1, 2, 2,
	/* 1921 */      2, 1, 2, 1, 1, 2,    1, 1, 2, 1, 2, 2,
					2, 1, 2, 2, 3, 2,    1, 1, 2, 1, 2, 2,
					1, 2, 2, 1, 2, 1,    2, 1, 2, 1, 1, 2,
					2, 1, 2, 1, 2, 2,    1, 2, 1, 2, 1, 1,
					2, 1, 2, 5, 2, 1,    2, 2, 1, 2, 1, 2,
					1, 1, 2, 1, 2, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 1, 2, 1, 2,    1, 2, 2, 1, 2, 2,
					1, 5, 1, 2, 1, 1,    2, 2, 1, 2, 2, 2,
					1, 2, 1, 1, 2, 1,    1, 2, 1, 2, 2, 2,
					1, 2, 2, 1, 1, 5,    1, 2, 1, 2, 2, 1, 
	/* 1931 */      2, 2, 2, 1, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 2, 2, 1, 2, 1,    2, 1, 1, 2, 1, 2,
					1, 2, 2, 1, 6, 1,    2, 1, 2, 1, 1, 2,
					1, 2, 1, 2, 2, 1,    2, 2, 1, 2, 1, 2,
					1, 1, 2, 1, 2, 1,    2, 2, 1, 2, 2, 1, 
					2, 1, 4, 1, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 1, 2, 1, 1,    2, 1, 2, 2, 2, 1,
					2, 2, 1, 1, 2, 1,    4, 1, 2, 2, 1, 2,
					2, 2, 1, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 2, 1, 2, 1, 2,    1, 1, 2, 1, 2, 1,
	/* 1941 */      2, 2, 1, 2, 2, 4,    1, 1, 2, 1, 2, 1,
					2, 1, 2, 2, 1, 2,    2, 1, 2, 1, 1, 2,
					1, 2, 1, 2, 1, 2,    2, 1, 2, 2, 1, 2,
					1, 1, 2, 4, 1, 2,    1, 2, 2, 1, 2, 2,
					1, 1, 2, 1, 1, 2,    1, 2, 2, 2, 1, 2, 
					2, 1, 1, 2, 1, 1,    2, 1, 2, 2, 1, 2,
					2, 5, 1, 2, 1, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 2, 1, 2, 1, 2,    3, 2, 1, 2, 1, 2,
					2, 1, 2, 2, 1, 2,    1, 1, 2, 1, 2, 1, 
	/* 1951 */      2, 1, 2, 2, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 4, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 1, 2, 2,    1, 2, 2, 1, 2, 2,
					1, 1, 2, 1, 1, 2,    1, 2, 2, 1, 2, 2,
					2, 1, 4, 1, 1, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 2, 1, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 2, 1,    1, 5, 2, 1, 2, 2,
					1, 2, 2, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					1, 2, 2, 1, 2, 1,    2, 1, 2, 1, 2, 1,
					2, 1, 2, 1, 2, 5,    2, 1, 2, 1, 2, 1, 
	/* 1961 */      2, 1, 2, 1, 2, 1,    2, 2, 1, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 2, 3, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 2, 1, 1, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 2, 1, 1,    2, 1, 1, 2, 2, 1, 
					2, 2, 5, 2, 1, 1,    2, 1, 1, 2, 2, 1,
					2, 2, 1, 2, 2, 1,    1, 2, 1, 2, 1, 2,
					1, 2, 2, 1, 2, 1,    5, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 1, 2,    2, 1, 2, 1, 2, 1,
					2, 1, 1, 2, 2, 1,    2, 1, 2, 2, 1, 2,
	/* 1971 */      1, 2, 1, 1, 5, 2,    1, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 2, 1, 1, 2,    1, 1, 2, 2, 2, 1,
					2, 2, 1, 5, 1, 2,    1, 1, 2, 2, 1, 2,
					2, 2, 1, 2, 1, 1,    2, 1, 1, 2, 1, 2,
					2, 2, 1, 2, 1, 2,    1, 5, 2, 1, 1, 2,
					2, 1, 2, 2, 1, 2,    1, 2, 1, 2, 1, 1,
					2, 2, 1, 2, 1, 2,    2, 1, 2, 1, 2, 1,
					2, 1, 1, 2, 1, 6,    1, 2, 2, 1, 2, 1,
					2, 1, 1, 2, 1, 2,    1, 2, 2, 1, 2, 2,
	/* 1981 */      1, 2, 1, 1, 2, 1,    1, 2, 2, 1, 2, 2,
					2, 1, 2, 3, 2, 1,    1, 2, 2, 1, 2, 2,
					2, 1, 2, 1, 1, 2,    1, 1, 2, 1, 2, 2,
					2, 1, 2, 2, 1, 1,    2, 1, 1, 5, 2, 2,
					1, 2, 2, 1, 2, 1,    2, 1, 1, 2, 1, 2, 
					1, 2, 2, 1, 2, 2,    1, 2, 1, 2, 1, 1,
					2, 1, 2, 2, 1, 5,    2, 2, 1, 2, 1, 2,
					1, 1, 2, 1, 2, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 1, 2, 1, 2,    1, 2, 2, 1, 2, 2,
					1, 2, 1, 1, 5, 1,    2, 1, 2, 2, 2, 2, 
	/* 1991 */      1, 2, 1, 1, 2, 1,    1, 2, 1, 2, 2, 2,
					1, 2, 2, 1, 1, 2,    1, 1, 2, 1, 2, 2,
					1, 2, 5, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 2, 2, 1, 2, 1,    2, 1, 1, 2, 1, 2,
					1, 2, 2, 1, 2, 2,    1, 5, 2, 1, 1, 2,
					1, 2, 1, 2, 2, 1,    2, 1, 2, 2, 1, 2,
					1, 1, 2, 1, 2, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 1, 2, 3, 2,    2, 1, 2, 2, 2, 1,
					2, 1, 1, 2, 1, 1,    2, 1, 2, 2, 2, 1,
					2, 2, 1, 1, 2, 1,    1, 2, 1, 2, 2, 1, 
	/* 2001 */      2, 2, 2, 3, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 2, 1, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 2, 1, 2, 2, 1,    2, 1, 1, 2, 1, 2,
					1, 5, 2, 2, 1, 2,    1, 2, 2, 1, 1, 2,
					1, 2, 1, 2, 1, 2,    2, 1, 2, 2, 1, 2, 
					1, 1, 2, 1, 2, 1,    5, 2, 2, 1, 2, 2,
					1, 1, 2, 1, 1, 2,    1, 2, 2, 2, 1, 2,
					2, 1, 1, 2, 1, 1,    2, 1, 2, 2, 1, 2,
					2, 2, 1, 1, 5, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 2, 1,    1, 2, 1, 2, 1, 2,
	/* 2011 */      2, 1, 2, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 1, 6, 2, 1, 2,    1, 1, 2, 1, 2, 1,
					2, 1, 2, 2, 1, 2,    1, 2, 1, 2, 1, 2,
					1, 2, 1, 2, 1, 2,    1, 2, 5, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 2, 2, 1, 2, 2, 
					1, 1, 2, 1, 1, 2,    1, 2, 2, 1, 2, 2,
					2, 1, 1, 2, 3, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 2, 1, 1,    2, 1, 2, 1, 2, 2,
					2, 1, 2, 1, 2, 1,    1, 2, 1, 2, 1, 2,
					2, 1, 2, 5, 2, 1,    1, 2, 1, 2, 1, 2, 
	/* 2021 */      1, 2, 2, 1, 2, 1,    2, 1, 2, 1, 2, 1,
					2, 1, 2, 1, 2, 2,    1, 2, 1, 2, 1, 2,
					1, 5, 2, 1, 2, 1,    2, 2, 1, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 2, 1, 2, 2, 1,
					2, 1, 2, 1, 1, 5,    2, 1, 2, 2, 2, 1,
					2, 1, 2, 1, 1, 2,    1, 2, 1, 2, 2, 2,
					1, 2, 1, 2, 1, 1,    2, 1, 1, 2, 2, 2,
					1, 2, 2, 1, 5, 1,    2, 1, 1, 2, 2, 1,
					2, 2, 1, 2, 2, 1,    1, 2, 1, 1, 2, 2,
					1, 2, 1, 2, 2, 1,    2, 1, 2, 1, 2, 1,
	/* 2031 */      2, 1, 5, 2, 1, 2,    2, 1, 2, 1, 2, 1,
					2, 1, 1, 2, 1, 2,    2, 1, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    5, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    2, 1, 2, 2, 2, 1,
					2, 1, 2, 1, 1, 2,    1, 1, 2, 2, 1, 2,
					2, 2, 1, 2, 1, 4,    1, 1, 2, 1, 2, 2,
					2, 2, 1, 2, 1, 1,    2, 1, 1, 2, 1, 2,
					2, 2, 1, 2, 1, 2,    1, 2, 1, 1, 2, 1,
					2, 2, 1, 2, 5, 2,    1, 2, 1, 2, 1, 1,
					2, 1, 2, 2, 1, 2,    2, 1, 2, 1, 2, 1,  
	/* 2041 */      2, 1, 1, 2, 1, 2,    2, 1, 2, 2, 1, 2,
					1, 5, 1, 2, 1, 2,    1, 2, 2, 2, 1, 2,
					1, 2, 1, 1, 2, 1,    1, 2, 2, 1, 2, 2,
};  

// 매해의 일자 수(음력)  - LunarTable로 생성한 값
int CKoreanDate::m_narrDaysOfYear[203] = {
/* 1841 */	383, 354,  384,  355,  354,  384,  355,  354,  384,  354, 
/* 1851 */	384, 354,  355,  384,  354,  355,  384,  354,  354,  384, 
/* 1861 */	354, 384,  355,  354,  384,  355,  354,  383,  354,  384, 
/* 1871 */	355, 355,  384,  354,  354,  384,  354,  354,  384,  355, 
/* 1881 */	384, 355,  354,  384,  354,  354,  384,  354,  355,  384, 
/* 1891 */	355, 384,  354,  354,  383,  355,  354,  384,  355,  384, 
/* 1901 */	354, 355,  383,  354,  355,  384,  354,  355,  384,  354, 
/* 1911 */	384, 354,  354,  384,  355,  354,  384,  355,  384,  354, 
/* 1921 */	354, 384,  354,  354,  385,  354,  355,  384,  354,  383, 
/* 1931 */	354, 355,  384,  355,  354,  384,  354,  384,  354,  354, 
/* 1941 */	384, 355,  355,  384,  354,  354,  384,  354,  384,  354, 
/* 1951 */	355, 384,  355,  354,  384,  354,  384,  354,  354,  384, 
/* 1961 */	355, 354,  384,  355,  353,  384,  355,  384,  354,  355, 
/* 1971 */	384, 354,  354,  384,  354,  384,  354,  355,  384,  355, 
/* 1981 */	354, 384,  354,  384,  354,  354,  385,  354,  355,  384, 
/* 1991 */	354, 354,  383,  355,  384,  355,  354,  384,  354,  354, 
/* 2001 */	384, 354,  355,  384,  355,  384,  354,  354,  384,  354, 
/* 2011 */	354, 384,  355,  384,  355,  354,  384,  354,  354,  384, 
/* 2021 */	354, 355,  384,  354,  384,  355,  354,  383,  355,  354, 
/* 2031 */	384, 355,  384,  354,  354,  384,  354,  354,  384,  355, 
/* 2041 */	355, 384,  354,
};

// 1841년을 시작으로 매해의 일자 수를 누적해서 저장(음력) - LunarTable로 생성한 값
long CKoreanDate::m_larrDaysAccm[203] = {
/* 1841 */	  383,   737,  1121,  1476,  1830,  2214,  2569,  2923,  3307,  3661,
/* 1851 */	 4045,  4399,  4754,  5138,  5492,  5847,  6231,  6585,  6939,  7323,
/* 1861 */	 7677,  8061,  8416,  8770,  9154,  9509,  9863, 10246, 10600, 10984,
/* 1871 */	11339, 11694, 12078, 12432, 12786, 13170, 13524, 13878, 14262, 14617,
/* 1881 */	15001, 15356, 15710, 16094, 16448, 16802, 17186, 17540, 17895, 18279,
/* 1891 */	18634, 19018, 19372, 19726, 20109, 20464, 20818, 21202, 21557, 21941,
/* 1901 */	22295, 22650, 23033, 23387, 23742, 24126, 24480, 24835, 25219, 25573,
/* 1911 */	25957, 26311, 26665, 27049, 27404, 27758, 28142, 28497, 28881, 29235,
/* 1921 */	29589, 29973, 30327, 30681, 31066, 31420, 31775, 32159, 32513, 32896,
/* 1931 */	33250, 33605, 33989, 34344, 34698, 35082, 35436, 35820, 36174, 36528,
/* 1941 */	36912, 37267, 37622, 38006, 38360, 38714, 39098, 39452, 39836, 40190,
/* 1951 */	40545, 40929, 41284, 41638, 42022, 42376, 42760, 43114, 43468, 43852,
/* 1961 */	44207, 44561, 44945, 45300, 45653, 46037, 46392, 46776, 47130, 47485,
/* 1971 */	47869, 48223, 48577, 48961, 49315, 49699, 50053, 50408, 50792, 51147,
/* 1981 */	51501, 51885, 52239, 52623, 52977, 53331, 53716, 54070, 54425, 54809,
/* 1991 */	55163, 55517, 55900, 56255, 56639, 56994, 57348, 57732, 58086, 58440,
/* 2001 */	58824, 59178, 59533, 59917, 60272, 60656, 61010, 61364, 61748, 62102,
/* 2011 */	62456, 62840, 63195, 63579, 63934, 64288, 64672, 65026, 65380, 65764,
/* 2021 */	66118, 66473, 66857, 67211, 67595, 67950, 68304, 68687, 69042, 69396,
/* 2031 */	69780, 70135, 70519, 70873, 71227, 71611, 71965, 72319, 72703, 73058,
/* 2041 */	73413, 73797, 74151,
};


CKoreanDate::CKoreanDate()
{
	SetDateAsCurrent();
}

// 지정한 일자가 올바른 일자값이 아니면 시스템 일자로 설정한다.
CKoreanDate::CKoreanDate(int nYear, int nMonth, int nDay, int nType, BOOL bIsLeapMonth)
{
	if( !SetDate(nYear, nMonth, nDay, nType, bIsLeapMonth) )
	{
		SetDateAsCurrent();
	}
}

CKoreanDate::~CKoreanDate()
{

}

#ifdef _DEBUG
// 디버그 모드에서 내용 확인
void CKoreanDate::Dump()
{
	TCHAR* szWeekdayName[] = {
		_T("일"), _T("월"), _T("화"), _T("수"), _T("목"), _T("금"), _T("토")
	};

	TRACE(_T("양력 %4d년 %02d월 %02d일 %s요일 ")  , m_nYearSolar
		                                     , m_nMonthSolar
						        		     , m_nDaySolar
								        	 , szWeekdayName[m_nDayOfWeek-1]);
	TRACE(_T("(%d / %d)주\r\n"), GetWeekNumber(), GetWeekCount());

	TRACE(_T("음력 %4d년 %02d월 %02d일 ")  , m_nYearLunar
		                                 , m_nMonthLunar
						        		 , m_nDayLunar);
	if( m_bIsLeapMonth )
		TRACE(_T("윤달\r\n"));
	else
		TRACE(_T("평달\r\n"));

}
#endif

// static public function
// 해당 년월일이 일자값인지 검사
BOOL CKoreanDate::IsDate(int nYear, int nMonth, int nDay, int nType, BOOL bIsLeapMonth)
{
	int nDaysOfMonth;

	if(nYear <= 0) return FALSE;
	if(nMonth < 1 || nMonth > 12) return FALSE;
	if(nDay < 1 || nDay > 31) return FALSE;

	nDaysOfMonth = GetDaysOfMonth(nYear, nMonth, nType, bIsLeapMonth);
	if( nDaysOfMonth == 0 ) return FALSE;
	return ( nDay <= nDaysOfMonth );
}

// static public function
// 해당 년월의 날짜 수를 구한다.
int CKoreanDate::GetDaysOfMonth(int nYear, int nMonth, int nType, BOOL bIsLeapMonth)
{
	int nDaysOfMonth = 0;
	int nLunarData;

	if( nMonth < 1 || nMonth > 12 ) return 0;

	if( nType == TYPE_SOLAR )
	{
		//if( nYear < 1 ) return 0;

		nDaysOfMonth = m_narrSolarDays[nMonth-1];        // 해당월의 기본 날짜 수
		if( nMonth == 2 )  // 2월이면 윤년인지 검사 필요
		{
			if( !(nYear%4) && nYear%100 || !(nYear%400) )  // 윤년이면
				nDaysOfMonth++;
		}
	}
	else
	{
		if( nYear < BASE_YEAR || nYear > MAX_YEAR ) return 0;
		
		nLunarData = m_szLunarTable[nYear-BASE_YEAR][nMonth-1];
		if( bIsLeapMonth && nLunarData <= 2 ) return 0;  //윤달이 없는데 윤달을 찾으면

		if( !bIsLeapMonth )  // 윤달이 아닐 경우 일자 검사
		{
			switch( nLunarData )
			{
			case 1:
			case 3:
			case 4:
				nDaysOfMonth = 29;
				break;
			default:
				nDaysOfMonth = 30;
				break;
			}
		}
		else                 // 윤달일 경우 일자 검사
		{
			switch( nLunarData )
			{
			case 3:
			case 5:
				nDaysOfMonth = 29;
				break;
			case 4:
			case 6:
				nDaysOfMonth = 30;
				break;
			}
		}  // end of if( !bIsLeapMonth )
	}

	return nDaysOfMonth;
}

int CKoreanDate::GetDaysOfMonth(int nType) const
{
	if( nType == TYPE_SOLAR )
		return GetDaysOfMonth(m_nYearSolar, m_nMonthSolar);
	else
		return GetDaysOfMonth(m_nYearLunar, m_nMonthLunar, TYPE_LUNAR, m_bIsLeapMonth);
}

// static public function
// 1년 1월 1일부터 지정 일자까지 흐른 날짜 수를 구한다.
// 정확한 일자 값을 지정하지 않으면 -1을 리턴한다.
long CKoreanDate::GetPassedDays(int nYear, int nMonth, int nDay)
{
	long	lTotal;   // 1년 1월 1일부터 설정 일자까지 흐른 총 날짜수
	int		i;
	
	if( !IsDate(nYear, nMonth, nDay) ) return -1;

	lTotal = (nYear - 1) * 365              // X년 * 365일
		   + (long)( (nYear - 1) / 4 )      // 4년마다 1일 추가
		   - (long)( (nYear - 1) / 100 )    // 100년 마다 1일 감소
		   + (long)( (nYear - 1) / 400 );   // 400년마다 1일 추가
	for(i = 1; i < nMonth; i++)
	{
		lTotal += GetDaysOfMonth(nYear, i);
	}
	lTotal += nDay;

	return lTotal;
}

// static public function
// 해당 년월의 요일을 구한다.
// 정확한 일자값을 지정하지 않으면 0을 리턴한다.
int CKoreanDate::GetDayOfWeek(int nYear, int nMonth, int nDay)
{
	return (int)(GetPassedDays(nYear, nMonth, nDay) % 7) + 1; // 1 base
}

// static public function
// 윤달이 있는 월인지 검사(음력)
BOOL CKoreanDate::HasLeapMonth(int nYear, int nMonth)
{
	if( !IsDate(nYear, nMonth, 1, TYPE_LUNAR, FALSE) ) return FALSE;
	return ( m_szLunarTable[nYear-BASE_YEAR][nMonth-1] > 2 );
}

// private function
// 양력 일자를 기준으로 음력 일자를 구한다.
BOOL CKoreanDate::ConvertSolarToLunar()
{
	long lTotal;               
	int nYear, nMonth, nDay;   // 구할 음력 일자(최종 값에 각각 베이스 값을 더해줘야 함. 일은 제외)
	BOOL bIsLeapMonth;         // 최종 구한 음력 일자가 윤달인지 여부
	BOOL bNewMonth;            // 윤달인 경우 달은 증가 시키지 않고 bNewMonth = FALSE로 설정
	int nLunarData;            // m_szLunarTable의 값

	m_nYearLunar = 0;
	m_nMonthLunar = 0;
	m_nDayLunar = 0;
	m_bIsLeapMonth = FALSE;

	if( m_nYearSolar < BASE_YEAR || m_nYearSolar > MAX_YEAR )
		return FALSE;

	lTotal = GetPassedDays(m_nYearSolar, m_nMonthSolar, m_nDaySolar) - BASE_PASSED;

	// 2043년부터 아래로 년도 추적
	// for 루프 벗어날 때 nYear + BASE_YEAR 가 음력 년도이다.
	for(nYear = MAX_YEAR - BASE_YEAR + 1; nYear > 0 ; nYear--) 
	{
		if( lTotal > m_larrDaysAccm[nYear-1] )
		{
			lTotal -= m_larrDaysAccm[nYear-1];  // 년도에 의해 결정되는 일수 빼주기
			break;
		}
	}

	// 월, 일, 윤달 여부 결정
	nMonth = 0;
	bNewMonth = TRUE;
	while( true )
	{
		nLunarData = m_szLunarTable[nYear][nMonth];
		nDay = GetDaysOfMonth(nYear+BASE_YEAR, nMonth+1, TYPE_LUNAR, !bNewMonth);
		bIsLeapMonth = ( !bNewMonth && nLunarData > 2 ) ? TRUE : FALSE;

		if( lTotal <= nDay )
		{
			nDay = lTotal;
			break;
		}
		else
		{
			lTotal -= nDay;
		}

		if( nLunarData > 2 && bNewMonth )  // 윤달이면
		{
			bNewMonth = FALSE;
		}
		else
		{
			nMonth++;
			bNewMonth = TRUE;
		}
	} // end of while

	m_nYearLunar = nYear + BASE_YEAR;
	m_nMonthLunar = nMonth + 1;
	m_nDayLunar = nDay;
	m_bIsLeapMonth = bIsLeapMonth;

	return TRUE;
}

// private function
// 음력 일자를 기준으로 양력 일자를 구한다.
BOOL CKoreanDate::ConvertLunarToSolar()
{
	long lTotal, lTemp;               
	int nYear, nMonth, nDay;
	int nLunarData;            // m_szLunarTable의 값

	nYear = m_nYearLunar-BASE_YEAR;

	// 년도까지 일수 구하기
	lTotal = BASE_PASSED;
	lTotal += (nYear == 0) ? 0 : m_larrDaysAccm[nYear-1];
	
	// (m_nMonthLunar - 1) 월까지의 일수 더하기
	for(nMonth = 0; nMonth < (m_nMonthLunar - 1); nMonth++)
	{
		nLunarData = m_szLunarTable[nYear][nMonth];
		switch(nLunarData)  // 3 이상은 윤달이 있는 달이므로 평달+윤달
		{
		case 1:
			lTotal += 29;
			break;
		case 2:
			lTotal += 30;
			break;
		case 3:
			lTotal += 58;
			break;
		case 4:
		case 5:
			lTotal += 59;
			break;
		case 6:
			lTotal += 60;
			break;
		}
	}

	// 현재 월이 윤달이면 윤달 전달을 더해준다
	if( m_bIsLeapMonth )
	{
		switch( m_szLunarTable[nYear][m_nMonthLunar - 1] )
		{
		case 3:
		case 4:
			lTotal += 29;
			break;
		case 5:
		case 6:
			lTotal += 30;
			break;
		}
	}

	// 일수 더하기
	// m_nDayLunar를 더하면 lTotal은 양력 1년 1월 1일 부터 현재까지 일수를 가진다
	lTotal += m_nDayLunar;
	                        
	// 일수를 기준으로 양력 일자를 구한다.
	nYear = (int)(lTotal / 365) + 1;
	while( true )
	{
		lTemp = GetPassedDays(nYear, 12, 31);
		
		if( lTotal == lTemp )
		{
			lTotal -= GetPassedDays(nYear-1, 12, 31);
			break;
		}
		else if( lTotal > lTemp )
		{
			nYear++;
			// 윤년이면 nYear해는 366일, 그렇지 않으면 365일이다
			nDay = ( !(nYear%4) && nYear%100 || !(nYear%400) ? 366 : 365 );

			if( (lTotal - lTemp) <= nDay )
			{
				lTotal -= GetPassedDays(nYear-1, 12, 31);
				break;
			}
		}
		else
		{
			lTemp = GetPassedDays(nYear, 1, 1);
			if( lTemp <= lTotal )
			{
				lTotal -= GetPassedDays(nYear-1, 12, 31);
				break;
			}
			else
			{
				nYear--;
			}
		}  // end of if
	}  // end of while( true )

	// 월, 일 결정
	for(nMonth = 1; nMonth <= 12; nMonth++)
	{
		nDay = GetDaysOfMonth(nYear, nMonth);
		if( lTotal <= nDay )
		{
			nDay = lTotal;
			break;
		}
		else
		{
			lTotal -= nDay;
		}
	}  // end of for

	if( IsDate(nYear, nMonth, nDay) )
	{
		m_nYearSolar = nYear;
		m_nMonthSolar = nMonth;
		m_nDaySolar = nDay;
		return TRUE;
	}

	return FALSE;
}

// public function
int CKoreanDate::GetYear(int nType) const
{
	return (nType == TYPE_SOLAR ? m_nYearSolar : m_nYearLunar);
}

// public function
int CKoreanDate::GetMonth(int nType) const
{
	return (nType == TYPE_SOLAR ? m_nMonthSolar : m_nMonthLunar);
}

// public function
int CKoreanDate::GetDay(int nType) const
{
	return (nType == TYPE_SOLAR ? m_nDaySolar : m_nDayLunar);
}

// public function
BOOL CKoreanDate::SetDay(int nDay, int nType)
{
	if( nType == TYPE_SOLAR )
	{
		if( IsDate(m_nYearSolar, m_nMonthSolar, nDay) )
		{
			SetDate(m_nYearSolar, m_nMonthSolar, nDay);
			return TRUE;
		}
	}
	else
	{
		if( IsDate(m_nYearLunar, m_nMonthLunar, nDay, TYPE_LUNAR, m_bIsLeapMonth) )
		{
			SetDate(m_nYearLunar, m_nMonthLunar, nDay, TYPE_LUNAR, m_bIsLeapMonth);
			return TRUE;
		}
	}

	return FALSE;
}

// public function
// 요일(1:일요일, 2:월요일, ...)
int CKoreanDate::GetDayOfWeek() const
{
	return m_nDayOfWeek;
}

// public function
// 윤달 여부(음력)
BOOL CKoreanDate::IsLeapMonth() const
{
	return m_bIsLeapMonth;
}

// public function
// 해당 월의 총 주의 수를 구한다.
int CKoreanDate::GetWeekCount() const
{
	int nDays;

	nDays = GetDaysOfMonth(m_nYearSolar, m_nMonthSolar) - 1;
	nDays += ( GetDayOfWeek(m_nYearSolar, m_nMonthSolar, 1) - 1 );
	return ( (int)(nDays / 7) + 1 );
}

// public function
// 해당 일이 몇 번째 주에 속하는지 구한다.
int CKoreanDate::GetWeekNumber() const
{
	int nDays;

	nDays = m_nDaySolar + GetDayOfWeek(m_nYearSolar, m_nMonthSolar, 1) - 2;
	return ( (int)(nDays / 7) + 1 );
}

// public function
// 1년 1월 1일부터 지정 일자까지 흐른 날짜 수를 구한다.
// 정확한 일자 값을 지정하지 않으면 -1을 리턴한다.
long CKoreanDate::GetPassedDays() const
{
	return GetPassedDays(m_nYearSolar, m_nMonthSolar, m_nDaySolar);
}

// public function
// 시스템 일자로 설정
void CKoreanDate::SetDateAsCurrent()
{
	struct tm atm;
	time_t now;
	
	::time(&now);
	atm = *(::localtime(&now));

	m_nYearSolar = atm.tm_year + 1900;  // 1900 base
	m_nMonthSolar = atm.tm_mon + 1;     // 0 base
	m_nDaySolar = atm.tm_mday;
	m_nDayOfWeek = GetDayOfWeek(m_nYearSolar, m_nMonthSolar, m_nDaySolar);

	ConvertSolarToLunar();
}

// public function
// 넘겨준 일자로 설정
BOOL CKoreanDate::SetDate(int nYear, int nMonth, int nDay, int nType, BOOL bIsLeapMonth)
{
	if( nYear <= 1 ) return FALSE;
	if( !IsDate(nYear, nMonth, nDay, nType, bIsLeapMonth) ) return FALSE;

	if( nType == TYPE_SOLAR )
	{
		m_nYearSolar = nYear;
		m_nMonthSolar = nMonth;
		m_nDaySolar = nDay;
		m_nDayOfWeek = GetDayOfWeek(m_nYearSolar, m_nMonthSolar, m_nDaySolar);

		ConvertSolarToLunar();
	}
	else
	{
		m_nYearLunar = nYear;
		m_nMonthLunar = nMonth;
		m_nDayLunar = nDay;
		m_bIsLeapMonth = bIsLeapMonth;

		if( ConvertLunarToSolar() )
		{
			m_nDayOfWeek = GetDayOfWeek(m_nYearSolar, m_nMonthSolar, m_nDaySolar);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

// private function
// 넘겨준 값만큼 날짜를 더하거나 뺀다. 뺄 때는 음수 값 넘겨줌
// 년도 먼저 계산하고 월, 일 순으로 계산한다.
// 단 월은 12로 나눈 나머지를 취하고 몫은 년도에 더한다.
// 일은 월 단위로 환산하지 않는다.
// 년 또는 월을 더하거나 뺀 후 일자가 없으면 그 달의 말일로 된다.
// 예) 2000.3.31 - 1달 = 2000.2.29  (2002년 2월은 윤달이다.)
// 그러므로 날짜 수를 정확히 하여 계산하고자 할 때는 년월을 0으로 하고 일자만 넘겨준다.
// 예) 현재 일자에서 50일을 더할 때       : DateAdd(0, 0, 50);
//                   1년 2개월을 더할 때  : DateAdd(1, 2, 0);
//                   3달 더하고 2일 뺄 때 : DateAdd(0, 3, -2);
BOOL CKoreanDate::DateAddSolar(int nYear, int nMonth, int nDay)
{
	int nTempYear, nTempMonth, nTempDay;
	long lTotal, lTemp;

	nTempYear = m_nYearSolar;
	nTempMonth = m_nMonthSolar;
	nTempDay = m_nDaySolar;

	nYear += nMonth / 12;
	nMonth %= 12;

	if( nYear != 0 )
	{
		nTempYear += nYear;
		if( nTempYear < 1 )	return FALSE;
	}

	if( nMonth != 0 )
	{
		nTempMonth += nMonth;
		if( nTempMonth > 12 )
		{
			nTempYear++;
			nTempMonth -= 12;
		}
		else if( nTempMonth <= 0 )
		{
			nTempYear--;
			nTempMonth += 12;
			if( nTempYear < 1 )	return FALSE;
		}
	}

	// 년월이 변경되어 날짜가 정확하지 않을 때 말일로 설정
	if( (nYear != 0 || nMonth != 0) && !IsDate(nTempYear, nTempMonth, nTempDay) )
	{
		nTempDay = GetDaysOfMonth(nTempYear, nTempMonth);
	}

	if( nDay != 0 )
	{
		lTotal = GetPassedDays(nTempYear, nTempMonth, nTempDay) + nDay;

		// 년도 결정
		// 정확히 계산할 수 없으므로 일단 1년을 365일로 가정하고 근사 년도를 구한다.
		// 근사 년도의 12월 31일까지 일수와 계산할 일수(lTotal)의 차이값으로
		// 정확한 년도를 구한다.
		// 년도를 구한 후에 lTotal 에는 (년도 - 1)년 12월 31일 까지 일수를 뺀 값을
		// 넣어주어 월, 일을 구하는데 사용한다.
		nTempYear = (int)(lTotal / 365) + 1;
		while( true )
		{
			lTemp = GetPassedDays(nTempYear, 12, 31);
			
			if( lTotal == lTemp )
			{
				lTotal -= GetPassedDays(nTempYear-1, 12, 31);
				break;
			}
			else if( lTotal > lTemp )
			{
				nTempYear++;
				// 윤년이면 nTempYear해는 366일, 그렇지 않으면 365일이다
				nTempDay = ( !(nTempYear%4) && nTempYear%100 || !(nTempYear%400) ? 366 : 365 );

				if( (lTotal - lTemp) <= nTempDay )
				{
					lTotal -= GetPassedDays(nTempYear-1, 12, 31);
					break;
				}
			}
			else
			{
				lTemp = GetPassedDays(nTempYear, 1, 1);
				if( lTemp <= lTotal )
				{
					lTotal -= GetPassedDays(nTempYear-1, 12, 31);
					break;
				}
				else
				{
					nTempYear--;
				}
			}  // end of if
		}  // end of while( true )

		// 월, 일 결정
		for(nTempMonth = 1; nTempMonth <= 12; nTempMonth++)
		{
			nTempDay = GetDaysOfMonth(nTempYear, nTempMonth);
			if( lTotal <= nTempDay )
			{
				nTempDay = lTotal;
				break;
			}
			else
			{
				lTotal -= nTempDay;
			}
		}  // end of for
	}  // end of if( nDay != 0 )

	return SetDate(nTempYear, nTempMonth, nTempDay);
}

// private function
// 음력 기준으로 일자를 더하거나 뺀다
// 음력에서 1년이 반드시 12개월은 아니다 (12, 13, 14 개월)
// 그러므로 년도없이 월을 계산하는 것은 불가능하다.
BOOL CKoreanDate::DateAddLunar(int nYear, int nMonth, int nDay)
{
	int nTempYear, nTempMonth, nTempDay;
	BOOL bIsLeapMonth;
	CKoreanDate ld(m_nYearLunar, m_nMonthLunar, m_nDayLunar, TYPE_LUNAR, m_bIsLeapMonth);

	nTempYear = m_nYearLunar + nYear;
	nTempMonth = m_nMonthLunar;
	nTempDay = m_nDayLunar;

	//--- 년도 변경 --------------------------------------------------------------
	// 단순히 년을 더해서 설정이 가능하면 년도 변경 완료된다.(윤달인 경우 윤달로)
	// 그렇지 않으면(윤달 이었는데 년도가 바뀌어 윤달이 없거나
	//               30일이었는데 년도가 바뀌어 29일이 말일인 경우)
	// 윤달을 변경해보고 그래도 바른 일자가 아니면 일-1 을 한다.
	// 일은 30에서 29로 바뀌어도 30을 nTempDay에 저장해서 월 변경에서 다시 설정하므로 상관없다.
	if( !IsDate(nTempYear, nTempMonth, nTempDay, TYPE_LUNAR, m_bIsLeapMonth) )
	{
		if( !IsDate(nTempYear, nTempMonth, nTempDay, TYPE_LUNAR, FALSE) )
		{
			if( !IsDate(nTempYear, nTempMonth, nTempDay-1, TYPE_LUNAR, FALSE) )
				return FALSE;
			if( !ld.SetDate(nTempYear, nTempMonth, nTempDay-1, TYPE_LUNAR, FALSE) )
				return FALSE;
		}
		else
		{
			ld.SetDate(nTempYear, nTempMonth, nTempDay, TYPE_LUNAR, FALSE);
		}
	}
	else
	{
		ld.SetDate(nTempYear, nTempMonth, nTempDay, TYPE_LUNAR, m_bIsLeapMonth);
	}
	//----------------------------------------------------------------------------

	//--- 월 변경 ----------------------------------------------------------------
	// 1달 더하기 : 현재월이 윤달 전달이면 윤달 플래그만 TRUE로 설정한다.
	//                       윤달이거나 평달이면 1달 더한다. 13월이면 1년 증가하고 1월로 설정한다.
	// 1달 빼기   : 현재월이 윤달이면 윤달 플래그만 FALSE로 설정한다.
	//                       윤달 전달이거나 평달이면 1달 뺀다 0월이면 1년 감소하고 12월로 설정한다.
	// 더하거나 빼는 월 수 만큼 루프 돈 다음 구한 년월일로 SetDate한다.
	if( nMonth != 0 )
	{
		bIsLeapMonth = ld.IsLeapMonth();

		while( nMonth != 0 )
		{
			if( nMonth > 0 )
			{
				if( HasLeapMonth(nTempYear, nTempMonth) && !bIsLeapMonth )  // 윤달 전달이면
				{
					bIsLeapMonth = TRUE;
				}
				else
				{
					nTempMonth++;
					if( nTempMonth > 12 )
					{
						nTempYear++;
						nTempMonth = 1;
					}

					bIsLeapMonth = FALSE;
				}
				nMonth--;
			}
			else
			{
				if( HasLeapMonth(nTempYear, nTempMonth) && bIsLeapMonth )   // 윤달이면
				{
					bIsLeapMonth = FALSE;
				}
				else
				{
					nTempMonth--;
					if( nTempMonth < 1 )
					{
						nTempYear--;
						nTempMonth = 12;
					}

					bIsLeapMonth = HasLeapMonth(nTempYear, nTempMonth);
				}
				nMonth++;
			}
		} // end of while

		if( !ld.SetDate(nTempYear, nTempMonth, nTempDay, TYPE_LUNAR, bIsLeapMonth) )
		{
			if( !ld.SetDate(nTempYear, nTempMonth, nTempDay-1, TYPE_LUNAR, bIsLeapMonth) )
				return FALSE;
		}
	} // end of if
	//----------------------------------------------------------------------------

	ld.DateAdd(0, 0, nDay);   // 일 변경 - 양력으로 변경

	return SetDate(ld.GetYear(), ld.GetMonth(), ld.GetDay());
}

// public function
BOOL CKoreanDate::DateAdd(int nYear, int nMonth, int nDay, int nType)
{
	if( nType == TYPE_SOLAR )
		return DateAddSolar(nYear, nMonth, nDay);  // 양력
	else
		return DateAddLunar(nYear, nMonth, nDay);  // 음력
}

// virtual protected function
// 출력 형식이 맘에 들지 않으면 상속 받아서
// 출력 형식을 원하는 스타일로 재정의 하면 된다.
// dwOption은 재정의 시에 여러 용도로 사용할 수 있다.
CString CKoreanDate::OnFormatDateString(int nType, DWORD dwOption)
{
	CString sDate;

	TCHAR* szWeekdayName[] = {
		_T("일"), _T("월"), _T("화"), _T("수"), _T("목"), _T("금"), _T("토")
	};

	if( nType == TYPE_SOLAR )
	{
		sDate.Format(_T("%d년 %02d월 %02d일 %s요일") , m_nYearSolar
										, m_nMonthSolar
						           		, m_nDaySolar
								        , szWeekdayName[m_nDayOfWeek-1]);
	}
	else
	{
		/*
		sDate.Format(_T("%d년 %02d월 %02d일 %s요일")  , m_nYearLunar
										, m_nMonthLunar
						           		, m_nDayLunar
								        , szWeekdayName[m_nDayOfWeek-1]);
		*/
		sDate.Format(_T("%d년 %02d월 %02d일")  , m_nYearLunar
										, m_nMonthLunar
						           		, m_nDayLunar);
		if( m_bIsLeapMonth )
			sDate += (_T("(윤달)"));
	}

	return CString((LPCTSTR)sDate);
}

// 휴일 여부. 일요일 포함
BOOL CKoreanDate::IsHoliday() const
{
	int i;
	int bIsLeapMonth;

	if( m_nDayOfWeek == 1 ) return TRUE;   // 일요일이면

	//--- 양력 휴일 검사 ------------------------------------------
	for(i = 0; i < sizeof(m_arrDayInfoSolar)/sizeof(TDayInfo); i++)
	{
		if( m_arrDayInfoSolar[i].nMonth > m_nMonthSolar ) break;

		if( m_arrDayInfoSolar[i].nMonth == m_nMonthSolar &&
			m_arrDayInfoSolar[i].nDay   == m_nDaySolar
		)
		{
			if( m_arrDayInfoSolar[i].bIsHoliday )
				return TRUE;
			else
				break;
		}
	}
	//-------------------------------------------------------------

	//--- 양력 휴일 검사 ------------------------------------------
	if( m_nMonthLunar == 12 )  // 설날 전날 검사. 음력 12월의 마지막 날인지 확인
	{
		bIsLeapMonth = HasLeapMonth(m_nYearLunar, m_nMonthLunar);
		if( m_bIsLeapMonth == bIsLeapMonth  && 
			m_nDayLunar == GetDaysOfMonth(m_nYearLunar, m_nMonthLunar, TYPE_LUNAR, bIsLeapMonth) )
			return TRUE;
	}

	if( !m_bIsLeapMonth )  // 윤달이 아닌 경우만
	{
		for(i = 0; i < sizeof(m_arrDayInfoLunar)/sizeof(TDayInfo); i++)
		{
			if( m_arrDayInfoLunar[i].nMonth > m_nMonthLunar ) break;

			if( m_arrDayInfoLunar[i].nMonth == m_nMonthLunar &&
				m_arrDayInfoLunar[i].nDay   == m_nDayLunar
			)
			{
				if( m_arrDayInfoLunar[i].bIsHoliday )
					return TRUE;
				else
					break;
			}
		}  // end of for
	}  // end of if( !m_bIsLeapMonth )
	//-------------------------------------------------------------

	return FALSE;
}

// public function
// diSolar에 양력 일자 정보를, diLunar에 음력 일자 정보를 넣어준다.
// Return value:
//    양력 일자 정보가 있으면 TYPE_SOLAR
//    음력 일자 정보가 있으면 TYPE_LUNAR
//    둘다 있으면             TYPE_SOLAR | TYPE_LUNAR
//    둘다 없으면             0
//
// Example:
//  CKoreanDate dt(2003, 5, 8);
//	TDayInfo diSolar, diLunar;
//	int nRet = dt.GetDayInfo(&diSolar, &diLunar);
//	if( nRet & TYPE_SOLAR )    // 양력 일자 정보가 있으면
//		TRACE("오늘은 양력으로 %s입니다.\r\n", diSolar.szDayName);
//	if( nRet & TYPE_LUNAR )    // 음력 일자 정보가 있으면
//		TRACE("오늘은 음력으로 %s입니다.\r\n", diLunar.szDayName);
int CKoreanDate::GetDayInfo(TDayInfo *diSolar, TDayInfo *diLunar) const
{
	int i;
	int bIsLeapMonth;
	int nReturn = 0;

	if( diSolar == NULL || diLunar == NULL ) return 0;

	memset(diSolar, 0, sizeof(TDayInfo));
	memset(diLunar, 0, sizeof(TDayInfo));
	diSolar->nType = TYPE_SOLAR;
	diLunar->nType = TYPE_LUNAR;

	//--- 양력 일자 검사 ------------------------------------------
	for(i = 0; i < sizeof(m_arrDayInfoSolar)/sizeof(TDayInfo); i++)
	{
		if( m_arrDayInfoSolar[i].nMonth > m_nMonthSolar ) break;

		if( m_arrDayInfoSolar[i].nMonth == m_nMonthSolar &&
			m_arrDayInfoSolar[i].nDay   == m_nDaySolar
		)
		{
			memcpy(diSolar, &m_arrDayInfoSolar[i], sizeof(TDayInfo));
			nReturn |= TYPE_SOLAR;
			break;
		}
	}
	//-------------------------------------------------------------

	//--- 음력 일자 검사 ------------------------------------------
	if( m_nMonthLunar == 12 )  // 설날 전날 검사. 음력 12월의 마지막 날인지 확인
	{
		bIsLeapMonth = HasLeapMonth(m_nYearLunar, m_nMonthLunar);
		if( m_bIsLeapMonth == bIsLeapMonth  && 
			m_nDayLunar == GetDaysOfMonth(m_nYearLunar, m_nMonthLunar, TYPE_LUNAR, bIsLeapMonth) )
		{
			diLunar->nMonth = m_nMonthLunar;
			diLunar->nDay = m_nDayLunar;
			diLunar->bIsHoliday = TRUE;
			diLunar->nType = TYPE_LUNAR;

			nReturn |= TYPE_LUNAR;
			return nReturn;
		}
	}

	if( !m_bIsLeapMonth )
	{
		for(i = 0; i < sizeof(m_arrDayInfoLunar)/sizeof(TDayInfo); i++)
		{
			if( m_arrDayInfoLunar[i].nMonth > m_nMonthLunar ) break;

			if( m_arrDayInfoLunar[i].nMonth == m_nMonthLunar &&
				m_arrDayInfoLunar[i].nDay   == m_nDayLunar
			)
			{
				memcpy(diLunar, &m_arrDayInfoLunar[i], sizeof(TDayInfo));
				nReturn |= TYPE_LUNAR;
				break;
			}
		} // end of for
	}
	//-------------------------------------------------------------

	return nReturn;
}

// public function
// 날짜 문자열 구하기
CString CKoreanDate::GetDateString(int nType, DWORD dwOption)
{
	return OnFormatDateString(nType, dwOption);
}

// public function
// = 연산자 정의
const CKoreanDate& CKoreanDate::operator=(const CKoreanDate& dateSrc)
{
	m_nYearSolar = dateSrc.m_nYearSolar;
	m_nMonthSolar = dateSrc.m_nMonthSolar;
	m_nDaySolar = dateSrc.m_nDaySolar;
	m_nYearLunar = dateSrc.m_nYearLunar;
	m_nMonthLunar = dateSrc.m_nMonthLunar;
	m_nDayLunar = dateSrc.m_nDayLunar;
	m_bIsLeapMonth = dateSrc.m_bIsLeapMonth;
	m_nDayOfWeek = dateSrc.m_nDayOfWeek;

	return *this;
}

// public function
// == 연산자 정의
BOOL CKoreanDate::operator==(const CKoreanDate& dateSrc) const
{
	return ( m_nYearSolar == dateSrc.m_nYearSolar &&
	         m_nMonthSolar == dateSrc.m_nMonthSolar &&
	         m_nDaySolar == dateSrc.m_nDaySolar);
}

// public function
// >= 연산자 정의
BOOL CKoreanDate::operator>=(const CKoreanDate& dateSrc) const
{
	return ( GetPassedDays() >= dateSrc.GetPassedDays() );
}

// public function
// <= 연산자 정의
BOOL CKoreanDate::operator<=(const CKoreanDate& dateSrc) const
{
	return ( GetPassedDays() <= dateSrc.GetPassedDays() );
}

// public function
// < 연산자 정의
BOOL CKoreanDate::operator<(const CKoreanDate& dateSrc) const
{
	return ( GetPassedDays() < dateSrc.GetPassedDays() );
}

// public function
// > 연산자 정의
BOOL CKoreanDate::operator>(const CKoreanDate& dateSrc) const
{
	return ( GetPassedDays() > dateSrc.GetPassedDays() );
}



