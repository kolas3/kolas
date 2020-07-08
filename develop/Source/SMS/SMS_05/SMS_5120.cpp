// SMS_5120.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_5120.h"
#include "SMS_5100.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_5120 dialog


CSMS_5120::CSMS_5120(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_5120)
	m_tSendTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT	
	p_Mgr = pParent;
}

CSMS_5120::~CSMS_5120()
{
}


VOID CSMS_5120::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_5120)
	DDX_DateTimeCtrl(pDX, IDC_datSEND_DATE1, m_tSendTime);
	//}}AFX_DATA_MAP

	
}


BEGIN_MESSAGE_MAP(CSMS_5120, CDialog)
	//{{AFX_MSG_MAP(CSMS_5120)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmbSMSTYPE, OnSelchangecmbSMSTYPE)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnDELETE, OnbtnDELETE)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_5120 message handlers

BOOL CSMS_5120::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr(_T("SMS_5120"));
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("InitESL_Mgr Error!!"));
		return FALSE;
	}

	CSMS_5100* pcSMS_5100 = (CSMS_5100*)pParentMgr;
	CButton* pRadioBT_SMS	= (CButton*)GetDlgItem(IDC_RADIO4);
	CButton* pRadioBT_Katok	= (CButton*)GetDlgItem(IDC_RADIO5);
	if( pcSMS_5100->m_useKatok == 1 )
	{
		CButton*  p_pRadio_SMS = (CButton*)p_Mgr->GetDlgItem(IDC_RADIO4);
		if(p_pRadio_SMS->GetCheck()){
			pRadioBT_SMS->SetCheck( TRUE );
		}else {
			pRadioBT_Katok->SetCheck( TRUE );
		}	
	}else{
		pRadioBT_SMS->SetCheck( TRUE );
		pRadioBT_Katok->ShowWindow( SW_HIDE );
	}
	ids = SetSMSSystemInfo();
	if( ids < 0 )
	{
		EndDialog(TRUE);
		return FALSE;
	}

	ids = InitScreen();
	if( 0 < ids ) 
	{
		EndDialog(TRUE);
		return FALSE;
	}

	SearchHistory();

	CESL_Grid* pGrid = ((CESL_Grid*)GetDlgItem(IDC_DETAIL));
	CESL_Grid* pGrid2 = ((CESL_Grid*)GetDlgItem(IDC_DETAIL2));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		pGrid2->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("SMS 이력조회");
		pGrid2->m_bPrivacyPrintLogRecord = TRUE;
		pGrid2->m_strPrintInfo = _T("Katok 이력조회");
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
 }



VOID CSMS_5120::OnbtnCLOSE() 
{
	OnCancel();	
}

VOID CSMS_5120::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if( NULL == GetSafeHwnd() ) 
	{
		return ;
	}
	
	CWnd* pWnd = GetDlgItem(IDC_DETAIL);
	if( NULL == pWnd->GetSafeHwnd() ) 
	{
		return;
	}

	CWnd* pWnd2 = GetDlgItem(IDC_DETAIL2);
	if( NULL == pWnd2->GetSafeHwnd() ) 
	{
		return;
	}

	pWnd->MoveWindow( 5 , 70 , cx-10 , cy-80 );
	pWnd2->MoveWindow( 5 , 70 , cx-10 , cy-80 );

EFS_END
}

INT CSMS_5120::SearchHistory()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_SMS_5120"));

	if( GetSendPlatform_RadioBT() )
	{
		pDM = FindDM(_T("DM_SMS_5120_KatokPL"));
	}
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SearchHistory") );
	}

	CString sWhere;

	ids = GetWhere(sWhere);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SearchHistory") );
	}

	if( 0 < ids ) 
	{
		return 1;
	}
	CString strNotSMSType;
	if( FALSE == m_bUseILL )
	{
		if(strNotSMSType.IsEmpty() == FALSE) strNotSMSType += _T(", ");
		strNotSMSType += _T("'TLL01', 'TLL02', 'TLL03' ");
	}

	if(strNotSMSType.IsEmpty() == FALSE)
	{
		sWhere += _T(" AND SMS_TYPE NOT IN (") + strNotSMSType + _T(") ");
	}


	ids = pDM->RefreshDataManager( sWhere );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SearchHistory") );
	}

	ids = SetSuccessInfo( pDM );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SearchHistory") );
	}

	AllControlDisplay( _T("CM_SMS_5120") );
	ShowGrid_Toggle();

	return 0;

EFS_END
return -1;
}

int CSMS_5120::CheckExt_RowData()
{
	CESL_DataMgr* pDM_1 = FindDM( _T("DM_SMS_5100_DAY") );
	CESL_DataMgr* pDM_2 = FindDM( _T("DM_SMS_5100_DAY_KatokPL") );
	if( (pDM_1->GetRowCount() + pDM_2->GetRowCount()) == 0 )
	{
		AfxMessageBox( _T("상세보기할 이력이 존재하지 않습니다.\r\n(일별 이력에 대한 상세보기만이 존재합니다)") );
		return 1;
	}
	return 0;
}
 
int		CSMS_5120::GetSendPlatform_RadioBT()
{
	if(	((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck() )
	{
		return 1;
	}
	return 0;
}

void	CSMS_5120::ShowGrid_Toggle()
{
	CWnd* p1	= (CWnd*)GetDlgItem(IDC_DETAIL);
	CWnd* p2	= (CWnd*)GetDlgItem(IDC_DETAIL2);

	if(  GetSendPlatform_RadioBT() )
	{
		p1->ShowWindow( SW_HIDE );
		p2->ShowWindow( SW_SHOW );
	}else{
		p1->ShowWindow( SW_SHOW );
		p2->ShowWindow( SW_HIDE );
	}
}

INT CSMS_5120::InitScreen()
{
EFS_BEGIN

	INT ids;

	if( CheckExt_RowData() == 1 ) return 1;
	CESL_Grid*		pGrid = (CESL_Grid*)FindControl( _T("CM_SMS_5100") , _T("일별이력화면그리드") );
	CESL_DataMgr*	pDM = FindDM( _T("DM_SMS_5100_DAY") );
	if( GetSendPlatform_RadioBT() )
	{
		pGrid	= (CESL_Grid*)FindControl( _T("CM_SMS_5100") , _T("일별이력화면그리드_카톡") );
		pDM		= FindDM( _T("DM_SMS_5100_DAY_KatokPL") );
	}

	INT nIndex = pGrid->GetIdx();

	CString sDate;

	ids = pDM->GetCellData( _T("발송일자") , nIndex , sDate );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("InitScreen") );
	}

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSMSTYPE);
	pCombo->SetCurSel(0);

	if( TRUE == m_bUseILL )
	{
		pCombo->AddString(_T("책두레입수"));
		pCombo->AddString(_T("책두레요청거절"));
		pCombo->AddString(_T("책두레발송거절확인"));
	}
 
	if( sDate.IsEmpty() == FALSE  )
		m_tSendTime.SetDate( _ttoi(sDate.Mid(0,4)) , _ttoi(sDate.Mid(5,2)) , _ttoi(sDate.Mid(8,2)) );

	UpdateData(FALSE);
	
	return 0;

EFS_END
return -1;
}

INT CSMS_5120::GetWhere( CString &sWhere )
{
EFS_BEGIN

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSMSTYPE);

	INT nIndex = pCombo->GetCurSel();

	CString sSMSType;
	
	CString strText;
	pCombo->GetWindowText(strText);

	if(_T("전체")== strText ) sSMSType = _T("");
	else if(_T("반납안내")== strText ) sSMSType = _T("SMS01");
	else if(_T("연체안내")== strText ) sSMSType = _T("SMS02");
	else if(_T("예약자료대출안내")== strText ) sSMSType = _T("SMS03");
	else if(_T("비치희망도서안내")== strText ) sSMSType = _T("SMS04");
	else if(_T("공지사항안내")== strText ) sSMSType = _T("SMS05");
	else if(_T("행사안내")== strText ) sSMSType = _T("SMS06");
	else if(_T("신착자료안내")== strText ) sSMSType = _T("SMS07");
	else if(_T("추천도서안내")== strText ) sSMSType = _T("SMS08");
	else if(_T("일반문자")== strText ) sSMSType = _T("SMS09");
	else if(_T("지역상호대차 입수자료안내")== strText ) sSMSType = _T("SMS12");
	else if(_T("지역상호대차 발송거절안내")== strText ) sSMSType = _T("SMS13");
	else if(_T("지역상호대차 도서발송 안내")== strText ) sSMSType = _T("SMS14");
	else if(_T("지역상호대차 미발송복귀 안내")== strText ) sSMSType = _T("SMS15");
	else if(_T("개인정보 약관 만료안내")== strText ) sSMSType = _T("SMS16");
	else if(_T("개인정보 약관 만료예정안내")== strText ) sSMSType = _T("SMS17");


	if( 0 != nIndex )
	{
		if( 0 > strText.Find(_T("책두레")) )
		{

		}
		else
		{
			if(		 _T("책두레입수")		== strText )
			{
				sSMSType.Format(_T("TLL01"));
			}
			else if( _T("책두레요청거절")	== strText )
			{
				sSMSType.Format(_T("TLL02"));
			}
			else if( _T("책두레발송거절확인") == strText )
			{
				sSMSType.Format(_T("TLL03"));
			}			
		}
	}

	if( TRUE != sSMSType.IsEmpty() )
	{
		if( _T("SMS03") == sSMSType && _T("Y") == m_sNightReservationFlag )
		{
			sWhere.Format( _T(" MSLT.SMS_TYPE IN ('SMS03','NRL03') ") );
		}
		if( _T("SMS04") == sSMSType )
		{
			sWhere.Format( _T(" MSLT.SMS_TYPE IN ('SMS04','SMS11','SMS18','SMS19') ") );
		}
		else
		{
			sWhere.Format( _T(" MSLT.SMS_TYPE = '%s' ") , sSMSType );
		}
	}

	if( TRUE != m_strSearchWhere.IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += m_strSearchWhere;
	}

	if( TRUE != sWhere.IsEmpty() ) 
	{
		sWhere += _T(" AND ");
	}

	CString sDate;
	sDate = CLocCommonAPI::GetDate( this , IDC_datSEND_DATE1 );

	sWhere +=	_T(" MSLT.SEND_TIME BETWEEN to_date('") + sDate +
				_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('") + sDate +
				_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

	return 0;

EFS_END
return -1;
}

VOID CSMS_5120::OnSelchangecmbSMSTYPE() 
{
EFS_BEGIN

	SearchHistory();

EFS_END
}

VOID CSMS_5120::OnbtnPREV() 
{
	MoveGrid(0);	
}

VOID CSMS_5120::OnbtnNEXT() 
{
	MoveGrid(1);
}

INT CSMS_5120::MoveGrid( INT nMode )
{
EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SMS_5100") , _T("일별이력화면그리드") );

	if( GetSendPlatform_RadioBT() )
	{
		pGrid	= (CESL_Grid*)FindControl( _T("CM_SMS_5100") , _T("일별이력화면그리드_카톡") );
	}

	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("InitScreen") );
	}

	
	INT nCurrentPos = pGrid->GetIdx();
	
	if( 0 == nMode )
	{
		if( 0 == nCurrentPos )
		{
			AfxMessageBox( _T("처음 목록입니다.") );
			return 0;
		}
		nCurrentPos--;
	}
	else
	{
		if( pGrid->GetCount() - 1 == nCurrentPos )
		{
			AfxMessageBox( _T("마지막 목록입니다.") );
			return 0;
		}
		nCurrentPos++;
	}

	pGrid->SetReverse(nCurrentPos);

	InitScreen();

	SearchHistory();

	return 0;

EFS_END
return -1;
}





INT CSMS_5120::PrintList()
{
EFS_BEGIN
		
	INT ids;
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SMS_5120") );
	if( GetSendPlatform_RadioBT() )
	{
		pSrcDM = FindDM(_T("DM_SMS_5120_KatokPL"));
	}

	if( NULL == pSrcDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("PrintList") );
	}
	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;

	if( NULL == &pSIReportManager)
	{
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}

	pSIReportManager->SetCONNECTION_INFO(pSrcDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("일별문자발송이력_상세내역"));
	if( 0 > ids ) 
	{
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	if( PrivacyPrintLogRecord(pSrcDM, _T("SMS 이력조회"), _T("R") ,0 ) < 0 ) return 1;

	CString		strSMSType;
	CComboBox*	pcmbSMSType	=	( CComboBox* )GetDlgItem( IDC_cmbSMSTYPE );
	pcmbSMSType->GetWindowText( strSMSType );

	pSIReportManager->SetTempVariable( _T("종류") , strSMSType );

	pSIReportManager->SetDataMgr(0, pSrcDM , 0);
	
	ids = pSIReportManager->Print();

	if( NULL != &pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	
	return 0;
	
EFS_END
return -1;
}

INT CSMS_5120::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if( NULL == pSIReportManager) 
	{
		return -1;
	}

	return 0;
	
EFS_END
return -1;	
}


VOID CSMS_5120::OnbtnPRINT() 
{
	AfxGetApp()->DoWaitCursor(1);

	PrintList();

	AfxGetApp()->DoWaitCursor(-1);
	
}

INT CSMS_5120::SetSuccessInfo(CESL_DataMgr *pDM)
{
	if ( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2 ( -1010, 	_T("SetSuccessInfo") );
	}

	INT nRowCnt = pDM->GetRowCount();
	if ( !nRowCnt ) 
	{
		return 0;
	}
	
	if ( _T("3") == m_strSMSSystemInfo )
	{
		CString strTranDate		= _T("");
		CString strEmmaLogTable	= _T("");
		CString strEmmaKey		= _T("");
		CString strData			= _T("");
		CString strQuery		= _T("");

		INT		i;
		for( i=0 ; i<nRowCnt ; i++ )
		{
			pDM->GetCellData( _T("EMMA_KEY"), i, strEmmaKey );
			strEmmaKey.TrimLeft();strEmmaKey.TrimRight();

			if ( TRUE == strEmmaKey.IsEmpty() ) 
			{
				continue;
			}

			pDM->GetCellData( _T("전송시간") , i , strTranDate );
			strTranDate.TrimLeft();strTranDate.TrimRight();
			strTranDate = strTranDate.Mid( 0 , 7 );
			strTranDate.Replace( _T("/"), _T("") );
			
			strEmmaLogTable = _T("EM_LOG_") + strTranDate;			

			strQuery.Format( _T("SELECT DECODE(TRAN_RSLT, '0', 'Y', 'N') FROM %s WHERE TRAN_PR = %s"), strEmmaLogTable, strEmmaKey);
			pDM->GetOneValueQuery( strQuery, strData );
	
			pDM->SetCellData( _T("EMMA_KEY"), strData, i );
		}	
	}

	return 0;
}

INT CSMS_5120::SetSMSSystemInfo()
{	
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_5120") );

	if( GetSendPlatform_RadioBT() )
	{
		pDM = FindDM(_T("DM_SMS_5120_KatokPL"));
	}

	if ( NULL == pDM )	
	{
		ERROR_MSG_RETURN_ERROR_CODE2 ( -1010 , _T("SetSMSSystemInfo") );
	}

	CString strWhere;
	
	strWhere=_T("SELECT MIN(VALUE_NAME) FROM ESL_MANAGE_TBL ")
			 _T("WHERE CLASS_ALIAS='SMS시스템정보' AND MANAGE_CODE=UDF_MANAGE_CODE");

	ids = pDM->GetOneValueQuery(strWhere, m_strSMSSystemInfo);
	if ( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2 ( -1020 , _T("SetSMSSystemInfo") );
	}

	strWhere.Empty();
	strWhere.Format( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='대출자료관리' AND VALUE_NAME = '야간대출예약기능유무' AND MANAGE_CODE=UDF_MANAGE_CODE") );
	ids = pDM->GetOneValueQuery(strWhere, m_sNightReservationFlag);
	if ( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2 ( -1020 , _T("SetSMSSystemInfo") );
	}
	
	if( _T("Y") != m_sNightReservationFlag || TRUE == m_sNightReservationFlag.IsEmpty() )
	{
		m_sNightReservationFlag = _T("N");
	}

	
	return 0;
}

HBRUSH CSMS_5120::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

VOID CSMS_5120::OnbtnDELETE()
{
EFS_BEGIN

	CString strQuery;
	CString notice;
	INT rowCnt;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_5120"));
	CESL_ControlMgr *pCM = FindCM(_T("CM_SMS_5120"));
	if( GetSendPlatform_RadioBT() )
	{
		pDM = FindDM(_T("DM_SMS_5120_KatokPL"));
	}

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("상세이력그리드"));

	if( GetSendPlatform_RadioBT() )
	{
		pGrid = (CESL_Grid*)pCM->FindControl(_T("상세이력그리드_카톡"));
	}

	if(NULL == pGrid) ERROR_MSG_RETURN_VOID2(-1000, _T("IDC_DETAIL control null"));

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();


	rowCnt = pGrid->SelectGetCount();
	if(rowCnt==0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다."));
		return;
	}
	else 
	{
		notice.Format(_T("%d건의 자료를 삭제하시겠습니까?"), rowCnt);
		if(IDYES != AfxMessageBox(notice,MB_YESNO))
		{
			return;
		}
	}
	
		INT idx;
	idx = pGrid->SelectGetIdx();	
	for( int i=0; i < rowCnt; i++ )
	{
		CString id=_T("");
		id = pDM->GetCellData( _T("문자이력정보KEY"), idx);
		
		pDM->StartFrame();

		if( GetSendPlatform_RadioBT() )
		{
			strQuery.Format( _T("DELETE FROM MN_KAKAO_PLUS_LOG_TBL MSLT WHERE MSLT.REC_KEY = '%s';"), id );
			pDM->AddFrame(strQuery);
		}
		else
		{
			strQuery.Format( _T("DELETE FROM MN_SMS_LOG_TBL MSLT WHERE MSLT.REC_KEY = '%s';"), id );		
			pDM->AddFrame(strQuery);	
			strQuery.Format( _T("DELETE FROM MN_LMS_LOG_TBL MSLT WHERE MSLT.REC_KEY = '%s';"), id );
			pDM->AddFrame(strQuery);
		}
		pDM->SendFrame();
		pDM->EndFrame();
		pGrid->SelectGetNext();
		pDM->DeleteRow(idx);
	}

	pCM->ControlDisplay(_T("상세이력그리드"));

	pCM->ControlDisplay(_T("상세이력그리드_카톡"));

EFS_END
return;
}

void CSMS_5120::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	InitScreen();
	SearchHistory();
	ShowGrid_Toggle();
	
}

void CSMS_5120::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	InitScreen();
	SearchHistory();
	ShowGrid_Toggle();
}
