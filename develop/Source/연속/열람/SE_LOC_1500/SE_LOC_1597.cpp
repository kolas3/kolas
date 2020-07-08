// SE_LOC_1597.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1597.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1597 dialog


CSE_LOC_1597::CSE_LOC_1597(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1597)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bPrint = FALSE;
}

CSE_LOC_1597::~CSE_LOC_1597()
{
}

VOID CSE_LOC_1597::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1597)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1597, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1597)
	ON_BN_CLICKED(IDC_btn159_CHANGE, Onbtn159CHANGE)
	ON_BN_CLICKED(IDC_btn159_CLOSE, Onbtn159CLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1597 message handlers

BOOL CSE_LOC_1597::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr( _T("SE_LOC_1597") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CSE_LOC_1597::Onbtn159CHANGE() 
{
EFS_BEGIN

	INT ids = GetControlData( _T("CM_SE_LOC_1597") , _T("인계일자") , m_sShelfDate );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnInitDialog") );

	m_bPrint = TRUE;

	OnCancel();
	

EFS_END
}

VOID CSE_LOC_1597::Onbtn159CLOSE() 
{
EFS_BEGIN

	OnCancel();
	

EFS_END
}

HBRUSH CSE_LOC_1597::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
