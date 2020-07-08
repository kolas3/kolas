/*
/--------------------------------------------------------------------
|
|      $Id: FilterDialog.cpp,v 1.1 2009/12/09 05:34:51 ¹ÚÁ¾¿î Exp $
|
\--------------------------------------------------------------------
*/

#include "stdafx.h"
#include "piclook.h"
#include "FilterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PLFilterDialog dialog


PLFilterDialog::PLFilterDialog(CWnd* pParent, PLBmp * pBmp,
                             const char * pszFilterName, bool bRadiusAllowed)
        : CDialog(PLFilterDialog::IDD, pParent),
        m_pBmp (pBmp),
        m_pszFilterName (pszFilterName),
        m_bRadiusAllowed (bRadiusAllowed)
{

    //{{AFX_DATA_INIT(PLFilterDialog)
    m_Height = 0;
    m_Width = 0;
    m_Radius = 0.0;
    //}}AFX_DATA_INIT

    m_Height = m_pBmp->GetHeight();
    m_Width = m_pBmp->GetWidth();
    m_Radius = 3;
    m_bChanging = false;
}


void PLFilterDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(PLFilterDialog)
    DDX_Control(pDX, IDC_CONSTRAINCHECK, m_ConstrainCheck);
    DDX_Control(pDX, IDC_RADIUSSTATIC, m_RadiusStatic);
    DDX_Control(pDX, IDC_RADIUSEDIT, m_RadiusEdit);
    DDX_Control(pDX, IDC_NAMESTATIC, m_NameStatic);
    DDX_Control(pDX, IDC_HEIGHTEDIT, m_HeightEdit);
    DDX_Control(pDX, IDC_WIDTHEDIT, m_WidthEdit);
    DDX_Text(pDX, IDC_HEIGHTEDIT, m_Height);
    DDV_MinMaxInt(pDX, m_Height, 0, 10000);
    DDX_Text(pDX, IDC_WIDTHEDIT, m_Width);
    DDV_MinMaxInt(pDX, m_Width, 0, 10000);
    DDX_Text(pDX, IDC_RADIUSEDIT, m_Radius);
    //}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(PLFilterDialog, CDialog)
    //{{AFX_MSG_MAP(PLFilterDialog)
    ON_EN_CHANGE(IDC_HEIGHTEDIT, OnChangeHeightedit)
    ON_EN_CHANGE(IDC_WIDTHEDIT, OnChangeWidthedit)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PLFilterDialog message handlers

BOOL PLFilterDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ConstrainCheck.SetCheck(true);
    m_NameStatic.SetWindowText (m_pszFilterName);

    m_RadiusStatic.EnableWindow (m_bRadiusAllowed);
    m_RadiusEdit.EnableWindow(m_bRadiusAllowed);
    return true;
}

void PLFilterDialog::OnOK()
{
    CDialog::OnOK();
}

void PLFilterDialog::OnChangeHeightedit()
{
    if (!m_bChanging)  // Prevent recursion
    {
        m_bChanging = true;
        if (m_ConstrainCheck.GetCheck())
        {
            char sz[256];
            m_HeightEdit.GetWindowText(sz, 255);
            double h = atoi(sz);
            sprintf (sz, "%i", (int)((h/(double)m_pBmp->GetHeight())*m_pBmp->GetWidth()+0.5));
            m_WidthEdit.SetWindowText (sz);
        }
        m_bChanging = false;
    }
}

void PLFilterDialog::OnChangeWidthedit()
{
    if (!m_bChanging)  // Prevent recursion
    {
        m_bChanging = true;
        if (m_ConstrainCheck.GetCheck())
        {
            char sz[256];
            m_WidthEdit.GetWindowText(sz, 255);
            double w = atoi(sz);
            sprintf (sz, "%i", (int)((w/(double)m_pBmp->GetWidth())*m_pBmp->GetHeight()+0.5));
            m_HeightEdit.SetWindowText (sz);
        }
        m_bChanging = false;
    }
}

int PLFilterDialog::GetWidth()
{
    return m_Width;
}

int PLFilterDialog::GetHeight()
{
    return m_Height;
}

double PLFilterDialog::GetRadius()
{
    return m_Radius;
}

/*
/--------------------------------------------------------------------
|
|      $Log: FilterDialog.cpp,v $
|      Revision 1.1  2009/12/09 05:34:51  ¹ÚÁ¾¿î
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:09  ¹Ú´ë¿ì
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:21  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:31  ¹Ú¿ì¶÷
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:30  È²Á¤¿µ
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:49  È²Á¤¿µ
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  È²Á¤¿µ
|      2006.05.23 ÃÊ±âÈ­
|
|      Revision 1.1  2006/05/18 01:31:09  È²Á¤¿µ
|      2006.05.17
|
|      Revision 1.5  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2000/03/30 21:47:41  Ulrich von Zadow
|      Added zoom-in mode, PLWinBmpEx, conditional use of DrawDIB
|      and some other nice stuff by Bernard Delmée.
|
|      Revision 1.3  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.2  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.1  1999/10/19 21:33:49  Ulrich von Zadow
|      Added filter support.
|
|
\--------------------------------------------------------------------
*/
