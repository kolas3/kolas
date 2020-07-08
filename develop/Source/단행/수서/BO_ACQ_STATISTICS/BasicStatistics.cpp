// BasicStatistics.cpp: implementation of the CBasicStatistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicStatistics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CBasicStatistics::CBasicStatistics(UINT IDD, CESL_Mgr * pParentMgr):CESL_Mgr(IDD, pParentMgr)
{
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDataProcessingDlg = NULL;
	m_pStatCondition = NULL;
	m_pGroupCollection		= NULL;
	///통계자료를 구성하는 쓰레드의 동작을 제어하는 변수
	m_bRunning = FALSE;
	m_bDonatorFlag	= FALSE;
	m_nStartRow = 0;
}

// 소멸자
CBasicStatistics::~CBasicStatistics()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

// Progress Ctrl Dlg
VOID CBasicStatistics::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL)
	{
		m_pDataProcessingDlg = new CDataProcessingDlg(this);
	}
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
}

// 통계구축
VOID CBasicStatistics::GetAcqStatistics()
{
	CString strMsg;
	if((m_pStatCondition->GetStatType()).IsEmpty())
		strMsg.Format(_T("통계자료를 구축하고 있습니다."));
	else
		strMsg.Format(_T("%s를 구축하고 있습니다."), m_pStatCondition->GetStatType());
	m_pDataProcessingDlg->InitPos();
	m_pDataProcessingDlg->SetMessage(strMsg);

	EvalueateStatistics();
	
	InsertSumRowToTargetDM(m_nStartRow);
	
	InsertPriceSumRowToTargetDM(m_nStartRow);
	
	if( m_bRunning == FALSE )	return ;
	m_pDataProcessingDlg->EndDialog(IDOK);
	if( m_pGrid == NULL )	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	else ::PostMessage(this->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - CStaticsCondition에서 구축된 기본 쿼리에 세로줄 조건을 추가하여 쿼리문을 완성한다.
//
// [ 미니스펙 ]
// 1. 임시DM의 접속정보를 설정한다.
// 2. 기증자별 통계라면 기증자ID와 렉키를 가져와 쿼리를 완료한다.
// 3. 그외의 모든 통계들은 코드값을 읽어와 쿼리문을 완성한다.
//
//////////////////////////////////////////////////////////////////////
INT CBasicStatistics::EvalueateStatistics()
{
	if( m_bRunning == FALSE )	 return 0;
	
	//================================================================
	// 변수정의
	//================================================================
	INT codeCnt, curPos, i, startrow = -1;
	CString code, desc, tmpquery, stracqCode, strOption, strResult;
	CString codes[6];
	CESL_DataMgr tmpDM, donatorDM;
	

	//================================================================
	// 1. 임시DM의 접속정보를 설정한다.
	//================================================================
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	donatorDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);


	//================================================================
	// 2. 기증자별 통계라면 기증자ID와 렉키를 가져와 쿼리를 완료한다.
	// 2005.11.16 국방대용 추가
	//================================================================
	if( m_bDonatorFlag == TRUE )
	{
		donatorDM.RestructDataManager(_T("SELECT REC_KEY, DONATOR_ID, DONATOR FROM CO_DONATOR_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE ORDER BY DONATOR"));
		codeCnt = donatorDM.GetRowCount();
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, codeCnt);
		curPos = m_pDataProcessingDlg->GetProgressCtrl()->GetPos();
		for( i=-1; i<codeCnt; i++)
		{
			if( m_bRunning == FALSE )	return 0;
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++curPos);

			if( i==-1 ) 
			{
				strOption.Format(_T(" AND ( %s IS NULL OR NOT %s IN (select rec_key from co_donator_tbl) ) AND MANAGE_CODE = UDF_MANAGE_CODE"), m_pStatCondition->GetVerticalField(), m_pStatCondition->GetVerticalField());
				desc = _T("적용안함");
			}
			else
			{
				strOption.Format(_T(" AND %s = %s AND MANAGE_CODE = UDF_MANAGE_CODE" ), m_pStatCondition->GetVerticalField(), donatorDM.GetCellData(i,0));
				desc = donatorDM.GetCellData(i,2) + _T("  [") + donatorDM.GetCellData(i,1) + _T("]");
			}
			tmpquery = m_pStatCondition->GetWhereStmt() + strOption + _T(" GROUP BY p.SUBJECT_CODE ");	
			tmpDM.RestructDataManager(tmpquery);			
			CopyTargetDM(&tmpDM, desc, m_pStatCondition->GetAcqType());
		}
	}
	//================================================================
	// 3. 그외의 모든 통계들은 코드값을 읽어와 쿼리문을 완성한다.
	//================================================================
	else
	{
		// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		CString strTempQuery;
		codeCnt = m_pStatCondition->GetVerticalDescCount();
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, codeCnt);
		curPos = m_pDataProcessingDlg->GetProgressCtrl()->GetPos();
		startrow = 0;
		for( i=0 ; i<codeCnt ; i++ )
		{
			if( m_bRunning == FALSE )	 return 0;
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++curPos);
			//===================================================
			//2010.04.26 UPDATE BY PJW : 코드를 선정된 것들만 가져오도록 수정함
//			startrow = m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), codes, 6, ++startrow);
//			code = codes[2];
//			desc = codes[3];
			m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), _T("코드"), code, startrow);
			startrow = m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), _T("설명"), desc, startrow);
			++startrow;
			//===================================================
			
			code.TrimLeft(); code.TrimRight();
			desc.TrimLeft(); desc.TrimRight();
			if(code.IsEmpty()) 
			{
				strOption.Format(_T(" AND (%s = '%s' OR %s IS NULL)"), m_pStatCondition->GetVerticalField(), code, m_pStatCondition->GetVerticalField());
			}
			else
			{
				strOption.Format(_T(" AND %s = '%s'"), m_pStatCondition->GetVerticalField(), code);
			}

			strTempQuery = m_pStatCondition->GetWhereStmt();

			if ( strTempQuery.Find(_T("BO_PURCHASEINFO_TBL")) > 0 || strTempQuery.Find(_T("BO_DONINFO_TBL")) > 0)
			{
				tmpquery =  strTempQuery + strOption + _T(" GROUP BY P.SUBJECT_CODE ");	
			}
			else
			{
				tmpquery =  strTempQuery + strOption + _T(" GROUP BY SUBSTR(B.CLASS_NO, 1, 1) ");
			}
			
			tmpDM.RestructDataManager(tmpquery);
			CopyTargetDM(&tmpDM, desc, m_pStatCondition->GetAcqType());
		}
		*/
		
		INT nRunCount = m_pStatCondition->GetVerticalDescCount() +1;		
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRunCount);
		INT nCurPos = m_pDataProcessingDlg->GetProgressCtrl()->GetPos();

		CString strTempQuery;
		CString strTempCodes;

		INT nStartIndex = 0;

		for (i=0; i<nRunCount; i++)
		{
			if (!m_bRunning)
				return 0;

			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nCurPos);

			if (nRunCount-1 > i)
			{
				CString strCode, strDesc;
				m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), _T("코드"), strCode, nStartIndex);
				nStartIndex = m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), _T("설명"), strDesc, nStartIndex);
				nStartIndex++;

				strCode.TrimLeft(); strCode.TrimRight();
				strDesc.TrimLeft(); strDesc.TrimRight();

				if(strCode.IsEmpty()) 
				{
					strOption.Format(_T(" AND (%s = '%s' OR %s IS NULL)"), m_pStatCondition->GetVerticalField(), strCode, m_pStatCondition->GetVerticalField());
				}
				else
				{
					strOption.Format(_T(" AND %s = '%s'"), m_pStatCondition->GetVerticalField(), strCode);

					if (strTempCodes.IsEmpty())
						strTempCodes.Format(_T("'%s'"), strCode);
					else
					{
						CString strTempBuf = strTempCodes;
						strTempCodes.Format(_T("%s,'%s'"), strTempBuf, strCode);
					}
				}

				strTempQuery = m_pStatCondition->GetWhereStmt();

				if (strTempQuery.Find(_T("BO_PURCHASEINFO_TBL")) > 0 || strTempQuery.Find(_T("BO_DONINFO_TBL")) > 0)
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY P.SUBJECT_CODE ");	
				}
				else
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY SUBSTR(B.CLASS_NO, 1, 1) ");
				}

				tmpDM.RestructDataManager(tmpquery);

				if(strCode.IsEmpty())
				{
					CopyTargetDM(&tmpDM, strDesc, m_pStatCondition->GetAcqType());
				}
				else
				{
					CString strDisp;
					strDisp.Format(_T("%s(%s)"), strDesc, strCode);

					CopyTargetDM(&tmpDM, strDisp, m_pStatCondition->GetAcqType());
				}
			}
			else // 기타 추가
			{
				strOption.Format(_T(" AND %s NOT IN (%s) AND %s IS NOT NULL"), m_pStatCondition->GetVerticalField(), strTempCodes, m_pStatCondition->GetVerticalField());

				strTempQuery = m_pStatCondition->GetWhereStmt();

				if (strTempQuery.Find(_T("BO_PURCHASEINFO_TBL")) > 0 || strTempQuery.Find(_T("BO_DONINFO_TBL")) > 0)
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY P.SUBJECT_CODE ");	
				}
				else
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY SUBSTR(B.CLASS_NO, 1, 1) ");
				}

				tmpDM.RestructDataManager(tmpquery);

				CopyTargetDM(&tmpDM, _T("기타"), m_pStatCondition->GetAcqType());
			}
		}
		// KOL.RED.2018.210 ---------------------------------------------------------------------------
	}
	
	return 0;
}

INT CBasicStatistics::InsertSumRowToTargetDM(INT startRow)
{
	if( m_bRunning == FALSE )	return 0;	
	if( startRow < 0 )			return 0;	
	if( m_pDM->GetRowCount() == startRow )	return 0;
	
	//================================================================
	// 변수정의
	//================================================================
	INT nCurRow, nBookCnt, nSpeciesCnt, i, j;
	CString strSpeciesCnt, strBookCnt, tmpstr, tmpstr1, strTmpData;	
	
	m_pDM->InsertRow(-1);
	nBookCnt = nSpeciesCnt = 0;
	nCurRow = m_pDM->GetRowCount() - 1;		
	for( i=0 ; i<12 ; i++ )
	{
		if( m_bRunning == FALSE )	return 0;
		nSpeciesCnt = nBookCnt = 0;
		if( i == 0 )	tmpstr = _T("적용안함");
		else if(i == 11) tmpstr = _T("계");
		else tmpstr.Format(_T("%d"), i-1);

		for( j=startRow ; j<nCurRow ; j++ )
		{
			strTmpData = m_pDM->GetCellData(tmpstr, j);
			CStringArray tmpArray;
			m_pGroupCollection->ParsingStringApplyDuplicate(_T("/"), strTmpData, &tmpArray);
			if(tmpArray.GetSize() > 0) nSpeciesCnt += _ttoi(tmpArray.GetAt(0));
			if(tmpArray.GetSize() > 1) nBookCnt += _ttoi(tmpArray.GetAt(1));
		}
		tmpstr1.Format(_T("%d/%d"), nSpeciesCnt, nBookCnt);
		m_pDM->SetCellData(tmpstr, tmpstr1, nCurRow);
	}
	m_pDM->SetCellData(_T("자료구분설명"), _T("계"), nCurRow);
	return nCurRow+1;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 마지막 라인에 현재까지 구한 dm의 가격합계의 합을 구한다.
//
// [ 미니스펙 ]
// 1. 멤버DM에 라인 하나를 추가한다. (총계를 나타내어질 라인)
// 2. 국방대 플래그에 따라 칼럼 갯수가 변경된다.
// 3. 마지막 라인에 총계정보를 구축한다. (칼럼->로우)
//		- 11/22 : 계
//		- 12/23 : 가격합계
//
//////////////////////////////////////////////////////////////////////
INT CBasicStatistics::InsertPriceSumRowToTargetDM(INT startRow)
{
	//================================================================
	// 변수정의
	//================================================================
	INT rowCnt, nColCnt, nCurRow, i, j;
	DOUBLE fPrice = 0.0;
	CString strPrice, strAlias, strPriceAlias;


	if( m_bRunning == FALSE )	return 0;
	if( startRow < 0 )			return 0;
	
	rowCnt = m_pDM->GetRowCount();
	if( rowCnt == startRow )	return 0;
	

	//================================================================
	// 1. 멤버DM에 라인 하나를 추가한다. (총계를 나타내어질 라인)
	//================================================================
	m_pDM->InsertRow(-1);
	nCurRow = m_pDM->GetRowCount() - 1;
	m_pDM->SetCellData(_T("자료구분설명"), _T("가격합계"), nCurRow);
	

	//================================================================
	// 2. 국방대 플래그에 따라 칼럼 갯수가 변경된다.
	//================================================================
	if( _T("Y") == m_strIs90 )	nColCnt = 24;
	else						nColCnt = 13;
	

	//================================================================
	// 3. 마지막 라인에 총계정보를 구축한다. (칼럼->로우)
	//		- 11/22 : 계
	//		- 12/23 : 가격합계
	//================================================================
	for( i=0 ; i<nColCnt ; i++ )
	{
		fPrice = 0.0;
		if( m_bRunning == FALSE )	return 0;
		
		switch( i )
		{
		case 0 :	strAlias = _T("적용안함");
			break;
		case 11:	
			{
				if( _T("Y") == m_strIs90 )	strAlias.Format(_T("%d"), i-1);
				else						
				{
					m_pDM->SetCellData(_T("계"), _T("0"), nCurRow);
					continue;
				}
			}
			break;
		case 12:
			{	
				if( _T("Y") == m_strIs90 )	strAlias.Format(_T("%d"), i-1);
				else						strAlias = _T("가격합계");//tmpstr = _T("계");
			}
			break;
		case 22:	
			{
				m_pDM->SetCellData(_T("계"), _T("0"), nCurRow);
				continue;
			}
			break;
		case 23:	strAlias = _T("가격합계");//tmpstr = _T("계");			
			break;
		default:	strAlias.Format(_T("%d"), i-1);
			break;
		}

		for( j=startRow ; j<nCurRow ; j++ )
		{
			if( strAlias == _T("가격합계") )	strPriceAlias = strAlias;
			else								strPriceAlias = strAlias + _T("_가격");

			strPrice = m_pDM->GetCellData(strPriceAlias, j);
			strPrice.Replace(_T(","), _T(""));
			//fPrice += (DOUBLE)atof(strPrice.GetBuffer(0));
			TCHAR* pEndPtr = NULL;
			fPrice += _tcstod(strPrice.GetBuffer(0), &pEndPtr);
		}

		strPrice.Format(_T("%.0f"), fPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strPrice, strPrice);
		m_pDM->SetCellData(strAlias, strPrice, nCurRow);
	}
	
	
	return nCurRow+1;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 임시로 구해온 DM을 멤버DM에 통계자료 형식에 맞게 채워준다.
//
// [ 미니스펙 ]
// 1. 멤버DM에 새 라인을 추가한다.
// 2. 인자로 온 DM에 값이 없다면 결과가 없으므로 모두 0으로 채워준다.
// 3. 멤버DM에 인자로온 DM의 내용을 추가한다.
// 4. 총정보를 DM에 설정한다.
//
//////////////////////////////////////////////////////////////////////
INT CBasicStatistics::CopyTargetDM(CESL_DataMgr *pDM, CString desc, ACQ_TYPE nAcqType)
{
	//================================================================
	// 변수정의
	//================================================================
	INT cnt, nCurRow, nBookCnt, nSpeciesCnt, i ;
	DOUBLE fPrice = 0.0;
	CString strBookCnt, strSpeciesCnt, strTmpData, tmpstr, strPrice;

	//================================================================	
	// 1. 멤버DM에 새 라인을 추가한다.
	//================================================================		
	m_pDM->InsertRow(-1);
	nCurRow = m_pDM->GetRowCount() - 1;	

	//================================================================
	// 2. 인자로 온 DM에 값이 없다면 결과가 없으므로 모두 0으로 채워준다.
	//================================================================	
	nBookCnt = nSpeciesCnt = 0;
	cnt = pDM->GetRowCount();	
	if( cnt == 0 )
	{
		m_pDM->SetCellData(_T("자료구분설명"),	desc,	nCurRow);
		return 0;
	}

	//================================================================	
	// 3. 멤버DM에 인자로온 DM의 내용을 추가한다.
	//================================================================	
	for( i=0 ; i<cnt ; i++ )
	{
		tmpstr			= pDM->GetCellData(i, 0);	// p.Subject_code
		strSpeciesCnt	= pDM->GetCellData(i, 1);	// COUNT(DISTINCT(I.REC_KEY))
		strBookCnt		= pDM->GetCellData(i, 2);	// COUNT(DISTINCT(B.REC_KEY))
		strPrice		= pDM->GetCellData(i, 3);	// SUM(B.PRICE) or SUM(GetPriceWon(B.ACQ_KEY, B.PRICE, 'Y'))
		
		// _T("종/권")에 형태로 값을 만듬
		strTmpData.Format(_T("%s/%s"), strSpeciesCnt, strBookCnt); 


		// 해당 코드값을 찾아서 종/권값 및 가격값을 설정한다.
		tmpstr.TrimLeft();		tmpstr.TrimRight();
		if( tmpstr.IsEmpty() == TRUE )	tmpstr = _T("적용안함");		
		m_pDM->SetCellData(tmpstr, strTmpData, nCurRow);
		m_pDM->SetCellData(tmpstr+_T("_가격"),	strPrice,	nCurRow);
		
		tmpstr.Format(_T("%d"), nAcqType);
		m_pDM->SetCellData(_T("수입구분"), tmpstr, nCurRow);
		m_pDM->SetCellData(_T("자료구분설명"), desc, nCurRow);
		
		strPrice.Replace(_T(","), _T(""));

		// 총 정보를 구한다.
		nBookCnt += _ttoi(strBookCnt.GetBuffer(0));
		nSpeciesCnt += _ttoi(strSpeciesCnt.GetBuffer(0));
		//fPrice += (DOUBLE)atof(strPrice.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		fPrice += _tcstod(strPrice.GetBuffer(0), &pEndPtr);
	}
	
	//================================================================		
	// 4. 총정보를 DM에 설정한다.
	//================================================================		
	tmpstr.Format(_T("%d/%d"), nSpeciesCnt, nBookCnt);
	strPrice.Format(_T("%.0f"), fPrice);

		m_pDM->SetCellData(_T("계"), tmpstr, nCurRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strPrice, strPrice);
		m_pDM->SetCellData(_T("가격합계"), strPrice, nCurRow);

	return 0;
}

CString CBasicStatistics::CheckValidateGroupNo(CString strGroupInfo, CString strAcqYear, CString strWorkCode)
{
	INT nInitCnt = -1;

	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	m_pGroupCollection->ParsingString(_T(","), strGroupInfo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	nInitCnt = cnt;

	INT ids = -1;

	CString strMsg;
	for(INT i=0 ; i<cnt && cnt>0; i++)
	{
		ids = m_pGroupCollection->CheckValidate(arrDivNo.GetAt(i), strAcqYear, strWorkCode);
		if(ids < 0)
		{
			if(m_pGroupCollection->GetAcqType() == PURCHASE)
				strMsg.Format(_T("입력하신 차수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else if(m_pGroupCollection->GetAcqType() == DONATE)
				strMsg.Format(_T("입력하신 접수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			ESLAfxMessageBox(strMsg);
			arrDivNo.RemoveAt(i);
			i--;
			cnt--;
		}
	}
	if(nInitCnt != cnt)
	{
		CString strTmpData;		strTmpData.Empty();
		for(i=0 ; i<cnt ; i++)
		{
			if(strTmpData.IsEmpty()) strTmpData = arrDivNo.GetAt(i);
			else strTmpData += _T(", ") + arrDivNo.GetAt(i);
		}
		return strTmpData;
	}

	return strGroupInfo;
}
