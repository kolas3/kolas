// CLASS_SIGN_MANAGER_04.cpp : implementation file
//

#include "stdafx.h"
#include "CLASS_SIGN_MANAGER_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_04 dialog


CCLASS_SIGN_MANAGER_04::CCLASS_SIGN_MANAGER_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLASS_SIGN_MANAGER_04)
		// 17/03/09 ±èÇý¿µ : °øÅë>ºÐ·ù±âÈ£¹×ÁÖÁ¦¸í°Ë»ö¿ä¾î>ºÐ·ù±âÈ£¿¡¼­ Á¶±âÇ¥ÂüÁ¶ ÆË¾÷¿¡¼­ ¸Þ¸ð¸® ´©¼ö ¹ß»ý
//*/ ADD ---------------------------------------------------------------------------
		m_pMCombo = NULL;
//*/ END ---------------------------------------------------------------------------
	//}}AFX_DATA_INIT
}

CCLASS_SIGN_MANAGER_04::~CCLASS_SIGN_MANAGER_04()
{	
	// 17/03/09 ±èÇý¿µ : °øÅë>ºÐ·ù±âÈ£¹×ÁÖÁ¦¸í°Ë»ö¿ä¾î>ºÐ·ù±âÈ£¿¡¼­ Á¶±âÇ¥ÂüÁ¶ ÆË¾÷¿¡¼­ ¸Þ¸ð¸® ´©¼ö ¹ß»ý
//*/ ADD ---------------------------------------------------------------------------
	if(m_pMCombo != NULL)
	{
		delete m_pMCombo;
		m_pMCombo = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}

BOOL CCLASS_SIGN_MANAGER_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CCLASS_SIGN_MANAGER_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLASS_SIGN_MANAGER_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLASS_SIGN_MANAGER_04, CDialog)
	//{{AFX_MSG_MAP(CCLASS_SIGN_MANAGER_04)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_04 message handlers

BOOL CCLASS_SIGN_MANAGER_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CCLASS_SIGN_MANAGER_04") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CCLASS_SIGN_MANAGER_04") );
		return false;
	}
	
	SetInitContrl();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCLASS_SIGN_MANAGER_04::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	OnSelchangeCombo1();
}

VOID CCLASS_SIGN_MANAGER_04::SetInitContrl()
{
	CComboBox *pCombo;
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("KDC") );
	pCombo->InsertString( 1, _T("DDC") );
	pCombo->SetCurSel( 0 );

	SetCode();

/*	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->InsertString( 0, _T("øöñÞÏ¡ÝÂøú") );
	pCombo->InsertString( 1, _T("ò¢æ´Ï¡ÝÂøú") );
	pCombo->InsertString( 2, _T("ùÛÏÐò¢æ´Ï¡ÝÂøú") );
	pCombo->InsertString( 3, _T("ùÛÏÐãÁÓÛÏ¡ÝÂøú") );
	pCombo->InsertString( 4, _T("ÏÐåÞÏ¡ÝÂøú") );
	pCombo->InsertString( 5, _T("ÙþùÊúþãÒÏ¡ÝÂøú") );
	pCombo->InsertString( 6, _T("åëåÞÍì÷×Ï¡ÝÂøú") );
	pCombo->InsertString( 7, _T("ðóÎçÍì÷×Ï¡ÝÂøú") );
	pCombo->SetCurSel( 0 );
*/
	//strHelpCode.Format(_T("%d"), m_pMCombo->GetCurSel()+1 );
}

VOID CCLASS_SIGN_MANAGER_04::SetCode()
{
EFS_BEGIN
	m_pMCombo = new CJComboBox;

	m_pMCombo->SubclassDlgItem( IDC_COMBO2, this );
	m_pMCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	m_pMCombo->AddItem( _T("1"), 0, 0 );
	m_pMCombo->AddItem( _T("øöñÞÏ¡ÝÂøú"), 1, 0 );

	m_pMCombo->AddItem( _T("2"), 0, 1 );
	m_pMCombo->AddItem( _T("ò¢æ´Ï¡ÝÂøú"), 1, 1 );

	m_pMCombo->AddItem( _T("3"), 0, 2 );
	m_pMCombo->AddItem( _T("ùÛÏÐò¢æ´Ï¡ÝÂøú"), 1, 2 );

	m_pMCombo->AddItem( _T("4"), 0, 3 );
	m_pMCombo->AddItem( _T("ùÛÏÐãÁÓÛÏ¡ÝÂøú"), 1, 3 );

	m_pMCombo->AddItem( _T("5"), 0, 4 );
	m_pMCombo->AddItem( _T("ÏÐåÞÏ¡ÝÂøú"), 1, 4 );

	m_pMCombo->AddItem( _T("6"), 0, 5 );
	m_pMCombo->AddItem( _T("ÙþùÊúþãÒÏ¡ÝÂøú"), 1, 5 );

	m_pMCombo->AddItem( _T("7"), 0, 6 );
	m_pMCombo->AddItem( _T("åëåÞÍì÷×Ï¡ÝÂøú"), 1, 6 );

	m_pMCombo->AddItem( _T("8"), 0, 7 );
	m_pMCombo->AddItem( _T("ðóÎçÍì÷×Ï¡ÝÂøú"), 1, 7 );
	
	m_pMCombo->SetCurSel( 0 );

	return ;
EFS_END
return ;
}


VOID CCLASS_SIGN_MANAGER_04::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	m_type = GetScreenType();
	// 0 : KDC, 1 : DDC

	CMSHFlexGrid *pGrid_1 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CMSHFlexGrid *pGrid_2 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	switch( m_type )
	{
		case 0 :
			pGrid_1->ShowWindow( true );
			pGrid_2->ShowWindow( false );
			break;
		case 1 :
			pGrid_1->ShowWindow( false );
			pGrid_2->ShowWindow( true );
			break;
		default :
			break;
	}
}

INT CCLASS_SIGN_MANAGER_04::GetScreenType()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );

	return pCombo->GetCurSel();
}

// CM_CLASS_SIGN_MANAGER_04
// DM_CLASS_SIGN_MANAGER_04_KDC
// DM_CLASS_SIGN_MANAGER_04_DDC

VOID CCLASS_SIGN_MANAGER_04::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT ctrlCnt = 3;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );
	
	SearchData( MakeSelectQuery( ctrlData ) );
	ShowGrid();
	
	//CDialog::OnOK();
}

VOID CCLASS_SIGN_MANAGER_04::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}

INT CCLASS_SIGN_MANAGER_04::GetCtrlData( CString data[] )
{
//	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
//	data[ 0 ].Format( _T("%d"), pCombo->GetCurSel() + 1 );

	data[ 0 ].Format(_T("%d"), m_pMCombo->GetCurSel()+1 );
	

	CEdit *pEdt;
	
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( data[ 1 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT3 );
	pEdt->GetWindowText( data[ 2 ] );
	
	return 0;
}

CString CCLASS_SIGN_MANAGER_04::MakeSelectQuery( CString data[] )
{
	CString query, tmp;

	if( data[ 0 ].GetLength() )
	{
		tmp.Format( _T("MEMHELP_CODE = '%s' AND "), data[ 0 ] );
		query += tmp;
	}

	if( data[ 1 ].GetLength() )
	{
		tmp.Format( _T("MEMHELP_NO >= '%s' AND "), data[ 1 ] );
		query += tmp;
	}

	if( data[ 2 ].GetLength() )
	{
		tmp.Format( _T("MEMHELP_NO <= '%s' AND "), data[ 2 ] );
		query += tmp;
	}

	if( _T("AND ") == query.Right( 4 ) )
	{
		tmp = query.Left( query.GetLength() - 4 );
		query = tmp;
	}

	query += _T("ORDER BY MEMHELP_NO ASC");

	return query;
}

INT CCLASS_SIGN_MANAGER_04::SearchData( CString where )
{
	CESL_DataMgr *pDM;
	switch( m_type )
	{
		case 0 :
			pDM = FindDM( _T("DM_CLASS_SIGN_MANAGER_04_KDC") );
			break;
		case 1 :
			pDM = FindDM( _T("DM_CLASS_SIGN_MANAGER_04_DDC") );
			break;
		default :
			break;
	}

	pDM->RefreshDataManager( where );
	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("°Ë»ö°á°ú°¡ ¾ø½À´Ï´Ù.") );
	
	return 0;
}

INT CCLASS_SIGN_MANAGER_04::ShowGrid()
{
	CString gridAlias;
	switch( m_type )
	{
		case 0 :
			gridAlias = _T("±×¸®µå_1");
			break;
		case 1 :
			gridAlias = _T("±×¸®µå_2");
			break;
		default :
			return -1;
			break;
	}

	ControlDisplay( _T("CM_CLASS_SIGN_MANAGER_04"), gridAlias );
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CCLASS_SIGN_MANAGER_04, CDialog)
    //{{AFX_EVENTSINK_MAP(CCLASS_SIGN_MANAGER_04)
	ON_EVENT(CCLASS_SIGN_MANAGER_04, IDC_mngGRID_1, 70 /* RowColChange */, OnRowColChangemngGRID1, VTS_NONE)
	ON_EVENT(CCLASS_SIGN_MANAGER_04, IDC_mngGRID_2, 70 /* RowColChange */, OnRowColChangemngGRID2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CCLASS_SIGN_MANAGER_04::OnRowColChangemngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	CESL_DataMgr *pDM = FindDM( _T("DM_CLASS_SIGN_MANAGER_04_KDC") );
	if( !pDM->GetRowCount() )
		return;

	INT row, col;
	GetCurrentCell( row, col, 0 );

	CString desc;
	pDM->GetCellData( _T("MEMHELP_DESC"), row - 1, desc );

	ViewDesc( desc );
}

VOID CCLASS_SIGN_MANAGER_04::OnRowColChangemngGRID2() 
{
	// TODO: Add your control notification handler code here
	
	CESL_DataMgr *pDM = FindDM( _T("DM_CLASS_SIGN_MANAGER_04_DDC") );
	if( !pDM->GetRowCount() )
		return;

	INT row, col;
	GetCurrentCell( row, col, 1 );

	CString desc;
	pDM->GetCellData( _T("MEMHELP_DESC"), row - 1, desc );

	ViewDesc( desc );
}

VOID CCLASS_SIGN_MANAGER_04::GetCurrentCell( INT &row, INT &col, INT option )
{
	CMSHFlexGrid *pGrid;
	switch( option )
	{
		case 0 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
			break;
		case 1 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
			break;
		default :
			return;
			break;
	}
		
	row = pGrid->GetRow();
	col = pGrid->GetCol();	
}

VOID CCLASS_SIGN_MANAGER_04::ViewDesc( CString data )
{
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT4 );
	pEdt->SetWindowText( data );
}


HBRUSH CCLASS_SIGN_MANAGER_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

