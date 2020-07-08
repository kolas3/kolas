// AccessControlChangeMethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlChangeMethodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlChangeMethodDlg dialog


CAccessControlChangeMethodDlg::CAccessControlChangeMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlChangeMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlChangeMethodDlg)
	m_sUSER_ID = _T("");
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;
}


VOID CAccessControlChangeMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlChangeMethodDlg)
	DDX_Control(pDX, IDC_cMETHOD, m_cMethod);
	DDX_Text(pDX, IDC_eNAME, m_sUSER_ID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlChangeMethodDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlChangeMethodDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlChangeMethodDlg message handlers

VOID CAccessControlChangeMethodDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pDM) {
		 delete m_pDM;
		 m_pDM = NULL;
	}		
}

BOOL CAccessControlChangeMethodDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	ShowList();

	NewButtonList.Add(IDC_bOK, this, IDB_INSERT_U, IDB_INSERT_D, IDB_INSERT_F);
	NewButtonList.Add(IDC_bCANCEL, this, IDB_CANCEL_U, IDB_CANCEL_D, IDB_CANCEL_F);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlChangeMethodDlg::ShowList(VOID)
{
	INT ids;
	CString query;
	query.Format(_T("SELECT USER_METHOD_NAME FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s'"), m_sUSER_ID);
	ids = m_pDM->GetOneValueQuery(query, USER_METHOD_NAME);
	USER_METHOD_NAME.TrimLeft();			USER_METHOD_NAME.TrimRight();
	m_pDM->RestructDataManager(_T("SELECT PK, METHOD_NAME FROM ESL_MENU_METHOD_TBL WHERE FLAG='M'"));
	INT nRow = m_pDM->GetRowCount();
	m_cMethod.ResetContent();
	m_cMethod.AddString(_T(" "));
	INT selectedrow;
	if (USER_METHOD_NAME.GetLength() == 0) selectedrow = 0;
	for (INT i = 0; i < nRow; i++) {
		m_cMethod.AddString(m_pDM->GetCellData(i, 1));
		if (m_pDM->GetCellData(i, 1) == USER_METHOD_NAME) selectedrow = i+1;
	}
	m_cMethod.SetCurSel(selectedrow);
	UpdateData(false);
	return;
}

VOID CAccessControlChangeMethodDlg::OnbOK() 
{
	CString query;
	INT ids, ids1, ids2, ids3;
	UpdateData(true);
	CString method;
	m_cMethod.GetWindowText(method);
	method.TrimLeft();			method.TrimRight();
	if (method.GetLength() == 0) {		// 설정된 메소드를 지우는 경우
		if (AfxMessageBox(_T("설정된 메소드를 지웁니다. 이후 유져는 메소드의 영향을 받지 않습니다."), MB_YESNO) != IDYES) return;
		query.Format(_T("UPDATE ESL_MENU_METHOD_TBL SET USER_METHOD_NAME='' WHERE FLAG='U' AND USER_ID='%s';"), m_sUSER_ID);
		ids = m_pDM->ExecuteQuery(query, true);
		if (ids == 0) {
			AfxMessageBox(_T("설정된 메소드를 지웠습니다."));
		} else {
			AfxMessageBox(_T("설정된 메소드를 지우던중 에러가 발생하였습니다."));
		}
		CDialog::OnOK();
		return;
	} else {							// 설정된 메소드를 변경하는 경우
		if (AfxMessageBox(_T("설정된 메소드를 변경합니다. 현재 유져에 설정된 데이타는 모두 삭제되며, 메소드에 설정된 데이타로 초기화 됩니다. 이후 유져는 해당 메소드의 영향을 받습니다."), MB_YESNO) != IDYES) return;
		ids = m_pDM->StartFrame();
		query.Format(_T("UPDATE ESL_MENU_METHOD_TBL SET USER_METHOD_NAME='%s' WHERE FLAG='U' AND USER_ID='%s';"), method, m_sUSER_ID);
		ids = m_pDM->AddFrame(query);
		query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='U' AND USER_ID='%s';"), m_sUSER_ID);
		ids = m_pDM->AddFrame(query);
		query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='U' AND USER_ID='%s';"), m_sUSER_ID);
		ids = m_pDM->AddFrame(query);

		ids1 = m_pDM->SendFrame();
		m_pDM->EndFrame();

		ids2 = ImportMethod_Menu(method);
		ids3 = ImportMethod_Toolbar(method);
		if (ids1 < 0 || ids2 < 0 || ids3 < 0) {
			AfxMessageBox(_T("입력하던중 에러가 발생하였습니다."));
		} else {
			AfxMessageBox(_T("입력되었습니다."));
			CDialog::OnOK();
			return;
		}
	}
	CDialog::OnOK();
	return;
}

VOID CAccessControlChangeMethodDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

INT CAccessControlChangeMethodDlg::ImportMethod_Menu(CString method)
{
	INT ids;
	CString query;
	query.Format(_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET FROM ESL_MENU_TBL WHERE FLAG='M' AND USER_ID='%s'"), method);
	ids = m_pDM->RestructDataManager(query);
	if (ids < 0) return ids;
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return 0;
	ids = m_pDM->StartFrame();
	TCHAR FLAG = 'U';
	CString strPK;
	for (INT i = 0; i < nRow; i++) {
		if (m_pDM->m_dbkind == 2) m_pDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
		if (m_pDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);
		query.Format(_T("INSERT INTO ESL_MENU_TBL (")
			         _T("PK, FLAG, USER_ID, ALIAS, ")
					 _T("GROUP_ALIAS, IDX, CAPTION, VISIBLE, ")
					 _T("STYLE, ACTION, TARGET) ")
					 _T("VALUES (%s, '%c', '%s', '%s', ")
					 _T("'%s', %s, '%s', '%s', ")
					 _T("'%s', '%s', '%s');"),
					 strPK, FLAG, m_sUSER_ID, m_pDM->GetCellData(i, 0), 
					 m_pDM->GetCellData(i, 1), m_pDM->GetCellData(i, 2), m_pDM->GetCellData(i, 3), m_pDM->GetCellData(i, 4), 
					 m_pDM->GetCellData(i, 5), m_pDM->GetCellData(i, 6), m_pDM->GetCellData(i, 7));
		ids = m_pDM->AddFrame(query);
	}
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) return ids;
	return 0;
}

INT CAccessControlChangeMethodDlg::ImportMethod_Toolbar(CString method)
{
	INT ids;
	CString query;
	query.Format(_T("SELECT ALIAS, MENU_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED FROM ESL_TOOLBAR_TBL WHERE FLAG = 'M' AND USER_ID='%s'"), method);
	ids = m_pDM->RestructDataManager(query);
	if (ids < 0) return ids;
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return 0;
	ids = m_pDM->StartFrame();
	TCHAR FLAG = 'U';
	CString strPK;
	for (INT i = 0; i < nRow; i++) {
		if (m_pDM->m_dbkind == 2) m_pDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
		if (m_pDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);
		query.Format(_T("INSERT INTO ESL_TOOLBAR_TBL (")
					 _T("PK, FLAG, USER_ID, ALIAS, ")
					 _T("MENU_ALIAS, IDX, CAPTION, STYLE, ")
					 _T("TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, ")
					 _T("BITMAP_DISABLED) ")
					 _T(" VALUES (%s, '%c', '%s', '%s', ")
					 _T("'%s', %s, '%s', '%s', ")
					 _T("'%s', '%s', '%s', '%s', ")
					 _T("'%s');"),
					 strPK, FLAG, m_sUSER_ID, m_pDM->GetCellData(i, 0), 
					 m_pDM->GetCellData(i, 1), m_pDM->GetCellData(i, 2), m_pDM->GetCellData(i, 3), m_pDM->GetCellData(i, 4), 
					 m_pDM->GetCellData(i, 5), m_pDM->GetCellData(i, 6), m_pDM->GetCellData(i, 7), m_pDM->GetCellData(i, 8), 
					 m_pDM->GetCellData(i, 9));
		ids = m_pDM->AddFrame(query);
	}
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) return ids;
	return 0;
}
