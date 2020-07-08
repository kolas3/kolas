// ListOfTexObject.cpp: implementation of the CListOfTexObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecomath.h"
#include "ListOfTexObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOfTexObject::CListOfTexObject()
{
	m_Top = NULL;
	m_Current = NULL;
	m_uNumberOfObject = 0;
}

CListOfTexObject::~CListOfTexObject()
{

}

VOID CListOfTexObject::InsertTexObject(CTexObject *newObject)
{

	if(m_Current != NULL) {
		if((m_Current ->cell) ->GetObjectType() == BLANK_OBJECT) {
			CString msg = (m_Current ->cell) ->GetObject();
			if(msg.GetLength() > 0) {
				ReplaceAtCurrent(newObject);
				return;
			}
		}
	}

	struct _Cell *newCell = (struct _Cell *)malloc(sizeof(struct _Cell));

	newCell -> cell = newObject;
	newCell -> next = NULL;
	newCell -> prev = NULL;

	if(m_Current == NULL) { // 리스트가 비어 있음...
		m_Top = newCell;
		m_Current = newCell;
	}
	else if(m_Top == m_Current) {
		struct _Cell *tmpCell;
		tmpCell = m_Top;
		m_Top = newCell;
		m_Top ->next = tmpCell;
		tmpCell ->prev = m_Top;
		tmpCell ->next = NULL;
	}
	else {
		newCell ->next = m_Current;
		newCell ->prev = m_Current ->prev;

		if(m_Current -> prev != NULL) {
			(m_Current -> prev) ->next = newCell;
		}

		m_Current -> prev = newCell;
	}

	m_uNumberOfObject++;
}

UINT CListOfTexObject::GetNumberOfObject()
{
	return m_uNumberOfObject;
}

struct _Cell * CListOfTexObject::GetFirstCell()
{
	return m_Top;
}

struct _Cell * CListOfTexObject::GetLastCell()
{
	struct _Cell *current;

	current = m_Top;

	while(current -> next != NULL) {
		current = current ->next;
	}

	
	while((current ->cell) ->IsVisible() == false) {
		if(current ->prev == NULL) break;
		current = current ->prev;
	}

	if((current ->cell) ->IsVisible() == false) {
		AfxMessageBox(_T("Invalid Buffer[1]"));
		exit(0);
	}

	return current;
}

bool CListOfTexObject::MoveLeft()
{
	struct _Cell *backup;
	bool bOK = false;

	backup = m_Current;

	while(true) {
		if(m_Current ->prev != NULL) {
				m_Current = m_Current ->prev;
		}
		if(((m_Current ->cell) ->GetObjectType()) == BLANK_OBJECT) {
			if(((m_Current ->cell) ->GetObject()).GetLength() == 0) {
				bOK = true;
				break;
			}
		}
		else if(((m_Current ->cell) ->GetObjectType()) == FENCE_OBJECT) {
			if((((m_Current ->cell) ->GetObject()).Compare(_T(" ^")) != 0)) {
				bOK = true;
				break;
			}
		}
		else {
			bOK = true;
			break;
		}
	}

	if(bOK == true) {
		if((m_Current ->cell) ->IsVisible() == true)
			return true;

		while((m_Current ->cell) ->IsVisible() == false) {
			if(m_Current ->prev == NULL) {
				m_Current = backup;
				return true;
			}
			m_Current = m_Current ->prev;
		}

		return true;
	}
	
	m_Current = backup;
	return true;
}

bool CListOfTexObject::MoveRight()
{
	struct _Cell *backup;
	bool bOK = false;

	backup = m_Current;

	while(true) {
		if(m_Current ->next != NULL) {
				m_Current = m_Current ->next;
		}
		if(((m_Current ->cell) ->GetObjectType()) == BLANK_OBJECT) {
			if(((m_Current ->cell) ->GetObject()).GetLength() == 0) {
				bOK = true;
				break;
			}
		}
		else if(((m_Current ->cell) ->GetObjectType()) == FENCE_OBJECT) {
			if(((m_Current ->cell) ->GetObject()).Compare(_T(" ^")) != 0) {
				bOK = true;
				break;
			}
		}
		else {
			bOK = true;
			break;
		}
	}

	if(bOK == true) {
		if((m_Current ->cell) ->IsVisible() == true)
			return true;

		while((m_Current ->cell) ->IsVisible() == false) {
			if(m_Current ->next == NULL) {
				AfxMessageBox(_T("Invalid Buffer[5]"));
				exit(0);
			}
			m_Current = m_Current ->next;
		}

		if((m_Current ->cell) ->IsVisible() == false) {
			AfxMessageBox(_T("Invalid Buffer[6]"));
			exit(0);
		}

		return true;
	}
	
	m_Current = backup;
	return true;
}

VOID CListOfTexObject::MoveUp()
{
	MoveLeft();
}

VOID CListOfTexObject::MoveDown()
{
	MoveRight();
}

VOID CListOfTexObject::MoveHome()
{
	m_Current = m_Top;
}

VOID CListOfTexObject::MoveEnd()
{
	while(m_Current ->next != NULL) {
		m_Current = m_Current ->next;
	}
}

VOID CListOfTexObject::DeletePrev()
{

	if(m_Current == NULL) { // 리스트가 비어 있음
		return;
	}
	else if(m_Current == m_Top) { // 현재 위치가 리스트의 처음임
		return;
	}
//	else if((m_Current ->cell)->GetObjectType() == BLANK_OBJECT) { 
//		return;
//	}
	else {
		struct _Cell *targetCell;
		CString msg;
		targetCell = m_Current -> prev;

		if(targetCell != NULL) {
			if((targetCell ->cell)->GetObjectType() == BLANK_OBJECT) { 
				return;
			}
			else if((targetCell ->cell) ->IsVisible() == false) {
				return;
			}

			if(targetCell ->prev != NULL) {
				(targetCell ->prev) ->next = m_Current;
				m_Current ->prev = (targetCell ->prev);
			}
			else {
				m_Top = m_Current;
				m_Current ->prev = NULL;
			}
		}
		else {
			return;
		}

		delete (targetCell ->cell);
		free(targetCell);

		m_uNumberOfObject--;	
	}
}

VOID CListOfTexObject::DeleteNext()
{
	if(m_Current == NULL) { // 리스트가 비어 있음
		return;
	}
	else if(m_Current ->next == NULL) { // 현재 위치가 리스트의 마지막임
		return;
	}
	else if((m_Current ->cell)->GetObjectType() == BLANK_OBJECT) { 
		return;
	}
	else {
		struct _Cell *targetCell;
		targetCell = m_Current;	
	
		if(targetCell ->prev != NULL) {
			(targetCell ->prev) ->next = targetCell ->next;
			if(targetCell ->next != NULL) {
				(targetCell ->next) -> prev = targetCell ->prev;
			}
			m_Current = targetCell ->next;
		}
		else {
			m_Current = targetCell ->next;
			m_Current ->prev = NULL;
			m_Top = m_Current;
		}

		delete (targetCell ->cell);
		free(targetCell);

		m_uNumberOfObject--;	
	}
}

VOID CListOfTexObject::ReplaceAtCurrent(CTexObject *newObject)
{
	struct _Cell *newCell = (struct _Cell *)malloc(sizeof(struct _Cell));
	struct _Cell *tmpCell;

	newCell -> cell = newObject;
	newCell -> next = NULL;
	newCell -> prev = NULL;

	AfxMessageBox(_T("Replace"));

	if(m_Current == NULL) { // 리스트가 비어 있음...
		m_Top = newCell;
		m_Current = newCell;
		m_uNumberOfObject++;
	}
	else if(m_Top == m_Current) {
		delete m_Current ->cell;
		delete m_Current;

		m_Top = newCell;
		m_Current = newCell;
	}
	else {
		newCell ->next = m_Current ->next;
		newCell ->prev = m_Current ->prev;

		if(m_Current ->prev != NULL) {
			(m_Current ->prev) ->next = newCell;
		}
		if(m_Current ->next != NULL) {
			(m_Current ->next) ->prev = newCell;
		}

		delete m_Current ->cell;
		delete m_Current;

		m_Current = newCell;
	}
}

VOID CListOfTexObject::DeleteAllElement()
{
	struct _Cell *current, *tmp;
	INT count = 0;
	

	current = m_Top;
	tmp = current;

	while(current -> next != NULL) {
		count++;
		current = current ->next;
		if(tmp != NULL) {
			delete tmp ->cell;
			delete tmp;
		}
		tmp = current;
	}

	m_Top = NULL;
	m_Current = NULL;
	m_uNumberOfObject = 0;
}
