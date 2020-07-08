#include "stdafx.h"
#include "PreviewImage.h"
#include "direct.h"
#include "../../../공통/Kolas2up_ImageEditor\Kolas2up_ImageEditorDlg.h"
#include "..\\..\\..\\..\\ESL\\PicManager\\Editor_Setup_Dlg.h"
#include "..\\..\\..\\..\\ESL\\PicManager\\Capture_Cam_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int TIMER_SELECTED_EDITOR	= 200;
const int TIMER_DEFAULT_EDITOR	= 300;


CPreviewImage::CPreviewImage(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(CPreviewImage::IDD, pParent)
{
	
	m_sFilePath = _T("");
	
	
	m_pView = NULL;
	m_nCloseMode = 0;
	
	m_mapsize = NULL;
	m_viewrect = NULL;
	m_pProcessHandle = NULL;
}


CPreviewImage::~CPreviewImage()
{
	
}

VOID CPreviewImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewImage)
	DDX_Text(pDX, IDC_edtIMG_FILE_PATH, m_sFilePath);
	DDV_MaxChars(pDX, m_sFilePath, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewImage, CDialog)
	//{{AFX_MSG_MAP(CPreviewImage)
	ON_BN_CLICKED(IDC_btnSEARCH_IMG_FILE, OnbtnSEARCHIMGFILE)
	ON_BN_CLICKED(IDC_btnIMAGE_SELECT, OnbtnIMAGESELECT)
	ON_BN_CLICKED(IDC_btnIMAGE_SELECT2, OnbtnIMAGESELECT2)
	ON_BN_CLICKED(IDC_btnIMAGE_SELECT3, OnbtnIMAGESELECT3)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CAMERA2, OnBtnCamera2)
	ON_BN_CLICKED(IDC_BTN_EDITOR_SETUP, OnBtnEditorSetup)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnImageDelete, OnbtnImageDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPreviewImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	UpdateData(FALSE);
	
	PreViewImage();
	
	return TRUE;  
	
}

VOID CPreviewImage::OnbtnSEARCHIMGFILE() 
{
	CStdioFile Dlg;
	
	TCHAR    szCurrentDir[512];
	
	CString sPath;
	_tgetcwd(szCurrentDir, 512);
	
	CFileDialog fd(TRUE, NULL , NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,_T("All Support Files (*.bmp;*.jpg)|*.bmp; *.jpg|BMP File (*.bmp)|*.bmp|JPG File (*.jpg)|*.jpg|") );

	if (fd.DoModal() != IDOK) return ;
	
	_tchdir(szCurrentDir);
	
	m_sFilePath = fd.GetPathName();
	
	UpdateData(FALSE);
	
	PreViewImage();
}

INT CPreviewImage::GetFileName( CString &sFileName )
{
	sFileName = m_sSelectFileName ;
	
	return 0;
}

VOID CPreviewImage::OnbtnIMAGESELECT() 
{
	UpdateData(TRUE);
	
	m_sSelectFileName = m_sFilePath;
	
	m_nCloseMode = 1;
	
	OnCancel();
}

VOID CPreviewImage::OnbtnIMAGESELECT2() 
{
	OnCancel();	
}


INT CPreviewImage::SetFileName( CString sFileName )
{
	m_sFilePath = sFileName;
	
	return 0;
}

VOID CPreviewImage::OnbtnIMAGESELECT3() 
{	
	if( m_sFilePath.IsEmpty() )
	{
		AfxMessageBox( _T("파일을 선택해주십시오") );
		return ;
	}
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	
	si.cb=sizeof( STARTUPINFO);
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_SHOWNORMAL;
	
	TCHAR exeFilePath[256] = {0};

	CString strImageEditor = _T("기본 편집기");

	CFileFind ff;
	if( !ff.FindFile(_T("EditorSetup.dat")) ) {
		FILE *fp = _tfopen( _T("EditorSetup.dat"), _T("w+b") );
		fputc(0xFF, fp); 
		fputc(0xFE, fp);		
		_ftprintf( fp, _T("%s"), strImageEditor );		
		fclose( fp );
	}
	else {
		CStdioFile fp;
		if( !fp.Open( _T("EditorSetup.dat"), CFile::modeRead | CFile::typeBinary ) )
			return;
		
		TCHAR cUni;
		fp.Read( &cUni , sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
		
		fp.ReadString(strImageEditor);
		fp.Close();
	}

	_tcscpy(exeFilePath, strImageEditor.GetBuffer(0));
	CString tempImageName;	
	tempImageName.Format(_T("\"%s\""), m_sFilePath);
	
	CString cmdLine;	
	cmdLine.Format(_T("%s %s"),exeFilePath,tempImageName);
	
	if( _tcscmp( _T("기본 편집기"),exeFilePath ) == 0 )	
	{		
		CString sOutPutFileName = m_sFilePath;
	
		CString sExt;
	
		sExt = m_sFilePath.Right(3);
	
		sOutPutFileName = sOutPutFileName.Mid( 0 , sOutPutFileName.GetLength() - 4 );
		
		sOutPutFileName += _T("_out.") + sExt;
		
		CKolas2up_ImageEditorDlg dlg;
	
		dlg.SetCropSize(300, 350);
	
		if( m_sFilePath.IsEmpty() )	
		{
			dlg.m_strInFileName = _T("");
		}
	
		dlg.m_strInFileName = m_sFilePath;
	
		dlg.m_strOutFileName = sOutPutFileName;
	
		if (dlg.DoModal() == IDOK) 
		{
			m_sFilePath = sOutPutFileName;
			UpdateData(FALSE);
			PreViewImage();
		}					
	}
	
	else	
	{
		if( m_sFilePath.IsEmpty() )	
		{
			CreateProcess ( NULL, exeFilePath , NULL , NULL , FALSE, 0 , NULL , NULL , &si, &pi);
		}
		
		else	
		{
			
			HANDLE hSrch;
			WIN32_FIND_DATA wfd;
				
			hSrch = FindFirstFile(m_sFilePath,&wfd);
				
			m_ftLastWriteTime = wfd.ftLastWriteTime;

			SetTimer(TIMER_SELECTED_EDITOR,1000,NULL);

			CreateProcess ( NULL, (LPTSTR)(LPCTSTR)cmdLine , NULL , NULL , FALSE, 0 , NULL , NULL , &si, &pi);

			
			m_pProcessHandle = pi.hProcess;
		}		
	}
}

INT CPreviewImage::PreViewImage()
{
	INT ids;
	
	delete m_pView;
	m_pView = NULL;
	
	if( m_sFilePath.IsEmpty() )
	{
		OnPaint();
		return 0;
	}
	
	CFile fFile;
	ids = fFile.Open( m_sFilePath , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 )
	{
		OnPaint();
		return 1;
	}
	
	fFile.Close();
	
	CRect WndPos;
	GetDlgItem(IDC_STATIC_PREVEW_IMAGE)->GetWindowRect(WndPos);
	ScreenToClient(WndPos);
	
	m_pView = new CKolas2up_ImageEditorView;
	m_pView->m_pWnd = this;
	m_mapsize = CSize(WndPos.Width() , WndPos.Height());			
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	m_viewrect = WndPos;
	m_pView->CreateEx( WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, m_viewrect, this, 1, NULL );	
	m_pView->ShowWindow(TRUE);
	
	m_pView->LoadBitmap(m_sFilePath);
	
	
	SIZE s = m_pView->GetSize();
	m_mapsize = CSize(s.cx, s.cy);			
	m_pView->SetScrollSizes( MM_TEXT, m_mapsize );
	
	
	return 0;
}

VOID CPreviewImage::OnPaint() 
{
	CPaintDC dc(this); 
	
}

VOID CPreviewImage::OnDestroy() 
{
	delete m_pView;
	m_pView = NULL;
	
	CDialog::OnDestroy();	
}


INT CPreviewImage::GetCloseMode()
{
	return m_nCloseMode;
}


VOID CPreviewImage::OnBtnCamera2()
{
	CCapture_Cam_Dlg CamDlg(this);
	if (CamDlg.DoModal() == IDOK)
	{
		m_sFilePath.Format(CamDlg.GetFileName());
		UpdateData(FALSE);
		PreViewImage();	
	}
}

VOID CPreviewImage::OnBtnEditorSetup() 
{
	
	CEditor_Setup_Dlg EditorDlg(this);
	if (EditorDlg.DoModal() == IDOK)
	{
	}
	
}

VOID CPreviewImage::OnTimer(UINT nIDEvent) 
{
	
	if(nIDEvent == TIMER_SELECTED_EDITOR)
	{
		if(m_pProcessHandle)
		{
			DWORD dwExitCode = STILL_ACTIVE;
			GetExitCodeProcess(m_pProcessHandle,&dwExitCode);
			if( dwExitCode != STILL_ACTIVE )	
			{
				KillTimer(TIMER_SELECTED_EDITOR);
				
				HANDLE hSrch;
				WIN32_FIND_DATA wfd;
				
				hSrch = FindFirstFile(m_sFilePath,&wfd);
				
				FILETIME recentWriteTime = wfd.ftLastWriteTime;
				
				if(m_ftLastWriteTime.dwLowDateTime!=recentWriteTime.dwLowDateTime)
				{
					UpdateData(FALSE);
					PreViewImage();	
				}	
			}
		}			
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CPreviewImage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

VOID CPreviewImage::OnbtnImageDelete() 
{
	UpdateData(TRUE);
	m_sFilePath = _T("");
	GetDlgItem(IDC_edtIMG_FILE_PATH)->SetWindowText(m_sFilePath);
	m_nCloseMode = 1;
	PreViewImage();
}
