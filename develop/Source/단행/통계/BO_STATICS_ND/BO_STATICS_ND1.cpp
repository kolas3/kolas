// BO_STATICS_ND1.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATICS_ND1.h"
#include "..\..\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_ND dialog


CBO_STATICS_ND::CBO_STATICS_ND(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_ND)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sExportFileName = _T("");
}
CBO_STATICS_ND::~CBO_STATICS_ND()
{
	
}

VOID CBO_STATICS_ND::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_ND)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_STATICS_ND, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_ND)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_ND message handlers

BOOL CBO_STATICS_ND::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_STATICS_ND::OnOK() 
{
	INT ids;
	
	CString sWhere;
	ids = MakeQuery( &sWhere );
	if ( ids != 0 ) return;
	
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계"))
	{
		ids = MakeStatics_ACQ( sWhere);
		if ( ids < 0 ) return;
	}
	else if ( m_sStaticsName == _T("교범통계") || m_sStaticsName == _T("교내발간자료통계") || 
			  m_sStaticsName == _T("비도서자료통계") )
	{
		ids = MakeStatics_ND( sWhere);
		if ( ids < 0 ) return;
	}
	else if ( m_sStaticsName == _T("자료류별통계") )
	{
		ids = MakeStatics_Class( sWhere);
		if ( ids < 0 ) return;
	}
	else if ( m_sStaticsName == _T("장서보유현황") )
	{
		ids = MakeStatics_ALL( sWhere);
		if ( ids < 0 ) return;
	}
	
	CButton* pbtnSaveFile;
	pbtnSaveFile = ( CButton* )GetDlgItem(IDC_chkExpFile);
	if ( pbtnSaveFile->GetCheck() )
	{
		SaveExcelFile();
	}

	ids = RunSIPreView();
	if ( ids < 0 ) return;
//	CDialog::OnOK();
}

VOID CBO_STATICS_ND::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-16
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 통계종류를 지정한다.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::SetStaticsName(CString sStaticsName)
{
	m_sStaticsName = sStaticsName;
	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-16
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 입력값을 통해 조건절을 만든다.
//
// [  PARAMETERS  ]
//    psGetWhere  : 리턴조건절     
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 컨트롤의 값을 구한다.
// 2. 입력값으로 조건절을 만든다.
// 3. 리턴조건절.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeQuery(CString *psGetWhere)
{
	INT ids;
	CString sErrMsg;

	//-----------------------------------------------------------------------------
	// 1. 컨트롤의 값을 구한다. 
	//-----------------------------------------------------------------------------
	CString sCMAlias = _T("CM_STATICS_ND");
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if ( pCM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 0 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	const INT nDataCnt = 5;
	CString sDataAlias[nDataCnt] = { _T("배가부서"), _T("배가일1") , _T("배가일2") , _T("변경일1"), _T("변경일2") };
	
	CESL_Control_Element *pCE;
	CString sShelfCode;							// 배가부서
	CString sStartRegDate;						// 시작배가일
	CString sEndRegDate;						// 끝배가일
	CString sStartChangeDate;					// 시작변경일
	CString sEndChangeDate;						// 끝변경일
	
	
	// 배가부서
	ids = pCM->GetControlMgrData( sDataAlias[0], sShelfCode, -1, 0 );
	if ( ids < 0 )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 1 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	// 시작배가일
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[1] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 2 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sStartRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartRegDate.TrimLeft(); sStartRegDate.TrimRight();

	// 끝배가일
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[2] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 3 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sEndRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sEndRegDate.TrimLeft(); sEndRegDate.TrimRight();
    
	// 시작변경일
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[3] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 4 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sStartChangeDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartChangeDate.TrimLeft(); sStartChangeDate.TrimRight();

	// 끝변경일
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[4] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 5 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sStartChangeDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartChangeDate.TrimLeft(); sStartChangeDate.TrimRight();

	// 2005-12-15 REM BY PDW(Jr) ==================================================
	// 요청사항
	/*
	// 배가일 조건은 입력되어야한다.
	// 2005-11- 19 WORK UPD 통계 종류에 따라 체크유무
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계") )
	{
		if ( sStartRegDate == _T("") || sEndRegDate == _T("") ) 
		{
			AfxMessageBox( _T("배가일이 입력되지 않았습니다.") );
			return 1 ;
		}
	}
	*/
	// ============================================================================

	//-----------------------------------------------------------------------------
	// 2. 입력값으로 조건절을 만든다.
	//-----------------------------------------------------------------------------
	CString sWhere;                      // 조건절
	CString sTemp;                       // 임시변수 
	sWhere.Empty();

	// 배가부서 조건 
	if ( !sShelfCode.IsEmpty() )
	{
		if ( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		sTemp.Format( _T(" B.SHELF_LOC_CODE = %s ") , sShelfCode );
		sWhere += sTemp;
	}

	// 2005-12-15 UDP BY PDW(Jr) ==================================================
	// 체크를 위에서 안한다.
	/* 수정전 
	// 배가일 조건 
	// 배가일유무는 위에서 확인한다.
	// 2005-11-21 WORK UPD
	// 변경일 조건 추가
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계") )
	{
		if ( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		sTemp.Format( _T(" B.SHELF_DATE BETWEEN '%s' AND '%s'") ,sStartRegDate ,sEndRegDate  );
		sWhere += sTemp;

		if ( !sStartChangeDate.IsEmpty() )
		{
			sTemp.Format( _T(" AND B.SHELF_CHANGE_DATE BETWEEN '%s' AND '%s'") ,sStartChangeDate ,sEndChangeDate  );
			sWhere += sTemp;
		}

	}
	*/
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계") )
	{
		if ( !sStartRegDate.IsEmpty() && sEndRegDate.IsEmpty() )
			sTemp.Format( _T(" B.SHELF_DATE BETWEEN '%s' AND '%s'") , sStartRegDate , sStartRegDate  );
		else if ( !sStartRegDate.IsEmpty() && !sEndRegDate.IsEmpty() )
			sTemp.Format( _T(" B.SHELF_DATE BETWEEN '%s' AND '%s'") ,sStartRegDate ,sEndRegDate  );
		else if ( sStartRegDate.IsEmpty() && !sEndRegDate.IsEmpty() )
		{
			AfxMessageBox( _T("배가일 입력이 잘못되어 있습니다.") );
			return 1 ;
		}

		if ( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		
		sWhere += sTemp;


		if ( !sStartChangeDate.IsEmpty() && sEndChangeDate.IsEmpty() )
		{
			sTemp.Format( _T(" AND B.SHELF_CHANGE_DATE BETWEEN '%s' AND '%s'") ,sStartChangeDate ,sStartChangeDate  );
			sWhere += sTemp;
		}
		else if ( !sStartChangeDate.IsEmpty() && !sEndChangeDate.IsEmpty() )
		{
			sTemp.Format( _T(" AND B.SHELF_CHANGE_DATE BETWEEN '%s' AND '%s'") ,sStartChangeDate ,sEndChangeDate  );
			sWhere += sTemp;
		}

	}
	// ============================================================================

	//-----------------------------------------------------------------------------
	// 3. 리턴 조건절
	//-----------------------------------------------------------------------------
	if ( !sWhere.IsEmpty() )
		sWhere = _T(" AND ") + sWhere;

	*psGetWhere = sWhere;
		
	return 0;
}

BOOL CBO_STATICS_ND::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	CString sErrMsg;

	ids = InitESL_Mgr( _T("BO_STATICS_ND") );
	if ( ids < 0 ) 
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::OnInitDialog Error") );
		AfxMessageBox ( sErrMsg );
		return FALSE;
	}

	GetDlgItem(IDC_eStaticsNDTitle)->SetWindowText(m_sStaticsName);
	 
	GetDlgItem(IDC_datREG_DATE1_ND)->EnableWindow( FALSE );
	GetDlgItem(IDC_datREG_DATE2_ND)->EnableWindow( FALSE );
	GetDlgItem(IDC_datCHANGE_DATE1_ND)->EnableWindow( FALSE );
	GetDlgItem(IDC_datCHANGE_DATE2_ND)->EnableWindow( FALSE );

	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계") )
	{
		GetDlgItem(IDC_datREG_DATE1_ND)->EnableWindow();
		GetDlgItem(IDC_datREG_DATE2_ND)->EnableWindow();
		GetDlgItem(IDC_datCHANGE_DATE1_ND)->EnableWindow();
		GetDlgItem(IDC_datCHANGE_DATE2_ND)->EnableWindow();
	}

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 수입구분별 통계, 관리구분별 통계
//
// [  PARAMETERS  ]
//    sWhere  : 조건절     
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 통계DM 초기화
// 2. 기본통계 정보 구성
// 3. 단행 통계 구성
// 4. 연속 통계 구성
// 5. 합계 구성
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_ACQ(CString sWhere)
{
    INT ids;
	CString sErrMsg;
	
	//-----------------------------------------------------------------------------
	// 1. 통계DM 초기화
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_수입관리구분별통계_국방대");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// 출력용 DM은 형식이 고정되어있다.
	const INT nResultRow = 18;
	const INT nResultCol = 4;
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0 / 0"));		
	}

	// 통계 쿼리용 DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	
	
	//-----------------------------------------------------------------------------
	// 2. 기본 쿼리 정보 구성
	//-----------------------------------------------------------------------------
	CString sSelect;
	CString sTableName;
	CString sBaseWhere;
	CString sGroupBy;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;
	CString sQuery;
	
	// 수입통계 관리구분 통계가 달라진다.
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") )
	{
		sSelect.Format ( _T("SELECT B.REG_CODE, B.ACQ_CODE ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY ") );
		sGroupBy.Format( _T("Group By B.REG_CODE , B.ACQ_CODE") );
	}
	else if ( m_sStaticsName == _T("관리구분별통계") )
	{
		sSelect.Format ( _T("SELECT B.REG_CODE, B.MANAGE_CODE ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY ") );
		sGroupBy.Format( _T("Group By B.REG_CODE , B.MANAGE_CODE") );
	}


	// 17/07/20 김혜영 : 책나래대출자료 상태 추가 
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );
// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
	// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL611O') ") );
//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );
	//=====================================================
*///AFTER------------------------------------------------------------------------------
	// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/	
	// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O', 'SEL511O' ) ") );


	//-----------------------------------------------------------------------------
	// 3. 단행 통계 구성
	//-----------------------------------------------------------------------------	
	sTableName = _T("BO_SPECIES_TBL S , BO_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")
				  		_T(" AND B.BOOK_APPENDIX_FLAG = 'B' %s ")
				  , sSelect, sTableName, sBaseWhere, sWhere);
	sQuery += sBoWorkingStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 0 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. 연속 통계 구성
	//-----------------------------------------------------------------------------	
	DM.FreeData();
	sTableName = _T("SE_SPECIES_TBL S , SE_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY) , COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s %s ")				  
				  , sSelect, sTableName, sBaseWhere, sWhere );
	sQuery += sSeWorkinStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 4 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 1 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 5 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	//-----------------------------------------------------------------------------
	// 5. 합계 구성
	//-----------------------------------------------------------------------------	
	// 가로 통계 구하기 
	UpdateRowSum( pDM_Dst, 0, 2, 3 );
	// 세로 통계 구하기 
	UpdateColSum( pDM_Dst, 0, 2, 3 );
	UpdateColSum( pDM_Dst, 4, 10, 11 );
	UpdateColSum( pDM_Dst, 12, 15, 16 );
	// 총계 구하기 
	// 더할 로우 3, 11, 16 -> 17
	UpdateColSum( pDM_Dst, 3, 3, 17 );
	UpdateColSum( pDM_Dst, 11, 11, 17 );
	UpdateColSum( pDM_Dst, 16, 16, 17 );
 

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// ScrDM에서 해당 등록구분에 한해서 pDstDM의 특정 Cell로 넣는다.
// 수입구분별통계(국방대), 관리구분별 , 자료류별 통계에서 사용된다.
//
// [  PARAMETERS  ]
//    pSrcDM  : 쿼리로 나온 결과 DM     
//    pDstDM  : 출력할 DM
//    nMode   : 단행 연속 구분 코드 ( 0 : 단행 , 1 : 연속 )
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. idx = 0의 정보( 등록구분 )로 입력될 위치가 바뀐다.
// 2. idx = 1의 정보( 수입구분 )로 입력될 위치가 바뀐다.
// 3. idx = 2,3의 정보가 입력될 통계 값이다.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::CopySrcToDst1(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM, INT nMode)
{
	INT ids;
	CString sErrMsg;

	CString sRegCode;								// 등록구분
	CString sData;								    // 저장 변수
    CString sInsertData;                            // 입력값 
	INT nInsertRowIdx;                              // 입력시킬 로우의 위치
	INT nInsertColIdx;								// 입력시킬 컬럼의 위치
	INT nRowCount = pSrcDM->GetRowCount();
	for ( INT i = 0; i < nRowCount;i++ )
	{
		ids = pSrcDM->GetCellData( i, 0, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 1 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		//-----------------------------------------------------------------------------
		// 1. inx = 0의 셀위치의 정보( 등록구분 )로 입력될 위치가 바뀐다.
		//-----------------------------------------------------------------------------	
		if ( sData == _T("EM") )		nInsertRowIdx = 0;
		else if ( sData == _T("JM") )   nInsertRowIdx = 1;		
		else if ( sData == _T("WM") )   nInsertRowIdx = 2;
		else if ( sData == _T("KN") )   nInsertRowIdx = 4;
		else if ( sData == _T("DJ") || sData == _T("DM") || sData == _T("DS") ) nInsertRowIdx = 5;
		else if ( sData == _T("MI") )   nInsertRowIdx = 6;
		else if ( sData == _T("MM") )   nInsertRowIdx = 7;
		else if ( sData == _T("AA") )   nInsertRowIdx = 8;
		else if ( sData == _T("EE") )   nInsertRowIdx = 9;
		else if ( sData == _T("ES") )   nInsertRowIdx = 12;
		else if ( sData == _T("JS") )   nInsertRowIdx = 13;
		else if ( sData == _T("WS") )	nInsertRowIdx = 14;
		else
		{
			if ( nMode == 0 )
				nInsertRowIdx = 10;
			else if ( nMode == 1 )
				nInsertRowIdx = 15;
		}

		//-----------------------------------------------------------------------------
		// 2. inx = 1의 셀위치의 정보( 수입,관리,분류기호 구분 )로 입력될 위치가 바뀐다.
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 1, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 2 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		if ( m_sStaticsName == _T("수입구분별통계(국방대)") )
		{
			if ( sData == _T("1") )
				nInsertColIdx = 0;
			else if ( sData == _T("2") )
				nInsertColIdx = 1;
			else
				nInsertColIdx = 2;
		}
		else if ( m_sStaticsName == _T("관리구분별통계") )
		{
			if ( sData == _T("MA") )
				nInsertColIdx = 0;
			else if ( sData == _T("BR") )
				nInsertColIdx = 1;
			else
				nInsertColIdx = 2;			
		}
		else if ( m_sStaticsName == _T("자료류별통계") )
		{
			if ( !sData.IsEmpty() )
				nInsertColIdx = _ttoi( sData );
			else
				nInsertColIdx = 10;
		}	

		//-----------------------------------------------------------------------------
		// 3. idx = 2,3의 정보가 입력될 갯수다
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 2, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		sInsertData = sData;

		ids = pSrcDM->GetCellData( i, 3, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		// 이전 데이터에 구한 결과를 합친다.
		CString sSpecies;
		CString sBook;
		CString sPreData;

		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}		

		GetDivValue( sPreData, &sSpecies, &sBook );

		sInsertData.Format( _T("%d"), _ttoi( sSpecies ) + _ttoi( sInsertData ) );
		sData.Format( _T("%d"), _ttoi( sBook ) + _ttoi( sData ) );
		sInsertData = sInsertData + _T(" / ") + sData;		
	
		// 입력
		ids = pDstDM->SetCellData( nInsertRowIdx, nInsertColIdx, sInsertData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// _T(" 종 / 권 ")의 정보를 분리한다.
//
// [  PARAMETERS  ]
//    sPreData  : 대상 데이터
//    psSpecies : 종정보
//    psBook    : 책정보
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::GetDivValue(CString sPreData, CString *psSpecies, CString *psBook)
{
	sPreData.Replace( _T(" "), _T("") );

	INT nPos = sPreData.FindOneOf( _T("/") );

	if ( nPos < 0 )
	{
		psSpecies->Format( _T("%s"), sPreData );
		psBook->Format( _T("%s"), _T("0") );
	}
	else
	{
		psSpecies->Format( _T("%s"), sPreData.Mid(0, nPos) );
		psBook->Format( _T("%s"), sPreData.Mid(nPos + 1) );
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 통계 결과를 SIReport로 보여준다.
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 해당 DM 찾기 , 해당 출력물종류 결정
// 2. SIReport 초기화 
// 3. SIReport 변수 입력
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::RunSIPreView()
{
	INT ids;
	CString sErrMsg;

	//-----------------------------------------------------------------------------
	//1. 해당 DM 찾기 , 해당 출력물종류 결정
	//-----------------------------------------------------------------------------
	CESL_DataMgr *pDM_Dst;
	CString sSIReportAlias;

	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계") )
	{
		sSIReportAlias = _T("수입관리구분별통계_국방대");
		pDM_Dst = FindDM ( _T("DM_수입관리구분별통계_국방대") );	
	}	
	else if ( m_sStaticsName == _T("장서보유현황") )
	{
		sSIReportAlias = _T("장서보유현황_국방대");
		pDM_Dst = FindDM ( _T("DM_장서보유현황_국방대") );	
	}
	else if ( m_sStaticsName == _T("교내발간자료통계") )
	{
		sSIReportAlias = _T("교내발간자료통계_국방대");
		pDM_Dst = FindDM ( _T("DM_교범비도서발간자료통계_국방대") );
	}
	else if ( m_sStaticsName == _T("교범통계") )
	{
		sSIReportAlias = _T("교범통계_국방대");
		pDM_Dst = FindDM ( _T("DM_교범비도서발간자료통계_국방대") );
	}
	else if ( m_sStaticsName == _T("비도서자료통계") )
	{
		sSIReportAlias = _T("비도서자료통계_국방대");
		pDM_Dst = FindDM ( _T("DM_교범비도서발간자료통계_국방대") );
	}
	else if ( m_sStaticsName == _T("자료류별통계") )
	{
		sSIReportAlias = _T("자료류별통계_국방대");
		pDM_Dst = FindDM ( _T("DM_자료류별통계_국방대") );
	}

	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	//2. SIReport 초기화 
	//-----------------------------------------------------------------------------
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
//	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) 
	{
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return -1;
	}

	pSIReportManager->SetCONNECTION_INFO(pDM_Dst->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();
	
	ids = pSIReportManager->GetSIReportFileFromDB( sSIReportAlias );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	//3. SIReport 변수입력
	//-----------------------------------------------------------------------------
	// 제목
	CString strStaticsTitle;
	GetDlgItem(IDC_eStaticsNDTitle)->GetWindowText(strStaticsTitle);
	pSIReportManager->SetTempVariable(_T("통계제목"), strStaticsTitle);

	CString sCMAlias = _T("CM_STATICS_ND");
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if ( pCM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 3 ERROR !!") );
		AfxMessageBox( sErrMsg );
		delete pSIReportManager;
		return -1;
	}

	
	CESL_Control_Element *pCE;
	CString sData;
	CString sStartRegDate;
	CString sEndRegDate;

	// 배가일1
	pCE = (CESL_Control_Element*)pCM->FindControlElement( _T("배가일1") );
	if ( pCE == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 4 ERROR !!") );
		AfxMessageBox( sErrMsg );
		delete pSIReportManager;
		return -1;
	}

	sStartRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartRegDate.TrimLeft(); sStartRegDate.TrimRight();

	// 배가일2
	pCE = (CESL_Control_Element*)pCM->FindControlElement( _T("배가일2") );
	if ( pCE == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 5 ERROR !!") );
		AfxMessageBox( sErrMsg );
		delete pSIReportManager;
		return -1;
	}
	
	sEndRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sEndRegDate.TrimLeft(); sEndRegDate.TrimRight();

	sData.Format( _T("%s ~ %s"), sStartRegDate, sEndRegDate );
	pSIReportManager->SetTempVariable(_T("배가일조건"), sData);
	
	// 통계구분
	CString sAlias1;
	CString sAlias2;
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") )
	{
		sAlias1 = _T("구입");
		sAlias2 = _T("기증");
	}
	else if ( m_sStaticsName == _T("관리구분별통계") )
	{
		sAlias1 = _T("본관");
		sAlias2 = _T("분관");
	}
	else
	{
		sAlias1 = _T("1");
		sAlias2 = _T("2");
	}

	pSIReportManager->SetTempVariable(_T("통계구분1"), sAlias1);
	pSIReportManager->SetTempVariable(_T("통계구분2"), sAlias2);

	// 출력일
	CTime tCur = CTime::GetCurrentTime();
	CString sCurTime = tCur.Format( _T("%Y년%m월 현재") );
	pSIReportManager->SetTempVariable(_T("출력일"), sCurTime);
	pSIReportManager->SetDataMgr(0, pDM_Dst );

	pSIReportManager->MakeXMLPageFiles();
//	Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;
}

INT CBO_STATICS_ND::Print_SetLastPageLine()
{
	INT ids;

	// XML 과 WorkForm객체를 생성한다.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML 처리기 생성에 실패하였습니다."));
		return -1;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("중간 XML 처리기 생성에 실패하였습니다."));
		return -1;
	}

	// 현재 마지막 페이지 번호를 가져온다.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("페이지 정보를 찾을 수 없습니다."));
		return -1;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// 현재 마지막 페이지 파일을 불러온다.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("페이지를 로드 하지 못하였습니다."));
		return -1;
	}
	// 불러온 WorkForm중에서 First Sheet를 가져온다.(중간 페이지 파일에는 Sheet가 한개뿐이다.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("페이지를 찾을 수 없습니다."));
		return -1;
	}
	// 마지막 수평줄 처리가 이미 되어 있는지 본다. 이미 처리되어 있다면 두번 할 필요 없다.
	CSIReportObject * pObject = pSheet->FindObject(_T("추가된마지막수평줄"));
	if (pObject) return -1;
	// 외부테이블 과 반복 테이블을 가져온다.(테이블 이름은 상황에 따라 변경할 것)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_7"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_37"));
	if (pIn == NULL) return -1;
	if (pOut == NULL) return -1;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// 반복테이블의 마지막 라인의 왼쪽 처음과 오른쪽 마지막셀의 Rect를 구하고
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// 외부테이블의 마지막 라인의 왼쪽 처음 셀의 Rect를 구한다.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// 외부테이블보다 반복테이블이 작으면 마지막 수평줄을 추가해준다.
	if (InLeft.bottom < OutLeft.bottom) {
		// 마지막 수평줄의 Rect를 계산한다.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// 마지막 수평줄을 중간 페이지 파일에 추가한다.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("추가된마지막수평줄"));
			pLine->SetRect(NewRect);
		}

		// 변경된 중간페이지 파일을 저장한다.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// 생성된 Object를 해제
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return 0 ;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 비도서자료통계, 교내발간자료 통계, 교범통계
//
// [  PARAMETERS  ]
//    sWhere  : 조건절     
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 통계DM 초기화
// 2. 기본통계 정보 구성
// 3. 단행 통계 구성
// 4. 연속 통계 구성
// 5. 합계 구성
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_ND(CString sWhere)
{
	INT ids;
	CString sErrMsg;
	
	//-----------------------------------------------------------------------------
	// 1. 통계DM 초기화
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_교범비도서발간자료통계_국방대");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// 출력용 DM은 형식이 고정되어있다.
	INT nRow = 0;
	if ( m_sStaticsName == _T("교범통계") )
		nRow = 15;
	else if ( m_sStaticsName == _T("교내발간자료통계") )
		nRow = 12;
	else if ( m_sStaticsName == _T("비도서자료통계") )
		nRow = 12;

	const INT nResultRow = nRow;
	const INT nResultCol = 9;

	
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0 / 0"));		
	}

	// 통계 쿼리용 DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	

	//-----------------------------------------------------------------------------
	// 2. 기본통계 정보 구성
	//-----------------------------------------------------------------------------	
	CString sSelect;
	CString sTableName;
	CString sBaseWhere;
	CString sGroupBy;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;
	CString sQuery;
	
	if ( m_sStaticsName == _T("교범통계") )
	{
		sSelect.Format ( _T("SELECT S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY AND B.REG_CODE = 'MM' ")  );
		sGroupBy.Format( _T("Group By S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS") );
	}
	else if ( m_sStaticsName == _T("교내발간자료통계") )
	{
		sSelect.Format ( _T("SELECT S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY AND B.REG_CODE = 'KN' ") );
		sGroupBy.Format( _T("Group By S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS") );
	}
	else if ( m_sStaticsName == _T("비도서자료통계") )
	{
		sSelect.Format ( _T("SELECT B.MEDIA_CODE , B.MANAGE_CODE , B.WORKING_STATUS ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY AND B.REG_CODE = 'AA' ") );
		sGroupBy.Format( _T("Group By B.MEDIA_CODE , B.MANAGE_CODE , B.WORKING_STATUS") );
	}

	// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );
// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
	// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL611O') ") );	
//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );
	//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );	
/*//END-------------------------------------------------------------------------------*/	
// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O', 'SEL511O' ) ") );

	//-----------------------------------------------------------------------------
	// 3. 단행 통계 구성
	//-----------------------------------------------------------------------------	
	sTableName = _T("BO_SPECIES_TBL S , BO_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")
				  		_T(" AND B.BOOK_APPENDIX_FLAG = 'B' ") 						
				  , sSelect, sTableName, sBaseWhere );

	sQuery += sWhere + sBoWorkingStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst3( &DM, pDM_Dst );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. 연속 통계 구성
	// 2005-11-21 UPD BY PDW
	// 비도서 자료는 제외한다.
	//-----------------------------------------------------------------------------	
	if ( m_sStaticsName != _T("비도서자료통계") )
	{
		DM.FreeData();
		sTableName = _T("SE_SPECIES_TBL S , SE_BOOK_TBL B ");
		sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY) , COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")				  		  
					  , sSelect, sTableName, sBaseWhere );
		sQuery += sWhere + sSeWorkinStatus + sGroupBy;
		
		ids = DM.RestructDataManager( sQuery );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		ids = CopySrcToDst3( &DM, pDM_Dst );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// 5. 합계 구성
	//-----------------------------------------------------------------------------	
	// 가로 합계 구하기 
	UpdateRowSum( pDM_Dst, 0, 2, 3 );
	UpdateRowSum( pDM_Dst, 4, 6, 7 );
	UpdateRowSum( pDM_Dst, 3, 3, 8 );	
	UpdateRowSum( pDM_Dst, 7, 7, 8 );
	// 세로 합계 구하기 
	UpdateColSum( pDM_Dst, 0, nResultRow - 3, nResultRow - 2 );
	// 비율 구하기
	CString sTemp;
	CString sSpeices;
	CString sBook;
	ids = pDM_Dst->GetCellData( nResultRow - 2, nResultCol - 1 , sTemp );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 6 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	GetDivValue( sTemp, &sSpeices, &sBook );

	UpdatePercentage(pDM_Dst, nResultRow - 2, nResultRow - 1, _ttoi(sBook), 0 ); 


	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// DM의 시작컬럼부터 끝컬럼까지 가로 합계를 타겟컬럼에 입력한다.
//
// [  PARAMETERS  ]
//    pDM       : 합계 적용 DM
//    nStartCol : 시작 컬럼 idx
//    nEndCol   : 끝 컬럼 idx
//    nTagetCol : 적용 컬럼 idx
//    nMode     : 0 -> 0/0    1 -> 0 형태로 
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::UpdateRowSum(CESL_DataMgr *pDM, INT nStartCol, INT nEndCol, INT nTagetCol, INT nMode)
{
	INT ids;
	CString sErrMsg;

	if ( pDM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	CString sData;
	CString sPreData;
	CString sBookCnt;
	CString sSpeciesCnt;
	INT nBookCnt;
	INT nSpeciesCnt;
	INT nRowCnt = pDM->GetRowCount();

	for ( INT i = 0;i < nRowCnt;i++ )
	{
		nBookCnt = 0;
		nSpeciesCnt = 0;

		for ( INT j = nStartCol;j < nEndCol + 1;j++ )
		{
			ids = pDM->GetCellData( i, j, sData );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 2 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			GetDivValue( sData, &sSpeciesCnt, &sBookCnt );

			nBookCnt += _ttoi( sBookCnt );
			nSpeciesCnt += _ttoi( sSpeciesCnt );
		}

		// 이전 데이터에 더한다.
		ids = pDM->GetCellData( i, nTagetCol, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		GetDivValue( sPreData, &sSpeciesCnt, &sBookCnt );

		nBookCnt += _ttoi( sBookCnt );
		nSpeciesCnt += _ttoi( sSpeciesCnt );

		if ( nMode == 0 )
			sData.Format( _T("%d / %d"), nSpeciesCnt, nBookCnt );
		else if ( nMode == 1 )
			sData.Format( _T("%d"), nSpeciesCnt );
		ids = pDM->SetCellData( i, nTagetCol, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// DM의 시작로우부터 끝로우까지 세로 합계를 타겟로우에 입력한다.
//
// [  PARAMETERS  ]
//    pDM       : 합계 적용 DM
//    nStartRow : 시작 로우 idx
//    nEndRow   : 끝 로우 idx
//    nTagetRow : 적용 로우 idx
//    nMode     : 0 -> 0/0    1 -> 0 형태로 
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::UpdateColSum(CESL_DataMgr *pDM, INT nStartRow, INT nEndRow, INT nTagetRow, INT nMode)
{
	INT ids;
	CString sErrMsg;

	if ( pDM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::UpdateColSum - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	CString sData;
	CString sPreData;
	CString sBookCnt;
	CString sSpeciesCnt;
	INT nBookCnt;
	INT nSpeciesCnt;
	INT nColCnt = pDM->GetColCount();

	for ( INT i = 0;i < nColCnt;i++ )
	{
		nBookCnt = 0;
		nSpeciesCnt = 0;

		for ( INT j = nStartRow;j < nEndRow + 1;j++ )
		{
			ids = pDM->GetCellData( j, i, sData );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdateColSum - 2 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			GetDivValue( sData, &sSpeciesCnt, &sBookCnt );

			nBookCnt += _ttoi( sBookCnt );
			nSpeciesCnt += _ttoi( sSpeciesCnt );
		}

		// 이전 데이터에 더한다.
		ids = pDM->GetCellData( nTagetRow, i, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		GetDivValue( sPreData, &sSpeciesCnt, &sBookCnt );

		nBookCnt += _ttoi( sBookCnt );
		nSpeciesCnt += _ttoi( sSpeciesCnt );

		if ( nMode == 0 )
			sData.Format( _T("%d / %d"), nSpeciesCnt, nBookCnt );
		else if ( nMode == 1 )
			sData.Format( _T("%d"), nSpeciesCnt );

		ids = pDM->SetCellData( nTagetRow, i, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateColSum - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 장서 보유 현황 통계
//
// [  PARAMETERS  ]
//    sWhere  : 조건절     
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 통계DM 초기화
// 2. 기본통계 정보 구성
// 3. 단행 통계 구성
// 4. 연속 통계 구성
// 5. 합계 구성
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_ALL(CString sWhere)
{
	INT ids;
	CString sErrMsg;

	//-----------------------------------------------------------------------------
	// 1. 통계DM 초기화
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_장서보유현황_국방대");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ALL - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// 출력용 DM은 형식이 고정되어있다.
	const INT nResultRow = 16;
	const INT nResultCol = 4;
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0"));		
	}

	// 통계 쿼리용 DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	
	
	//-----------------------------------------------------------------------------
	// 2. 기본 쿼리 정보 구성
	//-----------------------------------------------------------------------------
	CString sSelect;
	CString sQuery;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;

	// 17/07/20 김혜영 : 책나래대출자료 상태 추가 
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );
// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );

	// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL611O') ") );

//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );	
	//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/	
// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O' , 'SEL511O' ) ") );
	
	//-----------------------------------------------------------------------------
	// 3. 단행 통계 구성
	//-----------------------------------------------------------------------------	
	sQuery.Format(_T("SELECT B.REG_CODE, B.MANAGE_CODE , COUNT(DISTINCT B.REC_KEY) ")
		          _T("FROM BO_SPECIES_TBL S , BO_BOOK_TBL B ")
				  _T("WHERE B.SPECIES_KEY=S.REC_KEY ")
				   _T(" AND B.BOOK_APPENDIX_FLAG = 'B' %s %s ") 
				   _T(" Group By B.REG_CODE , B.MANAGE_CODE "), sWhere , sBoWorkingStatus);
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst2( &DM, pDM_Dst);
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. 연속 통계 구성
	//-----------------------------------------------------------------------------	
	DM.FreeData();
	sQuery.Format(_T("SELECT B.REG_CODE, B.MANAGE_CODE , COUNT(DISTINCT B.REC_KEY) ")
		          _T("FROM SE_SPECIES_TBL S , SE_BOOK_TBL B ")
				  _T("WHERE B.SPECIES_KEY=S.REC_KEY ")
				   _T(" %s %s ") 
				   _T(" Group By B.REG_CODE , B.MANAGE_CODE "), sWhere , sSeWorkinStatus);
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 4 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst2( &DM, pDM_Dst );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 5 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 5. 합계 구성
	//-----------------------------------------------------------------------------
	// 가로통계
	UpdateRowSum(pDM_Dst, 0, 2, 3, 1);
	// 세로통계 
	UpdateColSum(pDM_Dst, 0, 2, 3, 1);
	UpdateColSum(pDM_Dst, 4, 6, 7, 1);
	UpdateColSum(pDM_Dst, 10, 14, 15, 1);
	UpdateColSum(pDM_Dst, 3, 3, 15, 1);
	UpdateColSum(pDM_Dst, 7, 7, 15, 1);
	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// ScrDM에서 해당 등록구분에 한해서 pDstDM의 특정 Cell로 넣는다.
// 장서보유 현황통계에서 사용된다.
//
// [  PARAMETERS  ]
//    pSrcDM  : 쿼리로 나온 결과 DM     
//    pDstDM  : 출력할 DM
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. idx = 0의 정보( 등록구분 )로 입력될 위치가 바뀐다.
// 2. idx = 1의 정보( 수입구분 )로 입력될 위치가 바뀐다.
// 3. idx = 2의 정보가 입력될 통계 값이다.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::CopySrcToDst2(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM)
{
	INT ids;
	CString sErrMsg;

	CString sRegCode;								// 등록구분
	CString sData;								    // 저장 변수
    CString sInsertData;                            // 입력값 
	INT nInsertRowIdx;                              // 입력시킬 로우의 위치
	INT nInsertColIdx;								// 입력시킬 컬럼의 위치
	INT nRowCount = pSrcDM->GetRowCount();
	for ( INT i = 0; i < nRowCount;i++ )
	{
		ids = pSrcDM->GetCellData( i, 0, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 1 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		//-----------------------------------------------------------------------------
		// 1. inx = 0의 셀위치의 정보( 등록구분 )로 입력될 위치가 바뀐다.
		//-----------------------------------------------------------------------------	
		if ( sData == _T("EM") )		nInsertRowIdx = 0;
		else if ( sData == _T("JM") )   nInsertRowIdx = 1;		
		else if ( sData == _T("WM") )   nInsertRowIdx = 2;
		else if ( sData == _T("KN") )   nInsertRowIdx = 10;
		else if ( sData == _T("DJ") || sData == _T("DM") || sData == _T("DS") ) nInsertRowIdx = 12;
		else if ( sData == _T("MI") )   nInsertRowIdx = 13;
		else if ( sData == _T("MM") )   nInsertRowIdx = 11;
		else if ( sData == _T("ES") )   nInsertRowIdx = 4;
		else if ( sData == _T("JS") )   nInsertRowIdx = 5;
		else if ( sData == _T("WS") )	nInsertRowIdx = 6;
		else						nInsertRowIdx = 14;
		
		//-----------------------------------------------------------------------------
		// 2. inx = 1의 셀위치의 정보( 관리 구분 )로 입력될 위치가 바뀐다.
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 1, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 2 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		if ( sData == _T("MA") )
			nInsertColIdx = 0;
		else if ( sData == _T("BR") )
			nInsertColIdx = 1;
		else
			nInsertColIdx = 2;			
		
		//-----------------------------------------------------------------------------
		// 3. idx = 2의 정보가 입력될 갯수다
		//-----------------------------------------------------------------------------	
		CString sCurBook;
		CString sCurSpecies;

		ids = pSrcDM->GetCellData( i, 2, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		// 2005-12-26 ADD BY PDW(Jr) =================================================
		// 이전 데이터에 더하도록 변경 
		GetDivValue( sData, &sCurSpecies, &sCurBook );

		// 입력
		CString sSpecies;
		CString sBook;
		CString sPreData;

		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}		

		GetDivValue( sPreData, &sSpecies, &sBook );
		
		// 장서보유현황은 책수만 나타난다.
		sData.Format( _T("%d"), _ttoi( sSpecies ) + _ttoi( sCurSpecies ) );
		// ===========================================================================

		ids = pDstDM->SetCellData( nInsertRowIdx, nInsertColIdx, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// ScrDM에서 해당 자료구분에 한해서 pDstDM의 특정 Cell로 넣는다.
// 교내발간자료, 교범통계, 비도서자료통계에 사용된다.
//
// [  PARAMETERS  ]
//    pSrcDM  : 쿼리로 나온 결과 DM     
//    pDstDM  : 출력할 DM
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. idx = 0의 셀위치의 정보( 자료구분 || 매체구분 )로 입력될 위치가 바뀐다.
// 2. idx = 1, idx = 2의 셀위치의 정보( 관리 구분 && 자료상태 )로 입력될 위치가 바뀐다.
// 3. idx = 3,4의 정보가 입력될 갯수다
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::CopySrcToDst3(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM)
{
	INT ids;
	CString sErrMsg;

	CString sMatCode;								// 자료구분
	CString sManageCode;							// 관리구분
	CString sData;
	CString sTemp;
	CString sPreSpecies;
	CString sPreBook;

    CString sInsertData;                            // 입력값 
	INT nInsertRowIdx;                              // 입력시킬 로우의 위치
	INT nInsertColIdx;								// 입력시킬 컬럼의 위치
	INT nRowCount = pSrcDM->GetRowCount();
	for ( INT i = 0; i < nRowCount;i++ )
	{
		ids = pSrcDM->GetCellData( i, 0, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 1 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		//-----------------------------------------------------------------------------
		// 1. idx = 0의 셀위치의 정보( 자료구분 || 매체구분 )로 입력될 위치가 바뀐다.
		//-----------------------------------------------------------------------------	
		if      ( sData == _T("QA") || sData == _T("MD") || sData == _T("SG") )	  nInsertRowIdx = 0;
		else if ( sData == _T("QB") || sData == _T("ME") || sData == _T("PC") )   nInsertRowIdx = 1;		
		else if ( sData == _T("QC") || sData == _T("MF") || sData == _T("OB") )   nInsertRowIdx = 2;
		else if ( sData == _T("QD") || sData == _T("MH") || sData == _T("XA") )   nInsertRowIdx = 3;
		else if ( sData == _T("QE") || sData == _T("MJ") || sData == _T("XU") )   nInsertRowIdx = 4;
		else if ( sData == _T("QF") || sData == _T("MK") || sData == _T("MA") )   nInsertRowIdx = 5;
		else if ( sData == _T("QG") || sData == _T("ML") || sData == _T("LF") )   nInsertRowIdx = 6;
		else if ( sData == _T("QH") || sData == _T("MN") || sData == _T("LQ") )   nInsertRowIdx = 7;
		else if ( sData == _T("QI") || sData == _T("MQ") || sData == _T("LP") )   nInsertRowIdx = 8;
		else if ( sData == _T("QJ") )					                  nInsertRowIdx = 9;
		else if ( sData == _T("QK") )					                  nInsertRowIdx = 10;
		else if ( sData == _T("QL") )					                  nInsertRowIdx = 11;
		else										 
		{
			if ( m_sStaticsName == _T("교범통계") )                       nInsertRowIdx = 12;
			else if ( m_sStaticsName == _T("교내발간자료통계") )          nInsertRowIdx = 9;
			else if ( m_sStaticsName == _T("비도서자료통계") )            nInsertRowIdx = 9;
		}
		
		//-----------------------------------------------------------------------------
		// 2. idx = 1, idx = 2의 셀위치의 정보( 관리 구분 && 자료상태 )로 입력될 위치가 바뀐다.
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 1, sManageCode );			// 관리구분
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 2 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		ids = pSrcDM->GetCellData( i, 2, sData );				// 자료상태
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		if ( sManageCode == _T("MA") )			nInsertColIdx = 0;
		else if ( sManageCode == _T("BR") )		nInsertColIdx = 1;
		else								nInsertColIdx = 2;

		// 17/07/20 김혜영 : 책나래대출자료 상태 추가 
/*//BEFORE-----------------------------------------------------------------------------
		//=====================================================
		//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") ||
// 			 sData == _T("SEL3110") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") )
// 			 nInsertColIdx += 4;
		// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") || _T("BOL218O") || sData == _T("BOL511O") ||
			 sData == _T("SEL3110") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") || _T("BOL5110"))
			 nInsertColIdx += 4;
//		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") || _T('BOL218O') || sData == _T("BOL511O") ||
//			 sData == _T("SEL3110") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") )
//			 nInsertColIdx += 4;
		//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") || sData == _T("BOL218O") || sData == _T("BOL219O") || sData == _T("BOL411O") ||  sData == _T("BOL511O") || sData == _T("BOL611O") ||
			 sData == _T("SEL311O") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") || sData == _T("SEL411O") || sData == _T("SEL511O"))
			 nInsertColIdx += 4;
/*//END-------------------------------------------------------------------------------*/
		
		//-----------------------------------------------------------------------------
		// 3. idx = 3,4의 정보가 입력될 갯수다
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 3, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		sInsertData = sData;

		ids = pSrcDM->GetCellData( i, 4, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		// 입력
		// 기존정보에 더한다.
		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sTemp );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 6 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		GetDivValue( sTemp, &sPreSpecies, &sPreBook );

		sInsertData.Format(_T("%d / %d") , _ttoi(sInsertData) + _ttoi(sPreSpecies) , _ttoi(sData) + _ttoi(sPreBook));

		ids = pDstDM->SetCellData( nInsertRowIdx, nInsertColIdx, sInsertData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 7 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// DM의 타겟위치의 값의 백분율을 타겟 필드에 입력한다.
// 책에 대한 백분율이다.
//
// [  PARAMETERS  ]
//    pDstDM         : 퍼센트 적용 DM
//    nTotalValuePos : 합계 컬럼 idx
//    nUpdateField   : 타겟 퍼센트 컬럼 idx
//    nTotalValue    : 총합계 
//    nMode          : 0 -> 로우단위 1 -> 컬럼단위 로 계산한다.
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. nTotalValuePos 위치의 값과 nTotalValue 값으로 퍼센트를 구한다.
// 2. nUpdateField에 입력한다.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::UpdatePercentage(CESL_DataMgr *pDstDM, INT nTotalValuePos, INT nUpdateField, INT nTotalValue, INT nMode)
{
	INT ids;
	INT nLoofCnt;
	CString sErrMsg;

	// 로우 단위 작업을 한다.
	CString sTemp;
	CString sSpeices;
	CString sBook;
	float fPercent;
	if ( nMode == 0 )
	{
		nLoofCnt = pDstDM->GetColCount();
		
		for ( INT i = 0;i < nLoofCnt;i++ )
		{
			ids = pDstDM->GetCellData( nTotalValuePos, i, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 1 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			if ( nTotalValue > 0 )
			{
				GetDivValue( sTemp, &sSpeices, &sBook);
			
				fPercent = (float)_ttoi(sBook) / (float)nTotalValue;
				sTemp.Format( _T("%.02f"), fPercent * 100 );			
			}
			else
			{
				sTemp.Format( _T("0") );
			}

			ids = pDstDM->SetCellData( nUpdateField, i, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 2 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}
		}
		/*
		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sTemp );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 6 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		*/
	}
	// 컬럼 단위 작업을 한다.
	else if ( nMode == 1)
	{
		nLoofCnt = pDstDM->GetRowCount();
		for ( INT i = 0;i < nLoofCnt;i++ )
		{
			ids = pDstDM->GetCellData( i, nTotalValuePos, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 3 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			GetDivValue( sTemp, &sSpeices, &sBook);
			
			fPercent = (float)_ttoi(sBook) / (float)nTotalValue;
			sTemp.Format( _T("%.02f"), fPercent * 100 );	
			
			ids = pDstDM->SetCellData( i, nUpdateField, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 4 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-22
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   함수설명   ] 
// 자료 류별 통계
//
// [  PARAMETERS  ]
//    sWhere  : 조건절     
//
// [ RETURN VALUE ]
//    0    : 성공 
//    음수 : 실패
//
// [   미니스펙   ]
// 1. 통계DM 초기화
// 2. 기본통계 정보 구성
// 3. 단행 통계 구성
// 4. 합계 구성
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_Class(CString sWhere)
{
	INT ids;
	CString sErrMsg;
	
	//-----------------------------------------------------------------------------
	// 1. 통계DM 초기화
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_자료류별통계_국방대");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// 출력용 DM은 형식이 고정되어있다.
	const INT nResultRow = 18;
	const INT nResultCol = 13;
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0 / 0"));		
	}

	// 통계 쿼리용 DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	

	//-----------------------------------------------------------------------------
	// 2. 기본 쿼리 정보 구성
	//-----------------------------------------------------------------------------
	CString sSelect;
	CString sTableName;
	CString sBaseWhere;
	CString sGroupBy;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;
	CString sQuery;
	
	
	sSelect.Format ( _T("SELECT B.REG_CODE, ") 
		             _T(" DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '58' ), -1, DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '47' ) , 1 , SUBSTR( LTRIM( B.CLASS_NO, ' ' ), 0, 1 ), '' ), DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) , NULL, '' , 'A' , '0' , 'Z' , '0' , 'B' , DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) ) - '74' ) , 1, '2' , '1' ) , 'H' , '3' , 'J' , '3' , 'K' , '3' , 'L' , '3' , 'U' , '3' , 'V' , '3' , 'Q' , '4' , 'R' , '5' , 'S' , '5' , 'T' , '5' , 'M' , '6' , 'N' , '6' , 'P' , DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) , 'C' , '7' , 'E' , '7' , 'F' , '7' , 'K' , '7' , 'M' , '7' , '8' ) , 'C' , '9' , 'D' , '9' , 'E' , '9' , 'F' , '9' , 'G' , '9' , '' ) )") );
	sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY ") );
	sGroupBy.Format( _T("Group By B.REG_CODE , ")
		             _T(" DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '58' ), -1, DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '47' ) , 1 , SUBSTR( LTRIM( B.CLASS_NO, ' ' ), 0, 1 ), '' ), DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) , NULL, '' , 'A' , '0' , 'Z' , '0' , 'B' , DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) ) - '74' ) , 1, '2' , '1' ) , 'H' , '3' , 'J' , '3' , 'K' , '3' , 'L' , '3' , 'U' , '3' , 'V' , '3' , 'Q' , '4' , 'R' , '5' , 'S' , '5' , 'T' , '5' , 'M' , '6' , 'N' , '6' , 'P' , DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) , 'C' , '7' , 'E' , '7' , 'F' , '7' , 'K' , '7' , 'M' , '7' , '8' ) , 'C' , '9' , 'D' , '9' , 'E' , '9' , 'F' , '9' , 'G' , '9' , '' ) )") );
	
	// 17/07/20 김혜영 : 책나래대출자료 상태 추가 
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );

// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );

	// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL511O') ") );

//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );
	//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/	
// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O' , 'SEL511O' ) ") );
	

	//-----------------------------------------------------------------------------
	// 3. 단행 통계 구성
	//-----------------------------------------------------------------------------	
	sTableName = _T("BO_SPECIES_TBL S , BO_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")
				  		_T(" AND B.BOOK_APPENDIX_FLAG = 'B' %s ")
				  , sSelect, sTableName, sBaseWhere, sWhere);
	sQuery += sBoWorkingStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 0 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. 연속 통계 구성
	//-----------------------------------------------------------------------------	
	DM.FreeData();
	sTableName = _T("SE_SPECIES_TBL S , SE_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY) , COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s %s ")				  	  
				  , sSelect, sTableName, sBaseWhere, sWhere );
	sQuery += sSeWorkinStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 4 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 1 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 5 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 5. 합계 구성
	//-----------------------------------------------------------------------------	
	// 가로 통계 구하기 
	UpdateRowSum( pDM_Dst, 0, 10, 11 );
	// 세로 통계 구하기 
	UpdateColSum( pDM_Dst, 0, 2, 3 );
	UpdateColSum( pDM_Dst, 4, 10, 11 );
	UpdateColSum( pDM_Dst, 12, 15, 16 );
	// 총계 구하기 
	// 더할 로우 3, 11, 16 -> 17
	UpdateColSum( pDM_Dst, 3, 3, 17 );
	UpdateColSum( pDM_Dst, 11, 11, 17 );
	UpdateColSum( pDM_Dst, 16, 16, 17 );
	// 비율 구하기
	CString sTemp;
	CString sSpeices;
	CString sBook;
	ids = pDM_Dst->GetCellData( nResultRow - 1, nResultCol - 2 , sTemp );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 6 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	GetDivValue( sTemp, &sSpeices, &sBook );

	UpdatePercentage(pDM_Dst, nResultCol - 2, nResultCol - 1, _ttoi(sBook), 1 ); 

	// 인쇄 형식 변경 -- 칸이 적다
	for (i = 0;i < nResultRow;i++ )
	{
		for ( INT j = 0;j < nResultCol;j++ )
		{
			ids = pDM_Dst->GetCellData( i, j, sTemp);	
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 7 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			sTemp.Replace(_T(" / "), _T("\n("));

			if ( j != nResultCol - 1 )
				sTemp += _T(")");

			ids = pDM_Dst->SetCellData( i, j, sTemp);	
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 8 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}
		}
	}

	return 0;
}

/*--------------------------------------------------------------------------------------------
   Work Day : 2005-12-26
   Author   : Park, Daewoo(Jr)
   Type     : ADD   

   [   함수설명   ] 
      결과 DM 엑셀로 저장한다 
 
   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패

   [   미니스펙   ]
      1. 엑셀 파일명을 설정한다.
      2. 통계의 종류에 따라 참조 DM을 달리한다. 
--------------------------------------------------------------------------------------------*/
INT CBO_STATICS_ND::SaveExcelFile()
{
	INT ids = 0;

	//-----------------------------------------------------------------------------
	// 1. 엑셀 파일명을 설정한다.
	//-----------------------------------------------------------------------------
	TCHAR TmpDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,TmpDir);

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = this->GetSafeHwnd();
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("EXCEL 파일 저장");
	
	if (GetSaveFileName(&OFN) == 0)
	{
		SetCurrentDirectory(TmpDir);
		AfxMessageBox( _T("Excel 출력이 취소되었습니다.") );
        return 0;
	}
	
	C_ExcelManControl m_ExcelCtrl;
	
	CRect rect;
	this->GetClientRect(&rect);

	// Excel Manager OCX객체를		 연결한다.
	m_ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, 8805);
	if (m_ExcelCtrl.m_hWnd == NULL) {
		SetCurrentDirectory(TmpDir);
		AfxMessageBox(_T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오."), MB_ICONEXCLAMATION);
		return 0;
	}

    CExcelManager m_ExcelManager(&m_ExcelCtrl);
    if(m_ExcelManager.OpenManager() == FALSE) {
		SetCurrentDirectory(TmpDir);
        AfxMessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"), MB_ICONEXCLAMATION);
		return 0;
	}

	//-----------------------------------------------------------------------------
	// 2. 통계의 종류에 따라 참조 DM을 달리한다. 
	//-----------------------------------------------------------------------------	
	CString sDMAlias;
	GetDMAlias( sDMAlias );

	CESL_DataMgr* pDM = FindDM( sDMAlias );
	
	INT nRowCnt = pDM->GetRowCount();
	INT nColCnt = pDM->GetColCount();
	
	INT nStartRow = 0;
	INT nStartCol = 0;

	TCHAR TmpDir2[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,TmpDir2);
	SetCurrentDirectory(TmpDir);
	MakeExcelShape( &m_ExcelManager, nStartRow, nStartCol );
	SetCurrentDirectory(TmpDir2);

	CString strData;
	for (INT i = 0; i < nRowCnt; i++) 
	{
		for (INT j = 0; j < nColCnt; j++) 
		{
			pDM->GetCellData( i , j , strData );
			strData = _T("'") + strData;
			m_ExcelManager.SetTextMatrix(i+nStartRow, j+nStartCol, strData);			
		}		
	}
	
	// 패스와 파일이름을 설정하여 저장한다.
    //m_ExcelManager.SaveAs(sPath + sTitle + _T(".xls"));
	CString strPath = OFN.lpstrFile;
	strPath.Replace(_T(".xls"), _T(""));
	m_ExcelManager.SaveAs(strPath + _T(".xls"));

	// 사용한 엑셀 객체를 반환한다.
    m_ExcelManager.CloseManager();

	AfxMessageBox(_T("EXCEL로 저장되었습니다."), MB_ICONEXCLAMATION);
	SetCurrentDirectory(TmpDir);

	return 0;
}

/*--------------------------------------------------------------------------------------------
   Work Day : 2005-12-26
   Author   : Park, Daewoo(Jr)
   Type     : ADD   

   [   함수설명   ] 
      현재 통계의 DM 알리아스를 지정한다.
	
   [  PARAMETERS  ]
      sGetDMAlias : DM 알리아스

   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패   
--------------------------------------------------------------------------------------------*/
INT CBO_STATICS_ND::GetDMAlias(CString &sGetDMAlias)
{
	if ( m_sStaticsName == _T("수입구분별통계(국방대)") || m_sStaticsName == _T("관리구분별통계"))
	{
		sGetDMAlias = _T("DM_수입관리구분별통계_국방대");
	}
	else if ( m_sStaticsName == _T("교범통계") || m_sStaticsName == _T("교내발간자료통계") || 
			  m_sStaticsName == _T("비도서자료통계") )
	{
		sGetDMAlias = _T("DM_교범비도서발간자료통계_국방대");
	}
	else if ( m_sStaticsName == _T("자료류별통계") )
	{
		sGetDMAlias = _T("DM_자료류별통계_국방대");
	}
	else if ( m_sStaticsName == _T("장서보유현황") )
	{
		sGetDMAlias = _T("DM_장서보유현황_국방대");
	}

	return 0;
}

/*--------------------------------------------------------------------------------------------
   Work Day : 2005-12-26
   Author   : Park, Daewoo(Jr)
   Type     : ADD   

   [   함수설명   ] 
      엑셀형태를 만든다.
	
   [  PARAMETERS  ]
      pExcelMgr : [I N] 엑셀메니져
	  nStartRow : [OUT] 데이터가 입력시작될 로우인덱스 
	  nStartCol : [OUT] 데이터가 입력시작될 컬럼인덱스 

   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패   
--------------------------------------------------------------------------------------------*/
INT CBO_STATICS_ND::MakeExcelShape(CExcelManager *pExcelMgr, INT &nStartRow, INT &nStartCol)
{
	// 1. DM을 찾아서 ColCount를 구한다.
	CString sDMAlias;
	GetDMAlias( sDMAlias );
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_STATICS_ND::MakeExcelShape()") );
	
	INT nColCnt = pDM->GetColCount();
	
	INT nCol = 0;
	INT nRow = 0;
	CString sTmp = _T("");
	CString sCurrentDay = _T("");
	CTime time;	
	time = time.GetCurrentTime();
	sCurrentDay = _T("'") + time.Format(_T("%Y/%m/%d"));

/*	// 2. 첫줄에 통계명을 입력한다.
	pExcelMgr->SetTextMatrix( ++nRow , 1 , m_sStaticsName);
	
	// 3. 둘째줄에 파일저장일을 입력한다.
	CTime time;	
	time = time.GetCurrentTime();
	sTmp = _T("'") + time.Format(_T("%Y/%m/%d"));
	pExcelMgr->SetTextMatrix( ++nRow , 1 , sTmp);
*/
	// 4. 통계의 종류마다 컬럼이 틀리다
	//    형식을 파일로 구축한다. 파일명은 통계명하고 동일하다 
	CString sFileName = _T("..\\Cfg\\통계\\Excel형식\\") + m_sStaticsName + _T(".cfg");
	CStdioFile FILE;
	if ( !FILE.Open( sFileName, CFile::modeRead | CFile::typeBinary ) )
	{
		CString sErrorMsg;
		sErrorMsg.Format( _T("FILE OPEN ERROR :: %s") , sFileName );
		AfxMessageBox( sErrorMsg );
		return -1;
	}
	
	TCHAR cUni;
	FILE.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		FILE.SeekToBegin();
	}

	INT nPos = 0;
	INT nMAXCol = 0;
	CString sBuf;

	while ( FILE.ReadString( sTmp ) )
	{
		// 주석처리
		if ( sTmp.Mid(0, 1) == _T("#") )	continue;
		// 데이터 입력 시작 위치
		else if ( sTmp.Mid(0, 1) == _T("$") )
		{
			sTmp.Replace( _T("$"), _T("") );
			if ( sTmp.Replace( _T("StartRowIndex="), _T("") ) > 0 )
				nStartRow = _ttoi( sTmp );
			else if ( sTmp.Replace( _T("StartColIndex="), _T("") ) > 0 )
				nStartCol = _ttoi( sTmp );
		}
		// 추가 정보
		else if ( sTmp.Mid(0, 1) == _T("%") )
		{
			sTmp.Replace( _T("%"), _T("") );
			// ',' 단위로 분리 되어 있다
			nCol = 0;
			nRow++;
			while ( TRUE ) 
			{
				nPos = sTmp.Find( _T(",") );
				
				if ( nPos < 0 )
				{
					sBuf = sTmp;
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();
					
					if ( sBuf.Compare( _T("통계명") ) == 0 )
						sBuf = m_sStaticsName;
					else if ( sBuf.Compare( _T("통계일") ) == 0 )
                        sBuf = sCurrentDay;

					pExcelMgr->SetTextMatrix( nRow , ++nCol , sBuf);
					break;
				}
				else
				{
					sBuf = sTmp.Mid(0, nPos + 1);
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();
					
					if ( sBuf.Compare( _T("통계명") ) == 0 )
						sBuf = m_sStaticsName;
					else if ( sBuf.Compare( _T("통계일") ) == 0 )
                        sBuf = sCurrentDay;
											
					pExcelMgr->SetTextMatrix( nRow , ++nCol , sBuf);

					sTmp = sTmp.Mid(nPos + 1);
				}
			}
			
		}
		// 셀병합정보
		else if ( sTmp.Mid(0, 1) == _T("@") )
		{
			sTmp.Replace( _T("@"), _T("") );
			// 셀병합
			// ',' 단위로 구분되어 있으며
			INT nSX = 0;
			INT nSY = 0;
			INT nEX = 0;
			INT nEY = 0;
	
			while ( TRUE ) 
			{
				nPos = sTmp.Find( _T(",") );
				
				if ( nPos < 0 )
				{
					sTmp.Replace( _T("EY"), _T("") );	// EndY
					nEY = _ttoi( sTmp );
					break;
				}
				else
				{
					sBuf = sTmp.Mid(0, nPos + 1);
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();

					if ( sBuf.Replace( _T("SX"), _T("") ) > 0 )	// StartX
						nSX = _ttoi( sBuf );					
					else if ( sBuf.Replace( _T("SY"), _T("") ) > 0 )	// StartY
						nSY = _ttoi( sBuf );					
					else if ( sBuf.Replace( _T("EX"), _T("") ) > 0 )	// EndX
						nEX = _ttoi( sBuf );					

					sTmp = sTmp.Mid(nPos + 1);
				}
			}

			pExcelMgr->Merge( CRect( nSX, nSY, nEX, nEY ) );
		}		
		// 컬럼명
		else
		{
			// ',' 단위로 분리 되어 있는 컬럼들을 입력한다

			nCol = 0;
			nRow++;
			while ( TRUE )
			{
				nPos = sTmp.Find( _T(",") );
				if ( nPos < 0 )
				{
					pExcelMgr->SetTextMatrix( nRow , ++nCol , sTmp);
					// 컬럼에 관련된 부분은 색을 칠하자
					pExcelMgr->Cell_Color(nCol, nRow, nCol, nRow, 15);
					pExcelMgr->Font(nCol, nRow, nCol, nRow, TRUE, FALSE, 0, 9);
					if ( nCol > nMAXCol )	nMAXCol = nCol;
					break;
				}
				else
				{
					sBuf = sTmp.Mid(0, nPos + 1);
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();
					pExcelMgr->SetTextMatrix( nRow , ++nCol , sBuf);
					// 컬럼에 관련된 부분은 색을 칠하자
					pExcelMgr->Cell_Color(nCol, nRow, nCol, nRow, 15);
					pExcelMgr->Font(nCol, nRow, nCol, nRow, TRUE, FALSE, 0, 9);
					sTmp = sTmp.Mid(nPos + 1);
					if ( nCol > nMAXCol )	nMAXCol = nCol;
				}			
			}
		}
	}
	
	FILE.Close();

	// 선을 그리자 	
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 1, 7, 2, 0);//왼쪽
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 2, 7, 2, 0);//오른쪽
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 3, 7, 2, 0);//위쪽
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 4, 7, 2, 0);//아래쪽		
	
	pExcelMgr->Vertical_Alignment(1,3,nMAXCol,nRow, ALIGN_VCENTER);
	pExcelMgr->Horizontal_Alignment(1,3,nMAXCol,nRow, ALIGN_HCENTER);

/*	// 5. 시작 로우와 컬럼 인덱스를 지정한다. 
	//    통계의 종류마다 틀리다. 
	//    소스에 FIX, 문제가 될겠지만 모르겠다 ㅡㅜ	
	if ( m_sStaticsName == _T("장서보유현황") )
	{
		nStartRow = 4;nStartCol = 3;
	}
	else if ( m_sStaticsName == _T("비도서자료통계") || m_sStaticsName == _T("교범통계") || m_sStaticsName == _T("교내발간자료통계"))
	{
		nStartRow = 5;nStartCol = 2;
	}
	else if ( m_sStaticsName == _T("자료류별통계") || m_sStaticsName == _T("관리구분별통계") || m_sStaticsName == _T("수입구분별통계(국방대)"))
	{
		nStartRow = 4;nStartCol = 4;
	}
*/
	return 0;
}

HBRUSH CBO_STATICS_ND::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

