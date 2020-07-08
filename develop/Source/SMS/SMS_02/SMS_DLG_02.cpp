// SMS_DLG_02.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_02.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_02 dialog


CSMS_DLG_02::CSMS_DLG_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSMSContentsEditor = NULL;
	m_civilNumViewMode = FALSE;
	m_sIsUnityLoanService = _T("");
}

BOOL CSMS_DLG_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CSMS_DLG_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_DLG_02, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_02)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_02 message handlers

BOOL CSMS_DLG_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMS_DLG_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_02") );
		return false;
	}
	
	CStringList SMSList;
	SMSList.AddTail( _T("SMS04") );
	SMSList.AddTail( _T("SMS18") );
	SMSList.AddTail( _T("SMS19") );

	m_pSMSContentsEditor = new CSMSContentsEditDlg( this, SMSList );
	m_pSMSContentsEditor->Create( this );
	m_pSMSContentsEditor->CenterWindow();
	m_pSMSContentsEditor->ShowWindow( false );

	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("GRID") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SMS_DLG_02"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );

		if(!m_pInfo->m_bUserInfoExportYN)
		{
			pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		}
		else
		{
			pGrid->m_bPrivacyPrintLogRecord = TRUE;
			pGrid->m_strPrintInfo = _T("희망도서도착안내 SMS이력조회");
		}
	}

	SetCivilNumViewMode();

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	if(_T("Y")!=m_sIsUnityLoanService)
	{
		m_sIsUnityLoanService = _T("N");
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSMS_DLG_02::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	const INT term = 0;
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( term, term, cx - ( term * 2 ), cy - ( term * 2 ) );	
}

VOID CSMS_DLG_02::SearchData( CString where )
{
	INT		nRowCnt;
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_02") );
	
	DWORD START, FINISH;
	START = GetTickCount();
	
	pDM->RefreshDataManager( where );

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1,asDes2,asDes3,asDes4;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("주소"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("주민등록번호"), i, strEnc);
			asDes2.Add(strEnc);
			pDM->GetCellData(_T("전자우편"), i, strEnc);
			asDes3.Add(strEnc);
			pDM->GetCellData(_T("전화번호"), i, strEnc);
			asDes4.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes4);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주소"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("주민등록번호"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("전자우편"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("전화번호"), asDes4.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
	}

	FINISH = GetTickCount();
	CString log;
	log.Format( _T("[ END ] - SMS 신착자료 [ Time = %d.%03d sec ]"), ( FINISH - START ) / 1000, ( FINISH - START ) % 1000 ); 
	pDM->QueryLog( log.GetBuffer( 0 ) );

	nRowCnt		=	pDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		ControlDisplay( _T("CM_SMS_DLG_02"), _T("GRID") );
		AfxMessageBox( _T("검색 결과가 없습니다.") );
	}

	CLocCommonAPI::ChangeSecuriyCivilNo( this, FindDM( _T("DM_SMS_DLG_02") ), -1, _T("주민등록번호") , m_sIsUnityLoanService );

	ControlDisplay( _T("CM_SMS_DLG_02"), _T("GRID") );
}

VOID CSMS_DLG_02::ViewSMSContentsEditor()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_02") );
	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자를 검색하여 주십시요") );
		return;
	} 

	m_pSMSContentsEditor->ShowWindow( TRUE );
}

CSMS_DLG_02::~CSMS_DLG_02()
{
	if( m_pSMSContentsEditor )
	{
		delete m_pSMSContentsEditor;
		m_pSMSContentsEditor = NULL;
	}
}

VOID CSMS_DLG_02::SendSMSMsg()
{
	INT		ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_02") );
	if( NULL == pDM )
	{
		return;
	}
	
	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
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

	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;

	CSMS_MakeMsg MakeSMS( this );
	
 	m_pSMSContentsEditor->GetSMSInfo( _T("SMS04"), SMS_MsgInfo , SMS_ConnectionInfo );

	CSMS_Env_Info SMSDMSender( this );

	INT success = 0, fail = 0;
	INT nIdx;
	SMSDMSender.InitSMSFieldData();

	CStringList		slName, slPhonNum, slContent, slUserKey, slLoanKey, slSMSReceiptYN;
	CString			strName, strPhoneNum, strBookTitle, strMsg,	strSuccessYN , strUserKey , strSMSReceiptYN;
	for( INT i = 0; i < rowCnt; i++ )
	{
		nIdx = checkedRow.GetAt( i );
		
		ids =	pDM->GetCellData( _T("신청자") , nIdx , strName );
		if( 0 > ids )
		{
			return;
		}
	
		ids =	pDM->GetCellData( _T("전화번호"), nIdx , strPhoneNum );
		if( 0 > ids )
		{
			return;
		}

		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T(" "), _T("") );
		strPhoneNum.Replace( _T("-"), _T("") );
		if( TRUE == strPhoneNum.IsEmpty() )
		{
			continue;
		}


		ids =	pDM->GetCellData( _T("표제"), nIdx , strBookTitle );
		if( 0 > ids )
		{
			return;
		}

		ids	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx , strSMSReceiptYN );
		if( 0 > ids )
		{
			return;
		}

		MakeSMS.MakeSMSMsg( _T("SMS04") , &SMS_MsgInfo , strName , _T("") , strBookTitle , _T("") , strMsg );
	
		slName.AddTail( strName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );

		strUserKey		=	( pDM->GetCellData( _T("레코드KEY"), nIdx ) );
		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( _T("0") );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );
	}

	CSMSSendDlg dlg( this , SMS_ConnectionInfo, _T("SMS04"), SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.DoModal();		
}

VOID CSMS_DLG_02::GetCheckedGridRow( CArray< INT, INT > &checkedRow )
{	
	checkedRow.RemoveAll();
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );	
}

CString CSMS_DLG_02::GetNowTime(CESL_DataMgr *pDM)
{
	CTime time = pDM->GetDBTime();
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}

VOID CSMS_DLG_02::SetFailGridRow(INT row)
{
	SetGridRowColor( row, false );
}

VOID CSMS_DLG_02::SetGridRowColor( INT row, bool flag )
{
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
}

VOID CSMS_DLG_02::SetSuccessGridRow( INT row )
{
	SetGridRowColor( row, true );
}

HBRUSH CSMS_DLG_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

VOID CSMS_DLG_02::SetCivilNumViewMode()
{
	CString result;
	GetManageValue( _T("열람"), _T("공통"), _T("대출자주민등록번호보여주기"), _T("대출자주민번호"), result );
 
	if( _T("N") == result )
		m_civilNumViewMode = TRUE;
	else
		m_civilNumViewMode = FALSE;
}
