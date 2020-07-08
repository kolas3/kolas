#include "stdafx.h"
#include "SearchCond.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CString TokenString(CString* str, LPCTSTR tok);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearchCond::CSearchCond()
{
}

CSearchCond::~CSearchCond()
{
    INT i;

    for(i = m_CondArray.GetSize() - 1 ; i >= 0 ; i--) {
		COND_ELEM* pElem= (COND_ELEM*)m_CondArray[i];
		m_CondArray.RemoveAt(i);
		delete pElem;
	}

    for(i = m_SubCondArray.GetSize() - 1 ; i >= 0 ; i--) {
		CSearchCond* pElem= (CSearchCond*)m_SubCondArray[i];
		m_SubCondArray.RemoveAt(i);
		delete pElem;
	}
}

CMap <CString, LPCTSTR, CString, LPCTSTR> CSearchCond::m_InputValue;

VOID CSearchCond::InitSearchValue()
{
    m_InputValue.RemoveAll();
}

VOID CSearchCond::AddSearchValue(TCHAR* alias, TCHAR* value)
{
    m_InputValue.SetAt( alias , value );
}

INT CSearchCond::AddCond(CString* alias, CString* value)
{
    COND_ELEM* pElem = new COND_ELEM;

    _tcscpy( pElem->sAlias, alias->GetBuffer(0) );
    _tcscpy( pElem->sValue, value->GetBuffer(0) );

    m_CondArray.Add( pElem );

    return 0;
}

INT CSearchCond::AddSubCond(CSearchCond* cond)
{
    m_SubCondArray.Add( cond );

    return 0;
}

VOID CSearchCond::SetOutputValue(CString* value)
{
    m_sCode = *value;
}

CString CSearchCond::Search()
{
    BOOL bCatched = FALSE;
    INT i;

    for(i=0; i < m_CondArray.GetSize(); i++ ) {
        COND_ELEM* pElem= (COND_ELEM*)m_CondArray[i];
        bCatched = CompareData( pElem );
        if ( !bCatched ) return _T("");
    }

    if ( bCatched) {
        if ( m_SubCondArray.GetSize() == 0 && !m_sCode.IsEmpty() ) return m_sCode;
    }

    CString sOutput;
    for(i=0; i < m_SubCondArray.GetSize(); i++ ) {
        CSearchCond* pElem = (CSearchCond*)m_SubCondArray[i];
        sOutput = pElem->Search();
        if ( !sOutput.IsEmpty() ) return sOutput;
    }

    return _T("");
}

BOOL CSearchCond::CompareData(COND_ELEM* pElem)
{
    CString sValue;

    if ( _tcscmp( pElem->sAlias, _T("원/복본")      ) == 0 ) {
        if ( !m_InputValue.Lookup( pElem->sAlias, sValue ) ) return FALSE;

        if      ( _tcscmp( pElem->sValue, _T("원본") ) == 0 && sValue.IsEmpty() ) return TRUE;
        else if ( _tcscmp( pElem->sValue, _T("복본") ) == 0 && !sValue.IsEmpty() ) return TRUE;
    }
    else if ( _tcscmp( pElem->sAlias, _T("년도(F)")      ) == 0 ) {
        if ( !m_InputValue.Lookup( _T("년도"), sValue ) ) return FALSE;

        INT nYear1 = _ttoi( pElem->sValue );
        INT nYear2 = _ttoi( sValue );
        if ( nYear1 <= nYear2 ) return TRUE;
    }
    else if ( _tcscmp( pElem->sAlias, _T("년도(T)")      ) == 0 ) {
        if ( !m_InputValue.Lookup( _T("년도"), sValue ) ) return FALSE;

        INT nYear1 = _ttoi( pElem->sValue );
        INT nYear2 = _ttoi( sValue );
        if ( nYear1 > nYear2 ) return TRUE;
    }
    else if ( _tcscmp( pElem->sAlias, _T("분류기호(F)")  ) == 0 ) {
        if ( !m_InputValue.Lookup( _T("분류기호"), sValue ) ) return FALSE;

        if ( sValue.Compare( pElem->sValue ) >= 0 ) return TRUE;
    }
    else if ( _tcscmp( pElem->sAlias, _T("분류기호(T)")  ) == 0 ) {
        if ( !m_InputValue.Lookup( _T("분류기호"), sValue ) ) return FALSE;

        if ( sValue.Compare( pElem->sValue ) < 0 ) return TRUE;
    }
    else if ( m_InputValue.Lookup( pElem->sAlias, sValue ) ) {
        if ( CompareData( pElem->sValue, (TCHAR*)(LPCTSTR)sValue ) ) return TRUE;
    }

    return FALSE;
}

BOOL CSearchCond::CompareData(TCHAR* cond, TCHAR* input)
{
    CString sData = cond;
    CString sToken;
    BOOL bRet  = FALSE;
    BOOL bReverse = FALSE;

    if ( sData == _T("기타") ) return TRUE;
    if ( sData == _T("있음") && *input != NULL ) return TRUE;
    if ( sData == _T("없음") && *input == NULL ) return TRUE;

    if ( sData[0] == '!' ) {
        TokenString( &sData, _T(" !\t") );
        bReverse = TRUE;
    }

    bRet = FALSE;
    while(1) {
        sToken = TokenString( &sData, _T(" ,\t") );
        if ( sToken.IsEmpty() ) break;

        if ( sToken.Compare( input ) == 0 ) {
            bRet = TRUE;
            break;
        }
    }

    if ( bReverse ) bRet = !bRet;

    return bRet;
}

