// CropFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "piclook.h"
#include "CropFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCropFilterDlg dialog


CCropFilterDlg::CCropFilterDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CCropFilterDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCropFilterDlg)
    m_XMax = 0;
    m_XMin = 0;
    m_YMax = 0;
    m_YMin = 0;
    //}}AFX_DATA_INIT
}


void CCropFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCropFilterDlg)
    DDX_Text(pDX, IDC_XMAXEDIT, m_XMax);
    DDX_Text(pDX, IDC_XMINEDIT, m_XMin);
    DDX_Text(pDX, IDC_YMAXEDIT, m_YMax);
    DDX_Text(pDX, IDC_YMINEDIT, m_YMin);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCropFilterDlg, CDialog)
//{{AFX_MSG_MAP(CCropFilterDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCropFilterDlg message handlers

void CCropFilterDlg::OnOK()
{
    CDialog::OnOK();
}
