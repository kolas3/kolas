// ExeCreatorDlg.cpp : implementation file
//
// Sel-extracting Exe creation utility :
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
// Warning: this code hasn't been subject to a heavy testing, so
// use it on your own risk. The author accepts no liability for the 
// possible damage caused by this code.
//
// Version 1.0  21 Dec 2001
//              Initial release

#include "stdafx.h"
#include "ExeCreator.h"
#include "ExeCreatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExeCreatorDlg dialog

CExeCreatorDlg::CExeCreatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExeCreatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExeCreatorDlg)
	m_outFName = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CExeCreatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExeCreatorDlg)
	DDX_Control(pDX, IDC_INLIST, m_InList);
	DDX_Control(pDX, IDC_OUTFNAME, m_CtloutFName);
	DDX_Text(pDX, IDC_OUTFNAME, m_outFName);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	DDV_MaxChars(pDX, m_strVersion, 4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExeCreatorDlg, CDialog)
	//{{AFX_MSG_MAP(CExeCreatorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_MAKEEXE, OnMakeexe)
	ON_BN_CLICKED(IDC_INBROWSE, OnInBrowse)
	ON_BN_CLICKED(IDC_OUTBROWSE, OnOutBrowse)
	ON_BN_CLICKED(IDC_DELINFILES, OnDelinfiles)
	ON_EN_KILLFOCUS(IDC_VERSION, OnKillfocusVersion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExeCreatorDlg message handlers

BOOL CExeCreatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Get Home..
    HKEY    hKey;
    DWORD   dwReserved, dwType, n;

    TCHAR    szValue[80];

    dwReserved = 0;
    INT ids = RegOpenKey( HKEY_LOCAL_MACHINE,
                      _T("Software\\ECO\\KOLAS II Upgrade"),
                      &hKey );
    if ( ids != ERROR_SUCCESS ) {
        return -1;
    }

    n = 80;
    ids = RegQueryValueEx ( hKey,    // handle key
                            _T("HOME"),
                            NULL,
                            &dwType,
                            (UCHAR*)&szValue[0],
                            &n );
    if ( ids != ERROR_SUCCESS ) {
        return -2;
    }

    m_strHome = CString(szValue);

    if (m_strHome.IsEmpty()) {
        AfxMessageBox(_T("REGISTRY에 [KOLAS II Upgrade] 정보가 존재하지 않습니다."));
        EndDialog(0);
        return FALSE;
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CExeCreatorDlg::OnPaint() 
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
HCURSOR CExeCreatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Shows browse dialog box for addition of new files in list
VOID CExeCreatorDlg::OnInBrowse() 
{
	UpdateData();
	static CString strFName;
	CFileDialog dlg(TRUE,NULL,strFName,OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,_T("All Files(*.*)|*.*||"),this);
	if(dlg.DoModal() == IDOK)
	{
		// Deselect any selected items in list
		for(INT i = 0;i < m_InList.GetCount();++i)
			m_InList.SetSel(i,FALSE);
		// Add each file name selected to list
		for(POSITION pos = dlg.GetStartPosition();pos != NULL;)
		{
			strFName = dlg.GetNextPathName(pos);
/*
			// If this is the first file name in list create an exe name and set it as output file
			if(m_InList.GetCount() == 0)
			{
				if(strFName.ReverseFind('.') == -1)
					m_outFName += _T(".exe");
				else
					m_outFName = strFName.Left(strFName.ReverseFind('.')) + _T(".exe");
			}
*/
			// Add and Select new file name
			m_InList.SetSel(m_InList.AddString(strFName));
		}
		UpdateData(FALSE);
		m_InList.SetFocus();
	}
}

// delete the selected files in list
VOID CExeCreatorDlg::OnDelinfiles()
{
	// Create an array to store selected items
	INT nCount = m_InList.GetSelCount();
	CArray<INT,INT> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_InList.GetSelItems(nCount, aryListBoxSel.GetData());

	// Delete the selected items while considering that indexes will be reduced by deletion of an item
	for(INT i = 0;i < nCount;++i)
		m_InList.DeleteString(aryListBoxSel[i] - i);
}

// Show browse dialog box for output file name
VOID CExeCreatorDlg::OnOutBrowse()
{
	UpdateData();
	CFileDialog dlg(FALSE,NULL,m_outFName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All Files(*.)|*.*||"),this);
	if(dlg.DoModal() == IDOK)
	{
		m_outFName = dlg.GetPathName();
		UpdateData(FALSE);
		m_CtloutFName.SetSel(0,-1);
		m_CtloutFName.SetFocus();
	}
}

// Create the exe from the files added to list
VOID CExeCreatorDlg::OnMakeexe()
{
	UpdateData();
	if(m_InList.GetCount() == 0)
	{
		AfxMessageBox(_T("Please enter atleast one input file name"));
		return;
	}
	// Create a buffer which will be added as resource to the ExeExtractor exe
	// First byte in buffer is set to number of files in list
	LPCTSTR buff = (LPCTSTR)new DWORD(m_InList.GetCount());
	UINT nSize = sizeof(DWORD);
	// Add each file
	for(DWORD i = 0;i < *(DWORD *)buff;++i)
	{
		CString inFName;
		m_InList.GetText(i,inFName);

		// Open i file 
		FILE* fpIn = _tfopen(inFName,_T("rb"));
		if(fpIn == NULL)
		{
			CString errMsg;
			errMsg.Format(_T("Could not open %d file : %s"),i + 1,(LPCTSTR)inFName);
			AfxMessageBox(errMsg);
			delete buff;
			return;
		}

		// Retieve file size file 
		fseek(fpIn,0,SEEK_END);
		UINT nFSize = ftell(fpIn);
		fseek(fpIn,0,SEEK_SET);

		// Extract just file name from full pathname of file
		for(INT nCtr = 0,nMid = 0;nCtr < inFName.GetLength();++nCtr)
			if(inFName[nCtr] == '\\' || inFName[nCtr] == ':')
				nMid = nCtr + 1;
		CString onlyFName = inFName.Mid(nMid);

		// Realloc previous buffer to accomodate this file's info
		LPCTSTR newbuff = new TCHAR[nSize + (onlyFName.GetLength() + 1) + sizeof(UINT) + nFSize];
		wmemcpy(newbuff,buff,nSize);
		delete buff;
		buff = newbuff;

		// Copy file name with null character
		wmemcpy(buff + nSize,(LPCTSTR)onlyFName,onlyFName.GetLength() + 1);
		// Copy file size
		*(UINT *)(buff + nSize + onlyFName.GetLength() + 1) = nFSize;
		// Copy file data
		VERIFY(fread(buff + nSize + sizeof(UINT) + (onlyFName.GetLength() + 1),1,nFSize,fpIn) == nFSize);

		nSize += sizeof(UINT) + (onlyFName.GetLength() + 1) + nFSize;
		fclose(fpIn);
	}

	// Create a dummy EXE from ExeExtractor
	FILE* fpOut = _tfopen(m_outFName,_T("wb"));
	if(fpOut == NULL)
	{
		AfxMessageBox(_T("Could not create output file") + m_outFName);
		delete buff;
		return;
	}

	// Find Load and Lock resource
	HRSRC hResLoad = FindResource(NULL,MAKEINTRESOURCE(IDR_IDR_EXE_EXEEXTRACTOR),_T("IDR_EXE"));
	HGLOBAL hResData = LoadResource(NULL,hResLoad);
	LPCTSTR data = (LPCTSTR)LockResource(hResData);

    // Write the ExeCreator binary data to this file
	ASSERT(hResLoad != NULL && hResData != NULL && data != NULL);
	fwrite(data,1,SizeofResource(NULL,hResLoad),fpOut);
	fclose(fpOut);

	FreeResource(hResData);

	// Open the file for addition of file data in resource
	HANDLE hUpdateRes = BeginUpdateResource(m_outFName, FALSE);
	ASSERT(hUpdateRes != NULL);

	// Add the file data to the update list.
	VERIFY(UpdateResource(hUpdateRes,
		  _T("IDR_EXE"),MAKEINTRESOURCE(IDR_IDR_EXE_EXEEXTRACTOR),
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		  buff,nSize));
	delete buff;

	// Write changes and then close it. 
	VERIFY(EndUpdateResource(hUpdateRes, FALSE));

	// Perform cleanup
	CString strMsg;
	strMsg.Format(_T("%s successfully created from %d files."),m_outFName,m_InList.GetCount());
	AfxMessageBox(strMsg);
	m_InList.ResetContent();
	m_outFName = _T("");
    m_strVersion = _T("");
	UpdateData(FALSE);
}

VOID CExeCreatorDlg::OnKillfocusVersion() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
    m_outFName = m_strHome + CString(_T("SU")) + m_strVersion + CString(_T(".EXE"));
    UpdateData(FALSE);
}
