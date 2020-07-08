// SchemaEvaluationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SchemaEvaluation.h"
#include "SchemaEvaluationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>

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
// CSchemaEvaluationDlg dialog

CSchemaEvaluationDlg::CSchemaEvaluationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSchemaEvaluationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSchemaEvaluationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pDM_Reference = new CESL_DataMgr;
	pDM_Oracle = new CESL_DataMgr;

	pGrid_Reference = new CESL_Grid(NULL);
	pGrid_Oracle = new CESL_Grid(NULL);

	// KOL.UDF.022 시큐어코딩 보완
	INT i = 0, j = 0;
	for(i=0;i<2000;i++){
		for(j=0;j<10;j++){
			m_Result[i][j] = _T("");
		}
	}
	m_nResult = 0;
}

VOID CSchemaEvaluationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSchemaEvaluationDlg)
	DDX_Control(pDX, IDC_cTABLELIST, m_cTableList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSchemaEvaluationDlg, CDialog)
	//{{AFX_MSG_MAP(CSchemaEvaluationDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bEVAL, OnbEVAL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchemaEvaluationDlg message handlers

BOOL CSchemaEvaluationDlg::OnInitDialog()
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
	pGrid_Oracle->SubclassDlgItem(IDC_gORACLE, this);
	pGrid_Reference->SubclassDlgItem(IDC_gREFERENCE, this);

	ReadCfgFile.ReadCfgFile(_T(""));
	pDM_Oracle->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	pDM_Oracle->RestructDataManager(_T("SELECT TNAME FROM TAB"));
	m_cTableList.ResetContent();
	INT nRow = pDM_Oracle->GetRowCount();
	for (INT i =0; i < nRow; i++) {
		m_cTableList.AddString(pDM_Oracle->GetCellData(i, 0));
	}
	m_cTableList.SetCurSel(0);
	UpdateData(FALSE);
	pDM_Oracle->FreeData();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CSchemaEvaluationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CSchemaEvaluationDlg::OnPaint() 
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
HCURSOR CSchemaEvaluationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CSchemaEvaluationDlg::OnbEVAL() 
{
	RefreshOracleSchema();
	RefreshReferenceSchema();

	INT i, j, nReferenceRow, nOracleRow, ids;
	CString nameR, typeR, sizeR;
	CString nameO, typeO, sizeO;
	BOOL isequal;
	// Reference 것을 평가한다.
	nReferenceRow = m_nResult;
	nOracleRow = pDM_Oracle->GetRowCount();

	for (i = 0; i < nReferenceRow; i++) {
		nameR = m_Result[i][0];
		typeR = m_Result[i][1];
		sizeR = m_Result[i][2];
		isequal = FALSE;
		for (j = 0; j < nOracleRow; j++) {
			ids = pDM_Oracle->GetCellData(j, 0, nameO);
			ids = pDM_Oracle->GetCellData(j, 1, typeO);
			ids = pDM_Oracle->GetCellData(j, 2, sizeO);
			if (typeO == _T("DATE") && typeR == _T("DATE")) {
				if (nameO == nameR) {
					isequal = TRUE;
					break;
				}
			}
			if (typeO == _T("LONG") && typeR == _T("LONG")) {
				if (nameO == nameR) {
					isequal = TRUE;
					break;
				}
			}
			if (nameO == nameR && typeO == typeR && sizeO == sizeR) {
				isequal = TRUE;
				break;
			}
		}
		if (isequal == FALSE) {
			SetGridColor(pGrid_Reference, i+1, 255, 255, 153);		// 228,243,251
		}
	}

	for (i = 0; i < nOracleRow ; i++) {
		ids = pDM_Oracle->GetCellData(i, 0, nameO);
		ids = pDM_Oracle->GetCellData(i, 1, typeO);
		ids = pDM_Oracle->GetCellData(i, 2, sizeO);
		isequal = FALSE;
		for (j = 0; j < nReferenceRow; j++) {
			nameR = m_Result[j][0];
			typeR = m_Result[j][1];
			sizeR = m_Result[j][2];
			if (typeO == _T("DATE") && typeR == _T("DATE")) {
				if (nameO == nameR) {
					isequal = TRUE;
					break;
				}
			}
			if (typeO == _T("LONG") && typeR == _T("LONG")) {
				if (nameO == nameR) {
					isequal = TRUE;
					break;
				}
			}
			if (nameO == nameR && typeO == typeR && sizeO == sizeR) {
				isequal = TRUE;
				break;
			}
		}
		if (isequal == FALSE) {
			SetGridColor(pGrid_Oracle, i+1, 255, 102, 153);
		}
	}	
}

VOID CSchemaEvaluationDlg::SetGridColor(CESL_Grid *pGrid, INT nRowIdx, INT R, INT G, INT B)
{
	if (!pGrid) return;

	COLORREF c = RGB(R,G,B);

	pGrid->SetRow(nRowIdx);
	pGrid->SetCol(1);
	pGrid->SetCellBackColor(c);
	pGrid->SetCol(2);
	pGrid->SetCellBackColor(c);
	pGrid->SetCol(3);
	pGrid->SetCellBackColor(c);
	return;
}

BOOL CSchemaEvaluationDlg::DestroyWindow() 
{
	if (pDM_Reference) {
		delete pDM_Reference;
		pDM_Reference = NULL;
	}
	if (pDM_Oracle) {
		delete pDM_Oracle;
		pDM_Oracle = NULL;
	}
	if (pGrid_Reference) {
		delete pGrid_Reference;
		pGrid_Reference = NULL;
	}
	if (pGrid_Oracle) {
		delete pGrid_Oracle;
		pGrid_Oracle = NULL;
	}

	return CDialog::DestroyWindow();
}

INT CSchemaEvaluationDlg::RefreshOracleSchema(VOID)
{
	UpdateData(TRUE);
	CString strTableName;
	m_cTableList.GetWindowText(strTableName);

	CString sQuery;
	sQuery.Format(_T("SELECT CNAME, COLTYPE, WIDTH FROM COL WHERE TNAME='%s' ORDER BY CNAME"), strTableName);
	pDM_Oracle->RestructDataManager(sQuery);
	INT nRow = pDM_Oracle->GetRowCount();
	pGrid_Oracle->RemoveAll();
	if (nRow > 0) pGrid_Oracle->SetRows(nRow+1);
		else pGrid_Oracle->SetRows(2);
	pGrid_Oracle->SetCols(0, 4);
	pGrid_Oracle->SetColWidth(0, 0, 40*15);
	pGrid_Oracle->SetColWidth(1, 0, 200*15);
	pGrid_Oracle->SetColWidth(2, 0, 70*15);
	pGrid_Oracle->SetColWidth(3, 0, 40*15);

	CString strMsg;
	for (INT i = 0; i < nRow; i++) {
		strMsg.Format(_T("%d"), i);
		pGrid_Oracle->SetTextMatrix(i+1, 0, strMsg);
		strMsg.Format(_T("%s"), pDM_Oracle->GetCellData(i, 0));
		strMsg.TrimLeft();			strMsg.TrimRight();
		strMsg.MakeUpper();
		pDM_Oracle->SetCellData(i, 0, strMsg);

		strMsg.Format(_T("%s"), pDM_Oracle->GetCellData(i, 1));
		strMsg.TrimLeft();			strMsg.TrimRight();
		strMsg.MakeUpper();
		pDM_Oracle->SetCellData(i, 1, strMsg);

		pGrid_Oracle->SetTextMatrix(i+1, 1, pDM_Oracle->GetCellData(i, 0));
		pGrid_Oracle->SetTextMatrix(i+1, 2, pDM_Oracle->GetCellData(i, 1));
		pGrid_Oracle->SetTextMatrix(i+1, 3, pDM_Oracle->GetCellData(i, 2));
	}

	return 0;
}

INT CSchemaEvaluationDlg::RefreshReferenceSchema(VOID)
{
	UpdateData(TRUE);
	CString strTableName;
	m_cTableList.GetWindowText(strTableName);

	CString sQuery;
	sQuery.Format(_T("SELECT FIELD_NAME, FIELD_TYPE, FIELD_SIZE FROM ESL_REFERENCE_TBL WHERE TBL_NAME='%s' AND VIEW_FLAG='T' ORDER BY FIELD_NAME"), strTableName);
	MSSQL_SELECT(_T("192.168.1.50"), _T("sa"), _T("eco"), _T("pm"), sQuery);
	INT nRow = m_nResult;
	pGrid_Reference->RemoveAll();
	if (nRow > 0) pGrid_Reference->SetRows(nRow+1);
		else pGrid_Reference->SetRows(2);
	pGrid_Reference->SetCols(0, 4);
	pGrid_Reference->SetColWidth(0, 0, 40*15);
	pGrid_Reference->SetColWidth(1, 0, 200*15);
	pGrid_Reference->SetColWidth(2, 0, 70*15);
	pGrid_Reference->SetColWidth(3, 0, 40*15);

	CString strMsg;
	for (INT i = 0; i < nRow; i++) {
		strMsg.Format(_T("%d"), i);
		pGrid_Reference->SetTextMatrix(i+1, 0, strMsg);
		pGrid_Reference->SetTextMatrix(i+1, 1, m_Result[i][0]);
		pGrid_Reference->SetTextMatrix(i+1, 2, m_Result[i][1]);
		pGrid_Reference->SetTextMatrix(i+1, 3, m_Result[i][2]);
	}

	return 0;
}

INT CSchemaEvaluationDlg::MSSQL_SELECT(CString IP, CString ID, CString PWD, CString DB, CString sQuery)
{
	m_nResult = 0;
	CString query;
	PDBPROCESS  dbproc;
	PLOGINREC   login;
	_TUCHAR result[10][1024];
	CString tmp[10];

	dbinit();
    login = dblogin ();
	DBSETLUSER (login, ID);
	DBSETLPWD  (login, PWD);
    dbproc = dbopen (login, IP);
	
	if(!dbproc || dbdead (dbproc)){
		dbexit();
		return 0;
	}

	dbuse(dbproc, DB);

	INT nRow, nCol, i;

	nRow = nCol = 0;
	query = sQuery;
	dbcmd (dbproc, query);
    dbsqlexec (dbproc);

	if(dbresults(dbproc) == SUCCEED){	
		nCol = dbnumcols(dbproc);
		for(i = 0 ; i < nCol ; i++){
			dbbind(dbproc, i+1, NTBSTRINGBIND, 0, result[i]);
		}		
        while (dbnextrow (dbproc) != NO_MORE_ROWS)
        {
			for(i = 0 ; i < nCol ; i++){
				m_Result[nRow][i].Format(_T("%s"), result[i]);
				m_Result[nRow][i].TrimLeft();		m_Result[nRow][i].TrimRight();
			}					
			m_Result[nRow][0].MakeUpper();
			m_Result[nRow][1].MakeUpper();
			nRow++;
        }
	}
	m_nResult = nRow;
	dbfreebuf(dbproc);	// MSSQL Select buf 객체를 해제한다.
	dbexit();
	return 0;
}
