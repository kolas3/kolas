// BOBookAppViewer.cpp : implementation file
//

#include "stdafx.h"
#include "BOBookAppViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOBookAppViewer dialog


CBOBookAppViewer::CBOBookAppViewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOBookAppViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBOBookAppViewer::~CBOBookAppViewer()
{
	
}

BOOL CBOBookAppViewer::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CBOBookAppViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOBookAppViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOBookAppViewer, CDialog)
	//{{AFX_MSG_MAP(CBOBookAppViewer)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOBookAppViewer message handlers

BOOL CBOBookAppViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_BUSINESS_SEARCHER_05") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_BUSINESS_SEARCHER_05") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOBookAppViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	ViewAppendixInfo();
}

VOID CBOBookAppViewer::ViewAppendixInfo()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_05") );
	pDM->RefreshDataManager( MakeSelectQuery( m_bookKey ) );
	
	if( pDM->GetRowCount() )
		ShowGrid();
	else
	{
		AfxMessageBox( _T("부록 정보가 없습니다.") );
		PostMessage( WM_CLOSE );
	}
}

CString CBOBookAppViewer::MakeSelectQuery(CString pk)
{
	CString where;
	where.Format( _T(" SPECIES_KEY = '%s' "), pk );

	return where;
}

VOID CBOBookAppViewer::ShowGrid()
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

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_05") );

	// 자료실 구분 , 자료 상태 코드 -> 설명
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("자료실구분") , _T("SHELF_LOC_CODE") );
	
	// 2005-01-11 송승민 
	// 매체구분 코드 -> 설명.
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("매체구분") , _T("MEDIA_CODE") );

	CString sWorkingStatus;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("WORKING_STATUS") , i , sWorkingStatus );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1);
		pDM->SetCellData( _T("WORKING_STATUS") , sWorkingStatus , i );
		
	}
	

	INT ids = AllControlDisplay(_T("CM_MNG_BUSINESS_SEARCHER_05"));
	if( ids < 0 )
	{
		AfxMessageBox(_T("뿌려주는데 에러"));
		return ;
	}
}

HBRUSH CBOBookAppViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


BEGIN_EVENTSINK_MAP(CBOBookAppViewer, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOBookAppViewer)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
