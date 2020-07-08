// InsertLicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InsertLicense.h"
#include "InsertLicenseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertLicenseDlg dialog

CInsertLicenseDlg::CInsertLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertLicenseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertLicenseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	totalFileCnt = 0;
	totalHeaderFileCnt = 0;
	
	totDspCnt = 0;
}

CInsertLicenseDlg::~CInsertLicenseDlg()
{
	
}

void CInsertLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertLicenseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsertLicenseDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertLicenseDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btn_find_directory, Onbtnfinddirectory)
	ON_BN_CLICKED(IDC_btn_Find_License, OnbtnFindLicense)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertLicenseDlg message handlers

BOOL CInsertLicenseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	

	//pGrid = new CESL_Grid(NULL);
	//pGrid->SubclassDlgItem(IDC_gridCHECK_LIST, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//폴더탐색
void CInsertLicenseDlg::DetectDirectory( CString strPath ){
	
	CFileFind finder;
	
	BOOL bWorking = finder.FindFile( strPath + _T("/*.*"));

	while(bWorking){
		bWorking = finder.FindNextFile();
		
		totalFileCnt++;

		if(finder.IsDots()) continue;

		strFillName = finder.GetFileName();
		strFillName.Replace(finder.GetFileTitle() , _T(""));
		strFillName.MakeUpper();
		if( strFillName == _T(".H") ) {
			totalHeaderFileCnt++;
			InsertLicense( &finder );
		}
		/*
		else if (strFillName == _T(".DSP"))
		{
			totDspCnt++;
		}
		*/
		if( finder.IsDirectory() ){
			DetectDirectory( finder.GetFilePath());
		}
	}
}

//License 삽입
/*	return value desc
	0	: 성공
	10	: 예외대상
*/
int CInsertLicenseDlg::InsertLicense( CFileFind* finder ){

	CString strFileName = finder->GetFileName();
	strFileName.MakeUpper();
	//StdAfx.h //Resource.h //webbrowser2.h 
	if( (strFileName == _T("WEBBROWSER2.h ")) ||(strFileName == _T("STDAFX.H")) || (strFileName == _T("RESOURCE.H")) || strFileName.Find(_T("(TMP)")) > -1 )
	{
		return 10;
	}	
	
	strFileName = finder->GetFileName();
	CString strFilePath = finder->GetFilePath();
	
	//임시파일을 생성한다.
	CString strTempFile = finder->GetFilePath();
	strTempFile.Replace( finder->GetFileName(), _T("") );
	strTempFile += _T("(TMP)") + finder->GetFileName();

	//원본파일의 인코딩 복사
	CFile file;
	file.Open( strFilePath, CFile::modeRead | CFile::typeBinary );
	TCHAR cUni;
	file.Read(&cUni,sizeof(TCHAR));
	file.Close();

	CStdioFile fSrcFie,fTempFile;

	//임시파일에 인코딩을 복사한 입력
	CString strLine;
	fSrcFie.Open( strFilePath, CFile::modeRead );
	fTempFile.Open( strTempFile , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary);
	fTempFile.Write(&cUni,sizeof(TCHAR));
	fTempFile.Close();
	//임시파일에 License 를 쓴다.
	fTempFile.Open( strTempFile , CFile::modeWrite );
	fTempFile.WriteString(strLicense);
	//원본에서 임시파일로 라이센스 내용 복사 
	while(fSrcFie.ReadString(strLine)){
		fTempFile.WriteString(strLine + _T("\n"));
	}
	
	fSrcFie.Close();
	fTempFile.Close();
	//원본파일을 지우고 임시파일을 원본파일로 대체한다.
	CFile::Remove(strFilePath);
	CFile::Rename(strTempFile,strFilePath);

	/*
	char* cbuff = new char[nLength];
	fSrcFie.Read( cbuff, nLength);
	fSrcFie.Close();
	*/
//	CString strMsg = _T("");
//	strMsg.Format(_T("%s\n%s"),cLicense,cbuff);
	//strMsg.Replace(_T("\n"),_T("\r\n"));

//	fTempFile.Open( strTempFile , CFile::modeWrite | CFile::modeCreate);

//	fTempFile.WriteString( strMsg );



	/*
	CFile file;
	file.Open( strTargetPath, CFile::modeRead );
	
	CString strEnter = _T("\r\n");

	int nLength = file.GetLength();
	char *buffer = new char[nLength];
	file.Read( buffer, nLength );
	file.Close();
	WORD wd = 0xFEFF;
	file.Open( strTargetPath, CFile::modeWrite | CFile::modeCreate  | CFile::typeBinary );
	file.Write(&wd,2);
	file.SeekToEnd();
	file.Write(lcns.Content, lcns.Length);
	file.SeekToEnd();
	file.Write(buffer,nLength);

	file.Close();
	*/
	return 0;
}


void CInsertLicenseDlg::OnOK() 
{
	// TODO: Add extra validation here
	totalFileCnt = 0;
	totalHeaderFileCnt = 0;
	//CString strFullPath = _T("C:/Develop/KOLASIII/develop/");
	//strFullPath = _T("C:/Develop/KOLASIII_OPENSRC/develop/");

	DetectDirectory(strPath);
	//CDialog::OnOK();

	//검색된 파일수,헤더파일수 표시 
	CString strFileCnt,strHeaderCnt,strDspCnt;
	strFileCnt.Format(_T("%d"),totalFileCnt);
	strHeaderCnt.Format(_T("%d"),totalHeaderFileCnt);
	strDspCnt.Format(_T("%d"),totDspCnt);
	GetDlgItem(IDC_License_File2)->SetWindowText(strDspCnt);
	GetDlgItem(IDC_edt_Total_File_Cnt)->SetWindowText(strFileCnt);
	GetDlgItem(IDC_edt_Header_File_Cnt)->SetWindowText(strHeaderCnt);
	
}

static int CALLBACK BrowseCallBackProc(HWND hwnd, UINT uMsg, LPARAM lparam, LPARAM lpData){
	switch (uMsg){
		
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}
//소스디렉토리 찾기
void CInsertLicenseDlg::Onbtnfinddirectory() 
{
	// TODO: Add your control notification handler code here
	strPath = _T("");

	ITEMIDLIST *pidlBrowse;
	//WCHAR pszPathname[MAX_PATH];
	char pszPathname[MAX_PATH];

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset( &BrInfo, 0x00, sizeof(BrInfo) );

	BrInfo.pszDisplayName  = pszPathname;
	BrInfo.lpszTitle = _T("Select Folder");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;


	BrInfo.lpfn = BrowseCallBackProc;

	CString strInitPath = _T("C:\\Develop\\");
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer(0);
	
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if(pidlBrowse != NULL )
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	
		strPath = pszPathname;
		GetDlgItem(IDC_Folder_Path)->SetWindowText((LPCTSTR)pszPathname);
	}



}

int CInsertLicenseDlg::FileEncodingChekck(CString strLicenseName){
	
	CFile file;
	if(!file.Open( strLicenseName, CFile::modeRead | CFile::typeBinary)) return -1;

	unsigned char temp = 0;
	CString str;

	if( file.Read(&temp,1) > 0 )
	{
		if(temp==0xFF) str = _T("Unicode Little Endian");
		else if(temp==0xFE) str = _T("Unicode Big Endian");
		else if(temp==0xEF) str = _T("UTF-8");
		else str = _T("Normal Text");
	}
	else  
	{
		str = _T("Normal Text");
	}

	//AfxMessageBox(str);
	
	file.Close();

	/* Ver 1
	CFile file;
	file.Open( strLicenseName, CFile::modeRead | CFile::typeBinary);
	TCHAR cUni;
	
	file.Read( &cUni, sizeof(TCHAR) );
	file.Close();
	if( 0xFEFF == cUni )
	{
		AfxMessageBox(_T("Unicode big endian header"));
		//Unicode big endian header
	}
	else if( 0xFFFE == cUni )
	{
		AfxMessageBox(_T("Unicode file header"));
		//Unicode file header
	}
	else if( 0xEFBB == cUni )
	{
		AfxMessageBox(_T("UTF_8 file header"));
		//UTF_8 file header
	}
	else
	{
		AfxMessageBox(_T("ANSI"));
		//ANSI
	}
	*/

	return 0 ;
}

void CInsertLicenseDlg::OnbtnFindLicense() 
{
	// TODO: Add your control notification handler code here
	strLicenseName = _T("");

	TCHAR szFilter[] = _T("*.txt||");

	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	
	if (IDOK == dlg.DoModal())
	{
		strLicenseName = dlg.GetPathName();
	}
	else 
	{
		return;
	}
	
	GetDlgItem(IDC_License_File)->SetWindowText(strLicenseName);
	
	delete dlg;

	FileEncodingChekck(strLicenseName);

	CStdioFile fTempFile;

	fTempFile.Open( strLicenseName , CFile::modeRead);
	strLicense = _T("");
	CString strLine,strmsg;
	int mid = 0, pos = 0;
	while (fTempFile.ReadString(strLine))
	{
		/*
		if(strLine.GetLength()*5 > 2048 ){
			
			mid = strLine.GetLength()/5;
			strLine.Format(_T("%s\n%s\n%s\n%s\n%s"),
				strLine.Mid(0,mid),
				strLine.Mid(mid+1,mid),
				strLine.Mid(mid*2+2,mid),
				strLine.Mid(mid*3+3,mid),
				strLine.Mid(mid*4+4,strLine.GetLength())
				);
			strLicense +=strLine;
			
		}else {
			strLicense +=strLine+_T("\n");
		}*/
		strLicense +=strLine+_T("\n");

	}
	
	fTempFile.Close();

	CEdit* edt = (CEdit*)GetDlgItem(IDC_License_File2);	
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInsertLicenseDlg::OnPaint() 
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
HCURSOR CInsertLicenseDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
