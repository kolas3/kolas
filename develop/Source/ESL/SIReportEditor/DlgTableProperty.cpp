// DlgTableProperty.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgTableProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTableProperty dialog


CDlgTableProperty::CDlgTableProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTableProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTableProperty)
	horz_line = FALSE;
	part_sum = FALSE;
	repeat = FALSE;
	total_sum = FALSE;
	vert_line = FALSE;
	visible = FALSE;
	m_strName = _T("");
	col_count = 0;
	row_count = 0;
	//}}AFX_DATA_INIT

	CTime time(CTime::GetCurrentTime());
	
	m_strName.Format(_T("Å×ÀÌºí_%d"), time.GetSecond());
	
	// table
	row_count = 1;
	col_count = 1;

	visible = 1;
	horz_line = 1;
	vert_line = 1;

	part_sum = 0;
	total_sum = 0;
}


VOID CDlgTableProperty::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTableProperty)
	DDX_Check(pDX, IDC_chkHORZLINE, horz_line);
	DDX_Check(pDX, IDC_chkPARTSUM, part_sum);
	DDX_Check(pDX, IDC_chkREPEAT, repeat);
	DDX_Check(pDX, IDC_chkTOTALSUM, total_sum);
	DDX_Check(pDX, IDC_chkVERTLINE, vert_line);
	DDX_Check(pDX, IDC_chkVISIBLE, visible);
	DDX_Text(pDX, IDC_eName, m_strName);
	DDX_Text(pDX, IDC_eCOL, col_count);
	DDV_MinMaxInt(pDX, col_count, 1, 100);
	DDX_Text(pDX, IDC_eROW, row_count);
	DDV_MinMaxInt(pDX, row_count, 1, 100);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgTableProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgTableProperty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTableProperty message handlers

BOOL CDlgTableProperty::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CDlgTableProperty::OnOK() 
{
EFS_BEGIN

	CDialog::OnOK();

EFS_END
}

BOOL CDlgTableProperty::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
