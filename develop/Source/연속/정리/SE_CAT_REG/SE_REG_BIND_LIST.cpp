// SE_REG_BIND_LIST.cpp : implementation file
//

#include "stdafx.h"
#include "SE_REG_BIND_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_REG_BIND_LIST dialog


CSE_REG_BIND_LIST::CSE_REG_BIND_LIST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_REG_BIND_LIST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSE_REG_BIND_LIST::~CSE_REG_BIND_LIST()
{
}


VOID CSE_REG_BIND_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_REG_BIND_LIST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_REG_BIND_LIST, CDialog)
	//{{AFX_MSG_MAP(CSE_REG_BIND_LIST)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_REG_BIND_LIST message handlers

BOOL CSE_REG_BIND_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_등록_제본목록")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	

	m_RecKey.TrimLeft();
	m_RecKey.TrimRight();

	if( m_RecKey == _T(""))
	{
		ESLAfxMessageBox(_T("자료가 없습니다."));
		EndDialog(0);
		return FALSE;
	}


	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_제본목록"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return FALSE;
	}

	CString strQry,BookKeys;
	strQry = _T("SELECT BOOK_KEYS FROM SE_BINDING_BOOK_TBL WHERE BOOK_KEY =")+m_RecKey;

	pDM->GetOneValueQuery(strQry ,BookKeys);

	strQry = _T("B.REC_KEY IN(")+BookKeys+_T(")");
	pDM->RefreshDataManager(strQry);


	CESL_ControlMgr *pCM = FindCM(_T("CM_연속_정리_등록_제본목록"));
	if ( NULL == pCM ) {
		AfxMessageBox ( _T("CM_연속_정리_등록_메인") );
		return FALSE;
	}

	pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSE_REG_BIND_LIST::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
