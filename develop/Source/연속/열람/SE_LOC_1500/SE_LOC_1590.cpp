// SE_LOC_1590.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1590.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1590 dialog


CSE_LOC_1590::CSE_LOC_1590(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1590)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1590::~CSE_LOC_1590()
{
}

VOID CSE_LOC_1590::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1590)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1590, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1590)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1590 message handlers

BOOL CSE_LOC_1590::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SE_LOC_1590")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_SE_LOC_15(this);

	DisplayDefaultValue();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1590::OnClose() 
{
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;
	
	CDialog::OnClose();
}


/// 화면의 종을 보여준다.
INT CSE_LOC_1590::DisplayDefaultValue()
{
	INT ids;

	CString sMainDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayDefaultValue") );

	CESL_Grid *pMainGrid = (CESL_Grid*)FindControl(_T("CM_SE_LOC_1500"),_T("메인그리드"));
	if( pMainGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("종정보KEY") , pMainGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DisplayDefaultValue") ); 

	pMainGrid->SelectMakeList();

	CString sCMAlias = _T("CM_SE_LOC_1590");
	ids = SetControlData( sCMAlias , _T("종수") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 

	return 0;
}


VOID CSE_LOC_1590::OnbtnCHANGE() 
{
	INT ids;
	ids = m_pSpfWork->SPFExcutePath( ALL_SPECIES_SHELF_CHANGE );	
	if( ids == 0 )
	{
		AfxMessageBox( _T("선택된 종배가실이 변경되었습니다.") );
		return ;
	}
	if( ids > 0 )
	{
		return ;
	}
}

VOID CSE_LOC_1590::OnbtnCLOSE() 
{
	OnCancel();
}

HBRUSH CSE_LOC_1590::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
