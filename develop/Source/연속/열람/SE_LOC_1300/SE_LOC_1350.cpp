// SE_LOC_1350.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1350.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1350 dialog


CSE_LOC_1350::CSE_LOC_1350(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1350)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1350::~CSE_LOC_1350()
{
}

VOID CSE_LOC_1350::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1350)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1350, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1350)
	ON_BN_CLICKED(IDC_btnFSDCHANGE, OnbtnFSDCHANGE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1350 message handlers

BOOL CSE_LOC_1350::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr( _T("BO_LOC_1350") );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("BO_LOC_1350 Reference Error!!") );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




BOOL CSE_LOC_1350::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
	return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
	return FALSE;
	
}

VOID CSE_LOC_1350::OnbtnFSDCHANGE() 
{
	EFS_BEGIN
		
	AfxGetApp()->DoWaitCursor(1);
	
	ChangeFSD();	
	
	AfxGetApp()->DoWaitCursor(-1);
	
	EFS_END	
}

VOID CSE_LOC_1350::OnbtnCLOSE() 
{
	OnCancel();		
}



INT CSE_LOC_1350::ChangeFSD()
{
	EFS_BEGIN

	INT ids;

	/// 0. 최초 배가일자 확인
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

	/// 1. 그리드에서 선정된 자료만을 보낸다.
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1300") , _T("MainGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeFSD") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("변경할 자료가 존재하지 않습니다.") );
		return 1;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_SE_LOC_1300"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeFSD") );

	pGrid->SelectGetHeadPosition();

	/// 프로그래스바 생성

	CLocProgressBarDlg* pProgress = CreateProgressBar();
	pProgress->InitRange( nSelectCnt );

	/// -----------------------------

	INT nIndex;
	CString sBookKey;

	CString sLastLog;
	CLocCommonAPI::GetWorkLogMsg( this , sLastLog , __WFILE__ , __LINE__ );

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		pProgress->ChangeProgressBar(i);
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("BB_책KEY") , nIndex , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeFSD") );
		
		
		/// DB에 날리기
		pDM->StartFrame();

		pDM->InitDBFieldData();

		pDM->AddDBFieldData( _T("SHELF_DATE") , _T("STRING") , sFSD );

		pDM->AddDBFieldData( _T("LAST_WORK") , _T("STRING") , sLastLog );

		pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );

		
		

		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeFSD") );

		pDM->EndFrame();

		// 메인 DM과 그리드 수정
		ids = pDM->SetCellData( _T("BB_최초배가일자") , sFSD , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ChangeFSD") );

		ids = pGrid->DisplayLine(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ChangeFSD") );

		pGrid->SetAt( nIndex , 0 , _T("V") );
		

		pGrid->SelectGetNext();
	}

	pProgress->DestroyWindow();
	if( pProgress != NULL ) delete pProgress;
	

	AfxMessageBox( _T("완료되었습니다.") );

	return 0;

	EFS_END

	return -1;
}

CLocProgressBarDlg* CSE_LOC_1350::CreateProgressBar()
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

HBRUSH CSE_LOC_1350::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
