// MNG_CODE_SHELF_04.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_SHELF_04.h"

#include "MNG_CODE_SHELF_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_04 dialog


CMNG_CODE_SHELF_04::CMNG_CODE_SHELF_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_SHELF_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CODE_SHELF_04::~CMNG_CODE_SHELF_04()
{	
	if( m_checkedDMRow != NULL )
		free( m_checkedDMRow );
	m_checkedDMRow = NULL;
}

BOOL CMNG_CODE_SHELF_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_SHELF_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_SHELF_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_SHELF_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_SHELF_04)
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_rTYPE_1, OnrTYPE1)
	ON_BN_CLICKED(IDC_rTYPE_2, OnrTYPE2)
	ON_BN_CLICKED(IDC_rTYPE_3, OnrTYPE3)
	ON_WM_SHOWWINDOW()	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_04 message handlers

BOOL CMNG_CODE_SHELF_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_SHELF_04") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_CODE_SHELF_04") );
		return false;
	}
	
	m_type = 0;
	m_checkedDMRow = NULL;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 저장
VOID CMNG_CODE_SHELF_04::OnOK() 
{
	// TODO: Add extra validation here
	
	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString data[ 7 ];
	//CString data[ 8 ];
	//--2008.10.14 UPDATE BY PWR --}}
	GetData( data );

	INT ids;
	switch( m_type )
	{
		case 1 :		// 등록
			ids = SaveRecordData( data );
			break;
		case 2 :
			ids = SaveSepData( data );
			break;
		case 3 :
			ids = SaveClsData( data );
			break;
		default :
			AfxMessageBox( _T(" 등록, 별치, 분류를 선택하여 주십시오") );
			return;
			break;
	}
	
	if( -1 == ids )
		return;
	
	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	pParent->ShowGrid( m_type );
	
	if( 1 == m_mode )
	{
		AfxMessageBox( _T("저장되었습니다.") );
		CDialog::OnOK();
	}
	else
	{
		CString cmAlias[ 3 ] = {_T("등록"), _T("별치"), _T("분류")};
		SetReverse(_T("CM_MNG_CODE_SHELF_01_01"), cmAlias[ m_type - 1 ], m_checkedDMRow[ m_index ] );
		AfxMessageBox( _T("수정되었습니다.") );
		// 040318_HSS 수정 후 닫기 - redisplay되기 때문에 선정내용 날라감 -> 닫는 것이 깔끔
		CDialog::OnOK();
	}
}

// 이전
VOID CMNG_CODE_SHELF_04::OnBefore() 
{
	// TODO: Add your control notification handler code here
	m_index--;
	
	if( m_index < 0 )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		m_index = 0;
		return;
	}
	else
		ViewModifyData();

}

// 다음
VOID CMNG_CODE_SHELF_04::OnNext() 
{
	// TODO: Add your control notification handler code here

	m_index++;

	if( m_index > m_rowCnt - 1 )
	{
		AfxMessageBox( _T("마지막 목록입니다.") );
		m_index = m_rowCnt - 1;
		return;
	}
	else
		ViewModifyData();
	
}

VOID CMNG_CODE_SHELF_04::OnrTYPE1() 
{
	// TODO: Add your control notification handler code here
	
	SetControlStat();
}

VOID CMNG_CODE_SHELF_04::OnrTYPE2() 
{
	// TODO: Add your control notification handler code here
	
	SetControlStat();
}

VOID CMNG_CODE_SHELF_04::OnrTYPE3() 
{
	// TODO: Add your control notification handler code here
	
	SetControlStat();
}

VOID CMNG_CODE_SHELF_04::SetControlStat( INT type )
{	
	UINT ID[ 5 ] = { IDC_cINPUT_4, IDC_cINPUT_5, IDC_cINPUT_6, IDC_eINPUT_7, IDC_eINPUT_8 };
	bool flag[ 5 ] = { false, false, false, false, false };

	if( !type )
		type = GetSelectedRadio();

	m_type = type;
	
	switch( type )
	{
		case 1 :	// 등록
			flag[ 0 ] = true;			
			break;
		case 2 :	// 별치
			flag[ 1 ] = true;
			break;
		case 3 :	// 분류
			flag[ 2 ] = true;
			flag[ 3 ] = true;
			flag[ 4 ] = true;
			break;
	}

	CWnd *pWnd;
	for( INT i = 0; i < 5; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( ID[ i ] );
		pWnd->EnableWindow( flag[ i ] );
	}

	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	pParent->m_tab.SetCurSel( type - 1 );
	pParent->ViewTab( type );
}

INT CMNG_CODE_SHELF_04::GetSelectedRadio()
{
	CButton *pRdo;
	UINT ID[ 3 ] = { IDC_rTYPE_1, IDC_rTYPE_2, IDC_rTYPE_3 };

	for( INT i = 0; i < 3; i++ )
	{
		pRdo = ( CButton * )GetDlgItem( ID[ i ] );
		if( pRdo->GetCheck() )
			return i + 1;
	}

	return 0;
}

INT CMNG_CODE_SHELF_04::GetData( CString data[] )
{
	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString alias[ 7 ] = {
		_T("발행형태구분"), _T("자료실"), _T("등록구분"), _T("별치기호"), _T("분류표구분"), _T("분류기호_부터"), _T("분류기호_까지") };

	for( INT i = 0; i < 7; i++ )
		data[ i ] = TakeControlData( alias[ i ] );
	/*
	CString alias[ 8 ] = {
		_T("발행형태구분"), _T("관리구분"), _T("자료실"), _T("등록구분"), _T("별치기호"), _T("분류표구분"), _T("분류기호_부터"), _T("분류기호_까지") };

	for( INT i = 0; i < 8; i++ )
		data[ i ] = TakeControlData( alias[ i ] );
	*/
	//--2008.10.14 UPDATE BY PWR --}}
	
	return 0;
}

CString CMNG_CODE_SHELF_04::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_CODE_SHELF_04_01"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_CODE_SHELF_04_01"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MNG_CODE_SHELF_04_01"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

INT CMNG_CODE_SHELF_04::SaveRecordData( CString data[] )
{
	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),				//  0
		_T("MANAGE_CODE"),			//  1
		_T("REG_CODE"),				//  2
		_T("SHELF_LOC_CODE"),		//	3
		_T("PUBLISH_FORM_CODE"),	//	4
		_T("FIRST_WORK"),			//  5
		_T("LAST_WORK") };			//	6

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], m_pInfo->MANAGE_CODE, data[ 2 ] ) )
	{
		MessageBox( _T("중복된 데이타가 존재합니다."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}
		
	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );

	realData[ 1 ] = m_pInfo->MANAGE_CODE;
	realData[ 2 ] = data[ 2 ];
	realData[ 3 ] = data[ 1 ];
	realData[ 4 ] = data[ 0 ];
	/*
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),				//  0
		_T("MANAGE_CODE"),			//  1
		_T("REG_CODE"),				//  2
		_T("SHELF_LOC_CODE"),		//	3
		_T("PUBLISH_FORM_CODE"),	//	4
		_T("FIRST_WORK"),			//  5
		_T("LAST_WORK") };			//	6

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], data[ 1 ], data[ 3 ] ) )
	{
		MessageBox( _T("중복된 데이타가 존재합니다."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}
		
	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );

	realData[ 1 ] = data[ 1 ];
	realData[ 2 ] = data[ 3 ];
	realData[ 3 ] = data[ 2 ];
	realData[ 4 ] = data[ 0 ];
	*/
	//--2008.10.14 UPDATE BY PWR --}}
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );

	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("MN_REG_CODE_SHELF_RULE_MNG_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, realData, -1 );
	}
	else if( 2 == m_mode )
	{
		pDM->MakeUpdateFrame( _T("MN_REG_CODE_SHELF_RULE_MNG_TBL"), _T("REC_KEY"), _T("NUMERIC"), realData[ 0 ], m_checkedDMRow[ m_index ] );
		pDM->SetCellData( fieldName, fieldCnt, realData, m_checkedDMRow[ m_index ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();
			
	return 0;
}

INT CMNG_CODE_SHELF_04::SaveSepData(CString data[])
{
	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),
		_T("MANAGE_CODE"),
		_T("SEPARATE_SHELF_CODE"),
		_T("SHELF_LOC_CODE"),
		_T("PUBLISH_FORM_CODE"),
		_T("FIRST_WORK"),
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_2") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], m_pInfo->MANAGE_CODE, data[ 3 ] ) )
	{
		MessageBox( _T("중복된 데이타가 존재합니다."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}

	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	
	realData[ 1 ] = m_pInfo->MANAGE_CODE;
	realData[ 2 ] = data[ 3 ];
	realData[ 3 ] = data[ 1 ];
	realData[ 4 ] = data[ 0 ];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );
	/*
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),
		_T("MANAGE_CODE"),
		_T("SEPARATE_SHELF_CODE"),
		_T("SHELF_LOC_CODE"),
		_T("PUBLISH_FORM_CODE"),
		_T("FIRST_WORK"),
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_2") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], data[ 1 ], data[ 4 ] ) )
	{
		MessageBox( _T("중복된 데이타가 존재합니다."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}

	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	
	realData[ 1 ] = data[ 1 ];
	realData[ 2 ] = data[ 4 ];
	realData[ 3 ] = data[ 2 ];
	realData[ 4 ] = data[ 0 ];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );
	*/
	//--2008.10.14 UPDATE BY PWR --}}
	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("MN_SEP_CODE_SHELF_RULE_MNG_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, realData, -1 );
	}
	else if( 2 == m_mode )
	{
		pDM->MakeUpdateFrame( _T("MN_SEP_CODE_SHELF_RULE_MNG_TBL"), _T("REC_KEY"), _T("NUMERIC"), realData[ 0 ],  m_checkedDMRow[ m_index ] );
		pDM->SetCellData( fieldName, fieldCnt, realData, m_checkedDMRow[ m_index ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

INT CMNG_CODE_SHELF_04::SaveClsData(CString data[])
{
	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	const INT fieldCnt = 9;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), 
		_T("MANAGE_CODE"), 
		_T("CLASS_NO_TYPE"), 
		_T("START_CLASS_NO"), 
		_T("END_CLASS_NO"), 
		_T("PUBLISH_FORM_CODE"), 
		_T("SHELF_LOC_CODE"), 
		_T("FIRST_WORK"), 
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_3") );

	CString realData[ fieldCnt ];
	
	if( 1 == m_mode )
	{
		
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 7 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 8 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );

	realData[ 1 ] = m_pInfo->MANAGE_CODE;
	realData[ 2 ] = data[ 4 ];
	realData[ 3 ] = data[ 5 ];
	realData[ 4 ] = data[ 6 ];
	realData[ 5 ] = data[ 0 ];
	realData[ 6 ] = data[ 1 ];
	/*
	const INT fieldCnt = 9;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), 
		_T("MANAGE_CODE"), 
		_T("CLASS_NO_TYPE"), 
		_T("START_CLASS_NO"), 
		_T("END_CLASS_NO"), 
		_T("PUBLISH_FORM_CODE"), 
		_T("SHELF_LOC_CODE"), 
		_T("FIRST_WORK"), 
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_3") );

	CString realData[ fieldCnt ];
	
	
	//if( CheckOverlapData( data[ 0 ], data[ 1 ], data[ 5 ] ) )
	//{
	//	MessageBox( _T("중복된 데이타가 존재합니다."), _T("KOLAS 2 Upgrade"), MB_OK );
	//	return -1;
	//}

	if( 1 == m_mode )
	{
		
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 7 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 8 ] = GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ );

	realData[ 1 ] = data[ 1 ];
	realData[ 2 ] = data[ 5 ];
	realData[ 3 ] = data[ 6 ];
	realData[ 4 ] = data[ 7 ];
	realData[ 5 ] = data[ 0 ];
	realData[ 6 ] = data[ 2 ];
	*/
	//--2008.10.14 UPDATE BY PWR --}}

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );

	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("MN_CLS_NUM_SHELF_RULE_MNG_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, realData, -1 );
	}
	else if( 2 == m_mode )
	{
		pDM->MakeUpdateFrame( _T("MN_CLS_NUM_SHELF_RULE_MNG_TBL"), _T("REC_KEY"), _T("NUMERIC"), realData[ 0 ],  m_checkedDMRow[ m_index ] );
		pDM->SetCellData( fieldName, fieldCnt, realData, m_checkedDMRow[ m_index ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

VOID CMNG_CODE_SHELF_04::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	m_index = 0;
	if( m_checkedDMRow != NULL )
		free( m_checkedDMRow );
	m_checkedDMRow = NULL;
	m_rowCnt = 0;

	CButton *pBtn;
	INT i;
	UINT rdoID[ 3 ] = { IDC_rTYPE_1, IDC_rTYPE_2, IDC_rTYPE_3 };

	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	
	switch( m_mode )
	{
	case 1 :		// 입력
		
		pBtn = ( CButton * )GetDlgItem( IDC_BEFORE );
		pBtn->ShowWindow( false );
		pBtn = ( CButton * )GetDlgItem( IDC_NEXT );
		pBtn->ShowWindow( false );

		SetWindowText( _T("입력") );

		break;

	case 2 :		// 수정 
	
		m_type = pParent->m_tab.GetCurSel() + 1;
		pBtn = ( CButton * )GetDlgItem( rdoID[ m_type - 1 ] );

		m_checkedDMRow = pParent->GetCheckedDMRow( m_rowCnt, m_type );

		SetWindowText( _T("수정") );

		if( !m_rowCnt )
		{
			AfxMessageBox( _T("선택목록이 없습니다") );
			CDialog::EndDialog( 1 );
			return;
		}
		else
		{
			for( i = 0; i < 3; i++ )
			{
				pBtn = ( CButton * )GetDlgItem( rdoID[ i ] );
				
				if( m_type - 1 == i )
				{
					pBtn->EnableWindow( true );
					pBtn->SetCheck( true );
					SetControlStat();
				}
				else
				{
					pBtn->EnableWindow( false );
					pBtn->SetCheck( false );
				}
			}				

			ViewModifyData();
		}

		break;
	}	
}

VOID CMNG_CODE_SHELF_04::ViewModifyData()
{
	CESL_DataMgr *pDM;
	CString dmAlias[ 3 ] = {
		_T("DM_MNG_CODE_SHELF_01_1"), _T("DM_MNG_CODE_SHELF_01_2"), _T("DM_MNG_CODE_SHELF_01_3") };

	// 040318_HSS 그리드 Alias 추가
	CString cmAlias[ 3 ] = {
		_T("등록"), _T("별치"), _T("분류") };

	pDM = FindDM( dmAlias[ m_type - 1 ] );

	const INT ctrlCnt = 2;
	CString ctrlAlias[ ctrlCnt ] = {
		_T("PUBLISH_FORM_CODE"), _T("SHELF_LOC_CODE") };

	CString ctrlData[ ctrlCnt ];
	pDM->GetCellData( ctrlAlias, ctrlCnt, ctrlData, m_checkedDMRow[ m_index ] );

	ctrlAlias[ 0 ] = _T("발행형태구분");
	ctrlAlias[ 1 ] = _T("자료실");

	for( INT i = 0; i < 2; i++ )
		SendControlData( ctrlAlias[ i ], ctrlData[ i ] );

	// 040318_HSS 기본값 넣어주기 +++++++++++++++++++++++++++
	//발행형태구분
	m_DefaultValue[0] = ctrlData[0];
	//관리구분
	m_DefaultValue[1] = m_pInfo->MANAGE_CODE;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

	switch( m_type )
	{
		case 1 :

			ctrlAlias[ 0 ] = _T("REG_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("등록구분");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS 기본값 넣어주기 - 등록구분
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 2 :

			ctrlAlias[ 0 ] = _T("SEPARATE_SHELF_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("별치기호");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS 기본값 넣어주기 - 별치기호
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 3 :

			CString ctrlAlias1[ 3 ] = {
				_T("CLASS_NO_TYPE"), _T("START_CLASS_NO"), _T("END_CLASS_NO") };

			pDM->GetCellData( ctrlAlias1, 3, ctrlData, m_checkedDMRow[ m_index ] );

			ctrlAlias1[ 0 ] = _T("분류표구분");
			ctrlAlias1[ 1 ] = _T("분류기호_부터");
			ctrlAlias1[ 2 ] = _T("분류기호_까지");

			// 040318_HSS 기본값 넣어주기 - 분류표구분
			m_DefaultValue[2] = ctrlData[ 0 ];

			for( i = 0; i < 3; i++ )
				SendControlData( ctrlAlias1[ i ], ctrlData[ i ] );

			break;
	}
	/*
	CESL_DataMgr *pDM;
	CString dmAlias[ 3 ] = {
		_T("DM_MNG_CODE_SHELF_01_1"), _T("DM_MNG_CODE_SHELF_01_2"), _T("DM_MNG_CODE_SHELF_01_3") };

	// 040318_HSS 그리드 Alias 추가
	CString cmAlias[ 3 ] = {
		_T("등록"), _T("별치"), _T("분류") };

	pDM = FindDM( dmAlias[ m_type - 1 ] );

	const INT ctrlCnt = 3;
	CString ctrlAlias[ ctrlCnt ] = {
		_T("PUBLISH_FORM_CODE"), _T("MANAGE_CODE"), _T("SHELF_LOC_CODE") };

	CString ctrlData[ ctrlCnt ];
	pDM->GetCellData( ctrlAlias, ctrlCnt, ctrlData, m_checkedDMRow[ m_index ] );

	ctrlAlias[ 0 ] = _T("발행형태구분");
	ctrlAlias[ 1 ] = _T("관리구분");
	ctrlAlias[ 2 ] = _T("자료실");

	for( INT i = 0; i < 3; i++ )
		SendControlData( ctrlAlias[ i ], ctrlData[ i ] );

	// 040318_HSS 기본값 넣어주기 +++++++++++++++++++++++++++
	//발행형태구분
	m_DefaultValue[0] = ctrlData[0];
	//관리구분
	m_DefaultValue[1] = ctrlData[1];
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

	switch( m_type )
	{
		case 1 :

			ctrlAlias[ 0 ] = _T("REG_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("등록구분");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS 기본값 넣어주기 - 등록구분
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 2 :

			ctrlAlias[ 0 ] = _T("SEPARATE_SHELF_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("별치기호");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS 기본값 넣어주기 - 별치기호
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 3 :

			ctrlAlias[ 0 ] = _T("CLASS_NO_TYPE");
			ctrlAlias[ 1 ] = _T("START_CLASS_NO");
			ctrlAlias[ 2 ] = _T("END_CLASS_NO");
			pDM->GetCellData( ctrlAlias, 3, ctrlData, m_checkedDMRow[ m_index ] );

			ctrlAlias[ 0 ] = _T("분류표구분");
			ctrlAlias[ 1 ] = _T("분류기호_부터");
			ctrlAlias[ 2 ] = _T("분류기호_까지");

			// 040318_HSS 기본값 넣어주기 - 분류표구분
			m_DefaultValue[2] = ctrlData[ 0 ];

			for( i = 0; i < 3; i++ )
				SendControlData( ctrlAlias[ i ], ctrlData[ i ] );

			break;
	}
	*/

	SetReverse(_T("CM_MNG_CODE_SHELF_01_01"), cmAlias[ m_type - 1 ], m_checkedDMRow[ m_index ] );
}

INT CMNG_CODE_SHELF_04::SendControlData( CString alias, CString data )
{
	SetControlData( _T("CM_MNG_CODE_SHELF_04_01"), alias, data, -1, 0 );

	return 0;
}

INT CMNG_CODE_SHELF_04::CheckOverlapData( CString publishFormCode, CString manageCode, CString expData )
{
	const INT cnt = 3;
	CString table[ cnt ] = {
		_T("MN_REG_CODE_SHELF_RULE_MNG_TBL"), 
		_T("MN_SEP_CODE_SHELF_RULE_MNG_TBL"),
		_T("MN_CLS_NUM_SHELF_RULE_MNG_TBL") };
	
	CString expField[ cnt ] = {
		_T("REG_CODE"), _T("SEPARATE_SHELF_CODE"), _T("CLASS_NO_TYPE") };

	// 040318_HSS 처음 입력 데이터랑 다르지 않은 경우 리턴 
	//	-> 안그러면 자료실만 수정할 수가 없음
	if (m_DefaultValue[0] == publishFormCode		// 발행형태구분
		&& m_DefaultValue[1] == manageCode			// 관리구분
		&& m_DefaultValue[2] == expData)			// 등록/별치/분류표구분
		return 0;
	
	CString query, result;
	query.Format( _T("SELECT COUNT( * ) FROM %s WHERE PUBLISH_FORM_CODE = '%s' AND MANAGE_CODE = '%s' AND %s = '%s'"),
		table[ m_type - 1 ], publishFormCode, manageCode, expField[ m_type - 1 ], expData );
			
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );
	pDM->GetOneValueQuery( query, result );
		
	return _ttoi( result );
}
HBRUSH CMNG_CODE_SHELF_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
