#include "stdafx.h"
#include "BO_LOC_3170_ReserveInfo.h"

#include "ReserveLinkBookInfoDlg.h"
#include "ReserveComplete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CBO_LOC_3170_ReserveInfo::CBO_LOC_3170_ReserveInfo( CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager, CLonaManageValue *pLona, INT nMailClass )
	: CBO_LOC_3170(pParent, pLoanShareManager, pLona, nMailClass)
{
	
	m_lpszTemplateName = MAKEINTRESOURCE( IDD );
	m_nIDHelp = IDD;

	m_pParent = pParent;

	m_bFirstJob = FALSE;
}

CBO_LOC_3170_ReserveInfo::~CBO_LOC_3170_ReserveInfo()
{
}

VOID CBO_LOC_3170_ReserveInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3170_ReserveInfo)	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3170_ReserveInfo, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3170_ReserveInfo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnRESERVE_LINK, OnbtnRESERVELINK)
	ON_BN_CLICKED(IDC_btnLINK_CANCEL, OnbtnLINKCANCEL)
	ON_BN_CLICKED(IDC_btnMAKE_REXPIRE, OnbtnMAKEREXPIRE)
	ON_BN_CLICKED(IDC_btnS_EMAIL, OnbtnSEMAIL)
	ON_BN_CLICKED(IDC_btnR_CANCEL, OnbtnRCANCEL)
	ON_BN_CLICKED(IDC_btnR_CLOSE, OnbtnRCLOSE)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SEND_SMS_RESERVE_VOL, OnbtnSENDSMSReserveVol)
	ON_BN_CLICKED(IDC_BTN_PRINT_RESERVE_VOL, OnbtnPrintReserveVol)
	ON_WM_ACTIVATE(IDD_BO_LOC_3170_RESERVEINFO_DLG, OnActivate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBO_LOC_3170_ReserveInfo::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}

BOOL CBO_LOC_3170_ReserveInfo::OnInitDialog() 
{
EFS_BEGIN
	INT ids = 0;

 	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	ids = CBO_LOC_3170::OnInitDialog( );
	if ( FALSE==ids )	return ids;
	
	if( m_pLona->m_sUseReceipt == _T("Y") )
	{				
		CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_PRINT_RESERVE_VOL);
		if(NULL != pBtn && NULL != pBtn->GetSafeHwnd())
		{
			pBtn->ShowWindow(SW_SHOW);
		}		
	}

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") );
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( _T("VRBookInfoGrid") );
	
	
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( FALSE , _T("OnInitDialog") );
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	
	pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3170_VR_RESERVE_INFO"),_T("VRReserveGrid"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( FALSE , _T("OnInitDialog") );
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	ids = this->ViewScreen( );
	if ( ids < 0 ) return FALSE;	
	
	if(m_pInfo->MODE==10000)
	{
		(CButton*)GetDlgItem(IDC_btnS_EMAIL)->EnableWindow(FALSE);
 		(CButton*)GetDlgItem(IDC_BTN_SEND_SMS_RESERVE_VOL)->EnableWindow(FALSE);
	}
	return TRUE;  
	              
EFS_END
return FALSE;
}

VOID CBO_LOC_3170_ReserveInfo::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	

	if( bShow )
	{
		if( 1 == m_nReserveInfoMode )
		{
			SetTimer(10,100,NULL);
		}
			
		if( 0 == m_nMenuFlag )
		{
			CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
			CString sGridAlias = _T("MainGrid");

			CESL_ControlMgr *pCM = pParentMgr->FindCM( sCMAlias );
			if( pCM == NULL ) return;
			CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
			if( pGrid == NULL ) return;

			INT nRowIndex = pGrid->GetIdx();
			if( nRowIndex < 0 ) return;

			CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
			TCHAR *sFieldAlias[5] = 
			{
				_T("단행/연속구분"), _T("책KEY"), _T("예약이용자수"), _T("종KEY"), _T("권호정보KEY")
			};
			CString sGetData[5];

			for( INT i = 0 ; i < 5 ; i++ )
			{
				INT ids = pParentMgr->GetDataMgrData( sDMAlias , sFieldAlias[i] , sGetData[i] , nRowIndex );
				if( ids < 0 ) return;
			}						

			if( 1 == m_nReserveInfoMode )
			{ 
				SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1] );

			}
			else
			{ 
				SetInfo(sGetData[3], sGetData[4], sGetData[0], _T("") );
			}
		}
		
	}
}

INT CBO_LOC_3170_ReserveInfo::ViewScreen()
{
EFS_BEGIN

	INT ids = 0;

	
	CESL_DataMgr *pLoanInfoDM = FindDM(_T("DM_BO_LOC_3170_VR_LOAN_USER_INFO"));
	CESL_DataMgr *pReserveInfoDM = FindDM(_T("DM_BO_LOC_3170_VR_RESERVE_INFO"));
	
	INT		i , nLoanInfoRowCnt;
	nLoanInfoRowCnt	=	pLoanInfoDM->GetRowCount();
	for( i=0 ; i<nLoanInfoRowCnt ; i++ )
	{
		CString sValue;
		ids = pLoanInfoDM->GetCellData(_T("대출자번호"), i, sValue);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewScreen") );
		
		CLocCommonAPI::ChangeSecuriyUserNo( this , sValue , m_pLona->m_sIsUnityLoanService );

		ids = pLoanInfoDM->SetCellData(_T("대출자번호표시"), sValue, i);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ViewScreen") );
		}
	}

	INT		j , nReserveInfoRowCnt;
	nReserveInfoRowCnt	=	pReserveInfoDM->GetRowCount();
	for( j=0 ; j<nReserveInfoRowCnt ; j++ )
	{
		CString sValue;
		ids = pReserveInfoDM->GetCellData(_T("대출자번호"), j, sValue);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ViewScreen") );
		}

		CLocCommonAPI::ChangeSecuriyUserNo( this , sValue , m_pLona->m_sIsUnityLoanService );

		ids = pReserveInfoDM->SetCellData(_T("대출자번호표시"), sValue, j);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("ViewScreen") );
		}
	}

	ids = AllControlDisplay( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ViewScreen") );
	}

	ids = AllControlDisplay( _T("CM_BO_LOC_3170_VR_BOOK_INFO"), 0 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(  -1100 , _T("ViewScreen") );
	}

	ids = this->InitControl( );
	if( 0 > ids )	
	{
		return ids;
	}

	ids = this->SetSelect( 0 );
	if( 0 > ids )	
	{
		return ids;
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3170_ReserveInfo::InitControl()
{
EFS_BEGIN

	CString sReserveDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");

	CESL_DataMgr *pDM = FindDM( sReserveDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(  -1010 , _T("InitControl") );
	}

	INT		nRowCnt	=	pDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		EnableDisableButton( FALSE );
	}
	else
	{
		EnableDisableButton( TRUE );
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3170_ReserveInfo::SetSelect(INT nIndex)
{
EFS_BEGIN

	INT ids = 0;

	CString sCMAlias = _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
	CString sGridAlias = _T("VRReserveGrid");

	CESL_Grid *pGrid = (CESL_Grid*) FindControl( sCMAlias, sGridAlias );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(  -1010 , _T("SetSelect") );
	}
	
	if( nIndex == 0 ) 	
	{
		ids = pGrid->SetReverse(0);
	}
	else 
	{
		ids = pGrid->SetReverse( pGrid->GetIdx() );
	}

	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(  -1020 , _T("SetSelect") );
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3170_ReserveInfo::EnableDisableButton(BOOL bEnable)
{
EFS_BEGIN

	GetDlgItem( IDC_btnRESERVE_LINK )->EnableWindow( bEnable );	
	GetDlgItem( IDC_btnMAKE_REXPIRE )->EnableWindow( bEnable );	
	GetDlgItem( IDC_btnR_CANCEL )->EnableWindow( bEnable );		

	
	GetDlgItem( IDC_BTN_PRINT_RESERVE_VOL )->EnableWindow( bEnable );		
	GetDlgItem( IDC_btnLINK_CANCEL )->EnableWindow( bEnable );				
		
	
	
	if( 10000 == m_pInfo->MODE )
	{
		GetDlgItem( IDC_btnS_EMAIL )->EnableWindow( FALSE );	
		GetDlgItem( IDC_BTN_SEND_SMS_RESERVE_VOL )->EnableWindow( FALSE );	
	}
	else
	{
		GetDlgItem( IDC_btnS_EMAIL )->EnableWindow( bEnable );		
		GetDlgItem( IDC_BTN_SEND_SMS_RESERVE_VOL )->EnableWindow( bEnable );	
	}

EFS_END
}

INT CBO_LOC_3170_ReserveInfo::SetInfo(CString sSpeciesKey, CString sVolInfoKey, CString sPublishFormCode, CString sBookKey, INT nMenuFlag)
{
EFS_BEGIN

	CBO_LOC_3170::SetInfo( sSpeciesKey, sVolInfoKey, sPublishFormCode, sBookKey, nMenuFlag );

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3170_ReserveInfo::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN

	INT ids = 0;

	
	if( strDMFieldAlias.Compare( _T("배가상태") ) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( str , 1 );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 ,  _T("GridDisplayEventProc")  );
		}
	}
	else if( strDMFieldAlias.Compare( _T("자료실") ) == 0 )
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , str , str );
	}
	
	else if( strDMFieldAlias.Compare( _T("부서명") ) == 0 )
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , str , str );
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3170_ReserveInfo::OnbtnMAKEREXPIRE() 
{
EFS_BEGIN

	OnbtnDUEDATE();

	this->ViewScreen();


EFS_END
}

VOID CBO_LOC_3170_ReserveInfo::OnbtnRESERVELINK() 
{
EFS_BEGIN

	
	GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	

	INT ids = 0;
	CString sEMsg = _T("");

	ids =	ReserveComplete();

	if( 0 > ids ) 
	{
		sEMsg.Format( _T("기능을 정상적으로 수행하지 못했습니다.[%d]") , ids );
		AfxMessageBox( sEMsg );
	
		return;
	}

	m_bEdit = TRUE;
	


EFS_END
}

VOID CBO_LOC_3170_ReserveInfo::OnbtnSEMAIL() 
{
EFS_BEGIN

	OnbtnSENDMAIL( );
	

EFS_END
}

VOID CBO_LOC_3170_ReserveInfo::OnbtnRCANCEL() 
{
EFS_BEGIN

	OnbtnCANCEL( );

	this->ViewScreen( );


EFS_END
}

VOID CBO_LOC_3170_ReserveInfo::OnbtnRCLOSE() 
{
EFS_BEGIN

	OnCancel();

EFS_END
}

INT CBO_LOC_3170_ReserveInfo::AutoJob()
{
EFS_BEGIN

	INT ids = 0;
	INT nSIndex = -1;
	CString sValue = _T("");
	m_pLona->GetManageValue( _T("열람"),_T("공통"),_T("예약관리"),_T("자동예약자료지정여부"),sValue); 

	if( 0 != sValue.Compare(_T("Y")) )
	{
		return 1;
	}

	if( TRUE == m_sBookKey.IsEmpty( ) )		
	{
		return 2;
	}

	
	ids = AutoReserveComplete( nSIndex );
	if( 0 > ids ) 
	{
		CString sEMsg;
		sEMsg.Format( _T("기능을 정상적으로 수행하지 못했습니다.[%d]") , ids );
		AfxMessageBox( sEMsg );
		return -1;
	}

	if( 0 < ids ) 
	{
		return 1;
	}

	if( -1 == nSIndex ) 
	{
		return 2;
	}

	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("AutoJob") );
	}

	pGrid->SetRowSel(nSIndex+1);  
	pGrid->SetReverse( nSIndex );

	this->ViewScreen( );

	AfxMessageBox( _T("예약 지정이 완료되었습니다.") , MB_ICONINFORMATION );

	m_nReserveInfoMode = 0;
	
	pGrid->SetRowSel(nSIndex+1); 
	pGrid->SetReverse( nSIndex );

	CString strTempMsg , strFaileMsg;
	GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->GetWindowText( strFaileMsg );

	ids = SendEmail();
	if( 0 == ids )
	{
		strTempMsg = _T("E-Mail을 발송하였습니다.");
	}
	else if( 1050 == ids )
	{
		strTempMsg = _T("이용자가 E-Mail 수신을 신청하지 않았습니다.");
	}
	else if( 1060 == ids )
	{
		strTempMsg = _T("이용자의 E-Mail이 입력되지 않아 E-Mail 발송이 되지 않았습니다.");			
	}

	if( TRUE != strFaileMsg.IsEmpty() )
	{
		strFaileMsg += _T("     ");
	}

	strFaileMsg += strTempMsg;
	GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( strFaileMsg );

	m_bEdit = TRUE;

	return 0;
EFS_END
return -1;
}


INT CBO_LOC_3170_ReserveInfo::ReserveComplete()
{
EFS_BEGIN

	INT ids = 0;

	INT nSIndex = 0;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ReserveComplete") );
	}

	nSIndex = pGrid->GetIdx();

	CString sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	CString sReserveMark   = _T("");
	
	CESL_DataMgr*	pDM		=	FindDM(_T("DM_BO_LOC_3170_VR_RESERVE_INFO"));
	
	
	ids = GetDataMgrData( sDMAlias , _T("지정여부") , sReserveMark , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ReserveComplete") );
	}

	if ( _T("O") == sReserveMark )
	{
		AfxMessageBox( _T("선택한 예약자는 이미 지정된 예약자이기 때문에 지정할 수 없습니다") );
		return 0;
	}

	CReserveInfo RI;

	ids = GetDataMgrData( sDMAlias , _T("단행/연속구분") , RI.sPublishFormCode , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ReserveComplete") );
	}

	ids = GetDataMgrData( sDMAlias , _T("종KEY")         , RI.sSpeciesKey      , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ReserveComplete") );
	}

	ids = GetDataMgrData( sDMAlias , _T("권호정보KEY")   , RI.sVolInfo         , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ReserveComplete") );
	}
	
	CString strUnmanned;
	ids = pDM->GetCellData(_T("무인예약상태"), nSIndex, strUnmanned);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-15, _T("ReserveComplete"));
	if(_T("O") == strUnmanned)
	{
 		AfxMessageBox(_T("무인예약대기 자료입니다."), MB_ICONINFORMATION);
 		return 0;
	}

	CString		strReserveMark;
	pDM->GetCellData( _T("지정여부") , nSIndex , strReserveMark );
	if( _T("O") == strReserveMark )
	{
		AfxMessageBox( _T("이미 지정된 자료이기 때문에 지정할 수 없습니다.") , MB_ICONINFORMATION );
		return 0;
	}

	CReserveLinkBookInfoDlg BookDlg( this );
	BookDlg.SetIsAutoSetReservationDay( m_pLona->m_sIsAutoSetReservationDay );
	BookDlg.SetInfo( RI );

	if( 1 == m_nReserveInfoMode )
	{		
		
		BookDlg.LoadBookInfoFromDB();
		
		CESL_DataMgr*	pBookDM	=	FindDM( _T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG") );
		INT		nBookDMRowCnt	=	pBookDM->GetRowCount();
		if(1 > nBookDMRowCnt)
		{
			AfxMessageBox( _T("예약지정 할 수 있는 자료가 없습니다.") , MB_ICONINFORMATION );
			return 0;
		}

		CString		strTemp;
		INT		i;
		for( i=0; i<nBookDMRowCnt ; i++ )
		{
			ids		=	pBookDM->GetCellData( _T("책KEY") , i , strTemp );
			if( 0 > ids )  
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("ReserveComplete") );
			}

			if( m_strReturnBookKey == strTemp)
			{
				BookDlg.m_nSelectIdx		=	i;
				break;
			}
		}
		BookDlg.MakeRI();
		BookDlg.GetInfo( RI );

		m_nReserveInfoMode = 0;
	}
	else
	{
		
		ids = BookDlg.DoModal( );		
		
		if( 5 == ids )
		{
			AfxMessageBox( _T("예약지정 할 수 있는 자료가 없습니다.") , MB_ICONINFORMATION );
			return 0;
		}
		

		ids = BookDlg.GetInfo( RI );
		if( 0 < ids ) 
		{
			return 0;			
		}
	}
	
	CString sLoanKey = _T("");

	ids = GetDataMgrData( sDMAlias , _T("대출/반납정보KEY") , sLoanKey, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("ReserveComplete") );
	}

	CESL_DataMgr* pUpdateDM = FindDM( sDMAlias );
	if( NULL == pUpdateDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("ReserveComplete") );
	}

	CReserveComplete RC;

	ids = RC.DBReserveComplete( RI , this , sLoanKey , pUpdateDM );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ReserveComplete") );
	}

	
	CString strLibCode;
	
	strLibCode = m_pInfo->LIB_CODE;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
 	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("BOOK_KEY,SPECIES_KEY,SHELF_LOC_CODE,CLASS_NO,CLASS_NO_TYPE,REG_CODE,SEPARATE_SHELF_CODE,CALL_NO,REG_NO");

	UploadShareInfo.UploadShareLoanInfo.sLibCode = strLibCode;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );	
	
	ids = pUpdateDM->SetCellData( _T("지정여부"), _T("O"), nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("ReserveComplete") );
	}

	ids = pUpdateDM->SetCellData( _T("등록번호"), RI.sRegNo, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("ReserveComplete") );
	}
	
	CString		sData;
	ids = GetDataMgrData( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") , _T("대출자직급") , sData, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("ReserveComplete") );
	}
	
	m_pReservationInfo->SetPositionCode( sData );
	
	ids = m_pReservationInfo->SPFExcutePath( _T("예약만기일부여") );
	if( 0 > ids ) 
	{
		return -1;
	}

	BOOL bUnmanned = FALSE;
	CString strFaileMsg, strTmp;
	GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->GetWindowText( strFaileMsg );	
	if( 0 == ids )
	{
		strTmp = _T("");
	}
	else if( 1010 == ids )
	{
		strTmp = _T("SMS 메시지를 전송하였습니다.");
	}
	else if( 1030 == ids )
	{
		strTmp = _T("이용자의 핸드폰 번호가 입력되지 않아 SMS 전송이 되지 않았습니다.");
	}
	
	else if( 1040 == ids )
	{
		strTmp = _T("Procedure에서 SMS 전송이 중단되었습니다.");
	}
	
	else if( 1050 == ids )
	{
		bUnmanned = TRUE;
		strTmp = _T("무인예약 신청 자료입니다.");
	}
	else if( 0 < ids ) 
	{
		return -1;
	}
	else
	{
	}

	strFaileMsg += strTmp;
	GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( strFaileMsg );
		
	ids = pUpdateDM->SetCellData( _T("예약만기일") , m_pReservationInfo->m_sExpireDate , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("ReserveComplete") );
	}
	
	pGrid->DisplayLine( nSIndex );
	
	CESL_Mgr*	pMgr	=	pParentMgr->FindSM( _T("BO_LOC_3100") );
	if( NULL != pMgr )
	{
		CString		strReserveMarkUserKey , strSearchUserKey;

		
		CESL_DataMgr*	pDMMemberInfo		= FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
		if( NULL == pDMMemberInfo ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("ReserveComplete") );
		}
			
		ids	=	pDMMemberInfo->GetCellData( _T("대출자KEY") , 0 , strSearchUserKey );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("ReserveComplete") );
		}
		
		ids	=	pUpdateDM->GetCellData( _T("대출자KEY") , nSIndex , strReserveMarkUserKey );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("ReserveComplete") );
		}

		if( 0 == strSearchUserKey.Compare(strReserveMarkUserKey) )
		{
			CString		strLoanInfoLoanKey , strReservemarkLoanInfo;
			ids = pUpdateDM->GetCellData( _T("대출/반납정보KEY") , nSIndex , strReservemarkLoanInfo );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("ReserveComplete") );
			}
		
			CESL_DataMgr*	pDMLoanInfo		= FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
			if( NULL == pDMLoanInfo ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("ReserveComplete") );
			}

			INT		i , nRowCnt;
			nRowCnt	=	pDMLoanInfo->GetRowCount();
			for( i=0 ; i<nRowCnt ; i++ )
			{
				ids = pDMLoanInfo->GetCellData( _T("대출/반납정보KEY") , i , strLoanInfoLoanKey );
				if( 0 > ids )
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1200 , _T("ReserveComplete") );
				}

				if( 0 == strReservemarkLoanInfo.Compare( strLoanInfoLoanKey ) )
				{
					ids = pDMLoanInfo->SetCellData( _T("등록번호") , RI.sRegNo , i );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1210 , _T("ReserveComplete") );
					}

					ids = pDMLoanInfo->SetCellData( _T("예약만기일") , m_pReservationInfo->m_sExpireDate , i );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1220 , _T("ReserveComplete") );
					}
					
					ids = pDMLoanInfo->SetCellData( _T("상태"), RI.sWorkingStatus, i );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1230 , _T("ReserveComplete") );
					}
									
					ids = pDMLoanInfo->SetCellData( _T("청구기호"), RI.sCallNo, i );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1240 , _T("ReserveComplete") );
					}
										
					CESL_Grid*		pGridLoanList	= (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO") , _T("MainGrid") );
					if( NULL == pGridLoanList ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1270 , _T("ReserveComplete") );
					}
					
					pGridLoanList->DisplayLine(i);
				}
			}
		}
	}
	
	CESL_Mgr* pEslMgr = pParentMgr->FindSM(_T("SM_BO_LOC_RESERVE"));
	if(NULL != pEslMgr)
	{
		CString strLoanKey;
		ids = pUpdateDM->GetCellData(_T("대출/반납정보KEY"), nSIndex, strLoanKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
		CESL_DataMgr* pReserveMainDM = FindDM(_T("DM_BO_LOC_RESERVE"));
		if(NULL == pReserveMainDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
		for(INT i = 0; i < pReserveMainDM->GetRowCount(); i++)
		{
			CString strValue;
			pReserveMainDM->GetCellData(_T("LOAN_KEY"), i, strValue);
			if(strValue == strLoanKey)
			{
				pReserveMainDM->SetCellData(_T("예약만기일"), m_pReservationInfo->m_sExpireDate, i);
				pReserveMainDM->SetCellData(_T("등록번호"), RI.sRegNo, i);
				pReserveMainDM->SetCellData(_T("청구기호"), RI.sCallNo, i);
				CESL_Grid* pReserveGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_RESERVE"), _T("MainGrid"));
				if(NULL == pReserveGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
				pReserveGrid->DisplayLine(i);
				break;
			}
		}
	}

	if(TRUE == bUnmanned)
	{
		AfxMessageBox( _T("무인예약신청 자료에 대하여 예약지정이 완료되었습니다.") );
	}
	else
	{
		AfxMessageBox( _T("예약 지정이 완료되었습니다.") );
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_3170_ReserveInfo::AutoReserveComplete(INT &nSIndex)
{
EFS_BEGIN

	INT ids = 0;
	INT nRCnt = 0;
	CString sIsComplete = _T("");
	CString sLoanKey = _T("");
	CString sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	CReserveInfo RI;
	CReserveComplete RC;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("AutoReserveComplete") );
	}

	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( NULL == pDM )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("AutoReserveComplete") );
	}

	nRCnt = pDM->GetRowCount();
	INT		i;
	for( i=0 ; i<nRCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("지정여부") , i , sIsComplete );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("AutoReserveComplete") );
		}

		if( sIsComplete.Compare(_T("O")) != 0 )
		{
			nSIndex = i;
			pGrid->SetRowSel(i+1); 
			pGrid->SetReverse(i); 
			break;
		}
	}	
	
	if( 0 > nSIndex && 0 <  nRCnt ) 
	{
		nSIndex = 0;
	}

	if( nSIndex == -1 ) 
	{
		return 1;
	}	
	
	ids = GetDataMgrData( sDMAlias , _T("단행/연속구분") , RI.sPublishFormCode , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("AutoReserveComplete") );
	}

	ids = GetDataMgrData( sDMAlias , _T("종KEY") , RI.sSpeciesKey , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("AutoReserveComplete") );
	}

	ids = GetDataMgrData( sDMAlias , _T("권호정보KEY") , RI.sVolInfo, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("AutoReserveComplete") );
	}

	RI.sBookKey = m_sBookKey;	
	
	CReserveLinkBookInfoDlg BookDlg(this);
	BookDlg.SetIsAutoSetReservationDay( m_pLona->m_sIsAutoSetReservationDay );
	BookDlg.SetInfo(RI);
	ids = BookDlg.DoModal();
		
	if( 5 == ids )
	{
		AfxMessageBox( _T("예약지정 할 수 있는 자료가 없습니다.") , MB_ICONINFORMATION );
		return 0;
	}	

	ids = BookDlg.GetInfo(RI);
	
	if( 0 < ids ) 
	{
		return 2;
	}
	
	ids = GetDataMgrData( sDMAlias , _T("대출/반납정보KEY") , sLoanKey, nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("AutoReserveComplete") );
	}
	
	CESL_DataMgr* pUpdateDM = FindDM( sDMAlias );
	if( NULL == pUpdateDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("AutoReserveComplete") );
	}

	ids = RC.DBReserveComplete( RI , this , sLoanKey , pUpdateDM );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("AutoReserveComplete") );
	}
	
	ids = pUpdateDM->SetCellData( _T("지정여부") , _T("O") , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("AutoReserveComplete") );
	}

	ids = pUpdateDM->SetCellData( _T("등록번호") , RI.sRegNo , nSIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("AutoReserveComplete") );
	}
	
	OnbtnDUEDATE();
	
	pGrid->DisplayLine(nSIndex);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");	
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
 	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("BOOK_KEY,SPECIES_KEY,SHELF_LOC_CODE,CLASS_NO_TYPE,REG_CODE,SEPARATE_SHELF_CODE,CALL_NO,REG_NO");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_3170_ReserveInfo::ReservLinkCancel() 
{
	INT ids = 0;
	INT nSIndex = 0;
	CString sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	CString sLoanKey = _T("");
	CString	sIsComplete;
	

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ReservLinkCancel") );
	}

	CESL_DataMgr* pUpdateDM = FindDM( sDMAlias );
	if( NULL == pUpdateDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ReservLinkCancel") );
	}

	nSIndex = pGrid->GetIdx();
	if ( 0 > nSIndex )
	{
		AfxMessageBox( _T("선정된 이용자가 없습니다.") , MB_ICONINFORMATION);
		return 100;
	}

	ids = pUpdateDM->GetCellData( _T("지정여부") , nSIndex , sIsComplete );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ReservLinkCancel") );
	}

	if ( _T("O") != sIsComplete )
	{
		AfxMessageBox( _T("선정된 이용자는 예약지정 되어 있지 않습니다.") , MB_ICONINFORMATION );
		return 200;
	}

	ids = pUpdateDM->GetCellData( _T("대출/반납정보KEY") , nSIndex , sLoanKey );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ReservLinkCancel") );

	}
	
	if(_T("Y") == m_pLona->m_sUnmannedReserveSys)
	{
		CString strQuery, strValue, strMsg;;
		strQuery.Format(_T("SELECT UNMANNED_RESERVATION_LOAN FROM LS_WORK_T01 WHERE REC_KEY=%s"), sLoanKey);
		pUpdateDM->GetOneValueQuery(strQuery, strValue);
		
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

	pUpdateDM->StartFrame();	
	pUpdateDM->InitDBFieldData();
	pUpdateDM->AddDBFieldData( _T("RESERVATION_EXPIRE_DATE") , _T("STRING") , _T("")  , TRUE );
	pUpdateDM->AddDBFieldData( _T("REG_NO"), _T("STRING"), _T("") , TRUE );

	pUpdateDM->AddDBFieldData( _T("CALL_NO"), _T("STRING"), _T("") , TRUE );
	
	pUpdateDM->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sLoanKey );
	
	ids = pUpdateDM->SendFrame();
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ReservLinkCancel") );
	}

	pUpdateDM->EndFrame();
	
	CString strLibCode;
	
	strLibCode = m_pInfo->LIB_CODE;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("RESERVATION_EXPIRE_DATE,BOOK_KEY,SPECIES_KEY,SHELF_LOC_CODE,CLASS_NO_TYPE,REG_CODE,SEPARATE_SHELF_CODE,CALL_NO,REG_NO");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = strLibCode;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	ids = pUpdateDM->SetCellData( _T("지정여부") , _T("X") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("ReservLinkCancel") );
	}

	ids = pUpdateDM->SetCellData( _T("등록번호") , _T("") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("ReservLinkCancel") );
	}

	
	
	ids = pUpdateDM->SetCellData( _T("예약만기일") , _T("") , nSIndex );
	if( 0 > ids )  
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ReservLinkCancel") );
	}
	

	pGrid->DisplayLine(nSIndex);
	
	CESL_Mgr*	pMgr	=	pParentMgr->FindSM( _T("BO_LOC_3100") );
	if( NULL != pMgr )
	{
		CString		strReserveMarkUserKey , strSearchUserKey;

		
		CESL_DataMgr*	pDMMemberInfo		= FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
		if( NULL == pDMMemberInfo ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("ReservLinkCancel") );
		}
		
		
		ids	=	pDMMemberInfo->GetCellData( _T("대출자KEY") , 0 , strSearchUserKey );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("ReservLinkCancel") );
		}

		
		ids	=	pUpdateDM->GetCellData( _T("대출자KEY") , nSIndex , strReserveMarkUserKey );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("ReservLinkCancel") );
		}

		if( 0 == strSearchUserKey.Compare(strReserveMarkUserKey) )
		{
			CString		strLoanInfoLoanKey , strReservemarkLoanInfo;
			ids = pUpdateDM->GetCellData( _T("대출/반납정보KEY") , nSIndex , strReservemarkLoanInfo );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("ReservLinkCancel") );
			}

			
			CESL_DataMgr*	pDMLoanInfo		= FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
			if( NULL == pDMLoanInfo ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("ReservLinkCancel") );
			}

			INT		i , nRowCnt;
			nRowCnt	=	pDMLoanInfo->GetRowCount();
			for( i=0 ; i<nRowCnt ; i++ )
			{
				ids = pDMLoanInfo->GetCellData( _T("대출/반납정보KEY") , i , strLoanInfoLoanKey );
				if( 0 > ids )
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("ReservLinkCancel") );
				}

				if( 0 == strReservemarkLoanInfo.Compare( strLoanInfoLoanKey ) )
				{
					ids = pDMLoanInfo->SetCellData( _T("등록번호") , _T("") , i );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("ReservLinkCancel") );
					}

					ids = pDMLoanInfo->SetCellData( _T("예약만기일") , _T("") , i );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("ReservLinkCancel") );
					}

					
					
					ids = pDMLoanInfo->SetCellData( _T("청구기호") , _T("") , i );
					if( 0 > ids )  
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("ReservLinkCancel") );
					}
						
					CESL_Grid*		pGridLoanList	= (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO") , _T("MainGrid") );
					if( NULL == pGridLoanList ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("ReservLinkCancel") );
					}

					
					pGridLoanList->DisplayLine(i);
				}
			}
		}
	}
	
	
	CESL_Mgr* pEslMgr = pParentMgr->FindSM(_T("SM_BO_LOC_RESERVE"));
	if(NULL != pEslMgr)
	{
		CString strLoanKey;
		ids = pUpdateDM->GetCellData(_T("대출/반납정보KEY"), nSIndex, strLoanKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
		CESL_DataMgr* pReserveMainDM = FindDM(_T("DM_BO_LOC_RESERVE"));
		if(NULL == pReserveMainDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
		for(INT i = 0; i < pReserveMainDM->GetRowCount(); i++)
		{
			CString strValue;
			pReserveMainDM->GetCellData(_T("LOAN_KEY"), i, strValue);
			if(strValue == strLoanKey)
			{
				pReserveMainDM->SetCellData(_T("예약만기일"), _T(""), i);
				pReserveMainDM->SetCellData(_T("등록번호"), _T(""), i);
				pReserveMainDM->SetCellData(_T("청구기호"), _T(""), i);
				CESL_Grid* pReserveGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_RESERVE"), _T("MainGrid"));
				if(NULL == pReserveGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ViewExpireDate"));
				pReserveGrid->DisplayLine(i);
				break;
			}
		}
	}

	return 0;
}


VOID CBO_LOC_3170_ReserveInfo::OnbtnLINKCANCEL() 
{
EFS_BEGIN

	
	GetDlgItem( IDC_STATIC_FAILED_PRINT_3170_VR )->SetWindowText( _T("") );	
	
	INT	ids;

	if( AfxMessageBox(_T("선정된 이용자의 예약지정을 취소하시겠습니까?"), MB_YESNO) != IDYES )
	{	
		return;
	}

	ids = ReservLinkCancel();
	if ( 0 > ids )
	{
		AfxMessageBox(_T("예약지정 취소를 할 수 없습니다."));
	
		return;
	}
	else if ( 0 == ids )
	{
		AfxMessageBox(_T("예약지정이 취소되었습니다."));
		
		
		m_bEdit = TRUE;
		
	}

EFS_END
}

VOID CBO_LOC_3170_ReserveInfo::OnClose() 
{	

	OnCancel();
}

HBRUSH CBO_LOC_3170_ReserveInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

VOID CBO_LOC_3170_ReserveInfo::OnTimer(UINT nIDEvent)
{
	
	if( 10 == nIDEvent )
	{
		KillTimer(10);
		AutoJob();
	}
	
	CDialog::OnTimer(nIDEvent);
}


BOOL CBO_LOC_3170_ReserveInfo::PreTranslateMessage(MSG* pMsg) 
{

	return CESL_Mgr::PreTranslateMessage(pMsg);
}
VOID CBO_LOC_3170_ReserveInfo::OnbtnSENDSMSReserveVol()
{
	OnbtnSENDSMS();
}
VOID CBO_LOC_3170_ReserveInfo::OnbtnPrintReserveVol()
{
	OnbtnPrintTest();
	
}

INT CBO_LOC_3170_ReserveInfo::AutoSetReservationDay()
{
EFS_BEGIN

	INT ids = 0;	
		
	CESL_DataMgr*	pBookInfo = FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO"));
	if( NULL == pBookInfo )			
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("AutoSetReservationDay") );
	}
		
	CESL_DataMgr*	pReserveInfo = FindDM( _T("DM_BO_LOC_3170_VR_RESERVE_INFO"));
	if( NULL == pReserveInfo )		
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("AutoSetReservationDay") );
	}
		
	CESL_Grid*		pVRBookInfoGrid	=	(CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRBookInfoGrid") );
	if( NULL == pVRBookInfoGrid )	
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("AutoSetReservationDay") );
	}	
	
	CESL_Grid*		pVRReserveGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
	if( NULL == pVRReserveGrid )	
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("AutoSetReservationDay") );
	}
	
	INT		nSIndex = pVRReserveGrid->GetIdx();

	CString			strReserveMark;
	pReserveInfo->GetCellData( _T("지정여부") , nSIndex , strReserveMark );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("AutoSetReservationDay") );
	}

	if ( _T("O") == strReserveMark )
	{
		AfxMessageBox( _T("선택한 예약자는 이미 지정된 예약자이기 때문에 지정할 수 없습니다") );
		return 0;
	}
	
	CString	strTempReg;		
	INT		nVRBookInfoGridCnt	=	pVRBookInfoGrid->GetCount();	
	INT		nBookIndex , i;
	for( i=0 ; i<nVRBookInfoGridCnt ; i++ )
	{
		pBookInfo->GetCellData( _T("등록번호")	, i , strTempReg );
				
		nBookIndex	=	i;
		break;
		
	}
	
	
	return	0;

EFS_END
return	-1;
}


BEGIN_EVENTSINK_MAP(CBO_LOC_3170_ReserveInfo, CDialog)
	//{{AFX_EVENTSINK_MAP(CBO_LOC_3170_ReserveInfo)
	ON_EVENT(CBO_LOC_3170_ReserveInfo, IDC_gridRI_OTHERS, -601 /* DblClick */, OnDblClickgridRIOTHERS, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBO_LOC_3170_ReserveInfo::OnDblClickgridRIOTHERS() 
{
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3170_VR_RESERVE_INFO"), _T("VRReserveGrid"));
	if(NULL == pGrid){return;}
	if(1 > pGrid->GetMouseRow())
	{
		return;
	}

	INT nIdx = pGrid->GetIdx();
	if(0 > nIdx)
	{
		return;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3170_VR_RESERVE_INFO"));
	m_strSearchUserKey = pDM->GetCellData(_T("대출자KEY"), nIdx);	


	CESL_Mgr* pMgr = pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(pMgr)
	{
		pMgr->SendMessage(WM_SEARCH_USER_FROM_KEY, 1, (WPARAM)&m_strSearchUserKey);
	}
}

void CBO_LOC_3170_ReserveInfo::OnActivate(UINT nState, CWnd* pWndOther, bool bMinimized)
{
    CDialog::OnActivate(nState, pWndOther, bMinimized);
	
    if(bMinimized) return;
	
	switch(nState)
	{
	case WA_INACTIVE : 
		break;
	case WA_ACTIVE:
		break;
	case WA_CLICKACTIVE: 
		CString sDM = _T("");
		CESL_Mgr* pMgr = pParentMgr;
		do
		{
			if(NULL == pMgr) break;
			if(_T("BO_LOC_3100") == pMgr->SM_ALIAS)
			{
				sDM = _T("DM_BO_LOC_3100_LOAN_INFO");
				break;
			}
			else if(_T("BO_LOC_3200") == pMgr->SM_ALIAS)
			{
				sDM = _T("DM_BO_LOC_3200");
				break;
			}
			else if(_T("SM_BO_LOC_RESERVE") == pMgr->SM_ALIAS)
			{
				sDM = _T("DM_BO_LOC_RESERVE");
				break;
			}
			if(NULL == pMgr->pParentMgr)
			{
				pMgr = NULL;
				break;
			}
			pMgr = pMgr->pParentMgr;
		}while(TRUE);
		
		
		CESL_DataMgr* pDM = pParentMgr->FindDM(sDM);
		if( pDM->GetRowCount() == 0 ) break;
		
		HCURSOR hCur = LoadCursor(NULL, IDC_WAIT);
		CString strWindowTxt;
		SetCursor(hCur);
		GetWindowText(strWindowTxt);
		SetWindowText(strWindowTxt + _T("  (갱신중)") );
		m_pReservationInfo->SPFExcutePath( _T("예약정보검색") );
		this->ViewScreen();
		hCur = LoadCursor(NULL, IDC_ARROW);
		SetWindowText(strWindowTxt);
		SetCursor(hCur);
		break;
	}
}
