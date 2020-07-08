// LocStatisticsApi.cpp: implementation of the CLocStatisticsApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocStatisticsApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocStatisticsApi::CLocStatisticsApi(CESL_Mgr* pParentMgr)
{
	m_pMgr = pParentMgr;
	m_pDM_QUERY = NULL;
	m_pDM_RESULT = NULL;
	m_pDM_SUM = NULL;
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/
	m_pDM_LIST = NULL;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_strWhere = _T("");
	m_strExtraWhere = _T("");
	m_strClassField = _T("");

	m_bWonbuCondition = FALSE;
	m_nTYPE = 1; //default
	m_arrayWorkingStatus.RemoveAll();

}

CLocStatisticsApi::~CLocStatisticsApi()
{

}

// 달력이 나오는 날짜 컨트롤의 값 가져오기
CString CLocStatisticsApi::GetDate( CESL_Mgr* pEslMgr , UINT nCtrlID )
{
	CString sDate;
	
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )pEslMgr->GetDlgItem( nCtrlID );
	
	CTime tTime;
	
	DWORD dwResult = pDate->GetTime( tTime );

	if (dwResult != GDT_VALID ) return _T("");

	sDate.Format( _T("%04d/%02d/%02d"), tTime.GetYear(), tTime.GetMonth(), tTime.GetDay() );
	
	return sDate;
}

INT CLocStatisticsApi::RunStatisticsProc(CString strClassName)
{

	if(!m_pDM_QUERY || !m_pDM_RESULT || !m_pDM_SUM) 
	{
		AfxMessageBox(_T("관련 데이터 객체를 찾을 수 없습니다!"));
		return -1;
	}
	

	
	INT ids = 0;
	
	//DM을 설정하고 
	ids = SetQueryDM(strClassName);
	if(ids<0) return -1;

	//데이터를 가져온후 
	ids = RefreshQueryDM();
	if(ids<0) return -1;

	//결과 DM에 구성
	ids = MakeResultDM(strClassName);
	if(ids<0) return -1;

	
	return 0;
}

INT CLocStatisticsApi::SetQueryDM(CString strClassName)
{
	//TBL_NAME 
	if(m_bWonbuCondition)
	{
		
		m_pDM_QUERY->TBL_NAME = _T("IDX_SE_TBL I,SE_SPECIES_TBL S, SE_BOOK_TBL B,")
								_T(" SE_ACQ_TYPE_VOL_TBL V, CO_ACCESSION_REC_TBL W");

	}
	else
	{
		m_pDM_QUERY->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_BOOK_TBL B, SE_ACQ_TYPE_VOL_TBL V");
	}

	m_pDM_SUM->TBL_NAME = m_pDM_QUERY->TBL_NAME;
	
	
	//행조건 
	INT nColIdx;
	nColIdx = m_pDM_QUERY->FindColumn(_T("행조건"));
	

	//기타 조건
	if(_T("간행빈도코드") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("행조건"),_T("I.PUB_FREQ"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), I.PUB_FREQ");
		m_strClassField = _T("I.PUB_FREQ");
		
	}
	else if(_T("자료구분") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("행조건"),_T("I.MAT_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), I.MAT_CODE");
		m_strClassField = _T("I.MAT_CODE");
	}
	else if(_T("등록구분") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("행조건"),_T("B.REG_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.REG_CODE");
		m_strClassField = _T("B.REG_CODE");

	}
	else if(_T("자료실구분") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("행조건"),_T("B.SHELF_LOC_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.SHELF_LOC_CODE");	
		m_strClassField = _T("B.SHELF_LOC_CODE");

	}
	else if(_T("별치기호") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("행조건"),_T("B.SEPARATE_SHELF_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.SEPARATE_SHELF_CODE");		
		m_strClassField = _T("B.SEPARATE_SHELF_CODE");
	}
	else if(_T("배가상태") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("행조건"),_T("B.WORKING_STATUS"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.WORKING_STATUS");		
		m_strClassField = _T("B.WORKING_STATUS");
	}

	return 0;
}

INT CLocStatisticsApi::RefreshQueryDM()
{
	m_pDM_QUERY->RefreshDataManager(m_strWhere);
	
	INT nRowCnt = m_pDM_QUERY->GetRowCount();
	
	//===================================================
	//2010.01.11 ADD BY PJW : 완료되었습니다를 뿌려준다.
//	if(0 == nRowCnt)
//	{
//		AfxMessageBox(_T("검색 결과가 없습니다!"));
//		return -1;
//	}
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("검색 결과가 없습니다!"));
		return -1;
	}
	else
	{
		AfxMessageBox(_T("완료되었습니다!"));
	}
	//===================================================
	

	return 0;
}

INT CLocStatisticsApi::MakeResultDM(CString strClassName)
{
	m_pDM_RESULT->FreeData();
	
	//결과DM의 열조건을 setting한다
	//결과 DM의 첫 row에는 각 열의 header(분류기호)를 넣는다
	INT nKDCCode = 256;
	INT nDefaultValue = 0;
	CString strKDCCode[256], strKDCDesc[256];
	nDefaultValue = 0;

	m_pMgr->m_pInfo->pCodeMgr->GetCode(_T("KDC대주제부호관리"), strKDCCode, strKDCDesc, nKDCCode, nDefaultValue);	
	INT nKDCCodeCnt = m_pMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("KDC대주제부호관리"));

	INT idx = 0;
	m_pDM_RESULT->InsertRow(-1);
	CString strAlias;
	
	for(INT i=0;i<nKDCCodeCnt;i++)
	{
		
		strAlias.Format(_T("%d"),i+1);
		m_pDM_RESULT->SetCellData(strAlias,strKDCCode[i],idx);
	}

	m_pDM_RESULT->SetCellData(_T("합계"),_T("계"),idx);
	

	//결과DM의 행조건을 setting한다
	if(_T("배가상태") == strClassName) //배가 상태의 경우 
	{
		INT nSize = m_arrayWorkingStatus.GetSize();
		for(INT i=0;i<nSize;i++)
		{
			m_pDM_RESULT->InsertRow(-1);
			idx = m_pDM_RESULT->GetRowCount()-1;
			m_pDM_RESULT->SetCellData(_T("행조건"),m_arrayWorkingStatus.GetAt(i),idx);
		}

	}
	else //일반적인 경우 
	{
		INT nCode = 256;
		CString strCode[256], strDesc[256];
		nDefaultValue = 0;

		
		m_pMgr->m_pInfo->pCodeMgr->GetCode(strClassName, strCode, strDesc, nCode, nDefaultValue);	
		INT nCodeCnt = m_pMgr->m_pInfo->pCodeMgr->GetCodeCount(strClassName);
		
		
		for(i=0;i<nCodeCnt;i++)
		{
			m_pDM_RESULT->InsertRow(-1);
			idx = m_pDM_RESULT->GetRowCount()-1;
			m_pDM_RESULT->SetCellData(_T("행조건"),strCode[i],idx);
		}
	}

	//결과DM에 쿼리DM을 카피한다 
	CopyQueryDMToResultDM();

	//결과 DM의 마지막 바로 앞 row에는 중복허용 합계(각행 및 열을 더한값)를 넣는다.
	SetDuplicatedSumInResultDM();

	//결과 DM의 마지막 row에는 행열 조건에 따른 합계 query결과를 넣는다
	SetQuerySumInResultDM();

	//결과DM의 코드를 설명으로 바꾼다
	ConvertCodeToDescInResultDM(strClassName);


	return 0;
}
VOID CLocStatisticsApi::CopyQueryDMToResultDM()
{
	INT nResultRow = m_pDM_RESULT->GetRowCount();
	INT nQueryRowCnt = m_pDM_QUERY->GetRowCount();
	CString strRow;
	CString strColumn;
	CString strResultRow;
	CString strResultColumn;
	CString strAlias;


	for(INT i=0;i<nQueryRowCnt;i++)
	{
		
		strRow = _T("");
		strColumn = _T("");
		
		strRow = m_pDM_QUERY->GetCellData(_T("행조건"),i);
		strColumn = m_pDM_QUERY->GetCellData(_T("분류기호"),i);
		
	
		for(INT j=1;j<nResultRow;j++)
		{
			strResultRow = m_pDM_RESULT->GetCellData(_T("행조건"),j);
			
			if(strRow == strResultRow)
			{
				for(INT k=1;k<12;k++)
				{
					strAlias.Format(_T("%d"),k);
					strResultColumn = m_pDM_RESULT->GetCellData(strAlias,0);

					if(strColumn == strResultColumn)
					{
						SetDataInResultDM(i,j,strAlias);
					}
				}

			}
		
		}
	}

}


VOID CLocStatisticsApi::SetDataInResultDM(INT nQueryRow, INT nResultRow, CString strColumnAlias)
{
	CString strSpeciesCnt,strVolCnt,strBookCnt;

	strSpeciesCnt = m_pDM_QUERY->GetCellData(_T("종수"),nQueryRow);
	strVolCnt = m_pDM_QUERY->GetCellData(_T("권수"),nQueryRow);
	strBookCnt = m_pDM_QUERY->GetCellData(_T("책수"),nQueryRow);

	CString strFormat;
	strFormat.Format(_T("%s,%s,%s"),strSpeciesCnt,strVolCnt,strBookCnt);
	m_pDM_RESULT->SetCellData(strColumnAlias,strFormat,nResultRow);
}

VOID CLocStatisticsApi::SetDuplicatedSumInResultDM()
{
	//중복 column합계를 구한다
	INT nSpeciesSum,nVolSum,nBookSum;
	nSpeciesSum = nVolSum = nBookSum = 0;
	
	INT nRowCnt = m_pDM_RESULT->GetRowCount();
	CString strAlias;
	CString strData;
	CString strFormat;

	for(INT i=1;i<nRowCnt;i++)
	{
		for(INT j=1;j<12;j++)
		{
			strAlias.Format(_T("%d"),j);
			strData	= m_pDM_RESULT->GetCellData(strAlias,i);
			strData.TrimLeft();
			strData.TrimRight();
			
			if(!strData.IsEmpty()) 
			{
				GetSumTotal(strData,&nSpeciesSum,&nVolSum,&nBookSum);
			}
		
		}
		
		strFormat.Format(_T("%d,%d,%d"),nSpeciesSum,nVolSum,nBookSum);
		m_pDM_RESULT->SetCellData(_T("중복합계"),strFormat,i);
		nSpeciesSum = nVolSum = nBookSum = 0;
	}

	
	
	//결과 DM에 중복 row 합계 용으로 한 row 추가
	nSpeciesSum = nVolSum = nBookSum = 0;
	m_pDM_RESULT->InsertRow(-1);
	INT nIdx = m_pDM_RESULT->GetRowCount()-1;
	
	for(i=1;i<12;i++)
	{
		strAlias.Format(_T("%d"),i);
		
		for(INT j=1;j<nRowCnt;j++)
		{
			strData	= m_pDM_RESULT->GetCellData(strAlias,j);
			strData.TrimLeft();
			strData.TrimRight();

			if(!strData.IsEmpty()) 
			{
				GetSumTotal(strData,&nSpeciesSum,&nVolSum,&nBookSum);
			}

		}

		strFormat.Format(_T("%d,%d,%d"),nSpeciesSum,nVolSum,nBookSum);
		m_pDM_RESULT->SetCellData(strAlias,strFormat,nIdx);
		nSpeciesSum = nVolSum = nBookSum = 0;
	}
	

	//총 합계추가
	nSpeciesSum = nVolSum = nBookSum = 0;

	for(i=1;i<nRowCnt;i++)
	{
		strData	= m_pDM_RESULT->GetCellData(_T("중복합계"),i);
		strData.TrimLeft();
		strData.TrimRight();

		if(!strData.IsEmpty()) 
		{
			GetSumTotal(strData,&nSpeciesSum,&nVolSum,&nBookSum);
		}

	}
	
	strFormat.Format(_T("%d,%d,%d"),nSpeciesSum,nVolSum,nBookSum);
	m_pDM_RESULT->SetCellData(_T("중복합계"),strFormat,nIdx);

}

VOID CLocStatisticsApi::SetQuerySumInResultDM()
{

	//각 열, 행의 합계를 퀴리를 통해서 중복없이 구한다
	//(행,열로 각각 group by 후 total을 구한다)
	CString strSpeciesCnt,strVolCnt,strBookCnt;
	CString strFormat;



	//GROUP 설정
	INT nColIdx;
	nColIdx = m_pDM_SUM->FindColumn(_T("GROUP"));
	
	
	//column(분류기호)별 통계
	CString strCode;
	CString strColumn;
	m_pDM_SUM->SetDataMgrRef(nColIdx,_T("GROUP"),_T("SUBSTR (B.CLASS_NO, 0, 1)"),_T("str"),_T(""),_T(""));
	m_pDM_SUM->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1)");
	m_pDM_SUM->RefreshDataManager(m_strWhere);

	INT nRowCnt = m_pDM_SUM->GetRowCount();

	CString strAlias;
	m_pDM_RESULT->InsertRow(-1);
	INT nResultIdx = m_pDM_RESULT->GetRowCount()-1;

	if(nRowCnt>0)
	{
		for(INT i=0;i<nRowCnt;i++)
		{
			strCode = m_pDM_SUM->GetCellData(_T("GROUP"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			
			for(INT j=1;j<12;j++)
			{
				strAlias.Format(_T("%d"),j);

				strColumn = m_pDM_RESULT->GetCellData(strAlias,0); //column header
				strColumn.TrimLeft();
				strColumn.TrimRight();

				if(strCode == strColumn)
				{
					strSpeciesCnt = m_pDM_SUM->GetCellData(_T("종수"),i);
					strVolCnt = m_pDM_SUM->GetCellData(_T("권수"),i);
					strBookCnt = m_pDM_SUM->GetCellData(_T("책수"),i);
						
					strFormat.Format(_T("%s,%s,%s"),strSpeciesCnt,strVolCnt,strBookCnt);
					m_pDM_RESULT->SetCellData(strAlias,strFormat,nResultIdx);

				}

			}
		

		}
	}

	
	//row(조건)별 통계
	CString strExtraCondition;
	strExtraCondition.Format(_T("GROUP BY %s"),m_strClassField);
		
	m_pDM_SUM->SetDataMgrRef(nColIdx,_T("GROUP"),m_strClassField,_T("str"),_T(""),_T(""));
	m_pDM_SUM->EXTRA_CONDITION = strExtraCondition;
	m_pDM_SUM->RefreshDataManager(m_strWhere);

	nRowCnt = m_pDM_SUM->GetRowCount();
	INT nResultRowCnt = m_pDM_RESULT->GetRowCount()-2;


	if(nRowCnt>0)
	{
		for(INT i=0;i<nRowCnt;i++)
		{
			strCode = m_pDM_SUM->GetCellData(_T("GROUP"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			
			for(INT j=1;j<nResultRowCnt;j++)
			{
				strColumn = m_pDM_RESULT->GetCellData(_T("행조건"),j);
				strColumn.TrimLeft();
				strColumn.TrimRight();

				if(strCode == strColumn)
				{
					strSpeciesCnt = m_pDM_SUM->GetCellData(_T("종수"),i);
					strVolCnt = m_pDM_SUM->GetCellData(_T("권수"),i);
					strBookCnt = m_pDM_SUM->GetCellData(_T("책수"),i);
					
					strFormat.Format(_T("%s,%s,%s"),strSpeciesCnt,strVolCnt,strBookCnt);
					m_pDM_RESULT->SetCellData(_T("합계"),strFormat,j);

				}

			}
		

		}
	}


	//최종 합계 
	CString strQuery;
	CString strResult[3];
	strQuery.Format(_T("SELECT COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT V.REC_KEY),COUNT(DISTINCT B.REC_KEY)")
					_T(" FROM %s WHERE %s"),m_pDM_SUM->TBL_NAME,m_strWhere);

	m_pDM_SUM->GetValuesQuery(strQuery,strResult,3);
				
	strFormat.Format(_T("%s,%s,%s"),strResult[0],strResult[1],strResult[2]);
	
	INT nLastIdx = m_pDM_RESULT->GetRowCount()-1;
	m_pDM_RESULT->SetCellData(_T("합계"),strFormat,nLastIdx);
}


VOID CLocStatisticsApi::ConvertCodeToDescInResultDM(CString strClassName)
{

	//행 
	if(strClassName == _T("배가상태"))
	{
		INT nRowCnt = m_pDM_RESULT->GetRowCount();
		CString strStatus;
		
		for(INT i=0;i<nRowCnt;i++)
		{
			strStatus = m_pDM_RESULT->GetCellData(_T("행조건"),i);
			strStatus =	GetWorkingStatusDesc(strStatus);
			if(!strStatus.IsEmpty()) m_pDM_RESULT->SetCellData(_T("행조건"),strStatus,i);
		}
	}
	else
	{
		m_pMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM_RESULT,strClassName,_T("행조건"));
	}
	
	CString strAlias;
	CString strCode,strDesc;
	
	//열
	for(INT i=1;i<12;i++)
	{
		strAlias.Format(_T("%d"),i);
		strCode = m_pDM_RESULT->GetCellData(strAlias,0);
		m_pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"),strCode,strDesc);
		m_pDM_RESULT->SetCellData(strAlias,strDesc,0);

	}


}


VOID CLocStatisticsApi::GetSumTotal(CString strData,INT* nSpeciesSum,INT* nVolSum,INT* nBookSum)
{
	if(strData.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strData.GetLength();
	idx = strData.Find(_T(","), startidx);

	//종 
	if(idx > 0) *nSpeciesSum += _ttoi((strData.Left(idx)).GetBuffer(0));

	startidx = idx + 1;
	idx = strData.Find(_T(","), startidx);
	
	//권
	if(idx > 0)	*nVolSum += _ttoi((strData.Mid(startidx, idx - 1)).GetBuffer(0));

	startidx = idx + 1;
	
	//책
	*nBookSum += _ttoi((strData.Right(lastidx - startidx)).GetBuffer(0));
}

CString CLocStatisticsApi::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CLocStatisticsApi::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = %s"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN %s AND %s"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CLocStatisticsApi::GetWorkingStatusDesc(CString strStatus)
{
	if(strStatus.IsEmpty()) return _T("");
	
	CString strtmp;

	if( strStatus.Compare(_T("SEL311O")) == 0 )
	{
		strtmp.Format( _T("관외대출자료") );
	}
	else if( strStatus.Compare(_T("SEL312O")) == 0 )
	{
		strtmp.Format( _T("관내대출자료") );
	}
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
	// 18.09.27 JMJ KOL.RED.2018.008
	else if( strStatus.Compare(_T("SEL411O")) == 0 )
	{
		strtmp.Format( _T("연속책두레업무진행자료") );
	}
	else if( strStatus.Compare(_T("SEL511O")) == 0 )
	{
		strtmp.Format( _T("통합도서타관반납") );
	}
	//=====================================================
	else if( strStatus.Compare(_T("SEL313O")) == 0 )
	{
		strtmp.Format( _T("특별대출자료") );
	}
	else if( strStatus.Compare(_T("SEL314O")) == 0 )
	{
		strtmp.Format( _T("긴급대출자료") );
	}
	else if( strStatus.Compare(_T("SEL315O")) == 0 )
	{
		strtmp.Format( _T("배달자료") );
	}
	else if( strStatus.Compare(_T("SEL212N")) == 0 )
	{
		strtmp.Format( _T("비치자료 ") );
	}
	else if( strStatus.Compare(_T("SEL213O")) == 0 )
	{
		strtmp.Format( _T("수리제본자료") );
	}
	else if( strStatus.Compare(_T("SEL216O")) == 0 )
	{
		strtmp.Format( _T("분실자료") );
	}
	else if( strStatus.Compare(_T("SEL215O")) == 0 )
	{
		strtmp.Format( _T("제적자료") );
	}
	else if( strStatus.Compare(_T("SEL214O")) == 0 )
	{
		strtmp.Format( _T("파손자료") );
	}
	else if( strStatus.Compare(_T("SEL112N")) == 0 )
	{
		strtmp.Format( _T("실시간배가자료") );
	}

	return strtmp;
}

// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/
INT CLocStatisticsApi::RunStatisticsListProc(CString strClassName, CString sXValue, CString sYValue)
{
	if(!m_pDM_QUERY || !m_pDM_RESULT || !m_pDM_SUM) 
	{
		AfxMessageBox(_T("관련 데이터 객체를 찾을 수 없습니다!"));
		return -1;
	}
	
	//TBL_NAME 
	if(m_bWonbuCondition)
	{		
		m_pDM_LIST->TBL_NAME = _T("IDX_SE_TBL I,SE_SPECIES_TBL S, SE_BOOK_TBL B,")
								_T(" SE_ACQ_TYPE_VOL_TBL V, CO_ACCESSION_REC_TBL W");
	}
	else
	{
		m_pDM_LIST->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_BOOK_TBL B, SE_ACQ_TYPE_VOL_TBL V");
	}
	
		
	CString strWhereQuery = MakeSearcListQuery(strClassName, sXValue, sYValue);
	m_pDM_LIST->RefreshDataManager(strWhereQuery);

	return 0;
}

CString CLocStatisticsApi::MakeSearcListQuery(CString strClassName, CString sXValue, CString sYValue)
{
	CString strExtraWhere = _T("");
	CString sTmpWhere;
	INT ids;

	//열
	m_pMgr->m_pInfo->pCodeMgr->ConvertDescToCode(_T("KDC대주제부호관리"), sXValue, sXValue);
	if(sXValue != _T("합계"))
	{
		if(!sXValue.IsEmpty())
		{
			sTmpWhere.Format( _T(" AND SUBSTR (B.CLASS_NO, 0, 1)='%s'"), sXValue);
		}
		else
		{
			sTmpWhere.Format( _T(" AND SUBSTR (B.CLASS_NO, 0, 1) IS NULL"));
		}	
		strExtraWhere += sTmpWhere;
	}

	//행 
	if(strClassName == _T("배가상태"))
	{
		sYValue = GetWorkingStatusDescToCode(sYValue);
	}
	else
	{
		ids = m_pMgr->m_pInfo->pCodeMgr->ConvertDescToCode(strClassName , sYValue, sYValue);
	}
	if(sYValue != _T("합계"))
	{
		if(!sYValue.IsEmpty())
		{
			sTmpWhere.Format( _T(" AND %s='%s'"), m_strClassField, sYValue);
		}
		else
		{
			sTmpWhere.Format( _T(" AND %s IS NULL"), m_strClassField);
		}	
		strExtraWhere += sTmpWhere;
	}

	return m_strWhere + strExtraWhere;
}
CString CLocStatisticsApi::GetWorkingStatusDescToCode(CString strStatus)
{
	if(strStatus.IsEmpty()) return _T("");
	
	CString strtmp;

	if( strStatus.Compare(_T("관외대출자료")) == 0 )
	{
		strtmp.Format( _T("SEL311O") );
	}
	else if( strStatus.Compare(_T("관내대출자료")) == 0 )
	{
		strtmp.Format( _T("SEL312O") );
	}
	else if( strStatus.Compare(_T("통합도서타관반납")) == 0 )
	{
		strtmp.Format( _T("SEL511O") );
	}
	else if( strStatus.Compare(_T("특별대출자료")) == 0 )
	{
		strtmp.Format( _T("SEL313O") );
	}
	else if( strStatus.Compare(_T("긴급대출자료")) == 0 )
	{
		strtmp.Format( _T("SEL314O") );
	}
	else if( strStatus.Compare(_T("배달자료")) == 0 )
	{
		strtmp.Format( _T("SEL315O") );
	}
	else if( strStatus.Compare(_T("비치자료")) == 0 )
	{
		strtmp.Format( _T("SEL212N") );
	}
	else if( strStatus.Compare(_T("수리제본자료")) == 0 )
	{
		strtmp.Format( _T("SEL213O") );
	}
	else if( strStatus.Compare(_T("분실자료")) == 0 )
	{
		strtmp.Format( _T("SEL216O") );
	}
	else if( strStatus.Compare(_T("제적자료")) == 0 )
	{
		strtmp.Format( _T("SEL215O") );
	}
	else if( strStatus.Compare(_T("파손자료")) == 0 )
	{
		strtmp.Format( _T("SEL214O") );
	}
	else if( strStatus.Compare(_T("실시간배가자료")) == 0 )
	{
		strtmp.Format( _T("SEL112N") );
	}

	return strtmp;
}

INT CLocStatisticsApi::SetDataMgrRefList( CESL_DataMgr *pDM, CString sCondition, CString sInitValue, CString sGroupYN, INT nCol )
{
	EFS_BEGIN

	CString sUdf;

	if( sGroupYN.Compare(_T("Y")) == 0 ) sUdf = _T("");
	else sUdf = _T("udf");

	POSITION pos = pDM->RefList.FindIndex(nCol);
	if (pos == NULL) return -1;

	CESL_DataMgr::reference * pRef = (CESL_DataMgr::reference*)pDM->RefList.GetAt(pos);
	if (pRef == NULL) return -2;

	pRef->INIT_VALUE = sInitValue;
	pRef->DATA_TYPE = sUdf;
	pRef->FIELD_NAME = sCondition;

	return 0;

	EFS_END
	return -1;

}				
/*//END------------------------------------------------------------------------*/
// KOL.RED.2018.211 ---------------------------------------------------------------------------