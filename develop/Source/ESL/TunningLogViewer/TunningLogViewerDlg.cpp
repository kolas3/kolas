// TunningLogViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TunningLogViewer.h"
#include "TunningLogViewerDlg.h"
//DEL #include "des.h"

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
// CTunningLogViewerDlg dialog

CTunningLogViewerDlg::CTunningLogViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTunningLogViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTunningLogViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szText = NULL;
	m_bIsLowerUpper = FALSE;
	m_nSearchDirection = CSearchDlg::SEARCH_TYPE_PREV_SEARCH;
	
	// KOL.UDF.022 시큐어코딩 보완
	m_strSearchText = _T("");
	m_strFile = _T("");
	m_dwTextSize = 0;
}

CTunningLogViewerDlg::~CTunningLogViewerDlg()
{
	if(NULL != m_szText) delete []m_szText;
	m_szText = NULL;
}
void CTunningLogViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTunningLogViewerDlg)
	DDX_Control(pDX, IDC_STATIC_MSG, mCtrl_staMSG);
	DDX_Control(pDX, IDC_EDIT_TEXT, mCtrl_edtView);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTunningLogViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CTunningLogViewerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(IDM_READ_TUNNING_FILE, OnReadTunningFile)
	ON_WM_DROPFILES()
	ON_COMMAND(IDM_SAVE_TUNNING_FILE_FOR_TEXT, OnSaveTunningFileForText)
	ON_COMMAND(IDM_FIND, OnFind)
	ON_COMMAND(IDM_HELP, OnHelp)
	ON_WM_NCACTIVATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTunningLogViewerDlg message handlers

BOOL CTunningLogViewerDlg::OnInitDialog()
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
	
	m_edtBrush.CreateSolidBrush(RGB(255,255,255)); 

	ResizeControl();

	// 파일을 실행파일위에 드래그 했을 시나 튜닝파일 더블클릭시의 처리
	CString strCmdLine = AfxGetApp()->m_lpCmdLine;
	if(_T("") != strCmdLine)
	{	
		int nStart = strCmdLine.Find('"');
		int nEnd = strCmdLine.Find('"', nStart+1);
		if(-1 != nStart && -1 != nEnd)
		{
			strCmdLine = strCmdLine.Mid(nStart+1, nEnd-nStart-1);
		}
		View_TunningFile(strCmdLine);
	}
	
	
	INT nCnt = mCtrl_edtView.GetLineCount();
	mCtrl_edtView.LineScroll(nCnt);
	mCtrl_edtView.SetSel(-2,-1);
	UpdateWindow();

	// TODO: Add extra initialization here
	return TRUE;
}

/*
#include "des.h"
BOOL CTunningLogViewerDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
		
	
	CString strKey = _T("TUNN");	
	CString strText = _T("[2008-09-18.14:07:14][][00000] SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE FROM MN_USER_TBL WHERE ID='ECO8' AND PASSWD='ECO&L");
	
	// 한번에 암/복호화하는 있는 글자단위
	// DES 알고리즘은 8바이트 블록 알고리즘이라 8바이트단위로 데이터를 끊어줘야 한다.
	// 유니코드일 때 8바이트는 4글자, ANSI일 경우 8글자이다.
#ifdef _UNICODE
#define DEC_UNIT 4
#else 
#define	DEC_UNIT 8
#endif
	
	CString strDecryptResult = _T("");
	CString strRemainDecryptText = strText;
	
	// 
	TCHAR* pKey = new TCHAR[DEC_UNIT];
	_tcscpy(pKey, strKey);
	TCHAR* pText = new TCHAR[DEC_UNIT];
	memset(pText, 0x00, DEC_UNIT);
	
	
	// 8바이트씩 끊어 암호화한 후에 strDecryptResult에 통합한다.
	while(0 < strRemainDecryptText.GetLength())
	{
		CString strDecText = strRemainDecryptText.Left(DEC_UNIT);
		strRemainDecryptText.Replace(strDecText, _T(""));
		// 남은 글자가 8바이트보다 작은 경우 공백을 추가해 8바이트로 만들어준다.
		while(DEC_UNIT > strDecText.GetLength())
		{
			strDecText = strDecText + _T(" ");
		}
		
		_tcscpy(pText, strDecText);
		DES_Decrypt((void*)pKey, (BYTE*)pText);	 	
		
		strDecText.Format(_T("%s"), pText);				
		strDecryptResult = strDecryptResult + strDecText;
	}
//DEL 	{ 
//DEL 		CString filename = _T("D:\\KOLASIII\\tunning\\ESL_DM_20080918.txt");
//DEL 		
//DEL 		CStdioFile file;
//DEL 		CFileFind findfile;	
//DEL 		if(TRUE == findfile.FindFile(filename))
//DEL 		{
//DEL 			if(FALSE == file.Open(filename,CFile::modeReadWrite|CFile::typeBinary)) return FALSE;
//DEL 			file.SeekToEnd();
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			if(TRUE == file.Open(filename,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
//DEL 			{
//DEL 				TCHAR tchar = 0xFEFF;
//DEL 				file.Write(&tchar, sizeof(TCHAR));		
//DEL 			}
//DEL 			else return FALSE;
//DEL 			
//DEL 		}	
//DEL 		file.WriteString(strDecryptResult);
//DEL 		//DEL 	file.WriteHuge(LPCTSTR(msg), msg.GetLength()* sizeof(TCHAR));
//DEL 		file.Close();
//DEL 	}
//DEL 	CStdioFile fd;
//DEL 	CString strLine;
//DEL 	if (fd.Open(_T("D:\\KOLASIII\\tunning\\ESL_DM_20080918.txt"), CStdioFile::modeReadWrite|CFile::typeBinary) == TRUE)
//DEL 	{
//DEL 		TCHAR cUni;
//DEL 		fd.Read( &cUni, sizeof(TCHAR) );
//DEL 		if( 0xFEFF != cUni )
//DEL 		{
//DEL 			fd.SeekToBegin();
//DEL 		}		
//DEL 		fd.ReadString(strLine);		
//DEL 	}
//DEL 	fd.Close();	
//DEL 	
	CString strEncryptResult = _T("");
	//DEL  	strLine.TrimRight(_T("\r\n"));
	//DEL 	CString strRemainEncryptText = strLine;
	CString strRemainEncryptText = strDecryptResult;
	
	// 8바이트씩 끊어 복호화한 후에 strEncryptResult에 통합한다.
	while(0 < strRemainEncryptText.GetLength())
	{
		CString strEncText = strRemainEncryptText.Left(DEC_UNIT);
		strRemainEncryptText.Replace(strEncText, _T(""));
		// 남은 글자가 8바이트보다 작은 경우
		// 암호화가 잘못되었다!!
		if(DEC_UNIT > strEncText.GetLength())
		{
			// 잘못된 암호화시의 처리
			break;
		}
		
		_tcscpy(pText, strEncText);
		DES_Encrypt((void*)pKey, (BYTE*)pText);
		
		strEncText.Format(_T("%s"), pText);
		strEncryptResult = strEncryptResult + strEncText;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
*/
void CTunningLogViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTunningLogViewerDlg::OnPaint() 
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
HCURSOR CTunningLogViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// 함수이름: CTunningLogViewerDlg::DES_Encrypt_From_File
// 함수설명: 함호화된 파일을 읽어온다.
// 반환  형: INT 
// 인자[CString strFileName] : 
// 인자[CString *strResult] : 
//DEL INT CTunningLogViewerDlg::DES_Encrypt_From_File(CString strFileName, CString strKey, CString *strResult)
//DEL {	
//DEL 	CStdioFile src;
//DEL 	CFileException ex;
//DEL 	if ( !src.Open(strFileName, CFile::modeRead|CFile::shareDenyWrite, &ex) )
//DEL 	{
//DEL 			TCHAR szMsg[1024];
//DEL 			ex.GetErrorMessage(szMsg, 1024);
//DEL 			AfxMessageBox(szMsg);
//DEL 			return -1;
//DEL 	}
//DEL //DEL 	TCHAR tBom;
//DEL //DEL 	src.Read(&tBom, sizeof(TCHAR));
//DEL //DEL 		CString strName = src.GetFileName();
//DEL //DEL 		CString strTitle;
//DEL //DEL 		CString strExt;
//DEL //DEL 		int nPos;
//DEL //DEL 		if ( (nPos = strName.ReverseFind(_T('.'))) == -1 ) {
//DEL //DEL 			strTitle = strName;
//DEL //DEL 			strExt = _T("");
//DEL //DEL 		} else {
//DEL //DEL 			strTitle = strName.Left(nPos);
//DEL //DEL 			strExt = strName.Right(strName.GetLength()-nPos-1);
//DEL //DEL 		}
//DEL //DEL 		CString strPath = src.GetFilePath();
//DEL //DEL 		nPos = strPath.ReverseFind(_T('\\'));
//DEL //DEL 		strPath = strPath.Left(nPos);
//DEL /*
//DEL 		
//DEL 		nLength = src.GetLength();
//DEL 	
//DEL 		uint32_t nPlainTextLen;
//DEL 		uint32_t nCipherTextLen = nLength;
//DEL 		
//DEL 		pPlainText = new uchar8_t[(nLength/8+1)*8+1];
//DEL 		pCipherText = new uchar8_t[(nLength/8+1)*8+1];
//DEL 		
//DEL 		memset(pPlainText, NULL, (nLength/8+1)*8+1);
//DEL 		memset(pCipherText, NULL, (nLength/8+1)*8+1);
//DEL 				
//DEL 		DWORD dwBytes;
//DEL 		DWORD dwTotal = 0;
//DEL //DEL 		do {			
//DEL 		TCHAR tBom;
//DEL 		src.Read(&tBom, sizeof(TCHAR));
//DEL   			dwBytes = src.Read(pCipherText+dwTotal, nLength);
//DEL  			dwTotal += dwBytes;
//DEL 
//DEL //DEL 		} while ( dwTotal != nLength );
//DEL */
//DEL 	CString strLine = _T("");
//DEL 	TCHAR tBom;
//DEL 	src.Read(&tBom, sizeof(TCHAR));
//DEL 	CString str_EncLine = _T("");
//DEL 	CString str_Result = _T("");
//DEL  	while(TRUE == src.ReadString(strLine))
//DEL 	{
//DEL //DEL 	TCHAR lpBuffer[4096];
//DEL //DEL 	DWORD dwPos = src.GetPosition();
//DEL //DEL 	while(TRUE)
//DEL //DEL 	{
//DEL //DEL 		memset(lpBuffer, NULL, 4096);
//DEL //DEL 		dwPos = src.GetPosition();
//DEL //DEL 		INT nCount = src.Read(lpBuffer, 4096);
//DEL //DEL 		if(!nCount)
//DEL //DEL 		{
//DEL //DEL 			break;
//DEL //DEL 		}
//DEL 
//DEL //DEL 		CharConvert cv;
//DEL //DEL 		TCHAR*       szTmp = NULL;
//DEL //DEL 		INT         len   = 0;
//DEL //DEL 		
//DEL //DEL 		cv.MultiByteToUnicode((char*)lpBuffer, &szTmp, &len);		
//DEL //DEL 		if( 0 < len)
//DEL //DEL 		{
//DEL //DEL 			sLine.Format( _T("%s"), szTmp );
//DEL //DEL 			delete [] szTmp;
//DEL //DEL 		}
//DEL //DEL 		strLine.Format(_T("%s"), lpBuffer);
//DEL //DEL 		INT nFindPos = strLine.Find(_T("\r\n"));
//DEL //DEL 		if(-1 == nFindPos) { break; }
//DEL //DEL 		strLine = strLine.Left(nFindPos);
//DEL //DEL 		src.Seek(dwPos + (DWORD)nFindPos+1, CFile::begin);	
//DEL 
//DEL 	
//DEL 		if(_T("") == strLine) continue;
//DEL 		strLine.TrimRight(_T("\r\n"));
//DEL 		DES_Encrypt_From_str(strLine, strKey, &str_EncLine);		
//DEL 		str_Result = str_Result + _T("\r\n") + str_EncLine;
//DEL 	}
//DEL 	*strResult = str_Result;
//DEL 	src.Close();
//DEL 
//DEL 		return 0;
//DEL }


//DEL INT CTunningLogViewerDlg::MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
//DEL {
//DEL 
//DEL 	_TCHAR	*pszDestinationData;
//DEL 	INT		nWCharNeeded;
//DEL 	INT		nSouceDataByte;
//DEL 	
//DEL 
//DEL 	nSouceDataByte = strlen(fi_pSourceData);
//DEL 	
//DEL 	/* Query the number of WChar required to store the Dest string */
//DEL 	nWCharNeeded = MultiByteToWideChar( CP_ACP, 
//DEL 										(UINT)MB_PRECOMPOSED  , 
//DEL 										fi_pSourceData   , 
//DEL 										nSouceDataByte   , 
//DEL 										NULL             , 
//DEL 										0 );
//DEL 	if (nWCharNeeded == 0) 
//DEL 	{
//DEL 		*fo_nByteDes = 0;
//DEL 		return -1;
//DEL 	}
//DEL 	
//DEL 
//DEL 	/* Allocate the required amount of space */	
//DEL 	/* We need more 2 bytes for '\0' */
//DEL 	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));	
//DEL 		
//DEL 	/* Do the conversion */
//DEL 	nWCharNeeded = MultiByteToWideChar(CP_ACP, 
//DEL 										MB_PRECOMPOSED						,
//DEL 										fi_pSourceData					, 
//DEL 										nSouceDataByte			   		,
//DEL 										pszDestinationData          	, 
//DEL 										nWCharNeeded);
//DEL 	if (nWCharNeeded == 0) 
//DEL 	{
//DEL 		*fo_nByteDes = 0;
//DEL 		free(pszDestinationData);
//DEL 		return -2;
//DEL 	}	
//DEL 
//DEL 	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
//DEL 	
//DEL 	/* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
//DEL 	*fo_nByteDes = 2*nWCharNeeded ;	
//DEL 
//DEL 	*fo_pDestinationData = pszDestinationData;
//DEL 
//DEL 
//DEL 	return 0;
//DEL }

//DEL INT CTunningLogViewerDlg::DES_Encrypt_From_str(CString strSource, CString strKey, CString *strResult)
//DEL {
//DEL 	
//DEL 	uchar8_t *pPlainText;
//DEL 	uchar8_t *pCipherText;
//DEL 
//DEL 	INT nLength = strSource.GetLength();
//DEL 
//DEL 	uint32_t nPlainTextLen;
//DEL 	uint32_t nCipherTextLen = nLength;
//DEL 	
//DEL 	pPlainText = new uchar8_t[(nLength/8+1)*8+1];
//DEL 	pCipherText = new uchar8_t[(nLength/8+1)*8+1];
//DEL 
//DEL 	memset(pPlainText, NULL, (nLength/8+1)*8+1);
//DEL 	memset(pCipherText, NULL, (nLength/8+1)*8+1);
//DEL 
//DEL 
//DEL 	TCHAR* _szTmp = new TCHAR[(nLength/8+1)*8+1];
//DEL 	memset(_szTmp, NULL, (nLength/8+1)*8+1);
//DEL 	_tcscpy(_szTmp, strSource.GetBuffer(0));
//DEL 
//DEL 	INT _nLen = (nLength/8+1)*8+1;
//DEL 	for(INT nCnt = 0; nCnt < _nLen; nCnt++)
//DEL 	{
//DEL 		(pCipherText[nCnt]) = ((unsigned char)(LPWSTR)_szTmp[nCnt]);
//DEL 	}
//DEL 
//DEL 	CFWDES des;
//DEL 	des.Decipher(ECB, (uchar8_t*)(LPCTSTR)strKey, nCipherTextLen, pCipherText, &nPlainTextLen, pPlainText, 16/*Round 수*/);
//DEL 		
//DEL #if defined _UNICODE
//DEL 		TCHAR* szTmp = NULL;
//DEL 		INT nLen = 0;
//DEL 		MultiByteToUnicode((char*)pPlainText, &szTmp, &nLen);
//DEL 		if(0 < nLen)
//DEL 		{
//DEL 			strResult->Format(_T("%s"),szTmp);	
//DEL 			delete []szTmp;					
//DEL 		}
//DEL #else	
//DEL 		strResult->Format(_T("%s"),pPlainText);
//DEL #endif		
//DEL 		delete [] pPlainText;
//DEL 		delete [] pCipherText;
//DEL 		
//DEL 	return 0;
//DEL }

void CTunningLogViewerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

INT CTunningLogViewerDlg::Read_TunningFile(CString strFile, TCHAR** szText, DWORD* dwTextSize)
{	
	CFile file2;
	CFileException ex;
	BOOL bIsOpen = file2.Open(strFile, CFile::modeRead, &ex);
	if(FALSE == bIsOpen)
	{
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		AfxMessageBox(szMsg);
		return -1001;
	}

	// 문자열의 길이 설정
 	const int LEN = file2.GetLength()/sizeof(TCHAR);
	
	TCHAR *pChar = new TCHAR[LEN+1];
	memset(pChar, NULL, LEN);	
	pChar[LEN] = '\0';
	
 	file2.ReadHuge(pChar, LEN*sizeof(TCHAR));
	file2.Close();

	// 디코딩
	for(INT nCnt=0; nCnt < LEN; nCnt++)
	{
		pChar[nCnt] ^= 0xAA;
	}
	
	*szText = pChar;
	*dwTextSize = LEN;

//DEL 	delete []pChar;

	return 0;
}

void CTunningLogViewerDlg::OnReadTunningFile() 
{
	TCHAR szPath[MAX_PATH] = {0}; 
	//현재의 작업경로를 얻어와 저장 한다. 
	GetCurrentDirectory(MAX_PATH, szPath);
  
	// TODO: Add your command handler code here
	// 기본 파일 설정
	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	filename.Format(
		_T("ESL_DM_%04d%02d%02d.tunning"),
		t.GetYear(),
		t.GetMonth(),
		t.GetDay()
		);
	
	CFileDialog FileDlg(
		TRUE,
		_T("tunning"),
		filename,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("튜닝 파일(*.tunning)|*.tunning|모든 파일(*.*)|*.*||")
		);

//DEL 	// 기본 경로 설정
//DEL 	// 현재 작업영역에 *.tunning파일이 있으면 현재 영역, 
//DEL 	// 없으면 C:\Program Files\KOLAS III\tunning 폴더 열기	
//DEL 	CString strTunningFile = _T("");
//DEL 	strTunningFile.Format(_T("%s\\*.tunning"), szPath);
//DEL 
//DEL 	CFileFind FindFile;
//DEL 	if(TRUE == FindFile.FindFile(strTunningFile))
//DEL 	{
//DEL 		FileDlg.m_ofn.lpstrInitialDir = szPath;		
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		FileDlg.m_ofn.lpstrInitialDir = _T("C:\\Program Files\\KOLAS III\\tunning\\");
//DEL 	}

	if(IDOK == FileDlg.DoModal())
	{
		// 가장 마지막에 저장해 두었던 작업경로로 다시금 세팅한다. 
		SetCurrentDirectory(szPath);				
		View_TunningFile(FileDlg.GetPathName());		
	}

	return ;
}

void CTunningLogViewerDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	// 개수를 파악한다.
	const int DRAG_FILE_COUNT = DragQueryFile(
		hDropInfo,
		-1,// -1: 갯수를 꺼내라 
		0,
		0
		);
	
	TCHAR szName[256] = { 0 };
	DragQueryFile(hDropInfo, DRAG_FILE_COUNT-1, szName, 256);

	CString strFile = _T("");	
	strFile.Format(_T("%s"), szName);
	View_TunningFile(strFile);
	
	CDialog::OnDropFiles(hDropInfo);
}

void CTunningLogViewerDlg::OnSaveTunningFileForText() 
{
	// TODO: Add your command handler code here

	// 기본 파일이름 설정
	// 현재 읽어온 파일이름.확장자를 파일이름.txt로 설정
	CString strFile = m_strFile.Mid(
		m_strFile.ReverseFind('\\')+1,
		m_strFile.GetLength() - m_strFile.ReverseFind('\\')+1
		);

	if(0 < strFile.Find(_T(".")))
	{
	strFile.Replace(
		strFile.Mid(
			strFile.ReverseFind('.'),
			m_strFile.GetLength() - m_strFile.ReverseFind('\\')+1
			),
		_T(".txt")
		);
	}
	else
	{
		strFile += _T(".txt");
	}

	
	//현재의 작업경로를 얻어와 저장 한다. 
	TCHAR szPath[MAX_PATH] = {0}; 
	GetCurrentDirectory(MAX_PATH, szPath);
  
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	filename.Format(
		strFile,
		t.GetYear(),
		t.GetMonth(),
		t.GetDay()
		);
	
	CFileDialog FileDlg(
		FALSE,
		_T("txt"),
		filename,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("텍스트 파일(*.txt)|*.txt|모든 파일(*.*)|*.*||")
		);

	if(IDOK == FileDlg.DoModal())
	{
		// 가장 마지막에 저장해 두었던 작업경로로 다시금 세팅한다. 
		SetCurrentDirectory(szPath);
		if(0 == Save_TunningFile(FileDlg.GetPathName(), m_szText, m_dwTextSize))
		{
		}
		else
		{
		}
	}

	return ;
}

INT CTunningLogViewerDlg::Save_TunningFile(CString strFile, TCHAR* szText,  DWORD dwTextSize)
{
	CStdioFile file2;
	CFileException ex;
	BOOL bIsOpen = file2.Open(strFile, CFile::modeWrite|CFile::modeCreate|CFile::typeBinary, &ex);
	if(FALSE == bIsOpen)
	{
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		AfxMessageBox(szMsg);
		return -1001;
	}
	// 유니코드 텍스트파일임을 표기 
	TCHAR szBom = (TCHAR)0xFEFF;
	file2.Write(&szBom, sizeof(TCHAR));

	// 텍스트 저장
	file2.WriteHuge(szText, dwTextSize*sizeof(TCHAR));
	file2.Close();

	return 0;
}



// 함수이름: CTunningLogViewerDlg::View_TunningFile
// 함수설명: 인자로 받은 파일을 읽어들여 에디트에 뿌려준다.
// 반환  형: int 
// 인자[CString strFileName] : 
int CTunningLogViewerDlg::View_TunningFile(CString strFileName)
{
	// 파일읽기가 실패했을 때를 대비한 백업
	// 파일읽기가 성공하면 m_szText에 메모리가 할당되기 때문에
	// 기존의 할당되어있는 메모리주소를 잃어버리지 않기위해 szBakup가 가지고 있도록 한다.
	TCHAR* szBakup = m_szText;
	int ids = Read_TunningFile(strFileName, &m_szText, &m_dwTextSize);
	if(0 == ids)
	{			
		// 에디트 설정
		mCtrl_edtView.SetWindowText(m_szText);		
		
		// 캡션 설정
		CString strCaption = _T("");
		strCaption.Format(_T("튜닝 로그 뷰어 [%s]"), strFileName);
		SetWindowText(strCaption);		
		
		// 파일이름 설정
		m_strFile = strFileName;

		// 파일수정시간 얻기
		Get_ModifyTime(strFileName, m_LastModifyTime);

		delete []szBakup;
		szBakup = NULL;
	}
	else
	{
		m_szText = szBakup;
	}

	mCtrl_edtView.SetSel(-2,-1);
	UpdateWindow();	

	return ids;
}

BOOL CTunningLogViewerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{		
		BOOL b_Ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
		BOOL b_Shift = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;
		BOOL b_Alt   = (GetKeyState(VK_MENU)    & 0x8000) > 0;

		switch(pMsg->wParam)
		{
		case VK_RETURN:
		{
			return TRUE;
			break;
		}
		case VK_ESCAPE:
		{			
			return TRUE;
			break;
		}
		case VK_F1:
		{
			OnHelp();
			return TRUE;
			break;
		}
		case VK_F3:
		{
			if(_T("") != m_strSearchText)
			{		
				INT nResult = 0;
				INT nStart = 0, nEnd = 0;
				int nSearchDirection = (TRUE == b_Shift) ? !m_nSearchDirection : m_nSearchDirection;				
				if(CSearchDlg::SEARCH_TYPE_NEXT_SEARCH == nSearchDirection)
				{			
					nResult = Search_NextText(m_strSearchText, nStart, nEnd, m_bIsLowerUpper);
				}				
				else 
				{
					nResult = Search_PrevText(m_strSearchText, nStart, nEnd, m_bIsLowerUpper);
				}

				if(0 > nResult)
				{
					CString strMsg;
					strMsg.Format(_T("\"%s\"을(를) 찾을 수 없습니다."), m_strSearchText);
					AfxMessageBox(strMsg);
				}
				else
				{
	 				mCtrl_edtView.SetFocus();
	 				mCtrl_edtView.SetSel(nStart, nEnd);
				}
				
			}			
			return TRUE;
			break;
		}
		case VK_F4:
		{
// 			SetRedraw(FALSE);

			INT nOldStart = 0, nOldEnd = 0;
			INT nResult1 = 0, nResult2 = 0;
			INT nStart1 = 0, nStart2 = 0, nEnd1 = 0, nEnd2 = 0;
			
			nResult1 = Search_PrevText(_T("\n"), nStart1, nEnd1);
			nResult2 = Search_NextText(_T("\n"), nStart2, nEnd2);

			if(0 <= nResult1 && 0 <= nResult2)
			{
				mCtrl_edtView.GetSel(nOldStart, nOldEnd);
// 				mCtrl_edtView.SetSel(nStart1+1, nStart1);

				const INT SEARCH_CNT = 4;
				CString strSearchText[SEARCH_CNT] = 
				{
					_T("SELECT "), _T("UPDATE "), _T("DELETE "), _T("EXEC ")
				};

				INT nCnt = 0;
				for(nCnt = 0; nCnt < SEARCH_CNT; nCnt++)
				{
					nResult1 = Search_NextText(strSearchText[nCnt], nStart1, nEnd1);
					if(0 <= nResult1)
					{
						break;
					}
				}
				if(0 <= nResult1)
				{
	 				mCtrl_edtView.SetFocus();

					nStart1 = nEnd1-strSearchText[nCnt].GetLength();
					nEnd2 = nEnd2;

					if(nEnd2 > nStart1)
					{
	 	 				mCtrl_edtView.SetSel(nStart1, nEnd2);
					}
					else
					{						
						mCtrl_edtView.SetSel(nOldStart, nOldEnd);
					}
				}
			}
// 			SetRedraw(TRUE);
			UpdateWindow();

			break;
		}
		case VK_F5:
		{
			if(TRUE == mCtrl_staMSG.IsWindowVisible())
			{
				mCtrl_staMSG.ShowWindow(SW_HIDE);
				ResizeControl();
			}				
			View_TunningFile(m_strFile);
			return TRUE;
			break;
		}
		case 'A':
		{
			// Ctrl + A 키 눌렀을 때
//DEL 			BOOL b_Ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
//DEL 			BOOL b_Shift = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;
//DEL 			BOOL b_Alt   = (GetKeyState(VK_MENU)    & 0x8000) > 0;

			if(TRUE == b_Ctrl && FALSE == b_Shift && FALSE == b_Alt)
			{
 				mCtrl_edtView.SetFocus();
				mCtrl_edtView.SetSel(0, -1);
				return TRUE;
			}
			break;
		}
		case 'F':
		{
			// Ctrl + F 키 눌렀을 때
//DEL 			BOOL b_Ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
//DEL 			BOOL b_Shift = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;
//DEL 			BOOL b_Alt   = (GetKeyState(VK_MENU)    & 0x8000) > 0;

			if(TRUE == b_Ctrl && FALSE == b_Shift && FALSE == b_Alt)
			{
 				OnFind();	
				return TRUE;
			}			
			break;
		}
		case 'O':
		{
			// Ctrl + O 키 눌렀을 때
//DEL 			BOOL b_Ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
//DEL 			BOOL b_Shift = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;
//DEL 			BOOL b_Alt   = (GetKeyState(VK_MENU)    & 0x8000) > 0;

			if(TRUE == b_Ctrl && FALSE == b_Shift && FALSE == b_Alt)
			{
				OnReadTunningFile();
				return TRUE;
			}
			break;
		}
		case 'S':
		{
			// Ctrl + S 키 눌렀을 때
//DEL 			BOOL b_Ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
//DEL 			BOOL b_Shift = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;
//DEL 			BOOL b_Alt   = (GetKeyState(VK_MENU)    & 0x8000) > 0;

			if(TRUE == b_Ctrl && FALSE == b_Shift && FALSE == b_Alt)
			{
				OnSaveTunningFileForText();
				return TRUE;
			}
			break;
		}
		default:
			break;
		};
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CTunningLogViewerDlg::Search_NextText(CString strText, INT &nStart, INT &nEnd, BOOL bIsSplitLowUpper)
{
	if(_T("") == strText) return 0;

	if(0 == nStart)
	{
		nStart=0, nEnd=0;
		mCtrl_edtView.GetSel(nStart, nEnd);
	}

	const int SEARCH_TEXT_LENGTH = strText.GetLength();
	TCHAR* szSearchText = new TCHAR[SEARCH_TEXT_LENGTH+1];
	memset(szSearchText, NULL, SEARCH_TEXT_LENGTH);
	szSearchText[SEARCH_TEXT_LENGTH] = '\0';
	_tcscpy(szSearchText, strText.GetBuffer(0));
	
	// 대/소문자 구분을 안하는 경우 검색어를 대문자로 변환한다.
	if(FALSE == bIsSplitLowUpper)
	{
		CharUpperBuff(szSearchText, SEARCH_TEXT_LENGTH);
	}


	BOOL bIsFind = FALSE;

	const int END = ((int)m_dwTextSize - SEARCH_TEXT_LENGTH);
	for(int nStartPos = nStart; nStartPos < END; nStartPos++)
	{			
		BOOL bIsSame = TRUE;
		// 대소문자 구분을 안하는 경우 검색대상을
		// 대문자로 바꾼후에 비교검색한다.
		if(FALSE == bIsSplitLowUpper)
		{
			for(int nPos = 0; nPos < SEARCH_TEXT_LENGTH; nPos++)
			{
				TCHAR szTmp = m_szText[nStartPos + nPos];				
				CharUpperBuff(&szTmp, 1);
				if(szSearchText[nPos] != szTmp)
				{
					bIsSame = FALSE;
					break;
				}
			}
		}
		else
		{
			for(int nPos = 0; nPos < SEARCH_TEXT_LENGTH; nPos++)
			{
				if(szSearchText[nPos] != m_szText[nStartPos + nPos])
				{
					bIsSame = FALSE;
					break;
				}
			}
		}

		if(TRUE == bIsSame)
		{
			if(nStart == nStartPos && nEnd == nStartPos+SEARCH_TEXT_LENGTH)
			{
				continue;
			}
			else
			{
// 				mCtrl_edtView.SetFocus();
				nStart = nStartPos;
				nEnd = nStartPos+SEARCH_TEXT_LENGTH;
// 				mCtrl_edtView.SetSel(nStartPos, nStartPos+SEARCH_TEXT_LENGTH);
				bIsFind = TRUE;
			}
			break;
		}
	}
 	if(FALSE == bIsFind)
 	{ 		
// 		strMsg.Format(_T("\"%s\"을(를) 찾을 수 없습니다."), strText);
 		return -1000;
 	}

	delete [] szSearchText;
	return 0;
}

int CTunningLogViewerDlg::Search_PrevText(CString strText, INT &nStart, INT &nEnd, BOOL bIsSplitLowUpper)
{
	if(_T("") == strText) return 0;

	if(0 == nStart)
	{
		nStart=0, nEnd=0;
		mCtrl_edtView.GetSel(nStart, nEnd);
	}

	const int SEARCH_TEXT_LENGTH = strText.GetLength();
	TCHAR* szSearchText = new TCHAR[SEARCH_TEXT_LENGTH+1];
	memset(szSearchText, NULL, SEARCH_TEXT_LENGTH);
	szSearchText[SEARCH_TEXT_LENGTH] = '\0';
	_tcscpy(szSearchText, strText.GetBuffer(0));

	// 대/소문자 구분을 안하는 경우 검색어를 대문자로 변환한다.
	if(FALSE == bIsSplitLowUpper)
	{
		CharUpperBuff(szSearchText, SEARCH_TEXT_LENGTH);
	}

	BOOL bIsFind = FALSE;
	for(int nStartPos = nEnd-SEARCH_TEXT_LENGTH; nStartPos >= 0; nStartPos--)
	{
		BOOL bIsSame = TRUE;
		
		// 대소문자 구분을 안하는 경우 검색대상을
		// 대문자로 바꾼후에 비교검색한다.
		if(FALSE == bIsSplitLowUpper)
		{
			for(int nPos = 0; nPos < SEARCH_TEXT_LENGTH; nPos++)
			{
				TCHAR szTmp = m_szText[nStartPos + nPos];				
				CharUpperBuff(&szTmp, 1);
				if(szSearchText[nPos] != szTmp)
				{
					bIsSame = FALSE;
					break;
				}
			}
		}
		else
		{
			for(int nPos = 0; nPos < SEARCH_TEXT_LENGTH; nPos++)
			{
				if(szSearchText[nPos] != m_szText[nStartPos + nPos])
				{
					bIsSame = FALSE;
					break;
				}
			}
		}

		if(TRUE == bIsSame)
		{			
			// 찾은 위치가 현재 커서의 위치와 동일하면 다음 것 찾기
			if(nStart == nStartPos && nEnd == nStartPos+SEARCH_TEXT_LENGTH)
			{
				continue;
			}
			else
			{
// 				mCtrl_edtView.SetFocus();
				nStart = nStartPos;
				nEnd = nStartPos+SEARCH_TEXT_LENGTH;
// 				mCtrl_edtView.SetSel(nStartPos, nStartPos+SEARCH_TEXT_LENGTH);
				bIsFind = TRUE;
			}
			break;
		}
	}
	if(FALSE == bIsFind)
	{
// 		CString strMsg = _T("");
// 		strMsg.Format(_T("\"%s\"을(를) 찾을 수 없습니다."), strText);
// 		AfxMessageBox(strMsg, MB_ICONINFORMATION);
		return -1000;
	}

	delete [] szSearchText;
	return 0;

}

void CTunningLogViewerDlg::OnFind() 
{
	// TODO: Add your command handler code here
	CSearchDlg SearchDlg;
	SearchDlg.m_bIsLowerUpper = m_bIsLowerUpper;	
	SearchDlg.m_nSearchDirection = m_nSearchDirection;
	SearchDlg.m_strSearchText = m_strSearchText;

	if(IDOK == SearchDlg.DoModal())
	{
		INT S = 0, E = 0;
		mCtrl_edtView.GetSel(S, E);

		m_strSearchText = SearchDlg.m_strSearchText;
		m_bIsLowerUpper = SearchDlg.m_bIsLowerUpper;
		
		INT nResult;
		INT nStart = 0, nEnd = 0;
		if(CSearchDlg::SEARCH_TYPE_NEXT_SEARCH == SearchDlg.m_nSearchDirection)
		{
			nResult = Search_NextText(m_strSearchText, nStart, nEnd, m_bIsLowerUpper);
		}
		else 
		{
			nResult = Search_PrevText(m_strSearchText, nStart, nEnd, m_bIsLowerUpper);
		}
		if(0 > nResult)
		{
			CString strMsg;
			strMsg.Format(_T("\"%s\"을(를) 찾을 수 없습니다."), m_strSearchText);
			AfxMessageBox(strMsg);
		}
		else
		{
			mCtrl_edtView.SetFocus();
			mCtrl_edtView.SetSel(nStart, nEnd);
		}
		
		m_nSearchDirection = SearchDlg.m_nSearchDirection;
	}		
}

void CTunningLogViewerDlg::OnHelp() 
{
	// TODO: Add your command handler code here
	CAboutDlg Dlg;
	Dlg.DoModal();
}


// 함수이름: CTunningLogViewerDlg::Get_ModifyTime
// 함수설명: 파일의 수정시간을 얻는다.
// 반환  형: int 
// 인자[CString strFile] : 수정시간을 얻어올 파일이름
// 인자[SYSTEMTIME& time] : 
int CTunningLogViewerDlg::Get_ModifyTime(CString strFile, SYSTEMTIME& time)
{
	
	FILETIME ftLocal;
//DEL 	SYSTEMTIME stCreate;
	
	// get the file attributes
	_WIN32_FILE_ATTRIBUTE_DATA fileAttData;
	if(TRUE == GetFileAttributesEx(strFile,GetFileExInfoStandard,(void*)&fileAttData))
	{
		
		// Convert the last-write time to local time.
		if (!FileTimeToLocalFileTime(&fileAttData.ftLastWriteTime, &ftLocal))
			return -1;
		
		// Convert the local file time from UTC to system time.
		if(TRUE != FileTimeToSystemTime(&ftLocal, &time)) return -2;
	}
	else return -3;


	return 0;
}

//DEL void CTunningLogViewerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
//DEL {
//DEL 	CDialog::OnActivate(nState, pWndOther, bMinimized);
//DEL 	
//DEL 	// TODO: Add your message handler code here
//DEL 	if(_T("") != m_strFile)
//DEL 	{
//DEL 		SYSTEMTIME time;
//DEL 		Get_ModifyTime(m_strFile, time);
//DEL 		
//DEL 		CTime Time_1(m_LastModifyTime);
//DEL 		CTime Time_2(time);
//DEL 		
//DEL 		if(Time_1 != Time_2)
//DEL 		{
//DEL 			View_TunningFile(m_strFile);
//DEL 		}
//DEL 	}
//DEL }

BOOL CTunningLogViewerDlg::OnNcActivate(BOOL bActive) 
{

	// TODO: Add your message handler code here and/or call default
	if(_T("") != m_strFile)
	{
		SYSTEMTIME time;
		if(0 == Get_ModifyTime(m_strFile, time))
		{			
			CTime Time_1(m_LastModifyTime);
			CTime Time_2(time);
			
			if(Time_1 != Time_2)
			{
				CString strFile = m_strFile.Mid(
					m_strFile.ReverseFind('\\')+1,
					m_strFile.GetLength() - m_strFile.ReverseFind('\\')+1
					);

				CString strMsg = _T("");
				strMsg.Format(
					_T("[%s]파일이 변경 되었습니다. F5키를 누르면 파일을 다시 불러올 수 있습니다."),
					strFile
					);
				mCtrl_staMSG.SetWindowText(strMsg);
				mCtrl_staMSG.ShowWindow(SW_SHOW);
				ResizeControl();
			}
		}
	}

// 	mCtrl_edtView.SetSel(-2,-1);
//	INT nCnt = mCtrl_edtView.GetLineCount();
//  	mCtrl_edtView.LineScroll(nCnt);

	return CDialog::OnNcActivate(bActive);
}

void CTunningLogViewerDlg::ResizeControl()
{
	CRect rDlg;
	GetClientRect(rDlg);

	if(NULL != mCtrl_edtView.GetSafeHwnd())
	{			
		CWnd* pWnd_1 = GetDlgItem(IDC_STATIC_1);
		CWnd* pWnd_2 = GetDlgItem(IDC_STATIC_2);
		
		CRect r_1, r_2, r_22, rEdt;
		pWnd_1->GetWindowRect(r_1);
		pWnd_2->GetWindowRect(r_2);
		
		pWnd_1->SetWindowPos(
			NULL, 
			0, 
			rDlg.bottom - r_1.Height(), 
			0, 
			0, 
			SWP_NOSIZE|SWP_SHOWWINDOW
			);

		pWnd_2->SetWindowPos(
			NULL, 
			rDlg.right-r_2.Width(), 
			rDlg.bottom - r_2.Height(), 
			0, 
			0, 
			SWP_NOSIZE|SWP_SHOWWINDOW
			);

		pWnd_2->GetWindowRect(r_2);
		r_22 = r_2;
		ScreenToClient(r_22);

		CRect rStatic = CRect(0, 0, 0, -5);
		if(NULL != mCtrl_staMSG.GetSafeHwnd())
		{
			if(TRUE == mCtrl_staMSG.IsWindowVisible())
			{
				mCtrl_staMSG.GetWindowRect(rStatic);
				ScreenToClient(rStatic);

				mCtrl_staMSG.MoveWindow(rStatic.left, rStatic.top, rDlg.right, rStatic.Height());				
			}
		}

		rEdt = CRect(0, rStatic.bottom + 5, rDlg.right, r_22.top-1);		
		mCtrl_edtView.MoveWindow(rEdt);


	}
}

HBRUSH CTunningLogViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(CTLCOLOR_STATIC == nCtlColor || CTLCOLOR_DLG == nCtlColor)
	{
		SetBkColor(pDC->m_hDC,RGB(255,255,255));
		return m_edtBrush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CTunningLogViewerDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnEraseBkgnd(pDC);
}


void CTunningLogViewerDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	// TODO: Add your message handler code here
	if(550 > lpwndpos->cx)
	{
		lpwndpos->cx = 550;
	}
	if(150 > lpwndpos->cy)
	{
		lpwndpos->cy = 150;
	}
	CDialog::OnWindowPosChanging(lpwndpos);
}

void CTunningLogViewerDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	// TODO: Add your message handler code here
	if(550 > lpwndpos->cx)
	{
		lpwndpos->cx = 550;
	}	
	if(150 > lpwndpos->cy)
	{
		lpwndpos->cy = 150;
	}
	CDialog::OnWindowPosChanged(lpwndpos);
}
