// ExcelToPMAliasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExcelToPMAlias.h"
#include "ExcelToPMAliasDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ImportDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelToPMAliasDlg dialog

CExcelToPMAliasDlg::CExcelToPMAliasDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExcelToPMAliasDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcelToPMAliasDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pExcelCtrl = NULL;
	m_pExcelMgr = NULL;

	m_pSheetGrid = new CESL_Grid(NULL);
	m_pTableListGrid = new CESL_Grid(NULL);
	m_pTableGrid = new CESL_Grid(NULL);

	// KOL.UDF.022 시큐어코딩 보완
	m_CurSheetIdx = -1;
}

VOID CExcelToPMAliasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcelToPMAliasDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExcelToPMAliasDlg, CDialog)
	//{{AFX_MSG_MAP(CExcelToPMAliasDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bOPEN, OnbOPEN)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelToPMAliasDlg message handlers

BOOL CExcelToPMAliasDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_pSheetGrid->SubclassDlgItem(IDC_GRID_SHEET_LIST, this);
	m_pTableListGrid->SubclassDlgItem(IDC_GRID_TABLE_LIST, this);
	m_pTableGrid->SubclassDlgItem(IDC_GRID_TABLE, this);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CExcelToPMAliasDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CExcelToPMAliasDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExcelToPMAliasDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CExcelToPMAliasDlg::OnbOPEN() 
{
	TCHAR TmpDir[500];

	GetCurrentDirectory(500,TmpDir);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                    _T("All files(*.*)|"), this);
	if(dlg.DoModal() != IDOK) {
		SetCurrentDirectory(TmpDir);
        return;
	}

    m_strFilePath = dlg.GetPathName();
	SetCurrentDirectory(TmpDir);

	CRect rect;
	GetWindowRect(rect);

	FreeExcelCtrl();
	m_pExcelCtrl = new C_ExcelManControl;
	m_pExcelMgr = new CExcelManager;
	if (m_pExcelCtrl == NULL) return;
	if (m_pExcelMgr == NULL) return;

	m_pExcelCtrl->Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, WM_USER + 10000);
	if (m_pExcelCtrl->m_hWnd == NULL) {
		AfxMessageBox(_T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오."));
		return ;
	}

	m_pExcelMgr->SetManager(m_pExcelCtrl);
    if(m_pExcelMgr->OpenManager() == FALSE) 
	{
        AfxMessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"));
		return ;
	}
	m_pExcelMgr->SetUseTemplate(TRUE, m_strFilePath);
	if (m_pExcelMgr->OpenManager() == FALSE)
	{
		AfxMessageBox(_T("Open Failed"));
		return ;
	}

	INT nSheets = m_pExcelMgr->WorkSheet_Count();
	if (nSheets == 0) {
		AfxMessageBox(_T("빈문서입니다."));
		return;
	}

	m_pSheetGrid->SetRows(nSheets+1);
	m_pSheetGrid->SetCols(0, 1);
	SetColumnInfo(m_pSheetGrid, 0, _T("Sheet"), 100, 4);
	CString strMsg;
	for (INT i = 0; i < nSheets; i++) {
		strMsg.Format(_T("Sheet %d"), i+1);
		m_pSheetGrid->SetTextMatrix(i+1, 0, strMsg);
	}
	m_CurSheetIdx = -1;
}

INT CExcelToPMAliasDlg::FreeExcelCtrl(VOID)
{
	if (m_pExcelCtrl) {
		delete m_pExcelCtrl;
		m_pExcelCtrl = NULL;
	}
	if (m_pExcelMgr) {
		m_pExcelMgr->CloseManager();
		delete m_pExcelMgr;
		m_pExcelMgr = NULL;
	}
	return 0;
}

VOID CExcelToPMAliasDlg::OnbCLOSE() 
{
	FreeExcelCtrl();
	if (m_pSheetGrid) {
		delete m_pSheetGrid;
		m_pSheetGrid = NULL;
	}
	if (m_pTableListGrid) {
		delete m_pTableListGrid;
		m_pTableListGrid = NULL;
	}
	if (m_pTableGrid) {
		delete m_pTableGrid;
		m_pTableGrid = NULL;
	}

	CDialog::OnOK();
}

INT CExcelToPMAliasDlg::SetColumnInfo(CESL_Grid *pGrid, INT col, CString str, INT width, INT alignment)
{
	if (pGrid == NULL) return -1;

	pGrid->SetColAlignmentFixed(col, 4);
	pGrid->SetTextMatrix(0, col, str);
	pGrid->SetColWidth(col, 0, width*15);
	pGrid->SetColAlignment(col, alignment);
	return 0;
}

BEGIN_EVENTSINK_MAP(CExcelToPMAliasDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CExcelToPMAliasDlg)
	ON_EVENT(CExcelToPMAliasDlg, IDC_GRID_SHEET_LIST, -601 /* DblClick */, OnDblClickGridSheetList, VTS_NONE)
	ON_EVENT(CExcelToPMAliasDlg, IDC_GRID_TABLE_LIST, -601 /* DblClick */, OnDblClickGridTableList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CExcelToPMAliasDlg::OnDblClickGridSheetList() 
{
	OnOpenSheet();
}

INT CExcelToPMAliasDlg::OnOpenSheet(VOID)
{
	INT nSheet = m_pSheetGrid->GetRow();	
	m_pExcelMgr->WorkSheet_Activate(nSheet);
	CString strData, strTable, strEOF;
	CArray <CString, CString> arrTable;
	arrTable.RemoveAll();
	INT i;
	for (i = 1; i <= EXCEL_MAX_LINE; i++) {
		strEOF = m_pExcelMgr->GetTextMatrix(i, 1);
		strEOF.TrimLeft();			strEOF.TrimRight();
		if (strEOF.GetLength() == 0) break;

		strData = _T("");
		strTable = _T("");
		strData = m_pExcelMgr->GetTextMatrix(i, 5);
		strData.TrimLeft();		strData.TrimRight();
		strData.MakeUpper();
		
		if (strData.Find(_T("테이블"), 0) >= 0 && strData.Find(_T("ID"), 0) >= 0) {
			strTable = m_pExcelMgr->GetTextMatrix(i, 6);
			strTable.TrimLeft();		strTable.TrimRight();
			arrTable.Add(strTable);
		}
	}

	INT nTables = arrTable.GetSize();
	if (nTables == 0) {
		m_pTableListGrid->Clear();
		return -1;
	}
	m_pTableListGrid->SetRows(nTables+1);
	m_pTableListGrid->SetCols(0, 1);
	SetColumnInfo(m_pTableListGrid, 0, _T("Table"), 150, 1);
	for (i = 0; i < nTables; i++) {
		m_pTableListGrid->SetTextMatrix(i+1, 0, arrTable.GetAt(i));
	}

	m_CurSheetIdx = nSheet;
	SetReverse(m_pSheetGrid, nSheet);

	return 0;
}

VOID CExcelToPMAliasDlg::OnDblClickGridTableList() 
{
	OnOpenTable();
}

INT CExcelToPMAliasDlg::OnOpenTable(VOID)
{
	m_pExcelMgr->WorkSheet_Activate(m_CurSheetIdx);
	INT nRow = m_pTableListGrid->GetRow();
	CString strTable = m_pTableListGrid->GetTextMatrix(nRow, 0);
	strTable.TrimLeft();		strTable.TrimRight();
	strTable.MakeUpper();
	if (strTable.GetLength() == 0) {
		AfxMessageBox(_T("테이블 이름을 찾을 수 없습니다."));
		return -1;
	}

	CString strColumnName, strType, strLength, strDesc, strEOF, strData, strTmpTable;
	INT nStart = -1;
	INT i;
	for (i = 1; i <= EXCEL_MAX_LINE; i++) {
		strEOF = m_pExcelMgr->GetTextMatrix(i, 1);
		strEOF.TrimLeft();			strEOF.TrimRight();
		if (strEOF.GetLength() == 0) break;

		strData = _T("");
		strData = m_pExcelMgr->GetTextMatrix(i, 5);
		strData.TrimLeft();		strData.TrimRight();
		strData.MakeUpper();
		
		if (strData.Find(_T("테이블"), 0) >= 0 && strData.Find(_T("ID"), 0) >= 0) {
			strTmpTable = m_pExcelMgr->GetTextMatrix(i, 6);
			strTmpTable.TrimLeft();		strTmpTable.TrimRight();
			strTmpTable.MakeUpper();
			if (strTmpTable == strTable) {
				nStart = i + 3;
				break;
			}
		}
	}

	if (nStart == -1) {
		AfxMessageBox(_T("테이블 정보를 찾을 수 없습니다."));
		return -1;
	}
	INT nRowCount = 0;
	m_pTableGrid->SetRows(GRID_MAX_LINE);
	m_pTableGrid->SetCols(0, 4);
	SetColumnInfo(m_pTableGrid, 0, _T("컬럼명"), 150, 1);
	SetColumnInfo(m_pTableGrid, 1, _T("자료유형"), 100, 4);
	SetColumnInfo(m_pTableGrid, 2, _T("길이"), 60, 4);
	SetColumnInfo(m_pTableGrid, 3, _T("설명"), 200, 1);
	for (i = nStart; i <= EXCEL_MAX_LINE; i++) {
		strColumnName = m_pExcelMgr->GetTextMatrix(i, 1);
		strColumnName.TrimLeft();			strColumnName.TrimRight();
		if (strColumnName.GetLength() == 0) break;
		if (strColumnName.Find(_T("명"), 0) >= 0) break;

		strType = m_pExcelMgr->GetTextMatrix(i, 2);
		strType.TrimLeft();		strType.TrimRight();

		strLength = m_pExcelMgr->GetTextMatrix(i, 3);
		strLength.TrimLeft();		strLength.TrimRight();

		strDesc = m_pExcelMgr->GetTextMatrix(i, 8);
		strDesc.TrimLeft();		strDesc.TrimRight();

		m_pTableGrid->SetTextMatrix(nRowCount+1, 0, strColumnName);
		m_pTableGrid->SetTextMatrix(nRowCount+1, 1, strType);
		m_pTableGrid->SetTextMatrix(nRowCount+1, 2, strLength);
		m_pTableGrid->SetTextMatrix(nRowCount+1, 3, strDesc);

		nRowCount++;
	}
	if (nRowCount == 0) {
		m_pTableGrid->Clear();
		return -2;
	}
	m_pTableGrid->SetRows(nRowCount+1);
	SetReverse(m_pTableListGrid, nRow);
	m_CurTableName = strTable;

	return 0;
}

VOID CExcelToPMAliasDlg::SetReverse(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return;
	if (pGrid->m_nReverse != -1 && pGrid->GetRows() > pGrid->m_nReverse) {
		pGrid->SetRow(pGrid->m_nReverse);
		pGrid->SetCol(0);
		pGrid->SetCellBackColor(RGB(255, 255, 255));
	}
	COLORREF blue = RGB(228,243,251);			//E4F3FB
	pGrid->SetRow(nRow);
	pGrid->SetCol(0);
	pGrid->SetCellBackColor(blue);

	pGrid->m_nReverse = nRow;
	return;
}

VOID CExcelToPMAliasDlg::OnbIMPORT() 
{
	CImportDlg dlg(this);
	dlg.m_pTableGrid = m_pTableGrid;
	dlg.m_strTABLENAME = m_CurTableName;
	dlg.DoModal();
}
