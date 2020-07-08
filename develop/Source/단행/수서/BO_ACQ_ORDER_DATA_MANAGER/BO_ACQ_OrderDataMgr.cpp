// BO_ACQ_OrderDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_OrderDataMgr.h"
#include "BO_ACQ_CheckInDlg.h"
#include "OrderDataCancelDlg.h"
#include "OrderCancelDivnoSelectDlg.h"
#include "OrderDataPrintDlg.h"
#include "DataExportDlg.h"
#include "../BO_ACQ_STATISTICS/BO_Purchase_Statistics.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../공통/BO_ACQ_COMMON/OrderMailMgr.h"
#include "../../../공통/BO_ACQ_COMMON/ViewTransferDataDlg.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
// 2004.10.29 
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileImportMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileExportMgr.h"

#include "../../../공통/CMarcChanger/MARC_CHANGER_01.h"
#include "sbl.h"

//==============================================
//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
#include "SBL.H"
//==============================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderDataMgr dialog

CBO_ACQ_OrderDataMgr::CBO_ACQ_OrderDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_OrderDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_OrderDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRow		= -1;
	m_nEditCol	= -1;	
	m_bCellEditing	= TRUE;
	m_strDivNo		= _T("");
	m_strAcqYear	= _T("");
	m_strPosNumber	= _T("");
	m_strIs5163		= _T("");
	m_strIs64		= _T("");
	m_strOrderPrice_Unit = _T("");

	m_pCM				 = NULL;
	m_pDM = NULL;
	m_pActiveDM = NULL;
	m_pPrintDM_Species = NULL;
	m_pPrintDM_Volume = NULL;
	m_pSpeciesPrintDM	 = NULL;	
	m_pGrid_Species = NULL;
	m_pGrid_Volume = NULL;
	m_pSearchDlg		 = NULL;
	m_pResultSearchDlg	 = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;

}
CBO_ACQ_OrderDataMgr::~CBO_ACQ_OrderDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pResultSearchDlg)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}
	if(m_pPrintDlg)
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CBO_ACQ_OrderDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_OrderDataMgr)
	DDX_Control(pDX, IDC_tabORDER_DATA, m_tabOrderData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_OrderDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_OrderDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnRESULT_SEARCH, OnbtnRESULTSEARCH)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabORDER_DATA, OnSelchangetabORDERDATA)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)	
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderDataMgr message handlers

VOID CBO_ACQ_OrderDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// 클래스 초기화
//
// [ 미니스펙 ]
// 1. SM 초기화 
// 2. DM 초기화
// 3. CM 초기화
// 4. 우선정리자료 사용여부 값을 디비로 부터 받아온다.
// 5. 가격 절사단위 설정값을 가져와 멤버변수에 저장한다.
// 6. 다이알로그 내에 리소스들을 초기화 한다.
// 7. CM으로 부터 종정보/권정보 그리드 컨트롤 포인터를 가져온다.
// 8. 권정보 그리드를 첫화면에 활성화시키고, 종정보 그리드는 숨긴다.
// 9. 검색창 및 출력물창을 멤버변수에 등록한다.
// 10. 설정된 정렬항목을 검색시 정렬되도록 검색클래스에 적용한다.
// 11. 마크변환관리 클래스에 마크관리 클래스를 등록한다.
// 12. 종,권,책수에 대한 메시지를 알려주고
//	   [CM_주문자료관리]에 그룹번호와 수입년도를 설정한다.
// 15. 종/권정보 그리드의 팝업메뉴 설정 
// 16. 툴바버튼을 설정한다.
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_OrderDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 변수 정의	
	//=======================================================================
	CString strPriorityCatalogYN, strValue;

	CString strErrMsg = _T("");
	INT ids = 0 ;
		

	//=======================================================================
	// 1. SM 초기화 
	//=======================================================================
	if (InitESL_Mgr(_T("K2UPGRADE_주문자료관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	//=======================================================================
	// 2. DM 초기화
	//		- 초기화 된 SM에 대한 DM을 찾는다.
	//=======================================================================
	m_pDM				= FindDM(_T("DM_주문자료관리")			);
	m_pPrintDM_Species	= FindDM(_T("DM_주문자료관리_종")		);
	m_pPrintDM_Volume	= FindDM(_T("DM_주문자료관리_권")		);
	m_pSpeciesPrintDM = FindDM(_T("DM_주문자료관리_종_출력용"));
	
	if( m_pDM == NULL || m_pPrintDM_Species == NULL || m_pPrintDM_Volume == NULL || m_pSpeciesPrintDM == NULL)
	{
		ESLAfxMessageBox(_T("DM 초기화 실패\n [ DM_주문자료관리 ]\n [ DM_주문자료관리_종 ]\n [ DM_주문자료관리_권 ]\n [ DM_주문자료관리_종_출력용 ]\n [ SM=K2UPGRADE_주문자료관리 ]"));
		return FALSE;
	}

	//=======================================================================		
	// 3. CM 초기화
	//=======================================================================		
	m_pCM = FindCM(_T("CM_주문자료관리"));
	if( m_pCM == NULL ) 
	{
		ESLAfxMessageBox( _T("CM_주문자료관리 error") );
		return FALSE;
	}
	
	
	//=======================================================================
	// 4. 우선정리자료 사용여부 값을 디비로 부터 받아온다.
	//
	// 2004.11.16 우선정리자료 사용여부에 따라 CM을 다른 걸로 정의하여 사용한다.
	//=======================================================================	
	ids = m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), 	strPriorityCatalogYN);
	if( ids < 0 )
	{
		switch (ids)
		{
		case -4004:	strErrMsg.Format(_T("[ %d ] : 데이터베이스와 접속중이지 않습니다.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4005:	strErrMsg.Format(_T("[ %d ] : 질의문(Query)에 대한 실행시 문제가 있습니다.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4006:	strErrMsg.Format(_T("[ %d ] : 질의문(Query)에 대한 결과가 너무 많습니다.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4007:	strErrMsg.Format(_T("[ %d ] : 질의문(Query)에 대한 결과가 없습니다.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4008:	strErrMsg.Format(_T("[ %d ] : 검색필드가 결과필드보다 많습니다.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4999:	strErrMsg.Format(_T("[ %d ] : Oracle을 지원중인지 확인하십시오.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		default:	strErrMsg.Format(_T("[ %d ] : Unknown Error"), ids);
			break;
		}
		ESLAfxMessageBox(strErrMsg);
	}
	
	//=======================================================================
	// 5. 가격 절사단위 설정값을 가져와 멤버변수에 저장한다.
	//		- 0 : 원단위 절사
	//		- 1 : 소수점 1자리 이하 절사
	//		- 2 : 소수점 2자리 이하 절사 [기본값]
	//
	// 2005-01-13 가격 자릿수 설정값을 가져온다.
	//=======================================================================		
	ids = GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("가격절사단위설정값을 불러오지 못했습니다."));
	}
	
	// 발주가격단위설정을 가져온다. - 디폴트 0원단위
	ids = GetManageValue(_T("수서"), _T("단행"), _T("구입가격단위설정"), _T(""), m_strOrderPrice_Unit);
	if( ids < 0 )		m_strOrderPrice_Unit = _T("0");
	

	//=======================================================================
	// 6. 5163, 육사인지 판별값을 멤버변수로 저장한다.
	//=======================================================================
	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), m_strIs64);
	
	
	//=======================================================================
	// 7. 다이알로그 내에 리소스들을 초기화 한다.
	//		- 에디트박스 및 버튼을 숨긴다.
	//		- 탭컨트롤을 초기화한다.
	//=======================================================================
	GetDlgItem(IDC_stcORDER_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtORDER_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcORDER_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtORDER_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);;	
	GetDlgItem(IDC_btnRESULT_SEARCH)->ShowWindow(SW_HIDE);	
	m_tabOrderData.InsertItem(0, _T("종정보"));
	m_tabOrderData.InsertItem(1, _T("권정보"));
	m_tabOrderData.SetCurSel(1);

	//=======================================================================
	// 8. CM으로 부터 종정보/권정보 그리드 컨트롤 포인터를 가져온다.
	//		- 3,4 ***
	//=======================================================================
	m_pGrid_Species = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid_Volume = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
	m_pGrid_Species->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Volume->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;


	//=======================================================================
	// 9. 권정보 그리드를 첫화면에 활성화시키고, 종정보 그리드는 숨긴다.
	//		- 초기 기본값은 권정보가 활성화 DM이다.
	// 2004.10.15 화면과 ActiveDM 바뀜.
	//=======================================================================
	m_pGrid_Species->ShowWindow(SW_HIDE);
	m_pGrid_Volume->ShowWindow(SW_SHOW);
	m_pActiveDM = m_pPrintDM_Volume;


	//=======================================================================
	// 10. 검색창 및 출력물창을 멤버변수에 등록한다.
	//		- GroupCollection ***
	//=======================================================================
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_ORDER, PURCHASE, BO_ORDER, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_ORDER, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());


	//=======================================================================
	// 11. 설정된 정렬항목을 검색시 정렬되도록 검색클래스에 적용한다.
	//		- 정렬클래스에 의해 정의된 값은 멤버변수로 갖는다.
	//=======================================================================
	InitSortItem(m_pGrid_Volume, _T("MainGrid_Volume"));
	
	
	//=======================================================================
	// 12. 마크변환관리 클래스에 마크관리 클래스를 등록한다.
	//=======================================================================
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);


	//=======================================================================
	// 13. 종,권,책수에 대한 메시지를 알려주고
	//	   [CM_주문자료관리]에 그룹번호와 수입년도를 설정한다.
	//=======================================================================
	SetOrderDataInfo(m_tabOrderData.GetCurSel());
	
	//=======================================================================	
	// 16. 종/권정보 그리드의 팝업메뉴 설정 
	//     권정보그리드의 _T("납품책수") 수정을 그리드상에서 가능하도록 설정
	//=======================================================================	
	InitPopupMenuItem();


	//=======================================================================	
	// 17. 툴바버튼을 설정한다.
	//=======================================================================	
	SetToolBarBtnByTabSelect();

	return TRUE;  
}

VOID CBO_ACQ_OrderDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabORDER_DATA);
	if(pTab->GetSafeHwnd() == NULL) return;
	pTab->MoveWindow(0, 0, cx+4, cy);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridORDER_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 60, cx+2, cy);
	pGrid->MoveWindow(-2, 60, cx+2, cy-60);		
	//==========================================
	

	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridORDER_VOLUME_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : 스크롤바 나오게 하기위헤서 세로길이 줄임
	//pGrid->MoveWindow(-2, 60, cx+2, cy);
	pGrid->MoveWindow(-2, 60, cx+2, cy-60);		
	//==========================================

	CRect rect;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_stcORDER_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtORDER_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 32, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcORDER_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtORDER_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 32, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_btnRESULT_SEARCH);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(400, 32, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
}

INT CBO_ACQ_OrderDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	INT nGroupNo, nSerialNo, nWorkNo, nOrderSerialNo, ids;	
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode, strOrderSerialNo;

	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		strTmpData = m_pActiveDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
			m_pDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);//lks
		}
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		}
		strTmpData.Empty();	 strTmpData = m_pActiveDM->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
			m_pDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);//lks
		}
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
			m_pDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);//lks
		}
		m_nRow = nRow;
		if(m_nRow == m_pActiveDM->GetRowCount()-1) m_nRow = -1;
	}

	if( strDMFieldAlias == _T("UDF_차수정보") )
	{	
		strAcqYear = m_pActiveDM->GetCellData(_T("BP_수입년도"), nRow);
		strWorkCode = m_pActiveDM->GetCellData(_T("IBS_업무구분"), nRow);
		strGroupNo = m_pActiveDM->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pActiveDM->GetCellData(_T("BP_작업번호"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pActiveDM->GetCellData(_T("BP_일련번호"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		strOrderSerialNo = m_pActiveDM->GetCellData(_T("BP_발주일련번호"), nRow);
		nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
		strTmpData.Format(_T("%04d-%05d"), nGroupNo, nOrderSerialNo);
		m_pActiveDM->SetCellData(_T("UDF_출력차수정보"), strTmpData, nRow);
		m_pDM->SetCellData(_T("UDF_차수정보"), strTmpData, nRow);//lks
	}
	else if( strDMFieldAlias == _T("UDF_입력구분설명") )
	{
		strTmpData = m_pActiveDM->GetCellData(_T("BP_입력구분"), nRow);
		if(strTmpData == _T("0")) str = _T("화면입력");
		else if(strTmpData == _T("1")) str = _T("재활용반입");
		else if(strTmpData == _T("2")) str = _T("희망자료반입");
		else if(strTmpData == _T("3")) str = _T("파일반입");
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if( strDMFieldAlias == _T("BP_주제구분") )
	{
		strTmpData = m_pActiveDM->GetCellData(_T("IBS_분류표구분"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("2") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("3") || strTmpData == _T("7") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC주제부호"), str, str);
		}
		else //if(strTmpData.IsEmpty()) 
		{
			str = _T("");
		}

		m_pActiveDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
		m_pDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);//lks
	}
	else if( strDMFieldAlias == _T("UDF_주제구분설명") )
	{
		str = m_pActiveDM->GetCellData(_T("BP_주제구분"), nRow);
		strTmpData = m_pActiveDM->GetCellData(_T("IBS_분류표구분"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("2") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
		else if	( strTmpData == _T("3") || strTmpData == _T("7") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC주제부호"), str, str);
		}
		else //if(strTmpData.IsEmpty()) 
		{
			str = _T("");
		}

		m_pActiveDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
		m_pDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);//lks
	}
	else if( strDMFieldAlias == _T("UDF_구입처부호설명") )
	{
		strTmpData = m_pActiveDM->GetCellData(_T("BP_구입처부호"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);//lks
	}
	// 권발주가격설정
	else if( strDMFieldAlias == _T("UDF_할인가") )
	{
		//20070830 ADD PJW : 구입정보 변경에 따른 발주가격, 할인가를 DM에 셋한다.댜
		CString strBookPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		DOUBLE dBookPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;

		strBookPrice = m_pActiveDM->GetCellData(_T("UDF_변환가격"), nRow);
		strBookPrice.Replace(_T(","), _T(""));

		strPriceDiscountRate = m_pActiveDM->GetCellData(_T("BP_할인율"), nRow);
		strExchangeRate = m_pActiveDM->GetCellData(_T("BP_환율"), nRow);
		//dBookPrice = atof(strBookPrice);
		dBookPrice = _tcstod(strBookPrice.GetBuffer(0), &pEndPtr);

		//dExchangeRate = atof(strExchangeRate);
		dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

		//dPriceDiscountRate = atof(strPriceDiscountRate);
		dPriceDiscountRate = _tcstod(strPriceDiscountRate.GetBuffer(0), &pEndPtr);

		dOrderPRICE = dBookPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 );
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pActiveDM->SetCellData(_T("UDF_발주가격_권"), strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_할인가"), strTmpData, nRow);
		//

		DOUBLE dPrice;
		INT	ids;
		ids = m_pActiveDM->GetCellData(_T("UDF_발주가격_권"), nRow, strTmpData);
		if ( strTmpData == _T("") ) ids = m_pActiveDM->GetCellData(_T("UDF_할인가"), nRow, strTmpData);
		strTmpData.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		//2007.05.04 PJW 수정 원화일경우 소수점 제외
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			if( dPrice == 0 )	strTmpData = _T("0");
			else
			{
				if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else										strTmpData.Format(_T("%.0f"), dPrice);
			}
		}
		/*
		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}
		*/
		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_변환할인가격"), str, nRow);
	}
	else if( strDMFieldAlias == _T("UDF_가격") )
	{
		INT nBookCnt = 0;
		DOUBLE dPrice = 0.0;
		strTmpData = m_pActiveDM->GetCellData(_T("BB_가격"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pActiveDM->GetCellData(_T("UDF_책수"), nRow);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		//2007.05.04 PJW 수정 원화일경우 소수점 제외
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			dPrice = dPrice*nBookCnt;
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			strTmpData = TruncPrice(m_pActiveDM, dPrice*nBookCnt);
		}
		// 2004.12.03
		//strTmpData = TruncPrice(m_pActiveDM, dPrice*nBookCnt);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pActiveDM->SetCellData(_T("UDF_변환가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_종가격"))
	{
		// 2004.09.14  화폐구분값을 소수점 2자리까지 나타낸다.
		str.Replace(_T(","), _T(""));
		//2007.05.04 PJW 수정 원화일경우 소수점 제외
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			str.Format(_T("%d"), (INT)_ttoi(str));
			m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
			//str.Replace(_T(","), _T(""));
		}
		else
		{
			//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));
			str = TruncPrice(m_pActiveDM, _tcstod(str.GetBuffer(0), &pEndPtr));

			m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
			str.Replace(_T(",."), _T("."));
		}

		//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));
		//m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		// 2004.09.14
		//str.Replace(_T(",."), _T("."));
		
		m_pActiveDM->SetCellData(_T("UDF_변환종가격"), str, nRow);
	}
	// 종발주가격 설정
	else if( strDMFieldAlias == _T("UDF_할인종가격") )
	{
		//20070830 ADD PJW : 구입정보 변경에 따른 발주가격, 할인종가격을 DM에 셋한다.
		CString strSpeciesPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		CString strMoney;
		CString strVolRowCnt;
		CString strSpeciesKey;

		DOUBLE dSpeciesPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;
		INT iBookRowCnt;

		iBookRowCnt = m_pDM->GetRowCount(); 
		strSpeciesPrice = m_pActiveDM->GetCellData( _T("UDF_종가격"), nRow );
		strPriceDiscountRate = m_pActiveDM->GetCellData( _T("BP_할인율"), nRow );
		strExchangeRate = m_pActiveDM->GetCellData( _T("BP_환율"), nRow );
		strMoney = m_pActiveDM->GetCellData( _T("BP_화폐구분"), nRow );
		strSpeciesKey = m_pActiveDM->GetCellData( _T("IBS_종KEY"), nRow );

		for( INT i = 0 ; i < iBookRowCnt ; i++ )
		{
			if( strSpeciesKey == m_pDM->GetCellData( _T("IBS_종KEY"), i ) )
			{
				m_pDM->SetCellData(_T("BP_환율"), strExchangeRate , i);
				m_pDM->SetCellData(_T("BP_할인율"), strPriceDiscountRate , i);
				m_pDM->SetCellData(_T("BP_화폐구분"), strMoney, i);
			}
		}

		//dSpeciesPrice = atof(strSpeciesPrice);
		dSpeciesPrice = _tcstod(strSpeciesPrice.GetBuffer(0), &pEndPtr);

		//dExchangeRate = atof(strExchangeRate);
		dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

		//dPriceDiscountRate = atof(strPriceDiscountRate);
		dPriceDiscountRate = _tcstod(strPriceDiscountRate.GetBuffer(0), &pEndPtr);

		dOrderPRICE = dSpeciesPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 );
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pActiveDM->SetCellData(_T("BP_발주가격"), strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_할인종가격"), strTmpData, nRow);
		//

		DOUBLE dPrice;
		strTmpData = m_pActiveDM->GetCellData(_T("BP_발주가격"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		//2007.05.04 PJW 수정 원화일경우 소수점 제외
		if( _T("\\") ==  strMoney )
		{
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			if( dPrice == 0 )	strTmpData = _T("0");
			else
			{
				if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else										strTmpData.Format(_T("%.0f"), dPrice);
			}
		}
		/*
		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if( m_strOrderPrice_Unit == _T("1") )		strTmpData.Format(_T("%.1f"), dPrice);
			else if( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}*/
		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_변환할인종가격"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_단가"))
	{
		// 2004.09.14  화폐구분값을 소수점 2자리까지 나타낸다.
		str.Replace(_T(","), _T(""));
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_화폐구분"), nRow) )
		{
			str.Format(_T("%d"), (INT)_ttoi(str));
		}
		else
		{
			//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));	
			str = TruncPrice(m_pActiveDM, _tcstod(str.GetBuffer(0), &pEndPtr));	
		}
		//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_OrderDataMgr::DisplayGrid()
{
	INT nCurSel;
	BOOL bSetTopRow = TRUE;
	nCurSel = GetActiveGrid()->GetTopRow();

	if( nCurSel < 0 )								bSetTopRow = FALSE;
	if( GetActiveDM()->GetRowCount() == 0 )			bSetTopRow = FALSE;
	if( nCurSel > GetActiveDM()->GetRowCount() )	bSetTopRow = FALSE;

	GetActiveGrid()->Display();

	if( bSetTopRow == TRUE )	GetActiveGrid()->SetTopRow(nCurSel);
}


//###########################################################################
//
// [ 함수설명 ]
// - 종,책,권수에 대한 메시지를 보내주고, 
//	 [CM_주문자료관리]에 그룹번호와 수입년도를 설정한다.
//
// [ 미니스펙 ] 
// 1. 책수는 0보다 크고, 종수는 0이라면 책수만큼 종수를 설정한다.
// 2. 책수는 0보다 크고, 권수는 0이라면 책수만큼 권수를 설정한다.
// 3. 검색된 자료에 대한 메시지를 작성하여 보낸다. (메시지내용 == 1,2번 내용)
// 4. 검색클래스에 등록한 그룹관리 클래스에서 매핑된 그룹번호 값을 가져온다.
// 5. 가져온 그룹번호를 CM에 설정한다.
// 6. 수입년도 값을 검색클래스에 등록한 그룹관리 클래스에서 가져온후에
//	 CM에 설정한다.
//
//###########################################################################
VOID CBO_ACQ_OrderDataMgr::SetOrderDataInfo(INT nSelect)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	CString strMsg;
	INT nRowCnt = m_pPrintDM_Species->GetRowCount();
	INT nVolCnt = m_pPrintDM_Volume->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	INT ids = 0;

	//=======================================================================
	// 1. 책수는 0보다 크고, 종수는 0이라면 책수만큼 종수를 설정한다.
	//		- 수서정보Key, _T("") 로 매핑한다.
	//=======================================================================
	if(nBookCnt > 0 && nRowCnt == 0) 
		nRowCnt = GetSpeciesCnt(m_pDM);


	//=======================================================================
	// 2. 책수는 0보다 크고, 권수는 0이라면 책수만큼 권수를 설정한다.
	//		- 수서정보Key-권일련번호, _T("") 로 매핑한다.
	//=======================================================================
	if(nBookCnt > 0 && nVolCnt == 0)
		nVolCnt = GetVolumeCnt(m_pDM);
	
	//=======================================================================
	// 3. 검색된 자료에 대한 메시지를 작성하여 보낸다.
	//		- 1,2번 내용이 메시지 내용에 들어간다.
	//=======================================================================
	strMsg.Format(_T("%d종 %d권 %d책의 검수 자료가 검색되었습니다."), nRowCnt, nVolCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	

	//=======================================================================
	// 4. 검색클래스에 등록한 그룹관리 클래스에서 매핑된 그룹번호 값을 가져온다.
	//=======================================================================
	ids = m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("그룹번호 값을 가져올 수 없습니다."));
		return;
	}


	//=======================================================================
	// 5. 가져온 그룹번호를 CM에 설정한다.
	//=======================================================================
	ids = m_pCM->SetControlMgrData(_T("그룹번호"), strMsg);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("그룹번호 값을 [CM_주문자료관리]에 적용할 수 없습니다."));
		return;
	}

	//=======================================================================
	// 6. 수입년도 값을 검색클래스에 등록한 그룹관리 클래스에서 가져온후에
	//	 CM에 설정한다.
	//=======================================================================
	ids = m_pCM->SetControlMgrData(_T("수입년도"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("수입년도 값을 [CM_주문자료관리]에 적용할 수 없습니다."));
		return;
	}
}

VOID CBO_ACQ_OrderDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_OrderDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_OrderDataMgr::ShowOrderDataDlg(OPERATION_TYPE nOpType)
{
	INT nSelect, nRowCnt;
	CESL_Grid* pGrid	= NULL;
	CESL_DataMgr* pDM	= NULL;
	PARENT_TYPE nParentType = SPECIES;

	pGrid = GetActiveGrid();
	pDM = GetActiveDM();
	nSelect = m_tabOrderData.GetCurSel();
	nRowCnt = pDM->GetRowCount();
	
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	
	// 2005-01-06 권정보 그리드의 경우 다권본 수정시 화면 깜빡임을 방지 하기 위해 일괄 display하므로 
	// 선정 컬럼 보호 기능을 하면 전집처리할 경우 그리드의 맨 밑에 추가되므로 선정 데이터가 바뀌어 버린다.
	// if(pGrid) pGrid->m_bProtectSelectColumn = TRUE;
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_ORDER);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetOpType(nOpType);
	if( nOpType == SINSERT && nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("종정보 그리드화면에서 입력해 주세요."));
		return ;
	}
	if( nOpType == SUPDATE && nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("종정보 그리드화면에서 수정해 주세요."));
		return ;
	}
	if( nOpType != SINSERT )
	{
		if( 0 == nRowCnt )
		{
			ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
			return ;
		}
	}

	//===================================================
	//2009.04.08 ADD BY PJW : 그리드 수정을 할경우 선정된자료가 있을경우 현재커서를 선정에 추가한다.
	pGrid->SelectMakeList();
//DEL	if(nOpType == GUPDATE)
//DEL	{
		if( pGrid->SelectGetCount() > 0 )
		{
			pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );
			pGrid->SelectMakeList();
		}
//DEL	}	
	//===================================================

	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
	dlg.SetAccuKeyAlias(_T("BP_수서정보KEY"));
	dlg.SetParentType(nParentType);
	dlg.SetParentDM(pDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(pGrid);
	dlg.DoModal();
	SetOrderDataInfo(nSelect);
	if( nOpType == SINSERT )
	{
		INT nCurCnt = pDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			pGrid->DisplayNumber();
			pGrid->SetTopRow(nRowCnt+1);
		}
	}

	// 2005-01-06
	// 선정 컬럼 보호 기능을 해제 한다. 
	// if(pGrid) pGrid->m_bProtectSelectColumn = FALSE;


}

VOID CBO_ACQ_OrderDataMgr::DeleteOrderData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	INT nCurSel = m_tabOrderData.GetCurSel();
	if(nCurSel == 0)
	{
		ids = DeleteOrderSpeciesData();
	}
	else if(nCurSel == 1)
	{
		ids = DeleteOrderVolumeData();
	}
	if(ids == 0)
	{
		SetOrderDataInfo(nCurSel);
		DisplayGrid();	
	}
/*
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제할 수 없습니다."));
		return;
	}
	if(ids == 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제하였습니다."));
		SetOrderDataInfo(nCurSel);
		DisplayGrid();
	}
*/
}

VOID CBO_ACQ_OrderDataMgr::OrderCancel()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}

	COrderDataCancelDlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return;
	
	if(dlg.m_nOrderCancelType == 0)
	{
		OrderCancelByDivno();	
	}
	else if(dlg.m_nOrderCancelType == 1)
	{
		OrderCancelBySelData();
	}
}

VOID CBO_ACQ_OrderDataMgr::CheckInOrderData()
{
	if(m_pPrintDM_Volume->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검수처리할 자료가 없습니다."));
		return;
	}
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("권DM"), (DWORD*)m_pPrintDM_Volume);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("그리드"), (DWORD*)pGrid);
	api.SetTempData(_T("그리드컬럼번호"), m_nEditCol);
	//=====================================================
	//2009.02.24 UPDATE BY PJW : K2UP에는 FALSE로 셋팅되어 있음 -> 왜 TRUE로 되어 잇는지 모르겠음
// 	api.SetTempData(_T("상태변경여부"), TRUE);
 	api.SetTempData(_T("상태변경여부"), FALSE);
	//=====================================================
	
	api.SetTempData(_T("자료상태"), _T("BOA112N"));
	INT ids = api.SPFExecutePath(_T("검수처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("작업하신 검수작업 내용을 저장할 수 없습니다."));
		return;
	}
	if(ids > 0) return ;
	ESLAfxMessageBox(_T("작업하신 검수작업 내용을 저장하였습니다."));
	pGrid->Display();
}

VOID CBO_ACQ_OrderDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();

	BOOL bSpeciesYN;
	if(m_tabOrderData.GetCurSel() == 0) bSpeciesYN = TRUE;
	else bSpeciesYN = FALSE;

	CCodeChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetAcqType(1);
	dlg.SetSpeciesYN(bSpeciesYN);
	if(IDOK == dlg.DoModal())
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_OrderDataMgr::ShowChangePurchaseInfoDlg()
{
	//20070830 ADD PJW : 권정보탭에서 구입정보변경을 하지 못 하도록 수정
	INT nSelect;
	PARENT_TYPE nParentType = SPECIES;
	nSelect = m_tabOrderData.GetCurSel();
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	
	if( nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("종정보 그리드화면에서 구입정보를 변경해 주세요."));
		return ;
	}
	//

	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();
	CPurchaseInfoChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetSpeciesType(2);
	if(IDOK == dlg.DoModal())
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_OrderDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("주문"));
	mgr.DoModal();

	return ;
}

VOID CBO_ACQ_OrderDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_ORDER);
	INT nSelect = m_tabOrderData.GetCurSel();
	CESL_Grid * pGrid = NULL;
	if(nSelect == 0)
	{
		pDlg.SetParentDM(m_pPrintDM_Species);
		pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
		pDlg.SetSpeciesYN(TRUE);
	}
	else
	{
		pDlg.SetParentDM(m_pPrintDM_Volume);
		pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
		pDlg.SetSpeciesYN(FALSE);
		pDlg.SetParentBookDM(m_pDM);
	}
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.SetParentGrid(pGrid);
	pDlg.DoModal();
}

VOID CBO_ACQ_OrderDataMgr::CheckInComplete()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 차수정보가 없습니다."));
		return ;
	}
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return;
	}
	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CBO_ACQ_CheckInDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	CString strGroupNo, strSortDesc;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("그룹정보"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	ids = api.SPFExecutePath(_T("검수완료후등록대상처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 검수완료처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 검수완료처리 하였습니다."));
	//==========================================================
	//20080605 UPDATE BY PJW : 검수처리후 DM을 초기화하지 않는다.->그리드 데이터 사라지는거 방지
	//20080605 REM BY PJW : CBO_ACQ_Business_Api에서 DM을 선정한자료료 Refresh하기때문에 유지하기 곤란함
	//m_pDM->FreeData();
	//m_pPrintDM_Species->FreeData();
	//m_pPrintDM_Volume->FreeData();
	//INT nSelect = m_tabOrderData.GetCurSel();
	//INT nSelect = m_tabOrderData.GetCurSel();
	//DeleteDmRow( nSelect);
	//m_pGrid->Display();
	//==========================================================
	m_pDM->FreeData();
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	INT nSelect = m_tabOrderData.GetCurSel();
	SetTabSelect(nSelect);
}

BOOL CBO_ACQ_OrderDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_OrderDataMgr::OnbtnRESULTSEARCH() 
{
	// TODO: Add your control notification handler code here

	INT nCurSel = m_tabOrderData.GetCurSel();
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	if(nCurSel == 0) 
	{
		pDM = m_pPrintDM_Species;
		pGrid = m_pGrid_Species;
	}
	else if(nCurSel == 1) 
	{
		pDM = m_pPrintDM_Volume;
		pGrid = m_pGrid_Volume;
	}
	
	if(m_pResultSearchDlg != NULL)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}
	m_pResultSearchDlg = new CSearchMgr(this, BO_ACQ_RESEARCH, pDM, pGrid);
	m_pResultSearchDlg->ShowSearchDlg();
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종정보/권정보 그리드 탭 변경
//
// [ 미니스펙 ]
// 1. 툴바버튼을 그리드 탭에 맞게 설정한다.
// 2. 탭번호를 가져와 탭번호에 해당하는 DM과 Grid를 설정한다.
// 3. 정렬항목설정에 대한 파일을 읽어 정렬클래스를 정의하고 멤버변수로 등록한다.
//	- 정렬항목이 설정된 후 정렬클래스를 검색클래스에 넘겨 검색시 정렬되도록 한다.	
// 4. DM을 구성하고 정렬하며, CM을 설정한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_OrderDataMgr::OnSelchangetabORDERDATA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nSelect, ids ;
	CString strGridAlias;


	//=======================================================================
	// 1. 툴바버튼을 그리드 탭에 맞게 설정한다.
	//=======================================================================
	SetToolBarBtnByTabSelect();
	
	//=======================================================================
	// 2. 탭번호를 가져와 탭번호에 해당하는 DM과 Grid를 설정한다.
	//=======================================================================
	nSelect = m_tabOrderData.GetCurSel();
	if(nSelect == 0) 
	{
		m_pActiveDM = m_pPrintDM_Species;
		strGridAlias = _T("MainGrid");
		//========================================================================
		//20080605 ADD BY PJW 그리드 탭이 변경될때마다 DM의 선정값을 해제 한다.
		SetDmSelectToCancel( m_pPrintDM_Volume );
		SetDmSelectToCancel( m_pDM );
		//========================================================================

	}
	else if(nSelect == 1)
	{
		m_pActiveDM = m_pPrintDM_Volume;
		strGridAlias = _T("MainGrid_Volume");
		//========================================================================
		//20080605 ADD BY PJW 그리드 탭이 변경될때마다 DM의 선정값을 해제 한다.
		SetDmSelectToCancel( m_pPrintDM_Species );
		SetDmSelectToCancel( m_pDM );
		//========================================================================			
	}

	//=======================================================================
	// 3. 정렬항목설정에 대한 파일을 읽어 정렬항목설정클래스를 정의하고 멤버변수로 등록한다.
	//	- 정렬항목이 설정된 후 정렬항목설정클래스를 검색클래스에 넘겨 검색시 정렬되도록 한다.	
	//=======================================================================
	InitSortItem(GetActiveGrid(), strGridAlias);


	//=======================================================================
	// 4. DM을 구성하고 정렬하며, CM을 설정한다.
	//=======================================================================
	ids = MakeOrderDM(nSelect);

	if(m_pResultSearchDlg != NULL)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;	
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 탭번호에따라 그리드를 다시 그리고, CM을 설정한다.
//
// [ 미니스펙 ]
// 1. 인자(탭번호)에 따라서 그리드를 선택적으로 보여준다. 
// 2. 그리드를 다시 그린다.
// 3. 주문정보(수입년도, 차수(그룹)번호)를 CM에 설정한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_OrderDataMgr::SetTabSelect(INT nSelect)
{
	//=======================================================================
	// 변수 정의
	//=======================================================================
	INT nRowCnt = -1;


	//=======================================================================
	// 1. 인자(탭번호)에 따라서 그리드를 선택적으로 보여준다. 
	//=======================================================================
	switch( nSelect )
	{
	case 0:
	{
		m_pGrid_Species->ShowWindow(SW_SHOW);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
	}
		break;
	case 1:
	{
		m_pGrid_Species->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_SHOW);
	}
		break;
	default:	 return -1;
		break;
	}

	
	//=======================================================================
	// 2. 그리드를 다시 그린다.
	//=======================================================================
		DisplayGrid();


	//=======================================================================
	// 3. 주문정보(수입년도, 차수(그룹)번호)를 CM에 설정한다.
	//=======================================================================
		SetOrderDataInfo(nSelect);

	return 0;
}

UINT CBO_ACQ_OrderDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	INT nSelect;
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("주문자료를 구축중입니다."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	DeleteDataProcessingDlg();

	nSelect = m_tabOrderData.GetCurSel();

	return MakeOrderDM(nSelect);
}


////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 
//
// [ 미니스펙 ]
////////////////////////////////////////////////////////////////////////////
UINT CBO_ACQ_OrderDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	//=====================================================================
	// 변수정의
	//=====================================================================
	INT ids = -1;
	INT nCurSel ;	
	CString strDivAlias, strExtraAlias ;
	CMapStringToString MapDivValue ;
	CMakePrintDMMgr PrintMgr(this) ;

	//=====================================================================
	// 1. DM을 설정한다.
	//=====================================================================
	CESL_DataMgr* pDM = FindDM(_T("DM_주문자료관리_출력"));
	if( pDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_주문자료관리_출력]을 설정할 수 없습니다.") );
		return 1;
	}


	//=====================================================================
	// 2. 인자값(lParam)에 따라 해당자료목록의 출력을 한다.
	//=====================================================================
	switch( lParam )
	{
	// 검수대상자료목록
	//=====================================================================
	// 2-1. 검수대상자료목록
	//		- i. 현재 그리드가 종정보인지 권정보인지 파악한다.
	//=====================================================================
	case BO_ACQ_CHECKINING_DATA:
		{
			nCurSel = m_tabOrderData.GetCurSel();
			MapDivValue.RemoveAll() ;
			strDivAlias = _T("BP_수서정보KEY");

			if(nCurSel == 0)	// 종정보 그리드
		{
				if(m_pPrintDlg->GetPrintDataType() == 2)	// 그리드 선정자료
			{
					// 선정된 자료의 _T("BP_수서정보KEY")값을 매핑한다.
				ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
				if(ids < 0)
				{
					ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
					return -1;
				}
			}
				// 프로그래스 컨트롤 창을 생성 및 초기화 하여 멤버변수에 설정한다.
			ShowDataProcessingDlg();

			PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
			PrintMgr.SetTempData(_T("프린트타입"), (INT)lParam);
			PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
			PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
			PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_종가격"));
			PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인종가격"));
			PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
			PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());

			PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
			PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
			PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
				PrintMgr.SetTempPtr(_T("원본출력DM"), (DWORD*)m_pPrintDM_Species);
			PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pSpeciesPrintDM);
			PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);

			ids = PrintMgr.SPFExecutePath(_T("일반리스트출력"));

				// 프로그래스 컨트롤 창을 소멸한다.
			DeleteDataProcessingDlg();

			//===================================================
			//2009.10.21 ADD BY LKS : 주석제거/권정보에서 출력후 종정보 이동시 권정보로 인식하여 주석 제거
			//주석 처리 이유 모르겠음.
			m_pPrintDlg->SetIsSpecies(TRUE);
			//===================================================
			
			m_pPrintDlg->SetPrintDM(m_pSpeciesPrintDM);
		}
		else if(nCurSel == 1)
		{	
				strExtraAlias = _T("BB_권일련번호");
				
			if(m_pPrintDlg->GetPrintDataType() == 2)
			{
				ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue, strExtraAlias);
				if(ids < 0)
				{
					ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
					return -1;
				}
			}
			ShowDataProcessingDlg();

			PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
			PrintMgr.SetTempData(_T("프린트타입"), lParam);
			PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
			PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
			PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
			PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
			PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
				PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pPrintDM_Volume);
			PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)pDM);
			PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
			ids = PrintMgr.SPFExecutePath(_T("권리스트출력"));

			DeleteDataProcessingDlg();
			if(ids < 0) 
			{
				ESLAfxMessageBox(_T("출력데이터에 오류가 있습니다."));
				return -1;
			}
			m_pPrintDlg->SetPrintDM(pDM);
			m_pPrintDlg->SetIsSpecies(FALSE);
		}
		
		m_pPrintDlg->Print();
		return 0;
	}
		break;

	case BO_ACQ_CHECKIN_REQUEST_DATA:
	{
		//비치희망신청자료를 갖는 종DM을 만든다.
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_수서정보KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_종가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인종가격"));
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("원본출력DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pSpeciesPrintDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("신청자료도착목록출력"));
		DeleteDataProcessingDlg();
		m_pPrintDlg->SetIsSpecies(TRUE);
		m_pPrintDlg->SetPrintDM(m_pSpeciesPrintDM);	
		m_pPrintDlg->Print();
		return 0;
	}
		break;

	case BO_ACQ_CHECKIN_DATA:
	case BO_ACQ_NONCHECKIN_DATA:
	{
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_수서정보KEY");
		CString strExtraAlias = _T("BB_권일련번호");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue, strExtraAlias);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("원본책DM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("권리스트출력"));
		DeleteDataProcessingDlg();
	}
		break;

	case BO_ACQ_CHECKIN_STATISTICS:
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("주문"));
		dlg.SetSpeciesType(BO_ORDER);
		dlg.DoModal();
	}
		break;

	case BO_ACQ_NONCHECKIN_STATISTICS:
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(-1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("주문"));
		dlg.SetSpeciesType(BO_ORDER);
		dlg.DoModal();
	}
		break;
	
	case BO_ACQ_ORDER_DATA_ORDER:
	{	
		BOOL bExtraInfo = FALSE;
		if(wParam == 1000) bExtraInfo = TRUE;
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_수서정보KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인가"));
		PrintMgr.SetTempData(_T("책DM알리아스"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2형식"), bExtraInfo);
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());		
		PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("그룹컬렉션"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);
		PrintMgr.SetTempData(_T("정렬필드알리아스"), _T(""));
				
		ids = PrintMgr.SPFExecutePath(_T("소계자료출력"));
		DeleteDataProcessingDlg();
		
	}
		break;

	default:
	{
		BOOL bExtraInfo = FALSE;
		if(wParam == 1000) bExtraInfo = TRUE;
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_수서정보KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("프린트타입"), lParam);
		PrintMgr.SetTempData(_T("구분알리아스"), _T("BP_수서정보KEY"));
		PrintMgr.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
		PrintMgr.SetTempData(_T("가격알리아스"), _T("UDF_가격"));
		PrintMgr.SetTempData(_T("할인가알리아스"), _T("UDF_할인가"));
		PrintMgr.SetTempData(_T("책DM알리아스"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2형식"), bExtraInfo);
		PrintMgr.SetTempData(_T("출력자료정렬여부"), m_pPrintDlg->GetDataSortYN());		
		PrintMgr.SetTempData(_T("출력데이터타입"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("그리드선정자료"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("프로그래스컨트롤"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("그룹컬렉션"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("정렬조건"), (DWORD*)m_pSortItemSet);

		switch( lParam )
		{
		case BO_ACQ_ORDER_DATA_DIVNO:		PrintMgr.SetTempData(_T("정렬필드알리아스"), _T(""));
			break;
		case BO_ACQ_ORDER_DATA_PROVIDER:	PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BP_구입처부호"));
			break;
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:	PrintMgr.SetTempData(_T("정렬필드알리아스"), _T("BP_주제구분"));
			break;
		}
		
		ids = PrintMgr.SPFExecutePath(_T("소계자료출력"));
		DeleteDataProcessingDlg();
	}
		break;
	}

	if(ids < 0) return 0;
	m_pPrintDlg->SetPrintDM(pDM);
	m_pPrintDlg->Print();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM을 구성하고 정렬하며, CM을 설정한다.
//
// [ 미니스펙 ]
// 1. DM에 검색된 내용이 없다면 리턴한다.
// 2. Progress Ctrl Dialog 창 생성 및 초기화 
// 3. Business API를 초기화 한다.
// 4. 탭번호에 해당하는 DM 및 실행할 함수를 Business Api에 설정한다.
// 5. Progress Ctrl 창을 소멸한다.
// 6. 정렬클래스를 이용하여 현재 DM을 정렬한다.
// 7. 탭번호에 따라 그리드를 다시 그리고, CM을 설정한다.
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_OrderDataMgr::MakeOrderDM(INT nSelect)
{
	//============================================================================
	//2009.03.17 UPDATE BY LKS : 수정 후 탭 이동시 수정된 내용 반영안되는 오류 수정
	CString strQuery;
	m_pSearchDlg->GetWhereStmt(strQuery);
	if(!strQuery.IsEmpty())
	{
		m_pDM->RefreshDataManager(strQuery);
		m_pPrintDM_Species->FreeData();
		m_pPrintDM_Volume->FreeData();
	}
	//============================================================================

	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids = -1;
	CBO_ACQ_Business_Api mgr(this);


	//=======================================================================
	// 1. DM에 검색된 내용이 없다면 리턴한다.
	//=======================================================================
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		SetTabSelect(nSelect);
		return 0;
	}

	
	//=======================================================================
	// 2. Progress Ctrl Dialog 창 생성 및 초기화 
	//=======================================================================
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("주문자료를 구축중입니다."));
	

	//=======================================================================
	// 3. Business API를 초기화 한다.
	//=======================================================================
	mgr.InitTempData();
	
	//=======================================================================
	// 4. 탭번호에 해당하는 DM 및 실행할 함수를 Business Api에 설정한다.
	//=======================================================================
	switch( nSelect )
	{
	case 0:
	{
		m_pPrintDM_Species->FreeData();
		mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("종DM"), (DWORD*)m_pPrintDM_Species);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		ids = mgr.SPFExecutePath(_T("책To수서정보"));
	}
		break;

	case 1:
	{
		m_pPrintDM_Volume->FreeData();
		mgr.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("권DM"), (DWORD*)m_pPrintDM_Volume);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		ids = mgr.SPFExecutePath(_T("책To권"));
	}
		break;

	default:
		{
			ESLAfxMessageBox( _T("현재 탭에 해당하는 작업을 할 수 없습니다.") );
			return -1;
		}
		break;
	}


	//=======================================================================
	// 5. Progress Ctrl 창을 소멸한다.
	//=======================================================================
	DeleteDataProcessingDlg();

	if(ids < 0) return ids;
	


	//=======================================================================
	// 6. 정렬클래스를 이용하여 현재 DM을 정렬한다.
	//=======================================================================
	ids = m_pSortItemSet->DataSort(GetActiveDM(), !m_pSearchDlg->GetDataSortYN());
	if( ids < 0 ) return ids;
	//SortByGroupInfo();



	//=======================================================================
	// 7. 탭번호에 따라 그리드를 다시 그리고, CM을 설정한다.
	//=======================================================================
	ids = SetTabSelect(nSelect);

	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	INT nRowCount = m_pActiveDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================

	if( ids < 0 ) return -1;

	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_OrderDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_OrderDataMgr)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_VOLUME_DATA, -601 /* DblClick */, OnDblClickgridORDERVOLUMEDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_DATA, -601 /* DblClick */, OnDblClickgridORDERDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_VOLUME_DATA, -600 /* Click */, OnClickgridORDERVOLUMEDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_VOLUME_DATA, -607 /* MouseUp */, OnMouseUpgridORDERVOLUMEDATA, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_DATA, -607 /* MouseUp */, OnMouseUpgridORDERDATA, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_OrderDataMgr::OnDblClickgridORDERVOLUMEDATA() 
{
	// TODO: Add your control notification handler code here
	//===================================================
	
	//===================================================
	//2009.05.20 ADD BY LKS : 납품책수 컬럼을 더블클릭했을 경우는 따로 처리
	CESL_Grid * pGrid = GetActiveGrid();
	int nCol;
	nCol = pGrid->GetMouseCol();
	CString sColumnName;
	sColumnName = pGrid->GetTextMatrix(0,nCol);
	if(sColumnName == _T("납품책수"))
		return;
	//===================================================
	
	//2009.04.07 ADD BY PJW : 그리드 더블클릭했을경우 권정보 TAB이면 메세지 보여준다.
	INT nSelect;
	INT nParentType;
	nSelect = m_tabOrderData.GetCurSel();
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	if( nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("종정보 그리드화면에서 수정해 주세요."));
		return ;
	}
	//===================================================
	

	if(m_pGrid_Volume->GetMouseRow() == 0 ) return ;
	if(m_pGrid_Volume->GetCol() == m_nEditCol+1) return ;
	CESL_DataMgr * pDM = GetActiveDM();
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	//CESL_Grid * pGrid = GetActiveGrid();
	if(pGrid->GetMouseRow() == 0) return ;

	ShowOrderDataDlg(GUPDATE);
}

VOID CBO_ACQ_OrderDataMgr::OnDblClickgridORDERDATA() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr * pDM = GetActiveDM();
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	CESL_Grid * pGrid = GetActiveGrid();
	if(pGrid->GetMouseRow() == 0) return ;
	
	ShowOrderDataDlg(GUPDATE);
}

VOID CBO_ACQ_OrderDataMgr::OnClickgridORDERVOLUMEDATA() 
{
	// TODO: Add your control notification handler code here
}

BOOL CBO_ACQ_OrderDataMgr::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) 
	{
		if(pMsg->wParam == VK_F11) 
		{
			return FALSE;
		}
	}
	
	if (ESL_GRID_MODIFY == pMsg->message)
	{
		// 납풉책수 컬럼이 수정되었다면.
		if (pMsg->lParam== m_nEditCol) 
		{	
			if (0>ReCalcBookCount(pMsg->wParam + 1))	///수정 행번호
			{
				ESLAfxMessageBox(_T("미납책수를 설정하는데 실패하였습니다."));
			}	
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

BOOL CBO_ACQ_OrderDataMgr::CheckIsDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}


INT CBO_ACQ_OrderDataMgr::ReCalcBookCount(INT nRow/*def:-1*/)
{
	CESL_Grid *pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid_Volume"));

	if (!pGrid) return -2;
	
	INT nCurRow = -1;
	if (-1==nRow)	nCurRow = pGrid->GetRow();
	else			nCurRow = nRow;
	///입력된 책 수 검증
	CString sTemp, strDeliveredCount;
	BOOL bInvalidCount = FALSE;
	INT nOrderCount;
	
	nOrderCount	= _ttoi((pGrid->GetTextMatrix(nCurRow, m_nEditCol)).GetBuffer(0));
	strDeliveredCount = pGrid->GetTextMatrix(nCurRow, m_nEditCol+1);
	
	if(!CheckIsDigit(strDeliveredCount))
	{
		sTemp.Format(_T("납품책수에 숫자 아닌값( %s )이 입력되었습니다."), strDeliveredCount);
		bInvalidCount = TRUE;
	}
	INT nDeliveredCount = _ttoi((strDeliveredCount).GetBuffer(0));
	INT nNondeliveredCount = nOrderCount-nDeliveredCount;

	///납품책수가 주문책수보다 크다면
	if (nOrderCount<nDeliveredCount) 
	{
		sTemp.Format(_T("납품책수(%d)가 주문책수(%d)보다 큽니다."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	///납품책수가 0보다 작다면
	else if (0>nDeliveredCount)
	{
		sTemp.Format(_T("납품책수(%d)가 0 보다 작습니다."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	
	///잘못된 책수 입력됨
	if (bInvalidCount) 
	{
		ESLAfxMessageBox(sTemp);
		///원래 납품책수/주문책수 값으로 복원
		pGrid->SetTextMatrix(nCurRow, m_nEditCol+1, GetActiveDM()->GetCellData(_T("UDF_납품책수"), nCurRow-1));
		nDeliveredCount = _ttoi(GetActiveDM()->GetCellData(_T("UDF_납품책수"), nCurRow-1).GetBuffer(0));
		sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
		pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);
		return 1;
	}
	
	sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
	pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);

	return 0;
}

VOID CBO_ACQ_OrderDataMgr::SelDataDeliveryProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("GET_COLUMN"), _T("발주책수"));
	api.SetTempData(_T("SET_COLUMN"), _T("납품책수"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("미납책수"));
	ids = api.SPFExecutePath(_T("납품처리"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료를 납품처리할 수 없습니다."));
		return ;
	}
}

VOID CBO_ACQ_OrderDataMgr::SelDataDeliveryCancelProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("GET_COLUMN"), _T("발주책수"));
	api.SetTempData(_T("SET_COLUMN"), _T("미납책수"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("납품책수"));
	ids = api.SPFExecutePath(_T("납품취소"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료를 납품취소할 수 없습니다."));
		return ;
	}
}

VOID CBO_ACQ_OrderDataMgr::RegTransfer()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 차수정보가 없습니다."));
		return ;
	}

	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return ;
	}

	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CString strSpeciesKey;
	CString strBookSpeciesKey;
	CString strVolSortNo;
	CString strBookVolStroNo;
	INT nIdx, y;
	for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
	{	
		m_pDM->SetCellData(_T("선정"), _T(""), y );	
	}			

	CESL_DataMgr*	m_pPrintDM;		
	CESL_Grid*	pGrid;	
	if ( m_tabOrderData.GetCurSel() == 0 ) 
	{
		m_pPrintDM = m_pPrintDM_Species;
		pGrid = m_pGrid_Species;		
	}
	else 
	{
		m_pPrintDM = m_pPrintDM_Volume;
		pGrid = m_pGrid_Volume;
	}

	for (nIdx = 0 ; nIdx < m_pPrintDM->GetRowCount() ; nIdx++ )
	{
		if ( pGrid->GetTextMatrix(nIdx+1,1) == _T("V") )
		{			
			m_pPrintDM->SetCellData( _T("선정"), _T("V"), nIdx  );
			// 2008.03.12
			// 종정보의 선정 상태를 책정보에 반영한다.
			// DM_구입정보관리_종 -> DM_구입정보관리
			if ( m_tabOrderData.GetCurSel() == 0 ) 
			{
				strSpeciesKey = m_pPrintDM->GetCellData(_T("IBS_종KEY"), nIdx );
				for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
				{
					strBookSpeciesKey = m_pDM->GetCellData(_T("IBS_종KEY"), y );
					if ( strSpeciesKey == strBookSpeciesKey )
					{
						m_pDM->SetCellData(_T("선정"), _T("V"), y );
					}
				}
			}
			else
			{
				// 권단위 일경우 편권차 까지 판단한다.
				strSpeciesKey = m_pPrintDM->GetCellData(_T("IBS_종KEY"), nIdx );
				strVolSortNo = m_pPrintDM->GetCellData(_T("BB_권일련번호"), nIdx );
				for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
				{
					strBookSpeciesKey = m_pDM->GetCellData(_T("IBS_종KEY"), y );
					strBookVolStroNo = m_pDM->GetCellData(_T("BB_권일련번호"), y );
					if ( strSpeciesKey == strBookSpeciesKey && strVolSortNo == strBookVolStroNo )
					{
						m_pDM->SetCellData(_T("선정"), _T("V"), y );
					}
				}
			}
		}
		else
		{
			m_pPrintDM->SetCellData( _T("선정"), _T(""), nIdx  );
		}
	}

	CBO_ACQ_CheckInDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	CString strGroupNo, strSortDesc;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("그룹정보"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	ids = api.SPFExecutePath(_T("검수완료후등록인계"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 등록인계처리 할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("선정한 자료를 등록인계처리 하였습니다."));
	m_pDM->FreeData();
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	INT nSelect = m_tabOrderData.GetCurSel();
	SetTabSelect(nSelect);
}

INT CBO_ACQ_OrderDataMgr::CheckSelectDataValid()
{
	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();
	
	if(pDM->GetRowCount() == 0) return -1;

	if(!pGrid) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

CESL_DataMgr * CBO_ACQ_OrderDataMgr::GetActiveDM()
{
	INT nCurSel = m_tabOrderData.GetCurSel();
	if(nCurSel == 0) return m_pPrintDM_Species;
	return m_pPrintDM_Volume;
}

CESL_Grid * CBO_ACQ_OrderDataMgr::GetActiveGrid()
{
	INT nCurSel = m_tabOrderData.GetCurSel();
	if(nCurSel == 0) return m_pGrid_Species;
	return m_pGrid_Volume;
}


//##########################################################################
// [ 함수설명 ]
// - 종/권정보 그리드의 팝업메뉴를 설정하고, 
//   권정보 그리드에서 _T("납품책수")를 그리드상에서 수정 가능하게 한다.
//
// [ 미니스펙 ]
// 1. 종화면 그리드에 추가할 팝업메뉴를 입력한다.
// 2. 종화면 그리드에 팝업메뉴를 추가한다.
// 3. 권화면 그리드에 추가할 팝업메뉴를 입력한다.
// 4. 권화면 그리드에 팝업메뉴를 추가한다.
// 5. 권화면 그리드에 수정할 수 있는 캄럼을 _T("납품책수") 하나만으로 수정한다.
//
//##########################################################################
VOID CBO_ACQ_OrderDataMgr::InitPopupMenuItem()
{
	INT ids = 0;
	CBO_ACQ_Business_Api api(this);
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	
	//=======================================================================	
	// 1. 종화면 그리드에 추가할 팝업메뉴를 입력한다.
	//		- 납품처리, 미납처리, 변경내역저장
	//=======================================================================	
	RemoveListArray.Add(_T("납품처리"));
	RemoveListArray.Add(_T("미납처리"));
	RemoveListArray.Add(_T("변경내역저장"));

	//=======================================================================	
	// 2. 종화면 그리드에 팝업메뉴를 추가한다.
	//=======================================================================	
	ids = m_pGrid_Species->InitPopupMenuItem();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("종-그리드화면의 팝업메뉴를 초기화 할 수 없습니다."));
		return;
	}
	ids = m_pGrid_Species->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("종-그리드화면의 팝업메뉴를 등록할 수 없습니다."));
		return ;
	}

	//=======================================================================	
	// 3. 권화면 그리드에 추가할 팝업메뉴를 입력한다.
	//		- 기존의 리스트를 모두 지운후에
	//		- 주문취소를 추가한다.
	//=======================================================================	
	RemoveListArray.RemoveAll();
	RemoveListArray.Add(_T("주문취소"));

	//=======================================================================	
	// 4. 권화면 그리드에 팝업메뉴를 추가한다.
	//=======================================================================	
	m_pGrid_Volume->InitPopupMenuItem();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("권-그리드화면의 팝업메뉴를 초기화 할 수 없습니다."));
		return;
	}

	m_pGrid_Volume->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("권-그리드화면의 팝업메뉴를 등록할 수 없습니다."));
		return ;
	}

	//=======================================================================	
	// 5. 권화면 그리드에 수정할 수 있는 캄럼을 _T("납품책수") 하나만으로 수정한다.
	//=======================================================================	
	m_pGrid_Volume->m_bGridEdit = TRUE;
	m_pGrid_Volume->SELEDITOR_RemoveAllColumns();
	m_nEditCol = api.GetGridColumnIdx(m_pGrid_Volume, _T("납품책수"));
	m_pGrid_Volume->SELEDITOR_AddColumn(m_nEditCol);
}
//###########################################################################
// [ 함수설명 ]
// - 툴바버튼을 설정한다.
//
// [ 미니스펙 ]
// 1. 현재 그리드가 종인지 권인지 알기 위해 탭번호를 얻어온다.
// 2. 종정보일때의 설정할 버튼이름을 입력한다.
// 3. 권정보일때의 설정할 버튼이름을 입력한다.
// 4. 종/권정보 어느 것인지 판단하여 툴바버튼을 설정하라는 메시지를 보낸다.
//###########################################################################
VOID CBO_ACQ_OrderDataMgr::SetToolBarBtnByTabSelect()
{
	INT i, nCurSel;
	CStringArray arrSpeciseBtnAlias, arrVolumeBtnAlias;

	//=======================================================================	
	// 1. 현재 그리드가 종인지 권인지 알기 위해 탭번호를 얻어온다.
	//=======================================================================	
	nCurSel = m_tabOrderData.GetCurSel();
	

	//=======================================================================	
	// 2. 종정보일때의 설정할 버튼이름을 입력한다.
	//=======================================================================	
	arrSpeciseBtnAlias.Add(_T("납품처리"));		arrSpeciseBtnAlias.Add(_T("납품취소"));
	arrSpeciseBtnAlias.Add(_T("검수처리"));

	//=======================================================================	
	// 3. 권정보일때의 설정할 버튼이름을 입력한다.
	//=======================================================================	
	arrVolumeBtnAlias.Add(_T("주문취소"));
	

	//=======================================================================	
	// 4. 종/권정보 어느 것인지 판단하여 툴바버튼을 설정하라는 메시지를 보낸다.
	//=======================================================================	
	// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
	//                   권정보 탭에서 납품처리/납품취소/검수처리 툴바가 보이지 않는 오류 수정
/*//BEFORE-----------------------------------------------------------------------
	if(nCurSel == 0)
	{
		for(i=0 ; i<arrSpeciseBtnAlias.GetSize() ; i++)
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrSpeciseBtnAlias[i], TRUE);
		for(i=0 ; i<arrVolumeBtnAlias.GetSize() ; i++)
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrVolumeBtnAlias[i], FALSE);
	}
	else if(nCurSel == 1)
	{
		for(i=0 ; i<arrVolumeBtnAlias.GetSize() ; i++)
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrVolumeBtnAlias[i], TRUE);
	}
*///AFTER------------------------------------------------------------------------
	// 종정보 탭인 경우에만 표시한다
	for (i=0; i<arrSpeciseBtnAlias.GetSize(); i++)
	{
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrSpeciseBtnAlias[i], (nCurSel==0));
	}
	// 권정보 탭인 경우에만 표시한다
	for (i=0; i<arrVolumeBtnAlias.GetSize(); i++)
	{
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrVolumeBtnAlias[i], (nCurSel==1));
	}
/*//END------------------------------------------------------------------------*/
}

INT CBO_ACQ_OrderDataMgr::OrderCancelByDivno()
{
	CGroupCollection* pGroupCollection = NULL;
	COrderCancelDivnoSelectDlg dlg(this);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.DoModal();
	pGroupCollection = dlg.GetOrderCancelGroupCollection();
	if( pGroupCollection->GetGroupCount() == 0 )	return 0;
	
	//JOB.2019.0073 : 충돌 License 
	//주문취소 메일 보내야쥐
	//ShowOrderCancelMailMgr(CANCEL, AUTO, pGroupCollection);

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)m_pPrintDM_Species);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)m_pPrintDM_Volume);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)pGroupCollection);
	INT ids = api.SPFExecutePath(_T("그룹자료삭제"));
	
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("주문취소자료를 삭제할 수 없습니다."));
		return -1;
	}
	INT nCurSel = m_tabOrderData.GetCurSel();
	DeleteDmRow( nCurSel );

	

	
	SetTabSelect(nCurSel);
	return 0;
}

INT CBO_ACQ_OrderDataMgr::OrderCancelBySelData()
{
	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}

	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetWorkStatus(_T("구입대상"));
	dlg.SetSourceDM(GetActiveDM());
	dlg.SetSourceGrid(GetActiveGrid());
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return 0;

	CGroupInfo * pGroup = dlg.GetSelGroup();
	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();
	CStringArray arrAcqKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)pGroup);
	api.SetTempPtr(_T("ACQKEYLIST"), (DWORD*)&arrAcqKey);
	api.SetTempData(_T("BOOK_CNT_ALIAS"), _T("BP_발주책수"));
	
	ids = api.SPFExecutePath(_T("선정자료주문취소"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료를 주문취소 할 수 없습니다."));
		return -1;
	}

	//주문취소 메일 보내야쥐
	CString strBookKeyList = GetBookKeyList(&arrAcqKey);
	if(!strBookKeyList.IsEmpty())
	{
		//JOB.2019.0073 : 충돌 License
// 		COrderMailMgr mailMgr(this);
// 		mailMgr.SetRecKeyList(strBookKeyList);
// 		mailMgr.SetOperationType(CANCEL);
// 		mailMgr.SetOpenType(AUTO);
// 		mailMgr.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
// 		mailMgr.DoModal();
	}
	
	INT nCurSel = m_tabOrderData.GetCurSel();
	SetTabSelect(nCurSel);
	return 0;
}

CString CBO_ACQ_OrderDataMgr::GetBookKeyList(CStringArray *pArrAcqKey)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString strTmpData, strQuery;
	INT cnt = pArrAcqKey->GetSize();
	if(cnt == 0) return _T("");

	//=====================================================
	//2009.02.23 UPDATE BY PJW : 1000개 이상일경우 나누어서 쿼리를 날린다.
// 	for(INT i=0 ; i<cnt ; i++)
// 	{
// 		if(strTmpData.IsEmpty()) strTmpData = pArrAcqKey->GetAt(i);
// 		else strTmpData += _T(", ") + pArrAcqKey->GetAt(i);
// 	}
	CString strInQuery;

	for(INT i=0 ; i<cnt ; i++)
	{
		if(strTmpData.IsEmpty()) 
		{
			strTmpData = pArrAcqKey->GetAt(i);
		}
		else 
		{
			if( i%1000 == 999 )
			{				
				if( strInQuery.IsEmpty() )
				{
					strInQuery.Format(_T("ACQ_KEY IN (%s)"), strTmpData);
				}
				else
				{
					strInQuery = strInQuery + _T("OR ACQ_KEY IN ( "), strTmpData + _T(" )");
				}
				strTmpData = pArrAcqKey->GetAt(i);
			}
			else
			{
				strTmpData += _T(", ") + pArrAcqKey->GetAt(i);
			}
		}
	}

	if( !strTmpData.IsEmpty() )
	{
		if( strInQuery.IsEmpty() )
		{
			strInQuery.Format(_T("ACQ_KEY IN (%s)"), strTmpData);
		}
		else
		{
			strInQuery = strInQuery + _T("OR ACQ_KEY IN ( ") + strTmpData + _T(" )");
		}
	}
	
	strTmpData.Format(_T("SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE %s"), strInQuery);
	//=====================================================

	strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE ACQ_KEY IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE"), strTmpData);
	tmpDM.RestructDataManager(strQuery);
	cnt = tmpDM.GetRowCount();
	if(cnt == 0) return _T("");
	strTmpData.Empty();
	for(i=0 ; i<cnt ; i++)
	{
		if(strTmpData.IsEmpty()) strTmpData = tmpDM.GetCellData(i, 0);
		else strTmpData += _T(", ") + tmpDM.GetCellData(i, 0);
	}
	return strTmpData;
}

VOID CBO_ACQ_OrderDataMgr::ViewCheckInCompleteData()
{
	CViewTransferDataDlg dlg(this);
	dlg.DoModal();
}

VOID CBO_ACQ_OrderDataMgr::ShowOrderCancelMailMgr(INT nOpType, INT nOpenType, CGroupCollection * pGroupCollection)
{
	//JOB.2019.0073 : 충돌 License
// 	COrderMailMgr mailMgr(this);
// 	if(pGroupCollection == NULL) pGroupCollection = m_pSearchDlg->GetGroupCollection();
// 	
// 	mailMgr.SetParentGroupCollection(pGroupCollection);
// 	mailMgr.SetOperationType(nOpType);
// 	mailMgr.SetOpenType(nOpenType);
// 	mailMgr.SetAcqYear(pGroupCollection->GetAcqYear());
// 	mailMgr.DoModal();
}

INT CBO_ACQ_OrderDataMgr::MakePrintDM(CString strSortField)
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("전체자료"))
	{
		ESLAfxMessageBox(_T("현재 선정된 차수가 없습니다. 검색을 통하여 특정 차수를 선정해 주십시요."));
		return -1;
	}

	CESL_DataMgr * pDM = FindDM(_T("DM_주문자료관리_출력"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("출력용DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	api.SetTempData(_T("SORT_FIELD"), strSortField.GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("주문목록출력용DM생성"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("출력용 데이터를 구축할 수 없습니다."));
		return -1;
	}
	return 0;
}

INT CBO_ACQ_OrderDataMgr::DeleteOrderSpeciesData()
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)GetActiveDM());
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_ORDER);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("삭제타입"), SPECIES_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	api.SetTempData(_T("가격알리아스"), _T("UDF_변환할인종가격"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	INT ids = api.SPFExecutePath(_T("삭제"));

	return ids;
}

INT CBO_ACQ_OrderDataMgr::DeleteOrderVolumeData()
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)GetActiveDM());
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_ORDER);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("삭제타입"), (INT)VOLUME_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("UDF_책수"));
	api.SetTempData(_T("가격알리아스"), _T("UDF_변환할인가격"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	INT ids = api.SPFExecutePath(_T("삭제"));

	return ids;
}

///////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - Progress Ctrl Dialog 창 생성 및 초기화
///////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_OrderDataMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if( m_pDataProcessingDlg->GetCreate() == FALSE )
	{
		if( m_pDataProcessingDlg->Create(this) ==FALSE )
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_OrderDataMgr::DeleteDataProcessingDlg()
{
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

//###########################################################################
// [ 함수설명 ]
// - SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s'
// VAlUE_NAME 값을 얻어와서 리턴해준다.
//
// [ 미니스펙 ]
// 1. 쿼리문 작성
// 2. 쿼리 결과를 받아와 리턴해준다.
//###########################################################################
CString CBO_ACQ_OrderDataMgr::GetCommonManageValue(CString strGroup2, CString strGroup3, CString strClassAlias)
{
	CString strValue, strQuery, strErrMsg= _T("");
	INT		ids = 0;
	
	//=======================================================================
	// 1. 쿼리문 작성
	//		- ESL_MANAGE_TBL에서 VALUE_NAME을 얻어온다.
	//=======================================================================
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s'  AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		strGroup2, strGroup3, strClassAlias);
	
	//=======================================================================
	// 2. 쿼리 결과를 받아와 리턴해준다.
	//=======================================================================
	ids = m_pDM->GetOneValueQuery(strQuery, strValue);
	if( ids < 0 )
	{
		switch (ids)
		{
		case -4004:	strErrMsg.Format(_T("[ %d ] : 데이터베이스와 접속중이지 않습니다.\n%s"), ids, strQuery );
			break;
		case -4005:	strErrMsg.Format(_T("[ %d ] : 질의문(Query)에 대한 실행시 문제가 있습니다.\n%s"), ids, strQuery );
			break;
		case -4006:	strErrMsg.Format(_T("[ %d ] : 질의문(Query)에 대한 결과가 너무 많습니다.\n%s"), ids, strQuery );
			break;
		case -4007:	strErrMsg.Format(_T("[ %d ] : 질의문(Query)에 대한 결과가 없습니다.\n%s"), ids, strQuery );
			break;
		case -4008:	strErrMsg.Format(_T("[ %d ] : 검색필드가 결과필드보다 많습니다.\n%s"), ids, strQuery );
			break;
		case -4999:	strErrMsg.Format(_T("[ %d ] : Oracle을 지원중인지 확인하십시오.\n%s"), ids, strQuery );
			break;
		default:	strErrMsg.Format(_T("[ %d ] : Unknown Error"), ids);
			break;
		}
		ESLAfxMessageBox(strErrMsg);
		return _T("");
	}

	return strValue;
}

INT CBO_ACQ_OrderDataMgr::GetSpeciesCnt(CESL_DataMgr *pDM)
{
	CMapStringToString tmpMap;
	tmpMap.RemoveAll();

	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), i);
		tmpMap.SetAt(strTmpData, _T(""));
	}

	return tmpMap.GetCount();
}

INT CBO_ACQ_OrderDataMgr::GetVolumeCnt(CESL_DataMgr *pDM)
{
	CMapStringToString tmpMap;
	tmpMap.RemoveAll();
	
	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BP_수서정보KEY"), i);
		strTmpData += _T("-") + pDM->GetCellData(_T("BB_권일련번호"), i);
		tmpMap.SetAt(strTmpData, _T(""));
	}
	
	return tmpMap.GetCount();
}

VOID CBO_ACQ_OrderDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	CDataSortDlg dlg(this);

	if( m_pSortItemSet == NULL || GetActiveDM() == NULL ) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if( GetActiveDM()->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	dlg.SetMainDM(GetActiveDM());
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)		DisplayGrid();
}

UINT CBO_ACQ_OrderDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

//###########################################################################
// [ 함수설명 ]
// - 정렬항목설정에 대한 파일을 읽어 정렬클래스를 정의하고 멤버변수로 등록한다.
// - 정렬항목이 설정된 후 정렬클래스를 검색클래스에 넘겨 검색시 정렬되도록 한다.
//
// [ 미니스펙 ]
// 1. CSortItemSet 클래스(정렬항목설정클래스)를 생성하여 멤버변수로 등록한다.
// 2. 생성된 정렬클래스에 정렬하고자 하는 해당(인자) 그리드를 정의해준다.
// 3. 정렬클래스에 DM을 파일로 부터 초기화 한다.
// 4. 설정된 정렬항목이 저장된 파일을 정렬 클래스에 정의한다.
// 5. 정렬클래스의 m_HeadSortList에 수입년도, 업무구분, 그룹번호를 추가한다.
// 6. 정렬클래스의 m_TailSortList에 발주인련번호, 권일련번호를 추가한다.
// 7. UDF와 Alias를 매핑시킨다.
// 8. 4번에서 정의한 파일을 읽어들여 정렬항목설정클래스 DM에 추가한다.
// 9. 정의된 정렬항목설정클래스를 검색창클래스에 정렬항목으로 정의한다.
//###########################################################################
BOOL CBO_ACQ_OrderDataMgr::InitSortItem(CESL_Grid * pGrid, CString strGridAlias)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT		ids = 0;
	CStringArray* pTmpData = NULL;	
	
	
	//=======================================================================
	// 1. CSortItemSet 클래스(정렬항목설정클래스)를 생성하여 멤버변수로 등록한다.
	//=======================================================================
	if( m_pSortItemSet )	// CSortItemSet
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
	m_pSortItemSet = new CSortItemSet(this);

	//=======================================================================
	// 2. 생성된 정렬클래스에 정렬하고자 하는 해당(인자) 그리드를 정의해준다.
	//=======================================================================
	m_pSortItemSet->SetGrid(pGrid);

	//=======================================================================
	// 3. 정렬항목설정클래스에 DM을 파일로 부터 초기화 한다.
	//		- DM파일은 두번째 인자인 DM_정렬항목설정.DM 파일이다.
	//		- 해당함수는 CESL_DataMgr::InitDMPKFromFile()을 호출한다.
	//=======================================================================
	ids = m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("[DM_정렬항목설정]을 초기화 할 수 없습니다."));
		return FALSE;
	}

	//=======================================================================
	// 4. 설정된 정렬항목이 저장된 파일을 정렬 클래스에 정의한다.
	//		- Cfg/SortConfig/SMAlias_GridAlias.cfg 파일에 정의되어 있다.
	//=======================================================================
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(),strGridAlias);


	//=======================================================================
	// 5. 정렬항목설정클래스의 m_HeadSortList에 수입년도, 업무구분, 그룹번호를 추가한다.
	//=======================================================================
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("IBS_업무구분"));
	pTmpData->Add(_T("BP_그룹번호"));

	//=======================================================================
	// 6. 정렬항목설정클래스의 m_TailSortList에 발주인련번호, 권일련번호를 추가한다.
	//=======================================================================
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_발주일련번호"));
	// BB_권일련번호는 권정보에만 해당되는 사항이다.
	// 2005-10-12 강봉수
	//
	if( strGridAlias == _T("MainGrid_Volume") )	pTmpData->Add(_T("BB_권일련번호"));


	//=======================================================================
	// 7. UDF와 Alias를 매핑시킨다.
	//		- 주제구분설명, 입력구분설명, 구입처부호설명, 종가격, 할인종가격
	//=======================================================================
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	
	//2009.05.14 UPDATE BY LKS : 설명값이 아닌 코드값으로 정렬이 되어 수정 및 주석처리
	//2010.04.26 UPDATE BY PJW : 주제구분 설명이 아닌 코드값으로 정렬되는것이 맞음 , 김지연 대리 요구사항
//	pMap->SetAt(_T("BP_주제구분"), _T("UDF_주제구분설명"));
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BP_주제구분"));
	//===================================================
	//pMap->SetAt(_T("UDF_구입처부호설명"), _T("BP_구입처부호"));
	pMap->SetAt(_T("UDF_입력구분설명"), _T("BP_입력구분"));
	// 2005-04-06
	pMap->SetAt(_T("UDF_종가격"), _T("BP_가격"));
	pMap->SetAt(_T("UDF_할인종가격"), _T("BP_발주가격"));
	

	//=======================================================================
	// 8. 4번에서 정의한 파일을 읽어들여 정렬항목클래스 DM에 추가한다.
	//		- 정렬항목이 설정되어 있지 않다고 해도 에러는 아니므로 그냥 패스한다.
	//=======================================================================
	ids = m_pSortItemSet->ReadSortItem();	
	if( ids < 0 )
	{
	//	ESLAfxMessageBox(_T("[DM_정렬항목설정]을 설정할 수가 없습니다."));
	//	return FALSE;
	}

	//=======================================================================
	// 9. 정의된 정렬 클래스를 검색창클래스에 정렬항목으로 정의한다.
	//=======================================================================
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);

	return TRUE;
}

VOID CBO_ACQ_OrderDataMgr::FileImport()
{
	CBO_ACQ_FileImportMgr  ImportMgr(this);

	ImportMgr.SetSpeciesType(BO_ORDER);		//종타입
	// 파일을 반입한다.
	INT ids = ImportMgr.FileImportProc();
	if(ids < 0) return ;

//	ids =m_pDM->RefreshDataManager(ImportMgr.m_strWhereStmt);

	if(ids == 0)
		AfxMessageBox(_T("파일 반입을 완료하였습니다."));	
	else return ;

}

VOID CBO_ACQ_OrderDataMgr::FileExport()
{
	INT ids;
	CString strAcqYear, strGroupInfo, strWorkCode, strQuery, strResult;
	CDataExportDlg Dlg(this);
	ids = Dlg.DoModal();
	if( ids == IDCANCEL ) return ;

	strAcqYear = Dlg.GetAcqYear();
	strGroupInfo = Dlg.GetSeqNo();
	strWorkCode = Dlg.GetWorkCode();

	if(strAcqYear.IsEmpty() || strGroupInfo.IsEmpty() || strWorkCode.IsEmpty())
	{
		AfxMessageBox(_T("반출 조건을 모두 입력하셔야 합니다."));
		return ;
	}
	
	strQuery.Format(_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),
								strAcqYear, strGroupInfo, strWorkCode);
	strResult.Empty();
	ids = m_pDM->GetOneValueQuery(strQuery, strResult);

	if( ids <0 || strResult.IsEmpty()) 
	{
		AfxMessageBox(_T("입력조건에 맞는 차수가 존재하지 않습니다."));
		return ;
	}

	CBO_ACQ_FileExportMgr ExportMgr;
	
	ExportMgr.SetAcqYear(strAcqYear);
	ExportMgr.SetGroupInfo(strGroupInfo);
	ExportMgr.SetWorkCode(strWorkCode);
	ExportMgr.SetSpeciesType(BO_ORDER);		//종타입
	
	// 파일을 반출한다.
	ids = ExportMgr.FileExportProc();

	if(ids == 0)
		AfxMessageBox(_T("파일 반출을 완료하였습니다."));

}

CString CBO_ACQ_OrderDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 가격정책에 따라 절사단위설정값을 읽어와서 지정된 자릿수만큼 표기하도록 한다.
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);  // 2005-01-13
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if(m_strPosNumber == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(m_strPosNumber == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else  // m_strPosNumber == _T("0") || m_strPosNumber.Empty()
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

VOID CBO_ACQ_OrderDataMgr::ChangeMarc()
{
	//===============================================
	//20080627 ADD BY PJW : 마크교체는 종정보그리드에서 하도록 한다.
	PARENT_TYPE nParentType = SPECIES;
	INT nSelect;
	nSelect = m_tabOrderData.GetCurSel();
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	if( nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("종정보 그리드화면에서 마크교체를 해 주세요."));
		return ;
	}
	//===============================================
	// 종단위, 권단위 처리 필요
	CMARC_CHANGER_01 dlg(this);
	//===============================================
	//20080627 UPDATE BY PJW : 마크교체는 종정보그리드에서 하도록 한다.
	//2008.07.02 UPDATE BY PJW : 대표책의정보도 변경한다.
	//dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_주문자료관리"), _T("MainGrid_Volume"), _T("DM_주문자료관리_권"), _T("BS_MARC"));
	dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_주문자료관리"), _T("MainGrid"), _T("DM_주문자료관리_종"), _T("BS_MARC"), _T("DM_주문자료관리"));
	//===============================================

	//===============================================
	//2008.08.22 UPDATE BY PJW : 선정값을유지하고 그리드를 디스플레이한다.
// 	if ( dlg.DoModal() == IDOK )
//	{
//		AllControlDisplay(_T("CM_주문자료관리"));
//		m_pDM->RefreshPositionInfo();		
//	}
	m_pGrid_Species->SetProtectSelectColumn(TRUE);
	if ( dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_주문자료관리"));
		m_pDM->RefreshPositionInfo();	
	}
	m_pGrid_Species->Display();
	m_pGrid_Species->SetProtectSelectColumn(FALSE);
	//===============================================
	
//	m_pSearchDlg->ShowSearchDlg();
	
	//===============================================
	//20080627 UPDATE BY PJW : 마크교체이후 주문자료관리_종,주문자료관리_권,주문자료관리 DM을 Refresh한다.
//	INT nRowCnt;
//	CString strQuery;
//	CString strRecKeys;
//	CString strTempRecKeys;
//	CString strRecKey;
//	nRowCnt = m_pDM->GetRowCount();
//	for( INT i = 0 ; i < nRowCnt ; i++ )
//	{
//		
//	}

//	m_pDM				= FindDM(_T("DM_주문자료관리")			);
//	m_pPrintDM_Species	= FindDM(_T("DM_주문자료관리_종")		);
//	m_pPrintDM_Volume	= FindDM(_T("DM_주문자료관리_권")		);	
	
//	INT nRowCnt;
//	CString strQuery;
//	CString strRecKeys;
//	CString strTempRecKeys;
//	CString strRecKey;
//
//	nRowCnt = m_pPrintDM_Volume->GetRowCount();
//	for( INT i = 0 ; i < nRowCnt ; i++ )
//	{
//		m_pPrintDM_Volume->GetCellData(_T("BB_책KEY"), i, strRecKey);
//		if( strRecKeys.IsEmpty() )
//		{
//			strRecKeys = strRecKey;
//		}
//		else
//		{
//			strTempRecKeys = strRecKeys;
//			strRecKeys.Format(_T("%s, %s"), strTempRecKeys, strRecKey);
//		}
//	}
//
//	strQuery.Format(_T("B.REC_KEY IN( %s )"), strRecKeys );
//
//	m_pPrintDM_Volume->RefreshDataManager(strQuery);


//	//dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_주문자료관리"), _T("MainGrid_Volume"), _T("DM_주문자료관리_권"), _T("BS_MARC"));
//	CString strQuery;
//	//strQuery
//	m_pSearchDlg->GetWhereStmt(strQuery);
//	m_pDM->RefreshDataManager(strQuery);
//	m_pPrintDM_Species->RefreshDataManager(strQuery);
//	m_pPrintDM_Volume->RefreshDataManager(strQuery);
	//===============================================
}



//========================================================================
//20080605 ADD BY PJW : 종, 권 그리드에 따라서 DM의 선정 값을 변경한다.
INT CBO_ACQ_OrderDataMgr::SelectGridToDmChk( CESL_DataMgr* pDM		, 
											 CString strSpeciesKey	, 
											 CString strGrid		,
											 CString strChk			, 
											 CString strVolNo		)
{
	CString strTempKey;
	CString strTempVolNo;
	INT i;
	if( _T("종그리드") == strGrid )
	{
		for( i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			pDM->GetCellData( _T("IBS_종KEY"), i, strTempKey);
			if( strSpeciesKey == strTempKey )
			{
				pDM->SetCellData( _T("선정"), strChk, i );
			}
		}
	}
	else
	{
		for( i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			pDM->GetCellData( _T("IBS_종KEY"), i, strTempKey);
			pDM->GetCellData( _T("BB_편권차"), i, strTempVolNo);
			if( strSpeciesKey == strTempKey  &&  strVolNo == strTempVolNo)
			{
				pDM->SetCellData( _T("선정"), strChk, i );
			}
		}
	}
	return 0;
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : 마우스 UP시 선정값으로 SelectGridToDmChk()호출
void CBO_ACQ_OrderDataMgr::OnMouseUpgridORDERVOLUMEDATA(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	CString strChk;
	CString strSpeciesKey;
	CString strBookKey;
	CString strVolNo;
	CMSHFlexGrid *pGrid;	

	pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridORDER_VOLUME_DATA );
	nSelectRow	= pGrid->GetRow();
	nSelectCol	= pGrid->GetCol();
	strChk		= pGrid->GetText();
	if(  0 >= nSelectRow )
	{
		return;
	}

	nSelectRow -= 1;

	if ( 1 == Button && 1 == nSelectCol ) 
	{
		if( _T("V") == strChk ) 
		{
			m_pActiveDM->SetCellData( _T("선정"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_종KEY"), nSelectRow, strSpeciesKey);
			m_pActiveDM->GetCellData( _T("BB_편권차"), nSelectRow, strVolNo);
			SelectGridToDmChk( m_pDM, strSpeciesKey, _T("권그리드"), strChk,strVolNo);
		}
		else
		{
			m_pActiveDM->SetCellData( _T("선정"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_종KEY"), nSelectRow, strSpeciesKey);
			m_pActiveDM->GetCellData( _T("BB_편권차"), nSelectRow, strVolNo);
			SelectGridToDmChk( m_pDM, strSpeciesKey, _T("권그리드"), strChk, strVolNo);
		
		}

	}
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : 마우스 UP시 선정값으로 SelectGridToDmChk()호출
void CBO_ACQ_OrderDataMgr::OnMouseUpgridORDERDATA(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	CString strChk;
	CString strSpeciesKey;
	CString strBookKey;
	CMSHFlexGrid *pGrid;
	

	pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridORDER_DATA );
	nSelectRow	= pGrid->GetRow();
	nSelectCol	= pGrid->GetCol();
	strChk		= pGrid->GetText();
	
	if(  0 >= nSelectRow )
	{
		return;
	}

	nSelectRow -= 1;

	if ( 1 == Button && 1 == nSelectCol ) 
	{
		if( _T("V") == strChk ) 
		{
			m_pActiveDM->SetCellData( _T("선정"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_종KEY"), nSelectRow, strSpeciesKey);
			SelectGridToDmChk(m_pDM,strSpeciesKey,_T("종그리드"),strChk,_T(""));
		}
		else
		{
			m_pActiveDM->SetCellData( _T("선정"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_종KEY"), nSelectRow, strSpeciesKey);
			SelectGridToDmChk(m_pDM,strSpeciesKey,_T("종그리드"),strChk,_T(""));
		
		}
	}
}
//========================================================================



//========================================================================
//20080605 ADD BY PJW : 그리드 탭이 변경될때마다 DM의 선정값을 해제 한다.
void CBO_ACQ_OrderDataMgr::SetDmSelectToCancel( CESL_DataMgr* pCancelDM )
{
	INT nRow;
	nRow = pCancelDM->GetRowCount();
	for( INT i= 0 ; i<nRow ; i++ )
	{
		pCancelDM->SetCellData( _T("선정"), _T(""), i );
	}
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : 처리된 DM을 제거한다.
void CBO_ACQ_OrderDataMgr::DeleteDmRow( INT nSelectTab )
{
	CString strSelect;
	CString strActiveDMSpeciesKey;
	CString strNonActiveDMSpeciesKey;
	INT nActiveDMRowsCnt;
	INT nNonActiveDMRowsCnt;
	INT nChkCnt;
	INT i;
	INT j;

	if(nSelectTab == 0) 
	{
		//1. DM_주문자료관리_종의 ROW를 제거한다
		nActiveDMRowsCnt = m_pActiveDM->GetRowCount();
		for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		{	
			m_pActiveDM->GetCellData( _T("선정"), i, strSelect );
			if( _T("V") == strSelect )
			{
				m_pActiveDM->GetCellData( _T("IBS_종KEY"), i, strActiveDMSpeciesKey );
				//2. DM_주문자료관리_권 ROW를 제거한다
				nNonActiveDMRowsCnt = m_pPrintDM_Volume->GetRowCount();
				for( j = 0 ; j < nNonActiveDMRowsCnt ; j++)
				{
					m_pPrintDM_Volume->GetCellData( _T("IBS_종KEY"), j, strNonActiveDMSpeciesKey );
					if( strNonActiveDMSpeciesKey == strActiveDMSpeciesKey )
					{
						m_pPrintDM_Volume->DeleteRow(j);
						--nNonActiveDMRowsCnt;
						--j;
					}
				}
				m_pActiveDM->DeleteRow(i);
				--nActiveDMRowsCnt;
				--i;
			}
		}

		//3. DM_주문자료관리의 ROW를 제거한다
		//nActiveDMRowsCnt = m_pDM->GetRowCount();
		//for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		//{	
		//	m_pDM->GetCellData( _T("선정"), i, strSelect );
		//	if( _T("V") == strSelect )
		//	{
		//		m_pDM->DeleteRow(i);
		//		--nActiveDMRowsCnt;
		//		--i;
		//	}
		//}			
	}
	else
	{
		//1. DM_주문자료관리_권 ROW를 제거한다
		nActiveDMRowsCnt = m_pActiveDM->GetRowCount();
		for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		{	
			m_pActiveDM->GetCellData( _T("선정"), i, strSelect );
			if( _T("V") == strSelect )
			{
				m_pActiveDM->DeleteRow(i);
				--nActiveDMRowsCnt;
				--i;
			}
		}

		//2. DM_주문자료관리_종의 ROW를 제거한다
		nNonActiveDMRowsCnt = m_pPrintDM_Species->GetRowCount();
		for( i = 0 ; i < nNonActiveDMRowsCnt ; i++)
		{
			nChkCnt = 0;
			m_pPrintDM_Volume->GetCellData( _T("IBS_종KEY"), i, strNonActiveDMSpeciesKey );
			for( j = 0 ; j < nActiveDMRowsCnt ; j++)
			{
				m_pActiveDM->GetCellData( _T("IBS_종KEY"), j, strActiveDMSpeciesKey );
				if( strNonActiveDMSpeciesKey == strActiveDMSpeciesKey )
				{
					++nChkCnt;
					break;
				}
			}

			if( nChkCnt == 0 )
			{
				m_pPrintDM_Species->DeleteRow(i);
				--nNonActiveDMRowsCnt;
				--i;
			}			
		}
		//3. DM_주문자료관리의 ROW를 제거한다
		//nActiveDMRowsCnt = m_pDM->GetRowCount();
		//for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		//{	
		//	m_pDM->GetCellData( _T("선정"), i, strSelect );
		//	if( _T("V") == strSelect )
		//	{
		//		m_pDM->DeleteRow(i);
		//		--nActiveDMRowsCnt;
		//		--i;
		//	}
		//}	
	}
}
//========================================================================

VOID CBO_ACQ_OrderDataMgr::KSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo)
{
	INT nSelect;
	CString strQuery;

	strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN ('0', '1') AND B.WORKING_STATUS IN ('BOA112N','BOA113O') AND B.ACQ_CODE='%s'  AND P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO IN (%s)")
			, strAcqCode, strAcqYear, strWorkCode, strSeqNo);
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("주문자료를 구축중입니다."));	
	BEGIN_SBL();
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	
	DeleteDataProcessingDlg();	
	
	nSelect = m_tabOrderData.GetCurSel();

	MakeOrderDM(nSelect);

	END_SBL(m_pPrintDM_Volume->GetRowCount());

	m_pCM->SetControlMgrData(_T("그룹번호"), strSeqNo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);

	m_pSearchDlg->GetGroupCollection()->RemoveAll();
	m_pSearchDlg->GetGroupCollection()->SetAcqCode(strAcqCode);	
	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strAcqYear, strSeqNo, strWorkCode);	
	m_pSearchDlg->GetGroupCollection()->SetHaveGroupInfo(TRUE);
}
