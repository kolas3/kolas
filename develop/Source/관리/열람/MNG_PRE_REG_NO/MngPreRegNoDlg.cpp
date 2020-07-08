// MngPreRegNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngPreRegNoDlg.h"
#include "MngPreRegNoInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngPreRegNoDlg dialog


CMngPreRegNoDlg::CMngPreRegNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngPreRegNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMAlias = _T("SM_MNG_PRE_REG_NO");
	m_sDMAlias = _T("DM_MNG_PRE_REG_NO");
	m_sCMAlias = _T("CM_MNG_PRE_REG_NO");
	m_sGridAlias = _T("이전등록번호그리드");
}


VOID CMngPreRegNoDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngPreRegNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMngPreRegNoDlg, CDialog)
	//{{AFX_MSG_MAP(CMngPreRegNoDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngPreRegNoDlg message handlers

BOOL CMngPreRegNoDlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CMngPreRegNoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridREG_NO);
	if(pGrid) pGrid->MoveWindow(0, 0, cx, cy);
	//pGrid->MoveWindow(-2, -2, cx+4, cy+4);

	EFS_END
}

BOOL CMngPreRegNoDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// SM 적용
	INT ids;
	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	m_pDM = FindDM( m_sDMAlias );
	if( m_pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	
	m_pCM = FindCM( m_sCMAlias );
	if( m_pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	m_pGrid = (CESL_Grid*)(m_pCM->FindControl( m_sGridAlias ));
	if( m_pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("OnInitDialog") );

	ShowScreen();

	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("이전등록번호그리드") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_PRE_REG_NO"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}

	if(m_pInfo->MODE==10000)
	{
		CString strButtonAlias ;
		strButtonAlias = _T("수정");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("입력");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("삭제");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMngPreRegNoDlg::ShowScreen()
{
	EFS_BEGIN

	INT ids;

//	ids = m_pDM->RefreshDataManager(_T(""));
	ids = CLocCommonAPI::RefreshDataManager( this , m_pDM , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("ShowScreen") );

	ids = m_pCM->AllControlDisplay();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("ShowScreen") );

	return ;

	EFS_END
}

INT CMngPreRegNoDlg::DeletDB()
{
	EFS_BEGIN

	INT ids;

	ids = MessageBox( _T("정말로 삭제하시겠습니까?") , _T("삭제") , MB_YESNO | MB_ICONQUESTION );
	if( ids == IDNO )
		return 0;

	INT nIndex = m_pGrid->GetIdx();

	CString sRecKey;
	ids = m_pDM->GetCellData( _T("REC_KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeletDB") );

	// Mobile Api 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , m_pDM );


	m_pDM->StartFrame();
//	m_pDM->MakeDeleteFrame( _T("CO_PREV_REG_NO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	pMobileApi->MakeDeleteFrame( _T("CO_PREV_REG_NO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
//	ids = m_pDM->SendFrame();
	pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeletDB") );
	m_pDM->EndFrame();

	// Mobile Api delete
	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	m_pDM->DeleteRow(nIndex);
	
	m_pCM->AllControlDisplay();

	AfxMessageBox(_T("삭제되었습니다"));

	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CMngPreRegNoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngPreRegNoDlg)
	ON_EVENT(CMngPreRegNoDlg, IDC_gridREG_NO, -600 /* Click */, OnClickgridREGNO, VTS_NONE)
	ON_EVENT(CMngPreRegNoDlg, IDC_gridREG_NO, -601 /* DblClick */, OnDblClickgridREGNO, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngPreRegNoDlg::OnClickgridREGNO() 
{
	EFS_BEGIN

	if( m_pDM->GetRowCount() < 1 ) return ;

	EFS_END
}

VOID CMngPreRegNoDlg::OnDblClickgridREGNO() 
{
	if( m_pDM->GetRowCount() < 1 ) return ;

	if( m_pGrid->GetMouseRow() < 1 ) return;

	CMngPreRegNoInputDlg Dlg(this,1);
	Dlg.DoModal();
	
}
