// BO_CAT_PROC_0320.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0320.h"

#include "..\..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320 dialog


//CBO_CAT_PROC_0320::CBO_CAT_PROC_0320(CESL_Mgr* pParent, CString tmp /*=NULL*/)
CBO_CAT_PROC_0320::CBO_CAT_PROC_0320(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_BO_CAT_PROC_0300 = (CBO_CAT_PROC_0300*)pParent;		
	
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0320::~CBO_CAT_PROC_0320()
{
}


VOID CBO_CAT_PROC_0320::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bVARIABLE, OnbVARIABLE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320 message handlers

BOOL CBO_CAT_PROC_0320::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0320::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
		
	CString tmp;
	tmp = m_BO_CAT_PROC_0300->m_strStreamMarc;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0320"));
	pCM->AllControlDisplay(m_BO_CAT_PROC_0300->m_idx);	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE; 
}

VOID CBO_CAT_PROC_0320::OnbVARIABLE() 
{
	m_BO_CAT_PROC_0300->m_marcEditor.ApplyEdit();

	CString sYear1;
	GetControlData(_T("CM_BO_CAT_PROC_0320"), _T("발행년1"), sYear1);

	m_pInfo->pMarcMgr->SetItem(&m_BO_CAT_PROC_0300->m_marc, _T("008@7-10"), sYear1 );
	m_pInfo->pMarcMgr->DeleteItem(&m_BO_CAT_PROC_0300->m_marc, _T("발행년") );
	m_pInfo->pMarcMgr->SetItem(&m_BO_CAT_PROC_0300->m_marc, _T("발행년"), sYear1 );
	
	CString strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_BO_CAT_PROC_0300->m_marc, m_BO_CAT_PROC_0300->m_strStreamMarc);
	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	
	pDM->SetCellData(_T("MARC"), m_BO_CAT_PROC_0300->m_strStreamMarc , m_BO_CAT_PROC_0300->m_idx);

	CString tmp2 = pDM->GetCellData( 1, 1);
	CString tmp1 = pDM->GetCellData( _T("MARC.발행년1"), m_BO_CAT_PROC_0300->m_idx );

	m_BO_CAT_PROC_0300->m_marcEditor.Init(m_pInfo->pMarcMgr, &m_BO_CAT_PROC_0300->m_marc);
	m_BO_CAT_PROC_0300->m_marcEditor.Display();
}

BOOL CBO_CAT_PROC_0320::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 )
	{
		CKOMarcGetterDlg dlg( this );
		dlg.InitSetProperty( 4, _T("ULK") );
		dlg.DoModal();
	}

	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_CAT_PROC_0320::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}