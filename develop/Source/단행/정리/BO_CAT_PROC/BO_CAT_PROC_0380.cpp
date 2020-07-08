// BO_CAT_PROC_0380.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0380.h"
#include "BO_CAT_PROC_0382.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_CAT_PROC_0380::CBO_CAT_PROC_0380(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nModeMONO_SERIAL = cMODE_MONO;
}

CBO_CAT_PROC_0380::~CBO_CAT_PROC_0380()
{
}

VOID CBO_CAT_PROC_0380::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0380)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0380, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0380)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_b0380MARC, Onb0380MARC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0380 message handlers

BOOL CBO_CAT_PROC_0380::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0380::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	CString strSMAlias, strDMAlias, strCMAlias, strHD_Species;
	if (m_nModeMONO_SERIAL == cMODE_SERIAL)
	{
		strSMAlias = _T("SE_CAT_PROC_0380");
		strDMAlias = _T("DM_SE_CAT_PROC_0380");
		strCMAlias = _T("CM_SE_CAT_PROC_0380");
		strHD_Species = _T("SS");
	}
	else 
	{
		strSMAlias = _T("BO_CAT_PROC_0380");
		strDMAlias = _T("DM_BO_CAT_PROC_0380");
		strCMAlias = _T("CM_BO_CAT_PROC_0380");
		strHD_Species = _T("BS");
	}
	
	if (InitESL_Mgr(strSMAlias) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(strDMAlias);
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0380 ERROR") );
		return FALSE;
	}

	INT nRowCount = pDM->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (m_strParentSpeciesKey == pDM->GetCellData(strHD_Species + _T("_종KEY"), idx))
		{
			pDM->SetCellData(_T("비고"), _T("(자신)"), idx);
			break;
		}
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(strCMAlias);
	pCM->AllControlDisplay();	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0380::Onb0380MARC() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CBO_CAT_PROC_0382 dlg(this);
	dlg.m_nModeMONO_SERIAL = m_nModeMONO_SERIAL;
	dlg.DoModal();
	

EFS_END
}

HBRUSH CBO_CAT_PROC_0380::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
