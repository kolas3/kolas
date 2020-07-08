// CENTER_UPLOAD_PRINT.cpp : implementation file
//

#include "stdafx.h"
#include "CENTER_UPLOAD_PRINT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_PRINT dialog


CCENTER_UPLOAD_PRINT::CCENTER_UPLOAD_PRINT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_PRINT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}

CCENTER_UPLOAD_PRINT::~CCENTER_UPLOAD_PRINT()
{
}

BOOL CCENTER_UPLOAD_PRINT::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CCENTER_UPLOAD_PRINT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCENTER_UPLOAD_PRINT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCENTER_UPLOAD_PRINT, CDialog)
	//{{AFX_MSG_MAP(CCENTER_UPLOAD_PRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_PRINT message handlers

BOOL CCENTER_UPLOAD_PRINT::OnInitDialog() 
{
	CDialog::OnInitDialog();

    if (InitESL_Mgr(_T("SM_센터업로드_출력")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
    INT m_nMode = 0 ;
	CRect PrePos;
	GetDlgItem(IDC_RICH_CENTERUPLOAD)->GetWindowRect(PrePos);
	ScreenToClient(PrePos);
	if( m_nMode ==0 )
		GetDlgItem(IDC_RICH_CENTERUPLOAD)->MoveWindow(PrePos.left,15,PrePos.Width(),PrePos.Height());
    
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCENTER_UPLOAD_PRINT::OnOK() 
{
	// TODO: Add extra validation here
    CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0201") );    
 
    CESL_ControlMgr *pCM = FindCM(_T("CM_CENTERUPLOAD_01")) ;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid")) ;

	pGrid->SelectMakeList () ;
    INT nRowCnt = pGrid->SelectGetCount () ;
    
	INT m_idx ,ids ; 
	CString strMarc ;
    
	for(INT i =0 ; i < nRowCnt ;i++ )
	{
		m_idx = pGrid->SelectGetIdx () ;
		ids = GetDataMgrData (_T("DM_CENTERUPLOAD_01"),_T("MARC"),strMarc, m_idx ) ;
        ids = pDM->InsertRow (-1) ;
		ids = pDM->SetCellData (_T("BS_MARC"),strMarc,i) ;
		pGrid->SelectGetNext () ;
	}


	m_marcEditor.SubclassDlgItem(IDC_RICH_CENTERUPLOAD, -1, this);
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	//m_marcEditor.Display();

	INT iRowCount = pDM->GetRowCount();
	for(  i = 0 ; i <= iRowCount-1 ; i++ ) {
		CString sStreamMarc;
		CMarc marc;
		pDM->GetCellData( _T("BS_MARC"), i ,sStreamMarc);
		m_pInfo->pMarcMgr->Decoding( sStreamMarc, &m_marc );
		m_marcEditor.Display();

		CString sEditMarc;		
		
		GetDlgItem(IDC_RICH_CENTERUPLOAD)->GetWindowText( sEditMarc );
		
		CString str;
		str.Format(_T("%c"), SUBFIELD_CODE);
		sEditMarc.Replace(str, _T("▼"));

		str.Format(_T("%c"), FIELD_TERMINATOR);
		sEditMarc.Replace(str, _T("▲"));

		sEditMarc += _T("\r\n────────────────────────────────────────");


		pDM->SetCellData( _T("EDITMARC"), sEditMarc, i );
		
	
	}	

	
	//출력
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	
	
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("CO_UPLAOD_PRINT"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}

	pSIReportManager->SetDataMgr(0, pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
	//CDialog::OnOK();
}

VOID CCENTER_UPLOAD_PRINT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
INT CCENTER_UPLOAD_PRINT::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
}
HBRUSH CCENTER_UPLOAD_PRINT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
