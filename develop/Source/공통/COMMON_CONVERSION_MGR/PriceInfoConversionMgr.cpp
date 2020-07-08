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
	CString strTmpData, strData1, strData2;
	CStringArray arrTmpData;	arrTmpData.RemoveAll();
	INT nLength = 0;
	INT nPos = -1;
	// 2005-01-13 소수점 이하는 , 표시를 하지 않도록 하기 위해 소수점 앞자리만 넘겨준다.
	nPos = strSrcData.Find(_T("."));
	if( nPos < 0 ) // _T(".")이 없는 경우
	{
		strData1 = strSrcData;
		strData2.Empty();
	}
	else
	{
		strData1 = strSrcData.Left(nPos);
		strData2 = strSrcData.Mid(nPos);
	}

	strData1.TrimLeft();		strData1.TrimRight();
	while(!strData1.IsEmpty())
	{
		nLength = strData1.GetLength();
		if(nLength >= 0)
			strTmpData = strData1.Right(3);
		else
			strTmpData = strData1;
		arrTmpData.InsertAt(0, strTmpData);
		if(nLength-3 >= 0)
			strData1 = strData1.Mid(0, nLength-3);
		else 
			strData1.Empty();
	}

	strDesData.Empty();
	INT cnt = arrTmpData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strDesData.IsEmpty()) strDesData = arrTmpData.GetAt(i);
		else strDesData += _T(",") + arrTmpData.GetAt(i);
	}
	strDesData += strData2;

	return 0;
}
