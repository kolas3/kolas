#include "stdafx.h"
#include "BO_LOC_3199.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3199::CBO_LOC_3199(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
}

CBO_LOC_3199::~CBO_LOC_3199()
{
}
VOID CBO_LOC_3199::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBO_LOC_3199)
	//}}AFX_DATA_MAP		
	
}


BEGIN_MESSAGE_MAP(CBO_LOC_3199, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3199)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBO_LOC_3199::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;
	
	ids = InitESL_Mgr(_T("BO_LOC_3199"));
	if( ids < 0 ) return -1;
	
	HICON shit = AfxGetApp()->LoadIcon( IDR_MESSAGEBOX );

	SetIcon( shit , FALSE );
	
	return TRUE;  
	              
	EFS_END

	return FALSE;
}

INT CBO_LOC_3199::ViewMessageHistory()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3199");
	CString sDMAlias = _T("DM_메시지박스");

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( sCMAlias , _T("메시지그리드") );
	if( pGrid == NULL ) return 0;

	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) return 0;

	CLocCommonAPI::InsertGridRow( this , pGrid , pDM->GetRowCount()-1 , pDM->GetRowCount()-1 , 500 );

	if( pGrid->GetCount() == 1 ) return 0;

	pGrid->SetTopRow(pGrid->GetCount()-1);

	return 0;

	EFS_END

	return -1;
}

BOOL CBO_LOC_3199::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
	return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
	return FALSE;
	
}

VOID CBO_LOC_3199::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return ;

	CString sCMAlias = _T("CM_BO_LOC_3199");
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( sCMAlias , _T("메시지그리드") );
	if( pGrid == NULL ) return ;

	pGrid->MoveWindow( 0 , 30 , cx , cy );

	EFS_END
	
}

VOID CBO_LOC_3199::OnbtnCLEAR() 
{
	CString sDMAlias = _T("DM_메시지박스");
	CString sCMAlias = _T("CM_BO_LOC_3199");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) return ;

	pDM->FreeData();

	AllControlDisplay( sCMAlias );
	
}

HBRUSH CBO_LOC_3199::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
