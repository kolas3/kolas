// MngLocMgr04Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr04Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr04Dlg dialog


CMngLocMgr04Dlg::CMngLocMgr04Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr04Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sDMAlias = _T("DM_MNG_LOC_MGR_04_DLG");
	m_sCMAlias = _T("CM_MNG_LOC_MGR_04_DLG");
	m_sGridAlias = _T("대출직급별환경설정그리드");
	m_sComboAlias = _T("이용대상구분");

	m_pGridEdt = NULL;

	m_bFirstReturn = TRUE;

	m_pLoanShareManager = pLoanShareManager;

	m_bIsCopied = FALSE;

	// 2005.05.26
	m_strUserID=pParent->GetUserID();
	m_strLibCode=_T("");
}
CMngLocMgr04Dlg::~CMngLocMgr04Dlg()
{
	
}

VOID CMngLocMgr04Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr04Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr04Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr04Dlg)
	ON_WM_SIZE()
	ON_CBN_EDITCHANGE(IDC_cmbUSE_OBJ_CODE, OnEditchangecmbUSEOBJCODE)
	ON_CBN_SELCHANGE(IDC_cmbUSE_OBJ_CODE, OnSelchangecmbUSEOBJCODE)
	ON_MESSAGE(UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_COMMAND_RANGE(4000,4001,CopyAndPasteProc)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr04Dlg message handlers

BOOL CMngLocMgr04Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMngLocMgr04Dlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return;
	
	// 2006-03-15 UPDATE BY PDW(JR) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 메세지 스태틱박스 추가	
	const INT nCnt = 5;
	CWnd* pWnd[nCnt];
	CRect WndPos[nCnt];
	INT nIDC[nCnt] = 
	{
		IDC_staTITLE1 , IDC_gridPOSITION , IDC_staTITLE2 , IDC_staTITLE3 , IDC_gridPOSITION2
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pWnd[i] = GetDlgItem( nIDC[i] );
		if( pWnd[i]->GetSafeHwnd() == NULL ) return;
		pWnd[i]->GetWindowRect(WndPos[i]);
		ScreenToClient(WndPos[i]);
	}
	
	pWnd[0]->MoveWindow( 10 , 3 , cx-20 , WndPos[0].Height() );
	pWnd[1]->MoveWindow( 10 , WndPos[0].Height()+3 , cx-20 , WndPos[2].top-WndPos[0].Height()-3-5 );
	pWnd[2]->MoveWindow( 10 , WndPos[2].top , cx-20 , WndPos[2].Height() );
	pWnd[3]->MoveWindow( 10 , WndPos[3].top , cx-20 , WndPos[3].Height() );
	pWnd[4]->MoveWindow( 10 , WndPos[3].bottom + 1 , cx-20 , cy - WndPos[3].bottom - 30  );
	// --------------------------------------------------------------------------------------
}


INT CMngLocMgr04Dlg::ViewScreen()
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_MNG_LOC_MGR_04_DLG");
	CString sCMAlias = _T("CM_MNG_LOC_MGR_04_DLG");
	CString sCMAlias2 = _T("CM_MNG_LOC_MGR_04_2_DLG");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ViewScreen") );

//	ids = pDM->RefreshDataManager(_T(""));
	ids = CLocCommonAPI::RefreshDataManager( this , pDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("ViewScreen") ); 

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-3 , _T("ViewScreen") ); 
	pCM->AllControlDisplay();
	
	CESL_ControlMgr *pCM2 = FindCM( sCMAlias2 );
	if( pCM2 == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-3 , _T("ViewScreen") ); 
	pCM2->AllControlDisplay();

	// 대출 직급별 등록구분별 환경설정
	ClassRegCodeView();

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr04Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);

	CString sValue;
	CString sRecKey;

	//이동도서관용
	CString strTL_SetData;
	CString strTL_Condition;
	CString strTempData;

	// 2009.02.25 ADD BY PWR : 이동대출가능책수 추가
	const INT nCnt = 12;

	TCHAR *sDBColumnName[nCnt] =
	{
		_T("USEOBJCODE") ,
		_T("LOAN_PERIOD") , 
		_T("POSTPONEMENT_PERIOD") ,
		_T("UNITY_LOAN_BOOK_CNT") ,
		_T("자관대출책수") , 
		_T("LOAN_BOOK_CNT") , 
		_T("OTHER_LOAN_BOOK_CNT") , 
		_T("TL_LOAN_BOOK_CNT") , 
		_T("RESERVATION_EXPIRE_DAY") , 
		_T("PREENGAGE_BOOK_CNT") ,
		_T("PREENGAGE_CANCEL_LIMIT_PERIOD") ,
		_T("PREENGAGE_CANCEL_LIMIT_COUNT")
	};

	TCHAR *sColumnType[nCnt] =
	{
		_T("STRING") ,
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") , 
		_T("NUMERIC") ,
		_T("NUMERIC") ,
		_T("NUMERIC") , 
		_T("NUMERIC")
	};

	TCHAR *sAliasName[nCnt] =
	{
		_T("이용대상구분") ,
		_T("대출기한") , 
		_T("대출연장기간") , 
		_T("통합대출가능책수") , 
		_T("자관대출가능책수") , 
		_T("도서대출가능책수") , 
		_T("타관대출가능책수") , 
		_T("이동대출가능책수") , 
		_T("예약만기일수") ,
		_T("예약가능책수") ,
		_T("예약취소제한적용기간") , 
		_T("예약취소제한갯수")
	};

	// 2005.05.26 ADD BY PDJ
	if ( m_strLibCode.IsEmpty() )
	{
		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// 		CString	strWhere;
// 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
// 			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// 						_T("WHERE ID = '%s')"), m_strUserID );
// 		m_pDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
		m_strLibCode = m_pInfo->LIB_CODE;
	}

	// 직급별 반납연기 정보 변경시 센터 도서관에 알리기 
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	// Mobile API 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , m_pDM );
	pMobileApi->StartFrame();

	
	CString strUseObjectCode;				
	CString strLoanPeriod;
	CString strPostponementPeriod;
	CString strLoanBookCnt;
	CString strPreengageBookCnt; // 5	
	CString strReservationExpireDay;	
	CString strPreenageCancelLimitCount;
	CString strPreenageCancelLimitPeriod;
	CString strOtherLoanBookCnt; // 10
	CString strUnityLoanBookCnt;
	CString strTLLoanBookCnt;
	CString strValue;
	CString strCode;

	for( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ )
	{
		pMobileApi->InitDBFieldData();

		strTL_SetData = _T("");
		m_pDM->GetCellData( _T("REC_KEY") , i , sRecKey );

		BOOL bEdit = FALSE;
		for( INT j = 1 ; j < nCnt+1 ; j++ )
		{
			if(j==5) continue; //자관대출책수는 테이블이 따로 있기때문에 pass~
			//이동도서관일 경우
			if( m_pInfo->MODE == 10000 )
			{
				if((j!=2)&&(j!=3)&&(j!=8)) continue;
			}
			sValue.Empty();
			m_pGrid->GetAt( i , j , sValue );
			sValue.TrimRight(); sValue.TrimLeft();

			if( (CString(_T("NUMERIC")).Compare(sColumnType[j-1])==0) && sValue.IsEmpty() )
			{
				sValue = _T("0");
			}

			// 2009.07.01 ADD BY PWR : 수정한것만 UPDATE하도록한다.
			CString sValue2;
			m_pDM->GetCellData(sAliasName[j-1], i, sValue2);
			sValue2.TrimRight(); sValue2.TrimLeft();
			if(sValue2.IsEmpty())
			{
				sValue2 = _T("0");
			}
			if(sValue.IsEmpty())
			{
				sValue = _T("0");
			}
			if(sValue == sValue2)
			{
				continue;
			}
			bEdit = TRUE;			

			ids = pMobileApi->AddDBFieldData( sDBColumnName[j-1] , sColumnType[j-1] , sValue );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
			
			//이동도서관일 경우
			if( m_pInfo->MODE == 10000 )
			{
				if(!strTL_SetData.IsEmpty())
				{
					strTL_SetData+=_T(",");
				}
				if(!sValue.IsEmpty())
				{
					strTempData.Format(_T("%s=%s"),sDBColumnName[j-1],sValue);
					strTL_SetData +=strTempData;
				}
			}
		}
		if(bEdit == TRUE)
		{
			ids = pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("대출환경설정"), __WFILE__, __LINE__ ), TRUE );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
			ids = pMobileApi->MakeUpdateFrame( _T("CD_CODE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );

			// 공통 반납연기 정보 변경시 센터 도서관에 알리기 	
			UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
			UploadShareInfo.UploadShareLoanInfo.sCommand = _T("61");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CD_CODE_TBL");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("");
			UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
			m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		}
		//JOB.2019.0073
		/*
		//이동도서관일 경우
		if( m_pInfo->MODE == 10000 )
		{
			if(ids>=0)
			{
				if(!strTL_SetData.IsEmpty())
				{
					strTL_Condition.Format(_T("REC_KEY=%s"),sRecKey);
					m_syncApi.RefreshSyncData(_T("CD_CODE_TBL"),strTL_SetData,strTL_Condition);
				}
			}
		}
		*/
		//=============================================
		//2009.02.20 REM BY LKS : 이동도서관은 별도관리
		//CLocCommonAPI::MobileApiEndLog( pMobileApi );
		//=============================================
	}

	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveScreen") );
	pMobileApi->EndFrame();

	strTL_SetData.Empty();
	strTL_Condition.Empty();

	//++2008.12.09 ADD BY PWR {{++
	// 자관대출책수 UPDATE : 도서관별로 따로 테이블이 존재하므로 
	INT nTmp = m_pDM->GetRowCount();
	CString strTmp;
	// Mobile API 적용
	//CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this, m_pDM );
	pMobileApi->StartFrame();
	for( i = 0 ; i < m_pDM->GetRowCount() ; i++ )
	{
		strTmp.Format(_T("%d"),i);
		if(i==27)
		{
			int a=0;
		}
		TRACE(strTmp);
		m_pDM->GetCellData( _T("자관KEY"), i, sRecKey );
		if(sRecKey.IsEmpty()) continue;
		
		pMobileApi->InitDBFieldData();
		
		sValue.Empty();
		m_pGrid->GetAt( i , 5, sValue );
		sValue.TrimRight(); sValue.TrimLeft();
		if( sValue.IsEmpty() ) sValue = _T("0");

		// 2009.07.01 ADD BY PWR : 수정한것만 UPDATE하도록한다.
		CString sValue2;
		m_pDM->GetCellData(_T("자관대출가능책수"), i, sValue2);
		sValue2.TrimRight(); sValue2.TrimLeft();
		if( sValue2.IsEmpty() ) sValue2 = _T("0");

		if(sValue == sValue2)
		{
			continue;
		}

		ids = pMobileApi->AddDBFieldData( _T("LOAN_BOOK_CNT") , _T("NUMERIC") , sValue );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
		ids = pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("대출환경설정"), __WFILE__, __LINE__ ), TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );
		ids = pMobileApi->MakeUpdateFrame( _T("CD_INDIVIDUAL_CLASS_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );

		//JOB.2019.0073
		/*
		//이동도서관일 경우
		if( m_pInfo->MODE == 10000 )
		{
			if(ids>=0)
			{
				if(sValue.IsEmpty()) sValue = _T("0");
				strTL_SetData.Format(_T("LOAN_BOOK_CNT=%s"),sValue);
				strTL_Condition.Format(_T("REC_KEY=%s AND MANAGE_CODE='UDF_MANAGE_CODE'"),sRecKey);
				m_syncApi.RefreshSyncData(_T("CD_INDIVIDUAL_CLASS_TBL"),strTL_SetData,strTL_Condition);
			}
		}
		*/
		// Mobile API delete
		//=============================================
		//2009.02.20 REM BY LKS : 이동도서관은 별도관리
		//CLocCommonAPI::MobileApiEndLog( pMobileApi );
		//=============================================
	}
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveScreen") );
	m_pDM->EndFrame();

	// 등록구분별 직급별 환경 설정
	ids = SaveRegClassValue();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveScreen") );

	LockWindowUpdate();
	ViewScreen();
	UnlockWindowUpdate();

	return 0;

EFS_END
return -1;
}

BOOL CMngLocMgr04Dlg::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids;
	ids = InitESL_Mgr(_T("SM_MNG_LOC_MGR_04_DLG"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("OnInitDialog") );

	m_pDM = FindDM( m_sDMAlias );
	if( m_pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1010,_T("OnInitDialog"));

	m_pCM = FindCM( m_sCMAlias );
	if( m_pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1020,_T("OnInitDialog"));

	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);
	// 2009.02.25 UPDATE BY PWR : 이동대출가능책수 추가( 이동은 이동만 볼수있게한다. )
	if( m_pInfo->MODE == 10000 )
	{
		// 다른대출가능책수는 HIDE
		m_pGrid->SetColWidth(5, 0, 0);
		m_pGrid->SetColWidth(6, 0, 0);
		m_pGrid->SetColWidth(7, 0, 0);
		m_pGrid->SetColWidth(8, 0, 0);
	}
	else
	{
		// 2008.12.09 ADD BY PWR : 통합대출권수사용여부에 따른 처리
		CString sValue;
		if( GetManageValue( _T("열람"), _T("공통"), _T("통합대출자"), _T("통합대출권수사용여부"), sValue ) < 0 )
			ERROR_MSG_RETURN_ERROR_CODE2(-1030,_T("OnInitDialog"));
		sValue.TrimLeft( ); sValue.TrimRight( );
		if ( sValue.IsEmpty() ) sValue = _T("N");
		if( sValue == _T("Y") )
		{ // 사용함
			// 통합대출가능책수, 자관대출가능책수(도서관별) SHOW
			// 자관대출가능책수, 타관대출가능책수 HIDE
			m_pGrid->SetColWidth(7, 0, 0);
			m_pGrid->SetColWidth(8, 0, 0);
		}
		else
		{ // 사용안함
			// 자관대출가능책수, 타관대출가능책수 SHOW
			// 통합대출가능책수, 자관대출가능책수(도서관별) HIDE
			m_pGrid->SetColWidth(5, 0, 0);
			m_pGrid->SetColWidth(6, 0, 0);
		}
		//이동대출가능책수 HIDE
		m_pGrid->SetColWidth(9, 0, 0);
	}
	
	//JOB.2019.0073
	//// 이동도서관 설정값 동기화 Class DB설정
	//m_syncApi.SetSyncDBInfo_TL(m_pInfo->CONNECTION_INFO,1);

	// 18.09.27 JMJ KOL.RED.2018.008
	// 책두레운영모드 관리값 추가
	CString strValue;
	ids = GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레사용여부"), strValue);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("OnInitDialog"));
	}
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y") == strValue) 
	{
		ids = GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레운영모드"), strValue);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("OnInitDialog"));
		}
		strValue.TrimLeft(); strValue.TrimRight();
		if(strValue.IsEmpty())
		{
			m_sBookCooperativeMode = _T("I");
		}
		else
		{
			m_sBookCooperativeMode = strValue;
		}
	}
	else
	{
		m_sBookCooperativeMode = _T("I");
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BEGIN_EVENTSINK_MAP(CMngLocMgr04Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr04Dlg)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION, -600 /* Click */, OnClickgridPOSITION, VTS_NONE)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION, -601 /* DblClick */, OnDblClickgridPOSITION, VTS_NONE)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION, -604 /* KeyUp */, OnKeyUpgridPOSITION, VTS_PI2 VTS_I2)
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION2, -600 /* Click */, OnClickgridCLASSREG, VTS_NONE)
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION2, -601 /* DblClick */, OnDblClickgridCLASSREG, VTS_NONE)	
	ON_EVENT(CMngLocMgr04Dlg, IDC_gridPOSITION2, -604 /* KeyUp */, OnKeyUpgridCLASSREG, VTS_PI2 VTS_I2)
	// -----------------------------------------------------------------------------------------------
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngLocMgr04Dlg::OnClickgridPOSITION() 
{
EFS_BEGIN

//	m_pGrid->SetReverse( m_pGrid->GetIdx() );
//	FreeGridEdt();
	m_bFirstReturn = FALSE;
	// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++
	// 그리드 포인터가 유동적으로 변경된다.
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);
	// ---------------------------------------------------------------

EFS_END
}

// #########################################################################################
// [ 함수설명 ]
//	 기한설정 그리드의 선택 컬럼에 따른 인벤트 구분
// [ 미니스펙 ]
//
// #########################################################################################
VOID CMngLocMgr04Dlg::InputDataToGrid()
{
EFS_BEGIN

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	CString sValue = m_pGrid->GetText();
	
	// 2006-03-14 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// 환경설정 위치에 따라 틀리다.
	// 직급별 설정에서는 성인자료 대출여부 설정이 존재한다.
	/*
	if( !nRow )
		return;

	switch( nCol )
	{
		case 2 :		// 이용대상구분-성인자료대출여부
			SetYNCheck( sValue, nRow, nCol );
			break;
		case 3 :		// 대출기간
		case 4 :		// 반납연기일수
		case 5 :		// 대출가능책수
		case 6 :		// 예약유효일수
		case 7 :		// 예약가능책수
			GetGridCellNewValue( sValue, nRow, nCol );
			break;
		default :
			break;
	}
	*/
	if ( m_pGrid == (CESL_Grid*)GetDlgItem(IDC_gridPOSITION) )
	{
		// 헤더를 선택하거나 첫 컬럼을 설택하면 생성해서는 안된다.
		if ( !nRow || nCol == 1 )	return;

		// 2009.02.25 UPDATE BY PWR : 이동도서관 처리
		if( nCol == 2 ) 
		{ // 성인자료대출여부 설정을 한다.
			SetYNCheck( sValue, nRow, nCol );
		}
		if( m_pInfo->MODE == 10000 )
		{ // 이동도서관일때
			if( (nCol >= 3 && nCol <= 5) || nCol == 9 )
			{ // 직접 입력할 수 있는 입력창을 만든다.
				 GetGridCellNewValue( sValue, nRow, nCol );
			}
		}
		else
		{
			if( nCol >= 3 && nCol <= 13 && nCol != 9 )
			{ // 직접 입력할 수 있는 입력창을 만든다.
				 GetGridCellNewValue( sValue, nRow, nCol );
			}
		}
		//// 성인자료대출여부 설정을 한다.
		//if ( nCol == 2 ) SetYNCheck( sValue, nRow, nCol );
		//// 직접 입력할 수 있는 입력창을 만든다.
		//else if ( nCol >= 3 && nCol <= 13 ) GetGridCellNewValue( sValue, nRow, nCol );
	}
	// 직급별등록구분별 설정
	else if ( m_pGrid == (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2) )
	{
		// 헤더가 두줄이다.
		if ( nRow < 2 )			return;

		GetGridCellNewValue( sValue, nRow, nCol );				
	}
	// -------------------------------------------------------------------------
EFS_END
}

// #########################################################################################
// [ 함수설명 ]
//	 선택된 셀에 새로운 값을 입력할 수 있는 입력창을 생성한다.
// [ 미니스펙 ]
//
// #########################################################################################
VOID CMngLocMgr04Dlg::GetGridCellNewValue(CString value, INT row, INT col)
{
EFS_BEGIN
		
	// ===================================================================================
	// 1. 초기화
	// ===================================================================================
	//2004-12-20
	//modified by loveisverb
	FreeGridEdt();

	if( m_bFirstReturn ) 
		m_bFirstReturn = FALSE;
	else
	{
		m_bFirstReturn = TRUE;
		return;
	}

	// ===================================================================================
	// 2. 입력창 생성
	// ===================================================================================

	// 2006-03-14 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 직급별 설정과 직급별등록구분별 설정이 같이 사용하게 변경 되었다.
	/*
	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_gridPOSITION );
	*/
	CWnd* pWnd = ( CWnd* )m_pGrid;
	// -----------------------------------------------------------------------------------
	m_pGridEdt = new CGridEdt;	
	m_pGridEdt->row = row;
	m_pGridEdt->col = col;
	m_pGridEdt->parent = ( CWnd * )this;
	m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_UPPERCASE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );

	// ===================================================================================
	// 3. 생성된 입력창의 위치 및 크기 조정
	// ===================================================================================
	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top  -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	// ===================================================================================
	// 4. 입력창으로 포커스 전달
	// ===================================================================================
	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	m_pGridEdt->SetWindowText( value );
    m_pGridEdt->SetSel( 0, -1 );
	
	// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	// 직급별등록구분별에서는 'N' 설정이 가능하도록 수정
	if ( m_pGrid == (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2) && ( col%2 ) )
		m_pGridEdt->m_bUseNotMode = TRUE;	
	// -----------------------------------------------------------------------------------

EFS_END
}

VOID CMngLocMgr04Dlg::FreeGridEdt()
{
	EFS_BEGIN

	if( m_pGridEdt != NULL )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}

	EFS_END
}

VOID CMngLocMgr04Dlg::SetGridText(INT row, INT col, CString text)
{
	EFS_BEGIN

	INT curRow = m_pGrid->GetRow();
	INT curCol = m_pGrid->GetCol();

	m_pGrid->SetRow( row );
	m_pGrid->SetCol( col );
	m_pGrid->SetText( text );

	m_pGrid->SetRow( curRow );
	m_pGrid->SetCol( curCol );

	EFS_END
}

VOID CMngLocMgr04Dlg::OnUpdateGridCelText(WPARAM w, LPARAM l)
{
	EFS_BEGIN

	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );
		// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// N과 다른 키입력을 받는다면 N으로 고정 시킨다.
		if ( newValue.Replace( _T("N"), _T("") ) > 0 )
			newValue = _T("N");
		// ---------------------------------------------------------------------------------
		SetGridText( m_pGridEdt->row, m_pGridEdt->col, newValue );
	}
	FreeGridEdt();
	m_pGrid->SetFocus();


	EFS_END
}


INT CMngLocMgr04Dlg::SetYNCheck( CString sValue , INT nRow , INT nCol )
{
	EFS_BEGIN

	if( sValue.Compare(_T("Y"))==0 )
		sValue = _T("N");
	else 
		sValue = _T("Y");

	m_pGrid->SetAt( nRow-1 , nCol-1 , sValue );

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgr04Dlg::ViewMultiCombo(CString sValue, INT nRow, INT nCol )
{
	EFS_BEGIN

	CString sCode;
	if( sValue.GetLength() )
		sCode = GetCodeFromDM( nRow - 1 );	
	
	CRect rect = GetGridCellRect();

	m_pGrid->EnableWindow( false );
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSE_OBJ_CODE );
	INT rowCount = pCombo->GetItemHeight( 0 );
	pCombo->MoveWindow( rect.left - 2, rect.top - 3, rect.Width() + 2, rect.Height()  );

	pCombo->ShowWindow( true );
	pCombo->SetFocus();

	if( sCode.GetLength() )
		SetControlData( m_sCMAlias, _T("이용대상구분"), sCode );

	EFS_END
}

CString CMngLocMgr04Dlg::GetCodeFromDM(INT dmRow)
{
	EFS_BEGIN

	CString sCode;
	m_pDM->GetCellData( _T("이용대상구분"), dmRow, sCode );

	return sCode;

	EFS_END
	return _T("");

}

CRect CMngLocMgr04Dlg::GetGridCellRect()
{
	CWnd *wnd = ( CWnd * )GetDlgItem( IDC_gridPOSITION );

	CRect rect, gridRect;
	RECT parentRect;
	wnd->GetWindowRect( &gridRect );
	wnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( wnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left	= gridRect.left +	( long )( ( float )m_pGrid->GetCellLeft()		/ 15.0f );
	rect.top	= gridRect.top +	( long )( ( float )m_pGrid->GetCellTop()		/ 15.0f );
	rect.right	= rect.left +		( long )( ( float )m_pGrid->GetCellWidth()	/ 15.0f );
	rect.bottom = rect.top +		( long )( ( float )m_pGrid->GetCellHeight()	/ 15.0f );

	return rect;
}

VOID CMngLocMgr04Dlg::OnEditchangecmbUSEOBJCODE() 
{
EFS_BEGIN
EFS_END
}


CString CMngLocMgr04Dlg::TakeControlData(CString sAlias, INT nOption)
{
	EFS_BEGIN

	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString sResult;

	INT ids = 0;
	ids = GetControlData( m_sCMAlias, sAlias , sResult, -1, nOption );
	if(0 > ids)
	{
		MessageBox( _T("Error : GetControlData() of CM_부록예측"), _T("K2Up") );
		return _T("");
	}
	
	// 코드값이 _T("0") 이거나 설명값이 _T("적용안함")일때 _T("")를 반환한다. 
	switch( nOption )
	{
		case 0 :
			if( _T("0") == sResult )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == sResult )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_부록예측"), _T("K2Up") );
			return _T("");
			break;
	}

	return sResult;

	EFS_END
	return _T("");

}

VOID CMngLocMgr04Dlg::OnSelchangecmbUSEOBJCODE() 
{
	EFS_BEGIN

	CString sValue = TakeControlData( _T("이용대상구분") , 0);
	
	INT nRow = m_pGrid->GetRow();
	
	m_pDM->SetCellData( _T("이용대상구분코드") , sValue , nRow-1 );

	sValue = sValue + _T(" : ") + TakeControlData( _T("이용대상구분"), 1 );
	m_pGrid->SetText( sValue );

	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSE_OBJ_CODE );
	pCombo->ShowWindow( false );	
	
	m_pGrid->EnableWindow( true );
	m_pGrid->SetFocus();				

	EFS_END
}

VOID CMngLocMgr04Dlg::OnDblClickgridPOSITION() 
{
EFS_BEGIN
	
	// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++
	// 그리드 포인터가 유동적으로 변한다.
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION);
	// --------------------------------------------------------------------
	INT nCol = m_pGrid->GetCol();
	
	m_bFirstReturn = TRUE;
	InputDataToGrid();
	
	m_pGrid->SetCol( nCol );

EFS_END
}

VOID CMngLocMgr04Dlg::OnKeyUpgridPOSITION(short FAR* KeyCode, short Shift) 
{
EFS_BEGIN

	/*
	if( *KeyCode == 13 )
	{
		INT nCol = m_pGrid->GetCol();

		InputDataToGrid();
		
		m_pGrid->SetCol( nCol );
		
	}
	*/

EFS_END
}

BOOL CMngLocMgr04Dlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN


	if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB)
	{
		//2004.12.16
		// 리턴키를 누를경우 InputDataToGrid를 또 호출하지 않고 WM_LBUTTONDBLCLK 메시지를 보낸다.
		//InputDataToGrid();
		PostMessage(WM_LBUTTONDBLCLK,NULL,NULL);
		return TRUE;
	}


	// 2004.12.16
	// modified by loveisverb
	// 그리드에서 마우스 오른쪽 버튼을 이용해 copy & paste 지원 
	if( pMsg->message == WM_RBUTTONUP )
	{
		HWND MsgH = pMsg->hwnd;
		
		if( GetDlgItem(IDC_gridPOSITION)->m_hWnd==MsgH )
		{
			ShowCopyAndPasteMenu(pMsg->pt);
			return TRUE;
		}

		
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

INT CMngLocMgr04Dlg::ClassRegCodeView()
{
	EFS_BEGIN

	INT ids;

	ids = ClassRegCodeGridInit();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeView") );

	ids = ClassRegCodeShowValue();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ClassRegCodeView") );
	

	return 0;


	EFS_END

	return -1;
}

// #########################################################################################
// [ 함수설명 ]
//   직급별등록구분별 그리드 초기화
// [ 미니스펙 ]
// 
// #########################################################################################
INT CMngLocMgr04Dlg::ClassRegCodeGridInit()
{
EFS_BEGIN

	INT ids;
	
	// ==================================================================================
	// 1. DM 초기화
	// ==================================================================================
	// 등록구분을 가져온다.
	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
	if( pRegCodeDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeGridInit") );
	ids = CLocCommonAPI::RefreshDataManager( this , pRegCodeDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ClassRegCodeGridInit") );
	
	// 직급구분을 가져온다.
	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
	if( pClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ClassRegCodeGridInit") );
	ids = CLocCommonAPI::RefreshDataManager( this , pClassDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ClassRegCodeGridInit") );

	// 환경값을 가져온다.
	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
	if( pEnvDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ClassRegCodeGridInit") );
	ids = CLocCommonAPI::RefreshDataManager( this , pEnvDM , _T("") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ClassRegCodeGridInit") );	
	
	// ==================================================================================
	// 2. 그리드 셋팅
	// ==================================================================================
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG") , _T("대출환경설정"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ClassRegCodeGridInit") );
	
	// 2006-03-14 REMOVE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 기본 입력창으로 생성하지 않는다.
	/*
	pGrid->m_bGridEdit = TRUE;
	*/
	// ----------------------------------------------------------------------------------

	INT nCols = pRegCodeDM->GetRowCount()*2 + 1;
	pGrid->SetCols(  0 , nCols );
	pGrid->SetFixedCols(1);
	pGrid->SetMergeCells(1);
//	pGrid->SetMergeCol(1,TRUE);
	pGrid->SetMergeRow(0,TRUE);
	
	// 헤더 셋팅( 등록구분 )
	CString sColumnName;
	for( INT i = 0 ; i < pRegCodeDM->GetRowCount() ; i++ )
	{
		ids = pRegCodeDM->GetCellData( _T("등록구분") , i , sColumnName );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ClassRegCodeGridInit") );
		
		pGrid->SetTextMatrix( 0 , i*2+1 , sColumnName );
		pGrid->SetTextMatrix( 0 , i*2+2 , sColumnName );
		pGrid->SetTextMatrix( 1 , i*2+1 , _T("대출기한") );
		pGrid->SetTextMatrix( 1 , i*2+2 , _T("반납연기일수") );
		pGrid->SetColWidth( i*2+1 , 0 , 900);
		pGrid->SetColWidth( i*2+2 , 0 , 1200);
		pGrid->SetCol(i*2+1);
		pGrid->SetRow(0);
		pGrid->SetCellAlignment(4);
		pGrid->SetCellFontBold(TRUE);			
	}
	
	// 직급 및 값 셋팅 
	INT nRows = pClassDM->GetRowCount() + 2;
	pGrid->SetRows(nRows);
	pGrid->SetFixedRows(2);
	pGrid->SetColWidth( 0 , 0 , 900);
	for( i = 0 ; i < pClassDM->GetRowCount() ; i++ )
	{
		ids = pClassDM->GetCellData( _T("직급") , i , sColumnName );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ClassRegCodeGridInit") );
		
		pGrid->SetTextMatrix( i+2 , 0 , sColumnName );
		pGrid->SetCol(0);
		pGrid->SetRow(i+2);
		pGrid->SetCellFontBold(TRUE);
	}
	return 0;

EFS_END
	return -1;		
}


INT CMngLocMgr04Dlg::ClassRegCodeShowValue()
{
	EFS_BEGIN


	INT ids;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG") , _T("대출환경설정"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeShowValue") );
	
	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
	if( pEnvDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("ClassRegCodeShowValue") );
	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
	if( pRegCodeDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ClassRegCodeGridInit") );
	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
	if( pClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ClassRegCodeGridInit") );
	

	CString sLoanPeriod , sDelayDay;
	CString sRegKey , sClassKey;
	CString sColumn;
	INT nColIndex = -1;
	INT nRowIndex = -1;

	INT nTmp = pEnvDM->GetRowCount();
	for( INT i = 0 ; i < pEnvDM->GetRowCount() ; i++, nColIndex = -1, nRowIndex = -1 )
	{
		ids = pEnvDM->GetCellData( _T("대출기한") , i , sLoanPeriod );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ClassRegCodeShowValue") );
		ids = pEnvDM->GetCellData( _T("반납연기일수") , i , sDelayDay );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ClassRegCodeShowValue") );
		ids = pEnvDM->GetCellData( _T("등록구분KEY") , i , sRegKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ClassRegCodeShowValue") );
		ids = pEnvDM->GetCellData( _T("직급KEY") , i , sClassKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ClassRegCodeShowValue") );

		nTmp = pRegCodeDM->GetRowCount();
		for( INT j = 0 ; j < pRegCodeDM->GetRowCount() ; j++ )
		{
			ids = pRegCodeDM->GetCellData( _T("등록구분KEY") , j , sColumn );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ClassRegCodeShowValue") );

			if( sColumn.Compare(sRegKey) == 0 )
				nColIndex = (j*2)+1;
		}

		nTmp = pClassDM->GetRowCount();
		for( j = 0 ; j < pClassDM->GetRowCount() ; j++ )
		{
			ids = pClassDM->GetCellData( _T("직급KEY") , j , sColumn );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ClassRegCodeShowValue") );
			
			if( sColumn.Compare(sClassKey) == 0 )
				nRowIndex = j+2;
		}

		if(0 < nRowIndex && 0 < nColIndex)
		{
			pGrid->SetTextMatrix( nRowIndex , nColIndex , sLoanPeriod );
			pGrid->SetTextMatrix( nRowIndex , nColIndex+1 , sDelayDay );
		}
	}



	return 0;

	EFS_END
	
	return -1;

}

// #########################################################################################
// [ 함수설명 ]
//   직급별등록구분별 설정값 DB 반영
// [ 미니스펙 ]
// 
// #########################################################################################
INT CMngLocMgr04Dlg::SaveRegClassValue()
{
EFS_BEGIN

	INT ids;

	// 1. 초기화
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG"), _T("대출환경설정"));
	if(NULL == pGrid)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SaveRegClassValue"));
	}
	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
	if(NULL == pEnvDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("SaveRegClassValue"));
	}
	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
	if(NULL == pRegCodeDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("SaveRegClassValue"));
	}
	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
	if(NULL == pClassDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("SaveRegClassValue"));
	}
	if(m_strLibCode.IsEmpty())
	{
		m_strLibCode = m_pInfo->LIB_CODE;
	}

	INT nCols = pGrid->GetCols(0);
	INT nRows = pGrid->GetRows();

	CString sRecKey, sRegKey, sClassKey, strLoanPeriod_CM, strDelayDay_CM, sValue;

	// 이동도서관용 변수 선언
	CString strTL_SetData;
	CString strTL_Condition;

	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(this, pEnvDM);
	pMobileApi = CLocCommonAPI::InitMobileApi(this, pEnvDM);
	pMobileApi->StartFrame();

	// 그리드에 입력된 값 가져와서 비교
	for(INT i = 2 ; i < nRows; i++) // 직급
	{
		ids = pClassDM->GetCellData(_T("직급KEY"), i-2, sClassKey);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("SaveRegClassValue"));
		}

		for(INT j = 1; j < nCols; j++) //등록구분
		{
			strTL_SetData.Empty();
			strTL_Condition.Empty();

			ids = pRegCodeDM->GetCellData( _T("등록구분KEY"), (j-1)/2, sRegKey);
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("SaveRegClassValue"));
			}
	
			// 대출기간
			sValue = pGrid->GetTextMatrix(i, j);
			sValue.TrimLeft(); sValue.TrimRight();
			if(!sValue.IsEmpty())
			{
				strLoanPeriod_CM.Format( _T("%d"), _ttoi(sValue));
			}
			else
			{
				strLoanPeriod_CM = _T("");
			}
			// 반납연기일수
			sValue = pGrid->GetTextMatrix(i, j+1);
			sValue.TrimLeft() ; sValue.TrimRight();
			if(!sValue.IsEmpty())
			{
				strDelayDay_CM.Format(_T("%d"), _ttoi(sValue));
			}
			else 			
			{
				strDelayDay_CM = _T("");
			}

			// 그리드에 등록구분별 설정이 그리드에만 있는 내용인지 DB에도 있는 내용인지 확인한다.
			BOOL bIsFindDM = FALSE; // DB(MN_LOAN_ENV_POSREG_TBL)에 있는지 여부 
			sRecKey.Empty();
			CString strEnvRecKey, strEnvClassKey, strEnvRegKey;
			INT nEnvCnt = 0;
			const INT ENV_CNT = pEnvDM->GetRowCount(); // 실제 MN_LOAN_ENV_POSREG_TBL값들
			for(nEnvCnt = 0; nEnvCnt < ENV_CNT; nEnvCnt++)
			{
				ids = pEnvDM->GetCellData(_T("REC_KEY"), nEnvCnt, strEnvRecKey);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("SaveRegClassValue"));
				}
				else if(strEnvRecKey.IsEmpty())
				{
					continue;
				}

				ids = pEnvDM->GetCellData(_T("직급KEY"), nEnvCnt, strEnvClassKey);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("SaveRegClassValue"));
				}
				else if(strEnvClassKey != sClassKey)
				{
					continue;
				}

				ids = pEnvDM->GetCellData(_T("등록구분KEY"), nEnvCnt, strEnvRegKey);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("SaveRegClassValue"));
				}
				else if(strEnvRegKey != sRegKey)
				{
					continue;
				}

				ids = pEnvDM->SetCellData(_T("62_CMD_TYPE"), _T("U"), nEnvCnt);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1090, _T("SaveRegClassValue"));
				}
				ids = pEnvDM->SetCellData(_T("83_CMD_TYPE"), _T("U"), nEnvCnt);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("SaveRegClassValue"));
				}

				sRecKey = strEnvRecKey;
				bIsFindDM = TRUE;
				break;
			} // 실제 DB와 비교 끝
			
			pMobileApi->InitDBFieldData();

			// CM에는 값이 없는데 DM에는 있다면 삭제한것 -> DELETE
			if(_T("") == strDelayDay_CM) 
			{ // 62 COMMAND (센터에 등록구분의 직급별 반납연기일수 적용)
				if(TRUE == bIsFindDM)
				{	
					// 센터알림
					ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T("D"), nEnvCnt);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("SaveRegClassValue"));
					}
				}
			}
			if(_T("") == strLoanPeriod_CM && _T("") == strDelayDay_CM)
			{ // 83 COMMAND (타관의 등록구분의 직급별 대출기간,반납연기일수 동기화) 
				if(TRUE == bIsFindDM)
				{
					// 자관 정보 삭제
					pMobileApi->MakeDeleteFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
					// 타관 동기화
					ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T("D"), nEnvCnt);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1110, _T("SaveRegClassValue"));
					}
				}
			}
			else
			{
				// CM에는 값이 있고 DM에는 있다면 값수정 내용 -> UPDATE
				if(TRUE == bIsFindDM)
				{ 
					CString strLoanPeriod_DM, strDelayDay_DM;
					pEnvDM->GetCellData(_T("대출기한"), nEnvCnt, strLoanPeriod_DM);
					pEnvDM->GetCellData(_T("반납연기일수"), nEnvCnt, strDelayDay_DM);
					// 변경사항이 없으면 업데이트 안함.
					// 62 COMMAND
					if(strDelayDay_DM == strDelayDay_CM)
					{
						ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T(""), nEnvCnt);					
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1120, _T("SaveRegClassValue"));
						}
					}
					else
					{
						if(_T("") == strDelayDay_CM)
						{ // 없다면 DELETE한다.
							ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T("D"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1130, _T("SaveRegClassValue"));
							}
						}
						else
						{
							ids = pEnvDM->SetCellData( _T("62_CMD_TYPE"), _T("U"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1130, _T("SaveRegClassValue"));
							}
						}
					}
					// 83 COMMAND
					if(strLoanPeriod_DM == strLoanPeriod_CM && strDelayDay_DM == strDelayDay_CM)
					{
						ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T(""), nEnvCnt);
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1140, _T("SaveRegClassValue"));
						}
					}
					else
					{
						if(_T("") == strLoanPeriod_CM && _T("") == strDelayDay_CM)
						{ // 없다면 DELETE한다.
							pMobileApi->MakeDeleteFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
							ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T("D"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1150, _T("SaveRegClassValue"));
							}
						}
						else
						{
							pMobileApi->AddDBFieldData(_T("REG_CODE_KEY"), _T("NUMERIC"), sRegKey);
							pMobileApi->AddDBFieldData(_T("LOAN_CLASS_CODE_KEY"), _T("NUMERIC"), sClassKey);
							pMobileApi->AddDBFieldData(_T("LOAN_PERIOD"), _T("NUMERIC"), strLoanPeriod_CM);
							pMobileApi->AddDBFieldData(_T("POSTPONEMENT_PERIOD"), _T("NUMERIC"), strDelayDay_CM);			
							pMobileApi->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("열람환경설정"), __WFILE__, __LINE__ ), TRUE);
							pMobileApi->MakeUpdateFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);

							ids = pEnvDM->SetCellData( _T("83_CMD_TYPE"), _T("U"), nEnvCnt);
							if(0 > ids)
							{
								ERROR_MSG_RETURN_ERROR_CODE2(-1150, _T("SaveRegClassValue"));
							}
						}
					}
				}
				// CM에는 값이 있고 DM에는값이 없다면 추가설정한 내용 -> INSERT
				else
				{ 
					pMobileApi->MakeRecKey( sRecKey );
					pMobileApi->AddDBFieldData(_T("REG_CODE_KEY"), _T("NUMERIC"), sRegKey);
					pMobileApi->AddDBFieldData(_T("LOAN_CLASS_CODE_KEY"), _T("NUMERIC"), sClassKey);

					pEnvDM->InsertRow(-1);					
					INT nInsertIndex = pEnvDM->GetRowCount() - 1;

					ids = pEnvDM->SetCellData(_T("REC_KEY"), sRecKey, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1160, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("직급KEY"), sClassKey, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1170, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("등록구분KEY"), sRegKey, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1180, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("대출기한"), strLoanPeriod_CM, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1190, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("반납연기일수"), strDelayDay_CM, nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1200, _T("SaveRegClassValue"));
					}
					ids = pEnvDM->SetCellData(_T("83_CMD_TYPE"), _T("I"), nInsertIndex);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-1220, _T("SaveRegClassValue"));
					}

					if(_T("") != strLoanPeriod_CM)
					{
						pMobileApi->AddDBFieldData(_T("LOAN_PERIOD"), _T("NUMERIC"), strLoanPeriod_CM);
					}
					if(_T("") != strDelayDay_CM)
					{ // CMD 62
						pMobileApi->AddDBFieldData(_T("POSTPONEMENT_PERIOD"), _T("NUMERIC"), strDelayDay_CM);			
						ids = pEnvDM->SetCellData(_T("62_CMD_TYPE"), _T("I"), nInsertIndex);
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1220, _T("SaveRegClassValue"));
						}
					}
					pMobileApi->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("열람환경설정"), __WFILE__, __LINE__ ), TRUE);
					pMobileApi->MakeInsertFrame(_T("MN_LOAN_ENV_POSREG_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
				} // DB에 설정값이 없는 경우의 끝

			} // 그리드에 설정값이 있는 경우의 끝

			//JOB.2019.0073
			/*
			//이동도서관일 경우
			if( m_pInfo->MODE == 10000 )
			{
				if(!strLoanPeriod_CM.IsEmpty() && !strLoanPeriod_CM.IsEmpty())
				{
					if(strLoanPeriod_CM.IsEmpty()) strLoanPeriod_CM = _T("0");
					if(strDelayDay_CM.IsEmpty()) strDelayDay_CM = _T("0");
					strTL_SetData.Format(_T("REG_CODE_KEY=%s,LOAN_CLASS_CODE_KEY=%s,LOAN_PERIOD=%s,POSTPONEMENT_PERIOD=%s")
						,sRegKey,sClassKey,strLoanPeriod_CM,strDelayDay_CM);
					strTL_Condition.Format(_T("REG_CODE_KEY=%s AND LOAN_CLASS_CODE_KEY=%s"),sRegKey,sClassKey);
					
					m_syncApi.RefreshSyncData(_T("MN_LOAN_ENV_POSREG_TBL"),strTL_SetData,strTL_Condition);
				}
			}
			*/
			j++;
		}
		
	}

 	ids = pMobileApi->SendFrame();
	pEnvDM->EndFrame();

	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
	}
		
	// 공통 반납연기 정보 변경시 센터 도서관에 알리기 	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("MN_LOAN_ENV_POSREG_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;

	CString strEnvRecKey, strEnvType, strEnvClassKey, strEnvRegKey;
	CString strValue, strRegCode, strClassCode, strLoanPeriod, strPostponePeriod;
	for( i = 0 ; i < pEnvDM->GetRowCount() ; i++ )
	{
		pEnvDM->GetCellData(_T("REC_KEY"), i, strEnvRecKey);
		if(strEnvRecKey.IsEmpty())
		{
			continue;
		}
		pEnvDM->GetCellData(_T("62_CMD_TYPE"), i, strEnvType);
		if(strEnvType.IsEmpty())
		{
			continue;
		}
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Empty();
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("62");
		UploadShareInfo.UploadShareLoanInfo.sType = strEnvType;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strEnvRecKey;
		if(_T("U") == strEnvType)
		{
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("POSTPONEMENT_PERIOD");
		}
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	} // 대출이력공유 센터 적용의 끝
	
	return 0;

EFS_END
return -1;
}
// INT CMngLocMgr04Dlg::SaveRegClassValue()
// {
// EFS_BEGIN
// 
// 	INT ids;
// 
// 	CString sTableName = _T("MN_LOAN_ENV_POSREG_TBL");
// 	
// 	// ====================================================================================
// 	// 1. 초기화
// 	// ====================================================================================
// 	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_LOC_MGR_04_2_DLG") , _T("대출환경설정"));
// 	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") );
// 	
// 	CESL_DataMgr* pEnvDM = FindDM(_T("DM_MNG_LOC_MGR_04_2_DLG"));
// 	if( pEnvDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("SaveRegClassValue") );
// 	CESL_DataMgr* pRegCodeDM = FindDM(_T("DM_MNG_LOC_MGR_04_REG_CODE"));
// 	if( pRegCodeDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") );
// 	CESL_DataMgr* pClassDM = FindDM(_T("DM_MNG_LOC_MGR_04_CLASS_CODE"));
// 	if( pClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
// 
// 	// 2005.05.26 ADD BY PDJ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	if ( m_strLibCode.IsEmpty() )
// 	{
// 		// 2009.06.11 UPDATE BY PWR : m_pParentMgr->m_pInfo->LIB_CODE로 변경
// // 		CString	strWhere;
// // 		strWhere.Format(_T("SELECT MIN(LIB_CODE) FROM MN_LIBINFO2_TBL WHERE ")
// // 			            _T("MANAGE_CODE IN ( SELECT MIN(MANAGE_CODE) FROM MN_USER_TBL ")
// // 						_T("WHERE ID = '%s')"), m_strUserID );
// // 		pEnvDM->GetOneValueQuery( strWhere, m_strLibCode, FALSE );
// 		m_strLibCode = m_pInfo->LIB_CODE;
// 	}
// 	// -----------------------------------------------------------------------------------
// 
// 	// 저장한다.
// 	INT nCols = pGrid->GetCols(0);
// 	INT nRows = pGrid->GetRows();
// 
// 	CString sRegKey , sClassKey , sLoanPeriod , sDelayDay , sValue;
// 	CString sRecKey;
// 
// 	// ######################################################################################
// 	// --------------------------------------------------------------------------------------
// 	// 직급별 등록구분별 반납연기 정보 변경시 센터 도서관에 알리기 
// 	// 2003년 11월 8일 작업	
// 	UPLOAD_SHARE_INFO UploadShareInfo;
// 	UploadShareInfo.nType = 0;
// 	
// 	UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
// 	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("62");
// 	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("MN_LOAN_ENV_POSREG_TBL");
// 	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
// 	// --------------------------------------------------------------------------------------
// 	// ######################################################################################	
// 
// 	// Mobile API 적용
// 	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pEnvDM );
// 	pMobileApi->StartFrame();
// 
// 	for( INT i = 0 ; i < pEnvDM->GetRowCount() ; i++ )
// 	{
// 		ids = pEnvDM->GetCellData( _T("REC_KEY") , i , sRecKey );
// 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
// 
// 		pEnvDM->InitDBFieldData();
// 		pMobileApi->MakeDeleteFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
// 
// 		// ######################################################################################
// 		// --------------------------------------------------------------------------------------
// 		// 공통 반납연기 정보 변경시 센터 도서관에 알리기 
// 		// 2003년 11월 8일 작업		
// 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
// 		
// 		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );		
// 		// --------------------------------------------------------------------------------------
// 		// ######################################################################################
// 	}
// 
// 	ids = pMobileApi->SendFrame();
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
// 	
// 	pMobileApi->EndFrame();
// 	
// 	//=============================================
// 	//2009.02.20 REM BY LKS : 이동도서관은 별도관리
// 	//CLocCommonAPI::MobileApiEndLog( pMobileApi );
// 	//=============================================
// 
// 	pEnvDM->FreeData();
// 
// 	// ######################################################################################
// 	// --------------------------------------------------------------------------------------
// 	// 직급별 등록구분별 반납연기 정보 변경시 센터 도서관에 알리기 
// 	// 2003년 11월 8일 작업	
// 	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
// 
// 	CStringArray asRecKey;
// 	// --------------------------------------------------------------------------------------
// 	// ######################################################################################
// 	
// 	//이동도서관용 변수 선언
// 	CString strTL_SetData;
// 	CString strTL_Condition;
// 
// 	pMobileApi = CLocCommonAPI::InitMobileApi( this , pEnvDM );
// 	pMobileApi->StartFrame();
// 	for( i = 2 ; i < nRows ; i++ )
// 	{
// 		ids = pClassDM->GetCellData( _T("직급KEY") , i-2 , sClassKey );
// 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") )
// 
// 		for( INT j = 1 ; j < nCols ; j++ )
// 		{
// 			strTL_SetData.Empty();
// 			strTL_Condition.Empty();
// 
// 			ids = pRegCodeDM->GetCellData( _T("등록구분KEY") , (j-1)/2 , sRegKey );
// 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveRegClassValue") )
// 	
// 			sValue = pGrid->GetTextMatrix( i , j );
// 			sValue.TrimLeft() ; sValue.TrimRight();
// 			if( !sValue.IsEmpty() )
// 			{
// 				// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++
// 				// 'N'의 값을 DB Column Type에 맞는 가능한 INT로 변경
// 				if ( sValue == _T("N") )	sValue = _T("-1");
// 				// ---------------------------------------------------------------------
// 				sLoanPeriod.Format( _T("%d") , _ttoi(sValue) );
// 			}
// 			else
// 				sLoanPeriod = _T("NULL");
// 
// 			sValue = pGrid->GetTextMatrix( i , j+1 );
// 			sValue.TrimLeft() ; sValue.TrimRight();
// 			if( !sValue.IsEmpty() )
// 			{
// 				// 2006-03-14 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++
// 				// 'N'의 값을 DB Column Type에 맞는 가능한 INT로 변경
// 				if ( sValue == _T("N") )	sValue = _T("-1");
// 				// ---------------------------------------------------------------------
// 				sDelayDay.Format( _T("%d") , _ttoi(sValue) );
// 			}
// 			else 			
// 				sDelayDay = _T("NULL");
// 
// 			/// 두개의 값이 모두 NULL이라면 입력하지 않는다.
// 			if( (sLoanPeriod == _T("NULL")) && (sDelayDay == _T("NULL")) )
// 			{
// 				j++;
// 				continue;
// 			}
// 			
// 			pMobileApi->InitDBFieldData();
// 			pMobileApi->MakeRecKey( sRecKey );
// 
// 			pMobileApi->AddDBFieldData( _T("REG_CODE_KEY") , _T("NUMERIC") , sRegKey );
// 			pMobileApi->AddDBFieldData( _T("LOAN_CLASS_CODE_KEY") , _T("NUMERIC") , sClassKey );
// 			pMobileApi->AddDBFieldData( _T("LOAN_PERIOD") , _T("NUMERIC") , sLoanPeriod );
// 			pMobileApi->AddDBFieldData( _T("POSTPONEMENT_PERIOD") , _T("NUMERIC") , sDelayDay );
// 			pMobileApi->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("열람환경설정"), __WFILE__, __LINE__ ), TRUE );
// 			
// 			pMobileApi->MakeInsertFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
// 
// 			//이동도서관일 경우
// 			if( m_pInfo->MODE == 10000 )
// 			{
// 				strTL_SetData.Format(_T("REG_CODE_KEY=%s,LOAN_CLASS_CODE_KEY=%s,LOAN_PERIOD=%s,POSTPONEMENT_PERIOD=%s")
// 					,sRegKey,sClassKey,sLoanPeriod,sDelayDay);
// 				strTL_Condition.Format(_T("REG_CODE_KEY=%s AND LOAN_CLASS_CODE_KEY=%s"),sRegKey,sClassKey);
// 				
// 				m_syncApi.RefreshSyncData(_T("MN_LOAN_ENV_POSREG_TBL"),strTL_SetData,strTL_Condition);
// 			}
// 
// 			// ######################################################################################
// 			// --------------------------------------------------------------------------------------
// 			// 공통 반납연기 정보 변경시 센터 도서관에 알리기 
// 			// 2003년 11월 8일 작업			
// 			asRecKey.Add( sRecKey );
// 			// --------------------------------------------------------------------------------------
// 			// ######################################################################################
// 
// 			pEnvDM->InsertRow(-1);
// 		
// 			INT nInsertIndex = pEnvDM->GetRowCount()-1;
// 			pEnvDM->SetCellData( _T("REC_KEY") , sRecKey , nInsertIndex);
// 			pEnvDM->SetCellData( _T("직급KEY") , sClassKey , nInsertIndex);
// 			pEnvDM->SetCellData( _T("등록구분KEY") , sRegKey , nInsertIndex);
// 			pEnvDM->SetCellData( _T("대출기한") , sLoanPeriod , nInsertIndex);
// 			pEnvDM->SetCellData( _T("반납연기일수") , sDelayDay , nInsertIndex);
// 			
// 			j++;
// 		}
// 	}
// 
// 	ids = pMobileApi->SendFrame();
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveRegClassValue") );
// 	
// 	pEnvDM->EndFrame();
// 
// 	// ######################################################################################
// 	// --------------------------------------------------------------------------------------
// 	// 공통 반납연기 정보 변경시 센터 도서관에 알리기 
// 	// 2003년 11월 8일 작업		
// 	for( i = 0 ; i < asRecKey.GetSize() ; i++ )
// 	{
// 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = asRecKey.GetAt(i);
// 		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
// 	}
// 	// --------------------------------------------------------------------------------------
// 	// ######################################################################################
// 	
// 	//=============================================
// 	//2009.02.20 REM BY LKS : 이동도서관은 별도관리
// 	//CLocCommonAPI::MobileApiEndLog( pMobileApi );
// 	//=============================================
// 
// 	return 0;
// 
// EFS_END
// 	return -1;
// }


INT CMngLocMgr04Dlg::ShowCopyAndPasteMenu(POINT& pt)
{

	EFS_BEGIN

	INT nRow = m_pGrid->GetRow();
	if(0 == nRow) return 0;
	m_pGrid->SetReverse(nRow-1);


	CMenu menuCopyAndPaste;
	menuCopyAndPaste.CreatePopupMenu();

	if(m_bIsCopied)
	{
		menuCopyAndPaste.AppendMenu(MF_STRING,4001,_T("붙여넣기"));
		menuCopyAndPaste.AppendMenu(MF_SEPARATOR);
		menuCopyAndPaste.AppendMenu(MF_STRING,4000,_T("복사"));
		menuCopyAndPaste.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
	}
	else
	{
		menuCopyAndPaste.AppendMenu(MF_STRING,4000,_T("복사"));
		menuCopyAndPaste.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
	}




	return 0;

	EFS_END

	return -1;

}
VOID CMngLocMgr04Dlg::CopyAndPasteProc(UINT nIDC)
{
	EFS_BEGIN
	
	INT nRow = m_pGrid->GetRow();
	if(0 == nRow) return;

	if(4000 == nIDC)
	{
		for(INT i=0;i<6;i++)
		{
			m_pGrid->GetAt( nRow-1 , i+1 , m_strCopiedData[i] );
		}
	
		m_bIsCopied = TRUE;

	}
	else if(4001 == nIDC)
	{

		for(INT i=0;i<6;i++)
		{
			m_pGrid->SetAt( nRow-1 , i+1 , m_strCopiedData[i] );
		}

	}


	EFS_END
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++
// 직급별등록구분별 대출기한 설정 그리드 추가 이벤트
VOID CMngLocMgr04Dlg::OnClickgridCLASSREG() 
{
EFS_BEGIN

//	m_pGrid->SetReverse( m_pGrid->GetIdx() );
//	FreeGridEdt();
	m_bFirstReturn = FALSE;
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2);

EFS_END
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++
// 직급별등록구분별 대출기한 설정 그리드 추가 이벤트
VOID CMngLocMgr04Dlg::OnDblClickgridCLASSREG() 
{
EFS_BEGIN

	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_gridPOSITION2);
	INT nCol = m_pGrid->GetCol();
	
	m_bFirstReturn = TRUE;
	InputDataToGrid();
	
	m_pGrid->SetCol( nCol );

EFS_END
}

// 2006-03-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++
// 직급별등록구분별 대출기한 설정 그리드 추가 이벤트
VOID CMngLocMgr04Dlg::OnKeyUpgridCLASSREG(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	EFS_END
}

HBRUSH CMngLocMgr04Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}