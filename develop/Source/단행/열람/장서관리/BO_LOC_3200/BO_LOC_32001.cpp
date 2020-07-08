// BO_LOC_32001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_32001.h"
#include "lprn_api.h"
#include "BO_LOC_3290.h"
#include "Imm.h"
#include "PersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////m///////////////////////
// CBO_LOC_3200 dialog


CBO_LOC_3200::CBO_LOC_3200(CESL_Mgr* pParent )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;
}

CBO_LOC_3200::~CBO_LOC_3200()
{
	if( m_pLoanShareManager != NULL )	delete m_pLoanShareManager;
	if( m_pLoanMangeValue != NULL )	delete m_pLoanMangeValue;
}


VOID CBO_LOC_3200::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3200)
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3200, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3200)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMAIN, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_radSORT_LOAN_DATE, OnradSORT)
	ON_BN_CLICKED(IDC_BTN_DELAY_SUM, OnBtnDelaySum)
	ON_BN_CLICKED(IDC_btnSelect, OnBtnSelect)
	ON_EN_SETFOCUS(IDC_edtRegNo, OnSetfocusedtRegNo)
	ON_BN_CLICKED(IDC_radSORT_RETURN_DATE, OnradSORT)	
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3200 message handlers

BOOL CBO_LOC_3200::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_3200::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("BO_LOC_3200")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}


	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create(this);

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAIN);
	
	TC_ITEM item;
	
	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;
	
	pTab->InsertItem( 0 , _T("대출/반납 자료") );

	CWnd* pTitleBar = GetDlgItem(IDC_staMESSAGE);
	pTitleBar->SetWindowText(_T(""));
	
	const INT nGridCnt = 1;
	TCHAR *sGridAlias[nGridCnt][2] =
	{
		{ _T("CM_BO_LOC_3200") , _T("MainGrid") }
	};
	CArray <CString,CString> RemoveListArray;
	CESL_Grid *pGrid_MainGrid_ALL;
	for( INT i = 0 ; i < nGridCnt ; i++ )
	{
		pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( sGridAlias[i][0] , sGridAlias[i][1] );
		if( pGrid_MainGrid_ALL == NULL ) return -1;
		
		pGrid_MainGrid_ALL->InitPopupMenuItem();
		pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , &RemoveListArray );
	}

	pGrid_MainGrid_ALL->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid_MainGrid_ALL->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid_MainGrid_ALL->m_bPrivacyPrintLogRecord = TRUE;
		pGrid_MainGrid_ALL->m_strPrintInfo = _T("대출자료관리");
	}		

	m_pLoanMangeValue = new CLonaManageValue(this);
	m_pLoanMangeValue->pMain = pMain;
	m_pLoanMangeValue->pParentMgr = (CESL_Mgr*)this;
	m_pLoanMangeValue->SM_STYLE = 'T';
	m_pLoanMangeValue->m_pInfo = this->m_pInfo;

	m_pLoanMangeValue->DoModal();

	((CButton*)GetDlgItem(IDC_radSORT_LOAN_DATE))->SetCheck( 1 );

	ChangeToolBarButton();

	SetControlData( _T("CM_BO_LOC_3200") , _T("정렬기준") , _T("대출일") );
	
	CEdit* pEdtRegNo = ( CEdit*) GetDlgItem (IDC_edtRegNo);
	pEdtRegNo->SetLimitText(19);
	pEdtRegNo->SetFocus();

	CString strIsReturn;
	CString strColName;
	GetManageValue( _T("기타"), _T("공통"), _T("타관반납설정"), _T(""), strIsReturn );
	
	if( strIsReturn == _T("Y") )
	{
		GetLibList();		
	}
	else
	{
		pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_3200"), _T("MainGrid") );
		if( pGrid_MainGrid_ALL == NULL ) return -1;

		for( INT i = 0; i < pGrid_MainGrid_ALL->GetColCount(); i++ )
		{
			strColName = pGrid_MainGrid_ALL->GetTextMatrix( 0, i + 1 );
			if( strColName == _T("반납신청도서관") )
			{
				pGrid_MainGrid_ALL->SetColWidth( i + 1, 0, 0 );
			}
		}
	}

	if(m_pInfo->MODE==10000)
	{
		CString strButtonAlias ;
		strButtonAlias = _T("메일이력");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("메일");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
	}


	GetManageValue(_T("열람"), _T("공통"), _T("무인반납중자료상태사용여부"), _T(""), m_sSelfReturnUseYN);

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3200"), _T("등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3200::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN4);
	CWnd* pTitleBar = GetDlgItem(IDC_staMESSAGE);
	CWnd* pConditionBar = GetDlgItem(IDC_staCONDITION_BAR);
	CWnd* pBarcode = GetDlgItem(IDC_staBARCODE);

	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}
	if(pTitleBar->GetSafeHwnd() == NULL) {
		return;
	}
	if(pConditionBar->GetSafeHwnd() == NULL) {
		return;
	}

	CRect TitleBarRect;
	CRect ConditionBarRect;
	CRect BarcodeRect;

	pTitleBar->GetWindowRect(TitleBarRect);
	pConditionBar->GetWindowRect(ConditionBarRect);
	pBarcode->GetWindowRect(BarcodeRect);

	if(bUnionUse == TRUE)
	{
		pConditionBar->MoveWindow(-1, 0, ConditionBarRect.Width(), ConditionBarRect.Height());
		pBarcode->MoveWindow(ConditionBarRect.Width()-1, 0, BarcodeRect.Width(), ConditionBarRect.Height());
		pTitleBar->MoveWindow(ConditionBarRect.Width()+BarcodeRect.Width()+10, 0, cx+2-BarcodeRect.Width(), ConditionBarRect.Height());
		pGrid->MoveWindow(-2, TitleBarRect.Height() , cx+4, cy-TitleBarRect.Height()+4);
		
		(CEdit*)GetDlgItem(IDC_edtRegNo)->SetFocus();

		CWnd* pSumBtn = GetDlgItem(IDC_BTN_DELAY_SUM);
		if( NULL == pSumBtn->GetSafeHwnd() )
		{
			return;
		}
		
		CRect rcSumBtn;
		pSumBtn->GetWindowRect(rcSumBtn);
		pSumBtn->MoveWindow( cx-4-rcSumBtn.Width(), 4, rcSumBtn.Width(), rcSumBtn.Height() );
	}
	else
	{
		pConditionBar->MoveWindow(-1, 0, ConditionBarRect.Width( )/*cx+2*/, ConditionBarRect.Height());
		pTitleBar->MoveWindow(ConditionBarRect.Width( )-1, 0, cx+2-ConditionBarRect.Width( ), ConditionBarRect.Height());
		
		pGrid->MoveWindow(-2, TitleBarRect.Height() , cx+2, cy+4-TitleBarRect.Height());
		
		CWnd* pSumBtn = GetDlgItem(IDC_BTN_DELAY_SUM);
		if( NULL == pSumBtn->GetSafeHwnd() )
		{
			return;
		}
		
		CRect rcSumBtn;
		pSumBtn->GetWindowRect(rcSumBtn);
		pSumBtn->MoveWindow( cx-4-rcSumBtn.Width(), 4, rcSumBtn.Width(), rcSumBtn.Height() );
	}
	EFS_END
}

VOID CBO_LOC_3200::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	*pResult = 0;

	EFS_END
}


INT CBO_LOC_3200::SetPublishForm()
{
	EFS_BEGIN

	INT ids;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAIN);
	CString sPublishForm = _T("M");
	switch(pTab->GetCurSel()){
	case 0:
		sPublishForm = _T("M");
		break;
	case 1:
		sPublishForm = _T("N");
		break;
	case 2:
		sPublishForm = _T("S");
		break;
	default:
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3200::SetPublishForm()") );
	}
	ids = SetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3200::SetPublishForm()") );

	return 0;

	EFS_END
	return -1;

}
BEGIN_EVENTSINK_MAP(CBO_LOC_3200, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3200)
	ON_EVENT(CBO_LOC_3200, IDC_gridMAIN4, -601 /* DblClick */, OnDblClickgridMAIN4, VTS_NONE)
	ON_EVENT(CBO_LOC_3200, IDC_gridMAIN4, -605 /* MouseDown */, OnMouseDowngridMAIN4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_LOC_3200, IDC_gridMAIN4, -607 /* MouseUp */, OnMouseUpgridMAIN4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


INT CBO_LOC_3200::ChangeToolBarButton()
{
	EFS_BEGIN

	CString sValue;
	sValue = m_pLoanMangeValue->m_sReserveMode;
	CString strButtonAlias;
	
	if( sValue.Compare(_T("0"))==0 )
	{
		strButtonAlias = _T("예약처리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}

	sValue = m_pLoanMangeValue->m_sUseReceipt;
	if( sValue.Compare(_T("N"))==0 )
	{
		strButtonAlias = _T("영수증재발급");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		
	}

	CString strIsReturn;
	if( _T("Y") == m_pLoanMangeValue->m_strEachManageUseYN )
	{
		m_pLoanMangeValue->GetManageValue( _T("기타"), _T("공통"), _T("관리구분별반납설정"), _T(""), strIsReturn );		
	}

	if( _T("Y") != strIsReturn )
	{
		strButtonAlias = _T("분관반납자료처리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}
	
	GetManageValue( _T("기타"), _T("공통"), _T("타관반납설정"), _T(""), strIsReturn );	

	if( _T("Y") != strIsReturn )
	{
		strButtonAlias = _T("타관반납자료처리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}

	bUnionUse = TRUE;
	GetDlgItem(IDC_staRegNo)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnSelect)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtRegNo)->ShowWindow(SW_SHOW);

	INT ids;
	ids = GetManageValue( _T("열람") , _T("공통") , _T("대출자료관리") , _T("야간대출예약기능유무") , sValue );
	if( ids < 0 && ids != -4007 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("OnInitDialog") );
	if( _T("Y") != sValue )
	{
		strButtonAlias = _T("야간대출예약상태변경");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);	
	}


	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3200::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}

INT CBO_LOC_3200::PrintList()
{
EFS_BEGIN
	
	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_3200") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	if( PrivacyPrintLogRecord(pSrcDM, _T("대출자자료관리 목록출력"), _T("E") ,0 ) < 0 ) return 1;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3200") , _T("MainGrid") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_3200_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pPrintDM->FreeData();

	const INT nCopyCnt = 29;
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("등록번호"),_T("등록번호")},
		{_T("배가상태"),_T("배가상태")},
		{_T("대출일"),_T("대출일")},
		{_T("예약만기일"),_T("예약만기일")},
		{_T("반납일"),_T("반납일")},
		{_T("반납예정일"),_T("반납예정일")},
		{_T("주민등록번호"),_T("주민등록번호")},
		{_T("누적연체일"),_T("누적연체일")},
		{_T("E_MAIL"),_T("E_MAIL")},
		{_T("직급"),_T("직급")},
		{_T("소속"),_T("소속")},
		{_T("대출자"),_T("대출자")},
		{_T("대출자번호"),_T("대출자번호")},
		{_T("저작자"),_T("저작자")},
		{_T("발행자"),_T("발행자")},
		{_T("본표제"),_T("본표제")},
		{_T("예약일"),_T("예약일")},
		{_T("청구기호"),_T("청구기호")},
		{_T("대출상태"),_T("대출상태")},
		{_T("전화번호_자택"),_T("전화번호_자택")},
		{_T("전화번호_근무지"),_T("전화번호_근무지")},
		{_T("휴대폰"),_T("휴대폰")},
		{_T("주소_자택"),_T("주소_자택")},
		{_T("주소_근무지"),_T("주소_근무지")},
		{_T("교번"),_T("교번")},
		{_T("관리구분"),_T("관리구분")},
		{_T("대출형태"),_T("대출형태")},
		{_T("반납형태"),_T("반납형태")},
		{_T("대출자KEY"),_T("대출자KEY")}
	};
		
	CString sData;
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("관리구분") , _T("관리구분") );

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	ids = pSIReportManager->GetSIReportFileFromDB(_T("대출자료관리"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}
	
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );

	std::vector<CString> list;

	CString strKey;			
	for(int j=0; j<pPrintDM->GetRowCount(); j++)
	{
		ids = pPrintDM->GetCellData(_T("대출자KEY"), j, strKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3200::PrintList()") );
		list.push_back(strKey);
	}

	CPersonInfo::SetPrintLog(this, pDM, PERINFO_EXCEL, m_pInfo, _T("대출자료목록출력"), pPrintDM->GetRowCount(), &list);
	list.clear();

	return 0;
	
EFS_END
	return -1;
}

VOID CBO_LOC_3200::OnDblClickgridMAIN4() 
{
	EFS_BEGIN
		
	CString sCMAlias = _T("CM_BO_LOC_3200");
	CString sControlAlias = _T("MainGrid");
	
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3200::OnClickgridMAIN1()") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sControlAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CBO_LOC_3200::OnClickgridMAIN1()") );
	
	if( pGrid->GetMouseRow() < 1 ) return ;
	
	CBL_LOC_LOAN_MNG_PROC SpfWork(this,m_pLoanMangeValue,m_pLoanShareManager);
	SpfWork.SPFExcutePath( _T("대출자료관리_상세정보") );
	
	EFS_END
}	

INT CBO_LOC_3200::StationBookReturn()
{
	EFS_BEGIN

	CBL_LOC_LOAN_MNG_PROC SpfWork(this,m_pLoanMangeValue ,m_pLoanShareManager);
	SpfWork.SPFExcutePath( _T("무인반납처리") );

	return 0;
	
	EFS_END

	return -1;
}

INT CBO_LOC_3200::GetSelfReturnStatusUseYN(CString &sSelfReturnUseYN)
{
	EFS_BEGIN

	sSelfReturnUseYN = m_sSelfReturnUseYN;

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3200::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN

	CString sManageCode;

	if(strDMFieldAlias == _T("직급"))
	{	
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , str , str );
	}
	else if( strDMFieldAlias == _T("소속"))
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , str , str );
	}
	
	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3200::ChangeDMToTL()
{
	EFS_BEGIN

	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3200::OnradSORT() 
{
	SORT();	
}

INT CBO_LOC_3200::SORT()
{
	const INT SORT_COND = 2;
	INT ids = 0 ;
	CString sDMAlias = _T("DM_BO_LOC_3200");
	CString sCMAlias = _T("CM_BO_LOC_3200");
	INT nSortCond[SORT_COND];
	CString sSortCond[SORT_COND];

	CButton* pLoan = (CButton*) GetDlgItem( IDC_radSORT_LOAN_DATE );
	CButton* pReturn = (CButton*) GetDlgItem( IDC_radSORT_RETURN_DATE );

	CESL_DataMgr* pDM = ( CESL_DataMgr* ) FindDM( sDMAlias );
	if ( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3200::OnradSORT()") );
	
	if ( pDM->GetRowCount( ) < 2 )
		return 0;

	sSortCond[0] = _T("대출자");
	nSortCond[0] = ESL_DATAMGR_SORT_ASC;

	if ( 1 == pLoan->GetCheck( ) )
	{		
		sSortCond[1] = _T("대출일");
	}
	else if ( 1 == pReturn->GetCheck( ) )
	{
		sSortCond[1] = _T("반납일");
	}
	nSortCond[1] = ESL_DATAMGR_SORT_DESC;
	
	ids = pDM->SORT( SORT_COND, sSortCond, nSortCond, ESL_DATAMGR_SORT_ASC );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3200::pDM->SORT()") );

	SetControlData( _T("CM_BO_LOC_3200") , _T("정렬기준") , sSortCond[1] );

	ids = AllControlDisplay( sCMAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3200::AllControlDisplay()") );

	return 0;
}

INT CBO_LOC_3200::ReissueReceipt()
{
	INT ids = 0;
	INT nDataCnt = 0;

	CString sDMAlias = _T("DM_BO_LOC_3200_RECEIPT");
	CString sCMAlias = _T("CM_BO_LOC_3200");
	CString sWhere = _T("");
	CString sRctStr = _T("");
	CString sMsg = _T("");

	CESL_DataMgr* pDM = ( CESL_DataMgr* ) FindDM( sDMAlias );
	if ( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReissueReceipt()") );

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( sCMAlias, _T("MainGrid") );
	if ( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReissueReceipt()") );

	ids = getReceiptWhere( pGrid, sWhere );
	if ( ids < 0 ) return -3;
	else if ( ids > 0 ) return ids;

	ids = pDM->RefreshDataManager( sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReissueReceipt") );

	nDataCnt = pDM->GetRowCount( );
	if ( 0 == nDataCnt )
	{
		AfxMessageBox( _T("영수증 출력 정보가 없습니다.") );
		return 10;
	}

	sMsg.Format( _T("선택하신 자료 중 총 %d개의\n영수증 출력 정보가 있습니다."), nDataCnt );
	AfxMessageBox( sMsg );

	ids = pDM->StartFrame( );
	if ( ids < 0 )	return -5;

	for ( INT i = 0 ; i < nDataCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("영수증문자열"), i, sRctStr );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ReissueReceipt") );

		ids = printReceipt( sRctStr );
		if ( 0 != ids ) return ids;

		ids = setDataToDB( pDM, i );
		if ( ids < 0 ) return -7;
	}

	ids = pDM->SendFrame ( ) ;
	if ( ids < 0 )	return -8;

	ids = pDM->EndFrame ( ) ;
	if ( ids < 0 )	return -9;

	return 0;
}

INT CBO_LOC_3200::getReceiptWhere(CESL_Grid *pGrid, CString &sWhere)
{
	INT ids = 0;
	INT nIdx = 0;
	INT nSelectCnt = 0;
	CString sDMAlias = _T("DM_BO_LOC_3200");
	CString sData = _T("");
	CString sTmpWhere = _T("");

	ids = pGrid->SelectMakeList( );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("getReceiptWhere") );

	nSelectCnt = pGrid->SelectGetCount( );
	if ( 0 == nSelectCnt ) 
	{
		AfxMessageBox( _T("선정된 데이터가 없습니다.\n선정하신 후 진행해 주세요!") );
		return 1;
	}
	else if ( nSelectCnt < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("getReceiptWhere") );

	if ( 1 == ((CButton*)GetDlgItem( IDC_radSORT_LOAN_DATE ))->GetCheck( ) )
		sWhere += _T("SUBSTR(RECEIPT_STRING, 1, 15) LIKE CONCAT('%', CONCAT('대출', '%')) AND (");
	else
		sWhere += _T("SUBSTR(RECEIPT_STRING, 1, 15) LIKE CONCAT('%', CONCAT('반납', '%')) AND (");

	nIdx = pGrid->SelectGetHeadPosition( );
	if ( nIdx < 0 ) return -3;

	for ( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		ids = GetDataMgrData( sDMAlias, _T("대출/반납정보KEY"), sData, nIdx );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("getReceiptWhere") );

		if ( sTmpWhere.IsEmpty( ) )
			sTmpWhere = _T(" LOAN_KEY_LIST LIKE CONCAT('%', CONCAT('") + sData + _T("', '%'))");
		else
			sTmpWhere += _T(" OR LOAN_KEY_LIST LIKE CONCAT('%', CONCAT('") + sData + _T("', '%'))");

		if ( i+1 != nSelectCnt )
		{
			nIdx = pGrid->SelectGetNext( );
			if ( nIdx < 0 ) return -4;
		}
	}

	sWhere += sTmpWhere + _T(")");

	return 0;
}

INT CBO_LOC_3200::printReceipt(CString sRctStr)
{
	INT ids = 0;
	INT nStatus = 0;

	TCHAR ErrMsg[1024];
	UCHAR szStr[2048];

	nStatus = LP_GetStatus( );
	CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_GetStatus()"), _T("상태정보 가져오기"), __WFILE__, __LINE__, nStatus );
	if ( -1 == nStatus )
	{
		LP_GetErrorMsg( ErrMsg );
		CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_GetStatus()"), ErrMsg, __WFILE__, __LINE__, ids );
		AfxMessageBox(_T("발견된 출력기가 없습니다."));
		return 100;
	}
	else if ( 0 != nStatus )
	{
		ids = LP_StartUp();
		if( 0 != ids )
		{
			LP_GetErrorMsg( ErrMsg );
			CLocCommonAPI::MakeRFIDLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
			AfxMessageBox(_T("발견된 출력기가 없습니다."));
			return 200;
		}
		else
			CLocCommonAPI::MakeRFIDLog( _T("[LOG]LP_StartUp()"), _T("영수증 출력 준비"), __WFILE__, __LINE__, ids );
	}

	for( INT i = 0 ; i < sRctStr.GetLength( ) ; i++ )
		szStr[i] = ( UCHAR ) sRctStr.GetAt( i );

	szStr[i]= '\0';

	ids = LP_PrintData( szStr );	

	return 0;
}

INT CBO_LOC_3200::setDataToDB(CESL_DataMgr *pDM, INT nIdx)
{
	INT ids = 0;
	CString sReckey = _T("");

	ids = pDM->GetCellData( _T("영수증KEY"), nIdx, sReckey );
	if ( ids < 0 )	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("setDataToDB") );

	pDM->InitDBFieldData( );

	ids = pDM->AddDBFieldData( _T("PRINT_COUNT"), _T("NUMERIC"), _T("PRINT_COUNT+1") );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("setDataToDB") );

	ids = pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("영수증재출력"), __WFILE__, __LINE__ ) );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("setDataToDB") );

	ids = pDM->MakeUpdateFrame( _T("CO_LOAN_RECEIPT_TBL"), _T("REC_KEY"), _T("NUMERIC"), sReckey );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("setDataToDB") );

	return 0;
}

INT CBO_LOC_3200::OtherManageCodeBookReturn()
{
EFS_BEGIN

	CBL_LOC_LOAN_MNG_PROC SpfWork(this,m_pLoanMangeValue ,m_pLoanShareManager);

	return ( SpfWork.SPFExcutePath( _T("분관반납처리") ) );	
	
EFS_END
	return -1;
}

INT CBO_LOC_3200::OtherLibCodeBookReturn()
{
EFS_BEGIN

	CBL_LOC_LOAN_MNG_PROC SpfWork(this,m_pLoanMangeValue ,m_pLoanShareManager);

	return ( SpfWork.SPFExcutePath( _T("타관반납처리") ) );	
	
EFS_END
	return -1;
}

INT CBO_LOC_3200::ChangeNightLoanReservationStatus()
{
EFS_BEGIN
	
	CBO_LOC_3290 dlg;
	if( IDOK == dlg.DoModal() )
	{
		if( dlg.m_nMode == -1 )
		{
			AfxMessageBox( _T("변경작업이 취소 되었습니다.") );			
		}
		else
		{
			CBL_LOC_LOAN_MNG_PROC SpfWork(this,m_pLoanMangeValue ,m_pLoanShareManager);
			SpfWork.ChangeNightLoanStatus( dlg.m_nMode );
		}
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_3200::MakeValidRegNo(CString &strGetRegNo)
{
EFS_BEGIN

	if( strGetRegNo.IsEmpty() )
	{
		AfxMessageBox( _T("반납 대상 자료의 등록번호를 입력해야 합니다") );
		return -1001;
	}

	INT		nRegNoLength = strGetRegNo.GetLength();

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	if( 12 < nRegNoLength || nRegNoLength < nRegCodeLength)
	{
		AfxMessageBox(_T("잘못된 등록번호입니다."));
		strGetRegNo.Empty();
		return -1010;
	}
	else if( 12 > nRegNoLength )
	{		
		CString strRegCode = strGetRegNo.Mid(0,nRegCodeLength);
		CString strRegNum  = strGetRegNo.Mid(nRegCodeLength);

		strGetRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNum);
	}
	//=====================================================
	
	
	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3200::SelectSearchHistory()
{
EFS_BEGIN

	INT ids;
	CString strBookRegNo;
	GetDlgItemText(IDC_edtRegNo, strBookRegNo);

	if( strBookRegNo.IsEmpty() )
	{
		AfxMessageBox( _T("반납 대상 자료의 등록번호를 입력해야 합니다") );
		return -1001;
	}

	ids = MakeValidRegNo( strBookRegNo );
	if( 0 > ids )	return -1010;
   
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3200") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SelectSearchHistory()") );
			
	if( pDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("검색된 데이타가 없습니다") );
		return -1001;
	}
	else
	{
		for( int i = 0; i < pDM->GetRowCount(); i++ )
		{
			CString strGridBookRegNo;
			pDM->GetCellData( _T("등록번호"), i, strGridBookRegNo );
			if( strGridBookRegNo == strBookRegNo )
			{
				CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3200"), _T("MainGrid") );
				pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);
				break;
			}
		}
	}
	return 0;
EFS_END
return -1;
}

void CBO_LOC_3200::OnBtnSelect()
{
EFS_BEGIN
	
	SelectSearchHistory();
	SetDlgItemText(IDC_edtRegNo, _T(""));
	( CEdit*) GetDlgItem (IDC_edtRegNo)->SetFocus();

EFS_END
}

INT CBO_LOC_3200::GetLibList()
{
EFS_BEGIN

	return 0;
EFS_END
return -1;
}

void CBO_LOC_3200::OnBtnDelaySum() 
{
	GetTotalDelayCount();
}

INT	CBO_LOC_3200::GetTotalDelayCount()
{
	INT	ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3200") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetTotalDelayCount") );
			
	if( pDM->GetRowCount() < 1 )		
		return 1;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3200") , _T("MainGrid") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	
	INT		nSelectCnt = pGrid->SelectGetCount();
	INT		nIndex = 0;
	INT		nTotalDelayCount = 0;
	CString strData;

	if( 0 == nSelectCnt )
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 1;
	}

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = pDM->GetCellData(_T("연체일"), nIndex, strData);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTotalDelayCount") );

		if( !strData.IsEmpty() )
			nTotalDelayCount += _ttoi(strData);
		
		pGrid->SelectGetNext();
	}

	CString strMsg;
	strMsg.Format(_T("선정된 자료의 총 연체일 수는 %d 일 입니다."), nTotalDelayCount);

	AfxMessageBox( strMsg );

	return 0;
}

VOID CBO_LOC_3200::OnMouseDowngridMAIN4(short Button, short Shift, long x, long y)
{
	if( Button == 1 || Button == 2)
	{
	}
}

void CBO_LOC_3200::OnMouseUpgridMAIN4(short Button, short Shift, long x, long y) 
{

}

BOOL CBO_LOC_3200::PreTranslateMessage(MSG* pMsg) 
{
	if( WM_KEYDOWN == pMsg->message)
	{		
		if( VK_RETURN == pMsg->wParam )
		{
			HWND MsgH = pMsg->hwnd;
			
			if( GetDlgItem(IDC_edtRegNo)->m_hWnd==MsgH )
			{
				OnBtnSelect();
			}
			
			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}
	if (pMsg->message == WM_USER+1907)
	{
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3200"), _T("MainGrid"));
		ASSERT(pGrid);

		if (pGrid->GetSafeHwnd() == pMsg->hwnd && m_pInfo->m_bUserInfoExportYN == FALSE)
		{
			AfxMessageBox(_T("이용자 정보를 반출할 권한이 없습니다.\r\n해당 권한은 관리자를 통해서만 부여받을 수 있습니다."), MB_ICONWARNING);
			return TRUE;
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CBO_LOC_3200::OnSetfocusedtRegNo() 
{
EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtRegNo)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtRegNo)->m_hWnd,hIME); 
    }
	
EFS_END
}

HBRUSH CBO_LOC_3200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
