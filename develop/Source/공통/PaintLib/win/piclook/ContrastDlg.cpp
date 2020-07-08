// ContrastDlg.cpp : implementation file
//

#include "stdafx.h"
#include "piclook.h"
#include "ContrastDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContrastDlg dialog


CContrastDlg::CContrastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContrastDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContrastDlg)
	m_Offset = 0;
	m_Contrast = 0.0;
	//}}AFX_DATA_INIT
}


void CContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContrastDlg)
	DDX_Text(pDX, IDC_OFFSETEDIT, m_Offset);
	DDX_Text(pDX, IDC_CONTRASTEDIT, m_Contrast);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContrastDlg, CDialog)
	//{{AFX_MSG_MAP(CContrastDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


double CContrastDlg::GetContrast()
{
  return m_Contrast;
}

PLBYTE CContrastDlg::GetOffset()
{
  return m_Offset;
}
