#include "stdafx.h"
#include "BO_STATICS_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_LIST dialog


CBO_STATICS_LIST::CBO_STATICS_LIST(CESL_Mgr* pParent /*=NULL*/, CESL_DataMgr* pSourceDM /*=NULL*/)
	: CESL_Mgr(CBO_STATICS_LIST::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_LIST)
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pSourceDM = pSourceDM;
	m_strGridTitle = _T("");
	//}}AFX_DATA_INIT
}


void CBO_STATICS_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_LIST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_STATICS_LIST, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_LIST)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBO_STATICS_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_BO_STATICS_LIST")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_BO_STATICS_LIST"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_BO_STATICS_LIST Error!"));
		return FALSE;
	}

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_BO_STATICS_LIST"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_BO_STATICS_LIST Error!"));
		return FALSE;
	}	 
	
	SetGridFormat();

	m_pDM->CopyDM(m_pSourceDM);
	m_pGrid->Display(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CBO_STATICS_LIST::SetGridFormat()
{
	if(m_strGridTitle.IsEmpty()) return -1;

	CStringArray saListField;
	INT ids = CLocCommonAPI::MakeArrayList( m_strGridTitle , _T("/") , saListField );
	if( ids < 0 ) -1;

	INT nColCnt = saListField.GetSize();
	if( nColCnt == 2 )
	{
		m_pGrid->SetTextMatrix(0, 1, saListField.GetAt(0));
		m_pGrid->SetTextMatrix(0, 2, saListField.GetAt(1));
	}

	return 1;
}
