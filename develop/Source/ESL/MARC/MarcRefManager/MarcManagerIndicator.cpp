// MarcManagerIndicator.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerIndicator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerIndicator dialog


CMarcManagerIndicator::CMarcManagerIndicator(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerIndicator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerIndicator)
	m_sIndicatorCode = _T("");
	m_sIndicatorDescrip = _T("");
	m_iRadioButton = -1;
	m_bDefault = FALSE;
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_iModifyFlag = 0;
}


VOID CMarcManagerIndicator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerIndicator)
	DDX_Control(pDX, IDC_rIndicator1, m_cRadioButton);
	DDX_Text(pDX, IDC_eIndicatorCode, m_sIndicatorCode);
	DDX_Text(pDX, IDC_eIndicatorDescrip, m_sIndicatorDescrip);
	DDX_Radio(pDX, IDC_rIndicator1, m_iRadioButton);
	DDX_Check(pDX, IDC_cDefault, m_bDefault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerIndicator, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerIndicator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerIndicator message handlers

BOOL CMarcManagerIndicator::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	if(m_iModifyFlag == 1)
	{
		GetDlgItem(IDC_rIndicator1)->EnableWindow(FALSE);
        GetDlgItem(IDC_rIndicator2)->ShowWindow(FALSE);
	
	}
	else if(m_iModifyFlag == 2)
	{

        GetDlgItem(IDC_rIndicator1)->ShowWindow(FALSE);
        GetDlgItem(IDC_rIndicator2)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcManagerIndicator::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	
	if(m_sIndicatorCode.IsEmpty())
	{
		AfxMessageBox(_T("코드가 입력되지 않았습니다!"));
	    return;
	}

	INT ids = 0;
	ids = CheckillegalSign(m_sIndicatorCode);
	if(ids<0) return;
	
	ids = CheckillegalSign(m_sIndicatorDescrip);
	if(ids<0) return;

	CDialog::OnOK();
}

INT CMarcManagerIndicator::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}