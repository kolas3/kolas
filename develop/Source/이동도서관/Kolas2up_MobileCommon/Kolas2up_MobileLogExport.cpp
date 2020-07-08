// Kolas2up_MobileLogExport.cpp: implementation of the CKolas2up_MobileLogExport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_MobileLogExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "Kolas2up_TableInfo.h"
#include "QueryEditor_DLG.h"
#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_MobileLogExport::CKolas2up_MobileLogExport()
{
	m_pMobileDM = NULL;
	m_pMobileDM = new CESL_DataMgr;

	m_pMainDM = NULL;
	m_pMainDM = new CESL_DataMgr;
}

CKolas2up_MobileLogExport::~CKolas2up_MobileLogExport()
{
	FreeQueryList();
	if (m_pMobileDM) {
		delete m_pMobileDM;
		m_pMobileDM = NULL;
	}	
	if (m_pMainDM) {
		delete m_pMainDM;
		m_pMainDM = NULL;
	}
}

INT CKolas2up_MobileLogExport::SetMobileDBInfo(CString strCONNECTION_INFO, CString strDBKIND)
{
EFS_BEGIN

	if (m_pMobileDM == NULL) return -1;
	m_pMobileDM->SetCONNECTION_INFO(strCONNECTION_INFO);
	m_pMobileDM->SetDB(strDBKIND);

	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::SetMainDBInfo(CString strCONNECTION_INFO, CString strDBKIND)
{
EFS_BEGIN

	if (m_pMainDM == NULL) return -1;
	m_pMainDM->SetCONNECTION_INFO(strCONNECTION_INFO);
	m_pMainDM->SetDB(strDBKIND);

	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::ReadLogFromMobileDB(CString strOption/*=_T("")*/)
{
EFS_BEGIN

	if (m_pMobileDM == NULL) return -1;
	
	INT ids;
	CString sQuery;
	if (strOption.GetLength() == 0) 
	{
		//                      0          1            2          3           4
		sQuery.Format(_T("SELECT REC_KEY, QUERY_TYPE, TABLE_NAME, DATA_REC_KEY, QUERY ")
						_T("FROM QUERY_LOG_TBL ")
					   _T("ORDER BY LOGDATE, REC_KEY"));
	} 
	else 
	{
		//                      0          1            2          3           4
		sQuery.Format(_T("SELECT REC_KEY, QUERY_TYPE, TABLE_NAME, DATA_REC_KEY, QUERY ")
			            _T("FROM QUERY_LOG_TBL ")
					   _T("WHERE %s ")
					   _T("ORDER BY LOGDATE, REC_KEY"), strOption);
	}
	ids = m_pMobileDM->RestructDataManager(sQuery);
	if (ids < 0) return -3;

	FreeQueryList();
	m_NewRecKeyList.FreeNewRecKey();

	CString strTableName;
	INT nRows = m_pMobileDM->GetRowCount();
	CString strQueryType, strQuery;
	CKolas2up_MobileLogQuery * pQuery;

	CESL_DataMgr RestructMobileDM;
	RestructMobileDM.SetCONNECTION_INFO(m_pMobileDM->CONNECTION_INFO);

	for (INT i = 0 ; i < nRows ; i++) 
	{
		strQueryType = m_pMobileDM->GetCellData(i, 1);
		strQueryType.TrimLeft();	
		strQueryType.TrimRight();	
		strQueryType.MakeUpper();
		strQuery = _T("");

		strTableName = m_pMobileDM->GetCellData(i, 2);
		strTableName.TrimLeft();			strTableName.TrimRight();
		strTableName.MakeUpper();

		if(
			(strTableName != _T("BO_BOOK_TBL") && strTableName != _T("CO_LOAN_FAMILY_ID_TBL") && 
			strTableName != _T("CO_LOAN_TBL") && strTableName != _T("CO_LOAN_USER_TBL") && 
			strTableName != _T("CO_NON_DB_BOOK_TBL") && strTableName != _T("LS_WORK_T01") &&
			strTableName != _T("SE_BOOK_TBL")) && strTableName != _T("KL_LOCAL_TEMP_INFO_TBL")
			&& strQueryType != _T("Q")
			)
		{
			CString sQuery;
			sQuery.Format(_T("DELETE QUERY_LOG_TBL WHERE TABLE_NAME='%s';"),strTableName);
			RestructMobileDM.ExecuteQuery(sQuery,1);
			continue;
		}
		
		//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		ids = 0;
		if      (strQueryType == _T("I")) ids = LOG_MakeInsertFrame(m_pMobileDM, &RestructMobileDM , i, strQuery);
		else if (strQueryType == _T("U")) ids = LOG_MakeUpdateFrame(m_pMobileDM, &RestructMobileDM , i, strQuery);
		else if (strQueryType == _T("D")) ids = LOG_MakeDeleteFrame(m_pMobileDM, i, strQuery);
		else if (strQueryType == _T("Q")) ids = LOG_MakeQueryFrame (m_pMobileDM, i, strQuery);
		else 
			return -8;

		if (0>ids) {
			strQuery = _T("");
			return ids;
		}
		//박길주 2003-1104-------------------------------------------------------------
		
		pQuery = new CKolas2up_MobileLogQuery;
		if (NULL==pQuery) return -9;
		if (pQuery) 
		{
			QUERYLIST.AddTail((CObject*)pQuery);
			pQuery->strRecKey.Format(_T("%s"), m_pMobileDM->GetCellData(i, 0));
			pQuery->strQuery.Format(_T("%s"), strQuery);
		}
		if (strTableName == _T("CO_LOAN_USER_TBL")) {
			Make_CO_TL_LOAN_USER_TBL_Query(m_pMobileDM , i, strQueryType, pQuery);
		}
		if(strTableName == _T("LS_WORK_T01"))
		{
			/// 1. 우선 대출 공유를 하는 도서관이라면 Temp Table에 공유 정보를 입력한다.
			if( m_sIsCenterUpLoan == _T("1") || (m_sIsCenterUpLoan == _T("2")) )
			{
				CString sCTUpdateQuery;
				CString sType;
				if( strQueryType == _T("I") || strQueryType == _T("U") ) sType = _T("I");
				else sType = _T("D");

				CString sLibCode;
				CString sTempQuery;

				sTempQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL"),m_strUserManageCode);
				m_pMobileDM->GetOneValueQuery(sTempQuery,sLibCode);
				
				sCTUpdateQuery.Format(_T("INSERT INTO CT_UPLOAD_TEMP_LOAN_INFO_TBL(REC_KEY,TYPE,COMMAND,MASTER_USER_INFO_CHANGE_YN,UPDATED_TABLE_NAME,UPDATED_TABLE_REC_KEY,RECORD_DATE,UPLOAD_TRY_CNT,UPLOAD_FAIL_REASON_CODE,FIRST_WORK,LIB_CODE) ")
				_T("VALUES(ESL_SEQ.NextVal,'%s','51','Y','LH_STORAGE_V01',%s,SYSDATE,0,'444','이동UP','%s');"),sType,m_pMobileDM->GetCellData(i, 3),sLibCode);

				pQuery->m_arrAddonQuery.Add(sCTUpdateQuery);
			}
		}
		if(_T("KL_LOCAL_TEMP_INFO_TBL") == strTableName)		
		{
//			pQuery->m_arrAddonQuery.Add(strQuery);			
		}
	}
	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::ExportToFile(CString strFileName, CProgressCtrl *pBar/*=NULL*/)
{
EFS_BEGIN

	FILE *fd = _tfopen(strFileName, _T("wb"));
	if (fd == NULL) return -1;

		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);
		
	INT i;
	INT nData = QUERYLIST.GetCount();
	POSITION pos = QUERYLIST.GetHeadPosition();
	CKolas2up_MobileLogQuery * pQuery;
	for (i = 0; i < nData; i++) {
		pQuery = NULL;
		pQuery = (CKolas2up_MobileLogQuery *)QUERYLIST.GetNext(pos);
		if (pQuery == NULL) continue;

		_ftprintf(fd, _T("%s\r\n"), pQuery->strQuery);
		if (pBar) pBar->SetPos((INT)(((i+1)*100) / nData));
	}
	fclose(fd);
	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::ExportToMainDB(CProgressCtrl *pBar/*=NULL*/, CWnd *pMsg/*=NULL*/)
{
EFS_BEGIN

	if (m_pMobileDM == NULL) return -1;
	if (m_pMainDM == NULL)   return -2;

	INT ids, ids1, ids2, nSuccess, nError;
	INT nData	 = QUERYLIST.GetCount();
	POSITION pos = QUERYLIST.GetHeadPosition();

	CKolas2up_MobileLogQuery * pQuery;
	CString strExecuteLogQuery;
	CString strMsg;
	nSuccess = nError = 0;

	CTime t_cur = CTime::GetCurrentTime();
	CString strLogFileName;
	strLogFileName.Format(_T("..\\tunning\\TL_UPLOAD_%04d%02d%02d.log"), t_cur.GetYear(), t_cur.GetMonth(), t_cur.GetDay());

	//박길주 2003-1110=============================================================
	//FILE* fp = _tfopen (strLogFileName, _T("a+"));
	//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	FILE* fp = NULL;
	fp = _tfopen (strLogFileName, _T("a+b"));
	if (NULL==fp) return -100;
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	// KOLAS 2 UP UNICODE 수정작업
	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	// ------------------------------------------------------------------------------
		
	//박길주 2003-1104-------------------------------------------------------------

	strMsg.Format(_T("#[START TL UPLOAD LOG][%04d-%02d-%02d.%02d:%02d:%02d]---------------------------------------------------------"), t_cur.GetYear(), t_cur.GetMonth(), t_cur.GetDay(), t_cur.GetHour(), t_cur.GetMinute(), t_cur.GetSecond());
	if (fp) _ftprintf(fp, _T("%s\r\n"), strMsg);

	INT j;
	for (INT i = 0; i < nData; i++) 
	{
//		if (i % 50 == 0) Sleep(2000);
		t_cur = CTime::GetCurrentTime();
		ids = ids1 = ids2 = 0;
		pQuery = NULL;
		pQuery = (CKolas2up_MobileLogQuery *)QUERYLIST.GetNext(pos);
		
		//박길주 2003-1110=============================================================
		//if (pQuery == NULL) continue;
		//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pQuery == NULL) return -3;
		//박길주 2003-1104-------------------------------------------------------------

		//박길주 2003-1104=============================================================
		//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		ids1 = -1;
		//박길주 2003-1104-------------------------------------------------------------
		if (pQuery->strQuery.GetLength() > 0) 
		{
			m_pMainDM->StartFrame();
			m_pMainDM->AddFrame(pQuery->strQuery);
			if (pQuery->m_arrAddonQuery.GetSize() > 0) {
				for (j = 0; j < pQuery->m_arrAddonQuery.GetSize(); j++) {
					m_pMainDM->AddFrame(pQuery->m_arrAddonQuery.GetAt(j));
				}
			}
			ids1 = m_pMainDM->SendFrame();

			//쿼리수정여부에따라 오류쿼리 수정창 동작!
			if ( ids1 < 0 ) 
			{
				CQueryEditor_DLG dlg;
				dlg.SetErrorQuery ( pQuery->strQuery );
				dlg.DoModal();
				if ( dlg.m_nStatus == 1 ) 
				{
					//수정후 쿼리수행
					m_pMainDM->EndFrame();
					m_pMainDM->StartFrame();
					m_pMainDM->AddFrame( dlg.m_strErrQuery );
					ids1 = 0;
				}
				else if ( dlg.m_nStatus == 2 )
				{
					ids1 = 0;
				}
				else
				{
					AfxMessageBox( _T("종료되었습니다.") );
					m_pMainDM->EndFrame();
					return 0;
				}
			}

			m_pMainDM->EndFrame();

		} else {
			ids1 = 0;
		}
		//박길주 2003-1104=============================================================
		//else 
		//{
		//	ids1 = 0;
		//}
		//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//박길주 2003-1104-------------------------------------------------------------

		strMsg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][MAINDB][%05d]%s"), t_cur.GetYear(), t_cur.GetMonth(), t_cur.GetDay(), t_cur.GetHour(), t_cur.GetMinute(), t_cur.GetSecond(), ids1, pQuery->strQuery);
		if (fp) _ftprintf (fp, _T("%s\r\n"), strMsg);
		

		//박길주 2003-1110=============================================================
		//박길주 2003-1110 if (0 > ids1) ++++++++++++++++++++++++++++++++++++++++++++++
		if (0 > ids1) 
		{
			if (fp) fclose(fp);
			return -4;
		}
		else	
		{
		//박길주 2003-1110 if (0 > ids1) ----------------------------------------------

			
			//박길주 2003-1104=============================================================
			//if (ids1 == 0) 
			//{
			//	pQuery->FLAG = 'S';
			//} 
			//else 
			//{
			//	pQuery->FLAG = 'E';
			//	pQuery->strMsg.Format(_T("%d"), ids);
			//}
			//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			pQuery->FLAG = 'S';
			//박길주 2003-1104-------------------------------------------------------------

			
			//박길주 2003-1104=============================================================
			//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			ids2 = -1;
			//박길주 2003-1104-------------------------------------------------------------

			
			//박길주 2003-1104=============================================================
			//ids = pQuery->MakeQueryExecuteLog(strExecuteLogQuery);
			//if (ids == 0) 
			//{
			//	m_pMobileDM->StartFrame();
			//	m_pMobileDM->AddFrame(strExecuteLogQuery);
			//	ids2 = m_pMobileDM->SendFrame();
			//	m_pMobileDM->EndFrame();
			//}
			//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if ( 0 > pQuery->MakeQueryExecuteLog(strExecuteLogQuery) ) return -4;
			
			m_pMobileDM->StartFrame();
			m_pMobileDM->AddFrame(strExecuteLogQuery);
			ids2 = m_pMobileDM->SendFrame();
			m_pMobileDM->EndFrame();
			//박길주 2003-1104-------------------------------------------------------------
			
			strMsg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][MOBILEDB][%05d][%c]%s"), 
						  t_cur.GetYear(), t_cur.GetMonth(), t_cur.GetDay(), t_cur.GetHour(), t_cur.GetMinute(), t_cur.GetSecond(), 
						  //박길주 2003-1104=============================================================
						  //ids, 
						  //박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						  ids2,
						  //박길주 2003-1104-------------------------------------------------------------
						  pQuery->FLAG, strExecuteLogQuery);
			if (fp) _ftprintf (fp, _T("%s\r\n"), strMsg);


			//박길주 2003-1104=============================================================
			//박길주 2003-1104+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (0>ids2) 
			{
				if (fp) fclose(fp);
				return -5;
			}
			//박길주 2003-1104-------------------------------------------------------------

			
			if (ids1 >= 0 && ids2 >= 0) nSuccess++;
			else						nError++  ;

			if (pBar) pBar->SetPos((INT)(((i+1)*100) / nData));
			if (pMsg) 
			{
				strMsg.Format(_T("%d / %d : [Success : %d, Error : %d]"), i+1, nData, nSuccess, nError);
				pMsg->SetWindowText(strMsg);
			}
			
		//박길주 2003-1110=============================================================
		//박길주 2003-1110 if (0 > ids1) ++++++++++++++++++++++++++++++++++++++++++++++
		}
		//박길주 2003-1110 if (0 > ids1) ----------------------------------------------

	}
	
	strMsg.Format(_T("#[FINISH TL UPLOAD LOG][%04d-%02d-%02d.%02d:%02d:%02d]---------------------------------------------------------"), t_cur.GetYear(), t_cur.GetMonth(), t_cur.GetDay(), t_cur.GetHour(), t_cur.GetMinute(), t_cur.GetSecond());
	if (fp) _ftprintf (fp, _T("%s\r\n"), strMsg);
	if (fp) fclose(fp);

	return 0;	

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::FreeQueryList(VOID)
{
EFS_BEGIN

	INT nData = QUERYLIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_MobileLogQuery * pData;
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_MobileLogQuery*)QUERYLIST.RemoveHead();
		if (pData) delete pData;
	}
	QUERYLIST.RemoveAll();
	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::LOG_MakeInsertFrame(CESL_DataMgr *pDM, CESL_DataMgr *pDM2 ,INT nRowIdx, CString &strResult)
{
EFS_BEGIN

	strResult = _T("");
	if (pDM == NULL) return -1;
	if (m_pMainDM == NULL) return -1;
	if (nRowIdx < 0 || nRowIdx >= pDM->GetRowCount()) return -1;

	INT ids;
	
	CString strFileName;

	if(pDM->GetCellData(nRowIdx,2) == _T("CO_LAST_NUMBER_TBL"))
	{
		return 0;
	}
	if(pDM->GetCellData(nRowIdx,2) == _T("KL_LOCAL_TEMP_INFO_TBL"))
	{
#ifdef _UNICODE
		CString sSourcePath = __WFILE__;
#else
		CString sSourcePath = __FILE__;
#endif
		INT nBackSlashPos = sSourcePath.ReverseFind(_T('\\'));
		
		CString sSourceFile;
		sSourceFile = sSourcePath.Mid(nBackSlashPos+1);
		if (22 < sSourceFile.GetLength()) sSourceFile = sSourceFile.Left(22);
		
		CTime t = CTime::GetCurrentTime();

		CString strWorkLog;
		strWorkLog.Format(_T("%04d%02d%02d %02d%02d 이동UP>%s#%d"), 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), 
			sSourceFile, __LINE__);


		CString sData = m_pMobileDM->GetCellData(nRowIdx, 4);

		strResult.Format(
			_T("INSERT INTO KL_LOCAL_TEMP_INFO_TBL(REC_KEY,REQUEST_FRAME,RECORD_DATE,UPLOAD_TRY_CNT,FIRST_WORK,STATUS) ")
			_T("VALUES(ESL_SEQ.NEXTVAL, '%s', SYSDATE, '0', '%s', '0');"), sData, strWorkLog);

		return 0;
	}
	if(pDM->GetCellData(nRowIdx,2) == _T("LS_WORK_T01"))
	{
		strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\LS_WORK_T01_2.CFG"));
		CKolas2up_TableInfo TableInfo;
		ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
		if (ids < 0) return -4;


		//	CESL_DataMgr dm;
		//	dm.SetCONNECTION_INFO(m_pMobileDM->CONNECTION_INFO);
		TableInfo.CONDITION.Format(_T("REC_KEY=%s"), pDM->GetCellData(nRowIdx, 3));

		//Queue Log 정보를 이용하여 DB의 해당자료테이블의 해당레코드를 가져온다.
		CString strSelectQuery;
		TableInfo.MakeSelectQuery(strSelectQuery);
		ids = pDM2->RestructDataManager(strSelectQuery);
	}

	strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\%s.CFG"), pDM->GetCellData(nRowIdx, 2));


	CKolas2up_TableInfo TableInfo;
	ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
	if (ids < 0) return -4;

//	CESL_DataMgr dm;
//	dm.SetCONNECTION_INFO(m_pMobileDM->CONNECTION_INFO);
	TableInfo.CONDITION.Format(_T("REC_KEY=%s"), pDM->GetCellData(nRowIdx, 3));

	//Queue Log 정보를 이용하여 DB의 해당자료테이블의 해당레코드를 가져온다.
	CString strSelectQuery;
	TableInfo.MakeSelectQuery(strSelectQuery);
	if(pDM->GetCellData(nRowIdx,2) != _T("LS_WORK_T01"))
	{
		ids = pDM2->RestructDataManager(strSelectQuery);
	}

	if (ids < 0) return -3;

	//2003-1110 박길주=======================================================
	//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (1>pDM2->GetRowCount()) return 0;
	//2003-1110 박길주-------------------------------------------------------

	CString strQuery = _T("");
	//2003-1110 박길주=======================================================
	//if (pDM == NULL) return -1;	<- 불필요한 코드, 이게 왜 여기에 있는지?
	//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003-1110 박길주-------------------------------------------------------

	CString strFIELDS, strVALUES, strTmp1, strTmp2, strData;
	strQuery = _T("");
	strFIELDS = _T("");
	strVALUES = _T("");
	if (TableInfo.nField == 0) return -6;

	INT nMobileDataRowIdx = 0;
	for (INT i = 0; i < TableInfo.nField; i++) 
	{
		strTmp1 = _T("");
		strTmp2 = _T("");
		strData = pDM2->GetCellData(nMobileDataRowIdx, i);
		strData.TrimLeft();	
		strData.TrimRight();
		strData.Replace(_T("'"), _T("''"));
		TableInfo.FIELDNAME[i].MakeUpper();
		if (i == 0) 
		{
			strTmp1.Format(_T("%s"), TableInfo.FIELDNAME[i]);
			if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_INT) 
			{
				if (strData.GetLength() == 0)	strTmp2.Format(_T("NULL"));
				else							strTmp2.Format(_T("%s"), strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_STR) 
			{
				//if (strData.GetLength() == 0)	
				//	strTmp2.Format(_T("NULL"));
				//else
					strTmp2.Format(_T("'%s'"), strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_DATE) 
			{
				if (strData.GetLength() == 0)	strTmp2.Format(_T("NULL"), strData);
				else							strTmp2.Format(_T("to_char('%s', 'YYYY/MM/DD HH24:MI:SS')"), strData);
			}
			//2003-1110 박길주=======================================================
			//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
			else
				return -7;
			//2003-1110 박길주-------------------------------------------------------
		} 
		else 
		{
			strTmp1.Format(_T(", %s"), TableInfo.FIELDNAME[i]);
			if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_INT) 
			{
				if (strData.GetLength() == 0)	strTmp2.Format(_T(", NULL"));
				else							strTmp2.Format(_T(", %s"), strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_STR) 
			{
				//if (strData.GetLength() == 0)	
				//	strTmp2.Format(_T(", NULL"));
				//else
					strTmp2.Format(_T(", '%s'"), strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_DATE) 
			{
				if (strData.GetLength() == 0)	strTmp2.Format(_T(", NULL"), strData);
				else							strTmp2.Format(_T(", to_date('%s', 'YYYY/MM/DD HH24:MI:SS')"), strData);
			}
			//2003-1110 박길주=======================================================
			//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
			else
				return -7;
			//2003-1110 박길주-------------------------------------------------------
		} 

		strFIELDS += strTmp1;
		strVALUES += strTmp2;
	}

	strResult.Format(_T("INSERT INTO %s (%s) VALUES (%s);"), TableInfo.TABLE_NAME, strFIELDS, strVALUES);

	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::LOG_MakeUpdateFrame(CESL_DataMgr *pDM,CESL_DataMgr *pDM2, INT nRowIdx, CString &strResult)
{
EFS_BEGIN

	strResult = _T("");
	if (pDM == NULL) return -1;
	if (m_pMainDM == NULL) return -1;
	if (nRowIdx < 0 || nRowIdx >= pDM->GetRowCount()) return -1;

	INT ids;
	CString strFileName;

	if(pDM->GetCellData(nRowIdx,2) == _T("CO_LAST_NUMBER_TBL"))
	{
		return 0;
	}
	if(pDM->GetCellData(nRowIdx,2) == _T("KL_LOCAL_TEMP_INFO_TBL"))
	{
		return 0;
	}
	if(pDM->GetCellData(nRowIdx,2) == _T("CD_CODE_TBL"))
	{
		int i=0;
	}

	if(pDM->GetCellData(nRowIdx,2) == _T("LS_WORK_T01"))
	{
		strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\LS_WORK_T01_2.CFG"));
		CKolas2up_TableInfo TableInfo;
		ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
		if (ids < 0) return -4;


		//	CESL_DataMgr dm;
		//	dm.SetCONNECTION_INFO(m_pMobileDM->CONNECTION_INFO);
		TableInfo.CONDITION.Format(_T("REC_KEY=%s"), pDM->GetCellData(nRowIdx, 3));

		//Queue Log 정보를 이용하여 DB의 해당자료테이블의 해당레코드를 가져온다.
		CString strSelectQuery;
		TableInfo.MakeSelectQuery(strSelectQuery);
		ids = pDM2->RestructDataManager(strSelectQuery);
	}

	strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\%s.CFG"), pDM->GetCellData(nRowIdx, 2));
	CKolas2up_TableInfo TableInfo;
	
	if(pDM->GetCellData(nRowIdx,2) == _T("CO_LOAN_USER_TBL"))
	{
		CString strUserKey,strRemarkEditDate_Main,strRemarkEditDate_TL,strQuery;
		strUserKey = pDM->GetCellData(nRowIdx, 3);
		strQuery.Format(_T("SELECT to_char(REMARK_EDIT_DATE,'YYYYMMDD:HH24MISS') FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"),strUserKey);

		m_pMainDM->GetOneValueQuery(strQuery,strRemarkEditDate_Main);
		pDM->GetOneValueQuery(strQuery,strRemarkEditDate_TL);

		// nValue가 0이면 본관이 크다, 1이면 이동이 크다 그러므로 올리자!!
		INT nValue = CheckDateHigher(strRemarkEditDate_Main,strRemarkEditDate_TL);
		
		if(nValue == 1)
		{
			strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\CO_LOAN_USER_TBL_3.CFG"));
			CKolas2up_TableInfo TableInfo2;
			ids = TableInfo2.ReadTableFieldInfoFromFile(strFileName);
			if (ids < 0) return -4;

			TableInfo2.CONDITION.Format(_T("REC_KEY=%s"), pDM->GetCellData(nRowIdx, 3));

			//Queue Log 정보를 이용하여 DB의 해당자료테이블의 해당레코드를 가져온다.
			CString strSelectQuery;
			TableInfo2.MakeSelectQuery(strSelectQuery);

			ids = pDM2->RestructDataManager(strSelectQuery);

			strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\CO_LOAN_USER_TBL.CFG"));
			ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
		}
		else
		{
			strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\CO_LOAN_USER_TBL_2.CFG"));
			ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
			if (ids < 0) return -4;
			//CKolas2up_TableInfo TableInfo2;
			TableInfo.CONDITION.Format(_T("REC_KEY=%s"), pDM->GetCellData(nRowIdx, 3));

			CString strSelectQuery;
			TableInfo.MakeSelectQuery(strSelectQuery);

			ids = pDM2->RestructDataManager(strSelectQuery);
		}

		//strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\%s.CFG"), pDM->GetCellData(nRowIdx, 2));
	}
	else
	{
		ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
	}

	if (ids < 0) return -4;

	CString strRecKey = pDM->GetCellData(nRowIdx, 3);
//	CESL_DataMgr dm;
//	dm.SetCONNECTION_INFO(m_pMobileDM->CONNECTION_INFO);

	CString strSelectQuery;
	TableInfo.CONDITION.Format(_T("REC_KEY=%s"), pDM->GetCellData(nRowIdx, 3));
	TableInfo.MakeSelectQuery(strSelectQuery);
	
//DEL	if(pDM->GetCellData(nRowIdx,2) != _T("LS_WORK_T01"))
//DEL	{
//DEL		ids = pDM2->RestructDataManager(strSelectQuery);
//DEL	}

	if((pDM->GetCellData(nRowIdx,2) != _T("LS_WORK_T01"))&&(pDM->GetCellData(nRowIdx,2) != _T("CO_LOAN_USER_TBL")))
	{
		ids = pDM2->RestructDataManager(strSelectQuery);
	}

	if (ids < 0) return -3;

	//2003-1110 박길주=======================================================
	//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (1>pDM2->GetRowCount()) return 0;
	//2003-1110 박길주-------------------------------------------------------

	CString strQuery = _T("");
	//2003-1110 박길주=======================================================
	//if (pDM == NULL) return -1;	<- 불필요한 코드, 이게 왜 여기에 있는지?
	//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003-1110 박길주-------------------------------------------------------
	
	CString strFIELDS, strTmp1, strData;
	strQuery = _T("");
	strFIELDS = _T("");
	if (TableInfo.nField == 0) return -6;

	INT nMobileDataRowIdx = 0;
	for (INT i = 0; i < TableInfo.nField; i++) 
	{
		strTmp1 = _T("");
		strData = pDM2->GetCellData(nMobileDataRowIdx, i);
		strData.TrimLeft();		
		strData.TrimRight();
		strData.Replace(_T("'"), _T("''"));
		if (i == 0) 
		{
			if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_INT) 
			{
				if (strData.GetLength() == 0) strData = _T("NULL");
				strTmp1.Format(_T("%s = %s"), TableInfo.FIELDNAME[i], strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_STR) 
			{
				strTmp1.Format(_T("%s = '%s'"), TableInfo.FIELDNAME[i], strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_DATE) 
			{
				//2003-1110 박길주=======================================================
				//strTmp1.Format(_T("%s = to_date('%s', 'YYYY/MM/DD HH24:MI:SS')"), TableInfo.FIELDNAME[i], strData);
				//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (strData.GetLength() == 0)	
					strTmp1.Format(_T("%s = NULL"), TableInfo.FIELDNAME[i]);
				else
					strTmp1.Format(_T("%s = to_char('%s', 'YYYY/MM/DD HH24:MI:SS')"), TableInfo.FIELDNAME[i], strData);
				//2003-1110 박길주-------------------------------------------------------
			}
			//2003-1110 박길주=======================================================
			//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
			else
				return -7;
			//2003-1110 박길주-------------------------------------------------------
		}
		else 
		{
			if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_INT) 
			{
				if (strData.GetLength() == 0) strData = _T("NULL");
				strTmp1.Format(_T(", %s = %s"), TableInfo.FIELDNAME[i], strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_STR) 
			{
				strTmp1.Format(_T(", %s = '%s'"), TableInfo.FIELDNAME[i], strData);
			} 
			else if (TableInfo.FIELDTYPE[i] == TABLE_FIELD_DATE) 
			{
				//2003-1110 박길주=======================================================
				//strTmp1.Format(_T(", %s = to_date('%s', 'YYYY/MM/DD HH24:MI:SS')"), TableInfo.FIELDNAME[i], strData);
				//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (strData.GetLength() == 0)	
					strTmp1.Format(_T(", %s = NULL"), TableInfo.FIELDNAME[i]);
				else
					strTmp1.Format(_T(", %s = to_date('%s', 'YYYY/MM/DD HH24:MI:SS')"), TableInfo.FIELDNAME[i], strData);
				//2003-1110 박길주-------------------------------------------------------
			}
			//2003-1110 박길주=======================================================
			//2003-1110 박길주+++++++++++++++++++++++++++++++++++++++++++++++++++++++
			else
				return -7;
			//2003-1110 박길주-------------------------------------------------------
		} 
		strFIELDS += strTmp1;
	}

	strResult.Format(_T("UPDATE %s SET %s WHERE REC_KEY=%s;"), TableInfo.TABLE_NAME, strFIELDS, strRecKey);

	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::LOG_MakeDeleteFrame(CESL_DataMgr *pDM, INT nRowIdx, CString &strResult)
{
EFS_BEGIN

	strResult = _T("");
	if (pDM == NULL) return -1;
	if (nRowIdx < 0 || nRowIdx >= pDM->GetRowCount()) return -1;
	
	if(pDM->GetCellData(nRowIdx,2) == _T("KL_LOCAL_TEMP_INFO_TBL"))
	{
		return 0;
	}

	INT ids;
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Mobile\\UPLOAD\\%s.CFG"), pDM->GetCellData(nRowIdx, 2));
	
	CKolas2up_TableInfo TableInfo;
	ids = TableInfo.ReadTableFieldInfoFromFile(strFileName);
	if (ids < 0) return -4;

	strResult.Format(_T("DELETE FROM %s WHERE REC_KEY=%s;"), 
					 pDM->GetCellData(nRowIdx, 2), pDM->GetCellData(nRowIdx, 3));

	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::LOG_MakeQueryFrame(CESL_DataMgr *pDM, INT nRowIdx, CString &strResult)
{
EFS_BEGIN

	strResult = _T("");
	if (pDM == NULL) return -1;
	if (nRowIdx < 0 || nRowIdx >= pDM->GetRowCount()) return -1;

	strResult = pDM->GetCellData(nRowIdx, 4);
	strResult.TrimLeft();			
	strResult.TrimRight();
	
	return 0;

EFS_END
return -1;

}

INT CKolas2up_MobileLogExport::Make_CO_TL_LOAN_USER_TBL_Query(CESL_DataMgr *pDM, INT nRow, CString strQueryType, CKolas2up_MobileLogQuery *pQuery)
{
EFS_BEGIN

	if (strQueryType == _T("Q")) return 0;

	CString strQuery;

	/// 1. 우선 대출자 공유를 하는 도서관이라면 Temp Table에 공유 정보를 입력한다.
	if( m_sIsCenterUpUser == _T("1") || (m_sIsCenterUpUser == _T("2")) )
	{
		CString sCTUpdateQuery;
		CString sType;
		if( strQueryType == _T("I") || strQueryType == _T("U") ) sType = _T("I");
		else sType = _T("D");

		CString sLibCode;
		CString sTempQuery;

		sTempQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL"),m_strUserManageCode);
		m_pMobileDM->GetOneValueQuery(sTempQuery,sLibCode);

		sCTUpdateQuery.Format(_T("INSERT INTO CT_UPLOAD_TEMP_LOAN_INFO_TBL(REC_KEY,TYPE,COMMAND,MASTER_USER_INFO_CHANGE_YN,UPDATED_TABLE_NAME,UPDATED_TABLE_REC_KEY,RECORD_DATE,UPLOAD_TRY_CNT,UPLOAD_FAIL_REASON_CODE,FIRST_WORK,LIB_CODE) ")
		_T("VALUES(ESL_SEQ.NextVal,'%s','11','Y','CO_LOAN_USER_TBL',%s,SYSDATE,0,'444','이동UP','%s');"),sType,pDM->GetCellData(nRow, 3),sLibCode);

		pQuery->m_arrAddonQuery.Add(sCTUpdateQuery);
	}

	INT nResult = 8;
	CString strResult[8];
	//                        0            1                  2              3                          4                 5       6             7                
	strQuery.Format(_T("SELECT REC_KEY, LOAN_STOP_SET_DATE, LOAN_STOP_DATE, USER_CLASS_CODE ,USER_POSITION_CODE ,REMARK,DELAY_COUNT,DELAYDAY_COUNT   FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), pDM->GetCellData(nRow, 3));
	pDM->GetValuesQuery(strQuery, strResult, nResult);

	strResult[3].Replace(_T("'") , _T("''") );
	strResult[4].Replace(_T("'") , _T("''") );
	strResult[5].Replace(_T("'") , _T("''") );
	strResult[6].Replace(_T("'") , _T("''") );
	strResult[7].Replace(_T("'") , _T("''") );

	// Delete 인경우는 CO_TL_LOAN_USER_TBL에서 삭제해준다.
	if (strQueryType == _T("D")) {
		// CO_TL_LOAN_USER_TBL에 대한 DELETE 쿼리 추가
		// 2003년 12월 27일 조병걸 수정 REC_KEY에 아무것도 들어가지 않아 에러가 난다.
//		strQuery.Format(_T("DELETE FROM CO_TL_LOAN_USER_TBL WHERE USER_KEY = %s;"), strResult[0]);
		strQuery.Format(_T("DELETE FROM CO_TL_LOAN_USER_TBL WHERE USER_KEY = %s;"), pDM->GetCellData(nRow, 3));
		pQuery->m_arrAddonQuery.Add(strQuery);
		return 0;
	}

	// Insert, Update 인 경우
	if (strQueryType == _T("I") || strQueryType == _T("U")) {
		// CO_TL_LOAN_USER_TBL에 대한 DELETE 쿼리 추가
		strQuery.Format(_T("DELETE FROM CO_TL_LOAN_USER_TBL WHERE USER_KEY = %s AND MANAGE_CODE='%s';"), strResult[0], m_strUserManageCode);
		pQuery->m_arrAddonQuery.Add(strQuery);

		if( strResult[6].IsEmpty() ) strResult[6] = _T("NULL");
		if( strResult[7].IsEmpty() ) strResult[7] = _T("NULL");

		// CO_TL_LOAN_USER_TBL에 대한 INSERT 쿼리 추가
		strQuery.Format(_T("INSERT INTO CO_TL_LOAN_USER_TBL(")
						_T("REC_KEY, MANAGE_CODE, USER_KEY, ")
						_T("LOAN_STOP_SET_DATE, LOAN_STOP_DATE, USER_CLASS_CODE, ")
						_T("USER_POSITION_CODE , REMARK , DELAY_COUNT,DELAYDAY_COUNT ,FIRST_WORK) VALUES (")
						_T("ESL_SEQ.NEXTVAL, '%s', %s, ")
						_T("%s, %s, '%s', ")
						_T("'%s','%s', ")
						_T("%s,%s,")
						_T("'%s');"),
						m_strUserManageCode, strResult[0], 
						Make_Oracle_DateString(strResult[1]), Make_Oracle_DateString(strResult[2]), strResult[3],
						strResult[4] , strResult[5] , 
						strResult[6] , strResult[7] , 
						m_strWorkLogMsg
						);
		pQuery->m_arrAddonQuery.Add(strQuery);

		/*
		// 이동도서관대출정책공유여부 가 Y 일 경우에는 CO_LOAN_USER_TBL에 대한 UPDATE 추가
		if (m_strShareLoanPolicy == _T("Y")) {
			strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET ")
							_T("LOAN_STOP_SET_DATE = %s, ")
							_T("LOAN_STOP_DATE = %s, ")
							_T("USER_CLASS_CODE = '%s' ,")
							_T("USER_POSITION_CODE = '%s' ,")
							_T("REMARK = '%s' ,")
							_T("DELAY_COUNT = %s ,")
							_T("DELAYDAY_COUNT = %s ,")
							_T("LAST_WORK = '%s' ")
							_T("WHERE REC_KEY=%s;"),
							Make_Oracle_DateString(strResult[1]), 
							Make_Oracle_DateString(strResult[2]), 
							strResult[3],
							strResult[4],
							strResult[5],
							strResult[6],
							strResult[7],
							m_strWorkLogMsg,
							strResult[0]
							);
			pQuery->m_arrAddonQuery.Add(strQuery);
		}
		*/

		return 0;		
	}

	return 0;

EFS_END
return -1;

}

CString CKolas2up_MobileLogExport::Make_Oracle_DateString(CString strData)
{
EFS_BEGIN

	CString strTmp2;
	if (strData.GetLength() == 0)	strTmp2.Format(_T("NULL"), strData);
	else							strTmp2.Format(_T("to_date('%s', 'YYYY/MM/DD HH24:MI:SS')"), strData);

	return strTmp2;

EFS_END
return _T("");

}

INT CKolas2up_MobileLogExport::CheckDateHigher(CString strMainDate, CString strTLDate)
{
	//YYYYMMDD|HH24MISS

	INT nResult=0;
	//NULL이 넘어온경우도 처리
	//둘다 날짜가 없을때
	if(strMainDate.IsEmpty()&&strTLDate.IsEmpty())
	{
		nResult=0;
	}
	//본관만 날짜가 있을때
	else if(!strMainDate.IsEmpty()&&strTLDate.IsEmpty())
	{
		nResult=0;
	}
	//이동만 날짜가 있을때
	else if(strMainDate.IsEmpty()&&!strTLDate.IsEmpty())
	{
		nResult=1;
	}
	//둘다 날짜가 있을때
	{

		CString strRemarkEditDate_Main_Date;
		CString	strRemarkEditDate_TL_Date;
		CString	strRemarkEditDate_Main_Time;
		CString	strRemarkEditDate_TL_Time; 

		LONG	nRemarkEditDate_Main_Date;
		LONG	nRemarkEditDate_TL_Date;
		LONG	nRemarkEditDate_Main_Time;
		LONG	nRemarkEditDate_TL_Time;

		INT idx = strMainDate.Find(_T(":"));
		INT idx2 = strTLDate.Find(_T(":"));
		if(idx<0)
		{
			nResult = 0;
		}

		strRemarkEditDate_Main_Date = strMainDate.Left(idx);
		strRemarkEditDate_Main_Time = strMainDate.Mid(idx+1);
		strRemarkEditDate_TL_Date = strTLDate.Left(idx2);
		strRemarkEditDate_TL_Time = strTLDate.Mid(idx2+1);

		nRemarkEditDate_Main_Date = _ttoi(strRemarkEditDate_Main_Date);
		nRemarkEditDate_TL_Date = _ttoi(strRemarkEditDate_TL_Date);

		if(nRemarkEditDate_Main_Date > nRemarkEditDate_TL_Date)
		{
			nResult = 0;
		}
		else if(nRemarkEditDate_Main_Date < nRemarkEditDate_TL_Date)
		{
			nResult = 1;
		}
		else if(nRemarkEditDate_Main_Date == nRemarkEditDate_TL_Date)
		{
			nRemarkEditDate_Main_Time = _ttoi(strRemarkEditDate_Main_Time);
			nRemarkEditDate_TL_Time = _ttoi(strRemarkEditDate_TL_Time);

			if(nRemarkEditDate_Main_Time > nRemarkEditDate_TL_Time)
			{
				nResult = 0;
			}
			else if(nRemarkEditDate_Main_Time < nRemarkEditDate_TL_Time)
			{
				nResult = 1;
			}
			else if(nRemarkEditDate_Main_Time == nRemarkEditDate_TL_Time)
			{
				nResult = 0;
			}
		}
	}
	return nResult;
}
