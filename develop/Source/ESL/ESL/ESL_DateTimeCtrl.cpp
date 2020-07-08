// ESL_DateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_DateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_DateTimeCtrl

CESL_DateTimeCtrl::CESL_DateTimeCtrl(CESL_Control_Element * ELEMENT)
{
	this->ELEMENT = ELEMENT;
}

CESL_DateTimeCtrl::~CESL_DateTimeCtrl()
{
}


BEGIN_MESSAGE_MAP(CESL_DateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CESL_DateTimeCtrl)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_DateTimeCtrl message handlers

INT CESL_DateTimeCtrl::Display(INT RowIdx)
{
	if (RowIdx == -1) return 0;

	CString link_data = ELEMENT->LINK_DATA;
	if (link_data.GetLength() == 0) return 0;
	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;
		CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
		pMgr->GetDataMgrData(link_data.Mid(1, mgrpos-1), link_data.Mid(mgrpos+1), data, RowIdx);
		SetData(data);
	}
	return 0;
}

INT CESL_DateTimeCtrl::GetData(COleDateTime &result)
{
	GetTime(result);
	return 0;
}

INT CESL_DateTimeCtrl::GetData(CString &result)
{
	COleDateTime t;
	GetTime(t);
	
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return 0;
}

INT CESL_DateTimeCtrl::SetData(COleDateTime data)
{
	SetTime(data);
	return 0;
}

INT CESL_DateTimeCtrl::SetData(CString data)
{
	INT year, month, day;
	
	if (ParseDate(data, year, month, day) >= 0)
	{
		COleDateTime t(year, month, day, 12, 0, 0);
		SetData(t);
	}
	else
		SetData(COleDateTime::GetCurrentTime());
		
	return 0;
}

INT CESL_DateTimeCtrl::SetData(VOID)
{
	COleDateTime t = COleDateTime::GetCurrentTime();
	SetTime(t);
	return 0;
}

INT CESL_DateTimeCtrl::ParseDate(CString data, INT &iYear, INT &iMonth, INT &iDay) 
{
	CString year, month, day;
	INT PrevPosition, CursorPosition;
	CursorPosition = data.Find(_T("/"), 0);
	if (CursorPosition == -1) return -1;
	year = data.Mid(0, CursorPosition);
	PrevPosition = CursorPosition;
	CursorPosition = data.Find(_T("/"), PrevPosition+1);
	if (CursorPosition == -1) return -1;
	month = data.Mid(PrevPosition+1, CursorPosition-PrevPosition-1);
	day = data.Mid(CursorPosition+1);

	iYear = _ttoi(year);
	iMonth = _ttoi(month);
	iDay = _ttoi(day);
	return 0;	
}

INT CESL_DateTimeCtrl::InitControl(VOID)
{
	if (ELEMENT->INIT_VALUE.GetLength() > 0) {
		SetData(ELEMENT->INIT_VALUE);
	}
	return 0;	
}
