// MNG_MNGVALUE_OPERATOR_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MNGVALUE_OPERATOR_02.h"
#include "MNG_MNGVALUE_OPERATOR_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGVALUE_OPERATOR_02 dialog


CMNG_MNGVALUE_OPERATOR_02::CMNG_MNGVALUE_OPERATOR_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MNGVALUE_OPERATOR_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_MNGVALUE_OPERATOR_02::~CMNG_MNGVALUE_OPERATOR_02()
{
	
}

BOOL CMNG_MNGVALUE_OPERATOR_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_MNGVALUE_OPERATOR_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MNGVALUE_OPERATOR_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MNGVALUE_OPERATOR_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_MNGVALUE_OPERATOR_02)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGVALUE_OPERATOR_02 message handlers

BOOL CMNG_MNGVALUE_OPERATOR_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MNGVALUE_OPERATOR_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MNGVALUE_OPERATOR_02") );
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

VOID CMNG_MNGVALUE_OPERATOR_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	SearchData();
}

VOID CMNG_MNGVALUE_OPERATOR_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CMNG_MNGVALUE_OPERATOR_02::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}

// 입력
VOID CMNG_MNGVALUE_OPERATOR_02::OnbINSERT() 
{
	// TODO: Add your control notification handler code here
	
	CMNG_MNGVALUE_OPERATOR_03 dlg( this );
	dlg.m_type = 1;
	dlg.DoModal();
}

// 수정
VOID CMNG_MNGVALUE_OPERATOR_02::OnbUPDATE() 
{
	// TODO: Add your control notification handler code here
	
	CMNG_MNGVALUE_OPERATOR_03 dlg( this );
	dlg.m_type = 2;
	dlg.DoModal();
}

// 삭제
VOID CMNG_MNGVALUE_OPERATOR_02::OnbDELETE() 
{
	// TODO: Add your control notification handler code here
	
	INT cnt = 0 ;
	INT *rows;
	rows = GetCheckedRowInfo( cnt );

	if( !cnt )
	{
		AfxMessageBox( _T("선택 목록이 없습니다.") );
		return;
	}

	CString notice;
	notice.Format( _T("선택한 %d개의 목록을 삭제하시겠습니까?"), cnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
		return;

	if( DeleteData( rows, cnt ) )
		AfxMessageBox( _T("Error : INT CMNG_MNGVALUE_OPERATOR_02::DeleteData( INT *rows, INT col )") );
	else
	{
		ShowGrid();
		AfxMessageBox( _T("삭제 되었습니다.") );
	}
	
	free( rows );
}

VOID CMNG_MNGVALUE_OPERATOR_02::SearchData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_02") );
	pDM->RefreshDataManager( _T("") );
	ShowGrid();
}

INT * CMNG_MNGVALUE_OPERATOR_02::GetCheckedRowInfo( INT &cnt )
{
	cnt = 0;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_02") );
	INT rowCnt = pDM->GetRowCount();

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	CArray< INT, INT > rows;
	pGrid->SetCol( 1 );
	for( INT i = 0; i < rowCnt; i++ )
	{
		pGrid->SetRow( i + 1 );

		if( _T("V") == pGrid->GetText() )
			rows.Add( i );
	}

	cnt = rows.GetSize();
	INT *DMRow = ( INT * )calloc( cnt, sizeof( INT ) );

	for( i = 0; i < cnt; i++ )
		DMRow[ i ] = rows.GetAt( i );
	
	return DMRow;
}

INT CMNG_MNGVALUE_OPERATOR_02::DeleteData( INT *rows, INT cnt )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_02") );
	CString query, pk;
	
	for( INT i = cnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), rows[ i ], pk );
		query.Format( _T("DELETE FROM CO_LASTMNGNUM_MANAGE_TBL WHERE REC_KEY = %s;"), pk );
		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( rows[ i ] );
	}
	
	return 0;
}

CString CMNG_MNGVALUE_OPERATOR_02::GetCodeDesc( CString className, CString code )
{
	CString desc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc( className, code, desc );

	return desc;
}

VOID CMNG_MNGVALUE_OPERATOR_02::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_MNGVALUE_OPERATOR_02"), _T("그리드") );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGVALUE_OPERATOR_02") );
	INT rowCnt = pDM->GetRowCount();

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	for( INT i = 1; i < rowCnt + 1; i++ )
	{
		pGrid->SetRow( i );
		
		pGrid->SetCol( 2 );
		pGrid->SetText( GetCodeDesc( _T("등록구분"), pGrid->GetText() ) );

		pGrid->SetCol( 4 );
		pGrid->SetText( GetCodeDesc( _T("등록구분"), pGrid->GetText() ) );
	}

	pGrid->SetRow( row );
	pGrid->SetCol( col );
}

VOID CMNG_MNGVALUE_OPERATOR_02::OnOK() 
{
	// TODO: Add extra validation here
	
	// CDialog::OnOK();
}
HBRUSH CMNG_MNGVALUE_OPERATOR_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
