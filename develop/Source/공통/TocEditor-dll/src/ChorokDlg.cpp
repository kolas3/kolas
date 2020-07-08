// ChorokDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChorokDlg.h"

#include "TocViewerDlg.h"

#include "TocEditorDef.h"

#include "ChorokEditorDlg.h"
#include "TocEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PRINT_LINE_NUM	31
#define LINE_HEIGHT	12		// 한줄 높이 ( pixel )

/////////////////////////////////////////////////////////////////////////////
// CChorokDlg dialog


CChorokDlg::CChorokDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChorokDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChorokDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentWnd= NULL ;
	m_pTocEditorDlgWnd= NULL ;
	m_nLine= 0 ;	// 0 이면 단행본
	m_bPrev= m_bNext= FALSE ;
}


VOID CChorokDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChorokDlg)
	DDX_Control(pDX, IDC_CHOROKEDITOR, m_ChorokEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChorokDlg, CDialog)
	//{{AFX_MSG_MAP(CChorokDlg)
	ON_BN_CLICKED(IDC_VIEWER, OnViewer)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(EN_SELCHANGE, IDC_CHOROKEDITOR, OnSelchangeEditRichedit)
	ON_EN_CHANGE(IDC_CHOROKEDITOR, OnChangeToceditor)
	ON_MESSAGE(WM_READ_ECOMATH, OnReadEcoMath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChorokDlg message handlers

VOID CChorokDlg::OnViewer() 
{
	// TODO: Add your control notification handler code here
	CString sText ;
	m_ChorokEdit.GetWindowText( sText ) ;
	
	CTocViewerDlg dlg(m_pTocEditorDlgWnd) ;
	dlg.SetTocViewer( sText, FALSE, !m_nLine ) ;
	dlg.SetViewer(_T("초록"), m_nLine) ;

	if ( m_nLine==0 )	// 단행본
	{
	}
	else				// 연간물
	{
		m_bPrev= ((CTocEditorDlg*)m_pTocEditorDlgWnd)->KisaExistPrevnNext(m_nLine, TRUE, TRUE) ;
		m_bNext= ((CTocEditorDlg*)m_pTocEditorDlgWnd)->KisaExistPrevnNext(m_nLine, FALSE, TRUE) ;
	}
	dlg.EnablePrevNext(m_bPrev, m_bNext) ;

	dlg.DoModal() ;
}

//DEL VOID CChorokDlg::OnOK() 
//DEL {
//DEL 	// TODO: Add extra validation here
//DEL 	m_ChorokEdit.GetWindowText( m_sChorokText ) ;
//DEL 
//DEL 	CDialog::OnOK();
//DEL }

//DEL VOID CChorokDlg::OnCancel() 
//DEL {
//DEL 	// TODO: Add extra cleanup here
//DEL 	m_ChorokEdit.GetWindowText( m_sChorokText ) ;
//DEL 	if ( m_sFirstContents != m_sChorokText ) {
//DEL 		INT retval= MessageBox( _T("편집하던 초록을 저장없이 닫겠습니까?"), _T("초록 닫기 확인"), MB_YESNO|MB_ICONWARNING ) ;
//DEL 		if ( retval == IDNO )	return ;
//DEL 	}
//DEL 
//DEL 	CDialog::OnCancel();
//DEL }

VOID CChorokDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_ChorokEdit.SetParentWnd(this, m_pTocEditorDlgWnd) ;
	m_ChorokEdit.SetWindowText( m_sChorokText ) ;
	m_sFirstContents= m_sChorokText ;
}

CString CChorokDlg::GetChorokText()
{
	m_ChorokEdit.GetWindowText( m_sChorokText ) ;
	return m_sChorokText ;
}

VOID CChorokDlg::OnSelchangeEditRichedit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SELCHANGE *pSelChange = reinterpret_cast<SELCHANGE *>(pNMHDR);
	// TODO: The control will not send this notification unless you override the
	// CDialog::OnInitDialog() function to send the EM_SETEVENTMASK message
	// to the control with the ENM_SELCHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
/*
	LONG max = pSelChange->chrg.cpMax ;

	CString strEditText ;
	m_ChorokEdit.GetWindowText( strEditText ) ;
	if ( strEditText.IsEmpty() ) return ;
	if ( strEditText.GetLength() < max )	max = strEditText.GetLength() ;

	INT curLine = 1;
	for ( INT idx=0 ; idx < max ; idx++ )
		if ( strEditText.GetAt(idx) == '\n' ) curLine++ ;
*/
	*pResult = 0;
}

VOID CChorokDlg::OnChangeToceditor() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

BOOL CChorokDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg -> message == WM_KEYDOWN )
	{
		// 단축키
		if ( GetAsyncKeyState(VK_CONTROL) ) {
			switch( pMsg->wParam ) {
			case 78 :	//	Ctrl + 'N'	- New Toc
			case 79 :	//	Ctrl + 'O'	- Open
			case 83 :	//	Ctrl + 'S'	- Save
			case 80 :	//	Ctrl + 'P'	- Print
			case 67 :	//	Ctrl + 'C'	- Copy
			case 86 :	//	Ctrl + 'V'	- Paste
			case 88 :	//	Ctrl + 'X'	- Cut
			case 89 :	//	Ctrl + 'Y'	- Redo
			case 90 :	//	Ctrl + 'Z'	- Undo
				break ;
			case VK_HOME :	//	Ctrl + 'Home'
				m_ChorokEdit.SendMessage( WM_VSCROLL , SB_TOP , NULL ) ;
				break ;
			case VK_END :	//	Ctrl + 'End'
				m_ChorokEdit.SendMessage( WM_VSCROLL , SB_BOTTOM , NULL ) ;
				break ;
			case 65 :	//	Ctrl + 'A'	- 전체선택
				break ;
			default :
				return TRUE ;
			}
		}

		if ( pMsg->wParam == VK_ESCAPE ) {	// ESC 키를 막는다.
			return TRUE;
		}
/*
	} else if ( pMsg -> message == WM_RBUTTONDOWN ) {
		long nStartChar, nEndChar ;
		m_ChorokEdit.GetSel(nStartChar, nEndChar) ;
		if ( nStartChar==nEndChar )
			::SendMessage( m_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, pMsg->lParam ) ;
*/
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CChorokDlg::SetChorok(CString sChorokText, UINT nLine/*=0*/)
{
	m_sChorokText= sChorokText ;
//	if(!CheckChorokSyntax()) return FALSE;
	if ( m_ChorokEdit.m_hWnd!=NULL ) {
		m_ChorokEdit.SetWindowText(m_sChorokText) ;
	}

	m_nLine= nLine ;
	return TRUE;
}

INT CChorokDlg::MyCreate( CWnd *pParentWnd, CWnd *pTocEditorDlgWnd, CRect rect/*=CRect(0,0,0,0)*/ )
{
	if (pParentWnd==NULL || pTocEditorDlgWnd==NULL) {
		AfxMessageBox(_T("초록 편집 화면을 띄우는데 실패했습니다!")) ;
		return -1;
	}

	m_pParentWnd= pParentWnd ;
	m_pTocEditorDlgWnd= pTocEditorDlgWnd ;

	if ( Create(IDD_CHOROKEDITOR_DIALOG,pParentWnd) != 0 ) {
		if ( ! rect.IsRectEmpty() ) {

			CRect rcEdit(0,0,0,0) ;
			rcEdit.right= rect.right - rect.left  ;		// 7 만큼 떨어져 있다.
			rcEdit.bottom= rect.bottom - rect.top ;		// 7 만큼 떨어져 있다.

			GetDlgItem(IDC_CHOROKEDITOR)->MoveWindow(rcEdit) ;
			MoveWindow(rect);

		}

		ShowWindow(SW_SHOW);
	}
	else {
		AfxMessageBox(_T("초록 편집 화면을 띄우는데 실패했습니다!")) ;
		return -2 ;
	}

	return 0 ;
}

INT CChorokDlg::ExecuteSoosikEditor()
{
	m_cSoosikEditor.RemoveSharedMemoryPool() ;

	INT retval= m_cSoosikEditor.ExecuteSoosikEditor(m_hWnd) ;	// 핸들을 넘겨주자.

	return retval ;
}

INT CChorokDlg::ReadFromSoosikEditor()
{
	CString sRead ;
	sRead= m_cSoosikEditor.ReadFromSoosikEditor() ;

	if ( sRead.IsEmpty() )
		return -1 ;

	long nStartChar, nEndChar ;
	nStartChar= nEndChar= 0;
	m_ChorokEdit.GetSel( nStartChar, nEndChar ) ;
	CString sChorok ;
	m_ChorokEdit.GetWindowText(sChorok) ;
	sChorok.Insert( nStartChar, sRead ) ;

	m_ChorokEdit.SetWindowText(sChorok) ;

	INT nLength= sRead.GetLength() ;
	m_ChorokEdit.SetSel( nStartChar+nLength , nStartChar+nLength ) ;

	return 0 ;
}

VOID CChorokDlg::OnReadEcoMath()
{
	ReadFromSoosikEditor() ;
}

INT CChorokDlg::CheckChorokSyntax()
{
	CString sChorok ;
	m_ChorokEdit.GetWindowText(sChorok) ;

	CString sStartTag = CHOROK_START_TAG;
	CString sEndTag = CHOROK_END_TAG;

	sStartTag.Format(_T("<id="));
	sEndTag.Format(_T(">"));


			



		INT nTotalLine= m_ChorokEdit.GetLineCount() ;
		CString sOneLine ;
		INT nStart= 0 ;
		INT nFind= -1 ;
		INT nTagStart = -1;
		for ( INT nIdx=1 ; nIdx<=nTotalLine ; nIdx++ )
		{
			nFind= sChorok.Find( _T("\r\n") , nStart ) ;
			if ( nIdx==nTotalLine )	sOneLine= sChorok.Mid(nStart) ;
			else					sOneLine= sChorok.Mid(nStart, nFind-nStart) ;

			sOneLine.TrimLeft() ;
			sOneLine.TrimRight() ;
			if ( ! sOneLine.IsEmpty() )
			{
					if ( sStartTag==sOneLine.Left(sStartTag.GetLength()) )
					{
						if ( sOneLine.Mid( sOneLine.GetLength()-1 )==sEndTag )
							continue; //return TRUE;
						else
							return -1; //return FALSE ;
					}
					else
						return -1; //return FALSE ;
				
			}

			nStart= nFind+2 ;
		}

	sStartTag.Format(_T("<kabs>"));
	sEndTag.Format(_T("</kabs>"));
	nTagStart = sChorok.Find( sStartTag);
	if( nTagStart < 0) return -2;
	if( nTagStart > sChorok.Find( sEndTag ) ) return -2;
	
	
	sStartTag.Format(_T("<fabs>"));
	sEndTag.Format(_T("</fabs>"));
	nTagStart = sChorok.Find( sStartTag);
	if( nTagStart < 0) return -2;
	if( nTagStart > sChorok.Find( sEndTag ) ) return -3;

	return 0;
}

VOID CChorokDlg::SetReadOnly(BOOL bReadOnly)
{
	if ( bReadOnly )
		m_ChorokEdit.SetBackgroundColor(FALSE, RGB(191,191,191)) ;
	else
		m_ChorokEdit.SetBackgroundColor(FALSE, RGB(255,255,255)) ;

	m_ChorokEdit.SetReadOnly(bReadOnly);
}

VOID CChorokDlg::ShowView()
{
	OnViewer();
}

VOID CChorokDlg::ShowViewer(BOOL bPrev, BOOL bNext)
{
	m_bPrev= bPrev;
	m_bNext= bNext;
	ShowView();
}

VOID CChorokDlg::ShowViewer(UINT nLine)
{
	m_nLine= nLine;
	ShowView();
}
