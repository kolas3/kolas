// CardFormatInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CardFormatInfoDlg.h"
#include "LoanCardFormatSetter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardFormatInfoDlg dialog


CCardFormatInfoDlg::CCardFormatInfoDlg( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CCardFormatInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// 2009.04.21 ADD BY CJY : 리치에디트 컨트롤 초기화
	AfxInitRichEdit();

	m_index = 0;
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CCardFormatInfoDlg::~CCardFormatInfoDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

BOOL CCardFormatInfoDlg::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	
	
VOID CCardFormatInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardFormatInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardFormatInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCardFormatInfoDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO15, OnRadio15)
	ON_BN_CLICKED(IDC_RADIO16, OnRadio16)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bSETFONT, OnbSETFONT)
	ON_BN_CLICKED(IDC_bCOLOR, OnbCOLOR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardFormatInfoDlg message handlers

VOID CCardFormatInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT cnt = 17;
	CString data[ cnt ];
	GetCtrlData( data );

	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )pParentMgr;
	pParent->SaveLoanCardInfoData( data, m_inputMode, ( 2 == m_inputMode ? m_checkedRow.GetAt( m_index ) : 0 ) );
	
	//CDialog::OnOK();
}

INT CCardFormatInfoDlg::GetCtrlData( CString data[] )
{
	CString sBuf;
	
	// REC_KEY
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	if( 2 == m_inputMode )
		data[ 0 ] = pDM->GetCellData( _T("REC_KEY"), m_checkedRow.GetAt( m_index ) );
	else
		data[ 0 ] = _T("");
	
	// ALIAS
	GetDlgItemText( IDC_EDIT1, data[ 1 ] );
	// SIZE
	data[ 2 ] = GetBoxSize();
	//GetDlgItemText( IDC_EDIT2, data[ 2 ] );
	
	// TYPE
	if( GetType( sBuf ) )
		return -1;
	data[ 3 ] = sBuf;

	// USE_YN
	if( GetYNType( IDC_RADIO9, IDC_RADIO10, sBuf ) )
		return -2;
	data[ 4 ] = sBuf;

	// FIX
	if( GetYNType( IDC_RADIO15, IDC_RADIO16, sBuf ) )
		return -3;
	data[ 5 ] = sBuf;

	// FONT
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cFONT_COMBO );
	if( !pCombo->GetCurSel() )
		return -4;
	pCombo->GetWindowText( data[ 6 ] );
	//GetDlgItemText( IDC_EDIT6,  data[ 6 ] );
	
	// FONT SIZE
	GetDlgItemText( IDC_EDIT10, data[ 7 ] );
	// FILL COLOR
	GetDlgItemText( IDC_EDIT12, data[ 8 ] );
	// TCHAR LEN
	GetDlgItemText( IDC_EDIT13, data[ 9 ] );
	// CHAR_MAX
	GetDlgItemText( IDC_EDIT13, data[ 10 ] );

	if( GetAlignType( 0, sBuf ) )
		return -5;
	data[ 11 ] = sBuf;

	if( GetAlignType( 1, sBuf ) )
		return -6;
	data[ 12 ] = sBuf;

	if( GetYNType( IDC_RADIO17, IDC_RADIO18, sBuf ) )
		return -7;
	data[ 13 ] = sBuf;

	if( GetYNType( IDC_RADIO19, IDC_RADIO20, sBuf ) )
		return -8;
	data[ 14 ] = sBuf;

	if( GetContent( data[ 5 ], sBuf ) )
		return -9;
	data[ 15 ] = sBuf;

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_FONT_BOLD );
	if( pBtn->GetCheck() )
		data[ 16 ] = _T("Y");
	else
		data[ 16 ] = _T("N");

	return 0;
}

VOID CCardFormatInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow && 2 == m_inputMode )
		Display( 0 );
}

VOID CCardFormatInfoDlg::Display( INT flag )
{
	m_index += flag;
	
	const INT cnt = 17;
	CString data[ cnt ];
	GetDMData( data );
	
	ViewCardInfo( data );
	
	/*
	UINT id[ 15 ] = {
		IDC_EDIT1,
		IDC_EDIT2,
		IDC_EDIT3,
		IDC_EDIT4,
		IDC_EDIT5,
		IDC_EDIT6,
		IDC_EDIT10,
		IDC_EDIT11,
		IDC_EDIT12,
		IDC_EDIT13,
		IDC_EDIT14,
		IDC_EDIT7,
		IDC_EDIT15,
		IDC_EDIT16,
		IDC_EDIT8 };

	for( INT i = 0; i < 15; i++ )
		SetDlgItemText( id[ i ], data[ i + 1 ] );
	*/

	CESL_ControlMgr *pCM = FindCM( _T("CM_LoanCardFormatSetter") );
	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( _T("GRID") ) );
	pGrid->SetReverse( m_checkedRow.GetAt( m_index ) );
	
	ShowMoveBtns();
}

VOID CCardFormatInfoDlg::GetDMData( CString data[] )
{
	const INT cnt = 17;
	CString fieldAlias[ cnt ];
	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )pParentMgr;
	pParent->GetFieldName( fieldAlias );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_LoanCardFormatSetter") );
	pDM->GetCellData( fieldAlias, cnt, data, m_checkedRow.GetAt( m_index ) );
}

BOOL CCardFormatInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here

	CButton *pBtn = NULL;
	if( 2 != m_inputMode )
	{
		pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
		pBtn->ShowWindow( false );
		pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
		pBtn->ShowWindow( false );

		this->SetWindowText( _T("입력") );

		( CWnd * )GetDlgItem( IDC_RICHEDIT2 )->ShowWindow( false );
		//( CWnd * )GetDlgItem( IDC_EDIT8 )->ShowWindow( false );
		( CWnd * )GetDlgItem( IDC_COMBO1 )->ShowWindow( false );
	}
	else
		this->SetWindowText( _T("수정") );
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	const INT cnt = 9;
	CString valueList[ cnt ] = {	
								_T("자택주소"),
								_T("근무지주소"),
								_T("생일"),
								_T("주민등록번호"),
								_T("대출자이름"),
								_T("회원구분"),
								_T("등록일"),
								_T("대출자ID"),
								_T("대출자번호") };
	pCombo->InsertString( 0, _T("- 선택 -") );
	for( INT i = 0; i < cnt; i++ )
		pCombo->InsertString( i + 1, valueList[ i ] );
	pCombo->SetCurSel( 0 );
	
	MakeAlignCombo();
	MakeFontCombo();


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCardFormatInfoDlg::ShowMoveBtns()
{
	CButton *pBtn = NULL;

	pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
	if( !m_index )
		pBtn->EnableWindow( false );
	else
		pBtn->EnableWindow( true );

	pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
	if( m_index == m_checkedRow.GetSize() - 1 )
		pBtn->EnableWindow( false );
	else
		pBtn->EnableWindow( true );
}

VOID CCardFormatInfoDlg::OnRadio15() 
{
	// TODO: Add your control notification handler code here
	
	ViewComboValueList();
}

VOID CCardFormatInfoDlg::OnRadio16() 
{
	// TODO: Add your control notification handler code here
	
	ViewComboValueList();
}

VOID CCardFormatInfoDlg::ViewComboValueList()
{
	CRichEditCtrl *pEdt = ( CRichEditCtrl * )GetDlgItem( IDC_RICHEDIT2 );
	pEdt->ShowWindow( false );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->ShowWindow( false );
	
	CButton *pC1 = ( CButton * )GetDlgItem( IDC_RADIO15 );
	CButton *pC2 = ( CButton * )GetDlgItem( IDC_RADIO16 );

	if( pC1->GetCheck() )
		pEdt->ShowWindow( true );

	if( pC2->GetCheck() )
		pCombo->ShowWindow( true );
}

VOID CCardFormatInfoDlg::MakeAlignCombo()
{
	CComboBox *pCombo = NULL;

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("왼쪽") );
	pCombo->InsertString( 1, _T("중앙") );
	pCombo->InsertString( 2, _T("오른쪽") );
	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	pCombo->InsertString( 0, _T("위쪽") );
	pCombo->InsertString( 1, _T("중앙") );
	pCombo->InsertString( 2, _T("아래쪽") );
	pCombo->SetCurSel( 0 );
}

INT CCardFormatInfoDlg::GetType( CString &data )
{
	CButton *pRadio = ( CButton * )GetDlgItem( IDC_RADIO7 );
	if( pRadio->GetCheck() )
	{
		data = _T("T");
		return 0;
	}
	
	pRadio = ( CButton * )GetDlgItem( IDC_RADIO8 );
	if( pRadio->GetCheck() )
	{
		data = _T("B");
		return 0;
	}

	return -1;
}

INT CCardFormatInfoDlg::GetYNType( UINT yID, UINT nID, CString &data )
{
	CButton *pRadio = ( CButton * )GetDlgItem( yID );
	if( pRadio->GetCheck() )
	{
		data = _T("Y");
		return 0;
	}
	
	pRadio = ( CButton * )GetDlgItem( nID );
	if( pRadio->GetCheck() )
	{
		data = _T("N");
		return 0;
	}

	return 0;
}

INT CCardFormatInfoDlg::GetAlignType( INT opt, CString &data )
{
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_COMBO3, IDC_COMBO4 };

	CString value[ 3 ] = {
		_T("왼쪽"), _T("중앙"), _T("오른쪽") };
	if( opt )
	{
		value[ 0 ] = _T("위쪽");
		value[ 2 ] = _T("아래쪽");
	}

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( id[ opt ] );
	data = value[ pCombo->GetCurSel() ];
		
	return 0;
}

INT CCardFormatInfoDlg::GetContent( CString type, CString &data )
{
	//CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT8 );
	CRichEditCtrl *pEdt = ( CRichEditCtrl * )GetDlgItem( IDC_RICHEDIT2 );
	if( _T("Y") == type )
	{
		pEdt->GetWindowText( data );
		return 0;
	}
	
	if( _T("N") == type )
	{
		data = GetComboListValue();
		data.Replace( _T(" "), _T("") );
		return 0;
	}
	
	return -1;			
}

CString CCardFormatInfoDlg::GetComboListValue()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	if( !pCombo->GetCurSel() )
		return _T("");

	CString value;
	pCombo->GetWindowText( value );
	return value;
}

VOID CCardFormatInfoDlg::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here

	Display( -1 );
}

VOID CCardFormatInfoDlg::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	Display( 1 );
}

VOID CCardFormatInfoDlg::ViewCardInfo( CString data[] )
{
	// 16

	SetDlgItemText( IDC_EDIT1, data[ 1 ] );
	
	ViewBoxSize( data[ 2 ] );
	//SetDlgItemText( IDC_EDIT2, data[ 2 ] );

	ShowType( data[ 3 ] );
	ShowYNType( IDC_RADIO9, IDC_RADIO10, data[ 4 ] );
	ShowYNType( IDC_RADIO15, IDC_RADIO16, data[ 5 ] );

	CWnd *pW1 = ( CWnd * )GetDlgItem( IDC_RICHEDIT2 );
	pW1->ShowWindow( false );
	CWnd *pW2 = ( CWnd * )GetDlgItem( IDC_COMBO1 );
	pW2->ShowWindow( false );
	if( _T("Y") == data[ 5 ] )
		pW1->ShowWindow( true );
	else if( _T("N") == data[ 5 ] )
		pW2->ShowWindow( true );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cFONT_COMBO );
	CString sBuf;
	for( INT i = 0; i < pCombo->GetCount(); i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( sBuf );

		if( sBuf == data[ 6 ] )
			break;
		else
			pCombo->SetCurSel( 0 );
	}
	//SetDlgItemText( IDC_EDIT6,  data[ 6 ] );


	SetDlgItemText( IDC_EDIT10, data[ 7 ] );
	SetDlgItemText( IDC_EDIT11, data[ 8 ] );
	SetDlgItemText( IDC_EDIT12, data[ 9 ] );
	SetDlgItemText( IDC_EDIT13, data[ 10 ] );

	SetAlignType( 0, data[ 11 ] );
	SetAlignType( 1, data[ 12 ] );

	ShowYNType( IDC_RADIO17, IDC_RADIO18, data[ 13 ] );
	ShowYNType( IDC_RADIO19, IDC_RADIO20, data[ 14 ] );

	ShowContents( data[ 5 ], data[ 15 ] );

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_FONT_BOLD );
	if( _T("Y") == data[ 16 ] )
		pBtn->SetCheck( true );
	else
		pBtn->SetCheck( false );
}

VOID CCardFormatInfoDlg::ShowType( CString type )
{
	CButton *pRadio = ( CButton * )GetDlgItem( IDC_RADIO7 );
	pRadio->SetCheck( false );
	if( _T("T") == type )
		pRadio->SetCheck( true );
	
	pRadio = ( CButton * )GetDlgItem( IDC_RADIO8 );
	pRadio->SetCheck( false );
	if( _T("B") == type )
		pRadio->SetCheck( true );
}

VOID CCardFormatInfoDlg::ShowYNType( UINT yID, UINT nID, CString type )
{
	CButton *pRadio = ( CButton * )GetDlgItem( yID );
	pRadio->SetCheck( false );
	if( _T("Y") == type )
		pRadio->SetCheck( true );
	
	pRadio = ( CButton * )GetDlgItem( nID );
	pRadio->SetCheck( false );
	if( _T("N") == type )
		pRadio->SetCheck( true );	
}

VOID CCardFormatInfoDlg::SetAlignType( INT opt, CString type )
{
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_COMBO3, IDC_COMBO4 };
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( id[ opt ] );

	CString sBuf;
	for( INT i = 0; i < pCombo->GetCount(); i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( sBuf );

		if( sBuf == type )
			break;
	}
}

VOID CCardFormatInfoDlg::ShowContents( CString type, CString data )
{
	//CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT8 );
	CRichEditCtrl *pEdt = ( CRichEditCtrl * )GetDlgItem( IDC_RICHEDIT2 );
	if( _T("Y") == type )
	{
		pEdt->SetWindowText( data );
		return;
	}

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->SetCurSel( 0 );
	CString sBuf;
	for( INT i = 0; i < pCombo->GetCount(); i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( sBuf );

		if( sBuf == data )
			break;
	}
}

VOID CCardFormatInfoDlg::OnbSETFONT() 
{
	// TODO: Add your control notification handler code here
	
	LOGFONT longFont;
	CString fontName, size;

	GetDlgItemText( IDC_EDIT6,  fontName  );
	GetDlgItemText( IDC_EDIT10, size );
	size = fontName.Right( fontName.GetLength() - fontName.Find( _T(",") ) - 1 );
	fontName = fontName.Left( fontName.Find( _T(",") ) );
		
	_tcscpy( longFont.lfFaceName, fontName );

	CFontDialog dlg( &longFont );
	if( IDOK != dlg.DoModal() )
		return;

	dlg.GetCurrentFont( &longFont );
	fontName.Format( _T("%s"), longFont.lfFaceName );
	size.Format( _T("%d"), dlg.GetSize() / 10 );

	SetDlgItemText( IDC_EDIT6, fontName );
	SetDlgItemText( IDC_EDIT10, size );
}

VOID CCardFormatInfoDlg::OnbCOLOR() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CCardFormatInfoDlg::MakeFontCombo()
{
	const INT cnt = 8;
	CString list[ cnt ] = {
		_T("굴림"), _T("굴림체"), 
		_T("궁서"), _T("궁서체"), 
		_T("돋움"), _T("돋움체"), 
		_T("바탕"), _T("바탕체") };

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cFONT_COMBO );
	pCombo->InsertString( 0, _T("-선택-") );
	for( INT i = 0; i < cnt; i++ )
		pCombo->InsertString( i + 1, list[ i ] );
	pCombo->SetCurSel( 1 );

	SetDlgItemText( IDC_EDIT10, _T("10") );
}

VOID CCardFormatInfoDlg::ViewBoxSize( CString size )
{
	const INT cnt = 4;
	INT nSize[ cnt ];
	_stscanf( size.GetBuffer( 0 ), _T("%d,%d,%d,%d"), &nSize[ 0 ], &nSize[ 1 ], &nSize[ 2 ], &nSize[ 3 ] );

	UINT id[ cnt ] = {
		IDC_EDIT4, IDC_EDIT5, IDC_EDIT14, IDC_EDIT7 };
	CString sSize;
	for( INT i = 0; i < cnt; i++ )
	{
		sSize.Format( _T("%d"), nSize[ i ] );
		SetDlgItemText( id[ i ], sSize );
	}
}

CString CCardFormatInfoDlg::GetBoxSize()
{
	const INT cnt = 4;
	CString sSize[ cnt ];
	UINT id[ cnt ] = {
		IDC_EDIT4, IDC_EDIT5, IDC_EDIT14, IDC_EDIT7 };

	for( INT i = 0; i < cnt;i++ )
		GetDlgItemText( id[ i ], sSize[ i ] );

	CString size;
	size.Format( _T("%s,%s,%s,%s"), sSize[ 0 ], sSize[ 1 ], sSize[ 2 ], sSize[ 3 ] );
	return size;
}

HBRUSH CCardFormatInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
