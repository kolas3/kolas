// MarcMgr.cpp: implementation of the CMarcMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcMgr.h"
#include "MarcOperation.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcMgr::CMarcMgr(CString strGroupName /*= _T("") */, BOOL bUseRelation /*= TRUE */)
{
	m_nEncodingCharset = CP_UTF8;
	m_bUseRelation = FALSE;
	ReadAliasFromFile();

	if (!strGroupName.IsEmpty())
		InitMarcMgr(strGroupName, bUseRelation);
}

CMarcMgr::~CMarcMgr()
{
	m_mapMarcAlias.RemoveAll();
	FreeErrMsgList();
	FreeSpecialSortInfo();
}

INT CMarcMgr::FreeSpecialSortInfo()
{
	EFS_BEGIN

	CString strKey;
	CStringArray *pArr = NULL;
	POSITION pos = m_mapSpecialSort.GetStartPosition();
	while (pos)
	{
		m_mapSpecialSort.GetNextAssoc(pos, strKey, (CObject*&)pArr);
		if (pArr != NULL)
		{
			delete pArr;
			pArr = NULL;
		}
	}

	m_mapSpecialSort.RemoveAll();

	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::FreeErrMsgList()
{
	EFS_BEGIN

	INT nListCount = m_listErrMsg.GetCount();	
	if ( nListCount == 0 ) return 0;
	CErrorMessage *pErrMsg;
	for (INT i = 0; i < nListCount; i++)
	{
		pErrMsg = (CErrorMessage*)m_listErrMsg.RemoveHead();
		if (pErrMsg) delete pErrMsg;
	}
	m_listErrMsg.RemoveAll();
	return nListCount;

	EFS_END
	return -1;
}

VOID CMarcMgr::AddErrMsg(CString strTagCode, CString strErrMsg, CString strHelpMsg)
{
	EFS_BEGIN

	CErrorMessage *newErrMsg;
	newErrMsg = new CErrorMessage;
	if (!newErrMsg) return;

	newErrMsg->m_strTagCode = strTagCode;
	newErrMsg->m_strErrMsg = strErrMsg;
	newErrMsg->m_strHelpMsg = strHelpMsg;

	m_listErrMsg.AddTail((CObject*)newErrMsg);

	EFS_END
}

POSITION CMarcMgr::GetHeadPositionErrMsg()
{
	EFS_BEGIN

	POSITION pos = m_listErrMsg.GetHeadPosition();
	return pos;
	

	EFS_END
	return NULL;
}

VOID CMarcMgr::GetNextErrMsg(CString &strTagCode, CString &strErrMsg, CString &strHelpMsg, POSITION &pos)
{
	EFS_BEGIN

	CErrorMessage *pErrorMessage;
	pErrorMessage = (CErrorMessage*)m_listErrMsg.GetNext(pos);
	
	strTagCode = pErrorMessage->m_strTagCode;
	strErrMsg = pErrorMessage->m_strErrMsg;
	strHelpMsg = pErrorMessage->m_strHelpMsg;

	EFS_END
	return;
}

//////////////////////////////////////////////////////////////////////
// Init Marc Manager
INT CMarcMgr::InitMarcMgr(CString strGroupName, BOOL bUseRelation /*=TRUE*/)
{
	EFS_BEGIN

	FreeErrMsgList();
	
	if (!strGroupName.IsEmpty())
		SetMarcGroup(strGroupName);
	
	// Init Marc relation
	m_bUseRelation = bUseRelation;
	if (m_bUseRelation)
	{
		if (m_marcRelation.MakeMarcRelation(GetMarcGroup()) < 0)
		{
			CString strErrMsg;
			CString strHelpMsg;

			strErrMsg.Format(_T("Init MarcOperator Error : %s"), m_marcRelation.GetLastError());
			AddErrMsg(_T(""), strErrMsg, _T(""));			
			return -1;
		}
	}

	return 0;

	EFS_END
	return -1;
}

VOID CMarcMgr::SetMarcGroup(CString strGroupName)
{
	EFS_BEGIN

	if (m_strGroupName.IsEmpty())
	{
		m_strGroupName = strGroupName;
	}
	else if (m_strGroupName != strGroupName)
	{
		m_strGroupName = strGroupName;
		m_marcRelation.MakeMarcRelation(GetMarcGroup());
		ReadAliasFromFile();
	}

	EFS_END
	return ;
}

INT CMarcMgr::ReadAliasFromFile()
{
	EFS_BEGIN

	m_mapMarcAlias.RemoveAll();

	// find in file
	CString strFileName;

	if (GetMarcGroup() == _T("연속"))
	{
		strFileName =  _T("..\\cfg\\marc_alias_연속.cfg");
	}
	else
		strFileName =  _T("..\\cfg\\marc_alias.cfg");

	CStdioFile file;
	if (!file.Open(strFileName, CFile::modeRead|CFile::typeBinary))
	{
		AddErrMsg(_T(""), _T("marc_alias.cfg file does not exist"), _T(""));
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine;
	CString strKeyValue;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty())
			continue;
		
		INT nStart = strLine.Find(_T("="));
		if (nStart < 0)
			continue;

		CString strAlias = strLine.Mid(0, nStart);
		strAlias.TrimLeft();
		strAlias.TrimRight();
		
		strKeyValue = strLine.Mid(nStart + 1);
		strKeyValue.TrimLeft();
		strKeyValue.TrimRight();

		m_mapMarcAlias.SetAt(strAlias, strKeyValue);
	}

	return 0;	

	EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////
// Encoding/Decoding
//////////////////////////////////////////////////////////////////////

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2003.09.09 수정전 
//INT CMarcMgr::Encoding(CMarc *pMarc, CString &strStreamMarc)
INT CMarcMgr::Encoding(CMarc *pMarc, CString &strStreamMarc ,BOOL bNewSort /*= FALSE*/)
{
	EFS_BEGIN

	//////////////////////////////////////////////////////////////////
	// Preprocess of encoding
	// Init
	strStreamMarc.Empty();
	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("Marc is Null"), _T(""));
		return -1;
	}
    //+++++++++++++++++++++++++++++++++++++++++++++
	// Sort
	//2003.09.09 수정전 
	//SortMarc(pMarc);
    //++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.09.08 이학중 수정
	// Sort
	if(bNewSort == FALSE)
	{
		SortMarc(pMarc);
	}
	else 
	{
			SortMarc(pMarc,SORT_ASC,TRUE);
	}

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	
	
	INT n = 0;
	
	///////////////////////////////////////////////////////////////////
	// Process encoding

	// Directory
	INT nDirCnt = pMarc->m_listTag.GetCount();
	const INT nLeaderLength = 24;
	const INT nDirTagLength = 3;
	const INT nDirFieldLength = 4;
	const INT nDirFieldInitAddrLength = 5;
	
	CString strDirs;
	TCHAR szDir[nDirTagLength + nDirFieldLength + nDirFieldInitAddrLength + 1];
	ZeroMemory(szDir, nDirTagLength + nDirFieldLength + nDirFieldInitAddrLength + 1);

	// fields	
	CString strField;
	CString strTotalField;
	INT		nFieldLength;
	INT		nFieldStartAddress = 0;
	
	CString strFieldTerminatorCode;	
	strFieldTerminatorCode.Format(_T("%c"), FIELD_TERMINATOR);	

	POSITION pos = pMarc->m_listTag.GetHeadPosition();
	while (pos)
	{
		n = 0;
		CTag* pTag = (CTag*)pMarc->m_listTag.GetNext(pos);
		if (IsControlField(pTag->GetTagCode()))
		{
			strField = pTag->GetItemData();
			strField += pTag->GetFieldTerminator();
		}
		else
		{
			strField = (pTag->GetFirstIndicator() + pTag->GetSecondIndicator());	// Add Indicator
			POSITION posSub = pTag->m_listSubfield.GetHeadPosition();
			while (posSub)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSub);
				if (pSubfield == NULL) continue;

				// 처음 구두점은 넣지 않음
				// Add punctuation mark
				if (n++ != 0) 
					strField += pSubfield->GetPunctuationMark();
				// Add Subfield code
				strField += pSubfield->GetSubfieldCode();	
				// Add ItemData
				strField += pSubfield->GetItemData();		 
				
			}

			strField += strFieldTerminatorCode;			
		}
		// Add directory...
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2006.12.1 HJY. 데이터에 레코드 종단기호가 있으면 없애버린다.
		CString sRT;
		sRT.Format(_T("%c"),RECORD_TERMINATOR);
		if( 0 <= strField.Find(sRT) )
			strField.Replace(sRT,_T(""));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		if ( m_nEncodingCharset == CP_UTF8 )
		{
			nFieldLength = GetLengthUnicode(CP_UTF8, strField);
			_stprintf(szDir, _T("%03s%04d%05d"), pTag->GetTagCode(), nFieldLength, nFieldStartAddress);
			
		}
		else if ( m_nEncodingCharset == CP_ACP || m_nEncodingCharset == CP_THREAD_ACP )
		{
			nFieldLength = GetLengthUnicode(CP_ACP, strField);
			_stprintf(szDir, _T("%03s%04d%05d"), pTag->GetTagCode(), nFieldLength, nFieldStartAddress);						
		}
		else
		{
			nFieldLength = strField.GetLength();
			_stprintf(szDir, _T("%03s%04d%05d"), pTag->GetTagCode(), nFieldLength, nFieldStartAddress);						
		}
		
		nFieldStartAddress += nFieldLength;
		strDirs += szDir;		
		strTotalField += strField;		
	}
	
	// Add field terminate code to end of directory
	strDirs += strFieldTerminatorCode;

	// Add record terminate code to end of field
	//===================================================
	//2010.07.14 ADD BY PJW : 레코드 종단 기호가 없는경우는 추가해 준다.
	CString strRecordTerminatorCode;	
	strRecordTerminatorCode.Format(_T("%c"), pMarc->GetRecordTerminator());	
	if( _T('') == pMarc->GetRecordTerminator() )
	{	
		pMarc->SetRecordTerminator(RECORD_TERMINATOR);
	}
	//===================================================

	strTotalField += pMarc->GetRecordTerminator();

	// Leader
	/*----------------------------------------------------
	00 - 04 레코드길이
	05		레코드상태
	06		레코드형태
	07		서지수준
	08 - 09 빈칸
	10		지시기호자리수
	11		식별기호자리수
	12 - 16	데이터기본번지
	17		입력수준
	18		목록기술형식
	19		연관레코드조건
	20 - 23	엔트리맵
	-------------------------------------------------------*/
	TCHAR szLeader[25];
	ZeroMemory(szLeader, 25);
	
	pMarc->GetLeader(szLeader);

	// 레코드 길이
	INT nRecordLength = nLeaderLength + strDirs.GetLength() + strTotalField.GetLength();
	// 데이터기본번지
	INT nDataBaseAddr = nLeaderLength + strDirs.GetLength();
		
	TCHAR szBuffer[6];
	ZeroMemory(szBuffer, 6);

	// input record length
	INT nPos;
	
	_stprintf(szBuffer, _T("%05d"), nRecordLength);
	nPos = 0;
	wmemcpy(szLeader + nPos, szBuffer, 5);

	// input data base address
	ZeroMemory(szBuffer, 6);
	
	_stprintf(szBuffer, _T("%05d"), nDataBaseAddr);
	nPos = 12;
	wmemcpy(szLeader + nPos, szBuffer, 5);
	
	pMarc->SetLeader(szLeader);
	strStreamMarc = szLeader + strDirs + strTotalField;

	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::Decoding(CString strStreamMarc, CMarc *pMarc)
{
	EFS_BEGIN

	// Free Error Message list
	FreeErrMsgList();
	
	m_strStreamMarc = strStreamMarc;
	//////////////////////////////////////////////////////////////////
	// Chcek
	strStreamMarc.TrimLeft();
	strStreamMarc.TrimRight();

	if (strStreamMarc.IsEmpty())	
	{
		AddErrMsg(_T(""), _T("stream marc is empty"), _T(""));
		return -1;
	}

	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("Marc is Null"), _T(""));
		return -1;
	}

	INT ids;
	pMarc->Init();

	///////////////////////////////////////////////////////////////////
	// Process Decoding
	
	// Leader
	CString strLeader;
	strLeader = strStreamMarc.Left(24);
	pMarc->SetLeader(strLeader.GetBuffer(0));

	// 2007.11.1 Refactoring
	// UTF8 -> UTF16으로 처리하기 위한 분리작업.
	// 단, 순서대로 매핑하지 않고 정렬을 통하여 분리한다.
	// 또한 디렉토리나 데이터 필드가 깨졌을 경우도 대비한다.
	CStringArray sTagDirList;	
	CStringArray sTagFieldList;

	// Directory	
	INT nDirInitAddr = 24;									// Init address of directory

	if (strStreamMarc.GetLength() < 12 + 5) return -1;
	INT nDataBaseAddr = _ttoi(strStreamMarc.Mid(12, 5));		// Init address of data
	INT nDirCnt = (nDataBaseAddr - nDirInitAddr - 1) / 12;	// Count of directory

	// Directory fetch
	CString strDir;
	INT nCurDirAddr;
// 	INT nDirFieldLength;			// Field length
// 	INT nDirFieldStartAddr;			// Field Start Address
	INT i, j;
	
	nCurDirAddr = nDirInitAddr;
	sTagDirList.RemoveAll();
	for ( i = 0; i < nDirCnt; i++)
	{
		if (strStreamMarc.GetLength() < nCurDirAddr + 12) return -1;
		strDir = strStreamMarc.Mid(nCurDirAddr, 12);		
		if (strDir.GetLength() < 3 + 4) return -1;	
		nCurDirAddr += 12;
		
		sTagDirList.Add(strDir);		
	}
	
	CString strDataField;
	CString strField;
	strDataField = strStreamMarc.Mid(nCurDirAddr+1);
	
	sTagFieldList.RemoveAll();
	TCHAR szRT;

	szRT = 0x001E;
	INT offset = 0;
	INT nPos = 0;

	//=================================
	//20080603 ADD BY PJW : NULL인 필드가 있을경우 예외처리를 넣는다.

	//=================================
	

	while ( TRUE )
	{
		offset = strDataField.Find(szRT, nPos);
		//=================================
		//20080603 UPDATE BY PJW : NULL인 필드가 있을경우 예외처리를 넣는다.-szRT가 연속두개 존재
		//if ( offset > 0 )
		//=================================
		if ( offset >= 0 )
		{
			strField = strDataField.Mid(nPos, offset-nPos);
		}
		else
		{
			// 마지막 Field
			strField = strDataField.Mid(nPos);
			sTagFieldList.Add(strField);
			break;
		}
		sTagFieldList.Add(strField);
		nPos = offset+1;
	}

	CString strTagCode;	
	CString strSrc;
	CString strDesc;
	INT nIdx;
	
	for ( i =  0 ; i < sTagDirList.GetSize() ; i ++ )
	{
		nIdx = 0;				

		strSrc = sTagDirList.GetAt(i).Mid(7,5); 
		for ( j = 0 ; j < sTagDirList.GetSize() ; j ++ )
		{
			strDesc = sTagDirList.GetAt(j).Mid(7,5);
			if ( _ttoi(strSrc) > _ttoi(strDesc) )
			{
				nIdx++;				
			}
		}

		strTagCode = sTagDirList.GetAt(i).Left(3);
		if ( nIdx < sTagFieldList.GetSize() ) strField = sTagFieldList.GetAt(nIdx);
		else strField = _T("");

		// add tag to marc
		CTag *pTag = new CTag;
		if (pTag == NULL) continue;

		ids = FetchField(pTag, strTagCode, strField);
		if (ids < 0) 
		{
			delete pTag;
			pTag = NULL;
		}
		else
		{
			pMarc->AddTag(pTag);
		}
	}

	pMarc->SetRecordTerminator(RECORD_TERMINATOR);
	/*
	// Directory
	INT nDirInitAddr = 24;									// Init address of directory

	if (strStreamMarc.GetLength() < 12 + 5) return -1;
	INT nDataBaseAddr = _ttoi(strStreamMarc.Mid(12, 5));		// Init address of data
	INT nDirCnt = (nDataBaseAddr - nDirInitAddr - 1) / 12;	// Count of directory

	// Directory fetch
	INT nCurDirAddr = nDirInitAddr;
	INT nDirFieldLength;			// Field length
	INT nDirFieldStartAddr;			// Field Start Address
	
	CString strDir;					// One directory
	CString strTagCode;
	CString strField;
			
	for (INT i = 0; i < nDirCnt; i++)
	{
		if (strStreamMarc.GetLength() < nCurDirAddr + 12) return -1;
		strDir = strStreamMarc.Mid(nCurDirAddr, 12);

		strTagCode = strDir.Left(3);

		if (strDir.GetLength() < 3 + 4) return -1;
		nDirFieldLength = _ttoi(strDir.Mid(3, 4));
		nDirFieldStartAddr = nDataBaseAddr + _ttoi(strDir.Right(5));

		// get field
		if (strStreamMarc.GetLength() < nDirFieldStartAddr + nDirFieldLength) return -1;
		strField = strStreamMarc.Mid(nDirFieldStartAddr, nDirFieldLength);

		// add tag to marc
		CTag *pTag = new CTag;
		if (pTag == NULL) continue;

		ids = FetchField(pTag, strTagCode, strField);
		if (ids < 0) 
		{
			delete pTag;
			pTag = NULL;
		}
		else
		{
			pMarc->AddTag(pTag);
		}
		
		nCurDirAddr += 12;
	}

	// add record terminator
	pMarc->SetRecordTerminator(RECORD_TERMINATOR);
	*/
	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::FetchField(CTag *pTag, CString strTagCode, CString strField)
{
	EFS_BEGIN

	//CString sFtTer;
	//sFtTer.Format( _T("%c"), 30 );			

	// check param
	if (pTag == NULL)
	{
		AddErrMsg(_T(""), _T("pTag Is Empty"),_T(""));
		return -1;
	}
	if (strTagCode.IsEmpty())
	{
		AddErrMsg(_T(""), _T("strTagCode Is Empty"),_T(""));
		return -1;
	}
	if (strField.IsEmpty()) 
	{
		AddErrMsg(_T(""), _T("strField Is Empty"),_T(""));
		return -1;
	}

	CString strFieldTerminatorCode;	
	strFieldTerminatorCode.Format(_T("%c"), FIELD_TERMINATOR);	

	if (strField.Right(1) != strFieldTerminatorCode)
		strField += strFieldTerminatorCode;

	// Set Tag Code	
	pTag->SetTagCode(strTagCode);
	

	// 00X control field
	if (IsControlField(strTagCode))
	{
		pTag->SetItemData(strField.Left(strField.GetLength() - 1));
		pTag->SetFieldTerminator(strField.Right(1));
	}
	// Other field
	else
	{
		// check length
		if (strField.GetLength() < 3) 
		{
			AddErrMsg(_T(""), _T("strField length is short"),_T(""));
			return -1;
		}

		// Indicator
		pTag->SetFirstIndicator(strField.GetAt(0));
		pTag->SetSecondIndicator(strField.GetAt(1));

		// Field terminator
		pTag->SetFieldTerminator(strField.Right(1));

		// fetch subfield		
		CSubfield *pSubfield = NULL;
		CSubfield *pOldSubfield = NULL;
		CString strPunctuationMark;
		CString strSubfield;
		INT nStart = strField.Find(SUBFIELD_CODE, 2);
		while (nStart > 0)
		{
			// Find next subfield code
			INT nEnd = strField.Find(SUBFIELD_CODE, nStart + 1);
			if (nEnd == -1)
			{
				// find field terminate code
				nEnd = strField.Find(FIELD_TERMINATOR, nStart + 1);
				if (nEnd == -1)
					break;
			}					
			
			pSubfield = new CSubfield;
			if (!pSubfield) continue;
			
			strSubfield = strField.Mid(nStart, nEnd - nStart);		
			strSubfield.TrimLeft();
			strSubfield.TrimRight();
			
			// length check
			if (strSubfield.GetLength() < 2) 
			{
				AddErrMsg(_T(""), _T("strSubfield lenth is short"),_T(""));
				return -1;
			}
			
			// add subfield code
			pSubfield->SetSubfieldCode(strSubfield.Left(2));
			
			// add punctuation mark
			if (!strPunctuationMark.IsEmpty())
			{
				if (IsPunctuationMark(strTagCode, strSubfield + strPunctuationMark))
					pSubfield->SetPunctuationMark(strPunctuationMark);
				else if (pOldSubfield)
					pOldSubfield->SetItemData(pOldSubfield->GetItemData() + strPunctuationMark);
			}

			// add Itemdata
			if (IsPunctuationMark(strSubfield.Right(1)) && nEnd < strField.GetLength() - 1)
			//if (IsPunctuationMark(strTagCode, strSubfield))
			{
				strPunctuationMark = strSubfield.Right(1);
				pSubfield->SetItemData(strSubfield.Mid(2, strSubfield.GetLength() -3));
			}
			else
			{
				strPunctuationMark.Empty();
				pSubfield->SetItemData(strSubfield.Mid(2));
			}			

			//if ( pSubfield->GetSubfieldCode().Left(1) != sFtTer ) 
				pTag->AddSubfield(pSubfield);
			pOldSubfield = pSubfield;
			nStart = nEnd;			
		}
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::GetMarcFromEditMarc(CMarc *pMarc, CString strEditMarcText)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;
	pMarc->Init();

	// find field terminator
	strEditMarcText.TrimLeft();
	strEditMarcText.TrimRight();

	CString strField;
	INT nStart = 0;
	INT nEnd;

	nEnd = strEditMarcText.Find(FIELD_TERMINATOR);
	while (nEnd > 0)
	{
		strField = strEditMarcText.Mid(nStart, nEnd - nStart + 1);
		strField.TrimLeft();
		strField.TrimRight();

		// Check Length
		if (strField.GetLength() < 3) continue;
		
		// add tag
		CTag *pTag = NULL;
		CString strTagCode = strField.Left(3);
		
		// tag is not limit tag  --> create new tag and parse edit text and create new subfields
		if (pTag == NULL)
		{
			pTag = new CTag;
			if (pTag == NULL) continue;

			INT ids = FetchField(pTag, strTagCode, strField.Mid(3));
			if (ids < 0)
			{
				delete pTag;
				pTag = NULL;
				continue;
			}
		}

		pMarc->AddTag(pTag);

		nStart = nEnd + 1;
		nEnd = strEditMarcText.Find(FIELD_TERMINATOR, nStart);
		if (nEnd < 0)
		{
			nEnd = strEditMarcText.Find(RECORD_TERMINATOR, nStart);
			if (nEnd < 0)
			{
				nEnd = strEditMarcText.GetLength();
				strEditMarcText.Insert(nEnd, RECORD_TERMINATOR);
				strEditMarcText.Insert(nEnd + 1, RECORD_TERMINATOR);
			}
			else 
			{
				if (strEditMarcText.GetAt(nEnd - 1) == FIELD_TERMINATOR)
					break;
				else
					strEditMarcText.Insert(nEnd, FIELD_TERMINATOR);		
			}
		}
	}
	
	// record terminator --> auto ?
	pMarc->SetRecordTerminator(RECORD_TERMINATOR);

	SortMarc(pMarc);

	return 0;

	EFS_END
	return -1;
}

BOOL CMarcMgr::IsControlField(CString strTagCode)
{
	EFS_BEGIN

	INT nTag = _ttoi(strTagCode);
	if (nTag < 10)
		return TRUE;
	else 
		return FALSE;
	/*
	if (strTagCode == _T("001") ||
		strTagCode == _T("005")	||
		strTagCode == _T("006") ||
		strTagCode == _T("007") ||
		strTagCode == _T("008")
		)
		return TRUE;
	else
		return FALSE;
		*/

	EFS_END
	return FALSE;
}

BOOL CMarcMgr::IsPunctuationMark(CString strPunctuationMark)
{
	EFS_BEGIN

	if (strPunctuationMark == _T("=") ||
		strPunctuationMark == _T(":") ||
		strPunctuationMark == _T("/") ||
		strPunctuationMark == _T(";") ||
		strPunctuationMark == _T(".") ||
		strPunctuationMark == _T(",") ||
		strPunctuationMark == _T("+") 		
		)
		return TRUE;
	else
		return FALSE;

	EFS_END
	return FALSE;
}

BOOL CMarcMgr::IsPunctuationMark(CString strTagCode, CString strSubfield)
{
	EFS_BEGIN

	CString strPunctuationMark = strSubfield.Right(1);
	strSubfield.Replace(SUBFIELD_CODE, '$');
	if (strPunctuationMark.IsEmpty()) return FALSE;

	if (strPunctuationMark == _T("=") ||
		strPunctuationMark == _T(":") ||
		strPunctuationMark == _T("/") ||
		strPunctuationMark == _T(";") ||
		strPunctuationMark == _T(".") ||
		strPunctuationMark == _T(",") ||
		strPunctuationMark == _T("+") 		
		)
	{
		CString strResult;
		if (CheckSimpleSyntax(strPunctuationMark + strTagCode + strSubfield.Left(2), CHK_PUNCTUATION_MARK, strResult, FALSE) < 0)
			return FALSE;
		else 
			return TRUE;
	}
	else
		return FALSE;

	EFS_END
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Check syntax
//////////////////////////////////////////////////////////////////////

INT CMarcMgr::CheckSimpleSyntax(CMarc *pMarc)
{
	EFS_BEGIN

	if (!m_bUseRelation) return 0;

	FreeErrMsgList();
	
	INT ids = -1;
	BOOL bIsError = FALSE;
	
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	
	CString strResult;
	CString strTagCode;
	CString strIndicator;
	CString strSubfieldCode;
	CString strPunctuationMark;

	POSITION posTag = pMarc->m_listTag.GetHeadPosition();
	while (posTag != NULL)
	{
		pTag = (CTag*)pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		// 태그 코드		
		strTagCode = pTag->GetTagCode();
		
		if ( strTagCode == _T("245") ) {
			//continue;
			INT iTmp = 1;
		}
		if ( strTagCode == _T("950") ) {
			//continue;
			INT iTmp = 1;
		}
		
		ids = CheckSimpleSyntax(strTagCode, CHK_TAG, strResult, FALSE);
		if (ids < 0) bIsError = TRUE;

		// 지시기호
		strIndicator = pTag->GetFirstIndicator() + pTag->GetSecondIndicator();
		strIndicator.Replace(' ', '_');
		ids = CheckSimpleSyntax(strTagCode + strIndicator, CHK_INDICATOR, strResult, FALSE);
		if (ids < 0)
		{
			// 디폴트 지시기호를 넣어준다.
			CMarcSimpleRelation *pMarcSimpleRelation = NULL;
			CMarcInstruction marcInstruction;
			pMarcSimpleRelation = m_marcRelation.GetSimpleMarcRelation(m_strGroupName);
			if (pMarcSimpleRelation != NULL)
			{
				if (pMarcSimpleRelation->m_mapDefaultIndicator.Lookup(strTagCode, marcInstruction))
				{
					CString strTemp;
					strTemp.Format(_T("%s"), marcInstruction.m_pszParam);
					strTemp.Replace('_', ' ');
					
					pTag->SetFirstIndicator(strTemp.Left(1));
					pTag->SetSecondIndicator(strTemp.Right(1));
				}
				else
					bIsError = TRUE;
			}
			else
				bIsError = TRUE;
		}
		
		INT nCurPos = 0;
		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield != NULL)
		{
			strResult.Empty();

			pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//2006.12.01 HJY : 레코드 종단기호 제거
			CString sData, sRT;
			sData = pSubfield->GetItemData();
			sRT.Format(_T("%c"),RECORD_TERMINATOR);
			if( 0 <= sData.Find(sRT) )
			{
				sData.Replace(sRT,_T(""));
				pSubfield->SetItemData(sData);
			}


			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// 식별기호
			strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');

			CString sFtTer;
			sFtTer.Format( _T("%c%c"), 30, 29 );			
			if ( strSubfieldCode == sFtTer) {
				continue;
			}
			sFtTer.Format( _T("$c"), 30 );
			if ( strTagCode == _T("056") && strSubfieldCode == sFtTer ) {
				continue;
			}

			ids = CheckSimpleSyntax(strTagCode + strSubfieldCode, CHK_SUBFIELD, strResult, FALSE);
			if (ids < 0) bIsError = TRUE;
			else
			{
				// 구두점
				strPunctuationMark = pSubfield->GetPunctuationMark();
				if (strPunctuationMark.IsEmpty()) strPunctuationMark = _T("_");

				if ( strTagCode == _T("245") && strSubfieldCode == _T("$a") ) {
					INT iTmp = 1;
				}
				if ( strTagCode == _T("440") ) {
					INT iTmp = 1;
				}
				ids = CheckSimpleSyntax(strPunctuationMark + strTagCode + strSubfieldCode, CHK_PUNCTUATION_MARK, strResult, FALSE);
				if (ids < 0) 
				{
					if (nCurPos != 0)
					{
						// 자동고침 -> 기본 구두점을 붙인다.
						if (strPunctuationMark == _T("_"))
						{
							pSubfield->SetPunctuationMark(strResult.Left(1));
						}
						else
							bIsError = TRUE;
					}
				}
			}

			nCurPos++;
		}

		// 반복 식별기호
		INT nSubfieldCount = pTag->m_listSubfield.GetCount();  
		for (INT i = 0; i < nSubfieldCount - 1 ; i++)
		{
			pSubfield = (CSubfield*)pTag->m_listSubfield.GetAt(pTag->m_listSubfield.FindIndex(i));
			if (pSubfield == NULL) continue;

			for (INT j = i + 1; j < nSubfieldCount; j++)
			{
				CSubfield *pSubfieldSlave = (CSubfield*)pTag->m_listSubfield.GetAt(pTag->m_listSubfield.FindIndex(j));
				if (pSubfieldSlave == NULL) continue;
				
				if (pSubfield->GetSubfieldCode() == pSubfieldSlave->GetSubfieldCode())
				{
					strSubfieldCode = pSubfield->GetSubfieldCode();
					strSubfieldCode.Replace(SUBFIELD_CODE, '$');
					ids = CheckSimpleSyntax(strTagCode + strSubfieldCode, CHK_REPEAT_SUBFIELD, strResult, FALSE);
					if (ids < 0) bIsError = TRUE;
				}				
			}
		}
	}

	// 반복 태그코드
	INT nTagCount = pMarc->m_listTag.GetCount();
	for (INT i = 0; i < nTagCount - 1; i++)
	{
		pTag = (CTag*)pMarc->m_listTag.GetAt(pMarc->m_listTag.FindIndex(i));
		if (pTag == NULL) continue;

		for (INT j = i + 1; j < nTagCount; j++)
		{
			CTag *pTagSlave = (CTag*)pMarc->m_listTag.GetAt(pMarc->m_listTag.FindIndex(j));
			if (pTagSlave == NULL) continue;
	
			if (pTag->GetTagCode() == pTagSlave->GetTagCode())
			{
				ids = CheckSimpleSyntax(pTag->GetTagCode(), CHK_REPEAT_TAG, strResult, FALSE);
				if (ids < 0) bIsError = TRUE;
			}

		}
	}

	if (bIsError)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::CheckSimpleSyntax(CString strKeyValue, INT nCheckMode, CString &strResult, BOOL bFreeErrMsgList /*= TRUE*/)
{
	EFS_BEGIN

	if (!m_bUseRelation) return 0;

	if (bFreeErrMsgList)
		FreeErrMsgList();

	if (m_strGroupName.IsEmpty())
	{
		AddErrMsg(_T(""), _T("그룹 이름이 없습니다."), _T(""));
		return -1;
	}

	CMarcSimpleRelation *pMarcSimpleRelation;
	CMarcInstruction marcInstruction;
	pMarcSimpleRelation = m_marcRelation.GetSimpleMarcRelation(m_strGroupName);

	if (pMarcSimpleRelation == NULL)
	{
		AddErrMsg(_T(""), _T("Invalid group name for simple relation"),_T("Simple Relation을 지정하는 마크 그룹의 이름이 잘못되었습니다."));
		return -1;
	}

	switch (nCheckMode)
	{
	case CHK_TAG:
		if (!pMarcSimpleRelation->m_mapTag.Lookup(strKeyValue, marcInstruction))
		{
			AddErrMsg(strKeyValue, _T("없는 태그"),_T("없는 태그기호를 사용하셨습니다."));
			return -1;
		}
		/*else
		{
			strResult.Format(_T("%s"), marcInstruction.m_pszDesc);
		}
		*/
		break;

	case CHK_REPEAT_TAG:
		if (!pMarcSimpleRelation->m_mapRepeatTag.Lookup(strKeyValue, marcInstruction))
		{
			AddErrMsg(strKeyValue, _T("중복 태그"), _T("반복이 되지 않는 태그를 반복해서 사용하셨습니다."));
			return -1;
		}
		break;

	case CHK_SUBFIELD:
		if (!pMarcSimpleRelation->m_mapSubfiled.Lookup(strKeyValue, marcInstruction))
		{
			AddErrMsg(strKeyValue, _T("없는 식별기호"),_T("없는 식별기호를 사용하셨습니다."));
			return -1;
		}
		break;

	case CHK_INDICATOR:
		if (!pMarcSimpleRelation->m_mapIndicator.Lookup(strKeyValue, marcInstruction))
		{
			AddErrMsg(strKeyValue, _T("없는 지시기호"),_T("없는 지시기호를 사용하셨습니다."));
			return -1;
		}
		break;

	case CHK_REPEAT_SUBFIELD:
		if (!pMarcSimpleRelation->m_mapRepeatSubfield.Lookup(strKeyValue, marcInstruction))
		{
			AddErrMsg(strKeyValue, _T("반복불가 식별기호"),_T("반복이 되지 않는 식별기호를 반복해서 사용하셨습니다."));
			return -1;
		}
		break;

	case CHK_PUNCTUATION_MARK:
		if (!pMarcSimpleRelation->m_mapPunctuationMark.Lookup(strKeyValue, marcInstruction))
		{
			if (pMarcSimpleRelation->m_mapPunctuationMark.Lookup(strKeyValue.Mid(1), marcInstruction))
			{			
				strResult.Format(_T("%s"), marcInstruction.m_pszDesc);
				strResult.Replace('|', ' ');
			}
			/*
			AddErrMsg(strKeyValue, _T("잘못된 구두점"), strResult);
			*/
			
			return -1;
		}
		break;

	default:
		return -1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CMarcMgr::CheckDeepSyntax(CMarc *pMarc)
{
	EFS_BEGIN

	if (!m_bUseRelation) return 0;
	
	FreeErrMsgList();
	
	if (m_strGroupName.IsEmpty())
	{
		AddErrMsg(_T(""), _T("그룹 이름이 없습니다."), _T(""));
		return -1;
	}

	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("Marc pointer is null"), _T("Marc이 가리키는 포인터가 널값입니다."));
		return -1;
	}

	INT ids;
	CMarcOperation marcOperation;
	CMarcInstruction *pMarcInstruction;
	POSITION pos = m_marcRelation.m_listDeepRelation.GetHeadPosition();
	while (pos != NULL)
	{
		pMarcInstruction = (CMarcInstruction*)m_marcRelation.m_listDeepRelation.GetNext(pos);
		if (pMarcInstruction == NULL) continue;
		
		//if (pMarcInstruction->m_strGroupName != m_strGroupName) continue;
		if (_tcscmp(pMarcInstruction->m_pszGroupName, m_strGroupName.GetBuffer(0)) != 0) continue;
		
		ids = marcOperation.ExecuteOperation(this, pMarc, pMarcInstruction);
		if (ids < 0)
		{
			CString strTagCode;
			strTagCode.Format(_T("%s"), pMarcInstruction->m_pszBasicParam);
			strTagCode = strTagCode.Left(3);
			AddErrMsg(strTagCode, marcOperation.GetLastError(), marcOperation.GetLastHelp());
			//AddErrMsg(pMarcInstruction->m_strBasicParam.Left(3), marcOperation.GetLastError(), marcOperation.GetLastHelp());
		}
	}

	if (m_listErrMsg.GetCount() > 0) return -1;
	else return 0;

	EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////
// Set / Get / Delete Marc Item
//////////////////////////////////////////////////////////////////////
INT CMarcMgr::FetchMarcAlias(CString strAlias, CString &strAliasType, CString &strTagCode, CString &strData)
{	
	EFS_BEGIN

	CString strKeyValue;

	// whatever stralias : alias or key value
	if (m_mapMarcAlias.Lookup(strAlias, strKeyValue))
		return FetchKeyValue(strKeyValue, strAliasType, strTagCode, strData);
	else
		return FetchKeyValue(strAlias, strAliasType, strTagCode, strData);

	EFS_END
	return -1;
}

INT CMarcMgr::FetchKeyValue(CString strKeyValue, CString &strAliasType, CString &strTagCode, CString &strData)
{
	EFS_BEGIN

	// Fetch
	INT nStart;
	
	// *
	if ((nStart = strKeyValue.Find(_T("*"))) > 0)
	{
		strAliasType = _T("*");
		strTagCode = strKeyValue.Mid(0, nStart);
		strData = strKeyValue.Mid(nStart + 1);
	}
	// $
	else if ((nStart = strKeyValue.Find(_T("$"))) > 0)
	{
		strAliasType = _T("$");
		strTagCode = strKeyValue.Mid(0, nStart);
		strData = strKeyValue.Mid(nStart + 1);
	}
	// @
	else  if ((nStart = strKeyValue.Find(_T("@"))) > 0)
	{
		strAliasType = _T("@");
		strTagCode = strKeyValue.Mid(0, nStart);
		strData = strKeyValue.Mid(nStart + 1);
	}
	else
	{
		AddErrMsg(_T(""), _T("Can't find matched alias type"), _T(""));
		return -1;
	}

	return 0;

	EFS_END
	return -1;
}

BOOL CMarcMgr::GetKeyValueFromAlias(CString strAlias, CString &strKeyValue)
{
	EFS_BEGIN

	return m_mapMarcAlias.Lookup(strAlias, strKeyValue);

	EFS_END
	return FALSE;
}

INT CMarcMgr::SetTagIndicator(CMarc *pMarc, CString strTagCode, CString strFirstIndicator, CString strSecondIndicator)
{
	EFS_BEGIN

	if (strTagCode.IsEmpty()) return -1;
	if (strFirstIndicator.IsEmpty()) return -1;
	if (strSecondIndicator.IsEmpty()) return -1;

	POSITION pos = NULL;
	CTag *pTag = NULL;
	
	do
	{
		pTag = pMarc->FindTag(strTagCode, pos);
		if (pTag == NULL) continue;

		pTag->SetFirstIndicator(strFirstIndicator);
		pTag->SetSecondIndicator(strSecondIndicator);
	}
	while (pos != NULL);

	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::SetItem(CMarc* pMarc, CString strAlias, CString strNewItemData, CString strOldItemData /*= _T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN

	FreeErrMsgList();
	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	CString strAliasType;
	CString strTagCode;
	CString strData;

	if (FetchMarcAlias(strAlias, strAliasType, strTagCode, strData) < 0) return -1;

	return SetItem(pMarc, strAliasType, strTagCode, strData, strNewItemData, strOldItemData, nSetMode);

	EFS_END
	return -1;
}

INT CMarcMgr::SetItem(CMarc* pMarc, CString strAliasType, CString strTagCode, CString strData, CString strNewItemData, CString strOldItemData /*= _T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN

	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	if (nSetMode == ADD_DEFAULT)
		nSetMode = DetermineSetMode(strTagCode, strData);

	// 2005.11.05 ADD BY PDJ
	// 505 TAG 특성상 반복이라 할지라도 $n, $t, $d에 대해서는 tag반복을 하지 않는다.
	if ( strTagCode == _T("505") && ( strData == _T("n") || strData == _T("t") || strData == _T("d") || strData == _T("e")) )
	{
		nSetMode = 2;
	}
	
	if (strAliasType == _T("*"))
	{
		return pMarc->SetItemData(strTagCode, strNewItemData, strOldItemData, nSetMode);
	}
	else if (strAliasType == _T("$"))
	{
		TCHAR chInd[2];
		chInd[0] = ' ';
		chInd[1] = ' ';
		
		INT nFind = strData.Find('!');
		if (nFind > 0)
		{
			if (strData.GetLength() == nFind + 3)
			{
				chInd[0] = strData.GetAt(nFind + 1);
				if (chInd[0] == '_') chInd[0] = ' ';

				chInd[1] = strData.GetAt(nFind + 2);
				if (chInd[1] == '_') chInd[1] = ' ';
			}

			strData = strData.Left(nFind);
		}
		else
		{
			if (!m_strGroupName.IsEmpty())			
			{
				CMarcSimpleRelation *pMarcSimpleRelation;
				CMarcInstruction marcInstruction;	
				pMarcSimpleRelation = m_marcRelation.GetSimpleMarcRelation(m_strGroupName);
				if (pMarcSimpleRelation != NULL)
				{
					if (pMarcSimpleRelation->m_mapDefaultIndicator.Lookup(strTagCode, marcInstruction))
					{
						CString strTemp;
						strTemp.Format(_T("%s"), marcInstruction.m_pszParam);
						strTemp.Replace('_', ' ');
						chInd[0] = strTemp.GetAt(0);
						chInd[1] = strTemp.GetAt(1);
					}					
				}
			}
		}

		return pMarc->SetItemData(strTagCode, chInd, strData, strNewItemData, strOldItemData, nSetMode);
	}
	else if (strAliasType == _T("@"))
	{
		INT nStart = 0;
		INT nEnd = 0;
		if (strData.Find(_T("-")) > 0)
		{
			nStart = _ttoi(strData.Mid(0, strData.Find(_T("-"))));
			nEnd = _ttoi(strData.Mid(strData.Find(_T("-")) + 1));
		}
		else
		{
			nStart = nEnd = _ttoi(strData);
		}

		return pMarc->SetItemData(strTagCode, nStart, nEnd, strNewItemData, strOldItemData, nSetMode);
	}
	else
	{
		AddErrMsg(_T(""), pMarc->GetLastError(), _T(""));
		return -1;
	}

	EFS_END
	return -1;
}


INT CMarcMgr::SetItemWithFunc(CMarc* pMarc, CString strAlias, CString strNewItemData, CString strPunc, CString strOldItemData /*= _T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN

	FreeErrMsgList();
	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	CString strAliasType;
	CString strTagCode;
	CString strData;

	if (FetchMarcAlias(strAlias, strAliasType, strTagCode, strData) < 0) return -1;

	return SetItemWithFunc(pMarc, strAliasType, strTagCode, strData, strNewItemData, strPunc, strOldItemData, nSetMode);

	EFS_END
	return -1;
}

INT CMarcMgr::SetItemWithFunc(CMarc* pMarc, CString strAliasType, CString strTagCode, CString strData, CString strNewItemData, CString strPunc, CString strOldItemData /*= _T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN

	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	if (nSetMode == ADD_DEFAULT)
		nSetMode = DetermineSetMode(strTagCode, strData);

	// 2005.11.05 ADD BY PDJ
	// 505 TAG 특성상 반복이라 할지라도 $n, $t, $d에 대해서는 tag반복을 하지 않는다.
	if ( strTagCode == _T("505") && ( strData == _T("n") || strData == _T("t") || strData == _T("d") || strData == _T("e")) )
	{
		nSetMode = 2;
	}
	
	if (strAliasType == _T("*"))
	{
		return pMarc->SetItemData(strTagCode, strNewItemData, strOldItemData, nSetMode);
	}
	else if (strAliasType == _T("$"))
	{
		TCHAR chInd[2];
		chInd[0] = ' ';
		chInd[1] = ' ';
		
		INT nFind = strData.Find('!');
		if (nFind > 0)
		{
			if (strData.GetLength() == nFind + 3)
			{
				chInd[0] = strData.GetAt(nFind + 1);
				if (chInd[0] == '_') chInd[0] = ' ';

				chInd[1] = strData.GetAt(nFind + 2);
				if (chInd[1] == '_') chInd[1] = ' ';
			}

			strData = strData.Left(nFind);
		}
		else
		{
			if (!m_strGroupName.IsEmpty())			
			{
				CMarcSimpleRelation *pMarcSimpleRelation;
				CMarcInstruction marcInstruction;	
				pMarcSimpleRelation = m_marcRelation.GetSimpleMarcRelation(m_strGroupName);
				if (pMarcSimpleRelation != NULL)
				{
					if (pMarcSimpleRelation->m_mapDefaultIndicator.Lookup(strTagCode, marcInstruction))
					{
						CString strTemp;
						strTemp.Format(_T("%s"), marcInstruction.m_pszParam);
						strTemp.Replace('_', ' ');
						chInd[0] = strTemp.GetAt(0);
						chInd[1] = strTemp.GetAt(1);
					}					
				}
			}
		}

		return pMarc->SetItemDataWithFunc(strTagCode, chInd, strData, strNewItemData, strPunc, strOldItemData, nSetMode);
	}
	else if (strAliasType == _T("@"))
	{
		INT nStart = 0;
		INT nEnd = 0;
		if (strData.Find(_T("-")) > 0)
		{
			nStart = _ttoi(strData.Mid(0, strData.Find(_T("-"))));
			nEnd = _ttoi(strData.Mid(strData.Find(_T("-")) + 1));
		}
		else
		{
			nStart = nEnd = _ttoi(strData);
		}

		return pMarc->SetItemData(strTagCode, nStart, nEnd, strNewItemData, strOldItemData, nSetMode);
	}
	else
	{
		AddErrMsg(_T(""), pMarc->GetLastError(), _T(""));
		return -1;
	}

	EFS_END
	return -1;
}

INT CMarcMgr::DetermineSetMode(CString strTagCode, CString strSubfield)
{
	EFS_BEGIN

	INT nFind = strSubfield.Find(_T("!"));
	if (nFind > 0)
	{
		strSubfield = strSubfield.Mid(0, nFind);
	}
	if (m_strGroupName.IsEmpty()) return ADD_DEFAULT;
	
	CMarcSimpleRelation *pMarcSimpleRelation = m_marcRelation.GetSimpleMarcRelation(m_strGroupName);
	if (pMarcSimpleRelation == NULL) return ADD_DEFAULT;

	CMarcInstruction marcInstruction;
	if (pMarcSimpleRelation->m_mapRepeatTag.Lookup(strTagCode, marcInstruction))
		return ADD_TAG;
	else if (pMarcSimpleRelation->m_mapRepeatSubfield.Lookup(strTagCode + _T("$") + strSubfield, marcInstruction))
		return ADD_SUBFIELD;

	return ADD_DEFAULT;

	EFS_END
	return ADD_DEFAULT;
}

INT CMarcMgr::GetItem(CMarc* pMarc, CString strAlias, CString &strItemData, CArray<CString, CString&> *pArray /*= NULL */, CArray<CString, CString&> *pIndArray /*= NULL*/)
{
	EFS_BEGIN

	strItemData.Empty();
	if (pArray) pArray->RemoveAll();
	if (pIndArray) pIndArray->RemoveAll();

	FreeErrMsgList();
	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	CString strAliasType;
	CString strTagCode;
	CString strData;

	if (FetchMarcAlias(strAlias, strAliasType, strTagCode, strData) < 0) return -1;
	
	return GetItem(pMarc, strAliasType, strTagCode, strData, strItemData, pArray, pIndArray);

	EFS_END
	return -1;
}

INT CMarcMgr::GetItem(CMarc* pMarc, CString strAliasType, CString strTagCode, CString strData, CString &strItemData, CArray<CString, CString&> *pArray /*= NULL */, CArray<CString, CString&> *pIndArray /*= NULL*/)
{
	EFS_BEGIN

	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	if (strAliasType == _T("*"))
		return pMarc->GetItemData(strTagCode, strItemData, pArray);
	else if (strAliasType == _T("$"))
		return pMarc->GetItemData(strTagCode, strData, strItemData, pArray, pIndArray);
	else if (strAliasType == _T("@"))
	{
		INT nStart = 0;
		INT nEnd = 0;
		if (strData.Find(_T("-")) > 0)
		{
			nStart = _ttoi(strData.Mid(0, strData.Find(_T("-"))));
			nEnd = _ttoi(strData.Mid(strData.Find(_T("-")) + 1));
		}
		else
		{
			nStart = nEnd = _ttoi(strData);
		}

		return pMarc->GetItemData(strTagCode, nStart, nEnd, strItemData, pArray);
	}
	return -1;

	EFS_END
	return -1;
}

INT CMarcMgr::DeleteItem(CMarc* pMarc, CString strAlias, CString strItemData /*= _T("")*/)
{
	EFS_BEGIN

	FreeErrMsgList();
	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	CString strAliasType;
	CString strTagCode;
	CString strData;

	if (FetchMarcAlias(strAlias, strAliasType, strTagCode, strData) < 0) return -1;

	return DeleteItem(pMarc, strAliasType, strTagCode, strData, strItemData);

	EFS_END
	return -1;
}
INT CMarcMgr::DeleteItem(CMarc* pMarc, CString strAliasType, CString strTagCode, CString strData, CString strItemData /*= _T("")*/)
{
	EFS_BEGIN

	if (pMarc == NULL) 
	{
		AddErrMsg(_T(""), _T("Marc is NULL"), _T(""));
		return -1;
	}

	if (strAliasType == _T("*"))
		return pMarc->DeleteItemData(strTagCode, strItemData);
	else if (strAliasType == _T("$"))
		return pMarc->DeleteItemData(strTagCode, strData, strItemData);
	else if (strAliasType == _T("@"))
	{
		INT nStart = 0;
		INT nEnd = 0;
		if (strData.Find(_T("-")) > 0)
		{
			nStart = _ttoi(strData.Mid(0, strData.Find(_T("-"))));
			nEnd = _ttoi(strData.Mid(strData.Find(_T("-")) + 1));
		}
		else
		{
			nStart = nEnd = _ttoi(strData);
		}

		return pMarc->DeleteItemData(strTagCode, nStart, nEnd, strItemData);
	}
	return -1;

	EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////
// Field Operation
//////////////////////////////////////////////////////////////////////
INT CMarcMgr::GetField(CMarc *pMarc, CString strTagCode, CString &strField, CArray<CString, CString&> *pArray /*= NULL*/)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;
	if (strTagCode.IsEmpty()) return -1;

	return pMarc->GetField(strTagCode, strField, pArray);	

	EFS_END
	return -1;
}

INT CMarcMgr::InsertField(CMarc *pMarc, CString strField)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;
	if (strField.IsEmpty()) return -1;

	//strField.Replace('$', SUBFIELD_CODE);
	CString strFieldTerminatorCode;	
	strFieldTerminatorCode.Format(_T("%c"), FIELD_TERMINATOR);	

	if (strField.Right(1) != strFieldTerminatorCode)
		strField += strFieldTerminatorCode;

	CTag *pTag = new CTag;
	if (pTag == NULL) return -1;

	INT ids = FetchField(pTag, strField.Left(3), strField.Mid(3));
	if (ids < 0)
	{
		delete pTag;
		pTag = NULL;
		return -1;
	}

	pMarc->AddTag(pTag);

	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::UpdateField(CMarc *pMarc, CString strOldField, CString strNewField)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;
	if (strNewField.IsEmpty() || strOldField.IsEmpty()) return -1;

	strNewField.Replace('$', SUBFIELD_CODE);
	strOldField.Replace('$', SUBFIELD_CODE);

	CString strFieldTerminatorCode;	
	strFieldTerminatorCode.Format(_T("%c"), FIELD_TERMINATOR);	

	if (strNewField.Right(1) != strFieldTerminatorCode)
		strNewField  += strFieldTerminatorCode;

	INT ids = -1;
	
	ids = DeleteField(pMarc, strOldField.Left(3), strOldField.Mid(3));
	if (ids < 0) return -1;

	ids = InsertField(pMarc, strNewField);
	if (ids < 0) return -1;

	return 0;

	EFS_END
	return -1;
}

INT CMarcMgr::DeleteField(CMarc *pMarc, CString strTagCode, CString strTagData /*= _T("")*/)
{
	EFS_BEGIN

	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("pMarc Is Null"), _T(""));
		return -1;
	}

	CString strFieldTerminatorCode;	
	strFieldTerminatorCode.Format(_T("%c"), FIELD_TERMINATOR);		

	if (!strTagData.IsEmpty())
	{
		strTagData.Replace('$', SUBFIELD_CODE);
		if (strTagData.Right(1) != strFieldTerminatorCode)
			strTagData += strFieldTerminatorCode;
	}

	return pMarc->DeleteField(strTagCode, strTagData);	

	EFS_END
	return -1;
}

INT CMarcMgr::DeleteField(CMarc *pMarc, CString strTagCode, TCHAR chFirstIndicator, TCHAR chSecondIndicator)
{
	EFS_BEGIN

	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("pMarc Is Null"), _T(""));
		return -1;
	}

	if (strTagCode.GetLength() < 3) return -1;

	return pMarc->DeleteField(strTagCode, chFirstIndicator, chSecondIndicator);

	EFS_END
	return -1;
}

INT CMarcMgr::DeleteEmptyField(CMarc *pMarc)
{
	EFS_BEGIN

	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("pMarc Is Null"), _T(""));
		return -1;
	}

	return pMarc->DeleteEmptyField();

	EFS_END
	return -1;
}

INT CMarcMgr::DeleteEmptySubfield(CMarc *pMarc)
{
	EFS_BEGIN

	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("pMarc Is NULL"), _T(""));
		return -1;
	}

	return pMarc->DeleteEmptySubfield();

	EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////
// Marc Operation
//////////////////////////////////////////////////////////////////////


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2003.09.09 수정전
//INT CMarcMgr::SortMarc(CMarc* pMarc, INT nOrder /*= SORT_ASC*/)
/*{
	EFS_BEGIN

	FreeErrMsgList();
	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("pMarc is null"), _T(""));
		return -1;
	}

	// 특별한 순서로 정렬하는 TAG LOAD 한다.
	if (ReadSpecialSortInfo() < 0)
		return pMarc->SortMarc(nOrder);
	else
		return pMarc->SortMarc(nOrder, &m_mapSpecialSort);
	

	EFS_END
	return -1;
	
}

*/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2003.09.09 수정이후 

INT CMarcMgr::SortMarc(CMarc* pMarc, INT nOrder /*= SORT_ASC*/ ,BOOL bNewSort /*= FALSE*/ )
{
	EFS_BEGIN

	FreeErrMsgList();
	if (pMarc == NULL)
	{
		AddErrMsg(_T(""), _T("pMarc is null"), _T(""));
		return -1;
	}

	// 특별한 순서로 정렬하는 TAG LOAD 한다.
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.09.08 이학중 수정 
	//
	
	if(bNewSort == FALSE )
	{
		if (ReadSpecialSortInfo() < 0)
			return pMarc->SortMarc(nOrder);
		else
			return pMarc->SortMarc(nOrder, &m_mapSpecialSort);
	}
	else
	{
		if (ReadSpecialSortInfo() < 0)
			return pMarc->SortMarc(nOrder,NULL,TRUE);
		else
			return pMarc->SortMarc(nOrder, &m_mapSpecialSort,TRUE);

	}	
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	EFS_END
	return -1;
	
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




INT CMarcMgr::ReadSpecialSortInfo()
{
	EFS_BEGIN

	FreeSpecialSortInfo();

	CStringArray *pArr = NULL;
	CString strKey;

	CString strSpecialSortInfoFilePath = _T("..\\cfg\\Marc_SpecialSort.cfg");
	CStdioFile file;
	if (!file.Open(strSpecialSortInfoFilePath, CFile::modeRead|CFile::typeBinary)) return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	TCHAR	szLine[128];
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == _T('.') || strLine.Left(1) == _T('#')) continue;

		nFind = strLine.Find(_T('='));
		if (nFind < 0) continue;

		strKey = strLine.Mid(0, nFind);
		strKey.TrimLeft();
		strKey.TrimRight();

		strLine = strLine.Mid(nFind + 1);
		strLine.TrimLeft();
		strLine.TrimRight();

		_stprintf(szLine, _T("%s"), strLine );

		TCHAR *lpszToken = _tcstok(szLine, _T(","));
		if (lpszToken == NULL) continue;
		
		pArr = new CStringArray;

		while (lpszToken)
		{
			pArr->Add(lpszToken);
			lpszToken = _tcstok(NULL, _T(","));
		}

		m_mapSpecialSort.SetAt(strKey, (CObject*)pArr);
	}

	file.Close();

	return 0;

	EFS_END
	return -1;
}


INT CMarcMgr::GetLengthUnicode(INT nCharset, CString strData)
{
	INT		nBytesNeeded;
	INT nWCharSource = strData.GetLength();
	
	
	nBytesNeeded = WideCharToMultiByte( nCharset, 
		0,
		strData, 
		-1,
		NULL, 
		0,
		NULL, 
		NULL);
	
	return nBytesNeeded-1;
}