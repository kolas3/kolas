// ESL_ControlComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_ControlComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"
#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_ControlComboBox

CESL_ControlComboBox::CESL_ControlComboBox(CESL_Control_Element * pCME)
{
	ELEMENT = pCME;
}

CESL_ControlComboBox::~CESL_ControlComboBox()
{
}

BEGIN_MESSAGE_MAP(CESL_ControlComboBox, CComboBox)
	//{{AFX_MSG_MAP(CESL_ControlComboBox)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_ControlComboBox message handlers

INT CESL_ControlComboBox::InitControl(VOID)
{
EFS_BEGIN

	InitControl(ELEMENT->LIST_DATA);
	if (ELEMENT->INIT_VALUE.GetLength() > 0) {
		SetCurSel(_ttoi(ELEMENT->INIT_VALUE));
	}
	return 0;	

EFS_END
return -1;

}

INT CESL_ControlComboBox::GetData(CString &result)
{
EFS_BEGIN

	GetWindowText(result);
	result.TrimLeft();		result.TrimRight();
	return 0;	

EFS_END
return -1;

}

INT CESL_ControlComboBox::InitControl(CString data)
{
EFS_BEGIN

	if (ELEMENT->LIST_DATA.GetLength() == 0) return 0;
	ResetContent();
	CString list_data = ELEMENT->LIST_DATA.Mid(1, ELEMENT->LIST_DATA.GetLength()-2);
	CString column, sztemp;
	for (INT i = 0; true; i++) {
		column.Empty();
		column = list_data.SpanExcluding(_T(","));
		column.TrimLeft();			column.TrimRight();
		if (column.GetLength() > 0) {
			AddString(column);
		}
		if (column == list_data) break;
		sztemp = list_data.Mid(column.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		list_data = sztemp;
	}
	return 0;	

EFS_END
return -1;

}

INT CESL_ControlComboBox::Display(INT RowIdx)
{
EFS_BEGIN

	if (RowIdx == -1) return 0;

	CString link_data = ELEMENT->LINK_DATA;
	if (link_data.GetLength() == 0) return 0;
	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;		// Link Data의 형식이 이상함
		CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
		pMgr->GetDataMgrData(link_data.Mid(1, mgrpos-1), link_data.Mid(mgrpos+1), data, RowIdx);
		SetCurSel(_ttoi(data));
	}
	return 0;

EFS_END
return -1;

}