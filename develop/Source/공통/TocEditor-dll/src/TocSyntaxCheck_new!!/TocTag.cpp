// TocTag.cpp: implementation of the CTocTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocTag.h"
#include "Parse.h"
//#include _T("TiffFileManger.h")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

INT CTocTag::m_nTotalPage=0;
bool CTocTag::m_bPageCheck=true;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTocTag::CTocTag(	CErrorQueue* pErrorQueue,
					CString sSourceRecord,
					INT iTagType,
					INT iLevel,
					BOOLEAN bIsRepeatTag )
: CTocTagInfo(pErrorQueue, sSourceRecord, iTagType, iLevel, bIsRepeatTag)
{
	m_arrSubLevelTocTag.SetSize(0,1);
}

CTocTag::~CTocTag()
{
	CTocTagInfo* pTocTagInfo = NULL;
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		delete pTocTagInfo;
	}

	m_arrSubLevelTocTag.RemoveAll();
	m_arrSubLevelTocTag.FreeExtra();
}

INT CTocTag::CheckTocSyntax()
{
	// ================================================================================================================
	// Tag에 정보를 기반으로 하여 Syntax Check
	// [Step]
	// 1. Pair Tag인 경우 두 개의 Tag이 모두 존재하는가?
	// 2. 하위 목차 중에서 반복가능하지 않은 Tag이 반복되었는가?
	// 3. 하위 목차 중에서 구조적인 상하위 관계가 맞아떨어지는가?
	// 4. Page 정보일 경우 목차내용의 형식에 부합하는가?
	// 5. Tag왼편에 내용이 입력되어 있지는 않은가? 오른편에 내용이 입력가능한 Tag인가?
	// 6. SubToc에 대한 Syntax검사
	// ================================================================================================================

	INT iErrorLine = -1;
	CString sErrorMessage;
	// 1. Pair Tag인 경우 두 개의 Tag이 모두 존재하는가?
	if(!CheckExistPairTag(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 2. 하위 목차 중에서 반복가능하지 않은 Tag이 반복되었는가?
	if(!CheckRepeatTag(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 3. 하위 목차 중에서 구조적인 상하위 관계가 맞아떨어지는가?
	if(!CheckTagLevel(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 4. Page 정보일 경우 목차내용의 형식에 부합하는가?
	if(!CheckPageInfo(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// 5. Tag왼편에 내용이 입력되어 있지는 않은가? 오른편에 내용이 입력가능한 Tag인가?
	if(!CheckTagSyntax(iErrorLine, sErrorMessage)) {
		m_pErrorQueue -> AddErrorMessage(iErrorLine, sErrorMessage, _T(""));
	}
	// ================================================================================================================
	// 6. SubToc에 대한 Syntax검사
	// ================================================================================================================
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();
	CTocTagInfo* pTocTagInfo = NULL;
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		((CTocTag*)pTocTagInfo)	-> CheckTocSyntax();
	}	
	return 0;
}

// ================================================================================================================
// Pair Tag인 경우 두 개의 Tag이 모두 존재하는가?
// ================================================================================================================
BOOLEAN CTocTag::CheckExistPairTag(INT& iLine, CString& sErrorMessage)
{
	if(m_iLevel == LEVEL0) return TRUE;

	if(m_iTagType == TAG_TYPE_MULTI) {
		// ===========================================================================================================
		// Multi Tag인 경우
		// ===========================================================================================================
		m_sHeadTag.TrimLeft();
		m_sHeadTag.TrimRight();

		m_sTailTag.TrimLeft();
		m_sTailTag.TrimRight();

		if(m_sHeadTag.IsEmpty() || m_sTailTag.IsEmpty()) {
			if(m_sHeadTag.IsEmpty()) {
				iLine = m_iEndLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5007),m_sTailTag.GetBuffer(0));	//_T("<%s>에 대한 짝이 되는 Tag이 존재하지 않습니다.")
			} 

			if(m_sTailTag.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5008),m_sHeadTag.GetBuffer(0));	//_T("<%s>에 대한 짝이 되는 Tag이 존재하지 않습니다.")
			}
			
			return FALSE;
		} else {
			return TRUE;
		}
	} else if(m_iTagType == TAG_TYPE_SINGLE) {
		// ===========================================================================================================
		// Single Tag인 경우 ==> 무조건 TRUE
		// ===========================================================================================================		
		return TRUE;
	} else return TRUE;
}

// ================================================================================================================
// 하위 목차 중에서 반복가능하지 않은 Tag이 반복되었는가?
// ================================================================================================================
BOOLEAN CTocTag::CheckRepeatTag(INT& iLine, CString& sErrorMessage)
{
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();

	if(iMaxCount <= 0) return TRUE;

	if(m_iTagType == TAG_TYPE_PAGEINFO) return TRUE;

	CTocTagInfo* pTocTagInfo		= NULL;
	CTocTagInfo* pLowerTocTagInfo	= NULL;

	BOOLEAN bIsSameTag = FALSE;
	INT iErrorLine = -1;
	for(INT i = 0 ; i < iMaxCount; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		if(pTocTagInfo == NULL) continue;

		if(!pTocTagInfo -> IsRepeatTag()) {
			bIsSameTag = FALSE;
			// ======================================================================================================
			// 반복 가능하지 않은 Tag인 경우 아래에서 Tag이 반복되었는지 체크
			// ======================================================================================================
			for(INT j = i + 1 ; j < iMaxCount ; j++)
			{
				pLowerTocTagInfo = m_arrSubLevelTocTag.GetAt(j);
				if(pLowerTocTagInfo == NULL) continue;

				if(pTocTagInfo -> GetHeadTag() == pLowerTocTagInfo -> GetHeadTag()) {
					bIsSameTag = TRUE;
					iErrorLine = pLowerTocTagInfo -> GetLine();
					break;
				}
			}

			if(bIsSameTag) {
				iLine = iErrorLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5009),pTocTagInfo->GetHeadTag());		//_T("%s TAG는 중복해서 사용할 수 없습니다.")
				return FALSE;
			}
		}
	}

	return TRUE;
}

// ================================================================================================================
// 하위 목차 중에서 구조적인 상하위 관계가 맞아떨어지는가?
// ================================================================================================================
BOOLEAN CTocTag::CheckTagLevel(INT& iLine, CString& sErrorMessage)
{
	INT iMaxCount = m_arrSubLevelTocTag.GetSize();
	
	if(iMaxCount <= 0) return TRUE;

	CTocTagInfo* pTocTagInfo = NULL;
	
	INT iLevel = -1;

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		if(pTocTagInfo == NULL) continue;

		iLevel = pTocTagInfo -> GetLevel();
		if(m_iLevel < iLevel) {
			/*
			iLine = pTocTagInfo -> GetLine();
			sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5010),m_sHeadTag,pTocTagInfo -> GetHeadTag());	//_T("%s TAG은 %s TAG를 포함할 수 없습니다.")
			return FALSE;
			*/
		}
	}

	return TRUE;
}

// ================================================================================================================
// Page 정보일 경우 목차내용의 형식에 부합하는가?
// ================================================================================================================
BOOLEAN CTocTag::CheckPageInfo(INT& iLine, CString& sErrorMessage)
{
	if(m_iLevel == LEVEL0) return TRUE;
	// ================================================================================================================
	// 목차내용 = 책페이지, 스캔페이지, 길이
	// 1. _T("=") 이 존재하는지 체크
	// 2. 목차내용이 존재하는지 체크
	// 3. 목차페이지 정보가 존재하는지 체크
	// 4. 목차페이지 정보 형식 체크
	// ================================================================================================================

	// ====================================================================================================
	// 2001.9.11
	// _T("=") 이하 페이지 정보를 넣지 않아도 편집이 가능하도록 바꿀것이다.
	// 이것을 위해서 옵션을 추가하면 될듯..
	// ====================================================================================================
	if(m_iTagType == TAG_TYPE_PAGEINFO) {
		// ====================================================================================================
		// 페이지 체크를 하는 경우 (default 옵션)
		// ====================================================================================================
		//CString t; t.Format(_T("%d"), m_bPageCheck); AfxMessageBox(t);
		if (m_bPageCheck) 
		{
			CString sIndex;
			// ============================================================================================================
			// 1. _T("=") 이 존재하는지 체크
			// ============================================================================================================
			INT iCurIdx = m_sHeadTag.Find(_T("="));
			if(iCurIdx < 0) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5011),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. '='이 없습니다.")
				return FALSE;
			}
			// ============================================================================================================
			// 2. 목차내용이 존재하는지 체크
			// ============================================================================================================
			sIndex = m_sHeadTag.Left(iCurIdx);
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5012),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. = 좌측에 목차내용이 존재하지 않습니다.")
				return FALSE;
			}

			// ============================================================================================================
			// 3. 목차페이지 정보가 존재하는지 체크
			// ============================================================================================================
			// 나머지에서 또 _T("=")가 존재하는지 체크
			sIndex = m_sHeadTag.Right(m_sHeadTag.GetLength() - iCurIdx - 1);
				 
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.Find(_T("=")) >= 0) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5013),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. = 가 두개 이상 존재합니다.")
				return FALSE;
			}	
			
			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5014),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. = 우측에 페이지 정보가 존재하지 않습니다.")
				return FALSE;
			}

			CParse tokenParse(sIndex);
			tokenParse.SetDelimeter(_T(","));

			const INT iElementCnt = 3;
			CString sElement[iElementCnt];

			CString sElementAlias[iElementCnt] = {
				ConvertID2Chinese(IDS_STRING_5015),//_T("책페이지")	,		// 0
				ConvertID2Chinese(IDS_STRING_5016),//_T("스캔페이지"),		// 1
				ConvertID2Chinese(IDS_STRING_5017)//_T("길이")				// 2
			};

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			for(i = 0 ; i < iElementCnt ; i++)
			{
				if(sElement[i].IsEmpty()) {
					iLine = m_iStartLine;
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5018),iLine,sElementAlias[i]);	//_T("%i행의 목차형식에 오류가 있습니다. 페이지 정보에 %s가 존재하지 않습니다.")
					return FALSE;
				} 
			}

			// =======================================================================================================
			// 
			// =======================================================================================================
			m_iStartPage = _ttoi(sElement[1]);
			m_iLength = _ttoi(sElement[2]);
			m_iEndPage = m_iStartPage + m_iLength;
					
			return TRUE;
		}
		// ====================================================================================================
		// 페이지 체크를 안하는 경우.. 문제 있으면 그냥 0,1,1로 세팅해준다.
		// ====================================================================================================
		else
		{
			CString sIndex;
			// ============================================================================================================
			// 1. _T("=") 이 존재하는지 체크
			// ============================================================================================================
			INT iCurIdx = m_sHeadTag.Find(_T("="));
			if(iCurIdx < 0) {
				//iLine = m_iStartLine;
				//sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5019),iLine);	//_T("%i행의 목차형식에 오류가 있습니다. '='이 없습니다.")
				//return FALSE;
				m_iStartPage = 1;
				m_iLength = 1;
				m_iEndPage = 2;
				return TRUE;
			}
			// ============================================================================================================
			// 2. 목차내용이 존재하는지 체크
			// ============================================================================================================
			sIndex = m_sHeadTag.Left(iCurIdx);
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5020),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. = 좌측에 목차내용이 존재하지 않습니다.")
				return FALSE;
			}

			// ============================================================================================================
			// 3. 목차페이지 정보가 존재하는지 체크
			// ============================================================================================================
			// 나머지에서 또 _T("=")가 존재하는지 체크
			sIndex = m_sHeadTag.Right(m_sHeadTag.GetLength() - iCurIdx - 1);
				 
			sIndex.TrimLeft();
			sIndex.TrimRight();

			if(sIndex.Find(_T("=")) >= 0) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5021),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. = 가 두개 이상 존재합니다.")
				return FALSE;
			}	
			
			if(sIndex.IsEmpty()) {
				iLine = m_iStartLine;
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5022),iLine);		//_T("%i행의 목차형식에 오류가 있습니다. = 우측에 페이지 정보가 존재하지 않습니다.")
				return FALSE;
			}

			CParse tokenParse(sIndex);
			tokenParse.SetDelimeter(_T(","));

			const INT iElementCnt = 3;
			CString sElement[iElementCnt];

			CString sElementAlias[iElementCnt] = {
				ConvertID2Chinese(IDS_STRING_5023),//_T("책페이지")	,		// 0
				ConvertID2Chinese(IDS_STRING_5024),//_T("스캔페이지"),		// 1
				ConvertID2Chinese(IDS_STRING_5025)//_T("길이")				// 2
			};

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			for(i = 0 ; i < iElementCnt ; i++)
			{
				if(sElement[i].IsEmpty()) {
					iLine = m_iStartLine;
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5026),iLine,sElementAlias[i]);	//_T("%i행의 목차형식에 오류가 있습니다. 페이지 정보에 %s가 존재하지 않습니다.")
					return FALSE;
				} 
			}

			m_iStartPage = _ttoi(sElement[1]);
			m_iLength = _ttoi(sElement[2]);
			m_iEndPage = m_iStartPage + m_iLength;
					
			return TRUE;
		}

	} else return TRUE;
}

// ================================================================================================================
// Tag왼편에 내용이 입력되어 있지는 않은가? 오른편에 내용이 입력가능한 Tag인가?
// ================================================================================================================
BOOLEAN CTocTag::CheckTagSyntax(INT& iLine,CString& sErrorMessage)
{
	return TRUE;		
}

VOID CTocTag::AddSubLevelToc(CTocTagInfo *pTocTagInfo)
{
	m_arrSubLevelTocTag.Add(pTocTagInfo);
}

BOOLEAN CTocTag::CheckPage(INT iExpectedEndPage, INT &iStartPage, INT &iEndPage)
{
	if(m_iTagType == TAG_TYPE_SINGLE) return TRUE;

	INT iMaxCount = m_arrSubLevelTocTag.GetSize();

	if(m_iTagType == TAG_TYPE_PAGEINFO) {

		iStartPage = m_iStartPage;
		iEndPage = m_iEndPage;

		if (m_nTotalPage< m_iEndPage-1)
			m_nTotalPage= m_iEndPage-1;

		CString sPage;
		sPage.Format(_T("%i"),m_iStartPage);
		/*
		if(CTiffFileManager::IsValidPage(sPage)) {
			return TRUE;	
		} else {
			return FALSE;
		}
		*/

		return TRUE;
	} 
	
	INT iCurStartPage = -1;
	INT iCurEndPage = -1;

	INT iTempStartPage = 0;
	INT iTempEndPage = 0;

	INT iType = 0;
	INT iErrorLine = -1;
	CString sErrorMessage;

	INT iCurExpectedEndPage = -1;
	// ============================================================================================================
	// 해당 Tag 안에 위치한 Page 정보로 부터 시작 페이지 및 끝 페이지를 얻어온다.
	// ============================================================================================================
	BOOLEAN bIsFirst = TRUE;
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		CTocTagInfo *pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		iType = pTocTagInfo -> GetType();
		if (!pTocTagInfo ->IsPageCheck())
			continue;
			
		if(iType == TAG_TYPE_PAGEINFO) {
			if(((CTocTag*)pTocTagInfo) -> CheckPage(iCurExpectedEndPage, iTempStartPage, iTempEndPage)) {				

				if(bIsFirst) {
					bIsFirst = FALSE;
					iCurStartPage = iTempStartPage;
					iCurEndPage = iTempEndPage;
				} else {
					INT iBuffer = iCurEndPage - iTempStartPage;
					//if(iBuffer == 0 || iBuffer == 1) {  // 2001.6.13. 조건을 느슨하게 고쳤다.
					if(iBuffer <= 1) {
						iCurEndPage = iTempEndPage;	
					} else {
						sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5027),iCurEndPage - 1,iCurEndPage);	//_T("페이지가 %i또는 %i입니다.")
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo -> GetLine(), sErrorMessage, _T(""));	
						continue;
					}
				}
			} else {
				sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5028),iTempStartPage);	//_T("페이지가 %i(은)는 Tiff파일이 존재하지 않습니다.")
				m_pErrorQueue -> AddErrorMessage(pTocTagInfo -> GetLine(), sErrorMessage, _T(""));
				continue;
			}
		}
	}

	// ============================================================================================================
	// 해당 Tag 안에 위치한 하위목차의 페이지 정보를 체크하고 해당 목차 범위안의 포함되어 있는지 체크한다.
	// ============================================================================================================

	iStartPage = iCurStartPage;
	iEndPage = iCurEndPage;

	BOOLEAN bIsReturn = FALSE;

	INT iCurSubTocStartPage = -1;
	INT iCurSubTocEndPage = -1;

	BOOLEAN bIsFirstSubToc = TRUE;

	for(i = 0 ; i < iMaxCount ; i++)
	{
		CTocTagInfo *pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);

		iType = pTocTagInfo -> GetType();
		if (!pTocTagInfo ->IsPageCheck())
			continue;

		if(iType == TAG_TYPE_MULTI) {
			bIsReturn = ((CTocTag*)pTocTagInfo) -> CheckPage(iCurExpectedEndPage, iTempStartPage, iTempEndPage);
			// ======================================================================================================
			// bIsReturn = TRUE  : Error 없이 Page 정보 체크에 성공.
			// bIsReturn = FASLE : Page 정보에 이상있음.
			// ======================================================================================================
			if(bIsReturn) {
				iCurExpectedEndPage = iTempEndPage;

				if(iCurStartPage >= 0 && iCurStartPage > iTempStartPage) {
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5029),pTocTagInfo->GetHeadTag(),GetHeadTag());	//_T("<%s>의 페이지가 <%s>안의 페이지 범위를 벗어났습니다.")
					m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
					return FALSE;
				} else if(iCurEndPage >= 0 && iCurEndPage < iTempEndPage) {
					sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5030),pTocTagInfo->GetHeadTag(),GetHeadTag());	//_T("<%s>의 페이지가 <%s>안의 페이지 범위를 벗어났습니다.")
					m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
					return FALSE;
				} 

				if(bIsFirstSubToc) {
					if (_tcsicmp(pTocTagInfo->GetHeadTag(), _T("figure"))==0 || _tcsicmp(pTocTagInfo->GetHeadTag(), _T("table"))==0 )
						continue;

					bIsFirstSubToc = FALSE;
					iCurSubTocStartPage = iTempStartPage;
					iCurSubTocEndPage = iTempEndPage;

					if(iExpectedEndPage >= 0 && iCurSubTocStartPage - iExpectedEndPage != 0 && iCurSubTocStartPage - iExpectedEndPage != 1) {
						sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5031),pTocTagInfo->GetHeadTag());		//_T("<%s>의 페이지가 상위 페이지 범위를 벗어났습니다.")
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
						return FALSE;
					}
				} else {
					if (_tcsicmp(pTocTagInfo->GetHeadTag(), _T("figure"))==0 || _tcsicmp(pTocTagInfo->GetHeadTag(), _T("table"))==0 )
						continue;

					INT iBuffer = iCurSubTocEndPage - iTempStartPage;
					if(iBuffer != 0 && iBuffer != 1) {
						sErrorMessage.Format(ConvertID2Chinese(IDS_STRING_5032),pTocTagInfo->GetHeadTag());		//_T("<%s>의 페이지가 상위 페이지 범위를 벗어났습니다.")
						m_pErrorQueue -> AddErrorMessage(pTocTagInfo->GetLine(), sErrorMessage, _T(""));
						return FALSE;
					}
					iCurSubTocEndPage = iTempEndPage;
				}

			} else {
				return FALSE;
			}	
		}
	}

	return TRUE;
}

INT CTocTag::GetCountTagValue()
{
	for(INT i = 0 ; i < m_arrSubLevelTocTag.GetSize(); i++)
	{
		CTocTagInfo *pTocTagInfo = m_arrSubLevelTocTag.GetAt(i);
		CString tag= pTocTagInfo->m_sHeadTag;
		tag.MakeUpper();
		if (tag== _T("COUNT"))
			return _ttoi(pTocTagInfo->GetValue());
	}		

	return -1;
}

INT CTocTag::GetTotalPage()
{
	return m_nTotalPage;
}

VOID CTocTag::SetTotalPage(INT page)
{
	m_nTotalPage= page;
}

VOID CTocTag::SetPageCheck(bool flag)
{
	m_bPageCheck= flag;
}

bool CTocTag::GetPageCheck()
{
	return m_bPageCheck;
}
