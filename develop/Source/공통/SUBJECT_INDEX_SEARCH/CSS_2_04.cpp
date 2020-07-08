// CSS_2_04.cpp : implementation file
//

#include "stdafx.h"
#include "CSS_2_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_04 dialog
//
// 2005.01.25 확장개선 사업에서는 요청은 제외
/////////////////////////////////////////////////////////////////////////////


CCSS_2_04::CCSS_2_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSS_2_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CCSS_2_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSS_2_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCSS_2_04, CDialog)
	//{{AFX_MSG_MAP(CCSS_2_04)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_04 message handlers

VOID CCSS_2_04::OnOK() 
{
	//요청 (확장개섬에선 안함)
	
	CDialog::OnOK();
}

VOID CCSS_2_04::OnCancel() 
{
	// 닫기 (확장개섬에선 안함)
	
	CDialog::OnCancel();
}
