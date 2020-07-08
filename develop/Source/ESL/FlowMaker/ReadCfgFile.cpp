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
	//++2008.09.29 UPDATE BY CJY {{++
	// 암호화된 접속정보를 복호화한 후 설정
	// 다른 파일의 경우 기존의 로직대로 이용
		
	CString strFile = path.Mid(
		path.ReverseFind('\\')+1,
		path.GetLength() - path.ReverseFind('\\')+1
		);
	strFile.MakeUpper();

	CString info = _T("");
	if(_T("") == path || _T("USEDMINFOCHANGE.CFG") == strFile)
	{
		CString strDMInfo;
		CFWDES DES;		// 암복호화 클래스
		DES.Read_DES_File(	// 접속정보를 복호화하여 읽어온다.
			_T("..\\CFG\\UseDMInfoChange.cfg"),	// 접속정보 파일
			_T("k^Ty"),	// 암호키
			strDMInfo	// 복호화된 문자
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
//		m_sDBKIND = _T("ORACLE");
		m_sSequence = _T("ESL_SEQ");
		m_sUserID = _T("이씨오");
		
		return 0;
	}	
//DEL	이전소스
//DEL 	// 접속정보를 UseDMInfoChange.cfg에서 가져오도록 변경
//DEL 	// path가 있을경우엔 기존의 로직대로 이용
//DEL 	if(_T("") == path)
//DEL 	{
//DEL 		BOOL bDBInfoMode = FALSE;		// TRUE 변환Ｏ, FALSE 변환Ｘ
//DEL 		CStdioFile fd;
//DEL 		CString info = _T(""), strDBInfoBefore = _T(""), strDBInfoAfter = _T("");
//DEL 		if (fd.Open(_T("..\\CFG\\UseDMInfoChange.cfg"), CStdioFile::modeRead|CFile::typeBinary) == TRUE) {
//DEL 			TCHAR cUni;
//DEL 			fd.Read( &cUni, sizeof(TCHAR) );
//DEL 			if( 0xFEFF != cUni )
//DEL 			{
//DEL 				fd.SeekToBegin();
//DEL 			}
//DEL 			CString strLine, strTag, strData;
//DEL 			while(TRUE) {
//DEL 				if (fd.ReadString(strLine) == FALSE) break;
//DEL 				strLine.TrimLeft();				strLine.TrimRight();
//DEL 				if (strLine.GetLength() == 0) continue;
//DEL 				
//DEL 				INT nPos = strLine.Find('=', 0);
//DEL 				if (nPos == -1) continue;
//DEL 				
//DEL 				strTag = strLine.Left(nPos);
//DEL 				strTag.MakeUpper();			strTag.TrimLeft();		strTag.TrimRight();
//DEL 				strData = strLine.Mid(nPos+1);
//DEL 				strData.TrimLeft();			strData.TrimRight();
//DEL 				
//DEL 				if (strTag == _T("MODE")) {
//DEL 					strData.MakeUpper();
//DEL 					if (strData == _T("TRUE")) bDBInfoMode = TRUE;
//DEL 					else bDBInfoMode = FALSE;
//DEL 				}
//DEL 				if (strTag == _T("BEFORE")) strDBInfoBefore = strData;
//DEL 				if (strTag == _T("AFTER")) strDBInfoAfter = strData;
//DEL 			}
//DEL 			fd.Close();
//DEL 			if (strDBInfoBefore.GetLength() == 0) bDBInfoMode = FALSE;
//DEL 			if (strDBInfoAfter.GetLength() == 0) bDBInfoMode = FALSE;
//DEL 		}
//DEL 		if (bDBInfoMode == TRUE) 
//DEL 		{
//DEL 			strDBInfoAfter.TrimLeft();		strDBInfoAfter.TrimRight();
//DEL 			strDBInfoAfter.MakeLower();
//DEL 			info = strDBInfoAfter;
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			strDBInfoBefore.TrimLeft();		strDBInfoBefore.TrimRight();
//DEL 			strDBInfoBefore.MakeLower();
//DEL 			info = strDBInfoBefore;
//DEL 		}
//DEL 		m_sConnection = info;
//DEL 		m_sDBKIND = _T("ORACLE");
//DEL 		m_sSequence = _T("ESL_SEQ");
//DEL 		m_sUserID = _T("이씨오");
//DEL 
//DEL 		return 0;
//DEL 	}
	//--2008.09.29 UPDATE BY CJY --}}
		
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
		}
//		else if ( _tcscmp(name, _T("DB")) == 0 ) {
//			m_sDBKIND = value;
//		}
		
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

	// 처음의 공백과 tab을 제거한다
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

	// token의 끝을 찾는다
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
                // Brace일때는 ' ', '\t'대신 _T("가 종료조건이다
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

