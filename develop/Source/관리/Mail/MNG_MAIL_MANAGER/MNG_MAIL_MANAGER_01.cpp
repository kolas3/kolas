// MNG_MAIL_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MAIL_MANAGER_01.h"
#include "MNG_MAIL_MANAGER_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAIL_MANAGER_01 dialog


CMNG_MAIL_MANAGER_01::CMNG_MAIL_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MAIL_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_MAIL_MANAGER_01::~CMNG_MAIL_MANAGER_01()
{
	
}
BOOL CMNG_MAIL_MANAGER_01::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	
	
VOID CMNG_MAIL_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MAIL_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_MAIL_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_MAIL_MANAGER_01)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAIL_MANAGER_01 message handlers

BOOL CMNG_MAIL_MANAGER_01::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MAIL_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MAIL_MANAGER_01") );
		return false;
	}

	/// 메일링 시스템을 사용하지 않는 도서관이라면 입력과 삭제 기능은 없어야 한다.
	CString sValue;
	INT ids = GetManageValue( _T("기타") , _T("공통") , _T("메일링시스템") , _T("사용여부") , sValue );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("InitESL_Mgr Error : GetmanageValue 에러") );
		return false;	
	}

	if( sValue.Compare(_T("Y")) != 0 )
	{	
		CString alias = _T("입력");
		pMain->SendMessage( WM_USER + 2000, ( WPARAM )&alias, TRUE );
		alias = _T("삭제");
		pMain->SendMessage( WM_USER + 2000, ( WPARAM )&alias, TRUE );
	}

	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MAIL_MANAGER_01"), _T("그리드") );
	if( pGrid )
	{
		//RemoveListArray.Add( _T("") );

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
	}

	SearchUserID();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMNG_MAIL_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	//pGrid->MoveWindow( -2, -2 , cx + 4 , cy + 4 );
	pGrid->MoveWindow(0, 0, cx, cy);

	EFS_END
}


// CM_MNG_MAIL_MANAGER_01
// DM_MNG_MAIL_MANAGER_01
// DM_MNG_MAIL_MANAGER_USER

VOID CMNG_MAIL_MANAGER_01::SearchUserID()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_USER") );
	pDM->RefreshDataManager( _T("") );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_01::ShowGrid(INT nSetReverseIndex)
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MAIL_MANAGER_01"), _T("그리드") );

	CString sCheck;

	if( nSetReverseIndex != -2 )
	{
		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			pGrid->GetAt(i,0,sCheck);

			pDM->SetCellData( _T("선정") , sCheck , i );
		}
	}


	ControlDisplay( _T("CM_MNG_MAIL_MANAGER_01"), _T("그리드") );

	if( nSetReverseIndex != -1 )
		pGrid->SetReverse(nSetReverseIndex);


	EFS_END
}

CString * CMNG_MAIL_MANAGER_01::MakeMngGroup( CString type, INT &cnt )
{
	EFS_BEGIN

	CString where;
	where.Format( _T(" M_TYPE = '%s' "), type );
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01_group") );
	pDM->RefreshDataManager( where );

	cnt = pDM->GetRowCount();
	CString *group = new CString[ cnt ];

	for( INT i = 0; i < cnt; i++ )
		pDM->GetCellData( _T("MNG_GROUP"), i, group[ i ] );

	return group;

	EFS_END
	return NULL;

}

INT CMNG_MAIL_MANAGER_01::SearchData( CString where )
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
	pDM->RefreshDataManager( where );
	
	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("검색 결과가 없습니다.") );

	CString sCond[2] = 
	{
		_T("G_M_TYPE"),_T("M_MAIL_CLASS")
	};

	INT nCond[2] = 
	{
		ESL_DATAMGR_SORT_DESC , ESL_DATAMGR_SORT_DESC
	};

	pDM->SORT( 2 , sCond , nCond , ESL_DATAMGR_SORT_DESC);

	ShowGrid();

	return 0;

	EFS_END
	return -1;

}

INT *CMNG_MAIL_MANAGER_01::GetCheckedDMRow( INT &cnt )
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
	INT rowCnt = pDM->GetRowCount();
	if( rowCnt < 1 )
	{
		cnt = pDM->GetRowCount();
		return NULL;
	}
		
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MAIL_MANAGER_01"), _T("그리드") );
	pGrid->SelectMakeList();

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	CArray< INT, INT > checkedRow;	
	pGrid->SetCol( 1 );
	for( INT i = 0; i < rowCnt; i++ )
	{
		pGrid->SetRow( i + 1 );

		if( pGrid->SelectGetCount() > 0 )
		{
			if( _T("V") == pGrid->GetText() )
				checkedRow.Add( i );
		}
		else
			checkedRow.Add( i );


	}
			
	cnt = checkedRow.GetSize();
	INT *checkedDMRow = ( INT * )calloc( cnt, sizeof( INT ) );
	for( i = 0; i < cnt; i++ )
		checkedDMRow[ i ] =checkedRow.GetAt( i );	

	return checkedDMRow;

	EFS_END
	return NULL;

}

VOID CMNG_MAIL_MANAGER_01::DeleteData()
{
	EFS_BEGIN

	INT cnt = 0;
	INT *checkedDMRow = GetCheckedDMRow( cnt );

	if( !cnt )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		return;
	}
	
	CString notice;
	notice.Format( _T("선택된 %d 개 목록을 삭제하시겠습니까?"), cnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
	{
		free( checkedDMRow );
		return;
	}

	INT ids;
	INT nSuccess=0;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		ids = SendDeleteQuery( checkedDMRow[ i ] );
		if( ids == 0 ) nSuccess++;
	}

	CString sMsg;
	if( nSuccess > 0 ) 
		sMsg.Format( _T("%d건이 삭제되었습니다.") , nSuccess );
	else
		sMsg.Format( _T("삭제할수 있는 메일 형식이 존재하지 않습니다.") , nSuccess );
	AfxMessageBox( sMsg );
	ShowGrid(-2);

	EFS_END
}

INT CMNG_MAIL_MANAGER_01::SendDeleteQuery( INT idx )
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );

	CString sMailClass;
	pDM->GetCellData( _T("M_MAIL_CLASS") , idx , sMailClass );

	if( _ttoi(sMailClass) >= 30 && _ttoi(sMailClass) <= 60 )
	{
		CString query, pk;

		pDM->GetCellData( _T("G_REC_KEY"), idx, pk );
		query.Format( _T("DELETE FROM MN_MAIL_GROUP_TBL WHERE REC_KEY = %s;"), pk );
		pDM->ExecuteQuery( query, 1 );

		pDM->GetCellData( _T("M_REC_KEY"), idx, pk );
		query.Format( _T("DELETE FROM MN_MAIL_TBL WHERE REC_KEY = %s;"), pk );
		pDM->ExecuteQuery( query, 1 );

		pDM->DeleteRow( idx );
	}
	else 
		return 1;
	
	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CMNG_MAIL_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_MAIL_MANAGER_01)
	ON_EVENT(CMNG_MAIL_MANAGER_01, IDC_mngGRID_1, -601 /* DblClick */, OnDblClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_MAIL_MANAGER_01::OnDblClickmngGRID1() 
{
EFS_BEGIN

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MAIL_MANAGER_01"), _T("그리드") );

	if( pGrid->GetMouseRow() < 1 ) return;

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() > 0 )
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	CMNG_MAIL_MANAGER_02 Dlg(this);
	Dlg.m_mode = 2;
	
	Dlg.DoModal();
	

EFS_END	
}

HBRUSH CMNG_MAIL_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
