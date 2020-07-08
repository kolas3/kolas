// DlgRptFileManageModify.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgRptFileManageModify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRptFileManageModify dialog


CDlgRptFileManageModify::CDlgRptFileManageModify(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRptFileManageModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRptFileManageModify)
	m_sTITLE = _T("");
	m_sDESCRIPTION = _T("");
	//}}AFX_DATA_INIT

	m_strRptGroup = _T("");
}


VOID CDlgRptFileManageModify::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRptFileManageModify)
	DDX_Control(pDX, IDC_cSTEP3, m_cSTEP3);
	DDX_Control(pDX, IDC_cSTEP2, m_cSTEP2);
	DDX_Control(pDX, IDC_cSTEP1, m_cSTEP1);
	DDX_Text(pDX, IDC_eTITLE, m_sTITLE);
	DDX_Text(pDX, IDC_eDESCRIPTION, m_sDESCRIPTION);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgRptFileManageModify, CDialog)
	//{{AFX_MSG_MAP(CDlgRptFileManageModify)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRptFileManageModify message handlers

VOID CDlgRptFileManageModify::OnbSAVE() 
{
EFS_BEGIN

	UpdateData(TRUE);

	CString strSTEP1, strSTEP2, strSTEP3;
	m_cSTEP1.GetWindowText(strSTEP1);
	m_cSTEP2.GetWindowText(strSTEP2);
	m_cSTEP3.GetWindowText(strSTEP3);
	strSTEP1.TrimLeft();		strSTEP1.TrimRight();		strSTEP1.MakeUpper();
	strSTEP2.TrimLeft();		strSTEP2.TrimRight();		strSTEP2.MakeUpper();
	strSTEP3.TrimLeft();		strSTEP3.TrimRight();		strSTEP3.MakeUpper();
	m_strRptGroup.Format(_T("%s,%s,%s"), strSTEP1, strSTEP2, strSTEP3);

	CDialog::OnOK();

EFS_END
}

VOID CDlgRptFileManageModify::OnbCANCEL() 
{
EFS_BEGIN

	CDialog::OnCancel();

EFS_END
}

INT CDlgRptFileManageModify::InitBusinessStep(VOID)
{
EFS_BEGIN

	INT nSTEP1=0, nSTEP2=0, nSTEP3=0;
	CString strSTEP1[32], strSTEP2[32], strSTEP3[32];

	// (START) - 테스트 코드 - 코드관리로 바꿔야 함
	strSTEP1[nSTEP1++] = _T("LIGHT");
	strSTEP1[nSTEP1++] = _T("UP");

	strSTEP2[nSTEP2++] = _T("단행");
	strSTEP2[nSTEP2++] = _T("연속");
	strSTEP2[nSTEP2++] = _T("기타");

	strSTEP3[nSTEP3++] = _T("수서");
	strSTEP3[nSTEP3++] = _T("정리");
	strSTEP3[nSTEP3++] = _T("열람");
	strSTEP3[nSTEP3++] = _T("기타");
	// ( END ) - 테스트 코드 - 코드관리로 바꿔야 함

	CString strTmpSTEP1, strTmpSTEP2, strTmpSTEP3;
	INT pos1=0, pos2=0;
	if (m_strRptGroup.GetLength() > 0) {
		pos2 = m_strRptGroup.Find(_T(","), pos1);
		strTmpSTEP1 = m_strRptGroup.Mid(pos1, pos2-pos1);
		pos1 = pos2+1;

		pos2 = m_strRptGroup.Find(_T(","), pos1);
		strTmpSTEP2 = m_strRptGroup.Mid(pos1, pos2-pos1);
		pos1 = pos2+1;

		pos2 = m_strRptGroup.Find(_T(","), pos1);
		if (pos2 == -1) pos2 = m_strRptGroup.GetLength();
		strTmpSTEP3 = m_strRptGroup.Mid(pos1, pos2-pos1);
		pos1 = pos2+1;
	}

	InitBusinessStepComboBox(&m_cSTEP1, strSTEP1, nSTEP1, strTmpSTEP1);
	InitBusinessStepComboBox(&m_cSTEP2, strSTEP2, nSTEP2, strTmpSTEP2);
	InitBusinessStepComboBox(&m_cSTEP3, strSTEP3, nSTEP3, strTmpSTEP3);

	return 0;

EFS_END
return -1;

}

INT CDlgRptFileManageModify::InitBusinessStepComboBox(CComboBox *pCMB, CString str[], INT nstr, CString strSelect)
{
EFS_BEGIN

	if (!pCMB) return -1;

	INT nSelectedRow = -1;
	for (INT i = 0; i < nstr; i++) {
		pCMB->AddString(str[i]);
		if (str[i] == strSelect) nSelectedRow = i;
	}
	if (nSelectedRow >= 0) {
		pCMB->SetCurSel(nSelectedRow);
	}	
	return 0;

EFS_END
return -1;

}

BOOL CDlgRptFileManageModify::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	InitBusinessStep();
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}
