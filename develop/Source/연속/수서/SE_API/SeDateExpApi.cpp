// SeDateExpApi.cpp: implementation of the CSeDateExpApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeDateExpApi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeDateExpApi::CSeDateExpApi(CESL_Mgr *pParentMgr)
{
	m_pMgr = pParentMgr;
	m_strPUB_FREQ = _T("");
	m_strREC_KEY = _T("");
	m_nRepeatCnt = -1;
	m_nRepeatUnit = -1;
	m_nAddUnit = -1;
	m_strTBL_NAME = _T("");
	m_strDM_ALIAS = _T("");
	m_strTYPE = _T("");
	m_arrayExpDate.RemoveAll();
	m_arrayLastExpCount.RemoveAll();
	m_nCount = 0;
	m_bIsPubExpType = FALSE;
}

CSeDateExpApi::~CSeDateExpApi()
{


}

INT CSeDateExpApi::InitDateExpMgr(CString strPUB_FREQ, CString strREC_KEY, CString strTYPE, INT nMoreExp)
{

	if(!strPUB_FREQ.IsEmpty()) 	m_strPUB_FREQ = strPUB_FREQ;
	if(!strREC_KEY.IsEmpty())	m_strREC_KEY = strREC_KEY;
	if(!strTYPE.IsEmpty()) m_strTYPE = strTYPE;

	switch( strPUB_FREQ.GetAt(0) )
	{
		case 'a' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_SINGLE;
			break;
		case 'b' :
			m_nRepeatCnt = 6;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_SINGLE;
			break;
		case 'f' :
			m_nRepeatCnt = 2;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_SINGLE;
			break;
		case 'g' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_DOUBLE;
			break;
		case 'h' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_TRIPLE;
			break;
		case 'm' :
			m_nRepeatCnt = 12;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_SINGLE;
			break;
		case 't' :
			m_nRepeatCnt = 3;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_SINGLE;
			break;
		case 'q' :
			m_nRepeatCnt = 4;
			m_nRepeatUnit = YEAR;
			m_nAddUnit = U_SINGLE;
			break;
		case 'c' :
			m_nRepeatCnt = 2;
			m_nRepeatUnit = WEEK;
			m_nAddUnit = U_SINGLE;
			break;
		case 'd' :
			m_nRepeatCnt = 7;
			m_nRepeatUnit = WEEK;
			m_nAddUnit = U_SINGLE;
			break;
		case 'e' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = WEEK;
			m_nAddUnit = U_DOUBLE;
			break;
		case 'i' :
			m_nRepeatCnt = 3;
			m_nRepeatUnit = WEEK;
			m_nAddUnit = U_SINGLE;
			break;
		case 'w' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = WEEK;
			m_nAddUnit = U_SINGLE;
			break;
		case 'j' :
			m_nRepeatCnt = 3;
			m_nRepeatUnit = MONTH;
			m_nAddUnit = U_SINGLE;
			break;
		case 's' :
			m_nRepeatCnt = 2;
			m_nRepeatUnit = MONTH;
			m_nAddUnit = U_SINGLE;
			break;
		case 'u' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = UNKOWN;
			m_nAddUnit = U_SINGLE;
			break;			
		case 'z' :
			m_nRepeatCnt = 1;
			m_nRepeatUnit = UNKOWN;
			m_nAddUnit = U_SINGLE;
			break;
		default:
			m_nRepeatCnt = 1;
			m_nRepeatUnit = UNKOWN;
			m_nAddUnit = U_SINGLE;
			break;
	}
	
	
	switch(m_strTYPE.GetAt(0))
	{
	case 'M':
		m_strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		m_strDM_ALIAS = MONTH_DM_ALIAS;
		break;
	case 'W':
		m_strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		m_strDM_ALIAS = WEEK_DM_ALAIS;
		break;
	case 'D':
		m_strTBL_NAME = _T("SE_DAY_EXP_TBL");
		m_strDM_ALIAS = DAY_DM_ALIAS;
		break;
	case 'E':
		m_strTBL_NAME = _T("SE_ETC_EXP_TBL");
		m_strDM_ALIAS = ETC_DM_ALIAS;
		break;
	}



	m_pDM = m_pMgr->FindDM(m_strDM_ALIAS);
	if(!m_pDM)	AfxMessageBox(_T("적절한 일자 예측 DM을 찾을 수 없습니다!")); 
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);
	m_pDM->RefreshDataManager(strWhere);

	if(0 == m_pDM->GetRowCount()) 
	{
		AfxMessageBox(_T("일자 예측정보가 없습니다!")); 
	}
	

	if(MakeExpDateArray(nMoreExp)<0) {
		return -1001; //예측 정보 없음 
	}

	return 0;
}

INT CSeDateExpApi::MakeExpDateArray(INT nMoreExp)
{
	INT ids = 0;

	switch(m_strTYPE.GetAt(0))
	{
	case 'M':
		ids = MakeMonthTypeArray(nMoreExp);
		break;
	case 'W':
		ids = MakeWeekTypeArray(nMoreExp);
		break;
	case 'D':
		ids = MakeDayTypeArray(nMoreExp);
		break;
	case 'E':
		ids = MakeEtcTypeArray(nMoreExp);
		break;
	}

	return ids;

}


INT CSeDateExpApi::MakeMonthTypeArray(INT nMoreExp)
{
	CString lastExpDate,lastExpCount;
	lastExpDate = m_pDM->GetCellData(_T("EXP_START_DATE"),0);
	lastExpCount = m_pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),0);
	INT nLastExpCount = 0;
	if(!lastExpCount.IsEmpty()) nLastExpCount = _ttoi(lastExpCount.GetBuffer(0));

	if(lastExpDate.IsEmpty()) return -1;
	
	const INT cnt = 12;
	CString EXP_DAY[cnt];
	CString EXP_CNT[cnt];
	GetMonthTermDay(EXP_DAY,0);
	GetMonthBookCnt(EXP_CNT,0);

	INT year, month, day;
	year = month = day = -1;
	
	ExtractStringDateToIntDate(lastExpDate, &year, &month, &day);
	COleDateTime lastTime(year, month, day, 0, 0, 0); //예측시작일 
	COleDateTime currentTime = COleDateTime::GetCurrentTime(); //오늘 날자 

	CString strcurrentime;
	strcurrentime.Format(_T("%04d/%02d/%02d"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay());
	
	
	//먼저 년도를 비교한다
	INT lastYear = year;
	INT lastMonth = month;
	INT	currentYear = currentTime.GetYear();
	CString strDate;
	INT nYearGap = currentYear - lastYear;
	INT nVol = 0;
	INT nCheckCount = 0;
	CString strtmplastexpcnt;
	INT nUDFLastExpCount = 0;
	INT nDefaultVol = 0;
	INT nCurSize = 0;

	if(m_nAddUnit != U_SINGLE)
	{
		nYearGap = nYearGap/m_nAddUnit + 1;
	}
	
	if(lastTime <= currentTime) //오늘 날짜를 기본으로 하는 예측 
	{
		for(INT i=0;i<nYearGap+1;i++)
		{
			for(INT j=0;j<12;j++)
			{
					
				//발행 부수를 확인
				if(EXP_CNT[j].IsEmpty()) continue;
					
				strDate = _T("");
				strDate = GetMonthStringDate(EXP_DAY, lastYear + (i*m_nAddUnit) , j+1);	
				if(strDate.IsEmpty()) continue;
						
				if(CompareStringDates(strDate, lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
				{
					//오늘 날짜와 같거나 이전 값이면 
					if(CompareStringDates(strcurrentime, strDate)>=0)
					{
							
						nVol = _ttoi(EXP_CNT[j].GetBuffer(0));
						nDefaultVol = nVol;
						
						//기존에 예측된 count 적용
						if(0 == nCheckCount)
							nVol -= nLastExpCount;
							
						//발행부수만큼 array에 입력 
						for(INT k=0;k<nVol;k++)
						{
							m_arrayExpDate.Add(strDate);
							
							if(0 == nCheckCount) {

								nUDFLastExpCount = nLastExpCount+k+1;
								
								if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
								else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
							
							}	
							else 
							{
								nUDFLastExpCount = k+1;
								if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
								else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
								
							}

							m_arrayLastExpCount.Add(strtmplastexpcnt);

							//발행일의 경우는 총 갯수를 맞춘다 
							if(m_bIsPubExpType)
							{
								nCurSize = m_arrayExpDate.GetSize();
								if(nMoreExp == nCurSize) return 0;
							}
						}

						nCheckCount++;
					}
				}
			}
		}

	}
	
	if(m_bIsPubExpType) //발행일
	{
		INT nCurSize = m_arrayExpDate.GetSize();
		
		//발행일의 경우는 입수일과 갯수를 맞춘다
		INT nTotalPubExp = nMoreExp;
		nMoreExp = nTotalPubExp-nCurSize-1;
		
	}
	else //입수일 
	{
		if(0 == nMoreExp && 0 == m_arrayExpDate.GetSize()) return 0;

	}
	
	//최종적으로 한번 더 예측한다.
	nMoreExp++; 

	
	INT nCurYear = currentTime.GetYear();
	INT nCurMonth = currentTime.GetMonth();
	
	if(m_nAddUnit != U_SINGLE) //격년간, 3년 1회간 
	{
		INT tmpyear;
		nCurMonth = 1;

		while(nCurYear < lastYear)
		{
			nCurYear++;
		}
			
		while(1)
		{
			tmpyear = nCurYear - lastYear;
			
			if(tmpyear%m_nAddUnit == 0)
			{
				break;
			}
			else
			{
				nCurYear++;	
			}
		}
	}
	
	
	CString strCompareDate;
	INT nCurMoreExp = 0;

	while(1)
	{
		//발행 부수를 확인
		if(EXP_CNT[nCurMonth-1].IsEmpty()) {
				
			if(12 == nCurMonth)
			{
				nCurMonth = 1;
				nCurYear += m_nAddUnit;
			}
			else
			{
				nCurMonth++;
			}

			continue;
		}		
		
		strCompareDate = GetMonthStringDate(EXP_DAY, nCurYear, nCurMonth);
		
		if(CompareStringDates(strCompareDate, lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
		{
			if(CompareStringDates(strCompareDate, strcurrentime)>0) //오늘 날짜보다 앞이면
			{
				//예측 권수 
				nVol = _ttoi(EXP_CNT[nCurMonth-1].GetBuffer(0));
				nDefaultVol = nVol;
				
				//기존에 예측된 count 적용
				if(0 == nCheckCount)
					nVol -= nLastExpCount;
				
				
				//발행부수만큼 array에 입력 
				for(INT i=0;i<nVol;i++)
				{
					m_arrayExpDate.Add(strCompareDate);
							
					if(0 == nCheckCount) {

						nUDFLastExpCount = nLastExpCount+i+1;
						if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
						else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
							
					}	
					else 
					{
						nUDFLastExpCount = i+1;
						if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
						else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);

					}

					m_arrayLastExpCount.Add(strtmplastexpcnt);
					nCurMoreExp++;

					if(nCurMoreExp == nMoreExp) return 0; //예측 끝
				}
				nCheckCount++;
			}

		}

		if(12 == nCurMonth)
		{
			nCurMonth = 1;
			nCurYear += m_nAddUnit;
		}
		else
		{
			nCurMonth++;
		}
		
	}

	return 0;
}

INT CSeDateExpApi::MakeWeekTypeArray(INT nMoreExp)
{
	CString lastExpDate,lastExpCount;
	lastExpDate = m_pDM->GetCellData(_T("EXP_START_DATE"),0);
	lastExpCount = m_pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),0);
	INT nLastExpCount = 0;
	if(!lastExpCount.IsEmpty()) nLastExpCount = _ttoi(lastExpCount.GetBuffer(0));

	if(lastExpDate.IsEmpty()) return -1;
	
	const INT cnt = 7;
	CString EXP_DAY[cnt];
	GetWeekCnt(EXP_DAY,0); //DM에서 정보를 가져온 후 

	INT year, month, day;
	year = month = day = -1;
	
	ExtractStringDateToIntDate(lastExpDate, &year, &month, &day);
	COleDateTime lastTime(year, month, day, 0, 0, 0); //예측시작일 
	COleDateTime currentTime = COleDateTime::GetCurrentTime(); //오늘 날자 

	CString strcurrentime;
	strcurrentime.Format(_T("%04d/%02d/%02d"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay());

	//예측시작일의 COleDateTime의 요일을 형식에 맞추어 수정 (1:sunday -> 7:sunday)
	INT weekday = lastTime.GetDayOfWeek();
	if(weekday == 1) weekday = 7;
	else weekday -= 1;
	
	//요일에 따라서 날짜를 늘려 가면서 비교한다 
	INT weekcount = 0;
	COleDateTime tmptime;
	CString strtmptime;
	BOOL IsBreak = FALSE;
	CString strValidate;
	INT nVol = 0;
	INT nCheckCount = 0;
	CString strtmplastexpcnt;
	INT nDefaultVol = 0;
	INT nUDFLastExpCount = 0;
	INT nCurSize = 0;

	if(lastTime <= currentTime ) //오늘 날짜를 기준으로 하는 예측 
	{
		while(1)
		{
			for(INT i=1;i<=7;i++)
			{
				if(0 == weekcount &&  i < weekday) continue;
				
				//발행 부수를 확인 
				if(EXP_DAY[i-1].IsEmpty()) {
					
					if(7 == i) weekcount++;
					continue;
				}


				COleDateTimeSpan dif((7*weekcount*m_nAddUnit)+(i-weekday), 0, 0, 0);
				tmptime	= lastTime + dif;
				
				if(tmptime<0) return -1;

				strtmptime.Format(_T("%04d/%02d/%02d"), tmptime.GetYear(), tmptime.GetMonth(), tmptime.GetDay());
				
				if(CompareStringDates(strtmptime,lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
				{
				
					if(CompareStringDates(strcurrentime, strtmptime)>=0) //오늘 날짜와 같거나 보다 이전 값이면  
					{
						nVol = _ttoi(EXP_DAY[i-1].GetBuffer(0));
						nDefaultVol = nVol;
						
						//기존에 예측된 count 적용
						if(0 == nCheckCount)
							nVol -= nLastExpCount;

						//발행부수만큼 array에 입력 
						for(INT j=0;j<nVol;j++)
						{
							m_arrayExpDate.Add(strtmptime);
							
							if(0 == nCheckCount) {

								nUDFLastExpCount = nLastExpCount+j+1;
								
								if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
								else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
							
							}	
							else 
							{
								nUDFLastExpCount = j+1;
								if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
								else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
								
							}

							m_arrayLastExpCount.Add(strtmplastexpcnt);


							//발행일의 경우는 총 갯수를 맞춘다 
							if(m_bIsPubExpType)
							{
								nCurSize = m_arrayExpDate.GetSize();
								if(nMoreExp == nCurSize) return 0;
							}
						}


						nCheckCount++;
					}
					else
					{
						IsBreak = TRUE;
						break;
					}

				}

				if(7 == i) weekcount++;
			}
		
			if(IsBreak) break;
		}
	}


	
	if(m_bIsPubExpType) //발행일
	{
		INT nCurSize = m_arrayExpDate.GetSize();
		
		//발행일의 경우는 입수일과 갯수를 맞춘다
		INT nTotalPubExp = nMoreExp;
		nMoreExp = nTotalPubExp-nCurSize-1;
		
	}
	else //입수일 
	{
		if(0 == nMoreExp && 0 == m_arrayExpDate.GetSize()) return 0;

	}

	//최종적으로 한번 더 예측한다.
	nMoreExp++; 
	
	INT nCurMoreExp = 0;
	weekcount = 0;
	IsBreak = FALSE;

	//오늘의 요일을 기준으로 추가 예측을 한다
	INT curweekday = currentTime.GetDayOfWeek();
	if(curweekday == 1) curweekday = 7;
	else curweekday -= 1;
				
	while(1)
	{
		for(INT i=1;i<=7;i++)
		{
			
			//발행 부수를 확인 
			if(EXP_DAY[i-1].IsEmpty()) {
				
				if(7 == i) weekcount++;
				continue;
			}

			COleDateTimeSpan dif((7*weekcount*m_nAddUnit)+(i-curweekday), 0, 0, 0);

			tmptime	= currentTime + dif;
			strtmptime.Format(_T("%04d/%02d/%02d"), tmptime.GetYear(), tmptime.GetMonth(), tmptime.GetDay());
			
			if(CompareStringDates(strtmptime,lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
			{

				if(CompareStringDates(strtmptime,strcurrentime)>0) //오늘 보다 나중이면 
				{
					
					nVol = _ttoi(EXP_DAY[i-1].GetBuffer(0));
					nDefaultVol = nVol;
							
					//기존에 예측된 count 적용
					if(0 == nCheckCount)
						nVol -= nLastExpCount;


					//발행 부수만큼 입력
					for(INT j=0;j<nVol;j++)
					{
						m_arrayExpDate.Add(strtmptime);
							
						if(0 == nCheckCount) {

							nUDFLastExpCount = nLastExpCount+j+1;
								
							if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
							else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
							
						}	
						else 
						{
							nUDFLastExpCount = j+1;
							if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
							else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
								
						}

						m_arrayLastExpCount.Add(strtmplastexpcnt);
						nCurMoreExp++;
										
						if(nCurMoreExp == nMoreExp) return 0;

					}

					nCheckCount++;
				}
			}

			if(7 == i) weekcount++;
		}

	}


	return 0;
}


INT CSeDateExpApi::MakeDayTypeArray(INT nMoreExp)
{
	CString lastExpDate,lastExpCount;
	lastExpDate = m_pDM->GetCellData(_T("EXP_START_DATE"),0);
	lastExpCount = m_pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),0);
	INT nLastExpCount = 0;
	if(!lastExpCount.IsEmpty()) nLastExpCount = _ttoi(lastExpCount.GetBuffer(0));
	if(lastExpDate.IsEmpty()) return -1;
	
	const INT cnt = 6;
	CString EXP_DAY[cnt];
	CString EXP_CNT[cnt];
	GetDayTermDay(EXP_DAY,0);
	GetDayBookCnt(EXP_CNT,0);
	
	INT year, month, day;
	year = month = day = -1;
	
	ExtractStringDateToIntDate(lastExpDate, &year, &month, &day);
	COleDateTime lastTime(year, month, day, 0, 0, 0); //예측시작일 

	COleDateTime currentTime = COleDateTime::GetCurrentTime(); //오늘 날자 

	CString strcurrentime;
	strcurrentime.Format(_T("%04d/%02d/%02d"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay());
	
	//먼저 년도를 비교한다
	INT lastYear = year;
	INT lastMonth = month;
	INT	currentYear = currentTime.GetYear();
	CString strDate;
	INT nYearGap = currentYear - lastYear + 1;
	INT nVol = 0;
	INT nCheckCount = 0;
	CString strtmplastexpcnt;
	BOOL IsBreak = FALSE;
	INT nDefaultVol = 0;
	INT nUDFLastExpCount = 0;
	INT nCurSize = 0;
	
	if(lastTime <= currentTime)  //오늘 날짜를 기준으로 하는 기본 예측 
	{
		for(INT i=0;i<nYearGap+1;i++)
		{
			for(INT j=0;j<12;j++)
			{
				for(INT k=0;k<6;k++)
				{
					//발행 부수를 확인
					if(EXP_CNT[k].IsEmpty()) continue;
						
					strDate = _T("");
					strDate = GetDayStringDate(EXP_DAY[k], lastYear + i, j+1);	
					if(strDate.IsEmpty()) continue;
							
					if(CompareStringDates(strDate, lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
					{
						//오늘 날짜와 같거나 이전 값이면 
						if(CompareStringDates(strcurrentime, strDate)>=0)
						{
								
							nVol = _ttoi(EXP_CNT[k].GetBuffer(0));
							nDefaultVol = nVol;	

							//기존에 예측된 count 적용
							if(0 == nCheckCount)
								nVol -= nLastExpCount;

							
							//발행부수만큼 array에 입력 
							for(INT l=0;l<nVol;l++)
							{
								m_arrayExpDate.Add(strDate);
								
								if(0 == nCheckCount) {

									nUDFLastExpCount = nLastExpCount+l+1;
									
									if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
									else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
								
								}	
								else 
								{
									nUDFLastExpCount = l+1;
									if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
									else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
									
								}

								m_arrayLastExpCount.Add(strtmplastexpcnt);

								//발행일의 경우는 총 갯수를 맞춘다 
								if(m_bIsPubExpType)
								{
									nCurSize = m_arrayExpDate.GetSize();
									if(nMoreExp == nCurSize) return 0;
								}
							}

							nCheckCount++;
						
						}
						else 
						{
							IsBreak = TRUE;
							break;
						}

					}
				}

				if(IsBreak) break;
			}

			if(IsBreak) break;
		}

	}
	

	if(m_bIsPubExpType) //발행일
	{
		INT nCurSize = m_arrayExpDate.GetSize();
		
		//발행일의 경우는 입수일과 갯수를 맞춘다
		INT nTotalPubExp = nMoreExp;
		nMoreExp = nTotalPubExp-nCurSize-1;
		
	}
	else //입수일 
	{
		if(0 == nMoreExp && 0 == m_arrayExpDate.GetSize()) return 0;

	}
	
	//최종적으로 한번 더 예측한다.
	nMoreExp++;
	
	INT nCurYear = currentTime.GetYear();
	INT nCurMonth = currentTime.GetMonth();
	CString strCompareDate;
	IsBreak = FALSE;
	INT nCurMoreExp = 0;
	

	while(1)
	{
		for(INT i=0;i<6;i++)
		{
			if(EXP_DAY[i].IsEmpty()) 
			{
				if(5 == i) 
				{
					if(12 == nCurMonth)
					{
						nCurMonth = 1;
						nCurYear++;
					}
					else
					{
						nCurMonth++;
					}	
				}
				continue;
			}

			strCompareDate = GetDayStringDate(EXP_DAY[i], nCurYear, nCurMonth);
			
			if(CompareStringDates(strCompareDate, lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
			{
				if(CompareStringDates(strCompareDate, strcurrentime)>0)
				{
					//예측 권수 
					nVol = _ttoi(EXP_CNT[i].GetBuffer(0));
					nDefaultVol = nVol;	

					//기존에 예측된 count 적용
					if(0 == nCheckCount)
						nVol -= nLastExpCount;
					
					//발행부수만큼 array에 입력 
					for(INT j=0;j<nVol;j++)
					{
						m_arrayExpDate.Add(strCompareDate);
									
						if(0 == nCheckCount) {

							nUDFLastExpCount = nLastExpCount+j+1;
							if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
							else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
									
						}	
						else 
						{
							nUDFLastExpCount = j+1;
							if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
							else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
										
						}
						
						m_arrayLastExpCount.Add(strtmplastexpcnt);
						nCurMoreExp++; 
						if(nCurMoreExp == nMoreExp) return 0;
					}

					nCheckCount++;
				}
			
			}

			if(5 == i) 
			{
				if(12 == nCurMonth)
				{
					nCurMonth = 1;
					nCurYear++;
				}
				else
				{
					nCurMonth++;
				}
			}
		}

	}

	return 0;
}


INT CSeDateExpApi::MakeEtcTypeArray(INT nMoreExp)
{
	CString lastExpDate,lastExpCount;
	lastExpDate = m_pDM->GetCellData(_T("EXP_START_DATE"),0);
	lastExpCount = m_pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),0);
	INT nLastExpCount = 0;
	if(!lastExpCount.IsEmpty()) nLastExpCount = _ttoi(lastExpCount.GetBuffer(0));
	if(lastExpDate.IsEmpty()) return -1; //예측 정보 입력 문제
	
	INT tmpyear,tmpmonth,tmpday;
	tmpyear = tmpmonth = tmpday = 0;
	CString strtmp;
	strtmp = m_pDM->GetCellData(_T("EXP_TERM_YEAR"),0);
	if(!strtmp.IsEmpty()) tmpyear = _ttoi(strtmp.GetBuffer(0));
	strtmp = _T("");	
	
	strtmp = m_pDM->GetCellData(_T("EXP_TERM_MONTH"),0);
	if(!strtmp.IsEmpty()) tmpmonth = _ttoi(strtmp.GetBuffer(0));
	strtmp = _T("");

	strtmp = m_pDM->GetCellData(_T("EXP_TERM_DAY"),0);
	if(!strtmp.IsEmpty()) tmpday = _ttoi(strtmp.GetBuffer(0));
	strtmp = _T("");
	
	if(!tmpyear && !tmpmonth && !tmpday) return -1;

	INT year, month, day;
	year = month = day = -1;
	
	ExtractStringDateToIntDate(lastExpDate, &year, &month, &day);
	COleDateTime lastTime(year, month, day, 0, 0, 0); //예측시작일 
	COleDateTime currentTime = COleDateTime::GetCurrentTime(); //오늘 날자 

	CString strcurrentime;
	strcurrentime.Format(_T("%04d/%02d/%02d"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay());
	

	COleDateTime tmptime;
	tmptime = lastTime;
	CString strDate;
	INT nCheckCount = 0;
	INT nVolDB = 0;
	strtmp = m_pDM->GetCellData(_T("EXP_BOOK_CNT"),0);
	nVolDB = _ttoi(strtmp.GetBuffer(0));
	INT nVol = 0;
	CString strtmplastexpcnt;
	COleDateTimeSpan dif((tmpyear*365) + (tmpmonth*31) + tmpday, 0, 0, 0);
	INT nWhileCount = 0;
	INT nDefaultVol = 0;
	INT nUDFLastExpCount = 0;
	INT nCurSize = 0;


	if(lastTime <= currentTime) //오늘 날짜를 기준으로 하는 기본 예측  
	{
		while(1)
		{
			
			if(nWhileCount) tmptime = GetNextEtcTime(tmptime, tmpyear, tmpmonth, tmpday);
			
			if(tmptime<0) return -1;
			
			strDate.Format(_T("%04d/%02d/%02d"), tmptime.GetYear(), tmptime.GetMonth(), tmptime.GetDay()); 
			
			if(CompareStringDates(strDate, lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
			{
				//오늘 날짜와 같거나 이전 값이면 
				if(CompareStringDates(strcurrentime, strDate)>=0)
				{
								
					nVol = nVolDB;
					nDefaultVol = nVol;

					//기존에 예측된 count 적용
					if(0 == nCheckCount)
						nVol -= nLastExpCount;

					//발행부수만큼 array에 입력 
					for(INT i=0;i<nVol;i++)
					{
						m_arrayExpDate.Add(strDate);
								
						if(0 == nCheckCount) {

							nUDFLastExpCount = nLastExpCount+i+1;
							if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
							else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
								
						}	
						else 
						{

							nUDFLastExpCount = i+1;
							if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
							else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
			
						}

						m_arrayLastExpCount.Add(strtmplastexpcnt);

						//발행일의 경우는 총 갯수를 맞춘다 
						if(m_bIsPubExpType)
						{
							nCurSize = m_arrayExpDate.GetSize();
							if(nMoreExp == nCurSize) return 0;
						}
					}	
					
					nCheckCount++;
				}
				else
				{
					break;
				}
			}

			nWhileCount++;
		}
	}

	if(m_bIsPubExpType) //발행일
	{
		INT nCurSize = m_arrayExpDate.GetSize();
		
		//발행일의 경우는 입수일과 갯수를 맞춘다
		INT nTotalPubExp = nMoreExp;
		nMoreExp = nTotalPubExp-nCurSize-1;
		
	}
	else //입수일 
	{
		if(0 == nMoreExp && 0 == m_arrayExpDate.GetSize()) return 0;

	}

	//최종적으로 한번 더 예측한다.
	nMoreExp++;
	
	INT nCurYear = currentTime.GetYear();
	INT nCurMonth = currentTime.GetMonth();
	CString strCompareDate;
	INT comparecount = 0;
	INT nCurMoreExp = 0;


	while(1)
	{
		
		if(0 != comparecount) tmptime = GetNextEtcTime(tmptime, tmpyear, tmpmonth, tmpday);

		strCompareDate.Format(_T("%04d/%02d/%02d"), tmptime.GetYear(), tmptime.GetMonth(), tmptime.GetDay()); 
	
		if(CompareStringDates(strCompareDate, lastExpDate)>=0) //예측시작일과 같거나 그 보다 나중이고  
		{

			if(CompareStringDates(strCompareDate, strcurrentime)>0)
			{
				nVol = nVolDB;
				nDefaultVol = nVol;

				//기존에 예측된 count 적용
				if(0 == nCheckCount)
					nVol -= nLastExpCount;		
				
				//발행부수만큼 array에 입력 
				for(INT i=0;i<nVol;i++)
				{
					m_arrayExpDate.Add(strCompareDate);
									
					if(0 == nCheckCount) {

						nUDFLastExpCount = nLastExpCount+i+1;
						if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
						else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
									
					}	
					else 
					{

						nUDFLastExpCount = i+1;
						if(nDefaultVol == nUDFLastExpCount) strtmplastexpcnt = _T("0");
						else strtmplastexpcnt.Format(_T("%d"),nUDFLastExpCount);
				
					}

					m_arrayLastExpCount.Add(strtmplastexpcnt);
					nCurMoreExp++;
					if(nMoreExp == nCurMoreExp) return 0;
				}	
						
				nCheckCount++;
			}

		}

		comparecount++;
	}



	return 0;
}

COleDateTime CSeDateExpApi::GetNextEtcTime(COleDateTime lasttime, INT year, INT month, INT day)
{
	COleDateTimeSpan dif1(year*365,0,0,0);
	COleDateTimeSpan dif2(day,0,0,0);

	COleDateTime tmptime;
	tmptime = lasttime;

	//year gap
	tmptime += dif1;

	//month gap
	INT nYear,nMonth,nDay;
	nYear =  tmptime.GetYear();
	nMonth = tmptime.GetMonth();
	
	if(month>0)
	{
		nMonth += month;

		if(nMonth>12)
		{
			nYear+=1;
			nMonth-=12;
		}
	}
	
	
	nDay = tmptime.GetDay();
	
	COleDateTime resulttime(nYear, nMonth, nDay, 0, 0, 0);

	//day gap
	resulttime += dif2;

	return resulttime;
}


INT CSeDateExpApi::GetNextDate(CString &strDate)
{
	strDate = m_arrayExpDate.GetAt(m_nCount);
	if(strDate.IsEmpty()) return -1;

	m_nCount++;
	return 0;
}


INT CSeDateExpApi::CompareStringDates(CString date1, CString date2)
{
	if(date1.GetLength() == 0 || date2.GetLength() == 0) return 0;
	INT year, month, day;
	ExtractStringDateToIntDate(date1, &year, &month, &day);
	COleDateTime t1(year, month, day, 0, 0, 0);
	ExtractStringDateToIntDate(date2, &year, &month, &day);
	COleDateTime t2(year, month, day, 0, 0, 0);
	if(t1 > t2) return 1;
	else if(t1 == t2) return 0;
	return -1;

}

VOID CSeDateExpApi::ExtractStringDateToIntDate(CString	lastExpDate, INT *year, INT *month, INT *day)
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

VOID CSeDateExpApi::GetMonthTermDay(CString data[] ,INT nRowIdx)
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

	m_pDM->GetCellData(alias,12,data,nRowIdx);
}

VOID CSeDateExpApi::GetMonthBookCnt(CString data[],INT nRowIdx)
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
	
	m_pDM->GetCellData(alias,12,data,nRowIdx);
}

VOID CSeDateExpApi::GetDayTermDay(CString data[] ,INT nRowIdx)
{
	const INT cnt = 6;
	CString alias[cnt] = {
		_T("EXP_TERM_DATE1"),
		_T("EXP_TERM_DATE2"),
		_T("EXP_TERM_DATE3"),
		_T("EXP_TERM_DATE4"),
		_T("EXP_TERM_DATE5"),
		_T("EXP_TERM_DATE6")};

	m_pDM->GetCellData(alias,6,data,nRowIdx);
}

VOID CSeDateExpApi::GetDayBookCnt(CString data[],INT nRowIdx)
{

	const INT cnt = 6;
	CString alias[cnt] = {
		_T("EXP_BOOK_CNT1"),
		_T("EXP_BOOK_CNT2"),
		_T("EXP_BOOK_CNT3"),
		_T("EXP_BOOK_CNT4"),
		_T("EXP_BOOK_CNT5"),
		_T("EXP_BOOK_CNT6")};
	
	m_pDM->GetCellData(alias,6,data,nRowIdx);
}

CString CSeDateExpApi::GetMonthStringDate(CString data[], INT year, INT month)
{
	CString stringdate = _T("");
	
	CString day = _T("");
	day = data[month-1]; 
	if(day.IsEmpty()) return _T("");
	INT nday = _ttoi(day.GetBuffer(0));

	stringdate.Format(_T("%04d/%02d/%02d"), year, month, nday);
	return stringdate;
}

CString CSeDateExpApi::GetDayStringDate(CString day, INT year, INT month)
{
	CString stringdate = _T("");

	if(day.IsEmpty()) return _T("");
	INT nday = _ttoi(day.GetBuffer(0));
	stringdate.Format(_T("%04d/%02d/%02d"), year, month, nday);
	return stringdate;
}

VOID CSeDateExpApi::GetWeekCnt(CString data[] ,INT nRowIdx)
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
	
	m_pDM->GetCellData(alias,7,data,nRowIdx);
}