// ESL_CodeMgr.cpp: implementation of the CESL_CodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_CodeMgr.h"
#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_CodeMgr::CESL_CodeMgr()
{
EFS_BEGIN
	m_CLASS_NAME.RemoveAll();	
EFS_END
}

CESL_CodeMgr::~CESL_CodeMgr()
{
EFS_BEGIN
	FreeCodeMapList();
EFS_END
}

INT CESL_CodeMgr::ReadCodeData(CString user_pk, CProgressCtrl *pBar)
{
EFS_BEGIN
TRACE (_T("(Start) - Read Code Data \n"));
DWORD START, FINISH;
START = GetTickCount();
	if (user_pk.GetLength() == 0) return -2001;
	m_pCodeDM.SetCONNECTION_INFO(CONNECTION_INFO);

	CString sQuery;
	sQuery.Format(_T("SELECT U.REC_KEY, G.CLASS_NAME, C.CODE, C.DESCRIPTION, U.DEFAULT_VALUE, U.ACTIVE, C.CLASS FROM CD_CODE_TBL C, MN_USER_CODE_TBL U, CD_CLASS_CODE_TBL G WHERE C.REC_KEY = U.CODE_KEY AND C.CLASS = G.CLASS_CODE AND U.USER_KEY=%s ORDER BY C.CODE"), user_pk);
	
	INT ids = m_pCodeDM.RestructDataManager(sQuery, pBar);
	if (ids < 0) return ids;

	FreeCodeMapList();
	INT nRows = m_pCodeDM.GetRowCount();
	CString strFileName;
	for (INT i = 0; i < nRows; i++) {
		AddCode(m_pCodeDM.GetCellData(i, 1), m_pCodeDM.GetCellData(i, 2), m_pCodeDM.GetCellData(i, 3));
	}
FINISH = GetTickCount();
TRACE(_T("(Finish) - Read Code Data [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
EFS_END
	return 0;
}

INT CESL_CodeMgr::IsInCode(CString strClass_name)
{
EFS_BEGIN
	INT nCount = m_CLASS_NAME.GetSize();
	for (INT i = 0; i < nCount; i++) {
		if (m_CLASS_NAME.GetAt(i) == strClass_name) return i;
	}
EFS_END
	return -1;
}

INT CESL_CodeMgr::AddCode(CString strClass_name, CString strCode, CString strDesc)
{
EFS_BEGIN
	INT nPos = IsInCode(strClass_name);
	if (nPos == -1) {
		m_CLASS_NAME.Add(strClass_name);
		CMapStringToString * pTmpMap = new CMapStringToString;
		m_CodeMapList.AddTail((CObject*)pTmpMap);
		nPos = m_CodeMapList.GetCount()-1;
	}
	POSITION pos = m_CodeMapList.FindIndex(nPos);
	if (pos == NULL) return -1;
	CMapStringToString *pMap = (CMapStringToString *)m_CodeMapList.GetAt(pos);
	if (pMap == NULL) return -2;
	pMap->SetAt(strCode, strDesc);
EFS_END
	return 0;
}

INT CESL_CodeMgr::GetCode(CString class_name, CString data[], INT ndata, INT startrow)
{
EFS_BEGIN
	INT maxrow = m_pCodeDM.GetRowCount();
	if (maxrow == 0) return -2001;
	if (ndata < 4) return -2002;
	for (INT i = startrow; i < maxrow; i++) {
		if (m_pCodeDM.GetCellData(i, 5).GetLength() == 0) continue;
		for (INT j = 0; j < ndata; j++) {
			data[j] = m_pCodeDM.GetCellData(i, j);
		}
		if (data[1] == class_name && data[2] != _T("-1")) return i;
	}
EFS_END
	return -2003;
}

INT CESL_CodeMgr::GetCodeCount(CString class_name)
{
EFS_BEGIN
	INT maxrow = m_pCodeDM.GetRowCount();
	INT count=0;
	if (maxrow == 0) return -2001;
	for (INT i = 0; i < maxrow; i++) {
		if (m_pCodeDM.GetCellData(i, 1) == class_name && m_pCodeDM.GetCellData(i, 2) != _T("-1") && m_pCodeDM.GetCellData(i, 5) == _T("Y")) count++;
	}
	return count;
EFS_END
	return -1;
}

INT CESL_CodeMgr::GetCode(CString class_name, CString part, CString &data, INT startrow)
{
EFS_BEGIN
	INT maxrow = m_pCodeDM.GetRowCount();
	if (maxrow == 0) return -2001;
	CString local, temp;
	CString IsActive;
	for (INT i = startrow; i < maxrow; i++) {
		IsActive = m_pCodeDM.GetCellData(i, 5);
		if (IsActive == _T("N")) continue;
		local = m_pCodeDM.GetCellData(i, 1);
		temp = m_pCodeDM.GetCellData(i, 2);
		if (temp == _T("-1")) continue;
		if (local == class_name && part==_T("코드")) {
			data = m_pCodeDM.GetCellData(i, 2);
			return i;
		}
		if (local == class_name && part==_T("설명")) {
			data = m_pCodeDM.GetCellData(i, 3);
			return i;
		}
	}
EFS_END
	return -2003;
}

INT CESL_CodeMgr::GetDefaultCode(CString class_name, CString &data)
{
EFS_BEGIN
	INT maxrow = m_pCodeDM.GetRowCount();
	if (maxrow == 0) return -2001;
	CString local, temp, isdefault;
	for (INT i = 0; i < maxrow; i++) {
		local = m_pCodeDM.GetCellData(i, 1);
		temp = m_pCodeDM.GetCellData(i, 2);
		isdefault = m_pCodeDM.GetCellData(i, 4);
		if (temp == _T("-1")) continue;
		if (local == class_name && isdefault==_T("Y")) {
			data = temp;
			return i;
		}
	}
	data.Empty();
EFS_END
	return -2003;
}

INT CESL_CodeMgr::ConvertCodeToDesc(CString class_name, CString code, CString &desc)
{
EFS_BEGIN
	
	desc = _T("");
	INT nPos = IsInCode(class_name);
	if (nPos == -1) -2004;
	POSITION pos = m_CodeMapList.FindIndex(nPos);
	if (pos == NULL) return -2004;
	CMapStringToString *pMap = 	(CMapStringToString *)m_CodeMapList.GetAt(pos);
	if (pMap == NULL) return -2004;
	if (pMap->Lookup(code, desc) == 0) return -2004;
EFS_END
	return 0;	
}

INT CESL_CodeMgr::ConvertDescToCode(CString class_name, CString desc, CString &code)
{
EFS_BEGIN
	INT maxrow = m_pCodeDM.GetRowCount();
	if (maxrow == 0) return -2001;
	CString ClassName, Code, Descrip;
	for (INT i = 0; i < maxrow; i++) {
		ClassName = m_pCodeDM.GetCellData(i, 1);
		Code = m_pCodeDM.GetCellData(i, 2);
		Descrip = m_pCodeDM.GetCellData(i, 3);
		if (ClassName == class_name && Descrip == desc) {
			code = Code;
			return 0;
		}
	}
EFS_END
	return -2005;
}

INT CESL_CodeMgr::ConvertCodeToDescInDM(CESL_DataMgr *pTargetDM, CString class_name, INT nCol)
{
EFS_BEGIN
	INT i, j, ids, nCodeCount = 256, nDefaultValue;
	CString strCode[256], strDesc[256];

	if (!pTargetDM) return -2006;
	GetCode(class_name, strCode, strDesc, nCodeCount, nDefaultValue);
	if (nCodeCount == 0) return -2001;

	INT nRowCount = pTargetDM->GetRowCount();
	CString strTmpCode;
	for (i = 0; i < nRowCount; i++) {
		ids = pTargetDM->GetCellData(i, nCol, strTmpCode);
		for (j = 0; j < nCodeCount; j++) {
			if (strTmpCode == strCode[j]) {
				pTargetDM->SetCellData(i, nCol, strDesc[j]);
				break;
			}
		}
	}
EFS_END
	return 0;
}

INT CESL_CodeMgr::ConvertCodeToDescInDM(CESL_DataMgr *pTargetDM, CString class_name, CString FieldAlias)
{
EFS_BEGIN
	if (!pTargetDM) return -2006;
	INT nRows = pTargetDM->GetRowCount();
	if (nRows == 0) return 0;
	INT nCol = pTargetDM->FindColumn(FieldAlias);
	if (nCol < 0)	return -2008;
	return ConvertCodeToDescInDM(pTargetDM, class_name,nCol);
EFS_END
	return -1;
}

INT CESL_CodeMgr::GetCode(CString class_name, CString strCode[], CString strDesc[], INT &nCount, INT &nDefaultValue)
{
EFS_BEGIN
	INT maxrow = m_pCodeDM.GetRowCount();
	if (maxrow == 0) return -2001;
	TCHAR * cell = NULL;
	DWORD *row;
	CString strTmp;

	POSITION pos = m_pCodeDM.data.GetHeadPosition();
	INT codeCount = 0;

	CString local, temp;
	for (INT i = 0; i < maxrow; i++) {
		row = NULL;
		row = (DWORD*)m_pCodeDM.data.GetNext(pos);
		if (!row) continue;
		if (class_name.Compare((TCHAR*)row[1]) != 0) continue;
		if (_tcscmp((TCHAR*)row[5], _T("N")) == 0) continue;
		if (_tcscmp((TCHAR*)row[2], _T("-1")) == 0) continue;
		if (_tcscmp((TCHAR*)row[4], _T("Y")) == 0) nDefaultValue = codeCount;
		strCode[codeCount].Format(_T("%s"), (TCHAR*)row[2]);
		strDesc[codeCount].Format(_T("%s"), (TCHAR*)row[3]);
		codeCount++;
		if (codeCount >= nCount) return -1;
	}
	nCount = codeCount;
EFS_END
	return 0;
}

INT CESL_CodeMgr::FreeCodeMapList(VOID)
{
EFS_BEGIN
	INT nCount = m_CodeMapList.GetCount();
	if (nCount == 0) return 0;
	CMapStringToString *pData;
	POSITION pos = m_CodeMapList.GetHeadPosition();
	for (INT i = 0; i < nCount; i++) {
		pData = NULL;
		pData = (CMapStringToString*)m_CodeMapList.GetNext(pos);
		if (pData) {
			delete pData;
		}
	}
	m_CodeMapList.RemoveAll();

	m_CLASS_NAME.RemoveAll();
EFS_END
	return 0;
}