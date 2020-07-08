// DonatorIDGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "DonatorIDGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorIDGetterDlg dialog


CDonatorIDGetterDlg::CDonatorIDGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorIDGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_DMRow = -1;
}

CDonatorIDGetterDlg::~CDonatorIDGetterDlg()
{	
	
}

BOOL CDonatorIDGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CDonatorIDGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorIDGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorIDGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CDonatorIDGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorIDGetterDlg message handlers

BOOL CDonatorIDGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("DonatorIDGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : DonatorIDGetterDlg") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

VOID CDonatorIDGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		SearchDonatorID();
}

VOID CDonatorIDGetterDlg::SearchDonatorID()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_DonatorIDGetterDlg") );
	pDM->RefreshDataManager( _T("") );

	ControlDisplay( _T("CM_DonatorIDGetterDlg"), _T("그리드") );
}


BEGIN_EVENTSINK_MAP(CDonatorIDGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDonatorIDGetterDlg)
	ON_EVENT(CDonatorIDGetterDlg, IDC_mngGRID_D, -600 /* Click */, OnClickmngGRIDD, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDonatorIDGetterDlg::OnClickmngGRIDD() 
{
	// TODO: Add your control notification handler code here
	
	SetCurrentDMRow();
}

VOID CDonatorIDGetterDlg::SetCurrentDMRow()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_DonatorIDGetterDlg") );
	if( !pDM->GetRowCount() )
		return;
	
	CESL_ControlMgr *pCM = FindCM( _T("CM_DonatorIDGetterDlg") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	m_DMRow = pGrid->GetRow() - 1;

	pGrid->SetReverse( m_DMRow );	
}

VOID CDonatorIDGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	if( -1 == m_DMRow )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_DonatorIDGetterDlg") );
	pDM->GetCellData( _T("DONATOR_ID"), m_DMRow, m_ID );	
	
	CDialog::OnOK();
}

HBRUSH CDonatorIDGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
