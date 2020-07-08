// DitherDialog.cpp: implementation of the CDitherDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "piclook.h"
#include "DitherDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDitherDialog::CDitherDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDitherDialog::IDD, pParent)
{
	// KOL.UDF.022 시큐어코딩 보완
	m_iDitherType = 0;
	m_iDitherPaletteType = 0;

}

void CDitherDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCropFilterDlg)
	DDX_Control(pDX, IDC_DITHERTYPE, m_DitherTypeBox);
	DDX_Control(pDX, IDC_DITHERPALETTETYPE, m_DitherPaletteTypeBox);
    //}}AFX_DATA_MAP
}

//    DDX_Text(pDX, IDC_DITHERTYPE, m_iDitherType);
//    DDX_Text(pDX, IDC_DITHERPALETTETYPE, m_iDitherPaletteType);

BEGIN_MESSAGE_MAP(CDitherDialog, CDialog)
//{{AFX_MSG_MAP(CDitherDialog)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCropFilterDlg message handlers

void CDitherDialog::OnOK()
{
	if (m_DitherPaletteTypeBox.GetCurSel() != CB_ERR)
	{
		m_iDitherPaletteType = m_DitherPaletteTypeBox.GetCurSel();
	}
	else
	{
		m_iDitherPaletteType = 0;
	}

	
	if (m_DitherTypeBox.GetCurSel() != CB_ERR)
	{
		m_iDitherType = m_DitherTypeBox.GetCurSel();
	}
	else
	{
		m_iDitherType = 0;
	}

    CDialog::OnOK();
}

