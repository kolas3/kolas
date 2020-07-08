// OtherGroupNo_Select_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OtherGroupNo_Select_Dlg.h"
#include "OtherGroupNo_List_Dlg.h"
#include "OtherGroupNo_Insert_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Select_Dlg dialog


COtherGroupNo_Select_Dlg::COtherGroupNo_Select_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COtherGroupNo_Select_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherGroupNo_Select_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_strAcqYear.Empty();
	m_strAcqCode.Empty();
	m_pGroupCollection = NULL;
	m_pGroup = NULL;	
}

COtherGroupNo_Select_Dlg::~COtherGroupNo_Select_Dlg()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
}

VOID COtherGroupNo_Select_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherGroupNo_Select_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherGroupNo_Select_Dlg, CDialog)
	//{{AFX_MSG_MAP(COtherGroupNo_Select_Dlg)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_CREATE, OnbtnOTHERGROUPNOCREATE)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_VIEW_DIVNO, OnbtnOTHERGROUPNOVIEWDIVNO)
	ON_EN_KILLFOCUS(IDC_edtOTHER_GROUP_NO_ACQ_YEAR, OnKillfocusedtOTHERGROUPNOACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtOTHER_GROUP_NO, OnKillfocusedtOTHERGROUPNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Select_Dlg message handlers

BOOL COtherGroupNo_Select_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_수서그룹번호_선택")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_수서그룹번호_선택"));
	m_pDM = FindDM(_T("DM_수서그룹번호관리"));
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("그룹정보 선택 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COtherGroupNo_Select_Dlg::OnbtnOTHERGROUPNOCREATE() 
{
	// TODO: Add your control notification handler code here
	COtherGroupNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetAcqYear(m_strAcqYear);
	dlg.SetAcqCode(m_strAcqCode);
	if(IDOK == dlg.DoModal())
	{
		CString strAddGroupNo, strAddAcqCode;
		dlg.GetInsertInfo(strAddGroupNo, strAddAcqCode);
		m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo, strAddAcqCode);
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		m_pCM->SetControlMgrData(_T("그룹번호"), m_pGroup->GetGroupInfo());
	}		
}

VOID COtherGroupNo_Select_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pGroup)
	{
		ESLAfxMessageBox(_T("입력한 그룹번호 정보가 올바르지 않습니다."));
		return ;
	}
	
	CDialog::OnOK();
}

VOID COtherGroupNo_Select_Dlg::OnbtnOTHERGROUPNOVIEWDIVNO() 
{
	// TODO: Add your control notification handler code here
	CString strGroupNo;
	COtherGroupNo_List_Dlg dlg(this);
	dlg.SetAcqCode(m_strAcqCode);
	dlg.SetAcqYear(m_pGroupCollection->GetAcqYear());
	dlg.SetGroupCollection(m_pGroupCollection);
	if(IDOK == dlg.DoModal())
		strGroupNo = dlg.GetSelInfo();
	else
		return ;
	
	if(strGroupNo.IsEmpty()) return;
	
	INT pos = strGroupNo.Find(_T(","), 0);
	if(pos >= 0)
	{
		strGroupNo = strGroupNo.Mid(0, pos);
		strGroupNo.TrimLeft();	strGroupNo.TrimRight();
	}
	
	m_pCM->SetControlMgrData(_T("그룹번호"), strGroupNo);
	m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupNo);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();

	OnKillfocusedtOTHERGROUPNO();		
}

BOOL COtherGroupNo_Select_Dlg::Init()
{
	if(m_strAcqYear.IsEmpty())
	{
		CTime t = CTime::GetCurrentTime();
		m_strAcqYear.Format(_T("%04d"), t.GetYear());
	}
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	
	if(m_strAcqCode.IsEmpty()) return FALSE;

	m_pCM->SetControlMgrData(_T("수입구분"), m_strAcqCode);
	GetDlgItem(IDC_cmbOTHER_GROUP_NO_ACQ_CODE)->EnableWindow(FALSE);

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(OTHER_ACQ_TYPE);
	m_pGroupCollection->SetAcqCode(m_strAcqCode);

	return TRUE;
}

VOID COtherGroupNo_Select_Dlg::OnKillfocusedtOTHERGROUPNOACQYEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	OnKillfocusedtOTHERGROUPNO();		
}

VOID COtherGroupNo_Select_Dlg::OnKillfocusedtOTHERGROUPNO() 
{
	// TODO: Add your control notification handler code here
	CString strGroupNo;
	m_pCM->GetControlMgrData(_T("그룹번호"), strGroupNo);
	if(strGroupNo.IsEmpty()) return;
	
	INT pos = strGroupNo.Find(_T(","), 0);
	if(pos >= 0)
	{
		strGroupNo = strGroupNo.Mid(0, pos);
		strGroupNo.TrimLeft();	strGroupNo.TrimRight();
	}
	
	if(m_pGroup != NULL && m_pGroup->GetGroupInfo() == strGroupNo) return ;
	
	INT ids = m_pGroupCollection->CheckValidate(strGroupNo, m_strAcqYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("입력하신 그룹번호가 올바르지 않습니다."));
		m_pCM->SetControlMgrData(_T("그룹번호"), _T(""));
		return;
	}
	
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupNo);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();	
}

BOOL COtherGroupNo_Select_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_CHAR)
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

CGroupInfo * COtherGroupNo_Select_Dlg::GetSelGroup()
{
	return m_pGroup;
}

HBRUSH COtherGroupNo_Select_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
