// RECEIPT_PRINT_SET_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RECEIPT_PRINT_SET_DLG.h"
#include "lprn_api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRECEIPT_PRINT_SET_DLG dialog


CRECEIPT_PRINT_SET_DLG::CRECEIPT_PRINT_SET_DLG(CESL_Mgr* pParent, CLonaManageValue *pLMV/*=NULL*/)
	: CESL_Mgr(CRECEIPT_PRINT_SET_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRECEIPT_PRINT_SET_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLMV = pLMV;
}

CRECEIPT_PRINT_SET_DLG::~CRECEIPT_PRINT_SET_DLG()
{
}
VOID CRECEIPT_PRINT_SET_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRECEIPT_PRINT_SET_DLG)
	DDX_Control(pDX, IDC_EDT_NOTIFY, m_edtNotify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRECEIPT_PRINT_SET_DLG, CDialog)
	//{{AFX_MSG_MAP(CRECEIPT_PRINT_SET_DLG)
	ON_BN_CLICKED(IDC_btnLOANPRINT, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnOK, OnbtnOK)
	ON_BN_CLICKED(IDC_btnLocloanCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnCUT_RECEIPT, OnbtnCUTRECEIPT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRECEIPT_PRINT_SET_DLG message handlers

BOOL CRECEIPT_PRINT_SET_DLG::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CRECEIPT_PRINT_SET_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	CString	strFlag;
	CString	strbarcode, strbarcodeAll;		
	CString	strItem;
	CString	strMode;
	CString	strDevice;
	CString strPreview;
	CString strNotify;

	m_edtNotify.SetLimitText(42);
		
	m_pLMV->GetReceiptManageValue(strbarcode, strbarcodeAll, strItem, strMode, strDevice, strPreview, strNotify );	

	if ( strMode == _T("0") )
	{
		(( CButton*) GetDlgItem (IDC_rMODE1) )->SetCheck(TRUE);
	}
	else if ( strMode == _T("1") )
	{
		(( CButton*) GetDlgItem (IDC_rMODE2) )->SetCheck(TRUE);
	}
	else if ( strMode == _T("2") )
	{
		(( CButton*) GetDlgItem (IDC_rMODE3) )->SetCheck(TRUE);
	}
	else
	{
		(( CButton*) GetDlgItem (IDC_rMODE4) )->SetCheck(TRUE);
	}
	
	(( CEdit*) GetDlgItem (IDC_edtBARCODE) )->SetWindowText(strbarcode);
	(( CEdit*) GetDlgItem (IDC_edtBARCODE2) )->SetWindowText(strbarcodeAll);
	
	if ( strItem==_T("Y") )
	{
		(( CButton*) GetDlgItem (IDC_rDETAIL_Y) )->SetCheck(TRUE);		
		(( CButton*) GetDlgItem (IDC_rDETAIL_N) )->SetCheck(FALSE);		
	}
	else
	{
		(( CButton*) GetDlgItem (IDC_rDETAIL_Y) )->SetCheck(FALSE);		
		(( CButton*) GetDlgItem (IDC_rDETAIL_N) )->SetCheck(TRUE);		
	}

	( (CEdit*) GetDlgItem (IDC_edtBARCODE) )->SetLimitText(12);

	GetManageValue(_T("열람"),_T("공통"),_T("영수증출력"),_T("영수증출력사용형태"),m_strPrintTypeSet);

	if ( m_strPrintTypeSet.GetAt(0) == 'Y' ) 
	{
		(( CButton*) GetDlgItem (IDC_chkLOAN_RECEIPT))->SetCheck(1);
	}
	else 
	{
		(( CButton*) GetDlgItem (IDC_chkLOAN_RECEIPT))->SetCheck(0);
	}
		
	if ( m_strPrintTypeSet.GetAt(1) == 'Y' )
	{
		(( CButton*) GetDlgItem (IDC_chkRETURN_RECEIPT))->SetCheck(1);		
	}
	else
	{
		(( CButton*) GetDlgItem (IDC_chkRETURN_RECEIPT))->SetCheck(0);		
	}

	
	if ( strDevice == _T("S") ) 
	{
		(( CButton*) GetDlgItem (IDC_rSINGLE) )->SetCheck(1);		
		(( CButton*) GetDlgItem (IDC_rDUAL) )->SetCheck(0);		
	}
	else
	{
		(( CButton*) GetDlgItem (IDC_rSINGLE) )->SetCheck(0);		
		(( CButton*) GetDlgItem (IDC_rDUAL) )->SetCheck(1);		
	}
	
	if ( strPreview == _T("Y") )
	{
		(( CButton*) GetDlgItem (IDC_chkPREVIEW) )->SetCheck(1);
	}
	else
	{
		(( CButton*) GetDlgItem (IDC_chkPREVIEW) )->SetCheck(0);
	}
	
	if ( !strNotify.IsEmpty() )
	{
		strNotify = strNotify.Left(42);
		(( CEdit*) GetDlgItem ( IDC_EDT_NOTIFY ) )->SetWindowText( strNotify );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CRECEIPT_PRINT_SET_DLG::OnbtnPRINT() 
{	
	CSIReportManager *pSIReportManager;
	CSIReportObject_Barcode *pBarcode;
	CSIReportObject_Barcode *pBarcode2;
	CSIReportWorkSheet		*pSheet;
	CString					strBarcode, strBarcode2;	
	INT						ids;

	pSIReportManager = new CSIReportManager(this);
	if ( pSIReportManager == NULL ) 
	{
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;	

	if (&pSIReportManager == NULL) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	pSIReportManager->SetCONNECTION_INFO(m_pRctLogDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("영수증출력바코드"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}		
	pSheet = pSIReportManager->GetSheet(0);	
	pBarcode = pSIReportManager->FindBarcode(pSheet, _T("BARCODE"));	

	(( CEdit*) GetDlgItem (IDC_edtBARCODE) )->GetWindowText(strBarcode);
	pBarcode->m_strCaption = strBarcode;		

	pBarcode2 = pSIReportManager->FindBarcode(pSheet, _T("BARCODE2"));	
	(( CEdit*) GetDlgItem (IDC_edtBARCODE2) )->GetWindowText(strBarcode2);
	pBarcode2->m_strCaption = strBarcode2;		

	ids = pSIReportManager->Print();

	if (&pSIReportManager != NULL ) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
}

VOID CRECEIPT_PRINT_SET_DLG::OnbtnOK() 
{
	CString	strbarcode,strbarcodeAll;	
	CString	strItem;
	CString	strMode;
	CString strDevice;
	CString strPreview;

	CString strNotify;

	(( CEdit*) GetDlgItem ( IDC_EDT_NOTIFY ) )->GetWindowText(strNotify);

	BOOL	bCheck;
	(( CEdit*) GetDlgItem (IDC_edtBARCODE) )->GetWindowText(strbarcode);
	(( CEdit*) GetDlgItem (IDC_edtBARCODE2) )->GetWindowText(strbarcodeAll);
	
	bCheck = (( CButton*) GetDlgItem (IDC_rDETAIL_Y) )->GetCheck();
	if ( bCheck == TRUE ) strItem=_T("Y");
	else strItem=_T("N");
	
	bCheck = (( CButton*) GetDlgItem (IDC_rMODE1) )->GetCheck();
	if ( bCheck == TRUE ) strMode = _T("0");
	bCheck = (( CButton*) GetDlgItem (IDC_rMODE2) )->GetCheck();
	if ( bCheck == TRUE ) strMode = _T("1");	
	bCheck = (( CButton*) GetDlgItem (IDC_rMODE3) )->GetCheck();
	if ( bCheck == TRUE ) strMode = _T("2");	
	bCheck = (( CButton*) GetDlgItem (IDC_rMODE4) )->GetCheck();
	if ( bCheck == TRUE ) strMode = _T("3");

	bCheck = (( CButton*) GetDlgItem (IDC_rSINGLE) )->GetCheck();
	if ( bCheck == TRUE ) strDevice = _T("S");
	else strDevice = _T("D");

	bCheck = (( CButton*) GetDlgItem (IDC_chkPREVIEW) )->GetCheck();
	if ( bCheck == TRUE ) strPreview = _T("Y");
	else strPreview = _T("N");


	m_pLMV->SetReceiptManageValue( strbarcode, strbarcodeAll, strItem, strMode, strDevice, strPreview, strNotify );
	
	m_strPrintTypeSet=_T("");
	bCheck = (( CButton*) GetDlgItem (IDC_chkLOAN_RECEIPT))->GetCheck();
	if ( bCheck == TRUE )
	{
		m_strPrintTypeSet=_T("Y");
	}
	else 
	{
		m_strPrintTypeSet=_T("N");
	}

	bCheck = (( CButton*) GetDlgItem (IDC_chkRETURN_RECEIPT))->GetCheck();		
	if ( bCheck == TRUE )
	{
		m_strPrintTypeSet += _T("Y");
	}
	else
	{
		m_strPrintTypeSet += _T("N");
	}

	SetManageValue(_T("열람"),_T("공통"),_T("영수증출력"),_T("영수증출력사용형태"),m_strPrintTypeSet);	
	
	AfxMessageBox(_T("저장되었습니다."));
}

VOID CRECEIPT_PRINT_SET_DLG::OnbtnCLOSE() 
{
	CDialog::OnCancel();	
}

INT CRECEIPT_PRINT_SET_DLG::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}

VOID CRECEIPT_PRINT_SET_DLG::OnbtnCUTRECEIPT() 
{
	LP_StartUp();
	LP_PrintCut( );	
	LP_CleanUp();
}

HBRUSH CRECEIPT_PRINT_SET_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
