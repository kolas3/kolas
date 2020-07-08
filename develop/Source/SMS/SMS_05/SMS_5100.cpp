// SMS_5100.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_5100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_5100 dialog


CSMS_5100::CSMS_5100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_5100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sLifeLongFlag		=	_T("");
	m_bUseILL			=	FALSE;
	
	m_strSearchWhere	=	_T("");	
}

CSMS_5100::~CSMS_5100()
{
}


VOID CSMS_5100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_5100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_5100, CDialog)
	//{{AFX_MSG_MAP(CSMS_5100)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMSGOPTION, OnSelchangetabMSGOPTION)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_5100 message handlers

BOOL CSMS_5100::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT ids = InitESL_Mgr(_T("SMS_5100"));
	if( 0 > ids )
	{
		AfxMessageBox( _T("InitESL_Mgr Error!!") );
	}
	
	GetManageValue( _T("기타"), _T("공통"), _T("평생"), _T(""), m_sLifeLongFlag );
	m_sLifeLongFlag.TrimLeft();m_sLifeLongFlag.TrimRight();
	if ( m_sLifeLongFlag.IsEmpty() ) 
	{
		m_sLifeLongFlag = _T("N");
	}

	InitScreen();

	const INT GRID_COUNT = 12;
	CString gridAlias[ GRID_COUNT ] = { 
		_T("년별이력화면그리드"),			_T("월별이력화면그리드"),			_T("일별이력화면그리드"),
		_T("년별이력화면그리드_평생"),		_T("월별이력화면그리드_평생"),		_T("일별이력화면그리드_평생"),
		_T("년별이력화면그리드_카톡"),		_T("월별이력화면그리드_카톡"),		_T("일별이력화면그리드_카톡"),
		_T("년별이력화면그리드_평생_카톡"), _T("월별이력화면그리드_평생_카톡"), _T("일별이력화면그리드_평생_카톡")
	};

	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	INT		i;
	for( i=0 ; i<GRID_COUNT ; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SMS_5100"), gridAlias[ i ] );
		if( NULL == pGrid )
		{
			return -1;
		}

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}

	{		
		BOOL bGridHide = FALSE;
		CString strUseILL = _T("N");

		GetManageValue( _T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레사용여부"), strUseILL);

		if(strUseILL != _T("Y"))
			bGridHide = TRUE;

		if(bGridHide == TRUE)
		{
			CString strGridAlias[GRID_COUNT] = 
			{
				_T("년별이력화면그리드"),
				_T("년별이력화면그리드_평생"),
				_T("월별이력화면그리드"),
				_T("월별이력화면그리드_평생"),
				_T("일별이력화면그리드"),
				_T("일별이력화면그리드_평생")		
			};
			
			CESL_Grid* pGrid = NULL;
			int nGridCnt = 0;
			
			for( nGridCnt=0 ; nGridCnt<GRID_COUNT ; nGridCnt++ )
			{
				pGrid = (CESL_Grid*)FindControl(_T("CM_SMS_5100"), gridAlias[nGridCnt]);
				
				const int MAX_COL = pGrid->GetColCount();
				int nColCnt = 0, nColWidth = 0, nAlign = 0;
				CString strData;

				for( nColCnt=0 ; nColCnt<MAX_COL ; nColCnt++ )
				{
					pGrid->GetColumnInfo(nColCnt, strData, nColWidth, nAlign);

					if(strUseILL != _T("Y")){
						if( 0 <= strData.Find(_T("책두레")) )
						{
							pGrid->SetColumnInfo(nColCnt, strData, 0, nAlign);
						}
					}
				}
			}
		}

		if(strUseILL != _T("Y")){
			m_bUseILL = FALSE;
		}else{
			m_bUseILL = TRUE;
		}

	}
	ViewChangeGrid();

	CString query, value;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	query.Format(_T("SELECT MAX(VALUE_NAME) FROM ESL_MANAGE_TBL ")
				 _T("WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='SMS시스템정보' ")
				 _T("AND MANAGE_CODE=UDF_MANAGE_CODE") );

	dm.GetOneValueQuery(query,value);
	CString strButtonAlias = _T("전송이력조회");
	m_strSMSType = value;
	
	query.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구사용유무' AND MANAGE_CODE=UDF_MANAGE_CODE"));
	dm.GetOneValueQuery(query, value);
	m_useKatok = 0;
	if(	value.Find( _T('Y')) > -1 )
	{
		m_useKatok = 1;
	}
	CButton* pRadioBT_SMS	= (CButton*)GetDlgItem(IDC_RADIO4);
	CButton* pRadioBT_Katok	= (CButton*)GetDlgItem(IDC_RADIO5);
	if( m_useKatok == 1 )
	{
		pRadioBT_Katok->SetCheck( TRUE );
	}else{
		pRadioBT_SMS->SetCheck( TRUE );
		pRadioBT_Katok->ShowWindow( SW_HIDE );
	}

	ViewChangeGrid();
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CSMS_5100::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSMS_5100::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL )	 return;

	const INT nCnt = 13;
	INT nIDC[nCnt] = 
	{
		IDC_tabMSGOPTION , IDC_MAINGRID , IDC_MAINGRID2 , IDC_MAINGRID3, IDC_MAINGRID4, IDC_MAINGRID5, IDC_MAINGRID6,
		IDC_MAINGRID7 , IDC_MAINGRID8 , IDC_MAINGRID9, IDC_MAINGRID10, IDC_MAINGRID11, IDC_MAINGRID12
	};
	CWnd* pWnd;
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		pWnd = GetDlgItem(nIDC[i]);
		if(pWnd == NULL) continue;
		if( i == 0 )
		{
			pWnd->MoveWindow( 0, 0, cx+4, 30  );
		}else{
			pWnd->MoveWindow( 1, 60, cx-1, cy-75 );
		}
	}
}

INT CSMS_5100::InitScreen()
{
EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMSGOPTION);

	const INT nCnt = 3;

	CString sTitle[nCnt] =
	{
		_T("일별") , _T("월별") , _T("년별") 
	};
	
	INT		i;	
	for( i=0 ; i<nCnt ; i++ )
	{
		pTab->InsertItem( i , sTitle[i] );
	}

	return 0;

EFS_END
return -1;
}

int		CSMS_5100::GetSendPlatform_RadioBT()
{
	if(	((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck() )
	{
		return 1;
	}
	return 0;
}

INT CSMS_5100::PrintList()
{
	EFS_BEGIN

	INT ids;

	CString sClass[3] =
	{
		_T("일") , _T("월") , _T("년")
	};

	INT nIndex = ((CTabCtrl*)GetDlgItem(IDC_tabMSGOPTION))->GetCurSel();

	CString sDMAlias[6] =
	{
		_T("DM_SMS_5100_DAY") , _T("DM_SMS_5100_MONTH") , _T("DM_SMS_5100_YEAR") , 
		_T("DM_SMS_5100_DAY_KatokPL") , _T("DM_SMS_5100_MONTH_KatokPL") , _T("DM_SMS_5100_YEAR_KatokPL")
	};
	int idx_alias = nIndex;
	if( GetSendPlatform_RadioBT() > 0 ) idx_alias += 3;
	CESL_DataMgr *pSrcDM = FindDM( sDMAlias[idx_alias] );

	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;

	if (&pSIReportManager == NULL) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pSrcDM->CONNECTION_INFO);

	if(FALSE == m_bUseILL)
	{
		ids = pSIReportManager->GetSIReportFileFromDB(_T("일별문자발송이력"));
	}
	else
	{
		ids = pSIReportManager->GetSIReportFileFromDB(_T("일별문자발송이력_책두레"));
	}
	
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	pSIReportManager->SetTempVariable( _T("종류") , sClass[nIndex] );
	
	pSIReportManager->SetDataMgr(0, pSrcDM , 0);
	
	ids = pSIReportManager->Print();

	if (&pSIReportManager != NULL) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	
	return 0;

	EFS_END
	return -1;

}

INT CSMS_5100::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}


VOID CSMS_5100::OnSelchangetabMSGOPTION(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ViewChangeGrid();
	
	*pResult = 0;
}


INT CSMS_5100::ViewChangeGrid()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMSGOPTION);

	INT nIndex = pTab->GetCurSel();

	UINT uFlag = SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE;
	UINT uFlag_Show = SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW;
	UINT uFlag_Hide = SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW;

	if (_T("Y") == m_sLifeLongFlag) nIndex += 3;
	if( GetSendPlatform_RadioBT() > 0 ) nIndex += 6;
	const int nGridCnt = 12;
	int nGridID[nGridCnt] = 
	{
		IDC_MAINGRID  , IDC_MAINGRID2 , IDC_MAINGRID3, IDC_MAINGRID4  , IDC_MAINGRID5  , IDC_MAINGRID6,
		IDC_MAINGRID7 , IDC_MAINGRID8 , IDC_MAINGRID9, IDC_MAINGRID10 , IDC_MAINGRID11 , IDC_MAINGRID12
	};

	CWnd* pWnd = NULL;

	HDWP hdwp = ::BeginDeferWindowPos(nGridCnt);

	for(INT nCount=0; nCount < nGridCnt; nCount++)
	{	
		pWnd = (CWnd*)GetDlgItem(nGridID[nCount]);
		if(NULL != pWnd)
		{
			if(nIndex == nCount)
			{
				uFlag = uFlag_Show;
			}
			else
			{
				uFlag = uFlag_Hide;
			}

			::DeferWindowPos(
				hdwp, 
				pWnd->GetSafeHwnd(),
				HWND_TOP,                
				0,
				0,
				0,
				0,
				uFlag
				);
		}
	}

	EndDeferWindowPos(hdwp);
	UpdateWindow();
	
	return 0;

	EFS_END

	return -1;
}

HBRUSH CSMS_5100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

CString CSMS_5100::GetSearchWhere()
{
	return	m_strSearchWhere;
}

VOID CSMS_5100::SetSearchWhere( CString	strSearchWherer )
{
	m_strSearchWhere	=	strSearchWherer;
}

void CSMS_5100::OnRadio4() 
{
	ViewChangeGrid();
}

void CSMS_5100::OnRadio5() 
{
	ViewChangeGrid();
}
