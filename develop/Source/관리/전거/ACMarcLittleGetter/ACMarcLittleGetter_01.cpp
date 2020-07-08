// ACMarcLittleGetter_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "ACMarcLittleGetter_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACMarcLittleGetter_01 dialog


CACMarcLittleGetter_01::CACMarcLittleGetter_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CACMarcLittleGetter_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CACMarcLittleGetter_01::~CACMarcLittleGetter_01()
{
}

BOOL CACMarcLittleGetter_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CACMarcLittleGetter_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACMarcLittleGetter_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACMarcLittleGetter_01, CDialog)
	//{{AFX_MSG_MAP(CACMarcLittleGetter_01)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACMarcLittleGetter_01 message handlers

BOOL CACMarcLittleGetter_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("ACMarcLittleGetter_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : ACMarcLittleGetter_01") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CACMarcLittleGetter_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	m_checkedRow = -1;
	SearchData();
}

BEGIN_EVENTSINK_MAP(CACMarcLittleGetter_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CACMarcLittleGetter_01)
	ON_EVENT(CACMarcLittleGetter_01, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	ON_EVENT(CACMarcLittleGetter_01, IDC_mngGRID_3, -600 /* Click */, OnClickmngGRID3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CACMarcLittleGetter_01::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	SetGridCheck();
	if( -1 != m_checkedRow )
		ViewBookKeywordList();
}

VOID CACMarcLittleGetter_01::OnClickmngGRID3() 
{
	// TODO: Add your control notification handler code here
	
	SetGridCheck();
}

VOID CACMarcLittleGetter_01::SetGridCheck()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return;

	CESL_DataMgr *pDM = FindDM( _T("DM_ACMarcLittleGetter_01_IDX") );
	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetText( _T("") );
		m_checkedRow = -1;
		pDM->FreeData();
		ControlDisplay( _T("CM_ACMarcLittleGetter_01"), _T("그리드_2") );
		return;
	}

	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( row == i )
		{
			pGrid->SetText( _T("V") );
			m_checkedRow = row;
		}
		else
			pGrid->SetText( _T("") );
	}

	pGrid->SetRow( row );
}

VOID CACMarcLittleGetter_01::SearchData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_ACMarcLittleGetter_01") );
	pDM->RefreshDataManager( MakeSelectQuery() );
	if( !pDM->GetRowCount() )
	{
		AfxMessageBox( _T("검색결과가 없습니다.") );
		PostMessage( WM_CLOSE );
	}
	else
		ShowGrid();
}

CString CACMarcLittleGetter_01::MakeSelectQuery()
{
	CString where;

	switch( m_ACType )
	{
		case 1 :
			where.Format( _T("REC_KEY IN ( SELECT DISTINCT AC_MASTER_MAT_KEY FROM AC_INDEX_AUTHOR_TBL WHERE KEYWORD LIKE '%%%s%%' )"), m_keyWord );
			break;
		case 2 :
			where.Format( _T("REC_KEY IN ( SELECT DISTINCT AC_MASTER_MAT_KEY FROM AC_INDEX_TITLE_TBL WHERE KEYWORD LIKE '%%%s%%' )"), m_keyWord );
			break;
	}

	return where;
}

VOID CACMarcLittleGetter_01::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	OnCancel();
	//CDialog::OnClose();
}

VOID CACMarcLittleGetter_01::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CACMarcLittleGetter_01::ShowGrid()
{
	ControlDisplay( _T("CM_ACMarcLittleGetter_01"), _T("그리드_1") );

	CESL_DataMgr *pDM = FindDM( _T("DM_ACMarcLittleGetter_01") );
	
	CString ACType, marc;
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("AC_TYPE"), i, ACType );
		marc = GetGridText( i + 1, 2, 1 );
		SetGridText( i + 1, 2, GetACKeyword( marc, _ttoi( ACType ) ), 1 );
	}
}

INT CACMarcLittleGetter_01::SetGridText( INT row, INT col, CString data, INT option )
{
	CMSHFlexGrid *pGrid;
	switch( option )
	{
	case 1 :
		pGrid =  ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
		break;
	case 2 :
		pGrid =  ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
		break;
	}
	
	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();

	pGrid->SetRow( row );
	pGrid->SetCol( col );
	pGrid->SetText( data );

	pGrid->SetRow( curRow );
	pGrid->SetCol( curCol );
	
	return 0;
}

CString CACMarcLittleGetter_01::GetGridText( INT row, INT col, INT option )
{
	CMSHFlexGrid *pGrid;
	switch( option )
	{
	case 1 :
		pGrid =  ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
		break;
	case 2 :
		pGrid =  ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
		break;
	}
	
	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();

	CString data;
	pGrid->SetRow( row );
	pGrid->SetCol( col );
	data = pGrid->GetText();

	pGrid->SetRow( curRow );
	pGrid->SetCol( curCol );
		
	return data;
}

CString CACMarcLittleGetter_01::GetACKeyword( CString marc, INT ACType )
{
	CMarc marcS;
	if( m_pInfo->pMarcMgr->Decoding( marc, &marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return -1;
	}

	CString idx;
	switch( ACType )
	{
		case 0 :
			m_pInfo->pMarcMgr->GetItem( &marcS, _T("100$a"), idx );
			break;
		case 1 :
			m_pInfo->pMarcMgr->GetItem( &marcS, _T("110$a"), idx );
			break;
		case 2 :
			m_pInfo->pMarcMgr->GetItem( &marcS, _T("111$a"), idx );
			break;
		case 3 :
			m_pInfo->pMarcMgr->GetItem( &marcS, _T("130$a"), idx );
			break;
		case 4 :
			m_pInfo->pMarcMgr->GetItem( &marcS, _T("150$a"), idx );
			break;
	}

	return idx;
}

VOID CACMarcLittleGetter_01::ViewBookKeywordList()
{
	CString marc, idx, ACType;
	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_ACMarcLittleGetter_01") );
	pDM->GetCellData( _T("MARC"), m_checkedRow - 1, marc );
	pDM->GetCellData( _T("AC_TYPE"), m_checkedRow - 1, ACType );
	
	CMarc marcS;
	if( m_pInfo->pMarcMgr->Decoding( marc, &marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return ;
	}
	
	pDM = FindDM( _T("DM_ACMarcLittleGetter_01_IDX") );
	pDM->FreeData();
	
	CArray < CString, CString& > idxArray;
	idxArray.RemoveAll();
	INT idxCnt = 0;
	INT i;

	switch( _ttoi( ACType ) )
	{
		case 0 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("100$a"), idx );
			if( idxCnt > 0 )
				pDM->SetCellData( _T("IDX"), idx, -1 );
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("400$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("500$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			break;

		case 1 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("110$a"), idx );
			if( idxCnt > 0 )
				pDM->SetCellData( _T("IDX"), idx, -1 );

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("410$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("510$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			break;
			
		case 2 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("111$a"), idx );
			if( idxCnt > 0 )
				pDM->SetCellData( _T("IDX"), idx, -1 );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("411$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("511$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			break;

		case 3 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("130$a"), idx );
			if( idxCnt > 0 )
				pDM->SetCellData( _T("IDX"), idx, -1 );

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("430$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("530$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			break;

		case 4 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("150$a"), idx );
			if( idxCnt > 0 )
				pDM->SetCellData( _T("IDX"), idx, -1 );

			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("530$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				pDM->SetCellData( _T("IDX"), idxArray.GetAt( i ), -1 );
			idxArray.RemoveAll();

			break;
	}

	ControlDisplay( _T("CM_ACMarcLittleGetter_01"), _T("그리드_2") );
}

VOID CACMarcLittleGetter_01::OnOK() 
{
	// TODO: Add extra validation here
		
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		pGrid->SetCol( 1 );
		if( _T("V") == pGrid->GetText() )
		{
			pGrid->SetCol( 2 );
			m_returnValue = m_returnValue + _T(" ") + pGrid->GetText();
		}
	}

	CDialog::OnOK();
}

HBRUSH CACMarcLittleGetter_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
