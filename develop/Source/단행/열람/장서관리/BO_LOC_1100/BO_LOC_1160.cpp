// BO_LOC_1160.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1160.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1160 dialog


CBO_LOC_1160::CBO_LOC_1160(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1160)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pStatistics = NULL;	

	GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("") , m_sDefenceFlag );	
	m_sDefenceFlag.TrimLeft();m_sDefenceFlag.TrimRight();
}

CBO_LOC_1160::~CBO_LOC_1160()
{
	if( m_pStatistics != NULL )
	{
		for( INT i = 0 ; i < m_saShelfLoc.GetSize() ; i++ )
			delete m_pStatistics[i];
		delete []m_pStatistics;
	}
	m_pStatistics = NULL;
	
}


VOID CBO_LOC_1160::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1160)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1160, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1160)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabSHELF_LOC, OnSelchangetabSHELFLOC)
	ON_BN_CLICKED(IDC_btnSPRINT, OnbtnSPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1160 message handlers

BOOL CBO_LOC_1160::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;
	ids = InitESL_Mgr(_T("BO_LOC_1160"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
		
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	ids = InitScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	ids = MakeStatistics();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	if( ids > 0 ) EndDialog(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
	return FALSE;
}

VOID CBO_LOC_1160::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return;

	const INT nCnt = 5;
	CRect WndPos[nCnt];
	CWnd* Wnd[nCnt];
	
	INT nIDC[nCnt] = 
	{
		IDC_staTITLE2 , IDC_gridSTATISTICS ,  IDC_tabSHELF_LOC , IDC_btnClose , IDC_btnSPRINT
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		Wnd[i] = GetDlgItem(nIDC[i]);
		if( Wnd[i]->GetSafeHwnd() == NULL ) return;
		Wnd[i]->GetWindowRect(WndPos[i]);
		ScreenToClient(WndPos[i]);
	}
	
	Wnd[0]->MoveWindow( 0 , WndPos[0].top , cx , WndPos[0].Height() );	
	Wnd[2]->MoveWindow( 0 , WndPos[2].top , cx , WndPos[2].Height() );
	Wnd[3]->MoveWindow( cx - WndPos[3].Width() - 5 , WndPos[3].top , WndPos[3].Width() , WndPos[3].Height() );
	Wnd[4]->MoveWindow( cx - WndPos[4].Width() - WndPos[3].Width() - 10 , WndPos[4].top , WndPos[4].Width() , WndPos[4].Height() );

	INT nPos = cy - WndPos[1].top  - 10;
	Wnd[1]->MoveWindow( 0 , WndPos[1].top , cx , nPos );
	
	if ( m_sDefenceFlag == _T("Y") )
	{
		Wnd[1]->MoveWindow( 0 , WndPos[1].top , cx , nPos/2  );

		CWnd* pWnd = GetDlgItem( IDC_gridSTATISTICS2 );
		if ( !pWnd ) return;

		pWnd->MoveWindow( 0 , WndPos[1].top + (nPos/2) + 5 , cx  , nPos/2 - 5  );
	}
EFS_END
}

INT CBO_LOC_1160::MakeStatistics()
{
EFS_BEGIN

	INT ids;
	
	CESL_DataMgr* pParentDM = GetParentDM();
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeStatistics") );

	INT nSelectCnt = pParentDM->GetRowCount();
	if( nSelectCnt < 1 ) 
	{
		AfxMessageBox( _T("자료를 검색해 주십시오") );
		return 1;
	}
	
	CESL_DataMgr SrcDM;
	ids = MakeSrcDM(&SrcDM);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeStatistics") );

	ids = MakeShelfLocList(&SrcDM);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeStatistics") );
	
	ids = MakeStatisticsList(&SrcDM);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeStatistics") );

	ids = ShowScreen();
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("MakeStatistics") );	

	return 0;

EFS_END
	return -1;
}

CESL_Grid* CBO_LOC_1160::GetParentGridPtr()
{
	EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1100") , _T("MainGrid") );

	return pGrid;

	EFS_END

	return NULL;
}

CESL_DataMgr* CBO_LOC_1160::GetParentDM()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_1100") );

	return pDM;

	EFS_END

	return NULL;
}


INT CBO_LOC_1160::MakeSrcDM(CESL_DataMgr *pDstDM)
{
	EFS_BEGIN

	
	CESL_DataMgr *pSrcDM = GetParentDM();
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	
	CESL_Grid* pGrid = GetParentGridPtr();
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
	
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , pDstDM );
	
	if( nMode == 0 )
	{
		pDstDM->FreeData();
		CLocCommonAPI::AddDM( pSrcDM , pDstDM , this );
	}
	else
	{
		INT nIndex;
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			CLocCommonAPI::AddDM( pSrcDM , pDstDM , nIndex , -1 , this );
			pGrid->SelectGetNext();
		}
	}

	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_1160::MakeShelfLocList( CESL_DataMgr* pSrcDM )
{
EFS_BEGIN

	INT ids;

	CString sShelfLoc;	
	BOOL bIsInsert;

	m_saShelfLoc.RemoveAll();
	pSrcDM->SORT(_T("자료실"));	
	
	for( INT i = 0 ; i < pSrcDM->GetRowCount() ; i++ )
	{
		ids = pSrcDM->GetCellData(_T("자료실") , i , sShelfLoc);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeDivFromShelfLoc") );

		bIsInsert = TRUE;
		for( INT j = 0 ; j < m_saShelfLoc.GetSize() ; j++ )
		{
			if( sShelfLoc.Compare( m_saShelfLoc.GetAt(j) ) == 0 )
				bIsInsert = FALSE;
		}

		if( bIsInsert )
			m_saShelfLoc.Add(sShelfLoc);
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_1160::MakeStatisticsList( CESL_DataMgr* pSrcDM )
{
EFS_BEGIN

	INT ids;

	CString strAppendixBookIncludeYN;
	GetManageValue( _T("정리") , _T("단행") , _T("류별통계 부록포함여부") , _T("") , strAppendixBookIncludeYN );	

	INT nShelfCnt = m_saShelfLoc.GetSize();

	m_pStatistics = new CESL_DataMgr*[nShelfCnt];

	CString sDMAlias = _T("DM_BO_LOC_1160");
	if ( m_sDefenceFlag == _T("Y") ) sDMAlias = _T("DM_BO_LOC_1160_ND");

	CESL_DataMgr *pShapeDM = FindDM( sDMAlias );
	if ( !pShapeDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1160::MakeStatisticsList") );
	
	for( INT i = 0;i < nShelfCnt;i++ )
	{
		m_pStatistics[i] = new CESL_DataMgr;
		CLocCommonAPI::MakeCopyDM( this , pShapeDM , m_pStatistics[i] );
		ids = MakeStatisticsShape( m_pStatistics[i] );
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1160::MakeStatisticsList") );
	}

	CString sCnt;
	CString sShelfLoc;
	CString sClassNo;
	CString sClassNoType;
	CString sAppendixFlag;
	INT nDMIndex;
	INT nRowIndex;
	CString sColAlias;
	CString strAppendixFlag;

	for( i = 0 ; i < pSrcDM->GetRowCount() ; i++ )
	{
		ids = pSrcDM->GetCellData( _T("부록정보") , i , sAppendixFlag );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1160::MakeStatisticsList") );		
		if ( strAppendixBookIncludeYN == _T("N") && sAppendixFlag == _T("부록") ) continue;

		ids = pSrcDM->GetCellData( _T("자료실") , i , sShelfLoc );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1160::MakeStatisticsList") );		
		GetDMIndex( sShelfLoc , nDMIndex );

		ids = pSrcDM->GetCellData( _T("분류기호구분") , i , sClassNoType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_1160::MakeStatisticsList") );		
		GetRowIndex( sClassNoType , nRowIndex );

		ids = pSrcDM->GetCellData( _T("분류기호") , i , sClassNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_1160::MakeStatisticsList") );		
		GetColAlias( sClassNo , sColAlias , nRowIndex);

		m_pStatistics[nDMIndex]->GetCellData(sColAlias,nRowIndex , sCnt);

		sCnt.Format( _T("%d") , _ttoi(sCnt) + 1 );
		
		m_pStatistics[nDMIndex]->SetCellData(sColAlias,sCnt,nRowIndex);	

		m_pStatistics[nDMIndex]->GetCellData( _T("계"), nRowIndex, sCnt );
		sCnt.Format( _T("%d") , _ttoi(sCnt) + 1 );		
		m_pStatistics[nDMIndex]->SetCellData( _T("계"), sCnt, nRowIndex );
		// ==================================================================================
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_1160::MakeStatisticsShape( CESL_DataMgr* pDM )
{
EFS_BEGIN

	const INT nCnt = 3;
	pDM->FreeData();
	INT nColCnt = pDM->GetColCount();
	
	for( INT i = 0;i < nCnt;i++ )
	{
		pDM->InsertRow(-1);

		for( INT j = 0;j < nColCnt;j++ )
		{
			pDM->SetCellData( i , j , _T("0") );
		}
	}
	
	CString sCodeAlias[nCnt];
	
	sCodeAlias[0] = _T("KDC");
	sCodeAlias[1] = _T("DDC");
	sCodeAlias[2] = _T("기타");	

	for (i = 0;i < nCnt;i++ )
		pDM->SetCellData( _T("분류기호구분") , sCodeAlias[i], i  );

	if ( m_sDefenceFlag == _T("Y") )
	{
		pDM->InsertRow(-1);
		for( INT j = 0;j < nColCnt;j++ )
		{
			pDM->SetCellData( i , j , _T("0"));
		}

		pDM->SetCellData( _T("분류기호구분") , _T("LC"), i  );
	}
	return 0;

EFS_END	
	return -1;	
}

INT CBO_LOC_1160::GetDMIndex( CString sShelfLoc , INT &nDMIndex )
{
EFS_BEGIN

	for( INT i = 0 ; i < m_saShelfLoc.GetSize() ; i++ )
	{
		if( sShelfLoc.Compare(m_saShelfLoc.GetAt(i)) == 0 )
		{
			nDMIndex = i;
			return 0;
		}
	}

	return -1;

EFS_END
	return -1;
}

INT CBO_LOC_1160::GetRowIndex( CString sClassNoType , INT &nRowIndex )
{
EFS_BEGIN

	sClassNoType.TrimLeft(); sClassNoType.TrimRight();

	if( sClassNoType.Compare(_T("1")) == 0 )
		nRowIndex = 0;
	else if( sClassNoType.Compare(_T("2")) == 0 )
		nRowIndex = 1; 
	else if( sClassNoType.Compare(_T("3")) == 0 || sClassNoType.Compare(_T("7")) == 0 )
	{
		if ( m_sDefenceFlag == _T("Y") )
			nRowIndex = 3;
		else
			nRowIndex = 2;
	}
	else
		nRowIndex = 2;

	return 0;

EFS_END	
	return -1;
}

INT CBO_LOC_1160::GetColAlias( CString sClassNo , CString &sColAlias , INT nRowIndex /* = 0 */)
{
EFS_BEGIN

	sClassNo.TrimLeft(); sClassNo.TrimRight();
	
	if( sClassNo.IsEmpty() ) 
	{
		sColAlias = _T("기타");
		return 0;
	}

	TCHAR chClassCode = sClassNo.GetAt(0);
	
	if ( chClassCode > 47 && chClassCode < 58 )
	{
		if ( m_sDefenceFlag == _T("Y") )
		{
			if ( nRowIndex == 3 )
			{
				sColAlias = _T("기타");
				return 0;
			}

			INT nFlag = _ttoi( &chClassCode );

			if ( chClassCode < 56 )
				sColAlias.Format( _T("%c"), 'A' + nFlag );
			else
				sColAlias.Format( _T("%c"), 'A' + nFlag + 1 );			
		}
		else
		{
			sColAlias = sClassNo.Mid( 0 , 1 );
		}
	}
	else
	{
		sColAlias.Empty();

		if ( m_sDefenceFlag == _T("Y") )
		{
			CString sLCCode = _T("ABCDEFGHJKLMNPQRSTUVZ");
			TCHAR* pchResult = _tcsstr( sLCCode, sClassNo.Mid( 0 , 1 ) );
			if ( pchResult != NULL )
				sColAlias = sClassNo.Mid( 0 , 1 );
		}
		
		if ( sColAlias.IsEmpty() )
			sColAlias = _T("기타");
		
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_1160::ShowScreen()
{
EFS_BEGIN

	INT ids;

	ids = MakeTabCtrl();
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1160::ShowScreen") );

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabSHELF_LOC);
	if ( !pTab ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1160::ShowScreen") );
	
	for( INT i = pTab->GetItemCount() - 1;i > -1;i-- )
		ShowGridFromDM( i );
	
	return 0;

EFS_END	
	return -1;
}

INT CBO_LOC_1160::MakeTabCtrl()
{
EFS_BEGIN
		
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabSHELF_LOC);
	if ( !pTab ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1160::MakeTabCtrl") );
	pTab->DeleteAllItems();

	for ( INT i = 0;i < m_saShelfLoc.GetSize();i++ )
		pTab->InsertItem( i , m_saShelfLoc.GetAt(i) );	
	
	return 0;
	
EFS_END
	return -1;
}

INT CBO_LOC_1160::ShowGridFromDM( INT index )
{
EFS_BEGIN

	INT ids;

	INT nDMIndex;
	if( -1 == index )
	{
		CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabSHELF_LOC);
		if ( !pTab ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1160::ShowGridFromDM") );
		nDMIndex = pTab->GetCurSel();
	}
	else
		nDMIndex = index;

	const INT nCnt = 13;
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTATISTICS);
	if ( !pGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1160::ShowGridFromDM") );
	pGrid->SetRows( 4 );
	pGrid->SetCols( 0, nCnt);

	CString colDesc[ nCnt ] = {
		_T("분류기호구분"), _T("총류"), _T("철학"), _T("종교"), _T("사회과학"), _T("순수과학"), _T("기술과학"), _T("예술"), _T("어학"), _T("문학"), _T("역사"), _T("기타"), _T("계") };

	for( INT k = 0;k < nCnt;k++ )
		pGrid->SetTextMatrix( 0 , k , colDesc[k] );
	
	const INT nLCCnt = 24;
	CMSHFlexGrid* pLCGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTATISTICS2);
	if ( !pLCGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1160::ShowGridFromDM") );
	pLCGrid->SetRows( 2 );
	pLCGrid->SetCols( 0, nLCCnt);
	
	CString colLCDesc[ nLCCnt ] = {
		_T("분류기호구분"), _T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), _T("H"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), _T("P"), 
			_T("Q"), _T("R"), _T("S"), _T("T"), _T("U"), _T("V"), _T("Z"), _T("기타"), _T("계") };

	for ( k = 0;k < nLCCnt;k++ )
		pLCGrid->SetTextMatrix( 0 , k , colLCDesc[k] );

	CString sColumnName[ nCnt ] =
	{
		_T("분류기호구분") , _T("0")    , _T("1") , _T("2") , _T("3") , 
		_T("4")            , _T("5")    , _T("6") , _T("7") , _T("8") , 
		_T("9")            , _T("기타") , _T("계")
	};

	CString sResult;
	INT nSum;
	for( INT i = 0;i < 3;i++ )
	{
		nSum = 0;
		for( INT j = 0;j < nCnt;j++ )
		{
			CString sCurColName = sColumnName[j];
			if ( m_sDefenceFlag == _T("Y") ) sCurColName = colLCDesc[j];
			ids = m_pStatistics[nDMIndex]->GetCellData( sCurColName , i , sResult );
			if( ids < 0 )
			{
				CString sMsg;
				sMsg.Format( _T("m_pStatistics[%d]->GetCellData( sColumnName[%d] , %d , sResult )") , nDMIndex , j , i );
				AfxMessageBox( sMsg );
				ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_1160::ShowGridFromDM") );
			}			

			if( (j != 0) && (j != ( nCnt - 1 )) )
				nSum += _ttoi( sResult );			
			
			if ( j == ( nCnt - 2 ) )
			{
				ids = m_pStatistics[nDMIndex]->GetCellData( _T("기타") , i , sResult );	
				m_pStatistics[nDMIndex]->SetCellData( _T("기타") , sResult , i );
			}
		
			if( j == (nCnt-1) )
			{
				sResult.Format( _T("%d") , nSum );
				m_pStatistics[nDMIndex]->SetCellData( _T("계") , sResult , i );
			}
			
			pGrid->SetTextMatrix( i+1 , j , sResult );	
		}
	}

	if ( m_sDefenceFlag == _T("Y") )
	{
		nSum = 0;
		for( INT j = 0;j < nLCCnt;j++ )
		{
			ids = m_pStatistics[nDMIndex]->GetCellData( colLCDesc[j] , i , sResult );				
			if( ids < 0 )
			{
				CString sMsg;
				sMsg.Format( _T("m_pStatistics[%d]->GetCellData( sColumnName[%d] , %d , sResult )") , nDMIndex , j , i );
				AfxMessageBox( sMsg );
				ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_1160::ShowGridFromDM") );
			}			

			if( (j != 0) && (j != ( nLCCnt - 1 )) )
				nSum += _ttoi( sResult );			

			if( j == (nLCCnt-1) )
			{
				sResult.Format( _T("%d") , nSum );
				m_pStatistics[nDMIndex]->SetCellData( _T("계") , sResult , i );
			}
			
			pLCGrid->SetTextMatrix( 1 , j , sResult );	
		}
	}

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_1160::OnbtnClose() 
{
	OnCancel();	
}

VOID CBO_LOC_1160::OnSelchangetabSHELFLOC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowGridFromDM();
	
	*pResult = 0;
}

VOID CBO_LOC_1160::OnbtnSPRINT() 
{
	Print();	
}

INT CBO_LOC_1160::Print()
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_1160");
	if ( m_sDefenceFlag == _T("Y") ) sDMAlias = _T("DM_BO_LOC_1160_ND");

	CESL_DataMgr* pPrintDM = FindDM( sDMAlias );
	if ( !pPrintDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1160::Print()") );

	CSIReportManager *pSIReportManager = NULL;
	pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return NULL;
	}

	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);	
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;

	CString sSIReportAlias = _T("단행_자료실별_인계자료통계");
	if ( m_sDefenceFlag == _T("Y") ) sSIReportAlias = _T("단행_자료실별_인계자료통계_국방대");
	ids = pSIReportManager->GetSIReportFileFromDB( sSIReportAlias );
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return NULL;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();
	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	CString sPassDate;
	ids = GetControlData( _T("CM_BO_LOC_1160") , _T("인계일자") , sPassDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1160::Print()") );
	
	INT nSum;
	CString sBookCnt;
	
	CString sPrintPosition;
	for ( INT i = 0;i < m_saShelfLoc.GetSize();i++ )
	{
		sBookCnt = _T("");
		nSum = 0;

		for ( INT j = 0;j < m_pStatistics[i]->GetColCount();j++ )
		{
			for ( INT r = 0;r < m_pStatistics[i]->GetRowCount();r++ )
			{
				sPrintPosition.Empty();
				sPrintPosition.Format( _T("%d_%d"), r, j );
				m_pStatistics[i]->GetCellData( r, j , sBookCnt );
				ids = pSIReportManager->SetTempVariable( sPrintPosition , sBookCnt );			
			}
		}

		for( INT m = 0;m < 3;m++ )
		{
			m_pStatistics[i]->GetCellData( _T("계") , m , sBookCnt );
			nSum += _ttoi(sBookCnt);
		}

		sBookCnt.Format( _T("%d") , nSum );

		ids = pSIReportManager->SetTempVariable( _T("자료실") , m_saShelfLoc.GetAt(i) );
		ids = pSIReportManager->SetTempVariable( _T("인계일자") , sPassDate );
		ids = pSIReportManager->SetTempVariable( _T("책수") , sBookCnt );
		
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1160::Print()") );

		
	}
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
	
	
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1160::MakePrintPage( CSIReportManager *pSIReportMgr, INT index )
{	
	INT ids;

	CString sPassDate;
	ids = GetControlData( _T("CM_BO_LOC_1160") , _T("인계일자") , sPassDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1160::MakePrintPage()") );

	pSIReportMgr->SetTempVariable( _T("인계일자") , sPassDate );
	
	pSIReportMgr->SetTempVariable( _T("자료실") , m_saShelfLoc.GetAt( index ) );
	
	const INT nColCnt = 13;
	CString alias[ nColCnt ] = {
		_T("분류기호구분"), _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("기타"), _T("계") };
	
	CString data;

	const INT nLCColCnt = 24;
	CString colLCDesc[ nLCColCnt ] = {
		_T("분류기호구분"), _T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), _T("H"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), _T("P"), 
			_T("Q"), _T("R"), _T("S"), _T("T"), _T("U"), _T("V"), _T("Z"), _T("기타"), _T("계") };

	CString sColAlias;

	for( INT i = 0;i < m_pStatistics[ index ]->GetRowCount();i++ )
	{
		INT nCnt = nColCnt;
		if ( m_sDefenceFlag == _T("Y") )	nCnt = nLCColCnt;

		for( INT j = 0;j < nCnt;j++ )
		{
			if ( m_sDefenceFlag == _T("Y") )	sColAlias = colLCDesc[j];
			else							sColAlias = alias[j];

			ids = m_pStatistics[ index ]->GetCellData( sColAlias, i, data );

			sColAlias.Format( _T("%d_%s"), i, sColAlias );
			pSIReportMgr->SetTempVariable( sColAlias, data );
		}
	}

	INT nSum = 0;
	CString sBookCnt;
	for( INT m = 0;m < 3;m++ )
	{
		m_pStatistics[ index ]->GetCellData( _T("계") , m , sBookCnt );
		nSum += _ttoi( sBookCnt );
	}
	
	if ( m_sDefenceFlag == _T("Y") )
	{
		CString sSpeciecsCnt;
		GetDlgItemText( IDC_edtSpeciesCnt, sSpeciecsCnt );
		sBookCnt.Format( _T("%s/%d") , sSpeciecsCnt , nSum );
	}
	else
	{
		sBookCnt.Format( _T("%d") , nSum );	
	}
	
	ids = pSIReportMgr->SetTempVariable( _T("책수") , sBookCnt );

		
	return 0;
}


INT CBO_LOC_1160::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1160::InitScreen()
{
	CRect WndPos;
	GetWindowRect( WndPos );
	
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width()   , WndPos.Height() );
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width()+1 , WndPos.Height() );

	if ( m_sDefenceFlag != _T("Y") )
	{
		CWnd* pWnd = GetDlgItem( IDC_gridSTATISTICS2 );
		if ( !pWnd ) return -1;
		pWnd->ShowWindow( SW_HIDE );
	}

	return 0;
}

HBRUSH CBO_LOC_1160::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
