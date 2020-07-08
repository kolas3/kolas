// SE_CAT_SEARCH_MGR1.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_SEARCH_MGR_GROUP.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SEARCH_MGR_GROUP_GROUP dialog
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"

CSE_CAT_SEARCH_MGR_GROUP::CSE_CAT_SEARCH_MGR_GROUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_SEARCH_MGR_GROUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_SEARCH_MGR_GROUP::~CSE_CAT_SEARCH_MGR_GROUP()
{
}
VOID CSE_CAT_SEARCH_MGR_GROUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_SEARCH_MGR_GROUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_SEARCH_MGR_GROUP, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_SEARCH_MGR_GROUP)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_SEARCH_MGR_GROUP message handlers

BOOL CSE_CAT_SEARCH_MGR_GROUP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_등록_검색_매니저_원부")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_연속_정리_원부_메인"));

	CString m_Year;
	// 040304_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_수입년도"), m_Year);


	//간략 보기 
	OnbDetailNo(); 

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSE_CAT_SEARCH_MGR_GROUP::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CAT_SEARCH_MGR_GROUP::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,strYear;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_연속_정리_원부_메인"));

	// 차수 번호 와 가원부 번호를 메인에 보여 준다.
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("원부번호시작"), strpreGroup1);
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("원부번호끝"), strpreGroup2);		 

	GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_수입년도"), strYear);		 

	if( (strWorkNo1 != _T(""))&&(strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}
	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	SetControlData(_T("CM_연속_정리_원부_메인"), _T("원부번호"), strpreGroup1);
	SetControlData(_T("CM_연속_정리_원부_메인"), _T("원부년도"), strYear);


	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

	pDM->RefreshDataManager(strQry);

	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_연속_정리_원부_메인"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_연속_정리_원부_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//그리드 콜백 함수 연결 

	// 041006_HSS 검색결과 없음 메세지 추가
	INT nCount = pDM->GetRowCount();
	if (nCount == 0)
	{
		AfxMessageBox(_T("검색결과가 없습니다."));
		pCM->AllControlDisplay();
		return;
	}

	pCM->AllControlDisplay();

	if( nCount > 0 )
		ShowWindow(SW_HIDE);

}

INT CSE_CAT_SEARCH_MGR_GROUP::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_자료상태_코드")) {
            if ( str == _T("SER111N")) {
				str = _T("등록인계자료");
			}
			else if ( str == _T("SER112N") ) {
				str = _T("등록자료");
			}
			else if ( str == _T("SER113O") ) {
				str = _T("가원부자료");
			}
			else if ( str == _T("SEC111N") ) {
				str = _T("정리대상자료");
			}
			else if ( str == _T("SEC112O") ) {
				str = _T("열람인계대상");
			}
			else if ( str == _T("SEL211O") ) {
				str = _T("열람인계자료");
			}
			else if ( str == _T("SEL212N") ) {
				str = _T("배가자료");
			}
			else if ( str == _T("SEL213O") ) {
				str = _T("수리제본자료");
			}
			else if ( str == _T("SEL215O") ) {
				str = _T("제적자료");
			}
			else if ( str == _T("SEL214O") ) {
				str = _T("파손자료");
			}

        }
    }
	return 0;


EFS_END
return -1;

}

CString CSE_CAT_SEARCH_MGR_GROUP::MakeSelectQry()
{

	CString strQry,strTmp; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd,sAccessionNoYear;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//자료상태	
	strQry = strQry + _T(" B.WORKING_STATUS IN( ")
					  _T("'SER111N', 'SER112N', 'SER113O', ")
					  _T("'SEC111N', 'SEC112O', ")
					  //=====================================================
					  //2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 					  _T("'SEL211O', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O','SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O' ")
						// 18.09.27 JMJ KOL.RED.2018.008
					  _T("'SEL211O', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O','SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O' ")
					  //=====================================================
					  _T(") ");
	strQry = strQry + _T(" AND REG_NO IS NOT NULL ") ;

	//제본여부 
	CString sBindStatus;
	GetControlData( _T("CM_연속_등록_검색_원부"), _T("제본구분"), sBindStatus );
	if( sBindStatus ==_T("제본등록대상자료"))
	{
		strQry = strQry + _T(" AND B.BOOK_TYPE = 'B' ");

	}
	if( sBindStatus ==_T("낱권등록대상자료"))
	{
		strQry = strQry + _T(" AND B.BOOK_TYPE = 'C' ");

	}

	//원부 년도 
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_수입년도")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();
	
	//원부번호 
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("원부번호시작")   , sAccessionNoStart); 
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("원부번호끝")   , sAccessionNoEnd); 

	if ( (sAccessionNoStart != _T(""))&&(sAccessionNoEnd != _T("")) )
		strTmp = strTmp + _T(" AND a.ACCESSION_REC_NO >= ") + sAccessionNoStart + _T(" AND a.ACCESSION_REC_NO <= ") + sAccessionNoEnd ;
	
	else if ( sAccessionNoStart != _T("") )
		strTmp = strTmp + _T(" AND a.ACCESSION_REC_NO = ") + sAccessionNoStart;
	
	else if ( sAccessionNoEnd != _T("") )
		strTmp = strTmp + _T(" AND a.ACCESSION_REC_NO = ") + sAccessionNoEnd;
	
	if ( sACQUISIT_YEAR != _T("") ) {
		strTmp = strTmp + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
	}

	//수입 구분 코드 & 수입년도 
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("BB_수입구분_코드"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();

	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	//등록번호
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("BB_등록번호_시작"), sRegNoStart);
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("BB_등록번호_까지"), sRegNoEnd);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
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
	
	/// nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {

			sRegNoStart = pParentMgr->m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = pParentMgr->m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	

	if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {
		strTmp = strTmp + _T(" AND B.REG_NO = '") + sRegNoStart  + _T("'");
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
		AfxMessageBox( _T("시작 등록번호를 입력 하세요."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		strTmp = strTmp+_T(" AND B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("'");
	}


	//인계 처리일  
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("등록일자_시작"), sTransferDataStart);
	GetControlData(_T("CM_연속_등록_검색_원부"), _T("등록일자_까지"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("시작 등록처리일을 입력하세요.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.REG_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}

	// 040309_HSS 추가 
	strQry = strQry + _T(" AND B.BINDED_BOOK_YN = 'N' ");  

	//상세 검색일 경우 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		CMakeSearchData *pMakeData = NULL;
		pMakeData = new CMakeSearchData(this);

		//본표제  
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("본표제"), sTitle);	
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

			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return _T("");
			}
			
			strQry = strQry + _T(" AND ") + sWhereTitle;			
		}

		//저작자
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("저작자"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) {
			//sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );			
			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {			
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_PUBLISHER, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return _T("");
			}
			
			strQry = strQry + _T(" AND ") + sWhereTitle;
		}

		//코드 구분값
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_형식구분_코드"), sFORM_CODE);
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_매체구분_코드"), sMEDIA_CODE);
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_자료구분_코드"), sMAT_CODE);
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_이용대상구분_코드"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_관리구분_코드"), sMANAGE_CODE);

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
		GetControlData(_T("CM_연속_등록_검색_원부"), _T("BS_작업자"), sWorker);
		if ( sWorker != _T("") ) {
			strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	

		
	}
	
	if( strTmp == _T("") )
	{
		ESLAfxMessageBox(_T("자료가 많습니다. 조건을 걸어 주십시요!!"));
		return _T("");
	}

	strQry += strTmp;

	return strQry;

}

VOID CSE_CAT_SEARCH_MGR_GROUP::OnbDetailNo() 
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

VOID CSE_CAT_SEARCH_MGR_GROUP::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_연속_등록_검색_원부"));
	if (pCM)
		pCM->AllControlClear();		
}

HBRUSH CSE_CAT_SEARCH_MGR_GROUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
