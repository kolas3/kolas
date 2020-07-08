// ReadCfgFile.cpp: implementation of the CReadCfgFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReadCfgFile.h"
#include "FWDES.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReadCfgFile::CReadCfgFile()
{
	m_sUserID = _T("");
	m_sSequence = _T("");
	m_sConnection = _T("");
}

CReadCfgFile::~CReadCfgFile()
{

}

INT CReadCfgFile::ReadCfgFile(CString path)
{		
	CString strFile = path.Mid(
		path.ReverseFind('\\')+1,
		path.GetLength() - path.ReverseFind('\\')+1
		);
	strFile.MakeUpper();

	CString info = _T("");
	if(_T("") == path || _T("USEDMINFOCHANGE.CFG") == strFile)
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
		if(_T("TRUE") == strMode)
		{
			info = strDBInfoAfter;
		}
		else if(_T("FALSE") == strMode)
		{
			info = strDBInfoBefore;
		}
		
		m_sConnection = info;
		m_sDBKIND = _T("ORACLE");
		m_sSequence = _T("ESL_SEQ");
		m_sUserID = _T("이씨오");
		
		return 0;
	}	
		
	if (path.GetLength() == 0) {
#ifdef _UNICODE
		path = _T("..\\cfg\\RefEditor\\refeditor_uni.cfg");
#else
		path = _T("..\\cfg\\RefEditor\\refeditor_mbcs.cfg");
#endif
	}
	
	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) {
		path += _T(" 파일이 존재하지 않습니다.");
		AfxMessageBox(path, MB_ICONSTOP);
		return -1;
	}
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	_TCHAR szCfg[1000];
	
	INT nRead = file.Read(szCfg, 1000);
	
#ifdef _UNICODE
	nRead /= 2;
#endif 
	
	szCfg[nRead] = NULL;
	file.Close();
	
	if ( _tcslen(szCfg) == 0 ) {
		file.Close();
		return -1;
	}
	
	_TCHAR name[64];
	_TCHAR value[256];
	INT pos;
	
	const _TCHAR seps[] = _T("\n");
	_TCHAR* token;
	token = _tcstok(szCfg, seps);
	
	if ( token[0] == NULL ) return 0;
	
	while ( token != NULL ) 
	{
		name[0] = NULL;
		value[0] = NULL;
		pos = 0;
		
		if ( token[0] == _T('.') ) 
		{
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;
			
		} 
		else if ( token[0] == _T('\r') ) 
		{
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;
		}
		
		pos = GetToken(token, pos, name);
		pos = GetToken(token, pos, value);
		
		if ( _tcscmp(name, _T("ID")) == 0 ) {
			m_sUserID = value;
		} else if ( _tcscmp(name, _T("HOST")) == 0 ) {
			m_sConnection = value;
		} else if ( _tcscmp(name, _T("SEQ")) == 0 ) {
			m_sSequence = value;
		} else if ( _tcscmp(name, _T("DB")) == 0 ) {
			m_sDBKIND = value;
		}
		
		token = _tcstok(NULL, seps);
	}
	
	return 0;
}

INT CReadCfgFile::GetToken(_TCHAR* line, INT pos, _TCHAR* token)
{
	if ( pos < 0 ) {
		token[0] = NULL;
		return -1;
	} else if ( line[pos] == _T('\n') ) {
		token[0] = NULL;
		return -1;
	}

	INT end;

	while ( true ) {
		if ( line[pos] == _T(' ') ) {

		} else if ( line[pos] == _T('\t') ) {

		} else if ( line[pos] == _T('\r') ) {
			token[0] = NULL;
			return -1;

		} else if ( line[pos] == NULL ) {
			token[0] = NULL;
			return -1;
		
		} else {
			break;
		}
		pos++;
	}

    bool bBrace;
    if ( line[pos] == _T('"') ) {
        bBrace = true;
	    pos++;
    } else {
        bBrace = false;
    }
    end = pos;


	while ( true ) {
        if ( bBrace ) {
            if ( line[end] == _T('"') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == _T('\r') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == NULL ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;
		    }

        } else {
		    if ( line[end] == _T(' ') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == _T('\t') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == _T('\r') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == NULL ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;
		    }
        }
		end++;
	}

	return end+1;
}

