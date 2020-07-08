// SMS_5110.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_5110.h"
#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_5110 dialog


CSMS_5110::CSMS_5110(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_5110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bUseILL = FALSE;

	m_pParentMgr	=	(CSMS_5100*)pParent;
}

CSMS_5110::~CSMS_5110()
{

}

VOID CSMS_5110::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_5110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_5110, CDialog)
	//{{AFX_MSG_MAP(CSMS_5110)
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSEARCH_DETAIL, OnbtnSEARCHDETAIL)
	ON_EN_SETFOCUS(IDC_edtSMS05_USER_NAME1, OnSetfocusedtUSERNAME1)
	ON_EN_SETFOCUS(IDC_edtSMS05_USER_NAME2, OnSetfocusedtUSERNAME2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_5110 message handlers

BOOL CSMS_5110::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( 0 > InitESL_Mgr(_T("SMS_5110")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CButton* btnDiv = (CButton*)GetDlgItem(IDC_RADIO_DIVIDE);
	btnDiv->SetCheck(TRUE);

	InitScreen();
	
	SetWindowSize( CString(_T("간략검색")) );
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSMS_5110::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSMS_5110::OnbtnSEARCH() 
{
	AfxGetApp()->DoWaitCursor(1);

	INT ids = 0;
	m_TotalRow_SearchHistory = 0;
	SearchHistory(0);
	SearchHistory(1);

	if( m_TotalRow_SearchHistory == 0 )
	{
		AfxMessageBox(_T("검색된 이력이 존재하지 않습니다."));
	}
	AfxGetApp()->DoWaitCursor(0);
}

void	CSMS_5110::ChangeDMAlias( CString sDMAlias[3][2] )
{
	sDMAlias[0][0] = _T("DM_SMS_5100_DAY_FROM_DB_KatokPL");
	sDMAlias[0][1] = _T("DM_SMS_5100_DAY_KatokPL");
	sDMAlias[1][0] = _T("DM_SMS_5100_MONTH_FROM_DB_KatokPL");
	sDMAlias[1][1] = _T("DM_SMS_5100_MONTH_KatokPL");
	sDMAlias[2][0] = _T("DM_SMS_5100_YEAR_FROM_DB_KatokPL");
	sDMAlias[2][1] = _T("DM_SMS_5100_YEAR_KatokPL");
}

INT CSMS_5110::SearchHistory(int type)
{
EFS_BEGIN

	INT ids;

	const INT	nCnt = 3;
	CString		sDMAlias[nCnt][2] =
	{
		{ _T("DM_SMS_5100_DAY_FROM_DB"),	_T("DM_SMS_5100_DAY")	}, 
		{ _T("DM_SMS_5100_MONTH_FROM_DB"),	_T("DM_SMS_5100_MONTH")	}, 
		{ _T("DM_SMS_5100_YEAR_FROM_DB"),	_T("DM_SMS_5100_YEAR")	}
	};

	if( type == 1 )
		ChangeDMAlias( sDMAlias );

	BOOL	bIsSearch[nCnt] = { TRUE , TRUE , TRUE };

	INT		nCheckIDC[nCnt] = {	IDC_chkDAY , IDC_chkDAY2 , IDC_chkDAY3 };

	INT	i;
	for( i=0 ; i<nCnt ; i++ )
	{
		if( 1 == ((CButton*)GetDlgItem(nCheckIDC[i]))->GetCheck() )
		{
			bIsSearch[i] = TRUE;
		}
		else
		{
			bIsSearch[i] = FALSE;
		}
	}

	if( FALSE == bIsSearch[0] && FALSE == bIsSearch[1] && FALSE == bIsSearch[2] )
	{
		bIsSearch[0] = TRUE;
		bIsSearch[1] = TRUE;
		bIsSearch[2] = TRUE;
	}
		

	INT nTotalCnt = 0;
	for( i=0 ; i<nCnt ; i++ )
	{
		CESL_DataMgr* pDM = FindDM(sDMAlias[i][0]);
		if( NULL == pDM )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SearchHistory") );
		}

		pDM->FreeData();

		CESL_DataMgr* pDstDM = FindDM(sDMAlias[i][1]);
		if( NULL == pDM )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SearchHistory") );
		}

		pDstDM->FreeData();

		if( FALSE == bIsSearch[i] ) 
		{
			continue;
		}

		
		CString sWhere;
		ids = GetWhere(sWhere);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SearchHistory") );
		}
		else if( 0 < ids )
		{
			return 1;
		}
		
		ids = pDM->RefreshDataManager(sWhere);
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SearchHistory") );
		}

		ids = ConvertHistoryDM(i, type);	
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SearchHistory") ); 		
		}

		nTotalCnt += pDM->GetRowCount();
	}

	AllControlDisplay( _T("CM_SMS_5100") );

	m_TotalRow_SearchHistory += nTotalCnt;

	return 0;

EFS_END

	return -1;
}

INT CSMS_5110::InitScreen()
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_chkDAY))->SetCheck(TRUE);

	return 0;

	EFS_END

	return -1;

}

VOID CSMS_5110::OnbtnCLOSE() 
{
	OnCancel();	
}


INT CSMS_5110::GetWhere( CString &sWhere )
{
EFS_BEGIN

	INT			ids;
	CString		strTemp , strSearchWhere;

	BOOL		bIsSearchSimple;
	CString		strCaption;
	GetDlgItem( IDC_btnSEARCH_DETAIL )->GetWindowText( strCaption );
	if( _T("상세검색") == strCaption )
	{
		bIsSearchSimple	=	TRUE;
	}
	else
	{
		bIsSearchSimple =	FALSE;
	}
	
	CString		strSendDate1 , strSendDate2;
	strSendDate1	=	CLocCommonAPI::GetDate( this , IDC_datSEND_DATE1 );
	strSendDate2	=	CLocCommonAPI::GetDate( this , IDC_datSEND_DATE2 );

	if( TRUE == strSendDate1.IsEmpty() )
	{
		return 0;
	}

	if( TRUE == strSendDate2.IsEmpty() )
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

		strSendDate2.Format( _T("%04d/%02d/%02d") , time.wYear , time.wMonth , time.wDay );
	}

	if( FALSE == bIsSearchSimple )
	{
		CString		strLibCode;
		ids = GetControlData( _T("CM_SMS_5110") , _T("관리구분") , strLibCode , -1 , 0 );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetWhere") );
		}

		if(  _T("") != strLibCode )
		{
			sWhere.Format( _T("MSLT.MANAGE_CODE='%s' ") , strLibCode );
		}
		else
		{
			strTemp		=	( _T("MSLT.MANAGE_CODE=UDF_MANAGE_CODE ") );		
			sWhere		+=	strTemp;
		}

		const INT	nAliasCnt = 5;
		TCHAR		*szControlAlias[nAliasCnt] =
		{
			_T("대출자번호1"),	_T("대출자번호2"), 
			_T("대출자1"),		_T("대출자2"),		_T("핸드폰번호") 
		};

		CString		strGetData[nAliasCnt];
		INT			i;
		for( i=0 ; i<nAliasCnt ; i++ )
		{
			ids = GetControlData( _T("CM_SMS_5110") , szControlAlias[i] , strGetData[i] );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetWhere") );
			}
		}

		if( FALSE == strGetData[0].IsEmpty() )
		{
			if( FALSE == strGetData[1].IsEmpty() )
			{
				sWhere += _T("AND MSLT.USER_KEY IN (SELECT CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE CLU.USER_NO BETWEEN '") + strGetData[0] + _T("' AND '") + strGetData[1] + _T("') ");

				strSearchWhere += _T("MSLT.USER_KEY IN (SELECT CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE CLU.USER_NO BETWEEN '") + strGetData[0] + _T("' AND '") + strGetData[1] + _T("') ");
			}
			else
			{
				sWhere += _T("AND MSLT.USER_KEY = (SELECT CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE CLU.USER_NO = '") + strGetData[0] + _T("') ");

				strSearchWhere += _T("MSLT.USER_KEY = (SELECT CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE CLU.USER_NO = '") + strGetData[0] + _T("') ");
			}
		}
		
		if( FALSE == strGetData[2].IsEmpty() )
		{
			if( FALSE == strGetData[3].IsEmpty() )
			{
				sWhere += _T("AND MSLT.NAME BETWEEN '") + strGetData[2] + _T("' AND '") + strGetData[3] + _T("' ");

				if( TRUE != strSearchWhere.IsEmpty() )
				{
					strSearchWhere += _T(" AND ");
				}
				strSearchWhere += _T("MSLT.NAME BETWEEN '") + strGetData[2] + _T("' AND '") + strGetData[3] + _T("' ");
			}
			else
			{
				sWhere += _T("AND MSLT.NAME BETWEEN '") + strGetData[2] + _T("' AND '") + strGetData[2] + _T("힝' ");

				if( TRUE != strSearchWhere.IsEmpty() )
				{
					strSearchWhere += _T(" AND ");
				}
				strSearchWhere += _T("MSLT.NAME BETWEEN '") + strGetData[2] + _T("' AND '") + strGetData[2] + _T("힝' ");
			}
		}

		if( FALSE == strGetData[4].IsEmpty() )
		{
			strGetData[4].TrimLeft();
			strGetData[4].TrimRight();
			strGetData[4].Replace( _T(" "), _T("") );
			strGetData[4].Replace( _T("-"), _T("") );

			sWhere += _T("AND MSLT.SEND_PHONE_NUMBER LIKE '%") + strGetData[4] + _T("%' ");
			
			if( TRUE != strSearchWhere.IsEmpty() )
			{
				strSearchWhere += _T(" AND ");
			}
			strSearchWhere += _T("MSLT.SEND_PHONE_NUMBER LIKE '%") + strGetData[4] + _T("%' ");
		}

		m_pParentMgr->SetSearchWhere( strSearchWhere );
	}
	else
	{
		strTemp		=	( _T("MSLT.MANAGE_CODE=UDF_MANAGE_CODE ") );		
		sWhere		+=	strTemp;
	}

	strTemp.Format( _T("AND MSLT.SEND_TIME BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
						strSendDate1 , strSendDate2 );

	sWhere		+=	strTemp;

	return 0;

EFS_END
	return -1;
}

INT CSMS_5110::ConvertHistoryDM(INT nMode, int type)
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 3;

	CString strDMAlias[nCnt][2] =
	{
		{ _T("DM_SMS_5100_DAY_FROM_DB") , _T("DM_SMS_5100_DAY") } , 
		{ _T("DM_SMS_5100_MONTH_FROM_DB") , _T("DM_SMS_5100_MONTH")} , 
		{ _T("DM_SMS_5100_YEAR_FROM_DB") , _T("DM_SMS_5100_YEAR") }
	};
	
	if (type == 1)
		ChangeDMAlias( strDMAlias );

	CESL_DataMgr* pSrcDM = FindDM( strDMAlias[nMode][0] );
	if( NULL == pSrcDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ConvertHistoryDM") );
	}
	
	CESL_DataMgr* pDstDM = FindDM( strDMAlias[nMode][1] );
	if( NULL == pDstDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ConvertHistoryDM") );
	}

	pDstDM->FreeData();

	CButton* btnDiv = (CButton*)GetDlgItem(IDC_RADIO_DIVIDE);
	BOOL isDiv = btnDiv->GetCheck();

	const INT	nFieldCnt = 4;
	CString		strDMFIeldAlias[nFieldCnt]= 
	{
		_T("발송일자") , _T("타입") , _T("발송건수") , _T("나누어보낸건수")
	};

	CString		strGetData[nFieldCnt] , strSendDate , strCnt;
	INT		nUpdateIndex , i , j;
	for( i=0 ; i<pSrcDM->GetRowCount() ; i++ )
	{
		for(j = 0 ; j < nFieldCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( strDMFIeldAlias[j] , i , strGetData[j] );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ConvertHistoryDM") );
			}
		}

		nUpdateIndex = -1;
		for( j = 0 ; j<pDstDM->GetRowCount(); j++ )
		{
			ids = pDstDM->GetCellData( _T("발송일자") , j , strSendDate );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ConvertHistoryDM") );
			}

			if( 0 == strSendDate.Compare( strGetData[0]) )
			{
				nUpdateIndex = j;
			}
		}

		if( -1 == nUpdateIndex )
		{
			pDstDM->InsertRow(-1);
			nUpdateIndex = pDstDM->GetRowCount() - 1;
			pDstDM->SetCellData( _T("발송일자") , strGetData[0] , nUpdateIndex );
		}

		if(isDiv){			
			strCnt = strGetData[3];
		}
		else {
			strCnt = strGetData[2];	
		}

		if( strGetData[1] == _T("NRL03") )
		{
			strGetData[1] = _T("SMS03");
		}
		else if( _T("SMS11") == strGetData[1] )
		{
			strGetData[1] = _T("SMS04");
		}
		else if( _T("SMS18") == strGetData[1] || _T("SMS19") == strGetData[1] )
		{
			strGetData[1] = _T("SMS04");
		}

		if( _T("SMS01") == strGetData[1] || _T("SMS02") == strGetData[1] ||
			_T("SMS03") == strGetData[1] || _T("SMS04") == strGetData[1] ||
			_T("SMS05") == strGetData[1] || _T("SMS06") == strGetData[1] ||
			_T("SMS07") == strGetData[1] || _T("SMS08") == strGetData[1] ||
			_T("SMS09") == strGetData[1] || _T("LFE01") == strGetData[1] ||
			_T("LFE02") == strGetData[1] || _T("LFE03") == strGetData[1] ||
			_T("LFE04") == strGetData[1] ||
			_T("TLL01") == strGetData[1] || _T("TLL02") == strGetData[1] ||
			_T("TLL03") == strGetData[1] ||				
			_T("SMS12") == strGetData[1] || _T("SMS13") == strGetData[1] || 
			_T("SMS14") == strGetData[1] || _T("SMS15") == strGetData[1] ||
			_T("SMS16") == strGetData[1] || _T("SMS17") == strGetData[1] 	
			) 
		{
			if(_T("SMS04") == strGetData[1])
			{
				CString strSMS04 = _T("");
				ids = pDstDM->GetCellData(_T("SMS04"), nUpdateIndex, strSMS04);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ConvertHistoryDM") );

				INT	nTotal	=	_ttoi( strSMS04 );
				nTotal		=	nTotal + _ttoi( strCnt );
				strCnt.Format( _T("%d") , nTotal);
			}
			else if(_T("SMS03") == strGetData[1])
			{
				CString strSMS03 = _T("");
				ids = pDstDM->GetCellData(_T("SMS03"), nUpdateIndex, strSMS03);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ConvertHistoryDM") );

				INT	nTotal	=	_ttoi( strSMS03 );
				nTotal		=	nTotal + _ttoi( strCnt );
				strCnt.Format( _T("%d") , nTotal);
			}
			ids = pDstDM->SetCellData( strGetData[1] , strCnt , nUpdateIndex );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ConvertHistoryDM") );	
			}
		}
	}

	const INT SMS_TYPE_COUNT = 22;
	CString sSMSType[SMS_TYPE_COUNT] =
	{
		_T("SMS01") , _T("SMS02") , _T("SMS03") , _T("SMS04") , _T("SMS05") , 
		_T("SMS06") , _T("SMS07") , _T("SMS08") , _T("SMS09") , _T("LFE01") ,
		_T("LFE02") , _T("LFE03") , _T("LFE04") , _T("SMS12") , _T("SMS13") ,
		_T("TLL01") , _T("TLL02") , _T("TLL03") , _T("SMS14") , _T("SMS15") ,
		_T("SMS16") , _T("SMS17")
	};

	int nSMSTypeCnt = SMS_TYPE_COUNT;

	CString sCntData;
	CString sDivSum,sOneSum,sSum;
	for( i = 0 ; i < pDstDM->GetRowCount() ; i++ )
	{
		sDivSum = _T("0");
		sOneSum = _T("0");

		for( INT j = 0 ; j < SMS_TYPE_COUNT ; j++ )
		{
			if(m_bUseILL == FALSE){
				if(sSMSType[j] == _T("TLL01") || sSMSType[j] == _T("TLL02") || sSMSType[j] == _T("TLL03"))
					continue;
			}
			ids = pDstDM->GetCellData( sSMSType[j] , i , sCntData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertHistoryDM") );

			if( sCntData.IsEmpty() )
			{
				sCntData = _T("0");
				ids = pDstDM->SetCellData( sSMSType[j] , sCntData , i );
			}

			if( sCntData.Compare(_T("0")) == 0 ) continue;
			
			sDivSum.Format( _T("%d") , _ttoi(sDivSum) + _ttoi(sCntData ) );
		}
		sSum.Format( _T("%s") , sDivSum );
		pDstDM->SetCellData( _T("계") , sSum , i );
	}

	return 0;

	EFS_END

	return -1;

}

HBRUSH CSMS_5110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CSMS_5110::OnSetfocusedtUSERNAME1() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext( GetDlgItem(IDC_edtSMS05_USER_NAME1)->m_hWnd );
    if(hIME) 
    {
        ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus( hIME , dwConversion , dwSentence );

        ImmReleaseContext( GetDlgItem(IDC_edtSMS05_USER_NAME1)->m_hWnd , hIME ); 
    }	
}

void CSMS_5110::OnSetfocusedtUSERNAME2() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext( GetDlgItem(IDC_edtSMS05_USER_NAME2)->m_hWnd );
    if(hIME) 
    {
        ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus( hIME , dwConversion , dwSentence );

        ImmReleaseContext( GetDlgItem(IDC_edtSMS05_USER_NAME2)->m_hWnd , hIME ); 
    }	
}

void CSMS_5110::OnbtnSEARCHDETAIL() 
{
EFS_BEGIN

	CString strCaption;
	GetDlgItem( IDC_btnSEARCH_DETAIL )->GetWindowText( strCaption );
	SetWindowSize( strCaption );	

EFS_END	
}

VOID CSMS_5110::SetWindowSize(CString strCaption)
{
EFS_BEGIN

	CRect	rNowWndPos , rChangeWndPos;
	
	INT nIDC;
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	if( 0 == strCaption.Compare( szCaption[0] ) )
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staVIEW_DETAIL;
	}
	else
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staVIEW_SIMPLE;
	}

	GetWindowRect( &rNowWndPos );
	GetDlgItem( nIDC )->GetWindowRect( &rChangeWndPos );
	
	ClientToScreen( &rNowWndPos );
	ClientToScreen( &rChangeWndPos );

	rNowWndPos.SetRect( rNowWndPos.left, rNowWndPos.top, 
						rNowWndPos.right, rChangeWndPos.bottom );
	ScreenToClient( &rNowWndPos );
	MoveWindow( rNowWndPos );	
	
EFS_END
}

BOOL CSMS_5110::PreTranslateMessage(MSG* pMsg) 
{
	if ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnCLOSE )->GetSafeHwnd() )
		{
			OnbtnCLOSE();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSEARCH )->GetSafeHwnd() )
		{
			OnbtnSEARCH();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSEARCH_DETAIL )->GetSafeHwnd() )
		{
			OnbtnSEARCHDETAIL();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_edtSMS05_USER_NO1 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS05_USER_NO2 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS05_USER_NAME1 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS05_USER_NAME2 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS05_USER_HANDPHON )->GetSafeHwnd()						
			)
		{
			OnbtnSEARCH();
			::SetFocus( pMsg->hwnd );
			return TRUE;
		}
		return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
