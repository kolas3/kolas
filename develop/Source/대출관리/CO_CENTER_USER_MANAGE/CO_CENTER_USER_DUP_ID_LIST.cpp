// CO_CENTER_USER_DUP_ID_LIST.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_DUP_ID_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DUP_ID_LIST dialog


CCO_CENTER_USER_DUP_ID_LIST::CCO_CENTER_USER_DUP_ID_LIST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_DUP_ID_LIST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDupIDImportCnt = 0;
	m_nSucessImportCnt = 0;
	m_overlapedCnt = 0;
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CCO_CENTER_USER_DUP_ID_LIST::~CCO_CENTER_USER_DUP_ID_LIST()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CCO_CENTER_USER_DUP_ID_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_DUP_ID_LIST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_DUP_ID_LIST, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_DUP_ID_LIST)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DUP_ID_LIST message handlers

BOOL CCO_CENTER_USER_DUP_ID_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("CO_CENTER_USER_DUP_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	Display();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCO_CENTER_USER_DUP_ID_LIST::SetData(INT nSucessCnt, INT nOverLapCnt, INT nDupCnt)
{
	m_nSucessImportCnt = nSucessCnt;
	m_overlapedCnt = nOverLapCnt;
	m_nDupIDImportCnt = nDupCnt;
}

VOID CCO_CENTER_USER_DUP_ID_LIST::Display()
{
	CString strTmp;
	strTmp.Format(_T("%d 명"), m_nSucessImportCnt);
	( (CStatic*) GetDlgItem (IDC_sSucess) )->SetWindowText(strTmp);

	strTmp.Format(_T("%d 명"), m_overlapedCnt);
	( (CStatic*) GetDlgItem (IDC_sFail) )->SetWindowText(strTmp);
	//( (CStatic*) GetDlgItem ( ) )->SetWindowText();	

	
	CString CMAlias = _T("CM_CENTER_USER_DUP_LIST");
	CESL_ControlMgr *pCM = FindCM( CMAlias );
	if( pCM == NULL ) return;	

	pCM->AllControlDisplay();
}

VOID CCO_CENTER_USER_DUP_ID_LIST::InitControl()
{
	
}

HBRUSH CCO_CENTER_USER_DUP_ID_LIST::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
