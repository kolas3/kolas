// IndexInsert.cpp : implementation file
//

#include "stdafx.h"
#include "IndexInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexInsert dialog


CIndexInsert::CIndexInsert(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexInsert)
	m_strIndexName = _T("");
	//}}AFX_DATA_INIT
}


VOID CIndexInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexInsert)
	DDX_Text(pDX, IDC_edtNewIndexName, m_strIndexName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexInsert, CDialog)
	//{{AFX_MSG_MAP(CIndexInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexInsert message handlers

VOID CIndexInsert::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	m_strIndexName.TrimLeft();
	m_strIndexName.TrimRight();

	if(m_strIndexName.IsEmpty())
	{
		AfxMessageBox(_T("색인 이름이 입력되지 않았습니다!"));
		return;
	}


	CDialog::OnOK();
}
