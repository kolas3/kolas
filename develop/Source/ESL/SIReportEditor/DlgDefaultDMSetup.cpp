// DlgDefaultDMSetup.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgDefaultDMSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultDMSetup dialog


CDlgDefaultDMSetup::CDlgDefaultDMSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefaultDMSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDefaultDMSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pArrayDMAlias = NULL;
}


VOID CDlgDefaultDMSetup::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDefaultDMSetup)
	DDX_Control(pDX, IDC_cDM, m_cDM);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgDefaultDMSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgDefaultDMSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultDMSetup message handlers

BOOL CDlgDefaultDMSetup::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (m_pArrayDMAlias == NULL)
	{
		AfxMessageBox(_T("DM Alias 정보가 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	INT nCount = m_pArrayDMAlias->GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		m_cDM.AddString(m_pArrayDMAlias->GetAt(i));
	}

	UpdateData(FALSE);
	
	
	// Set Default String
	if (!m_strDefaultDM.IsEmpty())
	{
		CString strItem;
		INT nCount = m_cDM.GetCount();
		for (INT i = 0; i < nCount; i++)
		{
			m_cDM.GetLBText(i, strItem);
			if (strItem == m_strDefaultDM)
			{
				m_cDM.SetCurSel(i);
				break;
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}


VOID CDlgDefaultDMSetup::OnOK() 
{
EFS_BEGIN

	UpdateData();
	m_cDM.GetLBText(m_cDM.GetCurSel(), m_strDefaultDM);
	CDialog::OnOK();

EFS_END
}