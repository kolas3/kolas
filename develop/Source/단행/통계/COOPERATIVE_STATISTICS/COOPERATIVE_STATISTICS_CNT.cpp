// COOPERATIVE_STATISTICS_CNT.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_CNT.h"
#include "..\..\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_CNT dialog


CCOOPERATIVE_STATISTICS_CNT::CCOOPERATIVE_STATISTICS_CNT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_CNT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_CNT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentGrid = NULL;
	pParentMgr = pParent;
}
CCOOPERATIVE_STATISTICS_CNT::~CCOOPERATIVE_STATISTICS_CNT()
{
}


void CCOOPERATIVE_STATISTICS_CNT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_CNT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_CNT, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_CNT)
	ON_BN_CLICKED(IDC_btnSTATICS_START, OnbtnSTATICSSTART)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_CNT message handlers

BOOL CCOOPERATIVE_STATISTICS_CNT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = InitESL_Mgr( _T("COOPERATIVE_STATISTICS_CNT") );
	if( ids < 0 ) {
		AfxMessageBox ( _T("OnInitDialog Error") );
		return FALSE;
	}

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("통계종류"), sStaticsClass, -1, 0 );
	// 다이얼로그 제목
	SetWindowText( sStaticsClass );
	// 날짜 컨트롤
	if(_T("제공횟수통계") == sStaticsClass)
	{
		GetDlgItem(IDC_STC_CLASS_DATE)->SetWindowText( _T("제공일자") );
		GetDlgItem(IDC_staCooperativeStatisticsCnt)->SetWindowText( _T("제공횟수") );
		m_pParentGrid->SetTextMatrix(0, 8, _T("제공횟수"));
	}
	else
	{
		GetDlgItem(IDC_STC_CLASS_DATE)->SetWindowText( _T("신청일자") );
		GetDlgItem(IDC_staCooperativeStatisticsCnt)->SetWindowText( _T("신청횟수") );
		m_pParentGrid->SetTextMatrix(0, 8, _T("신청횟수"));
	}
	// 진행상태 (Default:0009(완료된자료))
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbCNT_STATE);
	pCombo->SetCurSel(4);

	return TRUE;
}

void CCOOPERATIVE_STATISTICS_CNT::OnbtnSTATICSSTART() 
{	
	// 통계실행
	RunStatics();
}

void CCOOPERATIVE_STATISTICS_CNT::OnbtnCLOSE() 
{
	OnCancel();	
}

/*
@Create
 - 2009.02.10 BY PWR
@Description
 - 통계실행
@Parameter
 - 
@Return Value
 - 0, 양수 : 정상
 - 음수 : 실패
@Mini Spec
 1. 쿼리 생성
 2. 단행 연속 결과를 가져온다.
 3. 결과 dm에 입력한다.
 4. 그리드 반영
 5. 출력물 지원 되도록 
*/
INT CCOOPERATIVE_STATISTICS_CNT::RunStatics()
{
EFS_BEGIN

	INT ids;
	CString strClassDate, strShelfDate, strClassCnt, strRank, strManageCode, strState;
	
	// 조건 가져오기
	if( 0 != GetControlSQL(strClassDate, strShelfDate, strClassCnt, strRank, strManageCode, strState) )
		return 0;

	CESL_DataMgr pTmpDM;
	pTmpDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	CESL_DataMgr* pDM = FindDM(_T("DM_COOPERATIVE_STATISTICS_CNT"));
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));
	pDM->FreeData();
	CESL_ControlMgr* pCM = FindCM(_T("CM_COOPERATIVE_STATISTICS_MAIN"));
	if( !pCM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("통계종류"), sStaticsClass, -1, 0 );

	// 1. 쿼리 생성
	//    단행 / 연속 한방에 끝내기
	CString strQuery;	
	strQuery.Format(
			_T("SELECT ROWNUM, ")
					_T("REG_NO, ")
					_T("TITLE, ")
					_T("AUTHOR, ")
					_T("PUBLISHER, ")
					_T("APL_MANAGE_CODE, ")
					_T("GIVE_MANAGE_CODE, ")
					_T("CNT ")
			_T("FROM (SELECT BB.REG_NO AS REG_NO, ")
							_T("IB.TITLE AS TITLE, ")
							_T("IB.AUTHOR AS AUTHOR, ")
							_T("IB.PUBLISHER AS PUBLISHER, ")
							_T("VV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
							_T("VV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
							_T("VV.CNT AS CNT ")
					_T("FROM BO_BOOK_TBL BB, ")
						_T("IDX_BO_TBL IB, ")
						_T("(SELECT TV.BOOK_KEY AS BOOK_KEY, ")
								_T("TV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
								_T("TV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
								_T("COUNT(1) AS CNT ")
						_T("FROM CO_LOAN_USER_TBL CLU, ")
								_T("ILL_TRANS_TBL IT, ")
								_T("BO_BOOK_TBL B, ")
								_T("TRANS_VIEW TV ")
						_T("WHERE IT.TRANS_NO=TV.TRANS_NO ")
							_T("AND CLU.REC_KEY=TV.USER_KEY ")
							_T("AND B.REC_KEY=TV.BOOK_KEY ")
							_T("AND IT.PUBLISH_FORM_CODE='MO' ")
							_T("%s") // STATE
							_T("%s") // GIVE_MANAGE_CODE
							_T("%s") // 제공일자
							_T("%s") // 배가일자
						_T("GROUP BY TV.BOOK_KEY, ")
							_T("TV.APL_MANAGE_CODE, ")
							_T("TV.GIVE_MANAGE_CODE ")
						_T("ORDER BY CNT DESC) VV ")
					_T("WHERE BB.REC_KEY=VV.BOOK_KEY ")
						_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
					_T("UNION ALL ")
					_T("SELECT BB.REG_NO AS REG_NO, ")
							_T("IB.TITLE AS TITLE, ")
							_T("IB.AUTHOR AS AUTHOR, ")
							_T("IB.PUBLISHER AS PUBLISHER, ")
							_T("VV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
							_T("VV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
							_T("VV.CNT AS CNT ")
					_T("FROM SE_BOOK_TBL BB, ")
						_T("IDX_SE_TBL IB, ")
						_T("(SELECT TV.BOOK_KEY AS BOOK_KEY, ")
							_T("TV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
							_T("TV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
							_T("COUNT(1) AS CNT ")
						_T("FROM CO_LOAN_USER_TBL CLU, ")
							   _T("ILL_TRANS_TBL IT, ")
							   _T("SE_BOOK_TBL B, ")
							   _T("TRANS_VIEW TV ")
						_T("WHERE IT.TRANS_NO=TV.TRANS_NO ")
							_T("AND CLU.REC_KEY=TV.USER_KEY ")
							_T("AND B.REC_KEY=TV.BOOK_KEY ")
							_T("AND IT.PUBLISH_FORM_CODE='SE' ")
							_T("%s") // STATE
							_T("%s") // GIVE_MANAGE_CODE
							_T("%s") // 제공일자
							_T("%s") // 배가일자
						_T("GROUP BY TV.BOOK_KEY, ")
							_T("TV.APL_MANAGE_CODE, ")
							_T("TV.GIVE_MANAGE_CODE ")
						_T("ORDER BY CNT DESC) VV ")
					_T("WHERE BB.REC_KEY=VV.BOOK_KEY ")
						_T("AND BB.SPECIES_KEY=IB.REC_KEY) ")
			_T("WHERE  1=1 ")
				_T("%s") // 횟수
				_T("%s") // 순위
			, strState, strManageCode, strClassDate, strShelfDate
			, strState, strManageCode, strClassDate, strShelfDate
			, strClassCnt, strRank );

	ids = pTmpDM.RestructDataManager( strQuery );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));

	
	// pTmpDM -> pDM 복사
	for(INT i = 0; i < pTmpDM.GetRowCount(); i++)
	{
		pDM->InsertRow(-1);
		CString sData, sDesc;
		// 순위
		ids = pTmpDM.GetCellData(i, 0, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("순위"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("AllControlDisplayGrid"));

		// 등록번호
		ids = pTmpDM.GetCellData(i, 1, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("등록번호"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("AllControlDisplayGrid"));

		// 서명
		ids = pTmpDM.GetCellData(i, 2, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("서명"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-6, _T("AllControlDisplayGrid"));

		// 저자
		ids = pTmpDM.GetCellData(i, 3, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-7, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("저자"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-8, _T("AllControlDisplayGrid"));

		// 발행자
		ids = pTmpDM.GetCellData(i, 4, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-9, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("발행자"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-10, _T("AllControlDisplayGrid"));

		// 신청도서관
		ids = pTmpDM.GetCellData(i, 5, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-11, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("신청관리구분"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-12, _T("AllControlDisplayGrid"));
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );
		ids = pDM->SetCellData( _T("신청관리구분설명"), sDesc, i );
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-13, _T("AllControlDisplayGrid"));

		// 제공도서관
		sDesc.Empty();
		ids = pTmpDM.GetCellData(i, 6, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-14, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("제공관리구분"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-15, _T("AllControlDisplayGrid"));
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );
		ids = pDM->SetCellData( _T("제공관리구분설명"), sDesc, i );
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-16, _T("AllControlDisplayGrid"));

		// 횟수
		ids = pTmpDM.GetCellData(i, 7, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-17, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("횟수"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-18, _T("AllControlDisplayGrid"));
	}

	pCM->ControlDisplay(_T("CntGrid"));
	
	INT nRowCnt = pDM->GetRowCount();
	if( !nRowCnt )
		AfxMessageBox(_T("통계결과가 없습니다."));
	else
		AfxMessageBox(_T("완료되었습니다"));

	return 0;

EFS_END
return -1;
}

/*
@Create
 - 2009.02.10 BY PWR
@Description
 - 컨트롤의 입력값으로 통계조건을 생성한다.
@Parameter
 - strClassDate[OUT]
 - strShelfDate[OUT]
 - strClassCnt[OUT]
 - strRank[OUT]
 - strManageCode[OUT]
 - strState[OUT]
@Return Value
 - 0, 양수 : 정상
 - 음수 : 실패
@Mini Spec
 1. 대출일 조건 생성
 2. 배가일 조건 생성
 3. 대출횟수 조건 생성
 4. 순위 조건 생성
*/
INT CCOOPERATIVE_STATISTICS_CNT::GetControlSQL(CString &strClassDate, CString &strShelfDate, CString &strClassCnt, CString &strRank, CString &strManageCode, CString &strState)
{
	CESL_ControlMgr* pCM = FindCM(_T("CM_COOPERATIVE_STATISTICS_CNT"));
	if( !pCM ){
		AfxMessageBox( _T("FindCM( CM_COOPERATIVE_STATISTICS_CNT ) ERROR ") );
		return -1;
	}

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("통계종류"), sStaticsClass, -1, 0 );

	const INT nControlCnt = 9;
	CString strCtrlAlias[nControlCnt] =
	{
		_T("조건별일자1"),	_T("조건별일자2"), 
		_T("배가일자1"),	_T("배가일자2"),
		_T("조건별횟수1"),	_T("조건별횟수2"), 
		_T("순위"),			_T("관리구분"),
		_T("진행상태")
	};

	CString strCtrlData[nControlCnt];
	CESL_Control_Element *pCE = NULL;

	// 날짜만 가져온다.
	for( INT i=0; i<4; i++ )
	{		
		pCE = (CESL_Control_Element*)pCM->FindControlElement( strCtrlAlias[i] );
		strCtrlData[i] = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		strCtrlData[i].TrimLeft(); strCtrlData[i].TrimRight();
	}

	if( strCtrlData[0].IsEmpty() && strCtrlData[2].IsEmpty() )
	{
		AfxMessageBox( _T("날짜를 입력해야합니다") );
		return 1;
	}

	// 기타
	for( i=4; i<nControlCnt; i++ )
	{
		pCM->GetControlMgrData( strCtrlAlias[i], strCtrlData[i] );
		strCtrlData[i].TrimLeft(); strCtrlData[i].TrimRight();
	}	

	// 대출일 쿼리
	if( !strCtrlData[0].IsEmpty() )
	{
		if( strCtrlData[1].IsEmpty() )
		{
			strCtrlData[1] = strCtrlData[0];
		}

		if(_T("제공횟수통계") == sStaticsClass)
		{
			strClassDate.Format( _T("AND IT.GIVE_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
								   _T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
									 strCtrlData[0], strCtrlData[1] );
		}
		else
		{
			strClassDate.Format( _T("AND IT.APL_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
								   _T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
									 strCtrlData[0], strCtrlData[1] );
		}
	}

	// 배가일 쿼리
	if( !strCtrlData[2].IsEmpty() )
	{
		if( strCtrlData[3].IsEmpty() )
		{
			strCtrlData[3] = strCtrlData[2];
		}

		strShelfDate.Format(_T("AND B.SHELF_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
							_T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
							strCtrlData[2], strCtrlData[3] );
	}

	// 대출횟수 
	if( !strCtrlData[4].IsEmpty() )
	{
		if( strCtrlData[5].IsEmpty() )
		{
			strClassCnt.Format( _T("AND CNT>=%s "), strCtrlData[4] );
		}
		else
		{
			strClassCnt.Format( _T("AND CNT BETWEEN %s AND %s "), strCtrlData[4], strCtrlData[5] );
		}
	}
	
	// 순위
	if( !strCtrlData[6].IsEmpty() )
	{
		strRank.Format( _T("AND ROWNUM<=%s "), strCtrlData[6] );
	}

	// 관리구분
	if( !strCtrlData[7].IsEmpty() )
	{
		if(_T("제공횟수통계") == sStaticsClass)
			strManageCode.Format( _T("AND TV.GIVE_MANAGE_CODE='%s' "), strCtrlData[7]);
		else
			strManageCode.Format( _T("AND TV.APL_MANAGE_CODE='%s' "), strCtrlData[7]);
	}

	// 진행상태
	if( !strCtrlData[8].IsEmpty() && strCtrlData[8]!=_T("----") )
	{
		strState.Format( _T("AND IT.STATE='%s' "), strCtrlData[8]);
	}

	return 0;
}