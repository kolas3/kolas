// MNG_PROVIDER_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_PROVIDER_MANAGER_02.h"

#include "MNG_PROVIDER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_PROVIDER_MANAGER_02 dialog


CMNG_PROVIDER_MANAGER_02::CMNG_PROVIDER_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_PROVIDER_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CMNG_PROVIDER_MANAGER_02::~CMNG_PROVIDER_MANAGER_02()
{
	
}

BOOL CMNG_PROVIDER_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CMNG_PROVIDER_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_PROVIDER_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_PROVIDER_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_PROVIDER_MANAGER_02)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_PROVIDER_MANAGER_02 message handlers

VOID CMNG_PROVIDER_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CButton *pBtn_1 = ( CButton * )GetDlgItem( IDC_bBEFORE );
	CButton *pBtn_2 = ( CButton * )GetDlgItem( IDC_bNEXT );
	
	switch( m_mode )
	{
		case 1 :

			SetWindowText( _T("구입처 입력") );

			pBtn_1->ShowWindow( false );
			pBtn_2->ShowWindow( false );

			break;

		case 2 :

			SetWindowText( _T("구입처 수정") );
			DisplayModifiedData( 0 );
			
			break;

		default :
			break;
	}
}

BOOL CMNG_PROVIDER_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_PROVIDER_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_PROVIDER_MANAGER_02") );
		return false;
	}


	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CMNG_PROVIDER_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT ctrlCnt = 11;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );

	if( SaveData( ctrlData ) )
	{
		AfxMessageBox( _T("Error : SaveData( CString data[] )") );
		return;
	}
	
	CMNG_PROVIDER_MANAGER_01 *pParent = ( CMNG_PROVIDER_MANAGER_01 * )pParentMgr;
	pParent->ShowGrid();

	if( 1 == m_mode )
	{
		AfxMessageBox( _T("저장되었습니다.") );
		CDialog::OnOK();
	}
	else
		AfxMessageBox( _T("수정되었습니다.") );
}

INT CMNG_PROVIDER_MANAGER_02::GetCtrlData( CString data[] )
{
	const INT ctrlCnt = 11;
	UINT ctrlID[ ctrlCnt ] = {
		IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT7, IDC_EDIT8, IDC_EDIT6, IDC_EDIT12 };
	
	CEdit *pEdt;
	for( INT i = 0; i < ctrlCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( ctrlID[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}
	
	return 0;
}

INT CMNG_PROVIDER_MANAGER_02::SaveData( CString data[] )
{
	const INT fieldCnt = 16;

	CString fieldName[ fieldCnt ] = {
		_T("PROVIDER_CODE"),	//	0
		_T("PROVIDER_NAME"),	//	1
		_T("ZIP_CODE"),			//	2
		_T("ADDRESS"),			//	3
		_T("TEL"),				//	4
		_T("FAX"),				//	5
		_T("EMAIL"),			//	6
		_T("DEPART"),			//	7
		_T("CHERK"),			//	8
		_T("PRESIDENT"),		//	9
		_T("REMARK"),			// 10
		_T("FIRST_WORK"),		// 11
		_T("LAST_WORK"),		// 12
		_T("REC_KEY"),			// 13
		_T("ACTIVE"),			// 14
		_T("DEFAULT_VALUE") };	// 15

	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING") }; 

	CString fieldData[ fieldCnt ];

	for( INT i = 0; i < 11; i++ )
		if( data[ i ].GetLength() )
			fieldData[ i ] = data[ i ];

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_PROVIDER_MANAGER_01") );

	switch( m_mode )
	{
		case 1 :
			pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeRecKey( fieldData[ 13 ] );
			fieldData[ 14 ] = _T("Y");
			fieldData[ 15 ] = _T("N");
			break;
		case 2 :
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->GetCellData( _T("REC_KEY"), m_index, fieldData[ 13 ] );
			fieldData[ 14 ] = _T("Y");
			fieldData[ 15 ] = _T("N");
			 break;
		default :
			break;
	}
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( i = 0; i < fieldCnt; i++ )
		if( fieldData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );

	switch( m_mode )
	{
		case 1 :
			pDM->MakeInsertFrame( _T("CO_PROVIDER_TBL") );
			break;
		case 2 :
			pDM->MakeUpdateFrame( _T("CO_PROVIDER_TBL"), _T("REC_KEY"), _T("NUMERIC"), fieldData[ 13 ], m_index );
			break;
		default :
			break;
	}	

	INT ids = pDM->SendFrame();
	CString error;
	if( ids < 0 )
	{
		error.Format( _T("Error %d : SendFrame()"), ids );
		AfxMessageBox( error );
		return -1;
	}

	pDM->EndFrame();

	pDM->SetCellData( fieldName, fieldCnt, fieldData, -1 );
	
	return 0;
}

VOID CMNG_PROVIDER_MANAGER_02::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifiedData( -1 );
}

VOID CMNG_PROVIDER_MANAGER_02::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifiedData( 1 );	
}

VOID CMNG_PROVIDER_MANAGER_02::DisplayModifiedData( INT flag )
{
	switch( flag )	
	{
		case 0 :

			SelectMakeList( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );
			m_index = SelectGetHeadPosition( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );

			if( m_index < 0 )
			{
				AfxMessageBox( _T("선택된 목록이 없습니다.") );
				PostMessage( WM_CLOSE );
				return;
			}

			break;

		case -1 :

			m_index = SelectGetPrev( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );

			if( -1 == m_index )
			{
				m_index = SelectGetHeadPosition( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T(" 맨 처음 목록입니다.") );
				return;
			}

			break;

		case 1 :

			m_index = SelectGetNext( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );
			if( -1 == m_index )
			{
				m_index = SelectGetTailPosition( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T("마지막 목록입니다.") );
				return;
			}

			break;

		default :

			break;
	}

	AllControlDisplay( _T("CM_MNG_PROVIDER_MANAGER_02"), m_index );
}

VOID CMNG_PROVIDER_MANAGER_02::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}


HBRUSH CMNG_PROVIDER_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

