// SynchronizeUserLoanInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynchronizeUserLoanInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSynchronizeUserLoanInfoDlg dialog


CSynchronizeUserLoanInfoDlg::CSynchronizeUserLoanInfoDlg( CESL_Mgr *pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CSynchronizeUserLoanInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CSynchronizeUserLoanInfoDlg::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	

VOID CSynchronizeUserLoanInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSynchronizeUserLoanInfoDlg)
	DDX_Control(pDX, IDC_RICHEDIT1, m_richEdt);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSynchronizeUserLoanInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSynchronizeUserLoanInfoDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynchronizeUserLoanInfoDlg message handlers

BOOL CSynchronizeUserLoanInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	AfxInitRichEdit();
	
	if( InitESL_Mgr( _T("SynchronizeUserLoanInfoDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CAT_MANAGER_01") );
		return false;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSynchronizeUserLoanInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		ShowCivilNo();
}

VOID CSynchronizeUserLoanInfoDlg::ShowCivilNo()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SynchronizeUserLoanInfoDlg") );

	pDM->RefreshDataManager( _T("") );

	/*
	CStdioFile fp;
	fp.Open( FILE_NAME, CFile::modeRead );
	
	CString civilNo;
	while( fp.ReadString( civilNo ) )
		pDM->SetCellData( _T("CIVIL_NO"), civilNo, -1 );
	*/
	
	AllControlDisplay( _T("CM_SynchronizeUserLoanInfoDlg") );
}

VOID CSynchronizeUserLoanInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CArray< INT, INT >nRow;
	GetSelectedRowNum( nRow );
	INT cnt = nRow.GetSize();
	SetProgressProperty( cnt );

	CESL_DataMgr *pDM = FindDM( _T("DM_SynchronizeUserLoanInfoDlg") );
	CESL_DataMgr *pUserDM = FindDM( _T("DM_SynchronizeUserLoanInfoDlg_USER") );
	
	CString civilNo, sMsg;
	INT ids = 0;
	for( INT i = 0; i < cnt; i++ )
	{
		civilNo = pDM->GetCellData( _T("CIVIL_NO"), nRow.GetAt( i ) );
		
		m_progress.SetPos( i );
		SelectUser( pUserDM, civilNo );

		ids = AddLoanInfoToMasterUser( pUserDM );
		switch( ids )
		{
			case 0 :
				sMsg.Format( _T("정상회원 없음\n") );
				break;
			case 1 :
				sMsg.Format( _T("★ %s\n"), civilNo );
				sMsg.Empty();
				break;
			default :
				sMsg.Format( _T("정상회원 %d 명 있음\n"), ids );
				break;
		}
		pUserDM->QueryLog( sMsg.GetBuffer( 0 ) );
		ViewLogMsg( sMsg );
	}

	m_progress.SetRange( 0, 1000 );
	m_progress.SetPos( 999 );
	
	//CDialog::OnOK();
}


INT CSynchronizeUserLoanInfoDlg::GetSelectedRowNum( CArray< INT, INT >&nRow )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( pGrid->GetTextMatrix( i, 1 ) == _T("V") )
			nRow.Add( i - 1 );

	return 0;
}

VOID CSynchronizeUserLoanInfoDlg::SetProgressProperty( INT cnt )
{
	m_progress.SetRange( 0, cnt );
	m_progress.ShowPercent( true );
	m_progress.SetStartColor( 8355711 );
	m_progress.SetEndColor( 2631720 );
	m_progress.SetTextColor( 255 );
	m_progress.SetBkColor( 16777215 );
}

INT CSynchronizeUserLoanInfoDlg::SelectUser( CESL_DataMgr *pUserDM, CString civilNo )
{
	CString where;
	where.Format( _T("CIVIL_NO = '%s'"), civilNo );
	pUserDM->RefreshDataManager( where );

	return 0;
}

INT CSynchronizeUserLoanInfoDlg::AddLoanInfoToMasterUser( CESL_DataMgr *pUserDM )
{
	INT nUserCnt = pUserDM->GetRowCount();
	CString sMasterUser;
	CArray< CString, CString >userKeys;
	INT nMasterUserCnt = 0;
	INT	nInferiorClass;
	for( INT i = 0; i < nUserCnt; i++ )
	{
		nInferiorClass = _ttoi( pUserDM->GetCellData( _T("USER_CLASS"), i ) );
		
		if( nInferiorClass )	// 정상회원 아님
			userKeys.Add( pUserDM->GetCellData( _T("REC_KEY"), i ) );
		else					// 정상회원
		{
			nMasterUserCnt++;
			sMasterUser = pUserDM->GetCellData( _T("REC_KEY"), i );
		}
	}

	if( nMasterUserCnt > 1 || !nMasterUserCnt )
		return nMasterUserCnt;
	
	// TEST
	// return nMasterUserCnt;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SynchronizeUserLoanInfoDlg_LoanInfo") );
	pDM->RefreshDataManager( MakeUserWhere( userKeys ) );
	
	AddLoanInfoToMasterUser( sMasterUser, pDM );
	
	return nMasterUserCnt;
}

INT CSynchronizeUserLoanInfoDlg::AddLoanInfoToMasterUser( CString masterUserKey, CESL_DataMgr *pDM )
{
	// TEST
	// return 0;
	
	const INT nFieldCnt = 41;
	CString sFieldName[ nFieldCnt ];
	GetFieldName( sFieldName );
	CString sFieldType[ nFieldCnt ];
	GetFieldType( sFieldType );

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	CString data[ nFieldCnt ];
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( sFieldName, nFieldCnt, data, i );
		MakeLoanInfoInsertQuery( pDM, sFieldName, sFieldType, data, nFieldCnt, masterUserKey );

		if( !( ( i + 1 )% 10 ) && i )
		{
			pDM->SendFrame();
			pDM->EndFrame();

			pDM->StartFrame();
			pDM->InitDBFieldData();
		}
	}

	pDM->SendFrame();
	pDM->EndFrame();	
		
	return 0;
}

CString CSynchronizeUserLoanInfoDlg::MakeUserWhere( CArray< CString, CString >&userKeys )
{
	INT nKeyCnt = userKeys.GetSize();
	if( !nKeyCnt )
		return _T("");

	CString keys;
	for( INT i = 0; i < nKeyCnt; i++ )
		keys = keys + userKeys.GetAt( i ) + _T(", ");

	keys = keys.Left( keys.GetLength() - 2 );

	CString where;
	where.Format( _T("USER_KEY IN( %s )"), keys );
	return where;
}

VOID CSynchronizeUserLoanInfoDlg::GetFieldName( CString fieldName[] )
{
	fieldName[  0 ] = _T("REC_KEY");
	fieldName[  1 ] = _T("PUBLISH_FORM_CODE");
	fieldName[  2 ] = _T("BOOK_KEY");
	fieldName[  3 ] = _T("USER_KEY");
	fieldName[  4 ] = _T("LOAN_KEY");
	fieldName[  5 ] = _T("LOAN_TYPE_CODE");
	fieldName[  6 ] = _T("RETURN_TYPE_CODE");
	fieldName[  7 ] = _T("LOAN_DATE");
	fieldName[  8 ] = _T("RETURN_PLAN_DATE");
	fieldName[  9 ] = _T("RESERVATION_DATE");
	fieldName[ 10 ] = _T("RESERVATION_EXPIRE_DATE");
	fieldName[ 11 ] = _T("RETURN_DATE");
	fieldName[ 12 ] = _T("STATUS");
	fieldName[ 13 ] = _T("SHELF_LOC_CODE");
	fieldName[ 14 ] = _T("ARREAR");
	fieldName[ 15 ] = _T("REPARATION");
	fieldName[ 16 ] = _T("CLASS_NO_TYPE");
	fieldName[ 17 ] = _T("CLASS_NO");
	fieldName[ 18 ] = _T("MANAGE_CODE");
	fieldName[ 19 ] = _T("REG_CODE");
	fieldName[ 20 ] = _T("MEMBER_CLASS");
	fieldName[ 21 ] = _T("SPECIES_KEY");
	fieldName[ 22 ] = _T("USER_CLASS_CODE");
	fieldName[ 23 ] = _T("DELAY_CNT");
	fieldName[ 24 ] = _T("USER_POSITION_CODE");
	fieldName[ 25 ] = _T("RESERVE_CANCEL_DATE");
	fieldName[ 26 ] = _T("DELAY_RETURN_DATE");
	fieldName[ 27 ] = _T("PRE_RETURN_PLAN_DATE");
	fieldName[ 28 ] = _T("FIRST_WORK");
	fieldName[ 29 ] = _T("LAST_WORK");
	fieldName[ 30 ] = _T("SEPARATE_SHELF_CODE");
	fieldName[ 31 ] = _T("REG_NO");
	fieldName[ 32 ] = _T("TITLE");
	fieldName[ 33 ] = _T("AUTHOR");
	fieldName[ 34 ] = _T("PUBLISHER");
	fieldName[ 35 ] = _T("CALL_NO");
	fieldName[ 36 ] = _T("SELF_RETURN_STATION");
	fieldName[ 37 ] = _T("SELF_RETURN_DOOR");
	fieldName[ 38 ] = _T("SELF_LOAN_STATION");
	fieldName[ 39 ] = _T("LOAN_MANAGE_CODE");
	fieldName[ 40 ] = _T("RETURN_MANAGE_CODE");
	//fieldName[ 41 ] = _T("TITLE2");
}

VOID CSynchronizeUserLoanInfoDlg::GetFieldType( CString fieldType[] )
{
	fieldType[  0 ] = _T("NUMERIC");
	fieldType[  1 ] = _T("STRING");
	fieldType[  2 ] = _T("NUMERIC");
	fieldType[  3 ] = _T("NUMERIC");
	fieldType[  4 ] = _T("NUMERIC");
	fieldType[  5 ] = _T("STRING");
	fieldType[  6 ] = _T("STRING");
	fieldType[  7 ] = _T("STRING");
	fieldType[  8 ] = _T("STRING");
	fieldType[  9 ] = _T("STRING");
	fieldType[ 10 ] = _T("STRING");
	fieldType[ 11 ] = _T("STRING");
	fieldType[ 12 ] = _T("STRING");
	fieldType[ 13 ] = _T("STRING");
	fieldType[ 14 ] = _T("NUMERIC");
	fieldType[ 15 ] = _T("NUMERIC");
	fieldType[ 16 ] = _T("STRING");
	fieldType[ 17 ] = _T("STRING");
	fieldType[ 18 ] = _T("STRING");
	fieldType[ 19 ] = _T("STRING");
	fieldType[ 20 ] = _T("STRING");
	fieldType[ 21 ] = _T("NUMERIC");
	fieldType[ 22 ] = _T("STRING");
	fieldType[ 23 ] = _T("NUMERIC");
	fieldType[ 24 ] = _T("STRING");
	fieldType[ 25 ] = _T("STRING");
	fieldType[ 26 ] = _T("STRING");
	fieldType[ 27 ] = _T("STRING");
	fieldType[ 28 ] = _T("STRING");
	fieldType[ 29 ] = _T("STRING");
	fieldType[ 30 ] = _T("STRING");
	fieldType[ 31 ] = _T("STRING");
	fieldType[ 32 ] = _T("STRING");
	fieldType[ 33 ] = _T("STRING");
	fieldType[ 34 ] = _T("STRING");
	fieldType[ 35 ] = _T("STRING");
	fieldType[ 36 ] = _T("STRING");
	fieldType[ 37 ] = _T("STRING");
	fieldType[ 38 ] = _T("STRING");
	fieldType[ 39 ] = _T("STRING");
	fieldType[ 40 ] = _T("STRING");
	//fieldType[ 41 ] = _T("STRING");
}

INT CSynchronizeUserLoanInfoDlg::MakeLoanInfoInsertQuery( 
														 CESL_DataMgr *pDM, 
														 CString fieldName[], 
														 CString fieldType[], 
														 CString data[],
														 INT cnt,
														 CString masterUserKey )
{
	data[  0 ] = _T("ESL_SEQ.NEXTVAL");
	data[  3 ] = masterUserKey;
	data[ 28 ] += _T(" - 대출이력통합");
	
	for( INT i = 0; i < cnt; i++ )
		pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );
	pDM->MakeInsertFrame( _T("LH_STORAGE_V01") );
		
	return 0;
}

VOID CSynchronizeUserLoanInfoDlg::ViewLogMsg( CString sMsg )
{
	CString text;
	m_richEdt.GetWindowText( text );

	text += sMsg;
	m_richEdt.SetWindowText( text );
}
