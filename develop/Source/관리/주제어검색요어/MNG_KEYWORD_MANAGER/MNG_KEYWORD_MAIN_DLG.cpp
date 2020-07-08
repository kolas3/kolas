// MNG_KEYWORD_MAIN_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_KEYWORD_MAIN_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_MAIN_DLG dialog


CMNG_KEYWORD_MAIN_DLG::CMNG_KEYWORD_MAIN_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KEYWORD_MAIN_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KEYWORD_MAIN_DLG::~CMNG_KEYWORD_MAIN_DLG()
{
}

VOID CMNG_KEYWORD_MAIN_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KEYWORD_MAIN_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KEYWORD_MAIN_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_KEYWORD_MAIN_DLG)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_MAIN_DLG message handlers

BOOL CMNG_KEYWORD_MAIN_DLG::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_KEYWORD_MAIN_DLG::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(0, 0, cx, cy);		
	
	
}

BOOL CMNG_KEYWORD_MAIN_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_관리_주제어검색요어_메인")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CMNG_KEYWORD_MAIN_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_KEYWORD_MAIN_DLG)
	ON_EVENT(CMNG_KEYWORD_MAIN_DLG, IDC_gMAIN, -600 /* Click */, OnClickgMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_KEYWORD_MAIN_DLG::OnClickgMAIN() 
{
	INT iClickGridRow = SetCheckGrid();	
}

INT CMNG_KEYWORD_MAIN_DLG::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	if( 1 != col ) return 0;
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		
		if( row == i )
		{
			pGrid->SetText( _T("V") );
		}
		else if( _T("V") == pGrid->GetText() )
			pGrid->SetText( _T("") );
	}
	
	pGrid->SetRow( row );	
	return row;

}

HBRUSH CMNG_KEYWORD_MAIN_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
