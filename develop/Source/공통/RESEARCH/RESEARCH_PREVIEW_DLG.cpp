// RESEARCH_PREVIEW_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RESEARCH_PREVIEW_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_PREVIEW_DLG dialog


CRESEARCH_PREVIEW_DLG::CRESEARCH_PREVIEW_DLG(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRESEARCH_PREVIEW_DLG)
		// 18.11.12 JMJ KOL.RED.2018.006
		m_HtmlView = NULL;
	//}}AFX_DATA_INIT
}

CRESEARCH_PREVIEW_DLG::~CRESEARCH_PREVIEW_DLG()
{
	
}

BOOL CRESEARCH_PREVIEW_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRESEARCH_PREVIEW_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESEARCH_PREVIEW_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESEARCH_PREVIEW_DLG, CDialog)
	//{{AFX_MSG_MAP(CRESEARCH_PREVIEW_DLG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_PREVIEW_DLG message handlers

BOOL CRESEARCH_PREVIEW_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ShowWebpage() ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRESEARCH_PREVIEW_DLG::ShowWebpage()
{
	
	// 18.11.12 JMJ KOL.RED.2018.006
	m_HtmlView = new CECO_HtmlViewer;
	DWORD dwstyle = 0;
	dwstyle = WS_VISIBLE | WS_CHILD;
	m_HtmlView->Create(NULL, NULL, dwstyle ,
		CRect(0,0,300,300),this,		
		IDD_HTMLVIEW, NULL   );
	


	//CWebBrowser2 *m_pBrowser = (CWebBrowser2  *)GetDlgItem (IDC_EXPLORER1);
	COleVariant noArg;
	
	CString strPath ;
	//strPath = _T("cfg\\webresearch\\research.htm") ;
	//strPath = _T("cfg\\webresearch\\TempPage.htm") ;

	strPath.Format (_T("cfg\\webresearch\\%s.htm"),m_strFileName) ;

	CString  strCurrentDir ;

	TCHAR sDir[1000];
	GetCurrentDirectory (1000,sDir);
	strCurrentDir = sDir;

    //strPath = _T("G:\\KOLAS2UP작업\\Kolas2up\\Cfg\\webresearch\\research.htm") ;
	
	//++2008.09.04 UPDATE BY CJY {{++
	strCurrentDir.MakeUpper();
	strCurrentDir.Replace (_T("BIN"), strPath.GetBuffer(0));	
	//             이전소스
	// strCurrentDir.Replace (_T("bin"),strPath.GetBuffer (0)) ;
	//--2008.09.04 UPDATE BY CJY --}}

	//m_pBrowser->Navigate(strCurrentDir,&noArg,&noArg,&noArg,&noArg);
	
	m_HtmlView->Navigate2(strCurrentDir);
	m_HtmlView->SetParent(this);
	
}

VOID CRESEARCH_PREVIEW_DLG::SetHTMLFileName(CString strName)
{
	m_strFileName = strName ;
}

HBRUSH CRESEARCH_PREVIEW_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
