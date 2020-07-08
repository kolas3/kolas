// ESL_ControlMultiComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "ESL_ControlMultiComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"
#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_ControlMultiComboBox

CESL_ControlMultiComboBox::CESL_ControlMultiComboBox(CESL_Control_Element * ELEMENT)
{
	this->ELEMENT = ELEMENT;
	m_cols = 0;

	RequireFlag = false;
	m_nDefaultRowIdx = 0;
	m_strBlank = _T("    ");
}

CESL_ControlMultiComboBox::~CESL_ControlMultiComboBox()
{
}


BEGIN_MESSAGE_MAP(CESL_ControlMultiComboBox, CJComboBox)
	//{{AFX_MSG_MAP(CESL_ControlMultiComboBox)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CESL_ControlMultiComboBox::InitMultiComboBox(INT tCol,INT nJustify,BOOL bCheckBox,UINT nEdge,INT ColDisplay)
{
EFS_BEGIN

	m_cols = tCol;
	FormatList(tCol, nJustify, bCheckBox, nEdge);

EFS_END
}

INT CESL_ControlMultiComboBox::SetData(TCHAR **data, INT ndatas, INT index)
{
EFS_BEGIN

	return -999;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::SetData(CString data[], INT ndatas, INT index)
{
EFS_BEGIN

	if (ndatas == 0) return -1;
	INT currentrow = GetRowCount();
	for (INT i = 0; i < ndatas; i++) {
		AddItem(data[i], i, currentrow);
	}
	return currentrow;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::GetData(TCHAR *data, INT cols, INT index)
{
EFS_BEGIN

	return -999;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::GetData(CString &data, INT index, INT cols)
{
EFS_BEGIN

	INT curpos = index;
	if (index == -1) curpos = GetCurSel();
	if (cols == -1) cols = 0;
	data = GetItemDataString(curpos, cols);
	data.TrimLeft();			data.TrimRight();
	return 0;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::GetRowCount()
{
EFS_BEGIN

	return GetCount();

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::GetColCount()
{
EFS_BEGIN

	return m_cols;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::SelectItem(CString data)
{
EFS_BEGIN

	SelectString(data.GetBuffer(0));
	return 0;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::InitMultiComboBoxRef(VOID)
{
EFS_BEGIN

	CString data;

	ParseListData();
	if (m_cols == 0) return 0;
	
	if (ELEMENT->REQUIRECHECK.GetLength() > 0) {
		m_bkR = 204;
		m_bkG = 255;
		m_bkB = 255;
		RequireFlag = true;
	}
	InitMultiComboBox(m_cols);
	if (m_cols != 2) return 0;

	if(columns [0] == _T("USER_DEFINE"))
	{
		CString strCode[256], strDesc[256];
		CString strListData ;
		strListData = ELEMENT->LIST_DATA ;
        
		if(strListData.GetLength () < 2) return 0 ;

		INT nPos ;
		nPos = strListData.GetLength () ;
        
		strListData.SetAt (0,' ') ;
        strListData.SetAt (nPos-1,' ') ;
        strListData.TrimLeft () ;strListData.TrimRight () ;
 
		CString strTempCode ;
        
		INT nFirst_to   = strListData.Find (_T("}")) ;
        
		strTempCode = strListData.Mid (0,nFirst_to ) ;
        
		TCHAR seps[2]   = _T(",");
		seps[1] = '\0' ;
		TCHAR *token;
		token = _tcstok( strTempCode.GetBuffer (0), seps );
		INT nCnt = 0 ;
		while( token != NULL )
		{
		  AddItem(token, 0, nCnt);
		  nCnt++ ;
		  token = _tcstok( NULL, seps );
		}

		       
        CString strTempDesc ;

		nPos = nFirst_to + 3;

		strTempDesc = strListData.Mid (nPos) ;

		strTempDesc.TrimLeft () ;strTempDesc.TrimRight () ;

        token = _tcstok( strTempDesc.GetBuffer (0), seps );
	    nCnt = 0 ;
		while( token != NULL )
		{
		  AddItem(token, 1, nCnt);
		  nCnt++ ;
		  token = _tcstok( NULL, seps );
		}
		INT nDefaultRow = 0;
		SetCurSel(nDefaultRow);
		m_nDefaultRowIdx = nDefaultRow;
		
		return 0;
	}

	CString code_class, code_part;
	if (columns[0].GetLength() == 0) return -4;
	GetListData(columns[0], code_class, code_part);
	CESL_Mgr * p = (CESL_Mgr*)ELEMENT->WorkWnd;
	if (!p) return -1;
	CESL_Information * pInfo = p->m_pInfo;
	if (!pInfo) return -2;
	CESL_CodeMgr *pCodeMgr = pInfo->pCodeMgr;
	if (!pCodeMgr) return -3;
	INT nDefaultRow = 0;
	CString strCode[512], strDesc[512];
	INT nCount = 512;
	pCodeMgr->GetCode(code_class, strCode, strDesc, nCount, nDefaultRow);
	INT CodeColumn=0, DescColumn=1;
	if (code_part == _T("Ό³Έν")) {
		CodeColumn=1;
		DescColumn=0;
	}
	for (INT i = 0; i < nCount; i++) {
		if (strCode[i].GetLength() == 0) strCode[i] = m_strBlank;
		if (strCode[i].GetLength() == 12 && strCode[i].GetAt(0) == ' ') strCode[i] = m_strBlank;
		if (CodeColumn == 0) {
			AddItem(strCode[i], 0, i);
			AddItem(strDesc[i], 1, i);
		} else {
			AddItem(strDesc[i], 0, i);
			AddItem(strCode[i], 1, i);
		}
	}
	SetCurSel(nDefaultRow);
	m_nDefaultRowIdx = nDefaultRow;

	return 0;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::ParseListData()
{
EFS_BEGIN

	m_cols = 0;
	CString list_data = ELEMENT->LIST_DATA;

	if(list_data.Find (_T("!")) !=-1)
	{    
		CString sztemp, column_info;
		for (INT i = 0; true; i++) {
			column_info.Empty();
			column_info = list_data.SpanExcluding(_T(","));
			columns[i] = column_info;
			m_cols++;
			if (column_info == list_data) break;
			sztemp = list_data.Mid(column_info.GetLength()+1);
			if (sztemp.GetLength() == 0) break;
			list_data = sztemp;
		}
		return 0;
    }
    else
	{
		m_cols = 2; 
		columns[0] = _T("USER_DEFINE") ;
	}

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::GetListData(CString str, CString &code_class, CString &code_part)
{
EFS_BEGIN

	INT pos = str.Find(_T("."), 1);
	code_class = str.Mid(1, pos-1);
	code_part = str.Mid(pos+1);
	return 0;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::Display(INT index)
{
EFS_BEGIN

	INT nCol = -1;

	CString link_data = ELEMENT->LINK_DATA;
	CString column_alias, sztemp;
	INT i, ids, curpos;
	for (i = 0; true; i++) {
		column_alias.Empty();
		column_alias = link_data.SpanExcluding(_T(","));
		curpos = link_data.Find(_T(","), 0);
		column_alias.TrimLeft();		column_alias.TrimRight();
		if (column_alias.GetLength() > 0) {
			nCol = i;
			break;
		}
		if (curpos == -1) return 0;
		sztemp = link_data.Mid(curpos+1);
		link_data = sztemp;
	}
	if (nCol < 0) return -1;
	CString DMAlias, DMFieldAlias, data;
	ParseMgrAndColumn(column_alias, DMAlias, DMFieldAlias);
	CESL_Mgr *pSM = (CESL_Mgr*)ELEMENT->WorkWnd;
	if (!pSM) return -2;
	ids = pSM->GetDataMgrData(DMAlias, DMFieldAlias, data, index);
	data.TrimLeft();		data.TrimRight();
	if (data.GetLength() == 0) data = m_strBlank;
	SelectString(data, -1, nCol);
	return 0;

EFS_END
return -1;

}

INT CESL_ControlMultiComboBox::ParseMgrAndColumn(CString link_data, CString &MgrAlias, CString &ColumnAlias)
{
EFS_BEGIN
	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;
		MgrAlias = link_data.Mid(1, mgrpos-1);
		ColumnAlias = link_data.Mid(mgrpos+1);
	}
	return 0;
EFS_END
return -1;
}

VOID CESL_ControlMultiComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CJComboBox::OnKeyDown(nChar, nRepCnt, nFlags);

	CString str, strCurrent;
	BOOL bFlag = FALSE;
	if (nChar == VK_NUMPAD0) nChar = '0';
	if (nChar == VK_NUMPAD1) nChar = '1';
	if (nChar == VK_NUMPAD2) nChar = '2';
	if (nChar == VK_NUMPAD3) nChar = '3';
	if (nChar == VK_NUMPAD4) nChar = '4';
	if (nChar == VK_NUMPAD5) nChar = '5';
	if (nChar == VK_NUMPAD6) nChar = '6';
	if (nChar == VK_NUMPAD7) nChar = '7';
	if (nChar == VK_NUMPAD8) nChar = '8';
	if (nChar == VK_NUMPAD9) nChar = '9';
	if (nChar >= '0' && nChar <= '9') {
		bFlag = TRUE;
	}
	if (nChar >= 'A' && nChar <= 'Z') {
		bFlag = TRUE;
	}

	if (bFlag == TRUE) {
		CJStruct * pCurrent;
		POSITION pos = m_jcbList.GetHeadPosition();
		INT nDatas = m_jcbList.GetCount();
		str.Format(_T("%c"), nChar);
		for (INT i = 0; i < nDatas; i++) {
			pCurrent = m_jcbList.GetNext(pos);
			if (pCurrent == NULL) continue;
			strCurrent = pCurrent->m_strKey[0];
			if (strCurrent.GetLength() < 1) continue;
			strCurrent.MakeUpper();
			if (strCurrent.GetAt(0) == str.GetAt(0)) {
				SetCurSel(i);				
				break;
			}
		}
	}
	
	if (bFlag == FALSE) {
		if (nChar == 32 || nChar == 8) {	
			SelectString(m_strBlank);
		}
	}
	return;
}