// UserRegIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserRegIPDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserRegIPDlg dialog


CUserRegIPDlg::CUserRegIPDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserRegIPDlg::IDD, pParent)
{	
	m_strUserIP = _T("");
	m_bReadOnly = FALSE;
	m_nValiCount = 0;
}


void CUserRegIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
}


BEGIN_MESSAGE_MAP(CUserRegIPDlg, CDialog)
	//{{AFX_MSG_MAP(CUserRegIPDlg)	
	ON_BN_CLICKED(IDC_BUTTON_ADD_2, OnButtonAdd2)
	ON_BN_CLICKED(IDC_BUTTON_ADD_1, OnButtonAdd1)
	ON_BN_CLICKED(IDC_BUTTON_ADD_3, OnButtonAdd3)	
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_EDIT_IPADDRES_01, OnFieldchangedEditIpaddres01)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_EDIT_IPADDRES_02, OnFieldchangedEditIpaddres02)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_EDIT_IPADDRES_03, OnFieldchangedEditIpaddres03)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserRegIPDlg message handlers

void CUserRegIPDlg::EnableControls(BOOL bEnable)
{
	for(int i=0; i<3; i++)
	{
		m_pArrIPControls[i]->EnableWindow(bEnable);
	}

	((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->EnableWindow(bEnable);
	((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->EnableWindow(bEnable);
}

void CUserRegIPDlg::AddControl()
{
	if(m_nValiCount == 1)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->EnableWindow(TRUE);
		
		m_pArrIPControls[1]->EnableWindow(TRUE);

		((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->SetWindowText(_T("삭제"));
	}	
	else if(m_nValiCount >= 2)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->EnableWindow(TRUE);
		
		m_pArrIPControls[2]->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->SetWindowText(_T("삭제"));
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->SetWindowText(_T("삭제"));
	}
}

void CUserRegIPDlg::RefreshControl()
{
	m_pArrIPControls[0]->EnableWindow(TRUE);
	m_pArrIPControls[1]->EnableWindow(FALSE);
	m_pArrIPControls[2]->EnableWindow(FALSE);

	((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->EnableWindow(FALSE);
	
	((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->EnableWindow(TRUE);

	if(m_nValiCount >= 1)
	{
		m_pArrIPControls[0]->EnableWindow(TRUE);
	}

	if(m_nValiCount >= 2)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->EnableWindow(TRUE);
		
		m_pArrIPControls[1]->EnableWindow(TRUE);
	}

	if(m_nValiCount >= 3)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->EnableWindow(TRUE);
		
		m_pArrIPControls[2]->EnableWindow(TRUE);
	}

	if(m_nValiCount == 1)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->SetWindowText(_T("추가"));
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->SetWindowText(_T("추가"));
	}
	else if(m_nValiCount == 2)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->SetWindowText(_T("삭제"));
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->SetWindowText(_T("추가"));
	}
	else if(m_nValiCount == 3)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->SetWindowText(_T("삭제"));
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->SetWindowText(_T("삭제"));
	}
}

void CUserRegIPDlg::DelData(int nCol)
{
	CString strOutput = _T("");

	if(m_strUserIP.Compare(_T("")) != 0)
	{
		CString strTok[3];

		int i = 0;

		for(i=0; i<3; i++)
		{
			AfxExtractSubString(strTok[i], m_strUserIP, i, ';');

			if( i+1 == nCol)
			{
				strTok[i] = _T("0.0.0.0");
			}			
		}

		for(i=0; i<3; i++)
		{			
			strOutput += strTok[i];
			strOutput += _T(";");
		}

		m_strUserIP = strOutput;
	}	
	SortData();
}

void CUserRegIPDlg::SortData()
{
	CString strOutput = _T(""), strTmp = _T("");

	m_nValiCount = 0;

	if(m_strUserIP.Compare(_T("")) != 0)
	{
		CString strTok[3];

		int i = 0;

		for(i=0; i<3; i++)
		{
			AfxExtractSubString(strTok[i], m_strUserIP, i, ';');

			if(strTok[i].Compare(_T("0.0.0.0")) != 0 && strTok[i].Compare(_T("")) != 0)
			{
				m_nValiCount++;
				strTmp.Format(_T("%s;"),strTok[i]);
				strOutput += strTmp;
			}
		}

		for(i=0; i<(3-m_nValiCount); i++)
		{
			strTmp.Format(_T("0.0.0.0;"));
			strOutput += strTmp;
		}

		m_strUserIP = strOutput;
	}
}

void CUserRegIPDlg::RefreshData()
{
	if(m_strUserIP.Compare(_T("")) != 0)
	{
		CString strTok[3];

		for(int i=0; i<3; i++)
		{
			AfxExtractSubString(strTok[i], m_strUserIP, i, ';');

			if(i == 0)
			{
				m_pArrIPControls[0]->SetWindowText(strTok[i]);
				
			}
			else if(i == 1)
			{
				m_pArrIPControls[1]->SetWindowText(strTok[i]);
			}
			else if(i == 2)
			{
				m_pArrIPControls[2]->SetWindowText(strTok[i]);
			}
		}

		UpdateData(FALSE);
	}	
}

BOOL CUserRegIPDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_pArrIPControls[0] = (CIPAddressCtrl*)GetDlgItem(IDC_EDIT_IPADDRES_01);
	m_pArrIPControls[1] = (CIPAddressCtrl*)GetDlgItem(IDC_EDIT_IPADDRES_02);
	m_pArrIPControls[2] = (CIPAddressCtrl*)GetDlgItem(IDC_EDIT_IPADDRES_03);

	m_pArrIPControls[0]->SetWindowText(_T("0.0.0.0"));
	m_pArrIPControls[1]->SetWindowText(_T("0.0.0.0"));
	m_pArrIPControls[2]->SetWindowText(_T("0.0.0.0"));

	EnableControls(FALSE);

	SortData();
	RefreshData();	
	RefreshControl();

	if(m_bReadOnly)
	{
		EnableControls(FALSE);		
		((CButton*)GetDlgItem(IDC_BUTTON_ADD_1))->EnableWindow(FALSE);
	}

	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CUserRegIPDlg::GetData()
{
	CString strTmp = _T("");

	BYTE nIP1, nIP2, nIP3, nIP4;

	m_pArrIPControls[0]->GetAddress(nIP1, nIP2, nIP3, nIP4);
	
	m_strUserIP.Format(_T("%d.%d.%d.%d;"), nIP1,nIP2,nIP3,nIP4);

	m_pArrIPControls[1]->GetAddress(nIP1, nIP2, nIP3, nIP4);

	strTmp.Format(_T("%d.%d.%d.%d;"), nIP1,nIP2,nIP3,nIP4);

	m_strUserIP += strTmp;

	m_pArrIPControls[2]->GetAddress(nIP1, nIP2, nIP3, nIP4);

	strTmp.Format(_T("%d.%d.%d.%d;"), nIP1,nIP2,nIP3,nIP4);

	m_strUserIP += strTmp;

}

void CUserRegIPDlg::OnOK() 
{
	GetData();
	SortData();
	
	CDialog::OnOK();
}

void CUserRegIPDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CUserRegIPDlg::OnButtonAdd1() 
{
	// TODO: Add your control notification handler code here
	GetData();
	SortData();

	DelData(1);	
	RefreshData();
	RefreshControl();

	if(!m_bReadOnly)
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	}	
}


void CUserRegIPDlg::OnButtonAdd2() 
{
	// TODO: Add your control notification handler code here
	GetData();
	SortData();

	CString strTmp; 
	((CButton*)GetDlgItem(IDC_BUTTON_ADD_2))->GetWindowText(strTmp);

	if(m_nValiCount >=2 || strTmp.Compare(_T("삭제"))== 0)
	{
		DelData(2);
		RefreshData();
		RefreshControl();
	}	
	else
	{
		RefreshData();
		RefreshControl();
		AddControl();
	}

	if(!m_bReadOnly)
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	}	
}


void CUserRegIPDlg::OnButtonAdd3() 
{
	// TODO: Add your control notification handler code here
	GetData();
	SortData();

	CString strTmp; 
	((CButton*)GetDlgItem(IDC_BUTTON_ADD_3))->GetWindowText(strTmp);

	if(m_nValiCount >=3 || strTmp.Compare(_T("삭제"))== 0)
	{
		DelData(3);
		RefreshData();
		RefreshControl();
	}
	else
	{
		RefreshData();
		RefreshControl();
		AddControl();
	}

	if(!m_bReadOnly)
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	}	
	
}


void CUserRegIPDlg::OnFieldchangedEditIpaddres01(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(!m_bReadOnly)
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	}
	
	*pResult = 0;
}

void CUserRegIPDlg::OnFieldchangedEditIpaddres02(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(!m_bReadOnly)
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	}
	
	*pResult = 0;
}

void CUserRegIPDlg::OnFieldchangedEditIpaddres03(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(!m_bReadOnly)
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	}
	
	*pResult = 0;
}
