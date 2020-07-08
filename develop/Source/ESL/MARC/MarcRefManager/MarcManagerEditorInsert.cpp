// MarcManagerEditorInsert.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerEditorInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerEditorInsert dialog


CMarcManagerEditorInsert::CMarcManagerEditorInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerEditorInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerEditorInsert)
	m_sEditorAlias = _T("");
	m_sEditorData = _T("");
	m_nEditorAliasType = -1;
	m_nEditorIdex = -1;
	m_sEditorTagCode = _T("");
	//}}AFX_DATA_INIT

	
    m_nEditorAliasType = 0;
	m_nEditorIdex = 0;
}


VOID CMarcManagerEditorInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerEditorInsert)
	DDX_Text(pDX, IDC_eEditorAlias, m_sEditorAlias);
	DDX_Text(pDX, IDC_eEditorData, m_sEditorData);
	DDX_Radio(pDX, IDC_rEditorAliasType1, m_nEditorAliasType);
	DDX_Radio(pDX, IDC_rEditorIndex1, m_nEditorIdex);
	DDX_Text(pDX, IDC_eEditorTagCode, m_sEditorTagCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerEditorInsert, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerEditorInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerEditorInsert message handlers

VOID CMarcManagerEditorInsert::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	if(m_sEditorAlias.IsEmpty() == TRUE)
	{
        AfxMessageBox(_T("ALIAS가 입력되지 않았습니다!"));
		return;
	}


	CDialog::OnOK();
}
