// ECO_DateEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ECO_ControlDateEdit.h"
#include "except.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"
#include "efs.h"

// Style bits for the individual edit controls
const INT WS_EDIT = WS_CHILD | WS_VISIBLE | ES_CENTER | ES_MULTILINE;
/////////////////////////////////////////////////////////////////////////////
// CECO_ControlDateEdit

CECO_ControlDateEdit::CECO_ControlDateEdit(CESL_Control_Element *ELEMENT)
{
EFS_BEGIN
	this->ELEMENT = NULL;
	this->ELEMENT = ELEMENT;
    m_bEnabled = TRUE;
	m_bReadOnly = FALSE;
EFS_END
}


CECO_ControlDateEdit::~CECO_ControlDateEdit()
{
}


/////////////////////////////////////////////////////////////////////////////
// CECO_ControlDateEdit message handlers
BEGIN_MESSAGE_MAP(CECO_ControlDateEdit, CEdit)
	//{{AFX_MSG_MAP(CECO_ControlDateEdit)
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


VOID CECO_ControlDateEdit::InitEdit()
{
EFS_BEGIN

	RequireFlag = false;
	if (ELEMENT) {
		CESL_Mgr *pMgr = (CESL_Mgr *)ELEMENT->WorkWnd;
		if (pMgr) {
			m_bkR = pMgr->m_bkR;
			m_bkG = pMgr->m_bkG;
			m_bkB = pMgr->m_bkB;
			m_BkBrush.CreateSolidBrush(RGB(m_bkR, m_bkG, m_bkB));
		}
		if (ELEMENT->REQUIRECHECK.GetLength() > 0) {
			RequireFlag = true;
		}
	}

    ModifyStyleEx(0, WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY);

	m_Elem[0].Create(WS_EDIT, CRect(0,0,0,0), this, IDC_YEAR);
	m_Elem[0].LimitText(4);
	m_Elem[0].SetParent(this);
	m_Elem[0].SetFont( GetFont() );
	m_Elem[0].EnableWindow(m_bEnabled);

	m_Elem[1].Create(WS_EDIT, CRect(0,0,0,0), this, IDC_MONTH);
	m_Elem[1].LimitText(2);
	m_Elem[1].SetParent(this);
	m_Elem[1].SetFont( GetFont() );
	m_Elem[0].EnableWindow(m_bEnabled);
	
	m_Elem[2].Create(WS_EDIT, CRect(0,0,0,0), this, IDC_DAY);
	m_Elem[2].LimitText(2);
	m_Elem[2].SetParent(this);
	m_Elem[2].SetFont( GetFont() );
	m_Elem[0].EnableWindow(m_bEnabled);

	CDC*	pDC = GetDC();
	CSize	szDot = pDC->GetTextExtent(_T("/"), 1);
	INT nDotWidth = szDot.cx;
	ReleaseDC(pDC);	
    
	//
	CRect	rcClient;
	GetClientRect(&rcClient);
	INT nEditWidth = (rcClient.Width() - (2 * nDotWidth)) / 4;
	INT nEditHeight = rcClient.Height();
	INT cyEdge = ::GetSystemMetrics(SM_CYEDGE);

	//
	m_rcElem[0]= CRect(0, cyEdge, nEditWidth * 2, nEditHeight);
	m_Elem[0].MoveWindow( m_rcElem[0] );
	m_rcElem[1]= CRect( nEditWidth * 2 + nDotWidth, cyEdge,
						nEditWidth * 3 + nDotWidth, nEditHeight);
	m_Elem[1].MoveWindow( m_rcElem[1] );
	m_rcElem[2]= CRect( nEditWidth * 3 + nDotWidth * 2, cyEdge,
						nEditWidth * 4 + nDotWidth * 2, nEditHeight);
	m_Elem[2].MoveWindow( m_rcElem[2] );

	//
	CRect rect = CRect(nEditWidth*2, 0, nEditWidth*2 + nDotWidth, nEditHeight);
	m_rcSlash[0] = rect;
	rect.OffsetRect(nEditWidth + nDotWidth, 0);
	m_rcSlash[1] = rect;

    INT   ids;
  
    ids = EcoModifyStyle( m_ControlStyle , TRUE );
    if ( 0 > ids ) return;

EFS_END
}

VOID CECO_ControlDateEdit::OnSetFocus(CWnd* pOldWnd)
{
EFS_BEGIN
    CEdit::OnSetFocus(pOldWnd);
	
	m_Elem[0].SetFocus();							// Set focus to first edit control
	m_Elem[0].SetSel(0, -1);						// Select entire contents

	Invalidate();
EFS_END
}

VOID CECO_ControlDateEdit::OnChildChar(UINT nChar, UINT nRepCnt, UINT nFlags, CECO_ControlDateEditElem& child)
{
EFS_BEGIN
    switch (nChar)
	{
	case '/':
	case VK_RIGHT:
	case ' ':
		{
			UINT nIDC = child.GetDlgCtrlID();
			if (nIDC < IDC_DAY)
			{
				m_Elem[nIDC - IDC_YEAR + 1].SetFocus();
				if (VK_RIGHT != nChar)
					m_Elem[nIDC - IDC_YEAR + 1].SetSel(0, -1);
			}
		}
		break;

	case VK_LEFT:
	case VK_BACK:
		{
			UINT nIDC = child.GetDlgCtrlID();
			if (nIDC > IDC_YEAR) m_Elem[nIDC - IDC_YEAR - 1].SetFocus();
		}
		break;

	case VK_TAB:
		{
			CString temp;
			GetData(temp);
			if ( *temp != NULL ) {
				if ( !CheckYear() ) return;
				if ( !CheckMonth() ) return;
				if ( !CheckDay() ) return;
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

	case 'y':
		OnChildChar('/', 0, nFlags, child);
		break;

	case 'm':
		OnChildChar('/', 0, nFlags, child);
		break;

	case 'd':
		OnChildChar('/', 0, nFlags, child);
		break;

	default:
		TRACE(_T("Unexpected call to CECO_ControlDateEdit::OnChildChar!\n"));
	}
EFS_END
}

VOID CECO_ControlDateEdit::OnPaint() 
{
EFS_BEGIN
    CPaintDC dc(this);
	dc.SelectObject( GetFont() );

	// Save mode and set to transparent (so background remains)
	INT nOldMode = dc.SetBkMode(TRANSPARENT);

	// If disabled, set text color to COLOR_GRAYTEXT, else use COLOR_WINDOWTEXT
	COLORREF crText;
	if (m_bEnabled)
		crText = ::GetSysColor(COLOR_WINDOWTEXT);
	else
		crText = ::GetSysColor(COLOR_GRAYTEXT);
	COLORREF crOldText = dc.SetTextColor(crText);

	// Draw the three _T(".") which separate the four edit controls
	for (INT ii = 0; ii < 2; ii++)
		dc.DrawText(_T("/"), 1, m_rcSlash[ii], DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// Restore old mode and color
	dc.SetBkMode(nOldMode);
	dc.SetTextColor(crOldText);

	// Do not call CEdit::OnPaint() for painting messages
EFS_END
}

VOID CECO_ControlDateEdit::OnEnable(BOOL bEnable) 
{
EFS_BEGIN
	CEdit::OnEnable(bEnable);
	ECO_EnableWindow(bEnable);
EFS_END
}

BOOL CECO_ControlDateEdit::OnEraseBkgnd(CDC* pDC) 
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

LONG CECO_ControlDateEdit::OnSetReadOnly(UINT wParam, LONG lParam)
{
EFS_BEGIN
    ECO_SetReadOnly( (BOOL) wParam );

	return TRUE;
EFS_END
	return TRUE;
}

VOID CECO_ControlDateEdit::ECO_EnableWindow(BOOL bEnable) 
{
EFS_BEGIN
    // Nothing to do unless the window state has changed
	if (bEnable != m_bEnabled)
	{
		// Save new state
		m_bEnabled = bEnable;

		// Adjust child controls appropriately
		for (INT ii = 0; ii < 3; ii++)
			m_Elem[ii].EnableWindow(bEnable);
	}

	Invalidate();
EFS_END
}

VOID CECO_ControlDateEdit::ECO_SetReadOnly(BOOL bReadOnly)
{
EFS_BEGIN
    m_bReadOnly = bReadOnly;

	for (INT ii = 0; ii < 3; ii++)
		m_Elem[ii].SetReadOnly(m_bReadOnly);

	Invalidate();
EFS_END
}

BOOL CECO_ControlDateEdit::CheckYear()
{
EFS_BEGIN
    INT y = GetYear();
	
	CString sText1,sText2;
	GetDlgItem(IDC_MONTH)->GetWindowText(sText1);
	GetDlgItem(IDC_DAY)->GetWindowText(sText2);
	if (sText1.IsEmpty() && sText2.IsEmpty()) return TRUE;

	if (y == 0) {
		MessageBox(_T("년도를 입력하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_YEAR))->SetFocus();
		((CEdit*)GetDlgItem(IDC_YEAR))->SetSel(0, -1);
		return FALSE;
	}
	else if(y > 2037)
	{
		MessageBox(_T("년도 제한범위는 2037년까지 입니다."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_YEAR))->SetFocus();
		((CEdit*)GetDlgItem(IDC_YEAR))->SetSel(0, -1);
		return FALSE;
	}


	return TRUE;
EFS_END
	return 0;
}

BOOL CECO_ControlDateEdit::CheckMonth()
{
EFS_BEGIN
    INT m = GetMonth();
	
	CString sText1,sText2;
	GetDlgItem(IDC_MONTH)->GetWindowText(sText1);
	GetDlgItem(IDC_DAY)->GetWindowText(sText2);
	if (sText1.IsEmpty() && sText2.IsEmpty()) return TRUE;

	if (m < 1 || m > 12) {
		MessageBox(_T("월에 1 ~ 12사이의 값을 선택하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_MONTH))->SetFocus();
		((CEdit*)GetDlgItem(IDC_MONTH))->SetSel(0, -1);
		SetMonth(_T(""));
		return FALSE;
	}

	return TRUE;
EFS_END
	return 0;
}

BOOL CECO_ControlDateEdit::CheckDay()
{
EFS_BEGIN
    INT max_day[]= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	INT y= GetYear();
	INT m= GetMonth();
	INT d= GetDay();

	if ( y == 0 ) {
		MessageBox(_T("년도를 입력하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_YEAR))->SetFocus();
		((CEdit*)GetDlgItem(IDC_YEAR))->SetSel(0, -1);
		return FALSE;
	}

	if ( m == 0 ) {
		MessageBox(_T("월에 1 ~ 12사이의 값을 선택하십시오."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_MONTH))->SetFocus();
		((CEdit*)GetDlgItem(IDC_MONTH))->SetSel(0, -1);
		return FALSE;
	}	

	// 윤년검사
	if ( (m == 2) && (y % 4 == 0) && ( (y % 100 != 0) || (y % 400 == 0) ) ) {
		max_day[1]= 29;
	}

	CString frm;	

	if ( d == 0 ) {
		frm.Format(_T("일에 1 ~ %d사이의 값을 선택하십시오."), max_day[m-1] );
		MessageBox(frm, _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_DAY))->SetFocus();
		((CEdit*)GetDlgItem(IDC_DAY))->SetSel(0, -1);
		return FALSE;
	}
	

	if ( d < 1 || d > max_day[m-1]) {
		frm.Format(_T("일에 1 ~ %d사이의 값을 선택하십시오."), max_day[m-1] );
		MessageBox(frm, _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		((CEdit*)GetDlgItem(IDC_DAY))->SetFocus();
		((CEdit*)GetDlgItem(IDC_DAY))->SetSel(0, -1);
		SetDay(_T(""));
		return FALSE;
	}

	return TRUE;
EFS_END
	return 0;
}
INT CECO_ControlDateEdit::EcoMoveControl( INT x, INT y, INT cx, INT xy)
{
EFS_BEGIN 
MoveWindow( x, y, cx,  xy );
    return 0;
EFS_END
	return 0;
}
VOID CECO_ControlDateEdit::EcoClear()
{
EFS_BEGIN 

EFS_END
}

VOID CECO_ControlDateEdit::Clear()
{
EFS_BEGIN 
	SetYear ( _T("") );
	SetMonth( _T("") );
	SetDay  ( _T("") );
EFS_END
}

INT CECO_ControlDateEdit::EcoModifyStyle( INT lStyle , BOOL bSetBits)
{
EFS_BEGIN 
if ( (lStyle & ECOS_READONLY ) == ECOS_READONLY ) {
        ECO_SetReadOnly( TRUE );
    } else {
        ECO_SetReadOnly( FALSE );
    }

    if ( (lStyle & ECOS_DISABLE ) == ECOS_DISABLE ) {
        ECO_EnableWindow( FALSE );
    } 
    return 0;
EFS_END
	return 0;
}

#define ISDIGIT(x) ((x)>='0' && (x) <='9')
#define ISSLASH(x) ((x)=='/')

#define YEAR_INDEX 0
#define YEAR_LEN 4
#define MONTH_INDEX 5
#define MONTH_LEN 2
#define DAY_INDEX 8
#define DAY_LEN 2

#define SLASH1_INDEX 4
#define SLASH2_INDEX 7

BOOL is_YYYYMMDD(LPCTSTR lpszDateYYYYMMDD)
{
EFS_BEGIN
INT i;
	if (!ISSLASH(lpszDateYYYYMMDD[SLASH1_INDEX])) return FALSE;
	if (!ISSLASH(lpszDateYYYYMMDD[SLASH2_INDEX])) return FALSE;
	for (i=YEAR_INDEX; i<YEAR_INDEX+YEAR_LEN; i++) {
		if (!ISDIGIT(lpszDateYYYYMMDD[i])) return FALSE;
	}
	for (i=MONTH_INDEX; i<MONTH_INDEX+MONTH_LEN; i++) {
		if (!ISDIGIT(lpszDateYYYYMMDD[i])) return FALSE;
	}
	for (i=DAY_INDEX; i<DAY_INDEX+DAY_LEN; i++) {
		if (!ISDIGIT(lpszDateYYYYMMDD[i])) return FALSE;
	}
	return TRUE;
EFS_END
}


BOOL convert_to_MMDDYYYY(LPCTSTR lpszDstMMDDYYYY, LPCTSTR lpszSrcYYYYMMDD)
{
EFS_BEGIN

	TCHAR szResultMMDDYYYY[DATE_FORMAT_LEN+1]={0,};
	if (!is_YYYYMMDD(lpszSrcYYYYMMDD)) return FALSE;
	_tcsncat(szResultMMDDYYYY, lpszSrcYYYYMMDD+MONTH_INDEX,MONTH_LEN);
	_tcscat(szResultMMDDYYYY, _T("/"));
	_tcsncat(szResultMMDDYYYY, lpszSrcYYYYMMDD+DAY_INDEX,DAY_LEN);
	_tcscat(szResultMMDDYYYY, _T("/"));
	_tcsncat(szResultMMDDYYYY, lpszSrcYYYYMMDD+YEAR_INDEX,YEAR_LEN);
	szResultMMDDYYYY[DATE_FORMAT_LEN] = '\0';
	_tcscpy((TCHAR*)lpszDstMMDDYYYY, szResultMMDDYYYY);
	return TRUE;
EFS_END
	return 0;
}

BOOL convert_to_YYYYMMDD(LPCTSTR lpszDstYYYYMMDD, LPCTSTR lpszSrcMMDDYYYY)
{
EFS_BEGIN
	TCHAR szResultYYYYMMDD[DATE_FORMAT_LEN+1]={0,};
	_tcsncat(szResultYYYYMMDD, lpszSrcMMDDYYYY+6,YEAR_LEN);
	_tcscat(szResultYYYYMMDD, _T("/"));
	_tcsncat(szResultYYYYMMDD, lpszSrcMMDDYYYY+0,MONTH_LEN);
	_tcscat(szResultYYYYMMDD, _T("/"));
	_tcsncat(szResultYYYYMMDD, lpszSrcMMDDYYYY+3,DAY_LEN);
	szResultYYYYMMDD[DATE_FORMAT_LEN] = '\0';
	_tcscpy((TCHAR*)lpszDstYYYYMMDD, szResultYYYYMMDD);
	return TRUE;
EFS_END
}

INT CECO_ControlDateEdit::Display(INT index)
{
	CString link_data = ELEMENT->LINK_DATA;
	if (link_data.GetLength() == 0) return 0;
	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;
		CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
		pMgr->GetDataMgrData(link_data.Mid(1, mgrpos-1), link_data.Mid(mgrpos+1), data, index);
		SetData(data);
	}
	return 0;
}

INT CECO_ControlDateEdit::SetYear  ( INT data )
{
	CString text;
	text.Format(_T("%d"), data);
	SetYear(text);
	return 0;
}

INT CECO_ControlDateEdit::SetMonth ( INT data )
{
	CString text;
	text.Format(_T("%d"), data);
	SetMonth(text);
	return 0;
}

INT CECO_ControlDateEdit::SetDay   ( INT data )
{
	CString text;
	text.Format(_T("%d"), data);
	SetDay(text);
	return 0;
}

INT CECO_ControlDateEdit::SetYear  ( CString text )
{
    GetDlgItem(IDC_YEAR)->SetWindowText(text);
	return 0;
}

INT CECO_ControlDateEdit::SetMonth ( CString text )
{
    GetDlgItem(IDC_MONTH)->SetWindowText(text);
	return 0;
}

INT CECO_ControlDateEdit::SetDay ( CString text )
{
    GetDlgItem(IDC_DAY)->SetWindowText(text);
	return 0;
}

INT CECO_ControlDateEdit::SetData( CString data )
{
	SetYear(_T(""));
	SetMonth(_T(""));
	SetDay(_T(""));

	INT nPos = data.Find(' ', 0);
	if (nPos >= 0) {
		CString strTmp;
		strTmp = data.Left(nPos);
		data = strTmp;
		strTmp.TrimLeft();				strTmp.TrimRight();
	}
	data.Replace(_T("-"), _T("/"));

	CString year, month, day;
	INT PrevPosition, CursorPosition;
	CursorPosition = data.Find(_T("/"), 0);
	if (CursorPosition == -1) return -1;
	year = data.Mid(0, CursorPosition);
	SetYear(year);
	PrevPosition = CursorPosition;
	CursorPosition = data.Find(_T("/"), PrevPosition+1);
	if (CursorPosition == -1) return -1;
	month = data.Mid(PrevPosition+1, CursorPosition-PrevPosition-1);
	SetMonth(month);
	day = data.Mid(CursorPosition+1);
	SetDay(day);

	return 0;
}

INT CECO_ControlDateEdit::GetYear( CString & text)
{
EFS_BEGIN
    GetDlgItem(IDC_YEAR)->GetWindowText(text);
	if (text.IsEmpty()) {
		text = _T("0000");
		return -1;
	}
	return 0;
EFS_END
	return 0;
}

INT CECO_ControlDateEdit::GetMonth( CString & text)
{
EFS_BEGIN
    GetDlgItem(IDC_MONTH)->GetWindowText(text);
	if (text.IsEmpty()) {
		text = _T("00");
		return -1;
	}
	return 0;
EFS_END
	return 0;
}

INT CECO_ControlDateEdit::GetDay( CString & text )
{
EFS_BEGIN
    GetDlgItem(IDC_DAY)->GetWindowText(text);
	if (text.IsEmpty()) {
		text = _T("00");
		return -1;
	}
	return 0;
EFS_END
	return 0;
}

INT CECO_ControlDateEdit::GetData( CString &data )
{
	CString year, month, day;
	INT ids_Y, ids_M, ids_D;
	ids_Y = GetYear(year);
	ids_M = GetMonth(month);
	ids_D = GetDay(day);

	if (ids_Y == -1 || ids_M == -1 || ids_D == -1) {
		data = _T("");
		return 0;
	}

	data.Format(_T("%04d/%02d/%02d"), _ttoi(year), _ttoi(month), _ttoi(day));

	return 0;
}

INT CECO_ControlDateEdit::GetYear  ( VOID )
{
	CString text;
    GetDlgItem(IDC_YEAR)->GetWindowText(text);
	if (text.GetLength() == 0) return 0;
	return _ttoi(text);
}

INT CECO_ControlDateEdit::GetMonth ( VOID )
{
	CString text;
    GetDlgItem(IDC_MONTH)->GetWindowText(text);
	if (text.GetLength() == 0) return 0;
	return _ttoi(text);
}

INT CECO_ControlDateEdit::GetDay   ( VOID )
{
	CString text;
    GetDlgItem(IDC_DAY)->GetWindowText(text);
	if (text.GetLength() == 0) return 0;
	return _ttoi(text);
}


HBRUSH CECO_ControlDateEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (RequireFlag) {
		if (pWnd->GetDlgCtrlID() == IDC_YEAR || pWnd->GetDlgCtrlID() == IDC_MONTH || pWnd->GetDlgCtrlID() == IDC_DAY) {
			pDC->SetBkMode(OPAQUE );
			pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
			return m_BkBrush;		
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CECO_ControlDateEdit::SetElemEncryption(TCHAR ch)
{
	EFS_BEGIN
	m_Elem[0].SetPasswordChar(ch);
	m_Elem[1].SetPasswordChar(ch);
	m_Elem[2].SetPasswordChar(ch);
	EFS_END
}