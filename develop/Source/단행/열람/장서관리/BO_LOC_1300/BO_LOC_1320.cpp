// BO_LOC_1320.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1320.h"
#include "..\LOC_MACRO\ErrorMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1320 dialog


CBO_LOC_1320::CBO_LOC_1320(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1320)
	m_nViewMode = 0;
	m_nCurrentIndex = 0;
	m_nLastIndex = 0;
	m_nFirstIndex = 0;
	bFirstView = FALSE;
	//}}AFX_DATA_INIT
}


CBO_LOC_1320::~CBO_LOC_1320()
{

}
VOID CBO_LOC_1320::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1320)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_1320, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1320)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1320 message handlers

BOOL CBO_LOC_1320::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_1320::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();

	EFS_END
}

BOOL CBO_LOC_1320::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("BO_LOC_1320")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CBO_LOC_1320::MoveIndex( INT nMode )
{	
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1320::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	ViewCurrentWindow();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1320::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;
	CString sBookRecKey;
	
	CString sParentDMAlias = _T("DM_BO_LOC_1300");
	ids = GetDataMgrData( sParentDMAlias , _T("BB_åKEY") , sBookRecKey , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1320::ViewCurrentWindow()") );

	CString sDMAlias = _T("DM_BO_LOC_1320");
	CESL_DataMgr *pDM = FindDM( sDMAlias );

	sWhere.Format( _T(" BB.REC_KEY = %s ") , sBookRecKey );

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_1320::ViewCurrentWindow()") );

	CString sCMAlias = _T("CM_BO_LOC_1320");
	ids = AllControlDisplay( sCMAlias , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_1320::ViewCurrentWindow()") );

	CString sParentCMAlias = _T("CM_BO_LOC_1300");

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_1320::ViewCurrentWindow()") );

	GetDlgItem(IDC_btnPREV)->EnableWindow( TRUE );
	GetDlgItem(IDC_btnNEXT)->EnableWindow( TRUE );

	if( m_nCurrentIndex == m_nFirstIndex )
		GetDlgItem(IDC_btnPREV)->EnableWindow( FALSE );	
	if( m_nCurrentIndex == m_nLastIndex )
		GetDlgItem(IDC_btnNEXT)->EnableWindow( FALSE );
	
	pGrid->SetReverse( m_nCurrentIndex );

	return 0;


	EFS_END
	return -1;

}


VOID CBO_LOC_1320::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	bFirstView = TRUE;

	InitSetStartPos();

	INT ids = ViewCurrentWindow();
	if( ids < 0 ) return;

	bFirstView = FALSE;
	

	EFS_END
}

INT CBO_LOC_1320::InitSetStartPos()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->SelectGetIdx();

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_1320::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex( 0 );	

	EFS_END
}

VOID CBO_LOC_1320::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex( 1 );

	EFS_END
}

VOID CBO_LOC_1320::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnClose();

	EFS_END
}

CESL_Grid* CBO_LOC_1320::GetMainGridPtr()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_1300");

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

HBRUSH CBO_LOC_1320::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
