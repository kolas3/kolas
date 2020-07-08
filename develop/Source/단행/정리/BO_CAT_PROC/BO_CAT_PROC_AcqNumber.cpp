// BO_CAT_PROC_AcqNumber.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_proc.h")
#include "BO_CAT_PROC_AcqNumber.h"
#include "..\BO_CAT_NUMBER\BO_CAT_ACQUISIT_NO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_AcqNumber dialog


CBO_CAT_PROC_AcqNumber::CBO_CAT_PROC_AcqNumber(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_AcqNumber)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_AcqNumber::~CBO_CAT_PROC_AcqNumber()
{
}

VOID CBO_CAT_PROC_AcqNumber::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_AcqNumber)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_AcqNumber, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_AcqNumber)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bAcqReGive, OnbAcqReGive)
	ON_BN_CLICKED(IDC_bAcqDropNoSave, OnbAcqDropNoSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_AcqNumber message handlers

BOOL CBO_CAT_PROC_AcqNumber::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_AcqNumber")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	SetControlData( _T("CM_BO_CAT_PROC_AcqNumber"), _T("별치기호"), m_sShelfCode );
	SetControlData( _T("CM_BO_CAT_PROC_AcqNumber"), _T("분류기호구분"), m_sClassType );
	SetControlData( _T("CM_BO_CAT_PROC_AcqNumber"), _T("분류기호"), m_sClassNo );
	SetControlData( _T("CM_BO_CAT_PROC_AcqNumber"), _T("수입순번호"), m_sAcqNo );

	GetDlgItem(IDC_bAcqReGive)->SetWindowText(m_AcqReGive);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_AcqNumber::SetData(CString sShelfCode, CString sClassType, CString sClassNo, CString sAcqNo, CString sAcqReGive/*, CString sAcqToolBar*/ )
{
EFS_BEGIN

	m_sShelfCode = sShelfCode;
	m_sClassType = sClassType;
	m_sClassNo = sClassNo;
	m_sAcqNo = sAcqNo;
	m_AcqReGive =  sAcqReGive;

EFS_END
}

VOID CBO_CAT_PROC_AcqNumber::OnbAcqReGive() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CDialog::OnOK();

EFS_END
}

VOID CBO_CAT_PROC_AcqNumber::OnbAcqDropNoSave() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_ACQUISIT_NO dlg(1, this);
	INT iWorkCode = 2;
	dlg.SetData( iWorkCode, m_sShelfCode, m_sClassType, m_sClassNo, m_sAcqNo );
	if ( dlg.DoModal() == IDOK ) {
		if( dlg.m_sOmitAcqNo != _T(""))
		{
			SetControlData( _T("CM_BO_CAT_PROC_AcqNumber"), _T("수입순번호"), dlg.m_sOmitAcqNo );
		}
		m_iDropAcqSw = 1;
		GetDlgItem(IDC_eAcqNumber)->GetWindowText( m_sAcqNumber );
	}

EFS_END
}

CString CBO_CAT_PROC_AcqNumber::GetAcqNoData()
{
EFS_BEGIN
	return m_sAcqNumber;

EFS_END
return _T("");
}


BOOL CBO_CAT_PROC_AcqNumber::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->lParam == VK_RETURN )
		{
			OnbAcqReGive();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_CAT_PROC_AcqNumber::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
