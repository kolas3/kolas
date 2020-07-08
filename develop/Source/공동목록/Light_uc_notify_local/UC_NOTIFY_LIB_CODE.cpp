// UC_NOTIFY_LIB_CODE.cpp : implementation file
//

#include "stdafx.h"
#include "UC_NOTIFY_LIB_CODE.h"
#include "..\check_dup_api\RS_SQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUC_NOTIFY_LIB_CODE dialog


CUC_NOTIFY_LIB_CODE::CUC_NOTIFY_LIB_CODE(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUC_NOTIFY_LIB_CODE::~CUC_NOTIFY_LIB_CODE()
{
	
}

BOOL CUC_NOTIFY_LIB_CODE::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CUC_NOTIFY_LIB_CODE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUC_NOTIFY_LIB_CODE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUC_NOTIFY_LIB_CODE, CDialog)
	//{{AFX_MSG_MAP(CUC_NOTIFY_LIB_CODE)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUC_NOTIFY_LIB_CODE message handlers

BOOL CUC_NOTIFY_LIB_CODE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_UC_NOTIFY_LIB_CODE"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
      
    m_CurrentCM =_T("CM_UC_NOTIFY_LIB_CODE");
	m_CurrentDM =_T("DM_UC_NOTIFY_LIB_CODE");
	m_CurrentGrid =_T("MainGrid") ;


    INT ids ;
	CString sWhere ;
	ids = InitDisplay(sWhere) ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CUC_NOTIFY_LIB_CODE::InitDisplay(CString sWhere)
{
	CESL_DataMgr *pDM = FindDM(m_CurrentDM);
	if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}
/*
	CRS_SQL m_rs_sql(this) ;
	
	m_rs_sql.Kolinet_Select (pDM,sWhere,m_CurrentDM,100,3) ;
*/
	AllControlDisplay (m_CurrentCM ,-1) ;


	return 0 ;
}

VOID CUC_NOTIFY_LIB_CODE::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	INT ids ;
	CString sWhere ;
    CString sName ;
	ids = GetControlData (m_CurrentCM,_T("µµ¼­°ü¸í"),sName,-1) ;


	sWhere.Format (_T(" NAME between  '%s' and '%sÈþ'"),sName,sName) ;
	
	ids = InitDisplay(sWhere) ;


}

VOID CUC_NOTIFY_LIB_CODE::OnOK() 
{
	// TODO: Add extra validation here
	CESL_ControlMgr *pCM = FindCM(m_CurrentCM);
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	CESL_Grid *m_pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid);
	if(m_pGrid == NULL) {
		AfxMessageBox(_T("CESL_Grid() Error~!"));
		return ;
	}

	m_arLibrary.RemoveAll ();
	m_Lib_Code.RemoveAll ();
	m_Lib_Name.RemoveAll () ;

	m_pGrid->SelectMakeList () ;
     
	INT      ids ,SelectCnt ,m_idx  ;
	CString  sLib_id,sName , sNameCode ;
	
	SelectCnt = m_pGrid->SelectGetCount () ;

    for(INT i = 0 ; i < SelectCnt ;i++) 
	{
	    m_idx = m_pGrid->SelectGetIdx () ;
		ids = GetDataMgrData (m_CurrentDM,_T("LIB_ID"),sLib_id,m_idx );	
        ids = GetDataMgrData (m_CurrentDM,_T("NAME"),sName ,m_idx ) ;
		sNameCode.Format (_T("%s(%s)"),sName,sLib_id) ;
        
		m_Lib_Code.Add (sLib_id);
		m_Lib_Name.Add (sName);
		m_arLibrary.Add (sNameCode) ;
		sLib_id.Empty ();
		sName.Empty ();
	    m_pGrid->SelectGetNext () ;
	}
     

	CDialog::OnOK();
}

HBRUSH CUC_NOTIFY_LIB_CODE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
