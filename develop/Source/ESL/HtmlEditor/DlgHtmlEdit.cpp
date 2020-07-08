// DlgHtmlEdit.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgHtmlEdit.h"
#include "DlgInsertTable.h"
#include "HrProperties.h"
#include "TableProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHtmlEdit dialog
// extern "C" __declspec(dllimport)
int CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* ptm, int nFontType, LPARAM lParam);

__declspec(dllexport) BOOL CreateHtmlEditor(CDlgHtmlEdit* pDlgHtmlEdit, CWnd* pParent)
{
	pDlgHtmlEdit->Create(IDD_MAIN_DIALOG, pParent);
	return TRUE;
}

CDlgHtmlEdit::CDlgHtmlEdit(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAIN_DIALOG, pParent)
{
	//{{AFX_DATA_INIT(CDlgHtmlEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pi_Editor = NULL;
	m_nTabMode = 0;
}


void CDlgHtmlEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHtmlEdit)
	DDX_Control(pDX, IDC_TAB, mCtrl_Tab);
	DDX_Control(pDX, IDC_STATIC_URL, mCtrl_StaticUrl);	
	DDX_Control(pDX, IDC_BUTTON_GOFORWARD, mCtrl_GoForward);
	DDX_Control(pDX, IDC_BUTTON_GOBACK, mCtrl_GoBack);
	DDX_Control(pDX, IDC_BUTTON_LOAD, mCtrl_Load);
	DDX_Control(pDX, IDC_EDIT_URL, mCtrl_EditUrl);
	//
	DDX_Control(pDX, IDC_BUTTON_SAVE, mCtrl_Save);
	DDX_Control(pDX, IDC_BUTTON_HELP, mCtrl_Help);
	DDX_Control(pDX, IDC_BUTTON_BACKCOLOR, mCtrl_BgText);
	DDX_Control(pDX, IDC_BUTTON_BGCOLOR, mCtrl_BgBody);
	DDX_Control(pDX, IDC_BUTTON_BGCOLOR_CELL, mCtrl_BgCell);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, mCtrl_Clear);
	DDX_Control(pDX, IDC_BUTTON_COL_LEFT, mCtrl_ColLeft);
	DDX_Control(pDX, IDC_BUTTON_COL_RIGHT, mCtrl_ColRight);
	DDX_Control(pDX, IDC_BUTTON_COMBINE, mCtrl_Combine);
	DDX_Control(pDX, IDC_BUTTON_DELETE_COL, mCtrl_DeleteCol);
	DDX_Control(pDX, IDC_BUTTON_DELETE_ROW, mCtrl_DeleteRow);
	DDX_Control(pDX, IDC_BUTTON_FORECOLOR, mCtrl_ForeColor);	
	DDX_Control(pDX, IDC_BUTTON_HR, mCtrl_Hr);
	DDX_Control(pDX, IDC_BUTTON_HRPROP, mCtrl_HrProp);
	DDX_Control(pDX, IDC_BUTTON_IMAGE, mCtrl_Image);
	DDX_Control(pDX, IDC_BUTTON_LINK, mCtrl_Link);
	DDX_Control(pDX, IDC_BUTTON_NOBR, mCtrl_NoBr);
	DDX_Control(pDX, IDC_BUTTON_REMOVEFORMAT, mCtrl_RemoveFormat);
	DDX_Control(pDX, IDC_BUTTON_ROW_ABOVE, mCtrl_RowAbove);
	DDX_Control(pDX, IDC_BUTTON_ROW_BELOW, mCtrl_RowBelow);
	DDX_Control(pDX, IDC_BUTTON_SPLIT, mCtrl_Split);	
	DDX_Control(pDX, IDC_BUTTON_TABLE, mCtrl_Table);	
	DDX_Control(pDX, IDC_BUTTON_TABLEPROP, mCtrl_TableProp);	
	DDX_Control(pDX, IDC_STATIC_EDITOR, mCtrl_StaticEditor);
	DDX_Control(pDX, IDC_CHECK_SOURCE, mCtrl_CheckSource);
	DDX_Control(pDX, IDC_CHECK_DESIGN, mCtrl_CheckDesign);
	DDX_Control(pDX, IDC_CHECK_BOLD, mCtrl_CheckBold);
	DDX_Control(pDX, IDC_CHECK_ITALIC, mCtrl_CheckItalic);
	DDX_Control(pDX, IDC_CHECK_UNDERLINE, mCtrl_CheckUnderline);
	DDX_Control(pDX, IDC_CHECK_LEFTALIGN, mCtrl_CheckLeftAlign);
	DDX_Control(pDX, IDC_CHECK_CENTERALIGN, mCtrl_CheckCenterAlign);
	DDX_Control(pDX, IDC_CHECK_RIGHTALIGN, mCtrl_CheckRightAlign);
	DDX_Control(pDX, IDC_CHECK_ORDERLIST, mCtrl_CheckOrderList);
	DDX_Control(pDX, IDC_CHECK_UNORDERLIST, mCtrl_CheckUnOrderList);
	DDX_Control(pDX, IDC_COMBO_FONTNAME, mCtrl_ComboFontName);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, mCtrl_ComboFontSize);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgHtmlEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgHtmlEdit)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_FONTNAME, OnComboSelchangeComboFontname)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTSIZE, OnSelchangeComboFontsize)
	ON_WM_TIMER()
	ON_CBN_EDITCHANGE(IDC_COMBO_FONTSIZE, OnEditchangeComboFontsize)
	ON_CBN_CLOSEUP(IDC_COMBO_FONTSIZE, OnCloseupComboFontsize)
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_WM_WINDOWPOSCHANGED()	
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHtmlEdit message handlers
BOOL CDlgHtmlEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitAllCtrl();
	mCtrl_CheckDesign.SetCheck(TRUE);

	// ############## RESIZING STUFF ###################
	
	// retrieve the origional size of Internet Explorer Control and main window
	// before any resizing took place (BEFORE assigning pi_Editor !!)
	mCtrl_StaticEditor.GetWindowRect(mk_OrgMsie);
	GetWindowRect(mk_OrgMain);

	// ############# END RESIZING STUFF ################


	pi_Editor = new CHtmlEditor();
	if (!pi_Editor->CreateEditor(&mCtrl_StaticEditor, this, FALSE, TRUE))
	{
		// 에러메시지
		TRACE(_T("Error creating CHtmlEditor"), _T("Error"), MB_ICONSTOP);
		CDialog::EndDialog(0);
		return TRUE;
	}

	// Fill the Font combo box
	EnumFontFamilies(GetDC()->m_hDC, 0, (FONTENUMPROC)EnumFontNameProc, (LPARAM)&mCtrl_ComboFontName);

//	CString s_Title;
//	GetWindowText(s_Title);
//	#ifdef _UNICODE
//		SetWindowText(s_Title + _T(" - Unicode"));
//	#else
//		SetWindowText(s_Title + _T(" - MultiByte"));
//	#endif	
	pi_Editor->ModifyStyle(WS_HSCROLL | ES_AUTOHSCROLL, ES_CENTER);
	return TRUE;	
}

void CDlgHtmlEdit::InitAllCtrl()
{
	mCtrl_Table.SetButtonStyle(mCtrl_Table.GetButtonStyle() | BS_OWNERDRAW);
	mCtrl_Table.LoadBitmaps(IDB_BITMAP_TABLE);
//	mCtrl_Table.SizeToContent();

	mCtrl_CheckUnderline.SetButtonStyle(mCtrl_CheckUnderline.GetButtonStyle() | BS_OWNERDRAW);
	mCtrl_CheckUnderline.LoadBitmaps(IDB_BITMAP_UNDERLINE);
//	mCtrl_CheckUnderline.SizeToContent();

	mCtrl_CheckBold.SetButtonStyle(mCtrl_CheckBold.GetButtonStyle() | BS_OWNERDRAW);
	mCtrl_CheckBold.LoadBitmaps(IDB_BITMAP_BOLD);	
//	mCtrl_CheckBold.SizeToContent();

	mCtrl_CheckItalic.SetButtonStyle(mCtrl_CheckItalic.GetButtonStyle() | BS_OWNERDRAW);
	mCtrl_CheckItalic.LoadBitmaps(IDB_BITMAP_ITALIC);		
//	mCtrl_CheckItalic.SizeToContent();

	mCtrl_ForeColor.SetButtonStyle(mCtrl_ForeColor.GetButtonStyle() | BS_OWNERDRAW);
	mCtrl_ForeColor.LoadBitmaps(IDB_BITMAP_FONTCOLOR);		
//	mCtrl_ForeColor.SizeToContent();

	mCtrl_BgText.SetButtonStyle(mCtrl_BgText.GetButtonStyle() | BS_OWNERDRAW);
	mCtrl_BgText.LoadBitmaps(IDB_BITMAP_FONTBGCOLOR);		
//	mCtrl_BgText.SizeToContent();

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = _T("편집모드");
	mCtrl_Tab.InsertItem(0, &item);
	item.pszText = _T("소스모드");
	mCtrl_Tab.InsertItem(1, &item);
}

HBRUSH CDlgHtmlEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor==CTLCOLOR_DLG)
	{
		hbr = CreateSolidBrush(RGB(255,255,255)); 	
		return hbr;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgHtmlEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnEraseBkgnd(pDC);
}

// received WM_COMMAND
void CDlgHtmlEdit::OnWmCommand(WPARAM wParam)
{
	// this table is just for demonstration
	CString s_NewTable = _T("<table border=1 bordercolor=#996633><tr><td colspan=2><div>1AB</div></td><td><div>1C</div></td><td><div>1D</div></td></tr><tr><td><div>2A</div></td><td><div>2B</div></td><td colspan=2><div>2CD</div></td></tr><tr><td><div>3A</div></td><td colspan=2><div>3BC</div></td><td><div>3D</div></td></tr></table>");

	cHtmlDocument *pi_Doc  = pi_Editor->GetDocument();
	cHtmlTableCell  i_Cell  = 0;
	cHtmlTable      i_Table = 0;
	BOOL            b_SelOK = TRUE;
	CString         s_Text;

	// ######## GET CELL #########

	switch (LOWORD(wParam))
	{
	case IDC_BUTTON_BGCOLOR_CELL:
	case IDC_BUTTON_COMBINE:
	case IDC_BUTTON_SPLIT:
	case IDC_BUTTON_DELETE_ROW:
	case IDC_BUTTON_DELETE_COL:
		i_Cell  = pi_Doc->GetSelectedCell(); // outputs error
		i_Table = i_Cell.GetParentTable();
		if (!i_Cell.Valid() || !i_Table.Valid())
			return;
	}

	switch (LOWORD(wParam))
	{
	
	// ##################  Control  ##################
	
	case IDC_BUTTON_CLEAR:
		pi_Editor->Clear();
		break;
	case IDC_BUTTON_SAVE:
		pi_Editor->GetDocument()->CleanUpTags(); // remove empty tags
		pi_Editor->Save();
		break;
	case IDC_BUTTON_LOAD:
		mCtrl_EditUrl.GetWindowText(s_Text);
		pi_Editor->Navigate(s_Text);
		break;
	case IDC_BUTTON_GOBACK:
		pi_Editor->GoBack();
		break;
	case IDC_BUTTON_GOFORWARD:
		pi_Editor->GoForward();
		break;
	case IDC_BUTTON_HELP:
		pi_Editor->NavigateToEmbeddedResource(_T("Html_Help"));
		break;
//	case IDC_CHECK_DESIGN:
//		mCtrl_CheckDesign.SetCheck(true);
//		mCtrl_CheckSource.SetCheck(false);
//		pi_Editor->SetDisplayMode(mCtrl_CheckSource.GetCheck(), 
//			                      mCtrl_CheckDesign.GetCheck());
//		break;
//	case IDC_CHECK_SOURCE:
//		mCtrl_CheckDesign.SetCheck(false);
//		mCtrl_CheckSource.SetCheck(true);
//		pi_Editor->SetDisplayMode(mCtrl_CheckSource.GetCheck(), 
//			                      mCtrl_CheckDesign.GetCheck());
//		break;
	
	// ################## Text formatting ##################

	case IDC_BUTTON_FORECOLOR:
		b_SelOK = OnButtonFontColor();
		break;
	case IDC_BUTTON_BACKCOLOR:
		b_SelOK = OnButtonFontBackColor();
		break;
	case IDC_BUTTON_REMOVEFORMAT:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_REMOVEFORMAT);
		break;
	case IDC_CHECK_BOLD:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_BOLD);
		break;
	case IDC_CHECK_ITALIC:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_ITALIC);
		break;
	case IDC_CHECK_UNDERLINE:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_UNDERLINE);
		break;
	case IDC_CHECK_LEFTALIGN:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_JUSTIFYLEFT);
		break;
	case IDC_CHECK_CENTERALIGN:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_JUSTIFYCENTER);
		break;
	case IDC_CHECK_RIGHTALIGN:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_JUSTIFYRIGHT);
		break;
	case IDC_CHECK_UNORDERLIST:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_UNORDERLIST);
		break;
	case IDC_CHECK_ORDERLIST:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_ORDERLIST);
		break;
	
	// ##################  Table  ##################

	case IDC_BUTTON_TABLE:
		// ---- comment out one of the following lines ! ----
//		b_SelOK = pi_Editor->GetDocument()->PasteIntoSelection(s_NewTable);
	 	//b_SelOK = pi_Editor->GetDocument()->InsertTable(2,2); // insert empty table 2 x 2 cells
		b_SelOK = InsertNewTable();
		break;
	case IDC_BUTTON_TABLEPROP:
		SetTableProperties();
		break;
	case IDC_BUTTON_ROW_ABOVE:
		pi_Doc->InsertNewTableRow(FALSE);
		break;
	case IDC_BUTTON_ROW_BELOW:
		pi_Doc->InsertNewTableRow(TRUE);
		break;
	case IDC_BUTTON_COL_LEFT:
		pi_Doc->InsertNewTableColumn(FALSE);
		break;
	case IDC_BUTTON_COL_RIGHT:
		pi_Doc->InsertNewTableColumn(TRUE);
		break;
	case IDC_BUTTON_DELETE_ROW:
		i_Table.DeleteRow(i_Cell.GetRowIndex());
		break;		
	case IDC_BUTTON_DELETE_COL:
		i_Table.DeleteColumn(i_Cell.GetColIndex());
		break;
	case IDC_BUTTON_BGCOLOR_CELL:
		i_Cell.SetBgColor(_T("#CC8855"));
		break;
	case IDC_BUTTON_COMBINE:
		if (!i_Cell.Combine())
			// 에러 메시지
			TRACE(_T("You can only combine cells which have a right neighbour."), _T("Error"), MB_ICONSTOP);
		break;		
	case IDC_BUTTON_SPLIT:
		i_Cell.Split();
		break;

	// ###############  other HTML stuff  ##################

	case IDC_BUTTON_BGCOLOR:
		pi_Doc->GetBody(TRUE).SetAttribute(_T("bgcolor"), _T("#FFDDAA"));
		break;
	case IDC_BUTTON_HR:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_HORIZONTALLINE);
		break;
	case IDC_BUTTON_HRPROP:
		SetHRproperties();
		break;
	case IDC_BUTTON_IMAGE:
		b_SelOK = pi_Editor->ExecSetCommand(IDM_IMAGE);
		break;
	case IDC_BUTTON_LINK:
		{
		cHtmlElement i_Sel = pi_Editor->GetDocument()->GetSelection(FALSE);
		if (!i_Sel.Valid()) b_SelOK = FALSE;
		else b_SelOK = pi_Editor->ExecSetCommand(IDM_HYPERLINK);
		break;
		}
	case IDC_BUTTON_NOBR:
		b_SelOK = pi_Doc->AddToSelection(_T("<Nobr>"), _T("</Nobr>"));
		break;
	}

	if (!b_SelOK) 
		// 에러메시지
		TRACE(_T("선택된 내용이나 커서의 위치가 올바르지 않습니다."), _T("Error"), MB_ICONSTOP);
}
// Handler for Windows Message WM_GETMINMAXINFO
// Main Window is about to be resized --> prevent illegal sizes
// Don't allow main window to become smaller than the origional size defined in Resources
// ptMinTrackSize requires WINDOW size
void CDlgHtmlEdit::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// ATTENTION:
	// This function is called once BEFORE OnInitDialog() is called !!!
	// At the first call of this function mk_OrgMain is not yet valid
	if (mk_OrgMain.Width() > 0)
	{
		lpMMI->ptMinTrackSize.x = mk_OrgMain.Width();
		lpMMI->ptMinTrackSize.y = mk_OrgMain.Height() *2 /3; // allow decreasing the height a little
	}	
	CDialog::OnGetMinMaxInfo(lpMMI);
}
void CDlgHtmlEdit::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else CDialog::OnPaint();
}

void CDlgHtmlEdit::OnDestroy(void)
{
	if (pi_Editor) pi_Editor->DestroyWindow();
	// delete pi_Editor is not required as DestroyWindow() does the job !
}

// Handler for Windows Message WM_WINDOWPOSCHANGED
// Called multiple times WHILE the user resizes or moves or maximizes the main window
// Do nothing here while resizing to avoid flickering
void CDlgHtmlEdit::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	// If main window was maximized or normalized --> resize MSIE
	static BOOL b_Zoomed = -1;
	if (b_Zoomed != IsZoomed())
	{
		b_Zoomed  = IsZoomed();

		if (!IsIconic()) OnExitSizeMove(0,0);
	}

	CDialog::OnWindowPosChanged(lpwndpos);
}

// Handler for Windows Message WM_EXITSIZEMOVE
// Called after the user has FINISHED to move or resize the main window
LRESULT CDlgHtmlEdit::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	if (!pi_Editor)
		return 0;

	CRect k_NewMain;
	GetWindowRect(k_NewMain);

	// IMPORTANT: Resize MSIE only if size has changed !!
	// Otherwise (afer a Move) MSIE would stay blank !
	static CSize k_Size(0,0);
	if (k_Size == k_NewMain.Size()) return 0;
	k_Size = k_NewMain.Size();

	CSize k_Diff = k_NewMain.Size() - (mk_OrgMain.Size() - mk_OrgMsie.Size());

	pi_Editor->MoveWindow(0, 0, k_Diff.cx, k_Diff.cy, SWP_NOMOVE);
	Invalidate(FALSE);
	return 0;
}

void CDlgHtmlEdit::OnSelchangeComboFontsize()
{
	SetTimer(ID_TIMER_COMBO_FONTSIZE, 200, 0);
}

void CDlgHtmlEdit::OnEditchangeComboFontsize() 
{
	SetTimer(ID_TIMER_COMBO_FONTSIZE, 200, 0);
}

void CDlgHtmlEdit::OnCloseupComboFontsize() 
{
	SetTimer(ID_TIMER_COMBO_FONTSIZE, 200, 0);
}

// called from ID_TIMER_COMBO_FONTSIZE
void CDlgHtmlEdit::OnUserChangedFontsize()
{
	CString s_Size;
	mCtrl_ComboFontSize.GetWindowText(s_Size);

	UINT u32_Size = _ttol(s_Size);
	// Do nothing after entering the first digit of a 2 digit number (1..3)
	if (u32_Size < 4 || u32_Size > 99) 
		return;

	// Do not use IDM_FONTSIZE here as it is very gross
	if (!pi_Editor->ExecSetCommand(IDM_FONTSIZE_STYLE, (int)u32_Size))
		MessageBox(_T("선택된 글자나 커서의 위치가 잘못 되었습니다."), _T("Error"), MB_ICONSTOP);
}

// Receives all clicks on buttons and checkboxes
LRESULT CDlgHtmlEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	if (message == WM_COMMAND)
	{
		// IDOK is sent when the user hits the Enter key 
		// (except if the cursor is in the Internet Explorer control)
		// Per default this would close the dialog, the application would shut down
		if (wParam == IDOK) 
		{
			// Did the user hit the Enter key while the cursor is in the URL Edit box ?
			if (::GetFocus() == mCtrl_EditUrl.m_hWnd)
			{
				OnWmCommand(IDC_BUTTON_LOAD); // Surf to the URL
			}
			return 0; // prevent this message from getting to CDialog::WindowProc()
		}

		OnWmCommand(wParam);
	}

	if (message == WM_IDLEUPDATECMDUI) OnUpdateUI (wParam);

	return CDialog::WindowProc(message, wParam, lParam);
}

// ################ FONTS ##################

// List all installed fonts into the combobox
int CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* ptm, int nFontType, LPARAM lParam)
{
	CComboBox* i_Combo = (CComboBox*) lParam;
	i_Combo->AddString(plf->lfFaceName);
	return TRUE;
}

// 폰트배경색 설정
BOOL CDlgHtmlEdit::OnButtonFontBackColor()
{	
	CColorDialog Dlg;
	CString strColor;
	COLORREF crNewFontBackColor;

	if(IDOK==Dlg.DoModal())
	{
		crNewFontBackColor = Dlg.GetColor();
		strColor.Format(_T("#%.2X%.2X%.2X"), 
			GetRValue(crNewFontBackColor), GetGValue(crNewFontBackColor), GetBValue(crNewFontBackColor));
		pi_Editor->ExecSetCommand(IDM_BACKCOLOR, LPCTSTR(strColor));
	}	
	return TRUE;
}

// 폰트색 설정
BOOL CDlgHtmlEdit::OnButtonFontColor()
{
	CColorDialog Dlg;
	CString strColor;
	COLORREF crNewFontColor;

	if(IDOK==Dlg.DoModal())
	{
		crNewFontColor = Dlg.GetColor();
		strColor.Format(_T("#%.2X%.2X%.2X"), 
			GetRValue(crNewFontColor), GetGValue(crNewFontColor), GetBValue(crNewFontColor));
		pi_Editor->ExecSetCommand(IDM_FORECOLOR, LPCTSTR(strColor));
	}	
	
	return TRUE;
}

void CDlgHtmlEdit::OnComboSelchangeComboFontname()
{
	CString s_Font;
	mCtrl_ComboFontName.GetWindowText(s_Font);

	if (!pi_Editor->ExecSetCommand(IDM_FONTNAME, s_Font.AllocSysString()))
		MessageBox(_T("선택된 글자나 커서의 위치가 잘못 되었습니다."), _T("Error"), MB_ICONSTOP);
}

HCURSOR CDlgHtmlEdit::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// This message is posted from cHtmlEditor::cMsieWnd
// Set the checkboxes according to the current selection / cursor position
// This function is called with every keypress of the user so it is speed optimized
void CDlgHtmlEdit::OnUpdateUI(WPARAM wParam)
{
	if (wParam == HTMLEDIT_INIT_DONE)
	{
		/*
		Put any code here you want to execute after the HTML Editor is ready initialized
		For example any code that loads content into the editor
		*/
		return;
	}

	// Called after every CleanUp (e.g. after Navigation)
	if (wParam == HTMLEDIT_SETDEFAULT)
	{
		cHtmlDocument *pi_Doc = pi_Editor->GetDocument();
		pi_Doc->SetDocumentDefault(_T("BODY"),  cHtmlStyle::E_FontSize,    _T("14"));
		pi_Doc->SetDocumentDefault(_T("TABLE"), cHtmlStyle::E_FontSize,    _T("14"));
		pi_Doc->SetDocumentDefault(_T("BODY"),  cHtmlStyle::E_FontFamily,  _T("Arial"));
		pi_Doc->SetDocumentDefault(_T("TABLE"), cHtmlStyle::E_FontFamily,  _T("Arial"));
		
		cHtmlElement i_Body = pi_Doc->GetBody(FALSE);
		if (i_Body.Valid())
		{
			
			i_Body.SetAttribute(_T("topMargin"),  _T("2"));
			i_Body.SetAttribute(_T("leftMargin"), _T("3"));			
	//		i_Body.SetAttribute(_T("style"), _T("overflow-x:hidden"));					
		}
		return;
	}

	if (wParam == HTMLEDIT_KILLFOCUS)
	{
		/*
		Put any code here you want to execute when the Html Editor loses focus
		For example any code to automatically save user changes
		*/
		return;
	}

	// Display the new URL in Edit box after a navigation took place
	// Replace Mime encoding ("%20" --> " ")
	if (wParam == HTMLEDIT_NAVIGATED)
	{
		mCtrl_EditUrl.SetWindowText(cMisc::DecodeMime(pi_Editor->GetDocument()->GetUrl()));
		return;
	}

	// illegal message
	if (wParam != HTMLEDIT_UPDATE_UI)
		return;

	// ---------------- Enable / Disable -------------------

	       UINT u32_CurDisplay  = pi_Editor->GetDisplayMode();
	static UINT u32_LastDisplay = 0;

    if (u32_LastDisplay != u32_CurDisplay) // do nothing if unchanged
	{
	    u32_LastDisplay   =  u32_CurDisplay;
		BOOL b_DesignMode = (u32_CurDisplay == IDM_EDITMODE);
		BOOL b_BrowseMode = (u32_CurDisplay == IDM_BROWSEMODE);
		BOOL b_SourceMode = (u32_CurDisplay == IDM_VIEWSOURCE);
		
		mCtrl_CheckBold.ShowWindow(!b_SourceMode);
		mCtrl_CheckItalic.ShowWindow(!b_SourceMode);
		mCtrl_CheckUnderline.ShowWindow(!b_SourceMode);
		mCtrl_CheckLeftAlign.ShowWindow(!b_SourceMode);
		mCtrl_CheckCenterAlign.ShowWindow(!b_SourceMode);
		mCtrl_CheckRightAlign.ShowWindow(!b_SourceMode);
		mCtrl_CheckOrderList.ShowWindow(!b_SourceMode);
		mCtrl_CheckUnOrderList.ShowWindow(!b_SourceMode);
		mCtrl_ComboFontName.ShowWindow(!b_SourceMode);
		mCtrl_ComboFontSize.ShowWindow(!b_SourceMode);
		mCtrl_BgText.ShowWindow(!b_SourceMode);
//		mCtrl_BgBody.ShowWindow(!b_SourceMode);

//		mCtrl_BgCell			.ShowWindow(!b_SourceMode);
//		mCtrl_ColLeft			.ShowWindow(!b_SourceMode);
//		mCtrl_ColRight			.ShowWindow(!b_SourceMode);
//		mCtrl_Combine			.ShowWindow(!b_SourceMode);
		mCtrl_ForeColor			.ShowWindow(!b_SourceMode);
//		mCtrl_DeleteCol			.ShowWindow(!b_SourceMode);
//		mCtrl_DeleteRow		.ShowWindow(!b_SourceMode);

//		mCtrl_Hr			.ShowWindow(!b_SourceMode);
//		mCtrl_HrProp			.ShowWindow(!b_SourceMode);
		mCtrl_Image			.ShowWindow(!b_SourceMode);
		mCtrl_Link			.ShowWindow(!b_SourceMode);
//		mCtrl_NoBr			.ShowWindow(!b_SourceMode);
//		mCtrl_RemoveFormat	.ShowWindow(!b_SourceMode);	
//		mCtrl_RowAbove			.ShowWindow(!b_SourceMode);	
//		mCtrl_RowBelow			.ShowWindow(!b_SourceMode);	
//		mCtrl_Split			.ShowWindow(!b_SourceMode);	
		mCtrl_Table				.ShowWindow(!b_SourceMode);	
//		mCtrl_TableProp		.ShowWindow(!b_SourceMode);		
			
//		mCtrl_EditUrl			.ShowWindow(!b_SourceMode);	
//		mCtrl_Load			.ShowWindow(!b_SourceMode);	
//		mCtrl_GoBack			.ShowWindow(!b_SourceMode);	
//		mCtrl_GoForward		.ShowWindow(!b_SourceMode);	
//		mCtrl_StaticUrl			.ShowWindow(!b_SourceMode);		

//		mCtrl_Save			.ShowWindow(!b_SourceMode);	
//		mCtrl_Help			.ShowWindow(!b_SourceMode);	
//		mCtrl_Clear				.ShowWindow(!b_SourceMode);	
//		mCtrl_CheckDesign     .EnableWindow(!b_SourceMode);
	}

// ---------- Set dynamic checkboxes (buttons) ---------

	// Arrays for all checkboxes, their state and their MSIE commands
	static UINT    u32_bCmd[] = { IDM_VIEWSOURCE,     IDM_EDITMODE,       IDM_BOLD,         IDM_ITALIC,         IDM_UNDERLINE,         IDM_JUSTIFYLEFT,       IDM_JUSTIFYCENTER,       IDM_JUSTIFYRIGHT,       IDM_ORDERLIST,         IDM_UNORDERLIST         };
	static CButton *p_Check[] = { &mCtrl_CheckSource, &mCtrl_CheckDesign, &mCtrl_CheckBold, &mCtrl_CheckItalic, &mCtrl_CheckUnderline, &mCtrl_CheckLeftAlign, &mCtrl_CheckCenterAlign, &mCtrl_CheckRightAlign, &mCtrl_CheckOrderList, &mCtrl_CheckUnOrderList };

	ASSERT(sizeof(u32_bCmd) == sizeof(mb_CheckStatus)); // illegal _CheckBoxCount

	// Set the checkboxes according to the current selection / cursor position
	for (UINT b=0; b<_CheckBoxCount; b++)
	{
		BOOL b_Active = (pi_Editor->QueryStatus(u32_bCmd[b]) & OLECMDF_LATCHED) > 0;
		if  (b_Active == mb_CheckStatus[b])
			continue;

		mb_CheckStatus[b] = b_Active;
		p_Check[b]->SetCheck(b_Active);
	}

	if (u32_CurDisplay != IDM_EDITMODE)
		return;

	// ---------- Set dynamic comboboxes (font) ---------

	static UINT      u32_cCmd[]    = { IDM_FONTNAME,         IDM_FONTSIZE_STYLE   };
	static CComboBox *p_Combo[]    = { &mCtrl_ComboFontName, &mCtrl_ComboFontSize };
	static BOOL       b_DropList[] = { TRUE, /*DropList*/    FALSE  /*DropDown*/  };

	ASSERT(sizeof(u32_cCmd) == sizeof(ms_ComboStatus)); // illegal _ComboBoxCount

	// Set font name and size according to the current selection / cursor position
	CString s_Value;
	for (UINT c=0; c<_ComboBoxCount; c++)
	{
		if (pi_Editor->ExecGetCommandStr(u32_cCmd[c], &s_Value))
		{
			if (s_Value == ms_ComboStatus[c] || s_Value.GetLength() == 0)
				continue;

			if (p_Combo[c]->GetDroppedState()) // If combobox not currently dropped down
				continue;
			
			ms_ComboStatus[c] = s_Value;

			// Depending on the style of the ComboBox we must choose another function here
			if (b_DropList[c]) p_Combo [c]->SelectString(0, s_Value);
			else               p_Combo [c]->SetWindowText(s_Value);
		}
	}
}

void CDlgHtmlEdit::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER_COMBO_FONTSIZE)
	{
		KillTimer(nIDEvent);
		OnUserChangedFontsize();
	}	
	
	CDialog::OnTimer(nIDEvent);
}

// 새 테이블 추가
BOOL CDlgHtmlEdit::InsertNewTable()
{
	CDlgInsertTable InsertTable(this);
	BOOL bReturn = TRUE;
	if (IDOK == InsertTable.DoModal())
	{
		bReturn = pi_Editor->GetDocument()->InsertTable(
			InsertTable.m_uRow, InsertTable.m_uCol, InsertTable.m_uBorderWidth,
			InsertTable.m_strBorderColor, InsertTable.m_uCellPadding, InsertTable.m_uCellSpacing,
			InsertTable.m_strTableWidth, InsertTable.m_strTableHeight, InsertTable.m_strBGColor);
	}
	return bReturn;
}

// set properties of a <HR> element (horizontal line)
void CDlgHtmlEdit::SetHRproperties()
{
	cHtmlDocument *pi_Doc = pi_Editor->GetDocument();
	cHtmlHR         i_HR  = pi_Doc->GetSelection();

	if (!i_HR.Valid())
	{
		MessageBox(_T("Please select a <HR> element. (horizontal line)"), _T("Error"), MB_ICONSTOP);
		return;
	}

	CHrProperties i_Prop(this);
	i_Prop.ms_Color   = i_HR.GetProperty(cHtmlHR::E_Color);
	i_Prop.ms_Width   = i_HR.GetProperty(cHtmlHR::E_Width);
	i_Prop.ms_Size    = i_HR.GetProperty(cHtmlHR::E_Size);
	i_Prop.ms32_Shade = i_HR.GetShade() ? 1 : 0;

	if (IDOK == i_Prop.DoModal())
	{
		i_HR.SetProperty(cHtmlHR::E_Color, i_Prop.ms_Color);
		i_HR.SetProperty(cHtmlHR::E_Width, i_Prop.ms_Width);
		i_HR.SetProperty(cHtmlHR::E_Size,  i_Prop.ms_Size);

		i_HR.SetShade(i_Prop.ms32_Shade == 1);
	}
}

// set properties of a table
void CDlgHtmlEdit::SetTableProperties()
{
	cHtmlElement i_Sel = pi_Editor->GetDocument()->GetSelection();
	cHtmlTable i_Table = i_Sel.FindParent(_T("TABLE"));
	if (!i_Table.Valid())
	{
		MessageBox(_T("커서의 위치를 테이블 안쪽으로 놓아 주십시요."), _T("Error"), MB_ICONSTOP);
		return;
	}

	cHtmlStyle i_Style = i_Table.GetStyle();

	CTableProperties i_Prop(this);

	i_Prop.mu32_Rules     = i_Table.GetBorderRules();
	i_Prop.ms_BackgColor  = i_Table.GetBgColor();
	i_Prop.ms_BorderColor = i_Table.GetBorderColor();
	i_Prop.ms_BorderWidth = i_Table.GetBorderWidth();
	i_Prop.ms_CellSpacing = i_Table.GetCellSpacing();
	i_Prop.ms_CellPadding = i_Table.GetCellPadding();
	i_Prop.ms_Width       = i_Style.GetProperty(cHtmlStyle::E_Width);
	i_Prop.ms_Height      = i_Style.GetProperty(cHtmlStyle::E_Height);
	i_Prop.ms_Width. Replace(_T("px"), _T(""));
	i_Prop.ms_Height.Replace(_T("px"), _T(""));

	if (IDOK == i_Prop.DoModal())
	{
		i_Table.SetBorderRules((cHtmlTable::eRules)i_Prop.mu32_Rules);
		i_Table.SetBgColor    (i_Prop.ms_BackgColor);
		i_Table.SetBorderColor(i_Prop.ms_BorderColor);
		i_Table.SetBorderWidth(i_Prop.ms_BorderWidth);
		i_Table.SetCellSpacing(i_Prop.ms_CellSpacing);
		i_Table.SetCellPadding(i_Prop.ms_CellPadding);

		if (i_Prop.ms_Width. GetLength() && i_Prop.ms_Width. Find(_T("%")) < 0) 
			i_Prop.ms_Width  += _T("px");
		if (i_Prop.ms_Height.GetLength() && i_Prop.ms_Height.Find(_T("%")) < 0) 
			i_Prop.ms_Height += _T("px");
		
		i_Style.SetProperty(cHtmlStyle::E_Width,  i_Prop.ms_Width);
		i_Style.SetProperty(cHtmlStyle::E_Height, i_Prop.ms_Height);
	}
}

CString CDlgHtmlEdit::GetHtml()
{
	return pi_Editor->GetDocument()->GetHtml();	
}

void CDlgHtmlEdit::SetHtml(CString strHtml)
{
	if(0==m_nTabMode)
	{
//		pi_Editor->GetDocument()->SetHtml(_T(""));	
//		pi_Editor->GetDocument()->PasteIntoSelection(strHtml);
//		pi_Editor->GetDocument()->CleanUpTags();
		pi_Editor->GetDocument()->SetHtml(strHtml);	
		pi_Editor->GetDocument()->CleanUpTags();
	}
	else
	{
//		pi_Editor->GetDocument()->SetHtml(strHtml);
//		pi_Editor->GetDocument()->CleanUpTags();
//		s_Html = pi_Editor->GetDocument()->GetHtml();
		pi_Editor->mi_RichEdit.SetWindowText(strHtml);
		pi_Editor->GetDocument()->SetHtml(strHtml);
		pi_Editor->GetDocument()->CleanUpTags();
	}
}

void CDlgHtmlEdit::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_nTabMode = mCtrl_Tab.GetCurSel();
	if(0==m_nTabMode)
	{
		mCtrl_CheckDesign.SetCheck(true);
		mCtrl_CheckSource.SetCheck(false);
		pi_Editor->SetDisplayMode(false, true);
	}
	else
	{
		mCtrl_CheckDesign.SetCheck(false);
		mCtrl_CheckSource.SetCheck(true);
		pi_Editor->SetDisplayMode(true, false);
	}
	/*
	case IDC_CHECK_DESIGN:
		mCtrl_CheckDesign.SetCheck(true);
		mCtrl_CheckSource.SetCheck(false);
		pi_Editor->SetDisplayMode(mCtrl_CheckSource.GetCheck(), 
			                      mCtrl_CheckDesign.GetCheck());
		break;
	case IDC_CHECK_SOURCE:
		mCtrl_CheckDesign.SetCheck(false);
		mCtrl_CheckSource.SetCheck(true);
*/
	*pResult = 0;
	
}



void CDlgHtmlEdit::MoveWindow(CRect rec, BOOL bRepaint)
{	
	CDialog::MoveWindow(rec, bRepaint);

	/*
	CRect recEdit = rec;	
	recEdit.top -= 25;
	recEdit.left = 9;
	recEdit.right -= 9;
	recEdit.bottom -= 9;	
	GetClientRect(rec);
	mCtrl_Tab.MoveWindow(rec);
	pi_Editor->MoveWindow(recEdit);
	Invalidate(TRUE);
	*/
}

void CDlgHtmlEdit::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect1;

	if ( pi_Editor == NULL ) return;
	pi_Editor->GetClientRect(rect1);
	ClientToScreen(rect1);
	rect1.top = 60;
	rect1.left = 5;
	rect1.bottom = cy-5;	
	rect1.right = cx-5;
	pi_Editor->MoveWindow(rect1);

	rect1.top = 0;
	rect1.left = 0;
	rect1.bottom = cy;	
	rect1.right = cx;

	mCtrl_Tab.MoveWindow(rect1);	
		/*

	CRect recEdit = rec;	
	recEdit.top -= 25;
	recEdit.left = 9;
	recEdit.right -= 9;
	recEdit.bottom -= 9;	
	GetClientRect(rec);
	mCtrl_Tab.MoveWindow(rec);
	pi_Editor->MoveWindow(recEdit);	
	*/
}
