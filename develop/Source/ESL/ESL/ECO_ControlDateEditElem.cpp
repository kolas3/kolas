// ECO_DateEditElem.cpp : implementation file
//

#include "stdafx.h"
#include "ECO_ControlDateEditElem.h"
#include "ECO_ControlDateEdit.h"
#include "except.h"
#include "resource.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
const TCHAR szDialogClass[] = _T("#32770");			// Special window class for dialogs

/////////////////////////////////////////////////////////////////////////////
// CECO_ControlDateEditElem

CECO_ControlDateEditElem::CECO_ControlDateEditElem()
{

}

CECO_ControlDateEditElem::~CECO_ControlDateEditElem()
{

}

BEGIN_MESSAGE_MAP(CECO_ControlDateEditElem, CEdit)
	//{{AFX_MSG_MAP(CECO_ControlDateEditElem)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECO_ControlDateEditElem message handlers
VOID CECO_ControlDateEditElem::SetParent(CECO_ControlDateEdit* pParent)
{
EFS_BEGIN
m_pParent = pParent;
EFS_END
}

VOID CECO_ControlDateEditElem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
			case IDC_YEAR  : m_pParent->OnChildChar('y', 0, nFlags, *this); break;
			case IDC_MONTH : m_pParent->OnChildChar('m', 0, nFlags, *this); 
				break;
			case IDC_DAY  : m_pParent->OnChildChar('d', 0, nFlags, *this); break;
			}
		}
	}
	else
		::MessageBeep(0xFFFFFFFF);
	
EFS_END
}

VOID CECO_ControlDateEditElem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

		// Clear out window class name arrays
		wmemset(cClass1, 0, sizeof(cClass1));
		wmemset(cClass2, 0, sizeof(cClass2));

		// Get parent and parent's parent window pointers
		CWnd* pDialog = m_pParent->GetParent();		// Get parent of the IP address edit control
		CWnd* pDialogParent = pDialog->GetParent();	// Get its parent

		// Get class names of the windows that own the IP address edit control and its parent
		if (NULL != pDialog)
			::GetClassName(pDialog->m_hWnd, cClass1, sizeof(cClass1)/sizeof(TCHAR));
		if (NULL != pDialogParent)
			::GetClassName(pDialogParent->m_hWnd, cClass2, sizeof(cClass2)/sizeof(TCHAR));

		// If parent is a dialog, and parent's parent is a dialog, simulate Cancel button press
		if (0 == _tcscmp(cClass1, szDialogClass) && 0 == _tcscmp(cClass2, szDialogClass))
		{
			CWnd* pCancel = pDialogParent->GetDlgItem(IDCANCEL);	// Get the Cancel button
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

VOID CECO_ControlDateEditElem::OnKillFocus( CWnd* pNewWnd )
{
EFS_BEGIN
    CWnd* pDialog;
	CEdit::OnKillFocus( pNewWnd );

	if (pNewWnd!=NULL) {
		if (m_pParent->GetDay() == 0 && m_pParent->GetMonth() == 0  && m_pParent->GetYear() == 0 ) return; 

		if (pNewWnd->GetParent()!=GetParent() &&  m_pParent->IsWindowVisible() == TRUE) 
		{
			if (m_pParent->CheckYear()==TRUE) 
			{
				if (m_pParent->CheckMonth()==TRUE) 
				{
					if(m_pParent->CheckDay()==TRUE) 
					{
						pDialog = m_pParent->GetParent();
						if ( NULL == pDialog ) return;
						if(m_pParent->ELEMENT)
						{
							pDialog->PostMessage(
								  WM_COMMAND,
								  MAKEWPARAM(m_pParent->ELEMENT->CTRL_ID, EN_KILLFOCUS ),
								  (LPARAM) (NULL != m_pParent->m_hWnd ? m_pParent->m_hWnd : NULL)
								);
						}
					}
				}
			}
			return;
		}
	}

	if ( IDC_YEAR == GetDlgCtrlID() || IDC_MONTH == GetDlgCtrlID()) {

		pDialog = m_pParent->GetParent();
		if ( NULL == pDialog ) return;

	}

EFS_END
}


