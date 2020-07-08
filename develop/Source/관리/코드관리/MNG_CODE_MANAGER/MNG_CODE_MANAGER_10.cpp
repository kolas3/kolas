// MNG_CODE_MANAGER_10.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_10.h"
#include "MNG_CODE_MANAGER_09.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_10 dialog


CMNG_CODE_MANAGER_10::CMNG_CODE_MANAGER_10(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_10)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CMNG_CODE_MANAGER_10::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_10)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_10, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_10)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_10 message handlers

BOOL CMNG_CODE_MANAGER_10::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitSetControls();
	m_checkedRowCnt = 0;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_MANAGER_10::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CEdit *pEdt;
	

	if( 1 == m_type )
	{
		pEdt = ( CEdit * )GetDlgItem( IDC_BEFORE );
		pEdt->ShowWindow( false );
		pEdt = ( CEdit * )GetDlgItem( IDC_NEXT );
		pEdt->ShowWindow( false );
		SetWindowText( _T("입 력") );
	}	
	else if( 2 == m_type )
	{
		if( !SetCheckedRowInfo() )
		{
			AfxMessageBox( _T("선택 목록이 없습니다.") );
			PostMessage( WM_CLOSE );
		}
		else
		{
			m_index = 0;
			ViewModifyData();
			SetWindowText( _T("수 정") );
		}
	}	
}

VOID CMNG_CODE_MANAGER_10::InitSetControls()
{
	CComboBox *pCombo;
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("Y") );
	pCombo->InsertString( 1, _T("N") );
	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("Y") );
	pCombo->InsertString( 1, _T("N") );
	pCombo->SetCurSel( 1 );

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->SetWindowText( GetNextClassNum() );
	pEdt->EnableWindow( false );
}

CString CMNG_CODE_MANAGER_10::GetNextClassNum()
{
	CString num;
	CMNG_CODE_MANAGER_09 *pParent = ( CMNG_CODE_MANAGER_09 * )pParentMgr;
	num.Format( _T("%d"), pParent->GetNextClassNum() );

	return num;
}

VOID CMNG_CODE_MANAGER_10::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT dataCnt = 5;
	CString data[ dataCnt ];
	GetData( data );
	
	data[ 2 ].TrimLeft();
	data[ 2 ].TrimRight();
	if( !data[ 2 ].GetLength() )
	{
		AfxMessageBox( _T("코드 클래스 설명을 입력하십시오") );
		return;
	}

	CMNG_CODE_MANAGER_09 *pParent = ( CMNG_CODE_MANAGER_09 * )pParentMgr;
	
	if( 1 == m_type )
	{
		if( !pParent->SaveData( data, m_type ) )
			AfxMessageBox( _T("저장되었습니다.") );
	}
	else if( 2 == m_type )
	{
		if( !pParent->SaveData( data, m_type, m_checkedDMRow[ m_index ] ) )
			AfxMessageBox( _T("수정되었습니다.") );
	}		
	
	CDialog::OnOK();
}

VOID CMNG_CODE_MANAGER_10::GetData( CString data[] )
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( data[ 1 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->GetWindowText( data[ 2 ] );

	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->GetWindowText( data[ 3 ] );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->GetWindowText( data[ 4 ] );	
}

VOID CMNG_CODE_MANAGER_10::ShowGrid()
{
	CMNG_CODE_MANAGER_09 *pParent = ( CMNG_CODE_MANAGER_09 * )pParentMgr;
	pParent->ShowGrid();
}

INT CMNG_CODE_MANAGER_10::SetCheckedRowInfo()
{
	CMNG_CODE_MANAGER_09 *pParent = ( CMNG_CODE_MANAGER_09 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedRowInfo( m_checkedRowCnt );

	return m_checkedRowCnt;
}

VOID CMNG_CODE_MANAGER_10::ViewModifyData()
{
	CMNG_CODE_MANAGER_09 *pParent = ( CMNG_CODE_MANAGER_09 * )pParentMgr;
	
	const INT ctrlCnt = 4;
	CString data[ ctrlCnt ];
	pParent->GetCurrentData( data, m_checkedDMRow[ m_index ] );

	ShowCtrlData( data );
}

VOID CMNG_CODE_MANAGER_10::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	if( m_checkedRowCnt )
		free( m_checkedDMRow );
	
	CDialog::OnClose();
}

// 이전
VOID CMNG_CODE_MANAGER_10::OnBefore() 
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

// 다음
VOID CMNG_CODE_MANAGER_10::OnNext() 
{
	// TODO: Add your control notification handler code here

	if( m_index == m_checkedRowCnt - 1 )
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
	else
	{
		m_index++;
		ViewModifyData();
	}
}

VOID CMNG_CODE_MANAGER_10::ShowCtrlData( CString data[] )
{
	// 	_T("CLASS_CODE"), _T("CLASS_NAME"), _T("USE_YN"), _T("FIX_YN") };

	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->SetWindowText( data[ 0 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( data[ 1 ] );

	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	if( _T("Y") == data[ 2 ] )
		pCombo->SetCurSel( 0 );
	else if( _T("N") == data[ 2 ] )
		pCombo->SetCurSel( 1 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	if( _T("Y") == data[ 2 ] )
		pCombo->SetCurSel( 0 );
	else if( _T("N") == data[ 2 ] )
		pCombo->SetCurSel( 1 );
}
