#include "stdafx.h"
#include "HourEditElem.h"
#include "HourEditControl.h"
#include "resource.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR szDialogClass[] = _T("#32770");			

CHourEditElem::CHourEditElem()
{
}

CHourEditElem::~CHourEditElem()
{
}


BEGIN_MESSAGE_MAP(CHourEditElem, CEdit)
	//{{AFX_MSG_MAP(CHourEditElem)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CHourEditElem::SetParent(CHourEditControl* pParent)
{
EFS_BEGIN
m_pParent = pParent;
EFS_END
}

VOID CHourEditElem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
EFS_BEGIN
    if (VK_TAB == nChar || '/' == nChar || ' ' == nChar || VK_RETURN == nChar )
	    m_pParent->OnChildChar(nChar, nRepCnt, nFlags, *this);
	else if (('0' <= nChar && '9'>= nChar) || iscntrl(nChar))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);

		if ((INT)GetLimitText() == (INT)GetWindowTextLength())
		{
			switch( GetDlgCtrlID() )
			{
			case IDC_HOUR : m_pParent->OnChildChar('h', 0, nFlags, *this); break;
			case IDC_MIN  : m_pParent->OnChildChar('m', 0, nFlags, *this); break;
			case IDC_SEC  : m_pParent->OnChildChar('s', 0, nFlags, *this); break;
			}
		}
	}
	else
		::MessageBeep(0xFFFFFFFF);
EFS_END
}

VOID CHourEditElem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
EFS_BEGIN
if (VK_LEFT == nChar || VK_RIGHT == nChar)
	{
		CPoint ptCaret = GetCaretPos();
		INT nCharPos = LOWORD(CharFromPos(ptCaret));
		if ((VK_LEFT == nChar && nCharPos == 0) ||
			(VK_RIGHT == nChar && nCharPos == GetWindowTextLength()))
			m_pParent->OnChildChar(nChar, nRepCnt, nFlags, *this);
	}
	else if (VK_ESCAPE == nChar)
	{
		TCHAR	cClass1[12];
		TCHAR	cClass2[12];

		
		wmemset(cClass1, 0, sizeof(cClass1));
		wmemset(cClass2, 0, sizeof(cClass2));

		
		CWnd* pDialog = m_pParent->GetParent();		
		CWnd* pDialogParent = pDialog->GetParent();	

		
		if (NULL != pDialog)
			::GetClassName(pDialog->m_hWnd, cClass1, sizeof(cClass1)/sizeof(TCHAR));
		if (NULL != pDialogParent)
			::GetClassName(pDialogParent->m_hWnd, cClass2, sizeof(cClass2)/sizeof(TCHAR));

		
		if (0 == _tcscmp(cClass1, szDialogClass) && 0 == _tcscmp(cClass2, szDialogClass))
		{
			CWnd* pCancel = pDialogParent->GetDlgItem(IDCANCEL);	
			if ( pCancel == NULL ) return;
			pDialogParent->SendMessage(WM_COMMAND,
				MAKEWPARAM(IDCANCEL, BN_CLICKED),
				(LPARAM) (NULL != pCancel->m_hWnd ? pCancel->m_hWnd : NULL));
			return;
		}
	}
	else if (VK_BACK == nChar)
	{
		if ( GetWindowTextLength() == 0 )
			m_pParent->OnChildChar(nChar, nRepCnt, nFlags, *this);
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	
EFS_END
}

VOID CHourEditElem::OnKillFocus( CWnd* pNewWnd )
{
EFS_BEGIN
    CWnd* pDialog;
	CEdit::OnKillFocus( pNewWnd );

	if (pNewWnd!=NULL) {
		if (m_pParent->GetSec() == -1 && m_pParent->GetMin() == -1  && m_pParent->GetHour() == -1 ) return; 
		
			
		
		if (pNewWnd->GetParent()!=GetParent() &&  m_pParent->IsWindowVisible() == TRUE) 
		
		{
			if (m_pParent->CheckHour()==TRUE) {
				if (m_pParent->CheckMin()==TRUE) {
					m_pParent->CheckSec();
				}
			}
			return;
		}
	}

	if ( IDC_HOUR == GetDlgCtrlID() || IDC_MIN == GetDlgCtrlID()) {
		pDialog = m_pParent->GetParent();
		if ( NULL == pDialog ) return;
	}

EFS_END
}