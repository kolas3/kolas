// FindBadDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindBadDll.h"
#include "FindBadDllDlg.h"

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
// CFindBadDllDlg dialog

CFindBadDllDlg::CFindBadDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindBadDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindBadDllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	//member attribute initialize
	m_strPath = _T("");
	m_pGrid = NULL;
	m_nVerifyCnt = 0;
	m_nFailCnt = 0;
	m_nGridRow = 1;
}

CFindBadDllDlg::~CFindBadDllDlg()
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

}

VOID CFindBadDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindBadDllDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindBadDllDlg, CDialog)
	//{{AFX_MSG_MAP(CFindBadDllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnPATH_FIND, OnbtnPATHFIND)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindBadDllDlg message handlers

BOOL CFindBadDllDlg::OnInitDialog()
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
	

	// Grid 초기화
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gridCHECK_LIST, this);
	m_pGrid->m_bFroceReverse = FALSE;
	InitGrid();
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}


VOID CFindBadDllDlg::InitGrid()
{
	
	m_pGrid->SetCols(0,2);
    m_pGrid->SetColumnInfo(0-1, _T("파일이름"),		200,			1);
	m_pGrid->SetColumnInfo(1-1, _T("경로"),	        400, 			1);

}


VOID CFindBadDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CFindBadDllDlg::OnPaint() 
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
HCURSOR CFindBadDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CFindBadDllDlg::OnbtnPATHFIND() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner =  this->GetSafeHwnd();
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	TCHAR szPath[MAX_PATH] = {0};
	SHGetPathFromIDList(pidl, szPath);    //szPath에  path이름이 들어가 있어요.
	GetDlgItem(IDC_edtFILE_PATH)->SetWindowText(szPath);


}

VOID CFindBadDllDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 검사 
	m_strPath = _T("");
	GetDlgItem(IDC_edtFILE_PATH)->GetWindowText(m_strPath);

	if(m_strPath.IsEmpty())
	{
		AfxMessageBox(_T("검사할 폴더를 입력해 주십시오!"));
		return;
	}
	
	m_nVerifyCnt = 0;
	m_nFailCnt = 0;
	m_nGridRow = 1;

	CheckFileInFolder(m_strPath);

	CString msg;
	msg.Format(_T("총 %d 개의 DLL을 검사하여  %d 개의 DLL에 문제가 발생 했습니다!"),m_nVerifyCnt,m_nFailCnt);
	AfxMessageBox(msg);
	

	return;
	CDialog::OnOK();
}

VOID CFindBadDllDlg::CheckFileInFolder(CString strFolderPath)
{
	
	CFileFind finder;
	CString strFileName;
	CString strFilePath;
	CString strFullName;

	BOOL bWorking = FALSE;

	strFilePath.Format(_T("%s\\*.*"),strFolderPath);
	bWorking = finder.FindFile(strFilePath);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();            
		strFileName=finder.GetFileName();
			
		// 도트이면 페얼런트 디렉토리와 현재 디렉토리
		if(finder.IsDots()) continue; 
			
		if (finder.IsDirectory()) 
		{
			CString strChildPath = finder.GetFilePath();
			CheckFileInFolder(strChildPath);
		}
		
		if(IsDllFile(strFileName))
		{
			m_nVerifyCnt++;

			strFullName = strFilePath+strFileName;
			
			if(VerifyDll(strFileName)<0)
			{
				PrintOnGrid(strFolderPath,strFileName,FALSE);
				m_nFailCnt++;
			}
			else
			{
				PrintOnGrid(strFolderPath,strFileName,TRUE);			
			}
		}

	}


}

BOOL CFindBadDllDlg::IsDllFile(CString strFileName)
{
	if(strFileName.IsEmpty()) return FALSE;
	
	INT nPos = strFileName.Find(_T("."));
	if(nPos<0) return FALSE;

	CString strFileForm;
	strFileForm = strFileName.Mid(nPos+1);
	
	strFileForm.MakeUpper();

	if(_T("DLL") == strFileForm) return TRUE;

	return FALSE;
}

INT CFindBadDllDlg::VerifyDll(CString strFileName)
{
	CString strErrMsg;
	HINSTANCE hInstLib = NULL;
    hInstLib = AfxLoadLibrary(strFileName);
  
	if (hInstLib == NULL)
	{
		return -1;
    } 	
	else 
	{
		AfxFreeLibrary(hInstLib);
	}

	return 0;
}

VOID CFindBadDllDlg::PrintOnGrid(CString strFolderPath,CString strFileName,BOOL bSuccess)
{
	m_nGridRow++;
	INT gridrow = m_nGridRow;


	m_pGrid->SetRows(gridrow);
	
	m_pGrid->SetRow(gridrow-1);
	m_pGrid->SetCol(0);
	m_pGrid->SetText(strFileName);

	m_pGrid->SetCol(1);
	m_pGrid->SetText(strFolderPath);

	
	if(!bSuccess)
	{
		SetGridRowUniqueColor(gridrow-2);
	}

	m_pGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );
}

VOID CFindBadDllDlg::SetGridRowUniqueColor(INT nRow)
{
	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		m_pGrid->SetRow(nRow + 1);
		
		for (INT i = 0 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(red);
			m_pGrid->SetCellForeColor(black);
		}
	}
	
	m_pGrid->SetCol(nCol);
}