// YonSujiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YonSujiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYonSujiDlg dialog


CYonSujiDlg::CYonSujiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYonSujiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYonSujiDlg)
	m_strId = _T("");
	m_strKey = _T("");
	m_strCall = _T("");
	m_strCount = _T("");
	m_strDate = _T("");
	m_strPub = _T("");
	m_strSertitle = _T("");
	m_strIssue = _T("");
	//}}AFX_DATA_INIT
}


VOID CYonSujiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYonSujiDlg)
	DDX_Text(pDX, IDC_EDIT_ID, m_strId);
	DDX_Text(pDX, IDC_EDIT_KEY, m_strKey);
	DDX_Text(pDX, IDC_EDIT_CALL, m_strCall);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_strCount);
	DDX_Text(pDX, IDC_EDIT_DATE, m_strDate);
	DDX_Text(pDX, IDC_EDIT_PUB, m_strPub);
	DDX_Text(pDX, IDC_EDIT_SERTITLE, m_strSertitle);
	DDX_Text(pDX, IDC_EDIT_ISSUE, m_strIssue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYonSujiDlg, CDialog)
	//{{AFX_MSG_MAP(CYonSujiDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYonSujiDlg message handlers

VOID CYonSujiDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData() ;

	CreateMessage() ;

	CDialog::OnOK() ;
}

VOID CYonSujiDlg::CreateMessage()
{
	m_strBibilography.Empty() ;

	CString tmp ;
	tmp.Format( _T("<id>%s\r\n") , m_strId ) ;
	m_strBibilography += tmp ;

	tmp.Format( _T("<key>%s\r\n") , m_strKey ) ;
	m_strBibilography += tmp ;

	tmp.Format( _T("<call>%s\r\n") , m_strCall ) ;
	m_strBibilography += tmp ;

	tmp.Format( _T("<count>%s\r\n") , m_strCount ) ;
	m_strBibilography += tmp ;
	
	tmp.Format( _T("<date>%s\r\n") , m_strDate ) ;
	m_strBibilography += tmp ;

	tmp.Format( _T("<pub>%s\r\n") , m_strPub ) ;
	m_strBibilography += tmp ;

	tmp.Format( _T("<sertitle>%s\r\n") , m_strSertitle ) ;
	m_strBibilography += tmp ;

	tmp.Format( _T("<issue>%s\r\n") , m_strIssue ) ;
	m_strBibilography += tmp ;

}
