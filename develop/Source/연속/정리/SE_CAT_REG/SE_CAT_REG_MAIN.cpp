// SE_CAT_REG_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REG_MAIN.h"

#include "..\..\..\단행\정리\BO_CAT_REGNO_GIVE\BO_CAT_REGNO_GIVE1.h"
#include "SE_CAT_REG_VIEW.h"
#include "MultiSortDlg.h"

// 연속수서api
#include "..\..\수서\SE_API\SeApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_MAIN dialog


CSE_CAT_REG_MAIN::CSE_CAT_REG_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_REG_MAIN::~CSE_CAT_REG_MAIN()
{
}

BOOL CSE_CAT_REG_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_CAT_REG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_REG_MAIN, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REG_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_MAIN message handlers

BOOL CSE_CAT_REG_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_등록_메인")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_REG_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	//pGrid->MoveWindow(-2, 35, cx+2, cy-35);	
	pGrid->MoveWindow(-2, 35, cx+2, cy);
	
}

INT CSE_CAT_REG_MAIN::CreateRegNO()
{

	INT idx;
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return FALSE;			
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return FALSE;
	}

	// 040305_HSS 등록구분 없는 넘은 등록번호 부여 안되도록
	//				등록완료자료(SER112N)는 처리 안되도록
	CString strTmp;
	
	while ( idx != -1 ) {
		// 등록완료자료 있으면 리턴
		strTmp = pDM->GetCellData( _T("BB_자료상태_코드") , idx );
		if (strTmp == _T("SER112N")) //등록완료자료 
		{
			ESLAfxMessageBox(_T("등록대상 자료만 처리 할 수 있습니다."));
			return FALSE;
		}
		// 등록구분 없으면 리턴
		strTmp = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
		if (strTmp == _T(""))
		{
			ESLAfxMessageBox(_T("등록구분이 없는 자료가 있습니다."));
			return FALSE;
		}

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	BO_CAT_REGNO_GIVE dlg(this, _T(""));
	dlg.m_strParentCMAlias = _T("CM_연속_정리_등록_메인");
	dlg.m_strParentDMAlias = _T("DM_연속_정리_등록_메인");
	dlg.m_nOpenMode = 3;
	dlg.DoModal();
	
	return 0;

}

VOID CSE_CAT_REG_MAIN::DoCatalog() //정리 대상처리
{
	INT idx, ids;
	
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus;

	// Confirm Registed
	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus == _T("SER111N")) // 등록대상자료 
		{
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			return;
		}

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	// Confirm
	if(IDCANCEL == ESLAfxMessageBox(_T("정리대상처리 작업을 하시겠습니까?"), MB_OKCANCEL))
		return;

	// 시간 만들기
	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	// 다시 리스트 만들기
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx);

	ids = pDM->StartFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("정리대상 자료로 변경중 에러가 발생했습니다."));
		return;
	}

	CString strQuery;
	INT Dm_index = 0;

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , idx - Dm_index);
		strQuery.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS='SEC111N', PREV_WORKING_STATUS='SER112N'")
						_T(", REG_TRANSFER_DATE = '%s', TRANSFER_YN='Y' WHERE REC_KEY=%s or BINDING_BOOK_KEY=%s;")
						, strTime, BookKey, BookKey );
		ids = pDM->AddFrame( strQuery );

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;
		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("정리대상 자료로 변경중 에러가 발생했습니다."));
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	ESLAfxMessageBox(_T("정리대상 자료로 변경 처리되었습니다."));
	
	return;

}

/*
VOID CSE_CAT_REG_MAIN::DoCatalog() //정리 대상처리
{
	INT idx;
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus == _T("SER111N")) // 등록대상자료 
		{
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			return;
		}

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("정리대상처리 작업을 하시겠습니까?"), MB_OKCANCEL))
		return;


	//대상 책 키 가져오기
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	// 변경 spl 실행
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEC111N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER112N") );

	//홍수 수정 +++
	pDM->AddDBFieldData(_T("REG_TRANSFER_DATE"), _T("STRING"), strTime );
	pDM->AddDBFieldData(_T("TRANSFER_YN"), _T("STRING"), _T("Y") );
	//홍수 수정 ---

	pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	
	//049 tag 생성
	sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_복본기호") , idx );  
	strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );


//	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

	//작업 로우 지우기 	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEC111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER112N") );
		
		//홍수 수정 +++
		pDM->AddDBFieldData(_T("REG_TRANSFER_DATE"), _T("STRING"), strTime );
		pDM->AddDBFieldData(_T("TRANSFER_YN"), _T("STRING"), _T("Y") );
		//홍수 수정 ---
		
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//049 tag 생성
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx - Dm_index);
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx - Dm_index);
		strCopyCode = pDM->GetCellData( _T("BB_복본기호") , idx - Dm_index);  
		strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx - Dm_index);

//		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

		//작업 로우 지우기 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	INT ids = pDM->SendFrame();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("정리대상 자료로 변경중 에러가 발생했습니다."));
		return;
	}


	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	ESLAfxMessageBox(_T("정리대상 자료로 변경 처리되었습니다."));
	
	return;

}
*/
VOID CSE_CAT_REG_MAIN::DoRealTimeShelf()
{
	INT idx;
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));


	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("SER111N")) //등록인계자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
		return;
	}

	idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("SER111N")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			return;
		}

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("실시간 처리 작업을 하시겠습니까?"), MB_OKCANCEL))
		return;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_연속_정리_등록_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_연속_정리_등록_메인 error") );
		return ;
	}


	//대상 책 키 가져오기
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEL111O") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER111N") );
	pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	

	//작업 로우 지우기 	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEL111O") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER111N") );
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//작업 로우 지우기 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	if( pDM->SendFrame() >= 0 )
	{

	
	}


	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("실시간 인계 자료로 처리 되었습니다."));

	return;


}

VOID CSE_CAT_REG_MAIN::DoBinding()
{
	INT idx;
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	if(idx < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	// HSS_040308 등록번호 있는 넘은 안됨
	
	// 등록인계자료인지 확인
	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		//등록자료(SER112N)는 수서 복귀시 (SER111N)으로 바뀌어야 한다.
		if (strStatus == _T("SER112N"))
		{
			ESLAfxMessageBox(_T("등록완료된 자료가 있습니다.\n등록번호를 취소해 주세요."));
			return;
		}
		// 제본자료인지 확인
		strStatus = pDM->GetCellData( _T("BB_권호유형") , idx );
		if (strStatus == _T("제본책"))
		{
			ESLAfxMessageBox(_T("선택한 자료 중 제본책이 있습니다.\n제본책은 다시 제본할 수 없습니다."));
			return;
		}
		// HSS_040401 부록자료는 안됨
		else if (strStatus == _T("부록책"))
		{
			ESLAfxMessageBox(_T("선택한 자료 중 부록책이 있습니다.\n부록책은 제본할 수 없습니다."));
			return;
		}

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	// 다시 Selected List 만들기
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	if(IDCANCEL == ESLAfxMessageBox(_T("제본처리 작업을 하시겠습니까?"), MB_OKCANCEL))
		return;

	//오늘 날짜 구하기
	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	pDM->StartFrame();
	INT Dm_index = 0;

	// 변경 spl 실행
	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , idx - Dm_index );

		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEB111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER111N") );
		pDM->AddDBFieldData(_T("REG_TRANSFER_DATE"), _T("STRING"), strTime );
		pDM->AddDBFieldData(_T("TRANSFER_YN"), _T("STRING"), _T("Y") );

		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx - Dm_index);
		pDM->InitDBFieldData();

		//작업 Row 지우기 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}

	INT ids = pDM->SendFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("제본 인계작업중 에러가 발생했습니다."));	
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	ESLAfxMessageBox(_T("제본 인계 자료로 처리 되었습니다."));

	return;

}

INT CSE_CAT_REG_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
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
			else if ( str == _T("SER1130") ) {
				str = _T("가원부자료");
			}
        }
    }
	return 0;


EFS_END
return -1;

}

BEGIN_EVENTSINK_MAP(CSE_CAT_REG_MAIN, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_REG_MAIN)
	ON_EVENT(CSE_CAT_REG_MAIN, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CAT_REG_MAIN::OnDblClickMshflexgrid() 
{
	CSE_CAT_REG_VIEW Dlg(this);
	Dlg.DoModal();
}

VOID CSE_CAT_REG_MAIN::DoSort()
{
	CString strDMAlias;
	CString strCMAlias;
	
	strDMAlias = _T("DM_연속_정리_등록_메인");
	strCMAlias = _T("CM_연속_정리_등록_메인");
	
	CESL_DataMgr *pDM = FindDM(strDMAlias);
	if (pDM == NULL) return ;

	CMultiSortDlg dlg(this);

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(strCMAlias, _T("MAIN_GRID"));
	if(pGrid == NULL)
		return;
	
	if (dlg.DoModal() == IDOK)
	{
		pDM->SORT(dlg.m_nSortCount, dlg.m_arrStrSortAlias, dlg.m_arrIntSortCond, ESL_DATAMGR_SORT_ASC);

		pGrid->Display();
	}
}

// 040309_HSS 새로 만들기
VOID CSE_CAT_REG_MAIN::DeleteRegNo()
{
	INT idx, ids;
	CString strQuery, sCount;
	CString strRegNo, strRegCode, strRecKey;

	// DM, CM, GRID 선정자료 체크 
	CESL_DataMgr *pDM;	
	pDM = FindDM(_T("DM_연속_정리_등록_메인"));		

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_연속_정리_등록_메인"));		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	// 리스트 만들기
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	if(idx < 0)	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );

	// Confirm
	if(IDCANCEL == ESLAfxMessageBox(_T("선정하신 자료의 등록번호를 취소하시겠습니까?"), MB_OKCANCEL))
		return;
	
	// StartFrame
	ids = pDM->StartFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("등록번호를 취소중 에러가 발생했습니다."));
		return;
	}

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
 	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	// AddFrame
	// 1. 누락 등록 테이블(CO_DROP_ACCESSION_NO_TBL)에 있는 지 조사 
	// 2. 없으면 누락 등록 테이블에 입력
	// 3. SE_BOOK_TBL에 업데이트(복본자료에도 적용)
	// 4. DM에 변경사항 적용
	// 5. 그리드에 반영
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		
		if (strRegNo != _T("")) //등록자료 
		{
			strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			strRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );

			// Update SE_BOOK_TBL 
			strQuery.Format( _T("UPDATE SE_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL, REG_DATE = ''")
						_T(", WORKING_STATUS = 'SER111N', PREV_WORKING_STATUS = 'SER112N', TRANSFER_YN = 'Y'")
						_T(" WHERE REC_KEY =%s or BINDING_BOOK_KEY=%s;")
						, strRecKey, strRecKey);
			ids = pDM->AddFrame( strQuery );

			// 누락 등록 테이블 같은 등록번호가 있는지 조사 
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			strQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s")
// 							, strRegCode , strRegNo.Mid(2,10));
			strQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s")
							, strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ));
			//=====================================================				
			pDM->GetOneValueQuery( strQuery , sCount );
			
			// Insert into CO_DROP_ACCESSION_NO_TBL
			if ( sCount == _T("0") ) {
				//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			strQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, ACCESSION_REC_NO, REG_CODE, REG_NO, MANAGE_CODE)")
// 					_T(" VALUES (ESL_SEQ.NEXTVAL, 'SE', '', '%s', %s, UDF_MANAGE_CODE );"), strRegCode, strRegNo.Mid(2,10) );
			strQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, ACCESSION_REC_NO, REG_CODE, REG_NO, MANAGE_CODE)")
					_T(" VALUES (ESL_SEQ.NEXTVAL, 'SE', '', '%s', %s, UDF_MANAGE_CODE );"), strRegCode, strRegNo.Mid(12 - nRegCodeLength ) );
			//=====================================================		
				
				ids = pDM->AddFrame( strQuery );		
			}		
			
			pDM->SetCellData( _T("원부번호") ,_T(""), idx );
			pDM->SetCellData( _T("BB_등록번호") ,_T(""), idx );
			pDM->SetCellData( _T("BB_자료상태_코드") ,_T("SER111N"), idx );

			// 해당 그리드 반영 
			pGrid->DisplayLine(idx);
		}
		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}
	
	// SendFrame
	ids = pDM->SendFrame();
	if (ids < 0 )
	{
		ESLAfxMessageBox(_T("등록번호 취소중 오류가 발생하였습니다."));
		return;
	}
	else{
		ESLAfxMessageBox(_T("등록번호 취소가 정상적으로 수행되었습니다."));	
	}

	// Display
	//pGrid->Display();

	return;

}


// 041323_HSS 책/부록삭제 추가
VOID CSE_CAT_REG_MAIN::DeleteBook()
{
	// DM, CM, GRID 선정자료 체크 
	CESL_DataMgr *pDM;	
	pDM = FindDM(_T("DM_연속_정리_등록_메인"));		

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_연속_정리_등록_메인"));		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SelectMakeList();

	INT idx = pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
	    return;
	}
	
	CString msg;
	INT nCount = pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] 건의 자료를 삭제 하시겠습니까?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 

	BeginWaitCursor();

	CSeApi api(this);
	CString strBOOK_KEY, strACQ_TYPE_VOL_KEY, strVOL_KEY;
	CString strSPECIES_KEY, strBOOK_TYPE, strBooktitle;

	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	ids = pDM->StartFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("StartFrame Error!"));
	    return;
	}

	while(idx>=0)
	{
		strBOOK_KEY = pDM->GetCellData(_T("BB_책KEY"),idx);
		strACQ_TYPE_VOL_KEY = pDM->GetCellData(_T("BB_수입구분별권KEY"),idx);
		strVOL_KEY = pDM->GetCellData(_T("권호키"),idx);
		strSPECIES_KEY = pDM->GetCellData(_T("IBS_종KEY"),idx);
		strBOOK_TYPE = pDM->GetCellData(_T("책종류"),idx);
		strBooktitle = pDM->GetCellData(_T("IBS_본표제"),idx);
		strBooktitle += _T(" - ") + pDM->GetCellData(_T("BB_편제명부록명"),idx);
		
		ids = api.BOOK_CAT_DELETE(pDM,
						strBOOK_KEY,
						strACQ_TYPE_VOL_KEY,
						strVOL_KEY,
						strSPECIES_KEY,
						strBOOK_TYPE,
						strBooktitle,
						TRUE, FALSE);

		if(ids<0)
		{
			EndWaitCursor();

			if(-1002 == ids) //cancel
			{
				msg.Format(_T("[ %s ] 자료를 삭제할 수 없습니다!"), strBooktitle);
				AfxMessageBox(msg);
				idx = pGrid->SelectGetNext();
				continue;
			}
			else
			{
				msg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"), strBooktitle);
				AfxMessageBox(msg);
				ids = pDM->EndFrame();
				return;
			}
		}
		else 
		{
			arrayDeletedRow.Add(idx);
		}

		idx = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("SendFrame Error!"));
	    return;
	}
	
	ids = pDM->EndFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("SendFrame Error!"));
	    return;
	}

	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		pDM->DeleteRow(nDeletedRow);
	}
	
	pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] 건의 자료를 삭제하였습니다!"),nSize);
	AfxMessageBox(msg);

	return;

}

HBRUSH CSE_CAT_REG_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
