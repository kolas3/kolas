// LoanCardFormatSetter.cpp : implementation file
//

#include "stdafx.h"
#include "LoanCardFormatSetter.h"

#include "..\BO_LOC_3300\BO_LOC_3395.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanCardFormatSetter dialog


CLoanCardFormatSetter::CLoanCardFormatSetter( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CLoanCardFormatSetter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_objRank = 0;

	SetDefaultLoanCardType();
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CLoanCardFormatSetter::~CLoanCardFormatSetter()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

BOOL CLoanCardFormatSetter::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	
	
VOID CLoanCardFormatSetter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanCardFormatSetter)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanCardFormatSetter, CDialog)
	//{{AFX_MSG_MAP(CLoanCardFormatSetter)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_bINPUT, OnbINPUT)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(ID_bDELETE, OnbDELETE)
	ON_BN_CLICKED(ID_bINIT_RANK, OnbINITRANK)
	ON_BN_CLICKED(ID_bSAVE_RANK, OnbSAVERANK)
	ON_CBN_SELCHANGE(IDC_cPTRN_LIST, OnSelchangecPTRNLIST)
	ON_BN_CLICKED(IDC_bADD_PTRN, OnbADDPTRN)
	ON_BN_CLICKED(IDC_bDELETE_PTRN, OnbDELETEPTRN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanCardFormatSetter message handlers

BOOL CLoanCardFormatSetter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here

	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );
	
	if( InitESL_Mgr( _T("LoanCardFormatSetter") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : LoanCardFormatSetter") );
		return false;
	}

	// DM_LoanCardFormatSetter
	// CM_LoanCardFormatSetter


	MakePtrnList();

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter_PtrnList") );
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
		if( m_defaultPtrn == pDM->GetCellData( _T("PATTERN"), i ) )
		{
			pCombo->SetCurSel( i + 1 );
			SetCurrentPtrn();
			break;
		}
		else
			pCombo->SetCurSel( 0 );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* 그리드 alias
ALIAS
제목
박스 크기 X, Y, WIDTH, HEIGHT 
TYPE
사용여부
고정
내용
바탕색
라인당 문자수
최대문자수
수평정렬
수직정렬
수평선 보임
수직선 보임
폰트
*/

VOID CLoanCardFormatSetter::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	ResizeWindow();
}

VOID CLoanCardFormatSetter::ResizeWindow()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	if( !pGrid->GetSafeHwnd() )
		return;

	CRect wndRect, rect;
	pGrid->GetWindowRect( rect );
	this->GetWindowRect( wndRect );
	ScreenToClient( rect );
	ScreenToClient( wndRect );
	
	const INT topTerm = 120;
	const INT term = 5;
	pGrid->MoveWindow( wndRect.left + term, wndRect.top + topTerm, wndRect.Width() - ( term * 2 ), wndRect.Height() - topTerm - term );
	
	CButton *pBtn = ( CButton * )GetDlgItem( IDCANCEL );
	pBtn->GetWindowRect( rect );
	ScreenToClient( rect );
	pBtn->MoveWindow( wndRect.Width() - rect.Width() - ( term * 2 ), rect.top, rect.Width(), rect.Height() );
	
}

VOID CLoanCardFormatSetter::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	this->ShowWindow( false );
	//CDialog::OnCancel();
}

VOID CLoanCardFormatSetter::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

//	ResizeWindow();
	
	// Do not call CDialog::OnPaint() for painting messages
}

VOID CLoanCardFormatSetter::OnbINPUT() 
{
	// TODO: Add your control notification handler code here
	
	CCardFormatInfoDlg dlg( this );
	dlg.DoModal();
	
	/*
	CLoanCardInfoAdder dlg( this );
	dlg.DoModal();
	*/
}

VOID CLoanCardFormatSetter::GetFieldName( CString field[] )
{
	field[  0 ] = _T("REC_KEY");
	field[  1 ] = _T("ALIAS");
	field[  2 ] = _T("BOX_SIZE");
	field[  3 ] = _T("TYPE");
	field[  4 ] = _T("USE_YN");
	field[  5 ] = _T("FIX");
	field[  6 ] = _T("FONT");
	field[  7 ] = _T("FONT_SIZE");
	field[  8 ] = _T("FILL_COLOR");
	field[  9 ] = _T("CHAR_LEN");
	field[ 10 ] = _T("CHAR_MAX");
	field[ 11 ] = _T("HORZ_ALIGN");
	field[ 12 ] = _T("VERT_ALIGN");
	field[ 13 ] = _T("HORZ_LINE");
	field[ 14 ] = _T("VERT_LINE");
	field[ 15 ] = _T("CONTENT");
	field[ 16 ] = _T("FONT_BOLD");
}

VOID CLoanCardFormatSetter::GetFieldType( CString fieldType[] )
{
	fieldType[  0 ] = _T("NUMERIC");
	fieldType[  1 ] = _T("STRING");
	fieldType[  2 ] = _T("STRING");
	fieldType[  3 ] = _T("STRING");
	fieldType[  4 ] = _T("STRING");
	fieldType[  5 ] = _T("STRING");
	fieldType[  6 ] = _T("STRING");
	fieldType[  7 ] = _T("NUMERIC");
	fieldType[  8 ] = _T("STRING");
	fieldType[  9 ] = _T("NUMERIC");
	fieldType[ 10 ] = _T("NUMERIC");
	fieldType[ 11 ] = _T("STRING");
	fieldType[ 12 ] = _T("STRING");
	fieldType[ 13 ] = _T("STRING");
	fieldType[ 14 ] = _T("STRING");
	fieldType[ 15 ] = _T("STRING");
	fieldType[ 16 ] = _T("STRING");
}

VOID CLoanCardFormatSetter::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		ShowGrid();
}


VOID CLoanCardFormatSetter::OnbUPDATE() 
{
	// TODO: Add your control notification handler code here
	
	CCardFormatInfoDlg dlg( this );
	
	CArray< INT, INT >checkedRow;
	GetCheckedRow( dlg.m_checkedRow );

	if( !dlg.m_checkedRow.GetSize() )
	{
		AfxMessageBox( _T("수정할 항목을 선택하여 주십시요") );
		return;
	}

	dlg.m_inputMode = 2;
	dlg.DoModal();
}

VOID CLoanCardFormatSetter::GetCheckedRow( CArray< INT , INT >&checkedRow )
{
	checkedRow.RemoveAll();	
	INT rowCnt = FindDM( _T("DM_LoanCardFormatSetter") )->GetRowCount();
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	for( INT i = 0; i < rowCnt; i++ )
		if( _T("V") == pGrid->GetTextMatrix( i + 1, 1 ) )
			checkedRow.Add( i );
}

VOID CLoanCardFormatSetter::OnbDELETE() 
{
	// TODO: Add your control notification handler code here
	
	CArray< INT, INT >checkedRow;
	GetCheckedRow( checkedRow );
	INT cnt = checkedRow.GetSize();
	if( !cnt )
	{
		AfxMessageBox( _T("삭제할 항목을 선택하여 주십시요") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	CString query;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		query.Format( _T("DELETE FROM CO_LOANCARD_PRINT_INFO_TBL WHERE REC_KEY = %s;"), pDM->GetCellData( _T("REC_KEY"), checkedRow.GetAt( i ) ) );
		pDM->ExecuteQuery( query, true );
		pDM->DeleteRow( checkedRow.GetAt( i ) );
	}
	query.Format( _T("%d 건의 자료가 삭제되었습니다."), cnt );
	AfxMessageBox( query );
	ShowGrid();	
}

INT CLoanCardFormatSetter::PrintLoanCard( CESL_DataMgr *pDM )
{
	// DM : DM_BO_LOC_3395
	
	CLoanCardPrinter printer( this );
	printer.PrintLoanCard( pDM );

	return 0;
}

VOID CLoanCardFormatSetter::SaveLoanCardInfoData( CString data[], INT mode, INT index )
{
	const INT cnt = 17;
	
	CString field[ cnt ];
	GetFieldName( field );

	CString type[ cnt ];
	GetFieldType( type );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	pDM->StartFrame();
	pDM->InitDBFieldData();

	if( 2 != mode )
		pDM->MakeRecKey( data[ 0 ] );
	
	data[ 4 ] = _T("Y");
	for( INT i = 0; i < cnt; i++ )
		pDM->AddDBFieldData( field[ i ], type[ i ], data[ i ], true );

	CString sRank;
	if( 2 == mode )
	{
		pDM->MakeUpdateFrame( _T("CO_LOANCARD_PRINT_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], index );
		pDM->SetCellData( field, cnt, data, index );
	}
	else
	{
		sRank = GetObjectLastRank();
		pDM->AddDBFieldData( _T("RANK"), _T("NUMERIC"), sRank );
		pDM->AddDBFieldData( _T("PATTERN"), _T("STRING"), m_currentPtrn );

		pDM->MakeInsertFrame( _T("CO_LOANCARD_PRINT_INFO_TBL") );
		pDM->SetCellData( field, cnt, data, -1 );

		pDM->SetCellData( _T("RANK"), sRank, pDM->GetRowCount() - 1 );
		pDM->SetCellData( _T("PATTERN"), m_currentPtrn, pDM->GetRowCount() - 1 );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	if( 2 == mode )
		AfxMessageBox( _T("수정되었습니다.") );
	else
		AfxMessageBox( _T("저장되었습니다.") );

	ShowGrid();
}

/*
59
232
48
329

257
430
48
329

456
629
48
329

656
829
48
329

top
bottom
left
right
*/

VOID CLoanCardFormatSetter::OnbINITRANK() 
{
	// TODO: Add your control notification handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetTextMatrix( i, 2, _T("") );
		pDM->SetCellData( _T("RANK"), _T(""), i - 1 );
	}
	m_objRank = 0;
}

VOID CLoanCardFormatSetter::OnbSAVERANK() 
{
	// TODO: Add your control notification handler code here
	
	SaveObjectRank();	
}

BEGIN_EVENTSINK_MAP(CLoanCardFormatSetter, CDialog)
    //{{AFX_EVENTSINK_MAP(CLoanCardFormatSetter)
	ON_EVENT(CLoanCardFormatSetter, IDC_gMAIN_GRID, -600 /* Click */, OnClickgMAINGRID, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLoanCardFormatSetter::OnClickgMAINGRID() 
{
	// TODO: Add your control notification handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 2 != col )
		return;
	
	if( !pGrid->GetTextMatrix( row, col ).IsEmpty() )
		return;

	CString sRank;
	sRank.Format( _T("%d"), ++m_objRank );
	pGrid->SetTextMatrix( row, col, sRank );

	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	pDM->SetCellData( _T("RANK"), sRank, row - 1 );
}

VOID CLoanCardFormatSetter::SaveObjectRank()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );

	INT rowCnt = pDM->GetRowCount();
	if( m_objRank != rowCnt )
	{
		AfxMessageBox( _T("우선순위를 모두 지정하여 주십시요") );
		return;
	}
	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	CString query;
	for( INT i = 0; i < rowCnt; i++ )
	{
		query.Format( _T("UPDATE CO_LOANCARD_PRINT_INFO_TBL SET RANK = %s WHERE REC_KEY = %s;"), pGrid->GetTextMatrix( i + 1, 2 ), pDM->GetCellData( _T("REC_KEY"), i ) );
		pDM->AddFrame( query );
	}
	pDM->SendFrame();
	pDM->EndFrame();

	AfxMessageBox( _T("대출증 오브젝트 우선순위가 저장되었습니다.") );
	SetCurrentPtrn();
}

CString CLoanCardFormatSetter::GetObjectLastRank()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	INT rank = 0;
	INT DMRank = 0;

	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		DMRank = _ttoi( pDM->GetCellData( _T("RANK"), i ) );
		if( rank < DMRank )
			rank = DMRank;
	}
	
	CString sRank;
	sRank.Format( _T("%d"), rank + 1 );
	return sRank;
}

// DM_LoanCardFormatSetter_PtrnList

VOID CLoanCardFormatSetter::MakePtrnList()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter_PtrnList") );
	pDM->RefreshDataManager( _T("") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	pCombo->ResetContent();
	pCombo->InsertString( 0, _T("-선택-") );
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
		pCombo->InsertString( i + 1, pDM->GetCellData( _T("PATTERN"), i ) );
	pCombo->SetCurSel( 0 );
}

VOID CLoanCardFormatSetter::OnSelchangecPTRNLIST() 
{
	// TODO: Add your control notification handler code here
	
	SetCurrentPtrn();
}

VOID CLoanCardFormatSetter::SetCurrentPtrn()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter_PtrnList") );

	INT curSel = pCombo->GetCurSel();
	if( !curSel )
		m_currentPtrn.Empty();
	else
		m_currentPtrn = pDM->GetCellData( _T("PATTERN"), curSel - 1 );

	pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	CString where;
	where.Format( _T("PATTERN = '%s'"), m_currentPtrn );
	pDM->RefreshDataManager( where );
	ShowGrid();

	m_defaultPtrn = m_currentPtrn;
}

// 양식추가
VOID CLoanCardFormatSetter::OnbADDPTRN() 
{
	// TODO: Add your control notification handler code here
	
	CNewLoanCardFormatAdder dlg( this );
	if( IDOK == dlg.DoModal() )
		AddNewPattern( dlg.m_sPatternName, dlg.m_sCopyFromPtrn );
}

// 양식삭제
VOID CLoanCardFormatSetter::OnbDELETEPTRN()
{
	// TODO: Add your control notification handler code here
	
	if( _T("기본") == m_currentPtrn )
	{
		AfxMessageBox( _T("기본양식은 지울 수 없습니다.") );
		return;
	}
	
	CString query;
	query.Format( _T("대출증 양식 %s 를 삭제하시겠습니까?"), m_currentPtrn );
	if( IDYES != AfxMessageBox( query, MB_YESNO ) )
		return;

	query.Format( _T("DELETE FROM CO_LOANCARD_PRINT_INFO_TBL WHERE PATTERN = '%s';"), m_currentPtrn );

	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter_PtrnList") );
	pDM->ExecuteQuery( query, true );

	query.Format( _T("대출증 양식 %s 를 삭제하였습니다."), m_currentPtrn );
	AfxMessageBox( query );
	
	MakePtrnList();
}

INT CLoanCardFormatSetter::AddNewPattern( CString ptrnName, CString copyFromPtrn )
{
	CString query;
	query.Format( _T("INSERT INTO CO_LOANCARD_PRINT_INFO_TBL( REC_KEY, PATTERN, USE_YN ) VALUES( ESL_SEQ.NEXTVAL, '%s', 'N' );"), ptrnName );
		
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter_PtrnList") );
	pDM->ExecuteQuery( query, true );

	MakePtrnList();

	CopyPattern( copyFromPtrn, ptrnName );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	CString sBuf;
	for( INT i = 0; i < pCombo->GetCount(); i++ )
	{
		pCombo->SetCurSel( i );
		
		pCombo->GetWindowText( sBuf );
		if( sBuf == ptrnName )
			break;
		else
			pCombo->SetCurSel( 0 );
	}

	SetCurrentPtrn();
	AfxMessageBox( _T("대출증 양식을 추가하였습니다.") );

	return 0;
}

VOID CLoanCardFormatSetter::ShowGrid()
{
	ControlDisplay( _T("CM_LoanCardFormatSetter"), _T("GRID") );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID );
	CString sText;
	for( INT i = 0; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRowHeight( i, 500 );

		sText = pGrid->GetTextMatrix( i, 5 );
		if( i && !sText.IsEmpty() )
			switch( sText.GetAt( 0 ) )
			{
				case 'T' :
					pGrid->SetTextMatrix( i, 5, _T("TEXT BOX") );
					break;
				case 'B' :
					pGrid->SetTextMatrix( i, 5, _T("BARCODE") );
					break;
				default :
					break;
			}
	}
}

INT CLoanCardFormatSetter::GetPtrnList( CStringList &ptrnList )
{
	ptrnList.RemoveAll();

	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter_PtrnList") );
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
		ptrnList.AddTail( pDM->GetCellData( _T("PATTERN"), i ) );
	
	return 0;
}

INT CLoanCardFormatSetter::CopyPattern( CString from, CString to )
{
	if( from.IsEmpty() )
		return 0;
	
	CString query = _T("");
	/*▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒*/
	// (M_2004-1117_HAN) FIRST_WORK와 LAST_WORK 추가
	/*▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒*/
	query.Format(_T("INSERT INTO CO_LOANCARD_PRINT_INFO_TBL (")
							_T(" SELECT ESL_SEQ.NEXTVAL,")
								_T(" ALIAS,")
								_T(" BOX_SIZE,")
								_T(" TYPE,")
								_T(" USE_YN,")
								_T(" FIX,")
								_T(" CONTENT,")
								_T(" FILL_COLOR,")
								_T(" CHAR_LEN,")
								_T(" CHAR_MAX,")
								_T(" HORZ_ALIGN,")
								_T(" VERT_ALIGN,")
								_T(" HORZ_LINE,")
								_T(" VERT_LINE,")
								_T(" FONT,")
								_T(" FONT_SIZE,")
								_T(" RANK,")
								_T(" FONT_BOLD,")
								_T(" '%s',")
								_T(" '%s',")
								_T(" ''")
							_T(" FROM CO_LOANCARD_PRINT_INFO_TBL")
							_T(" WHERE PATTERN = '%s' );"),
					to, GetWorkLogMsg( _T("이용자관리"), __WFILE__, __LINE__ ), from );

	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	pDM->ExecuteQuery( query, true );
	
	return 0;
}

VOID CLoanCardFormatSetter::SetDefaultLoanCardType()
{
	CStdioFile fp;
	fp.Open( _T("..\\cfg\\DefaultLoanCardInfo.cfg"), CFile::modeRead | CFile::typeBinary );
	if( NULL == fp.m_pStream )
		return;
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
	
	CString string;
	while( fp.ReadString( string ) )
	{
		if( _T(".") == string.Left( 1 ) )
			continue;

		if( string.Find( _T("DEFAULT_PATTERN") ) > -1 )
		{
			string.Replace( _T(" "), _T("") );
			string.Replace( _T("DEFAULT_PATTERN:"), _T("") );
			m_defaultPtrn = string;
		}

		if( string.Find( _T("LOAN_CARD_TYPE") ) > -1 )
		{
			string.Replace( _T(" "), _T("") );
			string.Replace( _T("LOAN_CARD_TYPE:"), _T("") );
			m_defaultLoanCardType = string;
		}
	}

}

CString CLoanCardFormatSetter::GetDefaultPtrnName()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	if( pCombo->GetCurSel() )
		pCombo->GetWindowText( m_defaultPtrn );
	else
		m_defaultPtrn.Empty();

	return m_defaultPtrn;
}	

HBRUSH CLoanCardFormatSetter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
