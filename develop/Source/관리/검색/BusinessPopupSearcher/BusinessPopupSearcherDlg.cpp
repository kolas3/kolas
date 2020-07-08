// BusinessPopupSearcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BusinessPopupSearcherDlg.h"

#include "..\..\분류기호\MNG_CLASS_SIGN_MINI_POPUP\MNG_CLASS_SIGN_MINI_POPUP_01.h"
#include "..\..\전거\ACMarcLittleGetter\ACMarcLittleGetter_01.h"
#include "..\..\Komarc\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBusinessPopupSearcherDlg dialog


CBusinessPopupSearcherDlg::CBusinessPopupSearcherDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBusinessPopupSearcherDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CBusinessPopupSearcherDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CBusinessPopupSearcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBusinessPopupSearcherDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBusinessPopupSearcherDlg, CDialog)
	//{{AFX_MSG_MAP(CBusinessPopupSearcherDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bDETAIL, OnbDETAIL)
	ON_BN_CLICKED(IDC_bDETAIL2, OnbDETAIL2)
	ON_BN_CLICKED(IDC_bCLEAR2, OnbCLEAR2)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDCANCEL2, OnCancel2)
	ON_BN_CLICKED(IDC_bCLEAR, OnbCLEAR)
	ON_CBN_SELCHANGE(IDC_COMBO5, OnSelchangeCombo5)
	ON_BN_CLICKED(IDC_ACBUTTON_1, OnAcbutton1)
	ON_BN_CLICKED(IDC_ACBUTTON_2, OnAcbutton2)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_UNV, OnButtonUnv)
	ON_BN_CLICKED(IDC_BUTTON_GOV, OnButtonGov)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBusinessPopupSearcherDlg message handlers

BOOL CBusinessPopupSearcherDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("BusinessPopupSearcherDlg") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - BusinessPopupSearcherDlg") );
		return false;
	}

	m_detailView = true;
	OnbDETAIL();
	InitCtrlSetting();	

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBusinessPopupSearcherDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->SetCurSel( m_type );

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eTITLE );
	pEdt->SetWindowText( m_title );
}

VOID CBusinessPopupSearcherDlg::InitCtrlSetting()
{
	CComboBox *pCombo;
	INT i;
	
	CString comboList[ 12 ] = {
		_T("전체"), _T("일반도서"), _T("학위도서"), _T("교과서"), _T("학습서"), _T("만화"), _T("아동/청소년도서"), _T("잡지"), _T("신문"), _T("비도서"), _T("점자자료"), _T("고서") };
	/*
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	for( i = 0; i < 12; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );
	*/

	comboList[ 0 ] = _T("전 체");
	comboList[ 1 ] = _T("단 행");
	comboList[ 2 ] = _T("연 속");
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	for( i = 0; i < 3; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );

	comboList[ 0 ] = _T("AND");
	comboList[ 1 ] = _T("OR");
	pCombo = ( CComboBox *)GetDlgItem( IDC_COMBO12 );
	for( i = 0; i < 2; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO13 );
	for( i = 0; i < 2; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );

	comboList[ 0 ] = _T("KDC");
	comboList[ 1 ] = _T("DDC");
	comboList[ 2 ] = _T("LC");
	comboList[ 3 ] = _T("UDC");
	comboList[ 4 ] = _T("ICS");
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	for( i = 0; i < 5; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );

	comboList[ 0 ] = _T("청구기호");
	comboList[ 1 ] = _T("제어번호");
	comboList[ 2 ] = _T("");
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO5 );
	for( i = 0; i < 3; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 2 );
	
	comboList[ 0 ] = _T("ISBN");
	comboList[ 1 ] = _T("ISSN");
	comboList[ 2 ] = _T("CODEN");
	comboList[ 3 ] = _T("STRN");
	comboList[ 4 ] = _T("RNSTRN");
	comboList[ 5 ] = _T("국가서지번호");
	comboList[ 6 ] = _T("저작권등록번호");
	comboList[ 7 ] = _T("");
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	for( i = 0; i < 8; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 7 );

	comboList[ 0 ] = _T("석사");
	comboList[ 1 ] = _T("박사");
	comboList[ 2 ] = _T("");
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO9 );
	for( i = 0; i < 3; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 2 );
	
	OnSelchangeCombo5();
}

// CM_BusinessPopupSearcherDlg
// DM_BusinessPopupSearcherDlg

VOID CBusinessPopupSearcherDlg::OnbDETAIL() 
{
	// TODO: Add your control notification handler code here
	
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_bDETAIL );
	
	if( m_detailView )
	{
		ModifyScreen( -1 );
		m_detailView = false;
	}
	else
	{
		ModifyScreen( 1 );
		m_detailView = true;
	}
}

VOID CBusinessPopupSearcherDlg::OnbDETAIL2() 
{
	// TODO: Add your control notification handler code here
	
	OnbDETAIL();
}

VOID CBusinessPopupSearcherDlg::OnbCLEAR2() 
{
	// TODO: Add your control notification handler code here
	
	OnbCLEAR();
}

VOID CBusinessPopupSearcherDlg::OnOk2() 
{
	// TODO: Add your control notification handler code here
	
	OnOK();
}

VOID CBusinessPopupSearcherDlg::OnCancel2() 
{
	// TODO: Add your control notification handler code here
	
	OnCancel();
}

VOID CBusinessPopupSearcherDlg::OnbCLEAR() 
{
	// TODO: Add your control notification handler code here

	CWnd *wnd;
	const INT wndCnt = 21;
	UINT wndID[ wndCnt ] = {
		IDC_eTITLE,  IDC_KEYWORD1,  IDC_KEYWORD2,  IDC_KEYWORD3,  IDC_EDIT5,  
		IDC_EDIT6,  IDC_EDIT8,  IDC_EDIT7,  IDC_EDIT9,  IDC_EDIT15, 
		IDC_EDIT16, IDC_EDIT17, IDC_EDIT18, IDC_EDIT19, IDC_EDIT20, 
		IDC_EDIT21, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT13, IDC_EDIT14 };

	for( INT i = 0; i < wndCnt; i++ )
	{
		wnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		wnd->SetWindowText( _T("") );
	}
}

VOID CBusinessPopupSearcherDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CString where;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	switch( pCombo->GetCurSel() )
	{
		case 0 :
			break;
		case 1 :
			where = MakeBOSelectQuery() + _T(" B.SPECIES_KEY = I.REC_KEY");
			break;
		case 2 :
			break;
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BusinessPopupSearcherDlg") );
	pDM->RefreshDataManager( where );

	m_cnt = pDM->GetRowCount();
	m_PK = new CString[ m_cnt ];
	for( INT i = 0; i < m_cnt; i++ )
		pDM->GetCellData( _T("REC_KEY"), i, m_PK[ i ] );

	CDialog::OnOK();
}

VOID CBusinessPopupSearcherDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CBusinessPopupSearcherDlg::OnSelchangeCombo5() 
{
	// TODO: Add your control notification handler code here

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO5 );

	UINT editID[ 5 ] = { 
		IDC_EDIT17, IDC_EDIT18, IDC_EDIT19, IDC_EDIT20, IDC_EDIT21 };
	bool showFlag[ 5 ] = { true, true, true, true, true };

	if( !pCombo->GetCurSel() )
		showFlag[ 0 ] = false;
	else
	{
		showFlag[ 1 ] = false;
		showFlag[ 2 ] = false;
		showFlag[ 3 ] = false;
		showFlag[ 4 ] = false;
	}

	CEdit *pEdt;
	for( INT i = 0; i < 5; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( editID[ i ] );
		pEdt->ShowWindow( showFlag[ i ] );
	}
}

INT CBusinessPopupSearcherDlg::ModifyScreen( INT flag )
{
	// flag   1 : 크게
	// flag  -1 : 작게
	
	CRect rect;
	this->GetWindowRect( rect );
	
	if( 1 == flag )
		rect.bottom += 245;
	else
		rect.bottom -= 245;

	this->MoveWindow( rect );		

	CWnd *pWnd;
	UINT wndID[ 4 ] = {
		IDC_STATIC_1, IDC_STATIC_2, IDC_COMBO4, IDC_EDIT16 };
	for( INT i = 0; i < 4; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );

		if( 1 == flag )
			pWnd->ShowWindow( true );
		else
			pWnd->ShowWindow( false );
	}
	
	CButton *pBtn;
	UINT btnID[ 4 ] = {
		IDC_bCLEAR2, IDC_bDETAIL2, IDOK2, IDCANCEL2 };
	
	for( i = 0; i < 4; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( btnID[ i ] );
		
		if( 1 == flag )
			pBtn->ShowWindow( false );
		else
			pBtn->ShowWindow( true );
	}	
	
	return 0;
}

INT CBusinessPopupSearcherDlg::SetConditionInfo( INT type, CString title )
{
	CString notice;
	if( type < 1 || type > 3 )
	{
		notice.Format( _T("Worng Type : %d"), type );
		AfxMessageBox( notice );
		return -1;
	}

	m_type = type - 1;
	m_title = title;

	return 0;
}

// 표제 전거
VOID CBusinessPopupSearcherDlg::OnAcbutton1() 
{
	// TODO: Add your control notification handler code here

	CString keyword, ACKeyword;
	CEdit *pEdt = pEdt = ( CEdit * )GetDlgItem( IDC_eTITLE );
	pEdt->GetWindowText( keyword );
	keyword.TrimLeft(); keyword.TrimRight();
	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("표제를 입력하여 주십시오") );
		return;
	}

	if( GetACKeyword( 2, keyword, ACKeyword ) )
		return;
	else
		pEdt->SetWindowText( keyword + _T(" ") + ACKeyword );	
}

// 저자명 전거
VOID CBusinessPopupSearcherDlg::OnAcbutton2() 
{
	// TODO: Add your control notification handler code here
	CString keyword, ACKeyword;
	CEdit *pEdt = pEdt = ( CEdit * )GetDlgItem( IDC_EDIT5 );
	pEdt->GetWindowText( keyword );
	keyword.TrimLeft(); keyword.TrimRight();
	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("저자명을 입력하여 주십시오") );
		return;
	}

	if( GetACKeyword( 1, keyword, ACKeyword ) )
		return;
	else
		pEdt->SetWindowText( keyword + ACKeyword );		
}

VOID CBusinessPopupSearcherDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT15 );
	
	CMNG_CLASS_SIGN_MINI_POPUP_01 dlg( this );
	if( IDOK == dlg.DoModal() )
		pEdt->SetWindowText( dlg.m_sign );		
}

INT CBusinessPopupSearcherDlg::GetACKeyword(INT option, CString keyword, CString &AC_Keyword)
{
	CACMarcLittleGetter_01 dlg( this );
	dlg.m_ACType = option;
	dlg.m_keyWord = keyword;
	if( IDOK == dlg.DoModal() )
		AC_Keyword = dlg.m_returnValue;
	else
		return -1;
	
	return 0;
}

CString CBusinessPopupSearcherDlg::MakeBOSelectQuery()
{
	CString where;
	if( false == m_detailView )
		where = 
			GetTitleQuery()    + GetKeywordQuery()   + GetAuthorQuery() + 
			GetRegNoQuery()    + GetPublisherQuery() + GetClassSignQuery();
	else
		where = 
			GetTitleQuery()    + GetKeywordQuery()   + GetAuthorQuery() + 
			GetRegNoQuery()    + GetPublisherQuery() + GetClassSignQuery() + 
			GetSTSignQuery()   + GetEXSignQuery()    + GetUnivCodeQuery() + 
			GetGovCodeQuery()  + GetDegCodeQuery()   + GetUserLimitQuery() +
			GetUseObjQuery()   + GetMediaCodeQuery() + GetMATCodeQuery() +
			GetTextLangQuery() + GetSumLangQuery()   + GetPlaceInfoQuery();
		
	return where;
}

CString CBusinessPopupSearcherDlg::GetTitleQuery()
{
	CString where, tmp;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eTITLE );
	pEdt->GetWindowText( tmp );

	tmp.TrimLeft(); tmp.TrimRight(); 
	tmp.Replace( _T(" "), _T("") );
	if( !tmp.GetLength() )
		return _T("");
	else
		where.Format( _T(" CATSEARCH( I.IDX_TITLE, '%s', NULL ) > 0 AND"), tmp );

	return where;
}

CString CBusinessPopupSearcherDlg::GetKeywordQuery()
{
	CString where;
	
	CEdit *pEdt;
	const INT edtCnt = 3;
	CString keyword[ edtCnt ];
	UINT edtID[ edtCnt ] = { IDC_KEYWORD1, IDC_KEYWORD2, IDC_KEYWORD3 };
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( keyword[ i ] );
		
		keyword[ i ].TrimLeft(); keyword[ i ].TrimRight();
		if( keyword[ i ].GetLength() )
			keyword[ i ] = _T(" CATSEARCH( I.IDX_KEYWORD, '") + keyword[ i ] + _T("', NULL ) > 0");
	}

	CComboBox *pCombo;
	const INT comboCnt = 2;
	CString comboType[ comboCnt ];
	UINT comboID[ comboCnt ] = { IDC_COMBO12 , IDC_COMBO13 };
	for( i = 0; i < comboCnt; i++ )
	{
		pCombo = ( CComboBox * )GetDlgItem( comboID[ i ] );
		if( pCombo->GetCurSel() )
			comboType[ i ] = _T(" OR ");
		else
			comboType[ i ] = _T(" AND ");
	}

	if( keyword[ 0 ].GetLength() )
		if( keyword[ 1 ].GetLength() )
			where = _T(" ( ") + keyword[ 0 ];
		else
			where = keyword[ 0 ];

	if( keyword[ 1 ].GetLength() )
		if( keyword[ 0 ].GetLength() )
			where = where + comboType[ 0 ] + keyword[ 1 ] + _T(" ) ");
		else
			where = where + comboType[ 0 ] + keyword[ 1 ];

	if( keyword[ 2 ].GetLength() )
		where = where + comboType[ 1 ] + keyword[ 2 ];
		
	if( where.GetLength() )
		return _T(" ( ") + where + _T(" ) AND");

	return _T("");
}

CString CBusinessPopupSearcherDlg::GetAuthorQuery()
{
	CString query, tmp;

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT5 );
	pEdt->GetWindowText( tmp );

	tmp.TrimLeft(); tmp.TrimRight();
	tmp.Replace( _T(" "), _T("") );
	if( tmp.GetLength() )
		query.Format( _T(" CATSEARCH( I.IDX_AUTHOR, '%s', NULL ) > 0 AND"), tmp );
	
	return query;
}

CString CBusinessPopupSearcherDlg::GetRegNoQuery()
{
	CString query, tmp, value;

	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT8 );
	pEdt->GetWindowText( value );
	if( value.GetLength() )
	{
		tmp.Format( _T(" I.PUB_YEAR = '%s' AND"), value );
		//tmp.Format( _T(" CATSEARCH( I.PUB_YEAR, '%s', NULL ) > 0 AND"), value );
		query += tmp;
	}

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT7 );
	pEdt->GetWindowText( value );
	if( value.GetLength() )
	{
		tmp.Format( _T(" B.REG_NO >= '%s' AND"), value );
		query += tmp;
	}

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT9 );
	pEdt->GetWindowText( value );
	if( value.GetLength() )
	{
		tmp.Format( _T(" B.REG_NO <= '%s' AND"), value );
		query += tmp;
	}

	return query;
}

CString CBusinessPopupSearcherDlg::GetPublisherQuery()
{
	CString query, tmp;

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT6 );
	pEdt->GetWindowText( tmp );

	tmp.TrimLeft(); tmp.TrimRight();
	tmp.Replace( _T(" "), _T("") );
	if( tmp.GetLength() )
		query.Format( _T(" CATSEARCH( I.IDX_PUBLISHER, '%s', NULL ) > 0 AND"), tmp );
	
	return query;
}

CString CBusinessPopupSearcherDlg::GetClassSignQuery()
{
	CString query;

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	CString field;
	switch( pCombo->GetCurSel() )
	{
		case 0 :
			field = _T(" I.KDC_CLASS ");
			break;
		case 1 :
			field = _T(" I.DDC_CLASS ");
			break;
		case 2 :
			field = _T(" I.LC_CLASS ");
			break;
		case 3 :
			field = _T(" I.UDC_CLASS ");
			break;
		case 4 :
			// ICS
			break;
		default :
			return _T("");
		}

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT15 );
	CString sign;
	pEdt->GetWindowText( sign );

	if( sign.GetLength() )
		query.Format( _T(" CATSEARCH( %s, '%s', NULL ) > 0 AND"), field, sign );

	return query;
}

CString CBusinessPopupSearcherDlg::GetSTSignQuery()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	CString field;
	switch( pCombo->GetCurSel() )
	{
		case 0 :
			field = _T(" I.ST_CODE ");
			break;
		case 1 :
			field = _T(" I.ST_ISSN ");
			break;
		case 2 :
			// CODEN 
			return _T("");
			break;
		case 3 :
			field = _T(" I.ST_STRN ");
			break;
		case 4 :
			field = _T(" I.ST_RNSTRN ");
			break;
		case 5 :
			field = _T(" I.ST_CBN ");
			break;
		case 6 :
			field = _T(" I.ST_CAN ");
			break;
		default :
			return _T("");
		}

	CString query;

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT16 );
	CString code;
	pEdt->GetWindowText( code );

	if( code.GetLength() )
		query.Format( _T(" %s = '%s' AND"), field, code );
		//query.Format( _T(" CATSEARCH( %s, '%s', NULL ) > 0 AND"), field, code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetEXSignQuery()
{
	CString query, tmp;

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO5 );
	INT curSel = pCombo->GetCurSel();

	CEdit *pEdt;
	CString code;

	if( 0 == curSel )
	{
		pEdt = ( CEdit * )GetDlgItem( IDC_EDIT18 );
		pEdt->GetWindowText( code );
		if( code.GetLength() )
		{
			tmp.Format( _T(" I.SE_SHELF_CODE = '%s' AND"), code );
			query += tmp;
		}

		pEdt = ( CEdit * )GetDlgItem( IDC_EDIT19 );
		pEdt->GetWindowText( code );
		if( code.GetLength() )
		{
			tmp.Format( _T(" I.CLASS_NO = '%s' AND"), code );
			query += tmp;
		}

		pEdt = ( CEdit * )GetDlgItem( IDC_EDIT20 );
		pEdt->GetWindowText( code );
		if( code.GetLength() )
		{
			tmp.Format( _T(" I.BOOK_CODE = '%s' AND"), code );
			query += tmp;
		}

		pEdt = ( CEdit * )GetDlgItem( IDC_EDIT21 );
		pEdt->GetWindowText( code );
		if( code.GetLength() )
		{
			tmp.Format( _T(" I.VOL_CODE = '%s' AND"), code );
			query += tmp;
		}
		
	}
	else if( 1 == curSel )
	{
		pEdt = ( CEdit * )GetDlgItem( IDC_EDIT17 );
		pEdt->GetWindowText( code );
		if( !code.GetLength() )
			return _T("");
		else
			query.Format( _T(" I.CONTROL_NO = '%s' AND"), code );
	}
	else
		return _T("");
	

	return query;
}

CString CBusinessPopupSearcherDlg::GetUnivCodeQuery()
{
	CString query, code;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT10 );
	pEdt->GetWindowText( code );

	if( code.GetLength() )
		query.Format( _T(" I.UNI_CODE = '%s' AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetGovCodeQuery()
{
	CString query, code;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT11 );
	pEdt->GetWindowText( code );

	if( code.GetLength() )
		query.Format( _T(" I.GOV_CODE = '%s' AND"), code );

	return query;	
}

CString CBusinessPopupSearcherDlg::GetDegCodeQuery()
{
	CString query, code;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT12 );
	pEdt->GetWindowText( code );

	if( code.GetLength() )
		query.Format( _T(" I.DEG_CODE = '%s' AND"), code );
		//query.Format( _T(" CATSEARCH( I.DEG_CODE, '%s', NULL ) > 0 AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetUserLimitQuery()
{
	CString query, code;

	code = TakeControlData( _T("이용제한구분") );
	if( code.GetLength() )
		query.Format( _T(" I.USE_LIMIT_CODE = '%s' AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetUseObjQuery()
{
	CString query, code;

	code = TakeControlData( _T("이용대상구분") );
	if( code.GetLength() )
		query.Format( _T(" I.USE_LIMIT_CODE = '%s' AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetMediaCodeQuery()
{
	CString query, code;

	code = TakeControlData( _T("매체구분") );
	if( code.GetLength() )
		query.Format( _T(" I.MEDIA_CODE = '%s' AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetMATCodeQuery()
{
	CString query, code;

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO9 );
	switch( pCombo->GetCurSel() )
	{
	case 0 :
		code = _T("DM");
		break;
	case 1 :
		code = _T("DP");
		break;
	default :
		return _T("");
		break;
	}

	query.Format( _T(" I.MAT_CODE = '%s' AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetTextLangQuery()
{
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT13 );
	CString code;
	pEdt->GetWindowText( code );

	CString query;
	if( code.GetLength() )
		query.Format( _T(" I.TEXT_LANG = '%s' AND"), code );
		//query.Format( _T(" CATSEARCH( I.TEXT_LANG, '%s', NULL ) > 0 AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetSumLangQuery()
{
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT14 );
	CString code;
	pEdt->GetWindowText( code );

	CString query;
	if( code.GetLength() )
		query.Format( _T(" I.SUM_LANG = '%s' AND"), code );
		//query.Format( _T(" CATSEARCH( I.SUM_LANG, '%s', NULL ) > 0 AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::GetPlaceInfoQuery()
{
	CString query, code;

	code = TakeControlData( _T("자료실") );
	if( code.GetLength() )
		query.Format( _T(" I.PLACE_INFO = '%s' AND"), code );

	return query;
}

CString CBusinessPopupSearcherDlg::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_BusinessPopupSearcherDlg"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_BusinessPopupSearcherDlg"), _T("K2Up") );
		AfxMessageBox( alias );
		return _T("");
	}
	
	// 코드값이 _T("0") 이거나 설명값이 _T("적용안함")일때 _T("")를 반환한다. 
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_BusinessPopupSearcherDlg"), _T("K2Up") );
			return _T("");
			break;
	}

	result.TrimLeft(); result.TrimRight();
	return result;
}

// 한국 대학교 명
VOID CBusinessPopupSearcherDlg::OnButtonUnv() 
{
	// TODO: Add your control notification handler code here
	
	SetKomarcData( IDC_EDIT10, 1 );
}

// 한국 정부기관 명
VOID CBusinessPopupSearcherDlg::OnButtonGov() 
{
	// TODO: Add your control notification handler code here

	SetKomarcData( IDC_EDIT11, 3 );
}

// 본문 언어
VOID CBusinessPopupSearcherDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	
	SetKomarcData( IDC_EDIT13, 2 );
}

// 요약문 언어
VOID CBusinessPopupSearcherDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here

	SetKomarcData( IDC_EDIT14, 2 );
}

VOID CBusinessPopupSearcherDlg::SetKomarcData( UINT id, INT type )
{
	CKOMarcGetterDlg dlg( this );
	dlg.InitSetProperty( type, _T("") );

	CEdit *pEdt = ( CEdit * )GetDlgItem( id );
	if( IDOK == dlg.DoModal() )
		pEdt->SetWindowText( dlg.m_code );
}
