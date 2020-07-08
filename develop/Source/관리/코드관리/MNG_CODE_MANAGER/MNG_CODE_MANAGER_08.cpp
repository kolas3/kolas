// MNG_CODE_MANAGER_08.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_CODE_MANAGER_08.h"

#include "MNG_CODE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_08 dialog


CMNG_CODE_MANAGER_08::CMNG_CODE_MANAGER_08(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_08)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bModify = false;
}

BOOL CMNG_CODE_MANAGER_08::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_MANAGER_08::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_08)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_08, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_08)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_08 message handlers

VOID CMNG_CODE_MANAGER_08::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CButton *pBtn;
	
	switch( m_type )
	{
		case 1 :
			
			SetWindowText( _T("구입처 입력") );

			pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
			pBtn->ShowWindow( false );
			pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
			pBtn->ShowWindow( false );

			break;

		case 2 :

			SetWindowText( _T("구입처 수정") );
			DisplayModifyData( 0 );
			
			break;

		default :
			break;
	}	
}

VOID CMNG_CODE_MANAGER_08::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT ctrlCnt = 11;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );

	const INT fieldCnt = 20;
	CString fieldData[ fieldCnt ];
	ConvertToFieldData( fieldData, ctrlData );
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	INT ids;
	ids = pParent->SaveCodeData( fieldData, m_index );
	if( ids < 0 )
	{
		AfxMessageBox( _T("코드 입력에 실패하였습니다.") );
		return;
	}
	else if ( ids == 100 )
	{
		AfxMessageBox( _T("코드가 중복되었습니다."));
		return;
	}
	
	m_bModify = true;
	switch( m_type )
	{
		case 1 :
			AfxMessageBox( _T("코드가 저장되었습니다.") );
			CDialog::OnOK();
			break;
		case 2 :
			AfxMessageBox( _T("코드가 수정되었습니다.") );
	}	
}

VOID CMNG_CODE_MANAGER_08::GetCtrlData(CString data[])
{
	const INT ctrlCnt = 11;

	CWnd *pWnd;
	UINT wndID[ ctrlCnt ] = {
		IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT9, 
		IDC_EDIT10, IDC_EDIT11, IDC_EDIT7, IDC_EDIT8, IDC_EDIT6, IDC_EDIT12 };

	for( INT i = 0; i < ctrlCnt; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->GetWindowText( data[ i ] );
	}
}

VOID CMNG_CODE_MANAGER_08::ConvertToFieldData( CString data[], CString ctrlData[] )
{
	data[  0 ] = ctrlData[  0 ];
	data[  1 ] = ctrlData[  1 ];
	data[ 11 ] = ctrlData[  2 ];
	data[ 12 ] = ctrlData[  3 ];
	data[ 13 ] = ctrlData[  4 ];
	data[ 14 ] = ctrlData[  5 ];
	data[ 15 ] = ctrlData[  6 ];
	data[ 16 ] = ctrlData[ 10 ];
	data[ 17 ] = ctrlData[  7 ];
	data[ 18 ] = ctrlData[  8 ];
	data[ 19 ] = ctrlData[  9 ];
}

VOID CMNG_CODE_MANAGER_08::DisplayModifyData( INT flag )
{
	switch( flag )	
	{
		case 0 :

			SelectMakeList( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
			m_index = SelectGetHeadPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );

			if( m_index < 0 )
			{
				AfxMessageBox( _T("선택된 코드가 없습니다.") );
				PostMessage( WM_CLOSE );
				return;
			}

			break;

		case -1 :

			m_index = SelectGetPrev( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );

			if( -1 == m_index )
			{
				m_index = SelectGetHeadPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T(" 맨 처음 코드입니다.") );
				return;
			}

			break;

		case 1 :

			m_index = SelectGetNext( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
			if( -1 == m_index )
			{
				m_index = SelectGetTailPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T("마지막 코드입니다.") );
				return;
			}

			break;

		default :

			break;
	}

	AllControlDisplay( _T("CM_MNG_CODE_MANAGER_08"), m_index );
}

VOID CMNG_CODE_MANAGER_08::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( -1 );
}

VOID CMNG_CODE_MANAGER_08::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( 1 );
}

BOOL CMNG_CODE_MANAGER_08::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_08") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_08") );
		return false;
	}

	m_index = -1;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
