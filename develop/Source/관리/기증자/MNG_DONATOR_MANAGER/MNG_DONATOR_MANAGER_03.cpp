// MNG_DONATOR_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_DONATOR_MANAGER_03.h"

#include "MNG_DONATOR_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_DONATOR_MANAGER_03 dialog


CMNG_DONATOR_MANAGER_03::CMNG_DONATOR_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_DONATOR_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_DONATOR_MANAGER_03::~CMNG_DONATOR_MANAGER_03()
{
	
}



BOOL CMNG_DONATOR_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}		

VOID CMNG_DONATOR_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_DONATOR_MANAGER_03)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_DONATOR_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_DONATOR_MANAGER_03)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_DONATOR_MANAGER_03 message handlers

BOOL CMNG_DONATOR_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	
	SetControlByMode();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	
 	
}

VOID CMNG_DONATOR_MANAGER_03::SetControlByMode()
{
	m_combo.InsertString( 0, _T("PN") );
	m_combo.InsertString( 1, _T("GR") );
	m_combo.SetCurSel( 0 );
	
	UINT wndID[ 3 ] = { IDC_COMBO1, IDC_EDIT4, IDC_EDIT5 };
	CWnd *pWnd;
	
	CButton * pBtn;
	
	INT i;
	
	switch( m_mode )
	{
		case 1 :

			pWnd = ( CWnd * )GetDlgItem( wndID[ 2 ] );
			pWnd->ShowWindow( false );
			SetWindowText( _T("기증자 입력") );

			pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
			pBtn->ShowWindow( false );
			pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
			pBtn->ShowWindow( false );

			break;

		case 2 :

			for( i = 0; i < 3; i++ )
			{
				pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
				pWnd->EnableWindow( false );
			}

			SetWindowText( _T("기증자 수정") );

			break;

		default :
			break;
	}
}

VOID CMNG_DONATOR_MANAGER_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	m_checkedRow = NULL;
	m_rowCnt = 0;

	CMNG_DONATOR_MANAGER_01 *pParent = ( CMNG_DONATOR_MANAGER_01 * )pParentMgr;
	
	switch( m_mode )
	{
		case 1 :
			ViewYear();
			break;
		case 2 :
			m_index = 0;
			m_checkedRow = pParent->GetCheckedDMRow( m_rowCnt );

			if( !m_rowCnt )
			{
				AfxMessageBox( _T("선택된 목록이 없습니다.") );
				PostMessage( WM_CLOSE );
				break;
			}

			DisPlayModifedData( 0 );
			break;
		default :
			break;
	}

}

// 저장
VOID CMNG_DONATOR_MANAGER_03::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT ctrlCnt = 10;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );

	const INT dataCnt = 16;
	CString realData[ dataCnt ];
	ConvertData( realData, ctrlData );
	
	if( SaveData( realData ) )
	{
		AfxMessageBox( _T("저장에 실패하였습니다.") );
		return;
	}
	
	CMNG_DONATOR_MANAGER_01 *pParent = ( CMNG_DONATOR_MANAGER_01 * )pParentMgr;
	
	switch( m_mode )
	{
		case 1 :
			AfxMessageBox( _T("기증자가 등록되었습니다.") );
			pParent->ShowGrid();
			CDialog::OnOK();
			break;
		case 2 :
			AfxMessageBox( _T("기증자정보가 수정되었습니다.") );
			pParent->ShowGrid();
			return;
		default :
			break;
	}	
}

VOID CMNG_DONATOR_MANAGER_03::OnCancel()
{
	CDialog::DestroyWindow();
}

INT CMNG_DONATOR_MANAGER_03::GetCtrlData( CString data[] )
{
	const INT ctrlCnt = 10;
	
	CWnd *pWnd;

	UINT wndID[ ctrlCnt ] = {
		IDC_COMBO1, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, 
		IDC_EDIT8, IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12 };

	for( INT i = 0; i < ctrlCnt; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->GetWindowText( data[ i ] );
	}
	
	return 0;
}

INT CMNG_DONATOR_MANAGER_03::SaveData( CString data[] )
{
	const INT fieldCnt = 16;
	
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),			//	0
		_T("INPUT_DATE"),		//	1
		_T("DONATOR_FLAG"),		//	2
		_T("DONATOR_ID"),		//	3
		_T("DONATOR"),			//	4
		_T("ZIP_CODE"),			//	5
		_T("ADDRESS"),			//	6
		_T("TEL"),				//	7
		_T("FAX"),				//	8
		_T("EMAIL"),			//	9
		_T("INDEX_DONATOR"),	// 10
		_T("REMARK"),			// 11
		_T("FIRST_WORK"),		// 12
		_T("LAST_WORK"),		// 13
		_T("YEAR"),				// 14
		_T("SERIAL_NUM") };		// 15
	
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("NUMERIC") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	
	switch( m_mode )
	{
		case 1 :
			pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeInsertFrame( _T("CO_DONATOR_TBL") );
			break;
		case 2 :
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeUpdateFrame( _T("CO_DONATOR_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], m_checkedRow[ m_index ] );
			break;
		default :
			break;
	}
	

	CString error;
	INT ids = pDM->SendFrame();
	if( ids < 0 )
	{
		error.Format( _T("Error %d : SendFrame()"), ids );
		AfxMessageBox( error );
		return -1;
	}

	pDM->EndFrame();		

	if( 1 == m_mode )
		pDM->SetCellData( fieldName, fieldCnt, data, -1 );

	return 0;
}

VOID CMNG_DONATOR_MANAGER_03::ConvertData( CString data[], CString ctrlData[] )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );

	switch( m_mode )
	{
		case 1 :
			pDM->MakeRecKey( data[ 0 ] );
			break;
		case 2 :
			pDM->GetCellData( _T("REC_KEY"), m_checkedRow[ m_index ], data[ 0 ] );
			break;
		default :
			break;
	}

	data[ 1 ] = GetToDay();
	data[ 2 ] = ctrlData[ 0 ];
	data[ 15 ].Format( _T("%d"), GetSerialNum( ctrlData[ 1 ], ctrlData[ 0 ] ) );

	switch( m_mode )
	{
		case 1 :
			data[ 3 ].Format( _T("%s%s%05d"), ctrlData[ 0 ], data[ 1 ].Left( 4 ), _ttoi( data[ 15 ] ) );
			break;
		case 2 :
			data[ 3 ].Format( _T("%s%s%s"), ctrlData[ 0 ], ctrlData[ 1 ], ctrlData[ 2 ] );
			break;
		default :
			break;
	}

	data[ 4 ] = ctrlData[ 3 ];
	data[ 5 ] = ctrlData[ 4 ];
	data[ 6 ] = ctrlData[ 5 ];
	data[ 7 ] = ctrlData[ 6 ];
	data[ 8 ] = ctrlData[ 7 ];
	data[ 9 ] = ctrlData[ 8 ];
	data[ 10 ] = ctrlData[ 3 ];
	data[ 10 ].Replace( _T(" "), _T("") );
	data[ 11 ] = ctrlData[ 9 ];
	data[ 14 ] = data[ 1 ].Left( 4 );
	data[ 12 ] = GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__ );
	data[ 13 ] = GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__ );
}

CString CMNG_DONATOR_MANAGER_03::GetToDay()
{
	CString today;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );

	CTime time;
	time = pDM->GetDBTime();

	today.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	return today;
}

INT CMNG_DONATOR_MANAGER_03::GetSerialNum( CString year, CString flag )
{
	CString query, serialNum;
	query.Format( _T("SELECT MAX( SERIAL_NUM ) FROM CO_DONATOR_TBL WHERE YEAR = %s AND DONATOR_FLAG = '%s'"), year, flag );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );
	pDM->GetOneValueQuery( query, serialNum );

	return _ttoi( serialNum ) + 1;
}

VOID CMNG_DONATOR_MANAGER_03::ViewYear()
{
	CString today;
	today = GetToDay();

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT4 );
	pEdt->SetWindowText( today.Left( 4 ) );
}

VOID CMNG_DONATOR_MANAGER_03::DisPlayModifedData( INT flag )
{
	m_index += flag;

	if( -1 == m_index )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		m_index++;
		return;
	}

	if( m_rowCnt == m_index )
	{
		AfxMessageBox( _T("마지막 목록입니다.") );
		m_index--;
		return;
	}
	
	const INT ctrlCnt = 10;
	CString ctrlData[ ctrlCnt ];
	GetModifyData( ctrlData );

	ShowModifyData( ctrlData );
}

VOID CMNG_DONATOR_MANAGER_03::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here

	DisPlayModifedData( -1 );	
}

VOID CMNG_DONATOR_MANAGER_03::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	DisPlayModifedData( 1 );
}

VOID CMNG_DONATOR_MANAGER_03::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	if( m_checkedRow )
		free( m_checkedRow );

	
	CDialog::DestroyWindow();
}

VOID CMNG_DONATOR_MANAGER_03::GetModifyData(CString data[])
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );
	
	const INT fieldCnt = 10;
	CString fieldName[ fieldCnt ] = {
		_T("DONATOR_FLAG"),
		_T("YEAR"),
		_T("SERIAL_NUM"),
		_T("DONATOR"),
		_T("ZIP_CODE"),
		_T("ADDRESS"),
		_T("TEL"),
		_T("FAX"),
		_T("EMAIL"),
		_T("REMARK") };

	pDM->GetCellData( fieldName, fieldCnt, data, m_checkedRow[ m_index ] );
}

VOID CMNG_DONATOR_MANAGER_03::ShowModifyData( CString data[] )
{
	if( _T("PN") == data[ 0 ] )
		m_combo.SetCurSel( 0 );
	else if( _T("GR") == data[ 0 ] )
		m_combo.SetCurSel( 1 );

	CWnd *pWnd;
	UINT wndID[ 9 ] = {
		IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8, IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12 };

	for( INT i = 0; i < 9; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->SetWindowText( data[ i + 1 ] );
	}
}

HBRUSH CMNG_DONATOR_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


