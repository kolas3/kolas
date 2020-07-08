// CLASS_SIGN_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "CLASS_SIGN_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_01 dialog


CCLASS_SIGN_MANAGER_01::CCLASS_SIGN_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLASS_SIGN_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CCLASS_SIGN_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CCLASS_SIGN_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLASS_SIGN_MANAGER_01)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLASS_SIGN_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CCLASS_SIGN_MANAGER_01)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_01 message handlers

BOOL CCLASS_SIGN_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitCtrlSetting();
	OnSelchangeCombo1();
	SetScreenType();
	
	if( InitESL_Mgr( _T("MNG_CCLASS_SIGN_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CCLASS_SIGN_MANAGER_01") );
		return false;
	}

	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	
	CESL_Grid *pGrid;

	const INT gridCnt = 3;
	CString gridAlias[ gridCnt ] = {
		_T("그리드_1"), _T("그리드_2"), _T("그리드_3") };

	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid =  ( CESL_Grid * )FindControl( _T("CM_MNG_KOMARC_MANAGER_01"), gridAlias[ i ] );
		
		if( pGrid )
		{
			//RemoveListArray.Add( _T("") );

			pGrid->InitPopupMenuItem();
			pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
		}
	}

	m_a.Format( _T("%ca"), 31 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCLASS_SIGN_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
//DEL	InitCtrlSetting();
//DEL	OnSelchangeCombo1();
//DEL	SetScreenType();	
}

VOID CCLASS_SIGN_MANAGER_01::InitCtrlSetting()
{
	CComboBox * pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("KDC") );
	pCombo->InsertString( 1, _T("DDC") );
	pCombo->InsertString( 2, _T("색인어") );
	pCombo->SetCurSel( 0 );
}

// 구분 콤보가 변경 될때.
VOID CCLASS_SIGN_MANAGER_01::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	SetScreenType();
	m_edit.SetWindowText( _T("") );
	
	m_KDCrow.RemoveAll();
	m_DDCrow.RemoveAll();

	ShowGrid();
}

INT CCLASS_SIGN_MANAGER_01::GetCurrentType()
{
	CComboBox * pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	
	return pCombo->GetCurSel();
}

INT CCLASS_SIGN_MANAGER_01::SetScreenType( INT option )
{
	CComboBox * pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	
	if( -1 == option )
		m_type = GetCurrentType();
	else
	{
		m_type = option;
		pCombo->SetCurSel( m_type );
	}
	// 0 : KDC
	// 1 : DDC
	// 2 : 색인어

	const INT gridCnt = 3;
	CMSHFlexGrid *pGrid;
	UINT gridID[ gridCnt ] = { IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3 };
	bool showFlag[ gridCnt ] = { false, false, false };
	showFlag[ m_type ] = true;

	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ i ] );
		pGrid->ShowWindow( showFlag[ i ] );
	}
	
	return 0;
}

INT CCLASS_SIGN_MANAGER_01::SearchData( INT type, CString keyword, CString keyword2 )
{
	m_type = type;
	
	CESL_DataMgr *pDM;
	switch( m_type )
	{
		case 0 :
			pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_KDC") );
			break;
		case 1 :
			pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_DDC") );
			break;
		case 2 :
			pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_IDX") );
			break;
		default :
			return -1;
			break;
	}

	CString where;
	if( 2 != m_type )
	{
		if ( keyword.GetLength() && keyword2.GetLength() )
		{
			where.Format( _T("CLASS_VAL BETWEEN '%s' AND '%s'"), keyword, keyword2 );
		}
		else if ( keyword.GetLength() )
		{
			where.Format( _T("CLASS_VAL LIKE '%s%%'"), keyword );
		}
		else if ( keyword2.GetLength() )
		{
			where.Format( _T("CLASS_VAL LIKE '%s%%'"), keyword2 );
		}
		
		where += _T(" ORDER BY CLASS_VAL ");
	}
	else if( keyword.GetLength() && 2 == m_type )
	{
		where.Format( _T("I.INDEX_WORD LIKE '%%%s%%'"), keyword );	
		where += _T(" ORDER BY I.INDEX_WORD ");
	}
	
	pDM->RefreshDataManager( where );

	
	INT rowCnt = pDM->GetRowCount();
	if( !rowCnt )
		AfxMessageBox( _T("검색 결과가 없습니다.") );
	
	ShowGrid();

	m_KDCrow.RemoveAll();
	m_DDCrow.RemoveAll();

	return 0;
}

VOID CCLASS_SIGN_MANAGER_01::ShowGrid( INT option )
{
	if( -1 != option )
		m_type = option;

	CString gridAlias;
	switch( m_type )
	{
		case 0 :
			gridAlias = _T("그리드_1");
			break;
		case 1 :
			gridAlias = _T("그리드_2");
			break;
		case 2 :
			gridAlias = _T("그리드_3");
			break;
		default :
			return;
			break;
	}

	ControlDisplay( _T("CM_CCLASS_SIGN_MANAGER_01"), gridAlias );
}

BEGIN_EVENTSINK_MAP(CCLASS_SIGN_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CCLASS_SIGN_MANAGER_01)
//	ON_EVENT(CCLASS_SIGN_MANAGER_01, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
//	ON_EVENT(CCLASS_SIGN_MANAGER_01, IDC_mngGRID_2, -600 /* Click */, OnClickmngGRID2, VTS_NONE)
	ON_EVENT(CCLASS_SIGN_MANAGER_01, IDC_mngGRID_3, -600 /* Click */, OnClickmngGRID3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT * CCLASS_SIGN_MANAGER_01::GetCheckedDMRow( INT &rowCnt, INT option )
{
	INT *checkedDMRow;
	rowCnt = 0;
	INT type = 0;

	if( -1 == option )
		type = m_type;
	else
		type = option;

	CMSHFlexGrid *pGrid;
	switch( type )
	{
		case 0 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
			break;
		case 1 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
			break;
		case 2 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_3 );
			break;
	}

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return NULL;

	INT i;
	INT totalRowCnt = pGrid->GetRows();
	
	pGrid->SetCol( 1 );
	for( i = 1; i < totalRowCnt; i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			rowCnt++;
	}

	if( !rowCnt )
		return NULL;
	
	checkedDMRow = ( INT * )calloc( rowCnt, sizeof( INT ) );

	INT index = 0;
	for( i = 1; i < totalRowCnt; i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			checkedDMRow[ index++ ] = i - 1;
	}

	return checkedDMRow;
}

CString CCLASS_SIGN_MANAGER_01::GetKDCTagString( INT *checkedDMRow, INT rowCnt )
{
	CString tagString, buf;
	tagString = _T("056  ");

	CESL_DataMgr *pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_KDC") );

	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("CLASS_VAL"), i, buf );
		tagString = tagString + _T("%a") + buf;
	}

	return tagString;
}

VOID CCLASS_SIGN_MANAGER_01::Set3rdGridCheck()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_3 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 2 != col && 4 != col )
		return;

	if( _T("V") == pGrid->GetText() )
		pGrid->SetText( _T("") );
	else
		pGrid->SetText( _T("V") );
}


VOID CCLASS_SIGN_MANAGER_01::GetSelectedCell( INT &row, INT &col, INT option )
{
	const INT gridCnt = 3;
	CMSHFlexGrid *pGrid;
	UINT gridID[ gridCnt ] = { IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3, };
	
	if( -1 == option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ m_type ] );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ option ] );
	
	row = pGrid->GetRow();
	col = pGrid->GetCol();
}


CString CCLASS_SIGN_MANAGER_01::GetCellText( INT row, INT col, INT option )
{
	const INT gridCnt = 3;
	CMSHFlexGrid *pGrid;
	UINT gridID[ gridCnt ] = { IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3, };
	
	if( -1 == option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ m_type ] );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ option ] );

	INT _row = pGrid->GetRow();
	INT _col = pGrid->GetCol();

	pGrid->SetRow( row );
	pGrid->SetCol( col );

	CString text = pGrid->GetText();

	pGrid->SetRow( _row );
	pGrid->SetCol( _col );

	return text;
}

// 색인어 GRID
VOID CCLASS_SIGN_MANAGER_01::OnClickmngGRID3() 
{
	// TODO: Add your control notification handler code here

	Set3rdGridCheck();

	INT row, col;
	GetSelectedCell( row, col, 2 );
	if( 2 != col && 4 != col )
		return;

	if( _T("V") == GetCellText( row, col, 2 ) )
	{
		if( 2 == col )
			m_KDCrow.Add( row );
		else if( 4 == col )
			m_DDCrow.Add( row );
	}
	else
	{
		if( 2 == col )
			m_KDCrow.RemoveAt( GetKDCArrayAt( row ) );
		else if( 4 == col )
			m_DDCrow.RemoveAt( GetDDCArrayAt( row ) );
	}
	
	m_edit.SetWindowText( GetKDCTag( 2 ) + _T("\r\n") + GetDDCTag( 2 ) );	
}

CString CCLASS_SIGN_MANAGER_01::GetDDCTag( INT option )
{
	CESL_DataMgr *pDM;
	if( 2 == option )
		pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_IDX") );
	else
		pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_DDC") );
	
	CString tag = _T("082  ");
	CString tmp;

	INT rowCnt = m_DDCrow.GetSize();
	if( !rowCnt )
		return _T("");

	for( INT i = 0; i < rowCnt; i++ )
	{	
		if( 2 == option )
			pDM->GetCellData( _T("I_DDC"), m_DDCrow.GetAt( i ) - 1, tmp );
		else
			pDM->GetCellData( _T("CLASS_VAL"), m_DDCrow.GetAt( i ) - 1, tmp );

		tag = tag + m_a + tmp;
	}	
	
	return tag;		
}

CString CCLASS_SIGN_MANAGER_01::GetKDCTag( INT option )
{
	CESL_DataMgr *pDM;
	if( 2 == option )
		pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_IDX") );
	else
		pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_KDC") );
	
	CString tag = _T("056  ");
	CString tmp;

	INT rowCnt = m_KDCrow.GetSize();
	if( !rowCnt )
		return _T("");

	for( INT i = 0; i < rowCnt; i++ )
	{
		if( 2 == option )
			pDM->GetCellData( _T("I_KDC"), m_KDCrow.GetAt( i ) - 1, tmp );
		else
			pDM->GetCellData( _T("CLASS_VAL"), m_KDCrow.GetAt( i ) - 1, tmp );

		tag = tag + m_a + tmp;
	}	
	
	return tag;
}

INT CCLASS_SIGN_MANAGER_01::GetKDCArrayAt( INT row )
{
	for( INT i = 0; i < m_KDCrow.GetSize(); i++ )
		if( row == m_KDCrow.GetAt( i ) )
			return i;

	return -1;
}

INT CCLASS_SIGN_MANAGER_01::GetDDCArrayAt(INT row)
{
	for( INT i = 0; i < m_DDCrow.GetSize(); i++ )
		if( row == m_DDCrow.GetAt( i ) )
			return i;

	return -1;
}


VOID CCLASS_SIGN_MANAGER_01::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	/*CMNG_CLASS_SIGN_POPUP_01 dlg( this );
	if( IDOK == dlg.DoModal() )
	{
		AfxMessageBox( dlg.m_KDCtag );
		AfxMessageBox( dlg.m_DDCtag );
	}*/
}


INT * CCLASS_SIGN_MANAGER_01::GetCheckedIDXRow( INT &cnt_1, INT &cnt_2 )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_3 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	INT * checkedRow;
	INT rowCnt = pGrid->GetRows();
	cnt_1 = 0; 
	cnt_2 = 0;

	for( INT i = 2; i < 5; i += 2 )
	{
		pGrid->SetCol( i );

		for( INT j = 1; j < rowCnt; j++ )
		{
			pGrid->SetRow( j );

			if( _T("V") == pGrid->GetText() )
			{
				if( 2 == i )
					cnt_1++;
				else if( 4 == i )
					cnt_2++;
			}
		}
	}

	checkedRow = ( INT * )calloc( cnt_1 + cnt_2, sizeof( INT ) );

	INT index = 0;
	for( i = 2; i < 5; i += 2 )
	{
		pGrid->SetCol( i );

		for( INT j = 1; j < rowCnt; j++ )
		{
			pGrid->SetRow( j );

			if( _T("V") == pGrid->GetText() )
				checkedRow[ index++ ] = j - 1;
		}
	}

	pGrid->SetRow( row );
	pGrid->SetCol( col );

	return checkedRow;
}

VOID CCLASS_SIGN_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid;
		
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	if( pGrid )
		//pGrid->MoveWindow( 15, 40, cx - 25, cy - 55 );
		pGrid->MoveWindow(-2, 35, cx+2, cy);

	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	if( pGrid )
		//pGrid->MoveWindow( 15, 40, cx - 25, cy - 55 );
		pGrid->MoveWindow(-2, 35, cx+2, cy);

	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_3 );
	if( pGrid )
		//pGrid->MoveWindow( 15, 40, cx - 25, cy - 55 );
		pGrid->MoveWindow(-2, 35, cx+2, cy);
}
