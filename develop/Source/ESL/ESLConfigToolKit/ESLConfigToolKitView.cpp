// ESLConfigToolKitView.cpp : implementation of the CESLConfigToolKitView class
//

#include "stdafx.h"
#include "ESLConfigToolKit.h"

#include "ESLConfigToolKitDoc.h"
#include "ESLConfigToolKitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitView

IMPLEMENT_DYNCREATE(CESLConfigToolKitView, CFormView)

BEGIN_MESSAGE_MAP(CESLConfigToolKitView, CFormView)
	//{{AFX_MSG_MAP(CESLConfigToolKitView)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_DM_USER_VIEW_CONFIG, ShowDlg_DMUserViewConfig)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitView construction/destruction

CESLConfigToolKitView::CESLConfigToolKitView()
	: CFormView(CESLConfigToolKitView::IDD)
{
	//{{AFX_DATA_INIT(CESLConfigToolKitView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	m_pDlg_DMUserViewConfig = NULL;
	ReadCfgFile.ReadCfgFile(_T(""));
	m_bkR = 255;
	m_bkG = 255;
	m_bkB = 255;
}

CESLConfigToolKitView::~CESLConfigToolKitView()
{
	FreeDlg_DMUserViewConfig();
}

VOID CESLConfigToolKitView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLConfigToolKitView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CESLConfigToolKitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

VOID CESLConfigToolKitView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitView printing

BOOL CESLConfigToolKitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

VOID CESLConfigToolKitView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

VOID CESLConfigToolKitView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

VOID CESLConfigToolKitView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitView diagnostics

#ifdef _DEBUG
VOID CESLConfigToolKitView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CESLConfigToolKitView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CESLConfigToolKitDoc* CESLConfigToolKitView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CESLConfigToolKitDoc)));
	return (CESLConfigToolKitDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitView message handlers

INT CESLConfigToolKitView::ShowDlg_DMUserViewConfig(VOID)
{
	FreeDlg_DMUserViewConfig();
	m_pDlg_DMUserViewConfig = new CDlg_DMUserViewConfig();

	m_pDlg_DMUserViewConfig->m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	m_pDlg_DMUserViewConfig->m_sSEQ = ReadCfgFile.m_sSequence;
	GetClientRect(&m_pDlg_DMUserViewConfig->m_rect);
	
	
	m_pDlg_DMUserViewConfig->Create(IDD_DM_USER_VIEW_CONFIG, this);
	m_pDlg_DMUserViewConfig->ShowWindow(TRUE);
	m_pDlg_DMUserViewConfig->MoveWindow( m_pDlg_DMUserViewConfig->m_rect );
	return 0;
}	

INT CESLConfigToolKitView::FreeDlg_DMUserViewConfig(VOID)
{
	if (m_pDlg_DMUserViewConfig) {
		delete m_pDlg_DMUserViewConfig;
		m_pDlg_DMUserViewConfig = NULL;
	}
	return 0;
}

VOID CESLConfigToolKitView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(&rect);
	
	if (m_pDlg_DMUserViewConfig) {
		m_pDlg_DMUserViewConfig->MoveWindow(&rect);
	}

	CRect rect2, rect3;
	CWnd *pWnd = GetDlgItem(IDC_STATIC1);
	if (rect.bottom - rect.top > 0 && rect.right - rect.left > 0) {
		if (pWnd) {
			pWnd->GetWindowRect(&rect2);
			rect3.top = ((rect.bottom-rect.top) - (rect2.bottom - rect2.top))/2;
			rect3.bottom = rect3.top + (rect2.bottom - rect2.top);
			rect3.left = ((rect.right - rect.left) - (rect2.right - rect2.left)) /2;
			rect3.right = rect3.left + (rect2.right - rect2.left);
				
			pWnd->MoveWindow(rect3);
		}
	}
}

HBRUSH CESLConfigToolKitView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/*
	if (pWnd->GetDlgCtrlID() == IDD_ESLCONFIGTOOLKIT_FORM) {
		pDC->SetBkMode(OPAQUE );
		pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
		return m_BkBrush;
	}
	*/
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
