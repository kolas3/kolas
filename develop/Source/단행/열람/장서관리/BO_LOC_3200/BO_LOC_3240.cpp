// BO_LOC_3240.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3240.h"

#include "..\\BL_LOC_LOAN\\ReserveLinkBookInfoDlg.h"
#include "..\\BL_LOC_LOAN\\ReserveComplete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3240 dialog


CBO_LOC_3240::CBO_LOC_3240(CESL_Mgr* pParent , CLonaManageValue *pLona, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3240)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pLona = pLona;

	m_pSpfWork = NULL;
	m_pSpfWork = new CBL_LOC_LOAN_MNG_PROC(this,m_pLona,m_pLoanShareManager);

	m_pParent = pParent;

	m_sParentCMAlias = _T("CM_BO_LOC_3200");
	m_sParentDMAlias = _T("DM_BO_LOC_3200");
	m_sGridAlias = _T("MainGrid");

	m_nViewMode = 0;
	m_nCurrentIndex = 0;
	m_nLastIndex = 0;
	m_nFirstIndex = 0;
	bFirstView = FALSE;

}

CBO_LOC_3240::~CBO_LOC_3240()
{
	delete m_pSpfWork;
}

VOID CBO_LOC_3240::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3240)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3240, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3240)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDetail, OnbtnDetail)
	ON_BN_CLICKED(IDC_btnRESERVE_SELECT, OnbtnRESERVESELECT)
	ON_BN_CLICKED(IDC_btnRESERVE_CANCEL, OnbtnRESERVECANCEL)
	ON_BN_CLICKED(IDC_btnRESERVE_DUEDATE, OnbtnRESERVEDUEDATE)
	ON_BN_CLICKED(IDC_btnRESERVE_SENDMAIL, OnbtnRESERVESENDMAIL)
	ON_BN_CLICKED(IDC_btnRCOMPLETE, OnbtnRCOMPLETE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_RESERVEMARK_CANCEL, OnbtnReserveMarkCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3240 message handlers

VOID CBO_LOC_3240::OnClose() 
{
EFS_BEGIN

	// TODO: Add your message handler code here and/or call default

	CDialog::OnCancel();

EFS_END
}

BOOL CBO_LOC_3240::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );
	
	// TODO: Add extra initialization here
	if( 0 > InitESL_Mgr(_T("BO_LOC_3240")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabLOG);

	TC_ITEM item;

	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;

	item.pszText = _T("예약 자료");
	pTab->InsertItem(0, &item);

	CESL_Grid*	pGrid	=	(CESL_Grid*)GetDlgItem( IDC_gridRESERVATION );
	if( TRUE != m_pLona->m_bVolReserve )
	{
		INT		nReserveMarkIdx = -1;
		CLocCommonAPI::GetGridColumnIndexs( m_pParent , pGrid , _T("지정여부") , nReserveMarkIdx );
		pGrid->SetColWidth( nReserveMarkIdx+1 , 0 , 0 );
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("예약자료처리");
	}

	CString		strSMSSystemUseMode;
	INT		ids = m_pParent->m_pInfo->GetSMSUseMode();
	if( 1 == ids )
	{
		strSMSSystemUseMode	=	_T("Y");
	}
	else
	{
		strSMSSystemUseMode	=	_T("N");
	}

	if(		_T("Y") != strSMSSystemUseMode				
		||	_T("Y") != m_pLona->m_sReserveExpireSMS )	
	{
		INT		nSMSResultIdx = -1;
		CLocCommonAPI::GetGridColumnIndexs( m_pParent , pGrid , _T("SMS발송결과") , nSMSResultIdx );
		pGrid->SetColWidth( nSMSResultIdx+1 , 0 , 0 );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

BOOL CBO_LOC_3240::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}


CESL_Grid* CBO_LOC_3240::GetMainGridPtr()
{
EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( m_sParentCMAlias );
	if( NULL == pCM ) 
	{
		return NULL;
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( m_sGridAlias ));
	if( NULL == pGrid ) 
	{
		return NULL;
	}

	return pGrid;

EFS_END
return NULL;
}

CESL_Grid* CBO_LOC_3240::GetClientGridPtr()
{
EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_3240");
	CString sGridAlias = _T("ReservationProcessGrid");

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( NULL == pCM ) 
	{
		return NULL;
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( NULL == pGrid ) 
	{
		return NULL;
	}

	return pGrid;

EFS_END
return NULL;
}

INT CBO_LOC_3240::ViewCurrentWindow()
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( &pDM == NULL ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewCurrentWindow") ); 
	}

	pDM->FreeData();

	CString *sColAlias;
	INT nColCnt = pDM->RefList.GetCount();
	sColAlias = new CString[nColCnt];
	ids = pDM->GetAllAlias( sColAlias , nColCnt );

	CString sParentDMAlias = m_sParentDMAlias;
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( ids < 0 ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewCurrentWindow") ); 	
	}

	m_pSpfWork->SPFExcutePath(_T("예약순서재부여"));
	
	CString sParentCMAlias = m_sParentCMAlias;
	CESL_Grid* pParentGrid = (CESL_Grid*)FindCM( sParentCMAlias )->FindControl( _T("MainGrid") );
	pParentGrid->SelectMakeList();
	INT nSelectCnt = pParentGrid->SelectGetCount();
	
	INT nMode = 0;
	if ( nSelectCnt ) 
	{
		nMode = 1;
	}
	else
	{
		nSelectCnt = pParentGrid->GetCount();
	}

	CString sTmpData;
	CString sLoanStatus;
	CString sBookKey;
	CString sLoanKey;
	CString sSql;
	CString sOrder;

	INT i , nCurIdx = 0;

	for( i=0 ; i<nSelectCnt ; i++ )
	{
		if ( nMode )	
		{
			nCurIdx = pParentGrid->SelectGetIdx();
		}
		else
		{
			nCurIdx = i;
		}

		ids = pParentDM->GetCellData( _T("대출상태") , nCurIdx , sLoanStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ViewCurrentWindow") ); 
		}

		if ( TRUE == m_pLona->m_bVolReserve )
		{
			if( 0 != sLoanStatus.Find( _T("예약(") ) )
			{
				if ( nMode )	
				{
					pParentGrid->SelectGetNext();
				}

				continue;
			}
		}
		else
		{
			if( 0 != sLoanStatus.Compare( _T("예약") ) )
			{
				if( 0 != sLoanStatus.Compare( _T("예약(무인대출신청)") ) )
				{
					if ( nMode )	
					{
						pParentGrid->SelectGetNext();
					}

					continue;
				}
			}
		}

		pDM->InsertRow(-1);
		INT		j;
		for( j=0 ; j<nColCnt ;j++ )
		{
			if(		( 0 == sColAlias[j].Compare(_T("부모INDEX")) ) 
				||	( 0 == sColAlias[j].Compare(_T("선정")) ) 
				||	( 0 == sColAlias[j].Compare(_T("SMS발송결과")) )  
				||  ( 0 == sColAlias[j].Compare(_T("회원상태설명")) ))
			{
				continue;
			}

			ids = pParentDM->GetCellData( sColAlias[j] , nCurIdx  , sTmpData );
			if( 0 > ids )
			{
				CString sMsg;
				sMsg.Format( _T("sColAlias : %s , i : %d ") , sColAlias[j] , nCurIdx );
				AfxMessageBox(sMsg);
			}
			
			if( 0 > ids && ( -4012 != ids ) ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("CBO_LOC_3240::ViewCurrentWindow()") );
			}

			if(0 == sColAlias[j].Compare(_T("무인대출예약")))
			{
				if(_T("Y") == sTmpData || _T("O") == sTmpData)
				{
					sTmpData = _T("V");
				}
			}

			ids = pDM->SetCellData( sColAlias[j] , sTmpData , pDM->GetRowCount() - 1 );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ViewCurrentWindow") ); 
			}
		}

		CString sParentIndex;
		sParentIndex.Format( _T("%d") , nCurIdx );
		ids = pDM->SetCellData( _T("부모INDEX") , sParentIndex , pDM->GetRowCount()-1 );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("ViewCurrentWindow") ); 
		}

		if ( nMode )	
		{
			pParentGrid->SelectGetNext();
		}
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("대출소속정보") , _T("소속")  );
	
	INT		nRowCnt		=	pDM->GetRowCount();
	for( i=0 ; i<nRowCnt ; i++)
	{
		CString sData, sDesc;
		ids = pDM->GetCellData( _T("관리구분") , i, sData);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("ViewCurrentWindow") ); 
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );
		ids = pDM->SetCellData( _T("관리구분설명") , sDesc, i );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("ViewCurrentWindow") ); 
		}
	}

	CString sCMAlias = _T("CM_BO_LOC_3240");
	ids = AllControlDisplay( sCMAlias );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ViewCurrentWindow") );
	}

	delete []sColAlias;

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3240::OnbtnClose() 
{
	EFS_BEGIN
	
	OnClose();

	EFS_END
}


INT CBO_LOC_3240::CheckParentDMCount()
{
EFS_BEGIN

	CESL_DataMgr *pParentDM = FindDM( m_sParentDMAlias );

	return pParentDM->GetRowCount();

EFS_END
return -1;
}

VOID CBO_LOC_3240::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	INT ids = 0;

	ids = SetScreenFromUtil( );
	if( 0 > ids ) 
	{
		return;
	}

	ids = ViewCurrentWindow();
	if( 0 > ids ) 
	{
		return;
	}
	
EFS_END
}

INT CBO_LOC_3240::SetScreenFromUtil()
{
	INT ids = 0;
	
	if ( TRUE != (m_pLona->m_bVolReserve) )
	{
		CRect rect1, rect2, rect3, rect4;

		CButton* pButton1 = ( CButton* )GetDlgItem(IDC_btnRCOMPLETE);
		CButton* pButton2 = ( CButton* )GetDlgItem(IDC_BTN_RESERVEMARK_CANCEL);
		CButton* pButton3 = ( CButton* )GetDlgItem(IDC_btnRESERVE_DUEDATE);
		CButton* pButton4 = ( CButton* )GetDlgItem(IDC_btnDetail);

		pButton1->GetWindowRect( &rect1 );
		pButton2->GetWindowRect( &rect2 );
		pButton3->GetWindowRect( &rect3 );
		pButton4->GetWindowRect( &rect4 );

		ScreenToClient( rect1 );
		ScreenToClient( rect2 );
		ScreenToClient( rect3 );
		ScreenToClient( rect4 );

		pButton4->MoveWindow( rect2.left, rect4.top, rect4.Width( ), rect4.Height( ) );
		pButton3->MoveWindow( rect1.left, rect3.top, rect3.Width( ), rect3.Height( ) );

		pButton2->ShowWindow( FALSE );
		pButton1->ShowWindow( FALSE );
	}

	return 0;
}

VOID CBO_LOC_3240::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl( cx , cy );

EFS_END
}


VOID CBO_LOC_3240::ResizeControl( INT cx , INT cy )
{
EFS_BEGIN

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridRESERVATION);
	
	if( NULL == pGrid->GetSafeHwnd() ) 
	{
		return;
	}

	CRect TopRect;
	CRect BottomRect;
	CRect TopRect2;
	CRect LeftRect;
	CRect ButtonBarRect;
	CRect CloseButton;

	GetDlgItem(IDC_TOP_POS)->GetWindowRect(&TopRect);
	GetDlgItem(IDC_BOTTOM_POS)->GetWindowRect(&BottomRect);
	GetDlgItem(IDC_TOP_POS2)->GetWindowRect(&TopRect2);
	GetDlgItem(IDC_LEFT_POS)->GetWindowRect(&LeftRect);
	GetDlgItem(IDC_BUTTON_BAR_POS)->GetWindowRect(&ButtonBarRect);
	GetDlgItem(IDC_btnClose )->GetWindowRect(&CloseButton);

	pGrid->MoveWindow( LeftRect.Width() , TopRect.Height() , cx - (LeftRect.Width()*2), cy-BottomRect.Height()-TopRect.Height());

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabLOG);
	pTab->MoveWindow( 0 , TopRect2.Height() , cx , cy - TopRect2.Height() );
	
	GetDlgItem( IDC_BUTTON_BAR_POS )->MoveWindow( 0 , 0 , cx , ButtonBarRect.Height() , cy );

	ScreenToClient( &CloseButton );
	GetDlgItem( IDC_btnClose )->MoveWindow( cx - CloseButton.Width() - 30 , CloseButton.top , CloseButton.Width() , CloseButton.Height() );

EFS_END
}

INT CBO_LOC_3240::SelectExpireDatePassData()
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SelectExpireDatePassData") ); 
	}

	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SelectExpireDatePassData") ); 
	}

	CString strGetData;
	CTime tCurrent = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CString sReservationExpireDate;

	INT		nCheckColIndex = -1;
	CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("선정") , nCheckColIndex );
	
	INT		i , nRowCnt;
	nRowCnt		=	pDM->GetRowCount();	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("예약만기일") , i , sReservationExpireDate );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SelectExpireDatePassData") ); 
		}

		if( TRUE == sReservationExpireDate.IsEmpty() ) 
		{
			continue;
		}

		CTime tExpireDate( _ttoi( sReservationExpireDate.Mid( 0 , 4 ) ) , _ttoi( sReservationExpireDate.Mid( 5 , 2 ) ) , _ttoi( sReservationExpireDate.Mid( 8 , 2 ) ) , 0 , 0 , 0  ); 
		
		if( tExpireDate < CTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), 0, 0, 0 ) )
		{
			pGrid->SetAt( i , nCheckColIndex , _T("V") );
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3240::OnbtnDetail() 
{
EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_예약처리_회원정보") );	

EFS_END
}

VOID CBO_LOC_3240::OnbtnRESERVESELECT() 
{
EFS_BEGIN

	INT ids;
	ids = SelectExpireDatePassData();

EFS_END
}

VOID CBO_LOC_3240::OnbtnRESERVECANCEL() 
{
EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_예약취소") );

EFS_END
}

VOID CBO_LOC_3240::OnbtnRCOMPLETE() 
{
EFS_BEGIN

	RComplete();

EFS_END
}

VOID CBO_LOC_3240::OnbtnReserveMarkCancel()
{
EFS_BEGIN

	INT	ids;

	if( AfxMessageBox(_T("선정된 이용자의 예약지정을 취소하시겠습니까?"), MB_YESNO) != IDYES )
	{
		return;
	}

	ids = ReserveMarkCancel();
	if ( 0 > ids )
	{
		AfxMessageBox(_T("예약지정 취소를 할 수 없습니다.") , MB_ICONINFORMATION);
		return;
	}
	else if ( 0 == ids )
	{
		AfxMessageBox(_T("예약지정이 취소되었습니다.") , MB_ICONINFORMATION);
		return;
	}

EFS_END
}

VOID CBO_LOC_3240::OnbtnRESERVEDUEDATE() 
{
EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_예약만기일부여") );

EFS_END
}

VOID CBO_LOC_3240::OnbtnRESERVESENDMAIL() 
{
EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_반납확인메일보내기") );	

EFS_END
}

INT CBO_LOC_3240::GetPublishForm( CString &sPublishForm )
{
EFS_BEGIN

	INT ids;

	ids = GetControlData( _T("CM_BO_LOC_3200_ETC_INFO") , _T("단행/연속구분") , sPublishForm );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetPublishForm") );
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3240::RComplete()
{
EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("RComplete") );
	}

	INT nSIndex = pGrid->GetIdx();
	

	CString sDMAlias = _T("DM_BO_LOC_3240_BO");

	CESL_DataMgr* pDMTmp = FindDM( sDMAlias );
	INT		nRowCnt		=	pDMTmp->GetRowCount();
	if ( 1 > nRowCnt ) 
	{
		return -1;
	}

	CReserveInfo RI;
	ids = this->GetDataMgrData( sDMAlias , _T("단행/연속구분") , RI.sPublishFormCode , nSIndex );
	if( 0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("RComplete") );
	}

	ids = this->GetDataMgrData( sDMAlias , _T("종KEY") , RI.sSpeciesKey , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("RComplete") );
	}

	ids = this->GetDataMgrData( sDMAlias , _T("권호정보KEY") , RI.sVolInfo, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("RComplete") );
	}

	CString		sReserveMark;
	ids = pDMTmp->GetCellData( _T("지정여부") , nSIndex , sReserveMark );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("RComplete") );
	}

	if( 0 == sReserveMark.Compare( _T("O") ) )
	{
		AfxMessageBox(_T("선택한 예약자는 이미 예약지정이 되어 있습니다.") , MB_ICONINFORMATION );
		return 0;
	}

	if(_T("Y") == m_pLona->m_sUnmannedReserveSys)
	{
		if(!m_pLona->m_bVolReserve)
		{
			CString strUnmanned;
			ids = pDMTmp->GetCellData(_T("무인대출예약"), nSIndex, strUnmanned);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("RComplete"));
			if(!strUnmanned.IsEmpty())
			{
				AfxMessageBox(_T("무인예약 자료입니다.") , MB_ICONINFORMATION );
				return 0;
			}
		}
	}

	CReserveLinkBookInfoDlg BookDlg(this);

	BookDlg.m_nMode = 1;

	BookDlg.SetIsAutoSetReservationDay( m_pLona->m_sIsAutoSetReservationDay );
	BookDlg.SetInfo(RI);
	BookDlg.LoadBookInfoFromDB();
	
	CESL_DataMgr*	pBookDM		=	BookDlg.FindDM( _T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG") );
	INT		nBookInfoDMRowCnt	=	pBookDM->GetRowCount();

	if( 1 > nBookInfoDMRowCnt )
	{
		AfxMessageBox( _T("예약지정 할 수 있는 자료가 없습니다.") , MB_ICONINFORMATION );
		return 0;
	}
	
	ids = BookDlg.DoModal();
	if( 5 == ids )
	{
		AfxMessageBox( _T("예약지정 할 수 있는 자료가 없습니다.") , MB_ICONINFORMATION );
		return 0;
	}
	
	ids = BookDlg.GetInfo(RI);

	if( 0 < ids ) 
	{
		return 0;
	}

	CString sLoanKey;
	ids = this->GetDataMgrData( sDMAlias , _T("대출/반납정보KEY") , sLoanKey, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("RComplete") );
	}
	
	CESL_DataMgr* pUpdateDM = this->FindDM( sDMAlias );
	if( NULL == pUpdateDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("RComplete") );
	}

	CReserveComplete RC;
	ids = RC.DBReserveComplete( RI , this , sLoanKey , pUpdateDM );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("RComplete") );
	}

	ids = this->SetDataMgrData( sDMAlias , _T("대출책KEY") , RI.sBookKey , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("RComplete") );
	}

	ids = this->SetDataMgrData( sDMAlias , _T("등록번호") , RI.sRegNo , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("RComplete") );
	}

	ids = this->SetDataMgrData( sDMAlias , _T("지정여부") , _T("O"), nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("RComplete") );
	}

	ids = this->SetDataMgrData( sDMAlias , _T("선정") , _T("V"), nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("RComplete") );
	}

	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("RComplete") );
	}

	CString sStatus , sSpeciesKey , sVolInfoKey;
	INT		i;
	for( i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("종KEY") , i , sSpeciesKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("RComplete") );
		}

		ids = pDM->GetCellData( _T("권호정보KEY") , i , sVolInfoKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("RComplete") );
		}

		if( ( 0 == sSpeciesKey.Compare(RI.sSpeciesKey) ) && ( 0 == sVolInfoKey.Compare(RI.sVolInfo) ) )
		{
			m_pSpfWork->ReViewOrder( pDM , i );
			pGrid->DisplayLine(i);
		}
	}

	CESL_DataMgr* pParentDM = FindDM( _T("DM_BO_LOC_3200") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("RComplete") );
	}

	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3200") , _T("MainGrid") );
	if( NULL == pParentGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("RComplete") );
	}

	for( i = 0 ; i < pParentDM->GetRowCount() ; i++ )
	{
		ids = pParentDM->GetCellData( _T("대출상태") , i , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("RComplete") );
		}

		ids = pParentDM->GetCellData( _T("종KEY") , i , sSpeciesKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("RComplete") );
		}

		ids = pParentDM->GetCellData( _T("권호정보KEY") , i , sVolInfoKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1200 , _T("RComplete") );
		}

		if(		( 0 == sStatus.Find(_T("예약(")) ) 
			&&	( 0 == sSpeciesKey.Compare(RI.sSpeciesKey) ) 
			&&	( 0 == sVolInfoKey.Compare(RI.sVolInfo) )		)
		{
			m_pSpfWork->ReViewOrder( pParentDM , i );
			pParentGrid->DisplayLine(i);
		}
	}

	CString sParentIndex;
	ids = pDM->GetCellData( _T("부모INDEX") , nSIndex , sParentIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1210 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("대출상태") , _T("예약(지정완료)") , _ttoi(sParentIndex) );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1220 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("등록번호") , RI.sRegNo , _ttoi(sParentIndex) );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1230 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("대출책KEY") , RI.sBookKey, _ttoi(sParentIndex) );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1240 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("IBS_청구_별치기호_코드") , RI.sSeparateShelfCode , _ttoi(sParentIndex) );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1250 , _T("RComplete") );
	}

	CLocCommonAPI::GetWorkingStatusDescToCode( RI.sWorkingStatus , 1 );
	ids = pParentDM->SetCellData( _T("배가상태") , RI.sWorkingStatus , _ttoi(sParentIndex) );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1260 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("청구기호") , RI.sCallNo , _ttoi(sParentIndex) );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1270 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("이전자료상태") , RI.sPrevWorkingStatus , _ttoi(sParentIndex) );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1280 , _T("RComplete") );
	}

	ids = pParentDM->SetCellData( _T("지정여부"), _T("O") , _ttoi(sParentIndex) );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1290 , _T("RComplete") );
	}

	m_pSpfWork->ReViewOrder( pParentDM , _ttoi(sParentIndex) );

	ids = m_pSpfWork->SPFExcutePath( _T("대출자료관리_예약만기일부여") );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1300 , _T("RComplete") );
	}

	pParentGrid->DisplayLine( _ttoi(sParentIndex) );

	return 0;

EFS_END
return -1;
}

HBRUSH CBO_LOC_3240::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBO_LOC_3240::PreTranslateMessage(MSG* pMsg) 
{
	if( (WM_KEYDOWN == pMsg->message) && (VK_ESCAPE  == pMsg->wParam) )
	{
		OnbtnClose();
		return TRUE;
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

INT CBO_LOC_3240::ReserveMarkCancel()
{
EFS_BEGIN

	INT ids = 0;
	INT nSIndex = 0;
	CString sLoanKey = _T("");
	CString	sIsComplete;

	CESL_Grid*		pGrid		=	(CESL_Grid*)FindControl(_T("CM_BO_LOC_3240"),_T("ReservationProcessGrid"));
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ReserveMarkCancel") );
	}

	CESL_DataMgr*	pDM	=	FindDM( _T("DM_BO_LOC_3240_BO") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ReserveMarkCancel") );
	}

	nSIndex = pGrid->GetIdx();
	if ( 0 > nSIndex  )
	{
		AfxMessageBox( _T("선정된 이용자가 없습니다.") , MB_ICONINFORMATION);
		return 1;
	}

	CReserveInfo RI;
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3240_BO") , _T("단행/연속구분") , RI.sPublishFormCode , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ReserveMarkCancel") );
	}
	
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3240_BO") , _T("종KEY") ,			RI.sSpeciesKey , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ReserveMarkCancel") );
	}
	
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3240_BO") , _T("권호정보KEY") ,	RI.sVolInfo, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ReserveMarkCancel") );
	}

	CString		sReserveMark;
	ids = pDM->GetCellData( _T("지정여부") , nSIndex , sReserveMark );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("ReserveMarkCancel") );
	}

	if( 0 != sReserveMark.Compare( _T("O") ) )
	{
		AfxMessageBox( _T("선정된 이용자는 예약지정 되어 있지 않습니다.") , MB_ICONINFORMATION );
		return 2;
	}

	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nSIndex , sLoanKey );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("ReservLinkCancel") );
	}

	if(_T("Y") == m_pLona->m_sUnmannedReserveSys)
	{
		CString strQuery, strValue, strMsg;;
		strQuery.Format(_T("SELECT UNMANNED_RESERVATION_LOAN FROM LS_WORK_T01 WHERE REC_KEY=%s"), sLoanKey);
		pDM->GetOneValueQuery(strQuery, strValue);
		if(!strValue.IsEmpty())
		{
			if(_T("O") == strValue)
			{
				strMsg = _T("무인예약 대기중인 자료는 예약지정을 취소 할 수 없습니다.");
				MessageBox(strMsg, _T("예약지정취소"), MB_ICONWARNING);
				return 300;
			}
			else if(_T("Y") == strValue)
			{
				strMsg = _T("무인예약 신청중인 자료입니다.\r\n예약지정을 취소하시겠습니까?");
				if(IDYES != MessageBox(strMsg, _T("예약지정취소"), MB_YESNO | MB_ICONQUESTION))
				{
					return 300;
				}
			}
		}
	}

	pDM->StartFrame();	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("RESERVATION_EXPIRE_DATE"),	_T("STRING"),  _T(""), TRUE );
	pDM->AddDBFieldData( _T("REG_NO"),					_T("STRING"),  _T(""), TRUE );
	pDM->AddDBFieldData( _T("CALL_NO"),					_T("STRING"),  _T(""), TRUE );

	pDM->MakeUpdateFrame(_T("LS_WORK_T01"),				_T("REC_KEY"), _T("NUMERIC") , sLoanKey );

	ids = pDM->SendFrame();
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("ReserveMarkCancel") );
	}

	pDM->EndFrame();

	ids = pDM->SetCellData( _T("지정여부"),	_T("X") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ReserveMarkCancel") );
	}

	ids = pDM->SetCellData( _T("등록번호"),	_T("") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("ReserveMarkCancel") );
	}

	ids = pDM->SetCellData( _T("예약만기일"),_T("") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("ReserveMarkCancel") );
	}

	ids = pDM->SetCellData( _T("청구기호"),_T("") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("ReserveMarkCancel") );
	}

	pGrid->DisplayLine(nSIndex);

	CESL_DataMgr*	pParentDM	= FindDM( _T("DM_BO_LOC_3200") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("ReserveMarkCancel") );
	}

	CESL_Grid*		pParentGrid	= (CESL_Grid*)FindControl( _T("CM_BO_LOC_3200") , _T("MainGrid") );
	if( NULL == pParentGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("ReserveMarkCancel") );	
	}

	CString strParentIndex;
	ids = pDM->GetCellData( _T("부모INDEX") , nSIndex , strParentIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("ReserveMarkCancel") );
	}

	INT		nParentIndex	=	_ttoi( strParentIndex );
	pParentDM->SetCellData( _T("지정여부"),	_T("X") , nParentIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("ReserveMarkCancel") );
	}

	pParentDM->SetCellData( _T("등록번호"),	_T("") , nParentIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("ReserveMarkCancel") );
	}

	pParentDM->SetCellData( _T("예약만기일"), _T("") , nParentIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("ReserveMarkCancel") );
	}

	pParentDM->SetCellData( _T("대출상태"), _T("예약(미지정)") , nParentIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("ReserveMarkCancel") );
	}

	pParentDM->SetCellData( _T("배가상태"), _T("") , nParentIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1200 , _T("ReserveMarkCancel") );
	}

	pParentDM->SetCellData( _T("청구기호"), _T("") , nParentIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1210 , _T("ReserveMarkCancel") );
	}

	pParentGrid->DisplayLine( nParentIndex );

	return 0;

EFS_END
return -1;
}
