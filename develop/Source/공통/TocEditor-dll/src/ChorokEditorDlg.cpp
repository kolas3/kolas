// ChorokEditor.cpp : implementation file
//

#include "stdafx.h"
#include "ChorokEditorDlg.h"

#include "TocEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChorokEditorDlg dialog


CChorokEditorDlg::CChorokEditorDlg(CString sChorokText, CString sKisaNo, CString sKisaTitle, INT nLine, CWnd* pParent)
	: CDialog(CChorokEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChorokEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentWnd= pParent ;
	m_sFirstChorokText= m_sChorokText= sChorokText ;
	m_sKisaTitle= sKisaTitle ;
	m_nLine= nLine ;
	m_sKisaNo= sKisaNo ;
	_tcscpy(m_szCurrentPath, _T(""));
}


VOID CChorokEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChorokEditorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChorokEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CChorokEditorDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_VIEWER, OnViewerButton)
	ON_BN_CLICKED(IDC_BUTTON_SOOSIK, OnSoosikButton)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON_FILEIMPORT, OnButtonFileimport)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChorokEditorDlg message handlers

BOOL CChorokEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	m_ChorokDlg.MyCreate(this, m_pParentWnd) ;
	m_ChorokDlg.SetChorok(m_sFirstChorokText, m_nLine) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CChorokEditorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here

	DetermineEnableButton() ;
}

CString CChorokEditorDlg::GetChorokText()
{
	return m_sChorokText ;
}

VOID CChorokEditorDlg::OnViewerButton()
{
	m_ChorokDlg.ShowViewer(m_nLine) ;
}

VOID CChorokEditorDlg::OnSoosikButton()
{
	m_ChorokDlg.ExecuteSoosikEditor() ;
}

VOID CChorokEditorDlg::OnDeleteButton()
{
	INT retval= AfxMessageBox( _T("현재 초록이 삭제됩니다!\r\n정말 삭제하겠습니까?") , MB_ICONWARNING|MB_YESNO) ;
	if ( retval!=IDYES )	return ;
	m_sChorokText= _T("") ;

	CDialog::OnOK();
}

VOID CChorokEditorDlg::OnOK() 
{
	// TODO: Add extra validation here
	if ( ! m_ChorokDlg.CheckChorokSyntax() )	// 문법체크!
	{
		AfxMessageBox( _T("초록 문법형식에 맞지 않습니다!" )) ;
		return ;
	}

	CString sChorokText ;
	sChorokText= m_ChorokDlg.GetChorokText() ;

	m_sChorokText= sChorokText ;

	CDialog::OnOK();
}

VOID CChorokEditorDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if ( ! MakeSureDisregard() )	return ;

	CDialog::OnCancel();
}

VOID CChorokEditorDlg::OnButtonFileimport() 
{
	// TODO: Add your control notification handler code here
	if ( ! MakeSureDisregard() )	return ;

	// 현재 경로를 저장한다.
	//탐색경로
	TCHAR szOriginPath[1024] = _T("\0"); //현재경로
	GetCurrentDirectory(1024,szOriginPath);
	// 탐색 시작
	CFileDialog filedlg(true,_T(""),_T(""),OFN_HIDEREADONLY|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST, _T("목차/초록 (*.toc) |*.toc|텍스트 (*.txt) |*.txt|"),this);
POPUPDLG:
	if(_tcscmp(m_szCurrentPath,_T(""))!=0)
		SetCurrentDirectory(m_szCurrentPath);
	if(filedlg.DoModal()==IDOK){
		GetCurrentDirectory(1024,m_szCurrentPath);
		SetCurrentDirectory(szOriginPath);
		CFile file;
		if(!file.Open(filedlg.GetPathName(),CFile::modeRead | CFile::typeBinary)){
			AfxMessageBox(_T("화일을 열 수 없습니다."));
			goto ERR_RETURN;
		}
		else{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
			
			// 1. 편집기의 내용백업
			CString strBackup;
			strBackup=m_ChorokDlg.GetChorokText();

			// 2. 오픈된 스트림 에서 편집기에 넣어준다.
			CString strFileStream;
			if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
				strFileStream.ReleaseBuffer();	file.Close();
				goto ERR_RETURN;
			}
			strFileStream.ReleaseBuffer();
			file.Close();
			m_ChorokDlg.SetChorok( strFileStream );

			// 3. 그대로 진행할것인지 사용자에게 묻는다.
			if(AfxMessageBox(_T("계속하시겠습니까?"),MB_YESNO)==IDNO){
				m_ChorokDlg.SetChorok( strBackup );
				goto POPUPDLG;
			}
		}
	}
	SetCurrentDirectory(szOriginPath);
	return ;
ERR_RETURN:
	SetCurrentDirectory(szOriginPath);
	return ;
}

VOID CChorokEditorDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	if ( ! MakeSureDisregard() )	return ;

	CString sChorokText ;
	INT nLine = 0 ;
	nLine= ((CTocEditorDlg*)m_pParentWnd)->GetChoroknKisa(sChorokText, m_sKisaNo, m_sKisaTitle, m_nLine, TRUE) ;
	if ( nLine<=0 ) {
		AfxMessageBox( _T("에러 발생"), MB_ICONERROR ) ;
		return ;
	}

	m_sFirstChorokText= m_sChorokText= sChorokText ;
	m_ChorokDlg.SetChorok(m_sFirstChorokText) ;
	m_nLine= nLine ;

	DetermineEnableButton() ;
}

VOID CChorokEditorDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if ( ! MakeSureDisregard() )	return ;

	CString sChorokText ;
	INT nLine = 0 ;
	nLine= ((CTocEditorDlg*)m_pParentWnd)->GetChoroknKisa(sChorokText, m_sKisaNo, m_sKisaTitle, m_nLine, FALSE) ;
	if ( nLine<=0 ) {
		AfxMessageBox( _T("에러 발생"), MB_ICONERROR ) ;
		return ;
	}

	m_sFirstChorokText= m_sChorokText= sChorokText ;
	m_ChorokDlg.SetChorok(m_sFirstChorokText) ;
	m_nLine= nLine ;

	DetermineEnableButton() ;
}

VOID CChorokEditorDlg::DetermineEnableButton()
{
	// 타이틀바에 기사 넣어주기
	if ( m_sKisaTitle.IsEmpty() )
		SetWindowText( _T("연속간행물 초록편집"));
	else
		SetWindowText( _T("연속간행물 초록편집 - ") + m_sKisaTitle );

	// 이전, 다음 버튼 활성화 또는 비활성화 시켜주기
	if ( ((CTocEditorDlg*)m_pParentWnd)->KisaExistPrevnNext(m_nLine, TRUE) )
	{
		GetDlgItem(IDC_BUTTON_PREV)->EnableWindow() ;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE) ;
	}

	if ( ((CTocEditorDlg*)m_pParentWnd)->KisaExistPrevnNext(m_nLine, FALSE) )
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow() ;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE) ;
	}
}

BOOL CChorokEditorDlg::MakeSureDisregard()
{
	CString sChorokText ;
	sChorokText= m_ChorokDlg.GetChorokText() ;

	if( m_sFirstChorokText != sChorokText ) {
		INT retval= MessageBox( _T("변경사항을 무시하겠습니까?"), _T("초록 편집 확인"), MB_YESNO|MB_ICONWARNING ) ;
		if ( retval == IDNO )	return FALSE ;
	}

	return TRUE ;
}

BOOL CChorokEditorDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{	
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
