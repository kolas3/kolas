// ShelfCodeGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "ShelfCodeGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShelfCodeGetterDlg dialog


CShelfCodeGetterDlg::CShelfCodeGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CShelfCodeGetterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShelfCodeGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CShelfCodeGetterDlg::~CShelfCodeGetterDlg()
{
	
}

BOOL CShelfCodeGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CShelfCodeGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShelfCodeGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShelfCodeGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CShelfCodeGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShelfCodeGetterDlg message handlers

BOOL CShelfCodeGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("ShelfCodeGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : ShelfCodeGetterDlg") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CShelfCodeGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		FindShelfCode();
}

BEGIN_EVENTSINK_MAP(CShelfCodeGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CShelfCodeGetterDlg)
	ON_EVENT(CShelfCodeGetterDlg, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CShelfCodeGetterDlg::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CShelfCodeGetterDlg::SetGridReverse()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_ShelfCodeGetterDlg") );
	if( !pDM->GetRowCount() )
		return;
	
	CESL_ControlMgr *pCM = FindCM( _T("CM_ShelfCodeGetterDlg") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	m_DMRow = pGrid->GetRow() - 1;

	pGrid->SetReverse( m_DMRow );
}

VOID CShelfCodeGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	if( -1 == m_DMRow )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		return;
	}
	
	SetCode();
		
	CDialog::OnOK();
}

VOID CShelfCodeGetterDlg::SetCode()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_ShelfCodeGetterDlg") );
	pDM->GetCellData( _T("CODE"), m_DMRow, m_code );
}

VOID CShelfCodeGetterDlg::FindShelfCode()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_ShelfCodeGetterDlg") );
	pDM->RefreshDataManager( _T("") );
	ControlDisplay( _T("CM_ShelfCodeGetterDlg"), _T("그리드") );
	m_DMRow = -1;
}


HBRUSH CShelfCodeGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
