// DBBackupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBBackup.h"
#include "DBBackupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD WINAPI DBBackupProc(LPVOID lpParameter);
DWORD WINAPI RestoreProc(LPVOID lpParameter);

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
// CDBBackupDlg dialog

CDBBackupDlg::CDBBackupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBBackupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBBackupDlg)
	m_pass = _T("");
	m_sid = _T("");
	m_user = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pGroup = NULL;
    m_pTable = NULL;

	// KOL.UDF.022
    m_arrayTable.RemoveAll();
    m_arrayGroup.RemoveAll();
    m_arrayDesc.RemoveAll();
    m_arrayRelation1.RemoveAll();
    m_arrayRelation2.RemoveAll();
    m_arrayDistinctGroup.RemoveAll();
    m_sOraHome = _T("");
    m_sExpArg = _T("");
    m_sImpArg = _T("");
	m_bInitDlg = FALSE;
}

VOID CDBBackupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBBackupDlg)
	DDX_Text(pDX, IDC_ePASS, m_pass);
	DDX_Text(pDX, IDC_eSID, m_sid);
	DDX_Text(pDX, IDC_eUSER, m_user);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBBackupDlg, CDialog)
	//{{AFX_MSG_MAP(CDBBackupDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bBACKUP, OnbBACKUP)
	ON_BN_CLICKED(IDC_bRESTORE, OnbRESTORE)
	ON_BN_CLICKED(IDC_bREFRESH, OnbREFRESH)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_MESSAGE(ESL_GRID_CHOICE, OnChoice)
END_MESSAGE_MAP()

INT CDBBackupDlg::InitMemory()
{
    m_arrayTable.RemoveAll();
    m_arrayGroup.RemoveAll();
    m_arrayDesc.RemoveAll();
    m_arrayRelation1.RemoveAll();
    m_arrayRelation2.RemoveAll();
    m_arrayDistinctGroup.RemoveAll();

    m_sid.Empty();
    m_user.Empty();
    m_pass.Empty();
    m_sOraHome.Empty();
    m_sExpArg.Empty();
    m_sImpArg.Empty();

    return 0;
}

INT CDBBackupDlg::InitDisplay()
{
    m_pGroup->RemoveAll();
    m_pGroup->InitColumn(2);
	m_pGroup->SetColumnInfo(0, _T("선택"), 30, 4);
	m_pGroup->SetColumnInfo(1, _T("이름"), 150, 1);

    m_pTable->RemoveAll();
	m_pTable->InitColumn(5);
	m_pTable->SetColumnInfo(0, _T("선택"), 30, 4);
	m_pTable->SetColumnInfo(1, _T("이름"), 150, 1);
	m_pTable->SetColumnInfo(2, _T("파일"), 30, 4);
	m_pTable->SetColumnInfo(3, _T("크기"), 50, 7);
	m_pTable->SetColumnInfo(4, _T("설명"), 100, 1);

    return 0;
}

INT CDBBackupDlg::Load()
{
    INT nError;

    nError = InitMemory();
    if ( nError < 0 ) return -1;

    nError = LoadEnvCfg();
    if ( nError < 0 ) return -1;

    nError = LoadDataCfg();
    if ( nError < 0 ) return -1;

    nError = InitDisplay();
    if ( nError < 0 ) return -1;

    nError = LoadDisplay();
    if ( nError < 0 ) return -1;

    return 0;
}

INT CDBBackupDlg::LoadEnvCfg()
{

	CString path;
#ifdef _UNICODE
	path = _T("dbbackup_env_uni.cfg");
#else
	path = _T("dbbackup_env_mbcs.cfg");
#endif


	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) return -1;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

    const INT nMax = 32000;
	TCHAR szCfg[nMax];
	INT nRead = file.Read(szCfg, nMax);

#ifdef _UNICODE
	nRead /= 2;
#endif 

	szCfg[nRead] = NULL;
	file.Close();

    if ( _tcslen(szCfg) == 0 ) {
        file.Close();
        return -1;
    }

    INT pos;
	TCHAR name[64];
	TCHAR value[256];

	const TCHAR seps[] = _T("\n");
	TCHAR* token;
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

        CString upper;
        upper = name;
        upper.MakeUpper();

        if ( upper == _T("SID") ) {
            m_sid = value;
        } else if ( upper == _T("USER") ) {
            m_user = value;
        } else if ( upper == _T("PASS") ) {
            m_pass = value;
        } else if ( upper == _T("ORACLE_HOME") ) {
            m_sOraHome = value;
        } else if ( upper == _T("EXP_ARG") ) {
            m_sExpArg = value;
        } else if ( upper == _T("IMP_ARG") ) {
            m_sImpArg = value;
        }

		token = _tcstok(NULL, seps);
	}
    UpdateData(false);
    
    return 0;
}

INT CDBBackupDlg::LoadDataCfg()
{

	CString path;
#ifdef _UNICODE
	path = _T("dbbackup_data_uni.cfg");
#else
	path = _T("dbbackup_data_mbcs.cfg");
#endif


	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) return -1;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

    const INT nMax = 32000;
	TCHAR szCfg[nMax];
	INT nRead = file.Read(szCfg, nMax);

#ifdef _UNICODE
	nRead /= 2;
#endif 

	szCfg[nRead] = NULL;
	file.Close();

    if ( _tcslen(szCfg) == 0 ) {
        file.Close();
        return -1;
    }

    INT pos;
	TCHAR table[64];
	TCHAR group[256];
    TCHAR desc[256];

	const TCHAR seps[] = _T("\n");
	TCHAR* token;
	token = _tcstok(szCfg, seps);

#ifdef _UNICODE
	// 2byte를 제외하고 데이터가 있는지 체크한다
	token++;
#endif 

	if ( token[0] == NULL ) return 0;

	while ( token != NULL ) {

		table[0] = NULL;
		group[0] = NULL;
		desc[0] = NULL;
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
		pos = GetToken(token, pos, group);
		pos = GetToken(token, pos, desc);

        LoadDataCfgImport(table, group, desc);

		token = _tcstok(NULL, seps);
	}
    
    return 0;
}

INT CDBBackupDlg::LoadDataCfgImport(CString table, CString group, CString desc)
{
    if ( table.IsEmpty() ) return 0;

    m_arrayTable.Add(table);
    m_arrayGroup.Add(group);
    m_arrayDesc.Add(desc);

    CStringArray arrayGroup;
    DevideString(group, _T(","), arrayGroup);

    INT i;
    for ( i = 0 ; i < arrayGroup.GetSize() ; i++ ) {

        INT idx_group;
        idx_group = -1;

        INT j;
        for ( j = 0 ; j < m_arrayDistinctGroup.GetSize() ; j++ ) {
            if ( m_arrayDistinctGroup[j] == arrayGroup[i] ) {
                idx_group = j;
            }
        }

        if ( idx_group == -1 ) {
            m_arrayDistinctGroup.Add(arrayGroup[i]);
            idx_group = m_arrayDistinctGroup.GetSize() - 1;
        }

        m_arrayRelation1.Add(m_arrayTable.GetSize()-1);
        m_arrayRelation2.Add(idx_group);
    }

    return 0;
}

INT CDBBackupDlg::LoadDisplay()
{
    INT size;
    INT i;

    size = m_arrayDistinctGroup.GetSize();
    for ( i = 0 ; i < size ; i++ ) {
        m_pGroup->SetAt(i, 1, m_arrayDistinctGroup[i]);
    }
    m_pGroup->DisplayNumber();

    size = m_arrayTable.GetSize();
    for ( i = 0 ; i < size ; i++ ) {
        m_pTable->SetAt(i, 1, m_arrayTable[i]);

        INT bExist, nSize;
        bExist = CheckFile(m_arrayTable[i], nSize);

        if ( bExist == 1 ) m_pTable->SetAt(i, 2, _T("V"));
        else m_pTable->SetAt(i, 2, _T(""));

        CString strSize;
        strSize.Format(_T("%d KB"), nSize);
        m_pTable->SetAt(i, 3, strSize);

        m_pTable->SetAt(i, 4, m_arrayDesc[i]);
    }
    m_pTable->DisplayNumber();

    return 0;
}

INT CDBBackupDlg::CheckFile(CString file, INT& size)
{
    if ( file.IsEmpty() ) return 0;
    size = 0;

    CString path;
    path.Format(_T("dmp\\%s.dmp"), file);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    
    hFind = FindFirstFile(path, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;

    } else {
        size = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
        size /= 1024;
        FindClose(hFind);
        return 1;
    }
}

INT CDBBackupDlg::DBBackup()
{
    UpdateData(true);
    if ( m_sid.IsEmpty() ) return -1;
    if ( m_user.IsEmpty() ) return -1;
    if ( m_pass.IsEmpty() ) return -1;
    if ( m_sOraHome.IsEmpty() ) return -1;

	DWORD id;
	HANDLE h;
    h = CreateThread(NULL, NULL, DBBackupProc, this, 0, &id);
	if (h == NULL) return -1;

    return 0;
}

INT CDBBackupDlg::DBBackupTable(CString table)
{
    // exp esl/esl#@esl tables=esl_mgr_tbl file=esl.dmp [EXP_ARG]
    if ( table.IsEmpty() ) return -1;

    TCHAR dir[1024];
    GetCurrentDirectory(1024, dir);

    CString arg;
    arg.Format(_T("%s/%s@%s tables=%s file=dmp\\%s.dmp %s"), m_user, m_pass, m_sid, table, table, m_sExpArg);


    CString path;
    path.Format(_T("%s\\bin\\exp"), m_sOraHome);

	SHELLEXECUTEINFO shExIf={0};

	shExIf.cbSize = sizeof( SHELLEXECUTEINFO );
	shExIf.cbSize = sizeof( SHELLEXECUTEINFO );
	shExIf.lpVerb = _T("open");
	shExIf.lpFile = path;
	shExIf.lpParameters = arg;
	shExIf.lpDirectory = dir;
	shExIf.nShow = SW_HIDE ;
	shExIf.fMask = SEE_MASK_NOCLOSEPROCESS;

	INT turn = ShellExecuteEx( &shExIf );

	DWORD result;
	result = WaitForSingleObject(shExIf.hProcess, INFINITE);

    return 0;
}

INT CDBBackupDlg::Restore()
{
    UpdateData(true);
    if ( m_sid.IsEmpty() ) return -1;
    if ( m_user.IsEmpty() ) return -1;
    if ( m_pass.IsEmpty() ) return -1;
    if ( m_sOraHome.IsEmpty() ) return -1;

	DWORD id;
	HANDLE h;
    h = CreateThread(NULL, NULL, RestoreProc, this, 0, &id);
	if (h == NULL) return -1;

    return 0;
}

INT CDBBackupDlg::RestoreFile(CString file)
{
    // imp esl/esl#@esl file=esl.dmp [IMP_ARG]
    if ( file.IsEmpty() ) return -1;

    TCHAR dir[1024];
    GetCurrentDirectory(1024, dir);

    CString arg;
    arg.Format(_T("%s/%s@%s file=dmp\\%s.dmp %s"), m_user, m_pass, m_sid, file, m_sImpArg);

    CString path;
    path.Format(_T("%s\\bin\\imp"), m_sOraHome);

	SHELLEXECUTEINFO shExIf={0};

	shExIf.cbSize = sizeof( SHELLEXECUTEINFO );
	shExIf.cbSize = sizeof( SHELLEXECUTEINFO );
	shExIf.lpVerb = _T("open");
	shExIf.lpFile = path;
	shExIf.lpParameters = arg;
	shExIf.lpDirectory = dir;
	shExIf.nShow = SW_HIDE ;
	shExIf.fMask = SEE_MASK_NOCLOSEPROCESS;

	INT turn = ShellExecuteEx( &shExIf );

	DWORD result;
	result = WaitForSingleObject(shExIf.hProcess, INFINITE);

    return 0;
}

VOID CDBBackupDlg::SetCount(CString table, INT cur, INT end)
{
    CString str;
    if ( cur < 0 ) 
        str.Format(_T("%s"), table);
    else 
        str.Format(_T("%s (%d/%d)"), table, cur, end);
    GetDlgItem(IDC_sPROCESS)->SetWindowText(str);
}
/////////////////////////////////////////////////////////////////////////////
// CDBBackupDlg message handlers

BOOL CDBBackupDlg::OnInitDialog()
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
	m_pGroup = new CESL_Grid();
	m_pGroup->SubclassDlgItem(IDC_gGROUP, this);

	m_pGroup->m_bGridEdit = false;
	m_pGroup->m_bMenu = true;
	m_pGroup->m_bCheck = true;

	m_pTable = new CESL_Grid();
	m_pTable->SubclassDlgItem(IDC_gTABLE, this);

	m_pTable->m_bGridEdit = false;
	m_pTable->m_bMenu = true;
	m_pTable->m_bCheck = true;

    _tmkdir(_T("dmp"));

    Load();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CDBBackupDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CDBBackupDlg::OnPaint() 
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
HCURSOR CDBBackupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CDBBackupDlg::OnbBACKUP() 
{
	// TODO: Add your control notification handler code here
    INT nError;
    nError = DBBackup();
    if ( nError < 0 ) {
        AfxMessageBox(_T("sid/user/pass를 입력 하십시오"));
        return;
    }

    Load();
}

VOID CDBBackupDlg::OnbRESTORE() 
{
	// TODO: Add your control notification handler code here
    INT nError;
    nError = Restore();
    if ( nError < 0 ) {
        AfxMessageBox(_T("sid/user/pass를 입력 하십시오"));
        return;
    }
}

VOID CDBBackupDlg::OnbREFRESH() 
{
	// TODO: Add your control notification handler code here
    Load();
}

VOID CDBBackupDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    if ( m_pGroup ) delete m_pGroup;
    m_pGroup = NULL;

    if ( m_pTable ) delete m_pTable;
    m_pTable = NULL;
	
}

VOID CDBBackupDlg::OnChoice(WPARAM w, LPARAM l)
{
    if ( m_arrayDistinctGroup.GetSize() == 0 ) return;
    if ( m_arrayTable.GetSize() == 0 ) return;

    if ( m_pGroup != (CESL_Grid*)w ) return;

    CString check;

    INT i;

    for ( i = 0 ; i < m_arrayTable.GetSize() ; i++ ) {
        m_pTable->SetAt(i, 0, _T(""));
    }


    for ( i = 0 ; i < m_arrayDistinctGroup.GetSize() ; i++ ) {
        m_pGroup->GetAt(i, 0, check);
        if ( check != _T("V") ) continue;

        INT j;
        for ( j = 0 ; j < m_arrayRelation2.GetSize() ; j++ ) {
            if ( m_arrayRelation2[j] == (UINT)i ) m_pTable->SetAt(m_arrayRelation1[j], 0, _T("V"));
        }
    }
}


DWORD WINAPI DBBackupProc(LPVOID lpParameter)
{
	CDBBackupDlg* dlg = (CDBBackupDlg*)lpParameter;

    dlg->SetCount(_T("Start.."), -1, -1);

    INT cnt;
    cnt = dlg->m_pTable->GetCount();

    INT i;
    for ( i = 0 ; i < cnt ; i++ ) {
        CString check;
        check = dlg->m_pTable->GetAt(i, 0);

        CString table;
        table = dlg->m_pTable->GetAt(i, 1);

        dlg->SetCount(table, i+1, cnt);

        if ( check == _T("V") ) {
            dlg->DBBackupTable(table);
        }
    }
    dlg->SetCount(_T("Completed.."), -1, -1);

    return 0;
}

DWORD WINAPI RestoreProc(LPVOID lpParameter)
{
	CDBBackupDlg* dlg = (CDBBackupDlg*)lpParameter;

    dlg->SetCount(_T("Start.."), -1, -1);

    INT cnt;
    cnt = dlg->m_pTable->GetCount();

    INT i;
    for ( i = 0 ; i < cnt ; i++ ) {
        CString check;
        check = dlg->m_pTable->GetAt(i, 0);

        CString table;
        table = dlg->m_pTable->GetAt(i, 1);

        CString file;
        file = dlg->m_pTable->GetAt(i, 2);

        dlg->SetCount(table, i+1, cnt);

        if ( check == _T("V") && file == _T("V") ) {
            dlg->RestoreFile(table);
        }
    }
    dlg->SetCount(_T("Completed.."), -1, -1);

    return 0;
}

