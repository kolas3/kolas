// SearchZipCode02Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchZipCode02Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCode02Dlg dialog


CSearchZipCode02Dlg::CSearchZipCode02Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchZipCode02Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sDMAlias = _T("DM_SEARCH_ZIP_CODE");
	m_sGridCMAlias = _T("CM_SEARCH_ZIP_CODE_GRID");
	m_sGridAlias = _T("주소그리드");
	m_sCMAlias = _T("CM_SEARCH_ZIP_CODE_02");

	// KOL.UDF.022 시큐어코딩 보완
	m_nMode = -1;
	m_nCurrentPos = -1;
	m_pGridCM = NULL;
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
}

CSearchZipCode02Dlg::~CSearchZipCode02Dlg()
{
	
}

VOID CSearchZipCode02Dlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchZipCode02Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSearchZipCode02Dlg, CDialog)
	//{{AFX_MSG_MAP(CSearchZipCode02Dlg)
	ON_BN_CLICKED(IDC_btnZIP_CODE_PREV, OnbtnZIPCODEPREV)
	ON_BN_CLICKED(IDC_btnZIP_CODE_NEXT, OnbtnZIPCODENEXT)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SAVE, OnbtnZIPCODESAVE)
	ON_BN_CLICKED(IDC_btnZIP_CODE_CLOSE, OnbtnZIPCODECLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCode02Dlg message handlers

BOOL CSearchZipCode02Dlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_SEARCH_ZIP_CODE_02")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pGridCM = FindCM( m_sGridCMAlias );
	if( m_pGridCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pGrid = (CESL_Grid*)(m_pGridCM->FindControl( m_sGridAlias ));
	if( m_pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pDM = FindDM( m_sDMAlias );
	if( m_pDM == NULL )ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	m_pCM = FindCM( m_sCMAlias );
	if( m_pCM == NULL )ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("OnInitDialog") );

	ViewScreen();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


VOID CSearchZipCode02Dlg::SetMode( INT nMode , INT nCurrentPos/*=-1*/)
{
	EFS_BEGIN

	m_nMode = nMode;
	m_nCurrentPos = nCurrentPos;

	EFS_END
}

VOID CSearchZipCode02Dlg::ViewScreen()
{
	EFS_BEGIN

	INT ids;
	if( m_nMode == 0 ) 
	{
		SetWindowText( _T("주소 입력") );

		GetDlgItem(IDC_btnZIP_CODE_PREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnZIP_CODE_NEXT)->ShowWindow(SW_HIDE);
	}
	else if( m_nMode == 1 )
	{
		SetWindowText( _T("주소 수정") );

		GetDlgItem(IDC_btnZIP_CODE_PREV)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnZIP_CODE_NEXT)->EnableWindow(TRUE);
		if( m_nCurrentPos == 0 ) GetDlgItem(IDC_btnZIP_CODE_PREV)->EnableWindow(FALSE);
		if( m_nCurrentPos == m_pDM->GetRowCount()-1 ) GetDlgItem(IDC_btnZIP_CODE_NEXT)->EnableWindow(FALSE);

		ids = AllControlDisplay( m_sCMAlias , m_nCurrentPos );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("ViewScreen") );

		m_pGrid->SetReverse( m_nCurrentPos );
	}

	EFS_END
}


VOID CSearchZipCode02Dlg::OnbtnZIPCODEPREV() 
{
	EFS_BEGIN

	m_nCurrentPos--;
	ViewScreen();

	EFS_END
}

VOID CSearchZipCode02Dlg::OnbtnZIPCODENEXT() 
{
	EFS_BEGIN

	m_nCurrentPos++; 
	ViewScreen();

	EFS_END
}

VOID CSearchZipCode02Dlg::OnbtnZIPCODESAVE() 
{
	EFS_BEGIN

	if( m_nMode == 0 )
	{
		Input();
	}
	else
	{
		Modify();
	}

	EFS_END
}

INT CSearchZipCode02Dlg::Input() 
{
	EFS_BEGIN

	INT ids;
	CString sRecKey;

	ids = m_pDM->MakeRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("Input") );	

	m_pDM->StartFrame();

	m_pDM->InitDBFieldData();

	ids = m_pDM->MakeControlMgrFiledData( (CObject*)m_pCM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("Input") );	

	m_pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

	ids = m_pDM->MakeControlMgrInsertFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("Input") );	

	ids = m_pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("Input") );	

	ids = m_pDM->EndFrame();
	
	m_pDM->InsertRow(-1);

	INT nInsertIndex = m_pDM->GetRowCount() -1;

	const INT nCnt = 5;
	TCHAR *sDMFieldAlias[nCnt] = 
	{
		_T("ZIP_CODE") , _T("SIDO") , _T("GUGUN") , _T("DONG") , _T("BUNJI")
	};

	CString sData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pCM->GetControlMgrData( sDMFieldAlias[i] , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -14 , _T("Input") );

		ids = m_pDM->SetCellData( sDMFieldAlias[i] , sData , nInsertIndex );
		if( ids < 0 ){
			AfxMessageBox(sDMFieldAlias[i]);
			return -1;
		}
	}
	ids = m_pDM->SetCellData( _T("REC_KEY") , sRecKey, nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("Input") );	
	
	ids = AllControlDisplay( m_sGridCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("Modify") );	

	return 0;

	EFS_END
	return -1;

}

INT CSearchZipCode02Dlg::Modify() 
{
	EFS_BEGIN

	INT ids;
	CString sRecKey;

	ids = m_pDM->GetCellData( _T("REC_KEY") , m_nCurrentPos , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("Modify") );	

	m_pDM->StartFrame();

	m_pDM->InitDBFieldData();

	ids = m_pDM->MakeControlMgrFiledData( (CObject*)m_pCM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("Modify") );	

	ids = m_pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"),_T("NUMERIC"),sRecKey);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("Modify") );	

	ids = m_pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("Modify") );	
	
	ids = m_pDM->EndFrame();

	INT nInsertIndex = m_nCurrentPos;

	const INT nCnt = 5;
	TCHAR *sDMFieldAlias[nCnt] = 
	{
		_T("ZIP_CODE") , _T("SIDO") , _T("GUGUN") , _T("DONG") , _T("BUNJI")
	};

	CString sData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pCM->GetControlMgrData( sDMFieldAlias[i] , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -14 , _T("Modify") );

		ids = m_pDM->SetCellData( sDMFieldAlias[i] , sData , nInsertIndex );
		if( ids < 0 ){
			AfxMessageBox(sDMFieldAlias[i]);
			return -1;
		}
	}
	ids = m_pDM->SetCellData( _T("REC_KEY") , sRecKey, nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("Modify") );	
	
	ids = AllControlDisplay( m_sGridCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("Modify") );	

	m_pGrid->SetReverse( m_nCurrentPos );

	return 0;

	EFS_END
	return -1;

}

VOID CSearchZipCode02Dlg::OnbtnZIPCODECLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

HBRUSH CSearchZipCode02Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
