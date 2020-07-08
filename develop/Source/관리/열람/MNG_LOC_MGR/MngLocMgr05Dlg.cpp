// MngLocMgr05Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr05Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr05Dlg dialog


CMngLocMgr05Dlg::CMngLocMgr05Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr05Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	// 15/02/18 이충무 : 2016년 국립중앙도서관 책이음개선
	// 주의 다이얼로그 1회 발생을 위한 변수 초기화
	/*//NEW------------------------------------------------------------------------*/
	m_bAlarm = FALSE;
	m_bLILL = FALSE;

	CString strUnityLoanService;
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), strUnityLoanService );
	strUnityLoanService.TrimLeft();	strUnityLoanService.TrimRight();
	strUnityLoanService.MakeUpper();
	if(strUnityLoanService.Compare(_T("Y")) == 0) 
		m_bLILL = TRUE;
	
	/*//END------------------------------------------------------------------------*/
}
CMngLocMgr05Dlg::~CMngLocMgr05Dlg()
{
	
}

VOID CMngLocMgr05Dlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr05Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMngLocMgr05Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr05Dlg)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmbCODE, OnSelchangecmbCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr05Dlg message handlers
BOOL CMngLocMgr05Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMngLocMgr05Dlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCode);

	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	//pGrid->MoveWindow(5, 40, cx-8, cy-60 );

	// 16/10/11 김혜영 : 공통>관리>업무환경설정>대출환경설정의 대출자료제한 그리드가 가려지는 현상 수정
/*//BEFORE-----------------------------------------------------------------------------
	pGrid->MoveWindow(-2, 35, cx+2, cy);
*///AFTER------------------------------------------------------------------------------
	pGrid->MoveWindow(-2, 35, cx+2, cy-45);
/*//END-------------------------------------------------------------------------------*/	
}

BOOL CMngLocMgr05Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT ids;

	// 15/02/23 이충무 : 2016년 국립중앙도서관 책이음개선
	// 통합도서사용유무 분기
	/*//BEFORE-----------------------------------------------------------------------
	ids = InitESL_Mgr(_T("SM_MNG_LOC_MGR_05_DLG"));
	*///AFTER------------------------------------------------------------------------
	if(m_bLILL)
	{
		ids = InitESL_Mgr(_T("SM_MNG_LOC_MGR_05_DLG_LILL"));
		// KOL.RED.006 JMC 20161111 안내문구 추가		
		GetDlgItem(IDC_LILL_DESC)->ShowWindow(TRUE);
	}
	else
	{
		ids = InitESL_Mgr(_T("SM_MNG_LOC_MGR_05_DLG"));
	}
	/*//END------------------------------------------------------------------------*/
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	// 2009.02.25 ADD BY PWR : 이동도서관은 자료실 안보인다.
	((CComboBox*)GetDlgItem(IDC_cmbCODE))->AddString( _T("별치기호") );
	((CComboBox*)GetDlgItem(IDC_cmbCODE))->AddString( _T("등록구분") );
	if(m_pInfo->MODE != 10000)
	{
		((CComboBox*)GetDlgItem(IDC_cmbCODE))->AddString( _T("자료실") );
	}

	((CComboBox*)GetDlgItem(IDC_cmbCODE))->SetCurSel(0);

	ViewScreen();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMngLocMgr05Dlg::ViewScreen()
{
	EFS_BEGIN

	INT nIndex = ((CComboBox*)GetDlgItem(IDC_cmbCODE))->GetCurSel();

	CString sWhere;
	if( nIndex == 0 )
		sWhere = _T("CLASS = 11");
	else if( nIndex == 1 )
		sWhere = _T("CLASS = 4");
	else if( nIndex == 2 )
		sWhere = _T("CLASS = 19");

	// 15/02/23 이충무 : 2016년 국립중앙도서관 책이음개선
	// 통합도서사용유무 분기
	/*//BEFORE-----------------------------------------------------------------------
	CString sDMAlias = _T("DM_MNG_LOC_MGR_05_DLG");
	CString sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	*///AFTER------------------------------------------------------------------------
	CString sDMAlias = _T("");
	CString sCMAlias = _T("");
	if(m_bLILL)
	{
		sDMAlias = _T("DM_MNG_LOC_MGR_05_DLG_LILL");
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG_LILL");
	}
	else
	{
		sDMAlias = _T("DM_MNG_LOC_MGR_05_DLG");
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	}
	/*//END------------------------------------------------------------------------*/

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewScreen") );

//	pDM->RefreshDataManager(sWhere);
	CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewScreen") );

	pCM->AllControlDisplay();

	return 0;

	EFS_END
	return -1;

}

INT CMngLocMgr05Dlg::SaveScreen()
{
	EFS_BEGIN

	INT ids;


	// 15/02/23 이충무 : 2016년 국립중앙도서관 책이음개선
	// 통합도서사용유무 분기
	/*//BEFORE-----------------------------------------------------------------------
	CString sDMAlias = _T("DM_MNG_LOC_MGR_05_DLG");
	CString sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	*///AFTER------------------------------------------------------------------------
	CString sDMAlias = _T("");
	CString sCMAlias = _T("");
	if(m_bLILL)
	{
		sDMAlias = _T("DM_MNG_LOC_MGR_05_DLG_LILL");
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG_LILL");
	}
	else
	{
		sDMAlias = _T("DM_MNG_LOC_MGR_05_DLG");
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	}
	/*//END------------------------------------------------------------------------*/

	CString sGridAlias = _T("코드별대출여부리스트");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewScreen") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	CString sValue;
	CString sRecKey;

	// 16/02/17 이충무 : 2016년 국립중앙도서관 책이음개선
	// 상호대차 제공여부 컬럼추가
	/*//NEW------------------------------------------------------------------------*/
	CString strPreData1, strPreData2;
	CString sValue2;

	INT nIndex = ((CComboBox*)GetDlgItem(IDC_cmbCODE))->GetCurSel();
	/*//END------------------------------------------------------------------------*/

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		sValue.Empty();
		pGrid->GetAt( i , 2 , sValue );
		sValue.TrimRight(); sValue.TrimLeft();

		pDM->GetCellData( _T("REC_KEY") , i , sRecKey );

		// 16/02/17 이충무 : 2016년 국립중앙도서관 책이음개선
		// 상호대차 제공여부 컬럼추가
		/*//NEW------------------------------------------------------------------------*/
		if(m_bLILL)
		{
			pDM->GetCellData( _T("대출여부") , i , strPreData1 );
			pDM->GetCellData( _T("상호대차제공여부") , i , strPreData2 );
		}
		/*//END------------------------------------------------------------------------*/

		// Mobile API 적용
		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pDM );

		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("LOAN_YN") , _T("STRING") , sValue);

		// 16/02/17 이충무 : 2016년 국립중앙도서관 책이음개선
		// 상호대차 제공여부 컬럼추가
		/*//NEW------------------------------------------------------------------------*/
		if(m_bLILL)
		{
			pGrid->GetAt( i , 3 , sValue2 );
			sValue2.TrimRight(); sValue2.TrimLeft();
			pDM->AddDBFieldData( _T("LILL_YN") , _T("STRING") , sValue2 );
		}
		/*//END------------------------------------------------------------------------*/

//		pDM->MakeUpdateFrame( _T("CD_CODE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("열람관리"), __WFILE__, __LINE__ ), TRUE );
		pMobileApi->MakeUpdateFrame( _T("CD_CODE_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
//		ids = pDM->SendFrame();
		ids = pMobileApi->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );		

		// 16/02/17 이충무 : 2016년 국립중앙도서관 책이음개선
		// 상호대차 제공여부 컬럼추가 BO_BOOK_TBL 수정
		/*//NEW------------------------------------------------------------------------*/				
		if(m_bLILL && (strPreData1.Compare(sValue) != 0 || strPreData2.Compare(sValue2) != 0))
		{
			pMobileApi->InitDBFieldData ();	

			CString strQuery;			
		
			pGrid->GetAt( i , 0 , sValue );
			
			CString sWhere;
			if( nIndex == 0 )
			{
				if(sValue.Compare(_T("")) == 0)
				{
					strQuery = _T("UPDATE BO_BOOK_TBL SET SEPARATE_SHELF_CODE = SEPARATE_SHELF_CODE WHERE SEPARATE_SHELF_CODE = '' OR SEPARATE_SHELF_CODE IS NULL;");				
				}
				else
				{
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SEPARATE_SHELF_CODE = SEPARATE_SHELF_CODE WHERE SEPARATE_SHELF_CODE = '%s';"), sValue);	
				}
			}
			else if( nIndex == 1 )
			{
				if(sValue.Compare(_T("")) == 0)
				{
					strQuery = _T("UPDATE BO_BOOK_TBL SET REG_CODE = REG_CODE WHERE REG_CODE = '' OR REG_CODE IS NULL;");
				}
				else
				{
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_CODE = REG_CODE WHERE REG_CODE = '%s';"), sValue);				
				}
			}
			else if( nIndex == 2 )
			{
				if(sValue.Compare(_T("")) == 0)
				{
					strQuery = _T("UPDATE BO_BOOK_TBL SET SHELF_LOC_CODE = SHELF_LOC_CODE WHERE SHELF_LOC_CODE = '' OR SHELF_LOC_CODE IS NULL;");
				}
				else
				{
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_LOC_CODE = SHELF_LOC_CODE WHERE SHELF_LOC_CODE = '%s';"), sValue);									
				}
			}

			pMobileApi->AddFrame(strQuery);

			pMobileApi->SendFrame();					
		}
		/*//END------------------------------------------------------------------------*/		

		pDM->EndFrame();

		// Mobile API delete
		CLocCommonAPI::MobileApiEndLog( pMobileApi );
	}

	ViewScreen();

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgr05Dlg::OnSelchangecmbCODE() 
{
	EFS_BEGIN

	ViewScreen();	

	EFS_END
}

BEGIN_EVENTSINK_MAP(CMngLocMgr05Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr05Dlg)
	ON_EVENT(CMngLocMgr05Dlg, IDC_gridCode, -600 /* Click */, OnClickgridCode, VTS_NONE)
	ON_EVENT(CMngLocMgr05Dlg, IDC_gridCode, -601 /* DblClick */, OnDblClickgridCode, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngLocMgr05Dlg::OnClickgridCode() 
{
	EFS_BEGIN

	// 15/02/23 이충무 : 2016년 국립중앙도서관 책이음개선
	// 통합도서사용유무 분기
	/*//BEFORE-----------------------------------------------------------------------
	CString sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	*///AFTER------------------------------------------------------------------------	
	CString sCMAlias = _T("");
	if(m_bLILL)
	{	
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG_LILL");
	}
	else
	{		
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	}
	/*//END------------------------------------------------------------------------*/
	
	CString sGridAlias = _T("코드별대출여부리스트");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("ViewScreen") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	
	pGrid->SetReverse( pGrid->GetIdx() );

	EFS_END
}

VOID CMngLocMgr05Dlg::OnDblClickgridCode() 
{
	EFS_BEGIN

	// 15/02/23 이충무 : 2016년 국립중앙도서관 책이음개선
	// 통합도서사용유무 분기
	/*//BEFORE-----------------------------------------------------------------------
	CString sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	*///AFTER------------------------------------------------------------------------	
	CString sCMAlias = _T("");
	if(m_bLILL)
	{	
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG_LILL");
	}
	else
	{		
		sCMAlias = _T("CM_MNG_LOC_MGR_05_DLG");
	}
	/*//END------------------------------------------------------------------------*/
	
	CString sGridAlias = _T("코드별대출여부리스트");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("ViewScreen") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	// 16/02/17 이충무 : 2016년 국립중앙도서관 책이음개선
	// 상호대차 신청제한
	/*//BEFORE-----------------------------------------------------------------------
	INT nCol = pGrid->GetCol();

	CString sYN;
	INT nRow = pGrid->GetIdx();
	pGrid->GetAt( nRow , 2 , sYN);

	if( sYN.Compare(_T("Y"))==0 )
		sYN = _T("N");
	else 
		sYN = _T("Y");


	pGrid->SetAt( nRow , 2 , sYN );
	*///AFTER------------------------------------------------------------------------

	CString sYN;

	INT nCol = pGrid->GetCol();
	INT nRow = 0;

	if(m_bLILL)
	{
		nRow = pGrid->GetRow();

		pGrid->GetAt( nRow-1 , nCol-1 , sYN);

		if(nCol == 3 || nCol == 4)
		{
			/*
			if(!m_bAlarm)
			{
				MessageBox(_T("대출 및 상호대차제공 여부 수정 시 \n\n도서권수에 따라 동기화 작업에 많은 시간이 \n\n소요될 수 있음을 알려드립니다. \n\n\n한번더 검토하시고 수정 부탁드립니다."), _T("알림"), MB_ICONWARNING|MB_OK);
				m_bAlarm = TRUE;
				return;
			}
			*/
			if( sYN.Compare(_T("Y"))==0 )
				sYN = _T("N");
			else 
				sYN = _T("Y");

			pGrid->SetAt( nRow-1 , nCol-1 , sYN );
		}
	}
	else
	{	
		nRow = pGrid->GetIdx();

		pGrid->GetAt( nRow , 2 , sYN);

		if( sYN.Compare(_T("Y"))==0 )
			sYN = _T("N");
		else 
			sYN = _T("Y");


		pGrid->SetAt( nRow , 2 , sYN );
	}	
	/*//END------------------------------------------------------------------------*/	

	EFS_END
}

HBRUSH CMngLocMgr05Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

