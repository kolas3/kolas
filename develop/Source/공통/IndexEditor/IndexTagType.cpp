// IndexTagType.cpp : implementation file
//

#include "stdafx.h"
#include "IndexTagType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexTagType dialog


CIndexTagType::CIndexTagType(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexTagType)
	m_nTagType = -1;
	m_bTagNoType = FALSE;
	//}}AFX_DATA_INIT

	m_strTagType = _T("");
}


VOID CIndexTagType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexTagType)
	DDX_Radio(pDX, IDC_radTagTypeA, m_nTagType);
	DDX_Check(pDX, IDC_chkTagNoType, m_bTagNoType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexTagType, CDialog)
	//{{AFX_MSG_MAP(CIndexTagType)
	ON_BN_CLICKED(IDC_chkTagNoType, OnchkTagNoType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexTagType message handlers

BOOL CIndexTagType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(m_strTagType.IsEmpty()) 
	{
		m_bTagNoType = TRUE;
		EnableRadioCtrl(FALSE);
	}
	else
	{
		switch(m_strTagType.GetAt(0))
		{	
		case 'A' :
			m_nTagType = 0;
			break;
		case 'B' :
			m_nTagType = 1;
			break;
		case 'C' :
			m_nTagType = 2;
			break;
		case 'D' :
			m_nTagType = 3;
			break;
		case 'E' :
			m_nTagType = 4;
			break;
		case 'F' :
			m_nTagType = 5;
			break;
		case 'G' :
			m_nTagType = 6;
			break;
		default :
			break;
		}
	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexTagType::EnableRadioCtrl(BOOL bEnable)
{

	if(bEnable)
	{
		GetDlgItem(IDC_radTagTypeA)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagTypeB)->EnableWindow(TRUE);	
		GetDlgItem(IDC_radTagTypeC)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagTypeD)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagTypeE)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagTypeF)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagTypeG)->EnableWindow(TRUE);
	}
	else 
	{

		GetDlgItem(IDC_radTagTypeA)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagTypeB)->EnableWindow(FALSE);	
		GetDlgItem(IDC_radTagTypeC)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagTypeD)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagTypeE)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagTypeF)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagTypeG)->EnableWindow(FALSE);
	}


}

VOID CIndexTagType::OnchkTagNoType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bTagNoType)
	{
		EnableRadioCtrl(FALSE);
	}
	else
	{
		EnableRadioCtrl(TRUE);
	}

}

VOID CIndexTagType::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);
	
	if(m_bTagNoType)
	{
		m_strTagType = _T("");
	}
	else
	{	
		if(m_nTagType<0) 
		{
			AfxMessageBox(_T("태그 유형이 설정되지 않았습니다!"));
			return;
		}
		
		switch(m_nTagType)
		{	
		case 0 :
			m_strTagType = _T("A");
			break;
		case 1 :
			m_strTagType = _T("B");
			break;
		case 2 :
			m_strTagType = _T("C");
			break;
		case 3 :
			m_strTagType = _T("D");
			break;
		case 4 :
			m_strTagType = _T("E");
			break;
		case 5 :
			m_strTagType = _T("F");
			break;
		case 6 :
			m_strTagType = _T("G");
			break;
		default :
			break;
		}
		
	}


	CDialog::OnOK();
}
