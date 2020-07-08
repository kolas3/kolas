// LightnessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "piclook.h"
#include "LightnessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightnessDlg dialog


CLightnessDlg::CLightnessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightnessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightnessDlg)
	m_Lightness = 0;
	//}}AFX_DATA_INIT
}


void CLightnessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightnessDlg)
	DDX_Text(pDX, ID_LIGHTNESSEDIT, m_Lightness);
	DDV_MinMaxInt(pDX, m_Lightness, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightnessDlg, CDialog)
	//{{AFX_MSG_MAP(CLightnessDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightnessDlg message handlers
