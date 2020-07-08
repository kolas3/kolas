// ExeExtractorDlg.cpp : implementation file
//
// Part of Sel-extracting Exe creation utility :
//		DO NOT RUN IT DIRECTLY, ExeCreator will create exes based on this ExeExtractor exe
// 
// Written by Kaushal Malhotra (malhotrakaushal@mantraonline.com)
// Copyright (c) 2001
//
// To use this utility, follow these steps:
//   - Run ExeCreator
//   - Add the files you want to include in Exe
//   - If you need another name/path for your Exe change the name in output filed to a desired pathnam
//   - Click Makeexe
//   - Next run the resultant exe and provide a path to place the extracted files
//  
// You can easily port this logic into your own program or if there are a lot of requests
//	I might create a class to isolate the processing part
//
//
// Warning: this code hasn't been subject to a heavy testing, so
// use it on your own risk. The author accepts no liability for the 
// possible damage caused by this code.
//
// Version 1.0  21 Dec 2001
//              Initial release

#include "stdafx.h"
#include "ExeExtractor.h"
#include "ExeExtractorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExeExtractorDlg dialog

CExeExtractorDlg::CExeExtractorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExeExtractorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExeExtractorDlg)
	m_FName = _T("");
	m_PatchCount = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CExeExtractorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExeExtractorDlg)
	DDX_Control(pDX, IDC_FNAME, m_CtlFName);
	DDX_Text(pDX, IDC_FNAME, m_FName);
	DDX_Text(pDX, IDC_PATCHCOUNT, m_PatchCount);
	DDV_MaxChars(pDX, m_PatchCount, 5);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExeExtractorDlg, CDialog)
	//{{AFX_MSG_MAP(CExeExtractorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_EXTRACTFILE, OnExtractfile)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExeExtractorDlg message handlers

BOOL CExeExtractorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Get registry info.
    HKEY    hKey;
    DWORD   dwReserved, dwType, n;

    TCHAR    szHome[80], szPk[80];

    dwReserved = 0;
    INT ids = RegOpenKey( HKEY_LOCAL_MACHINE,
                      _T("Software\\ECO\\KOLAS II Upgrade"),
                      &hKey );
    if ( ids != ERROR_SUCCESS ) {
        AfxMessageBox(_T("KOLAS II Upgrade 홈 정보를 찾지 못했습니다."));
        EndDialog(0);
        return FALSE;
    }

    n = 80;

    ids = RegQueryValueEx ( hKey,    // handle key
                            _T("LAST_PATCH_PK"),
                            NULL,
                            &dwType,
                            (UCHAR*)&szPk[0],
                            &n );
    if ( ids != ERROR_SUCCESS ) {
        _tcscpy(szPk, _T("0"));
        ids = RegSetValueEx ( hKey,    // handle key
                              _T("LAST_PATCH_PK"),
                              NULL,
                              REG_SZ,
                              (UCHAR*)&szPk[0],
                              1 );
        ((CEdit*)GetDlgItem(IDC_PATCHCOUNT))->SetReadOnly(FALSE);
    }
    m_PatchCount = CString(szPk);

    n = 80;

    ids = RegQueryValueEx ( hKey,    // handle key
                            _T("HOME"),
                            NULL,
                            &dwType,
                            (UCHAR*)&szHome[0],
                            &n );
    if ( ids != ERROR_SUCCESS ) {
		AfxMessageBox(_T("KOLAS II Upgrade 홈정보를 가져오지 못했습니다."));
        EndDialog(0);
        return FALSE;
    }
    m_strHome = CString(szHome);

    if (m_strHome.IsEmpty()) {
        AfxMessageBox(_T("REGISTRY에 [KOLAS II Upgrade] 정보가 존재하지 않습니다.\n"
                         _T("KOLAS II Upgrad프로그램을 재설치 해 주시기 바랍니다.")));
        EndDialog(0);
        return FALSE;
    }
    m_FName = m_strHome + CString(_T("BIN"));

    UpdateData(FALSE);
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CExeExtractorDlg::OnPaint() 
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
HCURSOR CExeExtractorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Open Brwose for directory dialog box and return the name of directory selected
CString CExeExtractorDlg::BrowseForFolder()
{
	// We're going to use the shell to display a 
	// _T("Choose Directory") dialog box for the user.

	CString strResult = _T("");

	LPMALLOC lpMalloc;  // pointer to IMalloc

	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return strResult; // failed to get allocator  TCHAR szDisplayName[_MAX_PATH];

	TCHAR szBuffer[_MAX_PATH];
	BROWSEINFO browseInfo;

	browseInfo.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	// set root at Desktop
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szBuffer;
	browseInfo.lpszTitle = _T("Please choose a folder for extraction");   // passed in
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS ;   // also passed in
	browseInfo.lpfn = NULL;      // not used
	browseInfo.lParam = 0;      // not used

	LPITEMIDLIST lpItemIDList;

	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the
		// item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path 
			// the user chose.
			if (szBuffer[0] == '\0')
			{
				// SHGetPathFromIDList failed, or
				// SHBrowseForFolder failed.
				AfxMessageBox(_T("Cannot use this path for extraction"));
				return strResult;
			}

			// We have a path in szBuffer!
			// Return it.
			strResult = szBuffer;
			return strResult;
		}
		else
		{
			// The thing referred to by lpItemIDList 
			// might not have been a file _tsystem object.
			// For whatever reason, SHGetPathFromIDList
			// didn't work!
			AfxMessageBox(_T("Cannot use this path for extraction"));
			return strResult; // strResult is empty 
		}
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();      
	}
	// If we made it this far, SHBrowseForFolder failed.
	return strResult;
}

// Show browse dialog box whenever browse button is clicked
VOID CExeExtractorDlg::OnBrowse()
{
	CString strFName = CExeExtractorDlg::BrowseForFolder();

	// If any directory is selected update edit box
	if(!strFName.IsEmpty())
	{
		m_FName = strFName;
		UpdateData(FALSE);
		m_CtlFName.SetFocus();
		m_CtlFName.SetSel(0,-1);
	}
}

// Extract the file(s) in resource to the selected directory
VOID CExeExtractorDlg::OnExtractfile() 
{
	UpdateData();
	// Ensure we have \ at the end
	if(!m_FName.IsEmpty() && m_FName[m_FName.GetLength() - 1] != '\\')
	{
		m_FName += '\\';
		UpdateData(FALSE);
	}

	// Find and Load the resource containing file(s) data
	HRSRC hResLoad = FindResource(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_IDR_EXE_EXEEXTRACTOR),_T("IDR_EXE"));
	HGLOBAL hResData = LoadResource(AfxGetResourceHandle(),hResLoad);
	LPCTSTR data = (LPCTSTR)LockResource(hResData);
	LPCTSTR prev_data = data;

	// We should have properly loaded the resource
	ASSERT(data != NULL && hResData != NULL);

	// First double word in data contains number of files in resource
	DWORD nFiles = *(DWORD *)data;
	data += sizeof(DWORD);
	for(DWORD nCtr = 0;nCtr < nFiles;++nCtr)
	{
		// Data is pointing to file name, use it with directory selected by user to create the final file name
		CString strFName = m_FName + data;
		data += CString(data).GetLength() + 1;
		FILE* fp = _tfopen(strFName,_T("wb"));
		// We might not create file if user selected invalid directory
		// We can add some error handling to ensure proper directory is selected but
		//		I guess this solves our purpose at this time
		if(fp == NULL)
		{
			AfxMessageBox(_T("Cannot create file ") + strFName);
			return;
		}
		// Data now contains file size in bytes followed by actual file
		fwrite(data + sizeof(UINT),1,*(UINT *)data,fp);
		fclose(fp);
		data = (data + *(UINT *)data) + sizeof(UINT);
	}

	// We should have reached the end of resurce by now
	ASSERT((DWORD)(data - prev_data) == SizeofResource(AfxGetResourceHandle(),hResLoad));

	// Perform cleanup
	FreeResource(hResData);

    // set registry info.
    HKEY    hKey;
    TCHAR    szPk[80];

    INT ids = RegOpenKey( HKEY_LOCAL_MACHINE,
                      _T("Software\\ECO\\KOLAS II Upgrade"),
                      &hKey );
    if ( ids != ERROR_SUCCESS ) {
        AfxMessageBox(_T("레지스트리 수정을 실패했습니다."));
        EndDialog(0);
    }

    _tcscpy(szPk, m_PatchCount);
    ids = RegSetValueEx ( hKey,    // handle key
                          _T("LAST_PATCH_PK"),
                          NULL,
                          REG_SZ,
                          (UCHAR*)&szPk[0],
                          80 );

    CString strMsg;
	strMsg.Format(_T("설치가 완료되었습니다."));
	AfxMessageBox(strMsg);
	EndDialog(IDOK);
}
