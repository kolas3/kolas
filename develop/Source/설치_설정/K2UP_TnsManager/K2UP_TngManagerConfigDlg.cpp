// K2UP_TngManagerConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "K2UP_TnsManager.h"
#include "K2UP_TngManagerConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "K2UP_TnsManagerConfig.h"

/////////////////////////////////////////////////////////////////////////////
// CK2UP_TngManagerConfigDlg dialog


CK2UP_TngManagerConfigDlg::CK2UP_TngManagerConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CK2UP_TngManagerConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CK2UP_TngManagerConfigDlg)
	m_strDBMS_HOST = _T("");
	m_strDBMS_ID = _T("");
	m_strDBMS_NAME = _T("");
	m_strDBMS_PWD = _T("");
	m_strK2UP_PATH = _T("");
	m_nHOME = 0;
	//}}AFX_DATA_INIT

	m_strFileName = _T("");
}


VOID CK2UP_TngManagerConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CK2UP_TngManagerConfigDlg)
	DDX_Text(pDX, IDC_eDBMS_HOST, m_strDBMS_HOST);
	DDX_Text(pDX, IDC_eDBMS_ID, m_strDBMS_ID);
	DDX_Text(pDX, IDC_eDBMS_NAME, m_strDBMS_NAME);
	DDX_Text(pDX, IDC_eDBMS_PWD, m_strDBMS_PWD);
	DDX_Text(pDX, IDC_eK2UP_PATH, m_strK2UP_PATH);
	DDX_Text(pDX, IDC_eORACLE_HOME, m_nHOME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CK2UP_TngManagerConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CK2UP_TngManagerConfigDlg)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bK2UP_MAIN, OnbK2UPMAIN)
	ON_BN_CLICKED(IDC_IDOK, OnOK)
	ON_BN_CLICKED(IDC_bINIT, OnbINIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CK2UP_TngManagerConfigDlg message handlers

VOID CK2UP_TngManagerConfigDlg::OnOK() 
{
	UpdateData(TRUE);
	CK2UP_TnsManagerConfig TnsManagerConfig;

	TnsManagerConfig.K2UP_PATH = m_strK2UP_PATH;
	TnsManagerConfig.EXECUTE_K2UP = GetExecute_K2UP((CButton*)GetDlgItem(IDC_RADIO1), (CButton*)GetDlgItem(IDC_RADIO2));
	TnsManagerConfig.EXECUTE_K2UP_AT_SCREEN_MODE = GetExecute_K2UP((CButton*)GetDlgItem(IDC_RADIO3), (CButton*)GetDlgItem(IDC_RADIO4));
	TnsManagerConfig.DBMS_HOST = m_strDBMS_HOST;
	TnsManagerConfig.DBMS_ID = m_strDBMS_ID;
	TnsManagerConfig.DBMS_PWD = m_strDBMS_PWD;
	TnsManagerConfig.DBMS_NAME = m_strDBMS_NAME;
	TnsManagerConfig.nHOME = m_nHOME;

	INT ids = TnsManagerConfig.SaveConfig();
	if (ids == 0) {
		AfxMessageBox(_T("적용되었습니다."));
	} else {
		AfxMessageBox(_T("적용중 에러가 발생하였습니다."));
	}
	CDialog::OnOK();
}

VOID CK2UP_TngManagerConfigDlg::OnbCANCEL() 
{
	CDialog::OnCancel();
}

VOID CK2UP_TngManagerConfigDlg::OnbK2UPMAIN() 
{
	TCHAR TmpDir[500];
	GetCurrentDirectory(500,TmpDir);

	CFileDialog dlg(TRUE, NULL, _T("*.EXE"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                    _T("EXE files(*.EXE)|All files(*.*)|*.*|"), this);
	if(dlg.DoModal() != IDOK) {
		SetCurrentDirectory(TmpDir);
        return;
	}
	SetCurrentDirectory(TmpDir);

	m_strK2UP_PATH = dlg.GetPathName();
	UpdateData(FALSE);
}

BOOL CK2UP_TngManagerConfigDlg::GetExecute_K2UP(CButton *pBtn_Yes, CButton *pBtn_No)
{
	if (pBtn_Yes == NULL || pBtn_No == NULL) return FALSE;

	if (pBtn_Yes->GetCheck() == TRUE) return TRUE;
	return FALSE;
}

VOID CK2UP_TngManagerConfigDlg::SetExecute_K2UP(BOOL EXECUTE_K2UP, CButton *pBtn_Yes, CButton *pBtn_No)
{
	if (pBtn_Yes == NULL || pBtn_No == NULL) return;
		
	if (EXECUTE_K2UP == TRUE) {
		pBtn_Yes->SetCheck(TRUE);
		pBtn_No->SetCheck(FALSE);
	} else {
		pBtn_Yes->SetCheck(FALSE);
		pBtn_No->SetCheck(TRUE);
	}

	return;
}

VOID CK2UP_TngManagerConfigDlg::LoadConfig(VOID)
{
	CK2UP_TnsManagerConfig TnsManagerConfig;
	INT ids = TnsManagerConfig.LoadConfig();
	if (ids < 0) return;

	m_strK2UP_PATH = TnsManagerConfig.K2UP_PATH;
	SetExecute_K2UP(TnsManagerConfig.EXECUTE_K2UP, (CButton*)GetDlgItem(IDC_RADIO1), (CButton*)GetDlgItem(IDC_RADIO2));
	SetExecute_K2UP(TnsManagerConfig.EXECUTE_K2UP_AT_SCREEN_MODE, (CButton*)GetDlgItem(IDC_RADIO3), (CButton*)GetDlgItem(IDC_RADIO4));
	m_strDBMS_HOST = TnsManagerConfig.DBMS_HOST;
	m_strDBMS_ID = TnsManagerConfig.DBMS_ID;
	m_strDBMS_PWD = TnsManagerConfig.DBMS_PWD;
	m_strDBMS_NAME = TnsManagerConfig.DBMS_NAME;
	m_nHOME = TnsManagerConfig.nHOME;

	UpdateData(FALSE);

	return;
}

BOOL CK2UP_TngManagerConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadConfig();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CK2UP_TngManagerConfigDlg::OnbINIT() 
{
	if (AfxMessageBox(_T("tnsnames.ora 파일을 초기화 합니다. 계속 하시겠습니까?"), MB_YESNO) != IDYES) return;
	
	CStdioFile file;
	if (!file.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		AfxMessageBox(_T("저장 실패 : 파일을 열수 없습니다."));
		return;
	}

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strTemp;
	strTemp.Format(_T("%s = \r\n")
				   _T("  (DESCRIPTION = \r\n")
				   _T("    (ADDRESS_LIST = \r\n")
				   _T("      (ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s)) \r\n")
				   _T("    ) \r\n")
				   _T("    (CONNECT_DATA = \r\n")
				   _T("      (SERVICE_NAME = %s)\r\n")
				   _T("    )\r\n")
				   _T("  )\r\n\r\n")
				   , _T("K2UP"), _T("192.168.1.230"), _T("1521"), _T("K2UP"));

	file.WriteString(strTemp);
	file.Close();
	
	AfxMessageBox(_T("tnsnames.ora 파일이 초기화 되었습니다."));
}
