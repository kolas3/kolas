// BO_LOC_11001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_11001.h"
#include "BO_LOC_1150.h"
#include "..\\..\\..\\..\\관리\\검색\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\관리\\검색\\BOBookDetailViewer\\BOBookAppViewer.h"
#include "BO_LOC_1160.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1100 dialog


CBO_LOC_1100::CBO_LOC_1100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nMouseRow = -1;
}

CBO_LOC_1100::~CBO_LOC_1100()
{
}

VOID CBO_LOC_1100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1100, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1100)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_REFRESH_GRID, OnRefreshGrid)		
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1100 message handlers

BOOL CBO_LOC_1100::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_1100::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MAINGRID);
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(0, 0, cx, cy);
	
}

BOOL CBO_LOC_1100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;

	ids = InitESL_Mgr( _T("BO_LOC_1100") );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1100::OnInitDialog()") )

	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1100::OnInitDialog()") )
		
	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , NULL );

	return TRUE;  

EFS_END
	return FALSE;
}

BEGIN_EVENTSINK_MAP(CBO_LOC_1100, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_1100)
	ON_EVENT(CBO_LOC_1100, IDC_MAINGRID, -601 /* DblClick */, OnDblClickMaingrid, VTS_NONE)
	ON_EVENT(CBO_LOC_1100, IDC_MAINGRID, -605 /* MouseDown */, OnMouseDownMaingrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_1100::OnDblClickMaingrid() 
{
	CString sCMAlias = _T("CM_BO_LOC_1100");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );

	INT nMouseRow = m_nMouseRow;
	if (m_nMouseRow == -1) nMouseRow = pGrid->GetMouseRow();	
	
	PostMessage(WM_USER_REFRESH_GRID);
	
	if (nMouseRow < 1) return;
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() > 0 )
	{
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );
		pGrid->SelectMakeList();
	}
	DetailView();
}

INT CBO_LOC_1100::MakePrintDM(CESL_DataMgr *pDM)
{
	CESL_DataMgr *DefaultpDM = FindDM( _T("DM_BO_LOC_1100") );
	if( !DefaultpDM->GetRowCount() )
		return 0;

	pDM->FreeData();
	CArray< INT, INT >checkedRow;
	INT printRowCnt = GetCheckedRow( checkedRow );
	
	for( INT i = 0; i < printRowCnt; i++ )
		CopyDM( DefaultpDM, pDM, i );
	
	pDM->SORT( _T("자료실") );
	
	return 0;
}

INT CBO_LOC_1100::CopyDM( CESL_DataMgr *from, CESL_DataMgr *to, INT index )
{
	if( from->GetRowCount() - 1 < index )
		return -1;
	
	INT refCnt = from->RefList.GetCount();
	if( !refCnt )
		return -2;

	CString *colAlias = new CString[ refCnt ];
	CString *data	  = new CString[ refCnt ];
	from->GetAllAlias( colAlias, refCnt );
	
	from->GetCellData( colAlias, refCnt, data, index );
	to->SetCellData( colAlias, refCnt, data, -1 );
	
	return 0;
}


INT CBO_LOC_1100::GetCheckedRow( CArray< INT, INT >&checkedRow )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_MAINGRID );
	INT rowCnt = pGrid->GetRows();
	for( INT i = 1; i < rowCnt; i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );

	if( !checkedRow.GetSize() )
		for( i = 1; i < rowCnt; i++ )
			checkedRow.Add( i - 1 );
		
	return 0;
}

INT CBO_LOC_1100::PrintListEx( CString passDate, INT paperType )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_1100_PRINT") );
	if( MakePrintDM( pDM ) )
		return 0;
	
	CSIReportManager *pSIReportManager = new CSIReportManager( this );
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	pSIReportManager->SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
	pSIReportManager->GetSIReportFileFromDB(_T("단행_자료실별_인계자료_갑지_목록"));

	INT rowCnt = pDM->GetRowCount();
	CString shelfLoc, pastShelfLoc;
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("자료실"), i, shelfLoc );

		if( pastShelfLoc != shelfLoc )
			pSIReportManager->MAKEPAGE_DrawWorkForm( -1 );

		pastShelfLoc = shelfLoc;
	}
	
	if( paperType )
	{
		pSIReportManager->m_nUserPaperSize = -9999;
		pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
		pSIReportManager->m_nResolution = 1728;
	}

	return 0;
}

INT CBO_LOC_1100::PrintList()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_1100") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1100::PrintList()") );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1100::PrintList()") );
	pGrid->SelectMakeList();
	
	INT nMode;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		nSelectCnt = pSrcDM->GetRowCount();
		nMode = 0;
	}
	else
	{
		nMode =1;
	}

	CESL_DataMgr PrintDM;
	CESL_DataMgr PrintDM2;
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , &PrintDM );
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , &PrintDM2 );
	
	if( nMode == 0 )
	{
		PrintDM.FreeData();
		CLocCommonAPI::AddDM( pSrcDM , &PrintDM , this );
	}
	else
	{
		INT nIndex;
		for( INT i = 0;i < nSelectCnt;i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			CLocCommonAPI::AddDM( pSrcDM , &PrintDM , nIndex , -1 , this );
			pGrid->SelectGetNext();
		}
	}
	

	if( PrintDM.GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	CBO_LOC_1150 Dlg(this);
	Dlg.DoModal();

	CString sPassDate = Dlg.m_sPassDate;
	sPassDate.TrimLeft(); sPassDate.TrimRight();

	if( _T("Y") == Dlg.m_strCloseChK_YN )
	{
		return 0;
	}

	if( sPassDate.IsEmpty() ) 
	{
		AfxMessageBox( _T("인계 일자가 없습니다.") );
		return 0;
	}

	CSIReportManager *pSIReportManager = new CSIReportManager( this );
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo* EventInfo ) )SIReportEventHandler;
	if ( !pSIReportManager ) 
	{
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return NULL;
	}

	pSIReportManager->SetCONNECTION_INFO(pSrcDM->CONNECTION_INFO);	
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
	
	CBO_LOC_1160 *pS = new CBO_LOC_1160( this );

	if( Dlg.m_bGabPrintOpt )
	{
		GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("") , m_sDefenceFlag );

		CString sPrintAlias = _T("단행_자료실별_인계자료_갑지_목록");
		if ( m_sDefenceFlag == _T("Y") ) sPrintAlias = _T("단행_자료실별_인계자료_갑지_목록_국방대");
		
		ids = pSIReportManager->GetSIReportFileFromDB( sPrintAlias );
		pS->Create( IDD_BO_LOC_1160, this );
		pS->ShowWindow( FALSE );
		pS->MakeStatistics();
	}
	else if( Dlg.GetInitPaperType() == 0 )
		ids = pSIReportManager->GetSIReportFileFromDB( _T("단행_자료실별_인계자료목록_1") );
	else
		ids = pSIReportManager->GetSIReportFileFromDB( _T("단행_자료실별_인계자료목록") );
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		if ( pSIReportManager )	delete pSIReportManager;
		if ( pS )               delete pS;
		return NULL;
	}

	ids = pSIReportManager->DeleteTempPageFiles();
	pSIReportManager->SetDataMgr( 0, &PrintDM2, 0 );	

	BOOL bIsPrint;
	INT  nPageCnt = 0;
	CString sData;
	CString sShelfLoc;
	CString sTmpShelfLoc;
	CString sPastShelfLoc;	
	
	PrintDM.SORT( _T("자료실") );
	INT nRowCnt = PrintDM.GetRowCount();

	for( INT i = 0;i < nRowCnt;i++ )
	{
		bIsPrint = FALSE;
		
		CLocCommonAPI::AddDM( &PrintDM , &PrintDM2 , i , -1 );
		
		if( ( i == PrintDM.GetRowCount() - 1 ) )
			bIsPrint = TRUE;
		else
		{
			ids = PrintDM.GetCellData( _T("자료실") , i , sShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1100::PrintList()") );
			
			ids = PrintDM.GetCellData( _T("자료실") , i + 1 , sTmpShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_1100::PrintList()") );
						
			if( sShelfLoc.Compare( sTmpShelfLoc ) != 0 )
				bIsPrint = TRUE;
		}

		if( !bIsPrint )    continue;
					
		INT nSpeciesCnt = 0;
		CString sPreSKey;
		CString sCurSKey;		
		CString sOrder;

		PrintDM2.SORT( _T("등록번호") );

		for( INT k = 0;k < PrintDM2.GetRowCount();k++ )
		{
			sOrder.Format( _T("%d") , k + 1 );
			ids = PrintDM2.SetCellData( _T("번호") , sOrder , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_1100::PrintList()") );
			
			ids = PrintDM2.GetCellData( _T("종정보KEY") , k , sCurSKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_1100::PrintList()") );

			if ( sPreSKey != sCurSKey )
			{
				sPreSKey = sCurSKey;
				nSpeciesCnt++;
			}
		}

		ids = PrintDM.GetCellData( _T("자료실") , i , sShelfLoc );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_1100::PrintList()") );

		CString sDocCnt;
		sDocCnt.Format( _T("%d") , k );
		if ( m_sDefenceFlag == _T("Y") )
			sDocCnt.Format( _T("%d/%d") , nSpeciesCnt, k );

		ids = pSIReportManager->SetTempVariable( _T("자료실")   , sShelfLoc );
		ids = pSIReportManager->SetTempVariable( _T("인계일자") , sPassDate );
		ids = pSIReportManager->SetTempVariable( _T("총책수")   , sDocCnt );

		if( Dlg.m_bGabPrintOpt )
			if( !i || sPastShelfLoc != sShelfLoc )
				pS->MakePrintPage( pSIReportManager, nPageCnt++ );

		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBO_LOC_1100::PrintList()") );

		PrintDM2.FreeData();
		
		sPastShelfLoc = sShelfLoc;
	}
	
	if( !Dlg.m_paperType )
	{
		pSIReportManager->m_nUserPaperSize = -9999;
		pSIReportManager->m_nOrientation   = DMORIENT_PORTRAIT;
		pSIReportManager->m_nResolution    = 1728;
	}
	
	ids = pSIReportManager->Print2();
	if ( pSIReportManager ) 	delete pSIReportManager;
	if ( pS )			     	delete pS;
	
	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_1100::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_1100::DetailView()
{
	EFS_BEGIN
		
	CString sParentCMAlias = _T("CM_BO_LOC_1100");
	CString sParentDMAlias = _T("DM_BO_LOC_1100");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("DetailView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("DetailView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("DetailView") );

	
	if( pParentDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("자료를 검색해주십시오") );
		return 0;
	}

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;
	
	INT nMode = 0;
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	INT	nStartPos = 0;

 	INT nTmpStartPos = pGrid->GetIdx();
 	if( nSelectCnt > 0 )
 	{
 		nMode = 1;
 		pGrid->SelectGetHeadPosition();
 	}
 	else
 	{
 		nSelectCnt = pParentDM->GetRowCount();
 		nStartPos = pGrid->GetIdx();
 	}
 	
 	INT nIndex;
 	for( INT i = 0 ; i < nSelectCnt ; i++ )
 	{
 		if( nMode == 1 )
 			nIndex = pGrid->SelectGetIdx();
 		else
 			nIndex = i;
 		
 		pParentDM->GetCellData( _T("종정보KEY") , nIndex, sSpeciesKey );
 		sSpeciesKeyList.AddTail( sSpeciesKey );
 		asGridRow.Add( nIndex );
 
 		if( nIndex == nTmpStartPos )
 		{
 			nStartPos = i;
 		}
 		
 		if( nMode == 1)
 			pGrid->SelectGetNext();
 		
 	}

	CBOBookDetailViewerDlg Dlg(this);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();
	
	return 0;
	
	EFS_END
	return -1;
	
}

INT CBO_LOC_1100::AppView()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_1100");
	CString sParentDMAlias = _T("DM_BO_LOC_1100");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("AppView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("AppView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("AppView") );
	
	CString sSpeciesKey;

	if( pParentDM->GetRowCount() < 1 )
		return 0;

	
	INT nIndex = pGrid->GetIdx();

	pParentDM->GetCellData( _T("종정보KEY") , nIndex , sSpeciesKey );

	CBOBookAppViewer dlg( this );
	dlg.m_bookKey = sSpeciesKey;
	dlg.DoModal();
	

	return 0;

	EFS_END

	return -1;
}


INT CBO_LOC_1100::SimplePrintList()
{
	EFS_BEGIN

	INT ids;
		
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_1100") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pGrid->SelectMakeList();
	
	INT nMode;
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		nSelectCnt = pSrcDM->GetRowCount();
		nMode = 0;
	}
	else
	{
		nMode =1;
	}
	
	CESL_DataMgr PrintDM;
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , &PrintDM );
	
	if( nMode == 0 )
	{
		PrintDM.FreeData();
		CLocCommonAPI::AddDM( pSrcDM , &PrintDM , this );
	}
	else
	{
		INT nIndex;
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			CLocCommonAPI::AddDM( pSrcDM , &PrintDM , nIndex , -1 , this );
			pGrid->SelectGetNext();
		}
	}
	
	
	if( PrintDM.GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	CString sOrder;
	for( INT i = 0 ; i < PrintDM.GetRowCount() ; i++ )
	{
		sOrder.Format( _T("%d") , i+1 );
		PrintDM.SetCellData( _T("번호") , sOrder , i  );
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(PrintDM.CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("단행_배가기록자료"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}
	
	pSIReportManager->SetDataMgr(0, &PrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;

	EFS_END
	
	return -1;
}


LRESULT CBO_LOC_1100::OnRefreshGrid(WPARAM wParam, LPARAM lParam) 
{
	EFS_BEGIN
		
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MAINGRID);
	pGrid->SetRowHeight(0, 400);
	
	return 0;
	
	EFS_END
		
	return -1;
}

VOID CBO_LOC_1100::OnMouseDownMaingrid( short Button, short Shift, long x, long y ) 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_1100");
	CString sGridAlias = _T("MainGrid");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );
	
	m_nMouseRow = pGrid->GetMouseRow();

	EFS_END
}

HBRUSH CBO_LOC_1100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
