// BO_Species_Detail_Marc_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Species_Detail_Marc_Viewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Marc_Viewer dialog


CBO_Species_Detail_Marc_Viewer::CBO_Species_Detail_Marc_Viewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_Species_Detail_Marc_Viewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Species_Detail_Marc_Viewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}
CBO_Species_Detail_Marc_Viewer::~CBO_Species_Detail_Marc_Viewer()
{
}
VOID CBO_Species_Detail_Marc_Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Species_Detail_Marc_Viewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Species_Detail_Marc_Viewer, CDialog)
	//{{AFX_MSG_MAP(CBO_Species_Detail_Marc_Viewer)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Marc_Viewer message handlers

BOOL CBO_Species_Detail_Marc_Viewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("마크보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_Species_Detail_Marc_Viewer::Init()
{
	if(m_strStreamMarc.IsEmpty())
	{
		return FALSE;
	}

	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);
	m_marcEditor.SubclassDlgItem(IDC_edtMARC_EDITOR, -1, this);
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();
	CMarcEditCtrl *pEditCtrl = m_marcEditor.GetEditCtrl();
	if (pEditCtrl)  pEditCtrl->SetReadOnly();

	return TRUE;
}

HBRUSH CBO_Species_Detail_Marc_Viewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{			
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
