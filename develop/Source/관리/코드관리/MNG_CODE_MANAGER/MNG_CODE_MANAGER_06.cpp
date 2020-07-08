// MNG_CODE_MANAGER_06.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_06.h"
#include "MNG_CODE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_06 dialog


CMNG_CODE_MANAGER_06::CMNG_CODE_MANAGER_06(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bModify = false;
}

BOOL CMNG_CODE_MANAGER_06::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_MANAGER_06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_06, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_06)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_06 message handlers

BOOL CMNG_CODE_MANAGER_06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_06") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_04") );
		return false;
	}

	m_index = -1;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 저장
VOID CMNG_CODE_MANAGER_06::OnOK() 
{
	// TODO: Add extra validation here
	
	CString data[ 20 ];
	if( GetCodeData( data ) )
	{
		AfxMessageBox( _T("코드값을 입력해 주십시오") );
		return;
	}
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	INT ids;
	ids = pParent->SaveCodeData( data, m_index );
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

// 닫기
VOID CMNG_CODE_MANAGER_06::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

// 이전
VOID CMNG_CODE_MANAGER_06::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( -1 );
}

// 다음
VOID CMNG_CODE_MANAGER_06::OnbNEXT() 
{
	// TODO: Add your control notification handler code here

	DisplayModifyData( 1 );	
}

VOID CMNG_CODE_MANAGER_06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	switch( m_type )
	{
		case 1 :
			SetWindowText( _T("코드 입력") );

			CButton *pBtn;
			pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
			pBtn->ShowWindow( false );
			pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
			pBtn->ShowWindow( false );

			break;
		case 2 :
			SetWindowText( _T("코드 수정") );
			DisplayModifyData( 0 );
			break;
		default :
			break;
	}
	
}

VOID CMNG_CODE_MANAGER_06::DisplayModifyData(INT flag)
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

			//m_idxCnt = GetSelectedRowCount();

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

	AllControlDisplay( _T("CM_MNG_CODE_MANAGER_06"), m_index );
}

INT CMNG_CODE_MANAGER_06::GetCodeData(CString data[])
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( data[ 0 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->GetWindowText( data[ 1 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT3 );
	pEdt->GetWindowText( data[ 2 ] );

	if( !data[ 0 ].GetLength() )
		return -1;
	
	return 0;
}
