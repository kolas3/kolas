// AccessControlDeleteUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlDeleteUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDeleteUserDlg dialog


CAccessControlDeleteUserDlg::CAccessControlDeleteUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlDeleteUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlDeleteUserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;
}


VOID CAccessControlDeleteUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlDeleteUserDlg)
	DDX_Control(pDX, IDC_lUSER, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlDeleteUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlDeleteUserDlg)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_lUSER, OnDblclklUSER)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDeleteUserDlg message handlers

VOID CAccessControlDeleteUserDlg::OnbCLOSE() 
{
	CDialog::OnOK();	
}

VOID CAccessControlDeleteUserDlg::OnbDELETE() 
{
	OnDblclklUSER();
}

VOID CAccessControlDeleteUserDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pDM) {
		 delete m_pDM;
		 m_pDM = NULL;
	}
}

BOOL CAccessControlDeleteUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	NewButtonList.Add(IDC_bDELETE, this, IDB_DELETE_U, IDB_DELETE_D, IDB_DELETE_F);
	NewButtonList.Add(IDC_bCLOSE, this, IDB_CLOSE_U, IDB_CLOSE_D, IDB_CLOSE_F);

	ShowList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlDeleteUserDlg::ShowList(VOID)
{
	m_pDM->RestructDataManager(_T("SELECT PK, USER_ID FROM ESL_MENU_METHOD_TBL WHERE FLAG='U'"));
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) {
		AfxMessageBox(_T("사용자가 없습니다."));
		CDialog::OnOK();
		return;
	}
	m_list.ResetContent();
	for (INT i = 0; i < nRow; i++) {
		m_list.AddString(m_pDM->GetCellData(i, 1));
	}
	return;
}

VOID CAccessControlDeleteUserDlg::OnDblclklUSER() 
{
	UpdateData(true);
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) {
		return;
	}

	INT selectrow = m_list.GetCurSel();
	CString pk, name;
	INT ids;
	if (m_pDM->GetCellData(selectrow, 0, pk) < 0) {
		AfxMessageBox(_T("손상된 데이타입니다."));
		return;
	}
	if (pk.GetLength() == 0 ) {
		AfxMessageBox(_T("손상된 데이타입니다."));
		return;
	}
	ids = m_pDM->GetCellData(selectrow, 1, name);
	if (AfxMessageBox(_T("정말 지우시겠습니까?"), MB_YESNO) != IDYES) return;

	CString query;
	ids = m_pDM->StartFrame();
	query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='U' AND USER_ID='%s';"), name);
	m_pDM->AddFrame(query);

	query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='U' AND USER_ID='%s';"), name);
	m_pDM->AddFrame(query);

	query.Format(_T("DELETE FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s';"), name);
	m_pDM->AddFrame(query);

	ids = m_pDM->SendFrame();
	if (ids == 0) AfxMessageBox(_T("삭제 되었습니다."));
		else AfxMessageBox(_T("삭제중 에러가 발생하였습니다."));
	ids = m_pDM->EndFrame();

	ShowList();
}
