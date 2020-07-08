// SMS_DLG_03.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_03.h"
#include "SMSTypeGetter.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_03 dialog


CSMS_DLG_03::CSMS_DLG_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSMSContentsEditor = NULL;
	m_civilNumViewMode = FALSE;
	m_sIsUnityLoanService = _T("");
}

BOOL CSMS_DLG_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	


VOID CSMS_DLG_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_DLG_03, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_03)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tBOOK, OnSelchangetBOOK)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_03 message handlers

BOOL CSMS_DLG_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if( InitESL_Mgr( _T("SMS_DLG_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_03") );
		return false;
	}
	
	CStringList SMSList;
	SMSList.AddTail( _T("SMS07") );
	SMSList.AddTail( _T("SMS08") );

	m_pSMSContentsEditor = new CSMSContentsEditDlg( this, SMSList );
	m_pSMSContentsEditor->Create( this );
	m_pSMSContentsEditor->CenterWindow();
	m_pSMSContentsEditor->ShowWindow( FALSE );
	
	SetCivilNumViewMode();
	
	InitSetCtrl();
	
	CESL_Grid	*pGrid = NULL;
	CArray< CString, CString >removeListArray;
	
	const INT	gridCnt = 3;
	CString		gridAlias[ gridCnt ] = { _T("GRID_BO_BOOK"), _T("GRID_SE_BOOK"), _T("GRID_USER") };

	INT		i;
	for( i=0 ; i<gridCnt ; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SMS_DLG_03"), gridAlias[ i ] );
		if( !pGrid )
		{
			return -1;
		}

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );

		if(!m_pInfo->m_bUserInfoExportYN && _T("GRID_USER") == gridAlias[i])
		{
			pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		}
		else
		{
			pGrid->m_bPrivacyPrintLogRecord = TRUE;
			pGrid->m_strPrintInfo = _T("신착자료 및 추천도서안내 SMS이력조회");
		}

	}
	InitGroupCode();
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSMS_DLG_03::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	ReSize( cx, cy );
}

VOID CSMS_DLG_03::ReSize(INT cx, INT cy)
{
	const UINT staticWidth  = 90;
	const UINT staticHeight = 12;
	const UINT space        = 5;
	INT gridWidth  = cx - 10;
	INT gridHeight = cy / 3 - 25;
	
	CStatic *pStatic = NULL;
	pStatic = ( CStatic * )GetDlgItem( IDC_sUSER_LIST );
	if( pStatic->GetSafeHwnd() )
	{
		pStatic->MoveWindow( space, space, staticWidth, staticHeight );
	}

	pStatic = ( CStatic * )GetDlgItem( IDC_sBOOK_LIST );
	if( pStatic->GetSafeHwnd() )
	{
		pStatic->MoveWindow( space, space * 4 + staticHeight + gridHeight, staticWidth, staticHeight );
	}
	
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
	if( pGrid->GetSafeHwnd() )
	{
		pGrid->MoveWindow( space, space * 2 + staticHeight, gridWidth, gridHeight );
	}
	
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	INT tabY = ( space * 5 ) + ( staticHeight * 2 ) + gridHeight;
	INT tabHeight = cy - gridHeight - ( staticHeight * 2 ) - ( space * 5 );
	if( pTab->GetSafeHwnd() )
	{
		pTab->MoveWindow( 0, tabY, cx, tabHeight );
	}

	INT bookGridWidth = cx;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_BO_BOOK );
	if( pGrid->GetSafeHwnd() )
	{
		pGrid->MoveWindow( space, tabY + 30, gridWidth, tabHeight - 35 );
	}

	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_SE_BOOK );
	if( pGrid->GetSafeHwnd() )
	{
		pGrid->MoveWindow( space, tabY + 30, gridWidth, tabHeight - 35 );	
	}	
}

BEGIN_EVENTSINK_MAP(CSMS_DLG_03, CDialog)
    //{{AFX_EVENTSINK_MAP(CSMS_DLG_03)
	ON_EVENT(CSMS_DLG_03, IDC_gMAIN_GRID_BO_BOOK, -600 /* Click */, OnClickgMAINGRIDBOOK, VTS_NONE)
	ON_EVENT(CSMS_DLG_03, IDC_gMAIN_GRID_SE_BOOK, -600 /* Click */, OnClickgMAINGRIDSEBOOK, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSMS_DLG_03::OnClickgMAINGRIDBOOK() 
{
	// TODO: Add your control notification handler code here

	SelectOneRecordFromBookGrid( IDC_gMAIN_GRID_BO_BOOK );
}

VOID CSMS_DLG_03::SelectOneRecordFromBookGrid( UINT id )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( id );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	INT rows = pGrid->GetRows();
	if( 1 != col )
	{
		return;
	}

	if( _T("V") == pGrid->GetTextMatrix( row, col ) )
	{
		pGrid->SetTextMatrix( row, col, _T("") );
	}
	else
	{
		pGrid->SetTextMatrix( row, col, _T("V") );
	}

	for( INT i = 1; i < rows; i++ )
	{
		if( row == i )
		{
			continue;
		}
		else
		{
			pGrid->SetTextMatrix( i, col, _T("") );
		}
	}
}

VOID CSMS_DLG_03::SearchUser( CString where )
{
	INT		ids , nRowCnt;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_03_USER") );
	pDM->RefreshDataManager( where );
	nRowCnt		=	pDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		AfxMessageBox( _T("검색 결과가 없습니다.") );
		ControlDisplay( _T("CM_SMS_DLG_03"), _T("GRID_USER") );
		return;
	}
	
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strEnc;
		CStringArray asDes1, asDes2, asDes3;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("주민번호"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("주민등록번호"), i, strEnc);
			asDes2.Add(strEnc);
			pDM->GetCellData(_T("휴대폰"), i, strEnc);
			asDes3.Add(strEnc);
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
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("주민등록번호"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰"), asDes3.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
	}

	if( TRUE == m_civilNumViewMode )
	{
		CLocCommonAPI::ChangeSecuriyCivilNo( this, FindDM( _T("DM_SMS_DLG_03_USER") ), -1, _T("주민번호") , m_sIsUnityLoanService );
	}

	INT		i;
	CString		strBuf , strDesc , strCode , strUserClass , strUserPosition;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		pDM->GetCellData( _T("대출자번호"), i, strCode );
		CLocCommonAPI::ChangeSecuriyUserNo( this , strCode , m_sIsUnityLoanService );
		pDM->SetCellData( _T("대출자번호설명"), strCode, i );
		
		strDesc.Empty();
		pDM->GetCellData( _T("직급"), i, strUserClass );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , strUserClass , strDesc );
		ids = pDM->SetCellData( _T("직급설명"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}

		strDesc.Empty();
		pDM->GetCellData( _T("소속"), i, strUserPosition );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , strUserPosition , strDesc );
		ids = pDM->SetCellData( _T("소속설명"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}
	}
	
	ControlDisplay( _T("CM_SMS_DLG_03"), _T("GRID_USER") );
}

VOID CSMS_DLG_03::InitSetCtrl()
{
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	pTab->InsertItem( 0, _T(" 단 행 ") );
	pTab->InsertItem( 1, _T(" 연 속 ") );
	pTab->SetCurSel( 0 );
}

VOID CSMS_DLG_03::ViewSMSContentsEditor()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_03_BO_BOOK") ); // Jong
	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자를 검색하여 주십시요") );
		return;
	} 

	m_pSMSContentsEditor->ShowWindow( TRUE );
}

CSMS_DLG_03::~CSMS_DLG_03()
{
	if( m_pSMSContentsEditor )
	{
		delete m_pSMSContentsEditor;
		m_pSMSContentsEditor = NULL;
	}
}

VOID CSMS_DLG_03::OnSelchangetBOOK(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ViewBookGrid();
	
	*pResult = 0;
}

VOID CSMS_DLG_03::ViewBookGrid()
{
	const INT cnt = 2;
	UINT id[ cnt ] = { IDC_gMAIN_GRID_BO_BOOK , IDC_gMAIN_GRID_SE_BOOK };
	
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	bool flag[ cnt ] = { false, false };

	INT	nCurSel	=	pTab->GetCurSel();
	flag[ nCurSel ] = true;

	m_pSMSContentsEditor->m_nSMS03PublishFormCode	=	nCurSel;

	CMSHFlexGrid *pGrid;
	for( INT i = 0; i < cnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( id[ i ] );
		pGrid->ShowWindow( flag[ i ] );
	}
}

VOID CSMS_DLG_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	ViewBookGrid();
}

VOID CSMS_DLG_03::OnClickgMAINGRIDSEBOOK() 
{
	// TODO: Add your control notification handler code here

	SelectOneRecordFromBookGrid( IDC_gMAIN_GRID_SE_BOOK );
}


VOID CSMS_DLG_03::SearchBOBook( CString where )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_03_BO_BOOK") );
	
	DWORD START, FINISH;
	START = GetTickCount();	
	
	pDM->RefreshDataManager( where );
	
	FINISH = GetTickCount();
	
	CString log;
	log.Format( _T("[ END ] - SMS 비치희망자료 단행검색 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	pDM->QueryLog( log.GetBuffer( 0 ) );
	
	INT		nRowCnt	=	pDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		AfxMessageBox( _T("단행자료 검색 결과가 없습니다.") );
		ControlDisplay( _T("CM_SMS_DLG_03"), _T("GRID_BO_BOOK") );
		return;
	}
	
	CString		strData , strDesc;
	INT		i , ids;
	for( i=0 ; i<nRowCnt; i++ )
	{
		ids	=	pDM->GetCellData( _T("관리구분") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("관리구분") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}

		strData.Empty();		strDesc.Empty();
		ids	=	pDM->GetCellData( _T("매체구분") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("매체구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("매체구분") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}
		
		strData.Empty();		strDesc.Empty();
		ids	=	pDM->GetCellData( _T("등록구분") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("매체구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("등록구분") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}

		strData.Empty();		strDesc.Empty();
		ids	=	pDM->GetCellData( _T("자료실") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("자료실") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}
	}

	ControlDisplay( _T("CM_SMS_DLG_03"), _T("GRID_BO_BOOK") );
}

VOID CSMS_DLG_03::SearchSEBook( CString where )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_03_SE_BOOK") );
	
	DWORD START, FINISH;
	START = GetTickCount();

	pDM->RefreshDataManager( where );

	FINISH = GetTickCount();
	
	CString log;
	log.Format( _T("[ END ] - SMS 비치희망자료 연속 검색 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	pDM->QueryLog( log.GetBuffer( 0 ) );	
	
	INT		nRowCnt		=	pDM->GetRowCount();
	if( 1>nRowCnt )
	{
		AfxMessageBox( _T("연속자료 검색 결과가 없습니다.") );
		ControlDisplay( _T("CM_SMS_DLG_03"), _T("GRID_SE_BOOK") );
		return;
	}

	CString		strData , strDesc;
	INT		i , ids;
	for( i=0 ; i<nRowCnt; i++ )
	{
		ids	=	pDM->GetCellData( _T("관리구분") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("관리구분") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}

		strData.Empty();		strDesc.Empty();
		ids	=	pDM->GetCellData( _T("매체구분") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("매체구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("매체구분") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}
		
		strData.Empty();		strDesc.Empty();
		ids	=	pDM->GetCellData( _T("등록구분") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("매체구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("등록구분") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}

		strData.Empty();		strDesc.Empty();
		ids	=	pDM->GetCellData( _T("자료실") , i , strData );
		if( 0 > ids )
		{
			return;
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , strData , strDesc );
		ids	=	pDM->SetCellData( _T("자료실") , strDesc , i );
		if( 0 > ids )
		{
			return;
		}
	}

	ControlDisplay( _T("CM_SMS_DLG_03"), _T("GRID_SE_BOOK") );
}

VOID CSMS_DLG_03::SendSMSMsg()
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_03_USER") );
	if( 1 > pDM->GetRowCount() )
	{
		return;
	}

	CString strBookTitle = GetBookTitle();
	if( TRUE == strBookTitle.IsEmpty() )
	{
		AfxMessageBox( _T("도서를 선택하여 주십시요") );
		return;
	}
	
	CArray< INT, INT > checkedRow;
	GetCheckedGridRow( checkedRow );
	INT rowCnt = checkedRow.GetSize();
	if( 1 > rowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자를 선택하여 주십시요") );
		return;
	}

	CString strSMSType = GetSMSType();
	if( TRUE == strSMSType.IsEmpty() )
	{
		return;
	}
	
	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;

	CSMS_MakeMsg MakeSMS( this );
	
 	m_pSMSContentsEditor->GetSMSInfo( strSMSType , SMS_MsgInfo , SMS_ConnectionInfo );

	CSMS_Env_Info SMSDMSender( this );

	SMSDMSender.InitSMSFieldData();

	CString			strName, strPhoneNum, strTitle, strMsg, strSuccessYN, strUserKey, strLoanKey, strSMSReceiptYN;
	CStringList		slName, slPhonNum, slContent, slUserKey, slLoanKey, slSMSReceiptYN;
	CStringArray	asPositionCode,asClassCode,asInferiorClass,asUserID,asBirthDay,asCivilNo;

	const INT nExAliasCnt = 6;
	CString sExAlias[nExAliasCnt] =
	{
		_T("소속") , _T("직급") , _T("회원상태코드") , _T("이용자ID") , _T("생일") , _T("주민번호")
	};

	CString strData;
	INT idx , i , j;
	for( i=0 ; i<rowCnt ; i++ )
	{
		idx = checkedRow.GetAt( i );
		
		strName = pDM->GetCellData( _T("대출자명") , idx );
		strPhoneNum = pDM->GetCellData( _T("휴대폰") , idx );
		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T(" "), _T("") );
		strPhoneNum.Replace( _T("-"), _T("") );
		if( TRUE == strPhoneNum.IsEmpty() )
		{
			continue;
		}

		
		
		MakeSMS.MakeSMSMsg( strSMSType , &SMS_MsgInfo , strName , _T("") , strBookTitle , _T("") , strMsg );

		slName.AddTail( strName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );

		strUserKey = pDM->GetCellData( _T("USER_KEY") , idx );
		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN") , idx );
		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( _T("0") );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );

		for( j=0 ; j<nExAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sExAlias[j] , idx , strData );
			if( 0 > ids ) 
			{
				AfxMessageBox( _T("ERROR") );
				return ;
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
	
	CSMSSendDlg dlg( this, SMS_ConnectionInfo, strSMSType, SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.SetInfoEx(  asPositionCode ,	asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo );
	dlg.DoModal();		
}	

CString CSMS_DLG_03::GetBookTitle()
{
	CMSHFlexGrid *pGrid = NULL;
	CESL_DataMgr *pDM = NULL;
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	switch( pTab->GetCurSel() )
	{
		case 0 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_BO_BOOK );
			pDM = FindDM( _T("DM_SMS_DLG_03_BO_BOOK") );
			break;
		case 1 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_SE_BOOK );
			pDM = FindDM( _T("DM_SMS_DLG_03_SE_BOOK") );
			break;
		default :
			return _T("");
	}
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			return pDM->GetCellData( _T("표제"), i - 1 );
	
	return _T("");
}

VOID CSMS_DLG_03::GetCheckedGridRow( CArray< INT, INT > &checkedRow )
{	
	checkedRow.RemoveAll();
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );	
}

CString CSMS_DLG_03::GetSMSType()
{
	CSMSTypeGetter SMSTypeGetter;
	if( SMSTypeGetter.DoModal() )
	{
		return SMSTypeGetter.m_SMSType;
	}
	else
	{
		return _T("");
	}
}

CString CSMS_DLG_03::GetNowTime(CESL_DataMgr *pDM)
{
	CTime time = pDM->GetDBTime();
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}

VOID CSMS_DLG_03::SetFailGridRow(INT row)
{
	SetGridRowColor( row, false );
}

VOID CSMS_DLG_03::SetGridRowColor( INT row, bool flag )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
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
}

VOID CSMS_DLG_03::SetSuccessGridRow( INT row)
{
	SetGridRowColor( row, true );
}

VOID CSMS_DLG_03::SetCivilNumViewMode()
{
	CString result;
	GetManageValue( _T("열람"), _T("공통"), _T("대출자주민등록번호보여주기"), _T("대출자주민번호"), result );

	if( _T("N") == result )
		m_civilNumViewMode = TRUE;
	else
		m_civilNumViewMode = FALSE;
}


INT CSMS_DLG_03::InitGroupCode()
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

HBRUSH CSMS_DLG_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
