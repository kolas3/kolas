// SearchBookListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchBookListDlg.h"
#include "..\\..\\..\\..\\관리\\검색\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\관리\\검색\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchBookListDlg dialog


CSearchBookListDlg::CSearchBookListDlg(CESL_Mgr* pParent /*=NULL*/,INT nEntryPoint )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CSearchBookListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCloseMode = 1;
	m_nEntryPoint = nEntryPoint;
	m_nIndex = 0;
}

CSearchBookListDlg::~CSearchBookListDlg()
{
}

VOID CSearchBookListDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchBookListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSearchBookListDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchBookListDlg)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDC_btnBOOK_RESERVATION, OnbtnBOOKRESERVATION)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMENU, OnSelchangetabMENU)
	ON_BN_CLICKED(IDC_btnBOOK_DETAIL, OnbtnBOOKDETAIL)
	ON_WM_CTLCOLOR()
	ON_WM_NCACTIVATE()
	ON_BN_CLICKED(IDC_btn3115_LoanUserView, Onbtn3115LoanUserView)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchBookListDlg message handlers

BOOL CSearchBookListDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3115")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

//	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );
	HICON shit = AfxGetApp()->LoadIcon( IDI_ICON1 );

	SetIcon( shit , FALSE );

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem( IDC_tabMENU );
	
	CString sTitle;
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3110_BO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	sTitle.Format( _T("단행(%d)") , pDM->GetRowCount() );

	pTab->InsertItem( 0 , sTitle );

	pDM = FindDM(_T("DM_BO_LOC_3110_SE"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	sTitle.Format( _T("연속(%d)") , pDM->GetRowCount() );
	pTab->InsertItem( 1 , sTitle );
	
	CString strQuery = _T("");
	PriorUseYN=_T("");
	strQuery.Format(_T("SELECT MAX(VALUE_1) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='수서' AND GROUP_3='단행' AND VALUE_NAME = '비치희망우선예약특별조건' AND MANAGE_CODE= UDF_MANAGE_CODE"));
	pDM->GetOneValueQuery(strQuery,PriorUseYN);
	
	if (PriorUseYN != _T("Y"))	GetDlgItem(IDC_chkReq_RESERVATION)->ShowWindow(SW_HIDE);

	ControlChange();

	ViewGrid();

	GridSelectView();

	CRect WndPos ;
	GetWindowRect( WndPos );
	WndPos.bottom++;
	MoveWindow(WndPos);

	nFINISH = GetTickCount();
	TRACE(_T("(Finish) - 이용자 자료 찾기 결과 윈도우 보여주는 시간 : [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CSearchBookListDlg::GetSelectRegNo( CString &sRegNo )
{
	EFS_BEGIN

	if( m_nCloseMode == 0 || (m_nCloseMode == 2) ) sRegNo = m_sRegNo;
	return 0;

	EFS_END
	return -1;

}

INT CSearchBookListDlg::GetSelectPublishForm( CString &sPublishForm )
{
	EFS_BEGIN

	if( m_nCloseMode == 0 || (m_nCloseMode == 2) ) sPublishForm = m_sPublishForm;
	return 0;

	EFS_END
	return -1;

}

INT CSearchBookListDlg::SetSelectRegNo()
{
EFS_BEGIN
	
	INT ids;
	TCHAR szFuncName[] = _T("SetSelectRegNo");

	CString sCMAlias = _T("CM_BO_LOC_3115");

	CString sGridAlias = GetSelectGridAlias();

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, szFuncName);
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, szFuncName);
	}

	if( pGrid->GetMouseRow() < 1 )
	{
		return 1;
	}

	INT nIndex = pGrid->GetIdx();	

	if( nIndex < 0 )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, szFuncName);
	}
	
	m_nIndex = nIndex;
	CString sDMAlias = GetSelectDMAlias();

	ids = GetDataMgrData( sDMAlias , _T("등록번호") , m_sRegNo , nIndex );
	if( ids < 0 )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, szFuncName);
	}

	ids = GetDataMgrData( sDMAlias , _T("단행/연속구분") , m_sPublishForm , nIndex );
	if( ids < 0 )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, szFuncName);
	}
	
	CString sWorkingStatus;
	ids = GetDataMgrData( sDMAlias , _T("자료상태") , m_sWorkingStatus, nIndex );
	if( ids < 0 )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, szFuncName);
	}

	return 0;

EFS_END
return -1;
}


INT CSearchBookListDlg::ViewGrid()
{
	EFS_BEGIN

	INT ids = AllControlDisplay( _T("CM_BO_LOC_3115") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("ViewGrid") );
	return 0;

	EFS_END
	return -1;

}

INT CSearchBookListDlg::SetSelect( INT n )
{
	EFS_BEGIN

	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CSearchBookListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSearchBookListDlg)
	ON_EVENT(CSearchBookListDlg, IDC_BOOKLISTGRID, -604 /* KeyUp */, OnKeyUpBooklistgrid, VTS_PI2 VTS_I2)
	ON_EVENT(CSearchBookListDlg, IDC_BOOKLISTGRID, -600 /* Click */, OnClickBooklistgrid, VTS_NONE)
	ON_EVENT(CSearchBookListDlg, IDC_BOOKLISTGRID, -601 /* DblClick */, OnDblClickBooklistgrid, VTS_NONE)
	ON_EVENT(CSearchBookListDlg, IDC_BOOKLISTGRID2, -600 /* Click */, OnClickBooklistgrid2, VTS_NONE)
	ON_EVENT(CSearchBookListDlg, IDC_BOOKLISTGRID2, -601 /* DblClick */, OnDblClickBooklistgrid2, VTS_NONE)
	ON_EVENT(CSearchBookListDlg, IDC_BOOKLISTGRID2, -604 /* KeyUp */, OnKeyUpBooklistgrid2, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSearchBookListDlg::OnKeyUpBooklistgrid(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	EFS_END
}

VOID CSearchBookListDlg::OnClickBooklistgrid() 
{
	EFS_BEGIN

	SetSelect();

	EFS_END
}

VOID CSearchBookListDlg::OnDblClickBooklistgrid() 
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3115") );
	if( pCM == NULL ) return;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("BOBookListGrid") ));
	if( pGrid == NULL ) return;
	if( pGrid->GetMouseRow() < 1 )
	{
		PostMessage( WM_USER+100 , 0 , 0 );
		return ;
	}

	{
		OnbtnSELECT();	
	}

	EFS_END
}

BOOL CSearchBookListDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	HWND MsgH = pMsg->hwnd;

	if(pMsg->message == WM_KEYUP && ( (pMsg->wParam == VK_UP) || (pMsg->wParam == VK_DOWN) )  )
	{
		if(	GetDlgItem(IDC_BOOKLISTGRID)->m_hWnd==MsgH )
		{
			SetSelect();
			return TRUE;
		}
	}
	
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnbtnCANCEL();
		return TRUE;
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CSearchBookListDlg::OnbtnBOOKRESERVATION() 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelectRegNo();
	if( ids < 0 ) return;
	if( ids > 0 ) return;

	m_nCloseMode = 2;

	if(m_sRegNo.IsEmpty())
	{
		AfxMessageBox(_T("등록번호가 없는 자료는 예약 할 수 없습니다."));
		return;
	}

	if(_T("단행정리대상자료") == m_sWorkingStatus ||
		_T("정리대상자료") == m_sWorkingStatus || 
		_T("긴급정리대상자료") == m_sWorkingStatus 
		)
	{
		AfxMessageBox(m_sWorkingStatus + _T("는 예약하실수 없는 자료입니다."));
		return;
	}	
	m_pParent->LoanReservationInSearchBookList(this->m_sRegNo, this->m_sPublishForm, 21 , IsDlgButtonChecked(IDC_chkReq_RESERVATION));	

	EFS_END
}



VOID CSearchBookListDlg::OnbtnSELECT() 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelectRegNo();
	if( ids < 0 ) return;
	if( ids > 0 ) return;

	m_nCloseMode = 0;

	if ( m_nEntryPoint == 1 ) 
	{
		if( IDNO==(AfxMessageBox(_T("선택된 자료를 대출하시겠습니까?"), MB_ICONQUESTION|MB_YESNO)) )
			return;

		OnOK();
	}
	else
	{
		if(m_sRegNo.IsEmpty())
		{
			AfxMessageBox(_T("등록번호가 없는 자료는 대출 할 수 없습니다."));
			return;
		}

		if(_T("단행정리대상자료") == m_sWorkingStatus ||
			_T("정리대상자료") == m_sWorkingStatus || 
			_T("긴급정리대상자료") == m_sWorkingStatus 
			)
		{
			AfxMessageBox(m_sWorkingStatus + _T("는 대출하실수 없는 자료입니다."));
			return;
		}
		
		if( IDNO==(AfxMessageBox(_T("선택된 자료를 대출하시겠습니까?"), MB_ICONQUESTION|MB_YESNO)) )
			return;

		m_pParent->LoanReservationInSearchBookList(this->m_sRegNo, this->m_sPublishForm, 11, FALSE);	
	}

	EFS_END
}


VOID CSearchBookListDlg::OnbtnCANCEL() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

void CSearchBookListDlg::OnCancel()
{
EFS_BEGIN

	m_nCloseMode = 1;
	CDialog::OnCancel();

EFS_END
}


INT CSearchBookListDlg::ControlChange()
{
	EFS_BEGIN

	if( m_nEntryPoint == 1 )
	{
		GetDlgItem(IDC_btnBOOK_RESERVATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSELECT)->SetWindowText(_T("선택"));
	}
	
	if( _T("0") == m_pParent->m_pCheckLoan->m_pManageValue->m_sReserveMode )
		GetDlgItem(IDC_btnBOOK_RESERVATION)->ShowWindow(SW_HIDE);		

	return 0;

	EFS_END

	return -1;
}
VOID CSearchBookListDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	
	if( GetSafeHwnd() == NULL )	
		return;

	CWnd* pGrid = GetDlgItem(IDC_BOOKLISTGRID);
	if( pGrid->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pGrid2 = GetDlgItem(IDC_BOOKLISTGRID2);
	if( pGrid->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pBottomBar = GetDlgItem(IDC_staBOTTOMLINE);
	if( pBottomBar->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pReserveButton = GetDlgItem(IDC_btnBOOK_RESERVATION);
	if( pReserveButton->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pLoanButton = GetDlgItem(IDC_btnSELECT);
	if( pLoanButton->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pCloseButton = GetDlgItem(IDC_btnLOAN_CANCEL);
	if( pCloseButton->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pTab = GetDlgItem(IDC_tabMENU);
	if( pTab->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pDetailButton = GetDlgItem(IDC_btnBOOK_DETAIL);
	if( pDetailButton->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pLoanUserViewButton = GetDlgItem(IDC_btn3115_LoanUserView);
	if( pLoanUserViewButton->GetSafeHwnd() == NULL ) 
		return;
	CWnd* pReqPriorReservation = GetDlgItem(IDC_chkReq_RESERVATION);
	if( pReqPriorReservation->GetSafeHwnd() == NULL ) 
		return;
	
	CRect WndPos , ButtonPos;
	GetWindowRect( WndPos );
	pReserveButton->GetWindowRect( ButtonPos );

	pGrid->MoveWindow( 10 , 30 ,cx-20, cy-82 );
	pGrid2->MoveWindow( 10 , 30 ,cx-20, cy-82 );
	pBottomBar->MoveWindow( -5 , cy - 40 , cx+10 , cy - 40 );

	pReserveButton->MoveWindow( cx - 277  , cy - 30 , ButtonPos.Width() , ButtonPos.Height() );
	pLoanButton->MoveWindow( cx - 187  , cy - 30 , ButtonPos.Width() , ButtonPos.Height() );
	pCloseButton->MoveWindow( cx - 97  , cy - 30 , ButtonPos.Width() , ButtonPos.Height() );
	pDetailButton->MoveWindow( cx - 377  , cy - 30 , ButtonPos.Width() , ButtonPos.Height() );
	pLoanUserViewButton->MoveWindow( cx - 477  , cy - 30 , ButtonPos.Width() , ButtonPos.Height() );
	pReqPriorReservation->MoveWindow( 10  , cy - 30 , ButtonPos.Width()+60, ButtonPos.Height() );
	pTab->MoveWindow( 0 , 0 , cx , cy - 40 );
	
	EFS_END
	
}

VOID CSearchBookListDlg::OnSelchangetabMENU(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	GridSelectView();
	
	*pResult = 0;

	EFS_END
}


INT CSearchBookListDlg::GridSelectView()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);

	INT nIndex = pTab->GetCurSel();

	if( nIndex == 0 )
	{
		GetDlgItem(IDC_BOOKLISTGRID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOKLISTGRID2)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BOOKLISTGRID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOOKLISTGRID2)->ShowWindow(SW_SHOW);
	}
	return 0;

	EFS_END

	return -1;

}

CString CSearchBookListDlg::GetSelectGridAlias()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);

	INT nIndex = pTab->GetCurSel();

	if( nIndex == 0 )
	{
		return _T("BOBookListGrid");
	}
	else
	{
		return _T("SEBookListGrid");
	}

	EFS_END
	
	return _T("");
}

CString CSearchBookListDlg::GetSelectDMAlias()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);

	INT nIndex = pTab->GetCurSel();

	if( nIndex == 0 )
	{
		return _T("DM_BO_LOC_3110_BO");
	}
	else
	{
		return _T("DM_BO_LOC_3110_SE");
	}

	EFS_END

	return _T("");

}

VOID CSearchBookListDlg::OnClickBooklistgrid2() 
{
	EFS_BEGIN

	SetSelect();

	EFS_END
}

VOID CSearchBookListDlg::OnDblClickBooklistgrid2() 
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3115") );
	if( pCM == NULL ) return;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("SEBookListGrid") ));
	if( pGrid == NULL ) return;
	if( pGrid->GetMouseRow() < 1 )
	{
		PostMessage( WM_USER+100 , 0 , 0 );
		return ;
	}

	{
		OnbtnSELECT();	
	}

	EFS_END
}

VOID CSearchBookListDlg::OnKeyUpBooklistgrid2(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	EFS_END
}

VOID CSearchBookListDlg::OnbtnBOOKDETAIL() 
{
	EFS_BEGIN

	ShowBookDetailInfo();


	EFS_END	
}

INT CSearchBookListDlg::ShowBookDetailInfo()
{
	EFS_BEGIN

	INT nMode = ((CTabCtrl*)GetDlgItem(IDC_tabMENU))->GetCurSel();

	CString sCMAlias = _T("CM_BO_LOC_3115");
	CString sDMAlias;
	CString sGridAlias;
	if( nMode == 0 )
	{
		sGridAlias = _T("BOBookListGrid");
		sDMAlias = _T("DM_BO_LOC_3110_BO");
	}
	else
	{
		sGridAlias = _T("SEBookListGrid");
		sDMAlias = _T("DM_BO_LOC_3110_SE");		
	}

	CESL_DataMgr* pDM = FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowBookDetailInfo") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("상세보기할 목록이 존재하지 않습니다.") );
		return 0;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( sCMAlias , sGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ShowBookDetailInfo") );

	INT nStartIndex = pGrid->GetIdx();

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("종KEY") , i , sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( i );
	}
	
	if( nMode == 0 )
	{
		CBOBookDetailViewerDlg Dlg(this);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartIndex );
		Dlg.DoModal();
	}
	else
	{
		CSEBookDetailViewerDlg Dlg(this);
		
		Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartIndex );
		Dlg.DoModal();
	}
	

	return 0;

	EFS_END

	return -1;
}

VOID CSearchBookListDlg::SetParent(CBL_LOC_LOAN_PROC* pParent)
{
	m_pParent = pParent;
}

HBRUSH CSearchBookListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

LRESULT CSearchBookListDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CSearchBookListDlg::OnNcActivate(BOOL bActive) 
{
	return CDialog::OnNcActivate(bActive);
}

void CSearchBookListDlg::Onbtn3115LoanUserView() 
{
	INT ids;
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3115"), GetSelectGridAlias());
	if(pGrid == NULL)
	{
		ERROR_MSG_RETURN_VOID2(-1000, _T("Onbtn3115LoanUserView"));
	}
	CESL_DataMgr* pDM = FindDM(GetSelectDMAlias());
	if(pDM == NULL)
	{
		ERROR_MSG_RETURN_VOID2(-1010, _T("Onbtn3115LoanUserView"));
	}
	if(pDM->GetRowCount() == 0)
	{
		m_pParent->SelfCloseMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	INT nIndex = pGrid->GetIdx();
	if( nIndex < 0 )
	{
		m_pParent->SelfCloseMessageBox(_T("자료를 다시 선택해 주십시오."));
		return;
	}
	CString strBookKey;
	ids = pDM->GetCellData(_T("책KEY"), nIndex, strBookKey);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_VOID2(-1020, _T("Onbtn3115LoanUserView"));
	}
	CString strQuery, strUserKey;
	strQuery.Format(_T("SELECT MAX(USER_KEY) FROM LS_WORK_T01 ")
					_T("WHERE STATUS IN('0','2','L','D') AND BOOK_KEY=%s"), strBookKey);
	pDM->GetOneValueQuery(strQuery, strUserKey);
	if(strUserKey.IsEmpty())
	{
		m_pParent->SelfCloseMessageBox(_T("대출중인 이용자가 없습니다."));
		return;
	}
	if((!strUserKey.IsEmpty())&&(m_pParent->m_pLoanShareManager!=NULL)&&(m_pParent->m_pManageValue!=NULL))
	{
		CBO_LOC_3320 ViewMember(this, m_pParent->m_pLoanShareManager, m_pParent->m_pManageValue);
		ViewMember.m_nAction = ACTION_READ_F_LOANUSERINFO;
		ViewMember.SetMode(6, TRUE);
		ViewMember.SetRecKey(strUserKey);
		ViewMember.DoModal();
	}
	return;
}

void CSearchBookListDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}
