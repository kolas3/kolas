// Applicant_Class.cpp : implementation file
//

#include "stdafx.h"
#include "Applicant_Class.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApplicant_Class dialog


CApplicant_Class::CApplicant_Class(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplicant_Class)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CApplicant_Class::~CApplicant_Class()
{
}

BOOL CApplicant_Class::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CApplicant_Class::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicant_Class)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplicant_Class, CDialog)
	//{{AFX_MSG_MAP(CApplicant_Class)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplicant_Class message handlers

BOOL CApplicant_Class::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_비치희망자료_신청구분_부서")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_비치희망자료_신청구분_부서"));	
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Open CM Error ") );
		return FALSE;
	}


	m_pDM = FindDM(_T("DM_비치희망자료_신청구분_부서"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox ( _T("Open DM Error ") );
		return FALSE;
	}

	AddColorControl(IDC_edtAPPLICANT2, _T(""), _T(""), 'E');

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CApplicant_Class::SetDialog( CString strAppDate, CString strAppCode, CString strAppName )
{
	m_pCM->SetControlMgrData(_T("신청일자"), strAppDate);

	m_pCM->SetControlMgrData(_T("부서명"), strAppCode);

	m_pCM->SetControlMgrData(_T("신청자"), strAppName);
}

VOID CApplicant_Class::SetInitInputDate()
{
	CString strDate;
	CString strHour,strMin,strSec;
	CTime t = CTime::GetCurrentTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

}

BOOL CApplicant_Class::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;	
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CApplicant_Class::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}