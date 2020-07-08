// ErrorLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "ErrorLogDlg.h"

#include "MyListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorLogDlg dialog


CErrorLogDlg::CErrorLogDlg(CErrorQueue* pErrorQueue, CWnd* pParent)
	: CDialog(CErrorLogDlg::IDD, pParent), m_ListCtrl(IDC_ERROR_LIST)
{
	m_pErrorQueue = pErrorQueue;
	m_iCurrentIdx = -1;
	m_iCurrentErrorLine  = -1;
	m_pParent	  = pParent;
	//{{AFX_DATA_INIT(CErrorLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CErrorLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorLogDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorLogDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorLogDlg)
	ON_NOTIFY(NM_CLICK, IDC_ERROR_LIST, OnClickErrorList)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorLogDlg message handlers

BOOL CErrorLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_ERROR_LIST);
	long lStyleOld = GetWindowLong( pList->m_hWnd, GWL_STYLE );
	lStyleOld     &= ~(LVS_TYPEMASK);
	lStyleOld     |= LVS_REPORT;
	SetWindowLong( pList->m_hWnd, GWL_STYLE, lStyleOld );

	UINT dwExStyle;
	dwExStyle = pList->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwExStyle = pList->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwExStyle | LVS_EX_FULLROWSELECT);
			
    pList -> InsertColumn(0,_T("Line"),LVCFMT_LEFT,100,0);
	pList -> InsertColumn(1,ConvertID2Chinese(IDS_STRING_5001),LVCFMT_LEFT,500,0);	//_T("오류사항")
	pList -> InsertColumn(2,ConvertID2Chinese(IDS_STRING_5002),LVCFMT_LEFT,500,0);	//_T("해결방안")
	m_ListCtrl.SetParentPtr(this);	

	INT iMaxCount = m_pErrorQueue->GetErrorCount();

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		m_ListCtrl.AddItem(0,0,m_pErrorQueue->GetLine(i));
		m_ListCtrl.AddItem(0,1,m_pErrorQueue->GetErrorMessage(i));
		m_ListCtrl.AddItem(0,2,m_pErrorQueue->GetSolution(i));
	}
	
	return TRUE;  
}

VOID CErrorLogDlg::OnClickErrorList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if(m_ListCtrl.IsSelectedItem()) {
		CString sLine = m_ListCtrl.GetSelectedItem(0);
		INT iLine = _ttoi(sLine);
		m_iCurrentIdx = m_ListCtrl.GetSelectedItemIdx();
		m_iCurrentErrorLine = _ttoi(m_ListCtrl.GetSelectedItem(0));
		m_pParent -> PostMessage(WM_F4);
	}
}

BOOL CErrorLogDlg::Create(CWnd * pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CErrorLogDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg -> message == WM_KEYDOWN && pMsg->wParam == VK_F4) {
		INT iMaxCount = m_pErrorQueue -> GetErrorCount();
		if(iMaxCount <= 0) return TRUE;

		if(m_iCurrentIdx < 0) { 
			m_iCurrentIdx = 0;
		} else {
			
			if(iMaxCount - 1 > m_iCurrentIdx) {
				m_iCurrentIdx++;
			} else {
				m_iCurrentIdx = 0;
			}
			
		}

		m_ListCtrl.SetSelectedItem(m_iCurrentIdx);

		m_iCurrentErrorLine = _ttoi(m_ListCtrl.GetSelectedItem(0));
		m_pParent -> PostMessage(WM_F4);

		return TRUE;
	}		

	if(pMsg -> message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		PostMessage(WM_CLOSE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CErrorLogDlg::OnClose() 
{
	//m_pParent -> PostMessage(WM_GOODBYE);
	CDialog::OnClose();
}

INT CErrorLogDlg::GetCurrentErrorLine()
{
	return m_iCurrentErrorLine;
}
