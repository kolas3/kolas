// DlgInsertTable.cpp : implementation file
//

#include "stdafx.h"
#include "HtmlEdit.h"
#include "DlgInsertTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInsertTable dialog


CDlgInsertTable::CDlgInsertTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInsertTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInsertTable)
	m_uBorderWidth = 1;
	m_strBGColor = _T("white");
	m_strBorderColor = _T("silver");
	m_uCellPadding = 0;
	m_uCellSpacing = 0;
	m_strTableHeight = _T("100");
	m_uRow = 4;
	m_strTableWidth = _T("100%");
	m_uCol = 4;
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	mu32_Rules = 0;
}


void CDlgInsertTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInsertTable)
	DDX_Control(pDX, IDC_BUTTON_TABLE_BORDER_COLOR, m_btnBorderColor);
	DDX_Control(pDX, IDC_BUTTON_TABLE_BG_COLOR, m_btnBGColor);
	DDX_Text(pDX, IDC_EDIT_BorderWidth, m_uBorderWidth);
//	DDX_Text(pDX, IDC_EDIT_BG_COLOR, m_strBGColor);
//	DDX_Text(pDX, IDC_EDIT_BORDERCOLOR, m_strBorderColor);
	DDX_Text(pDX, IDC_EDIT_CELLPADDING, m_uCellPadding);
	DDX_Text(pDX, IDC_EDIT_CELLSPACING, m_uCellSpacing);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_strTableHeight);
	DDX_Text(pDX, IDC_EDIT_ROW, m_uRow);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_strTableWidth);
	DDX_Text(pDX, IDC_EDIT_COL, m_uCol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInsertTable, CDialog)
	//{{AFX_MSG_MAP(CDlgInsertTable)
	ON_BN_CLICKED(IDC_BUTTON_TABLE_BG_COLOR, OnButtonTableBgColor)
	ON_BN_CLICKED(IDC_BUTTON_TABLE_BORDER_COLOR, OnButtonTableBorderColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgInsertTable message handlers

void CDlgInsertTable::OnButtonTableBgColor() 
{
	// TODO: Add your control notification handler code here
	CColorDialog Dlg;
	COLORREF crColor;
	CString strColor;

	if(IDOK==Dlg.DoModal())
	{
		crColor = Dlg.GetColor();
		m_strBGColor.Format(_T("#%.2X%.2X%.2X"), 
			GetRValue(crColor), GetGValue(crColor), GetBValue(crColor));
		m_btnBGColor.SetButtonColor(crColor);
	}		
}

void CDlgInsertTable::OnButtonTableBorderColor() 
{
	// TODO: Add your control notification handler code here
	CColorDialog Dlg;
	COLORREF crColor;
	CString strColor;

	if(IDOK==Dlg.DoModal())
	{
		crColor = Dlg.GetColor();
		m_strBorderColor.Format(_T("#%.2X%.2X%.2X"), 
			GetRValue(crColor), GetGValue(crColor), GetBValue(crColor));		
		m_btnBorderColor.SetButtonColor(crColor);
	}
}

BOOL CDlgInsertTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_btnBGColor.SetButtonColor(RGB(255,255,255));
	m_btnBorderColor.SetButtonColor(RGB(100,100,100));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
