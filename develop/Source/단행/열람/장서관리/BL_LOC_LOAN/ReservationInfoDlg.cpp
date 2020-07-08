// ReservationInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReservationInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReservationInfoDlg dialog


CReservationInfoDlg::CReservationInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CReservationInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCloseMode = 0;
}

CReservationInfoDlg::~CReservationInfoDlg()
{
}
VOID CReservationInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReservationInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CReservationInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CReservationInfoDlg)
	ON_BN_CLICKED(IDC_btnCOTINUE, OnbtnCOTINUE)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReservationInfoDlg message handlers

BOOL CReservationInfoDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(InitESL_Mgr(_T("RESERVATION_INFO_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}	

	INT ids = AllControlDisplay( _T("CM_RESERVATION_INFO") );
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("View Mgr Error!"));
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CReservationInfoDlg::GetCloseMode()
{
	EFS_BEGIN

	return m_nCloseMode;

	EFS_END
	return -1;

}


VOID CReservationInfoDlg::OnbtnCOTINUE() 
{
	EFS_BEGIN

	m_nCloseMode = 1;
	OnCancel();

	EFS_END
}

VOID CReservationInfoDlg::OnbtnCANCEL() 
{
	EFS_BEGIN

	m_nCloseMode = 0;
	OnCancel();

	EFS_END
}

HBRUSH CReservationInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
