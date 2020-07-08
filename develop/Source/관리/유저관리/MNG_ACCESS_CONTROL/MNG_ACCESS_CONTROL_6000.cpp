// MNG_ACCESS_CONTROL_6000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_ACCESS_CONTROL_6000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_ACCESS_CONTROL_5000.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_6000 dialog


CMNG_ACCESS_CONTROL_6000::CMNG_ACCESS_CONTROL_6000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ACCESS_CONTROL_6000)
	m_strSourceName = _T("");
	m_strTargetName = _T("");
	m_strDeleteName = _T("");
	//}}AFX_DATA_INIT
}
CMNG_ACCESS_CONTROL_6000::~CMNG_ACCESS_CONTROL_6000()
{
	
}

VOID CMNG_ACCESS_CONTROL_6000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ACCESS_CONTROL_6000)
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress1);
	DDX_Text(pDX, IDC_eSOURCENAME, m_strSourceName);
	DDX_Text(pDX, IDC_eTARGETNAME, m_strTargetName);
	DDX_Text(pDX, IDC_eDELETENAME, m_strDeleteName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ACCESS_CONTROL_6000, CDialog)
	//{{AFX_MSG_MAP(CMNG_ACCESS_CONTROL_6000)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bCOPY, OnbCOPY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_6000 message handlers

VOID CMNG_ACCESS_CONTROL_6000::OnbCLOSE() 
{
	CDialog::OnOK();	
}

BOOL CMNG_ACCESS_CONTROL_6000::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_ACCESS_CONTROL_6000::OnbCOPY() 
{
	UpdateData(TRUE);	

	TCHAR cSourceFlag = ' ', cTargetFlag = ' ';
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
	CButton *pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
	CButton *pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
	CButton *pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);

	if (pButton1->GetCheck() == TRUE) cSourceFlag = 'U';
	if (pButton2->GetCheck() == TRUE) cSourceFlag = 'M';
	if (pButton3->GetCheck() == TRUE) cTargetFlag = 'U';
	if (pButton4->GetCheck() == TRUE) cTargetFlag = 'M';
	if (m_strSourceName == _T("Administrator")) cSourceFlag = 'S';

	if (cSourceFlag == ' ') {
		AfxMessageBox(_T("Source의 Flag를 선택하여 주십시오."));
		return;
	}
	if (cTargetFlag == ' ') {
		AfxMessageBox(_T("Target의 Flag를 선택하여 주십시오."));
		return;
	}
	m_strSourceName.TrimLeft();			m_strSourceName.TrimRight();
	if (m_strSourceName.GetLength() == 0) {
		AfxMessageBox(_T("Source의 name을 입력하여 주십시오."));
		return;
	}
	m_strTargetName.TrimLeft();			m_strTargetName.TrimRight();
	if (m_strTargetName.GetLength() == 0) {
		AfxMessageBox(_T("Target의 name을 입력하여 주십시오."));
		return;
	}

	CMNG_ACCESS_CONTROL_5000 *pDlg = (CMNG_ACCESS_CONTROL_5000 *)pParentMgr;
	if (pDlg == NULL) {
		AfxMessageBox(_T("Fatal error!"));
		return;
	}
	INT ids = pDlg->METHOD_New(cTargetFlag, m_strTargetName, cSourceFlag, m_strSourceName, &m_pProgress1);
	CString strMsg;
	strMsg.Format(_T("Copy : Return value : [%d]"), ids);
	AfxMessageBox(strMsg);
	return;	
}

VOID CMNG_ACCESS_CONTROL_6000::OnbDELETE() 
{
	UpdateData(TRUE);	

	TCHAR cFlag = ' ';
	CButton *pButton5 = (CButton*)GetDlgItem(IDC_RADIO5);
	CButton *pButton6 = (CButton*)GetDlgItem(IDC_RADIO6);

	if (pButton5->GetCheck() == TRUE) cFlag = 'U';
	if (pButton6->GetCheck() == TRUE) cFlag = 'M';

	if (cFlag == ' ') {
		AfxMessageBox(_T("Flag를 선택하여 주십시오."));
		return;
	}
	m_strDeleteName.TrimLeft();			m_strDeleteName.TrimRight();
	if (m_strDeleteName.GetLength() == 0) {
		AfxMessageBox(_T("Delete할 name을 입력하여 주십시오."));
		return;
	}

	CString strMsg;
	if (cFlag == 'U') strMsg.Format(_T("사용자 [%s]의 메뉴와 툴바 정보를 모두 삭제하시겠습니까? \r\n메뉴와 툴바를 제외한 정보는 삭제되지 않습니다."), m_strDeleteName);
	if (cFlag == 'M') strMsg.Format(_T("메소드 [%s]의 메뉴와 툴바 정보를 모두 삭제하시겠습니까? \r\n메뉴와 툴바를 제외한 정보는 삭제되지 않습니다."), m_strDeleteName);
	if (AfxMessageBox(strMsg, MB_YESNO) != IDYES) return;

	CMNG_ACCESS_CONTROL_5000 *pDlg = (CMNG_ACCESS_CONTROL_5000 *)pParentMgr;
	if (pDlg == NULL) {
		AfxMessageBox(_T("Fatal error!"));
		return;
	}
	INT ids = pDlg->METHOD_Delete(cFlag, m_strDeleteName);
	strMsg.Format(_T("Delete : Return value : [%d]"), ids);
	AfxMessageBox(strMsg);
	
}


HBRUSH CMNG_ACCESS_CONTROL_6000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

