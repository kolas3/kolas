// SpeciesErrorDataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeciesErrorDataSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeciesErrorDataSearchDlg dialog


CSpeciesErrorDataSearchDlg::CSpeciesErrorDataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustSearchMgr(CSpeciesErrorDataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpeciesErrorDataSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSpeciesErrorDataSearchDlg::~CSpeciesErrorDataSearchDlg()
{
	
}

VOID CSpeciesErrorDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeciesErrorDataSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpeciesErrorDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeciesErrorDataSearchDlg)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeciesErrorDataSearchDlg message handlers

BOOL CSpeciesErrorDataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!Init())
	{
		ESLAfxMessageBox(_T("종오류정보 검색 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;		
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSpeciesErrorDataSearchDlg::Init()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)GetDlgItem(IDC_cmbDIV_VALUE);
	if(pCMB == NULL) return FALSE;
	pCMB->ResetContent();
	pCMB->AddString(_T("작업종자료"));
	pCMB->AddString(_T("재활용자료"));
	pCMB->AddString(_T("배가된복본추기자료"));
	pCMB->AddString(_T("자료상태불일치자료"));
	pCMB->SetCurSel(0);	
	return TRUE;
}

BOOL CSpeciesErrorDataSearchDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSpeciesErrorDataSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)GetDlgItem(IDC_cmbDIV_VALUE);
	if(pCMB == NULL) return ;
	
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel == 0)
	{
		m_strQuery.Format(_T("I.REC_KEY=S.REC_KEY(+) AND I.REC_KEY=B.SPECIES_KEY AND S.REC_KEY IS NULL AND I.SPECIES_CLASS='0'"));
	}
	else if(nCurSel == 1)
	{
		m_strQuery.Format(_T("I.REC_KEY=S.REC_KEY(+) AND I.REC_KEY=B.SPECIES_KEY AND S.REC_KEY IS NULL AND I.SPECIES_CLASS='2'"));
	}		 
	else if(nCurSel == 2)
	{
		m_strQuery.Format(
			_T("I.REC_KEY=S.REC_KEY(+) ")
			_T("AND i.rec_key = b.DUP_SPECIES_KEY ")
			_T("AND i.rec_key not in (select i.rec_key from idx_bo_tbl i, bo_book_tbl b where i.rec_key = b.DUP_SPECIES_KEY and (b.working_status < 'BOL' or b.working_status > 'BOR'))"));
	}
	else if(nCurSel == 3)
	{
		m_strQuery.Format(
			_T("I.REC_KEY=S.REC_KEY ")
			_T("AND I.REC_KEY=B.SPECIES_KEY ")
			_T("AND b.working_status > 'BOL' ")
			_T("AND b.working_status < 'BOR' ")
			_T("AND (i.working_status < 'BOL' or i.working_status > 'BOR')"));
	}

	SendQueryCreateMsg(nCurSel);
}

VOID CSpeciesErrorDataSearchDlg::SendQueryCreateMsg(INT nData)
{
	::SendMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, nData, 0);
}



HBRUSH CSpeciesErrorDataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
