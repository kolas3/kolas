#include "stdafx.h"
#include "..\..\include\공동목록\CodeDecision.h"
#include "SearchCond.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CString TokenString(CString* str, LPCTSTR tok)
{
    if (!str || str->IsEmpty()) return _T("");

    INT i = str->FindOneOf(tok);

    CString temp;
    if ( i == -1 ) {
        temp = *str;
        str->Empty(); 
    }
    else {
        temp = str->Left(i);
        do {
            *str = str->Right(str->GetLength()-i-1);
            i = str->FindOneOf(tok);
        } while( i == 0 );
    }

    return temp;
}

CCodeDecision::CCodeDecision()
{
}

CCodeDecision::~CCodeDecision()
{
    for (INT i = m_RootArray.GetSize() - 1 ; i >= 0 ; i--) {
        CSearchCond* pElem = (CSearchCond*)m_RootArray[i];
        m_RootArray.RemoveAt(i);
        delete pElem;
    }
}

INT CCodeDecision::Init( TCHAR* file_name )
{
    if ( !ReferenceFileLoad( file_name ) ) return -1;

    CSearchCond::InitSearchValue();

    return 0;
}

VOID CCodeDecision::AddSearchValue( TCHAR* alias, TCHAR* value )
{
    CSearchCond::AddSearchValue( alias, value );
}

INT CCodeDecision::DecideCode( TCHAR* code )
{
    CString sCode;
    for (INT i=0; i < m_RootArray.GetSize(); i++) {
        CSearchCond* pElem = (CSearchCond*)m_RootArray[i];

        sCode = pElem->Search();
        if ( !sCode.IsEmpty() ) {
            _tcscpy( code, (TCHAR*)(LPCTSTR)sCode );
            break;
        }
    }

    if ( sCode.IsEmpty() ) return -100;

    return 0;
}

INT CCodeDecision::DecideCode( TCHAR* code, TCHAR* name )
{
    INT ids;
    ids = DecideCode( code );
    if (ids) return ids;

    CString sName = GetCodeName( code );
    if ( !sName.IsEmpty() ) {
        _tcscpy( name , (TCHAR*)(LPCTSTR)sName );
    }

    return 0;
}

BOOL CCodeDecision::ReferenceFileLoad(LPCTSTR file_name)
{
    CStdioFile file;

    if ( !file.Open( file_name, CFile::modeRead | CFile::typeBinary ) ) {
        CString frm;
        frm.Format(_T("%s Open Error!"), file_name);
        AfxMessageBox( frm );
        return FALSE;
    }
    TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

    CString sToken;
    CString sLine;

    while ( file.ReadString( sLine ) ) {
        sLine.TrimLeft();
        sLine.TrimRight();

        if ( sLine.IsEmpty() ) continue;
        if ( sLine[0] == '.' ) continue;

        sToken = TokenString( &sLine, _T(" =\t") );

        if ( sToken == _T("CODENAME_REF") ) {
            m_sCodeNameRefPath = sLine;
            continue;
        }

        if ( sToken == _T("OUTPUT_ALIAS") ) {
            m_sOutputCodeAlias = sLine;
            continue;
        }

        if ( !SubReferenceFileLoad( sLine ) ) return FALSE;
    };

    return TRUE;
}

BOOL CCodeDecision::SubReferenceFileLoad(LPCTSTR file_name)
{
    CStdioFile file;

    CString sLine;
    CString sToken;

    REF_FILE_DEPTH_INFO DepthInfo[20];
    INT nCurDepth = 0;

    if ( !file.Open( file_name, CFile::modeRead | CFile::typeBinary ) ) {
        CString frm;
        frm.Format(_T("%s Open Error!"), file_name);
        AfxMessageBox( frm );
        return FALSE;
    }
    TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

    while ( file.ReadString( sLine ) ) {
        sLine.TrimLeft();
        sLine.TrimRight();

        if ( sLine.IsEmpty() ) continue;
        if ( sLine[0] == '.' ) continue;

        if ( sLine[0] == '#'  ) {
            DepthInfo[nCurDepth].TypeAliasArray.RemoveAll();
            while ( !sLine.IsEmpty() ) {
                sToken = TokenString( &sLine, _T(" \t") );
                DepthInfo[nCurDepth].TypeAliasArray.Add( sToken.Mid(1) );
            }
            continue;
        }

        DepthInfo[nCurDepth].pSearchCond = new CSearchCond();
        if ( DepthInfo[nCurDepth].pSearchCond == NULL ) {
            AfxMessageBox( _T("memory allocation error") );
            return FALSE;
        }
        if ( nCurDepth == 0 ) {
            m_RootArray.Add( DepthInfo[nCurDepth].pSearchCond );
        }
        else {
            DepthInfo[nCurDepth-1].pSearchCond->AddSubCond( DepthInfo[nCurDepth].pSearchCond );
        }

        BOOL bExistCountType = FALSE;
        for (INT i=0; i < DepthInfo[nCurDepth].TypeAliasArray.GetSize(); i++ ) {
            CString sAlias;

            sToken = TokenString( &sLine, _T(" =\t") );

            sAlias = DepthInfo[nCurDepth].TypeAliasArray[i];
            if      ( sAlias == _T("COUNT") ) {
                DepthInfo[nCurDepth].nRemainCount = _ttoi( sToken );
                bExistCountType = TRUE;
            }
            else if ( sAlias == m_sOutputCodeAlias ) {
                DepthInfo[nCurDepth].pSearchCond->SetOutputValue( &sToken );
            }
            else if ( sAlias != _T("DESC") ) {
                DepthInfo[nCurDepth].pSearchCond->AddCond( &sAlias, &sToken );
            }
        }

        if ( !bExistCountType ) DepthInfo[nCurDepth].nRemainCount = 0;

        INT nNextDepth = nCurDepth;
        if ( nCurDepth > 0 ) DepthInfo[nCurDepth-1].nRemainCount--;
        if ( DepthInfo[nCurDepth].nRemainCount > 0 ) nNextDepth++;
        else  {
            INT index = nCurDepth - 1;
            while ( index >= 0 ) {
                if ( DepthInfo[index].nRemainCount > 0 ) break;
                nNextDepth--;
                index--;
            }
        }
        nCurDepth = nNextDepth;
    }

    return TRUE;
}

CString CCodeDecision::GetCodeName(CString code)
{
    CStdioFile file;
    CString sLine;
    CString sToken;

    if ( !file.Open( m_sCodeNameRefPath, CFile::modeRead | CFile::typeBinary ) ) {
        CString frm;
        frm.Format(_T("%s Open Error!"), m_sCodeNameRefPath);
        AfxMessageBox( frm );
    }
    TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

    while ( file.ReadString( sLine ) ) {
        sToken = TokenString( &sLine, _T(" =\t") );

        if ( sToken == code ) return sLine;
    }

    return _T("");
}

