// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

// BO_STATICS_CLICK_LIST_1300.cpp : implementation file
//

#include "stdafx.h"
//#include "bo_statics_1300.h"
#include "BO_STATICS_CLICK_LIST_1300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BO_STATICS_CLICK_LIST_1300 dialog


BO_STATICS_CLICK_LIST_1300::BO_STATICS_CLICK_LIST_1300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(BO_STATICS_CLICK_LIST_1300::IDD, pParent)
{
	//{{AFX_DATA_INIT(BO_STATICS_CLICK_LIST_1300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void BO_STATICS_CLICK_LIST_1300::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BO_STATICS_CLICK_LIST_1300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BO_STATICS_CLICK_LIST_1300, CDialog)
	//{{AFX_MSG_MAP(BO_STATICS_CLICK_LIST_1300)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BO_STATICS_CLICK_LIST_1300 message handlers

BOOL BO_STATICS_CLICK_LIST_1300::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 전 작업자랑 같은 작업방식
	// 임시로 이미 만들어진 똑같은 설정 파일들로 ..
	if(InitESL_Mgr(_T("SM_BO_STATICS_LIST")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_BO_STATICS_LIST"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_BO_STATICS_LIST Error!"));
		return FALSE;
	}

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_BO_STATICS_LIST"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_BO_STATICS_LIST Error!"));
		return FALSE;
	}	 

	m_pGrid->SetTextMatrix(0, 1, _T("번호") );
	m_pGrid->SetTextMatrix(0, 2, _T("제목") );

	m_pArrQuery->FillGrid( m_pGrid );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// KOL.RED.2018.211 ---------------------------------------------------------------------------