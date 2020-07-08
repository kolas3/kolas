// BodyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BodyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBodyDlg dialog


CBodyDlg::CBodyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBodyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBodyDlg)
	m_bBody = TRUE;
	m_bTable = FALSE;
	m_bFigure = FALSE;
	m_strBody = _T("목차") ;
	m_strTable = _T("표목차") ;
	m_strFigure = _T("그림목차") ;
	//}}AFX_DATA_INIT
}


VOID CBodyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBodyDlg)
	DDX_Check(pDX, IDC_CHECK_BODY, m_bBody);
	DDX_Check(pDX, IDC_CHECK_TABLE, m_bTable);
	DDX_Check(pDX, IDC_CHECK_FIGURE, m_bFigure);
	DDX_Text(pDX, IDC_EDIT_BODY, m_strBody) ;
	DDX_Text(pDX, IDC_EDIT_TABLE, m_strTable) ;
	DDX_Text(pDX, IDC_EDIT_FIGURE, m_strFigure) ;
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBodyDlg, CDialog)
	//{{AFX_MSG_MAP(CBodyDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_CHECK_BODY, OnCheckBody)
	ON_BN_CLICKED(IDC_CHECK_TABLE, OnCheckTable)
	ON_BN_CLICKED(IDC_CHECK_FIGURE, OnCheckFigure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBodyDlg message handlers

BOOL CBodyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CMenu* mnu= NULL ;
	mnu= GetSystemMenu(FALSE) ;
	mnu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED ) ;

	GetDlgItem(IDC_EDIT_TABLE)->EnableWindow(FALSE) ;
	GetDlgItem(IDC_EDIT_FIGURE)->EnableWindow(FALSE) ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CBodyDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE )		// ESC 키를 막자.
		return TRUE ;

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBodyDlg::OnCheckBody() 
{
	// TODO: Add your control notification handler code here
	if( IsDlgButtonChecked(IDC_CHECK_BODY) )	GetDlgItem(IDC_EDIT_BODY)->EnableWindow() ;
	else	GetDlgItem(IDC_EDIT_BODY)->EnableWindow(FALSE) ;
}

VOID CBodyDlg::OnCheckTable() 
{
	// TODO: Add your control notification handler code here
	if( IsDlgButtonChecked(IDC_CHECK_TABLE) )	GetDlgItem(IDC_EDIT_TABLE)->EnableWindow() ;
	else	GetDlgItem(IDC_EDIT_TABLE)->EnableWindow(FALSE) ;
}

VOID CBodyDlg::OnCheckFigure() 
{
	// TODO: Add your control notification handler code here
	if( IsDlgButtonChecked(IDC_CHECK_FIGURE) )	GetDlgItem(IDC_EDIT_FIGURE)->EnableWindow() ;
	else	GetDlgItem(IDC_EDIT_FIGURE)->EnableWindow(FALSE) ;
}

VOID CBodyDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData() ;

	CreateMessage() ;

	CDialog::OnOK() ;
}

VOID CBodyDlg::CreateMessage()
{
	m_strResult.Empty() ;
	if ( ! m_bBody )	m_strBody.Empty() ;
	if ( ! m_bTable )	m_strTable.Empty() ;
	if ( ! m_bFigure )	m_strFigure.Empty() ;

	if( m_bBody )
	{
		m_strResult += _T("<body>") + m_strBody + _T("\r\n") ;
		m_strResult += _T("\r\n") ;
		m_strResult += _T("</body>\r\n") ;
	}

	if( m_bTable )
	{
		m_strResult += _T("<table>") + m_strTable + _T("\r\n") ;
		m_strResult += _T("\r\n") ;
		m_strResult += _T("</table>\r\n") ;
	}

	if( m_bFigure )
	{
		m_strResult += _T("<figure>") + m_strFigure + _T("\r\n") ;
		m_strResult += _T("\r\n") ;
		m_strResult += _T("</figure>\r\n") ;
	}
}
