#include "stdafx.h"
#include "resource.h"
#include "ILL_APLWEB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CILL_APLWEB::CILL_APLWEB(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
}


void CILL_APLWEB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_APLWEB)
	DDX_Control(pDX, IDC_expSEARCH, m_WebBrowser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_APLWEB, CDialog)
	//{{AFX_MSG_MAP(CILL_APLWEB)
	ON_BN_CLICKED(IDC_btnILL_APLWEB_GO, OnbtnILLAPLWEBGO)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CILL_APLWEB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	
	CFile File;
	CString strURL;
	TCHAR* pszT = NULL;
	if(TRUE == File.Open(_T("..\\cfg\\AplURL.txt"), CFile::modeRead))
	{
		pszT = new TCHAR[File.GetLength()];
		ZeroMemory(pszT, File.GetLength());
		TCHAR szBom;
		DWORD dw = File.GetLength();
		File.Read(&szBom, sizeof(TCHAR));
		File.Read(pszT, File.GetLength()-sizeof(TCHAR));
 		strURL.Format(_T("%s"), pszT);
		delete pszT;
		pszT = NULL;
		
		File.Close();
	}

	if(TRUE == strURL.IsEmpty())
	{
		strURL = _T("about:blank");
	}
	SetDlgItemText(IDC_edtILL_APLWEB_URL, strURL);
	OnbtnILLAPLWEBGO();

	return TRUE;  
	              
}

void CILL_APLWEB::OnbtnILLAPLWEBGO() 
{
	
	CString strURL;
	GetDlgItemText(IDC_edtILL_APLWEB_URL, strURL);
	m_WebBrowser.Navigate(strURL, NULL, NULL, NULL, NULL);

	CFile File;
	if(TRUE == File.Open(_T("..\\cfg\\AplURL.txt"), CFile::modeCreate|CFile::modeWrite))
	{
		TCHAR szBom = 0xFEFF;
		File.Write(&szBom, sizeof(TCHAR));
		File.Write(strURL.GetBuffer(0), strURL.GetLength()*sizeof(TCHAR));
		strURL.ReleaseBuffer();
		
		File.Close();
	}
}

void CILL_APLWEB::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
	if(NULL != m_WebBrowser.GetSafeHwnd())
	{
		m_WebBrowser.MoveWindow(9, 80, cx-18, cy -92);
		UpdateWindow();
	}
}
