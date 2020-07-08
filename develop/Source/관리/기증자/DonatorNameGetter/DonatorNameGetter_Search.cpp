// DonatorNameGetter_Search.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "DonatorNameGetter_Search.h"
#include "DonatorNameGetterDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search dialog


CDonatorNameGetter_Search::CDonatorNameGetter_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetter_Search)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDonatorNameGetter_Search::~CDonatorNameGetter_Search()
{	
	
}

BOOL CDonatorNameGetter_Search::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CDonatorNameGetter_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetter_Search)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetter_Search, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetter_Search)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO_2, OnRadio2)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search message handlers

BOOL CDonatorNameGetter_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("DonatorNameGetter_Search") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_02") );
		return false;
	}

	SetComboBox();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	
	  
}

VOID CDonatorNameGetter_Search::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CDonatorNameGetter_Search::OnOK() 
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
	
	CESL_DataMgr *pDM = FindDM( _T("DM_DonatorNameGetterDlg") );
	pDM->RefreshDataManager( MakeSelectQuery( ctrlData ) );
	
	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("검색결과가 없습니다.") );
	
	CDonatorNameGetterDlg *pParent = ( CDonatorNameGetterDlg * )pParentMgr;
	pParent->ShowGrid();

	CDialog::OnOK();
}

VOID CDonatorNameGetter_Search::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CDonatorNameGetter_Search::SetComboBox()
{
	CString list[ 2 ] = { _T("PN"), _T("GR") };
	for( INT i = 0; i < 2; i++ )
		m_combo.InsertString( i, list[ i ] );
	m_combo.SetCurSel( 0 );
}

VOID CDonatorNameGetter_Search::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	
	SetConditionContrl();
}

VOID CDonatorNameGetter_Search::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	
	SetConditionContrl();
}

VOID CDonatorNameGetter_Search::SetConditionContrl()
{
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

INT CDonatorNameGetter_Search::GetCheckedRdoBtn()
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

VOID CDonatorNameGetter_Search::GetCtrlData(CString data[])
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

CString CDonatorNameGetter_Search::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_DonatorNameGetter_Search"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_DonatorNameGetter_Search"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_DonatorNameGetter_Search"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

INT CDonatorNameGetter_Search::GetConditionType()
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

CString CDonatorNameGetter_Search::MakeSelectQuery(CString data[])
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
				tmp.Format( _T("YAER = '%s' AND "), data[ 1 ] );
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

	if( _T("AND ") == query.Right( 4 ) )
	{
		buf = query.Left( query.GetLength() - 4 );
		query = buf;
	}

	return query;
}

HBRUSH CDonatorNameGetter_Search::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
