// PermisionCFGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PermisionCFGDlg.h"
#include "UserCfgDlg.h"
#include "MNG_USER_MANAGER_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPermisionCFGDlg dialog


CPermisionCFGDlg::CPermisionCFGDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	m_pParent = pParent;
	m_strCurrentUserDefineType = _T("");
	//{{AFX_DATA_INIT(CPermisionCFGDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPermisionCFGDlg::~CPermisionCFGDlg()
{
}

void CPermisionCFGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPermisionCFGDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPermisionCFGDlg, CDialog)
//{{AFX_MSG_MAP(CPermisionCFGDlg)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_BTN_CONFIG, OnBtnConfig)
ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPermisionCFGDlg message handlers

BOOL CPermisionCFGDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	
	if( InitESL_Mgr( _T("SM_USER_PERMISION") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SM_USER_PERMISION") );
		return false;
	}
	pDM= FindDM( _T("DM_USER_PERMISION_INFO"));
	pDM2 = FindDM(_T("DM_MN_USER_TBL"));
	pDM->RefreshDataManager(_T(""));
	InitPermisionInfo();
	InitUserSideInfo();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPermisionCFGDlg::InitUserSideInfo(VOID)
{
	HWND handle=::FindWindow(NULL,_T("사용자 수정"));
	if(NULL!=handle)
	{
		CButton* bt=NULL;
		CMNG_USER_MANAGER_03* wnd = (CMNG_USER_MANAGER_03*)FromHandle(handle);
		CString strID;
		wnd->GetDlgItemText(IDC_EDIT1,strID);
		CString strOption;
		strOption.Format(_T("ID='%s'"),strID);
		pDM2->RefreshDataManager(strOption);
		
		SetDlgItemText(IDC_USERID,strID);

		if(pDM2->GetCellData(_T("AU_A"),0)==_T("T"))
		{
			bt = (CButton*)GetDlgItem(IDC_CHK2); bt->SetCheck(TRUE);
		}

		if(pDM2->GetCellData(_T("AU_B"),0)==_T("T"))
		{
			bt = (CButton*)GetDlgItem(IDC_CHK3); bt->SetCheck(TRUE);
		}

		if(pDM2->GetCellData(_T("AU_C"),0)==_T("T"))
		{
			bt = (CButton*)GetDlgItem(IDC_CHK4); bt->SetCheck(TRUE);
		}

		if(pDM2->GetCellData(_T("AU_D"),0)==_T("T"))
		{
			bt = (CButton*)GetDlgItem(IDC_CHK5); bt->SetCheck(TRUE);
		}

		if(pDM2->GetCellData(_T("AU_TOTAL"),0)==_T("T"))
		{
			bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(TRUE);
		}

		if( (pDM2->GetCellData(_T("USER_AU_A"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_B"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_C"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_D"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_E"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_F"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_G"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_H"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_I"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_J"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_K"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_L"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_M"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_N"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_O"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_P"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_Q"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_R"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_S"),0)==_T("T")) ||
			(pDM2->GetCellData(_T("USER_AU_T"),0)==_T("T"))
			)
		{
			CString strUserAuthorityType;
			if(pDM2->GetCellData(_T("USER_AU_A"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_A");
			else if(pDM2->GetCellData(_T("USER_AU_B"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_B");
			else if(pDM2->GetCellData(_T("USER_AU_C"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_C");
			else if(pDM2->GetCellData(_T("USER_AU_D"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_D");
			else if(pDM2->GetCellData(_T("USER_AU_E"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_E");
			else if(pDM2->GetCellData(_T("USER_AU_F"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_F");
			else if(pDM2->GetCellData(_T("USER_AU_G"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_G");
			else if(pDM2->GetCellData(_T("USER_AU_H"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_H");
			else if(pDM2->GetCellData(_T("USER_AU_I"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_I");
			else if(pDM2->GetCellData(_T("USER_AU_J"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_J");
			else if(pDM2->GetCellData(_T("USER_AU_K"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_K");
			else if(pDM2->GetCellData(_T("USER_AU_L"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_L");
			else if(pDM2->GetCellData(_T("USER_AU_M"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_M");
			else if(pDM2->GetCellData(_T("USER_AU_N"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_N");
			else if(pDM2->GetCellData(_T("USER_AU_O"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_O");
			else if(pDM2->GetCellData(_T("USER_AU_P"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_P");
			else if(pDM2->GetCellData(_T("USER_AU_Q"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_Q");
			else if(pDM2->GetCellData(_T("USER_AU_R"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_R");
			else if(pDM2->GetCellData(_T("USER_AU_S"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_S");
			else if(pDM2->GetCellData(_T("USER_AU_T"),0)==_T("T")) strUserAuthorityType = _T("USER_AU_T");
			

			m_strCurrentUserDefineType = strUserAuthorityType;
			strOption.Format(_T("AUTHORITY='%s'"),strUserAuthorityType);
			pDM->RefreshDataManager(strOption);

			CString str;
			
			if(strUserAuthorityType==_T("USER_AU_A")) str=_T("1.");
			else if(strUserAuthorityType==_T("USER_AU_B")) str=_T("2.");
			else if(strUserAuthorityType==_T("USER_AU_C")) str=_T("3.");
			else if(strUserAuthorityType==_T("USER_AU_D")) str=_T("4.");
			else if(strUserAuthorityType==_T("USER_AU_E")) str=_T("5.");
			else if(strUserAuthorityType==_T("USER_AU_F")) str=_T("6.");
			else if(strUserAuthorityType==_T("USER_AU_G")) str=_T("7.");
			else if(strUserAuthorityType==_T("USER_AU_H")) str=_T("8.");
			else if(strUserAuthorityType==_T("USER_AU_I")) str=_T("9.");
			else if(strUserAuthorityType==_T("USER_AU_J")) str=_T("10.");
			else if(strUserAuthorityType==_T("USER_AU_K")) str=_T("11.");
			else if(strUserAuthorityType==_T("USER_AU_L")) str=_T("12.");
			else if(strUserAuthorityType==_T("USER_AU_M")) str=_T("13.");
			else if(strUserAuthorityType==_T("USER_AU_N")) str=_T("14.");
			else if(strUserAuthorityType==_T("USER_AU_O")) str=_T("15.");
			else if(strUserAuthorityType==_T("USER_AU_P")) str=_T("16.");
			else if(strUserAuthorityType==_T("USER_AU_Q")) str=_T("17.");
			else if(strUserAuthorityType==_T("USER_AU_R")) str=_T("18.");
			else if(strUserAuthorityType==_T("USER_AU_S")) str=_T("19.");
			else if(strUserAuthorityType==_T("USER_AU_T")) str=_T("20.");			

			str+=pDM->GetCellData(0,1);
			SetDlgItemText(IDC_AU_USER_NAME,str);
			SetDlgItemText(IDC_AU_USER_DESC,pDM->GetCellData(0,2));
			bt = (CButton*)GetDlgItem(IDC_CHK_USER); bt->SetCheck(TRUE);
		}
		
	}
	else 
	{
		SetDlgItemText(IDC_USERID,_T(""));
		SetDlgItemText(IDC_DESC1,_T(""));
		CButton* bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(TRUE);
	}
}

VOID CPermisionCFGDlg::InitPermisionInfo(VOID)
{
	CString strAuthor;
	for(INT i=0;i<pDM->GetRowCount();i++)
	{
		strAuthor = pDM->GetCellData(i,0);
		if(strAuthor == _T("AU_TOTAL"))
		{
			SetDlgItemText(IDC_AU_TOTAL_NAME,pDM->GetCellData(i,1));
			SetDlgItemText(IDC_AU_TOTAL_DESC,pDM->GetCellData(i,2));
		}
		else if(strAuthor==_T("AU_A"))
		{
			SetDlgItemText(IDC_AU_A_NAME,pDM->GetCellData(i,1));
			SetDlgItemText(IDC_AU_A_DESC,pDM->GetCellData(i,2));
		}
		else if(strAuthor==_T("AU_B"))
		{
			SetDlgItemText(IDC_AU_B_NAME,pDM->GetCellData(i,1));
			SetDlgItemText(IDC_AU_B_DESC,pDM->GetCellData(i,2));
		}
		else if(strAuthor==_T("AU_C"))
		{
			SetDlgItemText(IDC_AU_C_NAME,pDM->GetCellData(i,1));
			SetDlgItemText(IDC_AU_C_DESC,pDM->GetCellData(i,2));
		}
		else if(strAuthor==_T("AU_D"))
		{
			SetDlgItemText(IDC_AU_D_NAME,pDM->GetCellData(i,1));
			SetDlgItemText(IDC_AU_D_DESC,pDM->GetCellData(i,2));
		}
	}
}


HBRUSH CPermisionCFGDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

LRESULT CPermisionCFGDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CButton* bt = NULL;
	switch(message)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CHK_USER:			
			bt = (CButton*)GetDlgItem(IDC_CHK_USER);
			if(bt->GetCheck())
			{
				if(m_strCurrentUserDefineType.IsEmpty())
				{
					AfxMessageBox(_T("사용자 정의 항목을 선택해 주십시오."));
					bt->SetCheck(FALSE);
					break;
				}
				bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(FALSE);								
			}
			else
			{

			}
			break;
		case IDC_CHK_MASTER:
			bt = (CButton*)GetDlgItem(IDC_CHK_MASTER);
			if(bt->GetCheck())
			{
				bt = (CButton*)GetDlgItem(IDC_CHK_USER); bt->SetCheck(FALSE);
				bt = (CButton*)GetDlgItem(IDC_CHK2); bt->SetCheck(FALSE);
				bt = (CButton*)GetDlgItem(IDC_CHK3); bt->SetCheck(FALSE);
				bt = (CButton*)GetDlgItem(IDC_CHK4); bt->SetCheck(FALSE);
				bt = (CButton*)GetDlgItem(IDC_CHK5); bt->SetCheck(FALSE);
			}

			break;
		case IDC_CHK2:
			bt = (CButton*)GetDlgItem(IDC_CHK2);
			if(bt->GetCheck())
			{
				bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(FALSE);
			}
			else
			{

			}
			break;
		case IDC_CHK3:
			bt = (CButton*)GetDlgItem(IDC_CHK3);
			if(bt->GetCheck())
			{
				bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(FALSE);
			}
			else
			{
				
			}
			break;
		case IDC_CHK4:
			bt = (CButton*)GetDlgItem(IDC_CHK4);
			if(bt->GetCheck())
			{
				bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(FALSE);
			}
			else
			{

			}
			break;
		case IDC_CHK5:
			bt = (CButton*)GetDlgItem(IDC_CHK5);
			if(bt->GetCheck())
			{
				bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(FALSE);
			}
			else
			{
				
			}
			break;
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPermisionCFGDlg::OnBtnConfig() 
{
	CString strAlias;

	( (CStatic*) GetDlgItem(IDC_AU_USER_NAME) )->GetWindowText(strAlias);
	CUserCfgDlg dlg(this->m_pParent);	
	dlg.m_strAlias = strAlias;
	if(IDOK==dlg.DoModal())
	{
		
		if(dlg.selectItem.IsEmpty()) return;
		else this->m_strCurrentUserDefineType=dlg.selectItem;
		
		
		CString strOption;
		strOption.Format(_T("AUTHORITY='%s'"),m_strCurrentUserDefineType);
		pDM->RefreshDataManager(strOption);
		
		CString str;
		CString str2 = pDM->GetCellData(0,2);
		
		if(m_strCurrentUserDefineType==_T("USER_AU_A")) str=_T("1.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_B")) str=_T("2.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_C")) str=_T("3.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_D")) str=_T("4.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_E")) str=_T("5.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_F")) str=_T("6.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_G")) str=_T("7.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_H")) str=_T("8.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_I")) str=_T("9.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_J")) str=_T("10.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_K")) str=_T("11.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_L")) str=_T("12.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_M")) str=_T("13.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_N")) str=_T("14.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_O")) str=_T("15.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_P")) str=_T("16.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_Q")) str=_T("17.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_R")) str=_T("18.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_S")) str=_T("19.");
		else if(m_strCurrentUserDefineType==_T("USER_AU_T")) str=_T("20.");		

		str += pDM->GetCellData(0,1);

		SetDlgItemText(IDC_AU_USER_NAME,str);
		SetDlgItemText(IDC_AU_USER_DESC,str2);
		
		CButton* bt = (CButton*)GetDlgItem(IDC_CHK_USER); bt->SetCheck(TRUE);
		bt = (CButton*)GetDlgItem(IDC_CHK_MASTER); bt->SetCheck(FALSE);
	}
}

void CPermisionCFGDlg::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	if( ((CButton*)GetDlgItem(IDC_CHK_MASTER))->GetCheck())
	{
		m_strAuthorityType = _T("AU_TOTAL@");
	}
	else{
	if( ((CButton*)GetDlgItem(IDC_CHK2))->GetCheck())
	{
		m_strAuthorityType += _T("AU_A@");
	}
	if( ((CButton*)GetDlgItem(IDC_CHK3))->GetCheck())
	{
		m_strAuthorityType += _T("AU_B@");
	}
	if( ((CButton*)GetDlgItem(IDC_CHK4))->GetCheck())
	{
		m_strAuthorityType += _T("AU_C@");
	}
	if( ((CButton*)GetDlgItem(IDC_CHK5))->GetCheck())
	{
		m_strAuthorityType += _T("AU_D@");
	}
	if( ((CButton*)GetDlgItem(IDC_CHK_USER))->GetCheck())
	{
		m_strAuthorityType+=m_strCurrentUserDefineType;	
		m_strAuthorityType += _T("@");
	}
	}
	if(			!(((CButton*)GetDlgItem(IDC_CHK_MASTER))->GetCheck()) &&
				!(((CButton*)GetDlgItem(IDC_CHK_USER))->GetCheck()) &&
				!(((CButton*)GetDlgItem(IDC_CHK5))->GetCheck()) &&
				!(((CButton*)GetDlgItem(IDC_CHK3))->GetCheck()) &&
				!(((CButton*)GetDlgItem(IDC_CHK4))->GetCheck()) &&
				!(((CButton*)GetDlgItem(IDC_CHK2))->GetCheck())
				)
	{
		AfxMessageBox(_T("사용할 권한을 선택해 주십시오."));
		return;
	}

	CDialog::OnOK();
}
