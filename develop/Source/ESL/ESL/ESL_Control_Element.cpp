// ESL_Control_Element.cpp: implementation of the CESL_Control_Element class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_Control_Element.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ESL_ControlEdit.h"
#include "ECO_ControlDateEdit.h"
#include "ESL_ControlMultiComboBox.h"
#include "ESL_Grid.h"
#include "ESL_ControlComboBox.h"

#include "ESL_Mgr.h"
#include "ESL_Information.h"
#include "ESL_DateTimeCtrl.h"
#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_Control_Element::CESL_Control_Element(CWnd* pParent /*=NULL*/)
{
	WorkWnd = pParent;
	Control = NULL;
}

CESL_Control_Element::~CESL_Control_Element()
{
	if (Control && CTRL_TYPE == _T("E")) {
		CESL_ControlEdit * pControl = (CESL_ControlEdit *)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
	if (Control && CTRL_TYPE == _T("D")) {
		CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit *)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
	if (Control && CTRL_TYPE == _T("M")) {
		CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox *)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
	if (Control && CTRL_TYPE == _T("C")) {
		CESL_ControlComboBox * pControl = (CESL_ControlComboBox *)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
	if (Control && CTRL_TYPE == _T("G")) {
		CESL_Grid *pControl = (CESL_Grid*)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
	if (Control && CTRL_TYPE == _T("G2")) {
		CESL_Grid2 *pControl = (CESL_Grid2*)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
	if (Control && CTRL_TYPE == _T("D2")) {
		CESL_DateTimeCtrl * pControl = (CESL_DateTimeCtrl *)Control;
		delete pControl;
		pControl = NULL;
		Control = NULL;
	}
}

CString CESL_Control_Element::GetAlias(VOID)
{
EFS_BEGIN
	return CTRL_ALIAS;
EFS_END
return _T("");
}

INT CESL_Control_Element::InitControl(VOID)
{
EFS_BEGIN
	CESL_Mgr * pp = (CESL_Mgr*)WorkWnd;
	CESL_Information *p = pp->m_pInfo;

	CString sStyle[16];
	INT nStyle = 16;
	ParseCTRLStyle(sStyle, nStyle);
	INT i;
	CString strTmp, strTmp2;
	CTime t = CTime::GetCurrentTime();

	if (CTRL_TYPE == _T("E")) {
		if (CTRL_ID < 0) return 0;
		if (Control) return -1;	
		CESL_ControlEdit * pControl = new CESL_ControlEdit(this);
		Control = (CObject*)pControl;
		if (pControl->SubclassDlgItem(CTRL_ID, WorkWnd)) {
			if (INIT_VALUE.GetLength() > 0) {
				strTmp = INIT_VALUE;
				strTmp.MakeUpper();
				strTmp.TrimLeft();		strTmp.TrimRight();
				if (strTmp == _T("SYSTEM_YEAR")) {
					strTmp2.Format(_T("%04d"), t.GetYear());
					pControl->SetData(strTmp2);
				} else {
					pControl->SetData(INIT_VALUE);
				}
			}
			pControl->InitControl();
			pControl->ValidationCheck(VALIDATION.GetBuffer(0));

			for (i = 0; i < nStyle; i++) {
				if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
				if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
			}
		}
	}		

	if (CTRL_TYPE == _T("D")) {
		if (CTRL_ID < 0) return 0;
		if (Control) return -1;	
		CECO_ControlDateEdit * pControl = new CECO_ControlDateEdit(this);
		Control = (CObject*)pControl;
		if (pControl->SubclassDlgItem(CTRL_ID, WorkWnd)) {
			pControl->m_ControlStyle = 0;
			for (i = 0; i < nStyle; i++) {
				if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
				if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
				if (sStyle[i] == _T("READ_ONLY")) pControl->m_ControlStyle = 4;
			}
			pControl->InitEdit();
			strTmp = INIT_VALUE;
			strTmp.MakeUpper();
			strTmp.TrimLeft();		strTmp.TrimRight();
			if (strTmp == _T("SYSTEM_DATE")) {
				strTmp2.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(),	t.GetDay());
				pControl->SetData(strTmp2);
			} else if (INIT_VALUE.GetLength() > 0) {
				pControl->SetData(INIT_VALUE);
			}
		}
	}
	if (CTRL_TYPE == _T("D2")) {
		if (CTRL_ID < 0) return 0;
		if (Control) return -1;
		CESL_DateTimeCtrl * pControl = new CESL_DateTimeCtrl(this);
		Control = (CObject*)pControl;
		if (pControl->SubclassDlgItem(CTRL_ID, WorkWnd)) {

			for (i = 0; i < nStyle; i++) {
				if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
				if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
				if (sStyle[i] == _T("READ_ONLY")) pControl->EnableWindow(false);
			}
			if (INIT_VALUE.GetLength() > 0) pControl->SetData(INIT_VALUE);
		}
	}	
	
	if (CTRL_TYPE == _T("M")) {
		if (CTRL_ID < 0) return 0;
		if (Control) return -1;
		CESL_ControlMultiComboBox * pControl = new CESL_ControlMultiComboBox(this);
		Control = (CObject*)pControl;
		if (pControl->SubclassDlgItem(CTRL_ID, WorkWnd)) {
			pControl->m_bkR = pp->m_bkR;
			pControl->m_bkG = pp->m_bkG;
			pControl->m_bkB = pp->m_bkB;
			pControl->InitMultiComboBoxRef();
			for (i = 0; i < nStyle; i++) {
				if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
				if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
			}
		}
	}		

	if (CTRL_TYPE == _T("G")) {
		if (CTRL_ID < 0) return 0;
		if (Control) return -1;
		CESL_Grid * pControl = new CESL_Grid(this);
		Control = (CObject*)pControl;
		if (pControl->SubclassDlgItem(CTRL_ID, WorkWnd)) {
			INT columns = pControl->CountLinkColumns();
			if (columns>=0) pControl->InitColumn(columns);
			for (i = 0; i < nStyle; i++) {
				if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
				if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
				if (sStyle[i].Find(_T("SELECTED"), 0) >= 0) ParseGridSelectStyle(sStyle[i]);
			}
			pControl->SetColumnInfo();
		}
	}

	if (CTRL_TYPE == _T("G2")) {
		if (CTRL_ID < 0) return 0;
		if (Control) return -1;

		CRect rRect = CRect(0, 0, 0, 0);

			CESL_Grid2* pControl = new CESL_Grid2(this);	
			Control = (CObject*)pControl;
			if (pControl->Create(rRect, WorkWnd, CTRL_ID)) {
				CFont *pFont = WorkWnd->GetFont();
				pControl->SetFont(pFont);
				pControl->SetDefCellMargin(1);
				Control = (CObject*)pControl;
				INT columns = pControl->CountLinkColumns();
				if (columns>=0) pControl->InitColumn(columns);
				for (i = 0; i < nStyle; i++) {
					if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
					if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
					if (sStyle[i].Find(_T("SELECTED"), 0) >= 0) ParseGridSelectStyle2(sStyle[i]);
				}
				pControl->SetColumnInfo();
				pControl->m_bMenu = FALSE;
			}
	}

	if (CTRL_TYPE == _T("S")) {
		if (CTRL_ID < 0) return 0;
		CWnd * pControl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pControl) return 0;
		pControl->SetWindowText(INIT_VALUE);
		for (i = 0; i < nStyle; i++) {
			if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
			if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
		}
	}

	if (CTRL_TYPE == _T("C")) {
		if (CTRL_ID < 0) return 0;

		if (Control) return -1;
		CESL_ControlComboBox * pControl = new CESL_ControlComboBox(this);
		Control = (CObject*)pControl;
		if (pControl->SubclassDlgItem(CTRL_ID, WorkWnd)) {
			pControl->InitControl();
			for (i = 0; i < nStyle; i++) {
				if (sStyle[i] == _T("UNVISIBLE")) pControl->ShowWindow(false);
				if (sStyle[i] == _T("DISABLE")) pControl->EnableWindow(false);
			}
		}
	}
	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::ParseGridSelectStyle(CString style)
{
EFS_BEGIN
	CESL_Grid *pControl = (CESL_Grid*)Control;
	if (!pControl) return 0;
	if (CTRL_ID < 0) return 0;
	CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
	if (!pCtrl) return 0;
	if (style.GetLength() > 1) {
		CString isselect, columns, disp_char;
		INT pos, start=1;
		pos = style.Find(_T(":"), start);
		isselect = style.Mid(1, pos-start);
		start = pos+1;
		pos = style.Find(_T(":"), start);
		columns = style.Mid(start, pos-start);
		start = pos+1;
		pos = style.Find(_T("]"), start);
		disp_char = style.Mid(start, pos-start);
		isselect.MakeUpper();
		if (isselect == _T("SELECTED") && columns.GetLength() > 0) {
			pControl->m_bMenu = true;
			pControl->m_bCheck = true;
			pControl->SetSelectionInfo('S', disp_char, _ttoi(columns));	
		}
	}
	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::ParseGridSelectStyle2(CString style)
{
EFS_BEGIN
	CESL_Grid2 *pControl = (CESL_Grid2*)Control;
	if (!pControl) return 0;
	if (CTRL_ID < 0) return 0;
	CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
	if (!pCtrl) return 0;
	if (style.GetLength() > 1) {
		CString isselect, columns, disp_char;
		INT pos, start=1;
		pos = style.Find(_T(":"), start);
		isselect = style.Mid(1, pos-start);
		start = pos+1;
		pos = style.Find(_T(":"), start);
		columns = style.Mid(start, pos-start);
		start = pos+1;
		pos = style.Find(_T("]"), start);
		disp_char = style.Mid(start, pos-start);
		isselect.MakeUpper();
		if (isselect == _T("SELECTED") && columns.GetLength() > 0) {
			pControl->m_bMenu = true;
			pControl->m_bCheck = true;
			pControl->SetSelectionInfo('S', disp_char, _ttoi(columns));	
		}
	}
	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::ParseCTRLStyle(CString result[], INT &nResult)
{
EFS_BEGIN
	if (CTRL_STYLE.GetLength() == 0) {
		nResult = 0;
		return 0;
	}
	CString temp_ctrl_style = CTRL_STYLE;
	CString column, sztemp;
	INT count = 0;
	for (INT i = 0; true; i++) {
		column.Empty();
		column = temp_ctrl_style.SpanExcluding(_T(","));
		if (column.GetLength() > 0) {
			result[count] = column;
			result[count].TrimLeft();		result[count].TrimRight();
			result[count].MakeUpper();
			count++;
		}
		if (column == temp_ctrl_style) break;
		sztemp = temp_ctrl_style.Mid(column.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		temp_ctrl_style = sztemp;
	}
	nResult = count;
	return 0;	
EFS_END
return -1;
}

INT CESL_Control_Element::GetControlMgrData(CString &data, INT index, INT col)
{
EFS_BEGIN
	bool bEmpty = false;
	if (Control == NULL) return -1;

	if (CTRL_TYPE == _T("E")) {
		CESL_ControlEdit * pControl = (CESL_ControlEdit*)Control;
		pControl->GetData(data);
		if (data.GetLength() == 0) bEmpty = true;
	}	
	if (CTRL_TYPE == _T("D")) {
		CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit*)Control;
		pControl->GetData(data);
		if (data.GetLength() == 0) bEmpty = true;
	}	
	if (CTRL_TYPE == _T("D2")) {
		CESL_DateTimeCtrl * pControl = (CESL_DateTimeCtrl*)Control;
		pControl->GetData(data);
		if (data.GetLength() == 0) bEmpty = true;
	}	
	if (CTRL_TYPE == _T("G")) {
		if (CTRL_ID < 0) return 0;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid * pControl = (CESL_Grid*)Control;
		INT currow=index, curcol=col;
		if (index == -1) currow = pControl->GetRow()-1;
		if (col == -1) curcol = pControl->GetCol();
		pControl->GetAt(currow, curcol, data);
	}
	if (CTRL_TYPE == _T("G2")) {
		if (CTRL_ID < 0) return 0;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid2 * pControl = (CESL_Grid2*)Control;
		INT currow=index, curcol=col;
		if (index == -1) currow = pControl->GetRow()-1;
		if (col == -1) curcol = pControl->GetCol();
		pControl->GetAt(currow, curcol, data);
	}
	if (CTRL_TYPE == _T("M")) {
		CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox *)Control;
		pControl->GetData(data, index, col);
		if (data.GetLength() == 0) bEmpty = true;
		if (data == _T("적용안함")) bEmpty = true;
	}
	if (CTRL_TYPE == _T("C")) {
		if (CTRL_ID < 0) return 0;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_ControlComboBox * pControl = (CESL_ControlComboBox *)Control;
		pControl->GetData(data);
		if (data.GetLength() == 0) bEmpty = true;
	}
	if (REQUIRECHECK.GetLength() > 0 && bEmpty) {
		WorkWnd->MessageBox(REQUIRECHECK, _T("필수 입력항목"), MB_ICONINFORMATION);
	}
	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::SetControlMgrData(CString data, INT index, INT col)
{
EFS_BEGIN
	if (Control == NULL) return -1;		// Control이 초기화 되지 않았음

	if (CTRL_TYPE == _T("E")) {
		CESL_ControlEdit * pControl = (CESL_ControlEdit*)Control;
		pControl->SetData(data);
	}
	if (CTRL_TYPE == _T("D")) {
		CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit*)Control;
		pControl->SetData(data);
	}
	if (CTRL_TYPE == _T("D2")) {
		CESL_DateTimeCtrl * pControl = (CESL_DateTimeCtrl*)Control;
		pControl->SetData(data);
	}
	if (CTRL_TYPE == _T("G")) {
		CESL_Grid * pControl = (CESL_Grid*)Control;
		if (CTRL_ID < 0) return 0;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		pControl->SetAt(index, col, data);
	}
	if (CTRL_TYPE == _T("G2")) {
		CESL_Grid2 * pControl = (CESL_Grid2*)Control;
		if (CTRL_ID < 0) return 0;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		pControl->SetAt(index, col, data);
	}
	if (CTRL_TYPE == _T("M")) {
		CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox*)Control;
		if (data.GetLength() > 0) {
			pControl->SelectString(data, index, col);
		} else {
			pControl->SelectString(pControl->m_strBlank, index, col);
		}
	}
	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::Display(INT index)
{
EFS_BEGIN
	CESL_Mgr * pp = (CESL_Mgr*)WorkWnd;
	CESL_Information *p = pp->m_pInfo;
	if (CTRL_TYPE == _T("E")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CESL_ControlEdit * pControl = (CESL_ControlEdit*)Control;
		pControl->Display(index);
	}		

	if (CTRL_TYPE == _T("D")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit*)Control;
		pControl->Display(index);
	}	
	
	if (CTRL_TYPE == _T("M")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox*)Control;
		pControl->Display(index);
	}		

	if (CTRL_TYPE == _T("G")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid *pControl = (CESL_Grid*)Control;
		pControl->Display();
	}
	if (CTRL_TYPE == _T("G2")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid2 *pControl = (CESL_Grid2*)Control;		
		pControl->Display();
	}

	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::SetControlReadOnly(VOID)
{
EFS_BEGIN
	if (CTRL_TYPE == _T("E")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CESL_ControlEdit * pControl = (CESL_ControlEdit*)Control;
		pControl->SetReadOnly(true);
	}			
	if (CTRL_TYPE == _T("G")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid * pControl = (CESL_Grid*)Control;
		pControl->EnableWindow(false);
	}			
	if (CTRL_TYPE == _T("G2")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid2 * pControl = (CESL_Grid2*)Control;
		pControl->EnableWindow(false);
	}		
	if (CTRL_TYPE == _T("D")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit*)Control;
		pControl->ECO_SetReadOnly(true);
	}	
	
	if (CTRL_TYPE == _T("M")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox*)Control;
		pControl->EnableWindow(false);
	}		
	return 0;
EFS_END
return -1;
}

INT CESL_Control_Element::Clear(VOID)
{
EFS_BEGIN
	CESL_Mgr * pp = (CESL_Mgr*)WorkWnd;
	CESL_Information *p = pp->m_pInfo;

	if (CTRL_TYPE == _T("E")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CESL_ControlEdit * pControl = (CESL_ControlEdit*)Control;
		if (pControl) pControl->EditClear();
	}		

	if (CTRL_TYPE == _T("D")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CECO_ControlDateEdit * pControl = (CECO_ControlDateEdit*)Control;
		if (INIT_VALUE.GetLength() > 0) pControl->SetData(INIT_VALUE);
			else pControl->Clear();
	}	
	
	if (CTRL_TYPE == _T("M")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CESL_ControlMultiComboBox * pControl = (CESL_ControlMultiComboBox*)Control;
		pControl->SetCurSel(pControl->m_nDefaultRowIdx);
	}		

	if (CTRL_TYPE == _T("G")) {
		if (CTRL_ID < 0) return 0;
		if (!Control) return -1;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		CESL_Grid *pControl = (CESL_Grid*)Control;
	}

	if (CTRL_TYPE == _T("C")) {
		if (CTRL_ID < 0) return 0;
		CWnd * pCtrl = WorkWnd->GetDlgItem(CTRL_ID);
		if (!pCtrl) return 0;
		if (!Control) return -1;		
		CESL_ControlComboBox *pControl = (CESL_ControlComboBox*)Control;
		pControl->SetCurSel(_ttoi(pControl->ELEMENT->INIT_VALUE));		
	}
	return 0;
EFS_END
return -1;
}
