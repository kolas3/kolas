// MNG_KEYWORD_DUP_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_KEYWORD_DUP_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_DUP_DLG dialog


CMNG_KEYWORD_DUP_DLG::CMNG_KEYWORD_DUP_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KEYWORD_DUP_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KEYWORD_DUP_DLG::~CMNG_KEYWORD_DUP_DLG()
{
}

VOID CMNG_KEYWORD_DUP_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KEYWORD_DUP_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KEYWORD_DUP_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_KEYWORD_DUP_DLG)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_DUP_DLG message handlers

BOOL CMNG_KEYWORD_DUP_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_관리_주제어검색요어_중복주제어")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	
	AllControlDisplay(_T("CM_관리_주제어검색요어_중복주제어"));

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_KEYWORD_DUP_DLG::OnOk() 
{
EFS_BEGIN
	SelectMakeList(_T("CM_관리_주제어검색요어_중복주제어"), _T("GRID"));
	INT n_index = SelectGetHeadPosition(_T("CM_관리_주제어검색요어_중복주제어"), _T("GRID"));
	if(n_index < 0)
	{
		//에러처리
		AfxMessageBox(_T("주제어를 선택하십시오."));
		return ;
	}	

	m_INDEX = n_index;

	CDialog::OnOK();
EFS_END	
}

INT CMNG_KEYWORD_DUP_DLG::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gDup );
	
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

BEGIN_EVENTSINK_MAP(CMNG_KEYWORD_DUP_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_KEYWORD_DUP_DLG)
	ON_EVENT(CMNG_KEYWORD_DUP_DLG, IDC_gDup, -600 /* Click */, OnClickgDup, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_KEYWORD_DUP_DLG::OnClickgDup() 
{
	INT iClickGridRow = SetCheckGrid();	
}

// 선택된 주제어를 부모에 전해줘
INT CMNG_KEYWORD_DUP_DLG::GetSelectIDX()
{
	return m_INDEX;
}

HBRUSH CMNG_KEYWORD_DUP_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
