// BO_Species_Detail_Appendix_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Species_Detail_Appendix_Viewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Appendix_Viewer dialog


CBO_Species_Detail_Appendix_Viewer::CBO_Species_Detail_Appendix_Viewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_Species_Detail_Appendix_Viewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Species_Detail_Appendix_Viewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// KOL.UDF.022 시큐어코딩 보완
	m_pDM = NULL;
	m_pCM  = NULL;
	m_pGrid  = NULL;
}

CBO_Species_Detail_Appendix_Viewer::~CBO_Species_Detail_Appendix_Viewer()
{
}

VOID CBO_Species_Detail_Appendix_Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Species_Detail_Appendix_Viewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Species_Detail_Appendix_Viewer, CDialog)
	//{{AFX_MSG_MAP(CBO_Species_Detail_Appendix_Viewer)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Appendix_Viewer message handlers

BOOL CBO_Species_Detail_Appendix_Viewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_단행_종_상세보기_부록")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_단행_종_상세보기_부록"));
	m_pDM = FindDM(_T("DMUP_단행_종_부록정보"));
	m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("부록정보 보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_Species_Detail_Appendix_Viewer::Init()
{
	if(m_strSpeciesKey.IsEmpty()) return FALSE;

	CString strQuery;
	strQuery.Format(_T("SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG='A'"), m_strSpeciesKey);
	m_pDM->RefreshDataManager(strQuery);
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("부록정보가 존재하지 않습니다."));
		EndDialog(IDCANCEL);
		return TRUE;
	}
	m_pGrid->Display();
	return TRUE;
}

HBRUSH CBO_Species_Detail_Appendix_Viewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
