// HourEditControl.cpp : implementation file
//

#include "stdafx.h"
#include "HourEditControl.h"
#include "efs.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Style bits for the individual edit controls
const INT WS_EDIT = WS_CHILD | WS_VISIBLE | ES_CENTER | ES_MULTILINE;

/////////////////////////////////////////////////////////////////////////////
// CHourEditControl

CHourEditControl::CHourEditControl()
{
	m_bEnabled = TRUE;
	m_bReadOnly = FALSE;
	// KOL.UDF.022
	m_ControlStyle = 0;
	m_rcElem [0] = NULL;
	m_rcElem [1] = NULL;
	m_rcElem [2] = NULL;
	m_rcSlash[0] = NULL;
	m_rcSlash[1] = NULL;
}

CHourEditControl::~CHourEditControl()
{
}


BEGIN_MESSAGE_MAP(CHourEditControl, CEdit)
	//{{AFX_MSG_MAP(CHourEditControl)
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHourEditControl message handlers
VOID CHourEditControl::InitEdit()
{
EFS_BEGIN
	
    ModifyStyleEx(0, WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY);

	m_Elem[0].Create(WS_EDIT, CRect(0,0,0,0), this, IDC_HOUR);
	m_Elem[0].LimitText(2);
	m_Elem[0].SetParent(this);
	m_Elem[0].SetFont( GetFont() );
	m_Elem[0].EnableWindow(m_bEnabled);

	m_Elem[1].Create(WS_EDIT, CRect(0,0,0,0), this, IDC_MIN);
	m_Elem[1].LimitText(2);
	m_Elem[1].SetParent(this);
	m_Elem[1].SetFont( GetFont() );
	m_Elem[0].EnableWindow(m_bEnabled);
	
	m_Elem[2].Create(WS_EDIT, CRect(0,0,0,0), this, IDC_SEC);
	m_Elem[2].LimitText(2);
	m_Elem[2].SetParent(this);
	m_Elem[2].SetFont( GetFont() );
	m_Elem[0].EnableWindow(m_bEnabled);

	CDC*	pDC = GetDC();
	CSize	szDot = pDC->GetTextExtent(_T(":"), 1);
	INT nDotWidth = szDot.cx;
	ReleaseDC(pDC);	
    
	//
	CRect	rcClient;
	GetClientRect(&rcClient);
	INT nEditWidth = (rcClient.Width() - (2 * nDotWidth)) / 3;
	INT nEditHeight = rcClient.Height();
	INT cyEdge = ::GetSystemMetrics(SM_CYEDGE);

	//
	m_rcElem[0]= CRect(0, cyEdge, nEditWidth, nEditHeight);
	m_Elem[0].MoveWindow( m_rcElem[0] );
	m_rcElem[1]= CRect( nEditWidth + nDotWidth, cyEdge,
						nEditWidth * 2 + nDotWidth, nEditHeight);
	m_Elem[1].MoveWindow( m_rcElem[1] );
	m_rcElem[2]= CRect( nEditWidth * 2 + nDotWidth * 2, cyEdge,
						nEditWidth * 3 + nDotWidth * 2, nEditHeight);
	m_Elem[2].MoveWindow( m_rcElem[2] );

	//
	CRect rect = CRect(nEditWidth, 0, nEditWidth + nDotWidth, nEditHeight);
	m_rcSlash[0] = rect;
	rect.OffsetRect(nEditWidth + nDotWidth, 0);
	m_rcSlash[1] = rect;

    INT   ids;
    ids = ControlModifyStyle( m_ControlStyle , TRUE );
    if ( 0 > ids ) return;

EFS_END
}

INT CHourEditControl::ControlModifyStyle( INT lStyle , BOOL bSetBits)
{
EFS_BEGIN 
  return 0;
EFS_END
	return 0;
}

VOID CHourEditControl::OnSetFocus(CWnd* pOldWnd)
{
EFS_BEGIN
    CEdit::OnSetFocus(pOldWnd);
	
	m_Elem[0].SetFocus();							// Set focus to first edit control
	m_Elem[0].SetSel(0, -1);						// Select entire contents

	Invalidate();
EFS_END
}

VOID CHourEditControl::OnEnable(BOOL bEnable) 
{
EFS_BEGIN
	CEdit::OnEnable(bEnable);
	Control_EnableWindow(bEnable);
EFS_END
}

BOOL CHourEditControl::OnEraseBkgnd(CDC* pDC) 
{
EFS_BEGIN
    CRect	rcClient;
	GetClientRect(&rcClient);

	if (m_bEnabled && !m_bReadOnly)
		::FillRect(pDC->m_hDC, rcClient, (HBRUSH) (COLOR_WINDOW+1));
	else
		::FillRect(pDC->m_hDC, rcClient, (HBRUSH) (COLOR_BTNFACE+1));

	return TRUE;
EFS_END	
	return TRUE;
}

VOID CHourEditControl::OnPaint() 
{
EFS_BEGIN
    CPaintDC dc(this);
	dc.SelectObject( GetFont() );

	// Save mode and set to transparent (so background remains)
	INT nOldMode = dc.SetBkMode(TRANSPARENT);

	COLORREF crText;
	if (m_bEnabled)
		crText = ::GetSysColor(COLOR_WINDOWTEXT);
	else
		crText = ::GetSysColor(COLOR_GRAYTEXT);
	COLORREF crOldText = dc.SetTextColor(crText);

	for (INT ii = 0; ii < 2; ii++)
		dc.DrawText(_T(":"), 1, m_rcSlash[ii], DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc.SetBkMode(nOldMode);
	dc.SetTextColor(crOldText);

EFS_END
}

VOID CHourEditControl::OnChildChar(UINT nChar, UINT nRepCnt, UINT nFlags, CHourEditElem& child)
{
EFS_BEGIN
    switch (nChar)
	{
	case '/':
	case VK_RIGHT:
	case ' ':
		{
			UINT nIDC = child.GetDlgCtrlID();
			if (nIDC < IDC_SEC)
			{
				m_Elem[nIDC - IDC_HOUR + 1].SetFocus();
				if (VK_RIGHT != nChar)
					m_Elem[nIDC - IDC_HOUR + 1].SetSel(0, -1);
			}
		}
		break;

	case VK_LEFT:
	case VK_BACK:
		{
			UINT nIDC = child.GetDlgCtrlID();
			if (nIDC > IDC_HOUR) m_Elem[nIDC - IDC_HOUR - 1].SetFocus();
		}
		break;

	case VK_TAB:
		{			
			CString temp;
			if ( *temp != NULL ) {
			}

			CWnd*	pWnd;
			SHORT nShift = ::GetKeyState(VK_SHIFT);
			if (nShift < 0)
				pWnd = GetParent()->GetNextDlgTabItem(this, TRUE);
			else
				pWnd = GetParent()->GetNextDlgTabItem(this, FALSE);
			if (NULL != pWnd)
				pWnd->SetFocus();
		}
		break;

	case VK_RETURN:
		{
			DWORD dw = ((CDialog*) GetParent())->GetDefID();
			if (DC_HASDEFID == HIWORD(dw))
			{
				CWnd* pWnd = GetParent()->GetDlgItem(LOWORD(dw));
                if ( NULL == pWnd ) break;
				WPARAM wp = MAKEWPARAM(LOWORD(dw), BN_CLICKED);
				GetParent()->SendMessage( 
                                          WM_COMMAND ,
                                          wp         ,
                                          (LPARAM) (NULL != pWnd->m_hWnd ? pWnd->m_hWnd : NULL)
                                        );
			}
		}
		break;

	case 'h':
		if ( !CheckHour() ) return;
		OnChildChar('/', 0, nFlags, child);
		break;

	case 'm':
		if ( !CheckMin() ) return;
		OnChildChar('/', 0, nFlags, child);
		break;

	case 's':
		if ( !CheckSec() ) return;
		OnChildChar('/', 0, nFlags, child);
		break;

	default:
		TRACE(_T("Unexpected call to CECO_ControlDateEdit::OnChildChar!\n"));
	}
EFS_END
}

LONG CHourEditControl::OnSetReadOnly(UINT wParam, LONG lParam)
{
EFS_BEGIN
    Control_SetReadOnly( (BOOL) wParam );

	return TRUE;
EFS_END
	return TRUE;
}

VOID CHourEditControl::Control_EnableWindow(BOOL bEnable) 
{
EFS_BEGIN
	if (bEnable != m_bEnabled)
	{
		m_bEnabled = bEnable;

		for (INT ii = 0; ii < 3; ii++)
			m_Elem[ii].EnableWindow(bEnable);
	}

	Invalidate();
EFS_END
}

VOID CHourEditControl::Control_SetReadOnly(BOOL bReadOnly)
{
EFS_BEGIN
    m_bReadOnly = bReadOnly;

	for (INT ii = 0; ii < 3; ii++)
		m_Elem[ii].SetReadOnly(m_bReadOnly);

	Invalidate();
EFS_END
}

BOOL CHourEditControl::CheckHour()
{
EFS_BEGIN
    INT h = GetHour();
	CString sText1,sText2;

	GetDlgItem(IDC_MIN)->GetWindowText(sText1);
	GetDlgItem(IDC_SEC)->GetWindowText(sText2);
	if (sText1.IsEmpty() && sText2.IsEmpty()) return TRUE;
	if (h<0 || h>23) {
		MessageBox(_T("시간에 0 ~ 23사이의 값을 입력하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_HOUR))->SetFocus();
		((CEdit*)GetDlgItem(IDC_HOUR))->SetSel(0, -1);
		SetHour(_T(""));
		return FALSE;
	}

	return TRUE;
EFS_END
	return 0;
}

BOOL CHourEditControl::CheckMin()
{
EFS_BEGIN
    INT m = GetMin();
	CString sText1,sText2;

	GetDlgItem(IDC_MIN)->GetWindowText(sText1);
	GetDlgItem(IDC_SEC)->GetWindowText(sText2);
	if (sText1.IsEmpty() && sText2.IsEmpty()) return TRUE;
	if (m < 0 || m > 59) {
		MessageBox(_T("분에 0 ~ 59사이의 값을 입력하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_MIN))->SetFocus();
		((CEdit*)GetDlgItem(IDC_MIN))->SetSel(0, -1);
		SetMin(_T(""));
		return FALSE;
	}

	return TRUE;
EFS_END
	return 0;
}

BOOL CHourEditControl::CheckSec()
{
EFS_BEGIN
    INT m = GetSec();
	CString sText1,sText2;

	GetDlgItem(IDC_MIN)->GetWindowText(sText1);
	GetDlgItem(IDC_SEC)->GetWindowText(sText2);
	if (sText1.IsEmpty() && sText2.IsEmpty()) return TRUE;
	if (m < 0 || m > 59) {
		MessageBox(_T("초에 0 ~ 59사이의 값을 입력하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_SEC))->SetFocus();
		((CEdit*)GetDlgItem(IDC_SEC))->SetSel(0, -1);
		SetSec(_T(""));
		return FALSE;
	}

	return TRUE;
EFS_END
	return 0;
}

INT CHourEditControl::GetHour( VOID )
{
	CString text;
    GetDlgItem(IDC_HOUR)->GetWindowText(text);
	if (text.GetLength() == 0) return -1;
	return _ttoi(text);
}

INT CHourEditControl::GetHour( CString &text)
{
EFS_BEGIN
    GetDlgItem(IDC_HOUR)->GetWindowText(text);
	if (text.IsEmpty()) {		
		text=_T("0");
		return -1;
	}
	return 0;
EFS_END
	return 0;
}

INT CHourEditControl::GetMin( VOID )
{
	CString text;
    GetDlgItem(IDC_MIN)->GetWindowText(text);
	if (text.GetLength() == 0) return -1;
	return _ttoi(text);
}

INT CHourEditControl::GetMin( CString &text)
{
EFS_BEGIN
    GetDlgItem(IDC_MIN)->GetWindowText(text);
	if (text.IsEmpty()) {
		text=_T("0");
		return -1;
	}
	return 0;
EFS_END
	return 0;
}

INT CHourEditControl::GetSec( VOID )
{
	CString text;
    GetDlgItem(IDC_SEC)->GetWindowText(text);
	if (text.GetLength() == 0) return -1;
	return _ttoi(text);
}

INT CHourEditControl::GetSec( CString &text)
{
EFS_BEGIN
    GetDlgItem(IDC_SEC)->GetWindowText(text);
	if (text.IsEmpty()) {		
		text=_T("0");
		return -1;
	}
	return 0;
EFS_END
	return 0;
}

INT CHourEditControl::GetData( CString &data )
{
	CString hour, min, sec;
	INT ids_H, ids_M, ids_S;
	ids_H = GetHour(hour);
	ids_M = GetMin(min);
	ids_S = GetSec(sec);
	if (ids_H == -1 && ids_M == -1 && ids_S == -1) {
		data = _T("");
		return 0;
	}
	
	data.Format(_T("%02d:%02d:%02d"), _ttoi(hour), _ttoi(min), _ttoi(sec));
	return 0;
}

INT CHourEditControl::SetHour( CString text )
{
    GetDlgItem(IDC_HOUR)->SetWindowText(text);
	return 0;
}

INT CHourEditControl::SetMin( CString text )
{
    GetDlgItem(IDC_MIN)->SetWindowText(text);
	return 0;
}

INT CHourEditControl::SetSec( CString text )
{
    GetDlgItem(IDC_SEC)->SetWindowText(text);
	return 0;
}

INT CHourEditControl::SetData( CString data )
{
	SetHour(_T(""));
	SetMin (_T(""));
	SetSec (_T(""));

	data.TrimLeft();data.TrimRight();

	CString hour, min, sec;
	INT PrevPosition, CursorPosition;
	CursorPosition = data.Find(_T(":"), 0);
	if (CursorPosition == -1) return -1;
	hour = data.Mid(0, CursorPosition);
	SetHour(hour);
	PrevPosition = CursorPosition;
	CursorPosition = data.Find(_T(":"), PrevPosition+1);
	if (CursorPosition == -1) 
	{
		min = data.Mid(PrevPosition+1);
		SetMin(min);
		SetSec(_T("00"));
		return 0;
	}

	min = data.Mid(PrevPosition+1, CursorPosition-PrevPosition-1);
	SetMin(min);
	sec = data.Mid(CursorPosition+1);
	SetSec(sec);

	return 0;
}

VOID CHourEditControl::Clear()
{
EFS_BEGIN 
	SetHour( _T("") );
	SetMin ( _T("") );
	SetSec ( _T("") );
EFS_END
}