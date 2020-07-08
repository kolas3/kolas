// ExcelHeaderManager.cpp : implementation file
//

#include "stdafx.h"
#include "ExcelHeaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExcelHeaderManager dialog


CExcelHeaderManager::CExcelHeaderManager( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CExcelHeaderManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGridEdt = NULL;
	m_pCombo = NULL;
	// KOL.UDF.022
	m_gridRow = 0;
	m_gridCol = 0;

	int i=0;
	for(i=0;i<DMALIAS_CNT;i++){
		m_DMAliasList[ i ] = _T("");
	}

	m_pGrid = NULL;
	m_sDMAlias.RemoveAll();
	m_sExcelHeaderAlias.RemoveAll();	
}

CExcelHeaderManager::~CExcelHeaderManager()
{

}

BOOL CExcelHeaderManager::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	

VOID CExcelHeaderManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcelHeaderManager)
	CString strText;	
	GetParent()->GetDlgItem(IDC_bSET_DM_ALIAS)->GetWindowText(strText);
	SetWindowText(strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExcelHeaderManager, CDialog)
	//{{AFX_MSG_MAP(CExcelHeaderManager)
	ON_BN_CLICKED(IDC_b3300_RELOAD, OnbRELOAD)
	ON_BN_CLICKED(IDC_b3300_SAVE, OnbSAVE)
	ON_CBN_SELCHANGE(IDC_cDMALIAS_LIST, OnSelchangecDMALIASLIST)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_MESSAGE( UPDATE_GRID_EDT, OnUpdateGridCelText)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelHeaderManager message handlers

BOOL CExcelHeaderManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( InitESL_Mgr( _T("ExcelHeaderManager") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : ExcelHeaderManager") );
		return false;
	}

	m_pGrid  = ( CESL_Grid * )GetDlgItem( IDC_FileInputOuputGrid );
	ShowGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CExcelHeaderManager::GetAliasInfo()
{
	CStdioFile fp;
	fp.Open( FILE_PATH, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( !fp.m_pStream )
	{
		AfxMessageBox( _T("반입반출필드정보.txt 파일을 찾을 수 없습니다.") );
		return -1;
	}
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}

	m_sDMAlias.RemoveAll();
	m_sExcelHeaderAlias.RemoveAll();
	
	CString sLine, sDMAlias, sExcelAlias;
	INT locate = 0;
	while( fp.ReadString( sLine ) )
	{
		if( _T(".") == sLine.Left( 0 ) )
			continue;
		
		locate = sLine.Find( _T(":") );
		sDMAlias = sLine.Left( locate );
		sExcelAlias = sLine.Right( sLine.GetLength() - locate - 1 );
		
		m_sDMAlias.AddTail( sDMAlias );
		m_sExcelHeaderAlias.AddTail( sExcelAlias );
	}
	
	return 0;
}

INT CExcelHeaderManager::ShowAliasInfo()
{
	const INT cnt = 2;
	INT rowCnt = m_sDMAlias.GetCount();

	CString field[ cnt ] = {
		_T("DMAlias"), _T("ExcelHeaderAlias") };
	CString data[ cnt ];

	CESL_DataMgr *pDM = FindDM( _T("DM_ExcelHeaderManager") );
	for( INT i = 0; i < rowCnt; i++ )
	{
		data[ 0 ] = m_sDMAlias.GetAt( m_sDMAlias.FindIndex( i ) );
		data[ 1 ] = m_sExcelHeaderAlias.GetAt( m_sExcelHeaderAlias.FindIndex( i ) );

		pDM->SetCellData( field, cnt, data, -1 );
	}
	ControlDisplay( _T("CM_ExcelHeaderManager"), _T("GRID") );


	return 0;
}

VOID CExcelHeaderManager::OnbRELOAD() 
{
	// TODO: Add your control notification handler code here
	
	ReloadAliasInfo();
}

VOID CExcelHeaderManager::ReloadAliasInfo()
{
	if( !GetAliasInfo() )
		ShowAliasInfo();		
}

VOID CExcelHeaderManager::OnbSAVE() 
{
	// TODO: Add your control notification handler code here
	
	SaveAliasInfoFile();
}

INT CExcelHeaderManager::ResetAliasInfo()
{
	m_sDMAlias.RemoveAll();
	m_sExcelHeaderAlias.RemoveAll();

	INT rowCnt = m_pGrid->GetRows();
	for( INT i = 1; i < rowCnt; i++ )
	{
		m_sDMAlias.AddTail( m_pGrid->GetTextMatrix( i, 1 ) );
		m_sExcelHeaderAlias.AddTail( m_pGrid->GetTextMatrix( i, 2 ) );
	}
	
	return 0;
}

INT CExcelHeaderManager::SaveAliasInfoFile()
{
	
	FILE *fp = _tfopen( FILE_PATH, _T("w+b") );
	if( NULL == fp )
	{
		CString sMsg = _T("");
		sMsg.Format(_T("%s 파일을 열 수 없습니다."), FILE_PATH);
		AfxMessageBox( sMsg );
		return -1;
	}
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	

	CESL_DataMgr *pDM = FindDM( _T("DM_COLUMN_NAME") );
	INT nRowCnt = pDM->GetRowCount();
	CString output = _T("");
	for( INT i= 0; i < nRowCnt; i++ )
	{
		output.Format( _T("%s:%s:%s:\r\n"), pDM->GetCellData( _T("컬럼"), i ), pDM->GetCellData( _T("수정YN"), i ),
			                          pDM->GetCellData( _T("사용YN"), i ) );
		_ftprintf( fp, _T("%s"), output );
	}
	fclose( fp );

	AfxMessageBox( _T("저장되었습니다.") );
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CExcelHeaderManager, CDialog)
    //{{AFX_EVENTSINK_MAP(CExcelHeaderManager)
	ON_EVENT(CExcelHeaderManager, IDC_FileInputOuputGrid, -601 /* DblClick */, OnDblClickFileInputOuputGrid, VTS_NONE)
	ON_EVENT(CExcelHeaderManager, IDC_FileInputOuputGrid, -600 /* Click */, OnClickFileInputOuputGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CExcelHeaderManager::OnDblClickFileInputOuputGrid() 
{
	if(m_pGrid->GetMouseRow() == 0) return;

	CESL_DataMgr *pDM = FindDM( _T("DM_COLUMN_NAME") );
	if (pDM == NULL) 
	{
		AfxMessageBox( _T("CExcelHeaderManager::OnDblClickFileInputOuputGrid() ERROR") );
		return;
	}
	if (pDM->GetRowCount() == 0) return;

	CString strData = _T("");
	INT nRow = m_pGrid->GetRow() - 1;
	INT nCol = m_pGrid->GetCol();
	if(nCol != 3) return ;
	
	pDM->GetCellData( _T("수정YN"), nRow, strData );
	if ( !strData.IsEmpty() )
	{
		CString sMsg = _T("");
		pDM->GetCellData( _T("컬럼"), nRow, strData );

		sMsg.Format(_T("'%s'는 필수컬럼으로 수정할 수 없습니다."), strData);
		AfxMessageBox(sMsg);
		return;
	}

	pDM->GetCellData( _T("사용YN"), nRow, strData );
	if ( _tcscmp(strData, _T("Y")) == 0 )	strData = _T("N");
	else								strData = _T("Y");

	pDM->SetCellData( _T("사용YN"), strData, nRow );

	m_pGrid->DisplayLine(nRow);
	m_pGrid->PostMessage(ESL_GRID_MODIFY, 0, 0);
}

VOID CExcelHeaderManager::ModifyAliasInfo()
{
	return;
	
	INT row = m_pGrid->GetRow();
	INT col = m_pGrid->GetCol();
	CString text = m_pGrid->GetTextMatrix( row, col );

	if( 2 == col )
		CreateGridCell( row, col, text );
}

VOID CExcelHeaderManager::CreateGridCell(INT row, INT col, CString text)
{
	FreeGridEdt();
	m_pGridEdt = new CGridEdt;

	m_pGridEdt->row = row;
	m_pGridEdt->col = col;
	
	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_FileInputOuputGrid );
	m_pGridEdt->parent = ( CWnd * )this;
	m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );

	CRect rect;
	GetGridCellRect( rect );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	m_pGridEdt->SetWindowText( text );
    m_pGridEdt->SetSel( 0, -1 );
}

VOID CExcelHeaderManager::FreeGridEdt()
{
	if( m_pGridEdt )
		delete m_pGridEdt;
	m_pGridEdt = NULL;
}

VOID CExcelHeaderManager::OnUpdateGridCelText(WPARAM w, LPARAM l)
{
	if( w )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );
		m_pGrid->SetTextMatrix( m_pGridEdt->row, m_pGridEdt->col, newValue );
	}

	FreeGridEdt();
}

INT CExcelHeaderManager::MakeDMAliasListCombo()
{
	SetDMAliasArray();
							
	m_pCombo->ResetContent();
	for( INT i = 0; i < DMALIAS_CNT; i++ )
		m_pCombo->InsertString( i, m_DMAliasList[ i ] );
	m_pCombo->SetCurSel( 0 );

	return 0;
}

BOOL CExcelHeaderManager::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CExcelHeaderManager::SetDMAliasArray()
{
	m_DMAliasList[  0 ] = _T("-선택-");			
	m_DMAliasList[  1 ] = _T("대출자ID");		
	m_DMAliasList[  2 ] = _T("대출자번호");		
	m_DMAliasList[  3 ] = _T("집_주소");		
	m_DMAliasList[  4 ] = _T("집_우편번호");	
	m_DMAliasList[  5 ] = _T("근무지_주소");	
	m_DMAliasList[  6 ] = _T("근무지_우편번호");
	m_DMAliasList[  7 ] = _T("생년월일");		
	m_DMAliasList[  8 ] = _T("근무지_직급명");	
	m_DMAliasList[  9 ] = _T("근무지_부서명");	
	m_DMAliasList[ 10 ] = _T("내선번호");		
	m_DMAliasList[ 11 ] = _T("E_MAIL");			
	m_DMAliasList[ 12 ] = _T("학년");			
	m_DMAliasList[ 13 ] = _T("휴대폰");			
	m_DMAliasList[ 14 ] = _T("직급키");			
	m_DMAliasList[ 15 ] = _T("소속키");			
	m_DMAliasList[ 16 ] = _T("우편발송지역");	
	m_DMAliasList[ 17 ] = _T("회원구분");		
	m_DMAliasList[ 18 ] = _T("이름");			
	m_DMAliasList[ 19 ] = _T("비밀번호");		
	m_DMAliasList[ 20 ] = _T("집_전화번호");	
	m_DMAliasList[ 21 ] = _T("근무지_전화번호");
	m_DMAliasList[ 22 ] = _T("학교");			
	m_DMAliasList[ 23 ] = _T("반");				
	m_DMAliasList[ 24 ] = _T("교번");			
	m_DMAliasList[ 25 ] = _T("비고");			
}

VOID CExcelHeaderManager::ShowGridCombo( INT row, INT col, CString text )
{
	CRect rect;
	GetGridCellRect( rect );

	CRect comboRect;
	m_pCombo->GetWindowRect( comboRect );
	m_pCombo->MoveWindow( rect.left - 3, rect.top - 3, rect.Width() + 4, comboRect.Height() );

	if( text.IsEmpty() )
		m_pCombo->SetCurSel( 0 );
	else
		for( INT i = 0; i < DMALIAS_CNT; i++ )
			if( m_DMAliasList[ i ] == text )
				m_pCombo->SetCurSel( i );
			else
				m_pCombo->SetCurSel( 0 );

	m_pCombo->ShowWindow( true );
	m_pGrid->EnableWindow( false );
	m_pCombo->SetFocus();

	m_gridRow = row;
	m_gridCol = col;
}

INT CExcelHeaderManager::GetGridCellRect( CRect &rect )
{
	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_FileInputOuputGrid );

	CRect gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	
	return 0;
}

VOID CExcelHeaderManager::OnSelchangecDMALIASLIST() 
{
	// TODO: Add your control notification handler code here
	
	CString alias;
	m_pCombo->GetWindowText( alias );

	m_pGrid->SetTextMatrix( m_gridRow, m_gridCol, alias );
	m_pGrid->EnableWindow( true );
	m_pGrid->SetFocus();
	m_pCombo->ShowWindow( false );
}

VOID CExcelHeaderManager::OnClickFileInputOuputGrid() 
{
	// TODO: Add your control notification handler code here
	
	return;

	INT row = m_pGrid->GetRow();
	INT col = m_pGrid->GetCol();
	CString text = m_pGrid->GetTextMatrix( row, col );

	if( 1 == col )
		ShowGridCombo( row, col, text );
}

VOID CExcelHeaderManager::OnbINPUT() 
{
	// TODO: Add your control notification handler code here
	
	CExcelHeaderSetter dlg( this );
	dlg.m_inputMode = 1;
	dlg.DoModal();

	ControlDisplay( _T("CM_ExcelHeaderManager"), _T("GRID") );
}

VOID CExcelHeaderManager::OnbUPDATE() 
{
	// TODO: Add your control notification handler code here
	
	CExcelHeaderSetter dlg( this );
	GetCheckedRow( dlg.m_checkedRow );
	if( !dlg.m_checkedRow.GetSize() )
	{
		AfxMessageBox( _T("수정항목을 선택하여 주십시요") );
		return;
	}

	dlg.m_inputMode = 2;
	dlg.DoModal();

	ControlDisplay( _T("CM_ExcelHeaderManager"), _T("GRID") );
}

VOID CExcelHeaderManager::OnbDELETE() 
{
	// TODO: Add your control notification handler code here

	CArray< INT, INT >checkedRow;
	GetCheckedRow( checkedRow );

	INT cnt = checkedRow.GetSize();
	if( !cnt )
	{
		AfxMessageBox( _T("삭제할 항목을 선택하여 주십시오") );
		return;
	}

	CString msg;
	msg.Format( _T("선택한 %d개 항목을 삭제하시겠습니까?"), cnt );
	if( IDYES != MessageBox( msg, _T("KOLAS2Up"), MB_YESNO ) )
		return;

	CESL_DataMgr *pDM = FindDM( _T("DM_ExcelHeaderManager") );
	for( INT i = cnt -1; i > -1; i-- )
		pDM->DeleteRow( checkedRow[ i ] );
	ControlDisplay( _T("CM_ExcelHeaderManager"), _T("GRID") );
}

VOID CExcelHeaderManager::SaveAliasInfoFile( CString data[], INT idx )
{
	const INT cnt = 2;
	CString field[ cnt ] = {
		_T("DMAlias"), _T("ExcelHeaderAlias") };
	CESL_DataMgr *pDM = FindDM( _T("DM_ExcelHeaderManager") );
	
	pDM->SetCellData( field, cnt, data, idx );
}

VOID CExcelHeaderManager::GetCheckedRow( CArray< INT, INT >&checkedRow )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_FileInputOuputGrid );
	INT rowCnt = pGrid->GetRows();

	for( INT i = 1; i < rowCnt; i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );
}

VOID CExcelHeaderManager::GetAliasData( CString data[], INT idx )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_ExcelHeaderManager") );
	const INT cnt = 2;
	CString field[ cnt ] = {
		_T("DMAlias"), _T("ExcelHeaderAlias") };
		
	pDM->GetCellData( field, cnt, data, idx );
}

VOID CExcelHeaderManager::ShowGrid()
{
	ControlDisplay( _T("CM_ExcelHeaderManager"), _T("GRID") );
}

HBRUSH CExcelHeaderManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
