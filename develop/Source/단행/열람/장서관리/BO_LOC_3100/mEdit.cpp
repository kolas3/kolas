#include "stdafx.h"
#include "mEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CmEdit
//		A masked edit control that is based on CEdit
//		allows masks to be on a per-character basis, so you set a mask for
//		each character in the control

// syncs data between controls and their members using DDX
VOID AFXAPI DDX_CmEdit(CDataExchange* pDX, INT nIDC, CmEdit& rControl, CString& data)
{
	DDX_Control(pDX, nIDC, (CWnd&)rControl);


	if (pDX->m_bSaveAndValidate)
		data = rControl.GetData();
	else
		rControl.SetData(data);
} // end VOID AFXAPI DDX_CmEdit(CDataExchange* pDX, INT nIDC, CmEdit& rControl, CString& data)

IMPLEMENT_DYNCREATE(CmEdit, CEdit)


CmEdit::CmEdit()
{
	chPrompt = _T('_');
	isShift = false;
	isCtrl = false;
	bLower = false;
	bUpper = false;
	bCopyAll = false;
	chPassword = NULL;
} // end CmEdit::CmEdit()

CmEdit::~CmEdit()
{
} // end CmEdit::~CmEdit()


BEGIN_MESSAGE_MAP(CmEdit, CEdit)
	//{{AFX_MSG_MAP(CmEdit)
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


VOID CmEdit::SetPrompt(TCHAR prompt)
{
	chPrompt = prompt;

	// update the control
	SetWindowText(NULL);
} // end VOID CmEdit::SetPrompt(TCHAR prompt)

VOID CmEdit::SetMask(CString mask)
{
	INT index, maskLen = mask.GetLength();
	INT start;
	CmEditChar tempCh;
	bool inMask = false;


	// empty the old mask
	arrChars.RemoveAll();

	// for each character of the mask..
	for (index = 0; index < maskLen; index++)
	{
		// set the character up with defaults
		tempCh.setMasked(false);
		tempCh.strAllowedChars.Empty();
		tempCh.chValue = 0;


		// if we are inside of a mask entry []
		if (inMask)
		{
			tempCh.setMasked();

			// go until we find a ] or the end of the mask and store each TCHAR or range
			while (mask[index] != _T(']') && index < maskLen)
			{
				if (mask[index + 1] == _T('-') && mask[index] != _T('\\') && (index + 2) < maskLen)
				{
					for (start = mask[index]; start <= mask[index + 2]; start++)
						tempCh.strAllowedChars += (TCHAR)start;

					index++; index++;
				} else
					if (mask[index] == _T('\\') && (index + 1) < maskLen && mask[index + 1] == _T(']'))
						tempCh.strAllowedChars += mask[++index];
					else
						tempCh.strAllowedChars += mask[index];

				index++;
			} // end while (mask[index] != _T(']') && index < maskLen)

			inMask = false;
		} else
			// start of a mask entry
			if (mask[index] == _T('[') && (index == 0 || mask[index - 1] != _T('\\')))
				inMask = true;
			// a static character
			else
				if (mask[index] == _T('\\') && index + 1 < maskLen && mask[index + 1] == _T('['))
					tempCh.chValue = mask[++index];
				else
					tempCh.chValue = mask[index];

		if (!inMask)
			arrChars.Add(tempCh);
	} // end for (index = 0; index < maskLen; index++)

	// update the control
	SetWindowText(NULL);
} // end VOID CmEdit::SetMask(CString mask)

VOID CmEdit::SetWindowText(LPCTSTR lpszString)
{
	INT index, start, end;
	CString temp;


	GetSel(start, end);

	// if the string is null we just want to update the editbox
	// with the current text, otherwise we want to replace it
	// so you clear the old data and 'type' in the new data
	if (lpszString != NULL)
	{
		Clear();

		// sets the text (simulates the user typing it in)
		SetSel(0, arrChars.GetSize());
		ReplaceSel(lpszString);
	} // end if (lpszString != NULL)


	// generate the string using the array
	for (index = 0; index < arrChars.GetSize(); index++)
		if (arrChars.GetData()[index].chValue == 0)
			temp += chPrompt;
		else
			temp += (chPassword != NULL && arrChars.GetData()[index].Masked()) ? chPassword : arrChars.GetData()[index].chValue;

	// update the control
	CEdit::SetWindowText(temp);


	// reposition the selection to where it was before
	if (start > temp.GetLength())
		start = end = temp.GetLength();

	if (end > temp.GetLength())
		end = temp.GetLength();

	SetSel(start, end);
} // end VOID CmEdit::SetWindowText(LPCTSTR lpszString)

VOID CmEdit::SetData(CString data)
{
	if (data.GetLength() == 0)
		return;

	SetWindowText(data);
} // end VOID CmEdit::SetData(CString data)

CString CmEdit::GetData(INT start, INT len)
{
	CString temp;


	// make sure starting point isnt past the end or negative
	if (start >= arrChars.GetSize() || start < 0)
		start = 0;

	// return to the end if len is -1
	if (start + len > arrChars.GetSize() || len < 0)
		len = arrChars.GetSize() - start;

	// make the string
	for (INT index = start; index < start + len; index++)
		if (bCopyAll)
			temp += (arrChars.GetData()[index].chValue == 0) ? chPrompt : arrChars.GetData()[index].chValue;
		else
			if (arrChars.GetData()[index].Masked() && arrChars.GetData()[index].chValue != 0)
				temp += arrChars.GetData()[index].chValue;

	return temp;
} // end CString CmEdit::GetData(INT start, INT len)

VOID CmEdit::SetPasswordChar(TCHAR ch)
{
	chPassword = ch;
	SetWindowText(NULL);
} // end VOID CmEdit::SetPasswordChar(TCHAR ch)

VOID CmEdit::Paste()
{
	CString strFromClipboard;
	HANDLE hData;
	INT start, end;


	if (!OpenClipboard())
		return;

	// only retrieve text (CF_TEXT)
	hData = GetClipboardData(CF_TEXT);

	// Couldn't get data from clipboard
	if (hData == NULL)
		return;

	// get clipboard text
	strFromClipboard = (TCHAR*)GlobalLock(hData);
	GlobalUnlock(hData);

	CloseClipboard();

	ReplaceSel(strFromClipboard);
	
	GetSel(start, end);
	SetMaskedSel(end + 1);
} // end VOID CmEdit::Paste()

VOID CmEdit::Cut()
{
	INT start, end;


	GetSel(start, end);

	if (start == end)
		return;

	if (chPassword != NULL)
		return;

	// copy the selected text
	Copy();

	// replaces masked chars in the selection with prompt chars
	for (; end >= start; end--)
		if (arrChars.GetData()[end].Masked())
			arrChars.GetData()[end].chValue = 0;

	// updates the control
	SetWindowText(NULL);
	SetMaskedSel(start);
} // end VOID CmEdit::Cut()

VOID CmEdit::Copy()
{
	CString temp;
	INT start, end;
	CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);


	if (chPassword != NULL)
		return;

	if (!OpenClipboard() || !EmptyClipboard())
		return;
	
	GetSel(start, end);
	
	if (start == end)
		return;

	// get the selected text
	temp = GetData(start, end - start);

	if (temp.GetLength() == 0)
		return;

	// store it in a shared memory file
	sf.Write(temp, temp.GetLength());

	// set the text in the clipboard
 	::SetClipboardData(CF_TEXT, sf.Detach());

	CloseClipboard();
} // end VOID CmEdit::Copy()

VOID CmEdit::Clear()
{
	INT index, textlen = arrChars.GetSize();


	// replaces masked chars with prompt chars
	for (index = 0; index < textlen; index++)
		if (arrChars.GetData()[index].Masked())
			arrChars.GetData()[index].chValue = 0;

	// updates the control
	SetWindowText(NULL);
	SetMaskedSel(0);
} // end VOID CmEdit::Clear()

VOID CmEdit::SetSel(INT nStartChar, INT nEndChar, BOOL bNoScroll)
{
	// this will find the first masked TCHAR to the left starting at start
	if (nEndChar < nStartChar)
		for (; nStartChar >= 0 && !arrChars.GetData()[nStartChar].Masked(); nStartChar--)
			;

	// this will select the first masked TCHAR starting at start
	if (nStartChar == nEndChar)
		for (; nStartChar < arrChars.GetSize() && !arrChars.GetData()[nStartChar].Masked(); nStartChar++)
			;

	// just move the cursor if end < start
	if (nEndChar < nStartChar)
		nEndChar = nStartChar;

	// select the text
	CEdit::SetSel(nStartChar, nEndChar, bNoScroll);
} // end VOID CmEdit::SetSel(INT nStartChar, INT nEndChar, BOOL bNoScroll)

VOID CmEdit::SetMaskedSel(INT begin)
{
	INT start, end, textlen = arrChars.GetSize();


	// if you are trying to set a mask at the end of the string, set it at the beginning instead
	if (begin == textlen)
		begin = 0;

	// find the first masked TCHAR starting at begin
	for (start = begin; start < textlen && !arrChars.GetData()[start].Masked(); start++)
		;

	// find the first non-masked TCHAR after start
	for (end = start; end < textlen && arrChars.GetData()[end].Masked(); end++)
		;

	// select that region
	SetSel(start, end);
} // end VOID CmEdit::SetMaskedSel(INT begin)

VOID CmEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
{
	INT start, end;
	UINT index, index2;


	GetSel(start, end);

	if (start == arrChars.GetSize())
		return;

	// if nothing is selected, just insert starting at this point
	if (start == end)
		end = arrChars.GetSize();

	// this simulates the user typing in the characters
	for (index = (UINT)start, index2 = 0; index < (UINT)end && index2 < _tcsclen(lpszNewText); index++)
		if (arrChars.GetData()[index].Masked())
		{
			while (index2 < _tcsclen(lpszNewText) && arrChars.GetData()[index].strAllowedChars.Find(lpszNewText[index2]) == -1 && lpszNewText[index2] != 0)
				index2++;

			if (index2 < _tcsclen(lpszNewText))
			{
				arrChars.GetData()[index].chValue = lpszNewText[index2++];

				// convert all uppercase chars to lowercase
				if (bLower)
					arrChars.GetData()[index].chValue = _totlower(arrChars.GetData()[index].chValue);

				// convert all lowercase chars to uppercase
				if (bUpper)
					arrChars.GetData()[index].chValue = _totupper(arrChars.GetData()[index].chValue);
			} // end if (index2 < _tcsclen(lpszNewText))
		} // end if (arrChars.GetData()[index].Masked())
		
	SetMaskedSel(start);

	// updates the control
	SetWindowText(NULL);
} // end VOID CmEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)


/////////////////////////////////////////////////////////////////////////////
// CmEdit message handlers

VOID CmEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// select the first masked portion
	SetMaskedSel(0);
} // end VOID CmEdit::OnSetFocus(CWnd* pOldWnd)

VOID CmEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	INT start, end;


	GetSel(start, end);

	if (start == arrChars.GetSize() || nChar == 8)
		return;

	// find the first masked TCHAR starting at the current position
	for (; start < arrChars.GetSize() && !arrChars.GetData()[start].Masked(); start++)
		;
	
	ReplaceSel((CString)nChar);

	// if we actually replaced the TCHAR, then select the remainder of this masked region
	if (arrChars.GetData()[start].strAllowedChars.Find(nChar) != -1)
	{
		start++;

		if (end < start)
			end++;

		SetMaskedSel(start);
	} // end if (arrChars.GetData()[start].strAllowedChars.Find(nChar) != -1)
} // end VOID CmEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)

VOID CmEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	INT start, end;


	GetSel(start, end);

	switch (nChar)
	{
		// move 1 masked TCHAR to the left if shift isnt down
		case VK_UP:
		case VK_LEFT:
			if (isShift)
				CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			else
				if (isCtrl)
				{
					// with ctrl down we need to find the previous masked range and select it
					for (; start > 0 && arrChars.GetData()[start].Masked(); start--)
						;
					for (; start > 0 && !arrChars.GetData()[start].Masked(); start--)
						;

					// if there isnt a previous one, select the last one
					if (start == 0)
					{
						for (start = arrChars.GetSize(); start > 0 && !arrChars.GetData()[start].Masked(); start--)
							;
						for (start = arrChars.GetSize(); start > 0 && arrChars.GetData()[start].Masked(); start--)
							;
					} else
						for (; start > 0 && arrChars.GetData()[start].Masked(); start--)
							;

					SetMaskedSel(start);
				} else
				{
					if (start == end && start != 0)
						start--;
					SetSel(start, -1);
				} // end if (isCtrl)
			break;

		// move 1 masked TCHAR to the right if shift isnt down
		case VK_DOWN:
		case VK_RIGHT:
			if (isShift)
				CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			else
				if (isCtrl)
				{
					// with ctrl down we need to find the next masked range and select it
					for (; end < arrChars.GetSize() && arrChars.GetData()[end].Masked(); end++)
						;

					SetMaskedSel(end);
				} else
				{
					if (start == end && end < arrChars.GetSize())
						end++;
					SetSel(end, end);
				} // end if (isCtrl)
			break;

		// move the the first masked region
		case VK_HOME:
			if (isShift)
				CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			else
				SetMaskedSel(0);
			break;

		// move to the last masked region
		case VK_END:
			if (isShift)
				CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			else
			{
				for (start = arrChars.GetSize() - 1; start >= 0 && !arrChars.GetData()[start].Masked(); start--)
					;
				for (; start >= 0 && arrChars.GetData()[start].Masked(); start--)
					;
				SetMaskedSel(start);
			} // end if (isShift)
			break;

		case VK_PRIOR:
			// we need to find the previous masked range and select it
			for (; start > 0 && arrChars.GetData()[start].Masked(); start--)
				;
			for (; start > 0 && !arrChars.GetData()[start].Masked(); start--)
				;

			// if there isnt a previous one, select the last one
			if (start == 0)
			{
				for (start = arrChars.GetSize(); start > 0 && !arrChars.GetData()[start].Masked(); start--)
					;
				for (start = arrChars.GetSize(); start > 0 && arrChars.GetData()[start].Masked(); start--)
					;
			} else
				for (; start > 0 && arrChars.GetData()[start].Masked(); start--)
					;

			SetMaskedSel(start);
			break;

		case VK_NEXT:
			// we need to find the next masked range and select it
			for (; end < arrChars.GetSize() && arrChars.GetData()[end].Masked(); end++)
				;

			SetMaskedSel(end);
			break;

		// deletes the masked TCHAR to the left, or the selected range
		case VK_BACK:
			if (start == end && start > 0)
			{
				for (start--; start > 0 && !arrChars.GetData()[start].Masked(); start--)
					;

				if (arrChars.GetData()[start].Masked())
					arrChars.GetData()[start].chValue = 0;
			} else
				for (end--; end >= start; end--)
					if (arrChars.GetData()[end].Masked())
						arrChars.GetData()[end].chValue = 0;
			SetWindowText(NULL);
			SetSel(start, start);
			break;

		// deletes the masked TCHAR to the right, or the selected range
		case VK_DELETE:
			if (start == end && arrChars.GetData()[start].Masked())
			{
				arrChars.GetData()[start].chValue = 0;
			} else
				for (end--; end >= start; end--)
					if (arrChars.GetData()[end].Masked())
						arrChars.GetData()[end].chValue = 0;
			SetWindowText(NULL);
			SetSel(start, start);
			break;

		// the shift key is down
		case VK_SHIFT:
			isShift = true;
			break;

		case VK_CONTROL:
			isCtrl = true;
			break;

		default:
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			break;
	} // end switch (nChar)
} // end VOID CmEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

VOID CmEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	INT start, end;


	switch (nChar)
	{
		// the shift key is no longer down
		case VK_SHIFT:
			isShift = false;
			break;

		case VK_CONTROL:
			isCtrl = false;
			break;

		// on enter key, select the next masked region or if already in the
		// last region try to move to the next control
		case VK_RETURN:
			GetSel(start, end);
			SetMaskedSel(start);
			GetSel(start, end);

			// if we are already at the end of the box, attempt to move focus
			// to the next dialog control window or if the next window is this one,
			// select the first masked region
			if (end + 1 > arrChars.GetSize())
			{
				if (GetParent()->GetNextDlgTabItem(this) == this)
					SetMaskedSel(0);
				else
					GetParent()->GetNextDlgTabItem(this)->SetFocus();
			} else
				SetMaskedSel(end + 1);
			break;

		default:
			CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	} // end switch (nChar)
} // end VOID CmEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)

VOID CmEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	INT start, end;


	CEdit::OnLButtonDown(nFlags, point);
	
	// select the first masked TCHAR after the TCHAR clicked on
	GetSel(start, end);
	SetSel(start, start);
} // end VOID CmEdit::OnLButtonDown(UINT nFlags, CPoint point)

VOID CmEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	INT start, end;


	CEdit::OnLButtonUp(nFlags, point);

	GetSel(start, end);

	if (start != end)
		for (; end > start; end--)
			if (!arrChars.GetData()[end].Masked())
			{
				SetMaskedSel(start);
				end = start;
			} // end if (!arrChars.GetData()[end].Masked())
} // end VOID CmEdit::OnLButtonUp(UINT nFlags, CPoint point)

VOID CmEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	INT start, end;


	GetSel(start, end);

	// find the first masked TCHAR to the left of the double click and select that masked region
	for (; start >= 0 && arrChars.GetData()[start].Masked(); start--)
		;

	SetMaskedSel((start < 0) ? 0 : start);
} // end VOID CmEdit::OnLButtonDblClk(UINT nFlags, CPoint point)

VOID CmEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	// disable the contex-menu for this edit box
} // end VOID CmEdit::OnRButtonUp(UINT nFlags, CPoint point)
