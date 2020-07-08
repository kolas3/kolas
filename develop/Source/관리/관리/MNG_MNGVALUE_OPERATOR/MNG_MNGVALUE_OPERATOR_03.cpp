// MNG_MNGVALUE_OPERATOR_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MNGVALUE_OPERATOR_03.h"
#include "MNG_MNGVALUE_OPERATOR_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGVALUE_OPERATOR_03 dialog


CMNG_MNGVALUE_OPERATOR_03::CMNG_MNGVALUE_OPERATOR_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MNGVALUE_OPERATOR_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_MNGVALUE_OPERATOR_03::~CMNG_MNGVALUE_OPERATOR_03()
{	
	
}



BOOL CMNG_MNGVALUE_OPERATOR_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_MNGVALUE_OPERATOR_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MNGVALUE_OPERATOR_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MNGVALUE_OPERATOR_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_MNGVALUE_OPERATOR_03)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()     
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGVALUE_OPERATOR_03 message handlers

BOOL CMNG_MNGVALUE_OPERATOR_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MNGVALUE_OPERATOR_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MNGVALUE_OPERATOR_02") );
		return false;
	}

	SetComboList();
	m_checkedRowCnt = 0;
	m_index = 0;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_MNGVALUE_OPERATOR_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	SetCtrlByType();

	if( 2 == m_type )
		if( !SetCheckedRowInfo() )
		{
			AfxMessageBox( _T("선택 목록이 없습니다.") );
			PostMessage( WM_CLOSE );
		}
		else
			ViewModifyData();
}

VOID CMNG_MNGVALUE_OPERATOR_03::SetComboList()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	CString num;
	for( INT i = 0; i < 39; i++ )
	{
		num.Format( _T("%d"), i );
		pCombo->InsertString( i, num );
	}
	pCombo->SetCurSel( 0 );
}

VOID CMNG_MNGVALUE_OPERATOR_03::SetCtrlByType()
{
	CEdit *pEdt;

	bool edtFlag = true;
	if( 1 == m_type )
	{
		edtFlag = false;
		SetWindowText( _T("입력") );
	}
	else
		SetWindowText( _T("수정") );
		
	pEdt = ( CEdit * )GetDlgItem( IDC_bBEFORE );
	pEdt->ShowWindow( edtFlag );
	pEdt = ( CEdit * )GetDlgItem( IDC_bNEXT );
	pEdt->ShowWindow( edtFlag );
}

//  저장
VOID CMNG_MNGVALUE_OPERATOR_03::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT cnt = 5;
	CString data[ cnt ];
	if( GetCtrlData( data ) )
	{
		AfxMessageBox( _T("항목을 모두 입력하여 주십시오") );
		return;
	}

	if( SaveData( data ) )
	{
		AfxMessageBox( _T("Error : VOID CMNG_MNGVALUE_OPERATOR_03::SaveData( CString data[] )") );
		return;
	}

	ShowGrid();

	if( 1 == m_type )
	{
		AfxMessageBox( _T("저장 되었습니다.") );
		CDialog::OnOK();
	}
	else
		AfxMessageBox( _T("수정되었습니다.") );	
}

VOID CMNG_MNGVALUE_OPERATOR_03::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

INT CMNG_MNGVALUE_OPERATOR_03::GetCtrlData( CString data[] )
{
	data[ 1 ] = TakeControlData( _T("이전등록구분") );
	data[ 3 ] = TakeControlData( _T("KOLAS2등록구분") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	data[ 2 ].Format( _T("%d"), pCombo->GetCurSel() );
	
	for( INT i = 1; i < 4; i++ )
	{
		data[ i ].TrimLeft();
		data[ i ].TrimRight();
		
		if( !data[ i ].GetLength() )
			return -1;
	}
	
	return 0;
}

CString CMNG_MNGVALUE_OPERATOR_03::TakeControlData( CString alias, INT option )
{
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_MNGVALUE_OPERATOR_03"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_MNGVALUE_OPERATOR_03"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MNG_MNGVALUE_OPERATOR_03"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

INT CMNG_MNGVALUE_OPERATOR_03::SaveData( CString data[] )
{
	const fieldCnt = 5;
	
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), 
		_T("PREV_REG_CODE"),
		_T("PREV_ACCESSION_NO_SPACE_CNT"),
		_T("KOLASII_REG_CODE"),
		_T("LAST_WORK") };
	if( 1 == m_type )
		fieldName[ 4 ].Format( _T("FIRST_WORK") );

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING") };
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_02") );

	if( 1 == m_type )
		pDM->MakeRecKey( data[ 0 ] );
	else if( 2 == m_type )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], data[ 0 ] );

	data[ 4 ] = GetWorkLogMsg( _T(""), __WFILE__, __LINE__ );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	if( 1 == m_type )
	{
		pDM->MakeInsertFrame( _T("CO_LASTMNGNUM_MANAGE_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, data, -1 );
	}
	else if( 2 == m_type )
		pDM->MakeUpdateFrame( _T("CO_LASTMNGNUM_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], m_checkedDMRow[ m_index ] );
		
	pDM->SendFrame();
	pDM->EndFrame();	
	
	return 0;
}



VOID CMNG_MNGVALUE_OPERATOR_03::ShowGrid()
{
	CMNG_MNGVALUE_OPERATOR_02 *pParent = ( CMNG_MNGVALUE_OPERATOR_02 * )pParentMgr;
	pParent->ShowGrid();
}

VOID CMNG_MNGVALUE_OPERATOR_03::FreeCheckedRowInfo()
{
	if( m_checkedRowCnt )
		free( m_checkedDMRow );
}

VOID CMNG_MNGVALUE_OPERATOR_03::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	FreeCheckedRowInfo();
	
	CDialog::OnClose();
}

INT CMNG_MNGVALUE_OPERATOR_03::SetCheckedRowInfo()
{
	CMNG_MNGVALUE_OPERATOR_02 *pParent = ( CMNG_MNGVALUE_OPERATOR_02 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedRowInfo( m_checkedRowCnt );
	
	return m_checkedRowCnt;	
}

VOID CMNG_MNGVALUE_OPERATOR_03::ViewModifyData()
{
	const INT ctrlCnt = 3;
	CString alias[ ctrlCnt ] = {
		_T("PREV_REG_CODE"), _T("PREV_ACCESSION_NO_SPACE_CNT"), _T("KOLASII_REG_CODE") };
	CString data[ ctrlCnt ];

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_02") );
	pDM->GetCellData( alias, ctrlCnt, data, m_checkedDMRow[ m_index ] );

	SendControlData( _T("이전등록구분"), data[ 0 ] );
	SendControlData( _T("KOLAS2등록구분"), data[ 2 ] );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->SetCurSel( _ttoi( data[ 1 ] ) );
}

// 이전
VOID CMNG_MNGVALUE_OPERATOR_03::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_index )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		return;
	}

	m_index--;
	ViewModifyData();
}	

// 다음
VOID CMNG_MNGVALUE_OPERATOR_03::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if( m_checkedRowCnt - 1 == m_index )
	{
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
		return;
	}

	m_index++;
	ViewModifyData();
}

INT CMNG_MNGVALUE_OPERATOR_03::SendControlData(CString alias, CString data)
{
	SetControlData( _T("CM_MNG_MNGVALUE_OPERATOR_03"), alias, data, -1, 0 );

	return 0;
}


HBRUSH CMNG_MNGVALUE_OPERATOR_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
