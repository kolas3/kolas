// RPTUpdaterClientCode.cpp : implementation file
//

#include "stdafx.h"
#include "RPTUpdaterClient.h"
#include "RPTUpdaterClientCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRPTUpdaterClientCode dialog


CRPTUpdaterClientCode::CRPTUpdaterClientCode(CWnd* pParent /*=NULL*/)
	: CDialog(CRPTUpdaterClientCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRPTUpdaterClientCode)
	m_strLibraryCode = _T("");
	//}}AFX_DATA_INIT
}


VOID CRPTUpdaterClientCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRPTUpdaterClientCode)
	DDX_Text(pDX, IDC_edtLibraryCode, m_strLibraryCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRPTUpdaterClientCode, CDialog)
	//{{AFX_MSG_MAP(CRPTUpdaterClientCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRPTUpdaterClientCode message handlers

VOID CRPTUpdaterClientCode::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_strLibraryCode.IsEmpty())
	{
		AfxMessageBox(_T("도서관 부호를 입력해 주십시요!"));
		return;
	}
	

	m_strLibraryCode.TrimLeft();
	m_strLibraryCode.TrimRight();


	CDialog::OnOK();
}
