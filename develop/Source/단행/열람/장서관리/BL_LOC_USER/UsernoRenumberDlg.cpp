#include "stdafx.h"
#include "UsernoRenumberDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "Kolas2up_MobileApi.h"
#include "KL_UserManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CUsernoRenumberDlg::CUsernoRenumberDlg(BL_LOC_USER_PROC* pSpfWork, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pSpfWork = pSpfWork;
	m_bIsKLMemberYN = FALSE;
	m_strUserNo = _T("");
	m_strUserNoClass = _T("");
	
	m_nMode = 0;
}
CUsernoRenumberDlg::~CUsernoRenumberDlg()
{
}

void CUsernoRenumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CUsernoRenumberDlg)
	//}}AFX_DATA_MAP		
	
}


BEGIN_MESSAGE_MAP(CUsernoRenumberDlg, CDialog)
	//{{AFX_MSG_MAP(CUsernoRenumberDlg)
	ON_BN_CLICKED(IDC_btnUR_Renumber, OnbtnRenumber)
	ON_BN_CLICKED(IDC_btnUR_Close, OnbtnClose)
	ON_BN_CLICKED(IDC_btnSelect, OnbtnSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CUsernoRenumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	INT ids = SetScreen();
	if(0>ids) return FALSE;
	
	return TRUE;  
	              
}

BOOL CUsernoRenumberDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( WM_KEYDOWN == pMsg->message ) && ( VK_RETURN == pMsg->wParam )  
		&& ( GetDlgItem(IDC_btnUR_Renumber)->GetSafeHwnd() == pMsg->hwnd) )
	{
		OnbtnRenumber();
		return TRUE;
	}

	if( ( WM_KEYDOWN == pMsg->message ) && ( VK_ESCAPE == pMsg->wParam ) )
	{
		OnbtnClose();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CUsernoRenumberDlg::OnbtnClose()
{
	OnCancel();		
}

void CUsernoRenumberDlg::OnbtnRenumber()
{
	INT ids = UserNoRenumber();
	if(0==ids) OnOK();
	else if(1010==ids) OnCancel();
}

INT CUsernoRenumberDlg::SetScreen()
{
	if(0==m_nMode)
	{
		SetWindowText(_T("대출자번호부여"));
		GetDlgItem(IDC_stcUR_UserNo)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtUR_RenumberUserNo)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnUR_Renumber)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnUR_Close)->ShowWindow(SW_SHOW);

		m_pSpfWork->GetAutoUserNo(m_strUserNo, m_strUserNoClass, _T("DM_BO_LOC_3300"), FALSE);
		GetDlgItem(IDC_edtUR_RenumberUserNo)->SetWindowText(m_strUserNo);
	}
	else
	{
		if(0>InitESL_Mgr(_T("SM_USERNO_RENUMBER")))
		{
			AfxMessageBox(_T("InitESL_Mgr Error!"));
			return -1010;
		}

		SetWindowText(_T("이용자 번호구분 선택"));
		GetDlgItem(IDC_staUR_UserNoClass)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbUR_UserNoClass)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSelect)->ShowWindow(SW_SHOW);
	}

	return 0;
}

INT CUsernoRenumberDlg::UserNoRenumber()
{
	
	if( IDNO == AfxMessageBox(_T("대출자번호를 부여(수정) 하시겠습니까?"), MB_YESNO) ) return 1010;

	
	CString strSql, strUserKey;
	GetDlgItem(IDC_edtUR_RenumberUserNo)->GetWindowText(m_strUserNo);

	
	
	m_pSpfWork->GetAutoUserNo(m_strUserNo, m_strUserNoClass, _T("DM_BO_LOC_3300"), TRUE);

	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3300"));
	strUserKey = pDM->GetCellData(_T("REC_KEY"),0);

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( this->m_pInfo->MODE );
	MobileApi.SetDM( pDM );
	MobileApi.StartLog();
	MobileApi.StartFrame();
	MobileApi.InitDBFieldData();
	MobileApi.AddDBFieldData(_T("USER_NO"), _T("STRING"), m_strUserNo, TRUE);
	MobileApi.MakeUpdateFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strUserKey);
	
	
	CString strBeforeUserNo;
	pDM->GetCellData(_T("대출자번호"), 0, strBeforeUserNo);
	pDM->SetCellData(_T("대출자번호"), m_strUserNo, 0);
	SetControlData(_T("CM_BO_LOC_3320"), _T("대출자번호"), m_strUserNo);
	
	
	if(TRUE==m_bIsKLMemberYN)
	{
		CKL_UserManagerDlg dlg(this);
		dlg.SetMode(4);
		dlg.SetBeforeUserNo(strBeforeUserNo);
		
		dlg.SetCI(m_pSpfWork->m_strCertifyCode);
		dlg.KL_MemberUpdate();

	}

	MobileApi.SendFrame();
	MobileApi.EndFrame();
	MobileApi.EndLog();

	AfxMessageBox(_T("대출자번호를 부여하였습니다."));

	return 0;
}

void CUsernoRenumberDlg::OnbtnSelect() 
{
	GetControlData( _T("CM_USERNO_RENUMBER"), _T("이용자번호구분") , m_strUserNoClass);
	OnOK();
}
