// SMS_DLG_01.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_01.h"
#include "..\\SMS_06\\SMS_061.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_01 dialog


CSMS_DLG_01::CSMS_DLG_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSMSContentsEditor = NULL;
	m_civilNumViewMode = FALSE;
	m_SMSType = _T("");
	m_strNightLoanReservationUseYn = _T("N");
	m_sIsUnityLoanService = _T("");
}

BOOL CSMS_DLG_01::Create(CWnd* pParentWnd)
{	
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

EFS_END
return FALSE;

}

VOID CSMS_DLG_01::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CSMS_DLG_01, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_01)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_01 message handlers

BOOL CSMS_DLG_01::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMS_DLG_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_01") );
		return false;
	}
	
	CStringList SMSList;

	GetManageValue(_T("열람"),_T("공통"),_T("대출자료관리"),_T("야간대출예약기능유무"),m_strNightLoanReservationUseYn);
	m_strNightLoanReservationUseYn.TrimLeft();	m_strNightLoanReservationUseYn.TrimRight();
	m_strNightLoanReservationUseYn.MakeUpper();
 
	GetManageValue(_T("열람"), _T("공통"), _T("부록관리"), _T("부록대출권수포함여부"), m_sIsIncludeLoanCnt);

	if( _T("Y") == m_strNightLoanReservationUseYn )
	{
		SMSList.AddTail( _T("NRL03") );
	}
	SMSList.AddTail( _T("SMS01") );
	SMSList.AddTail( _T("SMS02") );
	SMSList.AddTail( _T("SMS03") );
		
	m_pSMSContentsEditor = new CSMSContentsEditDlg( this, SMSList );

	m_pSMSContentsEditor->m_strNightLoanReservationUseYn = m_strNightLoanReservationUseYn;
	m_pSMSContentsEditor->Create( this );
	m_pSMSContentsEditor->CenterWindow();
	m_pSMSContentsEditor->ShowWindow( false );
	
	SetCivilNumViewMode();
	
	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("GRID") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SMS_DLG_01"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("대출업무관련 SMS이력조회");
	}

	InitGroupCode();

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	if(_T("Y")!=m_sIsUnityLoanService)
	{
		m_sIsUnityLoanService = _T("N");
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CSMS_DLG_01::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize( nType, cx, cy );
	
	// TODO: Add your message handler code here
	
	const INT term = 0;	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	if( pGrid->GetSafeHwnd() )
	{
		pGrid->MoveWindow( term, term, cx - ( term * 2 ), cy - ( term * 2 ) );
	}

EFS_END
}

VOID CSMS_DLG_01::SearchUser( CString where )
{
EFS_BEGIN

	INT		ids , nRowCnt;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_01") );
	if( NULL == pDM )
	{
		return;
	}

	int colNo = 0, colType = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE,INIT_VALUE, DATA_TYPE;
	CString strNewFieldName,strAddWhereCond;
	if( m_sIsIncludeLoanCnt == _T("N") )
	{				
		strNewFieldName.Format(
			_T("(SELECT COUNT(*) FROM LS_WORK_T01 CL WHERE CL.USER_KEY = U.REC_KEY AND CL.STATUS IN ('0','2','L','D') ")
			_T(" AND ( ( SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = CL.BOOK_KEY ) = 'B' ")
			_T(" OR (SELECT BOOK_TYPE FROM SE_BOOK_TBL WHERE REC_KEY = CL.BOOK_KEY )  = 'B' ")
			_T(" OR (SELECT 'B' FROM CO_NON_DB_BOOK_TBL WHERE REC_KEY = CL.BOOK_KEY )  = 'B' )")
			_T(" AND CL.RETURN_PLAN_DATE=L.RETURN_PLAN_DATE AND CL.MANAGE_CODE = UDF_MANAGE_CODE )")
			);
		
		pDM->FindColumn(_T("대출책수"), colNo, colType);
		pDM->GetColumnRefInfo( colNo, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE,INIT_VALUE, DATA_TYPE );
		pDM->SetDataMgrRef( colNo, FIELD_ALIAS, strNewFieldName, FIELD_TYPE,INIT_VALUE, DATA_TYPE );
		
		strNewFieldName.Format(
			_T("(SELECT COUNT(*) FROM LS_WORK_T01 CL WHERE CL.USER_KEY = U.REC_KEY AND CL.STATUS IN ('0','2','L','D') ")
			_T(" AND ( ( SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = CL.BOOK_KEY ) = 'B' ")
			_T(" OR (SELECT BOOK_TYPE FROM SE_BOOK_TBL WHERE REC_KEY = CL.BOOK_KEY )  = 'B' ")
			_T(" OR (SELECT 'B' FROM CO_NON_DB_BOOK_TBL WHERE REC_KEY = CL.BOOK_KEY )  = 'B' )")
			_T(" AND ( SYSDATE - CL.RETURN_PLAN_DATE ) > 0  AND CL.MANAGE_CODE = UDF_MANAGE_CODE )")
			);
		
		pDM->FindColumn(_T("연체책수"), colNo, colType);
		pDM->GetColumnRefInfo( colNo, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE,INIT_VALUE, DATA_TYPE );
		pDM->SetDataMgrRef( colNo, FIELD_ALIAS, strNewFieldName, FIELD_TYPE,INIT_VALUE, DATA_TYPE );
		
		if( !where.IsEmpty() ) where += _T(" AND ");
		
		strAddWhereCond.Format(
			_T(" ( ( SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = L.BOOK_KEY ) = 'B' ")
			_T(" OR (SELECT BOOK_TYPE FROM SE_BOOK_TBL WHERE REC_KEY = L.BOOK_KEY )  = 'B' ")
			_T(" OR (SELECT 'B' FROM CO_NON_DB_BOOK_TBL WHERE REC_KEY = L.BOOK_KEY )  = 'B' )")
			);
		
		where += strAddWhereCond;
	}

	CString str;

	CString strExtraCondition = pDM->EXTRA_CONDITION;
	CString strTblName = pDM->TBL_NAME;
			
	pDM->RefreshDataManager( where );
	nRowCnt	=	pDM->GetRowCount();
	if( 1 >nRowCnt )
	{
		ControlDisplay( _T("CM_SMS_DLG_01"), _T("GRID") );
		AfxMessageBox( _T("검색 결과가 없습니다.") );
		return;
	}
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strEnc;
		CStringArray asDes1, asDes2;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("주민번호"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("휴대폰"), i, strEnc);
			asDes2.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	INT SMSType = _ttoi( m_SMSType.Right( 2 ) );
	switch( SMSType )
	{
		case 1 :
		case 2 :
			MakeSMSList();	
			break;
		default :
			break;
	}

	CLocCommonAPI::ChangeSecuriyCivilNo( this, FindDM( _T("DM_SMS_DLG_01") ), -1, _T("주민번호") , m_sIsUnityLoanService );
	
	INT		i;
	CString		strBuf , strDesc , strCode , strUserClass , strUserPosition , strShelfLocCode;

	CString strValue1;
	ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnFIND") );

	strValue1.TrimLeft();	strValue1.TrimRight();
	strValue1.MakeUpper();

	nRowCnt	=	pDM->GetRowCount();
	for( i=0 ; i<nRowCnt ; i++ )
	{	
		CString strUserNo;
		ids = pDM->GetCellData( _T("대출자번호"), i , strUserNo );
		if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1020 , _T("OnbtnFIND") );

		if( _T("Y") != strValue1 )
		{
			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
		}

		ids = pDM->SetCellData( _T("대출자번호설명"), strUserNo , i );
		if( 0 > ids ) ERROR_MSG_RETURN_VOID2( -1050 , _T("OnbtnFIND") );

		strDesc.Empty();
		pDM->GetCellData( _T("직급") , i , strUserClass );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , strUserClass , strDesc );
		ids = pDM->SetCellData( _T("직급설명"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}

		strDesc.Empty();
		pDM->GetCellData( _T("소속") , i , strShelfLocCode );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , strShelfLocCode , strDesc );
		ids = pDM->SetCellData( _T("소속설명"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}

		strDesc.Empty();
		pDM->GetCellData( _T("대출자료실") , i , strUserPosition );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , strUserPosition , strDesc );
		ids = pDM->SetCellData( _T("대출자료실"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}
	}
	
	ControlDisplay( _T("CM_SMS_DLG_01"), _T("GRID") );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	const INT width = 1200;
	switch( SMSType )
	{
		case 1 :
		case 2 :
			pGrid->SetColWidth( 14, 0, 0 );
			pGrid->SetColWidth( 13, 0, width );
			break;
		case 3 :
			pGrid->SetColWidth( 14, 0, width );
			pGrid->SetColWidth( 13, 0, 0 );
			break;
		default :
			break;
	}

EFS_END
}

VOID CSMS_DLG_01::ViewSMSContentsEditor()
{
EFS_BEGIN
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_01") ); 
	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자를 검색하여 주십시요") );
		return;
	}

	m_pSMSContentsEditor->ShowWindow( TRUE );

EFS_END
}

CSMS_DLG_01::~CSMS_DLG_01()
{
	if( m_pSMSContentsEditor )
	{
		delete m_pSMSContentsEditor;
		m_pSMSContentsEditor = NULL;
	}
}

VOID CSMS_DLG_01::MakeSMSList()
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_01") );
	INT rowCnt = pDM->GetRowCount();
	CString delayCnt;
	for( INT i = rowCnt - 1; i; i-- )
	{
		if(1 == _ttoi(m_SMSType.Right(2)) || 2 == _ttoi(m_SMSType.Right(2)))
		{
			if( pDM->GetCellData( _T("USER_KEY"), i ) == pDM->GetCellData( _T("USER_KEY"), i - 1 )
			&& pDM->GetCellData( _T("반납예정일"), i ) == pDM->GetCellData( _T("반납예정일"), i - 1 ))
			{
				pDM->DeleteRow( i );
			}
		}
		else
		{
			if( pDM->GetCellData( _T("USER_KEY"), i ) == pDM->GetCellData( _T("USER_KEY"), i - 1 ))
			{
				pDM->DeleteRow( i );
			}
		}

	}

EFS_END
}

VOID CSMS_DLG_01::SendSMSMsg()
{
EFS_BEGIN

	INT		ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_01") );
	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		return;
	}

	CArray< INT, INT > checkedRow;
	GetCheckedGridRow( checkedRow );
	INT nRowCnt = checkedRow.GetSize();
	if( 1 > nRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자를 선택하여 주십시요") );
		return;
	}

	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;

	CSMS_MakeMsg MakeSMS( this );
	
	m_pSMSContentsEditor->GetSMSInfo( m_SMSType, SMS_MsgInfo , SMS_ConnectionInfo );
	
	CSMS_Env_Info SMSDMSender( this );

	CString strName, strPhoneNum, strBookTitle, strMsg, strDate, strBookCnt, strSuccessYN;
	
	SMSDMSender.InitSMSFieldData();
	CString dateAlias = _T("반납예정일");
	if( 3 == _ttoi( m_SMSType.Right( 2 ) ) )
	{
		dateAlias = _T("예약만기일");
	}

	CStringList		slName , slPhonNum , slContent , slUserKey , slLoanKey , slSMSReceiptYN;
	CString			strUserKey , strLoanKey , strSMSReceiptYN , strData;

	CStringArray	asPositionCode,asClassCode,asInferiorClass,asUserID,asBirthDay,asCivilNo;

	const INT		nExAliasCnt = 7;
	CString			sExAlias[nExAliasCnt] =
	{
		_T("소속"),		_T("직급"),	_T("회원상태코드"), 
		_T("이용자ID"), _T("생일"), _T("주민번호"),		_T("SMS_USE_YN")
	};

	INT		nIdx , i , j;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		nIdx = checkedRow.GetAt( i );
		
		strName = pDM->GetCellData( _T("대출자명"), nIdx );
		strPhoneNum = pDM->GetCellData( _T("휴대폰"), nIdx );
		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T(" "), _T("") );
		strPhoneNum.Replace( _T("-"), _T("") );
		if( TRUE == strPhoneNum.IsEmpty() )
		{
			continue;
		}

		strBookTitle = pDM->GetCellData( _T("서명"), nIdx );
		strDate = pDM->GetCellData( dateAlias, nIdx );
		if( 2 == _ttoi( m_SMSType.Right( 2 ) ) )
		{
			strBookCnt = pDM->GetCellData( _T("연체책수"), nIdx );
		}
		else if( 1 == _ttoi( m_SMSType.Right( 2 ) ) )
		{
			strBookCnt = pDM->GetCellData( _T("대출책수"), nIdx );
		}
		else if( 3 == _ttoi( m_SMSType.Right( 2 ) ) )
		{
			strBookCnt = _T("1");
		}

		strUserKey	=	pDM->GetCellData( _T("USER_KEY"), nIdx );
		strLoanKey	=	pDM->GetCellData( _T("대출이력KEY"), nIdx );
		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );

		MakeSMS.MakeSMSMsg( m_SMSType , &SMS_MsgInfo , strName , strDate, strBookTitle , strBookCnt , strMsg );

		slName.AddTail( strName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );
		
		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( strLoanKey );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );
		
		for( j=0 ; j<nExAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sExAlias[j] , nIdx , strData );
			if( 0 > ids ) 
			{
				AfxMessageBox( _T("ERROR") );
				return;
			}

			switch(j)
			{
			case 0:
				asPositionCode.Add(strData);
				break;
			case 1:
				asClassCode.Add(strData);
				break;
			case 2:
				asInferiorClass.Add(strData);
				break;
			case 3:
				asUserID.Add(strData);
				break;
			case 4:
				asBirthDay.Add(strData);
				break;
			case 5:
				asCivilNo.Add(strData);
				break;
			}
		}
	}
	
	CSMSSendDlg dlg( this, SMS_ConnectionInfo, m_SMSType , SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.SetInfoEx(  asPositionCode ,	asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo );
	dlg.DoModal();

	if( _T("NRL03") == m_SMSType )
	{
		INT nIdx = dlg.m_gridIdx.GetSize();
		if( nIdx )
		{
			CString strLoanKey;
			for( i=0 ; i<nIdx ; i++ )
			{				
				pDM->GetCellData( _T("대출이력KEY"), dlg.m_gridIdx.GetAt(i)-1, strLoanKey );

				if( TRUE != strLoanKey.IsEmpty() )
				{
					pDM->StartFrame();
					pDM->InitDBFieldData();
					pDM->AddDBFieldData( _T("NIGHT_RESERVATION_SMS_SEND_YN"), _T("STRING"), _T("Y") );
					pDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey );
					pDM->SendFrame();
					pDM->EndFrame();
				}
			}
		}
	}

EFS_END
}

VOID CSMS_DLG_01::GetCheckedGridRow( CArray< INT, INT > &checkedRow )
{
EFS_BEGIN

	checkedRow.RemoveAll();
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );	

EFS_END
}

CString CSMS_DLG_01::GetNowTime( CESL_DataMgr *pDM )
{
EFS_BEGIN

	CTime time = pDM->GetDBTime();
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;

EFS_END
return _T("");

}

VOID CSMS_DLG_01::SetFailGridRow( INT row )
{
EFS_BEGIN

	SetGridRowColor( row, false );

EFS_END
}

VOID CSMS_DLG_01::SetSuccessGridRow( INT row )
{
EFS_BEGIN

	SetGridRowColor( row, true );

EFS_END
}

VOID CSMS_DLG_01::SetGridRowColor( INT row, bool flag )
{
EFS_BEGIN

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();
	INT cols = pGrid->GetCols( 0 );
	
	COLORREF backColor;
	if( flag )
		backColor = RGB( 102, 204, 204 );
	else
		backColor = RGB( 193, 205, 193 );

	pGrid->SetRow( ++row );
	for( INT i = 1; i < cols; i++ )
	{
		pGrid->SetCol( i );
		pGrid->SetCellBackColor( backColor );
	}

	pGrid->SetRow( curRow );
	pGrid->SetCol( curCol );

EFS_END
}

VOID CSMS_DLG_01::SetCivilNumViewMode()
{
EFS_BEGIN

	CString result;
	GetManageValue( _T("열람"), _T("공통"), _T("대출자주민등록번호보여주기"), _T("대출자주민번호"), result );
 
	if( _T("N") == result )
		m_civilNumViewMode = TRUE;	
	else
		m_civilNumViewMode = FALSE;

EFS_END
}



INT CSMS_DLG_01::InitGroupCode()
{
EFS_BEGIN


	INT ids;

	CString sEcoMailUseYN;
	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitGroupCode") );

	if( sEcoMailUseYN.Compare(_T("Y")) != 0 )
	{
		return 0;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("InitGroupCode") );

	ids = pDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitGroupCode") ); 
	

	return 0;

EFS_END
return -1;

}

VOID CSMS_DLG_01::SendOneSMSMsg()
{
	CSMS_06 dlg(this);
	dlg.DoModal();
	
}

HBRUSH CSMS_DLG_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
