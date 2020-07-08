// SeExpectVerifier.cpp: implementation of the CSeExpectVerifier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeExpectVerifier.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeExpectVerifier::CSeExpectVerifier()
{

}

CSeExpectVerifier::~CSeExpectVerifier()
{

}


INT CSeExpectVerifier::VerifyDateExpData(CESL_DataMgr* pDM_DATE,INT nExpType,INT nRowIdx)
{
	
	//RULE 1: 예측시작일이 비어 있으면 안된다 
	INT ids = 0;
	CString strEXP_CLASS;
	INT nEXP_CLASS;
	strEXP_CLASS = pDM_DATE->GetCellData(_T("EXP_CLASS"),nRowIdx);
	nEXP_CLASS = _ttoi(strEXP_CLASS.GetBuffer(0));
	

	CString strEXP_START_DATE;
	CString msg;
	strEXP_START_DATE = pDM_DATE->GetCellData(_T("EXP_START_DATE"),nRowIdx);
	if(strEXP_START_DATE.IsEmpty())
	{
		if(1 == nEXP_CLASS) msg.Format(_T("[입수/발행예측] 발행 예측 시작일이 입력되지 않았습니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측 시작일이 입력되지 않았습니다!"));
		AfxMessageBox(msg);
		return -1;
	}


	switch(nExpType) 
	{
	case 0:
		ids = VerifyMonthType(pDM_DATE,nEXP_CLASS,strEXP_START_DATE,nRowIdx);
		break;
	case 1:
		ids = VerifyWeekType(pDM_DATE,nEXP_CLASS,strEXP_START_DATE,nRowIdx);
		break;
	case 2:
		ids = VerifyDayType(pDM_DATE,nEXP_CLASS,strEXP_START_DATE,nRowIdx);
		break;
	case 3:
		ids = VerifyEtcType(pDM_DATE,nEXP_CLASS,strEXP_START_DATE,nRowIdx);
		break;
	}
	
	return ids;
}

INT CSeExpectVerifier::VerifyMonthType(CESL_DataMgr* pDM,INT nTYPE,CString strEXP_START_DATE,INT nRowIdx)
{
	
	//RULE 1: 입수예측 시작일에 해당하는 날짜가 존재해야한다 
	//RULE 2: 날짜가 쓰여진 칸에는 부수가 존재해야 한다 
	//RULE 3: 날짜 칸에는 1 ~ 31일 사이의 숫자가 쓰여져야 한다 
	//RULE 3(수정): 각 월마다 예측 가능한 범위의 날짜가 쓰여야 한다 (1월 31일, 2월 28일, 3월 31일...) 
	//RULE 4: 부수칸의 숫자는 0 보다 커야한다

	INT month,day;
	ParsingDate(strEXP_START_DATE,month,day);
	
	const INT cnt = 12;
	CString TermDay[cnt];
	CString BookCnt[cnt];

	GetMonthTermDay(pDM, TermDay ,nRowIdx);
	GetMonthBookCnt(pDM, BookCnt ,nRowIdx);

	CString msg;
	if(TermDay[month-1].IsEmpty())
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측 시작일의 해당월에 날짜가 입력되지 않았습니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측 시작일의 해당월에 날짜가 입력되지 않았습니다!"));
		AfxMessageBox(msg);
		return -1;
	}	
	

	INT ntermday;
	ntermday = _ttoi(TermDay[month-1].GetBuffer(0));
	if(ntermday != day)
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측 시작일의 해당월에 날짜가 \n 발행 예측 시작일과 일치하지 않습니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측 시작일의 해당월에 날짜가 \n 입수 예측 시작일과 일치하지 않습니다!"));
		AfxMessageBox(msg);
		return -2;
	}

	INT nDay;
	INT nBook;
	INT nMaxDay;

	for(INT i=0;i<cnt;i++)
	{
		if(!TermDay[i].IsEmpty())
		{
			nDay = _ttoi(TermDay[i].GetBuffer(0));
			
			nMaxDay = GetMaxDay(i);
			
			if(nDay < 1 || nMaxDay < nDay)
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측의 %d 월의 날짜가 1 ~ %d일 사이에 해당하지 않습니다!"),i+1,nMaxDay);
				else msg.Format(_T("[입수/발행예측] 입수 예측의 %d 월의 날짜가 1 ~ %d일 사이에 해당하지 않습니다!"),i+1,nMaxDay);
				AfxMessageBox(msg);
				return -3;
			}

			if(BookCnt[i].IsEmpty())
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측의 %d 월의 부수가 입력되지 않았습니다!"),i+1);
				else msg.Format(_T("[입수/발행예측] 입수 예측의 %d 월의 부수가 입력되지 않았습니다!"),i+1);
				AfxMessageBox(msg);
				return -4;

			}

			nBook = _ttoi(BookCnt[i].GetBuffer(0));
			if(nBook<1) 
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측의 %d 월의 부수가 1 이하입니다!"),i+1);
				else msg.Format(_T("[입수/발행예측] 입수 예측의 %d 월의 부수가 1 이하입니다!"),i+1);
				AfxMessageBox(msg);
				return -5;
			}

		}
	}
	
	
	return 0;
}

INT CSeExpectVerifier::VerifyWeekType(CESL_DataMgr* pDM,INT nTYPE, CString strEXP_START_DATE, INT nRowIdx)
{
	//RULE 1: 입수예측 시작일에 해당하는 요일에 부수가 하나 이상 존재해야한다 
	//RULE 2: 부수는 1이상의 숫자여야 한다.
	
	INT year, month, day;
	year = month = day = -1;
	ExtractStringDateToIntDate(strEXP_START_DATE, &year, &month, &day);

	COleDateTime startTime(year, month, day, 0, 0, 0); //예측시작일 

	//예측시작일의 COleDateTime의 요일을 형식에 맞추어 수정 (1:sunday -> 7:sunday)
	INT weekday = startTime.GetDayOfWeek();
	if(weekday == 1) weekday = 7;
	else weekday -= 1;
	
	const INT cnt = 7;
	CString BookCnt[cnt];
	GetWeekCnt(pDM,BookCnt,nRowIdx);


	CString DayOfWeek[cnt] = {
			_T("월요일"),
			_T("화요일"),
			_T("수요일"),
			_T("목요일"),
			_T("금요일"),
			_T("토요일"),
			_T("일요일")};

	
	CString msg;
	if(BookCnt[weekday-1].IsEmpty())
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측 시작일의 해당 요일에 부수가 입력되지 않았습니다! \n 해당 요일은 [ %s ] 입니다."),DayOfWeek[weekday-1]);
		else msg.Format(_T("[입수/발행예측] 입수 예측 시작일의 해당 요일에 부수가 입력되지 않았습니다! \n 해당 요일은 [ %s ] 입니다."),DayOfWeek[weekday-1]);
		AfxMessageBox(msg);
		return -1;
	}	
	

	INT nBook;
	for(INT i=0;i<cnt;i++)
	{
		if(!BookCnt[i].IsEmpty())
		{
			nBook = _ttoi(BookCnt[i].GetBuffer(0));
			if(nBook<1)
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측 시작일의 %s의 부수가 1이하입니다!"),DayOfWeek[i]);
				else msg.Format(_T("[입수/발행예측] 입수 예측 시작일의 %s의 부수가 1이하입니다!"),DayOfWeek[i]);
				AfxMessageBox(msg);
				return -2;
			}
		}
	}


	
	return 0;
}

INT CSeExpectVerifier::VerifyDayType(CESL_DataMgr* pDM,INT nTYPE, CString strEXP_START_DATE, INT nRowIdx)
{
	//RULE 1: 입수예측 시작일에 해당하는 날짜가 존재해야한다 
	//RULE 2: 날짜가 쓰여진 칸에는 부수가 존재해야 한다 
	//RULE 3: 날짜 칸에는 1 ~ 31일 사이의 숫자가 쓰여져야 한다
	//RULE 3(수정): 날짜 칸에는 1 ~ 28일 사이의 숫자가 쓰여야 한다(2월 때문에 예측이 한정 된다)
	//RULE 4: 부수칸의 숫자는 0 보다 커야한다

	INT month,day;
	ParsingDate(strEXP_START_DATE,month,day);
	
	const INT cnt = 6;
	CString TermDay[cnt];
	CString BookCnt[cnt];

	GetDayTermDay(pDM, TermDay ,nRowIdx);
	GetDayBookCnt(pDM, BookCnt ,nRowIdx);


	INT nDay;
	INT nBook;
	BOOL IsChecked = FALSE;
	CString msg;
	for(INT i=0;i<cnt;i++)
	{
		if(!TermDay[i].IsEmpty())
		{
			nDay = _ttoi(TermDay[i].GetBuffer(0));

			if(nDay < 1 || 28 < nDay)
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측의 %d 번째 칸의 날짜가 1 ~ 28일 사이에 해당하지 않습니다!"),i+1);
				else msg.Format(_T("[입수/발행예측] 입수 예측의 %d  번째 칸의 날짜가 1 ~ 28일 사이에 해당하지 않습니다!"),i+1);
				AfxMessageBox(msg);
				return -1;
			}			

			if(BookCnt[i].IsEmpty())
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측의 %d 번째 칸의 부수가 입력되지 않았습니다!"),i+1);
				else msg.Format(_T("[입수/발행예측] 입수 예측의 %d 번째 칸의 부수가 입력되지 않았습니다!"),i+1);
				AfxMessageBox(msg);
				return -2;

			}

			nBook = _ttoi(BookCnt[i].GetBuffer(0));
			if(nBook<1) 
			{
				if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측의 %d 번째 칸의 부수가 1 이하입니다!"),i+1);
				else msg.Format(_T("[입수/발행예측] 입수 예측의 %d 번째 칸의 부수가 1 이하입니다!"),i+1);
				AfxMessageBox(msg);
				return -3;
			}

			if(day == nDay) IsChecked = TRUE;

		}
	}
	
	if(!IsChecked)
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측 시작일에 해당하는 날짜가 존재하지 않습니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측 시작일에 해당하는 날짜가 존재하지 않습니다!"));
		AfxMessageBox(msg);
		return -3;
	}
	
	
	return 0;
}

INT CSeExpectVerifier::VerifyEtcType(CESL_DataMgr* pDM,INT nTYPE, CString strEXP_START_DATE, INT nRowIdx)
{
	
	
	INT tmpyear,tmpmonth,tmpday;
	tmpyear = tmpmonth = tmpday = 0;
	
	CString strYear = _T("");
	strYear = pDM->GetCellData(_T("EXP_TERM_YEAR"),nRowIdx);
	strYear.TrimLeft();
	strYear.TrimRight();
	if(!strYear.IsEmpty()) tmpyear = _ttoi(strYear.GetBuffer(0));

	CString strMonth = _T("");
	strMonth = pDM->GetCellData(_T("EXP_TERM_MONTH"),nRowIdx);
	strMonth.TrimLeft();
	strMonth.TrimRight();
	if(!strMonth.IsEmpty()) tmpmonth = _ttoi(strMonth.GetBuffer(0));

	CString strDay = _T("");
	strDay = pDM->GetCellData(_T("EXP_TERM_DAY"),nRowIdx);
	strDay.TrimLeft();
	strDay.TrimRight();
	if(!strDay.IsEmpty()) tmpday = _ttoi(strDay.GetBuffer(0));


	CString msg;
	// 2005-02-21
	// RULE 1: 부수칸의 숫자는 0 보다 커야한다
	// BEGIN:
	CString strBookCnt = _T("");
	INT tmpBookCnt = 0;
	strBookCnt = pDM->GetCellData(_T("EXP_BOOK_CNT"),nRowIdx);
	strBookCnt.TrimLeft();
	strBookCnt.TrimRight();
	if(strBookCnt.IsEmpty()) 
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 부수가 입력되지 않았습니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측에 부수가 입력되지 않았습니다!"));
		AfxMessageBox(msg);			
		
		return -1;		
	}
	tmpBookCnt = _ttoi(strBookCnt.GetBuffer(0));
	if( tmpBookCnt<1 )
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 부수의 숫자가 1이하입니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측에 부수의 숫자가 1이하입니다!"));
		AfxMessageBox(msg);			
		return -2;	
	}
	// END:


	if(!tmpyear && !tmpmonth && !tmpday)
	{
		if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 년,월,일중 적어도 하나의 숫자가 입력되야 합니다!"));
		else msg.Format(_T("[입수/발행예측] 입수 예측에 년,월,일중 적어도 하나의 숫자가 입력되야 합니다!"));
		AfxMessageBox(msg);			
		
		return -1;
	}
	
	if(!strYear.IsEmpty())
	{
		//Ctimespan 제약
		if(tmpyear>10)
		{
			if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 년의 숫자는 10을 초과할 수 없습니다!"));
			else msg.Format(_T("[입수/발행예측] 입수 예측에 년의 숫자는 10을 초과할 수 없습니다!"));
			AfxMessageBox(msg);			
			return -2;		
		}
		
		//자연수 제약
		if(tmpyear<1)
		{
			if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 년의 숫자가 1이하입니다!"));
			else msg.Format(_T("[입수/발행예측] 입수 예측에 년의 숫자가 1이하입니다!"));
			AfxMessageBox(msg);			
			return -2;		
		}
	}
		
	if(!strMonth.IsEmpty())
	{
		if(tmpmonth<1 || tmpmonth>12)
		{
			if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 월의 숫자는 1~12 사이여야 합니다!"));
			else msg.Format(_T("[입수/발행예측] 입수 예측에 월의 숫자는 1~12 사이여야 합니다!"));
			AfxMessageBox(msg);			
			return -3;		
		}
	}
	
	if(!strDay.IsEmpty())
	{
		if(tmpday<1 || tmpday>31)
		{
			if(1 == nTYPE) msg.Format(_T("[입수/발행예측] 발행 예측에 일의 숫자는 1~31 사이여야 합니다!"));
			else msg.Format(_T("[입수/발행예측] 입수 예측에 일의 숫자는 1~31 사이여야 합니다!"));
			AfxMessageBox(msg);			
			return -4;		
		}
	}
	
	return 0;
}


VOID CSeExpectVerifier::ParsingDate(CString date, INT& month ,INT& day)
{
	INT npos = date.Find(_T("/"));
	CString tmp;
	CString strMonth,strDay;
	
	tmp = date.Mid(npos+1);
	npos = tmp.Find(_T("/"));
	strMonth = tmp.Left(npos);
	strDay = tmp.Mid(npos+1);
	
	strMonth.TrimLeft();
	strMonth.TrimRight();
	strDay.TrimLeft();
	strDay.TrimRight();
	
	month = _ttoi(strMonth.GetBuffer(0));
	day = _ttoi(strDay.GetBuffer(0));

}


INT CSeExpectVerifier::VerifyAppendixExpData(CESL_DataMgr* pDM_APPENDIX)
{
	//RULE 1: 부록명은 빈칸이 들어갈 수 없다
	CString strAPPENDIX_NAME;
	CString msg;

	for(INT i=0;i<pDM_APPENDIX->GetRowCount();i++)
	{
		strAPPENDIX_NAME = _T("");
		
		strAPPENDIX_NAME = pDM_APPENDIX->GetCellData(_T("부록명"),i);
		if(strAPPENDIX_NAME.IsEmpty())
		{
			msg.Format(_T("[부록예측] %d 번째 라인이 부록명이 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);
			return -1;
		}
	}


	return 0;
}

INT CSeExpectVerifier::VerifyVolExpData(CESL_DataMgr* pDM_EXP_VOL)
{
	//RULE 1: 단계명과 단계 식별명은 빈칸이 들어갈 수 없다
	//RULE 2: 단계명과 단계 식별명을 제외한 모든 데이터는 숫자이어야 하고 빈칸이 되면 안된다.   
//****RULE 2수정: 단계증가값은 숫자이외에 ':' 기호를 포함한다.
	//RULE 3: 첫번째 row 데이터  일 경우에 한해서만 단계증가값이 존재하면 안된다. 그이외의 경우는 존재해야만 한다   
	//RULE 4: 발행일 예측정보 시에는 예측시작수가 존재하면 안된다.

	INT nRowCnt = pDM_EXP_VOL->GetRowCount();
	if(0 == nRowCnt) return 0;

	CString strFORMAT_TYPE;
	strFORMAT_TYPE = pDM_EXP_VOL->GetCellData(_T("유형번호"),0);
	INT nTYPE = _ttoi(strFORMAT_TYPE.GetBuffer(0));


	CString strVOL_NAME;
	CString strLEVEL_SIGN;
	CString strEXP_START_NO;
	CString strEXP_END_NO;
	CString strINC_UNIT_LIMIT;
	CString strINC_UNIT;
	CString strACQ_EXP_NO;
	CString strDisplayOrder;
	CString strINC_UNIT_LEVEL_UDF;
	CString strINC_UNIT_LEVEL;

	CString msg;
	for(INT i=0;i<nRowCnt;i++)
	{
		strVOL_NAME		= _T("");
		strLEVEL_SIGN	= _T("");
		strEXP_START_NO = _T("");

		strVOL_NAME = pDM_EXP_VOL->GetCellData(_T("단계명"),i);
		strLEVEL_SIGN = pDM_EXP_VOL->GetCellData(_T("단계식별명"),i);
		strEXP_START_NO = pDM_EXP_VOL->GetCellData(_T("시작수"),i);
		strEXP_END_NO = pDM_EXP_VOL->GetCellData(_T("끝수"),i);
		strINC_UNIT_LEVEL_UDF = pDM_EXP_VOL->GetCellData(_T("UDF_단계증가레벨"),i);
		strINC_UNIT_LIMIT = pDM_EXP_VOL->GetCellData(_T("단계증가값"),i);
		strINC_UNIT_LEVEL = pDM_EXP_VOL->GetCellData(_T("단계증가레벨"),i);
		strINC_UNIT = pDM_EXP_VOL->GetCellData(_T("증가단위"),i);
		strACQ_EXP_NO = pDM_EXP_VOL->GetCellData(_T("예측시작수"),i);
		strDisplayOrder = pDM_EXP_VOL->GetCellData(_T("순서"),i);

		
		//발행일 예측정보 사용
		if(0 == i && _T("<발행일>") == strVOL_NAME) 
		{
			return 0;
		}
		
		if ( strDisplayOrder.IsEmpty() )
		{	

			BOOL bErrFlag = FALSE;

			for ( INT nIdx = 0 ; nIdx < pDM_EXP_VOL->GetRowCount() ; nIdx++ )
				if ( !pDM_EXP_VOL->GetCellData(_T("순서"), nIdx ).IsEmpty() ) bErrFlag = TRUE;


			if ( !bErrFlag ) 
			{
			}
			else
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인이 순서가 입력되지 않았습니다!"),i+1);
				else msg.Format(_T("[부차적권호예측] %d 번째 라인이 순서가 입력되지 않았습니다!"),i+1);
				AfxMessageBox(msg);			
				return -1;	
			}
		}
		else 
		{
			if(!CheckDigit(strDisplayOrder))
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 순서가 숫자가 아닙니다!"),i+1);
				else msg.Format(_T("[부차적권호예측] %d 번째 라인의 순서가 숫자가 아닙니다!"),i+1);
				AfxMessageBox(msg);			
				return -4;	
			}

			for ( INT nIdx = 0 ; nIdx < pDM_EXP_VOL->GetRowCount() ; nIdx++ )
			{
				if ( nIdx == i ) continue;

				if ( strDisplayOrder == pDM_EXP_VOL->GetCellData(_T("순서"), nIdx ) )
				{
					msg.Format( _T("예측정보 내에 같은 순서가 있습니다.") );
					AfxMessageBox(msg);			
					return -4;
				}
			}
		}

		//단계명
		if(strVOL_NAME.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인이 단계명이 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인이 단계명이 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -1;	
		}
		
		//단계식별명 
		if(strLEVEL_SIGN.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인이 단계식별명이 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인이 단계식별명이 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -2;	
		}
		
		//시작수 
		if(strEXP_START_NO.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인이 시작수가 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인이 시작수가 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -3;	
		}
		else 
		{
			if(!CheckDigit(strEXP_START_NO))
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 시작수가 숫자가 아닙니다!"),i+1);
				else msg.Format(_T("[부차적권호예측] %d 번째 라인의 시작수가 숫자가 아닙니다!"),i+1);
				AfxMessageBox(msg);			
				return -3;	
			}
		}
		
		//끝수 
		if(strEXP_END_NO.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인이 끝수가 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인이 끝수가 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -4;	
		}
		else
		{
			if(!CheckDigit(strEXP_END_NO))
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 끝수가 숫자가 아닙니다!"),i+1);
				else msg.Format(_T("[부차적권호예측] %d 번째 라인의 끝수가 숫자가 아닙니다!"),i+1);
				AfxMessageBox(msg);			
				return -4;	
			}
		}

		
		//단계증가값 
		if(strINC_UNIT_LEVEL_UDF.IsEmpty() && i != 0) 
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 단계증가값이 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인의 단계증가값이 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -5;	
		}
				
		if(0 == i &&  !strINC_UNIT_LEVEL_UDF.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] 첫번째 라인에 단계증가값이 입력되었습니다!"));
			else msg.Format(_T("[부차적권호예측] 첫번째 라인에 단계증가값이 입력되었습니다!"));
			AfxMessageBox(msg);			
			return -5;	
		}
		
		if(!strINC_UNIT_LEVEL_UDF.IsEmpty())
		{
			INT nPos = strINC_UNIT_LEVEL_UDF.Find(_T(":"));
			if(nPos < 0)
			{
				if(!CheckDigit(strINC_UNIT_LEVEL_UDF))
				{
					if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 단계증가값이 숫자가 아닙니다!"),i+1);
					else msg.Format(_T("[부차적권호예측] %d 번째 라인의 단계증가값이 숫자가 아닙니다!"),i+1);
					AfxMessageBox(msg);			
					return -5;	
				}
				pDM_EXP_VOL->SetCellData(_T("단계증가값"),strINC_UNIT_LEVEL_UDF, i);
				if(!strINC_UNIT_LEVEL.IsEmpty())
					pDM_EXP_VOL->SetCellData(_T("단계증가레벨"),_T(""), i);
			}
			else // 단계증가 레벨이 입력되어있으면 저장한다.
			{
				strINC_UNIT_LIMIT = strINC_UNIT_LEVEL_UDF.Left(nPos);
				if(!CheckDigit(strINC_UNIT_LIMIT))
				{
					if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 단계증가값이 숫자가 아닙니다!"),i+1);
					else msg.Format(_T("[부차적권호예측] %d 번째 라인의 단계증가값이 숫자가 아닙니다!"),i+1);
					AfxMessageBox(msg);			
					return -5;	
				}
				CString strINC_UNIT_LEVEL = strINC_UNIT_LEVEL_UDF.Mid(nPos+1);
				if(!CheckDigit(strINC_UNIT_LEVEL))
				{
					if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 단계증가레벨이 숫자가 아닙니다!"),i+1);
					else msg.Format(_T("[부차적권호예측] %d 번째 라인의 단계증가레벨이 숫자가 아닙니다!"),i+1);
					AfxMessageBox(msg);			
					return -5;	
				}
				pDM_EXP_VOL->SetCellData(_T("단계증가값"),strINC_UNIT_LIMIT, i);
				pDM_EXP_VOL->SetCellData(_T("단계증가레벨"),strINC_UNIT_LEVEL, i);
			}

		}
/*		if(!strINC_UNIT_LIMIT.IsEmpty())
		{
			if(!CheckDigit(strINC_UNIT_LIMIT))
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 단계증가값이 숫자가 아닙니다!"),i+1);
				else msg.Format(_T("[부차적권호예측] %d 번째 라인의 단계증가값이 숫자가 아닙니다!"),i+1);
				AfxMessageBox(msg);			
				return -5;	
			}
		}
*/	
		//증가단위 
		if(strINC_UNIT.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 증가단위가 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인의 증가단위가 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -6;	
		}
		else 
		{
			if(!CheckDigit(strINC_UNIT))
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측]  %d 번째 라인의 증가단위이 숫자가 아닙니다!"),i+1);
				else msg.Format(_T("[부차적권호예측]  %d 번째 라인의 증가단위이 숫자가 아닙니다!"),i+1);
				AfxMessageBox(msg);			
				return -6;	
			}
		}
	

		//예측시작수 
		if(strACQ_EXP_NO.IsEmpty())
		{
			if(1 == nTYPE) msg.Format(_T("[권호예측] %d 번째 라인의 예측시작수가 입력되지 않았습니다!"),i+1);
			else msg.Format(_T("[부차적권호예측] %d 번째 라인의 예측시작수가 입력되지 않았습니다!"),i+1);
			AfxMessageBox(msg);			
			return -8;	
		}
		else
		{
			if(!CheckDigit(strACQ_EXP_NO))
			{
				if(1 == nTYPE) msg.Format(_T("[권호예측]  %d 번째 라인의 예측시작수가 숫자가 아닙니다!"),i+1);
				else msg.Format(_T("[부차적권호예측]  %d 번째 라인의 예측시작수가 숫자가 아닙니다!"),i+1);
				AfxMessageBox(msg);			
				return -8;	
			}


		}

	}
	
	return 0;
}

VOID CSeExpectVerifier::GetMonthTermDay(CESL_DataMgr* pDM, CString data[] ,INT nRowIdx)
{
	const INT cnt = 12;
	CString alias[cnt] = {
		_T("EXP_TERM_DAY1"),
		_T("EXP_TERM_DAY2"),
		_T("EXP_TERM_DAY3"),
		_T("EXP_TERM_DAY4"),
		_T("EXP_TERM_DAY5"),
		_T("EXP_TERM_DAY6"),
		_T("EXP_TERM_DAY7"),
		_T("EXP_TERM_DAY8"),
		_T("EXP_TERM_DAY9"),
		_T("EXP_TERM_DAY10"),
		_T("EXP_TERM_DAY11"),
		_T("EXP_TERM_DAY12")};

	pDM->GetCellData(alias,12,data,nRowIdx);
}

VOID CSeExpectVerifier::GetMonthBookCnt(CESL_DataMgr* pDM, CString data[],INT nRowIdx)
{

	const INT cnt = 12;
	CString alias[cnt] = {
		_T("EXP_BOOK_CNT1"),
		_T("EXP_BOOK_CNT2"),
		_T("EXP_BOOK_CNT3"),
		_T("EXP_BOOK_CNT4"),
		_T("EXP_BOOK_CNT5"),
		_T("EXP_BOOK_CNT6"),
		_T("EXP_BOOK_CNT7"),
		_T("EXP_BOOK_CNT8"),
		_T("EXP_BOOK_CNT9"),
		_T("EXP_BOOK_CNT10"),
		_T("EXP_BOOK_CNT11"),
		_T("EXP_BOOK_CNT12")};
	
	pDM->GetCellData(alias,12,data,nRowIdx);
}

VOID CSeExpectVerifier::GetWeekCnt(CESL_DataMgr* pDM, CString data[] ,INT nRowIdx)
{
	const INT cnt = 7;
	
	CString alias[cnt] = {
		_T("MON_CNT"),
		_T("TUE_CNT"),
		_T("WED_CNT"),
		_T("THR_CNT"),
		_T("FRI_CNT"),
		_T("SAT_CNT"),
		_T("SUN_CNT")};
	
	pDM->GetCellData(alias,7,data,nRowIdx);
}

VOID CSeExpectVerifier::ExtractStringDateToIntDate(CString	lastExpDate, INT *year, INT *month, INT *day)
{
	if(lastExpDate.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = lastExpDate.GetLength();
	idx = lastExpDate.Find(_T("/"), startidx);
	if(idx > 0)
		*year = _ttoi((lastExpDate.Left(idx)).GetBuffer(0));
	startidx = idx + 1;
	idx = lastExpDate.Find(_T("/"), startidx);
	if(idx > 0)
		*month = _ttoi((lastExpDate.Mid(startidx, idx - 1)).GetBuffer(0));
	startidx = idx + 1;
	*day = _ttoi((lastExpDate.Right(lastidx - startidx)).GetBuffer(0));
	return ;
}

VOID CSeExpectVerifier::GetDayTermDay(CESL_DataMgr* pDM, CString data[] ,INT nRowIdx)
{
	const INT cnt = 6;
	CString alias[cnt] = {
		_T("EXP_TERM_DATE1"),
		_T("EXP_TERM_DATE2"),
		_T("EXP_TERM_DATE3"),
		_T("EXP_TERM_DATE4"),
		_T("EXP_TERM_DATE5"),
		_T("EXP_TERM_DATE6")};

	pDM->GetCellData(alias,6,data,nRowIdx);
}

VOID CSeExpectVerifier::GetDayBookCnt(CESL_DataMgr* pDM, CString data[],INT nRowIdx)
{

	const INT cnt = 6;
	CString alias[cnt] = {
		_T("EXP_BOOK_CNT1"),
		_T("EXP_BOOK_CNT2"),
		_T("EXP_BOOK_CNT3"),
		_T("EXP_BOOK_CNT4"),
		_T("EXP_BOOK_CNT5"),
		_T("EXP_BOOK_CNT6")};
	
	pDM->GetCellData(alias,6,data,nRowIdx);
}

BOOL CSeExpectVerifier::CheckDigit(CString str)
{
	if(str.IsEmpty()) return TRUE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;

}

INT CSeExpectVerifier::GetMaxDay(INT nMonth)
{
	INT nMaxDay[12];
	
	nMaxDay[0] = 31;
	nMaxDay[1] = 28;
	nMaxDay[2] = 31;	
	nMaxDay[3] = 30;
	nMaxDay[4] = 31;
	nMaxDay[5] = 30;
	nMaxDay[6] = 31;
	nMaxDay[7] = 31;
	nMaxDay[8] = 30;
	nMaxDay[9] = 31;
	nMaxDay[10] = 30;
	nMaxDay[11] = 31;


	return nMaxDay[nMonth];
}