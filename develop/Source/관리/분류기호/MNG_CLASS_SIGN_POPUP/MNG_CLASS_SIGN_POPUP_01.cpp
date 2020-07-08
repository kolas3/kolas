// MNG_CLASS_SIGN_POPUP_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"

#include "MNG_CLASS_SIGN_POPUP_01.h"
#include "MNG_CLASS_SIGN_POPUP_05.h"

#include "..\MNG_CLASS_SIGN_MANAGER\CLASS_SIGN_MANAGER_03.h"
#include "..\MNG_CLASS_SIGN_MANAGER\CLASS_SIGN_MANAGER_04.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CLASS_SIGN_POPUP_01 dialog


CMNG_CLASS_SIGN_POPUP_01::CMNG_CLASS_SIGN_POPUP_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CLASS_SIGN_POPUP_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CLASS_SIGN_POPUP_01::~CMNG_CLASS_SIGN_POPUP_01()
{
	
}  

BOOL CMNG_CLASS_SIGN_POPUP_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CLASS_SIGN_POPUP_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CLASS_SIGN_POPUP_01)
	DDX_Control(pDX, IDC_eTAG, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CLASS_SIGN_POPUP_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_CLASS_SIGN_POPUP_01)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_cTYPE, OnSelchangecTYPE)
	ON_BN_CLICKED(IDC_bFIND_MCSP, OnbFIND)
	ON_BN_CLICKED(IDC_bVIEW_VALUE, OnbVIEWVALUE)
	ON_BN_CLICKED(IDC_bVIEW_DESC, OnbVIEWDESC)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CLASS_SIGN_POPUP_01 message handlers

BOOL CMNG_CLASS_SIGN_POPUP_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CLASS_SIGN_POPUP_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CLASS_SIGN_POPUP_01") );
		return false;
	}

	m_arrayKDC.RemoveAll();
	m_arrayDDC.RemoveAll();
	m_arrayLC.RemoveAll();
	
	m_a.Format( _T("▼") );
	m_e.Format( _T("▲") );	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CMNG_CLASS_SIGN_POPUP_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	InitCtrlSetting();
	OnSelchangecTYPE();
	//SetScreenType();
}

VOID CMNG_CLASS_SIGN_POPUP_01::InitCtrlSetting()
{
	CComboBox * pCombo = ( CComboBox * )GetDlgItem( IDC_cTYPE );
	pCombo->InsertString( 0, _T("KDC") );
	pCombo->InsertString( 1, _T("DDC") );
	pCombo->InsertString( 2, _T("LC") );
	pCombo->InsertString( 3, _T("색인어") );
	pCombo->SetCurSel( 0 );
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnSelchangecTYPE() 
{
	// TODO: Add your control notification handler code here
	
	SetScreenType();
	m_edit.SetWindowText( _T("") );

	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_eCONDITION );
	pEdt->SetWindowText( _T("") );
	
	m_KDCrow.RemoveAll();
	m_DDCrow.RemoveAll();
	m_LCrow.RemoveAll();

	ShowGrid();

	pEdt = ( CEdit * )GetDlgItem( IDC_eCONDITION );
	pEdt->SetFocus();
}

INT CMNG_CLASS_SIGN_POPUP_01::SetScreenType(INT option)
{
	CComboBox * pCombo = ( CComboBox * )GetDlgItem( IDC_cTYPE );
	
	if( -1 == option )
		m_type = GetCurrentType();
	else
	{
		m_type = option;
		pCombo->SetCurSel( m_type );
	}
	// 0 : KDC
	// 1 : DDC
	// 2 : LC
	// 3 : 색인어

	const INT gridCnt = 4;
	CMSHFlexGrid *pGrid;
	UINT gridID[ gridCnt ] = { IDC_mngGRID_P1, IDC_mngGRID_P2, IDC_mngGRID_P3, IDC_mngGRID_P4};
	bool showFlag[ gridCnt ] = { false, false, false, false };
	showFlag[ m_type ] = true;

	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ i ] );
		pGrid->ShowWindow( showFlag[ i ] );
	}
	
	return 0;
}

INT CMNG_CLASS_SIGN_POPUP_01::GetCurrentType()
{
	CComboBox * pCombo = ( CComboBox * )GetDlgItem( IDC_cTYPE );
	
	return pCombo->GetCurSel();
}

VOID CMNG_CLASS_SIGN_POPUP_01::ShowGrid( INT option )
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
		case 3 :
			gridAlias = _T("그리드_4");
			break;
		default :
			return;
			break;
	}

	ControlDisplay( _T("CM_MNG_CLASS_SIGN_POPUP_01"), gridAlias );

}

BEGIN_EVENTSINK_MAP(CMNG_CLASS_SIGN_POPUP_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_CLASS_SIGN_POPUP_01)
	ON_EVENT(CMNG_CLASS_SIGN_POPUP_01, IDC_mngGRID_P1, -600 /* Click */, OnClickmngGRIDP1, VTS_NONE)
	ON_EVENT(CMNG_CLASS_SIGN_POPUP_01, IDC_mngGRID_P2, -600 /* Click */, OnClickmngGRIDP2, VTS_NONE)
	ON_EVENT(CMNG_CLASS_SIGN_POPUP_01, IDC_mngGRID_P3, -600 /* Click */, OnClickmngGRIDP3, VTS_NONE)
	ON_EVENT(CMNG_CLASS_SIGN_POPUP_01, IDC_mngGRID_P4, -600 /* Click */, OnClickmngGRIDP4, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CMNG_CLASS_SIGN_POPUP_01::OnClickmngGRIDP1() 
{
	// TODO: Add your control notification handler code here
	
	INT row, col;
	GetSelectedCell( row, col, 0 );
	if( 1 != col )
		return;

	CString tmp = GetCellText( row, col, 0 );
	if( _T("V") == GetCellText( row, col, 0 ) )
	{
		INT nIdx = GetKDCArrayAt( row );
		if ( nIdx >= 0 ) return;

		m_KDCrow.Add( row );
	}
	else
	{
		INT nIdx = GetKDCArrayAt( row );
		if ( nIdx < 0 ) return;
		m_KDCrow.RemoveAt( nIdx );
	}

	m_KDCtag = GetKDCTag() + m_e;
	m_edit.SetWindowText( m_KDCtag );
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnClickmngGRIDP2() 
{
	// TODO: Add your control notification handler code here
	
	INT row, col;
	GetSelectedCell( row, col, 1 );
	if( 1 != col )
		return;

	if( _T("V") == GetCellText( row, col, 1 ) )
	{
		INT nIdx = GetKDCArrayAt( row );
		if ( nIdx >= 0 ) return;

		m_DDCrow.Add( row );
	}
	else
	{
		INT nIdx = GetKDCArrayAt( row );
		if ( nIdx < 0 ) return;
		m_DDCrow.RemoveAt( nIdx );
	}
		

	m_DDCtag = GetDDCTag() + m_e;
	m_edit.SetWindowText( m_DDCtag );			
}
VOID CMNG_CLASS_SIGN_POPUP_01::OnClickmngGRIDP3() 
{
	// TODO: Add your control notification handler code here
	
	INT row, col;
	GetSelectedCell( row, col, 2 );
	if( 1 != col )
		return;

	if( _T("V") == GetCellText( row, col, 2 ) )
	{
		INT nIdx = GetLCArrayAt( row );
		if ( nIdx >= 0 ) return;

		m_LCrow.Add( row );
	}
	else
	{
		INT nIdx = GetLCArrayAt( row );
		if ( nIdx < 0 ) return;
		m_LCrow.RemoveAt( nIdx );
	}
		

	m_LCtag = GetLCTag() + m_e;
	m_edit.SetWindowText( m_LCtag );			
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnClickmngGRIDP4() 
{
	// TODO: Add your control notification handler code here
	
	Set3rdGridCheck();

	INT row, col;
	GetSelectedCell( row, col, 3 );
	if( 2 != col && 4 != col )
		return;

	if( _T("V") == GetCellText( row, col, 3 ) )
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
	
	CString kdc = GetKDCTag( 3 );
	if( kdc.GetLength() )
		m_KDCtag = kdc + m_e;
	else 
		m_KDCtag.Empty();
	
	CString ddc = GetDDCTag( 3 );
	if( ddc.GetLength() )
		m_DDCtag = ddc + m_e;
	else
		m_DDCtag.Empty();

	m_edit.SetWindowText( m_KDCtag + _T("\r\n") + m_DDCtag );	
}

INT CMNG_CLASS_SIGN_POPUP_01::SearchData(INT type, CString keyword)
{
	m_type = type;
	
	CESL_DataMgr *pDM;
	switch( m_type )
	{
		case 0 :
			pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_KDC") );
			break;
		case 1 :
			pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_DDC") );
			break;
		case 2 :
			pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_LC") );
			break;
		case 3 :
			pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_IDX") );
			break;
		default :
			return -1;
			break;
	}
	
	CString where;
	if( keyword.GetLength() && 3 != m_type )
		where.Format( _T("CLASS_VAL LIKE '%s%%' ORDER BY CLASS_VAL"), keyword ); //bjs
	else if( keyword.GetLength() && 3 == m_type )
		where.Format( _T("I.INDEX_WORD LIKE '%%%s%%' ORDER BY I.INDEX_WORD"), keyword );	
	
	pDM->RefreshDataManager( where );

	
	INT rowCnt = pDM->GetRowCount();
	if( !rowCnt )
		AfxMessageBox( _T("검색 결과가 없습니다.") );
	
	ShowGrid();

	m_KDCrow.RemoveAll();
	m_DDCrow.RemoveAll();
	m_LCrow.RemoveAll();

	return 0;
}

INT * CMNG_CLASS_SIGN_POPUP_01::GetCheckedDMRow(INT &rowCnt, INT option)
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
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_P1 );
			break;
		case 1 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_P2 );
			break;
		case 2 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_P3 );
			break;
		case 3 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_P4 );
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

CString CMNG_CLASS_SIGN_POPUP_01::GetKDCTagString(INT *checkedDMRow, INT rowCnt)
{
	CString tagString, buf;
	tagString = _T("056  ");

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_KDC") );

	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("CLASS_VAL"), i, buf );
		tagString = tagString + _T("%a") + buf;
	}

	return tagString;
}


VOID CMNG_CLASS_SIGN_POPUP_01::Set3rdGridCheck()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_P4 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 2 != col && 4 != col )
		return;

	if( _T("V") == pGrid->GetText() )
		pGrid->SetText( _T("") );
	else
		pGrid->SetText( _T("V") );
}

VOID CMNG_CLASS_SIGN_POPUP_01::GetSelectedCell(INT &row, INT &col, INT option)
{
	const INT gridCnt = 4;
	CMSHFlexGrid *pGrid;
	UINT gridID[ gridCnt ] = { IDC_mngGRID_P1, IDC_mngGRID_P2, IDC_mngGRID_P3, IDC_mngGRID_P4};
	
	if( -1 == option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ m_type ] );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ option ] );
	
	row = pGrid->GetRow();
	col = pGrid->GetCol();
}

CString CMNG_CLASS_SIGN_POPUP_01::GetCellText(INT row, INT col, INT option)
{
	const INT gridCnt = 4;
	CMSHFlexGrid *pGrid;
	UINT gridID[ gridCnt ] = { IDC_mngGRID_P1, IDC_mngGRID_P2, IDC_mngGRID_P3, IDC_mngGRID_P4};
	
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

INT CMNG_CLASS_SIGN_POPUP_01::GetKDCArrayAt(INT row)
{
	for( INT i = 0; i < m_KDCrow.GetSize(); i++ )
		if( row == m_KDCrow.GetAt( i ) )
			return i;

	return -1;
}

CString CMNG_CLASS_SIGN_POPUP_01::GetKDCTag( INT option )
{
	CESL_DataMgr *pDM;
	if( 3 == option )
		pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_IDX") );
	else
		pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_KDC") );
	
	CString tag = _T("056  ");
	CString tmp;

	INT rowCnt = m_KDCrow.GetSize();
	if( !rowCnt )
		return _T("");

	for( INT i = 0; i < rowCnt; i++ )
	{
		if( 3 == option )
			pDM->GetCellData( _T("I_KDC"), m_KDCrow.GetAt( i ) - 1, tmp );
		else
			pDM->GetCellData( _T("CLASS_VAL"), m_KDCrow.GetAt( i ) - 1, tmp );

		//=====================================================
		//2009.02.16 UPDATE BY PJW : Tag에 식변자 추가
// 		tag = tag + m_a + tmp;
		tag = tag + m_a+ _T("a") + tmp;
		//=====================================================
		
	}	
	
	return tag;
}

INT CMNG_CLASS_SIGN_POPUP_01::GetDDCArrayAt(INT row)
{
	for( INT i = 0; i < m_DDCrow.GetSize(); i++ )
		if( row == m_DDCrow.GetAt( i ) )
			return i;

	return -1;
}

CString CMNG_CLASS_SIGN_POPUP_01::GetDDCTag(INT option)
{
	CESL_DataMgr *pDM;
	if( 3 == option )
		pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_IDX") );
	else
		pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_DDC") );
	
	CString tag = _T("082  ");
	CString tmp;

	INT rowCnt = m_DDCrow.GetSize();
	if( !rowCnt )
		return _T("");

	for( INT i = 0; i < rowCnt; i++ )
	{	
		if( 3 == option )
			pDM->GetCellData( _T("I_DDC"), m_DDCrow.GetAt( i ) - 1, tmp );
		else
			pDM->GetCellData( _T("CLASS_VAL"), m_DDCrow.GetAt( i ) - 1, tmp );

		//=====================================================
		//2009.02.16 UPDATE BY PJW : Tag에 식변자 추가
// 		tag = tag + m_a + tmp;
		tag = tag + m_a+ _T("a") + tmp;
		//=====================================================
		
	}	
	
	return tag;		
}

INT CMNG_CLASS_SIGN_POPUP_01::GetLCArrayAt(INT row)
{
	for( INT i = 0; i < m_LCrow.GetSize(); i++ )
		if( row == m_LCrow.GetAt( i ) )
			return i;

	return -1;
}

CString CMNG_CLASS_SIGN_POPUP_01::GetLCTag(INT option)
{
	CESL_DataMgr *pDM;
	if( 3 == option )
		pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_IDX") );
	else
		pDM = FindDM( _T("DM_MNG_CLASS_SIGN_POPUP_01_LC") );
	
	CString tag = _T("0855 ");
	CString tmp;

	INT rowCnt = m_LCrow.GetSize();
	if( !rowCnt )
		return _T("");

	for( INT i = 0; i < rowCnt; i++ )
	{	
		if( 3 == option )
			pDM->GetCellData( _T("I_LC"), m_LCrow.GetAt( i ) - 1, tmp );
		else
			pDM->GetCellData( _T("CLASS_VAL"), m_LCrow.GetAt( i ) - 1, tmp );

		tag = tag + m_a + tmp;
	}	
	
	return tag;		
}

// 검색
VOID CMNG_CLASS_SIGN_POPUP_01::OnbFIND() 
{
	// TODO: Add your control notification handler code here
	
	CString keyword;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eCONDITION );
	pEdt->GetWindowText( keyword );

	keyword.TrimLeft(); keyword.TrimRight();
	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("검색어를 입력하여 주십시오") );
		return;
	}

	SearchData( m_type, keyword );
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnbVIEWVALUE() 
{
	CMNG_CLASS_SIGN_POPUP_05* dlg;

	dlg = new CMNG_CLASS_SIGN_POPUP_05(this);
	if ( dlg->DoModal() == IDOK ) 
	{
		CDialog::OnOK();
	}
	
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnbVIEWDESC() 
{

	CCLASS_SIGN_MANAGER_04 *pDlg = NULL;
	pDlg = new CCLASS_SIGN_MANAGER_04( this );
	if( pDlg->Create( this ) )
	{
		pDlg->ShowWindow( SW_SHOW );
		pDlg->UpdateWindow();
		pDlg->CenterWindow();
		return ;
	}
	else
	{
		delete pDlg;
		return ;
	}
}

INT * CMNG_CLASS_SIGN_POPUP_01::GetCheckedIDXRow( INT &cnt_1, INT &cnt_2 )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_P4 );
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

VOID CMNG_CLASS_SIGN_POPUP_01::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	OnClose();
	//CDialog::OnCancel();
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::EndDialog( -1 );
	//CDialog::OnClose();
}

VOID CMNG_CLASS_SIGN_POPUP_01::OnOK() 
{
	CString strGridAlias;
	CString strValue;
	CESL_ControlMgr* pCM = FindCM(_T("CM_MNG_CLASS_SIGN_POPUP_01"));
	CESL_Grid *pGrid;	
	INT i;
	
	strGridAlias.Format(_T("그리드_%d"), m_type+1 );
	pGrid = (CESL_Grid*)pCM->FindControl(strGridAlias);		
	//===================================================
	//2009.08.07 UPDATE BY PJW : 색인어 검색시 처리 추가함
//	pGrid->SelectMakeList();
//
//	pGrid->SelectGetHeadPosition();
//
//	INT nIndex;
//
//	for ( int i=0 ; i<pGrid->SelectGetCount() ; i++ )
//	{
//		nIndex = pGrid->SelectGetIdx();
//		//=====================================================
//		//2008.11.20 ADD BY PJW : 그리드의 IDX가 맞지 않아서 추가
//		nIndex += 1;	
//		//=====================================================
//		switch ( m_type )
//		{
//		case 0:
//			strValue = pGrid->GetTextMatrix(nIndex, 2);		
//			m_arrayKDC.Add(strValue);			
//			break;
//		case 1:
//			strValue = pGrid->GetTextMatrix(nIndex, 2);		
//			m_arrayDDC.Add(strValue);						
//			break;
//		case 2:
//			strValue = pGrid->GetTextMatrix(nIndex, 2);		
//			m_arrayLC.Add(strValue);						
//			break;
//		case 3:
//			
//			break;
//		default:
//			break;
//		}
//
//		pGrid->SelectGetNext();
//	}
	if( _T("그리드_4") == strGridAlias )
	{
		INT row = pGrid->GetRows();
		for( i = 0 ; i < row ; i++ )
		{
			strValue = pGrid->GetTextMatrix(i, 2);
			if( _T("V") == strValue )
			{
				strValue = pGrid->GetTextMatrix(i, 3);
				m_arrayKDC.Add(strValue);
			}
			strValue = pGrid->GetTextMatrix(i, 4);
			if( _T("V") == strValue )
			{
				strValue = pGrid->GetTextMatrix(i, 5);
				m_arrayDDC.Add(strValue);
			}
		}

	}
	else
	{
		pGrid->SelectMakeList();

		pGrid->SelectGetHeadPosition();

		INT nIndex;

		for (  i=0 ; i<pGrid->SelectGetCount() ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			//=====================================================
			//2008.11.20 ADD BY PJW : 그리드의 IDX가 맞지 않아서 추가
			nIndex += 1;	
			//=====================================================
			switch ( m_type )
			{
			case 0:
				strValue = pGrid->GetTextMatrix(nIndex, 2);		
				m_arrayKDC.Add(strValue);			
				break;
			case 1:
				strValue = pGrid->GetTextMatrix(nIndex, 2);		
				m_arrayDDC.Add(strValue);						
				break;
			case 2:
				strValue = pGrid->GetTextMatrix(nIndex, 2);		
				m_arrayLC.Add(strValue);						
				break;
			case 3:
				
				break;
			default:
				break;
			}

			pGrid->SelectGetNext();
		}
	}
	//===================================================
	
	CDialog::OnOK();
}

HBRUSH CMNG_CLASS_SIGN_POPUP_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
