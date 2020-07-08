// IntensityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "piclook.h"
#include "IntensityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntensityDlg dialog


CIntensityDlg::CIntensityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntensityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntensityDlg)
	m_Exponent = 0.0;
	m_Intensity = 0.0;
	m_Offset = 0;
	//}}AFX_DATA_INIT
}


void CIntensityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntensityDlg)
	DDX_Text(pDX, IDC_EXPONENTEDIT, m_Exponent);
	DDX_Text(pDX, IDC_INTENSITYEDIT, m_Intensity);
	DDX_Text(pDX, IDC_OFFSETEDIT, m_Offset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntensityDlg, CDialog)
	//{{AFX_MSG_MAP(CIntensityDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntensityDlg message handlers
