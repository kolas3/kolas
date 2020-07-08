// ESL_DataMgr.cpp: implementation of the CESL_DataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_DataMgr.h"
#include "ECO_SearchMgrApi.h"

#include "ESL_SearchEngine.h"
#include "XLEzAutomation.h"
#include "ESLDlg_ExportType.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ESLDlg_Error.h"
#include "ESL_ControlMgr.h"
#include "efs.h"

#include "_excelmancontrol.h"
#include "ExcelManager.h"

#include "FWDES.h"

#define MYSQL_CHAR_SET _T("latin1")
#define MYSQL_CONNECT \
	if(m_pMysql == NULL) \
	{\
		m_pMysql  = mysql_init((MYSQL*)NULL);\
		mysql_options(m_pMysql,MYSQL_SET_CHARSET_NAME,MYSQL_CHAR_SET);\
		mysql_real_connect(m_pMysql, m_dbhost.GetBuffer(0), m_dbid.GetBuffer(0), m_dbpwd.GetBuffer(0), NULL, MYSQL_PORT, NULL, 0); \
		QueryLog( _T("MySQL 접속 시도[0]") ); \
		if (mysql_ping(m_pMysql) != 0) \
		{\
			QueryLog( _T("MySQL 접속 재시도[1]") ); \
			UINT nMErrorCode = mysql_errno(m_pMysql); \
			CString sMEMsg; \
			sMEMsg.Format( _T("MySQL ErrorCode : %d ") , nMErrorCode ); \
			QueryLog( sMEMsg.GetBuffer(0) ); \
			\
			mysql_real_connect(m_pMysql, m_dbhost.GetBuffer(0), m_dbid.GetBuffer(0), m_dbpwd.GetBuffer(0), NULL, MYSQL_PORT, NULL, 0); \
			if (mysql_ping(m_pMysql) != 0) \
			{\
				QueryLog( _T("MySQL 접속 실패[2]") ); \
				UINT nMErrorCode = mysql_errno(m_pMysql); \
				CString sMEMsg; \
				sMEMsg.Format( _T("MySQL ErrorCode : %d ") , nMErrorCode ); \
				QueryLog( sMEMsg.GetBuffer(0) ); \
				return -4100; \
			}\
		}\
	} \
	else \
	{ \
		if(mysql_ping(m_pMysql) != 0) \
		{ \
			QueryLog( _T("MySQL 재 접속 실패[3]") ); \
			UINT nMErrorCode = mysql_errno(m_pMysql); \
			CString sMEMsg; \
			sMEMsg.Format( _T("MySQL ErrorCode : %d ") , nMErrorCode ); \
			QueryLog( sMEMsg.GetBuffer(0) ); \
			mysql_real_connect(m_pMysql, m_dbhost.GetBuffer(0), m_dbid.GetBuffer(0), m_dbpwd.GetBuffer(0), NULL, MYSQL_PORT, NULL, 0); \
			QueryLog( _T("MySQL 접속 재시도[4]") ); \
			if (mysql_ping(m_pMysql) != 0) \
			{\
				QueryLog( _T("MySQL 재 접속 재시도[5]") ); \
				UINT nMErrorCode = mysql_errno(m_pMysql); \
				CString sMEMsg; \
				sMEMsg.Format( _T("MySQL ErrorCode : %d ") , nMErrorCode ); \
				QueryLog( sMEMsg.GetBuffer(0) ); \
				return -4101; \
			}\
		} \
	} \

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_DataMgr::CESL_DataMgr()
{
	m_dbkind = 1;
	m_nCols = -1;

	m_pMarcMgr = NULL;
	m_pDataPosition = NULL;

	m_nRequestID = ORACLE_GetRequestID();
	m_nLastMsgID = 0;

	MANAGE_CODE = _T("");
	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}
}

CESL_DataMgr::~CESL_DataMgr()
{
	FreeFrame();
	FreeDBFieldData();
	FreeData();
	FreeDMRef();
	FreePositionInfo();
	
}

VOID CESL_DataMgr::SetCONNECTION_INFO(CString info)
{
EFS_BEGIN		
	
	{
		CString strDMInfo;
		CFWDES DES;
		DES.Read_DES_File(
			_T("..\\CFG\\UseDMInfoChange.cfg"),
			_T("k^Ty"),
			strDMInfo
			);
		
		CString strMode = _T("");
		CString strDBInfoBefore = _T("");
		CString strDBInfoAfter = _T("");
		
		CString strAlias = _T("");
		CString strLine = _T("");
		
		for(int nCnt=0; nCnt < 3; nCnt++)
		{
			AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');
			AfxExtractSubString(strAlias, strLine, 0, '=');
			
			strAlias.TrimLeft();
			strAlias.TrimRight();
			if(_T("MODE") == strAlias)
			{
				AfxExtractSubString(strMode, strLine, 1, '=');
				strMode.TrimLeft();
				strMode.TrimRight();
			}
			else if(_T("BEFORE") == strAlias)
			{
				AfxExtractSubString(strDBInfoBefore, strLine, 1, '=');
				strDBInfoBefore.TrimLeft();
				strDBInfoBefore.TrimRight();
			}
			else if(_T("AFTER") == strAlias)
			{
				AfxExtractSubString(strDBInfoAfter, strLine, 1, '=');
				strDBInfoAfter.TrimLeft();
				strDBInfoAfter.TrimRight();
			}			
		}
		if(info == strDBInfoBefore && _T("TRUE") == strMode)
		{			
			info = strDBInfoAfter;			
		}
		else if(_T("") == info)
		{
			if(_T("TRUE") == strMode)
			{
				info = strDBInfoAfter;
			}
			else
			{
				info = strDBInfoBefore;
			}
		}		
	}	

	CONNECTION_INFO = info;
	ParseCONNECTION_INFO();

EFS_END
}

VOID CESL_DataMgr::ParseCONNECTION_INFO(VOID)
{
EFS_BEGIN

	if (CONNECTION_INFO.GetLength() == 0) return;
	INT curpos=0, newpos=0;
	CString strbuf;

	newpos = CONNECTION_INFO.Find(_T("@"), curpos);
	m_dbid = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	curpos = newpos+1;

	newpos = CONNECTION_INFO.Find(_T("/"), curpos);
	m_dbservice = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	curpos = newpos+1;

	newpos = CONNECTION_INFO.Find(_T("["), curpos);
	m_dbpwd = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	curpos = newpos+1;
	
	newpos = CONNECTION_INFO.Find(_T("]"), curpos);
	m_dbhost = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	m_dbhost.TrimLeft();			m_dbhost.TrimRight();
	if (m_dbhost.GetLength() > 0) SetDB(_T("MYSQL"));

EFS_END
}

VOID CESL_DataMgr::SetDBInfo(CString host, CString id, CString pwd, CString service)
{
EFS_BEGIN

	m_dbhost = host;
	m_dbid = id;
	m_dbpwd = pwd;
	m_dbservice = service;

EFS_END
}

INT CESL_DataMgr::MallocCell(TCHAR *cell, INT size)
{
EFS_BEGIN

	if (cell) {
		free (cell);
		cell=NULL;
	}
	cell = (TCHAR *)malloc(sizeof(TCHAR)*size+sizeof(TCHAR));
	if (!cell) return -1;
	return size;

EFS_END
return -1;

}

DWORD * CESL_DataMgr::MakeRow(INT cells)
{
EFS_BEGIN

	DWORD * result = NULL;
	TCHAR * cell;

	result = (DWORD *)malloc(sizeof(DWORD) * cells);
	if (!result) return NULL;
	for (INT i = 0; i < cells; i++) 
	{
		cell = NULL;
		result[i] = (DWORD)cell;
	}
	return result;

EFS_END
return NULL;

}

VOID CESL_DataMgr::InitRow(DWORD * row)
{
EFS_BEGIN

	if (!row) return;
	INT refcount = RefList.GetCount();
	if (refcount == 0) return;
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	CString isudf;
	for(INT i = 0; i < refcount; i++) 
	{
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		isudf = pRef->DATA_TYPE;		isudf.MakeLower();
        if (pRef->INIT_VALUE.GetLength() > 0) SetCellData(row, i, pRef->INIT_VALUE);
	}
	return;		

EFS_END
}

INT CESL_DataMgr::AddRow(DWORD * row)
{
EFS_BEGIN

	data.AddTail((CObList*)row);
	return 1;

EFS_END
return -1;

}

INT CESL_DataMgr::InsertRow(INT index)
{
EFS_BEGIN

	INT totalcolumns = RefList.GetCount();
	if (m_nCols == 0 && totalcolumns == 0) return -4002;
	if (m_nCols > 0) totalcolumns = m_nCols;
	if (index > data.GetCount()) return -4003;
	DWORD * row = NULL;
	row = MakeRow(totalcolumns);
	InitRow(row);
	if (index == -1) 
	{
		data.AddTail((CObject*)row);
		RefreshPositionInfo();
		return 0;
	}

    if (index == -2) 
	{
		data.AddTail((CObject*)row);
		return (INT)row;
	} 

	POSITION pos = data.FindIndex(index);
	if (pos == NULL) 
	{
		if (row) FreeRow(row, totalcolumns);
		RefreshPositionInfo();
		return -4001;
	}
	data.InsertBefore(pos, (CObject*)row);
	RefreshPositionInfo();
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::DeleteRow(INT index, INT ColCount)
{
EFS_BEGIN

	if (index > data.GetCount()) return -4003;
	DWORD * row = NULL;
	POSITION pos = m_pDataPosition[index];
	if (pos == NULL) {
		return -4003;
	}
	row = (DWORD*)data.GetAt(pos);
	if (ColCount < 0) {
		if (row) FreeRow(row, RefList.GetCount());
	} else {
		if (row) FreeRow(row, ColCount);
	}	
	data.RemoveAt(pos);
	
	if(ColCount != -2)
		RefreshPositionInfo();	
	
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::FindColumn(CString FIELD_ALIAS)
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return -1;
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	for(INT i = 0; i < refcount; i++) {
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		if (pRef) {
			if (pRef->FIELD_ALIAS == FIELD_ALIAS) return i;
		}
	}
	return(-1);	

EFS_END
return -1;
 
}

INT CESL_DataMgr::FindColumnFieldName(CString FIELD_ALIAS, CString &field_name, CString &field_type)
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return -1;
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	for(INT i = 0; i < refcount; i++) {
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		if (pRef) {
			CString strTempFieldType;
			strTempFieldType = pRef->DATA_TYPE;	
			strTempFieldType.MakeLower();
			if ( strTempFieldType == _T("udf") ) continue;
			if (pRef->FIELD_ALIAS == FIELD_ALIAS) {
				field_name = pRef->FIELD_NAME;
				field_type = pRef->FIELD_TYPE;
				return 0;
			}
		}
	}
	return(-1);	

EFS_END
return -1;

}

INT CESL_DataMgr::FindUpdateField(CString FIELD_NAME)
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return -1;
	FIELD_NAME.MakeUpper();
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	for(INT i = 0; i < refcount; i++) {
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		if (pRef) {
			if (pRef->FIELD_NAME == FIELD_NAME) return i;
		}
	}
	return(-1);	

EFS_END
return -1;

}

CString CESL_DataMgr::GetCellData(DWORD *row, INT idx)
{
EFS_BEGIN

	CString result;
	if (row == NULL) return _T("");
	if (idx < 0) return _T("");

	TCHAR * cell;
	cell = (TCHAR*)row[idx];
	if (cell == NULL) return _T("");
	result.Format(_T("%s"), cell);
	return(result);

EFS_END
return _T("");

}

CString CESL_DataMgr::GetCellData(INT rowidx, INT idx)
{
EFS_BEGIN

	if (m_pDataPosition == NULL) return _T("");
	INT rows = GetRowCount();
	if (rows == 0 || rows <= rowidx) return _T("");
	if (idx < 0) return _T("");
	CString result = _T("");
	TCHAR * cell = NULL;
	DWORD *row = (DWORD*)data.GetAt(m_pDataPosition[rowidx]);
	if (row[idx] != NULL) {
		cell = (TCHAR *)row[idx];
		result.Format(_T("%s"), cell);
	} else {
		result = _T("");
	}
	return result;

EFS_END
return _T("");

}

INT CESL_DataMgr::GetCellData(INT rowidx, INT idx, CString &result)
{
EFS_BEGIN

	if (rowidx < 0) return -4011;
	result = GetCellData(rowidx, idx);
	return 0;

EFS_END
return -1;

}

CString CESL_DataMgr::GetCellData(CString alias, INT rowidx)
{
EFS_BEGIN

	if (rowidx < 0) return _T("");
	CString result;
	INT ids = GetCellData(alias, rowidx, result);
	if (ids != 0) return _T("");
	return result;

EFS_END
return _T("");

}

INT CESL_DataMgr::GetCellData(CString alias, INT rowidx, CString &result)
{
EFS_BEGIN

	if (rowidx < 0) return -4011;
	result = _T("");
	CString strMainAlias, strSubAlias;
	INT ids = GetParsedAlias(alias, strMainAlias, strSubAlias);
	INT col;
	if (strSubAlias.GetLength() == 0) {
		col = FindColumn(alias);
	} else {
		col = FindColumn(strMainAlias);
	}
	if (col < 0) return -1;
	if (strSubAlias.GetLength() == 0) {
		return (GetCellData(rowidx, col, result));
	} else {
		CString strTmp;
		GetCellData(rowidx, col, strTmp);
		ids = GetMarcTagData(strTmp, strSubAlias, result);
		return ids;
	}	

EFS_END
return -1;

}

VOID CESL_DataMgr::GetCellData(CString alias[], INT nalias, CString result[], INT rowidx)
{
EFS_BEGIN

	if (rowidx < 0) return;
	INT col;
	for(INT i = 0; i < nalias; i++) {
		col = FindColumn(alias[i]);
		if (col < 0) continue;
		result[i] = GetCellData(rowidx, col);
	}

EFS_END
}

INT CESL_DataMgr::SetCellData(DWORD *row, INT idx, CString str)
{
EFS_BEGIN

    if(idx < 0) 
	{
		return -1;
	}

	TCHAR * cell = NULL;
	cell = (TCHAR*)row[idx];
	if (cell != NULL) {
		free(cell);
		cell = NULL;
	}
	cell = (TCHAR*)malloc( str.GetLength()*sizeof(TCHAR) + sizeof(TCHAR));
	if (!cell) return -1;
	_tcscpy(cell, str);
	cell[str.GetLength()] = '\0';
	row[idx] = (DWORD)cell;
	return(str.GetLength());

EFS_END
return -1;

}

INT CESL_DataMgr::SetCellData(INT rowidx, INT idx, CString str)	
{
EFS_BEGIN

	INT rows = GetRowCount();
	if (rows == 0 || rows < rowidx) return -1;
	DWORD *row;

	POSITION pos = data.GetHeadPosition();
	for (INT i = 0; i < rows; i++) {
		row = NULL;
		row = (DWORD*)data.GetNext(pos);
		if (i == rowidx) {
			return (SetCellData(row, idx, str));
		}
	}
	return -1;

EFS_END
return -1;

}

INT CESL_DataMgr::SetCellData(CString alias, CString data, INT rowidx)
{
EFS_BEGIN

	INT ids = -1;
	if (rowidx == -1) {
		ids = InsertRow(-1);
		if (ids < 0) return ids;
		rowidx = GetRowCount() - 1;
	}
	INT col = FindColumn(alias);
	if (col < 0) return -4501;

	return SetCellData(rowidx, col, data);

EFS_END
return -1;

}

INT CESL_DataMgr::SetCellData(CString alias[], INT nalias, CString data[], INT rowidx)
{
EFS_BEGIN

	INT ids = -1;
	if (rowidx == -1) {
		ids = InsertRow(-1);
		if (ids < 0) return ids;
		rowidx = GetRowCount() - 1;
	}

	INT col;
	for (INT i = 0; i < nalias; i++) {
		col = FindColumn(alias[i]);
		if (col < 0) continue;
		SetCellData(rowidx, col, data[i]);
	}
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::FreeRow(DWORD *row, INT cells)
{
EFS_BEGIN

	if (row == NULL) return 0;
	TCHAR * cell;

	for (INT i = 0; i < cells; i++) {
		cell = NULL;
		cell = (TCHAR*)row[i];
		if (cell != NULL) {
			free(cell);
			cell = NULL;
		}

	}
	free(row);
	row = NULL;
	return 0;	

EFS_END
return -1;

}

INT CESL_DataMgr::RestructDataManager(CString sQuery,CProgressCtrl *pBar, EDBM_BIND_DATA_TYPE *pBind /* = NULL */)
{
EFS_BEGIN

	if(0 <= sQuery.Find(_T("UDF_MANAGE_CODE")))
	{
		CheckManageCode();
	}

	FreeData();
	
	if(!sQuery.IsEmpty()) {
		CString strManageCode;
		strManageCode = _T("'") + MANAGE_CODE + _T("'");
		sQuery.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
	}

	INT ids = -1;
	if (m_dbkind == 1)
	{			
		ids = ORACLE_RestructDataManager(sQuery, pBar, pBind );
	}
	else if (m_dbkind == 2)
	{			
		ids = MYSQL_RestructDataManager(sQuery, pBar);
	}
	else 
	{
		ids = -4998;
	}

	if ( ids == -1 ) return 0;

	if (ids < 0)
	{
		CEFSDMException *e = new CEFSDMException(this);
		e->m_QueryArr.Add(sQuery);					
		throw e;
		EndFrame();
	}
	return ids;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_RestructDataManager(CString sQuery, CProgressCtrl *pBar, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN
	
	CStringArray strarrayConditon;
	CStringArray strarrayTables;
	INT nTotCount;
	INT nPage;
	INT nLastPage;
	
	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	INT ids;

	if (pBar) pBar->SetPos(0);
	INT RowCount, ColumnCount;
	DB_DATA_ARRAY_TYPE * pOutData = NULL;
	m_nLastMsgID = 0;
	
	
	if ( m_bOpenbase )
	{
		nTotCount = GetECOSEARCH(sQuery, strarrayConditon, TBL_NAME, strarrayTables);		

		if ( nTotCount == -1 ) return -1;
		nLastPage = strarrayConditon.GetSize();
		
		for ( nPage=0 ; nPage<nLastPage ; nPage++ )
		{
			ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), strarrayConditon.GetAt(nPage).GetBuffer(0), 
				&RowCount, &ColumnCount, &pOutData, pBind);
			
			if (pBar) pBar->SetPos(50);
			QueryLog(strarrayConditon.GetAt(nPage).GetBuffer(0), ids);
			
			if (ids < 0) 
			{
				FreeDBDataArrayType(pOutData, ColumnCount);
				
				if (ids == ERR_SESSION_MAX) 
				{
					m_nLastMsgID = ERR_SESSION_MAX;
					return ERR_SESSION_MAX;
				}
				return ids;
			}
			
			if (RowCount > MAX_ROW) 
			{
				FreeDBDataArrayType(pOutData, ColumnCount);
				return -4006;
			}
			
			DWORD * row;
			TCHAR * cell;
			for (INT j = 0; j < RowCount; j++) 
			{
				row = NULL;
				row = MakeRow(ColumnCount);
				for (INT i = 0; i < ColumnCount; i++) 
				{
					cell = NULL;
					cell = pOutData[i].pDBDataUnion[j].pStringData;
					if (cell) SetCellData(row, i, cell);
					else	  SetCellData(row, i, _T(""));
				}
				if (row) data.AddTail((CObject*)row);
				if (pBar) pBar->SetPos(50 + (INT)((double)RowCount/ 50 * (j+1)));
			}
			m_nCols = ColumnCount;
			FreeDBDataArrayType(pOutData, ColumnCount);
			if (pBar) pBar->SetPos(100);
		}
	}
	else
	{
		sQuery = GetECOSEARCH(sQuery);
		
		if(  CESL_SearchEngine::GetUsed() )
		{
			WebECOSEARCHR_Replace( sQuery );
		}

		ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), sQuery.GetBuffer(0), 
			&RowCount, &ColumnCount, &pOutData, pBind);
		
		if (pBar) pBar->SetPos(50);
		QueryLog(sQuery.GetBuffer(0), ids);
		
		if (ids < 0) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			
			if (ids == ERR_SESSION_MAX) 
			{
				m_nLastMsgID = ERR_SESSION_MAX;
				return ERR_SESSION_MAX;
			}
			return ids;
		}
		
		if (RowCount > MAX_ROW) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return -4006;
		}
		
		DWORD * row;
		TCHAR * cell;
		for (INT j = 0; j < RowCount; j++) 
		{
			row = NULL;
			row = MakeRow(ColumnCount);
			for (INT i = 0; i < ColumnCount; i++) 
			{
				cell = NULL;
				cell = pOutData[i].pDBDataUnion[j].pStringData;
				if (cell) SetCellData(row, i, cell);
				else	  SetCellData(row, i, _T(""));
			}
			if (row) data.AddTail((CObject*)row);
			if (pBar) pBar->SetPos(50 + (INT)((double)RowCount/ 50 * (j+1)));
		}
		m_nCols = ColumnCount;
		FreeDBDataArrayType(pOutData, ColumnCount);	
		if (pBar) pBar->SetPos(100);	
	}
	
	RefreshPositionInfo();
	return 0;
	
	EFS_END
		return -1;
	
}

CString CESL_DataMgr::MakeSelectQuery(CString option, INT colindex[])
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return _T("");
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	CString query, fields, isudf;
	INT col = 0;
	for(INT i = 0; i < refcount; i++) 
	{
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		isudf = pRef->DATA_TYPE;		
		isudf.MakeLower();
		if (isudf == _T("udf")) continue;

		if (&pRef != NULL) 
		{
			if (fields.GetLength() > 0) 
			{
				fields += (_T(", ") + pRef->FIELD_NAME);
				colindex[col++] = i;
			} 
			else 
			{
				fields += (pRef->FIELD_NAME);
				colindex[col++] = i;
			}
		}
	}
	CString cond;
	if (option.GetLength() > 0) 
	{
		if (CONDITION.GetLength() > 0) 
		{
			cond.Format(_T("%s AND %s"), CONDITION, option);
		} 
		else 
		{
			cond.Format(_T("%s"), option);
		}
	}
	if (option.GetLength() == 0 && CONDITION.GetLength() > 0) 
	{
		cond.Format(_T("%s"), CONDITION);
	}
	if (cond.GetLength() > 0) 
	{
		cond = GetECOSEARCH(cond);
		query.Format(_T("SELECT %s FROM %s WHERE %s %s"), fields, TBL_NAME, cond, EXTRA_CONDITION);
	} 
	else 
	{
		query.Format(_T("SELECT %s FROM %s %s"), fields, TBL_NAME, EXTRA_CONDITION);
	}
	query.TrimLeft();
	query.TrimRight();
	
	return (query);		

EFS_END
return _T("");

}

INT CESL_DataMgr::MakeSelectQuery(CString option, INT colindex[], CStringArray &strarrayQuery)
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return 0;
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	CString query, fields, isudf;
	INT col = 0;
	for(INT i = 0; i < refcount; i++) 
	{
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		isudf = pRef->DATA_TYPE;		
		isudf.MakeLower();
		if (isudf == _T("udf")) continue;

		if (&pRef != NULL) 
		{
			if (fields.GetLength() > 0) 
			{
				fields += (_T(", ") + pRef->FIELD_NAME);
				colindex[col++] = i;
			} 
			else 
			{
				fields += (pRef->FIELD_NAME);
				colindex[col++] = i;
			}
		}
	}
	CString cond;
	if (option.GetLength() > 0) 
	{
		if (CONDITION.GetLength() > 0) 
		{
			cond.Format(_T("%s AND %s"), CONDITION, option);
		} 
		else 
		{
			cond.Format(_T("%s"), option);
		}
	}
	if (option.GetLength() == 0 && CONDITION.GetLength() > 0) 
	{
		cond.Format(_T("%s"), CONDITION);
	}

	INT nTotCount;
	INT nLastPage;
	INT nPage;

	nTotCount = 0;
	nLastPage = 0;
	nPage = 0;

	if (cond.GetLength() > 0) 
	{
		CStringArray strarrayConditon;		
		CStringArray strarrayTables;

		strarrayQuery.RemoveAll();
		strarrayTables.RemoveAll();

		nTotCount = GetECOSEARCH(cond, strarrayConditon, TBL_NAME, strarrayTables);

		if ( nTotCount == -1 ) return -1;
		nLastPage = strarrayConditon.GetSize();		
		
		for ( nPage=0 ; nPage<nLastPage ; nPage++ )
		{
			if ( strarrayTables.GetSize() == 0 )
			{
				query.Format(_T("SELECT %s FROM %s WHERE %s %s"), fields, TBL_NAME, strarrayConditon.GetAt(nPage), EXTRA_CONDITION);
				strarrayQuery.Add(query);
			}
			else
			{
				query.Format(_T("SELECT %s FROM %s WHERE %s %s"), fields, strarrayTables.GetAt(nPage), strarrayConditon.GetAt(nPage), EXTRA_CONDITION);
				strarrayQuery.Add(query);				
			}			
		}
	} 
	else 
	{
		query.Format(_T("SELECT %s FROM %s %s"), fields, TBL_NAME, EXTRA_CONDITION);
		strarrayQuery.Add(query);
	}
	return nTotCount;		

EFS_END
return -1;

}

INT CESL_DataMgr::RefreshDataManager(CString option, CProgressCtrl *pBar/*=NULL*/, EDBM_BIND_DATA_TYPE *pBind /* = NULL */ )
{
EFS_BEGIN	

	FreeData();
	INT nRetry;

	nRetry = 0;
	INT ids = -1;
	do
	{
		if (m_dbkind == 1) 
		{
			ids = ORACLE_RefreshDataManager(option, pBar, pBind);
		}
		else if (m_dbkind == 2) 
		{
			ids = MYSQL_RefreshDataManager(option);
		}
		else
		{
			ids = -4998;
		}
		if ( ids == -1 ) return -8001;

		if (ids < 0 && ids != -4007)
		{			
			::Beep(1000,50);
		}
		nRetry++;
		if ( nRetry >= 3 || ids == -4007) break;
	} while ( ids < 0 );

	if ( nRetry >= 3 )
	{
		CEFSDMException *e = new CEFSDMException(this);
		e->m_QueryArr.Add(m_lastmsg);					
		throw e;
		EndFrame();	
	}

	return ids;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_RefreshDataManager(CString option, CProgressCtrl *pBar/*=NULL*/, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN	


	INT colindex[256];
	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	INT ids;
	CStringArray strarrayQuery;
	
	if ( m_bOpenbase )
	{
		CString strQuery;
		INT nTotCount;
		INT nPage;
		INT nLastPage;
		
		nTotCount = MakeSelectQuery(option, colindex, strarrayQuery);
		// error
		if ( nTotCount == -1 ) return -1;
		if (strarrayQuery.GetSize() == 0) return -4005;
		
		nLastPage = strarrayQuery.GetSize();
		
		CString username;
		username.Format(_T("%s@%s"), m_dbid, m_dbservice);
		INT ids;
		
		INT RowCount, ColumnCount;	
		DB_DATA_ARRAY_TYPE * pOutData = NULL;
		
		m_nLastMsgID = 0;		
		nPage =0;
		if (pBar) pBar->SetPos(0);
		
		for ( nPage=0 ; nPage<nLastPage ; nPage++ )
		{
			strQuery = strarrayQuery.GetAt(nPage);
			
			if(0 <= strQuery.Find(_T("UDF_MANAGE_CODE")))
			{
				CheckManageCode();
			}
			
			if(!strQuery.IsEmpty()) {
				CString strManageCode;
				strManageCode = _T("'") + MANAGE_CODE + _T("'");
				strQuery.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
			}	
			
			m_lastmsg = strQuery;
			ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), strQuery.GetBuffer(0), 
				&RowCount, &ColumnCount, &pOutData, pBind);
			
			if (pBar) pBar->SetPos(50);
			QueryLog(strQuery.GetBuffer(0), ids);
			
			if (ids < 0) 
			{
				FreeDBDataArrayType(pOutData, ColumnCount);
				if (ids == ERR_SESSION_MAX) 
				{
					m_nLastMsgID = ERR_SESSION_MAX;
					return ERR_SESSION_MAX;
				}		
				return ids;
			}
			if (RowCount > MAX_ROW) 
			{
				FreeDBDataArrayType(pOutData, ColumnCount);
				return -4006;
			}

			if (RowCount == 0) 
			{
				if ( nPage == nLastPage-1 )
				{
					FreeDBDataArrayType(pOutData, ColumnCount);
					return 0;

				}
				else
				{
					continue;
				}				
			}
			TCHAR * cell;
			DWORD *row;
			INT totalcolumns = RefList.GetCount();
			for (INT count = 0; count < RowCount; count++)
			{
				row = NULL;
				row = MakeRow(totalcolumns);
				InitRow(row);
				for(INT i = 0; i < ColumnCount; i++)
				{
					cell = NULL;
					cell = pOutData[i].pDBDataUnion[count].pStringData;
					if (cell == NULL) SetCellData(row, colindex[i], _T(""));
					else			  SetCellData(row, colindex[i], cell);
				}
				data.AddTail((CObject*)row);
				if (pBar) pBar->SetPos(50 + (INT)((double)RowCount/ 50 * (count+1)));
			}
			m_nCols = totalcolumns;
			FreeDBDataArrayType(pOutData, ColumnCount);

			RefreshPositionInfo();
			
		}
		if (pBar) pBar->SetPos(100);
	}
	else
	{
		CString sQuery = MakeSelectQuery(option, colindex);
		if (sQuery.GetLength() == 0) return -4005;
		
		if(0 <= sQuery.Find(_T("UDF_MANAGE_CODE")))
		{
			CheckManageCode();
		}
		
		if(!sQuery.IsEmpty()) {
			CString strManageCode;
			strManageCode = _T("'") + MANAGE_CODE + _T("'");
			sQuery.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
		}	
		
		m_lastmsg = sQuery;
		
		if(  CESL_SearchEngine::GetUsed() )
		{
			WebECOSEARCHR_Replace( sQuery );
		}
		
		if (pBar) pBar->SetPos(0);
		INT RowCount, ColumnCount;
		DB_DATA_ARRAY_TYPE * pOutData = NULL;
		m_nLastMsgID = 0;
		ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), sQuery.GetBuffer(0), 
			&RowCount, &ColumnCount, &pOutData, pBind);
		
		if (pBar) pBar->SetPos(50);
		QueryLog(sQuery.GetBuffer(0), ids);
		
		if (ids < 0) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			if (ids == ERR_SESSION_MAX) 
			{
				m_nLastMsgID = ERR_SESSION_MAX;
				return ERR_SESSION_MAX;
			}
			return ids;
		}
		if (RowCount > MAX_ROW) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return -4006;
		}
		if (RowCount == 0) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return 0;
		}
		TCHAR * cell;
		DWORD *row;
		INT totalcolumns = RefList.GetCount();
		for (INT count = 0; count < RowCount; count++)
		{
			row = NULL;
			row = MakeRow(totalcolumns);
			InitRow(row);
			for(INT i = 0; i < ColumnCount; i++)
			{
				cell = NULL;
				cell = pOutData[i].pDBDataUnion[count].pStringData;
				if (cell == NULL) SetCellData(row, colindex[i], _T(""));
				else			  SetCellData(row, colindex[i], cell);
			}
			data.AddTail((CObject*)row);
			if (pBar) pBar->SetPos(50 + (INT)((double)RowCount/ 50 * (count+1)));
		}
		m_nCols = totalcolumns;
		FreeDBDataArrayType(pOutData, ColumnCount);
		if (pBar) pBar->SetPos(100);
		RefreshPositionInfo();
	}	
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::FreeData(VOID)
{
EFS_BEGIN

	FreePositionInfo();
 	if (m_nCols == -1 && RefList.GetCount() > 0) m_nCols = RefList.GetCount();
	INT rows = data.GetCount();
	if (rows == 0) return 1;
	DWORD * row;
	POSITION pos = data.GetHeadPosition();
	for (INT i = 0; i < rows; i++) 
	{
		row = NULL;
		row = (DWORD*)data.GetNext(pos);
		if (row) 
		{
			FreeRow(row, m_nCols);
			row = NULL;
		}
	}
	data.RemoveAll();
	return 1;

EFS_END
return -1;

}

INT CESL_DataMgr::GetRowCount()
{
EFS_BEGIN

	return(data.GetCount());

EFS_END
return -1;

}

INT CESL_DataMgr::GetColCount()
{
EFS_BEGIN

	return (m_nCols);

EFS_END
return -1;

}

INT CESL_DataMgr::GetOneValueQuery(CString sQuery, CString &result, INT nCommit/*=TRUE*/, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/ )
{

EFS_BEGIN

	if(0 <= sQuery.Find(_T("UDF_MANAGE_CODE")))
	{
		CheckManageCode();
	}

	INT nRetry;	

	if( !sQuery.IsEmpty() ) {
		CString strManageCode;
		strManageCode = _T("'") + MANAGE_CODE + _T("'");
		sQuery.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
	}

	nRetry = 0;
	INT ids = -1;
	do 
	{		
		if (m_dbkind == 1) 
		{
			ids = ORACLE_GetOneValueQuery(sQuery, result, nCommit, pBind);
		}
		else if (m_dbkind == 2) 
		{
			ids = MYSQL_GetOneValueQuery(sQuery, result, nCommit);
		}
		else
		{
			ids = -4998;
		}
		
		if (ids < 0 && ids != -4007)
		{			
			::Beep(1000,50);
		}
		nRetry++;
		if ( nRetry >= 3 || ids == -4007) break;
	} while ( ids < 0 );

	if ( nRetry >= 3 )
	{
		CEFSDMException *e = new CEFSDMException(this);
		e->m_QueryArr.Add(sQuery);
		HandleException(e, __WFILE__, __LINE__);	
		EndFrame();
	}

	return ids;

EFS_END
return -1;


}

INT CESL_DataMgr::ORACLE_GetOneValueQuery(CString sQuery, CString &result, INT nCommit/*=TRUE*/, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN
	
	if(0 <= sQuery.Find(_T("UDF_MANAGE_CODE")))
	{
		CheckManageCode();
	}

	CString strTmp;
	INT ids = ORACLE_GetValuesQuery(sQuery, &strTmp, 1, nCommit, pBind);
	result.Format(_T("%s"), strTmp);
	return ids;

EFS_END
return -1;

}

CTime CESL_DataMgr::GetDBTime()
{
	INT ids;
	CString strtime;
	if (m_dbkind == 1) 
	{
		ids = GetOneValueQuery(_T("select to_char(SYSDATE, 'YYYY-MM-DD HH24:MI:SS') from DUAL"), strtime);	
	}
	else if (m_dbkind == 2) 
	{
		ids = GetOneValueQuery(_T("SELECT now()"), strtime);
	}
	
	if (strtime.GetLength() != 19) return (CTime::GetCurrentTime());
	
	CTime result2(_ttoi(strtime.Mid(0, 4)), 
				  _ttoi(strtime.Mid(5, 2)), 
				  _ttoi(strtime.Mid(8, 2)), 
				  _ttoi(strtime.Mid(11, 2)), 
				  _ttoi(strtime.Mid(14, 2)), 
				  _ttoi(strtime.Mid(17, 2)));
	return (result2);
}

INT CESL_DataMgr::GetValuesQuery(CString sQuery, CString result[], INT nresult, INT nCommit/*=TRUE*/, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN

	if(0 <= sQuery.Find(_T("UDF_MANAGE_CODE")))
	{
		CheckManageCode();
	}

	if( !sQuery.IsEmpty() ) {
		CString strManageCode;
		strManageCode = _T("'") + MANAGE_CODE + _T("'");
		sQuery.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
	}
	if (m_dbkind == 1) 
	{
		return ORACLE_GetValuesQuery(sQuery, result, nresult, nCommit, pBind);
	}
	else if (m_dbkind == 2) 
	{
		return MYSQL_GetValuesQuery(sQuery, result, nresult, nCommit);	
	}
	
	return -4998;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_GetValuesQuery(CString sQuery, CString result[], INT nresult, INT nCommit/*=TRUE*/, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN

	INT RowCount, ColumnCount;
	DB_DATA_ARRAY_TYPE * pOutData = NULL;

	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	INT ids;		
	INT nTotCount;

	m_nLastMsgID = 0;
		
	if ( m_bOpenbase )
	{	
		CStringArray strarrayConditon;
		CStringArray strarrayTables;

		nTotCount = GetECOSEARCH(sQuery, strarrayConditon, TBL_NAME, strarrayTables);
		
		ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), strarrayConditon.GetAt(0).GetBuffer(0), 
			&RowCount, &ColumnCount, &pOutData, pBind);
		
		QueryLog(strarrayConditon.GetAt(0).GetBuffer(0), ids);
		if (ids == ERR_SESSION_MAX) 
		{
			m_nLastMsgID = ERR_SESSION_MAX;
			return ERR_SESSION_MAX;
		}
		if (ids < 0) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return ids;
		}
		if (RowCount > MAX_ROW) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return -4006;
		}
		
		if (RowCount > 0) 
		{
			TCHAR * cell;
			for (INT i = 0; i < ColumnCount; i++) 
			{
				cell = NULL;
				cell = pOutData[i].pDBDataUnion[0].pStringData;
				if (cell) result[i].Format(_T("%s"), cell);
			}
		}
		
	}
	else
	{
		sQuery = GetECOSEARCH(sQuery);
		ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), sQuery.GetBuffer(0), 
			&RowCount, &ColumnCount, &pOutData, pBind);
		
		QueryLog(sQuery.GetBuffer(0), ids);
		if (ids == ERR_SESSION_MAX) 
		{
			m_nLastMsgID = ERR_SESSION_MAX;
			return ERR_SESSION_MAX;
		}
		if (ids < 0) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return ids;
		}
		if (RowCount > MAX_ROW) 
		{
			FreeDBDataArrayType(pOutData, ColumnCount);
			return -4006;
		}
		
		if (RowCount > 0) 
		{
			TCHAR * cell;
			for (INT i = 0; i < ColumnCount; i++) 
			{
				cell = NULL;
				cell = pOutData[i].pDBDataUnion[0].pStringData;
				if (cell) result[i].Format(_T("%s"), cell);
			}
		}	
	}
	
	FreeDBDataArrayType(pOutData, ColumnCount);
	if (RowCount == 0) return -4007;
	return ColumnCount;		
		
EFS_END
return -1;

}

INT CESL_DataMgr::ExecuteQuery(CString sQuery, INT commitmode, INT nCommit/*=TRUE*/)
{
EFS_BEGIN

	if(!sQuery.IsEmpty()){
		CString strManageCode;
		strManageCode = _T("'") + MANAGE_CODE + _T("'");
		sQuery.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
	}
	if (m_dbkind == 1) 
	{
		return ORACLE_ExecuteQuery(sQuery, commitmode, nCommit);
	}
	else if (m_dbkind == 2) 
	{
		return MYSQL_ExecuteQuery(sQuery, commitmode, nCommit);
	}
	
	return -4998;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_ExecuteQuery(CString sQuery, INT commitmode, INT nCommit/*=TRUE*/)
{
EFS_BEGIN

	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);

	INT ids;
	m_nLastMsgID = 0;
	ids = ExecSQLProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), sQuery.GetBuffer(0), commitmode);
	QueryLog(sQuery.GetBuffer(0), ids);
	if (ids == ERR_SESSION_MAX) 
	{
		m_nLastMsgID = ERR_SESSION_MAX;
		return ERR_SESSION_MAX;
	}
	if (ids < 0) 
	{
		return ids;
	}
	if (nCommit == TRUE) ORACLE_Commit();
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::FreeFrame(VOID)
{
EFS_BEGIN

	INT querycount = frame.GetCount();
	if (querycount == 0) return 0;
	CString * query;
	for (INT i = 0; i < querycount; i++) 
	{
		query = (CString*)frame.RemoveHead();
		if (query) delete query;
	}
	frame.RemoveAll();
	return(querycount);

EFS_END
return -1;

}

INT CESL_DataMgr::StartFrame(VOID)
{
EFS_BEGIN

	INT ids;
	FreeFrame();

	QueryLog(_T("■ DM.StartFrame ■■■"));

	if (m_dbkind == 1) 
	{
		CString username;
		username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	
		ids = StartFrameProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0));
		
		if (0>ids) return ids;
	}

	if(  CESL_SearchEngine::GetUsed() )
	{
		WebSearch_UpdateDelete_Start();
	}

	return 1;

EFS_END
return -1;

}

INT CESL_DataMgr::EndFrame(VOID)
{
EFS_BEGIN
 
	QueryLog(_T("■ DM.EndFrame   ■■■"));

	if (m_dbkind == 1) 
	{
		INT rc  = EndFrameProc(m_nRequestID);
		if (0>rc)  return rc;
	}
	FreeFrame();
	FreeDBFieldData();
	
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::AddFrame(CString query)
{
EFS_BEGIN

	if(0 <= query.Find(_T("UDF_MANAGE_CODE")))
	{
		CheckManageCode();
	}

	query.TrimLeft();
	query.TrimRight();
	if (query.IsEmpty()) return 1;

	CString strManageTbl[] = {_T("ESL_MGR_TBL EM"), _T("ESL_DB_MGR_TBL"), _T("ESL_MGR_SUB_TBL"), _T("ESL_DB_MGR_SUB_TBL")};
	BOOL	bManage;
	bManage = FALSE;
	for ( INT i=0 ; i<4 ; i++ )
	{
		if ( query.Find(strManageTbl[i]) >= 0 )
		{
			bManage = TRUE;
			break;
		}
	}

	if ( bManage == FALSE )
	{
		CString strManageCode;
		strManageCode = _T("'") + MANAGE_CODE + _T("'");
		query.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
	}

	if(  CESL_SearchEngine::GetUsed() )
	{
		DeltaUpdate_Find_Core( query );
		WebSearch_AddDeleteQuery( query );
	}


	INT ids;
	if (1==m_dbkind) 
	{
		ids = MakeExecSQLFrameProc(m_nRequestID, query.GetBuffer(0));

		if (0>ids)
		{
			EndFrame();
			return ids;
		}
	}
	else if (2==m_dbkind) 
	{
		CString * querystr = new CString;
		if (querystr == NULL)
		{
			EndFrame();
			return -1;	
		}
		querystr->Format(_T("%s"), query);
		INT a = frame.GetCount();
		POSITION pos = frame.AddTail((CObject*)querystr);
		INT t = frame.GetCount();
	}
	else
	{
		EndFrame();
		return -4998;
	}

	return 1;
	
EFS_END
return -1;

}
INT CESL_DataMgr::SendFrame(INT nCommit/*=TRUE*/)
{
EFS_BEGIN
	INT ids = -1;

	QueryLog(_T("■ DM.SendFrame  ■■■"));

	if (m_dbkind == 1) 
	{
		ids = ORACLE_SendFrame(nCommit);
	}
	else if (m_dbkind == 2) 
	{
		if (frame.GetCount() == 0) return 0;
		ids = MYSQL_SendFrame();
		if (0<ids) ids = 0;
	}
	else
	{
		ids = -4998;
	}

	if (ids < 0) 
	{
		CEFSDMException *e = new CEFSDMException(this);
		CString *pQuery = NULL;
		POSITION pos = frame.GetHeadPosition();
		while (pos)
		{
			pQuery = (CString*)frame.GetNext(pos);
			if (pQuery == NULL) continue;
			e->m_QueryArr.Add(pQuery->GetBuffer(0));
		}
		HandleException(e, __WFILE__, __LINE__);
		EndFrame();
	}

	return ids;
	
EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_SendFrame(INT nCommit)
{
EFS_BEGIN

	CString username, strMsg;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	INT ids;


	INT querycount = frame.GetCount();
	POSITION pos = frame.GetHeadPosition();
	INT successcount = 0;
	
	strMsg.Format(_T("[START] - ORACLE SendFrame"));
	QueryLog(strMsg.GetBuffer(0), 0);


	m_nLastMsgID = 0;

	DWORD START, FINISH;
	START = GetTickCount();
	ids = SendFrameProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), NULL);
	FINISH = GetTickCount();

	if (ids == ERR_SESSION_MAX)
	{
		m_nLastMsgID = ERR_SESSION_MAX;
		strMsg.Format(_T("[ END ] - ORACLE SendFrame [Error = ERR_SESSION_MAX]"));
		QueryLog(strMsg.GetBuffer(0), 0);

		return ERR_SESSION_MAX;
	}

	if (0>ids)
	{
		INT rc = RollbackProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0));

		strMsg.Format(_T("[ END ] - ORACLE Rollback [SendError = %d][Rollback = %d]"), ids, rc);
		QueryLog(strMsg.GetBuffer(0), 0);

		if (0>rc) return -2001;
		return ids;
	}

	if (nCommit == TRUE) 
	{
		ids = ORACLE_Commit();		
		if (0>ids)
		{
			strMsg.Format(_T("[ END ] - ORACLE Commit [Error = %d]"), ids);
			QueryLog(strMsg.GetBuffer(0), 0);
			return ids;
		}

		if(  CESL_SearchEngine::GetUsed() )
		{
			DeltaUpdate_Send();
		}
	}	

	strMsg.Format(_T("[ END ] - ORACLE SendFrame [Time = %d.%03d sec][Return=%d]"), (FINISH-START)/1000, (FINISH-START)%1000, ids);
	QueryLog(strMsg.GetBuffer(0), 0);
	
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_Commit()
{
EFS_BEGIN

	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	m_nLastMsgID = 0;
	INT ids = CommitProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0));	
	if (ids == ERR_SESSION_MAX) 
	{
		m_nLastMsgID = ERR_SESSION_MAX;
		return ERR_SESSION_MAX;
	}
	return ids;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_DisconnectDB()
{
EFS_BEGIN

	m_nLastMsgID = 0;
	INT ids = DisconnectDB();	
	if (ids == ERR_SESSION_MAX) 
	{
		m_nLastMsgID = ERR_SESSION_MAX;
		return ERR_SESSION_MAX;
	}
	return ids;

EFS_END
return -1;

}

VOID CESL_DataMgr::InitDBFieldData(VOID)
{
EFS_BEGIN

	FreeDBFieldData();

EFS_END
}

VOID CESL_DataMgr::FreeDBFieldData(VOID)
{
EFS_BEGIN

	INT fieldcount = query.GetCount();
	if (fieldcount == 0) return;
	query_field * queryfield;
	for (INT i = 0; i < fieldcount; i++) {
		queryfield = (query_field*)query.RemoveHead();
		if (queryfield) delete queryfield;
	}
	query.RemoveAll();
	return;	

EFS_END
}

INT CESL_DataMgr::AddDBFieldData(CString field, CString type, CString data, INT replace_flag)
{
EFS_BEGIN

	CheckManageCode();

	query_field * queryfield = new query_field;
	if (queryfield == NULL) return -4009;
	field.MakeUpper();
	queryfield->field = field;
	type.MakeUpper();

	if( !data.IsEmpty() ) {
		CString strManageCode;
		strManageCode = MANAGE_CODE;
		data.Replace(_T("UDF_MANAGE_CODE"), strManageCode);		
	}
	else if( field == _T("MANAGE_CODE"))
	{
		data = MANAGE_CODE;
	}
	
	if (type == _T("NUMERIC")) 
	{
		queryfield->type = 1;
		queryfield->data.Format(_T("%s"), data);
	}
	else if (type == _T("STRING")) 
	{
		queryfield->type = 2;	
		if (m_dbkind == 2 && replace_flag == 1) data.Replace(_T("'"), _T("''"));		
		queryfield->data.Format(_T("%s"), data);
	}	
	else
	{
		queryfield->type = 1;
		queryfield->data.Format(_T("[%s]▶잘못된 컬럼타입▶[%s]"), field, type);
	}	

	query.AddTail((CObject*)queryfield);
	return 1;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeUpdateFrame(CString table_name, 
								  CString condition_field, 
								  CString condition_type, 
								  CString condition_data, 
								  INT RowIdx)
{
EFS_BEGIN

	CheckManageCode();
	
	INT nColumnCount = query.GetCount();
	if (0 == nColumnCount) 
	{	
		EndFrame();
		return -4010;
	}

	if(  CESL_SearchEngine::GetUsed() )
		DeltaUpdate_Find_Core(table_name);

	INT ids;

	if (1==m_dbkind) 
	{
		CString username;
		username.Format(_T("%s@%s"), m_dbid, m_dbservice);
		
		INT		i;
		INT		j;
		TCHAR	szTableName[512];
		TCHAR	szPKAttributeName[256];
		TCHAR	szTablePrimaryKey[256];
		
		_stprintf(szTableName, _T("%s"), table_name.GetBuffer(0));
		
		INT	   *InDataType = NULL;
		TCHAR **pFieldName = NULL;
		TCHAR **pFieldData = NULL;

		InDataType = (INT   *)malloc(sizeof(INT)   *nColumnCount);
		pFieldName = (TCHAR**)malloc(sizeof(TCHAR*)*nColumnCount);
		pFieldData = (TCHAR**)malloc(sizeof(TCHAR*)*nColumnCount);

		if (NULL==InDataType || NULL==pFieldName || NULL==pFieldData) 
		{
			if (InDataType) free(InDataType);
			if (pFieldName) free(pFieldName);
			if (pFieldData) free(pFieldData);
			EndFrame();
			return -4009;
		}
		
		POSITION pos = query.GetHeadPosition();
		query_field * field;
		for (i = 0; i < nColumnCount; i++) 
		{
			field = NULL;
			field = (query_field *)query.GetNext(pos);
			if (field == NULL) continue;
			
			if (field->type == 1)
			{	
				InDataType[i] = 2;
			}
			else if (field->type == 2)
			{	
				InDataType[i] = 1;
			}
			else 
			{
				EndFrame();
				return -4998;
			}

			pFieldName[i] = (TCHAR*)malloc(sizeof(TCHAR)*field->field.GetLength()+sizeof(TCHAR));
			pFieldData[i] = (TCHAR*)malloc(sizeof(TCHAR)*field->data.GetLength()+sizeof(TCHAR));
			
			if (NULL==pFieldName[i] || NULL==pFieldData[i]) 
			{
				free(InDataType);
				for (j = 0; j < i; j++) 
				{
					if (pFieldName[j]) free(pFieldName[j]);
					if (pFieldData[j]) free(pFieldData[j]);
				}
				free(pFieldName);
				free(pFieldData);
				EndFrame();
				return -4009;
			}

			_tcscpy(pFieldName[i], field->field.GetBuffer(0));
			_tcscpy(pFieldData[i], field->data.GetBuffer(0));	
			
			delete field;
		}
		query.RemoveAll();
		
		_tcscpy(szPKAttributeName, (LPCTSTR)condition_field);
		_tcscpy(szTablePrimaryKey, (LPCTSTR)condition_data);
		
		ids = MakeUpdateFrameProc(m_nRequestID       ,
								  szTableName		,
								  nColumnCount		,
								  pFieldName		,
								  pFieldData		,
								  InDataType		,
								  szPKAttributeName	,
								  szTablePrimaryKey
								  );
		
		free(InDataType);
		for (i = 0; i < nColumnCount; i++) 
		{
			free(pFieldName[i]);
			free(pFieldData[i]);
		}
		free(pFieldName);
		free(pFieldData);
		
		if (0>ids) 
		{
			EndFrame();
			return ids;
		}
	}
	else if (2==m_dbkind) 
	{
		CString update_data, temp;
		
		query_field * field;
		INT nCol;
		for (INT i = 0; i < nColumnCount; i++) 
		{
			field = (query_field*)query.RemoveHead();
			if (!field) continue;
			
			if		(field->type == 1) temp.Format(_T("%s = %s"),   field->field, field->data);
			else if (field->type == 2) temp.Format(_T("%s = '%s'"), field->field, field->data);
			else 
			{
				EndFrame();
				return -4998;
			}

			if (update_data.GetLength() > 0) update_data += _T(", ");
			update_data += temp;

			if (field && RowIdx >= 0) 
			{
				nCol = -1;
				nCol = FindUpdateField(field->field);
				if (nCol >= 0) 
				{
					SetCellData(RowIdx, nCol, field->data);
				}
			}
			if (field) 
			{
				delete field;
				field = NULL;
			}
		}
		query.RemoveAll();
		CString result;
		condition_type.MakeUpper();
		if (condition_type == _T("STRING")) result.Format(_T("UPDATE %s SET %s WHERE %s = '%s';"), table_name, update_data, condition_field, condition_data);
		if (condition_type == _T("NUMERIC")) result.Format(_T("UPDATE %s SET %s WHERE %s = %s;"), table_name, update_data, condition_field, condition_data);
m_lastmsg = result;		

		ids = AddFrame(result);
		if (0>ids) 
		{
			EndFrame();
			return ids;
		}
	}
	else
	{
		nColumnCount = -4998;	
	}

	FreeDBFieldData();
	return nColumnCount;

EFS_END
return -1;

}

INT CESL_DataMgr::ESL_UpdateProc(
	   	TCHAR  *szTableName		        ,
		INT	  nColumnCount			,
		TCHAR  **pFieldName				,
		TCHAR  **pFieldData				,
		INT	  *InDataType				,
		TCHAR *szPKAttributeName		,
		TCHAR *szTablePrimaryKey		
)
{
	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	
	
	INT ids = UpdateProc(
		m_nRequestID			,
		username.GetBuffer(0)	,
		m_dbpwd.GetBuffer(0)    ,	
		szTableName		        ,
		nColumnCount			,
		pFieldName				,
		pFieldData				,
		InDataType				,
		szPKAttributeName		,
		szTablePrimaryKey		,
		FALSE					,	
		TRUE
		);
		
	if ( ids )
	{
		AfxMessageBox(_T("Error!!!"));
		return -1;
	}
	
	
	return 0;
}

INT CESL_DataMgr::MakeInsertFrame(CString table_name)
{
EFS_BEGIN

	CheckManageCode();

	INT nColumnCount = query.GetCount();
	if (0 == nColumnCount) 
	{	
		EndFrame();
		return -4010;
	}

	if(  CESL_SearchEngine::GetUsed() )
		DeltaUpdate_Find_Core(table_name);

	INT ids; 

	if (1==m_dbkind) 
	{	
		CString username;
		username.Format(_T("%s@%s"), m_dbid, m_dbservice);
		
		INT		i;
		INT		j;
		TCHAR	szTableName[512];
		
		_stprintf(szTableName, _T("%s"), (LPCTSTR)table_name);
		
		INT	   *InDataType = NULL;
		TCHAR **pFieldName = NULL;
		TCHAR **pFieldData = NULL;
		
		InDataType = (INT   *)malloc(sizeof(INT)   *nColumnCount);
		pFieldName = (TCHAR**)malloc(sizeof(TCHAR*)*nColumnCount);
		pFieldData = (TCHAR**)malloc(sizeof(TCHAR*)*nColumnCount);
		
		if (NULL==InDataType || NULL==pFieldName || NULL==pFieldData) 
		{
			if (InDataType) free(InDataType);
			if (pFieldName) free(pFieldName);
			if (pFieldData) free(pFieldData);
			EndFrame();
			return -4009;
		}
		
		POSITION pos = query.GetHeadPosition();
		query_field * field;
		for (i = 0; i < nColumnCount; i++) 
		{
			field = NULL;
			field = (query_field *)query.GetNext(pos);
			if (field == NULL) continue;
			
			if (field->type == 1)
			{	
				InDataType[i] = 2;
			}
			else if (field->type == 2)
			{	
				InDataType[i] = 1;
			}
			else 
			{
				EndFrame();
				return -4998;
			}
			
			pFieldName[i] = (TCHAR*)malloc(sizeof(TCHAR)*field->field.GetLength()+sizeof(TCHAR));
			pFieldData[i] = (TCHAR*)malloc(sizeof(TCHAR)*field->data.GetLength()+sizeof(TCHAR));
			
			if (NULL==pFieldName[i] || NULL==pFieldData[i]) 
			{
				free(InDataType);
				for (j = 0; j < i; j++) 
				{
					if (pFieldName[j]) free(pFieldName[j]);
					if (pFieldData[j]) free(pFieldData[j]);
				}
				free(pFieldName);
				free(pFieldData);
				EndFrame();
				return -4009;
			}
			
			_tcscpy(pFieldName[i], field->field.GetBuffer(0));
			_tcscpy(pFieldData[i], field->data.GetBuffer(0));	

			
			delete field;
		}
		query.RemoveAll();

		BOOL bFind;
		bFind = FALSE;

		for ( i=0 ; i<nColumnCount ; i++ )
		{
			if ( _tcscmp(pFieldName[i], _T("MANAGE_CODE")) == 0 )
			{
				bFind = TRUE;
				break;
			}
		}

		CString strExceptTable[] = {
			_T("BO_ACQ_GROUP_TBL"),_T("BO_ACQ_SEQ_NO_TBL"),_T("BO_BOOK_CHK_RESULT_TBL"),_T("BO_BOOK_TBL"),_T("BO_BOOK_TBL B"), //4
			_T("BO_BOOK_TBL BB"),_T("BO_DELETE_BOOK_TBL"),_T("BO_DELETE_SPECIES_TBL"),_T("BO_EBOOK_TBL"),_T("BO_FURNISH_TBL"),	//9
			_T("BO_PURCHASEINFO_TBL"),_T("BO_PURCHASE_SEQ_NO_TBL"),_T("BO_PURCHASE_SEQ_NO_TBL BPSN"),_T("BO_PURCHASE_SEQ_NO_TBL Q"),	//13
			_T("BO_PURCHASE_SEQ_NO_TBL S"),_T("BO_PURCHASE_WORK_NO_TBL"),_T("BO_SPECIES_TBL"),_T("BO_SPECIES_TBL S"),	//17
			_T("CO_ACCESSION_REC_TBL"),_T("CO_ACCESSION_REC_TBL A"),_T("CO_BOOK_CHK_AREA_TBL"),_T("CO_BUDGET_COMPILATION_SUB_TBL"),	//21
			_T("CO_BUDGET_COMPILATION_TBL"),_T("CO_BUDGET_EXECUTION_SUB_TBL"),_T("CO_BUDGET_EXECUTION_TBL"),_T("CO_COVER_INFO_TBL"),//25
			_T("CO_DROP_ACCESSION_NO_TBL"),_T("CO_EXCHANGE_RATE_TBL"),_T("CO_LAST_BOOK_ACQUISITNO_TBL"),_T("CO_LAST_NUMBER_TBL"),	//29
			_T("CO_LOAN_MAIL_SEND_TBL CLMS"),_T("CO_NON_DB_BOOK_TBL NB"),_T("CO_OFFLINE_LOAN_TBL"),	//32
			_T("CO_OFFLINE_LOAN_TBL OFF"),_T("CO_OMIT_BOOK_ACQUISITNO_TBL"),_T("CO_PREV_REG_NO_TBL"),_T("CO_RELATED_SUBJECT_NAME_TBL"),	//36
			_T("CO_TEMP_ACCESSION_REC_TBL"),_T("CO_TEMP_ACCESSION_REC_TBL "),_T("CO_USER_CARD_FORMAT_TBL"),_T("ECO_NOTICE_TBL"),	//40
			_T("ECO_NOTICE_TBL U"),_T("EDUTEST"),_T("ESL_MANAGE_TBL"),_T("IDX_ARTICLE_TBL"),_T("IDX_BO_TBL"),_T("IDX_BO_TBL I"),	//46
			_T("IDX_BO_TOC_TBL"),_T("IDX_SE_TBL"),_T("IDX_SE_TBL I"),_T("IDX_SE_TOC_TBL"),_T("LOAN_VIEW"),_T("MN_AUTHOR_RULE_TBL"),	//52
			_T("MN_LIB_GUIDANCE_TBL"),_T("MN_LIB_HOLIDAY_TBL"),_T("MN_LIB_LEGAL_HOLIDAY_TBL"),_T("MN_SMS_LOG_TBL"),	//56
			_T("OC3.LH_STORAGE_V01"),_T("SE_BINDING_SEQ_TBL"),_T("SE_BINDING_SEQ_TBL S"),_T("SE_BOOK_TBL"),_T("SE_BOOK_TBL B"),	//61
			_T("SE_BOOK_TBL BB"),_T("SE_DELETE_SPECIES_TBL"),_T("SE_DONATE_MANAGE_TBL"),_T("SE_PURCHASEINFO_TBL"),	//65
			_T("SE_PURCHASE_MANAGE_TBL"),_T("SE_PURCHASE_SEQ_NO_TBL"),_T("SE_PURCHASE_SPECIES_TBL"),_T("SE_PURCHASE_WORK_NO_TBL"),	//69
			_T("SE_RECEIPT_TBL"),_T("SE_RECEIPT_TBL R"),_T("SE_SPECIES_TBL"),_T("SE_SPECIES_TBL S"),_T("SE_VOL_TBL"),_T("SE_VOL_TBL V")	//75
			_T("BO_DONINFO_TBL"),_T("BO_DON_RECEIPT_TBL"), _T("MN_MAIL_LOG_TBL"), _T("RETURN_MAIL_LOG")	//79

		};
		INT		nExceptCnt = 79;

		if( bFind == FALSE )
		{
			for ( i=0 ; i<nExceptCnt ; i++ )
			{
				if ( szTableName == strExceptTable[i])
				{
					bFind = TRUE;
					break;
				}
			}

			if ( bFind == TRUE )
			{
				nColumnCount++;
				InDataType = (INT   *)realloc(InDataType, sizeof(INT)   *nColumnCount);
				pFieldName = (TCHAR**)realloc(pFieldName, sizeof(TCHAR*)*nColumnCount);
				pFieldData = (TCHAR**)realloc(pFieldData, sizeof(TCHAR*)*nColumnCount);

				pFieldName[nColumnCount-1] = (TCHAR*)malloc(sizeof(TCHAR)*11+sizeof(TCHAR));
				pFieldData[nColumnCount-1] = (TCHAR*)malloc(sizeof(TCHAR)*2+sizeof(TCHAR));
				if (NULL==pFieldName[nColumnCount-1] || NULL==pFieldData[nColumnCount-1]) 
				{
					if (NULL==pFieldName[i] || NULL==pFieldData[i]) 			
					free(InDataType);
					for (j = 0; j < i; j++) 
					{
						if (pFieldName[j]) free(pFieldName[j]);
						if (pFieldData[j]) free(pFieldData[j]);
					}
					free(pFieldName);
					free(pFieldData);
					EndFrame();
					return -4009;
				}
				else
				{
					_tcscpy(pFieldName[nColumnCount-1], _T("MANAGE_CODE"));
					_tcscpy(pFieldData[nColumnCount-1], this->MANAGE_CODE);
					InDataType[nColumnCount-1] = 1;
				}

			}
		}		
		ids = MakeInsertFrameProc(m_nRequestID		, 
								  szTableName		,
								  nColumnCount		,
								  pFieldName		,
								  pFieldData		,
								  InDataType		
								  );
		
		free(InDataType);
		for (i = 0; i < nColumnCount; i++) 
		{
			free(pFieldName[i]);
			free(pFieldData[i]);
		}
		free(pFieldName);
		free(pFieldData);
		
		if (0>ids) 
		{
			EndFrame();
			return ids;
		}

	}
	else if (2==m_dbkind) 
	{
		CString field_name, field_data, temp;
		
		query_field * field;
		for (INT i = 0; i < nColumnCount; i++) 
		{
			field = (query_field*)query.RemoveHead();
			if (!field) continue;
			
			temp.Format(_T("%s"), field->field);
			if (field_name.GetLength() > 0) field_name += _T(", ");
			field_name += temp;
			
			if		(field->type == 1) temp.Format(_T("%s"),   field->data);
			else if (field->type == 2) temp.Format(_T("'%s'"), field->data);			
			else 
			{
				EndFrame();
				return -4998;
			}

			if (0<i) field_data += _T(", ");
			field_data += temp;

			if (field) delete field;
		}
		query.RemoveAll();
		CString result;
		result.Format(_T("INSERT INTO %s (%s) VALUES (%s);"), table_name, field_name, field_data);
m_lastmsg = result;	

		ids = AddFrame(result);
		if (0>ids) 
		{
			EndFrame();
			return ids;
		}
	}
	else
	{
		nColumnCount = -4998;
	}
	
	FreeDBFieldData();
	return nColumnCount;


EFS_END
return -1;

}

INT CESL_DataMgr::MakeDeleteFrame(CString table_name, 
								  CString condition_field, 
								  CString condition_type, 
								  CString condition_data)
{
EFS_BEGIN

	CheckManageCode();

	CString result;
	condition_type.MakeUpper();
	if		(condition_type == _T("STRING") ) result.Format(_T("DELETE FROM %s WHERE %s = '%s';"), table_name, condition_field, condition_data);
	else if (condition_type == _T("NUMERIC")) result.Format(_T("DELETE FROM %s WHERE %s = %s;"),   table_name, condition_field, condition_data);
	else 
	{
		EndFrame();
		return -4998;
	}

	if(  CESL_SearchEngine::GetUsed() )
		DeltaUpdate_Find_Core(table_name);

	m_lastmsg = result;	
	INT ids = AddFrame(result);	
	if (0>ids) 
	{
		EndFrame();
		return ids;
	}

	FreeDBFieldData();
	return 0;

EFS_END
return -1;

}

CString CESL_DataMgr::GETDMAlias(VOID)
{
EFS_BEGIN

	return DB_MGR_ALIAS;

EFS_END
return _T("");

}

VOID CESL_DataMgr::SETDMAlias(CString alias)
{
EFS_BEGIN

	DB_MGR_ALIAS = alias;
	return;

EFS_END
}

INT CESL_DataMgr::InitDMPKFromMemory(CESL_DataMgr *pDM, CString dm_pk)
{
EFS_BEGIN

	if (pDM == NULL) return -1;

	INT i;
	INT totalcount = pDM->GetRowCount();
	CString szbuf1, szbuf2;

	TCHAR * cell = NULL;
	DWORD *row;
	CString strTmp;

	POSITION pos = pDM->data.GetHeadPosition();

	for (i = 0; i < totalcount; i++) {

		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);

		if (dm_pk.Compare((TCHAR*)row[0]) == 0) {
			DB_MGR_ALIAS.Format(_T("%s"), (TCHAR *)row[1]);

			szbuf2.Format(_T("%s"), (TCHAR *)row[2]);
			SetCONNECTION_INFO(szbuf2);

			TBL_NAME.Format(_T("%s"), (TCHAR *)row[3]);

			CONDITION.Format(_T("%s"), (TCHAR *)row[4]);

			EXTRA_CONDITION.Format(_T("%s"), (TCHAR *)row[5]);
			break;
		}
	}
	FreeDMRef();
	reference * pRef;

	pos = pDM->data.GetHeadPosition();

	for (i = 0; i < totalcount; i++) {

		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);
		pRef = NULL;

		if (dm_pk.Compare((TCHAR*)row[0]) == 0) {
			pRef = new reference;
			if (!pRef) {
				FreeDMRef();
				return -1;
			}
			pRef->FIELD_ALIAS.Format(_T("%s"), (TCHAR *)row[6]);
			pRef->FIELD_NAME.Format(_T("%s"), (TCHAR *)row[7]);
			pRef->FIELD_TYPE.Format(_T("%s"), (TCHAR *)row[8]);
			pRef->INIT_VALUE.Format(_T("%s"), (TCHAR *)row[9]);
			pRef->DATA_TYPE.Format(_T("%s"), (TCHAR *)row[10]);
			pRef->DATA_TYPE.MakeLower();

			RefList.AddTail((CObject*)pRef);
		}
	}
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::InitDMPKFromFile(CString strDMPK)
{
EFS_BEGIN

	CString strFileName;
	CString strManageCode;

	strFileName.Format(_T("..\\CFG\\Reference\\DM\\%s.DM"), strDMPK);
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// 정보파일을 찾을 수 없음
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	TCHAR szBuf[16009];
	CString strLine;
	INT pos;
	
	fd.ReadString(szBuf, 16000);	

	fd.ReadString(szBuf, 16000);
	strLine.Format(_T("%s"), szBuf);
	strLine.TrimLeft();		strLine.TrimRight();
	pos = strLine.Find(_T("="));
	DB_MGR_ALIAS.Format(_T("%s"), strLine.Mid(pos+1));

	fd.ReadString(szBuf, 16000);
	strLine.Format(_T("%s"), szBuf);
	strLine.TrimLeft();		strLine.TrimRight();
	pos = strLine.Find(_T("="));
	SetCONNECTION_INFO(strLine.Mid(pos+1));

	fd.ReadString(szBuf, 16000);
	strLine.Format(_T("%s"), szBuf);
	strLine.TrimLeft();		strLine.TrimRight();
	pos = strLine.Find(_T("="));
	TBL_NAME.Format(_T("%s"), strLine.Mid(pos+1));

	fd.ReadString(szBuf, 16000);
	strLine.Format(_T("%s"), szBuf);
	strLine.TrimLeft();		strLine.TrimRight();
	pos = strLine.Find(_T("="));
	CONDITION.Format(_T("%s"), strLine.Mid(pos+1));

	fd.ReadString(szBuf, 16000);
	strLine.Format(_T("%s"), szBuf);
	strLine.TrimLeft();		strLine.TrimRight();
	pos = strLine.Find(_T("="));
	EXTRA_CONDITION.Format(_T("%s"), strLine.Mid(pos+1));

	FreeDMRef();
	reference * pRef;

	if(0 <= EXTRA_CONDITION.Find(_T("UDF_MANAGE_CODE")) || 0 <= CONDITION.Find(_T("UDF_MANAGE_CODE")))
	{
		CheckManageCode();
		strManageCode = _T("'") + MANAGE_CODE + _T("'");
		CONDITION.Replace( _T("UDF_MANAGE_CODE"), strManageCode);
		EXTRA_CONDITION.Replace( _T("UDF_MANAGE_CODE"), strManageCode);
	}

	INT nStart = 0;
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();
		
		nStart = 0;

		pRef = NULL;
		pRef = new reference;		
		if (!pRef) continue; 
		pos = strLine.Find('|', nStart);
		pRef->FIELD_ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		pRef->FIELD_NAME = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		pRef->FIELD_TYPE = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		pRef->INIT_VALUE = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		pRef->DATA_TYPE = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;
		pRef->DATA_TYPE.MakeLower();

		RefList.AddTail((CObject*)pRef);

	}
	fd.Close();
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::FreeDMRef(VOID)
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return 0;
	reference * pRef;
	for (INT i = 0; i < refcount; i++) {
		pRef = (reference*)RefList.RemoveHead();
		if (pRef) delete pRef;
	}
	RefList.RemoveAll();
	return(refcount);

EFS_END
return -1;

}

INT CESL_DataMgr::InitDataMgrRef(INT ncolumns)
{
EFS_BEGIN

	FreeDMRef();
	reference * pRef;
	for (INT i = 0; i < ncolumns; i++) {
		pRef = NULL;
		pRef = new reference;
		if (!pRef) return -1;
		RefList.AddTail((CObject*)pRef);
	}
	return ncolumns;

EFS_END
return -1;

}

INT CESL_DataMgr::SetDataMgrRef(INT ncolumn, CString FIELD_ALIAS, CString FIELD_NAME, CString FIELD_TYPE, CString INIT_VALUE, CString DATA_TYPE)
{
EFS_BEGIN

	POSITION pos = RefList.FindIndex(ncolumn);
	if (pos == NULL) return -1;
	
	reference * pRef = (reference*)RefList.GetAt(pos);
	if (pRef == NULL) return -2;

	pRef->FIELD_ALIAS = FIELD_ALIAS;
	pRef->FIELD_NAME = FIELD_NAME;
	pRef->FIELD_TYPE = FIELD_TYPE;
	pRef->INIT_VALUE = INIT_VALUE;
	pRef->DATA_TYPE = DATA_TYPE;

	return 1;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeRecKey(CString &result, CString table_name, CString rec_key_field)
{
EFS_BEGIN

	if (m_dbkind == 1) return (ORACLE_MakeRecKey(result, table_name));

	if (m_dbkind == 2) 
	{
		INT ids  ;
		ids = MySQL_MakeRecKey(result, table_name) ;
		if(result.GetLength () == 0)
        {
			for(INT i = 0 ; i < 3 ; i++)
			{
				Sleep (100) ;
				ids = MySQL_MakeRecKey(result, table_name) ;
				if(result.GetLength () >0) 
				{
					return ids ;
				}
				
			}
		}

		return ids ;
	}

	return -1;

EFS_END
return -1;

}

INT CESL_DataMgr::MySQL_MakeRecKey(CString table_name, CString rec_key_field, CString &result)
{
EFS_BEGIN

	CString query;
	query.Format(_T("SELECT max(%s)+1 FROM %s"), rec_key_field, table_name);
	return GetOneValueQuery(query, result);

EFS_END
return -1;

}

INT CESL_DataMgr::MySQL_MakeRecKey(CString &result, CString SEQ)
{
EFS_BEGIN
	if (SEQ.GetLength() == 0) SEQ = DEFAULT_SEQ_NAME;

	TCHAR szQuery[512], szUQuery[512];
	CString UQuery;
	_stprintf(szUQuery, _T("UPDATE SEQ_TBL SET SEQ=SEQ+1 WHERE ID='%s';"), SEQ);
	_stprintf(szQuery, _T("SELECT SEQ FROM SEQ_TBL WHERE ID='%s';"), SEQ);

#ifdef _MBCS

	MYSQL_RES *pMyRes;
	MYSQL_ROW MyRow;

	MYSQL_CONNECT ;
	if (mysql_ping(m_pMysql) != 0) return -4004;	
	
	INT nResult_Lock, nResult_UNLock;

	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0));  

	nResult_Lock = mysql_query(m_pMysql, _T("LOCK TABLE SEQ_TBL WRITE"));   

	INT nResult2 = mysql_query(m_pMysql, szUQuery);   
	INT nResult3 = mysql_query(m_pMysql, szQuery);   

	if(mysql_errno(m_pMysql)) 
	{
		return -4005;	
	}
	pMyRes =  mysql_store_result(m_pMysql);	
	UINT num_fields;
	unsigned __int64 num_row;

	num_row    = mysql_num_rows(pMyRes);	
	num_fields = mysql_num_fields(pMyRes);	
	if (num_row == 0) return -4007;
	if (num_fields == 0) return -4007;

	MyRow = mysql_fetch_row(pMyRes);
	if (MyRow[0] == NULL) {
		result = _T("0");
		return 0;
	}
	result.Format(_T("%s"), MyRow[0]);

	mysql_free_result(pMyRes);				

	nResult_UNLock = mysql_query(m_pMysql, _T("UNLOCK TABLES;"));  
			
#endif
	return 0;
EFS_END
return -1;

}

VOID CESL_DataMgr::QueryLog(TCHAR * query, INT ids)
{
EFS_BEGIN

	if ( _tcsstr(query, _T("ESL_TOOLBAR_STATISTICS_TBL")) != NULL ) return;	

	if (query == NULL) return;

	SYSTEMTIME t;
	GetLocalTime(&t);
	CString filename, msg;
	filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d"), t.wYear, t.wMonth, t.wDay);
	msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d.%03d][%s][%05d] %s;"), t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds, DB_MGR_ALIAS, ids, query);

	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));

	BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));

	if(FALSE == bFind && FALSE == bNoLogFind)
	{
		filename = filename + _T(".tunning");

		CFileFind filefind;
		UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
		if(TRUE == filefind.FindFile(filename))
		{
			nOpenFlags ^= CFile::modeCreate;
		}
		
		CFile file;
		BOOL bIsOpen = file.Open(filename, nOpenFlags);
		if(TRUE != bIsOpen) return;
		
		file.SeekToEnd();
		
		{
			const INT nLen = msg.GetLength();
			
			TCHAR *pChar = new TCHAR[nLen+1];
			memset(pChar, NULL, nLen);
			pChar[nLen] = '\0';
			_tcscpy(pChar, (TCHAR*)msg.GetBuffer(0));
			
			for(INT nCnt=0; nCnt < nLen; nCnt++)
			{
				pChar[nCnt] ^= 0xAA;
			}
			
			file.WriteHuge(pChar, sizeof(TCHAR)*nLen);
			delete []pChar;
		}
		
		{
			CString strLine = _T("\r\n");	
			const INT LEN = strLine.GetLength();
			TCHAR *szLine = new TCHAR[LEN+1];
			memset(szLine, NULL, LEN);
			szLine[LEN] = '\0';
			_tcscpy(szLine, (TCHAR*)strLine.GetBuffer(0));
			
			for(INT nCnt=0; nCnt < LEN; nCnt++)
			{
				szLine[nCnt] ^= 0xAA;
			}
			file.Write(szLine, sizeof(TCHAR)*LEN);
			delete []szLine;
		}
		
		file.Close();
	}
	else if(FALSE == bNoLogFind)
	{
		filename = filename + _T(".txt");

		FILE* fp;
		
		fp = _tfopen (filename, _T("a+b"));
		if (fp == NULL)
		{
			return ;
		}

		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}
		
		_ftprintf (fp, _T("%s\r\n"), msg);
		
		fclose(fp);
	}

EFS_END
}


INT CESL_DataMgr::ORACLE_MakeRecKey(CString& key, CString SEQ)
{
EFS_BEGIN

	key.Empty();

	CString sql;
	if (SEQ.GetLength() == 0) {
		SEQ.Format(_T("%s"), DEFAULT_SEQ_NAME);
	}
	sql.Format(_T("SELECT %s.NEXTVAL FROM DUAL"), SEQ);

	const INT nData = 1;
	CString pData[nData];

	INT nError;
	m_nLastMsgID = 0;
	nError = GetValuesQuery(sql, pData, nData);
	if (nError == ERR_SESSION_MAX) 
	{
		m_nLastMsgID = ERR_SESSION_MAX;
		return ERR_SESSION_MAX;
	}
	if ( nError < 0 ) return -1;

	key = pData[0];
	if ( key.IsEmpty() ) return -1;

	return 0;

EFS_END
return -1;

}

VOID CESL_DataMgr::SetDB(CString dbkind)
{
EFS_BEGIN

	if (dbkind == _T("ORACLE")) {
		m_dbkind = 1;
		return;
	}
	m_dbkind = 1;
	return;

EFS_END
}

INT CESL_DataMgr::MYSQL_RestructDataManager(CString sQuery, CProgressCtrl *pBar)
{
EFS_BEGIN

#ifdef _MBCS

	MYSQL_RES *pMyRes;
	MYSQL_ROW MyRow;

	MYSQL_CONNECT ;
	if (mysql_ping(m_pMysql) != 0) return -4004;
	
	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0)); 
	INT nResult2 = mysql_query(m_pMysql, sQuery.GetBuffer(0));   
	QueryLog(sQuery.GetBuffer(0), nResult2);
	if (pBar) pBar->SetPos(10);
	if(mysql_errno(m_pMysql)) 
	{	
		return -4005;
	}
	pMyRes =  mysql_store_result(m_pMysql);		 
	if (pBar) pBar->SetPos(50);
	UINT num_fields, i;
	unsigned __int64 num_row;

	num_row    = mysql_num_rows(pMyRes);		 
	num_fields = mysql_num_fields(pMyRes);

	INT count=0;
	if ( num_row > MAX_ROW) return -4006;
	DWORD * row;
	while ( MyRow = mysql_fetch_row(pMyRes) )
	{
		row = NULL;
		row = MakeRow(num_fields);
		DWORD *lengths;
		lengths = mysql_fetch_lengths(pMyRes);
		for(i = 0; i < num_fields; i++)
		{
			if (MyRow[i] == NULL) continue;
			if (_tcsclen(MyRow[i]) > 0) {
				SetCellData(row, i, MyRow[i]);
			} else {
				SetCellData(row, i, _T(""));
			}
		}
		data.AddTail((CObject*)row);
		count++;
		if (pBar) pBar->SetPos(50+(INT)(50*((double)count/(INT)num_row)));
	}
	RefreshPositionInfo();
	m_nCols = num_fields;

	mysql_free_result(pMyRes);			
	
	if (pBar) pBar->SetPos(100);
#endif
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MYSQL_RefreshDataManager(CString option)
{
EFS_BEGIN

#ifdef _MBCS
	INT colindex[256];
	CString sQuery = MakeSelectQuery(option, colindex);
	if (sQuery.GetLength() == 0) return -4005;
	m_lastmsg = sQuery;

	
	MYSQL_RES *pMyRes;
	MYSQL_ROW MyRow;

	MYSQL_CONNECT ;
	if (mysql_ping(m_pMysql) != 0) return -4004;
	
	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0)); 
	INT nResult2 = mysql_query(m_pMysql, sQuery.GetBuffer(0));   
	QueryLog(sQuery.GetBuffer(0), nResult2);

	if(mysql_errno(m_pMysql)) 
	{	
		return -4005;
	}
	pMyRes =  mysql_store_result(m_pMysql);
	UINT num_fields, i;
	unsigned __int64 num_row;

	num_row    = mysql_num_rows(pMyRes);
	num_fields = mysql_num_fields(pMyRes);

	if ( num_fields > MAX_ROW) return -4006;
	DWORD * row;
	INT totalcolumns = RefList.GetCount();
	while ( MyRow = mysql_fetch_row(pMyRes) )
	{
		row = NULL;
		row = MakeRow(totalcolumns);
		InitRow(row);
		DWORD *lengths;
		lengths = mysql_fetch_lengths(pMyRes);
		for(i = 0; i < num_fields; i++)
		{
			if (MyRow[i] == NULL) continue;
			if (_tcsclen(MyRow[i]) > 0) SetCellData(row, colindex[i], MyRow[i]);
				else  SetCellData(row, colindex[i], _T(""));
		}
		data.AddTail((CObject*)row);
	}
	m_nCols = totalcolumns;
	RefreshPositionInfo();

	mysql_free_result(pMyRes);			
#endif
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MYSQL_GetOneValueQuery(CString sQuery, CString &result, INT nCommit/*=TRUE*/)
{
EFS_BEGIN

	result = _T("");
#ifdef _MBCS

	
	MYSQL_RES *pMyRes;
	MYSQL_ROW MyRow;


	MYSQL_CONNECT ;
	if (mysql_ping(m_pMysql) != 0) return -4004;
	
	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0)); 
	INT nResult2 = mysql_query(m_pMysql, sQuery.GetBuffer(0));  
	QueryLog(sQuery.GetBuffer(0), nResult2);

	if(mysql_errno(m_pMysql))  
	{		
		return -4005;
	}
	pMyRes =  mysql_store_result(m_pMysql);		
	UINT num_fields;
	unsigned __int64 num_row;

	num_row    = mysql_num_rows(pMyRes);		
	num_fields = mysql_num_fields(pMyRes);		
	if (num_row == 0) return -4007;
	if (num_fields == 0) return -4007;

	MyRow = mysql_fetch_row(pMyRes);
	if (MyRow[0] == NULL) {
		result = _T("");
		return 0;
	}
	result.Format(_T("%s"), MyRow[0]);

	mysql_free_result(pMyRes);					
#endif
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MYSQL_GetValuesQuery(CString sQuery, CString result[], INT nresult, INT nCommit/*=TRUE*/)
{
EFS_BEGIN

#ifdef _MBCS

	MYSQL_RES *pMyRes;
	MYSQL_ROW MyRow;

	MYSQL_CONNECT ;
	if (mysql_ping(m_pMysql) != 0) return -4004;
	
	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0));  
	INT nResult2 = mysql_query(m_pMysql, sQuery.GetBuffer(0));   
	QueryLog(sQuery.GetBuffer(0), nResult2);

	if(mysql_errno(m_pMysql)) 
	{	
		return -4005;
	}
	pMyRes =  mysql_store_result(m_pMysql);
	UINT num_fields, i;
	unsigned __int64 num_row;

	num_row    = mysql_num_rows(pMyRes);		  
	num_fields = mysql_num_fields(pMyRes);		  
	if (num_row == 0) return -4007;	
	if (num_fields < (unsigned)nresult) return -4008;

	MyRow = mysql_fetch_row(pMyRes);
	for(i = 0; i < (unsigned)nresult; i++)
	{
		if (MyRow[i] == NULL) continue;
		if (_tcsclen(MyRow[i]) > 0) result[i] = MyRow[i];
	}

	mysql_free_result(pMyRes);		
	
	return num_fields;
#endif
	return -1;

EFS_END
return -1;

}

INT CESL_DataMgr::MYSQL_ExecuteQuery(CString sQuery, INT commitmode, INT nCommit/*=TRUE*/)
{
EFS_BEGIN

#ifdef _MBCS


	MYSQL_CONNECT ;

	if (mysql_ping(m_pMysql) != 0) return -4004;
	
	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0));  
	INT nResult2 = mysql_query(m_pMysql, _T("SET autocommit=0;"));   
	QueryLog(_T("SET autocommit=0;"), nResult2);
	INT nResult3 = mysql_query(m_pMysql, sQuery.GetBuffer(0));  
	QueryLog(sQuery.GetBuffer(0), nResult3);
	if (nResult3 != 0) return -4005;
	INT nResult4 = mysql_query(m_pMysql, _T("commit;"));   
	QueryLog(_T("commit;"), nResult3);

	if(mysql_errno(m_pMysql))  
	{	
		return -4005;
	}

#endif
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MYSQL_SendFrame(VOID)
{
EFS_BEGIN

#ifdef _MBCS
	INT querycount = frame.GetCount();

	MYSQL_RES *pMyRes;
	MYSQL_CONNECT ;
	if (mysql_ping(m_pMysql) != 0) return -4004;
	
	INT nResult1 = mysql_select_db(m_pMysql, m_dbservice.GetBuffer(0));
	INT nResult2 = mysql_query(m_pMysql, _T("SET autocommit=0;"));   
	QueryLog(_T("SET autocommit=0;"));

	INT nResult3;
	CString * query;
	POSITION pos = frame.GetHeadPosition();
	INT successcount = 0;
	BOOL nSuccessFlag = TRUE;
	for (INT i = 0; i < querycount; i++) {
		query = NULL;
		query = (CString*)frame.GetNext(pos);
		if (query == NULL) continue;
		if (query->GetLength() == 0) continue;
		nResult3 = mysql_query(m_pMysql, query->GetBuffer(0));   
		QueryLog(query->GetBuffer(0), nResult3);

		pMyRes =  mysql_store_result(m_pMysql);
		if(pMyRes)
		{
			mysql_free_result(pMyRes);	
		}

		if (nResult3 == 0) {
			successcount++;
		} else {
			nSuccessFlag = FALSE;
			break;
		}
	}

	INT nResult4;
	if (nSuccessFlag == TRUE) {
		nResult4 = mysql_query(m_pMysql, _T("commit;"));   
		QueryLog(_T("commit"));
	} else {
		nResult4 = mysql_query(m_pMysql, _T("rollback;"));   
		QueryLog(_T("rollback"));
	}
	if(mysql_errno(m_pMysql)) 
	{
		return -4005;
	}
	if (nSuccessFlag == FALSE) return -4005;
	return successcount;
#endif
	return -1;

EFS_END
return -1;

}

INT CESL_DataMgr::GetBaseFieldData(INT rowidx, TCHAR * fieldalias, TCHAR **outData)
{
EFS_BEGIN

	CString field;
	field.Format(_T("%s"), fieldalias);

	CString result;
	INT ids;
	ids = GetCellData(fieldalias, rowidx, result);
	*outData = (TCHAR*)malloc(sizeof(TCHAR)*(result.GetLength()+1));
	_stprintf(*outData, _T("%s\0"), result);
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::CheckDataObjectAlias(TCHAR *data)
{
EFS_BEGIN

	if (data == NULL) {
		if (DB_MGR_ALIAS.GetLength() == 0) return true;
			else return false;
	}
	CString temp;
	temp.Format(_T("%s"), data);
	temp.MakeUpper();
	DB_MGR_ALIAS.MakeUpper();
	if (DB_MGR_ALIAS == data) return true;
	return false;

EFS_END
return -1;

}

INT CESL_DataMgr::LOBUpdate(CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString fieldname, BYTE *data, INT datasize)
{
EFS_BEGIN

	if (m_dbkind == 1) {
		return ORACLE_LOBUpdate(table_name, PKFieldname, PKFieldtype, PKData, fieldname, data, datasize);
	}

	return -4999;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_LOBUpdate(CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString fieldname, BYTE *data, INT datasize)
{
EFS_BEGIN

	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);

	INT ids;
	m_nLastMsgID = 0;


	ids = UpdateLobProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), table_name.GetBuffer(0), fieldname.GetBuffer(0), data, datasize, PKFieldname.GetBuffer(0), PKData.GetBuffer(0), TRUE);
	if (ids == ERR_SESSION_MAX) 
	{
		m_nLastMsgID = ERR_SESSION_MAX;
		RollbackProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0));
		return ERR_SESSION_MAX;
	}
	if (ids) 
	{
		RollbackProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0));
		return -2;
	}

	ORACLE_Commit();
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::LOBGet(CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString fieldname, BYTE *result, INT *resultsize, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN

	if (!result) return -1;
	if (m_dbkind == 1) 
	{
		return ORACLE_LOBGet(table_name, PKFieldname, PKFieldtype, PKData, fieldname, result, resultsize, pBind);
	}

	return -4999;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_LOBGet(CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString fieldname, BYTE *result, INT *resultsize, EDBM_BIND_DATA_TYPE *pBind /*= NULL*/)
{
EFS_BEGIN

    INT  ids;
    INT  nRowCount;
    INT  nColumnCount;

    CString query;

    DB_DATA_ARRAY_TYPE* pData;

	CString username;
	username.Format(_T("%s@%s"), m_dbid, m_dbservice);
	m_nLastMsgID = 0;


	PKFieldtype.MakeUpper();
	if (PKFieldtype == _T("NUMERIC")) 
	{
		query.Format(_T("SELECT %s FROM %s WHERE %s=%s"), fieldname, table_name, PKFieldname, PKData);
	} 
	else if (PKFieldtype == _T("STRING")) 
	{
		query.Format(_T("SELECT %s FROM %s WHERE %s='%s'"), fieldname, table_name, PKFieldname, PKData);
	} 
	else return -4999;
	
	nRowCount = 0;
	ids = SelectProc(m_nRequestID, username.GetBuffer(0), m_dbpwd.GetBuffer(0), query.GetBuffer(0), &nRowCount, &nColumnCount, &pData, pBind);
    if (ids)
    {
		if (nRowCount == 0 )
		{
			*resultsize = 0;
			result = NULL;
			return 0;
		}
        return ids;
    }
	if (nColumnCount < 1) return -3;
	if (nRowCount < 1)    return -4;
	*resultsize = pData[0].pDBDataUnion[0].BinData.nDataSize;
	memcpy(result, pData[0].pDBDataUnion[0].BinData.pBinaryData, pData[0].pDBDataUnion[0].BinData.nDataSize);

    FreeDBDataArrayType(pData, nColumnCount);
	return 0;

EFS_END
return -1;

}

CString CESL_DataMgr::GetDefaultSEQName(VOID)
{
EFS_BEGIN

	CString result;
	result.Format(_T("%s"),DEFAULT_SEQ_NAME);
	return result;

EFS_END
return _T("");

}

INT CESL_DataMgr::MakeControlMgrFiledData(CObject * pOCM)
{
EFS_BEGIN

	CESL_ControlMgr *pCM = (CESL_ControlMgr *)pOCM;
	if (!pCM) return -1030;

	INT ids;
	CString field_name, field_type, data;
	INT cecount = pCM->Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) 
	{
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE) 
		{
			ids = FindColumnFieldName(pCE->CTRL_ALIAS, field_name, field_type);

			field_type.MakeLower();

			if (ids != 0) continue;

			if (pCE->CTRL_TYPE == _T("M"))	pCE->GetControlMgrData(data, -1, 0);
			else						pCE->GetControlMgrData(data);		
			
			if (field_type == _T("int"))	AddDBFieldData(field_name, _T("NUMERIC"), data, 1);
			else						AddDBFieldData(field_name, _T("STRING"), data, 1);
		}
	}

	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeControlMgrInsertFrame(VOID)
{
EFS_BEGIN

	return MakeInsertFrame(TBL_NAME);

EFS_END
return -1;

}

INT CESL_DataMgr::MakeControlMgrUpdateFrame(CString condition_field, CString condition_type, CString condition_data, INT RowIdx)
{
EFS_BEGIN

	return MakeUpdateFrame(TBL_NAME, condition_field, condition_type, condition_data, RowIdx);

EFS_END
return -1;

}

INT CESL_DataMgr::MakeControlMgrUpdateField(CString field_alias, CString data)
{
EFS_BEGIN

	CString field_name, field_type;
	if (FindColumnFieldName(field_alias, field_name, field_type) == -1) return -1031;
	INT count = query.GetCount();
	POSITION pos;
	query_field * QF;
	pos = query.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		QF = NULL;
		QF = (query_field *)query.GetNext(pos);
		if (QF && QF->field == field_name) {
			QF->data = data;
			return 0;
		}
	}
	return -1031;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeControlMgrRemoveField(CString field_alias)
{
EFS_BEGIN

	CString field_name, field_type;
	if (FindColumnFieldName(field_alias, field_name, field_type) == -1) return -1031;
	INT count = query.GetCount();
	POSITION pos;
	query_field * QF;
	INT DelIdx = -1;
	pos = query.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		QF = NULL;
		QF = (query_field *)query.GetNext(pos);
		if (QF && QF->field == field_name) {
			DelIdx = i;
			break;
		}
	}
	if (DelIdx == -1) return -1031;
	pos = query.FindIndex(DelIdx);
	QF = (query_field *)query.GetAt(pos);
	if (QF) {
		delete QF;
		QF = NULL;
	}
	query.RemoveAt(pos);
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::GetAllAlias(CString alias[], INT nAlias)
{
EFS_BEGIN

	INT refcount = RefList.GetCount();
	if (refcount == 0) return -1;
	reference * pRef;
	POSITION pos;
	INT count = 0;
	pos = RefList.GetHeadPosition();
	for(INT i = 0; i < refcount; i++) {
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		if (pRef) {
			alias[count] = pRef->FIELD_ALIAS;
			count++;
			if (nAlias <= count) return count;
		}
	}
	return refcount;	

EFS_END
return -1;

}

INT CESL_DataMgr::GetColumnRefInfo(INT nCol, CString &FIELD_ALIAS, CString &FIELD_NAME, CString &FIELD_TYPE, CString &INIT_VALUE, CString &DATA_TYPE)
{
EFS_BEGIN

	POSITION pos;
	if (nCol >= RefList.GetCount() ) return -4501;
	pos = RefList.FindIndex(nCol);
	reference * pRef;
	pRef = (reference*)RefList.GetAt(pos);

	if (&pRef == NULL) return -4501;

	FIELD_ALIAS		=	pRef->FIELD_ALIAS;
	FIELD_NAME		=	pRef->FIELD_NAME;
	FIELD_TYPE		=	pRef->FIELD_TYPE;
	INIT_VALUE		=	pRef->INIT_VALUE;
	DATA_TYPE		=	pRef->DATA_TYPE;

	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeInsertFrameQuery(CString table_name, CString &strQuery)
{
EFS_BEGIN

	CheckManageCode();

	INT fieldcount = query.GetCount();
	if (fieldcount == 0) return -4010;

	CString field_name, field_data, temp;

	if(  CESL_SearchEngine::GetUsed() )
		DeltaUpdate_Find_Core(table_name);

	query_field * field;
	for (INT i = 0; i < fieldcount; i++) {
		field = (query_field*)query.RemoveHead();
		if (!field) continue;
		if (field_name.GetLength() == 0) {
			temp.Format(_T("%s"), field->field);
			field_name += temp;
		} else {
			temp.Format(_T(", %s"), field->field);
			field_name += temp;
		}

		if (field_data.GetLength() == 0) {
			if (field->type == 1) temp.Format(_T("%s"), field->data);
			if (field->type == 2) temp.Format(_T("'%s'"), field->data);			
			field_data += temp;
		} else {
			if (field->type == 1) temp.Format(_T(", %s"), field->data);
			if (field->type == 2) temp.Format(_T(", '%s'"), field->data);			
			field_data += temp;
		}
		if (field) delete field;
	}
	query.RemoveAll();
	strQuery.Format(_T("INSERT INTO %s (%s) VALUES (%s);"), table_name, field_name, field_data);
	return(fieldcount);

EFS_END
return -1;

}

INT CESL_DataMgr::MakeUpdateFrameQuery(CString table_name, CString condition_field, CString condition_type, CString condition_data, CString &strQuery)
{
EFS_BEGIN

	CheckManageCode();

	INT fieldcount = query.GetCount();
	if (fieldcount == 0) return -4010;
	
	if(  CESL_SearchEngine::GetUsed() )
		DeltaUpdate_Find_Core(table_name);

	CString update_data, temp;

	query_field * field;
	for (INT i = 0; i < fieldcount; i++) {
		field = (query_field*)query.RemoveHead();
		if (!field) continue;

		if (update_data.GetLength() == 0) {
			if (field->type == 1) temp.Format(_T("%s = %s"), field->field, field->data);
			if (field->type == 2) temp.Format(_T("%s = '%s'"), field->field, field->data);
			update_data += temp;
		} else {
			if (field->type == 1) temp.Format(_T(", %s = %s"), field->field, field->data);
			if (field->type == 2) temp.Format(_T(", %s = '%s'"), field->field, field->data);
			update_data += temp;
		}
		if (field) {
			delete field;
			field = NULL;
		}
	}
	query.RemoveAll();
	condition_type.MakeUpper();
	if (condition_type == _T("STRING")) strQuery.Format(_T("UPDATE %s SET %s WHERE %s = '%s';"), table_name, update_data, condition_field, condition_data);
	if (condition_type == _T("NUMERIC")) strQuery.Format(_T("UPDATE %s SET %s WHERE %s = %s;"), table_name, update_data, condition_field, condition_data);
	return(fieldcount);

EFS_END
return -1;

}

INT CESL_DataMgr::CopyDM(CESL_DataMgr *pSourceDM)
{
EFS_BEGIN

	FreeData();
 	INT nRow = pSourceDM->GetRowCount();
	INT nCurRow;
	INT row, col;
	CString strTmp;

	if (nRow == 0) return 0;
	for (row = 0; row < nRow; row++) {
		m_nCols = pSourceDM->m_nCols;
		InsertRow(-1);
		nCurRow = GetRowCount() - 1;
		for (col = 0; col < pSourceDM->m_nCols; col++) {
			SetCellData(nCurRow, col, pSourceDM->GetCellData(row, col));
		}
	}
	return 0;

EFS_END
return -1;

}


INT CESL_DataMgr::AddDM(CESL_DataMgr *pSourceDM, INT nDstRow/*=-1*/)
{
EFS_BEGIN

	if(-1 == nDstRow)
	{
 		INT nRow = pSourceDM->GetRowCount();
		if (nRow == 0)
		{
			return 0;		
		}
		for(INT i = 0; i < nRow; i++)
		{
			DWORD* row = (DWORD*)pSourceDM->data.GetAt(pSourceDM->m_pDataPosition[i]);
			if (row != NULL)
			{
				data.AddTail((CObject*)row);
				pSourceDM->data.RemoveAt(pSourceDM->m_pDataPosition[i]);
			}
		}

	}
	else
	{
		DWORD* row = (DWORD*)pSourceDM->data.GetAt(pSourceDM->m_pDataPosition[nDstRow]);
		if (row != NULL)
		{
			data.AddTail((CObject*)row);
			pSourceDM->data.RemoveAt(pSourceDM->m_pDataPosition[nDstRow]);
		}
	}
	pSourceDM->RefreshPositionInfo();
	RefreshPositionInfo();

	return 0;

EFS_END
return -1;
}


INT CESL_DataMgr::CopyDM2(CESL_DataMgr *pSourceDM)
{
EFS_BEGIN

	FreeData();
 	INT nRow = pSourceDM->GetRowCount();
	if (nRow == 0)
	{
		return 0;		
	}
	for (INT i = 0; i < nRow; i++) 
	{
		DWORD* row = (DWORD*)pSourceDM->data.GetAt(pSourceDM->m_pDataPosition[i]);
		if (row != NULL)
		{
			data.AddTail((CObject*)row);
		}
	}

	RefreshPositionInfo();

	return 0;

EFS_END
return -1;
}

INT CESL_DataMgr::GetParsedAlias(CString alias, CString &strMainAlias, CString &strSubAlias)
{
EFS_BEGIN

	strMainAlias = _T("");
	strSubAlias = _T("");
	INT pos = alias.Find('.', 0);
	if (pos == -1) {
		strMainAlias = alias;
	} else {
		strMainAlias = alias.Left(pos);
		strSubAlias = alias.Mid(pos + 1);
	}
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::GetMarcTagData(CString &strMarc, CString strItemAlias, CString &strResult)
{
EFS_BEGIN

	if (m_pMarcMgr == NULL) return -1;
	CMarc marc;
	INT ids = m_pMarcMgr->Decoding(strMarc, &marc);
	if (ids < 0) return -1;
	ids = m_pMarcMgr->GetItem(&marc, strItemAlias, strResult);
	if (ids >= 0) return 0;
		else return -1;

EFS_END
return -1;

}

INT CESL_DataMgr::SetMarcMgr(CMarcMgr *pMarcMgr)
{
EFS_BEGIN

	m_pMarcMgr = pMarcMgr;
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_OCIMgr_MakeUpdateFrame(CString strTBLName, 
												CString strPKName, 
												CString strPKType, 
												CString strPKData)
{
EFS_BEGIN

	return -9999;

EFS_END
return -1;

}

INT CESL_DataMgr::FindColumnFieldName(INT nColumn, CString &field_alias, CString &field_name, CString &field_type, CString &data_type, INT RowIdx, CString &strData)
{
EFS_BEGIN

	POSITION pos= RefList.FindIndex(nColumn);
	if (!pos) return -1;
	
	reference * pRef = (reference *)RefList.GetAt(pos);
	if (!pRef) return -1;
	field_alias = pRef->FIELD_ALIAS;
	field_name = pRef->FIELD_NAME;
	field_type = pRef->FIELD_TYPE;
	data_type = pRef->DATA_TYPE;

	GetCellData(pRef->FIELD_ALIAS, RowIdx, strData);
	
	return 0;
EFS_END
return -1;

}

INT CESL_DataMgr::GetRefColumnCount(VOID)
{
EFS_BEGIN

	return RefList.GetCount();

EFS_END
return -1;

}

INT CESL_DataMgr::MakeInsertFrameWithRow(INT nROW)
{
EFS_BEGIN

	CheckManageCode();

	INT ids=0;
    INT refcount = RefList.GetCount();
	if (refcount == 0) return -1;
	reference * pRef;
	POSITION pos;
	INT count = 0;
	pos = RefList.GetHeadPosition();
	CString tmpdata;

	for(INT i = 0; i < refcount; i++)
	{
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		if (pRef) {

			CString strTempFieldType;
			strTempFieldType = pRef->DATA_TYPE;	
			strTempFieldType.MakeLower();
			if(strTempFieldType == _T("udf")) continue;			
			
			GetCellData(pRef->FIELD_ALIAS,nROW,tmpdata);
			if(strTempFieldType == _T("int") && tmpdata.GetLength() == 0) tmpdata = _T("NULL");
			
			if(strTempFieldType == _T("str")) {
				AddDBFieldData(pRef->FIELD_NAME,_T("STRING"),tmpdata);
            }
			else if(strTempFieldType == _T("int")) {
				AddDBFieldData(pRef->FIELD_NAME,_T("NUMERIC"),tmpdata);
			}
            tmpdata = _T("");			
		}
	}

	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::GetFrameLog(CArray<CString, CString&> *pArray)
{
EFS_BEGIN

	if (pArray == NULL) return -1;

	CString strTemp;
	CString *pStr = NULL;
	
	POSITION pos = frame.GetHeadPosition();
	while (pos != NULL)
	{
		pStr = (CString*)frame.GetNext(pos);
		if (pStr == NULL) continue;

		strTemp.Format(_T("%s"), pStr->GetBuffer(0));

		pArray->Add(strTemp);
	}

	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeQueryByDM(CArray <CString, CString> &RemoveAliasList, 
								CESL_DataMgr *pDM, 
								CString strTBL, 
								INT RowIdx, 
								CESL_DataMgr *pDM_OUT, 
								CString condition_field, 
								CString condition_type, 
								CString condition_data, 
								INT nUpdateRowIdx)
{
EFS_BEGIN

	CheckManageCode();

	INT nColumn = pDM->GetRefColumnCount();
	if (nColumn == 0) return -2;
	INT nRow = pDM->GetRowCount();
	if (nRow == 0) return 0;

	INT ids;

	CString field_alias, field_name, field_type, data_type, strData;
	INT i;
	INT nRemoveAlias = RemoveAliasList.GetSize();
	BOOL IsRemoveAlias;
	for (INT col = 0; col < nColumn; col++) 
	{
		IsRemoveAlias = FALSE;
		strData = _T("");
		pDM->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, RowIdx, strData);
		data_type.MakeLower();
		if (data_type == _T("udf")) continue;
		for (i = 0; i < nRemoveAlias; i++) {
			if (field_alias == RemoveAliasList.GetAt(i)) {
				IsRemoveAlias = TRUE;
				break;
			}
		}
		if (IsRemoveAlias == FALSE) {
			field_type.MakeLower();
			if (field_type == _T("str")) pDM_OUT->AddDBFieldData(field_name, _T("STRING"), strData, 1);
			if (field_type == _T("int")) {
				strData.TrimLeft();				strData.TrimRight();
				if (strData.GetLength() > 0) pDM_OUT->AddDBFieldData(field_name, _T("NUMERIC"), strData);
				if (strData.GetLength() == 0) pDM_OUT->AddDBFieldData(field_name, _T("NUMERIC"), _T("NULL"));
			}
		}
	}
	if (condition_field.GetLength() > 0) 
	{
		ids = pDM_OUT->MakeUpdateFrame(strTBL, condition_field, condition_type, condition_data, nUpdateRowIdx);
	} 
	else 
	{			
		ids = pDM_OUT->MakeInsertFrame(strTBL);
	}
	return ids;

EFS_END
return -1;

}

INT CESL_DataMgr::MakeQueryByJoinedDM(CArray <CString, CString> &RemoveAliasList, CESL_DataMgr *pDM, CString strTBL, INT RowIdx, CESL_DataMgr *pDM_OUT, CString condition_field, CString condition_type, CString condition_data, INT nUpdateRowIdx)
{
EFS_BEGIN

	CheckManageCode();

	INT nColumn = pDM->GetRefColumnCount();
	if (nColumn == 0) return -2;
	INT nRow = pDM->GetRowCount();
	if (nRow == 0) return 0;

	INT ids;

	CString field_alias, field_name, field_type, data_type, strData;
	INT i;
	INT nRemoveAlias = RemoveAliasList.GetSize();
	BOOL IsRemoveAlias;
	for (INT col = 0; col < nColumn; col++) {
		IsRemoveAlias = FALSE;
		strData = _T("");
		pDM->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, RowIdx, strData);
		data_type.MakeLower();
		if (data_type == _T("udf")) continue;
		for (i = 0; i < nRemoveAlias; i++) {
			if (field_alias == RemoveAliasList.GetAt(i)) {
				IsRemoveAlias = TRUE;
				break;
			}
		}
		if (IsRemoveAlias == FALSE) {
			field_type.MakeLower();
		
			INT npos = field_name.Find(_T("."));
			if(npos>0) field_name = field_name.Mid(npos+1);
	
			if (field_type == _T("str")) pDM_OUT->AddDBFieldData(field_name, _T("STRING"), strData, 1);
			if (field_type == _T("int")) {
				strData.TrimLeft();				strData.TrimRight();
				if (strData.GetLength() > 0) pDM_OUT->AddDBFieldData(field_name, _T("NUMERIC"), strData);
				if (strData.GetLength() == 0) pDM_OUT->AddDBFieldData(field_name, _T("NUMERIC"), _T("NULL"));
			}
		}
	}
	if (condition_field.GetLength() > 0) {
		ids = pDM_OUT->MakeUpdateFrame(strTBL, condition_field, condition_type, condition_data, nUpdateRowIdx);
	} else {
		ids = pDM_OUT->MakeInsertFrame(strTBL);
	}
	return ids;

EFS_END
return -1;

}

INT CESL_DataMgr::ORACLE_SendFrame2(INT nSendCount)
{
EFS_BEGIN

	return -9999;

EFS_END
return -1;

}

INT CESL_DataMgr::SORT(CString strAlias1, BOOL IsDesc/*=FALSE*/)
{
EFS_BEGIN

	if (m_pDataPosition == NULL) return -4990;
	INT nColumn1 = FindColumn(strAlias1);
	if (nColumn1 < 0) return -1;
	reference *pRef = (reference*)RefList.GetAt(RefList.FindIndex(nColumn1));
	if (!pRef) return 0;

	INT ids;
	if (pRef->FIELD_TYPE == _T("str") || pRef->FIELD_TYPE == _T("STR")) ids = SORT_QUICKSORT(nColumn1, ESL_DATAMGR_TYPE_STR, 0, GetRowCount()-1, IsDesc);
	if (pRef->FIELD_TYPE == _T("int") || pRef->FIELD_TYPE == _T("INT")) ids = SORT_QUICKSORT(nColumn1, ESL_DATAMGR_TYPE_INT, 0, GetRowCount()-1, IsDesc);
				
	if (pRef->FIELD_TYPE == _T("")) ids = SORT_QUICKSORT(nColumn1, ESL_DATAMGR_TYPE_STR, 0, GetRowCount()-1, IsDesc);
	return ids;

EFS_END
return -1;

}

INT CESL_DataMgr::SORT(CString strAlias1, INT nStartRowIdx, INT nFinishRowIdx, BOOL IsDesc)
{
EFS_BEGIN

	if (m_pDataPosition == NULL) return -4990;
	INT nColumn1 = FindColumn(strAlias1);
	if (nColumn1 < 0) return -1;
	reference *pRef = (reference*)RefList.GetAt(RefList.FindIndex(nColumn1));
	if (!pRef) return 0;

	INT i, j;

	BOOL IsStr = TRUE;
	if (pRef->FIELD_TYPE == _T("int") || pRef->FIELD_TYPE == _T("INT")) IsStr = FALSE;	

	DWORD *pRowMaster, *pRowSlave;
	for (i = nStartRowIdx; i < nFinishRowIdx - 1; i++)
	{
		pRowMaster = (DWORD*)data.GetAt(m_pDataPosition[i]);
		if (pRowMaster == NULL) continue;

		for (j = i + 1; j < nFinishRowIdx; j++)
		{
			pRowSlave = (DWORD*)data.GetAt(m_pDataPosition[j]);
			if (pRowSlave == NULL) continue;
			
			if (IsDesc == FALSE)
			{
				if (IsStr) {
					if (StringCompare((TCHAR*)pRowMaster[nColumn1] , (TCHAR*)pRowSlave[nColumn1]) > 0)
					{
						data.SetAt(m_pDataPosition[i], (CObject*)pRowSlave);
						data.SetAt(m_pDataPosition[j], (CObject*)pRowMaster);

						pRowMaster = pRowSlave;
					}
				} else {
					if (String_to_Integer((TCHAR*)pRowMaster[nColumn1]) > String_to_Integer((TCHAR*)pRowSlave[nColumn1]))
					{
						data.SetAt(m_pDataPosition[i], (CObject*)pRowSlave);
						data.SetAt(m_pDataPosition[j], (CObject*)pRowMaster);

						pRowMaster = pRowSlave;
					}
				}
			}
			else if (IsDesc == TRUE)
			{
				if (IsStr) {
					if (StringCompare((TCHAR*)pRowMaster[nColumn1] , (TCHAR*)pRowSlave[nColumn1]) < 0)
					{
						data.SetAt(m_pDataPosition[i], (CObject*)pRowSlave);
						data.SetAt(m_pDataPosition[j], (CObject*)pRowMaster);

						pRowMaster = pRowSlave;
					}
				} else {
					if (String_to_Integer((TCHAR*)pRowMaster[nColumn1]) < String_to_Integer((TCHAR*)pRowSlave[nColumn1]))
					{
						data.SetAt(m_pDataPosition[i], (CObject*)pRowSlave);
						data.SetAt(m_pDataPosition[j], (CObject*)pRowMaster);

						pRowMaster = pRowSlave;
					}
				}
			}
		}
	}
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::StringCompare(TCHAR *pStr1, TCHAR *pStr2)
{
EFS_BEGIN

	if (pStr1 == NULL && pStr2 != NULL) return _tcsicmp(_T("") , pStr2);
	if (pStr1 != NULL && pStr2 == NULL) return _tcsicmp(pStr1, _T(""));
	if (pStr1 == NULL && pStr2 == NULL) return _tcsicmp(_T(""), _T(""));
	return _tcsicmp(pStr1 , pStr2);
EFS_END
return -1;

}

INT CESL_DataMgr::String_to_Integer(TCHAR *pStr1)
{
EFS_BEGIN

	if (pStr1 == NULL) return 0;
	return _ttoi(pStr1);

EFS_END
return -1;

}

INT CESL_DataMgr::RefreshPositionInfo(VOID)
{
EFS_BEGIN

	if (m_pDataPosition != NULL) {
		FreePositionInfo();
	}
	INT nRows = data.GetCount();
	m_pDataPosition = (POSITION*)malloc(sizeof(POSITION)*nRows);
	if (m_pDataPosition == NULL) return -3;
	POSITION pos = data.GetHeadPosition();
	CObject *pData;
	for (INT i = 0; i < nRows; i++) {
		m_pDataPosition[i] = pos;
		pData = data.GetNext(pos);
	}
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::FreePositionInfo(VOID)
{
EFS_BEGIN

	if (m_pDataPosition) {
		free(m_pDataPosition);
		m_pDataPosition = NULL;
	}
	return 0;

EFS_END
return -1;

}

INT CESL_DataMgr::ExportToExcel(CWnd *pWnd, CString strFileName, CProgressCtrl *pBar/*=NULL*/)
{
EFS_BEGIN

	BOOL	bIsExcel	= FALSE,
			bIsHancell	= FALSE,
			bIsCSV		= FALSE
			;

	if(m_bIsExcel == FALSE && m_bIsHancell == FALSE && m_bIsCSV == FALSE)
	{
		CESLDlg_ExportType TempDlg;
		if(TempDlg.DoModal() == IDOK)
		{
			bIsExcel	= TempDlg.m_bIsExcel;
			bIsHancell	= TempDlg.m_bIsHancell;
			bIsCSV		= TempDlg.m_bIsCSV;
		}
		else
		{
			return -1;
		}
	}
	else
	{
			bIsExcel	= m_bIsExcel;
			bIsHancell	= m_bIsHancell;
			bIsCSV		= m_bIsCSV;
	}

	INT i = 0, j = 0, k = 0;
	INT nRows = GetRowCount();
	if(nRows == 0)
		return -1;

	CString strTotalData, strData, strTmpData;
	CString strTmp2;
	TCHAR szLineData[10000];
	FLOAT	nProgressStep=0;
	FLOAT	nProgress =0;
	CStringArray strTotalExportLine;	
	strTotalExportLine.RemoveAll();

	
	BOOL bRemainder = FALSE;
	if (pBar)
	{
		pBar->SetPos(0);
		pBar->SetRange(0, 100);
	}
	
	DWORD *pRow = NULL;
	
	memset(szLineData, 0, 10000);


	INT nCols = RefList.GetCount();
	POSITION pos = RefList.GetHeadPosition();
	reference * pRef;
	for (j = 0; j < nCols; j++) {
		pRef = (reference *)RefList.GetNext(pos);
		if (pRef) {
			strTmpData = strTmpData + _T("\"") + pRef->FIELD_ALIAS + _T("\"") + _T("\t");			
		}
	}
	
	strTmpData = strTmpData + _T("\n");
	_stprintf(szLineData, _T("%s"), strTmpData );						
	

	INT nTotalCellCount = nRows * nCols;
	INT nCurrentCellCount = 0;

	INT nLen = _tcslen(szLineData);
	for (k=0 ; k < nLen ; k++)
	{
		if ( szLineData[k] == 13 ) 
		{
			szLineData[k] = 32;						
		}
	}							
	strTotalData += szLineData;
	pos = RefList.GetHeadPosition();
	for (i = 0; i < nRows; i++) 
	{
		strData.Empty();	
		strTmpData.Empty();	
		memset(szLineData, 0, 10000);
		
		if (pos == NULL) break;
		pRow  = (DWORD*)this->data.GetNext(pos);
		if (pRow == NULL) continue;

		pos = RefList.GetHeadPosition();
		pRef = NULL;
		for (j = 0; j < nCols; j++) 
		{
			strData = GetCellData(i, j);
			strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");
		}
								
		
		if (pBar) 
		{
			nCurrentCellCount++;
			pBar->SetPos((100 * nCurrentCellCount) / nTotalCellCount);
		}

		strTmpData = strTmpData + _T("\n");
		
		_stprintf(szLineData, _T("%s"), strTmpData );
		
		
		INT nLen = _tcslen(szLineData);
		for (int k=0 ; k < nLen ; k++)
		{
			if ( szLineData[k] == 13 ) 
			{
				szLineData[k] = 32;						
			}
		}	
		
		strTotalData += szLineData;
		if ( i%1000 == 0 ) 
		{
			strTotalExportLine.Add(strTotalData);
			strTotalData = _T("");
			bRemainder = FALSE;
		}
		else if ( i%1000 == 1)
		{
			bRemainder = TRUE;
		}
	}
	
	if ( bRemainder )
	{
		strTotalExportLine.Add(strTotalData);
		strTotalData = _T("");
	}
	
	for ( i=0 ; i<strTotalExportLine.GetSize() ; i++ )
	{
		strTotalData += strTotalExportLine.GetAt(i);
	}
	if(pBar)
	{
		pBar->SetPos(100);
	}
	
	CXLEzAutomation XL;			

	XL.m_bIsExcel = bIsExcel;
	XL.m_bIsHancell = bIsHancell;
	XL.m_bIsCSV = bIsCSV;
	XL.ExportCString(strTotalData, strFileName);

	return 0;

EFS_END
return -1;
}

INT CESL_DataMgr::ExportToExcelWithFileDialog(CWnd *pWnd, CProgressCtrl *pBar/*=NULL*/)
{
EFS_BEGIN
	if (pBar) pBar->SetPos(0);
	TCHAR TmpDir[1025];
	GetCurrentDirectory(1024,TmpDir);

	CESLDlg_ExportType TempDlg;
	if(TempDlg.DoModal() == IDOK)
	{
		m_bIsExcel		= TempDlg.m_bIsExcel;
		m_bIsHancell	= TempDlg.m_bIsHancell;
		m_bIsCSV		= TempDlg.m_bIsCSV;
	}
	else
	{
		return -1;
	}

	CString strFileName, strExecuteType1, strExecuteType2, strExecuteType3;
	if(m_bIsExcel == TRUE)
	{
		strExecuteType1.Format(_T("xls"));
		strExecuteType2.Format(_T("*.xls"));
		strExecuteType3.Format(_T("xls(*.xls)|*.xls|모든파일(*.*)|*.*||"));
	}
	else if(m_bIsHancell == TRUE)
	{
		strExecuteType1.Format(_T("cell"));
		strExecuteType2.Format(_T("*.cell"));
		strExecuteType3.Format(_T("한셀파일(*.cell)|*.cell|모든파일(*.*)|*.*||"));
	}
	else if(m_bIsCSV == TRUE)
	{
		strExecuteType1.Format(_T("csv"));
		strExecuteType2.Format(_T("*.csv"));
		strExecuteType3.Format(_T("CSV파일(탭단위)(*.csv)|*.csv|모든파일(*.*)|*.*||"));
	}

	if(m_bIsCSV == TRUE)
	{	
		CFileDialog dlg(FALSE, strExecuteType1, strExecuteType2, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExecuteType3);
		if ( IDOK != dlg.DoModal() )
		{
			MessageBox(pWnd->GetSafeHwnd(), _T("출력이 취소되었습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
			SetCurrentDirectory(TmpDir);
			return 0;
		}
		if ( dlg.GetFileTitle().IsEmpty() == TRUE ) return 1;
		strFileName = dlg.GetPathName();
	}
	SetCurrentDirectory(TmpDir);

	INT ids = ExportToExcel(pWnd, strFileName, pBar);
	return ids;

EFS_END
return -1;
}

BOOL CESL_DataMgr::ORACLE_GetConnectedDB(VOID)
{
EFS_BEGIN

	return TRUE;
EFS_END
return -1;
}

CString CESL_DataMgr::GetDate(VOID)
{
	CTime t = CTime::GetCurrentTime();
	CString str;
	str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	return str;
}

INT CESL_DataMgr::SORT_SwapRow(INT nRow1, INT nRow2)
{
EFS_BEGIN
	if (nRow1 < 0 || nRow2 < 0) return -1;
	DWORD *pRow1 = (DWORD*)data.GetAt(m_pDataPosition[nRow1]);
	DWORD *pRow2 = (DWORD*)data.GetAt(m_pDataPosition[nRow2]);

	data.SetAt(m_pDataPosition[nRow1], (CObject*)pRow2);
	data.SetAt(m_pDataPosition[nRow2], (CObject*)pRow1);
	return 0;
EFS_END
return -1;
}

INT CESL_DataMgr::SORT_SwapRow(INT nRow1, INT nRow2, INT nColumn, INT nColumnType)
{
EFS_BEGIN
	if (nRow1 < 0 || nRow2 < 0) return -1;
	DWORD *pRow1 = (DWORD*)data.GetAt(m_pDataPosition[nRow1]);
	DWORD *pRow2 = (DWORD*)data.GetAt(m_pDataPosition[nRow2]);

	if (nColumnType == ESL_DATAMGR_TYPE_STR) {
		if (StringCompare((TCHAR*)pRow1[nColumn], (TCHAR*)pRow2[nColumn]) == 0) return 0;
	}
	if (nColumnType == ESL_DATAMGR_TYPE_INT) {
		if (String_to_Integer((TCHAR*)pRow1[nColumn]) == String_to_Integer((TCHAR*)pRow2[nColumn])) return 0; 
	}

	data.SetAt(m_pDataPosition[nRow1], (CObject*)pRow2);
	data.SetAt(m_pDataPosition[nRow2], (CObject*)pRow1);
	return 0;
EFS_END
return -1;
}

INT CESL_DataMgr::SORT(INT nCondColumns, CString strAlias[], INT nCond[], INT nSortDir, INT nStartRowIdx/*=-1*/, INT nFinishRowIdx/*=-1*/)
{
EFS_BEGIN
	if (m_pDataPosition == NULL) return -4990;
	INT nAlias[256], nType[256];
	if (FindColumn(nCondColumns, strAlias, nAlias, nType) < 0) return -4201;

	INT nRows = GetRowCount();
	if (nStartRowIdx < 0) nStartRowIdx = 0;
	if (nFinishRowIdx < 0) nFinishRowIdx = nRows-1;
	if (nStartRowIdx >= nRows) return -4202;
	if (nFinishRowIdx >= nRows) return -4203;

	INT i, j, nIds;
	DWORD *pRowMaster, *pRowSlave;
	for (i = nStartRowIdx; i <= nFinishRowIdx - 1; i++)
	{
		pRowMaster = (DWORD*)data.GetAt(m_pDataPosition[i]);
		if (pRowMaster == NULL) continue;

		for (j = i + 1; j <= nFinishRowIdx; j++)
		{
			pRowSlave = (DWORD*)data.GetAt(m_pDataPosition[j]);
			if (pRowSlave == NULL) continue;			
			nIds = SORT_LineCompare(nCondColumns, nAlias, nType, nCond, pRowMaster, pRowSlave);
			if (nSortDir == ESL_DATAMGR_SORT_ASC && nIds > 0) {
				data.SetAt(m_pDataPosition[i], (CObject*)pRowSlave);
				data.SetAt(m_pDataPosition[j], (CObject*)pRowMaster);
				pRowMaster = pRowSlave;
			}
			if (nSortDir == ESL_DATAMGR_SORT_DESC && nIds < 0) {
				data.SetAt(m_pDataPosition[i], (CObject*)pRowSlave);
				data.SetAt(m_pDataPosition[j], (CObject*)pRowMaster);
				pRowMaster = pRowSlave;
			}
		}
	}
	return 0;
EFS_END
return -1;
}

INT CESL_DataMgr::FindColumn(INT nColumns, CString strAlias[], INT nResult[], INT nType[])
{
EFS_BEGIN
	if (nColumns < 0) return -1;
	INT ids;
	for (INT i = 0; i < nColumns; i++) {
		ids = FindColumn(strAlias[i], nResult[i], nType[i]);
		if (ids < 0) return -1;
	}
	return 0;
EFS_END
return -1;
}

INT CESL_DataMgr::FindColumn(CString strAlias, INT &nResult, INT &nType)
{
EFS_BEGIN
	INT nData = RefList.GetCount();
	if (nData == 0) return -1;
	reference * pRef;
	POSITION pos;
	pos = RefList.GetHeadPosition();
	for(INT i = 0; i < nData; i++) {
		pRef = NULL;
		pRef = (reference*)RefList.GetNext(pos);
		if (pRef == NULL) continue;

		if (pRef->FIELD_ALIAS == strAlias) {
			nResult = i;
			if (pRef->FIELD_TYPE == _T("int") || pRef->FIELD_TYPE == _T("INT")) nType = ESL_DATAMGR_TYPE_INT;
			if (pRef->FIELD_TYPE == _T("str") || pRef->FIELD_TYPE == _T("STR")) nType = ESL_DATAMGR_TYPE_STR;
			return 0;
		}
	}
	return(-1);	
EFS_END
return -1;
}

INT CESL_DataMgr::SORT_LineCompare(INT nCondColumns, INT nAlias[], INT nType[], INT nCond[], DWORD *pRowMaster, DWORD *pRowSlave)
{
EFS_BEGIN
	INT nIds;
	for (INT i = 0; i < nCondColumns; i++) {
		nIds = 0;
		if (nType[i] == ESL_DATAMGR_TYPE_INT) {
			nIds = 0;
			if (String_to_Integer((TCHAR*)pRowMaster[nAlias[i]]) > String_to_Integer((TCHAR*)pRowSlave[nAlias[i]])) {
				nIds = 1;
			} else {
				nIds = -1;
			}
			if (String_to_Integer((TCHAR*)pRowMaster[nAlias[i]]) == String_to_Integer((TCHAR*)pRowSlave[nAlias[i]])) {
				nIds = 0;
			}
		}
		if (nType[i] == ESL_DATAMGR_TYPE_STR) {
			nIds = StringCompare((TCHAR*)pRowMaster[nAlias[i]] , (TCHAR*)pRowSlave[nAlias[i]]);
		}
		if (nIds == 0) continue;

		if (nIds > 0) {
			if (nCond[i] == ESL_DATAMGR_SORT_ASC) return 1; 
			if (nCond[i] == ESL_DATAMGR_SORT_DESC) return -1; 
		} 
		if (nIds < 0) {
			if (nCond[i] == ESL_DATAMGR_SORT_ASC) return -1; 
			if (nCond[i] == ESL_DATAMGR_SORT_DESC) return 1; 
		} 
	}
	return 0;
EFS_END
return -1;
}

INT CESL_DataMgr::SORT_QUICKSORT(INT nColumnIdx, INT nColumnType, INT nStartRowIdx, INT nFinishRowIdx, BOOL IsDesc)
{
EFS_BEGIN
	INT i, j;		   
	INT nPivot;
	TCHAR *pszPivot;
	DWORD *pRowTmp1;

	if(nStartRowIdx < nFinishRowIdx)	
	{
		i = nStartRowIdx;
		j = nFinishRowIdx + 1;
		pRowTmp1 = (DWORD*)data.GetAt(m_pDataPosition[nStartRowIdx]);
		if (nColumnType == ESL_DATAMGR_TYPE_INT) nPivot = String_to_Integer((TCHAR*)pRowTmp1[nColumnIdx]);
		if (nColumnType == ESL_DATAMGR_TYPE_STR) pszPivot = (TCHAR*)pRowTmp1[nColumnIdx];

		do
		{
			while(TRUE) 
			{
				i++;
				if (i >= nFinishRowIdx) {
					break;
				}
				pRowTmp1 = (DWORD*)data.GetAt(m_pDataPosition[i]);
				if (nColumnType == ESL_DATAMGR_TYPE_INT) {
					if (IsDesc == TRUE) if (String_to_Integer((TCHAR*)pRowTmp1[nColumnIdx]) <= nPivot) break;
					if (IsDesc == FALSE) if (String_to_Integer((TCHAR*)pRowTmp1[nColumnIdx]) >= nPivot) break;
				}
				if (nColumnType == ESL_DATAMGR_TYPE_STR) {
					if (IsDesc == TRUE) if (StringCompare((TCHAR*)pRowTmp1[nColumnIdx], pszPivot) <= 0) break;
					if (IsDesc == FALSE) if (StringCompare((TCHAR*)pRowTmp1[nColumnIdx], pszPivot) >= 0) break;
				}
			}
			while(TRUE)
			{
				j--;
				if (j < nStartRowIdx) {
					break;
				}
				pRowTmp1 = (DWORD*)data.GetAt(m_pDataPosition[j]);
				if (nColumnType == ESL_DATAMGR_TYPE_INT) {
					if (IsDesc == TRUE) if (String_to_Integer((TCHAR*)pRowTmp1[nColumnIdx]) >= nPivot) break;
					if (IsDesc == FALSE) if (String_to_Integer((TCHAR*)pRowTmp1[nColumnIdx]) <= nPivot) break;
				}
				if (nColumnType == ESL_DATAMGR_TYPE_STR) {
					if (IsDesc == TRUE) if (StringCompare((TCHAR*)pRowTmp1[nColumnIdx], pszPivot) >= 0) break;
					if (IsDesc == FALSE) if (StringCompare((TCHAR*)pRowTmp1[nColumnIdx], pszPivot) <= 0) break;
				}
			}
			if(i < j) {
				SORT_SwapRow( i, j, nColumnIdx, nColumnType);
			}
		}while(i < j);

		SORT_SwapRow(nStartRowIdx, j, nColumnIdx, nColumnType);
		
		SORT_QUICKSORT(nColumnIdx, nColumnType, nStartRowIdx, j-1, IsDesc);
		SORT_QUICKSORT(nColumnIdx, nColumnType, j+1, nFinishRowIdx, IsDesc);
	}
	return 0;
EFS_END
return -1;
}
CString CESL_DataMgr::GetECOSEARCH(CString strOption)
{
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_DOPENBASE")) )
	{
		return strOption;
	}

	INT ids;
	while(TRUE) {
		ids = GetECOSEARCHReplace(strOption);
		if (ids < 0) break;
	}
	return strOption;
}

INT CESL_DataMgr::GetECOSEARCH(CString strOption, CStringArray &strarrayCondion, CString strTables, CStringArray &strarrayTables)
{
	CString strSearchCondition;
	INT nTotal, nRemain;
	CECO_SearchMgrApi ESM(this);

	BOOL bSucess;
	bSucess = ESM.ECO_GetSearchKey(strOption, strarrayCondion, strTables, strarrayTables, nTotal, nRemain);
	if ( bSucess == FALSE )  return -1;

	return nTotal;
}

INT CESL_DataMgr::GetECOSEARCHReplace(CString &strOption)
{
	if (strOption.GetLength() == 0) return -5;

	INT nPos = -1;
	INT nPos1, nPos2, nPos3;

	CString strArg1, strArg2, strArg3;

	nPos = strOption.Find(_T("&ECOSEARCH("), 0);
	if (nPos == -1) return -1;

	nPos1 = strOption.Find(',', nPos+11);
	if (nPos1 == -1) return -2;

	nPos2 = strOption.Find(',', nPos1+1);
	if (nPos2 == -1) return -3;

	nPos3 = strOption.Find(')', nPos2+1);
	if (nPos3 == -1) return -4;

	strArg1 = strOption.Mid(nPos+11, nPos1-nPos-11);
	strArg1.TrimLeft();			strArg1.TrimRight();
	strArg2 = strOption.Mid(nPos1+1, nPos2-nPos1-1);
	strArg2.TrimLeft();			strArg2.TrimRight();
	strArg3 = strOption.Mid(nPos2+1, nPos3-nPos2-1);
	strArg3.TrimLeft();			strArg3.TrimRight();
	strArg3 = strArg3.Mid(1, strArg3.GetLength() - 2);
	strArg3.TrimLeft();			strArg3.TrimLeft();

	CString strBefore, strSearch, strAfter;
	strBefore = strOption.Mid(0, nPos);
	strSearch = GetECOSEARCHReplaceString(strArg1, strArg2, strArg3);
	strAfter = strOption.Mid(nPos3+1);
	
	strOption.Format(_T("%s %s %s"), strBefore, strSearch, strAfter);

	return 0;
}

CString CESL_DataMgr::GetECOSEARCHReplaceString(CString strField, CString strFieldTable, CString str)
{

	if(  CESL_SearchEngine::GetUsed() )
	{
		return GetECOSEARCHReplaceString_WebSearch( strField, strFieldTable, str );
	}

	strFieldTable.MakeUpper();

	CString strIdxTblName = GetECOSEARCHIndexTableName(strFieldTable);
	CString strToken, strTmp, strTmpToken;
	CString strWhere = _T("");
	
	BOOL IsEqualSearch = FALSE, IsTmpEqualSearch;

	CString strResult;
	INT nPosPrev=-1, nPosNext=-1;
	while(true) {
		INT nTemp[2] , nFind;
		str.Replace(_T(" | "), _T("|"));
		nTemp[0] = str.Find('|', nPosPrev+1);
		nTemp[1] = str.Find(' ', nPosPrev+1);
		if(-1==nTemp[0] && -1==nTemp[1])
		{
			nFind = 0;
			strTmpToken = str.Mid(nPosPrev+1);

			nPosNext = -1;		
		}
		else if(-1!=nTemp[0] && -1!=nTemp[1])
		{
			if(nTemp[0]<nTemp[1])
			{
				nFind = 1;
				nPosNext = nTemp[0];
			}
			else
			{
				nFind = 2;
				nPosNext = nTemp[1];
			}
			strTmpToken = str.Mid(nPosPrev+1, nPosNext-nPosPrev-1);
		}
		else if(-1!=nTemp[0])
		{
			nFind = 1;
			nPosNext = nTemp[0];
			strTmpToken = str.Mid(nPosPrev+1, nPosNext-nPosPrev-1);
		}
		else if(-1!=nTemp[1])
		{
			nFind = 2;
			nPosNext = nTemp[1];
			strTmpToken = str.Mid(nPosPrev+1, nPosNext-nPosPrev-1);
		}

		IsTmpEqualSearch = FALSE;
		strToken = GetECOSEARCHTokenString(strTmpToken, IsTmpEqualSearch);
		if (IsTmpEqualSearch == TRUE) IsEqualSearch = TRUE;
		if (strWhere.GetLength() == 0) 
		{
			if(0==nFind) strWhere = strToken; 
			else if(1==nFind) strWhere.Format(_T("%s OR"), strToken); 
			else if(2==nFind) strWhere.Format(_T("%s AND"), strToken); 
		} 
		else 
		{
			strTmpToken = strWhere;
			if(0==nFind) strWhere.Format(_T("%s %s"), strTmpToken, strToken); 
			else if(1==nFind) strWhere.Format(_T("%s %s OR"), strTmpToken, strToken); 
			else if(2==nFind) strWhere.Format(_T("%s %s AND"), strTmpToken, strToken); 
		}

		if (nPosNext == -1) break;
		nPosPrev = nPosNext;
	}

	if (IsEqualSearch == TRUE) 
	{
		strTmpToken = strWhere;
		strWhere.Format(_T("(%s) AND TOKEN_TYPE='F'"), strTmpToken);
	}

	strField.TrimLeft();		strField.TrimRight();
	if (strField.GetLength() == 0) {
	    if(m_dbkind == 1 || m_dbkind == 6)
		{
			strResult.Format(_T("((SELECT REC_KEY FROM IDX_BO_TBL WHERE REC_KEY=-1)")
							  _T("UNION")
							  _T("(SELECT DISTINCT ECO_KEY FROM %s WHERE %s))"), strIdxTblName, strWhere);
		}
		else if(m_dbkind == 2)
		{
			strResult.Format(_T("  SELECT REC_KEY FROM IDX_BO_TBL WHERE REC_KEY=-1 ")
							  _T(" UNION ALL ")
							  _T(" SELECT DISTINCT ECO_KEY FROM %s WHERE %s "), strIdxTblName, strWhere);
		}
	} else {
		strResult.Format(_T("%s IN (SELECT DISTINCT ECO_KEY FROM %s WHERE %s)"), strField, strIdxTblName, strWhere);
	}

	return strResult;
}

CString CESL_DataMgr::GetECOSEARCHIndexTableName(CString strFieldTable)
{
	FILE *fd;
	fd = _tfopen(_T("..\\CFG\\IDX_TABLE.CFG"), _T("rb"));
	if (fd == NULL) {
		AfxMessageBox(_T("..\\CFG\\IDX_TABLE.CFG를 열 수 없습니다."));
		return strFieldTable;
	}

	FILE* fpUni = fd;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}

	TCHAR szBuf[128];
	CString strLine, strAlias, strTable;
	INT nPos;
	while (true) {
		if (_fgetts(szBuf, 127, fd) == NULL) break;
		szBuf[_tcsclen(szBuf)-1] = '\0';
		strLine.Format(_T("%s"), szBuf);
		nPos = strLine.Find('=', 0);
		if (nPos == -1) continue;

		strAlias = strLine.Mid(0, nPos);
		strAlias.TrimLeft();			strAlias.TrimRight();
		strTable = strLine.Mid(nPos+1);
		strTable.TrimLeft();			strTable.TrimRight();

		if (strFieldTable == strAlias) {
			fclose(fd);
			return strTable;
		}
	}
	fclose(fd);
	return strFieldTable;
}

CString CESL_DataMgr::GetECOSEARCHTokenString(CString str, BOOL &IsEqualSearch)
{
	IsEqualSearch = FALSE;

	INT nPos = str.Find('*', 0);
	if (nPos == -1) {
		IsEqualSearch = TRUE;
	} else {
		IsEqualSearch = FALSE;
		str = str.Mid(0, nPos);
	}
	INT nLen = GetECOSEARCHTokenStringLength(str);

	CString strResult = _T("");

	if (IsEqualSearch == FALSE) 
	{	
		if (nLen > 20) 
		{
			wchar_t  wszBuffer[257];
			INT nCount = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)str.GetBuffer(0), str.GetLength(), wszBuffer, 255);
			wszBuffer[20] = 0; 

			TCHAR szBuffer[257];
			wmemset(szBuffer,0x00,257);
			INT nMBCSCount = WideCharToMultiByte(CP_ACP,0,wszBuffer,-1,(char*)szBuffer,wcslen(wszBuffer)*2,NULL,NULL);
			str.Format(_T("%s"), szBuffer);
		}
		strResult.Format(_T("TOKEN='%s'"), str);
	} 
	else 
	{						
		strResult.Format(_T("TOKEN='%s'"), str);
	}

	return strResult;
}

INT CESL_DataMgr::GetECOSEARCHTokenStringLength(CString str)
{
    wchar_t  wszBuffer[257];

    INT nCount = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)str.GetBuffer(0), str.GetLength(), wszBuffer, 255);

	wszBuffer[nCount] = 0; 

	INT nLen = wcslen(wszBuffer);
	return nLen;
}

INT CESL_DataMgr::ORACLE_GetRequestID(VOID)
{
	return (INT)GetSessionRequestID();
}

INT CESL_DataMgr::GetLastMsgID(VOID)
{
	return m_nLastMsgID;
}


INT CESL_DataMgr::CheckManageCode()
{
	if ( MANAGE_CODE.IsEmpty() )
	{
		DWORD dwPID = ::GetCurrentProcessId();
		CString strCHMfileName;
  		strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);
		
		CStdioFile fd;
		if (fd.Open(strCHMfileName, CFile::modeRead|CFile::typeBinary) == FALSE) 
		{
			strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info.log") );
			if (fd.Open(strCHMfileName, CFile::modeRead|CFile::typeBinary) == FALSE) 
			{
 				AfxMessageBox(_T("로그인정보를 찾을 수 없습니다."));
				return 0;
			}	
		} else {
			TCHAR cUni;
			fd.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fd.SeekToBegin();
			}
			CString strLine;
			fd.ReadString(strLine);
			strLine.TrimLeft();			strLine.TrimRight();
			if (strLine.GetLength() > 0) strCHMfileName = strLine;
			fd.Close();
			MANAGE_CODE=strLine;
		}		

	}
	return 0;
}

INT CESL_DataMgr::SetManageCode( CString strManageCode )
{ 
	MANAGE_CODE = strManageCode;
	return 0;
}
