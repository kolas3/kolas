// MNG_KOMARC_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_KOMARC_MANAGER_03.h"
#include "MNG_KOMARC_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KOMARC_MANAGER_03 dialog


CMNG_KOMARC_MANAGER_03::CMNG_KOMARC_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KOMARC_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KOMARC_MANAGER_03::~CMNG_KOMARC_MANAGER_03()
{
	FreeCheckedInfo();
}

BOOL CMNG_KOMARC_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_KOMARC_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KOMARC_MANAGER_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KOMARC_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_KOMARC_MANAGER_03)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KOMARC_MANAGER_03 message handlers

BOOL CMNG_KOMARC_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_KOMARC_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_01") );
		return false;
	}

	SetComboBox();

	m_checkedRowCnt = 0;
	m_checkedDMRow = NULL;
	m_index = 0;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_KOMARC_MANAGER_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	SetType();

	if( 2 == m_mode )
		SetControlData();

	InitSetControls();

}

VOID CMNG_KOMARC_MANAGER_03::InitSetControls()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	
	CButton *pBtn;
	if( 1 == m_mode )
	{
		pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
		pBtn->ShowWindow( false );
		pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
		pBtn->ShowWindow( false );
		SetWindowText( _T("입력") );
		pCombo->EnableWindow( true );
	}
	else
	{
		pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
		pBtn->ShowWindow( true );
		pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
		pBtn->ShowWindow( true );
		SetWindowText( _T("수정") );
		pCombo->EnableWindow( false );
	}
	
	SetEnableEdit();
}

VOID CMNG_KOMARC_MANAGER_03::SetEnableEdit( INT option )
{
	if( -1 == option )
		option = m_type;

	CString output;
	bool show = false;

	switch( option )
	{
		case 1 :
		case 3 :
		case 7 :
			output.Format( _T("관리구분") );
			show = true;
			break;
		case 4 :
			output.Format( _T("부호(num)") );
			show = true;
			break;
		default :
			break;	
	}

	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_STATIC_5 );
	pWnd->SetWindowText( output );
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT8 );
	pEdt->ShowWindow( show );
}

VOID CMNG_KOMARC_MANAGER_03::SetComboBox()
{
	const INT listCnt = 8;
	CString comboList[ listCnt ] = {
		_T("발행국부호"), _T("한국대학부호"), _T("언어구분부호"), _T("한국정부기관부호"), _T("한국지역구분분호"), _T("외국지역구분부호"), _T("국가부호"), _T("한국도서관부호") };
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	for( INT i = 0; i < listCnt; i++ )
		pCombo->InsertString( i, comboList[ i ] );
	pCombo->SetCurSel( 0 );
}

VOID CMNG_KOMARC_MANAGER_03::SetType()
{
	m_type = GetType();

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->SetCurSel( m_type );
}

INT CMNG_KOMARC_MANAGER_03::GetType()
{
	CMNG_KOMARC_MANAGER_01 *pParent = ( CMNG_KOMARC_MANAGER_01 * )pParentMgr;
	return pParent->m_type;
}

VOID CMNG_KOMARC_MANAGER_03::SetControlData()
{
	SetCheckedInfo();

	if( !m_checkedRowCnt )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		PostMessage( WM_CLOSE );
	}

	m_index = 0;
	ViewModifyData();
}

VOID CMNG_KOMARC_MANAGER_03::FreeCheckedInfo()
{
	m_checkedRowCnt = 0;

	if( !m_checkedDMRow )
	{
		free( m_checkedDMRow );
		m_checkedDMRow = NULL;
	}
}

VOID CMNG_KOMARC_MANAGER_03::SetCheckedInfo()
{
	CMNG_KOMARC_MANAGER_01 *pParent = ( CMNG_KOMARC_MANAGER_01 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedInfo( m_checkedRowCnt );
}

VOID CMNG_KOMARC_MANAGER_03::ViewModifyData()
{
	CESL_DataMgr *pDM;
	CString dmAlias[ 8 ] = {
		_T("DM_MNG_KOMARC_MANAGER_01_01"), _T("DM_MNG_KOMARC_MANAGER_01_02"), _T("DM_MNG_KOMARC_MANAGER_01_03"),
		_T("DM_MNG_KOMARC_MANAGER_01_04"), _T("DM_MNG_KOMARC_MANAGER_01_05"), _T("DM_MNG_KOMARC_MANAGER_01_06"),
		_T("DM_MNG_KOMARC_MANAGER_01_07"), _T("DM_MNG_KOMARC_MANAGER_01_08") };
	pDM = FindDM( dmAlias[ m_type ] );

	const INT cnt = 5;
	CString data[ cnt ];
	CString alias[ cnt ] = { _T("CLASS_CODE"), _T("DESCRIPTION"), _T("REFERENCE"), _T("REVERSE_REFERENCE"), _T("MANAGE_CLASS") };
	if( 4 == m_type )
		alias[ 4 ] = _T("NUMBER_CODE");

	INT aliasCnt;
	if( 1 == m_type || 3 == m_type || 4 == m_type || 7 == m_type )
		aliasCnt = 5;
	else
		aliasCnt = 4;

	pDM->GetCellData( alias, aliasCnt, data, m_checkedDMRow[ m_index ] );
	
	CEdit *pEdt;
	UINT edtID[ cnt ] = { IDC_EDIT1, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };
	for( INT i = 0; i < aliasCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->SetWindowText( data[ i ] );
	}
}

VOID CMNG_KOMARC_MANAGER_03::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_index )
		AfxMessageBox( _T("맨 처음 목록입니다.") );
	else
	{
		m_index--;
		ViewModifyData();
	}
}

VOID CMNG_KOMARC_MANAGER_03::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if( m_checkedRowCnt == m_index + 1 )
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
	else
	{
		m_index++;
		ViewModifyData();
	}
}

// 저장
VOID CMNG_KOMARC_MANAGER_03::OnOK() 
{
	// TODO: Add extra validation here
	OnSelchangeCombo1();
	const INT ctrlCnt = 5;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );

	if( SaveData( ctrlData ) )
	{
		AfxMessageBox( _T("Error : SaveData( CString ctrlData[] )") );
		return;
	}
	
	CMNG_KOMARC_MANAGER_01 *pParent = ( CMNG_KOMARC_MANAGER_01 * )pParentMgr;
	pParent->ShowGrid( m_type );
	
	if( 1 == m_mode )
	{
		AfxMessageBox( _T("저장되었습니다.") );
		CDialog::OnOK();
	}
	else if( 2 == m_mode )
		AfxMessageBox( _T("수정되었습니다.") );
}


VOID CMNG_KOMARC_MANAGER_03::GetCtrlData( CString data[] )
{
	CEdit *pEdt;
	const INT edtCnt = 5;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT1, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}
}

INT CMNG_KOMARC_MANAGER_03::SaveData( CString data[] )
{
	CESL_DataMgr *pDM;
	CString dmAlias[ 8 ] = {
		_T("DM_MNG_KOMARC_MANAGER_01_01"), _T("DM_MNG_KOMARC_MANAGER_01_02"), _T("DM_MNG_KOMARC_MANAGER_01_03"),
		_T("DM_MNG_KOMARC_MANAGER_01_04"), _T("DM_MNG_KOMARC_MANAGER_01_05"), _T("DM_MNG_KOMARC_MANAGER_01_06"),
		_T("DM_MNG_KOMARC_MANAGER_01_07"), _T("DM_MNG_KOMARC_MANAGER_01_08") };
	pDM = FindDM( dmAlias[ m_type ] );

	const INT cnt = 8;
	CString fieldName[ cnt ] = {
		_T("CLASS_CODE"),			//	0
		_T("DESCRIPTION"),			//	1
		_T("REFERENCE"),			//	2
		_T("REVERSE_REFERENCE"),	//	3
		_T("INDEX_DESCRIPTION"),	//	4
		_T("FIRST_WORK"),			//	5
		_T("REC_KEY"),				//	6
		_T("MANAGE_CLASS") };		//	7
	CString fieldType[ cnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING") };
	CString fieldData[ cnt ];

	if( 2 == m_mode )
		fieldName[ 5 ] = _T("LAST_WORK");

	if( 4 == m_type )
		fieldName[ 7 ] = _T("NUMBER_CODE");

	INT fieldCnt;
	if( 1 == m_type || 3 == m_type || 4 == m_type || 7 == m_type )
		fieldCnt = 8;
	else
		fieldCnt = 7;

	fieldData[ 0 ] = data[ 0 ];
	fieldData[ 1 ] = data[ 1 ];
	fieldData[ 2 ] = data[ 2 ];
	fieldData[ 3 ] = data[ 3 ];
	fieldData[ 4 ] = data[ 1 ];
	fieldData[ 4 ].Replace( _T(" "), _T("") );
	fieldData[ 5 ] = GetWorkLogMsg( _T("KOMARC 관리"), __WFILE__, __LINE__ );
	
	if( 1 == m_mode )
		pDM->MakeRecKey( fieldData[ 6 ] );
	else
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], fieldData[ 6 ] );

	fieldData[ 7 ] = data[ 4 ];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );
	
	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( GetCurrentTable() );
		pDM->SetCellData( fieldName, fieldCnt, fieldData, -1 );
	}
	else if( 2 == m_mode )
		pDM->MakeUpdateFrame( GetCurrentTable(), _T("REC_KEY"), _T("NUMERIC"), fieldData[ 6 ], m_checkedDMRow[ m_index ] );

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

VOID CMNG_KOMARC_MANAGER_03::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	m_type = pCombo->GetCurSel();
}

CString CMNG_KOMARC_MANAGER_03::GetCurrentTable()
{
	CString table[ 8 ] = { 
		_T("CO_PUBLISH_COUNTRY_CODE_TBL"), 
		_T("CO_KOR_UNIVERSITY_CODE_TBL"), 
		_T("CO_LANG_CODE_TBL"),
		_T("CO_GOVERNMENT_CODE_TBL"),
		_T("CO_KOR_REGION_CODE_TBL"),
		_T("CO_FOREIGN_REGION_CODE_TBL"),
		_T("CO_COUNTRY_CODE_TBL"),
		_T("CO_LIBRARY_CODE_ADMN_TBL") };

	return table[ m_type ];
}

// DM_MNG_KOMARC_MANAGER_01_01	CO_PUBLISH_COUNTRY_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_02	KOR_UNIVERSITY_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_03	LANG_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_04	GOVERNMENT_INSTRUMENT_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_05	KOR_REGION_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_06	FOREIGN_REGION_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_07	COUNTRY_CODE_TBL
// DM_MNG_KOMARC_MANAGER_01_08	LIBRARY_CODE_ADMN_TBL

HBRUSH CMNG_KOMARC_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
