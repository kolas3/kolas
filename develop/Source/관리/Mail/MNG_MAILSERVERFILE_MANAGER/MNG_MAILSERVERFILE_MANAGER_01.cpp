// MNG_MAILSERVERFILE_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MAILSERVERFILE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAILSERVERFILE_MANAGER_01 dialog


CMNG_MAILSERVERFILE_MANAGER_01::CMNG_MAILSERVERFILE_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MAILSERVERFILE_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_path.Format( _T("..\\cfg\\MailAccount.cfg") );
	m_PartPath.Format( _T("..\\cfg\\MailAccountPart.cfg") );

	m_bEcoMailingSystem = FALSE;
	m_pGrid = NULL;
	m_pGridEdt = NULL;
}

CMNG_MAILSERVERFILE_MANAGER_01::~CMNG_MAILSERVERFILE_MANAGER_01()
{
	
}

BOOL CMNG_MAILSERVERFILE_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_MAILSERVERFILE_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MAILSERVERFILE_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MAILSERVERFILE_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_MAILSERVERFILE_MANAGER_01)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAILSERVERFILE_MANAGER_01 message handlers

BOOL CMNG_MAILSERVERFILE_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MAILSERVERFILE_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MAILSERVERFILE_MANAGER_01") );
		return false;
	}

	CString sValue;
	INT ids = GetManageValue( _T("기타") , _T("공통") , _T("메일링시스템") , _T("사용여부") , sValue );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("InitESL_Mgr Error : OnInitDialog") );
		return false;
	}

	if( sValue.Compare(_T("Y")) == 0 ) m_bEcoMailingSystem = TRUE;

	if( !m_bEcoMailingSystem ) 
	{
		GetDlgItem(IDC_staPASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ePASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_staPASSWORD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eRECEIPT_SERVER)->ShowWindow(SW_HIDE);
	}
	
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_grid);
	LoadPartMailAccount();
	
	// 2009.10.21 ADD BY PWR : 통합 관리자에 경우에만 수정가능
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_eMAILSERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_eMAILACCOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_eSENDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ePASSWORD)->EnableWindow(FALSE);
		GetDlgItem(IDC_eRECEIPT_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_chkSendAccount)->EnableWindow(FALSE);
		GetDlgItem(IDC_eMAILACCOUNT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ePASSWORD2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_eMAILSERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_eMAILACCOUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_eSENDER)->EnableWindow(TRUE);
		GetDlgItem(IDC_ePASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_eRECEIPT_SERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_chkSendAccount)->EnableWindow(TRUE);
		GetDlgItem(IDC_eMAILACCOUNT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ePASSWORD2)->EnableWindow(TRUE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		ShowBeforeInfo();
}

// DM_MNG_MAILSERVERFILE_MANAGER_01


VOID CMNG_MAILSERVERFILE_MANAGER_01::ShowBeforeInfo()
{
	if( GetBeforeDataFromFile() )
		return;

	const INT cnt = 8;
	UINT id[ cnt ] = {
		IDC_eMAILSERVER, IDC_eMAILACCOUNT, IDC_eSENDER , IDC_ePASSWORD , IDC_eRECEIPT_SERVER, IDC_chkSendAccount, IDC_eMAILACCOUNT2, IDC_ePASSWORD2 };

	CEdit *pEdt;
	for( INT i = 0; i < cnt; i++ )
		if( m_fileContents[ i ].GetLength() )
		{
			// 사용유무는 체크박스이므로
			if ( i == 5 )
			{
				if ( m_fileContents[ i ] == _T("Y") )
				{
					( (CButton*) GetDlgItem(IDC_chkSendAccount ) )->SetCheck(TRUE);
				}
				else
				{
					( (CButton*) GetDlgItem(IDC_chkSendAccount ) )->SetCheck(FALSE);
				}
				continue;
			}
			pEdt = ( CEdit * )GetDlgItem( id[ i ] );
			pEdt->SetWindowText( m_fileContents[ i ] );
		}	
}

INT CMNG_MAILSERVERFILE_MANAGER_01::GetBeforeDataFromFile()
{
	CStdioFile fp;
	fp.Open( m_path, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( !fp )
		return -1;
		
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}

	m_strFile.RemoveAll();
	
	CString row;
	while( fp.ReadString( row ) )
	{
		m_strFile.AddTail( row );
		
		row.TrimLeft(); row.TrimRight();
		row.Replace( _T(" "), _T("") );
		if( _T(".") == row.Left( 1 ) || !row.GetLength() )
			continue;

		SetFileContents( row );
	}
	
	return 0;
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::SetFileContents( CString row )
{
	const INT cnt = 8;

	CString title[ cnt ] = {
		_T("메일서버"), _T("메일계정"), _T("발송자명") , _T("패스워드") , _T("수신서버"), _T("보내는메일인증사용"), _T("인증계정"), _T("인증암호") };

	for( INT i = 0; i < cnt; i++ )
		if( row.Find( title[ i ] ) > -1 )
		{			
			row.TrimLeft(); row.TrimRight();
			row.Replace( title[ i ] + _T("="), _T("") );
			m_fileContents[ i ] = row;
		}
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::UpdateMailServerInfo()
{
	GetNewInfo();
	MakeMailServerInfoFile();
	UpdateMailServerInfoToDB();
	SavePartMailAccount();

	AfxMessageBox( _T("변경사항이 적용되었습니다.") );
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::GetNewInfo()
{
	const INT cnt = 8;
	UINT id[ cnt ] = {
		IDC_eMAILSERVER, IDC_eMAILACCOUNT, IDC_eSENDER , IDC_ePASSWORD , IDC_eRECEIPT_SERVER, IDC_chkSendAccount, IDC_eMAILACCOUNT2, IDC_ePASSWORD2 };

	CEdit *pEdt;
	BOOL bCheck;

	for( INT i = 0; i < cnt; i++ )
	{
		// 인증유무
		if ( i==5 ) 
		{
			bCheck = ( ( CButton* ) GetDlgItem ( IDC_chkSendAccount ) )->GetCheck();
			if ( bCheck == TRUE ) m_fileContents[ i ] = _T("Y");
			else m_fileContents[ i ] = _T("N");
			continue;
		}		

		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->GetWindowText( m_fileContents[ i ] );
	}

	
	
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::MakeMailServerInfoFile()
{
	CString row;

	const INT cnt = 8;
	CString title[ cnt ] = {
		_T("메일서버"), _T("메일계정"), _T("발송자명") , _T("패스워드") , _T("수신서버"), _T("보내는메일인증사용"), _T("인증계정"), _T("인증암호") };

	INT nCopyCnt;
	if( m_bEcoMailingSystem )
		nCopyCnt = 5;
	else
		nCopyCnt = 3;

	FILE *file;
	file = _tfopen( m_path, _T("wb") );
	if( !file )
		return;
		
	fseek(file, 0, SEEK_END);		
	if( 0 == ftell(file) )
	{
		/*UNCHANGE*/fputc(0xFF, file);
		/*UNCHANGE*/fputc(0xFE, file);
	}		
	

	for( INT i = 0; i < nCopyCnt; i++ )
	{
	//++2008.10.09 UPDATE BY CJY {{++
	// 데몬에서 공란인식에 문제가 있다.
		row.Format( _T("%s=%s"), title[ i ], m_fileContents[ i ] );
	//DEL 	row.Format( _T("%s = %s"), title[ i ], m_fileContents[ i ] );
	//--2008.10.09 UPDATE BY CJY --}}
		_ftprintf( file, row + _T("\r\n") );

	}

	// 인증을 사용한다면.
	if ( m_fileContents[5] == _T("Y") )
	{	
		for( INT i = 5; i < cnt; i++ )
		{
		//++2008.10.09 UPDATE BY CJY {{++
		// 데몬에서 공란인식에 문제가 있다.
			row.Format( _T("%s=%s"), title[ i ], m_fileContents[ i ] );
		//DEL 	row.Format( _T("%s = %s"), title[ i ], m_fileContents[ i ] );
		//--2008.10.09 UPDATE BY CJY --}}
			_ftprintf( file, row + _T("\n") );
			
		}

	}


	fclose( file );
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::UpdateMailServerInfoToDB()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAILSERVERFILE_MANAGER_01") );
	if( !pDM )
		return;

	pDM->FreeData();

	INT ids = pDM->RefreshDataManager( _T("ALIAS = '메일서버설정'") );
	if( ids < 0 ) return ;

	const INT cnt = 6;
	CString fieldName[ cnt ];
	CString fieldType[ cnt ];
	GetDBFieldInfo( fieldName, fieldType );
	CString data[ cnt ];

	SetDBInfo( data, fieldName, pDM );
	
	BYTE *pBuf = GetFileBuffer( data[ 3 ] );

	pDM->StartFrame();
	pDM->InitDBFieldData();		

	for( INT i = 0; i < cnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	if( pDM->GetRowCount() )
		pDM->MakeUpdateFrame( _T("MN_MANAGE_FILE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ] );
	else
		pDM->MakeInsertFrame( _T("MN_MANAGE_FILE_TBL") );

	pDM->SendFrame();
	pDM->EndFrame();

	pDM->LOBUpdate( _T("MN_MANAGE_FILE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], _T("FILE_DATA"), pBuf, _ttoi( data[ 3 ] ) );
	
	if( pBuf )
	{
		free( pBuf );
		pBuf = NULL;
	}
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::GetDBFieldInfo( CString name[], CString type[] )
{
	name[ 0 ] = _T("REC_KEY");
	name[ 1 ] = _T("ALIAS");
	name[ 2 ] = _T("FILE_NAME");
	name[ 3 ] = _T("FILE_SIZE");
	name[ 4 ] = _T("VERSION");
	name[ 5 ] = _T("LAST_WORK");

	type[ 0 ] = _T("NUMERIC");
	type[ 1 ] = _T("STRING");
	type[ 2 ] = _T("STRING");
	type[ 3 ] = _T("NUMERIC");
	type[ 4 ] = _T("NUMERIC");
	type[ 5 ] = _T("STRING");

}

VOID CMNG_MAILSERVERFILE_MANAGER_01::SetDBInfo( CString data[], CString alias[], CESL_DataMgr *pDM )
{
	switch( pDM->GetRowCount() )
	{
		case 0 :
			pDM->MakeRecKey( data[ 0 ] );
			data[ 1 ] = _T("메일서버설정");
			data[ 2 ] = _T("MailAccount.cfg");
			data[ 4 ] = _T("1");
			data[ 5 ] = GetWorkLogMsg( _T("메일서버파일관리"), __WFILE__, __LINE__ );
			alias[ 5 ] = _T("FIRST_WORK");
			break;
		default :
			pDM->GetCellData( alias, 6, data, 0 );
			//data[ 5 ] = GetWorkLogMsg();
			data[ 5 ] = GetWorkLogMsg( _T("메일서버파일관리"), __WFILE__, __LINE__ );
			data[ 4 ].Format( _T("%d"), _ttoi( data[ 4 ] ) + 1 );
			break;
	}
}

BYTE *CMNG_MAILSERVERFILE_MANAGER_01::GetFileBuffer( CString &size )
{
	CFile fp;
	fp.Open( m_path, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( !fp )
		return NULL;
	
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}
		
	INT fileLength = fp.GetLength();
	size.Format( _T("%d"), fileLength );
	BYTE *pBuf = ( BYTE * )calloc( fileLength, sizeof( BYTE ) );
	fp.ReadHuge( pBuf, fileLength );
	
	fp.Close();

	return pBuf;
}


BEGIN_EVENTSINK_MAP(CMNG_MAILSERVERFILE_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_MAILSERVERFILE_MANAGER_01)
	ON_EVENT(CMNG_MAILSERVERFILE_MANAGER_01, IDC_grid, -601 /* DblClick */, OnDblClickgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_MAILSERVERFILE_MANAGER_01::OnDblClickgrid() 
{
	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	CString sValue = m_pGrid->GetText();
	
	// 헤더를 선택하거나 첫 컬럼을 설택하면 생성해서는 안된다.
	if ( !nRow || nCol == 1 )	return;
	else if ( nCol >= 2 && nCol <= 4 ) GetGridCellNewValue( sValue, nRow, nCol );	
}


VOID CMNG_MAILSERVERFILE_MANAGER_01::GetGridCellNewValue(CString value, INT row, INT col)
{
EFS_BEGIN
		
	// ===================================================================================
	// 1. 초기화
	// ===================================================================================	
	FreeGridEdt();

	/*
	if( m_bFirstReturn ) 
		m_bFirstReturn = FALSE;
	else
	{
		m_bFirstReturn = TRUE;
		return;
	}*/

	// ===================================================================================
	// 2. 입력창 생성
	// ===================================================================================	
	CWnd* pWnd = ( CWnd* )m_pGrid;
	// -----------------------------------------------------------------------------------
	m_pGridEdt = new CGridEdt;	
	m_pGridEdt->row = row;
	m_pGridEdt->col = col;
	m_pGridEdt->parent = ( CWnd * )this;

	if ( col == 3 )
	{
		// 패스워드 모드일경우
		m_pGridEdt->Create( ES_LEFT|WS_CHILD|ES_PASSWORD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );
		m_pGridEdt->ModifyStyle( NULL, ES_PASSWORD);
	}
	else
	{
		m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );
	}

	// ===================================================================================
	// 3. 생성된 입력창의 위치 및 크기 조정
	// ===================================================================================
	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top  -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	// ===================================================================================
	// 4. 입력창으로 포커스 전달
	// ===================================================================================
	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	if ( col == 3 )
	{
		m_pGridEdt->SetWindowText(m_arrayPassword.GetAt(row-1));
	}
	else
	{
		m_pGridEdt->SetWindowText( value );
	}	
	
    m_pGridEdt->SetSel( 0, -1 );
	

EFS_END
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::FreeGridEdt()
{
	EFS_BEGIN

	if( m_pGridEdt != NULL )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}

	EFS_END
}

BOOL CMNG_MAILSERVERFILE_MANAGER_01::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::OnUpdateGridCelText(WPARAM w, LPARAM l)
{
	EFS_BEGIN

	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );		
		if ( m_pGridEdt->col == 3 )
		{
			m_arrayPassword.SetAt(m_pGridEdt->row-1, newValue);
			if ( newValue == _T("") )
			{
				SetGridText( m_pGridEdt->row, m_pGridEdt->col, _T("") );
			}
			else
			{
				SetGridText( m_pGridEdt->row, m_pGridEdt->col, _T("**********") );
			}
			
		}
		else
		{
			SetGridText( m_pGridEdt->row, m_pGridEdt->col, newValue );
		}
		
	}
	FreeGridEdt();
	m_pGrid->SetFocus();


	EFS_END
}
VOID CMNG_MAILSERVERFILE_MANAGER_01::SetGridText(INT row, INT col, CString text)
{
	EFS_BEGIN

	INT curRow = m_pGrid->GetRow();
	INT curCol = m_pGrid->GetCol();

	m_pGrid->SetRow( row );
	m_pGrid->SetCol( col );
	m_pGrid->SetText( text );

	m_pGrid->SetRow( curRow );
	m_pGrid->SetCol( curCol );

	EFS_END
}

VOID CMNG_MAILSERVERFILE_MANAGER_01::LoadPartMailAccount()
{
	CStdioFile fFile;
	CString strData;	
	CString strTmp;
	CString strWork;
	CString strAccount;
	CString strPassword;
	CString strSenderName;
	CString strViewPass;

	INT		nOffset;
	INT		nIndex;
	INT		nItem;
	INT		nRowIndex;
	fFile.Open( m_PartPath , CFile::modeRead | CFile::typeBinary );	
	
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
	}
	
	nRowIndex = 0;
	m_arrayPassword.RemoveAll();
	while( fFile.ReadString(strData) )
	{
		strData.TrimLeft(); strData.TrimRight();		
		if( strData.IsEmpty() ) continue;

		nIndex = 0;
		nItem = 0;
		strWork = _T("");
		strAccount=_T("");
		strPassword=_T("");
		strSenderName=_T("");

		while (TRUE)
		{
			nOffset = strData.FindOneOf(_T("|"));
			if ( nOffset < 0 )
			{
				strSenderName = strData;
				break;
			}
			else
			{
				strTmp = strData.Mid(nIndex, nOffset);				
			}

			switch ( nItem )
			{
				case 0:
					strWork = strTmp;					
					break;
				case 1:
					strAccount = strTmp;					
					break;
				case 2:
					strPassword = strTmp;
					break;
				default:
					break;
			}

			strData = strData.Mid(nOffset+1);
			nItem++;
		}
		m_pGrid->SetRows(nRowIndex+2);
		m_pGrid->SetTextMatrix(nRowIndex+1, 1, strWork);
		m_pGrid->SetTextMatrix(nRowIndex+1, 2, strAccount);

		m_arrayPassword.Add(strPassword);	
		
		if ( !strPassword.IsEmpty() )		
		{
			m_pGrid->SetTextMatrix(nRowIndex+1, 3, _T("**********"));
		}
		

		m_pGrid->SetTextMatrix(nRowIndex+1, 4, strSenderName);	
		nRowIndex++;
	}
	fFile.Close();

}

VOID CMNG_MAILSERVERFILE_MANAGER_01::SavePartMailAccount()
{
	CStdioFile fFile;	
	CString strWork;
	CString strAccount;
	CString strPassword;
	CString strSenderName;
	CString strData;

	INT		nRowCount;
	
	nRowCount = m_pGrid->GetRows();
	fFile.Open( m_PartPath , CFile::modeWrite | CFile::typeText | CFile::modeCreate | CFile::typeBinary );	
	
	if( 0 == fFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fFile.Write( &cUni, sizeof(TCHAR));
	}
		
 	for ( INT i=1 ; i < nRowCount ; i ++ )
	{
		strWork			= m_pGrid->GetTextMatrix(i, 1);
		strAccount		= m_pGrid->GetTextMatrix(i, 2);
		strPassword		= m_arrayPassword.GetAt(i-1);
		strSenderName	= m_pGrid->GetTextMatrix(i, 4);

		strData.Format(_T("%s|%s|%s|%s\r\n"), strWork, strAccount, strPassword, strSenderName);
		fFile.WriteString(strData);
	}

	fFile.Close();
}

HBRUSH CMNG_MAILSERVERFILE_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
