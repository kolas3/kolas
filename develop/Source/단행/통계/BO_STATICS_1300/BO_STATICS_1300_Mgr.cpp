// BO_STATICS_1300_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATICS_1300_Mgr.h"
#include "..\..\..\관리\오류수정\ERROR_ADJUST_MGR\ErrorAdjustLauncher.h"

// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
#include "BO_STATICS_CLICK_LIST_1300.h"
// KOL.RED.2018.211 ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1300_Mgr dialog


CBO_STATICS_1300_Mgr::CBO_STATICS_1300_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CStatistics(CBO_STATICS_1300_Mgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_1300_Mgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_STATICS_1300_Mgr::~CBO_STATICS_1300_Mgr()
{
	
}

VOID CBO_STATICS_1300_Mgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_1300_Mgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_STATICS_1300_Mgr, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_1300_Mgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(CHANGE_CLASS_NO_TYPE, OnChangeClassNoType)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1300_Mgr message handlers

BOOL CBO_STATICS_1300_Mgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strIs90 = _T(""), strSM;

	
	strSM = _T("K2UPGRADE_자산통계");		
	if( InitESL_Mgr(strSM) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	


	SetCM(FindCM(_T("CM_자산통계")));
	SetDM(FindDM(_T("DM_자산통계자료")));

	
	SetGrid((CESL_Grid*)GetCM()->FindControl(_T("MainGrid")));
	GetGrid()->SetSort(FALSE);
	
	/*
	GetDlgItem(IDC_stcPURCHASE_STATISTICS_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_STATISTICS_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_STATISTICS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_STATISTICS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_STATISTICS_CONDITION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_STATISTICS_CONDITION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_STATISTICS_UNIT)->ShowWindow(SW_HIDE);
	*/

	CreateConditionDlg();
	InitPopupMenuItem();

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_STATICS_1300_Mgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

VOID CBO_STATICS_1300_Mgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
	// TODO: Add your message handler code here
	//=====================================================
	//2008.12.15 REM BY PJW : K3에서 제외됨
// 	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_STATISTICS_DATA);
// 	if(pBtn->GetSafeHwnd() == NULL) return;
// 	pBtn->MoveWindow(5, 5, cx-10, 30);
	//=====================================================
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_STATISTICS_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);
	pGrid->MoveWindow(-2, 40, cx+2, cy-40);
	
}

UINT CBO_STATICS_1300_Mgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//AfxMessageBox(_T("test"));
	SetStatisticsInfo();
	DisplayGrid();
	return 0;
}

UINT CBO_STATICS_1300_Mgr::OnChangeClassNoType(LPARAM lParam, WPARAM wParam)
{
	ChangeColumnHeaderOfSubjectCode();
	return 0;
}

VOID CBO_STATICS_1300_Mgr::ShowTestFrameWnd()
{
	m_Launcher.LaunchErrorAdjustFrame(GetParentFrame(),this);
}


HBRUSH CBO_STATICS_1300_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
int CBO_STATICS_1300_Mgr::MakeQuery( int x , int y )
{
	if(	Check_ClickAble(x,y) == false ) return -1;

	CString		Desc_x , Desc_y;
	CESL_Grid*	pGrid = GetGrid();

	Desc_x = pGrid->GetTextMatrix(0, x );
	Desc_y = pGrid->GetAt( y - 1 , 0 );

	CString		str_select , str_from , str_where;

	// 
	if( Check_IndexOrBook( Desc_x ) == 1 )
	{
		// (책)
		str_select	= _T("b.REG_NO , i.title_info");		
	}else{
		// (종)
		str_select	= _T("DISTINCT(i.CONTROL_NO) , i.title_info");
	}

	str_from	= _T("idx_bo_tbl i, bo_book_tbl b");	// 

	// 기본 Where 
	// 현재 풀 SQL 이 들어있다.
	str_where = m_pCondition->GetWhereStmt();

	// WHERE  절 까지 삭제
	int find = str_where.Find(_T("WHERE"));
	if( find >= 0 )
	{
		str_where = str_where.Mid( find + 5 );
	}
	// 뒤에 버티컬 필드 삭제
	m_cGridQuery.Remove_AND_NearLastWord( str_where , m_pCondition->GetVerticalField() );

	// 각 축별 Where
	CString		condi_x = Condition_X( Desc_x );
	CString		condi_y = Condition_Y( Desc_y );
	str_where += condi_x;
	str_where += condi_y;

	str_select	= _T("SELECT ") + str_select;
	str_from	= _T(" FROM ") + str_from;
	str_where	= _T(" WHERE ") + str_where;

	m_strSql = str_select + str_from + str_where;

	//OutputDebugString( m_strSql + _T("\n") );

	return 1;
}

bool CBO_STATICS_1300_Mgr::Check_ClickAble( int x , int y )
{
	return true;
}

int CBO_STATICS_1300_Mgr::Check_IndexOrBook( CString cstr_column )
{
	// (책) = 1 , (종) = 0 구분
	if( cstr_column.Find(_T("(책)")) >= 0 )
	{
		return 1;
	}
	return 0;
}

CString	CBO_STATICS_1300_Mgr::Condition_X( CString cstr_column )
{
	// BO_BOOK_TBL  ACQ_CODE , 수입구분 
	int code = -1;
	if( cstr_column.Find(_T("적용안함")) >= 0 )  code = 0;
	if( cstr_column.Find(_T("구입")) >= 0 )		code = 1;
	if( cstr_column.Find(_T("기증")) >= 0 )		code = 2;
	if( cstr_column.Find(_T("기타")) >= 0 )		code = 3;

	CString cstr;
	if( code < 0 ) return cstr;
	if( code > 0 )
	{
		cstr.Format( _T(" AND B.ACQ_CODE = '%d' ") , code );
	}
	else if( code == 0 )
	{
		cstr.Format( _T(" AND B.ACQ_CODE is null ") );
	}
	return cstr;
}

CString	CBO_STATICS_1300_Mgr::Condition_Y( CString cstr_column )
{
	CString cstr;
	if( cstr_column.Find(_T("적용안함")) >= 0 )
	{
		cstr.Format(_T(" AND (%s = '' OR %s IS NULL)"), m_pCondition->GetVerticalField(), m_pCondition->GetVerticalField());
		return cstr;
	}

	// 코드매니저
	int find = cstr_column.ReverseFind( _T('(') );
	if( find >= 0 )
	{
		cstr_column = cstr_column.Left( find );
	}
	CString cstr_code;
	m_pInfo->pCodeMgr->ConvertDescToCode( m_pCondition->GetVerticalDesc() , cstr_column , cstr_code );
	cstr.Format( _T(" AND %s = '%s' ") , m_pCondition->GetVerticalField() , cstr_code );
	return cstr;
}

BEGIN_EVENTSINK_MAP(CBO_STATICS_1300_Mgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_STATICS_1300_Mgr)
	ON_EVENT(CBO_STATICS_1300_Mgr, IDC_gridPURCHASE_STATISTICS_DATA, -600 /* Click */, OnClickgridPURCHASESTATISTICSDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBO_STATICS_1300_Mgr::OnClickgridPURCHASESTATISTICSDATA() 
{
	// TODO: Add your control notification handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_STATISTICS_DATA);
	ASSERT(pGrid);
	ASSERT(GetDM());

	if (GetDM()->GetRowCount() < 1)
	{
		// 통계 결과가 없다면 표시할 것이 없다
		return;
	}
	

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	int ids = 0;

	m_cGridQuery.SetCONNECTION_INFO( GetDM()->CONNECTION_INFO );
	m_cGridQuery.SetManageCode( GetDM()->MANAGE_CODE );

	ids = MakeQuery( nCol , nRow );
	if( ids < 0 )
		return;

	m_cGridQuery.RunQuery( m_strSql );
	if (m_cGridQuery.m_cDM.GetRowCount() > 0)
	{
		BO_STATICS_CLICK_LIST_1300	dlg;
		dlg.m_pArrQuery = &m_cGridQuery;
		dlg.DoModal();
	}
}
// KOL.RED.2018.211 ---------------------------------------------------------------------------
