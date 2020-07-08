// TocMetaData.cpp: implementation of the CTocMetaData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocMetaData.h"
#include "TocTag.h"
#include "TocParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
INT CTocMetaData::NEW_BASE_TOC = 0;			// 0
INT CTocMetaData::CURRENT_TOC  = 1;			// 1
INT CTocMetaData::END_BASE_TOC = 2;			// 2

CTocMetaData::CTocMetaData()
{
	m_nIsKindOf= _DAN_SINGLE ;
}

VOID CTocMetaData::Init(CString sWorkingDir, INT nIsKindOf/*=_DAN_SINGLE*/)
{
	sWorkingDir.TrimLeft();
	sWorkingDir.TrimRight(_T("\\"));
	sWorkingDir+=_T("\\");
//	CString metafile= sWorkingDir+sMetaFileName;
	CString metafile; //= sWorkingDir+sMetaFileName;

	switch(nIsKindOf) {
	case _DAN_SINGLE:
		metafile.Format(_T("%s%s"), sWorkingDir, sMetaFileDan);
		break;

	case _KISA_:
		metafile.Format(_T("%s%s"), sWorkingDir, sMetaFileKisa);
		break;

	default:
	case _YON_:
		metafile.Format(_T("%s%s"), sWorkingDir, sMetaFileYon);
		break;

	}

	CString tagfile= sWorkingDir+sTagListFileName;

	CFileException ex;
	if( ! m_MetaCfgFile.Open(metafile,CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary, &ex) ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString strError ;
		strError += _T("Couldn't open source file:\r\n\r\n");
		strError += szError;
		AfxMessageBox(strError) ;
		return ;
	}
	TCHAR cUni;
	m_MetaCfgFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_MetaCfgFile.SeekToBegin();
	}
	if( ! m_TagListFile.Open(tagfile,CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary, &ex) ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString strError ;
		strError += _T("Couldn't open source file:\r\n\r\n");
		strError += szError;
		AfxMessageBox(strError) ;
		return ;
	}
	TCHAR cUni;
	m_TagListFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_TagListFile.SeekToBegin();
	}

	m_arrTocTagList.SetSize(0,1);
	// =================================================================================================
	// Tag List로부터 Tag List을 Init한다.
	// =================================================================================================

	m_nIsKindOf= nIsKindOf ;

	CString sRecord;
	while(m_TagListFile.ReadString(sRecord))
	{
		if(sRecord.IsEmpty()) continue;

		sRecord.TrimLeft();
		sRecord.TrimRight();

		if (m_nIsKindOf==_DAN_SINGLE) {			// 단행본-단권
			if( sRecord!=_T(".단행본-단권") )	continue ;
			break ;
		}
		else if (m_nIsKindOf==_DAN_MULTI) {	// 단행본-다권
			if( sRecord!=_T(".단행본-다권") )	continue ;
			break ;
		}
		else if (m_nIsKindOf==_YON_) {	// 연속간행물
			if( sRecord!=_T(".연속간행물") )	continue ;
			break ;
		}
		else if (m_nIsKindOf==_KISA_) {	// 연속간행물
			if( sRecord!=_T(".기사물") )	continue ;
			break ;
		}
	}

	while ( m_TagListFile.ReadString(sRecord) && (sRecord.Mid(0,1)!=_T("."))) {
		if(sRecord.IsEmpty()) continue;
		m_arrTocTagList.Add(sRecord);
	}

}

CTocMetaData::~CTocMetaData()
{
	m_arrTocTagList.RemoveAll();
	m_arrTocTagList.FreeExtra();
}

// ========================================================================================================
// 원본 레코드를 입력받아서 Tag을 Parsing, 적절한 CTocTag을 생성해준다.
// ========================================================================================================

INT CTocMetaData::GetTocTag(CErrorQueue* pErrorQueue, INT iLine, CString sSourceRecord, CTocTag** pNewTocTag, CTocTag* pBaseTocTag /*= NULL*/)
{
	// =================================================================================================
	// Source Record으로 부터 Tag을 분리하고 동시에 Tag정보인지 Page 정보인지를 판별한다.
	// =================================================================================================
	INT iType = IsTocTag(sSourceRecord) ? CTocTagInfo::TAG_TYPE_MULTI : CTocTagInfo::TAG_TYPE_PAGEINFO;
			
	INT iStartLine		= -1;
	INT iEndLine		= -1;

	CString sHeadTag	= _T("");
	CString sTailTag	= _T("");
	INT iTagType		= CTocTagInfo::TAG_TYPE_PAGEINFO;
	INT iLevel			= CTocTagInfo::LEVEL5;
	BOOLEAN bIsRepeatTag= TRUE;	

	INT iReturn = -1;
	CTocTag* pTocTagInfo = NULL;
	if(iType != CTocTagInfo::TAG_TYPE_PAGEINFO) {
		// =================================================================================================
		// Tag인 경우
		// =================================================================================================
		CString sRecord;
		CString sToken;
		TCHAR cUni;
		m_MetaCfgFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			m_MetaCfgFile.SeekToBegin();
		}
		while(m_MetaCfgFile.ReadString(sRecord)) 
		{
			sRecord.TrimLeft();
			sRecord.TrimRight();

			if(sRecord.IsEmpty()) continue;

			// =================================================================================================
			// 주석 제거
			// =================================================================================================
			INT iCurIdx = sRecord.Find(_T(".."));
			if(iCurIdx >= 0) {
				sRecord = sRecord.Left(iCurIdx);
				sRecord.TrimLeft();
				sRecord.TrimRight();

				if(sRecord.IsEmpty()) {
					continue;
				}
			}

			// =================================================================================================
			// 레코드 형식 [TAG] | [TAGTYPE (0 ~ 2) ] | [LEVEL (0 ~ 5)] | [ISREPEATTAG(Y or N)]
			// =================================================================================================
			sHeadTag.Empty();
			sTailTag.Empty();

			CParse tokenParse(sRecord.GetBuffer(0));
			tokenParse.SetDelimeter(_T("|"));

			const INT iElementCnt = 4;
			CString sElement[iElementCnt];

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			sHeadTag = sElement[0];
			sTailTag = _T("/")+sElement[0];

			sHeadTag.MakeLower();
			sTailTag.MakeLower();
			sSourceRecord.MakeLower();

			iTagType	 = _ttoi(sElement[1]);
			iLevel		 = _ttoi(sElement[2]);
			bIsRepeatTag = sElement[3] == _T("Y") ? TRUE : FALSE;

			if(sHeadTag.Compare(sSourceRecord) == 0) {
				// =================================================================================================
				// 해당 Tag일 경우 메타 정보 Setting
				// =================================================================================================
				sHeadTag	= sSourceRecord;
				iStartLine	= iLine;
				
				pTocTagInfo	 = new CTocTag(pErrorQueue,sSourceRecord, iTagType, iLevel, bIsRepeatTag);
				pTocTagInfo -> SetHeadTag(iStartLine, sHeadTag);
				if(iTagType == CTocTagInfo::TAG_TYPE_SINGLE) 
					iReturn		 = CURRENT_TOC;
				else
					iReturn		 = NEW_BASE_TOC;

				break;
			} else if(sTailTag.Compare(sSourceRecord) == 0) {
				// =================================================================================================
				// </...>의 형식 Tag일 경우 기존 상위 TocTag에 데이터를 넣는다.
				// =================================================================================================				
				if(pBaseTocTag -> IsCurrentPairTag(sTailTag)) {
					pBaseTocTag -> SetTailTag(iLine,sTailTag);
					iReturn	= END_BASE_TOC;
				} else {
					pTocTagInfo	 = new CTocTag(pErrorQueue,sSourceRecord, iTagType, iLevel, bIsRepeatTag);
					pTocTagInfo -> SetTailTag(iLine,sTailTag);
					iReturn		 = NEW_BASE_TOC;
				}

				break;
			}
		}
	} else {
		// =================================================================================================
		// Page인 경우
		// =================================================================================================
		pTocTagInfo = new CTocTag(  pErrorQueue,
									sSourceRecord, 
		    						CTocTagInfo::TAG_TYPE_PAGEINFO, 
									CTocTagInfo::LEVEL5, 
									TRUE );
		pTocTagInfo -> SetHeadTag(iLine,sSourceRecord);
		iReturn		 = CURRENT_TOC;
	}

	*pNewTocTag = pTocTagInfo;
	return iReturn;
}

// ===================================================================================================
// 원본 파일로부터 Tag을 분리
// [Return]
// ===================================================================================================
INT CTocMetaData::ParseTocTag(CString& sSourceRecord, CString &sTag)
{
	// ===================================================================================================
	// [STEP]
	// 1. _T("<")를 Scan
	// 2. _T(">")를 Scan
	// 3. TagList에서 Tag을 Return;
	// ===================================================================================================
	sSourceRecord.TrimLeft() ;
	sSourceRecord.TrimRight() ;

	INT iCurIdx = sSourceRecord.Find(_T("<"));
	INT iStartIdx = iCurIdx;

	CString sLeftToken;
	CString sRightToken;
	if(iCurIdx == 0) {
		sRightToken = sSourceRecord.Right(sSourceRecord.GetLength() - iCurIdx - 1);

		if(sRightToken.IsEmpty()) return -1;

		iCurIdx = sRightToken.Find(_T(">"));
		if(iCurIdx < 0) return -2;

		sLeftToken = sRightToken.Left(iCurIdx);
//		sLeftToken.TrimLeft();
//		sLeftToken.TrimRight();

		sRightToken = sRightToken.Right(sRightToken.GetLength() - iCurIdx - 1);
		
		INT iMaxCount = m_arrTocTagList.GetSize();
		for(INT i = 0 ; i < iMaxCount ; i++)
		{
//			sLeftToken.MakeLower();		// 소문자만 가능하도록 해주자!
			if(sLeftToken.Compare(m_arrTocTagList.GetAt(i)) == 0) {
				sTag = sLeftToken;
				sSourceRecord = sRightToken;
				return iStartIdx;
			}
		}
	}

	return -3;
}

CString CTocMetaData::GetParsedToc(CString &sSourcRecord, CString sTag)
{
	return _T("");
}

BOOLEAN CTocMetaData::IsTocTag(CString sToken)
{
	INT iMaxCount = m_arrTocTagList.GetSize();

//	sToken.MakeLower();		// 소문자만 가능하도록 해주자!
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		if(sToken.Compare(m_arrTocTagList.GetAt(i)) == 0) {
			return TRUE;
		}
	}

	return FALSE;
}

INT CTocMetaData::GetTagCount()
{
	return m_arrTocTagList.GetSize();
}

CString CTocMetaData::GetTag(INT iIdx)
{
	return m_arrTocTagList.GetAt(iIdx);
}
