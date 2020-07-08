// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "ExcelToMarc.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pInfo = NULL;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

void CLoginDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strId;
	CString strPw;
	CEdit* pEdtId = (CEdit*)GetDlgItem(IDC_edtId);
	CEdit* pEdtPw = (CEdit*)GetDlgItem(IDC_edtPw);
	pEdtId->GetWindowText(strId);
	pEdtPw->GetWindowText(strPw);
	if( strId.IsEmpty())
	{
		AfxMessageBox(_T("LOGIN ID를 입력해 주세요!"));
		pEdtId->SetFocus();
		return;
	}

	if( strPw.IsEmpty())
	{
		AfxMessageBox(_T("LOGIN PW를 입력해 주세요!"));
		pEdtPw->SetFocus();
		return;
	}

	INT nIds = 0;
	nIds = SetParentInfo();
	if( 0 > nIds )
	{
		AfxMessageBox(_T("LOGIN 정보를 정확하게 입력해 주세요!"));
		pEdtId->SetFocus();
		return;
	}
	LoadCodeInfo();
	MakeManageCodeFile();

	CDialog::OnOK();
}

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (m_pInfo == NULL) 
	{
		MessageBox(_T("m_pInfo IS NULL"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CEdit* pEdtId = (CEdit*)GetDlgItem(IDC_edtId);
	pEdtId->SetFocus();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLoginDlg::SetParentInfo()
{
//	m_pInfo = new CESL_Information();
	INT ids = -1;
	m_pInfo->CONNECTION_INFO = m_strConnectInfo;
	
	m_pControlDm.SetCONNECTION_INFO(m_strConnectInfo);

	CString strQuery;

	CEdit* pEdtId = (CEdit*)GetDlgItem(IDC_edtId);
	CEdit* pEdtPw = (CEdit*)GetDlgItem(IDC_edtPw);
	pEdtId->GetWindowText(m_strID);
	pEdtPw->GetWindowText(m_strPassword);

	//                          0        1         2          3		
//	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE "
//						 _T("FROM MN_USER_TBL ")
//						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, m_strPassword);
//	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, ID "
//					 _T("FROM MN_USER_TBL ")
//					_T("WHERE REC_KEY = ( SELECT MIN( REC_KEY )FROM MN_USER_TBL WHERE PERMITION = 'A' )")));

	//                          0        1         2          3		
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, ID "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, m_strPassword);

	ids = m_pControlDm.RestructDataManager(strQuery);
	if (ids < 0) return ids;
	if (m_pControlDm.GetRowCount() < 1) return -1;	// 로그인 실패
	// m_pInfo 세팅한다.
//	m_pInfo->USER_ID		= m_strID;
	m_pInfo->USER_NAME		= m_pControlDm.GetCellData(0, 0); 
	m_pInfo->USER_PRIVILEGE = m_pControlDm.GetCellData(0, 1); 
	m_pInfo->USER_PK		= m_pControlDm.GetCellData(0, 2);
//	m_pInfo->LOCAL_IP		= GetIP();
	m_pInfo->MANAGE_CODE	= m_pControlDm.GetCellData(0, 3);
	m_pInfo->USER_ID		= m_pControlDm.GetCellData(0, 4);
	m_pInfo->MANAGE_CODE.TrimLeft();
	m_pInfo->MANAGE_CODE.TrimRight();
	m_pControlDm.MANAGE_CODE = m_pInfo->MANAGE_CODE;

	// 2009.11.19 ADD BY PWR : 통합대출권수사용여부가 사용안함이면 통합환경으로 보지 않는다. (PERMITION이 'A'이면 'S'로 설정)
	CString strTemp;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME='통합대출권수사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_pControlDm.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft(); strTemp.TrimRight();
	if(_T("Y") != strTemp)
	{
		if(_T("A") == m_pInfo->USER_PRIVILEGE)
		{
			m_pInfo->USER_PRIVILEGE = _T("S");
		}
	}

	//2008.11.11 ADD  BY PJW : 등록구분코드 자리수 설정
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '등록구분코드자리수설정' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	m_pControlDm.GetOneValueQuery(strQuery, strTemp);
	m_pInfo->nREG_CODE_LENGTH = _ttoi(strTemp);

	return 0;
}

INT CLoginDlg::LoadCodeInfo()
{
	m_pInfo->pCodeMgr->CONNECTION_INFO = m_pInfo->CONNECTION_INFO;

	return m_pInfo->pCodeMgr->ReadCodeData(m_pInfo->USER_PK, NULL);
}

void CLoginDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


INT CLoginDlg::MakeManageCodeFile() 
{
	CString strResult[2];
	CStdioFile file;
	DWORD dwSID = ::GetCurrentProcessId();
	CString strCHMfileName;
	strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwSID);
		
	if (file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		CString strLine;
		strLine.Format(_T("%s\r\n"), m_pInfo->MANAGE_CODE);
		file.WriteString(strLine);
		file.Close();
	}
	return 0;
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->wParam == VK_RETURN )
	{
		OnOK();
		return TRUE;
	}
	else if( pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
