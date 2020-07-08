// TestMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestMsgDlg dialog


CTestMsgDlg::CTestMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestMsgDlg)
		
	//}}AFX_DATA_INIT
	HINSTANCE m_hREInstance;     
    m_hREInstance = LoadLibrary(_T("RICHED32.DLL"));

}


VOID CTestMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMsgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CTestMsgDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMsgDlg message handlers
