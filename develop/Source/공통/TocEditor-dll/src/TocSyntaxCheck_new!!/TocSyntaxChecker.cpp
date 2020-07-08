// TocSyntaxChecker.cpp: implementation of the CTocSyntaxChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TocSyntaxChecker.h"
#include "Parse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTocSyntaxChecker::CTocSyntaxChecker(CErrorQueue* pErrorQueue /*= NULL*/, CString sWorkDir/*=_T("")*/, bool bPageCheckOption /*=true*/)
{

	m_TocMetaData.Init(sWorkDir);
	m_pErrorQueue = pErrorQueue;
	m_arrTocTag.SetSize(0,1);
	SetPageCheckOption(bPageCheckOption);
}

CTocSyntaxChecker::~CTocSyntaxChecker()
{
	INT iMaxCount = m_arrTocTag.GetSize();
	CTocTag* pTocTag = NULL;		

	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pTocTag = m_arrTocTag.GetAt(i);
		delete pTocTag;
	}

	m_arrTocTag.RemoveAll();
	m_arrTocTag.FreeExtra();
}

// ================================================================================================================
// Check Toc Syntax
// 입력된 파일로 부터 Syntax Check를 실시한다.
// 1. Toc 파일의 CTocTag Class에 의한 구조화
// 2. CTocTag Class로부터 일괄적으로 Syntax Check 및 Error Queue 구성
// ================================================================================================================
INT CTocSyntaxChecker::CheckSyntax(CString sSrcFileFullPath, BOOLEAN bIsMessageDlg /*= FALSE*/)
{
	sSrcFileFullPath.TrimLeft();
	sSrcFileFullPath.TrimRight();

	if(sSrcFileFullPath.IsEmpty()) return -1;

//	CStdioFile SrcFile(sSrcFileFullPath.GetBuffer(0), CFile::modeRead | CFile::shareDenyWrite | CFile::typeText);
	static TCHAR szBuff[500];
	FILE* fp= NULL;
	fp= FdkFopen(sSrcFileFullPath, _T("rb")) ;
	if( fp==NULL )
	{
		CString sMsg;
		sMsg= _T("Can't find src file");
		sMsg+= sSrcFileFullPath ;
		AfxMessageBox(sMsg);
		return -2;
	}
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

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
	//------------------------------------------	
	
	CString sRecord;
	INT iCurLine = 0;

	// ================================================================================================================
	// 1. Toc 파일의 CTocTag Class에 의한 구조화
	// Toc 원본 파일로부터 1Line씩 Read하여 순차적으로 구조화
	// ================================================================================================================
	CTocTag* pTocFile = NULL;
	CTocTag* pBeforeTocTag = NULL;
	CTocTag* pBaseTocTag = NULL;
	CTocTag* pCurrentTocTag = NULL;
	pTocFile = pBaseTocTag = new CTocTag(m_pErrorQueue,ConvertID2Chinese(IDS_STRING_5003),CTocTagInfo::TAG_TYPE_MULTI,CTocTagInfo::LEVEL0,FALSE);	//_T("본문메타정보")

	CArray<CTocTag*,CTocTag*> m_arrBaseTag;			// Base Tag

	INT iReturnType = 0;

	INT nCountTagValue=-1;
	INT nCountTagLine=0;	INT n_iEndPage=-11;

	CString sToken;	CString sTemp;	CString sPrev;
//	while(SrcFile.ReadString(sRecord))
	while (!feof(fp))
	{
		FdkFgets(szBuff, 499, fp);
		sRecord= szBuff ;
		iCurLine++;
		sRecord.TrimLeft();
		sRecord.TrimRight();

		if(sRecord.IsEmpty()) continue;

		CString sOrigRecord= sRecord;
		// ================================================================================================================
		// Record를 읽어서 _T("<")로 Parsing하여 다음 Tag까지 잘라서 넘겨준다.
		// ================================================================================================================
		while(!sRecord.IsEmpty())
		{
			sToken = GetParsedTocToken(sRecord);

if(!sToken.CompareNoCase(_T("/BODY")))		// body 부분의 페이지수만 체크한다.	added  2002.8.5
{
	CParse tokenParse(sPrev);
	tokenParse.SetDelimeter(_T(","));

	const INT iElementCnt = 3;
	CString sElement[iElementCnt];

	for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
	{
		sElement[i] = tokenParse.NextToken();
		sElement[i].TrimLeft();
		sElement[i].TrimRight();
	}

	INT n_iStartPage = _ttoi(sElement[1]);
	INT n_iLength = _ttoi(sElement[2]);
	n_iEndPage = n_iStartPage + n_iLength;	n_iEndPage--;
}
sPrev=sToken;	sPrev.MakeUpper();

			// count 태그의 값을 알아온다.
			CString sTemp= sToken; sTemp.MakeUpper();
			if (sTemp==_T("COUNT"))
			{
				INT idx= sOrigRecord.Find(_T(">"));
				nCountTagValue= _ttoi(sOrigRecord.Mid(idx+1));
				nCountTagLine= iCurLine;
				//AfxMessageBox(sOrigRecord.Mid(idx+1));
			}

			// ================================================================================================================
			// Tag별로 데이터를 Parsing
			// ================================================================================================================		

			iReturnType = m_TocMetaData.GetTocTag(m_pErrorQueue, iCurLine, sToken, &pCurrentTocTag, pBaseTocTag);

			if(iReturnType < 0) {
				AfxMessageBox(ConvertID2Chinese(IDS_STRING_5004));//_T("오류검사를 수행하던중 시스템 내부에 오류가 발생하였습니다."));
				return -3;
			}
			
			// ================================================================================================================
			// 제어번호를 저장해 둔다.
			// ================================================================================================================
			/*
			if(pCurrentTocTag -> IsControlId()) {
				m_sControlNo = pCurrentTocTag -> GetValue();
			}
			*/

			// 하위 목차를 Link
			// ================================================================================================================
			// Return Type에 따른 처리 방법
			// 정상적이라면 <TAG>와 </TAG>이 번갈아 나타나야 한다.
			// CASE 1 : <TAG> <TAG> </TAG> 
			// ================================================================================================================
			if(iReturnType == CTocMetaData::NEW_BASE_TOC) {
				pBaseTocTag->AddSubLevelToc(pCurrentTocTag);
				m_arrBaseTag.Add(pBaseTocTag);
				pBaseTocTag = pCurrentTocTag;
			} 

			if(iReturnType == CTocMetaData::END_BASE_TOC) {
				INT iMaxCount = m_arrBaseTag.GetSize();
				pBaseTocTag = m_arrBaseTag.GetAt(iMaxCount - 1);
				m_arrBaseTag.RemoveAt(iMaxCount - 1);
			}

			if(iReturnType == CTocMetaData::CURRENT_TOC) {
				pBaseTocTag->AddSubLevelToc(pCurrentTocTag);
				continue;
			}
		}
	}

	m_arrBaseTag.RemoveAll();
	m_arrBaseTag.FreeExtra();

	pTocFile -> CheckTocSyntax();

	INT iStartPage = -1;
	INT iEndPage = -1;
	pTocFile->SetTotalPage(0);
	pTocFile->CheckPage(-1,iStartPage,iEndPage);
	iEndPage = n_iEndPage;//pTocFile->GetTotalPage();	// modified  2002.8.5

	if (nCountTagValue==-1)
	{
		CString msg;
		msg.Format(ConvertID2Chinese(IDS_STRING_5005));//_T("<count>태그가 없습니다."));
		m_pErrorQueue->AddErrorMessage( nCountTagLine, msg, _T(""));
		return -4;
	}

	if (nCountTagValue!= iEndPage && CTocTag::GetPageCheck())
	{
		CString msg;
		msg.Format(ConvertID2Chinese(IDS_STRING_5006), nCountTagValue, iEndPage);	//_T("총페이지수(%d, %d)가 일치하지 않습니다.")
		m_pErrorQueue->AddErrorMessage( nCountTagLine, msg, _T(""));
		return -5;
	}

	return 0;
}

CString CTocSyntaxChecker::GetParsedTocToken(CString& sSource)
{
	// ================================================================================================================
	// Record를 읽어서 _T("<")로 Parsing하여 다음 Tag까지 잘라서 넘겨준다.
	// <PAGE ><TI>afasdf<TI/>
	// ================================================================================================================

	sSource.TrimLeft();
	sSource.TrimRight();

	if(sSource.IsEmpty()) {
		return _T("");
	}

	CString sLeftToken;
	CString sRightToken;

	INT iCurIdx = sSource.Find(_T("<"));
	if(iCurIdx >= 0) {
		sRightToken = sSource.Right(sSource.GetLength() - iCurIdx - 1);
		sRightToken.TrimLeft();
		sRightToken.TrimRight();

		iCurIdx = sRightToken.Find(_T(">"));
		if(iCurIdx >= 0) {
			sLeftToken = sRightToken.Left(iCurIdx);
			sLeftToken.TrimLeft();
			sLeftToken.TrimRight();

			if(m_TocMetaData.IsTocTag(sLeftToken)) {
				// ================================================================================================================
				// 다음_T("<")을 찾는다.
				// ================================================================================================================
				sRightToken = sRightToken.Right(sRightToken.GetLength() - iCurIdx - 1);

				iCurIdx = sRightToken.Find(_T("<"));
				if(iCurIdx >= 0) {
					sSource = sRightToken.Right(sRightToken.GetLength() - iCurIdx);
				} else {
					sSource.Empty();
				}
				
				sLeftToken.TrimLeft();
				sLeftToken.TrimRight();

				iCurIdx = sLeftToken.Find(_T(" "));
				if(iCurIdx >= 0) {
					sLeftToken = sLeftToken.Left(iCurIdx);
					sLeftToken.TrimLeft();
					sLeftToken.TrimRight();
				}
				return sLeftToken;
			} else {
				sLeftToken = sSource;
				sSource.Empty();
				return sLeftToken;
			}
		} else {
			sLeftToken = sSource;
			sSource.Empty();
			return sLeftToken;
		}
	} else {
		sLeftToken = sSource;
		sSource.Empty();
		return sLeftToken;
	}

}

CString CTocSyntaxChecker::GetControlNo()
{
	return m_sControlNo;
}

VOID CTocSyntaxChecker::SetPageCheckOption(bool flag)
{
	CTocTag::SetPageCheck(flag);
}

VOID CTocSyntaxChecker::CheckEssentialTag(CString& sMokcha)
{
/*	// 모든 태크 체크
	INT nCnt= (m_TocMetaData.m_arrTocTagList).GetSize() ;

	CString sTag ;
	CString sErrorMessage ;
	INT nIdx= 0 ;
	for ( INT i=0 ; i<nCnt ; i++ )
	{
		sTag= (m_TocMetaData.m_arrTocTagList).GetAt(i) ;
		if ( sTag.IsEmpty() )	continue ;

		sTag.TrimLeft() ;
		sTag.TrimRight() ;
		sTag= _T("<") + sTag  + _T(">") ;

		nIdx= sMokcha.Find(sTag) ;
		if ( nIdx==0 )	continue ;		// 2002.1.30.	modified 
		else if ( nIdx==1 || nIdx<0 || (sMokcha.Mid(nIdx-2,2 )!=_T("\r\n")) ) {
			sErrorMessage.Format(_T("%s 태그가 없습니다."),sTag);
			(m_pTocFile->m_pErrorQueue) -> AddErrorMessage(m_iCurLine, sErrorMessage, _T(""));
			continue ;
		}
	}
*/

	// id 와 count, body, /body 만 체크
/*	const INT iElementCnt = 4;
	CString sTag[iElementCnt];

	if(m_TocMetaData.m_nIsKindOf == _KISA_) {
		sTag[0].Format(_T("<id>"));
		sTag[1].Format(_T("<vol>"));
		sTag[2].Format(_T("<body>"));
		sTag[3].Format(_T("</body>"));

	} else {
		sTag[0].Format(_T("<id>"));
		sTag[1].Format(_T("<count>"));
		sTag[2].Format(_T("<body>"));
		sTag[3].Format(_T("</body>"));
	}

	CString sErrorMessage;
	INT nIdx= 0 ;
	for ( INT i=0 ; i<iElementCnt ; i++ )
	{
		nIdx= sMokcha.Find(sTag[i]) ;
		if ( nIdx==0 )	continue ;		// 2002.1.30.	modified 
		else if ( nIdx==1 || nIdx<0 || (sMokcha.Mid(nIdx-2,2 )!=_T("\r\n")) ) {
			sErrorMessage.Format(_T("%s 태그가 없습니다.",sTag[i]));
			(m_pTocFile->m_pErrorQueue) -> AddErrorMessage(m_iCurLine, sErrorMessage, _T(""));
			continue ;
		}
	}
*/
}
