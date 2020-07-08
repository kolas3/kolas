// DlgSIReportSample.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSIReportSample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSIReportSample dialog


CDlgSIReportSample::CDlgSIReportSample(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSIReportSample)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CDlgSIReportSample::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CDlgSIReportSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSIReportSample)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSIReportSample, CDialog)
	//{{AFX_MSG_MAP(CDlgSIReportSample)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSIReportSample message handlers

BOOL CDlgSIReportSample::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SIReport_Sample")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDlgSIReportSample::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CDlgSIReportSample::OnbSEARCH() 
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_SIReport_Sample"));
	if (pDM == NULL) return;

	ids = pDM->RefreshDataManager(_T("ID IS NOT NULL ORDER BY ID"));
	if (ids < 0) return;

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("직원직급"), _T("GRADE"));

	AllControlDisplay(_T("CM_SIReport_Sample"), -1);
}

VOID CDlgSIReportSample::OnbPRINT() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_SIReport_Sample"));
	if (pDM == NULL) return;

	if (m_strRptName.GetLength() == 0) m_strRptName = _T("sireporttest2.rpt");
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("SI REPORT TEST"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, pDM, 0);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
}

INT CDlgSIReportSample::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	CString msg;
	msg.Format(_T("- %d/%d -"), EventInfo->m_nPage, EventInfo->m_nTotalPage);


	//INT horz_align;							// 정렬(왼쪽:ALIGN_LEFT, 중앙:ALIGN_HCENTER, 오른쪽:ALIGN_RIGHT)
	//INT vert_align;							// 정렬(위:ALIGN_TOP, 가운데:ALIGN_VCENTER, 아래:ALIGN_BOTTOM)
	//pSIReportManager->SetTableCellText(_T("페이지번호"), 0, 0, msg, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pSIReportManager->SetTableCellText(_T("페이지번호"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);
	if (EventInfo->m_nPage == 1) pSIReportManager->SetBarcodeCaption(_T("바코드"), _T("123"));
	if (EventInfo->m_nPage == 2) pSIReportManager->SetBarcodeCaption(_T("바코드"), _T("123456"));
	if (EventInfo->m_nPage == 3) pSIReportManager->SetBarcodeCaption(_T("바코드"), _T("123456789"));
	return 0;
}



