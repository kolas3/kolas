// MakeSimplePrintDM.cpp: implementation of the CMakeSimplePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSimplePrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSimplePrintDM::CMakeSimplePrintDM(CESL_Mgr * pParentMgr) : CMakePrintDM(pParentMgr)
{
	m_pPrintObjDM = NULL;
	m_pPrintSourceDM = NULL;
	m_pPrintBookDM = NULL;
}

CMakeSimplePrintDM::~CMakeSimplePrintDM()
{
	if(m_pPrintObjDM)
	{
		delete m_pPrintObjDM;
		m_pPrintObjDM = NULL;
	}
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 프린트타입에 따라서 멤버변수를 설정한다.
//
// [ 미니스펙 ]
// 1. DM이 설정되지 않았다면 리턴한다.
// 2. 프린트타입이 [비치희망현황리스트]가 아니면 
//	  m_pPrintObjDM을 m_pPrintBookDM의 파일을 읽어와 초기화한다.
// 3. 프린트타입에 따라서 멤버변수를 설정한다.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::InitMakePrintDM()
{
	//================================================================
	// 1. DM이 설정되지 않았다면 리턴한다.
	//================================================================
	if(m_pPrintSourceDM == NULL || m_pPrintBookDM == NULL)		return -1;

	
	//================================================================
	// 2. 프린트타입이 [비치희망현황리스트]가 아니면 
	//	  m_pPrintObjDM을 m_pPrintBookDM의 파일을 읽어와 초기화한다.
	//================================================================
	if(m_nPrintType != BO_ACQ_REQUEST_DATA)
	{
		m_pPrintObjDM = new CESL_DataMgr;
		m_pPrintObjDM->SetCONNECTION_INFO(m_pPrintSourceDM->CONNECTION_INFO);
		m_pPrintObjDM->InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
		m_pPrintObjDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	}


	//================================================================
	// 3. 프린트타입에 따라서 멤버변수를 설정한다.
	//================================================================
	switch ( m_nPrintType )
	{
	// 기초자료목록
	case BO_ACQ_BASIC_DATA:
	{
		m_IgnoreAlias.Add(_T("BB_입력일"));
		m_IgnoreAlias.Add(_T("UDF_그룹명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 반입자료목록
	case BO_ACQ_IMPORT_DATA:
	{
		m_IgnoreAlias.Add(_T("BB_입력일"));
		m_IgnoreAlias.Add(_T("UDF_그룹명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 기증접수자료목록
	case BO_ACQ_DONATE_DATA :
	{
		m_IgnoreAlias.Add(_T("UDF_출력접수번호정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_기증자명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
		m_strTotalDiscPriceAlias = _T("UDF_변환가격");
	}
		break;

	// 기증미등록자료목록
	case BO_ACQ_NONDONATE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력접수번호정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_기증자명"));
		m_IgnoreAlias.Add(_T("BP_기증부당사유"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 기증등록인계대상자료목록
	case BO_ACQ_DONATE_REGOBJ_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력접수번호정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_기증자명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 수서통계출력물 리스트(기증)
	case BO_DONATE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력접수번호정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_기증자명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("BP_기증자명");
	}
		break;
	
	// 구입대상자료목록
	case BO_ACQ_PURCHASE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환종가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 수서통계출력물리스트(구입)
	case BO_PURCHASE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환종가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 재활용자료목록
	case BO_ACQ_RECYCLE_DATA:
	{
		m_IgnoreAlias.Add(_T("IBS_삭제일"));
		m_IgnoreAlias.Add(_T("UDF_그룹명"));
		m_IgnoreAlias.Add(_T("BP_수입구분"));
		m_IgnoreAlias.Add(_T("CF_구입신청자"));
		m_IgnoreAlias.Add(_T("CF_구입신청일"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 검수대상자료목록
	case BO_ACQ_CHECKINING_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_발주일"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환종가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
		m_strTotalSpeciesCntAlias = _T("IBS_발행년");
	}
		break;

	// 구입신청자료도착목록
	case BO_ACQ_CHECKIN_REQUEST_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_발주일"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환종가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
	}
		break;

	// 검수완료자료 목록
	case BO_ACQ_CHECKIN_COMPLETE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_IgnoreAlias.Add(_T("BP_검수일"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환종가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
	}
		break;
	
	// 구입대상자료목록_신청부서포함
	case BO_ACQ_PURCHASE_DATA_5163:		// 2005-01-29 5163용
	{
		m_IgnoreAlias.Add(_T("UDF_출력차수정보"));
		m_IgnoreAlias.Add(_T("UDF_주제구분설명"));
		m_IgnoreAlias.Add(_T("UDF_입력구분설명"));
		m_strTotalInfoTitle = _T("UDF_일련번호");
		m_strTotalBookCntAlias = _T("UDF_책수");
		m_strTotalPriceAlias = _T("UDF_변환종가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인종가격");
		m_strTotalSpeciesCntAlias = _T("UDF_할인단가");
	}
		break;
			
	default :	return 0;	// m_nPrintType < 0
		break;
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM을 구축한다.
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::MakePrintDM()
{
	INT ids;

	//================================================================
	// 1. 출력용 DM을 구성하기 위한 요소들을 체크한다.
	//================================================================
	ids = CheckValid();
	if(ids < 0) return ids;

	if(	m_nPrintType == BO_ACQ_BASIC_DATA || 
		m_nPrintType == BO_ACQ_IMPORT_DATA || 
		m_nPrintType == BO_ACQ_DONATE_DATA || 
		m_nPrintType == BO_ACQ_NONDONATE_DATA ||
		m_nPrintType == BO_ACQ_DONATE_REGOBJ_DATA || 
		m_nPrintType == BO_ACQ_PURCHASE_DATA ||
		m_nPrintType == BO_ACQ_RECYCLE_DATA || 
		m_nPrintType == BO_ACQ_CHECKINING_DATA	||
		m_nPrintType == BO_PURCHASE_DATA || 
		m_nPrintType == BO_DONATE_DATA ||  
		m_nPrintType == DONATOR_LABEL ||  
		m_nPrintType == BO_ACQ_PURCHASE_DATA_5163 )   // 2005-01-29 5163용 추가.
	{
		ids = MakeSpeciesVolumePrintDM(m_pPrintBookDM);
	}
	else if( m_nPrintType == BO_ACQ_REQUEST_DATA )
		ids = MakeRequestPrintDM();

	return ids;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 일반리스트 출력용 DM을 구축한다.
// - m_pPrintDM - 출력용DM
// - m_pPrintSourceDM - 원본DM
//
// [ 미니스펙 ]
// 1. 프로그래스 컨트롤 창을 보여준다.	
// 2. 권DM을 생성한다.
// 3. 권 DM을 구분 알리아스로 정렬한다.	
// 4. 원본 출력DM에서 총계정보를 추출한다.	
// 5. 출력용 DM를 Clear시킨다.
// 6. 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
// 7. 정렬여부가 Y라면 출력용 DM을 정렬한다.
// 8. 총계정보를 넣는다.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::MakeSpeciesVolumePrintDM(CESL_DataMgr *pDM_BOOK)
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids, nRowCnt, nSerialNo, i, nCondColumns = 0;
	INT nCond[2];
	DOUBLE dTotalOrderPrice = 0.0;
	CString strKey, strTmpData;	
	CString strAlias[2];
	


	//================================================================	
	// 1. 프로그래스 컨트롤 창을 보여준다.	
	//================================================================
	ShowDataProcessingDlg();
	
	// 원본DM을 정렬하지 않고 출력용 DM을 정렬하여 원본DM을 유지시킨다.
	// 2005.10.25 강봉수 변경
	/*
	//================================================================
	// 2. 정렬여부가 Y라면 출력용 원본 DM을 정렬한다.
	//================================================================
	if( m_bDataSortYN == TRUE ) 
		ids = PrintDataSortByUserSelect(m_pPrintSourceDM);	// 정렬
		//ids = PrintDataSortByUserSelect(m_pPrintDM);
	else 
		ids = PrintDataSortByBasicInfo(m_pPrintSourceDM);	// 정렬하지않음 ( 그냥리턴 )
		//ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO)
			return 0;
	}
	*/

	//================================================================
	// 2. 권DM을 생성한다. - m_pPrintObjDM
	//================================================================
	ids = MakeVolumeDM(m_pPrintObjDM, pDM_BOOK, m_strDivAlias);		
	if(ids < 0) return ids;

	//================================================================
	// 3. 권 DM을 구분 알리아스로 정렬한다.	
	//================================================================	
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	

	//================================================================
	// 4. 원본 출력DM에서 총계정보를 추출한다.	
	//================================================================
	ids = m_cTotalInfo.SetTotalInfo(m_pArrDivValeu, m_strDivAlias, m_pPrintSourceDM);
	if( ids < 0 )	return ids;


	//================================================================
	// 5. 출력용 DM를 Clear시킨다.
	//================================================================
	m_pPrintDM->FreeData();

	//================================================================
	// 6. 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
	//================================================================
	nRowCnt = m_pPrintSourceDM->GetRowCount();
	if( m_pDataProcessingDlg != NULL )
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	//================================================================
	// 7. 정렬여부가 Y라면 출력용 DM을 정렬한다.
	//================================================================
	// 기본화면에서 사용되는 원본을 바로 정렬하면 안되므로 더미를 하나 만들어서 정렬한다.
	CESL_DataMgr pDummyDM;

	// Dummy DM을 Source DM과 동일한 구조로 만든다.	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = m_pPrintSourceDM->m_nCols;	
	if(nCols < 0) nCols = m_pPrintSourceDM->GetRefColumnCount();
	ids = pDummyDM.InitDataMgrRef(nCols);
	for (col = 0; col < nCols; col++) {
		if (m_pPrintSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;		
		ids = pDummyDM.SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);		
	}
	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		// 출력용 DM을 생성
		DWORD* pRow = (DWORD*)pDummyDM.InsertRow(-2);
		if( pRow == NULL )		continue;

		ids = CopyDM(m_pPrintSourceDM, i, &pDummyDM, pRow);
		if( ids < 0 )	return ids;		
	}	

	pDummyDM.RefreshPositionInfo();

	if( m_bDataSortYN == TRUE ) 	ids = PrintDataSortByUserSelect(&pDummyDM);
	else							ids = PrintDataSortByBasicInfo(&pDummyDM);
	if( ids < 0 )
	{
		if( m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO )
		{
			return -1;
		}
	}	
	
	nSerialNo = 0;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		if( m_pDataProcessingDlg != NULL )	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = pDummyDM.GetCellData(m_strDivAlias, i);
		if( CheckPrintData(strKey) == FALSE )		continue;
		
		// 출력용 DM을 생성
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if( pRow == NULL )		continue;

		// 종DM을 출력용 DM으로 복사
		ids = CopyDM(&pDummyDM, i, m_pPrintDM, pRow);
		if( ids < 0 )	return ids;		
		
		// 일련번호 설정
		strTmpData.Format(_T("%d"), ++nSerialNo);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_일련번호")), strTmpData);
		
		// ISBN 설정
		strTmpData = m_pPrintSourceDM->GetCellData(_T("IBS_ISBN"), i);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("BB_낱권ISBN")), strTmpData);
		// 2006.03.16 KBS - 부서명 설정 (국방대 추가)
		if( pDummyDM.GetCellData(_T("BP_신청구분"), i) == _T("CLA") )
		{
			strTmpData.Empty();
			strTmpData = pDummyDM.GetCellData(_T("BP_부서코드"), i);
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("직원부서"), strTmpData, strTmpData);
			m_pPrintDM->SetCellData(_T("UDF_부서명"), strTmpData, i);
		}
		else	m_pPrintDM->SetCellData(_T("UDF_부서명"), _T(""), i);

		// 권DM을 출력용 DM으로 복사( strKey = 수서정보KEY )
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, strKey, SPECIES_PRINT);
		if( ids < 0 )	return ids;
		else if( ids == 1000 )	m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_단가")), _T(""));
		//strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_변환할인종가격")) );
		strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_변환가격")) );
		strTmpData.Replace(_T(","), _T(""));
		//dTotalOrderPrice += (DOUBLE)atof(strTmpData);
		TCHAR* pEndPtr = NULL;
		dTotalOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	//m_cTotalInfo.m_dTotalPrice = dTotalOrderPrice;
	ids = m_pPrintDM->RefreshPositionInfo();
	if( ids < 0 )	return -1;

	/*
	//================================================================
	// 7. 정렬여부가 Y라면 출력용 DM을 정렬한다.
	//================================================================
	if( m_bDataSortYN == TRUE ) 	ids = PrintDataSortByUserSelect(m_pPrintDM);
	else							ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if( ids < 0 )
	{
		if( m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO )
		{
			return -1;
		}
	}	
	*/
	
	//================================================================
	// 8. 총계정보를 넣는다.
	//================================================================
	m_pPrintDM->InsertRow(-1);	
	SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("총계"), dTotalOrderPrice);
	// 2004.12.03 환산총액을 넣어야쥐
	// 2005-01-27 환산총액 주석처리
	//m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	//SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("환산총액"));

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 출력용 DM을 구성하기 위해 필요한 요소를 체크한다.
//
// [ 미니스펙 ]
// 1. 프린트타입이 [비치희망현황리스트]를 제외한 모든 프린트타입들이
//	  출력DM이 없으면 리턴한다.
// 2. 원본출력DM과 원본DM이 없다면 리턴한다.
// 3. 구분알리아스가 없다면 리턴한다.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::CheckValid()
{
	//================================================================
	// 1. 프린트타입이 [비치희망현황리스트]를 제외한 모든 프린트타입들이
	//	  출력DM이 없으면 리턴한다.
	//================================================================
	if( m_nPrintType != BO_ACQ_REQUEST_DATA )
		if( m_pPrintDM == NULL || m_pPrintObjDM == NULL ) return -1;


	//================================================================
	// 2. 원본출력DM과 원본DM이 없다면 리턴한다.
	//================================================================
	if( m_pPrintSourceDM == NULL || m_pPrintBookDM == NULL ) return -1;	
	

	//================================================================
	// 3. 구분알리아스가 없다면 리턴한다.
	//================================================================
	if(m_strDivAlias.IsEmpty()) return -1;
	
	return 0;
}

INT CMakeSimplePrintDM::MakeRequestDataPrintDM()
{
	//프로그래스 컨트롤을 보여준다.
	ShowDataProcessingDlg();
	
	// 책DM을 순환하면서 BP_입력구분값이 '2' 정보 이외의 정보는 제거한다.
	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(m_pPrintBookDM->CONNECTION_INFO);
	DM_BOOK.InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids = MakeRequestData(m_pPrintBookDM, &DM_BOOK);
	if(ids < 0) return -1;
	if(DM_BOOK.GetRowCount() == 0) return 1000;

	// 종DM을 생성한다.
	ids = MakeSpeciesDM(&DM_SPECIES, &DM_BOOK, FALSE);
	if(ids < 0) return ids;
	ids = SetUDFDMFieldData(&DM_SPECIES, TRUE);
	if(ids < 0) return ids;

	//정렬
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(&DM_SPECIES);
	else ids = PrintDataSortByBasicInfo(&DM_SPECIES);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO)
			return 0;
	}

	//비치희망신청자 정보를 선정해야한다.
	ids = SetRequestInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	//일련번호 설정
	ids = SetDataSerialNo(&DM_SPECIES);
	if(ids < 0) return ids;

	//권DM을 생성한다.
	ids = MakeVolumeDM(m_pPrintObjDM, &DM_BOOK, m_strDivAlias);
	if(ids < 0) return ids;
	
	// 권 DM을 구분 알리아스로 정렬한다.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	//원본 출력DM에서 총계정보를 추출한다.
	ids = m_cTotalInfo.SetTotalInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	
	// 출력DM를 Clear시킨다.
	m_pPrintDM->FreeData();
	
	// 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
	INT nRowCnt = DM_SPECIES.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = DM_SPECIES.GetCellData(m_strDivAlias, i);
		
		DWORD * pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if(pRow == NULL) return -1;
		ids = CopyDM(&DM_SPECIES, i, m_pPrintDM, pRow);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
		else if(ids == 1000) m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_단가")), _T(""));
	}
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return ids;
	
	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// 총계정보를 넣아야쥐
		m_pPrintDM->InsertRow(-1);
		SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("총계"));
	}

	return 0;
}

INT CMakeSimplePrintDM::MakeRequestData(CESL_DataMgr *pSourceBookDM, CESL_DataMgr *pTargetBookDM)
{
	if(pSourceBookDM == NULL || pTargetBookDM == NULL) return -1;
	INT nRowCnt = pSourceBookDM->GetRowCount();
	CString strTmpData;
	INT ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pSourceBookDM->GetCellData(_T("BP_입력구분"), i);
		if(strTmpData == _T("2") && CheckPrintData(pSourceBookDM->GetCellData(m_strDivAlias, i)))
		{
			ids = CopyDM(pSourceBookDM, i, pTargetBookDM, -1);
			if(ids < 0) return -1;
		}
	}
	return 0;
}

INT CMakeSimplePrintDM::SetRequestInfo(CESL_DataMgr *pSourceDM)
{
	if(pSourceDM == NULL) return -1;
	if(pSourceDM->GetRowCount() == 0) return 0;

	CString strAcqKey;	strAcqKey.Empty();
	CString strQuery;
	INT nRowCnt = pSourceDM->GetRowCount();
	INT nTmpData, ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		nTmpData = (i+1)%600;
		if(nTmpData != 0)
		{
			if(strAcqKey.IsEmpty()) strAcqKey = pSourceDM->GetCellData(_T("BP_수서정보KEY"), i);
			else strAcqKey += _T(", ") + pSourceDM->GetCellData(_T("BP_수서정보KEY"), i);
		}
		else 
		{
			strQuery.Format(
				_T("SELECT ACQ_KEY, APPLICANT_NAME, APPLICANT_DATE ")
				_T("FROM BO_FURNISH_TBL ")
				_T("WHERE ACQ_KEY IN (%s)"), strAcqKey);
			ids = SetRequestInfo(pSourceDM, strQuery);
			if(ids < 0) return -1;
			strAcqKey.Empty();
		}
	}

	strQuery.Format(
		_T("SELECT ACQ_KEY, APPLICANT_NAME, APPLICANT_DATE ")
		_T("FROM BO_FURNISH_TBL ")
		_T("WHERE ACQ_KEY IN (%s)"), strAcqKey);
	ids = SetRequestInfo(pSourceDM, strQuery);
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakeSimplePrintDM::SetRequestInfo(CESL_DataMgr * pSourceDM, CString strQuery)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pSourceDM->CONNECTION_INFO);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	INT nSourceCnt = pSourceDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	CString strAcqKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strAcqKey = tmpDM.GetCellData(i, 0);
		for(INT j=0 ; j<nSourceCnt ; j++)
		{
			if(pSourceDM->GetCellData(_T("BP_수서정보KEY"), j) == strAcqKey) break;
		}
		if(nSourceCnt == j) continue;
		pSourceDM->SetCellData(_T("UDF_신청자"), tmpDM.GetCellData(i, 1), j);
		pSourceDM->SetCellData(_T("UDF_신청일"), tmpDM.GetCellData(i, 2), j);
	}
	return 0;
}

INT CMakeSimplePrintDM::MakeCheckInCompletePrintDM()
{
	DOUBLE dTotalOrderPrice = 0.0;
	CString strTmpData;
	//프로그래스 컨트롤을 보여준다.
	ShowDataProcessingDlg();
	
	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	
	// 종DM을 생성한다.
	INT ids = MakeSpeciesDM(&DM_SPECIES, m_pPrintBookDM, FALSE);
	if(ids < 0) return ids;
	ids = SetUDFDMFieldData(&DM_SPECIES, TRUE);
	if(ids < 0) return ids;	

	//권DM을 생성한다.
	ids = MakeVolumeDM(m_pPrintObjDM, m_pPrintBookDM, m_strDivAlias);
	if(ids < 0) return ids;
	
	// 권 DM을 구분 알리아스로 정렬한다.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	SetTotalInfoAlias(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias);
	//원본 출력DM에서 총계정보를 추출한다.
	ids = m_cTotalInfo.SetTotalInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	
	// 출력DM를 Clear시킨다.
	m_pPrintDM->FreeData();
	
	// 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
	INT nRowCnt = DM_SPECIES.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = DM_SPECIES.GetCellData(m_strDivAlias, i);
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		ids = CopyDM(&DM_SPECIES, i, m_pPrintDM, pRow);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
		else if(ids == 1000) m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_단가")), _T(""));
		strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_변환할인종가격")) );
		strTmpData.Replace(_T(","), _T(""));
		//dTotalOrderPrice += (DOUBLE)atof(strTmpData);
		TCHAR* pEndPtr = NULL;
		dTotalOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;

	//정렬
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO)
			return 0;
	}

	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// 총계정보를 넣아야쥐
		m_pPrintDM->InsertRow(-1);
		SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("총계"), dTotalOrderPrice);
	}

	return 0;
}

INT CMakeSimplePrintDM::PrintDataSortByBasicInfo(CESL_DataMgr *pDM)
{
	return 0;
}

INT CMakeSimplePrintDM::PrintDataSortByUserSelect(CESL_DataMgr *pDM)
{
	INT ids;
	if( m_pSortItem )
	{
		ids =  m_pSortItem->DataSort(pDM);
		if(ids < 0) return ids;
	}
	return 0;
}

INT CMakeSimplePrintDM::MakeRequestPrintDM()
{
	//프로그래스 컨트롤을 보여준다.
	ShowDataProcessingDlg();
	
	INT ids;
	//정렬
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintBookDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintBookDM);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("출력자료 정렬작업을 수행할 수 없습니다. 계속 진행 하시겠습니까?"), MB_YESNO) == IDNO)
			return 0;
	}
	
	// 출력DM를 Clear시킨다.
	m_pPrintSourceDM->FreeData();

	// 2005-03-11
	//if(m_pArrDivValeu && m_pArrDivValeu->GetCount() > 0)
	if(m_pArrDivValeu)
	{
		
		// 원본 종 출력DM을 하나씩 확인하여 해당하는 권 DM을 구축한다.
		INT nRowCnt = m_pPrintBookDM->GetRowCount();
		if(m_pDataProcessingDlg)
		{
			m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
			m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
		}
		INT nSerialNo = 0;
		CString strKey, strTmpData;
		for(INT i=0 ; i<nRowCnt ; i++)
		{
			if(m_pDataProcessingDlg)
				m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
			
			strKey = m_pPrintBookDM->GetCellData(m_strDivAlias, i);
			// 2005-03-11
			if(m_pArrDivValeu->GetCount() > 0)
			{
				if(!CheckPrintData(strKey)) continue;
			}
			
			DWORD * pRow = (DWORD*)m_pPrintSourceDM->InsertRow(-2);
			if(pRow == NULL) continue;
			ids = CopyDM(m_pPrintBookDM, i, m_pPrintSourceDM, pRow);
			if(ids < 0) return ids;
			strTmpData.Format(_T("%d"), ++nSerialNo);
			// 2005-03-11
			// m_pPrintSourceDM->SetCellData(pRow, m_pPrintSourceDM->FindColumn(_T("UDF_일련번호")), strTmpData);
			m_pPrintSourceDM->SetCellData(nSerialNo-1, m_pPrintSourceDM->FindColumn(_T("UDF_일련번호")), strTmpData);
		}
		ids = m_pPrintSourceDM->RefreshPositionInfo();
		if(ids < 0) return -1;
	}

	// 총계정보를 넣아야쥐
	return 0;
}
