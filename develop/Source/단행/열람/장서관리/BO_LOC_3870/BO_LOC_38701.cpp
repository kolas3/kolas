// BO_LOC_38701.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38701.h"
#include "BO_LOC_3873.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3870 dialog


CBO_LOC_3870::CBO_LOC_3870(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3870)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3870::~CBO_LOC_3870()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3870::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3870)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3870, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3870)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3870 message handlers

BOOL CBO_LOC_3870::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3870::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("SM_BO_LOC_3870") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

VOID CBO_LOC_3870::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CBO_LOC_3870::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN


	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd* pWnd = GetDlgItem(IDC_grid3870_Main);

	if( pWnd == NULL ) return ;

	if( pWnd->GetSafeHwnd() == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );
	


EFS_END	
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3870, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3870)
	ON_EVENT(CBO_LOC_3870, IDC_grid3870_Main, -601 /* DblClick */, OnDblClickgrid3870Main, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3870::OnDblClickgrid3870Main() 
{
EFS_BEGIN
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3870") , _T("비회원메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickgrid3870Main") );

	if( pGrid->GetMouseRow() < 1 ) return;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3870") );
	if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickgrid3870Main") );

	if( pDM->GetRowCount() < 1 ) return ;

	pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	CBO_LOC_3873 Dlg(this);
	Dlg.SetDlgType( __ECO_MODIFY_DBCLICK_MODE , _T("CM_BO_LOC_3870") , _T("비회원메인그리드") , _T("DM_BO_LOC_3870") );
	Dlg.DoModal();

EFS_END	
}

INT CBO_LOC_3870::DeleteProc()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3870") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteProc") );

	if( pDM->GetRowCount() < 1 ) return 1;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3870") , _T("비회원메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteProc") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("삭제할 목록을 선정해주십시오.") );
		return 2;
	}


	// 선정된걸 삭제하기
	CArray <INT,INT> aDIndexs;
	CString sCode;
	CString sRecKey;

	pGrid->SelectGetTailPosition();

	pDM->StartFrame();

	INT nIndex;
	INT nDCnt=0;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = pDM->GetCellData( _T("비회원키") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DeleteProc") );

		ids = pDM->MakeDeleteFrame( _T("CO_NONMEMBER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DeleteProc") );
		nDCnt++;

		aDIndexs.Add(nIndex);

		pGrid->SelectGetPrev();

	}


	if( nDCnt > 0 )
	{
		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("DeleteProc") );
	}

	pDM->EndFrame();

	if( nDCnt == 0 )
	{
		AfxMessageBox( _T("삭제할 목록이 존재하지 않습니다. ") );
		return 1;
	}

	/// DM 삭제
	for( i = 0 ; i < aDIndexs.GetSize() ; i++ )
	{
		ids = pDM->DeleteRow( aDIndexs.GetAt(i) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("DeleteProc") );
	}

	/// 그리드 적용
	ids = AllControlDisplay( _T("CM_BO_LOC_3870") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("DeleteProc") ); 

	AfxMessageBox( _T("삭제되었습니다.") );

	return 0;

EFS_END
return -1;
}

HBRUSH CBO_LOC_3870::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
