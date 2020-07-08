// DBSyncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBSync.h"
#include "DBSyncDlg.h"

#include "DBSyncDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CDBSyncDlg dialog

CDBSyncDlg::CDBSyncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBSyncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBSyncDlg)
	m_src_sid = _T("");
	m_dst_sid = _T("");
	m_src_pass = _T("");
	m_dst_pass = _T("");
	m_src_user = _T("");
	m_dst_user = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pGrid = NULL;
	// KOL.UDF.022
    SCHEMA_TABLE.RemoveAll();
    SCHEMA_RESULT.RemoveAll();
    DATA_TABLE.RemoveAll();
    DATA_FIELD.RemoveAll();
    DATA_WHERE.RemoveAll();
    DATA_SRC_CNT.RemoveAll();
    DATA_DST_CNT.RemoveAll();
	m_bInitDlg = FALSE;
	m_bReadOnly = FALSE;
}

CDBSyncDlg::~CDBSyncDlg()
{
    InitMemory();
}

VOID CDBSyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBSyncDlg)
	DDX_Text(pDX, IDC_eSID1, m_src_sid);
	DDX_Text(pDX, IDC_eSID2, m_dst_sid);
	DDX_Text(pDX, IDC_ePASS1, m_src_pass);
	DDX_Text(pDX, IDC_ePASS2, m_dst_pass);
	DDX_Text(pDX, IDC_eUSER1, m_src_user);
	DDX_Text(pDX, IDC_eUSER2, m_dst_user);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBSyncDlg, CDialog)
	//{{AFX_MSG_MAP(CDBSyncDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bRUN, OnbRUN)
	ON_BN_CLICKED(IDC_bDETAIL, OnbDETAIL)
	ON_BN_CLICKED(IDC_rSCHEMA, OnrSCHEMA)
	ON_BN_CLICKED(IDC_rDATA, OnrDATA)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CDBSyncDlg::InitMemory()
{
	if ( m_pGrid != NULL ) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

INT CDBSyncDlg::ReadCfg()
{
    CString envcfg, schemacfg, datacfg;

#ifdef _UNICODE
    envcfg = _T("dbsync_env_uni.cfg");
    schemacfg = _T("dbsync_schema_uni.cfg");
    datacfg = _T("dbsync_data_uni.cfg");
#else 
    envcfg = _T("dbsync_env_mbcs.cfg");
    schemacfg = _T("dbsync_schema_mbcs.cfg");
    datacfg = _T("dbsync_data_mbcs.cfg");
#endif 

    INT nError;
    nError = ReadEnvCfg(envcfg);
    if ( nError < 0 ) {
        AfxMessageBox(envcfg + _T("을 읽을 수 없습니다"));
        return 1;
    } else if ( nError > 0 ) {
        return 1;
    }

    nError = ReadSchemaCfg(schemacfg);
    if ( nError < 0 ) {
        AfxMessageBox(schemacfg + _T("을 읽을 수 없습니다"));
        return 1;
    } else if ( nError > 0 ) {
        return 1;
    }

    nError = ReadDataCfg(datacfg);
    if ( nError < 0 ) {
        AfxMessageBox(datacfg + _T("을 읽을 수 없습니다"));
        return 1;
    } else if ( nError > 0 ) {
        return 1;
    }

    return 0;
}

INT CDBSyncDlg::ReadEnvCfg(CString path)
{
    if ( path.IsEmpty() ) return -1;
    m_src_sid.Empty();
    m_src_user.Empty();
    m_src_pass.Empty();
    m_dst_sid.Empty();
    m_dst_user.Empty();
    m_dst_pass.Empty();

	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) return -1;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
    GetDlgItem(IDC_eENV)->SetWindowText(file.GetFilePath());

	_TCHAR szCfg[1000];

	INT nRead = file.Read(szCfg, 1000);

#ifdef _UNICODE
	nRead /= 2;
#endif 

	szCfg[nRead] = NULL;
	file.Close();

    if ( _tcslen(szCfg) == 0 ) {
        file.Close();
        return -1;
    }

	_TCHAR name[64];
	_TCHAR value[256];
	INT pos;

	const _TCHAR seps[] = _T("\n");
	_TCHAR* token;
	token = _tcstok(szCfg, seps);

#ifdef _UNICODE
	// 2byte를 제외하고 데이터가 있는지 체크한다
	token++;
#endif 

	if ( token[0] == NULL ) return 0;

	while ( token != NULL ) {

		name[0] = NULL;
		value[0] = NULL;
		pos = 0;

		if ( token[0] == _T('.') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;

		} else if ( token[0] == _T('\r') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;
		}

		pos = GetToken(token, pos, name);
		pos = GetToken(token, pos, value);
 
		if ( _tcscmp(name, _T("src_sid")) == 0 ) {
			m_src_sid = value;
		} else if ( _tcscmp(name, _T("src_user")) == 0 ) {
			m_src_user = value;
		} else if ( _tcscmp(name, _T("src_pass")) == 0 ) {
			m_src_pass = value;
		} else if ( _tcscmp(name, _T("dst_sid")) == 0 ) {
			m_dst_sid = value;
		} else if ( _tcscmp(name, _T("dst_user")) == 0 ) {
			m_dst_user = value;
		} else if ( _tcscmp(name, _T("dst_pass")) == 0 ) {
			m_dst_pass = value;
		}

		token = _tcstok(NULL, seps);
	}

    UpdateData(false);

	return 0;
}

INT CDBSyncDlg::ReadSchemaCfg(CString path)
{
    if ( path.IsEmpty() ) return -1;
    SCHEMA_TABLE.RemoveAll();
    SCHEMA_RESULT.RemoveAll();

	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) return -1;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
    GetDlgItem(IDC_eSCHEMA)->SetWindowText(file.GetFilePath());

	_TCHAR szCfg[1000];

	INT nRead = file.Read(szCfg, 1000);

#ifdef _UNICODE
	nRead /= 2;
#endif 

	szCfg[nRead] = NULL;
	file.Close();

    if ( _tcslen(szCfg) == 0 ) {
        file.Close();
        return -1;
    }

	_TCHAR name[64];
	_TCHAR value[256];
	INT pos;

	const _TCHAR seps[] = _T("\n");
	_TCHAR* token;
	token = _tcstok(szCfg, seps);

#ifdef _UNICODE
	// 2byte를 제외하고 데이터가 있는지 체크한다
	token++;
#endif 

	if ( token[0] == NULL ) return 0;

	while ( token != NULL ) {

		name[0] = NULL;
		value[0] = NULL;
		pos = 0;

		if ( token[0] == _T('.') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;

		} else if ( token[0] == _T('\r') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;
		}

		pos = GetToken(token, pos, name);
        SCHEMA_TABLE.Add(name);

		token = _tcstok(NULL, seps);
	}
	return 0;
}

INT CDBSyncDlg::ReadDataCfg(CString path)
{
    if ( path.IsEmpty() ) return -1;
    DATA_TABLE.RemoveAll();
    DATA_FIELD.RemoveAll();
    DATA_WHERE.RemoveAll();
    DATA_SRC_CNT.RemoveAll();
    DATA_DST_CNT.RemoveAll();

	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) return -1;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
    GetDlgItem(IDC_eDATA)->SetWindowText(file.GetFilePath());

	_TCHAR szCfg[1000];

	INT nRead = file.Read(szCfg, 1000);

#ifdef _UNICODE
	nRead /= 2;
#endif 

	szCfg[nRead] = NULL;
	file.Close();

    if ( _tcslen(szCfg) == 0 ) {
        file.Close();
        return -1;
    }

	_TCHAR table[128];
	_TCHAR field[128];
	_TCHAR where[1024];
	INT pos;

	const _TCHAR seps[] = _T("\n");
	_TCHAR* token;
	token = _tcstok(szCfg, seps);

#ifdef _UNICODE
	// 2byte를 제외하고 데이터가 있는지 체크한다
	token++;
#endif 

	if ( token[0] == NULL ) return 0;

	while ( token != NULL ) {

		table[0] = NULL;
		field[0] = NULL;
		pos = 0;

		if ( token[0] == _T('.') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;

		} else if ( token[0] == _T('\r') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;
		}

		pos = GetToken(token, pos, table);
		pos = GetToken(token, pos, field);
		pos = GetToken(token, pos, where);

        DATA_TABLE.Add(table);
        DATA_FIELD.Add(field);
        DATA_WHERE.Add(where);

		token = _tcstok(NULL, seps);
	}
	return 0;
}

bool CDBSyncDlg::IsSchema()
{
	if ( ((CButton*)GetDlgItem(IDC_rSCHEMA))->GetCheck() ) return true;
    else return false;
}

CString CDBSyncDlg::GetSrcConnection()
{
    CString conn;

    UpdateData(true);
    if ( m_src_sid.IsEmpty() ) return conn;
    if ( m_src_user.IsEmpty() ) return conn;
    if ( m_src_pass.IsEmpty() ) return conn;

    conn.Format(_T("%s@%s/%s[]"), m_src_user, m_src_sid, m_src_pass);
    return conn;
}

CString CDBSyncDlg::GetDstConnection()
{
    CString conn;

    UpdateData(true);
    if ( m_dst_sid.IsEmpty() ) return conn;
    if ( m_dst_user.IsEmpty() ) return conn;
    if ( m_dst_pass.IsEmpty() ) return conn;

    conn.Format(_T("%s@%s/%s[]"), m_dst_user, m_dst_sid, m_dst_pass);
    return conn;
}

INT CDBSyncDlg::LoadSchema()
{
	m_pGrid->RemoveAll();
	m_pGrid->InitColumn(3);
	m_pGrid->SetColumnInfo(0, _T("Check"), 45, 4);
	m_pGrid->SetColumnInfo(1, _T("Table"), 150, 1);
	m_pGrid->SetColumnInfo(2, _T("일치"), 50, 4);

	m_pGrid->m_bGridEdit = false;
	m_pGrid->m_bMenu = true;
	m_pGrid->m_bCheck = true;

    INT nRow;
    nRow = SCHEMA_TABLE.GetSize();

    INT i; 
    for ( i = 0 ; i < nRow ; i++ ) {
        m_pGrid->SetAt(i, 1, SCHEMA_TABLE[i]);
    }
    m_pGrid->DisplayNumber();

    return 0;
}

INT CDBSyncDlg::LoadData()
{
	m_pGrid->RemoveAll();
	m_pGrid->InitColumn(7);
	m_pGrid->SetColumnInfo(0, _T("Check"), 45, 4);
	m_pGrid->SetColumnInfo(1, _T("Table"), 100, 1);
	m_pGrid->SetColumnInfo(2, _T("Field"), 75, 1);
	m_pGrid->SetColumnInfo(3, _T("Where"), 120, 1);
	m_pGrid->SetColumnInfo(4, _T("일치"), 40, 4);
	m_pGrid->SetColumnInfo(5, _T("SrcCnt"), 50, 1);
	m_pGrid->SetColumnInfo(6, _T("DstCnt"), 50, 1);

	m_pGrid->m_bGridEdit = false;
	m_pGrid->m_bMenu = true;
	m_pGrid->m_bCheck = true;

    INT nRow;
    nRow = DATA_TABLE.GetSize();

    INT i; 
    for ( i = 0 ; i < nRow ; i++ ) {
        m_pGrid->SetAt(i, 1, DATA_TABLE[i]);
        m_pGrid->SetAt(i, 2, DATA_FIELD[i]);
        m_pGrid->SetAt(i, 3, DATA_WHERE[i]);
    }
    m_pGrid->DisplayNumber();

    return 0;
}

INT CDBSyncDlg::RunSchema()
{
    INT nError;

    INT cnt;
    cnt = m_pGrid->GetCount();

    INT i;
    for ( i = 0 ; i < cnt ; i++ ) {
        CString check;
        check = m_pGrid->GetAt(i, 0);

        CString table;
        table = m_pGrid->GetAt(i, 1);

        if ( check != _T("V") ) continue;

        nError = RunSchemaGetSrc(table);
        if ( nError < 0 ) continue;
        else if ( nError > 0 ) continue;

        nError = RunSchemaGetDst(table);
        if ( nError < 0 ) continue;
        else if ( nError > 0 ) continue;

        CString compare;
        compare = RunSchemaCompare();
        m_pGrid->SetAt(i, 2, compare);
    }

    return 0;
}

INT CDBSyncDlg::RunSchemaGetSrc(CString table)
{
    if ( table.IsEmpty() ) return -1;

    CString conn;
    conn = GetSrcConnection();
    if ( conn.IsEmpty() ) return -1;

    m_dmSrc.SetCONNECTION_INFO(conn);

	CString sql;
	sql.Format(_T("SELECT COLUMN_NAME, DATA_TYPE, DATA_LENGTH, NULLABLE, DATA_DEFAULT, CHAR_COL_DECL_LENGTH FROM COLS WHERE TABLE_NAME = '%s' ORDER BY COLUMN_NAME ASC"), table);

	INT nError;
	nError = m_dmSrc.RestructDataManager(sql);
	if ( nError < 0 ) return -1;

    return 0;
}

INT CDBSyncDlg::RunSchemaGetDst(CString table)
{
    if ( table.IsEmpty() ) return -1;

    CString conn;
    conn = GetDstConnection();
    if ( conn.IsEmpty() ) return -1;

    m_dmDst.SetCONNECTION_INFO(conn);

	CString sql;
	sql.Format(_T("SELECT COLUMN_NAME, DATA_TYPE, DATA_LENGTH, NULLABLE, DATA_DEFAULT, CHAR_COL_DECL_LENGTH FROM COLS WHERE TABLE_NAME = '%s' ORDER BY COLUMN_NAME ASC"), table);

	INT nError;
	nError = m_dmDst.RestructDataManager(sql);
	if ( nError < 0 ) return -1;

    return 0;
}

CString CDBSyncDlg::RunSchemaCompare()
{
    INT nSrcRow;
    nSrcRow = m_dmSrc.GetRowCount();

    INT nDstRow;
    nDstRow = m_dmDst.GetRowCount();

    if ( nSrcRow != nDstRow ) {
        return _T("X");
    }

    INT i;
    for ( i = 0 ; i < nSrcRow ; i++ ) {
        if ( m_dmSrc.GetCellData(i, 0) != m_dmDst.GetCellData(i, 0) ) {
            return _T("X");
        }
        if ( m_dmSrc.GetCellData(i, 1) != m_dmDst.GetCellData(i, 1) ) {
            return _T("X");
        }
        if ( m_dmSrc.GetCellData(i, 2) != m_dmDst.GetCellData(i, 2) ) {
            if ( m_dmSrc.GetCellData(i, 5) != m_dmDst.GetCellData(i, 5) ) {
                return _T("X");
            }
        }
        if ( m_dmSrc.GetCellData(i, 3) != m_dmDst.GetCellData(i, 3) ) {
            return _T("X");
        }
        if ( m_dmSrc.GetCellData(i, 4) != m_dmDst.GetCellData(i, 4) ) {
            return _T("X");
        }
    }
    return _T("O");
}

INT CDBSyncDlg::RunData()
{
    INT nError;

    INT cnt;
    cnt = m_pGrid->GetCount();

    INT i;
    for ( i = 0 ; i < cnt ; i++ ) {
        CString check;
        check = m_pGrid->GetAt(i, 0);

        CString table, field, where;
        table = m_pGrid->GetAt(i, 1);
        field = m_pGrid->GetAt(i, 2);
        where = m_pGrid->GetAt(i, 3);

        if ( check != _T("V") ) continue;

        nError = RunDataGetSrc(table, field, where);
        if ( nError < 0 ) continue;
        else if ( nError > 0 ) continue;

        nError = RunDataGetDst(table, field, where);
        if ( nError < 0 ) continue;
        else if ( nError > 0 ) continue;

        CString compare;
        compare = RunDataCompare();
        m_pGrid->SetAt(i, 4, compare);

        CString num;
        num.Format(_T("%d"), m_dmSrc.GetRowCount());
        m_pGrid->SetAt(i, 5, num);

        num.Format(_T("%d"), m_dmDst.GetRowCount());
        m_pGrid->SetAt(i, 6, num);
    }

    return 0;
}

INT CDBSyncDlg::RunDataGetSrc(CString table, CString field, CString where)
{
    if ( table.IsEmpty() ) return -1;
    if ( field.IsEmpty() ) return -1;

    CString conn;
    conn = GetSrcConnection();
    if ( conn.IsEmpty() ) return -1;

    m_dmSrc.SetCONNECTION_INFO(conn);

	CString sql;
    if ( where.IsEmpty() ) {
    	sql.Format(_T("SELECT %s FROM %s ORDER BY %s ASC"), field, table, field);
    } else {
        sql.Format(_T("SELECT %s FROM %s WHERE %s ORDER BY %s ASC"), field, table, where, field);
    }

	INT nError;
	nError = m_dmSrc.RestructDataManager(sql);
	if ( nError < 0 ) return -1;

    return 0;
}

INT CDBSyncDlg::RunDataGetDst(CString table, CString field, CString where)
{
    if ( table.IsEmpty() ) return -1;
    if ( field.IsEmpty() ) return -1;

    CString conn;
    conn = GetDstConnection();
    if ( conn.IsEmpty() ) return -1;

    m_dmDst.SetCONNECTION_INFO(conn);

	CString sql;
    if ( where.IsEmpty() ) {
    	sql.Format(_T("SELECT %s FROM %s ORDER BY %s ASC"), field, table, field);
    } else {
        sql.Format(_T("SELECT %s FROM %s WHERE %s ORDER BY %s ASC"), field, table, where, field);
    }

	INT nError;
	nError = m_dmDst.RestructDataManager(sql);
	if ( nError < 0 ) return -1;

    return 0;
}

CString CDBSyncDlg::RunDataCompare()
{
    INT nSrcRow;
    nSrcRow = m_dmSrc.GetRowCount();

    INT nDstRow;
    nDstRow = m_dmDst.GetRowCount();

    if ( nSrcRow != nDstRow ) {
        return _T("X");
    }

    INT i;
    for ( i = 0 ; i < nSrcRow ; i++ ) {
        if ( m_dmSrc.GetCellData(i, 0) != m_dmDst.GetCellData(i, 0) ) {
            return _T("X");
        }
    }

    return _T("O");
}

/////////////////////////////////////////////////////////////////////////////
// CDBSyncDlg message handlers

BOOL CDBSyncDlg::OnInitDialog()
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
    m_pGrid = new CESL_Grid();
	m_pGrid->SubclassDlgItem(IDC_gDATA, this);

    VERIFY(m_btnRun.AutoLoad(IDC_bRUN, this));
    VERIFY(m_btnDetail.AutoLoad(IDC_bDETAIL, this));
	VERIFY(m_btnCancel.AutoLoad(IDCANCEL, this));

    INT nError;
    nError = ReadCfg();
	if ( nError < 0 ) {
		AfxMessageBox(_T("설정파일이 존재하지 않거나 포맷이 틀립니다"), MB_ICONSTOP);
		return true;
    } else if ( nError > 0 ) {
        return true;
    }

    OnrSCHEMA();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CDBSyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CDBSyncDlg::OnPaint() 
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
HCURSOR CDBSyncDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CDBSyncDlg::OnbRUN() 
{
    INT nError;

    if ( IsSchema() ) {
        nError = RunSchema();
        if ( nError < 0 ) {
            AfxMessageBox(_T("비교 작업 중에 오류가 발생 했습니다"), MB_ICONSTOP);
            return;
        }
    } else {
        nError = RunData();
        if ( nError < 0 ) {
            AfxMessageBox(_T("비교 작업 중에 오류가 발생 했습니다"), MB_ICONSTOP);
            return;
        }
    }
}

VOID CDBSyncDlg::OnbDETAIL() 
{
    CDBSyncDetail dlg(this);
    if ( IsSchema() ) {
        dlg.m_nType = 1;
    } else {
        dlg.m_nType = 2;
    }
    dlg.DoModal();
 
}

VOID CDBSyncDlg::OnrSCHEMA() 
{
	((CButton*)GetDlgItem(IDC_rSCHEMA))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_rDATA))->SetCheck(false);

    INT nError;
    nError = LoadSchema();
	if ( nError < 0 ) {
		AfxMessageBox(_T("스키마를 Load하는데 실패 했습니다"), MB_ICONSTOP);
		return;
    } else if ( nError > 0 ) {
        return;
    }
}

VOID CDBSyncDlg::OnrDATA() 
{
	((CButton*)GetDlgItem(IDC_rSCHEMA))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_rDATA))->SetCheck(true);
	
    INT nError;
    nError = LoadData();
	if ( nError < 0 ) {
		AfxMessageBox(_T("Data를 Load하는데 실패 했습니다"), MB_ICONSTOP);
		return;
    } else if ( nError > 0 ) {
        return;
    }
}

