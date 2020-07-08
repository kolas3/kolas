// LoanCardInfoAdder.cpp : implementation file
//

#include "stdafx.h"
#include "LoanCardInfoAdder.h"
#include "LoanCardFormatSetter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanCardInfoAdder dialog


CLoanCardInfoAdder::CLoanCardInfoAdder( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CLoanCardInfoAdder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();
}

BOOL CLoanCardInfoAdder::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	
	
VOID CLoanCardInfoAdder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanCardInfoAdder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanCardInfoAdder, CDialog)
	//{{AFX_MSG_MAP(CLoanCardInfoAdder)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_bLCFM_FONT, OnbFONT)
	ON_BN_CLICKED(IDC_bBACK_COLOR, OnbBACKCOLOR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanCardInfoAdder message handlers

VOID CLoanCardInfoAdder::OnOK() 
{
	// TODO: Add extra validation here
	
	//LOANCARD_OBJ_INFO objInfo;
	GetCurrentObjInfo( objInfo );

	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )pParentMgr;
	if( pParent->SaveLoanCardInfo( objInfo ) )
	{
		MessageBox( _T("Error : SaveLoanCardInfo()") );
		return;
	}
	
	
	//CDialog::OnOK();
}

VOID CLoanCardInfoAdder::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	
	ViewFixedType();
}

VOID CLoanCardInfoAdder::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	
	ViewFixedType();
}

VOID CLoanCardInfoAdder::ViewFixedType()
{
	CButton *pY = ( CButton * )GetDlgItem( IDC_RADIO5 );
	CButton *pN = ( CButton * )GetDlgItem( IDC_RADIO6 );

	CWnd *pWnd = NULL;
	
	pWnd = ( CWnd * )GetDlgItem( IDC_RICHEDIT1 );
	if( pY->GetCheck() )
		pWnd->ShowWindow( true );
	else
		pWnd->ShowWindow( false );

	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO2 );
	if( pN->GetCheck() )
		pWnd->ShowWindow( true );
	else
		pWnd->ShowWindow( false );
}

BOOL CLoanCardInfoAdder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CLoanCardInfoAdder::OnbFONT() 
{
	// TODO: Add your control notification handler code here
	
	LOGFONT longFont;
	CString fontName, size;
	GetDlgItemText( IDC_EDIT6, fontName );
	fontName.Replace( _T(" "), _T("") );
	size = fontName.Right( fontName.GetLength() - fontName.Find( _T(",") ) - 1 );
	fontName = fontName.Left( fontName.Find( _T(",") ) );
	_tcscpy( longFont.lfFaceName, fontName );
	longFont.lfHeight = _ttoi( size );
	CFontDialog dlg( &longFont );
	
	if( IDOK != dlg.DoModal() )
		return;

	dlg.GetCurrentFont( &longFont );
	fontName.Format( _T("%s,%d"), longFont.lfFaceName, dlg.GetSize() / 10  );
	SetDlgItemText( IDC_EDIT6, fontName );
}

VOID CLoanCardInfoAdder::OnbBACKCOLOR() 
{
	// TODO: Add your control notification handler code here
	
	
	CColorDialog dlg;
	COLORREF color;
	if( IDOK == dlg.DoModal() )
		color = dlg.GetColor();

	//CEdit *pEdt = ( CEdti * )GetDlgItem( IDC_EDIT7 );
	
}
