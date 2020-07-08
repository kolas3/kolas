// Select_Pic_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Select_Pic_Dlg.h"
#include "Capture_Cam_Dlg.h"
#include "Editor_Setup_Dlg.h"

#define	IMAGEFROMFILE 0
#define IMAGEFROMCAMERA 1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int TIMER_WAIT	= 200;

/////////////////////////////////////////////////////////////////////////////
// CSelect_Pic_Dlg dialog

CSelect_Pic_Dlg::CSelect_Pic_Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CSelect_Pic_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelect_Pic_Dlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

void CSelect_Pic_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelect_Pic_Dlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelect_Pic_Dlg, CDialog)
//{{AFX_MSG_MAP(CSelect_Pic_Dlg)
ON_BN_CLICKED(IDC_BTN_CAMERA, OnBtnCamera)
ON_BN_CLICKED(IDC_BTN_IMAGE_BROWSE, OnBtnImageBrowse)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BTN_IMAGE_EDIT, OnBtnImageEdit)
ON_BN_CLICKED(IDC_BTN_EDITOR_SELECT, OnBtnEditorSelect)
ON_WM_DESTROY()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BTN_PIC_CONFIRM, OnBtnPicConfirm)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelect_Pic_Dlg message handlers

void CSelect_Pic_Dlg::OnBtnCamera() 
{
	// TODO: Add your control notification handler code here
	CCapture_Cam_Dlg capture_Cam_Dlg;
	
	if(IDOK == capture_Cam_Dlg.DoModal())
	{
	}
}

void CSelect_Pic_Dlg::OnBtnImageBrowse() 
{
	// TODO: Add your control notification handler code here
	
	static TCHAR BASED_CODE szFilter[] = _T("Image (*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.JPEG | All Files(*.*)|*.*||");
	
    TCHAR path[PATHSIZE] = {0};
    GetCurrentDirectory(PATHSIZE, path);    //  lpBuffer = ""C:\program\test"
	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
		this->SetFileName(dlg.GetPathName());
		this->isImageSelect = TRUE;
		this->imageFrom = IMAGEFROMFILE;
	}	
	SetCurrentDirectory(path);
	
	previewLastWriteTime = GetFileLastWriteTime(this->GetFileName());
	
	this->InvalidateRect(NULL, TRUE);
	
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(((LPCTSTR)(this->GetFileName())));
	
}

void CSelect_Pic_Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	if(this->isImageSelect)
	{
	
		if(imageFrom == IMAGEFROMFILE)
		{
			CRect rect;
			
			GetClientRect(&rect);
			
			int iWidth = rect.Width();
			int iHeight = rect.Height();
			
			m_Img.Load(this->GetFileName());
			
			//CRect imageRect(iWidth/2-71, (iHeight/2-71)+70, iWidth/2+71, (iHeight/2+71)+70);
			CRect imageRect(iWidth/2-54, (iHeight/2-72)+70, iWidth/2+54, (iHeight/2+72)+70);

			m_Img.Show(&dc, imageRect);}
		
		else if(imageFrom == IMAGEFROMCAMERA)
		{
			
			CRect rect;
			
			GetClientRect(&rect);
			
			int iWidth = rect.Width();
			int iHeight = rect.Height();
			
			m_Img.Load(this->GetFileName());
			
			CRect imageRect(iWidth/2-54, (iHeight/2-72)+70, iWidth/2+54, (iHeight/2+72)+70);
			
			m_Img.Show(&dc, imageRect);
		}
	}

// Do not call CDialog::OnPaint() for painting messages

}

void CSelect_Pic_Dlg::DrawCaptureImage()
{
	// 클라이언트 영역 구하기
	CClientDC cDc(this);
	CRect rect;
	
	GetClientRect(&rect);
	
	// Cam 으로부터 넘겨받은 파일경료를 화면에 Load 시킴
	int iWidth = rect.Width();
	int iHeight = rect.Height();
	
	m_Img.Load(this->GetFileName());
	
	CRect imageRect(iWidth/2-71, (iHeight/2-71)+70, iWidth/2+71, (iHeight/2+71)+70);
	
	m_Img.Show(&cDc, imageRect);
	
	this->InvalidateRect(NULL,TRUE);
}

void CSelect_Pic_Dlg::OnBtnImageEdit() 
{
	// TODO: Add your control notification handler code here
	
	TCHAR path[PATHSIZE] = {0};
    GetCurrentDirectory(PATHSIZE, path);
	
	//응용 프로그램 실행파일 가져오기
	TCHAR exeFilePath[256] = {0};
	
	CString dataName;
	
	dataName.Format(EDITORSETUPFILE);
	
	CString fullPath;
	
	fullPath.Format(_T("%s\\%s"),path,dataName);
	
	
	FILE* file = _tfopen(fullPath,_T("rb"));
	// KOL.UDF.022 시큐어코딩 보완
	//if(file != NULL)
	if(&file != NULL)
	{
		fread(exeFilePath,256,1,file);
	}
	
	fclose(file);
		
	CString tempImageName;
	
	tempImageName.Format(_T("\"%s\""), this->GetFileName());
	
	
	CString cmdLine;
	cmdLine.Format(_T("%s %s"),exeFilePath,tempImageName);
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	//구조체를 초기화한다
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	
	si.cb=sizeof( STARTUPINFO);
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_SHOWNORMAL;
	
	if(isImageSelect)
	{
		SetTimer(TIMER_WAIT,2000,NULL);
		
		GetDlgItem(IDC_BTN_PIC_CONFIRM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_IMAGE_BROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_IMAGE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CAMERA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_EDITOR_SELECT)->EnableWindow(FALSE);
		
		
		CreateProcess ( NULL, (LPTSTR)(LPCTSTR)cmdLine , NULL , NULL , FALSE, 0 , NULL , NULL , &si, &pi);
		
		// OnTimer에서 ProcessHandle 여부 검사
		processHandle = pi.hProcess;
		
	}
	
	
	else
	{		
		CreateProcess ( NULL, exeFilePath , NULL , NULL , FALSE, 0 , NULL , NULL , &si, &pi);
	}
	
}

void CSelect_Pic_Dlg::OnBtnEditorSelect() 
{
	// TODO: Add your control notification handler code here
	CEditor_Setup_Dlg editor_Setup_Dlg;
	
	if(IDOK == editor_Setup_Dlg.DoModal())
	{
	}
}

void CSelect_Pic_Dlg::SetFileName(CString fileName)
{
	this->fileName = fileName;
}

CString CSelect_Pic_Dlg::GetFileName()
{
	return this->fileName;
}

FILETIME CSelect_Pic_Dlg::GetFileLastWriteTime(CString filePath)
{
	
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	
	hSrch = FindFirstFile(filePath,&wfd);
	
	FILETIME b=wfd.ftLastWriteTime;
	
	return b;
	
}

void CSelect_Pic_Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CSelect_Pic_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->isImageSelect = FALSE;
	this->SetFileName(_T(""));
	this->imageFrom = -1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelect_Pic_Dlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_WAIT)
	{
		if(processHandle)
		{
			DWORD dwExitCode = STILL_ACTIVE;
			GetExitCodeProcess(processHandle,&dwExitCode);
			if( dwExitCode != STILL_ACTIVE )
			{
				KillTimer(TIMER_WAIT);
				GetDlgItem(IDC_BTN_PIC_CONFIRM)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_IMAGE_BROWSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_IMAGE_EDIT)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_CAMERA)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_EDITOR_SELECT)->EnableWindow(TRUE);
				
				HANDLE hSrch;
				WIN32_FIND_DATA wfd;
				
				hSrch = FindFirstFile(GetFileName(),&wfd);
				
				FILETIME afterImageLastWriteTime=wfd.ftLastWriteTime;
				
				if(previewLastWriteTime.dwLowDateTime!=afterImageLastWriteTime.dwLowDateTime)
				{
					if(isImageSelect){
						DrawCaptureImage();
						previewLastWriteTime = GetFileLastWriteTime(GetFileName());}		
				}	
			}
		}			
		CDialog::OnTimer(nIDEvent);
	}
}

void CSelect_Pic_Dlg::OnBtnPicConfirm() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
