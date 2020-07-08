// MNG_MNGTBL_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MNGTBL_MANAGER_02.h"
#include "MNG_MNGTBL_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGTBL_MANAGER_02 dialog


CMNG_MNGTBL_MANAGER_02::CMNG_MNGTBL_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MNGTBL_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CMNG_MNGTBL_MANAGER_02::~CMNG_MNGTBL_MANAGER_02()
{	
	
}


BOOL CMNG_MNGTBL_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_MNGTBL_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MNGTBL_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MNGTBL_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_MNGTBL_MANAGER_02)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGTBL_MANAGER_02 message handlers


BOOL CMNG_MNGTBL_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MNGTBL_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MNGTBL_MANAGER_01") );
		return false;
	}

	InitCtrlSetting();
	m_checkedDMRow = NULL;

	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_MNGTBL_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	switch( m_type )
	{
		case 1 :
			CleanControls();
			break;
		case 2 :
			SetCheckedInfo();
			ViewModifyData();
			break;
	}
}


// CM_MNG_MNGTBL_MANAGER_02

VOID CMNG_MNGTBL_MANAGER_02::InitCtrlSetting()
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	pCombo->InsertString( 1, _T("LIGHT") );
	pCombo->InsertString( 2, _T("UPGRADE") );
	pCombo->SetCurSel( 1 );
	pCombo->EnableWindow( false );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	pCombo->InsertString( 1, _T("수 서") );
	pCombo->InsertString( 2, _T("열 람") );
	pCombo->InsertString( 3, _T("정 리") );
	pCombo->InsertString( 4, _T("기 타") );
	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("단 행") );
	pCombo->InsertString( 1, _T("연 속") );
	pCombo->InsertString( 2, _T("공 통") );
	pCombo->SetCurSel( 2 );
	
	const INT btnCnt = 3;
	UINT btnID[ btnCnt ] = {
		IDC_bBEFORE, IDC_bNEXT, IDC_INSERT };
	bool btnOpt[ btnCnt ] = { false, false, false };

	switch( m_type )
	{
		case 1 :			// 입력
			btnOpt[ 2 ] = true;
			SetWindowText( _T(":::: 입 력 하 시 오") );
			break;
		case 2 :			// 수정
			btnOpt[ 0 ] = true;
			btnOpt[ 1 ] = true;
			SetWindowText( _T(":::: 수 정 하 시 오") );
			break;
		default :
			return;
			break;
	}

	CButton *pBtn;
	for( INT i = 0; i < btnCnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( btnID[ i ] );
		pBtn->ShowWindow( btnOpt[ i ] );
	};
}

// 저장
VOID CMNG_MNGTBL_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	
	INT ids = SaveData();
	if( -1 == ids )
	{
		AfxMessageBox( _T("Error : INT CMNG_MNGTBL_MANAGER_02::SaveData( VOID )") );
		return;
	}
	else if( -2 == ids )
		return;
	
	CDialog::OnOK();
}

VOID CMNG_MNGTBL_MANAGER_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	OnClose();
}

VOID CMNG_MNGTBL_MANAGER_02::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	FreeCheckedRowInfo();
	CDialog::DestroyWindow();
}

// 연속 입력
VOID CMNG_MNGTBL_MANAGER_02::OnInsert() 
{
	// TODO: Add your control notification handler code here
	
	INT ids = SaveData();
	if( -1 == ids )
	{
		AfxMessageBox( _T("Error : INT CMNG_MNGTBL_MANAGER_02::SaveData( VOID )") );
		return;
	}
	else if( -2 == ids )
		return;
		
	CleanControls();
}

INT CMNG_MNGTBL_MANAGER_02::SaveData()
{
	const INT cnt = 14;
	CString data[ cnt ];
	if( GetCtrlData( data ) )
		return -2;

	if( -1 == SaveData( data ) )
		return -2;
	
	CMNG_MNGTBL_MANAGER_01 *pParent = ( CMNG_MNGTBL_MANAGER_01 * )pParentMgr;
	pParent->ShowGrid();
	
	return 0;
}

VOID CMNG_MNGTBL_MANAGER_02::CleanControls()
{
	const INT edtCnt = 8;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

	CEdit *pEdt;
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->SetWindowText( _T("") );
	};

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->SetFocus();
}

INT CMNG_MNGTBL_MANAGER_02::GetCtrlData(CString data[])
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	switch( pCombo->GetCurSel() )
	{
		case 1 :
			data[ 1 ] = _T("L");
			break;
		case 2 :
			data[ 1 ] = _T("U");
			break;
		default :
			AfxMessageBox( _T("GROUP 1 을 선택하여 주십시오") );
			return -1;
			break;
	}

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	if( !pCombo->GetCurSel() )
	{
		AfxMessageBox( _T("GROUP 2 을 선택하여 주십시오") );
		return -1;
	}
	pCombo->GetWindowText( data[ 2 ] );
	data[ 2 ].Replace( _T(" "), _T("") );
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->GetWindowText( data[ 3 ] );
	data[ 3 ].Replace( _T(" "), _T("") );


	const INT edtCnt = 8;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

	CEdit *pEdt;
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( data[ i + 4 ] );
	}

	return 0;
}

INT CMNG_MNGTBL_MANAGER_02::SaveData( CString data[] )
{
	const INT fieldCnt = 15;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),			//	0
		_T("GROUP_1"),			//	1
		_T("GROUP_2"),			//	2
		_T("GROUP_3"),			//	3
		_T("GROUP_4"),			//	4
		_T("GROUP_5"),			//	5
		_T("CLASS_ALIAS"),		//	6
		_T("CLASS_DESC"),		//	7
		_T("VALUE_NAME"),		//	8
		_T("VALUE_1"),			//	9
		_T("VALUE_2"),			// 10
		_T("VALUE_3"),			// 11
		_T("FIRST_WORK"),		// 12
		_T("LAST_WORK"),		// 13
		_T("MANAGE_CODE")		// 14
	};

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	
	INT cnt = 0;
	CString notice;
	if( 1 == m_type )
	{
		cnt = CheckOverlapping( data[ 6 ] );
		if( cnt )
		{
			notice.Format( _T("%d 개의 중복된 Alias가 있습니다.\r\n계속 입력하시겠습니까?"), cnt );
		
			if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
				return -1;
		}
	}

	switch( m_type )
	{
		case 1 :
			pDM->MakeRecKey( data[ 0 ] );
			data[ 12 ] = GetWorkLogMsg( _T(""), __WFILE__, __LINE__ );
			break;
		case 2 :
			data[ 0 ] = GetCurrentPK();
			data[ 13 ] = GetWorkLogMsg( _T(""), __WFILE__, __LINE__ );
			break;
		default :
			return -1;
			break;
	}
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );
	
	switch( m_type )
	{
		case 1 :
			pDM->MakeInsertFrame( _T("ESL_MANAGE_TBL") );
			pDM->SetCellData( fieldName, fieldCnt, data, -1 );
			break;
		case 2 :
			pDM->MakeUpdateFrame( _T("ESL_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], m_checkedDMRow[ m_index ] );
			break;	
	}
	
	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

CString CMNG_MNGTBL_MANAGER_02::GetCurrentPK()
{
	CString pk;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], pk );

	return pk;

}

VOID CMNG_MNGTBL_MANAGER_02::SetCheckedInfo()
{
	m_index = 0;

	CMNG_MNGTBL_MANAGER_01 *pParent = ( CMNG_MNGTBL_MANAGER_01 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedRowInfo( m_checkedRowCnt );
}

VOID CMNG_MNGTBL_MANAGER_02::ViewModifyData()
{
	const ctrlCnt = 11;
	CString alias[ ctrlCnt ] = {
		_T("GROUP_1"), _T("GROUP_2"), _T("GROUP_3"), _T("GROUP_4"), _T("GROUP_5"), _T("CLASS_ALIAS"), _T("CLASS_DESC"), _T("VALUE_NAME"), _T("VALUE_1"), _T("VALUE_2"), _T("VALUE_3") };
	CString data[ ctrlCnt ];
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	pDM->GetCellData( alias, ctrlCnt, data, m_checkedDMRow[ m_index ] );

	CComboBox *pCombo;
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	if( _T("L") == data[ 0 ] )
		pCombo->SetCurSel( 1 );
	else if( _T("U") == data[ 0 ] )
		pCombo->SetCurSel( 2 );
	else
		pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	if( _T("수서") == data[ 1 ] )
		pCombo->SetCurSel( 1 );
	else if( _T("열람") == data[ 1 ] )
		pCombo->SetCurSel( 2 );
	else if( _T("정리") == data[ 1 ] )
		pCombo->SetCurSel( 3 );
	else if( _T("기타") == data[ 1 ] )
		pCombo->SetCurSel( 4 );
	else
		pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	if( _T("단행") == data[ 2 ] )
		pCombo->SetCurSel( 0 );
	else if( _T("연속") == data[ 2 ] )
		pCombo->SetCurSel( 1 );
	else if( _T("공통") == data[ 2 ] )
		pCombo->SetCurSel( 2 );

	const INT edtCnt = 8;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

	CEdit *pEdt;
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->SetWindowText( data[ i + 3 ] );
	}
}

// 이전
VOID CMNG_MNGTBL_MANAGER_02::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_index )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		return;
	}
	else
	{
		m_index--;
		ViewModifyData();
	}
}

// 다음
VOID CMNG_MNGTBL_MANAGER_02::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if( m_index == m_checkedRowCnt - 1 )
	{
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
		return;
	}
	else
	{
		m_index++;
		ViewModifyData();
	}
}

INT CMNG_MNGTBL_MANAGER_02::CheckOverlapping( CString alias )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	
	CString query, result;
	//++2008.10.22 UPDATE BY PWR {{++
	// 통합시스템 적용
	query.Format(	_T("SELECT COUNT(*) FROM ESL_MANAGE_TBL ")
					_T("WHERE CLASS_ALIAS='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), alias );
// 	query.Format( _T("SELECT COUNT( * ) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '%s'"), alias );
	//--2008.10.22 UPDATE BY PWR --}}
	pDM->GetOneValueQuery( query, result );
	
	return _ttoi( result );
}

VOID CMNG_MNGTBL_MANAGER_02::FreeCheckedRowInfo()
{
	if( 2 == m_type && m_checkedRowCnt )
	{
		free( m_checkedDMRow );
		m_checkedDMRow = NULL;
		m_checkedRowCnt = 0;
	}
}



HBRUSH CMNG_MNGTBL_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


