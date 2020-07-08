#include "stdafx.h"
#include "FAMILY_LOAN_HIS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFAMILY_LOAN_HIS dialog

CFAMILY_LOAN_HIS::CFAMILY_LOAN_HIS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CFAMILY_LOAN_HIS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

CFAMILY_LOAN_HIS::~CFAMILY_LOAN_HIS()
{
}

VOID CFAMILY_LOAN_HIS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFAMILY_LOAN_HIS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFAMILY_LOAN_HIS, CDialog)
	//{{AFX_MSG_MAP(CFAMILY_LOAN_HIS)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFAMILY_LOAN_HIS message handlers

BOOL CFAMILY_LOAN_HIS::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_FAMILY_LOAN_HOSTORY")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CESL_DataMgr* pDM = FindDM( _T("DM_FAMILY_LOAN_HISTORY") );
	if( !pDM ) return FALSE;

	CString strWhere;
	strWhere.Format( _T(" CLU.FID_KEY=%s "), m_strFamilyID );
	pDM->RefreshDataManager( strWhere );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_FAMILY_LOAN_HISTORY"), _T("MainGrid"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("가족 대출 현황");
	}

	INT ids;
	INT nCnt = pDM->GetRowCount();
	CString sData, sDesc;
	for(INT i = 0; i < nCnt; i++)
	{
		ids = pDM->GetCellData("관리구분", i, sData);
		if( ids < 0 ) return FALSE;
		ids = m_pParent->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );
		if( ids < 0 ) return FALSE;
		ids = pDM->SetCellData( _T("관리구분설명"), sDesc, i );
		if( ids < 0 ) return FALSE;		

		ids = pDM->GetCellData("대출상태", i, sData);
		if( ids < 0 ) return FALSE;
		if( sData == _T("0") ) sDesc = _T("대출");
		else if( sData == _T("2") ) sDesc = _T("반납연기");
		else if( sData == _T("L") ) sDesc = _T("책두레대출");
		else if( sData == _T("D") ) sDesc = _T("책두레반납연기");
		else sDesc = _T("대출");
		ids = pDM->SetCellData( _T("대출상태"), sDesc, i );
		if( ids < 0 ) return FALSE;
	}
	//--2007.12.17 ADD BY PWR --}}

	AllControlDisplay( _T("CM_FAMILY_LOAN_HISTORY") );

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CFAMILY_LOAN_HIS::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd* pWnd = GetDlgItem(IDC_gridFAMLIY_LOAN_HIS);

	if( pWnd == NULL ) return;
	
	pWnd->MoveWindow( 0 , 0 , cx , cy );	
}

HBRUSH CFAMILY_LOAN_HIS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
