// MarcManagerGroupInsert.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerGroupInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerGroupInsert dialog


CMarcManagerGroupInsert::CMarcManagerGroupInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerGroupInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerGroupInsert)
	m_sGroupCode = _T("");
	m_sGroupDescrip = _T("");
	//}}AFX_DATA_INIT

    m_bModifyFlag = FALSE;

}


VOID CMarcManagerGroupInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerGroupInsert)
	DDX_Text(pDX, IDC_eGroupCode, m_sGroupCode);
	DDX_Text(pDX, IDC_eGroupDescrip, m_sGroupDescrip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerGroupInsert, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerGroupInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerGroupInsert message handlers

VOID CMarcManagerGroupInsert::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	 
	if(m_sGroupCode.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("그룹코드가 입력되지 않았습니다!"));
		return;
	}

	CDialog::OnOK();
}

BOOL CMarcManagerGroupInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bModifyFlag == TRUE)
	{

       GetDlgItem(IDC_eGroupCode)->EnableWindow(FALSE);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
