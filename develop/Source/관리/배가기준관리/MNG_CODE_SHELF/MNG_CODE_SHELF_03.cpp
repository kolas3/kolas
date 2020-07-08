// MNG_CODE_SHELF_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_SHELF_03.h"

#include "MNG_CODE_SHELF_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_03 dialog


CMNG_CODE_SHELF_03::CMNG_CODE_SHELF_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_SHELF_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_CODE_SHELF_03::~CMNG_CODE_SHELF_03()
{	
	
}


BOOL CMNG_CODE_SHELF_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_SHELF_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_SHELF_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_SHELF_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_SHELF_03)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_mgrRDO_1, OnmgrRDO1)
	ON_BN_CLICKED(IDC_mgrRDO_2, OnmgrRDO2)
	ON_BN_CLICKED(IDC_mgrRDO_3, OnmgrRDO3)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_03 message handlers

BOOL CMNG_CODE_SHELF_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	 
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_SHELF_03") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_CODE_SHELF_03") );
		return false;
	}	
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_SHELF_03::OnFind() 
{
	// TODO: Add your control notification handler code here
	
	m_currentRadio = GetCurrentRadio();
	switch( m_currentRadio )
	{
		case 0 :
			AfxMessageBox( _T(" 등록, 별치, 분류 를 선택하여 주십시오") );
			return;
			break;
		case 1 :		// 등록
			SearchRecordData();
			break;
		case 2 :		// 별치
			SearchSepData();
			break;
		case 3 :		// 분류
			SearchClsData();
		break;
	}		

	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	pParent->ShowGrid( m_currentRadio );
}

INT CMNG_CODE_SHELF_03::GetCurrentRadio()
{
	CButton *pRdo;

	pRdo = ( CButton * )GetDlgItem( IDC_mgrRDO_1 );
	if( pRdo->GetCheck() )	// 등록
		return 1;

	pRdo = ( CButton * )GetDlgItem( IDC_mgrRDO_2 );
	if( pRdo->GetCheck() )	// 별치
		return 2;
	
	pRdo = ( CButton * )GetDlgItem( IDC_mgrRDO_3 );
	if( pRdo->GetCheck() )	// 분류
		return 3;
	
	return 0;
}

VOID CMNG_CODE_SHELF_03::OnmgrRDO1() 
{
	// TODO: Add your control notification handler code here
	
	SetRadioStat();
}

VOID CMNG_CODE_SHELF_03::OnmgrRDO2() 
{
	// TODO: Add your control notification handler code here
	
	SetRadioStat();
}

VOID CMNG_CODE_SHELF_03::OnmgrRDO3() 
{
	// TODO: Add your control notification handler code here
	
	SetRadioStat();
}


VOID CMNG_CODE_SHELF_03::SetRadioStat()
{
	bool stat[ 3 ] = { false, false, false };
	UINT ID[ 3 ] = { IDC_COMBO10, IDC_COMBO11, IDC_COMBO12 };

	m_currentRadio = GetCurrentRadio();
	stat[ m_currentRadio - 1 ] = true;

	CComboBox *pCombo;
	for( INT i = 0; i < 3; i++ )
	{
		pCombo = ( CComboBox * )GetDlgItem( ID[ i ] );
		pCombo->EnableWindow( stat[ i ] );
	}

	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	pParent->m_tab.SetCurSel( m_currentRadio - 1);
	pParent->ViewTab( m_currentRadio );
}

INT CMNG_CODE_SHELF_03::GetData(CString data[])
{
	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString alias[ 2 ] = {
		_T("발행형태"), _T("자료실") };
	
	for( INT i = 0; i < 2; i++ )
		data[ i ] = TakeControlData( alias[ i ] );
	//CString alias[ 3 ] = {
	//	_T("발행형태"), _T("관리구분"), _T("자료실") };
	//
	//for( INT i = 0; i < 3; i++ )
	//	data[ i ] = TakeControlData( alias[ i ] );
	//--2008.10.14 UPDATE BY PWR --}}

	return 0;
}

CString CMNG_CODE_SHELF_03::TakeControlData( CString alias, INT option )
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_CODE_SHELF_03_01"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_USER_MANAGER_01"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CMNG_USER_MANAGER_01"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

CString CMNG_CODE_SHELF_03::GetDefaultSelectQuery()
{
	CString where, tmp;

	//++2008.10.14 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString data[ 2 ];
	GetData( data );

	CString fieldName[ 2 ] = {
		_T("PUBLISH_FORM_CODE"), _T("SHELF_LOC_CODE") };

	for( INT i = 0; i < 2; i++ )
	{
		AllTrim( data[ i ] );
		
		if( data[ i ].GetLength() )
		{
			tmp.Format( _T(" %s = '%s' AND"), fieldName[ i ], data[ i ] );
			where += tmp;
		}
	}
	/*
	CString data[ 3 ];
	GetData( data );

	CString fieldName[ 3 ] = {
		_T("PUBLISH_FORM_CODE"), _T("MANAGE_CODE"), _T("SHELF_LOC_CODE") };

	for( INT i = 0; i < 3; i++ )
	{
		AllTrim( data[ i ] );
		
		if( data[ i ].GetLength() )
		{
			tmp.Format( _T(" %s = '%s' AND"), fieldName[ i ], data[ i ] );
			where += tmp;
		}
	}
	*/
	//--2008.10.14 UPDATE BY PWR --}}

	return where;
}

INT CMNG_CODE_SHELF_03::SearchSepData()
{
	CString where, tmp;
	where = GetDefaultSelectQuery();

	CString condition = TakeControlData( _T("별치") );
	
	AllTrim( condition );
	if( condition.GetLength() )
	{
		tmp.Format( _T(" SEPARATE_SHELF_CODE = '%s'"), condition );
		where += tmp;
	}

	DeleteAND( where );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_2") );
	pDM->RefreshDataManager( where );

	return 0;
}

// 등록구분별 자료 검색
VOID CMNG_CODE_SHELF_03::SearchRecordData()
{
	CString where, tmp;
	where = GetDefaultSelectQuery();

	CString condition = TakeControlData( _T("등록") );
	
	AllTrim( condition );
	if( condition.GetLength() )
	{
		tmp.Format( _T(" REG_CODE = '%s'"), condition );
		where += tmp;
	}
	
	DeleteAND( where );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );
	pDM->RefreshDataManager( where );
}


VOID CMNG_CODE_SHELF_03::SearchClsData()
{
	CString where, tmp;
	where = GetDefaultSelectQuery();

	CString condition = TakeControlData( _T("분류") );
	
	AllTrim( condition );
	if( condition.GetLength() )
	{
		tmp.Format( _T(" CLASS_NO_TYPE='%s'"), condition );
		where += tmp;
	}
	
	DeleteAND( where );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_3") );
	pDM->RefreshDataManager( where );
}

VOID CMNG_CODE_SHELF_03::AllTrim(CString &data)
{
	data.TrimLeft();
	data.TrimRight();
}

VOID CMNG_CODE_SHELF_03::DeleteAND( CString &where )
{
	CString tmp;
	if( _T("AND") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}
}

HBRUSH CMNG_CODE_SHELF_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

