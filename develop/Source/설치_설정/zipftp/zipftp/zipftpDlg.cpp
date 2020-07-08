// zipftpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zipftp.h"
#include "zipftpDlg.h"
#include "Iphlpapi.h"

#include "..\zipunzip\zipper.h"
#include "..\shared\folderdialog.h"
#include "..\shared\filemisc.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
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
// CZipftpDlg dialog

CZipftpDlg::CZipftpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZipftpDlg::IDD, pParent)
{
//	m_strIP = _T("192.168.1.126");
//	m_strID = _T("administrator");
//	m_strPass = _T("roqkf55");
	m_strIP = _T("222.231.47.131");
	m_strID = _T("smartuadmin");
	m_strPass = _T("suadm.=/");
	m_pConnection = NULL;
    m_pFileFind = NULL;
	//{{AFX_DATA_INIT(CZipftpDlg)
	m_stcStatus = _T("");
	m_edtFolderPath = _T("");
	m_strLibName = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	EnableThemeDialogTexture(GetSafeHwnd()); 
}
CZipftpDlg::~CZipftpDlg()
{
	if (m_pConnection != NULL) 
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	if (m_pFileFind != NULL) 
	{
        delete m_pFileFind;
        m_pFileFind = NULL;
    }
}

void CZipftpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZipftpDlg)
	DDX_Text(pDX, IDC_stcStatus, m_stcStatus);
	DDX_Text(pDX, IDC_edtFOLDERPATH, m_edtFolderPath);
	DDX_Text(pDX, IDC_edtLibName, m_strLibName);
	DDX_Text(pDX, IDC_edtUserName, m_strUserName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CZipftpDlg, CDialog)
	//{{AFX_MSG_MAP(CZipftpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnBROWSEFOLDER, OnbtnBROWSEFOLDER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZipftpDlg message handlers

BOOL CZipftpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString strPath;
	GetPath(strPath);
	CEdit* edtFolderPath = (CEdit*)GetDlgItem(IDC_edtFOLDERPATH);
	edtFolderPath->SetWindowText(strPath);

	CEdit* edtStatus = (CEdit*)GetDlgItem(IDC_stcStatus);
	edtStatus->SetWindowText(_T("로그를 전송하시겠습니까?"));

	InitDate();

// 	GetFtpEnvvar();

	SetMyInfo();

	GetMyIP();

	EnableThemeDialogTexture(GetSafeHwnd());	

	
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

INT CZipftpDlg::InitDate()
{
	SYSTEMTIME systime;	
	CDateTimeCtrl* pStartDate = (CDateTimeCtrl*) GetDlgItem(IDC_pickStartDate);
	CDateTimeCtrl* pEndDate = (CDateTimeCtrl*) GetDlgItem(IDC_pickEndDate);

	pStartDate->GetTime(&systime);
	DateTime_SetSystemtime(pStartDate->m_hWnd , GDT_NONE , &systime );

	pEndDate->GetTime(&systime);
	DateTime_SetSystemtime(pEndDate->m_hWnd , GDT_NONE , &systime );
	
	return 0;
}


INT CZipftpDlg::GetPath(CString &strPath)
{
	CString path;
	CString strTemp;
	INT nFind;
	path = GetCommandLine();
	path.Replace(_T("\""),_T(""));
	nFind = path.Find(_T("bin"));
	strTemp.Format(_T("bin"));
	if( nFind < 0 )
	{
		nFind = path.Find(_T("BIN"));
		strTemp.Format(_T("BIN"));
	}
	if( 0 < nFind )
	{
		strPath.Format(_T("%stunning"),path.Mid(0,nFind));
	}
	m_strEnvvarPath = strPath;
	m_strEnvvarPath.Replace(_T("tunning"),_T("bin"));
	
	return 0;
}

void CZipftpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CZipftpDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZipftpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CZipftpDlg::OnbtnBROWSEFOLDER() 
{
	CFolderDialog dialog(_T("Select Folder to Zip"), m_edtFolderPath, this, BIF_RETURNONLYFSDIRS | BIF_BROWSENOROOTDIR);

	if (dialog.DoModal() == IDOK)
	{
		m_edtFolderPath = dialog.GetFolderPath();
		m_sFilePath.Empty();
		UpdateData(FALSE);
	}
	
}

void CZipftpDlg::OnZip() 
{
	UpdateData();

	CZipper zip;
	BOOL bRes = FALSE;
	CString sZipPath;

	SYSTEMTIME systime;

	CString strStartDate;
	CString strEndDate;
	
	CDateTimeCtrl* pStartDate = (CDateTimeCtrl*) GetDlgItem(IDC_pickStartDate);
	CDateTimeCtrl* pEndDate = (CDateTimeCtrl*) GetDlgItem(IDC_pickEndDate);

	pStartDate->GetWindowText(strStartDate);
	strStartDate.Replace(_T("-"),_T(""));
	pEndDate->GetWindowText(strEndDate);
	strEndDate.Replace(_T("-"),_T(""));

	pStartDate->GetTime(&systime);
	if( DateTime_GetSystemtime(pStartDate->m_hWnd ,&systime) == GDT_NONE )
	{
		
		strStartDate.Empty();		
	}
	else
	{
		pStartDate->GetWindowText(strStartDate);
		strStartDate.Replace(_T("-"),_T(""));
		
	}

	pEndDate->GetTime(&systime);
	if( DateTime_GetSystemtime(pEndDate->m_hWnd ,&systime) == GDT_NONE )
	{
		
		strEndDate.Empty();	
	}
	else
	{
		pEndDate->GetWindowText(strEndDate);
		strEndDate.Replace(_T("-"),_T(""));					
	}	

	if (!m_sFilePath.IsEmpty())
	{
		char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
		_splitpath(m_sFilePath, szDrive, szFolder, szName, NULL);

		char szZipPath[MAX_PATH];
		_makepath(szZipPath, szDrive, szFolder, szName, "zip");

		CWaitCursor cursor;

		if (zip.OpenZip(szZipPath, NULL, m_bAddToZip == TRUE))
			bRes = zip.AddFileToZip(m_sFilePath);

		sZipPath = szZipPath;
	}	
	else if (!m_edtFolderPath.IsEmpty())
	{
		sZipPath = m_edtFolderPath + ".zip";

		CWaitCursor cursor;

		bRes = CZipper::ZipFolder(m_edtFolderPath, m_bIgnorePath == TRUE, strStartDate, strEndDate);
	}

	zip.CloseZip();

	if (bRes)
	{
		Z_FileInfo zi;
		zip.GetFileInfo(zi);

		m_nFileCount = zi.nFileCount;
		m_nFolderCount = zi.nFolderCount;
		m_nTotalUncompSize = zi.dwUncompressedSize;
		m_nTotalCompSize = (DWORD)GetFileSize(sZipPath);
	}
	else
	{
		m_nFileCount = 0;
		m_nFolderCount = 0;
		m_nTotalUncompSize = 0;
		m_nTotalCompSize = 0;
	}

	UpdateData(FALSE);
}

void CZipftpDlg::OnOK() 
{
	CString strName;

	CEdit* pEditLibName = (CEdit*)GetDlgItem( IDC_edtLibName );
	CEdit* pEditUserName = (CEdit*)GetDlgItem( IDC_edtUserName );

	pEditLibName->GetWindowText(strName);
	strName.TrimLeft();	strName.TrimRight();
	
	if( strName.IsEmpty() ) 
	{
		AfxMessageBox(_T("도서관 이름을 입력해 주세요!"));
		return;
	}
	pEditUserName->GetWindowText(strName);
	strName.TrimLeft();	strName.TrimRight();

	if( strName.IsEmpty() ) 
	{
		AfxMessageBox(_T("사용자 이름을 입력해 주세요!"));
		return;
	}



	CEdit* edtStatus = (CEdit*)GetDlgItem(IDC_stcStatus);
	edtStatus->SetWindowText(_T("파일을 압축 중 입니다."));
	OnZip() ;
	edtStatus->SetWindowText(_T("파일을 전송 중 입니다."));
	OnConnect();
	edtStatus->SetWindowText(_T("전송을 완료 하였습니다."));
// 	SetFtpEnvvar();
}

void CZipftpDlg::OnConnect() 
{
//  if (!AfxSocketInit()) 
//	{
//        AfxMessageBox(_T("Failed socket initialization"));
//  }
	CTime t = CTime::GetCurrentTime();
	INT nYear;
	INT nMonth;
	INT nDay;
	CString strDir;
	CString strLocalFilePath;
	CString strTemp;
	nYear = t.GetYear();
	nMonth = t.GetMonth();
	nDay = t.GetDay();

	if (m_pConnection != NULL) 
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
	
	if (m_pFileFind != NULL) 
	{
        delete m_pFileFind;
        m_pFileFind = NULL;
    }


    m_pConnection = m_Session.GetFtpConnection(m_strIP, m_strID, m_strPass);
		
    if (!m_pConnection) {
        m_pConnection = NULL;
        AfxMessageBox(_T("m_Session.GetFtpConnection() failed"));
    }

	m_pConnection->GetCurrentDirectory(sRemoteDir);
	
	strDir = _T("LibLogFile");
	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		m_pConnection->CreateDirectory(strDir);
		m_pConnection->SetCurrentDirectory(strDir);
	}


	strDir = m_strLibName;
	strDir.TrimLeft();	strDir.TrimRight();
	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		m_pConnection->CreateDirectory(strDir);
		m_pConnection->SetCurrentDirectory(strDir);
	}

	CString strDate;
	strDir.Format(_T("%04d%02d%02d"),nYear,nMonth,nDay);
	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		m_pConnection->CreateDirectory(strDir);
		m_pConnection->SetCurrentDirectory(strDir);
	}

	INT nVersion;
	CString strVersion;
	CString strCurDir;
	CString strIP;
	CString strFileName;
	CString strFtpSeverFileName;
	

	( (CEdit*) GetDlgItem (IDC_edtIP) )->GetWindowText(strIP);

	nVersion = 1;

	m_pConnection->GetCurrentDirectory(strCurDir);
	m_pFileFind = new CFtpFileFind(m_pConnection);
	BOOL bContinue = m_pFileFind->FindFile(strCurDir);
	while(bContinue)
	{
		bContinue = m_pFileFind->FindNextFile();
		strFtpSeverFileName = m_pFileFind->GetFileName();
		//디렉토리인지 폴더인지 확인
		if( !m_pFileFind->IsDirectory())
		{
			CString strTemp;
			CString strFtpSerVer;
			INT nFtpSerVer;
			INT nFirstPos;
			INT nLastPos;

			nFirstPos = strFtpSeverFileName.Find(_T("_"));
			nLastPos = strFtpSeverFileName.Find(_T("_"),nFirstPos+1);
			nFtpSerVer = _ttoi(strFtpSeverFileName.Mid(nFirstPos+1, nLastPos - nFirstPos-1)); 

			if( nFtpSerVer == nVersion )
			{
				++nVersion;
			}
			else if( nFtpSerVer > nVersion )
			{
				nVersion = nFtpSerVer+1;
			}
		}
	}
	strFileName.Format(_T("%s_%d_%s.zip"),m_strUserName,nVersion,strIP);

	strLocalFilePath.Format(_T("%s.zip"), m_edtFolderPath);
	m_pConnection->PutFile(strLocalFilePath,strFileName);	

	m_pConnection->Close();

	DeleteFile(strLocalFilePath);
}




void CZipftpDlg::OnCancel() 
{	
 	CDialog::OnCancel();
}

HBRUSH CZipftpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
//
//BOOL CZipftpDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
//{
//	// TODO: Add your specialized code here and/or call the base class
//	
//	return CDialog::Create(IDD, pParentWnd);
//}


VOID CZipftpDlg::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));	// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);	// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}



CString CZipftpDlg::GetLocalNetID()
{
	DWORD dwStatus;
    IP_ADAPTER_INFO *pAdapterInfo = NULL;
    IP_ADAPTER_INFO *pOriginalPtr = NULL;
    ULONG ulSizeAdapterInfo = 0;
    dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);
    //***********************************************************************
    //버퍼 오버 플로우 일때 ulSizeAdapterInfo 크기로 메모리를 할당하고 
    //다시 GetAdaptersInfo를 호출한다.
    if( dwStatus == ERROR_BUFFER_OVERFLOW)
    {
        if( !(pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo)) )
        {            
            return "";
        }
        dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);
    }
	
    //***********************************************************************
    char MacAddr[18];
    pOriginalPtr = pAdapterInfo;
    if( pAdapterInfo == NULL )
    {
        return "";
    }
	//Nic카드가 여러개 있다면 여기서 처리하면되겠죠..
    while( pAdapterInfo != NULL )
    {
		//여기서는 맥주소만을 처리합니다.
        if( pAdapterInfo->AddressLength == 6 )
        {
            wsprintf(MacAddr,"%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X",
                (DWORD)pAdapterInfo->Address[0],                
                (DWORD)pAdapterInfo->Address[1],                
                (DWORD)pAdapterInfo->Address[2],                
                (DWORD)pAdapterInfo->Address[3],                
                (DWORD)pAdapterInfo->Address[4],                
                (DWORD)pAdapterInfo->Address[5]);           
        }
        pAdapterInfo = pAdapterInfo->Next;
    }
    free(pAdapterInfo);

	CString strMacAddr;
	strMacAddr.Format(_T("%s"), MacAddr);    
	
    return strMacAddr;
}

VOID CZipftpDlg::GetMyIP()
{
    
    char host[15],ipName[20];       
   HOSTENT* hostent;            

    gethostname(host,15);         
    hostent=gethostbyname(host);    
    
    
    
    memcpy(ipName,inet_ntoa(*(struct in_addr *)*hostent->h_addr_list),20);    
                                                  
      CString strIpName;                    
    strIpName.Format("%s",ipName);         

	( (CEdit*) GetDlgItem (IDC_edtIP) )->SetWindowText(strIpName);

}
VOID CZipftpDlg::SetMyInfo(CString strInfo)
{
	m_strInfo = strInfo;
}

VOID CZipftpDlg::SetMyInfo()
{	
	if ( m_strInfo.IsEmpty() ) return;

	CString strLibName;
	CString strLibUser;

	( (CEdit*) GetDlgItem (IDC_edtLibName) )->GetWindowText(strLibName);
	( (CEdit*) GetDlgItem (IDC_edtUserName) )->GetWindowText(strLibUser);

	if ( !m_strLibrary.IsEmpty() && !m_strUser.IsEmpty() ) return;

	INT nOffset;
	nOffset = m_strInfo.FindOneOf(_T("/"));
	if ( nOffset < 0 ) return;
	m_strLibrary = m_strInfo.Left(nOffset);
	m_strUser = m_strInfo.Mid(nOffset+1);
	
	if ( strLibName.IsEmpty() ) ( (CEdit*) GetDlgItem (IDC_edtLibName) )->SetWindowText(m_strLibrary);
	if ( strLibUser.IsEmpty() ) ( (CEdit*) GetDlgItem (IDC_edtUserName) )->SetWindowText(m_strUser);

}



//=====================================================
//2008.11.26 PJW : 필요없어진 코드
INT CZipftpDlg::GetFtpEnvvar()
{
	INT ids, i ;
	INT nFind;
	ids = i = 0;
	CString strTemp;
	CString strFirstData;
	CString strSecondData;
	CStdioFile file;
// 	CString strEnvvarPath;

	CString filename;

	CEdit* pEditLibName = (CEdit*)GetDlgItem( IDC_edtLibName );
	CEdit* pEditUserName = (CEdit*)GetDlgItem( IDC_edtUserName );

// 	m_strEnvvarPath.FIND
// 	m_strEnvvarPath.Replace(_T(""));
// 

// 	strEnvvarPath = m_strEnvvarPath;
// 	strEnvvarPath.Replace(_T("bin"));
//	filename.Format(_T("%s\\LogUpduate\\LogFtpEnvvar.txt"),strEnvvarPath);	
 	filename.Format(_T("%s\\LogUpduate\\LogFtpEnvvar.txt"),m_strEnvvarPath);	


	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("파일열기에 실패했습니다."));
		return -1;
	}	
	while(file.ReadString(strTemp))
	{
		strTemp.TrimLeft();	strTemp.TrimRight();
		nFind = strTemp.Find(_T("|"));
		strFirstData = strTemp.Mid(0,nFind);
		strSecondData = strTemp.Mid(nFind+1,strTemp.GetLength() - nFind);
		if( _T("도서관명") == strFirstData )
		{
			pEditLibName->SetWindowText(strSecondData);
		}
		if( _T("사용자명") == strFirstData )
		{
			pEditUserName->SetWindowText(strSecondData);
		}
	}

	file.Close();
	
	return 0;
}

INT CZipftpDlg::SetFtpEnvvar()
{
	INT ids, i ;
	ids = i = 0;
	CString strTemp;
	CString	strWriteDate;
	CString strFirstData;
	CString strSecondData;
	CStdioFile file;
// 	CString strEnvvarPath;

	CString filename;
	//m_Envvar
//	CEdit* pEditLibName = (CEdit*)GetDlgItem( IDC_edtLibName );
//	CEdit* pEditUserName = (CEdit*)GetDlgItem( IDC_edtUserName );

// 	strEnvvarPath = m_strEnvvarPath;
// 	strEnvvarPath.Replace(_T("bin"));
// 	filename.Format(_T("%s\\LogUpduate\\LogFtpEnvvar.txt"),strEnvvarPath);	

	filename.Format(_T("%s\\LogUpduate\\LogFtpEnvvar.txt"),m_strEnvvarPath);	

	if ( file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == FALSE ) 
	{
		AfxMessageBox(_T("파일열기에 실패했습니다."));
		return -1;
	}
	else
	{
// 		pEditLibName->GetWindowText(strTemp);
		strTemp = m_strLibName;
		strTemp.TrimLeft();	strTemp.TrimRight();
		strWriteDate.Format(_T("도서관명|%s\r\n"),strTemp);
		file.WriteString(strWriteDate);
// 		pEditUserName->GetWindowText(strTemp);
		strTemp = m_strUserName;
		strTemp.TrimLeft();	strTemp.TrimRight();
		strWriteDate.Format(_T("사용자명|%s\r\n"),strTemp);
		file.WriteString(strWriteDate);
	}
	file.Close();
	
	return 0;
}
//=====================================================
