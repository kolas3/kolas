// PriceInfoConversionMgr.cpp: implementation of the PriceInfoConversionMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PriceInfoConversionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPriceInfoConversionMgr::CPriceInfoConversionMgr()
{

}

CPriceInfoConversionMgr::~CPriceInfoConversionMgr()
{

}

INT CPriceInfoConversionMgr::ConversionPriceInfoToDesc(CESL_DataMgr *pDM, CString strAlias, CString &strConvData, INT nIdx)
{
	if(pDM == NULL) return -1;
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt <= nIdx) return -1;
	CString strPriceInfo = pDM->GetCellData(strAlias, nIdx);
	ConversionPriceInfoToDesc(strPriceInfo, strConvData);
	return 0;
}

INT CPriceInfoConversionMgr::ConversionPriceInfoToDescInDM(CESL_DataMgr *pDM, CString strSrcAlias, CString strDesAlias, INT nIdx/*=-1*/)
{
	if(pDM == NULL) return -1;
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return 0;

	CString strConvData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		ConversionPriceInfoToDesc(pDM, strSrcAlias, strConvData, i);
		pDM->SetCellData(strDesAlias, strConvData, i);
	}
	return 0;
}	

INT CPriceInfoConversionMgr::ConversionDescToPriceInfo(CESL_DataMgr *pDM, CString strAlias, CString &strConvData, INT nIdx)
{
	if(pDM == NULL) return -1;
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt <= nIdx) return -1;
	
	CString strSrcData = pDM->GetCellData(strAlias, nIdx);
	strSrcData.Replace(_T(","), _T(""));
	strConvData = strSrcData;
	return 0;
}

INT CPriceInfoConversionMgr::ConversionDescToPriceInfoInDM(CESL_DataMgr *pDM, CString strSrcAlias, CString strDesAlias, INT nIdx)
{
	if(pDM == NULL) return -1;
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return 0;

	CString strConvData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		ConversionDescToPriceInfo(pDM, strSrcAlias, strConvData, i);
		pDM->SetCellData(strDesAlias, strConvData, i);
	}
	return 0;
}

INT CPriceInfoConversionMgr::ConversionPriceInfoToDesc(CString strSrcData, CString &strDesData)
{
	CString strTmpData;
	CStringArray arrTmpData;	arrTmpData.RemoveAll();
	INT nLength = 0;
	strSrcData.TrimLeft();		strSrcData.TrimRight();
	while(!strSrcData.IsEmpty())
	{
		nLength = strSrcData.GetLength();
		if(nLength >= 0)
			strTmpData = strSrcData.Right(3);
		else
			strTmpData = strSrcData;
		arrTmpData.InsertAt(0, strTmpData);
		if(nLength-3 >= 0)
			strSrcData = strSrcData.Mid(0, nLength-3);
		else 
			strSrcData.Empty();
	}

	strDesData.Empty();
	INT cnt = arrTmpData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strDesData.IsEmpty()) strDesData = arrTmpData.GetAt(i);
		else strDesData += _T(",") + arrTmpData.GetAt(i);
	}
	return 0;
}
