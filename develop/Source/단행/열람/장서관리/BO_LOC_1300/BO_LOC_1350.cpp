// BO_LOC_1350.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1350.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1350 dialog


CBO_LOC_1350::CBO_LOC_1350(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1350)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_1350::~CBO_LOC_1350()
{

}

VOID CBO_LOC_1350::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1350)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1350, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1350)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnFSDCHANGE, OnbtnFSDCHANGE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1350 message handlers

BOOL CBO_LOC_1350::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("BO_LOC_1350") );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("BO_LOC_1350 Reference Error!!") );
	}
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




BOOL CBO_LOC_1350::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
	return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
	return FALSE;
	
}

VOID CBO_LOC_1350::OnbtnCLOSE() 
{
	OnCancel();	
}

VOID CBO_LOC_1350::OnbtnFSDCHANGE() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	ChangeFSD();	

	AfxGetApp()->DoWaitCursor(-1);

	EFS_END
}


INT CBO_LOC_1350::ChangeFSD()
{
	EFS_BEGIN

	INT ids;

	CString sFSD;
	ids = GetControlData( _T("CM_BO_LOC_1350") , _T("최초배가일자") , sFSD );
	if( ids < 0 )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("ChangeFSD") );
	}

	if( sFSD.IsEmpty() ) 
	{
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1300") , _T("MainGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeFSD") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("변경할 자료가 존재하지 않습니다.") );
		return 1;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_1300"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeFSD") );

	pGrid->SelectGetHeadPosition();

	CLocProgressBarDlg* pProgress = CreateProgressBar();
	pProgress->InitRange( nSelectCnt );

	CString sLastLog;
	CLocCommonAPI::GetWorkLogMsg( this , sLastLog , __WFILE__ , __LINE__ );
	

	INT nIndex;
	CString sBookKey;
	CString sSpeciesKey;

	pGrid->SetProtectSelectColumn(TRUE);

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		pProgress->ChangeProgressBar(i);
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("BB_책KEY") , nIndex , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeFSD") );
		
		ids = pDM->GetCellData( _T("BS_종KEY") , nIndex , sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeFSD") );
		
		pDM->StartFrame();

		pDM->InitDBFieldData();

		pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sFSD );

		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		
		pDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );

		pDM->InitDBFieldData();

		pDM->AddDBFieldData( _T("HOLD_DATE") , _T("NUMERIC") , _T("SYSDATE") );

		pDM->MakeUpdateFrame( _T("IDX_BO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );

		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeFSD") );

		pDM->EndFrame();

		ids = pDM->SetCellData( _T("BB_최초배가일자") , sFSD , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ChangeFSD") );

		pGrid->SelectGetNext();
	}

	pProgress->DestroyWindow();
	if( pProgress != NULL ) delete pProgress;
	

	AfxMessageBox( _T("완료되었습니다.") );
	pGrid->Display();
	pGrid->SetProtectSelectColumn(FALSE);

	return 0;

	EFS_END

	return -1;
}

CLocProgressBarDlg* CBO_LOC_1350::CreateProgressBar()
{
	EFS_BEGIN

	CLocProgressBarDlg *ProgressBar;
	ProgressBar = new CLocProgressBarDlg( this );
	ProgressBar->Create( this );
	ProgressBar->CenterWindow( this );
	ProgressBar->ShowWindow( SW_SHOW );
	

	return ProgressBar;

	EFS_END

	return NULL;

}

HBRUSH CBO_LOC_1350::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
