// BO_LOC_1140.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1140.h"
#include "..\\BL_LOC\\BL_BOSHELF.h"
#include "..\..\..\..\공동목록\Light_BO_CAT_CENTERUPLOAD\BO_CAT_CENTERUPLOAD.h"
#include "UC_KOLISNetUseChecker.h"
#include "BO_LOC_1170.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1140 dialog


CBO_LOC_1140::CBO_LOC_1140(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1140)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nPossibleCnt = 0;


	//JOB.2019.0073 : 충돌 License
// 	m_pMailRecord = new SendMailRecord;
// 
// 	m_pMailRecord->m_nFileAttachCnt = 0;
// 	m_pMailRecord->m_pFileAttach = _T("");
// 	m_pMailRecord->m_sEncoding = _T("text/html");
// 
// 	m_pSendMailMgr = new CSendMailMgr(this);
// 
// 	m_pSendMailMgr->DeleteAllMailRecord();	

	m_strLibCode = _T("");
	m_strUserID=pParent->GetUserID();

	m_pLoanShareManager = NULL;
	
}

CBO_LOC_1140::~CBO_LOC_1140()
{	
	if ( m_pLoanShareManager != NULL ) delete m_pLoanShareManager;
	FreeMailMgr();
}

VOID CBO_LOC_1140::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1140)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1140, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1140)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnRecord, OnbtnRecord)
	ON_BN_CLICKED(IDC_radLOCRECORD2, OnradLOCRECORD2)
	ON_BN_CLICKED(IDC_radLOCRECORD1, OnradLOCRECORD1)
	ON_BN_CLICKED(IDC_chkCENTER_UPLOAD_BC, OnchkCENTERUPLOADBC)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1140 message handlers

VOID CBO_LOC_1140::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_LOC_1140::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("BO_LOC_1140")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetFirstEtcControl();
	ViewSpeciesBookCnt(0);

	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create( this );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_LOC_1140::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_1140::OnbtnClose() 
{
	OnCancel();	
}

VOID CBO_LOC_1140::OnbtnRecord() 
{
	RecordShelfDate();
}

VOID CBO_LOC_1140::SetFirstEtcControl()
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

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	TmpDM.MANAGE_CODE = m_pInfo->MANAGE_CODE;

	CString sCenterUploadYN;
	CString sQuery = _T("SELECT value_1 FROM ESL_MANAGE_TBL ")
		_T(" WHERE group_1='L' and group_2='기타' and group_3='공통' and class_alias='센터업로드사용여부'")
		_T(" and value_name='CENTER_UPLOAD' AND MANAGE_CODE = UDF_MANAGE_CODE ");
	TmpDM.GetOneValueQuery(sQuery, sCenterUploadYN);

	if ( _T("Y") == sCenterUploadYN ) 
	{
		GetDlgItem( IDC_chkCENTER_UPLOAD_BC )->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(1);
		
		UC_KOLISNetUseChecker KolisnetUseChk;
		if ( KolisnetUseChk.GetUseFlag(FALSE) == FALSE)
		{
			((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(0);
		}
	}
	else 
	{
		GetDlgItem( IDC_chkCENTER_UPLOAD_BC )->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(0);
	}
	UC_KOLISNetUseChecker KolisnetUseChk;
	if ( KolisnetUseChk.GetUseFlag(FALSE) == FALSE)
	{
		((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(0);
	}
}

VOID CBO_LOC_1140::OnradLOCRECORD2() 
{
	ViewSpeciesBookCnt(1);
}

INT CBO_LOC_1140::ViewSpeciesBookCnt(INT nMode)
{
	EFS_BEGIN

	INT ids;

	CString sMsg;
	INT nSelectCnt = 0;

	CESL_DataMgr* pBookCntDM = FindDM( _T("DM_자료실별책수") );
	if( pBookCntDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	pBookCntDM->FreeData();

	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	
	pParentGrid->SelectMakeList();
	

	pParentGrid->SelectGetHeadPosition();
		
	CESL_DataMgr* pParentDM = FindDM(_T("DM_BO_LOC_1100"));
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

VOID CBO_LOC_1140::OnradLOCRECORD1() 
{
	ViewSpeciesBookCnt(0);
}


INT CBO_LOC_1140::RecordShelfDate()
{
	EFS_BEGIN
	
	INT ids;

	CString sShelfDate , sShelfUser;
	CString sCMAlias = _T("CM_BO_LOC_1140");
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
	
	INT nMode = ((CButton*)GetDlgItem(IDC_radLOCRECORD2))->GetCheck();

	INT nSelectCnt = 0;
	
	CESL_DataMgr* pBookCntDM = FindDM( _T("DM_자료실별책수") );
	if( pBookCntDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	pBookCntDM->FreeData();
	
	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );
	
	pParentGrid->SelectMakeList();	
	
	pParentGrid->SelectGetHeadPosition();
	
	CESL_DataMgr* pParentDM = FindDM(_T("DM_BO_LOC_1100"));
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

	CString strReservationUseYN;
	pParentDM->GetOneValueQuery( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='예약관리' AND VALUE_NAME='예약기능사용여부' AND MANAGE_CODE = UDF_MANAGE_CODE"), strReservationUseYN );

	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	CStringArray saSpeciesKeyList;
	CStringArray saReservationIndexList;
	CString sSpeciesKey;
	CString sBookKey;
	BOOL bInsertSpeciesKey;
	BOOL bReservationFlag;
	INT	nReservationCnt;
	INT nReservationCancelCnt;
	nReservationCnt = 0;
	nReservationCancelCnt = 0;

	CStringArray strArrayControlList;
	CString strControlno;

	strArrayControlList.RemoveAll();
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
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
		
		if( sWorkingStatus.Compare( _T("BOL111O") ) != 0 ) 
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

		INT nMailResutType;
		nMailResutType = RecordDBProc(nIndex,&TmpDM,sShelfDate,sShelfUser);
		if( nMailResutType < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewSpeciesBookCnt") );

		CString strBookAppendixFlag;
		ids = pParentDM->GetCellData( _T("책_부록플래그") , nIndex , strBookAppendixFlag );
		if ( strReservationUseYN != _T("0") && strBookAppendixFlag == _T("B"))
		{
			if ( nMailResutType == 200 || nMailResutType == 300 )
			{
				ids = ReservationFurnishBook(pParentDM, nIndex, &bReservationFlag);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReservationFurnishBook") );
				ids = pParentDM->GetCellData( _T("책정보KEY") , nIndex , sBookKey );
				nReservationCnt++;				
				saReservationIndexList.Add(sBookKey);
			}
			else if ( nMailResutType == 100 )
			{
				nReservationCancelCnt++;
			}			
		}

		ids = RecordDMProc(nIndex,&TmpDM, bReservationFlag);
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

		ids = pParentDM->GetCellData( "제어번호" , nIndex , strControlno );		
		if ( strControlno != "" ) strArrayControlList.Add(strControlno);
		
		nSuccessCnt++;
	}
	
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );



	CString sResultMsg;

	INT nCenterUpload = ((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->GetCheck();
	if ( (nCenterUpload==1) && (saSpeciesKeyList.GetSize() > 0) )
	{
		MakeUploadFile(strArrayControlList);		
	}
	
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 %d건의 자료를 배가기록하였습니다.") , nSelectCnt );		
	else
		sResultMsg.Format( _T("배가기록 대상 자료 : %d 건 \r\n※ 성공한 자료 : %d 건\r\n\r\n※ 등록번호가 존재하지 않은 자료 : %d 건 \r\n※ 자료상태가 배가기록할수 없는 상태인 자료 : %d 건") 
		, nSelectCnt , nSuccessCnt ,  nRegFailCnt , nStatusFailCnt );

	CString strTmp;
	if ( nReservationCnt > 0 )
	{
		strTmp.Format(_T("\n\n※ 비치희망자료 자동예약자료 : %d 건"), nReservationCnt );
		sResultMsg += strTmp;
	}
	
	if ( nReservationCancelCnt > 0 )
	{
		strTmp.Format(_T("\n\n※ 예약초과 또는 대출불가로 예약취소된자료 : %d 건"), nReservationCancelCnt );
		sResultMsg += strTmp;
	}	

	AfxMessageBox( sResultMsg );	
	
	if( m_nPossibleCnt > 0 )
	{

	}

	if ( nReservationCnt > 0 )
	{
		CBO_LOC_1170 dlg(this);
		dlg.SetBookKeyList(&saReservationIndexList, nReservationCnt);		
		dlg.DoModal();
	}	

	return 0;

	EFS_END

	return -1;

}

INT CBO_LOC_1140::RecordDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sShelfDate , CString sShelfUser )
{
	EFS_BEGIN

	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_1100");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RegRepairDBProc") );

	ids = CLocCommonAPI::MakeCopyDM( this , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("RegRepairDBProc") ); 

	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("RegRepairDBProc") ); 

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("자료상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("RegRepairDBProc") ); 

	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL112N") );

	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );

	pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sShelfDate );

	pDM->AddDBFieldData( _T("SHELF_WORKER") , _T("STRING") , sShelfUser );

	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	
	pDM->AddDBFieldData( _T("DUP_SPECIES_KEY"), _T("NUMERIC"), _T("NULL") );
	
	pDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	CString sSpeciesKey;
	ids = pTmpDM->GetCellData( _T("종정보KEY") , 0 , sSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("RegRepairDBProc") ); 

	pDM->AddDBFieldData( _T("LOC_FLAG") , _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("DUP_FLAG") , _T("STRING") , _T("") );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("SPECIES_CLASS") , _T("STRING") , _T("1") );

	pDM->AddDBFieldData( _T("HOLD_DATE"), _T("NUMERIC"), _T("SYSDATE") );
	pDM->MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	pDM->InitDBFieldData();

	CLocCommonAPI::MakeIdxWokingStatusFrame( pDM , sSpeciesKey , 0 ,  this );
		
	CString sDupSpeciesKey;
	CString sTmpTblRecKey;
	ids = pTmpDM->GetCellData( _T("복본추기종KEY") ,  0 , sDupSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -241 , _T("RegRepairDBProc") ); 

	if( !sDupSpeciesKey.IsEmpty() )
	{
		pDM->MakeDeleteFrame( _T("BO_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDupSpeciesKey );
		pDM->MakeDeleteFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDupSpeciesKey );
		
		pDM->MakeRecKey( sTmpTblRecKey );
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("SPECIES_KEY") , _T("NUMERIC") , sDupSpeciesKey );
		pDM->AddDBFieldData( _T("IDX_WORK_FLAG") , _T("STRING") , _T("D") );
		pDM->AddDBFieldData( _T("INPUT_DATE") , _T("NUMERIC") , _T("SYSDATE") );

#ifdef __K2UP_INDEX_TBL_LOG_INSERT__
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("배가일기록"), __WFILE__, __LINE__ ), TRUE );
#endif
	}

	CString sAcqCode , sAcqKey;
	ids = pTmpDM->GetCellData( _T("수입구분") , 0 , sAcqCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->GetCellData( _T("구입정보KEY") , 0 , sAcqKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("RegRepairDBProc") ); 
	CString sFunishInfoUpdateSql;
	
	INT nMailResultType = 0;
	if( !sAcqKey.IsEmpty() )
	{
		if ( _T("-1") != sAcqKey )
		{
			nMailResultType = SendMailFurnish(pDM, nDMIndex, sAcqCode, sAcqKey);
			if ( nMailResultType > 0 && nMailResultType != 300 ) m_nPossibleCnt++;

			sFunishInfoUpdateSql.Format( _T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='3', FURNISH_DATE = '%s' WHERE ACQ_CODE='%s' AND ACQ_KEY=%s;") , 
										sShelfDate, sAcqCode , sAcqKey );
			pDM->AddFrame( sFunishInfoUpdateSql );
		}
	}	
	
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("RegRepairDBProc") ); 
	sWorkingStatus = _T("BOL112N");
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	ids = pTmpDM->SetCellData( _T("자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("최초배가일자") , sShelfDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("복본여부") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("RegRepairDBProc") ); 
	
	ids = pTmpDM->SetCellData( _T("복본추기종KEY") , _T("") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("RegRepairDBProc") ); 	

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -13 , _T("RegRepairDBProc") ); 

	return nMailResultType;

	EFS_END
	return -1;

}

INT CBO_LOC_1140::RecordDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM, BOOL bReservation )
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_1100");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("RegRepairDMProc") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1140::RecordGridProc( INT nDMIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewSpeciesBookCnt") );

	ids = CLocCommonAPI::InsertGridRow( this , pParentGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("RegRepairGridProc") );

	return 0;

	EFS_END
	return -1;

}

CLocProgressBarDlg* CBO_LOC_1140::CreateProgressBarDlg( INT nUpper )
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

INT CBO_LOC_1140::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	EFS_BEGIN
	
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
	
	EFS_END
	return -1;
		
}

INT CBO_LOC_1140::SendMailFurnish(CESL_DataMgr * TmpDM, INT nDMIndex, CString sAcqCode, CString sAcqKey)
{
EFS_BEGIN

	if( sAcqKey.IsEmpty() ) return 0;
	
	TmpDM->MANAGE_CODE = m_pInfo->MANAGE_CODE;

	CString sFurnishStatus;
	CString sQuery;
	sQuery.Format(_T("SELECT FURNISH_STATUS FROM  BO_FURNISH_TBL WHERE ACQ_CODE='%s' AND ACQ_KEY=%s AND MANAGE_CODE = UDF_MANAGE_CODE "), sAcqCode, sAcqKey);
	TmpDM->GetOneValueQuery(sQuery, sFurnishStatus);

	if ( sFurnishStatus == _T("3") || sFurnishStatus==_T("")) return 0;

	return OpenMail(TmpDM, nDMIndex, sAcqCode, sAcqKey);

EFS_END
	return -1;
}

INT CBO_LOC_1140::OpenMail(CESL_DataMgr * TmpDM, INT nDMIndex, CString sAcqCode, CString sAcqKey)
{
EFS_BEGIN
	CString sReservationYN;
	CString sWhere = _T("");
	CString strQuery;
	CString strUserNo;
	CString strTemp;

	CESL_DataMgr* pDM = FindDM( _T("DM_비치자료메일") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FindDM Failed") );	
	
	sWhere.Format(_T("acq_code = '%s' and acq_key = %s"), sAcqCode, sAcqKey);
	pDM->RefreshDataManager(sWhere);

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("이메일"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes1);
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("이메일"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();	
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	CESL_DataMgr* pUserDM = FindDM( _T("DM_BO_LOC_3300") );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FindDM Failed") );

	pDM->GetCellData(_T("대출자번호"), 0, strUserNo );
	if ( strUserNo != _T("") ) 
	{
		strTemp.Format(_T("USER_NO = '%s'"), strUserNo );
		pUserDM->RefreshDataManager(strTemp);

		if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pUserDM->GetRowCount())
		{
			CString strGetData;
			CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8,asDes9,asDes10,asDes11;
			for(INT i = 0; i < pUserDM->GetRowCount(); i++)
			{
				pUserDM->GetCellData(_T("자택주소"), i, strGetData);
				asDes1.Add(strGetData);
				pUserDM->GetCellData(_T("근무지주소"), i, strGetData);
				asDes2.Add(strGetData);
				pUserDM->GetCellData(_T("주민등록번호"), i, strGetData);
				asDes3.Add(strGetData);
				pUserDM->GetCellData(_T("E_MAIL"), i, strGetData);
				asDes4.Add(strGetData);
				pUserDM->GetCellData(_T("휴대폰"), i, strGetData);
				asDes5.Add(strGetData);
				pUserDM->GetCellData(_T("비밀번호"), i, strGetData);
				asDes6.Add(strGetData);
				pUserDM->GetCellData(_T("자택전화"), i, strGetData);
				asDes7.Add(strGetData);
				pUserDM->GetCellData(_T("근무지전화"), i, strGetData);
				asDes8.Add(strGetData);
				pUserDM->GetCellData(_T("IDX_핸드폰"), i, strGetData);
				asDes9.Add(strGetData);
				pUserDM->GetCellData(_T("회원증비밀번호"), i, strGetData);
				asDes10.Add(strGetData);
				pUserDM->GetCellData(_T("제2연락처"), i, strGetData);
				asDes11.Add(strGetData);
			}
			CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes6);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes7);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes8);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes9);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), asDes10);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes11);
						pApi->CloseSocket();	
					}
					for(i = 0; i < pUserDM->GetRowCount(); i++)
					{
						pUserDM->SetCellData(_T("자택주소"), asDes1.GetAt(i), i);
						pUserDM->SetCellData(_T("근무지주소"), asDes2.GetAt(i), i);
						pUserDM->SetCellData(_T("주민등록번호"), asDes3.GetAt(i), i);
						pUserDM->SetCellData(_T("E_MAIL"), asDes4.GetAt(i), i);
						pUserDM->SetCellData(_T("휴대폰"), asDes5.GetAt(i), i);
						pUserDM->SetCellData(_T("비밀번호"), asDes6.GetAt(i), i);
						pUserDM->SetCellData(_T("자택전화"), asDes7.GetAt(i), i);
						pUserDM->SetCellData(_T("근무지전화"), asDes8.GetAt(i), i);
						pUserDM->SetCellData(_T("IDX_핸드폰"), asDes9.GetAt(i), i);
						pUserDM->SetCellData(_T("회원증비밀번호"), asDes10.GetAt(i), i);
						pUserDM->SetCellData(_T("제2연락처"), asDes11.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
			asDes3.RemoveAll(); asDes3.FreeExtra();
			asDes4.RemoveAll(); asDes4.FreeExtra();
			asDes5.RemoveAll(); asDes5.FreeExtra();
			asDes6.RemoveAll(); asDes6.FreeExtra();
			asDes7.RemoveAll(); asDes7.FreeExtra();
			asDes8.RemoveAll(); asDes8.FreeExtra();
			asDes9.RemoveAll(); asDes9.FreeExtra();
			asDes10.RemoveAll(); asDes10.FreeExtra();
			asDes11.RemoveAll(); asDes11.FreeExtra();
		}
	}	

	pDM->GetCellData(_T("예약여부"), 0, sReservationYN);
	BOOL bReservationFlag = FALSE;
	BOOL bReservationFailFlag = FALSE;	

	if ( sReservationYN == _T("Y") )
	{
		if ( strUserNo == "" ) return 0;
		ReservationCheck(TmpDM, nDMIndex, pUserDM, &bReservationFlag);
		if ( bReservationFlag == FALSE ) 
		{
			bReservationFailFlag = TRUE;
			sReservationYN = _T("N");
		}
	}

 	INT nPossibleCnt = 0;

	if( nPossibleCnt > 0 ) 
	{
		if ( bReservationFailFlag ) return 100;
		
		if ( sReservationYN == _T("Y") ) return 200;

		return 1;
	}
	if ( bReservationFlag == TRUE && sReservationYN == 'Y' )
	{
		return 300;
	}

	return 0;
EFS_END
	return -1;
}

CString CBO_LOC_1140::MakeBodyContents(CString strBody,
									   CString strLibCode, 
									   CString strUserName, 
									   CString strBookTitle, 
									   CString strShelfCode, 
									   CString strAuthor, 
									   CString strPublisher,
									   CString strApplicantDate,
									   CString strReservationDate)
{

	CString strIs5163;
	CString strQuery;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	TmpDM.MANAGE_CODE = m_pInfo->MANAGE_CODE;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '5163' AND MANAGE_CODE = UDF_MANAGE_CODE"), strIs5163);
	strIs5163.Empty();
	TmpDM.GetOneValueQuery(strQuery, strIs5163);


	ReplaceMailData(strBody, _T("%도서관명%"), strLibCode);

	strQuery.Format(_T("SELECT NAME, USER_POSITION_CODE FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s' "), strUserName );
	TmpDM.RestructDataManager(strQuery);

	if ( strIs5163 == _T("Y") )
	{
		if ( !TmpDM.GetCellData(0, 0).IsEmpty() ) 
			ReplaceMailData(strBody, _T("%이용자명%"), TmpDM.GetCellData(0, 0));
		else
		{
			ReplaceMailData(strBody, _T("%이용자명%"), strUserName);
		}		

		CString strTmpData;
		strTmpData = TmpDM.GetCellData(0,1);

		if ( !strTmpData.IsEmpty() )
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), strTmpData, strTmpData);

		ReplaceMailData(strBody, _T("%대출자소속정보%"), strTmpData);
	}
	else 
		ReplaceMailData(strBody, _T("%이용자명%"), strUserName);
	
	ReplaceMailData(strBody, _T("%서명%"), strBookTitle);
	ReplaceMailData(strBody, _T("%저자%"), strAuthor);
	ReplaceMailData(strBody, _T("%발행자%"), strPublisher);
	ReplaceMailData(strBody, _T("%청구기호%"), strShelfCode);
	ReplaceMailData(strBody, _T("%예약일%"), strApplicantDate);
	ReplaceMailData(strBody, _T("%예약만기일%"), strReservationDate);
	
	
	ReplaceMailData(strBody, _T("\r\n"), _T("<br>"));
	ReplaceMailData(strBody, _T("\n"), _T("<br>"));

	
	return strBody ;
}

VOID CBO_LOC_1140::ReplaceMailData(CString &strMailBody, CString strName, CString strData)
{
	if ( strName.IsEmpty() ) return;

	strMailBody.Replace(strName, strData);

}

VOID CBO_LOC_1140::FreeMailMgr()
{

	if(m_pMailRecord)
    {
		delete m_pMailRecord;
		m_pMailRecord = NULL ;
	}


	if(m_pSendMailMgr)
	{
		delete m_pSendMailMgr ;
		m_pSendMailMgr = NULL ;
	}

}

VOID CBO_LOC_1140::OnchkCENTERUPLOADBC() 
{
	UC_KOLISNetUseChecker KolisnetUseChk( this );
	if ( KolisnetUseChk.GetUseFlag() == FALSE ) 	
	{
		((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(0);
	}	
}

INT CBO_LOC_1140::ReservationFurnishBook(CESL_DataMgr *pDM, INT nIndex, BOOL *ReservationYN)
{
	INT ids;
	CString sAcqCode , sAcqKey, sRecKey;
	CString strUserNo;
	CString strTemp;
	CString sFunishInfoUpdateSql;
	CString strBookKey, strSpeciesKey, Title, Author, VolInfo, ShelfLocCode;
	CString ClassNoType, ClassNo, ManageCode, RegCode, RegNo, SeprateShelfCode, Publisher, CallNo;
	CString UserKey, LoanUserPosition, LoanUserClass, MemberClass, Status, ReservationDate;
	CString sExpireDate;
	CString strQuery;
	CString strDelayCnt;
	CString strInferiorClass;
	CString sReservationYN;
	CString sRegCode;
	CString strValue;
	CString sSeprateShelfCode;
	CString sShelfLocCode;

	*ReservationYN = FALSE;

	CESL_DataMgr* pUserDM = FindDM( _T("DM_BO_LOC_3300") );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FindDM Failed") );
	
	INT nExpireDate;
	
	pUserDM->GetCellData(_T("대출직급정보")	, 0, LoanUserClass		);
	strQuery.Format(_T("SELECT RESERVATION_EXPIRE_DAY FROM CD_CODE_TBL WHERE CLASS = 31 and CODE='%s'"), LoanUserClass);	
	pDM->GetOneValueQuery( strQuery, sExpireDate );	

	pDM->GetCellData(_T("관리구분")		, nIndex, ManageCode	);
	nExpireDate = _ttoi(sExpireDate.GetBuffer(0));

	if ( ManageCode == _T("") )
	{
		sExpireDate.Format(_T("ECO$F_G_ISADDHOLIDAY$I(TRIM(SYSDATE), %d)+(23/24)+(59/24/60)+(59/24/60/60)"), nExpireDate);
	}
	else
	{
		sExpireDate.Format(_T("ECO$F_G_ISADDHOLIDAY$I(TRIM(SYSDATE), %d, '%s')+(23/24)+(59/24/60)+(59/24/60/60)"), nExpireDate, ManageCode);
	}	

	pDM->GetCellData(_T("IBS_청구_별치기호_코드")	, nIndex, SeprateShelfCode	);
	pDM->GetCellData(_T("책정보KEY")	, nIndex, strBookKey	);
	pDM->GetCellData(_T("종정보KEY")	, nIndex, strSpeciesKey );
	pDM->GetCellData(_T("서명")			, nIndex, Title			);
	pDM->GetCellData(_T("저작자")			, nIndex, Author		);
	
	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'")); 
	pDM->GetOneValueQuery(strQuery, strValue);
	if( _T("Y") == strValue )
	{
		pDM->GetCellData(_T("권일련번호")			, nIndex, VolInfo		);
	}
	pDM->GetCellData(_T("자료실")		, nIndex, ShelfLocCode	);
	pDM->GetCellData(_T("분류기호구분")	, nIndex, ClassNoType	);	
	pDM->GetCellData(_T("분류기호")		, nIndex, ClassNo		);
	pDM->GetCellData(_T("관리구분")		, nIndex, ManageCode	);
	pDM->GetCellData(_T("등록구분")		, nIndex, RegCode		);
	pDM->GetCellData(_T("등록번호")		, nIndex, RegNo			);	
	pDM->GetCellData(_T("발행자")		, nIndex, Publisher		);
	pDM->GetCellData(_T("청구기호")		, nIndex, CallNo		);	

	m_pInfo->pCodeMgr->ConvertDescToCode(_T("자료실구분"), ShelfLocCode,ShelfLocCode);
	
	pUserDM->GetCellData(_T("회원구분")		, 0, MemberClass		);	
	pUserDM->GetCellData(_T("대출소속정보")	, 0, LoanUserPosition	);
	pUserDM->GetCellData(_T("REC_KEY")		, 0, UserKey			);	
	pDM->MakeRecKey( sRecKey );		
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("BOOK_KEY"			) , _T("NUMERIC") , strBookKey);
	pDM->AddDBFieldData( _T("SPECIES_KEY"		) , _T("NUMERIC") , strSpeciesKey);
	pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE"	) , _T("STRING")  , _T("MO"));
	pDM->AddDBFieldData( _T("TITLE"				) , _T("STRING") , Title);
	pDM->AddDBFieldData( _T("AUTHOR"			) , _T("STRING") , Author);
    pDM->AddDBFieldData( _T("VOL_INFO"			) , _T("STRING") , VolInfo);
	pDM->AddDBFieldData( _T("SHELF_LOC_CODE"	) , _T("STRING") , ShelfLocCode);
	pDM->AddDBFieldData( _T("CLASS_NO_TYPE"		) , _T("STRING") , ClassNoType);
	pDM->AddDBFieldData( _T("CLASS_NO"			) , _T("STRING") , ClassNo);
    pDM->AddDBFieldData( _T("MANAGE_CODE"		) , _T("STRING") , ManageCode);
	pDM->AddDBFieldData( _T("REG_CODE"			) , _T("STRING") , RegCode);
	pDM->AddDBFieldData( _T("REG_NO"			) , _T("STRING") , RegNo);	
	pDM->AddDBFieldData( _T("PUBLISHER"			) , _T("STRING") , Publisher);
    pDM->AddDBFieldData( _T("CALL_NO"			) , _T("STRING") , CallNo);
	pDM->AddDBFieldData( _T("USER_KEY"			) , _T("NUMERIC") , UserKey);	
	pDM->AddDBFieldData( _T("MEMBER_CLASS"		) , _T("STRING") , MemberClass);
	pDM->AddDBFieldData( _T("STATUS"			) , _T("STRING") , _T("3"));
	pDM->AddDBFieldData( _T("RESERVATION_DATE"	) , _T("NUMERIC") , _T("SYSDATE"));	
	pDM->AddDBFieldData( _T("SEPARATE_SHELF_CODE") , _T("STRING") , SeprateShelfCode);
	pDM->AddDBFieldData( _T("USER_POSITION_CODE") , _T("STRING") , LoanUserPosition);
	pDM->AddDBFieldData( _T("USER_CLASS_CODE") , _T("STRING") , LoanUserClass);
	pDM->AddDBFieldData( _T("RESERVATION_EXPIRE_DATE") , _T("NUMERIC") , sExpireDate , TRUE );

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey);

	ids = pDM->MakeInsertFrame( _T("LS_WORK_T01") );
	if ( ids < 0 ) return ids;
	
	INT nReservationCnt;	
	CString strReservationCnt;
	pUserDM->GetCellData(_T("총예약책수"), 0, strReservationCnt );	
	nReservationCnt = _ttoi(strReservationCnt.GetBuffer(0));
	strReservationCnt.Format(_T("%d"), nReservationCnt+1);

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("RESERVATION_COUNT") , _T("NUMERIC") , strReservationCnt , TRUE );
	pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), UserKey );
	
	if ( m_strLibCode.IsEmpty() )
	{
		CString strWhere;
		strWhere = _T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");		

		pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
	}

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;

	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	

	
	CString sUpdateFields;
	sUpdateFields = _T("RESERVATION_COUNT");
				
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = UserKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;	

	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );	

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -13 , _T("ReservationFurnishBook") ); 

	*ReservationYN = TRUE;

	return 0;
}

INT CBO_LOC_1140::ReservationCheck(CESL_DataMgr *pDM, INT nIndex, CESL_DataMgr *pUserDM, BOOL *bReservationFlag)
{
	INT ids;
	CString sAcqCode , sAcqKey, sRecKey;
	CString strUserNo;
	CString strTemp;	
	CString strQuery;
	CString strDelayCnt;
	CString strInferiorClass;
	CString sReservationYN;
	CString sRegCode;	
	CString sSeprateShelfCode;
	CString sShelfLocCode;
	CString strValue;
	CString strUseObjectCode;
	CString sLoanClassCode;

	*bReservationFlag = TRUE;

	ids = pDM->GetCellData( _T("수입구분")		, nIndex, sAcqCode );
	if ( ids ) return 0;
	ids = pDM->GetCellData( _T("구입정보KEY")	, nIndex, sAcqKey );	
	if ( ids ) return 0;
	ids = pDM->GetCellData( _T("등록구분")		, nIndex, sRegCode );	
	if ( ids ) return 0;
	ids = pDM->GetCellData( _T("이용대상구분")		, nIndex, strUseObjectCode );	
	if ( ids ) return 0;
	ids = pDM->GetCellData( _T("IBS_청구_별치기호_코드")		, nIndex, sSeprateShelfCode);	
	if ( ids ) return 0;
	ids = pDM->GetCellData( _T("자료실코드")		, nIndex, sShelfLocCode);	
	if ( ids ) return 0;		

	pUserDM->GetCellData( _T("불량회원구분"), 0, strInferiorClass );
	pUserDM->GetCellData( _T("대출직급정보"), 0, sLoanClassCode);
	
	pUserDM->GetCellData( _T("대출자번호"), 0, strUserNo);		
	
	strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE STATUS||'' IN ('0', '2', 'L', 'D') ")
		_T("AND RETURN_PLAN_DATE < SYSDATE AND USER_KEY IN ")
		_T("( SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_NO='%s')"), strUserNo); 
	pUserDM->GetOneValueQuery(strQuery, strDelayCnt);

	if ( strInferiorClass != _T("0") || _ttoi(strDelayCnt) > 0 )
	{
		*bReservationFlag = FALSE;
		return 0;
	}

	CString strReservationCnt;
	pUserDM->GetCellData(_T("총예약책수"), 0, strReservationCnt );	
	
	strQuery.Format(_T("SELECT USEOBJCODE FROM CD_CODE_TBL WHERE CLASS=31 AND CODE = '%s'"), sLoanClassCode); 
	pUserDM->GetOneValueQuery(strQuery, strValue);

	if ( strUseObjectCode == _T("AD") && ( strValue==_T("") || strValue != _T("Y"))  ) 
	{
		*bReservationFlag = FALSE;
		return 0;
	}	

	strQuery.Format(_T("SELECT MAX(VALUE_1) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='수서' AND GROUP_3='단행' AND VALUE_NAME = '비치희망우선예약특별조건' AND MANAGE_CODE= UDF_MANAGE_CODE"));
	pUserDM->GetOneValueQuery(strQuery, strValue); 
	
	if(strValue != _T("Y")){
		strQuery.Format(_T("SELECT PREENGAGE_BOOK_CNT  FROM CD_CODE_TBL WHERE CLASS=31 AND CODE = '%s'"), sLoanClassCode); 
		pUserDM->GetOneValueQuery(strQuery, strValue);

		if ( strValue!=_T("") &&  ( _ttoi(strValue.GetBuffer(0)) <= _ttoi(strReservationCnt.GetBuffer(0))) )
		{
			*bReservationFlag = FALSE;
			return 0;
		}
	}

	strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 4 AND CODE='%s'"), sRegCode); 
	pUserDM->GetOneValueQuery(strQuery, strValue);
	if ( strValue != _T("Y") ) 
	{
		*bReservationFlag = FALSE;
		return 0;
	}


	if ( sSeprateShelfCode == _T("") )
	{
		strValue = _T("Y");
	}
	else
	{
		strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 11 AND CODE='%s'"), sSeprateShelfCode); 
		pUserDM->GetOneValueQuery(strQuery, strValue);
	}	
	
	if ( strValue != _T("Y") ) 
	{
		*bReservationFlag = FALSE;
		return 0;
	}

	strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE='%s'"), sShelfLocCode); 
	pUserDM->GetOneValueQuery(strQuery, strValue);
	if ( strValue != _T("Y") ) 
	{
		*bReservationFlag = FALSE;
		return 0;
	}	

	return 0;
}

BOOL CBO_LOC_1140::MakeUploadFile(CStringArray &pList)
{
	CStdioFile file;	
	CString		strLine;
	CString		strValue;	

	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	filename.Format(_T("Upload_tmp\\upload_%04d%02d%02d%02d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay(),t.GetHour(), t.GetMinute(), t.GetSecond());	
	
	if ( file.Open(filename, CFile::modeWrite | CFile::modeCreate  | CFile::typeBinary) )
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		for ( int i=0 ; i<pList.GetSize(); i++ )
		{			
			strLine.Format(_T("%s\n"), pList.GetAt(i));
			file.WriteString(strLine);			
		}
		file.Close();
				
		CString strCmd;
		CString strTemp;
		strTemp = m_pInfo->MANAGE_CODE;
				
		strCmd.Format(_T("CenterUploadManager.exe %s %s %s"), m_pInfo->USER_ID, m_pInfo->MANAGE_CODE, filename);		
		
		strCmd.Format(_T("CenterUploadManager.exe %s %s %s %s"), m_pInfo->USER_ID, m_pInfo->MANAGE_CODE, filename, m_pInfo->LOCAL_IP);

		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strCmd, -1, NULL, 0, NULL,NULL);
		char* ctmp = new char[len]; 
		WideCharToMultiByte(CP_ACP, 0, strCmd, -1, ctmp, len, NULL, NULL);
		::WinExec(ctmp, SW_SHOW);
		delete []ctmp;
	}

	return TRUE;
}

HBRUSH CBO_LOC_1140::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
