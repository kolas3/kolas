// Dlg_DMUSERViewConfig_Edit.cpp : implementation file
//

#include "stdafx.h"
#include "ESLConfigToolKit.h"
#include "Dlg_DMUSERViewConfig_Edit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_DMUSERViewConfig_Edit dialog


CDlg_DMUSERViewConfig_Edit::CDlg_DMUSERViewConfig_Edit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_DMUSERViewConfig_Edit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_DMUSERViewConfig_Edit)
	m_bUSERVIEW = FALSE;
	m_sALIAS = _T("");
	m_sINIT = _T("");
	m_sNAME = _T("");
	m_sUDF = _T("");
	m_sUSERALIAS = _T("");
	m_sUSERDESC = _T("");
	//}}AFX_DATA_INIT

	m_sOPENMODE = _T("INSERT");
}


VOID CDlg_DMUSERViewConfig_Edit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_DMUSERViewConfig_Edit)
	DDX_Control(pDX, IDC_cTYPE, m_cTYPE);
	DDX_Check(pDX, IDC_bUSERVIEW, m_bUSERVIEW);
	DDX_Text(pDX, IDC_eALIAS, m_sALIAS);
	DDX_Text(pDX, IDC_eINIT, m_sINIT);
	DDX_Text(pDX, IDC_eNAME, m_sNAME);
	DDX_Text(pDX, IDC_eUDF, m_sUDF);
	DDX_Text(pDX, IDC_eUSERALIAS, m_sUSERALIAS);
	DDX_Text(pDX, IDC_eUSERDESC, m_sUSERDESC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_DMUSERViewConfig_Edit, CDialog)
	//{{AFX_MSG_MAP(CDlg_DMUSERViewConfig_Edit)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_DMUSERViewConfig_Edit message handlers

VOID CDlg_DMUSERViewConfig_Edit::OnbOK() 
{
	UpdateData(TRUE);
	m_cTYPE.GetWindowText(m_sTYPE);

	if (m_bUSERVIEW == TRUE) m_sUSERVIEW = _T("T");
		else m_sUSERVIEW = _T("F");
	CDialog::OnOK();	
}

VOID CDlg_DMUSERViewConfig_Edit::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

BOOL CDlg_DMUSERViewConfig_Edit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cTYPE.ResetContent();
	m_cTYPE.AddString(_T("str"));
	m_cTYPE.AddString(_T("int"));
	UpdateData(FALSE);

	m_sTYPE.TrimLeft();		m_sTYPE.TrimRight();
	m_sTYPE.MakeLower();
	if (m_sTYPE == _T("int")) {
		m_cTYPE.SetCurSel(1);
	} else {
		m_cTYPE.SetCurSel(0);
	}
	m_sUSERVIEW.TrimLeft();		m_sUSERVIEW.TrimRight();
	m_sUSERVIEW.MakeUpper();
	if (m_sUSERVIEW == _T("T")) m_bUSERVIEW = TRUE;
		else m_bUSERVIEW = FALSE;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
