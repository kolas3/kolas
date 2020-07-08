// AC_SEARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "AC_SEARCH_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAC_SEARCH_DLG dialog


CAC_SEARCH_DLG::CAC_SEARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CAC_SEARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strSearchItem = _T("");
	m_pCM = NULL;
	m_pDM = NULL;
	m_pRefDM = NULL;
	m_strSearchText = _T("");

}

CAC_SEARCH_DLG::~CAC_SEARCH_DLG()
{
}

VOID CAC_SEARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAC_SEARCH_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAC_SEARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CAC_SEARCH_DLG)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAC_SEARCH_DLG message handlers


VOID CAC_SEARCH_DLG::OnbClose() 
{
	CDialog::OnCancel();	
}


BOOL CAC_SEARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_AC_SEARCH")) < 0)
		return FALSE;

	m_pCM = FindCM( _T("CM_AC_SEARCH") );
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox( _T("CM 초기화 에러") );
		return FALSE;
	}

	m_pDM = FindDM ( _T("DM_AC_SEARCH") );
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox( _T("DM 초기화 에러") );
		return FALSE;
	}

	m_pRefDM = FindDM ( _T("DM_AC_REFITEM") );
	if ( m_pRefDM == NULL ) 
	{
		AfxMessageBox( _T("DM 초기화 에러") );
		return FALSE;
	}


	if ( !InitSearchAC() ) 
	{
		CDialog::OnCancel();
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAC_SEARCH_DLG::InitSearchAC()
{
	if ( m_strSearchItem == _T("") ) 
	{
		AfxMessageBox( _T("검색어가 입력되지 않았습니다.") );
		return FALSE;
	}

	if ( m_strSearchGroup == _T("") ) 
	{
		AfxMessageBox( _T("서명검색인지 저자 검색인지 선택되지 않았습니다.") );
		return FALSE;
	}

	INT ids = -1;
	CString strTBLNAME;
	strTBLNAME.Empty();
	if ( m_strSearchGroup == _T("서명") )
	{	
		m_nAcSearchOpt = 0;
		strTBLNAME = _T("AC_INDEX_TITLE_TBL");
	}
	else if ( m_strSearchGroup == _T("저자") )
	{
		m_nAcSearchOpt = 1;
		strTBLNAME = _T("AC_INDEX_AUTHOR_TBL");
	}
	else 
		strTBLNAME = _T("");


	//채택표목 검색
	if ( !SetGrid1( strTBLNAME ) )
		return FALSE;

	//참조표목 검색
	if ( !SetGrid2( strTBLNAME ) ) 
		return FALSE;

	//DM 에 저장
	SetDM();

	m_pCM->AllControlDisplay();

	return TRUE;
}

BEGIN_EVENTSINK_MAP(CAC_SEARCH_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CAC_SEARCH_DLG)
	ON_EVENT(CAC_SEARCH_DLG, IDC_GRID1, -600 /* Click */, OnClickGrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAC_SEARCH_DLG::OnClickGrid1() 
{
	//Grid1이 선정되었을 경우 자동 검색 되도록..
	INT checkedRow = SetCheckGrid();

	INT ids = -1;
	CString strTBLNAME;
	strTBLNAME.Empty();
	if ( m_strSearchGroup == _T("서명") )
	{	
		m_nAcSearchOpt = 0;
		strTBLNAME = _T("AC_INDEX_TITLE_TBL");
	}
	else if ( m_strSearchGroup == _T("저자") )
	{
		m_nAcSearchOpt = 1;
		strTBLNAME = _T("AC_INDEX_AUTHOR_TBL");
	}
	else 
		strTBLNAME = _T("");

	if( checkedRow > 0 )
		SetGrid2( strTBLNAME, checkedRow-1 );

	
	CESL_Grid *pGrid = (CESL_Grid *)FindControl( m_pCM->GetAlias(), (_T("GRID2")) );	
	pGrid->Display();
	//m_pCM->AllControlDisplay();
	//AutoSearchProc();
	
}

VOID CAC_SEARCH_DLG::SetDM()
{
	CString strStreamMarc;
	INT ids = -1;
	CMarc* pMarc;
	pMarc = new CMarc;

	for ( INT idx = 0; idx < m_pDM->GetRowCount(); idx++ )
	{
		m_pDM->GetCellData( _T("MARC"), idx, strStreamMarc );

		
		ids = m_pInfo->pMarcMgr->Decoding ( strStreamMarc , pMarc );
		if ( ids < 0 ) return;

		//주기 DM에 입력
		CString strItemData;
		m_pInfo->pMarcMgr->GetItem( pMarc , _T("678$a"), strItemData );
		m_pDM->SetCellData ( _T("주기"), strItemData , idx );

		//채택표목
		m_pDM->SetCellData ( _T("채택표목"), m_strSearchItem , idx );

		//생몰년
		m_pDM->SetCellData ( _T("채택표목"), m_strSearchItem , idx );
	}

	if ( pMarc ) 
	{
		delete pMarc;
		pMarc = NULL;
	}

	return;

}

BOOL CAC_SEARCH_DLG::SetGrid1(CString strTBLNAME)
{
	
	CString strQuery;
	strQuery.Empty();
	
	// KEYWORD값을 가지고 각각의 TBL에 쿼리를 날린다.
	// 지금은 서명, 저자 검색만 지원 - K2에서 그것만 했으니까...
	strQuery = _T("REC_KEY IN ( SELECT AC_MASTER_MAT_KEY FROM ") + strTBLNAME + _T(" WHERE KEYWORD = '") + m_strSearchItem + _T("')");

	CString strAC_MASTER_KEY;
	strAC_MASTER_KEY.Empty();
	INT ids = -1;
//	ids = m_pDM->GetOneValueQuery( strQuery, strAC_MASTER_KEY );
//	if ( ids < 0 || strAC_MASTER_KEY.IsEmpty() ) 
//	{
//		CString strTmpMsg;
//		strTmpMsg = m_strSearchGroup + _T("(으)로 검색된 전거가 없습니다.") ;
//		AfxMessageBox( strTmpMsg );
//		return FALSE;
//	}
	
	// 검색된 AC_MASTER_MAT_KEY 를 가지고 Marc와 Rec_key 구해옴..
	//strQuery = _T("REC_KEY = ") + strAC_MASTER_KEY + _T("");
	ids = m_pDM->RefreshDataManager ( strQuery );
	if( ids < 0 ) return FALSE;

	if ( m_pDM->GetRowCount() < 1 )
	{
		AfxMessageBox(_T("검색된 전거가 없습니다."));
		return FALSE;
	}
	return TRUE;
}

BOOL CAC_SEARCH_DLG::SetGrid2( CString strTBLNAME, INT nGridIdx /*=0*/ )
{
	//검색된 채택 표목을 가지고 참조 표목을 검색한다. 
	if ( m_pDM == NULL || strTBLNAME == _T("") ) 
		return FALSE;

	CString strStreamMarc;
	INT ids = -1;
	CMarc* pMarc;
	pMarc = new CMarc;
	CArray<CString , CString& >  arrItem;
	//pArray->RemoveAll();


	m_pDM->GetCellData( _T("MARC"), nGridIdx , strStreamMarc );

	ids = m_pInfo->pMarcMgr->Decoding ( strStreamMarc , pMarc );
	if ( ids < 0 ) return FALSE;

	m_pRefDM->FreeData();

	//0번째 DM MARC에서 빼와서 참조표목 입력
	CString strItemData;
	if ( m_strSearchGroup == _T("서명") )
	{
		m_pInfo->pMarcMgr->GetItem( pMarc , _T("430$a"), strItemData, &arrItem );
	}
	else if ( m_strSearchGroup == _T("저자") )
	{
		m_pInfo->pMarcMgr->GetItem( pMarc , _T("400$a"), strItemData , &arrItem );
		if ( strItemData.IsEmpty() )
			m_pInfo->pMarcMgr->GetItem( pMarc , _T("410$a"), strItemData , &arrItem );
		if ( strItemData.IsEmpty() )
			m_pInfo->pMarcMgr->GetItem( pMarc , _T("411$a"), strItemData , &arrItem );
	}

	for ( INT idx = 0; idx < arrItem.GetSize(); idx++ )
	{
		strItemData = arrItem.GetAt( idx );
		m_pRefDM->InsertRow(-1);
		m_pRefDM->SetCellData ( _T("참조표목"), strItemData , idx );
	}

	return TRUE;
}

VOID CAC_SEARCH_DLG::AutoSearchProc()
{
	//그리드에 선정되었을 경우 자동으로 검색

}

VOID CAC_SEARCH_DLG::OnbOK() 
{
	m_strSearchText = _T("");
	CESL_Grid *pGrid;
	pGrid = (CESL_Grid*)m_pCM->FindControl(_T("GRID2"));

	//선정된 자료가 있는지 확인
	if ( !isCheckSelCnt( pGrid ) ) return ;

	//선정된 자료로 검색 Key 생성
	MakeSearchKeyword( pGrid );

	CDialog::OnOK();

}

BOOL CAC_SEARCH_DLG::isCheckSelCnt( CESL_Grid *pGrid  )
{
	if ( pGrid == NULL ) return FALSE;

	if ( pGrid->SelectGetCount() < 0 ) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return FALSE;
	}

	return TRUE;

}

VOID CAC_SEARCH_DLG::MakeSearchKeyword( CESL_Grid *pGrid )
{
	if ( pGrid == NULL ) return;
	pGrid->SelectMakeList();

	CString strGrid2Item;
	INT idx = pGrid->SelectGetHeadPosition();
	m_strSearchText = m_strSearchItem + _T(" ");
	while ( idx > -1 )
	{
		strGrid2Item = pGrid->GetTextMatrix(idx+1, 2);

		m_strSearchText += strGrid2Item + _T(" ");

		idx = pGrid->SelectGetNext();
	}

	m_strSearchText.TrimRight();
}

INT CAC_SEARCH_DLG::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_GRID1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )		return 0;
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		pGrid->SetText( _T("") );
	}
	for( i = 1; i < pGrid->GetRows(); i++ )
	{
		if( row == i )
		{
			pGrid->SetRow( i );
			pGrid->SetText( _T("V") );
			break;
		}	
	}

	return row;
}

HBRUSH CAC_SEARCH_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
