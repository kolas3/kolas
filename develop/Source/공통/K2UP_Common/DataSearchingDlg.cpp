// DataSearchingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataSearchingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSearchingDlg dialog


CDataSearchingDlg::CDataSearchingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSearchingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSearchingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CDataSearchingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSearchingDlg)
	DDX_Control(pDX, IDC_ANIMATE, m_ctrlAnimate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSearchingDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSearchingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSearchingDlg message handlers

BOOL CDataSearchingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_ctrlAnimate.Open(_T("../bmp/findfile.avi")) == 0)
	{
		AfxMessageBox(_T("에니메이션 파일을 찾기 실패!"));
		EndDialog(IDCANCEL);
		return FALSE;
	} 
	m_ctrlAnimate.Open(_T("../bmp/findfile.avi"));


//	static BOOL bContinue = TRUE;
//	CWinThread* pThread  = ::AfxBeginThread(ThreadFunc, bContinue);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataSearchingDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

/*UINT CDataSearchingDlg::ThreadFunc(LPVOID pParam)
{
	
//  pContinue->MessageBox(_T("Thread 시작."));

    if(m_ctrlAnimate.Open(_T("../bmp/findfile.avi")) == 0)
	{
		AfxMessageBox(_T("에니메이션 파일을 찾기 실패!"));
		EndDialog(IDCANCEL);
		return FALSE;
	} 
	m_ctrlAnimate.Open(_T("../bmp/findfile.avi"));
	
//  pCWin->MessageBox(_T("Thread 죽음."));
	return 0;

}	*/





BOOL CDataSearchingDlg::CloseDlg()
{
	m_ctrlAnimate.Close();
	return FALSE;
}
