// DlgHanjaWordInputModify.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHanjaWordInputModify.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHanjaWordInputModify dialog


CDlgHanjaWordInputModify::CDlgHanjaWordInputModify(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHanjaWordInputModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHanjaWordInputModify)
	m_strInput1 = _T("");
	m_strInput2 = _T("");
	//}}AFX_DATA_INIT

	m_nMode = -1;
}


VOID CDlgHanjaWordInputModify::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHanjaWordInputModify)
	DDX_Text(pDX, IDC_eINPUT1, m_strInput1);
	DDX_Text(pDX, IDC_eINPUT2, m_strInput2);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CDlgHanjaWordInputModify, CDialog)
	//{{AFX_MSG_MAP(CDlgHanjaWordInputModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHanjaWordInputModify message handlers

BOOL CDlgHanjaWordInputModify::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (m_nMode == MODE_INPUT)
	{
		SetWindowText(_T("한자 입력"));
	}
	else if (m_nMode == MODE_MODIFY)
	{
		SetWindowText(_T("한자 수정"));
		GetDlgItem(IDC_eINPUT1)->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CDlgHanjaWordInputModify::OnOK() 
{
	EFS_BEGIN

	UpdateData();
	m_strInput1.TrimLeft();
	m_strInput1.TrimRight();

	m_strInput2.TrimLeft();
	m_strInput2.TrimRight();

	UpdateData(FALSE);

	if (m_nMode == -1) 
	{
		EndDialog(IDCANCEL);
		return;
	}

	if (m_strInput1.IsEmpty() || m_strInput2.IsEmpty())
	{
		EndDialog(IDCANCEL);
		return;
	}

	INT nCount = m_strInput2.GetLength();
	TCHAR *pBuffer = m_strInput2.GetBuffer(0);

	TCHAR ch1;
	for (INT i = 0; i < nCount; i++)
	{
		ch1 = pBuffer[i];
		if( ( ch1 < 0x4E00 ) || (ch1  > 0x9FAF ) ) 		
		{ 
			MessageBox(_T("한자가 아닌 글자가 있습니다. 다시 입력해 주십시오"), _T("Warnning!"), MB_ICONEXCLAMATION);
			return;
		}
	}

	CDialog::OnOK();

	EFS_END
}
