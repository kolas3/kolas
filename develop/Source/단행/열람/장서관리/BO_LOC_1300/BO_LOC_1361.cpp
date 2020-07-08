// BO_LOC_1361.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1361.h"

#include "BO_LOC_1360.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1361 dialog


CBO_LOC_1361::CBO_LOC_1361( CESL_DataMgr *pDM, CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CBO_LOC_1361)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	pParentDM = pDM;
	m_index = 0;
}

BOOL CBO_LOC_1361::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	
	

VOID CBO_LOC_1361::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1361)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1361, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1361)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1361 message handlers

VOID CBO_LOC_1361::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		ShowDataInfo( 0 );
}

VOID CBO_LOC_1361::ShowDataInfo( INT flag )
{
	m_index += flag;

	CString windowText;
	windowText.Format( _T("지정자료 수정 %d/%d"), m_index + 1, m_checkedRow.GetSize() );
	this->SetWindowText( windowText );
	
	const INT ctrlCnt = 6;
	CString alias[ ctrlCnt ] = {
		_T("과목"), 
		_T("교수"), 
		_T("학과코드"), 
		_T("비고"), 
		_T("지정기간부터"), 
		_T("지정기간까지") }; 
	CString data[ ctrlCnt ];
	INT DMRow = m_checkedRow.GetAt( m_index );
	pParentDM->GetCellData( alias, ctrlCnt, data, DMRow );

	SetDlgItemText( IDC_eSUBJECT, data[ 0 ] );
	SetDlgItemText( IDC_ePROFESSOR, data[ 1 ] );
	SetDlgItemText( IDC_eREMARK, data[ 3 ] );
	SetControlData( _T("CM_BO_LOC_1361"), _T("학과"), data[ 2 ] );
	SetControlData( _T("CM_BO_LOC_1361"), _T("지정기간부터"), data[ 4 ] );
	SetControlData( _T("CM_BO_LOC_1361"), _T("지정기간까지"), data[ 5 ] );

	ShowMoveBtns();

	CBO_LOC_1360 *pParent = ( CBO_LOC_1360 * )pParentMgr;
	pParent->SetGridReverse( DMRow );
}

VOID CBO_LOC_1361::ShowMoveBtns()
{
	CButton *pBtn = NULL;
	pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
	if( !m_index )
		pBtn->EnableWindow( false );
	else
		pBtn->EnableWindow( true );

	pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
	if( m_checkedRow.GetSize() - 1 == m_index )
		pBtn->EnableWindow( false );
	else
		pBtn->EnableWindow( true );
}

BOOL CBO_LOC_1361::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("BO_LOC_1361") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error!") );
		return false;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1361::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	ShowDataInfo( -1 );
}

VOID CBO_LOC_1361::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	ShowDataInfo( 1 );
}

VOID CBO_LOC_1361::OnOK() 
{
	// TODO: Add extra validation here
	
	if( UpdateAssignInfo() < 0 )
	{
		AfxMessageBox( _T("Error : UpdateAssignInfo();... ") );
		return;
	}
	
	//CDialog::OnOK();
}

VOID CBO_LOC_1361::GetCtrlData( CString data[] )
{
	GetDlgItemText( IDC_eSUBJECT, data[ 0 ] );
	GetDlgItemText( IDC_ePROFESSOR, data[ 1 ] );
	GetDlgItemText( IDC_eREMARK, data[ 3 ] );
	GetControlData( _T("CM_BO_LOC_1361"), _T("학과"), data[ 2 ] );
	GetControlData( _T("CM_BO_LOC_1361"), _T("지정기간부터"), data[ 4 ] );
	GetControlData( _T("CM_BO_LOC_1361"), _T("지정기간까지"), data[ 5 ] );

	// 부모 DM 에 적용하기
	const INT cnt = 6;
	CString alias[ cnt ] = {
		_T("과목"), 
		_T("교수"), 
		_T("학과코드"), 
		_T("비고"), 
		_T("지정기간부터"), 
		_T("지정기간까지") }; 
			
	pParentDM->SetCellData( alias, cnt, data, m_checkedRow.GetAt( m_index ) );
}

INT CBO_LOC_1361::UpdateAssignInfo()
{
	const INT ctrlCnt = 6;
	CString data[ ctrlCnt ];
	GetCtrlData( data );

	INT idx = m_checkedRow.GetAt( m_index );
	CString query;
	
	CString assignObjectKey = pParentDM->GetCellData( _T("지정대상정보KEY"), idx );
	CString assignInfoKey = pParentDM->GetCellData( _T("지정정보KEY"), idx );
	
	if( pParentDM->StartFrame() < 0 )
	{
		AfxMessageBox( _T("Error : StartFrame...") );
		return -1;
	}
	pParentDM->InitDBFieldData();

	MakeAssignInfoQuery( data, assignInfoKey );
	MakeAssignObjectQuery( data, assignObjectKey );

	if( pParentDM->SendFrame() < 0 )
	{
		AfxMessageBox( _T("Error : SendFrame...") );
		return -1;
	}
	
	if( pParentDM->EndFrame() < 0 )
	{
		AfxMessageBox( _T("Error : EndFrame...") );
		return -1;
	}
	
	AfxMessageBox( _T("수정되었습니다.") );
	return 0;
}

INT CBO_LOC_1361::MakeAssignInfoQuery( CString data[], CString key )
{
	pParentDM->AddDBFieldData( _T("ASSIGN_START_DATE"), _T("STRING"), data[ 4 ] );
	pParentDM->AddDBFieldData( _T("ASSIGN_END_DATE"),	_T("STRING"), data[ 5 ] );
	pParentDM->AddDBFieldData( _T("LAST_WORK"),			_T("STRING"), GetWorkLogMsg( _T("지정정보수정"), __WFILE__, __LINE__ ) );

	pParentDM->MakeUpdateFrame( _T("CO_ASSIGN_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), key );

	return 0;
}

INT CBO_LOC_1361::MakeAssignObjectQuery( CString data[], CString key )
{
	CIndex index;
	pParentDM->AddDBFieldData( _T("SUBJECT_NAME"),		 _T("STRING"), data[ 0 ] );
	index.MakeIndex( data[ 0 ] );
	pParentDM->AddDBFieldData( _T("IDX_SUBJECT_NAME"),	 _T("STRING"), data[ 0 ] );
	pParentDM->AddDBFieldData( _T("PROFESSOR_NAME"),	 _T("STRING"), data[ 1 ] );
	index.MakeIndex( data[ 1 ] );
	pParentDM->AddDBFieldData( _T("IDX_PROFESSOR_NAME"), _T("STRING"), data[ 1 ] );
	pParentDM->AddDBFieldData( _T("COURSE_CODE"),		 _T("STRING"), data[ 2 ] );
	pParentDM->AddDBFieldData( _T("REMARK"),			 _T("STRING"), data[ 3 ] );
	pParentDM->AddDBFieldData( _T("LAST_WORK"),			 _T("STRING"), GetWorkLogMsg( _T("지정정보수정"), __WFILE__, __LINE__ ) );

	pParentDM->MakeUpdateFrame( _T("CO_ASSIGN_OBJECT_TBL"), _T("REC_KEY"), _T("NUMERIC"), key );
	
	return 0;
}
