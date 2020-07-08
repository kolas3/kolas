// MNG_CAT_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CAT_MANAGER_01.h"
#include "..\CatCodeGetter\CatCodeGetterDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_01 dialog


CMNG_CAT_MANAGER_01::CMNG_CAT_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_CAT_MANAGER_01::IDD,	pParent)
{
	//{{AFX_DATA_INIT(CMNG_CAT_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CAT_MANAGER_01::~CMNG_CAT_MANAGER_01()
{
}

BOOL CMNG_CAT_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CAT_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CAT_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CAT_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_CAT_MANAGER_01)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bTMP, OnbTMP)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_01 message handlers

BOOL CMNG_CAT_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CAT_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CAT_MANAGER_01") );
		return false;
	}
	
	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );
	if( pGrid )
	{
		//RemoveListArray.Add( _T("") );

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 삭제.
INT CMNG_CAT_MANAGER_01::DeleteData()
{
	INT *checkedDMRow, rowCnt;
	checkedDMRow = GetCheckedDMRow( rowCnt );

	if( !rowCnt )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		return -1;
	}
	
	CString notice;
	notice.Format( _T("선택된 %d개의 목록을 삭제하시겠습니까?"), rowCnt );
	if( IDNO == AfxMessageBox( notice , MB_YESNO ) )
		return -2;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
	CString pk, query;
	
	for( INT i = rowCnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), checkedDMRow[ i ], pk );
		query.Format( _T(" DELETE FROM MN_AUTHOR_RULE_TBL WHERE REC_KEY = %s;"), pk );

		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( checkedDMRow[ i ] );
	};
	
	ShowGrid();

	free( checkedDMRow );

	return 0;
}

VOID CMNG_CAT_MANAGER_01::ShowGrid()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("발행형태구분"), _T("PUBLISH_FORM_CODE_DESC") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("형식구분"),	   _T("FORM_CODE_DESC") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("보조등록구분"), _T("SUB_REG_CODE_DESC")  );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("표준저자기호"), _T("RULE_TYPE_DESC") );

	ControlDisplay( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );

	/*
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	CString desc;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
	if( !pDM->GetRowCount() )
		return;
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		pGrid->SetCol( 2 );
		desc = GetCodeDesc( _T("발행형태구분"), pGrid->GetText() );
		pGrid->SetText( desc );
		
		pGrid->SetCol( 3 );
		desc = GetCodeDesc( _T("형식구분"), pGrid->GetText() );
		pGrid->SetText( desc );

		pGrid->SetCol( 4 );
		desc = GetCodeDesc( _T("보조등록구분"), pGrid->GetText() );
		pGrid->SetText( desc );

		pGrid->SetCol( 6 );
		desc = GetCodeDesc( _T("표준저자기호"), pGrid->GetText() );
		pGrid->SetText( desc );		
	}
	*/
}

CString CMNG_CAT_MANAGER_01::GetCodeDesc(CString className, CString code)
{
	CString desc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc( className, code, desc );

	return desc;
}

INT * CMNG_CAT_MANAGER_01::GetCheckedDMRow( INT &rowCnt )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT *checkedDMRow, i;
	INT gridRows = pGrid->GetRows();
	rowCnt = 0;
	
	pGrid->SetCol( 1 );
	for( i = 1; i < gridRows; i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			rowCnt++;
	}

	checkedDMRow = ( INT * )calloc( rowCnt, sizeof( INT ) );
	INT index = 0;

	for( i = 1; i < gridRows; i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			checkedDMRow[ index++ ] = i - 1;
	}

	return checkedDMRow;
}

VOID CMNG_CAT_MANAGER_01::OnSize( UINT nType, INT cx, INT cy )
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	if( NULL == pGrid->GetSafeHwnd() )
		return;

	//pGrid->MoveWindow( -2, -2, cx + 4, cy + 4 );
	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CMNG_CAT_MANAGER_01::OnbTMP() 
{
	// TODO: Add your control notification handler code here
	
	CCatCodeGetterDlg dlg( this );
	dlg.DoModal();
}

HBRUSH CMNG_CAT_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
