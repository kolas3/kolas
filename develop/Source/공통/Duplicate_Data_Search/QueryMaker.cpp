// QueryMaker.cpp: implementation of the CQueryMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QueryMaker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define ARTICLE_COUNT 21

TCHAR*   s_pArticle[ARTICLE_COUNT]       = 
{ _T("THE")  , _T("A")     , _T("AN")    , _T("L")     , _T("LE")    ,
_T("LA")   , _T("LES")   , _T("UN")    , _T("UNE")   , _T("DER")   , 
_T("DES")  , _T("DEM")   , _T("DEN")   , _T("DIE")   , _T("DAS")   , 
_T("EIS")  , _T("EINES") , _T("EINEM") , _T("EINEN") , _T("EINER") , 
_T("EINE") };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueryMaker::CQueryMaker(CESL_Mgr * pMgr)
{
	m_pParentMgr = pMgr;
	m_nSearchType = -1;
	m_bSearchKeySetting = FALSE;
	m_nDupSearchType = DUP_SEARCH_TYPE_INVALID;
	m_ArrQuery.RemoveAll();
	m_bOpenbase = FALSE;
}

CQueryMaker::~CQueryMaker()
{
}

INT CQueryMaker::SetSearchKey(CString strFileName)
{
	if(strFileName.GetLength() == 0) return -1000;
	INT ids = m_SearchKey.SetSearchKeys(strFileName);
	if(ids < 0) return ids;
	m_bSearchKeySetting = TRUE;
	return 0;
}

VOID CQueryMaker::DelArticleFromStr( CString sRawStr, CString &sWorkStr)
{
	this->DelArticleFromStr(sRawStr.GetBuffer(0), sWorkStr.GetBuffer(0));
}

VOID CQueryMaker::DelArticleFromStr( TCHAR *f_pRawStr, TCHAR *f_pWorkStr )
{
    TCHAR  s_tmpbuffer[1024],s_ptmpPtr[1024];
	TCHAR *s_pStrPtr;
    INT   i;
    
    _tcscpy( s_tmpbuffer , f_pRawStr );
    

    if( f_pRawStr[0] == '('){
        DelArticle( f_pRawStr , f_pWorkStr );
    }
    else{
        f_pWorkStr[0] = '\0';
        s_pStrPtr = _tcschr( s_tmpbuffer , ' ');
        if( s_pStrPtr != NULL ){
            _tcsnccpy(s_ptmpPtr ,s_tmpbuffer , _tcsclen(s_tmpbuffer) -  _tcsclen( s_pStrPtr ) );
            s_ptmpPtr[_tcsclen(s_tmpbuffer) -  _tcsclen( s_pStrPtr )] = '\0';

            for( i = 0; i < ARTICLE_COUNT ; i++){
                _tcsupr(s_ptmpPtr);
                if( _tcscmp( s_ptmpPtr , s_pArticle[i] ) == 0 ){
                    _tcscpy( f_pWorkStr , &s_pStrPtr[1] );
                    break;
                }
            }
            if( i >= ARTICLE_COUNT ){
                _tcscpy( f_pWorkStr , s_tmpbuffer );
            }
        }
        else{
            _tcscpy( f_pWorkStr , s_tmpbuffer );
        }
    }
}

INT   CQueryMaker::WordHeaderCuttingData(CString in , CString &out) 
{
	return this->WordHeaderCuttingData(in.GetBuffer(0), out.GetBuffer(0));
}

INT   CQueryMaker::WordHeaderCuttingData(TCHAR *in , TCHAR *out) 
{
    TCHAR    tmpData[512], *TokenStr;
    INT     i;

    if ( in == NULL || out == NULL ) return -1;
    _tcscpy( tmpData, in );

    // 영문 단어만 절취한다.
    if ( in[0] & 0x80 ) {
//        _tcscpy( out, in );
        return 0;
    }
    i = 0;
    TokenStr = _tcstok( tmpData, _T(" ") );
    if ( TokenStr == NULL ) {
        _tcscpy( out, _T("") );
        return 0;
    }

    do {
        out[i] = TokenStr[0];
		out[++i] = '*';
        i++;

        TokenStr = _tcstok( NULL, _T(" ") );
        if ( TokenStr == NULL ) break;
		else
			out[i++] = ' ';
    } while(1);

    out[i] = 0;

    return 0;
}

VOID CQueryMaker::DelArticle( TCHAR *f_pRawStr, TCHAR *f_pWorkStr )

{
	TCHAR  s_tmpbuffer[10000];
	TCHAR *s_StrPtr;
	
	_tcscpy( s_tmpbuffer , f_pRawStr );

    f_pWorkStr[0] = '\0';

    if( s_tmpbuffer[0] == '('){
        s_StrPtr = _tcschr( s_tmpbuffer ,')');
        if( s_StrPtr != NULL ){
            _tcscpy( f_pWorkStr , &s_StrPtr[1] );
        }
    }
    else{
        _tcscpy( f_pWorkStr , f_pRawStr );
    }
}

BOOL CQueryMaker::CheckValid()
{
	if(!m_bSearchKeySetting) return FALSE;

	if(m_pParentMgr == NULL) return FALSE;

	if(m_nSearchType < 0) return FALSE;

	if(m_strTableName.IsEmpty()) return FALSE;

	if(m_nDupSearchType == DUP_SEARCH_TYPE_INVALID) return FALSE;

	return TRUE;
}

INT CQueryMaker::GetSearchType(CString searchField)
{
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//	if(searchField == _T("IDX_TITLE") || searchField == _T("IDX_AUTHOR") || searchField == _T("IDX_PUBLISHER") )
	if(searchField == _T("IDX_TITLE") || searchField == _T("IDX_AUTHOR") || searchField == _T("IDX_PUBLISHER") || searchField == _T("IDX_ALL_ITEM"))
	//===================================================	
		return 0;
	else if(searchField == _T("MEDIA_CODE") || searchField == _T("PUB_YEAR") || searchField == _T("IDX_IPUB_YEAR"))
		return 1;
	return 2;
}

INT CQueryMaker::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bUnique)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(bUnique && !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		else if(!bUnique)
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(bUnique && !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	else if(!bUnique)
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CQueryMaker::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
		return FALSE;
}

INT CQueryMaker::GetArticleData(CString &srcStr, CString &articleStr)
{
	//서명 관칭은 서명 앞에 나오는 '(', ')'안의 문자열을 의미한다.
	srcStr.TrimLeft();	srcStr.TrimRight();
	if(srcStr.GetLength() == 0) return -1;
	INT firstidx = srcStr.Find('(');
	INT lastidx = srcStr.Find(')');
	if(firstidx < 0 || firstidx != 0 || lastidx < 0) return 0;
	articleStr = srcStr.Mid(firstidx+1, lastidx - firstidx -1);
	INT len = srcStr.GetLength();
	srcStr = srcStr.Mid(lastidx+1, len-1);
	return 0;
	//return this->GetArticleData(srcStr.GetBuffer(0), articleStr.GetBuffer(0));
}

INT CQueryMaker::GetArticleData(TCHAR *srcStr, TCHAR *articleStr)
{
	if(srcStr[0] == '(')
	{
		for(INT i=1 ; i<(INT)_tcsclen(srcStr) ; i++)
		{
			if(srcStr[i] == ')') break;
			articleStr[i-1] = srcStr[i];
		}
		articleStr[i-1] = '\0';
	}
	return 0;
}
