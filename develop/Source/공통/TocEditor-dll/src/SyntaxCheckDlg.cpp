// SyntaxCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SyntaxCheckDlg.h"

#include "TocEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSyntaxCheckDlg dialog


CSyntaxCheckDlg::CSyntaxCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSyntaxCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSyntaxCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentWnd= NULL ;
}


VOID CSyntaxCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyntaxCheckDlg)
	DDX_Control(pDX, IDC_REPORT_OUTPUT, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSyntaxCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CSyntaxCheckDlg)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_REPORT_OUTPUT, OnColumnclickReportOutput)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_OUTPUT, OnClickReportOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyntaxCheckDlg message handlers

BOOL CSyntaxCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwStyles = m_List.GetExtendedStyle();
	m_List.SetExtendedStyle( dwStyles | LVS_EX_FULLROWSELECT );

	m_List.AddColumn(_T("위치"), 0, 50, LVCFMT_LEFT);
	m_List.AddColumn(_T("오류 내용"), 1, 470, LVCFMT_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSyntaxCheckDlg::OnColumnclickReportOutput(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_List.SortListView(pNMListView->iSubItem, TRUE);

	*pResult = 0;
}

VOID CSyntaxCheckDlg::OnClickReportOutput(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DisplayErrorLine() ;

	*pResult = 0;
}

VOID CSyntaxCheckDlg::DisplayErrorLine()
{
	UINT iCnt = m_List.GetSelectedCount() ;
	if ( iCnt != 1 ) return ;

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	INT nIdx = m_List.GetNextSelectedItem(pos);
	INT iCurrentErrorLine= 0 ;
	if(nIdx>=0) {
		CString sLine = m_List.GetItemText(nIdx, 0);
		iCurrentErrorLine = _ttoi(sLine);
	}

	if ( m_pParentWnd==NULL ) {
		AfxMessageBox( _T("m_pParentWnd is NULL error!"), MB_ICONERROR|MB_OK ) ;
		return ;
	}

	((CTocEditorDlg*)m_pParentWnd)->SetCurrentErrorLine(iCurrentErrorLine) ;
}

VOID CSyntaxCheckDlg::SetErrorMsg(CString sLine, CString sMsg, CString sSolution, INT nIdxLine)
{
	if ( m_List.m_hWnd==NULL ) {
		AfxMessageBox(_T("ListCtrl have not created yet."));
		return ;
	}

	TCHAR szTmp[128] ;
	_tcscpy( szTmp , sLine ) ;
	m_List.AddItemData ( szTmp, nIdxLine, 0, -1 );
	_tcscpy( szTmp , sMsg ) ;
	m_List.SetItemData1( szTmp, nIdxLine, 1, -1 );
	_tcscpy( szTmp , sSolution ) ;
	m_List.SetItemData1( szTmp, nIdxLine, 2, -1 );
}

VOID CSyntaxCheckDlg::DeleteAllItems()
{
	m_List.DeleteAllItems() ;
}

VOID CSyntaxCheckDlg::SetParentWnd(CWnd *pParentWnd)
{
	m_pParentWnd= pParentWnd ;
}
