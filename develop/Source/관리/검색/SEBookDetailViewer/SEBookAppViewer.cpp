// SEBookAppViewer.cpp : implementation file
//

#include "stdafx.h"
#include "SEBookAppViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEBookAppViewer dialog


CSEBookAppViewer::CSEBookAppViewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSEBookAppViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSEBookAppViewer::~CSEBookAppViewer()
{

}

BOOL CSEBookAppViewer::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CSEBookAppViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEBookAppViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSEBookAppViewer, CDialog)
	//{{AFX_MSG_MAP(CSEBookAppViewer)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEBookAppViewer message handlers

BOOL CSEBookAppViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if( InitESL_Mgr( _T("MNG_BUSINESS_SEARCHER_05_SE") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_BUSINESS_SEARCHER_05") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSEBookAppViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	ViewAppendixInfo();
}

VOID CSEBookAppViewer::ViewAppendixInfo()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_05_SE") );
	pDM->RefreshDataManager( MakeSelectQuery( m_bookKey ) );
	
	if( pDM->GetRowCount() )
		ShowGrid();
	else
	{
		AfxMessageBox( _T("부록 정보가 없습니다.") );
		PostMessage( WM_CLOSE );
	}
}

CString CSEBookAppViewer::MakeSelectQuery(CString pk)
{
	CString where;
	where.Format( _T(" B.SPECIES_KEY = '%s' "), pk );

	return where;
}

VOID CSEBookAppViewer::ShowGrid()
{
	/*
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_05") );
	CString alias[ 7 ] = {
		_T("VOL_TITLE"), _T("VOL"), _T("AUTHOR"), _T("PUBLISH_YEAR"), _T("SHELF_LOC_CODE"), _T("REG_NO"), _T("SHELF_STATUS_CODE") };
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
		
	CString data[ 7 ];
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pGrid->SetRow( i + 1 );
		pDM->GetCellData( alias, 7, data, i );
		
		for( INT j = 0; j < 7; j++ )
		{
			pGrid->SetCol( j + 1 );
			pGrid->SetText( data[ j ] );
		}
	}
	pGrid->SetRow( 0 );
	pGrid->SetCol( 0 );
	*/

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_05_SE") );

	// 자료실 구분 , 자료 상태 코드 -> 설명
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("자료실구분") , _T("SHELF_LOC_CODE") );
	// 2005-01-11 송승민
	// 매체구분, 메체구분 코드 -> 설명 추가.
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("매체구분") , _T("MEDIA_CODE") );

	CString sWorkingStatus;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("WORKING_STATUS") , i , sWorkingStatus );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1);
		pDM->SetCellData( _T("WORKING_STATUS") , sWorkingStatus , i );
		
	}
	

	INT ids = AllControlDisplay(_T("CM_MNG_BUSINESS_SEARCHER_05_SE"));
	if( ids < 0 )
	{
		AfxMessageBox(_T("뿌려주는데 에러"));
		return ;
	}
}

BEGIN_EVENTSINK_MAP(CSEBookAppViewer, CDialog)
    //{{AFX_EVENTSINK_MAP(CSEBookAppViewer)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


HBRUSH CSEBookAppViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

