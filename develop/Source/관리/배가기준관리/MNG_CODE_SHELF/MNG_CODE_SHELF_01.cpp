// MNG_CODE_SHELF_01.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_CODE_SHELF_01.h"
#include "MNG_CODE_SHELF_02.h"
#include "MNG_CODE_SHELF_03.h"
#include "MNG_CODE_SHELF_04.h"
#include "ESL_Grid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_01 dialog


//CMNG_CODE_SHELF_01::CMNG_CODE_SHELF_01( CESL_Mgr* pParent  /*=NULL*/)
/*	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_SHELF_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}
*/

CMNG_CODE_SHELF_01::CMNG_CODE_SHELF_01( UINT ID, CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_SHELF_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	if( ID == CHILD )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD );
		m_nIDHelp = IDD;
	}
	else if( ID == POPUP )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_POPUP );	
		m_nIDHelp = IDD_POPUP;		
	}
}

CMNG_CODE_SHELF_01::~CMNG_CODE_SHELF_01()
{
	
}



BOOL CMNG_CODE_SHELF_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_SHELF_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_SHELF_01)
	DDX_Control(pDX, IDC_mngTAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_SHELF_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_SHELF_01)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_mngTAB, OnSelchangemngTAB)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED( IDC_bFIND, OnFind )
	ON_BN_CLICKED( IDC_bINPUT, OnIinput )
	ON_BN_CLICKED( IDC_bDELETE, OnDelete )
	ON_BN_CLICKED( IDC_bBAGA, OnBaga )
	ON_BN_CLICKED( IDOK, OnOK )
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_01 message handlers

BOOL CMNG_CODE_SHELF_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_SHELF_01") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_CODE_SHELF_01") );
		return false;
	}
	
	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	
	CESL_Grid *pGrid;

	const INT gridCnt = 3;
	CString gridAlias[ gridCnt ] = {
		_T("등록"), _T("별치"), _T("분류") };

	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid =  ( CESL_Grid * )FindControl( _T("CM_MNG_CODE_SHELF_01"), gridAlias[ i ] );
		
		if( pGrid )
		{
			//RemoveListArray.Add( _T("") );

			pGrid->InitPopupMenuItem();
			pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
		}
	}

	/*
	if( 1 == m_wndStyle )
	{
		this->ModifyStyle( WS_CHILD, WS_POPUP | WS_BORDER | WS_VISIBLE );
		this->SetWindowPos( &wndTop, 0, 0, 900, 900, SWP_SHOWWINDOW );
		MAKEINTRESOURCE( IDD_MNG_CODE_SHELF_01 );	
		
		this->ShowWindow( true );
		this->EnableWindow( true );
	}
	*/


	m_tab.InsertItem( 0, _T(" 등록구분 ") );
	m_tab.InsertItem( 1, _T(" 별치기호 ") );
	m_tab.InsertItem( 2, _T(" 분류기호 ") );

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_SHELF_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid_1 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CMSHFlexGrid *pGrid_2 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	CMSHFlexGrid *pGrid_3 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_3 );

	if( NULL == pGrid_1->GetSafeHwnd() )
		return;

	if( NULL == pGrid_2->GetSafeHwnd() )
		return;

	if( NULL == pGrid_3->GetSafeHwnd() )
		return;
	
	//pGrid_1->MoveWindow( 15, 40, cx - 25, cy - 55 );
	//pGrid_2->MoveWindow( 15, 40, cx - 25, cy - 55 );
	//pGrid_3->MoveWindow( 15, 40, cx - 25, cy - 55 );
	pGrid_1->MoveWindow(0, 20, cx, cy);
	pGrid_2->MoveWindow(0, 20, cx, cy);
	pGrid_3->MoveWindow(0, 20, cx, cy);

	//m_tab.MoveWindow( 2, 3, cx - 2, cy - 3 );
	m_tab.MoveWindow(0, 0, cx+4, cy);
}

VOID CMNG_CODE_SHELF_01::OnSelchangemngTAB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ViewTab( m_tab.GetCurSel() + 1 );
	*pResult = 0;
}

VOID CMNG_CODE_SHELF_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
}

// 삭제
VOID CMNG_CODE_SHELF_01::DeleteData()
{
	INT *checkedDMRow, checkedRowCnt;
	checkedDMRow = NULL;
	checkedDMRow = GetCheckedDMRow( checkedRowCnt, m_tab.GetCurSel() + 1 );

	if( !checkedRowCnt )
	{
		AfxMessageBox( _T("선택목록이 없습니다.") );
		free( checkedDMRow );
		checkedDMRow = NULL;
		return;
	}

	CString notice;
	notice.Format( _T("%d 건의 목록을 삭제하시겠습니까?"), checkedRowCnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
		return;
	
	CESL_DataMgr *pDM;
	CString dmAlias[ 3 ] = { _T("DM_MNG_CODE_SHELF_01_1"), _T("DM_MNG_CODE_SHELF_01_2"), _T("DM_MNG_CODE_SHELF_01_3") };
	pDM = FindDM( dmAlias[ m_tab.GetCurSel() ] );
	
	CString tableName[ 3 ] = { _T("MN_REG_CODE_SHELF_RULE_MNG_TBL"), _T("MN_SEP_CODE_SHELF_RULE_MNG_TBL"), _T("MN_CLS_NUM_SHELF_RULE_MNG_TBL") };

	CString query, pk;

	INT type = m_tab.GetCurSel();
	for( INT i = 0; i < checkedRowCnt; i++ )
	{
		pDM->GetCellData( _T("REC_KEY"), checkedDMRow[ i ], pk );
		query.Format( _T("DELETE FROM %s WHERE REC_KEY = %s;"), tableName[ type ], pk );
		pDM->ExecuteQuery( query, 1 );		
	}

	for( i = checkedRowCnt - 1; i > -1; i-- )
		pDM->DeleteRow( checkedDMRow[ i ] );

	ShowGrid( type + 1 );

	AfxMessageBox( _T("자료를 삭제하였습니다.") );
	
	free( checkedDMRow );
	checkedDMRow = NULL;	
}

BEGIN_EVENTSINK_MAP(CMNG_CODE_SHELF_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_OrderDataMgr)
	ON_EVENT(CMNG_CODE_SHELF_01, IDC_mngGRID_1, -601 /* DblClick */, OnDblClickgrid_1, VTS_NONE)
	ON_EVENT(CMNG_CODE_SHELF_01, IDC_mngGRID_2, -601 /* DblClick */, OnDblClickgrid_2, VTS_NONE)
	ON_EVENT(CMNG_CODE_SHELF_01, IDC_mngGRID_3, -601 /* DblClick */, OnDblClickgrid_3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_CODE_SHELF_01::OnDblClickgrid_1()
{
	INT type = m_tab.GetCurSel();
	ShowGrid( type + 1 );
}

VOID CMNG_CODE_SHELF_01::OnDblClickgrid_2()
{
	INT type = m_tab.GetCurSel();
	ShowGrid( type + 1 );
}

VOID CMNG_CODE_SHELF_01::OnDblClickgrid_3()
{
	INT type = m_tab.GetCurSel();
	ShowGrid( type + 1 );
}

INT CMNG_CODE_SHELF_01::ShowGrid( INT option )
{
	/*
		1 : 등록
		2 : 별치
		3 : 뷴류 
	*/

	CESL_DataMgr *pDM;
	INT i;
	CString tmp;
	
	switch( option )
	{
		case 1 :
			
			ControlDisplay( _T("CM_MNG_CODE_SHELF_01_01"), _T("등록") );
			
			pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );
			if( !pDM->GetRowCount() )
			{
				AfxMessageBox( _T("검색 결과가 없습니다.") );
				break;
			}

			for( i = 0; i < pDM->GetRowCount(); i++ )
			{
				pDM->GetCellData( _T("PUBLISH_FORM_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("발행형태구분"), tmp, tmp );
				SetGridText( i + 1, 2, tmp, 1 );

				pDM->GetCellData( _T("MANAGE_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), tmp, tmp );
				SetGridText( i + 1, 3, tmp, 1 );

				pDM->GetCellData( _T("REG_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("등록구분"), tmp, tmp );
				SetGridText( i + 1, 5, tmp, 1 );

				pDM->GetCellData( _T("SHELF_LOC_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분"), tmp, tmp );
				SetGridText( i + 1, 7, tmp, 1 );
			}

			break;

		case 2 :
			
			ControlDisplay( _T("CM_MNG_CODE_SHELF_01_01"), _T("별치") );

			pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_2") );
			if( !pDM->GetRowCount() )
			{
				AfxMessageBox( _T("검색 결과가 없습니다.") );
				break;
			}

			for( i = 0; i < pDM->GetRowCount(); i++ )
			{
				pDM->GetCellData( _T("PUBLISH_FORM_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("발행형태구분"), tmp, tmp );
				SetGridText( i + 1, 2, tmp, 2 );

				pDM->GetCellData( _T("MANAGE_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), tmp, tmp );
				SetGridText( i + 1, 3, tmp, 2 );

				pDM->GetCellData( _T("SEPARATE_SHELF_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("별치기호"), tmp, tmp );
				SetGridText( i + 1, 5, tmp, 2 );

				pDM->GetCellData( _T("SHELF_LOC_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분"), tmp, tmp );
				SetGridText( i + 1, 7, tmp, 2 );
			}

			break;

		case 3 :

			ControlDisplay( _T("CM_MNG_CODE_SHELF_01_01"), _T("분류") );

			pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_3") );
			if( !pDM->GetRowCount() )
			{
				AfxMessageBox( _T("검색 결과가 없습니다.") );
				break;
			}

			for( i = 0; i < pDM->GetRowCount(); i++ )
			{
				pDM->GetCellData( _T("PUBLISH_FORM_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("발행형태구분"), tmp, tmp );
				SetGridText( i + 1, 2, tmp, 3 );

				pDM->GetCellData( _T("MANAGE_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), tmp, tmp );
				SetGridText( i + 1, 3, tmp, 3 );

				pDM->GetCellData( _T("CLASS_NO_TYPE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("분류표구분"), tmp, tmp );
				SetGridText( i + 1, 4, tmp, 3 );

				pDM->GetCellData( _T("SHELF_LOC_CODE"), i, tmp );
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분"), tmp, tmp );
				SetGridText( i + 1, 5, tmp, 3 );
			}

			break;
	}

	m_tab.SetCurSel( option - 1 );
	ViewTab( option );

	return 0;
}

INT CMNG_CODE_SHELF_01::ViewTab( INT option )
{
	CMSHFlexGrid *pGrid_0 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CMSHFlexGrid *pGrid_1 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	CMSHFlexGrid *pGrid_2 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_3 );

	switch( option )
	{
		case 1 :
			pGrid_0->ShowWindow( true );
			pGrid_1->ShowWindow( false );
			pGrid_2->ShowWindow( false );
			break;
		case 2 :
			pGrid_1->ShowWindow( true );
			pGrid_0->ShowWindow( false );
			pGrid_2->ShowWindow( false );
			break;
		case 3 : 
			pGrid_2->ShowWindow( true );
			pGrid_0->ShowWindow( false );
			pGrid_1->ShowWindow( false );
			break;
	}	
	
	return 0;
}

INT CMNG_CODE_SHELF_01::SetGridText(INT row, INT col, CString string, INT option)
{
	CMSHFlexGrid *pGrid;

	UINT gridID[ 3 ] = { IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3 };
	pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ option - 1 ] );

	pGrid->SetTextMatrix( row, col, string );

	return 0;
}

INT *CMNG_CODE_SHELF_01::GetCheckedDMRow( INT &rowCnt, INT option )
{
	INT *checkedDMRow;
	rowCnt = 0;

	CMSHFlexGrid *pGrid;
	UINT gridID[ 3 ] = { IDC_mngGRID_1, IDC_mngGRID_2, IDC_mngGRID_3 };
	pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ option - 1 ] );

	pGrid->SetCol( 1 );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		if( _T("V") == pGrid->GetText() )
			rowCnt++;		
	}

	checkedDMRow = ( INT * )calloc( rowCnt, sizeof( INT ) );

	INT index = 0;
	for( i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		if( _T("V") == pGrid->GetText() )
			checkedDMRow[ index++ ] = i - 1;
	}

	return checkedDMRow;
}

VOID CMNG_CODE_SHELF_01::OnFind()
{
	CMNG_CODE_SHELF_03 dlg( this );
	dlg.DoModal();
}

VOID CMNG_CODE_SHELF_01::OnIinput()
{
	CMNG_CODE_SHELF_04 dlg( this );
	dlg.m_mode = 1;
	dlg.DoModal();
}

VOID CMNG_CODE_SHELF_01::OnDelete()
{
	CMNG_CODE_SHELF_04 dlg( this );
	dlg.m_mode = 2;
	dlg.DoModal();
}

VOID CMNG_CODE_SHELF_01::OnBaga()
{
	CMNG_CODE_SHELF_02 dlg( this );
	dlg.m_openMode = 2;
	dlg.DoModal();
}

VOID CMNG_CODE_SHELF_01::OnOK()
{
	CDialog::OnOK();
}

HBRUSH CMNG_CODE_SHELF_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
