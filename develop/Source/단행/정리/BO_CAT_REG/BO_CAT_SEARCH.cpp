// BO_CAT_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SEARCH.h"
#include "../../../공통/BO_ACQ_API/SortCondition.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/OtherGroupNo_List_Dlg.h"
#include "../../../공통/K2UP_Common/MulitComboBoxManager.h"
#include "../../../공통/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "CommonDefine.h"

//==============================================
//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
#include "SBL.H"
//==============================================

// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
#include "ESL_SearchEngine.h"
// KOL.RED.2018.228 ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH dialog
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"

CBO_CAT_SEARCH::CBO_CAT_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_SEARCH)
	m_bDataSortYN = FALSE;
	//}}AFX_DATA_INIT
	sDetailFlag = 0;
	m_nAcqType = -1;
	m_pWorkNoList = NULL;
	pMakeSearchData = NULL;
	m_bCreate = FALSE;
	m_pGroups = new CGroupCollection(this);
	m_pSortItem = NULL;
}

CBO_CAT_SEARCH::~CBO_CAT_SEARCH()
{
	if(m_pWorkNoList)
	{
		delete[] m_pWorkNoList;
		m_pWorkNoList = NULL;
	}
	if(m_pGroups)
	{
		delete m_pGroups;
		m_pGroups = NULL;
	}
	if(pMakeSearchData)
	{
		delete pMakeSearchData;
		pMakeSearchData = NULL;
	}
}

VOID CBO_CAT_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SEARCH)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SEARCH)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
	ON_CBN_SELCHANGE(IDC_cobAcqCode, OnSelchangecobAcqCode)
	ON_BN_CLICKED(IDC_btnREG_VIEW_GROUP, OnbtnVIEWGROUP)
	ON_EN_KILLFOCUS(IDC_eRECEIPT_YEAR, OnKillfocuseRECEIPTYEAR)
	ON_EN_KILLFOCUS(IDC_NO1, OnKillfocusNo1)
	ON_CBN_SELCHANGE(IDC_cmbWORK_CODE, OnSelchangecmbWORKCODE)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_REG_TITLE, OnSetfocusRegTitle)
	ON_EN_SETFOCUS(IDC_REG_NO2, OnSetfocusRegNo2)
	ON_EN_SETFOCUS(IDC_REG_NO1, OnSetfocusRegNo1)
	ON_EN_SETFOCUS(IDC_REG_AUTHOR, OnSetfocusRegAuthor)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_GROUPNO_YEAR, OnKillfocusGroupnoYear)
	ON_CBN_KILLFOCUS(IDC_cobAcqCode, OnKillfocuscobAcqCode)
	ON_EN_KILLFOCUS(IDC_GROUP_NO, OnKillfocusGroupNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH message handlers

BOOL CBO_CAT_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("SM_단행_정리_등록_검색_매니저")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;

	pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	this->SetCONNECTION_INFO(pDM->CONNECTION_INFO);


	CString m_Year;
	// 040304_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도"), m_Year);
	SetControlData(_T("CM_단행_등록_검색"), _T("원부년도"), m_Year);


	//간략 보기 
	OnbDetailNo(); 

	// MakeSearchData 생성
	pMakeSearchData = new CMakeSearchData(this);
	// 수입구분별로 컨트롤 설정
	CString strAcqType;
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_수입구분_코드"), strAcqType);
	m_nAcqType = _ttoi(strAcqType.GetBuffer(0));
	m_pGroups->SetAcqType((ACQ_TYPE)m_nAcqType);

	ShowCtrlByAcqType();

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BB_등록번호_까지|E|1124|||||||
	//BB_등록번호_시작|E|1325|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_단행_등록_검색"), _T("BB_등록번호_시작"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_단행_등록_검색"), _T("BB_등록번호_까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_CAT_SEARCH::Create(CWnd* pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_SEARCH::OnbSEARCH()
{
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================
	INT idx, nCount;
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,sACQUISIT_YEAR,sACQUISIT_TYPE;
	CESL_DataMgr *pDM;
	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	// 정렬필드 설정
	CBO_ACQ_Business_Api api(this);
	pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	//년도를 보여 준다.
	GetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_수입구분_코드")	  , sACQUISIT_TYPE , -1 ,1);

	// 차수 번호 와 가원부 번호를 메인에 보여 준다.
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작"), strWorkNo1);
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호끝"), strWorkNo2);
	

	if( (strWorkNo1 != _T("")) && (strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}

	SetControlData(_T("CM_단행_정리_등록_메인"), _T("차수번호"), strWorkNo1);
	SetControlData(_T("CM_단행_정리_등록_메인"), _T("년도"), sACQUISIT_YEAR);
	SetControlData(_T("CM_단행_정리_등록_메인"), _T("입수구분"), sACQUISIT_TYPE);
	
	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

// 검색 자료가 없거나 검색 오류가 있을 경우 메세지 처리함 
	idx = pDM->RefreshDataManager(strQry);
	if(idx < 0)
	{
		AfxMessageBox(_T("검색작업오류")); 
		return;
	}

	pCM = FindCM(_T("CM_단행_정리_등록_메인"));

	nCount = pDM->GetRowCount();
	if( nCount == 0 )
	{
		CString strMsg1;
		AfxMessageBox(_T("검색결과가 없습니다."));
		pCM->AllControlDisplay();
		//===================================================================
		//20080616 ADD BY PJW : 검색결과가 없을경우 MAIN 결과창 0으로 셋팅
		strMsg1.Format(_T("종수 : %5d\t책수 : %5d"), 0, 0);
		GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg1.GetBuffer(0));
		strMsg1.ReleaseBuffer();
		//===================================================================
		
		return;
	}

	// 정렬필드 설정
	api.InitTempData();	api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	INT ids = api.SPFExecutePath(_T("정렬필드설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("정렬필드를 설정 할 수 없습니다."));
		return;
	}

	//정렬기능을 넣어야 한다.
	m_pSortItem->DataSort(pDM, !m_bDataSortYN);

//////////////////////////////////////////////////////////

	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
    
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.9 이학중 추가 
	// 검색결과 상태바에 나타나게 
    CString strMsg ;
	INT nRowCount = 0 ;
	INT nBookCount = 0;
	nBookCount = pDM->GetRowCount () ;
	CMap <CString ,LPCTSTR,CString,LPCTSTR> mapRecKey;
	CString strSpecies_key;
	for(INT i =0 ; i<nBookCount;i++)
	{
        strSpecies_key=_T("") ;
		pDM->GetCellData (_T("IBS_종KEY"),i,strSpecies_key);
		if(!strSpecies_key.IsEmpty ())
        { 
			mapRecKey.SetAt(strSpecies_key,strSpecies_key);
		}
	}

	nRowCount = mapRecKey.GetCount();
    
    strMsg.Format(_T("종수 : %5d\t책수 : %5d"), nRowCount, nBookCount);
    GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//그리드 콜백 함수 연결 

	pCM->AllControlDisplay();

	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
// 	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nBookCount);
	//==============================================

	if( nRowCount > 0 )
		ShowWindow(SW_HIDE);
}

INT CBO_CAT_SEARCH::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_자료상태_코드")) {
            if ( str == _T("BOR111N")) {
				str = _T("등록인계자료");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("등록자료");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("가원부자료");
			}

			CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
			if (pDM)
			{
				pDM->SetCellData(_T("BB_자료상태_설명"), str, nRow);
			}
        }
    }
	return 0;

EFS_END
return -1;

}


CString CBO_CAT_SEARCH::MakeSelectQry()
{
	CString strQry; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd,sAccessionNoYear;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//자료상태
	CString sWorkStatus;
	GetControlData( _T("CM_단행_등록_검색"), _T("자료상태"), sWorkStatus );
	
	if ( sWorkStatus == _T("전체") ) 
	{
		strQry = strQry + _T("  B.WORKING_STATUS >= 'BOR111N' AND B.WORKING_STATUS <= 'BOR112N'");
	}
	else if ( sWorkStatus == _T("등록인계자료") )
	{
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR111N'");
	}
	else if ( sWorkStatus == _T("등록자료") )
	{
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR112N'");
	}

	strQry += _T(" AND B.BOOK_APPENDIX_FLAG = 'B' ");


	//수입 년도 원부 조건 고려  
	GetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();

	if ( sACQUISIT_YEAR != _T("") ) 
	{
			strQry = strQry + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//차수번호 
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작")   , SEQ_NO1); 

	SEQ_NO1.TrimLeft();
	SEQ_NO1.TrimRight();
	SEQ_NO2.TrimLeft();
	SEQ_NO2.TrimRight();

	if(m_nAcqType > 0 && !SEQ_NO1.IsEmpty())
	{
		if( m_nAcqType == 1 )
		{
			CString strTmpQuery, strTmpData;
			strTmpQuery.Format(_T("SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_SEQ_NO IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE"), SEQ_NO1);
			GetControlData(_T("CM_단행_등록_검색"), _T("작업번호"), strTmpData);
			if(!strTmpData.IsEmpty() && strTmpData != _T("적용안함")) 
				strTmpQuery += _T(" AND ORDER_WORK_NO = ") + strTmpData;
			if(!strTmpQuery.IsEmpty())
				strQry += _T(" AND B.ACQ_KEY IN (") + strTmpQuery + _T(")");
			strTmpData.Empty();
			strTmpQuery.Empty();
			GetControlData(_T("CM_단행_등록_검색"), _T("업무구분_코드"), strTmpData);
			if( strTmpData.IsEmpty() == TRUE )	
			{
				strQry += _T(" AND I.WORK_CODE IS NULL");
			}
			else
			{
				strTmpQuery.Format(_T(" AND I.WORK_CODE = '%s'"), strTmpData);
				strQry += strTmpQuery;
			}
		}
		else
		{
			strQry = strQry + _T(" AND B.SEQ_NO IN ( ") + SEQ_NO1 + _T(" )");
		}
	}
	
	//원부년도 원부번호
	GetControlData(_T("CM_단행_등록_검색"), _T("원부번호")   , sAccessionNoStart); 
	GetControlData(_T("CM_단행_등록_검색"), _T("원부년도")   , sAccessionNoYear); 
	sAccessionNoStart.TrimLeft();
	sAccessionNoStart.TrimRight();

	if (sAccessionNoStart != _T(""))
	{
		strQry = strQry + _T(" AND a.accession_rec_no = ")+sAccessionNoStart;
		
		if ( sACQUISIT_YEAR != _T("") ) 
		{
			strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sAccessionNoYear + _T("'");
		}		
	}

	//수입 구분 코드 & 수입년도 
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_수입구분_코드"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();

	if ( sACQUISIT_CODE != _T("") ) 
	{
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	//등록번호
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_등록번호_시작"), sRegNoStart);
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_등록번호_까지"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) 
// 	{
// 		if ( sRegNoStart.GetLength() > 2 ) 
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 			sRegNoStart = sRegNoStart.Left(2) + sRight;
// 		}
// 	}
// 	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) 
// 	{
// 		if ( sRegNoEnd.GetLength() > 2 ) 
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 		}
// 	}

	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) 
	{
		if ( sRegNoStart.GetLength() > nRegCodeLength ) 
		{
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) 
	{
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) 
		{
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	
	if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) 
	{
		strQry = _T("B.REG_NO = '") + sRegNoStart  + _T("' AND ") + strQry;
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) 
	{
		AfxMessageBox( _T("시작 등록번호를 입력 하세요."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) 
	{
		strQry = _T("B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("' AND ") + strQry;
	}

	//인계 처리일  

	GetControlData(_T("CM_단행_등록_검색"), _T("대상처리일_시작"), sTransferDataStart);
	GetControlData(_T("CM_단행_등록_검색"), _T("대상처리일_까지"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) 
	{
		strQry = strQry + _T(" AND B.REG_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) 
	{
		AfxMessageBox ( _T("시작 인계처리일을 입력하세요.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) 
	{
		strQry = strQry + _T(" AND B.REG_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.REG_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}

	//상세 검색일 경우 
	CStringArray arrECOSEARCH;	arrECOSEARCH.RemoveAll();
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		CMakeSearchData MakeData(this);

		//본표제  
		GetControlData(_T("CM_단행_등록_검색"), _T("본표제"), sTitle);	
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sTitle != _T("") ) 
		{
			INT iCh = MakeData.ChangeHanjaToHangul( sTitle );	//한자 한글 변환
			if ( 0 != iCh ) 
			{			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}						
			iCh = MakeData.GetKeyWord( sTitle );				//공백, 특수문자 제거
			if ( 0 != iCh ) 
			{			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}

			if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
			{
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
				strQry = strQry + _T(" AND ") + sWhereTitle;			
			}
			else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
			{
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
				arrECOSEARCH.Add(sWhereTitle);
			}			
		}

		//저작자
		GetControlData(_T("CM_단행_등록_검색"), _T("저작자"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) 
		{
			INT iCh = MakeData.ChangeHanjaToHangul( sAuthor );	//한자 한글 변환
			if ( 0 != iCh ) 
			{			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}						
			iCh = MakeData.GetKeyWord( sAuthor );				//공백, 특수문자 제거
			if ( 0 != iCh ) 
			{			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}

			if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
			{
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
				strQry = strQry + _T(" AND ") + sWhereTitle;
			}
			else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
			{
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
				arrECOSEARCH.Add(sWhereTitle);
			}
		}
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_관리구분_코드"), sMANAGE_CODE);
		sMANAGE_CODE.TrimLeft();	sMANAGE_CODE.TrimRight();
		if ( sMANAGE_CODE != _T("") ) 
		{
			if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
			{
				sWhereTitle.Format( _T("CATSEARCH(I.MANAGE_CODE,'%s*',NULL)>0") , sMANAGE_CODE );
				strQry = strQry + _T(" AND ") + sWhereTitle;
			}
			else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
			{
				//csj
				// 웹검색할땐 안한다.
				// 일반 오라클 에코서치 할때도 구지 필요없지만 일단 유지..
				//sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_MANAGE_CODE, '%s*')"), sMANAGE_CODE );
				//arrECOSEARCH.Add(sWhereTitle);
				if(  CESL_SearchEngine::GetUsed() == FALSE )
				{
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_MANAGE_CODE, '%s*')"), sMANAGE_CODE );
				arrECOSEARCH.Add(sWhereTitle);
			}
		}
		}

		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			CESL_DataMgr TmpDM;
			CString strTmpData, strOption;
			INT nCnt = arrECOSEARCH.GetSize();
			INT ids;
			strOption.Empty();
			for(INT i=0 ; i<nCnt ; i++)
			{
				strTmpData = arrECOSEARCH.GetAt(i);
				strTmpData.MakeUpper();
				strTmpData.Replace(_T("I.REC_KEY"), _T(""));
				strTmpData.TrimLeft();	strTmpData.TrimRight();
				ids = TmpDM.GetECOSEARCHReplace(strTmpData);
				if(strOption.IsEmpty()) 
					strOption = strTmpData;
				else
					strOption += _T(" INTERSECT ") + strTmpData;
			}
			if(!strOption.IsEmpty())
			{
				strQry += _T(" AND I.REC_KEY IN ( ") + strOption + _T(" )");
			}
		}
		//코드 구분값
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_형식구분_코드"), sFORM_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_매체구분_코드"), sMEDIA_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_자료구분_코드"), sMAT_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_이용대상구분_코드"), sUSE_OBJ_CODE);
		
		CString sSUB_REG_CODE;
		GetControlData(_T("CM_단행_등록_검색"), _T("BB_보조등록구분"), sSUB_REG_CODE);
		
		
		//코드값
		sFORM_CODE.TrimLeft();
		if ( sFORM_CODE != _T("") ) 
		{
			strQry = strQry + _T(" AND I.FORM_CODE = '") + sFORM_CODE + _T("'");		
		}
		sMEDIA_CODE.TrimLeft();
		if ( sMEDIA_CODE != _T("") ) 
		{
			strQry = strQry + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}
		sMAT_CODE.TrimLeft();
		if ( sMAT_CODE != _T("") ) 
		{
			strQry = strQry + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sUSE_OBJ_CODE.TrimLeft();
		if ( sUSE_OBJ_CODE != _T("") )
		{
			strQry = strQry + _T(" AND I.USE_OBJ_CODE = '") + sUSE_OBJ_CODE + _T("'");
		}

		sSUB_REG_CODE.TrimLeft();
		if ( sSUB_REG_CODE != _T("") )
		{
			strQry = strQry + _T(" AND I.SUB_REG_CODE = '") + sSUB_REG_CODE + _T("'");
		}

		//csj
		// 웹검색할땐 바로 매니저 코드 지정.
		
		if(  CESL_SearchEngine::GetUsed() )
		{
			if ( sMANAGE_CODE != _T("") ) 
			{
				strQry = strQry + _T(" AND I.MANAGE_CODE = '") + sMANAGE_CODE + _T("'");
			}
		}
		
		
/*
		// 작업자를 처리합쉬다.
		UpdateData(TRUE);
		if(m_nMyData == 0)
		{
			strQry += _T(" AND S.WORKER = '") + m_pInfo->USER_ID + _T("'");
		}
*/
	}
	// 업무구분을 처리합쉬다.
	
	CString strTmpData;
	GetControlData(_T("CM_단행_등록_검색"), _T("업무구분_코드"),  strTmpData);
	strTmpData.TrimLeft();	strTmpData.TrimRight();
	/***************************************************
	 ***************************************************/
	//20080307 UPDATE BY PJW : 수입구분이 구입이 아닐경우 업무구분코드를 넣지 않는다
	//if(!strTmpData.IsEmpty())
	if(!strTmpData.IsEmpty() && 1 == m_nAcqType )
	{
		strQry += _T(" AND I.WORK_CODE = '") + strTmpData + _T("'");
	}
	/***************************************************
	 ***************************************************/

	return strQry;
}

VOID CBO_CAT_SEARCH::OnbDetailNo() 
{
	// TODO: Add your control notification handler code here
	CString sDetailSee;
	CWnd * pWnd = NULL;
	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcSORT_CONDITION_GROUP);
	ArrResourceID.Add(IDC_chkSORT_YN);
	ArrResourceID.Add(IDC_btnSET_SORT_CONDITION);
	CRect rect;
	INT i;
	
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetailSee);

	if ( sDetailSee == _T("간략검색") )
	{
		sDetailFlag = 0;
		this->GetWindowRect( rect );		
		rect.bottom -= 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("상세검색"));
		GetDlgItem(IDC_guide_line)->ShowWindow( false );		
		GetDlgItem(IDC_REG_7)->ShowWindow( false );
		GetDlgItem(IDC_REG_8)->ShowWindow( false );
		GetDlgItem(IDC_REG_TITLE)->ShowWindow( false );
		GetDlgItem(IDC_REG_AUTHOR)->ShowWindow( false );

		for(i=0 ; i<ArrResourceID.GetSize() ; i++)
		{
			pWnd = GetDlgItem(ArrResourceID.GetAt(i));
			if (pWnd->GetSafeHwnd() != NULL)
			{
				pWnd->GetWindowRect(&rect);
				ScreenToClient(rect);
				
				rect.top -= 165;
				rect.bottom -= 165;
				pWnd->MoveWindow(rect);
			}		
		}
	}
	else
	{
		sDetailFlag = 1;
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom += 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("간략검색"));
		GetDlgItem(IDC_guide_line)->ShowWindow( true );
		GetDlgItem(IDC_REG_7)->ShowWindow( true );
		GetDlgItem(IDC_REG_8)->ShowWindow( true );
		GetDlgItem(IDC_REG_TITLE)->ShowWindow( true );
		GetDlgItem(IDC_REG_AUTHOR)->ShowWindow( true );

		for(i=0 ; i<ArrResourceID.GetSize() ; i++)
		{
			pWnd = GetDlgItem(ArrResourceID.GetAt(i));
			if (pWnd->GetSafeHwnd() != NULL)
			{
				pWnd->GetWindowRect(&rect);
				ScreenToClient(rect);
				
				rect.top += 165;
				rect.bottom += 165;
				pWnd->MoveWindow(rect);
			}		
		}
	}

}

BOOL CBO_CAT_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_NO1)->GetSafeHwnd())
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != ',' && nVirtKey != ' ' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_CAT_SEARCH::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_등록_검색"));
	if (pCM)
		pCM->AllControlClear();
	/***************************************************
	 ***************************************************/
	//20080515 ADD BY PJW : 화면지우기 버튼 클릭시 작업번호란도 초기화되도록 수정
	CString strWorkNo;
	pCM->GetControlMgrData(_T("작업번호"), strWorkNo);
	if(!strWorkNo.IsEmpty())
	{
		CMulitComboBoxManager mgr(this);
			mgr.SetSingleComboBoxStrData(pCM->CM_ALIAS, _T("적용안함"), m_pWorkNoList, _T("작업번호"), 1);
	}
	/***************************************************
	 ***************************************************/
	/*******************************************************
	 *******************************************************/
	//20080131 ADD BY PJW : 변경된 수입구분에 따라서 화면을 다시 보여준다.
	OnSelchangecobAcqCode();
	/*******************************************************
	 *******************************************************/
}


//====================================
//20080612 ADD BY PJW : 스페이스바를 눌러서 콤보박스 변경할경우 적용 되도록 하기위해서 코드를 OnKillfocuscobAcqCode()로 옮긴다.
VOID CBO_CAT_SEARCH::OnSelchangecobAcqCode() 
{
	OnKillfocuscobAcqCode();
}
//====================================

VOID CBO_CAT_SEARCH::ShowCtrlByAcqType()
{
	if(m_nAcqType == 1)
	{
		GetDlgItem(IDC_NO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_REGWORK_NO)->SetWindowText(_T("차수번호"));
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->SetWindowText(_T("차수번호보기"));
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcWORKNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbWORK_NO)->ShowWindow(SW_SHOW);
		/*******************************************************
		 *******************************************************/
		//20080131 ADD BY PJW : 구입일때 업무구분을 보여 중다.
		GetDlgItem(IDC_stcWORK_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbWORK_CODE)->ShowWindow(SW_SHOW);
		/*******************************************************
		 *******************************************************/
	}
	else if(m_nAcqType == 2)
	{
		GetDlgItem(IDC_NO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_REGWORK_NO)->SetWindowText(_T("접수번호"));
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->SetWindowText(_T("접수번호보기"));
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcWORKNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbWORK_NO)->ShowWindow(SW_HIDE);
		/*******************************************************
		 *******************************************************/
		//20080131 ADD BY PJW : 기증일때 업무구분을 숨긴다.
		GetDlgItem(IDC_stcWORK_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbWORK_CODE)->ShowWindow(SW_HIDE);
		/*******************************************************
		 *******************************************************/
	}
	else if(m_nAcqType == 0)
	{
		GetDlgItem(IDC_NO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcWORKNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbWORK_NO)->ShowWindow(SW_HIDE);
		/*******************************************************
		 *******************************************************/
		//20080131 ADD BY PJW : 적용안함 일때 업무구분을 숨긴다.
		GetDlgItem(IDC_stcWORK_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbWORK_CODE)->ShowWindow(SW_HIDE);
		/*******************************************************
		 *******************************************************/
	}
	else
	{
		GetDlgItem(IDC_NO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_REGWORK_NO)->SetWindowText(_T("그룹번호"));
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->SetWindowText(_T("그룹번호보기"));
		GetDlgItem(IDC_btnREG_VIEW_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcWORKNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbWORK_NO)->ShowWindow(SW_HIDE);
	}
}

VOID CBO_CAT_SEARCH::OnbtnVIEWGROUP() 
{
	// TODO: Add your control notification handler code here
	if(m_nAcqType == 1) ShowDivNoList();
	else if(m_nAcqType == 2) ShowReceiptNoList();
    else if(m_nAcqType != 0) ShowOtherGroupNoList();
}

INT CBO_CAT_SEARCH::ShowDivNoList()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_등록_검색"));
	if(pCM == NULL) return -1;

	CString strAcqYear, strWorkCode;
	pCM->GetControlMgrData(_T("BS_수입년도"), strAcqYear);
	pCM->GetControlMgrData(_T("업무구분_코드"), strWorkCode);
	
	CDivisionNo_List_Dlg dlg(this);
	dlg.SetWorkStatus(_T("검수완료"));
	dlg.SetSpeciesType(BO_REG_PURCHASE);
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetGroupCollection(m_pGroups);
	if(IDOK == dlg.DoModal())
	{
		pCM->SetControlMgrData(_T("차수번호시작"), dlg.GetSelDivNo());
		pCM->SetControlMgrData(_T("BS_수입년도"), m_pGroups->GetAcqYear());
		pCM->SetControlMgrData(_T("업무구분_코드"), m_pGroups->GetWorkCode());
		OnKillfocusNo1();
		PostMessage(WM_COMMAND, IDC_bSEARCH, BN_CLICKED);
	}
	return 0;
}

INT CBO_CAT_SEARCH::ShowReceiptNoList()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_등록_검색"));
	if(pCM == NULL) return -1;
	
	CString strAcqYear;
	pCM->GetControlMgrData(_T("BS_수입년도"), strAcqYear);
	
	CRequisitionNo_List_Dlg dlg(this);
	dlg.SetSpeciesType(BO_REG_DONATE);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetGroupCollection(m_pGroups);
	if(IDOK == dlg.DoModal())
	{
		pCM->SetControlMgrData(_T("차수번호시작"), dlg.GetSelInfo());
		pCM->SetControlMgrData(_T("BS_수입년도"), m_pGroups->GetAcqYear());
		PostMessage(WM_COMMAND, IDC_bSEARCH, BN_CLICKED);
	}
	return 0;
}

VOID CBO_CAT_SEARCH::OnKillfocuseRECEIPTYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : 출판년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
	/*
	CString strAcqYear;
	INT nYear;
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_eRECEIPT_YEAR);
	pAcqYear->GetWindowText(strAcqYear);
	if( strAcqYear.IsEmpty() )
	{
		return;
	}
	else
	{
		nYear = _ttoi(strAcqYear);
	}
	if( nYear < 1000 || nYear > 9999 )
	{

		ESLAfxMessageBox(_T("1000 ~ 9999 사이의 값을 입력해주세요"));
		pAcqYear->SetWindowText(_T(""));
	}
	*/
	//============================================	
	OnKillfocusNo1();
}

VOID CBO_CAT_SEARCH::OnKillfocusNo1() 
{
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_NO1);
	if (pAcqYear->GetStyle() & ES_READONLY) return;	
	
	CESL_ControlMgr *m_pCM = FindCM(_T("CM_단행_등록_검색"));
	if(m_pCM == NULL) return ;
	
	CString strAcqYear, strWorkCode, strGroupNo;
	m_pCM->GetControlMgrData(_T("BS_수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("차수번호시작"), strGroupNo);
	m_pCM->GetControlMgrData(_T("업무구분_코드"), strWorkCode);
	
	strGroupNo = CheckValidateGroupNo(m_pCM, strGroupNo, strAcqYear, strWorkCode);
	m_pCM->SetControlMgrData(_T("차수번호시작"), strGroupNo);
	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	m_pGroups->ParsingString(_T(","), strGroupNo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	INT ids = -1;
	if(cnt == 1 && m_nAcqType == 1)
	{
		ids = SetWorkNoData(strAcqYear, arrDivNo.GetAt(0), strWorkCode);
		if(ids < 0) 
		{
			//===========================================================
			//20080529 ADD BY PJW : 작업번호 구축을 계속 하지 않도록 차수번호를 NULL로 바꾼다.
			m_pCM->SetControlMgrData(_T("차수번호시작"), _T(""));
			//===========================================================
			ESLAfxMessageBox(_T("작업번호 정보를 구축할 수 없습니다."));
			return;
		}
	}	
	else if(cnt > 1 && m_nAcqType == 1)
	{
		if(m_pWorkNoList != NULL)
		{
			delete[] m_pWorkNoList;
			m_pWorkNoList = NULL;
		}
		m_pWorkNoList = new CString[1];
		m_pWorkNoList[0] = _T("적용안함");
		CMulitComboBoxManager mgr(this);
		mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), m_pWorkNoList, _T("작업번호"), 1);
	}	
}

INT CBO_CAT_SEARCH::SetWorkNoData(CString strAcqYear, CString strDivNo, CString strWorkCode/*=_T("")*/)
{
	CESL_ControlMgr *m_pCM = FindCM(_T("CM_단행_등록_검색"));
	if(m_pCM == NULL) return -1;

	CGroupMgr groupmgr(this);
	groupmgr.InitTempData();
	groupmgr.SetTempData(_T("AcqYear"),		strAcqYear.GetBuffer(0)	);
	groupmgr.SetTempData(_T("DivNo"),		strDivNo.GetBuffer(0)	);
	groupmgr.SetTempData(_T("WorkCode"),	strWorkCode.GetBuffer(0));
	
	INT ids = groupmgr.SPFExecutePath(_T("마지막작업번호추출"));
	if(ids < 0) return ids;
	
	CString strTmpData;
	if(m_pWorkNoList != NULL)
	{
		delete[] m_pWorkNoList;
		m_pWorkNoList = NULL;
	}
	
	m_pWorkNoList = new CString[ids+1];
	m_pWorkNoList[0] = _T("적용안함");
	for(INT i=1 ; i<=ids ; i++)
	{
		strTmpData.Format(_T("%d"), i);
		m_pWorkNoList[i] = strTmpData;
	}
	
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), m_pWorkNoList, _T("작업번호"), ids+1);
	return 0;
}

CString CBO_CAT_SEARCH::CheckValidateGroupNo(CESL_ControlMgr * pCM, CString strGroupInfo, CString strAcqYear, CString strWorkCode/*=_T("")*/)
{
	INT nInitCnt = -1;
	
	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	m_pGroups->ParsingString(_T(","), strGroupInfo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	nInitCnt = cnt;
	
	INT ids = -1;
	
	CString strMsg, strData;
	for(INT i=0 ; i<cnt && cnt>0; i++)
	{
		strData = arrDivNo.GetAt(i);
		if(m_nAcqType== 1 || m_nAcqType== 2)
		{
			if(!pMakeSearchData->CheckDigit(strData))
			{
				strMsg.Format(_T("입력하신 [%s]정보는 사용할 수 없습니다."), strData);
				ESLAfxMessageBox(strMsg);
				arrDivNo.RemoveAt(i);
				i--;
				cnt--;
				continue;
			}
		}
		
		// 2008.03.13 REM BY PDJ
		// 차수상태에 따른 검색 제한 없앰
		/*
		ids = m_pGroups->CheckValidate(arrDivNo.GetAt(i), strAcqYear, strWorkCode);
		if(ids < 0)
		{
			if(m_nAcqType== 1)
				strMsg.Format(_T("입력하신 차수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else if(m_nAcqType== 2)
				strMsg.Format(_T("입력하신 접수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else
				strMsg.Format(_T("입력하신 그룹번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));

			ESLAfxMessageBox(strMsg);
			arrDivNo.RemoveAt(i);
			i--;
			cnt--;
		}
		*/
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

VOID CBO_CAT_SEARCH::OnSelchangecmbWORKCODE() 
{
	// TODO: Add your control notification handler code here
	OnKillfocusNo1();
}
	
INT CBO_CAT_SEARCH::ShowOtherGroupNoList()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_등록_검색"));
	if(pCM == NULL) return -1;
	
	CString strAcqYear, strAcqCode;
	pCM->GetControlMgrData(_T("BS_수입년도"), strAcqYear);
	strAcqCode.Format(_T("%d"), m_nAcqType);

	COtherGroupNo_List_Dlg dlg(this);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetAcqCode(strAcqCode);
	dlg.SetGroupCollection(m_pGroups);
	if(IDOK == dlg.DoModal())
	{
		pCM->SetControlMgrData(_T("차수번호시작"), dlg.GetSelInfo());
		pCM->SetControlMgrData(_T("BS_수입년도"), m_pGroups->GetAcqYear());
		PostMessage(WM_COMMAND, IDC_bSEARCH, BN_CLICKED);
	}
	return 0;
}

VOID CBO_CAT_SEARCH::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItem == NULL) return;
	CDataSortDlg dlg(this);
	dlg.SetSortItemSet(m_pSortItem);
	dlg.SetOpenMode(-1);
	dlg.DoModal();
}

VOID CBO_CAT_SEARCH::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton*)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;
}


VOID CBO_CAT_SEARCH::SetIME(INT nIDC, INT nMode)
{
    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
}

VOID CBO_CAT_SEARCH::OnSetfocusRegTitle() 
{
	SetIME(IDC_REG_TITLE, 1);	
}

VOID CBO_CAT_SEARCH::OnSetfocusRegNo2() 
{
	SetIME(IDC_REG_NO2, 0);	
}

VOID CBO_CAT_SEARCH::OnSetfocusRegNo1() 
{
	SetIME(IDC_REG_NO1, 0);	
}

VOID CBO_CAT_SEARCH::OnSetfocusRegAuthor() 
{
	SetIME(IDC_REG_AUTHOR, 1);	
}


HBRUSH CBO_CAT_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CBO_CAT_SEARCH::OnKillfocusGroupnoYear() 
{
	//============================================
	//20080603 ADD BY PJW : 출판년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
	CString strGroupYear;
	INT nYear;
	CEdit * pGroupYear = (CEdit*)GetDlgItem(IDC_GROUPNO_YEAR);
	pGroupYear->GetWindowText(strGroupYear);
	if( strGroupYear.IsEmpty() )
	{
		return;
	}
	else
	{
		nYear = _ttoi(strGroupYear);
	}
	if( nYear < 1000 || nYear > 9999 )
	{
		ESLAfxMessageBox(_T("1000 ~ 9999 사이의 값을 입력해주세요"));
		pGroupYear->SetWindowText(_T(""));
	}
	//============================================	
	
}

//====================================
//20080612 ADD BY PJW : 스페이스바를 눌러서 콤보박스 변경할경우 적용 되도록 하기위해서 코드를 OnKillfocuscobAcqCode()로 옮긴다.
void CBO_CAT_SEARCH::OnKillfocuscobAcqCode() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_등록_검색"));
	if(pCM == NULL) return;

	CString strAcqType, strAcqYear ;
	pCM->GetControlMgrData(_T("BB_수입구분_코드"), strAcqType);
	if(m_nAcqType != _ttoi(strAcqType.GetBuffer(0)))
	{
		m_nAcqType = _ttoi(strAcqType.GetBuffer(0));
		if(m_pGroups)
		{
			delete m_pGroups;
		}
		m_pGroups = new CGroupCollection(this);
		if(m_nAcqType == 1 || m_nAcqType == 2 || m_nAcqType == 0)
			m_pGroups->SetAcqType((ACQ_TYPE)m_nAcqType);
		else
		{
			m_pGroups->SetAcqType(OTHER_ACQ_TYPE);
			m_pGroups->SetAcqCode(strAcqType);
		}

		pCM->SetControlMgrData(_T("차수번호시작"), _T(""));
		
		CString strTmpData;
		if(m_pWorkNoList != NULL)
		{
			delete[] m_pWorkNoList;
			m_pWorkNoList = NULL;
		}
		
		m_pWorkNoList = new CString[1];
		m_pWorkNoList[0] = _T("적용안함");
		
		CMulitComboBoxManager mgr(this);
		mgr.SetSingleComboBoxStrData(pCM->CM_ALIAS, _T("적용안함"), m_pWorkNoList, _T("작업번호"), 1);
	}

	ShowCtrlByAcqType();	
}
//====================================



//===================================================
//2010.05.19 ADD BY PJW : 원부번호 입력시 숫자를 체크한다.
void CBO_CAT_SEARCH::OnKillfocusGroupNo() 
{
	// TODO: Add your control notification handler code here
	INT nIds = 0;
	CString strData;
	CEdit* pedtGROUP_NO = (CEdit*)GetDlgItem(IDC_GROUP_NO);
	pedtGROUP_NO->GetWindowText(strData);
	strData.TrimLeft(); strData.TrimRight();
	if( !strData.IsEmpty() )
	{
		nIds = CheckIsDigit(strData);
		if( 0 > nIds )
		{
			AfxMessageBox(_T("원부번호는 숫자만 입력 가능합니다."));
			pedtGROUP_NO->SetFocus();
		}
	}		
}

INT CBO_CAT_SEARCH::CheckIsDigit(CString strData) 
{
	TCHAR ch = 0;
	INT nLength = strData.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strData.GetAt(idx);
		if (!_istdigit(ch))
		{
			return -1;
		}
	}
	return 0;
}
//===================================================