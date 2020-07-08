// TocEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocEditorDlg.h"
#include "ErrorQueue.h"

#include "TocSyntaxChecker.h"
#include "ChorokDlg.h"
#include "TocViewerDlg.h"

#include "ChorokEditorDlg.h"

#include "BibilographyDlg.h"
#include "BodyDlg.h"
#include "YonSujiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINE_HEIGHT			12			// 한줄 높이 ( pixel )
#define BODY_START_TAG		_T("<body>")
#define BODY_START_LENGTH	6
#define BODY_END_TAG		_T("</body>")

CStringArray g_arrAlreadyExistKisaNo;	// 임시변수 - 나중에 멤버로 옮기기

/////////////////////////////////////////////////////////////////////////////
// CTocEditorDlg dialog


CTocEditorDlg::CTocEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTocEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTocEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nStartChar = m_nEndChar = 0 ;
	m_icurLine = 1 ;
	m_nIsKindOf = _DAN_SINGLE ;	// 단행본(단일본,다권본), 연속간행물 default는 단일단행본
	m_bUpDown = FALSE ;		// PAGE_UP or PAGE_DOWN key
	m_bTagEnd = FALSE ;		// '>' 뒤에 폰트(색) 변경 위해

	m_pParentWnd= NULL ;
	m_pYonKeyFunc= NULL ;
	m_pMoveRecordFunc= NULL ;

	Clear() ;

	m_aChorokStru.SetSize(0,1) ;
	m_aBackupChorokStru.SetSize(0,1) ;
	m_aRedoChorokStru.SetSize(0,1) ;
	m_arrWonmunKisaNo.SetSize(0,1) ;

	bIsValidKey= FALSE ;	// tmp
	bIsCut= FALSE ;			// tmp
//	csId.Empty(); csCall.Empty(); csCount.Empty(); csDate.Empty();
//	csPub.Empty(); csSertitle.Empty(); csTitle.Empty(); csIssue.Empty(); csKey.Empty();
	for(INT i=0;i<8;i++) csstrTag[i].Empty();
}

CTocEditorDlg::~CTocEditorDlg()
{
	m_aChorokStru.RemoveAll() ;
	m_aChorokStru.FreeExtra() ;
	m_aBackupChorokStru.RemoveAll() ;
	m_aBackupChorokStru.FreeExtra() ;
	m_aRedoChorokStru.RemoveAll() ;
	m_aRedoChorokStru.FreeExtra() ;

	m_arrWonmunKisaNo.RemoveAll() ;
	m_arrWonmunKisaNo.FreeExtra() ;

	m_pYonKeyFunc= NULL ;
	m_pMoveRecordFunc= NULL ;
}

VOID CTocEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTocEditorDlg)
	DDX_Control(pDX, IDC_TOCEDITOR, m_TocEdit);
	DDX_Control(pDX, IDC_LINE_VIEW, m_LineView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTocEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CTocEditorDlg)
	ON_COMMAND(ID_TOC_UNDO, OnUndo)
	ON_COMMAND(ID_TOC_REDO, OnRedo)
	ON_COMMAND(ID_TOC_CUT, OnTocCut)
	ON_COMMAND(ID_TOC_COPY, OnTocCopy)
	ON_COMMAND(ID_TOC_PASTE, OnTocPaste)
	ON_COMMAND(ID_CHECK_SYNTAX, OnCheckSyntax)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(EN_SELCHANGE, IDC_TOCEDITOR, OnSelchangeEditRichedit)
	ON_EN_CHANGE(IDC_TOCEDITOR, OnChangeToceditor)
	ON_WM_TIMER()
	ON_MESSAGE(MSG_EDIT_YON_CHOROK, OnEditYonChorok)
	ON_MESSAGE(MSG_EDIT_DAN_CHOROK, OnEditDanChorok)
	ON_MESSAGE(WM_READ_ECOMATH, OnReadEcoMath)
	ON_MESSAGE(MSG_SHOW_VIEWER, OnShowViewer)
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocEditorDlg message handlers

BOOL CTocEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_TocEdit.SetEventMask( m_TocEdit.GetEventMask() | ENM_CHANGE | ENM_SELCHANGE ) ;
	
//	m_TocEdit.SetOptions(ECOOP_SET, ~ECO_NOHIDESEL);
	m_LineView.SetBackgroundColor(FALSE, RGB(238,238,238));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CTocEditorDlg::OnUndo() 
{
//	return ;		// 사용하지 않도록 하자.(한글문제)

	if ( m_sBackupTocEdit.IsEmpty() ) {
		TRACE( _T("Undo Empty!\n")) ;
		return ;
	}

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
	CString tmp ;
	m_TocEdit.GetWindowText( tmp ) ;
	m_sRedoTocEdit= tmp ;
	m_aRedoChorokStru.Copy(m_aChorokStru) ;
	m_TocEdit.SetWindowText( m_sBackupTocEdit ) ;
	m_aChorokStru.Copy(m_aBackupChorokStru) ;
	m_sBackupTocEdit.Empty() ;
	m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;
	
	DisplayCurrentLineNumber() ;
}

VOID CTocEditorDlg::OnRedo() 
{
//	return ;		// 사용하지 않도록 하자.(한글문제)

	if ( m_sRedoTocEdit.IsEmpty() ) {
		TRACE(_T("Redo Empty!\n") ) ;
		return ;
	}

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;

	TocEditorBackup() ;

	m_TocEdit.SetWindowText( m_sRedoTocEdit ) ;
	m_aChorokStru.Copy(m_aRedoChorokStru) ;

	m_sRedoTocEdit.Empty() ;
	m_aRedoChorokStru.RemoveAll() ;

	m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;

	DisplayCurrentLineNumber() ;
}

VOID CTocEditorDlg::OnTocCut() 
{
	// 백업 해준다.
	m_TocEdit.GetSel( m_nStartChar , m_nEndChar ) ;
	if ( m_nStartChar==m_nEndChar ) return ;

	TocEditorBackup() ;

	m_TocEdit.Cut() ;

	DisplayCurrentLineNumber() ;
}

VOID CTocEditorDlg::OnTocCopy() 
{
	m_TocEdit.Copy() ;
}

VOID CTocEditorDlg::OnTocPaste() 
{
//	if (! ::IsClipboardFormatAvailable(CF_TEXT))	return ;
	if (! m_TocEdit.CanPaste() )	return ;

	HGLOBAL hglb=NULL;
	CString sText ;
	OpenClipboard();

	//++2008.09.04 UPDATE BY CJY {{++
	// 유니코드형식으로 변경
	hglb= GetClipboardData(CF_UNICODETEXT);
	// 이전소스
	// hglb= GetClipboardData(CF_TEXT);
	//--2008.09.04 UPDATE BY CJY --}}
	
	if (hglb != NULL)
	{
		sText= (LPCTSTR)(LPCTSTR)GlobalLock(hglb);
		GlobalUnlock(hglb);
	}
	CloseClipboard();

	if ( sText.IsEmpty() )	return ;

	INT nStartFind= sText.Find(_START_CHOROK_KEY_) ;
	if (nStartFind>=0)
	{
		if ( sText.Find(_END_CHOROK_KEY_ , nStartFind)>0 )
		{
			MessageBox(_T("기사제어번호 태그가 있는 텍스트는 붙여넣기 할 수 없습니다."), _T("경고"), MB_ICONWARNING) ;
			return ;
		}
	}

	LONG nStartChar, nEndChar ;
	m_TocEdit.GetSel( nStartChar, nEndChar ) ;
	m_TocEdit.SetSel( nStartChar-1, nEndChar-1 ) ;

	if( m_TocEdit.GetSelText()==_T(">") ) m_bTagEnd = TRUE ;
	m_TocEdit.SetSel( nStartChar, nEndChar ) ;

	// 백업 해준다.
	TocEditorBackup() ;

	m_TocEdit.Paste() ;

	long nIdx= nStartChar + sText.GetLength() ;

	// 태그에 색을 다시 칠해주기 위해
	m_TocEdit.GetWindowText( m_strBak ) ;
	m_TocEdit.SetWindowText( m_strBak ) ;

	m_TocEdit.SetSel(nIdx, nIdx) ;
//	m_TocEdit.DisplayTagColor(TRUE);

	DisplayCurrentLineNumber() ;
}

INT CTocEditorDlg::CheckSyntax()
{
	long nStartChar, nEndChar ;
	nStartChar= nEndChar= 0 ;

	m_TocEdit.HideSelection(TRUE, TRUE);

	m_TocEdit.GetSel( nStartChar, nEndChar ) ;

	CErrorQueue ErrorQueue;
	ErrorQueue.ResetContent();

//	cfg 파일 경로설정.
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( _T('\\') ) ;
	if( idx<=0 ) {
		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\n문법체크를 수행할 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return -2 ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_CFG ;

	CTocSyntaxChecker TocSyntaxChecker(& ErrorQueue, sWorkDir, m_nIsKindOf);

	// 태그에 색을 다시 칠해주기 위해
	m_TocEdit.GetWindowTextNoMath( m_strBak ) ;
//	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
//	m_TocEdit.SetWindowText( m_strBak );
	m_TocEdit.DisplayTagColor(FALSE);
//	m_TocEdit.SetSel(0, -1);
//	m_TocEdit.SetSelectionCharFormat(m_TocEdit.cfBase);
//	m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;

	m_TocEdit.DisplayTagColor(TRUE);

	// 새로 임시 파일을 생성한후 그것을 넘겨준다.	나중에 지운다.
	TCHAR szPath[MAX_PATH];
	TCHAR szTempFileName[MAX_PATH];
	GetTempPath(MAX_PATH, szPath);
	GetTempFileName(szPath, _T("toc"), 0, szTempFileName);
	SaveFile(szTempFileName);

	TocSyntaxChecker.CheckSyntax(szTempFileName);
	DeleteFile(szTempFileName);

	m_SyntaxCheckDlg.DeleteAllItems() ;

	INT iMaxCount = ErrorQueue.GetErrorCount();
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		m_SyntaxCheckDlg.SetErrorMsg(_T(" ") + ErrorQueue.GetLine(i), ErrorQueue.GetErrorMessage(i), ErrorQueue.GetSolution(i), i) ;
	}

	m_TocEdit.SetSel( nStartChar, nEndChar ) ;

	if( iMaxCount > 0 ) {	// 오류 있음
		m_SyntaxCheckDlg.ShowWindow(SW_SHOW) ;
		return -1 ;
	}
	else {					// 오류 없음
		m_SyntaxCheckDlg.ShowWindow(SW_HIDE) ;
		return 0 ;
	}

	m_TocEdit.HideSelection(FALSE, TRUE);

}

VOID CTocEditorDlg::OnCheckSyntax()
{
	INT retval= CheckSyntax() ;
	if ( retval==0 )
	{
		MessageBox( _T("오류가 없습니다."), _T("목차 오류체크") ) ;
	}
}

VOID CTocEditorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_SyntaxCheckDlg.Create(IDD_SYNTAXCHECK_DIALOG, this) ;	// Modaless 로 띄워준다.
	m_SyntaxCheckDlg.SetParentWnd(this) ;

	m_TocEdit.SetKindOf(m_nIsKindOf, this) ;

	// 라인 표시 - 1
	m_LineView.SetWindowText(_T("1"));

	m_TocEdit.OnInitialUpdate();
	
}

VOID CTocEditorDlg::SetCurrentErrorLine(INT iLine)
{
	if ( iLine<=0 )	return ;

	m_TocEdit.SetSelectionLine(iLine-1);

	while( TRUE ) {
		INT nVisibleTopLine = m_TocEdit.GetFirstVisibleLine() + 1;
		INT nVisibleBottomLine = GetVisibleBottomLine() ;

		// Error line을 화면에 보여준다.
		if( iLine < nVisibleTopLine )
			m_TocEdit.SendMessage( WM_VSCROLL , SB_PAGEUP , NULL ) ;
		else if( iLine > nVisibleBottomLine )
			m_TocEdit.SendMessage( WM_VSCROLL , SB_PAGEDOWN , NULL ) ;
		else break ;
	}

}

BOOL CTocEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == ID_TOC_NEW)
		AfxMessageBox(_T("new"));

	bIsValidKey= FALSE ;
	bIsCut= FALSE ;		//	Ctrl + 'X'	- Cut
	long nStartChar, nEndChar ;
	m_TocEdit.GetSel( nStartChar, nEndChar ) ;

	switch ( pMsg -> message ) {
	case WM_KEYDOWN :

		if( pMsg -> wParam == VK_UP || pMsg -> wParam == VK_DOWN ) m_bUpDown = TRUE ;

		if( pMsg->wParam >= 0x30 && pMsg->wParam <= 0x39 )		bIsValidKey = TRUE ;	// '0' ~ '9'
		else if( pMsg->wParam >= 0x41 && pMsg->wParam <= 0x5A )	bIsValidKey = TRUE ;	// 'A' ~ 'Z'
		else if( pMsg->wParam >= 0x60 && pMsg->wParam <= 0x6f )	bIsValidKey = TRUE ;	// numpad ; 96<pMsg->wParam<111
		else if( pMsg->wParam >= 0xba && pMsg->wParam <= 0xc0 )	bIsValidKey = TRUE ;	// 186<pMsg->wParam<192
		else if( pMsg->wParam >= 0xdb && pMsg->wParam <= 0xde )	bIsValidKey = TRUE ;	// 219<pMsg->wParam<222
		else if( pMsg->wParam == 0xe5 )	{	// 한글 입력시
//			m_TocEdit.GetWindowText( m_strBak ) ;
			bIsValidKey= TRUE ;
		}
		else {
			switch( pMsg->wParam ) {
			case VK_DELETE :
			case VK_BACK :
//				DisplayCurrentLineNumber() ;
			case VK_SPACE :
			case VK_RETURN :
				bIsValidKey = TRUE ;
				break ;
			}
		}

		if ( GetAsyncKeyState(VK_CONTROL) )
		{
			switch( pMsg->wParam ) {
			case 86 :	//	Ctrl + 'V'	- Paste
			case 88 :	//	Ctrl + 'X'	- Cut
				bIsValidKey= TRUE ;
				bIsCut= TRUE ;
				break ;
			default :
				bIsValidKey= FALSE ;
				break ;
			}
		}
		

		// 연간물에서 기사제어번호를 처리하기 위해서...
		if ( m_nIsKindOf==_YON_ && bIsValidKey )
		{
			CString sSelText ;
			sSelText= m_TocEdit.GetSelText() ;
			m_TocEdit.GetWindowText(m_strBak) ;

			if ( nStartChar!=nEndChar )
			{
				// 기사제어번호 태그 보호 루틴!
				// 기사제어번호태그는 편집할 수 없도록 해준다.
				if ( m_strBak.GetLength()>nEndChar && m_strBak.Mid( nEndChar, 2 )==_T("\r\n") )	// 한라인에서 목차키 지울때 nEndChar이 맨 끝에 걸쳐있을경우
				{
					if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar+1 ) )	// nEndChar 라인
					{
						return TRUE ;
					}
				}
				else
				{
					if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar ) )	// nEndChar 라인
					{
						return TRUE ;
					}
				}
				// nStartChar 라인
				{
//					if ( sSelText.Find(_T("\r\n"))>=0 )
//					{
						INT curLine= 1 ;
						for( INT idx=0 ; idx<nStartChar ; idx++ ) {
							if( m_strBak.GetAt(idx)=='\n' ) curLine++ ;
						}

						if ( IsInKisaTag(m_strBak, curLine, nStartChar ) )
						{
							return TRUE ;
						}
//					}
				}

				// 사용자가 선택한 셀에서 기사제어번호가 있다면 삭제메시지 후, 없애준다.
				CString sStartTag= _START_CHOROK_KEY_ ;
				CString sEndTag= _END_CHOROK_KEY_ ;
				if ( nEndChar-nStartChar>sStartTag.GetLength()+sEndTag.GetLength() )
				{
					INT nStart= 0 ;
					INT nFindStart= -1 ;
					INT nFindEnd= -1 ;
					CStringArray arrDeletedKisaNo ;
					arrDeletedKisaNo.SetSize(0,1) ;

					do {
						nFindStart= sSelText.Find( sStartTag, nStart ) ;
						if ( nFindStart>=0 )
						{
							nStart= nFindStart + sStartTag.GetLength() ;
							nFindEnd= sSelText.Find( sEndTag, nStart ) ;
							if ( nFindEnd>=0 )
							{
								CString sKisaNo ;
								sKisaNo= sSelText.Mid( nStart, nFindEnd-nStart) ;
								if ( IsYonKisaNoRight(sKisaNo) )
								{
									INT nSize= m_arrWonmunKisaNo.GetSize() ;
									for ( INT idx=0; idx<nSize ; idx++)
									{
										if ( sKisaNo==m_arrWonmunKisaNo.GetAt(idx) )
										{
											MessageBox(_T("원문이 구축된 기사는 지울 수 없습니다."), _T("경고"), MB_ICONWARNING|MB_OK ) ;
											return TRUE ;
										}
									}
									arrDeletedKisaNo.Add(sKisaNo) ;
								}

								nStart= nFindEnd + sEndTag.GetLength() ;
							}
						}

					} while ( nFindStart>=0 && nFindEnd>=0 ) ;

					if ( arrDeletedKisaNo.GetSize()>0 )
					{
						INT retval= MessageBox(_T("선택된 기사제어번호와 그에 따른 초록도 모두 지워집니다. 정말 지우겠습니까?"), _T("경고"), MB_YESNO|MB_ICONWARNING ) ;
						if ( retval!=IDYES )
						{
							return TRUE ;
						}
						else	// 기사제어번호 지우기
						{
							if ( bIsCut )
								OnTocCut() ;
							else
								TocEditorBackup() ;
							for ( INT i=0 ; i<arrDeletedKisaNo.GetSize() ; i++ )
							{
								if ( DeleteYonKisaNo(arrDeletedKisaNo.GetAt(i)) ) {
									m_sBackupTocEdit.Empty() ;
									m_aBackupChorokStru.RemoveAll() ;
									return TRUE ;			// 실패했을 경우!
								}
							}
							if ( bIsCut )	return TRUE ;
						}
					}

					arrDeletedKisaNo.RemoveAll() ;
					arrDeletedKisaNo.FreeExtra() ;
				}
			}
			else		// 기사제어번호가 있는 줄에서는 끝줄 외에는 줄바뀜이 일어나지 못하도록 하자.
			{
//				if ( IsInBody() ) {
				m_strBak= GetKisaNofromMokchaLine(m_icurLine) ;
				if ( IsYonKisaNoRight(m_strBak) )
				{
					m_TocEdit.GetWindowText(m_strBak) ;

					if ( pMsg->wParam==VK_RETURN ) {
						if ( m_strBak.Mid( nStartChar, 2 )!=_T("\r\n") ) {
							MessageBox(_T("기사제어번호가 있는 라인에서는 줄을 바꿀 수 없습니다.!"), _T("경고"), MB_ICONWARNING) ;
							return TRUE ;
						}
						else	// 맨 뒤에 리턴은 허용해주자.
							goto CONTINUE ;
					}

					// 기사제어번호키 태그의 앞에서 del 키를 누르는것 방지하기 위해
					if ( pMsg->wParam==VK_DELETE ) {
//						if ( m_strBak.Mid( nStartChar, 2 )!=_T("\r\n") ) {
							if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar+1 ) )	// nEndChar 라인
							{
								return TRUE ;
							}
//						}
					}
					// 기사제어번호 태그 보호 루틴!
					// 기사제어번호태그는 편집할 수 없도록 해준다.
					if ( IsInKisaTag(m_strBak, m_icurLine, nEndChar ) )	// nEndChar 라인
					{
						return TRUE ;
					}

				}
//				}


			}

		}

  
CONTINUE:
		// 단축키
		if( GetAsyncKeyState(VK_CONTROL) ) {
			bIsValidKey = FALSE ;
			switch( pMsg->wParam ) {
			case 78 :	//	Ctrl + 'N'	- New Toc
				break ;
			case 79 :	//	Ctrl + 'O'	- Open
				break ;
			case 83 :	//	Ctrl + 'S'	- Save
				break ;
			case 80 :	//	Ctrl + 'P'	- Print
				break ;
			case 67 :	//	Ctrl + 'C'	- Copy
				OnTocCopy() ;
				return TRUE ;
				break ;
			case 86 :	//	Ctrl + 'V'	- Paste
				OnTocPaste() ;
				return TRUE ;
			case 88 :	//	Ctrl + 'X'	- Cut
				OnTocCut() ;
				return TRUE ;
			case 89 :	//	Ctrl + 'Y'	- Redo
				OnRedo() ;
				return TRUE ;
			case 90 :	//	Ctrl + 'Z'	- Undo
				OnUndo() ;
				return TRUE ;
			case VK_HOME :	//	Ctrl + 'Home'
				m_TocEdit.SendMessage( WM_VSCROLL , SB_TOP , NULL ) ;
				break ;
			case VK_END :	//	Ctrl + 'End'
				m_TocEdit.SendMessage( WM_VSCROLL , SB_BOTTOM , NULL ) ;
				break ;
			case 65 :	//	Ctrl + 'A'	- 전체선택
				break ;
			default :
				return TRUE ;
			}
		}

		if ( pMsg->wParam == VK_F7 ) {	// F7 - 문법 체크 !
			OnCheckSyntax();
			return TRUE;
		}
		if ( pMsg->wParam == VK_ESCAPE ) {	// ESC 키를 막는다.
			return TRUE;
		}


		if(	bIsValidKey ) {
			// 앞의 문자가 '>' 인지 체크해준다.
/*			m_TocEdit.SetSel( nStartChar-1, nStartChar ) ;
			if ( m_TocEdit.GetSelText() == _T(">") )	m_bTagEnd = TRUE ;
			m_TocEdit.SetSel( nStartChar, nEndChar ) ;

			// 백업해준다.
			// 여기서는 구조체 백업을 해주면 안된다.
			m_TocEdit.GetWindowText( m_strBak ) ;
			m_sBackupTocEdit= m_strBak ;				// 백업
*/		}

		break ;

	case WM_KEYUP :
		switch( pMsg->wParam ) {
		case VK_DELETE :
		case VK_BACK :
			DisplayCurrentLineNumber() ;	// 라인 표시.
			break ;
		case 190 :	// '>' - tag 에 색을 입혀주기 위해
			if( GetAsyncKeyState(VK_SHIFT) ) {
/*				m_TocEdit.GetWindowText( m_strBak ) ;
				m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
				m_TocEdit.SetWindowText( m_strBak ) ;
				m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;
				m_bTagEnd = TRUE ;
*/
			}
			break ;
		case VK_RETURN :	// test
			if ( m_nIsKindOf==_YON_ ) {
				if ( IsInBody() ) {
					TocEditorBackup() ;

//					SetKisaNo2AllKisa();
				}
			}
			break ;
		}

		break ;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CTocEditorDlg::DisplayCurrentLineNumber()
{
	CString sMessage;
	CString sBuffer;
	INT nFirstVisible = m_TocEdit.GetFirstVisibleLine();
	INT cntLine = m_TocEdit.GetLineCount() ;
	for(INT i = nFirstVisible + 1 ; i <= cntLine ; i++)
	{
		sBuffer.Format(_T("%d "),i);
		sMessage += sBuffer + _T("\r\n");
	} 
	m_LineView.SetWindowText(sMessage);

}

VOID CTocEditorDlg::OnSelchangeEditRichedit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SELCHANGE *pSelChange = reinterpret_cast<SELCHANGE *>(pNMHDR);
	// TODO: The control will not send this notification unless you override the
	// CDialog::OnInitDialog() function to send the EM_SETEVENTMASK message
	// to the control with the ENM_SELCHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	LONG max = pSelChange->chrg.cpMax ;

	CString strEditText ;
	m_TocEdit.GetWindowText( strEditText ) ;

	if( strEditText.GetLength() < max )	max = strEditText.GetLength() ;

	INT curLine= 1 ;
	for( INT idx=0 ; idx < max ; idx++ )
		if( strEditText.GetAt(idx)=='\n' ) curLine++ ;

	m_icurLine= curLine ;
	INT nVisibleTopLine= m_TocEdit.GetFirstVisibleLine() + 1 ;
	INT nVisibleBottomLine= GetVisibleBottomLine() ;

	INT curPos= m_TocEdit.GetScrollPos(SB_VERT) ;
	if( m_bUpDown ) {
		if( nVisibleTopLine >= curLine ) {
			DisplayCurrentLineNumber() ;
		}
		if( nVisibleTopLine > curLine ) {
			DisplayCurrentLineNumber() ;
			m_bUpDown = FALSE ;
		}
	}
	else if( nVisibleBottomLine < curLine && m_bUpDown ) {
		DisplayCurrentLineNumber() ;
		m_bUpDown = FALSE ;
	}

	// line number 표시
	if( m_TocEdit.m_preScrollPos != curPos ) {
		DisplayCurrentLineNumber() ;
		m_TocEdit.m_preScrollPos = curPos ;
	}

	*pResult = 0;
}

INT CTocEditorDlg::GetTotalLine(CString &m_strFile)
{
	INT find = m_strFile.ReverseFind( _T('\n') ) ;
	if ( find==-1 ) return 1 ;
	
	if ( m_strFile.GetAt( find-1 )==_T('\r') )
		m_strFile.TrimRight( _T("\r\n") ) ;
	else 
		m_strFile.TrimRight( _T("\n") ) ;

	INT retval = 1 ;
	for ( INT idx=0 ; idx<m_strFile.GetLength() ; idx++ )
		if( m_strFile.GetAt(idx)=='\n' ) retval++ ;

	return retval ;
}

INT CTocEditorDlg::GetVisibleBottomLine()
{
	CRect rect ;
	m_TocEdit.GetClientRect( &rect ) ;	// m_TocEdit로 수정. 2001.10.9
	INT curPos = m_TocEdit.GetScrollPos(SB_VERT) ;

	return ( rect.bottom + curPos ) / LINE_HEIGHT ;
}

VOID CTocEditorDlg::OnChangeToceditor() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if( m_bTagEnd ) {		// '>' 바로 뒤의 폰트를 설정해 주기 위해
		m_TocEdit.GetWindowText( m_strBak ) ;
		m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
		m_TocEdit.SetWindowText( m_strBak ) ;
		m_TocEdit.SetSel( m_nStartChar, m_nEndChar ) ;
		m_bTagEnd = FALSE ;
//		m_TocEdit.DisplayTagColor(TRUE);
	}

	DisplayCurrentLineNumber() ;	// 라인을 보여주는 기능도 수행한다.
}

//DEL INT CTocEditorDlg::SetIsKindOf(INT nKindOf, CString sControlNo/*=_T("")*/)
//DEL {
//DEL 	m_nIsKindOf= nKindOf;
//DEL 
//DEL /*
//DEL 	if ( m_nIsKindOf==_YON_ ) {
//DEL 		if( m_sControlNo.IsEmpty() ) {
//DEL 			AfxMessageBox(_T("연간물일 경우에는 제어번호를 설정해 주어야 합니다!"), MB_ICONERROR|MB_OK) ;
//DEL 			return -1 ;
//DEL 		}
//DEL 	}
//DEL */
//DEL 	if ( m_TocEdit.m_hWnd!=NULL )
//DEL 		m_TocEdit.SetKindOf(m_nIsKindOf, this) ;
//DEL 
//DEL 	return 0 ;
//DEL }

VOID CTocEditorDlg::SetTocFromString(CString sTocContents)
{
	m_TocEdit.HideSelection(TRUE, TRUE);

	m_sFirstMokcha= sTocContents;
	m_sFirstVisible= sTocContents;

	if( m_TocEdit.m_hWnd!=NULL )
		m_TocEdit.SetWindowText(m_sFirstVisible) ;
	

	// added 2002.11.1
	if ( m_nIsKindOf==_YON_ )
	{
		GetYonAllKisaNo(g_arrAlreadyExistKisaNo);
	}
	m_TocEdit.DisplayTagColor(TRUE);
	m_TocEdit.HideSelection(FALSE, TRUE);
}

INT CTocEditorDlg::SaveFile(CString filename)
{
	CStdioFile file;
	CFileStatus status ;
	CString strError ;
	if( !file.GetStatus( filename , status ) ) {
		strError = _T("\"") + filename + _T("\" 이 존재하지 않습니다!");
		MessageBox( strError, _T("Error"), MB_OK | MB_ICONERROR );
		return -1;
	}

	if( status.m_attribute == 0x01 ) {
		strError = _T("\"") + filename + _T("\" 은 읽기 전용이므로 Write 할 수 없습니다!") ;
		MessageBox( strError, _T("Error"), MB_OK | MB_ICONERROR ) ;
		return -2 ;
	}

	CFileException ex;
	if( !file.Open(filename, CFile::modeCreate| CFile::modeWrite | CFile::typeBinary, &ex) ) {

		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		strError += _T("Couldn't open source file:\r\n\r\n");
		strError += szError;
		AfxMessageBox(strError) ;
		return -3;
	}
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strTocText;
	m_TocEdit.GetWindowTextNoMath(strTocText);
//	m_TocEdit.GetWindowText(strTocText);
//	if( strTocText.IsEmpty() ) return -1 ;

	INT idx=0;
	CString linebuf, buffer;
	while ((idx=strTocText.Find(_T("\r\n")))!=-1)
	{
		linebuf= strTocText.Mid(0, idx+1);
		linebuf.TrimRight(_T("\r\n"));
		buffer.Format(_T("%s\r\n"), linebuf);
		file.WriteString(buffer);
		strTocText= strTocText.Mid(idx+1);
	}
	
	if (strTocText!=_T(""))
	{
		linebuf= strTocText;
		linebuf.TrimRight(_T("\r\n"));
		buffer.Format(_T("%s\r\n"), linebuf);
		file.WriteString(buffer);
	}
	file.Close();
	return 0 ;
}

VOID CTocEditorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 ) {
		KillTimer(1) ;
		CheckSyntax() ;
	}

	CDialog::OnTimer(nIDEvent);
}

//DEL VOID CTocEditorDlg::OnButtonViewer() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CString sText ;
//DEL 	m_TocEdit.GetWindowText( sText ) ;
//DEL 
//DEL 	CTocViewerDlg dlg ;
//DEL 	dlg.SetTocViewer( sText ) ;
//DEL 	dlg.DoModal() ;
//DEL }

//DEL BOOL CTocEditorDlg::CheckbeforeExit()
//DEL {
//DEL 	CString sCurTocText ;
//DEL 	m_TocEdit.GetWindowText(sCurTocText) ;
//DEL 	if ( m_sFirstVisible!=sCurTocText ) {
//DEL 		INT retval = AfxMessageBox( _T("현재 편집중인 문서를 저장없이 닫습니까?") , MB_YESNO ) ;
//DEL 		switch( retval ) {
//DEL 		case IDNO :
//DEL 			return FALSE ;
//DEL 		case IDYES :
//DEL 			return TRUE ;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return TRUE ;
//DEL }

INT CTocEditorDlg::MyCreate( CWnd *pParentWnd, CRect rect/*=CRect(0,0,0,0)*/, INT IsKind/*=_DAN_SINGLE*/ )
{
	if (pParentWnd==NULL) {
		AfxMessageBox(_T("목차 편집 화면을 띄우는데 실패했습니다!")) ;
		return -1;
	}

	m_pParentWnd= pParentWnd ;

	if ( Create(IDD_TOCEDITOR_DIALOG,pParentWnd) != 0 ) {
		if ( ! rect.IsRectEmpty() ) {

			CRect rcLine(0,0,28,0) ;
			rcLine.bottom= rect.bottom- rect.top;		// 13 만큼 떨어져 있다.			

			CRect rcEdit(27,0,0,0) ;
			rcEdit.bottom= rect.bottom - rect.top ;		// 7 만큼 떨어져 있다.
			rcEdit.right= rect.right - rect.left ;			// 7 만큼 떨어져 있다.

			m_nIsKindOf= IsKind ;
//			if ( m_nIsKindOf != _YON_ ) {		// 단행물일때
				GetDlgItem(IDC_CHOROK_KEY)->ShowWindow(SW_HIDE) ;
//			}
/*
			else {						// 연간물일때

				CRect rcKisaNo ;
				rcKisaNo.top= 7 ;
				rcKisaNo.right= rect.right - rect.left - 7 ;			// 7 만큼 떨어져 있다.
				rcKisaNo.left= rcKisaNo.right - 70 ;				// width 를 40 으로 하자.
				rcKisaNo.bottom= rect.bottom - rect.top - 7 ;		// 7 만큼 떨어져 있다.

				rcEdit.right= rcKisaNo.left - 1;		// 여백을 1 로 설정하자.

				GetDlgItem(IDC_CHOROK_KEY)->MoveWindow(rcKisaNo) ;
			}
*/
			GetDlgItem(IDC_LINE_VIEW)->MoveWindow(rcLine) ;
			GetDlgItem(IDC_TOCEDITOR)->MoveWindow(rcEdit) ;
			MoveWindow(rect);
		}

		ShowWindow(SW_SHOW);
	}
	else {
		AfxMessageBox(_T("목차 편집 화면을 띄우는데 실패했습니다!")) ;
		return -2 ;
	}

	return 0 ;
}

// 이미 DB상에 작성되어 존재하는 연간물 초록을 세팅해준다.
INT CTocEditorDlg::SetExistYonChorok(CString sChorokText, CString sKisaNo)
{
	// 연간물일 경우에만 사용할 수 있도록 한다.
	if ( m_nIsKindOf!=_YON_ && m_nIsKindOf!=_KISA_ ) {
		AfxMessageBox(_T("초록 설정은 연간물일 때만 할 수 있습니다!")) ;
		return -1 ;
	}

	if ( sKisaNo.IsEmpty() ) {
		AfxMessageBox( _T("연간물 초록 설정에서 오류가 발생하였습니다!\r\n기사제어번호가 없습니다.") ) ;
		return -2 ;
	}
/*
	if ( ! IsYonKisaNoFormal(sKisaNo) )
	{
		CString sError ;
		sError= _T("다음 기사제어번호가 형식에 맞지 않습니다.\r\n") ;
		sError+= sKisaNo ;
		AfxMessageBox(sError) ;
		return -3 ;
	}
*/
	bool bFindinMokcha= TRUE ;
	if ( ! IsThereYonKisaNoinMokcha(m_sFirstMokcha, sKisaNo) )
	{
		bFindinMokcha= FALSE ;
		CString sError ;
		sError= _T("다음 기사제어번호가 목차에 존재하지 않습니다.\r\n") ;
		sError+= sKisaNo ;
		sError+= _T("\r\n삭제된것으로 처리합니다.") ;
		AfxMessageBox(sError) ;
//		return -4 ;
	}

	if( ! IsYonKisaNoUnique(sKisaNo) )
	{
		CString sError ;
		sError= _T("연간물 초록 세팅 실패!") ;
		AfxMessageBox(sError) ;
		return -5 ;
	}

	tCHOROK tOneChorok ;
	tOneChorok.sKey= sKisaNo;
	if ( !bFindinMokcha ) {		// 목차에 기사제어번호가 없는 경우 여기서는 삭제된것으로 세팅해둔다.
		tOneChorok.sKisaStatus= _CHOROK_STATUS_DELETE_ ;
		tOneChorok.sChorokText= _T("") ;
		if ( !sChorokText.IsEmpty() )
			tOneChorok.sChorokStatus= _CHOROK_STATUS_DELETE_ ;
		else
			tOneChorok.sChorokStatus= _T("") ;
	} else {
		tOneChorok.sKisaStatus= _T("") ;
		tOneChorok.sChorokText= sChorokText;
		tOneChorok.sChorokStatus= _T("") ;
	}

	m_aChorokStru.Add(tOneChorok);	// 추가해준다.
	m_aBackupChorokStru.Copy(m_aChorokStru) ;	// 백업 초기화

	if( ! bFindinMokcha )	return -6 ;

	return 0 ;
}

// DB상 원문 구축된 기사제어번호를 세팅해준다.
INT CTocEditorDlg::SetWonmunExistKisaNo(CString sKisaNo)
{
	// 연간물일 경우에만 사용할 수 있도록 한다.
	if ( m_nIsKindOf!=_YON_ ) {
		AfxMessageBox(_T("원문존재 유무 설정은 연간물에서만 할 수 있습니다!")) ;
		return -1 ;
	}

	if ( sKisaNo.IsEmpty() ) {
		AfxMessageBox( _T("원문 구축된 기사제어번호 설정에서 오류가 발생하였습니다!\r\n기사제어번호가 없습니다.") ) ;
		return -2 ;
	}

	bool bFindinMokcha= TRUE ;
	if ( ! IsThereYonKisaNoinMokcha(m_sFirstMokcha, sKisaNo) )
	{
		bFindinMokcha= FALSE ;
		CString sError ;
		sError= _T("다음 기사제어번호가 목차에 존재하지 않습니다.\r\n") ;
		sError+= sKisaNo ;
		AfxMessageBox(sError) ;
		return -3 ;
	}

	// Array 에 넣어준다.
	m_arrWonmunKisaNo.Add(sKisaNo) ;

	return 0 ;
}

// 목차 편집이 끝난 후 목차 string을 받고 싶을 때
CString CTocEditorDlg::GetFinalToc()
{
	CString sToc ;
	m_TocEdit.GetWindowText(sToc) ;

	return sToc ;
}

// line number 를 넘겨주면 그 라인의 기사제어번호를 찾아 넘겨주는 함수
CString CTocEditorDlg::GetKisaNofromMokchaLine(INT iLine)
{
	if( iLine<= 0 ) {
		CString sError ;
		sError.Format( _T("부적당한 Line 값(%d)입니다.") , iLine) ;
		AfxMessageBox( sError ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sOneLine ;
	TCHAR szLine[MAX_LINE] ;
	INT iLineLength= m_TocEdit.GetLine( iLine-1, szLine ) ;
	szLine[iLineLength]= '\0' ;
	sOneLine= szLine ;

	INT nFindStart, nFindEnd ;
	nFindStart= nFindEnd= -1 ;
	CString sStartTag ;
	sStartTag= _START_CHOROK_KEY_ ;
	nFindStart= sOneLine.Find(sStartTag) ;
	if ( nFindStart<0 )
	{
		return _T("") ;
	}

	nFindEnd=  sOneLine.Find(_END_CHOROK_KEY_, nFindStart ) ;
	if ( nFindEnd<0 )
	{
		return _T("") ;
	}

	INT nStart= nFindStart+sStartTag.GetLength() ;
	CString sKey ;
	sKey= sOneLine.Mid(nStart, nFindEnd-nStart) ;

	return sKey ;
}

INT CTocEditorDlg::GetCurrentLine()
{
	return m_icurLine ;
}

VOID CTocEditorDlg::ShowViewer(BOOL bPrev/*=FALSE*/, BOOL bNext/*=FALSE*/)
{
	CString sText ;
	m_TocEdit.GetWindowText( sText ) ;

	CTocViewerDlg dlg(this) ;
	if ( m_nIsKindOf==_YON_ )
	{
		dlg.SetTocViewer( sText, TRUE, FALSE ) ;

		// 기사라인 번호를 넘겨주기 위해...
		// View 다이얼로그가 떠 있는 동안은 목차는 편집되면 안된다.
		INT nSize= m_aChorokStru.GetSize() ;
		CString sKisaTag ;
		CString sFront ;
		for ( INT i=0 ; i<nSize ; i++ )
		{
			tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
			if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ||		// 초록이 있는 기사의 라인만 넘겨주도록 하자.
				tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ||
				(tOneChorok.sChorokText).IsEmpty() )
				continue ;

			sKisaTag= _START_CHOROK_KEY_ ;
			sKisaTag+= tOneChorok.sKey ;
			sKisaTag+= _END_CHOROK_KEY_ ;

			INT nFind= sText.Find(sKisaTag) ;
			if ( nFind<0 ) {	//에러발생
				AfxMessageBox( _T("기사태그 ") + sKisaTag + _T(" 이 목차에 존재하지 않습니다.")) ;
				continue ;
			}

			sFront= sText.Mid(0, nFind) ;
			INT nLine= sFront.Replace(_T("\r\n"), _T("\r\n"));
			dlg.SetChorokLine(nLine+1) ;
		}
	}
	else		// 단행본일 경우
		dlg.SetTocViewer( sText ) ;

	dlg.EnablePrevNext(bPrev, bNext) ;
	dlg.DoModal() ;
}

// 연간물에서 초록을 새롭게 작성할때는 새로운 기사제어번호를 생성시켜주어야 한다.
CString CTocEditorDlg::GetNewKisaNo()
{
	return _T("");
	TCHAR szRecKey[64] ;
	wmemset( szRecKey, '\0', sizeof(szRecKey) ) ;
	if ( m_pYonKeyFunc==NULL ) {
		AfxMessageBox( _T("키 값을 받아오는 함수 설정이 되지 않았습니다."), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}
	else if ( (m_pParentWnd->*m_pYonKeyFunc)(szRecKey)<0 ) {
		AfxMessageBox( _T("키 값을 부여받는데 실패했습니다!\r\nMakePrimaryKey 함수 에러!"), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNo ;
	sKisaNo= szRecKey ;

	return sKisaNo ;

}

CString CTocEditorDlg::GetYonChorok(INT iLine)
{
	if ( m_nIsKindOf!=_YON_ ) {	// 단행물일때는 에러를 리턴한다.
		AfxMessageBox( _T("현재 목차는 단행물로 설정되어있습니다.\r\n설정을 확인해 주십시오.") ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNo= GetKisaNofromMokchaLine(iLine) ;

	if ( sKisaNo==_CHOROK_ERROR_STRING_ ) {
		AfxMessageBox( _T("초록 가져오기 실패!"), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	if ( sKisaNo.IsEmpty() )
	{

		return _T("") ;		// 초록이 없기 때문에 Empty를 리턴해준다.

	}
	else	// 기존 초록을 편집하는 경우
	{
		for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
			tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
			if( sKisaNo==tOneChorok.sKey ) {
				return tOneChorok.sChorokText ;
			}
		}

//		CString sError ;
//		sError=	_T("기사제어번호 ") + sKisaNo + _T(" 과 연결된 초록이 없습니다.") ;
//		AfxMessageBox( _T("초록 가져오기 실패!"), MB_ICONERROR ) ;
//		return _CHOROK_ERROR_STRING_ ;
		return _T("") ;		// 맨처음 목차가 불려졌을때 기사제어번호는 있는데 초록은 없는 경우이다.
	}
}

CString CTocEditorDlg::GetYonKisaNo(INT iLine, bool bNew/*=TRUE*/)
{
	if ( m_nIsKindOf!=_YON_ ) {	// 단행물일때는 에러를 리턴한다.
		AfxMessageBox( _T("현재 목차는 단행물로 설정되어있습니다.\r\n설정을 확인해 주십시오.") ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNo= GetKisaNofromMokchaLine(iLine) ;

	if ( sKisaNo==_CHOROK_ERROR_STRING_ ) {	// 에러!
		AfxMessageBox( _T("기사제어번호 가져오기 실패!"), MB_ICONERROR ) ;
		return _CHOROK_ERROR_STRING_ ;
	}
	else if ( sKisaNo.IsEmpty() ) {		// 기사제어번호가 없는 라인
		if ( bNew ) {
			CString sNewKey= GetNewKisaNo() ;
			if( sNewKey==_CHOROK_ERROR_STRING_ ) {
				AfxMessageBox(_T("기사제어번호 가져오기 실패!"), MB_ICONERROR );
				return _CHOROK_ERROR_STRING_;
			}
			else	return sNewKey ;	// 새로운 기사제어번호를 얻어와서 넘겨준다.
		}
		else {
			return sKisaNo ;		// Empty 값을 넘겨주자.
		}
	}
	else	return sKisaNo ;		// 기사제어번호를 찾았다!

}

INT CTocEditorDlg::ReflectYonChorok2Stru(CString sChorokText, CString sKisaNo)
{
/*
	if ( ! IsYonKisaNoFormal(sKisaNo) ) {
		CString sError ;
		sError= _T("다음 기사제어번호를 연간물 초록에 반영시키지 못했습니다!\r\n") ;
		sError+= sKisaNo ;
		AfxMessageBox( sError ) ;
		return -1 ;
	}
*/
	BOOL bFindKey= FALSE ;
	for ( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;

		if ( sKisaNo==tOneChorok.sKey )	{
			bFindKey= TRUE ;

			tCHOROK tNewChorok ;
			tNewChorok.sKey= sKisaNo ;
			tNewChorok.sKisaStatus= tOneChorok.sKisaStatus ;
			tNewChorok.sChorokText= sChorokText ;

			if ( sChorokText.IsEmpty() ) {
				if ( ! (tOneChorok.sChorokText).IsEmpty() && tOneChorok.sChorokStatus!=_CHOROK_STATUS_NEW_ )
					tNewChorok.sChorokStatus=_CHOROK_STATUS_DELETE_ ;
			} else {
				if ( tOneChorok.sChorokText==sChorokText )
					tNewChorok.sChorokStatus=_T("") ;
				else {
					if ( tOneChorok.sChorokStatus==_T("") ) {
						if ( (tOneChorok.sChorokText).IsEmpty() )
							tNewChorok.sChorokStatus=_CHOROK_STATUS_NEW_ ;
						else
							tNewChorok.sChorokStatus=_CHOROK_STATUS_MODIFY_ ;
					}
					else if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_NEW_ )
						tNewChorok.sChorokStatus=_CHOROK_STATUS_NEW_ ;
					else if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_MODIFY_ )
						tNewChorok.sChorokStatus=_CHOROK_STATUS_MODIFY_ ;
					else if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ )
						tNewChorok.sChorokStatus=_CHOROK_STATUS_MODIFY_ ;
				}
			}

			m_aChorokStru.RemoveAt(i) ;		// 우선 지우고
			m_aChorokStru.InsertAt(i, tNewChorok) ;		// 그 위치에 새롭게 넣어준다.
			break ;
		}

	}

	if( ! bFindKey ) {	// 기사제어번호가 새로작성됨
		if( ! IsYonKisaNoUnique(sKisaNo) ) {
			CString sError ;
			sError= _T("다음 기사제어번호를 연간물 초록에 반영시키지 못했습니다.\r\n") ;
			sError+= sKisaNo ;
			AfxMessageBox( sError ) ;
			return -2 ;
		}

		tCHOROK tOneChorok ;
		tOneChorok.sKey= sKisaNo;
		tOneChorok.sKisaStatus= _CHOROK_STATUS_NEW_ ;		// 디폴트 - 새로작성 플래그
		// added 2002.11.1 - 기사에
		INT nSize= g_arrAlreadyExistKisaNo.GetSize();
		for( INT idx=0 ; idx<nSize ; idx++ )
		{
			if ( sKisaNo==g_arrAlreadyExistKisaNo.GetAt(idx) )
			{
				(tOneChorok.sKisaStatus).Empty();		// 처음에 존재하는 기사는 새로작성된 것이 아님
				break;
			}
		}
		tOneChorok.sChorokText= sChorokText;

		if ( sChorokText.IsEmpty() )
			tOneChorok.sChorokStatus= _T("") ;
		else
			tOneChorok.sChorokStatus= _CHOROK_STATUS_NEW_ ;

		m_aChorokStru.Add(tOneChorok);	// 추가해준다.
	}

	return 0 ;
}

CString CTocEditorDlg::WriteYonKisaNo2Mokcha(INT nLine, CString sKisaNo, CString sMokcha)
{
	INT nTotalLine= sMokcha.Replace( _T("\r\n"), _T("\r\n") ) ;
	nTotalLine++ ;

	if ( nTotalLine<nLine ) {
		AfxMessageBox( _T("Line 설정이 잘못되었습니다.\r\n연간물 목차에 새로운 기사제어번호를 넣어주지 못했습니다.") ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisaNoinMokcha ;
	sKisaNoinMokcha= _START_CHOROK_KEY_ + sKisaNo + _END_CHOROK_KEY_ ;	// 목차에 들어갈 기사제어번호 태그 포함

	INT nStart= 0 ;
	INT nLineCnt= 0 ;
	BOOL bSuccess= FALSE ;
	for (INT i=0 ; i<nTotalLine ; i++) {
		nStart= sMokcha.Find(_T("\r\n"), nStart) ;
		nLineCnt++ ;
		if( nLineCnt == nLine ) {
			bSuccess=TRUE ;
			if ( nStart<0 )	sMokcha+= sKisaNoinMokcha ;
			else			sMokcha.Insert(nStart, sKisaNoinMokcha) ;
			break ;
		}
		nStart= nStart+2 ;
	}

	if( !bSuccess ) { 
		CString sError ;
		sError.Format( _T("연간물 목차의 %d 라인에 기사제어번호 %s 를 넣어주지 못했습니다!") , nLine, sKisaNo) ;
		AfxMessageBox( sError ) ;
		return _CHOROK_ERROR_STRING_ ;
	}
	else 
		return sMokcha ;
}

INT CTocEditorDlg::WriteYonNewKisaNo2Mokcha(INT nLine, CString sKisaNo)
{
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;
	INT nTotalLine= sMokcha.Replace( _T("\r\n"), _T("\r\n") ) ;
	nTotalLine++ ;

	if ( nTotalLine<nLine ) {
		AfxMessageBox( _T("Line 설정이 잘못되었습니다.\r\n목차에 새로운 기사제어번호를 넣어주지 못했습니다.") ) ;
		return -1 ;
	}

	sKisaNo.TrimRight() ;
	sKisaNo.TrimLeft() ;

	CString sKey ;
	sKey= _START_CHOROK_KEY_ + sKisaNo + _END_CHOROK_KEY_ ;

	INT nStart= 0 ;
	INT nPrev= 0 ;
	INT nLineCnt= 0 ;
	BOOL bSuccess= FALSE ;
	for (INT i=0 ; i<nTotalLine ; i++) {
		nPrev= nStart ;
		nStart= sMokcha.Find(_T("\r\n"), nStart) ;
		nLineCnt++ ;
		if( nLineCnt == nLine ) {
			bSuccess= TRUE ;
			if ( nStart<0 ) sMokcha+= sKey ;		// 마지막 라인
			else			sMokcha.Insert(nStart, sKey) ;
			break ;
		}
		nStart= nStart+2 ;
	}

	if( !bSuccess ) { 
		CString sError ;
		sError.Format( _T("연간물 목차의 %d 라인에 기사제어번호 %s 를 넣어주지 못했습니다!") , nLine, sKisaNo) ;
		AfxMessageBox( sError ) ;
		return -3 ;
	}

	m_TocEdit.GetSel(m_nStartChar, m_nEndChar);
	m_TocEdit.SetWindowText(sMokcha) ;
	m_TocEdit.SetSel(m_nStartChar, m_nEndChar);

	return 0 ;
}

INT CTocEditorDlg::DeleteYonKisaNo(CString sKisaNo)
{
	if ( m_nIsKindOf!=_YON_ ) {	// 단행물일때는 에러를 리턴한다.
		AfxMessageBox( _T("현재 목차는 단행물로 설정되어있습니다.\r\n설정을 확인해 주십시오.") ) ;
		return -1 ;
	}

	INT retval = 0 ;
	// 우선 구조체에서 초록정보를 없애주자.
	BOOL bFindKey= FALSE ;
	for ( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if( sKisaNo==tOneChorok.sKey )	{
			bFindKey= TRUE ;

			if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_NEW_ ) {	// 새로 작성하고 지우는 경우일때는 그냥 지우기만 해준다.
				m_aChorokStru.RemoveAt(i) ;		// 지우기만 하고 insert 해 주지 않는다.
			} else if ( tOneChorok.sKisaStatus==_T("") ) {
				tCHOROK tNewChorok ;
				tNewChorok.sKey= sKisaNo ;
				tNewChorok.sChorokText= _T("") ;
				tNewChorok.sKisaStatus= _CHOROK_STATUS_DELETE_ ;
				if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ) {
					tNewChorok.sChorokStatus=_CHOROK_STATUS_DELETE_ ;
				} else {
					if ( ! (tOneChorok.sChorokText).IsEmpty() )
					{
						if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_NEW_ )
							(tNewChorok.sChorokStatus).Empty() ;
						else
							tNewChorok.sChorokStatus=_CHOROK_STATUS_DELETE_ ;
					}
				}

				m_aChorokStru.RemoveAt(i) ;		// 우선 지우고
				m_aChorokStru.InsertAt(i, tNewChorok) ;

			} else if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ) {	// 이미 삭제되어 있는 경우
				CString sError ;
				sError= _T("기사제어번호 ") + sKisaNo + _T(" 은 이미 삭제되어 있습니다!") ;
				AfxMessageBox(sError) ;
			}

			break ;
		}
	}

	if ( ! bFindKey ) {
		// 기사제어번호가 구조체에 없다.
		CString sError ;
		sError= _T("기사제어번호 ") + sKisaNo + _T(" 이 존재하지 않습니다.") ;
		AfxMessageBox(sError) ;
		retval= -1 ;
	}

	return retval ;
}

VOID CTocEditorDlg::Clear()
{
	m_sBackupTocEdit.Empty() ;
	m_sRedoTocEdit.Empty() ;

	m_aChorokStru.RemoveAll() ;
	m_aBackupChorokStru.RemoveAll() ;
	m_aRedoChorokStru.RemoveAll() ;

	if ( m_TocEdit.m_hWnd != NULL )
	{
		m_icurLine = 1 ;
		m_LineView.SetWindowText(_T("1"));

		// m_TocEdit 의 모든 내용을 지워준다.
		m_TocEdit.SetSel(0,-1);
		m_TocEdit.Clear() ;
	}

}

// 키가 형식에 맞는지 체크!		-> 제어번호_ID
//DEL BOOL CTocEditorDlg::IsYonKisaNoFormal(CString sKisaNo)
//DEL {
//DEL /*
//DEL 	if ( m_sControlNo.IsEmpty() ) {
//DEL 		AfxMessageBox(_T("제어번호가 설정되어 있지 않습니다!"), MB_ICONERROR|MB_OK) ;
//DEL 		return FALSE ;
//DEL 	}
//DEL */
//DEL 	TCHAR szKisaNo[64] ;
//DEL 	_tcscpy( szKisaNo, sKisaNo ) ;
//DEL 	TCHAR seps[] = _T("_");
//DEL 	TCHAR *token= NULL;
//DEL 	INT nCnt=0 ;
//DEL 
//DEL 	token = _tcstok( szKisaNo, seps );
//DEL 	while( token != NULL )
//DEL 	{
//DEL 		nCnt++ ;
//DEL 
//DEL 		/* While there are tokens in _T("string") */
//DEL 		if ( (nCnt>2) || ( nCnt==1 && m_sControlNo.Compare(token))) {
//DEL 			CString sError ;
//DEL 			sError= _T("형식에 맞지 않는 키가 사용되었습니다.\r\n") ;
//DEL 			sError+= sKisaNo ;
//DEL 			AfxMessageBox( sError ) ;
//DEL 			return FALSE ;
//DEL 		}
//DEL 
//DEL 		/* Get next token: */
//DEL 		token = _tcstok( NULL, seps );
//DEL 	}
//DEL 
//DEL 	return TRUE ;
//DEL }

// 키가 목차에도 존재하는지 여부 체크
BOOL CTocEditorDlg::IsThereYonKisaNoinMokcha(CString sMokcha, CString sKisaNo)
{
	CString sKey ;
	sKey= _START_CHOROK_KEY_ + sKisaNo + _END_CHOROK_KEY_ ;

	INT nCnt= sMokcha.Replace( sKey, sKey ) ;

	if ( nCnt==0 ) return FALSE ;

	if ( nCnt > 1 ) {		// 두 개이상 검출된 경우
		CString sMsg ;
		sMsg= _T("기사제어번호 ") + sKisaNo + _T(" 이 목차에 두개 이상 존재합니다.") ;
		sMsg+= _T("\r\n위쪽 기사제어번호만 유효합니다.") ;
		AfxMessageBox( sMsg ) ;
	}

	return TRUE ;
}

// 키가 유니크 한지 체크!
BOOL CTocEditorDlg::IsYonKisaNoUnique(CString sKisaNo)
{
	return TRUE;
	if( sKisaNo.IsEmpty() )
		return FALSE ;

	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if( sKisaNo==tOneChorok.sKey ) {
			CString sError ;
			sError= _T("다음 기사제어번호가 Unique하지 않습니다.\r\n") ;
			sError+= sKisaNo ;
			AfxMessageBox(sError) ;
			return FALSE ;
		}
	}

	return TRUE ;
}

// 키가 새로작성된 키인지 확인해준다.
BOOL CTocEditorDlg::IsYonKisaNoRight(CString sKisaNo)
{
	if( sKisaNo.IsEmpty() )
		return FALSE ;

	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if( sKisaNo==tOneChorok.sKey && tOneChorok.sKisaStatus!=_CHOROK_STATUS_DELETE_) {
			return TRUE ;		// 구조체상에 존재하는 키
		}
	}

	return FALSE ;
}

CString CTocEditorDlg::GetOneLineString(CString sMokcha, INT nLine)
{
	CString sOneLine ;

	TCHAR string[MAX_LINE] ;
	_tcscpy( string, sMokcha ) ;
	TCHAR seps[] = _T("\r\n");
	INT nCnt= 1 ;
	TCHAR *token= NULL;

	token = _tcstok( string, seps );
	while( token != NULL )
	{
		if( nCnt == nLine ) {
			sOneLine= token ;
			return sOneLine ;
		}

		/* Get next token: */
		token = _tcstok( NULL, seps );
		nCnt++ ;
	}

	return _T("") ;
}

INT CTocEditorDlg::GetOneLineString(INT nLine, CString& sOneLine)
{
	sOneLine= _T("") ;

	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;

	INT nTotalLine=	sMokcha.Replace( _T("\r\n") , _T("\r\n") ) ;
	nTotalLine++ ;

	if ( nTotalLine < nLine ) {
		AfxMessageBox( _T("라인 에러!"), MB_ICONERROR ) ;
		return -1 ;
	}

	INT nStart=0 ;
	INT nFind=-1 ;
	for ( INT idx=1 ; idx<nLine ; idx++ )
	{
		nFind= sMokcha.Find( _T("\r\n") , nStart ) ;
		if ( nFind<0 ) {
			AfxMessageBox( _T("라인 에러!"), MB_ICONERROR ) ;
			return -2 ;
		}
		nStart= nFind + 2 ;
	}

	if ( nTotalLine==nLine )		// 마지막 라인
	{
		sOneLine= sMokcha.Mid(nStart) ;
		return 0 ;
	}

	nFind= sMokcha.Find( _T("\r\n") , nStart ) ;
	if ( nFind<0 ) {
		AfxMessageBox( _T("라인 에러!"), MB_ICONERROR ) ;
		return -3 ;
	}

	sOneLine= sMokcha.Mid(nStart, nFind-nStart) ;
	return 0 ;
}

// 목차에 옆의 기사제어번호들이 잘 붙어 있는지 구조체와 검사해 준다.
// 검사 기준은 구조체이다.
// 단, 연간물일때만 사용한다.
INT CTocEditorDlg::CheckYonKisaNo(CString sFinalMokcha)
{
	INT nCnt = 0 ;		// 목차에서 기사제어번호가 없는 개수
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		CString sKisaNo ;
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		sKisaNo= _START_CHOROK_KEY_ ;
		sKisaNo+= tOneChorok.sKey ;
		sKisaNo+=_END_CHOROK_KEY_ ;		// 기사제어번호 형식!

		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ) {
			if ( sFinalMokcha.Find( sKisaNo )>=0 ) {
				nCnt++ ;
				CString sError ;
				sError = _T("목차에서 기사제어번호 ") + sKisaNo + _T(" 은 이미 지워진 기사제어번호입니다!") ;
				AfxMessageBox( sError, MB_ICONERROR ) ;
			}
			else
				continue ;
		}

		if ( sFinalMokcha.Find( sKisaNo )<0 ) {
			nCnt++ ;
			CString sError ;
			sError = _T("목차에서 기사제어번호에 대한 정보 ") + sKisaNo + _T(" 이 없습니다.\r\n확인하여 주십시오!") ;
			AfxMessageBox( sError, MB_ICONERROR ) ;
		}
	}

	return nCnt ;
}

INT CTocEditorDlg::GetKindofMokcha()
{
	return m_nIsKindOf ;
}

VOID CTocEditorDlg::GetResultYonChorokSet(CYonChorokSet& rs)
{
	rs.Free() ;
	CString sKisa, sKisaStatus ;

	// 기사를 모두 넣어준다. 아무런 플래그 없이. added 2002.10.31
	// 따라서 목차에 존재하는 기사제어번호 갯수만큼 결과에 넣어주게된다.
	CStringArray sArrKisaNo;
	CString sChorok, sKisaNo;	sKisaNo.Empty();
	BOOL bSkip= FALSE;	BOOL bExist= FALSE;
	if ( GetYonAllKisaNo(sArrKisaNo) )
	{
		return ;
	}
	INT nSize= g_arrAlreadyExistKisaNo.GetSize();
	INT nCurrentKisaCnt= sArrKisaNo.GetSize();
	for ( INT idx=0 ; idx<nSize ; idx++ )
	{
		sKisaNo= g_arrAlreadyExistKisaNo.GetAt(idx);

		bSkip= FALSE;	sChorok.Empty();	sKisaStatus.Empty();
		for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
			tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
			if( sKisaNo==tOneChorok.sKey ) {
				// 상태플래그 값이 empty 인 경우만 넣어준다.
				// 즉, 같은 기사제어번호를 두번 넣지 않기 위함이다.
				// 작업이 이루어진 것들은 밑의 for 문에서 넣어주기 때문이다.
				if ( ! ( (tOneChorok.sChorokStatus).IsEmpty() && (tOneChorok.sKisaStatus).IsEmpty() ) )
				{
					bSkip= TRUE;
				}
				sChorok= tOneChorok.sChorokText ;
				break;
			}
		}
		bExist= FALSE;	sKisaStatus.Empty();	sKisa.Empty();
		for ( INT j=0 ; j<nCurrentKisaCnt ; j++ )	// 기사 delete 추가
		{
			if( sKisaNo== sArrKisaNo.GetAt(j) )
			{
				bExist= TRUE;
			}
		}
		if ( !bExist )	sKisaStatus= _CHOROK_STATUS_DELETE_;
		else			sKisa= GetKisafromKisaNo( sKisaNo ) ;

		if ( !bSkip )	// 상태값이 있는것들은 넘어가자.
			rs.SetChorok(sKisaNo , sChorok, _T(""), sKisaStatus, sKisa ) ;

	}
	sArrKisaNo.RemoveAll();	sArrKisaNo.FreeExtra();



	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		// 위에서 넣어준것들(flag 가 없는것)은 여기서 넣어주면 안된다.
		// added 2002.11.1
		if ( (tOneChorok.sChorokStatus).IsEmpty() && (tOneChorok.sKisaStatus).IsEmpty() )	continue;
		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ )	{	// DELETE 된 KEY
			rs.SetChorok( tOneChorok.sKey, tOneChorok.sChorokText, tOneChorok.sChorokStatus, tOneChorok.sKisaStatus, _T("") ) ;
		}
		else {
			sKisa= GetKisafromKisaNo( tOneChorok.sKey ) ;
			rs.SetChorok( tOneChorok.sKey, tOneChorok.sChorokText, tOneChorok.sChorokStatus, tOneChorok.sKisaStatus, sKisa ) ;
		}
	}

}

CString CTocEditorDlg::GetKisafromKisaNo(CString sKisaNo)
{
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;

	INT nFindKey= sMokcha.Find(sKisaNo) ;
	if ( nFindKey<0 )
	{
		CString sError ;
		sError= _T("기사제어번호 ") + sKisaNo + _T(" 이 목차에 존재하지 않습니다!") ;
		MessageBox( sError, _T("기사 가져오기"), MB_ICONERROR | MB_OK ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	INT curLine = 1;
	for ( INT idx=0 ; idx<nFindKey ; idx++ )
	{
		if( sMokcha.GetAt(idx)=='\n' ) curLine++ ;
	}

	CString sOneLine ;
	TCHAR szLine[MAX_LINE] ;
	INT iLineLength = m_TocEdit.GetLine( curLine-1, szLine ) ;
	sOneLine= szLine ;

	if ( sOneLine.Find(sKisaNo)<0 )
	{
		CString sError ;
		sError= _T("기사제어번호 ") + sKisaNo + _T(" 이 목차에 존재하지 않습니다!") ;
		MessageBox( sError, _T("기사 가져오기"), MB_ICONERROR | MB_OK ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	INT nFindStart= sOneLine.Find(_START_CHOROK_KEY_) ;
	if ( nFindStart<0 )
	{
		CString sError ;
		sError= _T("기사제어번호 ") + sKisaNo + _T(" 이 목차에 존재하지 않습니다!") ;
		MessageBox( sError, _T("기사 가져오기"), MB_ICONERROR | MB_OK ) ;
		return _CHOROK_ERROR_STRING_ ;
	}

	CString sKisa ;
	sKisa= sOneLine.Mid(0,nFindStart) ;

	return sKisa ;
}

// 리치에디트에서 오른쪽 버튼 팝업메뉴의 초록 편집(단행본)
VOID CTocEditorDlg::OnEditDanChorok()
{
	if ( m_pParentWnd==NULL ) return ;
	m_pParentWnd->SendMessage(MSG_EDIT_DAN_CHOROK) ;
}

// 리치에디트에서 오른쪽 버튼 팝업메뉴의 초록 편집(연간물)
VOID CTocEditorDlg::OnEditYonChorok()
{
//	if ( m_pParentWnd==NULL ) return ;

	INT nLine= GetCurrentLine() ;

	CString sOldChorokText, sNewChorokText, sKisaNo ;
	sKisaNo= GetYonKisaNo(nLine, false) ;
	if ( sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ )
		return ;

	sOldChorokText= GetYonChorok(nLine) ;
	if ( sOldChorokText==_CHOROK_ERROR_STRING_ )
		return ;
		
	CChorokEditorDlg dlg(sOldChorokText, sKisaNo, GetKisafromKisaNo(sKisaNo), nLine, this) ;
	if ( IDCANCEL==dlg.DoModal() )
		return ;

	// 이전, 다음을 선택했을 경우 기사제어번호를 변경시켜주어야 함.
	// modified 2002.11.1
	sKisaNo= GetYonKisaNo(m_icurLine, false) ;
	if ( sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ )
		return ;

	// 초록 편집이 끝났을 때 초록 내용을 받아온다.
	sNewChorokText= dlg.GetChorokText()  ;

	if ( sNewChorokText==sOldChorokText ) return ;

	// 초록을 반영시켜준다. 수정
	TocEditorBackup() ;

	if ( ReflectYonChorok2Stru(sNewChorokText, sKisaNo) ) {
		AfxMessageBox( _T("에러 발생!") );
		return ;
	}
}
/*
INT CTocEditorDlg::ShowYonMokcha(CString sYonMokcha)
{
	if ( m_nIsKindOf!=_YON_ || m_TocEdit.m_hWnd==NULL ) return -1 ;

	m_TocEdit.SetWindowText(sYonMokcha) ;

	return 0 ;
}
*/
INT CTocEditorDlg::KisaNoDeleteMessage(CString sID)
{
	if ( sID.IsEmpty() )	return IDYES ;

	CString sKisaNo ;
	sKisaNo= sID ;

	CString sMsg ;
	sMsg= _T("기사제어번호 ") + sKisaNo + _T(" 을 지우겠습니까?\r\n만약 지운다면 초록을 다시 복구할수 없습니다.") ;
	INT retval= AfxMessageBox( sMsg, MB_YESNO, MB_ICONWARNING );

	if ( retval==IDYES )
	{	// 여기서 구조체에 저장되어 있는 키 정보까지도 'D' 로 setting 해 주자.
		TocEditorBackup() ;

		INT ret= DeleteYonKisaNo(sKisaNo) ;
		if ( ret<0 ) {
			sMsg= _T("기사제어번호 ") + sKisaNo + _T(" 을 지우지 못했습니다!") ;
			AfxMessageBox(sMsg) ;
			return IDNO ;
		}
	}
	else
	{
		return IDNO ;
	}

	return IDYES ;
}

VOID CTocEditorDlg::TocNewEdit()
{
//	if(m_nIsKindOf==_YON_ )
//		TocNewEdit(csId, csKey, csSertitle, csTitle, csIssue, csDate, csCall, csCount);
//	else
//		TocNewEdit(csId, csCall, csCount, csDate, csPub, csSertitle, csTitle, _T(""));
	TocNewEdit(csstrTag);
}

VOID CTocEditorDlg::TocNewEdit(CString *csstrTag)
{

	CString sMokcha ;
	m_TocEdit.GetWindowText( sMokcha ) ;

	if ( ! sMokcha.IsEmpty() ) {
		INT retval = -1 ;
		if( retval = AfxMessageBox( _T("현재 편집중인 목차를 저장없이 닫겠습니다.") , MB_YESNO ) ) {
			switch( retval ) {
			case IDYES :
				break ;
			case IDNO :
				return ;
			}
		}
	}

	// 변수 초기화.
	m_sBackupTocEdit.Empty() ;
	m_sRedoTocEdit.Empty() ;
	m_aBackupChorokStru.RemoveAll() ;
	m_aRedoChorokStru.RemoveAll() ;
	if ( m_TocEdit.m_hWnd != NULL )
	{
		m_icurLine = 1 ;
		m_LineView.SetWindowText(_T("1"));

		// m_TocEdit 의 모든 내용을 지워준다.
		m_TocEdit.SetSel(0,-1);
		m_TocEdit.Clear() ;
	}
	// 모든 기사를 delete 플래그로 해주자.
	// DeleteYonKisaNo 함수는 경우에 따라 초록 플래그도 자동적으로 업데이트시켜준다.
	for ( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( tOneChorok.sKisaStatus!=_CHOROK_STATUS_DELETE_ )
			DeleteYonKisaNo(tOneChorok.sKey) ;
	}


	m_LineView.SetWindowText(_T("1"));	// 라인 표시 - 1
	m_SyntaxCheckDlg.DeleteAllItems() ;
	m_SyntaxCheckDlg.ShowWindow(SW_HIDE) ;	// 오류사항 다이얼로그 숨기기

	m_sBackupTocEdit.Empty() ;
	m_sRedoTocEdit.Empty() ;
	m_aRedoChorokStru.RemoveAll() ;

	CString sTocEditText ;
	CBibilographyDlg dlg ;
	dlg.m_nIsKind = m_nIsKindOf;

	dlg.m_str1 = csstrTag[0];
	dlg.m_str2 = csstrTag[1];
	dlg.m_str3 = csstrTag[2];
	dlg.m_str4 = csstrTag[3];
	dlg.m_str5 = csstrTag[4];
	dlg.m_str6 = csstrTag[5];
	dlg.m_str7 = csstrTag[6];
	dlg.m_str8 = csstrTag[7];

	if( dlg.DoModal() != IDOK ) return ;
	sTocEditText = dlg.m_strBibilography ;

/*
	if ( m_nIsKindOf==_YON_ )
	{
		CYonSujiDlg dlg ;
		if( dlg.DoModal() != IDOK ) return ;
		sTocEditText = dlg.m_strBibilography ;
	}
	else 
	{
		CBibilographyDlg dlg ;
		if( dlg.DoModal() != IDOK ) return ;
		sTocEditText = dlg.m_strBibilography ;
	}
*/
	// 백업 해준다.
	TocEditorBackup() ;

	m_TocEdit.SetWindowText(sTocEditText) ;

	DisplayCurrentLineNumber() ;

	// 목차 다이얼로그
	CBodyDlg Bodydlg ;
	if( Bodydlg.DoModal() != IDOK ) return ;

	sTocEditText += Bodydlg.m_strResult ;
	sTocEditText.TrimRight( _T("\r\n") ) ;
	m_TocEdit.SetWindowText(sTocEditText);

	INT nLineIdx= 0 ;
/*	if ( m_nIsKindOf==_YON_ )
	{
		nLineIdx= m_TocEdit.LineIndex(9);

		SetKisaNo2AllKisa() ;		// body 의 빈공간에 목차제어번호를 넣어주자.
	}
	else
	{
		nLineIdx= m_TocEdit.LineIndex(8);
	}
*/
	m_TocEdit.SetSel(nLineIdx,nLineIdx) ;	// body 내용부분에 커서를 갖다주기 위해
	m_TocEdit.SetFocus() ;

	DisplayCurrentLineNumber() ;
}

INT CTocEditorDlg::ExecuteSoosikEditor()
{
	m_cSoosikEditor.RemoveSharedMemoryPool() ;

	INT retval= m_cSoosikEditor.ExecuteSoosikEditor(m_hWnd) ;	// 핸들을 넘겨주자.

	return retval ;
}

INT CTocEditorDlg::ReadFromSoosikEditor()
{
	CString sRead ;
	sRead= m_cSoosikEditor.ReadFromSoosikEditor() ;

	if ( sRead.IsEmpty() )
		return -1 ;

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;
	TocEditorBackup() ;

	sMokcha.Insert( m_nStartChar, sRead ) ;

	m_TocEdit.SetWindowText(sMokcha) ;

	INT nLength= sRead.GetLength() ;
	m_TocEdit.SetSel( m_nStartChar+nLength , m_nStartChar+nLength ) ;

	return 0 ;
}

VOID CTocEditorDlg::OnReadEcoMath()
{
	ReadFromSoosikEditor() ;
}

INT CTocEditorDlg::SetKisaNo2AllKisa(BOOL bCheckSyntax/*=FALSE*/)
{
	if ( bCheckSyntax ) {
		if ( CheckSyntax()<0 )
		{
			AfxMessageBox( _T("문법에 에러가 있습니다.\r\n먼저 문법에러 사항을 바로잡아주십시오."), MB_ICONWARNING) ;
			return 0 ;
		}
	}

	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;
	CString sKisaNo ;
	CString sOneKisa ;

	INT nStart= 0 ;
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	INT nStartLine= 1 ;

	do {
		nFindStart= sMokcha.Find( BODY_START_TAG , nStart ) ;
		if ( nFindStart>=0 )
		{
			nStart= nFindStart + BODY_START_LENGTH ;
			nFindEnd= sMokcha.Find( BODY_END_TAG , nStart ) ;
			if ( nFindEnd>=0 )		// <body>와 </body>를 찾았다.
			{
				INT nFindReturn= sMokcha.Find( _T("\r\n") , nFindStart ) ;
				if ( nFindReturn>=0 )
				{
					CString sTmp ;
					sTmp= sMokcha.Mid(0, nFindReturn+2 ) ;
					nStartLine= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					nStartLine++ ;	// body contents 가 시작되는 라인

					sTmp= sMokcha.Mid(nFindReturn+2, nFindEnd-nFindReturn-2 ) ;
					INT nReturnCnt= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					for ( INT i=0 ; i<nReturnCnt ; i++ )
					{
						sKisaNo= GetYonKisaNo(nStartLine, false) ;
						if ( sKisaNo==_CHOROK_ERROR_STRING_ )	continue ;
						if ( sKisaNo.IsEmpty() )
						{
							sKisaNo= GetNewKisaNo() ;
							if ( sKisaNo==_CHOROK_ERROR_STRING_ )	return -1 ;
//							if ( GetOneLineString(nStartLine, sOneKisa)<0 )	continue ;
//							if ( ! sOneKisa.IsEmpty() )	{	// 기사가 없을때는 기사제어번호를 추가해주지 말도록 하자!
								WriteYonNewKisaNo2Mokcha( nStartLine, sKisaNo ) ;
								ReflectYonChorok2Stru(_T(""), sKisaNo) ;
//							}
						}
						nStartLine++ ;
					}

					nStart= nFindEnd + BODY_START_LENGTH + 1 ;
				}
			}
		}

	} while ( nFindStart>=0 && nFindEnd>=0 ) ;


	return 0 ;
}

INT CTocEditorDlg::CheckKisaNofromAllKisa(CString sMokcha)
{
	CString sKisaNo ;
	CString sOneKisa ;

	INT nCnt= 0 ;
	INT nStart= 0 ;
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	INT nStartLine= 1 ;

	do {
		nFindStart= sMokcha.Find( BODY_START_TAG , nStart ) ;
		if ( nFindStart>=0 )
		{
			nStart= nFindStart + BODY_START_LENGTH ;
			nFindEnd= sMokcha.Find( BODY_END_TAG , nStart ) ;
			if ( nFindEnd>=0 )		// <body>와 </body>를 찾았다.
			{
				INT nFindReturn= sMokcha.Find( _T("\r\n") , nFindStart ) ;
				if ( nFindReturn>=0 )
				{
					CString sTmp ;
					sTmp= sMokcha.Mid(0, nFindReturn+2 ) ;
					nStartLine= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					nStartLine++ ;	// body contents 가 시작되는 라인

					sTmp= sMokcha.Mid(nFindReturn+2, nFindEnd-nFindReturn-2 ) ;
					INT nReturnCnt= sTmp.Replace( _T("\r\n") , _T("\r\n") ) ;
					for ( INT i=0 ; i<nReturnCnt ; i++ )
					{
						sKisaNo= GetYonKisaNo(nStartLine, false) ;
						if ( sKisaNo==_CHOROK_ERROR_STRING_ )	continue ;
						if ( sKisaNo.IsEmpty() )
						{
							nCnt++ ;
						}
						nStartLine++ ;
					}

					nStart= nFindEnd + BODY_START_LENGTH + 1 ;
				}
			}
		}

	} while ( nFindStart>=0 && nFindEnd>=0 ) ;


	return nCnt ;		// body 안의 기사 중에 기사제어번호가 없는 개수
}

BOOL CTocEditorDlg::IsInBody()
{
	CString sMokcha ;
	m_TocEdit.GetWindowText(sMokcha) ;

	m_TocEdit.GetSel( m_nStartChar, m_nEndChar ) ;

	INT nFindStart= 0 ;
	INT nFindEnd= 0 ;
	INT nFindIdx= -1 ;
	CString sTmp ;
	sTmp= sMokcha.Mid(0, m_nStartChar) ;	// 앞쪽

	do {
		nFindIdx= sTmp.Find(BODY_START_TAG, nFindStart) ;
		if ( nFindIdx!=-1 )	nFindStart= nFindIdx+BODY_START_LENGTH ;
	} while ( nFindIdx!=-1 ) ;

	do {
		nFindIdx= sTmp.Find(BODY_END_TAG, nFindEnd) ;
		if ( nFindIdx!=-1 )	nFindEnd= nFindIdx+BODY_START_LENGTH+1 ;
	} while ( nFindIdx!=-1 ) ;

	if ( nFindStart<=nFindEnd )	return FALSE ;

	sTmp= sMokcha.Mid(m_nStartChar) ;		// 뒤쪽
	nFindStart= sTmp.Find(BODY_START_TAG) ;
	nFindEnd= sTmp.Find(BODY_END_TAG) ;

	if ( nFindStart==-1 && nFindEnd!=-1 )	return TRUE ;
	if ( nFindStart<=nFindEnd )				return FALSE ;

	return TRUE ;
}

BOOL CTocEditorDlg::ChorokExist(CString sKisaNo)
{
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( tOneChorok.sKey==sKisaNo ) {
			if ( (tOneChorok.sChorokText).IsEmpty() )
				return FALSE ;
			else
				return TRUE ;
		}
	}

	return FALSE ;
}

VOID CTocEditorDlg::InitYonStatus()
{
	InitYonKisaStatus() ;
	InitYonChorokStatus() ;
}

VOID CTocEditorDlg::InitYonKisaStatus()
{
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( (tOneChorok.sKisaStatus).IsEmpty() ) continue ;

		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ) {
			m_aChorokStru.RemoveAt(i) ;		// 기사가 delete 되면 레코드를 지운다.
			i-- ;
			continue ;
		}

		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_NEW_ || 
			tOneChorok.sKisaStatus==_CHOROK_STATUS_MODIFY_ )
		{	(tOneChorok.sKisaStatus).Empty() ;	}
		else
		{	AfxMessageBox( _T("기사 상태정보에 비정상적인 값이 들어있습니다!") , MB_ICONERROR) ;	}
	}
}

VOID CTocEditorDlg::InitYonChorokStatus()
{
	for( INT i=0 ; i<m_aChorokStru.GetSize() ; i++ ) {
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( (tOneChorok.sChorokStatus).IsEmpty() )	continue ;

		if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ) {
			m_aChorokStru.RemoveAt(i) ;		// 지운다.
			i-- ;
			continue ;
		}

		if ( tOneChorok.sChorokStatus==_CHOROK_STATUS_NEW_ || 
			tOneChorok.sChorokStatus==_CHOROK_STATUS_MODIFY_ || 
			tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ )
			(tOneChorok.sChorokStatus).Empty() ;
		else
		{	AfxMessageBox( _T("초록 상태정보에 비정상적인 값이 들어있습니다!") , MB_ICONERROR) ;	}
	}
}

VOID CTocEditorDlg::SetReadOnly(BOOL bReadOnly)
{
	/*
	if ( bReadOnly )
		m_TocEdit.SetBackgroundColor(FALSE, RGB(191,191,191)) ;
	else
		m_TocEdit.SetBackgroundColor(FALSE, RGB(255,255,255)) ;

	m_TocEdit.SetReadOnly(bReadOnly) ;
	*/
}

// 기사제어번호 태그 보호 루틴!
// 기사제어번호태그는 편집할 수 없도록 해 주기 위해 사용한다.
BOOL CTocEditorDlg::IsInKisaTag(CString sMokcha, INT nLine, INT nCharIdx)
{
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	INT nLineIdx= 0 ;

//	GetOneLineString(nLine, sMokcha) ;	// m_TocEdit.GetLine() 함수 대용 함수
	TCHAR szLine[MAX_LINE] ;
	INT nLineLength= m_TocEdit.GetLine(nLine-1, szLine) ;
	szLine[nLineLength]= _T('\0') ;
	sMokcha= szLine ;

	nFindStart= sMokcha.Find(_START_CHOROK_KEY_) ;
	if ( nFindStart>=0 ) {
		CString sEndTag ;
		sEndTag= _END_CHOROK_KEY_ ;
		nFindEnd= sMokcha.Find(_END_CHOROK_KEY_, nFindStart) ;
		if ( nFindEnd>0 ) {
			nFindEnd+= sEndTag.GetLength() ;
			nLineIdx= m_TocEdit.LineIndex(nLine-1) ;
			if ( nFindStart+nLineIdx<nCharIdx && nFindEnd+nLineIdx>=nCharIdx ) {
				MessageBox(_T("기사제어번호 태그는 편집할 수 없습니다!"), _T("경고"), MB_ICONWARNING) ;
				return TRUE ;
			}
		}
	}

	return FALSE ;
}

VOID CTocEditorDlg::TocEditorBackup()
{
	m_TocEdit.GetWindowText( m_strBak ) ;
	m_sBackupTocEdit= m_strBak ;				// 백업
	if ( m_nIsKindOf==_YON_ )
		m_aBackupChorokStru.Copy(m_aChorokStru) ;	// 백업
}

INT CTocEditorDlg::GetChorokFromLine(UINT nLine, CString &sChorok)
{
	sChorok.Empty() ;

	CString sTmp ;
	sTmp= GetYonKisaNo(nLine, false) ;
	if ( sTmp.IsEmpty() || sTmp==_CHOROK_ERROR_STRING_ )
		return -1 ;

	sTmp= GetYonChorok(nLine) ;
	if ( sTmp==_CHOROK_ERROR_STRING_ )
		return -2 ;

	sChorok= sTmp ;

	return 0 ;
}

BOOL CTocEditorDlg::KisaExistPrevnNext(INT nLine, BOOL bPrev, BOOL bChorokEmptyCheck/*=FALSE*/)
{
	CString sKisaNo ;
	CString strChorok ;

	if ( bPrev )
	{
		for ( INT i=nLine-1 ; i>=1 ; i-- )
		{
			sKisaNo= GetYonKisaNo(i, false) ;
			if ( ! (sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ ) )
			{
				if ( bChorokEmptyCheck )	// 2002.5.3 added by jungjoo
				{
					strChorok= GetYonChorok(i) ;
					if ( strChorok==_CHOROK_ERROR_STRING_ || strChorok.IsEmpty() ) 
						continue ;
				}

				return TRUE ;
			}
		}
	}
	else
	{
		INT nTotalLine= m_TocEdit.GetLineCount() ;
		for ( INT i=nLine+1 ; i<=nTotalLine ; i++ )
		{
			sKisaNo= GetYonKisaNo(i, false) ;
			if ( ! (sKisaNo.IsEmpty() || sKisaNo==_CHOROK_ERROR_STRING_ ) )
			{
				if ( bChorokEmptyCheck )	// 2002.5.3 added by jungjoo
				{
					strChorok= GetYonChorok(i) ;
					if ( strChorok==_CHOROK_ERROR_STRING_ || strChorok.IsEmpty() ) 
						continue ;
				}

				return TRUE ;
			}
		}
	}

	return FALSE ;
}

INT CTocEditorDlg::GetChoroknKisa(CString &sChorok, CString &sKisaNo, CString &sKisa, INT nLine, BOOL bPrev, BOOL bChorokEmptyCheck/*=FALSE*/)
{
	CString strKisaNo ;
	CString strChorok ;
	CString strKisa ;

	if ( bPrev )
	{
		for ( INT i=nLine-1 ; i>=1 ; i-- )
		{
			strKisaNo= GetKisaNofromMokchaLine(i) ;
			if ( strKisaNo==_CHOROK_ERROR_STRING_ || strKisaNo.IsEmpty() ) {
				continue ;
			} else {
				strChorok= GetYonChorok(i) ;
				if ( strChorok==_CHOROK_ERROR_STRING_ ) 
					return -1 ;
				strKisa= GetKisafromKisaNo(strKisaNo) ;
				if ( strKisa==_CHOROK_ERROR_STRING_ )
					return -2 ;

				if ( bChorokEmptyCheck && strChorok.IsEmpty() )	continue ;	// 2002.5.3 added by jungjoo
				sChorok= strChorok ;
				sKisaNo= strKisaNo ;
				sKisa= strKisa ;
				m_icurLine= i ;	// added 2002.11.1 연간물 초록에서 이전 다음 기능을 위해
				return i ;		// 기사가 존재하는 라인을 리턴해준다.
			}

		}

	}
	else
	{
		INT nTotalLine= m_TocEdit.GetLineCount() ;
		for ( INT i=nLine+1 ; i<=nTotalLine ; i++ )
		{
			strKisaNo= GetKisaNofromMokchaLine(i) ;
			if ( strKisaNo==_CHOROK_ERROR_STRING_ || strKisaNo.IsEmpty() ) {
				continue ;
			} else {
				strChorok= GetYonChorok(i) ;
				if ( strChorok==_CHOROK_ERROR_STRING_ ) 
					return -1 ;
				strKisa= GetKisafromKisaNo(strKisaNo) ;
				if ( strKisa==_CHOROK_ERROR_STRING_ )
					return -2 ;

				if ( bChorokEmptyCheck && strChorok.IsEmpty() )	continue ;	// 2002.5.3 added by jungjoo
				sChorok= strChorok ;
				sKisaNo= strKisaNo ;
				sKisa= strKisa ;
				m_icurLine= i ;	// added 2002.11.1 연간물 초록에서 이전 다음 기능을 위해
				return i ;		// 기사가 존재하는 라인을 리턴해준다.
			}

		}
	}

	return -10 ;
}

BOOL CTocEditorDlg::ExistUndonRedo(BOOL bIsUndo)
{
	if ( bIsUndo )
		return ! m_sBackupTocEdit.IsEmpty() ;
	else
		return ! m_sRedoTocEdit.IsEmpty() ;
}

INT CTocEditorDlg::GetMoveRecord(BOOL bMokcha, BOOL bNext, CString& strNewString, BOOL *pbPrevEnable, BOOL *pbNextEnable)
{
	if ( m_pMoveRecordFunc==NULL ) {
		AfxMessageBox( _T("m_pMoveRecordFunc is NULL"), MB_ICONERROR ) ;
		return -20 ;
	}
	else 
	{
		INT ids= (m_pParentWnd->*m_pMoveRecordFunc)(bMokcha, bNext, strNewString, pbPrevEnable, pbNextEnable) ;
		if ( ids<0 )
		{
//			AfxMessageBox( _T("값을 부여받는데 실패했습니다!\r\nMoveRecord 함수 에러!"), MB_ICONERROR ) ;
			strNewString.Empty() ;	// 실패했을 경우 그냥 지워준다.
			return ids ;
		}
	}

	return 0 ;
}

VOID CTocEditorDlg::OnShowViewer()
{
	if ( m_pParentWnd==NULL ) return ;
	m_pParentWnd->SendMessage(MSG_SHOW_VIEWER) ;
}

VOID CTocEditorDlg::GetChorokLine(CUIntArray *parrChorokLine)
{
	parrChorokLine->RemoveAll() ;

	CString sText ;
	m_TocEdit.GetWindowText( sText ) ;

	// 기사라인 번호를 넘겨주기 위해...
	// View 다이얼로그가 떠 있는 동안은 목차는 편집되면 안된다.
	INT nSize= m_aChorokStru.GetSize() ;
	CString sKisaTag ;
	CString sFront ;
	for ( INT i=0 ; i<nSize ; i++ )
	{
		tCHOROK tOneChorok= m_aChorokStru.GetAt(i) ;
		if ( tOneChorok.sKisaStatus==_CHOROK_STATUS_DELETE_ ||		// 초록이 있는 기사의 라인만 넘겨주도록 하자.
			tOneChorok.sChorokStatus==_CHOROK_STATUS_DELETE_ ||
			(tOneChorok.sChorokText).IsEmpty() )
			continue ;

		sKisaTag= _START_CHOROK_KEY_ ;
		sKisaTag+= tOneChorok.sKey ;
		sKisaTag+= _END_CHOROK_KEY_ ;

		INT nFind= sText.Find(sKisaTag) ;
		if ( nFind<0 ) {	//에러발생
			AfxMessageBox( _T("기사태그 ") + sKisaTag + _T(" 이 목차에 존재하지 않습니다.")) ;
			continue ;
		}

		sFront= sText.Mid(0, nFind) ;
		INT nLine= sFront.Replace(_T("\r\n"), _T("\r\n"));
		parrChorokLine->Add(nLine+1) ;
	}
}

INT CTocEditorDlg::GetYonAllKisaNo(CStringArray& arrKisaNo)
{
	INT nTotalLine= m_TocEdit.GetLineCount() ;
	CString sKisaNo ;
	for ( INT i=1 ; i<=nTotalLine ; i++ ) {
		sKisaNo= GetKisaNofromMokchaLine(i) ;
		if ( sKisaNo==_CHOROK_ERROR_STRING_ ) {
			AfxMessageBox( _T("기사제어번호 가져오기 실패!"), MB_ICONERROR ) ;
			return -1 ;
		}
		else if ( ! sKisaNo.IsEmpty() )
		{
			arrKisaNo.Add(sKisaNo) ;
		}
	}

	return 0 ;
}

INT CTocEditorDlg::GetBanchoolFormat(CString sMokcha, CString& sResult)
{
	// 기사제어번호^연속간행물제어번호^권호제어번호^기사제목^저자명^시작페이지(26)
	// 기사제어번호 :		_START_CHOROK_KEY_ ~ _END_CHOROK_KEY_
	// 연속간행물제어번호 :	<id>
	// 권호제어번호 :		<key>
	// 기사제목
	// 저자명
	// 시작페이지

	// 초기화
	sMokcha.Replace(_T("\r\n"), _T("\n"));	// 혹시 있을지도 모르는 개행문자 치환
	sResult.Empty();
	TCHAR szDelimiter= 26;
	TCHAR sep1[] = _T("/");		// 기사제목과 저자명 구분자
	TCHAR sep2[] = _T("=");		// 기사와 시작페이지 구분자


	INT returnidx, tagidx, nYonControlTagLength, nKwonControlTagLength;
	CString sYonControlTag, sYonControlNo;
	CString sKwonControlTag, sKwonControlNo;
	sYonControlTag= _T("<id>");		nYonControlTagLength= sYonControlTag.GetLength();
	sKwonControlTag= _T("<key>");	nKwonControlTagLength= sKwonControlTag.GetLength();
	CString sKey, sKisaTitle, sAuthor, sStartPage;

	// 연속간행물제어번호
	tagidx= sMokcha.Find(sYonControlTag);
	if ( tagidx<0 )	return -1;
	returnidx= sMokcha.Find(_T("\n"), tagidx);	// 개행문자 검색
	if ( returnidx< 0 )	return -2;
	sYonControlNo= sMokcha.Mid(tagidx+nYonControlTagLength , returnidx-tagidx-nYonControlTagLength);

	// 권호제어번호
	tagidx= sMokcha.Find(sKwonControlTag);
	if ( tagidx<0 )	return -3;
	returnidx= sMokcha.Find(_T('\n'), tagidx);	// 개행문자 검색
	if ( returnidx< 0 )	return -4;
	sKwonControlNo= sMokcha.Mid(tagidx+nKwonControlTagLength , returnidx-tagidx-nKwonControlTagLength);

	INT nFindStart, nFindEnd, nStart, nReturn, ntmp;
	nFindStart= nFindEnd= 0 ;	// 초기화
	CString sStartTag, sEndTag, sCutting, sTmp ;
	sStartTag= _START_CHOROK_KEY_ ;
	sEndTag= _END_CHOROK_KEY_ ;


	while ( TRUE )
	{
		nFindStart= sMokcha.Find(sStartTag, nFindEnd) ;
		if ( nFindStart<0 )
		{
			break ;
		}

		nFindEnd=  sMokcha.Find(_END_CHOROK_KEY_, nFindStart ) ;
		if ( nFindEnd<0 )
		{
			return -10 ;
		}

		nStart= nFindStart+sStartTag.GetLength() ;
		sKey= sMokcha.Mid(nStart, nFindEnd-nStart) ;	// KEY 추출
		sCutting= sMokcha;

		sCutting= sCutting.Mid(0,nFindStart);
		nReturn= sCutting.ReverseFind('\n');
		if ( nReturn<0 ) return -11 ;
		sCutting= sCutting.Mid( nReturn+1 ) ;	// sCutting- 한 라인에서 기사제어번호 앞까지의 내용


		// 기사제목, 저자명, 시작페이지 가져온다.
		sKisaTitle= sCutting;
		ntmp= sCutting.Find(sep1);
		if( ntmp<0 )	// '/' 이 없는 경우
		{
			sAuthor= _T("");	// 저자명이 없는 것으로 간주한다.
			ntmp= sCutting.Find(sep2);
			if( ntmp<0 )	// 구분자가 모두 없는 경우
			{
				sStartPage= _T("");
			}
			else	// '=' 이 있는 경우
			{
				sKisaTitle= sCutting.Left(ntmp);
				sStartPage= sCutting.Mid(ntmp+1);
			}
		}
		else		// '/' 이 있는 경우
		{
			sKisaTitle= sCutting.Left(ntmp);
			sCutting= sCutting.Mid(ntmp+1);
			ntmp= sCutting.Find(sep2);
			if( ntmp<0 )	// '=' 이 없는 경우
			{
				sAuthor= sCutting;
				sStartPage= _T("");	// 시작페이지가 없는 것으로 간주한다.
			}
			else	// 구분자가 모두 있는 경우
			{
				sAuthor= sCutting.Left(ntmp);
				sStartPage= sCutting.Mid(ntmp+1);
			}
		}

		sKisaTitle.TrimLeft();	sKisaTitle.TrimRight();
		sAuthor.TrimLeft();		sAuthor.TrimRight();
		sStartPage.TrimLeft();	sStartPage.TrimRight();

		sResult+= szDelimiter + sKey + _T("^");	// 기사제어번호
		sResult+= sYonControlNo + _T("^");		// 연속간행물제어번호
		sResult+= sKwonControlNo + _T("^");		// 권호제어번호
		sResult+= sKisaTitle + _T("^");			// 기사제목
		sResult+= sAuthor + _T("^");			// 저자명
		sResult+= sStartPage;					// 시작페이지
	}

	sResult.TrimLeft(szDelimiter);

	return 0 ;	// 성공
}
/*
VOID CTocEditorDlg::SetTocDefault(CString csstr0, CString csstr1, CString csstr2,
		CString csstr3, CString csstr4, CString csstr5, CString csstr6);
{
	csId = csstr0;
	csCall = csstr1;
	csCount = csstr2;
	csDate = csstr3;
	csPub = csstr4;
	csSertitle = csstr5;
	csTitle = csstr6;	
}
*/
//VOID CTocEditorDlg::SetTocDefault(CString csstr0, CString csstr1, CString csstr2,
//		CString csstr3, CString csstr4, CString csstr5, CString csstr6, CString csstr7);
VOID CTocEditorDlg::SetTocDefault(CString *csstr)
{
	for(INT i=0;i<8;i++) {
		if(&csstr[i] == NULL) return;
		csstrTag[i] = csstr[i];
	}
}



