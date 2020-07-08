// StateInsert.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "StateInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStateInsert dialog


CStateInsert::CStateInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CStateInsert::IDD, pParent)
{
	
//	pTmpParent = (CSPFView*)pParent;
	
	//{{AFX_DATA_INIT(CStateInsert)
	m_NAME = _T("");
	m_DESC = _T("");
	//}}AFX_DATA_INIT
}


VOID CStateInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStateInsert)
	DDX_Text(pDX, IDC_STATE_NAME, m_NAME);
	DDX_Text(pDX, IDC_STATE_DESC, m_DESC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStateInsert, CDialog)
	//{{AFX_MSG_MAP(CStateInsert)
	ON_BN_CLICKED(IDC_btnStINSERT, OnbtnStINSERT)
	ON_BN_CLICKED(IDC_btnStNOMAKE, OnbtnStNOMAKE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateInsert message handlers

VOID CStateInsert::OnbtnStINSERT() 
{
	// TODO: Add your control notification handler code here
    //사용자의 입력을 받아서 DataMgr를 통해 DB에 저장
	

	CDialog::OnOK();
}

VOID CStateInsert::OnbtnStNOMAKE() 
{
	// TODO: Add your control notification handler code here
	//만든 그림을 지우고 다이얼로그를 종료  
	CDialog::OnCancel();
}
