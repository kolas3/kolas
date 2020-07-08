// MakePrintDMMgr.cpp: implementation of the CMakePrintDMMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakePrintDMMgr.h"
#include "MakeSimplePrintDM.h"
#include "MakeSubTotalPrintDM.h"
#include "MakeVolumePrintDM.h"
#include "MakeSubTotalBookPrintDM.h"
#include "../K2UP_Common/DataProcessingDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakePrintDMMgr::CMakePrintDMMgr(CESL_Mgr * pMgr) : CSPF_Work(pMgr)
{

}

CMakePrintDMMgr::~CMakePrintDMMgr()
{

}

INT CMakePrintDMMgr::SPFExecutePath(CString strPath)
{
	if(strPath.IsEmpty()) return -1;
	
	if(strPath == _T("일반리스트출력"))
		return MakeSimplePrintDM();
	else if(strPath == _T("신청자료도착목록출력"))
		return MakeRequestDataPrintDM();
	else if(strPath == _T("검수완료자료출력"))
		return MakeCheckInCompletePrintDM();
	else if(strPath == _T("소계자료출력"))
		return MakeSubTotalPrintDM();
	else if(strPath == _T("책소계자료출력"))
		return MakeSubTotalBookPrintDM();
	else if(strPath == _T("권리스트출력"))
		return MakeVolumePrintDM();
	return -1;
}

INT CMakePrintDMMgr::MakeSimplePrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	GetTempData(_T("프린트타입"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));

	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;

	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);

	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}

INT CMakePrintDMMgr::MakeSubTotalPrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	BOOL bKOLAS2Format = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CGroupCollection * pGroupCollection = NULL;
	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	GetTempData(_T("책DM알리아스"), strBookDMAlias);
	GetTempData(_T("정렬필드알리아스"), strSortFieldAlias);
	GetTempData(_T("KOLAS2형식"), bKOLAS2Format);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹컬렉션"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));

	if(pGroupCollection == NULL || pPrintDM == NULL) return -1;

	CMakeSubTotalPrintDM PrintMgr(m_pParentMgr);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetBookDMAlias(strBookDMAlias);
	PrintMgr.SetKolas2FormatYN(bKOLAS2Format);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	if(ids == 1000) return ids;

	return 0;
}

INT CMakePrintDMMgr::MakeVolumePrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeVolumePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	CString strDivAlias;
	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("구분알리아스"), strDivAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	
	if(pBookDM == NULL || pPrintDM == NULL) return -1;
	
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	
	INT ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}

INT CMakePrintDMMgr::MakeRequestDataPrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	GetTempData(_T("프린트타입"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeRequestDataPrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeCheckInCompletePrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	GetTempData(_T("프린트타입"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeCheckInCompletePrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeSubTotalBookPrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	INT nPrintType;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CGroupCollection * pGroupCollection = NULL;
	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	GetTempData(_T("정렬필드알리아스"), strSortFieldAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹컬렉션"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	
	if(pGroupCollection == NULL || pPrintDM == NULL || pBookDM == NULL) return -1;
	
	CMakeSubTotalBookPrintDM PrintMgr(m_pParentMgr);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	if(ids == 1000) return ids;
	
	return 0;
}
