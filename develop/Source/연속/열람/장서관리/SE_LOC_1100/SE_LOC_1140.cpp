// SE_LOC_1140.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1140.h"

#include "..\..\..\..\공동목록\Light_BO_CAT_CENTERUPLOAD\BO_CAT_CENTERUPLOAD.h"
#include "..\BL_SE_LOC_1500\BL_SE_LOC_15.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1140 dialog


CSE_LOC_1140::CSE_LOC_1140(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1140)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1140::~CSE_LOC_1140()
{
}

VOID CSE_LOC_1140::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1140)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1140, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1140)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnRecord, OnbtnRecord)
	ON_BN_CLICKED(IDC_radLOCRECORD2, OnradLOCRECORD2)
	ON_BN_CLICKED(IDC_radLOCRECORD1, OnradLOCRECORD1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1140 message handlers

VOID CSE_LOC_1140::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CSE_LOC_1140::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1140")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetFirstEtcControl();

	ViewSpeciesBookCnt(0);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSE_LOC_1140::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_LOC_1140::OnbtnClose() 
{
	OnCancel();	
}

VOID CSE_LOC_1140::OnbtnRecord() 
{
	RecordShelfDate();
}

VOID CSE_LOC_1140::SetFirstEtcControl()
{
	((CButton*)GetDlgItem( IDC_radLOCRECORD1 ))->SetCheck( TRUE );
	((CButton*)GetDlgItem( IDC_radLOCRECORD2 ))->SetCheck( FALSE );

	CString sUserID = GetUserID();

	GetDlgItem(IDC_USER)->SetWindowText(sUserID);

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_CAT);
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("SetFirstEtcControl") );

	pGrid->SetRows(2);
	pGrid->SetCols(0,2);
	pGrid->SetFixedRows(1);
	pGrid->SetTextMatrix(0,0,_T("자료실"));
	pGrid->SetTextMatrix(0,1,_T("책수"));
		
}

VOID CSE_LOC_1140::OnradLOCRECORD2() 
{
	// 자료실 책수 보여주기
	ViewSpeciesBookCnt(1);
}

/// 자료실 책수 보여주기
INT CSE_LOC_1140::ViewSpeciesBookCnt(INT nMode)
{
	EFS_BEGIN

	INT ids;

	CString sMsg;
	INT nSelectCnt = 0;

	CESL_DataMgr* pBookCntDM = FindDM( _T("DM_자료실별책수") );
	if( pBookCntDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	pBookCntDM->FreeData();

	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	
	pParentGrid->SelectMakeList();
	

	pParentGrid->SelectGetHeadPosition();
		
	CESL_DataMgr* pParentDM = FindDM(_T("DM_SE_LOC_1100"));
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewSpeciesBookCnt") );

	if( nMode == 1 )
		nSelectCnt = pParentGrid->SelectGetCount();
	else
		nSelectCnt = pParentDM->GetRowCount();

	INT nIndex;
	CString sShelfLocCode;
	CString sCnt;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 1 )
			nIndex = pParentGrid->SelectGetIdx();
		else
			nIndex = i;

		ids = pParentDM->GetCellData( _T("자료실") , nIndex , sShelfLocCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewSpeciesBookCnt") );

		ids = CLocCommonAPI::GetCellData( this , pBookCntDM , _T("자료실") , sShelfLocCode , _T("책수") , sCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewSpeciesBookCnt") );

		if( ids > 0 )
		{
			pBookCntDM->InsertRow(-1);
			INT nInsertIndex = pBookCntDM->GetRowCount() -1 ;
			ids = pBookCntDM->SetCellData( _T("자료실") , sShelfLocCode ,  nInsertIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5, _T("ViewSpeciesBookCnt") );
			ids = pBookCntDM->SetCellData( _T("책수") , _T("1") ,  nInsertIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ViewSpeciesBookCnt") );
		}
		else
		{
			sCnt.Format( _T("%d") , _ttoi(sCnt) + 1 );
			ids = CLocCommonAPI::SetCellData(this , pBookCntDM , _T("자료실") , sShelfLocCode , _T("책수") , sCnt );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ViewSpeciesBookCnt") );
		}


		if( nMode == 1 )
			pParentGrid->SelectGetNext();

	}

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_CAT);
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );

	pGrid->SetRows(2);
	pGrid->SetCols(0,2);
	pGrid->SetColWidth(0,0,2000);
	pGrid->SetColWidth(0,1,2000);
	pGrid->SetFixedRows(1);
	pGrid->SetTextMatrix(0,0,_T("자료실"));
	pGrid->SetTextMatrix(0,1,_T("책수"));

	INT nGridRowCnt = pBookCntDM->GetRowCount() + 1;
	if( nGridRowCnt > 2 )
		pGrid->SetRows(nGridRowCnt);
	else
	{
		pGrid->SetTextMatrix(1,0,_T(""));
		pGrid->SetTextMatrix(1,1,_T(""));
	}
	
	for( i = 1 ; i < nGridRowCnt ; i++ )
	{
		nIndex = i-1;
		ids = pBookCntDM->GetCellData( _T("자료실") , nIndex , sShelfLocCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ViewSpeciesBookCnt") );
		ids = pBookCntDM->GetCellData( _T("책수") , nIndex , sCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ViewSpeciesBookCnt") );

		pGrid->SetTextMatrix( i , 0 , sShelfLocCode );
		pGrid->SetTextMatrix( i , 1 , sCnt );
	}
	

	return 0;

	EFS_END

	return -1;
}

VOID CSE_LOC_1140::OnradLOCRECORD1() 
{
	// 자료실 책수 보여주기
	ViewSpeciesBookCnt(0);
}


INT CSE_LOC_1140::RecordShelfDate()
{
	EFS_BEGIN
	
	INT ids;

	// 가능한지 검사
	CString sShelfDate , sShelfUser;
	CString sCMAlias = _T("CM_SE_LOC_1140");
	ids = GetControlData( sCMAlias , _T("Date") , sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
	sShelfDate.TrimRight(); sShelfDate.TrimLeft();
	ids = GetControlData( sCMAlias , _T("User") , sShelfUser );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
	sShelfUser.TrimLeft(); sShelfUser.TrimRight();

	if( sShelfDate.IsEmpty() ) 
	{
		AfxMessageBox( _T("배가일을 입력하십시오") );
		return 1;
	}
	if( sShelfUser.IsEmpty() )
	{
		AfxMessageBox( _T("배가 담당자를 입력하십시오") );
		return 2;
	}
	


	//  0 : 전체 , 1 : 그리드만
	INT nMode = ((CButton*)GetDlgItem(IDC_radLOCRECORD2))->GetCheck();

	INT nSelectCnt = 0;
	
	CESL_DataMgr* pBookCntDM = FindDM( _T("DM_자료실별책수") );
	if( pBookCntDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	pBookCntDM->FreeData();
	
	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	
	pParentGrid->SelectMakeList();
	
	
	pParentGrid->SelectGetHeadPosition();
	
	CESL_DataMgr* pParentDM = FindDM(_T("DM_SE_LOC_1100"));
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewSpeciesBookCnt") );
	
	if( nMode == 1 )
		nSelectCnt = pParentGrid->SelectGetCount();
	else
		nSelectCnt = pParentDM->GetRowCount();
	
	INT nSuccessCnt = 0 ;
	INT nRegFailCnt = 0;
	INT nStatusFailCnt = 0;

	INT nIndex;
	CString sWorkingStatus;
	CString sRegNo;
	CString sCnt;

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	CStringArray saSpeciesKeyList;
	CString sSpeciesKey;
	BOOL bInsertSpeciesKey;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		if( nMode == 1 )
			nIndex = pParentGrid->SelectGetIdx();
		else
			nIndex = i;
		

		ids = pParentDM->GetCellData( _T("자료상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewSpeciesBookCnt") );
		ids = pParentDM->GetCellData( _T("등록번호") , nIndex , sRegNo);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewSpeciesBookCnt") );
		
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewSpeciesBookCnt") );
		
		// 자료 상태가 배가 대상자료가 아니거나 등록번호가 없다면 배가 할수 없다.
		if( sWorkingStatus.Compare( _T("SEL211O") ) != 0 ) 
		{
			if( nMode == 1 )
				pParentGrid->SelectGetNext();			
			nStatusFailCnt++;
			continue;
		}
		if( sRegNo.IsEmpty() ) 
		{
			if( nMode == 1 )
				pParentGrid->SelectGetNext();			
			nRegFailCnt++;
			continue;
		}
		
		CESL_DataMgr TmpDM;

		ids = RecordDBProc(nIndex,&TmpDM,sShelfDate,sShelfUser);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewSpeciesBookCnt") );
		ids = RecordDMProc(nIndex,&TmpDM);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewSpeciesBookCnt") );
		ids = RecordGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewSpeciesBookCnt") );
			
		if( nMode == 1 )
			pParentGrid->SelectGetNext();

		ids = pParentDM->GetCellData( _T("종정보KEY") , nIndex , sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewSpeciesBookCnt") );
		
		bInsertSpeciesKey = TRUE;
		for( INT i = 0 ; i < saSpeciesKeyList.GetSize() ; i++ )
		{
			if( saSpeciesKeyList.GetAt(i).Compare(sSpeciesKey) == 0 )
				bInsertSpeciesKey = FALSE;
		}

		if( bInsertSpeciesKey )
			saSpeciesKeyList.Add(sSpeciesKey);

		nSuccessCnt++;
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );



	CString sResultMsg;	// 결과 메시지	

/*	연속 센터 업로드는 쫌 이따가
	// Center Upload
	INT nCenterUpload = ((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->GetCheck();
	if ( (nCenterUpload==1) && (saSpeciesKeyList.GetSize() > 0) )
	{
		CString strBaseDM = _T("DM_CENTERUPLOAD_01");
		CString strDupDM = _T("DM_CENTERUPLOAD_복본자료");
		
		CESL_DataMgr *pDM_Basic = FindDM(strBaseDM);
		if (pDM_Basic == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ViewSpeciesBookCnt") );
		
		CString strWhere;
		CString strRecKey;
		for ( INT idx = 0; idx < saSpeciesKeyList.GetSize(); idx++)
		{
			strRecKey = saSpeciesKeyList.GetAt(idx);
			if (idx == 0)
				strWhere.Format(_T("I.REC_KEY IN (%s "), strRecKey);
			else
				strWhere += _T(",") + strRecKey;
		}
		strWhere += _T(")");
		
		if (pDM_Basic->RefreshDataManager(strWhere) < 0)ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ViewSpeciesBookCnt") );
		
		INT nRowCount = pDM_Basic->GetRowCount();
		
		CBO_CAT_CENTERUPLOAD dlg(this);
		dlg.SetDMAlias(strBaseDM, strDupDM, _T(""));
		dlg.CenterUpload(FALSE) ;
	}
*/	
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 배가기록하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("배가기록 대상 자료 : '%d' \r\n-성공한 자료 : %d\r\n\r\n-등록번호가 존재하지 않은 자료 : '%d' \r\n-자료상태가 배가기록할수 없는 상태인 자료 : '%d'") 
		, nSelectCnt , nSuccessCnt ,  nRegFailCnt , nStatusFailCnt );

	AfxMessageBox( sResultMsg );
	
	return 0;

	EFS_END

	return -1;

}




/// 제본등록 DB 작업
/// 1. 책정보 UPDATE
/// 2. 수리제본정보INSERT
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CSE_LOC_1140::RecordDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate , CString sShelfUser )
{
	EFS_BEGIN

	INT ids;
	
	CString sDMAlias = _T("DM_SE_LOC_1100");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RegRepairDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( this , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("RegRepairDBProc") ); 

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("RegRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("RegRepairDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	// 프레임 생성
	// 1. 책정보 UPDATE
	pDM->InitDBFieldData();
	// 자료상태
	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );
	// 이전 자료상태
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	// 배가일
	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );
	// 배가 담당자
	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sShelfUser );
	pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 2. 종정보 기록
	CString sSpeciesKey,sVolKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("권호KEY") , 0 , sVolKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -8 , _T("RegRepairDBProc") ); 
	// 자료상태 - 소장자료
	
	// IDX Table의 WORKING_STATUS 를 동기화
	CLocCommonAPI::MakeIdxWokingStatusFrame( pDM , sSpeciesKey  , 1 , this );

	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("RegRepairDBProc") ); 
	sWorkingStatus = _T("SEL212N");
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	ids = pTmpDM->SetCellData( _T("자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("RegRepairDBProc") ); 
	

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -13 , _T("RegRepairDBProc") ); 


	CBL_SE_LOC_15 BL(this);
	BL.SettingSpeciesField( sSpeciesKey , sVolKey );

	return 0;

	EFS_END
	return -1;

}

/// 제본 등록 DM작업
/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CSE_LOC_1140::RecordDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_SE_LOC_1100");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("RegRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

/// 제본등록 화면 그리드작업
/// 1. INDEX로부터 그리드의 내용 변경
INT CSE_LOC_1140::RecordGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );

	ids = CLocCommonAPI::InsertGridRow( this , pParentGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("RegRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}



// 프로그래스바 생성
CLocProgressBarDlg* CSE_LOC_1140::CreateProgressBarDlg( INT nUpper )
{
	EFS_BEGIN
		
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(this);
	pProgressBar->Create( this );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);
	
	return pProgressBar;
	
	EFS_END
	return NULL;
	
}


// 프로그래스바 종료
INT CSE_LOC_1140::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	EFS_BEGIN
	
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
	
	EFS_END
	return -1;
		
}


HBRUSH CSE_LOC_1140::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
