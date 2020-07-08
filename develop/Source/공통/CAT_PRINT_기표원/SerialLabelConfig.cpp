// SerialLabelConfig.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "SerialLabelConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerialLabelConfig dialog


CSerialLabelConfig::CSerialLabelConfig(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialLabelConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CSerialLabelConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialLabelConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSerialLabelConfig, CDialog)
	//{{AFX_MSG_MAP(CSerialLabelConfig)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialLabelConfig message handlers
BOOL CSerialLabelConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*
	CString sUnit;
	CString m_sUnit;
	
	CComboBox * pRegCode;	
	CString sRegCode;
	pRegCode=(CComboBox *)GetDlgItem(IDC_CobUnit);

	pRegCode->AddString(_T("픽셀(pixel)"));
	pRegCode->AddString(_T("밀리미터(mm)"));
	if ( sUnit == _T("밀리미터(mm)") ) {
		pRegCode->SetCurSel(0);
		m_sUnit = _T("밀리미터(mm)");
	}
	else {
		pRegCode->SetCurSel(1);
		m_sUnit = _T("픽셀(pixel)");
	}
*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}