// ECO_Time.cpp: implementation of the CECO_Time class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "except.h"
#include "ECO_Time.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CECO_Time::CECO_Time()
{

}

CECO_Time::~CECO_Time()
{

}

VOID CECO_Time::C_Time(
						INT *Sec ,
						INT *Min ,
						INT *Hour ,
						INT *Day ,
						INT *Month ,
						INT *Year
					)
{
ESL_EXCEPTION_BEGIN
	time_t ltime;
struct tm *cur_time;

/* 1. Read Time */
time( &ltime );
cur_time = localtime( &ltime );

/* 2. Move Sec, Min, Day, Month, Year */
*Sec = cur_time->tm_sec;
*Min = cur_time->tm_min;
*Hour = cur_time->tm_hour;
*Day = cur_time->tm_mday;
*Month = cur_time->tm_mon + 1;
*Year = cur_time->tm_year + 1900;

return;
ESL_EXCEPTION_END
}


VOID CECO_Time::Time_CNVL (
							INT dy ,
							INT dm ,
							INT dd ,
							INT ch ,
							INT cm ,
							INT cs ,
							LONG *lgtm
						)
{
ESL_EXCEPTION_BEGIN
	LONG qtime;
time_t result;
struct tm *newtime;
INT y;

if ( dy > 1900 ) y = dy - 1900;
else y = dy;

time(&qtime);
newtime = localtime( &qtime );

if ( y != -1 ) newtime->tm_year = y;
if ( dm != -1 ) newtime->tm_mon = dm-1;
if ( dd != -1 ) newtime->tm_mday = dd;
if ( ch != -1 ) newtime->tm_hour = ch;
if ( cm != -1 ) newtime->tm_min = cm;
if ( cs != -1 ) newtime->tm_sec = cs;
result=mktime( newtime );
*lgtm = result;

return;
ESL_EXCEPTION_END
}


INT CECO_Time::Time_LCNV	(
							LONG lgtm,
							INT *dy ,
		 INT *dm ,
		 INT *dd ,
							INT *ch ,
							INT *cm ,
		 INT *cs
						)
{
ESL_EXCEPTION_BEGIN
	struct tm *newtime;

*dy = *dm = *dd = *ch = *cm = *cs = 0;

newtime = localtime(&lgtm);
if ( newtime == NULL ) return(-1);
*dy = newtime->tm_year;
*dm = newtime->tm_mon + 1;
*dd = newtime->tm_mday;
*ch = newtime->tm_hour;
*cm = newtime->tm_min;
*cs = newtime->tm_sec;

return(1);
ESL_EXCEPTION_END
}
