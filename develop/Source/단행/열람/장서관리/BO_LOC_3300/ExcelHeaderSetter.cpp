// ExcelHeaderSetter.cpp : implementation file
//

#include "stdafx.h"
#include "ExcelHeaderManager.h"
#include "ExcelHeaderSetter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExcelHeaderSetter dialog


CExcelHeaderSetter::CExcelHeaderSetter(CESL_Mgr *pParent /*=NULL*/)
	: CESL_Mgr(CExcelHeaderSetter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcelHeaderSetter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_index = 0;
	m_inputMode = 0;

	int i=0;
	for(i=0;i<DMALIAS_CNT;i++){
		m_DMAliasList[ i ] = _T("");
	}


}

CExcelHeaderSetter::~CExcelHeaderSetter()
{

}

VOID CExcelHeaderSetter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcelHeaderSetter)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExcelHeaderSetter, CDialog)
	//{{AFX_MSG_MAP(CExcelHeaderSetter)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_b3300_BEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_b3300_NEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelHeaderSetter message handlers

BOOL CExcelHeaderSetter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	MakeDMAliasListCombo();
	ShowMoveBtns();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CExcelHeaderSetter::MakeDMAliasListCombo()
{
	CStdioFile fFile;
	INT ids = fFile.Open(_T("..\\cfg\\LoanUserInputOutput\\반입반출필드.txt") , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 )
	{
		AfxMessageBox( _T("설정 파일을 열수 없습니다.") );
		return ;
	}

		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
	
	CString sLine;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cDMALIAS_LIST );
	INT i = 0;

	pCombo->InsertString( 0 , _T("-선택-") );
	i++;
	while( fFile.ReadString(sLine) )
	{
		pCombo->InsertString( i, sLine );
		i++;
	}

	fFile.Close();

	pCombo->SetCurSel( 0 );

}

VOID CExcelHeaderSetter::SetDMAliasArray()
{

}

VOID CExcelHeaderSetter::OnOK() 
{
	// TODO: Add extra validation here
	
	CString data[ 2 ];
	GetDlgItemText( IDC_eEXCEL_HEADER_ALIAS, data[ 1 ] );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cDMALIAS_LIST );
	if( pCombo->GetCurSel() )
		pCombo->GetWindowText( data[ 0 ] );
	
	CExcelHeaderManager *pParent = ( CExcelHeaderManager * )m_pParent;
	pParent->SaveAliasInfoFile( data, ( 1 == m_inputMode ? -1 : m_checkedRow[ m_index ] ) );
	
	switch( m_inputMode )
	{
		case 1 :
			pParent->ShowGrid();
			AfxMessageBox( _T("목록에 추가되었습니다.") );
			break;
		case 2 :
			AfxMessageBox( _T("목록에서 수정되었습니다.") );
			break;
		default :
			break;
	}
}

VOID CExcelHeaderSetter::ShowMoveBtns()
{
	CEdit *pEdt = NULL;
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_b3300_BEFORE, IDC_b3300_NEXT };

	bool flag = true;
	if( 1 == m_inputMode )
		flag = false;

	for( INT i = 0; i < cnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( id[ i ] );
		pEdt->ShowWindow( flag );
	}
}

VOID CExcelHeaderSetter::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow && 2 == m_inputMode )
		Display( 0 );
}

VOID CExcelHeaderSetter::Display( INT flag )
{
	m_index += flag;

	CExcelHeaderManager *pParent = ( CExcelHeaderManager * )m_pParent;
	CString data[ 2 ];
	pParent->GetAliasData( data, m_checkedRow[ m_index ] );

	SetDlgItemText( IDC_eEXCEL_HEADER_ALIAS, data[ 1 ] );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cDMALIAS_LIST );
	for( INT i = 0; i < DMALIAS_CNT; i++ )
		if( m_DMAliasList[ i ] == data[ 0 ] )
		{
			pCombo->SetCurSel( i );
			break;
		}
		else
			pCombo->SetCurSel( 0 );
	
	EnableMoveBtns();
}

VOID CExcelHeaderSetter::EnableMoveBtns()
{
	CEdit *pEdt = NULL;

	pEdt = ( CEdit * )GetDlgItem( IDC_b3300_BEFORE );
	if( !m_index )
		pEdt->EnableWindow( false );
	else
		pEdt->EnableWindow( true );

	pEdt = ( CEdit * )GetDlgItem( IDC_b3300_NEXT );
	if( m_checkedRow.GetSize() - 1 == m_index )
		pEdt->EnableWindow( false );
	else
		pEdt->EnableWindow( true );
}

VOID CExcelHeaderSetter::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	Display( -1 );
}

VOID CExcelHeaderSetter::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	Display( 1 );
}

HBRUSH CExcelHeaderSetter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
