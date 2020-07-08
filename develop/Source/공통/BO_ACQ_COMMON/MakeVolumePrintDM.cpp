// MakeVolumePrintDM.cpp: implementation of the CMakeVolumePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeVolumePrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeVolumePrintDM::CMakeVolumePrintDM(CESL_Mgr * pMgr) : CMakePrintDM(pMgr)
{

}

CMakeVolumePrintDM::~CMakeVolumePrintDM()
{

}

INT CMakeVolumePrintDM::InitMakePrintDM()
{
	return 0;
}

INT CMakeVolumePrintDM::CheckValid()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pSourceBookDM == NULL) return -1;
	if(m_strDivAlias.IsEmpty()) return -1;
	return 0;
}

INT CMakeVolumePrintDM::MakePrintDM()
{
	INT ids ;
	if(m_nPrintType == BO_ACQ_CHECKIN_DATA)
		ids = MakeCheckInRelativeData(TRUE);
	else if(m_nPrintType == BO_ACQ_NONCHECKIN_DATA)
		ids = MakeCheckInRelativeData(FALSE);
	else if(m_nPrintType == BO_ACQ_CHECKIN_REQUEST_DATA)
		ids = MakeCheckInRequestData();

	return ids;
}

INT CMakeVolumePrintDM::MakeCheckInRelativeData(BOOL bCheckIn)
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids = MakeVolumeDM(m_pPrintDM, m_pSourceBookDM, m_strDivAlias, FALSE);
	if(ids < 0) return -1;
	
	SetUDFDMFieldData(m_pPrintDM, TRUE);
	
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_발주일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	CString strTmpData, strTmpAlias, strPrice, strDiscRate, strTmpPrice;
	DOUBLE dPrice;
	if(bCheckIn) strTmpAlias = _T("UDF_납품책수");
	else strTmpAlias = _T("UDF_미납책수");
	INT nRowCnt = m_pPrintDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);

		strTmpData = m_pPrintDM->GetCellData(strTmpAlias, i);
		if(strTmpData.IsEmpty() || strTmpData == _T("0"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strPrice = m_pPrintDM->GetCellData(_T("BB_가격"), i);
		strDiscRate = m_pPrintDM->GetCellData(_T("BP_할인율"), i);
		dPrice = (DOUBLE)atof(strPrice.GetBuffer(0)) * _ttoi(strTmpData.GetBuffer(0));
		strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(_T("UDF_변환가격"), strTmpPrice, i);
		dPrice = dPrice - (dPrice * ((DOUBLE)atof(strDiscRate.GetBuffer(0))/100));
		strTmpPrice.Format(_T("%.0f"), dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpPrice, strTmpPrice);
		m_pPrintDM->SetCellData(_T("UDF_변환할인가격"), strTmpPrice, i);
	}
	return 0;
}

INT CMakeVolumePrintDM::MakeCheckInRequestData()
{
	ShowDataProcessingDlg();
	m_pPrintDM->FreeData();
	INT ids = MakeVolumeDM(m_pPrintDM, m_pSourceBookDM, m_strDivAlias, FALSE);
	if(ids < 0) return -1;
	
	SetUDFDMFieldData(m_pPrintDM, TRUE);
	
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_수입년도");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_발주일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	CString strTmpData, strTmpAlias, strAcqKey, strQuery;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	
	strTmpAlias = _T("UDF_납품책수");
	INT nRowCnt = m_pPrintDM->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("출력 정보를 구축중입니다."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	for(INT i=nRowCnt-1 ; i>=0 ; i--)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nRowCnt-i);
		
		strTmpData = m_pPrintDM->GetCellData(strTmpAlias, i);
		if(strTmpAlias.IsEmpty() || strTmpAlias == _T("0"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strTmpData = m_pPrintDM->GetCellData(_T("BP_입력구분"), i);
		if(strTmpAlias != _T("2"))
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		strAcqKey = m_pPrintDM->GetCellData(_T("BP_수서정보KEY"), i);
		if(strAcqKey.IsEmpty()) 
		{
			m_pPrintDM->DeleteRow(i);
			continue;
		}
		
		strQuery.Format(_T("SELECT APPLICANT_NAME, APPLICANT_DATE FROM BO_FURNISH_TBL WHERE ACQ_KEY = %s"), strAcqKey);
		tmpDM.RestructDataManager(strQuery);
		if(tmpDM.GetRowCount() > 0) 
		{
			strTmpData = tmpDM.GetCellData(0, 0);
			m_pPrintDM->SetCellData(_T("UDF_신청자"), strTmpData, i);
			strTmpData = tmpDM.GetCellData(0, 1);
			m_pPrintDM->SetCellData(_T("UDF_신청일"), strTmpData, i);
		}
	}
	return 0;
}
