// Editor_Setup_Dlg.cpp : implementation file
//

#include "stdafx.h"
//#include "picmanager.h"
#include "Editor_Setup_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CEditor_Setup_Dlg dialog


CEditor_Setup_Dlg::CEditor_Setup_Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CEditor_Setup_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditor_Setup_Dlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditor_Setup_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditor_Setup_Dlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditor_Setup_Dlg, CDialog)
	//{{AFX_MSG_MAP(CEditor_Setup_Dlg)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditor_Setup_Dlg message handlers

void CEditor_Setup_Dlg::OnBtnBrowse() 
{
	// TODO: Add your control notification handler code here
	static TCHAR BASED_CODE szFilter[] = _T("Excutable (*.exe)|*.exe||");
	
	
	TCHAR path[PATHSIZE] = {0};
    GetCurrentDirectory(PATHSIZE, path);    //  lpBuffer = ""C:\program\test"
	
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
		this->SetFileName(dlg.GetPathName());
	}	
	SetCurrentDirectory(path);

	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(((LPCTSTR)(this->fileName)));
}

void CEditor_Setup_Dlg::OnBtnOk()
{
	// TODO: Add your control notification handler code here
	
	FILE* file;
	
	TCHAR path[PATHSIZE] = {0};
    GetCurrentDirectory(PATHSIZE, path);
	
	CString dataName;
	dataName.Format(_T("EditorSetup.dat"));
	
	CString fullPath;
	
	fullPath.Format(_T("%s\\%s"),path,dataName);
	
	file = _tfopen(fullPath,_T("wb"));
	
	CString selectedEditor;
	
	GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(selectedEditor);

	if( selectedEditor.Compare(DEFAULTEDITOR) == 0 )
	{
		// KOL.UDF.022 시큐어코딩 보완
//		if( file != NULL )
		if( &file != NULL )
		{
			fseek(file, 0, SEEK_END);		
			if(0 == ftell(file))
			{
				fputc(0xFF, file);
				fputc(0xFE, file);
			}			
			int i = _ftprintf(file, DEFAULTEDITOR);			
			/*
			CString sFilenameTemp = DEFAULTEDITOR;
			char* cFilenameTemp = (LPCTSTR)(LPTSTR)sFilenameTemp;
			int i = fwrite(cFilenameTemp,sizeof(cFilenameTemp),1,file);*/
			/*
			int i = fwrite(
				DEFAULTEDITOR	// fileName에 DEFAULTEDITOR 이 기록되어 있다.
			,256,1,file);
			*/
		}
		fclose(file);
		
	}
	else{
		// KOL.UDF.022
//		if( file != NULL )
		if( &file != NULL )
		{
			fseek(file, 0, SEEK_END);		
			if(0 == ftell(file))
			{
				fputc(0xFF, file);
				fputc(0xFE, file);
			}			
			int i = _ftprintf(file, selectedEditor);
			/*
			int i = fwrite(
				selectedEditor // 사용자가 선택한 파일 경로가 저장되어 있다.
				,256,1,file);*/
		}
		fclose(file);
	}
	this->OnOK();
}

void CEditor_Setup_Dlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	this->OnOK();
}

void CEditor_Setup_Dlg::OnBtnReset() 
{
	// TODO: Add your control notification handler code here
	this->SetFileName(DEFAULTEDITOR);
	
	FILE* file;
	
	file = _tfopen(EDITORSETUPFILE,_T("wb"));
	// KOL.UDF.022
//	if( file != NULL )
	if( &file != NULL )
	{
		int i = fwrite(this->GetFileName(),256,1,file);
	}
	fclose(file);
	
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(DEFAULTEDITOR);
	
	MessageBox(_T("기본 편집기로 설정 되었습니다!."),_T("알림"),MB_OK);
}

void CEditor_Setup_Dlg::SetFileName(CString fileName)
{
	this->fileName = fileName;
}

CString CEditor_Setup_Dlg::GetFileName()
{
	return this->fileName;
}

BOOL CEditor_Setup_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//이미지 편집기 실행 파일 저장 파일 없을 시 미리 생성
	
	FILE* checkFile;
	checkFile = _tfopen(EDITORSETUPFILE,_T("rb"));
	if( checkFile == NULL )
	{		
		FILE* file;
		file = _tfopen(EDITORSETUPFILE,_T("wb"));
		// KOL.UDF.022
		//if( file != NULL ){
		if( &file != NULL ){			
			fseek(file, 0, SEEK_END);		
			if(0 == ftell(file))
			{
				fputc(0xFF, file);
				fputc(0xFE, file);
			}
			int i = _ftprintf(file, DEFAULTEDITOR);			
			/*
			CString sFilenameTemp = DEFAULTEDITOR;
			char* cFilenameTemp = (LPCTSTR)(LPTSTR)sFilenameTemp;
			int i = fwrite(cFilenameTemp,sizeof(cFilenameTemp),1,file);*/
		}
		fclose(file);		
	}
	else if( checkFile != NULL )	// NULL이 아닌 경우 설정되어 있는 파일 불러와서 파일경로에 출력하기
	{
		TCHAR exeFilePath[256] = {0};
		fread(exeFilePath,256,1,checkFile);
		GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(exeFilePath);
		fclose(checkFile);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEditor_Setup_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (pMsg->hwnd == GetDlgItem(IDC_EDIT_FILE_PATH)->GetSafeHwnd())
		
	{
		
		// 엔터키가 눌러졌을때
		
		if (pMsg->wParam == VK_RETURN)
			
		{
			
			OnBtnOk();
			return TRUE;
			
		}
		
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
