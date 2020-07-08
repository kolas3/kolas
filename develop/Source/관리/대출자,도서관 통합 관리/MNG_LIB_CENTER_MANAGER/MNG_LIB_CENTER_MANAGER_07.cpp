// MNG_LIB_CENTER_MANAGER_07.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_07.h"
#include "MNG_LIB_CENTER_MANAGER_06.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_07 dialog


CMNG_LIB_CENTER_MANAGER_07::CMNG_LIB_CENTER_MANAGER_07(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_07)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_LIB_CENTER_MANAGER_07::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_LIB_CENTER_MANAGER_07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_07)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_07, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_07)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_07 message handlers

BOOL CMNG_LIB_CENTER_MANAGER_07::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_LIB_CENTER_MANAGER_07") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_CENTER_MANAGER_07") );
		return false;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIB_CENTER_MANAGER_07::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	ViewLibList();
}

VOID CMNG_LIB_CENTER_MANAGER_07::ViewLibList()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_07") );
	pDM->RefreshDataManager( _T("") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	
	pCombo->InsertString( 0, _T("☞ 선택") );
	CString lib;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("NAME"), i, lib );
		pCombo->InsertString( i + 1, lib );
	}
	pCombo->SetCurSel( 0 );
}

VOID CMNG_LIB_CENTER_MANAGER_07::OnOK() 
{
	// TODO: Add extra validation here
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	if( !pCombo->GetCurSel() )
	{
		AfxMessageBox( _T("소속 도서관을 선택하여 주십시오") );
		return;
	}
	
	const INT fieldCnt = 13;
	CString data[ fieldCnt ];
	GetLibData( data );

	AddDM( data );
	ShowGrid();
	
	CDialog::OnOK();
}

VOID CMNG_LIB_CENTER_MANAGER_07::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CMNG_LIB_CENTER_MANAGER_07::GetLibData( CString data[] )
{
	CEdit *pEdt;
	const INT edtCnt = 4;
	UINT edtID[ edtCnt ] = { IDC_EDIT16, IDC_EDIT17, IDC_EDIT18, IDC_EDIT19 };
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}
	
	data[ 4 ] = GetCurrentDate();
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_07") );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	
	data[ 5 ] = GetWorkLogMsg();
	pDM->GetCellData( _T("REC_KEY"), pCombo->GetCurSel() - 1, data[ 6 ] );
	data[ 7 ] = data[ 6 ];
	pDM->GetCellData( _T("NAME"), pCombo->GetCurSel() - 1, data[ 8 ] );
	pDM->MakeRecKey( data[ 9 ] );
}

CString CMNG_LIB_CENTER_MANAGER_07::GetCurrentDate()
{
	CString date;
	
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( IDC_DATETIMEPICKER2 );
	CTime time;
	pDate->GetTime( time );
	
	date.Format( _T("%d/%d/%d"), time.GetYear(), time.GetMonth(), time.GetDay() );	
	return date;
}

VOID CMNG_LIB_CENTER_MANAGER_07::AddDM(CString data[])
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_06") );
	
	const INT aliasCnt = 10;
	CString alias[ aliasCnt ] = {
		_T("USER_NO"), 		//	0
		_T("USER_ID"), 		//	1
		_T("PASSWORD"), 	//	2
		_T("FID_KEY"), 	//	3
		_T("MAKE_DATE"), 	//	4
		_T("FIRST_WORK"), 	//	5
		_T("L_REC_KEY"), 	//	6
		_T("LIB_KEY"), 		//	7
		_T("L_NAME"), 		//	8
		_T("REC_KEY") };	//	9

	pDM->SetCellData( alias, aliasCnt, data, -1 );
}

VOID CMNG_LIB_CENTER_MANAGER_07::ShowGrid()
{
	CMNG_LIB_CENTER_MANAGER_06 *pParent = ( CMNG_LIB_CENTER_MANAGER_06 * )pParentMgr;
	pParent->ShowGrid();
}
