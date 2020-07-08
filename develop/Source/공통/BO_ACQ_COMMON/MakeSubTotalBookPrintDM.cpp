// MakeSubTotalBookPrintDM.cpp: implementation of the CMakeSubTotalBookPrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSubTotalBookPrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSubTotalBookPrintDM::CMakeSubTotalBookPrintDM(CESL_Mgr * pParentMgr) : CMakeSubTotalPrintDM(pParentMgr)
{
	m_pPrintBookDM = NULL;
	m_pGroupCollection = NULL;
	m_bDeletPrintBookDM = FALSE;
}

CMakeSubTotalBookPrintDM::~CMakeSubTotalBookPrintDM()
{
	
}

BOOL CMakeSubTotalBookPrintDM::CheckTotalInfoAlias()
{
	if(m_strTotalInfoTitle.IsEmpty()		||
		m_strTotalBookCntAlias.IsEmpty()	||
		m_strTotalPriceAlias.IsEmpty()		||
		m_strTotalDiscPriceAlias.IsEmpty())
		return FALSE;
	return TRUE;
}

INT CMakeSubTotalBookPrintDM::InitMakePrintDM()
{
	if(m_pPrintDM == NULL || m_pPrintBookDM == NULL) return -1;
	if(m_nPrintType < 0) return -1;
	
	if(m_nPrintType == BO_ACQ_PURCHASE_DATA_REGCODE)
	{
		m_strTotalInfoTitle = _T("UDF_차수정보");
		m_strTotalBookCntAlias = _T("IBS_발행년");
		m_strTotalPriceAlias = _T("UDF_변환가격");
		m_strTotalDiscPriceAlias = _T("UDF_변환할인가격");
		return 0;
	}
	return -1;
}

INT CMakeSubTotalBookPrintDM::MakePrintDM()
{
	INT ids, nCurIdx;
	if(m_pGroupCollection == NULL || m_pPrintDM == NULL || m_pPrintBookDM == NULL)
		return -1;

	if(!CheckTotalInfoAlias())
	{
		if(IDNO == m_pParentMgr->ESLAfxMessageBox(_T("경고:합계정보를 나타낼 알리아스를 정의하지 않았습니다.\n출력작업을 계속 진행 하시겠습니까?"), MB_YESNO))
			return 1000;
	}

	//프로그래스 컨트롤을 보여준다.
	ShowDataProcessingDlg();

	// 종자료정렬
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(!m_strSortCondition.IsEmpty())
	{
		strAlias[nCondColumns] = m_strSortCondition;		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	}
	strAlias[nCondColumns] = _T("BP_일련번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	m_pPrintBookDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	
	
	// 원본 책DM정보를 복사한다
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, m_pPrintDM->GetRowCount());
	}
	
	m_pPrintDM->FreeData();
	for(INT i=0 ; i<m_pPrintBookDM->GetRowCount() ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		m_pPrintDM->InsertRow(-1);
		nCurIdx = m_pPrintDM->GetRowCount()-1;
		ids = CopyDM(m_pPrintBookDM, i, m_pPrintDM, nCurIdx);
		if(ids < 0) return -1;
	}
	// UDF컬럼들을 설정한다.
	SetUDFDMFieldData(m_pPrintDM);

	//SubTotal PrintDM구축
	CGroupInfo * pGroup = NULL;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nGroupCnt = m_pGroupCollection->GetGroupCount();
	for(i=0 ; i<nGroupCnt ; i++)
	{
		pGroup = m_pGroupCollection->GetGroupInfo(i);
		ids = MakeSubTotalRow(m_pPrintDM, pGroup);
		if(ids < 0) return -1;
		if(!m_strSortCondition.IsEmpty())
		{
			ids = MakeSubTotalRowByGroup(m_pPrintDM, pGroup);
			if(ids < 0) return -1;
		}
	}

	return 0;
}

INT CMakeSubTotalBookPrintDM::SetUDFDMFieldData(CESL_DataMgr * pDM)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode, strOrderSerialNo, strTmpData, str; 
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT nColCnt = pDM->GetRefColumnCount();
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		for(INT j=0 ; j<nColCnt ; j++)
		{
			pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(FIELD_ALIAS == _T("UDF_차수정보"))
			{	
				INT nGroupNo, nWorkNo, nSerialNo;
				strAcqYear = pDM->GetCellData(_T("BP_수입년도"), i);
				strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), i);
				strGroupNo = pDM->GetCellData(_T("BP_그룹번호"), i);
				nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
				strWorkNo = pDM->GetCellData(_T("BP_작업번호"), i);
				nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
				strSerialNo = pDM->GetCellData(_T("BP_일련번호"), i);
				nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
				str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
				pDM->SetCellData(FIELD_ALIAS, str, i);
			}
			else if(FIELD_ALIAS == _T("UDF_할인가"))
			{
				CString strTmpData = pDM->GetCellData(_T("BB_가격"), i);
				DOUBLE dPrice, dDisc_Rate, dDisc_Price;
				strTmpData.Replace(_T(","), _T(""));
				dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
				strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
				dDisc_Rate = (DOUBLE)atof(strTmpData.GetBuffer(0));
				dDisc_Price = dPrice - (dPrice * (dDisc_Rate/100));
				strTmpData.Format(_T("%.0f"), dDisc_Price);
				pDM->SetCellData(_T("UDF_할인가"), strTmpData, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환할인가격"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("BB_가격"))
			{
				CString strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
				pDM->SetCellData(_T("UDF_변환가격"), strTmpData, i);
			}
			else if(FIELD_ALIAS == _T("BB_등록구분"))
			{
				CString strTmpData = pDM->GetCellData(FIELD_ALIAS, i);
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), strTmpData, strTmpData);
				pDM->SetCellData(FIELD_ALIAS, strTmpData, i);
			}
		}
	}	
	return 0;
}
