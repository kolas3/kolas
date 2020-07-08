// BO_CAT_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SEARCH.h"
#include "../../../공통/BO_ACQ_COMMON/BO_ACQ_DataSortDlg.h"
#include "../../../공통/BO_ACQ_API/SortCondition.h"

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
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	sDetailFlag = 0;
}

CBO_CAT_SEARCH::~CBO_CAT_SEARCH()
{

}



VOID CBO_CAT_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SEARCH)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
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

	
	GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("차수년도"));

	CESL_DataMgr *pDM;

	pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	CString m_Year;
	CTime mTime = pDM->GetDBTime();
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도"), m_Year);
	SetControlData(_T("CM_단행_등록_검색"), _T("원부년도"), m_Year);


	//간략 보기 
	OnbDetailNo(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_CAT_SEARCH::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_SEARCH::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here

	//필수 검색 항목 체크
	CString sAccNoSearch, sRegNoSearch, sDateSearch, sTitleSearch, sAuthorSearch;
	GetDlgItem(IDC_GROUP_NO)->GetWindowText(sAccNoSearch);
	GetDlgItem(IDC_REG_NO1)->GetWindowText(sRegNoSearch);
	//GetDlgItem(IDC_eACCESSION_NO_START)->GetWindowText(sDateSearch);
	GetControlData( _T("CM_단행_등록_검색"), _T("대상처리일_시작"), sDateSearch);
	GetDlgItem(IDC_REG_TITLE)->GetWindowText(sTitleSearch);
	GetDlgItem(IDC_REG_AUTHOR)->GetWindowText(sAuthorSearch);	
	
	if ( sAccNoSearch == _T("") && sRegNoSearch == _T("") && sDateSearch == _T("") && sTitleSearch == _T("") && sAuthorSearch == _T("") ) {	
		//AfxMessageBox( _T("검색 항목을 입력하세요.") );
		AfxMessageBox( _T("검색 조건이 부족합니다.") );
		return;	
	}



	
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,sACQUISIT_YEAR,sACQUISIT_TYPE;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	//년도를 보여 준다.

	GetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_수입구분_코드")	  , sACQUISIT_TYPE , -1 ,1);

	// 차수 번호 와 가원부 번호를 메인에 보여 준다.
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작"), strWorkNo1);
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호끝"), strWorkNo2);
	

	if( (strWorkNo1 != _T(""))&&(strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}

	SetControlData(_T("CM_단행_정리_등록_메인"), _T("차수번호"), strWorkNo1);
	SetControlData(_T("CM_단행_정리_등록_메인"), _T("년도"), sACQUISIT_YEAR);
	SetControlData(_T("CM_단행_정리_등록_메인"), _T("입수구분"), sACQUISIT_TYPE);

	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

// 검색 자료가 없거나 검색 오류가 있을 경우 메세지 처리함 
	INT idx = pDM->RefreshDataManager(strQry);
	if (idx < 0)
	{
		AfxMessageBox(_T("검색작업오류")); 
		return;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리_등록_메인"));

	INT nCount = pDM->GetRowCount();
	if (nCount == 0)
	{
		AfxMessageBox(_T("검색결과가 없습니다."));
		pCM->AllControlDisplay();
		return;
	}

	//정렬기능을 넣어야 한다.
	INT nCondColumns = 0;
	CString strAlias[4];
	INT nCond[4];
	strAlias[nCondColumns] = _T("BB_수입구분");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_수입년도");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_일련번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

//////////////////////////////////////////////////////////
	CESL_Grid *pGrid_MainGrid_All;

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
	
	if ( sWorkStatus == _T("전체") ) {
		strQry = strQry + _T("  B.WORKING_STATUS >= 'BOR111N' AND B.WORKING_STATUS <= 'BOR112N'");
	}
	else if ( sWorkStatus == _T("등록인계자료") ) {
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR111N'");
	}
	else if ( sWorkStatus == _T("등록자료") ) {
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR112N'");
	}

	strQry += _T(" AND B.BOOK_APPENDIX_FLAG = 'B' ");


	//수입 년도 원부 조건 고려  
	GetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();

	if ( sACQUISIT_YEAR != _T("") ) {
			strQry = strQry + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}
	//일련번호
	GetControlData(_T("CM_단행_등록_검색"), _T("일련번호시작")   , preGroup1); 
	GetControlData(_T("CM_단행_등록_검색"), _T("일련번호끝")     , preGroup2); 


	//차수번호 
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작")   , SEQ_NO1); 
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호끝")   , SEQ_NO2); 

	SEQ_NO1.TrimLeft();
	SEQ_NO1.TrimRight();
	SEQ_NO2.TrimLeft();
	SEQ_NO2.TrimRight();

	if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 != _T("")) )
	{
		strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO1 + _T(" AND B.SEQ_NO <= ") + SEQ_NO2 ;

		if ( (preGroup1 != _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO >= ") + preGroup1 + _T(" AND B.SERIAL_NO <= ") + preGroup2 ;
		}
		if ( (preGroup1 != _T(""))&&(preGroup2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO = ") + preGroup1 ;
		}
		if ( (preGroup1 == _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO = ") + preGroup2 ;
		}


	}
	else if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 == _T("")) )
	{
		strQry = strQry + _T(" AND B.SEQ_NO = ") + SEQ_NO1;

		if ( (preGroup1 != _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO >= ") + preGroup1 + _T(" AND B.SERIAL_NO <= ") + preGroup2 ;
		}
		if ( (preGroup1 != _T(""))&&(preGroup2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO = ") + preGroup1 ;
		}
		if ( (preGroup1 == _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO = ") + preGroup2 ;
		}

	}
	else if ( (SEQ_NO1 == _T(""))&&(SEQ_NO2 != _T("")) )
	{
		strQry = strQry + _T(" AND B.SEQ_NO = ") + SEQ_NO2;

		if ( (preGroup1 != _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO >= ") + preGroup1 + _T(" AND B.SERIAL_NO <= ") + preGroup2 ;
		}
		if ( (preGroup1 != _T(""))&&(preGroup2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO = ") + preGroup1 ;
		}
		if ( (preGroup1 == _T(""))&&(preGroup2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SERIAL_NO = ") + preGroup2 ;
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
		
		if ( sACQUISIT_YEAR != _T("") ) {
			strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sAccessionNoYear + _T("'");
		}
		
	}


	//수입 구분 코드 & 수입년도 
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_수입구분_코드"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();


	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	

	//등록번호
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_등록번호_시작"), sRegNoStart);
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_등록번호_까지"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
// 		if ( sRegNoStart.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 			sRegNoStart = sRegNoStart.Left(2) + sRight;
// 		}
// 	}
// 	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
// 		if ( sRegNoEnd.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 		}
// 	}
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = m_pParent->m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) 
	{
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) 
		{
			sRegNoEnd = m_pParent->m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
		}
	}
	//=====================================================
	

	if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {
		strQry = _T("B.REG_NO = '") + sRegNoStart  + _T("' AND ") + strQry;
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
		AfxMessageBox( _T("시작 등록번호를 입력 하세요."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		strQry = _T("B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("' AND ") + strQry;
	}

	//인계 처리일  

	GetControlData(_T("CM_단행_등록_검색"), _T("대상처리일_시작"), sTransferDataStart);
	GetControlData(_T("CM_단행_등록_검색"), _T("대상처리일_까지"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strQry = strQry + _T(" AND B.REG_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("시작 인계처리일을 입력하세요.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strQry = strQry + _T(" AND B.REG_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.REG_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}

	//상세 검색일 경우 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		CMakeSearchData *pMakeData = NULL;

		//본표제  
		GetControlData(_T("CM_단행_등록_검색"), _T("본표제"), sTitle);	
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sTitle != _T("") ) {
			INT iCh = pMakeData->ChangeHanjaToHangul( sTitle );	//한자 한글 변환
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}						
			iCh = pMakeData->GetKeyWord( sTitle );				//공백, 특수문자 제거
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}

			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;			
		}

		//저작자
		GetControlData(_T("CM_단행_등록_검색"), _T("저작자"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) {
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			//#else		
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;
		}

		//코드 구분값
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_형식구분_코드"), sFORM_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_매체구분_코드"), sMEDIA_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_자료구분_코드"), sMAT_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_이용대상구분_코드"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_관리구분_코드"), sMANAGE_CODE);

		//코드값
		sFORM_CODE.TrimLeft();
		if ( sFORM_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.FORM_CODE = '") + sFORM_CODE + _T("'");		
		}
		sMEDIA_CODE.TrimLeft();
		if ( sMEDIA_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}
		sMAT_CODE.TrimLeft();
		if ( sMAT_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sUSE_OBJ_CODE.TrimLeft();
		if ( sUSE_OBJ_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.USE_OBJ_CODE = '") + sUSE_OBJ_CODE + _T("'");
		}
		sMANAGE_CODE.TrimLeft();
		if ( sMANAGE_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MANAGE_CODE = '") + sMANAGE_CODE + _T("'");
		}

		//작업자
		CString sWorker;
		GetControlData(_T("CM_단행_등록_검색"), _T("BS_작업자"), sWorker);
		if ( sWorker != _T("") ) {
//			strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	

		
	}
	

	return strQry;

}

VOID CBO_CAT_SEARCH::OnbDetailNo() 
{
	// TODO: Add your control notification handler code here
	CString sDetailSee;
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetailSee);

	if ( sDetailSee == _T("간략검색") )
	 {
		sDetailFlag = 0;
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom -= 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("상세검색"));
		GetDlgItem(IDC_guide_line)->ShowWindow( false );		
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
	}

}

BOOL CBO_CAT_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_CAT_SEARCH::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_등록_검색"));
	if (pCM)
		pCM->AllControlClear();
}

INT CBO_CAT_SEARCH::SortData(CESL_DataMgr * pDM)
{

	CBO_ACQ_DataSortDlg dlg;
	CSortCondition * pCondition = dlg.GetSortCondition(_T(""));

	if(pDM->GetRowCount() == 0)
	{
		return 0;
	}

	// SORT
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = _T("BB_등록번호");	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_수입구분");	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_수입년도");	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;

	if(pCondition->m_nFirstType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strFirstAlias;	
		nCond[nCondColumns] = pCondition->m_nFirstType;		
		nCondColumns++;
	}
	if(pCondition->m_nSecondType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strSecondAlias;	
		nCond[nCondColumns] = pCondition->m_nSecondType;	
		nCondColumns++;
	}
	if(pCondition->m_nThirdType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strThirdAlias;	
		nCond[nCondColumns] = pCondition->m_nThirdType;		
		nCondColumns++;
	}
	if(pCondition->m_nFourthType != 0)
	{
		strAlias[nCondColumns] = pCondition->m_strFourthAlias;	
		nCond[nCondColumns] = pCondition->m_nFourthType;	
		nCondColumns++;
	}

	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}