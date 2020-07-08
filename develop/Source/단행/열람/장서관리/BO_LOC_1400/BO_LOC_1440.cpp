// BO_LOC_1440.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1440.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1440 dialog


CBO_LOC_1440::CBO_LOC_1440(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1440)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strMenuName = _T("단행_수리제본_통계");	
}

CBO_LOC_1440::~CBO_LOC_1440()
{
	if( m_pSInfoMgr != NULL )
		delete m_pSInfoMgr;
	m_pSInfoMgr = NULL;
}

VOID CBO_LOC_1440::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1440)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1440, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1440)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnSTATISTICS2, OnbtnSTATISTICS2)
	ON_BN_CLICKED(IDC_btnSTATISTICS, OnbtnSTATISTICS)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_chkSUMZERODEL, OnchkSUMZERODEL)
	ON_BN_CLICKED(IDC_btnSPRINT, OnbtnSPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1440 message handlers

BOOL CBO_LOC_1440::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_1440")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	MakeDMInfo();
	MakeStaticsClass();

	CRect WndPos;
	GetWindowRect( WndPos );
	
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , WndPos.Height() );

	FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if( fd != NULL )
	{
		fclose(fd);
	}
	else
	{
		GetDlgItem(IDC_btnSTATISTICS2)->ShowWindow(SW_HIDE);
	}
	
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END

	return FALSE;
}

VOID CBO_LOC_1440::OnSize(UINT nType, INT cx, INT cy) 
{

	CDialog::OnSize(nType, cx, cy);	

	EFS_BEGIN

	if( GetSafeHwnd() == NULL ) return;

	const INT nCnt = 4;
	CRect WndPos[nCnt];
	CWnd* Wnd[nCnt];

	INT nIDC[4] = 
	{
		IDC_staTITLE , IDC_staTITLE2 , IDC_gridSTATISTICS , IDC_prgSTATICS2
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		Wnd[i] = GetDlgItem(nIDC[i]);
		if( Wnd[i]->GetSafeHwnd() == NULL ) return;
		Wnd[i]->GetWindowRect(WndPos[i]);
		ScreenToClient(WndPos[i]);
	}

	Wnd[0]->MoveWindow( 0 , WndPos[0].top , cx , WndPos[0].Height() );
	Wnd[1]->MoveWindow( 0 , WndPos[1].top , cx , WndPos[1].Height() );
		
	Wnd[2]->MoveWindow( 0 , WndPos[2].top , cx , cy - WndPos[2].top - WndPos[3].Height() - 10  );

	Wnd[3]->MoveWindow( WndPos[3].left , cy - WndPos[3].Height() - 5 , WndPos[3].Width() , WndPos[3].Height() );
	

	EFS_END
}


INT CBO_LOC_1440::MakeDMInfo()
{
	EFS_BEGIN
		
	m_pSInfoMgr = new CStaticsInfoMgr(this,m_strMenuName);
	m_pSInfoMgr->MakeDMInfo();

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_1440"));
	m_pSInfoMgr->SetSearchCM(pCM);
	
	m_pStaticsMainInfoDM = m_pSInfoMgr->m_pStaticsMainInfoDM;
	
	return 0;
	
	EFS_END
	return -1;
	
}



INT CBO_LOC_1440::MakeStaticsClass()
{
	EFS_BEGIN
		
		INT ids;
	
	CComboBox *pCombo = ((CComboBox*)GetDlgItem(IDC_cmbOPTIOIN));
	
	CString sJobClass , sStaticsClass;
	
	for( INT i = 0 ; i < m_pStaticsMainInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pStaticsMainInfoDM->GetCellData( _T("업무구분") , i , sJobClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
		
		if( m_strMenuName.Compare( sJobClass ) != 0 ) continue;
		
		ids = m_pStaticsMainInfoDM->GetCellData( _T("통계종류") , i , sStaticsClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );
		
		pCombo->AddString( sStaticsClass );
	}
	
	pCombo->SetCurSel(0);
	
	return 0;
	
	EFS_END
	return -1;
	
}

VOID CBO_LOC_1440::OnbtnSTATISTICS2() 
{
	EFS_BEGIN

	m_pSInfoMgr->DeBugDM();	

	EFS_END
}

VOID CBO_LOC_1440::OnbtnSTATISTICS() 
{	
	CString strValue;
	( (CEdit*) GetDlgItem (IDC_edtREQUEST_YEAR) )->GetWindowText(strValue);
	if ( strValue.IsEmpty() ) 
	{
		AfxMessageBox(_T("연도를 입력하세요."));
		return;
	}
	AfxGetApp()->DoWaitCursor( 1 );
	RunStatics();
	AfxGetApp()->DoWaitCursor( -1 );
}

VOID CBO_LOC_1440::OnbtnCLOSE() 
{
	OnCancel();	
}

INT CBO_LOC_1440::RunStatics()
{
	EFS_BEGIN

	INT ids;

	ids = SaveCondition();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
	if( ids > 0 ) return ids;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_1440") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("OnInitDialog") );

	CString sStaticsClass;
	ids = pCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitScreen") );

	CProgressCtrl *pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS);
	CProgressCtrl *pTotalProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS2);

	ids = m_pSInfoMgr->MakeStaticsCondition(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	if( ids > 0 ) return ids;
	
	ids = m_pSInfoMgr->RunStatics(sStaticsClass,pProgressCtrl,pTotalProgressCtrl);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
	
	ShowGrid();

	return 0;

	EFS_END

	return -1;

	
}

VOID CBO_LOC_1440::ShowGrid()
{
	EFS_BEGIN

	INT ids;
	
	CString sStaticsClass;
	CString sMenuName = m_strMenuName;
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTATISTICS);
	
	ids = GetControlData( _T("CM_BO_LOC_1440") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnSelchangetabMAINOPTION") );
	
	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;
	
	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 )
		bDupDel = FALSE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;
	
	
	ids = m_pSInfoMgr->StaticsResultViewGrid(sStaticsClass,pGrid,bDupDel , bZeroShow , sMenuName);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("RunStatics") );	
	


	EFS_END
}

VOID CBO_LOC_1440::OnchkSUMZERODEL() 
{
	ShowGrid();	
}

INT CBO_LOC_1440::SaveCondition()
{
	EFS_BEGIN

	INT ids;

	CString sYear;
	CString sRequestDate;
	ids = GetControlData( _T("CM_BO_LOC_1440") , _T("차수년도") , sYear );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveCondition") );

	if( !sYear.IsEmpty() )
	{
		sRequestDate.Format( _T("%s/01/01") , sYear );
		ids = SetControlData( _T("CM_BO_LOC_1440") , _T("의뢰일1") , sRequestDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveCondition") );
		sRequestDate.Format( _T("%s/12/31") , sYear );
		ids = SetControlData( _T("CM_BO_LOC_1440") , _T("의뢰일2") , sRequestDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveCondition") );
	}
	
	CString sSearchCond;
	ids = CMToSql( sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SaveCondition") );

	if( sSearchCond.IsEmpty() )
	{
		AfxMessageBox( _T("조건을 입력해주십시요") );
		return 1;
	}

	CString sStaticsClass;
	ids = GetControlData( _T("CM_BO_LOC_1440") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SaveCondition") );

	ids = m_pSInfoMgr->SetUserCondition( sStaticsClass , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SaveCondition") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1440::CMToSql( CString &sSearchCond )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_1440");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
	
	INT nControlCnt = pCM->Control_List.GetCount();
	CString sData;

	CESL_Control_Element *pCE;
	POSITION pos;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		pos = pCM->Control_List.FindIndex(i);
		pCE = (CESL_Control_Element*)pCM->Control_List.GetAt(pos);


		ids = pCM->GetControlMgrData( pCE->CTRL_ALIAS , sData , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
		sData.TrimLeft(); sData.TrimRight();

		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");

		if( sData.IsEmpty() ) continue;

		if( pCE->CTRL_ALIAS.Compare(_T("통계종류")) == 0 )
			continue;			

		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;

		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}


	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_1440::OnbtnSPRINT() 
{
	CString sStaticsClass;
	INT ids = GetControlData( _T("CM_BO_LOC_1440") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnSPRINT") );

	bool bZeroShow = TRUE;
		
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;

	m_pSInfoMgr->PrintResult(sStaticsClass,TRUE,bZeroShow ,NULL);
	
}

HBRUSH CBO_LOC_1440::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
