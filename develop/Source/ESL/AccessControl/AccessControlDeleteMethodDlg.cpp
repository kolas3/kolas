// AccessControlDeleteMethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlDeleteMethodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDeleteMethodDlg dialog


CAccessControlDeleteMethodDlg::CAccessControlDeleteMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlDeleteMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlDeleteMethodDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;
}


VOID CAccessControlDeleteMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlDeleteMethodDlg)
	DDX_Control(pDX, IDC_lMETHOD, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlDeleteMethodDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlDeleteMethodDlg)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_lMETHOD, OnDblclklMETHOD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDeleteMethodDlg message handlers

VOID CAccessControlDeleteMethodDlg::OnbDELETE() 
{
	OnDblclklMETHOD();
}

VOID CAccessControlDeleteMethodDlg::OnbCLOSE() 
{
	CDialog::OnOK();	
}

VOID CAccessControlDeleteMethodDlg::ShowList(VOID)
{
	m_pDM->RestructDataManager(_T("SELECT PK, METHOD_NAME FROM ESL_MENU_METHOD_TBL WHERE FLAG='M'"));
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) {
		AfxMessageBox(_T("Method가 없습니다."));
		CDialog::OnOK();
		return;
	}
	m_list.ResetContent();
	for (INT i = 0; i < nRow; i++) {
		m_list.AddString(m_pDM->GetCellData(i, 1));
	}
	return;
}

BOOL CAccessControlDeleteMethodDlg::OnInitDialog() 
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

VOID CAccessControlDeleteMethodDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pDM) {
		 delete m_pDM;
		 m_pDM = NULL;
	}	
}

VOID CAccessControlDeleteMethodDlg::OnDblclklMETHOD() 
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
	query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='M' AND USER_ID='%s';"), name);
	m_pDM->AddFrame(query);

	query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='M' AND USER_ID='%s';"), name);
	m_pDM->AddFrame(query);

	query.Format(_T("DELETE FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' AND METHOD_NAME='%s';"), name);
	m_pDM->AddFrame(query);

	query.Format(_T("UPDATE ESL_MENU_METHOD_TBL SET USER_METHOD_NAME='' WHERE USER_METHOD_NAME='%s';"), name);
	m_pDM->AddFrame(query);

	ids = m_pDM->SendFrame();
	if (ids == 0) AfxMessageBox(_T("삭제 되었습니다."));
		else AfxMessageBox(_T("삭제중 에러가 발생하였습니다."));
	ids = m_pDM->EndFrame();

	ShowList();	
}
