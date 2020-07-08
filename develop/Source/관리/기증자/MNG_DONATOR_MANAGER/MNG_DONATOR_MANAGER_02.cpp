// MNG_DONATOR_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_DONATOR_MANAGER_02.h"

#include "MNG_DONATOR_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_DONATOR_MANAGER_02 dialog


CMNG_DONATOR_MANAGER_02::CMNG_DONATOR_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_DONATOR_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_DONATOR_MANAGER_02::~CMNG_DONATOR_MANAGER_02()
{	
	
}

BOOL CMNG_DONATOR_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_DONATOR_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_DONATOR_MANAGER_02)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_DONATOR_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_DONATOR_MANAGER_02)
	ON_BN_CLICKED(IDC_RADIO_1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO_2, OnRadio2)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_DONATOR_MANAGER_02 message handlers

BOOL CMNG_DONATOR_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_DONATOR_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_02") );
		return false;
	}
	
	SetComboBox();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	
}


VOID CMNG_DONATOR_MANAGER_02::SetComboBox()
{
	CString list[ 2 ] = { _T("PN"), _T("GR") };
	for( INT i = 0; i < 2; i++ )
		m_combo.InsertString( i, list[ i ] );
	m_combo.SetCurSel( 0 );
}

VOID CMNG_DONATOR_MANAGER_02::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	
	SetConditionContrl();
}

VOID CMNG_DONATOR_MANAGER_02::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	
	SetConditionContrl();
}


VOID CMNG_DONATOR_MANAGER_02::SetConditionContrl()
{
	// 1 : 기증자 id
	// 2 : 기증자 이름
	// 3 : 아무것도 아닐때,
	
	CWnd *pWnd;
	UINT wndID[ 6 ] = { IDC_COMBO1, IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5 };
	bool wndOption[ 6 ] = { false, false, false, false, false, false };
	INT i;
	
	switch( GetCheckedRdoBtn() )
	{
		case 1 :
			for( i = 0; i < 4; i++ )
				wndOption[ i ] = true;
			break;
		case 2 :
			for( i = 4; i < 6; i++ )
				wndOption[ i ] = true;
			break;
		default :
			return;
			break;
	}

	for( i = 0; i < 6; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->EnableWindow( wndOption[ i ] );
	}		
}

INT CMNG_DONATOR_MANAGER_02::GetCheckedRdoBtn()
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_RADIO_1 );
	if( pBtn->GetCheck() )
		return 1;

	pBtn = ( CButton * )GetDlgItem( IDC_RADIO_2 );
	if( pBtn->GetCheck() )
		return 2;

	return 0;
}

VOID CMNG_DONATOR_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	
	if( !GetConditionType() )
	{
		AfxMessageBox( _T("검색구분을 선택하여 주십시오") );
		return;
	}
	
	const INT ctrlCnt = 8;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );
	pDM->RefreshDataManager( MakeSelectQuery( ctrlData ) );
	
	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("검색결과가 없습니다.") );
	
	CMNG_DONATOR_MANAGER_01 *pParent = ( CMNG_DONATOR_MANAGER_01 * )pParentMgr;
	pParent->ShowGrid();

	CDialog::OnOK();
}

VOID CMNG_DONATOR_MANAGER_02::GetCtrlData( CString data[] )
{
	const INT ctrlCnt = 6;

	CWnd *pWnd;
	UINT wndID[ ctrlCnt ] = {
		IDC_COMBO1, IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5 };
	
	for( INT i = 0; i < ctrlCnt; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->GetWindowText( data[ i ] );
	}

	data[ 6 ] = TakeControlData( _T("등록일자_부터") );
	data[ 7 ] = TakeControlData( _T("등록일자_까지") );
}

CString CMNG_DONATOR_MANAGER_02::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_DONATOR_MANAGER_02"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_DONATOR_MANAGER_02"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MNG_DONATOR_MANAGER_02"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

INT CMNG_DONATOR_MANAGER_02::GetConditionType()
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_RADIO_1 );
	if( pBtn->GetCheck() )
		return 1;

	pBtn = ( CButton * )GetDlgItem( IDC_RADIO_2 );
	if( pBtn->GetCheck() )
		return 2;

	return 0;
}

CString CMNG_DONATOR_MANAGER_02::MakeSelectQuery( CString data[] )
{
	CString query, tmp, buf;

	switch( GetConditionType() )
	{
		case 1 :

			if( data[ 0 ].GetLength() )
			{
				tmp.Format( _T("DONATOR_FLAG = '%s' AND "), data[ 0 ] );
				query += tmp;
			}

			if( data[ 1 ].GetLength() )
			{
				tmp.Format( _T("YEAR = '%s' AND "), data[ 1 ] );
				query += tmp;
			}

			if( data[ 2 ].GetLength() )
			{
				tmp.Format( _T("SERIAL_NUM >= '%s' AND "), data[ 2 ] );
				query += tmp;
			}

			if( data[ 3 ].GetLength() )
			{
				tmp.Format( _T("SERIAL_NUM <= '%s' AND "), data[ 3 ] );
				query += tmp;
			}
			
			break;

		case 2 :

			if( data[ 4 ].GetLength() )
			{
				buf = data[ 4 ];
				buf.Replace( _T(" "), _T("") );
				tmp.Format( _T("INDEX_DONATOR >= '%s' AND "), data[ 4 ] );
				query += tmp;
			}

			if( data[ 5 ].GetLength() )
			{
				buf = data[ 5 ];
				buf.Replace( _T(" "), _T("") );
				tmp.Format( _T("INDEX_DONATOR <= '%s' AND "), data[ 5 ] );
				query += tmp;
			}

			break;

		default :
			break;
	}

	if( data[ 6 ].GetLength() && data[ 7 ].GetLength() )
	{
		tmp.Format( _T("INPUT_DATE BETWEEN '%s' AND '%s' AND "), data[ 6 ], data[ 7 ] );
		query += tmp;
	}
	else
	{
		if( data[ 6 ].GetLength() )
		{
			tmp.Format( _T("INPUT_DATE >= '%s' AND "), data[ 6 ] );
			query += tmp;
		}

		if( data[ 7 ].GetLength() )
		{
			tmp.Format( _T("INPUT_DATE <= '%s' AND "), data[ 7 ] );
			query += tmp;
		}
	}

	if( _T("AND ") == query.Right( 4 ) )
	{
		buf = query.Left( query.GetLength() - 4 );
		query = buf;
	}

	return query;
}

VOID CMNG_DONATOR_MANAGER_02::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}


HBRUSH CMNG_DONATOR_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

