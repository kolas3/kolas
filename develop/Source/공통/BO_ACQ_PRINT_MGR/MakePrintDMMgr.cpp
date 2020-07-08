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
#include "../DATA_SORT_MANAGER/SortItemSet.h"

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
	if( strPath.IsEmpty() == TRUE )		return -1;
	
	if		( strPath == _T("일반리스트출력")		)	return MakeSimplePrintDM();
	else if	( strPath == _T("신청자료도착목록출력") )	return MakeRequestDataPrintDM();
	else if	( strPath == _T("검수완료자료출력")		)	return MakeCheckInCompletePrintDM();
	else if	( strPath == _T("소계자료출력")			)	return MakeSubTotalPrintDM();
	else if	( strPath == _T("책소계자료출력")		)	return MakeSubTotalBookPrintDM();
	else if	( strPath == _T("권리스트출력")			)	return MakeVolumePrintDM();
	else if	( strPath == _T("일련번호설정")			)	return SetDataSerialNo();
	else if	( strPath == _T("비치희망자료출력")		)	return MakeRequestPrintDM();
	
	return -1;
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - [일반리스트출력] DM 구성
//
// [ 미니스펙 ]
//
//////////////////////////////////////////////////////////////////////
INT CMakePrintDMMgr::MakeSimplePrintDM()
{
	//================================================================
	// 변수 정의
	//================================================================
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	CMapStringToString* pArrDivValue = NULL;
	CSortItemSet* pSortItem = NULL;
	CDataProcessingDlg* pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr* pBookDM = NULL;
	CESL_DataMgr* pPrintDM = NULL;
	CESL_DataMgr* pPrintSourceDM = NULL;
	CESL_DataMgr* pPrintObjDM = NULL;

	


	//================================================================
	// 1. 저장된 값을 얻어온다.
	//		- 프린트타입
	//		- 구분알리아스
	//		- 책수알리아스
	//		- 가격알리아스
	//		- 할인가알리아스
	//		- 출력자료정렬여부	
	//================================================================
	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	GetTempData(_T("출력자료정렬여부"), bDataSortYN);


	//================================================================
	// 2. 저장된 포인터를 얻어온다.
	//		- 프로그레스 컨트롤 창
	//		- 원본책 DM		(DM_구입자료관리)
	//		- 원본출력 DM	(DM_구입자료관리_종)
	//		- 출력 DM		(DM_구입자료관리_출력용)
	//		- 정렬조건
	//		- 그리드에 선정된 Key값들 (매핑)	
	//================================================================
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("그리드선정자료"));
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	

	//================================================================
	// 3. 가져온 데이터 및 포인터를 멤버변수로 설정한다.
	//================================================================
	PrintMgr.SetPrintBookDM(pBookDM);		// 원본책 DM
	PrintMgr.SetPrintSourceDM(pPrintDM);	// 원본출력 DM
	PrintMgr.SetDataProcessingDlg(pDlg);	// Progress Ctrl Dialog
	PrintMgr.SetPrintDM(pPrintObjDM);		// 출력 DM
	PrintMgr.SetPrintType(nPrintType);		// 프린트 타입 - BO_ACQ_CHECKING_DATA,...
	PrintMgr.SetDivAlias(strDivAlias);		// 구분알리아스 - BP_수서정보KEY,...
											// 책수알리아스, 가격알리아스, 할인가알리아스
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);		// 정렬조건
	PrintMgr.SetDataSortYN(bDataSortYN);	// 정렬여부
	PrintMgr.SetGridSelectDataArray(pArrDivValue);	// 선정된 그리드에 매핑된 Key값들


	//================================================================
	// 4. 프린트타입에 따라서 CMakeSimplePrintDM에 멤버변수를 설정한다.
	//================================================================
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;


	//================================================================
	// 5. DM을 구성한다.
	//================================================================
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}


///////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 소계자료 DM 구성
//
// [ 미니스펙 ]
///////////////////////////////////////////////////////////////////////
INT CMakePrintDMMgr::MakeSubTotalPrintDM()
{
	//================================================================
	// 변수정의
	//================================================================
	INT nPrintType, nPrintDataType;
	BOOL bKOLAS2Format = FALSE;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CGroupCollection * pGroupCollection = NULL;
	// 클래스
	CMakeSubTotalPrintDM PrintMgr(m_pParentMgr);



	//================================================================
	// 1. 설정된 데이터와 포인터를 가져와 CMakeSubTotalPrintDM에 그대로 설정한다.
	//================================================================
	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	
	GetTempData(_T("정렬필드알리아스"), strSortFieldAlias);
	GetTempData(_T("책DM알리아스"), strBookDMAlias);
	GetTempData(_T("KOLAS2형식"), bKOLAS2Format);

	GetTempData(_T("출력자료정렬여부"), bDataSortYN);
	GetTempData(_T("출력데이터타입"), nPrintDataType);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹컬렉션"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("그리드선정자료"));

	if( pGroupCollection == NULL || pPrintDM == NULL )	 return -1;

	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetBookDMAlias(strBookDMAlias);
	PrintMgr.SetKolas2FormatYN(bKOLAS2Format);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetPrintDataType(nPrintDataType);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	

	//================================================================
	// 2. DM을 설정하기 위해 초기화(멤버변수 설정)한다.
	//================================================================
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;


	//================================================================
	// 3. DM을 구성한다.
	//================================================================
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	else if(ids == 1000) return ids;

	return 0;
}

INT CMakePrintDMMgr::MakeVolumePrintDM()
{
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeVolumePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias;
	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("출력자료정렬여부"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("그리드선정자료"));
	
	if(pBookDM == NULL || pPrintDM == NULL) return -1;
	
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}

INT CMakePrintDMMgr::MakeRequestDataPrintDM()
{
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	GetTempData(_T("프린트타입"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	GetTempData(_T("출력자료정렬여부"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("그리드선정자료"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	

	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeRequestDataPrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeCheckInCompletePrintDM()
{
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	GetTempData(_T("프린트타입"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("책수알리아스"), strBookCntAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	GetTempData(_T("출력자료정렬여부"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeCheckInCompletePrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeSubTotalBookPrintDM()
{
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	CGroupCollection * pGroupCollection = NULL;
	// 클래스
	CMakeSubTotalBookPrintDM PrintMgr(m_pParentMgr);

	GetTempData(_T("프린트타입"), nPrintType);
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("가격알리아스"), strPriceAlias);
	GetTempData(_T("할인가알리아스"), strDiscPriceAlias);
	GetTempData(_T("정렬필드알리아스"), strSortFieldAlias);
	GetTempData(_T("출력자료정렬여부"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("그룹컬렉션"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("원본책DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("그리드선정자료"));
	
	if( pGroupCollection == NULL || pPrintDM == NULL || pBookDM == NULL )	 return -1;
	
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;

	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	else if(ids == 1000) return ids;
	
	return 0;
}

INT CMakePrintDMMgr::SetDataSerialNo()
{
	CESL_DataMgr * pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	if(pPrintDM == NULL) return -1;

	CMakePrintDM PrintMgr(m_pParentMgr);
	return PrintMgr.SetDataSerialNo(pPrintDM);
}


//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - Grid에 선정된 자료들을 Alias에 해당하는 값을 얻어와 키값으로 인자에 매핑한다.
//
// [ 미니스펙 ]
// 1. Grid와 DM이 없다면 리턴한다.
// 2. Grid에 자료가 없다면 리턴한다.
// 3. Grid에 선정체크된 자료들을 Grid 멤버변수에 저장한다.
// 4. Grid에 선정체크된 자료들 중 맨 윗 자료를 선택한다.
// 5. 선정체크된 자료들에 해당하여 인자로 들어온 Alias의 데이터값을 추출한다.
// 6. 추출한 값을 키값으로 매핑한다.
//
//////////////////////////////////////////////////////////////////////
INT CMakePrintDMMgr::GetGridSelectData(CESL_Grid * pGrid, CESL_DataMgr * pDM, CString strDivAlias, CMapStringToString *pArrDivValue, CString strExtraAlias/*=_T("")*/)
{
	//===============================================================
	// 변수 정의
	//===============================================================
	INT ids;
	CString strTmpData;

	//===============================================================
	// 1. Grid와 DM이 없다면 리턴한다.
	//===============================================================
	if(pGrid == NULL || pDM == NULL) return -1;


	//===============================================================
	// 2. Grid에 자료가 없다면 리턴한다.
	//===============================================================
	if(pDM->GetRowCount() == 0) return -1;
	

	//===============================================================
	// 3. Grid에 선정체크된 자료들을 Grid 멤버변수에 저장한다.
	//===============================================================
	ids = pGrid->SelectMakeList();
	if(ids < 0) return ids;


	//===============================================================
	// 4. Grid에 선정체크된 자료들 중 맨 윗 자료를 선택한다.
	//===============================================================
	ids = pGrid->SelectGetHeadPosition();
	
	//===============================================================
	// 5. 선정체크된 자료들에 해당하여 인자로 들어온 Alias의 데이터값을 추출한다.
	// 6. 추출한 값을 키값으로 매핑한다.
	//===============================================================
	while(ids >= 0)
	{
		strTmpData = pDM->GetCellData(strDivAlias, ids);

		if( strTmpData.IsEmpty() == FALSE )
		{
			if( strExtraAlias.IsEmpty() == TRUE )
			pArrDivValue->SetAt(strTmpData, _T(""));
		else
		{
			strTmpData += _T("-") + pDM->GetCellData(strExtraAlias, ids);
			pArrDivValue->SetAt(strTmpData, _T(""));
			}
		}
		ids = pGrid->SelectGetNext();
	}
	return 0;
}

INT CMakePrintDMMgr::MakeRequestPrintDM()
{
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pSourceDM = NULL;
	CESL_DataMgr * pSourcePrintDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	
	GetTempData(_T("프린트타입"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("구분알리아스"), strDivAlias);
	GetTempData(_T("출력자료정렬여부"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("프로그래스컨트롤"));
	pSourceDM = (CESL_DataMgr*)GetTempPtr(_T("원본DM"));
	pSourcePrintDM = (CESL_DataMgr*)GetTempPtr(_T("원본출력DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("출력DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("정렬조건"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("그리드선정자료"));
	
	if(pSourceDM == NULL || pSourcePrintDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pSourceDM);
	PrintMgr.SetPrintSourceDM(pSourcePrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	
	return 0;
}
