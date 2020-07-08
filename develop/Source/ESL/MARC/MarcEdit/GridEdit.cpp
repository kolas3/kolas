// GridEdit.cpp : implementation file
//

#include "stdafx.h"
#include "GridEdit.h"
#include "MarcEditCtrl.h"
#include "..\..\ESL\ConvertToHanjaDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridEdit

CGridEdit::CGridEdit()
{
	m_nEditMode = editEasy;
}

CGridEdit::~CGridEdit()
{
}


BEGIN_MESSAGE_MAP(CGridEdit, CEdit)
	//{{AFX_MSG_MAP(CGridEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridEdit message handlers

BOOL CGridEdit::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) 
	{
		//===================================================
		//2009.05.06 UPDATE BY PJW : "CTRL+S"일경우는 수정하지 않는다.
		CString strKey;
		
		if ( GetKeyState(VK_CONTROL) < 0 )
		{
			strKey.Format(_T("CTRL+%c"), pMsg->wParam);
			if (strKey == _T("CTRL+S")) 
			{
				NotifyCancel();
				return false;
			}
		}
		//===================================================
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyModify(0);
			return true;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return true;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

VOID CGridEdit::OnKillfocus() 
{
	NotifyModify(0);
}

VOID CGridEdit::NotifyModify(UINT nChar)
{
	::PostMessage(parent->GetSafeHwnd(), WM_GRID_EDIT_MODIFY, 1, nChar);
}

VOID CGridEdit::NotifyCancel()
{
	::PostMessage(parent->GetSafeHwnd(), WM_GRID_EDIT_MODIFY, 0, 0);
}

VOID CGridEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nEditMode == editEasy)
	{
		if (nChar == VK_UP || nChar == VK_DOWN)
		{
			NotifyModify(nChar);
		}
	}


	if (nChar == VK_F6)
	{

		CString strTemp, strSelText;

		INT iStrStart = 0, iStrEnd = 0;
		CEdit::GetSel(iStrStart, iStrEnd);
		CEdit::GetWindowText(strTemp);

		strSelText = strTemp.Mid(iStrStart, iStrEnd-iStrStart);

		CConvertToHanjaDlg dlg;
		dlg.m_strInput = strSelText;
		
		if (dlg.DoModal() == IDOK)
		{
			if (!dlg.m_strSelChi.IsEmpty())
			{
				ReplaceSel(dlg.m_strSelChi, TRUE);
			}
		}

	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
