// SE_LOC_14001.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_14001.h"
#include "..\\..\\..\\관리\\검색\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1400 dialog


CSE_LOC_1400::CSE_LOC_1400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1400::~CSE_LOC_1400()
{
}

VOID CSE_LOC_1400::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1400, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1400)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1400 message handlers

BOOL CSE_LOC_1400::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1400")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_1400( this );

	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("GRID") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SE_LOC_1400"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1400::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	//=====================================================
	//2009.01.29 UPDATE BY PJW : 그리드에 스크롤 나오도록 함
	//pGrid->MoveWindow(-2, -2, cx+4, cy+4);
	pGrid->MoveWindow(-2, 35, cx+2, cy-35);
	//=====================================================
	
}

BOOL CSE_LOC_1400::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


INT CSE_LOC_1400::PrintList()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1400") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1400_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	// pSrcDM -> pPrintDM으로 복사
	const INT nCopyCnt=4;
	pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("청구기호") , _T("청구기호") } , 
		{ _T("서명") , _T("서명") } , 
		{ _T("등록번호") , _T("등록번호") } , 
		{ _T("차수번호") , _T("차수번호") }
	};

	CESL_Grid *pGrid = (CESL_Grid *)FindControl( _T("CM_SE_LOC_1400") , _T("GRID") );
	if( pGrid == NULL ) return -1;
	pGrid->SelectMakeList();
	
	INT nSelectCnt;
	nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	
	CString sData;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("제본상태") , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

		if( sData.Compare( _T("의뢰") ) != 0 )
		{
			if( nMode == 1 )
				pGrid->SelectGetNext();
			continue;
		}

		pPrintDM->InsertRow(-1);

		INT nInsertIndex = pPrintDM->GetRowCount()-1;

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , j  , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

			ids = pPrintDM->SetCellData( sField[j][1] , sData , nInsertIndex );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , j , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1 )
			pGrid->SelectGetNext();
		
	}

	if( pPrintDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("검색된 자료중 제본 의뢰 자료가 존재하지 않습니다.") );
		return 1;
	}

	// 등록 번호 순으로 정렬한 다음 번호 부여	
	pPrintDM->SORT( _T("차수번호") );

	CString sNumber;
	for( i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		ids = pPrintDM->GetCellData( _T("차수번호") , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

		sNumber.Format( _T("%s - %d") , sData , i+1 );

		ids = pPrintDM->SetCellData( _T("번호") , sNumber , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
	}
	

	// 일반적인 반복물 출력
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속수리제본의뢰도서목록"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);

	CString sShelfLoc;
	ids = GetControlData( _T("CM_SE_LOC_1400") , _T("자료실") , sShelfLoc , -1 , 1);
	pSIReportManager->SetTempVariable( _T("소속") , sShelfLoc );

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1400::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}


INT CSE_LOC_1400::DetailView()
{
	EFS_BEGIN
		
	CString sParentCMAlias = _T("CM_SE_LOC_1400");
	CString sParentDMAlias = _T("DM_SE_LOC_1400");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("DetailView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("DetailView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("GRID") ));
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
	
	INT nStartPos = 0;
	INT nTmpStartPos = pGrid->GetIdx();
	if( nSelectCnt > 1 )
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
			nStartPos = i;
		
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	CSEBookDetailViewerDlg Dlg(this);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();
	
	return 0;
	
	EFS_END
	return -1;
	
}

BEGIN_EVENTSINK_MAP(CSE_LOC_1400, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_LOC_1400)
	ON_EVENT(CSE_LOC_1400, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_LOC_1400::OnDblClickgridMAIN() 
{
	// 상세 정보 보여주기	
	CString sCMAlias = _T("CM_SE_LOC_1400");
	CString sGridAlias = _T("GRID");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );
	
	if( pGrid->GetMouseRow() < 1 ) return;
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() > 0 )
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	
	DetailView();
}

HBRUSH CSE_LOC_1400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
