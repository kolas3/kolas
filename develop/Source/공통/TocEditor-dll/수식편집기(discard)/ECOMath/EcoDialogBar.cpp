// EcoDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "ecomath.h"
#include "EcoDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcoDialogBar dialog


CEcoDialogBar::CEcoDialogBar(CWnd* pParent /*=NULL*/)
//	: CInitDialogBar(CEcoDialogBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEcoDialogBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CEcoDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEcoDialogBar)
	DDX_Control(pDX, IDC_SUMMATION_BTN, m_SumBtn);
	DDX_Control(pDX, IDC_SCRIPT_BTN, m_ScriptBtn);
	DDX_Control(pDX, IDC_RADICAL_BTN, m_RadicalBtn);
	DDX_Control(pDX, IDC_SET_BTN, m_SetBtn);
	DDX_Control(pDX, IDC_OP_BTN, m_OpBtn);
	DDX_Control(pDX, IDC_BIREL_BTN, m_BirelBtn);
	DDX_Control(pDX, IDC_UPGREEK_BTN, m_UpgreekBtn);
	DDX_Control(pDX, IDC_MISC_BTN, m_MiscBtn);
	DDX_Control(pDX, IDC_LOGICAL_BTN, m_LogicalBtn);
	DDX_Control(pDX, IDC_GREEK_BTN, m_GreekBtn);
	DDX_Control(pDX, IDC_FRACTION_BTN, m_FractionBtn);
	DDX_Control(pDX, IDC_FENCE_TEMP_BTN, m_FenceBtn);
	DDX_Control(pDX, IDC_BAR_BTN, m_BarBtn);
	DDX_Control(pDX, IDC_ARROW_BTN, m_ArrowsBtn);
	DDX_Control(pDX, IDC_INTEGRAL_BTN, m_IntegralBtn);
	//}}AFX_DATA_MAP

	CRgn rgn;
	rgn.CreateEllipticRgn(5,5, 75, 45);
	m_IntegralBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(INTEGRALU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(INTEGRALD)),
						      FALSE, &rgn);

	m_ArrowsBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ARROWSU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(ARROWSD)),
						      FALSE, &rgn);

	m_BarBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(BARU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(BARD)),
						      FALSE, &rgn);

	m_FenceBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(FENCEU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(FENCED)),
						      FALSE, &rgn);

	m_FractionBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(FRACTIONU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(FRACTIOND)),
						      FALSE, &rgn);

	m_GreekBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(GREEKU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(GREEKD)),
						      FALSE, &rgn);

	m_LogicalBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(LOGICALU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(LOGICALD)),
						      FALSE, &rgn);

	m_MiscBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(MISCU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(MISCD)),
						      FALSE, &rgn);

	m_UpgreekBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(UPGREEKU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(UPGREEKD)),
						      FALSE, &rgn);

	m_BirelBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(RELATIONU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(RELATIOND)),
						      FALSE, &rgn);

	m_OpBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(OPERATORU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(OPERATORD)),
						      FALSE, &rgn);

	m_SetBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(SETU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(SETD)),
						      FALSE, &rgn);

	m_RadicalBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(RADICALU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(RADICALD)),
						      FALSE, &rgn);

	m_ScriptBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(SCRIPTU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(SCRIPTD)),
						      FALSE, &rgn);

	m_SumBtn.wSetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(SUMU)), 
				   		     ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(SUMD)),
						      FALSE, &rgn);
}


BEGIN_MESSAGE_MAP(CEcoDialogBar, CInitDialogBar)
	//{{AFX_MSG_MAP(CEcoDialogBar)
	ON_BN_CLICKED(IDC_BIREL_BTN, OnBirelBtn)
	ON_BN_CLICKED(IDC_GREEK_BTN, OnGreekBtn)
	ON_BN_CLICKED(IDC_UPGREEK_BTN, OnUpgreekBtn)
	ON_BN_CLICKED(IDC_HEBREW_BTN, OnHebrewBtn)
	ON_BN_CLICKED(IDC_ARROW_BTN, OnArrowBtn)
	ON_BN_CLICKED(IDC_OP_BTN, OnOpBtn)
	ON_BN_CLICKED(IDC_LOGICAL_BTN, OnLogicalBtn)
	ON_BN_CLICKED(IDC_SET_BTN, OnSetBtn)
	ON_BN_CLICKED(IDC_MISC_BTN, OnMiscBtn)
	ON_BN_CLICKED(IDC_FENCE_TEMP_BTN, OnFenceTempBtn)
	ON_BN_CLICKED(IDC_FRACTION_BTN, OnFractionBtn)
	ON_BN_CLICKED(IDC_RADICAL_BTN, OnRadicalBtn)
	ON_BN_CLICKED(IDC_SCRIPT_BTN, OnScriptBtn)
	ON_BN_CLICKED(IDC_SUMMATION_BTN, OnSummationBtn)
	ON_BN_CLICKED(IDC_INTEGRAL_BTN, OnIntegralBtn)
	ON_BN_CLICKED(IDC_BAR_BTN, OnBarBtn)
	ON_BN_CLICKED(IDC_PRODUCT_BTN, OnProductBtn)
	ON_BN_CLICKED(IDC_SETTHEORY_BTN, OnSettheoryBtn)
	ON_BN_CLICKED(IDC_MATRIX_BTN, OnMatrixBtn)
	ON_UPDATE_COMMAND_UI(IDC_BIREL_BTN, OnUpdateBiRelBtn)
	ON_UPDATE_COMMAND_UI(IDC_OP_BTN, OnUpdateOpBtn)
	ON_UPDATE_COMMAND_UI(IDC_ARROW_BTN, OnUpdateArrowBtn)
	ON_UPDATE_COMMAND_UI(IDC_LOGICAL_BTN, OnUpdateLogicalBtn)
	ON_UPDATE_COMMAND_UI(IDC_SET_BTN, OnUpdateSetBtn)
	ON_UPDATE_COMMAND_UI(IDC_MISC_BTN, OnUpdateMiscBtn)
	ON_UPDATE_COMMAND_UI(IDC_GREEK_BTN, OnUpdateGreekBtn)
	ON_UPDATE_COMMAND_UI(IDC_UPGREEK_BTN, OnUpdateUpGreekBtn)
	ON_UPDATE_COMMAND_UI(IDC_HEBREW_BTN, OnUpdateHebrewBtn)
	ON_UPDATE_COMMAND_UI(IDC_FENCE_TEMP_BTN, OnUpdateFenceTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_FRACTION_BTN, OnUpdateFractionTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_RADICAL_BTN, OnUpdateRadicalTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_SCRIPT_BTN, OnUpdateScriptTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_SUMMATION_BTN, OnUpdateSummationTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_INTEGRAL_BTN, OnUpdateIntegralTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_BAR_BTN, OnUpdateBarTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_PRODUCT_BTN, OnUpdateProductTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_SETTHEORY_BTN, OnUpdateSetTheoryTemplateBtn)
	ON_UPDATE_COMMAND_UI(IDC_MATRIX_BTN, OnUpdateMatrixTemplateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcoDialogBar message handlers

VOID CEcoDialogBar::OnUpdateBiRelBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateOpBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateLogicalBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateGreekBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateUpGreekBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateHebrewBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateArrowBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateSetBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateMiscBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateFenceTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateFractionTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateRadicalTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateScriptTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateSummationTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateIntegralTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateBarTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateLabeledArrowTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateProductTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateSetTheoryTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnUpdateMatrixTemplateBtn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

VOID CEcoDialogBar::OnBirelBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_BIREL_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);
}

VOID CEcoDialogBar::OnGreekBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_GREEK_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);	
}

VOID CEcoDialogBar::OnUpgreekBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_UPGREEK_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnHebrewBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	UpdateData();

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_HEBREW_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);	
}

VOID CEcoDialogBar::OnArrowBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_ARROW_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnOpBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_BIOP_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnLogicalBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_LOGICAL_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_SET_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);	
}

VOID CEcoDialogBar::OnMiscBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_MISC_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnFenceTempBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_FENCE_TEMPLATE_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnFractionBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_FRACTION_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnRadicalBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_RADICAL_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnScriptBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_SCRIPT_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnSummationBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_SUMMATION_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnIntegralBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	//UpdateData();

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_INTEGRAL_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);	
}

VOID CEcoDialogBar::OnBarBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_BAR_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnLabeledarrowBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_LABELEDARROW_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnProductBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_PRODUCT_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);			
}

VOID CEcoDialogBar::OnSettheoryBtn() 
{
	// TODO: Add your control notification handler code here
	CString csWidth = _T("0");
	BOOL	bShow4All = TRUE;
	BOOL	bShowBmp = TRUE;
	INT		nStretch = 1;

	HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VERTBMP));

	CBmpMenu oMenu(_ttoi(csWidth.GetBuffer(0)), bShow4All, bShowBmp?hBitmap:0, (nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_SETTHEORY_MENU);

	CPoint pt;
	GetCursorPos(&pt);
	oMenu.TrackPopupMenu(0, pt.x, pt.y, this);		
}

VOID CEcoDialogBar::OnMatrixBtn() 
{
	// TODO: Add your control notification handler code here
	
}