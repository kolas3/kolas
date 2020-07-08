// MNG_LIBINFO_MGR_2000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIBINFO_MGR_2000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIBINFO_MGR_2000 dialog


CMNG_LIBINFO_MGR_2000::CMNG_LIBINFO_MGR_2000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIBINFO_MGR_2000)
	m_strLIB_ADDRESS = _T("");
	m_strLIB_DIRECTOR_NAME = _T("");
	m_strLIB_EMAIL = _T("");
	m_strLIB_FAX = _T("");
	m_strLIB_NAME = _T("");
	m_strLIB_TEL = _T("");
	m_strLIB_URL = _T("");
	m_strLIB_ZIP_CODE = _T("");
	//}}AFX_DATA_INIT

	m_cUseMANAGE_CODE = 'N';
}
CMNG_LIBINFO_MGR_2000::~CMNG_LIBINFO_MGR_2000()
{
	
}

VOID CMNG_LIBINFO_MGR_2000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIBINFO_MGR_2000)
	DDX_Text(pDX, IDC_eLIB_ADDRESS, m_strLIB_ADDRESS);
	DDX_Text(pDX, IDC_eLIB_DIRECTOR_NAME, m_strLIB_DIRECTOR_NAME);
	DDX_Text(pDX, IDC_eLIB_EMAIL, m_strLIB_EMAIL);
	DDX_Text(pDX, IDC_eLIB_FAX, m_strLIB_FAX);
	DDX_Text(pDX, IDC_eLIB_NAME, m_strLIB_NAME);
	DDX_Text(pDX, IDC_eLIB_TEL, m_strLIB_TEL);
	DDX_Text(pDX, IDC_eLIB_URL, m_strLIB_URL);
	DDX_Text(pDX, IDC_eLIB_ZIP_CODE, m_strLIB_ZIP_CODE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIBINFO_MGR_2000, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIBINFO_MGR_2000)
	ON_WM_CTLCOLOR()    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIBINFO_MGR_2000 message handlers

BOOL CMNG_LIBINFO_MGR_2000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_LIBINFO_MGR_2000") ) < 0 )
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIBINFO_MGR_2000") );
	if (m_cUseMANAGE_CODE == 'N') {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_MNG_LIBINFO_MGR_2000"), _T("관리구분"));
		if (pCMB) {
			pCMB->EnableWindow(FALSE);
			pCMB->SetCurSel(0);
		}
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIBINFO_MGR_2000::OnOK() 
{
	UpdateData(TRUE);

	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_MNG_LIBINFO_MGR_2000"), _T("관리구분"));
	if (pCMB) {
		pCMB->GetData(m_strMANAGE_CODE);
	}
	//++2008.10.15 DEL BY PWR {{++
	// 통합시스템 적용
	m_strMANAGE_CODE.TrimLeft();		m_strMANAGE_CODE.TrimRight();
	if (m_cUseMANAGE_CODE == 'Y' && m_strMANAGE_CODE == _T("")) {
		AfxMessageBox(_T("관리구분별도서관설정이 'Y'인경우는 관리구분을 '적용안함'으로 할 수 없습니다."));
		return;
	}
	//--2008.10.15 DEL BY PWR --}}
	CDialog::OnOK();
}

VOID CMNG_LIBINFO_MGR_2000::OnCancel() 
{
	
	CDialog::OnCancel();
}



HBRUSH CMNG_LIBINFO_MGR_2000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}