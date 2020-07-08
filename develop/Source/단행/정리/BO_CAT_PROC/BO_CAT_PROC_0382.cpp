// BO_CAT_PROC_0382.cpp : implementation file
//

#include "stdafx.h"

#include "BO_CAT_PROC_0382.h"
#include "..\..\..\esl\marc\Marcedit\MarcEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0382 dialog


CBO_CAT_PROC_0382::CBO_CAT_PROC_0382(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0382)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid_Parent = NULL;
	m_pDM_Parent = NULL;
	m_nParentSelectedRow = 0;

	m_nModeMONO_SERIAL = cMODE_MONO;
}

CBO_CAT_PROC_0382::~CBO_CAT_PROC_0382()
{
}

VOID CBO_CAT_PROC_0382::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0382)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0382, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0382)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_b0382MarcCLOSE, Onb0382MarcCLOSE)
	ON_BN_CLICKED(IDC_bPREV_BCP, OnbPREVBCP)
	ON_BN_CLICKED(IDC_bNEXT_BCP, OnbNEXTBCP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0382 Operations

INT CBO_CAT_PROC_0382::Display()
{
	CString strHD_Species;
	if (m_nModeMONO_SERIAL == cMODE_SERIAL) strHD_Species = _T("SS");
	else strHD_Species = _T("BS");

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	CString strStreamMarc = m_pDM_Parent->GetCellData( strHD_Species + _T("_MARC") , m_nParentSelectedRow);

	INT ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
	if (ids < 0) return ids;
	
	m_marcEditor.InitUserAlias();
	m_marcEditor.InitLimitTag();
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0382 message handlers

BOOL CBO_CAT_PROC_0382::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0382::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	INT ids = -1;

	CString strDMAlias, strCMAlias;
	if (m_nModeMONO_SERIAL == cMODE_SERIAL)
	{
		strDMAlias = _T("DM_SE_CAT_PROC_0380");
		strCMAlias = _T("CM_SE_CAT_PROC_0380");
	}
	else 
	{
		strDMAlias = _T("DM_BO_CAT_PROC_0380");
		strCMAlias = _T("CM_BO_CAT_PROC_0380");
	}
	
	m_pGrid_Parent = (CESL_Grid*)FindControl(strCMAlias, _T("청구기호조사_그리드"));
	if (!m_pGrid_Parent) goto ERR;

	m_nParentSelectedRow = m_pGrid_Parent->GetIdx();
	if (m_nParentSelectedRow < 0) goto ERR;

	m_pDM_Parent = FindDM(strDMAlias);
	if ( NULL == m_pDM_Parent) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0330 error") );
		goto ERR;
	}

	m_pDM_Parent->SetMarcMgr(m_pInfo->pMarcMgr);

	m_marcEditor.SubclassDlgItem(IDC_REDT_0382_MARC_EDITOR, -1, this);		
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);

	Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	EndDialog(IDCANCEL);
	return FALSE;

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0382::Onb0382MarcCLOSE() 
{
EFS_BEGIN

	EndDialog(IDOK);

EFS_END
}

VOID CBO_CAT_PROC_0382::OnbPREVBCP() 
{
	if (m_nParentSelectedRow < 0) return;
	
	if (m_nParentSelectedRow > 0)
		m_nParentSelectedRow--;

	Display();
}

VOID CBO_CAT_PROC_0382::OnbNEXTBCP() 
{
	if (m_nParentSelectedRow < 0) return;
	
	INT nRowCount = m_pDM_Parent->GetRowCount();

	if (m_nParentSelectedRow < nRowCount - 1)
		m_nParentSelectedRow++;

	Display();
}

HBRUSH CBO_CAT_PROC_0382::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
