// Applicant_Dept.cpp : implementation file
//

#include "stdafx.h"
#include "Applicant_Dept.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApplicant_Dept dialog


CApplicant_Dept::CApplicant_Dept(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplicant_Dept)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CApplicant_Dept::~CApplicant_Dept()
{
}

BOOL CApplicant_Dept::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CApplicant_Dept::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicant_Dept)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplicant_Dept, CDialog)
	//{{AFX_MSG_MAP(CApplicant_Dept)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplicant_Dept message handlers

BOOL CApplicant_Dept::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_비치희망자료_신청구분_학부")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_비치희망자료_신청구분_학부"));	
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Open CM Error ") );
		return FALSE;
	}


	m_pDM = FindDM(_T("DM_비치희망자료_신청구분_학부"));
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

VOID CApplicant_Dept::SetDialog( CString strAppDate, CString strAppCode, CString strAppName )
{
	m_pCM->SetControlMgrData(_T("신청일자"), strAppDate);

	m_pCM->SetControlMgrData(_T("학부명"), strAppCode);

	m_pCM->SetControlMgrData(_T("신청자"), strAppName);
}

VOID CApplicant_Dept::SetInitInputDate()
{
	CString strDate;
	CString strHour,strMin,strSec;
	CTime t = CTime::GetCurrentTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
}

BOOL CApplicant_Dept::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;	
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CApplicant_Dept::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}