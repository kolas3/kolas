// Kolas2up_TableInfo.cpp: implementation of the CKolas2up_TableInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_TableInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_TableInfo::CKolas2up_TableInfo()
{
	nField = 0;
	TITLE = _T("");
	DESCRIPTION = _T("");
	TABLE_NAME = _T("");
	CONDITION = _T("");
	EXTRA_CONDITION = _T("");
	APPLYCONDITION_TABLE_NAME = _T("");

	m_bReplace = TRUE;
	m_nSENDCOUNT = 1;
	m_nSELECTCOUNT = -1;
}

CKolas2up_TableInfo::~CKolas2up_TableInfo()
{

}

INT CKolas2up_TableInfo::ReadTableFieldInfoFromFile(CString strFilename)
{
	nField = 0;
	TABLE_NAME = _T("");
	CONDITION = _T("");
	EXTRA_CONDITION = _T("");

	CStdioFile file;
	if (!file.Open(strFilename, CFile::modeRead | CFile::typeBinary)) return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	FILE_NAME = strFilename;

	CString strLine;
	INT pos;
	CString strFIELDNAME, strFIELDTYPE;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();				
		strLine.TrimRight();
		if (strLine.IsEmpty())		continue;
		if (strLine.Left(1) == '#') continue;
		
		if (strLine.Left(1) == '.') 
		{
			ReadTableInfo(strLine);
			continue;
		}
		
		strFIELDNAME = _T("");
		strFIELDTYPE = _T("");
		pos = strLine.Find('\t', 0);
		if (pos == -1) 
		{
			pos = strLine.Find(' ', 0);
			if (pos == -1) continue;
		}

		strFIELDNAME = strLine.Left(pos);
		strFIELDNAME.TrimLeft();		
		strFIELDNAME.TrimRight();			
		strFIELDTYPE = strLine.Mid(pos+1);
		strFIELDTYPE.TrimLeft();		
		strFIELDTYPE.TrimRight();		
		strFIELDTYPE.MakeUpper();

		if (strFIELDNAME.GetLength() == 0) continue;
		if (strFIELDTYPE.GetLength() == 0) continue;

		FIELDNAME[nField] = strFIELDNAME;
		if (strFIELDTYPE == _T("INT") || strFIELDTYPE == _T("NUMERIC"))		FIELDTYPE[nField] = TABLE_FIELD_INT;
		if (strFIELDTYPE == _T("STR") || strFIELDTYPE == _T("STRING"))		FIELDTYPE[nField] = TABLE_FIELD_STR;
		if (strFIELDTYPE == _T("DATE") || strFIELDTYPE == _T("DATETIME"))	FIELDTYPE[nField] = TABLE_FIELD_DATE;

		nField++;
	}
	file.Close();

	return 0;
}

INT CKolas2up_TableInfo::ReadTableInfo(CString strLine)
{
	INT pos = strLine.Find('=', 0);
	if (pos == -1) return -1;

	CString strTag, strData;
	strTag = strLine.Left(pos);
	strTag.TrimLeft();		
	strTag.TrimRight();			
	strData = strLine.Mid(pos+1);
	strData.TrimLeft();		
	strData.TrimRight();		
	if (strData.GetLength() == 0) return 0;

	if (strTag == _T(".TABLE_NAME"))					TABLE_NAME = strData;
	if (strTag == _T(".CONDITION"))						CONDITION = strData;
	if (strTag == _T(".EXTRA_CONDITION"))				EXTRA_CONDITION = strData;
	if (strTag == _T(".TITLE"))							TITLE = strData;
	if (strTag == _T(".DESCRIPTION"))					DESCRIPTION = strData;
	if (strTag == _T(".APPLYCONDITION_TABLE_NAME"))		APPLYCONDITION_TABLE_NAME = strData;
	if (strTag == _T(".APPLYCONDITION_KEY_FIELD_NAME")) APPLYCONDITION_KEY_FIELD_NAME = strData;

	if (strTag == _T(".REPLACE"))	
	{
		strData.MakeUpper();
		if (strData == _T("F") || strData == _T("FALSE")) 
			m_bReplace = FALSE;
		else 
			m_bReplace = TRUE;
	}
	if (strTag == _T(".SEND_COUNT"))	m_nSENDCOUNT	= _ttoi(strData);
	if (strTag == _T(".SELECT_COUNT"))	m_nSELECTCOUNT	= _ttoi(strData);

	return 0;
}

INT CKolas2up_TableInfo::MakeSelectQuery(CString &strQuery, CString strCondition)
{
	CString strFIELDS, strOption, strTmp;
	strQuery = _T("");
	strFIELDS = _T("");
	if (nField == 0) return -1;

	for (INT i = 0; i < nField; i++) {
		strTmp = _T("");
		if (i == 0) {
			strTmp.Format(_T("%s"), FIELDNAME[i]);
		} else {
			strTmp.Format(_T(", %s"), FIELDNAME[i]);
		} 
		strFIELDS += strTmp;
	}

	if (strCondition.GetLength() == 0) {
			strQuery.Format(_T("SELECT %s FROM %s"), 
							strFIELDS, 
							TABLE_NAME
							);
	} else {
			strQuery.Format(_T("SELECT %s FROM %s WHERE %s"), 
							strFIELDS, 
							TABLE_NAME,
							strCondition
							);
	}

	return 0;
}

CString CKolas2up_TableInfo::GetCondition(CString strSubCondField, CString strSubCondFieldType, CString strSubCondFieldData, CString strOption)
{
	CString strResult = CONDITION;
	CString strTmp1, strTmp2;

	if (TABLE_NAME == _T("IDX_BO_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE='%s')"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE='%s') OR REC_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE='%s')"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE='%s') OR REC_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}

		if( !strSubCondField .IsEmpty() )
			strResult.Format(_T("SPECIES_CLASS='1' AND %s"), strTmp1);
		else
			strResult.Format(_T("SPECIES_CLASS='1' "));
		
		return strResult;
	}
	if (TABLE_NAME == _T("BO_SPECIES_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE='%s')"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE='%s') OR REC_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE='%s')"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE='%s') OR REC_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}

		if( !strSubCondField .IsEmpty() )
			strResult.Format(_T("LOC_FLAG='0' AND %s"), strTmp1);
		else
			strResult.Format(_T("LOC_FLAG='0' "));

		return strResult;
	}
	if (TABLE_NAME == _T("BO_BOOK_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("MANAGE_CODE='%s'"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( MANAGE_CODE='%s' OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("SHELF_LOC_CODE='%s'"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( SHELF_LOC_CODE='%s' OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}
		
		if( !strSubCondField.IsEmpty() )
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			strResult.Format(_T("%s AND WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL252N')"),
// 							strTmp1
// 						);
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			// 18.09.27 JMJ KOL.RED.2018.008
			strResult.Format(_T("%s AND WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL411O','BOL511O','BOL611O','BOL252N')"),
							strTmp1
						);
//			strResult.Format(_T("%s AND WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL511O','BOL252N')"),
//							strTmp1
//						);
			//=====================================================
		else
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			strResult.Format(_T("WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL252N')"));
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			// 18.09.27 JMJ KOL.RED.2018.008
			strResult.Format(_T("WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL252N','BOL411O','BOL511O','BOL611O')"));
//			strResult.Format(_T("WORKING_STATUS IN ('BOL112N','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL113O','BOL114O','BOL115O','BOL252N','BOL511O')"));
			//=====================================================
			
		
			return strResult;
	}
	if (TABLE_NAME == _T("IDX_SE_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE='%s')"),
								strSubCondFieldData
							);
			} else {
				strTmp1.Format(_T("(REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE='%s') OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"),
								strSubCondFieldData
							);
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s')"),
								strSubCondFieldData
							);
			} else {
				strTmp1.Format(_T("(REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s') OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"),
								strSubCondFieldData
							);
			}
		}

		if( !strSubCondField .IsEmpty() )
			strResult.Format(_T("SPECIES_CLASS='1' AND %s"), strTmp1);
		else
			strResult.Format(_T("SPECIES_CLASS='1' "));

		return strResult;
	}
	if (TABLE_NAME == _T("SE_SPECIES_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE='%s')"),
								strSubCondFieldData
							);
			} else {
				strTmp1.Format(_T("(REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE='%s') OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"),
								strSubCondFieldData
							);
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s')"),
								strSubCondFieldData
							);
			} else {
				strTmp1.Format(_T("(REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s') OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"),
								strSubCondFieldData
							);
			}
		}

		if( !strSubCondField .IsEmpty() )
			strResult.Format(_T("LOC_FLAG='0' AND %s"), strTmp1);
		else
			strResult.Format(_T("LOC_FLAG='0' "));

		return strResult;
	}
	if (TABLE_NAME == _T("SE_VOL_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				//=====================================================
				//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE = '%s' AND WORKING_STATUS IN ( 'SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O'))"),
// 								strSubCondFieldData
// 								);
								// 18.09.27 JMJ KOL.RED.2018.008
								strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE = '%s' AND WORKING_STATUS IN ( 'SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O'))"),
								strSubCondFieldData
								);
				//=====================================================
				
							
			} else {
				//=====================================================
				//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE = '%s' AND WORKING_STATUS IN ( 'SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O')) OR REC_KEY IN (SELECT VOL_KEY FROM BO_BOOK_TBL WHERE BOOK_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3')))"),
// 								strSubCondFieldData
// 							);
				// 18.09.27 JMJ KOL.RED.2018.008
				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE MANAGE_CODE = '%s' AND WORKING_STATUS IN ( 'SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O')) OR REC_KEY IN (SELECT VOL_KEY FROM BO_BOOK_TBL WHERE BOOK_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3')))"),
								strSubCondFieldData
							);
				//=====================================================
				
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				//=====================================================
				//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s' AND WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O'))"), strSubCondFieldData);
				// 18.09.27 JMJ KOL.RED.2018.008
				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s' AND WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O'))"), strSubCondFieldData);
				//=====================================================
				
			} else {
				//=====================================================
				//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s' AND WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O')) OR REC_KEY IN (SELECT VOL_KEY FROM BO_BOOK_TBL WHERE BOOK_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3')))"), strSubCondFieldData);
				// 18.09.27 JMJ KOL.RED.2018.008
				strTmp1.Format(_T("REC_KEY IN ( SELECT VOL_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE='%s' AND WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O')) OR REC_KEY IN (SELECT VOL_KEY FROM BO_BOOK_TBL WHERE BOOK_KEY IN (SELECT SPECIES_KEY FROM LH_STORAGE_V01 WHERE STATUS IN ('0','2','3')))"), strSubCondFieldData);
				//=====================================================
			}
		}

		if( !strSubCondField.IsEmpty() )
			strResult = strTmp1;

		return strResult;
	}
	if (TABLE_NAME == _T("SE_BOOK_TBL")) {
		if (strSubCondField == _T("MANAGE_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("MANAGE_CODE='%s'"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( MANAGE_CODE='%s' OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}
		if (strSubCondField == _T("SHELF_LOC_CODE")) {
			if (strOption.GetLength() == 0) {
				strTmp1.Format(_T("SHELF_LOC_CODE='%s'"), strSubCondFieldData);
			} else {
				strTmp1.Format(_T("( SHELF_LOC_CODE='%s' OR REC_KEY IN (SELECT BOOK_KEY FROM LH_STORAGE_V01 WHERE STATUS IN('0', '2', '3')) )"), strSubCondFieldData);
			}
		}

		if( !strSubCondField.IsEmpty() )
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			strResult.Format(_T("%s AND WORKING_STATUS IN ('SEL112N', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O')"),
// 							strTmp1
// 						);
			// 18.09.27 JMJ KOL.RED.2018.008
			strResult.Format(_T("%s AND WORKING_STATUS IN ('SEL112N', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O')"),
							strTmp1
						);
			//=====================================================
			

		return strResult;
	}
	if (TABLE_NAME == _T("LH_STORAGE_V01")) {
		if (strOption.GetLength() == 0) {
			if (strSubCondField == _T("MANAGE_CODE")) {
				strTmp1.Format(_T("MANAGE_CODE='%s'"), strSubCondFieldData);
			}
			if (strSubCondField == _T("SHELF_LOC_CODE")) {
				strTmp1.Format(_T("SHELF_LOC_CODE='%s'"), strSubCondFieldData);
			}

			if( !strSubCondField.IsEmpty() )
				strResult.Format(_T("STATUS IN ('0', '2','3') AND %s"),  strTmp1);
			else
				strResult.Format(_T("STATUS IN ('0', '2','3') "));
		} else {
			strResult.Format(_T("STATUS IN ('0', '2','3')"));
		}

		return strResult;
	}

	return strResult;
}

INT CKolas2up_TableInfo::MakeSelectQuery(CString &strQuery)
{
	CString strFIELDS, strOption, strTmp;
	strQuery = _T("");
	strFIELDS = _T("");
	if (nField == 0) return -1;

	for (INT i = 0; i < nField; i++) 
	{
		strTmp = _T("");
		if (i == 0) strTmp.Format(_T("%s")  , FIELDNAME[i]);
		else		strTmp.Format(_T(", %s"), FIELDNAME[i]);

		strFIELDS += strTmp;
	}
	if (CONDITION.GetLength() == 0 && EXTRA_CONDITION.GetLength() == 0) 
	{
		strQuery.Format(_T("SELECT %s FROM %s"), strFIELDS, TABLE_NAME);
	}
	//if (CONDITION.GetLength() > 0 && EXTRA_CONDITION.GetLength() == 0) 
	else if (CONDITION.GetLength() > 0 && EXTRA_CONDITION.GetLength() == 0) 
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s"), strFIELDS, TABLE_NAME, CONDITION);
	}
	//if (CONDITION.GetLength() == 0 && EXTRA_CONDITION.GetLength() > 0) 
	else if (CONDITION.GetLength() == 0 && EXTRA_CONDITION.GetLength() > 0) 
	{
		strQuery.Format(_T("SELECT %s FROM %s %s"), strFIELDS, TABLE_NAME, EXTRA_CONDITION);
	}
	//if (CONDITION.GetLength() > 0 && EXTRA_CONDITION.GetLength() > 0) 
	else if (CONDITION.GetLength() > 0 && EXTRA_CONDITION.GetLength() > 0) 
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s %s"), strFIELDS, TABLE_NAME, EXTRA_CONDITION);
	}
	
	return 0;
}

INT CKolas2up_TableInfo::MakeInsertQuery(CESL_DataMgr *pDM, INT nRowIdx, CString &strQuery)
{
	INT ids;
	strQuery = _T("");
	if (pDM == NULL) return -1;
	CString strFIELDS, strVALUES, strTmp1, strTmp2, strData;
	strQuery = _T("");
	strFIELDS = _T("");
	strVALUES = _T("");
	if (nField == 0) return -2;

	for (INT i = 0; i < nField; i++) 
	{
		strTmp1 = _T("");
		strTmp2 = _T("");
		strData = pDM->GetCellData(nRowIdx, i);
		strData.TrimLeft();			
		strData.TrimRight();

		/// 2003년 12월 27일 조병걸 수정
		/// 한글인데 깨진 문자가 있는지 조사해볼까??
		if (FIELDTYPE[i] == TABLE_FIELD_STR) 
		{
			ids = CheckBreakHangeul(strData);
			if( ids > 0 )
			{
				CString sEMsg;
				//2004-03-03 [이학중]
				sEMsg.Format( _T(">> 깨진한글이 있음 : %s [ids:%d,index:%d,field:%s]") , strData , ids ,nRowIdx , FIELDNAME[i] );
				pDM->QueryLog( sEMsg.GetBuffer(0) );
			}
		}

		if (m_bReplace) 
		{
			strData.Replace(_T("'"), _T("''"));
			strData.Replace(_T("\\"), _T("\\\\"));
		}
		if (i == 0) 
		{
			strTmp1.Format(_T("%s"), FIELDNAME[i]);
			if (FIELDTYPE[i] == TABLE_FIELD_INT) 
			{
				if (strData.GetLength() == 0) 
					strTmp2.Format(_T("NULL"));
				else  
					strTmp2.Format(_T("%s"), strData);
			} 
			else if (FIELDTYPE[i] == TABLE_FIELD_STR) 
			{
				strTmp2.Format(_T("'%s'"), strData);
			} 
			else if (FIELDTYPE[i] == TABLE_FIELD_DATE) 
			{
				if (strData.GetLength() == 0) 
					strTmp2.Format(_T("NULL"), strData);
				else 
					strTmp2.Format(_T("'%s'"), strData);
			}
		} 
		else 
		{
			strTmp1.Format(_T(", %s"), FIELDNAME[i]);
			if (FIELDTYPE[i] == TABLE_FIELD_INT) 
			{
				if (strData.GetLength() == 0) 
					strTmp2.Format(_T(", NULL"));
				else
					strTmp2.Format(_T(", %s"), strData);
			} 
			else if (FIELDTYPE[i] == TABLE_FIELD_STR) 
			{
				strTmp2.Format(_T(", '%s'"), strData);
			} 
			else if (FIELDTYPE[i] == TABLE_FIELD_DATE) 
			{
				if (strData.GetLength() == 0) 
					strTmp2.Format(_T(", NULL"), strData);
				else 
					strTmp2.Format(_T(", '%s'"), strData);
			}
		} 
		strFIELDS += strTmp1;
		strVALUES += strTmp2;
	}

	strQuery.Format(_T("INSERT INTO %s (%s) VALUES (%s);"), TABLE_NAME, strFIELDS, strVALUES);
	return 0;
}

INT CKolas2up_TableInfo::GetFieldIdx(CString strFieldName)
{
	CString strTmp;
	strFieldName.MakeUpper();
	for (INT i = 0; i < nField; i++) {
		strTmp = FIELDNAME[i];
		strTmp.MakeUpper();

		if (strTmp == strFieldName) return i;
	}
	return -1;
}

INT CKolas2up_TableInfo::GetDataCountQuery(CString &strQuery, CString strCondition)
{
	strQuery = _T("");
	CString strFIELDS = _T("COUNT(*)");

	if (strCondition.GetLength() == 0) {
			strQuery.Format(_T("SELECT %s FROM %s"), 
							strFIELDS, 
							TABLE_NAME
							);
	} else {
			strQuery.Format(_T("SELECT %s FROM %s WHERE %s"), 
							strFIELDS, 
							TABLE_NAME,
							strCondition
							);
	}
	return 0;
}

INT CKolas2up_TableInfo::CheckBreakHangeul( CString& strData )
{

	INT nStrLength = strData.GetLength();

	TCHAR cTmp;
	for( INT i = 0 ; i < nStrLength ; i++ )
	{
		cTmp = strData.GetAt(i);
		if( cTmp >= 0 )
			continue;

		if( i+1 == nStrLength )
		{
			strData = strData.Mid( 0 , nStrLength-1 );
			return 1;
		}
			
		if( strData.GetAt(i+1) < 0 )
			i++;
		else
		{
			strData = strData.Mid( 0 , i ) ;
			return 2;
		}
	}

	return 0;

}