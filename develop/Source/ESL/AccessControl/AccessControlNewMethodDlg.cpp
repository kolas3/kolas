// AccessControlNewMethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlNewMethodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlNewMethodDlg dialog


CAccessControlNewMethodDlg::CAccessControlNewMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlNewMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlNewMethodDlg)
	m_sName = _T("");
	//}}AFX_DATA_INIT

	FLAG = ' ';
	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;
}


VOID CAccessControlNewMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlNewMethodDlg)
	DDX_Control(pDX, IDC_cMETHOD, m_cMETHOD);
	DDX_Text(pDX, IDC_eNAME, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlNewMethodDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlNewMethodDlg)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlNewMethodDlg message handlers

VOID CAccessControlNewMethodDlg::OnbOK() 
{
	UpdateData(true);
	if (m_sName.GetLength() == 0) {
		AfxMessageBox(_T("이름을 입력하십시오."));
		return;
	}

	if (m_sName == _T("Administrator")) {
		AfxMessageBox(_T("사용 할수 없는 이름 입니다! "));
		return;
	}

	CString method;
	m_cMETHOD.GetWindowText(method);
	method.TrimLeft();			method.TrimRight();
	m_sName.TrimLeft();			m_sName.TrimRight();

	CString query, tmp;
	if (FLAG == 'M') query.Format(_T("SELECT COUNT(*) FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' AND METHOD_NAME='%s'"), m_sName);
		else query.Format(_T("SELECT COUNT(*) FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s'"), m_sName);
	INT ids;
	ids = m_pDM->GetOneValueQuery(query, tmp);
	if (ids < 0) {
		AfxMessageBox(_T("DB에서 기존 정보를 읽던중 에러가 발생하였습니다."));
		return;
	}
	if (_ttoi(tmp) > 0) {
		AfxMessageBox(_T("이미 사용되는 이름입니다."));
		return;
	}

	INT ids1, ids2, ids3;
	ids1 = ids2 = ids3 = 0;
	// 메소드 입력
	CString strPK;
	if (m_pDM->m_dbkind == 2) m_pDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
	if (m_pDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);
	if (FLAG == 'M') {
		query.Format(_T("INSERT INTO ESL_MENU_METHOD_TBL (PK, FLAG, METHOD_NAME) VALUES (%s, 'M', '%s');"), 
					 strPK, m_sName);
	} else {
		query.Format(_T("INSERT INTO ESL_MENU_METHOD_TBL (PK, FLAG, USER_ID, USER_METHOD_NAME) VALUES (%s, 'U', '%s', '%s');"), 
					 strPK, m_sName, method);
	}
	ids = m_pDM->StartFrame();
	ids = m_pDM->AddFrame(query);
	ids1 = m_pDM->SendFrame();
	ids = m_pDM->EndFrame();

	// Import 할 메소드가 있으면 Import 한다.
	if (method.GetLength() > 0) {
		ids2 = ImportMethod_Menu(method);
		ids3 = ImportMethod_Toolbar(method);
	}
	if (ids1 < 0 || ids2 < 0 || ids3 < 0) {
		AfxMessageBox(_T("입력하던중 에러가 발생하였습니다."));
	} else {
		AfxMessageBox(_T("입력되었습니다."));
		CDialog::OnOK();
	}
}

VOID CAccessControlNewMethodDlg::OnbCANCEL() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

VOID CAccessControlNewMethodDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}
}

BOOL CAccessControlNewMethodDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add extra initialization by won-jong, choi
	NewButtonList.Add(IDC_bOK, this, IDB_OK_U, IDB_OK_D, IDB_OK_F);
	NewButtonList.Add(IDC_bCANCEL, this, IDB_CANCEL_U, IDB_CANCEL_D, IDB_CANCEL_F);

	ReadCfgFile.ReadCfgFile(_T(""));
	m_pDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	
	m_pDM->RestructDataManager(_T("SELECT PK, METHOD_NAME FROM ESL_MENU_METHOD_TBL WHERE FLAG='M'"));
	INT nRow = m_pDM->GetRowCount();
	m_cMETHOD.AddString(_T(""));
	for (INT i = 0; i < nRow; i++) {
		m_cMETHOD.AddString(m_pDM->GetCellData(i, 1));
	}

	if (FLAG == 'M') SetWindowText(_T("New method dialog"));
		else SetWindowText(_T("New user dialog"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CAccessControlNewMethodDlg::ImportMethod_Menu(CString method)
{
	INT ids;
	CString query;
	query.Format(_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET FROM ESL_MENU_TBL WHERE FLAG='M' AND USER_ID='%s'"), method);
	ids = m_pDM->RestructDataManager(query);
	if (ids < 0) return ids;
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return 0;
	ids = m_pDM->StartFrame();
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
					 strPK, FLAG, m_sName, m_pDM->GetCellData(i, 0), 
					 m_pDM->GetCellData(i, 1), m_pDM->GetCellData(i, 2), m_pDM->GetCellData(i, 3), m_pDM->GetCellData(i, 4), 
					 m_pDM->GetCellData(i, 5), m_pDM->GetCellData(i, 6), m_pDM->GetCellData(i, 7));
		ids = m_pDM->AddFrame(query);
	}
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) return ids;
	return 0;
}

INT CAccessControlNewMethodDlg::ImportMethod_Toolbar(CString method)
{
	INT ids;
	CString query;
	query.Format(_T("SELECT ALIAS, MENU_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED FROM ESL_TOOLBAR_TBL WHERE FLAG = 'M' AND USER_ID='%s'"), method);
	ids = m_pDM->RestructDataManager(query);
	if (ids < 0) return ids;
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return 0;
	ids = m_pDM->StartFrame();
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
					 strPK, FLAG, m_sName, m_pDM->GetCellData(i, 0), 
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
