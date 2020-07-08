// MarcEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EditIntellisens.h"
#include "MarcEditCtrl.h"
#include "efs.h"
#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcEdit

CMarcEditCtrl::CMarcEditCtrl()
{
	// auto list
	m_FocusFlag = FALSE;
	m_bAutoListFocus = FALSE;
	m_ctlAutoList = NULL;
	m_bAltKey = FALSE;
	m_nType = -1;
	m_strSubfieldCode.Format(_T("%c"), 31);	
	
	// KOL.UDF.022 시큐어코딩 보완
	m_szSubfieldCode[0] = '\0';
	m_nType = -1;
}

CMarcEditCtrl::~CMarcEditCtrl()
{	

}


BEGIN_MESSAGE_MAP(CMarcEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CMarcEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_IME_CHAR , OnImeChar)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcEdit message handlers

BOOL CMarcEditCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{		
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle|ES_MULTILINE|ES_WANTRETURN, rect, pParentWnd, nID, pContext);
}

void CMarcEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	EFS_BEGIN

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);

	IntellisendRun();	

	EFS_END	
}

CString CMarcEditCtrl::GetExSelText() const
{
	ASSERT(::IsWindow(m_hWnd));
	CHARRANGE cr;
	cr.cpMin = cr.cpMax = 0;
	::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
	LPTSTR lpsz = (TCHAR*)_alloca((cr.cpMax - cr.cpMin + 1)*2);
	lpsz[0] = NULL;	
	::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpsz);	

	CString strBuffer(lpsz);	
	strBuffer.Replace( _T("\r\n"), _T("\n"));	

	

	return strBuffer;
}

BOOL CMarcEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	INT	s_CtrlKeyFlag, s_AltKeyFlag;

	if ( pMsg->message == WM_IME_COMPOSITION )
	{
		m_ctlAutoList->SetKeyWord(_T(""));
		m_ctlAutoList->SearchKeyWord(-1);					
		m_ctlAutoList->ShowWindow(SW_HIDE);
		m_bAltKey = FALSE;		
		return 0;
	}
	
	if ( pMsg->message == WM_SYSKEYDOWN )
	{
		INT nAltKey = GetKeyState(VK_MENU);
		if (nAltKey < 0 && m_bAltKey == FALSE )
		{
			m_bAltKey = TRUE;			
			IntellisendRun(TRUE);			
			return TRUE;
		}

		if (GetKeyState(VK_MENU))
		{
			TCHAR	nChar;
			// ReadOnly 이면 처리안한다.
			if (GetStyle() & ES_READONLY) return TRUE;
			
			nChar = pMsg->wParam;

			if ( m_szSubfieldCode == nChar ) return TRUE;
			
			if (_istalnum(nChar))
			{
				CString subFieldCode;
				// 알파벳이면 소문자로 바꾼다.
				if (_istalpha(nChar) && _istupper(nChar))
				{
					nChar = _tolower(nChar);
				}
				subFieldCode.Format(_T("%s%c"), m_strSubfieldCode, nChar);
				ReplaceSel(subFieldCode, TRUE);
				IntellisendRun();
				m_szSubfieldCode = nChar;
				return TRUE;
			}
		}
	}

	if ( pMsg->message == WM_SYSKEYUP )
	{
		m_bAltKey = FALSE;
		/*
		m_ctlAutoList->ShowWindow(FALSE);
		::Beep(1000,100);		
		m_bAltKey = FALSE;
		m_szSubfieldCode = 0;
		*/

		/*
		INT nAltKey = GetKeyState(VK_MENU);		
		
		if (nAltKey >= 0 && m_bAltKey == TRUE )
		{			
			m_bAltKey = FALSE;
			m_ctlAutoList->ShowWindow(FALSE);
			return TRUE;
		}
		*/
	}

	if ( pMsg->message == WM_IME_CHAR )
	{
		AfxMessageBox(_T("IME"));
	}
	
	if ( pMsg->message == WM_KEYDOWN )
	{	
		CHARRANGE cr;
		s_CtrlKeyFlag = GetKeyState(VK_CONTROL);
		s_AltKeyFlag  = GetKeyState(VK_LMENU);
		switch ( pMsg->wParam )
		{
			case VK_DELETE :
				break;
			default:				
				CreateAutoList();			
				break;
		}		

		if ( !(s_CtrlKeyFlag < 0  || s_AltKeyFlag < 0) )
		{
			switch ( pMsg->wParam )
			{
				case VK_DOWN:
					if ( m_ctlAutoList->IsWindowVisible() ) 
					{
						if ( m_bAutoListFocus == FALSE ) 
						{													
							//if ( m_ctlAutoList->GetSelectedIndex() < 0 )
							if ( m_ctlAutoList->m_ctlListCtrl.GetItemCount() == 1 )
							{
								m_ctlAutoList->ShowWindow(SW_HIDE);
								m_bAltKey = FALSE;
								SetTimer(1,10, NULL);
								break;
							}
							else 
							{
								if ( m_nType > -1 ) m_ctlAutoList->PrevNextSelection(1);		
							}
							/*
							else
							{
								m_ctlAutoList->ShowWindow(SW_HIDE);
								m_bAltKey = FALSE;
								this->IntellisendRun();
								break;
							}
							*/
						}
						else 
						{
							if ( m_nType > -1 ) m_ctlAutoList->PrevNextSelection(2);
						}
						return TRUE;
					}
					else
					{
						SetTimer(1,1, NULL);
					}
					break;
				case VK_UP:
					if ( m_ctlAutoList->IsWindowVisible() ) 
					{
						if ( m_ctlAutoList->GetSelectedIndex() >= 0 )
						{
							if ( m_bAutoListFocus == TRUE ) 
							{
								if ( m_nType > -1 ) m_ctlAutoList->PrevNextSelection(3);
							}
						}
						else
						{
							m_ctlAutoList->ShowWindow(SW_HIDE);
							m_bAltKey = FALSE;
							SetTimer(1,1, NULL);
							break;
						}
						return TRUE;
					}
					else
					{
						SetTimer(1,1, NULL);
					}
					break;
				case VK_LEFT:									
					GetSel(cr);
					if ( cr.cpMin == cr.cpMax )
					{
						SetSel(cr.cpMax -1, cr.cpMax -1);
					}
					else
					{
						SetSel(cr.cpMin, cr.cpMin);	
					}
					m_ctlAutoList->SetKeyWord(_T(""));
					m_ctlAutoList->SearchKeyWord(-1);					
					m_ctlAutoList->ShowWindow(SW_HIDE);
					m_bAltKey = FALSE;
					this->IntellisendRun();					
					return TRUE;
					break;
				case VK_RIGHT:										
					GetSel(cr);
					if ( cr.cpMin == cr.cpMax )
					{
						SetSel(cr.cpMax +1, cr.cpMax +1);
					}
					else
					{
						SetSel(cr.cpMax, cr.cpMax);	
					}
					m_ctlAutoList->SetKeyWord(_T(""));
					m_ctlAutoList->SearchKeyWord(-1);					
					m_ctlAutoList->ShowWindow(SW_HIDE);
					m_bAltKey = FALSE;
					this->IntellisendRun();	
					return TRUE;
					break;
				case VK_RETURN :
					{	
						if ( m_bAutoListFocus == TRUE )
						{							
							m_bAutoListFocus = FALSE;					
							m_ctlAutoList->ShowWindow(SW_HIDE);
							
							return TRUE;
						}
						else
						{
							m_ctlAutoList->ShowWindow(SW_HIDE);							
						}					
					}
					break;
				case VK_DELETE :					
					break;
				case VK_ESCAPE:
					{
						if ( m_bAutoListFocus == TRUE )	
						{
							m_bAutoListFocus = FALSE;					
							m_ctlAutoList->ShowWindow(SW_HIDE);		
							return TRUE;
						}
						else
						{
							return GetParent()->PreTranslateMessage(pMsg);
						}
					}
					break;
				case VK_BACK:
					if ( m_bAutoListFocus == TRUE )	
					{
						m_bAutoListFocus = FALSE;					
						m_ctlAutoList->ShowWindow(SW_HIDE);		
					}
					break;
				case VK_TAB:
					IntellisendRun(TRUE);
					/*
					if ( m_bAutoListFocus == FALSE ) 
					{
						m_ctlAutoList->PrevNextSelection(1);		
					}
					*/
					return TRUE;
					break;
				case VK_F1:
					break;
				case VK_F2:
					break;
				case VK_F3:
					break;
				case VK_F4:
					break;
				case VK_F5:
					break;
				case VK_F6:
					break;
				case VK_F7:
					break;
				case VK_F8:
					break;
				case VK_F9:
					break;
				case VK_F10:
					break;
				case VK_F11:
					break;
				case VK_F12:
					break;
				case VK_HOME:
					break;
				case VK_END:
					break;
				case VK_INSERT:
					break;
				default:
					break;
			}
		}
	}
	
	if ( pMsg->wParam == VK_RETURN )
	{
		return CRichEditCtrl::PreTranslateMessage(pMsg);
	}	
	
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}


void CMarcEditCtrl::CreateAutoList()
{	
EFS_BEGIN
	if ( m_ctlAutoList != NULL ) {return;}	
	if ( m_bAutoListFocus == TRUE ) {return;}
	if ( GetSafeHwnd() == NULL ) {return;}		

	CWnd	*EditWnd;
	UINT   nEditID;

	EditWnd = GetFocus();
	if ( EditWnd == NULL ) {return;}
	nEditID = EditWnd->GetDlgCtrlID();	
	
	
	EditWnd->GetClientRect(&m_rect);
	ClientToScreen(m_rect);		
	
	CPoint capos = this->GetCaretPos();
	m_rect.left += capos.x;
	m_rect.top += capos.y+20;
	m_rect.right = m_rect.left + 200;
	m_rect.bottom = m_rect.top + 100;

	/*m_rect.left -= 2;
	m_rect.top += m_rect.bottom - m_rect.top+2;
	//m_rect.right = m_rect.left + 337;
	m_rect.bottom = m_rect.top + 150; 
	*/	
	
	m_ctlAutoList = new CECO_MarcAutoList( this );
	if ( m_ctlAutoList == NULL ) {return;}
	m_ctlAutoList->Create(NULL, NULL, LVS_SINGLESEL, m_rect, GetParent(), NULL, NULL);	
	//m_ctlAutoList->MoveWindow(m_rect);
	m_ctlAutoList->SetResource( nEditID, this );
	m_ctlAutoList->Display();
	m_ctlAutoList->ShowWindow(SW_HIDE);
	//SetFocus();
EFS_END
}

void CMarcEditCtrl::DeleteAutoList()
{
EFS_BEGIN
	if ( m_ctlAutoList != NULL && GetSafeHwnd() != NULL) 
	{
		CString strText;
		GetWindowText(strText);

		m_ctlAutoList->SetKeyWord(strText.GetBuffer(0));
		m_ctlAutoList->ReferenceWrite();
		delete m_ctlAutoList;
		m_ctlAutoList = NULL;
		m_bAutoListFocus = FALSE;
	}
EFS_END
}

int CMarcEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	if ( m_ctlAutoList->m_pParentWnd != NULL )
		CreateAutoList();	
	return 0;
	
	return 0;
}


VOID CMarcEditCtrl::ShowAutoList(BOOL bFlag)
{
	if ( m_ctlAutoList == NULL ) {return;}	
	if ( m_bAutoListFocus == TRUE ) {return;}	
	

	INT nItemCount = m_ctlAutoList->m_strArraySearchKeyWordList.GetSize();
	if ( nItemCount <= 0 ) return;

	CWnd	*EditWnd;
	UINT   nEditID;

	EditWnd = GetFocus();
	if ( EditWnd == NULL ) {return;}
	nEditID = EditWnd->GetDlgCtrlID();	
	
	
	EditWnd->GetClientRect(&m_rect);
	ClientToScreen(m_rect);		
	
	CPoint capos = this->GetCaretPos();
	
	if ( m_nType == _TAG_ )
	{
		m_rect.left += 0;
	}
	else if ( m_nType == _1ST_ )
	{
		m_rect.left += capos.x-8;
	}
	else if ( m_nType == _2ST_ )
	{
		m_rect.left += capos.x-8;
	}
	else 
	{
		m_rect.left += capos.x-20;
	}

	m_rect.top += capos.y+20;

	
	INT nLength = m_ctlAutoList->MaxSearchKeywordSize();

	/*
	if ( m_rect.left + ( nLength * 6 ) > 500 )
	{
		m_rect.right = m_rect.left + ( nLength * 6 )+150;
	}
	else
	{
		m_rect.right = m_rect.left + 250;
	}
	*/
	if ( m_nType == _TAG_ )
	{
		m_rect.right = m_rect.left + ( nLength * 7 ) + 200;
	}
	else if ( m_nType == _1ST_ || m_nType == _2ST_ )
	{
		m_rect.right = m_rect.left + ( nLength * 7 ) + 30;
	}
	else if ( m_nType == _SUBFIELD_ )
	{
		m_rect.right = m_rect.left + ( nLength * 7 ) + 110;
	}
	else
	{
	}
	
	m_rect.bottom = m_rect.top + (nItemCount*14)+5;	
	//m_rect.bottom = m_rect.top + (nItemCount*20)+6;	

	m_ctlAutoList->MoveWindow(m_rect);	
	m_ctlAutoList->ShowWindow(bFlag);

	// Font Size & String Size
	
}

VOID CMarcEditCtrl::IntellisendRun(BOOL bAbsPos)
{
	HideSelection(TRUE, FALSE);
	// 자동완성
	CString		strPrevString;
	CString		strTag;	
	CString		strIndi;
	CString		strSubfieldCode;
	CHARRANGE	cr;
	INT			nCodeIndex;
	INT			nLine;
	 
	//_TAG_:0, _1ST_:1, _2ST_:2, _SUBFIELD_:3, _DATA_:4		
	// 시작일 경우
	// 0 부터 시작
	strTag = _T("");	
	nLine = GetFirstVisibleLine();	
	CPoint nCap = GetCaretPos();	

	this->GetSel(cr);		
	if ( nLine == 0 && nCap.y == 1)
	{
		nCodeIndex = cr.cpMax;
		SetSel(0, nCodeIndex);
		strPrevString = this->GetExSelText();
	}
	else
	{	
		SetSel(0, cr.cpMax );
		strPrevString = GetExSelText();
		INT nPos = strPrevString.ReverseFind(_T('\r'));		
		strPrevString = strPrevString.Mid(nPos+1);
		nCodeIndex = strPrevString.GetLength();
	}	
			
	if ( nCodeIndex >= 3 )
	{
		m_nType = _TAG_;
		strTag = strPrevString.Mid(0, 3);
	}
	else if ( nCodeIndex < 3 )
	{
		m_nType = _TAG_;
		strTag = strPrevString.Mid(0, nCodeIndex);		
	}
	
	if ( (nCodeIndex == 4 && bAbsPos == FALSE) || (nCodeIndex == 3 && bAbsPos == TRUE) )
	{
		m_nType = _1ST_;
		
		if ( nCodeIndex == 4 ) strIndi = strPrevString.Mid(nCodeIndex-1, nCodeIndex);
		else strIndi=_T("?");
	}
	else if ( ( nCodeIndex == 5 && bAbsPos == FALSE) || (nCodeIndex == 4 && bAbsPos == TRUE))
	{
		m_nType = _2ST_;
		
		if ( nCodeIndex == 5 ) strIndi = strPrevString.Mid(nCodeIndex-1, nCodeIndex);
		else strIndi = _T("?");
	}		
	else if ( nCodeIndex > 5 || (nCodeIndex == 5 && bAbsPos == TRUE) )
	{
		CString temp;
		m_nType = _SUBFIELD_;
		
		if ( m_bAltKey == TRUE )
		{
			strSubfieldCode = _T("?");
			temp = strPrevString.Mid(nCodeIndex-2, nCodeIndex);
			if ( temp.Left(1) == m_strSubfieldCode )
			{
				//strSubfieldCode = temp;
				strSubfieldCode.Format(_T("▼%s"), temp.Mid(1,1));
			}				
		}
		else
		{
			temp = strPrevString.Mid(nCodeIndex-2, nCodeIndex);
			if ( temp.Left(1) == m_strSubfieldCode )
			{
				//strSubfieldCode = temp;
				strSubfieldCode.Format(_T("▼%s"), temp.Mid(1,1));				
			}				
			else
			{
				//m_nType = -1;
				SetSel(cr);
				m_ctlAutoList->SetKeyWord(_T(""));
				m_ctlAutoList->SearchKeyWord(-1);
				m_ctlAutoList->ShowWindow(SW_HIDE);
				m_bAltKey = FALSE;
				HideSelection(FALSE, FALSE);				
				return;
			}
		}		
	}	

	SetSel(cr);

	if ( m_ctlAutoList != NULL ) 
	{	
		if ( m_nType == _TAG_ ) 
		{
			m_ctlAutoList->SetKeyWord(strTag.GetBuffer(0));
		}
		else if ( m_nType == _1ST_  || m_nType == _2ST_ ) 
		{
			strTag += _T(":") + strIndi;
			m_ctlAutoList->SetKeyWord(strTag.GetBuffer(0));
		}
		else if ( m_nType == _SUBFIELD_ )
		{
			strTag += _T(":") + strSubfieldCode;
			m_ctlAutoList->SetKeyWord(strTag.GetBuffer(0));
		}
		
		m_ctlAutoList->SearchKeyWord(m_nType);	
		ShowAutoList(TRUE);
		HideSelection(FALSE, FALSE);
		return;
	}
	
	CreateAutoList();
	HideSelection(FALSE, FALSE);
}

void CMarcEditCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditCtrl::OnSetFocus(pOldWnd);	
}

void CMarcEditCtrl::OnImeChar()
{
	return ;	  
}

void CMarcEditCtrl::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		this->IntellisendRun();
	}
	
	CRichEditCtrl::OnTimer(nIDEvent);
}

void CMarcEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetTimer(1,1, NULL);
	
	CRichEditCtrl::OnLButtonDown(nFlags, point);
}
