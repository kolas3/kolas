// BO_LOC_21001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_21001.h"
#include "BO_LOC_2160.h"
#include "BO_LOC_21010.h"
#include "BO_LOC_2190_CLICK_LIST.h"

#include "ESL_SearchEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2100 dialog


CBO_LOC_2100::CBO_LOC_2100(CString strMenuAlias,CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sStrMenuName = strMenuAlias;
}

CBO_LOC_2100::~CBO_LOC_2100()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}

	m_cAxisVal_Statics.Clear();
}

VOID CBO_LOC_2100::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2100, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2100)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabOPTION, OnSelchangetabOPTION)
	ON_BN_CLICKED(IDC_btnMOVE, OnbtnMOVE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MOVE_SHELF_LOC, OnBtnMoveShelfLoc)
	ON_BN_CLICKED(IDC_BTN_DELETE_SHELF_LOC, OnBtnDeleteShelfLoc)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkAppendixYN, OnchkAppendixYN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2100 message handlers

BOOL CBO_LOC_2100::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("BO_LOC_2100")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);

	pGrid->SetRowHeight(0, 400);

	m_pSpfWork = NULL;
	m_pSpfWork = new CBLBookCheckProc(this);

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);

	TC_ITEM item;

	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;

	item.pszText = _T("장서체크결과");
	pTab->InsertItem(0, &item);

	item.pszText = _T("통계");
	pTab->InsertItem(1, &item);


	INT ids = InitScreen();	
	
	const INT nGridCnt = 3;
	TCHAR *sGridAlias[nGridCnt][2] =
	{
		{ _T("CM_BO_LOC_2100") , _T("gridMAIN") } , 
		{ _T("CM_BO_LOC_2100_NOT_IN_DB_ETC_GRID") , _T("NotInDBETCList") } , 
		{ _T("CM_BO_LOC_2100_IN_DB_ETC_GRID") , _T("InDBETCList") } 
	};
	CArray <CString,CString> RemoveListArray;
	CESL_Grid *pGrid_MainGrid_ALL;

	for( INT i = 0 ; i < 1 ; i++ )
	{
		pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( sGridAlias[i][0] , sGridAlias[i][1] );
		if( pGrid_MainGrid_ALL == NULL ) return -1;

		pGrid_MainGrid_ALL->InitPopupMenuItem();
		pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , &RemoveListArray );
	}
	
	SetTimer( ORACLE_TIMER_QRY, 50000, NULL );
	
	InitStaticsGridQuery();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2100::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	CRect Bar1, Bar2, Bar3;
	CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_STATIC_BAR1);
	CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_STATIC_BAR2);
	CStatic* pStatic3 = (CStatic*)GetDlgItem(IDC_STATIC_BAR3);	
	if(pStatic1->GetSafeHwnd() == NULL){ return; }
	if(pStatic2->GetSafeHwnd() == NULL){ return; }
	if(pStatic3->GetSafeHwnd() == NULL){ return; }
	pStatic1->GetWindowRect( &Bar1 );
	pStatic2->GetWindowRect( &Bar2 );
	pStatic3->GetWindowRect( &Bar3 );
	ScreenToClient( Bar1 );
	ScreenToClient( Bar2 );
	ScreenToClient( Bar3 );
	pStatic1->MoveWindow(0, Bar1.top , cx, Bar1.Height());
	pStatic2->MoveWindow(0, Bar2.top , cx, Bar2.Height());
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);
	pTab->MoveWindow( -2 , Bar3.top , cx+2 , cy );

	const INT GRID_COUNT = 2;
	CMSHFlexGrid *pGridMain[GRID_COUNT] = { NULL, NULL };
	pGridMain[0] = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	pGridMain[1] = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN4);
	for(INT nCount=0; nCount<GRID_COUNT; nCount++)	
	{
		if(NULL == pGridMain[nCount]) return;
		else pGridMain[nCount]->MoveWindow(0, 88, cx-2, cy-88);
	}

	//2020.03.27 JOB.2020.0021 add by kyj : 장서점검 부록 선택 옵션 추가 
	CRect CheckBoxRect1;
	GetDlgItem(IDC_chkAppendixYN)->GetWindowRect(&CheckBoxRect1);
	
	CRect ScreenRect;
	this->GetWindowRect(&ScreenRect);
	ScreenToClient(ScreenRect);
	INT nScrrenRight = ScreenRect.Width();
	//INT nScrrenBottom = rc.Height();
	ScreenToClient(CheckBoxRect1);
	GetDlgItem(IDC_chkAppendixYN)->MoveWindow( nScrrenRight - 5 - CheckBoxRect1.Width(), CheckBoxRect1.top, CheckBoxRect1.Width(), CheckBoxRect1.Height());

	EFS_END
}

BOOL CBO_LOC_2100::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CBO_LOC_2100::InitScreen()
{
	EFS_BEGIN

	INT ids = m_pSpfWork->SPFExcutePath( INIT_BOOK_CHECK_NUMBER );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen()") );
	
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_2100::OnClose() 
{
	EFS_BEGIN

	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;

	CDialog::OnClose();

	EFS_END
}

VOID CBO_LOC_2100::OnbtnCHANGE() 
{
	EFS_BEGIN

	CBO_LOC_2160 dlg(this);
	dlg.m_mode = 1;
	dlg.m_nLoadSPF = 0;
	dlg.DoModal();

	EFS_END
}

VOID CBO_LOC_2100::OnSelchangetabOPTION(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);

	INT nIndex = pTab->GetCurSel();

	const INT nCnt=4;
	INT nGridIDC[nCnt] = { IDC_gridMAIN , IDC_gridMAIN2 , IDC_gridMAIN3 , IDC_gridMAIN4 }; 

	for( INT i=0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nGridIDC[i])->ShowWindow(SW_HIDE);
	}
	
	if( nIndex == 0 )
		GetDlgItem(nGridIDC[0])->ShowWindow(SW_SHOW);
	else if(nIndex == 1)
		GetDlgItem(nGridIDC[3])->ShowWindow(SW_SHOW);

	CString strTabAlias;
	CStringArray arrSpeciseBtnAlias;
	arrSpeciseBtnAlias.Add(_T("목록출력"));

	if(nIndex == 1 )
	{
		pMain->SendMessage( WM_USER + 2000, ( WPARAM )&arrSpeciseBtnAlias[0],  TRUE);		
	}
	else
	{
		pMain->SendMessage( WM_USER + 2000, ( WPARAM )&arrSpeciseBtnAlias[0], FALSE );
	}
		
	*pResult = 0;

	EFS_END
}

VOID CBO_LOC_2100::OnbtnMOVE() 
{
	EFS_BEGIN

	m_pSpfWork->SPFExcutePath( MOVE_GRID );	

	EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_LOC_2100, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_2100)
	ON_EVENT(CBO_LOC_2100, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CBO_LOC_2100, IDC_gridMAIN2, -601 /* DblClick */, OnDblClickgridMAIN2, VTS_NONE)
	ON_EVENT(CBO_LOC_2100, IDC_gridMAIN4, -600 /* Click */, OnClickgridMAIN4, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_2100::OnDblClickgridMAIN() 
{
	EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_2100") , _T("gridMAIN") );
	
	if( pGrid->GetMouseRow() < 1 ) return ;
	
	m_pSpfWork->SPFExcutePath( DETAIL_VIEW );

	EFS_END
}

VOID CBO_LOC_2100::ResultGridDisplay()
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 3;
	TCHAR *sCMAlias[nCnt] = { _T("CM_BO_LOC_2100") , _T("CM_BO_LOC_2100_IN_DB_ETC_GRID") , _T("CM_BO_LOC_2100_NOT_IN_DB_ETC_GRID") };

	for( INT i=1 ; i < 1 ; i++ )
	{
		ids = AllControlDisplay( sCMAlias[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-3,_T("ResultGridDisplay")); 
	}

	return ;

	EFS_END
}

INT CBO_LOC_2100::IsShelfLocNoSelect()
{
	EFS_BEGIN

	CString sShelfLocCode = m_pSpfWork->GetShelfLocCode();
	if( sShelfLocCode.IsEmpty() )
	{
		return 1;
	}

	return 0;
EFS_END
	return -1;
}

VOID CBO_LOC_2100::OnDblClickgridMAIN2() 
{
	EFS_BEGIN

	EFS_END
}

INT CBO_LOC_2100::PrintList()
{
	EFS_BEGIN

	INT ids;

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);
	INT nIndex = pTab->GetCurSel();

	if( nIndex > 2 ) return 0;

	CString sPrintAlias[3] =
	{
		_T("장서점검비소장목록출력") , _T("장서점검기타자료1목록출력") , _T("장서점검기타자료2목록출력") 
	};

	CESL_DataMgr *pNoLibBookDM = FindDM(_T("DM_BO_LOC_2100"));
	if( pNoLibBookDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	CESL_DataMgr *pEtc1DM = FindDM(_T("DM_BO_LOC_2100_ETC"));
	if( pEtc1DM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	CESL_DataMgr *pEtc2DM = FindDM(_T("DM_BO_LOC_2100_ETC_NOT_IN_DB"));
	if( pEtc2DM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr **pPrintDM;
	pPrintDM = new CESL_DataMgr*[3];
	pPrintDM[0] = pNoLibBookDM;
	pPrintDM[1] = pEtc1DM;
	pPrintDM[2] = pEtc2DM;

	INT nTotalCnt = pPrintDM[nIndex]->GetRowCount();

	if( nTotalCnt < 1 )
	{
		delete []pPrintDM;
		AfxMessageBox( _T("목록으로 출력할 자료가 존재하지 않습니다.") );
		return 0;
	}
	
	CString sOrder;
	for( INT i = 0 ; i < nTotalCnt ; i++ )
	{
		sOrder.Format( _T("%d") , i+1 );
		ids = pPrintDM[nIndex]->SetCellData( _T("번호") , sOrder , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}

	pSIReportManager->SetCONNECTION_INFO(pNoLibBookDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(sPrintAlias[nIndex]);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	CString sTotalCnt;
	sTotalCnt.Format( _T("%d") , nTotalCnt );
	pSIReportManager->SetTempVariable( _T("총자료수") , sTotalCnt );
	
	CString priceSum = GetTotalPrice( pNoLibBookDM );
	if( !nIndex )
		pSIReportManager->SetTempVariable( _T("가격합계") , priceSum );

	pSIReportManager->SetDataMgr(0, pPrintDM[nIndex] , 0);

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}		

	delete []pPrintDM;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_2100::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

CString CBO_LOC_2100::GetTotalPrice( CESL_DataMgr *pDM )
{
	INT nRowCnt = pDM->GetRowCount();
	INT nSum = 0;
	CString price;
	for( INT i = 0; i < nRowCnt; i++ )
	{
		price = pDM->GetCellData( _T("가격"), i );
		price.Replace( _T(","), _T("") );
		nSum += _ttoi( price );
	}

	CString sumOfPrice;
	sumOfPrice.Format( _T("%d"), nSum );
	return m_pSpfWork->ConvertPrice( sumOfPrice ) + _T(" 원");
}

VOID CBO_LOC_2100::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( ORACLE_TIMER_QRY == nIDEvent )
	{
		CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_2100_ETC") );
		CString sBuf;
		pDM->GetOneValueQuery( _T("SELECT SYSDATE FROM DUAL"), sBuf );
	}
	
	
	CDialog::OnTimer(nIDEvent);
}

void CBO_LOC_2100::OnBtnMoveShelfLoc() 
{
	if( 0 > MoveSelectShelfLoc() )
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못했습니다"), MB_ICONSTOP);
	}
}

INT CBO_LOC_2100::MoveSelectShelfLoc()
{
EFS_BEGIN

	INT		ids;
	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_2100_ETC_INFO"));
	if( NULL == pCM )	return -1010;

	CString strInsertCode[2];
	ids = pCM->GetControlMgrData( _T("자료실명") , strInsertCode[0] , -1 , 0 );
	if( 0 > ids ) return -1021;
	ids = pCM->GetControlMgrData( _T("자료실명") , strInsertCode[1] , -1 , 1 );
	if( 0 > ids ) return -1022;
	
	strInsertCode[0].TrimLeft();strInsertCode[0].TrimRight();	
	if( strInsertCode[0].IsEmpty() )	return 0;

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("자료실조건")));
	if( NULL == pCMCB )	return -1030;

	INT nRowCount = pCMCB->GetCount();
	if( 1 > nRowCount )
	{
		pCMCB->InitMultiComboBox(2);
	}

	CString strData;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = pCMCB->GetData( strData , i , 0 );
		if( 0 > ids ) return -1032;

		strData.TrimLeft(); strData.TrimRight();
		if( 0 == strData.Compare( strInsertCode[0] ) )	return 0;
	}

	ids = pCMCB->SetData( strInsertCode , 2 );
	if( 0 > ids ) return -1033;

	pCMCB->SetCurSel(nRowCount);

	return 0;
EFS_END
return -1;
}

void CBO_LOC_2100::OnBtnDeleteShelfLoc() 
{
	if( 0 > RemoveSelectShelfLoc() )
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못했습니다"), MB_ICONSTOP);
	}
}

INT CBO_LOC_2100::RemoveSelectShelfLoc()
{
EFS_BEGIN

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_2100_ETC_INFO"));
	if( NULL == pCM )	return -1010;

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("자료실조건")));
	if( NULL == pCMCB )	return -1030;

	INT nCount = pCMCB->GetRowCount();
	
	if( 0 == nCount )
		return 0;

	INT nCurSel = pCMCB->GetCurSel();

	CString** sItem;
	sItem = NULL;
	sItem = new CString*[nCount];
	for( INT i = 0 ; i < nCount ; i++ )
	{
		sItem[i] = new CString[2];
	}

	for( i = 0 ; i < nCount ; i++ )
	{
		pCMCB->GetData( sItem[i][0] , i , 0 );
		pCMCB->GetData( sItem[i][1] , i , 1 );
	}

	pCMCB->ResetContent();
	pCMCB->FreeComboData();

	pCMCB->InitMultiComboBox(2);

	for( i = 0 ; i < nCount ; i++ )
	{
		if( i == nCurSel ) continue;
		
		pCMCB->SetData( sItem[i] , 2 );
	}

	for( i = 0 ; i < nCount ; i++ )
		delete []sItem[i];

	delete []sItem;
			
	pCMCB->SetCurSel(0);

	return 0;
EFS_END
return -1;
}

HBRUSH CBO_LOC_2100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

int FP_MultiSort_UserCompear_BookCheck( CString Alias , CString* p1 , CString* p2 )
{
	
	return 100;
}

CESL_Mgr* CBO_LOC_2100::ShowSortDlg()
{

	CESL_DataMgr*	pDM = FindDM( _T("DM_BO_LOC_2100") );
	CBO_LOC_21010*	pdlg = new CBO_LOC_21010(this);
	pdlg->m_cstr_RecentFile = _T("..\\CFG\\장서점검_정렬_최근상태.cfg");
	pdlg->SetDM( pDM , this , _T("CM_BO_LOC_2100") );
	
	if (pdlg->Create(IDD_BO_LOC_21010,this))
	{
		pdlg->ShowWindow(SW_SHOW);
		pdlg->UpdateWindow();
		pdlg->CenterWindow();
	}else{
		delete pdlg;
		pdlg = NULL;
	}
	return pdlg;

}

BOOL CBO_LOC_2100::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CBO_LOC_2100::InitStaticsGridQuery()
{
	CESL_DataMgr*	pDM_StaticsList = FindDM(_T("DM_BO_STATICS_LIST"));

	CLocCommonAPI::MakeCopyDM( this , pDM_StaticsList , &m_cDM_StaticsGridData );

	m_nMode_Statics = -1;
	m_sXAlias_Statics = _T("");
	m_sYAlias_Statics = _T("");

	m_cDM_StaticsRegNo.InitDataMgrRef(1);
	m_cDM_StaticsRegNo.SetDataMgrRef( 0 , _T("등록번호") , _T("") , _T("")  , _T("")  , _T("") );		

}

void CBO_LOC_2100::OnClickgridMAIN4() 
{
	// TODO: Add your control notification handler code here
	if( m_nMode_Statics == -1 || m_sYAlias_Statics.IsEmpty() )
		return;

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN4);
	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	CString		Desc_x , Desc_y;

	Desc_x = pGrid->GetTextMatrix(0, nCol );
	Desc_y = pGrid->GetTextMatrix( nRow , 0 );

	CString AliasX,AliasY;
	m_cAxisVal_Statics.GetWhere( 0 , 0 , Desc_x , AliasX );
	m_cAxisVal_Statics.GetWhere( 1 , 0 , Desc_y , AliasY );
	if( AliasX.IsEmpty() || AliasY.IsEmpty() ) return;
	m_cAxisVal_Statics.GetWhere( 0 , 1 , Desc_x , AliasX );
	m_cAxisVal_Statics.GetWhere( 1 , 1 , Desc_y , AliasY );
	if( AliasX.IsEmpty() || AliasY.IsEmpty() ) return;

	int ids = 0;

	int			idx_reg = 0;
	int			t_reg_split = 900;
	CString		where_reg_no_in;
	CStringArray	reg_no_arr;
	reg_no_arr.RemoveAll();

	CString		reg_no;
	int			reg_row_count = m_cDM_StaticsRegNo.GetRowCount();
	bool		fst = true;

	while(true)
	{
		if( fst == false )where_reg_no_in += _T(",");
		reg_no = m_cDM_StaticsRegNo.GetCellData( idx_reg , 0 );
		idx_reg++;
		reg_no = _T("'") + reg_no + _T("'");
		where_reg_no_in += reg_no;
		if( idx_reg >= reg_row_count ){
			reg_no_arr.Add(where_reg_no_in);
			where_reg_no_in=_T("");
			break;
		}		
		if( idx_reg % t_reg_split == 0 )
		{
			fst = true;
			reg_no_arr.Add(where_reg_no_in);
			where_reg_no_in=_T("");
		}else{
			fst = false;
		}
	}

	CESL_DataMgr* pADD_DM = new CESL_DataMgr;
	pADD_DM->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	pADD_DM->SetManageCode( m_pInfo->MANAGE_CODE );
	int idx_arr;
	CString where_reg_no,AddQuery;

	m_cDM_StaticsGridData.FreeData();

	CESL_DataMgr cDM , *pDM;
	cDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	cDM.SetManageCode( m_pInfo->MANAGE_CODE );
	pDM = &cDM;

	CString		whereAxis_X , whereAxis_Y;
	
	for( int i = 0 ; i < 2 ; i++ )
	{
		pDM->FreeData();
		CString query = _T("");
		if( i == 0 )
		{
			query = _T("select B.REG_NO , i.title_info from  BO_BOOK_TBL B , idx_bo_tbl i  where ");
			query += _T(" I.REC_KEY=B.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL252N') ");
			if (!IsDlgButtonChecked(IDC_chkAppendixYN)) query += _T(" AND b.BOOK_APPENDIX_FLAG='B' ");
			m_cAxisVal_Statics.GetWhere( 0 , i , Desc_x , whereAxis_X );
			m_cAxisVal_Statics.GetWhere( 1 , i , Desc_y , whereAxis_Y );
			query += whereAxis_Y;
			if(whereAxis_X.Find(_T("substr(B.CLASS_NO,0,1) = ''")) > -1 )	query += _T(" AND B.CLASS_NO IS NULL");
			else	query += whereAxis_X;	
			for (idx_arr = 0 ; idx_arr < reg_no_arr.GetSize(); idx_arr++ )
			{							
				where_reg_no = reg_no_arr.GetAt(idx_arr);
				if(where_reg_no.IsEmpty()) continue;
				AddQuery.Format(_T("%s AND REG_NO IN (%s)"), query, where_reg_no );
				pADD_DM->RestructDataManager(AddQuery); 
				pDM->AddDM(pADD_DM);
			}
			
		}else{
			query = _T("select B.REG_NO , i.title_info from SE_BOOK_TBL B,SE_VOL_TBL V,  idx_se_tbl i  where ");
			query += _T(" I.REC_KEY=B.SPECIES_KEY AND B.MANAGE_CODE = UDF_MANAGE_CODE AND B.VOL_KEY=V.REC_KEY AND B.WORKING_STATUS IN ('SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O') ");
			if (!IsDlgButtonChecked(IDC_chkAppendixYN)) 
				query += _T(" AND B.BOOK_TYPE IN ('B','C') ");
			else
				query += _T(" AND B.BOOK_TYPE IN ('B','C','A') ");
			m_cAxisVal_Statics.GetWhere( 0 , i , Desc_x , whereAxis_X );
			m_cAxisVal_Statics.GetWhere( 1 , i , Desc_y , whereAxis_Y );
			query += whereAxis_Y;
			if(whereAxis_X.Find(_T("substr(B.CLASS_NO,0,1) = ''")) > -1 )	query += _T(" AND B.CLASS_NO IS NULL");
			else	query += whereAxis_X;	
			for (idx_arr = 0 ; idx_arr < reg_no_arr.GetSize(); idx_arr++ )
			{								
				where_reg_no = reg_no_arr.GetAt(idx_arr);
				if(where_reg_no.IsEmpty()) continue;
				AddQuery.Format(_T("%s AND REG_NO IN (%s)"), query, where_reg_no );
				pADD_DM->RestructDataManager(AddQuery); 
				pDM->AddDM(pADD_DM);
			}
		}

		for(int o = 0 ; o <  pDM->GetRowCount() ; o++)
		{
			CString reg_no = pDM->GetCellData(o,0);
			CString title = pDM->GetCellData(o,1);
			m_cDM_StaticsGridData.InsertRow(-1);
			int row_idx = m_cDM_StaticsGridData.GetRowCount()-1;
			ids = m_cDM_StaticsGridData.SetCellData( row_idx , 0 , reg_no );
			ids = m_cDM_StaticsGridData.SetCellData( row_idx , 1 , reg_no );
			ids = m_cDM_StaticsGridData.SetCellData( row_idx , 2 , title );
		}
	}

	BO_LOC_2190_CLICK_LIST	dlg;
	dlg.m_pDM_Src = &m_cDM_StaticsGridData;
	dlg.DoModal();
}

void CBO_LOC_2100::OnchkAppendixYN() 
{	
	if ( IsDlgButtonChecked(IDC_chkAppendixYN) ) {
		AfxMessageBox(_T("부록을 포함하여 장서점검을 진행합니다.\r\n장섬점검 과정중 변경 할 경우 올바르지 않은 데이터가 산출될 수 있습니다. "));
	}
	else 
	{
		AfxMessageBox(_T("부록을 제외하여 장서점검을 진행합니다.\r\n장섬점검 과정중 변경 할 경우 올바르지 않은 데이터가 산출될 수 있습니다. "));
	}	
}
