// BO_CAT_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SEARCH.h"
#include "CommonDefine.h"

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

	
	if ( OpenMode == 0 ) //등록 대상 자료관리
	{
		GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("차수년도"));

	}
	if ( OpenMode == 1 ) //가원부 자료관리
	{
		GetDlgItem(IDC_REG_NO_DESC)->ShowWindow(FALSE);
		GetDlgItem(IDC_GROUP_NO1)->ShowWindow(FALSE);

		GetDlgItem(IDC_REGDATA_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_cobWorkingStatus)->ShowWindow(FALSE);
		GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("가원부년도"));

	}
	if (  OpenMode == 2) //가원부 자료관리
	{
		GetDlgItem(IDC_REG_NO_DESC)->ShowWindow(FALSE);
		GetDlgItem(IDC_GROUP_NO1)->ShowWindow(FALSE);

		GetDlgItem(IDC_REGDATA_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_cobWorkingStatus)->ShowWindow(FALSE);

		GetDlgItem(IDC_REGWORK_NO)->SetWindowText(_T("원부번호"));
		GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("원부년도"));

	}

	CESL_DataMgr *pDM;

	if ( OpenMode == 0 ) //등록자료관리
		pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	else if ( OpenMode == 1 ) // 가원부 자료 관리
		pDM = FindDM(_T("DM_단행_정리_가원부_메인"));
	else if ( OpenMode == 2 ) // 원부 대장 관리
		pDM = FindDM(_T("DM_단행_정리_원부_메인"));

	CString m_Year;
	CTime mTime = pDM->GetDBTime();
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도"), m_Year);


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
	
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,sACQUISIT_YEAR,sACQUISIT_TYPE;
	INT ids;

	CESL_DataMgr *pDM;
	if ( OpenMode == 0 ) //등록자료관리
		pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	else if ( OpenMode == 1 ) // 가원부 자료 관리
		pDM = FindDM(_T("DM_단행_정리_가원부_메인"));
	else if ( OpenMode == 2 ) // 원부 대장 관리
		pDM = FindDM(_T("DM_단행_정리_원부_메인"));

	//년도를 보여 준다.

	GetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	GetControlData(_T("CM_단행_등록_검색"), _T("BB_수입구분_코드")	  , sACQUISIT_TYPE , -1 ,1);

	// 차수 번호 와 가원부 번호를 메인에 보여 준다.
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작"), strWorkNo1);
	GetControlData(_T("CM_단행_등록_검색"), _T("차수번호끝"), strWorkNo2);
	if(  OpenMode == 2) // 원부 대장관리
	{
		GetControlData(_T("CM_단행_등록_검색"), _T("원부번호_시작"), strpreGroup1);
		strpreGroup2=_T("");
	}
	else
	{
		GetControlData(_T("CM_단행_등록_검색"), _T("가원부번호시작"), strpreGroup1);
		GetControlData(_T("CM_단행_등록_검색"), _T("가원부번호끝"), strpreGroup2);		 
	}

	if( (strWorkNo1 != _T(""))&&(strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}
	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	if ( OpenMode == 0 ) //등록 자료 관리 
	{
		SetControlData(_T("CM_단행_정리_등록_메인"), _T("차수번호"), strWorkNo1);
		SetControlData(_T("CM_단행_정리_등록_메인"), _T("가원부번호"), strpreGroup1);
		SetControlData(_T("CM_단행_정리_등록_메인"), _T("년도"), sACQUISIT_YEAR);
		SetControlData(_T("CM_단행_정리_등록_메인"), _T("입수구분"), sACQUISIT_TYPE);
	}
	else if ( OpenMode == 1 ) //가원부 관리 
	{
		SetControlData(_T("CM_단행_정리_가원부_메인"), _T("차수번호"), strWorkNo1);
		SetControlData(_T("CM_단행_정리_가원부_메인"), _T("가원부번호"), strpreGroup1);
		SetControlData(_T("CM_단행_정리_가원부_메인"), _T("년도"), sACQUISIT_YEAR);
		SetControlData(_T("CM_단행_정리_가원부_메인"), _T("입수구분"), sACQUISIT_TYPE);

	}
	else if ( OpenMode == 2 ) //원부대장 관리 
	{
		SetControlData(_T("CM_단행_정리_원부_메인"), _T("원부번호"), strWorkNo1);
		SetControlData(_T("CM_단행_정리_원부_메인"), _T("년도"), sACQUISIT_YEAR);
		SetControlData(_T("CM_단행_정리_원부_메인"), _T("입수구분"), sACQUISIT_TYPE);

	}




	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0)
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return;
	}


	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	if ( OpenMode == 0 ) //등록자료 관리
	{
		pCM = FindCM(_T("CM_단행_정리_등록_메인"));
		pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
		pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	}	
	else if ( OpenMode == 1 ) //가원부 자료 관리
	{
		pCM = FindCM(_T("CM_단행_정리_가원부_메인"));
		pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
		pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	}
	else if ( OpenMode == 2 ) //원부대장 관리
	{
		pCM = FindCM(_T("CM_단행_정리_원부_메인"));
		pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_원부_메인"), _T("MAIN_GRID"));
		pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	}

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
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//자료상태
	CString sWorkStatus;
	GetControlData( _T("CM_단행_등록_검색"), _T("자료상태"), sWorkStatus );

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	if (OpenMode == 0) //등록자료관리 
	{
		if ( sWorkStatus == _T("전체") ) {
			strQry = strQry + _T("  B.WORKING_STATUS >= 'BOR111N' AND B.WORKING_STATUS <= 'BOR112N'");
		}
		else if ( sWorkStatus == _T("등록인계자료") ) {
			strQry = strQry + _T(" B.WORKING_STATUS = 'BOR111N'");
		}
		else if ( sWorkStatus == _T("등록자료") ) {
			strQry = strQry + _T(" B.WORKING_STATUS = 'BOR112N'");
		}
	}
	else if (OpenMode == 1) // 가원부자료관리
	{
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR113O'");
	}
	else if (OpenMode == 2) //원부자료관리
	{
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR112N'");
	}

	//수입 년도 원부 조건 고려  
	GetControlData(_T("CM_단행_등록_검색"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();


	if (OpenMode == 2)
	{
		//차수번호
		GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작"), SEQ_NO1);
		GetControlData(_T("CM_단행_등록_검색"), _T("차수번호끝")  , SEQ_NO2);

		SEQ_NO1.TrimLeft();
		SEQ_NO1.TrimRight();
		SEQ_NO2.TrimLeft();
		SEQ_NO2.TrimRight();

		if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 != _T("")) )
		{
			strQry = strQry + _T(" AND a.accession_rec_no >= ") + SEQ_NO1 + _T(" AND a.accession_rec_no <= ") + SEQ_NO2 ;
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		else if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 == _T("")) )
		{
			strQry = strQry + _T(" AND a.accession_rec_no = ") + SEQ_NO1;
			sAccessionFlag = 1;
			
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		else if ( (SEQ_NO1 == _T(""))&&(SEQ_NO2 != _T("")) )
		{

			strQry = strQry + _T(" AND a.accession_rec_no = ") + SEQ_NO2;
			sAccessionFlag = 1;
			
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		


	}
	else
	{
		//차수번호
		GetControlData(_T("CM_단행_등록_검색"), _T("차수번호시작"), SEQ_NO1);
		GetControlData(_T("CM_단행_등록_검색"), _T("차수번호끝")  , SEQ_NO2);

		SEQ_NO1.TrimLeft();
		SEQ_NO1.TrimRight();
		SEQ_NO2.TrimLeft();
		SEQ_NO2.TrimRight();

		if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO1 + _T(" AND B.SEQ_NO <= ") + SEQ_NO2 ;

		}
		else if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO1;

		}
		else if ( (SEQ_NO1 == _T(""))&&(SEQ_NO2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO2;

		}


		//원부번호 
		GetControlData(_T("CM_단행_등록_검색"), _T("원부번호_시작")   , sAccessionNoStart); 
		sAccessionNoStart.TrimLeft();
		sAccessionNoStart.TrimRight();
		if (sAccessionNoStart != _T(""))
		{
			strQry = strQry + _T(" AND a.accession_rec_no = ")+sAccessionNoStart;
			sAccessionFlag = 1;
			
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		else
		{

			if (OpenMode == 1)
			{
				if ( sACQUISIT_YEAR != _T("") ) {
					strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
				}
			}
			else
			{
				if ( sACQUISIT_YEAR != _T("") ) {
					strQry = strQry + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
				}
			}

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
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}

	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
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

	//가원부 번호
	GetControlData(_T("CM_단행_등록_검색"), _T("가원부번호시작"), preGroup1);
	GetControlData(_T("CM_단행_등록_검색"), _T("가원부번호끝")  , preGroup2);

	if(preGroup1 != _T("") && preGroup2 != _T("")){

		strQry = strQry + _T(" AND a.accession_rec_no >= ")+preGroup1+_T(" AND a.accession_rec_no <= ")+preGroup2;


		if(sAccessionFlag == 1)
		{
			AfxMessageBox(_T("원부와 가원부는 통시에 검색 할수 없습니다!"));
			return _T("");
		}


	}
	if(preGroup1 != _T("") && preGroup2 == _T("")){

		strQry = strQry + _T(" AND a.accession_rec_no = ")+preGroup1;


		if(sAccessionFlag == 1)
		{
			AfxMessageBox(_T("원부와 가원부는 통시에 검색 할수 없습니다!"));
			return _T("");
		}


	}
	if(preGroup1 == _T("") && preGroup2 != _T("")){

		strQry = strQry + _T(" AND a.accession_rec_no = ")+preGroup2;


		if(sAccessionFlag == 1)
		{
			AfxMessageBox(_T("원부와 가원부는 통시에 검색 할수 없습니다!"));
			return _T("");
		}


	}


	//인계 처리일  

	GetControlData(_T("CM_단행_등록_검색"), _T("대상처리일_시작"), sTransferDataStart);
	GetControlData(_T("CM_단행_등록_검색"), _T("대상처리일_까지"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strQry = strQry + _T(" AND B.CAT_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("시작 인계처리일을 입력하세요.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strQry = strQry + _T(" AND B.CAT_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.CAT_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
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

						
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return _T("");
			}
			strQry = strQry + _T(" AND ") + sWhereTitle;
			
			/*
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;			
			*/
		}

		//저작자
		GetControlData(_T("CM_단행_등록_검색"), _T("저작자"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) {

						
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return _T("");
			}
			strQry = strQry + _T(" AND ") + sWhereTitle;
			
			/*
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;
			*/
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
